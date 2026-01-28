FindString
This function determines if a string s2 exists in string s1.
Menu: 
            Data Utilities
             > String Functions > find
Type: Data_Processor | Width: 55 | Height: 50

            ┌─────────────────────────┐
          ►─┤                         ├►─ 
        s1──┤                         ├── (S)
            │        find str         │
        s2──┤                         ├── (B)
            │                         ├──index (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Reverse Search:** 0 (Bool) - Biases search for backward searching
- **Find String (-> s2):**  (String) - String to compare to unless overridden by port s2

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 4 ports (1 events, 3 data)
