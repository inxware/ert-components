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
1	75	0	0	-1	0
940	375
940	300
END_LINE
inputport

0	3	950	335	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	15	0	0	-1	0
940	265
940	335
END_LINE
outputport

1	3	1000	305	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	5	5	1	-1	0
END_LINE
startport

2	3	950	285	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	75	0	2	-1	0
945	310
945	285
END_LINE
startport

2	3	950	320	1	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	75	1	2	-1	0
935	320
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
285	335	335	380
1	0
outputport

1	1	335	365	0	0	1	mandatory= 0	0	1


Count	1	Reset	1	EndOfFunc
BEGIN_LINE
0	18	0	1	-1	0
END_LINE
startport

2	0	285	345	1	0	1	mandatory= 0	0	1


Count	0	EndOfFunc
BEGIN_LINE
1	31	5	2	-1	0
1205	475
1205	840
275	840
275	345
END_LINE
startport

2	-1	285	365	1	0	1	mandatory= 0	0	1
rst

Reset	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	335	345	0	0	1	mandatory= 0	0	1


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

0	1	375	315	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	1	EndOfFunc
BEGIN_LINE
1	17	0	0	-1	0
335	365
335	315
END_LINE
outputport

1	3	475	315	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	2	EndOfFunc
BEGIN_LINE
0	19	0	1	-1	0
END_LINE
startport

2	3	375	295	0	0	1	mandatory= 0	0	1


Run_ConvertorIntToString	0	EndOfFunc
BEGIN_LINE
1	17	0	2	-1	0
335	345
335	295
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
940	220
940	495
END_LINE
inputport

0	3	950	530	5	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	15	0	0	-1	0
940	265
940	530
END_LINE
outputport

1	3	1000	500	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	31	5	1	-1	0
END_LINE
startport

2	3	950	480	1	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	75	2	2	-1	0
945	335
945	480
END_LINE
startport

2	-1	950	515	3	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	75	3	2	-1	1
935	345
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

buffer_fifossq
_
75	0	0	0
_
_
This is a string fifo buffer.
FIFO_String
_
595	300	695	389
1	0
parameter
Max. length	1	0	1000000	5
EndOfValues
EndOfLabels
maximum buffer length

inputport

0	3	595	375	1	0	1	mandatory= 0	0	1
data

Run_PushFIFOString	1	EndOfFunc
BEGIN_LINE
1	19	0	0	-1	1
550	315
550	375
END_LINE
outputport

1	3	695	375	0	0	1	mandatory= 0	0	1
data

Run_PopFIFOString	1	EndOfFunc
BEGIN_LINE
0	91	0	1	-1	0
END_LINE
startport

2	0	595	310	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOString	0	EndOfFunc
BEGIN_LINE
1	5	5	2	-1	0
1195	280
1195	180
580	180
580	310
END_LINE
startport

2	-1	595	335	0	0	1	mandatory= 0	0	1
push

Run_PushFIFOString	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	1
590	295
590	335
END_LINE
startport

2	3	595	360	2	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOString	0	EndOfFunc
BEGIN_LINE
1	58	5	2	-1	0
1195	690
1195	765
580	765
580	360
END_LINE
finishport

3	3	695	310	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOString	2	EndOfFunc
BEGIN_LINE
0	7	0	3	-1	0
END_LINE
finishport

3	3	695	320	1	0	1	mandatory= 0	0	1
err

Run_PopFIFOString	3	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
finishport

3	0	695	335	0	0	1	mandatory= 0	0	1
push

Run_PushFIFOString	2	EndOfFunc
BEGIN_LINE
0	91	0	3	-1	0
END_LINE
finishport

3	3	695	345	3	0	1	mandatory= 0	0	1
err

Run_PushFIFOString	3	EndOfFunc
BEGIN_LINE
0	44	1	3	-1	0
END_LINE
finishport

3	3	695	360	4	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOString	1	EndOfFunc
BEGIN_LINE
0	58	4	3	-1	0
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
935	220
935	720
END_LINE
inputport

0	3	945	755	0	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	108	0	0	-1	0
935	815
935	755
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
1	75	2	2	-1	0
940	335
940	705
END_LINE
startport

2	3	945	740	0	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	75	4	2	-1	0
930	360
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

buffer_fifoiiq
_
110	0	0	0
_
_
This is an integer fifo buffer.
FIFO_Int
_
595	400	695	489
1	0
parameter
Max. length	1	0	1000000	5
EndOfValues
EndOfLabels
maximum buffer length

inputport

0	1	595	475	0	0	1	mandatory= 0	0	1
data

Run_PushFIFOInt	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	695	475	0	0	1	mandatory= 0	0	1
data

