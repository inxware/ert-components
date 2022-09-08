<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DivisionInt</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Operators
      <Menu>divide
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input integer divider.</LongDescription>
  <Port>
    <DataType>I</DataType>
    <Description>N</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Run_DivisionInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>D</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Run_DivisionInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run_DivisionInt</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_DivisionInt</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_DivisionInt</FunctionName>
  </Port>
</Component>
