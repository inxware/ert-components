#V:2.1.2

CanvasSizeXY
1000	2210

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
write_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	220734	0	0	-1	0
175	-125
175	-195
-376	-195
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
-360	-620
-360	-640
END_LINE
startport

2	3	coords= -331 -615 0	2	0	0	mandatory= 0	0	1
write
write
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	220734	0	2	-1	0
175	-135
175	-195
-376	-195
-376	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	220697	0	3	-1	0
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
0	220840	1	3	-1	0
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
1	220895	0	2	-1	0
61	1035
61	980
-731	980
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

json_parser_int
_
220696	0	0	0
_
_
Parse the integers from json string input or file.
json_parser_int
0
_
-490	360	-440	575
1	0
blockattr
BEGIN_BA
type	Undefined
text	8	5	1.25	0	JSON
trans	-1	0	0	0
offset	0	-15
hash	0x43E9
Instance_Info	_
End_Instance
END_BA
parameter
file	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
JSON filename. Will be ignored if there is the string input.

parameter
k1	3	_	_	0	0	0	0	1.2	key1
EndOfValues
EndOfLabels
key 1

parameter
k2	3	_	_	0	0	0	0	1.2	key2
EndOfValues
EndOfLabels
key 2

parameter
k3	3	_	_	0	0	0	0	1.2	key3
EndOfValues
EndOfLabels
key 3

parameter
k4	3	_	_	0	0	0	0	1.2	key4
EndOfValues
EndOfLabels
key 4

parameter
k5	3	_	_	0	0	0	0	1.2	key5
EndOfValues
EndOfLabels
key 5

parameter
k6	3	_	_	0	0	0	0	1.2	key6
EndOfValues
EndOfLabels
key 6

parameter
k7	3	_	_	0	0	0	0	1.2	key7
EndOfValues
EndOfLabels
key 7

parameter
k8	3	_	_	0	0	0	0	1.2	key8
EndOfValues
EndOfLabels
key 8

parameter
k9	3	_	_	0	0	0	0	1.2	key9
EndOfValues
EndOfLabels
key 9

parameter
k10	3	_	_	0	0	0	0	1.2	key10
EndOfValues
EndOfLabels
key 10

parameter
k11	3	_	_	0	0	0	0	1.2	key11
EndOfValues
EndOfLabels
key 11

parameter
k12	3	_	_	0	0	0	0	1.2	key12
EndOfValues
EndOfLabels
key 12

parameter
k13	3	_	_	0	0	0	0	1.2	key13
EndOfValues
EndOfLabels
key 13

parameter
k14	3	_	_	0	0	0	0	1.2	key14
EndOfValues
EndOfLabels
key 14

parameter
k15	3	_	_	0	0	0	0	1.2	key15
EndOfValues
EndOfLabels
key 15

parameter
k16	3	_	_	0	0	0	0	1.2	key16
EndOfValues
EndOfLabels
key 16

inputport

0	3	coords= -491 397 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	1	EndOfFunc
BEGIN_LINE
1	220730	0	0	-1	0
-603	406
-593	397
END_LINE
inputport

0	3	coords= -491 417 0	1	0	1	mandatory= 0	0	1
k1

funcName= parse 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 427 0	2	0	1	mandatory= 0	0	1
k2

funcName= parse 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 437 0	3	0	1	mandatory= 0	0	1
k3

funcName= parse 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 447 0	4	0	1	mandatory= 0	0	1
k4

funcName= parse 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 457 0	5	0	1	mandatory= 0	0	1
k5

funcName= parse 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 467 0	6	0	1	mandatory= 0	0	1
k6

funcName= parse 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 477 0	7	0	1	mandatory= 0	0	1
k7

funcName= parse 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 487 0	8	0	1	mandatory= 0	0	1
k8

funcName= parse 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 497 0	9	0	1	mandatory= 0	0	1
k9

funcName= parse 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 507 0	10	0	1	mandatory= 0	0	1
k10

funcName= parse 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 517 0	11	0	1	mandatory= 0	0	1
k11

funcName= parse 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 527 0	12	0	1	mandatory= 0	0	1
k12

funcName= parse 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 537 0	13	0	1	mandatory= 0	0	1
k13

funcName= parse 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 547 0	14	0	1	mandatory= 0	0	1
k14

funcName= parse 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 557 0	15	0	1	mandatory= 0	0	1
k15

funcName= parse 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -491 567 0	16	0	1	mandatory= 0	0	1
k16

funcName= parse 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -440 407 0	0	0	1	mandatory= 0	0	1
errno

funcName= parse 1	18	EndOfFunc
BEGIN_LINE
0	220532	0	1	-1	0
END_LINE
outputport

1	1	coords= -440 417 0	1	0	1	mandatory= 0	0	1
o1

funcName= parse 1	19	EndOfFunc
BEGIN_LINE
0	220532	0	1	-1	0
END_LINE
outputport

1	1	coords= -440 427 0	2	0	1	mandatory= 0	0	1
o2

funcName= parse 1	20	EndOfFunc
BEGIN_LINE
0	220532	1	1	-1	0
END_LINE
outputport

1	1	coords= -440 437 0	3	0	1	mandatory= 0	0	1
o3

funcName= parse 1	21	EndOfFunc
BEGIN_LINE
0	220532	2	1	-1	0
END_LINE
outputport

1	1	coords= -440 447 0	4	0	1	mandatory= 0	0	1
o4

funcName= parse 1	22	EndOfFunc
BEGIN_LINE
0	220532	3	1	-1	0
END_LINE
outputport

1	1	coords= -440 457 0	5	0	1	mandatory= 0	0	1
o5

funcName= parse 1	23	EndOfFunc
BEGIN_LINE
0	220532	4	1	-1	0
END_LINE
outputport

1	1	coords= -440 467 0	6	0	1	mandatory= 0	0	1
o6

funcName= parse 1	24	EndOfFunc
BEGIN_LINE
0	220532	5	1	-1	0
END_LINE
outputport

1	1	coords= -440 477 0	7	0	1	mandatory= 0	0	1
o7

funcName= parse 1	25	EndOfFunc
BEGIN_LINE
0	220532	6	1	-1	0
END_LINE
outputport

1	1	coords= -440 487 0	8	0	1	mandatory= 0	0	1
o8

funcName= parse 1	26	EndOfFunc
BEGIN_LINE
0	220532	7	1	-1	0
END_LINE
outputport

1	1	coords= -440 497 0	9	0	1	mandatory= 0	0	1
o9

funcName= parse 1	27	EndOfFunc
BEGIN_LINE
0	220546	0	1	-1	0
END_LINE
outputport

1	1	coords= -440 507 0	10	0	1	mandatory= 0	0	1
o10

funcName= parse 1	28	EndOfFunc
BEGIN_LINE
0	220546	1	1	-1	0
END_LINE
outputport

1	1	coords= -440 517 0	11	0	1	mandatory= 0	0	1
o11

funcName= parse 1	29	EndOfFunc
BEGIN_LINE
0	220546	2	1	-1	0
END_LINE
outputport

1	1	coords= -440 527 0	12	0	1	mandatory= 0	0	1
o12

funcName= parse 1	30	EndOfFunc
BEGIN_LINE
0	220546	3	1	-1	0
END_LINE
outputport

1	1	coords= -440 537 0	13	0	1	mandatory= 0	0	1
o13

funcName= parse 1	31	EndOfFunc
BEGIN_LINE
0	220546	4	1	-1	0
END_LINE
outputport

