GUI_keypress
Detects when a key is pressed and asserts the ASCII or extended key code integer
Menu: 
            User Interface
             > Hardware > Keyboard
Type: IO

            ┌─────────────────────────┐
    enable►─┤                         ├►─-- 
   disable►─┤                         ├►─-- 
            │                         ├►─key 
            │                         ├──ascii (I)
            │                         ├──char (S)
            │                         ├►─control 
            │                         ├──id (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  2 ports (2 events, 0 data)
- **Right:** 7 ports (4 events, 3 data)
