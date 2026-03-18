UART
The UART transceiving interface that is configured by the UART Config function b
Menu: 
            Peripherals
             > Serial > UART
Type: IO | Height: 185

            ┌─────────────────────────┐
     start►─┤                         ├►─OK 
      port──┤                         │
     close►─┤                         ├►─closed 
            │                         ├►─error 
            │                         ├►─received 
            │          UART           │
            │                         ├──data (S)
            │                         ├──size (I)
      send►─┤                         ├►─OK 
      data──┤                         ├►─error 
      size──┤                         ├──errno (I)
            └─────────────────────────┘

            Internal Events:
              • recv_cb_start

Legend: ── Data | ►─ Event

**Parameters (3):**
- **GPIO switch:** -1 (-1 to 100) - The GPIO used to switch the UART capability off. I
- **RS485:** 0 (Bool) - Whether this is RS485 function block
- **ComPort:** 0 (0 to 255) - Default UART port to use. (ttyX or COMX for windws)

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 9 ports (6 events, 3 data)

**Ports by Function:**

*start:*
  - **start** (Start Event)
  - **port** (I) - Input
  - **start_OK** (Finish Event)

*close:*
  - **close** (Start Event)
  - **closed** (Finish Event)
  - **error** (Finish Event)

*recv_cb:*
  - **recv_cb_start** (Internal Event - callable from ISR/thread)
  - **recv_data** (S) - Output
  - **recv_size** (I) - Output
  - **receved** (Finish Event)

*send:*
  - **send** (Start Event)
  - **send_data** (S) - Input
  - **send_size** (I) - Input
  - **errno** (I) - Output
  - **send_OK** (Finish Event)
  - **send_error** (Finish Event)
