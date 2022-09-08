<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>AvailableLanguages</Class>
  <Menu>User Interface
    <Menu>National Language Support
      <Menu>Report Languages</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is the NLS language choice function block.</LongDescription>
  <Port>
    <Description>lookup</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">lookup</FunctionName>
  </Port>
  <Port>
    <Description>getnum</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="0">getnum</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>idx</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="1">lookup</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>lang</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">lookup</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>num</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="1">getnum</FunctionName>
  </Port>
  <Port>
    <Description>lookup</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">lookup</FunctionName>
  </Port>
  <Port>
    <Description>got</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="2">getnum</FunctionName>
  </Port>
</Component>
