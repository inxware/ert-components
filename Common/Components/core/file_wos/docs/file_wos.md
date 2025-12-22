FILE_WriteOnly_String
Writes string data to a file. If a field separator parameter is specified (withi
Menu: 
            File System
             > File Access > File Output > string
Type: IO | Width: 65 | Height: 120

            ┌─────────────────────────┐
      open►─┤                         ├►─-- 
      path──┤                         ├►─err 
     close►─┤                         ├►─-- 
            │                         ├►─err 
            │         File WO         │
     write►─┤                         ├►─-- 
            │                         ├►─err 
      data──┤                         ├──err code (I)
      size──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (5):**
- **File name:** out.txt (String) - output file name
- **Field Width:** 0 (0 to 2048) - Data Field Width (space padded)
- **Delimiter:** ( ) (String) - Field Delimiter Character
- **Append:** 0 (Bool) - Append rather than overwrite.
- **Binary:** 0 (Bool) - Binary file mode

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 7 ports (6 events, 1 data)
