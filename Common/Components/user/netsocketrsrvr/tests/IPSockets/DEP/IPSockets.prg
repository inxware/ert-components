#V:2.0.0

CanvasSizeXY
2000	1194

IconData
BEGIN_BLOCK

inx-netsocket
_
2	0	0	0
_
_
provides tcpip and udp socket data comms.
netSocket
0
_
225	100	334	289
1	0
parameter
URL	3	0	256	127.0.0.1
EndOfValues
EndOfLabels
URL of the server socket. N/A for sockets

parameter
port	1	0	65535	8021
EndOfValues
EndOfLabels
IP socket number to open

parameter
tcp/udp	1	0	1	0
EndOfValues
EndOfLabels
0=tcp, 1=udp

inputport

0	1	219	130	0	0	1	mandatory= 0	0	1
port

open	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	219	120	1	0	1	mandatory= 0	0	1
url

open	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	219	230	2	0	1	mandatory= 0	0	1
data

send	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	1
189	360
189	230
END_LINE
inputport

0	1	219	240	3	0	1	mandatory= 0	0	1
size

send	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	1
END_LINE
outputport

1	1	338	240	0	0	1	mandatory= 0	0	1
errno

open	3	close	1	send	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	338	190	1	0	1	mandatory= 0	0	1
data

open	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	338	200	2	0	1	mandatory= 0	0	1
size

open	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	219	110	0	0	1	mandatory= 0	0	1
open

open	0	EndOfFunc
BEGIN_LINE
1	10	5	2	-1	1
955	435
955	95
209	95
209	110
END_LINE
startport

2	1	219	150	2	0	1	mandatory= 0	0	1
close

close	0	EndOfFunc
BEGIN_LINE
1	21	0	2	-1	0
440	326
440	85
204	85
204	150
END_LINE
startport

2	1	219	220	2	0	1	mandatory= 0	0	1
send

send	0	EndOfFunc
BEGIN_LINE
1	2	2	2	-1	1
343	110
343	205
209	205
209	220
END_LINE
finishport

3	1	338	160	0	0	1	mandatory= 0	0	1
error

close	2	open	6	EndOfFunc
BEGIN_LINE
0	11	3	3	-1	0
END_LINE
finishport

3	1	338	230	1	0	1	mandatory= 0	0	1
error

send	4	open	7	EndOfFunc
BEGIN_LINE
0	21	1	3	-1	0
END_LINE
finishport

3	1	338	110	2	0	1	mandatory= 0	0	1
----

open	8	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
finishport

3	1	338	120	3	0	1	mandatory= 0	0	1
error

open	9	EndOfFunc
BEGIN_LINE
0	11	1	3	-1	0
END_LINE
finishport

3	3	338	180	4	0	1	mandatory= 0	0	1
received

open	10	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
END_LINE
finishport

3	1	338	150	5	0	1	mandatory= 0	0	1
---

open	11	close	3	EndOfFunc
BEGIN_LINE
0	11	2	3	-1	0
END_LINE
finishport

3	1	338	220	6	0	1	mandatory= 0	0	1
---

send	5	open	12	EndOfFunc
BEGIN_LINE
0	21	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
3	0	0	0

_
This is a string constant.
Constant_String1
0
_
60	330	160	375
1	0
parameter
String Constant	3	_	_	Hello TCP World 
EndOfValues
EndOfLabels
string constant

outputport

1	3	160	360	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	2	1	-1	0
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
4	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
845	165	945	339
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

0	1	845	270	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	280	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	290	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	300	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	315	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	330	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	158	0	0	-1	1
785	420
785	330
END_LINE
outputport

1	1	945	270	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	945	280	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	945	290	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	945	300	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	845	175	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	185	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	200	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	210	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	255	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	158	0	2	-1	1
800	400
800	255
END_LINE
finishport

3	1	945	175	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	185	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	200	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	210	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	255	5	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	225	4	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	945	235	6	0	1	mandatory= 0	0	1
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
850	375	950	549
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

