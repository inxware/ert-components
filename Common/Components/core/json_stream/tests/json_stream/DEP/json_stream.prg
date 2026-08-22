#V:2.1.2

CanvasSizeXY
1565	1295

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
write_res_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	138378	0	0	-1	0
50	-83
50	-30
-381	-30
-381	-595
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
write_res
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	138378	0	2	-1	0
50	-93
50	-10
-381	-10
-381	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	138328	0	3	-1	0
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
0	138324	2	3	-1	0
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

done_test
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	138332	0	2	-1	0
-816	325
-816	-610
END_LINE
finishport

3	3	coords= -655 -610 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

json_stream
_
138322	0	0	0
_
_
Processes a text stream and generates events related to JSON parsing.
json_stream_parser
0
_
-360	-265	-255	-30
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

0	3	coords= -361 -230 0	0	0	1	mandatory= 0	0	1
data

funcName= read 1	1	EndOfFunc
BEGIN_LINE
1	138326	0	0	-1	1
-521	-230
-511	-230
END_LINE
outputport

1	3	coords= -255 -230 0	0	0	1	mandatory= 0	0	1
--

funcName= read 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -255 -100 0	1	0	1	mandatory= 0	0	1
dataType

funcName= parse 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -255 -90 0	2	0	1	mandatory= 0	0	1
data

funcName= parse 2	2	EndOfFunc
BEGIN_LINE
0	138378	1	1	-1	0
END_LINE
outputport

1	3	coords= -255 -140 0	3	0	1	mandatory= 0	0	1
Key

funcName= parse 2	3	EndOfFunc
BEGIN_LINE
0	138378	0	1	-1	0
END_LINE
outputport

1	3	coords= -255 -130 0	4	0	1	mandatory= 0	0	1
parent

funcName= parse 2	4	EndOfFunc
BEGIN_LINE
0	138370	0	1	-1	0
END_LINE
outputport

1	1	coords= -255 -60 0	5	0	1	mandatory= 0	0	1
--

funcName= parse 2	5	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	coords= -361 -240 0	0	0	0	mandatory= 0	0	1
read

funcName= read 1	0	EndOfFunc
BEGIN_LINE
1	138326	0	2	-1	1
END_LINE
startport

2	-1	coords= -361 -210 0	1	0	0	mandatory= 0	0	1
next

funcName= parse 2	0	EndOfFunc
BEGIN_LINE
1	138324	0	2	-1	1
-380	-131
-380	-210
END_LINE
startport

2	-1	coords= -361 -40 0	2	0	0	mandatory= 0	0	1
eos

funcName= eos 3	0	EndOfFunc
BEGIN_LINE
1	138323	0	2	-1	1
-375	-64
-375	-40
END_LINE
finishport

3	3	coords= -255 -240 0	0	0	1	mandatory= 0	0	1
--

funcName= read 1	3	EndOfFunc
BEGIN_LINE
0	115336	2	3	-1	0
END_LINE
finishport

3	3	coords= -255 -210 0	1	0	1	mandatory= 0	0	1
--

funcName= parse 2	6	EndOfFunc
BEGIN_LINE
0	94414	1	3	-1	0
END_LINE
finishport

3	3	coords= -255 -40 0	2	0	1	mandatory= 0	0	1
--
end_of_file
funcName= eos 3	1	EndOfFunc
BEGIN_LINE
0	138371	0	3	-1	0
END_LINE
finishport

3	3	coords= -255 -200 0	6	0	1	mandatory= 0	0	1
startMap

funcName= parse 2	7	EndOfFunc
BEGIN_LINE
0	94419	0	3	-1	0
END_LINE
finishport

3	3	coords= -255 -190 0	4	0	1	mandatory= 0	0	1
endMap

funcName= parse 2	8	EndOfFunc
BEGIN_LINE
0	94413	2	3	-1	0
END_LINE
finishport

3	3	coords= -255 -150 0	5	0	1	mandatory= 0	0	1
mapKey

funcName= parse 2	9	EndOfFunc
BEGIN_LINE
0	138379	0	3	-1	0
END_LINE
finishport

3	3	coords= -255 -180 0	3	0	1	mandatory= 0	0	1
startArray

funcName= parse 2	10	EndOfFunc
BEGIN_LINE
0	138338	1	3	-1	0
END_LINE
finishport

3	3	coords= -255 -170 0	7	0	1	mandatory= 0	0	1
endArray

funcName= parse 2	11	EndOfFunc
BEGIN_LINE
0	138338	2	3	-1	0
END_LINE
finishport

3	2	coords= -255 -110 0	0	0	1	mandatory= 0	0	1
value

funcName= parse 2	12	EndOfFunc
BEGIN_LINE
0	138379	1	3	-1	0
END_LINE
finishport

3	3	coords= -255 -70 0	9	0	1	mandatory= 0	0	1
error

funcName= parse 2	13	EndOfFunc
BEGIN_LINE
0	94419	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
138323	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-405	-85	-385	-45
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

