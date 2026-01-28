SubString
This function outputs a substring specified by a start index and length.
Menu: 
            Data Utilities
             > String Functions > substring
Type: Data_Processor | Width: 40 | Height: 60

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
          ──┤                         ├── (S)
            │        Substring        │
     start──┤                         │
    length──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **start:** 0 (0 to 1024) - 0-based index to start string extract
- **length:** 1 (0 to 1024) - Number of characters to extract

**Port Summary:**
- **Left:**  4 ports (1 events, 3 data)
- **Right:** 2 ports (1 events, 1 data)
