<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>inx-permanent_storage.bmp</Icon>
    <Class>permanent_storage_int</Class>
    <Module>usercomponents</Module>
    <Menu>System
        <Menu>Permanent Storage Int</Menu>
    </Menu>
    <ShortDescription>permanent_storage_int</ShortDescription>
    <LongDescription>permanent_storage_int</LongDescription>
    <Port>
        <CName>write</CName>
        <Description>Write</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">write</FunctionName>
    </Port>
    <Port>
        <CName>read</CName>
        <Description>Read</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>id</CName>
        <Description>id</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">write</FunctionName>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="2">write</FunctionName>
    </Port>
    <Port>
        <CName>finishwrite</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">write</FunctionName>
    </Port>
    <Port>
        <CName>finishread</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <CName>finisherr</CName>
        <Description>read error</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="2">read</FunctionName>
    </Port>
</Component>