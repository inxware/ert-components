#V:2.0.0

CanvasSizeXY
2000	1544

IconData
BEGIN_BLOCK

gui_text_string2
_
25	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
955	840	1055	1014
1	0
parameter
Widget Tag	5	_	_	LoginError
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	955	945	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	955	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	965	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	975	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	955	990	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	955	1005	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	29	3	0	-1	1
865	785
865	1005
END_LINE
outputport

1	1	1055	945	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	955	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	965	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1055	975	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	955	850	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	860	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	875	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	885	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	955	930	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	29	4	2	-1	1
820	745
820	930
END_LINE
finishport

3	1	1055	850	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	860	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	875	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	885	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1055	930	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	13	4	3	-1	0
END_LINE
finishport

3	1	1055	900	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	66	2	3	-1	0
END_LINE
finishport

3	1	1055	910	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
27	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
195	885	255	975
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	500000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	195	930	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	195	945	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	195	960	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	195	895	4	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	51	4	2	-1	0
1080	685
1080	1060
180	1060
180	895
END_LINE
startport

2	0	195	910	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	255	925	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	29	4	3	-1	0
END_LINE
finishport

3	0	255	895	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	255	910	2	0	1	mandatory= 0	0	1


stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

url_get
_
29	0	0	0
_
_
Retrieves URLs using get or post and returns the data to a file location or as an output string.
UrlGet
0
_
390	650	521	969
1	0
parameter
URL	3	_	_	http://devport.inx-systems.com/headers.php
EndOfValues
EndOfLabels
URL for the get method (provides base for post)

parameter
Post Data	3	_	_	_
EndOfValues
EndOfLabels
post data to be appended to the URL (Leave empty to use get method).

parameter
Local Path	3	_	_	stuffXX.txt
EndOfValues
EndOfLabels
Local path to save data - empty means no save.

parameter
Use POST (not GET)	0	_	_	0
EndOfValues
EndOfLabels
Forces a post method using the URL string

parameter
Use URL Path	0	_	_	1
EndOfValues
EndOfLabels
Uses N levels of path when storing files to /Path

parameter
Path Levels to Remove (N)	1	0	10000	0
EndOfValues
EndOfLabels
If Use URL path is set then this parameter specfified how many path levels to remove from the beginning of the path.

parameter
duty gap (ms)	1	-100	1000000	0
EndOfValues
EndOfLabels
Duty gap allows a pause in ms to be inserted between each data chunk get to reduce network flooding issues.

parameter
Timeout	1	-1	3600	60
EndOfValues
EndOfLabels
Times out the initial http request and returns Error and http code event if no connection is made.

parameter
Use Cookies	0	_	_	1
EndOfValues
EndOfLabels
Enable/disable the URL to send and receive cookies

parameter
Host Username	3	_	_	pdrezet
EndOfValues
EndOfLabels
Username of the host site.

parameter
Host Password	3	_	_	HelloWorld23445
EndOfValues
EndOfLabels
Password of the host site.

parameter
Client SSL Certificate	3	_	_	_
EndOfValues
EndOfLabels
Name of SSL certificate to use to communicate with site.

parameter
Client SSL Certificate password	3	_	_	_
EndOfValues
EndOfLabels
Password to use the local SSL certificate.

parameter
Use Proxy Server	0	_	_	1
EndOfValues
EndOfLabels
Enable/disable the system proxy server settings in file ./sysinfo/proxies.

parameter
Always overwrite local file	0	_	_	0
EndOfValues
EndOfLabels
Set to true to override skipping downloads when file server reports a file

inputport

0	3	390	670	0	0	1	mandatory= 0	0	1
URL

getpost	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	680	1	0	1	mandatory= 0	0	1
post data

getpost	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	695	2	0	1	mandatory= 0	0	1
local path

getpost	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	390	705	3	0	1	mandatory= 0	0	1
URLdirDump

getpost	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	390	720	4	0	1	mandatory= 0	0	1
idle time

getpost	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	390	760	5	0	1	mandatory= 0	0	1
Cookies on

getpost	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	390	740	6	0	1	mandatory= 0	0	1
Force Write

getpost	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	785	7	0	1	mandatory= 0	0	1
Cookies

getpost	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	800	8	0	1	mandatory= 0	0	1
Server user

getpost	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	810	9	0	1	mandatory= 0	0	1
pass

getpost	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	825	10	0	1	mandatory= 0	0	1
Certificate name

getpost	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	390	835	11	0	1	mandatory= 0	0	1
pass

getpost	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	390	770	12	0	1	mandatory= 0	0	1
Use POST

