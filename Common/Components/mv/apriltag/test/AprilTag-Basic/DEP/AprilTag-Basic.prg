#V:2.1.2

CanvasSizeXY
1820	880

IconData
BEGIN_BLOCK

camera
_
2	0	0	0
_
_
Run machine learning camera
Camera
0
_
-415	-410	-305	-280
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	Camera
trans	-1	0	0	0
offset	0	-15
hash	0xC05A
Instance_Info	_
End_Instance
END_BA
parameter
camera_id	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
Camera ID

parameter
fps	1	0	100	0	0	0	0	1.2	100
EndOfValues
EndOfLabels
frame rate

parameter
width	1	0	10000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
width

parameter
height	1	0	10000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
height

parameter
asynchronous	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
camera outputs frames without capture event

inputport

0	3	coords= -416 -373 0	0	0	1	mandatory= 0	0	1
id

funcName= startCamera 1	1	EndOfFunc
BEGIN_LINE
1	267006	0	0	-1	0
-463	-394
-463	-373
END_LINE
outputport

1	1	coords= -305 -363 0	0	0	1	mandatory= 0	0	1
errno

funcName= startCamera 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -305 -325 0	1	0	1	mandatory= 0	0	1
width

funcName= grabFrame 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -305 -315 0	2	0	1	mandatory= 0	0	1
height

funcName= grabFrame 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -305 -335 0	3	0	1	mandatory= 0	0	1
stream_id

funcName= grabFrame 3	3	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
END_LINE
startport

2	3	coords= -416 -383 0	0	1	0	mandatory= 0	0	1
start

funcName= startCamera 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -416 -340 0	1	0	0	mandatory= 0	0	1
capture

funcName= grabFrame 3	0	EndOfFunc
BEGIN_LINE
1	4	0	2	-1	0
-485	-285
-485	-340
END_LINE
startport

2	1	coords= -416 -298 0	2	0	0	mandatory= 0	0	1
stop

funcName= stopCamera 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -305 -383 0	0	0	1	mandatory= 0	0	1
--

funcName= startCamera 1	3	EndOfFunc
BEGIN_LINE
0	4	0	3	-1	0
END_LINE
finishport

3	3	coords= -305 -373 0	1	0	1	mandatory= 0	0	1
err

funcName= startCamera 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -305 -345 0	2	0	1	mandatory= 0	0	1
--

funcName= grabFrame 3	4	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
finishport

3	1	coords= -305 -298 0	3	0	1	mandatory= 0	0	1
--

funcName= stopCamera 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

frame_fmt
_
3	0	0	0
_
_
Modifies camera frame format e.g. int8_3 to float32_3
FrameFormatter
0
_
-70	-425	40	-325
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	Frame_Fmt
trans	-1	0	0	0
offset	0	-15
hash	0x521F
Instance_Info	_
End_Instance
END_BA
parameter
format	1	0	10	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
format data type

inputport

0	1	coords= -71 -388 0	0	0	1	mandatory= 0	0	1
stream_id

funcName= format 1	1	EndOfFunc
BEGIN_LINE
1	2	3	0	-1	0
-160	-335
-160	-388
END_LINE
inputport

0	1	coords= -71 -360 0	1	0	1	mandatory= 0	0	1
format

funcName= format 1	2	EndOfFunc
BEGIN_LINE
1	267007	0	0	-1	0
-145	-310
-145	-360
END_LINE
outputport

1	1	coords= 40 -358 0	0	0	1	mandatory= 0	0	1
errno

funcName= format 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 40 -388 0	1	0	1	mandatory= 0	0	1
stream_id

funcName= format 1	4	EndOfFunc
BEGIN_LINE
0	1	5	1	-1	0
END_LINE
startport

2	1	coords= -71 -398 0	0	0	0	mandatory= 0	0	1
run

funcName= format 1	0	EndOfFunc
BEGIN_LINE
1	2	2	2	-1	0
-175	-345
-175	-398
END_LINE
finishport

3	1	coords= 40 -398 0	0	0	1	mandatory= 0	0	1
--

funcName= format 1	5	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
finishport

3	1	coords= 40 -368 0	1	0	1	mandatory= 0	0	1
err

funcName= format 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
4	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
-605	-340	-540	-250
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
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	33333
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= -606 -305 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -606 -295 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -606 -285 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= -606 -315 0	0	0	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
-255	-383
-255	-450
-656	-450
-656	-315
END_LINE
startport

2	0	coords= -606 -265 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -540 -285 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
finishport

3	0	coords= -540 -315 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= -540 -265 0	2	0	1	mandatory= 0	0	1


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

