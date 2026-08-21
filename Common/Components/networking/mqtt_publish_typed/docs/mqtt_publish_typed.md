mqtt_publish_typed
Publish an integer, float, or boolean to an MQTT topic as ASCII text: 21.75 is s
Menu: 
            Networking
             > MQTT > MQTT Publish Typed
Type: IO | Width: 105 | Height: 93

            ┌─────────────────────────┐
   publish►─┤                         ├►─-- 
   value_i──┤                         │
   value_f──┤                         │
            │      MQTT Pub Val       │
   value_b──┤                         │
     topic──┤                         │
       qos──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **topic:** default (String) - Default MQTT topic to publish to
- **qos:** 0 (0 to 2) - Quality of service (0, 1, or 2)

**Port Summary:**
- **Left:**  6 ports (1 events, 5 data)
- **Right:** 1 ports (1 events, 0 data)

**Ports by Function:**

*publish:*
  - **publish** (Start Event)
  - **value_i** (I) - Input
  - **value_f** (R) - Input
  - **value_b** (B) - Input
  - **topic** (S) - Input
  - **qos** (I) - Input
  - **done** (Finish Event)