getpost	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	390	750	13	0	1	mandatory= 0	0	1
Proxy on

getpost	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	520	685	0	0	1	mandatory= 0	0	1
Response

getpost	15	EndOfFunc
BEGIN_LINE
0	51	5	1	-1	0
END_LINE
outputport

1	1	520	720	1	0	1	mandatory= 0	0	1
E-num

getpost	17	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	520	730	2	0	1	mandatory= 0	0	1
E-str

getpost	18	EndOfFunc
BEGIN_LINE
0	67	5	1	-1	0
END_LINE
outputport

1	3	520	785	3	0	1	mandatory= 0	0	1
Cookies

getpost	16	EndOfFunc
BEGIN_LINE
0	25	5	1	-1	0
END_LINE
outputport

1	1	520	700	4	0	1	mandatory= 0	0	1
bytes

getpost	19	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	390	660	0	0	0	mandatory= 0	0	1
Get

getpost	0	EndOfFunc
BEGIN_LINE
1	51	5	2	-1	0
1025	655
1025	605
375	605
375	660
END_LINE
startport

2	-1	390	905	1	0	0	mandatory= 0	0	1
Abort

abort	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	390	870	2	0	1	mandatory= 0	0	1
Save cookies

savecookies	0	EndOfFunc
BEGIN_LINE
1	25	5	2	-1	0
1070	900
1070	1050
330	1050
330	870
END_LINE
startport

2	0	390	880	3	0	1	mandatory= 0	0	1
Clear cookies

clearcookies	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	390	850	4	0	0	mandatory= 0	0	1
Get next

getmoredata	0	EndOfFunc
BEGIN_LINE
1	27	0	2	-1	0
310	925
310	850
END_LINE
finishport

3	-1	520	660	0	0	1	mandatory= 0	0	1
Getting

getpost	20	EndOfFunc
BEGIN_LINE
0	87	0	3	-1	0
END_LINE
finishport

3	-1	520	905	1	0	1	mandatory= 0	0	1
------------

abort	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	520	675	3	0	1	mandatory= 0	0	1
Data Ready

getpost	21	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
finishport

3	3	520	710	2	0	1	mandatory= 0	0	1
Error

getpost	23	EndOfFunc
BEGIN_LINE
0	68	0	3	-1	0
END_LINE
finishport

3	3	520	745	4	0	1	mandatory= 0	0	1
Complete

getpost	22	EndOfFunc
BEGIN_LINE
0	25	4	3	-1	0
END_LINE
finishport

3	0	520	870	5	0	1	mandatory= 0	0	1
------------

savecookies	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	520	880	6	0	1	mandatory= 0	0	1
------------

clearcookies	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
51	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
910	595	1010	769
1	0
parameter
Widget Tag	5	_	_	loginReturned
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	910	700	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	710	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	720	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	730	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	910	745	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	910	760	1	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	29	0	0	-1	0
850	685
850	760
END_LINE
outputport

1	1	1010	700	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1010	710	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1010	720	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	1010	730	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	910	605	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	615	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	630	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	910	640	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	910	685	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	29	2	2	-1	0
865	675
865	685
END_LINE
finishport

3	1	1010	605	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	615	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	630	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	640	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	1010	685	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	27	0	3	-1	0
END_LINE
finishport

3	1	1010	655	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	66	0	3	-1	0
END_LINE
finishport

3	1	1010	665	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
63	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
840	155	940	329
1	0
parameter
Widget Tag	5	_	_	HTTPSlReturn
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	840	260	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	840	270	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	840	280	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	840	290	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	840	305	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	840	320	0	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	66	3	0	-1	0
780	320
780	320
END_LINE
outputport

1	1	940	260	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	940	270	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	940	280	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	940	290	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	840	165	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	840	175	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	840	190	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	1	2	-1	0
END_LINE
startport

2	-1	840	200	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	840	245	4	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	66	4	2	-1	0
795	280
795	245
END_LINE
finishport

3	1	940	165	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	175	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	190	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	200	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	940	245	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	13	4	3	-1	0
END_LINE
finishport

3	1	940	215	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	162	0	3	-1	0
END_LINE
finishport

3	1	940	225	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

time_clock
_
64	0	0	0
_
_
Creates periodic timed events.
time_clock
0
_
325	450	385	540
1	0
parameter
Time Interval (uSecs)	1	1	2147483647	500000
EndOfValues
EndOfLabels
Time interval between events.

parameter
Periodic Flag	0	_	_	0
EndOfValues
EndOfLabels
Periodic if TRUE, single delay only if FALSE.

parameter
Retriggerable Flag	0	_	_	1
EndOfValues
EndOfLabels
Retriggerable if TRUE.

