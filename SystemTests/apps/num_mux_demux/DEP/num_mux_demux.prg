#V:2.1.2

CanvasSizeXY
953	990

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
write_res_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	724	0	0	-1	0
-366	-340
-366	-595
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
write_res
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	724	0	2	-1	0
-366	-350
-366	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	61	2	3	-1	0
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
0	892	1	3	-1	0
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
-600	-789
-600	-655
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
-450	-790
-450	-700
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
-215	-715
-440	-715
-440	-635
END_LINE
startport

2	-1	coords= -401 -620 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	5	2	-1	0
-215	-605
-215	-715
-440	-715
-440	-620
END_LINE
startport

2	-1	coords= -401 -605 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	22	0	2	-1	0
-425	-571
-425	-605
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
-600	-789
-600	-655
-726	-655
-726	-610
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
1	720	0	2	-1	0
-119	-245
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

trigger_eventManRstAnd2
_
61	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1030	-365	-1010	-315
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

2	-1	coords= -1031 -355 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	61	1	2	-1	0
-974	-325
-974	-385
-1081	-385
-1081	-355
END_LINE
startport

2	-1	coords= -1031 -340 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-390
-1086	-390
-1086	-340
END_LINE
startport

2	1	coords= -1031 -325 0	1	0	0	mandatory= 0	0	1

test_start
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-225	-665
-225	-395
-1091	-395
-1091	-325
END_LINE
finishport

3	-1	coords= -1024 -345 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	761	2	3	-1	0
END_LINE
finishport

3	-1	coords= -1024 -325 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
720	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-170	-265	-150	-215
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

2	-1	coords= -171 -255 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	720	1	2	-1	0
-114	-225
-114	-285
-221	-285
-221	-255
END_LINE
startport

2	-1	coords= -171 -240 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-213	-615
-213	-240
END_LINE
startport

2	-1	coords= -171 -225 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	892	0	2	-1	0
-370	100
-370	-225
END_LINE
finishport

3	-1	coords= -164 -245 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	-1	coords= -164 -225 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	677	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8r
_
723	0	0	0
_
_
This is a 8-input real multiplexer.
MultiplexEightInputReal
0
_
-795	-330	-765	-110
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x8571
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -796 -295 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	725	0	0	-1	0
-826	-290
-816	-295
END_LINE
inputport

0	2	coords= -796 -270 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	726	0	0	-1	0
-831	-260
-821	-270
END_LINE
inputport

0	2	coords= -796 -245 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	727	0	0	-1	0
-836	-230
-826	-245
END_LINE
inputport

0	2	coords= -796 -220 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	728	0	0	-1	0
-825	-200
-825	-220
END_LINE
inputport

0	2	coords= -796 -195 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	729	0	0	-1	0
-825	-170
-825	-195
END_LINE
inputport

0	2	coords= -796 -170 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	730	0	0	-1	0
-830	-140
-830	-170
END_LINE
inputport

0	2	coords= -796 -145 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	731	0	0	-1	0
-830	-110
-830	-145
END_LINE
inputport

0	2	coords= -796 -120 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	732	0	0	-1	0
-825	-80
-825	-120
END_LINE
outputport

1	2	coords= -765 -295 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	824	0	1	-1	0
END_LINE
startport

2	2	coords= -796 -305 0	0	0	0	mandatory= 0	0	1

test_start
funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-255	-665
-255	-390
-886	-390
-886	-305
END_LINE
startport

2	2	coords= -796 -280 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	61	0	2	-1	0
-865	-345
-865	-280
END_LINE
startport

2	2	coords= -796 -255 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	761	0	2	-1	0
-863	-275
-863	-255
END_LINE
startport

2	2	coords= -796 -230 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	791	0	2	-1	0
-970	-195
-970	-230
END_LINE
startport

2	2	coords= -796 -205 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	822	0	2	-1	0
-970	-120
-970	-205
END_LINE
startport

