led
LED
Menu: 
            Peripherals
             > LED
Type: IO

            ┌─────────────────────────┐
    enable►─┤                         ├►─-- 
   disbale►─┤                         ├►─-- 
            │                         ├──err id (I)
            │           LED           │
       set►─┤                         ├►─-- 
    on/off──┤                         │
         %──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **ID:** 0 (0 to 255) - LED ID
- **ON/OFF:** 0 (Bool) - Default state ON/OFF
- **Brightness:** 100 (0 to 100) - Default LED brightness (0-100)%

**Port Summary:**
- **Left:**  5 ports (3 events, 2 data)
- **Right:** 4 ports (3 events, 1 data)
