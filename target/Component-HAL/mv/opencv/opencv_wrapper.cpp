#include "opencv/opencv_wrapper.h"
#include <opencv2/opencv.hpp>
#ifdef CV_LIBCAMERA_SUPPORT
#include "lccv.hpp"
#include "libcamera_app.hpp"
#endif // CV_LIBCAMERA_SUPPORT
#include <memory>
#include <cstring>
#include <new>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <chrono>
#ifdef __linux__
#include <unistd.h>
#include <cstdio>
#include <cstdlib>   /* getenv */
#endif

extern "C" {
#include "hal_logger.h"
}

/* ============================================================
 *  Global highgui thread
 *
 *  OpenCV's Qt highgui backend ties QApplication (and all QWidget
 *  windows) to the thread that first calls cv::imshow().  eRT's SODL
 *  reload spawns a fresh display worker thread on every reload, so if
 *  those threads call cv::imshow() directly, Qt's "main thread" stays
 *  the first worker (now exited) and subsequent imshow/waitKey calls
 *  from the new thread are silently ignored — the window never updates.
 *
 *  Fix: one persistent background thread that owns all highgui calls
 *  for the process lifetime.  cv_mat_imshow() and
 *  cv_mat_destroy_all_windows() enqueue requests; the thread processes
 *  them and pumps the Qt event loop at ~60 fps continuously.
 *  cv_mat_waitkey() is therefore a no-op.
 * ============================================================ */
struct HighguiReq {
    enum Kind { NONE, IMSHOW, DESTROY_ALL } kind = NONE;
    std::string                  window_name;
    std::shared_ptr<cv::Mat>     frame;  /* shared ownership; no pixel copy */
};

static std::mutex              g_hg_mtx;
static std::condition_variable g_hg_cv;
static HighguiReq              g_hg_req;
static bool                    g_hg_running = false;

static void highgui_thread_run()
{
    for (;;) {
        HighguiReq req;
        {
            std::unique_lock<std::mutex> lk(g_hg_mtx);
            /* Wake on a new request, or every 16 ms to keep Qt alive */
            g_hg_cv.wait_for(lk, std::chrono::milliseconds(16),
                []{ return g_hg_req.kind != HighguiReq::NONE; });
            std::swap(req, g_hg_req); /* consume; resets kind to NONE */
        }
        if (req.kind == HighguiReq::IMSHOW && req.frame && !req.frame->empty())
            cv::imshow(req.window_name, *req.frame);
        else if (req.kind == HighguiReq::DESTROY_ALL)
            cv::destroyAllWindows();
        /* Pump Qt event loop on every iteration, even with no new frame */
        cv::waitKey(1);
    }
}

/* Start the global highgui thread exactly once (idempotent). */
static void cv_ensure_highgui_thread()
{
    std::lock_guard<std::mutex> lk(g_hg_mtx);
    if (!g_hg_running) {
        g_hg_running = true;
        std::thread(highgui_thread_run).detach(); /* process-lifetime */
    }
}

/* Returns true when a usable GUI display is present, false on a headless system.
 *
 * OpenCV's highgui Qt backend calls qFatal()/abort() if QApplication cannot
 * find a display, so all cv::imshow / cv::waitKey / cv::startWindowThread
 * calls must be skipped when there is no display available.
 *
 * Auto-discovery order (most to least preferred):
 *   1. Wayland  — probe $XDG_RUNTIME_DIR/wayland-0 or /run/user/<uid>/wayland-0
 *   2. X11/xcb  — probe /tmp/.X11-unix/X0
 *
 * If a socket is found but the matching environment variable is not yet set,
 * it is set here so that Qt picks it up when it initialises.  The result is
 * cached in a static after the first call to avoid repeated filesystem probes.
 */
