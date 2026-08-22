#V:2.1.2

CanvasSizeXY
2375	1180

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
1	159	0	0	-1	0
185	-595
185	-720
-381	-720
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
write_res
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	159	0	2	-1	0
180	-605
180	-715
-376	-715
-376	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	445	1	3	-1	0
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
0	445	1	3	-1	0
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
-710	-655
-710	-610
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
1	334	0	2	-1	0
1401	-370
1401	-10
-726	-10
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

demux_8i
_
51	0	0	0
_
_
This is a 8-output integer demultiplexer.
DemultiplexEightOutputInt
0
_
-590	-305	-560	-85
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0xA1B0
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -591 -160 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	funcName= Run_Output5 5	1	funcName= Run_Output6 6	1	funcName= Run_Output7 7	1	funcName= Run_Output8 8	1	EndOfFunc
BEGIN_LINE
1	59	0	0	-1	0
-633	165
-633	-160
END_LINE
outputport

1	1	coords= -560 -270 0	0	0	1	mandatory= 0	0	1
o1

funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	56	0	1	-1	0
END_LINE
outputport

1	1	coords= -560 -245 0	1	0	1	mandatory= 0	0	1
o2

funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	56	1	1	-1	0
END_LINE
outputport

1	1	coords= -560 -220 0	2	0	1	mandatory= 0	0	1
o3

funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	56	2	1	-1	0
END_LINE
outputport

1	1	coords= -560 -195 0	3	0	1	mandatory= 0	0	1
o4

funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	56	3	1	-1	0
END_LINE
outputport

1	1	coords= -560 -170 0	4	0	1	mandatory= 0	0	1
o5

funcName= Run_Output5 5	2	EndOfFunc
BEGIN_LINE
0	56	4	1	-1	0
END_LINE
outputport

1	1	coords= -560 -145 0	5	0	1	mandatory= 0	0	1
o6

funcName= Run_Output6 6	2	EndOfFunc
BEGIN_LINE
0	56	5	1	-1	0
END_LINE
outputport

1	1	coords= -560 -120 0	6	0	1	mandatory= 0	0	1
o7

funcName= Run_Output7 7	2	EndOfFunc
BEGIN_LINE
0	56	6	1	-1	0
END_LINE
outputport

1	1	coords= -560 -95 0	7	0	1	mandatory= 0	0	1
o8

funcName= Run_Output8 8	2	EndOfFunc
BEGIN_LINE
0	56	7	1	-1	0
END_LINE
startport

2	1	coords= -591 -280 0	0	0	0	mandatory= 0	0	1
s1
test_start
funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-330
-646	-330
-646	-280
END_LINE
startport

2	-1	coords= -591 -265 0	2	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	61	0	2	-1	0
-645	-370
-645	-265
END_LINE
startport

2	-1	coords= -591 -250 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	62	0	2	-1	0
-653	-300
-653	-250
END_LINE
startport

2	-1	coords= -591 -235 0	2	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	63	0	2	-1	0
-645	-230
-635	-235
END_LINE
startport

2	-1	coords= -591 -220 0	2	0	0	mandatory= 0	0	1
s5

funcName= Run_Output5 5	0	EndOfFunc
BEGIN_LINE
1	64	0	2	-1	0
-645	-165
-645	-220
END_LINE
startport

2	-1	coords= -591 -205 0	2	0	0	mandatory= 0	0	1
s6

funcName= Run_Output6 6	0	EndOfFunc
BEGIN_LINE
1	65	0	2	-1	0
-645	-95
-645	-205
END_LINE
startport

2	-1	coords= -591 -190 0	2	0	0	mandatory= 0	0	1
s7

funcName= Run_Output7 7	0	EndOfFunc
BEGIN_LINE
1	95	0	2	-1	0
-645	-20
-645	-190
END_LINE
startport

2	-1	coords= -591 -175 0	2	0	0	mandatory= 0	0	1
s8

funcName= Run_Output8 8	0	EndOfFunc
BEGIN_LINE
1	126	0	2	-1	0
-645	55
-645	-175
END_LINE
finishport

3	1	coords= -560 -280 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	51	1	3	-1	0
END_LINE
finishport

3	1	coords= -560 -255 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	51	2	3	-1	0
END_LINE
finishport

3	1	coords= -560 -230 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	51	3	3	-1	0
END_LINE
finishport

3	1	coords= -560 -205 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
finishport

3	1	coords= -560 -180 0	4	0	1	mandatory= 0	0	1


funcName= Run_Output5 5	3	EndOfFunc
BEGIN_LINE
0	51	5	3	-1	0
END_LINE
finishport

3	1	coords= -560 -155 0	5	0	1	mandatory= 0	0	1


funcName= Run_Output6 6	3	EndOfFunc
BEGIN_LINE
0	51	6	3	-1	0
END_LINE
finishport

3	1	coords= -560 -130 0	6	0	1	mandatory= 0	0	1


funcName= Run_Output7 7	3	EndOfFunc
BEGIN_LINE
0	51	7	3	-1	0
END_LINE
finishport

3	1	coords= -560 -105 0	7	0	1	mandatory= 0	0	1

write_int_done
funcName= Run_Output8 8	3	EndOfFunc
BEGIN_LINE
0	160	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_8r
_
52	0	0	0
_
_
This is a 8-output real demultiplexer.
DemultiplexEightOutputReal
0
_
-15	-285	15	-65
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x2FD9
Instance_Info	_
End_Instance
END_BA
inputport

0	2	coords= -16 -140 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	funcName= Run_Output5 5	1	funcName= Run_Output6 6	1	funcName= Run_Output7 7	1	funcName= Run_Output8 8	1	EndOfFunc
BEGIN_LINE
1	162	0	0	-1	0
-46	200
-46	-140
END_LINE
outputport

1	2	coords= 15 -250 0	0	0	1	mandatory= 0	0	1
o1

funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	55	0	1	-1	0
END_LINE
outputport

1	2	coords= 15 -225 0	1	0	1	mandatory= 0	0	1
o2

funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	55	1	1	-1	0
END_LINE
outputport

1	2	coords= 15 -200 0	2	0	1	mandatory= 0	0	1
o3

funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	55	2	1	-1	0
END_LINE
outputport

1	2	coords= 15 -175 0	3	0	1	mandatory= 0	0	1
o4

funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	55	3	1	-1	0
END_LINE
outputport

1	2	coords= 15 -150 0	4	0	1	mandatory= 0	0	1
o5

funcName= Run_Output5 5	2	EndOfFunc
BEGIN_LINE
0	55	4	1	-1	0
END_LINE
outputport

