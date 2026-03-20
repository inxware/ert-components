#V:2.1.2

CanvasSizeXY
43285	41245

IconData
BEGIN_BLOCK

ml_image_inference
_
3	0	0	0
_
_
Inference of data input based on loaded model with vaiants like yolov8-seg. Hardware Acceleration is available for supported targets.
ml_image_inference
0
_
-1570	-1520	-1480	-1405
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	IMG_Infer
trans	-1	0	0	0
offset	0	-15
hash	0xF512
Instance_Info	_
End_Instance
END_BA
parameter
Model Type	1	0	4001	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The type of model loaded. It is used to determine the inference output format.

parameter
Model File Ext	1	1	4	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
The file extension of the loaded model. 1 for `.tflite`, 2 for `.onnx`, 3 for `.pb`, 4 for `.hef`.

parameter
Data Type	1	0	4	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The Data type of model input/ouput. 0 for auto-detection, 1 for 8-bit, 2 for 16-bit, 3 for 32-bit, 4 for 64-bit

parameter
HW Accelerate	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Whether to enable the hardware acceleration. Some platforms might not support it and error will occur when this is True. Some platforms require a specific Model file format, where this block would emit error if wrong model file format is loaded while this is True.

parameter
Conf Thres	2	0	1	0	0	0	0	1.2	0.5
EndOfValues
EndOfLabels
The confidence Threshold for the output data. The range is from 0 to 1.

parameter
Thread Number	1	0	16	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The number of threads for inference. If the platform does not support threading, this parameter will be ignored.

parameter
Use Application Dir	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Whether to use application directory as the root of model path. If not, the User directory will be used.

inputport

0	3	coords= -1571 -1480 0	0	0	1	mandatory= 0	0	1
path

funcName= load_model 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -1571 -1440 0	1	0	1	mandatory= 0	0	1
stream_id

funcName= inference 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -1480 -1470 0	0	0	1	mandatory= 0	0	1
errno

funcName= load_model 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -1480 -1430 0	1	0	1	mandatory= 0	0	1
errno

funcName= inference 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -1480 -1420 0	2	0	1	mandatory= 0	0	1
json

funcName= inference 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -1571 -1490 0	0	0	0	mandatory= 0	0	1
load

funcName= load_model 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -1571 -1450 0	1	0	0	mandatory= 0	0	1
do

funcName= inference 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -1480 -1490 0	0	0	1	mandatory= 0	0	1
--

funcName= load_model 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -1480 -1480 0	1	0	1	mandatory= 0	0	1
err

funcName= load_model 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -1480 -1450 0	2	0	1	mandatory= 0	0	1
--

funcName= inference 2	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -1480 -1440 0	3	0	1	mandatory= 0	0	1
err

funcName= inference 2	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

ml_image_inference
_
8	0	0	0
_
_
Inference of data input based on loaded model with vaiants like yolov8-seg. Hardware Acceleration is available for supported targets.
ml_image_inference
0
_
-44005	-42415	-43915	-42290
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	IMG_Infer
trans	-1	0	0	0
offset	0	-15
hash	0xF512
Instance_Info	_
End_Instance
END_BA
parameter
Model Type	1	0	4001	0	0	0	0	1.2	1003
EndOfValues
EndOfLabels
The type of model loaded. It is used to determine the inference output format.

parameter
Model File Ext	1	1	4	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
The file extension of the loaded model. 1 for `.tflite`, 2 for `.onnx`, 3 for `.pb`, 4 for `.hef`.

parameter
Data Type	1	0	4	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The Data type of model input/ouput. 0 for auto-detection, 1 for 8-bit, 2 for 16-bit, 3 for 32-bit, 4 for 64-bit

parameter
HW Accelerate	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Whether to enable the hardware acceleration. Some platforms might not support it and error will occur when this is True. Some platforms require a specific Model file format, where this block would emit error if wrong model file format is loaded while this is True.

parameter
Conf Thres	2	0	1	0	0	0	0	1.2	0.35
EndOfValues
EndOfLabels
The confidence Threshold for the output data. The range is from 0 to 1.

parameter
Thread Number	1	0	16	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
The number of threads for inference. If the platform does not support threading, this parameter will be ignored.

parameter
Use Application Dir	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Whether to use application directory as the root of model path. If not, the User directory will be used.

inputport

0	3	coords= -44006 -42375 0	0	0	1	mandatory= 0	0	1
path

funcName= load_model 1	1	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
-44150	-42399
-44150	-42375
END_LINE
inputport

