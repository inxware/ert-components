#V:2.1.2

CanvasSizeXY
325	190

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
mode	0	0	1	0	0	0	0	1.2	0
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
0	218100	0	3	-1	0
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

adc_read_single
_
218099	0	0	0
_
_
Read ADC value when the sample port is triggered. It will output error when the ADC unit is not initialised or configured in a different mode
adc_read_single
0
_
920	-430	995	-370
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	ADC_S.S.
trans	-1	0	0	0
offset	0	-15
hash	0xB63C
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
fixed_point	1	0	16	0	0	0	0	1.2	10
EndOfValues
EndOfLabels
fixed_point

outputport

1	1	coords= 995 -393 0	0	0	1	mandatory= 0	0	1
value

funcName= sample 1	1	EndOfFunc
BEGIN_LINE
0	218101	0	1	-1	0
END_LINE
startport

2	1	coords= 919 -403 0	0	0	0	mandatory= 0	0	1
samp.

funcName= sample 1	0	EndOfFunc
BEGIN_LINE
1	218100	0	2	-1	0
877	-405
887	-403
END_LINE
finishport

3	1	coords= 995 -403 0	0	0	1	mandatory= 0	0	1
--

funcName= sample 1	2	EndOfFunc
BEGIN_LINE
0	218101	0	3	-1	0
END_LINE
finishport

3	1	coords= 995 -383 0	1	0	1	mandatory= 0	0	1
error

funcName= sample 1	3	EndOfFunc
BEGIN_LINE
0	218101	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
218100	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
780	-460	845	-370
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

0	1	coords= 779 -425 0	0	0	1	mandatory= 0	0	1
period

funcName= start 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 779 -415 0	1	0	1	mandatory= 0	0	1
rpt

funcName= start 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 779 -405 0	2	0	1	mandatory= 0	0	1
retrig

funcName= start 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	0	coords= 779 -435 0	0	0	0	mandatory= 0	0	1
start

funcName= start 1	0	EndOfFunc
BEGIN_LINE
1	218023	0	2	-1	0
1030	-518
1030	-580
734	-580
734	-435
END_LINE
startport

2	0	coords= 779 -385 0	1	0	0	mandatory= 0	0	1
stop

funcName= stop 3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	coords= 845 -405 0	0	0	1	mandatory= 0	0	1
clk

funcName= tick 2	1	EndOfFunc
BEGIN_LINE
0	218099	0	3	-1	0
END_LINE
finishport

3	0	coords= 845 -435 0	1	0	1	mandatory= 0	0	1


funcName= start 1	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	coords= 845 -385 0	2	0	1	mandatory= 0	0	1


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

mux_2i
_
218101	0	0	0
_
_
This is a 2-input integer multiplexer.
MultiplexTwoInputInt
0
_
1075	-425	1105	-355
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	3	5	1.25	0	Mux
trans	-1	0	0	0
offset	0	0
hash	0x84B5
Instance_Info	_
End_Instance
END_BA
inputport

0	1	coords= 1074 -390 0	0	0	1	mandatory= 0	0	1
i1

funcName= Run_Input1 1	1	EndOfFunc
BEGIN_LINE
1	218099	0	0	-1	0
1039	-393
1049	-390
END_LINE
inputport

0	1	coords= 1074 -365 0	1	0	1	mandatory= 0	0	1
i2

funcName= Run_Input2 2	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	1	coords= 1105 -390 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	2	funcName= Run_Input2 2	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	1	coords= 1074 -400 0	0	0	0	mandatory= 0	0	1


funcName= Run_Input1 1	0	EndOfFunc
BEGIN_LINE
1	218099	0	2	-1	0
1039	-403
1049	-400
END_LINE
startport

2	1	coords= 1074 -375 0	1	0	0	mandatory= 0	0	1


funcName= Run_Input2 2	0	EndOfFunc
BEGIN_LINE
1	218099	1	2	-1	0
1039	-383
1049	-375
END_LINE
finishport

3	1	coords= 1105 -400 0	0	0	1	mandatory= 0	0	1


funcName= Run_Input1 1	3	funcName= Run_Input2 2	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
