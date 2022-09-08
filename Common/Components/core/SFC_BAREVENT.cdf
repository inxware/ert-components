<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SFCBarGroup1Event</Class>
  <Menu>Software Structure
    <Menu>State Machine
      <Menu>Event Driven
        <Menu>Single Bar</Menu>
      </Menu>
    </Menu>
  </Menu>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>10</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_OnEntry1Event</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>55</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL1Event</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>55</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="1">Run_CL1Event</FunctionName>
  </Port>
</Component>
