IndexedDemultiplexer_Float
Distributes Data to Indexed Output
Menu: 
            Data Utilities
             > Switcher > Indexed > real
Type: Data_Processor | Width: 30 | Height: 235

            ┌─────────────────────────┐
        go►─┤                         │
            │                         ├►─ 
       ind──┤                         ├──1 (F)
       val──┤                         ├►─ 
            │                         ├──2 (F)
            │                         ├►─ 
            │                         ├──3 (F)
            │                         ├►─ 
            │                         ├──4 (F)
            │           DMX           │
            │                         ├►─ 
            │                         ├──5 (F)
            │                         ├►─ 
            │                         ├──6 (F)
            │                         ├►─ 
            │                         ├──7 (F)
            │                         ├►─ 
            │                         ├──8 (F)
            │                         ├►─ovflw 
            │                         ├──offs (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Index start:** 1 (-2147483648 to 2147483647) - Value for firing port 1

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 18 ports (9 events, 9 data)
