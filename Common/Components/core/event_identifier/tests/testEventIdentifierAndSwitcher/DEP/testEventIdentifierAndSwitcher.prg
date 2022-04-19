CanvasSizeXY
2150	2100

IconData
BEGIN_BLOCK

event_identifier
_
66	0	0	0
_
_
Triggering Event Identified by Index Output
IndexedMultiplexer_Event
_
1070	845	1120	1019
1	0
parameter
Requires Resetting	0	_	_	1
EndOfValues
EndOfLabels
Requires manual resetting each time it is triggered before it can be triggered again.

outputport

1	1	1120	865	0	0	1	mandatory= 0	0	1
Ind

Run_Input1	1	Run_Input2	1	Run_Input3	1	Run_Input4	1	Run_Input5	1	Run_Input6	1	Run_Input7	1	Run_Input8	1	EndOfFunc
BEGIN_LINE
0	101	0	1	-1	0
END_LINE
startport

2	-1	1070	875	4	0	1	mandatory= 0	0	1
1

Run_Input1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1070	890	6	0	1	mandatory= 0	0	1
2

Run_Input2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1070	905	0	0	1	mandatory= 0	0	1
3

Run_Input3	0	EndOfFunc
BEGIN_LINE
1	70	5	2	-1	0
1000	775
1000	905
END_LINE
startport

2	-1	1070	920	1	0	1	mandatory= 0	0	1
4

Run_Input4	0	EndOfFunc
BEGIN_LINE
1	71	5	2	-1	0
1000	965
1000	920
END_LINE
startport

2	-1	1070	935	5	0	1	mandatory= 0	0	1
5

Run_Input5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1070	950	2	0	1	mandatory= 0	0	1
6

Run_Input6	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1070	965	7	0	1	mandatory= 0	0	1
7

Run_Input7	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1070	980	3	0	1	mandatory= 0	0	1
8

Run_Input8	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	1070	1000	8	0	1	mandatory= 0	0	1
Reset

Reset	0	EndOfFunc
BEGIN_LINE
1	101	9	2	-1	0
1590	855
1590	1160
1035	1160
1035	1000
END_LINE
finishport

3	-1	1120	855	0	0	1	mandatory= 0	0	1
Go

Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	Run_Input5	2	Run_Input6	2	Run_Input7	2	Run_Input8	2	EndOfFunc
BEGIN_LINE
0	101	0	3	-1	0
END_LINE
finishport

3	1	1120	1000	1	0	1	mandatory= 0	0	1
--

Reset	1	EndOfFunc
BEGIN_LINE
0	67	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

EventCounter_versatile1
_
67	0	0	0
_
_
event counter providing option to set a maximum and minimum threshold with an input value.
EventCounterVersatile1
_
1200	975	1250	1095
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

0	1	1200	1075	0	0	1	mandatory= 0	0	1
max

Run_CountEventCounter	1	Run_DecrementEventCounter	1	Run_ResetEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1200	1085	1	0	1	mandatory= 0	0	1
min

Run_CountEventCounter	2	Run_DecrementEventCounter	2	Run_ResetEventCounter	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1250	1080	0	0	1	mandatory= 0	0	1
cnt

Run_CountEventCounter	3	Run_DecrementEventCounter	3	Run_ResetEventCounter	3	EndOfFunc
BEGIN_LINE
0	85	5	1	-1	0
END_LINE
startport

2	-1	1200	985	0	1	1	mandatory= 0	0	1
start

Run_StartEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1200	1000	0	0	1	mandatory= 0	0	1
inc

Run_CountEventCounter	0	EndOfFunc
BEGIN_LINE
1	66	0	2	-1	0
1165	855
1165	1000
END_LINE
startport

2	1	1200	1025	2	0	1	mandatory= 0	0	1
dec

Run_DecrementEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	1200	1050	3	0	1	mandatory= 0	0	1
rst

Run_ResetEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	1	2	-1	0
END_LINE
startport

2	1	1200	1060	4	0	1	mandatory= 0	0	1
stop

Run_StopEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1250	1000	0	0	1	mandatory= 0	0	1
--

Run_CountEventCounter	4	EndOfFunc
BEGIN_LINE
0	85	4	3	-1	0
END_LINE
finishport

3	1	1250	1025	1	0	1	mandatory= 0	0	1
--

Run_DecrementEventCounter	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1250	1010	2	0	1	mandatory= 0	0	1
ovf

Run_CountEventCounter	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1250	1035	3	0	1	mandatory= 0	0	1
ovf

