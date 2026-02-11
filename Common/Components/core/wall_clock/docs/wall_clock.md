wall_clock
CSets and Retrieves the current system time.
Menu: 
            System
             > Local > Date & Time
Type: Event_Processor | Height: 215

            ┌─────────────────────────┐
       set►─┤                         ├►─----- 
 unix time──┤                         │
      time──┤                         │
  timezone──┤                         │
       get►─┤                         ├►─------ 
            │                         ├──date string (S)
            │                         ├──unix time (I)
            │                         ├──mins 1970 (I)
            │          Clock          │
            │                         ├──& secs (I)
            │                         ├──year (I)
            │                         ├──month (I)
            │                         ├──MDay (I)
            │                         ├──WDay (I)
            │                         ├──hour (I)
            │                         ├──minute (I)
            │                         ├──second (I)
            │                         ├──local diff (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **format:** 0 (0 to 3) - Date output format [Display: 20,20]

**Port Summary:**
- **Left:**  5 ports (2 events, 3 data)
- **Right:** 14 ports (2 events, 12 data)
