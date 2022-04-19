#V:2.0.0

IconData
BEGIN_BLOCK

ENCAPSULATE2
PairingCodeGen
11	1	1	0
PairingCodeGen1
_
_
PairingCodeGen
0
DevicePairing\
155	25	237	96
1	0
outputport

1	3	241	55	0	0	1	mandatory= 0	0	1
pairingcode

EndOfFunc
BEGIN_LINE
0	12	5	1	-1	0
END_LINE
startport

2	-1	149	35	0	0	1	mandatory= 0	0	1
getnewcode

EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	241	35	0	0	1	mandatory= 0	0	1
created

EndOfFunc
BEGIN_LINE
0	12	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

unity2
_
12	0	0	0
_
_
unity
unity2
0
_
405	105	514	244
1	0
inputport

0	3	399	135	0	0	1	mandatory= 0	0	1
zone id

create	1	EndOfFunc
BEGIN_LINE
1	13	0	0	-1	0
295	170
295	135
END_LINE
inputport

0	3	399	145	1	0	1	mandatory= 0	0	1
Type

create	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	399	155	2	0	1	mandatory= 0	0	1
Params

create	3	EndOfFunc
BEGIN_LINE
1	18	0	0	-1	0
369	230
369	155
END_LINE
inputport

0	3	399	185	3	0	1	mandatory= 0	0	1
Media Type

setmedia	1	EndOfFunc
BEGIN_LINE
1	24	0	0	-1	0
359	345
359	185
END_LINE
inputport

0	3	399	195	4	0	1	mandatory= 0	0	1
Path

setmedia	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	399	205	5	0	1	mandatory= 0	0	1
Text

setmedia	3	EndOfFunc
BEGIN_LINE
1	11	0	0	-1	0
339	55
339	205
END_LINE
inputport

0	3	399	215	6	0	1	mandatory= 0	0	1
Params

setmedia	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	518	135	0	0	1	mandatory= 0	0	1
event

create	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	399	115	0	0	1	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
1	11	0	2	-1	0
389	35
389	115
END_LINE
startport

2	3	399	165	2	0	1	mandatory= 0	0	1
Destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	399	175	1	0	1	mandatory= 0	0	1
Set Media

setmedia	0	EndOfFunc
BEGIN_LINE
1	12	0	2	-1	0
533	115
533	90
384	90
384	175
END_LINE
startport

2	3	399	225	3	0	1	mandatory= 0	0	1
Stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	518	115	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	12	1	3	-1	0
END_LINE
finishport

3	-1	518	125	1	0	1	mandatory= 0	0	1
Event

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	518	165	2	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	518	175	3	0	1	mandatory= 0	0	1
--

setmedia	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	518	225	4	0	1	mandatory= 0	0	1
--

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
13	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
150	140	250	185
1	0
parameter
String Constant	3	_	_	1
EndOfValues
EndOfLabels
string constant

outputport

1	3	250	170	0	0	1	mandatory= 0	0	1


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
18	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
150	200	250	245
1	0
parameter
String Constant	3	_	_	{"id": "3","top": "0%","left": "0%","bottom": "100%","right": "100%","textType" : "title","fgrColor":"255.255.255.255","bgrColor":"186.147.4.255","type":"quad","textConfig":{"color":"230.240.255.255","fontScale":0.45,"alignment":"left"}, "flags":"textSpinsCube|usePreviousMedia|mediaAlwaysPlay"}
EndOfValues
EndOfLabels
string constant

outputport

1	3	250	230	0	0	1	mandatory= 0	0	1


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
24	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
160	315	260	360
1	0
parameter
String Constant	3	_	_	smilText
EndOfValues
EndOfLabels
string constant

outputport

1	3	260	345	0	0	1	mandatory= 0	0	1


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
END_OF_BLOCKS