0	1	coords= -44006 -42335 0	0	0	1	mandatory= 0	0	1
stream_id

funcName= inference 2	1	EndOfFunc
BEGIN_LINE
1	128	1	0	-1	0
-44067	-42293
-44067	-42335
END_LINE
outputport

1	1	coords= -43915 -42365 0	0	0	1	mandatory= 0	0	1
errno

funcName= load_model 1	2	EndOfFunc
BEGIN_LINE
0	20	0	1	-1	0
END_LINE
outputport

1	3	coords= -43915 -42355 0	1	0	1	mandatory= 0	0	1
model info

funcName= load_model 1	3	EndOfFunc
BEGIN_LINE
0	18	1	1	-1	0
END_LINE
outputport

1	1	coords= -43915 -42325 0	2	0	1	mandatory= 0	0	1
errno

funcName= inference 2	2	EndOfFunc
BEGIN_LINE
0	20	1	1	-1	0
END_LINE
outputport

1	3	coords= -43915 -42315 0	3	0	1	mandatory= 0	0	1
json

funcName= inference 2	3	EndOfFunc
BEGIN_LINE
0	18	3	1	-1	0
END_LINE
startport

2	0	coords= -44006 -42385 0	0	1	0	mandatory= 0	0	1
load

funcName= load_model 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44006 -42345 0	0	0	0	mandatory= 0	0	1
do

funcName= inference 2	0	EndOfFunc
BEGIN_LINE
1	128	0	2	-1	0
-44075	-42303
-44075	-42345
END_LINE
finishport

3	3	coords= -43915 -42385 0	0	0	1	mandatory= 0	0	1
--

funcName= load_model 1	4	EndOfFunc
BEGIN_LINE
0	18	0	3	-1	0
END_LINE
finishport

3	3	coords= -43915 -42375 0	1	0	1	mandatory= 0	0	1
err

funcName= load_model 1	5	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
finishport

3	3	coords= -43915 -42345 0	2	0	1	mandatory= 0	0	1
--

funcName= inference 2	4	EndOfFunc
BEGIN_LINE
0	266	1	3	-1	0
END_LINE
finishport

3	1	coords= -43915 -42335 0	3	0	1	mandatory= 0	0	1
err

funcName= inference 2	5	EndOfFunc
BEGIN_LINE
0	20	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

camera
_
10	0	0	0
_
_
Run machine learning camera
Camera
0
_
-44615	-42475	-44505	-42335
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
fps	1	0	1000	0	0	0	0	1.2	5
EndOfValues
EndOfLabels
frame rate

parameter
width	1	0	10000	0	0	0	0	1.2	800
EndOfValues
EndOfLabels
width

parameter
height	1	0	10000	0	0	0	0	1.2	600
EndOfValues
EndOfLabels
height

parameter
asynchronous	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
camera outputs frames without capture event

parameter
show_frame	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Display the image with given frame

parameter
greyscale	0	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Make the camera output greyscale. Note that this has limited support on various devices. Devices with libcamera support generally support this feature.

parameter
OpenCL Acceleration	1	0	1	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
OpenCL GPU acceleration for MV pipeline operations: 0=disabled (cv::Mat, CPU default), 1=enabled (cv::UMat, OpenCL). When enabled, resize/crop/convert run on the OpenCL GPU. Call EhsCameraFrameEnsureCPU before ML inference (TFLite/Hailo). Not supported on Raspberry Pi VideoCore.

inputport

0	3	coords= -44616 -42438 0	0	0	1	mandatory= 0	0	1
id

funcName= startCamera 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44616 -42428 0	1	0	1	mandatory= 0	0	1
OpenCL: 0=off 1=on

funcName= startCamera 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44616 -42395 0	2	0	1	mandatory= 0	0	1
show

funcName= grabFrame 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -44505 -42428 0	0	0	1	mandatory= 0	0	1
errno

funcName= startCamera 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44505 -42390 0	1	0	1	mandatory= 0	0	1
width

funcName= grabFrame 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44505 -42380 0	2	0	1	mandatory= 0	0	1
height

funcName= grabFrame 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44505 -42400 0	3	0	1	mandatory= 0	0	1
stream_id

funcName= grabFrame 3	4	EndOfFunc
BEGIN_LINE
0	86	0	1	-1	0
END_LINE
startport

2	3	coords= -44616 -42448 0	0	1	0	mandatory= 0	0	1
start

funcName= startCamera 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44616 -42405 0	1	0	0	mandatory= 0	0	1
capture

