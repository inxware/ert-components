<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ICalc</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>calculation
      <Menu>int (1 input)</Menu>
    </Menu>
  </Menu>
  <LongDescription>Integer Operations on 'a' and 'b'. +,-,*,/,%,^,abs(x),square(y),min(x,y),max(x,y),sin(output mult, angle),cos(output mult, angle)</LongDescription>
  <Parameter>
    <Name>Expression</Name>
    <DataType>S</DataType>
    <Description>The expression to calculate</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>I</DataType>
    <Description>A</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
</Component>
