indexed_mux_str
Multiplex the string inputs according to the index.
Menu: 
            Data Utilities
             > Indexed Mux > string
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─-- 
     index──┤                         ├►─err 
         1──┤                         ├──out (S)
         2──┤                         │
         3──┤                         │
         4──┤                         │
            │         Str Mux         │
         5──┤                         │
         6──┤                         │
         7──┤                         │
         8──┤                         │
            │                         ├►─ovf 
            │                         ├──ovf (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (8):**
- **in1:**  (String)
- **in2:**  (String)
- **in3:**  (String)
- **in4:**  (String)
- **in5:**  (String)
- **in6:**  (String)
- **in7:**  (String)
- **in8:**  (String)

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 5 ports (3 events, 2 data)
