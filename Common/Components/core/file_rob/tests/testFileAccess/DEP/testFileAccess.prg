CanvasSizeXY
2070	4350

IconData
BEGIN_BLOCK

file_wos
_
3	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii fo
FILE_WriteOnly_String
_
345	1905	445	2005
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
Data Field Width (space padded)

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	1
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	345	1990	0	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
1	30	0	0	-1	0
295	1970
295	1990
END_LINE
outputport

1	1	445	1990	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_String	2	Run_CloseFILE_WriteOnly_String	1	Run_OpenFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	345	1915	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	345	1940	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	345	1965	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	445	1915	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	445	1940	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	445	1965	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	445	1925	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	445	1950	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	445	1975	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
7	0	0	0
output
_
This widget displays an string to the GUI output.
gui_text_string2
_
735	2500	835	2674
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

0	1	735	2605	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	2615	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	2625	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	2635	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	735	2650	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	735	2665	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	39	0	0	-1	0
675	2480
675	2665
END_LINE
outputport

1	1	835	2605	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	2615	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	2625	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	835	2635	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	735	2510	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	2520	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	2535	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	2545	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	735	2590	3	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	835	2510	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	2520	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	2535	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	2545	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	2590	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	835	2560	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
8	0	0	0
clickMe
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
730	2290	830	2464
1	0
parameter
Widget Tag	5	_	_	read
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	730	2395	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2405	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2415	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2425	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2440	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2455	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	90	0	0	-1	0
670	2310
670	2455
END_LINE
outputport

1	1	830	2395	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	830	2405	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	830	2415	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	830	2425	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	730	2300	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2310	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2325	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2335	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2380	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	90	0	2	-1	0
685	2290
685	2380
END_LINE
finishport

3	1	830	2300	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2310	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2325	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2335	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2380	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2350	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
19	0	0	0
clickMe
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
130	2460	230	2634
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

0	1	130	2565	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	130	2575	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	130	2585	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	130	2595	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	130	2610	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	130	2625	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	88	0	0	-1	0
655	2395
655	2670
70	2670
70	2625
END_LINE
outputport

1	1	230	2565	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	230	2575	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	230	2585	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	230	2595	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	130	2470	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	130	2480	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	130	2495	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	130	2505	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	130	2550	2	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	88	0	2	-1	0
640	2375
640	2655
85	2655
85	2550
END_LINE
finishport

3	1	230	2470	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	2480	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	2495	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	2505	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	2550	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	230	2520	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	3	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
30	0	0	0
_
_
This is a string constant.
Constant_String1
_
140	1940	240	1985
1	0
parameter
String Constant	3	_	_	aa
EndOfValues
EndOfLabels
string constant

outputport

1	3	240	1970	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_roi
_
31	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Int
_
340	2545	440	2655
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	3
EndOfValues
EndOfLabels
Integer Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Integer Field Delimiter Character

outputport

1	1	440	2630	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Int	1	EndOfFunc
BEGIN_LINE
0	38	0	1	-1	0
END_LINE
outputport

1	1	440	2640	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Int	2	Run_CloseFILE_ReadOnly_Int	1	Run_OpenFILE_ReadOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	340	2555	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	340	2580	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	340	2605	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	440	2555	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	440	2580	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2605	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2565	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2590	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2615	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Int	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wor
_
32	0	0	0
_
_
Writes real data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii form
FILE_WriteOnly_Real
_
340	2205	440	2305
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	8
EndOfValues
EndOfLabels
Integer Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Real Field Delimiter Character

parameter
Precision	1	0	100	4
EndOfValues
EndOfLabels
Integer Field Width

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	2	340	2290	0	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_Real	1	EndOfFunc
BEGIN_LINE
1	92	0	0	-1	0
330	2295
330	2290
END_LINE
outputport

1	1	440	2290	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_Real	2	Run_CloseFILE_WriteOnly_Real	1	Run_OpenFILE_WriteOnly_Real	1	EndOfFunc
BEGIN_LINE
0	90	1	1	-1	0
END_LINE
startport

2	-1	340	2215	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_Real	0	EndOfFunc
BEGIN_LINE
1	60	5	2	-1	0
845	2155
845	2490
325	2490
325	2215
END_LINE
startport

2	-1	340	2240	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_Real	0	EndOfFunc
BEGIN_LINE
1	19	5	2	-1	0
270	2520
270	2240
END_LINE
startport

2	3	340	2265	1	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_Real	0	EndOfFunc
BEGIN_LINE
1	8	5	2	-1	0
845	2350
845	2475
325	2475
325	2265
END_LINE
finishport

3	1	440	2215	3	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_Real	2	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
finishport

3	1	440	2240	4	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_Real	2	EndOfFunc
BEGIN_LINE
0	88	0	3	-1	0
END_LINE
finishport

