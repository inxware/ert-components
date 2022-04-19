<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>mqtt_subscribe</Class>
    <Module>usercomponents</Module>
    <Menu>
        Networking
        <Menu>MQTT Subscribe</Menu>
    </Menu>
    <ShortDescription>mqtt_subscribe</ShortDescription>
    <LongDescription>mqtt_subscribe</LongDescription>
    <Port>
        <CName>subscribe</CName>
        <Description>subscribe</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">subscribe</FunctionName>
    </Port>
    <Port>
        <CName>finishsubscribe</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>60</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">subscribe</FunctionName>
    </Port>
    <Port>
        <CName>finishevent</CName>
        <Description>Event</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>60</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="2">subscribe</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>event</CName>
        <Description>event</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>60</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <FunctionName argument="1">subscribe</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>topic</CName>
        <Description>topic</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">subscribe</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>qos</CName>
        <Description>qos</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="2">subscribe</FunctionName>
    </Port>
    <Port>
        <CName>unsubscribe</CName>
        <Description>unsubscribe</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>110</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">unsubscribe</FunctionName>
    </Port>
    <Port>
        <CName>finishunsubscribe</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>60</XCoordinate>
        <YCoordinate>110</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">unsubscribe</FunctionName>
    </Port>
</Component>
