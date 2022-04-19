#V:2.0.0

CanvasSizeXY
2000	1000

IconData
BEGIN_BLOCK

trigger_counter
_
7	0	0	0
_
_
This is an event counter with reset.
EventCounter
0
_
460	215	510	260
1	0
outputport

1	1	510	245	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	127	5	1	-1	0
END_LINE
startport

2	-1	460	225	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	18	0	2	-1	1
450	510
450	225
END_LINE
startport

2	-1	460	245	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	510	225	2	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	127	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
8	0	0	0
output
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
585	390	685	564
1	0
parameter
Widget Tag	5	_	_	dirOut
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	585	495	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	505	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	515	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	525	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	585	540	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	585	555	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	18	0	0	-1	1
525	567
525	555
END_LINE
outputport

1	1	685	495	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	505	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	515	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	685	525	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	585	400	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	410	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	425	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	435	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	585	480	3	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	1
540	541
540	480
END_LINE
finishport

3	1	685	400	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	410	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	425	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	435	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	480	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	685	450	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
9	0	0	0
setDir
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
0
_
575	105	675	279
1	0
parameter
Widget Tag	5	_	_	setDir
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	575	210	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	575	220	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	575	230	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	575	240	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	575	255	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	575	270	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	7	0	0	-1	1
515	245
515	270
END_LINE
outputport

1	1	675	210	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	675	220	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	675	230	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	675	240	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	575	115	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	575	125	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
1	18	1	2	-1	1
505	527
505	125
END_LINE
startport

2	-1	575	140	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	575	150	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
1	18	4	2	-1	1
540	557
540	150
END_LINE
startport

2	-1	575	195	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	7	0	2	-1	1
530	225
530	195
END_LINE
finishport

3	1	675	115	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	675	125	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	675	140	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	675	150	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	675	195	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	675	165	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
16	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
110	520	210	565
1	0
parameter
String Constant	3	_	_	./deleteme
EndOfValues
EndOfLabels
string constant

outputport

1	3	210	550	0	0	1	mandatory= 0	0	1


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

fs_dir_list
_
18	0	0	0
_
_
Allows a directory path to be set and outputs files contained in the directory on each get. Separate events are asserted for each type of file or directory.
FileSystemDirList1
0
_
310	500	410	589
1	0
parameter
Directory	3	_	_	_
EndOfValues
EndOfLabels
Media Directory

parameter
Use Application Dir	0	_	_	0
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	310	520	0	0	1	mandatory= 0	0	1
dir

set	1	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	1
250	550
250	520
END_LINE
outputport

1	3	410	567	0	0	1	mandatory= 0	0	1
name

get	1	EndOfFunc
BEGIN_LINE
0	126	5	1	-1	0
END_LINE
startport

2	-1	310	510	0	1	1	mandatory= 0	0	1
set dir

set	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	310	537	2	0	1	mandatory= 0	0	1
get entry

get	0	EndOfFunc
BEGIN_LINE
1	8	5	2	-1	1
700	450
700	635
295	635
295	537
END_LINE
startport

2	3	310	577	2	0	1	mandatory= 0	0	1
release

rel	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	410	510	2	0	1	mandatory= 0	0	1
Ready

set	2	EndOfFunc
BEGIN_LINE
0	9	0	3	-1	0
END_LINE
finishport

3	3	410	527	1	0	1	mandatory= 0	0	1
Error

get	5	EndOfFunc
BEGIN_LINE
0	9	1	3	-1	0
END_LINE
finishport

3	-1	410	537	0	0	1	mandatory= 0	0	1
file

get	2	EndOfFunc
BEGIN_LINE
0	19	0	3	-1	0
END_LINE
finishport

3	3	410	547	3	0	1	mandatory= 0	0	1
dir

get	3	EndOfFunc
BEGIN_LINE
0	19	1	3	-1	0
END_LINE
finishport

3	3	410	557	5	0	1	mandatory= 0	0	1
no more

get	4	EndOfFunc
BEGIN_LINE
0	9	3	3	-1	0
END_LINE
finishport

3	3	410	577	4	0	1	mandatory= 0	0	1
released

rel	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
19	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
450	520	470	560
1	0
startport

2	-1	450	530	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	18	2	2	-1	1
445	537
445	530
END_LINE
startport

2	-1	450	550	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	18	3	2	-1	1
435	547
435	550
END_LINE
finishport

3	3	470	541	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	126	4	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