const_s1
_
267006	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-625	-405	-485	-384
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	0
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -485 -394 0	0	0	1	mandatory= 0	0	1	memsize= 10


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	2	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
267007	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-260	-320	-165	-300
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9647
Instance_Info	Grey Format
End_Instance
END_BA
parameter
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -165 -310 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	3	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_parser_int
_
267008	0	0	0
_
_
Parse the integers from json string input or file.
json_parser_int
0
_
315	-485	365	-270
1	0
blockattr
BEGIN_BA
type	Undefined
text	8	5	1.25	0	JSON
trans	-1	0	0	0
offset	0	-15
hash	0x43E9
Instance_Info	_
End_Instance
END_BA
parameter
file	3	_	_	0	0	0	0	1.2	default.js
EndOfValues
EndOfLabels
JSON filename. Will be ignored if there is the string input.

parameter
k1	3	_	_	0	0	0	0	1.2	n
EndOfValues
EndOfLabels
key 1

parameter
k2	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 2

parameter
k3	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 3

parameter
k4	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 4

parameter
k5	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 5

parameter
k6	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 6

parameter
k7	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 7

parameter
k8	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 8

parameter
k9	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 9

parameter
k10	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 10

parameter
k11	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 11

parameter
k12	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 12

parameter
k13	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 13

parameter
k14	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 14

parameter
k15	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 15

parameter
k16	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 16

inputport

0	3	coords= 314 -448 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	1	EndOfFunc
BEGIN_LINE
1	267122	1	0	-1	0
255	-365
255	-448
END_LINE
inputport

0	3	coords= 314 -428 0	1	0	1	mandatory= 0	0	1
k1

funcName= parse 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -418 0	2	0	1	mandatory= 0	0	1
k2

funcName= parse 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -408 0	3	0	1	mandatory= 0	0	1
k3

funcName= parse 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -398 0	4	0	1	mandatory= 0	0	1
k4

funcName= parse 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -388 0	5	0	1	mandatory= 0	0	1
k5

funcName= parse 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -378 0	6	0	1	mandatory= 0	0	1
k6

funcName= parse 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -368 0	7	0	1	mandatory= 0	0	1
k7

funcName= parse 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -358 0	8	0	1	mandatory= 0	0	1
k8

funcName= parse 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -348 0	9	0	1	mandatory= 0	0	1
k9

funcName= parse 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -338 0	10	0	1	mandatory= 0	0	1
k10

funcName= parse 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -328 0	11	0	1	mandatory= 0	0	1
k11

funcName= parse 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -318 0	12	0	1	mandatory= 0	0	1
k12

funcName= parse 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -308 0	13	0	1	mandatory= 0	0	1
k13

funcName= parse 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -298 0	14	0	1	mandatory= 0	0	1
k14

funcName= parse 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -288 0	15	0	1	mandatory= 0	0	1
k15

funcName= parse 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -278 0	16	0	1	mandatory= 0	0	1
k16

funcName= parse 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 365 -438 0	0	0	1	mandatory= 0	0	1
errno

funcName= parse 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -428 0	1	0	1	mandatory= 0	0	1
o1

funcName= parse 1	19	EndOfFunc
BEGIN_LINE
0	267013	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -418 0	2	0	1	mandatory= 0	0	1
o2

funcName= parse 1	20	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -408 0	3	0	1	mandatory= 0	0	1
o3

funcName= parse 1	21	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -398 0	4	0	1	mandatory= 0	0	1
o4

funcName= parse 1	22	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -388 0	5	0	1	mandatory= 0	0	1
o5

funcName= parse 1	23	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -378 0	6	0	1	mandatory= 0	0	1
o6

funcName= parse 1	24	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -368 0	7	0	1	mandatory= 0	0	1
o7

funcName= parse 1	25	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -358 0	8	0	1	mandatory= 0	0	1
o8

funcName= parse 1	26	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -348 0	9	0	1	mandatory= 0	0	1
o9

funcName= parse 1	27	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -338 0	10	0	1	mandatory= 0	0	1
o10

funcName= parse 1	28	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -328 0	11	0	1	mandatory= 0	0	1
o11

funcName= parse 1	29	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -318 0	12	0	1	mandatory= 0	0	1
o12

funcName= parse 1	30	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -308 0	13	0	1	mandatory= 0	0	1
o13

funcName= parse 1	31	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -298 0	14	0	1	mandatory= 0	0	1
o14

funcName= parse 1	32	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -288 0	15	0	1	mandatory= 0	0	1
o15

funcName= parse 1	33	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 365 -278 0	16	0	1	mandatory= 0	0	1
o16

funcName= parse 1	34	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= 314 -458 0	0	0	0	mandatory= 0	0	1