#ifdef __linux__
static bool cv_has_display()
{
    static int cached = -1; /* -1 = not yet determined */
    if (cached >= 0) return cached != 0;

    /* Already configured — trust the environment */
    if (std::getenv("DISPLAY") || std::getenv("WAYLAND_DISPLAY")) {
        cached = 1; return true;
    }

    /* --- Probe Wayland first (preferred: lower overhead, no X server needed) --- */
    char sock[256];
    const char* xdg = std::getenv("XDG_RUNTIME_DIR");
    if (xdg) {
        std::snprintf(sock, sizeof(sock), "%s/wayland-0", xdg);
        if (access(sock, F_OK) == 0) {
            setenv("WAYLAND_DISPLAY", "wayland-0", 0);
            cached = 1; return true;
        }
    }
    /* Fallback Wayland path when XDG_RUNTIME_DIR is not set */
    std::snprintf(sock, sizeof(sock), "/run/user/%d/wayland-0", (int)getuid());
    if (access(sock, F_OK) == 0) {
        setenv("WAYLAND_DISPLAY", "wayland-0", 0);
        cached = 1; return true;
    }

    /* --- Probe X11 display :0 --- */
    if (access("/tmp/.X11-unix/X0", F_OK) == 0) {
        setenv("DISPLAY", ":0", 0);
        cached = 1; return true;
    }

    EHSH_LOG_ERROR("[opencv_wrapper] No display found"
                   " (DISPLAY/WAYLAND_DISPLAY not set,"
                   " no Wayland or X11 socket detected) -"
                   " highgui disabled");
    cached = 0; return false;
}
#else
static bool cv_has_display() { return true; }
#endif

/* ---------- tiny RAII VideoCapture wrapper ------------------ */
class CameraWrapper {
public:
    bool open(int id, int w, int h, int fps, int buffer_size, bool monochrome, bool async_mode) {
        if (libcamera_enabled_){
#ifdef CV_LIBCAMERA_SUPPORT
            if (cam_ || running_ || !setup_libcam(id, w, h, fps, monochrome)) return false;
            try {
                if (!cam_->startVideo()) return false;
            } catch (const std::exception&) { return false; }
              catch (...)                   { return false; }
#endif // CV_LIBCAMERA_SUPPORT
        }else{
            if (running_ || cap_.isOpened()) return false; // already running, make sure to close it first
            if (!cap_.open(id)) return false;
        }
        return setup(w, h, fps, buffer_size, monochrome, async_mode);
    }
    bool open(const char* path, int w, int h, int fps, int buffer_size, bool monochrome, bool async_mode) {
        if (!path) return false;
        if (libcamera_enabled_){
#ifdef CV_LIBCAMERA_SUPPORT
            // libcamera only use integer based id
            if (running_) return false;
#endif // CV_LIBCAMERA_SUPPORT
        }else{
            if (running_ || cap_.isOpened()) return false; // already running, make sure to close it first
            if (!cap_.open(path)) return false;
        }
        return setup(w, h, fps, buffer_size, monochrome, async_mode);
    }
    void close()            { stop_async(); release(); }
    bool read(cv::Mat& out) { return (!async_mode_) ? read_normal(out) : read_async(out); }
    void enable_libcamera(bool enable, int timeout=1000) { libcamera_enabled_ = enable; libcamera_timeout_ = timeout; }
private:
    void release()
    {
        if(libcamera_enabled_){
#ifdef CV_LIBCAMERA_SUPPORT
            if(cam_) { cam_->stopVideo(); cam_ = nullptr; }
#endif // CV_LIBCAMERA_SUPPORT
        }else{
            cap_.release();
        }
    }
    bool setup(int w, int h, int fps, int buffer_size, bool monochrome, bool async_mode) {
        bool is_open = false;
        if(libcamera_enabled_){
#ifdef CV_LIBCAMERA_SUPPORT
            is_open = cam_ != nullptr;
#endif // CV_LIBCAMERA_SUPPORT
        }else{
            if (w  > 0) cap_.set(cv::CAP_PROP_FRAME_WIDTH,  w);
            if (h  > 0) cap_.set(cv::CAP_PROP_FRAME_HEIGHT, h);
            if (fps > 0) cap_.set(cv::CAP_PROP_FPS,         fps);
            if (monochrome) cap_.set(cv::CAP_PROP_MONOCHROME, 100);
            if (buffer_size > 0) cap_.set(cv::CAP_PROP_BUFFERSIZE, buffer_size);
            is_open = cap_.isOpened();
        }
        async_mode_ = async_mode;
        if (is_open && async_mode_){
            is_open = start_async();
            if(!is_open) close();
        }
        return is_open;
    }
#ifdef CV_LIBCAMERA_SUPPORT
    bool setup_libcam(int id, int w, int h, int fps, bool is_monochrome){
        cam_ = std::make_unique<lccv::PiCamera>(id);
        if(!cam_) return false;
        if(w > 0) cam_->options->video_width=w;
        if(h > 0) cam_->options->video_height=h;
        if(fps > 0) cam_->options->framerate=fps;
        cam_->options->saturation = is_monochrome ? 0 : 1;
        //cam_->options->verbose=true;
        return true;
    }
#endif
    bool start_async() {
        if (running_) return false;
        running_ = true;
        thread_ = std::thread(&CameraWrapper::grab_loop, this);
        return true;
    }
    void stop_async() {
        if (async_mode_ && running_) {
            running_ = false;
            if (thread_.joinable()) {
                thread_.join();
            }
            std::lock_guard<std::mutex> lk(mutex_);
            frame_.release(); // Clear any leftover frame
        }
        running_=false;
    }
    bool read_async(cv::Mat& out) {
        if (!running_) return false;
        std::lock_guard<std::mutex> lk(mutex_);
        if (frame_.empty()) return false;
        out = frame_.clone();
        return true; 
    }
    bool read_normal(cv::Mat& out) {
        if(libcamera_enabled_){
#ifdef CV_LIBCAMERA_SUPPORT
            return (cam_) ? cam_->getVideoFrame(out, libcamera_timeout_) : false;
#endif // CV_LIBCAMERA_SUPPORT
        }
        return cap_.read(out);
    }

