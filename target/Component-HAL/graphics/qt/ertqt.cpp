// ertqt.cpp

#include "ertqt.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtCore/QTimer>
#include <QtCore/QVariant>
#include <QtCore/QMetaObject>
#include <QtCore/QDebug>
#include <QtCore/qglobal.h>
#include <QtCore/QSignalMapper>

#include <vector>
#include <string>
#include <mutex>
#include <cstring>

namespace
{

struct ObjectRecord
{
    QObject * ptr = nullptr;
    std::string name;
};

static QGuiApplication * g_app = nullptr;
static QQmlApplicationEngine * g_engine = nullptr;

static std::vector<ObjectRecord> g_objects;
static std::mutex g_objects_mutex;

static ertqt_tick_callback g_tick_cb = nullptr;
static void * g_tick_user_data = nullptr;
static unsigned int g_tick_interval_ms = 0;

static bool g_initialised = false;

/* Helper to translate a handle to QObject * safely */
// Internal helper for resolving an ertqt_object_handle into a QObject pointer.
//
// This function validates the supplied handle against the current object table,
// and returns the corresponding QObject pointer if it is in range and non NULL.
//
// Parameters:
// - h: Opaque handle value previously returned by the public API.
//
// Returns:
// - A valid QObject * pointer if the handle is in range and refers to a live object.
// - nullptr if the handle is out of range or the slot does not currently hold a QObject.
//
// Notes:
// - Callers should treat the returned pointer as transient. The mapping may be
//   rebuilt by rebuild_object_table(), which can invalidate previously observed
//   QObject pointers.
// - All access is serialised via g_objects_mutex to avoid concurrent modification.
//
static QObject * handle_to_qobject(ertqt_object_handle h)
{
    if (h == 0)  // Basic null check
    {
        return nullptr;
    }
    return reinterpret_cast<QObject*>(h);
}

/* Rebuild the object table from the engine's root objects */
// Internal helper for rebuilding the global object handle table from the QML engine.
//
// This function clears the current g_objects table and repopulates it using the
// QML engine's root objects and all of their QObject children discovered via
// findChildren(). Each entry stores a raw QObject pointer and a cached copy of
// its objectName().
//
// Parameters:
// - (none)
//
// Returns:
// - (none) The function operates on the global g_objects vector.
//
// Notes:
// - If g_engine is nullptr, the function performs no work.
// - The order of entries is not specified but is stable for a given pass. Handles
//   are simple indices into this vector.
// - Existing handles may become invalid after this function runs. Public API
//   functions are responsible for detecting invalid handles and returning an
//   appropriate error code.
//
static void rebuild_object_table()
{
    std::lock_guard<std::mutex> lock(g_objects_mutex);
    g_objects.clear();

    if (!g_engine)
    {
        return;
    }

    const QList<QObject *> roots = g_engine->rootObjects();
    for (QObject *root : roots)
    {
        if (!root)
        {
            continue;
        }
        // Include root
        ObjectRecord rec;
        rec.ptr = root;
        rec.name = root->objectName().toStdString();
        g_objects.push_back(rec);

        // Include all children
        const QList<QObject *> children = root->findChildren<QObject *>();
        for (QObject *child : children)
        {
            if (!child)
            {
                continue;
            }
            ObjectRecord crec;
            crec.ptr = child;
            crec.name = child->objectName().toStdString();
            g_objects.push_back(crec);
        }
    }
}

/* Tick driver: called by a QTimer when enabled */
// Internal helper used as the QTimer callback for driving the registered tick callback.
//
// This function is scheduled by ertqt_set_tick_callback() using QTimer::singleShot
// when a non zero interval and tick callback have been configured. It invokes the
// user supplied tick callback and, if the interval is still non zero, re-arms the
// timer for the next invocation.
//
// Parameters:
// - (none)
//
// Returns:
// - (none)
//
// Notes:
// - This function is always executed on the Qt application's thread. The user
//   supplied callback must therefore avoid long blocking operations to keep the
//   GUI responsive.
// - The tick chain is stopped by either clearing g_tick_cb or setting the interval
//   to zero via a subsequent call to ertqt_set_tick_callback().
//
static void tick_timer_fired()
{
    if (g_tick_cb)
    {
        g_tick_cb(g_tick_user_data);
    }

    // Re arm timer if interval is non zero
    if (g_tick_interval_ms > 0)
    {
        QTimer::singleShot(static_cast<int>(g_tick_interval_ms), &tick_timer_fired);
    }
}

} // anonymous namespace

