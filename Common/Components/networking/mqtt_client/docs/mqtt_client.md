mqtt_client
mqtt_client
Menu: 
            Networking
             > MQTT > MQTT Client
Type: IO | Width: 105 | Height: 120

            ┌─────────────────────────┐
   connect►─┤                         ├►─-- 
      host──┤                         ├►─err 
      port──┤                         ├──err msg (S)
  clientid──┤                         │
            │       MQTT Client       │
  username──┤                         │
  password──┤                         │
       tls──┤                         │
disconnect►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (9):**
- **host:**  (String) - url to mqtt broker
- **port:** 0 (0 to 65535) - mqtt broker port
- **clientId:**  (String) - mqtt client id
- **username:**  (String) - username
- **password:**  (String) - password
- **tls:** 0 (Bool) - enable tls
- **client cert:**  (String) - Client TLS certiicate (PEM format)
- **client key:**  (String) - Client TLS key (base64)
- **server cert:**  (String) - Server root authority certificate.

**Port Summary:**
- **Left:**  8 ports (2 events, 6 data)
- **Right:** 4 ports (3 events, 1 data)
