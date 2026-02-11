DemultiplexTwoOutputBool
This is a 2-output boolean demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux2 > bool
Type: Data_Processor | Width: 30 | Height: 70

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (B)
            │           DMX           │
            │                         ├►─ 
          ──┤                         ├──o2 (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 4 ports (2 events, 2 data)
