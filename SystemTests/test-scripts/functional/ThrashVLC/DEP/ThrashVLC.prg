CanvasSizeXY
2510	2120

IconData
BEGIN_BLOCK

trigger_eventor2
_
4	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
660	380	680	420
1	0
startport

2	-1	660	390	0	0	1	0	1


Or1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	660	410	1	0	1	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	680	401	0	0	1	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	143	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

dtv_pvr_play2
_
45	0	0	0
_
_
Provides facilities to control PVR playback in terms of trick modes, play, pause and stop.
DtvPvrPlay2
_
905	395	1005	563
1	0
inputport

0	3	905	525	0	0	1	0	1
input

setinput	1	EndOfFunc
BEGIN_LINE
1	61	0	0	-1	0
895	370
895	525
END_LINE
inputport

0	1	905	535	1	0	1	0	1
pos

setpos	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	905	545	2	0	1	0	1
time

settime	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	905	565	3	0	1	0	1
vol

vol	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	905	585	4	0	1	0	1
index

playIndex	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	1005	520	0	0	1	0	1
input

get	1	EndOfFunc
BEGIN_LINE
0	191	5	1	-1	0
END_LINE
outputport

1	1	1005	530	1	0	1	0	1
pos

get	2	EndOfFunc
BEGIN_LINE
0	210	5	1	-1	0
END_LINE
outputport

1	1	1005	540	2	0	1	0	1
time
time
get	3	EndOfFunc
BEGIN_LINE
0	193	5	1	-1	0
END_LINE
outputport

1	1	1005	550	3	0	1	0	1
length

get	4	EndOfFunc
BEGIN_LINE
0	228	5	1	-1	0
END_LINE
outputport

1	1	1005	560	4	0	1	0	1
speed

get	5	EndOfFunc
BEGIN_LINE
0	247	5	1	-1	0
END_LINE
outputport

1	1	1005	585	5	0	1	0	1
index

get	6	EndOfFunc
BEGIN_LINE
0	267	5	1	-1	0
END_LINE
startport

2	-1	905	405	0	0	1	0	1
set input

setinput	0	EndOfFunc
BEGIN_LINE
1	289	0	2	-1	0
900	211
900	405
END_LINE
startport

2	3	905	415	1	0	1	0	1
set pos

setpos	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	425	2	0	1	0	1
set time

settime	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	435	3	0	1	0	1
get

get	0	EndOfFunc
BEGIN_LINE
1	192	0	2	-1	0
870	390
870	435
END_LINE
startport

2	1	905	445	9	0	1	0	1
play

play	0	EndOfFunc
BEGIN_LINE
1	45	9	2	-1	0
1050	495
1050	350
860	350
860	445
END_LINE
startport

2	1	905	455	5	0	1	0	1
pause

pause	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	465	6	0	1	0	1
rev

reverse	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	475	7	0	1	0	1
faster

faster	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	485	8	0	1	0	1
slower

slower	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	495	4	0	1	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	45	0	2	-1	0
1020	405
1020	440
890	440
890	495
END_LINE
startport

2	1	905	515	10	0	1	0	1
next

next	0	EndOfFunc
BEGIN_LINE
0	-1	10	2	-1	0
END_LINE
startport

2	1	905	555	11	0	1	0	1
setVol

vol	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	905	575	0	0	1	0	1
playIndex

playIndex	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	1005	405	0	0	1	0	1


setinput	2	EndOfFunc
BEGIN_LINE
0	45	4	3	-1	0
END_LINE
finishport

3	1	1005	415	1	0	1	0	1


setpos	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1005	425	2	0	1	0	1


settime	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1005	435	3	0	1	0	1

ePlayerInfo
get	7	EndOfFunc
BEGIN_LINE
0	267	4	3	-1	0
END_LINE
finishport

3	1	1005	445	4	0	1	0	1


play	1	EndOfFunc
BEGIN_LINE
0	142	0	3	-1	0
END_LINE
finishport

3	1	1005	455	5	0	1	0	1


pause	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1005	465	6	0	1	0	1


reverse	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1005	475	7	0	1	0	1


faster	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1005	485	8	0	1	0	1


slower	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1005	495	9	0	1	0	1


stop	1	EndOfFunc
BEGIN_LINE
0	45	4	3	-1	0
END_LINE
finishport

3	1	1005	505	10	0	1	0	1
end

end	1	EndOfFunc
BEGIN_LINE
0	289	1	3	-1	0
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
61	0	0	0
_
_
This is a string constant.
Constant_String1
_
465	340	565	385
1	0
parameter
String Constant	3	_	_	../userdata/media/ShortPlay.mp3
EndOfValues
EndOfLabels
string constant

outputport

