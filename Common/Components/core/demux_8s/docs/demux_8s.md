DemultiplexEightOutputString
This is a 8-output string demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux8 > string
Type: Data_Processor | Width: 30 | Height: 220

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (S)
        s3►─┤                         ├►─ 
            │                         ├──o2 (S)
        s4►─┤                         ├►─ 
        s5►─┤                         ├──o3 (S)
        s6►─┤                         ├►─ 
        s7►─┤                         ├──o4 (S)
            │           DMX           │
        s8►─┤                         ├►─ 
            │                         ├──o5 (S)
          ──┤                         ├►─ 
            │                         ├──o6 (S)
            │                         ├►─ 
            │                         ├──o7 (S)
            │                         ├►─ 
            │                         ├──o8 (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  9 ports (8 events, 1 data)
- **Right:** 16 ports (8 events, 8 data)
