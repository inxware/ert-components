pwm
pwm
Menu: 
            Deprecated
             > Peripherals > GPIO > PWM
Type: IO | Width: 80 | Height: 105

            ┌─────────────────────────┐
    config►─┤                         ├►─-- 
        hz──┤                         │
       pin──┤                         │
            │           PWM           │
    enable►─┤                         ├►─-- 
    enable──┤                         │
  set duty►─┤                         ├►─-- 
         %──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **Pin ID:** 0 (0 to 255) - PWM pin id number
- **Enabled:** 0 (Bool) - PWM default enabled/disabled status
- **Period (Hz):** 0 (0 to 2147483647) - PWM period frequency in Hz
- **Duty (%):** 0 (0 to 100) - PWM duty cycle (0-100)%

**Port Summary:**
- **Left:**  7 ports (3 events, 4 data)
- **Right:** 3 ports (3 events, 0 data)
