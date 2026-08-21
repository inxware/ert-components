ui_data_real
Provides bidirectional access to a real-valued property on a UI object. The set 
Menu: User Interface
                 > Bindings > Real
Type: IO | Width: 130 | Height: 58

            ┌─────────────────────────┐
       set►─┤                         ├►─--- 
            │         UI Real         │
     value──┤                         ├►─--> 
            │                         ├──value_out (F)
            └─────────────────────────┘

            Internal Events:
              • changed_cb_start

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Object:** _qml_root_object_ - Name of the UI object to bind to (matches the obje
- **Property:** value - Name of the real-valued property on the UI object  [Display: 10,45]

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 3 ports (2 events, 1 data)

**Ports by Function:**

*set:*
  - **set** (Start Event)
  - **value** (F) - Input
  - **done** (Finish Event)

*changed_cb:*
  - **changed_cb_start** (Internal Event - callable from ISR/thread)
  - **value_out** (F) - Output
  - **changed** (Finish Event)
