LIFO_Bool
This is a boolean Last-In-Last-Out buffer.
Menu: 
            Data Utilities
             > Buffers > LIFO > bool
Type: Data_Processor | Width: 65 | Height: 100

            ┌─────────────────────────┐
       pop►─┤                         ├►─pop 
            │                         ├►─err 
      push►─┤                         ├►─push 
            │          LIFO           │
            │                         ├►─err 
     flush►─┤                         ├►─flush 
      data──┤                         ├──data (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Max. length:** 5 (0 to 1000000) - maximum buffer length

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 6 ports (5 events, 1 data)
