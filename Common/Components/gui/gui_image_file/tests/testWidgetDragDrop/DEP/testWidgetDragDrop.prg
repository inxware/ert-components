CanvasSizeXY
3160	13410

IconData
BEGIN_BLOCK

gui_text_real2
_
1	0	0	0
_
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
895	835	995	1009
1	0
parameter
Widget Tag	5	_	_	portait_png
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

0	1	895	940	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	895	950	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	895	960	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
1	159	0	0	-1	0
865	970
865	960
END_LINE
inputport

0	1	895	970	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	895	985	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	895	1000	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	995	940	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	995	950	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	995	960	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	995	970	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	895	845	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	895	855	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	895	870	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	895	880	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	895	925	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
1010	895
1010	820
880	820
880	925
END_LINE
finishport

3	1	995	845	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	995	855	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	995	870	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	995	880	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	995	925	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
finishport

3	3	995	895	0	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
9	0	0	0
x
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1645	980	1745	1154
1	0
parameter
Widget Tag	5	_	_	x
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
Register Click Disregarding Z-Order	0	_	_	1
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	1645	1085	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1645	1095	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1645	1105	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1645	1115	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1645	1130	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1645	1145	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	31	0	0	-1	0
1585	1175
1585	1145
END_LINE
outputport

1	1	1745	1085	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1745	1095	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1745	1105	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1745	1115	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1645	990	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1645	1000	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1645	1015	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1645	1025	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1645	1070	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	4	2	-1	0
END_LINE
finishport

3	1	1745	990	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1745	1000	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1745	1015	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1745	1025	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1745	1070	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1745	1040	5	0	1	mandatory= 0	0	1
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
y
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1780	1045	1880	1219
1	0
parameter
Widget Tag	5	_	_	y
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

0	1	1780	1150	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1780	1160	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1780	1170	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1780	1180	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1780	1195	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1780	1210	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	161	0	0	-1	0
1720	1600
1720	1210
END_LINE
outputport

1	1	1880	1150	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1880	1160	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1880	1170	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1880	1180	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1780	1055	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1780	1065	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1780	1080	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1780	1090	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1780	1135	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	161	0	2	-1	0
1735	1580
1735	1135
END_LINE
finishport

3	1	1880	1055	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1880	1065	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1880	1080	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1880	1090	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1880	1135	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1880	1105	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
26	0	0	0
width
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
1940	1100	2040	1274
1	0
parameter
Widget Tag	5	_	_	width
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

0	1	1940	1205	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1940	1215	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1940	1225	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1940	1235	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1940	1250	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1940	1265	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	183	0	0	-1	0
1880	1645
1880	1265
END_LINE
outputport

1	1	2040	1205	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2040	1215	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2040	1225	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2040	1235	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1940	1110	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1940	1120	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1940	1135	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1940	1145	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1940	1190	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	183	0	2	-1	0
1895	1625
1895	1190
END_LINE
finishport

3	1	2040	1110	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2040	1120	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2040	1135	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2040	1145	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2040	1190	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2040	1160	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
28	0	0	0
height
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
2090	1180	2190	1354
1	0
parameter
Widget Tag	5	_	_	height
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

0	1	2090	1285	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2090	1295	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2090	1305	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2090	1315	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2090	1330	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	2090	1345	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	185	0	0	-1	0
2030	1695
2030	1345
END_LINE
outputport

1	1	2190	1285	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2190	1295	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2190	1305	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	2190	1315	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	2090	1190	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2090	1200	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2090	1215	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2090	1225	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	2090	1270	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	185	0	2	-1	0
2045	1675
2045	1270
END_LINE
finishport

3	1	2190	1190	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2190	1200	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2190	1215	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2190	1225	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2190	1270	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	2190	1240	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
30	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
1085	1235	1185	1280
1	0
parameter
Integer Value	1	-2147483648	2147483647	100
EndOfValues
EndOfLabels
integer constant

outputport

1	1	1185	1265	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	3	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_image_file
_
31	0	0	0
_
_
Displays an image based on a filename. Use create to load new filenames dynamically.
GUI_Image_File
_
1290	1055	1390	1230
1	0
parameter
Widget Tag	5	_	_	widget11
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	1
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	1290	1175	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1290	1185	3	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1290	1195	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
1	30	0	0	-1	0
1205	1265
1205	1195
END_LINE
inputport

0	1	1290	1205	1	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1290	1220	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1290	1075	1	0	1	mandatory= 0	0	1
file

