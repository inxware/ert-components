#V:2.1.2

CanvasSizeXY
990	835

IconData
BEGIN_BLOCK

file_wos
_
1	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, <nl?> for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii format data for the value prepended with zeros. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.
FILE_WriteOnly_String
0
_
-330	-690	-265	-570
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
Append	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Append rather than overwrite.

parameter
Binary	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Binary file mode

inputport

0	3	coords= -331 -655 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_WriteOnly_String 1	1	EndOfFunc
BEGIN_LINE
1	5	0	0	-1	0
-398	-655
-388	-655
END_LINE
inputport

0	3	coords= -331 -595 0	1	0	1	mandatory= 0	0	1
data
write_data_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	65	0	0	-1	0
-30	-275
-30	-5
-386	-5
-386	-595
END_LINE
inputport

0	1	coords= -331 -585 0	2	0	1	mandatory= 0	0	1
size

funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -265 -590 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -331 -665 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_WriteOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	5	0	2	-1	0
-398	-665
-388	-665
END_LINE
startport

2	3	coords= -331 -640 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	0
-365	-620
-365	-640
END_LINE
startport

2	3	coords= -331 -615 0	2	0	0	mandatory= 0	0	1
write
write_data
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	65	0	2	-1	0
-30	-285
-30	-5
-386	-5
-386	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
start_test
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	72	0	3	-1	0
END_LINE
finishport

3	3	coords= -265 -640 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_WriteOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -265 -615 0	2	0	1	mandatory= 0	0	1
--
write_done
funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	72	0	3	-1	0
END_LINE
finishport

3	3	coords= -265 -655 0	0	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_WriteOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	6	1	3	-1	0
END_LINE
finishport

3	1	coords= -265 -630 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_WriteOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	7	0	3	-1	0
END_LINE
finishport

3	1	coords= -265 -605 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_WriteFILE_WriteOnly_String 3	5	EndOfFunc
BEGIN_LINE
0	7	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

fs_dir_create_remove
_
2	0	0	0
_
_
Allows a directory to be created or removed. Parametes available to allow complete removal of internal data too.
FileSystemDirCreateRemove
0
_
-545	-815	-480	-743
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

0	3	coords= -546 -780 0	0	0	1	mandatory= 0	0	1
path

funcName= create 1	1	funcName= remove 2	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
-603	-789
-593	-780
END_LINE
startport

2	0	coords= -546 -790 0	0	1	0	mandatory= 0	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -546 -763 0	1	0	0	mandatory= 0	0	1
remove

funcName= remove 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -480 -790 0	0	0	1	mandatory= 0	0	1


funcName= create 1	2	EndOfFunc
BEGIN_LINE
0	5	0	3	-1	0
END_LINE
finishport

3	3	coords= -480 -780 0	1	0	1	mandatory= 0	0	1
err

funcName= create 1	3	EndOfFunc
BEGIN_LINE
0	6	0	3	-1	0
END_LINE
finishport

3	3	coords= -480 -763 0	2	0	1	mandatory= 0	0	1


funcName= remove 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -480 -753 0	3	0	1	mandatory= 0	0	1
err

funcName= remove 2	3	EndOfFunc
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
-790	-800	-650	-779
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

1	3	coords= -650 -789 0	0	0	1	mandatory= 0	0	1


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
5	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
-505	-680	-475	-635
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

0	3	coords= -506 -655 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
-605	-789
-605	-655
END_LINE
inputport

0	3	coords= -506 -645 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -475 -655 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
END_LINE
startport

2	3	coords= -506 -665 0	0	0	0	mandatory= 0	0	1


funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
-430	-790
-430	-700
-556	-700
-556	-665
END_LINE
finishport

3	3	coords= -475 -665 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor3
_
8	0	0	0
_
_
This is a 3-input event OR.
EventThreeInputOr
0
_
-160	-790	-140	-740
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

