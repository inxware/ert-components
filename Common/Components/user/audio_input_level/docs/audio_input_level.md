audio_input_level
This function block gets the audio input amplitude. The sampling frequency, bit 
Menu: 
            AV
             > Sound > Audio Input Level
Type: IO | Height: 95

            ┌─────────────────────────┐
     start►─┤                         ├►─done 
            │                         ├►─fail 
            │       Audio Level       │
            │                         ├►─data_got 
            │                         ├──value (I)
   destroy►─┤                         ├►─destroyed 
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (3):**
- **sampling_rate:** 44100 (0 to 192000) - Sampling rate in Hz
- **bit_rate:** 8 (0 to 8) - The bit length of each sample.
- **output_period:** 1000 (100 to 3600000) - THe period of the data output in ms. Incremental s

**Port Summary:**
- **Left:**  2 ports (2 events, 0 data)
- **Right:** 5 ports (4 events, 1 data)
