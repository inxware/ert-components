#V:2.1.2

CanvasSizeXY
1335	1220

IconData
BEGIN_BLOCK

inx-netsocket
_
2	0	0	0
_
_
provides tcpip and udp socket data comms.
netSocket
0
_
225	100	330	280
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.5	0	Sck_Client
trans	-1	0	0	0
offset	0	-15
hash	0xF78A
Instance_Info	_
End_Instance
END_BA
parameter
URL	3	0	256	0	0	0	0	1.2	127.0.0.1
EndOfValues
EndOfLabels
URL of the server socket. N/A for sockets

parameter
port	1	0	65535	0	0	0	0	1.2	8021
EndOfValues
EndOfLabels
IP socket number to open

parameter
tcp/udp	1	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
0=tcp, 1=udp

inputport

0	1	coords= 224 145 0	0	0	1	mandatory= 0	0	1
port

funcName= open 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 224 135 0	1	0	1	mandatory= 0	0	1
url

funcName= open 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 224 245 0	2	0	1	mandatory= 0	0	1
data

funcName= send 3	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	1
212	341
212	245
END_LINE
inputport

0	1	coords= 224 255 0	3	0	1	mandatory= 0	0	1
size

funcName= send 3	2	EndOfFunc
BEGIN_LINE
1	20	0	0	-1	0
192	265
202	255
END_LINE
outputport

1	1	coords= 330 255 0	0	0	1	mandatory= 0	0	1
errno

funcName= open 1	3	funcName= close 2	1	funcName= send 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 330 205 0	1	0	1	mandatory= 0	0	1
data

funcName= open 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 330 215 0	2	0	1	mandatory= 0	0	1
size

funcName= open 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 224 125 0	0	0	1	mandatory= 0	0	1
open

funcName= open 1	0	EndOfFunc
BEGIN_LINE
1	10	5	2	-1	1
1020	450
1020	360
179	360
179	125
END_LINE
startport

2	1	coords= 224 165 0	2	0	1	mandatory= 0	0	1
close

funcName= close 2	0	EndOfFunc
BEGIN_LINE
1	21	0	2	-1	0
477	326
477	80
164	80
164	165
END_LINE
startport

2	1	coords= 224 235 0	2	0	1	mandatory= 0	0	1
send

funcName= send 3	0	EndOfFunc
BEGIN_LINE
1	2	2	2	-1	1
395	125
395	65
159	65
159	235
END_LINE
finishport

3	1	coords= 330 175 0	0	0	1	mandatory= 0	0	1
error

funcName= close 2	2	funcName= open 1	6	EndOfFunc
BEGIN_LINE
0	11	3	3	-1	0
END_LINE
finishport

3	1	coords= 330 245 0	1	0	1	mandatory= 0	0	1
error

funcName= send 3	4	funcName= open 1	7	EndOfFunc
BEGIN_LINE
0	21	1	3	-1	0
END_LINE
finishport

3	1	coords= 330 125 0	2	0	1	mandatory= 0	0	1
----

funcName= open 1	8	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
finishport

3	1	coords= 330 135 0	3	0	1	mandatory= 0	0	1
error

funcName= open 1	9	EndOfFunc
BEGIN_LINE
0	11	1	3	-1	0
END_LINE
finishport

3	3	coords= 330 195 0	4	0	1	mandatory= 0	0	1
received

funcName= open 1	10	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
END_LINE
finishport

3	1	coords= 330 165 0	5	0	1	mandatory= 0	0	1
---

funcName= open 1	11	funcName= close 2	3	EndOfFunc
BEGIN_LINE
0	11	2	3	-1	0
END_LINE
finishport

3	1	coords= 330 235 0	6	0	1	mandatory= 0	0	1
---

funcName= send 3	5	funcName= open 1	12	EndOfFunc
BEGIN_LINE
0	21	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
3	0	0	0

_
This is a string constant.
Constant_String1
0
_
60	330	200	351
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
String Constant	3	0	100	1	5	4	0	1.2	Hello TCP World 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 200 341 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	2	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
4	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
860	90	975	305
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

