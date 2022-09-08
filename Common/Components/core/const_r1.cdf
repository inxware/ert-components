<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Constant_Real1</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Constant
      <Menu>real</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a real constant.</LongDescription>
  <Parameter>
    <Name>Real Constant</Name>
    <DataType>F</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-1.7E308</MinValue>
    <MaxValue>1.7E+308</MaxValue>
    <Description>real constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_ConstantReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConstantReal</FunctionName>
  </Port>
</Component>
