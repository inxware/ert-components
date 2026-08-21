ui_data_bool
Provides bidirectional access to a boolean property on a UI object. The set port
Menu: User Interface
                 > Bindings > Boolean
Type: IO | Width: 130 | Height: 58

            ┌─────────────────────────┐
       set►─┤                         ├►─--- 
            │         UI Bool         │
     value──┤                         ├►─--> 
            │                         ├──value_out (B)
            └─────────────────────────┘

            Internal Events:
              • changed_cb_start

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Object:** _qml_root_object_ - Name of the UI object to bind to (matches the obje
- **Property:** checked - Name of the boolean property on the UI object to r [Display: 10,45]

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 3 ports (2 events, 1 data)

**Ports by Function:**

*set:*
  - **set** (Start Event)
  - **value** (B) - Input
  - **done** (Finish Event)

*changed_cb:*
  - **changed_cb_start** (Internal Event - callable from ISR/thread)
  - **value_out** (B) - Output
  - **changed** (Finish Event)
