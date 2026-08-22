#V:2.1.2

CanvasSizeXY
1990	3135

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
Append	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Append rather than overwrite.

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

0	3	coords= -331 -590 0	1	0	1	mandatory= 0	0	1
data
write_res_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	58	0	0	-1	0
480	-380
480	-450
-376	-450
-376	-590
END_LINE
outputport

1	1	coords= -265 -590 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
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
write_res
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	58	0	2	-1	0
480	-390
480	-450
-376	-450
-376	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	52	0	3	-1	0
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
write_res_done
funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	603	0	3	-1	0
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

funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
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

xcomment
_
4	0	0	0
_
_
<nl?>        
xcomment
0
_
-795	-860	-298	-823
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
Text	3	0	0	0	0	0	0	1.2	Create test 'results' directory. Make sure it doesn't exist before running test.
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
-610	-789
-610	-655
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
-425	-790
-425	-735
-551	-735
-551	-665
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
-435	-710
-435	-635
END_LINE
startport

2	-1	coords= -401 -620 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-215	-605
-215	-710
-435	-710
-435	-620
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
Append	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Append rather than overwrite.

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

0	3	coords= -581 -520 0	1	0	1	mandatory= 0	0	1
data

funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	21	0	0	-1	0
-613	-549
-613	-520
END_LINE
outputport

1	1	coords= -515 -520 0	0	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_WriteOnly_String 2	1	funcName= Run_OpenFILE_WriteOnly_String 1	2	funcName= Run_WriteFILE_WriteOnly_String 3	2	EndOfFunc
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
-485	-545
-485	-630
-610	-630
-610	-570
END_LINE
startport

2	3	coords= -581 -545 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	20	0	2	-1	0
-485	-595
-485	-630
-610	-630
-610	-545
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

funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
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

funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
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
-780	-560	-640	-539
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

1	3	coords= -640 -549 0	0	0	1	mandatory= 0	0	1


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
-685	-485	-422	-448
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
-610	-789
-610	-670
-721	-670
-721	-610
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
1	138	0	2	-1	0
-928	1675
-928	-620
END_LINE
finishport

3	3	coords= -645 -620 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
51	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-780	-245	-715	-120
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	File_RO
trans	-1	0	0	0
offset	0	-15
hash	0x560C
Instance_Info	Load sched1
End_Instance
END_BA
parameter
File name	3	0	0	0	0	0	0	1.2	sched1.scd
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
Use Application Dir	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

parameter
Binary	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Binary file mode

inputport

0	3	coords= -781 -210 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -715 -148 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	139	2	1	-1	0
END_LINE
outputport

1	1	coords= -715 -128 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -715 -138 0	2	0	1	mandatory= 0	0	1
size

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	139	3	1	-1	0
END_LINE
startport

2	0	coords= -781 -220 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
-866	-360
-866	-220
END_LINE
startport

2	3	coords= -781 -195 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	604	3	2	-1	0
-435	-128
-435	-280
-825	-280
-825	-195
END_LINE
startport

2	3	coords= -781 -170 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	51	0	2	-1	0
-670	-220
-670	-280
-825	-280
-825	-170
END_LINE
finishport

3	3	coords= -715 -220 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	51	2	3	-1	0
END_LINE
finishport

3	3	coords= -715 -195 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	58	0	3	-1	0
END_LINE
finishport

3	1	coords= -715 -170 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	139	1	3	-1	0
END_LINE
finishport

3	1	coords= -715 -210 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -715 -185 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -715 -160 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_manager
_
52	0	0	0
_
_
<nl?>        
state_manager
0
_
-1215	-405	-1150	-350
1	0
blockattr
BEGIN_BA
type	State_Manager
trans	-1	0	0	0
offset	0	0
hash	0x4639
Instance_Info	_
End_Instance
END_BA
parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	53
EndOfValues
EndOfLabels
_

outputport

1	1	coords= -1150 -365 0	0	0	1	mandatory= 0	0	1
state

funcName= enabled 1	2	EndOfFunc
BEGIN_LINE
0	605	0	1	-1	0
END_LINE
startport

2	1	coords= -1216 -395 0	0	0	0	mandatory= 0	0	1
enable
test_start
funcName= enabled 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-210	-665
-210	-440
-1261	-440
-1261	-395
END_LINE
finishport

3	1	coords= -1150 -395 0	0	0	1	mandatory= 0	0	1


funcName= enabled 1	3	EndOfFunc
BEGIN_LINE
0	54	0	3	-1	0
END_LINE
finishport

3	1	coords= -1150 -380 0	1	0	1	mandatory= 0	0	1


funcName= enabled 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
sm_outputport

6	6	coords= -1183 -411 0	0	0	1	mandatory= 0	0	1


funcName= enabled 1	1	EndOfFunc
BEGIN_LINE
0	53	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
53	0	0	0
_
_
<nl?>        
state
0
_
-1070	-495	-1010	-435
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Load1
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	53
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -1070 -465 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	52	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1011 -451 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	54	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
54	0	0	0
_
_
<nl?>        
state_condition
0
_
-970	-375	-940	-345
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	53
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	55
EndOfValues
EndOfLabels
_

startport

2	1	coords= -971 -360 0	0	0	0	mandatory= 0	0	1


funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	52	0	2	-1	0
-1066	-395
-1066	-360
END_LINE
finishport

3	6	coords= -940 -360 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	51	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -958 -374 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	53	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -954 -343 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	55	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
55	0	0	0
_
_
<nl?>        
state
0
_
-960	-175	-900	-115
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Load2
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	55
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -909 -167 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	54	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -913 -118 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	56	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
56	0	0	0
_
_
<nl?>        
state_condition
0
_
-970	0	-940	30
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	55
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	109
EndOfValues
EndOfLabels
_

startport

2	1	coords= -971 15 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-25
-1026	-25
-1026	15
END_LINE
finishport

3	6	coords= -940 15 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	57	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -961 1 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	55	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -952 31 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	109	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
57	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-775	35	-710	160
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
File name	3	0	0	0	0	0	0	1.2	sched2.scd
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
Use Application Dir	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

