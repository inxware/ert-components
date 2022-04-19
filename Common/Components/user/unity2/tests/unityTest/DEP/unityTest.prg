#V:2.0.0

CanvasSizeXY
2030	1300

IconData
BEGIN_BLOCK

unity2
_
1	0	0	0
zone a title
_
unity
unity2
0
_
440	80	549	219
1	0
inputport

0	3	434	110	0	0	1	mandatory= 0	0	1
zone id

create	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
424	125
424	110
END_LINE
inputport

0	3	434	120	1	0	1	mandatory= 0	0	1
Type

create	2	EndOfFunc
BEGIN_LINE
1	4	0	0	-1	0
414	175
414	120
END_LINE
inputport

0	3	434	130	2	0	1	mandatory= 0	0	1
Params

create	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	434	160	3	0	1	mandatory= 0	0	1
Media Type

setmedia	1	EndOfFunc
BEGIN_LINE
1	13	0	0	-1	0
394	160
394	160
END_LINE
inputport

0	3	434	170	4	0	1	mandatory= 0	0	1
Path

setmedia	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	434	180	5	0	1	mandatory= 0	0	1
Text

setmedia	3	EndOfFunc
BEGIN_LINE
1	10	0	0	-1	0
379	220
379	180
END_LINE
inputport

0	3	434	190	6	0	1	mandatory= 0	0	1
Params

setmedia	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	553	110	0	0	1	mandatory= 0	0	1
event

create	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	434	90	0	1	1	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	434	140	1	0	1	mandatory= 0	0	1
Destroy

destroy	0	EndOfFunc
BEGIN_LINE
1	30	0	2	-1	0
910	345
910	65
419	65
419	140
END_LINE
startport

2	3	434	150	2	0	1	mandatory= 0	0	1
Set Media

setmedia	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
578	90
578	55
409	55
409	150
END_LINE
startport

2	3	434	200	3	0	1	mandatory= 0	0	1
Stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	553	90	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
finishport

3	-1	553	100	1	0	1	mandatory= 0	0	1
Event

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	553	140	2	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	553	150	3	0	1	mandatory= 0	0	1
--

setmedia	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	553	200	4	0	1	mandatory= 0	0	1
--

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

unity2
_
2	0	0	0
zone b video
_
unity
unity2
0
_
570	285	679	424
1	0
inputport

0	3	564	315	0	0	1	mandatory= 0	0	1
zone id

create	1	EndOfFunc
BEGIN_LINE
1	5	0	0	-1	0
429	330
429	315
END_LINE
inputport

0	3	564	325	1	0	1	mandatory= 0	0	1
Type

create	2	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	0
419	390
419	325
END_LINE
inputport

0	3	564	335	2	0	1	mandatory= 0	0	1
Params

create	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	564	365	3	0	1	mandatory= 0	0	1
Media Type

setmedia	1	EndOfFunc
BEGIN_LINE
1	12	0	0	-1	0
524	370
524	365
END_LINE
inputport

0	3	564	375	4	0	1	mandatory= 0	0	1
Path

setmedia	2	EndOfFunc
BEGIN_LINE
1	27	0	0	-1	0
514	415
514	375
END_LINE
inputport

0	3	564	385	5	0	1	mandatory= 0	0	1
Text

setmedia	3	EndOfFunc
BEGIN_LINE
1	28	0	0	-1	0
504	465
504	385
END_LINE
inputport

0	3	564	395	6	0	1	mandatory= 0	0	1
Params

setmedia	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	683	315	0	0	1	mandatory= 0	0	1
event

create	4	EndOfFunc
BEGIN_LINE
0	30	0	1	-1	0
END_LINE
startport

2	-1	564	295	0	1	1	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	564	345	1	0	1	mandatory= 0	0	1
Destroy

destroy	0	EndOfFunc
BEGIN_LINE
1	30	0	2	-1	0
910	345
910	270
549	270
549	345
END_LINE
startport

2	3	564	355	2	0	1	mandatory= 0	0	1
Set Media

setmedia	0	EndOfFunc
BEGIN_LINE
1	22	3	2	-1	0
539	1025
539	355
END_LINE
startport

2	3	564	405	3	0	1	mandatory= 0	0	1
Stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	683	295	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	26	0	3	-1	0
END_LINE
finishport

