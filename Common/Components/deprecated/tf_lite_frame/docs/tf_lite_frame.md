TF_Lite_from_frame
Pass image frame to machine learning Tensorflow Lite model. Supports yolov5 ...
Menu: 
            Machine Learning
             > Tensorflow Lite Image DEPRECATED
Type: Data | Height: 125

               ┌─────────────────────────┐
         load►─┤                         ├►─-- 
        model──┤                         ├►─err 
               │                         ├──errno (I)
               │                         ├──info (S)
               │   ML Image Inference    │
           do►─┤                         ├►─--
    stream_id──┤                         ├──json (S)
               │                         ├►─err 
               │                         ├──errno (I)
               └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **Model Type:** 0 (0 to 99999999) - Model Type
- **Conf Thres:** 0.5 (Real) - Confidence Threshold
- **Thread Count:** 2 (0 to 20) - Number of model processing  threads
- **Flat JSON:** 0 (Bool) - Use a flat JSON output format
- **Use Application Dir:** 0 (Bool) - Load model in Application Directory rather than th

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 8 ports (4 events, 4 data)

**Ports by Function:**

*load_model:*
  - **load_model** (Start Event)
  - **model_file_path** (S) - Input
  - **load_errno** (I) - Output
  - **model_info** (S) - Output
  - **load_ok** (Finish Event)
  - **load_error** (Finish Event)

*do_inference:*
  - **do_inference** (Start Event)
  - **frame_id** (I) - Input
  - **output** (S) - Output
  - **inference_errno** (I) - Output
  - **done_inference** (Finish Event)
  - **inference_error** (Finish Event)