2	-1	coords= -161 -780 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	2	1	2	-1	0
-286	-780
-286	-780
END_LINE
startport

2	-1	coords= -161 -765 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	3	2	-1	0
-205	-655
-205	-765
END_LINE
startport

2	-1	coords= -161 -750 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-190	-605
-190	-750
END_LINE
finishport

3	-1	coords= -148 -765 0	0	0	1	mandatory= 0	0	1

test_err
funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
9	0	0	0
_
_
<nl?>        
xcomment
0
_
-400	-555	-102	-520
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
19	0	0	0
_
_
This is a 3-input event OR.
EventThreeInputOr
0
_
-400	-645	-380	-595
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

2	-1	coords= -401 -635 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	1	4	2	-1	0
-215	-630
-215	-710
-451	-710
-451	-635
END_LINE
startport

2	-1	coords= -401 -620 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-220	-605
-220	-740
-456	-740
-456	-620
END_LINE
startport

2	-1	coords= -401 -605 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	22	0	2	-1	0
-415	-571
-415	-605
END_LINE
finishport

3	-1	coords= -388 -620 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wos
_
20	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, <nl?> for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii format data for the value prepended with zeros. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.
FILE_WriteOnly_String
0
_
-580	-620	-515	-500
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
Append	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Append rather than overwrite.

parameter
Binary	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Binary file mode

inputport

0	3	coords= -581 -585 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_WriteOnly_String 1	1	EndOfFunc
BEGIN_LINE
1	50	0	0	-1	0
-625	-610
-625	-585
END_LINE
inputport

0	3	coords= -581 -525 0	1	0	1	mandatory= 0	0	1
data

funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	21	0	0	-1	0
-613	-509
-613	-525
END_LINE
inputport

0	1	coords= -581 -515 0	2	0	1	mandatory= 0	0	1
size

funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -515 -520 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -581 -595 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_WriteOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	50	0	2	-1	0
-618	-620
-618	-595
END_LINE
startport

2	3	coords= -581 -570 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	20	2	2	-1	0
-465	-545
-465	-640
-631	-640
-631	-570
END_LINE
startport

2	3	coords= -581 -545 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	20	0	2	-1	0
-465	-595
-465	-640
-631	-640
-631	-545
END_LINE
finishport

3	3	coords= -515 -595 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	20	2	3	-1	0
END_LINE
finishport

3	3	coords= -515 -570 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_WriteOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	22	1	3	-1	0
END_LINE
finishport

3	1	coords= -515 -545 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	20	1	3	-1	0
END_LINE
finishport

3	1	coords= -515 -585 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_WriteOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	22	0	3	-1	0
END_LINE
finishport

3	1	coords= -515 -560 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_WriteOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	22	2	3	-1	0
END_LINE
finishport

3	1	coords= -515 -535 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_WriteFILE_WriteOnly_String 3	5	EndOfFunc
BEGIN_LINE
0	22	3	3	-1	0
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
-800	-520	-660	-499
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

1	3	coords= -660 -509 0	0	0	1	mandatory= 0	0	1


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
22	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-450	-605	-430	-540
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

2	-1	coords= -451 -595 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	20	3	2	-1	0
-478	-585
-468	-595
END_LINE
startport

2	-1	coords= -451 -580 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	20	1	2	-1	0
-478	-570
-468	-580
END_LINE
startport

2	-1	coords= -451 -565 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	20	4	2	-1	0
-478	-560
-468	-565
END_LINE
startport

2	-1	coords= -451 -550 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	20	5	2	-1	0
-478	-535
-468	-550
END_LINE
finishport

3	-1	coords= -438 -571 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	19	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
23	0	0	0
_
_
<nl?>        
xcomment
0
_
-815	-485	-552	-448
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
50	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
-675	-635	-645	-590
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

0	3	coords= -676 -610 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
-605	-789
-605	-655
-745	-655
-745	-610
END_LINE
inputport

