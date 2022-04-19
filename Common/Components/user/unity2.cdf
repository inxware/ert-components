<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>unity2.bmp</Icon>
    <Class>unity2</Class>
    <Module>usercomponents</Module>
    <Menu>System
        <Menu>Unity</Menu>
    </Menu>
    <ShortDescription>unity</ShortDescription>
    <LongDescription>unity</LongDescription>
    <Port>
        <CName>create</CName>
        <Description>create</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">create</FunctionName>
    </Port>
    <Port>
        <CName>finishcreate</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">create</FunctionName>
    </Port>
    <Port>
        <CName>finishevent</CName>
        <Description>Event</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="2">create</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>event</CName>
        <Description>event</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">create</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>id</CName>
        <Description>zone id</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">create</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>type</CName>
        <Description>Type</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="2">create</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>params</CName>
        <Description>Params</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="3">create</FunctionName>
    </Port>

    <Port>
        <CName>destroy</CName>
        <Description>Destroy</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">destroy</FunctionName>
    </Port>
    <Port>
        <CName>finishdestroy</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">destroy</FunctionName>
    </Port>

    <Port>
        <CName>setmedia</CName>
        <Description>Set Media</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">setmedia</FunctionName>
    </Port>
    <Port>
        <CName>finishsetmedia</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">setmedia</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>mediatype</CName>
        <Description>Media Type</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <FunctionName argument="1">setmedia</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>path</CName>
        <Description>Path</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>90</YCoordinate>
        <FunctionName argument="2">setmedia</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>text</CName>
        <Description>Text</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>100</YCoordinate>
        <FunctionName argument="3">setmedia</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>params</CName>
        <Description>Params</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>110</YCoordinate>
        <FunctionName argument="4">setmedia</FunctionName>
    </Port>

    <Port>
        <CName>stop</CName>
        <Description>Stop</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>120</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">stop</FunctionName>
    </Port>
    <Port>
        <CName>finishstop</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>120</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">stop</FunctionName>
    </Port>
</Component>