inputport

0	1	325	495	0	0	1	mandatory= 0	0	1
period

start	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	325	510	1	0	1	mandatory= 0	0	1
rpt

start	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	325	525	2	0	1	mandatory= 0	0	1
retrig

start	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
startport

2	3	325	460	4	0	1	mandatory= 0	0	1
start

start	0	EndOfFunc
BEGIN_LINE
1	63	4	2	-1	0
965	245
965	95
310	95
310	460
END_LINE
startport

2	0	325	475	1	0	1	mandatory= 0	0	1
stop

stop	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	0	385	490	0	0	1	mandatory= 0	0	1
clk

tick	1	EndOfFunc
BEGIN_LINE
0	29	4	3	-1	0
END_LINE
finishport

3	0	385	460	1	0	1	mandatory= 0	0	1


start	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	385	475	2	0	1	mandatory= 0	0	1


stop	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


tick	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

url_get
_
66	0	0	0
_
_
Retrieves URLs using get or post and returns the data to a file location or as an output string.
UrlGet
0
_
450	185	581	504
1	0
parameter
URL	3	_	_	https://www.google.com
EndOfValues
EndOfLabels
URL for the get method (provides base for post)

parameter
Post Data	3	_	_	_
EndOfValues
EndOfLabels
post data to be appended to the URL (Leave empty to use get method).

parameter
Local Path	3	_	_	stuff.txt
EndOfValues
EndOfLabels
Local path to save data - empty means no save.

parameter
Use POST (not GET)	0	_	_	0
EndOfValues
EndOfLabels
Forces a post method using the URL string

parameter
Use URL Path	0	_	_	1
EndOfValues
EndOfLabels
Uses N levels of path when storing files to /Path

parameter
Path Levels to Remove (N)	1	0	10000	0
EndOfValues
EndOfLabels
If Use URL path is set then this parameter specfified how many path levels to remove from the beginning of the path.

parameter
duty gap (ms)	1	-100	1000000	0
EndOfValues
EndOfLabels
Duty gap allows a pause in ms to be inserted between each data chunk get to reduce network flooding issues.

parameter
Timeout	1	-1	3600	60
EndOfValues
EndOfLabels
Times out the initial http request and returns Error and http code event if no connection is made.

parameter
Use Cookies	0	_	_	1
EndOfValues
EndOfLabels
Enable/disable the URL to send and receive cookies

parameter
Host Username	3	_	_	_
EndOfValues
EndOfLabels
Username of the host site.

parameter
Host Password	3	_	_	_
EndOfValues
EndOfLabels
Password of the host site.

parameter
Client SSL Certificate	3	_	_	_
EndOfValues
EndOfLabels
Name of SSL certificate to use to communicate with site.

parameter
Client SSL Certificate password	3	_	_	_
EndOfValues
EndOfLabels
Password to use the local SSL certificate.

parameter
Use Proxy Server	0	_	_	1
EndOfValues
EndOfLabels
Enable/disable the system proxy server settings in file ./sysinfo/proxies.

parameter
Always overwrite local file	0	_	_	0
EndOfValues
EndOfLabels
Set to true to override skipping downloads when file server reports a file

inputport

0	3	450	205	0	0	1	mandatory= 0	0	1
URL

getpost	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	215	1	0	1	mandatory= 0	0	1
post data

getpost	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	230	2	0	1	mandatory= 0	0	1
local path

getpost	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	240	3	0	1	mandatory= 0	0	1
URLdirDump

getpost	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	450	255	4	0	1	mandatory= 0	0	1
idle time

getpost	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	450	295	5	0	1	mandatory= 0	0	1
Cookies on

getpost	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	450	275	6	0	1	mandatory= 0	0	1
Force Write

getpost	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	320	7	0	1	mandatory= 0	0	1
Cookies

getpost	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	335	8	0	1	mandatory= 0	0	1
Server user

getpost	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	345	9	0	1	mandatory= 0	0	1
pass

getpost	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	360	10	0	1	mandatory= 0	0	1
Certificate name

getpost	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	450	370	11	0	1	mandatory= 0	0	1
pass

getpost	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	450	305	12	0	1	mandatory= 0	0	1
Use POST

getpost	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	450	285	13	0	1	mandatory= 0	0	1
Proxy on

getpost	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	580	220	2	0	1	mandatory= 0	0	1
Response

getpost	15	EndOfFunc
BEGIN_LINE
0	63	5	1	-1	0
END_LINE
outputport

1	1	580	255	1	0	1	mandatory= 0	0	1
E-num

