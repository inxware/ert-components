DemultiplexThreeOutputBool
This is a 3-output boolean demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux3 > bool
Type: Data_Processor | Width: 30 | Height: 95

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (B)
        s3►─┤                         ├►─ 
            │           DMX           │
            │                         ├──o2 (B)
          ──┤                         ├►─ 
            │                         ├──o3 (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 6 ports (3 events, 3 data)
