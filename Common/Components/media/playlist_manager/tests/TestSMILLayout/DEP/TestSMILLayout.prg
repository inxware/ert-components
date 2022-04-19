#V:2.0.0

CanvasSizeXY
2050	1000

IconData
BEGIN_BLOCK

playlist_manager
_
1	0	0	0
_
_
Manages parsing SMIL-based playlists and controlling media players and graphics.
PlayManager
0
_
290	250	420	549
1	0
parameter
Start Behaviour	1	0	2	0
EndOfValues
EndOfLabels
0:CLEAR_ALL, 1:KEEP_EARLIERTHANNEW, 2:KEEP_ALL

parameter
Wait for downloads before start	0	_	_	0
EndOfValues
EndOfLabels
set to true if play events should only active after all media has been downloaded.

parameter
Download Grace Period	1	-1	36000	-1
EndOfValues
EndOfLabels
number of seconds waited after a new play list before Play events can occur

inputport

0	3	290	270	0	0	1	mandatory= 0	0	1
Playlist path

Next_Playlist	1	EndOfFunc
BEGIN_LINE
1	23	0	0	-1	0
250	275
250	270
END_LINE
inputport

0	3	290	445	1	0	1	mandatory= 0	0	1
URL

Got	1	EndOfFunc
BEGIN_LINE
1	1	2	0	-1	0
440	405
440	230
270	230
270	445
END_LINE
inputport

0	3	290	455	2	0	1	mandatory= 0	0	1
Local File

Got	2	EndOfFunc
BEGIN_LINE
1	1	5	0	-1	0
450	435
450	220
260	220
260	455
END_LINE
inputport

0	3	290	490	3	0	1	mandatory= 0	0	1
DL URL

Set_Paths	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	500	4	0	1	mandatory= 0	0	1
DL local

Set_Paths	2	EndOfFunc
BEGIN_LINE
1	69	0	0	-1	0
240	490
240	500
END_LINE
inputport

0	1	290	510	5	0	1	mandatory= 0	0	1
start mode

Set_Paths	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	290	305	6	0	1	mandatory= 0	0	1
ID

Started	1	EndOfFunc
BEGIN_LINE
1	1	12	0	-1	0
490	325
490	180
220	180
220	305
END_LINE
inputport

0	1	290	340	7	0	1	mandatory= 0	0	1
ID

Played	1	EndOfFunc
BEGIN_LINE
1	1	12	0	-1	0
500	325
500	170
210	170
210	340
END_LINE
inputport

0	1	290	435	8	0	1	mandatory= 0	0	1
ID

Got	3	EndOfFunc
BEGIN_LINE
1	1	13	0	-1	1
510	415
510	160
200	160
200	435
END_LINE
outputport

1	3	420	335	0	0	1	mandatory= 0	0	1
Media File

Tick	1	EndOfFunc
BEGIN_LINE
0	9	5	1	-1	0
END_LINE
outputport

1	3	420	270	15	0	1	mandatory= 0	0	1
Playlist path

Next_Playlist	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	405	2	0	1	mandatory= 0	0	1
URL

Next_Playlist	3	Next_URL	1	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
outputport

1	3	420	445	3	0	1	mandatory= 0	0	1
------------

Got	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	455	4	0	1	mandatory= 0	0	1
------------

Got	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	435	5	0	1	mandatory= 0	0	1
Local File

Next_Playlist	4	Next_URL	2	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
END_LINE
outputport

1	3	420	490	6	0	1	mandatory= 0	0	1
-------------

Set_Paths	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	500	7	0	1	mandatory= 0	0	1
--------------

Set_Paths	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	530	8	0	1	mandatory= 0	0	1
start time

Tick	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	355	9	0	1	mandatory= 0	0	1
class

Tick	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	365	17	0	1	mandatory= 0	0	1
Text

Tick	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	345	11	0	1	mandatory= 0	0	1
Type

Tick	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	420	325	12	0	1	mandatory= 0	0	1
ID

Tick	5	EndOfFunc
BEGIN_LINE
0	1	7	1	-1	0
END_LINE
outputport

