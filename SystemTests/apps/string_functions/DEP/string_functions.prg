#V:2.1.2

CanvasSizeXY
1370	861

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
1	217347	0	0	-1	0
315	-188
315	-245
-376	-245
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
1	217347	0	2	-1	0
315	-198
315	-245
-376	-245
-376	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	217344	0	3	-1	0
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
test_done
funcName= Run_WriteFILE_WriteOnly_String 3	4	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
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
1	1	2	2	-1	0
-210	-615
-210	-745
-741	-745
-741	-610
END_LINE
finishport

3	3	coords= -655 -610 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_charats
_
217328	0	0	0
_
_
This function outputs the character at the specified index.
CharAtString
0
_
-225	-240	-200	-195
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	15	30	1.25	1	char_at
trans	-1	0	0	0
offset	0	0
hash	0xFE14
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -226 -220 0	0	0	1	mandatory= 0	0	1


funcName= Run_CharAtString 1	1	EndOfFunc
BEGIN_LINE
1	217343	0	0	-1	1
-415	-295
-415	-220
END_LINE
inputport

0	1	coords= -226 -210 0	1	0	1	mandatory= 0	0	1


funcName= Run_CharAtString 1	2	EndOfFunc
BEGIN_LINE
1	217332	0	0	-1	1
-246	-265
-246	-210
END_LINE
outputport

1	3	coords= -200 -220 0	0	0	1	mandatory= 0	0	1


funcName= Run_CharAtString 1	3	EndOfFunc
BEGIN_LINE
0	217285	1	1	-1	0
END_LINE
startport

2	3	coords= -226 -230 0	0	0	0	mandatory= 0	0	1


funcName= Run_CharAtString 1	0	EndOfFunc
BEGIN_LINE
1	217332	0	2	-1	0
-241	-275
-241	-230
END_LINE
finishport

3	3	coords= -200 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_CharAtString 1	4	EndOfFunc
BEGIN_LINE
0	217282	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
217329	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-575	-245	-525	-205
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
Format	3	0	0	0	0	0	0	1.2	%s%s
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -576 -223 0	1	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	217340	0	0	-1	0
-605	-359
-605	-223
END_LINE
inputport

0	3	coords= -576 -213 0	0	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	217339	0	0	-1	1
-628	-230
-618	-213
END_LINE
outputport

1	3	coords= -525 -223 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	217278	0	1	-1	0
END_LINE
startport

2	3	coords= -576 -233 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	217339	0	2	-1	0
-628	-240
-618	-233
END_LINE
finishport

3	3	coords= -525 -233 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	217278	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_lens
_
217330	0	0	0
_
_
This function returns the length of the string.
LenString
0
_
-395	-315	-365	-275
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

0	3	coords= -396 -295 0	0	0	1	mandatory= 0	0	1


funcName= Run_LenString 1	1	EndOfFunc
BEGIN_LINE
1	217343	0	0	-1	1
-411	-295
-401	-295
END_LINE
outputport

1	1	coords= -365 -295 0	0	0	1	mandatory= 0	0	1


funcName= Run_LenString 1	2	EndOfFunc
BEGIN_LINE
0	217280	0	1	-1	0
END_LINE
startport

2	3	coords= -396 -305 0	0	0	0	mandatory= 0	0	1


funcName= Run_LenString 1	0	EndOfFunc
BEGIN_LINE
1	217343	0	2	-1	0
-426	-305
-416	-305
END_LINE
finishport

3	1	coords= -365 -305 0	0	0	1	mandatory= 0	0	1


funcName= Run_LenString 1	3	EndOfFunc
BEGIN_LINE
0	217280	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cmps
_
217331	0	0	0
_
_
This is a 2-input string comparator.
CmpString
0
_
65	-200	100	-155
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	cmp
trans	-1	0	0	0
offset	0	0
hash	0xC389
Instance_Info	_
End_Instance
END_BA
parameter
Compare String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= 64 -180 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CmpString 1	1	EndOfFunc
BEGIN_LINE
1	217336	0	0	-1	1
-3	-195
7	-180
END_LINE
inputport

0	3	coords= 64 -170 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CmpString 1	2	EndOfFunc
BEGIN_LINE
1	217338	0	0	-1	1
-58	-9
-58	-170
END_LINE
outputport

