MultiplexThreeInputInt
This is a 3-input integer multiplexer.
Menu: 
            Data Utilities
             > Selector > Mux3 > int
Type: Data_Processor | Width: 30 | Height: 95

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (I)
          ►─┤                         │
            │           Mux           │
        i2──┤                         │
          ►─┤                         │
        i3──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 2 ports (1 events, 1 data)
