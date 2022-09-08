<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>MultiplyTwoInputReal</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Operators
      <Menu>multiply
        <Menu>real (2 input)</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>2-input real multiplier.</LongDescription>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Run_MultiplyTwoInputReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Run_MultiplyTwoInputReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="3">Run_MultiplyTwoInputReal</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Run_MultiplyTwoInputReal</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_MultiplyTwoInputReal</FunctionName>
  </Port>
</Component>
