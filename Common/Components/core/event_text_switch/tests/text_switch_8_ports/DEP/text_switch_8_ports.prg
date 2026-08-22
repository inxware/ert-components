#V:2.1.2

CanvasSizeXY
1615	1561

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
1	588	0	0	-1	0
-160	-230
-160	-295
-381	-295
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
1	588	0	2	-1	0
-165	-240
-165	-290
-386	-290
-386	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	578	0	3	-1	0
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
0	720	1	3	-1	0
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
-625	-789
-625	-655
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
-445	-790
-445	-700
-540	-700
-540	-665
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
-430	-710
-430	-635
END_LINE
startport

2	-1	coords= -401 -620 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-215	-605
-215	-710
-430	-710
-430	-620
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
-613	-554
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
-805	-565	-665	-544
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

1	3	coords= -665 -554 0	0	0	1	mandatory= 0	0	1


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
-625	-789
-625	-655
-735	-655
-735	-610
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
1	722	2	2	-1	0
-110	335
-110	230
-726	230
-726	-620
END_LINE
finishport

3	3	coords= -645 -620 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
61	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	-400	-850	-350
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

2	-1	coords= -871 -390 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	61	1	2	-1	0
-814	-360
-814	-415
-921	-415
-921	-390
END_LINE
startport

2	-1	coords= -871 -375 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-425
-926	-425
-926	-375
END_LINE
startport

2	1	coords= -871 -360 0	1	0	0	mandatory= 0	0	1

str1_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	0	2	-1	0
-465	-190
-465	-280
-916	-280
-916	-360
END_LINE
finishport

3	-1	coords= -864 -380 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	1	3	-1	0
END_LINE
finishport

3	-1	coords= -864 -360 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
62	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	-330	-850	-280
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

2	-1	coords= -871 -320 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	62	1	2	-1	0
-814	-290
-814	-350
-921	-350
-921	-320
END_LINE
startport

2	-1	coords= -871 -305 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-355
-926	-355
-926	-305
END_LINE
startport

2	1	coords= -871 -290 0	2	0	0	mandatory= 0	0	1

str2_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	1	2	-1	0
-475	-175
-475	-365
-916	-365
-916	-290
END_LINE
finishport

3	-1	coords= -864 -310 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	2	3	-1	0
END_LINE
finishport

3	-1	coords= -864 -290 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	62	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
63	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	-260	-850	-210
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

2	-1	coords= -871 -250 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	63	1	2	-1	0
-814	-220
-814	-275
-921	-275
-921	-250
END_LINE
startport

2	-1	coords= -871 -235 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-285
-926	-285
-926	-235
END_LINE
startport

2	1	coords= -871 -220 0	3	0	0	mandatory= 0	0	1

str3_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	2	2	-1	0
-475	-160
-475	-295
-916	-295
-916	-220
END_LINE
finishport

3	-1	coords= -864 -240 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	3	3	-1	0
END_LINE
finishport

3	-1	coords= -864 -220 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	63	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
64	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	-195	-850	-145
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

2	-1	coords= -871 -185 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	64	1	2	-1	0
-814	-155
-814	-205
-921	-205
-921	-185
END_LINE
startport

2	-1	coords= -871 -170 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-220
-926	-220
-926	-170
END_LINE
startport

2	1	coords= -871 -155 0	4	0	0	mandatory= 0	0	1

str4_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	3	2	-1	0
-465	-145
-465	-300
-926	-300
-926	-155
END_LINE
finishport

3	-1	coords= -864 -175 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	4	3	-1	0
END_LINE
finishport

3	-1	coords= -864 -155 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	64	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
65	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	-125	-850	-75
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

2	-1	coords= -871 -115 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	65	1	2	-1	0
-814	-85
-814	-145
-921	-145
-921	-115
END_LINE
startport

2	-1	coords= -871 -100 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-150
-926	-150
-926	-100
END_LINE
startport

2	1	coords= -871 -85 0	5	0	0	mandatory= 0	0	1

str5_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	4	2	-1	0
-465	-130
-465	-300
-926	-300
-926	-85
END_LINE
finishport

