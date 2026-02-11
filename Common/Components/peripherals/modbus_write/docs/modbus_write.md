modbus_write
MODBUS-RT output/write 
Menu: 
            Peripherals
             > MODBUS > MODBUS Master Write
Type: IO | Width: 115 | Height: 80

            ┌─────────────────────────┐
       Set►─┤                         ├►─-- 
    device──┤                         ├►─error 
            │       MODBUS Out        │
      type──┤                         ├──Error (I)
  register──┤                         │
     value──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **device_id:** 0 (0 to 255) - MODBUS bus ID [Display: 40,30]
- **reg_type:** 0 (0 to 3) - Register type from 0 to 3 [Display: 40,40]
- **reg_addr:** 0 (0 to 65535) - Register Address [Display: 40,50]

**Port Summary:**
- **Left:**  5 ports (1 events, 4 data)
- **Right:** 3 ports (2 events, 1 data)
