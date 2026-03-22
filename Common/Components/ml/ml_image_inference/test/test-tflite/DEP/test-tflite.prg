#V:2.1.2

CanvasSizeXY
43335	41500

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
-43875	-42420	-43785	-42295
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
Conf Thres	2	0	1	0	0	0	0	1.2	0.4
EndOfValues
EndOfLabels
The confidence Threshold for the output data. The range is from 0 to 1.

parameter
Thread Number	1	0	16	0	0	0	0	1.2	2
EndOfValues
EndOfLabels
The number of threads for inference. If the platform does not support threading, this parameter will be ignored.

parameter
Use Application Dir	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Whether to use application directory as the root of model path. If not, the User directory will be used.

inputport

0	3	coords= -43876 -42380 0	0	0	1	mandatory= 0	0	1
path

funcName= load_model 1	1	EndOfFunc
BEGIN_LINE
1	16	0	0	-1	0
-44039	-42399
-44039	-42380
END_LINE
inputport

0	1	coords= -43876 -42340 0	0	0	1	mandatory= 0	0	1
stream_id

funcName= inference 2	1	EndOfFunc
BEGIN_LINE
1	128	1	0	-1	1
-43915	-42293
-43915	-42340
END_LINE
outputport

1	1	coords= -43785 -42370 0	0	0	1	mandatory= 0	0	1
errno

funcName= load_model 1	2	EndOfFunc
BEGIN_LINE
0	20	0	1	-1	0
END_LINE
outputport

1	3	coords= -43785 -42360 0	1	0	1	mandatory= 0	0	1
model info

funcName= load_model 1	3	EndOfFunc
BEGIN_LINE
0	18	1	1	-1	0
END_LINE
outputport

1	1	coords= -43785 -42330 0	2	0	1	mandatory= 0	0	1
errno

funcName= inference 2	2	EndOfFunc
BEGIN_LINE
0	20	1	1	-1	0
END_LINE
outputport

1	3	coords= -43785 -42320 0	3	0	1	mandatory= 0	0	1
json

funcName= inference 2	3	EndOfFunc
BEGIN_LINE
0	1346	5	1	-1	0
END_LINE
startport

2	0	coords= -43876 -42390 0	0	1	0	mandatory= 0	0	1
load

funcName= load_model 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= -43876 -42350 0	0	0	0	mandatory= 0	0	1
do

funcName= inference 2	0	EndOfFunc
BEGIN_LINE
1	896	0	2	-1	1
-43948	-42334
-43948	-42350
END_LINE
finishport

3	3	coords= -43785 -42390 0	0	0	1	mandatory= 0	0	1
--

funcName= load_model 1	4	EndOfFunc
BEGIN_LINE
0	18	0	3	-1	0
END_LINE
finishport

3	3	coords= -43785 -42380 0	1	0	1	mandatory= 0	0	1
err

funcName= load_model 1	5	EndOfFunc
BEGIN_LINE
0	20	0	3	-1	0
END_LINE
finishport

3	3	coords= -43785 -42350 0	2	0	1	mandatory= 0	0	1
--

funcName= inference 2	4	EndOfFunc
BEGIN_LINE
0	1346	5	3	-1	0
END_LINE
finishport

3	1	coords= -43785 -42340 0	3	0	1	mandatory= 0	0	1
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
-44665	-42465	-44555	-42325
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
fps	1	0	1000	0	0	0	0	1.2	10
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
show_frame	0	0	1	0	0	0	0	1.2	0
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

0	3	coords= -44666 -42428 0	0	0	1	mandatory= 0	0	1
id

funcName= startCamera 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44666 -42418 0	1	0	1	mandatory= 0	0	1
OpenCL: 0=off 1=on

funcName= startCamera 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44666 -42385 0	2	0	1	mandatory= 0	0	1
show

funcName= grabFrame 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -44555 -42418 0	0	0	1	mandatory= 0	0	1
errno

