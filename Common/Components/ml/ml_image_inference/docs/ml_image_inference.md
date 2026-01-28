ml_image_inference
Inference of data input based on loaded model with vaiants like yolov8-seg. Hard
Menu: 
            Machine Learning
             > Image Inference
Type: Data | Height: 115

            ┌─────────────────────────┐
      load►─┤                         ├►─-- 
      path──┤                         ├►─err 
            │                         ├──errno (I)
            │        IMG Infer        │
        do►─┤                         ├►─-- 
 stream_id──┤                         ├►─err 
            │                         ├──errno (I)
            │                         ├──json (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (7):**
- **Model Type:** 0 (0 to 4001) - The type of model loaded. It is used to determine 
- **Model File Ext:** 1 (1 to 4) - The file extension of the loaded model. 1 for `.tf
- **Data Type:** 0 (0 to 4) - The Data type of model input/ouput. 0 for auto-det
- **HW Accelerate:** 0 (Bool) - Whether to enable the hardware acceleration. Some 
- **Conf Thres:** 0.5 (Real) - The confidence Threshold for the output data. The 
- **Thread Number:** 0 (0 to 16) - The number of threads for inference. If the platfo
- **Use Application Dir:** 0 (Bool) - Whether to use application directory as the root o

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 7 ports (4 events, 3 data)
