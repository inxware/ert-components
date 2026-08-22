#V:2.1.2

CanvasSizeXY
1035	961

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
write_result_str
funcName= Run_WriteFILE_WriteOnly_String 3	1	EndOfFunc
BEGIN_LINE
1	56	0	0	-1	0
140	-220
140	-290
-376	-290
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
write_result
funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	56	0	2	-1	0
140	-230
140	-290
-376	-290
-376	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	55	3	3	-1	0
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
write_results_done
funcName= Run_WriteFILE_WriteOnly_String 3	3	EndOfFunc
BEGIN_LINE
0	64	1	3	-1	0
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
1	63	0	2	-1	0
281	-145
281	-200
-721	-200
-721	-620
END_LINE
finishport

3	3	coords= -645 -620 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

event_switch
_
54	0	0	0
_
_
Switches Event to Indexed Output
IndexedDemultiplexer_Event
0
_
-140	-200	-110	-15
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	5	5	1.25	0	Swt
trans	-1	0	0	0
offset	0	-15
hash	0x6E2A
Instance_Info	_
End_Instance
END_BA
parameter
Index start	1	-2147483648	2147483647	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Value for firing port 1

inputport

0	1	coords= -141 -165 0	0	0	1	mandatory= 0	0	1
ind

funcName= Run 1	1	EndOfFunc
BEGIN_LINE
1	55	0	0	-1	0
-216	-200
-216	-165
END_LINE
outputport

1	1	coords= -110 -20 0	0	0	1	mandatory= 0	0	1
off

funcName= Run 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= -141 -175 0	0	0	0	mandatory= 0	0	1
go

funcName= Run 1	0	EndOfFunc
BEGIN_LINE
1	55	0	2	-1	0
-200	-250
-200	-175
END_LINE
finishport

3	1	coords= -110 -155 0	0	0	1	mandatory= 0	0	1
1

funcName= Run 1	3	EndOfFunc
BEGIN_LINE
0	56	0	3	-1	0
END_LINE
finishport

3	1	coords= -110 -140 0	1	0	1	mandatory= 0	0	1
2

funcName= Run 1	4	EndOfFunc
BEGIN_LINE
0	56	1	3	-1	0
END_LINE
finishport

3	1	coords= -110 -125 0	2	0	1	mandatory= 0	0	1
3

funcName= Run 1	5	EndOfFunc
BEGIN_LINE
0	56	2	3	-1	0
END_LINE
finishport

3	1	coords= -110 -110 0	3	0	1	mandatory= 0	0	1
4

funcName= Run 1	6	EndOfFunc
BEGIN_LINE
0	57	0	3	-1	0
END_LINE
finishport

3	1	coords= -110 -95 0	4	0	1	mandatory= 0	0	1
5

funcName= Run 1	7	EndOfFunc
BEGIN_LINE
0	57	1	3	-1	0
END_LINE
finishport

3	1	coords= -110 -80 0	5	0	1	mandatory= 0	0	1
6

funcName= Run 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -110 -65 0	6	0	1	mandatory= 0	0	1
7

funcName= Run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -110 -50 0	7	0	1	mandatory= 0	0	1
8

funcName= Run 1	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -110 -30 0	8	0	1	mandatory= 0	0	1
ovf

funcName= Run 1	11	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -110 -175 0	9	0	1	mandatory= 0	0	1


funcName= Run 1	12	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

EventCounter_versatile1
_
55	0	0	0
_
_
event counter providing option to set a maximum and minimum threshold with an input value.
EventCounterVersatile1
0
_
-345	-290	-280	-115
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
Max. Threshold	1	-2147483648	2147483647	0	0	0	0	1.2	5
EndOfValues
EndOfLabels
Threshold value for ovf event.

parameter
Min. Threshold	1	-2147483648	2147483647	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Threshold value for ovf event.

inputport

0	1	coords= -346 -135 0	0	0	1	mandatory= 0	0	1
max

