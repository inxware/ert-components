# CLAUDE-ert-graphics.md

Context file for Claude Code covering the eRT graphics subsystem architecture, widget rendering modes, and the Qt integration layer.

## Graphics Subsystem Overview

The eRT graphics subsystem spans three layers:

1. **Component Layer** (`Common/Components/gui/`) — Platform-independent function block implementations (e.g. `inx-gui_widget.c`). These define the EHS function blocks that application designers wire up in Lucid. They communicate with the HAL through well-defined interfaces and never touch platform-specific APIs.

2. **Common HAL** (`Common/HAL/graphics/`) — Shared abstractions: `widget.h` (the `EhsWidgetStruct` superclass), `widget_ui.h` (UI widget subclass and event IDs), `widget_viewport.h`, `hal_viewport.h`. This layer defines the data structures and function signatures that any target HAL must implement.

3. **Target HAL** (`target/Component-HAL/graphics/<implementation>/`) — Platform-specific implementations. Current implementations:
   - `gtk/` — GTK2/framebuffer (Render Mode A)
   - `lvgl/` — LVGL library (Render Mode B)
   - `qt/` — Qt5/Qt6 QML (Render Mode B Qt)

## Render Mode A vs Render Mode B

This is the most important architectural distinction in the graphics subsystem. The render mode determines who owns pixel buffers, who does hit-testing, and how mouse/touch events reach EHS function blocks.

### Render Mode A (GTK, framebuffer)

**eRT owns everything.** The HAL manages pixel buffers, blits images to a viewport framebuffer, and renders text using its own font engine.

**Input handling:** The HAL receives raw mouse/touch coordinates from the OS. It must perform **coordinate-based hit-testing** — walking the widget table, comparing the raw coordinates against each widget's `xCurRect` bounding rectangle, respecting `nZ` z-order to determine which widget is on top. Once it identifies the target widget, it fires the appropriate EHS kernel finish port using **per-widget port numbers** stored directly on the `EhsWidgetStruct`:

| Field | Purpose |
|-------|---------|
| `mouseClickPortNumber` | Finish port fired on click |
| `mouseDownPortNumber` | Finish port fired on mouse down |
| `mouseUpPortNumber` | Finish port fired on mouse up |
| `mouseDragPortNumber` | Finish port fired on drag |
| `mouseUpDownAbsXPortNumber` | Finish port for absolute X coordinate |
| `mouseUpDownAbsYPortNumber` | Finish port for absolute Y coordinate |
| `mouseDragOffsetXPortNumber` | Finish port for drag X delta |
| `mouseDragOffsetYPortNumber` | Finish port for drag Y delta |

These port numbers are populated during widget creation from the CDF/function-block definition and allow the HAL to dispatch events without knowing the function block's internal wiring.

Additional Mode A-only fields on `EhsWidgetStruct`:
- `pfMouseDownEventFunc` / `pMouseDownEventData` — callback for non-FB widgets (e.g. GPIO)
- `nMouseDownX` / `nMouseDownY` — coordinates of last mouse-down, used for drag offset calculation
- `bRegisteredMouseDown` — tracks active mouse-down state for pairing down/up events
- `bOptimiseForSpeed` — hint to prefer speed over memory
- `MediaRect` as `EhsGraphicsRectangleClass` (full x,y,w,h needed for blitting)

The `specificWidgetType` union includes `image` and `textbox` subclasses in Mode A, since eRT handles image decoding and text rendering directly.

**Compile guard:** `#if !defined(EHS_GUI_SUPPORT_MODE_B)` compiles out all Mode A-only fields for Mode B targets.

### Render Mode B (LVGL, Qt)

**An external widget library owns rendering and hit-testing.** eRT does not manage pixel buffers or perform coordinate-based hit-testing.

**Input handling:** The external library knows which widget was interacted with. Events arrive in eRT through the `event_callback` function pointer in `EhsWidgetUiSubclass` (accessed via the `EHS_WIDGET_UI(pWidget)` macro). The callback carries a generic event ID:

