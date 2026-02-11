PlayManager
Manages parsing SMIL-based playlists and controlling media players and graphics.
Menu: 
			AV
			 > Media Management > SMIL Playlist Parser
Type: IO

               ┌─────────────────────────┐
 new playlist►─┤                         ├►─-------------- 
playlist path──┤                         ├──playlist path (S)
               │                         ├►─play now 
 started play►─┤                         ├►─play remote 
           ID──┤                         ├►─play stop 
               │                         ├──ID (I)
   last ended►─┤                         ├──media file (S)
           ID──┤                         ├──type (S)
               │                         ├──volume (I)
               │                         ├──fade (I)
               │                         ├──class (S)
               │                         ├──text (S)
               │                         ├──region (S)
     next URL►─┤                         ├►─------------ 
               │                         ├►─get 
               │                         ├──URL (S)
               │                         ├──ID (I)
          got►─┤                         ├►─------------ 
           ID──┤                         ├──local file (S)
          URL──┤                         ├──------------ (S)
   local file──┤                         ├──------------ (S)
               │                         ├►─tried all 
    set paths►─┤                         ├►─------------ 
       DL URL──┤                         ├──------------- (S)
     DL local──┤                         ├──-------------- (S)
   start mode──┤                         │
               │                         ├►─earliest 
               │                         ├──start time (S)
               │                         ├──stop time (S)
               │                         ├►─new layout 
               │                         ├──region JSON (S)
               │                         ├──AR JSON (S)
               │                         ├──file count (I)
               └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **Start Behaviour:** 0 (0 to 2) - 0:CLEAR_ALL, 1:KEEP_EARLIERTHANNEW, 2:KEEP_ALL
- **Start After Downloaded:** 1 (Bool) - set to true if play events should only active afte
- **Download Grace Period:** 60 (-1 to 36000) - number of seconds waited after a new play list bef

**Port Summary:**
- **Left:**  15 ports (6 events, 9 data)
- **Right:** 32 ports (11 events, 21 data)