3	-1	coords= -864 -105 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	5	3	-1	0
END_LINE
finishport

3	-1	coords= -864 -85 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
95	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	-50	-850	0
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

2	-1	coords= -871 -40 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	95	1	2	-1	0
-814	-10
-814	-70
-921	-70
-921	-40
END_LINE
startport

2	-1	coords= -871 -25 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-75
-926	-75
-926	-25
END_LINE
startport

2	1	coords= -871 -10 0	7	0	0	mandatory= 0	0	1

str6_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	5	2	-1	0
-475	-115
-475	-270
-916	-270
-916	-10
END_LINE
finishport

3	-1	coords= -864 -30 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	6	3	-1	0
END_LINE
finishport

3	-1	coords= -864 -10 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
126	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-870	25	-850	75
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

2	-1	coords= -871 35 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	126	1	2	-1	0
-814	65
-814	5
-921	5
-921	35
END_LINE
startport

2	-1	coords= -871 50 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	0
-926	0
-926	50
END_LINE
startport

2	1	coords= -871 65 0	6	0	0	mandatory= 0	0	1

str7_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	6	2	-1	0
-465	-100
-465	-10
-916	-10
-916	65
END_LINE
finishport

3	-1	coords= -864 45 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	7	3	-1	0
END_LINE
finishport

3	-1	coords= -864 65 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
577	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
-550	-245	-520	-60
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

0	3	coords= -551 -210 0	0	0	1	mandatory= 0	0	1
test
str_val
funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	578	0	0	-1	0
-596	-245
-596	-210
END_LINE
inputport

0	3	coords= -551 -190 0	1	0	1	mandatory= 0	0	1
1
str1
funcName= Run 1	2	EndOfFunc
BEGIN_LINE
1	579	0	0	-1	0
-633	136
-633	-190
END_LINE
inputport

0	3	coords= -551 -175 0	2	0	1	mandatory= 0	0	1
2
str2
funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	580	0	0	-1	0
-633	166
-633	-175
END_LINE
inputport

0	3	coords= -551 -160 0	3	0	1	mandatory= 0	0	1
3
str3
funcName= Run 1	4	EndOfFunc
BEGIN_LINE
1	581	0	0	-1	0
-633	196
-633	-160
END_LINE
inputport

0	3	coords= -551 -145 0	4	0	1	mandatory= 0	0	1
4
str4
funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	582	0	0	-1	0
-633	226
-633	-145
END_LINE
inputport

0	3	coords= -551 -130 0	5	0	1	mandatory= 0	0	1
5
str5
funcName= Run 1	6	EndOfFunc
BEGIN_LINE
1	583	0	0	-1	0
-633	256
-633	-130
END_LINE
inputport

0	3	coords= -551 -115 0	6	0	1	mandatory= 0	0	1
6
str6
funcName= Run 1	7	EndOfFunc
BEGIN_LINE
1	584	0	0	-1	0
-633	286
-633	-115
END_LINE
inputport

0	3	coords= -551 -100 0	7	0	1	mandatory= 0	0	1
7
str7
funcName= Run 1	8	EndOfFunc
BEGIN_LINE
1	585	0	0	-1	0
-633	316
-633	-100
END_LINE
inputport

0	3	coords= -551 -85 0	8	0	1	mandatory= 0	0	1
8
str8
funcName= Run 1	9	EndOfFunc
BEGIN_LINE
1	586	0	0	-1	0
-633	346
-633	-85
END_LINE
startport

2	3	coords= -551 -220 0	0	0	0	mandatory= 0	0	1
go
str_val_ev
funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	578	0	2	-1	0
-596	-255
-596	-220
END_LINE
finishport

3	3	coords= -520 -190 0	0	0	1	mandatory= 0	0	1

str1_ev
funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	61	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -175 0	1	0	1	mandatory= 0	0	1

str2_ev
funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	62	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -160 0	2	0	1	mandatory= 0	0	1

str3_ev
funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	63	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -145 0	3	0	1	mandatory= 0	0	1

