adc_read_single
Read ADC value when the sample port is triggered. It will output error when the 
Menu: 
            Peripherals
             > ADC > ADC Read Single
Type: IO | Width: 75 | Height: 60

            ┌─────────────────────────┐
     samp.►─┤                         ├►─-- 
            │        ADC S.S.         │
            │                         ├──value (I)
            │                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **device_id:** 0 (0 to 9) - The ADC Device number to be configured. For intern [Display: 30,20]
- **channel:** 0 (0 to 15) - The ADC Channel ID. If it’s disabled, it will neit [Display: 30,40]
- **fixed_point:** 10 (0 to 16) - fixed_point

**Port Summary:**
- **Left:**  1 ports (1 events, 0 data)
- **Right:** 3 ports (2 events, 1 data)