funcName= grabFrame 3	0	EndOfFunc
BEGIN_LINE
1	14	0	2	-1	0
-44674	-42410
-44674	-42405
END_LINE
startport

2	1	coords= -44616 -42363 0	2	0	0	mandatory= 0	0	1
stop

funcName= stopCamera 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -44505 -42448 0	0	0	1	mandatory= 0	0	1
--

funcName= startCamera 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -44505 -42438 0	1	0	1	mandatory= 0	0	1
err

funcName= startCamera 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44505 -42410 0	2	0	1	mandatory= 0	0	1
--

funcName= grabFrame 3	5	EndOfFunc
BEGIN_LINE
0	250	0	3	-1	0
END_LINE
finishport

3	1	coords= -44505 -42363 0	3	0	1	mandatory= 0	0	1
--

funcName= stopCamera 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
14	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
-44765	-42465	-44700	-42375
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
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	500000
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

0	1	coords= -44766 -42430 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44766 -42420 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44766 -42410 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= -44766 -42440 0	0	1	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44766 -42390 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -44700 -42410 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	250	0	3	-1	0
END_LINE
finishport

3	0	coords= -44700 -42440 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= -44700 -42390 0	2	0	1	mandatory= 0	0	1


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

const_s1
_
16	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
-44310	-42410	-44170	-42389
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
String Constant	3	0	100	1	5	4	0	1.2	coco640f16.tfl
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= -44170 -42399 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	8	0	1	-1	0
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
18	0	0	0
_
_
This is a 4-input string multiplexer.
MultiplexFourInputString
0
_
-43730	-42425	-43700	-42305
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

0	3	coords= -43731 -42390 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -43731 -42365 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	8	1	0	-1	0
-43831	-42355
-43831	-42365
END_LINE
inputport

0	3	coords= -43731 -42340 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -43731 -42315 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	8	3	0	-1	0
-43815	-42315
-43815	-42315
END_LINE
outputport

1	3	coords= -43700 -42390 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= -43731 -42400 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	8	0	2	-1	0
-43839	-42385
-43839	-42400
END_LINE
startport

2	3	coords= -43731 -42375 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	8	1	2	-1	0
-43831	-42375
-43831	-42375
END_LINE
startport

2	3	coords= -43731 -42350 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	8	2	2	-1	0
-43823	-42345
-43823	-42350
END_LINE
startport

2	3	coords= -43731 -42325 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -43700 -42400 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_4i
_
20	0	0	0
_
_
This is a 4-input integer multiplexer.
MultiplexFourInputInt
0
_
-43730	-42295	-43700	-42175
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x2133
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= -43731 -42260 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
-43759	-42365
-43759	-42260
END_LINE
inputport

0	1	coords= -43731 -42235 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	8	2	0	-1	0
-43887	-42325
-43887	-42235
END_LINE
inputport

0	1	coords= -43731 -42210 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43731 -42185 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43700 -42260 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= -43731 -42270 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	8	1	2	-1	0
-43767	-42375
-43767	-42270
END_LINE
startport

2	1	coords= -43731 -42245 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	8	3	2	-1	0
-43783	-42335
-43783	-42245
END_LINE
startport

2	1	coords= -43731 -42220 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -43731 -42195 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -43700 -42270 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

frame_show
_
86	0	0	0
_
_
Show the frame in a pop window with given frame ID input.
frame_show
0
_
-44410	-42530	-44300	-42480
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
window_title	3	0	0	0	0	0	0	1.2	frame
EndOfValues
EndOfLabels
Title of the pop window.

inputport

0	1	coords= -44411 -42495 0	0	0	1	mandatory= 1	0	1
frame id

funcName= show 1	1	EndOfFunc
BEGIN_LINE
1	10	3	0	-1	0
-44485	-42400
-44485	-42495
END_LINE
startport

2	3	coords= -44411 -42505 0	0	0	0	mandatory= 0	0	1
show

funcName= show 1	0	EndOfFunc
BEGIN_LINE
1	10	2	2	-1	0
-44490	-42410
-44490	-42505
END_LINE
finishport

3	3	coords= -44300 -42505 0	0	0	1	mandatory= 0	0	1
--

funcName= show 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44300 -42495 0	1	0	1	mandatory= 0	0	1
error

funcName= show 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

frame_resize
_
106	0	0	0
_
_
Resize camera frame
FrameResize
0
_
-44460	-42325	-44350	-42225
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	Frame_Resize
trans	-1	0	0	0
offset	0	-15
hash	0xE4D7
Instance_Info	_
End_Instance
END_BA
parameter
size_x	1	0	10000	0	0	0	0	1.2	640
EndOfValues
EndOfLabels
Resize frame width