    void grab_loop() {
        cv::Mat frame;
        while (running_) {
            bool ret = false;
            if(libcamera_enabled_){
#ifdef CV_LIBCAMERA_SUPPORT
                ret = (cam_) ? cam_->getVideoFrame(frame, libcamera_timeout_) : false;
#endif // CV_LIBCAMERA_SUPPORT
            } else {
                ret = cap_.read(frame);
            }
            if (!ret) {
                continue;
            }
            {
                std::lock_guard<std::mutex> lk(mutex_);
                frame.copyTo(frame_);
            }
            // @TODO - add callback for async mode
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
#ifdef CV_LIBCAMERA_SUPPORT
    std::unique_ptr<lccv::PiCamera> cam_=nullptr;
#endif // CV_LIBCAMERA_SUPPORT
    cv::VideoCapture cap_;
    bool async_mode_=false;
    bool libcamera_enabled_=false;
    std::atomic<int> libcamera_timeout_=1000;
    std::atomic<bool> running_=false;
    std::mutex mutex_;
    cv::Mat frame_;      // async frame
    std::thread thread_; // async thread
};

/* ---------- helpers to cast opaque C handles ---------------- */
static inline CameraWrapper* to_cpp(cv_camera* c) {
    return c ? static_cast<CameraWrapper*>(c->impl) : nullptr;
}

/* A cv_mat wraps either a shared_ptr<cv::Mat> (CPU, opencl_mode=0)
   or a shared_ptr<cv::UMat> (OpenCL GPU, opencl_mode=1).           */
using MatPtr  = std::shared_ptr<cv::Mat>;
using UMatPtr = std::shared_ptr<cv::UMat>;

static inline MatPtr*  to_mat (cv_mat* m) { return m && !m->opencl_mode ? static_cast<MatPtr*> (m->impl) : nullptr; }
static inline UMatPtr* to_umat(cv_mat* m) { return m &&  m->opencl_mode ? static_cast<UMatPtr*>(m->impl) : nullptr; }

/* Helper: release whatever is stored in impl (Mat or UMat). */
static void cv_mat_release_impl(cv_mat* m)
{
    if (!m || !m->impl) return;
    if (m->opencl_mode) { delete static_cast<UMatPtr*>(m->impl); }
    else                { delete static_cast<MatPtr*> (m->impl); }
    m->impl = nullptr;
}

/* ============================================================ *
 *                      C  API  STARTS HERE                     *
 * ============================================================ */
extern "C" {

int  cv_cam_enable_libcamera_mode (cv_camera* cam, int enable, int cap_timeout_ms)
{
    if (!cam) return -1;
#ifdef CV_LIBCAMERA_SUPPORT
    cam->libcamera_support_enabled = enable;
    if(cap_timeout_ms > 0) cam->libcamera_support_cap_timeout_ms = cap_timeout_ms;
#else
    cam->libcamera_support_enabled = 0;
    return 1;
#endif // CV_LIBCAMERA_SUPPORT
    return 0;
}

int  cv_cam_libcamera_support()
{
#ifdef CV_LIBCAMERA_SUPPORT
    return 1;
#endif // CV_LIBCAMERA_SUPPORT
    return 0;
}

/* Check whether the system has the prerequisites for libcamera to work without
 * crashing.  libcamera's DmaBufAllocator (dma_buf_allocator.cpp:119) will call
 * abort() if it cannot open any DMA-buf heap device.  We check for the known
 * heap paths before ever calling into libcamera so we can return a clean error.
 *
 * Also verifies the V4L2 media device for the requested camera id exists.
 *
 * Returns 1 if libcamera prerequisites are met, 0 otherwise.
 */
int cv_cam_libcamera_ready(int device_id)
{
#ifdef __linux__
    /* DMA-buf heap devices required by libcamera's DmaBufAllocator */
    static const char* const dma_heap_paths[] = {
        "/dev/dma_heap/linux,cma",
        "/dev/dma_heap/reserved",
        "/dev/udmabuf",
        nullptr
    };
    int dma_found = 0;
    for (const char* const* p = dma_heap_paths; *p; ++p) {
        if (access(*p, R_OK) == 0) { dma_found = 1; break; }
    }
    if (!dma_found) return 0;

    /* V4L2 video device for the requested id */
    char dev_path[32];
    snprintf(dev_path, sizeof(dev_path), "/dev/video%d", device_id);
    if (access(dev_path, F_OK) != 0) return 0;
#endif
    return 1;
}

int cv_cam_open(cv_camera* cam, int id, int w, int h, int fps, int buffer_size, bool is_monochrome, int async_mode)
{
    if (!cam) return CV_CAM_OPEN_ERR;
#ifdef __linux__
    /* Pre-validate: device node must exist before we try anything. */
    char dev_path[32];
    snprintf(dev_path, sizeof(dev_path), "/dev/video%d", id);
    if (access(dev_path, F_OK) != 0) return CV_CAM_NOT_FOUND_ERR;
#endif
    /* --- Try standard OpenCV / V4L2 first (works for USB and most V4L2 cameras) --- */
    {
        std::unique_ptr<CameraWrapper> cpp(new (std::nothrow) CameraWrapper);
        if (!cpp) return CV_CAM_ALLOC_ERR;
        try {
            if (cpp->open(id, w, h, fps, buffer_size, is_monochrome, (async_mode > 0))) {
                cam->impl = cpp.release();
                return CV_CAM_OK;
            }
        } catch (...) { /* fall through to libcamera */ }
    }
    /* --- OpenCV/V4L2 failed: fall back to libcamera if compiled in and prerequisites met.
     *     libcamera is required for MIPI/CSI cameras on platforms (e.g. Raspberry Pi) where
     *     the kernel no longer exposes the camera as a usable V4L2 device. --- */
#ifdef CV_LIBCAMERA_SUPPORT
    if (cv_cam_libcamera_ready(id)) {
        std::unique_ptr<CameraWrapper> cpp(new (std::nothrow) CameraWrapper);
        if (!cpp) return CV_CAM_ALLOC_ERR;
        int timeout_ms = (cam->libcamera_support_cap_timeout_ms > 0) ? cam->libcamera_support_cap_timeout_ms : 1000;
        cpp->enable_libcamera(true, timeout_ms);
        try {
            if (cpp->open(id, w, h, fps, buffer_size, is_monochrome, (async_mode > 0))) {
                cam->impl = cpp.release();
                return CV_CAM_OK;
            }
        } catch (const std::exception&) { return CV_CAM_EXCEPTION_ERR; }
          catch (...)                   { return CV_CAM_EXCEPTION_ERR; }
    }
#endif
    return CV_CAM_OPEN_ERR;
}

int  cv_cam_path (cv_camera* cam, const char* path, int w, int h, int fps, int buffer_size, bool is_monochrome, int async_mode)
{
    if (!cam || !path) return CV_CAM_OPEN_ERR;
#ifdef __linux__
    /* Pre-validate: check the device/file path exists before the library tries
     * to open it. Catches missing /dev/videoX paths and bad file paths early. */
    if (access(path, F_OK) != 0) return CV_CAM_NOT_FOUND_ERR;
#endif
    /* Path-based IDs always use the standard OpenCV/V4L2 backend.
     * libcamera does not support path-based device selection — use an integer
     * device index if libcamera fallback is needed. */
    std::unique_ptr<CameraWrapper> cpp(new (std::nothrow) CameraWrapper);
    if (!cpp) return CV_CAM_ALLOC_ERR;
    try {
        if (!cpp->open(path, w, h, fps, buffer_size, is_monochrome, (async_mode > 0))) return CV_CAM_OPEN_ERR;
    } catch (const std::exception&) { return CV_CAM_EXCEPTION_ERR; }
      catch (...)                   { return CV_CAM_EXCEPTION_ERR; }
    cam->impl = cpp.release();
    return CV_CAM_OK;
}

void cv_cam_close(cv_camera* cam)
{
    if (auto* cpp = to_cpp(cam)) {
        cpp->close();
        delete cpp;
        cam->impl = nullptr;
    }
}

/* ---------- zero-copy cv::Mat path --------------------------- */
int cv_cam_read(cv_camera* cam,
                cv_mat* m)
{
    if (!cam || !m) return CV_CAM_READ_ERR;
    CameraWrapper* cpp = to_cpp(cam);
    if (!cpp) return CV_CAM_READ_ERR;

    cv::Mat frame;
    if (!cpp->read(frame)) return CV_CAM_READ_ERR;

    /* create a shared_ptr on heap so it survives after function returns */
    MatPtr* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(frame)));
    if (!holder) return CV_CAM_ALLOC_ERR;

    m->impl = static_cast<void*>(holder);
    m->width  = (*holder)->cols;
    m->height  = (*holder)->rows;
    m->channels = 3;
    return CV_CAM_OK;
}

int cv_mat_read(const char* filename, cv_mat* dst)
{
    if (!filename || !dst) return CV_CAM_READ_ERR;

    cv::Mat img = cv::imread(filename, cv::IMREAD_UNCHANGED);
    if (img.empty()) return CV_CAM_READ_ERR;

    auto* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(img)));
    if (!holder) return CV_CAM_ALLOC_ERR;

    dst->impl = holder;
    dst->width = (*holder)->cols;
    dst->height = (*holder)->rows;
    dst->channels = (*holder)->channels();

    return CV_CAM_OK;
}

