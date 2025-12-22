indexed_mux_real
Multiplex the real inputs according to the index.
Menu: 
            Data Utilities
             > Indexed Mux > real
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─-- 
     index──┤                         ├►─err 
         1──┤                         ├──out (F)
         2──┤                         │
         3──┤                         │
         4──┤                         │
            │        Real Mux         │
         5──┤                         │
         6──┤                         │
         7──┤                         │
         8──┤                         │
            │                         ├►─ovf 
            │                         ├──ovf (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (8):**
- **in1:** 0 (Real)
- **in2:** 0 (Real)
- **in3:** 0 (Real)
- **in4:** 0 (Real)
- **in5:** 0 (Real)
- **in6:** 0 (Real)
- **in7:** 0 (Real)
- **in8:** 0 (Real)

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 5 ports (3 events, 2 data)
