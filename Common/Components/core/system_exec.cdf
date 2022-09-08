<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>Exec</Class>
  <Menu>System
    <Menu>Host OS
      <Menu>Run Executable</Menu>
    </Menu>
  </Menu>
  <LongDescription>This function block runs the executable specified by the parameter with the arguments specified by the input. It outputs the output of the executable</LongDescription>
  <Parameter>
    <Name>Executable</Name>
    <DataType>S</DataType>
    <Description>The executable to run</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <Description>args</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>out</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">run</FunctionName>
  </Port>
  <Port>
    <Description>OK</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
  <Port>
    <Description>Error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="4">run</FunctionName>
  </Port>
</Component>
