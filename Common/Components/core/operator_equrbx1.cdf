<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ComparatorEqualReal1</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Operators
      <Menu>=
        <Menu>real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input real comparator.</LongDescription>
  <Parameter>
    <Name>Constant (can use x.yE+nn notation)</Name>
    <DataType>F</DataType>
    <DefaultValue>0.0</DefaultValue>
    <MinValue>-1.7E+308</MinValue>
    <MaxValue>1.7E+308</MaxValue>
    <Description>Comparator constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Run_ComparatorEqualReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">Run_ComparatorEqualReal</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_ComparatorEqualReal</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ComparatorEqualReal</FunctionName>
  </Port>
  <Port>
    <Description>t</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_ComparatorEqualReal</FunctionName>
  </Port>
  <Port>
    <Description>f</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="5">Run_ComparatorEqualReal</FunctionName>
  </Port>
</Component>
