<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>IndexedMultiplexer_Event</Class>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Identifier</Menu>
    </Menu>
  </Menu>
  <LongDescription>Triggering Event Identified by Index Output</LongDescription>
  <Parameter>
    <Name>Requires Resetting</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Requires manual resetting each time it is triggered before it can be triggered again.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>1</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input1</FunctionName>
  </Port>
  <Port>
    <Description>2</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input2</FunctionName>
  </Port>
  <Port>
    <Description>3</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input3</FunctionName>
  </Port>
  <Port>
    <Description>4</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input4</FunctionName>
  </Port>
  <Port>
    <Description>5</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input5</FunctionName>
  </Port>
  <Port>
    <Description>6</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input6</FunctionName>
  </Port>
  <Port>
    <Description>7</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input7</FunctionName>
  </Port>
  <Port>
    <Description>8</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_Input8</FunctionName>
  </Port>
  <Port>
    <Description>Go</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">Run_Input1</FunctionName>
    <FunctionName argument="2">Run_Input2</FunctionName>
    <FunctionName argument="2">Run_Input3</FunctionName>
    <FunctionName argument="2">Run_Input4</FunctionName>
    <FunctionName argument="2">Run_Input5</FunctionName>
    <FunctionName argument="2">Run_Input6</FunctionName>
    <FunctionName argument="2">Run_Input7</FunctionName>
    <FunctionName argument="2">Run_Input8</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Ind</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_Input1</FunctionName>
    <FunctionName argument="1">Run_Input2</FunctionName>
    <FunctionName argument="1">Run_Input3</FunctionName>
    <FunctionName argument="1">Run_Input4</FunctionName>
    <FunctionName argument="1">Run_Input5</FunctionName>
    <FunctionName argument="1">Run_Input6</FunctionName>
    <FunctionName argument="1">Run_Input7</FunctionName>
    <FunctionName argument="1">Run_Input8</FunctionName>
  </Port>
  <Port>
    <Description>Reset</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>155</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Reset</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>155</YCoordinate>
    <FunctionName argument="1">Reset</FunctionName>
  </Port>
</Component>
