DevmanScheduler
This function will retrieve data from the schedule information stored on a devic
Menu: 
            Control Systems
             > Devman Scheduler
Type: Data | Height: 150

            ┌─────────────────────────┐
      init►─┤                         ├►─---- 
            │                         ├──crc (I)
       get►─┤                         ├►─---- 
      time──┤                         ├►─error 
       day──┤                         ├──errno (I)
            │        Scheduler        │
            │                         ├──value (I)
            │                         ├──on (B)
            │                         ├──changed (B)
       set►─┤                         ├►─---- 
      data──┤                         ├►─error 
      size──┤                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **ChannelID:**  (0 to 9) - The ID of the scheduler's channel [Display: 20,20]
- **AutoMode:**  (Bool) - Whether we want the time polled from a clock autom

**Port Summary:**
- **Left:**  7 ports (3 events, 4 data)
- **Right:** 11 ports (5 events, 6 data)
