#V:2.1.2

CanvasSizeXY
2341	1712

IconData
BEGIN_BLOCK

file_ros
_
115335	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-470	-880	-405	-760
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_RO
trans	-1	0	0	0
offset	0	-15
hash	0x560C
Instance_Info	_
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	calib.json
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Use the form (x) where x is the delimiter

parameter
Use Application Dir	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	coords= -471 -845 0	0	0	1	mandatory= 0	0	1
path
load_json_settings_file
funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
1	116630	0	0	-1	0
-486	-415
-486	-845
END_LINE
outputport

1	3	coords= -405 -780 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	116041	0	1	-1	0
END_LINE
outputport

1	1	coords= -405 -770 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -471 -855 0	0	0	0	mandatory= 0	0	1
open
load_json_settings
funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116630	0	2	-1	0
-486	-425
-486	-855
END_LINE
startport

2	3	coords= -471 -830 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	115336	0	2	-1	0
-275	-851
-275	-905
-505	-905
-505	-830
END_LINE
startport

2	3	coords= -471 -805 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	115335	0	2	-1	0
-360	-855
-360	-905
-505	-905
-505	-805
END_LINE
finishport

3	3	coords= -405 -855 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	2	3	-1	0
END_LINE
finishport

3	3	coords= -405 -830 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -405 -805 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	116041	0	3	-1	0
END_LINE
finishport

3	1	coords= -405 -845 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	115336	0	3	-1	0
END_LINE
finishport

3	1	coords= -405 -820 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -405 -795 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	115336	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
115336	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-310	-885	-290	-820
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -311 -875 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	115335	3	2	-1	0
-350	-845
-350	-875
END_LINE
startport

2	-1	coords= -311 -860 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	115335	5	2	-1	0
-345	-795
-345	-860
END_LINE
startport

2	-1	coords= -311 -845 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116041	0	2	-1	0
-10	-735
-10	-780
-335	-780
-335	-845
END_LINE
startport

2	-1	coords= -311 -830 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -298 -851 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	115335	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115568	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-1120	10	-1099
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	p1s
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -1109 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115569	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-1090	10	-1069
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	p1o
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -1079 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115570	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-1060	10	-1039
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	p2s
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -1049 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115571	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-930	10	-909
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	z1s
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -919 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115572	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-900	10	-879
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	z1o
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -889 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115573	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-870	10	-849
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	f1s
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -859 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115574	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-840	10	-819
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	f1o
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -829 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115575	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-1030	10	-1009
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	p2o
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -1019 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	5	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115576	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-1000	10	-979
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	t1s
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -989 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	6	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115577	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-970	10	-949
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	t1o
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -959 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	7	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_stream
_
116041	0	0	0
_
_
Processes a text stream and generates events related to JSON parsing.
json_stream_parser
0
_
-160	-760	-55	-525
1	0
blockattr
BEGIN_BA
type	Undefined
text	5	5	1.5	0	JSON_Parser
trans	-1	0	0	0
offset	0	-15
hash	0xDCB6
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -161 -725 0	0	0	1	mandatory= 0	0	1
data

funcName= read 1	1	EndOfFunc
BEGIN_LINE
1	115335	0	0	-1	0
-370	-780
-370	-725
END_LINE
outputport

1	3	coords= -55 -725 0	0	0	1	mandatory= 0	0	1
--

funcName= read 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -55 -595 0	1	0	1	mandatory= 0	0	1
dataType

funcName= parse 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -55 -585 0	2	0	1	mandatory= 0	0	1
data

funcName= parse 2	2	EndOfFunc
BEGIN_LINE
0	116894	0	1	-1	0
END_LINE
outputport

1	3	coords= -55 -635 0	3	0	1	mandatory= 0	0	1
Key
json_key_str
funcName= parse 2	3	EndOfFunc
BEGIN_LINE
0	117823	0	1	-1	0
END_LINE
outputport

1	3	coords= -55 -625 0	4	0	1	mandatory= 0	0	1
parent

funcName= parse 2	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -55 -555 0	5	0	1	mandatory= 0	0	1
--

funcName= parse 2	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= -161 -735 0	0	0	0	mandatory= 0	0	1
read

funcName= read 1	0	EndOfFunc
BEGIN_LINE
1	115335	2	2	-1	0
-360	-805
-360	-735
END_LINE
startport

2	-1	coords= -161 -705 0	1	0	0	mandatory= 0	0	1
next

funcName= parse 2	0	EndOfFunc
BEGIN_LINE
1	116044	0	2	-1	0
-180	-626
-180	-705
END_LINE
startport

2	-1	coords= -161 -535 0	2	0	0	mandatory= 0	0	1
eos

funcName= eos 3	0	EndOfFunc
BEGIN_LINE
1	116043	0	2	-1	0
-175	-559
-175	-535
END_LINE
finishport

3	3	coords= -55 -735 0	0	0	1	mandatory= 0	0	1
--

funcName= read 1	3	EndOfFunc
BEGIN_LINE
0	115336	2	3	-1	0
END_LINE
finishport

3	3	coords= -55 -705 0	1	0	1	mandatory= 0	0	1
--

funcName= parse 2	6	EndOfFunc
BEGIN_LINE
0	94414	1	3	-1	0
END_LINE
finishport

3	3	coords= -55 -535 0	2	0	1	mandatory= 0	0	1
--
loading_json_done
funcName= eos 3	1	EndOfFunc
BEGIN_LINE
0	118812	1	3	-1	0
END_LINE
finishport

3	3	coords= -55 -695 0	6	0	1	mandatory= 0	0	1
startMap

funcName= parse 2	7	EndOfFunc
BEGIN_LINE
0	94419	0	3	-1	0
END_LINE
finishport

3	3	coords= -55 -685 0	4	0	1	mandatory= 0	0	1
endMap

funcName= parse 2	8	EndOfFunc
BEGIN_LINE
0	94413	2	3	-1	0
END_LINE
finishport

3	3	coords= -55 -645 0	5	0	1	mandatory= 0	0	1
mapKey

funcName= parse 2	9	EndOfFunc
BEGIN_LINE
0	94421	3	3	-1	0
END_LINE
finishport

3	3	coords= -55 -675 0	3	0	1	mandatory= 0	0	1
startArray

funcName= parse 2	10	EndOfFunc
BEGIN_LINE
0	94414	4	3	-1	0
END_LINE
finishport

3	3	coords= -55 -665 0	7	0	1	mandatory= 0	0	1
endArray

funcName= parse 2	11	EndOfFunc
BEGIN_LINE
0	94414	8	3	-1	0
END_LINE
finishport

3	2	coords= -55 -605 0	0	0	1	mandatory= 0	0	1
value

funcName= parse 2	12	EndOfFunc
BEGIN_LINE
0	116894	0	3	-1	0
END_LINE
finishport

3	3	coords= -55 -565 0	9	0	1	mandatory= 0	0	1
error

funcName= parse 2	13	EndOfFunc
BEGIN_LINE
0	94419	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
116043	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-205	-580	-185	-540
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_2
trans	-1	0	0	0
offset	0	0
hash	0x0A91
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -206 -570 0	1	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116041	9	2	-1	0
-10	-565
-10	-510
-250	-510
-250	-570
END_LINE
startport

2	3	coords= -206 -550 0	2	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116041	4	2	-1	0
-10	-685
-10	-510
-250	-510
-250	-550
END_LINE
finishport

3	-1	coords= -193 -559 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	94413	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
116044	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-205	-660	-185	-595
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -206 -650 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116041	0	2	-1	0
-10	-735
-10	-780
-250	-780
-250	-650
END_LINE
startport

2	-1	coords= -206 -635 0	0	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116041	3	2	-1	0
-10	-695
-10	-780
-250	-780
-250	-635
END_LINE
startport

2	3	coords= -206 -620 0	1	0	0	mandatory= 0	0	1

json_value_ready
funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	119819	0	2	-1	0
165	-555
165	-10
-256	-10
-256	-620
END_LINE
startport

2	-1	coords= -206 -605 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	116041	5	2	-1	0
-10	-645
-10	-780
-250	-780
-250	-605
END_LINE
finishport

3	-1	coords= -193 -626 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	94413	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
116045	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
130	-995	160	-810
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	0	5	1.25	0	TxSw
trans	-1	0	0	0
offset	0	-15
hash	0xCF87
Instance_Info	_
End_Instance
END_BA
parameter
case 1	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	coords= 129 -960 0	0	0	1	mandatory= 0	0	1
test
json_key_str
funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	116041	3	0	-1	0
42	-635
42	-960
END_LINE
inputport

0	3	coords= 129 -940 0	2	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	115568	0	0	-1	0
74	-1109
74	-940
END_LINE
inputport

0	3	coords= 129 -925 0	3	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	115569	0	0	-1	0
74	-1079
74	-925
END_LINE
inputport

0	3	coords= 129 -910 0	4	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	115570	0	0	-1	0
74	-1049
74	-910
END_LINE
inputport

0	3	coords= 129 -895 0	5	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	115575	0	0	-1	0
74	-1019
74	-895
END_LINE
inputport

0	3	coords= 129 -880 0	6	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
1	115576	0	0	-1	0
74	-989
74	-880
END_LINE
inputport

0	3	coords= 129 -865 0	7	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
1	115577	0	0	-1	0
74	-959
74	-865
END_LINE
inputport

0	3	coords= 129 -850 0	1	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
1	115571	0	0	-1	0
74	-919
74	-850
END_LINE
inputport

0	3	coords= 129 -835 0	2	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
1	115572	0	0	-1	0
74	-889
74	-835
END_LINE
startport

2	3	coords= 129 -970 0	0	0	0	mandatory= 0	0	1
go
json_value_ready
funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	119819	0	2	-1	0
127	-555
127	-970
END_LINE
finishport

3	3	coords= 160 -940 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	119817	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -925 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	119817	1	3	-1	0
END_LINE
finishport

3	3	coords= 160 -910 0	2	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	119817	2	3	-1	0
END_LINE
finishport

3	3	coords= 160 -895 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	119817	3	3	-1	0
END_LINE
finishport

3	3	coords= 160 -880 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	119818	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -865 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	119818	1	3	-1	0
END_LINE
finishport

3	3	coords= 160 -850 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	119818	2	3	-1	0
END_LINE
finishport

3	3	coords= 160 -835 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	119818	3	3	-1	0
END_LINE
finishport

3	3	coords= 160 -820 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
116046	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
130	-790	160	-605
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	0	5	1.25	0	TxSw
trans	-1	0	0	0
offset	0	-15
hash	0xCF87
Instance_Info	_
End_Instance
END_BA
parameter
case 1	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	coords= 129 -755 0	0	0	1	mandatory= 0	0	1
test
json_key_str
funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	116041	3	0	-1	0
42	-635
42	-755
END_LINE
inputport

