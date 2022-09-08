<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventSelfResetTwoInputLatchingAnd</Class>
  <Block>
    <Type>And_Event</Type>
    <TypeVariant>stdn_2</TypeVariant>
  </Block>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Resetting AND
        <Menu>AND2</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input event latching AND with an internal reset.</LongDescription>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">And1</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">And2</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>18</YCoordinate>
    <FunctionName argument="1">And1</FunctionName>
    <FunctionName argument="1">And2</FunctionName>
  </Port>
</Component>
