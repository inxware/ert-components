TFLM
Run machine learning models with limited resources like a few kilobytes of memor
Menu: 
            Machine Learning
             > Tensor Flow Lite Micro
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