parameter
Binary	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Binary file mode

inputport

0	3	coords= -776 70 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -710 132 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	140	2	1	-1	0
END_LINE
outputport

1	1	coords= -710 152 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -710 142 0	2	0	1	mandatory= 0	0	1
size

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	140	3	1	-1	0
END_LINE
startport

2	0	coords= -776 60 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	56	0	2	-1	0
-863	15
-863	60
END_LINE
startport

2	3	coords= -776 85 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	605	3	2	-1	0
-435	202
-435	0
-810	0
-810	85
END_LINE
startport

2	3	coords= -776 110 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	57	0	2	-1	0
-665	60
-665	0
-810	0
-810	110
END_LINE
finishport

3	3	coords= -710 60 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	57	2	3	-1	0
END_LINE
finishport

3	3	coords= -710 85 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	58	1	3	-1	0
END_LINE
finishport

3	1	coords= -710 110 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	140	1	3	-1	0
END_LINE
finishport

3	1	coords= -710 70 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -710 95 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -710 120 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8s
_
58	0	0	0
_
_
This is a 8-input string multiplexer.
MultiplexEightInputString
0
_
395	-415	425	-195
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2E61
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 394 -380 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	59	0	0	-1	0
307	-354
307	-380
END_LINE
inputport

0	3	coords= 394 -355 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	83	0	0	-1	0
307	-319
307	-355
END_LINE
inputport

0	3	coords= 394 -330 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	108	0	0	-1	0
307	-279
307	-330
END_LINE
inputport

0	3	coords= 394 -305 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	607	0	0	-1	0
57	-145
57	-305
END_LINE
inputport

0	3	coords= 394 -280 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	690	0	0	-1	0
87	180
87	-280
END_LINE
inputport

0	3	coords= 394 -255 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	941	0	0	-1	0
112	355
112	-255
END_LINE
inputport

0	3	coords= 394 -230 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 394 -205 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 425 -380 0	0	0	1	mandatory= 0	0	1

write_res_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= 394 -390 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	51	1	2	-1	0
-640	-195
-640	-390
END_LINE
startport

2	3	coords= 394 -365 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	57	1	2	-1	0
-220	85
-220	-365
END_LINE
startport

2	3	coords= 394 -340 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	135	1	2	-1	0
-200	325
-200	-340
END_LINE
startport

2	3	coords= 394 -315 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	607	0	2	-1	0
57	-155
57	-315
END_LINE
startport

2	3	coords= 394 -290 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	690	0	2	-1	0
87	170
87	-290
END_LINE
startport

2	3	coords= 394 -265 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	941	0	2	-1	0
112	345
112	-265
END_LINE
startport

2	3	coords= 394 -240 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= 394 -215 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= 425 -390 0	0	0	1	mandatory= 0	0	1

write_res
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
59	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
70	-365	210	-344
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
String Constant	3	0	100	1	5	4	0	1.2	[sched1_loaded]
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 210 -354 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	58	0	1	-1	0
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
83	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
70	-330	210	-309
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
String Constant	3	0	100	1	5	4	0	1.2	[sched2_loaded]
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 210 -319 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	58	1	1	-1	0
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
108	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
70	-290	210	-269
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
String Constant	3	0	100	1	5	4	0	1.2	[sched3_loaded]
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 210 -279 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	58	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
109	0	0	0
_
_
<nl?>        
state
0
_
-1015	145	-955	205
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Load3
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	109
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -971 148 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	56	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -990 206 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	136	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_ros
_
135	0	0	0
_
_
Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.
FILE_ReadOnly_String
0
_
-775	275	-710	400
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
File name	3	0	0	0	0	0	0	1.2	sched3.scd
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
Use Application Dir	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Find Files in Application Directory rather than the user data directory

parameter
Binary	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Binary file mode

inputport

0	3	coords= -776 310 0	0	0	1	mandatory= 0	0	1
path

funcName= Run_OpenFILE_ReadOnly_String 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -710 372 0	0	0	1	mandatory= 0	0	1
data

funcName= Run_ReadFILE_ReadOnly_String 3	1	EndOfFunc
BEGIN_LINE
0	141	2	1	-1	0
END_LINE
outputport

1	1	coords= -710 392 0	1	0	1	mandatory= 0	0	1
err code

funcName= Run_CloseFILE_ReadOnly_String 2	1	funcName= Run_OpenFILE_ReadOnly_String 1	2	funcName= Run_ReadFILE_ReadOnly_String 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -710 382 0	2	0	1	mandatory= 0	0	1
size

funcName= Run_ReadFILE_ReadOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	141	3	1	-1	0
END_LINE
startport

2	0	coords= -776 300 0	0	0	0	mandatory= 0	0	1
open

funcName= Run_OpenFILE_ReadOnly_String 1	0	EndOfFunc
BEGIN_LINE
1	136	0	2	-1	0
-891	315
-881	300
END_LINE
startport

2	3	coords= -776 325 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_ReadOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	606	3	2	-1	0
-430	512
-430	240
-815	240
-815	325
END_LINE
startport

2	3	coords= -776 350 0	2	0	0	mandatory= 0	0	1
read

funcName= Run_ReadFILE_ReadOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	135	0	2	-1	0
-665	300
-665	240
-815	240
-815	350
END_LINE
finishport

3	3	coords= -710 300 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_OpenFILE_ReadOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	57	2	3	-1	0
END_LINE
finishport

3	3	coords= -710 325 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_CloseFILE_ReadOnly_String 2	2	EndOfFunc
BEGIN_LINE
0	58	2	3	-1	0
END_LINE
finishport

3	1	coords= -710 350 0	2	0	1	mandatory= 0	0	1
--

funcName= Run_ReadFILE_ReadOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	141	1	3	-1	0
END_LINE
finishport

3	1	coords= -710 310 0	3	0	1	mandatory= 0	0	1
err

funcName= Run_OpenFILE_ReadOnly_String 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -710 335 0	4	0	1	mandatory= 0	0	1
err

funcName= Run_CloseFILE_ReadOnly_String 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -710 360 0	5	0	1	mandatory= 0	0	1
err

