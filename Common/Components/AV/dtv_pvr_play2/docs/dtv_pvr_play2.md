DtvPvrPlay2
Plays local files & streams remote URLs, presents to the video viewport. 
 Ports
Menu: 
            AV
             > Players > URLPlayer
Type: IO

            ┌─────────────────────────┐
 set input►─┤                         ├►─---------- 
  path/URL──┤                         ├──meta (S)
      play►─┤                         ├►─---------- 
   set pos►─┤                         ├►─------------ 
       (%)──┤                         ├──---info (I)
  set time►─┤                         ├►─------------ 
       (s)──┤                         ├──---info (I)
     pause►─┤                         ├►─---------- 
   reverse►─┤                         ├►─---------- 
    faster►─┤                         ├►─---------- 
    slower►─┤                         ├►─---------- 
      stop►─┤                         ├►─---------- 
            │                         ├►─ended 
   getinfo►─┤                         ├►─---------- 
            │                         ├──length info (I)
            │                         ├──speed info (I)
 setWindow►─┤                         │
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
         w──┤                         ├──w (I)
         h──┤                         ├──h (I)
  to front►─┤                         ├►─---------- 
   to back►─┤                         ├►─---------- 
    setVol►─┤                         ├►─---------- 
       (%)──┤                         ├►─---------- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Widget Tag:** widget - widget tag
- **Widget-Group:** default - widget group

**Port Summary:**
- **Left:**  22 ports (14 events, 8 data)
- **Right:** 24 ports (15 events, 9 data)
