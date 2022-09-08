<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SFCBarGroup3Event</Class>
  <Menu>Software Structure
    <Menu>State Machine
      <Menu>Event Driven
        <Menu>3 Bar</Menu>
      </Menu>
    </Menu>
  </Menu>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>10</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_OnEntry3Event</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>55</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL1Event</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>150</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL2Event</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>245</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL3Event</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>55</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="1">Run_CL1Event</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>150</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="1">Run_CL2Event</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>245</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="1">Run_CL3Event</FunctionName>
  </Port>
</Component>
