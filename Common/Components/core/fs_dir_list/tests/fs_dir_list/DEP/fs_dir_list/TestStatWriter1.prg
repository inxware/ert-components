#V:2.0.0

IconData
BEGIN_BLOCK

file_wos
_
111	0	0	0
Writes only once
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii fo
FILE_WriteOnly_String
0
_
405	95	505	170
1	0
parameter
File name	3	_	_	test-status.log
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	12	0
EndOfValues
EndOfLabels
Data Field Width (space padded)

parameter
Delimiter	3	_	_	( )
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	405	155	0	0	1	mandatory= 0	0	1


Run_WriteFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
1	114	0	0	-1	0
375	160
375	155
END_LINE
outputport

1	1	505	155	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_String	2	Run_CloseFILE_WriteOnly_String	1	Run_OpenFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	405	105	0	1	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	405	120	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	111	2	2	-1	0
520	135
520	80
390	80
390	120
END_LINE
startport

2	-1	405	135	2	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	114	0	2	-1	0
380	140
380	135
END_LINE
finishport

3	-1	505	105	0	0	1	mandatory= 0	0	1


Run_OpenFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	505	120	1	0	1	mandatory= 0	0	1


Run_CloseFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	87	0	3	-1	0
END_LINE
finishport

3	1	505	135	2	0	1	mandatory= 0	0	1


Run_WriteFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
112	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
150	240	250	285
1	0
parameter
String Constant	3	_	_	fail
EndOfValues
EndOfLabels
string constant

outputport

1	3	250	270	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	7	3	1	-1	0
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
113	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
155	185	255	230
1	0
parameter
String Constant	3	_	_	pass
EndOfValues
EndOfLabels
string constant

outputport

1	3	255	215	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	7	2	1	-1	0
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
114	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
300	130	350	275
1	0
inputport

0	3	300	155	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	300	190	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	300	225	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	113	0	0	-1	0
270	215
270	225
END_LINE
inputport

0	3	300	260	3	0	1	mandatory= 0	0	1
i4

Run_Input4	1	EndOfFunc
BEGIN_LINE
1	112	0	0	-1	0
260	270
260	260
END_LINE
outputport

1	3	350	160	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
startport

2	3	300	140	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	300	175	0	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	300	210	1	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
1	118	0	2	-1	0
275	110
275	210
END_LINE
startport

2	-1	300	245	0	0	1	mandatory= 0	0	1


Run_Input4	0	EndOfFunc
BEGIN_LINE
1	117	0	2	-1	0
265	326
265	245
END_LINE
finishport

3	3	350	140	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	Run_Input4	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
11	0	0	0
pass
_
This is an encapsulation start control.
xstart
0
_
0	70	84	99
1	0
finishport

3	-1	88	83	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
12	0	0	0
fail
_
This is an encapsulation start control.
xstart
0
_
0	170	84	199
1	0
finishport

3	-1	88	183	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	106	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ehs_controller
_
115	0	0	0
_
_
stop, pause, change application etc. can be done with this function block.
ehs_controller
0
_
555	250	684	439
1	0
startport

2	-1	549	270	0	0	0	mandatory= 0	0	1
exit

exit_ehs	0	EndOfFunc
BEGIN_LINE
0	-1	1	2	-1	0
END_LINE
startport

2	-1	549	310	1	0	0	mandatory= 0	0	1
fore

move_to_foreground	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	549	370	2	0	0	mandatory= 0	0	1
back

move_to_background	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	688	330	0	0	1	mandatory= 0	0	1
error

move_to_foreground	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	688	370	1	0	1	mandatory= 0	0	1
------

move_to_background	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	688	310	2	0	1	mandatory= 0	0	1
------

EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	688	390	3	0	1	mandatory= 0	0	1
error

EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	688	270	4	0	1	mandatory= 0	0	1
exit error

exit_ehs	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
116	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
45	380	105	470
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	1000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	45	425	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	45	440	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	45	455	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	45	390	0	1	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	45	405	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	105	420	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	89	1	3	-1	0
END_LINE
finishport

3	0	105	390	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	105	405	2	0	1	mandatory= 0	0	1


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

trigger_eventor2
_
117	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
190	305	210	345
1	0
startport

2	0	190	315	1	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	12	0	2	1	0
185	183
185	315
END_LINE
startport

2	-1	190	335	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	210	326	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	86	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
118	0	0	0
Fail prioritiser
_
Creates periodic timed events.
time_clock
0
_
165	70	225	160
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	5000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	165	115	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	165	130	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	165	145	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	165	80	2	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	11	0	2	0	0
END_LINE
startport

2	3	165	95	3	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	12	0	2	1	0
END_LINE
finishport

3	0	225	110	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	103	2	3	-1	0
END_LINE
finishport

3	0	225	80	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	225	95	2	0	1	mandatory= 0	0	1


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
END_OF_BLOCKS
