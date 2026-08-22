#V:2.1.2

CanvasSizeXY
1325	1870

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
-403	-655
-393	-655
END_LINE
inputport

0	3	coords= -331 -595 0	1	0	1	mandatory= 0	0	1
data
write_data_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	1117	0	0	-1	0
-130	-5
-130	-75
-376	-75
-376	-595
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
-408	-665
-398	-665
END_LINE
startport

2	3	coords= -331 -640 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	19	0	2	-1	0
-370	-620
-370	-640
END_LINE
startport

2	3	coords= -331 -615 0	2	0	0	mandatory= 0	0	1
write
write_data
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	1117	0	2	-1	0
-130	-15
-130	-75
-376	-75
-376	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	1115	0	3	-1	0
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
write_data_done
funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	1119	1	3	-1	0
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
-435	-790
-435	-735
-561	-735
-561	-665
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
-215	-720
-456	-720
-456	-635
END_LINE
startport

2	-1	coords= -401 -620 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-215	-605
-215	-720
-456	-720
-456	-620
END_LINE
startport

2	-1	coords= -401 -605 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	22	0	2	-1	0
-420	-571
-420	-605
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
-623	-600
-613	-585
END_LINE
inputport

0	3	coords= -581 -525 0	1	0	1	mandatory= 0	0	1
data

funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	21	0	0	-1	0
-636	-504
-636	-525
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
-618	-610
-608	-595
END_LINE
startport

2	3	coords= -581 -570 0	1	0	0	mandatory= 0	0	1
close

funcName= Run_CloseFILE_WriteOnly_String 2	0	EndOfFunc
BEGIN_LINE
1	20	2	2	-1	0
-480	-545
-480	-630
-620	-630
-620	-570
END_LINE
startport

2	3	coords= -581 -545 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	20	0	2	-1	0
-480	-595
-480	-630
-620	-630
-620	-545
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
-820	-515	-680	-494
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

1	3	coords= -680 -504 0	0	0	1	mandatory= 0	0	1


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
-685	-625	-655	-580
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

0	3	coords= -686 -600 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
-610	-789
-610	-715
-720	-715
-720	-600
END_LINE
inputport

0	3	coords= -686 -590 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -655 -600 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	20	0	1	-1	0
END_LINE
startport

2	3	coords= -686 -610 0	0	0	0	mandatory= 0	0	1

test_done
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	1120	0	2	-1	0
-64	895
-64	840
-731	840
-731	-610
END_LINE
finishport

3	3	coords= -655 -610 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_absrx
_
1080	0	0	0
_
_
This is an absolute real value.
AbsReal
0
_
-790	110	-760	145
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	18	1.25	1	abs
trans	-1	0	0	0
offset	0	0
hash	0x9775
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 130 0	0	0	1	mandatory= 0	0	1

const_neg_1
funcName= Run_AbsReal 1	1	EndOfFunc
BEGIN_LINE
1	1113	0	0	-1	0
-898	405
-898	130
END_LINE
outputport

1	2	coords= -760 130 0	0	0	1	mandatory= 0	0	1


funcName= Run_AbsReal 1	2	EndOfFunc
BEGIN_LINE
0	966	4	1	-1	0
END_LINE
startport

2	2	coords= -791 120 0	0	0	0	mandatory= 0	0	1


funcName= Run_AbsReal 1	0	EndOfFunc
BEGIN_LINE
1	1091	0	2	-1	0
-710	75
-710	100
-841	100
-841	120
END_LINE
finishport

3	2	coords= -760 120 0	0	0	1	mandatory= 0	0	1


funcName= Run_AbsReal 1	3	EndOfFunc
BEGIN_LINE
0	911	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_tanhrx
_
1081	0	0	0
_
_
This is the hyperbolic tan function.
HyperbolicTanReal
0
_
-790	650	-760	685
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	20	1.25	1	tanh
trans	-1	0	0	0
offset	0	0
hash	0x4CD5
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 670 0	0	0	1	mandatory= 0	0	1

const_1
funcName= Run_HyperbolicTanReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-903	465
-903	670
END_LINE
outputport

1	2	coords= -760 670 0	0	0	1	mandatory= 0	0	1


funcName= Run_HyperbolicTanReal 1	2	EndOfFunc
BEGIN_LINE
0	899	0	1	-1	0
END_LINE
startport

2	2	coords= -791 660 0	0	0	0	mandatory= 0	0	1


funcName= Run_HyperbolicTanReal 1	0	EndOfFunc
BEGIN_LINE
1	1084	0	2	-1	0
-645	600
-645	630
-841	630
-841	660
END_LINE
finishport

3	2	coords= -760 660 0	0	0	1	mandatory= 0	0	1


funcName= Run_HyperbolicTanReal 1	3	EndOfFunc
BEGIN_LINE
0	899	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_sinhrx
_
1082	0	0	0
_
_
This is the hyperbolic sine function.
HyperbolicSineReal
0
_
-795	530	-765	565
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	18	1.25	1	sinh
trans	-1	0	0	0
offset	0	0
hash	0x75FB
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -796 550 0	0	0	1	mandatory= 0	0	1

const_1
funcName= Run_HyperbolicSineReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-906	465
-906	550
END_LINE
outputport

1	2	coords= -765 550 0	0	0	1	mandatory= 0	0	1


funcName= Run_HyperbolicSineReal 1	2	EndOfFunc
BEGIN_LINE
0	901	0	1	-1	0
END_LINE
startport

2	2	coords= -796 540 0	0	0	0	mandatory= 0	0	1


funcName= Run_HyperbolicSineReal 1	0	EndOfFunc
BEGIN_LINE
1	1106	0	2	-1	0
-645	470
-645	505
-841	505
-841	540
END_LINE
finishport