int cv_mat_write(const char* filename, const cv_mat* src)
{
    if (!filename || !src || !src->impl) return CV_CAM_WRITE_ERR;

    auto sp_src = *(reinterpret_cast<const std::shared_ptr<cv::Mat>*>(src->impl));
    if (!sp_src || sp_src->empty()) return CV_CAM_WRITE_ERR;

    bool success = cv::imwrite(filename, *sp_src);
    return success ? CV_CAM_OK : CV_CAM_WRITE_ERR;
}

void cv_mat_release(cv_mat* m)
{
    cv_mat_release_impl(m);
}

int cv_cam_read_opencl(cv_camera* cam, cv_mat* m)
{
    /* Capture a CPU frame then upload to UMat for the OpenCL pipeline. */
    if (!cam || !m) return CV_CAM_READ_ERR;
    CameraWrapper* cpp = to_cpp(cam);
    if (!cpp) return CV_CAM_READ_ERR;

    cv::Mat frame;
    if (!cpp->read(frame)) return CV_CAM_READ_ERR;

    cv_mat_release_impl(m); /* free any previous buffer */

    cv::UMat u_frame;
    frame.copyTo(u_frame); /* upload CPU→GPU */

    auto* holder = new (std::nothrow) UMatPtr(std::make_shared<cv::UMat>(std::move(u_frame)));
    if (!holder) return CV_CAM_ALLOC_ERR;

    m->impl        = holder;
    m->opencl_mode = 1;
    m->width       = frame.cols;
    m->height      = frame.rows;
    m->channels    = frame.channels();
    return CV_CAM_OK;
}

