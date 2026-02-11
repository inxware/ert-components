pwm_gen
Generate a signal with Pulse Width Modulation. PWM will work on a certain set of
Menu: 
            Peripherals
             > GPIO > PWM
Type: Data_Processor | Width: 62 | Height: 94

            ┌─────────────────────────┐
     setup►─┤                         ├►─setup_done 
      freq──┤                         ├►─err 
    maxVal──┤                         ├──errno (I)
        en►─┤                         ├►─enabled 
            │         pwm_gen         │
       dis►─┤                         ├►─disabled 
      duty►─┤                         ├►─set_duty_done 
         %──┤                         ├►─err 
            │                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **channel:** 0 (0 to 1000) - The channel on the device to generate the PWM sign [Display: 20,20]
- **io_num:** 0 (0 to 1000) - PWM GPIO number
- **max_duty:** 100 (1 to 2147483647) - Maximum number of duty input. The output will be i
- **frequency:** 1000 (1 to 2147483647) - The frequency (in Hz) of the PWM controller

**Port Summary:**
- **Left:**  7 ports (4 events, 3 data)
- **Right:** 8 ports (6 events, 2 data)