2	-1	coords= -406 -75 0	1	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	138322	9	2	-1	1
-210	-70
-210	-15
-450	-15
-450	-75
END_LINE
startport

2	3	coords= -406 -55 0	2	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	138322	4	2	-1	1
-210	-190
-210	-300
-470	-300
-470	-55
END_LINE
finishport

3	-1	coords= -393 -64 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	94413	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
138324	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-405	-165	-385	-100
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

2	-1	coords= -406 -155 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	138322	0	2	-1	0
-210	-240
-210	-285
-450	-285
-450	-155
END_LINE
startport

2	-1	coords= -406 -140 0	0	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	138322	3	2	-1	1
-210	-200
-210	-285
-450	-285
-450	-140
END_LINE
startport

2	-1	coords= -406 -125 0	0	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	138338	0	2	-1	0
-447	-91
-447	-125
END_LINE
startport

2	-1	coords= -406 -110 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	138322	5	2	-1	1
-210	-150
-210	-285
-450	-285
-450	-110
END_LINE
finishport

3	-1	coords= -393 -131 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	138322	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
138326	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-720	-265	-690	-145
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

0	3	coords= -721 -230 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	138327	0	0	-1	0
-766	-284
-766	-230
END_LINE
inputport

0	3	coords= -721 -205 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	138367	0	0	-1	0
-763	-194
-753	-205
END_LINE
inputport

0	3	coords= -721 -180 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	138373	0	0	-1	0
-761	-104
-761	-180
END_LINE
inputport

0	3	coords= -721 -155 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -690 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	138322	0	1	-1	0
END_LINE
startport

2	3	coords= -721 -240 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	138330	0	2	-1	1
-868	-160
-868	-240
END_LINE
startport

2	3	coords= -721 -215 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	138368	0	2	-1	0
-826	25
-826	-215
END_LINE
startport

2	3	coords= -721 -190 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	138371	0	2	-1	0
-780	225
-780	-190
END_LINE
startport

2	3	coords= -721 -165 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -690 -240 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	138322	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
138327	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-950	-295	-810	-274
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
String Constant	3	0	100	1	5	4	0	1.2	{"x":1,"y":2,"z":3,"t":"text"}
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -810 -284 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	138326	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_manager
_
138328	0	0	0
_
_
<nl?>        
state_manager
0
_
-1305	-245	-1240	-190
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
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	138329
EndOfValues
EndOfLabels
_

outputport

1	1	coords= -1240 -205 0	0	0	1	mandatory= 0	0	1
state

funcName= enabled 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= -1306 -235 0	0	0	0	mandatory= 0	0	1
enable
test_start
funcName= enabled 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-210	-665
-210	-280
-1351	-280
-1351	-235
END_LINE
finishport

3	1	coords= -1240 -235 0	0	0	1	mandatory= 0	0	1


funcName= enabled 1	3	EndOfFunc
BEGIN_LINE
0	138330	0	3	-1	0
END_LINE
finishport

3	1	coords= -1240 -220 0	1	0	1	mandatory= 0	0	1


funcName= enabled 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
sm_outputport

6	6	coords= -1273 -251 0	0	0	1	mandatory= 0	0	1


funcName= enabled 1	1	EndOfFunc
BEGIN_LINE
0	138329	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
138329	0	0	0
_
_
<nl?>        
state
0
_
-1150	-255	-1090	-195
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
Name	3	0	0	1	25	30	1	1.2	start
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138329
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -1141 -246 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	138328	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1092 -210 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138330	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
138330	0	0	0
_
_
<nl?>        
state_condition
0
_
-1055	-175	-1025	-145
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
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138329
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	138331
EndOfValues
EndOfLabels
_

startport

2	1	coords= -1056 -160 0	0	0	0	mandatory= 0	0	1


funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	138328	0	2	-1	0
-1153	-235
-1153	-160
END_LINE
finishport

3	6	coords= -1025 -160 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	138326	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -1042 -174 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	138329	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1030 -146 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138331	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
138331	0	0	0
_
_
<nl?>        
state
0
_
-1025	-85	-965	-25
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
Name	3	0	0	1	25	30	1	1.2	test1
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138331
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -996 -84 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	138330	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -976 -30 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138368	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
138332	0	0	0
_
_
<nl?>        
state_condition
0
_
-1240	310	-1210	340
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
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138372
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	138336
EndOfValues
EndOfLabels
_

startport

2	1	coords= -1241 325 0	0	0	0	mandatory= 0	0	1

end_of_file
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	138322	2	2	-1	0
-205	-40
-205	290
-1291	290
-1291	325
END_LINE
finishport

3	6	coords= -1210 325 0	0	0	1	mandatory= 0	0	1

done_test
funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
sm_inputport