Run_PopFIFOInt	1	EndOfFunc
BEGIN_LINE
0	113	0	1	-1	0
END_LINE
startport

2	1	595	410	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	595	435	1	0	1	mandatory= 0	0	1
push

Run_PushFIFOInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	595	460	2	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	695	410	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOInt	2	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
finishport

3	1	695	420	1	0	1	mandatory= 0	0	1
err

Run_PopFIFOInt	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	695	435	2	0	1	mandatory= 0	0	1
push

Run_PushFIFOInt	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	695	445	3	0	1	mandatory= 0	0	1
err

Run_PushFIFOInt	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	695	460	4	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOInt	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

buffer_fiforrq
_
111	0	0	0
_
_
This is a real fifo buffer.
FIFO_Real
_
595	520	695	609
1	0
parameter
Max. length	1	0	1000000	5
EndOfValues
EndOfLabels
maximum buffer length

inputport

0	2	595	595	0	0	1	mandatory= 0	0	1
data

Run_PushFIFOReal	1	EndOfFunc
BEGIN_LINE
1	114	0	0	-1	0
555	550
555	595
END_LINE
outputport

1	2	695	595	0	0	1	mandatory= 0	0	1
data

Run_PopFIFOReal	1	EndOfFunc
BEGIN_LINE
0	115	0	1	-1	0
END_LINE
startport

2	2	595	530	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOReal	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	2	595	555	1	0	1	mandatory= 0	0	1
push

Run_PushFIFOReal	0	EndOfFunc
BEGIN_LINE
1	114	0	2	-1	0
560	530
560	555
END_LINE
startport

2	2	595	580	2	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOReal	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	2	695	530	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOReal	2	EndOfFunc
BEGIN_LINE
0	115	0	3	-1	0
END_LINE
finishport

3	2	695	540	1	0	1	mandatory= 0	0	1
err

Run_PopFIFOReal	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	695	555	2	0	1	mandatory= 0	0	1
push

Run_PushFIFOReal	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	695	565	3	0	1	mandatory= 0	0	1
err

Run_PushFIFOReal	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	695	580	4	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOReal	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

buffer_fifobbq
_
112	0	0	0
_
_
This is a boolean fifo buffer.
FIFO_Bool
_
595	635	695	724
1	0
parameter
Max. length	1	0	1000000	5
EndOfValues
EndOfLabels
maximum buffer length

inputport

0	0	595	710	0	0	1	mandatory= 0	0	1
data

Run_PushFIFOBool	1	EndOfFunc
BEGIN_LINE
1	117	0	0	-1	0
585	705
585	710
END_LINE
outputport

1	0	695	710	0	0	1	mandatory= 0	0	1
data

Run_PopFIFOBool	1	EndOfFunc
BEGIN_LINE
0	118	0	1	-1	0
END_LINE
startport

2	0	595	645	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	595	670	1	0	1	mandatory= 0	0	1
push

Run_PushFIFOBool	0	EndOfFunc
BEGIN_LINE
1	117	0	2	-1	0
580	685
580	670
END_LINE
startport

2	0	595	695	2	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	695	645	0	0	1	mandatory= 0	0	1
pop

Run_PopFIFOBool	2	EndOfFunc
BEGIN_LINE
0	118	0	3	-1	0
END_LINE
finishport

3	0	695	655	1	0	1	mandatory= 0	0	1
err

Run_PopFIFOBool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	695	670	2	0	1	mandatory= 0	0	1
push

Run_PushFIFOBool	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	695	680	3	0	1	mandatory= 0	0	1
err

Run_PushFIFOBool	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	695	695	4	0	1	mandatory= 0	0	1
flush

Run_FlushFIFOBool	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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
1	110	0	0	-1	0
715	475
715	510
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
1	110	0	2	-1	1
745	410
745	490
END_LINE
finishport

3	1	850	490	0	0	1	mandatory= 0	0	1


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

2	2	380	530	0	0	1	mandatory= 0	0	1


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
1	111	0	0	-1	0
745	595
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
1	111	0	2	-1	0
750	530
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
1	112	0	0	-1	0
735	710
735	740
END_LINE
outputport

1	3	845	740	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	745	720	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	0	EndOfFunc
BEGIN_LINE
1	112	0	2	-1	0
740	645
740	720
END_LINE
finishport

3	3	845	720	0	0	1	mandatory= 0	0	1


Run_ConvertorBoolToString	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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
0	-1	0	0	-1	0
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

2	1	375	685	0	0	1	mandatory= 0	0	1


Run_ModulusInt	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
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
END_OF_BLOCKS
