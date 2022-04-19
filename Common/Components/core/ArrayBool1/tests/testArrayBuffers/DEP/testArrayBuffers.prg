CanvasSizeXY
2170	2230

IconData
BEGIN_BLOCK

gui_text_string2
_
5	0	0	0
popOff
_
This widget displays an string to the GUI output.
gui_text_string2
_
1080	220	1180	394
1	0
parameter
Widget Tag	5	_	_	popOff
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1080	325	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	335	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	345	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	355	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	370	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1080	385	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	7	0	0	-1	1
1030	305
1030	385
END_LINE
outputport

1	1	1180	325	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1180	335	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1180	345	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1180	355	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1080	230	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	240	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	255	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	265	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	1080	310	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	7	0	2	-1	0
1045	285
1045	310
END_LINE
finishport

3	1	1180	230	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	240	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	255	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	265	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	310	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	280	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
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
370	350	470	395
1	0
parameter
String Constant	3	_	_	test
EndOfValues
EndOfLabels
string constant

outputport

1	3	470	380	0	0	1	mandatory= 0	0	1


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

mux_2s
_
7	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
_
950	275	1000	350
1	0
inputport

0	3	950	300	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	118	0	0	-1	0
940	740
940	300
END_LINE
inputport

0	3	950	335	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	15	0	0	-1	0
930	265
930	335
END_LINE
outputport

1	3	1000	305	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	5	5	1	-1	0
END_LINE
startport

2	3	950	285	1	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	118	0	2	-1	0
945	720
945	285
END_LINE
startport

2	-1	950	320	3	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	128	4	2	-1	1
935	685
935	320
END_LINE
finishport

3	0	1000	285	3	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	5	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
15	0	0	0
_
_
This is a string constant.
Constant_String1
_
770	235	870	280
1	0
parameter
String Constant	3	_	_	error
EndOfValues
EndOfLabels
string constant

outputport

1	3	870	265	0	0	1	mandatory= 0	0	1


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

trigger_counter
_
17	0	0	0
_
_
This is an event counter with reset.
EventCounter
_
280	335	330	380
1	0
outputport

1	1	330	365	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	124	1	1	-1	0
END_LINE
startport

2	0	280	345	1	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	31	5	2	-1	0
1200	475
1200	875
265	875
265	345
END_LINE
startport

2	-1	280	365	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	330	345	0	0	1	mandatory= 0	0	1


Count	2	Reset	2	EndOfFunc
BEGIN_LINE
0	18	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
18	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
_
375	285	475	330
1	0
inputport

0	1	375	315	2	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	475	315	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	2	EndOfFunc
BEGIN_LINE
0	19	0	1	-1	0
END_LINE
startport

2	-1	375	295	1	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	475	295	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	3	EndOfFunc
BEGIN_LINE
0	19	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
19	0	0	0
_
_
This is a 2-input string cat.
CatString
_
505	285	555	345
1	0
inputport

0	3	505	315	0	0	1	mandatory= 0	0	1
s1

Run_CatString	1	EndOfFunc
BEGIN_LINE
1	18	0	0	-1	0
END_LINE
inputport

0	3	505	330	0	0	1	mandatory= 0	0	1
s2

Run_CatString	2	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	0
485	380
485	330
END_LINE
outputport

1	3	555	315	0	0	1	mandatory= 0	0	1


Run_CatString	3	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
startport

2	3	505	295	0	0	1	mandatory= 0	0	1


Run_CatString	0	EndOfFunc
BEGIN_LINE
1	18	0	2	-1	0
END_LINE
finishport

3	3	555	295	0	0	1	mandatory= 0	0	1


Run_CatString	4	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
31	0	0	0
pushOn
_
This widget displays an string to the GUI output.
gui_text_string2
_
1085	415	1185	589
1	0
parameter
Widget Tag	5	_	_	pushOn
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1085	520	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1085	530	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1085	540	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1085	550	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1085	565	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1085	580	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	44	0	0	-1	1
1025	500
1025	580
END_LINE
outputport