Run_DecrementEventCounter	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1250	985	4	0	1	mandatory= 0	0	1
--

Run_StartEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1250	1050	5	0	1	mandatory= 0	0	1
--

Run_ResetEventCounter	4	EndOfFunc
BEGIN_LINE
0	100	1	3	-1	0
END_LINE
finishport

3	1	1250	1060	6	0	1	mandatory= 0	0	1
--

Run_StopEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
69	0	0	0
index
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1250	590	1350	764
1	0
parameter
Widget Tag	5	_	_	widget0
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

0	1	1250	695	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1250	705	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1250	715	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1250	725	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1250	740	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1250	755	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	66	0	0	-1	0
1145	865
1145	755
END_LINE
outputport

1	1	1350	695	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1350	705	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1350	715	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1350	725	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1250	600	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1250	610	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1250	625	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1250	635	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1250	680	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	66	0	2	-1	0
1165	855
1165	680
END_LINE
finishport

3	1	1350	600	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1350	610	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1350	625	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1350	635	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1350	680	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1350	650	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1350	660	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
70	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
_
760	715	860	880
1	0
parameter
Widget Tag	5	_	_	widget1
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

0	1	760	820	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	760	830	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	760	840	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	760	850	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	760	865	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	860	820	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	860	830	1	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	860	840	2	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	860	850	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	760	725	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	735	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	750	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	760	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	805	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	860	725	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	860	735	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	860	750	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	860	760	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	860	805	4	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	860	775	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	66	0	3	-1	0
END_LINE
finishport

3	1	860	785	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
71	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
_
755	905	855	1070
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

0	1	755	1010	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	1020	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	1030	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	1040	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	1055	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	855	1010	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	855	1020	1	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	855	1030	2	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	855	1040	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	755	915	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	925	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	940	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	950	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	995	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	855	915	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	925	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	940	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	950	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	995	4	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	965	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	66	1	3	-1	0
END_LINE
finishport

3	1	855	975	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
85	0	0	0
count
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1355	950	1455	1124
1	0
parameter
Widget Tag	5	_	_	widget4
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

0	1	1355	1055	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1355	1065	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1355	1075	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1355	1085	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1355	1100	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1355	1115	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	67	0	0	-1	0
1285	1080
1285	1115
END_LINE
outputport

1	1	1455	1055	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1455	1065	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1455	1075	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1455	1085	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1355	960	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1355	970	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1355	985	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1355	995	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1355	1040	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	100	0	2	-1	0
1345	1036
1345	1040
END_LINE
finishport

3	1	1455	960	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1455	970	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1455	985	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1455	995	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1455	1040	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	101	0	3	-1	0
END_LINE
finishport

3	1	1455	1010	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1455	1020	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
100	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
1295	1015	1315	1055
1	0
startport

2	-1	1295	1025	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	67	0	2	-1	0
1290	1000
1290	1025
END_LINE
startport

2	-1	1295	1045	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	67	5	2	-1	0
1280	1050
1280	1045
END_LINE
finishport

3	-1	1315	1036	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	85	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_switch
_
101	0	0	0
_
_
Switches Event to Indexed Output
IndexedDemultiplexer_Event
_
1505	845	1555	1019
1	0
inputport

0	1	1505	865	0	0	1	mandatory= 0	0	1
Ind

Run	1	EndOfFunc
BEGIN_LINE
1	66	0	0	-1	0
END_LINE
outputport

1	1	1555	1010	0	0	1	mandatory= 0	0	1
Off

Run	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1505	855	0	0	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
1	85	4	2	-1	0
1485	1040
1485	855
END_LINE
finishport

3	1	1555	875	0	0	1	mandatory= 0	0	1
1

Run	3	EndOfFunc
BEGIN_LINE
0	111	0	3	-1	0
END_LINE
finishport

3	1	1555	890	1	0	1	mandatory= 0	0	1
2

Run	4	EndOfFunc
BEGIN_LINE
0	111	1	3	-1	0
END_LINE
finishport

3	1	1555	905	2	0	1	mandatory= 0	0	1
3

Run	5	EndOfFunc
BEGIN_LINE
0	111	2	3	-1	0
END_LINE
finishport

3	1	1555	920	3	0	1	mandatory= 0	0	1
4

Run	6	EndOfFunc
BEGIN_LINE
0	111	3	3	-1	0
END_LINE
finishport

3	1	1555	935	4	0	1	mandatory= 0	0	1
5

Run	7	EndOfFunc
BEGIN_LINE
0	111	4	3	-1	0
END_LINE
finishport

3	1	1555	950	5	0	1	mandatory= 0	0	1
6

