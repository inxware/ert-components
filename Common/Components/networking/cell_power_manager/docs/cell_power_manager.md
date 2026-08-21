cell_power_manager
Requests the cellular power-saving modes and reports what the network actually g
Menu: 
            Networking
             > Cellular > Power Manager
Type: IO | Width: 95 | Height: 158

            ┌─────────────────────────┐
     Apply►─┤                         │
            │                         ├►─ADone 
            │                         ├►─AFail 
      Read►─┤                         │
            │                         ├►─RDone 
            │                         ├──psmGranted (B)
            │       Cell Power        │
            │                         ├──grantedTau (I)
            │                         ├──grantedActive (I)
            │                         ├──edrxGranted (B)
            │                         ├──grantedEdrxMs (I)
            │                         ├──grantedPtwMs (I)
            │                         ├──raiOk (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (7):**
- **psmEnable:** 1 (Bool) - Request Power Saving Mode. The modem sleeps deeply
- **tauSeconds:** 3600 (0 to 35712000) - Requested periodic tracking-area-update interval i
- **activeSeconds:** 60 (0 to 11160) - Requested active time in seconds - how long the de
- **edrxEnable:** 0 (Bool) - Request extended Discontinuous Reception, which le
- **edrxCycleMs:** 0 (0 to 2621440) - Requested eDRX cycle in milliseconds. 0 lets the m
- **asRai:** 0 (Bool) - Request Access Stratum release assistance, which t
- **cpRai:** 0 (Bool) - Request Control Plane release assistance.

**Port Summary:**
- **Left:**  2 ports (2 events, 0 data)
- **Right:** 10 ports (3 events, 7 data)

**Ports by Function:**

*do_apply:*
  - **apply** (Start Event)
  - **apply_ok** (Finish Event)
  - **apply_fail** (Finish Event)

*do_read:*
  - **read** (Start Event)
  - **psm_granted** (B) - Output
  - **granted_tau** (I) - Output
  - **granted_active** (I) - Output
  - **edrx_granted** (B) - Output
  - **granted_edrx_ms** (I) - Output
  - **granted_ptw_ms** (I) - Output
  - **rai_ok** (B) - Output
  - **read_ok** (Finish Event)
