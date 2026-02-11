JSONObject
Holds a memory representation of a JSON object for querying and building.
Menu: 
            Data Utilities
             > JSON > JSON Object
Type: Data | Height: 180

            ┌─────────────────────────┐
     reset►─┤                         ├►─--[reset] 
      read►─┤                         ├►─--[read] 
data[read]──┤                         │
     query►─┤                         ├►─--[query] 
            │                         ├►─error 
     path1──┤                         ├──result1 (S)
            │       JSON Object       │
     path2──┤                         ├──result2 (S)
     path3──┤                         ├──result3 (S)
     path4──┤                         ├──result4 (S)
     path5──┤                         ├──result5 (S)
     path6──┤                         ├──result6 (S)
     path7──┤                         ├──result7 (S)
     path8──┤                         ├──result8 (S)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Port Summary:**
- **Left:**  12 ports (3 events, 9 data)
- **Right:** 12 ports (4 events, 8 data)
