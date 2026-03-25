# ROS2 Component-HAL — Design Reference

This document records the architectural decisions made during the initial ROS2 integration
design session so that future implementation work can proceed without re-litigating them.

---

## Background and scope

The goal is to expose ROS2 as a first-class inxware toolbox — the **Robotics toolbox** — with
function blocks that surface ROS2 pub/sub, services, actions, transforms, and higher-level
navigation/control abstractions. The toolbox targets multi-platform deployment: Linux (arm64,
amd64), QNX, Windows, and MCUs via micro-ROS.

---

## Layer assignment rationale

### What lives in `Common/Components/ros2/` (CDF + C implementation)

Any ROS2 concept whose implementation is **identical across all supported targets** belongs
here with no HAL indirection:

- Core pub/sub, services, actions, parameters (`rclcpp` / `rclc` on micro-ROS where API is
  equivalent)
- Message type libraries: `geometry_msgs`, `std_msgs`, `sensor_msgs`, `nav_msgs`, etc.
  (IDL-generated, OS-independent)
- `tf2` (pure C++ math, no OS calls)
- `tf2_ros` (wraps tf2 with rclcpp topics — same API on Linux, QNX, Windows)

These are called **directly** from the function block C code. No HAL shim is needed because
there is no cross-platform variation to hide.

### What lives under `target/Component-HAL/ros2/` (this directory)

Concepts that exist on some targets but not others, or that have meaningfully different
implementations across targets, get a HAL abstraction here. The primary cases are:

| Concept | Reason for HAL |
|---------|---------------|
| `nav2` navigation stack | Linux-only in practice (BehaviorTree.CPP, Costmap2D, AMCL); absent on QNX, Windows, MCUs |
| `MoveIt2` motion planning | Linux-only; memory/compute requirements preclude other targets |
| `ros2_control` (full controller manager) | Linux-centric for the full stack; micro-ROS has only a minimal subset |
| micro-ROS transport | Serial/UDP/CAN transport init differs per MCU board; no equivalent on Linux |

The HAL provides a uniform inxware API to these features. Variants are selected at build time
via `EHS_ROS2_*_SUPPORT` flags set in `target/os-arch/*/config.mk` or
`target/platform/*/config.mk`.

### What gets a capability flag but no HAL variant

Ecosystem packages that are simply unavailable on some targets (no alternative implementation
exists) are gated by a support flag alone — no alternate variant directory:

```makefile
# target/os-arch/linux_arm64/config.mk
EHS_ROS2_NAV2_SUPPORT   ?= 1
EHS_ROS2_MOVEIT_SUPPORT ?= 1

# QNX, Windows, freertos configs: flags absent → blocks excluded from build
```

---

## Anticipated Component-HAL variant structure

```
target/Component-HAL/ros2/
    README.md                          ← this file
    ros2_node_init/
        rclcpp/                        ← Linux, QNX, Windows
        micro_ros/                     ← FreeRTOS / Zephyr MCUs (rclc, transport init)
        stubbed/                       ← host build / unit tests
    ros2_control_interface/
        full/                          ← Linux — full ros2_control controller manager
        micro_ros_control/             ← MCU — rclc_action / micro ros2_control subset
        stubbed/
    ros2_navigation/                   ← nav2 wrapper (Linux only — no MCU variant)
        nav2/
        stubbed/
    ros2_motion_planning/              ← MoveIt2 wrapper (Linux only)
        moveit2/
        stubbed/
```

`rclcpp`-based variants are selected by all rclcpp-capable os-arch configs. `micro_ros`
variants are selected by `freertos_*` and `zephyr_*` os-arch configs.

---

## Design decisions (fixed)

### 1. Multiple ROS2 nodes per inxware instance

A single inxware runtime can host **multiple ROS2 nodes**. Each node is represented by a
`ros2_node` function block (or equivalent initialisation block). Function blocks that publish,
subscribe, or call services take a node reference as a parameter (or are grouped under a named
node). This mirrors the standard ROS2 multi-node-per-process pattern and allows logical
separation of concerns within one inxware application.