0	3	coords= -676 -600 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -645 -610 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	20	0	1	-1	0
END_LINE
startport

2	3	coords= -676 -620 0	0	0	0	mandatory= 0	0	1

test_done
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	71	0	2	-1	0
-119	-10
-119	-5
-731	-5
-731	-620
END_LINE
finishport

3	3	coords= -645 -620 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
52	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
0
_
-1060	-345	-1025	-325
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x4CFB
Instance_Info	_
End_Instance
END_BA
parameter
Boolean Constant	0	0	0	1	5	4	0	1.2	1
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	coords= -1025 -335 0	0	0	1	mandatory= 0	0	1

TRUE
funcName= Run_ConstantBool 1	1	EndOfFunc
BEGIN_LINE
0	53	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantBool 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

PID
_
53	0	0	0
_
_
A PID controller with tailored specific configurations.
PID
0
_
-450	-415	-345	-125
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	40	5	1.25	0	PID
trans	-1	0	0	0
offset	0	-15
hash	0xB237
Instance_Info	_
End_Instance
END_BA
parameter
PIDNo	1	1	3	1	50	20	0	1.2	1
EndOfValues
EndOfLabels
The channel number of this PID controller

parameter
ISR Mode	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
If Non-ISR, the value are read from the "measured" port. If enabled, it aquires values from the ADC internally.

parameter
Sensor ID	1	0	10	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Sensor ID

parameter
P Default	2	-100000	100000	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Default value of P

parameter
I Default	2	-100000	100000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Default value of I

parameter
D Default	2	-100000	100000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Default value of D

parameter
Output Mode	1	0	10	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Output Mode

parameter
Output1 Default	1	0	10	0	0	0	0	1.2	2
EndOfValues
EndOfLabels
Output1 Default

parameter
Output2 Default	1	0	10	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Output2 Default

parameter
Output3 Default	1	0	10	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Output3 Default

parameter
Output Disabled	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Disables Output of the controller e.g. scheduler disabled mode.

parameter
Calibration Mode	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Calibration Mode

inputport

0	0	coords= -451 -380 0	0	0	1	mandatory= 0	0	1
isr mode
FALSE
funcName= init 1	1	EndOfFunc
BEGIN_LINE
1	62	0	0	-1	0
-576	-305
-576	-380
END_LINE
inputport

0	2	coords= -451 -348 0	1	0	1	mandatory= 0	0	1
value

funcName= new_measure 2	1	EndOfFunc
BEGIN_LINE
1	54	0	0	-1	0
-505	-295
-505	-348
END_LINE
inputport

0	2	coords= -451 -303 0	2	0	1	mandatory= 0	0	1
p

funcName= set_pid_config 4	1	EndOfFunc
BEGIN_LINE
1	56	0	0	-1	0
-498	-240
-498	-303
END_LINE
inputport

0	2	coords= -451 -293 0	3	0	1	mandatory= 0	0	1
i
const_f_zero
funcName= set_pid_config 4	2	EndOfFunc
BEGIN_LINE
1	57	0	0	-1	0
-558	-400
-558	-293
END_LINE
inputport

0	2	coords= -451 -283 0	4	0	1	mandatory= 0	0	1
d
const_f_zero
funcName= set_pid_config 4	3	EndOfFunc
BEGIN_LINE
1	57	0	0	-1	0
-558	-400
-558	-283
END_LINE
inputport

0	2	coords= -451 -273 0	5	0	1	mandatory= 0	0	1
max i

funcName= set_pid_config 4	4	EndOfFunc
BEGIN_LINE
1	58	0	0	-1	0
-498	-215
-498	-273
END_LINE
inputport

0	2	coords= -451 -263 0	6	0	1	mandatory= 0	0	1
max global

funcName= set_pid_config 4	5	EndOfFunc
BEGIN_LINE
1	59	0	0	-1	0
-498	-190
-498	-263
END_LINE
inputport