0	1	850	480	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	850	490	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	850	500	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	850	510	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	850	525	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	850	540	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	64	2	0	-1	1
790	295
790	540
END_LINE
outputport

1	1	950	480	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	950	490	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	950	500	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	950	510	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	850	385	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	850	395	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	850	410	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	850	420	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	850	465	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	64	4	2	-1	1
805	265
805	465
END_LINE
finishport

3	1	950	385	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	950	395	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	950	410	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	950	420	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	950	465	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	950	435	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
finishport

3	1	950	445	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
11	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
460	45	480	111
1	0
startport

2	-1	460	55	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	2	2	2	-1	1
450	110
450	55
END_LINE
startport

2	-1	460	70	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	2	3	2	-1	1
445	120
445	70
END_LINE
startport

2	-1	460	85	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
1	2	5	2	-1	1
435	150
435	85
END_LINE
startport

2	-1	460	100	3	0	1	mandatory= 0	0	1


Or4	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	1
425	160
425	100
END_LINE
finishport

3	-1	480	79	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	Or4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
19	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
810	35	830	101
1	0
startport

2	-1	810	45	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	64	0	2	-1	1
800	195
800	45
END_LINE
startport

2	-1	810	60	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	64	1	2	-1	1
795	205
795	60
END_LINE
startport

2	-1	810	75	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
1	64	5	2	-1	1
785	235
785	75
END_LINE
startport

2	-1	810	90	3	0	1	mandatory= 0	0	1


Or4	0	EndOfFunc
BEGIN_LINE
1	64	6	2	-1	1
775	245
775	90
END_LINE
finishport

3	-1	830	69	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	Or4	1	EndOfFunc
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
0
_
50	250	150	295
1	0
parameter
Integer Value	1	-2147483648	2147483647	3
EndOfValues
EndOfLabels
integer constant

outputport

1	1	150	280	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	49	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
21	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
405	305	425	345
1	0
startport

2	-1	405	315	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	2	6	2	-1	1
395	220
395	315
END_LINE
startport

2	-1	405	335	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	2	1	2	-1	1
390	230
390	335
END_LINE
finishport

3	-1	425	326	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
22	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
485	260	505	326
1	0
startport

2	-1	485	270	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	2	4	2	-1	1
475	180
475	270
END_LINE
startport

2	-1	485	285	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	485	300	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	485	315	3	0	1	mandatory= 0	0	1


Or4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	505	294	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	Or4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
34	0	0	0

_
This is a string constant.
Constant_String1
0
_
410	365	510	410
1	0
parameter
String Constant	3	_	_	Back at ya! 
EndOfValues
EndOfLabels
string constant

outputport

1	3	510	395	0	0	1	mandatory= 0	0	1


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

trigger_eventor2
_
35	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
730	325	750	365
1	0
startport

2	-1	730	335	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	64	2	2	-1	1
720	315
720	335
END_LINE
startport

2	-1	730	355	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	64	3	2	-1	1
705	325
705	355
END_LINE
finishport

3	-1	750	346	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

inx-netsocket
_
49	0	0	0
_
_
provides tcpip and udp socket data comms.
netSocket
0
_
245	690	354	879
1	0
parameter
URL	3	0	256	127.0.0.1
EndOfValues
EndOfLabels
URL of the server socket. N/A for sockets

parameter
port	1	0	65535	8020
EndOfValues
EndOfLabels
IP socket number to open

parameter
tcp/udp	1	0	1	1
EndOfValues
EndOfLabels
0=tcp, 1=udp

inputport

0	1	239	720	0	0	1	mandatory= 0	0	1
port

open	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	239	710	1	0	1	mandatory= 0	0	1
url

open	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	239	820	2	0	1	mandatory= 0	0	1
data

send	1	EndOfFunc
BEGIN_LINE
1	50	0	0	-1	1
209	905
209	820
END_LINE
inputport

0	1	239	830	3	0	1	mandatory= 0	0	1
size

send	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	1
END_LINE
outputport

1	1	358	830	0	0	1	mandatory= 0	0	1
errno

