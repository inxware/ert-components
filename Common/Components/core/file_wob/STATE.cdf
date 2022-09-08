<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>state</Class>
  <Menu>Software Structure
    <Menu>State Machine
      <Menu>State</Menu>
    </Menu>
  </Menu>
  <Parameter>
    <Name>Name</Name>
    <DataType>S</DataType>
    <DefaultValue>State</DefaultValue>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>40</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_state</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Name.</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>83</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_state</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>40</XCoordinate>
    <YCoordinate>65</YCoordinate>
    <FunctionName argument="2">Run_state</FunctionName>
  </Port>
</Component>