getpost	17	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	580	265	0	0	1	mandatory= 0	0	1
E-str

getpost	18	EndOfFunc
BEGIN_LINE
0	67	5	1	-1	0
END_LINE
outputport

1	3	580	320	3	0	1	mandatory= 0	0	1
Cookies

getpost	16	EndOfFunc
BEGIN_LINE
0	63	5	1	-1	0
END_LINE
outputport

1	1	580	235	4	0	1	mandatory= 0	0	1
bytes

getpost	19	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	450	195	0	0	0	mandatory= 0	0	1
Get

getpost	0	EndOfFunc
BEGIN_LINE
1	51	5	2	-1	0
1025	655
1025	600
435	600
435	195
END_LINE
startport

2	-1	450	440	1	0	0	mandatory= 0	0	1
Abort

abort	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	450	405	2	0	1	mandatory= 0	0	1
Save cookies

savecookies	0	EndOfFunc
BEGIN_LINE
1	25	5	2	-1	0
1070	900
1070	1050
330	1050
330	405
END_LINE
startport

2	0	450	415	3	0	1	mandatory= 0	0	1
Clear cookies

clearcookies	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	450	385	4	0	0	mandatory= 0	0	1
Get next

getmoredata	0	EndOfFunc
BEGIN_LINE
1	64	0	2	-1	0
400	490
440	385
END_LINE
finishport

3	-1	580	195	0	0	1	mandatory= 0	0	1
Getting

getpost	20	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	-1	580	440	1	0	1	mandatory= 0	0	1
------------

abort	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	580	210	2	0	1	mandatory= 0	0	1
Data Ready

getpost	21	EndOfFunc
BEGIN_LINE
0	63	4	3	-1	0
END_LINE
finishport

3	3	580	245	3	0	1	mandatory= 0	0	1
Error

getpost	23	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
finishport

3	3	580	280	4	0	1	mandatory= 0	0	1
Complete

getpost	22	EndOfFunc
BEGIN_LINE
0	63	4	3	-1	0
END_LINE
finishport

3	0	580	405	5	0	1	mandatory= 0	0	1
------------

savecookies	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	580	415	6	0	1	mandatory= 0	0	1
------------

clearcookies	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

gui_text_string2
_
67	0	0	0
_
_
This widget displays an string to the GUI output.
gui_text_string2
0
_
845	350	945	524
1	0
parameter
Widget Tag	5	_	_	HTTPSError
EndOfValues
EndOfLabels
widget tag

parameter
Widget-Group	4	_	_	default
EndOfValues
EndOfLabels
widget group

parameter
Maintain Aspect Ratio	0	_	_	0
EndOfValues
EndOfLabels
maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.

parameter
Register Click Disregarding Z-Order	0	_	_	0
EndOfValues
EndOfLabels
allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.

inputport

0	1	845	455	0	0	1	mandatory= 0	0	1
off.x

update	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	465	1	0	1	mandatory= 0	0	1
off.y

update	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	475	2	0	1	mandatory= 0	0	1
off.wid

update	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	485	3	0	1	mandatory= 0	0	1
off.ht

update	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	845	500	4	0	1	mandatory= 0	0	1
alpha

update	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	845	515	1	0	1	mandatory= 0	0	1
data

update	6	EndOfFunc
BEGIN_LINE
1	29	2	0	-1	0
785	730
785	515
END_LINE
outputport

1	1	945	455	0	0	1	mandatory= 0	0	1
abs x

create	1	update	7	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	945	465	1	0	1	mandatory= 0	0	1
abs y

create	2	update	8	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	945	475	2	0	1	mandatory= 0	0	1
wid

create	3	update	9	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	1	945	485	3	0	1	mandatory= 0	0	1
ht

create	4	update	10	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	845	360	0	1	0	mandatory= 0	0	1
create

create	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	370	1	0	0	mandatory= 0	0	1
destroy

destroy	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	385	2	1	0	mandatory= 0	0	1
show

show	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	845	395	3	0	0	mandatory= 0	0	1
hide

hide	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	845	440	0	0	0	mandatory= 0	0	1
update

update	0	EndOfFunc
BEGIN_LINE
1	68	0	2	-1	0
810	896
810	440
END_LINE
finishport

3	1	945	360	0	0	1	mandatory= 0	0	1
--

create	5	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	370	1	0	1	mandatory= 0	0	1
--

destroy	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	385	2	0	1	mandatory= 0	0	1
--

show	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	395	3	0	1	mandatory= 0	0	1
--

hide	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	440	4	0	1	mandatory= 0	0	1
--

update	11	EndOfFunc
BEGIN_LINE
0	13	4	3	-1	0
END_LINE
finishport

