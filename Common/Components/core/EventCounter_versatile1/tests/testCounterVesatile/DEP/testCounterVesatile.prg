CanvasSizeXY
2150	2040

IconData
BEGIN_BLOCK

gui_text_int2
_
1	0	0	0
incCnt
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
690	175	790	349
1	0
parameter
Widget Tag	5	_	_	incCnt
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	690	265	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	280	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	295	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	310	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	325	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	340	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
630	270
630	340
END_LINE
outputport

1	1	790	265	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	790	280	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	790	295	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	790	310	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	690	185	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	200	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	215	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	230	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	245	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	16	0	2	-1	0
645	250
645	245
END_LINE
finishport

3	1	790	185	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	200	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	215	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	230	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	245	5	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	255	5	0	1	mandatory= 0	0	1
click
clickInc
create	6	EndOfFunc
BEGIN_LINE
0	86	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
2	0	0	0
_
_
Creates periodic timed events.
time_clock
_
205	190	265	280
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	1000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	1
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	205	235	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	205	250	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	205	265	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	205	200	0	1	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	205	215	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	265	230	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	3	2	3	-1	0
END_LINE
finishport

3	0	265	200	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
finishport

3	0	265	215	2	0	1	mandatory= 0	0	1


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

EventCounter_versatile1
_
3	0	0	0
_
_
event counter providing option to set a maximum and minimum threshold with an input value.
EventCounterVersatile1
_
340	770	390	890
1	0
parameter
Max. Threshold	1	-2147483648	2147483647	30
EndOfValues
EndOfLabels
Threshold value for ovf event.

parameter
Min. Threshold	1	-2147483648	2147483647	-30
EndOfValues
EndOfLabels
Threshold value for ovf event.

inputport

0	1	340	870	0	0	1	mandatory= 0	0	1
max

Run_CountEventCounter	1	Run_DecrementEventCounter	1	Run_ResetEventCounter	1	EndOfFunc
BEGIN_LINE
1	86	0	0	-1	0
320	870
320	870
END_LINE
inputport

0	1	340	880	1	0	1	mandatory= 0	0	1
min

Run_CountEventCounter	2	Run_DecrementEventCounter	2	Run_ResetEventCounter	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	390	875	0	0	1	mandatory= 0	0	1
cnt

Run_CountEventCounter	3	Run_DecrementEventCounter	3	Run_ResetEventCounter	3	EndOfFunc
BEGIN_LINE
0	24	5	1	-1	0
END_LINE
startport

2	-1	340	780	0	0	1	mandatory= 0	0	1
start
clickStart
Run_StartEventCounter	0	EndOfFunc
BEGIN_LINE
1	49	5	2	-1	0
1070	615
1070	725
325	725
325	780
END_LINE
startport

2	1	340	795	1	0	1	mandatory= 0	0	1
inc

Run_CountEventCounter	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
305	230
305	795
END_LINE
startport

2	1	340	820	2	0	1	mandatory= 0	0	1
dec

Run_DecrementEventCounter	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	340	845	3	0	1	mandatory= 0	0	1
rst
clickReset
Run_ResetEventCounter	0	EndOfFunc
BEGIN_LINE
1	80	5	2	-1	0
1055	1325
1055	1270
325	1270
325	845
END_LINE
startport

2	1	340	855	4	0	1	mandatory= 0	0	1
stop
clickStop
Run_StopEventCounter	0	EndOfFunc
BEGIN_LINE
1	83	5	2	-1	0
1060	1555
1060	1500
325	1500
325	855
END_LINE
finishport

3	1	390	795	0	0	1	mandatory= 0	0	1
--

Run_CountEventCounter	4	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
finishport

3	1	390	820	3	0	1	mandatory= 0	0	1
--

Run_DecrementEventCounter	4	EndOfFunc
BEGIN_LINE
0	36	0	3	-1	0
END_LINE
finishport

3	1	390	805	2	0	1	mandatory= 0	0	1
ovf

Run_CountEventCounter	5	EndOfFunc
BEGIN_LINE
0	8	4	3	-1	0
END_LINE
finishport

3	1	390	830	1	0	1	mandatory= 0	0	1
ovf

Run_DecrementEventCounter	5	EndOfFunc
BEGIN_LINE
0	35	0	3	-1	0
END_LINE
finishport

3	1	390	780	4	0	1	mandatory= 0	0	1
--

Run_StartEventCounter	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
finishport

3	1	390	845	5	0	1	mandatory= 0	0	1
--

Run_ResetEventCounter	4	EndOfFunc
BEGIN_LINE
0	81	0	3	-1	0
END_LINE
finishport

