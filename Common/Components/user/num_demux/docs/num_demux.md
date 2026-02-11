NumDemultiplexReal
This is a 8-output real number demultiplexer.
Menu: 
            Data Utilities
             > Num > Num Demux
Type: Data_Processor

            ┌─────────────────────────┐
         i►─┤                         ├►─o 
        i1──┤                         ├──-ve (B)
            │                         ├──n1 (F)
            │                         ├──n2 (F)
            │                         ├──n3 (F)
            │           nDX           │
            │                         ├──n4 (F)
            │                         ├──n5 (F)
            │                         ├──n6 (F)
            │                         ├──d1 (F)
            │                         ├──d2 (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 10 ports (1 events, 9 data)