0	1	coords= -451 -240 0	7	0	1	mandatory= 0	0	1
sensor id

funcName= set_io_config 5	1	EndOfFunc
BEGIN_LINE
1	61	0	0	-1	0
-498	-165
-498	-240
END_LINE
inputport

0	1	coords= -451 -230 0	8	0	1	mandatory= 0	0	1
output mode
const_i_zero
funcName= set_io_config 5	2	EndOfFunc
BEGIN_LINE
1	60	0	0	-1	0
-558	-370
-558	-230
END_LINE
inputport

0	1	coords= -451 -220 0	9	0	1	mandatory= 0	0	1
output1

funcName= set_io_config 5	3	EndOfFunc
BEGIN_LINE
1	63	0	0	-1	0
-498	-135
-498	-220
END_LINE
inputport

0	1	coords= -451 -210 0	10	0	1	mandatory= 0	0	1
output2

funcName= set_io_config 5	4	EndOfFunc
BEGIN_LINE
1	64	0	0	-1	0
-498	-105
-498	-210
END_LINE
inputport

0	1	coords= -451 -200 0	11	0	1	mandatory= 0	0	1
output3
const_i_zero
funcName= set_io_config 5	5	EndOfFunc
BEGIN_LINE
1	60	0	0	-1	0
-558	-370
-558	-200
END_LINE
inputport

0	2	coords= -451 -325 0	13	0	1	mandatory= 0	0	1
value

funcName= set_point_value 3	1	EndOfFunc
BEGIN_LINE
1	55	0	0	-1	0
-498	-265
-498	-325
END_LINE
inputport

0	0	coords= -451 -190 0	14	0	1	mandatory= 0	0	1
relay check
FALSE
funcName= set_io_config 5	6	EndOfFunc
BEGIN_LINE
1	62	0	0	-1	0
-586	-305
-586	-190
END_LINE
inputport

0	0	coords= -451 -133 0	14	0	1	mandatory= 0	0	1
disabled

funcName= disable_ctrl 8	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -451 -370 0	15	0	1	mandatory= 0	0	1
calib

funcName= init 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	2	coords= -345 -228 0	0	0	1	mandatory= 0	0	1
value

funcName= new_measure 2	2	funcName= get_isr_value 6	1	EndOfFunc
BEGIN_LINE
0	66	0	1	-1	0
END_LINE
outputport

1	2	coords= -345 -195 0	1	0	1	mandatory= 0	0	1
out%

funcName= get_ctrl_value 7	1	EndOfFunc
BEGIN_LINE
0	67	0	1	-1	0
END_LINE
outputport

1	0	coords= -345 -185 0	2	0	1	mandatory= 0	0	1
pwm1

funcName= get_ctrl_value 7	2	EndOfFunc
BEGIN_LINE
0	68	0	1	-1	0
END_LINE
outputport

1	0	coords= -345 -175 0	3	0	1	mandatory= 0	0	1
pwm2

funcName= get_ctrl_value 7	3	EndOfFunc
BEGIN_LINE
0	69	0	1	-1	0
END_LINE
outputport

1	0	coords= -345 -165 0	4	0	1	mandatory= 0	0	1
pwm3

funcName= get_ctrl_value 7	4	EndOfFunc
BEGIN_LINE
0	70	0	1	-1	0
END_LINE
outputport

1	0	coords= -345 -218 0	5	0	1	mandatory= 0	0	1
connected

funcName= new_measure 2	3	funcName= get_isr_value 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -451 -390 0	0	0	0	mandatory= 0	0	1
init
start_test
funcName= init 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-5
-506	-5
-506	-390
END_LINE
startport

2	0	coords= -451 -358 0	1	0	0	mandatory= 0	0	1
measure

funcName= new_measure 2	0	EndOfFunc
BEGIN_LINE
1	73	0	2	-1	0
-695	-195
-695	-358
END_LINE
startport