funcName= Run_CountEventCounter 2	1	funcName= Run_DecrementEventCounter 3	1	funcName= Run_ResetEventCounter 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -346 -125 0	1	0	1	mandatory= 0	0	1
min

funcName= Run_CountEventCounter 2	2	funcName= Run_DecrementEventCounter 3	2	funcName= Run_ResetEventCounter 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -346 -155 0	2	0	1	mandatory= 0	0	1
ini

funcName= Run_CountEventCounter 2	3	funcName= Run_DecrementEventCounter 3	3	funcName= Run_ResetEventCounter 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -346 -145 0	3	0	1	mandatory= 0	0	1
step

funcName= Run_CountEventCounter 2	4	funcName= Run_DecrementEventCounter 3	4	funcName= Run_ResetEventCounter 4	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -280 -200 0	0	0	1	mandatory= 0	0	1
cnt

funcName= Run_CountEventCounter 2	5	funcName= Run_DecrementEventCounter 3	5	funcName= Run_ResetEventCounter 4	5	EndOfFunc
BEGIN_LINE
0	116863	0	1	-1	0
END_LINE
startport

2	1	coords= -346 -180 0	0	0	0	mandatory= 0	0	1
enabl

funcName= Run_StartEventCounter 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= -346 -250 0	0	0	0	mandatory= 0	0	1
inc

funcName= Run_CountEventCounter 2	0	EndOfFunc
BEGIN_LINE
1	64	0	2	-1	0
-375	-234
-365	-250
END_LINE
startport

2	1	coords= -346 -225 0	2	0	0	mandatory= 0	0	1
dec

funcName= Run_DecrementEventCounter 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -346 -265 0	3	0	0	mandatory= 0	0	1
rst
test_start
funcName= Run_ResetEventCounter 4	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-315
-401	-315
-401	-265
END_LINE
startport

2	1	coords= -346 -170 0	4	0	0	mandatory= 0	0	1
disabl

funcName= Run_StopEventCounter 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -280 -250 0	0	0	1	mandatory= 0	0	1
--

funcName= Run_CountEventCounter 2	6	EndOfFunc
BEGIN_LINE
0	116863	0	3	-1	0
END_LINE
finishport

3	1	coords= -280 -225 0	1	0	1	mandatory= 0	0	1
--

funcName= Run_DecrementEventCounter 3	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -280 -240 0	2	0	1	mandatory= 0	0	1
ovf
counter_last
funcName= Run_CountEventCounter 2	7	EndOfFunc
BEGIN_LINE
0	63	1	3	-1	0
END_LINE
finishport

3	1	coords= -280 -215 0	3	0	1	mandatory= 0	0	1
ovf

funcName= Run_DecrementEventCounter 3	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -280 -265 0	4	0	1	mandatory= 0	0	1
--

funcName= Run_ResetEventCounter 4	6	EndOfFunc
BEGIN_LINE
0	116864	1	3	-1	0
END_LINE
finishport

3	1	coords= -280 -180 0	5	0	1	mandatory= 0	0	1
--

funcName= Run_StartEventCounter 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -280 -170 0	6	0	1	mandatory= 0	0	1
--

funcName= Run_StopEventCounter 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
56	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
55	-255	85	-135
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

0	3	coords= 54 -220 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	58	0	0	-1	0
-18	-369
-18	-220
END_LINE
inputport

0	3	coords= 54 -195 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	59	0	0	-1	0
-18	-329
-18	-195
END_LINE
inputport

0	3	coords= 54 -170 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	60	0	0	-1	0
-18	-289
-18	-170
END_LINE
inputport

0	3	coords= 54 -145 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	57	0	0	-1	0
29	-70
29	-145
END_LINE
outputport

1	3	coords= 85 -220 0	0	0	1	mandatory= 0	0	1

write_result_str
funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= 54 -230 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	54	0	2	-1	0
-23	-155
-23	-230
END_LINE
startport

2	3	coords= 54 -205 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	54	1	2	-1	0
-23	-140
-23	-205
END_LINE
startport

