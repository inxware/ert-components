<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>IndexedDemultiplexer_Event</Class>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Switch</Menu>
    </Menu>
  </Menu>
  <LongDescription>Switches Event to Indexed Output</LongDescription>
  <Parameter>
    <Name>Index start</Name>
    <DataType>I</DataType>
    <DefaultValue>1</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Value for firing port 1</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>Go</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Ind</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Off</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <FunctionName argument="2">Run</FunctionName>
  </Port>
  <Port>
    <Description>1</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run</FunctionName>
  </Port>
  <Port>
    <Description>2</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="4">Run</FunctionName>
  </Port>
  <Port>
    <Description>3</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="5">Run</FunctionName>
  </Port>
  <Port>
    <Description>4</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="6">Run</FunctionName>
  </Port>
  <Port>
    <Description>5</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="7">Run</FunctionName>
  </Port>
  <Port>
    <Description>6</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="8">Run</FunctionName>
  </Port>
  <Port>
    <Description>7</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="9">Run</FunctionName>
  </Port>
  <Port>
    <Description>8</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="10">Run</FunctionName>
  </Port>
  <Port>
    <Description>Ovf</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>155</YCoordinate>
    <FunctionName argument="11">Run</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="12">Run</FunctionName>
  </Port>
</Component>