0	1	coords= 859 247 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 257 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 267 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 277 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 287 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 859 207 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	158	0	0	-1	1
799	515
799	207
END_LINE
inputport

0	3	coords= 859 217 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
1	190	0	0	-1	0
834	6
834	217
END_LINE
outputport

1	1	coords= 975 247 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 975 257 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 975 267 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 975 277 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 975 207 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 975 217 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= 859 115 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 125 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 140 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 150 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 235 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
1	158	0	2	-1	1
804	505
804	235
END_LINE
startport

2	1	coords= 859 195 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	0
835	159
835	195
END_LINE
finishport

3	1	coords= 975 115 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 125 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 140 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 150 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 235 0	5	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	66	4	3	-1	0
END_LINE
finishport

3	1	coords= 975 165 0	4	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 175 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 195 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 975 185 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
10	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
850	375	965	590
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
Widget Tag	5	0	0	1	40	200	0	1.2	widget2
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

0	1	coords= 849 532 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 849 542 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 849 552 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 849 562 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 849 572 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 849 492 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	64	2	0	-1	1
764	310
764	492
END_LINE
inputport

0	3	coords= 849 502 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 965 532 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 965 542 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 965 552 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 965 562 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 965 492 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 965 502 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= 849 400 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 849 410 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 849 425 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 849 435 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 849 520 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
1	64	4	2	-1	1
759	260
759	520
END_LINE
startport

2	1	coords= 849 480 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	0
830	159
830	480
END_LINE
finishport

3	1	coords= 965 400 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 410 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 425 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 435 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 520 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 450 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 460 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 965 480 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 965 470 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
11	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
485	5	505	70
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 484 15 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	2	2	2	-1	1
407	125
407	15
END_LINE
startport

2	-1	coords= 484 30 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	2	3	2	-1	1
412	135
412	30
END_LINE
startport

2	-1	coords= 484 45 0	2	0	1	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	2	5	2	-1	1
417	165
417	45
END_LINE
startport

2	-1	coords= 484 60 0	3	0	1	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	1
422	175
422	60
END_LINE
finishport

3	-1	coords= 497 39 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
19	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
790	125	810	190
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 789 135 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	64	0	2	-1	1
734	210
734	135
END_LINE
startport

2	-1	coords= 789 150 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	64	1	2	-1	1
739	220
739	150
END_LINE
startport

2	-1	coords= 789 165 0	2	0	1	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	64	5	2	-1	1
744	330
744	165
END_LINE
startport

2	-1	coords= 789 180 0	3	0	1	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	64	6	2	-1	1
749	340
749	180
END_LINE
finishport

3	-1	coords= 802 159 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	10	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
20	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
75	255	170	275
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	14
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= 170 265 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	2	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
21	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
405	305	425	345
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

2	-1	coords= 404 315 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	2	6	2	-1	1
394	220
394	315
END_LINE
startport

2	-1	coords= 404 335 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	2	1	2	-1	1
372	245
372	335
END_LINE
finishport

3	-1	coords= 417 326 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
22	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
485	260	505	325
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 484 270 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	2	4	2	-1	1
407	195
407	270
END_LINE
startport

2	-1	coords= 484 285 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 484 300 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 484 315 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= 497 294 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	64	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
34	0	0	0

_
This is a string constant.
Constant_String1
0
_
410	365	550	386
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
String Constant	3	0	100	1	5	4	0	1.2	Back at ya! 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 550 376 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
35	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
730	325	750	365
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

2	-1	coords= 729 335 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	64	2	2	-1	1
719	315
719	335
END_LINE
startport

2	-1	coords= 729 355 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	64	3	2	-1	1
713	325
713	355
END_LINE
finishport

3	-1	coords= 742 346 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

inx-netsocket
_
49	0	0	0
_
_
provides tcpip and udp socket data comms.
netSocket
0
_
245	690	350	870
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.5	0	Sck_Client
trans	-1	0	0	0
offset	0	-15
hash	0xF78A
Instance_Info	_
End_Instance
END_BA
parameter
URL	3	0	256	0	0	0	0	1.2	127.0.0.1
EndOfValues
EndOfLabels
URL of the server socket. N/A for sockets