3	-1	683	305	1	0	1	mandatory= 0	0	1
Event

create	6	EndOfFunc
BEGIN_LINE
0	30	0	3	-1	0
END_LINE
finishport

3	3	683	345	2	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	683	355	3	0	1	mandatory= 0	0	1
--

setmedia	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	683	405	4	0	1	mandatory= 0	0	1
--

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
3	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
175	95	275	140
1	0
parameter
String Constant	3	_	_	a
EndOfValues
EndOfLabels
string constant

outputport

1	3	275	125	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
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
4	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
175	145	275	190
1	0
parameter
String Constant	3	_	_	title
EndOfValues
EndOfLabels
string constant

outputport

1	3	275	175	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
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
5	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
145	300	245	345
1	0
parameter
String Constant	3	_	_	b
EndOfValues
EndOfLabels
string constant

outputport

1	3	245	330	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	0	1	-1	0
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
6	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
145	360	245	405
1	0
parameter
String Constant	3	_	_	presentation
EndOfValues
EndOfLabels
string constant

outputport

1	3	245	390	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

unity2
_
7	0	0	0
zone c spinner
_
unity
unity2
0
_
825	760	934	899
1	0
inputport

0	3	819	790	0	0	1	mandatory= 0	0	1
zone id

create	1	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
459	795
459	790
END_LINE
inputport

0	3	819	800	1	0	1	mandatory= 0	0	1
Type

create	2	EndOfFunc
BEGIN_LINE
1	9	0	0	-1	0
449	845
449	800
END_LINE
inputport

0	3	819	810	2	0	1	mandatory= 0	0	1
Params

create	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	819	840	3	0	1	mandatory= 0	0	1
Media Type

setmedia	1	EndOfFunc
BEGIN_LINE
1	14	0	0	-1	0
429	890
429	840
END_LINE
inputport

0	3	819	850	4	0	1	mandatory= 0	0	1
Path

setmedia	2	EndOfFunc
BEGIN_LINE
1	17	0	0	-1	0
769	980
769	850
END_LINE
inputport

0	3	819	860	5	0	1	mandatory= 0	0	1
Text

setmedia	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	819	870	6	0	1	mandatory= 0	0	1
Params

setmedia	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	938	790	0	0	1	mandatory= 0	0	1
event

create	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	819	770	0	1	1	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	819	820	1	0	1	mandatory= 0	0	1
Destroy

destroy	0	EndOfFunc
BEGIN_LINE
1	30	0	2	-1	0
910	345
910	290
804	290
804	820
END_LINE
startport

2	3	819	830	2	0	1	mandatory= 0	0	1
Set Media

setmedia	0	EndOfFunc
BEGIN_LINE
1	17	0	2	-1	0
794	960
794	830
END_LINE
startport

2	3	819	880	3	0	1	mandatory= 0	0	1
Stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	938	770	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	24	0	3	-1	0
END_LINE
finishport

3	-1	938	780	1	0	1	mandatory= 0	0	1
Event

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	938	820	2	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	938	830	3	0	1	mandatory= 0	0	1
--

setmedia	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	938	880	4	0	1	mandatory= 0	0	1
--

stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
8	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
220	765	320	810
1	0
parameter
String Constant	3	_	_	c
EndOfValues
EndOfLabels
string constant

outputport

1	3	320	795	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	7	0	1	-1	0
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
9	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
220	815	320	860
1	0
parameter
String Constant	3	_	_	spinner
EndOfValues
EndOfLabels
string constant

outputport

1	3	320	845	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	7	1	1	-1	0
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
10	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
175	190	275	235
1	0
parameter
String Constant	3	_	_	This is the title
EndOfValues
EndOfLabels
string constant

outputport

1	3	275	220	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	5	1	-1	0
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
12	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
360	340	460	385
1	0
parameter
String Constant	3	_	_	video
EndOfValues
EndOfLabels
string constant

outputport

1	3	460	370	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	3	1	-1	0
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
13	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
285	130	385	175
1	0
parameter
String Constant	3	_	_	text
EndOfValues
EndOfLabels
string constant

outputport