funcName= startCamera 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44555 -42380 0	1	0	1	mandatory= 0	0	1
width

funcName= grabFrame 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44555 -42370 0	2	0	1	mandatory= 0	0	1
height

funcName= grabFrame 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44555 -42390 0	3	0	1	mandatory= 0	0	1
stream_id

funcName= grabFrame 3	4	EndOfFunc
BEGIN_LINE
0	86	0	1	-1	0
END_LINE
startport

2	3	coords= -44666 -42438 0	0	1	0	mandatory= 0	0	1
start

funcName= startCamera 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44666 -42395 0	1	0	0	mandatory= 0	0	1
capture

funcName= grabFrame 3	0	EndOfFunc
BEGIN_LINE
1	14	0	2	-1	1
-44724	-42400
-44724	-42395
END_LINE
startport

2	1	coords= -44666 -42353 0	2	0	0	mandatory= 0	0	1
stop

funcName= stopCamera 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -44555 -42438 0	0	0	1	mandatory= 0	0	1
--

funcName= startCamera 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -44555 -42428 0	1	0	1	mandatory= 0	0	1
err

funcName= startCamera 1	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44555 -42400 0	2	0	1	mandatory= 0	0	1
--

funcName= grabFrame 3	5	EndOfFunc
BEGIN_LINE
0	250	0	3	-1	0
END_LINE
finishport

3	1	coords= -44555 -42353 0	3	0	1	mandatory= 0	0	1
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
-44815	-42455	-44750	-42365
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

0	1	coords= -44816 -42420 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44816 -42410 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44816 -42400 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= -44816 -42430 0	0	1	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44816 -42380 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -44750 -42400 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	10	1	3	-1	0
END_LINE
finishport

3	0	coords= -44750 -42430 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= -44750 -42380 0	2	0	1	mandatory= 0	0	1


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
-43575	-42395	-43545	-42275
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

0	3	coords= -43576 -42360 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -43576 -42335 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	8	1	0	-1	1
-43604	-42360
-43604	-42335
END_LINE
inputport

0	3	coords= -43576 -42310 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -43576 -42285 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
1	8	3	0	-1	1
-43650	-42320
-43650	-42285
END_LINE
outputport

1	3	coords= -43545 -42360 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	3	coords= -43576 -42370 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	8	0	2	-1	0
-43620	-42390
-43620	-42370
END_LINE
startport

2	3	coords= -43576 -42345 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	8	1	2	-1	0
-43628	-42380
-43628	-42345
END_LINE
startport

2	3	coords= -43576 -42320 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	2	2	-1	1
END_LINE
startport

2	3	coords= -43576 -42295 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= -43545 -42370 0	0	0	1	mandatory= 0	0	1


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
-43575	-42265	-43545	-42145
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

0	1	coords= -43576 -42230 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
-43635	-42370
-43635	-42230
END_LINE
inputport

0	1	coords= -43576 -42205 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	8	2	0	-1	1
-43655	-42330
-43655	-42205
END_LINE
inputport

0	1	coords= -43576 -42180 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43576 -42155 0	3	0	1	mandatory= 0	0	1
i4

funcName= Run_Input4 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43545 -42230 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	funcName= Run_Input4 4	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= -43576 -42240 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	8	1	2	-1	0
-43620	-42380
-43620	-42240
END_LINE
startport

2	1	coords= -43576 -42215 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	8	3	2	-1	1
-43645	-42340
-43645	-42215
END_LINE
startport

2	1	coords= -43576 -42190 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -43576 -42165 0	3	0	0	mandatory= 0	0	1


funcName= Run_Input4 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	1	coords= -43545 -42240 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	funcName= Run_Input4 4	3	EndOfFunc
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
1	10	3	0	-1	1
-44485	-42390
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

2	-1	coords= -44461 -42298 0	0	0	0	mandatory= 0	0	1
run