2	3	coords= 54 -180 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	54	2	2	-1	0
-23	-125
-23	-180
END_LINE
startport

2	3	coords= 54 -155 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
1	57	0	2	-1	0
29	-80
29	-155
END_LINE
finishport

3	3	coords= 85 -230 0	0	0	1	mandatory= 0	0	1

write_result
funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	63	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4s
_
57	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-35	-105	-5	15
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

0	3	coords= -36 -70 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	61	0	0	-1	0
-61	46
-61	-70
END_LINE
inputport

0	3	coords= -36 -45 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	62	0	0	-1	0
-61	81
-61	-45
END_LINE
inputport

0	3	coords= -36 -20 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -36 5 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -5 -70 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	56	3	1	-1	0
END_LINE
startport

2	3	coords= -36 -80 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	54	3	2	-1	0
-78	-110
-78	-80
END_LINE
startport

2	3	coords= -36 -55 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	54	4	2	-1	0
-78	-95
-78	-55
END_LINE
startport

2	3	coords= -36 -30 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= -36 -5 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -5 -80 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	56	3	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
58	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-220	-380	-80	-359
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
String Constant	3	0	100	1	5	4	0	1.2	string1 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -80 -369 0	0	0	1	mandatory= 0	0	1	memsize= 16


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	56	0	1	-1	0
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
59	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-220	-340	-80	-319
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
String Constant	3	0	100	1	5	4	0	1.2	string2 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -80 -329 0	0	0	1	mandatory= 0	0	1	memsize= 16


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	56	1	1	-1	0
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
60	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-220	-300	-80	-279
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
String Constant	3	0	100	1	5	4	0	1.2	string3 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -80 -289 0	0	0	1	mandatory= 0	0	1	memsize= 16


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	56	2	1	-1	0
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
61	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-235	35	-95	56
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
String Constant	3	0	100	1	5	4	0	1.2	string4 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -95 46 0	0	0	1	mandatory= 0	0	1	memsize= 16


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	57	0	1	-1	0
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
62	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-235	70	-95	91
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
String Constant	3	0	100	1	5	4	0	1.2	string5 
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -95 81 0	0	0	1	mandatory= 0	0	1	memsize= 16


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	57	1	1	-1	0
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
63	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
220	-165	240	-115
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

2	-1	coords= 219 -155 0	0	0	0	mandatory= 1	0	1


funcName= And1 1	0	EndOfFunc
BEGIN_LINE
1	63	1	2	-1	0
271	-125
271	-200
174	-200
174	-155
END_LINE
startport

2	-1	coords= 219 -140 0	1	0	0	mandatory= 1	0	1

counter_last
funcName= And2 2	0	EndOfFunc
BEGIN_LINE
1	55	2	2	-1	0
-36	-240
-36	-140
END_LINE
startport

2	-1	coords= 219 -125 0	2	0	0	mandatory= 0	0	1

write_result
funcName= Reset 3	0	EndOfFunc
BEGIN_LINE
1	56	0	2	-1	0
157	-230
157	-125
END_LINE
finishport

3	-1	coords= 226 -145 0	0	0	1	mandatory= 1	0	1

test_done
funcName= And1 1	1	funcName= And2 2	1	EndOfFunc
BEGIN_LINE
0	50	0	3	-1	0
END_LINE
finishport

3	-1	coords= 226 -125 0	1	0	1	mandatory= 0	0	1


funcName= Reset 3	1	EndOfFunc
BEGIN_LINE
0	63	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
64	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-415	-255	-395	-215
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

2	1	coords= -416 -245 0	1	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	55	4	2	-1	0
-230	-265
-230	-310
-466	-310
-466	-245
END_LINE
startport

2	-1	coords= -416 -225 0	1	0	0	mandatory= 0	0	1

write_results_done
funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-220	-615
-220	-280
-471	-280
-471	-225
END_LINE
finishport

3	-1	coords= -403 -234 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	55	1	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