1	3	385	160	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	1	3	1	-1	0
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
14	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
155	860	255	905
1	0
parameter
String Constant	3	_	_	image
EndOfValues
EndOfLabels
string constant

outputport

1	3	255	890	0	0	1	mandatory= 0	0	1


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

mux_4s
_
17	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
680	950	730	1095
1	0
inputport

0	3	680	975	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	18	0	0	-1	0
665	975
665	975
END_LINE
inputport

0	3	680	1010	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	19	0	0	-1	0
635	1025
635	1010
END_LINE
inputport

0	3	680	1045	2	0	1	mandatory= 0	0	1
i3

Run_Input3	1	EndOfFunc
BEGIN_LINE
1	20	0	0	-1	0
670	1080
670	1045
END_LINE
inputport

0	3	680	1080	3	0	1	mandatory= 0	0	1
i4

Run_Input4	1	EndOfFunc
BEGIN_LINE
1	21	0	0	-1	0
645	1135
645	1080
END_LINE
outputport

1	3	730	980	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	Run_Input3	2	Run_Input4	2	EndOfFunc
BEGIN_LINE
0	7	4	1	-1	0
END_LINE
startport

2	3	680	960	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	22	0	2	-1	0
675	965
675	960
END_LINE
startport

2	3	680	995	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	22	1	2	-1	0
665	985
665	995
END_LINE
startport

2	3	680	1030	2	0	1	mandatory= 0	0	1


Run_Input3	0	EndOfFunc
BEGIN_LINE
1	22	2	2	-1	0
655	1005
655	1030
END_LINE
startport

2	3	680	1065	3	0	1	mandatory= 0	0	1


Run_Input4	0	EndOfFunc
BEGIN_LINE
1	22	3	2	-1	0
645	1025
645	1065
END_LINE
finishport

3	3	730	960	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	Run_Input3	3	Run_Input4	3	EndOfFunc
BEGIN_LINE
0	7	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
18	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
530	945	630	990
1	0
parameter
String Constant	3	_	_	/data/image1.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	630	975	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	17	0	1	-1	0
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
19	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
525	995	625	1040
1	0
parameter
String Constant	3	_	_	/data/image2.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	625	1025	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	17	1	1	-1	0
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
20	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
525	1050	625	1095
1	0
parameter
String Constant	3	_	_	/data/image3.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	625	1080	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	17	2	1	-1	0
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
21	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
530	1105	630	1150
1	0
parameter
String Constant	3	_	_	/data/image4.png
EndOfValues
EndOfLabels
string constant

outputport

1	3	630	1135	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	17	3	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_8indexedi
_
22	0	0	0
_
_
Distributes Data to Indexed Output
IndexedDemultiplexer_Int
0
_
400	945	450	1164
1	0
parameter
Index start	1	-2147483648	2147483647	1
EndOfValues
EndOfLabels
Value for firing port 1

inputport

0	1	400	975	0	0	1	mandatory= 0	0	1
Ind

Run	2	EndOfFunc
BEGIN_LINE
1	25	0	0	-1	0
390	975
390	975
END_LINE
inputport

0	1	400	990	1	0	1	mandatory= 0	0	1
Val

Run	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	450	975	0	0	1	mandatory= 0	0	1
1

Run	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	995	1	0	1	mandatory= 0	0	1
2

Run	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1015	2	0	1	mandatory= 0	0	1
3

Run	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1035	3	0	1	mandatory= 0	0	1
4

Run	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1055	4	0	1	mandatory= 0	0	1
5

Run	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1075	5	0	1	mandatory= 0	0	1
6

Run	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1095	6	0	1	mandatory= 0	0	1
7

Run	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1115	7	0	1	mandatory= 0	0	1
8

Run	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	450	1155	8	0	1	mandatory= 0	0	1
Offset

Run	11	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	400	955	0	0	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
1	25	0	2	-1	0
395	955
395	955
END_LINE
finishport

3	1	450	965	0	0	1	mandatory= 0	0	1


Run	12	EndOfFunc
BEGIN_LINE
0	17	0	3	-1	0
END_LINE
finishport

3	1	450	985	1	0	1	mandatory= 0	0	1


Run	13	EndOfFunc
BEGIN_LINE
0	17	1	3	-1	0
END_LINE
finishport

