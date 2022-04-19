<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>modbus_write</Class>
    <Module>usercomponents</Module>
    <Menu>
        Peripherals
        <Menu>Modbus Write</Menu>
    </Menu>
    <ShortDescription>modbus_write</ShortDescription>
    <LongDescription>modbus_write</LongDescription>
    <Port>
        <CName>write</CName>
        <Description>Set</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">write</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>channel</CName>
        <Description>channel</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">write</FunctionName>
    </Port>
    <Port>
        <CName>finishwrite</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>100</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">write</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="2">write</FunctionName>
    </Port>
</Component>
