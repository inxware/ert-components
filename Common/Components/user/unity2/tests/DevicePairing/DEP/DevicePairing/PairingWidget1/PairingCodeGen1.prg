#V:2.0.0

IconData
BEGIN_BLOCK

devman_interface
_
27	0	0	0
_
_
This component reports data to the Portal using Devman Monitor and gets the Devman settings
DevmanInterface
0
_
775	220	864	389
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

0	3	769	240	0	0	1	mandatory= 0	0	1
misc. info

send	1	EndOfFunc
BEGIN_LINE
1	30	0	0	-1	0
759	200
759	240
END_LINE
outputport

1	3	868	290	0	0	1	mandatory= 0	0	1
server URL

getServerUrl	1	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
outputport

1	3	868	320	1	0	1	mandatory= 0	0	1
JSON

getMiscData	1	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
outputport

1	3	868	350	2	0	1	mandatory= 0	0	1
new JSON

listen4New	1	EndOfFunc
BEGIN_LINE
0	3	5	1	-1	0
END_LINE
startport

2	3	769	230	0	0	0	mandatory= 0	0	1
send

send	0	EndOfFunc
BEGIN_LINE
1	30	0	2	-1	0
759	180
759	230
END_LINE
startport

2	3	769	280	1	0	0	mandatory= 0	0	1
get Server URL

getServerUrl	0	EndOfFunc
BEGIN_LINE
0	-1	2	2	-1	0
END_LINE
startport

2	3	769	340	2	0	1	mandatory= 0	0	1
listen Misc

listen4New	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
startport

2	3	769	310	3	0	1	mandatory= 0	0	1
get All

getMiscData	0	EndOfFunc
BEGIN_LINE
0	-1	5	2	-1	0
END_LINE
finishport

3	3	868	230	0	0	1	mandatory= 0	0	1
----

send	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
finishport

3	3	868	280	1	0	1	mandatory= 0	0	1
----

getServerUrl	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
finishport

3	3	868	310	2	0	1	mandatory= 0	0	1
---

getMiscData	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
finishport

3	3	868	340	3	0	1	mandatory= 0	0	1
new

listen4New	2	EndOfFunc
BEGIN_LINE
0	3	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

wall_clock
_
28	0	0	0
_
_
CSets and Retrieves the current system time.
wall_clock
0
_
235	85	330	194
1	0
inputport

0	3	235	160	0	0	1	mandatory= 0	0	1
time

set	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	333	105	0	0	1	mandatory= 0	0	1
Date String

get	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	333	115	1	0	1	mandatory= 0	0	1
Mins 1970

get	2	EndOfFunc
BEGIN_LINE
0	10	0	1	-1	0
END_LINE
outputport

1	1	333	125	2	0	1	mandatory= 0	0	1
& secs

get	3	EndOfFunc
BEGIN_LINE
0	10	1	1	-1	0
END_LINE
startport

2	-1	235	95	0	0	1	mandatory= 0	0	1
get

get	0	EndOfFunc
BEGIN_LINE
1	31	0	2	-1	0
225	96
225	95
END_LINE
startport

2	3	235	150	1	0	1	mandatory= 0	0	1
set

set	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	333	95	0	0	1	mandatory= 0	0	1
------

get	4	EndOfFunc
BEGIN_LINE
0	10	0	3	-1	0
END_LINE
finishport

3	3	333	150	1	0	1	mandatory= 0	0	1
-----

set	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
29	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
555	140	655	185
1	0
inputport

0	1	555	170	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	1	EndOfFunc
BEGIN_LINE
1	33	0	0	-1	0
545	155
545	170
END_LINE
outputport

1	3	655	170	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	2	EndOfFunc
BEGIN_LINE
0	58	0	1	-1	0
END_LINE
startport

2	1	555	150	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	0	EndOfFunc
BEGIN_LINE
1	33	0	2	-1	0
550	135
550	150
END_LINE
finishport