3	2	coords= -765 540 0	0	0	1	mandatory= 0	0	1


funcName= Run_HyperbolicSineReal 1	3	EndOfFunc
BEGIN_LINE
0	901	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_tanrx
_
1083	0	0	0
_
_
This is the tan function.
TanReal
0
_
-795	455	-765	490
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	15	1.25	1	tan
trans	-1	0	0	0
offset	0	0
hash	0x67DD
Instance_Info	_
End_Instance
END_BA
parameter
Radians:1, Degree:0	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	coords= -796 475 0	0	0	1	mandatory= 0	0	1

const_45
funcName= Run_TanReal 1	1	EndOfFunc
BEGIN_LINE
1	1109	0	0	-1	0
-906	540
-906	475
END_LINE
outputport

1	2	coords= -765 475 0	0	0	1	mandatory= 0	0	1


funcName= Run_TanReal 1	2	EndOfFunc
BEGIN_LINE
0	922	0	1	-1	0
END_LINE
startport

2	2	coords= -796 465 0	0	0	0	mandatory= 0	0	1


funcName= Run_TanReal 1	0	EndOfFunc
BEGIN_LINE
1	1088	0	2	-1	0
-650	415
-650	445
-841	445
-841	465
END_LINE
finishport

3	2	coords= -765 465 0	0	0	1	mandatory= 0	0	1


funcName= Run_TanReal 1	3	EndOfFunc
BEGIN_LINE
0	922	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_acoshrx
_
1084	0	0	0
_
_
This is the inverse hyperbolic cosine function.
ArcHyperbolicCosineReal
0
_
-725	590	-695	625
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	25	1.25	1	acosh
trans	-1	0	0	0
offset	0	0
hash	0x7020
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -726 610 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicCosineReal 1	1	EndOfFunc
BEGIN_LINE
1	1090	0	0	-1	0
-741	610
-731	610
END_LINE
outputport

1	2	coords= -695 610 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicCosineReal 1	2	EndOfFunc
BEGIN_LINE
0	929	4	1	-1	0
END_LINE
startport

2	2	coords= -726 600 0	0	0	0	mandatory= 0	0	1


funcName= Run_ArcHyperbolicCosineReal 1	0	EndOfFunc
BEGIN_LINE
1	1090	0	2	-1	0
-741	600
-731	600
END_LINE
finishport

3	2	coords= -695 600 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicCosineReal 1	3	EndOfFunc
BEGIN_LINE
0	894	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_atanhrx
_
1085	0	0	0
_
_
This is the inverse hyperbolic tan function.
ArcHyperbolicTanReal
0
_
-720	655	-690	690
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	23	1.25	1	atanh
trans	-1	0	0	0
offset	0	0
hash	0x7B2B
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -721 675 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicTanReal 1	1	EndOfFunc
BEGIN_LINE
1	1081	0	0	-1	0
-746	670
-736	675
END_LINE
outputport

1	2	coords= -690 675 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicTanReal 1	2	EndOfFunc
BEGIN_LINE
0	929	5	1	-1	0
END_LINE
startport

2	2	coords= -721 665 0	0	0	0	mandatory= 0	0	1


funcName= Run_ArcHyperbolicTanReal 1	0	EndOfFunc
BEGIN_LINE
1	1081	0	2	-1	0
-741	660
-731	665
END_LINE
finishport

3	2	coords= -690 665 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicTanReal 1	3	EndOfFunc
BEGIN_LINE
0	1119	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_sinrx
_
1086	0	0	0
_
_
This is the sine function.
SineReal
0
_
-795	350	-765	385
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	15	1.25	1	sin
trans	-1	0	0	0
offset	0	0
hash	0xB3D1
Instance_Info	_
End_Instance
END_BA
parameter
Radians:1, Degree:0	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	coords= -796 370 0	0	0	1	mandatory= 0	0	1

const_90
funcName= Run_SineReal 1	1	EndOfFunc
BEGIN_LINE
1	1107	0	0	-1	0
-901	565
-901	370
END_LINE
outputport

1	2	coords= -765 370 0	0	0	1	mandatory= 0	0	1


funcName= Run_SineReal 1	2	EndOfFunc
BEGIN_LINE
0	921	0	1	-1	0
END_LINE
startport

2	2	coords= -796 360 0	0	0	0	mandatory= 0	0	1


funcName= Run_SineReal 1	0	EndOfFunc
BEGIN_LINE
1	1118	0	2	-1	0
-865	355
-855	360
END_LINE
finishport

3	2	coords= -765 360 0	0	0	1	mandatory= 0	0	1


funcName= Run_SineReal 1	3	EndOfFunc
BEGIN_LINE
0	921	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_asinhrx
_
1087	0	0	0
_
_
This is the inverse hyperbolic sine function.
ArcHyperbolicSineReal
0
_
-725	535	-695	570
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	23	1.25	1	asinh
trans	-1	0	0	0
offset	0	0
hash	0xF54D
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -726 555 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicSineReal 1	1	EndOfFunc
BEGIN_LINE
1	1082	0	0	-1	0
-751	550
-741	555
END_LINE
outputport

1	2	coords= -695 555 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicSineReal 1	2	EndOfFunc
BEGIN_LINE
0	929	3	1	-1	0
END_LINE
startport

2	2	coords= -726 545 0	0	0	0	mandatory= 0	0	1


funcName= Run_ArcHyperbolicSineReal 1	0	EndOfFunc
BEGIN_LINE
1	1082	0	2	-1	0
-751	540
-741	545
END_LINE
finishport

