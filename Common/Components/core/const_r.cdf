<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Constant_Real</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Deprecated
    <Menu>Constant
      <Menu>real</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a real constant.</LongDescription>
  <Parameter>
    <Name>Real Constant</Name>
    <DataType>F</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>1.7E-308</MinValue>
    <MaxValue>1.7E+308</MaxValue>
    <Description>real constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConstantReal</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ConstantReal</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Run_ConstantReal</FunctionName>
  </Port>
</Component>