0	3	coords= 129 -735 0	3	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	115573	0	0	-1	0
74	-859
74	-735
END_LINE
inputport

0	3	coords= 129 -720 0	4	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	115574	0	0	-1	0
74	-829
74	-720
END_LINE
inputport

0	3	coords= 129 -705 0	1	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	117595	0	0	-1	0
69	-449
69	-705
END_LINE
inputport

0	3	coords= 129 -690 0	8	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	117596	0	0	-1	0
69	-419
69	-690
END_LINE
inputport

0	3	coords= 129 -675 0	5	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
1	117597	0	0	-1	0
69	-389
69	-675
END_LINE
inputport

0	3	coords= 129 -660 0	6	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
1	117602	0	0	-1	0
69	-359
69	-660
END_LINE
inputport

0	3	coords= 129 -645 0	7	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
1	117603	0	0	-1	0
69	-329
69	-645
END_LINE
inputport

0	3	coords= 129 -630 0	8	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
1	117604	0	0	-1	0
69	-299
69	-630
END_LINE
startport

2	3	coords= 129 -765 0	0	0	0	mandatory= 0	0	1
go
json_value_ready
funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	119819	0	2	-1	0
119	-555
119	-765
END_LINE
finishport

3	3	coords= 160 -735 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	119825	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -720 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	119825	1	3	-1	0
END_LINE
finishport

3	3	coords= 160 -705 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	119820	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -690 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	119820	1	3	-1	0
END_LINE
finishport

3	3	coords= 160 -675 0	2	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	119820	2	3	-1	0
END_LINE
finishport

3	3	coords= 160 -660 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	119820	3	3	-1	0
END_LINE
finishport

3	3	coords= 160 -645 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	119821	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -630 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	119821	1	3	-1	0
END_LINE
finishport

3	3	coords= 160 -615 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wos
_
116268	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, <nl?> for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii format data for the value prepended with zeros. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.
FILE_WriteOnly_String
0
_
-375	-1420	-310	-1300
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_WO
trans	-1	0	0	0
offset	0	-15
hash	0xBBA0
Instance_Info	_
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Data Field Width (space padded)

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	coords= -376 -1385 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_WriteOnly_String 1	1	EndOfFunc
BEGIN_LINE
1	116271	0	0	-1	0
-443	-1385
-433	-1385
END_LINE
inputport

0	3	coords= -376 -1320 0	1	0	1	mandatory= 0	0	1
data
write_results_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	118815	0	0	-1	0
915	-800
915	-30
-426	-30
-426	-1320
END_LINE
outputport

1	1	coords= -310 -1320 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -376 -1395 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_WriteOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116271	0	2	-1	0
-443	-1395
-433	-1395
END_LINE
startport

2	3	coords= -376 -1370 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	116274	0	2	-1	0
-410	-1350
-410	-1370
END_LINE
startport

2	3	coords= -376 -1345 0	2	0	0	mandatory= 0	0	1
write
write_results
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	118815	0	2	-1	0
915	-810
915	-10
-426	-10
-426	-1345
END_LINE
finishport

3	1	coords= -310 -1395 0	3	0	1	mandatory= 0	0	1
--
start_test
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	116315	0	3	-1	0
END_LINE
finishport

3	3	coords= -310 -1370 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_WriteOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -310 -1345 0	2	0	1	mandatory= 0	0	1
--
write_done
funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	118959	0	3	-1	0
END_LINE
finishport

3	3	coords= -310 -1385 0	0	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_WriteOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	6	1	3	-1	0
END_LINE
finishport

3	1	coords= -310 -1360 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_WriteOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	7	0	3	-1	0
END_LINE
finishport

3	1	coords= -310 -1335 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

fs_dir_create_remove
_
116269	0	0	0
_
_
Allows a directory to be created or removed. Parametes available to allow complete removal of internal data too.
FileSystemDirCreateRemove
0
_
-590	-1545	-525	-1473
1	0
blockattr
BEGIN_BA
type	IO
text	5	5	1.25	0	FS_Cr/Rm
trans	-1	0	0	0
offset	0	-15
hash	0x7AE4
Instance_Info	_
End_Instance
END_BA
parameter
Directory	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
Media Directory

parameter
EmptyOnly	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Remove contents only

inputport

0	3	coords= -591 -1510 0	0	0	1	mandatory= 0	0	1
path

funcName= create 1	1	funcName= remove 2	1	EndOfFunc
BEGIN_LINE
1	116270	0	0	-1	0
-648	-1519
-638	-1510
END_LINE
startport

2	0	coords= -591 -1520 0	0	1	0	mandatory= 0	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -591 -1493 0	1	0	0	mandatory= 0	0	1
remove

funcName= remove 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -525 -1520 0	0	0	1	mandatory= 0	0	1


funcName= create 1	2	EndOfFunc
BEGIN_LINE
0	5	0	3	-1	0
END_LINE
finishport

3	3	coords= -525 -1510 0	1	0	1	mandatory= 0	0	1
err

funcName= create 1	3	EndOfFunc
BEGIN_LINE
0	6	0	3	-1	0
END_LINE
finishport

3	3	coords= -525 -1493 0	2	0	1	mandatory= 0	0	1


funcName= remove 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -525 -1483 0	3	0	1	mandatory= 0	0	1
err

funcName= remove 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
116270	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-835	-1530	-695	-1509
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	inx-tests/results
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -695 -1519 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	20	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
116271	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
-550	-1410	-520	-1365
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	/test_result.txt
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= -551 -1385 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	116270	0	0	-1	0
-650	-1519
-650	-1385
END_LINE
inputport

0	3	coords= -551 -1375 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -520 -1385 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
startport

2	3	coords= -551 -1395 0	0	0	0	mandatory= 0	0	1


funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	116269	0	2	-1	0
-480	-1520
-480	-1450
-606	-1450
-606	-1395
END_LINE
finishport

3	3	coords= -520 -1395 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor3
_
116272	0	0	0
_
_
This is a 3-input event OR.
EventThreeInputOr
0
_
-205	-1520	-185	-1470
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_3
trans	-1	0	0	0
offset	0	0
hash	0xBA97
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -206 -1510 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116269	1	2	-1	0
-331	-1510
-331	-1510
END_LINE
startport

2	-1	coords= -206 -1495 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116268	3	2	-1	0
-250	-1385
-250	-1495
END_LINE
startport

2	-1	coords= -206 -1480 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116268	5	2	-1	0
-235	-1335
-235	-1480
END_LINE
finishport

3	-1	coords= -193 -1495 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
116273	0	0	0
_
_
<nl?>        
xcomment
0
_
-445	-1285	-147	-1250
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Write all test results to this file once it opens.
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

trigger_eventor3
_
116274	0	0	0
_
_
This is a 3-input event OR.
EventThreeInputOr
0
_
-445	-1375	-425	-1325
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_3
trans	-1	0	0	0
offset	0	0
hash	0xBA97
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -446 -1365 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116268	4	2	-1	0
-265	-1360
-265	-1435
-480	-1435
-480	-1365
END_LINE
startport

2	-1	coords= -446 -1350 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116268	5	2	-1	0
-265	-1335
-265	-1435
-480	-1435
-480	-1350
END_LINE
startport

2	-1	coords= -446 -1335 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116277	0	2	-1	0
-460	-1301
-460	-1335
END_LINE
finishport

3	-1	coords= -433 -1350 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wos
_
116275	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, <nl?> for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii format data for the value prepended with zeros. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.
FILE_WriteOnly_String
0
_
-625	-1350	-560	-1230
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_WO
trans	-1	0	0	0
offset	0	-15
hash	0xBBA0
Instance_Info	_
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Data Field Width (space padded)

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	coords= -626 -1315 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_WriteOnly_String 1	1	EndOfFunc
BEGIN_LINE
1	116279	0	0	-1	0
-670	-1340
-670	-1315
END_LINE
inputport

0	3	coords= -626 -1250 0	1	0	1	mandatory= 0	0	1
data

funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	116276	0	0	-1	0
-658	-1279
-658	-1250
END_LINE
outputport

1	1	coords= -560 -1250 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -626 -1325 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_WriteOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116279	0	2	-1	0
-663	-1350
-663	-1325
END_LINE
startport

2	3	coords= -626 -1300 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	116275	2	2	-1	0
-530	-1275
-530	-1355
-655	-1355
-655	-1300
END_LINE
startport

2	3	coords= -626 -1275 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	116275	0	2	-1	0
-540	-1325
-540	-1225
-655	-1225
-655	-1275
END_LINE
finishport

3	3	coords= -560 -1325 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	20	2	3	-1	0
END_LINE
finishport

3	3	coords= -560 -1300 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_WriteOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	22	1	3	-1	0
END_LINE
finishport

3	1	coords= -560 -1275 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	20	1	3	-1	0
END_LINE
finishport

3	1	coords= -560 -1315 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_WriteOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
END_LINE
finishport

3	1	coords= -560 -1290 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_WriteOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	22	2	3	-1	0
END_LINE
finishport

3	1	coords= -560 -1265 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	22	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
116276	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-825	-1290	-685	-1269
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	done
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -685 -1279 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	20	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
116277	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-495	-1335	-475	-1270
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -496 -1325 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116275	3	2	-1	0
-523	-1315
-513	-1325
END_LINE
startport

2	-1	coords= -496 -1310 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116275	1	2	-1	0
-523	-1300
-513	-1310
END_LINE
startport

2	-1	coords= -496 -1295 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116275	4	2	-1	0
-523	-1290
-513	-1295
END_LINE
startport

2	-1	coords= -496 -1280 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	116275	5	2	-1	0
-523	-1265
-513	-1280
END_LINE
finishport

3	-1	coords= -483 -1301 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
116278	0	0	0
_
_
<nl?>        
xcomment
0
_
-860	-1215	-597	-1178
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Create a flag file once your test is done
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
116279	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
-720	-1365	-690	-1320
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	done flag
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	/test_done
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= -721 -1340 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	116270	0	0	-1	0
-650	-1519
-650	-1385
-776	-1385
-776	-1340
END_LINE
inputport

0	3	coords= -721 -1330 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -690 -1340 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	20	0	1	-1	0
END_LINE
startport

2	3	coords= -721 -1350 0	0	0	0	mandatory= 0	0	1

test_done
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	118960	0	2	-1	0
-1025	-65
-1025	-1350
END_LINE
finishport

3	3	coords= -690 -1350 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
116280	0	0	0
_
_
<nl?>        
xcomment
0
_
-1155	-1080	-988	-1017
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Read json files as if they \ncame from the server.
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

