# Embedded Vision SoC Comparison

## Overview
This table summarizes key embedded vision SoCs, their compute hardware, acceleration APIs, AI capabilities, and typical use cases. It includes practical insights for OpenCV / OpenCL / OpenVX pipelines.

---

## Architecture Notes

- **Jetson**: GPU-centric, CUDA + Tensor cores for AI, excellent ecosystem.  
- **RK3588**: Hybrid GPU + NPU, OpenCL + Vulkan compute, strong CPU.  
- **i.MX8M Plus**: GPU + NPU, OpenCL/OpenVX, industrial reliability.  
- **TI Jacinto**: DSP + vision accelerators, OpenVX, deterministic pipelines.  
- **Qualcomm RB5**: DSP + GPU hybrid, OpenCL/OpenVX, mobile/robotics focus.  
- **Intel N100/iGPU**: OpenCL/OpenVINO, suitable for low-power AI/vision.

---

## SoC Comparison

| SoC | CPU | GPU | Vision / AI Accelerator | AI Performance | Key APIs | Power (W) | Typical Use Case | Realistic Vision Pipeline FPS |
|---|---|---|---|---|---|---|---|---|
| **NVIDIA Jetson Orin Nano** | 6× Cortex‑A78 | Ampere GPU | Tensor cores + DLA | ~40–70 TOPS | CUDA, TensorRT | 10–15 | Robotics, AI pipelines | 60–120+ FPS (1080p, multi-camera AI) |
| **Rockchip RK3588** | 4× Cortex‑A76 + 4× Cortex‑A55 | Mali‑G610 | 6 TOPS NPU | 6 TOPS | OpenCL 2.2, Vulkan | 5–8 | High-performance SBC, AI vision | 30–60 FPS (1080p OpenCL CV + NPU inference) |
| **NXP i.MX8M Plus** | 4× Cortex‑A53 + M7 | Vivante GC7000UL | 2.3 TOPS NPU | 2.3 TOPS | OpenCL 1.2, OpenVX | 2–5 | Industrial vision, embedded devices | 20–40 FPS (1080p OpenVX pipelines) |
| **TI TDA4VM (Jacinto)** | Cortex‑A72 + MCU cores | GPU + DSP | Vision accelerators | ~8–16 TOPS | OpenVX (TIOVX) | 5–10 | Automotive ADAS, industrial | 30–60 FPS (multi-camera ADAS pipelines) |
| **Qualcomm RB5** | Kryo CPU | Adreno GPU | Hexagon DSP + AI engine | ~15 TOPS | OpenCL, OpenVX | ~10 | Robotics, mobile vision | 40–80 FPS (1080p OpenCL + DSP CV) |
| **Intel N100 / iGPU** | x86 cores | Intel Xe | iGPU + VPU | varies | OpenCL, OpenVINO | 6–15 | Low-power AI inference, vision | 20–50 FPS (OpenVINO optimized pipelines) |

---

## Architecture Notes

- **Jetson**: GPU-centric, CUDA + Tensor cores, best ecosystem, excellent for real-time AI robotics.  
- **RK3588**: Hybrid GPU + NPU, OpenCL + Vulkan compute, strong CPU performance.  
- **i.MX8M Plus**: GPU + NPU, OpenCL/OpenVX, industrial reliability, great for embedded vision devices.  
- **TI Jacinto**: DSP + vision accelerators, OpenVX/TIOVX, deterministic pipelines, widely used in automotive.  
- **Qualcomm RB5**: DSP + GPU hybrid, OpenCL/OpenVX, good for robotics/mobile.  
- **Intel N100/iGPU**: OpenCL/OpenVINO, low-power AI/vision inference, limited GPU compute.


---

## Acceleration API Summary

| API | Supported Platforms | Typical Use |
|---|---|---|
CUDA | Jetson | GPU compute, AI inference |
OpenCL | RK3588, i.MX8, Intel iGPU | GPU compute for OpenCV pipelines |
OpenVX | i.MX8, TI Jacinto, Qualcomm RB5 | Embedded vision pipelines, graph-based scheduling |
OpenCV CPU | All | Baseline image processing, fallback |

---

## Vision Pipeline Performance Insights

- **Raspberry Pi 4/5**: CPU-only processing, ~10–20 FPS for 1080p pipelines. GPU mostly for video decode.  
- **Jetson Orin Nano**: Multi-camera AI + CV pipelines can reach 60–120+ FPS.  
- **RK3588**: OpenCL + NPU pipelines ~30–60 FPS depending on workload.  
- **i.MX8M Plus**: OpenVX pipelines ~20–40 FPS at 1080p.  
- **TI Jacinto TDA4VM**: Multi-camera ADAS ~30–60 FPS, deterministic.  
- **Qualcomm RB5**: DSP + GPU pipelines ~40–80 FPS.  
- **Intel N100 / iGPU**: OpenVINO-optimized pipelines ~20–50 FPS.

---

## Notes
- CUDA is NVIDIA-specific, Tensor cores provide huge AI acceleration advantage.  
- OpenCL is supported on RK3588, i.MX8, and some Intel iGPUs.  
- OpenVX is best supported on NXP i.MX8, TI Jacinto, and some Qualcomm SoCs.  
- Power consumption varies widely; choose according to thermal and battery constraints.  
- Actual FPS depends on CPU/NPU/GPU utilization, memory bandwidth, and camera resolution.  
- OpenVX provides deterministic embedded vision acceleration; OpenCL is more general-purpose GPU compute.  
- For industrial embedded vision, i.MX8 + OpenVX often provides better stability than high-FPS SBCs.

## Practical Recommendations

| Use Case | Recommended Platform |
|---|---|
AI robotics | Jetson Orin |
Industrial embedded vision | i.MX8M Plus |
Automotive / safety systems | TI Jacinto |
Cheap high-performance SBC | RK3588 |
Low-power smart devices | i.MX8M Plus |

---




