<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>pwm</Class>
    <Hash>0xc6e7</Hash>
    <Module>usercomponents</Module>
    <Menu>
        Peripherals
        <Menu>PWM</Menu>
    </Menu>
    <ShortDescription>pwm</ShortDescription>
    <LongDescription>pwm</LongDescription>
    <Port>
        <CName>config</CName>
        <Description>config</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>0</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0xd74e">config</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>hz</CName>
        <Description>hz</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="1" hash="0xd74e">config</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>pin</CName>
        <Description>pin</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="2" hash="0xd74e">config</FunctionName>
    </Port>
    <Port>
        <CName>finishconfig</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>0</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0xd74e">config</FunctionName>
    </Port>
    <Port>
        <CName>enable</CName>
        <Description>enable</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <DataType>B</DataType>
        <CName>enable</CName>
        <Description>enable</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>80</YCoordinate>
        <FunctionName argument="1" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <CName>finishenable</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>60</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0x35b3">enable</FunctionName>
    </Port>
    <Port>
        <CName>duty</CName>
        <Description>duty</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>100</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0x8e8">duty</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>percent</CName>
        <Description>percent</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>120</YCoordinate>
        <FunctionName argument="1" hash="0x8e8">duty</FunctionName>
    </Port>
    <Port>
        <CName>finishduty</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>105</XCoordinate>
        <YCoordinate>100</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0x8e8">duty</FunctionName>
    </Port>
</Component>
