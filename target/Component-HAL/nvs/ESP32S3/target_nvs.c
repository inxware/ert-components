#include "hal_nvs.h"

eEhsNvsError_t EhsNvsOpen(ehs_nvs_obj_t* nvs, const ehs_char* namespace)
{
    esp_err_t err;
    err = nvs_open(namespace, NVS_READWRITE, nvs);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_FAIL:
            return eEhsNvsErrCorrupt;
        case ESP_ERR_NVS_NOT_INITIALIZED:
            return eEhsNvsErrNotInitialized;
        case ESP_ERR_NVS_PART_NOT_FOUND:
            return eEhsNvsErrNotFound;
        case ESP_ERR_NVS_NOT_FOUND:
            return eEhsNvsErrNotFound;
        case ESP_ERR_NVS_INVALID_NAME:
            return eEhsNvsErrInvalidArg;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
            return eEhsNvsErrNoFreePages;
        case ESP_ERR_NO_MEM:
            return eEhsNvsErrNoMem;
        default:
            return eEhsNvsErrFlashIo;
    }
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsClose(ehs_nvs_obj_t* nvs)
{
    nvs_close(*nvs);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetString(ehs_nvs_obj_t* nvs, const ehs_char* key, const ehs_char* value, ehs_uint32 len)
{
    esp_err_t err;
    err = nvs_set_str(*nvs, key, value);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        case ESP_ERR_NVS_READ_ONLY:
            return eEhsNvsErrWriteProtected;
        case ESP_ERR_NVS_INVALID_NAME:
            return eEhsNvsErrInvalidArg;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
            return eEhsNvsErrNoFreePages;
        case ESP_ERR_NVS_REMOVE_FAILED:
            return eEhsNvsErrFlashIo;
        case ESP_ERR_NVS_VALUE_TOO_LONG:
            return eEhsNvsErrValueTooLong;
    }
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetString(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_char* out_value, ehs_uint32 *max_len)
{
    esp_err_t err;
    err = nvs_get_str(*nvs, key, out_value, max_len);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_FAIL:
            return eEhsNvsErrCorrupt;
        case ESP_ERR_NVS_NOT_FOUND:
            return eEhsNvsErrNotFound;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        case ESP_ERR_NVS_INVALID_NAME:
            return eEhsNvsErrInvalidArg;
        case ESP_ERR_NVS_INVALID_LENGTH:
            return eEhsNvsErrInvalidSize;
        default:
            return eEhsNvsErrFlashIo;
    }
    return eEhsNvsOk;
}

#define EHS_NVS_SET_IMPL(nvs_set_func, nvs, key, value) do { \
    esp_err_t err; \
    err = nvs_set_func(*nvs, key, value); \
    switch (err) { \
        case ESP_OK: \
            return eEhsNvsOk; \
        case ESP_FAIL: \
            return eEhsNvsErrCorrupt; \
        case ESP_ERR_NVS_INVALID_HANDLE: \
            return eEhsNvsErrInvalidState; \
        case ESP_ERR_NVS_READ_ONLY: \
            return eEhsNvsErrWriteProtected; \
        case ESP_ERR_NVS_INVALID_NAME: \
            return eEhsNvsErrInvalidArg; \
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE: \
            return eEhsNvsErrNoFreePages; \
        case ESP_ERR_NVS_REMOVE_FAILED: \
            return eEhsNvsErrFlashIo; \
    } \
} while(0)

#define EHS_NVS_GET_IMPL(nvs_get_func, nvs, key, out_value) do { \
    esp_err_t err; \
    err = nvs_get_func(*nvs, key, out_value); \
    switch (err) { \
        case ESP_OK: \
            return eEhsNvsOk; \
        case ESP_FAIL: \
            return eEhsNvsErrCorrupt; \
        case ESP_ERR_NVS_NOT_FOUND: \
            return eEhsNvsErrNotFound; \
        case ESP_ERR_NVS_INVALID_HANDLE: \
            return eEhsNvsErrInvalidState; \
        case ESP_ERR_NVS_INVALID_NAME: \
            return eEhsNvsErrInvalidArg; \
        case ESP_ERR_NVS_INVALID_LENGTH: \
            return eEhsNvsErrInvalidSize; \
        default: \
            return eEhsNvsErrFlashIo; \
    } \
} while(0)

