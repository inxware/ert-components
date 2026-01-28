ArrayString1
Arrays of string variables can be stored in the component 
The index must fall b
Menu: 
            Data Utilities
             > Buffers > Array > string
Type: Data_Processor | Width: 65 | Height: 105

            ┌─────────────────────────┐
     write►─┤                         ├►─ 
     index──┤                         ├►─err 
      data──┤                         │
            │          Array          │
      read►─┤                         ├►─ 
     index──┤                         ├►─err 
            │                         ├──data (S)
     clear►─┤                         ├►─clear 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **size:** 1024 (2 to 1024) - Memory allocated for array
- **default value:**  (String) - default value for uninitialised values

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 6 ports (5 events, 1 data)