funcName= parse 1	0	EndOfFunc
BEGIN_LINE
1	267122	1	2	-1	0
265	-395
265	-458
END_LINE
finishport

3	3	coords= 365 -458 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	35	EndOfFunc
BEGIN_LINE
0	267010	0	3	-1	0
END_LINE
finishport

3	3	coords= 365 -448 0	1	0	1	mandatory= 0	0	1
err

funcName= parse 1	36	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_parser_real
_
267009	0	0	0
_
_
Parse the float numbers from json string input or file.
json_parser_real
0
_
315	-215	365	0
1	0
blockattr
BEGIN_BA
type	Undefined
text	8	5	1.25	0	JSON
trans	-1	0	0	0
offset	0	-15
hash	0x15FB
Instance_Info	_
End_Instance
END_BA
parameter
file	3	_	_	0	0	0	0	1.2	default.js
EndOfValues
EndOfLabels
JSON filename. Will be ignored if there is the string input.

parameter
k1	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 1

parameter
k2	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 2

parameter
k3	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 3

parameter
k4	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 4

parameter
k5	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 5

parameter
k6	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 6

parameter
k7	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 7

parameter
k8	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 8

parameter
k9	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 9

parameter
k10	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 10

parameter
k11	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 11

parameter
k12	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 12

parameter
k13	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 13

parameter
k14	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 14

parameter
k15	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 15

parameter
k16	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
key 16

inputport

0	3	coords= 314 -178 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	1	EndOfFunc
BEGIN_LINE
1	267122	1	0	-1	0
260	-365
260	-178
END_LINE
inputport

0	3	coords= 314 -158 0	1	0	1	mandatory= 0	0	1
k1

funcName= parse 1	2	EndOfFunc
BEGIN_LINE
1	267011	0	0	-1	0
235	-225
235	-158
END_LINE
inputport

0	3	coords= 314 -148 0	2	0	1	mandatory= 0	0	1
k2

funcName= parse 1	3	EndOfFunc
BEGIN_LINE
1	267026	0	0	-1	0
235	-170
235	-148
END_LINE
inputport

0	3	coords= 314 -138 0	3	0	1	mandatory= 0	0	1
k3

funcName= parse 1	4	EndOfFunc
BEGIN_LINE
1	267057	0	0	-1	0
240	-120
240	-138
END_LINE
inputport

0	3	coords= 314 -128 0	4	0	1	mandatory= 0	0	1
k4

funcName= parse 1	5	EndOfFunc
BEGIN_LINE
1	267076	0	0	-1	0
240	-65
240	-128
END_LINE
inputport

0	3	coords= 314 -118 0	5	0	1	mandatory= 0	0	1
k5

funcName= parse 1	6	EndOfFunc
BEGIN_LINE
1	267078	0	0	-1	0
240	-15
240	-118
END_LINE
inputport

0	3	coords= 314 -108 0	6	0	1	mandatory= 0	0	1
k6

funcName= parse 1	7	EndOfFunc
BEGIN_LINE
1	267101	0	0	-1	0
240	40
240	-108
END_LINE
inputport

0	3	coords= 314 -98 0	7	0	1	mandatory= 0	0	1
k7

funcName= parse 1	8	EndOfFunc
BEGIN_LINE
1	267103	0	0	-1	0
240	90
240	-98
END_LINE
inputport

0	3	coords= 314 -88 0	8	0	1	mandatory= 0	0	1
k8

funcName= parse 1	9	EndOfFunc
BEGIN_LINE
1	267105	0	0	-1	0
240	145
240	-88
END_LINE
inputport

0	3	coords= 314 -78 0	9	0	1	mandatory= 0	0	1
k9

funcName= parse 1	10	EndOfFunc
BEGIN_LINE
1	267107	0	0	-1	0
240	195
240	-78
END_LINE
inputport

0	3	coords= 314 -68 0	10	0	1	mandatory= 0	0	1
k10

funcName= parse 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -58 0	11	0	1	mandatory= 0	0	1
k11

funcName= parse 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -48 0	12	0	1	mandatory= 0	0	1
k12

funcName= parse 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -38 0	13	0	1	mandatory= 0	0	1
k13

funcName= parse 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -28 0	14	0	1	mandatory= 0	0	1
k14

funcName= parse 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -18 0	15	0	1	mandatory= 0	0	1
k15

funcName= parse 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 314 -8 0	16	0	1	mandatory= 0	0	1
k16

funcName= parse 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 365 -168 0	0	0	1	mandatory= 0	0	1
errno

funcName= parse 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -158 0	1	0	1	mandatory= 0	0	1
o1

