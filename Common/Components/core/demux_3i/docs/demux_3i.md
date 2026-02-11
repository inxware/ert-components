DemultiplexThreeOutputInt
This is a 3-output integer demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux3 > int
Type: Data_Processor | Width: 30 | Height: 95

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (I)
        s3►─┤                         ├►─ 
            │           DMX           │
            │                         ├──o2 (I)
          ──┤                         ├►─ 
            │                         ├──o3 (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 6 ports (3 events, 3 data)
