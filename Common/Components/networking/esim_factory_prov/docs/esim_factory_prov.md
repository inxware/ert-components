esim_factory_prov
Installs a Bound Profile Package that the factory's equipment has already obtain
Menu: 
            Networking
             > Cellular > SIM Provisioning > Factory Install
Type: IO | Width: 130 | Height: 243

            ┌─────────────────────────┐
   ReadEID►─┤                         ├►─RDone 
            │                         ├►─RFail 
            │                         ├──EID (S)
            │                         ├──errCode (I)
            │                         ├──maxSegLen (I)
     Begin►─┤                         ├►─BDone 
            │                         ├►─BFail 
            │                         ├──errCode (I)
            │     Factory Install     │
   PushSeg►─┤                         ├►─PDone 
   segData──┤                         ├►─PFail 
    segLen──┤                         ├──errCode (I)
    Commit►─┤                         ├►─CDone 
            │                         ├►─CFail 
            │                         ├──instICCID (S)
            │                         ├──errCode (I)
     Abort►─┤                         ├►─ADone 
            │                         ├──errCode (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  7 ports (5 events, 2 data)
- **Right:** 17 ports (9 events, 8 data)

**Ports by Function:**

*do_read_eid:*
  - **read_eid** (Start Event)
  - **eid** (S) - Output
  - **read_err_code** (I) - Output
  - **max_seg_len** (I) - Output
  - **read_ok** (Finish Event)
  - **read_fail** (Finish Event)

*do_begin_install:*
  - **begin_install** (Start Event)
  - **begin_err_code** (I) - Output
  - **begin_ok** (Finish Event)
  - **begin_fail** (Finish Event)

*do_push_segment:*
  - **push_segment** (Start Event)
  - **seg_data** (S) - Input
  - **seg_len** (I) - Input
  - **push_err_code** (I) - Output
  - **push_ok** (Finish Event)
  - **push_fail** (Finish Event)

*do_commit_install:*
  - **commit_install** (Start Event)
  - **inst_iccid** (S) - Output
  - **commit_err_code** (I) - Output
  - **commit_ok** (Finish Event)
  - **commit_fail** (Finish Event)

*do_abort_install:*
  - **abort_install** (Start Event)
  - **abort_err_code** (I) - Output
  - **abort_ok** (Finish Event)
