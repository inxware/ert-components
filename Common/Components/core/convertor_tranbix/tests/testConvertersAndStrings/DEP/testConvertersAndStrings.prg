CanvasSizeXY
2000	2000

IconData
BEGIN_BLOCK

convertor_transbx
_
5	0	0	0
_
_
This is a string to boolean converter.
ConvertorStringToBool
_
410	355	510	400
1	0
inputport

0	3	410	385	0	0	1	mandatory= 0	0	1


Run_ConvertorStringToBool	1	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	0
400	380
400	385
END_LINE
outputport

1	0	510	385	0	0	1	mandatory= 0	0	1


Run_ConvertorStringToBool	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	410	365	0	1	1	mandatory= 0	0	1


Run_ConvertorStringToBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	510	365	0	0	1	mandatory= 0	0	1


Run_ConvertorStringToBool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
6	0	0	0
_
_
This is a string constant.
Constant_String1
_
260	350	360	395
1	0
parameter
String Constant	3	_	_	EHS_FALSE
EndOfValues
EndOfLabels
string constant

outputport

1	3	360	380	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	5	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_bool2
_
8	0	0	0
boolOut
_
This widget displays an boolean to the GUI output - currently as o or 1! (to save you converting to a string!).
gui_text_bool2
_
630	260	730	434
1	0
parameter
Widget Tag	5	_	_	boolOut
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	630	350	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	630	365	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	630	380	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	630	395	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	630	410	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	630	425	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	9	0	0	-1	0
570	500
570	425
END_LINE
outputport

1	1	730	350	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	730	365	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	730	380	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	730	395	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	630	270	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	630	285	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	630	300	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	630	315	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	630	330	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	9	0	2	-1	0
585	480
585	330
END_LINE
finishport

3	1	730	270	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	730	285	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	730	300	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	730	315	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	730	330	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	730	340	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrbx
_
9	0	0	0
_
_
This is a real to boolean converter.
ConvertorRealToBool
_
410	470	510	515
1	0
inputport

0	2	410	500	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToBool	1	EndOfFunc
BEGIN_LINE
1	14	0	0	-1	0
400	540
400	500
END_LINE
outputport

1	0	510	500	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToBool	2	EndOfFunc
BEGIN_LINE
0	8	5	1	-1	0
END_LINE
startport

2	0	410	480	0	1	1	mandatory= 0	0	1


Run_ConvertorRealToBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	510	480	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToBool	3	EndOfFunc
BEGIN_LINE
0	8	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrix
_
10	0	0	0
_
_
This is a real to integer converter.
ConvertorRealToInt
_
405	530	505	575
1	0
inputport

0	2	405	560	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToInt	1	EndOfFunc
BEGIN_LINE
1	14	0	0	-1	0
395	540
395	560
END_LINE
outputport

1	1	505	560	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToInt	2	EndOfFunc
BEGIN_LINE
0	12	5	1	-1	0
END_LINE
startport

2	1	405	540	0	1	1	mandatory= 0	0	1


Run_ConvertorRealToInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	505	540	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToInt	3	EndOfFunc
BEGIN_LINE
0	12	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
11	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
_
405	600	505	645
1	0
inputport

0	2	405	630	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	1	EndOfFunc
BEGIN_LINE
1	14	0	0	-1	0
395	540
395	630
END_LINE
outputport

1	3	505	630	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	2	EndOfFunc
BEGIN_LINE
0	5	0	1	-1	0
END_LINE
startport

2	3	405	610	0	1	1	mandatory= 0	0	1


Run_ConvertorRealToString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	505	610	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
12	0	0	0
intOut
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
625	450	725	624
1	0
parameter
Widget Tag	5	_	_	intOut
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	625	540	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	625	555	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	625	570	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	625	585	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	625	600	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	625	615	5	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	10	0	0	-1	0
565	560
565	615
END_LINE
outputport

1	1	725	540	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	725	555	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	725	570	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	725	585	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	625	460	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	625	475	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	625	490	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	625	505	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	625	520	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	10	0	2	-1	0
580	540
580	520
END_LINE
finishport

3	1	725	460	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	725	475	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	725	490	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	725	505	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	725	520	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	725	530	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
13	0	0	0
strOut
_
This widget displays an string to the GUI output.
gui_text_string2
_
620	645	720	819
1	0
parameter
Widget Tag	5	_	_	strOut
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	620	735	0	0	1	mandatory= 0	0	1
x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	620	750	1	0	1	mandatory= 0	0	1
y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	620	765	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	620	780	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	620	795	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	620	810	0	0	1	mandatory= 0	0	1