3	2	coords= -695 545 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcHyperbolicSineReal 1	3	EndOfFunc
BEGIN_LINE
0	904	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_acosrx
_
1088	0	0	0
_
_
This is the arc cosine function.
ArcCosineReal
0
_
-725	405	-695	440
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	20	1.25	1	acos
trans	-1	0	0	0
offset	0	0
hash	0x3E7D
Instance_Info	_
End_Instance
END_BA
parameter
Radians:1, Degree:0	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	coords= -726 425 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcCosineReal 1	1	EndOfFunc
BEGIN_LINE
1	1089	0	0	-1	0
-746	425
-736	425
END_LINE
outputport

1	2	coords= -695 425 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcCosineReal 1	2	EndOfFunc
BEGIN_LINE
0	929	1	1	-1	0
END_LINE
startport

2	2	coords= -726 415 0	0	0	0	mandatory= 0	0	1


funcName= Run_ArcCosineReal 1	0	EndOfFunc
BEGIN_LINE
1	1089	0	2	-1	0
-741	415
-731	415
END_LINE
finishport

3	2	coords= -695 415 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcCosineReal 1	3	EndOfFunc
BEGIN_LINE
0	897	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cosrx
_
1089	0	0	0
_
_
This is the cosine function.
CosineReal
0
_
-795	405	-765	440
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	18	1.25	1	cos
trans	-1	0	0	0
offset	0	0
hash	0x82C8
Instance_Info	_
End_Instance
END_BA
parameter
Radians:1, Degree:0	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	coords= -796 425 0	0	0	1	mandatory= 0	0	1

const_0
funcName= Run_CosineReal 1	1	EndOfFunc
BEGIN_LINE
1	1108	0	0	-1	0
-896	440
-896	425
END_LINE
outputport

1	2	coords= -765 425 0	0	0	1	mandatory= 0	0	1


funcName= Run_CosineReal 1	2	EndOfFunc
BEGIN_LINE
0	902	0	1	-1	0
END_LINE
startport

2	2	coords= -796 415 0	0	0	0	mandatory= 0	0	1


funcName= Run_CosineReal 1	0	EndOfFunc
BEGIN_LINE
1	1105	0	2	-1	0
-650	360
-650	390
-841	390
-841	415
END_LINE
finishport

3	2	coords= -765 415 0	0	0	1	mandatory= 0	0	1


funcName= Run_CosineReal 1	3	EndOfFunc
BEGIN_LINE
0	902	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_coshrx
_
1090	0	0	0
_
_
This is the hyperbolic cosine function.
HyperbolicCosineReal
0
_
-795	590	-765	625
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	20	1.25	1	cosh
trans	-1	0	0	0
offset	0	0
hash	0xB057
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -796 610 0	0	0	1	mandatory= 0	0	1

const_1
funcName= Run_HyperbolicCosineReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-906	465
-906	610
END_LINE
outputport

1	2	coords= -765 610 0	0	0	1	mandatory= 0	0	1


funcName= Run_HyperbolicCosineReal 1	2	EndOfFunc
BEGIN_LINE
0	898	0	1	-1	0
END_LINE
startport

2	2	coords= -796 600 0	0	0	0	mandatory= 0	0	1


funcName= Run_HyperbolicCosineReal 1	0	EndOfFunc
BEGIN_LINE
1	1087	0	2	-1	0
-645	545
-645	575
-846	575
-846	600
END_LINE
finishport

3	2	coords= -765 600 0	0	0	1	mandatory= 0	0	1


funcName= Run_HyperbolicCosineReal 1	3	EndOfFunc
BEGIN_LINE
0	898	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_exprx
_
1091	0	0	0
_
_
This is the exponential function.
ExponentialReal
0
_
-790	65	-760	100
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	18	1.25	1	exp
trans	-1	0	0	0
offset	0	0
hash	0x79A5
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 85 0	0	0	1	mandatory= 0	0	1

const_0
funcName= Run_ExponentialReal 1	1	EndOfFunc
BEGIN_LINE
1	1108	0	0	-1	0
-898	440
-898	85
END_LINE
outputport

1	2	coords= -760 85 0	0	0	1	mandatory= 0	0	1


funcName= Run_ExponentialReal 1	2	EndOfFunc
BEGIN_LINE
0	966	3	1	-1	0
END_LINE
startport

2	2	coords= -791 75 0	0	0	0	mandatory= 0	0	1


funcName= Run_ExponentialReal 1	0	EndOfFunc
BEGIN_LINE
1	1092	0	2	-1	0
-715	15
-715	50
-840	50
-840	75
END_LINE
finishport

3	2	coords= -760 75 0	0	0	1	mandatory= 0	0	1


funcName= Run_ExponentialReal 1	3	EndOfFunc
BEGIN_LINE
0	893	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_log10rx
_
1092	0	0	0
_
_
This is a real log base 10.
LogBaseTenReal
0
_
-790	5	-760	40
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	12	1.25	1	lg
trans	-1	0	0	0
offset	0	0
hash	0xACCA
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 25 0	0	0	1	mandatory= 0	0	1

const_2
funcName= Run_LogBaseTenReal 1	1	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-898	490
-898	25
END_LINE
outputport

1	2	coords= -760 25 0	0	0	1	mandatory= 0	0	1


funcName= Run_LogBaseTenReal 1	2	EndOfFunc
BEGIN_LINE
0	966	2	1	-1	0
END_LINE
startport

2	2	coords= -791 15 0	0	0	0	mandatory= 0	0	1


funcName= Run_LogBaseTenReal 1	0	EndOfFunc
BEGIN_LINE
1	1093	0	2	-1	0
-720	-30
-720	-5
-830	-5
-830	15
END_LINE
finishport

3	2	coords= -760 15 0	0	0	1	mandatory= 0	0	1


