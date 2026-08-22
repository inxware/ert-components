#V:2.1.2

CanvasSizeXY
923	1105

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

funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	111	0	0	-1	0
-360	-345
-360	-595
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
-360	-620
-360	-640
END_LINE
startport

2	3	coords= -331 -615 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	111	0	2	-1	1
-370	-355
-370	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	106	0	3	-1	0
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
0	119	1	3	-1	0
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
0	-1	0	2	-1	1
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
1	2	0	2	-1	1
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
1	2	1	2	-1	1
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
-220	-630
-220	-720
-455	-720
-455	-635
END_LINE
startport

2	-1	coords= -401 -620 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-220	-605
-220	-720
-455	-720
-455	-620
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
-600	-630
-600	-570
END_LINE
startport

2	3	coords= -581 -545 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	20	0	2	-1	0
-480	-595
-480	-630
-600	-630
-600	-545
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
-610	-645
-745	-645
-745	-600
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
1	119	0	2	-1	0
-314	180
-314	125
-731	125
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

key_value
_
106	0	0	0
_
_
Key Value Pair Storage
key_value
0
_
-725	-360	-615	-270
1	0
blockattr
BEGIN_BA
type	Undefined
text	20	5	1.25	0	Key_Value
trans	-1	0	0	0
offset	0	-15
hash	0xDC09
Instance_Info	_
End_Instance
END_BA
parameter
domain	3	_	_	1	30	30	0	1.2	default
EndOfValues
EndOfLabels
The domain of this key value pair

parameter
key	3	_	_	1	30	45	0	1.2	str
EndOfValues
EndOfLabels
The key of the pair

parameter
default_value	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
The default value of the pair when value is not set or value input is empty during set

inputport

0	3	coords= -726 -323 0	0	0	1	mandatory= 0	0	1
value

funcName= upsert 1	1	EndOfFunc
BEGIN_LINE
1	110	0	0	-1	0
-771	-309
-761	-323
END_LINE
outputport

1	1	coords= -615 -323 0	0	0	1	mandatory= 0	0	1
errno

funcName= upsert 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -615 -295 0	1	0	1	mandatory= 0	0	1
value

funcName= query 2	1	EndOfFunc
BEGIN_LINE
0	111	0	1	-1	0
END_LINE
outputport

1	1	coords= -615 -285 0	2	0	1	mandatory= 0	0	1
errno

funcName= query 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= -726 -333 0	0	0	0	mandatory= 0	0	1
set
test_start
funcName= upsert 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-385
-781	-385
-781	-333
END_LINE
startport

2	1	coords= -726 -305 0	1	0	0	mandatory= 0	0	1
get

funcName= query 2	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	1
-570	-333
-570	-250
-771	-250
-771	-305
END_LINE
finishport

3	3	coords= -615 -333 0	0	0	1	mandatory= 0	0	1
--

funcName= upsert 1	3	EndOfFunc
BEGIN_LINE
0	106	1	3	-1	0
END_LINE
finishport

3	1	coords= -615 -305 0	1	0	1	mandatory= 0	0	1
--

funcName= query 2	3	EndOfFunc
BEGIN_LINE
0	112	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

key_value_bool
_
107	0	0	0
_
_
Key Value Pair Storage
key_value_bool
0
_
-725	-235	-615	-145
1	0
blockattr
BEGIN_BA
type	Undefined
text	20	5	1.25	0	Key_Value
trans	-1	0	0	0
offset	0	-15
hash	0xE471
Instance_Info	_
End_Instance
END_BA
parameter
domain	3	_	_	1	30	30	0	1.2	default
EndOfValues
EndOfLabels
The domain of this key value pair

parameter
key	3	_	_	1	30	45	0	1.2	bool
EndOfValues
EndOfLabels
The key of the pair

parameter
default_value	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The default value of the pair when value is not set or value input is empty during set or an error happens

inputport

0	0	coords= -726 -198 0	0	0	1	mandatory= 0	0	1
value

