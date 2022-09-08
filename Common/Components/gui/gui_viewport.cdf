<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>gui_viewport</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>User Interface
    <Menu>View Port
      <Menu>Window</Menu>
    </Menu>
  </Menu>
  <LongDescription>GUI window position, size, colour and transparency. Edit parameters in IGB.</LongDescription>
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
  <Parameter>
    <Name>Relatative % (false for abs pixels)</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Screen Coordinates in % full screen size (or flase for absolute pixels)</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>top</Name>
    <DataType>I</DataType>
    <DefaultValue>-1</DefaultValue>
    <MinValue>-1</MinValue>
    <MaxValue>100000000</MaxValue>
    <Description>coordinate in pixels or screen perentange</Description>
    <ListPlacement>4</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>height</Name>
    <DataType>I</DataType>
    <DefaultValue>-1</DefaultValue>
    <MinValue>-1</MinValue>
    <MaxValue>100000000</MaxValue>
    <Description>coordinate in pixels or screen perentange</Description>
    <ListPlacement>5</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>left</Name>
    <DataType>I</DataType>
    <DefaultValue>-1</DefaultValue>
    <MinValue>-1</MinValue>
    <MaxValue>100000000</MaxValue>
    <Description>coordinate in pixels or screen perentange</Description>
    <ListPlacement>6</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>width</Name>
    <DataType>I</DataType>
    <DefaultValue>-1</DefaultValue>
    <MinValue>-1</MinValue>
    <MaxValue>100000000</MaxValue>
    <Description>coordinate in pixels or screen perentange</Description>
    <ListPlacement>7</ListPlacement>
  </Parameter>
  <Port>
    <Description>create</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">create</FunctionName>
  </Port>
  <Port>
    <Description>destroy</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">destroy</FunctionName>
  </Port>
  <Port>
    <Description>show</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">show</FunctionName>
  </Port>
  <Port>
    <Description>hide</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">hide</FunctionName>
  </Port>
  <Port>
    <Description>update</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.x</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>155</YCoordinate>
    <FunctionName argument="1">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.y</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <FunctionName argument="2">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.wid</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="3">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.ht</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <FunctionName argument="4">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>alpha</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>200</YCoordinate>
    <FunctionName argument="5">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs x</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>155</YCoordinate>
    <FunctionName argument="1">create</FunctionName>
    <FunctionName argument="6">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs y</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <FunctionName argument="2">create</FunctionName>
    <FunctionName argument="7">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>wid</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="3">create</FunctionName>
    <FunctionName argument="8">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ht</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <FunctionName argument="4">create</FunctionName>
    <FunctionName argument="9">update</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="9">create</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="1">destroy</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="1">show</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="1">hide</FunctionName>
  </Port>
  <Port>
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="10">update</FunctionName>
  </Port>
  <Port>
    <Description>mouse down</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="10">create</FunctionName>
  </Port>
  <Port>
    <Description>mouse up</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="11">create</FunctionName>
  </Port>
  <Port>
    <Description>mouse drag</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="12">create</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs.x</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="5">create</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs.y</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="6">create</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.x</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="7">create</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.y</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>125</YCoordinate>
    <FunctionName argument="8">create</FunctionName>
  </Port>
</Component>