funcName= Run_ReadFILE_ReadOnly_String 3	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
136	0	0	0
_
_
<nl?>        
state_condition
0
_
-1045	300	-1015	330
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	109
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	229
EndOfValues
EndOfLabels
_

startport

2	1	coords= -1046 315 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	275
-1101	275
-1101	315
END_LINE
finishport

3	6	coords= -1015 315 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	135	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -1027 300 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	109	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1022 329 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	229	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
137	0	0	0
_
_
<nl?>        
state
0
_
-1350	1690	-1290	1750
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Done
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	137
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -1333 1746 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	603	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1355 1716 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
138	0	0	0
_
_
<nl?>        
state_condition
0
_
-1565	1660	-1535	1690
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	137
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
_

startport

2	1	coords= -1566 1675 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-215	-615
-215	1640
-1616	1640
-1616	1675
END_LINE
finishport

3	6	coords= -1535 1675 0	0	0	1	mandatory= 0	0	1

test_done
funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -1539 1685 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	137	0	5	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
142	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
50	485	80	605
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 49 520 0	0	0	1	mandatory= 0	0	1
i1
const_zero
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	146	0	0	-1	0
-628	-50
-628	520
END_LINE
inputport

0	1	coords= 49 545 0	1	0	1	mandatory= 0	0	1
i2
const_09_30
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	279	0	0	-1	0
-628	95
-628	545
END_LINE
inputport

0	1	coords= 49 570 0	2	0	1	mandatory= 0	0	1
i3
const_12_30
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	227	0	0	-1	0
-628	125
-628	570
END_LINE
inputport

0	1	coords= 49 595 0	3	0	1	mandatory= 0	0	1
i4
const_23_30
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	185	0	0	-1	0
-628	175
-628	595
END_LINE
outputport

1	1	coords= 80 520 0	0	0	1	mandatory= 0	0	1

sched1_time
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	140	0	1	-1	0
END_LINE
startport

2	1	coords= 49 510 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	230	0	2	-1	0
-341	645
-341	510
END_LINE
startport

2	1	coords= 49 535 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	232	0	2	-1	0
-306	840
-306	535
END_LINE
startport

2	1	coords= 49 560 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	330	0	2	-1	0
-146	925
-146	560
END_LINE
startport

2	1	coords= 49 585 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	332	0	2	-1	0
-46	1040
-46	585
END_LINE
finishport

3	1	coords= 80 510 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	144	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
143	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
50	610	80	730
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 49 645 0	0	0	1	mandatory= 0	0	1
i1
const_zero
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	146	0	0	-1	0
-628	-50
-628	645
END_LINE
inputport

0	1	coords= 49 670 0	1	0	1	mandatory= 0	0	1
i2
const_thur
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	186	0	0	-1	0
-628	0
-628	670
END_LINE
inputport

0	1	coords= 49 695 0	2	0	1	mandatory= 0	0	1
i3
const_tue
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	228	0	0	-1	0
-628	-25
-628	695
END_LINE
inputport

0	1	coords= 49 720 0	3	0	1	mandatory= 0	0	1
i4
const_sat
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	184	0	0	-1	0
-628	25
-628	720
END_LINE
outputport

1	1	coords= 80 645 0	0	0	1	mandatory= 0	0	1

sched_day
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	140	1	1	-1	0
END_LINE
startport

2	1	coords= 49 635 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	230	0	2	-1	0
-341	645
-331	635
END_LINE
startport

2	1	coords= 49 660 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	232	0	2	-1	0
-306	840
-306	660
END_LINE
startport

2	1	coords= 49 685 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	330	0	2	-1	0
-146	925
-146	685
END_LINE
startport

2	1	coords= 49 710 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	332	0	2	-1	0
-46	1040
-46	710
END_LINE
finishport

3	1	coords= 80 635 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	144	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
144	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
145	570	165	605
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

2	-1	coords= 144 580 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	142	0	2	-1	0
117	510
117	580
END_LINE
startport

2	-1	coords= 144 595 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	143	0	2	-1	0
107	635
107	595
END_LINE
finishport

3	-1	coords= 151 588 0	0	0	1	mandatory= 0	0	1

sched1_ev
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	140	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
145	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
-385	-205	-355	-175
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0x08B6
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -386 -185 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	1	EndOfFunc
BEGIN_LINE
1	604	0	0	-1	0
-440	-165
-440	-185
END_LINE
outputport

1	3	coords= -355 -185 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	2	EndOfFunc
BEGIN_LINE
0	607	0	1	-1	0
END_LINE
startport

2	3	coords= -386 -195 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	0	EndOfFunc
BEGIN_LINE
1	608	0	2	-1	0
-397	-85
-397	-195
END_LINE
finishport

3	3	coords= -355 -195 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	3	EndOfFunc
BEGIN_LINE
0	607	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
146	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	-60	-1295	-40
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

1	1	coords= -1295 -50 0	0	0	1	mandatory= 0	0	1

const_zero
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	519	0	1	-1	0
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
184	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	15	-1295	35
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	6
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -1295 25 0	0	0	1	mandatory= 0	0	1

const_sat
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	519	3	1	-1	0
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
185	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	165	-1295	185
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	84600
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -1295 175 0	0	0	1	mandatory= 0	0	1

const_23_30
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	518	3	1	-1	0
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
186	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	-10	-1295	10
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	4
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -1295 0 0	0	0	1	mandatory= 0	0	1

const_thur
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	519	1	1	-1	0
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
227	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	115	-1295	135
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	45000
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -1295 125 0	0	0	1	mandatory= 0	0	1

const_12_30
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	518	2	1	-1	0
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
228	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	-35	-1295	-15
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

1	1	coords= -1295 -25 0	0	0	1	mandatory= 0	0	1

const_tue
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	440	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
229	0	0	0
_
_
<nl?>        
state
0
_
-910	520	-850	580
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched1_1
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	229
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -905 533 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	136	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -852 564 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	230	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
230	0	0	0
_
_
<nl?>        
state_condition
0
_
-770	630	-740	660
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	229
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	231
EndOfValues
EndOfLabels
_

