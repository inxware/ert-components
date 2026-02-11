Exec
This function block runs the executable specified by the parameter with the argu
Menu: 
			System
			 > Host OS > Run Executable
Type: Data_Processor | Width: 65 | Height: 60

            ┌─────────────────────────┐
          ►─┤                         ├►─OK 
            │          Exec           │
      args──┤                         ├──out (S)
            │                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Executable:**  (String) - The executable to run

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 3 ports (2 events, 1 data)
