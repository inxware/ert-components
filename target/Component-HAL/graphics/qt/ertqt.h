#ifndef ERTQT_H
#define ERTQT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Status and basic types                                                    */
/* ------------------------------------------------------------------------- */

// Status codes returned by the ertqt API.
//
// These values indicate the outcome of operations performed by the Qt
// integration layer, allowing callers to distinguish between success,
// invalid inputs, handle problems and backend failures.
//
// Notes:
// - Zero represents success (ERTQT_OK).
// - Negative values represent different classes of error.
typedef enum ertqt_status
{
    ERTQT_OK = 0,
    ERTQT_ERR_GENERIC = -1,
    ERTQT_ERR_INVALID_HANDLE = -2,
    ERTQT_ERR_INVALID_ARGUMENT = -3,
    ERTQT_ERR_NOT_FOUND = -4,
    ERTQT_ERR_TYPE_MISMATCH = -5,
    ERTQT_ERR_BACKEND_FAILURE = -6
} ertqt_status;

// Opaque handle type for referencing QObjects inside the Qt library.
//
// This type is used to refer to QObjects managed by the integration layer
// without exposing Qt or C++ types to callers. Handles are produced by
// functions such as ertqt_get_object_by_name() and consumed by property
// and signal binding functions.
//
// Notes:
// - The concrete representation is an integer index into an internal table.
// - Callers must treat handle values as opaque and not make assumptions
//   about their structure or lifetime.
typedef intptr_t ertqt_object_handle;

/* ------------------------------------------------------------------------- */
/* Application lifecycle state machine                                       */
/* ------------------------------------------------------------------------- */

// State of the ertqt application lifecycle.
//
// This enum tracks the progress of QML loading and object table scanning.
// The tick callback should call `ertqt_process_state()` each iteration to
// advance the state machine.
typedef enum ertqt_app_state
{
    ERTQT_APP_STATE_IDLE = 0,          // Steady state — app running, objects bound
    ERTQT_APP_STATE_LOADING,           // QML submitted to engine, load() in progress
    ERTQT_APP_STATE_OBJECTS_READY,     // QML loaded, object tree populated, scan pending
    ERTQT_APP_STATE_SCANNED,           // Object table rebuilt — widgets can bind
    ERTQT_APP_STATE_RESCAN_NEEDED      // Dynamic QObject change detected, rescan pending
} ertqt_app_state;

// Query the current application lifecycle state.
//
// Returns:
// - The current `ertqt_app_state` value.
ertqt_app_state ertqt_get_app_state(void);

// Advance the application lifecycle state machine.
//
// This function should be called from the tick callback on each iteration.
// It checks the current state and performs any pending work:
//
// - `ERTQT_APP_STATE_OBJECTS_READY`: Rebuilds the object table and
//   transitions to `ERTQT_APP_STATE_SCANNED`.
// - `ERTQT_APP_STATE_SCANNED`: Transitions to `ERTQT_APP_STATE_IDLE`.
// - `ERTQT_APP_STATE_RESCAN_NEEDED`: Rebuilds the object table and
//   transitions to `ERTQT_APP_STATE_SCANNED`.
// - All other states: No action taken.
//
// Returns:
// - The state after processing (which may differ from the state before).
ertqt_app_state ertqt_process_state(void);

// Request a rescan of the QObject tree on the next tick.
//
// This sets the state to `ERTQT_APP_STATE_RESCAN_NEEDED` so that the next
// call to `ertqt_process_state()` will rebuild the object table. Use this
// when dynamic QML content has been added (Loader, Repeater, etc.).
void ertqt_request_rescan(void);

/* ------------------------------------------------------------------------- */
/* Initialisation and event loop                                             */
/* ------------------------------------------------------------------------- */

