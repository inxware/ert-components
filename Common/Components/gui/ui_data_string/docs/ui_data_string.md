ui_data_string
Provides bidirectional access to a string property on a UI object. The set port 
Menu: User Interface
                 > Bindings > String
Type: IO | Width: 150 | Height: 58

            ┌─────────────────────────┐
       set►─┤                         ├►─--- 
            │        UI String        │
     value──┤                         ├►─--> 
            │                         ├──value_out (S)
            └─────────────────────────┘

            Internal Events:
              • changed_cb_start

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Object:** _qml_root_object_ - Name of the UI object to bind to (matches the obje
- **Property:** text - Name of the string property on the UI object to re [Display: 10,45]

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 3 ports (2 events, 1 data)

**Ports by Function:**

*set:*
  - **set** (Start Event)
  - **value** (S) - Input
  - **done** (Finish Event)

*changed_cb:*
  - **changed_cb_start** (Internal Event - callable from ISR/thread)
  - **value_out** (S) - Output
  - **changed** (Finish Event)
