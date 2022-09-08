<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FindString</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>String Functions
      <Menu>find</Menu>
    </Menu>
  </Menu>
  <LongDescription>This function determines if a string s2 exists in string s1.</LongDescription>
  <Parameter>
    <Name>Reverse Search</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Biases search for backward searching</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Find String (-&gt; s2)</Name>
    <DataType>S</DataType>
    <Description>String to compare to unless overridden by port s2</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <Description>s1</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="1">Run_FindString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s2</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="2">Run_FindString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="3">Run_FindString</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="4">Run_FindString</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_FindString</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="6">Run_FindString</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>index</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="5">Run_FindString</FunctionName>
  </Port>
</Component>