// Public interface for initialising the Qt infrastructure.
//
// This function creates the QGuiApplication and QQmlApplicationEngine
// instances. It does not load any QML content; call `ertqt_load_app()` to
// load a QML file after initialisation.
//
// Parameters:
// - argc: Application argument count, usually the same as `main()` received. If
//   zero, a dummy argument list is constructed internally.
// - argv: Application argument vector. If `NULL` while `argc` is non zero,
//   behaviour is undefined. If both `argc` and `argv` indicate "no arguments",
//   a dummy argument list containing a single placeholder is used.
//
// Returns:
// - `ERTQT_OK` on successful initialisation.
// - `ERTQT_ERR_BACKEND_FAILURE` if a Qt application instance already exists or
//   the Qt application or QML engine could not be created.
//
// Notes:
// - This function does not start the Qt event loop. Call `ertqt_run()` after
//   loading an application via `ertqt_load_app()` and setting up callbacks.
// - Repeated calls after a successful initialisation are treated as no-ops and
//   return `ERTQT_OK`.
ertqt_status ertqt_init(int argc, char ** argv);

// Public interface for loading a QML application file.
//
// This function loads the specified QML file into the QML engine and rebuilds
// the internal object table so that all QObjects with an `objectName` become
// discoverable via `ertqt_get_object_by_name()`. It may be called multiple
// times to load new applications after EHS reloads.
//
// Parameters:
// - qml_path: Path to the QML file to load, encoded as UTF 8. Must not be
//   `NULL`.
//
// Returns:
// - `ERTQT_OK` if the QML file was loaded and at least one root object was
//   created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `qml_path` is `NULL`.
// - `ERTQT_ERR_GENERIC` if `ertqt_init()` has not been called successfully.
// - `ERTQT_ERR_BACKEND_FAILURE` if the QML file failed to produce at least one
//   root object.
//
// Notes:
// - Existing handles may become invalid after this call. Callers that cache
//   handles should re-acquire them via `ertqt_get_object_by_name()`.
// - This function should be called from the Qt thread (e.g. from a tick
//   callback).
ertqt_status ertqt_load_app(const char * qml_path);

/* This may potentially be called externally if we (e.g. Widget module if QML is dynamically loaded */
// Function  for resolving an ertqt_object_handle into a QObject pointer.
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
// void rebuild_object_table();


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
// - `ERTQT_OK` if the event loop exited with a zero return code.
// - `ERTQT_ERR_GENERIC` if Qt has not been initialised via `ertqt_init()` or
//   the global application pointer is `NULL`.
// - `ERTQT_ERR_BACKEND_FAILURE` if the Qt event loop returns a non zero exit
//   code.
//
// Notes:
// - This call is blocking. It should usually be the final call made by the
//   caller during start-up, after all bindings and callbacks have been set up.
// - The event loop can be terminated programmatically by calling
//   `ertqt_quit()`, or by any other mechanism that causes the Qt application
//   to quit.
ertqt_status ertqt_run(void);

// Public interface for requesting termination of the Qt event loop.
//
// This function requests that the Qt application exits its event loop by
// issuing a quit request on the global application instance.
//
// Parameters:
// - (none)
//
// Returns:
// - `ERTQT_OK` if the quit request was issued successfully.
// - `ERTQT_ERR_GENERIC` if the global application pointer is `NULL`, which
//   indicates that `ertqt_init()` has not completed successfully.
//
// Notes:
// - This function does not force an immediate return from `ertqt_run()`. It
//   simply requests that the event loop finishes at the next opportunity,
//   according to Qt's normal shutdown semantics.
ertqt_status ertqt_quit(void);

/* ------------------------------------------------------------------------- */
/* Object lookup and introspection                                           */
/* ------------------------------------------------------------------------- */

// Public interface for looking up a QObject by its `objectName` property.
//
// This function searches the current object table for an entry whose cached
// `objectName` matches the supplied UTF 8 name exactly, using a case sensitive
// comparison. If a match is found, its index is returned as an opaque handle.
//
// Parameters:
// - name: UTF 8 encoded `objectName` to look for. Must not be `NULL`.
//
// Returns:
// - A non negative `ertqt_object_handle` value when a matching object is found.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `name` is `NULL`.
// - `ERTQT_ERR_NOT_FOUND` if no matching `objectName` exists in the current
//   table.
//
// Notes:
// - The search covers all entries currently stored in the internal object
//   table. These entries are populated from the root objects of the QML engine
//   and their children at the time the table was last rebuilt.
// - Handles may become invalid after a subsequent table rebuild. Public API
//   functions detect invalid handles and return an error code.
ertqt_object_handle ertqt_get_object_by_name(const char * name);

