DevmanPlayer
This component provides contact with a devman server to enable the exchange of p
Menu: 
            AV
             > Media Management > Devman Content Manager
Type: IO

                ┌─────────────────────────┐
      Activate►─┤                         ├►─------------ 
   De-activate►─┤                         ├►─------------ 
       Set url►─┤                         ├►─------------ 
           URL──┤                         ├──URL (S)
        Config►─┤                         ├►─------------ 
   plylst path──┤                         ├►─new playlist 
                │                         ├──plylst URL (S)
   update rate──┤                         ├──X-playlist (S)
                │                         ├──media/ URL (S)
                │                         ├──media path (S)
 Rst media src►─┤                         ├►─Loaded Stored 
                │                         ├►─rem. list 
    track info►─┤                         ├──files (S)
                │                         ├►─rem. unused 
   playlist id──┤                         ├►─rem. all 
     plyr mode──┤                         ├►─Override 
   curr. media──┤                         ├──track url (S)
    start time──┤                         ├►─media parms 
media duration──┤                         ├──playerJSON (S)
  player state──┤                         ├──channels (I)
    PlayerJSON──┤                         ├►─set mute 
     misc info►─┤                         ├──on/off (B)
    media misc──┤                         ├►─status 
                │                         ├──state (I)
                │                         ├──Raw XML (S)
       getNext►─┤                         ├►─passThru 
      sendNext►─┤                         ├──passJSON (S)
      sendJSON──┤                         │
                └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (6):**
- **Devman Server URL:**  (String) - devman server url
- **Playlist Path:**  (String) - playlist path
- **Devman Ping Period:** 60 (1 to 3600) - devman ping period in seconds
- **Use Devman Core URL:** 1 (Bool) - Use the URL specified by devman.url
- **Start on Init:** 1 (Bool) - Start checking on init
- **Load playlist at start:** 1 (Bool) - Loads the last downloaded Media URL path etc.

**Port Summary:**
- **Left:**  21 ports (9 events, 12 data)
- **Right:** 27 ports (14 events, 13 data)
