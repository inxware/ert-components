CanvasSizeXY
2130	2000

IconData
BEGIN_BLOCK

operator_acosrx
_
1	0	0	0
_
_
This is the arc cosine function.
ArcCosineReal
_
465	65	515	110
1	0
parameter
Radians:1, Degree:0	0	_	_	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	465	95	0	0	1	mandatory= 0	0	1


Run_ArcCosineReal	1	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
455	120
455	95
END_LINE
outputport

1	2	515	95	0	0	1	mandatory= 0	0	1


Run_ArcCosineReal	2	EndOfFunc
BEGIN_LINE
0	12	5	1	-1	0
END_LINE
startport

2	2	465	75	0	0	1	mandatory= 0	0	1


Run_ArcCosineReal	0	EndOfFunc
BEGIN_LINE
1	16	0	2	-1	0
460	100
460	75
END_LINE
finishport

3	2	515	75	0	0	1	mandatory= 0	0	1


Run_ArcCosineReal	3	EndOfFunc
BEGIN_LINE
0	4	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cmerbx1
_
3	0	0	0
_
_
This is a 2-input real comparator.
ComparatorGreaterEqualReal1
_
850	40	950	100
1	0
parameter
Constant	2	1.7E-308	1.7E+308	1.7E-308
EndOfValues
EndOfLabels
Comparator constant

inputport

0	2	850	70	0	0	1	mandatory= 1	0	1
A

Run_ComparatorGreaterEqualReal	1	EndOfFunc
BEGIN_LINE
1	10	0	0	-1	0
840	65
840	70
END_LINE
inputport

0	2	850	85	1	0	1	mandatory= 1	0	1
B

Run_ComparatorGreaterEqualReal	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	950	85	0	0	1	mandatory= 0	0	1


Run_ComparatorGreaterEqualReal	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	850	50	0	0	1	mandatory= 0	0	1


Run_ComparatorGreaterEqualReal	0	EndOfFunc
BEGIN_LINE
1	10	0	2	-1	0
845	45
845	50
END_LINE
finishport

3	0	950	50	0	0	1	mandatory= 0	0	1
t

Run_ComparatorGreaterEqualReal	4	EndOfFunc
BEGIN_LINE
0	14	4	3	-1	0
END_LINE
finishport

3	0	950	65	1	0	1	mandatory= 0	0	1
f

Run_ComparatorGreaterEqualReal	5	EndOfFunc
BEGIN_LINE
0	15	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cosrx
_
4	0	0	0
_
_
This is the cosine function.
CosineReal
_
545	65	595	110
1	0
parameter
Radians:1, Degree:0	0	_	_	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	545	95	0	0	1	mandatory= 0	0	1


Run_CosineReal	1	EndOfFunc
BEGIN_LINE
1	1	0	0	-1	0
535	95
535	95
END_LINE
outputport

1	2	595	95	0	0	1	mandatory= 0	0	1


Run_CosineReal	2	EndOfFunc
BEGIN_LINE
0	8	0	1	-1	0
END_LINE
startport

2	2	545	75	0	0	1	mandatory= 0	0	1


Run_CosineReal	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
540	75
540	75
END_LINE
finishport

3	2	595	75	0	0	1	mandatory= 0	0	1


Run_CosineReal	3	EndOfFunc
BEGIN_LINE
0	8	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranirx
_
6	0	0	0
_
_
This is an integer to real converter.
ConvertorIntToReal
_
175	95	275	140
1	0
inputport

0	1	175	125	0	0	1	mandatory= 0	0	1


Run	1	EndOfFunc
BEGIN_LINE
1	11	0	0	-1	0
160	210
160	125
END_LINE
outputport

1	2	275	125	0	0	1	mandatory= 0	0	1


Run	2	EndOfFunc
BEGIN_LINE
0	16	0	1	-1	0
END_LINE
startport

2	2	175	105	0	0	1	mandatory= 0	0	1


Run	0	EndOfFunc
BEGIN_LINE
1	11	0	2	-1	0
155	130
155	105
END_LINE
finishport