parameter
port	1	0	65535	0	0	0	0	1.2	8020
EndOfValues
EndOfLabels
IP socket number to open

parameter
tcp/udp	1	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
0=tcp, 1=udp

inputport

0	1	coords= 244 735 0	0	0	1	mandatory= 0	0	1
port

funcName= open 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 244 725 0	1	0	1	mandatory= 0	0	1
url

funcName= open 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 244 835 0	2	0	1	mandatory= 0	0	1
data

funcName= send 3	1	EndOfFunc
BEGIN_LINE
1	50	0	0	-1	1
232	886
232	835
END_LINE
inputport

0	1	coords= 244 845 0	3	0	1	mandatory= 0	0	1
size

funcName= send 3	2	EndOfFunc
BEGIN_LINE
1	55	0	0	-1	0
209	805
209	845
END_LINE
outputport

1	1	coords= 350 845 0	0	0	1	mandatory= 0	0	1
errno

funcName= open 1	3	funcName= close 2	1	funcName= send 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 350 795 0	1	0	1	mandatory= 0	0	1
data

funcName= open 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 350 805 0	2	0	1	mandatory= 0	0	1
size

funcName= open 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 244 715 0	0	0	1	mandatory= 0	0	1
open

funcName= open 1	0	EndOfFunc
BEGIN_LINE
1	52	5	2	-1	1
1025	1035
1025	930
194	930
194	715
END_LINE
startport

2	1	coords= 244 755 0	2	0	1	mandatory= 0	0	1
close

funcName= close 2	0	EndOfFunc
BEGIN_LINE
1	56	0	2	-1	1
497	871
497	670
184	670
184	755
END_LINE
startport

2	1	coords= 244 825 0	2	0	1	mandatory= 0	0	1
send

funcName= send 3	0	EndOfFunc
BEGIN_LINE
1	49	2	2	-1	1
415	715
415	655
179	655
179	825
END_LINE
finishport

3	1	coords= 350 765 0	0	0	1	mandatory= 0	0	1
error

funcName= close 2	2	funcName= open 1	6	EndOfFunc
BEGIN_LINE
0	11	3	3	-1	0
END_LINE
finishport

3	1	coords= 350 835 0	1	0	1	mandatory= 0	0	1
error

funcName= send 3	4	funcName= open 1	7	EndOfFunc
BEGIN_LINE
0	21	1	3	-1	0
END_LINE
finishport

3	1	coords= 350 715 0	2	0	1	mandatory= 0	0	1
----

funcName= open 1	8	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
finishport

3	1	coords= 350 725 0	3	0	1	mandatory= 0	0	1
error

funcName= open 1	9	EndOfFunc
BEGIN_LINE
0	11	1	3	-1	0
END_LINE
finishport

3	3	coords= 350 785 0	4	0	1	mandatory= 0	0	1
received

funcName= open 1	10	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
END_LINE
finishport

3	1	coords= 350 755 0	5	0	1	mandatory= 0	0	1
---

funcName= open 1	11	funcName= close 2	3	EndOfFunc
BEGIN_LINE
0	11	2	3	-1	0
END_LINE
finishport

3	1	coords= 350 825 0	6	0	1	mandatory= 0	0	1
---

funcName= send 3	5	funcName= open 1	12	EndOfFunc
BEGIN_LINE
0	21	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
50	0	0	0

_
This is a string constant.
Constant_String1
0
_
80	875	220	896
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
String Constant	3	0	100	1	5	4	0	1.2	Hello UDP World 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 220 886 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	2	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
51	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
895	650	1010	865
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
Widget Tag	5	0	0	1	40	200	0	1.2	widget3
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

0	1	coords= 894 807 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 894 817 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 894 827 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 894 837 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 894 847 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 894 767 0	0	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	126	0	0	-1	0
874	810
874	767
END_LINE
inputport

0	3	coords= 894 777 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 1010 807 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1010 817 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1010 827 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1010 837 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 1010 767 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 1010 777 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= 894 675 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 894 685 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 894 700 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 894 710 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= 894 795 0	0	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
1	126	0	2	-1	0
884	800
894	795
END_LINE
startport

2	1	coords= 894 755 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
822	619
822	755
END_LINE
finishport