1	3	565	370	0	0	1	0	1

remoteStream
Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	45	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
85	0	0	0
_
_
This is a string constant.
Constant_String1
_
540	680	640	725
1	0
parameter
String Constant	3	_	_	No Network: Playing Downloaded Tracks
EndOfValues
EndOfLabels
string constant

outputport

1	3	640	710	0	0	1	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	129	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
107	0	0	0
Streaming From Network
_
This is a string constant.
Constant_String1
_
540	595	640	640
1	0
parameter
String Constant	3	_	_	Streaming from Server
EndOfValues
EndOfLabels
string constant

outputport

1	3	640	625	0	0	1	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	129	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
129	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
_
760	565	810	640
1	0
inputport

0	3	760	590	0	0	1	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	107	0	0	-1	0
730	625
730	590
END_LINE
inputport

0	3	760	625	1	0	1	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	85	0	0	-1	0
720	710
720	625
END_LINE
outputport

1	3	810	595	0	0	1	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	63	5	1	-1	0
END_LINE
startport

2	3	760	575	0	0	1	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	4	0	2	-1	0
715	401
715	575
END_LINE
startport

2	3	760	610	1	0	1	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	810	575	0	0	1	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	65	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
139	0	0	0
0
_
This is an integer constant.
Constant_Int1
_
645	545	745	590
1	0
parameter
Integer Value	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	745	575	0	0	1	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	138	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
147	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
765	525	785	565
1	0
startport

2	-1	765	535	0	1	1	0	1


Or1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	765	555	1	0	1	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	187	0	2	-1	0
750	645
750	555
END_LINE
finishport

3	-1	785	546	0	0	1	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	138	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
187	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
_
430	635	448	675
1	0
inputport

0	0	430	665	0	0	1	0	1


Update	1	EndOfFunc
BEGIN_LINE
1	188	0	0	-1	0
420	655
420	665
END_LINE
startport

2	0	430	645	0	0	1	0	1


Update	0	EndOfFunc
BEGIN_LINE
0	-1	10	2	-1	0
END_LINE
finishport

3	0	448	645	0	0	1	0	1


Update	2	EndOfFunc
BEGIN_LINE
0	147	1	3	-1	0
END_LINE
finishport

3	0	448	665	1	0	1	0	1


Update	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2b
_
188	0	0	0
_
_
This is a 2-input boolean multiplexer.
MultiplexTwoInputBool
_
265	625	315	700
1	0
inputport

0	0	265	650	0	0	1	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	189	0	0	-1	0
255	645
255	650
END_LINE
inputport

0	0	265	685	1	0	1	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	190	0	0	-1	0
245	695
245	685
END_LINE
outputport

1	0	315	655	0	0	1	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	191	5	1	-1	0
END_LINE
startport

2	0	265	635	0	0	1	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	265	670	1	0	1	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	315	635	0	0	1	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	191	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
189	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
_
120	615	220	660
1	0
parameter
Boolean Constant	0	_	_	1
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	220	645	0	0	1	0	1


Run_ConstantBool	1	EndOfFunc
BEGIN_LINE
0	143	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
190	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
_
120	665	220	710
1	0
parameter
Boolean Constant	0	_	_	0
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	220	695	0	0	1	0	1


Run_ConstantBool	1	EndOfFunc
BEGIN_LINE
0	143	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


Run_ConstantBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
191	0	0	0
_
_
This is a string GUI output.
gui_text_string2
_
1090	460	1190	634
1	0
parameter
Widget Tag	5	_	_	TerackName
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1090	550	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1090	565	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1090	580	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1090	595	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1090	610	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1090	625	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	45	0	0	-1	0
1030	520
1030	625
END_LINE
outputport

1	1	1190	550	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1190	565	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1190	580	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1190	595	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1090	470	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1090	485	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1090	500	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1090	515	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1090	530	4	0	0	0	1
update
ePlayerInfo
update	0	EndOfFunc
BEGIN_LINE
1	45	3	2	-1	0
1045	435
1045	530
END_LINE
finishport

3	1	1190	470	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1190	485	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1190	500	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1190	515	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1190	530	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1190	540	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
192	0	0	0
_
_
Creates periodic timed events.
time_clock
_
765	350	825	440
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	100000
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

0	1	765	395	0	0	1	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	765	410	1	0	1	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	765	425	2	0	1	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	765	360	0	1	1	0	1
start

start	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	765	375	1	0	1	0	1
stop

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	825	390	0	0	1	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	45	3	3	-1	0
END_LINE
finishport

3	0	825	360	1	0	1	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	825	375	2	0	1	0	1


stop	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
193	0	0	0
time
_
This is an integer GUI output.
gui_text_int2
_
950	730	1050	904
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

