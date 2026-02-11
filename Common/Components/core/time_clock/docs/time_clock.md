time_clock
Creates periodic timed events.
Menu: 
            Event
             > Timer
Type: Event_Processor | Width: 65 | Height: 90

            ┌─────────────────────────┐
     start►─┤                         ├►─ 
    period──┤                         │
            │          Timer          │
       rpt──┤                         │
    retrig──┤                         ├►─clk 
      stop►─┤                         ├►─ 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **Time Interval (uSecs):** 1000000 (1 to 2147483647) - Time interval between events.
- **Periodic Flag:** 1 (Bool) - Periodic if TRUE, single delay only if FALSE. 
- **Retriggerable Flag:** 1 (Bool) - Retriggerable if TRUE. 

**Port Summary:**
- **Left:**  5 ports (2 events, 3 data)
- **Right:** 3 ports (3 events, 0 data)
