#V:2.1.2

CanvasSizeXY
795	835

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
1	52	0	0	-1	0
40	-283
40	-330
-370	-330
-370	-595
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
1	52	0	2	-1	0
20	-293
20	-315
-381	-315
-381	-615
END_LINE
finishport

3	1	coords= -265 -665 0	3	0	1	mandatory= 0	0	1
--
test_start
funcName= Run_OpenFILE_WriteOnly_String 1	3	EndOfFunc
BEGIN_LINE
0	51	0	3	-1	0
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
-220	-605
-220	-500
-456	-500
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
-465	-545
-465	-495
-631	-495
-631	-570
END_LINE
startport

2	3	coords= -581 -545 0	2	0	0	mandatory= 0	0	1
write

funcName= Run_WriteFILE_WriteOnly_String 3	0	EndOfFunc
BEGIN_LINE
1	20	0	2	-1	0
-485	-595
-485	-640
-615	-640
-615	-545
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
-483	-585
-473	-595
END_LINE
startport

2	-1	coords= -451 -580 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	20	1	2	-1	0
-488	-570
-478	-580
END_LINE
startport

2	-1	coords= -451 -565 0	2	0	0	mandatory= 0	0	1


funcName= Or3 3	0	EndOfFunc
BEGIN_LINE
1	20	4	2	-1	0
-493	-560
-483	-565
END_LINE
startport

2	-1	coords= -451 -550 0	3	0	0	mandatory= 0	0	1


funcName= Or4 4	0	EndOfFunc
BEGIN_LINE
1	20	5	2	-1	0
-498	-535
-488	-550
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


funcName= Run_CatString 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
-215	-615
-215	-710
-736	-710
-736	-610
END_LINE
finishport

3	3	coords= -655 -610 0	0	0	1	mandatory= 0	0	1


funcName= Run_CatString 1	4	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

map_int
_
51	0	0	0
_
_
This an integer mapping function.
MapInt
0
_
-300	-305	-260	-188
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	1	5	1.25	0	MapInt
trans	-1	0	0	0
offset	0	0
hash	0x4455
Instance_Info	_
End_Instance
END_BA
parameter
not found	1	-2147483648	2147483647	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
key not found value

parameter
map	3	_	_	0	0	0	0	1.2	0=10,1=20, 5 = 1 , 1000= -10 , -3=1234  
EndOfValues
EndOfLabels
define the integers map

inputport

0	1	coords= -301 -268 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input 1	1	EndOfFunc
BEGIN_LINE
1	53	0	0	-1	0
-398	-245
-398	-268
END_LINE
inputport

0	1	coords= -301 -258 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input 1	2	EndOfFunc
BEGIN_LINE
1	54	0	0	-1	0
-398	-215
-398	-258
END_LINE
inputport

0	1	coords= -301 -248 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input 1	3	EndOfFunc
BEGIN_LINE
1	72	0	0	-1	0
-398	-185
-398	-248
END_LINE
inputport

0	1	coords= -301 -238 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input 1	4	EndOfFunc
BEGIN_LINE
1	73	0	0	-1	0
-398	-155
-398	-238
END_LINE
inputport

0	1	coords= -301 -228 0	4	0	1	mandatory= 0	0	1
i5

funcName= Run_Input 1	5	EndOfFunc
BEGIN_LINE
1	94	0	0	-1	0
-398	-125
-398	-228
END_LINE
inputport

0	1	coords= -301 -218 0	5	0	1	mandatory= 0	0	1
i6

funcName= Run_Input 1	6	EndOfFunc
BEGIN_LINE
1	95	0	0	-1	0
-398	-95
-398	-218
END_LINE
inputport

0	1	coords= -301 -208 0	6	0	1	mandatory= 0	0	1
i7

funcName= Run_Input 1	7	EndOfFunc
BEGIN_LINE
1	96	0	0	-1	0
-398	-65
-398	-208
END_LINE
inputport

0	1	coords= -301 -198 0	7	0	1	mandatory= 0	0	1
i8

funcName= Run_Input 1	8	EndOfFunc
BEGIN_LINE
1	97	0	0	-1	0
-398	-35
-398	-198
END_LINE
outputport

1	1	coords= -260 -268 0	0	0	1	mandatory= 0	0	1
o1

funcName= Run_Input 1	9	EndOfFunc
BEGIN_LINE
0	52	0	1	-1	0
END_LINE
outputport

1	1	coords= -260 -258 0	1	0	1	mandatory= 0	0	1
o2

funcName= Run_Input 1	10	EndOfFunc
BEGIN_LINE
0	52	1	1	-1	0
END_LINE
outputport

1	1	coords= -260 -248 0	2	0	1	mandatory= 0	0	1
o3

funcName= Run_Input 1	11	EndOfFunc
BEGIN_LINE
0	52	2	1	-1	0
END_LINE
outputport

1	1	coords= -260 -238 0	3	0	1	mandatory= 0	0	1
o4

funcName= Run_Input 1	12	EndOfFunc
BEGIN_LINE
0	52	3	1	-1	0
END_LINE
outputport