3	1	450	1005	2	0	1	mandatory= 0	0	1


Run	14	EndOfFunc
BEGIN_LINE
0	17	2	3	-1	0
END_LINE
finishport

3	1	450	1025	3	0	1	mandatory= 0	0	1


Run	15	EndOfFunc
BEGIN_LINE
0	2	2	3	-1	0
END_LINE
finishport

3	1	450	1045	4	0	1	mandatory= 0	0	1


Run	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	1065	5	0	1	mandatory= 0	0	1


Run	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	1085	6	0	1	mandatory= 0	0	1


Run	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	1105	7	0	1	mandatory= 0	0	1


Run	19	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	450	1135	8	0	1	mandatory= 0	0	1
Overflow

Run	20	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
24	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
155	945	215	1035
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	1000000
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

0	1	155	990	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	155	1005	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	155	1020	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	155	955	0	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	7	0	2	-1	0
943	770
943	755
150	755
150	955
END_LINE
startport

2	0	155	970	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
1	22	3	2	-1	0
465	1025
465	930
140	930
140	970
END_LINE
finishport

3	0	215	985	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	25	0	3	-1	0
END_LINE
finishport

3	0	215	955	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	215	970	2	0	1	mandatory= 0	0	1


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
25	0	0	0
_
_
This is an event counter with reset.
EventCounter
0
_
275	945	325	990
1	0
outputport

1	1	325	975	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	22	0	1	-1	0
END_LINE
startport

2	-1	275	955	0	0	1	mandatory= 1	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	24	0	2	-1	0
270	985
270	955
END_LINE
startport

2	-1	275	975	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	325	955	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
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
0
_
360	385	460	430
1	0
parameter
String Constant	3	_	_	/data/video1.mp4
EndOfValues
EndOfLabels
string constant

outputport

1	3	460	415	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	4	1	-1	0
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
0
_
350	435	450	480
1	0
parameter
String Constant	3	_	_	this is the overlay
EndOfValues
EndOfLabels
string constant

outputport

1	3	450	465	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	2	5	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
30	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
845	305	895	479
1	0
parameter
case 1	3	_	_	_
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	_	_	_
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	_	_	_
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	_	_	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	_	_	_
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	_	_	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	_	_	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	_	_	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	845	325	0	0	1	mandatory= 0	0	1
test

Run	1	EndOfFunc
BEGIN_LINE
1	2	0	0	-1	0
835	315
835	325
END_LINE
inputport

0	3	845	345	1	0	1	mandatory= 0	0	1
case 1

Run	2	EndOfFunc
BEGIN_LINE
1	31	0	0	-1	0
825	400
825	345
END_LINE
inputport

0	3	845	360	2	0	1	mandatory= 0	0	1
case 2

Run	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	375	3	0	1	mandatory= 0	0	1
case 3

Run	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	390	4	0	1	mandatory= 0	0	1
case 4

Run	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	405	5	0	1	mandatory= 0	0	1
case 5

Run	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	420	6	0	1	mandatory= 0	0	1
case 6

Run	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	435	7	0	1	mandatory= 0	0	1
case 7

Run	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	450	8	0	1	mandatory= 0	0	1
case 8

Run	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	845	315	0	0	1	mandatory= 0	0	1
Go

Run	0	EndOfFunc
BEGIN_LINE
1	2	1	2	-1	0
840	305
840	315
END_LINE
finishport

3	3	895	345	0	0	1	mandatory= 0	0	1
1

Run	10	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
finishport

3	3	895	360	1	0	1	mandatory= 0	0	1
2

Run	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	375	2	0	1	mandatory= 0	0	1
3

Run	12	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	390	3	0	1	mandatory= 0	0	1
4

Run	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	405	4	0	1	mandatory= 0	0	1
5

Run	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	420	5	0	1	mandatory= 0	0	1
6

Run	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	435	6	0	1	mandatory= 0	0	1
7

Run	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	450	7	0	1	mandatory= 0	0	1
8

Run	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	895	465	8	0	1	mandatory= 0	0	1
Default

Run	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
31	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
700	370	800	415
1	0
parameter
String Constant	3	_	_	videoend
EndOfValues
EndOfLabels
string constant

outputport

1	3	800	400	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	30	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