create	1	EndOfFunc
BEGIN_LINE
1	73	0	0	-1	0
1230	1115
1230	1075
END_LINE
outputport

1	1	1390	1175	0	0	1	mandatory= 0	0	1
abs x

create	2	update	6	EndOfFunc
BEGIN_LINE
0	29	0	1	-1	0
END_LINE
outputport

1	1	1390	1185	0	0	1	mandatory= 0	0	1
abs y

create	3	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1390	1195	0	0	1	mandatory= 0	0	1
wid

create	4	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1390	1205	0	0	1	mandatory= 0	0	1
ht

create	5	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1290	1065	1	0	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
1	31	1	2	-1	0
1400	1090
1400	1030
1280	1030
1280	1065
END_LINE
startport

2	-1	1290	1090	0	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
1	73	0	2	-1	0
1270	1095
1270	1090
END_LINE
startport

2	-1	1290	1105	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1290	1115	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1290	1160	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	5	2	-1	0
1400	1130
1400	1115
1280	1115
1280	1160
END_LINE
finishport

3	3	1390	1065	0	0	1	mandatory= 0	0	1
--

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1390	1090	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	31	0	3	-1	0
END_LINE
finishport

3	3	1390	1105	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1390	1115	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1390	1155	5	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
finishport

3	1	1390	1130	4	0	1	mandatory= 0	0	1
click

create	7	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
32	0	0	0
_
_
This is a string constant.
Constant_String1
_
970	1140	1070	1185
1	0
parameter
String Constant	3	_	_	screenshot.jpg
EndOfValues
EndOfLabels
string constant

outputport

1	3	1070	1170	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	73	2	1	-1	0
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
68	0	0	0
_
_
This is a string constant.
Constant_String1
_
970	1055	1070	1100
1	0
parameter
String Constant	3	_	_	portrait.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	1070	1085	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	73	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
73	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
_
1165	1085	1215	1230
1	0
inputport

0	3	1165	1110	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	68	0	0	-1	0
1155	1085
1155	1110
END_LINE
inputport

0	3	1165	1145	5	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	91	0	0	-1	0
1145	1130
1145	1145
END_LINE
inputport

0	3	1165	1180	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
1135	1170
1135	1180
END_LINE
inputport

0	3	1165	1215	3	0	1	mandatory= 0	0	1
i4

Run_Input4	1	EndOfFunc
BEGIN_LINE
1	93	0	0	-1	0
1125	1215
1125	1215
END_LINE
outputport

1	3	1215	1115	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	EndOfFunc
BEGIN_LINE
0	31	5	1	-1	0
END_LINE
startport

2	3	1165	1095	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
1020	895
1020	1095
END_LINE
startport

2	3	1165	1130	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	113	5	2	-1	0
1150	890
1150	1130
END_LINE
startport

2	3	1165	1165	2	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
1	115	5	2	-1	0
1305	890
1305	805
1115	805
1115	1165
END_LINE
startport

2	3	1165	1200	3	0	1	mandatory= 0	0	1


Run_Input4	0	EndOfFunc
BEGIN_LINE
1	117	5	2	-1	0
1450	895
1450	790
1095	790
1095	1200
END_LINE
finishport

3	3	1215	1095	5	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	Run_Input4	3	EndOfFunc
BEGIN_LINE
0	31	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
91	0	0	0
_
_
This is a string constant.
Constant_String1
_
970	1100	1070	1145
1	0
parameter
String Constant	3	_	_	screenshot.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	1070	1130	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	31	5	1	-1	0
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
93	0	0	0
_
_
This is a string constant.
Constant_String1
_
970	1185	1070	1230
1	0
parameter
String Constant	3	_	_	screenshot.gif
EndOfValues
EndOfLabels
string constant

outputport

1	3	1070	1215	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	73	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
113	0	0	0
_
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
1045	830	1145	1004
1	0
parameter
Widget Tag	5	_	_	screenshot_png
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1045	935	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1045	945	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1045	955	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1045	965	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1045	980	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	1045	995	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1145	935	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1145	945	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1145	955	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1145	965	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1045	840	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1045	850	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1045	865	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1045	875	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1045	920	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	1	1145	840	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1145	850	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1145	865	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1145	875	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	1145	920	0	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1145	890	4	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	113	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
115	0	0	0
_
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
1180	830	1280	1004
1	0
parameter
Widget Tag	5	_	_	screenshot_jpg
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1180	935	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1180	945	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1180	955	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1180	965	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1180	980	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	1180	995	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1280	935	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1280	945	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1280	955	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1280	965	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1180	840	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1180	850	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1180	865	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1180	875	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1180	920	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	1	1280	840	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1280	850	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1280	865	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1280	875	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1280	920	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
finishport

