<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DtvRemoteKey</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Deprecated
    <Menu>DTV
      <Menu>Remote Key</Menu>
    </Menu>
  </Menu>
  <LongDescription>Remote key providing events for button presses</LongDescription>
  <Port>
    <Description>menu</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
  <Port>
    <Description>ok</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <Description>exit</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
  <Port>
    <Description>up</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="4">run</FunctionName>
  </Port>
  <Port>
    <Description>down</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="5">run</FunctionName>
  </Port>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">run</FunctionName>
  </Port>
</Component>