3	1	945	410	5	0	1	mandatory= 0	0	1
click

create	6	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	945	420	6	0	1	mandatory= 0	0	1
mouse down

create	7	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor2
_
68	0	0	0
_
_
This is a 2-input event OR.
EventTwoInputOr
0
_
780	875	800	915
1	0
startport

2	-1	780	885	4	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	29	3	2	-1	0
770	710
770	885
END_LINE
startport

2	-1	780	905	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
1	29	2	2	-1	0
605	675
605	905
END_LINE
finishport

3	-1	800	896	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	EndOfFunc
BEGIN_LINE
0	67	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_cats
_
69	0	0	0
_
_
This is a 2-input string cat.
CatString
0
_
815	635	865	695
1	0
parameter
Append String (-> s2)	3	_	_	_
EndOfValues
EndOfLabels
String to concatenate - unless overridden by port s2

inputport

0	3	815	665	0	0	1	mandatory= 0	0	1
s1

Run_CatString	1	EndOfFunc
BEGIN_LINE
1	71	0	0	-1	0
690	605
690	665
END_LINE
inputport

0	3	815	680	1	0	1	mandatory= 0	0	1
s2

Run_CatString	2	EndOfFunc
BEGIN_LINE
1	87	0	0	-1	0
795	720
795	680
END_LINE
outputport

1	3	865	665	0	0	1	mandatory= 0	0	1


Run_CatString	3	EndOfFunc
BEGIN_LINE
0	71	1	1	-1	0
END_LINE
startport

2	-1	815	645	0	0	1	mandatory= 0	0	1


Run_CatString	0	EndOfFunc
BEGIN_LINE
1	104	0	2	-1	0
805	638
805	645
END_LINE
finishport

3	3	865	645	0	0	1	mandatory= 0	0	1


Run_CatString	4	EndOfFunc
BEGIN_LINE
0	51	4	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
71	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
620	575	670	650
1	0
inputport

0	3	620	600	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	72	0	0	-1	0
610	605
610	600
END_LINE
inputport

0	3	620	635	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	69	0	0	-1	0
875	665
875	615
610	615
610	635
END_LINE
outputport

1	3	670	605	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	69	0	1	-1	0
END_LINE
startport

2	3	620	585	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	29	0	2	-1	0
560	660
560	585
END_LINE
startport

2	-1	620	620	0	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	29	2	2	-1	0
605	675
605	620
END_LINE
finishport

3	3	670	585	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	69	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
72	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
435	575	535	620
1	0
parameter
String Constant	3	_	_	_
EndOfValues
EndOfLabels
string constant

outputport

1	3	535	605	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	87	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

mux_2s
_
87	0	0	0
_
_
This is a 2-input string multiplexer.
MultiplexTwoInputString
0
_
695	690	745	765
1	0
inputport

0	3	695	715	0	0	1	mandatory= 0	0	1
i1

Run_Input1	1	EndOfFunc
BEGIN_LINE
1	72	0	0	-1	0
580	605
580	715
END_LINE
inputport

0	3	695	750	1	0	1	mandatory= 0	0	1
i2

Run_Input2	1	EndOfFunc
BEGIN_LINE
1	29	0	0	-1	0
675	685
675	750
END_LINE
outputport

1	3	745	720	0	0	1	mandatory= 0	0	1


Run_Input1	2	Run_Input2	2	EndOfFunc
BEGIN_LINE
0	51	5	1	-1	0
END_LINE
startport

2	3	695	700	0	0	1	mandatory= 0	0	1


Run_Input1	0	EndOfFunc
BEGIN_LINE
1	29	0	2	-1	0
685	660
685	700
END_LINE
startport

2	-1	695	735	0	0	1	mandatory= 0	0	1


Run_Input2	0	EndOfFunc
BEGIN_LINE
1	29	2	2	-1	0
680	675
680	735
END_LINE
finishport

3	3	745	700	0	0	1	mandatory= 0	0	1


Run_Input1	3	Run_Input2	3	EndOfFunc
BEGIN_LINE
0	104	1	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventrstand2
_
104	0	0	0
_
_
This is a 2-input event latching AND with an internal reset.
EventSelfResetTwoInputLatchingAnd
0
_
765	620	780	655
1	0
startport

2	3	765	630	1	0	1	mandatory= 0	0	1


And1	0	EndOfFunc
BEGIN_LINE
1	71	0	2	-1	0
715	585
715	630
END_LINE
startport

2	-1	765	645	1	0	1	mandatory= 0	0	1


And2	0	EndOfFunc
BEGIN_LINE
1	87	0	2	-1	0
755	700
755	645
END_LINE
finishport