int cv_mat_ensure_cpu(cv_mat* m)
{
    /* No-op if already CPU. */
    if (!m || !m->impl || !m->opencl_mode) return CV_CAM_OK;

    auto sp_umat = *(static_cast<UMatPtr*>(m->impl));
    cv::Mat cpu_mat = sp_umat->getMat(cv::ACCESS_READ).clone();

    cv_mat_release_impl(m); /* free UMat */

    auto* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(cpu_mat)));
    if (!holder) return CV_CAM_ALLOC_ERR;

    m->impl        = holder;
    m->opencl_mode = 0;
    return CV_CAM_OK;
}

void* cv_mat_data(const cv_mat* mat)
{
    if (!mat || !mat->impl) return nullptr;
    /* If frame is on the GPU, download to CPU first (mutate is intentional —
     * cv_mat_data is by definition a CPU-access operation). */
    if (mat->opencl_mode) {
        cv_mat_ensure_cpu(const_cast<cv_mat*>(mat));
    }
    auto sp = *(static_cast<const MatPtr*>(mat->impl));
    if (!sp || sp->empty()) return nullptr;
    return static_cast<void*>(sp->data);
}

unsigned int cv_mat_size_bytes(const cv_mat* mat)
{
    if (!mat || !mat->impl) return 0;
    if (mat->opencl_mode) {
        auto sp = *(static_cast<const UMatPtr*>(mat->impl));
        if (!sp || sp->empty()) return 0;
        return static_cast<unsigned int>(sp->total() * sp->elemSize());
    }
    auto sp = *(static_cast<const MatPtr*>(mat->impl));
    if (!sp || sp->empty()) return 0;
    return static_cast<unsigned int>(sp->total() * sp->elemSize());
}

