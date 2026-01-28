sample2str
Storing real value samples in a string buffer.
Menu: 
            Data Utilities
             > Num > Samples to String
Type: Data_Processor

            ┌─────────────────────────┐
         i►─┤                         ├►─o 
        i1──┤                         ├──o1 (S)
            │         Sam2Str         │
        i2──┤                         ├──o2 (S)
        i3──┤                         ├──o3 (S)
     clear►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **Sample Count:** 10 (0 to 255) - Number of samples to be stored in the output strin
- **Decimal Places:** 2 (0 to 6) - Max number of decimal places

**Port Summary:**
- **Left:**  5 ports (2 events, 3 data)
- **Right:** 5 ports (2 events, 3 data)
