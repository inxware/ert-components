elapsed_timer
This returns the elapsed time in microseconds between two event triggers.
Menu: 
            Event
             > Elapsed Timer
Type: Event_Processor | Height: 70

            ┌─────────────────────────┐
     start►─┤                         ├►─-- 
            │         Elapsed         │
      stop►─┤                         ├►─-- 
            │                         ├──elapsed (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  2 ports (2 events, 0 data)
- **Right:** 3 ports (2 events, 1 data)
