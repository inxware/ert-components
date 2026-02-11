state_manager
Menu: 
            Software Structure
             > State Machine > State Manager
Type: State_Manager | Width: 65 | Height: 55

            ┌─────────────────────────┐
            │                         ├── (U)
    enable►─┤                         ├►─ 
            │                         ├►─ 
            │                         ├──state (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **__nextStateId:** -1 (-2147483648 to 2147483647)

**Port Summary:**
- **Left:**  1 ports (1 events, 0 data)
- **Right:** 4 ports (2 events, 2 data)
