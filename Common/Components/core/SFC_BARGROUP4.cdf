<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>SFCBarGroup4</Class>
  <Menu>Software Structure
    <Menu>State Machine
      <Menu>Boolean Driven
        <Menu>4 Bar</Menu>
      </Menu>
    </Menu>
  </Menu>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>10</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>55</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL1</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>150</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL2</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>245</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL3</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>340</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run_CL4</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>70</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <FunctionName argument="1">Run_CL1</FunctionName>
    <FunctionName argument="1">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>165</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <FunctionName argument="1">Run_CL2</FunctionName>
    <FunctionName argument="2">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>260</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <FunctionName argument="1">Run_CL3</FunctionName>
    <FunctionName argument="3">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>355</XCoordinate>
    <YCoordinate>-5</YCoordinate>
    <FunctionName argument="1">Run_CL4</FunctionName>
    <FunctionName argument="4">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>55</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="2">Run_CL1</FunctionName>
    <FunctionName argument="5">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>150</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="2">Run_CL2</FunctionName>
    <FunctionName argument="6">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>245</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="2">Run_CL3</FunctionName>
    <FunctionName argument="7">Run_OnEntry4</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>340</XCoordinate>
    <YCoordinate>38</YCoordinate>
    <FunctionName argument="2">Run_CL4</FunctionName>
    <FunctionName argument="8">Run_OnEntry4</FunctionName>
  </Port>
</Component>
