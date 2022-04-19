CanvasSizeXY
2000	2000

IconData
BEGIN_BLOCK

gui_text_string2
_
1	0	0	0
click
_
This widget displays an string to the GUI output.
gui_text_string2
_
195	225	295	399
1	0
parameter
Widget Tag	5	_	_	click
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	195	330	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	195	340	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	195	350	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	195	360	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	195	375	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	195	390	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	5	0	0	-1	0
135	395
135	390
END_LINE
outputport

1	1	295	330	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	295	340	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	295	350	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	295	360	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	195	235	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	195	245	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	195	260	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	195	270	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	195	315	4	1	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	295	235	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	245	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	260	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	270	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	315	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	285	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	7	0	3	-1	0
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
_
805	225	905	399
1	0
parameter
Widget Tag	5	_	_	widget2
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	805	330	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	805	340	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	805	350	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	805	360	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	805	375	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	805	390	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	0
745	295
745	390
END_LINE
outputport

1	1	905	330	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	905	340	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	905	350	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	905	360	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	805	235	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	805	245	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	805	260	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	805	270	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	805	315	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	6	0	2	-1	0
760	275
760	315
END_LINE
finishport

3	1	905	235	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	905	245	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	905	260	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	905	270	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	905	315	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	905	285	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
5	0	0	0
_
_
This is a string constant.
Constant_String1
_
15	365	115	410
1	0
parameter
String Constant	3	_	_	click
EndOfValues
EndOfLabels
string constant

outputport

1	3	115	395	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	5	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
6	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
_
665	265	715	410
1	0
inputport

0	3	665	290	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	15	0	0	-1	0
655	70
655	290
END_LINE
inputport

0	3	665	325	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	24	0	0	-1	0
645	115
645	325
END_LINE
inputport

0	3	665	360	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	26	0	0	-1	0
635	160
635	360
END_LINE
inputport

0	3	665	395	3	0	1	mandatory= 0	0	1
i4

Run_Input4	1	EndOfFunc
BEGIN_LINE
1	28	0	0	-1	0
625	205
625	395
END_LINE
outputport

1	3	715	295	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
startport

2	3	665	275	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	29	0	2	-1	0
660	280
660	275
END_LINE
startport

2	3	665	310	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	29	1	2	-1	0
650	295
650	310
END_LINE
startport

2	3	665	345	2	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
1	29	2	2	-1	0
640	310
640	345
END_LINE
startport

2	3	665	380	3	0	1	mandatory= 0	0	1


Run_Input4	0	EndOfFunc
BEGIN_LINE
1	29	8	2	-1	0
630	405
630	380
END_LINE
finishport

3	3	715	275	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	Run_Input4	3	EndOfFunc
BEGIN_LINE
0	4	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
8	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
345	245	395	290
1	0
outputport

1	1	395	275	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	7	1	1	-1	0
END_LINE
startport

2	-1	345	255	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
340	285
340	255
END_LINE
startport

2	-1	345	275	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	395	255	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	7	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
15	0	0	0
_
_
This is a string constant.
Constant_String1
_
485	40	585	85
1	0
parameter
String Constant	3	_	_	1
EndOfValues
EndOfLabels
string constant

outputport

1	3	585	70	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	6	0	1	-1	0
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
_
490	85	590	130
1	0
parameter
String Constant	3	_	_	2
EndOfValues
EndOfLabels
string constant

outputport

1	3	590	115	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	6	1	1	-1	0
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
_
490	130	590	175
1	0
parameter
String Constant	3	_	_	3
EndOfValues
EndOfLabels
string constant

outputport

1	3	590	160	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	6	2	1	-1	0
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
28	0	0	0
_
_
This is a string constant.
Constant_String1
_
485	175	585	220
1	0
parameter
String Constant	3	_	_	ovf
EndOfValues
EndOfLabels
string constant

outputport

1	3	585	205	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	6	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_switch
_
29	0	0	0
_
_
Switches Event to Indexed Output
IndexedDemultiplexer_Event
_
440	250	490	424
1	0
inputport

0	1	440	270	1	0	1	mandatory= 0	0	1
Ind

Run	1	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
430	275
430	270
END_LINE
outputport

1	1	490	415	0	0	1	mandatory= 0	0	1
Off

Run	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	440	260	0	0	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
1	8	0	2	-1	0
435	255
435	260
END_LINE
finishport

3	1	490	280	0	0	1	mandatory= 0	0	1
1

Run	3	EndOfFunc
BEGIN_LINE
0	6	0	3	-1	0
END_LINE
finishport

3	1	490	295	1	0	1	mandatory= 0	0	1
2

Run	4	EndOfFunc
BEGIN_LINE
0	6	1	3	-1	0
END_LINE
finishport

3	1	490	310	2	0	1	mandatory= 0	0	1
3

Run	5	EndOfFunc
BEGIN_LINE
0	6	2	3	-1	0
END_LINE
finishport

3	1	490	325	3	0	1	mandatory= 0	0	1
4

Run	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	340	4	0	1	mandatory= 0	0	1
5

Run	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	355	5	0	1	mandatory= 0	0	1
6

Run	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	370	6	0	1	mandatory= 0	0	1
7

Run	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	385	7	0	1	mandatory= 0	0	1
8

Run	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	490	405	8	0	1	mandatory= 0	0	1
Ovf

Run	11	EndOfFunc
BEGIN_LINE
0	6	3	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
