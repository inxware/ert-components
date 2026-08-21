CanvasSizeXY
2000	2050

IconData
BEGIN_BLOCK

dtv_pvr_play2
_
1	0	0	0
_
_
Provides facilities to control PVR playback in terms of trick modes, play, pause and stop.
DtvPvrPlay2
_
555	180	655	348
1	0
inputport

0	3	555	310	0	0	1	0	1
input

setinput	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
445	320
445	310
END_LINE
inputport

0	1	555	320	1	0	1	0	1
pos

setpos	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	555	330	2	0	1	0	1
time

settime	1	EndOfFunc
BEGIN_LINE
1	21	0	0	-1	0
475	485
475	330
END_LINE
inputport

0	1	555	350	3	0	1	0	1
vol

vol	1	EndOfFunc
BEGIN_LINE
1	22	0	0	-1	0
490	530
490	350
END_LINE
inputport

0	1	555	370	4	0	1	0	1
index

playIndex	1	EndOfFunc
BEGIN_LINE
1	39	0	0	-1	0
505	585
505	370
END_LINE
outputport

1	3	655	305	0	0	1	0	1
input

get	1	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
outputport

1	1	655	315	1	0	1	0	1
pos

get	2	EndOfFunc
BEGIN_LINE
0	6	5	1	-1	0
END_LINE
outputport

1	1	655	325	2	0	1	0	1
time

get	3	EndOfFunc
BEGIN_LINE
0	7	5	1	-1	0
END_LINE
outputport

1	1	655	335	3	0	1	0	1
length

get	4	EndOfFunc
BEGIN_LINE
0	8	5	1	-1	0
END_LINE
outputport

1	1	655	345	4	0	1	0	1
speed

get	5	EndOfFunc
BEGIN_LINE
0	16	5	1	-1	0
END_LINE
outputport

1	1	655	370	5	0	1	0	1
index

get	6	EndOfFunc
BEGIN_LINE
0	17	5	1	-1	0
END_LINE
startport

2	1	555	190	12	0	1	0	1
set input

setinput	0	EndOfFunc
BEGIN_LINE
1	61	5	2	-1	0
430	720
430	190
END_LINE
startport

2	3	555	200	1	0	1	0	1
set pos

setpos	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	555	210	5	0	1	0	1
set time

settime	0	EndOfFunc
BEGIN_LINE
1	40	5	2	-1	0
195	230
195	195
410	195
410	210
END_LINE
startport

2	1	555	220	3	0	1	0	1
get

get	0	EndOfFunc
BEGIN_LINE
1	5	5	2	-1	0
520	85
520	220
END_LINE
startport

2	1	555	230	7	0	1	0	1
play

play	0	EndOfFunc
BEGIN_LINE
1	57	5	2	-1	0
215	395
215	230
END_LINE
startport

2	1	555	240	4	0	1	0	1
pause

pause	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	555	250	6	0	1	0	1
rev

reverse	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	555	260	2	0	1	0	1
faster

faster	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	555	270	8	0	1	0	1
slower

slower	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	555	280	9	0	1	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	59	5	2	-1	0
460	560
460	280
END_LINE
startport

2	1	555	300	10	0	1	0	1
next

next	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	555	340	11	0	1	0	1
setVol

vol	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	555	360	0	0	1	0	1
playIndex

playIndex	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	655	190	0	0	1	0	1


setinput	2	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
finishport

3	1	655	200	1	0	1	0	1


setpos	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	210	2	0	1	0	1


settime	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	220	3	0	1	0	1


get	7	EndOfFunc
BEGIN_LINE
0	18	4	3	-1	0
END_LINE
finishport

3	1	655	230	4	0	1	0	1


play	1	EndOfFunc
BEGIN_LINE
0	4	4	3	-1	0
END_LINE
finishport

3	1	655	240	5	0	1	0	1


pause	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	250	6	0	1	0	1


reverse	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	260	7	0	1	0	1


faster	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	270	8	0	1	0	1


slower	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	280	9	0	1	0	1


stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	290	10	0	1	0	1
end

end	1	EndOfFunc
BEGIN_LINE
0	18	2	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


