#V:2.0.0

CanvasSizeXY
2000	2000

IconData
BEGIN_BLOCK

stringfn_formats8
_
1	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
245	160	295	299
1	0
parameter
Format	3	_	_	%s%s%s%s%s%s%s%s
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	245	190	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	2	0	0	-1	0
235	110
235	190
END_LINE
inputport

0	3	245	200	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	0
225	170
225	200
END_LINE
inputport

0	3	245	210	2	0	1	mandatory= 0	0	1
s3

run	3	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
215	230
215	210
END_LINE
inputport

0	3	245	220	3	0	1	mandatory= 0	0	1
s4

run	4	EndOfFunc
BEGIN_LINE
1	10	0	0	-1	0
205	280
205	220
END_LINE
inputport

0	3	245	230	4	0	1	mandatory= 0	0	1
s5

run	5	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
195	340
195	230
END_LINE
inputport

0	3	245	240	5	0	1	mandatory= 0	0	1
s6

run	6	EndOfFunc
BEGIN_LINE
1	14	0	0	-1	0
185	385
185	240
END_LINE
inputport

0	3	245	250	6	0	1	mandatory= 0	0	1
s7

run	7	EndOfFunc
BEGIN_LINE
1	18	0	0	-1	0
175	435
175	250
END_LINE
inputport

0	3	245	260	7	0	1	mandatory= 0	0	1
s8

run	8	EndOfFunc
BEGIN_LINE
1	12	0	0	-1	0
165	490
165	260
END_LINE
inputport

0	3	245	280	8	0	1	mandatory= 0	0	1
fmt

run	9	EndOfFunc
BEGIN_LINE
1	31	0	0	-1	0
365	415
365	495
235	495
235	280
END_LINE
outputport

1	3	295	190	0	0	1	mandatory= 0	0	1


run	10	EndOfFunc
BEGIN_LINE
0	19	5	1	-1	0
END_LINE
startport

2	3	245	170	0	1	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	3	295	170	0	0	1	mandatory= 0	0	1


run	11	EndOfFunc
BEGIN_LINE
0	19	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
2	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
50	80	150	125
1	0
parameter
String Constant	3	_	_	Test1
EndOfValues
EndOfLabels
string constant

outputport

1	3	150	110	0	0	1	mandatory= 0	0	1


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
6	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
50	140	150	185
1	0
parameter
String Constant	3	_	_	Test2
EndOfValues
EndOfLabels
string constant

outputport

1	3	150	170	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
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
8	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
45	200	145	245
1	0
parameter
String Constant	3	_	_	Test3
EndOfValues
EndOfLabels
string constant

outputport

1	3	145	230	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
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
10	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
60	250	160	295
1	0
parameter
String Constant	3	_	_	Test4
EndOfValues
EndOfLabels
string constant

outputport

1	3	160	280	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	3	1	-1	0
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
12	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
70	460	170	505
1	0
parameter
String Constant	3	_	_	Test8
EndOfValues
EndOfLabels
string constant

outputport

1	3	170	490	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	7	1	-1	0
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
14	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
60	355	160	400
1	0
parameter
String Constant	3	_	_	Test6
EndOfValues
EndOfLabels
string constant

outputport

1	3	160	385	0	0	1	mandatory= 0	0	1


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

const_s1
_
16	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
65	310	165	355
1	0
parameter
String Constant	3	_	_	Test5
EndOfValues
EndOfLabels
string constant

outputport

1	3	165	340	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	4	1	-1	0
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
65	405	165	450
1	0
parameter
String Constant	3	_	_	Test7
EndOfValues
EndOfLabels
string constant

outputport

1	3	165	435	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	6	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
19	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
400	140	500	314
1	0
parameter
Widget Tag	5	_	_	TestAllInoutFormat
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

0	1	400	245	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	400	255	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	400	265	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	400	275	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	400	290	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	400	305	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	1	0	0	-1	0
340	190
340	305
END_LINE
outputport

