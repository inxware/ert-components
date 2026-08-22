#V:2.1.2

CanvasSizeXY
1805	1440

IconData
BEGIN_BLOCK

cgi2json
_
115334	0	0	0
_
_
Converts CGI (Common Gateway Interface) string to JSON string
cgi2json
0
_
-505	-970	-420	-923
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0x1920
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -506 -945 0	0	0	1	mandatory= 0	0	1
cgi

funcName= convert 1	1	EndOfFunc
BEGIN_LINE
1	115335	0	0	-1	1
-595	-890
-595	-945
END_LINE
inputport

0	1	coords= -506 -933 0	1	0	1	mandatory= 0	0	1
len

funcName= convert 1	2	EndOfFunc
BEGIN_LINE
1	115337	0	0	-1	0
-530	-875
-530	-933
END_LINE
outputport

1	3	coords= -420 -945 0	0	0	1	mandatory= 0	0	1
json

funcName= convert 1	3	EndOfFunc
BEGIN_LINE
0	116041	0	1	-1	0
END_LINE
outputport

1	1	coords= -420 -933 0	1	0	1	mandatory= 0	0	1
len

funcName= convert 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= -506 -958 0	0	0	0	mandatory= 0	0	1
run

funcName= convert 1	0	EndOfFunc
BEGIN_LINE
1	115337	0	2	-1	0
-536	-885
-536	-958
END_LINE
finishport

3	-1	coords= -420 -958 0	0	0	1	mandatory= 0	0	1
done

funcName= convert 1	5	EndOfFunc
BEGIN_LINE
0	116041	0	3	-1	0
END_LINE
END_BLOCK
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
-700	-990	-635	-870
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
File name	3	0	0	0	0	0	0	1.2	calibcgi.txt
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

0	3	coords= -701 -955 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -635 -890 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	115334	0	1	-1	0
END_LINE
outputport

1	1	coords= -635 -880 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -701 -965 0	0	0	0	mandatory= 0	0	1
open
test_start
funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	116268	0	2	-1	1
-265	-1395
-265	-1015
-756	-1015
-756	-965
END_LINE
startport

2	3	coords= -701 -940 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	115336	0	2	-1	0
-535	-1026
-535	-1095
-725	-1095
-725	-940
END_LINE
startport

2	3	coords= -701 -915 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	115335	0	2	-1	1
-615	-965
-615	-1025
-725	-1025
-725	-915
END_LINE
finishport

3	3	coords= -635 -965 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	2	3	-1	0
END_LINE
finishport

3	3	coords= -635 -940 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -635 -915 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	115337	0	3	-1	0
END_LINE
finishport

3	1	coords= -635 -955 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	115336	0	3	-1	0
END_LINE
finishport

3	1	coords= -635 -930 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -635 -905 0	5	0	1	mandatory= 0	0	1
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
-570	-1060	-550	-995
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

2	-1	coords= -571 -1050 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	115335	3	2	-1	1
-610	-955
-610	-1050
END_LINE
startport

2	-1	coords= -571 -1035 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	115335	5	2	-1	0
-600	-905
-600	-1035
END_LINE
startport

2	-1	coords= -571 -1020 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	115334	0	2	-1	1
-395	-958
-395	-985
-590	-985
-590	-1020
END_LINE
startport

2	-1	coords= -571 -1005 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -558 -1026 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	115335	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_lens
_
115337	0	0	0
_
_
This function returns the length of the string.
LenString
0
_
-590	-895	-560	-855
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	22	1.25	1	Len
trans	-1	0	0	0
offset	0	0
hash	0x9437
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -591 -875 0	0	0	1	mandatory= 0	0	1


funcName= Run_LenString 1	1	EndOfFunc
BEGIN_LINE
1	115335	0	0	-1	1
-616	-890
-606	-875
END_LINE
outputport

1	1	coords= -560 -875 0	0	0	1	mandatory= 0	0	1


funcName= Run_LenString 1	2	EndOfFunc
BEGIN_LINE
0	115334	1	1	-1	0
END_LINE
startport

2	1	coords= -591 -885 0	0	0	0	mandatory= 0	0	1


