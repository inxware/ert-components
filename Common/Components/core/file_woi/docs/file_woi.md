FILE_WriteOnly_Int
Writes string data to a file. If a field separator parameter is specified (withi
Menu: 
            File System
             > File Access > File Output > int
Type: IO | Width: 65 | Height: 120

            ┌─────────────────────────┐
      open►─┤                         ├►─-- 
      path──┤                         ├►─err 
     close►─┤                         ├►─-- 
            │         File WO         │
            │                         ├►─err 
     write►─┤                         ├►─-- 
            │                         ├►─err 
      data──┤                         ├──err code (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (4):**
- **File name:** out.txt (String) - output file name
- **Field Width:** 0 (0 to 12) - Integer Field Width
- **Delimiter:** ( ) (String) - Integer Field Delimiter Character
- **Append:** 0 (Bool) - Append rather than overwrite.

**Port Summary:**
- **Left:**  5 ports (3 events, 2 data)
- **Right:** 7 ports (6 events, 1 data)