3	1	390	855	6	0	1	mandatory= 0	0	1
--

Run_StopEventCounter	1	EndOfFunc
BEGIN_LINE
0	84	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
8	0	0	0
incOvf
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
700	395	800	569
1	0
parameter
Widget Tag	5	_	_	incOvf
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	700	485	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	500	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	515	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	530	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	545	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	560	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	9	0	0	-1	0
640	485
640	560
END_LINE
outputport

1	1	800	485	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	800	500	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	800	515	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	800	530	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	700	405	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	420	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	435	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	450	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	465	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	9	0	2	-1	0
655	465
655	465
END_LINE
finishport

3	1	800	405	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	420	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	435	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	450	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	465	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	475	5	0	1	mandatory= 0	0	1
click
clickOvf
create	6	EndOfFunc
BEGIN_LINE
0	86	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
9	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
530	455	580	500
1	0
outputport

1	1	580	485	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	8	5	1	-1	0
END_LINE
startport

2	-1	530	465	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	2	2	-1	0
440	805
440	465
END_LINE
startport

2	-1	530	485	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	580	465	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	8	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
16	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
535	240	585	285
1	0
outputport

1	1	585	270	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	1	5	1	-1	0
END_LINE
startport

2	-1	535	250	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	0	2	-1	0
425	795
425	250
END_LINE
startport

2	-1	535	270	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	585	250	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
24	0	0	0
counter
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
510	560	610	734
1	0
parameter
Widget Tag	5	_	_	counter
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	510	650	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	510	665	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	510	680	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	510	695	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	510	710	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	510	725	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
450	875
450	725
END_LINE
outputport

1	1	610	650	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	610	665	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	610	680	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	610	695	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	510	570	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	510	585	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	510	600	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	510	615	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	510	630	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
320	230
320	630
END_LINE
finishport

3	1	610	570	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	610	585	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	610	600	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	610	615	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	610	630	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	610	640	5	0	1	mandatory= 0	0	1
click
clickCounter
create	6	EndOfFunc
BEGIN_LINE
0	86	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
33	0	0	0
decCnt
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
690	780	790	954
1	0
parameter
Widget Tag	5	_	_	decCnt
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	690	870	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	885	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	900	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	915	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	930	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	690	945	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	36	0	0	-1	0
630	875
630	945
END_LINE
outputport

1	1	790	870	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	790	885	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	790	900	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	790	915	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	690	790	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	805	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	820	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	835	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	690	850	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	36	0	2	-1	0
645	855
645	850
END_LINE
finishport

3	1	790	790	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	805	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	820	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	835	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	850	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	790	860	4	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
34	0	0	0
decOvf
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
700	1000	800	1174
1	0
parameter
Widget Tag	5	_	_	decOvf
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	700	1090	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	1105	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	1120	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	1135	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	1150	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	700	1165	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
640	1090
640	1165
END_LINE
outputport

1	1	800	1090	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	800	1105	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	800	1120	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	800	1135	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	700	1010	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	1025	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	1040	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	1055	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	700	1070	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	35	0	2	-1	0
655	1070
655	1070
END_LINE
finishport

3	1	800	1010	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	1025	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	1040	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	1055	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	1070	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	800	1080	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
35	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
530	1060	580	1105
1	0
outputport

1	1	580	1090	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	8	5	1	-1	0
END_LINE
startport

2	-1	530	1070	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	3	2	-1	0
525	830
525	1070
END_LINE
startport

2	-1	530	1090	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	580	1070	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	8	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
36	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
535	845	585	890
1	0
outputport

1	1	585	875	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	1	5	1	-1	0
END_LINE
startport

2	-1	535	855	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	1	2	-1	0
530	820
530	855
END_LINE
startport

2	-1	535	875	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	585	855	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
49	0	0	0
startCnt
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
955	535	1055	709
1	0
parameter
Widget Tag	5	_	_	startCnt
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	955	625	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	640	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	655	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	670	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	685	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	700	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	65	0	0	-1	0
930	625
930	700
END_LINE
outputport

1	1	1055	625	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	640	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	655	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	670	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	955	545	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	560	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	575	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	590	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	605	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	65	0	2	-1	0
910	605
910	605
END_LINE
finishport

3	1	1055	545	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	560	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	575	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	590	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	605	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	615	5	0	1	mandatory= 0	0	1
click
clickStart
create	6	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
65	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
855	595	905	640
1	0
outputport

1	1	905	625	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	49	5	1	-1	0
END_LINE
startport