open	3	close	1	send	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	358	780	1	0	1	mandatory= 0	0	1
data

open	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	358	790	2	0	1	mandatory= 0	0	1
size

open	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	239	700	0	0	1	mandatory= 0	0	1
open

open	0	EndOfFunc
BEGIN_LINE
1	52	5	2	-1	1
1030	980
1030	685
229	685
229	700
END_LINE
startport

2	1	239	740	2	0	1	mandatory= 0	0	1
close

close	0	EndOfFunc
BEGIN_LINE
1	56	0	2	-1	1
460	871
460	675
224	675
224	740
END_LINE
startport

2	1	239	810	2	0	1	mandatory= 0	0	1
send

send	0	EndOfFunc
BEGIN_LINE
1	49	2	2	-1	1
383	700
383	665
214	665
214	810
END_LINE
finishport

3	1	358	750	0	0	1	mandatory= 0	0	1
error

close	2	open	6	EndOfFunc
BEGIN_LINE
0	11	3	3	-1	0
END_LINE
finishport

3	1	358	820	1	0	1	mandatory= 0	0	1
error

send	4	open	7	EndOfFunc
BEGIN_LINE
0	21	1	3	-1	0
END_LINE
finishport

3	1	358	700	2	0	1	mandatory= 0	0	1
----

open	8	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
finishport

3	1	358	710	3	0	1	mandatory= 0	0	1
error

open	9	EndOfFunc
BEGIN_LINE
0	11	1	3	-1	0
END_LINE
finishport

3	3	358	770	4	0	1	mandatory= 0	0	1
received

open	10	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
END_LINE
finishport

3	1	358	740	5	0	1	mandatory= 0	0	1
---

open	11	close	3	EndOfFunc
BEGIN_LINE
0	11	2	3	-1	0
END_LINE
finishport

3	1	358	810	6	0	1	mandatory= 0	0	1
---

send	5	open	12	EndOfFunc
BEGIN_LINE
0	21	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
50	0	0	0

_
This is a string constant.
Constant_String1
0
_
80	875	180	920
1	0
parameter
String Constant	3	_	_	Hello UDP World 
EndOfValues
EndOfLabels
string constant

outputport

1	3	180	905	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	2	1	-1	0
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
51	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
865	710	965	884
1	0
parameter
Widget Tag	5	_	_	widget3
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

0	1	865	815	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	865	825	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	865	835	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	865	845	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	865	860	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	865	875	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	126	0	0	-1	0
805	805
805	875
END_LINE
outputport

1	1	965	815	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	965	825	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	965	835	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	965	845	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	865	720	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	865	730	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	865	745	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	865	755	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	865	800	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	126	0	2	-1	0
END_LINE
finishport

3	1	965	720	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	965	730	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	965	745	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	965	755	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	965	800	5	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	63	1	3	-1	0
END_LINE
finishport

3	1	965	770	4	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	965	780	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
52	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
870	920	970	1094
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

0	1	870	1025	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	870	1035	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	870	1045	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	870	1055	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	870	1070	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	870	1085	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	63	2	0	-1	1
810	795
810	1085
END_LINE
outputport

1	1	970	1025	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	970	1035	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	970	1045	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	970	1055	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	870	930	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	870	940	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	870	955	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	870	965	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	870	1010	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	63	4	2	-1	1
825	765
825	1010
END_LINE
finishport

3	1	970	930	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	970	940	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	970	955	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	970	965	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	970	1010	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	970	980	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
finishport

3	1	970	990	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
53	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
480	590	500	656
1	0
startport

2	-1	480	600	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	49	2	2	-1	1
470	700
470	600
END_LINE
startport

2	-1	480	615	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	49	3	2	-1	1
465	710
465	615
END_LINE
startport

2	-1	480	630	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
1	49	5	2	-1	1
455	740
455	630
END_LINE
startport

2	-1	480	645	3	0	1	mandatory= 0	0	1


Or4	0	EndOfFunc
BEGIN_LINE
1	49	0	2	-1	1
445	750
445	645
END_LINE
finishport