3	1	coords= 1010 675 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1010 685 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1010 700 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1010 710 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1010 795 0	5	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	63	1	3	-1	0
END_LINE
finishport

3	1	coords= 1010 725 0	4	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1010 735 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1010 755 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 1010 745 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
52	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
860	960	975	1175
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
Widget Tag	5	0	0	1	40	200	0	1.2	widget4
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

0	1	coords= 859 1117 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 1127 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 1137 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 1147 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 859 1157 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 859 1077 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	63	2	0	-1	1
749	770
749	1077
END_LINE
inputport

0	3	coords= 859 1087 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 975 1117 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 975 1127 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 975 1137 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 975 1147 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 975 1077 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 975 1087 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= 859 985 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 995 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 1010 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 1020 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 859 1105 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
1	63	4	2	-1	1
700	720
700	1105
END_LINE
startport

2	1	coords= 859 1065 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
855	619
855	1065
END_LINE
finishport

3	1	coords= 975 985 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 995 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 1010 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 1020 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 1105 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 1035 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 1045 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 975 1065 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 975 1055 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
53	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
480	590	500	655
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 479 600 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	49	2	2	-1	1
469	700
469	600
END_LINE
startport

2	-1	coords= 479 615 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	49	3	2	-1	1
409	725
409	615
END_LINE
startport

2	-1	coords= 479 630 0	2	0	1	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	49	5	2	-1	1
409	755
409	630
END_LINE
startport

2	-1	coords= 479 645 0	3	0	1	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	49	0	2	-1	1
409	765
409	645
END_LINE
finishport

3	-1	coords= 492 624 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	63	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
54	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
800	585	820	650
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 799 595 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	63	0	2	-1	1
789	655
789	595
END_LINE
startport

2	-1	coords= 799 610 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	63	1	2	-1	1
749	680
749	610
END_LINE
startport

2	-1	coords= 799 625 0	2	0	1	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	63	5	2	-1	1
754	790
754	625
END_LINE
startport

2	-1	coords= 799 640 0	3	0	1	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	63	6	2	-1	1
759	800
759	640
END_LINE
finishport

3	-1	coords= 812 619 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	52	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
55	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
70	795	165	815
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	16
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= 165 805 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	49	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
56	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
425	850	445	890
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

2	-1	coords= 424 860 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	49	6	2	-1	1
387	825
387	860
END_LINE
startport

2	-1	coords= 424 880 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	49	1	2	-1	1
385	835
385	880
END_LINE
finishport

3	-1	coords= 437 871 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
57	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
505	805	525	870
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 504 815 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	49	4	2	-1	1
427	785
427	815
END_LINE
startport

2	-1	coords= 504 830 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 504 845 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 504 860 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= 517 839 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	63	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
58	0	0	0

_
This is a string constant.
Constant_String1
0
_
445	935	585	956
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
String Constant	3	0	100	1	5	4	0	1.2	Back at ya! 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 585 946 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
59	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
780	700	800	740
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

2	-1	coords= 779 710 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	63	2	2	-1	1
744	740
744	710
END_LINE
startport

2	-1	coords= 779 730 0	1	0	1	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	63	3	2	-1	1
739	710
739	730
END_LINE
finishport

3	-1	coords= 792 721 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

netsocketrsrvr
_
60	0	0	0
_
_
Listens on specified port
netsocketsrvr
0
_
1240	355	1345	535
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.5	0	Sck_Server
trans	-1	0	0	0
offset	0	-15
hash	0x5A86
Instance_Info	_
End_Instance
END_BA
parameter
Interface	3	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Interface (IF)

parameter
Port	1	0	100000	0	0	0	0	1.2	8000
EndOfValues
EndOfLabels
Listening Port

parameter
udp	1	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
tcp/udp mode

inputport

0	3	coords= 1239 390 0	0	0	1	mandatory= 0	0	1
IF

funcName= open 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1239 400 0	1	0	1	mandatory= 0	0	1
port

funcName= open 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 1239 510 0	2	0	1	mandatory= 0	0	1
data

funcName= sendData 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1239 520 0	3	0	1	mandatory= 0	0	1
inSize

