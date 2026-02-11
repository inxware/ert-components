gui_text_string2
This widget displays an string to the GUI output.
Menu: 
            User Interface
             > Graphics > Text
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
            │         Text UI         │
      data──┤                         ├──data (S)
     label──┤                         ├──label (S)
    update►─┤                         ├►─-- 
  x offset──┤                         ├──abs x (I)
  y offset──┤                         ├──abs y (I)
     width──┤                         ├──wid (I)
    height──┤                         ├──ht (I)
     alpha──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **Widget Tag:** widget - widget tag [Display: 40,200]
- **Widget-Group:** default - widget group
- **Maintain Aspect Ratio:** 0 (Bool) - maintains aspect ratio of widget when width is cha
- **Always Clickable:** 0 (Bool) - allows a widget to register a mouse click event wh

**Port Summary:**
- **Left:**  13 ports (6 events, 7 data)
- **Right:** 15 ports (9 events, 6 data)
