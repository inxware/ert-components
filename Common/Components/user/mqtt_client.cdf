<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>mqtt_client</Class>
    <Hash>0xcdd0</Hash>
    <Module>usercomponents</Module>
    <Menu>
        Networking
        <Menu>MQTT Client</Menu>
    </Menu>
    <ShortDescription>mqtt_client</ShortDescription>
    <LongDescription>mqtt_client</LongDescription>
    <Parameter>
        <Name>client cert</Name>
        <DataType>S</DataType>
        <DefaultValue>client.pem</DefaultValue>
        <MinValue>0</MinValue>
        <MaxValue>0</MaxValue>
        <Description>Client
TLS certiicate (PEM
format)</Description>
        <ListPlacement>1</ListPlacement>
        <ArgPlacement>1</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>client key</Name>
        <DataType>S</DataType>
        <DefaultValue>client.key</DefaultValue>
        <MinValue>0</MinValue>
        <MaxValue>0</MaxValue>
        <Description>Client
TLS key
(base64)</Description>
        <ListPlacement>2</ListPlacement>
        <ArgPlacement>2</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>server cert</Name>
        <DataType>S</DataType>
        <DefaultValue>server.crt</DefaultValue>
        <MinValue>0</MinValue>
        <MaxValue>0</MaxValue>
        <Description>Server
root authority
certificate.</Description>
        <ListPlacement>3</ListPlacement>
        <ArgPlacement>3</ArgPlacement>
    </Parameter>
    <Port>
        <DataType>S</DataType>
        <CName>password</CName>
        <Description>password</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>130</YCoordinate>
        <FunctionName argument="1" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <CName>finishdisconnect</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>75</XCoordinate>
        <YCoordinate>150</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <DataType>B</DataType>
        <CName>tls</CName>
        <Description>tls</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <FunctionName argument="2" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <CName>disconnect</CName>
        <Description>disconnect</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>150</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0xb850">disconnect</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>username</CName>
        <Description>username</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>110</YCoordinate>
        <FunctionName argument="3" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>clientid</CName>
        <Description>clientid</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>90</YCoordinate>
        <FunctionName argument="4" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>port</CName>
        <Description>port</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="5" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>host</CName>
        <Description>host</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="6" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <CName>connect</CName>
        <Description>connect</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0xb097">connect</FunctionName>
    </Port>
    <Port>
        <CName>finishconnect</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>75</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="2" hash="0xb097">connect</FunctionName>
    </Port>
</Component>
