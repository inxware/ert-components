IndexedDemultiplexer_Int
Distributes Data to Indexed Output
Menu: 
            Data Utilities
             > Switcher > Indexed > int
Type: Data_Processor | Width: 30 | Height: 235

            ┌─────────────────────────┐
        go►─┤                         │
            │                         ├►─ 
       ind──┤                         ├──1 (I)
       val──┤                         ├►─ 
            │                         ├──2 (I)
            │                         ├►─ 
            │                         ├──3 (I)
            │                         ├►─ 
            │                         ├──4 (I)
            │           DMX           │
            │                         ├►─ 
            │                         ├──5 (I)
            │                         ├►─ 
            │                         ├──6 (I)
            │                         ├►─ 
            │                         ├──7 (I)
            │                         ├►─ 
            │                         ├──8 (I)
            │                         ├►─ovflw 
            │                         ├──offs (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Index start:** 1 (-2147483648 to 2147483647) - Value for firing port 1

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 18 ports (9 events, 9 data)
