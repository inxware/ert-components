#V:2.0.0

CanvasSizeXY
2000	1000

IconData
BEGIN_BLOCK

const_i1
_
2	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
25	185	125	230
1	0
parameter
Integer Value	1	-2147483648	2147483647	79
EndOfValues
EndOfLabels
integer constant

outputport

1	1	125	215	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

inx-unity
_
3	0	0	0
_
_
Controls the current Unity window
unity
0
_
235	245	324	354
1	0
inputport

0	1	229	275	0	0	1	mandatory= 0	0	1
Number

write	2	EndOfFunc
BEGIN_LINE
1	2	0	0	-1	0
219	215
219	275
END_LINE
startport

2	-1	229	255	0	1	0	mandatory= 0	0	1
write

write	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	328	255	0	0	1	mandatory= 0	0	1
-----

write	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
