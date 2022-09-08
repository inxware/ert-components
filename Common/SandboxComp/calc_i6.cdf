<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ICalc</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>calculation
      <Menu>int (6 input)</Menu>
    </Menu>
  </Menu>
  <LongDescription>Integer Operations on a,b,c,d. supports +,-,*,/,%,^,abs(x),square(y),min(x,y),max(x,y),sin(output mult, angle),cos(output mult, angle)</LongDescription>
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
    <Description>B</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>C</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>D</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="4">run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>E</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="5">run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>F</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="6">run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="7">run</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="8">run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="9">run</FunctionName>
  </Port>
</Component>