end	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
3	0	0	0
_
_
This is a string constant.
Constant_String1
_
235	290	335	335
1	0
parameter
String Constant	3	_	_	../userdata/media/download/morningsong001.mp3
EndOfValues
EndOfLabels
string constant

outputport

1	3	335	320	0	0	1	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
4	0	0	0
_
_
This is a string GUI output.
gui_text_string2
_
790	15	890	189
1	0
parameter
Widget Tag	5	_	_	PlayFile
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	790	105	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	120	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	135	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	150	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	165	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	790	180	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	1	0	0	-1	0
705	305
705	180
END_LINE
outputport

1	1	890	105	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	120	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	135	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	150	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	790	25	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	40	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	55	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	70	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	85	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
745	220
745	85
END_LINE
finishport

3	1	890	25	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	40	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	55	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	70	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	85	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	95	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
5	0	0	0
_
_
This is an integer GUI output.
gui_Patch
_
245	5	345	170
1	0
parameter
Widget Tag	5	_	_	Get
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	245	95	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	245	110	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	245	125	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	245	140	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	245	155	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	345	95	0	0	1	0	1
x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	345	110	1	0	1	0	1
y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	345	125	2	0	1	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	345	140	3	0	1	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	245	15	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	245	30	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	245	45	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	245	60	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	245	75	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	345	15	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	345	30	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	345	45	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	345	60	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	345	75	4	0	1	0	1


update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	345	85	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
6	0	0	0
_
_
This is an integer GUI output.
gui_text_int2
_
790	185	890	359
1	0
parameter
Widget Tag	5	_	_	Position
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	790	275	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	290	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	305	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	320	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	335	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	350	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	1	1	0	-1	0
730	315
730	350
END_LINE
outputport

1	1	890	275	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	290	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	305	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	320	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	790	195	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	210	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	225	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	240	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	255	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
745	220
745	255
END_LINE
finishport

3	1	890	195	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	210	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	225	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	240	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	255	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	265	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
7	0	0	0
_
_
This is an integer GUI output.
gui_text_int2
_
790	360	890	534
1	0
parameter
Widget Tag	5	_	_	time
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	790	450	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	465	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	480	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	495	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	510	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	525	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	1	2	0	-1	0
730	325
730	525
END_LINE
outputport

1	1	890	450	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	465	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	480	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	495	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	790	370	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	385	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	400	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	415	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	430	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
745	220
745	430
END_LINE
finishport

3	1	890	370	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	385	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	400	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	415	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	430	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	440	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
8	0	0	0
_
_
This is an integer GUI output.
gui_text_int2
_
790	530	890	704
1	0
parameter
Widget Tag	5	_	_	length
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	790	620	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	635	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	650	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	665	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	680	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	790	695	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	1	3	0	-1	0
715	335
715	695
END_LINE
outputport

1	1	890	620	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	635	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	650	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	890	665	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	790	540	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	555	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	570	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	585	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	790	600	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
745	220
745	600
END_LINE
finishport

3	1	890	540	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	555	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	570	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	585	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	600	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	890	610	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
16	0	0	0
_
_
This is an integer GUI output.
gui_text_int2
_
925	190	1025	364
1	0
parameter
Widget Tag	5	_	_	speed
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	925	280	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	295	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	310	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	325	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	340	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	355	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	1	4	0	-1	0
775	345
775	315
865	315
865	355
END_LINE
outputport

1	1	1025	280	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1025	295	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1025	310	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1025	325	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	925	200	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	215	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	230	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	245	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	260	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
880	220
880	260
END_LINE
finishport

3	1	1025	200	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	215	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	230	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	245	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	260	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	270	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
17	0	0	0
_
_
This is an integer GUI output.
gui_text_int2
_
925	365	1025	539
1	0
parameter
Widget Tag	5	_	_	index
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	925	455	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	470	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	485	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	500	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	515	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	925	530	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	1	5	0	-1	0
865	370
865	530
END_LINE
outputport

1	1	1025	455	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1025	470	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1025	485	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1025	500	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	925	375	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	390	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	405	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	420	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	925	435	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
880	220
880	435
END_LINE
finishport