str4_ev
funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	64	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -130 0	4	0	1	mandatory= 0	0	1

str5_ev
funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	65	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -115 0	5	0	1	mandatory= 0	0	1

str6_ev
funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	95	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -100 0	6	0	1	mandatory= 0	0	1

str7_ev
funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	95	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -85 0	7	0	1	mandatory= 0	0	1

str8_ev
funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	677	2	3	-1	0
END_LINE
finishport

3	3	coords= -520 -70 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8s
_
578	0	0	0
_
_
This is a 8-input string multiplexer.
MultiplexEightInputString
0
_
-695	-280	-665	-60
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

0	3	coords= -696 -245 0	0	0	1	mandatory= 0	0	1
i1
str1
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	579	0	0	-1	0
-728	136
-728	-245
END_LINE
inputport

0	3	coords= -696 -220 0	1	0	1	mandatory= 0	0	1
i2
str2
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	580	0	0	-1	0
-728	166
-728	-220
END_LINE
inputport

0	3	coords= -696 -195 0	2	0	1	mandatory= 0	0	1
i3
str3
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	581	0	0	-1	0
-728	196
-728	-195
END_LINE
inputport

0	3	coords= -696 -170 0	3	0	1	mandatory= 0	0	1
i4
str4
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	582	0	0	-1	0
-728	226
-728	-170
END_LINE
inputport

0	3	coords= -696 -145 0	4	0	1	mandatory= 0	0	1
i5
str5
funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	583	0	0	-1	0
-728	256
-728	-145
END_LINE
inputport

0	3	coords= -696 -120 0	5	0	1	mandatory= 0	0	1
i6
str6
funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	584	0	0	-1	0
-728	286
-728	-120
END_LINE
inputport

0	3	coords= -696 -95 0	6	0	1	mandatory= 0	0	1
i7
str7
funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	585	0	0	-1	0
-728	316
-728	-95
END_LINE
inputport

0	3	coords= -696 -70 0	7	0	1	mandatory= 0	0	1
i8
str8
funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	586	0	0	-1	0
-728	346
-728	-70
END_LINE
outputport

1	3	coords= -665 -245 0	0	0	1	mandatory= 0	0	1

str_val
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	588	0	1	-1	0
END_LINE
startport

2	3	coords= -696 -255 0	0	0	0	mandatory= 0	0	1

test_start
funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-305
-751	-305
-751	-255
END_LINE
startport

2	3	coords= -696 -230 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	61	0	2	-1	0
-785	-380
-785	-230
END_LINE
startport

2	3	coords= -696 -205 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	62	0	2	-1	0
-785	-310
-785	-205
END_LINE
startport

2	3	coords= -696 -180 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	63	0	2	-1	0
-785	-240
-785	-180
END_LINE
startport

2	3	coords= -696 -155 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	64	0	2	-1	0
-785	-175
-785	-155
END_LINE
startport

2	3	coords= -696 -130 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	65	0	2	-1	0
-775	-105
-775	-130
END_LINE
startport

2	3	coords= -696 -105 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	95	0	2	-1	0
-775	-30
-775	-105
END_LINE
startport

2	3	coords= -696 -80 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	126	0	2	-1	0
-775	45
-775	-80
END_LINE
finishport

3	3	coords= -665 -255 0	0	0	1	mandatory= 0	0	1

str_val_ev
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	588	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
579	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	125	-770	146
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
String Constant	3	0	100	1	5	4	0	1.2	str1
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 136 0	0	0	1	mandatory= 0	0	1

str1
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	0	1	-1	0
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
580	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	155	-770	176
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
String Constant	3	0	100	1	5	4	0	1.2	str2
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 166 0	0	0	1	mandatory= 0	0	1

str2
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	2	1	-1	0
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
581	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	185	-770	206
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
String Constant	3	0	100	1	5	4	0	1.2	str3
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 196 0	0	0	1	mandatory= 0	0	1

