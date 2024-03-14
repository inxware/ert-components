<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Retrieves URLs using get or post and returns the data to a file location or as an output string. </LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                HTTP
                <Menu>URL Client</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Height>285</Height>
        <Text>HTTP Client</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.5</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>UrlGet</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x3CB2</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>6a545345</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>URL</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>URL for the get method (provides base for post)</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Post Data</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>parameters to be appended to the URL.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Local Path</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Local path to save data - empty means no save.</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use POST (not GET) </Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>

            <Description>Forces a post method using the URL string</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use URL Path</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Uses N levels of path when storing files to /Path</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Path Levels to Remove (N) </Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>10000</MaxValue>
            <Description>If Use URL path is set then this parameter specfified how many path levels to remove from the beginning of the path.</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>duty gap (ms)</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-100</MinValue>
            <MaxValue>1000000</MaxValue>
            <Description>Duty gap allows a pause in ms to be inserted between each data chunk get to reduce network flooding issues. </Description>
            <ListPlacement>7</ListPlacement>
            <ArgPlacement>7</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Timeout </Name>
            <DataType>I</DataType>
            <DefaultValue>60</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>3600</MaxValue>
            <Description>Times out the initial http request and returns Error and http code event if no connection is made. </Description>
            <ListPlacement>8</ListPlacement>
            <ArgPlacement>8</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use Cookies</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Enable/disable the URL to send and receive cookies</Description>
            <ListPlacement>9</ListPlacement>
            <ArgPlacement>9</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Host Username</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Username of the host site.</Description>
            <ListPlacement>10</ListPlacement>
            <ArgPlacement>10</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Host Password</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Password of the host site.</Description>
            <ListPlacement>11</ListPlacement>
            <ArgPlacement>11</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Client SSL Certificate</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Name of SSL certificate to use to communicate with site.</Description>
            <ListPlacement>12</ListPlacement>
            <ArgPlacement>12</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Client SSL Certificate password</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Password to use the local SSL certificate.</Description>
            <ListPlacement>13</ListPlacement>
            <ArgPlacement>13</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Host CA SSL Certificate</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Filename and optional prefix path of PEM format Host CA certificate.</Description>
            <ListPlacement>14</ListPlacement>
            <ArgPlacement>14</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Certificate file locations (Host &amp; client)</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>5</MaxValue>
            <Description>Locations@ 0:apps/, 1:user, 2:user-app, 3:devman/certs 4: system/, 5: os root</Description>
            <ListPlacement>15</ListPlacement>
            <ArgPlacement>15</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use Proxy Server</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Enable/disable the system proxy server settings in file ./sysinfo/proxies.</Description>
            <ListPlacement>16</ListPlacement>
            <ArgPlacement>16</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Always overwrite local file</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Set to true to override skipping downloads when file server reports a file</Description>
            <ListPlacement>17</ListPlacement>
            <ArgPlacement>17</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>getpost</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>abort</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>savecookies</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>clearcookies</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>getmoredata</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>Get</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>Get</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>Getting</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Getting</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Abort</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>255</YCoordinate>
            <CName>Abort</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>255</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>URL</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>URL</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>query str</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>query_str</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>local path</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>local_path</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>URLdirDump</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>URLdirDump</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>idle time</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>idle_time</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Response</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>Response</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Data Ready</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Data_Ready</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Error</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>E-num</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>E_num</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>E-str</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <CName>E_str</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Complete</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Complete</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>Cookies on</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>Cookies_on</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>Force Write</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>90</YCoordinate>
            <CName>Force_Write</CName>
            <Function argument="14">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Save cookies</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <CName>Save_cookies</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Clear cookies</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>230</YCoordinate>
            <CName>Clear_cookies</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>230</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Headers</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>Headers</CName>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Server user</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>Server_user</CName>
            <Function argument="8">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>pass</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <CName>pass</CName>
            <Function argument="9">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Certificate name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <CName>Certificate_name</CName>
            <Function argument="10">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>pass</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>185</YCoordinate>
            <CName>pass</CName>
            <Function argument="11">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>Use POST</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>Use_POST</CName>
            <Function argument="13">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>Proxy on</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>Proxy_on</CName>
            <Function argument="12">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Headers</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>Headers</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Get next</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>200</YCoordinate>
            <CName>Get_next</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>bytes</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>bytes</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Final URL</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>Final_URL</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