int cv_mat_convert_to(const cv_mat* src,
                      cv_mat* dst,
                      int rtype,
                      double alpha,
                      double beta)
{
    if (!src || !src->impl || !dst) return CV_CAM_READ_ERR;
    cv_mat_release_impl(dst);

    if (src->opencl_mode) {
        auto sp_src = *(static_cast<const UMatPtr*>(src->impl));
        if (!sp_src || sp_src->empty()) return CV_CAM_READ_ERR;
        cv::UMat converted;
        sp_src->convertTo(converted, rtype, alpha, beta);
        auto* holder = new (std::nothrow) UMatPtr(std::make_shared<cv::UMat>(std::move(converted)));
        if (!holder) return CV_CAM_ALLOC_ERR;
        dst->impl        = holder;
        dst->opencl_mode = 1;
        dst->width       = sp_src->cols;
        dst->height      = sp_src->rows;
        dst->channels    = sp_src->channels();
    } else {
        auto sp_src = *(static_cast<const MatPtr*>(src->impl));
        if (!sp_src || sp_src->empty()) return CV_CAM_READ_ERR;
        cv::Mat converted;
        sp_src->convertTo(converted, rtype, alpha, beta);
        auto* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(converted)));
        if (!holder) return CV_CAM_ALLOC_ERR;
        dst->impl        = holder;
        dst->opencl_mode = 0;
        dst->width       = converted.cols;
        dst->height      = converted.rows;
        dst->channels    = converted.channels();
    }
    return CV_CAM_OK;
}

