#V:2.1.2

CanvasSizeXY
200	455

IconData
BEGIN_BLOCK

adc_config
_
218023	0	0	0
_
_
Configure the ADC unit either from parameters or JSON string
adc_config
0
_
910	-545	985	-470
1	0
blockattr
BEGIN_BA
type	IO
text	5	5	1.25	0	ADC_Config
trans	-1	0	0	0
offset	0	-15
hash	0x566F
Instance_Info	_
End_Instance
END_BA
parameter
device_id	1	0	9	1	30	20	0	1.2	0
EndOfValues
EndOfLabels
The ADC Device number to be configured. For internal ADCs, this is the unit. For external ADCs, this is the address enumeration.

parameter
f_s_hz	1	100	1000000000	0	0	0	0	1.2	1500
EndOfValues
EndOfLabels
Total sampling frequency in Hz. With n channels enabled, the sampling frequency of each channel is f_s_hz/n Hz.

parameter
mode	0	0	1	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
The ADC conversion mode. False for Single-shot. True for Continuous.

parameter
channel_0	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_1	1	-1	999	0	0	0	0	1.2	1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_2	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_3	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_4	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_5	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_6	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_7	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_8	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_9	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_10	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_11	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_12	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_13	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_14	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

parameter
channel_15	1	-1	999	0	0	0	0	1.2	-1
EndOfValues
EndOfLabels
Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.

inputport

0	3	coords= 909 -508 0	0	0	1	mandatory= 0	0	1
JSON

funcName= config 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	1	coords= 909 -518 0	0	1	0	mandatory= 0	0	1
init

funcName= config 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= 985 -518 0	0	0	1	mandatory= 0	0	1
--

funcName= config 1	2	EndOfFunc
BEGIN_LINE
0	218024	0	3	-1	0
END_LINE
finishport

3	3	coords= 985 -508 0	1	0	1	mandatory= 0	0	1
error

funcName= config 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_3i
_
218096	0	0	0
_
_
This is a 3-input integer multiplexer.
MultiplexThreeInputInt
0
_
1075	-340	1105	-245
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x7CFD
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 1074 -305 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	218097	0	0	-1	0
1034	-318
1044	-305
END_LINE
inputport

0	1	coords= 1074 -280 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
1	218097	1	0	-1	0
1034	-308
1034	-280
END_LINE
inputport

0	1	coords= 1074 -255 0	2	0	1	mandatory= 0	0	1
i3

funcName= Run_Input3 3	1	EndOfFunc
BEGIN_LINE
1	218097	2	0	-1	0
1034	-298
1034	-255
END_LINE
outputport

1	1	coords= 1105 -305 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	funcName= Run_Input3 3	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 1074 -315 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	218097	0	2	-1	0
1034	-328
1044	-315
END_LINE
startport

2	1	coords= 1074 -290 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	218097	1	2	-1	0
1024	-288
1034	-290
END_LINE
startport

2	1	coords= 1074 -265 0	2	0	0	mandatory= 0	0	1


funcName= Run_Input3 3	0	EndOfFunc
BEGIN_LINE
1	218097	4	2	-1	0
1034	-273
1044	-265
END_LINE
finishport

3	1	coords= 1105 -315 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	funcName= Run_Input3 3	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

adc_read_continuous
_
218097	0	0	0
_
_
Read value continuously as mean and mean squared values. The port and data are triggered only if the data is ready.
adc_read_continuous
0
_
910	-355	985	-220
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	ADC_Cont
trans	-1	0	0	0
offset	0	-15
hash	0xB20D
Instance_Info	_
End_Instance
END_BA
parameter
device_id	1	0	9	1	30	20	0	1.2	0
EndOfValues
EndOfLabels
The ADC Device number to be configured. For internal ADCs, this is the unit. For external ADCs, this is the address enumeration.

parameter
channel	1	0	15	1	30	40	0	1.2	1
EndOfValues
EndOfLabels
The ADC Channel ID. If it’s disabled, it will neither output anything nor trigger the output event.

parameter
decimation	1	1	1000000000	0	0	0	0	1.2	2500
EndOfValues
EndOfLabels
The ADC Decimation number. This means dividing the sampling frequency by this parameter. Minimum 1.

parameter
decimate_average	1	1	1000000000	0	0	0	0	1.2	2500
EndOfValues
EndOfLabels
The number of the last samples in the decimation period for average. It must be greater than 1 and less than the decimation parameter.

parameter
bias	2	-200000000	200000000	0	0	0	0	1.2	0
EndOfValues
EndOfLabels
The bias of the out need to be taken from the value

inputport

0	1	coords= 909 -263 0	0	0	1	mandatory= 0	0	1
MS bias

funcName= set_bias 4	1	EndOfFunc
BEGIN_LINE
1	218097	0	0	-1	0
1040	-318
1040	-400
855	-400
855	-263
END_LINE
outputport

1	1	coords= 985 -318 0	0	0	1	mandatory= 0	0	1
mean

funcName= convert 3	1	EndOfFunc
BEGIN_LINE
0	218097	0	1	-1	0
END_LINE
outputport

1	1	coords= 985 -308 0	1	0	1	mandatory= 0	0	1
var.

funcName= convert 3	2	EndOfFunc
BEGIN_LINE
0	218096	1	1	-1	0
END_LINE
outputport

1	1	coords= 985 -298 0	2	0	1	mandatory= 0	0	1
M.S.

funcName= convert 3	3	EndOfFunc
BEGIN_LINE
0	218096	2	1	-1	0
END_LINE
startport

2	2	coords= 909 -248 0	0	0	0	mandatory= 0	0	1
enable

funcName= enable 1	0	EndOfFunc
BEGIN_LINE
1	218023	0	2	-1	0
1015	-518
1015	-445
839	-445
839	-248
END_LINE
startport

2	2	coords= 909 -233 0	1	0	0	mandatory= 0	0	1
disable

funcName= disable 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	1	coords= 909 -273 0	2	0	0	mandatory= 0	0	1
set

funcName= set_bias 4	0	EndOfFunc
BEGIN_LINE
1	218097	0	2	-1	0
1030	-328
1030	-390
864	-390
864	-273
END_LINE
finishport

3	2	coords= 985 -328 0	0	0	1	mandatory= 0	0	1
--

funcName= convert 3	4	EndOfFunc
BEGIN_LINE
0	218097	2	3	-1	0
END_LINE
finishport

3	1	coords= 985 -288 0	1	0	1	mandatory= 0	0	1
error

funcName= convert 3	5	EndOfFunc
BEGIN_LINE
0	218096	1	3	-1	0
END_LINE
finishport

3	1	coords= 985 -248 0	2	0	1	mandatory= 0	0	1
--

funcName= enable 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 985 -233 0	3	0	1	mandatory= 0	0	1
--

funcName= disable 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	coords= 985 -273 0	4	0	1	mandatory= 0	0	1
--

funcName= set_bias 4	2	EndOfFunc
BEGIN_LINE
0	218096	2	3	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= convert 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