1	0	coords= 100 -180 0	0	0	1	mandatory= 0	0	1


funcName= Run_CmpString 1	3	EndOfFunc
BEGIN_LINE
0	217295	0	1	-1	0
END_LINE
startport

2	0	coords= 64 -190 0	0	0	0	mandatory= 0	0	1


funcName= Run_CmpString 1	0	EndOfFunc
BEGIN_LINE
1	217336	0	2	-1	1
12	-205
22	-190
END_LINE
finishport

3	0	coords= 100 -190 0	0	0	1	mandatory= 0	0	1


funcName= Run_CmpString 1	4	EndOfFunc
BEGIN_LINE
0	217295	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

operator_subix
_
217332	0	0	0
_
_
This is a 2-input integer subtractor.
SubtractionInt
0
_
-305	-285	-275	-240
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	3	1.25	0	a-b
trans	-1	0	0	0
offset	0	0
hash	0xD06E
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -306 -265 0	0	0	1	mandatory= 1	0	1
a

funcName= Run_SubtractionInt 1	1	EndOfFunc
BEGIN_LINE
1	217330	0	0	-1	1
-336	-295
-336	-265
END_LINE
inputport

0	1	coords= -306 -255 0	1	0	1	mandatory= 1	0	1
b

funcName= Run_SubtractionInt 1	2	EndOfFunc
BEGIN_LINE
1	217333	0	0	-1	0
-343	-135
-343	-255
END_LINE
outputport

1	1	coords= -275 -265 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubtractionInt 1	3	EndOfFunc
BEGIN_LINE
0	217275	1	1	-1	0
END_LINE
startport

2	1	coords= -306 -275 0	0	0	0	mandatory= 0	0	1


funcName= Run_SubtractionInt 1	0	EndOfFunc
BEGIN_LINE
1	217330	0	2	-1	0
-330	-305
-330	-275
END_LINE
finishport

3	1	coords= -275 -275 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubtractionInt 1	4	EndOfFunc
BEGIN_LINE
0	217275	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
217333	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-485	-145	-390	-125
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

1	1	coords= -390 -135 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	217280	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_lwrs
_
217334	0	0	0
_
_
This function converts a string to lowercase.
ToLowerString
0
_
-205	-150	-175	-105
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	25	1.25	1	lower
trans	-1	0	0	0
offset	0	0
hash	0x3BCE
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -206 -130 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToLowerString 1	1	EndOfFunc
BEGIN_LINE
1	217335	0	0	-1	0
-251	-99
-251	-130
END_LINE
outputport

1	3	coords= -175 -130 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToLowerString 1	2	EndOfFunc
BEGIN_LINE
0	217285	1	1	-1	0
END_LINE
startport

2	3	coords= -206 -140 0	0	0	0	mandatory= 0	0	1


funcName= Run_ToLowerString 1	0	EndOfFunc
BEGIN_LINE
1	217328	0	2	-1	0
-160	-230
-160	-180
-251	-180
-251	-140
END_LINE
finishport

3	3	coords= -175 -140 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToLowerString 1	3	EndOfFunc
BEGIN_LINE
0	217285	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
217335	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-485	-110	-345	-89
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
String Constant	3	0	100	1	5	4	0	1.2	WORL1
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -345 -99 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	217282	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_inserts
_
217336	0	0	0
_
_
This function inserts a string s2 into string s1 at the specified position.
InsertString
0
_
-100	-215	-70	-165
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	21	37	1.25	1	Insert
trans	-1	0	0	0
offset	0	0
hash	0xC01A
Instance_Info	_
End_Instance
END_BA
parameter
index	1	0	1024	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
0-based index where to insert string

parameter
string to insert	3	0	1024	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to inser

inputport

0	3	coords= -101 -195 0	1	0	1	mandatory= 0	0	1
s1

funcName= Run_InsertString 1	1	EndOfFunc
BEGIN_LINE
1	217334	0	0	-1	1
-121	-130
-121	-195
END_LINE
inputport

0	3	coords= -101 -185 0	0	0	1	mandatory= 0	0	1
s2

