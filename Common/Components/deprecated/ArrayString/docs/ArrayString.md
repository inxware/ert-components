ArrayString
Menu: 
            Deprecated
             > Array > string
Type: Data_Processor

            ┌─────────────────────────┐
        wr►─┤                         ├►─wr 
       chw──┤                         │
       sIn──┤                         │
        rd►─┤                         ├►─rd 
       chr──┤                         ├──sOut (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **size:** 1024 (2 to 1024) - Memory allocated for array

**Port Summary:**
- **Left:**  5 ports (2 events, 3 data)
- **Right:** 3 ports (2 events, 1 data)
