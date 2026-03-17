rs485_config
RS-485/RS-422 advanced line configuration. Configures local echo and line termin
Menu: 
            Peripherals
             > Serial > RS-485 Config
Type: IO | Width: 50 | Height: 55

             ┌─────────────────────────┐
     config►─┤                         ├►─-- 
 local echo──┤                         ├──error id (I)
             │           485           │
termination──┤                         │
       read►─┤                         ├►─-- 
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Port:** 0 (0 to 3) - Serial port index (0 = first RS-485 port).

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 3 ports (2 events, 1 data)

**Ports by Function:**

*ConfigFunc:*
  - **config** (Start Event)
  - **echo** (B) - Input
  - **termination** (B) - Input
  - **error_id** (I) - Output
  - **done** (Finish Event)

*ReadFunc:*
  - **read** (Start Event)
  - **read_done** (Finish Event)
