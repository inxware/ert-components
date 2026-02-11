MultiplexFourInputBool
This is a 4-input boolean multiplexer.
Menu: 
            Data Utilities
             > Selector > Mux4 > bool
Type: Data_Processor | Width: 30 | Height: 120

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (B)
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
