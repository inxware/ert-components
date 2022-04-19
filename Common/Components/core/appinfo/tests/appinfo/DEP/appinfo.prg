CanvasSizeXY
2000	2000

IconData
BEGIN_BLOCK

gui_text_string2
_
3	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
585	65	685	239
1	0
parameter
Widget Tag	5	_	_	AppName
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	585	155	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	170	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	185	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	200	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	215	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	585	230	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	6	0	-1	0
525	245
525	230
END_LINE
outputport

1	1	685	155	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	170	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	185	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	200	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	585	75	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	90	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	105	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	120	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	135	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	0	2	-1	0
540	230
540	135
END_LINE
finishport

3	1	685	75	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	90	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	105	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	120	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	135	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	145	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
7	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
735	280	835	454
1	0
parameter
Widget Tag	5	_	_	date
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	735	370	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	385	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	400	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	415	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	430	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	735	445	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	2	0	-1	0
675	290
675	445
END_LINE
outputport

1	1	835	370	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	385	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	400	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	415	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	735	290	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	305	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	320	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	335	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	350	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	0	2	-1	0
690	230
690	350
END_LINE
finishport

3	1	835	290	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	305	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	320	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	335	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	350	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	360	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
11	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
585	495	685	669
1	0
parameter
Widget Tag	5	_	_	minEHS
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	585	585	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	600	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	615	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	630	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	645	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	585	660	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	3	0	-1	0
525	305
525	660
END_LINE
outputport

1	1	685	585	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	600	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	615	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	630	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	585	505	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	520	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	535	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	550	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	565	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	0	2	-1	0
540	230
540	565
END_LINE
finishport

3	1	685	505	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	520	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	535	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	550	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	565	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	575	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
20	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
725	480	825	654
1	0
parameter
Widget Tag	5	_	_	Dependencies
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	725	570	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	585	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	600	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	615	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	630	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	725	645	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	4	0	-1	0
705	320
705	645
END_LINE
outputport

1	1	825	570	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	825	585	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	825	600	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	825	615	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	725	490	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	505	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	520	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	535	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	550	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	0	2	-1	0
710	230
710	550
END_LINE
finishport

3	1	825	490	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	505	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	520	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	535	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	550	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	560	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
28	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
420	465	520	639
1	0
parameter
Widget Tag	5	_	_	ProcGroup
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	420	555	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	420	570	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	420	585	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	420	600	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	420	615	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	420	630	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	5	0	-1	0
395	335
395	630
END_LINE
outputport

1	1	520	555	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	520	570	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	520	585	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	520	600	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	420	475	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	420	490	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	420	505	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	420	520	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	420	535	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	0	2	-1	0
405	230
405	535
END_LINE
finishport

3	1	520	475	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	520	490	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	520	505	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	520	520	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	520	535	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	520	545	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
29	0	0	0
_
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
570	270	670	444
1	0
parameter
Widget Tag	5	_	_	buildNo
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	570	360	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	570	375	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	570	390	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	570	405	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	570	420	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	570	435	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	37	0	0	-1	0
1015	115
1015	25
510	25
510	435
END_LINE
outputport

1	1	670	360	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	670	375	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	670	390	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	670	405	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	570	280	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	570	295	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	570	310	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	570	325	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	570	340	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	37	0	2	-1	0
1000	95
1000	40
525	40
525	340
END_LINE
finishport

3	1	670	280	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	670	295	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	670	310	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	670	325	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	670	340	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	670	350	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
30	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
735	95	835	269
1	0
parameter
Widget Tag	5	_	_	widget6
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	735	185	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	200	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	215	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	230	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	245	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	735	260	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
675	187
675	260
END_LINE
outputport

1	1	835	185	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	200	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	215	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	230	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	735	105	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	120	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	135	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	150	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	165	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	35	2	2	-1	0
690	157
690	165
END_LINE
finishport

3	1	835	105	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	120	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	135	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	150	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	165	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	175	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

appinfo
_
31	0	0	0
_
_
Returns the app automatic version number, data stamp and given name,
AppInfo
_
250	220	350	419
1	0
parameter
Default App Name	3	_	_	_
EndOfValues
EndOfLabels
Name of app to identify. Blank gives currently loaded application info. Use "dvm://devman-default/" for the default devman app server or full url dvm:// to get version info.

inputport

0	3	250	245	0	0	1	mandatory= 0	0	1
name

getinfo	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	350	260	0	0	1	mandatory= 0	0	1
version

getinfo	2	EndOfFunc
BEGIN_LINE
0	30	5	1	-1	0
END_LINE
outputport

1	1	350	275	1	0	1	mandatory= 0	0	1
build no

getinfo	3	EndOfFunc
BEGIN_LINE
0	29	5	1	-1	0
END_LINE
outputport

1	3	350	290	2	0	1	mandatory= 0	0	1
date

getinfo	4	EndOfFunc
BEGIN_LINE
0	20	5	1	-1	0
END_LINE
outputport

1	3	350	305	3	0	1	mandatory= 0	0	1
min EHS

getinfo	5	EndOfFunc
BEGIN_LINE
0	11	5	1	-1	0
END_LINE
outputport

