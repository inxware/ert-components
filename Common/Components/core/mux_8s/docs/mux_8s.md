MultiplexEightInputString
This is a 8-input string multiplexer.
Menu: 
            Data Utilities
             > Selector > Mux8 > string
Type: Data_Processor | Width: 30 | Height: 220

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (S)
          ►─┤                         │
        i2──┤                         │
          ►─┤                         │
        i3──┤                         │
          ►─┤                         │
        i4──┤                         │
            │           Mux           │
          ►─┤                         │
        i5──┤                         │
          ►─┤                         │
        i6──┤                         │
          ►─┤                         │
        i7──┤                         │
          ►─┤                         │
        i8──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  16 ports (8 events, 8 data)
- **Right:** 2 ports (1 events, 1 data)
