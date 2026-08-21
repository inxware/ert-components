esim_profile_mgr
Manages the subscription profiles already present on an eSIM (eUICC): list them,
Menu: 
            Networking
             > Cellular > SIM Provisioning > eSIM Profiles
Type: IO | Width: 105 | Height: 313

            ┌─────────────────────────┐
 ListProfs►─┤                         │
            │                         ├►─LDone 
            │                         ├►─LFail 
            │                         ├──profCount (I)
            │                         ├──errCode (I)
GetProfile►─┤                         │
     index──┤                         │
            │                         ├►─GDone 
            │                         ├►─GFail 
            │                         ├──profICCID (S)
            │                         ├──profName (S)
            │                         ├──isEnabled (B)
            │                         ├──errCode (I)
    Enable►─┤                         │
            │      eSIM Profiles      │
     ICCID──┤                         │
            │                         ├►─NDone 
            │                         ├►─NFail 
            │                         ├──errCode (I)
   Disable►─┤                         │
     ICCID──┤                         │
            │                         ├►─BDone 
            │                         ├►─BFail 
            │                         ├──errCode (I)
    Delete►─┤                         │
     ICCID──┤                         │
            │                         ├►─XDone 
            │                         ├►─XFail 
            │                         ├──errCode (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  9 ports (5 events, 4 data)
- **Right:** 19 ports (10 events, 9 data)

**Ports by Function:**

*do_list_profiles:*
  - **list_profiles** (Start Event)
  - **profile_count** (I) - Output
  - **list_err_code** (I) - Output
  - **list_ok** (Finish Event)
  - **list_fail** (Finish Event)

*do_get_profile:*
  - **get_profile** (Start Event)
  - **profile_index** (I) - Input
  - **profile_iccid** (S) - Output
  - **profile_name** (S) - Output
  - **profile_enabled** (B) - Output
  - **get_err_code** (I) - Output
  - **get_ok** (Finish Event)
  - **get_fail** (Finish Event)

*do_enable_profile:*
  - **enable_profile** (Start Event)
  - **enable_iccid** (S) - Input
  - **enable_err_code** (I) - Output
  - **enable_ok** (Finish Event)
  - **enable_fail** (Finish Event)

*do_disable_profile:*
  - **disable_profile** (Start Event)
  - **disable_iccid** (S) - Input
  - **disable_err_code** (I) - Output
  - **disable_ok** (Finish Event)
  - **disable_fail** (Finish Event)

*do_delete_profile:*
  - **delete_profile** (Start Event)
  - **delete_iccid** (S) - Input
  - **delete_err_code** (I) - Output
  - **delete_ok** (Finish Event)
  - **delete_fail** (Finish Event)
