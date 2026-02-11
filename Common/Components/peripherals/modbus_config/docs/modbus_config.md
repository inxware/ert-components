modbus_config
MODBUS-RT config
Menu: 
			Peripherals
			 > MODBUS > Modbus Config
Type: IO | Width: 115 | Height: 100

            ┌─────────────────────────┐
    enable►─┤                         ├►─-- 
    bus_ID──┤                         ├──errno (I)
      port──┤                         │
            │       MODBUS conf       │
        IP──┤                         │
      cert──┤                         │
   disable►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (6):**
- **bus_id:** 0 (0 to 100) - Bus ID [Display: 40,20]
- **master:** 0 (Bool) - Whether MODBUS is in Master mode
- **data_mode:** 0 (0 to 2) - Whether MODBUS is in RTU, ASCII or TCP/IP mode. 0 
- **port_id:** 0 (0 to 2147483647) - The port ID of the MODBUS interface [Display: 40,40]
- **ip:** 127.0.0.1 (String) - The target IP if in TCP/IP mode
- **cert:**  (String) - The certificate filename of the MODBUS TCP/IP comm

**Port Summary:**
- **Left:**  6 ports (2 events, 4 data)
- **Right:** 3 ports (2 events, 1 data)