2	-1	855	605	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	4	2	-1	0
655	780
655	605
END_LINE
startport

2	-1	855	625	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	905	605	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	49	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
80	0	0	0
resetCnt
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
940	1245	1040	1419
1	0
parameter
Widget Tag	5	_	_	resetCnt
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	940	1335	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	940	1350	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	940	1365	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	940	1380	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	940	1395	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	940	1410	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	81	0	0	-1	0
915	1335
915	1410
END_LINE
outputport

1	1	1040	1335	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1040	1350	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1040	1365	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1040	1380	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	940	1255	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	940	1270	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	940	1285	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	940	1300	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	940	1315	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	81	0	2	-1	0
895	1315
895	1315
END_LINE
finishport

3	1	1040	1255	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1040	1270	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1040	1285	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1040	1300	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1040	1315	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1040	1325	5	0	1	mandatory= 0	0	1
click
clickReset
create	6	EndOfFunc
BEGIN_LINE
0	3	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
81	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
840	1305	890	1350
1	0
outputport

1	1	890	1335	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	49	5	1	-1	0
END_LINE
startport

2	-1	840	1315	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	5	2	-1	0
430	845
430	1315
END_LINE
startport

2	-1	840	1335	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	890	1315	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	49	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
83	0	0	0
stopCnt
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
945	1475	1045	1649
1	0
parameter
Widget Tag	5	_	_	stopCnt
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	945	1565	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	945	1580	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	945	1595	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	945	1610	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	945	1625	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	945	1640	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	84	0	0	-1	0
920	1566
920	1640
END_LINE
outputport

1	1	1045	1565	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1045	1580	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1045	1595	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1045	1610	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	945	1485	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	945	1500	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	945	1515	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	945	1530	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	945	1545	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	84	0	2	-1	0
900	1546
900	1545
END_LINE
finishport

3	1	1045	1485	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1045	1500	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1045	1515	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1045	1530	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1045	1545	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1045	1555	5	0	1	mandatory= 0	0	1
click
clickStop
create	6	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
84	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
845	1536	895	1581
1	0
outputport

1	1	895	1566	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	49	5	1	-1	0
END_LINE
startport

2	-1	845	1546	4	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	3	6	2	-1	0
410	855
410	1546
END_LINE
startport

2	-1	845	1566	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	895	1546	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	49	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
85	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
30	850	130	895
1	0
parameter
Integer Value	1	-2147483648	2147483647	5
EndOfValues
EndOfLabels
integer constant

outputport

1	1	130	880	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
86	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
_
200	840	250	985
1	0
inputport

0	1	200	865	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	85	0	0	-1	0
180	880
180	865
END_LINE
inputport

0	1	200	900	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	107	0	0	-1	0
180	930
180	900
END_LINE
inputport

0	1	200	935	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	109	0	0	-1	0
170	985
170	935
END_LINE
inputport

0	1	200	970	3	0	1	mandatory= 0	0	1
i4

Run_Input4	1	EndOfFunc
BEGIN_LINE
1	132	0	0	-1	0
180	1070
180	970
END_LINE
outputport

1	1	250	870	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	EndOfFunc
BEGIN_LINE
0	3	1	1	-1	0
END_LINE
startport

2	1	200	850	0	0	1	mandatory= 0	0	1

clickInc
Run_Input1	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
805	255
805	795
185	795
185	850
END_LINE
startport

2	1	200	885	1	0	1	mandatory= 0	0	1

clickOvf
Run_Input2	0	EndOfFunc
BEGIN_LINE
1	8	5	2	-1	0
815	475
815	830
185	830
185	885
END_LINE
startport

2	1	200	920	2	0	1	mandatory= 0	0	1

clickCounter
Run_Input3	0	EndOfFunc
BEGIN_LINE
1	24	5	2	-1	0
625	640
625	865
185	865
185	920
END_LINE
startport

2	1	200	955	3	1	1	mandatory= 0	0	1


Run_Input4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	250	850	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	Run_Input4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
107	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
30	900	130	945
1	0
parameter
Integer Value	1	-2147483648	2147483647	10
EndOfValues
EndOfLabels
integer constant

outputport

1	1	130	930	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	86	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
109	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
35	955	135	1000
1	0
parameter
Integer Value	1	-2147483648	2147483647	15
EndOfValues
EndOfLabels
integer constant

outputport

1	1	135	985	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	86	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
132	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
45	1040	145	1085
1	0
parameter
Integer Value	1	-2147483648	2147483647	1000
EndOfValues
EndOfLabels
integer constant

outputport

1	1	145	1070	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	86	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
