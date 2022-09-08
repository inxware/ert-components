<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>TanReal</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Functions
      <Menu>trigonometry
        <Menu>tan</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is the tan function.</LongDescription>
  <Parameter>
    <Name>Radians:1, Degree:0</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Switch between radians and degrees mode.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>F</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_TanReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_TanReal</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_TanReal</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_TanReal</FunctionName>
  </Port>
</Component>