Run	8	EndOfFunc
BEGIN_LINE
0	111	5	3	-1	0
END_LINE
finishport

3	1	1555	965	6	0	1	mandatory= 0	0	1
7

Run	9	EndOfFunc
BEGIN_LINE
0	111	6	3	-1	0
END_LINE
finishport

3	1	1555	980	7	0	1	mandatory= 0	0	1
8

Run	10	EndOfFunc
BEGIN_LINE
0	111	7	3	-1	0
END_LINE
finishport

3	1	1555	1000	8	0	1	mandatory= 0	0	1
Ovf

Run	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1555	855	9	0	1	mandatory= 0	0	1
--

Run	12	EndOfFunc
BEGIN_LINE
0	66	8	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_identifier
_
111	0	0	0
_
_
Triggering Event Identified by Index Output
IndexedMultiplexer_Event
_
1675	845	1725	1019
1	0
parameter
Requires Resetting	0	_	_	0
EndOfValues
EndOfLabels
Requires manual resetting each time it is triggered before it can be triggered again.

outputport

1	1	1725	865	0	0	1	mandatory= 0	0	1
Ind

Run_Input1	1	Run_Input2	1	Run_Input3	1	Run_Input4	1	Run_Input5	1	Run_Input6	1	Run_Input7	1	Run_Input8	1	EndOfFunc
BEGIN_LINE
0	122	5	1	-1	0
END_LINE
startport

2	-1	1675	875	4	0	1	mandatory= 0	0	1
1

Run_Input1	0	EndOfFunc
BEGIN_LINE
1	101	0	2	-1	0
1670	875
1670	875
END_LINE
startport

2	-1	1675	890	6	0	1	mandatory= 0	0	1
2

Run_Input2	0	EndOfFunc
BEGIN_LINE
1	101	1	2	-1	0
1660	890
1660	890
END_LINE
startport

2	-1	1675	905	0	0	1	mandatory= 0	0	1
3

Run_Input3	0	EndOfFunc
BEGIN_LINE
1	101	2	2	-1	0
1650	905
1650	905
END_LINE
startport

2	-1	1675	920	1	0	1	mandatory= 0	0	1
4

Run_Input4	0	EndOfFunc
BEGIN_LINE
1	101	3	2	-1	0
1640	920
1640	920
END_LINE
startport

2	-1	1675	935	5	0	1	mandatory= 0	0	1
5

Run_Input5	0	EndOfFunc
BEGIN_LINE
1	101	4	2	-1	0
1630	935
1630	935
END_LINE
startport

2	-1	1675	950	2	0	1	mandatory= 0	0	1
6

Run_Input6	0	EndOfFunc
BEGIN_LINE
1	101	5	2	-1	0
1620	950
1620	950
END_LINE
startport

2	-1	1675	965	7	0	1	mandatory= 0	0	1
7

Run_Input7	0	EndOfFunc
BEGIN_LINE
1	101	6	2	-1	0
1610	965
1610	965
END_LINE
startport

2	-1	1675	980	3	0	1	mandatory= 0	0	1
8

Run_Input8	0	EndOfFunc
BEGIN_LINE
1	101	7	2	-1	0
1600	980
1600	980
END_LINE
startport

2	1	1675	1000	8	0	1	mandatory= 0	0	1
Reset

Reset	0	EndOfFunc
BEGIN_LINE
0	101	9	2	-1	0
END_LINE
finishport

3	-1	1725	855	0	0	1	mandatory= 0	0	1
Go

Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	Run_Input5	2	Run_Input6	2	Run_Input7	2	Run_Input8	2	EndOfFunc
BEGIN_LINE
0	122	4	3	-1	0
END_LINE
finishport

3	1	1725	1000	1	0	1	mandatory= 0	0	1
--

Reset	1	EndOfFunc
BEGIN_LINE
0	67	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
122	0	0	0
count
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1820	775	1920	949
1	0
parameter
Widget Tag	5	_	_	widget5
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

0	1	1820	880	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1820	890	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1820	900	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1820	910	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1820	925	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1820	940	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	111	0	0	-1	0
1760	865
1760	940
END_LINE
outputport

1	1	1920	880	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1920	890	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1920	900	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1920	910	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1820	785	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1820	795	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1820	810	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1820	820	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1820	865	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	111	0	2	-1	0
1775	855
1775	865
END_LINE
finishport

3	1	1920	785	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1920	795	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1920	810	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1920	820	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1920	865	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	101	0	3	-1	0
END_LINE
finishport

3	1	1920	835	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1920	845	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