1	3	350	320	4	0	1	mandatory= 0	0	1
dependencies

getinfo	6	EndOfFunc
BEGIN_LINE
0	20	5	1	-1	0
END_LINE
outputport

1	3	350	335	5	0	1	mandatory= 0	0	1
processing groups

getinfo	7	EndOfFunc
BEGIN_LINE
0	28	5	1	-1	0
END_LINE
outputport

1	3	350	245	6	0	1	mandatory= 0	0	1
app name

getinfo	8	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
startport

2	-1	250	230	0	1	0	mandatory= 0	0	1
get info

getinfo	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	350	230	0	0	1	mandatory= 0	0	1


getinfo	9	EndOfFunc
BEGIN_LINE
0	30	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

appget
_
34	0	0	0
_
_
Provides the means for an application to download, install & Run other applications.
AppGet
_
1045	165	1145	364
1	0
parameter
Server	3	_	_	www.inx-systems.com/cgi-bin/
EndOfValues
EndOfLabels
base URL of devman server hosting applications

inputport

0	3	1045	185	0	0	1	mandatory= 0	0	1
Keywords

listremote	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1045	310	1	0	1	mandatory= 0	0	1
Server

listremote	2	getrun	2	getinstall	2	getinfo	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1045	295	2	0	1	mandatory= 0	0	1
app

getrun	1	getinstall	1	getinfo	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	1145	185	0	0	1	mandatory= 0	0	1
list

listremote	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	1145	295	1	0	1	mandatory= 0	0	1
errors

listremote	4	getrun	3	getinstall	3	getinfo	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	1145	270	2	0	1	mandatory= 0	0	1
info

getinfo	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1045	175	0	1	0	mandatory= 0	0	1
get list

listremote	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	1045	210	1	0	0	mandatory= 0	0	1
run

getrun	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	1045	235	2	0	0	mandatory= 0	0	1
install

getinstall	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	1045	260	3	0	0	mandatory= 0	0	1
app info

getinfo	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	1145	175	0	0	1	mandatory= 0	0	1
------

listremote	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	195	1	0	1	mandatory= 0	0	1
error

listremote	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	210	2	0	1	mandatory= 0	0	1
------

getrun	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	220	3	0	1	mandatory= 0	0	1
error

getrun	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	235	4	0	1	mandatory= 0	0	1
-----

getinstall	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	245	5	0	1	mandatory= 0	0	1
error

getinstall	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	260	6	0	1	mandatory= 0	0	1
-----

getinfo	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	280	7	0	1	mandatory= 0	0	1
error

getinfo	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

fs_dir_list
_
35	0	0	0
_
_
Allows a directory path to be set and outputs files contained in the directory on each get. Separate events are asserted for each type of file or directory.
FileSystemDirList1
_
410	120	510	209
1	0
parameter
Directory	3	_	_	_
EndOfValues
EndOfLabels
Media Directory

parameter
FindFiles	0	_	_	1
EndOfValues
EndOfLabels
Find Files

parameter
FindDirs	0	_	_	0
EndOfValues
EndOfLabels
Find Dirs

inputport

0	3	410	140	0	0	1	mandatory= 0	0	1
dir

set	1	EndOfFunc
BEGIN_LINE
1	36	0	0	-1	0
400	125
400	140
END_LINE
outputport

1	3	510	187	0	0	1	mandatory= 0	0	1
name

get	1	EndOfFunc
BEGIN_LINE
0	30	5	1	-1	0
END_LINE
startport

2	-1	410	130	0	1	1	mandatory= 0	0	1
set dir

set	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	410	157	1	0	1	mandatory= 0	0	1
get entry

get	0	EndOfFunc
BEGIN_LINE
1	30	5	2	-1	0
850	175
850	80
395	80
395	157
END_LINE
startport

2	3	410	198	2	0	1	mandatory= 0	0	1
reset

reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	510	130	0	0	1	mandatory= 0	0	1
Ready

set	2	EndOfFunc
BEGIN_LINE
0	37	0	3	-1	0
END_LINE
finishport

3	3	510	140	1	0	1	mandatory= 0	0	1
Error

set	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	510	157	2	0	1	mandatory= 0	0	1
file

get	2	EndOfFunc
BEGIN_LINE
0	30	4	3	-1	0
END_LINE
finishport

3	3	510	167	3	0	1	mandatory= 0	0	1
dir

get	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	510	177	4	0	1	mandatory= 0	0	1
no more

get	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	510	198	5	0	1	mandatory= 0	0	1
-------

reset	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
36	0	0	0
_
_
This is a string constant.
Constant_String1
_
265	95	365	140
1	0
parameter
String Constant	3	_	_	../userdata/stuff
EndOfValues
EndOfLabels
string constant

outputport

1	3	365	125	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	35	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
37	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
905	85	955	130
1	0
outputport

1	1	955	115	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	29	5	1	-1	0
END_LINE
startport

2	-1	905	95	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	35	0	2	-1	0
530	130
530	55
900	55
900	95
END_LINE
startport

2	-1	905	115	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	955	95	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	29	4	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