funcName= Run_LenString 1	0	EndOfFunc
BEGIN_LINE
1	115335	2	2	-1	0
-611	-915
-611	-885
END_LINE
finishport

3	1	coords= -560 -885 0	0	0	1	mandatory= 0	0	1


funcName= Run_LenString 1	3	EndOfFunc
BEGIN_LINE
0	115334	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
115567	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-130	-1150	10	-1129
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
String Constant	3	0	100	1	5	4	0	1.2	noResetSettings
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 10 -1139 0	0	0	1	mandatory= 0	0	1	memsize= 32


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	116045	1	1	-1	0
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

calibrate
_
115807	0	0	0
_
_
Calibrate
Calibrate
0
_
425	-920	510	-873
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0xAD13
Instance_Info	PT100 Cali
End_Instance
END_BA
parameter
id	1	-1	10000	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
calibration id

inputport

0	2	coords= 424 -895 0	0	0	1	mandatory= 0	0	1
scale
p1s
funcName= calibrate 1	1	EndOfFunc
BEGIN_LINE
1	116047	0	0	-1	1
359	-930
359	-895
END_LINE
inputport

0	2	coords= 424 -883 0	1	0	1	mandatory= 0	0	1
offset
p1o
funcName= calibrate 1	2	EndOfFunc
BEGIN_LINE
1	116049	0	0	-1	1
359	-890
369	-883
END_LINE
outputport

1	1	coords= 510 -883 0	0	0	1	mandatory= 0	0	1
errno

funcName= calibrate 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 424 -908 0	0	0	0	mandatory= 0	0	1
run

funcName= calibrate 1	0	EndOfFunc
BEGIN_LINE
1	116048	0	2	-1	1
380	-957
380	-908
END_LINE
finishport

3	2	coords= 510 -908 0	0	0	1	mandatory= 0	0	1
--

funcName= calibrate 1	4	EndOfFunc
BEGIN_LINE
0	116288	0	3	-1	0
END_LINE
finishport

3	2	coords= 510 -895 0	1	0	1	mandatory= 0	0	1
err

funcName= calibrate 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

calibrate
_
115808	0	0	0
_
_
Calibrate
Calibrate
0
_
430	-820	515	-773
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0xAD13
Instance_Info	PT200 Cali
End_Instance
END_BA
parameter
id	1	-1	10000	0	0	0	0	1.2	2
EndOfValues
EndOfLabels
calibration id

inputport

0	2	coords= 429 -795 0	0	0	1	mandatory= 0	0	1
scale
p2s
funcName= calibrate 1	1	EndOfFunc
BEGIN_LINE
1	116081	0	0	-1	0
362	-835
362	-795
END_LINE
inputport

0	2	coords= 429 -783 0	1	0	1	mandatory= 0	0	1
offset
p2o
funcName= calibrate 1	2	EndOfFunc
BEGIN_LINE
1	116083	0	0	-1	0
362	-795
372	-783
END_LINE
outputport

1	1	coords= 515 -783 0	0	0	1	mandatory= 0	0	1
errno

funcName= calibrate 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 429 -808 0	0	0	0	mandatory= 0	0	1
run

funcName= calibrate 1	0	EndOfFunc
BEGIN_LINE
1	116082	0	2	-1	0
390	-862
390	-808
END_LINE
finishport

3	2	coords= 515 -808 0	0	0	1	mandatory= 0	0	1
--

funcName= calibrate 1	4	EndOfFunc
BEGIN_LINE
0	116288	1	3	-1	0
END_LINE
finishport

3	2	coords= 515 -795 0	1	0	1	mandatory= 0	0	1
err

funcName= calibrate 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

calibrate
_
115809	0	0	0
_
_
Calibrate
Calibrate
0
_
430	-720	515	-673
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0xAD13
Instance_Info	Therm1 Cali
End_Instance
END_BA
parameter
id	1	-1	10000	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
calibration id

inputport

0	2	coords= 429 -695 0	0	0	1	mandatory= 0	0	1
scale
t1s
funcName= calibrate 1	1	EndOfFunc
BEGIN_LINE
1	116117	0	0	-1	0
380	-740
380	-695
END_LINE
inputport

