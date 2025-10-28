#V:2.1.2

CanvasSizeXY
760	255

IconData
BEGIN_BLOCK

camera
_
1	0	0	0
_
_
Run machine learning camera
Camera
0
_
20	-510	130	-380
1	0
blockattr
BEGIN_BA
type	IO
text	25	5	1.25	0	Camera
trans	-1	0	0	0
offset	0	-15
hash	0xC05A
Instance_Info	_
End_Instance
END_BA
parameter
camera_id	3	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Camera ID

parameter
fps	1	0	1000	0	0	0	0	1.2	100
EndOfValues
EndOfLabels
frame rate

parameter
width	1	0	10000	0	0	0	0	1.2	100
EndOfValues
EndOfLabels
width

parameter
height	1	0	10000	0	0	0	0	1.2	100
EndOfValues
EndOfLabels
height

parameter
asynchronous	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
camera outputs frames without capture event

parameter
show_frame	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Display the image with given frame

parameter
greyscale	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Make the camera output greyscale. Note that this has limited support on various devices. Devices with libcamera support generally support this feature.

inputport

0	3	coords= 19 -473 0	0	0	1	mandatory= 0	0	1
id

funcName= startCamera 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 19 -430 0	1	0	1	mandatory= 0	0	1
show

funcName= grabFrame 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 130 -463 0	0	0	1	mandatory= 0	0	1
errno

funcName= startCamera 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 130 -425 0	1	0	1	mandatory= 0	0	1
width

funcName= grabFrame 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 130 -415 0	2	0	1	mandatory= 0	0	1
height

funcName= grabFrame 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 130 -435 0	3	0	1	mandatory= 0	0	1
stream_id

funcName= grabFrame 3	4	EndOfFunc
BEGIN_LINE
0	3	0	1	-1	0
END_LINE
startport

2	3	coords= 19 -483 0	0	1	0	mandatory= 0	0	1
start

funcName= startCamera 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 19 -440 0	1	0	0	mandatory= 0	0	1
capture

funcName= grabFrame 3	0	EndOfFunc
BEGIN_LINE
1	2	0	2	-1	0
-60	-420
-60	-440
END_LINE
startport

2	1	coords= 19 -398 0	2	0	0	mandatory= 0	0	1
stop

funcName= stopCamera 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= 130 -483 0	0	0	1	mandatory= 0	0	1
--

funcName= startCamera 1	3	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
finishport

3	3	coords= 130 -473 0	1	0	1	mandatory= 0	0	1
err

funcName= startCamera 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 130 -445 0	2	0	1	mandatory= 0	0	1
--

funcName= grabFrame 3	5	EndOfFunc
BEGIN_LINE
0	3	0	3	-1	0
END_LINE
finishport

3	1	coords= 130 -398 0	3	0	1	mandatory= 0	0	1
--

funcName= stopCamera 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
2	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
-145	-475	-80	-385
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	15	5	1.5	0	Timer
trans	-1	0	0	0
offset	0	-15
hash	0xEEFF
Instance_Info	_
End_Instance
END_BA
parameter
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	1000000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= -146 -440 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -146 -430 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -146 -420 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= -146 -450 0	0	0	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
1	1	0	2	-1	0
180	-483
180	-550
-196	-550
-196	-450
END_LINE
startport

2	0	coords= -146 -400 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -80 -420 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	1	1	3	-1	0
END_LINE
finishport

3	0	coords= -80 -450 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= -80 -400 0	2	0	1	mandatory= 0	0	1


funcName= stop 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= tick 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

frame_crop
_
3	0	0	0
_
_
Crops camera frame
FrameCrop
0
_
280	-485	390	-385
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	Frame_Crop
trans	-1	0	0	0
offset	0	-15
hash	0x6631
Instance_Info	_
End_Instance
END_BA
parameter
crop_x1	1	0	10000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
crop top

parameter
crop_y1	1	0	10000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Crop left

parameter
crop_x2	1	0	10000	0	0	0	0	1.2	50
EndOfValues
EndOfLabels
Crop right

parameter
crop_y2	1	0	10000	0	0	0	0	1.2	50
EndOfValues
EndOfLabels
Crop bottom

inputport

0	1	coords= 279 -448 0	0	0	1	mandatory= 0	0	1
stream_id

funcName= run_cropping 1	1	EndOfFunc
BEGIN_LINE
1	1	3	0	-1	0
165	-435
165	-448
END_LINE
inputport

0	1	coords= 279 -433 0	1	0	1	mandatory= 0	0	1
crop_x1

funcName= run_cropping 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 279 -423 0	2	0	1	mandatory= 0	0	1
crop_y1

funcName= run_cropping 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 279 -413 0	3	0	1	mandatory= 0	0	1
crop_x2

funcName= run_cropping 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= 279 -403 0	4	0	1	mandatory= 0	0	1
crop_y2

funcName= run_cropping 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 390 -418 0	0	0	1	mandatory= 0	0	1
errno

funcName= run_cropping 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= 390 -448 0	1	0	1	mandatory= 0	0	1
stream_id

funcName= run_cropping 1	7	EndOfFunc
BEGIN_LINE
0	4	0	1	-1	0
END_LINE
startport

2	1	coords= 279 -458 0	0	0	0	mandatory= 0	0	1
run

funcName= run_cropping 1	0	EndOfFunc
BEGIN_LINE
1	1	2	2	-1	0
244	-445
244	-458
END_LINE
finishport

3	1	coords= 390 -458 0	0	0	1	mandatory= 0	0	1
--

funcName= run_cropping 1	8	EndOfFunc
BEGIN_LINE
0	4	0	3	-1	0
END_LINE
finishport

3	1	coords= 390 -428 0	1	0	1	mandatory= 0	0	1
err

funcName= run_cropping 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

frame_show
_
4	0	0	0
_
_
Show the frame in a pop window with given frame ID input.
frame_show
0
_
475	-445	585	-395
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	Frame_Show
trans	-1	0	0	0
offset	0	-15
hash	0x578F
Instance_Info	_
End_Instance
END_BA
parameter
window_title	3	0	0	0	0	0	0	1.2	frame_cropped
EndOfValues
EndOfLabels
Title of the pop window.

inputport

0	1	coords= 474 -410 0	0	0	1	mandatory= 1	0	1
frame id

funcName= show 1	1	EndOfFunc
BEGIN_LINE
1	3	1	0	-1	0
449	-448
449	-410
END_LINE
startport

2	3	coords= 474 -420 0	0	0	0	mandatory= 0	0	1
show

funcName= show 1	0	EndOfFunc
BEGIN_LINE
1	3	0	2	-1	0
444	-458
444	-420
END_LINE
finishport

3	3	coords= 585 -420 0	0	0	1	mandatory= 0	0	1
--

funcName= show 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 585 -410 0	1	0	1	mandatory= 0	0	1
error

funcName= show 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
