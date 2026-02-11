IndexedMultiplexer_Event
Triggering Event Identified by Index Output
Menu: 
            Event
             > Combiners/Flow > Identifier
Type: Data_Processor | Width: 30 | Height: 180

            ┌─────────────────────────┐
            │                         ├►─go 
            │                         ├──ind (I)
         1►─┤                         │
         2►─┤                         │
         3►─┤                         │
            │          Ident          │
         4►─┤                         │
         5►─┤                         │
         6►─┤                         │
         7►─┤                         │
         8►─┤                         │
     reset►─┤                         ├►─ 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **Requires Resetting:** 0 (Bool) - Requires manual resetting each time it is triggere

**Port Summary:**
- **Left:**  9 ports (9 events, 0 data)
- **Right:** 3 ports (2 events, 1 data)
