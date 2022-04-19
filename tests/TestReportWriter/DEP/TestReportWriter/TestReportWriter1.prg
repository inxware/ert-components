#V:2.0.0

CanvasSizeXY
2010	1010

IconData
BEGIN_BLOCK

file_wos
_
1	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii fo
FILE_WriteOnly_String
0
_
440	130	540	230
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
Delimiter	3	_	_	()
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	440	150	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	440	215	1	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
1	4	0	0	-1	0
420	150
420	215
END_LINE
outputport

1	1	540	215	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_String	2	Run_CloseFILE_WriteOnly_String	1	Run_OpenFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	440	140	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	4	0	2	-1	0
435	130
435	140
END_LINE
startport

2	3	440	165	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
555	190
555	115
425	115
425	165
END_LINE
startport

2	3	440	190	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
565	140
565	105
415	105
415	190
END_LINE
finishport

3	3	540	140	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
finishport

3	3	540	165	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	540	190	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
finishport

3	1	540	150	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	540	175	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	540	200	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
2	0	0	0
write
_
This is an encapsulation start control.
xstart
0
_
35	85	119	114
1	0
finishport

3	-1	123	98	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	4	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XINPUTB
_
3	0	0	0
pass/fail
_
This is an encapsulation input.
xinput
0
_
35	265	119	294
1	0
outputport

1	0	123	278	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8
_
4	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
290	120	340	259
1	0
parameter
Format	3	_	_	{"TestName" :"%s",%s%s%s%s%s%s%s
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	290	150	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	5	0	0	-1	0
205	163
205	150
END_LINE
inputport

0	3	290	160	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	170	2	0	1	mandatory= 0	0	1
s3

run	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	180	3	0	1	mandatory= 0	0	1
s4

run	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	190	4	0	1	mandatory= 0	0	1
s5

run	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	200	5	0	1	mandatory= 0	0	1
s6

run	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	210	6	0	1	mandatory= 0	0	1
s7

run	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	220	7	0	1	mandatory= 0	0	1
s8

run	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	290	240	8	0	1	mandatory= 0	0	1
fmt

run	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	340	150	0	0	1	mandatory= 0	0	1


run	10	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	290	130	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
280	98
280	130
END_LINE
finishport

3	3	340	130	0	0	1	mandatory= 0	0	1


run	11	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XINPUTS
_
5	0	0	0
componentname
_
This is an encapsulation input.
xinput
0
_
35	150	119	179
1	0
outputport

1	3	123	163	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	4	0	1	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
