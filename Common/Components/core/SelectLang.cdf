<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SelectLang</Class>
  <Menu>User Interface
    <Menu>National Language Support
      <Menu>Switch Language</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is the NLS language selection function block.</LongDescription>
  <Port>
    <Description>select</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">select</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>lang</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">select</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>valid</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">select</FunctionName>
  </Port>
  <Port>
    <Description>select</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">select</FunctionName>
  </Port>
</Component>
