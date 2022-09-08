<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>FILE_WriteOnly_Int</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>File System
    <Menu>File Access
      <Menu>File Output
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>Writes string data to a file. If a field separator parameter is specified (within brackets, 
 for a new line) then then this will be used inserted after data points. If a Fixed width parameter is specified then this specifies the length of the ascii string appended with spaces. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.</LongDescription>
  <Parameter>
    <Name>File name</Name>
    <DataType>S</DataType>
    <DefaultValue>out.txt</DefaultValue>
    <Description>output file name</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Field Width</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>12</MaxValue>
    <Description>Integer Field Width</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Delimiter</Name>
    <DataType>S</DataType>
    <DefaultValue>( )</DefaultValue>
    <Description>Integer Field Delimiter Character</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Append</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Append rather than overwrite.</Description>
    <ListPlacement>4</ListPlacement>
  </Parameter>
  <Port>
    <Description>open</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_OpenFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Run_OpenFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_OpenFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>close</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_CloseFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="2">Run_CloseFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>write</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Run_WriteFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>data</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="1">Run_WriteFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>err code</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="1">Run_CloseFILE_WriteOnly_Int</FunctionName>
    <FunctionName argument="2">Run_OpenFILE_WriteOnly_Int</FunctionName>
    <FunctionName argument="2">Run_WriteFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="3">Run_WriteFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="4">Run_OpenFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_CloseFILE_WriteOnly_Int</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="4">Run_WriteFILE_WriteOnly_Int</FunctionName>
  </Port>
</Component>
