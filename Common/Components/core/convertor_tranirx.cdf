<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ConvertorIntToReal</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Conversion
      <Menu>integer-&gt;
        <Menu>real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an integer to real converter.</LongDescription>
  <Port>
    <DataType>I</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run</FunctionName>
  </Port>
</Component>