funcName= resize 1	0	EndOfFunc
BEGIN_LINE
1	1010	0	2	-1	0
-44353	-42374
-44353	-42435
-44541	-42435
-44541	-42298
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
1	106	1	0	-1	1
-44294	-42288
-44294	-42293
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
0	86	0	1	-1	0
END_LINE
startport

2	-1	coords= -44206 -42303 0	1	0	0	mandatory= 0	0	1
run

funcName= format 1	0	EndOfFunc
BEGIN_LINE
1	1036	0	2	-1	0
-44230	-42339
-44230	-42303
END_LINE
finishport

3	1	coords= -44095 -42303 0	0	0	1	mandatory= 0	0	1
--

funcName= format 1	5	EndOfFunc
BEGIN_LINE
0	86	0	3	-1	0
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
-44420	-42025	-44350	-41955
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

1	1	coords= -44350 -41970 0	0	0	1	mandatory= 0	0	1
elapsed

funcName= stop_elapsed 2	1	EndOfFunc
BEGIN_LINE
0	285	5	1	-1	0
END_LINE
startport

2	-1	coords= -44421 -42000 0	0	0	0	mandatory= 0	0	1
start

funcName= count_elasped 1	0	EndOfFunc
BEGIN_LINE
1	10	2	2	-1	1
-44520	-42400
-44520	-42000
END_LINE
startport

2	-1	coords= -44421 -41980 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop_elapsed 2	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	1
-44315	-42298
-44315	-42053
-44435	-42053
-44435	-41980
END_LINE
finishport

3	-1	coords= -44350 -42000 0	0	0	1	mandatory= 0	0	1
--

funcName= count_elasped 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -44350 -41980 0	1	0	1	mandatory= 0	0	1
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
-44190	-42180	-44120	-42110
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

1	1	coords= -44120 -42125 0	0	0	1	mandatory= 0	0	1
elapsed

funcName= stop_elapsed 2	1	EndOfFunc
BEGIN_LINE
0	267	5	1	-1	0
END_LINE
startport

2	-1	coords= -44191 -42155 0	0	0	0	mandatory= 0	0	1
start

funcName= count_elasped 1	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	1
-44315	-42298
-44315	-42155
END_LINE
startport

2	-1	coords= -44191 -42135 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop_elapsed 2	0	EndOfFunc
BEGIN_LINE
1	128	0	2	-1	1
-43999	-42303
-43999	-42195
-44230	-42195
-44230	-42135
END_LINE
finishport

3	-1	coords= -44120 -42155 0	0	0	1	mandatory= 0	0	1
--

funcName= count_elasped 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -44120 -42135 0	1	0	1	mandatory= 0	0	1
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
-44025	-41965	-43910	-41750
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
Widget Tag	5	0	0	1	40	200	0	1.2	pixelFormar
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

0	1	coords= -44026 -41808 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44026 -41798 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44026 -41788 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44026 -41778 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44026 -41768 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44026 -41848 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	266	0	0	-1	1
-44080	-42125
-44080	-41848
END_LINE
inputport

0	3	coords= -44026 -41838 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43910 -41808 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43910 -41798 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43910 -41788 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43910 -41778 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43910 -41848 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -43910 -41838 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -44026 -41940 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44026 -41930 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44026 -41915 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44026 -41905 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44026 -41820 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44026 -41860 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	266	1	2	-1	0
-44070	-42135
-44070	-41860
END_LINE
finishport

3	1	coords= -43910 -41940 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41930 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41915 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41905 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41820 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41890 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41880 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43910 -41860 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -43910 -41870 0	8	0	1	mandatory= 0	0	1
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
-44295	-41985	-44180	-41770
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

0	1	coords= -44296 -41828 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44296 -41818 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44296 -41808 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44296 -41798 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44296 -41788 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44296 -41868 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	250	0	0	-1	1
-44324	-41970
-44324	-41868
END_LINE
inputport