2	2	coords= -451 -313 0	2	0	0	mandatory= 0	0	1
pid config

funcName= set_pid_config 4	0	EndOfFunc
BEGIN_LINE
1	53	2	2	-1	0
-295	-335
-295	-435
-501	-435
-501	-313
END_LINE
startport

2	2	coords= -451 -250 0	3	0	0	mandatory= 0	0	1
io config

funcName= set_io_config 5	0	EndOfFunc
BEGIN_LINE
1	53	3	2	-1	0
-295	-313
-295	-435
-501	-435
-501	-250
END_LINE
startport

2	1	coords= -451 -165 0	4	0	0	mandatory= 0	0	1
get isr

funcName= get_isr_value 6	0	EndOfFunc
BEGIN_LINE
1	53	1	2	-1	0
-295	-358
-295	-435
-501	-435
-501	-165
END_LINE
startport

2	1	coords= -451 -155 0	5	0	0	mandatory= 0	0	1
get ctrl

funcName= get_ctrl_value 7	0	EndOfFunc
BEGIN_LINE
1	72	2	2	-1	0
-715	-270
-715	-155
END_LINE
startport

2	1	coords= -451 -335 0	6	0	0	mandatory= 0	0	1
set point

funcName= set_point_value 3	0	EndOfFunc
BEGIN_LINE
1	53	0	2	-1	0
-295	-390
-295	-435
-501	-435
-501	-335
END_LINE
startport

2	0	coords= -451 -143 0	7	0	0	mandatory= 0	0	1
set

funcName= disable_ctrl 8	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -345 -390 0	0	0	1	mandatory= 0	0	1
--

funcName= init 1	3	EndOfFunc
BEGIN_LINE
0	53	6	3	-1	0
END_LINE
finishport

3	1	coords= -345 -358 0	1	0	1	mandatory= 0	0	1
--

funcName= new_measure 2	4	EndOfFunc
BEGIN_LINE
0	53	4	3	-1	0
END_LINE
finishport

3	1	coords= -345 -335 0	2	0	1	mandatory= 0	0	1
--

funcName= set_point_value 3	2	EndOfFunc
BEGIN_LINE
0	53	2	3	-1	0
END_LINE
finishport

3	2	coords= -345 -313 0	3	0	1	mandatory= 0	0	1
--

funcName= set_pid_config 4	6	EndOfFunc
BEGIN_LINE
0	53	3	3	-1	0
END_LINE
finishport

3	2	coords= -345 -253 0	4	0	1	mandatory= 0	0	1
--

funcName= set_io_config 5	7	EndOfFunc
BEGIN_LINE
0	72	0	3	-1	0
END_LINE
finishport

3	2	coords= -345 -238 0	5	0	1	mandatory= 0	0	1
measured

funcName= new_measure 2	5	funcName= get_isr_value 6	3	EndOfFunc
BEGIN_LINE
0	66	0	3	-1	0
END_LINE
finishport

3	2	coords= -345 -205 0	6	0	1	mandatory= 0	0	1
ctrl

funcName= get_ctrl_value 7	5	EndOfFunc
BEGIN_LINE
0	68	0	3	-1	0
END_LINE
finishport

3	0	coords= -345 -143 0	7	0	1	mandatory= 0	0	1
--

funcName= disable_ctrl 8	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
54	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-650	-305	-555	-285
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9458
Instance_Info	_
End_Instance
END_BA
parameter
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	100
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -555 -295 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	53	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
55	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-650	-275	-555	-255
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9458
Instance_Info	_
End_Instance
END_BA
parameter
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	70
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -555 -265 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	53	13	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
56	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-650	-250	-555	-230
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9458
Instance_Info	_
End_Instance
END_BA
parameter
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	1
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -555 -240 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	53	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
57	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1065	-410	-970	-390
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9458
Instance_Info	_
End_Instance
END_BA
parameter
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	0
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -970 -400 0	0	0	1	mandatory= 0	0	1

