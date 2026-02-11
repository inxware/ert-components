mqtt_publish
mqtt_publish
Menu: 
            Networking
             > MQTT > MQTT Publish
Type: IO | Width: 105 | Height: 80

            ┌─────────────────────────┐
   publish►─┤                         ├►─-- 
     topic──┤                         │
            │        MQTT Pub         │
   payload──┤                         │
       qos──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **topic:** default (String) - topic
- **qos:** 0 (0 to 255) - quality of service

**Port Summary:**
- **Left:**  4 ports (1 events, 3 data)
- **Right:** 1 ports (1 events, 0 data)
