indexed_mux_int
Multiplex the integer inputs according to the index.
Menu: 
            Data Utilities
             > Indexed Mux > int
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─-- 
     index──┤                         ├►─err 
         1──┤                         ├──out (I)
         2──┤                         │
         3──┤                         │
         4──┤                         │
            │         Int Mux         │
         5──┤                         │
         6──┤                         │
         7──┤                         │
         8──┤                         │
            │                         ├►─ovf 
            │                         ├──ovf (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (8):**
- **in1:** 0 (-2147483647 to 2147483647)
- **in2:** 0 (-2147483647 to 2147483647)
- **in3:** 0 (-2147483647 to 2147483647)
- **in4:** 0 (-2147483647 to 2147483647)
- **in5:** 0 (-2147483647 to 2147483647)
- **in6:** 0 (-2147483647 to 2147483647)
- **in7:** 0 (-2147483647 to 2147483647)
- **in8:** 0 (-2147483647 to 2147483647)

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 5 ports (3 events, 2 data)