update	6	EndOfFunc
BEGIN_LINE
1	47	0	0	-1	0
560	715
560	810
END_LINE
outputport

1	1	720	735	0	0	1	mandatory= 0	0	1
x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	720	750	1	0	1	mandatory= 0	0	1
y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	720	765	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	720	780	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	620	655	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	620	670	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	620	685	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	620	700	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	620	715	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	47	0	2	-1	0
575	695
575	715
END_LINE
finishport

3	1	720	655	0	0	1	mandatory= 0	0	1


create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	720	670	1	0	1	mandatory= 0	0	1


destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	720	685	2	0	1	mandatory= 0	0	1


show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	720	700	3	0	1	mandatory= 0	0	1


hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	720	715	4	0	1	mandatory= 0	0	1


update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	720	725	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
14	0	0	0
_
_
This is a real constant.
Constant_Real1
_
220	510	320	555
1	0
parameter
Real Constant	2	-1.7E308	1.7E+308	-1323.2313
EndOfValues
EndOfLabels
real constant

outputport

1	2	320	540	0	0	1	mandatory= 0	0	1


Run_ConstantReal	1	EndOfFunc
BEGIN_LINE
0	11	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantReal	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
26	0	0	0
_
_
This is a string constant.
Constant_String1
_
290	815	390	860
1	0
parameter
String Constant	3	_	_	test
EndOfValues
EndOfLabels
string constant

outputport

1	3	390	845	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	13	5	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
28	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
295	945	395	990
1	0
parameter
Integer Value	1	-2147483648	2147483647	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	395	975	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	47	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_inserts
_
29	0	0	0
_
_
This function inserts a string s2 into string s1 at the specified position.
InsertString
_
500	855	550	930
1	0
inputport

0	3	500	885	0	0	1	mandatory= 0	0	1
s1

Run_InsertString	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	500	900	1	0	1	mandatory= 0	0	1
s2

Run_InsertString	2	EndOfFunc
BEGIN_LINE
1	45	0	0	-1	0
425	915
425	900
END_LINE
inputport

0	1	500	915	1	0	1	mandatory= 0	0	1


Run_InsertString	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	550	885	0	0	1	mandatory= 0	0	1


Run_InsertString	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	500	865	0	1	1	mandatory= 0	0	1


Run_InsertString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	550	865	0	0	1	mandatory= 0	0	1


Run_InsertString	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
45	0	0	0
_
_
This is a string constant.
Constant_String1
_
295	885	395	930
1	0
parameter
String Constant	3	_	_	ing
EndOfValues
EndOfLabels
string constant

outputport

1	3	395	915	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	29	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_strats
_
46	0	0	0
_
_
This function outputs the string at the specified index.
StrAtString
_
465	775	515	835
1	0
inputport

0	3	465	805	0	0	1	mandatory= 0	0	1


Run_StrAtString	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	465	820	1	0	1	mandatory= 0	0	1


Run_StrAtString	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	515	805	0	0	1	mandatory= 0	0	1


Run_StrAtString	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	465	785	0	1	1	mandatory= 0	0	1


Run_StrAtString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	515	785	0	0	1	mandatory= 0	0	1


Run_StrAtString	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

string_subs
_
47	0	0	0
_
_
This function outputs a substring specified by a start index and length.
SubString
_
450	685	500	760
1	0
inputport

0	3	450	715	5	0	1	mandatory= 0	0	1


Run_SubString	1	EndOfFunc
BEGIN_LINE
1	26	0	0	-1	0
400	845
400	715
END_LINE
inputport

0	1	450	730	2	0	1	mandatory= 0	0	1
start

Run_SubString	2	EndOfFunc
BEGIN_LINE
1	28	0	0	-1	0
410	975
410	730
END_LINE
inputport

0	1	450	745	2	0	1	mandatory= 0	0	1
length

Run_SubString	3	EndOfFunc
BEGIN_LINE
1	65	0	0	-1	0
420	1035
420	745
END_LINE
outputport

1	3	500	715	0	0	1	mandatory= 0	0	1


Run_SubString	4	EndOfFunc
BEGIN_LINE
0	13	5	1	-1	0
END_LINE
startport

2	3	450	695	0	1	1	mandatory= 0	0	1


Run_SubString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	500	695	0	0	1	mandatory= 0	0	1


Run_SubString	5	EndOfFunc
BEGIN_LINE
0	13	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
65	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
295	1005	395	1050
1	0
parameter
Integer Value	1	-2147483648	2147483647	-1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	395	1035	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	47	2	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