funcName= Run_LogBaseTenReal 1	3	EndOfFunc
BEGIN_LINE
0	905	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_logrx
_
1093	0	0	0
_
_
This is the natural log function.
LogReal
0
_
-790	-40	-760	-5
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	12	1.25	1	ln
trans	-1	0	0	0
offset	0	0
hash	0x4998
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 -20 0	0	0	1	mandatory= 0	0	1

const_2
funcName= Run_LogReal 1	1	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-898	490
-898	-20
END_LINE
outputport

1	2	coords= -760 -20 0	0	0	1	mandatory= 0	0	1


funcName= Run_LogReal 1	2	EndOfFunc
BEGIN_LINE
0	966	1	1	-1	0
END_LINE
startport

2	2	coords= -791 -30 0	0	0	0	mandatory= 0	0	1


funcName= Run_LogReal 1	0	EndOfFunc
BEGIN_LINE
1	1094	0	2	-1	0
-715	-95
-715	-50
-836	-50
-836	-30
END_LINE
finishport

3	2	coords= -760 -30 0	0	0	1	mandatory= 0	0	1


funcName= Run_LogReal 1	3	EndOfFunc
BEGIN_LINE
0	906	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_modrx
_
1094	0	0	0
_
_
This is a 2-input real modulus.
ModulusReal
0
_
-790	-105	-760	-60
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	22	1.25	1	mod
trans	-1	0	0	0
offset	0	0
hash	0x888B
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 -85 0	0	0	1	mandatory= 1	0	1
N
const_1
funcName= Run_ModulusReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-893	465
-893	-85
END_LINE
inputport

0	2	coords= -791 -75 0	1	0	1	mandatory= 1	0	1
D
const_2
funcName= Run_ModulusReal 1	2	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-893	490
-893	-75
END_LINE
outputport

1	2	coords= -760 -85 0	0	0	1	mandatory= 1	0	1


funcName= Run_ModulusReal 1	3	EndOfFunc
BEGIN_LINE
0	966	0	1	-1	0
END_LINE
startport

2	2	coords= -791 -95 0	0	0	0	mandatory= 1	0	1


funcName= Run_ModulusReal 1	0	EndOfFunc
BEGIN_LINE
1	1104	0	2	-1	0
-878	70
-878	-95
END_LINE
finishport

3	2	coords= -760 -95 0	0	0	1	mandatory= 0	0	1


funcName= Run_ModulusReal 1	4	EndOfFunc
BEGIN_LINE
0	907	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_powrx
_
1095	0	0	0
_
_
This is a 2-input real power.
PowerReal
0
_
-790	265	-760	310
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	22	1.25	1	x^y
trans	-1	0	0	0
offset	0	0
hash	0x88A5
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 285 0	0	0	1	mandatory= 0	0	1
x
const_2
funcName= Run_PowerReal 1	1	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-893	490
-893	285
END_LINE
inputport

0	2	coords= -791 295 0	1	0	1	mandatory= 0	0	1
y
const_2
funcName= Run_PowerReal 1	2	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-893	490
-893	295
END_LINE
outputport

1	2	coords= -760 285 0	0	0	1	mandatory= 0	0	1


funcName= Run_PowerReal 1	3	EndOfFunc
BEGIN_LINE
0	966	7	1	-1	0
END_LINE
startport

2	2	coords= -791 275 0	0	0	0	mandatory= 0	0	1


funcName= Run_PowerReal 1	0	EndOfFunc
BEGIN_LINE
1	1096	0	2	-1	0
-715	220
-715	255
-836	255
-836	275
END_LINE
finishport

3	2	coords= -760 275 0	0	0	1	mandatory= 0	0	1


funcName= Run_PowerReal 1	4	EndOfFunc
BEGIN_LINE
0	1118	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_sqrrx
_
1096	0	0	0
_
_
This is a 2-input real square.
SquareReal
0
_
-790	210	-760	245
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	17	1.25	1	x^2
trans	-1	0	0	0
offset	0	0
hash	0x0143
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 230 0	0	0	1	mandatory= 0	0	1
x
const_2
funcName= Run_SquareReal 1	1	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-893	490
-893	230
END_LINE
outputport

1	2	coords= -760 230 0	0	0	1	mandatory= 0	0	1


funcName= Run_SquareReal 1	2	EndOfFunc
BEGIN_LINE
0	966	6	1	-1	0
END_LINE
startport

2	2	coords= -791 220 0	0	0	0	mandatory= 0	0	1


funcName= Run_SquareReal 1	0	EndOfFunc
BEGIN_LINE
1	1097	0	2	-1	0
-715	170
-715	200
-836	200
-836	220
END_LINE
finishport

3	2	coords= -760 220 0	0	0	1	mandatory= 0	0	1


funcName= Run_SquareReal 1	3	EndOfFunc
BEGIN_LINE
0	909	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_sqrtrx
_
1097	0	0	0
_
_
This is a 2-input real square root.
SquareRootReal
0
_
-790	160	-760	195
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	18	1.25	1	sqrt
trans	-1	0	0	0
offset	0	0
hash	0x72E6
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -791 180 0	0	0	1	mandatory= 0	0	1

const_2
funcName= Run_SquareRootReal 1	1	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-898	490
-898	180
END_LINE
outputport

1	2	coords= -760 180 0	0	0	1	mandatory= 0	0	1


funcName= Run_SquareRootReal 1	2	EndOfFunc
BEGIN_LINE
0	966	5	1	-1	0
END_LINE
startport

2	2	coords= -791 170 0	0	0	0	mandatory= 0	0	1


funcName= Run_SquareRootReal 1	0	EndOfFunc
BEGIN_LINE
1	1080	0	2	-1	0
-715	120
-715	150
-836	150
-836	170
END_LINE
finishport

