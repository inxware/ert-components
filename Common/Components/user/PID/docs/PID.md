PID
A PID controller with tailored specific configurations.
Menu: 
            Control Systems
             > PID Controller
Type: Data | Height: 290

             ┌─────────────────────────┐
       init►─┤                         ├►─-- 
   isr mode──┤                         │
      calib──┤                         │
    measure►─┤                         ├►─-- 
      value──┤                         │
  set point►─┤                         ├►─-- 
      value──┤                         │
 pid config►─┤                         ├►─-- 
          p──┤                         │
          i──┤                         │
          d──┤                         │
      max i──┤                         │
             │           PID           │
 max global──┤                         │
  io config►─┤                         ├►─-- 
  sensor id──┤                         ├►─measured 
output mode──┤                         ├──value (F)
    output1──┤                         ├──connected (B)
    output2──┤                         ├►─ctrl 
    output3──┤                         ├──out% (F)
relay check──┤                         ├──pwm1 (B)
             │                         ├──pwm2 (B)
    get isr►─┤                         ├──pwm3 (B)
   get ctrl►─┤                         │
        set►─┤                         ├►─-- 
   disabled──┤                         │
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (12):**
- **PIDNo:** 1 (1 to 3) - The channel number of this PID controller [Display: 50,20]
- **ISR Mode:** 0 (Bool) - If Non-ISR, the value are read from the "measured"
- **Sensor ID:** 0 (0 to 10) - Sensor ID
- **P Default:** 1 (Real) - Default value of P
- **I Default:** 0 (Real) - Default value of I
- **D Default:** 0 (Real) - Default value of D
- **Output Mode:** 0 (0 to 10) - Output Mode
- **Output1 Default:** 0 (0 to 10) - Output1 Default
- **Output2 Default:** 0 (0 to 10) - Output2 Default
- **Output3 Default:** 0 (0 to 10) - Output3 Default
- **Output Disabled:** 0 (Bool) - Disables Output of the controller e.g. scheduler d
- **Calibration Mode:** 0 (Bool) - Calibration Mode

**Port Summary:**
- **Left:**  24 ports (8 events, 16 data)
- **Right:** 14 ports (8 events, 6 data)
