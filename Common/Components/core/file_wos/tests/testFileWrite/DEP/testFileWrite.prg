CanvasSizeXY
2000	2180

IconData
BEGIN_BLOCK

file_wos
_
1	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii fo
FILE_WriteOnly_String
_
765	530	865	630
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0
EndOfValues
EndOfLabels
Data Field Width (space padded)

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	765	550	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	765	615	0	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
600	615
600	615
END_LINE
outputport

1	1	865	615	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_String	2	Run_CloseFILE_WriteOnly_String	1	Run_OpenFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	765	540	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	765	565	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	765	590	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	3	2	2	-1	0
585	590
585	590
END_LINE
finishport

3	-1	865	540	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	865	565	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	590	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	550	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	575	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	600	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
3	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_String
_
190	530	290	640
1	0
parameter
File name	3	_	_	deleteMe2/out1.txt
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

inputport

0	3	190	550	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_String	1	EndOfFunc
BEGIN_LINE
0	58	0	0	-1	0
END_LINE
outputport

1	3	290	615	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_String	1	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
outputport

1	1	290	625	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_String	2	Run_CloseFILE_ReadOnly_String	1	Run_OpenFILE_ReadOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	190	540	0	1	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
0	58	0	2	-1	0
END_LINE
startport

2	-1	190	565	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	190	590	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
1	4	5	2	-1	0
490	420
490	535
175	535
175	590
END_LINE
finishport

3	-1	290	540	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	33	2	3	-1	0
END_LINE
finishport

3	-1	290	565	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	290	590	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
finishport

3	1	290	550	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	290	575	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	290	600	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
4	0	0	0
output
_
This widget displays an string to the GUI output.
gui_text_string2
_
375	360	475	534
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

inputport

0	1	375	465	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	475	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	485	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	495	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	510	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	375	525	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
325	615
325	525
END_LINE
outputport

1	1	475	465	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	475	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	485	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	495	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	375	370	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	380	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	395	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	405	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	375	450	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	3	2	2	-1	0
340	590
340	450
END_LINE
finishport

3	1	475	370	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	380	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	395	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	405	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	450	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
finishport

3	1	475	420	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	3	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
9	0	0	0
close
_
This widget displays an string to the GUI output.
gui_text_string2
_
560	360	660	534
1	0
parameter
Widget Tag	5	_	_	close
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	560	465	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	560	475	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	560	485	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	560	495	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	560	510	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	560	525	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
0	3	0	0	-1	0
END_LINE
outputport

1	1	660	465	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	660	475	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	660	485	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	660	495	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	560	370	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	560	380	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	560	395	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	560	405	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	560	450	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	3	2	2	-1	0
END_LINE
finishport

3	1	660	370	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	660	380	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	660	395	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	660	405	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	660	450	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
finishport

3	1	660	420	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
15	0	0	0
fileA
_
This widget displays an string to the GUI output.
gui_text_string2
_
335	650	435	824
1	0
parameter
Widget Tag	5	_	_	fileA
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	335	755	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	335	765	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	335	775	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	335	785	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	335	800	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	335	815	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	27	0	0	-1	0
275	775
275	815
END_LINE
outputport

1	1	435	755	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	435	765	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	435	775	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	435	785	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	335	660	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	335	670	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	335	685	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	335	695	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	335	740	0	1	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	3	2	2	-1	0
END_LINE
finishport

3	1	435	660	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	670	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	685	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	695	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	740	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
finishport

3	1	435	710	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	16	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3s
_
16	0	0	0
_
_
This is a 3-input string multiplexer.
MultiplexThreeInputString
_
590	655	640	765
1	0
inputport

0	3	590	680	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	27	0	0	-1	0
500	775
500	680
END_LINE
inputport

0	3	590	715	2	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	28	0	0	-1	0
535	980
535	715
END_LINE
inputport

0	3	590	750	1	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	640	685	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
startport

2	3	590	665	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	15	5	2	-1	0
495	710
495	665
END_LINE
startport

2	3	590	700	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	26	5	2	-1	0
530	915
530	700
END_LINE
startport

2	3	590	735	2	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	640	665	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
26	0	0	0
fileB
_
This widget displays an string to the GUI output.
gui_text_string2
_
330	855	430	1029
1	0
parameter
Widget Tag	5	_	_	fileB
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	330	960	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	330	970	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	330	980	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	330	990	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	330	1005	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	330	1020	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	28	0	0	-1	0
270	980
270	1020
END_LINE
outputport

1	1	430	960	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	430	970	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	430	980	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	430	990	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	330	865	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	330	875	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	330	890	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	330	900	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	330	945	0	1	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	3	2	2	-1	0
END_LINE
finishport

3	1	430	865	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	430	875	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	430	890	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	430	900	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	430	945	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	33	0	3	-1	0
END_LINE
finishport

3	1	430	915	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	16	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
27	0	0	0
_
_
This is a string constant.
Constant_String1
_
170	745	270	790
1	0
parameter
String Constant	3	_	_	out1.txt
EndOfValues
EndOfLabels
string constant

outputport

1	3	270	775	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	16	0	1	-1	0
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
28	0	0	0
_
_
This is a string constant.
Constant_String1
_
160	950	260	995
1	0
parameter
String Constant	3	_	_	out2.txt
EndOfValues
EndOfLabels
string constant