1	1	1185	520	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1185	530	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1185	540	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1185	550	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1085	425	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1085	435	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1085	450	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1085	460	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	1085	505	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	44	0	2	-1	0
1045	480
1045	505
END_LINE
finishport

3	1	1185	425	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1185	435	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1185	450	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1185	460	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1185	505	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1185	475	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	17	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
44	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
_
950	470	1000	545
1	0
inputport

0	3	950	495	1	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	73	0	0	-1	0
920	220
920	495
END_LINE
inputport

0	3	950	530	5	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	15	0	0	-1	0
930	265
930	530
END_LINE
outputport

1	3	1000	500	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	31	5	1	-1	0
END_LINE
startport

2	3	950	480	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	128	1	2	-1	0
945	640
945	480
END_LINE
startport

2	3	950	515	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	128	3	2	-1	0
935	650
935	515
END_LINE
finishport

3	3	1000	480	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	31	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
58	0	0	0
flush
_
This widget displays an string to the GUI output.
gui_text_string2
_
1080	630	1180	804
1	0
parameter
Widget Tag	5	_	_	widget3
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

inputport

0	1	1080	735	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	745	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	755	2	0	1	mandatory= 0	0	1
wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	765	3	0	1	mandatory= 0	0	1
ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	1080	780	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	1080	795	5	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	91	0	0	-1	0
1025	725
1025	795
END_LINE
outputport

1	1	1180	735	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1180	745	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1180	755	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1180	765	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	1080	640	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	650	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	665	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	675	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1080	720	3	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	91	0	2	-1	1
1045	705
1045	720
END_LINE
finishport

3	1	1180	640	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	650	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	665	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	675	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	720	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1180	690	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	75	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
73	0	0	0
_
_
This is a string constant.
Constant_String1
_
770	190	870	235
1	0
parameter
String Constant	3	_	_	pushed on
EndOfValues
EndOfLabels
string constant

outputport

1	3	870	220	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	91	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
91	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
_
945	695	995	770
1	0
inputport

0	3	945	720	1	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	73	0	0	-1	0
920	220
920	720
END_LINE
inputport

0	3	945	755	0	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	108	0	0	-1	0
925	815
925	755
END_LINE
outputport

1	3	995	725	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	58	5	1	-1	0
END_LINE
startport

2	3	945	705	1	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	118	0	2	-1	0
940	720
940	705
END_LINE
startport

2	3	945	740	0	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	128	0	2	-1	0
930	710
930	740
END_LINE
finishport

3	3	995	705	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	58	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
108	0	0	0
_
_
This is a string constant.
Constant_String1
_
780	785	880	830
1	0
parameter
String Constant	3	_	_	flushed
EndOfValues
EndOfLabels
string constant

outputport

1	3	880	815	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	91	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
113	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
_
750	480	850	525
1	0
inputport

0	1	750	510	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	1	EndOfFunc
BEGIN_LINE
1	126	0	0	-1	0
740	465
740	510
END_LINE
outputport

1	3	850	510	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	750	490	3	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	0	EndOfFunc
BEGIN_LINE
1	126	2	2	-1	1
745	445
745	490
END_LINE
finishport

3	3	850	490	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranirx
_
114	0	0	0
_
_
This is an integer to real converter.
ConvertorIntToReal
_
380	520	480	565
1	0
inputport

0	1	380	550	0	0	1	mandatory= 0	0	1


Run	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	2	480	550	0	0	1	mandatory= 0	0	1


Run	2	EndOfFunc
BEGIN_LINE
0	111	0	1	-1	0
END_LINE
startport

2	1	380	530	0	0	1	mandatory= 0	0	1


Run	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	2	480	530	0	0	1	mandatory= 0	0	1


Run	3	EndOfFunc
BEGIN_LINE
0	111	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
115	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
_
755	595	855	640
1	0
inputport

0	2	755	625	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	1	EndOfFunc
BEGIN_LINE
1	127	0	0	-1	0
745	580
745	625
END_LINE
outputport