| Event ID | Value | Meaning |
|----------|-------|---------|
| `EHS_WIDGET_UI_EVENT_MOUSE_DOWN` | 0x0001 | Mouse/touch down |
| `EHS_WIDGET_UI_EVENT_MOUSE_CLICKED` | 0x0002 | Mouse/touch click (release) |
| `EHS_WIDGET_UI_EVENT_DATA_UPDATED` | 0x0004 | Data updated by library |
| `EHS_WIDGET_UI_EVENT_LABEL_UPDATED` | 0x0008 | Label updated by library |
| `EHS_WIDGET_UI_EVENT_DATA_CHANGED` | 0x0010 | Data changed by user interaction |
| `EHS_WIDGET_UI_EVENT_LABEL_CHANGED` | 0x0020 | Label changed by user interaction |

The Mode B event handler in the component layer (`gui_widget_event_callback` in `inx-gui_widget.c`) receives these events and fires the appropriate EHS finish port using the function block's own port definitions — not per-widget stored port numbers.

**Property updates (eRT → library):** eRT pushes changes to the external library via the `pfDrawFunc` virtual method. To minimise unnecessary updates, the draw function checks **dirty flags**:
- `bContentUpdated` — text/label/data changed
- `bPositionUpdated` — position or size changed
- `bColourUpdated` — colour or alpha changed

These flags are typically set by a function block's update port handler.

The `specificWidgetType` union uses `EhsWidgetUiSubclass ui` in Mode B (instead of `image`/`textbox`), since the external library handles those widget types.

**Qt-specific:** `qt_handle` (`ertqt_object_handle`) maps an EHS widget to its QML QObject. Only present when `EHS_GUI_SUPPORT_MODE_B_QT` is defined.

### Why Port Numbers Are Mode A Only

In Mode A, eRT receives raw coordinates and must:
1. Walk the widget table checking bounding rectangles and z-order
2. Identify which widget was touched
3. Know which finish port to fire for each event type

The widget struct stores port numbers so the HAL can fire ports directly without understanding function block internals.

In Mode B, none of this is needed because:
1. The external library already knows which widget was clicked (it owns hit-testing)
2. Events arrive via `event_callback` with a generic event ID
3. The component-layer event handler uses the function block's own port definitions
4. The widget struct doesn't need to know about port wiring — cleaner separation of concerns

## EhsWidgetStruct — Full Field Reference

Defined in `Common/HAL/graphics/widget.h`. See the inline comments there for per-field details. Key groupings:

| Group | Fields | Mode |
|-------|--------|------|
| Geometry | `xDesignRect`, `xOrigRect`, `xCurRect`, `UpdatedOffsettRect`, `MediaRect` | Shared (MediaRect type differs) |
| Z-order | `nZ` | Shared |
| Virtual methods | `pfFadeFunc`, `pfCreateFunc`, `pfDestroyFunc`, `pfDrawFunc` | Shared |
| Type discriminator | `eWidgetKind` | Shared |
| Mouse state | `pfMouseDownEventFunc`, `pMouseDownEventData`, `nMouseDownX/Y` | Mode A only |
| FB back-pointer | `pFIData` | Shared |
| Type-specific union | `image`, `textbox` (Mode A), `patch` (shared), `ui` (Mode B) | Mixed |
| Appearance | `nAlpha`, `bMaintainAspectRatio`, `bContentChanged` | Shared |
| Dirty flags | `bContentUpdated`, `bPositionUpdated`, `bColourUpdated` | Mode B |
| Lifecycle | `nState` | Shared |
| Port numbers | `mouseClickPortNumber`, `mouseDownPortNumber`, etc. (8 fields) | Mode A only |
| Mouse tracking | `bRegisteredMouseDown`, `bOptimiseForSpeed` | Mode A only |
| Qt handle | `qt_handle` | Mode B Qt only |
| Click capture | `bCaptureClicksIgnoringZOrder` | Shared |

## EhsWidgetUiSubclass — UI Widget Subclass

Defined in `Common/HAL/graphics/widget_ui.h`. Accessed via `EHS_WIDGET_UI(pWidget)` macro. Key fields:

