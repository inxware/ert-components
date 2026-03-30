frame_show
Show the frame in a pop window with given frame ID input.
Menu: 
            Machine Vision
             > Frame Show
Type: Data | Height: 50

            ┌─────────────────────────┐
      show►─┤                         ├►─-- 
            │       Frame Show        │
  frame id──┤                         ├►─error 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **window_title:** frame (String) - Title of the pop window (OpenCV path only).
- **x:** 0 (-32768 to 32767) - Horizontal position on screen in pixels. 0 = defau
- **y:** 0 (-32768 to 32767) - Vertical position on screen in pixels. 0 = default
- **w:** 0 (0 to 32767) - Display width in pixels. 0 = use the frame's natur
- **h:** 0 (0 to 32767) - Display height in pixels. 0 = use the frame's natu

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 2 ports (2 events, 0 data)

**Ports by Function:**

*show:*
  - **show** (Start Event)
  - **frame_id** (I) - Input
  - **shown** (Finish Event)
  - **error** (Finish Event)