0	2	coords= 429 -683 0	1	0	1	mandatory= 0	0	1
offset
t1o
funcName= calibrate 1	2	EndOfFunc
BEGIN_LINE
1	116119	0	0	-1	0
357	-700
367	-683
END_LINE
outputport

1	1	coords= 515 -683 0	0	0	1	mandatory= 0	0	1
errno

funcName= calibrate 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 429 -708 0	0	0	0	mandatory= 0	0	1
run

funcName= calibrate 1	0	EndOfFunc
BEGIN_LINE
1	116118	0	2	-1	0
392	-767
392	-708
END_LINE
finishport

3	2	coords= 515 -708 0	0	0	1	mandatory= 0	0	1
--

funcName= calibrate 1	4	EndOfFunc
BEGIN_LINE
0	116288	2	3	-1	0
END_LINE
finishport

3	2	coords= 515 -695 0	1	0	1	mandatory= 0	0	1
err

funcName= calibrate 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

calibrate
_
115810	0	0	0
_
_
Calibrate
Calibrate
0
_
430	-630	515	-583
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0xAD13
Instance_Info	0-10V Cali
End_Instance
END_BA
parameter
id	1	-1	10000	0	0	0	0	1.2	4
EndOfValues
EndOfLabels
calibration id

inputport

0	2	coords= 429 -605 0	0	0	1	mandatory= 0	0	1
scale
z1s
funcName= calibrate 1	1	EndOfFunc
BEGIN_LINE
1	116156	0	0	-1	0
362	-645
362	-605
END_LINE
inputport

0	2	coords= 429 -593 0	1	0	1	mandatory= 0	0	1
offset
z1o
funcName= calibrate 1	2	EndOfFunc
BEGIN_LINE
1	116158	0	0	-1	0
362	-605
372	-593
END_LINE
outputport

1	1	coords= 515 -593 0	0	0	1	mandatory= 0	0	1
errno

funcName= calibrate 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 429 -618 0	0	0	0	mandatory= 0	0	1
run

funcName= calibrate 1	0	EndOfFunc
BEGIN_LINE
1	116157	0	2	-1	0
392	-672
392	-618
END_LINE
finishport

3	2	coords= 515 -618 0	0	0	1	mandatory= 0	0	1
--

funcName= calibrate 1	4	EndOfFunc
BEGIN_LINE
0	116288	3	3	-1	0
END_LINE
finishport

3	2	coords= 515 -605 0	1	0	1	mandatory= 0	0	1
err

funcName= calibrate 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

calibrate
_
115811	0	0	0
_
_
Calibrate
Calibrate
0
_
430	-540	515	-493
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0xAD13
Instance_Info	4-20mA Cali
End_Instance
END_BA
parameter
id	1	-1	10000	0	0	0	0	1.2	5
EndOfValues
EndOfLabels
calibration id

inputport

0	2	coords= 429 -515 0	0	0	1	mandatory= 0	0	1
scale
f1s
funcName= calibrate 1	1	EndOfFunc
BEGIN_LINE
1	116198	0	0	-1	0
362	-545
362	-515
END_LINE
inputport

0	2	coords= 429 -503 0	1	0	1	mandatory= 0	0	1
offset
f1o
funcName= calibrate 1	2	EndOfFunc
BEGIN_LINE
1	116200	0	0	-1	0
362	-505
372	-503
END_LINE
outputport

1	1	coords= 515 -503 0	0	0	1	mandatory= 0	0	1
errno

funcName= calibrate 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 429 -528 0	0	0	0	mandatory= 0	0	1
run

funcName= calibrate 1	0	EndOfFunc
BEGIN_LINE
1	116199	0	2	-1	0
392	-572
392	-528
END_LINE
finishport

3	2	coords= 515 -528 0	0	0	1	mandatory= 0	0	1
--

funcName= calibrate 1	4	EndOfFunc
BEGIN_LINE
0	116282	0	3	-1	0
END_LINE
finishport

3	2	coords= 515 -515 0	1	0	1	mandatory= 0	0	1
err

funcName= calibrate 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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
1	115334	0	0	-1	1
-295	-945
-295	-725
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
0	116050	0	1	-1	0
END_LINE
outputport

