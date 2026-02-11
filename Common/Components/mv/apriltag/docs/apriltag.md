apriltag
Detect all apriltags with their IDs and coordinates of 4 corners in the image.
Menu: 
            Machine Vision
             > AprilTag
Type: Data | Height: 155

             ┌─────────────────────────┐
     config►─┤                         ├►─-- 
     family──┤                         │
   decimate──┤                         │
      sigma──┤                         │
             │        AprilTag         │
refine_edge──┤                         │
     detect►─┤                         ├►─-- 
  stream_id──┤                         ├►─err 
    sharpen──┤                         ├──errno (I)
             │                         ├──json (S)
             └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **apriltag_family:** 0 (0 to 7) - The AprilTag family to be detected. The enumeratio
- **quad_decimate:** 2 (Real) - Detection of quads can be done on a lower-resoluti
- **quad_sigma:** 0 (Real) - What Gaussian blur should be applied to the segmen
- **refine_edges:** 1 (Bool) - When true, the edges of the each quad are adjusted
- **decode_sharpening:** 0.25 (Real) - How much sharpening should be done to decoded imag

**Port Summary:**
- **Left:**  8 ports (2 events, 6 data)
- **Right:** 5 ports (3 events, 2 data)
