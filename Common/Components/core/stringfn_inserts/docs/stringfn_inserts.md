InsertString
This function inserts a string s2 into string s1 at the specified position.
Menu: 
            Data Utilities
             > String Functions > insert
Type: Data_Processor | Width: 30 | Height: 50

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        s1──┤                         ├── (S)
            │         Insert          │
        s2──┤                         │
          ──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **index:** 0 (0 to 1024) - 0-based index where to insert string
- **string to insert:**  (String) - String to inser

**Port Summary:**
- **Left:**  4 ports (1 events, 3 data)
- **Right:** 2 ports (1 events, 1 data)
