#V:2.1.2

CanvasSizeXY
2000	1000

IconData
BEGIN_BLOCK

mqtt_client
_
1	0	0	0
_
_
mqtt_client
mqtt_client
0
_
760	340	865	460
1	0
blockattr
BEGIN_BA
type	IO
text	5	5	1.5	0	MQTT_Client
trans	-1	0	0	0
offset	0	-15
hash	0xCDD0
Instance_Info	_
End_Instance
END_BA
parameter
client cert	3	0	0	0	0	0	0	1.2	client.pem
EndOfValues
EndOfLabels
Client

parameter
client key	3	0	0	0	0	0	0	1.2	client.key
EndOfValues
EndOfLabels
Client

parameter
server cert	3	0	0	0	0	0	0	1.2	server.crt
EndOfValues
EndOfLabels
Server

inputport

0	3	coords= 759 415 0	0	0	1	mandatory= 0	0	1
password

funcName= connect 1	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	coords= 759 425 0	1	0	1	mandatory= 0	0	1
tls

funcName= connect 1	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 759 405 0	2	0	1	mandatory= 0	0	1
username

funcName= connect 1	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	coords= 759 395 0	3	0	1	mandatory= 0	0	1
clientid

funcName= connect 1	4	EndOfFunc
BEGIN_LINE
1	6	0	0	-1	0
707	421
707	395
END_LINE
inputport

0	1	coords= 759 385 0	4	0	1	mandatory= 0	0	1
port

funcName= connect 1	5	EndOfFunc
BEGIN_LINE
1	5	0	0	-1	0
697	395
707	385
END_LINE
inputport

0	3	coords= 759 375 0	5	0	1	mandatory= 0	0	1
host

funcName= connect 1	6	EndOfFunc
BEGIN_LINE
1	4	0	0	-1	0
737	371
747	375
END_LINE
startport

2	0	coords= 759 440 0	0	0	1	mandatory= 0	0	1
disconnect

funcName= disconnect 2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	coords= 759 365 0	1	1	1	mandatory= 0	0	1
connect

funcName= connect 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	coords= 865 440 0	0	0	1	mandatory= 0	0	1
--

funcName= connect 1	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	coords= 865 365 0	1	0	1	mandatory= 0	0	1
--

funcName= connect 1	8	EndOfFunc
BEGIN_LINE
0	2	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mqtt_subscribe
_
2	0	0	0
_
_
mqtt_subscribe
mqtt_subscribe
0
_
1085	335	1190	415
1	0
blockattr
BEGIN_BA
type	IO
text	5	5	1.5	0	MQTT_Sub
trans	-1	0	0	0
offset	0	-15
hash	0xE7F5
Instance_Info	_
End_Instance
END_BA
inputport

0	3	coords= 1084 370 0	0	0	1	mandatory= 0	0	1
topic

EndOfFunc
BEGIN_LINE
1	3	0	0	-1	0
1044	391
1044	370
END_LINE
inputport

0	1	coords= 1084 380 0	1	0	1	mandatory= 0	0	1
qos

EndOfFunc
BEGIN_LINE
1	7	0	0	-1	0
1044	430
1044	380
END_LINE
outputport

1	3	coords= 1190 380 0	0	0	1	mandatory= 0	0	1
Data

EndOfFunc
BEGIN_LINE
0	8	0	1	-1	0
END_LINE
startport

2	-1	coords= 1084 360 0	0	0	1	mandatory= 0	0	1
subscribe

EndOfFunc
BEGIN_LINE
1	1	1	2	-1	0
1004	365
1014	360
END_LINE
startport

2	1	coords= 1084 395 0	1	0	1	mandatory= 0	0	1
unsubscribe

funcName= subscribe 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	coords= 1190 360 0	0	0	1	mandatory= 0	0	1
--

EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	coords= 1190 370 0	1	0	1	mandatory= 0	0	1
Received

EndOfFunc
BEGIN_LINE
0	8	0	3	-1	0
END_LINE
finishport

3	1	coords= 1190 395 0	2	0	1	mandatory= 0	0	1
--

