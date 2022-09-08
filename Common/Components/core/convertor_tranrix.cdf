<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ConvertorRealToInt</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Conversion
      <Menu>real-&gt;
        <Menu>integer</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a real to integer converter.</LongDescription>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConvertorRealToInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_ConvertorRealToInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ConvertorRealToInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_ConvertorRealToInt</FunctionName>
  </Port>
</Component>
