<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>uart_config</Class>
    <Hash>0xd172</Hash>
    <Module>usercomponents</Module>
    <Menu>
        System
        <Menu>UART Config</Menu>
    </Menu>
    <ShortDescription>uart_config</ShortDescription>
    <LongDescription>uart_config</LongDescription>
    <Port>
        <CName>enable</CName>
        <Description>Enable</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>0</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>baudrate</CName>
        <Description>Baudrate</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="1" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>stopbits</CName>
        <Description>Stop Bits</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="2" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>parity</CName>
        <Description>Parity</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <FunctionName argument="3" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <CName>finishenable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>0</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <CName>disable</CName>
        <Description>Disable</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0x3d7a">disable</FunctionName>
    </Port>
    <Port>
        <CName>finishdisable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0x3d7a">disable</FunctionName>
    </Port>
</Component>