0	3	coords= -44296 -41858 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -44180 -41828 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44180 -41818 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44180 -41808 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44180 -41798 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44180 -41868 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -44180 -41858 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -44296 -41960 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44296 -41950 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44296 -41935 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44296 -41925 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44296 -41840 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44296 -41880 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	250	1	2	-1	0
-44315	-41980
-44315	-41880
END_LINE
finishport

3	1	coords= -44180 -41960 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41950 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41935 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41925 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41840 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41910 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41900 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44180 -41880 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -44180 -41890 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

elapsed_timer
_
442	0	0	0
_
_
This returns the elapsed time in microseconds between two event triggers.
elapsed_timer
0
_
-44410	-42670	-44340	-42600
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

1	1	coords= -44340 -42615 0	0	0	1	mandatory= 0	0	1
elapsed

funcName= stop_elapsed 2	1	EndOfFunc
BEGIN_LINE
0	483	5	1	-1	0
END_LINE
startport

2	-1	coords= -44411 -42645 0	0	0	0	mandatory= 0	0	1
start

funcName= count_elasped 1	0	EndOfFunc
BEGIN_LINE
1	483	7	2	-1	0
-44070	-42685
-44070	-42820
-44445	-42820
-44445	-42645
END_LINE
startport

2	-1	coords= -44411 -42625 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop_elapsed 2	0	EndOfFunc
BEGIN_LINE
1	462	0	2	-1	0
-44455	-42670
-44455	-42625
END_LINE
finishport

3	-1	coords= -44340 -42645 0	0	0	1	mandatory= 0	0	1
--

funcName= count_elasped 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -44340 -42625 0	1	0	1	mandatory= 0	0	1
--

funcName= stop_elapsed 2	2	EndOfFunc
BEGIN_LINE
0	483	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
462	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
-44565	-42725	-44500	-42635
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
Time Interval (uSecs)	1	1	2147483647	0	0	0	0	1.2	25000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE. 

parameter
Retriggerable Flag	0	_	_	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Retriggerable if TRUE. 

inputport

0	1	coords= -44566 -42690 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44566 -42680 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= -44566 -42670 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	-1	coords= -44566 -42700 0	0	0	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
1	541	0	2	-1	0
-44613	-42664
-44613	-42700
END_LINE
startport

2	0	coords= -44566 -42650 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= -44500 -42670 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	442	1	3	-1	0
END_LINE
finishport

3	0	coords= -44500 -42700 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= -44500 -42650 0	2	0	1	mandatory= 0	0	1


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

gui_text_int2
_
483	0	0	0
_
_
This widget displays an integer to the GUI output. It can also be configured as Slider, Progress Bar or Gauge via iGB.
gui_text_int2
0
_
-44255	-42790	-44140	-42575
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
Widget Tag	5	0	0	1	40	200	0	1.2	EHSJitter
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

0	1	coords= -44256 -42633 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44256 -42623 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44256 -42613 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44256 -42603 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44256 -42593 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -44256 -42673 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	442	0	0	-1	0
-44280	-42615
-44280	-42673
END_LINE
inputport

0	3	coords= -44256 -42663 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -44140 -42633 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44140 -42623 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44140 -42613 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44140 -42603 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -44140 -42673 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -44140 -42663 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -44256 -42765 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44256 -42755 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44256 -42740 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44256 -42730 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -44256 -42645 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -44256 -42685 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	442	1	2	-1	0
-44280	-42625
-44280	-42685
END_LINE
finishport

3	1	coords= -44140 -42765 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42755 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42740 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42730 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42645 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42715 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42705 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -44140 -42685 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	442	0	3	-1	0
END_LINE
finishport

3	3	coords= -44140 -42695 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
541	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-44645	-42685	-44625	-42645
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

2	0	coords= -44646 -42675 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	483	7	2	-1	0
-44068	-42685
-44068	-42822
-44695	-42822
-44695	-42675
END_LINE
startport

2	-1	coords= -44646 -42655 0	1	1	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= -44633 -42664 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	462	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

elapsed_timer
_
831	0	0	0
_
_
This returns the elapsed time in microseconds between two event triggers.
elapsed_timer
0
_
-43965	-42070	-43895	-42000
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