1	3	coords= -55 -635 0	3	0	1	mandatory= 0	0	1
Key

funcName= parse 2	3	EndOfFunc
BEGIN_LINE
0	116046	0	1	-1	0
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
1	115334	0	2	-1	1
-280	-958
-280	-735
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
0	94420	0	3	-1	0
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

funcName= eos 3	1	EndOfFunc
BEGIN_LINE
0	94423	2	3	-1	0
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
0	116050	0	3	-1	0
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
-5	-565
-5	-510
-250	-510
-250	-570
END_LINE
startport

2	3	coords= -206 -550 0	2	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	116041	4	2	-1	0
-5	-685
-5	-510
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


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	116050	0	2	-1	0
80	-595
80	-510
-250	-510
-250	-620
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

funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	116041	3	0	-1	0
42	-635
42	-960
END_LINE
inputport

0	3	coords= 129 -940 0	1	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	115567	0	0	-1	0
74	-1139
74	-940
END_LINE
inputport

0	3	coords= 129 -925 0	2	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	115568	0	0	-1	0
74	-1109
74	-925
END_LINE
inputport

0	3	coords= 129 -910 0	3	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	115569	0	0	-1	0
74	-1079
74	-910
END_LINE
inputport

0	3	coords= 129 -895 0	4	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	115570	0	0	-1	0
74	-1049
74	-895
END_LINE
inputport

0	3	coords= 129 -880 0	5	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
1	115575	0	0	-1	0
74	-1019
74	-880
END_LINE
inputport

0	3	coords= 129 -865 0	6	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
1	115576	0	0	-1	0
74	-989
74	-865
END_LINE
inputport

0	3	coords= 129 -850 0	7	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
1	115577	0	0	-1	0
74	-959
74	-850
END_LINE
inputport

0	3	coords= 129 -835 0	8	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	coords= 129 -970 0	0	0	0	mandatory= 0	0	1
go

funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	116050	0	2	-1	0
79	-595
79	-970
END_LINE
finishport

3	3	coords= 160 -940 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	116047	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -925 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	116047	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -910 0	2	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	116049	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -895 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	116081	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -880 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	116083	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -865 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	116117	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -850 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	116119	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -835 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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

funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	116041	3	0	-1	0
42	-635
42	-755
END_LINE
inputport

0	3	coords= 129 -735 0	1	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	115571	0	0	-1	0
74	-919
74	-735
END_LINE
inputport

0	3	coords= 129 -720 0	2	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	115572	0	0	-1	0
74	-889
74	-720
END_LINE
inputport

0	3	coords= 129 -705 0	3	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	115573	0	0	-1	0
74	-859
74	-705
END_LINE
inputport

0	3	coords= 129 -690 0	4	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	115574	0	0	-1	0
74	-829
74	-690
END_LINE
inputport

0	3	coords= 129 -675 0	5	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -660 0	6	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -645 0	7	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 129 -630 0	8	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	coords= 129 -765 0	0	0	0	mandatory= 0	0	1
go

funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	116050	0	2	-1	0
79	-595
79	-765
END_LINE
finishport

3	3	coords= 160 -735 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	116156	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -720 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	116158	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -705 0	2	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	116198	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -690 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	116200	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -675 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -660 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -645 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 160 -630 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
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

mux_1r
_
116047	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-950	285	-920
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -930 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-930
END_LINE
outputport

1	2	coords= 285 -930 0	0	0	1	mandatory= 0	0	1

p1s
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116280	0	1	-1	0
END_LINE
startport

2	2	coords= 249 -940 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116045	1	2	-1	0
199	-925
209	-940
END_LINE
finishport

3	2	coords= 285 -940 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
116048	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
335	-975	355	-940
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_2
trans	-1	0	0	0
offset	0	0
hash	0x2A9C
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 334 -965 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116047	0	2	-1	0
309	-940
309	-965
END_LINE
startport

2	-1	coords= 334 -950 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116049	0	2	-1	0
315	-900
315	-950
END_LINE
finishport

3	-1	coords= 341 -957 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	115807	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116049	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-910	285	-880
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -890 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-890
END_LINE
outputport

