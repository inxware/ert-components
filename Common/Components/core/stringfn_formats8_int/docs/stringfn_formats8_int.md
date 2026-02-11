string_format8_int
This is an 8-input integer formatter. Use the format property to show how the st
Menu: 
            Data Utilities
             > String Functions > format > 8 input int
Type: Data_Processor | Width: 50 | Height: 145

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (S)
        i2──┤                         │
        i3──┤                         │
        i4──┤                         │
            │         Format          │
        i5──┤                         │
        i6──┤                         │
        i7──┤                         │
        i8──┤                         │
       fmt──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Format:** %d%d%d%d%d%d%d%d (String) - C-style format string - supports %d or %u only

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 2 ports (1 events, 1 data)