3	2	coords= -760 170 0	0	0	1	mandatory= 0	0	1


funcName= Run_SquareRootReal 1	3	EndOfFunc
BEGIN_LINE
0	910	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_add2rx
_
1098	0	0	0
_
_
This is a 2-input real adder.
AdditionTwoInputReal
0
_
-635	900	-605	940
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	15	1.25	1	+
trans	-1	0	0	0
offset	0	0
hash	0xBD06
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -636 920 0	0	0	1	mandatory= 1	0	1

const_1
funcName= Run_AdditionTwoInputReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-821	465
-821	920
END_LINE
inputport

0	2	coords= -636 930 0	1	0	1	mandatory= 1	0	1

const_2
funcName= Run_AdditionTwoInputReal 1	2	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-816	490
-816	930
END_LINE
outputport

1	2	coords= -605 920 0	0	0	1	mandatory= 0	0	1


funcName= Run_AdditionTwoInputReal 1	3	EndOfFunc
BEGIN_LINE
0	1041	2	1	-1	0
END_LINE
startport

2	2	coords= -636 910 0	0	0	0	mandatory= 0	0	1


funcName= Run_AdditionTwoInputReal 1	0	EndOfFunc
BEGIN_LINE
1	1100	0	2	-1	0
-560	850
-560	885
-681	885
-681	910
END_LINE
finishport

3	2	coords= -605 910 0	0	0	1	mandatory= 0	0	1


funcName= Run_AdditionTwoInputReal 1	4	EndOfFunc
BEGIN_LINE
0	915	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_subrx
_
1099	0	0	0
_
_
This is a 2-input real subtractor.
SubtractionReal
0
_
-630	965	-600	1010
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	3	1.25	0	a-b
trans	-1	0	0	0
offset	0	0
hash	0x7728
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -631 985 0	0	0	1	mandatory= 1	0	1
a
const_1
funcName= Run_SubtractionReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-818	465
-818	985
END_LINE
inputport

0	2	coords= -631 995 0	1	0	1	mandatory= 1	0	1
b
const_2
funcName= Run_SubtractionReal 1	2	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-813	490
-813	995
END_LINE
outputport

1	2	coords= -600 985 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubtractionReal 1	3	EndOfFunc
BEGIN_LINE
0	1041	3	1	-1	0
END_LINE
startport

2	2	coords= -631 975 0	0	0	0	mandatory= 0	0	1


funcName= Run_SubtractionReal 1	0	EndOfFunc
BEGIN_LINE
1	1098	0	2	-1	0
-560	910
-560	945
-676	945
-676	975
END_LINE
finishport

3	2	coords= -600 975 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubtractionReal 1	4	EndOfFunc
BEGIN_LINE
0	1041	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_mul2rx
_
1100	0	0	0
_
_
2-input real multiplier.
MultiplyTwoInputReal
0
_
-635	840	-605	880
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	13	1.25	1	x
trans	-1	0	0	0
offset	0	0
hash	0xFCD2
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -636 860 0	0	0	1	mandatory= 1	0	1

const_2
funcName= Run_MultiplyTwoInputReal 1	1	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-821	490
-821	860
END_LINE
inputport

0	2	coords= -636 870 0	1	0	1	mandatory= 1	0	1

const_2
funcName= Run_MultiplyTwoInputReal 1	2	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-816	490
-816	870
END_LINE
outputport

1	2	coords= -605 860 0	0	0	1	mandatory= 1	0	1


funcName= Run_MultiplyTwoInputReal 1	3	EndOfFunc
BEGIN_LINE
0	1041	1	1	-1	0
END_LINE
startport

2	2	coords= -636 850 0	0	0	0	mandatory= 1	0	1


funcName= Run_MultiplyTwoInputReal 1	0	EndOfFunc
BEGIN_LINE
1	1101	0	2	-1	0
-565	795
-565	830
-681	830
-681	850
END_LINE
finishport

3	2	coords= -605 850 0	0	0	1	mandatory= 0	0	1


funcName= Run_MultiplyTwoInputReal 1	4	EndOfFunc
BEGIN_LINE
0	912	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_divrx
_
1101	0	0	0
_
_
This is a 2-input real divider.
DivisionReal
0
_
-640	785	-610	825
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	12	6	2	0	/
trans	-1	0	0	0
offset	0	0
hash	0xB077
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -641 805 0	0	0	1	mandatory= 1	0	1
N
const_1
funcName= Run_DivisionReal 1	1	EndOfFunc
BEGIN_LINE
1	1110	0	0	-1	0
-823	465
-823	805
END_LINE
inputport

0	2	coords= -641 815 0	1	0	1	mandatory= 1	0	1
D
const_2
funcName= Run_DivisionReal 1	2	EndOfFunc
BEGIN_LINE
1	1111	0	0	-1	0
-818	490
-818	815
END_LINE
outputport

1	2	coords= -610 805 0	0	0	1	mandatory= 0	0	1


funcName= Run_DivisionReal 1	3	EndOfFunc
BEGIN_LINE
0	1041	0	1	-1	0
END_LINE
startport

2	2	coords= -641 795 0	0	0	0	mandatory= 0	0	1


funcName= Run_DivisionReal 1	0	EndOfFunc
BEGIN_LINE
1	1119	0	2	-1	0
-710	740
-710	795
END_LINE
finishport

3	2	coords= -610 795 0	0	0	1	mandatory= 0	0	1


