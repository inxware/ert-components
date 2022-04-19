#V:2.0.0

CanvasSizeXY
2000	1000

IconData
BEGIN_BLOCK

devman_player
_
2	0	0	0
_
_
This component provides contact with a devman server to enable the exchange of playlists and player status information. The component detects new playlists made available on the devman server, downloads them and notifies the application that it is availab
DevmanPlayer
0
_
100	100	231	439
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

0	3	100	155	0	0	1	mandatory= 0	0	1
URL

changeurl	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	100	190	1	0	1	mandatory= 0	0	1
plylst path

reconfigure	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	100	205	2	0	1	mandatory= 0	0	1
update rate

reconfigure	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	100	295	3	0	1	mandatory= 0	0	1
plylst index

trackchanged	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	100	310	4	0	1	mandatory= 0	0	1
curr. media

trackchanged	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	100	325	5	0	1	mandatory= 0	0	1
start time

trackchanged	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	100	340	6	0	1	mandatory= 0	0	1
media duration

trackchanged	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	100	280	7	0	1	mandatory= 0	0	1
playlist id

trackchanged	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	100	355	8	0	1	mandatory= 0	0	1
player state

trackchanged	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	100	370	9	0	1	mandatory= 0	0	1
Meas. Volume

trackchanged	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	230	195	0	0	1	mandatory= 0	0	1
plylst URL

out	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	215	1	0	1	mandatory= 0	0	1
media/ URL

out	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	255	2	0	1	mandatory= 0	0	1
files

out	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	315	3	0	1	mandatory= 0	0	1
track url

out	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	230	345	4	0	1	mandatory= 0	0	1
volume

out	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	230	360	5	0	1	mandatory= 0	0	1
channels

out	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	230	390	6	0	1	mandatory= 0	0	1
on/off

out	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	230	415	7	0	1	mandatory= 0	0	1
state

out	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	225	8	0	1	mandatory= 0	0	1
media /path

out	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	205	9	0	1	mandatory= 0	0	1
X-playlist

out	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	430	10	0	1	mandatory= 0	0	1
Raw XML

out	11	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	230	155	11	0	1	mandatory= 0	0	1
URL

changeurl	2	out	12	startchecking	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	100	110	0	0	0	mandatory= 0	0	1
Activate

startchecking	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	100	125	1	0	0	mandatory= 0	0	1
De-activate

stopchecking	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	100	140	2	0	0	mandatory= 0	0	1
Set url

changeurl	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	100	175	3	0	0	mandatory= 0	0	1
Config

reconfigure	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	100	265	4	0	0	mandatory= 0	0	1
track info

trackchanged	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	100	235	5	0	0	mandatory= 0	0	1
Rst media src

reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	230	110	0	0	1	mandatory= 0	0	1
------------

startchecking	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	230	125	1	0	1	mandatory= 0	0	1
------------

stopchecking	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	230	140	2	0	1	mandatory= 0	0	1
------------

changeurl	3	startchecking	3	out	21	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	170	3	0	1	mandatory= 0	0	1
------------

reconfigure	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	185	4	0	1	mandatory= 0	0	1
new playlist

out	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	230	245	5	0	1	mandatory= 0	0	1
rem. list

out	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	230	270	6	0	1	mandatory= 0	0	1
rem. unused

out	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	230	285	7	0	1	mandatory= 0	0	1
rem. all

out	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	230	300	8	0	1	mandatory= 0	0	1
Override

out	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	230	330	9	0	1	mandatory= 0	0	1
media parms

out	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	375	10	0	1	mandatory= 0	0	1
set mute

out	19	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	230	405	11	0	1	mandatory= 0	0	1
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
