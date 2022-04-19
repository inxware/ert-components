CanvasSizeXY
2000	2000

IconData
BEGIN_BLOCK

playlist_manager
_
1	0	0	0
_
_
Manages parsing playlists and controlling media players
PlayManager
_
490	180	620	479
1	0
parameter
Start Behaviour	1	0	2	0
EndOfValues
EndOfLabels
0:CLEAR_ALL, 1:KEEP_EARLIERTHANNEW, 2:KEEP_ALL

inputport

0	3	490	200	0	0	1	mandatory= 0	0	1
Playlist path

Next_Playlist	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	490	365	1	0	1	mandatory= 0	0	1
URL

Got	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	490	375	2	0	1	mandatory= 0	0	1
Local File

Got	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	490	410	3	0	1	mandatory= 0	0	1
DL URL

Set_Paths	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	490	420	4	0	1	mandatory= 0	0	1
DL local

Set_Paths	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	490	430	5	0	1	mandatory= 0	0	1
start mode

Set_Paths	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	490	235	6	0	1	mandatory= 0	0	1
ID

Started	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	490	260	7	0	1	mandatory= 0	0	1
ID

Played	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	490	355	8	0	1	mandatory= 0	0	1
ID

Got	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	620	255	0	0	1	mandatory= 0	0	1
Media File

Tick	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	200	1	0	1	mandatory= 0	0	1
Playlist path

Next_Playlist	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	325	2	0	1	mandatory= 0	0	1
URL

Next_Playlist	3	Next_URL	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	365	3	0	1	mandatory= 0	0	1
------------

Got	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	375	4	0	1	mandatory= 0	0	1
------------

Got	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	355	5	0	1	mandatory= 0	0	1
Local File

Next_Playlist	4	Next_URL	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	410	6	0	1	mandatory= 0	0	1
-------------

Set_Paths	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	420	7	0	1	mandatory= 0	0	1
--------------

Set_Paths	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	450	8	0	1	mandatory= 0	0	1
start time

Tick	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	275	9	0	1	mandatory= 0	0	1
class

Tick	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	285	10	0	1	mandatory= 0	0	1
Text

Tick	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	265	11	0	1	mandatory= 0	0	1
Type

Tick	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	620	245	12	0	1	mandatory= 0	0	1
ID

Tick	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	620	335	13	0	1	mandatory= 0	0	1
ID

Next_Playlist	5	Next_URL	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	620	460	14	0	1	mandatory= 0	0	1
Stop time

Tick	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	490	190	0	0	1	mandatory= 0	0	1
New Playlist

Next_Playlist	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	490	250	1	0	1	mandatory= 0	0	1
Last Ended

Played	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	490	305	2	0	1	mandatory= 0	0	1
Next URL

Next_URL	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	490	345	3	0	1	mandatory= 0	0	1
Got

Got	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	490	400	4	0	1	mandatory= 0	0	1
Set Paths

Set_Paths	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	490	225	5	0	1	mandatory= 0	0	1
Started Play

Started	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	620	215	0	0	1	mandatory= 0	0	1
------------

Played	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	190	1	0	1	mandatory= 0	0	1
--------------

Next_Playlist	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	225	2	0	1	mandatory= 0	0	1
Play Now

Tick	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	305	3	0	1	mandatory= 0	0	1
------------

Next_URL	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	315	4	0	1	mandatory= 0	0	1
Get

Next_Playlist	7	Next_URL	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	345	5	0	1	mandatory= 0	0	1
------------

Got	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	400	6	0	1	mandatory= 0	0	1
------------

Set_Paths	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	620	235	7	0	1	mandatory= 0	0	1
play remote

Tick	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	440	8	0	1	mandatory= 0	0	1
Earliest

Tick	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	620	385	9	0	1	mandatory= 0	0	1
tried all

Next_Playlist	8	Next_URL	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

