modbus_slave_register
Slave register for the MODBUS protocol.
Menu: 
            Peripherals
             > MODBUS > Slave Register
Type: IO | Height: 90

            ┌─────────────────────────┐
       Set►─┤                         ├►─-- 
      Data──┤                         ├►─Error 
            │       MODBUS Reg        │
       Get►─┤                         ├►─-- 
            │                         ├──Data (I)
            │                         ├►─cb 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **register_type:** 0 (0 to 3) - The type of the register. Varied from Input, Coil, [Display: 30,20]
- **address:** 0 (0 to 65536) - The register address to be written [Display: 30,40]

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 5 ports (4 events, 1 data)
