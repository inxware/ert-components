<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FILE_ReadOnly_Bool</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>File System
    <Menu>File Access
      <Menu>File Input
        <Menu>bool</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>Read Only boolean file input reads '0'=false and '1'=true. If a field separator parameter is entered then then this will be used to identify subsequent data points. If a fixed width value is provided then this will override any separator information and an additional character will be discarded after the fixed field.   </LongDescription>
  <Parameter>
    <Name>File name</Name>
    <DataType>S</DataType>
    <DefaultValue>out.txt</DefaultValue>
    <Description>output file name</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Delimiter</Name>
    <DataType>S</DataType>
    <DefaultValue>()</DefaultValue>
    <Description>Boolean Field Delimiter Character</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use Application Dir</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Find Files in Application Directory rather than the user data directory</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Port>
    <Description>open</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_OpenFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_OpenFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_OpenFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>close</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_CloseFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="2">Run_CloseFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>read</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_ReadFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>data</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="1">Run_ReadFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>err code</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="1">Run_CloseFILE_ReadOnly_Bool</FunctionName>
    <FunctionName argument="2">Run_OpenFILE_ReadOnly_Bool</FunctionName>
    <FunctionName argument="2">Run_ReadFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">Run_ReadFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="4">Run_OpenFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_CloseFILE_ReadOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="4">Run_ReadFILE_ReadOnly_Bool</FunctionName>
  </Port>
</Component>