str3
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	722	4	1	-1	0
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
582	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	215	-770	236
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
String Constant	3	0	100	1	5	4	0	1.2	str4
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 226 0	0	0	1	mandatory= 0	0	1

str4
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	3	1	-1	0
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
583	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	245	-770	266
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
String Constant	3	0	100	1	5	4	0	1.2	str5
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 256 0	0	0	1	mandatory= 0	0	1

str5
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	4	1	-1	0
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
584	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	275	-770	296
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
String Constant	3	0	100	1	5	4	0	1.2	str6
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 286 0	0	0	1	mandatory= 0	0	1

str6
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	5	1	-1	0
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
585	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	305	-770	326
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
String Constant	3	0	100	1	5	4	0	1.2	str7
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 316 0	0	0	1	mandatory= 0	0	1

str7
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	6	1	-1	0
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
586	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-910	335	-770	356
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
String Constant	3	0	100	1	5	4	0	1.2	str8
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -770 346 0	0	0	1	mandatory= 0	0	1

str8
funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	676	7	1	-1	0
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
587	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
505	-285	535	-100
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
case 1	3	0	0	0	0	0	0	1.2	str1
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	0	0	0	0	0	0	1.2	str2
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	0	0	0	0	0	0	1.2	str3
EndOfValues
EndOfLabels
case 3 string

parameter
case 4	3	0	0	0	0	0	0	1.2	str4
EndOfValues
EndOfLabels
case 4 string

parameter
case 5	3	0	0	0	0	0	0	1.2	str5
EndOfValues
EndOfLabels
case 5 string

parameter
case 6	3	0	0	0	0	0	0	1.2	str6
EndOfValues
EndOfLabels
case 6 string

parameter
case 7	3	0	0	0	0	0	0	1.2	str7
EndOfValues
EndOfLabels
case 7 string

parameter
case 8	3	0	0	0	0	0	0	1.2	str8
EndOfValues
EndOfLabels
case 8 string

inputport

0	3	coords= 504 -250 0	0	0	1	mandatory= 0	0	1
test
str_p_val
funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	676	0	0	-1	0
409	-270
409	-250
END_LINE
inputport

0	3	coords= 504 -230 0	1	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -215 0	2	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -200 0	3	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -185 0	4	0	1	mandatory= 0	0	1
4

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -170 0	5	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -155 0	6	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -140 0	7	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 504 -125 0	8	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	coords= 504 -260 0	0	0	0	mandatory= 0	0	1
go
str_p_ev
funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	676	0	2	-1	0
409	-280
409	-260
END_LINE
finishport

3	3	coords= 535 -230 0	0	0	1	mandatory= 0	0	1

str_p_1_ev
funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	669	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -215 0	1	0	1	mandatory= 0	0	1

str_p_2_ev
funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	670	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -200 0	2	0	1	mandatory= 0	0	1

str_p_3_ev
funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	671	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -185 0	3	0	1	mandatory= 0	0	1

str_p_4_ev
funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	673	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -170 0	4	0	1	mandatory= 0	0	1

str_p_5_ev
funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	673	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -155 0	5	0	1	mandatory= 0	0	1

str_p_6_ev
funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	674	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -140 0	6	0	1	mandatory= 0	0	1

str_p_7_ev
funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	675	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -125 0	7	0	1	mandatory= 0	0	1

str_p_8_ev
funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	720	2	3	-1	0
END_LINE
finishport

3	3	coords= 535 -110 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
588	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
-240	-265	-210	-195
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

0	3	coords= -241 -230 0	0	0	1	mandatory= 0	0	1
i1
str_val
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	578	0	0	-1	0
-453	-245
-453	-230
END_LINE
inputport

0	3	coords= -241 -205 0	0	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	721	0	0	-1	0
-266	-173
-266	-205
END_LINE
outputport

1	3	coords= -210 -230 0	0	0	1	mandatory= 0	0	1

write_data_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -241 -240 0	0	0	0	mandatory= 0	0	1

str_val_ev
funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	578	0	2	-1	0
-438	-255
-428	-240
END_LINE
startport