1	2	coords= 285 -890 0	0	0	1	mandatory= 0	0	1

p1o
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116282	1	1	-1	0
END_LINE
startport

2	2	coords= 249 -900 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116045	2	2	-1	0
209	-910
219	-900
END_LINE
finishport

3	2	coords= 285 -900 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_transrx
_
116050	0	0	0
_
_
This is a string to real converter.
ConvertorStringToReal
0
_
10	-605	40	-575
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

0	3	coords= 9 -585 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorStringToReal 1	1	EndOfFunc
BEGIN_LINE
1	116041	2	0	-1	0
-18	-585
-8	-585
END_LINE
outputport

1	2	coords= 40 -585 0	0	0	1	mandatory= 0	0	1

calib_json_value
funcName= Run_ConvertorStringToReal 1	2	EndOfFunc
BEGIN_LINE
0	116200	0	1	-1	0
END_LINE
startport

2	2	coords= 9 -595 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorStringToReal 1	0	EndOfFunc
BEGIN_LINE
1	116041	8	2	-1	0
-28	-605
-18	-595
END_LINE
finishport

3	3	coords= 40 -595 0	8	0	1	mandatory= 0	0	1


funcName= Run_ConvertorStringToReal 1	3	EndOfFunc
BEGIN_LINE
0	116045	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116081	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-855	285	-825
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -835 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-835
END_LINE
outputport

1	2	coords= 285 -835 0	0	0	1	mandatory= 0	0	1

p2s
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116282	0	1	-1	0
END_LINE
startport

2	2	coords= 249 -845 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116045	3	2	-1	0
209	-895
209	-845
END_LINE
finishport

3	2	coords= 285 -845 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
116082	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
340	-880	360	-845
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_2
trans	-1	0	0	0
offset	0	0
hash	0x2A9C
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 339 -870 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116081	0	2	-1	0
312	-845
312	-870
END_LINE
startport

2	-1	coords= 339 -855 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116083	0	2	-1	0
317	-805
317	-855
END_LINE
finishport

3	-1	coords= 346 -862 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	115808	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116083	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-815	285	-785
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -795 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-795
END_LINE
outputport

1	2	coords= 285 -795 0	0	0	1	mandatory= 0	0	1

p2o
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116280	3	1	-1	0
END_LINE
startport

2	2	coords= 249 -805 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116045	4	2	-1	0
200	-880
200	-805
END_LINE
finishport

3	2	coords= 285 -805 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116117	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-760	285	-730
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -740 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-740
END_LINE
outputport

1	2	coords= 285 -740 0	0	0	1	mandatory= 0	0	1

t1s
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116283	0	1	-1	0
END_LINE
startport

2	2	coords= 249 -750 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116045	5	2	-1	0
190	-865
190	-750
END_LINE
finishport

3	2	coords= 285 -750 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
116118	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
340	-785	360	-750
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_2
trans	-1	0	0	0
offset	0	0
hash	0x2A9C
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 339 -775 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116117	0	2	-1	0
312	-750
312	-775
END_LINE
startport

2	-1	coords= 339 -760 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116119	0	2	-1	0
317	-710
317	-760
END_LINE
finishport

3	-1	coords= 346 -767 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	115809	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116119	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-720	285	-690
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -700 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-700
END_LINE
outputport

1	2	coords= 285 -700 0	0	0	1	mandatory= 0	0	1

t1o
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116283	1	1	-1	0
END_LINE
startport

2	2	coords= 249 -710 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116045	6	2	-1	0
180	-850
180	-710
END_LINE
finishport

3	2	coords= 285 -710 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116156	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-665	285	-635
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -645 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-645
END_LINE
outputport

1	2	coords= 285 -645 0	0	0	1	mandatory= 0	0	1

z1s
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116285	0	1	-1	0
END_LINE
startport

2	2	coords= 249 -655 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116046	0	2	-1	0
230	-735
230	-655
END_LINE
finishport

3	2	coords= 285 -655 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
116157	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
340	-690	360	-655
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_2
trans	-1	0	0	0
offset	0	0
hash	0x2A9C
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 339 -680 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116156	0	2	-1	0
312	-655
312	-680
END_LINE
startport

