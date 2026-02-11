json_stream_parser
Processes a text stream and generates events related to JSON parsing.
Menu: 
            Data Utilities
             > JSON > JSON Stream Parser
Type: Data | Height: 235

            ┌─────────────────────────┐
      read►─┤                         ├►─-- 
      data──┤                         ├──-- (S)
      next►─┤                         ├►─-- 
            │                         ├►─startMap 
            │                         ├►─endMap 
            │                         ├►─startArray 
            │                         ├►─endArray 
            │                         ├►─mapKey 
            │       JSON Parser       │
            │                         ├──Key (S)
            │                         ├──parent (S)
            │                         ├►─value 
            │                         ├──dataType (I)
            │                         ├──data (S)
            │                         ├►─error 
            │                         ├──-- (I)
       eos►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  4 ports (3 events, 1 data)
- **Right:** 16 ports (10 events, 6 data)