extern "C"
{

// Public interface for initialising the Qt integration and loading the QML file.
//
// This function creates the Qt application instance if none exists yet, sets up
// the QML engine, and loads the specified QML file. It also builds the initial
// internal object table used to resolve ertqt_object_handle values.
//
// Parameters:
// - qml_path: Path to the QML file to load, encoded as UTF 8. Must not be NULL.
// - argc: Application argument count, usually the same as main() received. If zero,
//         a dummy argument list is constructed internally.
// - argv: Application argument vector. If NULL while argc is non zero, behaviour is
//         undefined. If both argc and argv indicate "no arguments", a dummy argument
//         list containing a single placeholder is used.
//
// Returns:
// - ERTQT_OK on successful initialisation and QML loading.
// - ERTQT_ERR_INVALID_ARGUMENT if qml_path is NULL.
// - ERTQT_ERR_BACKEND_FAILURE if a Qt application instance already exists, the Qt
//   application or QML engine could not be created, or the QML file failed to
//   produce at least one root object.
//
// Notes:
// - This function does not start the Qt event loop. Call ertqt_run() after a
//   successful return value to begin processing events.
// - Repeated calls after a successful initialisation are treated as no-ops and
//   return ERTQT_OK without reloading QML.
//
ertqt_status ertqt_init(const char * qml_path, int argc, char ** argv)
{
    if (g_initialised)
    {
        return ERTQT_OK;
    }

    if (!qml_path)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    if (QCoreApplication::instance())
    {
        // Application already exists, this would complicate things
        return ERTQT_ERR_BACKEND_FAILURE;
    }

    int local_argc = argc;
    char ** local_argv = argv;

    if (local_argc == 0 || !local_argv)
    {
        // Provide a dummy argv if caller did not
        static char app_name[] = "ertqt_app";
        static char * dummy_argv[] = { app_name, nullptr };
        local_argc = 1;
        local_argv = dummy_argv;
    }

    try
    {
        g_app = new QGuiApplication(local_argc, local_argv);
        g_engine = new QQmlApplicationEngine();

        QObject::connect(g_engine, &QQmlApplicationEngine::objectCreated, g_engine, [](QObject *obj, const QUrl &url)
        {
            Q_UNUSED(url);
            if (!obj)
            {
                qWarning() << "Root object creation failed";
                return;
            }
            rebuild_object_table();
        });

        g_engine->load(QUrl::fromLocalFile(QString::fromUtf8(qml_path)));

        if (g_engine->rootObjects().isEmpty())
        {
            return ERTQT_ERR_BACKEND_FAILURE;
        }

        rebuild_object_table();
        g_initialised = true;
        return ERTQT_OK;
    }
    catch (...)
    {
        return ERTQT_ERR_BACKEND_FAILURE;
    }
}

// Public interface for entering the Qt event loop.
//
// This function runs the Qt application's main event loop, which processes GUI
// events, timers and other queued work. On typical desktop platforms it will
// only return when the application is quitting.
//
// Parameters:
// - (none)
//
// Returns:
// - ERTQT_OK if the event loop exited with a zero return code.
// - ERTQT_ERR_GENERIC if Qt has not been initialised via ertqt_init() or the
//   global application pointer is NULL.
// - ERTQT_ERR_BACKEND_FAILURE if the Qt event loop returns a non zero exit code.
//
// Notes:
// - This call is blocking. It should usually be the final call made by the
//   caller during start-up, after all bindings and callbacks have been set up.
// - The event loop can be terminated programmatically by calling ertqt_quit(),
//   or by any other mechanism that causes QGuiApplication::quit() to be invoked.
//
ertqt_status ertqt_run(void)
{
    if (!g_initialised || !g_app)
    {
        return ERTQT_ERR_GENERIC;
    }

    const int rc = g_app->exec();
    return rc == 0 ? ERTQT_OK : ERTQT_ERR_BACKEND_FAILURE;
}

// Public interface for requesting termination of the Qt event loop.
//
// This function requests that the Qt application exits its event loop by calling
// QGuiApplication::quit() on the global application instance.
//
// Parameters:
// - (none)
//
// Returns:
// - ERTQT_OK if the quit request was issued successfully.
// - ERTQT_ERR_GENERIC if the global application pointer is NULL, which indicates
//   that ertqt_init() has not completed successfully.
//
// Notes:
// - This function does not force an immediate return from ertqt_run(). It simply
//   requests that the event loop finishes at the next opportunity, according to
//   Qt's normal shutdown semantics.
//
ertqt_status ertqt_quit(void)
{
    if (!g_app)
    {
        return ERTQT_ERR_GENERIC;
    }
    g_app->quit();
    return ERTQT_OK;
}

/* ------------------------------------------------------------------------- */
/* Object lookup                                                             */
/* ------------------------------------------------------------------------- */

// Public interface for looking up a QObject by its objectName property.
//
// This function searches the current object table for an entry whose cached
// objectName matches the supplied UTF 8 name exactly, using a case sensitive
// comparison. If a match is found, its index is returned as an opaque handle.
//
// Parameters:
// - name: UTF 8 encoded objectName to look for. Must not be NULL.
//
// Returns:
// - A non negative ertqt_object_handle value when a matching object is found.
// - ERTQT_ERR_INVALID_ARGUMENT if name is NULL.
// - ERTQT_ERR_NOT_FOUND if no matching objectName exists in the current table.
//
// Notes:
// - The search covers all entries currently stored in g_objects. These entries
//   are populated from the root objects of the QML engine and their children
//   at the time rebuild_object_table() was last called.
// - Handles may become invalid after a subsequent table rebuild. Public API
//   functions detect invalid handles and return an error code.
//
ertqt_object_handle ertqt_get_object_by_name(const char * name)
{
    if (!name)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    std::lock_guard<std::mutex> lock(g_objects_mutex);

    for (size_t i = 0; i < g_objects.size(); ++i)
    {
        const ObjectRecord &rec = g_objects[i];
        if (!rec.ptr)
        {
            continue;
        }
        if (rec.name == name)
        {
            return reinterpret_cast<ertqt_object_handle>(rec.ptr);
        }
    }

    return ERTQT_ERR_NOT_FOUND;
}

// Public interface for retrieving the objectName for a given handle.
//
// This function looks up the entry associated with the supplied handle and
// copies its cached objectName into the caller's buffer if there is enough
// space. The required buffer size is always reported via required_size.
//
// Parameters:
// - h: Opaque handle referring to an entry in the object table.
// - buffer: Destination buffer for the UTF 8 encoded name. May be NULL to query
//           the required size without copying data.
// - buffer_size: Size of buffer in bytes.
// - required_size: Out parameter that will receive the number of bytes required
//                  including the terminating NUL character. Must not be NULL.
//
// Returns:
// - ERTQT_OK on success, regardless of whether the name was copied to buffer.
// - ERTQT_ERR_INVALID_ARGUMENT if required_size is NULL.
// - ERTQT_ERR_INVALID_HANDLE if h is out of range or refers to an empty slot.
//
// Notes:
// - If buffer is NULL or buffer_size is too small, no data is written to buffer
//   and the function still returns ERTQT_OK after setting required_size.
// - The returned name is whatever was cached at the last rebuild. If a QObject's
//   objectName has changed since then, the cache may not reflect the new value.
//
ertqt_status ertqt_get_object_name(ertqt_object_handle h, char * buffer, size_t buffer_size, size_t * required_size)
{
    if (!required_size)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    std::lock_guard<std::mutex> lock(g_objects_mutex);
    if (h < 0 || static_cast<size_t>(h) >= g_objects.size())
    {
        return ERTQT_ERR_INVALID_HANDLE;
    }

    const std::string &name = g_objects[static_cast<size_t>(h)].name;
    const size_t len = name.size() + 1;

    *required_size = len;

    if (!buffer || buffer_size < len)
    {
        return ERTQT_OK;
    }

    std::memcpy(buffer, name.c_str(), len);
    return ERTQT_OK;
}

/* ------------------------------------------------------------------------- */
/* Property helpers                                                          */
/* ------------------------------------------------------------------------- */

// Internal helper for setting a Qt property to a QVariant value.
//
// This function resolves the supplied handle to a QObject, then attempts to set
// the named property using QObject::setProperty(). It is the common underlying
// implementation used by the various typed public property setter functions.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be NULL.
// - value: QVariant containing the value to assign to the property.
//
// Returns:
// - ERTQT_OK if the property was set successfully according to Qt.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if QObject::setProperty() reports failure.
//
// Notes:
// - No type checking is performed here beyond what QObject::setProperty()
//   does internally. It is the responsibility of the typed wrappers to ensure
//   that the QVariant holds a sensible type for the target property.
//
static ertqt_status set_property(ertqt_object_handle h, const char * prop_name, const QVariant &value)
{
    if (!prop_name)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    QObject * obj = handle_to_qobject(h);
    if (!obj)
    {
        return ERTQT_ERR_INVALID_HANDLE;
    }

    const bool ok = obj->setProperty(prop_name, value);
    return ok ? ERTQT_OK : ERTQT_ERR_BACKEND_FAILURE;
}

// Internal helper for retrieving a Qt property as a QVariant.
//
// This function resolves the supplied handle to a QObject, then queries the
// named property using QObject::property(). The resulting QVariant is returned
// via the out parameter if valid.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be NULL.
// - out: Pointer to a QVariant that will receive the value. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property value was retrieved and is valid.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name or out is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup yields an invalid QVariant.
//
// Notes:
// - This function does not perform type conversion. Typed wrappers are expected
//   to call canConvert<T>() on the returned QVariant as appropriate.
//
static ertqt_status get_property(ertqt_object_handle h, const char * prop_name, QVariant * out)
{
    if (!prop_name || !out)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    QObject * obj = handle_to_qobject(h);
    if (!obj)
    {
        return ERTQT_ERR_INVALID_HANDLE;
    }

    const QVariant v = obj->property(prop_name);
    if (!v.isValid())
    {
        return ERTQT_ERR_BACKEND_FAILURE;
    }

    *out = v;
    return ERTQT_OK;
}

// Public interface for setting an integer Qt property.
//
// This function wraps set_property() and assigns the supplied integer value to
// the named Qt property on the target QObject.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be NULL.
// - value: Integer value to assign.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the underlying Qt property assignment fails.
//
// Notes:
// - No explicit type checking is performed beyond what QObject::setProperty()
//   provides. The target property must accept an integer type or a type that
//   QVariant can construct from an int.
//
ertqt_status ertqt_set_property_int(ertqt_object_handle h, const char * prop_name, int value)
{
    return set_property(h, prop_name, QVariant(value));
}

// Public interface for retrieving an integer Qt property.
//
// This function queries the named property on the target QObject, verifies that
// it can be converted to an int, and writes the converted value to out_value.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be NULL.
// - out_value: Pointer that receives the converted integer value. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name or out_value is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails or yields an invalid value.
// - ERTQT_ERR_TYPE_MISMATCH if the value cannot be converted to an int.
//
// Notes:
// - The conversion uses QVariant::toInt(), which applies Qt's usual conversion
//   rules for numeric and string types.
//
ertqt_status ertqt_get_property_int(ertqt_object_handle h, const char * prop_name, int * out_value)
{
    if (!out_value)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    QVariant v;
    ertqt_status st = get_property(h, prop_name, &v);
    if (st != ERTQT_OK)
    {
        return st;
    }

    if (!v.canConvert<int>())
    {
        return ERTQT_ERR_TYPE_MISMATCH;
    }

    *out_value = v.toInt();
    return ERTQT_OK;
}

/* Similar pattern for double, bool, string */

// Public interface for setting a double precision Qt property.
//
// This function wraps set_property() and assigns the supplied double value to
// the named Qt property on the target QObject.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be NULL.
// - value: Double precision value to assign.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the underlying Qt property assignment fails.
//
// Notes:
// - The target property must accept a double or a type that QVariant can
//   construct from a double.
//
ertqt_status ertqt_set_property_double(ertqt_object_handle h, const char * prop_name, double value)
{
    return set_property(h, prop_name, QVariant(value));
}

// Public interface for retrieving a double precision Qt property.
//
// This function queries the named property on the target QObject, verifies that
// it can be converted to a double, and writes the converted value to out_value.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be NULL.
// - out_value: Pointer that receives the converted double value. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name or out_value is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails or yields an invalid value.
// - ERTQT_ERR_TYPE_MISMATCH if the value cannot be converted to a double.
//
// Notes:
// - The conversion uses QVariant::toDouble(), which applies Qt's usual rules
//   for numeric and string types.
//
ertqt_status ertqt_get_property_double(ertqt_object_handle h, const char * prop_name, double * out_value)
{
    if (!out_value)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    QVariant v;
    ertqt_status st = get_property(h, prop_name, &v);
    if (st != ERTQT_OK)
    {
        return st;
    }

    if (!v.canConvert<double>())
    {
        return ERTQT_ERR_TYPE_MISMATCH;
    }

    *out_value = v.toDouble();
    return ERTQT_OK;
}

// Public interface for setting a boolean Qt property.
//
// This function wraps set_property() and assigns a boolean value derived from
// the supplied integer to the named Qt property on the target QObject.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be NULL.
// - value: Integer value interpreted as false when zero and true otherwise.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the underlying Qt property assignment fails.
//
// Notes:
// - The property is assigned a QVariant holding a bool, not the raw integer.
//
ertqt_status ertqt_set_property_bool(ertqt_object_handle h, const char * prop_name, bool value)
{
    const bool b = (value != 0);
    return set_property(h, prop_name, QVariant(b));
}

// Public interface for retrieving a boolean Qt property.
//
// This function queries the named property on the target QObject, verifies that
// it can be converted to a bool, and writes 0 or 1 to out_value accordingly.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be NULL.
// - out_value: Pointer that receives 0 (false) or 1 (true). Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name or out_value is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails or yields an invalid value.
// - ERTQT_ERR_TYPE_MISMATCH if the value cannot be converted to a bool.
//
// Notes:
// - The conversion uses QVariant::toBool(), which applies Qt's usual truthiness
//   rules for different underlying types.
//
ertqt_status ertqt_get_property_bool(ertqt_object_handle h, const char * prop_name, bool * out_value)
{
    if (!out_value)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    QVariant v;
    ertqt_status st = get_property(h, prop_name, &v);
    if (st != ERTQT_OK)
    {
        return st;
    }

    if (!v.canConvert<bool>())
    {
        return ERTQT_ERR_TYPE_MISMATCH;
    }

    *out_value = v.toBool() ? 1 : 0;
    return ERTQT_OK;
}

// Public interface for setting a string Qt property using UTF 8 input.
//
// This function wraps set_property() and assigns a QString constructed from
// the supplied UTF 8 encoded C string to the named Qt property.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be NULL.
// - utf8_value: NUL terminated UTF 8 string containing the value. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name or utf8_value is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the underlying Qt property assignment fails.
//
// Notes:
// - The input is assumed to be valid UTF 8. Invalid byte sequences will be
//   handled by QString::fromUtf8() according to Qt's usual rules.
//
ertqt_status ertqt_set_property_string(ertqt_object_handle h, const char * prop_name, const char * utf8_value)
{
    if (!utf8_value)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }
    return set_property(h, prop_name, QString::fromUtf8(utf8_value));
}