3	1	440	2265	0	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_Real	3	EndOfFunc
BEGIN_LINE
0	94	4	3	-1	0
END_LINE
finishport

3	-1	440	2225	0	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_Real	3	EndOfFunc
BEGIN_LINE
0	61	1	3	-1	0
END_LINE
finishport

3	1	440	2250	5	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_Real	3	EndOfFunc
BEGIN_LINE
0	88	1	3	-1	0
END_LINE
finishport

3	-1	440	2275	1	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_Real	4	EndOfFunc
BEGIN_LINE
0	90	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_woi
_
33	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted after data points. If a Fixed width parameter is specified then this specifies the length of the ascii stri
FILE_WriteOnly_Int
_
340	2325	440	2425
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

0	1	340	2410	0	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_Int	1	EndOfFunc
BEGIN_LINE
1	93	0	0	-1	0
330	2390
330	2410
END_LINE
outputport

1	1	440	2410	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_Int	2	Run_CloseFILE_WriteOnly_Int	1	Run_OpenFILE_WriteOnly_Int	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	340	2335	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	340	2360	2	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	340	2385	1	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_Int	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	440	2335	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	440	2360	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_Int	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2385	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2345	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2370	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_Int	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	440	2395	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_Int	4	EndOfFunc
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
680	2770	780	2880
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Boolean Field Delimiter Character

outputport

1	0	780	2855	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	37	0	1	-1	0
END_LINE
outputport

1	1	780	2865	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Bool	2	Run_CloseFILE_ReadOnly_Bool	1	Run_OpenFILE_ReadOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	680	2780	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	680	2805	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	680	2830	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	780	2780	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	780	2805	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	2830	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	2790	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	2815	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	780	2840	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
35	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_String
_
555	2765	655	2875
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	3
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	_	_	(-)
EndOfValues
EndOfLabels
in the form (x) where x is the delimiter

outputport

1	3	655	2850	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	655	2860	1	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_String	2	Run_CloseFILE_ReadOnly_String	1	Run_OpenFILE_ReadOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	555	2775	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	555	2800	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	555	2825	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	655	2775	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	655	2800	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	655	2825	0	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	7	4	3	-1	0
END_LINE
finishport

3	1	655	2785	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	2810	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	655	2835	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wob
_
36	0	0	0
_
_
Writes boolean data to a file. If a field separator parameter is specified then then this will be used inserted between data points. Fixed width is not available for this data type. The append option specified if the file is truncated to an empty file whe
FILE_WriteOnly_Bool
_
350	2015	450	2115
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Boolean Field Delimiter Character

parameter
Append	0	_	_	1
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	0	350	2100	0	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_Bool	1	EndOfFunc
BEGIN_LINE
1	91	0	0	-1	0
340	2090
340	2100
END_LINE
outputport

1	1	450	2100	1	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_Bool	2	Run_CloseFILE_WriteOnly_Bool	1	Run_OpenFILE_WriteOnly_Bool	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	350	2025	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	350	2050	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	350	2075	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_Bool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	450	2025	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	2050	2	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_Bool	2	EndOfFunc
BEGIN_LINE
0	37	0	3	-1	0
END_LINE
finishport

3	-1	450	2075	1	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	2035	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	2060	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_Bool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	2085	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_Bool	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbsx
_
37	0	0	0
_
_
This is a boolean to string converter.
ConvertorBoolToString
_
820	2820	920	2865
1	0
inputport

0	0	820	2850	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	1	EndOfFunc
BEGIN_LINE
1	34	0	0	-1	0
810	2855
810	2850
END_LINE
outputport

1	3	920	2850	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	820	2830	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	0	EndOfFunc
BEGIN_LINE
0	-1	2	2	-1	0
END_LINE
finishport

3	3	920	2830	3	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	3	EndOfFunc
BEGIN_LINE
0	7	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
38	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
_
495	2570	595	2615
1	0
inputport

0	1	495	2600	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	1	EndOfFunc
BEGIN_LINE
1	31	0	0	-1	0
485	2630
485	2600
END_LINE
outputport

1	3	595	2600	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	495	2580	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	595	2580	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
39	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
_
495	2450	595	2495
1	0
inputport

0	2	495	2480	5	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	595	2480	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	2	EndOfFunc
BEGIN_LINE
0	7	5	1	-1	0
END_LINE
startport

2	-1	495	2460	4	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	595	2460	2	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	3	EndOfFunc
BEGIN_LINE
0	38	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
60	0	0	0
clickMe
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
730	2095	830	2269
1	0
parameter
Widget Tag	5	_	_	open
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	730	2200	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2210	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2220	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2230	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2245	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	730	2260	1	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	61	0	0	-1	0
670	2225
670	2260
END_LINE
outputport

1	1	830	2200	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	830	2210	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	830	2220	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	830	2230	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	730	2105	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2115	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2130	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	730	2140	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	730	2185	1	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	61	0	2	-1	0
685	2205
685	2185
END_LINE
finishport