3	3	655	150	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	3	EndOfFunc
BEGIN_LINE
0	30	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
30	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
685	170	735	230
1	0
parameter
Format	3	_	_	{"pairing":{"code":"%s"}}
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	685	200	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	29	0	0	-1	0
665	170
665	200
END_LINE
inputport

0	3	685	215	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	735	200	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	12	0	1	-1	0
END_LINE
startport

2	3	685	180	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	29	0	2	-1	0
675	150
675	180
END_LINE
finishport

3	3	735	180	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
31	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
170	75	190	115
1	0
startport

2	-1	170	85	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	13	0	2	-1	0
165	78
165	85
END_LINE
startport

2	-1	170	105	1	1	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	190	96	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	4	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_mul2ix
_
32	0	0	0
_
_
2-input integer multiplier.
MultiplyTwoInputInt
0
_
365	85	415	145
1	0
inputport

0	1	365	115	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	1	EndOfFunc
BEGIN_LINE
1	28	1	0	-1	0
355	115
355	115
END_LINE
inputport

0	1	365	130	1	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	2	EndOfFunc
BEGIN_LINE
1	28	2	0	-1	0
355	125
355	130
END_LINE
outputport

1	1	415	115	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	3	EndOfFunc
BEGIN_LINE
0	5	0	1	-1	0
END_LINE
startport

2	1	365	95	0	0	1	mandatory= 1	0	1


Run_MultiplyTwoInputInt	0	EndOfFunc
BEGIN_LINE
1	28	0	2	-1	0
355	95
355	95
END_LINE
finishport

3	1	415	95	0	0	1	mandatory= 0	0	1


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
860	75	944	104
1	0
startport

2	3	857	88	0	0	1	mandatory= 0	0	1


end	0	EndOfFunc
BEGIN_LINE
1	35	0	2	-1	0
817	90
817	88
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
860	135	944	164
1	0
inputport

0	3	857	148	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	35	0	0	-1	0
835	110
835	148
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
40	65	124	94
1	0
finishport

3	-1	128	78	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	9	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_modix
_
33	0	0	0
_
_
This is a 2-input integer modulus.
ModulusInt
0
_
470	125	520	185
1	0
inputport

0	1	470	155	0	0	1	mandatory= 1	0	1
N

Run_ModulusInt	1	EndOfFunc
BEGIN_LINE
1	32	0	0	-1	0
440	115
440	155
END_LINE
inputport

0	1	470	170	1	0	1	mandatory= 1	0	1
D

Run_ModulusInt	2	EndOfFunc
BEGIN_LINE
1	34	0	0	-1	0
450	285
450	170
END_LINE
outputport

1	1	520	155	0	0	1	mandatory= 0	0	1


Run_ModulusInt	3	EndOfFunc
BEGIN_LINE
0	29	0	1	-1	0
END_LINE
startport

2	3	470	135	0	0	1	mandatory= 1	0	1


Run_ModulusInt	0	EndOfFunc
BEGIN_LINE
1	32	0	2	-1	0
455	95
455	135
END_LINE
finishport

3	1	520	135	0	0	1	mandatory= 0	0	1


Run_ModulusInt	4	EndOfFunc
BEGIN_LINE
0	29	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
34	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
325	255	425	300
1	0
parameter
Integer Value	1	-2147483648	2147483647	999999
EndOfValues
EndOfLabels
integer constant

outputport

1	1	425	285	0	0	1	mandatory= 0	0	1


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
35	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
755	80	805	140
1	0
parameter
Format	3	_	_	Your Pairing Code is %s\nlogin to https://cms.tellisign.com 
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	755	110	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	29	0	0	-1	0
665	170
665	110
END_LINE
inputport

0	3	755	125	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	805	110	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	12	0	1	-1	0
END_LINE
startport

2	-1	755	90	0	0	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
1	30	0	2	-1	0
740	180
740	90
END_LINE
finishport

3	3	805	90	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	11	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