1	1	coords= -440 547 0	14	0	1	mandatory= 0	0	1
o14

funcName= parse 1	32	EndOfFunc
BEGIN_LINE
0	220546	5	1	-1	0
END_LINE
outputport

1	1	coords= -440 557 0	15	0	1	mandatory= 0	0	1
o15

funcName= parse 1	33	EndOfFunc
BEGIN_LINE
0	220546	6	1	-1	0
END_LINE
outputport

1	1	coords= -440 567 0	16	0	1	mandatory= 0	0	1
o16

funcName= parse 1	34	EndOfFunc
BEGIN_LINE
0	220546	7	1	-1	0
END_LINE
startport

2	3	coords= -491 387 0	0	0	0	mandatory= 0	0	1


funcName= parse 1	0	EndOfFunc
BEGIN_LINE
1	220786	0	2	-1	0
-553	310
-553	387
END_LINE
finishport

3	3	coords= -440 387 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	35	EndOfFunc
BEGIN_LINE
0	220532	0	3	-1	0
END_LINE
finishport

3	3	coords= -440 397 0	1	0	1	mandatory= 0	0	1
err

funcName= parse 1	36	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_parser_str
_
220697	0	0	0
_
_
Parse the string value from json string input or file.
json_parser_str
0
_
-495	-340	-445	-125
1	0
blockattr
BEGIN_BA
type	Undefined
text	8	5	1.25	0	JSON
trans	-1	0	0	0
offset	0	-15
hash	0xE643
Instance_Info	_
End_Instance
END_BA
parameter
file	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
JSON filename. Will be ignored if there is the string input.

parameter
k1	3	_	_	0	0	0	0	1.2	key1
EndOfValues
EndOfLabels
key 1

parameter
k2	3	_	_	0	0	0	0	1.2	key2
EndOfValues
EndOfLabels
key 2

parameter
k3	3	_	_	0	0	0	0	1.2	key3
EndOfValues
EndOfLabels
key 3

parameter
k4	3	_	_	0	0	0	0	1.2	key4
EndOfValues
EndOfLabels
key 4

parameter
k5	3	_	_	0	0	0	0	1.2	key5
EndOfValues
EndOfLabels
key 5

parameter
k6	3	_	_	0	0	0	0	1.2	key6
EndOfValues
EndOfLabels
key 6

parameter
k7	3	_	_	0	0	0	0	1.2	key7
EndOfValues
EndOfLabels
key 7

parameter
k8	3	_	_	0	0	0	0	1.2	key8
EndOfValues
EndOfLabels
key 8

parameter
k9	3	_	_	0	0	0	0	1.2	key9
EndOfValues
EndOfLabels
key 9

parameter
k10	3	_	_	0	0	0	0	1.2	key10
EndOfValues
EndOfLabels
key 10

parameter
k11	3	_	_	0	0	0	0	1.2	key11
EndOfValues
EndOfLabels
key 11

parameter
k12	3	_	_	0	0	0	0	1.2	key12
EndOfValues
EndOfLabels
key 12

parameter
k13	3	_	_	0	0	0	0	1.2	key13
EndOfValues
EndOfLabels
key 13

parameter
k14	3	_	_	0	0	0	0	1.2	key14
EndOfValues
EndOfLabels
key 14

parameter
k15	3	_	_	0	0	0	0	1.2	key15
EndOfValues
EndOfLabels
key 15

parameter
k16	3	_	_	0	0	0	0	1.2	key16
EndOfValues
EndOfLabels
key 16

inputport

0	3	coords= -496 -303 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	1	EndOfFunc
BEGIN_LINE
1	220702	0	0	-1	0
-623	-334
-623	-303
END_LINE
inputport

0	3	coords= -496 -283 0	1	0	1	mandatory= 0	0	1
k1

funcName= parse 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -273 0	2	0	1	mandatory= 0	0	1
k2

funcName= parse 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -263 0	3	0	1	mandatory= 0	0	1
k3

funcName= parse 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -253 0	4	0	1	mandatory= 0	0	1
k4

funcName= parse 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -243 0	5	0	1	mandatory= 0	0	1
k5

funcName= parse 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -233 0	6	0	1	mandatory= 0	0	1
k6

funcName= parse 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -223 0	7	0	1	mandatory= 0	0	1
k7

funcName= parse 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -213 0	8	0	1	mandatory= 0	0	1
k8

funcName= parse 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -203 0	9	0	1	mandatory= 0	0	1
k9

funcName= parse 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -193 0	10	0	1	mandatory= 0	0	1
k10

funcName= parse 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -183 0	11	0	1	mandatory= 0	0	1
k11

funcName= parse 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -173 0	12	0	1	mandatory= 0	0	1
k12

funcName= parse 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -163 0	13	0	1	mandatory= 0	0	1
k13

funcName= parse 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -153 0	14	0	1	mandatory= 0	0	1
k14

funcName= parse 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -143 0	15	0	1	mandatory= 0	0	1
k15

funcName= parse 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 -133 0	16	0	1	mandatory= 0	0	1
k16

funcName= parse 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -445 -293 0	0	0	1	mandatory= 0	0	1
errno

funcName= parse 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -445 -283 0	1	0	1	mandatory= 0	0	1
o1

funcName= parse 1	19	EndOfFunc
BEGIN_LINE
0	5	0	1	-1	0
END_LINE
outputport

1	3	coords= -445 -273 0	2	0	1	mandatory= 0	0	1
o2

funcName= parse 1	20	EndOfFunc
BEGIN_LINE
0	5	1	1	-1	0
END_LINE
outputport

1	3	coords= -445 -263 0	3	0	1	mandatory= 0	0	1
o3

funcName= parse 1	21	EndOfFunc
BEGIN_LINE
0	5	2	1	-1	0
END_LINE
outputport

1	3	coords= -445 -253 0	4	0	1	mandatory= 0	0	1
o4

funcName= parse 1	22	EndOfFunc
BEGIN_LINE
0	5	3	1	-1	0
END_LINE
outputport

1	3	coords= -445 -243 0	5	0	1	mandatory= 0	0	1
o5

funcName= parse 1	23	EndOfFunc
BEGIN_LINE
0	5	4	1	-1	0
END_LINE
outputport

1	3	coords= -445 -233 0	6	0	1	mandatory= 0	0	1
o6

funcName= parse 1	24	EndOfFunc
BEGIN_LINE
0	5	5	1	-1	0
END_LINE
outputport

1	3	coords= -445 -223 0	7	0	1	mandatory= 0	0	1
o7

funcName= parse 1	25	EndOfFunc
BEGIN_LINE
0	5	6	1	-1	0
END_LINE
outputport

1	3	coords= -445 -213 0	8	0	1	mandatory= 0	0	1
o8

funcName= parse 1	26	EndOfFunc
BEGIN_LINE
0	5	7	1	-1	0
END_LINE
outputport

1	3	coords= -445 -203 0	9	0	1	mandatory= 0	0	1
o9

funcName= parse 1	27	EndOfFunc
BEGIN_LINE
0	6	0	1	-1	0
END_LINE
outputport

1	3	coords= -445 -193 0	10	0	1	mandatory= 0	0	1
o10

funcName= parse 1	28	EndOfFunc
BEGIN_LINE
0	6	1	1	-1	0
END_LINE
outputport

1	3	coords= -445 -183 0	11	0	1	mandatory= 0	0	1
o11

funcName= parse 1	29	EndOfFunc
BEGIN_LINE
0	6	2	1	-1	0
END_LINE
outputport

