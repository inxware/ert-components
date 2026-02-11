StringDivader
String Divider
Menu: 
            Data Utilities
             > String Functions > Raw Data Conversion > String Divider
Type: Data | Height: 135

            ┌─────────────────────────┐
     split►─┤                         ├►─-- 
       str──┤                         ├►─err 
       len──┤                         ├──str1 (S)
            │                         ├──len1 (I)
            │                         ├──str2 (S)
            │       Str Divide        │
            │                         ├──len2 (I)
            │                         ├──str3 (S)
            │                         ├──len3 (I)
            │                         ├──str4 (S)
            │                         ├──len4 (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **delim1:**  (String) - delim1
- **delim2:**  (String) - delim2
- **delim3:**  (String) - delim3

**Port Summary:**
- **Left:**  3 ports (1 events, 2 data)
- **Right:** 10 ports (2 events, 8 data)
