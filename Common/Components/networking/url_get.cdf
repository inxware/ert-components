<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>UrlGet</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Networking
    <Menu>HTTP
      <Menu>URL Client</Menu>
    </Menu>
  </Menu>
  <LongDescription>Retrieves URLs using get or post and returns the data to a file location or as an output string. </LongDescription>
  <Parameter>
    <Name>URL</Name>
    <DataType>S</DataType>
    <Description>URL for the get method (provides base for post)</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Post Data</Name>
    <DataType>S</DataType>
    <Description>parameters to be appended to the URL.</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Local Path</Name>
    <DataType>S</DataType>
    <Description>Local path to save data - empty means no save.</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use POST (not GET) </Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Forces a post method using the URL string</Description>
    <ListPlacement>4</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use URL Path</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Uses N levels of path when storing files to /Path</Description>
    <ListPlacement>5</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Path Levels to Remove (N) </Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>10000</MaxValue>
    <Description>If Use URL path is set then this parameter specfified how many path levels to remove from the beginning of the path.</Description>
    <ListPlacement>6</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>duty gap (ms)</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-100</MinValue>
    <MaxValue>1000000</MaxValue>
    <Description>Duty gap allows a pause in ms to be inserted between each data chunk get to reduce network flooding issues. </Description>
    <ListPlacement>7</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Timeout </Name>
    <DataType>I</DataType>
    <DefaultValue>60</DefaultValue>
    <MinValue>-1</MinValue>
    <MaxValue>3600</MaxValue>
    <Description>Times out the initial http request and returns Error and http code event if no connection is made. </Description>
    <ListPlacement>8</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use Cookies</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Enable/disable the URL to send and receive cookies</Description>
    <ListPlacement>9</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Host Username</Name>
    <DataType>S</DataType>
    <Description>Username of the host site.</Description>
    <ListPlacement>10</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Host Password</Name>
    <DataType>S</DataType>
    <Description>Password of the host site.</Description>
    <ListPlacement>11</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Client SSL Certificate</Name>
    <DataType>S</DataType>
    <Description>Name of SSL certificate to use to communicate with site.</Description>
    <ListPlacement>12</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Client SSL Certificate password</Name>
    <DataType>S</DataType>
    <Description>Password to use the local SSL certificate.</Description>
    <ListPlacement>13</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Host CA SSL Certificate</Name>
    <DataType>S</DataType>
    <Description>Filename and optional prefix path of PEM format Host CA certificate.</Description>
    <ListPlacement>14</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Certificate file locations (Host &amp; client)</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>5</MaxValue>
    <Description>Locations@ 0:apps/, 1:user, 2:user-app, 3:devman/certs 4: system/, 5: os root</Description>
    <ListPlacement>15</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use Proxy Server</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Enable/disable the system proxy server settings in file ./sysinfo/proxies.</Description>
    <ListPlacement>16</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Always overwrite local file</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Set to true to override skipping downloads when file server reports a file</Description>
    <ListPlacement>17</ListPlacement>
  </Parameter>
  <Port>
    <Description>Get</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Getting</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="21">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Abort</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>255</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">abort</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>255</YCoordinate>
    <FunctionName argument="1">abort</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>URL</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>query str</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>local path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>URLdirDump</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="4">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>idle time</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="5">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Response</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="15">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Data Ready</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="22">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="24">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>E-num</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="18">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>E-str</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="19">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Complete</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="23">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>Cookies on</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="6">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>Force Write</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="14">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Save cookies</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>220</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">savecookies</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>220</YCoordinate>
    <FunctionName argument="1">savecookies</FunctionName>
  </Port>
  <Port>
    <Description>Clear cookies</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>230</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">clearcookies</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>230</YCoordinate>
    <FunctionName argument="1">clearcookies</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Headers</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="7">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Server user</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="8">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>pass</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>160</YCoordinate>
    <FunctionName argument="9">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Certificate name</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="10">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>pass</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <FunctionName argument="11">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>Use POST</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="13">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>Proxy on</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <FunctionName argument="12">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Headers</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="16">getpost</FunctionName>
  </Port>
  <Port>
    <Description>Get next</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>200</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">getmoredata</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>bytes</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="20">getpost</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Final URL</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="17">getpost</FunctionName>
  </Port>
</Component>
