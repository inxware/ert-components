FrameResize
Resize camera frame
Menu: 
            Machine Vision
             > Frame Resize
Type: Data | Height: 100

            ┌─────────────────────────┐
       run►─┤                         ├►─-- 
 stream_id──┤                         ├──stream_id (I)
            │      Frame Resize       │
            │                         ├►─err 
    size_x──┤                         ├──errno (I)
    size_y──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **size_x:** 0 (0 to 10000) - Resize frame width
- **size_y:** 0 (0 to 10000) - Resize frame height

**Port Summary:**
- **Left:**  4 ports (1 events, 3 data)
- **Right:** 4 ports (2 events, 2 data)
