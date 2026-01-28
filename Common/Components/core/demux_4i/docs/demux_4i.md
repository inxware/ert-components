DemultiplexFourOutputInt
This is a 4-output integer demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux4 > int
Type: Data_Processor | Width: 30 | Height: 120

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (I)
        s3►─┤                         ├►─ 
            │                         ├──o2 (I)
            │           DMX           │
        s4►─┤                         ├►─ 
          ──┤                         ├──o3 (I)
            │                         ├►─ 
            │                         ├──o4 (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  5 ports (4 events, 1 data)
- **Right:** 8 ports (4 events, 4 data)
