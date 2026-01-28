permanent_storage_bool
permanent_storage_bool
Menu: 
            File System
             > NV Storage > bool
Type: IO | Width: 85 | Height: 80

            ┌─────────────────────────┐
     Write►─┤                         ├►─-- 
      Read►─┤                         ├►─-- 
            │        NV Store         │
        id──┤                         ├──value (B)
     value──┤                         ├►─read error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 4 ports (3 events, 1 data)
