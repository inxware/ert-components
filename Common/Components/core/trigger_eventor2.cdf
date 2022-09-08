<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventTwoInputOr</Class>
  <Block>
    <Type>Or_Event</Type>
    <TypeVariant>norm_2</TypeVariant>
  </Block>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Event OR
        <Menu>OR2</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input event OR.</LongDescription>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Or1</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="0">Or2</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>20</XCoordinate>
    <YCoordinate>21</YCoordinate>
    <FunctionName argument="1">Or1</FunctionName>
    <FunctionName argument="1">Or2</FunctionName>
  </Port>
</Component>
