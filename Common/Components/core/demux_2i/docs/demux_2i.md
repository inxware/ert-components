DemultiplexTwoOutputInt
This is a 2-output integer demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux2 > int
Type: Data_Processor | Width: 30 | Height: 70

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (I)
            │           DMX           │
            │                         ├►─ 
          ──┤                         ├──o2 (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 4 ports (2 events, 2 data)