funcName= upsert 1	1	EndOfFunc
BEGIN_LINE
1	113	0	0	-1	0
-760	-140
-760	-198
END_LINE
outputport

1	1	coords= -615 -198 0	0	0	1	mandatory= 0	0	1
errno

funcName= upsert 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -615 -170 0	1	0	1	mandatory= 0	0	1
value

funcName= query 2	1	EndOfFunc
BEGIN_LINE
0	114	0	1	-1	0
END_LINE
outputport

1	1	coords= -615 -160 0	2	0	1	mandatory= 0	0	1
errno

funcName= query 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -726 -208 0	0	0	0	mandatory= 0	0	1
set

funcName= upsert 1	0	EndOfFunc
BEGIN_LINE
1	112	0	2	-1	0
-768	-185
-768	-208
END_LINE
startport

2	1	coords= -726 -180 0	1	0	0	mandatory= 0	0	1
get

funcName= query 2	0	EndOfFunc
BEGIN_LINE
1	107	0	2	-1	0
-585	-208
-585	-240
-750	-240
-750	-180
END_LINE
finishport

3	0	coords= -615 -208 0	0	0	1	mandatory= 0	0	1
--

funcName= upsert 1	3	EndOfFunc
BEGIN_LINE
0	107	1	3	-1	0
END_LINE
finishport

3	1	coords= -615 -180 0	1	0	1	mandatory= 0	0	1
--

funcName= query 2	3	EndOfFunc
BEGIN_LINE
0	115	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

key_value_int
_
108	0	0	0
_
_
Key Value Pair Storage
key_value_int
0
_
-720	-90	-610	0
1	0
blockattr
BEGIN_BA
type	Undefined
text	20	5	1.25	0	Key_Value
trans	-1	0	0	0
offset	0	-15
hash	0x292C
Instance_Info	_
End_Instance
END_BA
parameter
domain	3	_	_	1	30	30	0	1.2	default
EndOfValues
EndOfLabels
The domain of this key value pair

parameter
key	3	_	_	1	30	45	0	1.2	int
EndOfValues
EndOfLabels
The key of the pair

parameter
default_value	1	-2147483647	2147483647	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The default value of the pair when value is not set or value input is empty during set or an error happens

inputport

0	1	coords= -721 -53 0	0	0	1	mandatory= 0	0	1
value

funcName= upsert 1	1	EndOfFunc
BEGIN_LINE
1	120	0	0	-1	0
-760	-10
-760	-53
END_LINE
outputport

1	1	coords= -610 -53 0	0	0	1	mandatory= 0	0	1
errno

funcName= upsert 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -610 -25 0	1	0	1	mandatory= 0	0	1
value

funcName= query 2	1	EndOfFunc
BEGIN_LINE
0	117	0	1	-1	0
END_LINE
outputport

1	1	coords= -610 -15 0	2	0	1	mandatory= 0	0	1
errno

funcName= query 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= -721 -63 0	0	0	0	mandatory= 0	0	1
set

funcName= upsert 1	0	EndOfFunc
BEGIN_LINE
1	115	0	2	-1	0
-763	-60
-753	-63
END_LINE
startport

2	1	coords= -721 -35 0	1	0	0	mandatory= 0	0	1
get

funcName= query 2	0	EndOfFunc
BEGIN_LINE
1	108	0	2	-1	0
-565	-63
-565	-100
-770	-100
-770	-35
END_LINE
finishport

3	1	coords= -610 -63 0	0	0	1	mandatory= 0	0	1
--

funcName= upsert 1	3	EndOfFunc
BEGIN_LINE
0	108	1	3	-1	0
END_LINE
finishport

3	1	coords= -610 -35 0	1	0	1	mandatory= 0	0	1
--

funcName= query 2	3	EndOfFunc
BEGIN_LINE
0	116	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

key_value_real
_
109	0	0	0
_
_
Key Value Pair Storage
key_value_real
0
_
-725	45	-615	135
1	0
blockattr
BEGIN_BA
type	Undefined
text	20	5	1.25	0	Key_Value
trans	-1	0	0	0
offset	0	-15
hash	0x4651
Instance_Info	_
End_Instance
END_BA
parameter
domain	3	_	_	1	30	30	0	1.2	default
EndOfValues
EndOfLabels
The domain of this key value pair

