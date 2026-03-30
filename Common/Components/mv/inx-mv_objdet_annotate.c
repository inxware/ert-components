//ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-mv_objdet_annotate.h"
//ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_mv.h"
#include <stdlib.h>   /* strtol, strtof */
#include <string.h>   /* strstr, strlen, snprintf */
#include <stdio.h>    /* snprintf */

//ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_mv_objdet_annotate_state
{
    ehs_sint32     box_thickness;
    ehs_bool       show_label;
    ehs_bool       copy_frame;
    EhsCameraFrame anno_frame;   /* allocated only when copy_frame=EHS_TRUE */
} inx_mv_objdet_annotate_state_type;
//ICB STATE VAR MACRO END -- DO NOT ALTER

typedef enum {
    ANNOTATE_OK = 0,
    ANNOTATE_FAILED,
    ANNOTATE_NO_DETECTIONS
} EhsAnnotateErr;

//ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(mv_objdet_annotate)
EHS_FB_FUNCTION_ENTRY("annotate", 0x01, mv_objdet_annotate_annotate)
EHS_FB_FUNCTIONS_END
//ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

//ICB FRIENDLY LABELS MACRO START -- DO NOT ALTER
#define INX_mv_objdet_annotate_ARG_annotate_frame_id    1
#define INX_mv_objdet_annotate_ARG_annotate_detections  2
#define INX_mv_objdet_annotate_ARG_annotate_errno              1
#define INX_mv_objdet_annotate_ARG_annotate_done               1
#define INX_mv_objdet_annotate_ARG_annotate_error              2
#define INX_mv_objdet_annotate_ARG_annotate_annotated_frame_id 2
//ICB FRIENDLY LABELS MACRO END -- DO NOT ALTER

//ICB PARAMETER DEFAULTS MACRO START -- DO NOT ALTER
#define INX_FB_mv_objdet_annotate_box_thickness  2
#define INX_FB_mv_objdet_annotate_show_label     1
#define INX_FB_mv_objdet_annotate_copy_frame     0
//ICB PARAMETER DEFAULTS MACRO END -- DO NOT ALTER

/* --------------------------------------------------------------------
 * Colour palette — 16 distinct colours, indexed by class_id % 16.
 * Values are (R, G, B), 0-255.
 * -------------------------------------------------------------------- */
static const ehs_uint8 kAnnotatePalette[16][3] = {
    {255,  64,  64},  /*  0  red          */
    { 64, 220,  64},  /*  1  green        */
    { 64, 128, 255},  /*  2  blue         */
    {255, 220,  32},  /*  3  yellow       */
    {255, 128,  32},  /*  4  orange       */
    {220,  64, 220},  /*  5  magenta      */
    { 32, 210, 210},  /*  6  cyan         */
    { 64, 255, 220},  /*  7  mint         */
    {255, 160,  64},  /*  8  amber        */
    {128, 255, 128},  /*  9  light green  */
    {160, 160, 255},  /* 10  lavender     */
    {255, 140, 140},  /* 11  salmon       */
    {128, 230, 230},  /* 12  light cyan   */
    {255, 160, 220},  /* 13  pink         */
    {180, 180,  48},  /* 14  olive        */
    {180,  64, 180},  /* 15  purple       */
};

/* --------------------------------------------------------------------
 * Minimal JSON field scanner — no malloc, no JSMN dependency.
 *
 * Searches for a JSON key of the form  "key":  (with enclosing quotes)
 * and returns a pointer to the first character of the value, or NULL.
 * Safe to call with json==NULL.
 * -------------------------------------------------------------------- */
static const char* json_find_value(const char* json, const char* key_with_quotes)
{
    if (!json || !key_with_quotes) return NULL;
    const char* p = strstr(json, key_with_quotes);
    if (!p) return NULL;
    p += strlen(key_with_quotes);
    /* skip optional whitespace after the colon that is already in key */
    while (*p == ' ' || *p == '\t') p++;
    return p;
}

/* Extract a float value for a JSON key like "\"x0\":".
 * Returns def_val if key is not found. */