1	3	coords= -445 -173 0	12	0	1	mandatory= 0	0	1
o12

funcName= parse 1	30	EndOfFunc
BEGIN_LINE
0	6	3	1	-1	0
END_LINE
outputport

1	3	coords= -445 -163 0	13	0	1	mandatory= 0	0	1
o13

funcName= parse 1	31	EndOfFunc
BEGIN_LINE
0	6	4	1	-1	0
END_LINE
outputport

1	3	coords= -445 -153 0	14	0	1	mandatory= 0	0	1
o14

funcName= parse 1	32	EndOfFunc
BEGIN_LINE
0	6	5	1	-1	0
END_LINE
outputport

1	3	coords= -445 -143 0	15	0	1	mandatory= 0	0	1
o15

funcName= parse 1	33	EndOfFunc
BEGIN_LINE
0	6	6	1	-1	0
END_LINE
outputport

1	3	coords= -445 -133 0	16	0	1	mandatory= 0	0	1
o16

funcName= parse 1	34	EndOfFunc
BEGIN_LINE
0	6	7	1	-1	0
END_LINE
startport

2	3	coords= -496 -313 0	0	0	0	mandatory= 0	0	1

test_start
funcName= parse 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-365
-551	-365
-551	-313
END_LINE
finishport

3	3	coords= -445 -313 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	35	EndOfFunc
BEGIN_LINE
0	5	0	3	-1	0
END_LINE
finishport

3	3	coords= -445 -303 0	1	0	1	mandatory= 0	0	1
err

funcName= parse 1	36	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_parser_real
_
220698	0	0	0
_
_
Parse the float numbers from json string input or file.
json_parser_real
0
_
-495	35	-445	250
1	0
blockattr
BEGIN_BA
type	Undefined
text	8	5	1.25	0	JSON
trans	-1	0	0	0
offset	0	-15
hash	0x15FB
Instance_Info	_
End_Instance
END_BA
parameter
file	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
JSON filename. Will be ignored if there is the string input.

parameter
k1	3	_	_	0	0	0	0	1.2	key1
EndOfValues
EndOfLabels
key 1

parameter
k2	3	_	_	0	0	0	0	1.2	key2
EndOfValues
EndOfLabels
key 2

parameter
k3	3	_	_	0	0	0	0	1.2	key3
EndOfValues
EndOfLabels
key 3

parameter
k4	3	_	_	0	0	0	0	1.2	key4
EndOfValues
EndOfLabels
key 4

parameter
k5	3	_	_	0	0	0	0	1.2	key5
EndOfValues
EndOfLabels
key 5

parameter
k6	3	_	_	0	0	0	0	1.2	key6
EndOfValues
EndOfLabels
key 6

parameter
k7	3	_	_	0	0	0	0	1.2	key7
EndOfValues
EndOfLabels
key 7

parameter
k8	3	_	_	0	0	0	0	1.2	key8
EndOfValues
EndOfLabels
key 8

parameter
k9	3	_	_	0	0	0	0	1.2	key9
EndOfValues
EndOfLabels
key 9

parameter
k10	3	_	_	0	0	0	0	1.2	key10
EndOfValues
EndOfLabels
key 10

parameter
k11	3	_	_	0	0	0	0	1.2	key11
EndOfValues
EndOfLabels
key 11

parameter
k12	3	_	_	0	0	0	0	1.2	key12
EndOfValues
EndOfLabels
key 12

parameter
k13	3	_	_	0	0	0	0	1.2	key13
EndOfValues
EndOfLabels
key 13

parameter
k14	3	_	_	0	0	0	0	1.2	key14
EndOfValues
EndOfLabels
key 14

parameter
k15	3	_	_	0	0	0	0	1.2	key15
EndOfValues
EndOfLabels
key 15

parameter
k16	3	_	_	0	0	0	0	1.2	key16
EndOfValues
EndOfLabels
key 16

inputport

0	3	coords= -496 72 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	1	EndOfFunc
BEGIN_LINE
1	220729	0	0	-1	0
-621	56
-611	72
END_LINE
inputport

0	3	coords= -496 92 0	1	0	1	mandatory= 0	0	1
k1

funcName= parse 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 102 0	2	0	1	mandatory= 0	0	1
k2

funcName= parse 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 112 0	3	0	1	mandatory= 0	0	1
k3

funcName= parse 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 122 0	4	0	1	mandatory= 0	0	1
k4

funcName= parse 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 132 0	5	0	1	mandatory= 0	0	1
k5

funcName= parse 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 142 0	6	0	1	mandatory= 0	0	1
k6

funcName= parse 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 152 0	7	0	1	mandatory= 0	0	1
k7

funcName= parse 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 162 0	8	0	1	mandatory= 0	0	1
k8

funcName= parse 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 172 0	9	0	1	mandatory= 0	0	1
k9

funcName= parse 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 182 0	10	0	1	mandatory= 0	0	1
k10

funcName= parse 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 192 0	11	0	1	mandatory= 0	0	1
k11

funcName= parse 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 202 0	12	0	1	mandatory= 0	0	1
k12

funcName= parse 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 212 0	13	0	1	mandatory= 0	0	1
k13

funcName= parse 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 222 0	14	0	1	mandatory= 0	0	1
k14

funcName= parse 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 232 0	15	0	1	mandatory= 0	0	1
k15

funcName= parse 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -496 242 0	16	0	1	mandatory= 0	0	1
k16

funcName= parse 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -445 82 0	0	0	1	mandatory= 0	0	1
errno

funcName= parse 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	2	coords= -445 92 0	1	0	1	mandatory= 0	0	1
o1

funcName= parse 1	19	EndOfFunc
BEGIN_LINE
0	220564	0	1	-1	0
END_LINE
outputport

1	2	coords= -445 102 0	2	0	1	mandatory= 0	0	1
o2

funcName= parse 1	20	EndOfFunc
BEGIN_LINE
0	220564	1	1	-1	0
END_LINE
outputport

1	2	coords= -445 112 0	3	0	1	mandatory= 0	0	1
o3

funcName= parse 1	21	EndOfFunc
BEGIN_LINE
0	220564	2	1	-1	0
END_LINE
outputport

1	2	coords= -445 122 0	4	0	1	mandatory= 0	0	1
o4

funcName= parse 1	22	EndOfFunc
BEGIN_LINE
0	220564	3	1	-1	0
END_LINE
outputport

1	2	coords= -445 132 0	5	0	1	mandatory= 0	0	1
o5

funcName= parse 1	23	EndOfFunc
BEGIN_LINE
0	220564	4	1	-1	0
END_LINE
outputport

1	2	coords= -445 142 0	6	0	1	mandatory= 0	0	1
o6

funcName= parse 1	24	EndOfFunc
BEGIN_LINE
0	220564	5	1	-1	0
END_LINE
outputport

1	2	coords= -445 152 0	7	0	1	mandatory= 0	0	1
o7

funcName= parse 1	25	EndOfFunc
BEGIN_LINE
0	220564	6	1	-1	0
END_LINE
outputport

1	2	coords= -445 162 0	8	0	1	mandatory= 0	0	1
o8

funcName= parse 1	26	EndOfFunc
BEGIN_LINE
0	220564	7	1	-1	0
END_LINE
outputport

1	2	coords= -445 172 0	9	0	1	mandatory= 0	0	1
o9

funcName= parse 1	27	EndOfFunc
BEGIN_LINE
0	220583	0	1	-1	0
END_LINE
outputport

