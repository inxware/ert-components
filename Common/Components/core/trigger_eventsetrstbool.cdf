<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventSetReset</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Event
    <Menu>Boolean Control
      <Menu>Set Reset Bool</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a boolean that can be set and reset by events.</LongDescription>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Reset</FunctionName>
    <FunctionName argument="1">Set</FunctionName>
  </Port>
  <Port>
    <Description>set</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Set</FunctionName>
  </Port>
  <Port>
    <Description>rst</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="0">Reset</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Reset</FunctionName>
    <FunctionName argument="2">Set</FunctionName>
  </Port>
</Component>