// Public interface for refreshing the internal object table.
//
// This function rebuilds the object table by re-enumerating all QObject
// children of the QML engine's root objects. It should be called after
// dynamic QML content has been loaded (e.g. via Loader, Repeater, or
// Component.createObject) so that newly created objects become discoverable
// by ertqt_get_object_by_name().
//
// Parameters:
// - (none)
//
// Returns:
// - `ERTQT_OK` if the table was rebuilt successfully.
// - `ERTQT_ERR_GENERIC` if Qt has not been initialised via `ertqt_init()`.
//
// Notes:
// - Existing handles may become invalid after this call. Callers that cache
//   handles should re-acquire them via ertqt_get_object_by_name().
ertqt_status ertqt_refresh_objects(void);

#ifdef ERTQT_SINGLETON_SCAN
// Register a QML expression to be evaluated as an extra scan root during
// object table rebuilds.
//
// QML singletons (pragma Singleton) are registered in the QML type system and
// are NOT children of the engine's root objects. They are therefore invisible
// to the findChildren() traversal used by rebuild_object_table(). This function
// lets callers register a QML identifier string (e.g. "Rooms") that will be
// evaluated in the context of the first root object on each rebuild. The
// resulting QObject and all its QObject children are added to the table.
//
// Parameters:
// - qml_expression: A QML identifier resolvable in the root object's context,
//   e.g. "Rooms". Must not be NULL.
//
// Returns:
// - ERTQT_OK on success.
// - ERTQT_ERR_INVALID_ARGUMENT if qml_expression is NULL.
//
// Notes:
// - Expressions are stored persistently and re-evaluated on every rebuild,
//   including after app reloads.
// - If the expression cannot be resolved at rebuild time a warning is printed
//   and the expression is silently skipped for that rebuild.
// - Only available when ERTQT_SINGLETON_SCAN is defined at compile time.
ertqt_status ertqt_add_singleton_scan(const char * qml_expression);
#endif /* ERTQT_SINGLETON_SCAN */

// Public interface for retrieving the `objectName` for a given handle.
//
// This function looks up the entry associated with the supplied handle and
// copies its cached `objectName` into the caller's buffer if there is enough
// space. The required buffer size is always reported via `required_size`.
//
// Parameters:
// - h: Opaque handle referring to an entry in the object table.
// - buffer: Destination buffer for the UTF 8 encoded name. May be `NULL` to
//   query the required size without copying data.
// - buffer_size: Size of `buffer` in bytes.
// - required_size: Out parameter that receives the number of bytes required,
//   including the terminating NUL character. Must not be `NULL`.
//
// Returns:
// - `ERTQT_OK` on success, regardless of whether the name was copied to
//   `buffer`.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `required_size` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` is out of range or refers to an empty
//   slot.
//
// Notes:
// - If `buffer` is `NULL` or `buffer_size` is too small, no data is written to
//   `buffer` and the function still returns `ERTQT_OK` after setting
//   `required_size`.
// - The returned name is whatever was cached at the last rebuild. If a
//   QObject's `objectName` has changed since then, the cache may not reflect
//   the new value.
ertqt_status ertqt_get_object_name(ertqt_object_handle h, char * buffer, size_t buffer_size, size_t * required_size);

/* ------------------------------------------------------------------------- */
/* Property access helpers                                                   */
/* ------------------------------------------------------------------------- */

// Public interface for setting an integer Qt property.
//
// This function assigns the supplied integer value to the named Qt property on
// the target object. It operates on properties exposed via the QObject
// metaobject, typically those declared with `Q_PROPERTY` or standard QML
// properties.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be `NULL`.
// - value: Integer value to assign.
//
// Returns:
// - `ERTQT_OK` if the property was set successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the underlying Qt property assignment fails.
//
// Notes:
// - Property names are case sensitive.
// - The target property must accept an integer type or a type that Qt can
//   construct from an `int`.
ertqt_status ertqt_set_property_int(ertqt_object_handle h, const char * prop_name, int value);