1	2	coords= -445 182 0	10	0	1	mandatory= 0	0	1
o10

funcName= parse 1	28	EndOfFunc
BEGIN_LINE
0	220583	1	1	-1	0
END_LINE
outputport

1	2	coords= -445 192 0	11	0	1	mandatory= 0	0	1
o11

funcName= parse 1	29	EndOfFunc
BEGIN_LINE
0	220583	2	1	-1	0
END_LINE
outputport

1	2	coords= -445 202 0	12	0	1	mandatory= 0	0	1
o12

funcName= parse 1	30	EndOfFunc
BEGIN_LINE
0	220583	3	1	-1	0
END_LINE
outputport

1	2	coords= -445 212 0	13	0	1	mandatory= 0	0	1
o13

funcName= parse 1	31	EndOfFunc
BEGIN_LINE
0	220583	4	1	-1	0
END_LINE
outputport

1	2	coords= -445 222 0	14	0	1	mandatory= 0	0	1
o14

funcName= parse 1	32	EndOfFunc
BEGIN_LINE
0	220583	5	1	-1	0
END_LINE
outputport

1	2	coords= -445 232 0	15	0	1	mandatory= 0	0	1
o15

funcName= parse 1	33	EndOfFunc
BEGIN_LINE
0	220583	6	1	-1	0
END_LINE
outputport

1	2	coords= -445 242 0	16	0	1	mandatory= 0	0	1
o16

funcName= parse 1	34	EndOfFunc
BEGIN_LINE
0	220583	7	1	-1	0
END_LINE
startport

2	3	coords= -496 62 0	0	0	0	mandatory= 0	0	1


funcName= parse 1	0	EndOfFunc
BEGIN_LINE
1	220733	0	2	-1	0
-565	-5
-565	62
END_LINE
finishport

3	3	coords= -445 62 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	35	EndOfFunc
BEGIN_LINE
0	220564	0	3	-1	0
END_LINE
finishport

3	3	coords= -445 72 0	1	0	1	mandatory= 0	0	1
err

funcName= parse 1	36	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_parser_bool
_
220699	0	0	0
_
_
Parse the boolean values from json string input or file.
json_parser_bool
0
_
-510	690	-460	905
1	0
blockattr
BEGIN_BA
type	Undefined
text	8	5	1.25	0	JSON
trans	-1	0	0	0
offset	0	-15
hash	0xB7DB
Instance_Info	_
End_Instance
END_BA
parameter
file	3	_	_	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
JSON filename. Will be ignored if there is the string input.

parameter
k1	3	_	_	0	0	0	0	1.2	key1
EndOfValues
EndOfLabels
key 1

parameter
k2	3	_	_	0	0	0	0	1.2	key2
EndOfValues
EndOfLabels
key 2

parameter
k3	3	_	_	0	0	0	0	1.2	key3
EndOfValues
EndOfLabels
key 3

parameter
k4	3	_	_	0	0	0	0	1.2	key4
EndOfValues
EndOfLabels
key 4

parameter
k5	3	_	_	0	0	0	0	1.2	key5
EndOfValues
EndOfLabels
key 5

parameter
k6	3	_	_	0	0	0	0	1.2	key6
EndOfValues
EndOfLabels
key 6

parameter
k7	3	_	_	0	0	0	0	1.2	key7
EndOfValues
EndOfLabels
key 7

parameter
k8	3	_	_	0	0	0	0	1.2	key8
EndOfValues
EndOfLabels
key 8

parameter
k9	3	_	_	0	0	0	0	1.2	key9
EndOfValues
EndOfLabels
key 9

parameter
k10	3	_	_	0	0	0	0	1.2	key10
EndOfValues
EndOfLabels
key 10

parameter
k11	3	_	_	0	0	0	0	1.2	key11
EndOfValues
EndOfLabels
key 11

parameter
k12	3	_	_	0	0	0	0	1.2	key12
EndOfValues
EndOfLabels
key 12

parameter
k13	3	_	_	0	0	0	0	1.2	key13
EndOfValues
EndOfLabels
key 13

parameter
k14	3	_	_	0	0	0	0	1.2	key14
EndOfValues
EndOfLabels
key 14

parameter
k15	3	_	_	0	0	0	0	1.2	key15
EndOfValues
EndOfLabels
key 15

parameter
k16	3	_	_	0	0	0	0	1.2	key16
EndOfValues
EndOfLabels
key 16

inputport

0	3	coords= -511 727 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	1	EndOfFunc
BEGIN_LINE
1	220731	0	0	-1	0
-616	706
-616	727
END_LINE
inputport

0	3	coords= -511 747 0	1	0	1	mandatory= 0	0	1
k1

funcName= parse 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 757 0	2	0	1	mandatory= 0	0	1
k2

funcName= parse 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 767 0	3	0	1	mandatory= 0	0	1
k3

funcName= parse 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 777 0	4	0	1	mandatory= 0	0	1
k4

funcName= parse 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 787 0	5	0	1	mandatory= 0	0	1
k5

funcName= parse 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 797 0	6	0	1	mandatory= 0	0	1
k6

funcName= parse 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 807 0	7	0	1	mandatory= 0	0	1
k7

funcName= parse 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 817 0	8	0	1	mandatory= 0	0	1
k8

funcName= parse 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 827 0	9	0	1	mandatory= 0	0	1
k9

funcName= parse 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 837 0	10	0	1	mandatory= 0	0	1
k10

funcName= parse 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 847 0	11	0	1	mandatory= 0	0	1
k11

funcName= parse 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 857 0	12	0	1	mandatory= 0	0	1
k12

funcName= parse 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 867 0	13	0	1	mandatory= 0	0	1
k13

funcName= parse 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 877 0	14	0	1	mandatory= 0	0	1
k14

funcName= parse 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 887 0	15	0	1	mandatory= 0	0	1
k15

funcName= parse 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -511 897 0	16	0	1	mandatory= 0	0	1
k16

funcName= parse 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -460 737 0	0	0	1	mandatory= 0	0	1
errno

funcName= parse 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 747 0	1	0	1	mandatory= 0	0	1
o1

funcName= parse 1	19	EndOfFunc
BEGIN_LINE
0	220563	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 757 0	2	0	1	mandatory= 0	0	1
o2

funcName= parse 1	20	EndOfFunc
BEGIN_LINE
0	220584	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 767 0	3	0	1	mandatory= 0	0	1
o3

funcName= parse 1	21	EndOfFunc
BEGIN_LINE
0	220585	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 777 0	4	0	1	mandatory= 0	0	1
o4

funcName= parse 1	22	EndOfFunc
BEGIN_LINE
0	220586	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 787 0	5	0	1	mandatory= 0	0	1
o5

funcName= parse 1	23	EndOfFunc
BEGIN_LINE
0	220608	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 797 0	6	0	1	mandatory= 0	0	1
o6

funcName= parse 1	24	EndOfFunc
BEGIN_LINE
0	220609	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 807 0	7	0	1	mandatory= 0	0	1
o7

funcName= parse 1	25	EndOfFunc
BEGIN_LINE
0	220610	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 817 0	8	0	1	mandatory= 0	0	1
o8

funcName= parse 1	26	EndOfFunc
BEGIN_LINE
0	220611	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 827 0	9	0	1	mandatory= 0	0	1
o9

funcName= parse 1	27	EndOfFunc
BEGIN_LINE
0	220638	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 837 0	10	0	1	mandatory= 0	0	1
o10