5	6	coords= -1221 310 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	138372	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1221 341 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138336	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
138336	0	0	0
_
_
<nl?>        
state
0
_
-1330	375	-1270	435
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
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138336
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -1287 377 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	138332	0	5	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor4
_
138338	0	0	0
_
_
This is a 4-input event OR.
EventFourInputOr
0
_
-510	-125	-490	-60
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

2	3	coords= -511 -115 0	1	0	0	mandatory= 0	0	1

write_done
funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-215	-615
-215	-145
-561	-145
-561	-115
END_LINE
startport

2	-1	coords= -511 -100 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	138322	6	2	-1	1
-210	-180
-210	-300
-556	-300
-556	-100
END_LINE
startport

2	-1	coords= -511 -85 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	138322	7	2	-1	1
-210	-170
-210	-300
-556	-300
-556	-85
END_LINE
startport

2	-1	coords= -511 -70 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -498 -91 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	funcName= Or3 3	1	funcName= Or4 4	1	EndOfFunc
BEGIN_LINE
0	138324	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
138367	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-955	-205	-815	-184
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
String Constant	3	0	100	1	5	4	0	1.2	{"obj":{"item1":1,"item2":2}}
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -815 -194 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	138326	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
138368	0	0	0
_
_
<nl?>        
state_condition
0
_
-970	10	-940	40
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
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138331
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	138369
EndOfValues
EndOfLabels
_

startport

2	1	coords= -971 25 0	0	0	0	mandatory= 0	0	1

end_of_file
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	138322	2	2	-1	0
-210	-40
-210	-290
-1016	-290
-1016	25
END_LINE
finishport

3	6	coords= -940 25 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	138326	1	3	-1	0
END_LINE
sm_inputport

5	6	coords= -960 10 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	138331	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -961 41 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138369	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
138369	0	0	0
_
_
<nl?>        
state
0
_
-1000	105	-940	165
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
Name	3	0	0	1	25	30	1	1.2	test2
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138369
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -958 107 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	138368	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -977 166 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138371	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state_condition
_
138371	0	0	0
_
_
<nl?>        
state_condition
0
_
-1025	210	-995	240
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
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138369
EndOfValues
EndOfLabels
_

parameter
__nextStateId	1	-2147483648	2147483647	0	0	0	0	1.2	138372
EndOfValues
EndOfLabels
_

startport

2	1	coords= -1026 225 0	0	0	0	mandatory= 0	0	1

end_of_file
funcName= doCondition 1	0	EndOfFunc
BEGIN_LINE
1	138322	2	2	-1	0
-210	-40
-210	185
-1081	185
-1081	225
END_LINE
finishport

3	6	coords= -995 225 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	2	EndOfFunc
BEGIN_LINE
0	138326	2	3	-1	0
END_LINE
sm_inputport

5	6	coords= -1004 210 0	0	0	1	mandatory= 0	0	1


funcName= doCondition 1	1	EndOfFunc
BEGIN_LINE
1	138369	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1010 241 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138372	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

state
_
138372	0	0	0
_
_
<nl?>        
state
0
_
-1085	270	-1025	330
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
Name	3	0	0	1	25	30	1	1.2	test3
EndOfValues
EndOfLabels
_

parameter
__stateId	1	-2147483648	2147483647	0	0	0	0	1.2	138372
EndOfValues
EndOfLabels
_

sm_inputport

5	6	coords= -1032 280 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
1	138371	0	5	-1	0
END_LINE
sm_outputport

6	6	coords= -1089 287 0	0	0	1	mandatory= 0	0	1


EndOfFunc
BEGIN_LINE
0	138332	0	6	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
138373	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-950	-115	-810	-94
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
String Constant	3	0	100	1	5	4	0	1.2	[{"a1x":1,"a1y":2,"a1z":3},{"a2x":1,"a2y":2,"a2z":3},{"a3x":1,"a3y":2,"a3z":3}]
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -810 -104 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	138326	2	1	-1	0
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
138378	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-50	-105	0	-65
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
Format	3	0	0	0	0	0	0	1.2	k=%s,v=%s 
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -51 -83 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	138322	3	0	-1	0
-140	-140
-140	-83
END_LINE
inputport

0	3	coords= -51 -73 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	138322	2	0	-1	0
-158	-90
-148	-73
END_LINE
outputport

1	3	coords= 0 -83 0	0	0	1	mandatory= 0	0	1

write_res_str
funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -51 -93 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	138379	0	2	-1	0
-80	-102
-70	-93
END_LINE
finishport

3	3	coords= 0 -93 0	0	0	1	mandatory= 0	0	1

write_res
funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
138379	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
-115	-120	-95	-85
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

2	-1	coords= -116 -110 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	138322	5	2	-1	0
-181	-150
-181	-110
END_LINE
startport

2	-1	coords= -116 -95 0	1	0	0	mandatory= 1	0	1


funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	138322	8	2	-1	0
-181	-110
-171	-95
END_LINE
finishport

3	-1	coords= -109 -102 0	0	0	1	mandatory= 0	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	138378	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
