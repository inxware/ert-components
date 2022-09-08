<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>gui_leds</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>User Interface
    <Menu>Hardware 
      <Menu>Display LEDs</Menu>
    </Menu>
  </Menu>
  <LongDescription>Controls the illumination of LEDs for displays to be controlled. The function block is updated by enumerated boolean variables (and two special LEds for cluster and backlight- to be deprecated).</LongDescription>
  <Port>
    <Description>update</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>cluster</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>F1</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>F2</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="3">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>F3</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="4">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>F4</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="5">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>F5</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="6">update</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>backlight</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="7">update</FunctionName>
  </Port>
</Component>
