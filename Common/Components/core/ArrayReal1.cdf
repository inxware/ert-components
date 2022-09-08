<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ArrayReal1</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Buffers
      <Menu>Array
        <Menu>Real</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>Arrays of real value (floating point) variables can be stored in the component
The index must fall between 0 and the maximum size -1. No Error is generated for out of range indexes.</LongDescription>
  <Parameter>
    <Name>size</Name>
    <DataType>I</DataType>
    <DefaultValue>1024</DefaultValue>
    <MinValue>2</MinValue>
    <MaxValue>1024</MaxValue>
    <Description>Memory allocated for array</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>default value</Name>
    <DataType>F</DataType>
    <DefaultValue>NaN</DefaultValue>
    <Description>default value for uninitialised values</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <Description>clear</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_ArrayRealClear</FunctionName>
  </Port>
  <Port>
    <Description>write</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">Run_ArrayRealWrite</FunctionName>
  </Port>
  <Port>
    <Description>read</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_ArrayRealRead</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>index</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="2">Run_ArrayRealWrite</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>index</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="1">Run_ArrayRealRead</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <Description>data</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">Run_ArrayRealWrite</FunctionName>
  </Port>
  <Port>
    <DataType>F</DataType>
    <Description>data</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>65</YCoordinate>
    <FunctionName argument="2">Run_ArrayRealRead</FunctionName>
  </Port>
  <Port>
    <Description>clear</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="1">Run_ArrayRealClear</FunctionName>
  </Port>
  <Port>
    <Description>write</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_ArrayRealWrite</FunctionName>
  </Port>
  <Port>
    <Description>read</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_ArrayRealRead</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="4">Run_ArrayRealWrite</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="4">Run_ArrayRealRead</FunctionName>
  </Port>
</Component>