funcName= sendData 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 1345 460 0	0	0	1	mandatory= 0	0	1
data

funcName= open 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1345 470 0	1	0	1	mandatory= 0	0	1
size

funcName= open 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 1345 480 0	2	0	1	mandatory= 0	0	1
source

funcName= open 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 1239 380 0	0	0	1	mandatory= 0	0	1
open

funcName= open 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 1239 420 0	1	0	1	mandatory= 0	0	1
close

funcName= close 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 1239 500 0	2	0	1	mandatory= 0	0	1
send

funcName= sendData 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= 1345 380 0	0	0	1	mandatory= 0	0	1
---

funcName= open 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1345 390 0	1	0	1	mandatory= 0	0	1
error

funcName= open 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1345 450 0	2	0	1	mandatory= 0	0	1
received

funcName= open 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1345 420 0	3	0	1	mandatory= 0	0	1
---

funcName= open 1	9	funcName= close 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1345 430 0	4	0	1	mandatory= 0	0	1
error

funcName= open 1	10	funcName= close 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1345 500 0	5	0	1	mandatory= 0	0	1
---

funcName= open 1	11	funcName= sendData 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1345 510 0	6	0	1	mandatory= 0	0	1
error

funcName= open 1	12	funcName= sendData 3	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

netsocketrsrvr
_
63	0	0	0
_
_
Listens on specified port
netsocketsrvr
0
_
605	645	710	825
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.5	0	Sck_Server
trans	-1	0	0	0
offset	0	-15
hash	0x5A86
Instance_Info	_
End_Instance
END_BA
parameter
Interface	3	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Interface (IF)

parameter
Port	1	0	100000	0	0	0	0	1.2	8020
EndOfValues
EndOfLabels
Listening Port

parameter
udp	1	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
tcp/udp mode

inputport

0	3	coords= 604 680 0	0	0	1	mandatory= 0	0	1
IF

funcName= open 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 604 690 0	1	0	1	mandatory= 0	0	1
port

funcName= open 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 604 800 0	2	0	1	mandatory= 0	0	1
data

funcName= sendData 3	1	EndOfFunc
BEGIN_LINE
1	58	0	0	-1	0
635	946
635	925
590	925
590	800
END_LINE
inputport

0	1	coords= 604 810 0	3	0	1	mandatory= 0	0	1
inSize

funcName= sendData 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 710 750 0	0	0	1	mandatory= 0	0	1
data

funcName= open 1	3	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
outputport

1	1	coords= 710 760 0	1	0	1	mandatory= 0	0	1
size

funcName= open 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 710 770 0	2	0	1	mandatory= 0	0	1
source

funcName= open 1	5	EndOfFunc
BEGIN_LINE
0	10	5	1	-1	0
END_LINE
startport

2	1	coords= 604 670 0	0	0	1	mandatory= 0	0	1
open

funcName= open 1	0	EndOfFunc
BEGIN_LINE
1	51	5	2	-1	1
1030	725
1030	615
554	615
554	670
END_LINE
startport

2	1	coords= 604 710 0	1	0	1	mandatory= 0	0	1
close

funcName= close 2	0	EndOfFunc
BEGIN_LINE
1	95	5	2	-1	1
1250	775
1250	595
514	595
514	710
END_LINE
startport

2	1	coords= 604 790 0	2	0	1	mandatory= 0	0	1
send

funcName= sendData 3	0	EndOfFunc
BEGIN_LINE
1	57	0	2	-1	0
555	839
555	790
END_LINE
finishport

3	1	coords= 710 670 0	0	0	1	mandatory= 0	0	1
---

funcName= open 1	6	EndOfFunc
BEGIN_LINE
0	19	0	3	-1	0
END_LINE
finishport

3	1	coords= 710 680 0	1	0	1	mandatory= 0	0	1
error

funcName= open 1	7	EndOfFunc
BEGIN_LINE
0	19	1	3	-1	0
END_LINE
finishport

3	1	coords= 710 740 0	4	0	1	mandatory= 0	0	1
received

funcName= open 1	8	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
finishport

3	1	coords= 710 710 0	5	0	1	mandatory= 0	0	1
---

