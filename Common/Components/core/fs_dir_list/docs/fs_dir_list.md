FileSystemDirList1
Allows a directory path to be set and outputs files contained in the directory o
Menu: 
            File System
             > Directories > List Entries
Type: IO | Width: 65 | Height: 102

            ┌─────────────────────────┐
   set dir►─┤                         ├►─ready 
       dir──┤                         │
            │                         ├►─error 
 get entry►─┤                         ├►─file 
            │         FS List         │
            │                         ├►─dir 
            │                         ├►─no more 
            │                         ├──name (S)
   release►─┤                         ├►─ 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Directory:**  (String) - Media Directory
- **Use Application Dir:** 0 (Bool) - Find Files in Application Directory rather than th

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 7 ports (6 events, 1 data)