3	3	1280	890	0	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	73	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
117	0	0	0
_
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
1315	835	1415	1009
1	0
parameter
Widget Tag	5	_	_	screenshot_gif
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1315	940	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1315	950	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1315	960	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1315	970	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1315	985	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	1315	1000	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1415	940	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1415	950	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1415	960	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1415	970	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1315	845	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1315	855	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1315	870	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1315	880	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1315	925	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	1	1415	845	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1415	855	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1415	870	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1415	880	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1415	925	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	1	4	3	-1	0
END_LINE
finishport

3	3	1415	895	0	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	73	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
138	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
630	1160	730	1205
1	0
parameter
Integer Value	1	-2147483648	2147483647	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	730	1195	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	118	2	1	-1	0
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
159	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
755	940	855	985
1	0
parameter
Integer Value	1	-2147483648	2147483647	100
EndOfValues
EndOfLabels
integer constant

outputport

1	1	855	970	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_viewport
_
160	0	0	0
_
_
GUI window position, size, colour and transparency. Edit parameters in IGB.
gui_viewport
_
680	1555	780	1720
1	0
parameter
Widget Tag	5	_	_	viewport1
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	680	1710	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	680	1720	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	680	1730	0	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	680	1740	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	680	1755	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	780	1710	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	780	1720	1	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	780	1730	2	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	780	1740	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	780	1635	6	0	1	mandatory= 0	0	1
abs.x

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	780	1645	7	0	1	mandatory= 0	0	1
abs.y

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	780	1670	4	0	1	mandatory= 0	0	1
off.x

create	7	EndOfFunc
BEGIN_LINE
0	277	1	1	-1	0
END_LINE
outputport

1	1	780	1680	5	0	1	mandatory= 0	0	1
off.y

create	8	EndOfFunc
BEGIN_LINE
0	381	0	1	-1	0
END_LINE
startport

2	-1	680	1565	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	680	1575	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	680	1590	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	680	1600	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	680	1695	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	780	1565	0	0	1	mandatory= 0	0	1
--

create	9	EndOfFunc
BEGIN_LINE
0	118	4	3	-1	0
END_LINE
finishport

3	1	780	1575	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	1590	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	1600	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	1695	4	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	1615	5	0	1	mandatory= 0	0	1
mouse down

create	10	EndOfFunc
BEGIN_LINE
0	381	0	3	-1	0
END_LINE
finishport

3	1	780	1625	6	0	1	mandatory= 0	0	1
mouse up

create	11	EndOfFunc
BEGIN_LINE
0	274	1	3	-1	0
END_LINE
finishport

3	1	780	1660	7	0	1	mandatory= 0	0	1
mouse drag

create	12	EndOfFunc
BEGIN_LINE
0	118	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
161	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
1435	1570	1485	1615
1	0
outputport

1	1	1485	1600	1	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	17	5	1	-1	0
END_LINE
startport

2	-1	1435	1580	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	160	5	2	-1	0
1370	1615
1370	1580
END_LINE
startport

2	-1	1435	1600	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1485	1580	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	17	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
183	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
1525	1615	1575	1660
1	0
outputport

1	1	1575	1645	2	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	26	5	1	-1	0
END_LINE
startport

2	-1	1525	1625	1	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	532	0	2	-1	0
1565	1488
1565	1555
1510	1555
1510	1625
END_LINE
startport

2	-1	1525	1645	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1575	1625	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	26	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_counter
_
185	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
1600	1665	1650	1710
1	0
outputport

1	1	1650	1695	3	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	28	5	1	-1	0
END_LINE
startport

2	-1	1600	1675	1	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	214	0	2	-1	0
1595	1440
1595	1675
END_LINE
startport

2	-1	1600	1695	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	1650	1675	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	28	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
186	0	0	0
_
_
Creates periodic timed events.
time_clock
_
540	1065	600	1155
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

0	1	540	1110	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	540	1125	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	540	1140	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	540	1075	0	1	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	540	1090	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	600	1105	6	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	211	0	3	-1	0
END_LINE
finishport

3	0	600	1075	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	600	1090	2	0	1	mandatory= 0	0	1


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

trigger_counter
_
211	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
640	1100	690	1145
1	0
outputport

1	1	690	1130	1	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	212	0	1	-1	0
END_LINE
startport

