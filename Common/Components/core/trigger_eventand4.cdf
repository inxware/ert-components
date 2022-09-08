<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventFourInputLatchingAnd</Class>
  <Block>
    <Type>And_Event</Type>
    <TypeVariant>norm_4</TypeVariant>
  </Block>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Latching AND
        <Menu>AND4</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 4-input event latching AND with an external reset. Once triggered by receiving all inputs, will trigger on each input received until reset.</LongDescription>
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
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="0">And4</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">And1</FunctionName>
    <FunctionName argument="1">And2</FunctionName>
    <FunctionName argument="1">And3</FunctionName>
    <FunctionName argument="1">And4</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="1">Reset</FunctionName>
  </Port>
  <Port>
    <Description>R</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Reset</FunctionName>
  </Port>
</Component>
