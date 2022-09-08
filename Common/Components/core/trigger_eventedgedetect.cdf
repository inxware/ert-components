<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventEdgeDetect</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Event
    <Menu>Boolean Control
      <Menu>Edge Detect</Menu>
    </Menu>
  </Menu>
  <LongDescription>This object is asserted an event an edge is detected on a boolean.</LongDescription>
  <Parameter>
    <Name>Boolean Initialvalue</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>initial value</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Test</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Test</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Test</FunctionName>
  </Port>
</Component>
