interface_manager
This manages the network interface. It can switch the interface between Ethernet
Menu: 
            Networking
             > Interface Manager
Type: IO | Height: 60

            ┌─────────────────────────┐
    config►─┤                         ├►─-- 
            │      NetIF Config       │
      wifi──┤                         ├──err (I)
  ethernet──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 2 ports (1 events, 1 data)