1	3	855	625	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	755	605	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	0	EndOfFunc
BEGIN_LINE
1	127	2	2	-1	0
750	560
750	605
END_LINE
finishport

3	3	855	605	0	0	1	mandatory= 0	0	1


Run_ConvertorRealToString	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranibx
_
117	0	0	0
_
_
This is an integer to boolean converter.
ConvertorIntToBool
_
440	675	540	720
1	0
inputport

0	1	440	705	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToBool	1	EndOfFunc
BEGIN_LINE
1	119	0	0	-1	0
430	705
430	705
END_LINE
outputport

1	0	540	705	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToBool	2	EndOfFunc
BEGIN_LINE
0	112	0	1	-1	0
END_LINE
startport

2	0	440	685	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToBool	0	EndOfFunc
BEGIN_LINE
1	119	0	2	-1	0
435	685
435	685
END_LINE
finishport

3	0	540	685	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToBool	3	EndOfFunc
BEGIN_LINE
0	112	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbsx
_
118	0	0	0
_
_
This is a boolean to string converter.
ConvertorBoolToString
_
745	710	845	755
1	0
inputport

0	0	745	740	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	1	EndOfFunc
BEGIN_LINE
1	128	0	0	-1	0
735	695
735	740
END_LINE
outputport

1	3	845	740	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	2	EndOfFunc
BEGIN_LINE
0	91	0	1	-1	0
END_LINE
startport

2	3	745	720	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	0	EndOfFunc
BEGIN_LINE
1	128	2	2	-1	0
740	675
740	720
END_LINE
finishport

3	0	845	720	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	3	EndOfFunc
BEGIN_LINE
0	91	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_modix
_
119	0	0	0
_
_
This is a 2-input integer modulus.
ModulusInt
_
375	675	425	735
1	0
inputport

0	1	375	705	0	0	1	mandatory= 0	0	1
N

Run_ModulusInt	1	EndOfFunc
BEGIN_LINE
1	17	0	0	-1	0
335	365
335	705
END_LINE
inputport

0	1	375	720	1	0	1	mandatory= 0	0	1
D

Run_ModulusInt	2	EndOfFunc
BEGIN_LINE
1	120	0	0	-1	0
385	790
385	740
365	740
365	720
END_LINE
outputport

1	1	425	705	0	0	1	mandatory= 0	0	1


Run_ModulusInt	3	EndOfFunc
BEGIN_LINE
0	117	0	1	-1	0
END_LINE
startport

2	3	375	685	0	0	1	mandatory= 0	0	1


Run_ModulusInt	0	EndOfFunc
BEGIN_LINE
1	17	0	2	-1	0
350	345
350	685
END_LINE
finishport

3	1	425	685	0	0	1	mandatory= 0	0	1


Run_ModulusInt	4	EndOfFunc
BEGIN_LINE
0	117	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
120	0	0	0
_
_
This is an integer constant.
Constant_Int1
_
275	760	375	805
1	0
parameter
Integer Value	1	-2147483648	2147483647	2
EndOfValues
EndOfLabels
integer constant

outputport

1	1	375	790	0	0	1	mandatory= 0	0	1


Run_ConstantInt	1	EndOfFunc
BEGIN_LINE
0	119	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ArrayString1
_
125	0	0	0
_
_
Arrays of string variables can be stored in the component \nThe index must fall between 0 and the maximum size -1. \nNo Error is generated for out of range indexes.\nThe string length matches that if the connected ports.
ArrayString1
_
610	275	710	372
1	0
parameter
size	1	2	1024	10
EndOfValues
EndOfLabels
Memory allocated for array

parameter
default value	3	_	_	a
EndOfValues
EndOfLabels
default value for uninitialised values

inputport

0	1	610	295	0	0	1	mandatory= 0	0	1
index

Run_ArrayStringWrite	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	610	330	1	0	1	mandatory= 0	0	1
index

Run_ArrayStringRead	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	610	305	1	0	1	mandatory= 0	0	1
data

