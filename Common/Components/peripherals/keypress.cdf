<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>GUI_keypress</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>User Interface
    <Menu>Hardware
      <Menu>Keyboard</Menu>
    </Menu>
  </Menu>
  <LongDescription>Detects when a key is pressed and asserts the ASCII or extended key code integer</LongDescription>
  <Port>
    <Description>key</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="4">hit</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ascii</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="1">hit</FunctionName>
  </Port>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">hit</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>char</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="2">hit</FunctionName>
  </Port>
  <Port>
    <Description>control</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="5">hit</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>id</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="3">hit</FunctionName>
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
