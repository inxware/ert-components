adc_config
Configure the ADC unit either from parameters or JSON string
Menu: 
            Peripherals
             > ADC > ADC Config
Type: IO | Width: 75 | Height: 75

            ┌─────────────────────────┐
      init►─┤                         ├►─-- 
            │       ADC Config        │
      JSON──┤                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (19):**
- **device_id:** 0 (0 to 9) - The ADC Device number to be configured. For intern [Display: 30,20]
- **f_s_hz:** 1000 (100 to 1000000000) - Total sampling frequency in Hz. With n channels en
- **mode:** 0 (Bool) - The ADC conversion mode. False for Single-shot. Tr
- **channel_0:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_1:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_2:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_3:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_4:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_5:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_6:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_7:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_8:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_9:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_10:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_11:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_12:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_13:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_14:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in
- **channel_15:** -1 (-1 to 999) - Pin mapping for individual channels of this ADC in

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 2 ports (2 events, 0 data)
