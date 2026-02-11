indexed_demux_real
Demultiplex the floating point number according to the input index
Menu: 
            Data Utilities
             > Indexed Demux > real
Type: Data

            ┌─────────────────────────┐
        --►─┤                         ├►─err 
     index──┤                         │
        in──┤                         ├►─1 
            │                         ├──1 (F)
            │                         ├►─2 
            │                         ├──2 (F)
            │                         ├►─3 
            │                         ├──3 (F)
            │                         ├►─4 
            │                         ├──4 (F)
            │          Demux          │
            │                         ├►─5 
            │                         ├──5 (F)
            │                         ├►─6 
            │                         ├──6 (F)
            │                         ├►─7 
            │                         ├──7 (F)
            │                         ├►─8 
            │                         ├──8 (F)
            │                         ├►─ovf 
            │                         ├──off (I)
            │                         ├──in (F)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **default:** 0 (Real) - Input default value [Display: 20,30]

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 20 ports (10 events, 10 data)
