usb_power
Software-controlled USB port power switching. Enables or disables power to an in
Menu: 
            Peripherals
             > USB > USB Power
Type: IO | Width: 50 | Height: 65

             ┌─────────────────────────┐
     enable►─┤                         ├►─--
    disable►─┤                         ├►─-- 
             │           USB           │
             │                         ├──error id (I)
       read►─┤                         ├►─-- 
             │                         ├──powered (B)
             └─────────────────────────┘
    Legend: ── Data | ►─ Event

**Parameters (1):**
- **Port:** 1 (1 to 4) - USB port number to control (1-4).

**Port Summary:**
- **Left:**  3 ports (3 events, 0 data)
- **Right:** 5 ports (3 events, 2 data)

**Ports by Function:**

*EnableFunc:*
  - **enable** (Start Event)
  - **error_id** (I) - Output
  - **enabled** (Finish Event)

*DisableFunc:*
  - **disable** (Start Event)
  - **disabled** (Finish Event)

*ReadStatusFunc:*
  - **read_status** (Start Event)
  - **powered** (B) - Output
  - **status_done** (Finish Event)