1	1	coords= -43895 -42015 0	0	0	1	mandatory= 0	0	1
elapsed

funcName= stop_elapsed 2	1	EndOfFunc
BEGIN_LINE
0	483	5	1	-1	0
END_LINE
startport

2	1	coords= -43966 -42045 0	0	0	0	mandatory= 0	0	1
start

funcName= count_elasped 1	0	EndOfFunc
BEGIN_LINE
1	128	0	2	-1	1
-44000	-42303
-44000	-42045
END_LINE
startport

2	-1	coords= -43966 -42025 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop_elapsed 2	0	EndOfFunc
BEGIN_LINE
1	8	2	2	-1	1
-43760	-42350
-43760	-42158
-44010	-42158
-44010	-42025
END_LINE
finishport

3	-1	coords= -43895 -42045 0	0	0	1	mandatory= 0	0	1
--

funcName= count_elasped 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= -43895 -42025 0	1	0	1	mandatory= 0	0	1
--

funcName= stop_elapsed 2	2	EndOfFunc
BEGIN_LINE
0	483	5	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_int2
_
832	0	0	0
_
_
This widget displays an integer to the GUI output. It can also be configured as Slider, Progress Bar or Gauge via iGB.
gui_text_int2
0
_
-43815	-42080	-43700	-41865
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
Widget Tag	5	0	0	1	40	200	0	1.2	InferenceTime
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

0	1	coords= -43816 -41923 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43816 -41913 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43816 -41903 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43816 -41893 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43816 -41883 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43816 -41963 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	831	0	0	-1	1
-43875	-42015
-43875	-41963
END_LINE
inputport

0	3	coords= -43816 -41953 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43700 -41923 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43700 -41913 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43700 -41903 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43700 -41893 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43700 -41963 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -43700 -41953 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -43816 -42055 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43816 -42045 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43816 -42030 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43816 -42020 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43816 -41935 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -43816 -41975 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	831	1	2	-1	0
-43867	-42025
-43867	-41975
END_LINE
finishport

3	1	coords= -43700 -42055 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -42045 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -42030 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -42020 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -41935 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -42005 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -41995 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43700 -41975 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	442	0	3	-1	0
END_LINE
finishport

3	3	coords= -43700 -41985 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
896	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-43980	-42355	-43960	-42315
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

2	-1	coords= -43981 -42345 0	0	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	coords= -43981 -42325 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	128	0	2	-1	0
-43990	-42303
-43990	-42325
END_LINE
finishport

3	-1	coords= -43968 -42334 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	8	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
1010	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-44445	-42395	-44425	-42355
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

2	3	coords= -44446 -42385 0	1	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
1	10	2	2	-1	1
-44465	-42400
-44465	-42385
END_LINE
startport

2	-1	coords= -44446 -42365 0	1	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
0	128	0	2	-1	0
END_LINE
finishport

3	-1	coords= -44433 -42374 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	106	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
1036	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
-44270	-42360	-44250	-42320
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

2	3	coords= -44271 -42350 0	1	0	0	mandatory= 0	0	1


funcName= Or1 1	0	EndOfFunc
BEGIN_LINE
0	10	2	2	-1	1
END_LINE
startport

2	1	coords= -44271 -42330 0	0	0	0	mandatory= 0	0	1


funcName= Or2 2	0	EndOfFunc
BEGIN_LINE
1	106	0	2	-1	1
-44315	-42298
-44315	-42330
END_LINE
finishport

3	-1	coords= -44258 -42339 0	0	0	1	mandatory= 0	0	1


funcName= Or1 1	1	funcName= Or2 2	1	EndOfFunc
BEGIN_LINE
0	128	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
1346	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
-43480	-42550	-43365	-42335
1	0
blockattr
BEGIN_BA
type	IO
text	22	5	1.25	0	Text_UI
trans	-1	0	0	0
offset	0	-15
hash	0x150B
Instance_Info	_
End_Instance
END_BA
parameter
Widget Tag	5	0	0	1	40	200	0	1.2	Detections
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

