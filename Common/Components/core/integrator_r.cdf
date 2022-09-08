<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Integrator_Real</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Operators
      <Menu>integrator
        <Menu>real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a real integrator.</LongDescription>
  <Parameter>
    <Name>Offset (can use x.yE+nn notation)</Name>
    <DataType>F</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-1.7E308</MinValue>
    <MaxValue>1.7E+308</MaxValue>
    <Description>Offset value for cumulative sum.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>F</DataType>
    <Description>offset</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_IntegratorReal</FunctionName>
    <FunctionName argument="1">Run_IntegratorRealReset</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <Description>val</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Run_IntegratorReal</FunctionName>
  </Port>
  <Port>
    <Description>reset</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="0">Run_IntegratorRealReset</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run_IntegratorReal</FunctionName>
    <FunctionName argument="2">Run_IntegratorRealReset</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_IntegratorReal</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_IntegratorReal</FunctionName>
    <FunctionName argument="3">Run_IntegratorRealReset</FunctionName>
  </Port>
</Component>