// Public interface for retrieving a string Qt property as UTF 8.
//
// This function queries the named property on the target QObject, verifies that
// it can be converted to a QString, converts it to UTF 8, and copies it into
// the caller's buffer if there is enough space. The required buffer size is
// always reported via required_size.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be NULL.
// - buffer: Destination buffer for the UTF 8 encoded string. May be NULL to
//           query the required size without copying data.
// - buffer_size: Size of buffer in bytes.
// - required_size: Out parameter that receives the number of bytes required,
//                  including the terminating NUL character. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully, regardless
//   of whether the data was copied into buffer.
// - ERTQT_ERR_INVALID_ARGUMENT if prop_name or required_size is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails or yields an invalid value.
// - ERTQT_ERR_TYPE_MISMATCH if the value cannot be converted to a QString.
//
// Notes:
// - If buffer is NULL or buffer_size is too small, no data is written to buffer
//   and the function still returns ERTQT_OK after setting required_size.
// - The UTF 8 data is always NUL terminated when written to buffer.
//
ertqt_status ertqt_get_property_string(ertqt_object_handle h, const char * prop_name, char * buffer, size_t buffer_size, size_t * required_size)
{
    if (!required_size)
    {
        return ERTQT_ERR_INVALID_ARGUMENT;
    }

    QVariant v;
    ertqt_status st = get_property(h, prop_name, &v);
    if (st != ERTQT_OK)
    {
        return st;
    }

    if (!v.canConvert<QString>())
    {
        return ERTQT_ERR_TYPE_MISMATCH;
    }

    const QByteArray utf8 = v.toString().toUtf8();
    const size_t len = static_cast<size_t>(utf8.size()) + 1;

    *required_size = len;

    if (!buffer || buffer_size < len)
    {
        return ERTQT_OK;
    }

    std::memcpy(buffer, utf8.constData(), utf8.size());
    buffer[utf8.size()] = '\0';
    return ERTQT_OK;
}

