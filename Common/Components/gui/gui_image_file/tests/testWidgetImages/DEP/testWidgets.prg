CanvasSizeXY
2270	2650

IconData
BEGIN_BLOCK

gui_viewport
_
4	0	0	0
viewport
_
GUI window position, size, colour and transparency. Edit parameters in IGB.
gui_viewport
_
755	585	855	750
1	0
parameter
Widget Tag	5	_	_	viewport
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	755	690	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	700	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	710	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	720	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	755	735	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	855	690	0	0	1	mandatory= 0	0	1
abs x

create	1	update	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	855	700	2	0	1	mandatory= 0	0	1
abs y

create	2	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	855	710	1	0	1	mandatory= 0	0	1
wid

create	3	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	855	720	3	0	1	mandatory= 0	0	1
ht

create	4	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	755	595	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	605	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	620	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	630	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	755	675	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	1	855	595	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	605	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	620	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	630	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	855	675	5	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	855	645	4	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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
1400	755	1500	929
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

inputport

0	1	1400	845	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1400	860	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1400	875	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1400	890	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1400	905	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1400	920	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	0	0	-1	0
1340	960
1340	920
END_LINE
outputport

1	1	1500	845	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1500	860	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1500	875	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1500	890	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1400	765	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1400	780	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1400	795	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1400	810	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1400	825	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	4	2	-1	0
1355	940
1355	825
END_LINE
finishport

3	1	1500	765	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1500	780	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1500	795	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1500	810	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1500	825	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1500	835	5	0	1	mandatory= 0	0	1
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
1535	820	1635	994
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

inputport

0	1	1535	910	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1535	925	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1535	940	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1535	955	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1535	970	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1535	985	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	1	0	-1	0
1475	970
1475	985
END_LINE
outputport

1	1	1635	910	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1635	925	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1635	940	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1635	955	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1535	830	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1535	845	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1535	860	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1535	875	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1535	890	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	4	2	-1	0
1490	940
1490	890
END_LINE
finishport

3	1	1635	830	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1635	845	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1635	860	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1635	875	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1635	890	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1635	900	5	0	1	mandatory= 0	0	1
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
1695	875	1795	1049
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

inputport

0	1	1695	965	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1695	980	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1695	995	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1695	1010	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1695	1025	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1695	1040	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	2	0	-1	0
1635	980
1635	1040
END_LINE
outputport

1	1	1795	965	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1795	980	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1795	995	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1795	1010	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1695	885	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1695	900	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1695	915	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1695	930	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1695	945	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	4	2	-1	0
1650	940
1650	945
END_LINE
finishport

3	1	1795	885	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1795	900	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1795	915	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1795	930	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1795	945	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1795	955	5	0	1	mandatory= 0	0	1
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
1845	955	1945	1129
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

inputport

0	1	1845	1045	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1845	1060	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1845	1075	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1845	1090	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1845	1105	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1845	1120	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	31	3	0	-1	0
1785	990
1785	1120
END_LINE
outputport

1	1	1945	1045	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1945	1060	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1945	1075	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1945	1090	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1845	965	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1845	980	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1845	995	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1845	1010	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1845	1025	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	31	4	2	-1	0
1800	940
1800	1025
END_LINE
finishport

3	1	1945	965	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1945	980	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1945	995	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1945	1010	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1945	1025	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1945	1035	5	0	1	mandatory= 0	0	1
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
400	925	500	970
1	0
parameter
Integer Value	1	-2147483648	2147483647	-20
EndOfValues
EndOfLabels
integer constant

outputport

1	1	500	955	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	31	3	1	-1	0
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
760	840	860	1015
1	0
parameter
Widget Tag	5	_	_	widget7
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	760	960	3	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
1	69	0	0	-1	0
700	925
700	960
END_LINE
inputport

0	1	760	970	0	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	760	980	1	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
1	108	0	0	-1	0
730	1000
730	980
END_LINE
inputport

0	1	760	990	1	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
1	108	0	0	-1	0
720	1000
720	990
END_LINE
inputport

0	1	760	1005	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	760	860	5	0	1	mandatory= 0	0	1
file

create	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
700	850
700	860
END_LINE
outputport

1	1	860	960	0	0	1	mandatory= 0	0	1
abs x

create	2	update	6	EndOfFunc
BEGIN_LINE
0	29	0	1	-1	0
END_LINE
outputport

1	1	860	970	1	0	1	mandatory= 0	0	1
abs y

create	3	update	7	EndOfFunc
BEGIN_LINE
0	17	5	1	-1	0
END_LINE
outputport

1	1	860	980	2	0	1	mandatory= 0	0	1
wid

create	4	update	8	EndOfFunc
BEGIN_LINE
0	26	5	1	-1	0
END_LINE
outputport

1	1	860	990	3	0	1	mandatory= 0	0	1
ht

create	5	update	9	EndOfFunc
BEGIN_LINE
0	28	5	1	-1	0
END_LINE
startport

2	-1	760	850	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	875	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	890	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	760	900	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	760	945	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	69	0	2	-1	0
715	905
715	945
END_LINE
finishport

3	3	860	850	0	0	1	mandatory= 0	0	1
--

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	860	875	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	860	890	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	860	900	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	860	940	5	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
finishport

3	1	860	915	4	0	1	mandatory= 0	0	1
click

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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
560	820	660	865
1	0
parameter
String Constant	3	_	_	screenshot.gif
EndOfValues
EndOfLabels
string constant

outputport

1	3	660	850	0	0	1	mandatory= 0	0	1


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

gui_image_file
_
65	0	0	0
_
_
Displays an image based on a filename. Use create to load new filenames dynamically.
GUI_Image_File
_
780	1150	880	1325
1	0
parameter
Widget Tag	5	_	_	widget8
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	780	1270	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	780	1280	3	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	780	1290	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	780	1300	1	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	780	1315	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	780	1170	5	0	1	mandatory= 0	0	1
file