funcName= subscribe 1	1	EndOfFunc
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
885	380	1025	401
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
String Constant	3	0	100	1	5	4	0	1.2	test-topic
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 1025 391 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	2	0	1	-1	0
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
4	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
555	360	695	381
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
String Constant	3	0	100	1	5	4	0	1.2	test.mosquitto.org
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 695 371 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	1	5	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
5	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
560	385	655	405
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
Integer Value	1	-2147483648	2147483647	1	5	4	0	1.2	1883
EndOfValues
EndOfLabels
integer constant

outputport

1	1	coords= 655 395 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	1	4	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
6	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
535	410	675	431
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
String Constant	3	0	100	1	5	4	0	1.2	inx-test
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 675 421 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	1	3	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_i1
_
7	0	0	0
_
_
This is an integer constant.
Constant_Int1
0
_
930	420	1025	440
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

1	1	coords= 1025 430 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	1	EndOfFunc
BEGIN_LINE
0	2	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantInt 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cmps
_
8	0	0	0
_
_
This is a 2-input string comparator.
CmpString
0
_
1260	350	1325	420
1	0
blockattr
BEGIN_BA
type	Data_Processor
text	15	5	1.25	0	Comp
trans	-1	0	0	0
offset	0	-15
hash	0xC389
Instance_Info	_
End_Instance
END_BA
parameter
Compare String (-> s2)	3	0	0	0	0	0	0	1.2	_
EndOfValues
EndOfLabels
String to compare to unless overridden by port s2

inputport

0	3	coords= 1259 395 0	0	0	1	mandatory= 0	0	1
s1

funcName= Run_CmpString 1	1	EndOfFunc
BEGIN_LINE
1	2	0	0	-1	0
1224	380
1234	395
END_LINE
inputport

0	3	coords= 1259 410 0	1	0	1	mandatory= 0	0	1
s2

funcName= Run_CmpString 1	2	EndOfFunc
BEGIN_LINE
1	9	0	0	-1	0
1244	441
1244	410
END_LINE
outputport

1	0	coords= 1325 395 0	0	0	1	mandatory= 0	0	1


funcName= Run_CmpString 1	3	EndOfFunc
BEGIN_LINE
0	10	0	1	-1	0
END_LINE
startport

2	0	coords= 1259 375 0	0	0	0	mandatory= 0	0	1


funcName= Run_CmpString 1	0	EndOfFunc
BEGIN_LINE
1	2	1	2	-1	0
1194	370
1204	375
END_LINE
finishport

3	0	coords= 1325 375 0	0	0	1	mandatory= 0	0	1


funcName= Run_CmpString 1	4	EndOfFunc
BEGIN_LINE
0	10	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
9	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
1070	430	1210	451
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
String Constant	3	0	100	1	5	4	0	1.2	on
EndOfValues
EndOfLabels
string constant

outputport

1	3	coords= 1210 441 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	1	EndOfFunc
BEGIN_LINE
0	8	1	1	-1	0
END_LINE
internalport

4	-1	coords= 0 0 0	0	0	1	mandatory= 0	0	1


funcName= Run_ConstantString 1	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gpio_out
_
10	0	0	0
_
_
GPIO Output
gpio_out
0
_
1210	520	1260	565
1	0
blockattr
BEGIN_BA
type	IO
text	10	5	1.25	0	GPIO
trans	-1	0	0	0
offset	0	-15
hash	0x87EF
Instance_Info	_
End_Instance
END_BA
parameter
Pin ID	1	0	32767	0	0	0	0	1.2	47
EndOfValues
EndOfLabels
Pin ID

inputport

0	0	coords= 1209 555 0	0	0	1	mandatory= 0	0	1


funcName= write 1	1	EndOfFunc
BEGIN_LINE
1	8	0	0	-1	0
1435	395
1435	480
1145	480
1145	555
END_LINE
startport

2	1	coords= 1209 545 0	0	0	1	mandatory= 0	0	1
write

funcName= write 1	0	EndOfFunc
BEGIN_LINE
1	8	0	2	-1	0
1445	375
1445	490
1160	490
1160	545
END_LINE
END_BLOCK
END_OF_BLOCKS
