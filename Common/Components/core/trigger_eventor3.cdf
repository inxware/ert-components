<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventThreeInputOr</Class>
  <Block>
    <Type>Or_Event</Type>
    <TypeVariant>norm_3</TypeVariant>
  </Block>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Event OR
        <Menu>OR3</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 3-input event OR.</LongDescription>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Or1</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="0">Or2</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="0">Or3</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>20</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="1">Or1</FunctionName>
    <FunctionName argument="1">Or2</FunctionName>
    <FunctionName argument="1">Or3</FunctionName>
  </Port>
</Component>
