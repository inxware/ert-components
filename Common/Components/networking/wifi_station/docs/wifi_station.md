wifi_station
The device works as a Wi-Fi station to connect to an access point.
This function
Menu: 
            Networking
             > Wi-Fi > Wi-Fi Station
Type: IO | Height: 255

            ┌─────────────────────────┐
   Connect►─┤                         ├►─CDone 
      SSID──┤                         ├►─OK 
      type──┤                         ├──ipAddr (S)
            │                         ├──RSSI (I)
   PSKPass──┤                         ├──AP_MAC (S)
   EntType──┤                         ├►─Fail 
       EAP──┤                         ├──errCode (I)
     TTLS2──┤                         │
            │      Wi-Fi Station      │
  needCert──┤                         │
serverCert──┤                         │
   tlsCert──┤                         │
    tlsKey──┤                         │
     eapID──┤                         │
   eapUser──┤                         │
   eapPass──┤                         ├►─DDone 
            │                         ├►─Disconnected 
Disconnect►─┤                         ├──Reason (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (16):**
- **SSID:**  (String) - Wi-Fi SSID [Display: 35,30]
- **onStartup:** 1 (Bool) - Connect the Wi-Fi on startup before the app is loa
- **type:** 0 (0 to 2) - Wi-Fi authentication type. 0 for Password authenti
- **tryReconnect:** 0 (Bool) - It will try to reconnect to the AP when it is disc
- **retry:** 1 (1 to 100) - The maximum number of retries to re-connect to the
- **reconnectPeriod:** 300 (1 to 9999999) - The period in seconds between the re-connection af
- **EntType:** 0 (0 to 2) - Enterprise Network authentication type.0 for WPA2-
- **EAP:** 1 (0 to 2) - The Extensible Authentication Protocol to be used 
- **TTLS2:** 1 (1 to 4) - EAP's TTLS second stage connection method. Only va
- **needServerCert:** 0 (Bool) - Weather to validate the servers' certificate using
- **serverCert:**  (String) - Path to the CA cert to vlidate the servers' certif
- **tlsCert:**  (String) - Path to TLS client certificate. Only valid if type
- **tlsKey:**  (String) - Path to TLS client key if EApP method is TLS. Only
- **eapID:**  (String) - ID for EAP authentication. Most of time, it is ema
- **eapUser:**  (String) - Username for EAP authentication. Only valid if typ
- **eapPass:**  (String) - Password for EAP authentication. Only valid if typ

**Port Summary:**
- **Left:**  15 ports (2 events, 13 data)
- **Right:** 10 ports (5 events, 5 data)
