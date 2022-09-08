<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ConvertorStringToInt</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Conversion
      <Menu>string-&gt;
        <Menu>integer</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a string to integer converter.</LongDescription>
  <Port>
    <DataType>S</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConvertorStringToInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_ConvertorStringToInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ConvertorStringToInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_ConvertorStringToInt</FunctionName>
  </Port>
</Component>
