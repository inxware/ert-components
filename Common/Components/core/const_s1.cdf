<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Constant_String1</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Constant
      <Menu>string</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a string constant.</LongDescription>
  <Parameter>
    <Name>String Constant</Name>
    <DataType>S</DataType>
    <Description>string constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_ConstantString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConstantString</FunctionName>
  </Port>
</Component>
