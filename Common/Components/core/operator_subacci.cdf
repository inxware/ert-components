<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SubAccumulateInt</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Deprecated
    <Menu>Operators
      <Menu>Sub Accumulate
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an integer subtract accumulator.</LongDescription>
  <Port>
    <DataType>I</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_SubAccumulateInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_SubAccumulateInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_SubAccumulateInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_SubAccumulateInt</FunctionName>
  </Port>
</Component>
