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

**Parameters (1):**
- **window_title:** frame (String) - Title of the pop window.

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 2 ports (2 events, 0 data)

**Ports by Function:**

*show:*
  - **show** (Start Event)
  - **frame_id** (I) - Input
  - **shown** (Finish Event)
  - **error** (Finish Event)
