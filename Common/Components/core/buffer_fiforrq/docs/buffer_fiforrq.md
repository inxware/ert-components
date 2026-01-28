FIFO_Real
This is a real fifo buffer.
Menu: 
            Data Utilities
             > Buffers > FIFO > real
Type: Data_Processor | Width: 65 | Height: 100

            ┌─────────────────────────┐
       pop►─┤                         ├►─pop 
            │                         ├►─err 
      push►─┤                         ├►─push 
            │          FIFO           │
            │                         ├►─err 
     flush►─┤                         ├►─flush 
      data──┤                         ├──data (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Max. length:** 5 (0 to 1000000) - maximum buffer length

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 6 ports (5 events, 1 data)
