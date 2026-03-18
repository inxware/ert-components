lorawan
This function block describes the LoRaWAN communication with external LoRaWAN mo
Menu: 
            Networking
             > LoraWAN
Type: IO | Height: 660

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
       class──┤                         │
              │                         ├►─received 
              │                         ├──message (S)
              │                         ├──fport (I)
              │                         ├──lnkStat (S)
              │                         ├──rssi (I)
        send►─┤                         ├►─-- 
     confirm──┤                         ├►─busy 
       fport──┤                         ├►─ok 
     payload──┤                         ├►─failed 
              │                         ├──errno (I)
       reset►─┤                         ├►─-- 
              │                         ├►─busy 
              │                         ├►─done 
      status►─┤                         ├►─-- 
              │                         ├►─busy 
              │         LoRaWAN         │
              │                         ├►─ok 
              │                         ├──status (S)
              │                         ├──devEui (S)
              │                         ├──linkMargin (I)
         set►─┤                         ├──gateways (I)
          DR──┤                         ├──txPower (I)
              │                         ├►─ok 
   getLength►─┤                         ├►─-- 
              │                         ├►─busy 
              │                         ├►─ok 
              │                         ├──len (I)
     disable►─┤                         ├►─-- 
              │                         ├►─busy 
              │                         ├►─disabled 
   set_class►─┤                         ├►─-- 
       class──┤                         ├►─busy 
              │                         ├►─ok 
set_tx_power►─┤                         ├►─-- 
       txPwr──┤                         ├►─busy 
              │                         ├►─ok 
  link_check►─┤                         ├►─-- 
              │                         ├►─busy 
              │                         ├►─done 
              │                         ├──linkMargin (I)
              │                         ├──gateways (I)
              └─────────────────────────┘

              Internal Events:
                • ccbi
                • smcbi
                • rcbi
                • gsdcbi
                • sfrcbi
                • gplcbi
                • dcbi
                • ormsgi
                • sccbi
                • stpcbi
                • lccbi

Legend: ── Data | ►─ Event

**Parameters (19):**
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
- **Class:** 0 (0 to 2) - Device class. 0=A (default), 1=B, 2=C. Class A is 
- **SubBand:** 0 (0 to 8) - Sub-band selection for US915, AU915 and CN470 regi
- **RXWIN2_DR:** 0 (0 to 15) - Data rate for the second receive window (RX2). Pai
- **TxPower:** 0 (0 to 7) - Device TX power index. 0 = maximum EIRP for the re
- **ComPort:** 3 (0 to 255) - COM port number for the LoRaWAN modem serial conne

**Port Summary:**
- **Left:**  25 ports (10 events, 15 data)
- **Right:** 51 ports (33 events, 18 data)

**Ports by Function:**

*connect:*
  - **connect** (Start Event)
  - **AppKey** (S) - Input
  - **AppEui** (S) - Input
  - **AppSKey** (S) - Input
  - **NwkSKey** (S) - Input
  - **adr_in** (B) - Input
  - **DevAddr_ABP** (S) - Input
  - **miscOpt** (S) - Input
  - **region** (I) - Input
  - **class_in** (I) - Input
  - **connectDone** (Finish Event)
  - **connectFail** (Finish Event)

*set_datarate:*
  - **set_datarate** (Start Event)
  - **dr_in** (I) - Input
  - **set_datarate_sent** (Finish Event)
  - **set_datarate_busy** (Finish Event)

*get_payload_length:*
  - **get_pl_len** (Start Event)
  - **get_pl_len_sent** (Finish Event)
  - **get_pl_len_busy** (Finish Event)

*disable:*
  - **disable** (Start Event)
  - **disable_sent** (Finish Event)
  - **disable_busy** (Finish Event)

*set_datarate_cb:*
  - **sfrcbi** (Internal Event - callable from ISR/thread)
  - **set_datarate_ok** (Finish Event)

*get_payload_length_cb:*
  - **gplcbi** (Internal Event - callable from ISR/thread)
  - **pl_len** (I) - Output
  - **pl_len_got** (Finish Event)

*disable_cb:*
  - **dcbi** (Internal Event - callable from ISR/thread)
  - **disabled** (Finish Event)

*set_class:*
  - **set_class** (Start Event)
  - **class_in_sc** (I) - Input
  - **set_class_sent** (Finish Event)
  - **set_class_busy** (Finish Event)

*set_class_cb:*
  - **sccbi** (Internal Event - callable from ISR/thread)
  - **set_class_ok** (Finish Event)

*set_tx_power:*
  - **set_tx_power** (Start Event)
  - **tx_power_in** (I) - Input
  - **set_tx_power_sent** (Finish Event)
  - **set_tx_power_busy** (Finish Event)

*set_tx_power_cb:*
  - **stpcbi** (Internal Event - callable from ISR/thread)
  - **set_tx_power_ok** (Finish Event)

*on_receive_msg:*
  - **ormsgi** (Internal Event - callable from ISR/thread)
  - **fport_rx** (I) - Output
  - **link_status** (S) - Output
  - **recv_msg** (S) - Output
  - **rssi** (I) - Output
  - **snr** (F) - Output
  - **rxwin** (I) - Output
  - **received** (Finish Event)

*link_check:*
  - **link_check** (Start Event)
  - **link_check_sent** (Finish Event)
  - **link_check_busy** (Finish Event)

*link_check_cb:*
  - **lccbi** (Internal Event - callable from ISR/thread)
  - **link_margin_out** (I) - Output
  - **gateway_count_out** (I) - Output
  - **link_check_done** (Finish Event)

*send_msg:*
  - **send_msg** (Start Event)
  - **payload** (S) - Input
  - **frame_port** (I) - Input
  - **msg_confirmed** (B) - Input
  - **send_done** (Finish Event)
  - **send_fail** (Finish Event)

*reset:*
  - **reset** (Start Event)
  - **reset_sent** (Finish Event)
  - **reset_send_failed** (Finish Event)

*get_statusData:*
  - **get_data** (Start Event)
  - **get_sd_failed** (Finish Event)
  - **data_got** (Finish Event)

*connect_cb:*
  - **ccbi** (Internal Event - callable from ISR/thread)
  - **DevAddr** (S) - Output
  - **connect_errno** (I) - Output
  - **connect_cb_ok** (Finish Event)
  - **connect_fail** (Finish Event)

*send_msg_cb:*
  - **smcbi** (Internal Event - callable from ISR/thread)
  - **send_errCode** (I) - Output
  - **msg_sent** (Finish Event)
  - **send_failed** (Finish Event)

*reset_cb:*
  - **rcbi** (Internal Event - callable from ISR/thread)
  - **reset_done** (Finish Event)

*get_statusData_cb:*
  - **gsdcbi** (Internal Event - callable from ISR/thread)
  - **status** (S) - Output
  - **DevEui** (S) - Output
  - **linkMargin** (I) - Output
  - **gateways** (I) - Output
  - **txPower** (I) - Output
  - **currentDR** (I) - Output
  - **get_sd_cb_ok** (Finish Event)
