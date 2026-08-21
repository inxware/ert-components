ui_data_int
Provides bidirectional access to an integer property on a UI object. The set por
Menu: User Interface
                 > Bindings > Integer
Type: IO | Width: 130 | Height: 58

            ┌─────────────────────────┐
       set►─┤                         ├►─--- 
            │         UI Int          │
     value──┤                         ├►─--> 
            │                         ├──value_out (I)
            └─────────────────────────┘

            Internal Events:
              • changed_cb_start

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Object:** _qml_root_object_ - Name of the UI object to bind to (matches the obje
- **Property:** value - Name of the integer property on the UI object to r [Display: 10,45]

**Port Summary:**
- **Left:**  2 ports (1 events, 1 data)
- **Right:** 3 ports (2 events, 1 data)

**Ports by Function:**

*set:*
  - **set** (Start Event)
  - **value** (I) - Input
  - **done** (Finish Event)

*changed_cb:*
  - **changed_cb_start** (Internal Event - callable from ISR/thread)
  - **value_out** (I) - Output
  - **changed** (Finish Event)
