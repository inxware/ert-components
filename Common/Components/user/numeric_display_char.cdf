<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>ndc</Class>
    <Module>usercomponents</Module>
    <Menu>
        Peripherals
        <Menu>Numeric Display Char</Menu>
    </Menu>
    <ShortDescription>numeric_display_char</ShortDescription>
    <LongDescription>numeric_display_char</LongDescription>
    <Port>
        <CName>set</CName>
        <Description>set</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">set</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>char</CName>
        <Description>number</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">set</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>displayno</CName>
        <Description>Display No</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="2">set</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>index</CName>
        <Description>Pos</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <FunctionName argument="3">set</FunctionName>
    </Port>
    <Port>
        <CName>finishset</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>75</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">set</FunctionName>
    </Port>
</Component>
