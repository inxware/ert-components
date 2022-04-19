CanvasSizeXY
2050	2000

IconData
BEGIN_BLOCK

trigger_counter
_
7	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
510	155	560	200
1	0
outputport

1	1	560	185	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	127	5	1	-1	0
END_LINE
startport

2	-1	510	165	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	94	0	2	-1	0
505	370
505	165
END_LINE
startport

2	-1	510	185	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	560	165	2	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	127	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
8	0	0	0
dirOut
_
This widget displays an string to the GUI output.
gui_text_string2
_
635	390	735	564
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

0	1	635	495	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	505	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	515	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	525	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	540	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	635	555	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	94	0	0	-1	0
575	427
575	555
END_LINE
outputport

1	1	735	495	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	735	505	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	735	515	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	735	525	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	635	400	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	410	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	425	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	435	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	480	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	95	0	2	-1	0
590	461
590	480
END_LINE
finishport

3	1	735	400	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	410	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	425	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	435	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	480	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
finishport

3	1	735	450	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
finishport

3	1	735	460	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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
_
830	165	930	339
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

0	1	830	270	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	830	280	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	830	290	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	830	300	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	830	315	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	830	330	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	19	0	0	-1	0
770	195
770	330
END_LINE
outputport

1	1	930	270	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	930	280	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	930	290	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	930	300	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	830	175	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	830	185	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	830	200	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	830	210	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	830	255	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	0
785	175
785	255
END_LINE
finishport

3	1	930	175	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	930	185	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	930	200	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	930	210	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	930	255	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	930	225	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	94	0	3	-1	0
END_LINE
finishport

3	1	930	235	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
19	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
_
680	165	730	310
1	0
inputport

0	1	680	190	5	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	7	0	0	-1	0
635	185
635	190
END_LINE
inputport

0	1	680	225	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	20	0	0	-1	0
660	255
660	225
END_LINE
inputport

0	1	680	260	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	31	0	0	-1	0
650	305
650	260
END_LINE
inputport

0	1	680	295	3	0	1	mandatory= 0	0	1
i4

Run_Input4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	730	195	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	EndOfFunc
BEGIN_LINE
0	9	5	1	-1	0
END_LINE
startport

2	-1	680	175	4	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	7	0	2	-1	0
640	165
640	175
END_LINE
startport

2	1	680	210	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	94	1	2	-1	0
665	387
665	210
END_LINE
startport

2	1	680	245	2	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
1	94	4	2	-1	0
655	417
655	245
END_LINE
startport

2	1	680	280	3	0	1	mandatory= 0	0	1


Run_Input4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	730	175	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	Run_Input4	3	EndOfFunc
BEGIN_LINE
0	9	4	3	-1	0
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
540	225	640	270
1	0
parameter
Integer Value	1	-2147483648	2147483647	-1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	640	255	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	19	1	1	-1	0
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
31	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
540	275	640	320
1	0
parameter
Integer Value	1	-2147483648	2147483647	-2
EndOfValues
EndOfLabels
integer constant

outputport

1	1	640	305	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	19	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
33	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_String
_
840	755	940	865
1	0
parameter
File name	3	_	_	readme.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
in the form (x) where x is the delimiter

parameter
Use Application Dir	0	_	_	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	840	775	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_String	1	EndOfFunc
BEGIN_LINE
1	118	0	0	-1	1
830	695
830	775
END_LINE
outputport

1	3	940	840	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_String	1	EndOfFunc
BEGIN_LINE
0	87	5	1	-1	0
END_LINE
outputport

1	1	940	850	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_String	2	Run_CloseFILE_ReadOnly_String	1	Run_OpenFILE_ReadOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	840	765	1	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
1	96	0	2	-1	1
975	686
975	750
835	750
835	765
END_LINE
startport

2	-1	840	790	0	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
1	94	2	2	-1	1
825	397
825	790
END_LINE
startport

2	-1	840	815	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
1	43	5	2	-1	1
820	670
820	815
END_LINE
finishport

3	-1	940	765	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	33	2	3	-1	0
END_LINE
finishport

3	-1	940	790	0	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_String	2	EndOfFunc
BEGIN_LINE
0	96	0	3	-1	0
END_LINE
finishport

3	1	940	815	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	87	4	3	-1	0
END_LINE
finishport

3	1	940	775	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	800	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	96	1	3	-1	0
END_LINE
finishport

3	1	940	825	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
43	0	0	0
fileOut
_
This widget displays an string to the GUI output.
gui_text_string2
_
635	610	735	784
1	0
parameter
Widget Tag	5	_	_	fileOut
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