static float json_get_float(const char* json, const char* key, float def_val)
{
    const char* p = json_find_value(json, key);
    if (!p) return def_val;
    char* end;
    float v = strtof(p, &end);
    return (end == p) ? def_val : v;
}

/* Extract an integer value. Returns def_val if not found. */
static int json_get_int(const char* json, const char* key, int def_val)
{
    const char* p = json_find_value(json, key);
    if (!p) return def_val;
    char* end;
    long v = strtol(p, &end, 10);
    return (end == p) ? def_val : (int)v;
}

/* Extract a quoted string value into buf[bufsize]. Returns 1 on success. */
static int json_get_str(const char* json, const char* key,
                         char* buf, int bufsize)
{
    const char* p = json_find_value(json, key);
    if (!p || *p != '"') return 0;
    p++;  /* skip opening quote */
    int i = 0;
    while (*p && *p != '"' && i < bufsize - 1) buf[i++] = *p++;
    buf[i] = '\0';
    return 1;
}

/* --------------------------------------------------------------------
 * Detection struct and array parser.
 * Supports both JSON formats emitted by EhsML_ObjDet_Json_FromDetections:
 *
 *   Object-array format (default):
 *     {"type":1,"det_cnt":2,"res":[
 *       {"cls":0,"lbl":"person","cnf":0.87,"x":320,"y":240,"w":100,"h":80},
 *       ...
 *     ]}
 *
 *   Flat format (enable_flat_json=true):
 *     {"type":1,"det_cnt":2,
 *      "cls0":0,"lbl0":"person","cnf0":0.87,"x0":320,"y0":240,"w0":100,"h0":80,
 *      ...}
 * -------------------------------------------------------------------- */

#define ANNOTATE_MAX_DETS  50
#define ANNOTATE_LBL_MAX   24

typedef struct {
    int   cls;
    float cnf;
    float cx, cy, w, h;       /* centre x,y + full width/height in pixels */
    char  lbl[ANNOTATE_LBL_MAX];
} AnnotateDet;

/* Parse the array-format "res" section.
 * p must point to the character after the opening '['. */
static int parse_array_format(const char* p, AnnotateDet* dets, int max)
{
    int count = 0;
    while (*p && count < max) {
        /* advance to next '{' — start of a detection object */
        while (*p && *p != '{' && *p != ']') p++;
        if (!*p || *p == ']') break;
        p++;  /* skip '{' */

        /* record the start of this object's content */
        const char* obj = p;

        /* Find the matching '}' — objects are flat (no nesting) */
        while (*p && *p != '}') p++;
        /* p now points at '}' or end-of-string */

        AnnotateDet* d = &dets[count];
        d->cls = json_get_int(obj,   "\"cls\":",  0);
        d->cnf = json_get_float(obj, "\"cnf\":",  0.0f);
        d->cx  = json_get_float(obj, "\"x\":",    0.0f);
        d->cy  = json_get_float(obj, "\"y\":",    0.0f);
        d->w   = json_get_float(obj, "\"w\":",    0.0f);
        d->h   = json_get_float(obj, "\"h\":",    0.0f);
        if (!json_get_str(obj, "\"lbl\":", d->lbl, ANNOTATE_LBL_MAX))
            d->lbl[0] = '\0';

        if (d->w > 0.0f && d->h > 0.0f) count++;
        if (*p) p++;  /* skip '}' */
    }
    return count;
}