1	2	coords= 15 -125 0	5	0	1	mandatory= 0	0	1
o6

funcName= Run_Output6 6	2	EndOfFunc
BEGIN_LINE
0	55	5	1	-1	0
END_LINE
outputport

1	2	coords= 15 -100 0	6	0	1	mandatory= 0	0	1
o7

funcName= Run_Output7 7	2	EndOfFunc
BEGIN_LINE
0	55	6	1	-1	0
END_LINE
outputport

1	2	coords= 15 -75 0	7	0	1	mandatory= 0	0	1
o8

funcName= Run_Output8 8	2	EndOfFunc
BEGIN_LINE
0	55	7	1	-1	0
END_LINE
startport

2	2	coords= -16 -260 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	160	0	2	-1	0
-125	-370
-125	-260
END_LINE
startport

2	2	coords= -16 -245 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	236	0	2	-1	0
-125	-325
-125	-245
END_LINE
startport

2	2	coords= -16 -230 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	237	0	2	-1	0
-125	-255
-125	-230
END_LINE
startport

2	2	coords= -16 -215 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	238	0	2	-1	0
-118	-185
-118	-215
END_LINE
startport

2	2	coords= -16 -200 0	4	0	0	mandatory= 0	0	1
s5

funcName= Run_Output5 5	0	EndOfFunc
BEGIN_LINE
1	239	0	2	-1	0
-118	-120
-118	-200
END_LINE
startport

2	2	coords= -16 -185 0	5	0	0	mandatory= 0	0	1
s6

funcName= Run_Output6 6	0	EndOfFunc
BEGIN_LINE
1	240	0	2	-1	0
-118	-50
-118	-185
END_LINE
startport

2	2	coords= -16 -170 0	6	0	0	mandatory= 0	0	1
s7

funcName= Run_Output7 7	0	EndOfFunc
BEGIN_LINE
1	241	0	2	-1	0
-118	25
-118	-170
END_LINE
startport

2	2	coords= -16 -155 0	7	0	0	mandatory= 0	0	1
s8

funcName= Run_Output8 8	0	EndOfFunc
BEGIN_LINE
1	242	0	2	-1	0
-118	100
-118	-155
END_LINE
finishport

3	2	coords= 15 -260 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	236	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -235 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	237	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -210 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	238	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -185 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	239	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -160 0	4	0	1	mandatory= 0	0	1


funcName= Run_Output5 5	3	EndOfFunc
BEGIN_LINE
0	240	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -135 0	5	0	1	mandatory= 0	0	1


funcName= Run_Output6 6	3	EndOfFunc
BEGIN_LINE
0	241	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -110 0	6	0	1	mandatory= 0	0	1


funcName= Run_Output7 7	3	EndOfFunc
BEGIN_LINE
0	242	2	3	-1	0
END_LINE
finishport

3	2	coords= 15 -85 0	7	0	1	mandatory= 0	0	1

write_real_done
funcName= Run_Output8 8	3	EndOfFunc
BEGIN_LINE
0	287	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_8s
_
53	0	0	0
_
_
This is a 8-output string demultiplexer.
DemultiplexEightOutputString
0
_
535	-255	565	-35
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x919E
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 534 -110 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	funcName= Run_Output5 5	1	funcName= Run_Output6 6	1	funcName= Run_Output7 7	1	funcName= Run_Output8 8	1	EndOfFunc
BEGIN_LINE
1	288	0	0	-1	0
490	201
490	-110
END_LINE
outputport

1	3	coords= 565 -220 0	0	0	1	mandatory= 0	0	1
o1

funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	57	0	1	-1	0
END_LINE
outputport

1	3	coords= 565 -195 0	1	0	1	mandatory= 0	0	1
o2

funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	57	1	1	-1	0
END_LINE
outputport

1	3	coords= 565 -170 0	2	0	1	mandatory= 0	0	1
o3

funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	57	2	1	-1	0
END_LINE
outputport

1	3	coords= 565 -145 0	3	0	1	mandatory= 0	0	1
o4

funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	57	3	1	-1	0
END_LINE
outputport

1	3	coords= 565 -120 0	4	0	1	mandatory= 0	0	1
o5

funcName= Run_Output5 5	2	EndOfFunc
BEGIN_LINE
0	57	4	1	-1	0
END_LINE
outputport

1	3	coords= 565 -95 0	5	0	1	mandatory= 0	0	1
o6

funcName= Run_Output6 6	2	EndOfFunc
BEGIN_LINE
0	57	5	1	-1	0
END_LINE
outputport

1	3	coords= 565 -70 0	6	0	1	mandatory= 0	0	1
o7

funcName= Run_Output7 7	2	EndOfFunc
BEGIN_LINE
0	57	6	1	-1	0
END_LINE
outputport

1	3	coords= 565 -45 0	7	0	1	mandatory= 0	0	1
o8

funcName= Run_Output8 8	2	EndOfFunc
BEGIN_LINE
0	57	7	1	-1	0
END_LINE
startport

2	3	coords= 534 -230 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	287	0	2	-1	0
490	-415
490	-230
END_LINE
startport

2	3	coords= 534 -215 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	383	0	2	-1	0
477	-315
477	-215
END_LINE
startport

2	3	coords= 534 -200 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	384	0	2	-1	0
477	-245
477	-200
END_LINE
startport

2	3	coords= 534 -185 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	385	0	2	-1	0
467	-175
477	-185
END_LINE
startport

2	3	coords= 534 -170 0	4	0	0	mandatory= 0	0	1
s5

funcName= Run_Output5 5	0	EndOfFunc
BEGIN_LINE
1	386	0	2	-1	0
467	-110
467	-170
END_LINE
startport

2	3	coords= 534 -155 0	5	0	0	mandatory= 0	0	1
s6

funcName= Run_Output6 6	0	EndOfFunc
BEGIN_LINE
1	387	0	2	-1	0
467	-40
467	-155
END_LINE
startport

2	3	coords= 534 -140 0	6	0	0	mandatory= 0	0	1
s7

funcName= Run_Output7 7	0	EndOfFunc
BEGIN_LINE
1	388	0	2	-1	0
467	35
467	-140
END_LINE
startport

2	3	coords= 534 -125 0	7	0	0	mandatory= 0	0	1
s8

funcName= Run_Output8 8	0	EndOfFunc
BEGIN_LINE
1	389	0	2	-1	0
467	110
467	-125
END_LINE
finishport

