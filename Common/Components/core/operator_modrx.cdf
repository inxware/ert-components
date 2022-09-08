<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ModulusReal</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Math
    <Menu>Functions
      <Menu>modulus
        <Menu>real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input real modulus.</LongDescription>
  <Port>
    <DataType>F</DataType>
    <Description>N</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">Run_ModulusReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <Description>D</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="2">Run_ModulusReal</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="3">Run_ModulusReal</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Run_ModulusReal</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_ModulusReal</FunctionName>
  </Port>
</Component>