int cv_mat_resize(const cv_mat* src, cv_mat* dst, int target_width, int target_height, int interp)
{
    if (!src || !src->impl || !dst) return CV_CAM_READ_ERR;
    const cv::Size target_size(target_width, target_height);

    if (src->opencl_mode) {
        /* UMat path — stays on GPU */
        auto sp_src = *(static_cast<const UMatPtr*>(src->impl));
        if (!sp_src || sp_src->empty()) return CV_CAM_READ_ERR;
        dst->opencl_mode = 1;
        if (dst->impl && dst->opencl_mode) {
            cv::resize(*sp_src, *(*static_cast<UMatPtr*>(dst->impl)), target_size, 0, 0, interp);
        } else {
            cv_mat_release_impl(dst);
            cv::UMat resized;
            cv::resize(*sp_src, resized, target_size, 0, 0, interp);
            auto* holder = new (std::nothrow) UMatPtr(std::make_shared<cv::UMat>(std::move(resized)));
            if (!holder) return CV_CAM_ALLOC_ERR;
            dst->impl = holder;
        }
        dst->channels = sp_src->channels();
    } else {
        /* Mat path (CPU) — reuse buffer where possible */
        auto sp_src = *(static_cast<const MatPtr*>(src->impl));
        if (!sp_src || sp_src->empty()) return CV_CAM_READ_ERR;
        dst->opencl_mode = 0;
        if (dst->impl && !dst->opencl_mode) {
            cv::resize(*sp_src, *(*static_cast<MatPtr*>(dst->impl)), target_size, 0, 0, interp);
        } else {
            cv_mat_release_impl(dst);
            cv::Mat resized;
            cv::resize(*sp_src, resized, target_size, 0, 0, interp);
            auto* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(resized)));
            if (!holder) return CV_CAM_ALLOC_ERR;
            dst->impl = holder;
        }
        dst->channels = sp_src->channels();
    }

    dst->width  = target_width;
    dst->height = target_height;
    return CV_CAM_OK;
}

int cv_mat_crop(const cv_mat* src,
                cv_mat*       dst,
                int           x,
                int           y,
                int           crop_w,
                int           crop_h)
{
    if (!src || !src->impl || !dst) return CV_CAM_READ_ERR;
    cv_mat_release_impl(dst);

    /* Compute clamped ROI from source dimensions */
    int src_w = src->width;
    int src_h = src->height;
    int x0 = std::max(0, x),           y0 = std::max(0, y);
    int x1 = std::min(x + crop_w, src_w), y1 = std::min(y + crop_h, src_h);
    int w = x1 - x0, h = y1 - y0;
    if (w <= 0 || h <= 0) return CV_CAM_READ_ERR;
    cv::Rect roi(x0, y0, w, h);

    if (src->opencl_mode) {
        auto sp_src = *(static_cast<const UMatPtr*>(src->impl));
        if (!sp_src || sp_src->empty()) return CV_CAM_READ_ERR;
        cv::UMat cropped = (*sp_src)(roi).clone();
        auto* holder = new (std::nothrow) UMatPtr(std::make_shared<cv::UMat>(std::move(cropped)));
        if (!holder) return CV_CAM_ALLOC_ERR;
        dst->impl        = holder;
        dst->opencl_mode = 1;
        dst->channels    = sp_src->channels();
    } else {
        auto sp_src = *(static_cast<const MatPtr*>(src->impl));
        if (!sp_src || sp_src->empty()) return CV_CAM_READ_ERR;
        cv::Mat cropped = (*sp_src)(roi).clone();
        auto* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(cropped)));
        if (!holder) return CV_CAM_ALLOC_ERR;
        dst->impl        = holder;
        dst->opencl_mode = 0;
        dst->channels    = sp_src->channels();
    }
    dst->width  = w;
    dst->height = h;
    return CV_CAM_OK;
}


int cv_mat_to_grayscale(const cv_mat* src, cv_mat* dst)
{
    if (!src || !src->impl || !dst) return CV_CAM_READ_ERR;
    cv_mat_release_impl(dst);

    if (src->opencl_mode) {
        auto sp_src = *(static_cast<const UMatPtr*>(src->impl));
        if (!sp_src || sp_src->channels() != 3) return CV_CAM_READ_ERR;
        cv::UMat gray;
        cv::cvtColor(*sp_src, gray, cv::COLOR_BGR2GRAY);
        auto* holder = new (std::nothrow) UMatPtr(std::make_shared<cv::UMat>(std::move(gray)));
        if (!holder) return CV_CAM_ALLOC_ERR;
        dst->impl        = holder;
        dst->opencl_mode = 1;
        dst->width       = sp_src->cols;
        dst->height      = sp_src->rows;
    } else {
        auto sp_src = *(static_cast<const MatPtr*>(src->impl));
        if (!sp_src || sp_src->channels() != 3) return CV_CAM_READ_ERR;
        cv::Mat gray;
        cv::cvtColor(*sp_src, gray, cv::COLOR_BGR2GRAY);
        auto* holder = new (std::nothrow) MatPtr(std::make_shared<cv::Mat>(std::move(gray)));
        if (!holder) return CV_CAM_ALLOC_ERR;
        dst->impl        = holder;
        dst->opencl_mode = 0;
        dst->width       = gray.cols;
        dst->height      = gray.rows;
    }
    dst->channels = 1;
    return CV_CAM_OK;
}

