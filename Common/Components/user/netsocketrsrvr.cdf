<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Network Socket Server</ShortDescription>
        <LongDescription>Listens on specified port</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                Sockets
                <Menu>Socket Server</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Height>180</Height>
        <Text>Sck Server</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.5</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>netsocketsrvr</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x5A86</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>b7a7062f</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Interface</Name>
            <DataType>S</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Interface (IF)</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Port</Name>
            <DataType>I</DataType>
            <DefaultValue>8000</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>100000</MaxValue>
            <Description>Listening Port</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>udp</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>tcp/udp mode</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>open</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>close</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>sendData</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>open</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>open</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>close</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>close</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>send</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <CName>send</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>---</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>opened</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>received</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <Wcet>0</Wcet>
            <CName>received</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>data</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>90</YCoordinate>
            <CName>data</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>size</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>size</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>IF</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>IF</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>port</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>port</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>data</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>inData</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>inSize</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>inSize</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>source</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>source</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>---</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <Wcet>0</Wcet>
            <CName>closed</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>closeError</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>---</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <Wcet>0</Wcet>
            <CName>sent</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <Wcet>0</Wcet>
            <CName>sendError</CName>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
