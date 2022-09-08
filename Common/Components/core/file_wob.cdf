<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FILE_WriteOnly_Bool</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>File System
    <Menu>File Access
      <Menu>File Output
        <Menu>bool</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>Writes boolean data to a file. If a field separator parameter is specified then then this will be used inserted between data points. Fixed width is not available for this data type. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.</LongDescription>
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
    <Name>Append</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Append rather than overwrite.</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Port>
    <Description>open</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_OpenFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_OpenFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_OpenFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>close</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_CloseFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="2">Run_CloseFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>write</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_WriteFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>data</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="1">Run_WriteFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>err code</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="1">Run_CloseFILE_WriteOnly_Bool</FunctionName>
    <FunctionName argument="2">Run_OpenFILE_WriteOnly_Bool</FunctionName>
    <FunctionName argument="2">Run_WriteFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">Run_WriteFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="4">Run_OpenFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_CloseFILE_WriteOnly_Bool</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="4">Run_WriteFILE_WriteOnly_Bool</FunctionName>
  </Port>
</Component>