// Public interface for retrieving an integer Qt property.
//
// This function queries the named property on the target QObject and, if it can
// be converted to an integer, writes the converted value to `out_value`.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be `NULL`.
// - out_value: Pointer that receives the converted integer value. Must not be
//   `NULL`.
//
// Returns:
// - `ERTQT_OK` if the property was retrieved and converted successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` or `out_value` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the property lookup fails or yields an
//   invalid value.
// - `ERTQT_ERR_TYPE_MISMATCH` if the value cannot be converted to an `int`.
//
// Notes:
// - The conversion uses Qt's usual rules for numeric and string types.
ertqt_status ertqt_get_property_int(ertqt_object_handle h, const char * prop_name, int * out_value);

// Public interface for setting a double precision Qt property.
//
// This function assigns the supplied double value to the named Qt property on
// the target object.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be `NULL`.
// - value: Double precision value to assign.
//
// Returns:
// - `ERTQT_OK` if the property was set successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the underlying Qt property assignment fails.
//
// Notes:
// - The target property must accept a double or a type that Qt can construct
//   from a `double`.
ertqt_status ertqt_set_property_double(ertqt_object_handle h, const char * prop_name, double value);

// Public interface for retrieving a double precision Qt property.
//
// This function queries the named property on the target QObject and, if it can
// be converted to a double, writes the converted value to `out_value`.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be `NULL`.
// - out_value: Pointer that receives the converted double value. Must not be
//   `NULL`.
//
// Returns:
// - `ERTQT_OK` if the property was retrieved and converted successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` or `out_value` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the property lookup fails or yields an
//   invalid value.
// - `ERTQT_ERR_TYPE_MISMATCH` if the value cannot be converted to a `double`.
//
// Notes:
// - The conversion uses Qt's usual rules for numeric and string types.
ertqt_status ertqt_get_property_double(ertqt_object_handle h, const char * prop_name, double * out_value);

// Public interface for setting a boolean Qt property.
//
// This function assigns a boolean value to the named Qt property on the target
// object. It is intended for properties that semantically represent true/false
// state, such as `enabled`, `visible`, `checked` and similar.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be `NULL`.
// - value: Boolean value to assign to the property.
//
// Returns:
// - `ERTQT_OK` if the property was set successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the underlying Qt property assignment fails.
//
// Notes:
// - The property is assigned a QVariant holding a `bool`.
ertqt_status ertqt_set_property_bool(ertqt_object_handle h, const char * prop_name, bool value);

// Public interface for retrieving a boolean Qt property.
//
// This function queries the named property on the target QObject and, if it can
// be converted to a `bool`, writes the result to `out_value`.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be `NULL`.
// - out_value: Pointer that receives the converted boolean value. Must not be
//   `NULL`.
//
// Returns:
// - `ERTQT_OK` if the property was retrieved and converted successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` or `out_value` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the property lookup fails or yields an
//   invalid value.
// - `ERTQT_ERR_TYPE_MISMATCH` if the value cannot be converted to a `bool`.
//
// Notes:
// - The conversion uses Qt's usual truthiness rules for different underlying
//   types.
ertqt_status ertqt_get_property_bool(ertqt_object_handle h, const char * prop_name, bool * out_value);

// Public interface for setting a string Qt property using UTF 8 input.
//
// This function assigns a QString constructed from the supplied UTF 8 encoded
// C string to the named Qt property on the target object.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to set. Must not be `NULL`.
// - utf8_value: NUL terminated UTF 8 string containing the value. Must not be
//   `NULL`.
//
// Returns:
// - `ERTQT_OK` if the property was set successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` or `utf8_value` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the underlying Qt property assignment fails.
//
// Notes:
// - The input is assumed to be valid UTF 8. Invalid byte sequences are handled
//   by Qt according to its usual rules.
ertqt_status ertqt_set_property_string(ertqt_object_handle h, const char * prop_name, const char * utf8_value);

