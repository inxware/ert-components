sgp32_download
Downloads a new subscription profile onto an eSIM (eUICC) using GSMA SGP.32 remo
Menu: 
            Networking
             > Cellular > SIM Provisioning > SGP.32 Download
Type: IO | Width: 105 | Height: 133

                ┌─────────────────────────┐
        SetEIM►─┤                         │
    eimAddress──┤                         │
                │                         ├►─EDone 
                │                         ├►─EFail 
      Download►─┤                         │
                │     SGP.32 Download     │
activationCode──┤                         │
                │                         ├►─DDone 
                │                         ├►─DFail 
                │                         ├──errCode (I)
                │                         ├──dlICCID (S)
                └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 6 ports (4 events, 2 data)

**Ports by Function:**

*do_set_eim:*
  - **set_eim** (Start Event)
  - **eim_address** (S) - Input
  - **set_eim_ok** (Finish Event)
  - **set_eim_fail** (Finish Event)

*do_download:*
  - **download** (Start Event)
  - **activation_code** (S) - Input
  - **dl_err_code** (I) - Output
  - **dl_iccid** (S) - Output
  - **dl_ok** (Finish Event)
  - **dl_fail** (Finish Event)
