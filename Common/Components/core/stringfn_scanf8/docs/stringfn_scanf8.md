stringfn_scan8
This is an 8-field string parser. Use the format property to identify the string
Menu: 
            Data Utilities
             > String Functions > sscanf > 8 ouput
Type: Data_Processor | Width: 65 | Height: 135

            ┌─────────────────────────┐
          ►─┤                         ├►─ok 
            │                         ├►─error 
    string──┤                         ├──s1 (S)
            │                         ├──s2 (S)
            │                         ├──s3 (S)
            │         Sscanf          │
            │                         ├──s4 (S)
            │                         ├──s5 (S)
            │                         ├──s6 (S)
            │                         ├──s7 (S)
            │                         ├──s8 (S)
       fmt──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Format:** %s%s%s%s%s%s%s%s (String) - C-style format string - supports %s only 

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 10 ports (2 events, 8 data)
