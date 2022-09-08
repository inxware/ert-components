<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ToLowerString</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>String Functions
      <Menu>to lower</Menu>
    </Menu>
  </Menu>
  <LongDescription>This function converts a string to lowercase.</LongDescription>
  <Port>
    <DataType>S</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ToLowerString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_ToLowerString</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ToLowerString</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_ToLowerString</FunctionName>
  </Port>
</Component>