funcName= parse 1	19	EndOfFunc
BEGIN_LINE
0	267114	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -148 0	2	0	1	mandatory= 0	0	1
o2

funcName= parse 1	20	EndOfFunc
BEGIN_LINE
0	267114	1	1	-1	0
END_LINE
outputport

1	2	coords= 365 -138 0	3	0	1	mandatory= 0	0	1
o3

funcName= parse 1	21	EndOfFunc
BEGIN_LINE
0	267114	2	1	-1	0
END_LINE
outputport

1	2	coords= 365 -128 0	4	0	1	mandatory= 0	0	1
o4

funcName= parse 1	22	EndOfFunc
BEGIN_LINE
0	267114	3	1	-1	0
END_LINE
outputport

1	2	coords= 365 -118 0	5	0	1	mandatory= 0	0	1
o5

funcName= parse 1	23	EndOfFunc
BEGIN_LINE
0	267114	4	1	-1	0
END_LINE
outputport

1	2	coords= 365 -108 0	6	0	1	mandatory= 0	0	1
o6

funcName= parse 1	24	EndOfFunc
BEGIN_LINE
0	267114	5	1	-1	0
END_LINE
outputport

1	2	coords= 365 -98 0	7	0	1	mandatory= 0	0	1
o7

funcName= parse 1	25	EndOfFunc
BEGIN_LINE
0	267114	6	1	-1	0
END_LINE
outputport

1	2	coords= 365 -88 0	8	0	1	mandatory= 0	0	1
o8

funcName= parse 1	26	EndOfFunc
BEGIN_LINE
0	267115	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -78 0	9	0	1	mandatory= 0	0	1
o9

funcName= parse 1	27	EndOfFunc
BEGIN_LINE
0	267115	1	1	-1	0
END_LINE
outputport

1	2	coords= 365 -68 0	10	0	1	mandatory= 0	0	1
o10

funcName= parse 1	28	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -58 0	11	0	1	mandatory= 0	0	1
o11

funcName= parse 1	29	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -48 0	12	0	1	mandatory= 0	0	1
o12

funcName= parse 1	30	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -38 0	13	0	1	mandatory= 0	0	1
o13

funcName= parse 1	31	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -28 0	14	0	1	mandatory= 0	0	1
o14

funcName= parse 1	32	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -18 0	15	0	1	mandatory= 0	0	1
o15

funcName= parse 1	33	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= 365 -8 0	16	0	1	mandatory= 0	0	1
o16

funcName= parse 1	34	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= 314 -188 0	0	0	0	mandatory= 0	0	1


funcName= parse 1	0	EndOfFunc
BEGIN_LINE
1	267113	0	2	-1	0
267	-180
267	-188
END_LINE
finishport

3	3	coords= 365 -188 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	35	EndOfFunc
BEGIN_LINE
0	267115	0	3	-1	0
END_LINE
finishport

3	3	coords= 365 -178 0	1	0	1	mandatory= 0	0	1
err

funcName= parse 1	36	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_equibx1
_
267010	0	0	0
_
_
This is a 2-input integer comparator.
ComparatorEqualInt1
0
_
420	-485	470	-425
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	12	5	1.25	0	A=B
trans	-1	0	0	0
offset	0	-15
hash	0x458D
Instance_Info	_
End_Instance
END_BA
parameter
Constant	1	-2147483648	2147483647	1	12	38	0	1.2	0
EndOfValues
EndOfLabels
Comparator constant

inputport

0	1	coords= 419 -450 0	0	0	1	mandatory= 1	0	1


funcName= Run_ComparatorEqualInt 1	1	EndOfFunc
BEGIN_LINE
1	267008	1	0	-1	0
387	-428
387	-450
END_LINE
inputport

0	1	coords= 419 -440 0	1	0	1	mandatory= 1	0	1


funcName= Run_ComparatorEqualInt 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	coords= 470 -440 0	0	0	1	mandatory= 0	0	1


funcName= Run_ComparatorEqualInt 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= 419 -460 0	0	0	0	mandatory= 0	0	1


funcName= Run_ComparatorEqualInt 1	0	EndOfFunc
BEGIN_LINE
1	267008	0	2	-1	0
370	-458
370	-460
END_LINE
finishport

3	0	coords= 470 -460 0	0	0	1	mandatory= 0	0	1
t

funcName= Run_ComparatorEqualInt 1	4	EndOfFunc
BEGIN_LINE
0	267118	0	3	-1	0
END_LINE
finishport

3	3	coords= 470 -450 0	0	0	1	mandatory= 0	0	1
f

