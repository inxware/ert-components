gui_text_real2
This widget displays an floating point number to the GUI output. It can also be 
Menu: 
            User Interface
             > Graphics > Floating Point
Type: IO | Width: 115 | Height: 215

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
            │                         ├►─click 
            │                         ├►─mouse down 
            │                         ├►─data change 
    update►─┤                         ├►─-- 
            │        Float UI         │
      data──┤                         ├──data (F)
     label──┤                         ├──label (S)
    update►─┤                         ├►─-- 
  x offset──┤                         ├──abs x (I)
  y offset──┤                         ├──abs y (I)
     width──┤                         ├──wid (I)
    height──┤                         ├──ht (I)
     alpha──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **Widget Tag:** widget - widget tag [Display: 40,200]
- **Widget-Group:** default - widget group
- **Maintain Aspect Ratio:** 0 (Bool) - maintains aspect ratio of widget when width is cha
- **Always Clickable:** 0 (Bool) - allows a widget to register a mouse click event wh
- **Number of decimal places:** 2 (0 to 6) - Max number of decimal places

**Port Summary:**
- **Left:**  13 ports (6 events, 7 data)
- **Right:** 15 ports (9 events, 6 data)