funcName= open 1	9	funcName= close 2	1	EndOfFunc
BEGIN_LINE
0	59	1	3	-1	0
END_LINE
finishport

3	1	coords= 710 720 0	6	0	1	mandatory= 0	0	1
error

funcName= open 1	10	funcName= close 2	2	EndOfFunc
BEGIN_LINE
0	10	4	3	-1	0
END_LINE
finishport

3	1	coords= 710 790 0	2	0	1	mandatory= 0	0	1
---

funcName= open 1	11	funcName= sendData 3	3	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
finishport

3	1	coords= 710 800 0	3	0	1	mandatory= 0	0	1
error

funcName= open 1	12	funcName= sendData 3	4	EndOfFunc
BEGIN_LINE
0	19	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

netsocketrsrvr
_
64	0	0	0
_
_
Listens on specified port
netsocketsrvr
0
_
575	185	680	365
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.5	0	Sck_Server
trans	-1	0	0	0
offset	0	-15
hash	0x5A86
Instance_Info	_
End_Instance
END_BA
parameter
Interface	3	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Interface (IF)

parameter
Port	1	0	100000	0	0	0	0	1.2	8021
EndOfValues
EndOfLabels
Listening Port

parameter
udp	1	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
tcp/udp mode

inputport

0	3	coords= 574 220 0	0	0	1	mandatory= 0	0	1
IF

funcName= open 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 574 230 0	1	0	1	mandatory= 0	0	1
port

funcName= open 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 574 340 0	2	0	1	mandatory= 0	0	1
data

funcName= sendData 3	1	EndOfFunc
BEGIN_LINE
1	34	0	0	-1	0
562	376
562	340
END_LINE
inputport

0	1	coords= 574 350 0	3	0	1	mandatory= 0	0	1
inSize

funcName= sendData 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 680 290 0	0	0	1	mandatory= 0	0	1
data

funcName= open 1	3	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
outputport

1	1	coords= 680 300 0	1	0	1	mandatory= 0	0	1
size

funcName= open 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= 680 310 0	2	0	1	mandatory= 0	0	1
source

funcName= open 1	5	EndOfFunc
BEGIN_LINE
0	10	5	1	-1	0
END_LINE
startport

2	1	coords= 574 210 0	0	0	1	mandatory= 0	0	1
open

funcName= open 1	0	EndOfFunc
BEGIN_LINE
1	4	5	2	-1	1
1030	165
1030	65
519	65
519	210
END_LINE
startport

2	1	coords= 574 250 0	1	0	1	mandatory= 0	0	1
close

funcName= close 2	0	EndOfFunc
BEGIN_LINE
1	66	5	2	-1	0
1195	275
1195	155
524	155
524	250
END_LINE
startport

2	1	coords= 574 330 0	1	0	1	mandatory= 0	0	1
send

funcName= sendData 3	0	EndOfFunc
BEGIN_LINE
1	22	0	2	-1	1
540	294
540	330
END_LINE
finishport

3	1	coords= 680 210 0	0	0	1	mandatory= 0	0	1
---

funcName= open 1	6	EndOfFunc
BEGIN_LINE
0	19	0	3	-1	0
END_LINE
finishport

3	1	coords= 680 220 0	1	0	1	mandatory= 0	0	1
error

funcName= open 1	7	EndOfFunc
BEGIN_LINE
0	19	1	3	-1	0
END_LINE
finishport

3	1	coords= 680 280 0	4	0	1	mandatory= 0	0	1
received

funcName= open 1	8	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
finishport

3	1	coords= 680 250 0	5	0	1	mandatory= 0	0	1
---

funcName= open 1	9	funcName= close 2	1	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
finishport

3	1	coords= 680 260 0	6	0	1	mandatory= 0	0	1
error

funcName= open 1	10	funcName= close 2	2	EndOfFunc
BEGIN_LINE
0	10	4	3	-1	0
END_LINE
finishport

3	1	coords= 680 330 0	2	0	1	mandatory= 0	0	1
---

funcName= open 1	11	funcName= sendData 3	3	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
finishport

3	1	coords= 680 340 0	3	0	1	mandatory= 0	0	1
error