1	1	coords= -260 -228 0	4	0	1	mandatory= 0	0	1
o5

funcName= Run_Input 1	13	EndOfFunc
BEGIN_LINE
0	52	4	1	-1	0
END_LINE
outputport

1	1	coords= -260 -218 0	5	0	1	mandatory= 0	0	1
o6

funcName= Run_Input 1	14	EndOfFunc
BEGIN_LINE
0	52	5	1	-1	0
END_LINE
outputport

1	1	coords= -260 -208 0	6	0	1	mandatory= 0	0	1
o7

funcName= Run_Input 1	15	EndOfFunc
BEGIN_LINE
0	52	6	1	-1	0
END_LINE
outputport

1	1	coords= -260 -198 0	7	0	1	mandatory= 0	0	1
o8

funcName= Run_Input 1	16	EndOfFunc
BEGIN_LINE
0	52	7	1	-1	0
END_LINE
startport

2	3	coords= -301 -280 0	0	0	0	mandatory= 0	0	1
i
test_start
funcName= Run_Input 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
-220	-665
-220	-5
-356	-5
-356	-280
END_LINE
finishport

3	3	coords= -260 -280 0	0	0	1	mandatory= 0	0	1
o

funcName= Run_Input 1	17	EndOfFunc
BEGIN_LINE
0	52	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats8_int
_
52	0	0	0
_
_
This is an 8-input integer formatter. Use the format property to show how the string should appear.
string_format8_int
0
_
-75	-305	-25	-160
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

0	1	coords= -76 -283 0	0	0	1	mandatory= 0	0	1
i1

funcName= run 1	1	EndOfFunc
BEGIN_LINE
1	51	0	0	-1	0
-163	-268
-153	-283
END_LINE
inputport

0	1	coords= -76 -273 0	1	0	1	mandatory= 0	0	1
i2

funcName= run 1	2	EndOfFunc
BEGIN_LINE
1	51	1	0	-1	0
-163	-258
-153	-273
END_LINE
inputport

0	1	coords= -76 -263 0	2	0	1	mandatory= 0	0	1
i3

funcName= run 1	3	EndOfFunc
BEGIN_LINE
1	51	2	0	-1	0
-163	-248
-153	-263
END_LINE
inputport

0	1	coords= -76 -253 0	3	0	1	mandatory= 0	0	1
i4

funcName= run 1	4	EndOfFunc
BEGIN_LINE
1	51	3	0	-1	0
-163	-238
-153	-253
END_LINE
inputport

0	1	coords= -76 -243 0	4	0	1	mandatory= 0	0	1
i5

funcName= run 1	5	EndOfFunc
BEGIN_LINE
1	51	4	0	-1	0
-163	-228
-153	-243
END_LINE
inputport

0	1	coords= -76 -233 0	5	0	1	mandatory= 0	0	1
i6

funcName= run 1	6	EndOfFunc
BEGIN_LINE
1	51	5	0	-1	0
-163	-218
-153	-233
END_LINE
inputport

0	1	coords= -76 -223 0	6	0	1	mandatory= 0	0	1
i7

funcName= run 1	7	EndOfFunc
BEGIN_LINE
1	51	6	0	-1	0
-163	-208
-153	-223
END_LINE
inputport

0	1	coords= -76 -213 0	7	0	1	mandatory= 0	0	1
i8

funcName= run 1	8	EndOfFunc
BEGIN_LINE
1	51	7	0	-1	0
-163	-198
-153	-213
END_LINE
inputport

0	3	coords= -76 -193 0	8	0	1	mandatory= 0	0	1
fmt

funcName= run 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	coords= -25 -283 0	0	0	1	mandatory= 0	0	1


funcName= run 1	10	EndOfFunc
BEGIN_LINE
0	1	1	1	-1	0
END_LINE
startport

2	3	coords= -76 -293 0	0	0	0	mandatory= 0	0	1


funcName= run 1	0	EndOfFunc
BEGIN_LINE
1	51	0	2	-1	0
-163	-280
-153	-293
END_LINE
finishport

3	3	coords= -25 -293 0	0	0	1	mandatory= 0	0	1


funcName= run 1	11	EndOfFunc
BEGIN_LINE
0	1	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
53	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-255	-505	-235
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

1	1	coords= -505 -245 0	0	0	1	mandatory= 0	0	1


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

const_i1
_
54	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-225	-505	-205
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

1	1	coords= -505 -215 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	1	1	-1	0
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
72	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-195	-505	-175
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

1	1	coords= -505 -185 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	2	1	-1	0
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
73	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-165	-505	-145
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	-3
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -505 -155 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	3	1	-1	0
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
94	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-135	-505	-115
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	1000
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -505 -125 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	4	1	-1	0
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
95	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-105	-505	-85
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

1	1	coords= -505 -95 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	5	1	-1	0
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
96	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-75	-505	-55
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

1	1	coords= -505 -65 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	6	1	-1	0
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
97	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
-600	-45	-505	-25
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	88
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= -505 -35 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	51	7	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
