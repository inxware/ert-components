watchdog
Hardware watchdog timer. Enables/disables the watchdog and provides a kick (hear
Menu: 
            Peripherals
             > Watchdog
Type: IO | Width: 50 | Height: 80

            ┌─────────────────────────┐
    enable►─┤                         ├►─-- 
   disable►─┤                         ├►─-- 
            │                         ├──error id (I)
            │          WDog           │
      kick►─┤                         ├►─-- 
    status►─┤                         ├►─-- 
            │                         ├──expired (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Timeout (s):** 60 (1 to 3600) - Watchdog timeout in seconds. System resets if kick

**Port Summary:**
- **Left:**  4 ports (4 events, 0 data)
- **Right:** 6 ports (4 events, 2 data)

**Ports by Function:**

*EnableFunc:*
  - **enable** (Start Event)
  - **error_id** (I) - Output
  - **enabled** (Finish Event)

*DisableFunc:*
  - **disable** (Start Event)
  - **disabled** (Finish Event)

*KickFunc:*
  - **kick** (Start Event)
  - **kicked** (Finish Event)

*ReadStatusFunc:*
  - **read_status** (Start Event)
  - **expired** (B) - Output
  - **status_done** (Finish Event)