startport

2	1	coords= -771 645 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	605
-826	605
-826	645
END_LINE
finishport

3	6	coords= -740 645 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	143	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -767 636 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	229	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -747 659 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	231	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
231	0	0	0
_
_
<nl?>        
state
0
_
-745	705	-685	765
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched1_2
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	231
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -734 712 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	230	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -705 764 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	232	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
232	0	0	0
_
_
<nl?>        
state_condition
0
_
-700	825	-670	855
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	231
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	329
EndOfValues
EndOfLabels
_

startport

2	1	coords= -701 840 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	800
-756	800
-756	840
END_LINE
finishport

3	6	coords= -670 840 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	143	1	3	-1	0
END_LINE
sm_inputport

5	6	coords= -686 825 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	231	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -676 854 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	329	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
279	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	85	-1295	105
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	34200
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -1295 95 0	0	0	1	mandatory= 0	0	1

const_09_30
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	518	1	1	-1	0
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
328	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-1390	140	-1295	160
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	61200
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -1295 150 0	0	0	1	mandatory= 0	0	1

const_17_00
funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
329	0	0	0
_
_
<nl?>        
state
0
_
-535	885	-475	945
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched1_3
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	329
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -534 912 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	232	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -473 912 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	330	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
330	0	0	0
_
_
<nl?>        
state_condition
0
_
-380	910	-350	940
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	329
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	331
EndOfValues
EndOfLabels
_

startport

2	1	coords= -381 925 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	885
-436	885
-436	925
END_LINE
finishport

3	6	coords= -350 925 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	143	2	3	-1	0
END_LINE
sm_inputport

5	6	coords= -377 916 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	329	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -356 938 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	331	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
331	0	0	0
_
_
<nl?>        
state
0
_
-295	955	-235	1015
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched1_4
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	331
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -294 979 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	330	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -233 978 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	332	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
332	0	0	0
_
_
<nl?>        
state_condition
0
_
-180	1025	-150	1055
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	331
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	444
EndOfValues
EndOfLabels
_

startport

2	1	coords= -181 1040 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-228	-615
-228	1040
END_LINE
finishport

3	6	coords= -150 1040 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	143	3	3	-1	0
END_LINE
sm_inputport

5	6	coords= -177 1031 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	331	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -176 1054 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	444	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
439	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
65	1080	95	1200
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 64 1115 0	0	0	1	mandatory= 0	0	1
i1
const_zero
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	146	0	0	-1	0
-621	-50
-621	1115
END_LINE
inputport

0	1	coords= 64 1140 0	1	0	1	mandatory= 0	0	1
i2
const_09_30
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	279	0	0	-1	0
-621	95
-621	1140
END_LINE
inputport

0	1	coords= 64 1165 0	2	0	1	mandatory= 0	0	1
i3
const_12_30
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	227	0	0	-1	0
-621	125
-621	1165
END_LINE
inputport

0	1	coords= 64 1190 0	3	0	1	mandatory= 0	0	1
i4
const_23_30
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	185	0	0	-1	0
-621	175
-621	1190
END_LINE
outputport

1	1	coords= 95 1115 0	0	0	1	mandatory= 0	0	1

sched2_time
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	140	0	1	-1	0
END_LINE
startport

2	1	coords= 64 1105 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	445	0	2	-1	0
-368	1120
-358	1105
END_LINE
startport

2	1	coords= 64 1130 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	447	0	2	-1	0
-241	1355
-241	1130
END_LINE
startport

2	1	coords= 64 1155 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	449	0	2	-1	0
-83	1450
-83	1155
END_LINE
startport

2	1	coords= 64 1180 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	450	0	2	-1	0
22	1595
22	1180
END_LINE
finishport

3	1	coords= 95 1105 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	144	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
440	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
65	1205	95	1325
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 64 1240 0	0	0	1	mandatory= 0	0	1
i1
const_zero
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	146	0	0	-1	0
-621	-50
-621	1240
END_LINE
inputport

0	1	coords= 64 1265 0	1	0	1	mandatory= 0	0	1
i2
const_thur
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	186	0	0	-1	0
-621	0
-621	1265
END_LINE
inputport

0	1	coords= 64 1290 0	2	0	1	mandatory= 0	0	1
i3
const_tue
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	228	0	0	-1	0
-621	-25
-621	1290
END_LINE
inputport

0	1	coords= 64 1315 0	3	0	1	mandatory= 0	0	1
i4
const_sat
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	184	0	0	-1	0
-621	25
-621	1315
END_LINE
outputport

1	1	coords= 95 1240 0	0	0	1	mandatory= 0	0	1

sched2_day
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	140	1	1	-1	0
END_LINE
startport

2	1	coords= 64 1230 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	445	0	2	-1	0
-378	1120
-378	1230
END_LINE
startport

2	1	coords= 64 1255 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	447	0	2	-1	0
-241	1355
-241	1255
END_LINE
startport

2	1	coords= 64 1280 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	449	0	2	-1	0
-83	1450
-83	1280
END_LINE
startport

2	1	coords= 64 1305 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	450	0	2	-1	0
22	1595
22	1305
END_LINE
finishport

3	1	coords= 95 1230 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	144	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
441	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
160	1165	180	1200
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

2	-1	coords= 159 1175 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	439	0	2	-1	0
132	1105
132	1175
END_LINE
startport

2	-1	coords= 159 1190 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	440	0	2	-1	0
122	1230
122	1190
END_LINE
finishport

3	-1	coords= 166 1183 0	0	0	1	mandatory= 0	0	1

sched2_ev
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	140	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
444	0	0	0
_
_
<nl?>        
state
0
_
-1025	980	-965	1040
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched2_1
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	444
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -972 989 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	332	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -966 1023 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	445	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
445	0	0	0
_
_
<nl?>        
state_condition
0
_
-840	1105	-810	1135
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	444
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	446
EndOfValues
EndOfLabels
_

startport

2	1	coords= -841 1120 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-215	-615
-215	1085
-891	1085
-891	1120
END_LINE
finishport

