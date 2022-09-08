<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FileSystemDirCreateRemove</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>File System
    <Menu>Directories
      <Menu>Create Remove</Menu>
    </Menu>
  </Menu>
  <LongDescription>Allows a directory to be created or removed. Parametes available to allow complete removal of internal data too.</LongDescription>
  <Parameter>
    <Name>Directory</Name>
    <DataType>S</DataType>
    <Description>Media Directory</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>EmptyOnly</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Remove contents only</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <Description>create</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">create</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">create</FunctionName>
    <FunctionName argument="1">remove</FunctionName>
  </Port>
  <Port>
    <Description>created</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">create</FunctionName>
  </Port>
  <Port>
    <Description>error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="3">create</FunctionName>
  </Port>
  <Port>
    <Description>remove</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>37</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">remove</FunctionName>
  </Port>
  <Port>
    <Description>removed</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>37</YCoordinate>
    <FunctionName argument="2">remove</FunctionName>
  </Port>
  <Port>
    <Description>error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>47</YCoordinate>
    <FunctionName argument="3">remove</FunctionName>
  </Port>
</Component>
