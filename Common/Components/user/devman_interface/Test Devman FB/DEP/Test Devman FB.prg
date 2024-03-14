#V:2.1.2

CanvasSizeXY
1870	1235

IconData
BEGIN_BLOCK

time_clock
_
1	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
-180	-600	-115	-510
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
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	1000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	1	2147483647	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	1	2147483647	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= -181 -565 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -181 -555 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -181 -545 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= -181 -575 0	0	1	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -181 -525 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -115 -545 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
finishport

3	0	coords= -115 -575 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= -115 -525 0	2	0	1	mandatory= 0	0	1


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

devman_interface
_
2	0	0	0
_
_
This component reports data to the Portal using Devman Monitor and gets the Devman settings
DevmanInterface
0
_
80	-590	190	-430
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Devman_Interface
trans	-1	0	0	0
offset	0	-15
hash	0x061C
Instance_Info	_
End_Instance
END_BA
parameter
App Name	3	0	0	1	0	0	0	1.2	_
EndOfValues
EndOfLabels
identifies data service channel

parameter
Process Name	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
Optional name of another process to monitor

inputport

0	3	coords= 79 -555 0	0	0	1	mandatory= 0	0	1
misc. info

funcName= send 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 190 -505 0	0	0	1	mandatory= 0	0	1
server URL

funcName= getServerUrl 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 190 -475 0	1	0	1	mandatory= 0	0	1
JSON

funcName= getMiscData 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 190 -445 0	2	0	1	mandatory= 0	0	1
new JSON

funcName= listen4New 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= 79 -565 0	0	0	0	mandatory= 0	0	1
send

funcName= send 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= 79 -515 0	1	0	0	mandatory= 0	0	1
get Server URL

funcName= getServerUrl 2	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-13	-545
-13	-515
END_LINE
startport

2	3	coords= 79 -455 0	2	0	0	mandatory= 0	0	1
listen Misc

funcName= listen4New 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= 79 -485 0	3	0	0	mandatory= 0	0	1
get All

funcName= getMiscData 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= 190 -565 0	0	0	1	mandatory= 0	0	1
----

funcName= send 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 190 -515 0	1	0	1	mandatory= 0	0	1
----

funcName= getServerUrl 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 190 -485 0	2	0	1	mandatory= 0	0	1
---

funcName= getMiscData 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 190 -455 0	3	0	1	mandatory= 0	0	1
new

funcName= listen4New 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
