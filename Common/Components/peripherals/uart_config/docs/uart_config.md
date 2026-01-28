uart_config
uart_config
Menu: 
			Peripherals
			 > Serial > UART Config
Type: IO | Width: 95 | Height: 90

             ┌─────────────────────────┐
        set►─┤                         ├►─-- 
   baudrate──┤                         ├──errno (I)
  stop bits──┤                         │
             │        UART conf        │
     parity──┤                         │
data length──┤                         │
    HW ctrl──┤                         │
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (6):**
- **Port:** 0 (0 to 100) - UART port [Display: 20,20]
- **Baud:** 115200 (110 to 921600) - Baudrate of the UART
- **Data Length:** 8 (5 to 8) - UART data length
- **Parity:** 0 (0 to 2) - Parity of UART. 0 for No parity, 1 for odd parity,
- **Stop bits:** 0 (0 to 2) - UART stop bit count. 0 for 1 bit, 1 for 1.5 bits, 
- **Hardware Control:** 0 (Bool) - Whether to enable hardware control of UART

**Port Summary:**
- **Left:**  6 ports (1 events, 5 data)
- **Right:** 2 ports (1 events, 1 data)
