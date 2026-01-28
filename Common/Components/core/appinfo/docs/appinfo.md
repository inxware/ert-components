AppInfo
Returns the app automatic version number, data stamp and given name,    
Menu: 
            System
             > Local > App Info
Type: Data_Processor | Width: 100 | Height: 105

            ┌─────────────────────────┐
  get info►─┤                         ├►─ 
      name──┤                         ├── (S)
            │                         ├──version (S)
            │                         ├──build no (I)
            │        App Info         │
            │                         ├──date (S)
            │                         ├──min EHS (S)
            │                         ├──depndcs (S)
            │                         ├──proc grps (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Default App Name:**  (String) - Name of app to identify. Blank gives currently loa

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 8 ports (1 events, 7 data)