funcName= Run_ComparatorEqualInt 1	5	EndOfFunc
BEGIN_LINE
0	267013	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267011	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	-250	70	-205
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 -225 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267012	0	0	-1	0
-10	-224
-10	-225
END_LINE
inputport

0	3	coords= 39 -215 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	-275
-16	-275
-16	-215
END_LINE
outputport

1	3	coords= 70 -225 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	1	1	-1	0
END_LINE
startport

2	3	coords= 39 -235 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	-280
-21	-280
-21	-235
END_LINE
finishport

3	3	coords= 70 -235 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267111	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267012	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	-235	-30	-214
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	id
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 -224 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
267013	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
555	-380	585	-350
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0x08B6
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 554 -360 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	1	EndOfFunc
BEGIN_LINE
1	267109	0	0	-1	0
END_LINE
outputport

1	3	coords= 585 -360 0	0	0	1	mandatory= 0	0	1	memsize= 5

n_detected
funcName= Run_ConvertorIntToString 1	2	EndOfFunc
BEGIN_LINE
0	267107	1	1	-1	0
END_LINE
startport

2	1	coords= 554 -370 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	0	EndOfFunc
BEGIN_LINE
1	267109	0	2	-1	0
END_LINE
finishport

3	0	coords= 585 -370 0	1	0	1	mandatory= 0	0	1

detected
funcName= Run_ConvertorIntToString 1	3	EndOfFunc
BEGIN_LINE
0	267107	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267026	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	-195	70	-150
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 -170 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267027	0	0	-1	0
-10	-169
-10	-170
END_LINE
inputport

0	3	coords= 39 -160 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	-220
-16	-220
-16	-160
END_LINE
outputport

1	3	coords= 70 -170 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	2	1	-1	0
END_LINE
startport

2	3	coords= 39 -180 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	-225
-21	-225
-21	-180
END_LINE
finishport

3	3	coords= 70 -180 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267111	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267027	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	-180	-30	-159
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	xlt
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 -169 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267057	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	-145	70	-100
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 -120 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267058	0	0	-1	0
-10	-119
-10	-120
END_LINE
inputport

0	3	coords= 39 -110 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	-170
-16	-170
-16	-110
END_LINE
outputport

1	3	coords= 70 -120 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	3	1	-1	0
END_LINE
startport

2	3	coords= 39 -130 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	-175
-21	-175
-21	-130
END_LINE
finishport

3	3	coords= 70 -130 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267111	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267058	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	-130	-30	-109
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ylt
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 -119 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267076	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	-90	70	-45
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 -65 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267077	0	0	-1	0
-10	-64
-10	-65
END_LINE
inputport

0	3	coords= 39 -55 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	-115
-16	-115
-16	-55
END_LINE
outputport

1	3	coords= 70 -65 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	4	1	-1	0
END_LINE
startport

2	3	coords= 39 -75 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	-120
-21	-120
-21	-75
END_LINE
finishport

3	3	coords= 70 -75 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267111	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267077	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	-75	-30	-54
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	xrt
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 -64 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267078	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	-40	70	5
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 -15 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267079	0	0	-1	0
-10	-14
-10	-15
END_LINE
inputport

0	3	coords= 39 -5 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	-65
-16	-65
-16	-5
END_LINE
outputport

1	3	coords= 70 -15 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	5	1	-1	0
END_LINE
startport

2	3	coords= 39 -25 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	-70
-21	-70
-21	-25
END_LINE
finishport

3	3	coords= 70 -25 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267112	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267079	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	-25	-30	-4
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	yrt
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 -14 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267101	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	15	70	60
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 40 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267102	0	0	-1	0
-10	41
-10	40
END_LINE
inputport

0	3	coords= 39 50 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	-10
-16	-10
-16	50
END_LINE
outputport

1	3	coords= 70 40 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	6	1	-1	0
END_LINE
startport

2	3	coords= 39 30 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	-15
-21	-15
-21	30
END_LINE
finishport

3	3	coords= 70 30 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267112	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267102	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	30	-30	51
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	xrb
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 41 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267103	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	65	70	110
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 90 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267104	0	0	-1	0
-10	91
-10	90
END_LINE
inputport

0	3	coords= 39 100 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	40
-16	40
-16	100
END_LINE
outputport

1	3	coords= 70 90 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	7	1	-1	0
END_LINE
startport

2	3	coords= 39 80 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	35
-21	35
-21	80
END_LINE
finishport

3	3	coords= 70 80 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267112	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267104	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	80	-30	101
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	yrb
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 91 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267105	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	120	70	165
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 145 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267106	0	0	-1	0
-10	146
-10	145
END_LINE
inputport

0	3	coords= 39 155 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	95
-16	95
-16	155
END_LINE
outputport