0	1	coords= -43481 -42393 0	0	0	1	mandatory= 0	0	1
x offset

funcName= update 5	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43481 -42383 0	1	0	1	mandatory= 0	0	1
y offset

funcName= update 5	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43481 -42373 0	2	0	1	mandatory= 0	0	1
width

funcName= update 5	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43481 -42363 0	3	0	1	mandatory= 0	0	1
height

funcName= update 5	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	coords= -43481 -42353 0	4	0	1	mandatory= 0	0	1
alpha

funcName= update 5	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= -43481 -42433 0	5	0	1	mandatory= 0	0	1
data

funcName= data 6	1	EndOfFunc
BEGIN_LINE
1	8	3	0	-1	0
-43650	-42320
-43650	-42433
END_LINE
inputport

0	3	coords= -43481 -42423 0	6	0	1	mandatory= 0	0	1
label

funcName= data 6	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= -43365 -42393 0	0	0	1	mandatory= 0	0	1
abs x

funcName= create 1	1	funcName= update 5	6	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43365 -42383 0	1	0	1	mandatory= 0	0	1
abs y

funcName= create 1	2	funcName= update 5	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43365 -42373 0	2	0	1	mandatory= 0	0	1
wid

funcName= create 1	3	funcName= update 5	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	coords= -43365 -42363 0	3	0	1	mandatory= 0	0	1
ht

funcName= create 1	4	funcName= update 5	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -43365 -42433 0	4	0	1	mandatory= 0	0	1
data

funcName= create 1	5	funcName= data 6	3	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	coords= -43365 -42423 0	5	0	1	mandatory= 0	0	1
label

funcName= create 1	6	funcName= data 6	4	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	0	coords= -43481 -42525 0	0	1	0	mandatory= 1	0	1
create

funcName= create 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43481 -42515 0	1	0	0	mandatory= 0	0	1
destroy

funcName= destroy 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43481 -42500 0	2	1	0	mandatory= 0	0	1
show

funcName= show 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43481 -42490 0	3	0	0	mandatory= 0	0	1
hide

funcName= hide 4	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	coords= -43481 -42405 0	4	0	0	mandatory= 0	0	1
update

funcName= update 5	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= -43481 -42445 0	5	0	0	mandatory= 0	0	1
update

funcName= data 6	0	EndOfFunc
BEGIN_LINE
1	8	2	2	-1	0
-43765	-42350
-43765	-42445
END_LINE
finishport

3	1	coords= -43365 -42525 0	0	0	1	mandatory= 0	0	1
--

funcName= create 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42515 0	1	0	1	mandatory= 0	0	1
--

funcName= destroy 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42500 0	2	0	1	mandatory= 0	0	1
--

funcName= show 3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42490 0	3	0	1	mandatory= 0	0	1
--

funcName= hide 4	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42405 0	4	0	1	mandatory= 0	0	1
--

funcName= update 5	10	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42475 0	5	0	1	mandatory= 0	0	1
click

funcName= create 1	8	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42465 0	6	0	1	mandatory= 0	0	1
mouse down

funcName= create 1	9	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= -43365 -42445 0	7	0	1	mandatory= 0	0	1
--

funcName= create 1	10	funcName= data 6	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= -43365 -42455 0	8	0	1	mandatory= 0	0	1
data change

funcName= create 1	11	funcName= data 6	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

xcomment
_
1444	0	0	0
_
_
<nl?>        
xcomment
0
_
-43885	-42905	-43358	-42762
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
Text	3	0	0	0	0	0	0	1.2	Machine Vision And Machine Learning Regression Test.\nRequires a camera capable fo 10FPS (e.g. USB).\n\nUsually tested on a raspberry Pi.\n\nOther test apps using various hardware accelerators should also be considered.
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
END_OF_BLOCKS