const_f_zero
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	53	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
58	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-650	-225	-555	-205
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9458
Instance_Info	_
End_Instance
END_BA
parameter
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	0.2
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -555 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	53	5	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
59	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-650	-200	-555	-180
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9458
Instance_Info	_
End_Instance
END_BA
parameter
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	5
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -555 -190 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	53	6	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
60	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1065	-380	-970	-360
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9647
Instance_Info	_
End_Instance
END_BA
parameter
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	0
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -970 -370 0	0	0	1	mandatory= 0	0	1

const_i_zero
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	53	12	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
61	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-650	-175	-555	-155
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9647
Instance_Info	_
End_Instance
END_BA
parameter
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -555 -165 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	53	7	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
62	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
0
_
-1060	-315	-1025	-295
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x4CFB
Instance_Info	_
End_Instance
END_BA
parameter
Boolean Constant	0	0	0	1	5	4	0	1.2	0
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	coords= -1025 -305 0	0	0	1	mandatory= 0	0	1

FALSE
funcName= Run_ConstantBool 1	1	EndOfFunc
BEGIN_LINE
0	53	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantBool 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
63	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-650	-145	-555	-125
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9647
Instance_Info	_
End_Instance
END_BA
parameter
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	1
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -555 -135 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	53	9	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
64	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-650	-115	-555	-95
1	0
blockattr
BEGIN_BA
type	Data_Processor
trans	-1	0	0	0
offset	0	5
hash	0x9647
Instance_Info	_
End_Instance
END_BA
parameter
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	2
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -555 -105 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	53	10	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
65	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-105	-310	-75	-190
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

0	3	coords= -106 -275 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	66	0	0	-1	1
-150	-240
-150	-275
END_LINE
inputport

0	3	coords= -106 -250 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	67	0	0	-1	0
-125	-188
-125	-250
END_LINE
inputport

0	3	coords= -106 -225 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -106 -200 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -75 -275 0	0	0	1	mandatory= 0	0	1

write_data_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -106 -285 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	66	0	2	-1	1
-163	-250
-163	-285
END_LINE
startport

2	3	coords= -106 -260 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	67	0	2	-1	0
-135	-198
-135	-260
END_LINE
startport

2	3	coords= -106 -235 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -106 -210 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -75 -285 0	0	0	1	mandatory= 0	0	1

write_data
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
66	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
0
_
-260	-260	-230	-230
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0x1E6A
Instance_Info	_
End_Instance
END_BA
parameter
decimal places	1	0	6	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
Max number of decimal places

inputport

0	2	coords= -261 -240 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	1	EndOfFunc
BEGIN_LINE
1	53	0	0	-1	1
-303	-228
-293	-240
END_LINE
outputport

1	3	coords= -230 -240 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	2	EndOfFunc
BEGIN_LINE
0	65	0	1	-1	0
END_LINE
startport

2	3	coords= -261 -250 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	0	EndOfFunc
BEGIN_LINE
1	53	5	2	-1	1
-308	-238
-298	-250
END_LINE
finishport

3	3	coords= -230 -250 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	3	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
67	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
-195	-210	-145	-65
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
Format	3	0	0	0	0	0	0	1.2	%.1f,%.0f,%.0f,%.0f
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= -196 -188 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	53	1	0	-1	0
-271	-195
-261	-188
END_LINE
inputport

0	2	coords= -196 -178 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	68	0	0	-1	0
-236	-150
-236	-178
END_LINE
inputport

0	2	coords= -196 -168 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	69	0	0	-1	0
-225	-110
-225	-168
END_LINE
inputport

0	2	coords= -196 -158 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	70	0	0	-1	0
-215	-65
-215	-158
END_LINE
inputport

0	2	coords= -196 -148 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= -196 -138 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= -196 -128 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= -196 -118 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -196 -98 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -145 -188 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	65	1	1	-1	0
END_LINE
startport

