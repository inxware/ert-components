network_config
Configures network interface
Menu: 
            Networking
             > Network Config
Type: IO

            ┌─────────────────────────┐
    status►─┤                         ├►─-- 
            │                         ├──connected (B)
       get►─┤                         ├►─-- 
            │                         ├──err (I)
            │                         ├──mode (I)
            │                         ├──address (S)
            │                         ├──gateway (S)
            │       Net Config        │
            │                         ├──mask (S)
            │                         ├──dns (S)
       set►─┤                         ├►─-- 
      mode──┤                         ├──err (I)
   address──┤                         │
   gateway──┤                         │
      mask──┤                         │
       dns──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  8 ports (3 events, 5 data)
- **Right:** 11 ports (3 events, 8 data)
