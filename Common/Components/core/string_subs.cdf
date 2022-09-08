<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SubString</Class>
  <Menu>Data Utilities
    <Menu>String Functions
      <Menu>substring</Menu>
    </Menu>
  </Menu>
  <LongDescription>This function outputs a substring specified by a start index and length.</LongDescription>
  <Parameter>
    <Name>start</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>1024</MaxValue>
    <Description>0-based index to start string extract</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>length</Name>
    <DataType>I</DataType>
    <DefaultValue>1</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>1024</MaxValue>
    <Description>Number of characters to extract</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_SubString</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>start</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="2">Run_SubString</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>length</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">Run_SubString</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="4">Run_SubString</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_SubString</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="5">Run_SubString</FunctionName>
  </Port>
</Component>
