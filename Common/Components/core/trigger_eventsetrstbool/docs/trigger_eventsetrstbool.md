EventSetReset
This is a boolean that can be set and reset by events.
Menu: 
            Event
             > Boolean Control > Set Reset Bool
Type: Event_Processor | Width: 40 | Height: 45

            ┌─────────────────────────┐
       set►─┤                         ├►─ 
            │           SR            │
       rst►─┤                         ├── (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  2 ports (2 events, 0 data)
- **Right:** 2 ports (1 events, 1 data)
