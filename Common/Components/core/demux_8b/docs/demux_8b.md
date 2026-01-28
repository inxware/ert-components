DemultiplexEightOutputBool
This is a 8-output boolean demultiplexer.
Menu: 
            Data Utilities
             > Switcher > Demux8 > bool
Type: Data_Processor | Width: 30 | Height: 220

            ┌─────────────────────────┐
        s1►─┤                         ├►─ 
        s2►─┤                         ├──o1 (B)
        s3►─┤                         ├►─ 
            │                         ├──o2 (B)
        s4►─┤                         ├►─ 
        s5►─┤                         ├──o3 (B)
        s6►─┤                         ├►─ 
        s7►─┤                         ├──o4 (B)
            │           DMX           │
        s8►─┤                         ├►─ 
            │                         ├──o5 (B)
          ──┤                         ├►─ 
            │                         ├──o6 (B)
            │                         ├►─ 
            │                         ├──o7 (B)
            │                         ├►─ 
            │                         ├──o8 (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  9 ports (8 events, 1 data)
- **Right:** 16 ports (8 events, 8 data)
