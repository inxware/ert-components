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

Legend: ── Data | ►─ Event

**Parameters (2):**
- **GPIO switch:** -1 (-1 to 100) - The GPIO used to switch the UART capability off. I
- **RS485:** 0 (Bool) - Whether this is RS485 function block

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 9 ports (6 events, 3 data)
