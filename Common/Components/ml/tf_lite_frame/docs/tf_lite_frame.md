TF_Lite_from_frame
Pass image frame to machine learning Tensorflow Lite model. Supports yolov5 ...
Menu: 
            Machine Learning
             > Tensorflow Lite from frame
Type: Data | Height: 95

            ┌─────────────────────────┐
      load►─┤                         ├►─-- 
     model──┤                         ├►─err 
            │        ML TFLite        │
            │                         ├──errno (I)
        do►─┤                         ├►─-- 
 stream_id──┤                         ├──json (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **Model Type:** 0 (0 to 10) - Model Type
- **Conf Thres:** 0.5 (Real) - Confidence Threshold
- **Thread Count:** 2 (0 to 20) - Number of model processing  threads
- **Flat JSON:** 0 (Bool) - Use a flat JSON output format
- **Use Application Dir:** 0 (Bool) - Load model in Application Directory rather than th

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 5 ports (3 events, 2 data)
