#V:2.0.0

IconData
BEGIN_BLOCK

ENCAPSULATE2
PairingCodeGen
1232	1	1	0
PairingCodeGen1
_
_
PairingCodeGen
0
unityDevmanIntegration2\PairingWidget1\
225	40	307	111
1	0
outputport

1	3	311	70	0	0	1	mandatory= 0	0	1
pairingcode

EndOfFunc
BEGIN_LINE
0	12	5	1	-1	0
END_LINE
startport

2	-1	219	50	0	0	1	mandatory= 0	0	1
getnewcode

EndOfFunc
BEGIN_LINE
1	25	0	2	0	0
209	38
209	50
END_LINE
startport

2	-1	219	65	1	0	1	mandatory= 0	0	1
stop

EndOfFunc
BEGIN_LINE
1	251	0	2	1	0
204	273
204	65
END_LINE
finishport

3	-1	311	50	0	0	1	mandatory= 0	0	1
created
crfeate
EndOfFunc
BEGIN_LINE
0	736	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

unity2
_
1233	0	0	0
_
_
unity
unity2
0
_
665	120	774	259
1	0
inputport

0	3	659	150	0	0	1	mandatory= 0	0	1
zone id

create	1	EndOfFunc
BEGIN_LINE
1	1234	0	0	-1	0
365	180
365	150
END_LINE
inputport

0	3	659	160	1	0	1	mandatory= 0	0	1
Type

create	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	659	170	2	0	1	mandatory= 0	0	1
Params

create	3	EndOfFunc
BEGIN_LINE
1	1235	0	0	-1	0
439	240
439	170
END_LINE
inputport

0	3	659	200	3	0	1	mandatory= 0	0	1
Media Type

setmedia	1	EndOfFunc
BEGIN_LINE
1	1236	0	0	-1	0
429	355
429	200
END_LINE
inputport

0	3	659	210	4	0	1	mandatory= 0	0	1
Path

setmedia	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	659	220	0	0	1	mandatory= 0	0	1
Text

setmedia	3	EndOfFunc
BEGIN_LINE
1	1239	0	0	-1	0
539	260
539	220
END_LINE
inputport

0	3	659	230	6	0	1	mandatory= 0	0	1
Params

setmedia	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	778	150	0	0	1	mandatory= 0	0	1
event

create	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	659	130	0	0	1	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
1	1232	0	2	4	0
589	50
589	130
END_LINE
startport

2	3	659	180	2	0	1	mandatory= 0	0	1
Destroy

destroy	0	EndOfFunc
BEGIN_LINE
1	251	0	2	1	0
360	273
360	180
END_LINE
startport

2	0	659	190	0	0	1	mandatory= 0	0	1
Set Media

setmedia	0	EndOfFunc
BEGIN_LINE
1	1242	0	2	-1	0
634	190
634	190
END_LINE
startport

2	3	659	240	3	0	1	mandatory= 0	0	1
Stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	778	130	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	274	1	3	-1	0
END_LINE
finishport

3	-1	778	140	1	0	1	mandatory= 0	0	1
Event

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	778	180	2	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	778	190	3	0	1	mandatory= 0	0	1
--

setmedia	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	778	240	4	0	1	mandatory= 0	0	1
--

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
1234	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
220	150	320	195
1	0
parameter
String Constant	3	_	_	1
EndOfValues
EndOfLabels
string constant

outputport

1	3	320	180	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	12	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
1235	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
220	210	320	255
1	0
parameter
String Constant	3	_	_	{"id": "1","top": "0%","left": "0%","bottom": "100%","right": "100%","textType" : "title","fgrColor":"255.255.255.255","bgrColor":"6.147.4.255","type":"quad","textConfig":{"color":"230.240.255.255","fontScale":0.3,"alignment":"left"}}
EndOfValues
EndOfLabels
string constant

outputport

1	3	320	240	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	12	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
1236	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
230	325	330	370
1	0
parameter
String Constant	3	_	_	smilText
EndOfValues
EndOfLabels
string constant

outputport

1	3	330	355	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	12	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
25	0	0	0
show
_
This is an encapsulation start control.
xstart
0
_
100	25	184	54
1	0
finishport

