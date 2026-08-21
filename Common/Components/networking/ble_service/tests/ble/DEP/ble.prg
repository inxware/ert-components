#V:2.1.2

CanvasSizeXY
720	443

IconData
BEGIN_BLOCK

mux_1i
_
510733	0	0	0
_
_
This is a single input integer latch.
MultiplexOneInputInt
0
_
505	-195	540	-165
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7A6B
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 504 -175 0	0	0	1	mandatory= 0	0	1
i

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	510780	0	0	-1	1
469	-230
469	-175
END_LINE
outputport

1	1	coords= 540 -175 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 504 -185 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	510780	1	2	-1	1
477	-240
477	-185
END_LINE
finishport

3	1	coords= 540 -185 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
510753	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
65	-325	130	-235
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	15	5	1.5	0	Timer
trans	-1	0	0	0
offset	0	-15
hash	0xEEFF
Instance_Info	_
End_Instance
END_BA
parameter
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	5000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= 64 -290 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 64 -280 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 64 -270 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= 64 -300 0	0	1	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 64 -250 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= 130 -270 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	510731	0	3	-1	0
END_LINE
finishport

3	0	coords= 130 -300 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= 130 -250 0	2	0	1	mandatory= 0	0	1


funcName= stop 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= tick 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
510774	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
520	-270	540	-230
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_2
trans	-1	0	0	0
offset	0	0
hash	0x0A91
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 519 -260 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	510780	0	2	-1	1
471	-250
481	-260
END_LINE
startport

2	-1	coords= 519 -240 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	510780	1	2	-1	0
451	-240
451	-240
END_LINE
finishport

3	1	coords= 532 -249 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	510731	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ble_service
_
510780	0	0	0
_
_
Creates and manages a Bluetooth Low Energy (BLE) GATT service with configurable characteristics. Supports up to 16 characteristics with read, write, notify, and indicate properties. Handles client connections, characteristic access, and notifications. Suitable for creating custom BLE peripherals on embedded devices.
ble_service
0
_
285	-270	400	103
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	5	1	0	BLE_Service
trans	-1	0	0	0
offset	0	0
hash	0xA0B5
Instance_Info	_
End_Instance
END_BA
parameter
service_uuid	3	_	_	0	0	0	0	1.2	0000180A-0000-1000-8000-00805F9B34FB
EndOfValues
EndOfLabels
128-bit UUID for the BLE service (format: XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX) or 16-bit UUID (format: 0xXXXX)

parameter
service_name	3	_	_	0	0	0	0	1.2	ServiceName
EndOfValues
EndOfLabels
Human-readable name for the service

parameter
num_chars	1	1	16	0	0	0	0	1.2	4
EndOfValues
EndOfLabels
Number of characteristics in this service (1-16)

parameter
adv_interval_ms	1	20	10240	0	0	0	0	1.2	100
EndOfValues
EndOfLabels
Advertising interval in milliseconds (20-10240ms). Standard: 100ms for fast, 1000ms for slow.

parameter
char_0_uuid	3	_	_	0	0	0	0	1.2	1000180A-0000-1000-8000-00805F9B34FB
EndOfValues
EndOfLabels
UUID for characteristic 0

parameter
char_0_name	3	_	_	0	0	0	0	1.2	Char0
EndOfValues
EndOfLabels
Name for characteristic 0

parameter
char_0_props	1	0	15	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
Properties for char 0: 1=Read, 2=Write, 4=Notify, 8=Indicate (combine with +)

parameter
char_0_max_len	1	1	512	0	0	0	0	1.2	20
EndOfValues
EndOfLabels
Maximum data length for characteristic 0

parameter
char_1_uuid	3	_	_	0	0	0	0	1.2	2A01
EndOfValues
EndOfLabels
UUID for characteristic 1

parameter
char_1_name	3	_	_	0	0	0	0	1.2	Char1
EndOfValues
EndOfLabels
Name for characteristic 1

parameter
char_1_props	1	0	15	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
Properties for char 1: 1=Read, 2=Write, 4=Notify, 8=Indicate (combine with +)

parameter
char_1_max_len	1	1	512	0	0	0	0	1.2	20
EndOfValues
EndOfLabels
Maximum data length for characteristic 1

parameter
char_2_uuid	3	_	_	0	0	0	0	1.2	2A02
EndOfValues
EndOfLabels
UUID for characteristic 2

parameter
char_2_name	3	_	_	0	0	0	0	1.2	Char2
EndOfValues
EndOfLabels
Name for characteristic 2

parameter
char_2_props	1	0	15	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
Properties for char 2: 1=Read, 2=Write, 4=Notify, 8=Indicate (combine with +)

parameter
char_2_max_len	1	1	512	0	0	0	0	1.2	20
EndOfValues
EndOfLabels
Maximum data length for characteristic 2

parameter
char_3_uuid	3	_	_	0	0	0	0	1.2	2A03
EndOfValues
EndOfLabels
UUID for characteristic 3

parameter
char_3_name	3	_	_	0	0	0	0	1.2	Char3
EndOfValues
EndOfLabels
Name for characteristic 3

parameter
char_3_props	1	0	15	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
Properties for char 3: 1=Read, 2=Write, 4=Notify, 8=Indicate (combine with +)

parameter
char_3_max_len	1	1	512	0	0	0	0	1.2	20
EndOfValues
EndOfLabels
Maximum data length for characteristic 3

inputport

