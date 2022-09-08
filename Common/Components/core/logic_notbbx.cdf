<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>LogicNot</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Logic
    <Menu>NOT</Menu>
  </Menu>
  <LongDescription>This is a logical inverter.</LongDescription>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_LogicNot</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_LogicNot</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_LogicNot</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_LogicNot</FunctionName>
  </Port>
</Component>