funcName= Run_InsertString 1	2	EndOfFunc
BEGIN_LINE
1	217328	0	0	-1	1
-170	-220
-170	-185
END_LINE
inputport

0	1	coords= -101 -175 0	2	0	1	mandatory= 0	0	1


funcName= Run_InsertString 1	3	EndOfFunc
BEGIN_LINE
1	217337	0	0	-1	0
-160	-40
-160	-175
END_LINE
outputport

1	3	coords= -70 -195 0	0	0	1	mandatory= 0	0	1


funcName= Run_InsertString 1	4	EndOfFunc
BEGIN_LINE
0	217296	1	1	-1	0
END_LINE
startport

2	3	coords= -101 -205 0	0	0	0	mandatory= 0	0	1


funcName= Run_InsertString 1	0	EndOfFunc
BEGIN_LINE
1	217334	0	2	-1	0
-130	-140
-130	-205
END_LINE
finishport

3	3	coords= -70 -205 0	0	0	1	mandatory= 0	0	1


funcName= Run_InsertString 1	5	EndOfFunc
BEGIN_LINE
0	217279	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
217337	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-310	-50	-215	-30
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

1	1	coords= -215 -40 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	217285	2	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
217338	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-310	-20	-170	1
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
String Constant	3	0	100	1	5	4	0	1.2	world1
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -170 -9 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	217296	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_lwrs
_
217339	0	0	0
_
_
This function converts a string to lowercase.
ToLowerString
0
_
-700	-250	-670	-205
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	25	1.25	1	lower
trans	-1	0	0	0
offset	0	0
hash	0x3BCE
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -701 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToLowerString 1	1	EndOfFunc
BEGIN_LINE
1	217341	0	0	-1	1
-728	-230
-718	-230
END_LINE
outputport

1	3	coords= -670 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToLowerString 1	2	EndOfFunc
BEGIN_LINE
0	217276	1	1	-1	0
END_LINE
startport

2	3	coords= -701 -240 0	0	0	0	mandatory= 0	0	1


funcName= Run_ToLowerString 1	0	EndOfFunc
BEGIN_LINE
1	217341	0	2	-1	0
-728	-240
-718	-240
END_LINE
finishport

3	3	coords= -670 -240 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToLowerString 1	3	EndOfFunc
BEGIN_LINE
0	217276	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
217340	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-815	-370	-675	-349
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
String Constant	3	0	100	1	5	4	0	1.2	hello
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -675 -359 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	217296	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_uprs
_
217341	0	0	0
_
_
This function converts a string to uppercase.
ToUpperString
0
_
-795	-250	-765	-205
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	20	27	1.25	1	upper
trans	-1	0	0	0
offset	0	0
hash	0xF2A3
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= -796 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToUpperString 1	1	EndOfFunc
BEGIN_LINE
1	217344	0	0	-1	0
-816	-215
-806	-230
END_LINE
outputport

1	3	coords= -765 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToUpperString 1	2	EndOfFunc
BEGIN_LINE
0	217288	0	1	-1	0
END_LINE
startport

2	3	coords= -796 -240 0	0	0	0	mandatory= 0	0	1


funcName= Run_ToUpperString 1	0	EndOfFunc
BEGIN_LINE
1	217344	0	2	-1	0
-816	-225
-806	-240
END_LINE
finishport

3	3	coords= -765 -240 0	0	0	1	mandatory= 0	0	1


funcName= Run_ToUpperString 1	3	EndOfFunc
BEGIN_LINE
0	217288	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
217342	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1105	-150	-965	-129
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
String Constant	3	0	100	1	5	4	0	1.2	rld
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -965 -139 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	217293	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

string_subs
_
217343	0	0	0
_
_
This function outputs a substring specified by a start index and length.
SubString
0
_
-475	-315	-435	-255
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	35	43	1.25	1	Substring
trans	-1	0	0	0
offset	0	0
hash	0xA876
Instance_Info	_
End_Instance
END_BA
parameter
start	1	0	1024	0	0	0	0	1.2	5
EndOfValues
EndOfLabels
0-based index to start string extract

parameter
length	1	0	1024	0	0	0	0	1.2	5
EndOfValues
EndOfLabels
Number of characters to extract

inputport