// Public interface for retrieving a string Qt property as UTF 8.
//
// This function queries the named property on the target QObject and, if it can
// be converted to a QString, converts it to UTF 8 and copies it into the
// caller's buffer if there is enough space. The required buffer size is always
// reported via `required_size`.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
// - prop_name: Name of the Qt property to query. Must not be `NULL`.
// - buffer: Destination buffer for the UTF 8 encoded string. May be `NULL` to
//   query the required size without copying data.
// - buffer_size: Size of `buffer` in bytes.
// - required_size: Out parameter that receives the number of bytes required,
//   including the terminating NUL character. Must not be `NULL`.
//
// Returns:
// - `ERTQT_OK` if the property was retrieved and converted successfully,
//   regardless of whether the data was copied into `buffer`.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `prop_name` or `required_size` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if the handle does not resolve to a valid
//   QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the property lookup fails or yields an
//   invalid value.
// - `ERTQT_ERR_TYPE_MISMATCH` if the value cannot be converted to a QString.
//
// Notes:
// - If `buffer` is `NULL` or `buffer_size` is too small, no data is written to
//   `buffer` and the function still returns `ERTQT_OK` after setting
//   `required_size`.
// - When data is written, the UTF 8 string in `buffer` is always NUL terminated.
ertqt_status ertqt_get_property_string(ertqt_object_handle h, const char * prop_name, char * buffer, size_t buffer_size, size_t * required_size);

/* ------------------------------------------------------------------------- */
/* Widget update                                                             */
/* ------------------------------------------------------------------------- */

// Public interface for requesting a visual update of a Qt widget.
//
// This function requests that the target QObject schedules a repaint by
// calling its `update()` method (if available). This is typically used to
// force a refresh when widget properties have changed but Qt has not
// automatically detected the need for a repaint.
//
// Parameters:
// - h: Opaque handle referencing the target QObject.
//
// Returns:
// - `ERTQT_OK` if the update was requested successfully.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object does not support the `update()`
//   method or the call failed.
//
// Notes:
// - This schedules a repaint; the actual painting happens asynchronously when
//   Qt processes its event loop.
// - For QML items (QQuickItem), this calls QQuickItem::update().
// - Not all QObject types support updating; for non visual items this may fail.
ertqt_status ertqt_update_widget(ertqt_object_handle h);

/* ------------------------------------------------------------------------- */
/* Signal binding                                                            */
/* ------------------------------------------------------------------------- */

// Callback type for parameterless signals used by simple bindings.
//
// This function pointer type is used for callbacks that do not require any
// signal payload. It is commonly used for events such as button clicks.
//
// Parameters:
// - user_data: Opaque pointer supplied at registration time. The library never
//   dereferences this pointer; it is simply passed through to the callback.
typedef void (*ertqt_void_callback)(void * user_data);

// Public interface for binding a C callback to a `clicked()` style signal.
//
// This function connects a parameterless `clicked()` signal on the target
// QObject, if present, to a C callback. It is intended for use with button like
// controls that expose a `clicked()` signal in their metaobject.
//
// Parameters:
// - h: Opaque handle referencing the target QObject, typically a button type.
// - cb: Function pointer to the callback to invoke when the signal fires. Must
//   not be `NULL`.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if a compatible signal was found and the connection was created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `cb` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object has no signal named `clicked()`
//   or the connection could not be created.
//
// Notes:
// - Multiple callbacks may be registered for the same object by calling this
//   function multiple times. Each call adds another connection.
// - The callback is invoked on the Qt application's thread. Implementations
//   should avoid long blocking operations.
// - Note: Qt's clicked() signal is emitted on button release, not press.
ertqt_status ertqt_bind_clicked(ertqt_object_handle h, ertqt_void_callback cb, void * user_data);