3	-1	500	624	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	Or4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
54	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
830	580	850	646
1	0
startport

2	-1	830	590	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	63	0	2	-1	1
820	695
820	590
END_LINE
startport

2	-1	830	605	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	63	1	2	-1	1
815	705
815	605
END_LINE
startport

2	-1	830	620	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
1	63	5	2	-1	1
805	735
805	620
END_LINE
startport

2	-1	830	635	3	0	1	mandatory= 0	0	1


Or4	0	EndOfFunc
BEGIN_LINE
1	63	6	2	-1	1
795	745
795	635
END_LINE
finishport

3	-1	850	614	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	Or4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
55	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
70	795	170	840
1	0
parameter
Integer Value	1	-2147483648	2147483647	16
EndOfValues
EndOfLabels
integer constant

outputport

1	1	170	825	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	2	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
56	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
425	850	445	890
1	0
startport

2	-1	425	860	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	49	6	2	-1	1
415	810
415	860
END_LINE
startport

2	-1	425	880	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	49	1	2	-1	1
410	820
410	880
END_LINE
finishport

3	-1	445	871	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
57	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
505	805	525	871
1	0
startport

2	-1	505	815	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	49	4	2	-1	1
500	770
500	815
END_LINE
startport

2	-1	505	830	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	505	845	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	505	860	3	0	1	mandatory= 0	0	1


Or4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	525	839	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	Or4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
58	0	0	0

_
This is a string constant.
Constant_String1
0
_
445	935	545	980
1	0
parameter
String Constant	3	_	_	Back at ya! 
EndOfValues
EndOfLabels
string constant

outputport

1	3	545	965	0	0	1	mandatory= 0	0	1


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

trigger_eventor2
_
59	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
730	855	750	895
1	0
startport

2	-1	730	865	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	63	2	2	-1	1
720	815
720	865
END_LINE
startport

2	-1	730	885	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	63	3	2	-1	1
710	825
710	885
END_LINE
finishport

3	-1	750	876	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

netsocketrsrvr
_
60	0	0	0
_
_
Listens on specified port
netsocketsrvr
0
_
1240	355	1349	534
1	0
parameter
Interface	3	0	0	0
EndOfValues
EndOfLabels
Interface (IF)

parameter
Port	1	0	100000	8000
EndOfValues
EndOfLabels
Listening Port

parameter
udp	1	0	1	0
EndOfValues
EndOfLabels
tcp/udp mode

inputport

0	3	1234	375	0	0	1	mandatory= 0	0	1
IF

open	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1234	385	1	0	1	mandatory= 0	0	1
port

open	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1234	495	2	0	1	mandatory= 0	0	1
data

sendData	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1234	505	3	0	1	mandatory= 0	0	1
inSize

sendData	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	1353	445	0	0	1	mandatory= 0	0	1
data

open	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1353	455	1	0	1	mandatory= 0	0	1
size

open	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	1353	465	2	0	1	mandatory= 0	0	1
source

open	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	1234	365	0	0	1	mandatory= 0	0	1
open

open	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	1234	405	1	0	1	mandatory= 0	0	1
close

close	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	1234	485	2	0	1	mandatory= 0	0	1
send

sendData	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1353	365	0	0	1	mandatory= 0	0	1
---

open	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1353	375	1	0	1	mandatory= 0	0	1
error

open	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1353	405	2	0	1	mandatory= 0	0	1
---

close	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1353	415	3	0	1	mandatory= 0	0	1
error

close	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1353	485	4	0	1	mandatory= 0	0	1
---

sendData	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1353	495	5	0	1	mandatory= 0	0	1
error

sendData	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1353	435	6	0	1	mandatory= 0	0	1
received

open	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

netsocketrsrvr
_
63	0	0	0
_
_
Listens on specified port
netsocketsrvr
0
_
585	685	694	864
1	0
parameter
Interface	3	0	0	0
EndOfValues
EndOfLabels
Interface (IF)

parameter
Port	1	0	100000	8020
EndOfValues
EndOfLabels
Listening Port

