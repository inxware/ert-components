<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventThreeInputManRstAnd</Class>
  <Block>
    <Type>And_Event</Type>
    <TypeVariant>manu_3</TypeVariant>
  </Block>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Manual AND
        <Menu>AND3</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 3-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.</LongDescription>
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
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">And3</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">And1</FunctionName>
    <FunctionName argument="1">And2</FunctionName>
    <FunctionName argument="1">And3</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="1">Reset</FunctionName>
  </Port>
  <Port>
    <Description>R</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="0">Reset</FunctionName>
  </Port>
</Component>