parameter
size_y	1	0	10000	0	0	0	0	1.2	640
EndOfValues
EndOfLabels
Resize frame height

parameter
Interpolation	1	0	3	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Interpolation method: 0=Nearest (fastest, ML), 1=Linear (default), 2=Cubic (best quality), 3=Area (best for shrinking)

inputport

0	1	coords= -44461 -42288 0	1	0	1	mandatory= 0	0	1
stream_id

funcName= resize 1	1	EndOfFunc
BEGIN_LINE
1	10	3	0	-1	0
-44485	-42400
-44485	-42288
END_LINE
inputport

0	1	coords= -44461 -42260 0	1	0	1	mandatory= 0	0	1
size_x

funcName= resize 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44461 -42250 0	2	0	1	mandatory= 0	0	1
size_y

funcName= resize 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -44350 -42258 0	0	0	1	mandatory= 0	0	1
errno

funcName= resize 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44350 -42288 0	1	0	1	mandatory= 0	0	1
stream_id

funcName= resize 1	5	EndOfFunc
BEGIN_LINE
0	8	1	1	-1	0
END_LINE
startport

2	3	coords= -44461 -42298 0	1	0	0	mandatory= 0	0	1
run

funcName= resize 1	0	EndOfFunc
BEGIN_LINE
1	10	2	2	-1	0
-44480	-42410
-44480	-42298
END_LINE
finishport

3	1	coords= -44350 -42298 0	0	0	1	mandatory= 0	0	1
--

funcName= resize 1	6	EndOfFunc
BEGIN_LINE
0	250	1	3	-1	0
END_LINE
finishport

3	1	coords= -44350 -42268 0	1	0	1	mandatory= 0	0	1
err

funcName= resize 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

frame_fmt
_
128	0	0	0
_
_
Modifies camera frame format e.g. int8_3 to float32_3
FrameFormatter
0
_
-44205	-42330	-44095	-42230
1	0
blockattr
BEGIN_BA
type	Undefined
text	25	5	1.25	0	Frame_Fmt
trans	-1	0	0	0
offset	0	-15
hash	0x521F
Instance_Info	_
End_Instance
END_BA
parameter
format	1	0	10	0	0	0	0	1.2	3
EndOfValues
EndOfLabels
format data type

inputport

0	1	coords= -44206 -42293 0	0	0	1	mandatory= 0	0	1
stream_id

funcName= format 1	1	EndOfFunc
BEGIN_LINE
1	106	1	0	-1	0
-44342	-42288
-44342	-42293
END_LINE
inputport

0	1	coords= -44206 -42265 0	1	0	1	mandatory= 0	0	1
format

funcName= format 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -44095 -42263 0	0	0	1	mandatory= 0	0	1
errno

funcName= format 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44095 -42293 0	1	0	1	mandatory= 0	0	1
stream_id

funcName= format 1	4	EndOfFunc
BEGIN_LINE
0	8	1	1	-1	0
END_LINE
startport

2	1	coords= -44206 -42303 0	0	0	0	mandatory= 0	0	1
run

funcName= format 1	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	0
-44342	-42298
-44342	-42303
END_LINE
finishport

3	1	coords= -44095 -42303 0	0	0	1	mandatory= 0	0	1
--

funcName= format 1	5	EndOfFunc
BEGIN_LINE
0	266	1	3	-1	0
END_LINE
finishport

3	1	coords= -44095 -42273 0	1	0	1	mandatory= 0	0	1
err

funcName= format 1	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

elapsed_timer
_
250	0	0	0
_
_
This returns the elapsed time in microseconds between two event triggers.
elapsed_timer
0
_
-44240	-42535	-44170	-42465
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	15	5	1.25	0	Elapsed
trans	-1	0	0	0
offset	0	-15
hash	0x90EC
Instance_Info	_
End_Instance
END_BA
outputport

1	1	coords= -44170 -42480 0	0	0	1	mandatory= 0	0	1
elapsed

funcName= stop_elapsed 2	1	EndOfFunc
BEGIN_LINE
0	285	5	1	-1	0
END_LINE
startport

2	-1	coords= -44241 -42510 0	0	0	0	mandatory= 0	0	1
start

funcName= count_elasped 1	0	EndOfFunc
BEGIN_LINE
1	10	2	2	-1	0
-44269	-42410
-44269	-42510
END_LINE
startport

