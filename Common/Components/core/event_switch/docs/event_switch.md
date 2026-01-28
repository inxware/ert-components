IndexedDemultiplexer_Event
Switches Event to Indexed Output
Menu: 
            Event
             > Combiners/Flow > Switch
Type: Data_Processor | Width: 30 | Height: 185

            ┌─────────────────────────┐
        go►─┤                         ├►─_ 
       ind──┤                         │
            │                         ├►─1 
            │                         ├►─2 
            │                         ├►─3 
            │                         ├►─4 
            │           Swt           │
            │                         ├►─5 
            │                         ├►─6 
            │                         ├►─7 
            │                         ├►─8 
            │                         ├►─ovf 
            │                         ├──off (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Index start:** 1 (-2147483648 to 2147483647) - Value for firing port 1

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 11 ports (10 events, 1 data)