file_ros
_
116315	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-1260	-940	-1195	-820
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_RO
trans	-1	0	0	0
offset	0	-15
hash	0x560C
Instance_Info	calibration.dat
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	calibration.dat
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Use the form (x) where x is the delimiter

parameter
Use Application Dir	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	coords= -1261 -905 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -1195 -840 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	116481	0	1	-1	0
END_LINE
outputport

1	1	coords= -1195 -830 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1261 -915 0	0	0	0	mandatory= 0	0	1
open
start_test
funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116268	0	2	-1	0
-260	-1395
-260	-10
-1311	-10
-1311	-915
END_LINE
startport

2	3	coords= -1261 -890 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	116316	0	2	-1	0
-1100	-956
-1100	-1005
-1285	-1005
-1285	-890
END_LINE
startport

2	3	coords= -1261 -865 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	116315	0	2	-1	0
-1170	-915
-1170	-970
-1285	-970
-1285	-865
END_LINE
finishport

3	3	coords= -1195 -915 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	2	3	-1	0
END_LINE
finishport

3	3	coords= -1195 -890 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	116629	0	3	-1	0
END_LINE
finishport

3	1	coords= -1195 -865 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	116481	0	3	-1	0
END_LINE
finishport

3	1	coords= -1195 -905 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	115336	0	3	-1	0
END_LINE
finishport

3	1	coords= -1195 -880 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -1195 -855 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	115336	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
116316	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-1135	-990	-1115	-925
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1136 -980 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116315	3	2	-1	0
-1170	-905
-1170	-980
END_LINE
startport

2	-1	coords= -1136 -965 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116315	5	2	-1	0
-1165	-855
-1165	-965
END_LINE
startport

2	-1	coords= -1136 -950 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116315	2	2	-1	0
-1155	-865
-1155	-950
END_LINE
startport

2	-1	coords= -1136 -935 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -1123 -956 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	115335	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wos
_
116318	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, <nl?> for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii format data for the value prepended with zeros. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.
FILE_WriteOnly_String
0
_
-560	-635	-495	-515
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_WO
trans	-1	0	0	0
offset	0	-15
hash	0xBBA0
Instance_Info	_
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Data Field Width (space padded)

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	coords= -561 -600 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_WriteOnly_String 1	1	EndOfFunc
BEGIN_LINE
1	116526	0	0	-1	0
-601	-600
-591	-600
END_LINE
inputport

0	3	coords= -561 -535 0	1	0	1	mandatory= 0	0	1
data

funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	116481	0	0	-1	0
-740	-840
-740	-535
END_LINE
outputport

1	1	coords= -495 -535 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -561 -610 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_WriteOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116526	0	2	-1	0
-611	-610
-601	-610
END_LINE
startport

2	3	coords= -561 -585 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	116318	2	2	-1	0
-460	-560
-460	-655
-595	-655
-595	-585
END_LINE
startport

2	3	coords= -561 -560 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	116318	0	2	-1	0
-460	-610
-460	-655
-595	-655
-595	-560
END_LINE
finishport

3	3	coords= -495 -610 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	116318	2	3	-1	0
END_LINE
finishport

3	3	coords= -495 -585 0	1	0	1	mandatory= 0	0	1
--
json_save_done
funcName= Run_CloseFILE_WriteOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	116630	0	3	-1	0
END_LINE
finishport

3	1	coords= -495 -560 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	116318	1	3	-1	0
END_LINE
finishport

3	1	coords= -495 -600 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_WriteOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	116627	0	3	-1	0
END_LINE
finishport

3	1	coords= -495 -575 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_WriteOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	116627	1	3	-1	0
END_LINE
finishport

3	1	coords= -495 -550 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	116627	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_finds
_
116319	0	0	0
_
_
This function determines if a string s2 exists in string s1.
FindString
0
_
-895	-970	-840	-920
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	5	1.25	0	find_str
trans	-1	0	0	0
offset	0	0
hash	0x7D98
Instance_Info	is calib?
End_Instance
END_BA
parameter
Reverse Search	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Biases search for backward searching

parameter
Find String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= -896 -950 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_FindString 1	1	EndOfFunc
BEGIN_LINE
1	116481	0	0	-1	0
-955	-840
-955	-950
END_LINE
inputport

0	3	coords= -896 -940 0	1	0	1	mandatory= 0	0	1
s2
is_calib
funcName= Run_FindString 1	2	EndOfFunc
BEGIN_LINE
1	117495	0	0	-1	0
-971	-1384
-971	-940
END_LINE
outputport

1	3	coords= -840 -950 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -840 -940 0	1	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	4	EndOfFunc
BEGIN_LINE
0	116478	0	1	-1	0
END_LINE
outputport

1	1	coords= -840 -930 0	2	0	1	mandatory= 0	0	1
index

funcName= Run_FindString 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -896 -960 0	0	0	0	mandatory= 0	0	1


funcName= Run_FindString 1	0	EndOfFunc
BEGIN_LINE
1	116481	0	2	-1	0
-970	-850
-970	-960
END_LINE
finishport

3	0	coords= -840 -960 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	6	EndOfFunc
BEGIN_LINE
0	116478	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_finds
_
116356	0	0	0
_
_
This function determines if a string s2 exists in string s1.
FindString
0
_
-895	-885	-840	-835
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	5	1.25	0	find_str
trans	-1	0	0	0
offset	0	0
hash	0x7D98
Instance_Info	is hrdw?
End_Instance
END_BA
parameter
Reverse Search	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Biases search for backward searching

parameter
Find String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= -896 -865 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_FindString 1	1	EndOfFunc
BEGIN_LINE
1	116481	0	0	-1	0
-941	-840
-941	-865
END_LINE
inputport

0	3	coords= -896 -855 0	1	0	1	mandatory= 0	0	1
s2
is_hrdw
funcName= Run_FindString 1	2	EndOfFunc
BEGIN_LINE
1	117496	0	0	-1	0
-971	-1349
-971	-855
END_LINE
outputport

1	3	coords= -840 -865 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -840 -855 0	1	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	4	EndOfFunc
BEGIN_LINE
0	116436	0	1	-1	0
END_LINE
outputport

1	1	coords= -840 -845 0	2	0	1	mandatory= 0	0	1
index

funcName= Run_FindString 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -896 -875 0	0	0	0	mandatory= 0	0	1


funcName= Run_FindString 1	0	EndOfFunc
BEGIN_LINE
1	116478	1	2	-1	0
-750	-950
-750	-900
-930	-900
-930	-875
END_LINE
finishport

3	0	coords= -840 -875 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	6	EndOfFunc
BEGIN_LINE
0	116436	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_finds
_
116395	0	0	0
_
_
This function determines if a string s2 exists in string s1.
FindString
0
_
-895	-800	-840	-750
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	5	1.25	0	find_str
trans	-1	0	0	0
offset	0	0
hash	0x7D98
Instance_Info	is ctrl1?
End_Instance
END_BA
parameter
Reverse Search	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Biases search for backward searching

parameter
Find String (-> s2)	3	0	0	0	0	0	0	1.2	"id":1
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= -896 -780 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_FindString 1	1	EndOfFunc
BEGIN_LINE
1	116481	0	0	-1	0
-951	-840
-951	-780
END_LINE
inputport

0	3	coords= -896 -770 0	1	0	1	mandatory= 0	0	1
s2
is_ctrl1
funcName= Run_FindString 1	2	EndOfFunc
BEGIN_LINE
1	117497	0	0	-1	0
-971	-1314
-971	-770
END_LINE
outputport

1	3	coords= -840 -780 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -840 -770 0	1	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	4	EndOfFunc
BEGIN_LINE
0	116396	0	1	-1	0
END_LINE
outputport

1	1	coords= -840 -760 0	2	0	1	mandatory= 0	0	1
index

funcName= Run_FindString 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -896 -790 0	0	0	0	mandatory= 0	0	1


funcName= Run_FindString 1	0	EndOfFunc
BEGIN_LINE
1	116436	1	2	-1	0
-750	-865
-750	-815
-930	-815
-930	-790
END_LINE
finishport

3	0	coords= -840 -790 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	6	EndOfFunc
BEGIN_LINE
0	116396	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
116396	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-795	-800	-772	-770
1	0
blockattr
BEGIN_BA
type	Event_Processor
trans	-1	0	0	0
offset	0	0
hash	0xA177
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -796 -780 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	116395	1	0	-1	0
-818	-770
-808	-780
END_LINE
startport

2	0	coords= -796 -790 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	116395	0	2	-1	0
-823	-790
-813	-790
END_LINE
finishport

3	0	coords= -772 -790 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	117321	0	3	-1	0
END_LINE
finishport

3	0	coords= -772 -780 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	117403	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
116436	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-795	-885	-772	-855
1	0
blockattr
BEGIN_BA
type	Event_Processor
trans	-1	0	0	0
offset	0	0
hash	0xA177
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -796 -865 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	116356	1	0	-1	0
-813	-855
-803	-865
END_LINE
startport

2	0	coords= -796 -875 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	116356	0	2	-1	0
-813	-875
-803	-875
END_LINE
finishport

3	0	coords= -772 -875 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	116526	1	3	-1	0
END_LINE
finishport

3	0	coords= -772 -865 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	116395	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
116478	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-795	-970	-772	-940
1	0
blockattr
BEGIN_BA
type	Event_Processor
trans	-1	0	0	0
offset	0	0
hash	0xA177
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -796 -950 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	116319	1	0	-1	0
-813	-940
-803	-950
END_LINE
startport

2	0	coords= -796 -960 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	116319	0	2	-1	0
-813	-960
-803	-960
END_LINE
finishport

3	0	coords= -772 -960 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	116526	0	3	-1	0
END_LINE
finishport

3	0	coords= -772 -950 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	116356	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
116479	0	0	0
_
_
<nl?>        
xcomment
0
_
-905	-1015	-595	-980
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	TODO - extend txSw to do find as well as cmp 
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

mux_4s
_
116481	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-1025	-875	-995	-755
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0xB90E
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -1026 -840 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116315	0	0	-1	0
-1106	-840
-1096	-840
END_LINE
inputport

0	3	coords= -1026 -815 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	116524	0	0	-1	0
-1080	-620
-1080	-815
END_LINE
inputport

0	3	coords= -1026 -790 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -1026 -765 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	117164	0	0	-1	0
-970	-590
-970	-665
-1050	-665
-1050	-765
END_LINE
outputport

1	3	coords= -995 -840 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	117404	0	1	-1	0
END_LINE
startport

2	3	coords= -1026 -850 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116315	2	2	-1	0
-1116	-865
-1106	-850
END_LINE
startport

2	3	coords= -1026 -825 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	116524	2	2	-1	0
-1095	-645
-1095	-825
END_LINE
startport