3	3	coords= 565 -230 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	383	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -205 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	384	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -180 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	385	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -155 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	386	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -130 0	4	0	1	mandatory= 0	0	1


funcName= Run_Output5 5	3	EndOfFunc
BEGIN_LINE
0	387	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -105 0	5	0	1	mandatory= 0	0	1


funcName= Run_Output6 6	3	EndOfFunc
BEGIN_LINE
0	388	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -80 0	6	0	1	mandatory= 0	0	1


funcName= Run_Output7 7	3	EndOfFunc
BEGIN_LINE
0	389	2	3	-1	0
END_LINE
finishport

3	3	coords= 565 -55 0	7	0	1	mandatory= 0	0	1

write_str_done
funcName= Run_Output8 8	3	EndOfFunc
BEGIN_LINE
0	197	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

demux_8b
_
54	0	0	0
_
_
This is a 8-output boolean demultiplexer.
DemultiplexEightOutputBool
0
_
1050	-250	1080	-30
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	DMX
trans	-1	0	0	0
offset	0	0
hash	0x8DF9
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= 1049 -105 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	1	funcName= Run_Output2 2	1	funcName= Run_Output3 3	1	funcName= Run_Output4 4	1	funcName= Run_Output5 5	1	funcName= Run_Output6 6	1	funcName= Run_Output7 7	1	funcName= Run_Output8 8	1	EndOfFunc
BEGIN_LINE
1	452	0	0	-1	0
992	215
992	-105
END_LINE
outputport

1	0	coords= 1080 -215 0	0	0	1	mandatory= 0	0	1
o1

funcName= Run_Output1 1	2	EndOfFunc
BEGIN_LINE
0	58	0	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -190 0	1	0	1	mandatory= 0	0	1
o2

funcName= Run_Output2 2	2	EndOfFunc
BEGIN_LINE
0	58	1	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -165 0	2	0	1	mandatory= 0	0	1
o3

funcName= Run_Output3 3	2	EndOfFunc
BEGIN_LINE
0	58	2	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -140 0	3	0	1	mandatory= 0	0	1
o4

funcName= Run_Output4 4	2	EndOfFunc
BEGIN_LINE
0	58	3	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -115 0	4	0	1	mandatory= 0	0	1
o5

funcName= Run_Output5 5	2	EndOfFunc
BEGIN_LINE
0	58	4	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -90 0	5	0	1	mandatory= 0	0	1
o6

funcName= Run_Output6 6	2	EndOfFunc
BEGIN_LINE
0	58	5	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -65 0	6	0	1	mandatory= 0	0	1
o7

funcName= Run_Output7 7	2	EndOfFunc
BEGIN_LINE
0	58	6	1	-1	0
END_LINE
outputport

1	0	coords= 1080 -40 0	7	0	1	mandatory= 0	0	1
o8

funcName= Run_Output8 8	2	EndOfFunc
BEGIN_LINE
0	58	7	1	-1	0
END_LINE
startport

2	0	coords= 1049 -225 0	0	0	0	mandatory= 0	0	1
s1

funcName= Run_Output1 1	0	EndOfFunc
BEGIN_LINE
1	197	0	2	-1	0
960	-375
960	-225
END_LINE
startport

2	0	coords= 1049 -210 0	1	0	0	mandatory= 0	0	1
s2

funcName= Run_Output2 2	0	EndOfFunc
BEGIN_LINE
1	445	0	2	-1	0
960	-305
960	-210
END_LINE
startport

2	0	coords= 1049 -195 0	2	0	0	mandatory= 0	0	1
s3

funcName= Run_Output3 3	0	EndOfFunc
BEGIN_LINE
1	446	0	2	-1	0
960	-235
960	-195
END_LINE
startport

2	0	coords= 1049 -180 0	3	0	0	mandatory= 0	0	1
s4

funcName= Run_Output4 4	0	EndOfFunc
BEGIN_LINE
1	447	0	2	-1	0
975	-165
985	-180
END_LINE
startport

2	0	coords= 1049 -165 0	4	0	0	mandatory= 0	0	1
s5

funcName= Run_Output5 5	0	EndOfFunc
BEGIN_LINE
1	448	0	2	-1	0
975	-100
975	-165
END_LINE
startport

2	0	coords= 1049 -150 0	5	0	0	mandatory= 0	0	1
s6

funcName= Run_Output6 6	0	EndOfFunc
BEGIN_LINE
1	449	0	2	-1	0
975	-30
975	-150
END_LINE
startport

2	0	coords= 1049 -135 0	6	0	0	mandatory= 0	0	1
s7

funcName= Run_Output7 7	0	EndOfFunc
BEGIN_LINE
1	450	0	2	-1	0
975	45
975	-135
END_LINE
startport

2	0	coords= 1049 -120 0	7	0	0	mandatory= 0	0	1
s8

funcName= Run_Output8 8	0	EndOfFunc
BEGIN_LINE
1	451	0	2	-1	0
975	120
975	-120
END_LINE
finishport

3	0	coords= 1080 -225 0	0	0	1	mandatory= 0	0	1


funcName= Run_Output1 1	3	EndOfFunc
BEGIN_LINE
0	445	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -200 0	1	0	1	mandatory= 0	0	1


funcName= Run_Output2 2	3	EndOfFunc
BEGIN_LINE
0	446	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -175 0	2	0	1	mandatory= 0	0	1


funcName= Run_Output3 3	3	EndOfFunc
BEGIN_LINE
0	447	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -150 0	3	0	1	mandatory= 0	0	1


funcName= Run_Output4 4	3	EndOfFunc
BEGIN_LINE
0	448	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -125 0	4	0	1	mandatory= 0	0	1


funcName= Run_Output5 5	3	EndOfFunc
BEGIN_LINE
0	449	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -100 0	5	0	1	mandatory= 0	0	1


funcName= Run_Output6 6	3	EndOfFunc
BEGIN_LINE
0	450	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -75 0	6	0	1	mandatory= 0	0	1


funcName= Run_Output7 7	3	EndOfFunc
BEGIN_LINE
0	451	2	3	-1	0
END_LINE
finishport

3	0	coords= 1080 -50 0	7	0	1	mandatory= 0	0	1

write_bool_done
funcName= Run_Output8 8	3	EndOfFunc
BEGIN_LINE
0	334	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8r
_
55	0	0	0
_
_
This is a 8-input real multiplexer.
MultiplexEightInputReal
0
_
125	-285	155	-65
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

0	2	coords= 124 -250 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	52	0	0	-1	0
69	-250
79	-250
END_LINE
inputport

0	2	coords= 124 -225 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	52	1	0	-1	0
74	-225
84	-225
END_LINE
inputport