2	2	coords= -796 -180 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	823	0	2	-1	0
-955	-40
-955	-180
END_LINE
startport

2	2	coords= -796 -155 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	858	0	2	-1	0
-955	30
-955	-155
END_LINE
startport

2	2	coords= -796 -130 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	892	0	2	-1	0
-955	100
-955	-130
END_LINE
finishport

3	2	coords= -765 -305 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	824	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
724	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
0
_
-425	-360	-395	-330
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
inputport

0	2	coords= -426 -340 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	1	EndOfFunc
BEGIN_LINE
1	825	0	0	-1	0
-461	-315
-461	-340
END_LINE
outputport

1	3	coords= -395 -340 0	0	0	1	mandatory= 0	0	1

write_res_str
funcName= Run_ConvertorRealToString 1	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -426 -350 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	0	EndOfFunc
BEGIN_LINE
1	825	0	2	-1	0
-461	-325
-461	-350
END_LINE
finishport

3	3	coords= -395 -350 0	0	0	1	mandatory= 0	0	1

write_res
funcName= Run_ConvertorRealToString 1	3	EndOfFunc
BEGIN_LINE
0	720	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
725	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-300	-855	-280
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	123456.78
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -855 -290 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	0	1	-1	0
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
726	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-270	-855	-250
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	0.99
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -855 -260 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	1	1	-1	0
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
727	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-240	-855	-220
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

1	2	coords= -855 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	2	1	-1	0
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
728	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-210	-855	-190
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	999.99
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -855 -200 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	3	1	-1	0
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
729	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-180	-855	-160
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

1	2	coords= -855 -170 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	4	1	-1	0
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
730	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-150	-855	-130
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	-123456.78
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -855 -140 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	5	1	-1	0
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
731	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-120	-855	-100
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
Real Constant	2	-1.7E308	1.7E+308	1	5	4	0	1.2	-0.99
EndOfValues
EndOfLabels
real constant

outputport

1	2	coords= -855 -110 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	6	1	-1	0
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
732	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-950	-90	-855	-70
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

1	2	coords= -855 -80 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	723	7	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
761	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1035	-295	-1015	-245
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

2	-1	coords= -1036 -285 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	761	1	2	-1	0
-979	-255
-979	-315
-1086	-315
-1086	-285
END_LINE
startport

2	-1	coords= -1036 -270 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-320
-1091	-320
-1091	-270
END_LINE
startport

2	1	coords= -1036 -255 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	61	0	2	-1	0
-975	-345
-975	-385
-1105	-385
-1105	-255
END_LINE
finishport

3	-1	coords= -1029 -275 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	791	2	3	-1	0
END_LINE
finishport

3	-1	coords= -1029 -255 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
791	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1040	-215	-1020	-165
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

2	-1	coords= -1041 -205 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	791	1	2	-1	0
-984	-175
-984	-235
-1091	-235
-1091	-205
END_LINE
startport

2	-1	coords= -1041 -190 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-240
-1096	-240
-1096	-190
END_LINE
startport

2	1	coords= -1041 -175 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	761	0	2	-1	0
-980	-275
-980	-315
-1115	-315
-1115	-175
END_LINE
finishport

3	-1	coords= -1034 -195 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	822	2	3	-1	0
END_LINE
finishport

3	-1	coords= -1034 -175 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
822	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1045	-140	-1025	-90
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

2	-1	coords= -1046 -130 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	822	1	2	-1	0
-989	-100
-989	-160
-1096	-160
-1096	-130
END_LINE
startport

2	-1	coords= -1046 -115 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-165
-1101	-165
-1101	-115
END_LINE
startport

2	1	coords= -1046 -100 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	791	0	2	-1	0
-985	-195
-985	-235
-1110	-235
-1110	-100
END_LINE
finishport

3	-1	coords= -1039 -120 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	720	2	3	-1	0
END_LINE
finishport

3	-1	coords= -1039 -100 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
823	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1050	-60	-1030	-10
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