2	3	coords= -1026 -800 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -1026 -775 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	117164	0	2	-1	0
-985	-600
-985	-650
-1065	-650
-1065	-775
END_LINE
finishport

3	3	coords= -995 -850 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	116319	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
116524	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-1265	-720	-1200	-600
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_RO
trans	-1	0	0	0
offset	0	-15
hash	0x560C
Instance_Info	controller1.dat
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	controller1.dat
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Use the form (x) where x is the delimiter

parameter
Use Application Dir	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	coords= -1266 -685 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -1200 -620 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	116481	1	1	-1	0
END_LINE
outputport

1	1	coords= -1200 -610 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1266 -695 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116629	0	2	-1	0
-1295	-695
-1285	-695
END_LINE
startport

2	3	coords= -1266 -670 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	116525	0	2	-1	0
-1105	-736
-1105	-785
-1290	-785
-1290	-670
END_LINE
startport

2	3	coords= -1266 -645 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	116524	0	2	-1	1
-1175	-695
-1175	-750
-1290	-750
-1290	-645
END_LINE
finishport

3	3	coords= -1200 -695 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	2	3	-1	0
END_LINE
finishport

3	3	coords= -1200 -670 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	117240	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -645 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	116525	2	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -685 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	115336	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -660 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -635 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	115336	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
116525	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-1140	-770	-1120	-705
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1141 -760 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116524	3	2	-1	0
-1175	-685
-1175	-760
END_LINE
startport

2	-1	coords= -1141 -745 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116524	5	2	-1	0
-1165	-635
-1165	-745
END_LINE
startport

2	-1	coords= -1141 -730 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116524	2	2	-1	1
-1166	-645
-1166	-730
END_LINE
startport

2	-1	coords= -1141 -715 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -1128 -736 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	115335	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3s
_
116526	0	0	0
_
_
This is a 3-input string multiplexer.
MultiplexThreeInputString
0
_
-680	-635	-650	-540
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0xD657
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -681 -600 0	0	0	1	mandatory= 0	0	1
i1
calib_json
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116528	0	0	-1	0
-450	-1384
-450	-660
-736	-660
-736	-600
END_LINE
inputport

0	3	coords= -681 -575 0	1	0	1	mandatory= 0	0	1
i2
hrdw_json
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	116576	0	0	-1	0
-450	-1349
-450	-660
-736	-660
-736	-575
END_LINE
inputport

0	3	coords= -681 -550 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	117321	0	0	-1	0
-640	-440
-640	-370
-745	-370
-745	-550
END_LINE
outputport

1	3	coords= -650 -600 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	EndOfFunc
BEGIN_LINE
0	116630	0	1	-1	0
END_LINE
startport

2	3	coords= -681 -610 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116478	0	2	-1	0
-710	-960
-710	-610
END_LINE
startport

2	3	coords= -681 -585 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	116436	0	2	-1	0
-725	-875
-725	-585
END_LINE
startport

2	3	coords= -681 -560 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	117321	0	2	-1	0
-630	-450
-630	-360
-755	-360
-755	-560
END_LINE
finishport

3	3	coords= -650 -610 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	EndOfFunc
BEGIN_LINE
0	116318	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
116528	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1415	-1395	-1275	-1374
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	calib.json
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1275 -1384 0	0	0	1	mandatory= 0	0	1	memsize= 16

calib_json
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116526	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
116576	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1415	-1360	-1275	-1339
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	hrdw.json
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1275 -1349 0	0	0	1	mandatory= 0	0	1	memsize= 16

hrdw_json
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116526	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
116626	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1415	-1325	-1275	-1304
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ctrl1.json
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1275 -1314 0	0	0	1	mandatory= 0	0	1	memsize= 16

ctrl1_json
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117321	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor3
_
116627	0	0	0
_
_
This is a 3-input event OR.
EventThreeInputOr
0
_
-400	-575	-380	-525
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_3
trans	-1	0	0	0
offset	0	0
hash	0xBA97
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -401 -565 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116318	3	2	-1	0
-430	-600
-430	-565
END_LINE
startport

2	-1	coords= -401 -550 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116318	4	2	-1	0
-443	-575
-443	-550
END_LINE
startport

2	-1	coords= -401 -535 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116318	5	2	-1	0
-460	-550
-460	-535
END_LINE
finishport

3	-1	coords= -388 -550 0	0	0	1	mandatory= 0	0	1

json_save_error
funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	EndOfFunc
BEGIN_LINE
0	118959	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
116628	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-1430	-705	-1410	-665
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_2
trans	-1	0	0	0
offset	0	0
hash	0x0A91
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1431 -695 0	0	0	0	mandatory= 0	0	1

write_done
funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116268	2	2	-1	0
-260	-1345
-260	-10
-1481	-10
-1481	-695
END_LINE
startport

2	-1	coords= -1431 -675 0	1	0	0	mandatory= 0	0	1

json_save_error
funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116627	0	2	-1	0
-333	-550
-333	-590
-1476	-590
-1476	-675
END_LINE
finishport

3	-1	coords= -1418 -684 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	116629	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
116629	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1340	-715	-1320	-665
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	manu_2
trans	-1	0	0	0
offset	0	0
hash	0xCA39
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1341 -705 0	2	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116629	1	2	-1	0
-1310	-675
-1310	-750
-1360	-750
-1360	-705
END_LINE
startport

2	-1	coords= -1341 -690 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116628	0	2	-1	0
-1375	-684
-1365	-690
END_LINE
startport

2	-1	coords= -1341 -675 0	0	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	116315	1	2	-1	0
-1140	-890
-1140	-790
-1365	-790
-1365	-675
END_LINE
finishport

3	-1	coords= -1334 -695 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	116524	0	3	-1	0
END_LINE
finishport

3	-1	coords= -1334 -675 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	116629	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
116630	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-540	-450	-510	-330
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0xB90E
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -541 -415 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116526	0	0	-1	0
-625	-600
-625	-415
END_LINE
inputport

0	3	coords= -541 -390 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -541 -365 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -541 -340 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -510 -415 0	0	0	1	mandatory= 0	0	1

load_json_settings_file
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	115335	0	1	-1	0
END_LINE
startport

2	3	coords= -541 -425 0	0	0	0	mandatory= 0	0	1

json_save_done
funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116318	1	2	-1	0
-470	-585
-470	-495
-616	-495
-616	-425
END_LINE
startport

2	3	coords= -541 -400 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -541 -375 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -541 -350 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -510 -425 0	0	0	1	mandatory= 0	0	1

load_json_settings
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	115335	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
116631	0	0	0
_
_
<nl?>        
xcomment
0
_
-605	-500	-388	-463
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Save json settings to file system
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
116642	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
480	-1135	530	-990
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x1627
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	{"id":3,"p1s":%.2f,"p1o":%.2f,"p2s":%.2f,"p2o":%.2f,"t1s":%.2f,"t1o":%.2f,"z1s":%.2f,"z1o":%.2f,
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 479 -1113 0	0	0	1	mandatory= 0	0	1
r1
p1s
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	119817	0	0	-1	0
434	-1070
434	-1113
END_LINE
inputport

0	2	coords= 479 -1103 0	1	0	1	mandatory= 0	0	1
r2
p1o
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	119817	1	0	-1	0
434	-1045
434	-1103
END_LINE
inputport

0	2	coords= 479 -1093 0	2	0	1	mandatory= 0	0	1
r3
p2s
funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	119817	2	0	-1	0
434	-1020
434	-1093
END_LINE
inputport

0	2	coords= 479 -1083 0	3	0	1	mandatory= 0	0	1
r4
p2o
funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	119817	3	0	-1	0
434	-995
434	-1083
END_LINE
inputport

0	2	coords= 479 -1073 0	4	0	1	mandatory= 0	0	1
r5
t1s
funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	119818	0	0	-1	0
432	-940
432	-1073
END_LINE
inputport

0	2	coords= 479 -1063 0	5	0	1	mandatory= 0	0	1
r6
t1o
funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	119818	1	0	-1	0
432	-915
432	-1063
END_LINE
inputport

0	2	coords= 479 -1053 0	6	0	1	mandatory= 0	0	1
r7
z1s
funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	119818	2	0	-1	0
432	-890
432	-1053
END_LINE
inputport

0	2	coords= 479 -1043 0	7	0	1	mandatory= 0	0	1
r8
z1o
funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	119818	3	0	-1	0
432	-865
432	-1043
END_LINE
inputport

0	3	coords= 479 -1023 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 530 -1113 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	116644	0	1	-1	0
END_LINE
startport

2	3	coords= 479 -1123 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	116764	0	2	-1	0
457	-1210
457	-1123
END_LINE
finishport

3	3	coords= 530 -1123 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	116643	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
116643	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
480	-975	530	-830
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x1627
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	"f1s":%.2f,"f1o":%.2f}
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 479 -953 0	0	0	1	mandatory= 0	0	1
r1
f1s
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	119825	0	0	-1	0
422	-800
422	-953
END_LINE
inputport

0	2	coords= 479 -943 0	1	0	1	mandatory= 0	0	1
r2
f1o
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	119825	1	0	-1	0
422	-775
422	-943
END_LINE
inputport

0	2	coords= 479 -933 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 479 -923 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 479 -913 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 479 -903 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 479 -893 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 479 -883 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 479 -863 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 530 -953 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	116644	1	1	-1	0
END_LINE
startport

2	3	coords= 479 -963 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	116642	0	2	-1	0
575	-1123
575	-1170
425	-1170
425	-963
END_LINE
finishport

3	3	coords= 530 -963 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	116644	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
116644	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
735	-895	765	-850
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 734 -870 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	116642	0	0	-1	0
632	-1113
632	-870
END_LINE
inputport

0	3	coords= 734 -860 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	116643	0	0	-1	0
605	-953
605	-860
END_LINE
outputport

1	3	coords= 765 -870 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	118815	0	1	-1	0
END_LINE
startport

2	3	coords= 734 -880 0	0	0	0	mandatory= 0	0	1


funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	116643	0	2	-1	0
617	-963
617	-880
END_LINE
finishport

3	3	coords= 765 -880 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	118815	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
116645	0	0	0
_
_
<nl?>        
xcomment
0
_
80	-1225	208	-1188
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Demux all settings
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
116764	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
420	-1230	440	-1180
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	manu_2
trans	-1	0	0	0
offset	0	0
hash	0xCA39
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 419 -1220 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116764	1	2	-1	0
471	-1190
471	-1265
374	-1265
374	-1220
END_LINE
startport

2	-1	coords= 419 -1205 0	2	0	0	mandatory= 1	0	1

loading_json_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116041	2	2	-1	0
187	-535
187	-1205
END_LINE
startport

2	-1	coords= 419 -1190 0	1	0	0	mandatory= 0	0	1

