#V:2.0.0

CanvasSizeXY
2000	1000

IconData
BEGIN_BLOCK

event_text_switch
_
1	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
210	70	260	244
1	0
parameter
case 1	3	_	_	"This Onexxxx!"
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	_	_	"Another for 2"
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	_	_	somethingwithoutspaces
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	_	_	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	_	_	"This One!"
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	_	_	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	_	_	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	_	_	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	210	90	0	0	1	mandatory= 0	0	1
test

Run	1	EndOfFunc
BEGIN_LINE
1	18	0	0	-1	0
200	180
200	90
END_LINE
inputport

0	3	210	110	1	0	1	mandatory= 0	0	1
case 1

Run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	125	2	0	1	mandatory= 0	0	1
case 2

Run	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	140	3	0	1	mandatory= 0	0	1
case 3

Run	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	155	4	0	1	mandatory= 0	0	1
case 4

Run	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	170	5	0	1	mandatory= 0	0	1
case 5

Run	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	185	6	0	1	mandatory= 0	0	1
case 6

Run	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	200	7	0	1	mandatory= 0	0	1
case 7

Run	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	215	8	0	1	mandatory= 0	0	1
case 8

Run	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	210	80	0	1	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	260	110	4	0	1	mandatory= 0	0	1
1

Run	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	125	1	0	1	mandatory= 0	0	1
2

Run	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	140	2	0	1	mandatory= 0	0	1
3

Run	12	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	155	3	0	1	mandatory= 0	0	1
4

Run	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	170	7	0	1	mandatory= 0	0	1
5

Run	14	EndOfFunc
BEGIN_LINE
0	5	4	3	-1	0
END_LINE
finishport

3	3	260	185	5	0	1	mandatory= 0	0	1
6

Run	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	200	6	0	1	mandatory= 0	0	1
7

Run	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	215	0	0	1	mandatory= 0	0	1
8

Run	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	230	8	0	1	mandatory= 0	0	1
Default

Run	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
4	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
210	275	260	449
1	0
parameter
case 1	3	_	_	"Bogus"
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	_	_	"Overwrite Me"
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	_	_	"And Overwrite Me"
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	_	_	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	_	_	_
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	_	_	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	_	_	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	_	_	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	210	295	0	0	1	mandatory= 0	0	1
test

Run	1	EndOfFunc
BEGIN_LINE
1	11	0	0	-1	0
200	315
200	295
END_LINE
inputport

0	3	210	315	1	0	1	mandatory= 0	0	1
case 1

Run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	330	2	0	1	mandatory= 0	0	1
case 2

Run	3	EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
180	400
180	330
END_LINE
inputport

0	3	210	345	3	0	1	mandatory= 0	0	1
case 3

Run	4	EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
170	400
170	345
END_LINE
inputport

0	3	210	360	4	0	1	mandatory= 0	0	1
case 4

Run	5	EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
160	400
160	360
END_LINE
inputport

0	3	210	375	5	0	1	mandatory= 0	0	1
case 5

Run	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	390	7	0	1	mandatory= 0	0	1
case 6

Run	7	EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
160	400
160	390
END_LINE
inputport

0	3	210	405	6	0	1	mandatory= 0	0	1
case 7

Run	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	210	420	1	0	1	mandatory= 0	0	1
case 8

Run	9	EndOfFunc
BEGIN_LINE
1	11	0	0	-1	0
190	315
190	420
END_LINE
startport

2	-1	210	285	0	1	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	260	315	7	0	1	mandatory= 0	0	1
1

Run	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	330	1	0	1	mandatory= 0	0	1
2

Run	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	345	2	0	1	mandatory= 0	0	1
3

Run	12	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	360	0	0	1	mandatory= 0	0	1
4

Run	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	375	4	0	1	mandatory= 0	0	1
5

Run	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	390	5	0	1	mandatory= 0	0	1
6

Run	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	405	6	0	1	mandatory= 0	0	1
7

Run	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	260	420	3	0	1	mandatory= 0	0	1
8

Run	17	EndOfFunc
BEGIN_LINE
0	10	4	3	-1	0
END_LINE
finishport

3	3	260	435	8	0	1	mandatory= 0	0	1
Default

Run	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
5	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
395	65	495	239
1	0
parameter
Widget Tag	5	_	_	ParameterTest
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	395	170	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	395	180	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	395	190	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	395	200	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	395	215	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	230	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	26	0	0	-1	0
365	75
365	230
END_LINE
outputport

1	1	495	170	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	495	180	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	495	190	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	495	200	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	395	75	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	395	85	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	395	100	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	395	110	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	395	155	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	4	2	-1	0
350	170
350	155
END_LINE
finishport

3	1	495	75	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	495	85	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	495	100	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	495	110	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	495	155	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	495	125	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	495	135	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
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
390	280	490	454
1	0
parameter
Widget Tag	5	_	_	InputTest
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	390	385	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	390	395	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	390	405	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	390	415	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	390	430	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	445	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	26	0	0	-1	0
365	75
365	445
END_LINE
outputport

1	1	490	385	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	490	395	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	490	405	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	490	415	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	390	290	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	390	300	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	390	315	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	390	325	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	390	370	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	4	7	2	-1	0
345	420
345	370
END_LINE
finishport

3	1	490	290	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	300	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	315	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	325	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	370	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	340	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	350	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
11	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
20	285	120	330
1	0
parameter
String Constant	3	_	_	Another One that might Change
EndOfValues
EndOfLabels
string constant

outputport

1	3	120	315	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	4	0	1	-1	0
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
35	150	135	195
1	0
parameter
String Constant	3	_	_	This One!
EndOfValues
EndOfLabels
string constant

outputport

1	3	135	180	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
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
26	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
250	45	350	90
1	0
parameter
String Constant	3	_	_	PASSED!
EndOfValues
EndOfLabels
string constant

outputport

1	3	350	75	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	10	5	1	-1	0
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
35	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
35	370	135	415
1	0
parameter
String Constant	3	_	_	Another One that mightdiff 1
EndOfValues
EndOfLabels
string constant

outputport

1	3	135	400	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	4	8	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
