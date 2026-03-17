buzzer
Piezo buzzer control. Plays a tone at a specified frequency for a specified dura
Menu: 
            Peripherals
             > Buzzer
Type: IO | Width: 50 | Height: 55

             ┌─────────────────────────┐
       beep►─┤                         ├►─-- 
    freq Hz──┤                         │
             │          Buzz           │
duration ms──┤                         │
       stop►─┤                         ├►─-- 
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Frequency (Hz):** 1000 (100 to 10000) - Default tone frequency in Hz.
- **Duration (ms):** 200 (1 to 60000) - Default tone duration in milliseconds. 0 = continu

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 2 ports (2 events, 0 data)

**Ports by Function:**

*BeepFunc:*
  - **beep** (Start Event)
  - **freq_hz** (I) - Input
  - **duration_ms** (I) - Input
  - **done** (Finish Event)

*StopFunc:*
  - **stop** (Start Event)
  - **stopped** (Finish Event)
