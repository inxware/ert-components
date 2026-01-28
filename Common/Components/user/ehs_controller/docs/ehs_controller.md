ehs_controller
Controls the current EHS window
Menu: 
            System
             > Local > Runtime Control
Type: Data_Processor | Width: 105 | Height: 105

            ┌─────────────────────────┐
      exit►─┤                         ├►─error[exit] 
      fore►─┤                         ├►─------[fore] 
            │   Runtime Controller    │
            │                         ├►─error[fore] 
      back►─┤                         ├►─------[back] 
            │                         ├►─error[back] 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (3 events, 0 data)
- **Right:** 5 ports (5 events, 0 data)
