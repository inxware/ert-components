RuntimeInfo2
Device ID: MAC address, Network Address: IP addr, SODL Type: SODL format version
Menu: 
			System
			 > Local > Runtime Info
Type: IO | Width: 100 | Height: 235

              ┌─────────────────────────┐
   getStatic►─┤                         ├►─ 
miscApp name──┤                         ├──device ID (S)
              │                         ├──RAM (kB) (I)
              │                         ├──stor (kB) (I)
              │                         ├──EHS version (S)
              │                         ├──moduleList (S)
              │                         ├──install dir (S)
  getDynamic►─┤                         ├►─ 
              │                         ├──IP addr (S)
              │      Runtime Info       │
              │                         ├──RAM avail kB (I)
              │                         ├──store avail kB (I)
              │                         ├──cpu (eRT) (I)
              │                         ├──RAM (eRT) (I)
              │                         ├──cpu (misc) (I)
              │                         ├──RAM (misc) (I)
              │                         ├──temp (cpu) (I)
              │                         ├──temp (op) (I)
req. pair ID►─┤                         ├►─pair ID 
              │                         ├──pair ID (I)
              └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 19 ports (3 events, 16 data)
