<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FIFO_Bool</Class>
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
        <Menu>bool</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a boolean fifo buffer.</LongDescription>
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
    <DataType>B</DataType>
    <Description>data</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Run_PushFIFOBool</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>data</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Run_PopFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>pop</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_PopFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>push</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="0">Run_PushFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>flush</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="0">Run_FlushFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>pop</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Run_PopFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="3">Run_PopFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>push</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="2">Run_PushFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_PushFIFOBool</FunctionName>
  </Port>
  <Port>
    <Description>flush</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="1">Run_FlushFIFOBool</FunctionName>
  </Port>
</Component>
