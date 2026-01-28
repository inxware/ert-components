TextCaseDemultiplexer_Event
Switches Event based on text cases
Menu: 
            Event
             > Combiners/Flow > TextSwitch
Type: Data_Processor | Width: 30 | Height: 185

            ┌─────────────────────────┐
        go►─┤                         │
      test──┤                         │
         1──┤                         ├►─_ 
         2──┤                         ├►─_ 
         3──┤                         ├►─_ 
            │          TxSw           │
         4──┤                         ├►─_ 
         5──┤                         ├►─_ 
         6──┤                         ├►─_ 
         7──┤                         ├►─_ 
         8──┤                         ├►─_ 
            │                         ├►─deflt 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (9):**
- **case 1:**  (String) - case 1 string
- **case 2:**  (String) - case 2 string
- **case 3:**  (String) - case 3 string
- **case 4:**  (String) - case 4 string
- **case 5:**  (String) - case 5 string
- **case 6:**  (String) - case 6 string
- **case 7:**  (String) - case 7 string
- **case 8:**  (String) - case 8 string
- **Search Mode:** 0 (Bool) - Search for a first string that matches the case st

**Port Summary:**
- **Left:**  10 ports (1 events, 9 data)
- **Right:** 9 ports (9 events, 0 data)
