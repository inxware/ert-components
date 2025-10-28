#ifndef _CHEADER_HAL_NVS_H_
#define _CHEADER_HAL_NVS_H_

#include "target_nvs.h"

typedef enum {
    eEhsNvsOk = 0,
    eEhsNvsErrNotFound,
    eEhsNvsErrNoMem,
    eEhsNvsErrInvalidArg,
    eEhsNvsErrCorrupt,
    eEhsNvsErrWriteProtected,
    eEhsNvsErrInvalidState,
    eEhsNvsErrFlashIo,
    eEhsNvsErrPartitionFull,
    eEhsNvsErrNoFreePages,
    eEhsNvsErrValueTooLong,
    eEhsNvsErrPageFull,
    eEhsNvsErrNotInitialized,
    eEhsNvsErrInvalidSize
} eEhsNvsError_t;

typedef ehs_target_nvs_obj_t ehs_nvs_obj_t;

eEhsNvsError_t EhsNvsOpen(ehs_nvs_obj_t* nvs, const ehs_char* namespacex);

eEhsNvsError_t EhsNvsClose(ehs_nvs_obj_t* nvs);

eEhsNvsError_t EhsNvsSetString(ehs_nvs_obj_t* nvs, const ehs_char* key, const ehs_char* value, ehs_uint32 len);

eEhsNvsError_t EhsNvsGetString(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_char* out_value, ehs_uint32 *max_len);

eEhsNvsError_t EhsNvsSetSInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint8 value);

eEhsNvsError_t EhsNvsGetSInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint8* out_value);

eEhsNvsError_t EhsNvsSetUInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint8 value);

eEhsNvsError_t EhsNvsGetUInt8(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint8* out_value);

eEhsNvsError_t EhsNvsSetSInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint16 value);

eEhsNvsError_t EhsNvsGetSInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint16* out_value);

eEhsNvsError_t EhsNvsSetUInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint16 value);

eEhsNvsError_t EhsNvsGetUInt16(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint16* out_value);

eEhsNvsError_t EhsNvsSetSInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint32 value);

eEhsNvsError_t EhsNvsGetSInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint32* out_value);

eEhsNvsError_t EhsNvsSetUInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint32 value);

eEhsNvsError_t EhsNvsGetUInt32(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint32* out_value);

eEhsNvsError_t EhsNvsSetSint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint64 value);

eEhsNvsError_t EhsNvsGetSint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_sint64* out_value);

eEhsNvsError_t EhsNvsSetUint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint64 value);

eEhsNvsError_t EhsNvsGetUint64(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_uint64* out_value);

eEhsNvsError_t EhsNvsSetBool(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_bool value);

eEhsNvsError_t EhsNvsGetBool(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_bool* out_value);

eEhsNvsError_t EhsNvsSetFloat(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_float value);

eEhsNvsError_t EhsNvsGetFloat(ehs_nvs_obj_t* nvs, const ehs_char* key, ehs_float* out_value);

eEhsNvsError_t EhsNvsSetBlob(ehs_nvs_obj_t* nvs, const ehs_char* key, const void* value, ehs_uint32 len);

eEhsNvsError_t EhsNvsGetBlob(ehs_nvs_obj_t* nvs, const ehs_char* key, void* out_value, ehs_uint32 max_len, ehs_uint32* out_len);

eEhsNvsError_t EHsNvsDelete(ehs_nvs_obj_t* nvs, const ehs_char* key);

eEhsNvsError_t EhsNvsErase(ehs_nvs_obj_t* nvs);

eEhsNvsError_t EhsNvsCommit(ehs_nvs_obj_t* nvs);

#endif// _CHEADER_HAL_NVS_H_
