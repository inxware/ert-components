DtvPvrPlay1
Provides facilities to control PVR playback in terms of trick modes, play, pause
Menu: 
      Deprecated
       > DTV > Play
Type: IO

            ┌─────────────────────────┐
       set►─┤                         ├►─set 
       get►─┤                         ├►─get 
      play►─┤                         ├►─play 
     pause►─┤                         ├►─pause 
       rev►─┤                         ├►─rev 
    faster►─┤                         ├►─faster 
    slower►─┤                         ├►─slower 
      stop►─┤                         ├►─stop 
      name──┤                         ├►─end 
       pos──┤                         ├──pos (I)
            │                         ├──speed (I)
            │                         ├►─err 
            │                         ├──errNo (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  10 ports (8 events, 2 data)
- **Right:** 13 ports (10 events, 3 data)