1	3	coords= 70 145 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	8	1	-1	0
END_LINE
startport

2	3	coords= 39 135 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	90
-21	90
-21	135
END_LINE
finishport

3	3	coords= 70 135 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267112	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267106	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	135	-30	156
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	xlb
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 146 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
267107	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
40	170	70	215
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 39 195 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	267108	0	0	-1	0
-10	196
-10	195
END_LINE
inputport

0	3	coords= 39 205 0	1	0	1	mandatory= 0	0	1
s2
n_detected
funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	267013	0	0	-1	0
630	-360
630	145
-16	145
-16	205
END_LINE
outputport

1	3	coords= 70 195 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	267009	9	1	-1	0
END_LINE
startport

2	3	coords= 39 185 0	0	0	0	mandatory= 0	0	1

detected
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	267013	0	2	-1	0
625	-370
625	140
-21	140
-21	185
END_LINE
finishport

3	3	coords= 70 185 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	267113	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267108	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-170	185	-30	206
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ylb
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -30 196 0	0	0	1	mandatory= 0	0	1	memsize= 3


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267011	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_subix
_
267109	0	0	0
_
_
This is a 2-input integer subtractor.
SubtractionInt
0
_
490	-380	520	-335
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	3	1.25	0	a-b
trans	-1	0	0	0
offset	0	0
hash	0xD06E
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 489 -360 0	0	0	1	mandatory= 1	0	1
a

funcName= Run_SubtractionInt 1	1	EndOfFunc
BEGIN_LINE
1	267008	1	0	-1	0
464	-428
464	-360
END_LINE
inputport

0	1	coords= 489 -350 0	1	0	1	mandatory= 1	0	1
b

funcName= Run_SubtractionInt 1	2	EndOfFunc
BEGIN_LINE
1	267110	0	0	-1	0
END_LINE
outputport

1	1	coords= 520 -360 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubtractionInt 1	3	EndOfFunc
BEGIN_LINE
0	267013	0	1	-1	0
END_LINE
startport

2	3	coords= 489 -370 0	0	0	0	mandatory= 0	0	1


funcName= Run_SubtractionInt 1	0	EndOfFunc
BEGIN_LINE
1	267010	1	2	-1	0
480	-450
480	-370
END_LINE
finishport

3	1	coords= 520 -370 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubtractionInt 1	4	EndOfFunc
BEGIN_LINE
0	267013	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
267110	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
375	-360	470	-340
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9647
Instance_Info	_
End_Instance
END_BA
parameter
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= 470 -350 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	267109	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand4
_
267111	0	0	0
_
_
This is a 4-input event latching AND with an internal reset.
EventSelfResetFourInputLatchingAnd
0
_
160	-235	180	-170
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_4
trans	-1	0	0	0
offset	0	0
hash	0x774B
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 159 -225 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	267011	0	2	-1	0
134	-235
134	-225
END_LINE
startport

2	-1	coords= 159 -210 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	267026	0	2	-1	0
100	-180
100	-210
END_LINE
startport

2	-1	coords= 159 -195 0	2	0	0	mandatory= 1	0	1


funcName= And3 3	0	EndOfFunc
BEGIN_LINE
1	267057	0	2	-1	0
105	-130
105	-195
END_LINE
startport

2	-1	coords= 159 -180 0	3	0	0	mandatory= 0	0	1


funcName= And4 4	0	EndOfFunc
BEGIN_LINE
1	267076	0	2	-1	0
119	-75
119	-180
END_LINE
finishport

3	-1	coords= 166 -202 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	funcName= And3 3	1	funcName= And4 4	1	EndOfFunc
BEGIN_LINE
0	267113	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand4
_
267112	0	0	0
_
_
This is a 4-input event latching AND with an internal reset.
EventSelfResetFourInputLatchingAnd
0
_
160	-10	180	55
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_4
trans	-1	0	0	0
offset	0	0
hash	0x774B
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 159 0 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	267078	0	2	-1	0
134	-25
134	0
END_LINE
startport

2	-1	coords= 159 15 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	267101	0	2	-1	0
129	30
129	15
END_LINE
startport

2	-1	coords= 159 30 0	2	0	0	mandatory= 1	0	1


funcName= And3 3	0	EndOfFunc
BEGIN_LINE
1	267103	0	2	-1	0
90	80
90	30
END_LINE
startport

2	-1	coords= 159 45 0	3	0	0	mandatory= 0	0	1


funcName= And4 4	0	EndOfFunc
BEGIN_LINE
1	267105	0	2	-1	0
119	135
119	45
END_LINE
finishport

