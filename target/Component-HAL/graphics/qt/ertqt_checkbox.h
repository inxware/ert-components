/* ertqt_checkbox.h */

#ifndef ERTQT_CHECKBOX_H
#define ERTQT_CHECKBOX_H

#include "ertqt.h"

#ifdef __cplusplus
extern "C" {
#endif

// Public interface for resolving a checkbox by its objectName.
//
// This convenience wrapper looks up a QObject by its objectName and returns an
// ertqt_object_handle that is expected to refer to a checkbox like control. It
// simply forwards to ertqt_get_object_by_name(), but makes the intent clearer
// at call sites that are specifically dealing with checkboxes.
//
// Parameters:
// - name: UTF 8 encoded objectName of the checkbox in the QML scene. Must not be NULL.
//
// Returns:
// - A non negative ertqt_object_handle value if a matching object was found.
// - ERTQT_ERR_INVALID_ARGUMENT if name is NULL.
// - ERTQT_ERR_NOT_FOUND if no object with the specified name exists in the
//   current object table.
//
// Notes:
// - The returned handle is not validated to ensure that the underlying QObject
//   is actually a checkbox type. Callers should use this only with known
//   checkbox objectNames defined in the QML.
//
static inline ertqt_object_handle ertqt_checkbox_by_name(const char * name)
{
    return ertqt_get_object_by_name(name);
}

// Public interface for setting the checkbox's label text.
//
// This function sets the "text" property on the specified checkbox object to
// the supplied UTF 8 string. It is a convenience wrapper around
// ertqt_set_property_string() that hard codes the property name.
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - utf8_text: NUL terminated UTF 8 string containing the label text. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if utf8_text is NULL.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The underlying QML control must expose a "text" property for this to work.
//   This is true for standard Qt Quick Controls CheckBox.
//
static inline ertqt_status ertqt_checkbox_set_text(ertqt_object_handle checkbox, const char * utf8_text)
{
    return ertqt_set_property_string(checkbox, "text", utf8_text);
}

// Public interface for retrieving the checkbox's label text.
//
// This function reads the "text" property from the specified checkbox and
// returns it to the caller as a UTF 8 encoded string. It is a convenience
// wrapper around ertqt_get_property_string() that hard codes the property name.
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
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
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a string.
//
// Notes:
// - If buffer is NULL or buffer_size is too small, no bytes are written to
//   buffer. required_size still receives the full size needed.
//
static inline ertqt_status ertqt_checkbox_get_text(ertqt_object_handle checkbox, char * buffer, size_t buffer_size, size_t * required_size)
{
    return ertqt_get_property_string(checkbox, "text", buffer, buffer_size, required_size);
}

// Public interface for enabling or disabling a checkbox.
//
// This function sets the "enabled" property on the specified checkbox object,
// controlling whether the checkbox is interactive in the UI.
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - enabled: `false` (disabled) or `true` (enabled).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The underlying QML control must expose an "enabled" property for this to
//   have an effect. This is true for standard Qt Quick Controls CheckBox.
//
static inline ertqt_status ertqt_checkbox_set_enabled(ertqt_object_handle checkbox, bool enabled)
{
    return ertqt_set_property_bool(checkbox, "enabled", enabled);
}

// Public interface for querying whether a checkbox is enabled.
//
// This function reads the "enabled" property from the specified checkbox and
// reports its value as `false` (disabled) or `true` (enabled).
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - enabled: Pointer that receives `false` if the checkbox is disabled,
//            `true` if enabled. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if enabled is NULL.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
// Notes:
// - The conversion uses ertqt_get_property_bool(), which ultimately relies on
//   QVariant::toBool().
//
static inline ertqt_status ertqt_checkbox_get_enabled(ertqt_object_handle checkbox, bool * enabled)
{
    return ertqt_get_property_bool(checkbox, "enabled", enabled);
}

// Public interface for controlling checkbox visibility.
//
// This function sets the "visible" property on the specified checkbox, which
// determines whether the control is drawn in the UI.
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - visible: `false` (not visible) or `true` (visible).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The "visible" property is provided by the underlying visual item type. It
//   is commonly available on QML Items and standard controls.
//
static inline ertqt_status ertqt_checkbox_set_visible(ertqt_object_handle checkbox, bool visible)
{
    return ertqt_set_property_bool(checkbox, "visible", visible);
}

// Public interface for querying checkbox visibility.
//
// This function reads the "visible" property from the specified checkbox and
// reports its value as `false` (not visible) or `true` (visible).
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - visible: Pointer that receives `false` if the checkbox is not visible,
//            `true` if visible. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if visible is NULL.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
static inline ertqt_status ertqt_checkbox_get_visible(ertqt_object_handle checkbox, bool * visible)
{
    return ertqt_get_property_bool(checkbox, "visible", visible);
}

// Public interface for setting the checked state of a checkbox.
//
// This function sets the "checked" property on the specified checkbox, which
// controls whether the checkbox is logically checked.
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - checked: `false` (not checked) or `true` (checked).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - For tristate capable controls, "checked" typically reflects only the
//   boolean aspect of the state. More detailed state handling (such as
//   `checkState`) can be added later if required.
//
static inline ertqt_status ertqt_checkbox_set_checked(ertqt_object_handle checkbox, bool checked)
{
    return ertqt_set_property_bool(checkbox, "checked", checked);
}

// Public interface for querying the checked state of a checkbox.
//
// This function reads the "checked" property from the specified checkbox and
// reports its value as `false` (not checked) or `true` (checked).
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - checked: Pointer that receives `false` if the checkbox is not checked,
//            `true` if checked. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if checked is NULL.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
// Notes:
// - For non tristate checkboxes this fully represents the logical state.
//   Tristate aware handling can be added via a dedicated helper later.
//
static inline ertqt_status ertqt_checkbox_get_checked(ertqt_object_handle checkbox, bool * checked)
{
    return ertqt_get_property_bool(checkbox, "checked", checked);
}

// Public interface for registering a click callback on a checkbox.
//
// This function registers a C callback that will be invoked whenever the
// checkbox's "clicked()" signal is emitted. It is a thin wrapper around
// ertqt_bind_clicked() that is specialised for checkboxes.
//
// Parameters:
// - checkbox: Opaque handle referencing the target checkbox QObject.
// - cb: Function pointer to the callback to invoke when the checkbox is clicked.
//       Must not be NULL.
// - user_data: Opaque pointer passed back to the callback on each invocation.
//
// Returns:
// - ERTQT_OK if the binding was set up successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if cb is NULL.
// - ERTQT_ERR_INVALID_HANDLE if checkbox does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the object has no compatible "clicked()"
//   signal or the connection could not be created.
//
// Notes:
// - The callback is executed on the Qt application's thread and should avoid
//   blocking operations.
// - Multiple callbacks may be registered for the same checkbox by calling this
//   function multiple times.
//
static inline ertqt_status ertqt_checkbox_on_clicked(ertqt_object_handle checkbox, ertqt_void_callback cb, void * user_data)
{
    return ertqt_bind_clicked(checkbox, cb, user_data);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ERTQT_CHECKBOX_H */
