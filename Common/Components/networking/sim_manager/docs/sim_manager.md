sim_manager
Reports SIM state and identity, and switches between the SIM types the hardware 
Menu: 
            Networking
             > Cellular > SIM Manager
Type: IO | Width: 105 | Height: 223

            ┌─────────────────────────┐
      Read►─┤                         │
            │                         ├►─Done 
            │                         ├──simState (I)
            │                         ├──activeType (I)
            │                         ├──availableTypes (I)
            │                         ├──selectableTypes (I)
            │                         ├──ICCID (S)
            │                         ├──IMSI (S)
            │                         ├──pinAttemptsLeft (I)
            │       SIM Manager       │
            │                         ├──errCode (I)
SelectType►─┤                         │
   simType──┤                         │
            │                         ├►─SDone 
            │                         ├►─SFail 
  EnterPin►─┤                         │
       PIN──┤                         │
            │                         ├►─PDone 
            │                         ├►─PFail 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **autoEnterPin:** 0 (Bool) - Enter the stored PIN automatically at startup. OFF
- **preferredType:** 0 (0 to 2) - SIM type to select at startup if it is selectable.

**Port Summary:**
- **Left:**  5 ports (3 events, 2 data)
- **Right:** 13 ports (5 events, 8 data)

**Ports by Function:**

*do_read:*
  - **read** (Start Event)
  - **sim_state** (I) - Output
  - **active_type** (I) - Output
  - **available_types** (I) - Output
  - **selectable_types** (I) - Output
  - **iccid** (S) - Output
  - **imsi** (S) - Output
  - **pin_attempts_left** (I) - Output
  - **err_code** (I) - Output
  - **read_ok** (Finish Event)

*do_select_type:*
  - **select_type** (Start Event)
  - **sim_type** (I) - Input
  - **select_ok** (Finish Event)
  - **select_fail** (Finish Event)

*do_enter_pin:*
  - **enter_pin** (Start Event)
  - **pin** (S) - Input
  - **pin_ok** (Finish Event)
  - **pin_fail** (Finish Event)
