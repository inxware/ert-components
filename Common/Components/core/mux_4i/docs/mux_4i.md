MultiplexFourInputInt
This is a 4-input integer multiplexer.
Menu: 
            Data Utilities
             > Selector > Mux4 > int
Type: Data_Processor | Width: 30 | Height: 120

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (I)
          ►─┤                         │
        i2──┤                         │
            │           Mux           │
          ►─┤                         │
        i3──┤                         │
          ►─┤                         │
        i4──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  8 ports (4 events, 4 data)
- **Right:** 2 ports (1 events, 1 data)