funcName= Run_DivisionReal 1	4	EndOfFunc
BEGIN_LINE
0	916	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cmerbx1
_
1102	0	0	0
_
_
This is a 2-input real comparator.
ComparatorGreaterEqualReal1
0
_
-1030	-145	-980	-85
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	12	5	1.25	0	B>=A
trans	-1	0	0	0
offset	0	-15
hash	0x0533
Instance_Info	_
End_Instance
END_BA
parameter
Constant (x.yE+n allowed)	2	-1.7E+308	1.7E+308	1	12	38	0	1.2	0.0
EndOfValues
EndOfLabels
Comparator constant

inputport

0	2	coords= -1031 -110 0	0	0	1	mandatory= 1	0	1
A
const_0
funcName= Run_ComparatorGreaterEqualReal 1	1	EndOfFunc
BEGIN_LINE
1	1108	0	0	-1	0
-955	440
-955	370
-1081	370
-1081	-110
END_LINE
inputport

0	2	coords= -1031 -100 0	1	0	1	mandatory= 1	0	1
B

funcName= Run_ComparatorGreaterEqualReal 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	coords= -980 -100 0	0	0	1	mandatory= 0	0	1


funcName= Run_ComparatorGreaterEqualReal 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1031 -120 0	0	0	0	mandatory= 0	0	1


funcName= Run_ComparatorGreaterEqualReal 1	0	EndOfFunc
BEGIN_LINE
1	1115	0	2	-1	0
-930	-195
-930	-150
-1076	-150
-1076	-120
END_LINE
finishport

3	0	coords= -980 -120 0	0	0	1	mandatory= 0	0	1
t

funcName= Run_ComparatorGreaterEqualReal 1	4	EndOfFunc
BEGIN_LINE
0	919	0	3	-1	0
END_LINE
finishport

3	0	coords= -980 -110 0	1	0	1	mandatory= 0	0	1
f

funcName= Run_ComparatorGreaterEqualReal 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cmprbx1
_
1103	0	0	0
_
_
This is a 2-input real comparator.
ComparatorGreaterReal1
0
_
-1025	-50	-975	10
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	12	5	1.25	0	B>A
trans	-1	0	0	0
offset	0	-15
hash	0x641E
Instance_Info	_
End_Instance
END_BA
parameter
Constant (x.yE+n allowed)	2	-1.7E+308	1.7E+308	1	12	38	0	1.2	0.0
EndOfValues
EndOfLabels
Comparator constant

inputport

0	2	coords= -1026 -15 0	0	0	1	mandatory= 1	0	1
A
const_neg_1
funcName= Run_ComparatorGreaterReal 1	1	EndOfFunc
BEGIN_LINE
1	1113	0	0	-1	0
-955	405
-955	335
-1076	335
-1076	-15
END_LINE
inputport

0	2	coords= -1026 -5 0	1	0	1	mandatory= 1	0	1
B

funcName= Run_ComparatorGreaterReal 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	coords= -975 -5 0	0	0	1	mandatory= 0	0	1


funcName= Run_ComparatorGreaterReal 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1026 -25 0	0	0	0	mandatory= 0	0	1


funcName= Run_ComparatorGreaterReal 1	0	EndOfFunc
BEGIN_LINE
1	1102	0	2	-1	0
-935	-120
-935	-70
-1071	-70
-1071	-25
END_LINE
finishport

3	0	coords= -975 -25 0	0	0	1	mandatory= 0	0	1
t

funcName= Run_ComparatorGreaterReal 1	4	EndOfFunc
BEGIN_LINE
0	920	0	3	-1	0
END_LINE
finishport

3	0	coords= -975 -15 0	1	0	1	mandatory= 0	0	1
f

funcName= Run_ComparatorGreaterReal 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_equrbx1
_
1104	0	0	0
_
_
This is a 2-input real comparator.
ComparatorEqualReal1
0
_
-1025	45	-975	105
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	12	5	1.25	0	A=B
trans	-1	0	0	0
offset	0	-15
hash	0xC30B
Instance_Info	_
End_Instance
END_BA
parameter
Constant (x.yE+n allowed)	2	-1.7E+308	1.7E+308	1	12	38	0	1.2	0.0
EndOfValues
EndOfLabels
Comparator constant

inputport

0	2	coords= -1026 80 0	0	0	1	mandatory= 1	0	1

const_0
funcName= Run_ComparatorEqualReal 1	1	EndOfFunc
BEGIN_LINE
1	1108	0	0	-1	0
-955	440
-955	370
-1076	370
-1076	80
END_LINE
inputport

0	2	coords= -1026 90 0	1	0	1	mandatory= 1	0	1


funcName= Run_ComparatorEqualReal 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	coords= -975 90 0	0	0	1	mandatory= 0	0	1


funcName= Run_ComparatorEqualReal 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1026 70 0	0	0	0	mandatory= 0	0	1


funcName= Run_ComparatorEqualReal 1	0	EndOfFunc
BEGIN_LINE
1	1103	0	2	-1	0
-930	-25
-930	25
-1071	25
-1071	70
END_LINE
finishport

3	0	coords= -975 70 0	0	0	1	mandatory= 0	0	1
t

funcName= Run_ComparatorEqualReal 1	4	EndOfFunc
BEGIN_LINE
0	908	0	3	-1	0
END_LINE
finishport

3	0	coords= -975 80 0	1	0	1	mandatory= 0	0	1
f

funcName= Run_ComparatorEqualReal 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_asinrx
_
1105	0	0	0
_
_
This is the arc sine function.
ArcSineReal
0
_
-725	350	-695	385
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	18	1.25	1	asin
trans	-1	0	0	0
offset	0	0
hash	0x9D9F
Instance_Info	_
End_Instance
END_BA
parameter
Radians:1, Degree:0	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	coords= -726 370 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcSineReal 1	1	EndOfFunc
BEGIN_LINE
1	1086	0	0	-1	0
-746	370
-736	370
END_LINE
outputport