3	2	275	105	0	0	1	mandatory= 0	0	1


Run	3	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
7	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
45	20	65	60
1	0
startport

2	-1	45	30	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	11	4	2	-1	0
150	115
150	65
30	65
30	30
END_LINE
startport

2	-1	45	50	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	12	4	2	-1	1
1230	110
1230	55
30	55
30	50
END_LINE
finishport

3	-1	65	41	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	5	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_subrx
_
8	0	0	0
_
_
This is a 2-input real subtractor.
SubtractionReal
_
650	45	700	105
1	0
inputport

0	2	650	75	0	0	1	mandatory= 1	0	1
+

Run_SubtractionReal	1	EndOfFunc
BEGIN_LINE
1	4	0	0	-1	0
620	95
620	75
END_LINE
inputport

0	2	650	90	1	0	1	mandatory= 1	0	1
-

Run_SubtractionReal	2	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
630	120
630	90
END_LINE
outputport

1	2	700	75	0	0	1	mandatory= 0	0	1


Run_SubtractionReal	3	EndOfFunc
BEGIN_LINE
0	12	5	1	-1	0
END_LINE
startport

2	2	650	55	0	0	1	mandatory= 0	0	1


Run_SubtractionReal	0	EndOfFunc
BEGIN_LINE
1	4	0	2	-1	0
615	75
615	55
END_LINE
finishport

3	2	700	55	0	0	1	mandatory= 0	0	1


Run_SubtractionReal	4	EndOfFunc
BEGIN_LINE
0	10	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_sqrrx
_
10	0	0	0
_
_
This is a 2-input real square.
SquareReal
_
770	35	820	80
1	0
inputport

0	2	770	65	0	0	1	mandatory= 0	0	1


Run_SquareReal	1	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
760	75
760	65
END_LINE
outputport

1	2	820	65	0	0	1	mandatory= 0	0	1


Run_SquareReal	2	EndOfFunc
BEGIN_LINE
0	12	5	1	-1	0
END_LINE
startport

2	2	770	45	0	0	1	mandatory= 0	0	1


Run_SquareReal	0	EndOfFunc
BEGIN_LINE
1	8	0	2	-1	0
765	55
765	45
END_LINE
finishport

3	2	820	45	0	0	1	mandatory= 0	0	1


Run_SquareReal	3	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

EventCounter_versatile1
_
11	0	0	0
_
_
event counter providing option to set a maximum and minimum threshold with an input value.
EventCounterVersatile1
_
85	105	135	225
1	0
parameter
Max. Threshold	1	-2147483648	2147483647	1000
EndOfValues
EndOfLabels
Threshold value for ovf event.

parameter
Min. Threshold	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
Threshold value for ovf event.

inputport

0	1	85	205	0	0	1	mandatory= 0	0	1
max

Run_CountEventCounter	1	Run_DecrementEventCounter	1	Run_ResetEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	85	215	1	0	1	mandatory= 0	0	1
min

Run_CountEventCounter	2	Run_DecrementEventCounter	2	Run_ResetEventCounter	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	135	210	0	0	1	mandatory= 0	0	1
cnt

Run_CountEventCounter	3	Run_DecrementEventCounter	3	Run_ResetEventCounter	3	EndOfFunc
BEGIN_LINE
0	6	0	1	-1	0
END_LINE
startport

2	-1	85	115	0	1	1	mandatory= 0	0	1
start

Run_StartEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	85	130	0	0	1	mandatory= 0	0	1
inc

Run_CountEventCounter	0	EndOfFunc
BEGIN_LINE
1	7	0	2	-1	0
65	41
65	130
END_LINE
startport

2	1	85	155	2	0	1	mandatory= 0	0	1
dec

Run_DecrementEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	85	180	3	0	1	mandatory= 0	0	1
rst

Run_ResetEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	85	190	4	0	1	mandatory= 0	0	1
stop

Run_StopEventCounter	0	EndOfFunc
BEGIN_LINE
1	11	2	2	-1	0
150	140
150	140
70	140
70	190
END_LINE
finishport