funcName= open 1	12	funcName= sendData 3	4	EndOfFunc
BEGIN_LINE
0	19	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
66	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
0
_
1040	200	1145	380
1	0
blockattr
BEGIN_BA
type	IO
text	40	5	1.25	0	Patch
trans	-1	0	0	0
offset	0	-15
hash	0xB38B
Instance_Info	_
End_Instance
END_BA
parameter
Widget Tag	5	0	0	1	40	165	0	1.2	widget5
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

0	1	coords= 1039 320 0	0	0	1	mandatory= 0	0	1
off.x

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1039 330 0	1	0	1	mandatory= 0	0	1
off.y

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1039 340 0	2	0	1	mandatory= 0	0	1
off.wid

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1039 350 0	3	0	1	mandatory= 0	0	1
off.ht

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1039 365 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 1145 320 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1145 330 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1145 340 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1145 350 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= 1039 225 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1039 235 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 1039 250 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1039 260 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1039 305 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
1	4	4	2	-1	0
985	235
985	305
END_LINE
finishport

3	1	coords= 1145 225 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1145 235 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1145 250 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1145 260 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1145 305 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1145 275 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	6	EndOfFunc
BEGIN_LINE
0	158	1	3	-1	0
END_LINE
finishport

3	1	coords= 1145 285 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
95	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
0
_
1055	700	1160	880
1	0
blockattr
BEGIN_BA
type	IO
text	40	5	1.25	0	Patch
trans	-1	0	0	0
offset	0	-15
hash	0xB38B
Instance_Info	_
End_Instance
END_BA
parameter
Widget Tag	5	0	0	1	40	165	0	1.2	widget6
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

0	1	coords= 1054 820 0	0	0	1	mandatory= 0	0	1
off.x

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1054 830 0	1	0	1	mandatory= 0	0	1
off.y

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1054 840 0	2	0	1	mandatory= 0	0	1
off.wid

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1054 850 0	3	0	1	mandatory= 0	0	1
off.ht

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 1054 865 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 1160 820 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1160 830 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1160 840 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 1160 850 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= 1054 725 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1054 735 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= 1054 750 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1054 760 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= 1054 805 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= 1160 725 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1160 735 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1160 750 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1160 760 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1160 805 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 1160 775 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	6	EndOfFunc
BEGIN_LINE
0	126	1	3	-1	0
END_LINE
finishport

3	1	coords= 1160 785 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
125	0	0	0

_
This is a string constant.
Constant_String1
0
_
80	540	220	561
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
String Constant	3	0	100	1	5	4	0	1.2	---
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 220 551 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	158	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
126	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
775	775	805	845
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

0	3	coords= 774 810 0	5	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	63	0	0	-1	1
737	750
737	810
END_LINE
inputport

0	3	coords= 774 835 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	125	0	0	-1	0
545	551
545	835
END_LINE
outputport

1	3	coords= 805 810 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	51	5	1	-1	0
END_LINE
startport

2	-1	coords= 774 800 0	4	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	63	4	2	-1	1
737	720
737	800
END_LINE
startport

2	3	coords= 774 825 0	1	0	1	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	95	5	2	-1	0
1215	775
1215	760
729	760
729	825
END_LINE
finishport

3	3	coords= 805 800 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
158	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
680	480	710	550
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

0	3	coords= 679 515 0	5	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	64	0	0	-1	1
730	290
730	440
629	440
629	515
END_LINE
inputport

0	3	coords= 679 540 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	125	0	0	-1	0
454	551
464	540
END_LINE
outputport

1	3	coords= 710 515 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
startport

2	-1	coords= 679 505 0	4	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	64	4	2	-1	1
715	260
715	445
614	445
614	505
END_LINE
startport

2	3	coords= 679 530 0	1	0	1	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	66	5	2	-1	0
1200	275
1200	455
634	455
634	530
END_LINE
finishport

3	3	coords= 710 505 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	4	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
190	0	0	0

_
This is a string constant.
Constant_String1
0
_
655	-5	795	16
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
String Constant	3	0	100	1	5	4	0	1.2	Socket Server Received Data
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 795 6 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	4	6	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
