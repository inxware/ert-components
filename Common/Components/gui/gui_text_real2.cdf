<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>gui_text_real2</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>User Interface
    <Menu>Graphics
      <Menu>Widgets
        <Menu>float display</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This widget displays an floating point number to the GUI output (to save you converting to a string!).</LongDescription>
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
    <Name>Maintain Aspect Ratio</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>maintains aspect ratio of widget when width is changed and ignores changes in height if TRUE, allows width and height to be altered independently if FALSE.</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Register Click Disregarding Z-Order</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>allows a widget to register a mouse click event when it is hidden behind another widget if TRUE, the widget only registers mouse clicks if the widget is visible at the coords that the mouse click occurs at if FALSE.</Description>
    <ListPlacement>4</ListPlacement>
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
    <YCoordinate>90</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.x</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="1">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.y</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="2">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.wid</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>125</YCoordinate>
    <FunctionName argument="3">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.ht</Description>
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
    <DataType>F</DataType>
    <Description>data</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <FunctionName argument="6">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs x</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="1">create</FunctionName>
    <FunctionName argument="7">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs y</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="2">create</FunctionName>
    <FunctionName argument="8">update</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>wid</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>125</YCoordinate>
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
    <Description>--</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="5">create</FunctionName>
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
    <YCoordinate>90</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="11">update</FunctionName>
  </Port>
  <Port>
    <Description>click</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="6">create</FunctionName>
  </Port>
  <Port>
    <Description>mouse down</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="7">create</FunctionName>
  </Port>
</Component>
