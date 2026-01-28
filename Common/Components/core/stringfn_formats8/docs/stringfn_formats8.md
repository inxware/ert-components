string_format8
This is an 8-input string formatter. Use the format property to show how the str
Menu: 
            Data Utilities
             > String Functions > format > 8 input
Type: Data_Processor | Width: 50 | Height: 145

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        s1──┤                         ├── (S)
        s2──┤                         │
        s3──┤                         │
        s4──┤                         │
            │         Format          │
        s5──┤                         │
        s6──┤                         │
        s7──┤                         │
        s8──┤                         │
       fmt──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Format:** %s%s%s%s%s%s%s%s (String) - C-style format string - supports %s only

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 2 ports (1 events, 1 data)
