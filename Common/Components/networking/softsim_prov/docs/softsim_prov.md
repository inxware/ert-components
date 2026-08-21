softsim_prov
Writes a SoftSIM subscription profile to the device.

SoftSIM is a software-only
Menu: 
            Networking
             > Cellular > SIM Provisioning > SoftSIM
Type: IO | Width: 95 | Height: 118

            ┌─────────────────────────┐
 Provision►─┤                         │
   Profile──┤                         │
            │                         ├►─Done 
            │                         ├►─Fail 
            │      SoftSIM Prov       │
            │                         ├──errCode (I)
     Check►─┤                         │
            │                         ├►─CDone 
            │                         ├──provisioned (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **provider:** 0 (0 to 1) - Profile format. 0 generic, 1 Onomondo.

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 5 ports (3 events, 2 data)

**Ports by Function:**

*do_provision:*
  - **provision** (Start Event)
  - **blob** (S) - Input
  - **err_code** (I) - Output
  - **prov_ok** (Finish Event)
  - **prov_fail** (Finish Event)

*do_check:*
  - **check** (Start Event)
  - **provisioned** (B) - Output
  - **check_ok** (Finish Event)