0	2	coords= 124 -200 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	52	2	0	-1	0
79	-200
89	-200
END_LINE
inputport

0	2	coords= 124 -175 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	52	3	0	-1	0
84	-175
94	-175
END_LINE
inputport

0	2	coords= 124 -150 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	52	4	0	-1	0
89	-150
99	-150
END_LINE
inputport

0	2	coords= 124 -125 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	52	5	0	-1	0
94	-125
104	-125
END_LINE
inputport

0	2	coords= 124 -100 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	52	6	0	-1	0
99	-100
109	-100
END_LINE
inputport

0	2	coords= 124 -75 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	52	7	0	-1	0
104	-75
114	-75
END_LINE
outputport

1	2	coords= 155 -250 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	198	0	1	-1	0
END_LINE
startport

2	2	coords= 124 -260 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	52	0	2	-1	0
109	-260
119	-260
END_LINE
startport

2	2	coords= 124 -235 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	52	1	2	-1	0
114	-235
124	-235
END_LINE
startport

2	2	coords= 124 -210 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	52	2	2	-1	0
69	-210
79	-210
END_LINE
startport

2	2	coords= 124 -185 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	52	3	2	-1	0
69	-185
79	-185
END_LINE
startport

2	2	coords= 124 -160 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	52	4	2	-1	0
114	-160
124	-160
END_LINE
startport

2	2	coords= 124 -135 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	52	5	2	-1	0
114	-135
124	-135
END_LINE
startport

2	2	coords= 124 -110 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	52	6	2	-1	0
114	-110
124	-110
END_LINE
startport

2	2	coords= 124 -85 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	52	7	2	-1	0
114	-85
124	-85
END_LINE
finishport

3	2	coords= 155 -260 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	198	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8i
_
56	0	0	0
_
_
This is a 8-input integer multiplexer.
MultiplexEightInputInt
0
_
-480	-305	-450	-85
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x0828
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -481 -270 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	51	0	0	-1	0
-516	-270
-506	-270
END_LINE
inputport

0	1	coords= -481 -245 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	51	1	0	-1	0
-516	-245
-506	-245
END_LINE
inputport

0	1	coords= -481 -220 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	51	2	0	-1	0
-516	-220
-506	-220
END_LINE
inputport

0	1	coords= -481 -195 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	51	3	0	-1	0
-516	-195
-506	-195
END_LINE
inputport

0	1	coords= -481 -170 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	51	4	0	-1	0
-516	-170
-506	-170
END_LINE
inputport

0	1	coords= -481 -145 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	51	5	0	-1	0
-516	-145
-506	-145
END_LINE
inputport

0	1	coords= -481 -120 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	51	6	0	-1	0
-516	-120
-506	-120
END_LINE
inputport

0	1	coords= -481 -95 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	51	7	0	-1	0
-516	-95
-506	-95
END_LINE
outputport

1	1	coords= -450 -270 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	60	0	1	-1	0
END_LINE
startport

2	1	coords= -481 -280 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	51	0	2	-1	0
-516	-280
-506	-280
END_LINE
startport

2	1	coords= -481 -255 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	51	1	2	-1	0
-516	-255
-506	-255
END_LINE
startport

2	1	coords= -481 -230 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	51	2	2	-1	0
-516	-230
-506	-230
END_LINE
startport

2	1	coords= -481 -205 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	51	3	2	-1	0
-516	-205
-506	-205
END_LINE
startport

2	1	coords= -481 -180 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	51	4	2	-1	0
-516	-180
-506	-180
END_LINE
startport

2	1	coords= -481 -155 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	51	5	2	-1	0
-516	-155
-506	-155
END_LINE
startport

2	1	coords= -481 -130 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	51	6	2	-1	0
-516	-130
-506	-130
END_LINE
startport

2	1	coords= -481 -105 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	51	7	2	-1	0
-516	-105
-506	-105
END_LINE
finishport

3	1	coords= -450 -280 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	60	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8s
_
57	0	0	0
_
_
This is a 8-input string multiplexer.
MultiplexEightInputString
0
_
695	-255	725	-35
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

0	3	coords= 694 -220 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	53	0	0	-1	0
629	-220
639	-220
END_LINE
inputport

0	3	coords= 694 -195 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	53	1	0	-1	0
634	-195
644	-195
END_LINE
inputport

0	3	coords= 694 -170 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	53	2	0	-1	0
639	-170
649	-170
END_LINE
inputport

0	3	coords= 694 -145 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	53	3	0	-1	0
644	-145
654	-145
END_LINE
inputport

0	3	coords= 694 -120 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	53	4	0	-1	0
649	-120
659	-120
END_LINE
inputport

0	3	coords= 694 -95 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	53	5	0	-1	0
654	-95
664	-95
END_LINE
inputport

0	3	coords= 694 -70 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	53	6	0	-1	0
659	-70
669	-70
END_LINE
inputport

0	3	coords= 694 -45 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	53	7	0	-1	0
664	-45
674	-45
END_LINE
outputport

1	3	coords= 725 -220 0	0	0	1	mandatory= 0	0	1

write_str_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	159	2	1	-1	0
END_LINE
startport

2	3	coords= 694 -230 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	53	0	2	-1	0
674	-230
684	-230
END_LINE
startport

2	3	coords= 694 -205 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	53	1	2	-1	0
679	-205
689	-205
END_LINE
startport

2	3	coords= 694 -180 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	53	2	2	-1	0
684	-180
694	-180
END_LINE
startport

2	3	coords= 694 -155 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	53	3	2	-1	0
629	-155
639	-155
END_LINE
startport

2	3	coords= 694 -130 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	53	4	2	-1	0
629	-130
639	-130
END_LINE
startport

2	3	coords= 694 -105 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	53	5	2	-1	0
684	-105
694	-105
END_LINE
startport

2	3	coords= 694 -80 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	53	6	2	-1	0
684	-80
694	-80
END_LINE
startport

2	3	coords= 694 -55 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	53	7	2	-1	0
684	-55
694	-55
END_LINE
finishport

3	3	coords= 725 -230 0	0	0	1	mandatory= 0	0	1

write_str
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	159	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_8b
_
58	0	0	0
_
_
This is a 8-input boolean multiplexer.
MultiplexEightInputBool
0
_
1220	-250	1250	-30
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2751
Instance_Info	_
End_Instance
END_BA
inputport

0	0	coords= 1219 -215 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	54	0	0	-1	0
1144	-215
1154	-215
END_LINE
inputport

