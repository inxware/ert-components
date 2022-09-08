<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventCounterVersatile1</Class>
  <Block>
    <Type>Event_Processor</Type>
  </Block>
  <Menu>Event
    <Menu>Counters/Iterators
      <Menu>Counter Versatile</Menu>
    </Menu>
  </Menu>
  <LongDescription>event counter providing option to set a maximum and minimum threshold with an input value.</LongDescription>
  <Parameter>
    <Name>Enable at Init</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Enable the counter at initialisation time if TRUE, else counter is disabled at initialisation time.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Initial Value</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Initial Value for count.</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Step Size</Name>
    <DataType>I</DataType>
    <DefaultValue>1</DefaultValue>
    <MinValue>1</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Value incremented/decremented on each count.</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Max. Threshold</Name>
    <DataType>I</DataType>
    <DefaultValue>1000</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Threshold value for ovf event.</Description>
    <ListPlacement>4</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Min. Threshold</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Threshold value for ovf event.</Description>
    <ListPlacement>5</ListPlacement>
  </Parameter>
  <Port>
    <Description>enable</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="0">Run_StartEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>inc</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="0">Run_CountEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>dec</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="0">Run_DecrementEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>rst</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>disable</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="0">Run_StopEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>max</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <FunctionName argument="1">Run_CountEventCounter</FunctionName>
    <FunctionName argument="1">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="1">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>min</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="2">Run_CountEventCounter</FunctionName>
    <FunctionName argument="2">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="2">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ini</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="3">Run_CountEventCounter</FunctionName>
    <FunctionName argument="3">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="3">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>step</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>130</YCoordinate>
    <FunctionName argument="4">Run_CountEventCounter</FunctionName>
    <FunctionName argument="4">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="4">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>cnt</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="5">Run_CountEventCounter</FunctionName>
    <FunctionName argument="5">Run_DecrementEventCounter</FunctionName>
    <FunctionName argument="5">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="6">Run_CountEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="6">Run_DecrementEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>ovf</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="7">Run_CountEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>ovf</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="7">Run_DecrementEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="6">Run_ResetEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="1">Run_StartEventCounter</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>60</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="1">Run_StopEventCounter</FunctionName>
  </Port>
</Component>
