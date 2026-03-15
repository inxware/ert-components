Camera
Run machine learning camera
Menu: 
            Machine Vision
             > Camera
Type: IO | Height: 130

            ┌─────────────────────────┐
     start►─┤                         ├►─-- 
        id──┤                         ├►─err 
            │                         ├──errno (I)
   capture►─┤                         ├►─-- 
            │         Camera          │
      show──┤                         ├──stream_id (I)
            │                         ├──width (I)
            │                         ├──height (I)
      stop►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (7):**
- **camera_id:**  (String) - Camera ID
- **fps:** 0 (0 to 1000) - frame rate
- **width:** 0 (0 to 10000) - width
- **height:** 0 (0 to 10000) - height
- **asynchronous:** 0 (Bool) - camera outputs frames without capture event
- **show_frame:** 0 (Bool) - Display the image with given frame
- **greyscale:** 0 (Bool) - Make the camera output greyscale. Note that this h

**Port Summary:**
- **Left:**  5 ports (3 events, 2 data)
- **Right:** 8 ports (4 events, 4 data)

**Ports by Function:**

*startCamera:*
  - **start_cam** (Start Event)
  - **device_id** (S) - Input
  - **start_errno** (I) - Output
  - **start_ok** (Finish Event)
  - **start_error** (Finish Event)

*stopCamera:*
  - **stop_cam** (Start Event)
  - **stopped** (Finish Event)

*grabFrame:*
  - **capture** (Start Event)
  - **im_show** (B) - Input
  - **frame_width** (I) - Output
  - **frame_height** (I) - Output
  - **frame_id** (I) - Output
  - **captur_ok** (Finish Event)
