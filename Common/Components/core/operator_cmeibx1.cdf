<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ComparatorGreaterEqualInt1</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Operators
      <Menu>&gt;=
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input integer comparator.</LongDescription>
  <Parameter>
    <Name>Constant</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Comparator constant</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>I</DataType>
    <Description>A</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Run_ComparatorGreaterEqualInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>B</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Run_ComparatorGreaterEqualInt</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_ComparatorGreaterEqualInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ComparatorGreaterEqualInt</FunctionName>
  </Port>
  <Port>
    <Description>t</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_ComparatorGreaterEqualInt</FunctionName>
  </Port>
  <Port>
    <Description>f</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="5">Run_ComparatorGreaterEqualInt</FunctionName>
  </Port>
</Component>