0	0	coords= 1219 -190 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	54	1	0	-1	0
1144	-190
1154	-190
END_LINE
inputport

0	0	coords= 1219 -165 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	54	2	0	-1	0
1144	-165
1154	-165
END_LINE
inputport

0	0	coords= 1219 -140 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	54	3	0	-1	0
1144	-140
1154	-140
END_LINE
inputport

0	0	coords= 1219 -115 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input5 5	1	EndOfFunc
BEGIN_LINE
1	54	4	0	-1	0
1144	-115
1154	-115
END_LINE
inputport

0	0	coords= 1219 -90 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input6 6	1	EndOfFunc
BEGIN_LINE
1	54	5	0	-1	0
1144	-90
1154	-90
END_LINE
inputport

0	0	coords= 1219 -65 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input7 7	1	EndOfFunc
BEGIN_LINE
1	54	6	0	-1	0
1144	-65
1154	-65
END_LINE
inputport

0	0	coords= 1219 -40 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input8 8	1	EndOfFunc
BEGIN_LINE
1	54	7	0	-1	0
1144	-40
1154	-40
END_LINE
outputport

1	0	coords= 1250 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	funcName= Run_Input5 5	2	funcName= Run_Input6 6	2	funcName= Run_Input7 7	2	funcName= Run_Input8 8	2	EndOfFunc
BEGIN_LINE
0	390	0	1	-1	0
END_LINE
startport

2	0	coords= 1219 -225 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
1149	-225
1159	-225
END_LINE
startport

2	0	coords= 1219 -200 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	54	1	2	-1	0
1144	-200
1154	-200
END_LINE
startport

2	0	coords= 1219 -175 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	54	2	2	-1	0
1144	-175
1154	-175
END_LINE
startport

2	0	coords= 1219 -150 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	54	3	2	-1	0
1144	-150
1154	-150
END_LINE
startport

2	0	coords= 1219 -125 0	4	0	0	mandatory= 0	0	1


funcName= Run_Input5 5	0	EndOfFunc
BEGIN_LINE
1	54	4	2	-1	0
1144	-125
1154	-125
END_LINE
startport

2	0	coords= 1219 -100 0	5	0	0	mandatory= 0	0	1


funcName= Run_Input6 6	0	EndOfFunc
BEGIN_LINE
1	54	5	2	-1	0
1144	-100
1154	-100
END_LINE
startport

2	0	coords= 1219 -75 0	6	0	0	mandatory= 0	0	1


funcName= Run_Input7 7	0	EndOfFunc
BEGIN_LINE
1	54	6	2	-1	0
1144	-75
1154	-75
END_LINE
startport

2	0	coords= 1219 -50 0	7	0	0	mandatory= 0	0	1


funcName= Run_Input8 8	0	EndOfFunc
BEGIN_LINE
1	54	7	2	-1	0
1144	-50
1154	-50
END_LINE
finishport

3	0	coords= 1250 -225 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	funcName= Run_Input5 5	3	funcName= Run_Input6 6	3	funcName= Run_Input7 7	3	funcName= Run_Input8 8	3	EndOfFunc
BEGIN_LINE
0	390	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
59	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-785	155	-690	175
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	5
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -690 165 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranisx
_
60	0	0	0
_
_
This is an integer to string converter.
ConvertorIntToString
0
_
-400	-290	-370	-260
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

0	1	coords= -401 -270 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	1	EndOfFunc
BEGIN_LINE
1	56	0	0	-1	0
-421	-270
-411	-270
END_LINE
outputport

1	3	coords= -370 -270 0	0	0	1	mandatory= 0	0	1

write_int_str
funcName= Run_ConvertorIntToString 1	2	EndOfFunc
BEGIN_LINE
0	159	0	1	-1	0
END_LINE
startport

2	3	coords= -401 -280 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorIntToString 1	0	EndOfFunc
BEGIN_LINE
1	56	0	2	-1	0
-421	-280
-411	-280
END_LINE
finishport

3	3	coords= -370 -280 0	0	0	1	mandatory= 0	0	1

write_int
funcName= Run_ConvertorIntToString 1	3	EndOfFunc
BEGIN_LINE
0	159	0	3	-1	0
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
-715	-390	-695	-340
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

2	-1	coords= -716 -380 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	61	1	2	-1	0
-685	-350
-685	-410
-740	-410
-740	-380
END_LINE
startport

2	-1	coords= -716 -365 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-415
-771	-415
-771	-365
END_LINE
startport

2	1	coords= -716 -350 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	0	2	-1	0
-515	-280
-515	-410
-760	-410
-760	-350
END_LINE
finishport

3	-1	coords= -709 -370 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	1	3	-1	0
END_LINE
finishport

3	-1	coords= -709 -350 0	1	0	1	mandatory= 0	0	1


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
-715	-320	-695	-270
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

2	-1	coords= -716 -310 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	62	1	2	-1	0
-685	-280
-685	-340
-745	-340
-745	-310
END_LINE
startport

2	-1	coords= -716 -295 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-345
-771	-345
-771	-295
END_LINE
startport

2	1	coords= -716 -280 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	1	2	-1	0
-515	-255
-515	-410
-760	-410
-760	-280
END_LINE
finishport

3	-1	coords= -709 -300 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	2	3	-1	0
END_LINE
finishport

3	-1	coords= -709 -280 0	1	0	1	mandatory= 0	0	1


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
-715	-250	-695	-200
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

2	-1	coords= -716 -240 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	63	1	2	-1	0
-690	-210
-690	-265
-745	-265
-745	-240
END_LINE
startport

2	-1	coords= -716 -225 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-275
-771	-275
-771	-225
END_LINE
startport

2	1	coords= -716 -210 0	3	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	2	2	-1	0
-515	-230
-515	-410
-760	-410
-760	-210
END_LINE
finishport

3	-1	coords= -709 -230 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	3	3	-1	0
END_LINE
finishport

3	-1	coords= -709 -210 0	1	0	1	mandatory= 0	0	1


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
-715	-185	-695	-135
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

2	-1	coords= -716 -175 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	64	1	2	-1	0
-690	-145
-690	-200
-745	-200
-745	-175
END_LINE
startport

2	-1	coords= -716 -160 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-210
-771	-210
-771	-160
END_LINE
startport

2	1	coords= -716 -145 0	4	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	3	2	-1	0
-515	-205
-515	-410
-760	-410
-760	-145
END_LINE
finishport

3	-1	coords= -709 -165 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
finishport

3	-1	coords= -709 -145 0	1	0	1	mandatory= 0	0	1


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
-715	-115	-695	-65
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

