<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>CatString</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>String Functions
      <Menu>cat</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-input string cat.</LongDescription>
  <Parameter>
    <Name>Append String (-&gt; s2)</Name>
    <DataType>S</DataType>
    <Description>String to concatenate - unless overridden by port s2</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <Description>s1</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_CatString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s2</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">Run_CatString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run_CatString</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_CatString</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">Run_CatString</FunctionName>
  </Port>
</Component>
