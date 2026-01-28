html_java
load plain JavaScript or HTML with Javascript and run functions
Menu: 
			Languages
			 > JavaScript
Type: Data | Height: 155

            ┌─────────────────────────┐
loadScript►─┤                         ├►─loaded 
   baseURL──┤                         │
 scriptURL──┤                         │
    unLoad►─┤                         ├►─unloaded 
            │         WebKit          │
       run►─┤                         ├►─complete 
  function──┤                         ├──jsonOUT (S)
    jsonIN──┤                         │
      show►─┤                         ├►─--> 
      hide►─┤                         ├►─--> 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (6):**
- **Widget Tag:** widget - Widget Tag
- **Widget Group:** default - Widget Group
- **ScriptURL:**  (String) - direct data or URL to HTML or JavaScript
- **Data:** 1 (Bool) - use URL data as content
- **Function:** JSmain (String) - default Function to run if not specified
- **BaseURL:**  (String) - This is the base URL that would be used for cross 

**Port Summary:**
- **Left:**  9 ports (5 events, 4 data)
- **Right:** 6 ports (5 events, 1 data)