1	1	420	415	13	0	1	mandatory= 0	0	1
ID

Next_Playlist	5	Next_URL	3	EndOfFunc
BEGIN_LINE
0	1	8	1	-1	0
END_LINE
outputport

1	3	420	540	14	0	1	mandatory= 0	0	1
Stop time

Tick	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	420	565	10	0	1	mandatory= 0	0	1
Region JSON

Tick	8	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
END_LINE
outputport

1	3	420	575	16	0	1	mandatory= 0	0	1
AR JSON

Tick	9	EndOfFunc
BEGIN_LINE
0	22	5	1	-1	0
END_LINE
outputport

1	3	420	375	1	0	1	mandatory= 0	0	1
Region

Tick	10	EndOfFunc
BEGIN_LINE
0	36	5	1	-1	0
END_LINE
startport

2	-1	290	260	0	0	1	mandatory= 0	0	1
New Playlist

Next_Playlist	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
280	216
280	260
END_LINE
startport

2	-1	290	330	0	0	1	mandatory= 0	0	1
Last Ended

Played	0	EndOfFunc
BEGIN_LINE
1	70	0	2	-1	0
275	341
275	330
END_LINE
startport

2	3	290	385	2	0	1	mandatory= 0	0	1
Next URL

Next_URL	0	EndOfFunc
BEGIN_LINE
1	53	0	2	-1	0
260	376
260	385
END_LINE
startport

2	3	290	425	3	0	1	mandatory= 0	0	1
Got

Got	0	EndOfFunc
BEGIN_LINE
1	1	4	2	-1	0
455	395
455	215
255	215
255	425
END_LINE
startport

2	3	290	480	4	0	1	mandatory= 0	0	1
Set Paths

Set_Paths	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	290	295	5	0	1	mandatory= 0	0	1
Started Play

Started	0	EndOfFunc
BEGIN_LINE
1	24	1	2	-1	0
620	165
620	100
235	100
235	295
END_LINE
finishport

3	3	420	285	0	0	1	mandatory= 0	0	1
------------

Played	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	420	260	11	0	1	mandatory= 0	0	1
--------------

Next_Playlist	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	420	295	0	0	1	mandatory= 0	0	1
Play Now

Tick	11	EndOfFunc
BEGIN_LINE
0	25	0	3	-1	0
END_LINE
finishport

3	3	420	385	3	0	1	mandatory= 0	0	1
------------

Next_URL	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	420	395	4	0	1	mandatory= 0	0	1
Get

Next_Playlist	7	Next_URL	5	EndOfFunc
BEGIN_LINE
0	1	3	3	-1	0
END_LINE
finishport

3	3	420	425	5	0	1	mandatory= 0	0	1
------------

Got	6	EndOfFunc
BEGIN_LINE
0	53	0	3	-1	0
END_LINE
finishport

3	3	420	480	6	0	1	mandatory= 0	0	1
------------

Set_Paths	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	420	305	7	0	1	mandatory= 0	0	1
play remote

Tick	12	EndOfFunc
BEGIN_LINE
0	25	1	3	-1	0
END_LINE
finishport

3	3	420	520	8	0	1	mandatory= 0	0	1
Earliest

Tick	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	420	465	9	0	1	mandatory= 0	0	1
Tried all

Next_Playlist	8	Next_URL	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	420	315	10	0	1	mandatory= 0	0	1
Play Stop

Tick	14	EndOfFunc
BEGIN_LINE
0	70	1	3	-1	0
END_LINE
finishport

3	3	420	555	1	0	1	mandatory= 0	0	1
New Layout

Tick	15	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

JSONObjectFunctionBlock
_
3	0	0	0
_
_
Holds a memory representation of a JSON object for querying and building.
JSONObject
0
_
645	305	734	484
1	0
inputport

0	3	639	345	0	0	1	mandatory= 0	0	1
data

read	1	EndOfFunc
BEGIN_LINE
1	1	15	0	-1	0
580	565
580	345
END_LINE
inputport

0	3	639	385	0	0	1	mandatory= 0	0	1
path1