2	-1	coords= -716 -105 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	65	1	2	-1	0
-690	-75
-690	-130
-745	-130
-745	-105
END_LINE
startport

2	-1	coords= -716 -90 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-140
-771	-140
-771	-90
END_LINE
startport

2	1	coords= -716 -75 0	5	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	4	2	-1	0
-515	-180
-515	-410
-760	-410
-760	-75
END_LINE
finishport

3	-1	coords= -709 -95 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	5	3	-1	0
END_LINE
finishport

3	-1	coords= -709 -75 0	1	0	1	mandatory= 0	0	1


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
-715	-40	-695	10
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

2	-1	coords= -716 -30 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	95	1	2	-1	0
-690	0
-690	-55
-745	-55
-745	-30
END_LINE
startport

2	-1	coords= -716 -15 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-65
-771	-65
-771	-15
END_LINE
startport

2	1	coords= -716 0 0	6	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	5	2	-1	0
-515	-155
-515	-410
-760	-410
-760	0
END_LINE
finishport

3	-1	coords= -709 -20 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	6	3	-1	0
END_LINE
finishport

3	-1	coords= -709 0 0	1	0	1	mandatory= 0	0	1


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
-715	35	-695	85
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

2	-1	coords= -716 45 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	126	1	2	-1	0
-690	75
-690	20
-745	20
-745	45
END_LINE
startport

2	-1	coords= -716 60 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	10
-771	10
-771	60
END_LINE
startport

2	1	coords= -716 75 0	7	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	6	2	-1	0
-515	-130
-515	-410
-760	-410
-760	75
END_LINE
finishport

3	-1	coords= -709 55 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	51	7	3	-1	0
END_LINE
finishport

3	-1	coords= -709 75 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
159	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
105	-630	135	-510
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

0	3	coords= 104 -595 0	0	0	1	mandatory= 0	0	1
i1
write_int_str
funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	60	0	0	-1	0
-133	-270
-133	-595
END_LINE
inputport

0	3	coords= 104 -570 0	1	0	1	mandatory= 0	0	1
i2
write_real_str
funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	198	0	0	-1	0
280	-250
280	-305
59	-305
59	-570
END_LINE
inputport

0	3	coords= 104 -545 0	2	0	1	mandatory= 0	0	1
i3
write_str_str
funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	57	0	0	-1	0
780	-220
780	-290
59	-290
59	-545
END_LINE
inputport

0	3	coords= 104 -520 0	3	0	1	mandatory= 0	0	1
i4
write_bool_str
funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	390	0	0	-1	0
1435	-215
1435	-270
59	-270
59	-520
END_LINE
outputport

1	3	coords= 135 -595 0	0	0	1	mandatory= 0	0	1

write_res_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= 104 -605 0	0	0	0	mandatory= 0	0	1

write_int
funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	60	0	2	-1	0
-158	-280
-158	-605
END_LINE
startport

2	3	coords= 104 -580 0	1	0	0	mandatory= 0	0	1

write_real
funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	198	0	2	-1	0
305	-260
305	-310
84	-310
84	-580
END_LINE
startport

2	3	coords= 104 -555 0	2	0	0	mandatory= 0	0	1

write_str
funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	57	0	2	-1	0
780	-230
780	-290
59	-290
59	-555
END_LINE
startport

2	3	coords= 104 -530 0	3	0	0	mandatory= 0	0	1

write_bool
funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	390	0	2	-1	0
1435	-225
1435	-270
59	-270
59	-530
END_LINE
finishport

3	3	coords= 135 -605 0	0	0	1	mandatory= 0	0	1

write_res
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
160	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-340	-390	-320	-340
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

2	-1	coords= -341 -380 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	160	1	2	-1	0
-300	-350
-300	-410
-391	-410
-391	-380
END_LINE
startport

2	-1	coords= -341 -365 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-415
-396	-415
-396	-365
END_LINE
startport

2	-1	coords= -341 -350 0	2	0	0	mandatory= 0	0	1

write_int_done
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	51	7	2	-1	0
-498	-105
-498	-350
END_LINE
finishport

3	-1	coords= -334 -370 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	0	3	-1	0
END_LINE
finishport

3	-1	coords= -334 -350 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	160	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_r1
_
162	0	0	0
_
_
This is a real constant.
Constant_Real1
0
_
-180	190	-85	210
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

1	2	coords= -85 200 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantReal 1	1	EndOfFunc
BEGIN_LINE
0	52	0	1	-1	0
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
197	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
725	-395	745	-345
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

2	-1	coords= 724 -385 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	197	1	2	-1	0
781	-355
781	-415
674	-415
674	-385
END_LINE
startport

2	-1	coords= 724 -370 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
234	-615
234	-370
END_LINE
startport

2	-1	coords= 724 -355 0	2	0	0	mandatory= 0	0	1

write_str_done
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	7	2	-1	0
639	-55
639	-355
END_LINE
finishport

3	-1	coords= 731 -375 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	0	3	-1	0
END_LINE
finishport

3	-1	coords= 731 -355 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	160	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranrsx
_
198	0	0	0
_
_
This is a real to string converter.
ConvertorRealToString
0
_
195	-270	225	-240
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

0	2	coords= 194 -250 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	1	EndOfFunc
BEGIN_LINE
1	55	0	0	-1	0
174	-250
184	-250
END_LINE
outputport

1	3	coords= 225 -250 0	0	0	1	mandatory= 0	0	1

write_real_str
funcName= Run_ConvertorRealToString 1	2	EndOfFunc
BEGIN_LINE
0	159	1	1	-1	0
END_LINE
startport

2	3	coords= 194 -260 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorRealToString 1	0	EndOfFunc
BEGIN_LINE
1	55	0	2	-1	0
179	-260
189	-260
END_LINE
finishport

3	3	coords= 225 -260 0	0	0	1	mandatory= 0	0	1

write_real
funcName= Run_ConvertorRealToString 1	3	EndOfFunc
BEGIN_LINE
0	159	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
236	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	-345	-215	-295
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

2	-1	coords= -236 -335 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	236	1	2	-1	0
-205	-305
-205	-365
-260	-365
-260	-335
END_LINE
startport

2	-1	coords= -236 -320 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	-320
END_LINE
startport

2	1	coords= -236 -305 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	0	2	-1	0
60	-260
60	-370
-281	-370
-281	-305
END_LINE
finishport

3	-1	coords= -229 -325 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	1	3	-1	0
END_LINE
finishport

3	-1	coords= -229 -305 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
237	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	-275	-215	-225
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