2	3	coords= -241 -215 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	721	0	2	-1	0
-266	-183
-266	-215
END_LINE
finishport

3	3	coords= -210 -240 0	0	0	1	mandatory= 0	0	1

write_data
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
669	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	-425	130	-375
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

2	-1	coords= 109 -415 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	669	1	2	-1	0
166	-385
166	-440
59	-440
59	-415
END_LINE
startport

2	-1	coords= 109 -400 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	-400
END_LINE
startport

2	1	coords= 109 -385 0	1	0	0	mandatory= 0	0	1

str_p_1_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	0	2	-1	0
585	-230
585	-315
59	-315
59	-385
END_LINE
finishport

3	-1	coords= 116 -405 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	1	3	-1	0
END_LINE
finishport

3	-1	coords= 116 -385 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
670	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	-355	130	-305
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

2	-1	coords= 109 -345 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	670	1	2	-1	0
166	-315
166	-375
59	-375
59	-345
END_LINE
startport

2	-1	coords= 109 -330 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	-330
END_LINE
startport

2	1	coords= 109 -315 0	2	0	0	mandatory= 0	0	1

str_p_2_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	1	2	-1	0
580	-215
580	-380
64	-380
64	-315
END_LINE
finishport

3	-1	coords= 116 -335 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	2	3	-1	0
END_LINE
finishport

3	-1	coords= 116 -315 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	62	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
671	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	-285	130	-235
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

2	-1	coords= 109 -275 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	671	1	2	-1	0
166	-245
166	-300
59	-300
59	-275
END_LINE
startport

2	-1	coords= 109 -260 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	-260
END_LINE
startport

2	1	coords= 109 -245 0	3	0	0	mandatory= 0	0	1

str_p_3_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	2	2	-1	0
575	-200
575	-305
69	-305
69	-245
END_LINE
finishport

3	-1	coords= 116 -265 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	3	3	-1	0
END_LINE
finishport

3	-1	coords= 116 -245 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	63	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
672	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	-220	130	-170
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

2	-1	coords= 109 -210 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	672	1	2	-1	0
166	-180
166	-230
59	-230
59	-210
END_LINE
startport

2	-1	coords= 109 -195 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	-195
END_LINE
startport

2	1	coords= 109 -180 0	4	0	0	mandatory= 0	0	1

str_p_4_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	3	2	-1	0
570	-185
570	-300
74	-300
74	-180
END_LINE
finishport

3	-1	coords= 116 -200 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	4	3	-1	0
END_LINE
finishport

3	-1	coords= 116 -180 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	64	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
673	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	-150	130	-100
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

2	-1	coords= 109 -140 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	673	1	2	-1	0
166	-110
166	-170
59	-170
59	-140
END_LINE
startport

2	-1	coords= 109 -125 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	-125
END_LINE
startport

2	1	coords= 109 -110 0	5	0	0	mandatory= 0	0	1

str_p_5_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	4	2	-1	0
565	-170
565	-295
79	-295
79	-110
END_LINE
finishport

3	-1	coords= 116 -130 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	5	3	-1	0
END_LINE
finishport

3	-1	coords= 116 -110 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
674	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	-75	130	-25
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

2	-1	coords= 109 -65 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	674	1	2	-1	0
166	-35
166	-95
59	-95
59	-65
END_LINE
startport

2	-1	coords= 109 -50 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	-50
END_LINE
startport

2	1	coords= 109 -35 0	7	0	0	mandatory= 0	0	1

str_p_6_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	5	2	-1	0
610	-155
610	-330
34	-330
34	-35
END_LINE
finishport

3	-1	coords= 116 -55 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	6	3	-1	0
END_LINE
finishport

3	-1	coords= 116 -35 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
675	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
110	0	130	50
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

2	-1	coords= 109 10 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	675	1	2	-1	0
166	40
166	-20
59	-20
59	10
END_LINE
startport

2	-1	coords= 109 25 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-83	-615
-83	25
END_LINE
startport

2	1	coords= 109 40 0	6	0	0	mandatory= 0	0	1