/* ------------------------------------------------------------------------- */
/* Signal binding                                                            */
/* ------------------------------------------------------------------------- */

// Public interface for binding a C callback to a "clicked()" style Qt signal.
//
// This function connects the object's parameterless "clicked()" signal, if
// present, to a small lambda that invokes the supplied C callback with the
// provided user_data pointer.
//
// Parameters:
// - h: Opaque handle referencing the target QObject, typically a button type.
// - cb: Function pointer to the C callback to invoke when the signal fires.
//       Must not be NULL.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - ERTQT_OK if a compatible signal was found and the connection was created.
// - ERTQT_ERR_INVALID_ARGUMENT if cb is NULL.
// - ERTQT_ERR_INVALID_HANDLE if the handle does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the object has no signal named "clicked()"
//   according to its metaobject.
//
// Notes:
// - Multiple callbacks may be bound for the same object. Each call to this
//   function adds another connection that will be invoked when the signal is
//   emitted.
// - The callback is invoked on the Qt application's thread. The implementation
//   must therefore remain quick and non blocking to avoid impacting the UI.
//
ertqt_status ertqt_bind_clicked(ertqt_object_handle h, ertqt_void_callback cb, void * user_data)
{
    printf("bind_clicked: h=%p cb=%p user_data=%p\n", h, cb, user_data);

    if (!h || !cb)
        return ERTQT_ERR_INVALID_ARGUMENT;

    QObject *obj = reinterpret_cast<QObject*>(h);

    printf("bind_clicked: obj className=%s\n", obj->metaObject()->className());
    printf("bind_clicked: g_app=%p\n", g_app);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // Qt5-compatible bridge:
    // 1) Use old-style SIGNAL() to connect unknown QObject* to QSignalMapper::map()
    // 2) Use new-style connect from QSignalMapper's typed signal to a lambda

    auto *mapper = new QSignalMapper(obj); // parented to obj for lifetime safety
    mapper->setMapping(obj, obj);

    printf("bind_clicked: mapping set\n");

    if (!QObject::connect(obj, SIGNAL(clicked()), mapper, SLOT(map())))
        return ERTQT_ERR_BACKEND_FAILURE;

    printf("bind_clicked: connected clicked signal\n");

    // QSignalMapper::mapped(QObject*) is overloaded in some Qt versions; disambiguate:
    auto mappedObj = static_cast<void (QSignalMapper::*)(QObject*)>(&QSignalMapper::mapped);

    printf("bind_clicked: created mapped object\n");

    QObject::connect(mapper, mappedObj, g_app,
                     [cb, user_data](QObject*)
                     {
                         cb(user_data);
                     });

    printf("bind_clicked: connected callback\n");

    return ERTQT_OK;

#else
    // Qt6 path (your original approach may be OK here)
    QObject::connect(obj, SIGNAL(clicked()), g_app, [cb, user_data]()
    {
        cb(user_data);
    });
    return ERTQT_OK;
#endif
}