2	-1	coords= -44241 -42490 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop_elapsed 2	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	0
-44277	-42298
-44277	-42490
END_LINE
finishport

3	-1	coords= -44170 -42510 0	0	0	1	mandatory= 0	0	1
--

funcName= count_elasped 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -44170 -42490 0	1	0	1	mandatory= 0	0	1
--

funcName= stop_elapsed 2	2	EndOfFunc
BEGIN_LINE
0	285	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

elapsed_timer
_
266	0	0	0
_
_
This returns the elapsed time in microseconds between two event triggers.
elapsed_timer
0
_
-43855	-42520	-43785	-42450
1	0
blockattr
BEGIN_BA
type	Event_Processor
text	15	5	1.25	0	Elapsed
trans	-1	0	0	0
offset	0	-15
hash	0x90EC
Instance_Info	_
End_Instance
END_BA
outputport

1	1	coords= -43785 -42465 0	0	0	1	mandatory= 0	0	1
elapsed

funcName= stop_elapsed 2	1	EndOfFunc
BEGIN_LINE
0	267	5	1	-1	0
END_LINE
startport

2	-1	coords= -43856 -42495 0	0	0	0	mandatory= 0	0	1
start

funcName= count_elasped 1	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	0
-43884	-42298
-43884	-42495
END_LINE
startport

2	-1	coords= -43856 -42475 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop_elapsed 2	0	EndOfFunc
BEGIN_LINE
1	128	0	2	-1	0
-43892	-42303
-43892	-42475
END_LINE
finishport

3	-1	coords= -43785 -42495 0	0	0	1	mandatory= 0	0	1
--

funcName= count_elasped 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -43785 -42475 0	1	0	1	mandatory= 0	0	1
--

funcName= stop_elapsed 2	2	EndOfFunc
BEGIN_LINE
0	267	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
267	0	0	0
_
_
This widget displays an integer to the GUI output. It can also be configured as Slider, Progress Bar or Gauge via iGB.
gui_text_int2
0
_
-43640	-42530	-43525	-42315
1	0
blockattr
BEGIN_BA
type	IO
text	22	5	1.25	0	Integer_UI
trans	-1	0	0	0
offset	0	-15
hash	0x4709
Instance_Info	_
End_Instance
END_BA
parameter
Widget Tag	5	0	0	1	40	200	0	1.2	MLTime
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	0	0	0	0	0	0	1.2	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Always Clickable	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	coords= -43641 -42373 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43641 -42363 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43641 -42353 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43641 -42343 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43641 -42333 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43641 -42413 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	266	0	0	-1	1
-43725	-42465
-43725	-42413
END_LINE
inputport

0	3	coords= -43641 -42403 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43525 -42373 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43525 -42363 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43525 -42353 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43525 -42343 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43525 -42413 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -43525 -42403 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -43641 -42505 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43641 -42495 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43641 -42480 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43641 -42470 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43641 -42385 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -43641 -42425 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	266	1	2	-1	0
-43725	-42475
-43725	-42425
END_LINE
finishport

3	1	coords= -43525 -42505 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42495 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42480 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42470 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42385 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42455 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42445 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43525 -42425 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -43525 -42435 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
285	0	0	0
_
_
This widget displays an integer to the GUI output. It can also be configured as Slider, Progress Bar or Gauge via iGB.
gui_text_int2
0
_
-44075	-42650	-43960	-42435
1	0
blockattr
BEGIN_BA
type	IO
text	22	5	1.25	0	Integer_UI
trans	-1	0	0	0
offset	0	-15
hash	0x4709
Instance_Info	_
End_Instance
END_BA
parameter
Widget Tag	5	0	0	1	40	200	0	1.2	preprocesstime
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	0	0	0	0	0	0	1.2	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Always Clickable	0	0	0	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	coords= -44076 -42493 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44076 -42483 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44076 -42473 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44076 -42463 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44076 -42453 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44076 -42533 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	250	0	0	-1	0
-44110	-42480
-44110	-42533
END_LINE
inputport

0	3	coords= -44076 -42523 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43960 -42493 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43960 -42483 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43960 -42473 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43960 -42463 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43960 -42533 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -43960 -42523 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -44076 -42625 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44076 -42615 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44076 -42600 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44076 -42590 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44076 -42505 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44076 -42545 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	250	1	2	-1	0
-44110	-42490
-44110	-42545
END_LINE
finishport

3	1	coords= -43960 -42625 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42615 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42600 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42590 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42505 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42575 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42565 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43960 -42545 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -43960 -42555 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
