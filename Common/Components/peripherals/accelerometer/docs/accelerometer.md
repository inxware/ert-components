accelerometer
3-axis accelerometer. Returns X, Y, Z acceleration values in milli-g.
Menu: 
            Peripherals
             > Accelerometer
Type: IO | Width: 50 | Height: 90

            ┌─────────────────────────┐
    enable►─┤                         ├►─-- 
   disable►─┤                         ├►─-- 
            │                         ├──error id (I)
            │          Accel          │
      read►─┤                         ├►─-- 
            │                         ├──X (mg) (F)
            │                         ├──Y (mg) (F)
            │                         ├──Z (mg) (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (3 events, 0 data)
- **Right:** 7 ports (3 events, 4 data)

**Ports by Function:**

*EnableFunc:*
  - **enable** (Start Event)
  - **error_id** (I) - Output
  - **enabled** (Finish Event)

*DisableFunc:*
  - **disable** (Start Event)
  - **disabled** (Finish Event)

*ReadFunc:*
  - **read** (Start Event)
  - **x** (F) - Output
  - **y** (F) - Output
  - **z** (F) - Output
  - **done** (Finish Event)
