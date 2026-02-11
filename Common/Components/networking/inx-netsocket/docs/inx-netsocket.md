netSocket
provides tcpip and udp socket data comms.
Menu: 
            Networking
             > Sockets > Socket Client
Type: IO | Height: 180

            ┌─────────────────────────┐
      open►─┤                         ├►─---- 
       url──┤                         ├►─error 
      port──┤                         │
     close►─┤                         ├►─--- 
            │                         ├►─error 
            │       Sck Client        │
            │                         ├►─received 
            │                         ├──data (S)
            │                         ├──size (I)
      send►─┤                         ├►─--- 
      data──┤                         ├►─error 
      size──┤                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **URL:**  (String) - URL of the server socket. N/A for sockets
- **port:** 8000 (0 to 65535) - IP socket number to open
- **tcp/udp:** 0 (0 to 1) - 0=tcp, 1=udp

**Port Summary:**
- **Left:**  7 ports (3 events, 4 data)
- **Right:** 10 ports (7 events, 3 data)
