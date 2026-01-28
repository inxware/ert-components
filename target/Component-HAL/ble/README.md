# BLE Service HAL Implementation

This directory contains the Hardware Abstraction Layer (HAL) for the BLE Service component.

## Directory Structure

```
ble/
├── nimble/                  # NimBLE stack implementation (ESP32-S3)
│   ├── ble_service_nimble.c
│   └── ble_service_nimble.h
├── ble.mk                   # Makefile for BLE HAL
└── README.md                # This file
```

## NimBLE Implementation (ESP32-S3)

The NimBLE implementation provides full BLE GATT server functionality for ESP32-S3 platforms.

### Features

- **GATT Service Creation**: Create custom BLE services with up to 16 characteristics
- **Characteristic Properties**: Read, Write, Notify, and Indicate support
- **Connection Management**: Handle client connections and disconnections
- **Advertising Control**: Start/stop BLE advertising with configurable intervals
- **MTU Configuration**: Configurable MTU size (23-512 bytes)
- **Event Callbacks**: Async notifications for client writes, connections, and disconnections

### Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    inx-ble_service.c                        │
│                  (Component Implementation)                  │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│              inx-ble_service_hal_glue.c                     │
│           (Platform-Independent Glue Layer)                  │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│          ble/nimble/ble_service_nimble.c                    │
│            (ESP32-S3 NimBLE Implementation)                  │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                   NimBLE Stack (ESP-IDF)                    │
│              Bluetooth Low Energy Protocol                   │
└─────────────────────────────────────────────────────────────┘
```

### API Functions

#### Initialization
```c
ehs_sint32 inx_ble_service_hal_init(
    const char* service_uuid,
    const char* service_name,
    uint8_t num_chars,
    uint32_t adv_interval_ms,
    uint16_t mtu_size,
    inx_ble_char_config_t* char_configs,
    inx_ble_service_callbacks_t* callbacks,
    void* component_context);
```

Initializes the BLE service with the specified configuration.

#### GATT Registration
```c
ehs_sint32 inx_ble_service_hal_register_gatt(void);
```

Registers the GATT service and characteristics with the BLE stack.

#### Advertising Control
```c
ehs_sint32 inx_ble_service_hal_start_adv(void);
ehs_sint32 inx_ble_service_hal_stop_adv(void);
```

Start and stop BLE advertising.

#### Characteristic Operations
```c
ehs_sint32 inx_ble_service_hal_write_char(uint8_t char_idx, const char* data, uint16_t length);
ehs_sint32 inx_ble_service_hal_read_char(uint8_t char_idx, char* data, uint16_t* length, uint16_t max_len);
ehs_sint32 inx_ble_service_hal_notify(uint8_t char_idx, const char* data, uint16_t length);
```

Read, write, and notify operations for characteristics.

### Callbacks

The HAL provides three callback functions for async events:

1. **on_client_write**: Called when a client writes to a characteristic
2. **on_connect**: Called when a client connects
3. **on_disconnect**: Called when a client disconnects

These callbacks trigger the corresponding InternalPort functions in the component, which then fire finish events to the component graph.

### UUID Format

The implementation supports both 16-bit and 128-bit UUIDs:

- **16-bit UUID**: `"180A"` (4 hex digits)
- **128-bit UUID**: `"0000180A-0000-1000-8000-00805F9B34FB"` (standard UUID format)

### Characteristic Properties

Properties are specified as a bitmask:

- `0x01` - Read
- `0x02` - Write
- `0x04` - Notify
- `0x08` - Indicate

Example: `0x03` (Read + Write), `0x06` (Write + Notify)

### Example Usage

From the component's perspective:

1. **Initialize**: Call `init` port to initialize BLE service
2. **Start Advertising**: Call `start_adv` port to begin advertising
3. **Wait for Connection**: `connect_evt` fires when client connects
4. **Handle Writes**: `client_wrote_evt` fires when client writes to a characteristic
5. **Send Notifications**: Call `notify` port to send data to connected client
6. **Handle Disconnection**: `disconnect_evt` fires when client disconnects

### Platform Requirements

- ESP-IDF 4.4 or later
- NimBLE stack enabled in ESP-IDF configuration
- FreeRTOS for task management
- Sufficient heap memory for BLE stack (~50KB)

### Build Integration

The HAL is automatically included when building for ESP32 platforms. The makefile `ble.mk` handles platform detection and source file inclusion.

## Future Implementations

Additional BLE stacks can be added as subdirectories:

- `bluez/` - Linux BlueZ stack
- `win32/` - Windows BLE API
- `android/` - Android BLE API
- `stubbed/` - Stubbed implementation for testing

## Notes

- The implementation uses a single global context (one BLE service per application)
- GATT definitions are dynamically allocated but not freed (persistent for application lifetime)
- The NimBLE host task runs continuously in a FreeRTOS task
- MTU negotiation happens automatically after connection
