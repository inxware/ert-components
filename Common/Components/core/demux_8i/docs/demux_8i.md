DemultiplexEightOutputInt
This is a 8-output integer demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux8 > int
Type: Data_Processor | Width: 30 | Height: 220

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (I)
        s3►─┤                         ├►─ 
            │                         ├──o2 (I)
        s4►─┤                         ├►─ 
        s5►─┤                         ├──o3 (I)
        s6►─┤                         ├►─ 
        s7►─┤                         ├──o4 (I)
            │           DMX           │
        s8►─┤                         ├►─ 
            │                         ├──o5 (I)
          ──┤                         ├►─ 
            │                         ├──o6 (I)
            │                         ├►─ 
            │                         ├──o7 (I)
            │                         ├►─ 
            │                         ├──o8 (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  9 ports (8 events, 1 data)
- **Right:** 16 ports (8 events, 8 data)
