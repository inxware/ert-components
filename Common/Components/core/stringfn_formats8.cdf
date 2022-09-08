<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>string_format8</Class>
  <Menu>Data Utilities
    <Menu>String Functions
      <Menu>format
        <Menu>8 input</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is an 8-input string formatter. Use the format property to show how the string should appear.</LongDescription>
  <Parameter>
    <Name>Format</Name>
    <DataType>S</DataType>
    <DefaultValue>%s%s%s%s%s%s%s%s</DefaultValue>
    <Description>C-style format string - supports %s only</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <Description>s1</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s2</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="2">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s3</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="3">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s4</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="4">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s5</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="5">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s6</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="6">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s7</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="7">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>s8</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <FunctionName argument="8">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>fmt</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="9">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="10">run</FunctionName>
  </Port>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="11">run</FunctionName>
  </Port>
</Component>
