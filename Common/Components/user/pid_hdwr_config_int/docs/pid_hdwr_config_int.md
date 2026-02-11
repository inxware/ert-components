pid_hdwr_config_int
Configure PID controller hardware Integer
Menu: 
            Control Systems
             > Hardware Config (int)
Type: Data | Height: 60

            ┌─────────────────────────┐
    config►─┤                         ├►─-- 
            │       PID HW Conf       │
        id──┤                         ├►─err 
     value──┤                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **id:** -1 (-1 to 10000) - config id
- **value:** 0 (-2147483647 to 2147483647) - config value

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 3 ports (2 events, 1 data)
