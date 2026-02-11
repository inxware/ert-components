json_parser_bool
Parse the boolean values from json string input or file.
Menu: 
            Data Utilities
             > JSON > Key Value > bool
Type: Data | Height: 215

            ┌─────────────────────────┐
     parse►─┤                         ├►─done 
 input_str──┤                         ├►─err 
            │                         ├──errno (I)
        k1──┤                         ├──o1 (B)
        k2──┤                         ├──o2 (B)
        k3──┤                         ├──o3 (B)
        k4──┤                         ├──o4 (B)
        k5──┤                         ├──o5 (B)
        k6──┤                         ├──o6 (B)
            │          JSON           │
        k7──┤                         ├──o7 (B)
        k8──┤                         ├──o8 (B)
        k9──┤                         ├──o9 (B)
       k10──┤                         ├──o10 (B)
       k11──┤                         ├──o11 (B)
       k12──┤                         ├──o12 (B)
       k13──┤                         ├──o13 (B)
       k14──┤                         ├──o14 (B)
       k15──┤                         ├──o15 (B)
       k16──┤                         ├──o16 (B)
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (17):**
- **file:** default.js (String) - JSON filename. Will be ignored if there is the str
- **k1:**  (String) - key 1
- **k2:**  (String) - key 2
- **k3:**  (String) - key 3
- **k4:**  (String) - key 4
- **k5:**  (String) - key 5
- **k6:**  (String) - key 6
- **k7:**  (String) - key 7
- **k8:**  (String) - key 8
- **k9:**  (String) - key 9
- **k10:**  (String) - key 10
- **k11:**  (String) - key 11
- **k12:**  (String) - key 12
- **k13:**  (String) - key 13
- **k14:**  (String) - key 14
- **k15:**  (String) - key 15
- **k16:**  (String) - key 16

**Port Summary:**
- **Left:**  18 ports (1 events, 17 data)
- **Right:** 19 ports (2 events, 17 data)
