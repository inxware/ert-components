ups
Uninterruptible power supply management. Reads battery and mains status, and fir
Menu: 
            Peripherals
             > UPS
Type: IO | Width: 50 | Height: 65

            ┌─────────────────────────┐
      read►─┤                         ├►─-- 
            │                         ├──on battery (B)
            │           UPS           │
            │                         ├──battery mV (I)
            │                         ├►─-- 
            │                         ├►─-- 
            └─────────────────────────┘

            Internal Events:
              • _power_fail_trigger
              • _power_restore_trigger

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  1 ports (1 events, 0 data)
- **Right:** 5 ports (3 events, 2 data)

**Ports by Function:**

*ReadStatusFunc:*
  - **read** (Start Event)
  - **on_battery** (B) - Output
  - **battery_mv** (I) - Output
  - **done** (Finish Event)

*PowerFailCbFunc:*
  - **_power_fail_trigger** (Internal Event - callable from ISR/thread)
  - **power_fail** (Finish Event)

*PowerRestoreCbFunc:*
  - **_power_restore_trigger** (Internal Event - callable from ISR/thread)
  - **power_restored** (Finish Event)
