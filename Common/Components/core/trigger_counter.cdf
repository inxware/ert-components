<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventCounter</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Event
    <Menu>Counters/Iterators
      <Menu>Counter</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an event counter with reset.</LongDescription>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Count</FunctionName>
  </Port>
  <Port>
    <Description>rst</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="0">Reset</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Count</FunctionName>
    <FunctionName argument="1">Reset</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Count</FunctionName>
    <FunctionName argument="2">Reset</FunctionName>
  </Port>
</Component>
