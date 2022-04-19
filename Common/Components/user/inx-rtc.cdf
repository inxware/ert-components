<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>inx-rtc.bmp</Icon>
    <Class>rtc</Class>
    <Module>usercomponents</Module>
    <Menu>System
        <Menu>RTC</Menu>
    </Menu>
    <ShortDescription>rtc</ShortDescription>
    <LongDescription>rtc</LongDescription>
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
        <CName>time</CName>
        <Description>seconds</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="1">set</FunctionName>
    </Port>
    <Port>
        <CName>finishset</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">set</FunctionName>
    </Port>
    <Port>
        <CName>read</CName>
        <Description>read</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">read</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>tz</CName>
        <Description>tz</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <CName>finishread</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>time</CName>
        <Description>utc seconds</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="1">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>year</CName>
        <Description>year</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="2">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>month</CName>
        <Description>month</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <FunctionName argument="3">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>dom</CName>
        <Description>dom</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <FunctionName argument="4">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>dow</CName>
        <Description>dow</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <FunctionName argument="5">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>hour</CName>
        <Description>hour</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>90</YCoordinate>
        <FunctionName argument="6">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>minute</CName>
        <Description>minute</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>100</YCoordinate>
        <FunctionName argument="7">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>second</CName>
        <Description>second</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>110</YCoordinate>
        <FunctionName argument="8">read</FunctionName>
    </Port>
</Component>