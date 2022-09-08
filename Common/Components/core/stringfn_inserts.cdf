<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>InsertString</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>String Functions
      <Menu>insert</Menu>
    </Menu>
  </Menu>
  <LongDescription>This function inserts a string s2 into string s1 at the specified position.</LongDescription>
  <Parameter>
    <Name>index</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>1024</MaxValue>
    <Description>0-based index where to insert string</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>string to insert</Name>
    <DataType>S</DataType>
    <MinValue>0</MinValue>
    <MaxValue>1024</MaxValue>
    <Description>String to inser</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <Description>s1</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_InsertString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s2</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">Run_InsertString</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">Run_InsertString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="4">Run_InsertString</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_InsertString</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="5">Run_InsertString</FunctionName>
  </Port>
</Component>