3	-1	coords= 166 23 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	funcName= And3 3	1	funcName= And4 4	1	EndOfFunc
BEGIN_LINE
0	267113	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand3
_
267113	0	0	0
_
_
This is a 3-input event latching AND with an internal reset.
EventSelfResetThreeInputLatchingAnd
0
_
235	-205	255	-155
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_3
trans	-1	0	0	0
offset	0	0
hash	0x11D7
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 234 -195 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	267111	0	2	-1	0
186	-202
186	-195
END_LINE
startport

2	-1	coords= 234 -180 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	267112	0	2	-1	0
186	23
186	-180
END_LINE
startport

2	-1	coords= 234 -165 0	2	0	0	mandatory= 1	0	1


funcName= And3 3	0	EndOfFunc
BEGIN_LINE
1	267107	0	2	-1	0
199	185
199	-165
END_LINE
finishport

3	-1	coords= 241 -180 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	funcName= And3 3	1	EndOfFunc
BEGIN_LINE
0	267009	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
267114	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
545	-200	595	-55
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x1627
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	ID: %.0f\nLT: (%.2f , %.2f)\nRT: (%.2f , %.2f)\nRB: (%.2f , %.2f)
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 544 -178 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	267009	1	0	-1	0
514	-158
514	-178
END_LINE
inputport

0	2	coords= 544 -168 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	267009	2	0	-1	0
509	-148
509	-168
END_LINE
inputport

0	2	coords= 544 -158 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	267009	3	0	-1	0
454	-138
454	-158
END_LINE
inputport

0	2	coords= 544 -148 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	267009	4	0	-1	0
499	-128
499	-148
END_LINE
inputport

0	2	coords= 544 -138 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	267009	5	0	-1	0
410	-118
410	-138
END_LINE
inputport

0	2	coords= 544 -128 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	267009	6	0	-1	0
469	-108
469	-128
END_LINE
inputport

0	2	coords= 544 -118 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	267009	7	0	-1	0
420	-98
420	-118
END_LINE
inputport

0	2	coords= 544 -108 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	8	0	-1	0
END_LINE
inputport

0	3	coords= 544 -88 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 595 -178 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	267116	0	1	-1	0
END_LINE
startport

2	3	coords= 544 -188 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	267009	0	2	-1	0
469	-188
469	-188
END_LINE
finishport

3	3	coords= 595 -188 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	267116	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
267115	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
545	-55	595	90
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x1627
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	LB: (%.2f , %.2f)
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 544 -33 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	267009	8	0	-1	0
479	-88
479	-33
END_LINE
inputport

0	2	coords= 544 -23 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	267009	9	0	-1	0
470	-78
470	-23
END_LINE
inputport

0	2	coords= 544 -13 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 544 -3 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 544 7 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 544 17 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 544 27 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 544 37 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 544 57 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 595 -33 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	267116	1	1	-1	0
END_LINE
startport

2	3	coords= 544 -43 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	267009	0	2	-1	0
519	-188
519	-43
END_LINE
finishport

3	3	coords= 595 -43 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
267116	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
675	-200	725	-160
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	format
trans	-1	0	0	0
offset	0	-2
hash	0xDF0A
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s\n%s
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= 674 -178 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	267114	0	0	-1	0
624	-178
624	-178
END_LINE
inputport

0	3	coords= 674 -168 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	267115	0	0	-1	0
644	-33
644	-168
END_LINE
outputport

1	3	coords= 725 -178 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	267118	1	1	-1	0
END_LINE
startport

2	3	coords= 674 -188 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	267114	0	2	-1	0
624	-188
624	-188
END_LINE
finishport

3	3	coords= 725 -188 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	267118	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
267117	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
1080	-260	1195	-45
1	0
blockattr
BEGIN_BA
type	IO
text	22	5	1.25	0	Text_UI
trans	-1	0	0	0
offset	0	-15
hash	0x150B
Instance_Info	_
End_Instance
END_BA
parameter
Widget Tag	5	0	0	1	40	200	0	1.2	widget0
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	0	0	0	0	0	0	1.2	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Always Clickable	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	coords= 1079 -103 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1079 -93 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1079 -83 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1079 -73 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1079 -63 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 1079 -143 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	267118	0	0	-1	0
995	-205
995	-143
END_LINE
inputport

0	3	coords= 1079 -133 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 1195 -103 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1195 -93 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1195 -83 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1195 -73 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 1195 -143 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 1195 -133 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= 1079 -235 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1079 -225 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1079 -210 0	2	0	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
1	267117	0	2	-1	0
1245	-235
1245	-300
1029	-300
1029	-210
END_LINE
startport