parameter
key	3	_	_	1	30	45	0	1.2	float
EndOfValues
EndOfLabels
The key of the pair

parameter
default_value	2	-2147483647	2147483647	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The default value of the pair when value is not set or value input is empty during set or an error happens

inputport

0	2	coords= -726 82 0	0	0	1	mandatory= 0	0	1
value

funcName= upsert 1	1	EndOfFunc
BEGIN_LINE
1	121	0	0	-1	0
-755	140
-755	82
END_LINE
outputport

1	1	coords= -615 82 0	0	0	1	mandatory= 0	0	1
errno

funcName= upsert 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= -615 110 0	1	0	1	mandatory= 0	0	1
value

funcName= query 2	1	EndOfFunc
BEGIN_LINE
0	118	0	1	-1	0
END_LINE
outputport

1	1	coords= -615 120 0	2	0	1	mandatory= 0	0	1
errno

funcName= query 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	2	coords= -726 72 0	0	0	0	mandatory= 0	0	1
set

funcName= upsert 1	0	EndOfFunc
BEGIN_LINE
1	116	0	2	-1	0
-778	60
-768	72
END_LINE
startport

2	1	coords= -726 100 0	1	0	0	mandatory= 0	0	1
get

funcName= query 2	0	EndOfFunc
BEGIN_LINE
1	109	0	2	-1	0
-570	72
-570	30
-771	30
-771	100
END_LINE
finishport

3	2	coords= -615 72 0	0	0	1	mandatory= 0	0	1
--

funcName= upsert 1	3	EndOfFunc
BEGIN_LINE
0	109	1	3	-1	0
END_LINE
finishport

3	1	coords= -615 100 0	1	0	1	mandatory= 0	0	1
--

funcName= query 2	3	EndOfFunc
BEGIN_LINE
0	118	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
110	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-955	-320	-815	-299
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
String Constant	3	0	100	1	5	4	0	1.2	test_str
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -815 -309 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	106	0	1	-1	0
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
111	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-430	-380	-400	-260
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

0	3	coords= -431 -345 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	106	1	0	-1	0
-510	-295
-510	-345
END_LINE
inputport

0	3	coords= -431 -320 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	114	0	0	-1	0
-485	-205
-485	-320
END_LINE
inputport

0	3	coords= -431 -295 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	117	0	0	-1	0
-465	-25
-465	-295
END_LINE
inputport

0	3	coords= -431 -270 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	118	0	0	-1	0
-450	110
-450	-270
END_LINE
outputport

1	3	coords= -400 -345 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -431 -355 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	106	1	2	-1	1
-518	-305
-518	-355
END_LINE
startport

2	3	coords= -431 -330 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	114	0	2	-1	0
-495	-215
-495	-330
END_LINE
startport

2	3	coords= -431 -305 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	117	0	2	-1	0
-475	-35
-475	-305
END_LINE
startport

2	3	coords= -431 -280 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	118	0	2	-1	0
-456	100
-456	-280
END_LINE
finishport

3	3	coords= -400 -355 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
112	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-825	-205	-805	-155
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

2	-1	coords= -826 -195 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	112	1	2	-1	0
-785	-165
-785	-225
-876	-225
-876	-195
END_LINE
startport

2	-1	coords= -826 -180 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-230
-881	-230
-881	-180
END_LINE
startport

2	-1	coords= -826 -165 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	106	1	2	-1	1
-575	-305
-575	-260
-886	-260
-886	-165
END_LINE
finishport

3	-1	coords= -819 -185 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	107	0	3	-1	0
END_LINE
finishport

3	-1	coords= -819 -165 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	112	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
113	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
0
_
-970	-150	-935	-130
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

