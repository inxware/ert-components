<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>time_clock</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Event
    <Menu>Clock</Menu>
  </Menu>
  <LongDescription>Creates periodic timed events.</LongDescription>
  <Parameter>
    <Name>Time Interval (uSecs)</Name>
    <DataType>I</DataType>
    <DefaultValue>1000000</DefaultValue>
    <MinValue>1</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Time interval between events.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Periodic Flag</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Periodic if TRUE, single delay only if FALSE. </Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Retriggerable Flag</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Retriggerable if TRUE. </Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Port>
    <Description>start</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">start</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>period</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="1">start</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>rpt</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="2">start</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>retrig</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="3">start</FunctionName>
  </Port>
  <Port>
    <Description>clk</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="1">tick</FunctionName>
  </Port>
  <Port>
    <Description>stop</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="0">stop</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="4">start</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="1">stop</FunctionName>
  </Port>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">tick</FunctionName>
  </Port>
</Component>
