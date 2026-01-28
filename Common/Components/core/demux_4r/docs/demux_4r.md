DemultiplexFourOutputReal
This is a 4-output real demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux4 > real
Type: Data_Processor | Width: 30 | Height: 120

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (F)
        s3►─┤                         ├►─ 
            │                         ├──o2 (F)
            │           DMX           │
        s4►─┤                         ├►─ 
          ──┤                         ├──o3 (F)
            │                         ├►─ 
            │                         ├──o4 (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  5 ports (4 events, 1 data)
- **Right:** 8 ports (4 events, 4 data)