// ertqt_status ertqt_bind_clicked(ertqt_object_handle h, ertqt_void_callback cb, void *user_data)
// {
//     if (!h || !cb)
//         return ERTQT_ERR_INVALID_ARGUMENT;

//     auto *obj = reinterpret_cast<QObject*>(h);
//     if (!obj)
//         return ERTQT_ERR_INVALID_HANDLE;

//     const QMetaObject *mo = obj->metaObject();
//     if (!mo)
//         return ERTQT_ERR_BACKEND_FAILURE;

//     int idx = mo->indexOfSignal("clicked()");
//     bool takesBool = false;

//     if (idx < 0) {
//         idx = mo->indexOfSignal("clicked(bool)");
//         takesBool = (idx >= 0);
//     }

//     if (idx < 0)
//         return ERTQT_ERR_BACKEND_FAILURE;

//     QMetaMethod sig = mo->method(idx);

//     // QMetaMethod + functor works in supported 5.x releases and above
//     bool ok = false;
//     if (!takesBool) {
//         ok = QObject::connect(obj, sig, g_app, [cb, user_data]() {
//             cb(user_data);
//         });
//     } else {
//         ok = QObject::connect(obj, sig, g_app, [cb, user_data](bool) {
//             cb(user_data);
//         });
//     }

