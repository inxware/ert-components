<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Constant_Bool</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Deprecated
    <Menu>Constant
      <Menu>bool</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a boolean constant.</LongDescription>
  <Parameter>
    <Name>Boolean Constant</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>boolean constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConstantBool</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ConstantBool</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Run_ConstantBool</FunctionName>
  </Port>
</Component>
