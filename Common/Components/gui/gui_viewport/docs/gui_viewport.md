gui_viewport
GUI window position, size, colour and transparency. Edit parameters in IGB.
Menu: 
            User Interface
             > View Port > Window
Type: IO | Width: 105 | Height: 250

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
            │                         ├►─mouse down 
            │                         ├►─mouse up 
            │                         ├──abs.x (I)
            │                         ├──abs.y (I)
            │                         ├►─mouse drag 
            │                         ├──off.x (I)
            │         Window          │
            │                         ├──off.y (I)
    update►─┤                         ├►─-- 
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
   off.wid──┤                         ├──wid (I)
    off.ht──┤                         ├──ht (I)
     alpha──┤                         ├►─drag top 
            │                         ├►─drag left 
            │                         ├►─drag right 
            │                         ├►─drag bottom 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (7):**
- **Widget Tag:** widget - widget tag [Display: 5,232]
- **Widget-Group:** default - widget group
- **Relative %:** 1 (Bool) - Screen Coordinates in % full screen size (or false
- **top:** -1 (-1 to 100000000) - coordinate in pixels or screen perentange
- **height:** -1 (-1 to 100000000) - coordinate in pixels or screen perentange
- **left:** -1 (-1 to 100000000) - coordinate in pixels or screen perentange
- **width:** -1 (-1 to 100000000) - coordinate in pixels or screen perentange

**Port Summary:**
- **Left:**  10 ports (5 events, 5 data)
- **Right:** 20 ports (12 events, 8 data)
