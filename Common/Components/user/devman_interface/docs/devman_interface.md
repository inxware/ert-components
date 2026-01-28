DevmanInterface
This component reports data to the Portal using Devman Monitor and gets the Devm
Menu: 
            System
             > Remote > Devman Interface
Type: Data_Processor | Width: 110 | Height: 190

             ┌─────────────────────────┐
       send►─┤                         ├►─---- 
 misc. info──┤                         │
    Set URL►─┤                         ├►─--- 
        URL──┤                         │
    get URL►─┤                         ├►─---- 
             │                         ├──curr. URL (S)
             │    Devman Interface     │
    get All►─┤                         ├►─--- 
             │                         ├──JSON (S)
listen Misc►─┤                         ├►─new 
             │                         ├──new JSON (S)
 net status►─┤                         ├►─--- 
             │                         ├──connected (B)
             │                         ├──error (I)
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **App Name:**  (String) - identifies data service channel [Display: 0,0]
- **Process Name:**  (String) - Optional name of another process to monitor

**Port Summary:**
- **Left:**  8 ports (6 events, 2 data)
- **Right:** 11 ports (6 events, 5 data)
