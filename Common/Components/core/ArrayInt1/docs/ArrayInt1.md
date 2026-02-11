ArrayInt1
Arrays of integer variables can be stored in the component
  The index must fall
Menu: 
            Data Utilities
             > Buffers > Array > int
Type: Data_Processor | Width: 65 | Height: 105

            ┌─────────────────────────┐
     write►─┤                         ├►─ 
     index──┤                         ├►─err 
      data──┤                         │
            │          Array          │
      read►─┤                         ├►─ 
     index──┤                         ├►─err 
            │                         ├──data (I)
     clear►─┤                         ├►─clear 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **size:** 1024 (2 to 1024) - Memory allocated for array
- **default value:** 0 (0 to 0) - default value for uninitialised values

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 6 ports (5 events, 1 data)
