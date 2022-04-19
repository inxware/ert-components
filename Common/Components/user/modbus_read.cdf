<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>modbus_read</Class>
    <Module>usercomponents</Module>
    <Menu>
        Peripherals
        <Menu>Modbus Read</Menu>
    </Menu>
    <ShortDescription>modbus_read</ShortDescription>
    <LongDescription>modbus_read</LongDescription>
    <Port>
        <CName>read</CName>
        <Description>Set Channel</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>channel</CName>
        <Description>channel</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <CName>finishread</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>100</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>100</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">read</FunctionName>
    </Port>
</Component>