2	3	coords= -196 -198 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	70	0	2	-1	0
-251	-75
-251	-198
END_LINE
finishport

3	3	coords= -145 -198 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	71	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbrx
_
68	0	0	0
_
_
This is a boolean to real converter.
ConvertorBoolToReal
0
_
-295	-170	-265	-140
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xFC95
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -296 -150 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	1	EndOfFunc
BEGIN_LINE
1	53	2	0	-1	0
-321	-185
-321	-150
END_LINE
outputport

1	2	coords= -265 -150 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	2	EndOfFunc
BEGIN_LINE
0	67	1	1	-1	0
END_LINE
startport

2	2	coords= -296 -160 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	0	EndOfFunc
BEGIN_LINE
1	53	6	2	-1	1
-316	-205
-316	-160
END_LINE
finishport

3	2	coords= -265 -160 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	3	EndOfFunc
BEGIN_LINE
0	69	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbrx
_
69	0	0	0
_
_
This is a boolean to real converter.
ConvertorBoolToReal
0
_
-295	-130	-265	-100
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xFC95
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -296 -110 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	1	EndOfFunc
BEGIN_LINE
1	53	3	0	-1	0
-321	-175
-321	-110
END_LINE
outputport

1	2	coords= -265 -110 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	2	EndOfFunc
BEGIN_LINE
0	67	2	1	-1	0
END_LINE
startport

2	2	coords= -296 -120 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	0	EndOfFunc
BEGIN_LINE
1	68	0	2	-1	0
-210	-160
-210	-180
-351	-180
-351	-120
END_LINE
finishport

3	2	coords= -265 -120 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	3	EndOfFunc
BEGIN_LINE
0	70	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbrx
_
70	0	0	0
_
_
This is a boolean to real converter.
ConvertorBoolToReal
0
_
-295	-85	-265	-55
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xFC95
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -296 -65 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	1	EndOfFunc
BEGIN_LINE
1	53	4	0	-1	0
-321	-165
-321	-65
END_LINE
outputport

1	2	coords= -265 -65 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	2	EndOfFunc
BEGIN_LINE
0	67	3	1	-1	0
END_LINE
startport

2	2	coords= -296 -75 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	0	EndOfFunc
BEGIN_LINE
1	69	0	2	-1	0
-210	-120
-210	-155
-351	-155
-351	-75
END_LINE
finishport

3	2	coords= -265 -75 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToReal 1	3	EndOfFunc
BEGIN_LINE
0	71	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
71	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-170	-30	-150	20
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

2	-1	coords= -171 -20 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	71	1	2	-1	0
-109	10
-109	-55
-226	-55
-226	-20
END_LINE
startport

2	-1	coords= -171 -5 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-223	-615
-223	-5
END_LINE
startport

2	-1	coords= -171 10 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	70	0	2	-1	0
-245	-75
-245	10
END_LINE
finishport

3	-1	coords= -164 -10 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	72	0	3	-1	0
END_LINE
finishport

3	-1	coords= -164 10 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	71	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
72	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
-845	-345	-780	-255
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	15	5	1.5	0	Timer
trans	-1	0	0	0
offset	0	-15
hash	0xEEFF
Instance_Info	_
End_Instance
END_BA
parameter
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	100000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= -846 -310 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -846 -300 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -846 -290 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	1	coords= -846 -320 0	0	0	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
1	53	4	2	-1	0
-295	-253
-295	-435
-896	-435
-896	-320
END_LINE
startport

2	0	coords= -846 -270 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
1	73	2	2	-1	0
-725	-185
-725	-355
-896	-355
-896	-270
END_LINE
finishport

3	0	coords= -780 -290 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	73	1	3	-1	0
END_LINE
finishport

3	0	coords= -780 -320 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	53	0	3	-1	0
END_LINE
finishport

