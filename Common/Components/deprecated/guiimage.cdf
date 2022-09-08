<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>GUI_Image</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Deprecated
    <Menu>GUI_Image</Menu>
  </Menu>
  <LongDescription>This is a GUI image.</LongDescription>
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
    <FunctionName argument="0">RunGUI_LoadImage</FunctionName>
  </Port>
  <Port>
    <Description>destroy</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">DestroyGUI_Image</FunctionName>
  </Port>
  <Port>
    <Description>show</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">RealizeGUI_Image</FunctionName>
  </Port>
  <Port>
    <Description>hide</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">UnrealizeGUI_Image</FunctionName>
  </Port>
  <Port>
    <Description>move</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">RunGUI_MoveImage</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>x</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="1">RunGUI_MoveImage</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>y</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="2">RunGUI_MoveImage</FunctionName>
  </Port>
</Component>
