lorawan
This function block describes the LoRaWAN communication with external LoRaWAN mo
Menu: 
            Networking
             > LoraWAN
Type: IO | Height: 480

            ┌─────────────────────────┐
      join►─┤                         ├►─-- 
    region──┤                         ├►─busy 
       ADR──┤                         ├►─ok 
    appKey──┤                         ├──devAddr (S)
    appEui──┤                         ├►─failed 
   nwkSKey──┤                         ├──errno (I)
   appSKey──┤                         │
   devAddr──┤                         │
   miscOpt──┤                         │
            │                         ├►─received 
            │                         ├──message (S)
            │                         ├──fport (I)
            │                         ├──lnkStat (S)
      send►─┤                         ├►─-- 
   confirm──┤                         ├►─busy 
     fport──┤                         ├►─ok 
   payload──┤                         ├►─failed 
            │                         ├──errno (I)
            │         LoRaWAN         │
     reset►─┤                         ├►─-- 
            │                         ├►─busy 
            │                         ├►─done 
    status►─┤                         ├►─-- 
            │                         ├►─busy 
            │                         ├►─ok 
            │                         ├──status (S)
            │                         ├──devEui (S)
       set►─┤                         ├►─-- 
        DR──┤                         ├►─busy 
            │                         ├►─ok 
 getLength►─┤                         ├►─-- 
            │                         ├►─busy 
            │                         ├►─ok 
            │                         ├──len (I)
   disable►─┤                         ├►─-- 
            │                         ├►─busy 
            │                         ├►─disabled 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (14):**
- **Target:** 0 (0 to 0) - The LoRaWAN target module. Default to 0 (WIO-E5). 
- **Mode:** 0 (Bool) - The connection mode of LoRaWAN. 0 for OTAA, 1 for 
- **Region:** 0 (0 to 10) - Region Parameters of LoRaWAN communications. Defau
- **Repeat:** 1 (1 to 15) - Unconfirmed message repeat time. From 1 to 15.
- **Retry:** 3 (0 to 254) - Confirmed message retry times. From 0 to 254.
- **AutoJoin:** 300 (0 to 86400) - Fixed period in seconds up to 24 hours to join the
- **ADR:** 0 (Bool) - Adaptive Data Rate. With this on, the DR setting w
- **DR:** 0 (0 to 15) - Data Rate. Varies from 0 to 15.
- **RXWIN2:** 869.525 (Real) - Second RX window frequency in MHz
- **appKey:** 00000000000000000000000000000000 (String) - Default Application key for OTAA join mode
- **appEui:** 0000000000000000 (String) - Application Unique ID for OTAA join mode
- **nwkSKey:** 00000000000000000000000000000000 (String) - Network Session Key for ABP join mode
- **appSKey:** 00000000000000000000000000000000 (String) - Application Session Key for ABP join mode
- **devAddrABP:** 00000000 (String) - Device Address for ABP join mode

**Port Summary:**
- **Left:**  19 ports (7 events, 12 data)
- **Right:** 33 ports (24 events, 9 data)
