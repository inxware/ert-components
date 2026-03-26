gpio_out
GPIO Output
Menu: 
            Peripherals
             > GPIO > GPIO Out
Type: IO | Width: 65 | Height: 48

            ┌─────────────────────────┐
     write►─┤                         ├►─-- 
            │          GPIO           │
     value──┤                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **Pin ID:** 0 (0 to 32767) - Pin ID
- **Intial Value:** 0 (0 to 2) - Intial Value (2 Hi Z) 
- **Open Drain:** 0 (Bool) - Logic High = Hi Z) 

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 2 ports (2 events, 0 data)

**Ports by Function:**

*write:*
  - **write** (Start Event)
  - **value** (B) - Input
  - **done** (Finish Event)
  - **error** (Finish Event)