2	-1	coords= -236 -265 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	237	1	2	-1	0
-205	-235
-205	-295
-265	-295
-265	-265
END_LINE
startport

2	-1	coords= -236 -250 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	-250
END_LINE
startport

2	1	coords= -236 -235 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	1	2	-1	0
60	-235
60	-370
-281	-370
-281	-235
END_LINE
finishport

3	-1	coords= -229 -255 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	2	3	-1	0
END_LINE
finishport

3	-1	coords= -229 -235 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	62	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
238	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	-205	-215	-155
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

2	-1	coords= -236 -195 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	238	1	2	-1	0
-210	-165
-210	-220
-265	-220
-265	-195
END_LINE
startport

2	-1	coords= -236 -180 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	-180
END_LINE
startport

2	1	coords= -236 -165 0	3	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	2	2	-1	0
60	-210
60	-370
-281	-370
-281	-165
END_LINE
finishport

3	-1	coords= -229 -185 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	3	3	-1	0
END_LINE
finishport

3	-1	coords= -229 -165 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	63	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
239	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	-140	-215	-90
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

2	-1	coords= -236 -130 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	239	1	2	-1	0
-210	-100
-210	-155
-265	-155
-265	-130
END_LINE
startport

2	-1	coords= -236 -115 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	-115
END_LINE
startport

2	1	coords= -236 -100 0	4	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	3	2	-1	0
60	-185
60	-370
-281	-370
-281	-100
END_LINE
finishport

3	-1	coords= -229 -120 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	4	3	-1	0
END_LINE
finishport

3	-1	coords= -229 -100 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	64	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
240	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	-70	-215	-20
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

2	-1	coords= -236 -60 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	240	1	2	-1	0
-210	-30
-210	-85
-265	-85
-265	-60
END_LINE
startport

2	-1	coords= -236 -45 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	-45
END_LINE
startport

2	1	coords= -236 -30 0	5	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	4	2	-1	0
60	-160
60	-370
-281	-370
-281	-30
END_LINE
finishport

3	-1	coords= -229 -50 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	5	3	-1	0
END_LINE
finishport

3	-1	coords= -229 -30 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
241	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	5	-215	55
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

2	-1	coords= -236 15 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	241	1	2	-1	0
-210	45
-210	-10
-265	-10
-265	15
END_LINE
startport

2	-1	coords= -236 30 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	30
END_LINE
startport

2	1	coords= -236 45 0	6	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	5	2	-1	0
60	-135
60	-370
-281	-370
-281	45
END_LINE
finishport

3	-1	coords= -229 25 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	6	3	-1	0
END_LINE
finishport

3	-1	coords= -229 45 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
242	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
-235	80	-215	130
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

2	-1	coords= -236 90 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	242	1	2	-1	0
-210	120
-210	65
-265	65
-265	90
END_LINE
startport

2	-1	coords= -236 105 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-256	-615
-256	105
END_LINE
startport

2	1	coords= -236 120 0	7	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	6	2	-1	0
60	-110
60	-370
-281	-370
-281	120
END_LINE
finishport

3	-1	coords= -229 100 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	52	7	3	-1	0
END_LINE
finishport

3	-1	coords= -229 120 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
287	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
185	-435	205	-385
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

2	-1	coords= 184 -425 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	287	1	2	-1	0
225	-395
225	-455
134	-455
134	-425
END_LINE
startport

2	-1	coords= 184 -410 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-46	-615
-46	-410
END_LINE
startport

2	-1	coords= 184 -395 0	2	0	0	mandatory= 0	0	1

write_real_done
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	52	7	2	-1	0
94	-85
94	-395
END_LINE
finishport

3	-1	coords= 191 -415 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	0	3	-1	0
END_LINE
finishport

3	-1	coords= 191 -395 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	160	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
288	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
220	190	360	211
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
String Constant	3	0	100	1	5	4	0	1.2	test
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 360 201 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	53	0	1	-1	0
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
334	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
1345	-390	1365	-340
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

2	-1	coords= 1344 -380 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	334	1	2	-1	0
1385	-350
1385	-410
1294	-410
1294	-380
END_LINE
startport

2	-1	coords= 1344 -365 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
534	-615
534	-365
END_LINE
startport

2	-1	coords= 1344 -350 0	2	0	0	mandatory= 0	0	1

write_bool_done
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	7	2	-1	0
1207	-50
1207	-350
END_LINE
finishport

3	-1	coords= 1351 -370 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	-1	coords= 1351 -350 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	160	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
383	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	-335	425	-285
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

2	-1	coords= 404 -325 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	383	1	2	-1	0
435	-295
435	-355
380	-355
380	-325
END_LINE
startport

2	-1	coords= 404 -310 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	-310
END_LINE
startport

2	1	coords= 404 -295 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	0	2	-1	0
610	-230
610	-360
359	-360
359	-295
END_LINE
finishport

3	-1	coords= 411 -315 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	1	3	-1	0
END_LINE
finishport

3	-1	coords= 411 -295 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
384	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	-265	425	-215
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

2	-1	coords= 404 -255 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	384	1	2	-1	0
435	-225
435	-285
375	-285
375	-255
END_LINE
startport

2	-1	coords= 404 -240 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	-240
END_LINE
startport

2	1	coords= 404 -225 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	1	2	-1	0
610	-205
610	-360
359	-360
359	-225
END_LINE
finishport

3	-1	coords= 411 -245 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	2	3	-1	0
END_LINE
finishport

3	-1	coords= 411 -225 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	62	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
385	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	-195	425	-145
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

2	-1	coords= 404 -185 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	385	1	2	-1	0
430	-155
430	-210
375	-210
375	-185
END_LINE
startport

2	-1	coords= 404 -170 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	-170
END_LINE
startport

2	1	coords= 404 -155 0	3	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	2	2	-1	0
610	-180
610	-360
359	-360
359	-155
END_LINE
finishport

3	-1	coords= 411 -175 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	3	3	-1	0
END_LINE
finishport

3	-1	coords= 411 -155 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	63	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
386	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	-130	425	-80
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

2	-1	coords= 404 -120 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	386	1	2	-1	0
430	-90
430	-145
375	-145
375	-120
END_LINE
startport

2	-1	coords= 404 -105 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	-105
END_LINE
startport

2	1	coords= 404 -90 0	4	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	3	2	-1	0
610	-155
610	-360
359	-360
359	-90
END_LINE
finishport

3	-1	coords= 411 -110 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	4	3	-1	0
END_LINE
finishport

3	-1	coords= 411 -90 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	64	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
387	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	-60	425	-10
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

