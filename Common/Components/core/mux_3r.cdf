<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>MultiplexThreeInputReal</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Selector
      <Menu>Mux3
        <Menu>real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 3-input real multiplexer.</LongDescription>
  <Port>
    <DataType>F</DataType>
    <Description>i1</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="1">Run_Input1</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <Description>i2</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="1">Run_Input2</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <Description>i3</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="1">Run_Input3</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run_Input1</FunctionName>
    <FunctionName argument="2">Run_Input2</FunctionName>
    <FunctionName argument="2">Run_Input3</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_Input1</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="0">Run_Input2</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="0">Run_Input3</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_Input1</FunctionName>
    <FunctionName argument="3">Run_Input2</FunctionName>
    <FunctionName argument="3">Run_Input3</FunctionName>
  </Port>
</Component>