parameter
udp	1	0	1	1
EndOfValues
EndOfLabels
tcp/udp mode

inputport

0	3	579	705	0	0	1	mandatory= 0	0	1
IF

open	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	579	715	1	0	1	mandatory= 0	0	1
port

open	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	579	825	2	0	1	mandatory= 0	0	1
data

sendData	1	EndOfFunc
BEGIN_LINE
1	58	0	0	-1	0
569	965
569	825
END_LINE
inputport

0	1	579	835	3	0	1	mandatory= 0	0	1
inSize

sendData	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	698	775	0	0	1	mandatory= 0	0	1
data

open	3	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
outputport

1	1	698	785	1	0	1	mandatory= 0	0	1
size

open	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	698	795	2	0	1	mandatory= 0	0	1
source

open	5	EndOfFunc
BEGIN_LINE
0	10	5	1	-1	0
END_LINE
startport

2	1	579	695	0	0	1	mandatory= 0	0	1
open

open	0	EndOfFunc
BEGIN_LINE
1	51	5	2	-1	1
990	770
990	680
569	680
569	695
END_LINE
startport

2	1	579	735	1	0	1	mandatory= 0	0	1
close

close	0	EndOfFunc
BEGIN_LINE
1	95	5	2	-1	1
1170	760
1170	670
564	670
564	735
END_LINE
startport

2	1	579	815	2	0	1	mandatory= 0	0	1
send

sendData	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	698	695	0	0	1	mandatory= 0	0	1
---

open	6	EndOfFunc
BEGIN_LINE
0	19	0	3	-1	0
END_LINE
finishport

3	1	698	705	1	0	1	mandatory= 0	0	1
error

open	7	EndOfFunc
BEGIN_LINE
0	19	1	3	-1	0
END_LINE
finishport

3	1	698	815	4	0	1	mandatory= 0	0	1
---

sendData	3	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
finishport

3	1	698	825	5	0	1	mandatory= 0	0	1
error

sendData	4	EndOfFunc
BEGIN_LINE
0	59	1	3	-1	0
END_LINE
finishport

3	1	698	765	6	0	1	mandatory= 0	0	1
received

open	8	EndOfFunc
BEGIN_LINE
0	10	4	3	-1	0
END_LINE
finishport

3	1	698	735	2	0	1	mandatory= 0	0	1
---

open	9	close	1	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
finishport

3	1	698	745	3	0	1	mandatory= 0	0	1
error

open	10	close	2	EndOfFunc
BEGIN_LINE
0	19	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

netsocketrsrvr
_
64	0	0	0
_
_
Listens on specified port
netsocketsrvr
0
_
575	185	684	364
1	0
parameter
Interface	3	0	0	0
EndOfValues
EndOfLabels
Interface (IF)

parameter
Port	1	0	100000	8021
EndOfValues
EndOfLabels
Listening Port

parameter
udp	1	0	1	0
EndOfValues
EndOfLabels
tcp/udp mode

inputport

0	3	569	205	0	0	1	mandatory= 0	0	1
IF

open	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	569	215	1	0	1	mandatory= 0	0	1
port

open	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	569	325	2	0	1	mandatory= 0	0	1
data

sendData	1	EndOfFunc
BEGIN_LINE
1	34	0	0	-1	0
559	395
559	325
END_LINE
inputport

0	1	569	335	3	0	1	mandatory= 0	0	1
inSize

sendData	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	688	275	0	0	1	mandatory= 0	0	1
data

open	3	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
outputport

1	1	688	285	1	0	1	mandatory= 0	0	1
size

open	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	688	295	2	0	1	mandatory= 0	0	1
source

open	5	EndOfFunc
BEGIN_LINE
0	10	5	1	-1	0
END_LINE
startport

2	1	569	195	0	0	1	mandatory= 0	0	1
open

open	0	EndOfFunc
BEGIN_LINE
1	4	5	2	-1	1
950	225
950	180
559	180
559	195
END_LINE
startport

