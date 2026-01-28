ble_service
Creates and manages a Bluetooth Low Energy (BLE) GATT service with configurable 
Menu: 
            Networking
             > Bluetooth
Type: Data_Processor | Width: 120 | Height: 620

             ┌─────────────────────────┐
       init►─┤                         ├►─init_done 
             │                         ├►─init_error 
             │                         ├──status (I)
  start_adv►─┤                         ├►─adv_started 
             │                         ├►─adv_error 
   stop_adv►─┤                         ├►─adv_stopped 
      write►─┤                         ├►─write_done 
   char_idx──┤                         ├──write_status (I)
       data──┤                         ├►─write_error 
     length──┤                         │
       read►─┤                         ├►─read_done 
   read_idx──┤                         ├──read_data (S)
             │                         ├──read_length (I)
             │                         ├──read_status (I)
             │       BLE Service       │
             │                         ├►─read_error 
     notify►─┤                         ├►─notify_sent 
 notify_idx──┤                         ├──notify_status (I)
notify_data──┤                         ├►─notify_error 
 notify_len──┤                         │
             │                         ├──wrote_idx (I)
             │                         ├──wrote_data (S)
             │                         ├──wrote_len (I)
             │                         ├►─client_wrote_evt 
             │                         ├──conn_handle (I)
             │                         ├►─connect_evt 
             │                         ├──disc_handle (I)
             │                         ├──disc_reason (I)
             │                         ├►─disconnect_evt 
             └─────────────────────────┘

             Internal Events:
               • client_wrote
               • connect
               • disconnect

Legend: ── Data | ►─ Event

**Parameters (21):**
- **service_uuid:** 0000180A-0000-1000-8000-00805F9B34FB (String) - 128-bit UUID for the BLE service (format: XXXXXXXX
- **service_name:** Custom Service (String) - Human-readable name for the service
- **num_chars:** 4 (1 to 16) - Number of characteristics in this service (1-16)
- **adv_interval_ms:** 100 (20 to 10240) - Advertising interval in milliseconds (20-10240ms).
- **mtu_size:** 23 (23 to 512) - Requested Maximum Transmission Unit size in bytes (23-512). 
- **char_0_uuid:** 00002A00-0000-1000-8000-00805F9B34FB (String) - UUID for characteristic 0
- **char_0_name:** Char 0 (String) - Name for characteristic 0
- **char_0_props:** 3 (0 to 15) - Properties for char 0: 1=Read, 2=Write, 4=Notify, 
- **char_0_max_len:** 20 (1 to 512) - Maximum data length for characteristic 0
- **char_1_uuid:** 00002A01-0000-1000-8000-00805F9B34FB (String) - UUID for characteristic 1
- **char_1_name:** Char 1 (String) - Name for characteristic 1
- **char_1_props:** 3 (0 to 15) - Properties for char 1: 1=Read, 2=Write, 4=Notify, 
- **char_1_max_len:** 20 (1 to 512) - Maximum data length for characteristic 1
- **char_2_uuid:** 00002A02-0000-1000-8000-00805F9B34FB (String) - UUID for characteristic 2
- **char_2_name:** Char 2 (String) - Name for characteristic 2
- **char_2_props:** 3 (0 to 15) - Properties for char 2: 1=Read, 2=Write, 4=Notify, 
- **char_2_max_len:** 20 (1 to 512) - Maximum data length for characteristic 2
- **char_3_uuid:** 00002A03-0000-1000-8000-00805F9B34FB (String) - UUID for characteristic 3
- **char_3_name:** Char 3 (String) - Name for characteristic 3
- **char_3_props:** 3 (0 to 15) - Properties for char 3: 1=Read, 2=Write, 4=Notify, 5=All.
- **char_3_max_len:** 20 (1 to 512) - Maximum data length for characteristic 3
TODO - add 4 more here eventually.

**Port Summary:**
- **Left:**  13 ports (6 events, 7 data)
- **Right:** 26 ports (14 events, 12 data)

**Ports by Function:**

*init:*
  - **init** (Start Event)
  - **status** (I) - Output
  - **init_done** (Finish Event)
  - **init_error** (Finish Event)

*start_adv:*
  - **start_adv** (Start Event)
  - **adv_started** (Finish Event)
  - **adv_error** (Finish Event)

*stop_adv:*
  - **stop_adv** (Start Event)
  - **adv_stopped** (Finish Event)

*write_char:*
  - **write** (Start Event)
  - **char_idx** (I) - Input
  - **data** (S) - Input
  - **length** (I) - Input
  - **write_status** (I) - Output
  - **write_done** (Finish Event)
  - **write_error** (Finish Event)

*read_char:*
  - **read** (Start Event)
  - **read_idx** (I) - Input
  - **read_data** (S) - Output
  - **read_length** (I) - Output
  - **read_status** (I) - Output
  - **read_done** (Finish Event)
  - **read_error** (Finish Event)

*notify_char:*
  - **notify** (Start Event)
  - **notify_idx** (I) - Input
  - **notify_data** (S) - Input
  - **notify_len** (I) - Input
  - **notify_status** (I) - Output
  - **notify_sent** (Finish Event)
  - **notify_error** (Finish Event)

*on_client_write:*
  - **client_wrote** (Internal Event - callable from ISR/thread)
  - **wrote_idx** (I) - Output
  - **wrote_data** (S) - Output
  - **wrote_len** (I) - Output
  - **client_wrote_evt** (Finish Event)

*on_connect:*
  - **connect** (Internal Event - callable from ISR/thread)
  - **conn_handle** (I) - Output
  - **connect_evt** (Finish Event)

*on_disconnect:*
  - **disconnect** (Internal Event - callable from ISR/thread)
  - **disc_handle** (I) - Output
  - **disc_reason** (I) - Output
  - **disconnect_evt** (Finish Event)

#TODO:
- The MTU size is not guarenteed for read or write. For payload writes we will need to generate an error of write messages are longer than the MTU size. We probably want an erro number port or a request to identify the current MTU size for the application to manage this.
- Add more charactersitics paramters to the function blocks. 
- Do we need to specificy the capability of notify/write/read for each charactersitic?
- We should change the language for write to set (or send)
- Merge Notify and write and use a boolean to decide if it is a send or notify. 