1	1	500	245	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	500	255	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	500	265	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	500	275	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	400	150	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	400	160	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	400	175	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	400	185	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	400	230	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
355	170
355	230
END_LINE
finishport

3	1	500	150	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	500	160	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	500	175	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	500	185	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	500	230	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	500	200	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	500	210	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
31	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
255	385	355	430
1	0
parameter
String Constant	3	_	_	Do1=[%s],Do2=[%s],Do3=[%s],Do4=[%s],Do5=[%s],Do6=[%s],Do7=[%s],Do8=[%s]
EndOfValues
EndOfLabels
string constant

outputport

1	3	355	415	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	8	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8
_
44	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
800	165	850	304
1	0
parameter
Format	3	_	_	Do1=[%s],Do2=[%s],Do3=[%s],Do4=[%s],Do5=[%s],Do6=[%s],Do7=[%s],Do8=[%s]
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	800	195	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	45	0	0	-1	0
790	115
790	195
END_LINE
inputport

0	3	800	205	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
1	46	0	0	-1	0
780	175
780	205
END_LINE
inputport

0	3	800	215	2	0	1	mandatory= 0	0	1
s3

run	3	EndOfFunc
BEGIN_LINE
1	47	0	0	-1	0
770	235
770	215
END_LINE
inputport

0	3	800	225	3	0	1	mandatory= 0	0	1
s4

run	4	EndOfFunc
BEGIN_LINE
1	48	0	0	-1	0
760	285
760	225
END_LINE
inputport

0	3	800	235	4	0	1	mandatory= 0	0	1
s5

run	5	EndOfFunc
BEGIN_LINE
1	51	0	0	-1	0
750	345
750	235
END_LINE
inputport

0	3	800	245	5	0	1	mandatory= 0	0	1
s6

run	6	EndOfFunc
BEGIN_LINE
1	50	0	0	-1	0
740	390
740	245
END_LINE
inputport

0	3	800	255	6	0	1	mandatory= 0	0	1
s7

run	7	EndOfFunc
BEGIN_LINE
1	52	0	0	-1	0
730	440
730	255
END_LINE
inputport

0	3	800	265	7	0	1	mandatory= 0	0	1
s8

run	8	EndOfFunc
BEGIN_LINE
1	49	0	0	-1	0
720	495
720	265
END_LINE
inputport

0	3	800	285	8	0	1	mandatory= 0	0	1
fmt

run	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	850	195	0	0	1	mandatory= 0	0	1


run	10	EndOfFunc
BEGIN_LINE
0	19	5	1	-1	0
END_LINE
startport

2	3	800	175	0	1	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	3	850	175	0	0	1	mandatory= 0	0	1


run	11	EndOfFunc
BEGIN_LINE
0	19	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
45	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
605	85	705	130
1	0
parameter
String Constant	3	_	_	Test1
EndOfValues
EndOfLabels
string constant

outputport

1	3	705	115	0	0	1	mandatory= 0	0	1


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
46	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
605	145	705	190
1	0
parameter
String Constant	3	_	_	Test2
EndOfValues
EndOfLabels
string constant

outputport

1	3	705	175	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
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
47	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
600	205	700	250
1	0
parameter
String Constant	3	_	_	Test3
EndOfValues
EndOfLabels
string constant

outputport

1	3	700	235	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
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
48	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
615	255	715	300
1	0
parameter
String Constant	3	_	_	Test4
EndOfValues
EndOfLabels
string constant

outputport

1	3	715	285	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	3	1	-1	0
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
49	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
625	465	725	510
1	0
parameter
String Constant	3	_	_	Test8
EndOfValues
EndOfLabels
string constant

outputport

1	3	725	495	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	7	1	-1	0
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
50	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
615	360	715	405
1	0
parameter
String Constant	3	_	_	Test6
EndOfValues
EndOfLabels
string constant