// Public interface for binding a C callback to a `pressed()` style signal.
//
// This function connects a parameterless `pressed()` signal on the target
// QObject, if present, to a C callback. It is intended for use with button like
// controls that expose a `pressed()` signal in their metaobject.
//
// Parameters:
// - h: Opaque handle referencing the target QObject, typically a button type.
// - cb: Function pointer to the callback to invoke when the signal fires. Must
//   not be `NULL`.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if a compatible signal was found and the connection was created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `cb` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object has no signal named `pressed()`
//   or the connection could not be created.
//
// Notes:
// - The pressed() signal is emitted when the button is pressed down.
// - For standard Qt Quick Controls Button, pressed() fires on mouse/touch down.
ertqt_status ertqt_bind_pressed(ertqt_object_handle h, ertqt_void_callback cb, void * user_data);

// Public interface for binding a C callback to a `released()` style signal.
//
// This function connects a parameterless `released()` signal on the target
// QObject, if present, to a C callback. It is intended for use with button like
// controls that expose a `released()` signal in their metaobject.
//
// Parameters:
// - h: Opaque handle referencing the target QObject, typically a button type.
// - cb: Function pointer to the callback to invoke when the signal fires. Must
//   not be `NULL`.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if a compatible signal was found and the connection was created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `cb` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object has no signal named `released()`
//   or the connection could not be created.
//
// Notes:
// - The released() signal is emitted when the button is released.
// - For standard Qt Quick Controls Button, released() fires on mouse/touch up.
// - EHS treats button release as the actual "click" event.
ertqt_status ertqt_bind_released(ertqt_object_handle h, ertqt_void_callback cb, void * user_data);

/* ------------------------------------------------------------------------- */
/* Optional tick integration                                                 */
/* ------------------------------------------------------------------------- */

// Callback type for periodic tick notifications driven by Qt timers.
//
// This function pointer type is used for callbacks that are invoked at regular
// intervals while the Qt event loop is running.
//
// Parameters:
// - user_data: Opaque pointer supplied at registration time. The library never
//   dereferences this pointer; it is simply passed through to the callback.
typedef void (*ertqt_tick_callback)(void * user_data);

// Callback type for text based signals on text entry controls.
//
// This function pointer type is used for callbacks that receive the current
// text from a text entry control when a relevant signal is emitted.
//
// Parameters:
// - utf8_text: NUL terminated UTF 8 string with the current field text. This
//   pointer is only valid for the duration of the callback.
// - user_data: Opaque pointer supplied at registration time. The library never
//   dereferences this pointer; it is simply passed through to the callback.
//
// Notes:
// - Callers that need to retain the text beyond the callback must make their
//   own copy.
typedef void (*ertqt_text_callback)(const char * utf8_text, void * user_data);

// Callback type for value based signals on numeric controls.
//
// This function pointer type is used for callbacks that receive a double
// precision floating point value from a QML control when a relevant signal
// (e.g. `valueChanged`) is emitted.
//
// Parameters:
// - value: The current numeric value read from the QML control's `value`
//   property at the time the signal was emitted.
// - user_data: Opaque pointer supplied at registration time. The library never
//   dereferences this pointer; it is simply passed through to the callback.
typedef void (*ertqt_value_callback)(double value, void * user_data);

// Public interface for binding a C callback to a `valueChanged` style signal.
//
// This function connects a numeric control's change notification signal to a C
// callback. On QML Slider / SpinBox / Dial controls this is typically mapped to
// the `valueChanged()` signal. The callback receives the current value as a
// double each time it is invoked.
//
// Parameters:
// - h: Opaque handle referencing the target numeric QObject.
// - cb: Function pointer to the callback to invoke when the value changes. Must
//   not be `NULL`.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if a compatible signal was found and the connection was created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `cb` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object has no compatible `valueChanged()`
//   signal or the connection could not be created.
//
// Notes:
// - The callback is invoked on the Qt application's thread. Implementations
//   must remain quick and non blocking to avoid impacting the UI.
// - The value is read from the QML object's `value` property as a double at
//   the time the signal fires.
ertqt_status ertqt_bind_value_changed(ertqt_object_handle h, ertqt_value_callback cb, void * user_data);

