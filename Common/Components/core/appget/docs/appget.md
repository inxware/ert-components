AppGet
Provides the means to download, install & run applications from the application 
Menu: 
            System
             > Remote > Application Server
Type: Data_Processor | Width: 65 | Height: 200

            ┌─────────────────────────┐
  get list►─┤                         ├►─ 
  keywords──┤                         ├──list (S)
            │                         ├►─error 
   install►─┤                         ├►─ 
      name──┤                         ├►─error 
       run──┤                         │
   default──┤                         │
      mode──┤                         │
            │       App Server        │
      info►─┤                         ├►─ 
      name──┤                         ├──info (S)
            │                         ├──name (S)
            │                         ├──cmrcl name (S)
            │                         ├──version (S)
            │                         ├──descr (S)
            │                         ├►─error 
    server──┤                         ├──errors (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **Server:** devman.inx-systems.com (String) - Base URL of devman server hosting applications.
- **Install and Run:** 0 (Bool) - Run the application after it has been installed.
- **Install as Default:** 0 (Bool) - Set the application as the Default App when it is 
- **Install Mode:** 0 (0 to 2) - Install Mode, 0 install as standard App in canonic

**Port Summary:**
- **Left:**  10 ports (3 events, 7 data)
- **Right:** 13 ports (6 events, 7 data)
