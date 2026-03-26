adc_read
adc_read
Menu: 
            Peripherals
             > ADC > ADC Read
Type: IO | Width: 75 | Height: 75

             ┌─────────────────────────┐
Set Channel►─┤                         │
    channel──┤                         │
             │           ADC           │
     Sample►─┤                         ├►─-- 
             │                         ├──value (F)
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **channel:** 0 (0 to 4096) - ADC channel [Display: 45,5]
- **enable_continuous:** 0 (Bool) - Enable continuous mode clock
- **clock_rate_hz:** 1 (Real) - Optional clock rate (Hz) for continous mode
- **Average:** 1 (0 to 1000000) - Acquisitions to average
- **Bias:** 0 (Real) - The bias to be taken from the average

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 2 ports (1 events, 1 data)
