ui_event
Provides bidirectional event access to a signal on a UI object. The emit port fi
Menu: User Interface
                 > Bindings > Event
Type: IO | Width: 150 | Height: 42

            ┌─────────────────────────┐
      emit►─┤                         ├►─--- 
            │        UI Event         │
            │                         ├►─--> 
            └─────────────────────────┘

            Internal Events:
              • received_cb_start

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Object:** _qml_root_object_ - Name of the UI object to bind to (matches the obje
- **Signal:** clicked - Name of the signal on the UI object to emit and li [Display: 10,30]

**Port Summary:**
- **Left:**  1 ports (1 events, 0 data)
- **Right:** 2 ports (2 events, 0 data)

**Ports by Function:**

*emit:*
  - **emit** (Start Event)
  - **done** (Finish Event)

*received_cb:*
  - **received_cb_start** (Internal Event - callable from ISR/thread)
  - **received** (Finish Event)
