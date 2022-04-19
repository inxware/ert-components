<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>mqtt_publish</Class>
    <Module>usercomponents</Module>
    <Menu>
        Networking
        <Menu>MQTT Publish</Menu>
    </Menu>
    <ShortDescription>mqtt_publish</ShortDescription>
    <LongDescription>mqtt_publish</LongDescription>
    <Port>
        <CName>publish</CName>
        <Description>publish</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">publish</FunctionName>
    </Port>
    <Port>
        <CName>finishpublish</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>100</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">publish</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>topic</CName>
        <Description>topic name</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">publish</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>payload</CName>
        <Description>Payload</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="2">publish</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>qos</CName>
        <Description>qos</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <FunctionName argument="3">publish</FunctionName>
    </Port>
</Component>