3	1	1025	375	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	390	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	405	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	420	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	435	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1025	445	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
18	0	0	0
_
_
This is an integer GUI output.
gui_text_int2
_
930	535	1030	709
1	0
parameter
Widget Tag	5	_	_	ENDED
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	930	625	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	930	640	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	930	655	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	930	670	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	930	685	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	930	700	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
0	1	3	0	-1	0
END_LINE
outputport

1	1	1030	625	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1030	640	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1030	655	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1030	670	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	930	545	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	930	560	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	930	575	2	0	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
1	1	10	2	-1	0
900	290
900	575
END_LINE
startport

2	-1	930	590	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	930	605	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	3	2	-1	0
END_LINE
finishport

3	1	1030	545	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1030	560	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1030	575	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1030	590	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1030	605	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1030	615	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
20	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
180	410	280	455
1	0
parameter
Integer Value	1	-2147483648	2147483647	50
EndOfValues
EndOfLabels
integer constant

outputport

1	1	280	440	0	0	1	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
21	0	0	0
1
_
This is an integer constant.
Constant_Int1
_
180	455	280	500
1	0
parameter
Integer Value	1	-2147483648	2147483647	75
EndOfValues
EndOfLabels
integer constant

outputport

1	1	280	485	0	0	1	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
22	0	0	0
20
_
This is an integer constant.
Constant_Int1
_
185	500	285	545
1	0
parameter
Integer Value	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	285	530	0	0	1	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
37	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
175	605	275	650
1	0
parameter
Integer Value	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	275	635	0	0	1	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
39	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
180	555	280	600
1	0
parameter
Integer Value	1	-2147483648	2147483647	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	280	585	0	0	1	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1	4	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
40	0	0	0
_
_
This is an integer GUI output.
gui_Patch
_
60	150	160	315
1	0
parameter
Widget Tag	5	_	_	Pause
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	60	240	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	60	255	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	60	270	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	60	285	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	60	300	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	160	240	0	0	1	0	1
x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	160	255	1	0	1	0	1
y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	160	270	2	0	1	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	160	285	3	0	1	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	60	160	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	60	175	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	60	190	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	60	205	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	60	220	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	160	160	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	160	175	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	160	190	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	160	205	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	160	220	4	0	1	0	1


update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	160	230	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
57	0	0	0
_
_
This is an integer GUI output.
gui_Patch
_
55	315	155	480
1	0
parameter
Widget Tag	5	_	_	Faster
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	55	405	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	55	420	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	55	435	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	55	450	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	55	465	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	155	405	0	0	1	0	1
x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	155	420	1	0	1	0	1
y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	155	435	2	0	1	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	155	450	3	0	1	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	55	325	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	55	340	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	55	355	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	55	370	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	55	385	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	155	325	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	155	340	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	155	355	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	155	370	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	155	385	4	0	1	0	1


update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	155	395	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	7	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
59	0	0	0
_
_
This is an integer GUI output.
gui_Patch
_
40	480	140	645
1	0
parameter
Widget Tag	5	_	_	Stop
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	40	570	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	40	585	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	40	600	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	40	615	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	40	630	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	140	570	0	0	1	0	1
x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	140	585	1	0	1	0	1
y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	140	600	2	0	1	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	140	615	3	0	1	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	40	490	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	40	505	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	40	520	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	40	535	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	40	550	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	140	490	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	140	505	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	140	520	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	140	535	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	140	550	4	0	1	0	1


update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	140	560	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	9	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
61	0	0	0
_
_
This is an integer GUI output.
gui_Patch
_
45	640	145	805
1	0
parameter
Widget Tag	5	_	_	PlayIndex
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	45	730	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	45	745	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	45	760	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	45	775	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	45	790	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	145	730	0	0	1	0	1
x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	145	745	1	0	1	0	1
y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	145	760	2	0	1	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	145	775	3	0	1	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	45	650	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	45	665	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	45	680	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	45	695	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	45	710	4	0	0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	145	650	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	145	665	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	145	680	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	145	695	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	145	710	4	0	1	0	1


update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	145	720	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	12	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