demux_calib
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	119817	0	2	-1	0
404	-1080
404	-1190
END_LINE
finishport

3	-1	coords= 426 -1210 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	116642	0	3	-1	0
END_LINE
finishport

3	-1	coords= 426 -1190 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	116764	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_transrx
_
116894	0	0	0
_
_
This is a string to real converter.
ConvertorStringToReal
0
_
5	-600	35	-570
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xC1E9
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 4 -580 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorStringToReal 1	1	EndOfFunc
BEGIN_LINE
1	116041	2	0	-1	0
-26	-585
-16	-580
END_LINE
outputport

1	2	coords= 35 -580 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_ConvertorStringToReal 1	2	EndOfFunc
BEGIN_LINE
0	119819	0	1	-1	0
END_LINE
startport

2	2	coords= 4 -590 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorStringToReal 1	0	EndOfFunc
BEGIN_LINE
1	116041	8	2	-1	0
-16	-605
-6	-590
END_LINE
finishport

3	1	coords= 35 -590 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorStringToReal 1	3	EndOfFunc
BEGIN_LINE
0	119819	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117092	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1415	-1290	-1275	-1269
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ctrl2.json
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1275 -1279 0	0	0	1	mandatory= 0	0	1	memsize= 16

ctrl2_json
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117321	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117163	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1415	-1255	-1275	-1234
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ctrl3.json
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1275 -1244 0	0	0	1	mandatory= 0	0	1	memsize= 16

ctrl3_json
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117321	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
117164	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-1030	-625	-1000	-505
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0xB90E
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -1031 -590 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	117237	0	0	-1	0
-1075	-400
-1075	-590
END_LINE
inputport

0	3	coords= -1031 -565 0	2	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	117317	0	0	-1	0
-1050	-180
-1050	-565
END_LINE
inputport

0	3	coords= -1031 -540 0	1	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -1031 -515 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -1000 -590 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	116481	3	1	-1	0
END_LINE
startport

2	3	coords= -1031 -600 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	117237	2	2	-1	0
-1090	-425
-1090	-600
END_LINE
startport

2	3	coords= -1031 -575 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	117317	2	2	-1	0
-1065	-205
-1065	-575
END_LINE
startport

2	3	coords= -1031 -550 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -1031 -525 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -1000 -600 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	116481	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
117237	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-1265	-500	-1200	-380
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_RO
trans	-1	0	0	0
offset	0	-15
hash	0x560C
Instance_Info	controller2.dat
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	controller2.dat
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Use the form (x) where x is the delimiter

parameter
Use Application Dir	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	coords= -1266 -465 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -1200 -400 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	117164	0	1	-1	0
END_LINE
outputport

1	1	coords= -1200 -390 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1266 -475 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	117240	0	2	-1	1
-1295	-475
-1285	-475
END_LINE
startport

2	3	coords= -1266 -450 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	117238	0	2	-1	0
-1105	-516
-1105	-565
-1290	-565
-1290	-450
END_LINE
startport

2	3	coords= -1266 -425 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	117237	0	2	-1	0
-1175	-475
-1175	-530
-1290	-530
-1290	-425
END_LINE
finishport

3	3	coords= -1200 -475 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	2	3	-1	0
END_LINE
finishport

3	3	coords= -1200 -450 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	117320	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -425 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	117238	2	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -465 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	115336	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -440 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -415 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	115336	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
117238	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-1140	-550	-1120	-485
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1141 -540 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	117237	3	2	-1	0
-1175	-465
-1175	-540
END_LINE
startport

2	-1	coords= -1141 -525 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	117237	5	2	-1	0
-1165	-415
-1165	-525
END_LINE
startport

2	-1	coords= -1141 -510 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	117237	2	2	-1	1
-1160	-425
-1160	-510
END_LINE
startport

2	-1	coords= -1141 -495 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -1128 -516 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	115335	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
117239	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-1430	-485	-1410	-445
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_2
trans	-1	0	0	0
offset	0	0
hash	0x0A91
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1431 -475 0	0	0	0	mandatory= 0	0	1

write_done
funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116268	2	2	-1	0
-255	-1345
-255	-15
-1476	-15
-1476	-475
END_LINE
startport

2	-1	coords= -1431 -455 0	1	0	0	mandatory= 0	0	1

json_save_error
funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116627	0	2	-1	0
-343	-550
-343	-600
-1476	-600
-1476	-455
END_LINE
finishport

3	-1	coords= -1418 -464 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	116629	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
117240	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1340	-495	-1320	-445
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	manu_2
trans	-1	0	0	0
offset	0	0
hash	0xCA39
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1341 -485 0	2	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	117240	1	2	-1	1
-1315	-455
-1315	-530
-1365	-530
-1365	-485
END_LINE
startport

2	-1	coords= -1341 -470 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	117239	0	2	-1	1
-1375	-464
-1365	-470
END_LINE
startport

2	-1	coords= -1341 -455 0	0	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	116524	1	2	-1	1
-1145	-670
-1145	-575
-1370	-575
-1370	-455
END_LINE
finishport

3	-1	coords= -1334 -475 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	116524	0	3	-1	0
END_LINE
finishport

3	-1	coords= -1334 -455 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	117240	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
117317	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-1265	-280	-1200	-160
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_RO
trans	-1	0	0	0
offset	0	-15
hash	0x560C
Instance_Info	controller3.dat
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	controller3.dat
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
String Field Width

parameter
Delimiter	3	0	0	0	0	0	0	1.2	( )
EndOfValues
EndOfLabels
Use the form (x) where x is the delimiter

parameter
Use Application Dir	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

inputport

0	3	coords= -1266 -245 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -1200 -180 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	117164	2	1	-1	0
END_LINE
outputport

1	1	coords= -1200 -170 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1266 -255 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	117320	0	2	-1	0
-1295	-255
-1285	-255
END_LINE
startport

2	3	coords= -1266 -230 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	117318	0	2	-1	0
-1105	-296
-1105	-345
-1290	-345
-1290	-230
END_LINE
startport

2	3	coords= -1266 -205 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	117317	0	2	-1	0
-1175	-255
-1175	-310
-1290	-310
-1290	-205
END_LINE
finishport

3	3	coords= -1200 -255 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	2	3	-1	0
END_LINE
finishport

3	3	coords= -1200 -230 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	118960	2	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -205 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	117318	2	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -245 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	115336	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -220 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -1200 -195 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	115336	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
117318	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-1140	-330	-1120	-265
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_4
trans	-1	0	0	0
offset	0	0
hash	0x549D
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1141 -320 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	117317	3	2	-1	0
-1175	-245
-1175	-320
END_LINE
startport

2	-1	coords= -1141 -305 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	117317	5	2	-1	0
-1165	-195
-1165	-305
END_LINE
startport

2	-1	coords= -1141 -290 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	117317	2	2	-1	1
-1160	-205
-1160	-290
END_LINE
startport

2	-1	coords= -1141 -275 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -1128 -296 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	115335	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
117319	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-1430	-265	-1410	-225
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_2
trans	-1	0	0	0
offset	0	0
hash	0x0A91
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1431 -255 0	0	0	0	mandatory= 0	0	1

write_done
funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116268	2	2	-1	0
-250	-1345
-250	-20
-1471	-20
-1471	-255
END_LINE
startport

2	-1	coords= -1431 -235 0	1	0	0	mandatory= 0	0	1

json_save_error
funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116627	0	2	-1	0
-343	-550
-343	-290
-1486	-290
-1486	-235
END_LINE
finishport

3	-1	coords= -1418 -244 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	116629	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
117320	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1340	-275	-1320	-225
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	manu_2
trans	-1	0	0	0
offset	0	0
hash	0xCA39
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1341 -265 0	2	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	117320	1	2	-1	0
-1305	-235
-1305	-310
-1370	-310
-1370	-265
END_LINE
startport

2	-1	coords= -1341 -250 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	117319	0	2	-1	0
-1375	-244
-1365	-250
END_LINE
startport

2	-1	coords= -1341 -235 0	0	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	117237	1	2	-1	0
-1145	-450
-1145	-350
-1386	-350
-1386	-235
END_LINE
finishport

3	-1	coords= -1334 -255 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	116524	0	3	-1	0
END_LINE
finishport

3	-1	coords= -1334 -235 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	117320	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3s
_
117321	0	0	0
_
_
This is a 3-input string multiplexer.
MultiplexThreeInputString
0
_
-690	-475	-660	-380
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0xD657
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -691 -440 0	0	0	1	mandatory= 0	0	1
i1
ctrl1_json
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116626	0	0	-1	0
-873	-1314
-873	-440
END_LINE
inputport

0	3	coords= -691 -415 0	1	0	1	mandatory= 0	0	1
i2
ctrl2_json
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	117092	0	0	-1	0
-873	-1279
-873	-415
END_LINE
inputport

0	3	coords= -691 -390 0	2	0	1	mandatory= 0	0	1
i3
ctrl3_json
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	117163	0	0	-1	0
-873	-1244
-873	-390
END_LINE
outputport

1	3	coords= -660 -440 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	EndOfFunc
BEGIN_LINE
0	116526	2	1	-1	0
END_LINE
startport

2	3	coords= -691 -450 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116396	0	2	-1	0
-725	-790
-725	-450
END_LINE
startport

2	3	coords= -691 -425 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	117408	0	2	-1	0
-725	-700
-725	-425
END_LINE
startport

2	3	coords= -691 -400 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	117407	0	2	-1	0
-725	-615
-725	-400
END_LINE
finishport

3	3	coords= -660 -450 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	EndOfFunc
BEGIN_LINE
0	116526	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_finds
_
117403	0	0	0
_
_
This function determines if a string s2 exists in string s1.
FindString
0
_
-920	-710	-865	-660
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	5	1.25	0	find_str
trans	-1	0	0	0
offset	0	0
hash	0x7D98
Instance_Info	is ctrl2?
End_Instance
END_BA
parameter
Reverse Search	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Biases search for backward searching

parameter
Find String (-> s2)	3	0	0	0	0	0	0	1.2	"id":3
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= -921 -690 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_FindString 1	1	EndOfFunc
BEGIN_LINE
1	116481	0	0	-1	0
-963	-840
-963	-690
END_LINE
inputport

0	3	coords= -921 -680 0	1	0	1	mandatory= 0	0	1
s2
is_ctrl2
funcName= Run_FindString 1	2	EndOfFunc
BEGIN_LINE
1	117498	0	0	-1	0
-983	-1279
-983	-680
END_LINE
outputport

1	3	coords= -865 -690 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -865 -680 0	1	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	4	EndOfFunc
BEGIN_LINE
0	116478	0	1	-1	0
END_LINE
outputport

