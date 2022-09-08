<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>ArrayString</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Deprecated
    <Menu>Array
      <Menu>String</Menu>
    </Menu>
  </Menu>
  <Parameter>
    <Name>size</Name>
    <DataType>I</DataType>
    <DefaultValue>1024</DefaultValue>
    <MinValue>2</MinValue>
    <MaxValue>1024</MaxValue>
    <Description>Memory allocated for array</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>wr</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_ArrayStringWrite</FunctionName>
  </Port>
  <Port>
    <Description>rd</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_ArrayStringRead</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>chw</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="2">Run_ArrayStringWrite</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>chr</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Run_ArrayStringRead</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>sIn</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="1">Run_ArrayStringWrite</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>sOut</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="2">Run_ArrayStringRead</FunctionName>
  </Port>
  <Port>
    <Description>wr</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_ArrayStringWrite</FunctionName>
  </Port>
  <Port>
    <Description>rd</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">Run_ArrayStringRead</FunctionName>
  </Port>
</Component>