2	-1	coords= -1051 -50 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	823	1	2	-1	0
-994	-20
-994	-80
-1101	-80
-1101	-50
END_LINE
startport

2	-1	coords= -1051 -35 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-85
-1106	-85
-1106	-35
END_LINE
startport

2	1	coords= -1051 -20 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	822	0	2	-1	0
-990	-120
-990	-160
-1115	-160
-1115	-20
END_LINE
finishport

3	-1	coords= -1044 -40 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	723	5	3	-1	0
END_LINE
finishport

3	-1	coords= -1044 -20 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

num_demux
_
824	0	0	0
_
_
This is a 8-output real number demultiplexer.
NumDemultiplexReal
0
_
-680	-350	-630	-218
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	nDX
trans	-1	0	0	0
offset	0	0
hash	0xFA38
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -681 -315 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Output 1	1	EndOfFunc
BEGIN_LINE
1	723	0	0	-1	0
-718	-295
-718	-315
END_LINE
outputport

1	2	coords= -630 -303 0	1	0	1	mandatory= 0	0	1
n1

funcName= Run_Output 1	2	EndOfFunc
BEGIN_LINE
0	825	6	1	-1	0
END_LINE
outputport

1	2	coords= -630 -293 0	0	0	1	mandatory= 0	0	1
n2

funcName= Run_Output 1	3	EndOfFunc
BEGIN_LINE
0	825	7	1	-1	0
END_LINE
outputport

1	2	coords= -630 -283 0	2	0	1	mandatory= 0	0	1
n3

funcName= Run_Output 1	4	EndOfFunc
BEGIN_LINE
0	825	0	1	-1	0
END_LINE
outputport

1	2	coords= -630 -273 0	3	0	1	mandatory= 0	0	1
n4

funcName= Run_Output 1	5	EndOfFunc
BEGIN_LINE
0	825	5	1	-1	0
END_LINE
outputport

1	2	coords= -630 -263 0	4	0	1	mandatory= 0	0	1
n5

funcName= Run_Output 1	6	EndOfFunc
BEGIN_LINE
0	825	1	1	-1	0
END_LINE
outputport

1	2	coords= -630 -253 0	5	0	1	mandatory= 0	0	1
n6

funcName= Run_Output 1	7	EndOfFunc
BEGIN_LINE
0	825	2	1	-1	0
END_LINE
outputport

1	2	coords= -630 -238 0	6	0	1	mandatory= 0	0	1
d1

funcName= Run_Output 1	8	EndOfFunc
BEGIN_LINE
0	825	3	1	-1	0
END_LINE
outputport

1	2	coords= -630 -228 0	7	0	1	mandatory= 0	0	1
d2

funcName= Run_Output 1	9	EndOfFunc
BEGIN_LINE
0	825	4	1	-1	0
END_LINE
outputport

1	0	coords= -630 -315 0	8	0	1	mandatory= 0	0	1
-ve

funcName= Run_Output 1	10	EndOfFunc
BEGIN_LINE
0	825	8	1	-1	0
END_LINE
startport

2	-1	coords= -681 -325 0	0	0	0	mandatory= 0	0	1
i

funcName= Run_Output 1	0	EndOfFunc
BEGIN_LINE
1	723	0	2	-1	0
-718	-305
-718	-325
END_LINE
finishport

3	-1	coords= -630 -325 0	0	0	1	mandatory= 0	0	1
o

funcName= Run_Output 1	11	EndOfFunc
BEGIN_LINE
0	825	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

num_mux
_
825	0	0	0
_
_
This is a 8-input real multiplexer.
NumMultiplexReal
0
_
-555	-350	-505	-218
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	nMX
trans	-1	0	0	0
offset	0	0
hash	0xDAA7
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -556 -283 0	0	0	1	mandatory= 0	0	1
n3

funcName= Run_Input 1	1	EndOfFunc
BEGIN_LINE
1	824	2	0	-1	1
-588	-283
-578	-283
END_LINE
inputport