3	1	830	2105	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2115	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2130	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2140	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2185	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	830	2155	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2i
_
61	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
_
550	2195	600	2270
1	0
inputport

0	1	550	2220	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	62	0	0	-1	0
500	2170
500	2220
END_LINE
inputport

0	1	550	2255	5	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
530	2290
530	2255
END_LINE
outputport

1	1	600	2225	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	19	5	1	-1	0
END_LINE
startport

2	-1	550	2205	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	32	0	2	-1	0
545	2215
545	2205
END_LINE
startport

2	-1	550	2240	4	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	32	3	2	-1	0
535	2225
535	2240
END_LINE
finishport

3	1	600	2205	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	19	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
62	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
330	2140	430	2185
1	0
parameter
Integer Value	1	-2147483648	2147483647	-1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	430	2170	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	90	0	1	-1	0
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
88	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
_
545	2365	595	2440
1	0
inputport

0	1	545	2390	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	62	0	0	-1	0
535	2170
535	2390
END_LINE
inputport

0	1	545	2425	5	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
525	2290
525	2425
END_LINE
outputport

1	1	595	2395	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	19	5	1	-1	0
END_LINE
startport

2	-1	545	2375	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	32	1	2	-1	0
540	2240
540	2375
END_LINE
startport

2	-1	545	2410	4	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	32	4	2	-1	0
530	2250
530	2410
END_LINE
finishport

3	1	595	2375	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	19	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2i
_
90	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
_
545	2280	595	2355
1	0
inputport

0	1	545	2305	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	62	0	0	-1	0
465	2170
465	2305
END_LINE
inputport

0	1	545	2340	5	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
525	2290
525	2340
END_LINE
outputport

1	1	595	2310	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	19	5	1	-1	0
END_LINE
startport

2	-1	545	2290	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	32	2	2	-1	0
540	2265
540	2290
END_LINE
startport

2	-1	545	2325	4	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	32	5	2	-1	0
530	2275
530	2325
END_LINE
finishport

3	1	595	2290	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	19	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
91	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
_
135	2060	235	2105
1	0
parameter
Boolean Constant	0	_	_	1
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	235	2090	0	0	1	mandatory= 0	0	1


Run_ConstantBool	1	EndOfFunc
BEGIN_LINE
0	36	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
92	0	0	0
_
_
This is a real constant.
Constant_Real1
_
130	2265	230	2310
1	0
parameter
Real Constant	2	-1.7E308	1.7E+308	1.12345
EndOfValues
EndOfLabels
real constant

outputport

1	2	230	2295	0	0	1	mandatory= 0	0	1


Run_ConstantReal	1	EndOfFunc
BEGIN_LINE
0	32	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantReal	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
93	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
130	2360	230	2405
1	0
parameter
Integer Value	1	-2147483648	2147483647	-123
EndOfValues
EndOfLabels
integer constant

outputport

1	1	230	2390	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	33	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_real2
_
94	0	0	0
floatOut
_
This widget displays an floating point number to the GUI output (to save you converting to a string!).
gui_text_real2
_
910	2500	1010	2674
1	0
parameter
Widget Tag	5	_	_	floatOut
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	910	2605	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	2615	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	2625	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	2635	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	2650	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	910	2665	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	95	0	0	-1	0
850	2515
850	2665
END_LINE
outputport

1	1	1010	2605	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1010	2615	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1010	2625	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1010	2635	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	910	2510	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	2520	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	2535	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	2545	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	2590	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	32	2	2	-1	0
865	2265
865	2590
END_LINE
finishport

3	1	1010	2510	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	2520	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	2535	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	2545	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	2590	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	2560	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ror
_
95	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and a
FILE_ReadOnly_Real
_
335	2430	435	2540
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	3
EndOfValues
EndOfLabels
Real Field Width

parameter
Delimiter	3	_	_	(\n)
EndOfValues
EndOfLabels
Real Field Delimiter Character

outputport

1	2	435	2515	0	0	1	mandatory= 0	0	1
data

Run_ReadFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	94	5	1	-1	0
END_LINE
outputport

1	1	435	2525	0	0	1	mandatory= 0	0	1
err code

Run_ReadFILE_ReadOnly_Real	2	Run_OpenFILE_ReadOnly_Real	1	Run_CloseFILE_ReadOnly_Real	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	335	2440	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	335	2465	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	335	2490	2	0	0	mandatory= 0	0	1
read

Run_ReadFILE_ReadOnly_Real	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	435	2440	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_ReadOnly_Real	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	435	2465	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_ReadOnly_Real	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	2490	2	0	1	mandatory= 0	0	1
--

Run_ReadFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	2450	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	2475	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_ReadOnly_Real	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	435	2500	5	0	1	mandatory= 0	0	1
err

Run_ReadFILE_ReadOnly_Real	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
