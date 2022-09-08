<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>wall_clock</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>System
    <Menu>Local
      <Menu>Date &amp; Time</Menu>
    </Menu>
  </Menu>
  <LongDescription>CSets and Retrieves the current system time.</LongDescription>
  <Port>
    <Description>get</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">get</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Date String</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>98</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">get</FunctionName>
  </Port>
  <Port>
    <Description>------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>98</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">get</FunctionName>
  </Port>
  <Port>
    <Description>set</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>65</YCoordinate>
    <FunctionName argument="0">set</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>time</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">set</FunctionName>
  </Port>
  <Port>
    <Description>-----</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>98</XCoordinate>
    <YCoordinate>65</YCoordinate>
    <FunctionName argument="2">set</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Mins 1970</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>98</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">get</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>&amp; secs</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>98</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="3">get</FunctionName>
  </Port>
</Component>