2	-1	coords= 339 -665 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116158	0	2	-1	0
317	-615
317	-665
END_LINE
finishport

3	-1	coords= 346 -672 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	115810	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116158	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-625	285	-595
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -605 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
139	-585
139	-605
END_LINE
outputport

1	2	coords= 285 -605 0	0	0	1	mandatory= 0	0	1

z1o
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116285	1	1	-1	0
END_LINE
startport

2	2	coords= 249 -615 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116046	1	2	-1	0
220	-720
220	-615
END_LINE
finishport

3	2	coords= 285 -615 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116198	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-565	285	-535
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -545 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
149	-585
149	-545
END_LINE
outputport

1	2	coords= 285 -545 0	0	0	1	mandatory= 0	0	1

f1s
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116286	0	1	-1	0
END_LINE
startport

2	2	coords= 249 -555 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116046	2	2	-1	0
209	-705
209	-555
END_LINE
finishport

3	2	coords= 285 -555 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
116199	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
340	-590	360	-555
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_2
trans	-1	0	0	0
offset	0	0
hash	0x2A9C
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 339 -580 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116198	0	2	-1	0
312	-555
312	-580
END_LINE
startport

2	-1	coords= 339 -565 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116200	0	2	-1	0
317	-515
317	-565
END_LINE
finishport

3	-1	coords= 346 -572 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	115811	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_1r
_
116200	0	0	0
_
_
This is a single input real value latch.
MultiplexOneInputReal
0
_
250	-525	285	-495
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	4	1.25	0	Latch
trans	-1	0	0	0
offset	0	0
hash	0x7442
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= 249 -505 0	0	0	1	mandatory= 0	0	1
i
calib_json_value
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116050	0	0	-1	0
149	-585
149	-505
END_LINE
outputport

1	2	coords= 285 -505 0	0	0	1	mandatory= 0	0	1

f1o
funcName= Run_Input1 1	2	EndOfFunc
BEGIN_LINE
0	116286	1	1	-1	0
END_LINE
startport

2	2	coords= 249 -515 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116046	3	2	-1	0
190	-690
190	-515
END_LINE
finishport

3	2	coords= 285 -515 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	EndOfFunc
BEGIN_LINE
0	116048	1	3	-1	0
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
write_data_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	116284	0	0	-1	0
915	-820
915	-890
-421	-890
-421	-1320
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
write_data
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	116284	0	2	-1	0
915	-830
915	-890
-421	-890
-421	-1345
END_LINE
finishport

3	1	coords= -310 -1395 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	115335	0	3	-1	0
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
0	116287	1	3	-1	0
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
1	116287	0	2	-1	0
896	-610
896	-665
-766	-665
-766	-1350
END_LINE
finishport

3	3	coords= -690 -1350 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
116280	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
690	-925	740	-780
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
Format	3	0	0	0	0	0	0	1.2	[%.2f,%.2f], [%.2f,%.2f], [%.2f,%.2f], [%.2f,%.2f],
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 689 -903 0	0	0	1	mandatory= 0	0	1
r1
p1s
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	116047	0	0	-1	0
487	-930
487	-903
END_LINE
inputport

0	2	coords= 689 -893 0	1	0	1	mandatory= 0	0	1
r2
p1o
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	116049	0	0	-1	0
492	-890
502	-893
END_LINE
inputport

0	2	coords= 689 -883 0	0	0	1	mandatory= 0	0	1
r3
p2s
funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	116081	0	0	-1	0
497	-835
497	-883
END_LINE
inputport

0	2	coords= 689 -873 0	1	0	1	mandatory= 0	0	1
r4
p2o
funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	116083	0	0	-1	0
482	-795
482	-873
END_LINE
inputport

0	2	coords= 689 -863 0	0	0	1	mandatory= 0	0	1
r5
t1s
funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	116117	0	0	-1	0
507	-740
507	-863
END_LINE
inputport

0	2	coords= 689 -853 0	1	0	1	mandatory= 0	0	1
r6
t1o
funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	116119	0	0	-1	0
512	-700
512	-853
END_LINE
inputport

