xml_stream_parser
Parsers XML as a stream generating events for the XML chunks it sees.
Menu: 
            Data Utilities
             > XML > XML Stream Parser
Type: XML Parser | Height: 305

                 ┌─────────────────────────┐
          reset►─┤                         ├►─-- 
           read►─┤                         ├►─-- 
           data──┤                         ├──-- (S)
           next►─┤                         ├►─-- 
  ignore_script──┤                         │
ignore_entities──┤                         ├►─startElem 
                 │                         ├►─finishElem 
                 │                         ├──name (S)
                 │                         ├►─charData 
                 │                         ├──charData (S)
                 │                         ├►─attribute 
                 │       XML Parser        │
                 │                         ├──attrName (S)
                 │                         ├──attrValue (S)
                 │                         ├►─error 
                 │                         ├►─procAvail 
                 │                         ├──procTarget (S)
                 │                         ├──procInstr (S)
                 │                         ├►─comment 
                 │                         ├──comment (S)
                 │                         ├►─startCDATA 
                 │                         ├►─finishCDATA 
                 │                         ├►─allDone 
    streamEnded►─┤                         ├►─----- 
                 └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **ignore_script:** 0 (Bool) - Ignore content of script tags.
- **ignore_entities:** & (String) - Replace & before expat sees it. Try to put it back

**Port Summary:**
- **Left:**  7 ports (4 events, 3 data)
- **Right:** 22 ports (14 events, 8 data)
