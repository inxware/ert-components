MultiplexTwoInputInt
This is a 2-input integer multiplexer.
Menu: 
            Data Utilities
             > Selector > Mux2 > int
Type: Data_Processor | Width: 30 | Height: 70

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        i1──┤                         ├── (I)
            │           Mux           │
          ►─┤                         │
        i2──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 2 ports (1 events, 1 data)
