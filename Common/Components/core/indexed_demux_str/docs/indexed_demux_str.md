indexed_demux_str
Demultiplex the string according to the input index
Menu: 
            Data Utilities
             > Indexed Demux > string
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─err 
     index──┤                         │
        in──┤                         ├►─1 
            │                         ├──1 (S)
            │                         ├►─2 
            │                         ├──2 (S)
            │                         ├►─3 
            │                         ├──3 (S)
            │                         ├►─4 
            │                         ├──4 (S)
            │          Demux          │
            │                         ├►─5 
            │                         ├──5 (S)
            │                         ├►─6 
            │                         ├──6 (S)
            │                         ├►─7 
            │                         ├──7 (S)
            │                         ├►─8 
            │                         ├──8 (S)
            │                         ├►─ovf 
            │                         ├──off (I)
            │                         ├──in (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **default:**  (String) - Input default value

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 20 ports (10 events, 10 data)