2	0	coords= 1079 -200 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1079 -115 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 1079 -155 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	267118	0	2	-1	0
1054	-215
1054	-155
END_LINE
finishport

3	1	coords= 1195 -235 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	267117	2	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -225 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -210 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -200 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -115 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -185 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -175 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1195 -155 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 1195 -165 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
267118	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
920	-240	950	-170
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x0094
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 919 -205 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	267119	0	0	-1	0
894	-209
894	-205
END_LINE
inputport

0	3	coords= 919 -180 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	267116	0	0	-1	0
750	-178
750	-180
END_LINE
outputport

1	3	coords= 950 -205 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	267117	5	1	-1	0
END_LINE
startport

2	3	coords= 919 -215 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	267010	0	2	-1	0
894	-460
894	-215
END_LINE
startport

2	3	coords= 919 -190 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	267116	0	2	-1	0
745	-188
745	-190
END_LINE
finishport

3	3	coords= 950 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	267117	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
267119	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
645	-220	785	-199
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	No Tags found!
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 785 -209 0	0	0	1	mandatory= 0	0	1	memsize= 20


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	267118	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
267120	0	0	0
_
_
<nl?>        
xcomment
0
_
490	-565	734	-496
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Always Output the Detection data of\nthe last detected tag
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

apriltag
_
267122	0	0	0
_
_
Detect all apriltags with their IDs and coordinates of 4 corners in the image.
apriltag
0
_
115	-495	225	-340
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	AprilTag
trans	-1	0	0	0
offset	0	-15
hash	0xC8D5
Instance_Info	_
End_Instance
END_BA
parameter
apriltag_family	1	0	7	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The AprilTag family to be detected. The enumeration is: 0 - tag36h11, 1 - tag25h9, 2 - tag16h5, 3 - tagCircle21h7, 4 - tagCircle49h12, 5 - tagStandard41h12, 6 - tagStandard52h13, 7 - tagCustom48h12.

parameter
quad_decimate	2	1	25	0	0	0	0	1.2	2
EndOfValues
EndOfLabels
Detection of quads can be done on a lower-resolution image, improving speed at a cost of pose accuracy and a slight decrease in detection rate. Decoding the binary payload is still done at full resolution.

parameter
quad_sigma	2	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
What Gaussian blur should be applied to the segmented image (used for quad detection?)  Parameter is the standard deviation in pixels.  Very noisy images benefit from non-zero values (e.g. 0.8).

parameter
refine_edges	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
When true, the edges of the each quad are adjusted to "snap to" strong gradients nearby. This is useful when decimation is employed, as it can increase the quality of the initial quad estimate substantially. Generally recommended to be on (true).

parameter
decode_sharpening	2	0	1	0	0	0	0	1.2	0.25
EndOfValues
EndOfLabels
How much sharpening should be done to decoded images? This can help decode small tags but may or may not help in odd lighting conditions or low light conditions.

inputport

0	2	coords= 114 -435 0	0	0	1	mandatory= 0	0	1
sigma

funcName= config 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 114 -445 0	1	0	1	mandatory= 0	0	1
decimate

funcName= config 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 114 -375 0	2	0	1	mandatory= 0	0	1
sharpen

funcName= config 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 114 -425 0	3	0	1	mandatory= 0	0	1
refine_edge

funcName= config 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 114 -455 0	4	0	1	mandatory= 0	0	1
family

funcName= config 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 114 -385 0	5	0	1	mandatory= 1	0	1
stream_id

funcName= detect 2	1	EndOfFunc
BEGIN_LINE
1	3	1	0	-1	0
85	-388
85	-385
END_LINE
outputport

1	1	coords= 225 -375 0	0	0	1	mandatory= 0	0	1
errno

funcName= detect 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 225 -365 0	1	0	1	mandatory= 1	0	1
json

funcName= detect 2	3	EndOfFunc
BEGIN_LINE
0	267009	0	1	-1	0
END_LINE
startport

2	2	coords= 114 -465 0	0	0	0	mandatory= 0	0	1
config

funcName= config 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 114 -395 0	1	0	0	mandatory= 1	0	1
detect

funcName= detect 2	0	EndOfFunc
BEGIN_LINE
1	3	0	2	-1	0
94	-398
94	-395
END_LINE
finishport

3	2	coords= 225 -465 0	0	0	1	mandatory= 0	0	1
--

funcName= config 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 225 -395 0	1	0	1	mandatory= 0	0	1
--

funcName= detect 2	4	EndOfFunc
BEGIN_LINE
0	267009	0	3	-1	0
END_LINE
finishport

3	1	coords= 225 -385 0	2	0	1	mandatory= 0	0	1
err

funcName= detect 2	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
