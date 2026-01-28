RuntimeInfo
Device ID: MAC address, Network Address: IP addr, SODL Type: SODL format version
Menu: 
			Deprecated
			 > System > Local > Runtime Info
Type: IO | Width: 100 | Height: 205

              ┌─────────────────────────┐
   getStatic►─┤                         ├►─ 
miscApp name──┤                         ├──device ID (S)
              │                         ├──RAM (kB) (I)
              │                         ├──stor (kB) (I)
              │                         ├──EHS version (S)
              │                         ├──moduleList (S)
              │                         ├──install dir (S)
  getDynamic►─┤                         ├►─ 
              │      Runtime Info       │
              │                         ├──IP addr (S)
              │                         ├──RAM avail kB (I)
              │                         ├──store avail kB (I)
              │                         ├──cpu (eRT) (I)
              │                         ├──RAM (eRT) (I)
              │                         ├──cpu (misc) (I)
              │                         ├──RAM (misc) (I)
req. pair ID►─┤                         ├►─pair ID 
              │                         ├──pair ID (I)
              └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 17 ports (3 events, 14 data)