int cv_mat_draw_rectangle(cv_mat* mat,
                          int x, int y,
                          int width, int height,
                          int r, int g, int b,
                          int thickness)
{
    if (!mat || !mat->impl) {
        return CV_CAM_ALLOC_ERR;
    }

    auto mat_ptr = static_cast<std::shared_ptr<cv::Mat>*>(mat->impl);
    if (!mat_ptr || !(*mat_ptr) || (*mat_ptr)->empty()) {
        return CV_CAM_ALLOC_ERR;
    }

    try {
        cv::rectangle(*(*mat_ptr),
                      cv::Rect(x, y, width, height),
                      cv::Scalar(b, g, r),  // OpenCV uses BGR order
                      thickness);
        return CV_CAM_OK;
    } catch (...) {
        return CV_CAM_ALLOC_ERR;
    }
}

int cv_mat_draw_text(cv_mat* mat,
                     const char* text,
                     int x, int y,
                     double font_scale,
                     int r, int g, int b,
                     int thickness)
{
    if (!mat || !mat->impl || !text) return CV_CAM_ALLOC_ERR;

    auto mat_ptr = static_cast<std::shared_ptr<cv::Mat>*>(mat->impl);
    if (!mat_ptr || !(*mat_ptr) || (*mat_ptr)->empty()) return CV_CAM_ALLOC_ERR;

    try {
        cv::putText(*(*mat_ptr),
                    text,
                    cv::Point(x, y),
                    cv::FONT_HERSHEY_SIMPLEX,
                    font_scale,
                    cv::Scalar(b, g, r),
                    thickness);
        return CV_CAM_OK;
    } catch (...) {
        return CV_CAM_ALLOC_ERR;
    }
}


int cv_mat_show(const char* window_name, const cv_mat* mat, int wait_ms) {
    (void)wait_ms; /* global highgui thread handles event pumping */
    return cv_mat_imshow(window_name, mat);
}

int cv_mat_imshow(const char* window_name, const cv_mat* mat) {
    if (!cv_has_display()) return CV_CAM_OK;
    if (!window_name || !mat || !mat->impl) return CV_CAM_ALLOC_ERR;
    auto* mat_ptr = static_cast<std::shared_ptr<cv::Mat>*>(mat->impl);
    if (!mat_ptr || !(*mat_ptr) || (*mat_ptr)->empty()) return CV_CAM_ALLOC_ERR;

    cv_ensure_highgui_thread();

    {
        std::lock_guard<std::mutex> lk(g_hg_mtx);
        g_hg_req.kind        = HighguiReq::IMSHOW;
        g_hg_req.window_name = window_name;
        g_hg_req.frame       = *mat_ptr; /* shared ownership; latest frame wins */
    }
    g_hg_cv.notify_one();
    return CV_CAM_OK;
}

void cv_window_start_thread(void) {
    if (!cv_has_display()) return;
    cv_ensure_highgui_thread();
}

void cv_mat_waitkey(int wait_ms) {
    /* No-op: the global highgui thread pumps Qt events continuously. */
    (void)wait_ms;
}

void cv_mat_destroy_all_windows() {
    if (!cv_has_display()) return;
    {
        std::lock_guard<std::mutex> lk(g_hg_mtx);
        g_hg_req.kind = HighguiReq::DESTROY_ALL;
        g_hg_req.frame.reset();
        g_hg_req.window_name.clear();
    }
    g_hg_cv.notify_one();
}

} /* extern "C" */