1	1	coords= -865 -670 0	2	0	1	mandatory= 0	0	1
index

funcName= Run_FindString 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -921 -700 0	0	0	0	mandatory= 0	0	1


funcName= Run_FindString 1	0	EndOfFunc
BEGIN_LINE
1	116396	1	2	-1	0
-750	-780
-750	-725
-950	-725
-950	-700
END_LINE
finishport

3	0	coords= -865 -700 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	6	EndOfFunc
BEGIN_LINE
0	116478	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_finds
_
117404	0	0	0
_
_
This function determines if a string s2 exists in string s1.
FindString
0
_
-920	-625	-865	-575
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	5	1.25	0	find_str
trans	-1	0	0	0
offset	0	0
hash	0x7D98
Instance_Info	is ctrl3?
End_Instance
END_BA
parameter
Reverse Search	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Biases search for backward searching

parameter
Find String (-> s2)	3	0	0	0	0	0	0	1.2	"id":2
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= -921 -605 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_FindString 1	1	EndOfFunc
BEGIN_LINE
1	116481	0	0	-1	0
-963	-840
-963	-605
END_LINE
inputport

0	3	coords= -921 -595 0	1	0	1	mandatory= 0	0	1
s2
is_ctrl3
funcName= Run_FindString 1	2	EndOfFunc
BEGIN_LINE
1	117499	0	0	-1	0
-983	-1244
-983	-595
END_LINE
outputport

1	3	coords= -865 -605 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -865 -595 0	1	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	4	EndOfFunc
BEGIN_LINE
0	116436	0	1	-1	0
END_LINE
outputport

1	1	coords= -865 -585 0	2	0	1	mandatory= 0	0	1
index

funcName= Run_FindString 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -921 -615 0	0	0	0	mandatory= 0	0	1


funcName= Run_FindString 1	0	EndOfFunc
BEGIN_LINE
1	117408	1	2	-1	0
-775	-690
-775	-640
-955	-640
-955	-615
END_LINE
finishport

3	0	coords= -865 -615 0	0	0	1	mandatory= 0	0	1


funcName= Run_FindString 1	6	EndOfFunc
BEGIN_LINE
0	116436	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
117407	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-820	-625	-797	-595
1	0
blockattr
BEGIN_BA
type	Event_Processor
trans	-1	0	0	0
offset	0	0
hash	0xA177
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -821 -605 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	117404	1	0	-1	0
-838	-595
-828	-605
END_LINE
startport

2	0	coords= -821 -615 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	117404	0	2	-1	0
-838	-615
-828	-615
END_LINE
finishport

3	0	coords= -797 -615 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	117321	2	3	-1	0
END_LINE
finishport

3	0	coords= -797 -605 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	116395	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
117408	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-820	-710	-797	-680
1	0
blockattr
BEGIN_BA
type	Event_Processor
trans	-1	0	0	0
offset	0	0
hash	0xA177
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -821 -690 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	117403	1	0	-1	0
-838	-680
-828	-690
END_LINE
startport

2	0	coords= -821 -700 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	117403	0	2	-1	0
-838	-700
-828	-700
END_LINE
finishport

3	0	coords= -797 -700 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	117321	1	3	-1	0
END_LINE
finishport

3	0	coords= -797 -690 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	116356	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
117409	0	0	0
_
_
<nl?>        
xcomment
0
_
-1415	-1445	-1323	-1408
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Settings files
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

const_s1
_
117495	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1175	-1395	-1035	-1374
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	"id":3
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1035 -1384 0	0	0	1	mandatory= 0	0	1	memsize= 16

is_calib
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116319	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117496	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1175	-1360	-1035	-1339
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	"id":2
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1035 -1349 0	0	0	1	mandatory= 0	0	1	memsize= 16

is_hrdw
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116356	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117497	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1175	-1325	-1035	-1304
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	"pid":0
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1035 -1314 0	0	0	1	mandatory= 0	0	1	memsize= 16

is_ctrl1
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116395	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117498	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1175	-1290	-1035	-1269
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	"pid":1
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1035 -1279 0	0	0	1	mandatory= 0	0	1	memsize= 16

is_ctrl2
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117403	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117499	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1175	-1255	-1035	-1234
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	"pid":2
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -1035 -1244 0	0	0	1	mandatory= 0	0	1	memsize= 16

is_ctrl3
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117404	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
117500	0	0	0
_
_
<nl?>        
xcomment
0
_
-1180	-1450	-1051	-1413
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Settings identifiers
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

xcomment
_
117501	0	0	0
_
_
<nl?>        
xcomment
0
_
-1285	-60	-1077	-23
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	TODO - add hardware settings
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

const_s1
_
117595	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-460	20	-439
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	pid
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -449 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117596	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-430	20	-409
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	cs
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -419 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117597	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-400	20	-379
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ct
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -389 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	5	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117598	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-270	20	-249
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	cm3
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -259 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117822	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117599	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-240	20	-219
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	sm
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -229 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117822	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117600	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-210	20	-189
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	td
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -199 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117822	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117601	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-180	20	-159
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ram
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -169 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117822	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117602	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-370	20	-349
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	em
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -359 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	6	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117603	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-340	20	-319
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	cm1
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -329 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	7	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117604	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-310	20	-289
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	cm2
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -299 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116046	8	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117708	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-145	20	-124
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	msp
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -134 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117709	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-115	20	-94
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	tsp
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -104 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117710	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-85	20	-64
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	la
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -74 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117711	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-55	20	-34
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	ua
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -44 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117819	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	-20	20	1
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	pp
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 -9 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	5	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117820	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	10	20	31
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	pi
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 21 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	6	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
117821	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-120	40	20	61
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x1777
Instance_Info	_
End_Instance
END_BA
parameter
String Constant	3	0	100	1	5	4	0	1.2	pd
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 20 51 0	0	0	1	mandatory= 0	0	1	memsize= 8


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	117823	7	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
117822	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
130	-475	160	-290
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	0	5	1.25	0	TxSw
trans	-1	0	0	0
offset	0	-15
hash	0xCF87
Instance_Info	_
End_Instance
END_BA
parameter
case 1	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	coords= 129 -440 0	0	0	1	mandatory= 0	0	1
test
json_key_str
funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	116041	3	0	-1	0
32	-635
32	-440
END_LINE
inputport

0	3	coords= 129 -420 0	1	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	117598	0	0	-1	0
69	-259
69	-420
END_LINE
inputport

0	3	coords= 129 -405 0	2	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	117599	0	0	-1	0
69	-229
69	-405
END_LINE
inputport

0	3	coords= 129 -390 0	3	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	117600	0	0	-1	0
69	-199
69	-390
END_LINE
inputport

0	3	coords= 129 -375 0	4	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	117601	0	0	-1	0
69	-169
69	-375
END_LINE
inputport

0	3	coords= 129 -360 0	1	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
1	117708	0	0	-1	0
69	-134
69	-360
END_LINE
inputport

0	3	coords= 129 -345 0	2	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
1	117709	0	0	-1	0
69	-104
69	-345
END_LINE
inputport

0	3	coords= 129 -330 0	3	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
1	117710	0	0	-1	0
69	-74
69	-330
END_LINE
inputport

0	3	coords= 129 -315 0	4	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
1	117711	0	0	-1	0
69	-44
69	-315
END_LINE
startport

2	3	coords= 129 -450 0	0	0	0	mandatory= 0	0	1
go
json_value_ready
funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	119819	0	2	-1	0
125	-555
125	-450
END_LINE
finishport

3	3	coords= 160 -420 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	119822	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -405 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	119821	3	3	-1	0
END_LINE
finishport

3	3	coords= 160 -390 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	118415	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -375 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	118416	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -360 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	119823	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -345 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	119823	1	3	-1	0
END_LINE
finishport

3	3	coords= 160 -330 0	2	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	119823	2	3	-1	0
END_LINE
finishport

3	3	coords= 160 -315 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	119823	3	3	-1	0
END_LINE
finishport

3	3	coords= 160 -300 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
117823	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
130	-255	160	-70
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	0	5	1.25	0	TxSw
trans	-1	0	0	0
offset	0	-15
hash	0xCF87
Instance_Info	_
End_Instance
END_BA
parameter
case 1	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	coords= 129 -220 0	0	0	1	mandatory= 0	0	1
test
json_key_str
funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	116041	3	0	-1	0
32	-635
32	-220
END_LINE
inputport

0	3	coords= 129 -200 0	5	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	117819	0	0	-1	0
69	-9
69	-200
END_LINE
inputport

0	3	coords= 129 -185 0	6	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	117820	0	0	-1	0
69	21
69	-185
END_LINE
inputport

0	3	coords= 129 -170 0	7	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	117821	0	0	-1	0
69	51
69	-170
END_LINE
inputport

0	3	coords= 129 -155 0	8	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -140 0	6	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -125 0	7	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -110 0	5	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -95 0	8	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	coords= 129 -230 0	0	0	0	mandatory= 0	0	1
go
json_value_ready
funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	119819	0	2	-1	0
125	-555
125	-230
END_LINE
finishport

3	3	coords= 160 -200 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	118671	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -185 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	118672	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -170 0	2	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	118673	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -155 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -140 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -125 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -110 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -95 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -80 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
117936	0	0	0
_
_
<nl?>        
xcomment
0
_
-120	-1165	-42	-1134
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Calibration
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

xcomment
_
118050	0	0	0
_
_
<nl?>        
xcomment
0
_
-95	-500	-26	-466
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	PID 1,2,3
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