eEhsNvsError_t EhsNvsSetSInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint8 value)
{
    EHS_NVS_SET_IMPL(nvs_set_i8, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetSInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint8* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_i8, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetUInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint8 value)
{
    EHS_NVS_SET_IMPL(nvs_set_u8, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetUInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint8* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_u8, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetSInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint16 value)
{
    EHS_NVS_SET_IMPL(nvs_set_i16, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetSInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint16* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_i16, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetUInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint16 value)
{
    EHS_NVS_SET_IMPL(nvs_set_u16, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetUInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint16* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_u16, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetSInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint32 value)
{
    EHS_NVS_SET_IMPL(nvs_set_i32, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetSInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint32* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_i32, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetUInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint32 value)
{
    EHS_NVS_SET_IMPL(nvs_set_u32, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetUInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint32* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_u32, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetSint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint64 value)
{
    EHS_NVS_SET_IMPL(nvs_set_i64, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetSint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint64* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_i64, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetUint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint64 value)
{
    EHS_NVS_SET_IMPL(nvs_set_u64, nvs, key, value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetUint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint64* out_value)
{
    EHS_NVS_GET_IMPL(nvs_get_u64, nvs, key, out_value);
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsSetBool(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_bool value)
{
    return EhsNvsSetUInt8(nvs, key, (value == EHS_TRUE) ? 1 : 0);
}

eEhsNvsError_t EhsNvsGetBool(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_bool* out_value)
{
    ehs_uint8 val = 0;
    eEhsNvsError_t err = EhsNvsGetUInt8(nvs, key, (ehs_uint8*)&val);
    if (err == eEhsNvsOk) {
        *out_value = (val >= 1) ? EHS_TRUE : EHS_FALSE;
    }
    return err;
}

eEhsNvsError_t EhsNvsSetFloat(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_float value)
{
    return EhsNvsSetBlob(nvs, key, &value, sizeof(ehs_float));
}

eEhsNvsError_t EhsNvsGetFloat(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_float* out_value)
{
    ehs_float val = 0;
    ehs_uint32 len = 0;
    eEhsNvsError_t err = EhsNvsGetBlob(nvs, key, &val, sizeof(ehs_float), &len);
    if (err == eEhsNvsOk && len == sizeof(ehs_float)) {
        *out_value = val;
    }
    else {
        err = eEhsNvsErrInvalidSize;
    }
    return err;
}

eEhsNvsError_t EhsNvsSetBlob(ehs_nvs_obj_t* nvs, const ehs_char* key, const void* value, ehs_uint32 len)
{
    esp_err_t err;
    err = nvs_set_blob(*nvs, key, value, len);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_FAIL:
            return eEhsNvsErrCorrupt;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        case ESP_ERR_NVS_READ_ONLY:
            return eEhsNvsErrWriteProtected;
        case ESP_ERR_NVS_INVALID_NAME:
            return eEhsNvsErrInvalidArg;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
            return eEhsNvsErrNoFreePages;
        case ESP_ERR_NVS_REMOVE_FAILED:
            return eEhsNvsErrFlashIo;
        case ESP_ERR_NVS_VALUE_TOO_LONG:
            return eEhsNvsErrValueTooLong;
    }
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsGetBlob(ehs_nvs_obj_t* nvs, const ehs_char* key, void* out_value, ehs_uint32 max_len, ehs_uint32* out_len)
{
    esp_err_t err;
    err = nvs_get_blob(*nvs, key, out_value, out_len);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_FAIL:
            return eEhsNvsErrCorrupt;
        case ESP_ERR_NVS_NOT_FOUND:
            return eEhsNvsErrNotFound;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        case ESP_ERR_NVS_INVALID_NAME:
            return eEhsNvsErrInvalidArg;
        case ESP_ERR_NVS_INVALID_LENGTH:
            return eEhsNvsErrInvalidSize;
        default:
            return eEhsNvsErrFlashIo;
    }
    return eEhsNvsOk;
}

eEhsNvsError_t EHsNvsDelete(ehs_nvs_obj_t* nvs, const ehs_char* key)
{
    esp_err_t err;
    err = nvs_erase_key(*nvs, key);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_FAIL:
            return eEhsNvsErrCorrupt;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        case ESP_ERR_NVS_READ_ONLY:
            return eEhsNvsErrWriteProtected;
        case ESP_ERR_NVS_NOT_FOUND:
            return eEhsNvsErrNotFound;
        default:
            return eEhsNvsErrFlashIo;
    }
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsErase(ehs_nvs_obj_t* nvs)
{
    esp_err_t err;
    err = nvs_erase_all(*nvs);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_FAIL:
            return eEhsNvsErrCorrupt;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        case ESP_ERR_NVS_READ_ONLY:
            return eEhsNvsErrWriteProtected;
        default:
            return eEhsNvsErrFlashIo;
    }
    return eEhsNvsOk;
}

eEhsNvsError_t EhsNvsCommit(ehs_nvs_obj_t* nvs)
{
    esp_err_t err;
    err = nvs_commit(*nvs);
    switch (err) {
        case ESP_OK:
            return eEhsNvsOk;
        case ESP_ERR_NVS_INVALID_HANDLE:
            return eEhsNvsErrInvalidState;
        default:
            return eEhsNvsErrFlashIo;
    }
    return eEhsNvsOk;
}
