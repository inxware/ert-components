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

**Parameters (3):**
- **size_x:** 0 (0 to 10000) - Resize frame width
- **size_y:** 0 (0 to 10000) - Resize frame height
- **Interpolation:** 1 (0 to 3) - Interpolation method: 0=Nearest (fastest, ML), 1=L

**Port Summary:**
- **Left:**  4 ports (1 events, 3 data)
- **Right:** 4 ports (2 events, 2 data)

**Ports by Function:**

*resize:*
  - **run_fmt** (Start Event)
  - **frame_id** (I) - Input
  - **size_x** (I) - Input
  - **size_y** (I) - Input
  - **start_errno** (I) - Output
  - **formated_frame_id** (I) - Output
  - **start_ok** (Finish Event)
  - **start_error** (Finish Event)