3	6	coords= -810 1120 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	440	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -830 1106 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	444	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -821 1136 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	446	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
446	0	0	0
_
_
<nl?>        
state
0
_
-745	1225	-685	1285
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched2_2
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	446
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -744 1247 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	445	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -683 1252 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	447	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
447	0	0	0
_
_
<nl?>        
state_condition
0
_
-585	1340	-555	1370
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	446
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	448
EndOfValues
EndOfLabels
_

startport

2	1	coords= -586 1355 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	1315
-641	1315
-641	1355
END_LINE
finishport

3	6	coords= -555 1355 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	440	1	3	-1	0
END_LINE
sm_inputport

5	6	coords= -572 1340 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	446	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -557 1365 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	448	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
448	0	0	0
_
_
<nl?>        
state
0
_
-425	1355	-365	1415
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched2_3
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	448
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -401 1414 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	447	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -363 1380 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	449	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
449	0	0	0
_
_
<nl?>        
state_condition
0
_
-270	1435	-240	1465
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	448
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	451
EndOfValues
EndOfLabels
_

startport

2	1	coords= -271 1450 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	1410
-326	1410
-326	1450
END_LINE
finishport

3	6	coords= -240 1450 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	440	2	3	-1	0
END_LINE
sm_inputport

5	6	coords= -260 1435 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	448	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -252 1466 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	451	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
450	0	0	0
_
_
<nl?>        
state_condition
0
_
-60	1580	-30	1610
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	451
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	602
EndOfValues
EndOfLabels
_

startport

2	1	coords= -61 1595 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-168	-615
-168	1595
END_LINE
finishport

3	6	coords= -30 1595 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	440	3	3	-1	0
END_LINE
sm_inputport

5	6	coords= -49 1580 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	451	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -55 1610 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	602	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
451	0	0	0
_
_
<nl?>        
state
0
_
-175	1525	-115	1585
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched2_4
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	451
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -174 1559 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	449	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -113 1546 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	450	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
518	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
-30	1715	0	1835
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -31 1750 0	0	0	1	mandatory= 0	0	1
i1
const_zero
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	146	0	0	-1	0
-668	-50
-668	1750
END_LINE
inputport

0	1	coords= -31 1775 0	1	0	1	mandatory= 0	0	1
i2
const_09_30
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	279	0	0	-1	0
-668	95
-668	1775
END_LINE
inputport

0	1	coords= -31 1800 0	2	0	1	mandatory= 0	0	1
i3
const_12_30
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	227	0	0	-1	0
-668	125
-668	1800
END_LINE
inputport

0	1	coords= -31 1825 0	3	0	1	mandatory= 0	0	1
i4
const_23_30
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	185	0	0	-1	0
-668	175
-668	1825
END_LINE
outputport

1	1	coords= 0 1750 0	0	0	1	mandatory= 0	0	1

Sched3_time
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	141	0	1	-1	0
END_LINE
startport

2	1	coords= -31 1740 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	521	0	2	-1	0
-463	1755
-453	1740
END_LINE
startport

2	1	coords= -31 1765 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	523	0	2	-1	0
-336	1990
-336	1765
END_LINE
startport

2	1	coords= -31 1790 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	525	0	2	-1	0
-178	2085
-178	1790
END_LINE
startport

2	1	coords= -31 1815 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	603	0	2	-1	0
-61	2260
-61	1815
END_LINE
finishport

3	1	coords= 0 1740 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	144	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
519	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
-30	1840	0	1960
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -31 1875 0	0	0	1	mandatory= 0	0	1
i1
const_zero
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	146	0	0	-1	0
-668	-50
-668	1875
END_LINE
inputport

0	1	coords= -31 1900 0	1	0	1	mandatory= 0	0	1
i2
const_thur
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	186	0	0	-1	0
-668	0
-668	1900
END_LINE
inputport

0	1	coords= -31 1925 0	2	0	1	mandatory= 0	0	1
i3
const_sat
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	184	0	0	-1	0
-668	25
-668	1925
END_LINE
inputport

0	1	coords= -31 1950 0	3	0	1	mandatory= 0	0	1
i4
const_sat
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	184	0	0	-1	0
-668	25
-668	1950
END_LINE
outputport

1	1	coords= 0 1875 0	0	0	1	mandatory= 0	0	1

Sched3_day
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	141	1	1	-1	0
END_LINE
startport

2	1	coords= -31 1865 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	521	0	2	-1	0
-473	1755
-473	1865
END_LINE
startport

2	1	coords= -31 1890 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	523	0	2	-1	0
-336	1990
-336	1890
END_LINE
startport

2	1	coords= -31 1915 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	525	0	2	-1	0
-178	2085
-178	1915
END_LINE
startport

2	1	coords= -31 1940 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	603	0	2	-1	0
-61	2260
-61	1940
END_LINE
finishport

3	1	coords= 0 1865 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	144	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
520	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
65	1800	85	1835
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

2	-1	coords= 64 1810 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	518	0	2	-1	0
37	1740
37	1810
END_LINE
startport

2	-1	coords= 64 1825 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	519	0	2	-1	0
27	1865
27	1825
END_LINE
finishport

3	-1	coords= 71 1818 0	0	0	1	mandatory= 0	0	1

sched3_ev
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	141	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
521	0	0	0
_
_
<nl?>        
state_condition
0
_
-935	1740	-905	1770
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	602
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	522
EndOfValues
EndOfLabels
_

startport

2	1	coords= -936 1755 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	1715
-991	1715
-991	1755
END_LINE
finishport

3	6	coords= -905 1755 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	440	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -920 1740 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	602	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -916 1771 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	522	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
522	0	0	0
_
_
<nl?>        
state
0
_
-840	1860	-780	1920
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched3_2
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	522
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -839 1882 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	521	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -778 1887 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	523	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
523	0	0	0
_
_
<nl?>        
state_condition
0
_
-680	1975	-650	2005
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	522
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	524
EndOfValues
EndOfLabels
_

startport

2	1	coords= -681 1990 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	1950
-736	1950
-736	1990
END_LINE
finishport

3	6	coords= -650 1990 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	440	1	3	-1	0
END_LINE
sm_inputport

