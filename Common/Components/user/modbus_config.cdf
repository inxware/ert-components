<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>modbus_config</Class>
    <Module>usercomponents</Module>
    <Menu>
        Peripherals
        <Menu>Modbus Config</Menu>
    </Menu>
    <ShortDescription>modbus_config</ShortDescription>
    <LongDescription>modbus_config</LongDescription>
    <Port>
        <CName>enable</CName>
        <Description>Enable</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">enable</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>id</CName>
        <Description>id</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">enable</FunctionName>
    </Port>
    <Port>
        <CName>finishenable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>100</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">enable</FunctionName>
    </Port>
    <Port>
        <CName>disable</CName>
        <Description>Disable</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">disable</FunctionName>
    </Port>
    <Port>
        <CName>finishdisable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>100</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">disable</FunctionName>
    </Port>
</Component>