3	1	135	130	0	0	1	mandatory= 0	0	1
--

Run_CountEventCounter	4	EndOfFunc
BEGIN_LINE
0	6	0	3	-1	0
END_LINE
finishport

3	1	135	155	1	0	1	mandatory= 0	0	1
--

Run_DecrementEventCounter	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	135	140	2	0	1	mandatory= 0	0	1
ovf

Run_CountEventCounter	5	EndOfFunc
BEGIN_LINE
0	15	3	3	-1	0
END_LINE
finishport

3	1	135	165	3	0	1	mandatory= 0	0	1
ovf

Run_DecrementEventCounter	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	135	115	4	0	1	mandatory= 0	0	1
--

Run_StartEventCounter	1	EndOfFunc
BEGIN_LINE
0	7	0	3	-1	0
END_LINE
finishport

3	1	135	180	5	0	1	mandatory= 0	0	1
--

Run_ResetEventCounter	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	135	190	6	0	1	mandatory= 0	0	1
--

Run_StopEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
12	0	0	0
_
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
1115	20	1215	194
1	0
parameter
Widget Tag	5	_	_	Degrees
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

0	1	1115	125	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	135	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	145	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	155	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	170	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	1115	185	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	1	0	0	-1	0
990	95
990	185
END_LINE
outputport

1	1	1215	125	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1215	135	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1215	145	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1215	155	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1115	30	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1115	40	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1115	55	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1115	65	4	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1115	110	3	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	13	0	2	-1	0
1080	61
1080	110
END_LINE
finishport

3	1	1215	30	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	40	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	55	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	65	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	110	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
finishport

3	1	1215	80	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	90	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
13	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
1030	40	1050	80
1	0
startport

2	-1	1030	50	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	3	0	2	-1	0
1025	50
1025	50
END_LINE
startport

2	-1	1030	70	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	3	1	2	-1	0
1015	65
1015	70
END_LINE
finishport

3	-1	1050	61	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	12	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
15	0	0	0
_
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
1115	230	1215	404
1	0
parameter
Widget Tag	5	_	_	Difference
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

0	1	1115	335	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	345	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	355	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	365	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1115	380	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	1115	395	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	10	0	0	-1	0
840	65
840	395
END_LINE
outputport

1	1	1215	335	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1215	345	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1215	355	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1215	365	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1115	240	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1115	250	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1115	265	2	0	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
1	3	1	2	-1	0
1090	65
1090	265
END_LINE
startport

2	-1	1115	275	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	2	2	-1	0
END_LINE
startport

2	-1	1115	320	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	3	1	2	-1	0
990	65
990	320
END_LINE
finishport

3	1	1215	240	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	250	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	265	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	275	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	320	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	290	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1215	300	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_divrx
_
16	0	0	0
_
_
This is a 2-input real divider.
DivisionReal
_
355	90	405	150
1	0
inputport

0	2	355	120	0	0	1	mandatory= 1	0	1
N

Run_DivisionReal	1	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	1
345	125
345	120
END_LINE
inputport

0	2	355	135	1	0	1	mandatory= 1	0	1
D

Run_DivisionReal	2	EndOfFunc
BEGIN_LINE
1	17	0	0	-1	0
335	300
335	135
END_LINE
outputport

1	2	405	120	0	0	1	mandatory= 0	0	1


Run_DivisionReal	3	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
startport

2	2	355	100	0	0	1	mandatory= 0	0	1


Run_DivisionReal	0	EndOfFunc
BEGIN_LINE
1	6	0	2	-1	1
350	105
350	100
END_LINE
finishport

3	2	405	100	0	0	1	mandatory= 0	0	1


Run_DivisionReal	4	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
17	0	0	0
_
_
This is a real constant.
Constant_Real1
_
205	270	305	315
1	0
parameter
Real Constant	2	-1.7E308	1.7E+308	10000
EndOfValues
EndOfLabels
real constant

outputport

1	2	305	300	0	0	1	mandatory= 0	0	1


Run_ConstantReal	1	EndOfFunc
BEGIN_LINE
0	16	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantReal	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