2	-1	640	1110	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	186	0	2	-1	0
635	1105
635	1110
END_LINE
startport

2	-1	640	1130	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	690	1110	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	212	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_mul2ix
_
212	0	0	0
_
_
This is a 2-input integer multiplier.
MultiplyTwoInputInt
_
755	1115	805	1175
1	0
inputport

0	1	755	1145	1	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	211	0	0	-1	0
745	1130
745	1145
END_LINE
inputport

0	1	755	1160	2	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	138	0	0	-1	0
740	1195
740	1160
END_LINE
outputport

1	1	805	1145	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	118	0	1	-1	0
END_LINE
startport

2	1	755	1125	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	211	0	2	-1	0
750	1110
750	1125
END_LINE
finishport

3	1	805	1125	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	118	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventand2
_
214	0	0	0
_
_
This is a 2-input event latching AND with an external reset. Once triggered by receiving all inputs, will trigger on each input received until reset.
EventTwoInputLatchingAnd
_
1485	1420	1500	1466
1	0
startport

2	-1	1485	1430	4	0	1	mandatory= 0	0	1


And1	0	EndOfFunc
BEGIN_LINE
1	275	6	2	-1	0
1480	1420
1480	1430
END_LINE
startport

2	-1	1485	1445	0	0	1	mandatory= 0	0	1


And2	0	EndOfFunc
BEGIN_LINE
1	160	7	2	-1	0
1400	1660
1400	1445
END_LINE
startport

2	-1	1485	1460	0	0	1	mandatory= 0	0	1
R

Reset	0	EndOfFunc
BEGIN_LINE
1	160	6	2	-1	0
1410	1625
1410	1460
END_LINE
finishport

3	-1	1500	1440	0	0	1	mandatory= 0	0	1


And1	1	And2	1	EndOfFunc
BEGIN_LINE
0	532	0	3	-1	0
END_LINE
finishport

3	-1	1500	1460	1	0	1	mandatory= 0	0	1


Reset	1	EndOfFunc
BEGIN_LINE
0	381	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_patch
_
275	0	0	0
_
_
This widget displays a rectangular patch on the  GUI output.
gui_patch
_
1290	1350	1390	1515
1	0
parameter
Widget Tag	5	_	_	widget13
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

0	1	1290	1455	1	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
1	277	0	0	-1	0
1280	1380
1280	1455
END_LINE
inputport

0	1	1290	1465	0	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
1	417	0	0	-1	0
1270	1470
1270	1465
END_LINE
inputport

0	1	1290	1475	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1290	1485	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1290	1500	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	1390	1455	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1390	1465	1	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1390	1475	2	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1390	1485	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1290	1360	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1290	1370	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1290	1385	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1290	1395	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1290	1440	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	454	0	2	-1	0
1285	1431
1285	1440
END_LINE
finishport

3	1	1390	1360	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1390	1370	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1390	1385	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1390	1395	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1390	1440	4	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1390	1410	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	1390	1420	0	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	118	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_addacci
_
276	0	0	0
_
_
This is an integer add accumulator.
AddAccumulateInt
_
1045	1350	1095	1395
1	0
inputport

0	1	1045	1380	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	1	EndOfFunc
BEGIN_LINE
1	346	0	0	-1	0
1035	1395
1035	1380
END_LINE
outputport

1	1	1095	1380	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	2	EndOfFunc
BEGIN_LINE
0	277	0	1	-1	0
END_LINE
startport

2	1	1045	1360	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	0	EndOfFunc
BEGIN_LINE
1	346	0	2	-1	0
1040	1375
1040	1360
END_LINE
finishport

3	1	1095	1360	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	3	EndOfFunc
BEGIN_LINE
0	492	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_add2ix
_
277	0	0	0
_
_
This is a 2-input integer adder.
AdditionTwoInputInt
_
1175	1350	1225	1410
1	0
inputport

0	1	1175	1380	0	0	1	mandatory= 1	0	1


Run_AdditionTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	276	0	0	-1	0
1125	1380
1125	1380
END_LINE
inputport

0	1	1175	1395	1	0	1	mandatory= 1	0	1


Run_AdditionTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	160	6	0	-1	0
1130	1670
1130	1395
END_LINE
outputport

1	1	1225	1380	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	118	0	1	-1	0
END_LINE
startport

2	-1	1175	1360	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	492	0	2	-1	0
1170	1351
1170	1360
END_LINE
finishport

