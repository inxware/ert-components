<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FIFO_Int</Class>
  <Block>
    <Type>Data_Processor</Type>
    <Text>FIFO</Text>
    <TextX>55</TextX>
    <TextY>40</TextY>
    <TextScale>1.3</TextScale>
    <TextVertical>1</TextVertical>
  </Block>
  <Menu>Data Utilities
    <Menu>Buffers
      <Menu>FIFO
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an integer fifo buffer.</LongDescription>
  <Parameter>
    <Name>Max. length</Name>
    <DataType>I</DataType>
    <DefaultValue>5</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>1000000</MaxValue>
    <Description>maximum buffer length</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>I</DataType>
    <Description>data</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Run_PushFIFOInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>data</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Run_PopFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>pop</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_PopFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>push</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="0">Run_PushFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>flush</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="0">Run_FlushFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>pop</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Run_PopFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="3">Run_PopFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>push</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="2">Run_PushFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_PushFIFOInt</FunctionName>
  </Port>
  <Port>
    <Description>flush</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="1">Run_FlushFIFOInt</FunctionName>
  </Port>
</Component>
