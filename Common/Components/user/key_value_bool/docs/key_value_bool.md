key_value_bool
Key Value Pair Storage
Menu: 
            Data Utilities
             > Key Value Pair > bool
Type: Data | Height: 90

            ┌─────────────────────────┐
       set►─┤                         ├►─-- 
     value──┤                         ├──errno (I)
            │        Key Value        │
       get►─┤                         ├►─-- 
            │                         ├──value (B)
            │                         ├──errno (I)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **domain:** default (String) - The domain of this key value pair [Display: 30,30]
- **key:** default (String) - The key of the pair [Display: 30,45]
- **default_value:** 0 (Bool) - The default value of the pair when value is not se

**Port Summary:**
- **Left:**  3 ports (2 events, 1 data)
- **Right:** 5 ports (2 events, 3 data)
