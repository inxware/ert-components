ui_spinner
User interface spinner widget
Menu: 
            User Interface
             > Graphics > Advanced > Spinner
Type: IO

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
    update►─┤                         ├►─-- 
            │         Spinner         │
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
   off.wid──┤                         ├──wid (I)
    off.ht──┤                         ├──ht (I)
     alpha──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **Widget Tag:** widget - widget tag
- **Widget-Group:** default - widget group
- **Time:** 1000 (0 to 60000) - Speed of spinning in milliseconds
- **Arc Length:** 60 (0 to 360) - Length of spinning arc animation

**Port Summary:**
- **Left:**  10 ports (5 events, 5 data)
- **Right:** 9 ports (5 events, 4 data)
