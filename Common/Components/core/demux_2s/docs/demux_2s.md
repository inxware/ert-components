DemultiplexTwoOutputString
This is a 2-output string demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux2 > string
Type: Data_Processor | Width: 30 | Height: 70

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (S)
            │           DMX           │
            │                         ├►─ 
          ──┤                         ├──o2 (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 4 ports (2 events, 2 data)
