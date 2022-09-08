<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>GUI_OutputTextBoxInt</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Deprecated
    <Menu>GUI_Output
      <Menu>int</Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an integer GUI output.</LongDescription>
  <Parameter>
    <Name>Widget Tag</Name>
    <DataType>WT</DataType>
    <DefaultValue>widget</DefaultValue>
    <Description>widget tag</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Widget-Group</Name>
    <DataType>ST</DataType>
    <DefaultValue>default</DefaultValue>
    <Description>widget group</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Port>
    <Description>create</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">CreateGUI_WriteTextBoxInt</FunctionName>
  </Port>
  <Port>
    <Description>destroy</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">DestroyGUI_WriteTextBoxInt</FunctionName>
  </Port>
  <Port>
    <Description>write</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">RunGUI_WriteTextBoxInt</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="1">RunGUI_WriteTextBoxInt</FunctionName>
  </Port>
  <Port>
    <Description>updated</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">RunGUI_WriteTextBoxInt</FunctionName>
  </Port>
</Component>