create	1	EndOfFunc
BEGIN_LINE
1	66	0	0	-1	0
720	1190
720	1170
END_LINE
outputport

1	1	880	1270	0	0	1	mandatory= 0	0	1
abs x

create	2	update	6	EndOfFunc
BEGIN_LINE
0	29	0	1	-1	0
END_LINE
outputport

1	1	880	1280	1	0	1	mandatory= 0	0	1
abs y

create	3	update	7	EndOfFunc
BEGIN_LINE
0	17	5	1	-1	0
END_LINE
outputport

1	1	880	1290	2	0	1	mandatory= 0	0	1
wid

create	4	update	8	EndOfFunc
BEGIN_LINE
0	26	5	1	-1	0
END_LINE
outputport

1	1	880	1300	3	0	1	mandatory= 0	0	1
ht

create	5	update	9	EndOfFunc
BEGIN_LINE
0	28	5	1	-1	0
END_LINE
startport

2	-1	780	1160	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	780	1185	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	780	1200	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	780	1210	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	780	1255	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	1	5	2	-1	0
END_LINE
finishport

3	3	880	1160	0	0	1	mandatory= 0	0	1
--

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	880	1185	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	880	1200	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	880	1210	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	880	1250	5	0	1	mandatory= 0	0	1
--

update	10	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
finishport

3	1	880	1225	4	0	1	mandatory= 0	0	1
click

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
66	0	0	0
_
_
This is a string constant.
Constant_String1
_
575	1160	675	1205
1	0
parameter
String Constant	3	_	_	screenshot.gif
EndOfValues
EndOfLabels
string constant

outputport

1	3	675	1190	0	0	1	mandatory= 0	0	1


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

time_clock
_
67	0	0	0
_
_
Creates periodic timed events.
time_clock
_
410	715	470	805
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	50000
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

0	1	410	760	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	410	775	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	410	790	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	410	725	4	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	89	5	2	-1	0
405	700
405	725
END_LINE
startport

2	0	410	740	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	470	755	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
finishport

3	0	470	725	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	470	740	2	0	1	mandatory= 0	0	1


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
68	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
500	860	550	905
1	0
outputport

1	1	550	890	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	108	0	1	-1	0
END_LINE
startport

2	-1	500	870	0	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	67	0	2	-1	0
475	755
475	870
END_LINE
startport

2	-1	500	890	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	550	870	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	108	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_mul2ix
_
69	0	0	0
_
_
This is a 2-input integer multiplier.
MultiplyTwoInputInt
_
630	895	680	955
1	0
inputport

0	1	630	925	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	68	0	0	-1	0
570	890
570	925
END_LINE
inputport

0	1	630	940	2	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	30	0	0	-1	0
550	955
550	940
END_LINE
outputport

1	1	680	925	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	31	1	1	-1	0
END_LINE
startport

2	-1	630	905	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	68	0	2	-1	0
570	870
570	905
END_LINE
finishport

3	1	680	905	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
87	0	0	0
_
_
This is a string constant.
Constant_String1
_
60	745	160	790
1	0
parameter
String Constant	3	_	_	click me
EndOfValues
EndOfLabels
string constant

outputport

1	3	160	775	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	89	5	1	-1	0
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
89	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
_
225	640	325	814
1	0
parameter
Widget Tag	5	_	_	widget9
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	225	745	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	225	755	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	225	765	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	225	775	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	225	790	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	225	805	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	87	0	0	-1	0
165	775
165	805
END_LINE
outputport

1	1	325	745	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	325	755	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	325	765	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	325	775	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	225	650	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	225	660	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	225	675	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	225	685	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	225	730	4	1	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	325	650	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	325	660	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	325	675	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	325	685	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	325	730	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	325	700	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	67	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_mul2ix
_
108	0	0	0
_
_
This is a 2-input integer multiplier.
MultiplyTwoInputInt
_
615	970	665	1030
1	0
inputport

0	1	615	1000	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	68	0	0	-1	0
605	890
605	1000
END_LINE
inputport

0	1	615	1015	2	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	128	0	0	-1	0
595	1020
595	1015
END_LINE
outputport

1	1	665	1000	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	31	3	1	-1	0
END_LINE
startport

2	-1	615	980	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	68	0	2	-1	0
610	870
610	980
END_LINE
finishport

3	1	665	980	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
128	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
410	990	510	1035
1	0
parameter
Integer Value	1	-2147483648	2147483647	-5
EndOfValues
EndOfLabels
integer constant

outputport

1	1	510	1020	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	108	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
147	0	0	0
_
_
This is a string constant.
Constant_String1
_
560	770	660	815
1	0
parameter
String Constant	3	_	_	screenshot.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	660	800	0	0	1	mandatory= 0	0	1


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
167	0	0	0
_
_
This is a string constant.
Constant_String1
_
560	720	660	765
1	0
parameter
String Constant	3	_	_	screenshot.jpg
EndOfValues
EndOfLabels
string constant

outputport

1	3	660	750	0	0	1	mandatory= 0	0	1


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
188	0	0	0
_
_
This is a string constant.
Constant_String1
_
575	1110	675	1155
1	0
parameter
String Constant	3	_	_	screenshot.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	675	1140	0	0	1	mandatory= 0	0	1


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
189	0	0	0
_
_
This is a string constant.
Constant_String1
_
575	1060	675	1105
1	0
parameter
String Constant	3	_	_	screenshot.jpg
EndOfValues
EndOfLabels
string constant

outputport

1	3	675	1090	0	0	1	mandatory= 0	0	1


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
END_OF_BLOCKS