0	1	coords= 284 -155 0	0	0	1	mandatory= 0	0	1
char_idx

funcName= write_char 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 284 -145 0	1	0	1	mandatory= 0	0	1
data

funcName= write_char 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 284 -135 0	2	0	1	mandatory= 0	0	1
length

funcName= write_char 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 284 -103 0	3	0	1	mandatory= 0	0	1
read_idx

funcName= read_char 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 284 -43 0	4	0	1	mandatory= 0	0	1
notify_idx

funcName= notify_char 6	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 284 -33 0	5	0	1	mandatory= 0	0	1
notify_data

funcName= notify_char 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 284 -23 0	6	0	1	mandatory= 0	0	1
notify_len

funcName= notify_char 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 400 -230 0	0	0	1	mandatory= 0	0	1
status

funcName= init 1	1	EndOfFunc
BEGIN_LINE
0	510733	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 -155 0	1	0	1	mandatory= 0	0	1
write_status

funcName= write_char 4	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 400 -103 0	2	0	1	mandatory= 0	0	1
read_data

funcName= read_char 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 -93 0	3	0	1	mandatory= 0	0	1
read_length

funcName= read_char 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 -83 0	4	0	1	mandatory= 0	0	1
read_status

funcName= read_char 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 -43 0	5	0	1	mandatory= 0	0	1
notify_status

funcName= notify_char 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 5 0	6	0	1	mandatory= 0	0	1
wrote_idx

funcName= on_client_write 7	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 400 15 0	7	0	1	mandatory= 0	0	1
wrote_data

funcName= on_client_write 7	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 25 0	8	0	1	mandatory= 0	0	1
wrote_len

funcName= on_client_write 7	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 52 0	9	0	1	mandatory= 0	0	1
conn_handle

funcName= on_connect 8	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 80 0	10	0	1	mandatory= 0	0	1
disc_handle

funcName= on_disconnect 9	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 400 90 0	11	0	1	mandatory= 0	0	1
disc_reason

funcName= on_disconnect 9	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 284 -250 0	0	0	0	mandatory= 0	0	1
init

funcName= init 1	0	EndOfFunc
BEGIN_LINE
1	510753	0	2	-1	0
190	-270
190	-250
END_LINE
startport

2	1	coords= 284 -213 0	1	0	0	mandatory= 0	0	1
start_adv

funcName= start_adv 2	0	EndOfFunc
BEGIN_LINE
1	510793	0	2	-1	1
799	-255
799	-374
213	-374
213	-213
END_LINE
startport

2	1	coords= 284 -185 0	2	0	0	mandatory= 0	0	1
stop_adv

funcName= stop_adv 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 284 -165 0	3	0	0	mandatory= 0	0	1
write

funcName= write_char 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 284 -113 0	4	0	0	mandatory= 0	0	1
read

funcName= read_char 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 284 -53 0	5	0	0	mandatory= 0	0	1
notify

funcName= notify_char 6	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= 400 -250 0	0	0	1	mandatory= 0	0	1
init_done

funcName= init 1	2	EndOfFunc
BEGIN_LINE
0	510793	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -240 0	1	0	1	mandatory= 0	0	1
init_error

funcName= init 1	3	EndOfFunc
BEGIN_LINE
0	510774	1	3	-1	0
END_LINE
finishport

3	1	coords= 400 -213 0	2	0	1	mandatory= 0	0	1
adv_started

funcName= start_adv 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -203 0	3	0	1	mandatory= 0	0	1
adv_error

funcName= start_adv 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -185 0	4	0	1	mandatory= 0	0	1
adv_stopped

funcName= stop_adv 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -165 0	5	0	1	mandatory= 0	0	1
write_done

funcName= write_char 4	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -145 0	6	0	1	mandatory= 0	0	1
write_error

funcName= write_char 4	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -113 0	7	0	1	mandatory= 0	0	1
read_done

funcName= read_char 5	5	EndOfFunc
BEGIN_LINE
0	510780	3	3	-1	0
END_LINE
finishport

3	1	coords= 400 -73 0	8	0	1	mandatory= 0	0	1
read_error

funcName= read_char 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -53 0	9	0	1	mandatory= 0	0	1
notify_sent

funcName= notify_char 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -33 0	10	0	1	mandatory= 0	0	1
notify_error

funcName= notify_char 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 -5 0	11	0	1	mandatory= 0	0	1
client_wrote_evt

funcName= on_client_write 7	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 42 0	12	0	1	mandatory= 0	0	1
connect_evt

funcName= on_connect 8	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 400 70 0	13	0	1	mandatory= 0	0	1
disconnect_evt

funcName= on_disconnect 9	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= on_client_write 7	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	1	0	1	mandatory= 0	0	1


funcName= on_connect 8	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	2	0	1	mandatory= 0	0	1


funcName= on_disconnect 9	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
510793	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
670	-310	735	-220
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	15	5	1.5	0	Timer
trans	-1	0	0	0
offset	0	-15
hash	0xEEFF
Instance_Info	_
End_Instance
END_BA
parameter
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	5000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= 669 -275 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 669 -265 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 669 -255 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= 669 -285 0	0	0	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
1	510780	0	2	-1	0
586	-250
586	-285
END_LINE
startport

2	0	coords= 669 -235 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= 735 -255 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	510780	1	3	-1	0
END_LINE
finishport

3	0	coords= 735 -285 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= 735 -235 0	2	0	1	mandatory= 0	0	1


funcName= stop 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= tick 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
