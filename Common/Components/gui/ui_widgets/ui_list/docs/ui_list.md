ui_list
User interface plot chart widget
Menu: 
            User Interface
             > Graphics > Advanced > List
Type: IO

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
    update►─┤                         ├►─-- 
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
            │         List UI         │
   off.wid──┤                         ├──wid (I)
    off.ht──┤                         ├──ht (I)
     alpha──┤                         │
    append►─┤                         ├►─-- 
      item──┤                         │
       rgb──┤                         │
     clear►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **Widget Tag:** widget - widget tag
- **Widget-Group:** default - widget group
- **Max Size:** 10 (0 to 1000) - Max size of the list items
- **Reversed:** 0 (Bool) - Order the list growth

**Port Summary:**
- **Left:**  14 ports (7 events, 7 data)
- **Right:** 11 ports (7 events, 4 data)