3	-1	188	38	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
251	0	0	0
hide
_
This is an encapsulation start control.
xstart
0
_
80	260	164	289
1	0
finishport

3	-1	168	273	0	0	1	mandatory= 0	0	1

destroy
EndOfFunc
BEGIN_LINE
0	904	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
1237	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
930	120	990	210
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	20000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	930	165	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	930	180	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	930	195	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	930	130	0	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	1233	0	2	-1	0
815	130
815	130
END_LINE
startport

2	0	930	145	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	251	0	2	1	0
915	273
915	145
END_LINE
finishport

3	0	990	160	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	274	0	3	-1	0
END_LINE
finishport

3	0	990	130	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	990	145	2	0	1	mandatory= 0	0	1


stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
1238	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
230	395	330	440
1	0
parameter
String Constant	3	_	_	Generating Pairing Code. Please wait...
EndOfValues
EndOfLabels
string constant

outputport

1	3	330	425	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	274	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3s
_
1239	0	0	0
_
_
This is a 3-input string multiplexer.
MultiplexThreeInputString
0
_
465	230	515	340
1	0
inputport

0	3	465	255	5	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	1232	0	0	3	0
409	70
409	255
END_LINE
inputport

0	3	465	290	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	1238	0	0	-1	0
450	425
450	290
END_LINE
inputport

0	3	465	325	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	1240	0	0	-1	0
435	480
435	325
END_LINE
outputport

1	3	515	260	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	EndOfFunc
BEGIN_LINE
0	259	5	1	-1	0
END_LINE
startport

2	3	465	240	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	1237	0	2	-1	0
1000	160
1000	245
455	245
455	240
END_LINE
startport

2	3	465	275	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	1233	0	2	-1	0
793	130
793	105
450	105
450	275
END_LINE
startport

2	3	465	310	2	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
1	576	0	2	2	0
205	355
205	355
440	358
440	310
END_LINE
finishport

3	3	515	240	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	EndOfFunc
BEGIN_LINE
0	259	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
1240	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
240	450	340	495
1	0
parameter
String Constant	3	_	_	Downloading your first playlist. Please wait... 
EndOfValues
EndOfLabels
string constant

outputport

1	3	340	480	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	274	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
576	0	0	0
show downloading
_
This is an encapsulation start control.
xstart
0
_
85	345	169	374
1	0
finishport

3	-1	173	358	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	585	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventsetrstbool
_
1241	0	0	0
_
_
This is a boolean that can be set and reset by events.
EventSetReset
0
_
120	180	170	225
1	0
outputport

1	0	170	210	0	0	1	mandatory= 0	0	1

visible
Set	1	Reset	1	EndOfFunc
BEGIN_LINE
0	737	0	1	-1	0
END_LINE
startport

2	0	120	190	0	0	1	mandatory= 0	0	1
set
crfeate
Set	0	EndOfFunc
BEGIN_LINE
1	1232	0	2	-1	0
326	50
326	290
105	290
105	190
END_LINE
startport

2	0	120	205	1	0	1	mandatory= 0	0	1
rst
destroy
Reset	0	EndOfFunc
BEGIN_LINE
1	251	0	2	1	0
183	273
183	305
105	305
105	205
END_LINE
finishport

3	0	170	190	0	0	1	mandatory= 0	0	1


Set	2	Reset	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
1242	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
580	180	598	220
1	0
inputport

0	0	580	210	0	0	1	mandatory= 1	0	1

visible
Update	1	EndOfFunc
BEGIN_LINE
1	1241	0	0	-1	0
570	210
570	210
END_LINE
startport

2	3	580	190	1	0	1	mandatory= 1	0	1


Update	0	EndOfFunc
BEGIN_LINE
1	1239	0	2	-1	0
525	240
525	190
END_LINE
finishport

3	0	598	190	0	0	1	mandatory= 1	0	1


Update	2	EndOfFunc
BEGIN_LINE
0	728	2	3	-1	0
END_LINE
finishport

3	0	598	210	1	0	1	mandatory= 0	0	1


Update	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
