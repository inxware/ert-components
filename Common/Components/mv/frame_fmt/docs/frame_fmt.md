FrameFormatter
Modifies camera frame format e.g. int8_3 to float32_3
Menu: 
            Machine Vision
             > Frame Formatter
Type: Data | Height: 100

            ┌─────────────────────────┐
       run►─┤                         ├►─-- 
 stream_id──┤                         ├──stream_id (I)
            │        Frame Fmt        │
            │                         ├►─err 
    format──┤                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **format:** 0 (0 to 10) - format data type

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 4 ports (2 events, 2 data)
