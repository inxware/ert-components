<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SquareRootReal</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Functions
      <Menu>square root
        <Menu>real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input real square root.</LongDescription>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_SquareRootReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_SquareRootReal</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_SquareRootReal</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_SquareRootReal</FunctionName>
  </Port>
</Component>
