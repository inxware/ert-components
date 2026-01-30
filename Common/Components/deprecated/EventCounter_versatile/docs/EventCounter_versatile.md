EventCounterVersatile
event counter providing option to set threshold with an input value.
Menu: 
			Deprecated
			 > Event Counter Versatile
Type: Event_Processor

            ┌─────────────────────────┐
     start►─┤                         │
    thresh──┤                         │
      inc.►─┤                         ├►─ 
       dec►─┤                         ├►─ 
       rst►─┤                         ├── (I)
      stop►─┤                         ├►─ovf 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Threshold:** 1000 (0 to 2147483647) - Threshold value for ovf event.

**Port Summary:**
- **Left:**  6 ports (5 events, 1 data)
- **Right:** 4 ports (3 events, 1 data)