3	1	1225	1360	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	454	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
311	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
800	1400	900	1445
1	0
parameter
Integer Value	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	900	1435	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	346	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2i
_
346	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
_
950	1365	1000	1440
1	0
inputport

0	1	950	1390	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	160	6	0	-1	0
910	1670
910	1390
END_LINE
inputport

0	1	950	1425	0	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	311	0	0	-1	0
930	1435
930	1425
END_LINE
outputport

1	1	1000	1395	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	276	0	1	-1	0
END_LINE
startport

2	1	950	1375	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	532	0	2	-1	0
1565	1488
1565	1290
930	1290
930	1375
END_LINE
startport

2	1	950	1410	0	1	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
0	214	1	2	-1	0
END_LINE
finishport

3	1	1000	1375	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	276	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_addacci
_
379	0	0	0
_
_
This is an integer add accumulator.
AddAccumulateInt
_
1045	1440	1095	1485
1	0
inputport

0	1	1045	1470	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	1	EndOfFunc
BEGIN_LINE
1	381	0	0	-1	0
1035	1510
1035	1470
END_LINE
outputport

1	1	1095	1470	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	2	EndOfFunc
BEGIN_LINE
0	417	0	1	-1	0
END_LINE
startport

2	1	1045	1450	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	0	EndOfFunc
BEGIN_LINE
1	381	0	2	-1	0
1025	1490
1025	1450
END_LINE
finishport

3	1	1095	1450	0	0	1	mandatory= 0	0	1


Run_AddAccumulateInt	3	EndOfFunc
BEGIN_LINE
0	531	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
380	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
800	1515	900	1560
1	0
parameter
Integer Value	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	900	1550	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	346	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2i
_
381	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
_
950	1480	1000	1555
1	0
inputport

0	1	950	1505	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	160	7	0	-1	0
940	1680
940	1505
END_LINE
inputport

0	1	950	1540	0	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	380	0	0	-1	0
930	1550
930	1540
END_LINE
outputport

1	1	1000	1510	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	276	0	1	-1	0
END_LINE
startport

2	1	950	1490	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	532	0	2	-1	0
1575	1488
1575	1280
920	1280
920	1490
END_LINE
startport

2	1	950	1525	0	1	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
0	214	1	2	-1	0
END_LINE
finishport

3	1	1000	1490	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	276	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_add2ix
_
417	0	0	0
_
_
This is a 2-input integer adder.
AdditionTwoInputInt
_
1170	1440	1220	1500
1	0
inputport

0	1	1170	1470	0	0	1	mandatory= 1	0	1


Run_AdditionTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	379	0	0	-1	0
1110	1470
1110	1470
END_LINE
inputport

0	1	1170	1485	1	0	1	mandatory= 1	0	1


Run_AdditionTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	160	7	0	-1	0
1115	1680
1115	1485
END_LINE
outputport

1	1	1220	1470	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	275	1	1	-1	0
END_LINE
startport

2	-1	1170	1450	1	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	531	0	2	-1	0
1165	1441
1165	1450
END_LINE
finishport

3	1	1220	1450	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	454	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
454	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
1240	1410	1260	1450
1	0
startport

2	1	1240	1420	1	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	277	0	2	-1	0
1235	1360
1235	1420
END_LINE
startport

2	-1	1240	1440	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	1260	1431	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	275	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
492	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
1135	1330	1155	1370
1	0
startport

2	-1	1135	1340	4	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	214	0	2	-1	0
1515	1440
1515	1325
1120	1325
1120	1340
END_LINE
startport

2	-1	1135	1360	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	1155	1351	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	277	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
531	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
_
1125	1420	1145	1460
1	0
startport

2	-1	1125	1430	4	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	214	0	2	-1	0
1525	1440
1525	1320
1110	1320
1110	1430
END_LINE
startport

2	-1	1125	1450	4	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	1145	1441	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	417	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
532	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
_
1535	1470	1550	1505
1	0
startport

2	-1	1535	1480	0	0	1	mandatory= 0	0	1


And1	0	EndOfFunc
BEGIN_LINE
1	214	0	2	-1	0
1530	1440
1530	1480
END_LINE
startport

2	-1	1535	1495	2	0	1	mandatory= 0	0	1


And2	0	EndOfFunc
BEGIN_LINE
1	214	1	2	-1	0
1515	1460
1515	1495
END_LINE
finishport

3	-1	1550	1488	0	0	1	mandatory= 0	0	1


And1	1	And2	1	EndOfFunc
BEGIN_LINE
0	381	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
