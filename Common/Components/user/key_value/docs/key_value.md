key_value
Key Value Pair Storage
Menu: 
            Data Utilities
             > Key Value Pair > string
Type: Data | Height: 90

            ┌─────────────────────────┐
       set►─┤                         ├►─-- 
     value──┤                         ├──errno (I)
            │        Key Value        │
       get►─┤                         ├►─-- 
            │                         ├──value (S)
            │                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **domain:** default (String) - The domain of this key value pair [Display: 30,30]
- **key:** default (String) - The key of the pair [Display: 30,45]
- **default_value:**  (String) - The default value of the pair when value is not se

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 5 ports (2 events, 3 data)
