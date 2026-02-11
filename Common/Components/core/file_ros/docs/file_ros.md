FILE_ReadOnly_String
Read Only boolean file input reads '0'=false and '1'=true. If a field separator 
Menu: 
            File System
             > File Access > File Input > string
Type: IO | Width: 65 | Height: 125

            ┌─────────────────────────┐
      open►─┤                         ├►─-- 
      path──┤                         ├►─err 
     close►─┤                         ├►─-- 
            │                         ├►─err 
            │         File RO         │
      read►─┤                         ├►─-- 
     width──┤                         ├►─err 
     delim──┤                         ├──data (S)
            │                         ├──size (I)
            │                         ├──err code (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **File name:** out.txt (String) - output file name
- **Field Width:** 0 (0 to 2048) - String Field Width
- **Delimiter:** ( ) (String) - Use the form (x) where x is the delimiter
- **Use Application Dir:** 0 (Bool) - Find Files in Application Directory rather than th
- **Binary:** 0 (Bool) - Binary file mode

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 9 ports (6 events, 3 data)