| Field | Purpose |
|-------|---------|
| `pUiObject` | Pointer to target-specific graphics object (void*) |
| `data` | Pointer to widget-specific data (e.g. `EhsWidgetUi` containing label and data pointers) |
| `event_callback` | Function pointer for receiving events from the external library (Mode B) |
| `xFgColour` / `xBgColour` | Foreground and background colours |
| `id` | UI type ID (see enums: `EHS_STRING_UI_WIDGET`, `EHS_BOOL_UI_WIDGET_BUTTON`, etc.) |
| `properties` | Custom properties ID |
| `curvature` | Roundness of the object |
| `parent_id` | Widget parent ID |
| `nIndentLeft/Right/Top/Bottom` | Text indents |
| `nLineSep` | Line separation |
| `nFgBaseAlpha` / `nBgBaseAlpha` | Starting alpha for foreground/background colours |
| `nUiState` | Widget state in the rendering loop (`EHS_WIDGET_UI_STATE_*`) |
| `nNoOfDecPlaces` | Number of decimal places |

## Widget Lifecycle (Mode B)

The full lifecycle from kernel startup to widget event handling:

```
Kernel boot
  → EhsInit()
  → EhsAppLoadingStateMachine()  — loads the app definition
  → EhsMainLoop / EhsMainLoopSingle()
    → Kernel scheduler processes function blocks
      → gui_widget IDENTIFY
      → gui_widget INIT
      → gui_widget_create RUN
        → EhsWidget_create()
          → pfCreateFunc()  [virtual method]
            → EhsWidgetUi_create()
              → EhsTargetWidgetUi_create()  [target HAL]
                → Looks up Qt object by name
                → Stores qt_handle
                → Registers signal handlers (pressed/released/clicked)
                → event_callback is already set by the component layer
```

**Critical dependency:** `EhsMainLoopSingle` (or `EhsMainLoop`) must complete the kernel state machine transition from `EHSKE_STATE_READY` to `EHSKE_STATE_RUNNING` for function blocks to be processed. If this transition doesn't happen, widgets are never created.

## GUI Targets

### LVGL

