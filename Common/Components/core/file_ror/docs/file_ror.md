FILE_ReadOnly_Real
Read Only boolean file input reads '0'=false and '1'=true. If a field separator 
Menu: 
            File System
             > File Access > File Input > real
Type: IO | Width: 65 | Height: 120

            ┌─────────────────────────┐
      open►─┤                         ├►─-- 
      path──┤                         ├►─err 
     close►─┤                         ├►─-- 
            │                         ├►─err 
            │         File RO         │
      read►─┤                         ├►─-- 
     width──┤                         ├►─err 
     delim──┤                         ├──data (F)
            │                         ├──err code (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **File name:** out.txt (String) - output file name
- **Field Width:** 0 (0 to 12) - Real Field Width
- **Delimiter:** ( ) (String) - Real Field Delimiter Character
- **Use Application Dir:** 0 (Bool) - Find Files in Application Directory rather than th

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 8 ports (6 events, 2 data)
