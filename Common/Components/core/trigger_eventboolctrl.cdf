<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventBooleanControl</Class>
  <Menu>Event
    <Menu>Boolean Control
      <Menu>Bool Control</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an event controlled by a boolean.</LongDescription>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Update</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Update</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>18</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Update</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>18</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Update</FunctionName>
  </Port>
</Component>
