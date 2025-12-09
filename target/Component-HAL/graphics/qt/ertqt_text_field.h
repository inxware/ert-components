/* ertqt_text_field.h */

#ifndef ERTQT_TEXT_FIELD_H
#define ERTQT_TEXT_FIELD_H

#include "ertqt.h"

#ifdef __cplusplus
extern "C" {
#endif

// Public interface for resolving a text field by its objectName.
//
// This convenience wrapper looks up a QObject by its objectName and returns an
// ertqt_object_handle that is expected to refer to a text entry control. It
// simply forwards to ertqt_get_object_by_name(), but makes the intent clearer
// at call sites that are specifically dealing with text fields.
//
// Parameters:
// - name: UTF 8 encoded objectName of the text field in the QML scene. Must not
//         be NULL.
//
// Returns:
// - A non negative ertqt_object_handle value if a matching object was found.
// - ERTQT_ERR_INVALID_ARGUMENT if name is NULL.
// - ERTQT_ERR_NOT_FOUND if no object with the specified name exists in the
//   current object table.
//
// Notes:
// - The returned handle is not validated to ensure that the underlying QObject
//   is actually a TextField or TextInput. Callers should use this only with
//   known objectNames defined in the QML.
//
static inline ertqt_object_handle ertqt_text_field_by_name(const char * name)
{
    return ertqt_get_object_by_name(name);
}

// Public interface for setting the text field's current text.
//
// This function sets the "text" property on the specified text field object to
// the supplied UTF 8 string. It is a convenience wrapper around
// ertqt_set_property_string() that hard codes the property name.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - utf8_text: NUL terminated UTF 8 string containing the new text. Must not be
//              NULL.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if utf8_text is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The underlying QML control must expose a "text" property for this to work.
//   This is true for standard Qt Quick Controls TextField.
//
static inline ertqt_status ertqt_text_field_set_text(ertqt_object_handle field, const char * utf8_text)
{
    return ertqt_set_property_string(field, "text", utf8_text);
}

// Public interface for retrieving the text field's current text.
//
// This function reads the "text" property from the specified text field and
// returns it to the caller as a UTF 8 encoded string. It is a convenience
// wrapper around ertqt_get_property_string() that hard codes the property name.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - buffer: Destination buffer for the UTF 8 encoded text. May be NULL to query
//           the required size without copying data.
// - buffer_size: Size of buffer in bytes.
// - required_size: Out parameter that receives the number of bytes required,
//                  including the terminating NUL character. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully, regardless
//   of whether buffer was large enough to receive the data.
// - ERTQT_ERR_INVALID_ARGUMENT if required_size is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a string.
//
// Notes:
// - If buffer is NULL or buffer_size is too small, no bytes are written to
//   buffer. required_size still receives the full size needed.
//
static inline ertqt_status ertqt_text_field_get_text(ertqt_object_handle field, char * buffer, size_t buffer_size, size_t * required_size)
{
    return ertqt_get_property_string(field, "text", buffer, buffer_size, required_size);
}

// Public interface for setting the text field's placeholder text.
//
// This function sets the "placeholderText" property on the specified text
// field object to the supplied UTF 8 string. It is a convenience wrapper
// around ertqt_set_property_string().
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - utf8_text: NUL terminated UTF 8 string containing the placeholder text.
//              Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if utf8_text is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The underlying QML control must expose a "placeholderText" property for
//   this to have an effect. This is true for standard Qt Quick Controls
//   TextField.
//
static inline ertqt_status ertqt_text_field_set_placeholder(ertqt_object_handle field, const char * utf8_text)
{
    return ertqt_set_property_string(field, "placeholderText", utf8_text);
}

// Public interface for retrieving the text field's placeholder text.
//
// This function reads the "placeholderText" property from the specified text
// field and returns it to the caller as a UTF 8 encoded string.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - buffer: Destination buffer for the UTF 8 encoded text. May be NULL to query
//           the required size without copying data.
// - buffer_size: Size of buffer in bytes.
// - required_size: Out parameter that receives the number of bytes required,
//                  including the terminating NUL character. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully, regardless
//   of whether buffer was large enough to receive the data.
// - ERTQT_ERR_INVALID_ARGUMENT if required_size is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a string.
//
// Notes:
// - If buffer is NULL or buffer_size is too small, no bytes are written to
//   buffer. required_size still receives the full size needed.
//
static inline ertqt_status ertqt_text_field_get_placeholder(ertqt_object_handle field, char * buffer, size_t buffer_size, size_t * required_size)
{
    return ertqt_get_property_string(field, "placeholderText", buffer, buffer_size, required_size);
}

// Public interface for enabling or disabling a text field.
//
// This function sets the "enabled" property on the specified text field object,
// controlling whether the field accepts user input.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - enabled: `false` (disabled) or `true` (enabled).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - When disabled, a typical TextField will not accept focus or input and may
//   be rendered in a disabled visual style.
//
static inline ertqt_status ertqt_text_field_set_enabled(ertqt_object_handle field, bool enabled)
{
    return ertqt_set_property_bool(field, "enabled", enabled);
}

// Public interface for querying whether a text field is enabled.
//
// This function reads the "enabled" property from the specified text field and
// reports its value as `false` (disabled) or `true` (enabled).
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - enabled: Pointer that receives `false` if the field is disabled, `true` if enabled.
//            Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if enabled is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
static inline ertqt_status ertqt_text_field_get_enabled(ertqt_object_handle field, bool * enabled)
{
    return ertqt_get_property_bool(field, "enabled", enabled);
}

// Public interface for controlling text field visibility.
//
// This function sets the "visible" property on the specified text field, which
// determines whether the control is drawn in the UI.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - visible: `false` (not visible) or `true` (visible).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The "visible" property is provided by the underlying visual item type.
//   It is commonly available on QML Items and standard controls.
//
static inline ertqt_status ertqt_text_field_set_visible(ertqt_object_handle field, bool visible)
{
    return ertqt_set_property_bool(field, "visible", visible);
}

// Public interface for querying text field visibility.
//
// This function reads the "visible" property from the specified text field and
// reports its value as `false` (not visible) or `true` (visible).
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - enabled: Pointer that receives `false` if the field is not visible, `true` if enabled.
//            Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if visible is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
static inline ertqt_status ertqt_text_field_get_visible(ertqt_object_handle field, bool * visible)
{
    return ertqt_get_property_bool(field, "visible", visible);
}

// Public interface for configuring the maximum text length.
//
// This function sets the "maximumLength" property on the specified text field,
// which constrains the number of characters the user can enter.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - max_length: Maximum number of characters allowed. A non positive value is
//               usually interpreted as "no limit" by Qt, but behaviour depends
//               on the underlying control.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The underlying QML control must expose a "maximumLength" property for this
//   to have an effect. This is true for standard Qt Quick Controls TextField.
//
static inline ertqt_status ertqt_text_field_set_max_length(ertqt_object_handle field, int max_length)
{
    return ertqt_set_property_int(field, "maximumLength", max_length);
}

// Public interface for querying the configured maximum text length.
//
// This function reads the "maximumLength" property from the specified text
// field and reports its value as an integer.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - max_length: Pointer that receives the configured maximum length. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if max_length is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to an int.
//
static inline ertqt_status ertqt_text_field_get_max_length(ertqt_object_handle field, int * max_length)
{
    return ertqt_get_property_int(field, "maximumLength", max_length);
}

// Public interface for configuring the echo mode of a text field.
//
// This function sets the "echoMode" property on the specified text field. The
// value is an integer corresponding to the enum used by the underlying control,
// for example TextInput.Normal, TextInput.Password and so on.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - echo_mode: Integer value describing the desired echo mode, as defined by
//              the QML type used for the field.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - This helper does not interpret the enum values. The caller is responsible
//   for using values that make sense for the specific QML type.
//
static inline ertqt_status ertqt_text_field_set_echo_mode(ertqt_object_handle field, int echo_mode)
{
    return ertqt_set_property_int(field, "echoMode", echo_mode);
}

// Public interface for querying the echo mode of a text field.
//
// This function reads the "echoMode" property from the specified text field and
// reports its value as an integer.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - echo_mode: Pointer that receives the echo mode value. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if echo_mode is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to an int.
//
// Notes:
// - The meaning of the returned value depends on the QML type backing the
//   text field. For a standard TextField it corresponds to TextInput.EchoMode.
//
static inline ertqt_status ertqt_text_field_get_echo_mode(ertqt_object_handle field, int * echo_mode)
{
    return ertqt_get_property_int(field, "echoMode", echo_mode);
}

// Public interface for registering a callback on each text change.
//
// This function registers a C callback that will be invoked whenever the text
// field's text changes. It wraps ertqt_bind_text_changed() for convenience.
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - cb: Function pointer to the callback to invoke when the text changes.
//       Must not be NULL.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - ERTQT_OK if the binding was set up successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if cb is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the underlying binding fails.
//
// Notes:
// - The utf8_text pointer passed to cb is only valid until the callback
//   returns. Callers must copy the string if they need to retain it.
//
static inline ertqt_status ertqt_text_field_on_text_changed(ertqt_object_handle field, ertqt_text_callback cb, void * user_data)
{
    return ertqt_bind_text_changed(field, cb, user_data);
}

// Public interface for registering a callback when editing is finished.
//
// This function registers a C callback that will be invoked when the user has
// finished editing the text field, typically mapped to the "accepted()" signal
// on QML TextField / TextInput controls. It wraps ertqt_bind_editing_finished().
//
// Parameters:
// - field: Opaque handle referencing the target text field QObject.
// - cb: Function pointer to the callback to invoke when editing is finished.
//       Must not be NULL.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - ERTQT_OK if the binding was set up successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if cb is NULL.
// - ERTQT_ERR_INVALID_HANDLE if field does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the underlying binding fails.
//
// Notes:
// - The utf8_text pointer passed to cb is only valid until the callback
//   returns.
//
static inline ertqt_status ertqt_text_field_on_editing_finished(ertqt_object_handle field, ertqt_text_callback cb, void * user_data)
{
    return ertqt_bind_editing_finished(field, cb, user_data);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ERTQT_TEXT_FIELD_H */
