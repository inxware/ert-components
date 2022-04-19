<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>inx-uart_config.bmp</Icon>
    <Class>uart_config</Class>
    <Module>usercomponents</Module>
    <Menu>System
        <Menu>UART Config</Menu>
    </Menu>
    <ShortDescription>uart_config</ShortDescription>
    <LongDescription>uart_config</LongDescription>
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
        <CName>baudrate</CName>
        <Description>Baudrate</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="1">enable</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>stopbits</CName>
        <Description>Stop Bits</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="2">enable</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>parity</CName>
        <Description>Parity</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="3">enable</FunctionName>
    </Port>
    <Port>
        <CName>finishenable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
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
        <XCoordinate>105</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">disable</FunctionName>
    </Port>
</Component>