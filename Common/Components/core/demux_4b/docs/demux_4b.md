DemultiplexFourOutputBool
This is a 4-output boolean demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux4 > bool
Type: Data_Processor | Width: 30 | Height: 120

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (B)
        s3►─┤                         ├►─ 
            │                         ├──o2 (B)
            │           DMX           │
        s4►─┤                         ├►─ 
          ──┤                         ├──o3 (B)
            │                         ├►─ 
            │                         ├──o4 (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  5 ports (4 events, 1 data)
- **Right:** 8 ports (4 events, 4 data)
