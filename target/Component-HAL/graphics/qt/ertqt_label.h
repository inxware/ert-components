/* ertqt_label.h */

#ifndef ERTQT_LABEL_H
#define ERTQT_LABEL_H

#include "ertqt.h"

#ifdef __cplusplus
extern "C" {
#endif

// Public interface for resolving a label by its objectName.
//
// This convenience wrapper looks up a QObject by its objectName and returns an
// ertqt_object_handle that is expected to refer to a label like control. It
// simply forwards to ertqt_get_object_by_name(), but makes the intent clearer
// at call sites that are specifically dealing with labels.
//
// Parameters:
// - name: UTF 8 encoded objectName of the label in the QML scene. Must not be NULL.
//
// Returns:
// - A non negative ertqt_object_handle value if a matching object was found.
// - ERTQT_ERR_INVALID_ARGUMENT if name is NULL.
// - ERTQT_ERR_NOT_FOUND if no object with the specified name exists in the
//   current object table.
//
// Notes:
// - The returned handle is not validated to ensure that the underlying QObject
//   is actually a label type. Callers should use this only with known label
//   objectNames defined in the QML.
//
static inline ertqt_object_handle ertqt_label_by_name(const char * name)
{
    return ertqt_get_object_by_name(name);
}

// Public interface for setting the label's text.
//
// This function sets the "text" property on the specified label object to the
// supplied UTF 8 string. It is a convenience wrapper around
// ertqt_set_property_string() that hard codes the property name.
//
// Parameters:
// - label: Opaque handle referencing the target label QObject.
// - utf8_text: NUL terminated UTF 8 string containing the label text. Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if utf8_text is NULL.
// - ERTQT_ERR_INVALID_HANDLE if label does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The underlying QML control must expose a "text" property for this to work.
//   This is true for standard Qt Quick Controls Label.
//
static inline ertqt_status ertqt_label_set_text(ertqt_object_handle label, const char * utf8_text)
{
    return ertqt_set_property_string(label, "text", utf8_text);
}

// Public interface for retrieving the label's text.
//
// This function reads the "text" property from the specified label and returns
// it to the caller as a UTF 8 encoded string. It is a convenience wrapper
// around ertqt_get_property_string() that hard codes the property name.
//
// Parameters:
// - label: Opaque handle referencing the target label QObject.
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
// - ERTQT_ERR_INVALID_HANDLE if label does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a string.
//
// Notes:
// - If buffer is NULL or buffer_size is too small, no bytes are written to
//   buffer. required_size still receives the full size needed.
//
static inline ertqt_status ertqt_label_get_text(ertqt_object_handle label, char * buffer, size_t buffer_size, size_t * required_size)
{
    return ertqt_get_property_string(label, "text", buffer, buffer_size, required_size);
}

// Public interface for enabling or disabling a label.
//
// This function sets the "enabled" property on the specified label object,
// which may influence how the label is rendered (for example, disabled style).
//
// Parameters:
// - label: Opaque handle referencing the target label QObject.
// - enabled: `false` (disabled) or `true` (enabled).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if label does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - A label is normally non interactive, so "enabled" typically only affects
//   visual appearance rather than behaviour.
//
static inline ertqt_status ertqt_label_set_enabled(ertqt_object_handle label, bool enabled)
{
    return ertqt_set_property_bool(label, "enabled", enabled);
}

// Public interface for querying whether a label is enabled.
//
// This function reads the "enabled" property from the specified label and
// reports its value as `false` (disabled) or `true` (enabled).
//
// Parameters:
// - label: Opaque handle referencing the target label QObject.
// - enabled: Pointer that receives `false` if the label is disabled, `true` if enabled.
//            Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if enabled is NULL.
// - ERTQT_ERR_INVALID_HANDLE if label does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
static inline ertqt_status ertqt_label_get_enabled(ertqt_object_handle label, bool * enabled)
{
    return ertqt_get_property_bool(label, "enabled", enabled);
}

// Public interface for controlling label visibility.
//
// This function sets the "visible" property on the specified label, which
// determines whether the label is drawn in the UI.
//
// Parameters:
// - label: Opaque handle referencing the target label QObject.
// - visible: `false` (not visible) or `true` (visible).
//
// Returns:
// - ERTQT_OK if the property was set successfully.
// - ERTQT_ERR_INVALID_HANDLE if label does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property assignment fails in Qt.
//
// Notes:
// - The "visible" property is provided by the underlying visual item type. It
//   is commonly available on QML Items and standard controls.
//
static inline ertqt_status ertqt_label_set_visible(ertqt_object_handle label, bool visible)
{
    return ertqt_set_property_bool(label, "visible", visible);
}

// Public interface for querying label visibility.
//
// This function reads the "visible" property from the specified label and
// reports its value as `false` (not visible) or `true` (visible).
//
// Parameters:
// - label: Opaque handle referencing the target label QObject.
// - enabled: Pointer that receives `false` if the label is not visible, `true` if enabled.
//            Must not be NULL.
//
// Returns:
// - ERTQT_OK if the property was retrieved and converted successfully.
// - ERTQT_ERR_INVALID_ARGUMENT if visible is NULL.
// - ERTQT_ERR_INVALID_HANDLE if label does not resolve to a valid QObject.
// - ERTQT_ERR_BACKEND_FAILURE if the property lookup fails.
// - ERTQT_ERR_TYPE_MISMATCH if the property cannot be converted to a bool.
//
static inline ertqt_status ertqt_label_get_visible(ertqt_object_handle label, bool * visible)
{
    return ertqt_get_property_bool(label, "visible", visible);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ERTQT_LABEL_H */