0	1	950	820	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	950	835	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	950	850	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	950	865	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	950	880	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	950	895	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	45	2	0	-1	0
1065	540
1065	335
890	335
890	895
END_LINE
outputport

1	1	1050	820	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1050	835	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1050	850	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1050	865	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	950	740	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	950	755	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	950	770	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	950	785	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	950	800	4	0	0	0	1
update
ePlayerInfo
update	0	EndOfFunc
BEGIN_LINE
1	45	3	2	-1	0
1050	435
1050	350
905	350
905	800
END_LINE
finishport

3	1	1050	740	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1050	755	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1050	770	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1050	785	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1050	800	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1050	810	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
210	0	0	0
position
_
This is an integer GUI output.
gui_text_int2
_
1075	655	1175	829
1	0
parameter
Widget Tag	5	_	_	position
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1075	745	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	760	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	775	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	790	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	805	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	820	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	45	1	0	-1	0
1025	530
1025	820
END_LINE
outputport

1	1	1175	745	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1175	760	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1175	775	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1175	790	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1075	665	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	680	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	695	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	710	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	725	4	0	0	0	1
update
ePlayerInfo
update	0	EndOfFunc
BEGIN_LINE
1	45	3	2	-1	0
1030	435
1030	725
END_LINE
finishport

3	1	1175	665	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	680	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	695	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	710	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	725	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	735	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
228	0	0	0
length
_
This is an integer GUI output.
gui_text_int2
_
1190	660	1290	834
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

0	1	1190	750	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1190	765	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1190	780	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1190	795	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1190	810	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1190	825	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	45	3	0	-1	0
1130	550
1130	825
END_LINE
outputport

1	1	1290	750	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1290	765	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1290	780	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1290	795	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1190	670	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1190	685	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1190	700	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1190	715	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1190	730	4	0	0	0	1
update
ePlayerInfo
update	0	EndOfFunc
BEGIN_LINE
1	45	3	2	-1	0
1145	435
1145	730
END_LINE
finishport

3	1	1290	670	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	685	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	700	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	715	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	730	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1290	740	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
247	0	0	0
speed
_
This is an integer GUI output.
gui_text_int2
_
1075	860	1175	1034
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

0	1	1075	950	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	965	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	980	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	995	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	1010	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1075	1025	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	45	4	0	-1	0
1060	560
1060	1025
END_LINE
outputport

1	1	1175	950	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1175	965	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1175	980	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1175	995	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1075	870	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	885	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	900	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	915	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1075	930	4	0	0	0	1
update
ePlayerInfo
update	0	EndOfFunc
BEGIN_LINE
1	45	3	2	-1	0
1030	435
1030	930
END_LINE
finishport

3	1	1175	870	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	885	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	900	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	915	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	930	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1175	940	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
267	0	0	0
index
_
This is an integer GUI output.
gui_text_int2
_
1195	870	1295	1044
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

0	1	1195	960	0	0	1	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1195	975	1	0	1	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1195	990	2	0	1	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1195	1005	3	0	1	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1195	1020	4	0	1	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1195	1035	5	0	1	0	1


update	6	EndOfFunc
BEGIN_LINE
1	45	5	0	-1	0
1135	585
1135	1035
END_LINE
outputport

1	1	1295	960	0	0	1	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1295	975	1	0	1	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1295	990	2	0	1	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1295	1005	3	0	1	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1195	880	0	1	0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1195	895	1	0	0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1195	910	2	1	0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1195	925	3	0	0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1195	940	4	0	0	0	1
update
ePlayerInfo
update	0	EndOfFunc
BEGIN_LINE
1	45	3	2	-1	0
1150	435
1150	940
END_LINE
finishport

3	1	1295	880	0	0	1	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1295	895	1	0	1	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1295	910	2	0	1	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1295	925	3	0	1	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1295	940	4	0	1	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1295	950	5	0	1	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
288	0	0	0
new file
_
Creates periodic timed events.
time_clock
_
690	205	750	295
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

0	1	690	250	0	0	1	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	690	265	1	0	1	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	690	280	2	0	1	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	690	215	0	1	1	0	1
start

start	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	690	230	1	0	1	0	1
stop

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	750	245	0	0	1	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	289	0	3	-1	0
END_LINE
finishport

3	0	750	215	1	0	1	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	750	230	2	0	1	0	1


stop	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	0	1


tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
289	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
805	190	825	230
1	0
startport

2	-1	805	200	0	0	1	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	288	0	2	-1	0
800	245
800	200
END_LINE
startport

2	-1	805	220	1	0	1	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	10	2	-1	0
END_LINE
finishport

3	0	825	211	0	0	1	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	45	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
