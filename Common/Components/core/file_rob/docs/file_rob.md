FILE_ReadOnly_Bool
Read Only boolean file input reads '0'=false and '1'=true. If a field separator 
Menu: 
            File System
             > File Access > File Input > bool
Type: IO | Width: 65 | Height: 120

            ┌─────────────────────────┐
      open►─┤                         ├►─ 
      path──┤                         ├►─err 
     close►─┤                         ├►─ 
            │                         ├►─err 
            │         File RO         │
      read►─┤                         ├►─ 
     delim──┤                         ├►─err 
            │                         ├──data (B)
            │                         ├──err code (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **File name:** out.txt (String) - output file name
- **Delimiter:** ( ) (String) - Boolean Field Delimiter Character
- **Use Application Dir:** 0 (Bool) - Find Files in Application Directory rather than th

**Port Summary:**
- **Left:**  5 ports (3 events, 2 data)
- **Right:** 8 ports (6 events, 2 data)