2	1	569	235	1	0	1	mandatory= 0	0	1
close

close	0	EndOfFunc
BEGIN_LINE
1	66	5	2	-1	0
1130	290
1130	170
554	170
554	235
END_LINE
startport

2	1	569	315	1	0	1	mandatory= 0	0	1
send

sendData	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	1
END_LINE
finishport

3	1	688	195	0	0	1	mandatory= 0	0	1
---

open	6	EndOfFunc
BEGIN_LINE
0	19	0	3	-1	0
END_LINE
finishport

3	1	688	205	1	0	1	mandatory= 0	0	1
error

open	7	EndOfFunc
BEGIN_LINE
0	19	1	3	-1	0
END_LINE
finishport

3	1	688	315	4	0	1	mandatory= 0	0	1
---

sendData	3	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
finishport

3	1	688	325	5	0	1	mandatory= 0	0	1
error

sendData	4	EndOfFunc
BEGIN_LINE
0	35	1	3	-1	0
END_LINE
finishport

3	1	688	265	6	0	1	mandatory= 0	0	1
received

open	8	EndOfFunc
BEGIN_LINE
0	10	4	3	-1	0
END_LINE
finishport

3	1	688	235	2	0	1	mandatory= 0	0	1
---

open	9	close	1	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
finishport

3	1	688	245	3	0	1	mandatory= 0	0	1
error

open	10	close	2	EndOfFunc
BEGIN_LINE
0	19	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
65	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
105	70	165	160
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	1000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	105	115	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	105	130	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	105	145	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	105	80	0	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	1
END_LINE
startport

2	0	105	95	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	165	110	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
finishport

3	0	165	80	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	165	95	2	0	1	mandatory= 0	0	1


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

gui_patch
_
66	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
0
_
1015	230	1115	395
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

0	1	1015	335	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1015	345	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1015	355	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1015	365	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1015	380	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1115	335	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1115	345	1	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1115	355	2	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1115	365	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1015	240	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1015	250	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1015	265	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1015	275	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1015	320	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1115	240	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1115	250	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1115	265	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1115	275	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1115	320	4	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1115	290	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	158	1	3	-1	0
END_LINE
finishport

3	1	1115	300	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
95	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
0
_
1055	700	1155	865
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

0	1	1055	805	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1055	815	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1055	825	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1055	835	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1055	850	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1155	805	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1155	815	1	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1155	825	2	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1155	835	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1055	710	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1055	720	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1055	735	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1055	745	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1055	790	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1155	710	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1155	720	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1155	735	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1155	745	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1155	790	4	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1155	760	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	126	1	3	-1	0
END_LINE
finishport

3	1	1155	770	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
125	0	0	0

_
This is a string constant.
Constant_String1
0
_
80	540	180	585
1	0
parameter
String Constant	3	_	_	---
EndOfValues
EndOfLabels
string constant

outputport

1	3	180	570	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	158	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
126	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
775	775	825	850
1	0
inputport

0	3	775	800	5	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	63	0	0	-1	1
715	775
715	800
END_LINE
inputport

0	3	775	835	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	125	0	0	-1	0
755	570
755	835
END_LINE
outputport

1	3	825	805	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	51	5	1	-1	0
END_LINE
startport

2	-1	775	785	4	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	63	4	2	-1	1
708	765
708	785
END_LINE
startport

2	3	775	820	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	95	5	2	-1	0
1170	760
1170	685
760	685
760	820
END_LINE
finishport

3	3	825	785	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
158	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
710	390	760	465
1	0
inputport

0	3	710	415	5	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	64	0	0	-1	1
698	275
698	415
END_LINE
inputport

0	3	710	450	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	125	0	0	-1	0
690	570
690	450
END_LINE
outputport

1	3	760	420	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
startport

2	-1	710	400	4	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	64	4	2	-1	1
698	265
698	400
END_LINE
startport

2	3	710	435	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	66	5	2	-1	0
1130	290
1130	215
695	215
695	435
END_LINE
finishport

3	3	760	400	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	4	4	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
