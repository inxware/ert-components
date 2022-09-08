<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>PowerInt</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Functions
      <Menu>power
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input integer power.</LongDescription>
  <Port>
    <DataType>I</DataType>
    <Description>X</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_PowerInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>y</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">Run_PowerInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run_PowerInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_PowerInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_PowerInt</FunctionName>
  </Port>
</Component>