query	1	EndOfFunc
BEGIN_LINE
1	37	0	0	-1	0
629	485
629	385
END_LINE
inputport

0	3	639	395	2	0	1	mandatory= 0	0	1
path2

query	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	639	405	3	0	1	mandatory= 0	0	1
path3

query	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	639	415	4	0	1	mandatory= 0	0	1
path4

query	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	639	425	5	0	1	mandatory= 0	0	1
path5

query	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	639	435	6	0	1	mandatory= 0	0	1
path6

query	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	639	445	7	0	1	mandatory= 0	0	1
path7

query	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	639	455	8	0	1	mandatory= 0	0	1
path8

query	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	738	395	2	0	1	mandatory= 0	0	1
result2

query	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	738	405	1	0	1	mandatory= 0	0	1
result3

query	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	738	385	0	0	1	mandatory= 0	0	1
result1

query	11	EndOfFunc
BEGIN_LINE
0	4	5	1	-1	0
END_LINE
outputport

1	3	738	425	3	0	1	mandatory= 0	0	1
result5

query	12	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	738	435	4	0	1	mandatory= 0	0	1
result6

query	13	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	738	445	5	0	1	mandatory= 0	0	1
result7

query	14	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	738	455	6	0	1	mandatory= 0	0	1
result8

query	15	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	738	415	7	0	1	mandatory= 0	0	1
result4

query	16	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	639	315	0	0	1	mandatory= 0	0	1
reset

reset	0	EndOfFunc
BEGIN_LINE
1	1	11	2	-1	0
565	555
565	315
END_LINE
startport

2	-1	639	335	1	0	1	mandatory= 0	0	1
read

read	0	EndOfFunc
BEGIN_LINE
1	3	2	2	-1	0
753	315
753	290
624	290
624	335
END_LINE
startport

2	3	639	365	0	0	1	mandatory= 0	0	1
query

query	0	EndOfFunc
BEGIN_LINE
1	37	0	2	-1	0
615	465
615	365
END_LINE
finishport

3	-1	738	375	3	0	1	mandatory= 0	0	1
error

query	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	738	335	1	0	1	mandatory= 0	0	1
--

read	2	EndOfFunc
BEGIN_LINE
0	3	2	3	-1	0
END_LINE
finishport

3	-1	738	315	2	0	1	mandatory= 0	0	1
--

reset	1	EndOfFunc
BEGIN_LINE
0	3	1	3	-1	0
END_LINE
finishport

3	-1	738	365	0	0	1	mandatory= 0	0	1
--

query	18	EndOfFunc
BEGIN_LINE
0	4	4	3	-1	0
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
825	365	925	539
1	0
parameter
Widget Tag	5	_	_	QueryInfo
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

0	1	825	470	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	825	480	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	825	490	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	825	500	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	825	515	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	825	530	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	3	2	0	-1	0
765	385
765	530
END_LINE
outputport

1	1	925	470	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	925	480	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	925	490	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	925	500	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	825	375	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	825	385	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	825	400	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	825	410	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	825	455	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	3	3	2	-1	0
780	365
780	455
END_LINE
finishport

3	1	925	375	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	925	385	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	925	400	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	925	410	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	925	455	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	925	425	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	925	435	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
9	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
725	35	825	209
1	0
parameter
Widget Tag	5	_	_	MediaFile
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

0	1	725	140	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	150	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	160	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	170	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	725	185	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	725	200	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	1	0	0	-1	0
605	335
605	200
END_LINE
outputport

1	1	825	140	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	825	150	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	825	160	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	825	170	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	725	45	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	55	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	70	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	80	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	725	125	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	25	0	2	-1	0
580	286
580	125
END_LINE
finishport

3	1	825	45	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	55	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	70	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	80	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	125	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	825	95	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
finishport

3	1	825	105	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
15	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
640	700	740	874
1	0
parameter
Widget Tag	5	_	_	LayoutJSON
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

0	1	640	805	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	640	815	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	640	825	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	640	835	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	640	850	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	640	865	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	1	15	0	-1	0
590	565
590	865
END_LINE
outputport

