<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DtvPvrList</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Deprecated
    <Menu>DTV
      <Menu>List Programmes</Menu>
    </Menu>
  </Menu>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Name</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>Valid</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
</Component>
