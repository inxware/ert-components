#V:2.0.0

IconData
BEGIN_BLOCK

devman_interface
_
1245	0	0	0
_
_
This component reports data to the Portal using Devman Monitor and gets the Devman settings
DevmanInterface
0
_
1310	295	1399	464
1	0
parameter
App Name	3	0	0	appname1
EndOfValues
EndOfLabels
identifies data service channel

parameter
Process Name	3	0	0	processname1
EndOfValues
EndOfLabels
Optional name of another process to monitor

inputport

0	3	1304	315	0	0	1	mandatory= 0	0	1
misc. info

send	1	EndOfFunc
BEGIN_LINE
1	1248	0	0	-1	0
1294	320
1294	315
END_LINE
outputport

1	3	1403	365	0	0	1	mandatory= 0	0	1
server URL

getServerUrl	1	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
outputport

1	3	1403	395	1	0	1	mandatory= 0	0	1
JSON

getMiscData	1	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
outputport

1	3	1403	425	2	0	1	mandatory= 0	0	1
new JSON

listen4New	1	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
startport

2	3	1304	305	0	0	0	mandatory= 0	0	1
send

send	0	EndOfFunc
BEGIN_LINE
1	1248	0	2	-1	0
1294	300
1294	305
END_LINE
startport

2	3	1304	355	1	0	0	mandatory= 0	0	1
get Server URL

getServerUrl	0	EndOfFunc
BEGIN_LINE
0	-1	2	2	-1	0
END_LINE
startport

2	3	1304	415	2	0	1	mandatory= 0	0	1
listen Misc

listen4New	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
startport

2	3	1304	385	3	0	1	mandatory= 0	0	1
get All

getMiscData	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	3	1403	305	0	0	1	mandatory= 0	0	1
----

send	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
finishport

3	3	1403	355	1	0	1	mandatory= 0	0	1
----

getServerUrl	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
finishport

3	3	1403	385	2	0	1	mandatory= 0	0	1
---

getMiscData	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
finishport

3	3	1403	415	3	0	1	mandatory= 0	0	1
new

listen4New	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

wall_clock
_
1246	0	0	0
_
_
CSets and Retrieves the current system time.
wall_clock
0
_
235	125	330	234
1	0
inputport

0	3	235	200	0	0	1	mandatory= 0	0	1
time

set	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	333	145	0	0	1	mandatory= 0	0	1
Date String

get	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	333	155	1	0	1	mandatory= 0	0	1
Mins 1970

get	2	EndOfFunc
BEGIN_LINE
0	10	0	1	-1	0
END_LINE
outputport

1	1	333	165	2	0	1	mandatory= 0	0	1
& secs

get	3	EndOfFunc
BEGIN_LINE
0	594	1	1	-1	0
END_LINE
startport

2	-1	235	135	0	0	1	mandatory= 0	0	1
get

get	0	EndOfFunc
BEGIN_LINE
1	1249	0	2	-1	0
225	136
225	135
END_LINE
startport

2	3	235	190	1	0	1	mandatory= 0	0	1
set

set	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	333	135	0	0	1	mandatory= 0	0	1
------

get	4	EndOfFunc
BEGIN_LINE
0	10	0	3	-1	0
END_LINE
finishport

3	3	333	190	1	0	1	mandatory= 0	0	1
-----

set	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
1247	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
1000	170	1100	215
1	0
inputport

0	1	1000	200	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	1	EndOfFunc
BEGIN_LINE
1	1262	0	0	-1	0
990	195
990	200
END_LINE
outputport

1	3	1100	200	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	2	EndOfFunc
BEGIN_LINE
0	58	0	1	-1	0
END_LINE
startport

2	0	1000	180	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	0	EndOfFunc
BEGIN_LINE
1	1262	0	2	-1	0
995	175
995	180
END_LINE
finishport

