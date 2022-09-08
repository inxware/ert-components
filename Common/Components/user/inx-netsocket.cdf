<?xml version="1.0" encoding="UTF-8"?><Component>
    <ID>1</ID>
    <Class>netSocket</Class>
    <FbApiDescriptorHash>todo Api Descriptor</FbApiDescriptorHash>
    <Hashes>
        <NameHash_CRC16>0xF78A</NameHash_CRC16>
        <FbApiDescriptorHash_CRC16>todo Api Descriptor hash</FbApiDescriptorHash_CRC16>
    </Hashes>
    <Module>usercomponents</Module>
    <Menu>
        Networking
        <Menu>socket client</Menu>
    </Menu>
    <ShortDescription>network Socket Component</ShortDescription>
    <LongDescription>provides tcpip and udp socket data comms.</LongDescription>
    <Parameter>
        <Name>URL</Name>
        <DataType>S</DataType>
        <DefaultValue/>
        <MinValue>0</MinValue>
        <MaxValue>256</MaxValue>
        <Description>URL of the server socket. N/A for sockets</Description>
        <ListPlacement>1</ListPlacement>
        <ArgPlacement>1</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>port</Name>
        <DataType>I</DataType>
        <DefaultValue>8000</DefaultValue>
        <MinValue>0</MinValue>
        <MaxValue>65535</MaxValue>
        <Description>IP socket number to open</Description>
        <ListPlacement>2</ListPlacement>
        <ArgPlacement>2</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>tcp/udp</Name>
        <DataType>I</DataType>
        <DefaultValue>0</DefaultValue>
        <MinValue>0</MinValue>
        <MaxValue>1</MaxValue>
        <Description>0=tcp, 1=udp</Description>
        <ListPlacement>3</ListPlacement>
        <ArgPlacement>3</ArgPlacement>
    </Parameter>
    <Port>
        <CName/>
        <Description>open</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>close</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash_8bit="1" hash_CRC16="0x5E0C">close</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName/>
        <Description>port</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>send</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>120</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash_8bit="2" hash_CRC16="0x1027">send</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>error</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash_8bit="1" hash_CRC16="0x5E0C">close</FunctionName>
        <FunctionName argument="1" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>error</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>130</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash_8bit="2" hash_CRC16="0x1027">send</FunctionName>
        <FunctionName argument="2" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>----</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="3" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>error</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="4" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName/>
        <Description>url</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="2" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName/>
        <Description>data</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>130</YCoordinate>
        <FunctionName argument="1" hash_8bit="2" hash_CRC16="0x1027">send</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName/>
        <Description>errno</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>140</YCoordinate>
        <FunctionName argument="1" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
        <FunctionName argument="1" hash_8bit="1" hash_CRC16="0x5E0C">close</FunctionName>
        <FunctionName argument="1" hash_8bit="2" hash_CRC16="0x1027">send</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName/>
        <Description>data</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>90</YCoordinate>
        <FunctionName argument="2" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>received</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="5" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName/>
        <Description>size</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>140</YCoordinate>
        <FunctionName argument="2" hash_8bit="2" hash_CRC16="0x1027">send</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName/>
        <Description>size</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>100</YCoordinate>
        <FunctionName argument="3" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>---</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="6" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
        <FunctionName argument="2" hash_8bit="1" hash_CRC16="0x5E0C">close</FunctionName>
    </Port>
    <Port>
        <CName/>
        <Description>---</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>120</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="2" hash_8bit="2" hash_CRC16="0x1027">send</FunctionName>
        <FunctionName argument="7" hash_8bit="0" hash_CRC16="0x73B6">open</FunctionName>
    </Port>
</Component>
