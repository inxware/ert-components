AvailableLanguages
This is the NLS language choice function block.
Menu: 
            User Interface
             > National Language Support > Report Languages
Type: Data_Processor | Width: 65 | Height: 85

            ┌─────────────────────────┐
    lookup►─┤                         ├►─ 
    getnum►─┤                         ├►─ 
            │          Langs          │
       idx──┤                         ├──lang (S)
            │                         ├──num (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 4 ports (2 events, 2 data)
