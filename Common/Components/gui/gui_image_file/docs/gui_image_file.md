GUI_Image_File
Menu: 
            User Interface
             > Graphics > Image
Type: IO | Width: 105 | Height: 200

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      load►─┤                         ├►─-- 
      file──┤                         │
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
            │                         ├►─click 
            │          Image          │
            │                         ├►─mouse down 
    update►─┤                         ├►─-- 
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
   off.wid──┤                         ├──wid (I)
    off.ht──┤                         ├──ht (I)
     alpha──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **Widget Tag:** widget - widget tag [Display: 40,185]
- **Widget-Group:** default - widget group
- **Maintain Aspect Ratio:** 0 (Bool) - maintains aspect ratio of widget when width is cha
- **Always Clickable:** 0 (Bool) - allows a widget to register a mouse click event wh
- **Load From App Dir:** 0 (Bool) - load the image from the app directory at runtime i

**Port Summary:**
- **Left:**  12 ports (6 events, 6 data)
- **Right:** 12 ports (8 events, 4 data)
