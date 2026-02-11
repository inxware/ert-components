application_info_getter
Will retrieve the list of locally installed apps along with details of them such
Menu: 
            System
             > Local > Installed Apps
Type: Data_Processor | Width: 100 | Height: 270

                      ┌─────────────────────────┐
            get list►─┤                         ├►─-----[list] 
            keywords──┤                         ├──list (S)
                      │                         ├──amount (I)
                      │                         ├►─error[list] 
            app info►─┤                         ├►─-----[info] 
      app name[info]──┤                         ├──info (S)
                      │                         ├──name (S)
                      │                         ├──commercial Name (S)
                      │                         ├──version (S)
                      │     Installed Apps      │
                      │                         ├──description (S)
                      │                         ├►─error[info] 
         get default►─┤                         ├►─-----[getDefault] 
                      │                         ├──app name[getDefault] (S)
                      │                         ├►─error[getDefault] 
         set default►─┤                         ├►─-----[setDefault] 
app name[setDefault]──┤                         ├►─error[setDefault] 
           uninstall►─┤                         ├►─-----[delete] 
    app name[delete]──┤                         ├►─error[delete] 
                      │                         ├──errors (S)
                      └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  9 ports (5 events, 4 data)
- **Right:** 19 ports (10 events, 9 data)
