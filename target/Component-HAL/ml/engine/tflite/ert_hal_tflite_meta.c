#include "ert_hal_tflite_meta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* =========================================================================
 * Minimal TFLite FlatBuffer parser
 *
 * TFLite files are FlatBuffers.  We parse just enough of the schema to
 * extract Model.description and Model.metadata without pulling in the full
 * flatcc / flatbuffers C++ runtime.
 *
 * TFLite schema field indices used here:
 *   Model:    version=0, operator_codes=1, subgraphs=2, description=3,
 *             buffers=4, metadata_buffer=5(deprecated), metadata=6
 *   Metadata: name=0, buffer=1
 *   Buffer:   data=0
 * ========================================================================= */

typedef struct { const uint8_t* d; uint32_t sz; } Fb;

static uint32_t fb_u32(const Fb* b, uint32_t o)
{
    if (o + 4 > b->sz) return 0;
    uint32_t v; memcpy(&v, b->d + o, 4); return v;
}
static int32_t fb_i32(const Fb* b, uint32_t o)
{
    if (o + 4 > b->sz) return 0;
    int32_t v;  memcpy(&v, b->d + o, 4); return v;
}
static uint16_t fb_u16(const Fb* b, uint32_t o)
{
    if (o + 2 > b->sz) return 0;
    uint16_t v; memcpy(&v, b->d + o, 2); return v;
}

/* Absolute byte offset of field fi in the table at obj; 0 = not present. */
static uint32_t fb_field(const Fb* b, uint32_t obj, int fi)
{
    if (!obj || obj >= b->sz) return 0;
    uint32_t vt   = (uint32_t)((int32_t)obj - fb_i32(b, obj));
    if (vt >= b->sz) return 0;
    uint16_t vtsz = fb_u16(b, vt);
    uint16_t slot = (uint16_t)(4 + fi * 2);
    if (slot + 2 > vtsz) return 0;
    uint16_t off  = fb_u16(b, vt + slot);
    return off ? obj + off : 0;
}

/* Copy the FlatBuffer string at field fi of obj into dst (null-terminated). */
static void fb_str(const Fb* b, uint32_t obj, int fi, char* dst, uint32_t dsz)
{
    if (!dst || dsz == 0) return;
    dst[0] = '\0';
    uint32_t fp = fb_field(b, obj, fi);
    if (!fp) return;
    uint32_t soff = fp + fb_u32(b, fp);
    if (soff + 4 > b->sz) return;
    uint32_t slen = fb_u32(b, soff); soff += 4;
    if (soff + slen > b->sz) slen = b->sz - soff;
    uint32_t n = (slen < dsz - 1) ? slen : dsz - 1;
    memcpy(dst, b->d + soff, n);
    dst[n] = '\0';
}

/* Resolve a vector at field fi; returns start of element array, count in *cnt. */
static uint32_t fb_vec(const Fb* b, uint32_t obj, int fi, uint32_t* cnt)
{
    if (cnt) *cnt = 0;
    uint32_t fp = fb_field(b, obj, fi);
    if (!fp) return 0;
    uint32_t vp = fp + fb_u32(b, fp);
    if (vp + 4 > b->sz) return 0;
    if (cnt) *cnt = fb_u32(b, vp);
    return vp + 4; /* points to first element */
}

/* Table reference at vector element i (vector of tables). */
static uint32_t fb_vtab(const Fb* b, uint32_t vstart, uint32_t i)
{
    uint32_t ref = vstart + i * 4;
    if (ref + 4 > b->sz) return 0;
    return ref + fb_u32(b, ref);
}

/* =========================================================================
 * TFLite C-API helpers
 * ========================================================================= */

static const char* tflite_dtype_name(TfLiteType t)
{
    switch (t) {
        case kTfLiteFloat32: return "float32";
        case kTfLiteFloat16: return "float16";
        case kTfLiteInt8:    return "int8";
        case kTfLiteUInt8:   return "uint8";
        case kTfLiteInt16:   return "int16";
        case kTfLiteInt32:   return "int32";
        case kTfLiteInt64:   return "int64";
        case kTfLiteBool:    return "bool";
        case kTfLiteFloat64: return "float64";
        default:             return "unknown";
    }
}

