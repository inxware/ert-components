mqtt_subscribe_typed
Subscribe to an MQTT topic and receive the payload parsed as integer, float, and
Menu: 
            Networking
             > MQTT > MQTT Subscribe Typed
Type: IO | Width: 105 | Height: 93

             ┌─────────────────────────┐
  subscribe►─┤                         ├►─-- 
      topic──┤                         ├►─received 
        qos──┤                         ├──value_i (I)
             │      MQTT Sub Val       │
             │                         ├──value_f (R)
             │                         ├──value_b (B)
unsubscribe►─┤                         ├►─-- 
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **topic:** default (String) - MQTT topic to subscribe to
- **qos:** 0 (0 to 2) - Quality of service (0, 1, or 2)

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 6 ports (3 events, 3 data)

**Ports by Function:**

*subscribe:*
  - **subscribe** (Start Event)
  - **topic** (S) - Input
  - **qos** (I) - Input
  - **value_i** (I) - Output
  - **value_f** (R) - Output
  - **value_b** (B) - Output
  - **done_subscribe** (Finish Event)
  - **received** (Finish Event)

*unsubscribe:*
  - **unsubscribe** (Start Event)
  - **done_unsubscribe** (Finish Event)
