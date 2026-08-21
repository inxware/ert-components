cell_status
Reads live link quality and metered-link accounting in one place.

In full mode 
Menu: 
            Networking
             > Cellular > Status
Type: IO | Width: 95 | Height: 198

               ┌─────────────────────────┐
         Read►─┤                         │
               │                         ├►─Done 
               │                         ├──RSRP (I)
               │                         ├──RSRQ (I)
               │                         ├──SNR (I)
               │                         ├──CELevel (I)
               │                         ├──TXPower (I)
               │                         ├──TXReps (I)
               │       Cell Status       │
               │                         ├──RXReps (I)
               │                         ├──DLPathloss (I)
               │                         ├──Energy (I)
               │                         ├──evalOk (B)
               │                         ├──TXBytes (I)
               │                         ├──RXBytes (I)
ResetCounters►─┤                         │
               │                         ├►─RDone 
               └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (1):**
- **mode:** 1 (0 to 1) - 0 reads signal strength only. 1 additionally perfo

**Port Summary:**
- **Left:**  2 ports (2 events, 0 data)
- **Right:** 14 ports (2 events, 12 data)

**Ports by Function:**

*do_read:*
  - **read** (Start Event)
  - **rsrp** (I) - Output
  - **rsrq** (I) - Output
  - **snr** (I) - Output
  - **ce_level** (I) - Output
  - **tx_power** (I) - Output
  - **tx_reps** (I) - Output
  - **rx_reps** (I) - Output
  - **dl_pathloss** (I) - Output
  - **energy_estimate** (I) - Output
  - **eval_ok** (B) - Output
  - **tx_bytes** (I) - Output
  - **rx_bytes** (I) - Output
  - **read_ok** (Finish Event)

*do_reset_counters:*
  - **reset_counters** (Start Event)
  - **reset_ok** (Finish Event)