0	2	coords= -556 -263 0	1	0	1	mandatory= 0	0	1
n5

funcName= Run_Input 1	2	EndOfFunc
BEGIN_LINE
1	824	4	0	-1	1
-588	-263
-578	-263
END_LINE
inputport

0	2	coords= -556 -253 0	2	0	1	mandatory= 0	0	1
n6

funcName= Run_Input 1	3	EndOfFunc
BEGIN_LINE
1	824	5	0	-1	1
-588	-253
-578	-253
END_LINE
inputport

0	2	coords= -556 -238 0	3	0	1	mandatory= 0	0	1
d1

funcName= Run_Input 1	4	EndOfFunc
BEGIN_LINE
1	824	6	0	-1	1
-588	-238
-578	-238
END_LINE
inputport

0	2	coords= -556 -228 0	4	0	1	mandatory= 0	0	1
d2

funcName= Run_Input 1	5	EndOfFunc
BEGIN_LINE
1	824	7	0	-1	1
-588	-228
-578	-228
END_LINE
inputport

0	2	coords= -556 -273 0	5	0	1	mandatory= 0	0	1
n4

funcName= Run_Input 1	6	EndOfFunc
BEGIN_LINE
1	824	3	0	-1	1
-588	-273
-578	-273
END_LINE
inputport

0	2	coords= -556 -303 0	6	0	1	mandatory= 0	0	1
n1

funcName= Run_Input 1	7	EndOfFunc
BEGIN_LINE
1	824	0	0	-1	1
-588	-303
-578	-303
END_LINE
inputport

0	2	coords= -556 -293 0	7	0	1	mandatory= 0	0	1
n2

funcName= Run_Input 1	8	EndOfFunc
BEGIN_LINE
1	824	1	0	-1	1
-588	-293
-578	-293
END_LINE
inputport

0	0	coords= -556 -315 0	8	0	1	mandatory= 0	0	1
-ve

funcName= Run_Input 1	9	EndOfFunc
BEGIN_LINE
1	824	8	0	-1	1
-588	-315
-578	-315
END_LINE
outputport

1	2	coords= -505 -315 0	0	0	1	mandatory= 0	0	1
o1

funcName= Run_Input 1	10	EndOfFunc
BEGIN_LINE
0	724	0	1	-1	0
END_LINE
startport

2	2	coords= -556 -325 0	0	0	0	mandatory= 0	0	1
i

funcName= Run_Input 1	0	EndOfFunc
BEGIN_LINE
1	824	0	2	-1	1
-588	-325
-578	-325
END_LINE
finishport

3	2	coords= -505 -325 0	0	0	1	mandatory= 0	0	1
o

funcName= Run_Input 1	11	EndOfFunc
BEGIN_LINE
0	724	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
858	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1055	10	-1035	60
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

2	-1	coords= -1056 20 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	858	1	2	-1	0
-999	50
-999	-10
-1095	-10
-1095	20
END_LINE
startport

2	-1	coords= -1056 35 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-15
-1111	-15
-1111	35
END_LINE
startport

2	1	coords= -1056 50 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	823	0	2	-1	0
-995	-40
-995	-80
-1110	-80
-1110	50
END_LINE
finishport

3	-1	coords= -1049 30 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	723	6	3	-1	0
END_LINE
finishport

3	-1	coords= -1049 50 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
892	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-1055	80	-1035	130
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

2	-1	coords= -1056 90 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	892	1	2	-1	0
-999	120
-999	60
-1095	60
-1095	90
END_LINE
startport

2	-1	coords= -1056 105 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	55
-1111	55
-1111	105
END_LINE
startport

2	1	coords= -1056 120 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	858	0	2	-1	0
-1000	30
-1000	-10
-1105	-10
-1105	120
END_LINE
finishport

3	-1	coords= -1049 100 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	720	2	3	-1	0
END_LINE
finishport

3	-1	coords= -1049 120 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