3	3	1100	180	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	3	EndOfFunc
BEGIN_LINE
0	277	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
1248	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
1230	290	1280	350
1	0
parameter
Format	3	_	_	{"pairing":{"code":"%s"}}
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	1230	320	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	1255	0	0	-1	0
1220	320
1220	320
END_LINE
inputport

0	3	1230	335	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	1280	320	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	12	0	1	-1	0
END_LINE
startport

2	3	1230	300	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	1255	0	2	-1	0
1225	300
1225	300
END_LINE
finishport

3	3	1280	300	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
1249	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
170	115	190	155
1	0
startport

2	-1	170	125	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	13	0	2	0	0
165	118
165	125
END_LINE
startport

2	-1	170	145	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	190	136	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	4	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_mul2ix
_
1250	0	0	0
_
_
2-input integer multiplier.
MultiplyTwoInputInt
0
_
555	125	605	185
1	0
inputport

0	1	555	155	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	1246	1	0	-1	0
355	155
355	155
END_LINE
inputport

0	1	555	170	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	1246	2	0	-1	0
535	165
535	170
END_LINE
outputport

1	1	605	155	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	5	0	1	-1	0
END_LINE
startport

2	0	555	135	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	1261	0	2	-1	0
550	131
550	135
END_LINE
finishport

3	1	605	135	0	0	1	mandatory= 0	0	1


Run_MultiplyTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	5	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XFINISH
_
11	0	0	0
created
_
This is an encapsulation stop control.
xfinish
0
_
1310	120	1394	149
1	0
startport

2	3	1307	133	0	0	1	mandatory= 0	0	1


end	0	EndOfFunc
BEGIN_LINE
1	1256	0	2	4	0
1297	140
1297	133
END_LINE
END_BLOCK
BEGIN_BLOCK

XOUTPUTS
_
12	0	0	0
pairingcode
_
This is an encapsulation output.
xoutput
0
_
1310	180	1394	209
1	0
inputport

0	3	1307	193	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	1256	0	0	3	0
1297	160
1297	193
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
13	0	0	0
getnewcode
_
This is an encapsulation start control.
xstart
0
_
40	105	124	134
1	0
finishport

3	-1	128	118	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	9	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_modix
_
1251	0	0	0
_
_
This is a 2-input integer modulus.
ModulusInt
0
_
660	165	710	225
1	0
inputport

0	1	660	195	0	0	1	mandatory= 1	0	1
N

Run_ModulusInt	1	EndOfFunc
BEGIN_LINE
1	1250	0	0	-1	0
630	155
630	195
END_LINE
inputport

0	1	660	210	1	0	1	mandatory= 1	0	1
D

Run_ModulusInt	2	EndOfFunc
BEGIN_LINE
1	1252	0	0	-1	0
640	325
640	210
END_LINE
outputport

1	1	710	195	0	0	1	mandatory= 0	0	1


Run_ModulusInt	3	EndOfFunc
BEGIN_LINE
0	1168	0	1	-1	0
END_LINE
startport

2	3	660	175	0	0	1	mandatory= 1	0	1


Run_ModulusInt	0	EndOfFunc
BEGIN_LINE
1	1250	0	2	-1	0
645	135
645	175
END_LINE
finishport

3	1	710	175	0	0	1	mandatory= 0	0	1


Run_ModulusInt	4	EndOfFunc
BEGIN_LINE
0	629	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
1252	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
515	295	615	340
1	0
parameter
Integer Value	1	-2147483648	2147483647	999999
EndOfValues
EndOfLabels
integer constant

outputport

1	1	615	325	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	44	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
1253	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
1150	125	1200	185
1	0
parameter
Format	3	_	_	Your Pairing Code is %s\nlogin to https://cms.tellisign.com 
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	1150	155	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	1247	0	0	-1	0
1135	200
1135	155
END_LINE
inputport

0	3	1150	170	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	1200	155	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	12	0	1	-1	0
END_LINE
startport