/* Infer YOLO model family from output tensor shape. */
static const char* infer_model_family(const TfLiteTensor* out_t)
{
    if (!out_t || TfLiteTensorNumDims(out_t) != 3) return "unknown";
    int d1 = TfLiteTensorDim(out_t, 1); /* rows */
    int d2 = TfLiteTensorDim(out_t, 2); /* cols */

    /* YOLOv5-family: [batch, anchors, classes+5]  anchors >> classes+5 */
    if (d1 > d2 && d2 >= 5) {
        int cls = d2 - 5;
        if (cls == 80) return "yolov5-coco80";
        if (cls == 20) return "yolov5-voc20";
        return "yolov5-family";
    }
    /* YOLOv8-family: [batch, classes+4, anchors]  no objectness, transposed */
    if (d2 > d1 && d1 >= 4) {
        int cls = d1 - 4;
        if (cls == 80) return "yolov8-coco80";
        if (cls == 20) return "yolov8-voc20";
        return "yolov8-family";
    }
    return "unknown";
}

/* Write one tensor's JSON object into buf at *pos. */
static void write_tensor_json(char* buf, uint32_t bufsz, uint32_t* pos,
                              const TfLiteTensor* t)
{
#define TJAPP(fmt, ...) do { \
    int _n = snprintf(buf + (*pos), bufsz - (*pos), fmt, ##__VA_ARGS__); \
    if (_n > 0) (*pos) += ((uint32_t)_n < bufsz - (*pos)) \
                          ? (uint32_t)_n : bufsz - (*pos) - 1u; \
} while (0)

    int nd = TfLiteTensorNumDims(t);
    TfLiteQuantizationParams qp = TfLiteTensorQuantizationParams(t);
    TJAPP("{\"name\":\"%s\",\"dtype\":\"%s\",\"shape\":[",
          TfLiteTensorName(t) ? TfLiteTensorName(t) : "",
          tflite_dtype_name(TfLiteTensorType(t)));
    for (int d = 0; d < nd; d++)
        TJAPP("%s%d", d ? "," : "", (int)TfLiteTensorDim(t, d));
    TJAPP("],\"bytes\":%zu,\"quant_scale\":%.7g,\"quant_zp\":%d}",
          TfLiteTensorByteSize(t), (double)qp.scale, (int)qp.zero_point);
#undef TJAPP
}

/* =========================================================================
 * Public function
 * ========================================================================= */

