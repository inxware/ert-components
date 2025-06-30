
#include "globals.h"
#include "hal_ml.h"
#include "hal-api.h"
#include "tflite_model.h"


EhsML_Err EhsML_Create(EhsML_Context* ctx, const ehs_char* model_path, EhsML_Type model_type, ehs_float conf_thres, ehs_sint32 thread_count)
{
    EhsML_Err err = EHS_ML_FAILED;

    if(!ctx) goto ml_fail;

    if(ctx->ml_model_ctx){
        err = EHS_ML_MODEL_IN_USE;
        goto ml_fail;
    }

    if(!model_path || *model_path == '\0'){
        err = EHS_ML_MODEL_PATH_ERR;
        goto ml_fail;
    }

    TfLiteModelCtx* tfl_model_ctx = (TfLiteModelCtx*)EhsHMem_permAlloc(sizeof(TfLiteModelCtx));;
    if(!tfl_model_ctx){
        err = EHS_ML_MEMORY_ERR;
        goto ml_fail;
    }
    EhsMemset(tfl_model_ctx, 0, sizeof(TfLiteModelCtx));
    tfl_model_ctx->conf_thres = (float)conf_thres; 
    TfLiteErr tfErr = TfLiteModel_Create(tfl_model_ctx, model_path, thread_count);
    if(tfErr == TF_LITE_FAILED){
        err = EHS_ML_MODEL_LOAD_ERR;
        EhsHMem_permFree(tfl_model_ctx);
        EhsMemset(ctx, 0, sizeof(EhsML_Context));
        goto ml_fail;
    }
    // store model context
    ctx->ml_model_ctx = (void*)tfl_model_ctx;
    ctx->conf_thres = conf_thres;
    ctx->type = model_type;

    return EHS_ML_OK;
ml_fail:
    return err;
}

void EhsML_Destroy(EhsML_Context* ctx)
{
    if(ctx){
        if(ctx->ml_model_ctx) {
            TfLiteModel_Destroy((TfLiteModelCtx*)ctx->ml_model_ctx);
            EhsHMem_permFree((TfLiteModelCtx*)ctx->ml_model_ctx);
        }
        EhsMemset(ctx, 0, sizeof(EhsML_Context));
    }
}

EhsML_Err EhsML_SetInputData(EhsML_Context* ctx, const void* data, ehs_uint32 size)
{
    if(!ctx || !ctx->ml_model_ctx) return EHS_ML_FAILED;
    return TfLiteModel_Set_Input_Data((TfLiteModelCtx*)ctx->ml_model_ctx, data, size) == TF_LITE_OK ? EHS_ML_OK : EHS_ML_FAILED;
}

// TODO
// EhsML_RunOutputData(EhsML_Context* ctx, void* data, ehs_uint32 size);

// Creates object detections results as a json array  e.g.
// {"type":0,...,"det":[{"cls":0,"cnf":0.91,"x":20,"y":21,"w":22,"h":23},{"cls":1,"cnf":0.93,"x":30,"y":31,"w":32,"h":33}, ... ]}
ehs_bool TfLiteArrayJsonObjectDetection(EhsML_Context* ctx, TfLiteBoxDetections* detections, ehs_char* json, ehs_uint32 size){
    ehs_uint32 used = snprintf(json, size, "{\"type\":%d,\"proc_time\":%d,\"err_no\":%d,\"det_cnt\":%d,\"det\":[", 
                                ctx->type, detections->processing_time_ms, detections->error_id, detections->box_detections_count);
    if (used >= size) return EHS_FALSE;

    bool first = true;
    for (int i = 0; i < detections->box_detections_count; i++) {
        TfLiteBoxDetection* d = &(detections->box_detections[i]);
        // Try to write directly into the remaining json buffer
        int remaining = size - used - 2; // reserve space for "]}" and null terminator
        if (remaining <= 0) break;
        int len = snprintf(json + used, remaining,
            "%s{\"cls\":%d,\"cnf\":%.2f,\"x\":%u,\"y\":%u,\"w\":%u,\"h\":%u}",
            first ? "" : ",", d->class_id, d->confidence, d->x, d->y, d->w, d->h);
        if (len <= 0 || len >= remaining) break; // Avoid overflow
        used += (ehs_uint32)len;
        first = false;
    }
    
    // Finalize JSON object
    if (used + 2 >= size) return EHS_FALSE;
    json[used++] = ']';
    json[used++] = '}';
    json[used] = '\0';

    return EHS_TRUE;
}

// Creates object detections results as a flat json  e.g.
// {"type":0,...,"cls0":0,"cnf0":0.91,"x0":20,"y0":21,"w0":22,"h0":23,"cls1":0,"cnf1":0.91,"x1":20,"y1":21,"w1":22,"h1":23, ... , "clsN":0 ... }
ehs_bool TfLiteFlatJsonObjectDetection(EhsML_Context* ctx, TfLiteBoxDetections* detections, ehs_char* json, ehs_uint32 size){
    ehs_uint32 used = snprintf(json, size, "{\"type\":%d,\"proc_time\":%d,\"err_no\":%d,\"det_cnt\":%d", 
                               ctx->type, detections->processing_time_ms, detections->error_id, detections->box_detections_count);
    if (used >= size) return EHS_FALSE;

    for (int i = 0; i < detections->box_detections_count; i++) {
        TfLiteBoxDetection* d = &(detections->box_detections[i]);

        int remaining = size - used - 1; // reserve space for closing } and null terminator
        if (remaining <= 0) break;

        int len = snprintf(json + used, remaining,
            ",\"cls%d\":%d,\"cnf%d\":%.2f,\"x%d\":%u,\"y%d\":%u,\"w%d\":%u,\"h%d\":%u",
            i, d->class_id,
            i, d->confidence,
            i, d->x,
            i, d->y,
            i, d->w,
            i, d->h);

        if (len <= 0 || len >= remaining) break;

        used += (ehs_uint32)len;
    }

    // Close the JSON object
    if (used + 1 >= size) return EHS_FALSE;
    json[used++] = '}';
    json[used] = '\0'; // null-terminate the string

    return EHS_TRUE;
}

EhsML_Err EhsML_RunOutputJson(EhsML_Context* ctx, ehs_char* json, ehs_uint32 size)
{
    if(!ctx || !ctx->ml_model_ctx || !json || !size) return EHS_ML_FAILED;

    switch (ctx->type)
    { 
    case EHS_ML_OBJ_DETECTOR:{
        ////////////////////////////////////////////////////////////////////////
        //* Object Detector *  e.g YOLOv5, YOLOv8, SSD MobileNet, CenterNet
        // Output:
        // Bounding boxes + class + confidence (e.g., x, y, w, h, class, score/confidence)
        TfLiteBoxDetections detections = {0};
        TfLiteErr err = TfLiteModel_Run_Detector((TfLiteModelCtx*)ctx->ml_model_ctx, &detections);
        if(err != TF_LITE_OK) return EHS_ML_FAILED;
        if(ctx->enable_flat_json == EHS_TRUE){
            if(EHS_FALSE == TfLiteFlatJsonObjectDetection(ctx, &detections, json, size)) return EHS_ML_FAILED;
        }else{
            if(EHS_FALSE == TfLiteArrayJsonObjectDetection(ctx, &detections, json, size)) return EHS_ML_FAILED;
        }
        
        break;
    }
    case EHS_ML_CLASSIFIER:{
        return EHS_ML_FAILED; // @TODO - implement classifier
        break;
    }
    /////////////////////////////////////////
    // @TODO - handle other model types
    default:
        break;
    }

    return EHS_ML_OK;
}