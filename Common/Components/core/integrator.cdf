<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Integrator_Int</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Operators
      <Menu>integrator
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an integer integrator.</LongDescription>
  <Parameter>
    <Name>Offset</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Offset value for cumulative sum.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>I</DataType>
    <Description>offset</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_IntegratorInt</FunctionName>
    <FunctionName argument="1">Run_IntegratorIntReset</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>val</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Run_IntegratorInt</FunctionName>
  </Port>
  <Port>
    <Description>reset</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="0">Run_IntegratorIntReset</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run_IntegratorInt</FunctionName>
    <FunctionName argument="2">Run_IntegratorIntReset</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_IntegratorInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_IntegratorInt</FunctionName>
    <FunctionName argument="3">Run_IntegratorIntReset</FunctionName>
  </Port>
</Component>