0	1	635	715	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	725	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	735	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	745	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	635	760	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	635	775	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	94	0	0	-1	1
575	427
575	775
END_LINE
outputport

1	1	735	715	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	735	725	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	735	735	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	735	745	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	635	620	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	630	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	645	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	635	655	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	635	700	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	94	2	2	-1	1
590	397
590	700
END_LINE
finishport

3	1	735	620	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	630	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	645	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	655	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	735	700	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	1	3	-1	0
END_LINE
finishport

3	1	735	670	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	33	2	3	-1	0
END_LINE
finishport

3	1	735	680	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
44	0	0	0
_
_
This is a string constant.
Constant_String1
_
205	365	305	410
1	0
parameter
String Constant	3	_	_	_
EndOfValues
EndOfLabels
string constant

outputport

1	3	305	395	0	0	1	mandatory= 0	0	1

dir
Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	4	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
72	0	0	0
_
_
This is a string constant.
Constant_String1
_
145	460	245	505
1	0
parameter
String Constant	3	_	_	/
EndOfValues
EndOfLabels
string constant

outputport

1	3	245	490	0	0	1	mandatory= 0	0	1

sep
Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	45	1	1	-1	0
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
87	0	0	0
output
_
This widget displays an string to the GUI output.
gui_text_string2
_
1125	635	1225	809
1	0
parameter
Widget Tag	5	_	_	output
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

0	1	1125	740	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1125	750	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1125	760	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1125	770	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1125	785	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1125	800	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	33	0	0	-1	1
955	840
955	800
END_LINE
outputport

1	1	1225	740	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1225	750	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1225	760	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1225	770	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1125	645	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1125	655	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1125	670	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1125	680	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	1125	725	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	33	2	2	-1	1
970	815
970	725
END_LINE
finishport

3	1	1225	645	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1225	655	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1225	670	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1225	680	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1225	725	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
finishport

3	1	1225	695	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	33	2	3	-1	0
END_LINE
finishport

3	1	1225	705	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ror
_
88	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Real
_
840	885	940	995
1	0
parameter
File name	3	_	_	inr.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
Real Field Width

parameter
Delimiter	3	_	_	( )
EndOfValues
EndOfLabels
Real Field Delimiter Character

parameter
Use Application Dir	0	_	_	0
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	840	905	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	2	940	970	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	91	5	1	-1	0
END_LINE
outputport

1	1	940	980	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Real	2	Run_OpenFILE_ReadOnly_Real	2	Run_CloseFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	840	895	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	840	920	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	840	945	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	940	895	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	940	920	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Real	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	945	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	91	4	3	-1	0
END_LINE
finishport

3	1	940	905	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	930	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	955	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_roi
_
89	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Int
_
840	1010	940	1120
1	0
parameter
File name	3	_	_	inint.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
Integer Field Width

parameter
Delimiter	3	_	_	( )
EndOfValues
EndOfLabels
Integer Field Delimiter Character

parameter
Use Application Dir	0	_	_	0
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	840	1030	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	940	1095	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Int	1	EndOfFunc
BEGIN_LINE
0	93	5	1	-1	0
END_LINE
outputport

1	1	940	1105	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Int	2	Run_CloseFILE_ReadOnly_Int	1	Run_OpenFILE_ReadOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	840	1020	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	840	1045	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	840	1070	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	940	1020	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	940	1045	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	1070	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	93	4	3	-1	0
END_LINE
finishport

3	1	940	1030	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	1055	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	1080	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_rob
_
90	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Bool
_
840	1140	940	1250
1	0
parameter
File name	3	_	_	inbool.txt
EndOfValues
EndOfLabels
output file name

parameter
Delimiter	3	_	_	()
EndOfValues
EndOfLabels
Boolean Field Delimiter Character

parameter
Use Application Dir	0	_	_	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	840	1160	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	940	1225	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	92	5	1	-1	0
END_LINE
outputport

1	1	940	1235	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Bool	2	Run_CloseFILE_ReadOnly_Bool	1	Run_OpenFILE_ReadOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	840	1150	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	840	1175	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	840	1200	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	940	1150	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	940	1175	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	1200	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	92	4	3	-1	0
END_LINE
finishport

3	1	940	1160	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	1185	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	1210	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
91	0	0	0
outputF
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
1120	870	1220	1044
1	0
parameter
Widget Tag	5	_	_	outputF
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

0	1	1120	975	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	985	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	995	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	1005	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	1020	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	1120	1035	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	88	0	0	-1	0
1060	970
1060	1035
END_LINE
outputport