5	6	coords= -667 1975 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	522	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -652 2000 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	524	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
524	0	0	0
_
_
<nl?>        
state
0
_
-520	1990	-460	2050
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched3_3
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	524
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -496 2049 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	523	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -458 2015 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	525	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
525	0	0	0
_
_
<nl?>        
state_condition
0
_
-365	2070	-335	2100
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	524
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	526
EndOfValues
EndOfLabels
_

startport

2	1	coords= -366 2085 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	2045
-421	2045
-421	2085
END_LINE
finishport

3	6	coords= -335 2085 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	519	2	3	-1	0
END_LINE
sm_inputport

5	6	coords= -355 2070 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	524	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -347 2101 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	526	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
526	0	0	0
_
_
<nl?>        
state
0
_
-270	2160	-210	2220
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched3_4
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	526
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -269 2194 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	525	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -208 2186 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	603	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
602	0	0	0
_
_
<nl?>        
state
0
_
-1055	1620	-995	1680
1	0
blockattr
BEGIN_BA
type	State
trans	-1	0	0	0
offset	0	0
hash	0xCEA8
Instance_Info	_
End_Instance
END_BA
parameter
Name	3	0	0	1	25	30	1	1.2	Sched3_1
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	602
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -997 1639 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	450	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -996 1661 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	521	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
603	0	0	0
_
_
<nl?>        
state_condition
0
_
-130	2245	-100	2275
1	0
blockattr
BEGIN_BA
type	State_Condition
trans	-1	0	0	0
offset	0	0
hash	0xFDEE
Instance_Info	_
End_Instance
END_BA
parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	526
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	137
EndOfValues
EndOfLabels
_

startport

2	1	coords= -131 2260 0	0	0	0	mandatory= 0	0	1

write_res_done
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-203	-615
-203	2260
END_LINE
finishport

3	6	coords= -100 2260 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	519	3	3	-1	0
END_LINE
sm_inputport

5	6	coords= -125 2249 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	526	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -129 2272 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	137	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

scheduler
_
604	0	0	0
_
_
_
DevmanScheduler
0
_
-555	-235	-485	-98
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0x142C
Instance_Info	_
End_Instance
END_BA
parameter
ChannelID	1	0	9	1	20	20	0	1.2	1
EndOfValues
EndOfLabels
The ID of the scheduler's channel

parameter
AutoMode	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Whether we want the time polled from a clock automatically

inputport

0	1	coords= -556 -185 0	0	0	1	mandatory= 0	0	1
time
sched1_time
funcName= get_schedule 2	1	EndOfFunc
BEGIN_LINE
1	142	0	0	-1	0
130	520
130	475
-606	475
-606	-185
END_LINE
inputport

0	1	coords= -556 -175 0	1	0	1	mandatory= 0	0	1
day
sched_day
funcName= get_schedule 2	2	EndOfFunc
BEGIN_LINE
1	143	0	0	-1	0
135	645
135	595
-601	595
-601	-175
END_LINE
inputport

0	3	coords= -556 -118 0	2	0	1	mandatory= 0	0	1
data

funcName= set_schedule 3	1	EndOfFunc
BEGIN_LINE
1	51	0	0	-1	0
-626	-148
-626	-118
END_LINE
inputport

0	1	coords= -556 -108 0	3	0	1	mandatory= 0	0	1
size

funcName= set_schedule 3	2	EndOfFunc
BEGIN_LINE
1	51	2	0	-1	0
-640	-138
-640	-108
END_LINE
outputport

1	1	coords= -485 -165 0	0	0	1	mandatory= 0	0	1
value

funcName= get_schedule 2	3	EndOfFunc
BEGIN_LINE
0	145	0	1	-1	0
END_LINE
outputport

1	1	coords= -485 -213 0	1	0	1	mandatory= 0	0	1
crc

funcName= init_schedule 1	1	funcName= set_schedule 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -485 -175 0	2	0	1	mandatory= 0	0	1
errno

funcName= get_schedule 2	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -485 -155 0	3	0	1	mandatory= 0	0	1
on

funcName= get_schedule 2	5	EndOfFunc
BEGIN_LINE
0	608	0	1	-1	0
END_LINE
outputport

1	0	coords= -485 -145 0	4	0	1	mandatory= 0	0	1
changed

funcName= get_schedule 2	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -485 -108 0	5	0	1	mandatory= 0	0	1
errno

funcName= set_schedule 3	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -556 -223 0	0	1	0	mandatory= 0	0	1
init

funcName= init_schedule 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -556 -195 0	0	0	0	mandatory= 0	0	1
get
sched1_ev
funcName= get_schedule 2	0	EndOfFunc
BEGIN_LINE
1	144	0	2	-1	0
221	588
221	540
-586	540
-586	-195
END_LINE
startport

2	1	coords= -556 -128 0	1	0	0	mandatory= 0	0	1
set

funcName= set_schedule 3	0	EndOfFunc
BEGIN_LINE
1	51	2	2	-1	0
-615	-170
-615	-128
END_LINE
finishport

3	1	coords= -485 -223 0	0	0	1	mandatory= 0	0	1
----

funcName= init_schedule 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -485 -195 0	0	0	1	mandatory= 0	0	1
----

funcName= get_schedule 2	7	EndOfFunc
BEGIN_LINE
0	608	0	3	-1	0
END_LINE
finishport

3	1	coords= -485 -185 0	2	0	1	mandatory= 0	0	1
error

funcName= get_schedule 2	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -485 -128 0	2	0	1	mandatory= 0	0	1
----

funcName= set_schedule 3	5	EndOfFunc
BEGIN_LINE
0	51	1	3	-1	0
END_LINE
finishport

3	0	coords= -485 -118 0	4	0	1	mandatory= 0	0	1
error

funcName= set_schedule 3	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

scheduler
_
605	0	0	0
_
_
_
DevmanScheduler
0
_
-545	95	-475	232
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0x142C
Instance_Info	_
End_Instance
END_BA
parameter
ChannelID	1	0	9	1	20	20	0	1.2	2
EndOfValues
EndOfLabels
The ID of the scheduler's channel