//     return ok ? ERTQT_OK : ERTQT_ERR_BACKEND_FAILURE;
// }

// ertqt_status ertqt_bind_clicked(ertqt_object_handle h, ertqt_void_callback cb, void * user_data)
// {
//     if (!cb)
//     {
//         return ERTQT_ERR_INVALID_ARGUMENT;
//     }

//     QObject *obj = handle_to_qobject(h);
//     if (!obj)
//     {
//         return ERTQT_ERR_INVALID_HANDLE;
//     }

//     // Very simple approach: expect a signal "clicked()" on the object
//     const QMetaObject *mo = obj->metaObject();
//     const int idx = mo->indexOfSignal("clicked()");
//     if (idx < 0)
//     {
//         return ERTQT_ERR_BACKEND_FAILURE;
//     }

//     // Use a lambda that captures cb and user_data
//     QObject::connect(obj, SIGNAL(clicked()), g_app, [cb, user_data]()
//     {
//         cb(user_data);
//     });
//     return ERTQT_OK;
// }

/* ------------------------------------------------------------------------- */
/* Tick callback                                                             */
/* ------------------------------------------------------------------------- */

// Public interface for configuring a periodic tick callback driven by Qt timers.
//
// This function configures a callback that will be invoked periodically while
// the Qt event loop is running. It stores the supplied callback, user_data and
// interval globally and, when appropriate, schedules the first timer.
//
// Parameters:
// - interval_ms: Requested interval between invocations in milliseconds. If zero,
//                any existing tick chain will not be continued.
// - cb: Function pointer to the tick callback. May be NULL to disable ticking.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - ERTQT_OK if the configuration was applied successfully.
// - ERTQT_ERR_GENERIC if the Qt application instance is not available, which
//   indicates that ertqt_init() has not completed successfully.
//
// Notes:
// - The actual timing behaviour is subject to the usual Qt timer and platform
//   scheduling characteristics. Callers must not rely on exact periodicity.
// - The callback is invoked on the Qt application's thread. It should perform
//   only light work and return promptly.
//
ertqt_status ertqt_set_tick_callback(unsigned int interval_ms, ertqt_tick_callback cb, void * user_data)
{
    g_tick_cb = cb;
    g_tick_user_data = user_data;
    g_tick_interval_ms = interval_ms;

    if (!g_app)
    {
        return ERTQT_ERR_GENERIC;
    }

    if (g_tick_cb && g_tick_interval_ms > 0)
    {
        // Start the chain
        QTimer::singleShot(static_cast<int>(g_tick_interval_ms), &tick_timer_fired);
    }

    return ERTQT_OK;
}

} // extern "C"
