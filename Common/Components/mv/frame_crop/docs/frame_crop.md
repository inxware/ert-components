FrameCrop
Crops camera frame
Menu: 
            Machine Vision
             > Frame Crop
Type: Data | Height: 100

            ┌─────────────────────────┐
       run►─┤                         ├►─-- 
 stream_id──┤                         ├──stream_id (I)
   crop_x1──┤                         ├►─err 
            │       Frame Crop        │
   crop_y1──┤                         ├──errno (I)
   crop_x2──┤                         │
   crop_y2──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **crop_x1:** 0 (0 to 10000) - crop top
- **crop_y1:** 0 (0 to 10000) - Crop left
- **crop_x2:** 0 (0 to 10000) - Crop right
- **crop_y2:** 0 (0 to 10000) - Crop bottom

**Port Summary:**
- **Left:**  6 ports (1 events, 5 data)
- **Right:** 4 ports (2 events, 2 data)
