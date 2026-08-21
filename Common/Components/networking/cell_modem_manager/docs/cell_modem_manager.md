cell_modem_manager
Brings the cellular link up and down and reports both connection state and modem
Menu: 
            Networking
             > Cellular > Modem Manager
Type: IO | Width: 95 | Height: 363

            ┌─────────────────────────┐
   Connect►─┤                         │
            │                         ├►─CDone 
Disconnect►─┤                         │
            │                         ├►─DDone 
        --►─┤                         │
            │                         ├►─Connected 
            │                         ├►─Failed 
            │                         ├►─Disconnected 
            │                         ├──state (I)
            │                         ├──regStatus (I)
            │                         ├──ipAddr (S)
            │                         ├──isRoaming (B)
            │                         ├──activeRAT (I)
            │                         ├──failReason (I)
       Set►─┤                         │
            │       Cell Modem        │
       APN──┤                         │
   APNUser──┤                         │
   APNPass──┤                         │
       RAT──┤                         │
            │                         ├►─SDone 
  ReadInfo►─┤                         │
            │                         ├►─IDone 
            │                         ├──IMEI (S)
            │                         ├──Operator (S)
            │                         ├──MCC (I)
            │                         ├──MNC (I)
            │                         ├──CellID (I)
            │                         ├──TAC (I)
            │                         ├──Band (I)
            │                         ├──ModemFW (S)
            │                         ├──errCode (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (6):**
- **onStartup:** 1 (Bool) - Connect on startup, before the app is loaded.
- **rat:** 1 (0 to 3) - Radio access technology. 0 leaves the modem defaul
- **ratPreference:** 0 (0 to 4) - Which technology the modem should prefer when both
- **tryReconnect:** 1 (Bool) - Re-attach automatically after the link is lost, us
- **backoffInitial:** 60 (10 to 3600) - Delay in seconds before the first re-attach attemp
- **backoffMax:** 3600 (60 to 86400) - Longest delay in seconds between re-attach attempt

**Port Summary:**
- **Left:**  9 ports (5 events, 4 data)
- **Right:** 22 ports (7 events, 15 data)

**Ports by Function:**

*do_connect:*
  - **connect** (Start Event)
  - **do_connect_OK** (Finish Event)

*do_disconnect:*
  - **disconnect** (Start Event)
  - **do_disconnect_OK** (Finish Event)

*internal_monitor:*
  - **do_int** (Internal Event - callable from ISR/thread)
  - **state** (I) - Output
  - **reg_status** (I) - Output
  - **ip_address** (S) - Output
  - **is_roaming** (B) - Output
  - **active_rat** (I) - Output
  - **fail_reason** (I) - Output
  - **connected** (Finish Event)
  - **connect_failed** (Finish Event)
  - **disconnected** (Finish Event)

*do_set:*
  - **set** (Start Event)
  - **apn** (S) - Input
  - **apn_user** (S) - Input
  - **apn_pass** (S) - Input
  - **set_rat** (I) - Input
  - **set_ok** (Finish Event)

*do_read_info:*
  - **read_info** (Start Event)
  - **imei** (S) - Output
  - **operator_name** (S) - Output
  - **mcc** (I) - Output
  - **mnc** (I) - Output
  - **cell_id** (I) - Output
  - **tac** (I) - Output
  - **band** (I) - Output
  - **modem_fw** (S) - Output
  - **err_code** (I) - Output
  - **read_info_ok** (Finish Event)