Run_ArrayStringWrite	1	EndOfFunc
BEGIN_LINE
1	19	0	0	-1	1
565	315
565	305
END_LINE
outputport

1	3	710	340	0	0	1	mandatory= 0	0	1
data

Run_ArrayStringRead	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	610	355	0	0	1	mandatory= 0	0	1
clear

Run_ArrayStringClear	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	610	285	0	0	1	mandatory= 0	0	1
write

Run_ArrayStringWrite	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	1
570	295
570	285
END_LINE
startport

2	-1	610	320	2	0	1	mandatory= 0	0	1
read

Run_ArrayStringRead	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	710	355	0	0	1	mandatory= 0	0	1
clear

Run_ArrayStringClear	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	710	285	1	0	1	mandatory= 0	0	1
write

Run_ArrayStringWrite	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	710	320	2	0	1	mandatory= 0	0	1
read

Run_ArrayStringRead	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	710	295	3	0	1	mandatory= 0	0	1
err

Run_ArrayStringWrite	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	710	330	4	0	1	mandatory= 0	0	1
err

Run_ArrayStringRead	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ArrayInt1
_
126	0	0	0
_
_
Arrays of integer variables can be stored in the component\nThe index must fall between 0 and the maximum size -1. No Error is generated for out of range indexes.
ArrayInt1
_
610	400	710	497
1	0
parameter
size	1	2	1024	10
EndOfValues
EndOfLabels
Memory allocated for array

parameter
default value	1	_	_	0
EndOfValues
EndOfLabels
default value for uninitialised values

inputport

0	1	610	420	0	0	1	mandatory= 0	0	1
index

Run_ArrayIntWrite	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	610	455	1	0	1	mandatory= 0	0	1
index

Run_ArrayIntRead	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	610	430	0	0	1	mandatory= 0	0	1
data

Run_ArrayIntWrite	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	710	465	0	0	1	mandatory= 0	0	1
data

Run_ArrayIntRead	2	EndOfFunc
BEGIN_LINE
0	113	0	1	-1	0
END_LINE
startport

2	-1	610	480	0	0	1	mandatory= 0	0	1
clear

Run_ArrayIntClear	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	2	610	410	0	0	1	mandatory= 0	0	1
write

Run_ArrayIntWrite	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	610	445	2	0	1	mandatory= 0	0	1
read

Run_ArrayIntRead	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	2	710	480	0	0	1	mandatory= 0	0	1
clear

Run_ArrayIntClear	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	710	410	1	0	1	mandatory= 0	0	1
write

Run_ArrayIntWrite	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	710	445	0	0	1	mandatory= 0	0	1
read

Run_ArrayIntRead	3	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
finishport

3	2	710	420	3	0	1	mandatory= 0	0	1
err

Run_ArrayIntWrite	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	710	455	4	0	1	mandatory= 0	0	1
err

Run_ArrayIntRead	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ArrayReal1
_
127	0	0	0
_
_
Arrays of real value (floating point) variables can be stored in the component\nThe index must fall between 0 and the maximum size -1. No Error is generated for out of range indexes.
ArrayReal1
_
605	515	705	612
1	0
parameter
size	1	2	1024	10
EndOfValues
EndOfLabels
Memory allocated for array

parameter
default value	2	_	_	NaN
EndOfValues
EndOfLabels
default value for uninitialised values

inputport

0	1	605	535	0	0	1	mandatory= 0	0	1
index

Run_ArrayRealWrite	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	605	570	1	0	1	mandatory= 0	0	1
index

Run_ArrayRealRead	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	605	545	0	0	1	mandatory= 0	0	1
data

Run_ArrayRealWrite	1	EndOfFunc
BEGIN_LINE
1	114	0	0	-1	0
555	550
555	545
END_LINE
outputport

1	2	705	580	0	0	1	mandatory= 0	0	1
data

Run_ArrayRealRead	2	EndOfFunc
BEGIN_LINE
0	115	0	1	-1	0
END_LINE
startport

2	-1	605	595	0	0	1	mandatory= 0	0	1
clear