2	-1	1150	135	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	1247	0	2	-1	0
1140	180
1140	135
END_LINE
finishport

3	3	1200	135	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
1254	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
1005	345	1065	435
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	600000000
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

0	1	1005	390	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	1005	405	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	1005	420	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	1005	355	0	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	1251	0	2	-1	0
735	175
735	355
END_LINE
startport

2	0	1005	370	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	915	0	2	1	0
990	348
990	370
END_LINE
finishport

3	0	1065	385	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	295	1	3	-1	0
END_LINE
finishport

3	0	1065	355	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	1065	370	2	0	1	mandatory= 0	0	1


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

mux_2s
_
1255	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
1145	290	1195	365
1	0
inputport

0	3	1145	315	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	1247	0	0	-1	0
1110	200
1110	315
END_LINE
inputport

0	3	1145	350	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	1257	0	0	-1	0
1125	320
1125	350
END_LINE
outputport

1	3	1195	320	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	272	0	1	-1	0
END_LINE
startport

2	3	1145	300	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	1247	0	2	-1	0
1110	180
1110	300
END_LINE
startport

2	3	1145	335	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	1254	0	2	-1	0
1130	385
1130	335
END_LINE
finishport

3	3	1195	300	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	272	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
1256	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
1230	130	1280	205
1	0
inputport

0	3	1230	155	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	1253	0	0	-1	0
1220	155
1220	155
END_LINE
inputport

0	3	1230	190	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	1258	0	0	-1	0
1210	255
1210	190
END_LINE
outputport

1	3	1280	160	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	12	0	1	-1	0
END_LINE
startport

2	3	1230	140	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	1253	0	2	-1	0
1220	135
1220	140
END_LINE
startport

2	3	1230	175	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	1254	0	2	-1	0
1215	385
1215	175
END_LINE
finishport

3	3	1280	140	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
1257	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
975	290	1075	335
1	0
parameter
String Constant	3	_	_	0
EndOfValues
EndOfLabels
string constant

outputport

1	3	1075	320	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	279	1	1	-1	0
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
1258	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
990	225	1090	270
1	0
parameter
String Constant	3	_	_	Pairing Code Expired  Please Reboot
EndOfValues
EndOfLabels
string constant

outputport

1	3	1090	255	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	295	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cmeibx1
_
1259	0	0	0
_
_
This is a 2-input integer comparator.
ComparatorGreaterEqualInt1
0
_
390	60	490	120
1	0
parameter
Constant	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
Comparator constant

inputport

0	1	390	90	1	0	1	mandatory= 1	0	1
A

Run_ComparatorGreaterEqualInt	1	EndOfFunc
BEGIN_LINE
1	1246	2	0	-1	0
345	165
345	90
END_LINE
inputport

0	1	390	105	1	0	1	mandatory= 1	0	1
B

Run_ComparatorGreaterEqualInt	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	490	105	0	0	1	mandatory= 0	0	1


Run_ComparatorGreaterEqualInt	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	390	70	0	0	1	mandatory= 0	0	1


Run_ComparatorGreaterEqualInt	0	EndOfFunc
BEGIN_LINE
1	1246	0	2	-1	0
355	135
355	70
END_LINE
finishport

3	0	490	70	0	0	1	mandatory= 0	0	1
t

Run_ComparatorGreaterEqualInt	4	EndOfFunc
BEGIN_LINE
0	604	0	3	-1	0
END_LINE
finishport

3	0	490	85	1	0	1	mandatory= 0	0	1
f

Run_ComparatorGreaterEqualInt	5	EndOfFunc
BEGIN_LINE
0	605	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
1260	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
245	25	305	115
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	5000000
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

0	1	245	70	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	245	85	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	245	100	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	245	35	0	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	1259	0	2	-1	0
500	70
500	20
240	20
240	35
END_LINE
startport

2	0	245	50	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	915	0	2	1	0
230	348
230	50
END_LINE
finishport