funcName= parse 1	28	EndOfFunc
BEGIN_LINE
0	220639	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 847 0	11	0	1	mandatory= 0	0	1
o11

funcName= parse 1	29	EndOfFunc
BEGIN_LINE
0	220640	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 857 0	12	0	1	mandatory= 0	0	1
o12

funcName= parse 1	30	EndOfFunc
BEGIN_LINE
0	220641	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 867 0	13	0	1	mandatory= 0	0	1
o13

funcName= parse 1	31	EndOfFunc
BEGIN_LINE
0	220642	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 877 0	14	0	1	mandatory= 0	0	1
o14

funcName= parse 1	32	EndOfFunc
BEGIN_LINE
0	220643	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 887 0	15	0	1	mandatory= 0	0	1
o15

funcName= parse 1	33	EndOfFunc
BEGIN_LINE
0	220644	0	1	-1	0
END_LINE
outputport

1	0	coords= -460 897 0	16	0	1	mandatory= 0	0	1
o16

funcName= parse 1	34	EndOfFunc
BEGIN_LINE
0	220645	0	1	-1	0
END_LINE
startport

2	3	coords= -511 717 0	0	0	0	mandatory= 0	0	1


funcName= parse 1	0	EndOfFunc
BEGIN_LINE
1	220840	0	2	-1	0
-558	635
-558	717
END_LINE
finishport

3	3	coords= -460 717 0	0	0	1	mandatory= 0	0	1


funcName= parse 1	35	EndOfFunc
BEGIN_LINE
0	220563	0	3	-1	0
END_LINE
finishport

3	3	coords= -460 727 0	1	0	1	mandatory= 0	0	1
err

funcName= parse 1	36	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8
_
220700	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
-350	-325	-300	-180
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0xD55E
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s %s %s %s %s %s %s %s
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	coords= -351 -303 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220697	1	0	-1	0
-393	-283
-393	-303
END_LINE
inputport

0	3	coords= -351 -293 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220697	2	0	-1	0
-393	-273
-393	-293
END_LINE
inputport

0	3	coords= -351 -283 0	2	0	1	mandatory= 0	0	1
s3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220697	3	0	-1	0
-393	-263
-393	-283
END_LINE
inputport

0	3	coords= -351 -273 0	3	0	1	mandatory= 0	0	1
s4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220697	4	0	-1	0
-393	-253
-393	-273
END_LINE
inputport

0	3	coords= -351 -263 0	4	0	1	mandatory= 0	0	1
s5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220697	5	0	-1	0
-393	-243
-393	-263
END_LINE
inputport

0	3	coords= -351 -253 0	5	0	1	mandatory= 0	0	1
s6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220697	6	0	-1	0
-393	-233
-393	-253
END_LINE
inputport

0	3	coords= -351 -243 0	6	0	1	mandatory= 0	0	1
s7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220697	7	0	-1	0
-393	-223
-393	-243
END_LINE
inputport

0	3	coords= -351 -233 0	7	0	1	mandatory= 0	0	1
s8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220697	8	0	-1	0
-393	-213
-393	-233
END_LINE
inputport

0	3	coords= -351 -213 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -300 -303 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220502	0	1	-1	0
END_LINE
startport

2	3	coords= -351 -313 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220697	0	2	-1	0
-393	-313
-383	-313
END_LINE
finishport

3	3	coords= -300 -313 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	6	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8
_
220701	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
-345	-165	-295	-20
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Format
trans	-1	0	0	0
offset	0	-2
hash	0xD55E
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s %s %s %s %s %s %s %s
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	coords= -346 -143 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220697	9	0	-1	0
-401	-203
-401	-143
END_LINE
inputport

0	3	coords= -346 -133 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220697	10	0	-1	0
-401	-193
-401	-133
END_LINE
inputport

0	3	coords= -346 -123 0	2	0	1	mandatory= 0	0	1
s3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220697	11	0	-1	0
-401	-183
-401	-123
END_LINE
inputport

0	3	coords= -346 -113 0	3	0	1	mandatory= 0	0	1
s4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220697	12	0	-1	0
-401	-173
-401	-113
END_LINE
inputport

0	3	coords= -346 -103 0	4	0	1	mandatory= 0	0	1
s5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220697	13	0	-1	0
-401	-163
-401	-103
END_LINE
inputport

0	3	coords= -346 -93 0	5	0	1	mandatory= 0	0	1
s6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220697	14	0	-1	0
-401	-153
-401	-93
END_LINE
inputport

0	3	coords= -346 -83 0	6	0	1	mandatory= 0	0	1
s7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220697	15	0	-1	0
-401	-143
-401	-83
END_LINE
inputport

0	3	coords= -346 -73 0	7	0	1	mandatory= 0	0	1
s8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220697	16	0	-1	0
-401	-133
-401	-73
END_LINE
inputport

0	3	coords= -346 -53 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -295 -143 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220502	1	1	-1	0
END_LINE
startport

2	3	coords= -346 -153 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220700	0	2	-1	0
-255	-313
-255	-175
-391	-175
-391	-153
END_LINE
finishport

3	3	coords= -295 -153 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220502	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
220702	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-880	-345	-740	-324
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
String Constant	3	0	100	1	5	4	0	1.2	{"key1":"str1", "key2" : "str2","key3":"str3", "key4":"str4","key5":"str5","key6":"str6","key7":"str7","key8":"str8","key9":"str9","key10":"str10","key11":"str11","key12":"str12","key13":"str13","key14":"str14","key15": "str15","key16": "str16"}
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -740 -334 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	2	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
220703	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-185	-195	-135	-155
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	format
trans	-1	0	0	0
offset	0	-2
hash	0xDF0A
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s %s
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -186 -173 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220700	0	0	-1	0
-248	-303
-248	-173
END_LINE
inputport

0	3	coords= -186 -163 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220701	0	0	-1	0
-236	-143
-236	-163
END_LINE
outputport

1	3	coords= -135 -173 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	220734	0	1	-1	0
END_LINE
startport

2	3	coords= -186 -183 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220701	0	2	-1	0
-236	-153
-236	-183
END_LINE
finishport

3	3	coords= -135 -183 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	220734	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
220704	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-170	150	-120	190
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	format
trans	-1	0	0	0
offset	0	-2
hash	0xDF0A
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s %s
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -171 172 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220712	0	0	-1	0
-238	37
-238	172
END_LINE
inputport

0	3	coords= -171 182 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220713	0	0	-1	0
-228	192
-218	182
END_LINE
outputport

1	3	coords= -120 172 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	220734	1	1	-1	0
END_LINE
startport

2	3	coords= -171 162 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220713	0	2	-1	0
-228	182
-228	162
END_LINE
finishport

3	3	coords= -120 162 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	220786	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
220705	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-160	510	-110	550
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	format
trans	-1	0	0	0
offset	0	-2
hash	0xDF0A
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s %s
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -161 532 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220707	0	0	-1	0
-226	372
-226	532
END_LINE
inputport

0	3	coords= -161 542 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220708	0	0	-1	0
-226	537
-216	542
END_LINE
outputport

1	3	coords= -110 532 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	220734	2	1	-1	0
END_LINE
startport

2	3	coords= -161 522 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220708	0	2	-1	0
-216	527
-206	522
END_LINE
finishport

3	3	coords= -110 522 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	220840	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
220706	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-145	855	-95	895
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	format
trans	-1	0	0	0
offset	0	-2
hash	0xDF0A
Instance_Info	_
End_Instance
END_BA
parameter
Format	3	0	0	0	0	0	0	1.2	%s %s
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -146 877 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220709	0	0	-1	0
-201	727
-201	877
END_LINE
inputport