outputport

1	3	715	390	0	0	1	mandatory= 0	0	1


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

const_s1
_
51	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
620	315	720	360
1	0
parameter
String Constant	3	_	_	Test5
EndOfValues
EndOfLabels
string constant

outputport

1	3	720	345	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	4	1	-1	0
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
52	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
620	410	720	455
1	0
parameter
String Constant	3	_	_	Test7
EndOfValues
EndOfLabels
string constant

outputport

1	3	720	440	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	6	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
53	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
955	145	1055	319
1	0
parameter
Widget Tag	5	_	_	TestAllParameterFormat
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

0	1	955	250	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	260	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	270	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	280	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	295	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	955	310	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	44	0	0	-1	0
895	195
895	310
END_LINE
outputport

1	1	1055	250	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	260	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	270	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	280	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	955	155	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	165	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	180	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	190	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	235	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	44	0	2	-1	0
910	175
910	235
END_LINE
finishport

3	1	1055	155	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	165	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	180	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	190	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	235	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	205	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	1055	215	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8
_
77	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
220	595	270	734
1	0
parameter
Format	3	_	_	%s%s%s%s%s%s%s%s
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	220	625	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	78	0	0	-1	0
210	545
210	625
END_LINE
inputport

0	3	220	635	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
1	79	0	0	-1	0
200	605
200	635
END_LINE
inputport

0	3	220	645	2	0	1	mandatory= 0	0	1
s3

run	3	EndOfFunc
BEGIN_LINE
1	80	0	0	-1	0
190	665
190	645
END_LINE
inputport

0	3	220	655	3	0	1	mandatory= 0	0	1
s4

run	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	220	665	4	0	1	mandatory= 0	0	1
s5

run	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	220	675	5	0	1	mandatory= 0	0	1
s6

run	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	220	685	6	0	1	mandatory= 0	0	1
s7

run	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	220	695	7	0	1	mandatory= 0	0	1
s8

run	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	220	715	8	0	1	mandatory= 0	0	1
fmt

run	9	EndOfFunc
BEGIN_LINE
1	87	0	0	-1	0
190	790
190	715
END_LINE
outputport

1	3	270	625	0	0	1	mandatory= 0	0	1


run	10	EndOfFunc
BEGIN_LINE
0	19	5	1	-1	0
END_LINE
startport

2	3	220	605	0	1	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	3	270	605	0	0	1	mandatory= 0	0	1


run	11	EndOfFunc
BEGIN_LINE
0	19	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
78	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
25	515	125	560
1	0
parameter
String Constant	3	_	_	Test1
EndOfValues
EndOfLabels
string constant

outputport

1	3	125	545	0	0	1	mandatory= 0	0	1


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
79	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
25	575	125	620
1	0
parameter
String Constant	3	_	_	Test2
EndOfValues
EndOfLabels
string constant

outputport

1	3	125	605	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
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
80	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
20	635	120	680
1	0
parameter
String Constant	3	_	_	Test3
EndOfValues
EndOfLabels
string constant

outputport

1	3	120	665	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
86	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
375	575	475	749
1	0
parameter
Widget Tag	5	_	_	PartialConnectsAndLess%s
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

0	1	375	680	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	690	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	700	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	710	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	725	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	375	740	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	77	0	0	-1	0
315	625
315	740
END_LINE
outputport

1	1	475	680	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	690	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	700	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	710	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	375	585	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	595	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	610	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	620	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	665	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	77	0	2	-1	0
330	605
330	665
END_LINE
finishport

3	1	475	585	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	595	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	610	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	620	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	665	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	635	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	475	645	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
87	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
35	760	135	805
1	0
parameter
String Constant	3	_	_	Do1=[%s],Do2=[%s],Do3=[%s],Do4=[%s],Do5=[%s],Do6=[%s],
EndOfValues
EndOfLabels
string constant

outputport

1	3	135	790	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	8	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