outputport

1	3	260	980	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	16	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wob
_
29	0	0	0
_
_
Writes boolean data to a file. If a field separator parameter is specified then then this will be used inserted between data points. Fixed width is not available for this data type. The append option specified if the file is truncated to an empty file whe
FILE_WriteOnly_Bool
_
775	250	875	350
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Delimiter	3	_	_	()
EndOfValues
EndOfLabels
Boolean Field Delimiter Character

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	775	270	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_WriteOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	775	335	1	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_Bool	1	EndOfFunc
BEGIN_LINE
1	34	0	0	-1	0
755	335
755	335
END_LINE
outputport

1	1	875	335	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_Bool	2	Run_CloseFILE_WriteOnly_Bool	1	Run_OpenFILE_WriteOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	775	260	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	775	285	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	775	310	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_Bool	0	EndOfFunc
BEGIN_LINE
1	34	2	2	-1	0
750	310
750	310
END_LINE
finishport

3	3	875	260	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	875	285	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	310	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	270	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	295	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	320	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_woi
_
30	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted after data points. If a Fixed width parameter is specified then this specifies the length of the ascii stri
FILE_WriteOnly_Int
_
765	405	865	505
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
Integer Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Integer Field Delimiter Character

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	765	425	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_WriteOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	765	490	1	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	865	490	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_Int	2	Run_CloseFILE_WriteOnly_Int	1	Run_OpenFILE_WriteOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	765	415	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	765	440	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	765	465	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	865	415	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	865	440	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	465	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	425	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	450	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	865	475	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wor
_
31	0	0	0
_
_
Writes real data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii form
FILE_WriteOnly_Real
_
775	135	875	235
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
Integer Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Real Field Delimiter Character

parameter
Precision	1	0	100	3
EndOfValues
EndOfLabels
Integer Field Width

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	775	155	1	0	1	mandatory= 0	0	1
path

Run_OpenFILE_WriteOnly_Real	1	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
690	685
690	155
END_LINE
inputport

0	2	775	220	1	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_Real	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
755	220
755	220
END_LINE
outputport

1	1	875	220	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_Real	2	Run_CloseFILE_WriteOnly_Real	1	Run_OpenFILE_WriteOnly_Real	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	775	145	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_Real	0	EndOfFunc
BEGIN_LINE
1	16	0	2	-1	0
680	665
680	145
END_LINE
startport

2	-1	775	170	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_Real	0	EndOfFunc
BEGIN_LINE
1	9	5	2	-1	0
730	420
730	170
END_LINE
startport

2	3	775	195	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_Real	0	EndOfFunc
BEGIN_LINE
1	32	2	2	-1	0
750	195
750	195
END_LINE
finishport

3	3	875	145	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	875	170	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_Real	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	195	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	155	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	180	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	875	205	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ror
_
32	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Real
_
200	135	300	245
1	0
parameter
File name	3	_	_	deleteMe2R/out1.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
Real Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Real Field Delimiter Character

inputport

0	3	200	155	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	2	300	220	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	35	5	1	-1	0
END_LINE
outputport

1	1	300	230	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Real	2	Run_OpenFILE_ReadOnly_Real	2	Run_CloseFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	200	145	0	1	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	200	170	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	200	195	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
1	35	5	2	-1	0
500	70
500	-15
175	-15
175	195
END_LINE
finishport

3	3	300	145	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	300	170	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Real	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	300	195	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	35	4	3	-1	0
END_LINE
finishport

3	1	300	155	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	300	180	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	300	205	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_roi
_
33	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Int
_
195	405	295	515
1	0
parameter
File name	3	_	_	out.txt
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

inputport

0	3	195	425	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	295	490	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	295	500	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Int	2	Run_CloseFILE_ReadOnly_Int	1	Run_OpenFILE_ReadOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	195	415	0	1	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	195	440	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	195	465	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	295	415	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	295	440	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	465	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	425	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	450	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	295	475	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_rob
_
34	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Bool
_
200	250	300	360
1	0
parameter
File name	3	_	_	deleteMe2B/out1.txt
EndOfValues
EndOfLabels
output file name

parameter
Delimiter	3	_	_	()
EndOfValues
EndOfLabels
Boolean Field Delimiter Character

inputport

0	3	200	270	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_ReadOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	300	335	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	29	1	1	-1	0
END_LINE
outputport

1	1	300	345	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Bool	2	Run_CloseFILE_ReadOnly_Bool	1	Run_OpenFILE_ReadOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	200	260	0	1	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	200	285	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	200	310	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	300	260	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	300	285	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	300	310	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	29	2	3	-1	0
END_LINE
finishport

3	1	300	270	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	300	295	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	300	320	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
35	0	0	0
outputF
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
375	10	475	184
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

0	1	375	115	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	125	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	135	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	145	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	375	160	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	375	175	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
315	220
315	175
END_LINE
outputport

1	1	475	115	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	125	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	135	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	475	145	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	375	20	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	30	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	45	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	55	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	375	100	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	32	2	2	-1	0
330	195
330	100
END_LINE
finishport

3	1	475	20	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	30	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	45	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	55	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	100	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	475	70	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	32	2	3	-1	0
END_LINE
finishport

3	1	475	80	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
