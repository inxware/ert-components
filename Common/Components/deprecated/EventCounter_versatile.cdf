<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventCounterVersatile</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Deprecated
    <Menu>Event Counter Versatile</Menu>
  </Menu>
  <LongDescription>event counter providing option to set threshold with an input value.</LongDescription>
  <Parameter>
    <Name>Threshold</Name>
    <DataType>I</DataType>
    <DefaultValue>1000</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Threshold value for ovf event.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>start</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_StartEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>stop</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="0">Run_StopEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>rst</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="0">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>Inc.</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="0">Run_CountEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>thresh</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="1">Run_CountEventCounter</FunctionName>
    <FunctionName argument="1">Run_DecrementEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="2">Run_CountEventCounter</FunctionName>
    <FunctionName argument="2">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="1">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="3">Run_CountEventCounter</FunctionName>
    <FunctionName argument="2">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>ovf</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="4">Run_CountEventCounter</FunctionName>
    <FunctionName argument="4">Run_DecrementEventCounter</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="3">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="3">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>Dec</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="0">Run_DecrementEventCounter</FunctionName>
  </Port>
</Component>
