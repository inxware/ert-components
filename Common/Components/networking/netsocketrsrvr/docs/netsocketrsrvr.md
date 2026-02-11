netsocketsrvr
Listens on specified port
Menu: 
            Networking
             > Sockets > Socket Server
Type: IO | Height: 180

            ┌─────────────────────────┐
      open►─┤                         ├►─--- 
        IF──┤                         ├►─error 
      port──┤                         │
     close►─┤                         ├►─--- 
            │                         ├►─error 
            │                         ├►─received 
            │       Sck Server        │
            │                         ├──data (S)
            │                         ├──size (I)
            │                         ├──source (S)
      send►─┤                         ├►─--- 
      data──┤                         ├►─error 
    inSize──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **Interface:** 0 (String) - Interface (IF)
- **Port:** 8000 (0 to 100000) - Listening Port
- **udp:** 0 (0 to 1) - tcp/udp mode

**Port Summary:**
- **Left:**  7 ports (3 events, 4 data)
- **Right:** 10 ports (7 events, 3 data)