parameter
AutoMode	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Whether we want the time polled from a clock automatically

inputport

0	1	coords= -546 145 0	0	0	1	mandatory= 0	0	1
time
sched2_time
funcName= get_schedule 2	1	EndOfFunc
BEGIN_LINE
1	439	0	0	-1	0
145	1115
145	1070
-596	1070
-596	145
END_LINE
inputport

0	1	coords= -546 155 0	1	0	1	mandatory= 0	0	1
day
sched2_day
funcName= get_schedule 2	2	EndOfFunc
BEGIN_LINE
1	440	0	0	-1	0
150	1240
150	1190
-591	1190
-591	155
END_LINE
inputport

0	3	coords= -546 212 0	2	0	1	mandatory= 0	0	1
data

funcName= set_schedule 3	1	EndOfFunc
BEGIN_LINE
1	57	0	0	-1	0
-635	132
-635	212
END_LINE
inputport

0	1	coords= -546 222 0	3	0	1	mandatory= 0	0	1
size

funcName= set_schedule 3	2	EndOfFunc
BEGIN_LINE
1	57	2	0	-1	0
-650	142
-650	222
END_LINE
outputport

1	1	coords= -475 165 0	0	0	1	mandatory= 0	0	1
value

funcName= get_schedule 2	3	EndOfFunc
BEGIN_LINE
0	442	0	1	-1	0
END_LINE
outputport

1	1	coords= -475 117 0	1	0	1	mandatory= 0	0	1
crc

funcName= init_schedule 1	1	funcName= set_schedule 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -475 155 0	2	0	1	mandatory= 0	0	1
errno

funcName= get_schedule 2	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -475 175 0	3	0	1	mandatory= 0	0	1
on

funcName= get_schedule 2	5	EndOfFunc
BEGIN_LINE
0	691	0	1	-1	0
END_LINE
outputport

1	0	coords= -475 185 0	4	0	1	mandatory= 0	0	1
changed

funcName= get_schedule 2	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -475 222 0	5	0	1	mandatory= 0	0	1
errno

funcName= set_schedule 3	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -546 107 0	0	1	0	mandatory= 0	0	1
init

funcName= init_schedule 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -546 135 0	0	0	0	mandatory= 0	0	1
get
sched2_ev
funcName= get_schedule 2	0	EndOfFunc
BEGIN_LINE
1	441	0	2	-1	0
236	1183
236	1135
-576	1135
-576	135
END_LINE
startport

2	1	coords= -546 202 0	1	0	0	mandatory= 0	0	1
set

funcName= set_schedule 3	0	EndOfFunc
BEGIN_LINE
1	57	2	2	-1	0
-620	110
-620	202
END_LINE
finishport

3	1	coords= -475 107 0	0	0	1	mandatory= 0	0	1
----

funcName= init_schedule 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -475 135 0	0	0	1	mandatory= 0	0	1
----

funcName= get_schedule 2	7	EndOfFunc
BEGIN_LINE
0	691	0	3	-1	0
END_LINE
finishport

3	1	coords= -475 145 0	2	0	1	mandatory= 0	0	1
error

funcName= get_schedule 2	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -475 202 0	2	0	1	mandatory= 0	0	1
----

funcName= set_schedule 3	5	EndOfFunc
BEGIN_LINE
0	57	1	3	-1	0
END_LINE
finishport

3	0	coords= -475 212 0	4	0	1	mandatory= 0	0	1
error

funcName= set_schedule 3	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

scheduler
_
606	0	0	0
_
_
_
DevmanScheduler
0
_
-540	405	-470	542
1	0
blockattr
BEGIN_BA
type	Undefined
trans	-1	0	0	0
offset	0	0
hash	0x142C
Instance_Info	_
End_Instance
END_BA
parameter
ChannelID	1	0	9	1	20	20	0	1.2	3
EndOfValues
EndOfLabels
The ID of the scheduler's channel

parameter
AutoMode	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Whether we want the time polled from a clock automatically

inputport

0	1	coords= -541 455 0	0	0	1	mandatory= 0	0	1
time
Sched3_time
funcName= get_schedule 2	1	EndOfFunc
BEGIN_LINE
1	518	0	0	-1	0
50	1750
50	1685
-591	1685
-591	455
END_LINE
inputport

0	1	coords= -541 465 0	1	0	1	mandatory= 0	0	1
day
Sched3_day
funcName= get_schedule 2	2	EndOfFunc
BEGIN_LINE
1	519	0	0	-1	0
55	1875
55	1805
-586	1805
-586	465
END_LINE
inputport

0	3	coords= -541 522 0	2	0	1	mandatory= 0	0	1
data

funcName= set_schedule 3	1	EndOfFunc
BEGIN_LINE
1	135	0	0	-1	0
-625	372
-625	522
END_LINE
inputport

0	1	coords= -541 532 0	3	0	1	mandatory= 0	0	1
size

funcName= set_schedule 3	2	EndOfFunc
BEGIN_LINE
1	135	2	0	-1	0
-640	382
-640	532
END_LINE
outputport

1	1	coords= -470 475 0	0	0	1	mandatory= 0	0	1
value

funcName= get_schedule 2	3	EndOfFunc
BEGIN_LINE
0	443	0	1	-1	0
END_LINE
outputport

1	1	coords= -470 427 0	0	0	1	mandatory= 0	0	1
crc

funcName= init_schedule 1	1	funcName= set_schedule 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -470 465 0	2	0	1	mandatory= 0	0	1
errno

funcName= get_schedule 2	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -470 485 0	3	0	1	mandatory= 0	0	1
on

funcName= get_schedule 2	5	EndOfFunc
BEGIN_LINE
0	942	0	1	-1	0
END_LINE
outputport

1	0	coords= -470 495 0	4	0	1	mandatory= 0	0	1
changed

funcName= get_schedule 2	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -470 532 0	5	0	1	mandatory= 0	0	1
errno

funcName= set_schedule 3	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -541 417 0	0	1	0	mandatory= 0	0	1
init