1	1	740	805	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	740	815	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	740	825	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	740	835	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	640	710	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	640	720	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	640	735	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	640	745	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	640	790	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	11	2	-1	0
605	555
605	790
END_LINE
finishport

3	1	740	710	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	740	720	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	740	735	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	740	745	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	740	790	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	740	760	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	740	770	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
22	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
465	705	565	879
1	0
parameter
Widget Tag	5	_	_	ARJSON
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

0	1	465	810	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	465	820	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	465	830	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	465	840	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	465	855	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	465	870	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	1	16	0	-1	0
430	575
430	870
END_LINE
outputport

1	1	565	810	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	565	820	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	565	830	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	565	840	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	465	715	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	465	725	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	465	740	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	465	750	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	465	795	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	11	2	-1	0
445	555
445	795
END_LINE
finishport

3	1	565	715	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	565	725	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	565	740	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	565	750	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	565	795	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	565	765	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	565	775	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
23	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
100	245	200	290
1	0
parameter
String Constant	3	_	_	smil.xml
EndOfValues
EndOfLabels
string constant

outputport

1	3	200	275	0	0	1	mandatory= 0	0	1


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

time_clock
_
24	0	0	0
Play time
_
Creates periodic timed events.
time_clock
0
_
505	155	565	245
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	50000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	0
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	505	200	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	505	215	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	505	230	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	505	165	0	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	25	0	2	-1	0
495	286
495	165
END_LINE
startport

2	0	505	180	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	565	195	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
finishport

3	0	565	165	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	1	5	3	-1	0
END_LINE
finishport

3	0	565	180	2	0	1	mandatory= 0	0	1


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

trigger_eventor2
_
25	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
425	265	445	305
1	0
startport

2	-1	425	275	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
430	295
430	275
END_LINE
startport

2	-1	425	295	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	1	7	2	-1	0
430	305
430	295
END_LINE
finishport

3	3	445	286	2	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
36	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
815	695	915	869
1	0
parameter
Widget Tag	5	_	_	regionId
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

0	1	815	800	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	815	810	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	815	820	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	815	830	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	815	845	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	815	860	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	37	0	0	-1	0
630	485
630	640
755	640
755	860
END_LINE
outputport

1	1	915	800	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	915	810	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	915	820	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	915	830	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	815	705	0	1	0	mandatory= 1	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	815	715	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	815	730	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	815	740	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	815	785	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	37	0	2	-1	0
690	465
690	625
770	625
770	785
END_LINE
finishport

3	1	915	705	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	915	715	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	915	730	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	915	740	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	915	785	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	915	755	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	915	765	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
37	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
485	455	535	515
1	0
parameter
Format	3	_	_	[%s]
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	485	485	1	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	1	17	0	-1	0
444	375
444	485
END_LINE
inputport

0	3	485	500	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	535	485	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	3	1	1	-1	0
END_LINE
startport

2	-1	485	465	2	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	25	0	2	-1	0
455	286
455	465
END_LINE
finishport

3	3	535	465	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	36	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
53	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
215	355	235	395
1	0
startport

2	-1	215	365	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
430	425
430	235
210	235
210	365
END_LINE
startport

2	-1	215	385	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	235	376	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
54	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
140	195	160	235
1	0
startport

2	3	140	205	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	9	5	2	-1	0
835	95
835	150
130	150
130	205
END_LINE
startport

2	-1	140	225	1	1	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	160	216	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
69	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
75	460	175	505
1	0
parameter
String Constant	3	_	_	media/schedmedia/_data/
EndOfValues
EndOfLabels
string constant

outputport

1	3	175	490	0	0	1	mandatory= 0	0	1


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

trigger_eventor2
_
70	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
155	320	175	360
1	0
startport

2	3	155	330	1	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	24	0	2	-1	0
575	195
575	140
140	140
140	330
END_LINE
startport

2	-1	155	350	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	1	10	2	-1	0
435	315
435	235
140	235
140	350
END_LINE
finishport

3	-1	175	341	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
