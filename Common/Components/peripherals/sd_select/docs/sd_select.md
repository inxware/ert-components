sd_select
Dual SD card slot selector. Switches the active boot/storage SD slot and reads t
Menu: 
            Peripherals
             > Storage > SD Card
Type: IO | Width: 50 | Height: 55

               ┌─────────────────────────┐
       select►─┤                         ├►─-- 
slot (0 or 1)──┤                         ├──error id (I)
               │           SD            │
  read active►─┤                         ├►─-- 
               │                         ├──active slot (I)
               └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 4 ports (2 events, 2 data)

**Ports by Function:**

*SelectFunc:*
  - **select** (Start Event)
  - **slot** (I) - Input
  - **error_id** (I) - Output
  - **done** (Finish Event)

*ReadActiveFunc:*
  - **read_active** (Start Event)
  - **active_slot** (I) - Output
  - **read_done** (Finish Event)
