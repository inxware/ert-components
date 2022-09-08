<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DtvRemoteKey2</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Peripherals
    <Menu>Remote Control Units</Menu>
  </Menu>
  <LongDescription>Remote key providing events for button presses. For devices with other types of input hardware, such as keyboards, this device will still pick relevant keypresses (navigation keys and numbers keys).</LongDescription>
  <Port>
    <Description>menu</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
  <Port>
    <Description>ok</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <Description>exit</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
  <Port>
    <Description>up</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="4">run</FunctionName>
  </Port>
  <Port>
    <Description>down</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="5">run</FunctionName>
  </Port>
  <Port>
    <Description>left</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="6">run</FunctionName>
  </Port>
  <Port>
    <Description>right</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="7">run</FunctionName>
  </Port>
  <Port>
    <Description>play</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <FunctionName argument="8">run</FunctionName>
  </Port>
  <Port>
    <Description>pause</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="9">run</FunctionName>
  </Port>
  <Port>
    <Description>stop</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="10">run</FunctionName>
  </Port>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">run</FunctionName>
  </Port>
  <Port>
    <Description>enable</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">enable</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="1">enable</FunctionName>
  </Port>
  <Port>
    <Description>disable</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="0">disable</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">disable</FunctionName>
  </Port>
</Component>
