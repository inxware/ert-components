dac
Digital-to-Analog Converter
Menu: 
            Peripherals
             > DAC
Type: IO | Width: 65 | Height: 53

            ┌─────────────────────────┐
       set►─┤                         ├►─-- 
            │           DAC           │
     value──┤                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **channel:** 0 (0 to 100) - The channel of the DAC [Display: 25,25]
- **max:** 10000 (1 to 2147483647) - The maximum value of the value input port
- **sample rate:** 1000000 (1 to 2147483647) - The sampling rate when DAC is a PWM, PDM or SDM mo

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 2 ports (2 events, 0 data)

**Ports by Function:**

*set:*
  - **set** (Start Event)
  - **value** (I) - Input
  - **done** (Finish Event)
  - **error** (Finish Event)