3	-1	780	638	0	0	1	mandatory= 0	0	1


And1	1	And2	1	EndOfFunc
BEGIN_LINE
0	69	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
120	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
155	730	255	775
1	0
parameter
String Constant	3	_	_	SomeNicestuff=3487568
EndOfValues
EndOfLabels
string constant

outputport

1	3	255	760	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	121	0	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

stringfn_formats
_
121	0	0	0
_
_
This is a 2-input string formatter. Use the format property to show how the string should appear.
string_format
0
_
285	740	335	800
1	0
parameter
Format	3	_	_	%s\r%s\r
EndOfValues
EndOfLabels
C-style format string

inputport

0	3	285	770	0	0	1	mandatory= 0	0	1
s1

run	1	EndOfFunc
BEGIN_LINE
1	120	0	0	-1	0
275	760
275	770
END_LINE
inputport

0	3	285	785	1	0	1	mandatory= 0	0	1
s2

run	2	EndOfFunc
BEGIN_LINE
1	139	0	0	-1	0
265	815
265	785
END_LINE
outputport

1	3	335	770	0	0	1	mandatory= 0	0	1


run	3	EndOfFunc
BEGIN_LINE
0	29	7	1	-1	0
END_LINE
startport

2	3	285	750	0	1	1	mandatory= 0	0	1


run	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	3	335	750	0	0	1	mandatory= 0	0	1


run	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_s1
_
139	0	0	0
_
_
This is a string constant.
Constant_String1
0
_
150	785	250	830
1	0
parameter
String Constant	3	_	_	SomeMorestuff=1111111111
EndOfValues
EndOfLabels
string constant

outputport

1	3	250	815	0	0	1	mandatory= 0	0	1


Run_ConstantString	1	EndOfFunc
BEGIN_LINE
0	121	1	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantString	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
140	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
0
_
190	660	290	705
1	0
parameter
Boolean Constant	0	_	_	0
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	290	690	0	0	1	mandatory= 0	0	1


Run_ConstantBool	1	EndOfFunc
BEGIN_LINE
0	29	13	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

const_b1
_
141	0	0	0
_
_
This is a boolean constant.
Constant_Bool1
0
_
330	375	430	420
1	0
parameter
Boolean Constant	0	_	_	0
EndOfValues
EndOfLabels
boolean constant

outputport

1	0	430	405	0	0	1	mandatory= 0	0	1


Run_ConstantBool	1	EndOfFunc
BEGIN_LINE
0	66	13	1	-1	0
END_LINE
internalport

4	-1	0	0	0	0	0	mandatory= 0	0	1


Run_ConstantBool	0	EndOfFunc
BEGIN_LINE
0	-1	0	4	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

url_get
_
162	0	0	0
_
_
Retrieves URLs using get or post and returns the data to a file location or as an output string.
UrlGet
0
_
395	1125	526	1444
1	0
parameter
URL	3	_	_	http://data.inx-systems.net/testdata.txt
EndOfValues
EndOfLabels
URL for the get method (provides base for post)

parameter
Post Data	3	_	_	_
EndOfValues
EndOfLabels
post data to be appended to the URL (Leave empty to use get method).

parameter
Local Path	3	_	_	_
EndOfValues
EndOfLabels
Local path to save data - empty means no save.

parameter
Use POST (not GET)	0	_	_	0
EndOfValues
EndOfLabels
Forces a post method using the URL string

parameter
Use URL Path	0	_	_	0
EndOfValues
EndOfLabels
Uses N levels of path when storing files to /Path

parameter
Path Levels to Remove (N)	1	0	10000	0
EndOfValues
EndOfLabels
If Use URL path is set then this parameter specfified how many path levels to remove from the beginning of the path.

parameter
duty gap (ms)	1	-100	1000000	0
EndOfValues
EndOfLabels
Duty gap allows a pause in ms to be inserted between each data chunk get to reduce network flooding issues.

parameter
Timeout	1	-1	3600	60
EndOfValues
EndOfLabels
Times out the initial http request and returns Error and http code event if no connection is made.

parameter
Use Cookies	0	_	_	1
EndOfValues
EndOfLabels
Enable/disable the URL to send and receive cookies

parameter
Host Username	3	_	_	_
EndOfValues
EndOfLabels
Username of the host site.

parameter
Host Password	3	_	_	_
EndOfValues
EndOfLabels
Password of the host site.

parameter
Client SSL Certificate	3	_	_	_
EndOfValues
EndOfLabels
Name of SSL certificate to use to communicate with site.