1	1	1220	975	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1220	985	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1220	995	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1220	1005	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1120	880	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	890	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	905	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	915	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	960	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	88	2	2	-1	0
1075	945
1075	960
END_LINE
finishport

3	1	1220	880	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	890	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	905	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	915	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	960	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	930	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	940	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_bool2
_
92	0	0	0
outputB
_
This widget displays an boolean to the GUI output - currently as o or 1! (to save you converting to a string!).
gui_text_bool2
_
1120	1130	1220	1304
1	0
parameter
Widget Tag	5	_	_	outputB
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

0	1	1120	1235	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	1245	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	1255	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	1265	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1120	1280	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	1120	1295	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	90	0	0	-1	0
1060	1225
1060	1295
END_LINE
outputport

1	1	1220	1235	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1220	1245	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1220	1255	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1220	1265	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1120	1140	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	1150	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	1165	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	1175	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1120	1220	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	90	2	2	-1	0
1075	1200
1075	1220
END_LINE
finishport

3	1	1220	1140	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	1150	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	1165	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	1175	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	1220	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	1190	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1220	1200	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
93	0	0	0
outputI
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1005	990	1105	1164
1	0
parameter
Widget Tag	5	_	_	outputI
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

0	1	1005	1095	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1005	1105	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1005	1115	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1005	1125	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1005	1140	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1005	1155	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	89	0	0	-1	0
945	1095
945	1155
END_LINE
outputport

1	1	1105	1095	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1105	1105	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1105	1115	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1105	1125	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1005	1000	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1005	1010	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1005	1025	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1005	1035	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1005	1080	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	89	2	2	-1	0
960	1070
960	1080
END_LINE
finishport

3	1	1105	1000	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1105	1010	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1105	1025	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1105	1035	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1105	1080	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1105	1050	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1105	1060	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

fs_dir_list
_
94	0	0	0
_
_
Allows a directory path to be set and outputs files contained in the directory on each get. Separate events are asserted for each type of file or directory.
FileSystemDirList1
_
395	360	495	449
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

0	3	395	380	0	0	1	mandatory= 0	0	1
dir
dir
set	1	EndOfFunc
BEGIN_LINE
1	44	0	0	-1	0
345	395
345	380
END_LINE
outputport

1	3	495	427	0	0	1	mandatory= 0	0	1
name

get	1	EndOfFunc
BEGIN_LINE
0	33	0	1	-1	0
END_LINE
startport

2	3	395	370	2	0	1	mandatory= 0	0	1
set dir

set	0	EndOfFunc
BEGIN_LINE
1	9	5	2	-1	0
935	225
935	160
390	160
390	370
END_LINE
startport

2	-1	395	397	2	0	1	mandatory= 0	0	1
get entry

get	0	EndOfFunc
BEGIN_LINE
1	8	5	2	-1	0
750	450
750	485
380	485
380	397
END_LINE
finishport

3	3	495	370	2	0	1	mandatory= 0	0	1
Ready

set	2	EndOfFunc
BEGIN_LINE
0	9	0	3	-1	0
END_LINE
finishport

3	3	495	387	0	0	1	mandatory= 0	0	1
Error

get	5	EndOfFunc
BEGIN_LINE
0	19	1	3	-1	0
END_LINE
finishport

3	3	495	397	0	0	1	mandatory= 0	0	1
file

get	2	EndOfFunc
BEGIN_LINE
0	33	1	3	-1	0
END_LINE
finishport

3	3	495	407	3	0	1	mandatory= 0	0	1
dir

get	3	EndOfFunc
BEGIN_LINE
0	95	1	3	-1	0
END_LINE
finishport

3	3	495	417	1	0	1	mandatory= 0	0	1
no more

get	4	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
95	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
610	440	630	480
1	0
startport

2	-1	610	450	3	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	94	2	2	-1	0
565	397
565	450
END_LINE
startport

2	-1	610	470	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	94	3	2	-1	0
595	407
595	470
END_LINE
finishport

3	-1	630	461	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	8	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
96	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
950	665	970	705
1	0
startport

2	-1	950	675	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	33	1	2	-1	0
945	790
945	675
END_LINE
startport

2	-1	950	695	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	33	4	2	-1	0
945	800
945	695
END_LINE
finishport

3	-1	970	686	1	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
118	0	0	0
_
_
This is a string constant.
Constant_String1
_
250	665	350	710
1	0
parameter
String Constant	3	_	_	readme.txt
EndOfValues
EndOfLabels
string constant

outputport

1	3	350	695	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	33	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
