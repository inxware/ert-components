FILE_WriteOnly_Real
Writes real data to a file. If a field separator parameter is specified (within 
Menu: 
            File System
             > File Access > File Output > real
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

**Parameters (5):**
- **File name:** out.txt (String) - output file name
- **Field Width:** 0 (0 to 12) - Integer Field Width
- **Delimiter:** ( ) (String) - Real Field Delimiter Character
- **Precision:** 3 (0 to 100) - Integer Field Width
- **Append:** 0 (Bool) - Append rather than overwrite.

**Port Summary:**
- **Left:**  5 ports (3 events, 2 data)
- **Right:** 7 ports (6 events, 1 data)
