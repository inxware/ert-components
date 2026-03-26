gpio_in
GPIO Input
Menu: 
            Peripherals
             > GPIO > GPIO In
Type: IO | Width: 75 | Height: 58

            ┌─────────────────────────┐
      read►─┤                         ├►─finishevent 
            │          GPIO           │
            │                         ├──value (B)
            │                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Pin ID:** 0 (0 to 32767) - Pin ID

**Port Summary:**
- **Left:**  1 ports (1 events, 0 data)
- **Right:** 3 ports (2 events, 1 data)

**Ports by Function:**

*read:*
  - **read** (Start Event)
  - **value** (B) - Output
  - **finishevent** (Finish Event)
  - **error** (Finish Event)