1	2	coords= -695 370 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcSineReal 1	2	EndOfFunc
BEGIN_LINE
0	929	0	1	-1	0
END_LINE
startport

2	2	coords= -726 360 0	0	0	0	mandatory= 0	0	1


funcName= Run_ArcSineReal 1	0	EndOfFunc
BEGIN_LINE
1	1086	0	2	-1	0
-741	360
-731	360
END_LINE
finishport

3	2	coords= -695 360 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcSineReal 1	3	EndOfFunc
BEGIN_LINE
0	903	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_atanrx
_
1106	0	0	0
_
_
This is the arc tan function.
ArcTanReal
0
_
-720	460	-690	495
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	18	20	1.25	1	atan
trans	-1	0	0	0
offset	0	0
hash	0xE9B4
Instance_Info	_
End_Instance
END_BA
parameter
Radians:1, Degree:0	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Switch between radians and degrees mode.

inputport

0	2	coords= -721 480 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcTanReal 1	1	EndOfFunc
BEGIN_LINE
1	1083	0	0	-1	0
-748	475
-738	480
END_LINE
outputport

1	2	coords= -690 480 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcTanReal 1	2	EndOfFunc
BEGIN_LINE
0	929	2	1	-1	0
END_LINE
startport

2	2	coords= -721 470 0	0	0	0	mandatory= 0	0	1


funcName= Run_ArcTanReal 1	0	EndOfFunc
BEGIN_LINE
1	1083	0	2	-1	0
-748	465
-738	470
END_LINE
finishport

3	2	coords= -690 470 0	0	0	1	mandatory= 0	0	1


funcName= Run_ArcTanReal 1	3	EndOfFunc
BEGIN_LINE
0	895	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
1107	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1170	555	-1075	575
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	90
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -1075 565 0	0	0	1	mandatory= 0	0	1

const_90
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	900	0	1	-1	0
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
1108	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1170	430	-1075	450
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

1	2	coords= -1075 440 0	0	0	1	mandatory= 0	0	1

const_0
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	920	0	1	-1	0
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
1109	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1170	530	-1075	550
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	45
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -1075 540 0	0	0	1	mandatory= 0	0	1

const_45
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	897	0	1	-1	0
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
1110	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1170	455	-1075	475
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

1	2	coords= -1075 465 0	0	0	1	mandatory= 0	0	1

const_1
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	920	1	1	-1	0
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
1111	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1170	480	-1075	500
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	2
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -1075 490 0	0	0	1	mandatory= 0	0	1

const_2
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	915	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
1112	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
-490	365	-440	510
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
Format	3	0	0	0	0	0	0	1.2	func2=(%.5f %.5f %.5f %.5f %.5f %.5f)
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= -491 387 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	1105	0	0	-1	1
-593	370
-583	387
END_LINE
inputport

0	2	coords= -491 397 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	1088	0	0	-1	1
-588	425
-588	397
END_LINE
inputport

0	2	coords= -491 407 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	1106	0	0	-1	1
-580	480
-580	407
END_LINE
inputport

0	2	coords= -491 417 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	1087	0	0	-1	1
-570	555
-570	417
END_LINE
inputport

0	2	coords= -491 427 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	1084	0	0	-1	1
-593	610
-593	427
END_LINE
inputport

0	2	coords= -491 437 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	1085	0	0	-1	1
-550	675
-550	437
END_LINE
inputport

0	2	coords= -491 447 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= -491 457 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 477 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -440 387 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	1117	1	1	-1	0
END_LINE
startport

2	3	coords= -491 377 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	1085	0	2	-1	0
-605	665
-605	377
END_LINE
finishport

3	3	coords= -440 377 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	1117	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
1113	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-1170	395	-1075	415
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	-1
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -1075 405 0	0	0	1	mandatory= 0	0	1

const_neg_1
funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	1003	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
1114	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
-555	-10	-505	135
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
Format	3	0	0	0	0	0	0	1.2	func1=(%.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f)
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= -556 12 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	1094	0	0	-1	1
-663	-85
-663	12
END_LINE
inputport

0	2	coords= -556 22 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	1093	0	0	-1	1
-663	-20
-663	22
END_LINE
inputport

0	2	coords= -556 32 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	1092	0	0	-1	1
-663	25
-653	32
END_LINE
inputport

0	2	coords= -556 42 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	1091	0	0	-1	1
-653	85
-653	42
END_LINE
inputport

0	2	coords= -556 52 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	1080	0	0	-1	1
-653	130
-653	52
END_LINE
inputport

0	2	coords= -556 62 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	1097	0	0	-1	1
-653	180
-653	62
END_LINE
inputport

0	2	coords= -556 72 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	1096	0	0	-1	0
-653	230
-653	72
END_LINE
inputport

0	2	coords= -556 82 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	1095	0	0	-1	0
-653	285
-653	82
END_LINE
inputport

0	3	coords= -556 102 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -505 12 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	1117	0	1	-1	0
END_LINE
startport

2	3	coords= -556 2 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	1095	0	2	-1	0
-625	275
-625	2
END_LINE
finishport

3	3	coords= -505 2 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	1117	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_cmerbx1
_
1115	0	0	0
_
_
This is a 2-input real comparator.
ComparatorGreaterEqualReal1
0
_
-1025	-220	-975	-160
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	12	5	1.25	0	B>=A
trans	-1	0	0	0
offset	0	-15
hash	0x0533
Instance_Info	_
End_Instance
END_BA
parameter
Constant (x.yE+n allowed)	2	-1.7E+308	1.7E+308	1	12	38	0	1.2	0.0
EndOfValues
EndOfLabels
Comparator constant