str_p_7_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	6	2	-1	0
615	-140
615	0
89	0
89	40
END_LINE
finishport

3	-1	coords= 116 20 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	578	7	3	-1	0
END_LINE
finishport

3	-1	coords= 116 40 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8s
_
676	0	0	0
_
_
This is a 8-input string multiplexer.
MultiplexEightInputString
0
_
285	-305	315	-85
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

0	3	coords= 284 -270 0	0	0	1	mandatory= 0	0	1
i1
str1
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	579	0	0	-1	0
-238	136
-238	-270
END_LINE
inputport

0	3	coords= 284 -245 0	1	0	1	mandatory= 0	0	1
i2
str2
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	580	0	0	-1	0
-238	166
-238	-245
END_LINE
inputport

0	3	coords= 284 -220 0	2	0	1	mandatory= 0	0	1
i3
str3
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	581	0	0	-1	0
-238	196
-238	-220
END_LINE
inputport

0	3	coords= 284 -195 0	3	0	1	mandatory= 0	0	1
i4
str4
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	582	0	0	-1	0
-238	226
-238	-195
END_LINE
inputport

0	3	coords= 284 -170 0	4	0	1	mandatory= 0	0	1
i5
str5
funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	583	0	0	-1	0
-238	256
-238	-170
END_LINE
inputport

0	3	coords= 284 -145 0	5	0	1	mandatory= 0	0	1
i6
str6
funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	584	0	0	-1	0
-238	286
-238	-145
END_LINE
inputport

0	3	coords= 284 -120 0	6	0	1	mandatory= 0	0	1
i7
str7
funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	585	0	0	-1	0
-238	316
-238	-120
END_LINE
inputport

0	3	coords= 284 -95 0	7	0	1	mandatory= 0	0	1
i8
str8
funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	586	0	0	-1	0
-238	346
-238	-95
END_LINE
outputport

1	3	coords= 315 -270 0	0	0	1	mandatory= 0	0	1

str_p_val
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	588	1	1	-1	0
END_LINE
startport

2	3	coords= 284 -280 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	677	0	2	-1	0
222	-525
222	-280
END_LINE
startport

2	3	coords= 284 -255 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	669	0	2	-1	0
195	-405
195	-255
END_LINE
startport

2	3	coords= 284 -230 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	670	0	2	-1	0
195	-335
195	-230
END_LINE
startport

2	3	coords= 284 -205 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	671	0	2	-1	0
195	-265
195	-205
END_LINE
startport

2	3	coords= 284 -180 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	672	0	2	-1	0
195	-200
195	-180
END_LINE
startport

2	3	coords= 284 -155 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	673	0	2	-1	0
205	-130
205	-155
END_LINE
startport

2	3	coords= 284 -130 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	674	0	2	-1	0
205	-55
205	-130
END_LINE
startport

2	3	coords= 284 -105 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	675	0	2	-1	0
205	20
205	-105
END_LINE
finishport

3	3	coords= 315 -280 0	0	0	1	mandatory= 0	0	1

str_p_ev
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	588	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
677	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
145	-545	165	-495
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

2	-1	coords= 144 -535 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	677	1	2	-1	0
201	-505
201	-565
94	-565
94	-535
END_LINE
startport

2	-1	coords= 144 -520 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-56	-615
-56	-520
END_LINE
startport

2	-1	coords= 144 -505 0	2	0	0	mandatory= 0	0	1

str8_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	577	7	2	-1	0
-198	-85
-198	-505
END_LINE
finishport

3	-1	coords= 151 -525 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	676	0	3	-1	0
END_LINE
finishport

3	-1	coords= 151 -505 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	677	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
721	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
-350	-195	-300	-155
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
Format	3	0	0	0	0	0	0	1.2	%s_p
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	coords= -351 -173 0	1	0	1	mandatory= 0	0	1
s1
str_p_val
funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	676	0	0	-1	0
365	-270
365	-345
-401	-345
-401	-173
END_LINE
inputport