0	3	coords= -146 887 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220710	0	0	-1	0
-191	892
-181	887
END_LINE
outputport

1	3	coords= -95 877 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	220734	3	1	-1	0
END_LINE
startport

2	3	coords= -146 867 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220710	0	2	-1	0
-191	882
-181	867
END_LINE
finishport

3	3	coords= -95 867 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	220895	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
220707	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
-330	350	-280	495
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
Format	3	0	0	0	0	0	0	1.2	%d %d %d %d %d %d %d %d
EndOfValues
EndOfLabels
C-style format string - supports %d or %u only

inputport

0	1	coords= -331 372 0	0	0	1	mandatory= 0	0	1
i1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220696	1	0	-1	0
-381	417
-381	372
END_LINE
inputport

0	1	coords= -331 382 0	1	0	1	mandatory= 0	0	1
i2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220696	2	0	-1	0
-381	427
-381	382
END_LINE
inputport

0	1	coords= -331 392 0	2	0	1	mandatory= 0	0	1
i3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220696	3	0	-1	0
-381	437
-381	392
END_LINE
inputport

0	1	coords= -331 402 0	3	0	1	mandatory= 0	0	1
i4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220696	4	0	-1	0
-381	447
-381	402
END_LINE
inputport

0	1	coords= -331 412 0	4	0	1	mandatory= 0	0	1
i5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220696	5	0	-1	0
-381	457
-381	412
END_LINE
inputport

0	1	coords= -331 422 0	5	0	1	mandatory= 0	0	1
i6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220696	6	0	-1	0
-381	467
-381	422
END_LINE
inputport

0	1	coords= -331 432 0	6	0	1	mandatory= 0	0	1
i7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220696	7	0	-1	0
-381	477
-381	432
END_LINE
inputport

0	1	coords= -331 442 0	7	0	1	mandatory= 0	0	1
i8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220696	8	0	-1	0
-381	487
-381	442
END_LINE
inputport

0	3	coords= -331 462 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -280 372 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220528	0	1	-1	0
END_LINE
startport

2	3	coords= -331 362 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220696	0	2	-1	0
-381	387
-381	362
END_LINE
finishport

3	3	coords= -280 362 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220546	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
220708	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
-330	515	-280	660
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
Format	3	0	0	0	0	0	0	1.2	%d %d %d %d %d %d %d %d
EndOfValues
EndOfLabels
C-style format string - supports %d or %u only

inputport

0	1	coords= -331 537 0	0	0	1	mandatory= 0	0	1
i1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220696	9	0	-1	0
-391	497
-391	537
END_LINE
inputport

0	1	coords= -331 547 0	1	0	1	mandatory= 0	0	1
i2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220696	10	0	-1	0
-391	507
-391	547
END_LINE
inputport

0	1	coords= -331 557 0	2	0	1	mandatory= 0	0	1
i3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220696	11	0	-1	0
-391	517
-391	557
END_LINE
inputport

0	1	coords= -331 567 0	3	0	1	mandatory= 0	0	1
i4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220696	12	0	-1	0
-391	527
-391	567
END_LINE
inputport

0	1	coords= -331 577 0	4	0	1	mandatory= 0	0	1
i5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220696	13	0	-1	0
-391	537
-391	577
END_LINE
inputport

0	1	coords= -331 587 0	5	0	1	mandatory= 0	0	1
i6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220696	14	0	-1	0
-391	547
-391	587
END_LINE
inputport

0	1	coords= -331 597 0	6	0	1	mandatory= 0	0	1
i7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220696	15	0	-1	0
-391	557
-391	597
END_LINE
inputport

0	1	coords= -331 607 0	7	0	1	mandatory= 0	0	1
i8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220696	16	0	-1	0
-391	567
-391	607
END_LINE
inputport

0	3	coords= -331 627 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -280 537 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220528	1	1	-1	0
END_LINE
startport

2	3	coords= -331 527 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220707	0	2	-1	0
-235	362
-235	505
-376	505
-376	527
END_LINE
finishport

3	3	coords= -280 527 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220528	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
220709	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
-295	705	-245	850
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
Format	3	0	0	0	0	0	0	1.2	%d %d %d %d %d %d %d %d
EndOfValues
EndOfLabels
C-style format string - supports %d or %u only

inputport

0	1	coords= -296 727 0	0	0	1	mandatory= 0	0	1
i1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220711	0	0	-1	0
-336	720
-326	727
END_LINE
inputport

0	1	coords= -296 737 0	1	0	1	mandatory= 0	0	1
i2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220714	0	0	-1	0
-326	760
-326	737
END_LINE
inputport

0	1	coords= -296 747 0	2	0	1	mandatory= 0	0	1
i3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220715	0	0	-1	0
-326	800
-326	747
END_LINE
inputport

0	1	coords= -296 757 0	3	0	1	mandatory= 0	0	1
i4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220716	0	0	-1	0
-326	840
-326	757
END_LINE
inputport

0	1	coords= -296 767 0	4	0	1	mandatory= 0	0	1
i5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220717	0	0	-1	0
-326	885
-326	767
END_LINE
inputport

0	1	coords= -296 777 0	5	0	1	mandatory= 0	0	1
i6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220718	0	0	-1	0
-326	925
-326	777
END_LINE
inputport

0	1	coords= -296 787 0	6	0	1	mandatory= 0	0	1
i7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220719	0	0	-1	0
-326	965
-326	787
END_LINE
inputport

0	1	coords= -296 797 0	7	0	1	mandatory= 0	0	1
i8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220720	0	0	-1	0
-326	1005
-326	797
END_LINE
inputport

0	3	coords= -296 817 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -245 727 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220531	0	1	-1	0
END_LINE
startport

2	3	coords= -296 717 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220728	0	2	-1	0
-326	1330
-326	717
END_LINE
finishport

3	3	coords= -245 717 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220562	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
220710	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
-295	870	-245	1015
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
Format	3	0	0	0	0	0	0	1.2	%d %d %d %d %d %d %d %d
EndOfValues
EndOfLabels
C-style format string - supports %d or %u only

inputport

0	1	coords= -296 892 0	0	0	1	mandatory= 0	0	1
i1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220721	0	0	-1	0
-326	1055
-326	892
END_LINE
inputport

0	1	coords= -296 902 0	1	0	1	mandatory= 0	0	1
i2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220722	0	0	-1	0
-326	1095
-326	902
END_LINE
inputport

0	1	coords= -296 912 0	2	0	1	mandatory= 0	0	1
i3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220723	0	0	-1	0
-326	1135
-326	912
END_LINE
inputport

0	1	coords= -296 922 0	3	0	1	mandatory= 0	0	1
i4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220724	0	0	-1	0
-326	1175
-326	922
END_LINE
inputport

0	1	coords= -296 932 0	4	0	1	mandatory= 0	0	1
i5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220725	0	0	-1	0
-326	1220
-326	932
END_LINE
inputport

0	1	coords= -296 942 0	5	0	1	mandatory= 0	0	1
i6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220726	0	0	-1	0
-326	1260
-326	942
END_LINE
inputport

0	1	coords= -296 952 0	6	0	1	mandatory= 0	0	1
i7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220727	0	0	-1	0
-326	1300
-326	952
END_LINE
inputport

0	1	coords= -296 962 0	7	0	1	mandatory= 0	0	1
i8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220728	0	0	-1	0
-326	1340
-326	962
END_LINE
inputport

