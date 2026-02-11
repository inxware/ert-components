DemultiplexEightOutputReal
This is a 8-output real demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux8 > real
Type: Data_Processor | Width: 30 | Height: 220

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (F)
        s3►─┤                         ├►─ 
            │                         ├──o2 (F)
        s4►─┤                         ├►─ 
        s5►─┤                         ├──o3 (F)
        s6►─┤                         ├►─ 
        s7►─┤                         ├──o4 (F)
            │           DMX           │
        s8►─┤                         ├►─ 
            │                         ├──o5 (F)
          ──┤                         ├►─ 
            │                         ├──o6 (F)
            │                         ├►─ 
            │                         ├──o7 (F)
            │                         ├►─ 
            │                         ├──o8 (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  9 ports (8 events, 1 data)
- **Right:** 16 ports (8 events, 8 data)