0	3	coords= -351 -163 0	1	0	1	mandatory= 0	0	1
s2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -300 -173 0	0	0	1	mandatory= 0	0	1


funcName= run 1	3	EndOfFunc
BEGIN_LINE
0	588	1	1	-1	0
END_LINE
startport

2	3	coords= -351 -183 0	1	0	0	mandatory= 0	0	1

str_p_ev
funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	676	0	2	-1	0
380	-280
380	-340
-416	-340
-416	-183
END_LINE
finishport

3	3	coords= -300 -183 0	0	0	1	mandatory= 0	0	1


funcName= run 1	4	EndOfFunc
BEGIN_LINE
0	588	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_text_switch
_
722	0	0	0
_
_
Switches Event based on text cases
TextCaseDemultiplexer_Event
0
_
-190	250	-160	435
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
case 1	3	0	0	0	0	0	0	1.2	str2
EndOfValues
EndOfLabels
case 1 string

parameter
case 2	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
case 2 string

parameter
case 3	3	0	0	0	0	0	0	1.2	"brown fox"
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

parameter
Find First	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Search for a first string that matches the case string.

inputport

0	3	coords= -191 285 0	0	0	1	mandatory= 0	0	1
test

funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	724	0	0	-1	0
-328	331
-328	285
END_LINE
inputport

0	3	coords= -191 305 0	1	0	1	mandatory= 0	0	1
1

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -191 320 0	2	0	1	mandatory= 0	0	1
2

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
1	726	0	0	-1	0
-241	511
-241	320
END_LINE
inputport

0	3	coords= -191 335 0	3	0	1	mandatory= 0	0	1
3

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -191 350 0	4	0	1	mandatory= 0	0	1
4
str3
funcName= Run 1	5	EndOfFunc
BEGIN_LINE
1	581	0	0	-1	0
-471	196
-471	350
END_LINE
inputport

0	3	coords= -191 365 0	5	0	1	mandatory= 0	0	1
5

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -191 380 0	6	0	1	mandatory= 0	0	1
6

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -191 395 0	7	0	1	mandatory= 0	0	1
7

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -191 410 0	8	0	1	mandatory= 0	0	1
8

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= -191 275 0	0	0	0	mandatory= 0	0	1
go

funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	775	0	2	-1	0
-325	105
-325	275
END_LINE
finishport

3	3	coords= -160 305 0	0	0	1	mandatory= 0	0	1


funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 320 0	1	0	1	mandatory= 0	0	1


funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -160 335 0	0	0	1	mandatory= 0	0	1

test_done
funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 350 0	3	0	1	mandatory= 0	0	1


funcName= Run 1	13	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 365 0	4	0	1	mandatory= 0	0	1


funcName= Run 1	14	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 380 0	5	0	1	mandatory= 0	0	1


funcName= Run 1	15	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 395 0	6	0	1	mandatory= 0	0	1


funcName= Run 1	16	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 410 0	7	0	1	mandatory= 0	0	1


funcName= Run 1	17	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -160 425 0	8	0	1	mandatory= 0	0	1
deflt

funcName= Run 1	18	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
724	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-565	320	-425	341
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
String Constant	3	0	100	1	5	4	0	1.2	the quick brown fox jumps over the lazy dog typing test
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -425 331 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	722	0	1	-1	0
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
726	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-440	500	-300	521
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
String Constant	3	0	100	1	5	4	0	1.2	qqqq
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -300 511 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	722	2	1	-1	0
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
775	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-455	85	-435	135
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

2	-1	coords= -456 95 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	775	1	2	-1	0
-399	125
-399	65
-506	65
-506	95
END_LINE
startport

2	-1	coords= -456 110 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	60
-511	60
-511	110
END_LINE
startport

2	-1	coords= -456 125 0	2	0	0	mandatory= 0	0	1

str_p_8_ev
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	587	7	2	-1	0
575	-125
575	55
-516	55
-516	125
END_LINE
finishport

3	-1	coords= -449 105 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	722	0	3	-1	0
END_LINE
finishport

3	-1	coords= -449 125 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	677	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
