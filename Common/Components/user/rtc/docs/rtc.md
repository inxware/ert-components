rtc
rtc
Menu: 
            Peripherals
             > RTC
Type: IO | Width: 105 | Height: 150

            ┌─────────────────────────┐
            │                         ├►─-- 
       set►─┤                         │
   seconds──┤                         │
      read►─┤                         ├►─-- 
        tz──┤                         ├──year (I)
            │           RTC           │
            │                         ├──month (I)
            │                         ├──dom (I)
            │                         ├──dow (I)
            │                         ├──hour (I)
            │                         ├──minute (I)
            │                         ├──second (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 10 ports (2 events, 8 data)