3	0	coords= -780 -270 0	2	0	1	mandatory= 0	0	1


funcName= stop 3	1	EndOfFunc
BEGIN_LINE
0	53	5	3	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= tick 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

EventCounter_versatile1
_
73	0	0	0
_
_
event counter providing option to set a maximum and minimum threshold with an input value.
EventCounterVersatile1
0
_
-840	-235	-775	-60
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	5	5	1.5	0	Counter
trans	-1	0	0	0
offset	0	-15
hash	0xD5F3
Instance_Info	_
End_Instance
END_BA
parameter
Enable at Init	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Enable the counter at initialisation time if TRUE, else counter is disabled at initialisation time.

parameter
Initial Value	1	-2147483648	2147483647	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Initial Value for count.

parameter
Step Size	1	1	2147483647	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Value incremented/decremented on each count.

parameter
Max. Threshold	1	-2147483648	2147483647	0	0	0	0	1.2	10
EndOfValues
EndOfLabels
Threshold value for ovf event.

parameter
Min. Threshold	1	-2147483648	2147483647	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Threshold value for ovf event.

inputport

0	1	coords= -841 -80 0	0	0	1	mandatory= 0	0	1
max

funcName= Run_CountEventCounter 2	1	funcName= Run_DecrementEventCounter 3	1	funcName= Run_ResetEventCounter 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -841 -70 0	1	0	1	mandatory= 0	0	1
min

funcName= Run_CountEventCounter 2	2	funcName= Run_DecrementEventCounter 3	2	funcName= Run_ResetEventCounter 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -841 -100 0	2	0	1	mandatory= 0	0	1
ini

funcName= Run_CountEventCounter 2	3	funcName= Run_DecrementEventCounter 3	3	funcName= Run_ResetEventCounter 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -841 -90 0	3	0	1	mandatory= 0	0	1
step

funcName= Run_CountEventCounter 2	4	funcName= Run_DecrementEventCounter 3	4	funcName= Run_ResetEventCounter 4	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -775 -145 0	0	0	1	mandatory= 0	0	1
cnt

funcName= Run_CountEventCounter 2	5	funcName= Run_DecrementEventCounter 3	5	funcName= Run_ResetEventCounter 4	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= -841 -125 0	0	0	0	mandatory= 0	0	1
enabl

funcName= Run_StartEventCounter 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -841 -195 0	1	0	0	mandatory= 0	0	1
inc

funcName= Run_CountEventCounter 2	0	EndOfFunc
BEGIN_LINE
1	72	0	2	-1	0
-730	-290
-730	-365
-891	-365
-891	-195
END_LINE
startport

2	1	coords= -841 -170 0	2	0	0	mandatory= 0	0	1
dec

funcName= Run_DecrementEventCounter 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -841 -210 0	3	0	0	mandatory= 0	0	1
rst

funcName= Run_ResetEventCounter 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -841 -115 0	4	0	0	mandatory= 0	0	1
disabl

funcName= Run_StopEventCounter 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -775 -195 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_CountEventCounter 2	6	EndOfFunc
BEGIN_LINE
0	53	1	3	-1	0
END_LINE
finishport

3	1	coords= -775 -170 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_DecrementEventCounter 3	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -775 -185 0	2	0	1	mandatory= 0	0	1
ovf

funcName= Run_CountEventCounter 2	7	EndOfFunc
BEGIN_LINE
0	72	1	3	-1	0
END_LINE
finishport

3	1	coords= -775 -160 0	3	0	1	mandatory= 0	0	1
ovf

funcName= Run_DecrementEventCounter 3	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -775 -210 0	4	0	1	mandatory= 0	0	1
--

funcName= Run_ResetEventCounter 4	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -775 -125 0	5	0	1	mandatory= 0	0	1
--

funcName= Run_StartEventCounter 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -775 -115 0	6	0	1	mandatory= 0	0	1
--

funcName= Run_StopEventCounter 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