0	2	coords= 689 -843 0	0	0	1	mandatory= 0	0	1
r7
z1s
funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	116156	0	0	-1	0
517	-645
517	-843
END_LINE
inputport

0	2	coords= 689 -833 0	1	0	1	mandatory= 0	0	1
r8
z1o
funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	116158	0	0	-1	0
522	-605
522	-833
END_LINE
inputport

0	3	coords= 689 -813 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 740 -903 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	116284	0	1	-1	0
END_LINE
startport

2	3	coords= 689 -913 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	116288	0	2	-1	0
630	-852
630	-913
END_LINE
finishport

3	3	coords= 740 -913 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	116284	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
116282	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
700	-750	750	-605
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
Format	3	0	0	0	0	0	0	1.2	[%.2f,%.2f]
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= 699 -728 0	0	0	1	mandatory= 0	0	1
r1
f1s
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	116198	0	0	-1	0
492	-545
492	-728
END_LINE
inputport

0	2	coords= 699 -718 0	1	0	1	mandatory= 0	0	1
r2
f1o
funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	116200	0	0	-1	0
497	-505
497	-718
END_LINE
inputport

0	2	coords= 699 -708 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 699 -698 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 699 -688 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 699 -678 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 699 -668 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= 699 -658 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 699 -638 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 750 -728 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	116284	1	1	-1	0
END_LINE
startport

2	3	coords= 699 -738 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	115811	0	2	-1	0
602	-528
602	-738
END_LINE
finishport

3	3	coords= 750 -738 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	116287	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
116284	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
830	-855	860	-785
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x0094
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 829 -820 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	116280	0	0	-1	0
775	-903
775	-820
END_LINE
inputport

0	3	coords= 829 -795 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	116282	0	0	-1	0
800	-728
800	-795
END_LINE
outputport

1	3	coords= 860 -820 0	0	0	1	mandatory= 0	0	1

write_data_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	116268	1	1	-1	0
END_LINE
startport

2	3	coords= 829 -830 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	116280	0	2	-1	0
789	-913
789	-830
END_LINE
startport

2	3	coords= 829 -805 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	116282	0	2	-1	0
784	-738
784	-805
END_LINE
finishport

3	3	coords= 860 -830 0	0	0	1	mandatory= 0	0	1

write_data
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	116268	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
116287	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
835	-630	855	-580
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

2	-1	coords= 834 -620 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116287	1	2	-1	0
905	-590
905	-650
784	-650
784	-620
END_LINE
startport

2	-1	coords= 834 -605 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	116268	2	2	-1	0
267	-1345
267	-605
END_LINE
startport

2	-1	coords= 834 -590 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	116282	0	2	-1	0
770	-738
770	-590
END_LINE
finishport

3	-1	coords= 841 -610 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	116279	0	3	-1	0
END_LINE
finishport

3	-1	coords= 841 -590 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	116287	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand4
_
116288	0	0	0
_
_
This is a 4-input event latching AND with an internal reset.
EventSelfResetFourInputLatchingAnd
0
_
575	-885	595	-820
1	0
blockattr
BEGIN_BA
type	And_Event
type_var	stdn_4
trans	-1	0	0	0
offset	0	0
hash	0x774B
Instance_Info	_
End_Instance
END_BA
startport

2	-1	coords= 574 -875 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	115807	0	2	-1	0
547	-908
547	-875
END_LINE
startport

2	-1	coords= 574 -860 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	115808	0	2	-1	0
539	-808
539	-860
END_LINE
startport

2	-1	coords= 574 -845 0	2	0	0	mandatory= 1	0	1


funcName= And3 3	0	EndOfFunc
BEGIN_LINE
1	115809	0	2	-1	0
545	-708
545	-845
END_LINE
startport

2	-1	coords= 574 -830 0	3	0	0	mandatory= 0	0	1


funcName= And4 4	0	EndOfFunc
BEGIN_LINE
1	115810	0	2	-1	0
555	-618
555	-830
END_LINE
finishport

3	-1	coords= 581 -852 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	funcName= And3 3	1	funcName= And4 4	1	EndOfFunc
BEGIN_LINE
0	116280	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
