ml_svm_inference
Run machine learning models with limited resources like a few kilobytes of memor
Menu: 
            Machine Learning
             > SVM Inference
Type: Data | Height: 95

            ┌─────────────────────────┐
      load►─┤                         ├►─-- 
     model──┤                         ├►─err 
            │                         ├──errno (I)
            │           ML            │
        do►─┤                         ├►─-- 
      data──┤                         ├──output (S)
            │                         ├──class/regression (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **model_type:** 0 (0 to 10) - Model Type

**Port Summary:**
- **Left:**  4 ports (2 events, 2 data)
- **Right:** 6 ports (3 events, 3 data)

**Ports by Function:**

*load_model:*
  - **load_model** (Start Event)
  - **model_file_path** (S) - Input
  - **load_errno** (I) - Output
  - **load_ok** (Finish Event)
  - **load_error** (Finish Event)

*do_inference:*
  - **do_inference** (Start Event)
  - **data** (S) - Input
  - **output** (S) - Output
  - **class_regression** (I) - Output
  - **done_inference** (Finish Event)
