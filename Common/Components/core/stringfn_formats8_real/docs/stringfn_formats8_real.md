string_format8_real
This is an 8-input real formatter. Use the format property to show how the strin
Menu: 
            Data Utilities
             > String Functions > format > 8 input real
Type: Data_Processor | Width: 50 | Height: 145

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        r1──┤                         ├── (S)
        r2──┤                         │
        r3──┤                         │
        r4──┤                         │
            │         Format          │
        r5──┤                         │
        r6──┤                         │
        r7──┤                         │
        r8──┤                         │
       fmt──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Format:** %.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f (String) - C-style format string - supports %f only (Should h

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 2 ports (1 events, 1 data)