parameter
Client SSL Certificate password	3	_	_	_
EndOfValues
EndOfLabels
Password to use the local SSL certificate.

parameter
Use Proxy Server	0	_	_	0
EndOfValues
EndOfLabels
Enable/disable the system proxy server settings in file ./sysinfo/proxies.

parameter
Always overwrite local file	0	_	_	0
EndOfValues
EndOfLabels
Set to true to override skipping downloads when file server reports a file

inputport

0	3	395	1145	0	0	1	mandatory= 0	0	1
URL

getpost	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1155	1	0	1	mandatory= 0	0	1
post data

getpost	2	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1170	2	0	1	mandatory= 0	0	1
local path

getpost	3	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	395	1180	3	0	1	mandatory= 0	0	1
URLdirDump

getpost	4	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	1	395	1195	4	0	1	mandatory= 0	0	1
idle time

getpost	5	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	395	1235	5	0	1	mandatory= 0	0	1
Cookies on

getpost	6	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	395	1215	6	0	1	mandatory= 0	0	1
Force Write

getpost	14	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1260	7	0	1	mandatory= 0	0	1
Cookies

getpost	7	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1275	8	0	1	mandatory= 0	0	1
Server user

getpost	8	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1285	9	0	1	mandatory= 0	0	1
pass

getpost	9	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1300	10	0	1	mandatory= 0	0	1
Certificate name

getpost	10	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	395	1310	11	0	1	mandatory= 0	0	1
pass

getpost	11	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	395	1245	12	0	1	mandatory= 0	0	1
Use POST

getpost	13	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	0	395	1225	13	0	1	mandatory= 0	0	1
Proxy on

getpost	12	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
outputport

1	3	525	1160	0	0	1	mandatory= 0	0	1
Response

getpost	15	EndOfFunc
BEGIN_LINE
0	185	1	1	-1	0
END_LINE
outputport

1	1	525	1195	1	0	1	mandatory= 0	0	1
E-num

getpost	17	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
outputport

1	3	525	1205	2	0	1	mandatory= 0	0	1
E-str

getpost	18	EndOfFunc
BEGIN_LINE
0	67	5	1	-1	0
END_LINE
outputport

1	3	525	1260	3	0	1	mandatory= 0	0	1
Cookies

getpost	16	EndOfFunc
BEGIN_LINE
0	25	5	1	-1	0
END_LINE
outputport

1	1	525	1175	4	0	1	mandatory= 0	0	1
bytes

getpost	19	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	395	1135	0	0	0	mandatory= 0	0	1
Get

getpost	0	EndOfFunc
BEGIN_LINE
1	63	5	2	-1	0
945	215
945	150
390	150
390	1135
END_LINE
startport

2	-1	395	1380	1	0	0	mandatory= 0	0	1
Abort

abort	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	395	1345	2	0	1	mandatory= 0	0	1
Save cookies

savecookies	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	0	395	1355	3	0	1	mandatory= 0	0	1
Clear cookies

clearcookies	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	3	395	1325	4	0	0	mandatory= 0	0	1
Get next

getmoredata	0	EndOfFunc
BEGIN_LINE
1	185	2	2	-1	1
955	1270
955	1080
350	1080
350	1325
END_LINE
finishport

3	-1	525	1135	0	0	1	mandatory= 0	0	1
Getting

getpost	20	EndOfFunc
BEGIN_LINE
0	185	0	3	-1	0
END_LINE
finishport

3	-1	525	1380	1	0	1	mandatory= 0	0	1
------------

abort	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	3	525	1150	3	0	1	mandatory= 0	0	1
Data Ready

getpost	21	EndOfFunc
BEGIN_LINE
0	185	2	3	-1	0
END_LINE
finishport

3	3	525	1185	2	0	1	mandatory= 0	0	1
Error

getpost	23	EndOfFunc
BEGIN_LINE
0	68	0	3	-1	0
END_LINE
finishport

3	3	525	1220	4	0	1	mandatory= 0	0	1
Complete

getpost	22	EndOfFunc
BEGIN_LINE
0	186	1	3	-1	0
END_LINE
finishport

3	0	525	1345	5	0	1	mandatory= 0	0	1
------------

savecookies	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	0	525	1355	6	0	1	mandatory= 0	0	1
------------

clearcookies	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

file_wos
_
185	0	0	0
_
_
Writes string data to a file. If a field separator parameter is specified (within brackets, \n for a new line) then then this will be used inserted between data points. If a Fixed width parameter is specified then this specifies the length of the ascii fo
FILE_WriteOnly_String
0
_
810	1210	910	1310
1	0
parameter
File name	3	_	_	out.txt
EndOfValues
EndOfLabels
output file name

