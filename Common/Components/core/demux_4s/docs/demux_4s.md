DemultiplexFourOutputString
This is a 4-output string demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux4 > string
Type: Data_Processor | Width: 30 | Height: 120

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (S)
        s3►─┤                         ├►─ 
            │                         ├──o2 (S)
            │           DMX           │
        s4►─┤                         ├►─ 
          ──┤                         ├──o3 (S)
            │                         ├►─ 
            │                         ├──o4 (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  5 ports (4 events, 1 data)
- **Right:** 8 ports (4 events, 4 data)
