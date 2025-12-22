indexed_mux_bool
Multiplex the boolean inputs according to the index.
Menu: 
            Data Utilities
             > Indexed Mux > bool
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─-- 
     index──┤                         ├►─err 
         1──┤                         ├──out (B)
         2──┤                         │
         3──┤                         │
         4──┤                         │
            │        Bool Mux         │
         5──┤                         │
         6──┤                         │
         7──┤                         │
         8──┤                         │
            │                         ├►─ovf 
            │                         ├──ovf (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (8):**
- **in1:** 0 (Bool)
- **in2:** 0 (Bool)
- **in3:** 0 (Bool)
- **in4:** 0 (Bool)
- **in5:** 0 (Bool)
- **in6:** 0 (Bool)
- **in7:** 0 (Bool)
- **in8:** 0 (Bool)

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 5 ports (3 events, 2 data)