1	0	coords= -935 -140 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantBool 1	1	EndOfFunc
BEGIN_LINE
0	107	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantBool 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbsx
_
114	0	0	0
_
_
This is a boolean to string converter.
ConvertorBoolToString
0
_
-545	-225	-515	-195
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xCCEB
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -546 -205 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToString 1	1	EndOfFunc
BEGIN_LINE
1	107	1	0	-1	0
-565	-170
-565	-205
END_LINE
outputport

1	3	coords= -515 -205 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToString 1	2	EndOfFunc
BEGIN_LINE
0	111	1	1	-1	0
END_LINE
startport

2	3	coords= -546 -215 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToString 1	0	EndOfFunc
BEGIN_LINE
1	107	1	2	-1	0
-576	-180
-576	-215
END_LINE
finishport

3	3	coords= -515 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToString 1	3	EndOfFunc
BEGIN_LINE
0	111	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
115	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-820	-80	-800	-30
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

2	-1	coords= -821 -70 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	115	1	2	-1	0
-780	-40
-780	-105
-866	-105
-866	-70
END_LINE
startport

2	-1	coords= -821 -55 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-105
-876	-105
-876	-55
END_LINE
startport

2	-1	coords= -821 -40 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	107	1	2	-1	0
-560	-180
-560	-115
-880	-115
-880	-40
END_LINE
finishport

3	-1	coords= -814 -60 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	108	0	3	-1	0
END_LINE
finishport

3	-1	coords= -814 -40 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	115	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
116	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-825	40	-805	90
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

2	-1	coords= -826 50 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	116	1	2	-1	0
-785	80
-785	20
-860	20
-860	50
END_LINE
startport

2	-1	coords= -826 65 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	15
-881	15
-881	65
END_LINE
startport

2	-1	coords= -826 80 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	108	1	2	-1	0
-565	-35
-565	10
-871	10
-871	80
END_LINE
finishport

3	-1	coords= -819 60 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	109	0	3	-1	0
END_LINE
finishport

3	-1	coords= -819 80 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	116	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
117	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
-525	-45	-495	-15
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

0	1	coords= -526 -25 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	1	EndOfFunc
BEGIN_LINE
1	108	1	0	-1	0
-563	-25
-553	-25
END_LINE
outputport

1	3	coords= -495 -25 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	2	EndOfFunc
BEGIN_LINE
0	111	2	1	-1	0
END_LINE
startport

2	3	coords= -526 -35 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	0	EndOfFunc
BEGIN_LINE
1	108	1	2	-1	0
-563	-35
-553	-35
END_LINE
finishport

3	3	coords= -495 -35 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	3	EndOfFunc
BEGIN_LINE
0	111	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
118	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
0
_
-520	90	-490	120
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
decimal places	1	0	6	0	0	0	0	1.2	5
EndOfValues
EndOfLabels
Max number of decimal places

inputport

0	2	coords= -521 110 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	1	EndOfFunc
BEGIN_LINE
1	109	1	0	-1	0
-563	110
-553	110
END_LINE
outputport

1	3	coords= -490 110 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	2	EndOfFunc
BEGIN_LINE
0	111	3	1	-1	0
END_LINE
startport

2	3	coords= -521 100 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	0	EndOfFunc
BEGIN_LINE
1	109	1	2	-1	0
-563	100
-553	100
END_LINE
finishport

3	3	coords= -490 100 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	3	EndOfFunc
BEGIN_LINE
0	119	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
119	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-375	160	-355	210
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

2	-1	coords= -376 170 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	119	1	2	-1	0
-324	200
-324	125
-421	125
-421	170
END_LINE
startport

2	-1	coords= -376 185 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	135
-431	135
-431	185
END_LINE
startport

2	-1	coords= -376 200 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	118	0	2	-1	0
-438	100
-438	200
END_LINE
finishport

3	-1	coords= -369 180 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	-1	coords= -369 200 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	119	0	3	-1	0
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
0
_
-1025	-20	-930	0
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	123456789
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -930 -10 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	108	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
121	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-995	130	-900	150
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	1234.56789
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -900 140 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	109	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
