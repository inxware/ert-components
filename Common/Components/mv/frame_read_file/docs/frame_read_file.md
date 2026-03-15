FrameReadFromFile
Reads frame from a file
Menu: 
            Machine Vision
             > Frame Read From File
Type: Data | Height: 100

            ┌─────────────────────────┐
       run►─┤                         ├►─-- 
  filepath──┤                         ├──stream_id (I)
            │     Frame From File     │
            │                         ├►─err 
            │                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Use Application Dir:** 0 (Bool) - File in Application Directory rather than the user

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 4 ports (2 events, 2 data)

**Ports by Function:**

*run_read_file:*
  - **run_fmt** (Start Event)
  - **filepath** (S) - Input
  - **start_errno** (I) - Output
  - **formated_frame_id** (I) - Output
  - **start_ok** (Finish Event)
  - **start_error** (Finish Event)