3	0	305	65	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	605	1	3	-1	0
END_LINE
finishport

3	0	305	35	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	305	50	2	0	1	mandatory= 0	0	1


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
1261	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
515	110	535	150
1	0
startport

2	-1	515	120	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	1259	1	2	-1	0
510	85
510	120
END_LINE
startport

2	-1	515	140	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	1260	0	2	-1	0
340	65
340	40
500	40
500	140
END_LINE
finishport

3	-1	535	131	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	594	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_add2ix
_
1262	0	0	0
_
_
This is a 2-input integer adder.
AdditionTwoInputInt
0
_
890	165	940	225
1	0
inputport

0	1	890	195	0	0	1	mandatory= 1	0	1


Run_AdditionTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	1251	0	0	-1	0
745	195
745	195
END_LINE
inputport

0	1	890	210	0	0	1	mandatory= 1	0	1


Run_AdditionTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	1265	0	0	-1	0
870	485
870	210
END_LINE
outputport

1	1	940	195	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	1149	0	1	-1	0
END_LINE
startport

2	0	890	175	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	1265	0	2	-1	0
885	465
885	175
END_LINE
finishport

3	1	940	175	0	0	1	mandatory= 0	0	1


Run_AdditionTwoInputInt	4	EndOfFunc
BEGIN_LINE
0	1149	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
1263	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
605	555	705	600
1	0
parameter
Integer Value	1	-2147483648	2147483647	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	705	585	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1169	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

XSTART
_
915	0	0	0
stop
_
This is an encapsulation start control.
xstart
0
_
100	335	184	364
1	0
finishport

3	-1	188	348	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	1001	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cmeibx1
_
1264	0	0	0
_
_
This is a 2-input integer comparator.
ComparatorGreaterEqualInt1
0
_
610	465	710	525
1	0
parameter
Constant	1	-2147483648	2147483647	99999
EndOfValues
EndOfLabels
Comparator constant

inputport

0	1	610	495	0	0	1	mandatory= 1	0	1
A

Run_ComparatorGreaterEqualInt	1	EndOfFunc
BEGIN_LINE
1	1251	0	0	-1	0
720	195
720	155
600	155
600	495
END_LINE
inputport

0	1	610	510	1	0	1	mandatory= 1	0	1
B

Run_ComparatorGreaterEqualInt	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	710	510	0	0	1	mandatory= 0	0	1


Run_ComparatorGreaterEqualInt	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	610	475	0	0	1	mandatory= 0	0	1


Run_ComparatorGreaterEqualInt	0	EndOfFunc
BEGIN_LINE
1	1251	0	2	-1	0
725	175
725	420
595	420
595	475
END_LINE
finishport

3	0	710	475	0	0	1	mandatory= 0	0	1
t

Run_ComparatorGreaterEqualInt	4	EndOfFunc
BEGIN_LINE
0	1169	0	3	-1	0
END_LINE
finishport

3	0	710	490	1	0	1	mandatory= 0	0	1
f

Run_ComparatorGreaterEqualInt	5	EndOfFunc
BEGIN_LINE
0	1169	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2i
_
1265	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
0
_
790	455	840	530
1	0
inputport

0	1	790	480	1	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	1266	0	0	-1	0
780	400
780	480
END_LINE
inputport

0	1	790	515	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	1263	0	0	-1	0
770	585
770	515
END_LINE
outputport

1	1	840	485	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	1165	1	1	-1	0
END_LINE
startport

2	1	790	465	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	1264	0	2	-1	0
785	475
785	465
END_LINE
startport

2	1	790	500	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	1264	1	2	-1	0
775	490
775	500
END_LINE
finishport

3	1	840	465	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	1165	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
1266	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
565	370	665	415
1	0
parameter
Integer Value	1	-2147483648	2147483647	100000
EndOfValues
EndOfLabels
integer constant

outputport

1	1	665	400	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	1169	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
