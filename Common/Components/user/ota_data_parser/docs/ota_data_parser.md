ota_data_parser
Menu: 
            System
             > OTA Tools > OTA Data Parser
Type: Data_Processor

            ┌─────────────────────────┐
     start►─┤                         ├►─-- 
      info──┤                         ├──sha256 (S)
            │                         ├►─next 
            │                         ├──payload (S)
     parse►─┤                         ├►─done 
  raw data──┤                         ├►─ready 
            │       OTA Parser        │
      size──┤                         ├──data (S)
            │                         ├──size (I)
            │                         ├──offset (I)
            │                         ├►─error 
            │                         ├──error no (I)
     abort►─┤                         ├►─-- 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (2):**
- **SHA256:** 1 (Bool) - Using SHA256 for verifying OTA
- **Chunk Size:** 128 (0 to 65536) - OTA chunk size in bytes

**Port Summary:**
- **Left:**  6 ports (3 events, 3 data)
- **Right:** 12 ports (6 events, 6 data)