parameter
Field Width	1	0	2048	0
EndOfValues
EndOfLabels
Data Field Width (space padded)

parameter
Delimiter	3	_	_	()
EndOfValues
EndOfLabels
Field Delimiter Character

parameter
Append	0	_	_	0
EndOfValues
EndOfLabels
Append rather than overwrite.

inputport

0	3	810	1230	0	0	1	mandatory= 0	0	1
path

Run_OpenFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
0	-1	0	0	-1	0
END_LINE
inputport

0	3	810	1295	1	0	1	mandatory= 0	0	1
data

Run_WriteFILE_WriteOnly_String	1	EndOfFunc
BEGIN_LINE
1	162	0	0	-1	1
760	1160
760	1295
END_LINE
outputport

1	1	910	1295	0	0	1	mandatory= 0	0	1
err code

Run_WriteFILE_WriteOnly_String	2	Run_CloseFILE_WriteOnly_String	1	Run_OpenFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	1	-1	0
END_LINE
startport

2	-1	810	1220	0	0	0	mandatory= 0	0	1
open

Run_OpenFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	162	0	2	-1	1
800	1135
800	1220
END_LINE
startport

2	-1	810	1245	1	0	0	mandatory= 0	0	1
close

Run_CloseFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	186	0	2	-1	0
795	1300
795	1245
END_LINE
startport

2	3	810	1270	1	0	0	mandatory= 0	0	1
write

Run_WriteFILE_WriteOnly_String	0	EndOfFunc
BEGIN_LINE
1	186	1	2	-1	1
785	1320
785	1270
END_LINE
finishport

3	3	910	1220	0	0	1	mandatory= 0	0	1
--

Run_OpenFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	187	0	3	-1	0
END_LINE
finishport

3	3	910	1245	1	0	1	mandatory= 0	0	1
--

Run_CloseFILE_WriteOnly_String	2	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	910	1270	2	0	1	mandatory= 0	0	1
--

Run_WriteFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	186	0	3	-1	0
END_LINE
finishport

3	1	910	1230	3	0	1	mandatory= 0	0	1
err

Run_OpenFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	910	1255	4	0	1	mandatory= 0	0	1
err

Run_CloseFILE_WriteOnly_String	3	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
finishport

3	1	910	1280	5	0	1	mandatory= 0	0	1
err

Run_WriteFILE_WriteOnly_String	4	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventManRstAnd2
_
186	0	0	0
_
_
This is a 2-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.
EventTwoInputManRstAnd
0
_
705	1280	720	1326
1	0
startport

2	-1	705	1290	0	0	1	mandatory= 0	0	1


And1	0	EndOfFunc
BEGIN_LINE
1	185	2	2	-1	1
915	1270
915	1205
700	1205
700	1290
END_LINE
startport

2	-1	705	1305	2	0	1	mandatory= 0	0	1


And2	0	EndOfFunc
BEGIN_LINE
1	162	4	2	-1	0
690	1220
690	1305
END_LINE
startport

2	3	705	1320	2	0	1	mandatory= 0	0	1
R

Reset	0	EndOfFunc
BEGIN_LINE
1	162	2	2	-1	1
645	1150
645	1320
END_LINE
finishport

3	-1	720	1300	0	0	1	mandatory= 0	0	1


And1	1	And2	1	EndOfFunc
BEGIN_LINE
0	185	1	3	-1	0
END_LINE
finishport

3	-1	720	1320	1	0	1	mandatory= 0	0	1


Reset	1	EndOfFunc
BEGIN_LINE
0	185	2	3	-1	0
END_LINE
END_BLOCK
BEGIN_BLOCK

trigger_eventor3
_
187	0	0	0
_
_
This is a 3-input event OR.
EventThreeInputOr
0
_
1060	1315	1080	1366
1	0
startport

2	-1	1060	1325	0	0	1	mandatory= 0	0	1


Or1	0	EndOfFunc
BEGIN_LINE
1	185	0	2	-1	1
1055	1220
1055	1325
END_LINE
startport

2	-1	1060	1340	1	0	1	mandatory= 0	0	1


Or2	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
startport

2	-1	1060	1355	2	0	1	mandatory= 0	0	1


Or3	0	EndOfFunc
BEGIN_LINE
0	-1	0	2	-1	0
END_LINE
finishport

3	-1	1080	1340	0	0	1	mandatory= 0	0	1


Or1	1	Or2	1	Or3	1	EndOfFunc
BEGIN_LINE
0	-1	0	3	-1	0
END_LINE
END_BLOCK
END_OF_BLOCKS