0	3	coords= -296 982 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -245 892 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220531	1	1	-1	0
END_LINE
startport

2	3	coords= -296 882 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220709	0	2	-1	0
-205	717
-205	860
-315	860
-315	882
END_LINE
finishport

3	3	coords= -245 882 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220531	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220711	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	700	-365	730
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 720 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	1	0	-1	0
-423	747
-423	720
END_LINE
outputport

1	1	coords= -365 720 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	0	1	-1	0
END_LINE
startport

2	1	coords= -396 710 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220699	0	2	-1	0
-423	717
-413	710
END_LINE
finishport

3	1	coords= -365 710 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220584	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
220712	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
-345	15	-295	160
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
Format	3	0	0	0	0	0	0	1.2	%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= -346 37 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220698	1	0	-1	0
-391	92
-391	37
END_LINE
inputport

0	2	coords= -346 47 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220698	2	0	-1	0
-391	102
-391	47
END_LINE
inputport

0	2	coords= -346 57 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220698	3	0	-1	0
-391	112
-391	57
END_LINE
inputport

0	2	coords= -346 67 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220698	4	0	-1	0
-391	122
-391	67
END_LINE
inputport

0	2	coords= -346 77 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220698	5	0	-1	0
-391	132
-391	77
END_LINE
inputport

0	2	coords= -346 87 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220698	6	0	-1	0
-391	142
-391	87
END_LINE
inputport

0	2	coords= -346 97 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220698	7	0	-1	0
-391	152
-391	97
END_LINE
inputport

0	2	coords= -346 107 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220698	8	0	-1	0
-391	162
-391	107
END_LINE
inputport

0	3	coords= -346 127 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -295 37 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220513	0	1	-1	0
END_LINE
startport

2	3	coords= -346 27 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220698	0	2	-1	0
-391	62
-391	27
END_LINE
finishport

3	3	coords= -295 27 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220583	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_real
_
220713	0	0	0
_
_
This is an 8-input real formatter. Use the format property to show how the string should appear.
string_format8_real
0
_
-345	170	-295	315
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
Format	3	0	0	0	0	0	0	1.2	%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f
EndOfValues
EndOfLabels
C-style format string - supports %f only (Should handle different decimal places)

inputport

0	2	coords= -346 192 0	0	0	1	mandatory= 0	0	1
r1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	220698	9	0	-1	0
-401	172
-401	192
END_LINE
inputport

0	2	coords= -346 202 0	1	0	1	mandatory= 0	0	1
r2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	220698	10	0	-1	0
-401	182
-401	202
END_LINE
inputport

0	2	coords= -346 212 0	2	0	1	mandatory= 0	0	1
r3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	220698	11	0	-1	0
-401	192
-401	212
END_LINE
inputport

0	2	coords= -346 222 0	3	0	1	mandatory= 0	0	1
r4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	220698	12	0	-1	0
-401	202
-401	222
END_LINE
inputport

0	2	coords= -346 232 0	4	0	1	mandatory= 0	0	1
r5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	220698	13	0	-1	0
-401	212
-401	232
END_LINE
inputport

0	2	coords= -346 242 0	5	0	1	mandatory= 0	0	1
r6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	220698	14	0	-1	0
-401	222
-401	242
END_LINE
inputport

0	2	coords= -346 252 0	6	0	1	mandatory= 0	0	1
r7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	220698	15	0	-1	0
-401	232
-401	252
END_LINE
inputport

0	2	coords= -346 262 0	7	0	1	mandatory= 0	0	1
r8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	220698	16	0	-1	0
-401	242
-401	262
END_LINE
inputport

0	3	coords= -346 282 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -295 192 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	220513	1	1	-1	0
END_LINE
startport

2	3	coords= -346 182 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	220712	0	2	-1	0
-250	27
-250	150
-391	150
-391	182
END_LINE
finishport

3	3	coords= -295 182 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	220513	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220714	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	740	-365	770
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 760 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	2	0	-1	0
-433	757
-423	760
END_LINE
outputport

1	1	coords= -365 760 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	1	1	-1	0
END_LINE
startport

2	1	coords= -396 750 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220711	0	2	-1	0
-320	710
-320	665
-420	665
-420	750
END_LINE
finishport

3	1	coords= -365 750 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220585	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220715	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	780	-365	810
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 800 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	3	0	-1	0
-433	767
-433	800
END_LINE
outputport

1	1	coords= -365 800 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	2	1	-1	0
END_LINE
startport

2	1	coords= -396 790 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220714	0	2	-1	0
-340	750
-340	735
-415	735
-415	790
END_LINE
finishport

3	1	coords= -365 790 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220586	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220716	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	820	-365	850
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 840 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	4	0	-1	0
-433	777
-433	840
END_LINE
outputport

1	1	coords= -365 840 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	3	1	-1	0
END_LINE
startport

2	1	coords= -396 830 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220715	0	2	-1	0
-340	790
-340	775
-420	775
-420	830
END_LINE
finishport

3	1	coords= -365 830 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220608	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220717	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	865	-365	895
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 885 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	5	0	-1	0
-433	787
-433	885
END_LINE
outputport

1	1	coords= -365 885 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	4	1	-1	0
END_LINE
startport

2	1	coords= -396 875 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220716	0	2	-1	0
-340	830
-340	815
-420	815
-420	875
END_LINE
finishport

3	1	coords= -365 875 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220609	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220718	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	905	-365	935
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 925 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	6	0	-1	0
-433	797
-433	925
END_LINE
outputport

1	1	coords= -365 925 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	5	1	-1	0
END_LINE
startport

2	1	coords= -396 915 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220717	0	2	-1	0
-350	875
-350	860
-420	860
-420	915
END_LINE
finishport

3	1	coords= -365 915 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220610	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220719	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	945	-365	975
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 965 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	7	0	-1	0
-433	807
-433	965
END_LINE
outputport

1	1	coords= -365 965 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	6	1	-1	0
END_LINE
startport

2	1	coords= -396 955 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220718	0	2	-1	0
-340	915
-340	900
-420	900
-420	955
END_LINE
finishport

3	1	coords= -365 955 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220611	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220720	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	985	-365	1015
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1005 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	8	0	-1	0
-433	817
-433	1005
END_LINE
outputport

1	1	coords= -365 1005 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220561	7	1	-1	0
END_LINE
startport

2	1	coords= -396 995 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220719	0	2	-1	0
-340	955
-340	940
-420	940
-420	995
END_LINE
finishport

3	1	coords= -365 995 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220638	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220721	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1035	-365	1065
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1055 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	9	0	-1	0
-433	827
-433	1055
END_LINE
outputport

1	1	coords= -365 1055 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	0	1	-1	0
END_LINE
startport

2	1	coords= -396 1045 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220720	0	2	-1	0
-340	995
-340	980
-420	980
-420	1045
END_LINE
finishport

3	1	coords= -365 1045 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220639	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220722	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1075	-365	1105
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1095 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	10	0	-1	0
-433	837
-433	1095
END_LINE
outputport

1	1	coords= -365 1095 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	1	1	-1	0
END_LINE
startport

2	1	coords= -396 1085 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220721	0	2	-1	0
-340	1045
-340	1025
-420	1025
-420	1085
END_LINE
finishport

3	1	coords= -365 1085 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220640	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220723	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1115	-365	1145
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1135 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	11	0	-1	0
-433	847
-433	1135
END_LINE
outputport

1	1	coords= -365 1135 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	2	1	-1	0
END_LINE
startport

