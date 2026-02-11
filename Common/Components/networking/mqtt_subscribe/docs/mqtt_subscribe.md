mqtt_subscribe
mqtt_subscribe
Menu: 
            Networking
             > MQTT > MQTT Subscribe
Type: IO | Width: 105 | Height: 80

             ┌─────────────────────────┐
  subscribe►─┤                         ├►─-- 
      topic──┤                         ├►─received 
             │        MQTT Sub         │
        qos──┤                         ├──data (S)
             │                         ├──size (I)
unsubscribe►─┤                         ├►─-- 
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **topic:** default (String) - topic
- **qos:** 0 (0 to 255) - quality of service

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 5 ports (3 events, 2 data)
