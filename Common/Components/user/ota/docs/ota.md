OTA
This will write to the inactive OTA section with the incoming data. The data cou
Menu: 
            System
             > OTA Tools > OTA
Type: Data | Height: 190

            ┌─────────────────────────┐
     begin►─┤                         ├►─-- 
    SHA256──┤                         ├►─error 
            │                         ├──errno (I)
     write►─┤                         ├►─-- 
      data──┤                         ├►─error 
            │           OTA           │
      size──┤                         ├►─written 
    offset──┤                         ├►─error 
            │                         ├──errno (I)
       end►─┤                         ├►─-- 
            │                         ├►─error 
     abort►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **ota_partition_alt:** 0 (Bool) - Whether to use the alternative partition for OTA
- **partition_number:** 0 (0 to 9) - The number of the inactive partition to be updated

**Port Summary:**
- **Left:**  8 ports (4 events, 4 data)
- **Right:** 11 ports (9 events, 2 data)
