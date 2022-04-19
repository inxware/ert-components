CanvasSizeXY
2300	2100

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
Requires Resetting	0	_	_	0
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
0	-1	9	2	-1	0
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
1400	840	1450	960
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

0	1	1400	940	0	0	1	mandatory= 0	0	1
max

Run_CountEventCounter	1	Run_DecrementEventCounter	1	Run_ResetEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1400	950	1	0	1	mandatory= 0	0	1
min

Run_CountEventCounter	2	Run_DecrementEventCounter	2	Run_ResetEventCounter	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1450	945	0	0	1	mandatory= 0	0	1
cnt

Run_CountEventCounter	3	Run_DecrementEventCounter	3	Run_ResetEventCounter	3	EndOfFunc
BEGIN_LINE
0	85	5	1	-1	0
END_LINE
startport

2	-1	1400	850	0	1	1	mandatory= 0	0	1
start

Run_StartEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	1400	865	0	0	1	mandatory= 0	0	1
inc

Run_CountEventCounter	0	EndOfFunc
BEGIN_LINE
1	123	0	2	-1	0
1385	880
1385	865
END_LINE
startport

2	1	1400	890	2	0	1	mandatory= 0	0	1
dec

Run_DecrementEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	1400	915	3	0	1	mandatory= 0	0	1
rst

Run_ResetEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	1	2	-1	0
END_LINE
startport

2	1	1400	925	4	0	1	mandatory= 0	0	1
stop

Run_StopEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1450	865	0	0	1	mandatory= 0	0	1
--

Run_CountEventCounter	4	EndOfFunc
BEGIN_LINE
0	85	4	3	-1	0
END_LINE
finishport

3	1	1450	890	1	0	1	mandatory= 0	0	1
--

Run_DecrementEventCounter	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1450	875	2	0	1	mandatory= 0	0	1
ovf

Run_CountEventCounter	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1450	900	3	0	1	mandatory= 0	0	1
ovf

Run_DecrementEventCounter	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1450	850	4	0	1	mandatory= 0	0	1
--

Run_StartEventCounter	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1450	915	5	0	1	mandatory= 0	0	1
--

Run_ResetEventCounter	4	EndOfFunc
BEGIN_LINE
0	100	1	3	-1	0
END_LINE
finishport

3	1	1450	925	6	0	1	mandatory= 0	0	1
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
1555	815	1655	989
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

0	1	1555	920	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1555	930	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1555	940	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1555	950	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1555	965	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1555	980	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	67	0	0	-1	0
1485	945
1485	980
END_LINE
outputport

1	1	1655	920	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1655	930	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1655	940	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1655	950	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1555	825	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1555	835	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1555	850	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1555	860	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1555	905	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	100	0	2	-1	0
1545	901
1545	905
END_LINE
finishport

3	1	1655	825	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1655	835	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1655	850	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1655	860	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1655	905	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	101	0	3	-1	0
END_LINE
finishport

3	1	1655	875	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	123	0	3	-1	0
END_LINE
finishport

3	1	1655	885	6	0	1	mandatory= 0	0	1
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
1495	880	1515	920
1	0
startport

2	-1	1495	890	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	67	0	2	-1	0
1490	865
1490	890
END_LINE
startport

2	-1	1495	910	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	67	5	2	-1	0
1480	915
1480	910
END_LINE
finishport

3	-1	1515	901	0	0	1	mandatory= 0	0	1


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
1820	865	1870	1039
1	0
inputport

0	1	1820	885	0	0	1	mandatory= 0	0	1
Ind

Run	1	EndOfFunc
BEGIN_LINE
1	123	0	0	-1	0
1330	945
1330	1090
1810	1090
1810	885
END_LINE
outputport

1	1	1870	1030	0	0	1	mandatory= 0	0	1
Off

Run	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1820	875	0	0	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
1	85	4	2	-1	0
1785	905
1785	875
END_LINE
finishport

3	1	1870	895	0	0	1	mandatory= 0	0	1
1

Run	3	EndOfFunc
BEGIN_LINE
0	111	0	3	-1	0
END_LINE
finishport

3	1	1870	910	1	0	1	mandatory= 0	0	1
2

Run	4	EndOfFunc
BEGIN_LINE
0	111	1	3	-1	0
END_LINE
finishport

3	1	1870	925	2	0	1	mandatory= 0	0	1
3

Run	5	EndOfFunc
BEGIN_LINE
0	111	2	3	-1	0
END_LINE
finishport

3	1	1870	940	3	0	1	mandatory= 0	0	1
4

Run	6	EndOfFunc
BEGIN_LINE
0	111	3	3	-1	0
END_LINE
finishport

3	1	1870	955	4	0	1	mandatory= 0	0	1
5

Run	7	EndOfFunc
BEGIN_LINE
0	111	4	3	-1	0
END_LINE
finishport

3	1	1870	970	5	0	1	mandatory= 0	0	1
6

Run	8	EndOfFunc
BEGIN_LINE
0	111	5	3	-1	0
END_LINE
finishport

3	1	1870	985	6	0	1	mandatory= 0	0	1
7

Run	9	EndOfFunc
BEGIN_LINE
0	111	6	3	-1	0
END_LINE
finishport

3	1	1870	1000	7	0	1	mandatory= 0	0	1
8

Run	10	EndOfFunc
BEGIN_LINE
0	111	7	3	-1	0
END_LINE
finishport