2	1	coords= -396 1125 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220722	0	2	-1	0
-340	1085
-340	1070
-420	1070
-420	1125
END_LINE
finishport

3	1	coords= -365 1125 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220641	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220724	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1155	-365	1185
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1175 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	12	0	-1	0
-433	857
-433	1175
END_LINE
outputport

1	1	coords= -365 1175 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	3	1	-1	0
END_LINE
startport

2	1	coords= -396 1165 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220723	0	2	-1	0
-340	1125
-340	1110
-420	1110
-420	1165
END_LINE
finishport

3	1	coords= -365 1165 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220642	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220725	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1200	-365	1230
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1220 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	13	0	-1	0
-433	867
-433	1220
END_LINE
outputport

1	1	coords= -365 1220 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	4	1	-1	0
END_LINE
startport

2	1	coords= -396 1210 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220724	0	2	-1	0
-340	1165
-340	1150
-420	1150
-420	1210
END_LINE
finishport

3	1	coords= -365 1210 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220643	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220726	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1240	-365	1270
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1260 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	14	0	-1	0
-433	877
-433	1260
END_LINE
outputport

1	1	coords= -365 1260 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	5	1	-1	0
END_LINE
startport

2	1	coords= -396 1250 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220725	0	2	-1	0
-340	1210
-340	1190
-420	1190
-420	1250
END_LINE
finishport

3	1	coords= -365 1250 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220644	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220727	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1280	-365	1310
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1300 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	15	0	-1	0
-433	887
-433	1300
END_LINE
outputport

1	1	coords= -365 1300 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	6	1	-1	0
END_LINE
startport

2	1	coords= -396 1290 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220726	0	2	-1	0
-340	1250
-340	1235
-420	1235
-420	1290
END_LINE
finishport

3	1	coords= -365 1290 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220645	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbix
_
220728	0	0	0
_
_
This is a boolean to integer converter.
ConvertorBoolToInt
0
_
-395	1320	-365	1350
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	10	10	1.25	0	~
trans	-1	0	0	0
offset	0	0
hash	0xAC8B
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= -396 1340 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	1	EndOfFunc
BEGIN_LINE
1	220699	16	0	-1	0
-433	897
-433	1340
END_LINE
outputport

1	1	coords= -365 1340 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	2	EndOfFunc
BEGIN_LINE
0	220562	7	1	-1	0
END_LINE
startport

2	1	coords= -396 1330 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	0	EndOfFunc
BEGIN_LINE
1	220727	0	2	-1	0
-340	1290
-340	1275
-425	1275
-425	1330
END_LINE
finishport

3	1	coords= -365 1330 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToInt 1	3	EndOfFunc
BEGIN_LINE
0	220561	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
220729	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-875	45	-735	66
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
String Constant	3	0	100	1	5	4	0	1.2	{"key1": 11872.7635, "key2": 1873.4164, "key3": 8339.7553, "key4": 4084.6016, "key5": 6669.3042, "key6": 4617.5272, "key7": 741.4125, "key8": 6994.5864, "key9": 12329.665, "key10": 12010.0398, "key11": 5271.9021, "key12": 4105.637, "key13": 2128.5582, "key14":3651.6976,"key15":  2570.827,"key16": 10057.3284}
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -735 56 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
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
220730	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-865	395	-725	416
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
String Constant	3	0	100	1	5	4	0	1.2	{"key1": -412257455,"key2": 123429701,"key3": -1463172613, "key4": -1077231675, "key5": -33969510, "key6": -125396016, "key7": 1117763319, "key8": 217815343, "key9": 828375785, "key10": -2103413092, "key11": 2026929143, "key12": 375516407, "key13": 1463530636, "key14": 352549545, "key15": -1318373997, "key16": 315796531}
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -725 406 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	1	0	1	-1	0
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
220731	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-860	695	-720	716
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
String Constant	3	0	100	1	5	4	0	1.2	{"key1": true, "key2": false, "key3": false, "key4": true, "key5": false, "key6": true, "key7": false, "key8": true, "key9": false, "key10": false, "key11": true, "key12": false, "key13": true, "key14": false, "key15": false, "key16": true}
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -720 706 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	4	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
220733	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-630	-25	-610	25
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

2	-1	coords= -631 -15 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	220733	1	2	-1	0
-579	15
-579	-60
-676	-60
-676	-15
END_LINE
startport

2	-1	coords= -631 0 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-50
-686	-50
-686	0
END_LINE
startport

2	-1	coords= -631 15 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	220703	0	2	-1	0
-80	-183
-80	-35
-705	-35
-705	15
END_LINE
finishport

3	-1	coords= -624 -5 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	220698	0	3	-1	0
END_LINE
finishport

3	-1	coords= -624 15 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	220733	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
220734	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
90	-160	120	-40
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

0	3	coords= 89 -125 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	220703	0	0	-1	0
-28	-173
-28	-125
END_LINE
inputport

0	3	coords= 89 -100 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	220704	0	0	-1	0
-11	172
-11	-100
END_LINE
inputport

0	3	coords= 89 -75 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	220705	0	0	-1	0
-6	532
-6	-75
END_LINE
inputport

0	3	coords= 89 -50 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	220706	0	0	-1	0
2	877
2	-50
END_LINE
outputport

1	3	coords= 120 -125 0	0	0	1	mandatory= 0	0	1

write_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= 89 -135 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	220703	0	2	-1	0
-28	-183
-28	-135
END_LINE
startport

2	3	coords= 89 -110 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	220704	0	2	-1	0
-11	162
-11	-110
END_LINE
startport

2	3	coords= 89 -85 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	220705	0	2	-1	0
-6	522
-6	-85
END_LINE
startport

2	3	coords= 89 -60 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	220706	0	2	-1	0
2	867
2	-60
END_LINE
finishport

3	3	coords= 120 -135 0	0	0	1	mandatory= 0	0	1

write
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
220786	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-610	290	-590	340
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

2	-1	coords= -611 300 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	220786	1	2	-1	0
-559	330
-559	255
-656	255
-656	300
END_LINE
startport

2	-1	coords= -611 315 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	265
-666	265
-666	315
END_LINE
startport

2	-1	coords= -611 330 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	220704	0	2	-1	0
-65	162
-65	270
-675	270
-675	330
END_LINE
finishport

3	-1	coords= -604 310 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	220696	0	3	-1	0
END_LINE
finishport

3	-1	coords= -604 330 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	220733	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
220840	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-600	615	-580	665
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

2	-1	coords= -601 625 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	220840	1	2	-1	0
-549	655
-549	580
-646	580
-646	625
END_LINE
startport

2	-1	coords= -601 640 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	590
-656	590
-656	640
END_LINE
startport

2	-1	coords= -601 655 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	220705	0	2	-1	0
-55	522
-55	615
-665	615
-665	655
END_LINE
finishport

3	-1	coords= -594 635 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	220699	0	3	-1	0
END_LINE
finishport

3	-1	coords= -594 655 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	220733	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
220895	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
0	1015	20	1065
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

2	-1	coords= -1 1025 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	220895	1	2	-1	0
51	1055
51	980
-46	980
-46	1025
END_LINE
startport

2	-1	coords= -1 1040 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-138	-615
-138	1040
END_LINE
startport

2	-1	coords= -1 1055 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	220706	0	2	-1	0
-53	867
-53	1055
END_LINE
finishport

3	-1	coords= 6 1035 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	-1	coords= 6 1055 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	220733	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
