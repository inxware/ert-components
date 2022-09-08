<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Constant_Int</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Deprecated
    <Menu>Constant
      <Menu>int</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an integer constant.</LongDescription>
  <Parameter>
    <Name>Integer Value</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>integer constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConstantInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ConstantInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Run_ConstantInt</FunctionName>
  </Port>
</Component>
