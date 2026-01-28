ArrayReal1
Arrays of real value (floating point) variables can be stored in the component
 
Menu: 
            Data Utilities
             > Buffers > Array > real
Type: Data_Processor | Width: 65 | Height: 105

            ┌─────────────────────────┐
     write►─┤                         ├►─ 
     index──┤                         ├►─err 
      data──┤                         │
            │          Array          │
      read►─┤                         ├►─ 
     index──┤                         ├►─err 
            │                         ├──data (F)
     clear►─┤                         ├►─clear 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **size:** 1024 (2 to 1024) - Memory allocated for array
- **default value:** NaN (Real) - default value for uninitialised values

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 6 ports (5 events, 1 data)