EhsML_Err EhsML_TFLite_GetModelInfoJson(TfLiteModelCtx*  tfl,
                                         const ehs_char*  model_path,
                                         ehs_char*        json_buf,
                                         ehs_uint32       json_size)
{
    if (!tfl || !tfl->interp || !json_buf || json_size < 2)
        return EHS_ML_FAILED;

    json_buf[0] = '\0';
    uint32_t pos = 0;

#define JAPP(fmt, ...) do { \
    int _n = snprintf(json_buf + pos, json_size - pos, fmt, ##__VA_ARGS__); \
    if (_n > 0) pos += ((uint32_t)_n < json_size - pos) \
                       ? (uint32_t)_n : json_size - pos - 1u; \
} while (0)

    /* --- Read model file for FlatBuffer metadata parsing --- */
    Fb fb = {NULL, 0};
    char fb_description[256] = {0};
    uint32_t fb_buffers_start = 0, fb_buffers_cnt = 0;
    uint32_t fb_meta_start    = 0, fb_meta_cnt    = 0;

    if (model_path) {
        FILE* f = fopen(model_path, "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            long fsz = ftell(f);
            rewind(f);
            if (fsz > 0) {
                uint8_t* buf = (uint8_t*)malloc((size_t)fsz);
                if (buf) {
                    fb.sz = (uint32_t)fread(buf, 1, (size_t)fsz, f);
                    fb.d  = buf;
                }
            }
            fclose(f);
        }
    }

    if (fb.d && fb.sz > 8) {
        /* Bytes 0-3: root table offset from start of buffer.
         * Bytes 4-7: "TFL3" file identifier (not used here). */
        uint32_t root = fb_u32(&fb, 0);
        fb_str(&fb, root, 3, fb_description, sizeof(fb_description));
        fb_buffers_start = fb_vec(&fb, root, 4, &fb_buffers_cnt);
        fb_meta_start    = fb_vec(&fb, root, 6, &fb_meta_cnt);
    }

    /* --- Build JSON --- */
    JAPP("{");
    JAPP("\"runtime\":\"%s\"", TfLiteVersion());

    if (fb_description[0])
        JAPP(",\"description\":\"%s\"", fb_description);

    int in_cnt  = TfLiteInterpreterGetInputTensorCount(tfl->interp);
    int out_cnt = TfLiteInterpreterGetOutputTensorCount(tfl->interp);
    JAPP(",\"input_count\":%d,\"output_count\":%d", in_cnt, out_cnt);

    /* Input tensors */
    JAPP(",\"inputs\":[");
    for (int i = 0; i < in_cnt; i++) {
        if (i) JAPP(",");
        const TfLiteTensor* t = TfLiteInterpreterGetInputTensor(tfl->interp, i);
        if (t) write_tensor_json(json_buf, json_size, &pos, t);
    }
    JAPP("]");

    /* Output tensors */
    JAPP(",\"outputs\":[");
    for (int i = 0; i < out_cnt; i++) {
        if (i) JAPP(",");
        const TfLiteTensor* t = TfLiteInterpreterGetOutputTensor(tfl->interp, i);
        if (t) write_tensor_json(json_buf, json_size, &pos, t);
    }
    JAPP("]");

    JAPP(",\"inferred_type\":\"%s\"", infer_model_family(tfl->out_tensor));
    JAPP(",\"xnnpack\":%s", tfl->accel_delegate ? "true" : "false");

    /* Metadata entries */
    if (fb_meta_cnt > 0) {
        JAPP(",\"metadata\":[");
        for (uint32_t mi = 0; mi < fb_meta_cnt; mi++) {
            if (mi) JAPP(",");
            uint32_t meta_obj = fb_vtab(&fb, fb_meta_start, mi);
            char meta_name[128] = {0};
            fb_str(&fb, meta_obj, 0, meta_name, sizeof(meta_name));

            /* Buffer index stored as uint32 scalar at field 1 */
            uint32_t buf_idx = 0;
            uint32_t bfp = fb_field(&fb, meta_obj, 1);
            if (bfp) buf_idx = fb_u32(&fb, bfp);

            uint32_t data_start = 0, data_sz = 0;
            if (fb_buffers_start && buf_idx < fb_buffers_cnt) {
                uint32_t buf_obj = fb_vtab(&fb, fb_buffers_start, buf_idx);
                data_start = fb_vec(&fb, buf_obj, 0, &data_sz);
            }

            /* Output as string value if content is short printable ASCII,
             * otherwise just report the byte size. */
            int printable = (data_sz > 0 && data_sz < 256 && data_start != 0);
            if (printable) {
                for (uint32_t ci = 0; ci < data_sz && printable; ci++) {
                    uint8_t c = fb.d[data_start + ci];
                    if (c < 0x20 || c > 0x7E) printable = 0;
                }
            }
            if (printable) {
                JAPP("{\"name\":\"%s\",\"value\":\"", meta_name);
                uint32_t avail = json_size - pos - 3u; /* room for "} */
                uint32_t n = (data_sz < avail) ? data_sz : avail;
                memcpy(json_buf + pos, fb.d + data_start, n);
                pos += n;
                JAPP("\"}");
            } else {
                JAPP("{\"name\":\"%s\",\"size\":%u}", meta_name, data_sz);
            }
        }
        JAPP("]");
    }

    JAPP("}");

    if (fb.d) free((void*)fb.d);
    return EHS_ML_OK;

#undef JAPP
}
