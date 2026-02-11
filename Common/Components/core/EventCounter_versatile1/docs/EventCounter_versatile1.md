EventCounterVersatile1
event counter providing option to set a maximum and minimum threshold with an in
Menu: 
            Event
             > Counters/Iterators > Counter Versatile
Type: Event_Processor | Width: 65 | Height: 175

            ┌─────────────────────────┐
       rst►─┤                         ├►─-- 
       inc►─┤                         ├►─-- 
            │                         ├►─ovf 
       dec►─┤                         ├►─-- 
            │                         ├►─ovf 
            │                         ├──cnt (I)
            │         Counter         │
     enabl►─┤                         ├►─-- 
    disabl►─┤                         ├►─-- 
       ini──┤                         │
      step──┤                         │
       max──┤                         │
       min──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **Enable at Init:** 1 (Bool) - Enable the counter at initialisation time if TRUE,
- **Initial Value:** 0 (-2147483648 to 2147483647) - Initial Value for count.
- **Step Size:** 1 (1 to 2147483647) - Value incremented/decremented on each count.
- **Max. Threshold:** 1000 (-2147483648 to 2147483647) - Threshold value for ovf event.
- **Min. Threshold:** 0 (-2147483648 to 2147483647) - Threshold value for ovf event.

**Port Summary:**
- **Left:**  9 ports (5 events, 4 data)
- **Right:** 8 ports (7 events, 1 data)
