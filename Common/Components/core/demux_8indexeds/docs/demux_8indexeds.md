IndexedDemultiplexer_String
Distributes Data to Indexed Output
Menu: 
            Data Utilities
             > Switcher > Indexed > string
Type: Data_Processor | Width: 30 | Height: 235

            ┌─────────────────────────┐
        go►─┤                         │
            │                         ├►─ 
       ind──┤                         ├──1 (S)
       val──┤                         ├►─ 
            │                         ├──2 (S)
            │                         ├►─ 
            │                         ├──3 (S)
            │                         ├►─ 
            │                         ├──4 (S)
            │           DMX           │
            │                         ├►─ 
            │                         ├──5 (S)
            │                         ├►─ 
            │                         ├──6 (S)
            │                         ├►─ 
            │                         ├──7 (S)
            │                         ├►─ 
            │                         ├──8 (S)
            │                         ├►─ovflw 
            │                         ├──offs (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Index start:** 1 (-2147483648 to 2147483647) - Value for firing port 1

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 18 ports (9 events, 9 data)
