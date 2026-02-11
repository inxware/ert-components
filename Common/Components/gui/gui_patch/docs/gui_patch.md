gui_patch
This widget displays a rectangular patch on the  GUI output.
Menu: 
            User Interface
             > Graphics > Patch
Type: IO | Width: 105 | Height: 180

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
            │                         ├►─click 
            │                         ├►─mouse down 
            │          Patch          │
    update►─┤                         ├►─-- 
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
   off.wid──┤                         ├──wid (I)
    off.ht──┤                         ├──ht (I)
     alpha──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **Widget Tag:** widget - widget tag [Display: 40,165]
- **Widget-Group:** default - widget group
- **Maintain Aspect Ratio:** 0 (Bool) - maintains aspect ratio of widget when width is cha
- **Always Clickable:** 0 (Bool) - allows a widget to register a mouse click event wh

**Port Summary:**
- **Left:**  10 ports (5 events, 5 data)
- **Right:** 11 ports (7 events, 4 data)
