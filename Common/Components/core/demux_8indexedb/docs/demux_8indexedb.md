IndexedDemultiplexer_Bool
Distributes Data to Indexed Output
Menu: 
            Data Utilities
             > Switcher > Indexed > bool
Type: Data_Processor | Width: 30 | Height: 235

            ┌─────────────────────────┐
        go►─┤                         │
            │                         ├►─ 
       ind──┤                         ├──1 (B)
       val──┤                         ├►─ 
            │                         ├──2 (B)
            │                         ├►─ 
            │                         ├──3 (B)
            │                         ├►─ 
            │                         ├──4 (B)
            │           DMX           │
            │                         ├►─ 
            │                         ├──5 (B)
            │                         ├►─ 
            │                         ├──6 (B)
            │                         ├►─ 
            │                         ├──7 (B)
            │                         ├►─ 
            │                         ├──8 (B)
            │                         ├►─ovflw 
            │                         ├──offs (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Index start:** 1 (0 to 0) - Value for firing port 1

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 18 ports (9 events, 9 data)