// Public interface for binding a C callback to a `textChanged` style signal.
//
// This function connects a text field's change notification signal to a C
// callback. On QML TextField / TextInput controls this is typically mapped to
// the `textChanged()` signal. The callback receives the current text as a UTF 8
// string each time it is invoked.
//
// Parameters:
// - h: Opaque handle referencing the target text field QObject.
// - cb: Function pointer to the callback to invoke when the text changes. Must
//   not be `NULL`.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if a compatible signal was found and the connection was created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `cb` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object has no compatible `textChanged()`
//   signal or the connection could not be created.
//
// Notes:
// - The callback is invoked on the Qt application's thread. Implementations
//   must remain quick and non blocking to avoid impacting the UI.
// - The `utf8_text` pointer is only valid until the callback returns. Callers
//   must copy the string if they need to retain it.
ertqt_status ertqt_bind_text_changed(ertqt_object_handle h, ertqt_text_callback cb, void * user_data);

// Public interface for binding a C callback to an "editing finished" style signal.
//
// This function connects a text field's "editing complete" notification signal
// to a C callback. For QML TextField / TextInput controls this is typically
// mapped to the `accepted()` signal, which is emitted when the user presses
// Enter or otherwise accepts the current value.
//
// Parameters:
// - h: Opaque handle referencing the target text field QObject.
// - cb: Function pointer to the callback to invoke when editing is finished.
//   Must not be `NULL`.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if a compatible signal was found and the connection was created.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `cb` is `NULL`.
// - `ERTQT_ERR_INVALID_HANDLE` if `h` does not resolve to a valid QObject.
// - `ERTQT_ERR_BACKEND_FAILURE` if the object has no compatible `accepted()`
//   signal or the connection could not be created.
//
// Notes:
// - The callback is invoked on the Qt application's thread.
// - The `utf8_text` value passed to the callback (as part of the underlying
//   implementation) is only valid until the callback returns. Callers must
//   copy any text they need to retain.
ertqt_status ertqt_bind_editing_finished(ertqt_object_handle h, ertqt_text_callback cb, void * user_data);

// Public interface for registering a periodic tick callback driven by Qt timers.
//
// This function configures a callback that will be invoked periodically while
// the Qt event loop is running. It stores the supplied callback, `user_data`
// and interval and, when appropriate, arranges for a Qt timer to drive the
// callback.
//
// Parameters:
// - interval_ms: Requested interval between invocations in milliseconds. If
//   zero, any existing tick chain will not be continued.
// - cb: Function pointer to the tick callback. May be `NULL` to disable
//   ticking.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - `ERTQT_OK` if the configuration was applied successfully.
// - `ERTQT_ERR_GENERIC` if the Qt application instance is not available, which
//   indicates that `ertqt_init()` has not completed successfully.
//
// Notes:
// - The actual timing behaviour is subject to Qt timer and platform scheduling
//   characteristics. Callers must not rely on exact periodicity.
// - The callback is invoked on the Qt application's thread. It should perform
//   only light work and return promptly.
ertqt_status ertqt_set_tick_callback(unsigned int interval_ms, ertqt_tick_callback cb, void * user_data);

/* ------------------------------------------------------------------------- */
/* Window management                                                         */
/* ------------------------------------------------------------------------- */

// Public interface for querying the Qt window dimensions.
//
// This function retrieves the width and height of the main Qt window (the root
// QML object). This is typically used by the EHS graphics system to determine
// the available screen space.
//
// Parameters:
// - out_width: Pointer that receives the window width in pixels. Must not be
//   `NULL`.
// - out_height: Pointer that receives the window height in pixels. Must not be
//   `NULL`.
//
// Returns:
// - `ERTQT_OK` if the dimensions were retrieved successfully.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `out_width` or `out_height` is `NULL`.
// - `ERTQT_ERR_GENERIC` if Qt has not been initialized or no root object exists.
//
// Notes:
// - Returns the dimensions of the first root object from the QML engine.
// - If the window is resizable, these dimensions may change over time.
ertqt_status ertqt_get_window_size(int * out_width, int * out_height);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ERTQT_H */