inputport

0	2	coords= -1026 -185 0	0	0	1	mandatory= 1	0	1
A
const_neg_1
funcName= Run_ComparatorGreaterEqualReal 1	1	EndOfFunc
BEGIN_LINE
1	1113	0	0	-1	0
-955	405
-955	335
-1076	335
-1076	-185
END_LINE
inputport

0	2	coords= -1026 -175 0	1	0	1	mandatory= 1	0	1
B

funcName= Run_ComparatorGreaterEqualReal 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	0	coords= -975 -175 0	0	0	1	mandatory= 0	0	1


funcName= Run_ComparatorGreaterEqualReal 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1026 -195 0	0	0	0	mandatory= 0	0	1

test_start
funcName= Run_ComparatorGreaterEqualReal 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-245
-1081	-245
-1081	-195
END_LINE
finishport

3	0	coords= -975 -195 0	0	0	1	mandatory= 0	0	1
t

funcName= Run_ComparatorGreaterEqualReal 1	4	EndOfFunc
BEGIN_LINE
0	918	0	3	-1	0
END_LINE
finishport

3	0	coords= -975 -185 0	1	0	1	mandatory= 0	0	1
f

funcName= Run_ComparatorGreaterEqualReal 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
1116	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
-415	835	-365	980
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
Format	3	0	0	0	0	0	0	1.2	func3=(%.5f %.5f %.5f %.5f)
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= -416 857 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	1101	0	0	-1	1
-518	805
-518	857
END_LINE
inputport

0	2	coords= -416 867 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	1100	0	0	-1	1
-516	860
-506	867
END_LINE
inputport

0	2	coords= -416 877 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	1098	0	0	-1	1
-485	920
-485	877
END_LINE
inputport

0	2	coords= -416 887 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	1099	0	0	-1	1
-470	985
-470	887
END_LINE
inputport

0	2	coords= -416 897 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	898	0	0	-1	1
END_LINE
inputport

0	2	coords= -416 907 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	899	0	0	-1	1
END_LINE
inputport

0	2	coords= -416 917 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	2	coords= -416 927 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -416 947 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -365 857 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	1117	2	1	-1	0
END_LINE
startport

2	3	coords= -416 847 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	1099	0	2	-1	0
-503	975
-503	847
END_LINE
finishport

3	3	coords= -365 847 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	1120	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3s
_
1117	0	0	0
_
_
This is a 3-input string multiplexer.
MultiplexThreeInputString
0
_
-215	-40	-185	55
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

0	3	coords= -216 -5 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	1114	0	0	-1	0
-356	12
-346	-5
END_LINE
inputport

0	3	coords= -216 20 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	1112	0	0	-1	0
-315	387
-315	20
END_LINE
inputport

0	3	coords= -216 45 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	1116	0	0	-1	0
-275	857
-275	45
END_LINE
outputport

1	3	coords= -185 -5 0	0	0	1	mandatory= 0	0	1

write_data_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -216 -15 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	1114	0	2	-1	0
-356	2
-346	-15
END_LINE
startport

2	3	coords= -216 10 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	1112	0	2	-1	0
-323	377
-323	10
END_LINE
startport

2	3	coords= -216 35 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	1116	0	2	-1	0
-286	847
-286	35
END_LINE
finishport

3	3	coords= -185 -15 0	0	0	1	mandatory= 0	0	1

write_data
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
1118	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-930	335	-910	385
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

2	-1	coords= -931 345 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	1118	1	2	-1	0
-874	375
-874	315
-981	315
-981	345
END_LINE
startport

2	-1	coords= -931 360 0	1	0	0	mandatory= 1	0	1

write_data_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	310
-986	310
-986	360
END_LINE
startport

2	-1	coords= -931 375 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	1095	0	2	-1	0
-700	275
-700	305
-991	305
-991	375
END_LINE
finishport

3	-1	coords= -924 355 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	1086	0	3	-1	0
END_LINE
finishport

3	-1	coords= -924 375 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	1118	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
1119	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-775	720	-755	770
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

2	-1	coords= -776 730 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	1119	1	2	-1	0
-724	760
-724	700
-821	700
-821	730
END_LINE
startport

2	-1	coords= -776 745 0	1	0	0	mandatory= 1	0	1

write_data_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	695
-831	695
-831	745
END_LINE
startport

2	-1	coords= -776 760 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	1085	0	2	-1	0
-645	665
-645	710
-840	710
-840	760
END_LINE
finishport

3	-1	coords= -769 740 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	1101	0	3	-1	0
END_LINE
finishport

3	-1	coords= -769 760 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	1119	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
1120	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-125	875	-105	925
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

2	-1	coords= -126 885 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	1120	1	2	-1	0
-74	915
-74	840
-171	840
-171	885
END_LINE
startport

2	-1	coords= -126 900 0	1	0	0	mandatory= 1	0	1

write_data_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-201	-615
-201	900
END_LINE
startport

2	-1	coords= -126 915 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	1116	0	2	-1	0
-251	847
-251	915
END_LINE
finishport

3	-1	coords= -119 895 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	-1	coords= -119 915 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	1120	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
1121	0	0	0
_
_
<nl?>        
xcomment
0
_
-550	-80	-508	-49
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
Text	3	0	0	0	0	0	0	1.2	func1
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
1122	0	0	0
_
_
<nl?>        
xcomment
0
_
-510	310	-466	341
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
Text	3	0	0	0	0	0	0	1.2	func2
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
1123	0	0	0
_
_
<nl?>        
xcomment
0
_
-445	780	-400	811
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
Text	3	0	0	0	0	0	0	1.2	func3
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
END_OF_BLOCKS
