indexed_demux_int
Demultiplex the integer according to the input index
Menu: 
            Data Utilities
             > Indexed Demux > int
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─err 
     index──┤                         │
        in──┤                         ├►─1 
            │                         ├──1 (I)
            │                         ├►─2 
            │                         ├──2 (I)
            │                         ├►─3 
            │                         ├──3 (I)
            │                         ├►─4 
            │                         ├──4 (I)
            │          Demux          │
            │                         ├►─5 
            │                         ├──5 (I)
            │                         ├►─6 
            │                         ├──6 (I)
            │                         ├►─7 
            │                         ├──7 (I)
            │                         ├►─8 
            │                         ├──8 (I)
            │                         ├►─ovf 
            │                         ├──off (I)
            │                         ├──in (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **default:** 0 (-2147483648 to 2147483647) - Input default value [Display: 20,30]

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 20 ports (10 events, 10 data)