xcomment
_
118165	0	0	0
_
_
<nl?>        
xcomment
0
_
-100	130	108	167
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	TODO - add hardware settings
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
118674	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
490	-690	540	-545
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x5FEC
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	{"id":1,"pid":%d,"cs":%d,"ct":%d,"em":%d,"erk":0,"cm1":%d,"cm2":%d,"cm3":%d,"sm":%d,
EndOfValues
EndOfLabels
C-style format string - supports %d or %u only

inputport

0	1	coords= 489 -668 0	0	0	1	mandatory= 0	0	1
i1
pid_id
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	119820	0	0	-1	0
437	-660
437	-668
END_LINE
inputport

0	1	coords= 489 -658 0	1	0	1	mandatory= 0	0	1
i2
cs
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	119820	1	0	-1	0
437	-635
437	-658
END_LINE
inputport

0	1	coords= 489 -648 0	2	0	1	mandatory= 0	0	1
i3
ct
funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	119820	2	0	-1	0
437	-610
437	-648
END_LINE
inputport

0	1	coords= 489 -638 0	3	0	1	mandatory= 0	0	1
i4
em
funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	119820	3	0	-1	0
437	-585
437	-638
END_LINE
inputport

0	1	coords= 489 -628 0	4	0	1	mandatory= 0	0	1
i5
cm1
funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	119821	0	0	-1	0
439	-525
439	-628
END_LINE
inputport

0	1	coords= 489 -618 0	5	0	1	mandatory= 0	0	1
i6
cm2
funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	119821	1	0	-1	0
439	-500
439	-618
END_LINE
inputport

0	1	coords= 489 -608 0	6	0	1	mandatory= 0	0	1
i7
cm3
funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	119821	2	0	-1	0
439	-475
439	-608
END_LINE
inputport

0	1	coords= 489 -598 0	7	0	1	mandatory= 0	0	1
i8
sm
funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	119821	3	0	-1	0
439	-450
439	-598
END_LINE
inputport

0	3	coords= 489 -578 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 540 -668 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	118813	0	1	-1	0
END_LINE
startport

2	3	coords= 489 -678 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	118812	0	2	-1	0
470	-770
470	-678
END_LINE
finishport

3	3	coords= 540 -678 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	118675	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
118675	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
490	-510	540	-365
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x5FEC
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	"td":%d,"ram":%d,
EndOfValues
EndOfLabels
C-style format string - supports %d or %u only

inputport

0	1	coords= 489 -488 0	0	0	1	mandatory= 0	0	1
i1
td
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	119822	0	0	-1	0
402	-390
402	-488
END_LINE
inputport

0	1	coords= 489 -478 0	1	0	1	mandatory= 0	0	1
i2
ram
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	119822	1	0	-1	0
402	-365
402	-478
END_LINE
inputport

0	1	coords= 489 -468 0	2	0	1	mandatory= 0	0	1
i3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 489 -458 0	3	0	1	mandatory= 0	0	1
i4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 489 -448 0	4	0	1	mandatory= 0	0	1
i5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 489 -438 0	5	0	1	mandatory= 0	0	1
i6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 489 -428 0	6	0	1	mandatory= 0	0	1
i7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 489 -418 0	7	0	1	mandatory= 0	0	1
i8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 489 -398 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 540 -488 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	118813	1	1	-1	0
END_LINE
startport

2	3	coords= 489 -498 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	118674	0	2	-1	0
585	-678
585	-725
444	-725
444	-498
END_LINE
finishport

3	3	coords= 540 -498 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	118813	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
118676	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
495	-300	545	-155
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0x1627
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	"msp":%.2f,"tsp":%.2f,"la":%.2f,"ua":%.2f,"p":%.4f,"i":%.4f,"d":%.4f,"cl1":100.0001,"cl2":200.0002,"cl3":300.0003}
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 494 -278 0	0	0	1	mandatory= 0	0	1
r1
msp
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	119823	0	0	-1	0
449	-305
449	-278
END_LINE
inputport

0	2	coords= 494 -268 0	1	0	1	mandatory= 0	0	1
r2
tsp
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	119823	1	0	-1	0
444	-280
444	-268
END_LINE
inputport

0	2	coords= 494 -258 0	2	0	1	mandatory= 0	0	1
r3
la
funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	119823	2	0	-1	0
414	-255
424	-258
END_LINE
inputport

0	2	coords= 494 -248 0	3	0	1	mandatory= 0	0	1
r4
ua
funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	119823	3	0	-1	0
444	-230
444	-248
END_LINE
inputport

0	2	coords= 494 -238 0	4	0	1	mandatory= 0	0	1
r5
p
funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	119824	0	0	-1	0
404	-165
404	-238
END_LINE
inputport

0	2	coords= 494 -228 0	5	0	1	mandatory= 0	0	1
r6
i
funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	119824	1	0	-1	0
404	-140
404	-228
END_LINE
inputport

0	2	coords= 494 -218 0	6	0	1	mandatory= 0	0	1
r7
d
funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	119824	2	0	-1	0
404	-115
404	-218
END_LINE
inputport

0	2	coords= 494 -208 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 494 -188 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 545 -278 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	118814	1	1	-1	0
END_LINE
startport

2	3	coords= 494 -288 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	118813	0	2	-1	0
670	-475
670	-330
455	-330
455	-288
END_LINE
finishport

3	3	coords= 545 -288 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	118814	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
118812	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
435	-790	455	-740
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	manu_2
trans	-1	0	0	0
offset	0	0
hash	0xCA39
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 434 -780 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	118812	1	2	-1	0
486	-750
486	-825
389	-825
389	-780
END_LINE
startport

2	-1	coords= 434 -765 0	2	0	0	mandatory= 1	0	1

loading_json_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116041	2	2	-1	0
194	-535
194	-765
END_LINE
startport

2	-1	coords= 434 -750 0	1	0	0	mandatory= 0	0	1

demux_pid_ctrl
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	119820	0	2	-1	0
409	-670
409	-750
END_LINE
finishport

3	-1	coords= 441 -770 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	118674	0	3	-1	0
END_LINE
finishport

3	-1	coords= 441 -750 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	116764	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
118813	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
620	-490	650	-445
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 619 -465 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	118674	0	0	-1	0
590	-668
590	-465
END_LINE
inputport

0	3	coords= 619 -455 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	118675	0	0	-1	0
565	-488
565	-455
END_LINE
outputport

1	3	coords= 650 -465 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	118814	0	1	-1	0
END_LINE
startport

2	3	coords= 619 -475 0	0	0	0	mandatory= 0	0	1


funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	118675	0	2	-1	0
574	-498
574	-475
END_LINE
finishport

3	3	coords= 650 -475 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	118676	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
118814	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
740	-760	770	-715
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cat
trans	-1	0	0	0
offset	0	0
hash	0x56CC
Instance_Info	_
End_Instance
END_BA
parameter
Append String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	coords= 739 -735 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	118813	0	0	-1	0
694	-465
694	-735
END_LINE
inputport

0	3	coords= 739 -725 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	118676	0	0	-1	0
710	-278
710	-725
END_LINE
outputport

1	3	coords= 770 -735 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	118815	1	1	-1	0
END_LINE
startport

2	3	coords= 739 -745 0	0	0	0	mandatory= 0	0	1


funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	118676	0	2	-1	0
680	-288
680	-745
END_LINE
finishport

3	3	coords= 770 -745 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	118815	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3s
_
118815	0	0	0
_
_
This is a 3-input string multiplexer.
MultiplexThreeInputString
0
_
835	-835	865	-740
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0xD657
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 834 -800 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116644	0	0	-1	0
795	-870
795	-800
END_LINE
inputport

0	3	coords= 834 -775 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	118814	0	0	-1	0
805	-735
805	-775
END_LINE
inputport

0	3	coords= 834 -750 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 865 -800 0	0	0	1	mandatory= 0	0	1

write_results_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	EndOfFunc
BEGIN_LINE
0	116268	1	1	-1	0
END_LINE
startport

2	3	coords= 834 -810 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116644	0	2	-1	0
804	-880
804	-810
END_LINE
startport

2	3	coords= 834 -785 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	118814	0	2	-1	0
797	-745
797	-785
END_LINE
startport

2	3	coords= 834 -760 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= 865 -810 0	0	0	1	mandatory= 0	0	1

write_results
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	EndOfFunc
BEGIN_LINE
0	116268	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
118816	0	0	0
_
_
<nl?>        
xcomment
0
_
465	-130	682	-93
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	TODO - add remining cl1,cl2,cl3
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

xcomment
_
118817	0	0	0
_
_
<nl?>        
xcomment
0
_
705	-965	906	-928
1	0
blockattr
BEGIN_BA
type	Comment_Block
trans	2	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Text	3	0	0	0	0	0	0	1.2	Write settings to the result file
EndOfValues
EndOfLabels
_

parameter
Text Color	3	0	0	0	0	0	0	1.2	#000000
EndOfValues
EndOfLabels
_

parameter
Backgound Color	3	0	0	0	0	0	0	1.2	#9DFF9C
EndOfValues
EndOfLabels
_

END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
118959	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-1435	-75	-1415	-35
1	0
blockattr
BEGIN_BA
type	Or_Event
type_var	norm_2
trans	-1	0	0	0
offset	0	0
hash	0x0A91
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1436 -65 0	0	0	0	mandatory= 0	0	1

write_done
funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	116268	2	2	-1	0
-245	-1345
-245	-25
-1471	-25
-1471	-65
END_LINE
startport

2	-1	coords= -1436 -45 0	1	0	0	mandatory= 0	0	1

json_save_error
funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116627	0	2	-1	0
-343	-550
-343	-100
-1491	-100
-1491	-45
END_LINE
finishport

3	-1	coords= -1423 -54 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	116629	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
118960	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1345	-85	-1325	-35
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	manu_2
trans	-1	0	0	0
offset	0	0
hash	0xCA39
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= -1346 -75 0	2	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	118960	1	2	-1	0
-1310	-45
-1310	-115
-1370	-115
-1370	-75
END_LINE
startport

2	-1	coords= -1346 -60 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	118959	0	2	-1	0
-1380	-54
-1370	-60
END_LINE
startport

2	-1	coords= -1346 -45 0	0	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	117317	1	2	-1	0
-1150	-230
-1150	-140
-1390	-140
-1390	-45
END_LINE
finishport

3	-1	coords= -1339 -65 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	116279	0	3	-1	0
END_LINE
finishport

3	-1	coords= -1339 -45 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	117320	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_4r
_
119817	0	0	0
_
_
This is a 4-output real demultiplexer.
DemultiplexFourOutputReal
0
_
280	-1105	310	-985
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x1DDA
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 279 -1015 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	EndOfFunc
BEGIN_LINE
1	116894	0	0	-1	0
157	-580
157	-1015
END_LINE
outputport

1	2	coords= 310 -1070 0	0	0	1	mandatory= 0	0	1
o1
p1s
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	116642	0	1	-1	0
END_LINE
outputport

1	2	coords= 310 -1045 0	0	0	1	mandatory= 0	0	1
o2
p1o
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	116642	1	1	-1	0
END_LINE
outputport

1	2	coords= 310 -1020 0	0	0	1	mandatory= 0	0	1
o3
p2s
funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	116642	2	1	-1	0
END_LINE
outputport

1	2	coords= 310 -995 0	0	0	1	mandatory= 0	0	1
o4
p2o
funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	116642	3	1	-1	0
END_LINE
startport

2	2	coords= 279 -1080 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	116045	0	2	-1	0
214	-940
214	-1080
END_LINE
startport

2	2	coords= 279 -1065 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	116045	1	2	-1	0
214	-925
214	-1065
END_LINE
startport

2	2	coords= 279 -1050 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	116045	2	2	-1	0
214	-910
214	-1050
END_LINE
startport

2	2	coords= 279 -1035 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	116045	3	2	-1	0
214	-895
214	-1035
END_LINE
finishport