Run_ArrayRealClear	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	2	605	525	1	0	1	mandatory= 0	0	1
write

Run_ArrayRealWrite	0	EndOfFunc
BEGIN_LINE
1	114	0	2	-1	0
560	530
560	525
END_LINE
startport

2	-1	605	560	2	0	1	mandatory= 0	0	1
read

Run_ArrayRealRead	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	705	595	0	0	1	mandatory= 0	0	1
clear

Run_ArrayRealClear	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	705	525	1	0	1	mandatory= 0	0	1
write

Run_ArrayRealWrite	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	705	560	0	0	1	mandatory= 0	0	1
read

Run_ArrayRealRead	3	EndOfFunc
BEGIN_LINE
0	115	0	3	-1	0
END_LINE
finishport

3	0	705	535	3	0	1	mandatory= 0	0	1
err

Run_ArrayRealWrite	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	705	570	4	0	1	mandatory= 0	0	1
err

Run_ArrayRealRead	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ArrayBool1
_
128	0	0	0
_
_
Arrays of boolean variables can be stored in the component. The index must fall between 0 and the maximum size -1. No Error is generated for out of range indexes.
ArrayBool1
_
605	630	705	727
1	0
parameter
size	1	2	1024	10
EndOfValues
EndOfLabels
Memory allocated for array

parameter
default value	0	_	_	0
EndOfValues
EndOfLabels
default value for uninitialised values

inputport

0	1	605	650	0	0	1	mandatory= 0	0	1
index

Run_ArrayBoolWrite	2	EndOfFunc
BEGIN_LINE
1	17	0	0	-1	0
575	365
575	650
END_LINE
inputport

0	1	605	685	1	0	1	mandatory= 0	0	1
index

Run_ArrayBoolRead	1	EndOfFunc
BEGIN_LINE
1	17	0	0	-1	0
555	365
555	685
END_LINE
inputport

0	0	605	660	0	0	1	mandatory= 0	0	1
data

Run_ArrayBoolWrite	1	EndOfFunc
BEGIN_LINE
1	117	0	0	-1	0
585	705
585	660
END_LINE
outputport

1	0	705	695	0	0	1	mandatory= 0	0	1
data

Run_ArrayBoolRead	2	EndOfFunc
BEGIN_LINE
0	118	0	1	-1	0
END_LINE
startport

2	3	605	710	2	0	1	mandatory= 0	0	1
clear

Run_ArrayBoolClear	0	EndOfFunc
BEGIN_LINE
1	58	5	2	-1	0
1195	690
1195	860
590	860
590	710
END_LINE
startport

2	0	605	640	1	0	1	mandatory= 0	0	1
write

Run_ArrayBoolWrite	0	EndOfFunc
BEGIN_LINE
1	117	0	2	-1	0
580	685
580	640
END_LINE
startport

2	0	605	675	0	0	1	mandatory= 0	0	1
read

Run_ArrayBoolRead	0	EndOfFunc
BEGIN_LINE
1	5	5	2	-1	0
1195	280
1195	175
570	175
570	675
END_LINE
finishport

3	3	705	710	4	0	1	mandatory= 0	0	1
clear

Run_ArrayBoolClear	1	EndOfFunc
BEGIN_LINE
0	58	4	3	-1	0
END_LINE
finishport

3	3	705	640	0	0	1	mandatory= 0	0	1
write

Run_ArrayBoolWrite	3	EndOfFunc
BEGIN_LINE
0	44	0	3	-1	0
END_LINE
finishport

3	0	705	675	0	0	1	mandatory= 0	0	1
read

Run_ArrayBoolRead	3	EndOfFunc
BEGIN_LINE
0	118	0	3	-1	0
END_LINE
finishport

3	3	705	650	3	0	1	mandatory= 0	0	1
err

Run_ArrayBoolWrite	4	EndOfFunc
BEGIN_LINE
0	44	1	3	-1	0
END_LINE
finishport

3	3	705	685	4	0	1	mandatory= 0	0	1
err

Run_ArrayBoolRead	4	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
