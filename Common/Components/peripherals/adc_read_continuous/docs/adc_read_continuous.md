adc_read_continuous
Read value continuously as mean and mean squared values. The port and data are t
Menu: 
            Peripherals
             > ADC > ADC Read Continuous
Type: IO | Width: 75 | Height: 135

            ┌─────────────────────────┐
            │                         ├►─-- 
            │                         ├──mean (I)
            │                         ├──var. (I)
            │                         ├──M.S. (I)
            │        ADC Cont         │
            │                         ├►─error 
       set►─┤                         ├►─-- 
   MS bias──┤                         │
    enable►─┤                         ├►─-- 
   disable►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **device_id:** 0 (0 to 9) - The ADC Device number to be configured. For intern [Display: 30,20]
- **channel:** 0 (0 to 15) - The ADC Channel ID. If it’s disabled, it will neit [Display: 30,40]
- **decimation:** 1 (1 to 1000000000) - The ADC Decimation number. This means dividing the
- **decimate_average:** 1 (1 to 1000000000) - The number of the last samples in the decimation p
- **bias:** 0 (Real) - The bias of the out need to be taken from the valu

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 8 ports (5 events, 3 data)