2	-1	coords= 404 -50 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	387	1	2	-1	0
430	-20
430	-75
375	-75
375	-50
END_LINE
startport

2	-1	coords= 404 -35 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	-35
END_LINE
startport

2	1	coords= 404 -20 0	5	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	4	2	-1	0
610	-130
610	-360
359	-360
359	-20
END_LINE
finishport

3	-1	coords= 411 -40 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	5	3	-1	0
END_LINE
finishport

3	-1	coords= 411 -20 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
388	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	15	425	65
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

2	-1	coords= 404 25 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	388	1	2	-1	0
430	55
430	0
375	0
375	25
END_LINE
startport

2	-1	coords= 404 40 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	40
END_LINE
startport

2	1	coords= 404 55 0	6	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	5	2	-1	0
610	-105
610	-360
359	-360
359	55
END_LINE
finishport

3	-1	coords= 411 35 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	6	3	-1	0
END_LINE
finishport

3	-1	coords= 411 55 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
389	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
405	90	425	140
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

2	-1	coords= 404 100 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	389	1	2	-1	0
430	130
430	75
375	75
375	100
END_LINE
startport

2	-1	coords= 404 115 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
64	-615
64	115
END_LINE
startport

2	1	coords= 404 130 0	7	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	53	6	2	-1	0
610	-80
610	-360
359	-360
359	130
END_LINE
finishport

3	-1	coords= 411 110 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	53	7	3	-1	0
END_LINE
finishport

3	-1	coords= 411 130 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

convertor_tranbsx
_
390	0	0	0
_
_
This is a boolean to string converter.
ConvertorBoolToString
0
_
1350	-235	1380	-205
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

0	0	coords= 1349 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConvertorBoolToString 1	1	EndOfFunc
BEGIN_LINE
1	58	0	0	-1	0
1294	-215
1304	-215
END_LINE
outputport

1	3	coords= 1380 -215 0	0	0	1	mandatory= 0	0	1

write_bool_str
funcName= Run_ConvertorBoolToString 1	2	EndOfFunc
BEGIN_LINE
0	159	3	1	-1	0
END_LINE
startport

2	3	coords= 1349 -225 0	0	0	0	mandatory= 0	0	1


funcName= Run_ConvertorBoolToString 1	0	EndOfFunc
BEGIN_LINE
1	58	0	2	-1	0
1294	-225
1304	-225
END_LINE
finishport

3	3	coords= 1380 -225 0	0	0	1	mandatory= 0	0	1

write_bool
funcName= Run_ConvertorBoolToString 1	3	EndOfFunc
BEGIN_LINE
0	159	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
445	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	-325	925	-275
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

2	-1	coords= 904 -315 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	445	1	2	-1	0
935	-285
935	-345
880	-345
880	-315
END_LINE
startport

2	-1	coords= 904 -300 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	-300
END_LINE
startport

2	1	coords= 904 -285 0	1	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
1125	-225
1125	-350
859	-350
859	-285
END_LINE
finishport

3	-1	coords= 911 -305 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	1	3	-1	0
END_LINE
finishport

3	-1	coords= 911 -285 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	61	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
446	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	-255	925	-205
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

2	-1	coords= 904 -245 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	446	1	2	-1	0
935	-215
935	-275
875	-275
875	-245
END_LINE
startport

2	-1	coords= 904 -230 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	-230
END_LINE
startport

2	1	coords= 904 -215 0	2	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	1	2	-1	0
1125	-200
1125	-350
859	-350
859	-215
END_LINE
finishport

3	-1	coords= 911 -235 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	2	3	-1	0
END_LINE
finishport

3	-1	coords= 911 -215 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	62	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
447	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	-185	925	-135
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

2	-1	coords= 904 -175 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	447	1	2	-1	0
930	-145
930	-200
875	-200
875	-175
END_LINE
startport

2	-1	coords= 904 -160 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	-160
END_LINE
startport

2	1	coords= 904 -145 0	3	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	2	2	-1	0
1125	-175
1125	-350
859	-350
859	-145
END_LINE
finishport

3	-1	coords= 911 -165 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	3	3	-1	0
END_LINE
finishport

3	-1	coords= 911 -145 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	63	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
448	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	-120	925	-70
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

2	-1	coords= 904 -110 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	448	1	2	-1	0
930	-80
930	-135
875	-135
875	-110
END_LINE
startport

2	-1	coords= 904 -95 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	-95
END_LINE
startport

2	1	coords= 904 -80 0	4	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	3	2	-1	0
1125	-150
1125	-350
859	-350
859	-80
END_LINE
finishport

3	-1	coords= 911 -100 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	4	3	-1	0
END_LINE
finishport

3	-1	coords= 911 -80 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	64	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
449	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	-50	925	0
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

2	-1	coords= 904 -40 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	449	1	2	-1	0
930	-10
930	-65
875	-65
875	-40
END_LINE
startport

2	-1	coords= 904 -25 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	-25
END_LINE
startport

2	1	coords= 904 -10 0	5	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	4	2	-1	0
1125	-125
1125	-350
859	-350
859	-10
END_LINE
finishport

3	-1	coords= 911 -30 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	5	3	-1	0
END_LINE
finishport

3	-1	coords= 911 -10 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
450	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	25	925	75
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

2	-1	coords= 904 35 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	450	1	2	-1	0
930	65
930	10
875	10
875	35
END_LINE
startport

2	-1	coords= 904 50 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	50
END_LINE
startport

2	1	coords= 904 65 0	6	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	5	2	-1	0
1125	-100
1125	-350
859	-350
859	65
END_LINE
finishport

3	-1	coords= 911 45 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	6	3	-1	0
END_LINE
finishport

3	-1	coords= 911 65 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
451	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
905	100	925	150
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

2	-1	coords= 904 110 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	451	1	2	-1	0
930	140
930	85
875	85
875	110
END_LINE
startport

2	-1	coords= 904 125 0	1	0	0	mandatory= 1	0	1

write_done
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
314	-615
314	125
END_LINE
startport

2	1	coords= 904 140 0	7	0	0	mandatory= 0	0	1


funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	54	6	2	-1	0
1125	-75
1125	-350
859	-350
859	140
END_LINE
finishport

3	-1	coords= 911 120 0	0	0	1	mandatory= 1	0	1


funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	54	7	3	-1	0
END_LINE
finishport

3	-1	coords= 911 140 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	65	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
452	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
0
_
910	205	945	225
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

1	0	coords= 945 215 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantBool 1	1	EndOfFunc
BEGIN_LINE
0	54	0	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantBool 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
