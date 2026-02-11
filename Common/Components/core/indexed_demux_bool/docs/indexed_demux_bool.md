indexed_demux_bool
Demultiplex the boolean according to the input index
Menu: 
            Data Utilities
             > Indexed Demux > bool
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─err 
     index──┤                         │
        in──┤                         ├►─1 
            │                         ├──1 (B)
            │                         ├►─2 
            │                         ├──2 (B)
            │                         ├►─3 
            │                         ├──3 (B)
            │                         ├►─4 
            │                         ├──4 (B)
            │          Demux          │
            │                         ├►─5 
            │                         ├──5 (B)
            │                         ├►─6 
            │                         ├──6 (B)
            │                         ├►─7 
            │                         ├──7 (B)
            │                         ├►─8 
            │                         ├──8 (B)
            │                         ├►─ovf 
            │                         ├──off (I)
            │                         ├──in (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **default:** 0 (Bool) - Input default value [Display: 20,30]

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 20 ports (10 events, 10 data)