LVGL is a Mode B target using the [LVGL](https://lvgl.io/) embedded graphics library. Implementation is in `target/Component-HAL/graphics/lvgl/`.

*(LVGL documentation to be expanded.)*

### Qt

#### Overview

The Qt target implements Mode B rendering using Qt5 or Qt6 QML. The application UI is defined in QML files, and eRT controls QML objects by setting properties and receiving signals through a C glue layer.

#### Files

| File | Language | Purpose |
|------|----------|---------|
| `target/Component-HAL/graphics/qt/ertqt.h` | C | Public C API for the Qt integration layer |
| `target/Component-HAL/graphics/qt/ertqt.cpp` | C++ | Implementation: QML engine, object table, property access, signal binding |
| `target/Component-HAL/graphics/qt/qt_main_integration.c` | C | EHS-side integration: tick callback, Qt init/run wrappers |
| `target/Component-HAL/graphics/qt/qt_main_integration.h` | C | Header for above |
| `target/Component-HAL/graphics/qt/target_viewport.c` | C | Mode B viewport HAL: widget create/draw/destroy, signal handlers |
| `target/Component-HAL/graphics/qt/ertqt_button.h` | C | Convenience wrappers for button widgets |
| `target/Component-HAL/graphics/qt/ertqt_label.h` | C | Convenience wrappers for label widgets |
| `target/Component-HAL/graphics/qt/ertqt_text_field.h` | C | Convenience wrappers for text field widgets |
| `target/Component-HAL/graphics/qt/ertqt_checkbox.h` | C | Convenience wrappers for checkbox widgets |

#### Event Loop Integration

Qt owns the event loop. The EHS kernel is driven by a Qt timer:

```
target_main.c (ifdef EHS_MAIN_LOOP_ITERATIVE):
  EhsInit(argc, argv)
  EhsTV_initQt(argc, argv)        → ertqt_init() → creates QGuiApplication + QQmlApplicationEngine
  EhsAppLoadingStateMachine()      → loads the app definition into the kernel
  EhsTV_registerTickCallback()     → ertqt_set_tick_callback(10ms, ehs_tick_callback)
  EhsTV_runQt()                    → ertqt_run() → g_app->exec() [BLOCKS]
```

The tick callback (in `qt_main_integration.c`):
```c
static void ehs_tick_callback(void * user_data)
{
    Ehs_ConsoleCommand_Type cmd;
    cmd = EhsMainLoopSingle(NULL, NULL);  // Single-step the kernel
    if (EhsCheckAppExitLoop(cmd) == EHS_TRUE)
    {
        ertqt_quit();  // Request Qt event loop exit
    }
}
```

`EhsMainLoopSingle` (in libehs.a) sets `EhsYieldWhenEmpty = EHS_TRUE` then calls `EhsMainLoop` + `EhsProcessInAppStateMachine` + `EhsProcessExAppStateMachine`. The yield flag prevents the kernel from blocking when there's no work to do, allowing control to return to the Qt event loop.

#### Qt Object Handle System

The `ertqt.cpp` layer maintains an internal object table (`g_objects`) populated by `rebuild_object_table()`, which walks all `QObject` children of the QML engine's root objects. EHS widgets are mapped to QML objects by `objectName`:

```
EhsTargetWidgetUi_create(pWidget)
  → ertqt_get_object_by_name("widget_name")  // looks up objectName in table
  → returns ertqt_object_handle (opaque, actually a cast QObject pointer)
  → pWidget->qt_handle = h
```

The handle is then used for all property access and signal binding.

#### Qt Signal → EHS Event Flow (Click Example)

```
User clicks QML Button
  → Qt emits pressed() signal
    → ertqt.cpp lambda fires (connected via QMetaObject introspection)
      → Invokes C callback: qt_on_button_pressed(pWidget)
        → Calls event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_DOWN, NULL, NULL)
          → gui_widget_event_callback() in inx-gui_widget.c
            → Fires EHS_FB_FINISH on the function block's mouse_down port

  → User releases button
  → Qt emits released() signal
    → ertqt.cpp lambda fires
      → Invokes C callback: qt_on_button_released(pWidget)
        → Calls event_callback(pWidget, EHS_WIDGET_UI_EVENT_MOUSE_CLICKED, NULL, NULL)
          → gui_widget_event_callback()
            → Fires EHS_FB_FINISH on the function block's click port
```

Note: EHS treats `released()` as the actual "click" event (not Qt's `clicked()` signal).

#### Qt Property Push Flow (Data Update Example)

```
EHS function block update port fires
  → gui_widget sets bContentUpdated = EHS_TRUE on pWidget
  → Next draw cycle calls pfDrawFunc(pWidget, pViewport, pClipRect)
    → EhsTargetWidgetUi_draw(pWidget)  [in target_viewport.c]
      → Checks bContentUpdated flag
      → Reads EHS_WIDGET_UI(pWidget).data → EhsWidgetUi.data
      → For string widgets: ertqt_set_property_string(qt_handle, "text", text)
      → For bool widgets: ertqt_set_property_bool(qt_handle, "checked", value)
      → Clears bContentUpdated flag
      → Calls ertqt_update_widget(qt_handle) to request Qt repaint
```

#### Signal Connection Mechanism

`ertqt.cpp` uses Qt's `QMetaObject` introspection to connect signals by name at runtime:

1. Looks up signal index via `QMetaObject::indexOfSignal()`
2. Tries parameterless first (e.g. `clicked()`), then with common params (e.g. `clicked(bool)`)
3. Uses `QSignalMapper` as bridge to connect string-based signals to C callbacks (works on both Qt5 and Qt6)
4. Lambda invokes the C callback with `user_data`

This avoids compile-time coupling to specific QML types and works with any QObject that exposes the expected signal names.

#### Build Configuration

Set in `target/platform/<platform>/config.mk`:

```makefile
EHS_GUI_SUPPORT=qt
EHS_GUI_SUPPORT_MODE_B_QT=yes
EHS_MAIN_LOOP_ITERATIVE=yes
EHS_DEBUG_TCPIP_CONSOLE=stubbed   # Conflicts with Qt event loop
EHS_GUI_SUPPORT_QT6=yes           # Set for Qt6, omit for Qt5
```

The `EHS_MAIN_LOOP_ITERATIVE` flag is translated to a compiler define in `target/platform/platform.mk`:

```makefile
ifdef EHS_MAIN_LOOP_ITERATIVE
    DEFS += EHS_MAIN_LOOP_ITERATIVE
endif
```

This selects the Qt timer-based startup path in `target_main.c` instead of the blocking `EhsMain()` call.

Qt include and library paths are configured in `target/Component-HAL/graphics/qt/graphics.mk`, with separate blocks for Qt5 and Qt6 selected by `EHS_GUI_SUPPORT_QT6`.

#### Execution Model (Detailed)

Qt owns the main event loop. The eRT kernel does **not** run its own loop — instead it is single-stepped from a Qt timer callback on the GUI thread. This means all eRT work (widget creation, property pushes, signal handling) runs on the Qt thread, satisfying Qt's thread-affinity requirements for QObject operations.

The startup sequence (in `target_main.c` when `EHS_MAIN_LOOP_ITERATIVE` is defined):

```
main()
  → EhsInit(argc, argv)              — initialise eRT kernel
  → EhsTV_initQt(argc, argv)         — calls ertqt_init():
      → Creates QGuiApplication
      → Creates QQmlApplicationEngine
      → Adds app directory to QML import path
      → Loads app.qml from the application's appdata directory
      → Builds object table from QML scene (rebuild_object_table)
  → EhsAppLoadingStateMachine()      — loads SODL app definition into kernel
  → EhsTV_registerTickCallback()     — calls ertqt_set_tick_callback(10ms, ...)
  → EhsTV_runQt()                    — calls ertqt_run() → g_app->exec() [BLOCKS]
```

Once `exec()` is running, the only entry point back into eRT is the tick callback.

#### The Tick Timer

`ertqt_set_tick_callback()` starts a `QTimer::singleShot` chain on the Qt main thread. Each firing:

1. Invokes the registered C callback (`ehs_tick_callback` in `qt_main_integration.c`)
2. Re-arms the timer for the next interval (default 10 ms)

The tick callback does:
```c
EhsTimer_tick();                    // process expired eRT timers
cmd = EhsMainLoopSingle(NULL, NULL); // single-step the kernel scheduler
```

`EhsMainLoopSingle` sets `EhsYieldWhenEmpty = EHS_TRUE` so the kernel returns immediately when there is no work, allowing the Qt event loop to remain responsive.

#### The Glue Layer — ertqt.cpp

`ertqt.cpp` is the **sole C++ file** in the Qt target. It provides a pure-C API (declared in `ertqt.h`) that the rest of eRT calls. All Qt headers and C++ standard library usage are confined to this one file.

It has five main responsibilities:

1. **Lifecycle** — `ertqt_init()`, `ertqt_run()`, `ertqt_quit()` manage the QGuiApplication and QQmlApplicationEngine.

2. **Object discovery** — After QML is loaded, `rebuild_object_table()` enumerates all QObject children of the engine's root objects via `findChildren<QObject*>()`. Each entry records a `QObject*` and its `objectName`. This table is the bridge between eRT widget names and QML objects.

3. **Property access** — `ertqt_set_property_{string,int,double,bool}()` and `ertqt_get_property_*()` wrap `QObject::setProperty()`/`property()`. `ertqt_update_widget()` calls `QQuickItem::update()` to schedule a repaint.

4. **Signal binding** — `ertqt_bind_signal()` uses `QMetaObject` introspection to connect a named Qt signal (e.g. `"clicked"`, `"pressed"`, `"textChanged"`) to a C callback at runtime, without compile-time coupling to specific QML types. Both Qt5 and Qt6 use `QSignalMapper` as a bridge (Qt6 removed string-based connect with lambdas).

5. **Tick timer** — `ertqt_set_tick_callback()` manages the `QTimer::singleShot` chain that drives the eRT kernel.

#### QML Object Discovery and Widget Name Mapping

When a widget is created, `EhsTargetWidgetUi_create()` (in `target_viewport.c`) calls:

```c
ertqt_object_handle h = ertqt_get_object_by_name(widget_name);
```

This searches the object table for a QObject whose `objectName` matches `widget_name`. The widget name comes from the `.gui` parameter file — it is the first field after the version number in each widget's parameter line (e.g. `ClientIPAddress` in `ClientIPAddress,GUI_TextBox2,15,450,...`).

The returned handle (an opaque cast of the `QObject*`) is stored in `pWidget->qt_handle` and used for all subsequent property and signal operations on that widget.

**Important:** The `objectName` property in the QML file must exactly match the widget name from the `.gui` file. For example, if the `.gui` file has `ClientIPAddress`, the QML must have `objectName: "ClientIPAddress"`. If no match is found, the widget will have no Qt handle and cannot render or receive events.

#### How Properties Flow Between eRT and QML

**eRT → QML (data push):**
```
Function block update port fires (e.g. gui_text_string2_data)
  → Sets gui.data pointer to new text
  → Calls EhsWidgetUI_update() → sets bContentUpdated = TRUE
  → Calls Ehs_widget_commit() → pfDrawFunc()
    → EhsTargetWidgetUi_draw() in target_viewport.c
      → Reads data from EHS_WIDGET_UI(pWidget).data
      → Calls ertqt_set_property_string(qt_handle, "text", text)
        → QObject::setProperty("text", value) on the QML object
      → Calls ertqt_update_widget(qt_handle)
        → QQuickItem::update() schedules repaint
      → Clears bContentUpdated
```

**QML → eRT (user interaction):**
```
User clicks a QML Button
  → Qt emits pressed()/released()/clicked() signals
  → ertqt.cpp lambda fires (connected via QMetaObject at widget creation time)
    → Invokes C callback registered by target_viewport.c
      → Calls event_callback(pWidget, EHS_WIDGET_UI_EVENT_*, label, data)
        → gui_widget_event_callback() in inx-gui_widget.c
          → Populates output ports and fires EHS_FB_FINISH
```

#### QML File Conventions for eRT

The QML file (`appdata/default/app.qml`) defines the visual layout. Each QML object that eRT should control must have:

- An `objectName` property matching the widget name from the `.gui` file
- An `id` property for internal QML references (can differ from objectName)

For widgets with custom `contentItem` (e.g. Buttons with styled text), the contentItem's `text` property should bind to the parent's `text` property using the QML `id`, not the `objectName`:

```qml
Button {
    id: myButton
    objectName: "ClientIPAddress"   // must match .gui widget name
    text: "default text"

    contentItem: Text {
        text: myButton.text         // bind to parent via QML id
        color: "white"
    }
}
```

This ensures that when eRT sets the Button's `text` property via `ertqt_set_property_string()`, the contentItem's text binding updates automatically through QML's reactive property system.

QML files in the same directory as `app.qml` are automatically available as types by filename (e.g. `ApplicationFlow.qml` can be used as `ApplicationFlow {}` in QML). The app directory is added to the QML engine's import path at init time. Note that native Qt modules (e.g. `QtQuick.Timeline`) must be installed separately — they cannot be provided as QML files.

#### Known Issues and Current Debug State

**Kernel State Machine Regression:** When `EhsMainLoopSingle` was introduced, the kernel's `EHSKE_STATE_READY → EHSKE_STATE_RUNNING` transition was initially missing. Without this transition, the kernel returns without processing function blocks, so widgets are never created. The fix was to ensure `EhsMainLoopSingle` includes this state transition in the kernel code (in libehs.a, separate repo).

**Debug Tracing:** Several files currently contain `[TRACE]` printf statements for debugging the click event path. These should be removed once the click event flow is confirmed working end-to-end:
- `ertqt.cpp` — signal firing lambdas, bind functions
- `target_viewport.c` — button handlers, signal registration, widget creation
- `ertqt_button.h` — `ertqt_button_on_clicked`
- `inx-gui_widget.c` — `gui_widget_event_callback`
- `qt_main_integration.c` — heartbeat `X` in tick callback

**Widget Name Lookup:** `EhsTargetWidgetUi_create()` maps the actual widget name from the `pWidget` structure (parsed from the `.gui` file) to the QML object's `objectName`. The names must match exactly.

## Porting to a New Mode B Platform

When implementing a new Mode B target:

1. **Create library widgets** in `pfCreateFunc` — look up or create the library-native widget, store a handle on the `EhsWidgetStruct`
2. **Push property changes** in `pfDrawFunc` — check `bContentUpdated`/`bPositionUpdated`/`bColourUpdated` flags, push only what changed
3. **Bind library events** — connect the library's click/press/release/change signals to invoke `event_callback` with the appropriate `EHS_WIDGET_UI_EVENT_*` ID
4. **Implement viewport stubs** — Mode B targets typically stub out `EhsTV_blit`, `EhsTV_fillRect`, surface management, etc. since the library owns rendering
5. **You do NOT need to** implement hit-testing, manage per-widget port numbers, or handle raw mouse coordinates