/* Parse the flat format using indexed keys like "x0", "cls1", etc. */
static int parse_flat_format(const char* json, int det_cnt,
                              AnnotateDet* dets, int max)
{
    int count = 0;
    char key[32];
    int limit = (det_cnt < max) ? det_cnt : max;
    for (int i = 0; i < limit; i++) {
        AnnotateDet* d = &dets[i];
        snprintf(key, sizeof(key), "\"cls%d\":",  i); d->cls = json_get_int(json, key, 0);
        snprintf(key, sizeof(key), "\"cnf%d\":",  i); d->cnf = json_get_float(json, key, 0.0f);
        snprintf(key, sizeof(key), "\"x%d\":",    i); d->cx  = json_get_float(json, key, 0.0f);
        snprintf(key, sizeof(key), "\"y%d\":",    i); d->cy  = json_get_float(json, key, 0.0f);
        snprintf(key, sizeof(key), "\"w%d\":",    i); d->w   = json_get_float(json, key, 0.0f);
        snprintf(key, sizeof(key), "\"h%d\":",    i); d->h   = json_get_float(json, key, 0.0f);
        snprintf(key, sizeof(key), "\"lbl%d\":",  i);
        if (!json_get_str(json, key, d->lbl, ANNOTATE_LBL_MAX))
            d->lbl[0] = '\0';
        if (d->w > 0.0f && d->h > 0.0f) count++;
    }
    return count;
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(mv_objdet_annotate)
{
    EHS_FB_IDENTIFY_MEMORY = sizeof(inx_mv_objdet_annotate_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

//ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(mv_objdet_annotate)
{
    inx_mv_objdet_annotate_state_type* state =
        (inx_mv_objdet_annotate_state_type*)EHS_FB_INIT_CONTEXT;

    state->box_thickness = INX_FB_mv_objdet_annotate_box_thickness;
    state->show_label    = INX_FB_mv_objdet_annotate_show_label;
    state->copy_frame    = INX_FB_mv_objdet_annotate_copy_frame;
    EhsMemset(&state->anno_frame, 0, sizeof(EhsCameraFrame));

    const char* pParams = EHS_FB_INIT_PARAMETERS;
    if (pParams) {
        pParams = EhsGetSint32FromString(&state->box_thickness, pParams);
        ehs_sint32 sl = state->show_label;
        pParams = EhsGetSint32FromString(&sl, pParams);
        state->show_label = (sl != 0) ? EHS_TRUE : EHS_FALSE;
        ehs_sint32 cf = state->copy_frame;
        pParams = EhsGetSint32FromString(&cf, pParams);
        state->copy_frame = (cf != 0) ? EHS_TRUE : EHS_FALSE;
    }

    if (state->box_thickness < 1) state->box_thickness = 1;
    if (state->box_thickness > 10) state->box_thickness = 10;

    if (state->copy_frame) {
        EhsCameraFrameCreate(&state->anno_frame);
        EhsCameraFrameAdd(&state->anno_frame);
    }

    return EHS_TRUE;
}
//ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

//ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(mv_objdet_annotate)
{
    inx_mv_objdet_annotate_state_type* state =
        (inx_mv_objdet_annotate_state_type*)EHS_FB_DESTROY_CONTEXT;
    if (state->copy_frame) {
        EhsCameraFrameRemove(&state->anno_frame);
        EhsCameraFrameDestroy(&state->anno_frame);
    }
    return EHS_TRUE;
}
//ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER THIS LINE

//ICB FUNCTION annotate MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(mv_objdet_annotate_annotate)
{
    inx_mv_objdet_annotate_state_type* state =
        (inx_mv_objdet_annotate_state_type*)EHS_FB_RUN_CONTEXT;

    EhsAnnotateErr err = ANNOTATE_OK;
    ehs_sint32 frame_id = -1;
    ehs_sint32 out_frame_id = -1;
    const char* json = NULL;

    /* --- read inputs --- */
    if (EHS_FB_IN_CONNECTED_API2(INX_mv_objdet_annotate_ARG_annotate_frame_id))
        frame_id = EHS_FB_IN_I_API2(INX_mv_objdet_annotate_ARG_annotate_frame_id);
    if (EHS_FB_IN_CONNECTED_API2(INX_mv_objdet_annotate_ARG_annotate_detections))
        json = (const char*)EHS_FB_IN_S_API2(INX_mv_objdet_annotate_ARG_annotate_detections);

    if (frame_id < 0 || !json) { err = ANNOTATE_FAILED; goto done; }

    EhsCameraFrame* src = EhsCameraFrameGetById(frame_id);
    if (!src) { err = ANNOTATE_FAILED; goto done; }

    /* Determine which frame to annotate */
    EhsCameraFrame* frame;
    if (state->copy_frame) {
        if (!EhsCameraFrameCopy(src, &state->anno_frame))
            { err = ANNOTATE_FAILED; goto done; }
        frame = &state->anno_frame;
        out_frame_id = state->anno_frame.id;
    } else {
        frame = src;
        out_frame_id = frame_id;
    }

    /* --- parse det_cnt --- */
    int det_cnt = json_get_int(json, "\"det_cnt\":", 0);
    if (det_cnt <= 0) { err = ANNOTATE_NO_DETECTIONS; goto done; }
    if (det_cnt > ANNOTATE_MAX_DETS) det_cnt = ANNOTATE_MAX_DETS;

    /* --- parse detections --- */
    AnnotateDet dets[ANNOTATE_MAX_DETS];
    int n_dets = 0;
    const char* res_ptr = strstr(json, "\"res\":[");
    if (res_ptr) {
        n_dets = parse_array_format(res_ptr + 7, dets, ANNOTATE_MAX_DETS);
    } else {
        n_dets = parse_flat_format(json, det_cnt, dets, ANNOTATE_MAX_DETS);
    }

    if (n_dets == 0) { err = ANNOTATE_NO_DETECTIONS; goto done; }

    /* Ensure the frame data is in CPU memory before drawing */
    EhsCameraFrameEnsureCPU(frame);

    /* --- draw each detection --- */
    for (int i = 0; i < n_dets; i++) {
        AnnotateDet* d = &dets[i];
        int ci = ((d->cls % 16) + 16) % 16;  /* handles negative cls gracefully */
        ehs_uint8 r = kAnnotatePalette[ci][0];
        ehs_uint8 g = kAnnotatePalette[ci][1];
        ehs_uint8 b = kAnnotatePalette[ci][2];

        /* convert centre+size to corner coordinates */
        ehs_sint32 x1 = (ehs_sint32)(d->cx - d->w * 0.5f);
        ehs_sint32 y1 = (ehs_sint32)(d->cy - d->h * 0.5f);
        ehs_sint32 x2 = (ehs_sint32)(d->cx + d->w * 0.5f);
        ehs_sint32 y2 = (ehs_sint32)(d->cy + d->h * 0.5f);

        /* build label string: "person 87%" */
        char tag[ANNOTATE_LBL_MAX + 8];
        const char* label_ptr = NULL;
        if (state->show_label) {
            int pct = (int)(d->cnf * 100.0f + 0.5f);
            if (pct > 100) pct = 100;
            if (d->lbl[0])
                snprintf(tag, sizeof(tag), "%s %d%%", d->lbl, pct);
            else
                snprintf(tag, sizeof(tag), "cls%d %d%%", d->cls, pct);
            label_ptr = tag;
        }

        EhsCameraFrameDrawBBox(frame, x1, y1, x2, y2,
                                r, g, b,
                                state->box_thickness,
                                label_ptr);
        /* Individual box errors are tolerated — continue with remaining boxes */
    }

done:
    if (EHS_FB_OUT_CONNECTED_API2(INX_mv_objdet_annotate_ARG_annotate_errno))
        EHS_FB_OUT_I_API2(INX_mv_objdet_annotate_ARG_annotate_errno) = (ehs_sint32)err;

    if (EHS_FB_OUT_CONNECTED_API2(INX_mv_objdet_annotate_ARG_annotate_annotated_frame_id))
        EHS_FB_OUT_I_API2(INX_mv_objdet_annotate_ARG_annotate_annotated_frame_id) =
            (err == ANNOTATE_OK || err == ANNOTATE_NO_DETECTIONS) ? out_frame_id : -1;

    if (err == ANNOTATE_OK || err == ANNOTATE_NO_DETECTIONS)
        EHS_FB_FINISH(INX_mv_objdet_annotate_ARG_annotate_done);
    else
        EHS_FB_FINISH(INX_mv_objdet_annotate_ARG_annotate_error);

}//ICB FUNCTION annotate MACRO END -- DO NOT ALTER THIS LINE
