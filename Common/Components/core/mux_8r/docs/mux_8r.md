MultiplexEightInputReal
This is a 8-input real multiplexer.
Menu: 
            Data Utilities
             > Selector > Mux8 > real
Type: Data_Processor | Width: 30 | Height: 220

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (F)
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
