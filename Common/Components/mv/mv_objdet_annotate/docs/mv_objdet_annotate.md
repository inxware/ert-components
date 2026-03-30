mv_objdet_annotate
Parses a JSON detection string (from ml_image_inference or tf_lite_frame) and dr
Menu: 
            Machine Vision
             > Annotate
Type: Data | Width: 75 | Height: 70

            ┌─────────────────────────┐
       run►─┤                         ├►─done 
  frame id──┤                         ├──errno (I)
            │        Annotate         │
detections──┤                         ├►─error 
            │                         ├──annotated frame id (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **box_thickness:** 2 (1 to 10) - Bounding box outline thickness in pixels.
- **show_label:** 1 (Bool) - When 1, draw the class label and confidence above 
- **copy_frame:** 0 (Bool) - When 1, annotate a copy of the input frame and out

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 4 ports (2 events, 2 data)

**Ports by Function:**

*annotate:*
  - **run** (Start Event)
  - **frame_id** (I) - Input
  - **detections** (S) - Input
  - **errno** (I) - Output
  - **annotated_frame_id** (I) - Output
  - **done** (Finish Event)
  - **error** (Finish Event)
