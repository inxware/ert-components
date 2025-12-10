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
/* Initialisation and event loop                                             */
/* ------------------------------------------------------------------------- */

// Public interface for initialising the Qt integration and loading the QML file.
//
// This function creates the Qt application instance if none exists yet, sets up
// the QML engine, and loads the specified QML file. It also builds the initial
// internal object table used to resolve ertqt_object_handle values.
//
// Parameters:
// - qml_path: Path to the QML file to load, encoded as UTF 8. Must not be `NULL`.
// - argc: Application argument count, usually the same as `main()` received. If
//   zero, a dummy argument list is constructed internally.
// - argv: Application argument vector. If `NULL` while `argc` is non zero,
//   behaviour is undefined. If both `argc` and `argv` indicate "no arguments",
//   a dummy argument list containing a single placeholder is used.
//
// Returns:
// - `ERTQT_OK` on successful initialisation and QML loading.
// - `ERTQT_ERR_INVALID_ARGUMENT` if `qml_path` is `NULL`.
// - `ERTQT_ERR_BACKEND_FAILURE` if a Qt application instance already exists, the
//   Qt application or QML engine could not be created, or the QML file failed
//   to produce at least one root object.
//
// Notes:
// - This function does not start the Qt event loop. Call `ertqt_run()` after a
//   successful return value to begin processing events.
// - Repeated calls after a successful initialisation are treated as no-ops and
//   return `ERTQT_OK` without reloading QML.
ertqt_status ertqt_init(const char * qml_path, int argc, char ** argv);

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
ertqt_status ertqt_bind_clicked(ertqt_object_handle h, ertqt_void_callback cb, void * user_data);

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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ERTQT_H */