0	3	coords= -476 -295 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubString 1	1	EndOfFunc
BEGIN_LINE
1	217329	0	0	-1	1
-495	-223
-495	-295
END_LINE
inputport

0	1	coords= -476 -285 0	1	0	1	mandatory= 0	0	1
start

funcName= Run_SubString 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -476 -275 0	2	0	1	mandatory= 0	0	1
length

funcName= Run_SubString 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -435 -295 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubString 1	4	EndOfFunc
BEGIN_LINE
0	217275	0	1	-1	0
END_LINE
startport

2	1	coords= -476 -305 0	0	0	0	mandatory= 0	0	1


funcName= Run_SubString 1	0	EndOfFunc
BEGIN_LINE
1	217329	0	2	-1	0
-505	-233
-505	-305
END_LINE
finishport

3	3	coords= -435 -305 0	0	0	1	mandatory= 0	0	1


funcName= Run_SubString 1	5	EndOfFunc
BEGIN_LINE
0	217278	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
217344	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
-875	-240	-845	-195
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

0	3	coords= -876 -215 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CatString 1	1	EndOfFunc
BEGIN_LINE
1	217345	0	0	-1	0
-918	-179
-918	-215
END_LINE
inputport

0	3	coords= -876 -205 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CatString 1	2	EndOfFunc
BEGIN_LINE
1	217342	0	0	-1	0
-903	-139
-903	-205
END_LINE
outputport

1	3	coords= -845 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	3	EndOfFunc
BEGIN_LINE
0	217290	0	1	-1	0
END_LINE
startport

2	3	coords= -876 -225 0	0	0	0	mandatory= 0	0	1

test_start
funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-265
-931	-265
-931	-225
END_LINE
finishport

3	3	coords= -845 -225 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	217290	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
217345	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-1110	-190	-970	-169
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
String Constant	3	0	100	1	5	4	0	1.2	wo
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -970 -179 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	217293	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventboolctrl
_
217346	0	0	0
_
_
This is an event controlled by a boolean.
EventBooleanControl
0
_
135	-200	158	-170
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

0	0	coords= 134 -180 0	0	0	1	mandatory= 1	0	1


funcName= Update 1	1	EndOfFunc
BEGIN_LINE
1	217331	0	0	-1	0
117	-180
127	-180
END_LINE
startport

2	0	coords= 134 -190 0	0	0	0	mandatory= 1	0	1


funcName= Update 1	0	EndOfFunc
BEGIN_LINE
1	217331	0	2	-1	0
122	-190
132	-190
END_LINE
finishport

3	0	coords= 158 -190 0	0	0	1	mandatory= 1	0	1
t

funcName= Update 1	2	EndOfFunc
BEGIN_LINE
0	217296	0	3	-1	0
END_LINE
finishport

3	0	coords= 158 -180 0	1	0	1	mandatory= 0	0	1
f

funcName= Update 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8
_
217347	0	0	0
_
_
This is an 8-input string formatter. Use the format property to show how the string should appear.
string_format8
0
_
210	-210	260	-65
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
Format	3	0	0	0	0	0	0	1.2	%s %s
EndOfValues
EndOfLabels
C-style format string - supports %s only

inputport

0	3	coords= 209 -188 0	0	0	1	mandatory= 0	0	1
s1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	217340	0	0	-1	0
180	-359
180	-188
END_LINE
inputport

0	3	coords= 209 -178 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	217338	0	0	-1	0
165	-9
165	-178
END_LINE
inputport

0	3	coords= 209 -168 0	2	0	1	mandatory= 0	0	1
s3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 209 -158 0	3	0	1	mandatory= 0	0	1
s4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 209 -148 0	4	0	1	mandatory= 0	0	1
s5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 209 -138 0	5	0	1	mandatory= 0	0	1
s6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 209 -128 0	6	0	1	mandatory= 0	0	1
s7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 209 -118 0	7	0	1	mandatory= 0	0	1
s8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 209 -98 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= 260 -188 0	0	0	1	mandatory= 0	0	1

write_data_str
funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= 209 -198 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	217346	0	2	-1	0
183	-190
193	-198
END_LINE
finishport

3	3	coords= 260 -198 0	0	0	1	mandatory= 0	0	1

write_data
funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