funcName= init_schedule 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -541 445 0	0	0	0	mandatory= 0	0	1
get
sched3_ev
funcName= get_schedule 2	0	EndOfFunc
BEGIN_LINE
1	520	0	2	-1	0
176	1818
176	1735
-536	1735
-536	445
END_LINE
startport

2	1	coords= -541 512 0	1	0	0	mandatory= 0	0	1
set

funcName= set_schedule 3	0	EndOfFunc
BEGIN_LINE
1	135	2	2	-1	0
-610	350
-610	512
END_LINE
finishport

3	1	coords= -470 417 0	1	0	1	mandatory= 0	0	1
----

funcName= init_schedule 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -470 445 0	0	0	1	mandatory= 0	0	1
----

funcName= get_schedule 2	7	EndOfFunc
BEGIN_LINE
0	443	0	3	-1	0
END_LINE
finishport

3	1	coords= -470 455 0	2	0	1	mandatory= 0	0	1
error

funcName= get_schedule 2	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -470 512 0	2	0	1	mandatory= 0	0	1
----

funcName= set_schedule 3	5	EndOfFunc
BEGIN_LINE
0	135	1	3	-1	0
END_LINE
finishport

3	0	coords= -470 522 0	4	0	1	mandatory= 0	0	1
error

funcName= set_schedule 3	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
607	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
-310	-180	-280	-110
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

0	3	coords= -311 -145 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	145	0	0	-1	0
-338	-185
-338	-145
END_LINE
inputport

0	3	coords= -311 -120 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	609	0	0	-1	0
-333	-29
-333	-120
END_LINE
outputport

1	3	coords= -280 -145 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	58	3	1	-1	0
END_LINE
startport

2	3	coords= -311 -155 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	145	0	2	-1	0
-338	-195
-338	-155
END_LINE
startport

2	3	coords= -311 -130 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	608	1	2	-1	0
-354	-75
-354	-130
END_LINE
finishport

3	3	coords= -280 -155 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	58	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
608	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-430	-95	-407	-65
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

0	0	coords= -431 -75 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	604	3	0	-1	0
-460	-155
-460	-75
END_LINE
startport

2	0	coords= -431 -85 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	604	1	2	-1	0
-455	-195
-455	-85
END_LINE
finishport

3	0	coords= -407 -85 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	145	0	3	-1	0
END_LINE
finishport

3	0	coords= -407 -75 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	607	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
609	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-505	-40	-365	-19
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
String Constant	3	0	100	1	5	4	0	1.2	OFF
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -365 -29 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	607	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
689	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
-325	120	-295	150
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0x08B6
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -326 140 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	1	EndOfFunc
BEGIN_LINE
1	605	0	0	-1	0
-375	165
-375	140
END_LINE
outputport

1	3	coords= -295 140 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	2	EndOfFunc
BEGIN_LINE
0	607	0	1	-1	0
END_LINE
startport

2	3	coords= -326 130 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	0	EndOfFunc
BEGIN_LINE
1	691	0	2	-1	0
-350	240
-350	130
END_LINE
finishport

3	3	coords= -295 130 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	3	EndOfFunc
BEGIN_LINE
0	607	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
690	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
-240	145	-210	215
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

0	3	coords= -241 180 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	689	0	0	-1	0
-275	140
-275	180
END_LINE
inputport

0	3	coords= -241 205 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	692	0	0	-1	0
-263	296
-263	205
END_LINE
outputport

1	3	coords= -210 180 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	58	4	1	-1	0
END_LINE
startport

2	3	coords= -241 170 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	689	0	2	-1	0
-268	130
-268	170
END_LINE
startport

2	3	coords= -241 195 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	691	1	2	-1	0
-284	250
-284	195
END_LINE
finishport

3	3	coords= -210 170 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	58	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
691	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-400	230	-377	260
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

0	0	coords= -401 250 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	605	3	0	-1	0
-443	175
-443	250
END_LINE
startport

2	0	coords= -401 240 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	605	1	2	-1	0
-425	135
-425	240
END_LINE
finishport

3	0	coords= -377 240 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	145	0	3	-1	0
END_LINE
finishport

3	0	coords= -377 250 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	607	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
692	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-430	285	-290	306
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
String Constant	3	0	100	1	5	4	0	1.2	OFF
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -290 296 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	607	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
940	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
-290	335	-260	365
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0x08B6
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -291 355 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	1	EndOfFunc
BEGIN_LINE
1	606	0	0	-1	0
-415	475
-415	355
END_LINE
outputport

1	3	coords= -260 355 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	2	EndOfFunc
BEGIN_LINE
0	607	0	1	-1	0
END_LINE
startport

2	3	coords= -291 345 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	0	EndOfFunc
BEGIN_LINE
1	942	0	2	-1	0
-315	420
-315	345
END_LINE
finishport

3	3	coords= -260 345 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	3	EndOfFunc
BEGIN_LINE
0	607	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
941	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
-200	320	-170	390
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

0	3	coords= -201 355 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	940	0	0	-1	0
-235	355
-235	355
END_LINE
inputport

0	3	coords= -201 380 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	943	0	0	-1	0
-223	476
-223	380
END_LINE
outputport

1	3	coords= -170 355 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	58	5	1	-1	0
END_LINE
startport

2	3	coords= -201 345 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	940	0	2	-1	0
-228	345
-228	345
END_LINE
startport

2	3	coords= -201 370 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	942	1	2	-1	0
-244	430
-244	370
END_LINE
finishport

3	3	coords= -170 345 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	58	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
942	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
-360	410	-337	440
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

0	0	coords= -361 430 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	606	3	0	-1	0
-411	485
-411	430
END_LINE
startport

2	3	coords= -361 420 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	606	1	2	-1	0
-440	445
-440	420
END_LINE
finishport

3	0	coords= -337 420 0	0	0	1	mandatory= 1	0	1
T 

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	145	0	3	-1	0
END_LINE
finishport

3	0	coords= -337 430 0	1	0	1	mandatory= 0	0	1
F

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	607	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
943	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-405	465	-265	486
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
String Constant	3	0	100	1	5	4	0	1.2	OFF
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -265 476 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	607	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
