<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>inx-pwm.bmp</Icon>
    <Class>pwm</Class>
    <Module>usercomponents</Module>
    <Menu>System
        <Menu>PWM</Menu>
    </Menu>
    <ShortDescription>pwm</ShortDescription>
    <LongDescription>pwm</LongDescription>
    <Port>
        <CName>config</CName>
        <Description>config</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">config</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>hz</CName>
        <Description>hz</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="1">config</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>pin</CName>
        <Description>pin</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="2">config</FunctionName>
    </Port>
    <Port>
        <CName>finishconfig</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">config</FunctionName>
    </Port>
    <Port>
        <CName>enable</CName>
        <Description>enable</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">enable</FunctionName>
    </Port>
    <Port>
        <DataType>B</DataType>
        <CName>enable</CName>
        <Description>enable</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="1">enable</FunctionName>
    </Port>
    <Port>
        <CName>finishenable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">enable</FunctionName>
    </Port>
    <Port>
        <CName>duty</CName>
        <Description>duty</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">duty</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>percent</CName>
        <Description>percent</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <FunctionName argument="1">duty</FunctionName>
    </Port>
    <Port>
        <CName>finishduty</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">duty</FunctionName>
    </Port>
</Component>