**Rationale:** inxware applications frequently combine multiple subsystems (sensors, actuators,
HMI, comms) in one runtime. Forcing a single node would pollute the topic/service namespace
and prevent independent QoS/lifecycle management per subsystem.

### 2. Typed function blocks per common message type

Rather than one generic publish/subscribe block parameterised by type string, **specific
function blocks are provided for each common message type family**. Approximately 20–25 typed
variants are warranted, covering the most-used types:

- Primitives: `Bool`, `Int32`/`Int64`, `Float32`/`Float64`, `String`
- Geometry: `Point`, `Pose`, `PoseStamped`, `Twist`, `Transform`, `TransformStamped`
- Sensors: `Image`, `LaserScan`, `Imu`, `JointState`, `PointCloud2`
- Navigation: `Path`, `Odometry`, `OccupancyGrid`
- A `GenericSubscriber` block for schema-less / arbitrary structured data (returns raw JSON
  or byte array)

**Rationale:** Typed blocks give the inxware IDE compile-time port type checking, clear
documentation, and no runtime type negotiation overhead. The ~20 most common types cover the
vast majority of real robotics applications. `GenericSubscriber` handles the long tail.

### 3. Robotics toolbox — ROS2 sub-branch in CDF menus

All new function blocks are part of a new **Robotics** toolbox. ROS2-specific blocks appear
under a `Robotics > ROS2 > ...` menu hierarchy in the CDF `<Menu>` field, e.g.:

```xml
<Menu>
    Robotics
    <Menu>
        ROS2
        <Menu>Publishers</Menu>
    </Menu>
</Menu>
```

Non-ROS2 robotics blocks (kinematics utilities, trajectory math, etc.) sit under `Robotics`
directly. This makes ROS2 dependency explicit and keeps the menu navigable.

**Rationale:** Naming blocks by their menu location rather than by an internal prefix keeps
the CDF class names clean and makes the ROS2 dependency visible to the integrator at block
selection time, not just at link time.

---

## Target OS/arch support summary

| OS-arch | ROS2 core | tf2 | nav2 / MoveIt2 | ros2_control | micro-ROS |
|---------|-----------|-----|----------------|--------------|-----------|
| linux_arm64 | rclcpp | yes | yes (Linux only) | full | no |
| linux_amd64 | rclcpp | yes | yes | full | no |
| qnx_arm64 | rclcpp | yes | no (unported) | partial | no |
| windows_x86_64 | rclcpp | yes | no (unported) | partial | no |
| freertos_xtensa (ESP32) | — | no | no | no | micro-ROS |
| freertos_arm (other MCUs) | — | no | no | no | micro-ROS |

---

## Next implementation steps

1. Create `Common/Components/ros2/` directory and initial CDFs:
   - `ros2_node.cdf` — node init/shutdown, spin policy parameter
   - `ros2_publish_<type>.cdf` for each typed publisher family
   - `ros2_subscribe_<type>.cdf` for each typed subscriber family
   - `ros2_service_call.cdf`, `ros2_service_server.cdf`
   - `ros2_action_client.cdf`, `ros2_action_server.cdf`
   - `ros2_get_param.cdf`, `ros2_set_param.cdf`
   - `ros2_tf2_lookup.cdf`, `ros2_tf2_broadcast.cdf`
   - `ros2_generic_subscribe.cdf`

2. Create `target/Component-HAL/ros2/ros2_node_init/rclcpp/` and `micro_ros/` skeleton
   implementations.

3. Add `EHS_ROS2_SUPPORT`, `EHS_ROS2_NAV2_SUPPORT`, `EHS_ROS2_MOVEIT_SUPPORT` flags to
   appropriate `target/os-arch/*/config.mk` files.

4. Add `ros2` entry to `target/Component-HAL/component-hal.mk` build dispatch.

5. Generate help HTML for each CDF (`make components_gendocs`).
