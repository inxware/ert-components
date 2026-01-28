modbus_read
MODBUS-RT input/read 
Menu: 
            Peripherals
             > MODBUS > MODBUS Master Read
Type: IO | Width: 115 | Height: 70

            ┌─────────────────────────┐
      Read►─┤                         ├►─-- 
    device──┤                         ├──value (I)
            │        MODBUS In        │
      type──┤                         ├►─Error 
  register──┤                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **device_id:** 0 (0 to 255) - The Slave ID where the master interacts with. [Display: 40,30]
- **reg_type:** 0 (0 to 3) - Register type to operate [Display: 40,40]
- **reg_addr:** 0 (0 to 65535) - Register Address to handle with [Display: 40,50]

**Port Summary:**
- **Left:**  4 ports (1 events, 3 data)
- **Right:** 4 ports (2 events, 2 data)