3	2	coords= 310 -1080 0	0	0	1	mandatory= 0	0	1

demux_calib
funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	116764	1	3	-1	0
END_LINE
finishport

3	2	coords= 310 -1055 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 310 -1030 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 310 -1005 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_4r
_
119818	0	0	0
_
_
This is a 4-output real demultiplexer.
DemultiplexFourOutputReal
0
_
280	-975	310	-855
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x1DDA
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 279 -885 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	EndOfFunc
BEGIN_LINE
1	116894	0	0	-1	0
157	-580
157	-885
END_LINE
outputport

1	2	coords= 310 -940 0	0	0	1	mandatory= 0	0	1
o1
t1s
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	116642	4	1	-1	0
END_LINE
outputport

1	2	coords= 310 -915 0	0	0	1	mandatory= 0	0	1
o2
t1o
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	116642	5	1	-1	0
END_LINE
outputport

1	2	coords= 310 -890 0	0	0	1	mandatory= 0	0	1
o3
z1s
funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	116642	6	1	-1	0
END_LINE
outputport

1	2	coords= 310 -865 0	0	0	1	mandatory= 0	0	1
o4
z1o
funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	116642	7	1	-1	0
END_LINE
startport

2	2	coords= 279 -950 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	116045	4	2	-1	0
214	-880
214	-950
END_LINE
startport

2	2	coords= 279 -935 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	116045	5	2	-1	0
214	-865
214	-935
END_LINE
startport

2	2	coords= 279 -920 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	116045	6	2	-1	0
214	-850
214	-920
END_LINE
startport

2	2	coords= 279 -905 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	116045	7	2	-1	0
214	-835
214	-905
END_LINE
finishport

3	2	coords= 310 -950 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 310 -925 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 310 -900 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 310 -875 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrix
_
119819	0	0	0
_
_
This is a real to integer converter.
ConvertorRealToInt
0
_
85	-565	115	-535
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xD329
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 84 -545 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_ConvertorRealToInt 1	1	EndOfFunc
BEGIN_LINE
1	116894	0	0	-1	0
64	-580
64	-545
END_LINE
outputport

1	1	coords= 115 -545 0	0	0	1	mandatory= 0	0	1

json_value_int
funcName= Run_ConvertorRealToInt 1	2	EndOfFunc
BEGIN_LINE
0	119822	0	1	-1	0
END_LINE
startport

2	1	coords= 84 -555 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorRealToInt 1	0	EndOfFunc
BEGIN_LINE
1	116894	0	2	-1	0
64	-590
64	-555
END_LINE
finishport

3	2	coords= 115 -555 0	0	0	1	mandatory= 0	0	1

json_value_ready
funcName= Run_ConvertorRealToInt 1	3	EndOfFunc
BEGIN_LINE
0	117823	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_4i
_
119820	0	0	0
_
_
This is a 4-output integer demultiplexer.
DemultiplexFourOutputInt
0
_
285	-695	315	-575
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0xE2F7
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 284 -605 0	0	0	1	mandatory= 0	0	1

json_value_int
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	EndOfFunc
BEGIN_LINE
1	119819	0	0	-1	0
199	-545
199	-605
END_LINE
outputport

1	1	coords= 315 -660 0	0	0	1	mandatory= 0	0	1
o1
pid_id
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	118674	0	1	-1	0
END_LINE
outputport

1	1	coords= 315 -635 0	0	0	1	mandatory= 0	0	1
o2
cs
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	118674	1	1	-1	0
END_LINE
outputport

1	1	coords= 315 -610 0	0	0	1	mandatory= 0	0	1
o3
ct
funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	118674	2	1	-1	0
END_LINE
outputport

1	1	coords= 315 -585 0	0	0	1	mandatory= 0	0	1
o4
em
funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	118674	3	1	-1	0
END_LINE
startport

2	1	coords= 284 -670 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	116046	2	2	-1	0
227	-705
227	-670
END_LINE
startport

2	1	coords= 284 -655 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	116046	3	2	-1	0
227	-690
227	-655
END_LINE
startport

2	1	coords= 284 -640 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	116046	4	2	-1	0
227	-675
227	-640
END_LINE
startport

2	1	coords= 284 -625 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	116046	5	2	-1	0
227	-660
227	-625
END_LINE
finishport

3	1	coords= 315 -670 0	0	0	1	mandatory= 0	0	1

demux_pid_ctrl
funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	118812	2	3	-1	0
END_LINE
finishport

3	1	coords= 315 -645 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 315 -620 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 315 -595 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_4i
_
119821	0	0	0
_
_
This is a 4-output integer demultiplexer.
DemultiplexFourOutputInt
0
_
285	-560	315	-440
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0xE2F7
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 284 -470 0	0	0	1	mandatory= 0	0	1

json_value_int
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	EndOfFunc
BEGIN_LINE
1	119819	0	0	-1	0
204	-545
204	-470
END_LINE
outputport

1	1	coords= 315 -525 0	0	0	1	mandatory= 0	0	1
o1
cm1
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	118674	4	1	-1	0
END_LINE
outputport

1	1	coords= 315 -500 0	0	0	1	mandatory= 0	0	1
o2
cm2
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	118674	5	1	-1	0
END_LINE
outputport

1	1	coords= 315 -475 0	0	0	1	mandatory= 0	0	1
o3
cm3
funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	118674	6	1	-1	0
END_LINE
outputport

1	1	coords= 315 -450 0	0	0	1	mandatory= 0	0	1
o4
sm
funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	118674	7	1	-1	0
END_LINE
startport

2	1	coords= 284 -535 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	116046	6	2	-1	0
227	-645
227	-535
END_LINE
startport

2	1	coords= 284 -520 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	116046	7	2	-1	0
227	-630
227	-520
END_LINE
startport

2	1	coords= 284 -505 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	117822	0	2	-1	0
217	-420
217	-505
END_LINE
startport

2	1	coords= 284 -490 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	117822	1	2	-1	0
217	-405
217	-490
END_LINE
finishport

3	1	coords= 315 -535 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 315 -510 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 315 -485 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 315 -460 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_2i
_
119822	0	0	0
_
_
This is a 2-output integer demultiplexer.
DemultiplexTwoOutputInt
0
_
285	-425	315	-355
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0xAE32
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 284 -365 0	0	0	1	mandatory= 0	0	1

json_value_int
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	EndOfFunc
BEGIN_LINE
1	119819	0	0	-1	0
204	-545
204	-365
END_LINE
outputport

1	1	coords= 315 -390 0	0	0	1	mandatory= 0	0	1
o1
td
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	118675	0	1	-1	0
END_LINE
outputport

1	1	coords= 315 -365 0	0	0	1	mandatory= 0	0	1
o2
ram
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	118675	1	1	-1	0
END_LINE
startport

2	1	coords= 284 -400 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	117822	2	2	-1	0
217	-390
227	-400
END_LINE
startport

2	1	coords= 284 -385 0	0	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	117822	3	2	-1	0
217	-375
227	-385
END_LINE
finishport

3	1	coords= 315 -400 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 315 -375 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_4r
_
119823	0	0	0
_
_
This is a 4-output real demultiplexer.
DemultiplexFourOutputReal
0
_
285	-340	315	-220
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x1DDA
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 284 -250 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	EndOfFunc
BEGIN_LINE
1	116894	0	0	-1	0
114	-580
114	-250
END_LINE
outputport

1	2	coords= 315 -305 0	0	0	1	mandatory= 0	0	1
o1
msp
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	118676	0	1	-1	0
END_LINE
outputport

1	2	coords= 315 -280 0	0	0	1	mandatory= 0	0	1
o2
tsp
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	118676	1	1	-1	0
END_LINE
outputport

1	2	coords= 315 -255 0	0	0	1	mandatory= 0	0	1
o3
la
funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	118676	2	1	-1	0
END_LINE
outputport

1	2	coords= 315 -230 0	0	0	1	mandatory= 0	0	1
o4
ua
funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	118676	3	1	-1	0
END_LINE
startport

2	2	coords= 284 -315 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	117822	4	2	-1	0
227	-360
227	-315
END_LINE
startport

2	2	coords= 284 -300 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	117822	5	2	-1	0
227	-345
227	-300
END_LINE
startport

2	2	coords= 284 -285 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	117822	6	2	-1	0
227	-330
227	-285
END_LINE
startport

2	2	coords= 284 -270 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	117822	7	2	-1	0
227	-315
227	-270
END_LINE
finishport

3	2	coords= 315 -315 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 315 -290 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 315 -265 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 315 -240 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_3r
_
119824	0	0	0
_
_
This is a 3-output real demultiplexer.
DemultiplexThreeOutputReal
0
_
285	-200	315	-105
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x9887
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 284 -125 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	EndOfFunc
BEGIN_LINE
1	116894	0	0	-1	0
164	-580
164	-125
END_LINE
outputport

1	2	coords= 315 -165 0	0	0	1	mandatory= 0	0	1
o1
p
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	118676	4	1	-1	0
END_LINE
outputport

1	2	coords= 315 -140 0	0	0	1	mandatory= 0	0	1
o2
i
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	118676	5	1	-1	0
END_LINE
outputport

1	2	coords= 315 -115 0	0	0	1	mandatory= 0	0	1
o3
d
funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	118676	6	1	-1	0
END_LINE
startport

2	2	coords= 284 -175 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	117823	0	2	-1	0
227	-200
227	-175
END_LINE
startport

2	2	coords= 284 -160 0	0	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	117823	1	2	-1	0
227	-185
227	-160
END_LINE
startport

2	2	coords= 284 -145 0	0	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	117823	2	2	-1	0
227	-170
227	-145
END_LINE
finishport

3	2	coords= 315 -175 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 315 -150 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 315 -125 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_2r
_
119825	0	0	0
_
_
This is a 2-output real demultiplexer.
DemultiplexTwoOutputReal
0
_
280	-835	310	-765
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x4E56
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 279 -775 0	0	0	1	mandatory= 0	0	1

json_value_float
funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	EndOfFunc
BEGIN_LINE
1	116894	0	0	-1	0
157	-580
157	-775
END_LINE
outputport

1	2	coords= 310 -800 0	0	0	1	mandatory= 0	0	1
o1
f1s
funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	116643	0	1	-1	0
END_LINE
outputport

1	2	coords= 310 -775 0	0	0	1	mandatory= 0	0	1
o2
f1o
funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	116643	1	1	-1	0
END_LINE
startport

2	2	coords= 279 -810 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	116046	0	2	-1	0
214	-735
214	-810
END_LINE
startport

2	2	coords= 279 -795 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	116046	1	2	-1	0
214	-720
214	-795
END_LINE
finishport

3	2	coords= 310 -810 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	2	coords= 310 -785 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