3	1	1870	1020	8	0	1	mandatory= 0	0	1
Ovf

Run	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1870	875	9	0	1	mandatory= 0	0	1
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
1990	865	2040	1039
1	0
parameter
Requires Resetting	0	_	_	0
EndOfValues
EndOfLabels
Requires manual resetting each time it is triggered before it can be triggered again.

outputport

1	1	2040	885	0	0	1	mandatory= 0	0	1
Ind

Run_Input1	1	Run_Input2	1	Run_Input3	1	Run_Input4	1	Run_Input5	1	Run_Input6	1	Run_Input7	1	Run_Input8	1	EndOfFunc
BEGIN_LINE
0	122	5	1	-1	0
END_LINE
startport

2	-1	1990	895	4	0	1	mandatory= 0	0	1
1

Run_Input1	0	EndOfFunc
BEGIN_LINE
1	101	0	2	-1	0
1985	895
1985	895
END_LINE
startport

2	-1	1990	910	6	0	1	mandatory= 0	0	1
2

Run_Input2	0	EndOfFunc
BEGIN_LINE
1	101	1	2	-1	0
1975	910
1975	910
END_LINE
startport

2	-1	1990	925	0	0	1	mandatory= 0	0	1
3

Run_Input3	0	EndOfFunc
BEGIN_LINE
1	101	2	2	-1	0
1965	925
1965	925
END_LINE
startport

2	-1	1990	940	1	0	1	mandatory= 0	0	1
4

Run_Input4	0	EndOfFunc
BEGIN_LINE
1	101	3	2	-1	0
1955	940
1955	940
END_LINE
startport

2	-1	1990	955	5	0	1	mandatory= 0	0	1
5

Run_Input5	0	EndOfFunc
BEGIN_LINE
1	101	4	2	-1	0
1945	955
1945	955
END_LINE
startport

2	-1	1990	970	2	0	1	mandatory= 0	0	1
6

Run_Input6	0	EndOfFunc
BEGIN_LINE
1	101	5	2	-1	0
1935	970
1935	970
END_LINE
startport

2	-1	1990	985	7	0	1	mandatory= 0	0	1
7

Run_Input7	0	EndOfFunc
BEGIN_LINE
1	101	6	2	-1	0
1925	985
1925	985
END_LINE
startport

2	-1	1990	1000	3	0	1	mandatory= 0	0	1
8

Run_Input8	0	EndOfFunc
BEGIN_LINE
1	101	7	2	-1	0
1915	1000
1915	1000
END_LINE
startport

2	1	1990	1020	8	0	1	mandatory= 0	0	1
Reset

Reset	0	EndOfFunc
BEGIN_LINE
0	101	9	2	-1	0
END_LINE
finishport

3	-1	2040	875	0	0	1	mandatory= 0	0	1
Go

Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	Run_Input5	2	Run_Input6	2	Run_Input7	2	Run_Input8	2	EndOfFunc
BEGIN_LINE
0	122	4	3	-1	0
END_LINE
finishport

3	1	2040	1020	1	0	1	mandatory= 0	0	1
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
2135	795	2235	969
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

0	1	2135	900	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2135	910	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2135	920	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2135	930	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2135	945	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2135	960	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	111	0	0	-1	0
2075	885
2075	960
END_LINE
outputport

1	1	2235	900	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2235	910	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2235	920	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2235	930	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	2135	805	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2135	815	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2135	830	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2135	840	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2135	885	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	111	0	2	-1	0
2090	875
2090	885
END_LINE
finishport

3	1	2235	805	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2235	815	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2235	830	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2235	840	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2235	885	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	101	0	3	-1	0
END_LINE
finishport

3	1	2235	855	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2235	865	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

buffer_fifoiiq
_
123	0	0	0
_
_
This is an integer fifo buffer.
FIFO_Int
_
1190	870	1290	959
1	0
parameter
Max. length	1	0	1000000	50
EndOfValues
EndOfLabels
maximum buffer length

inputport

0	1	1190	945	0	0	1	mandatory= 0	0	1
data

Run_PushFIFOInt	1	EndOfFunc
BEGIN_LINE
1	66	0	0	-1	0
1145	865
1145	945
END_LINE
outputport

1	1	1290	945	0	0	1	mandatory= 0	0	1
data

Run_PopFIFOInt	1	EndOfFunc
BEGIN_LINE
0	101	0	1	-1	0
END_LINE
startport

2	1	1190	880	1	0	1	mandatory= 0	0	1
pop

Run_PopFIFOInt	0	EndOfFunc
BEGIN_LINE
1	85	5	2	-1	0
1660	875
1660	810
1185	810
1185	880
END_LINE
startport

2	-1	1190	905	0	0	1	mandatory= 0	0	1
push

Run_PushFIFOInt	0	EndOfFunc
BEGIN_LINE
1	66	0	2	-1	0
1165	855
1165	905
END_LINE
startport

2	1	1190	930	2	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1290	880	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOInt	2	EndOfFunc
BEGIN_LINE
0	67	1	3	-1	0
END_LINE
finishport

3	1	1290	890	1	0	1	mandatory= 0	0	1
err

Run_PopFIFOInt	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	905	2	0	1	mandatory= 0	0	1
push

Run_PushFIFOInt	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	915	3	0	1	mandatory= 0	0	1
err

Run_PushFIFOInt	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	930	4	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOInt	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
