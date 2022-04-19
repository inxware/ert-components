CanvasSizeXY
2000	2000

IconData
BEGIN_BLOCK

calc_i1
_
1	0	0	0
_
_
Integer Operations on 'a' and 'b'. +,-,*,/,%,^,abs(x),square(y),min(x,y),max(x,y),sin(output mult, angle),cos(output mult, angle)
ICalc
_
300	175	350	217
1	0
parameter
Expression	3	_	_	_
EndOfValues
EndOfLabels
The expression to calculate

inputport

0	1	300	205	0	0	1	mandatory= 0	0	1
A

run	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	350	205	0	0	1	mandatory= 0	0	1


run	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	300	185	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	350	185	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

calc_i2
_
2	0	0	0
_
_
Integer Operations on 'a' and 'b'. +,-,*,/,%,^,abs(x),square(y),min(x,y),max(x,y),sin(output mult, angle),cos(output mult, angle)
ICalc
_
295	230	345	287
1	0
parameter
Expression	3	_	_	(A*B)/square(B)
EndOfValues
EndOfLabels
The expression to calculate

inputport

0	1	295	260	0	0	1	mandatory= 0	0	1
A

run	1	EndOfFunc
BEGIN_LINE
1	4	0	0	-1	0
285	230
285	260
END_LINE
inputport

0	1	295	275	1	0	1	mandatory= 0	0	1
B

run	2	EndOfFunc
BEGIN_LINE
1	5	0	0	-1	0
275	290
275	275
END_LINE
outputport

1	1	345	260	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
startport

2	1	295	240	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	3	5	2	-1	0
555	185
555	120
290	120
290	240
END_LINE
finishport

3	1	345	240	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
3	0	0	0
_
_
This widget displays an integer to the GUI output (to save you converting to a string!).
gui_text_int2
_
450	125	550	299
1	0
parameter
Widget Tag	5	_	_	widget0
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	450	230	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	240	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	250	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	260	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	275	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	290	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	2	0	0	-1	0
440	260
440	290
END_LINE
outputport

1	1	550	230	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	550	240	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	550	250	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	550	260	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	450	135	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	450	145	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	450	160	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	450	170	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	450	215	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
440	240
440	215
END_LINE
finishport

3	1	550	135	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	550	145	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	550	160	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	550	170	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	550	215	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	550	185	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
4	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
115	200	215	245
1	0
parameter
Integer Value	1	-2147483648	2147483647	4
EndOfValues
EndOfLabels
integer constant

outputport

1	1	215	230	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	2	0	1	-1	0
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
5	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
120	260	220	305
1	0
parameter
Integer Value	1	-2147483648	2147483647	2
EndOfValues
EndOfLabels
integer constant

outputport

1	1	220	290	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	2	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

calc_i8
_
6	0	0	0
_
_
Integer Operations on a,b,c,d,e,f,g,h. supports +,-,*,/,%,^,abs(x),square(y),min(x,y),max(x,y),sin(output mult, angle),cos(output mult, angle)
ICalc
_
295	305	345	455
1	0
parameter
Expression	3	_	_	_
EndOfValues
EndOfLabels
The expression to calculate

inputport

0	1	295	335	0	0	1	mandatory= 0	0	1
A

run	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	350	1	0	1	mandatory= 0	0	1
B

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	365	2	0	1	mandatory= 0	0	1
C

run	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	380	3	0	1	mandatory= 0	0	1
D

run	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	395	4	0	1	mandatory= 0	0	1
E

run	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	410	5	0	1	mandatory= 0	0	1
F

run	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	425	6	0	1	mandatory= 0	0	1
G

run	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	295	440	7	0	1	mandatory= 0	0	1
H

run	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	345	335	0	0	1	mandatory= 0	0	1


run	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	295	315	0	0	1	mandatory= 0	0	1


run	10	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	345	315	0	0	1	mandatory= 0	0	1


run	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
