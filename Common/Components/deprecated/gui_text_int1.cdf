<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>gui_text_int1</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Deprecated
    <Menu>GUI text1
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
    <FunctionName argument="0">create</FunctionName>
  </Port>
  <Port>
    <Description>destroy</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">destroy</FunctionName>
  </Port>
  <Port>
    <Description>show</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">show</FunctionName>
  </Port>
  <Port>
    <Description>hide</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">hide</FunctionName>
  </Port>
  <Port>
    <Description>update</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>x</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="1">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>y</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="2">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>wid</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="3">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ht</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="4">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>alpha</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="5">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <FunctionName argument="6">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>x</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="1">create</FunctionName>
    <FunctionName argument="7">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>y</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="2">create</FunctionName>
    <FunctionName argument="8">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>wid</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="3">create</FunctionName>
    <FunctionName argument="9">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ht</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="4">create</FunctionName>
    <FunctionName argument="10">update</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="5">create</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="1">destroy</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="1">show</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="1">hide</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="11">update</FunctionName>
  </Port>
</Component>
