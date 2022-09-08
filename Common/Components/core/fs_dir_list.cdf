<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FileSystemDirList1</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>File System
    <Menu>Directories
      <Menu>List Entries</Menu>
    </Menu>
  </Menu>
  <LongDescription>Allows a directory path to be set and outputs files contained in the directory on each get. Separate events are asserted for each type of file or directory.</LongDescription>
  <Parameter>
    <Name>Directory</Name>
    <DataType>S</DataType>
    <Description>Media Directory</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use Application Dir</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Find Files in Application Directory rather than the user data directory</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <Description>set dir</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">set</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>dir</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">set</FunctionName>
  </Port>
  <Port>
    <Description>Ready</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">set</FunctionName>
  </Port>
  <Port>
    <Description>Error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>27</YCoordinate>
    <FunctionName argument="5">get</FunctionName>
  </Port>
  <Port>
    <Description>get entry</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>37</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">get</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>name</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>67</YCoordinate>
    <FunctionName argument="1">get</FunctionName>
  </Port>
  <Port>
    <Description>file</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>37</YCoordinate>
    <FunctionName argument="2">get</FunctionName>
  </Port>
  <Port>
    <Description>dir</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>47</YCoordinate>
    <FunctionName argument="3">get</FunctionName>
  </Port>
  <Port>
    <Description>no more</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>57</YCoordinate>
    <FunctionName argument="4">get</FunctionName>
  </Port>
  <Port>
    <Description>release</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>77</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">rel</FunctionName>
  </Port>
  <Port>
    <Description>released</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>77</YCoordinate>
    <FunctionName argument="1">rel</FunctionName>
  </Port>
</Component>
