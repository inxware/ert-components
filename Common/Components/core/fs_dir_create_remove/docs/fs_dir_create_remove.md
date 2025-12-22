FileSystemDirCreateRemove
Allows a directory to be created or removed. Parametes available to allow comple
Menu: 
            File System
             > Directories > Create Remove
Type: IO | Width: 65 | Height: 72

            ┌─────────────────────────┐
    create►─┤                         ├►─ 
      path──┤                         ├►─err 
            │        FS Cr/Rm         │
    remove►─┤                         ├►─ 
            │                         ├►─err 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Directory:**  (String) - Media Directory
- **EmptyOnly:** 1 (Bool) - Remove contents only

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 4 ports (4 events, 0 data)
