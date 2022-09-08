<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ConvertorStringToBool</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Conversion
      <Menu>string-&gt;
        <Menu>bool</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a string to boolean converter.</LongDescription>
  <Port>
    <DataType>S</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ConvertorStringToBool</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_ConvertorStringToBool</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ConvertorStringToBool</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_ConvertorStringToBool</FunctionName>
  </Port>
</Component>