devman_player
_
2	0	0	0
_
_
This component provides contact with a devman server to enable the exchange of playlists and player status information. The component detects new playlists made available on the devman server, downloads them and notifies the application that it is availab
DevmanPlayer
_
115	115	246	454
1	0
parameter
Devman Server URL	3	_	_	_
EndOfValues
EndOfLabels
devman server url

parameter
Playlist Path	3	_	_	_
EndOfValues
EndOfLabels
playlist path

parameter
Devman Ping Period	1	1	3600	60
EndOfValues
EndOfLabels
devman ping period in seconds

parameter
Use Devman Core URL	0	_	_	1
EndOfValues
EndOfLabels
Use the URL specified by devman.url

parameter
Start on Init	0	_	_	1
EndOfValues
EndOfLabels
Start checking on init

inputport

0	3	115	170	0	0	1	mandatory= 0	0	1
URL

changeurl	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	115	210	1	0	1	mandatory= 0	0	1
plylst path

reconfigure	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	115	225	2	0	1	mandatory= 0	0	1
update rate

reconfigure	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	115	280	3	0	1	mandatory= 0	0	1
plylst index

trackchanged	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	115	295	4	0	1	mandatory= 0	0	1
curr. media

trackchanged	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	115	310	5	0	1	mandatory= 0	0	1
start time

trackchanged	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	115	325	6	0	1	mandatory= 0	0	1
media duration

trackchanged	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	115	265	7	0	1	mandatory= 0	0	1
playlist

trackchanged	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	115	340	8	0	1	mandatory= 0	0	1
player state

trackchanged	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	245	210	0	0	1	mandatory= 0	0	1
plylst URL

out	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	230	1	0	1	mandatory= 0	0	1
media/ URL

out	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	270	2	0	1	mandatory= 0	0	1
files

out	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	330	3	0	1	mandatory= 0	0	1
track url

out	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	245	360	4	0	1	mandatory= 0	0	1
volume

out	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	245	375	5	0	1	mandatory= 0	0	1
channels

out	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	245	405	6	0	1	mandatory= 0	0	1
on/off

out	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	245	430	7	0	1	mandatory= 0	0	1
state

out	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	240	8	0	1	mandatory= 0	0	1
media /path

out	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	220	9	0	1	mandatory= 0	0	1
X-playlist

out	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	445	10	0	1	mandatory= 0	0	1
Raw XML

out	11	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	245	170	11	0	1	mandatory= 0	0	1
URL

changeurl	2	out	12	startchecking	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	115	125	0	0	0	mandatory= 0	0	1
Activate

startchecking	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	115	140	1	0	0	mandatory= 0	0	1
De-activate

stopchecking	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	115	155	2	0	0	mandatory= 0	0	1
Set url

changeurl	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	115	185	3	0	0	mandatory= 0	0	1
Config

reconfigure	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	115	250	4	0	0	mandatory= 0	0	1
track info

trackchanged	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	245	125	0	0	1	mandatory= 0	0	1
------------

startchecking	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	245	140	1	0	1	mandatory= 0	0	1
------------

stopchecking	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	245	155	2	0	1	mandatory= 0	0	1
------------

changeurl	3	startchecking	3	out	21	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	245	185	3	0	1	mandatory= 0	0	1
------------

reconfigure	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	245	200	4	0	1	mandatory= 0	0	1
new playlist

out	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	245	260	5	0	1	mandatory= 0	0	1
rem. list

out	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	245	285	6	0	1	mandatory= 0	0	1
rem. unused

out	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	245	300	7	0	1	mandatory= 0	0	1
rem. all

out	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	245	315	8	0	1	mandatory= 0	0	1
Override

out	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	245	345	9	0	1	mandatory= 0	0	1
media parms

out	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	245	390	10	0	1	mandatory= 0	0	1
set mute

out	19	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	245	420	11	0	1	mandatory= 0	0	1
status

out	20	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	1	mandatory= 0	0	1


out	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
