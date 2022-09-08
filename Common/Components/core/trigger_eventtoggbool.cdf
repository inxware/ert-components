<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventToggleBoolean</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Event
    <Menu>Boolean Control
      <Menu>Toggle Bool</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a boolean that is toggled by an event.</LongDescription>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Toggle</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Toggle</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Toggle</FunctionName>
  </Port>
</Component>
