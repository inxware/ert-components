# linux_arm64_lvgl_jetson_nano — Platform Setup Guide

Target: **NVIDIA Jetson Orin Nano** running JetPack 6.x (Ubuntu 22.04 Jammy).

This platform enables:
- GStreamer-based camera capture via `mv/jetson` (CSI and USB cameras)
- TensorRT GPU inference via `ml/tensorrt`
- LVGL display output via SDL2 or DRM framebuffer
- Standard Linux UART, networking, and device management

---

## 1. Jetson Host Setup

### 1.1 Flash JetPack 6.x

Flash the Jetson Orin Nano with JetPack 6.x using NVIDIA SDK Manager on a Linux x86_64 host:

1. Download and install [NVIDIA SDK Manager](https://developer.nvidia.com/sdk-manager).
2. Connect the Jetson to the host via USB-C (in recovery mode: hold REC button, power on).
3. Select **Jetson Orin Nano** and **JetPack 6.x** (latest 6.x release).
4. Install **Jetson Linux** and **Jetson SDK Components** (includes CUDA, TensorRT, GStreamer).

After flashing, the Jetson boots Ubuntu 22.04 with:
- CUDA 12.x at `/usr/local/cuda`
- TensorRT 10.x at `/usr/lib/aarch64-linux-gnu/` and `/usr/include/aarch64-linux-gnu/`
- GStreamer with NVIDIA plugins (`nvarguscamerasrc`, `nvvidconv`, `nvv4l2decoder`)

### 1.2 Verify JetPack installation

SSH to the Jetson and confirm:

```bash
# Check CUDA
nvcc --version

# Check TensorRT
dpkg -l | grep libnvinfer

# Check GStreamer NVIDIA plugins
gst-inspect-1.0 nvarguscamerasrc
gst-inspect-1.0 nvvidconv
```

### 1.3 Camera wiring

**CSI camera** (e.g. Raspberry Pi Camera Module v2 or IMX219):
- Connect to the MIPI CSI-2 connector on the Jetson Orin Nano carrier board.
- Verify: `ls /dev/video*` — CSI cameras appear as `/dev/video0`.
- In the eRT camera function block, set camera ID to `csi:0` (or `csi:1` for a second sensor).

**USB camera** (UVC-compatible):
- Connect via USB. Verify: `v4l2-ctl --list-devices`.
- Set camera ID to the device number (e.g. `0` for `/dev/video0`) or the full path `/dev/video0`.

---

## 2. Building eRT for Jetson

### 2.1 Build environment

The build uses a Docker container for cross-compilation from an x86_64 Linux host.

```bash
# From the ert-components root directory
make targetenv TARGET=linux_arm64_lvgl_jetson_nano
```

This builds the Docker image defined in `Dockerfile` and tags it with the name in `Dockerimagename`.

> **TensorRT headers**: The Dockerfile installs CUDA cross-compilation headers automatically.
> TensorRT dev headers may need to be installed manually — see section 2.2.

### 2.2 TensorRT headers (if Dockerfile installation fails)

If `libnvinfer-dev` is not available from the NVIDIA apt repo for your Docker host:

1. Download the TensorRT 10.x `.tar.gz` for Linux aarch64 from
   https://developer.nvidia.com/tensorrt (free login required).

2. Extract and copy headers and stubs into the Docker image:

```bash
tar -xzf TensorRT-10.x.y.z.Linux.aarch64-gnu.cuda-12.x.tar.gz
# From your host, before or after docker build:
docker cp TensorRT-10.x.y.z/include/. <container>:/usr/include/aarch64-linux-gnu/
docker cp TensorRT-10.x.y.z/lib/.     <container>:/usr/lib/aarch64-linux-gnu/
```

   Or mount the TensorRT directory into the container at build time using a Docker volume.

### 2.3 Compile

```bash
# Build the eRT binary for Jetson
make all TARGET=linux_arm64_lvgl_jetson_nano

# The binary is output to:
#   build/linux_arm64_lvgl_jetson_nano/<app>/ehs_app
```

### 2.4 Deploy to Jetson

Copy the binary and any required assets to the Jetson:

```bash
scp build/linux_arm64_lvgl_jetson_nano/<app>/ehs_app user@jetson-ip:~/
scp -r <app>/appland user@jetson-ip:~/
```

---

## 3. TensorRT Engine Files

TensorRT engines are **platform-specific** — an engine built on one GPU cannot run on a different GPU or a different version of TensorRT. Engines must be built on the target Jetson (or an identical JetPack configuration).

### 3.1 Build an engine from ONNX

On the Jetson (after JetPack install):

```bash
# FP16 precision — recommended for Orin Nano (reduces latency, minimal accuracy loss)
trtexec \
    --onnx=yolov8n.onnx \
    --saveEngine=yolov8n_fp16.engine \
    --fp16 \
    --workspace=1024

# INT8 quantised — fastest, requires calibration dataset for accuracy
trtexec \
    --onnx=yolov8n.onnx \
    --saveEngine=yolov8n_int8.engine \
    --int8 \
    --calib=calibration.cache \
    --workspace=2048
```

`trtexec` is installed with JetPack at `/usr/src/tensorrt/bin/trtexec`.

### 3.2 Exporting ONNX from a YOLOv8 model

Using Ultralytics on a desktop x86 machine:

```bash
pip install ultralytics
yolo export model=yolov8n.pt format=onnx imgsz=640 opset=17
```

The exported `yolov8n.onnx` is portable across platforms. Build the `.engine` on the Jetson.

### 3.3 Engine file location

Place the `.engine` file anywhere accessible on the Jetson filesystem.  Configure the path in your eRT application's ML function block parameter (the `model_path` input).

Example: `/home/user/models/yolov8n_fp16.engine`

### 3.4 Engine caching (re-use across restarts)

The `EhsML_FW_TensorRT_Create()` call loads a pre-built `.engine` file directly.  Engine loading typically takes 1–5 seconds. For production deployments, keep the engine on fast storage (eMMC, not SD card) to minimise startup time.

---

## 4. Camera Pipeline Details

The `mv/jetson` HAL builds a GStreamer pipeline at `EhsCameraStart()` time.

### CSI camera (nvarguscamerasrc)

```
nvarguscamerasrc sensor-id=0
  ! video/x-raw(memory:NVMM), width=1280, height=720, framerate=30/1
  ! nvvidconv
  ! video/x-raw, format=BGRx
  ! videoconvert
  ! video/x-raw, format=BGR
  ! appsink name=sink sync=false max-buffers=2 drop=true
```

The `nvarguscamerasrc` element requires the Argus daemon. Verify it is running:

```bash
systemctl status nvargus-daemon
# If not running:
sudo systemctl start nvargus-daemon
```

### USB camera (v4l2src)

```
v4l2src device=/dev/video0
  ! video/x-raw, width=1280, height=720, framerate=30/1
  ! videoconvert
  ! video/x-raw, format=BGR
  ! appsink name=sink sync=false max-buffers=2 drop=true
```

USB cameras do not use NVMM and will not have a CUDA device pointer in the frame pool.  TensorRT still works correctly via the CPU→GPU copy path in `EhsML_FW_TensorRT_SetInputData`.

### Camera ID in eRT function block

| Value | Camera |
|-------|--------|
| `csi:0` | CSI-2 sensor 0 (nvarguscamerasrc) |
| `csi:1` | CSI-2 sensor 1 (second camera port) |
| `0` | `/dev/video0` (v4l2src) |
| `/dev/video1` | `/dev/video1` (v4l2src, explicit path) |

---

## 5. LVGL Display Output

On Jetson, LVGL renders via SDL2 or DRM (direct framebuffer).

### SDL2 (recommended for desktop use with HDMI monitor)

SDL2 is the default LVGL backend on this platform. Ensure the Jetson is booted to a graphical session or set:

```bash
export DISPLAY=:0
./ehs_app
```

### DRM / KMS (headless or embedded panel)

For DSI panels or headless display via `/dev/dri/card0`, the LVGL DRM backend
can be enabled by setting `EHS_LVGL_DRM=yes` in `config.mk` (if supported by your
LVGL HAL configuration).

---

## 6. Running the application

```bash
ssh user@jetson-ip
cd ~

# Ensure the Argus daemon is running for CSI cameras
sudo systemctl start nvargus-daemon

# Run the eRT application
./ehs_app
```

Application logs are written to stdout and to `/var/log/ehs/` if the logging HAL is configured.

---

## 7. Performance Notes

| Feature | Expected latency (Orin Nano 8GB) |
|---------|----------------------------------|
| CSI camera grab (720p@30fps) | < 5 ms |
| TensorRT YOLOv8n FP16 inference (640×640) | 8–15 ms |
| TensorRT YOLOv8n INT8 inference (640×640) | 5–10 ms |
| LVGL render (1080p) | 16 ms target (60 fps) |

- Set `max-buffers=2 drop=true` on appsink (default) to prevent the camera pipeline from stalling behind a slow inference loop.
- For production use, resize the camera frame to the model input size (typically 640×640) using `EhsCameraFrameResize` **before** calling `EhsML_SetInputData` to avoid sending a large frame to TensorRT.

---

## 8. Known Limitations

- `EhsCameraFrameReadFromFile` and `EhsCameraFrameWriteToFile` are not implemented on this target (returns `EHS_FALSE`). Use the OpenCV target (`linux_arm64_lvgl_raspberrypi_debian13`) if file I/O on frames is required.
- Image resize/crop in `mv/jetson` uses a CPU nearest-neighbour implementation. For high-throughput pipelines, replace with an NvBufSurface transform call (VIC hardware).
- The CUDA frame pool device pointer (`cuda_dev_ptr`) is not yet populated for USB cameras (v4l2src). CSI cameras with full NVMM support can expose the pointer in a future revision.
- Only single-input, single-output TensorRT engines are supported. Multi-head output engines require modification to `ert_hal_tensorrt.cpp`.
