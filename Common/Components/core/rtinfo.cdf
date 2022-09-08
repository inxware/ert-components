<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>RuntimeInfo</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>System
    <Menu>Local
      <Menu>Runtime Info</Menu>
    </Menu>
  </Menu>
  <LongDescription>Device ID: MAC address, Network Address: IP addr, SODL Type: SODL format version, SODL name: Name of application,    </LongDescription>
  <Port>
    <Description>GetStatic</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">getstat</FunctionName>
  </Port>
  <Port>
    <Description>---</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="7">getstat</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Device ID</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">getstat</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>RAM (KB) </Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">getstat</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Storage (KB)</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="3">getstat</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>EHS version</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="4">getstat</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>ModuleList</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="5">getstat</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Install Dir</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="6">getstat</FunctionName>
  </Port>
  <Port>
    <Description>GetDynamic</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">getdyn</FunctionName>
  </Port>
  <Port>
    <Description>---</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="6">getdyn</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>IP Addre</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="1">getdyn</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>RAM Avail (KB)</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="2">getdyn</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Stor Avail (KB)</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="3">getdyn</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>CPU</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>125</YCoordinate>
    <FunctionName argument="4">getdyn</FunctionName>
  </Port>
  <Port>
    <Description>Req. Pair ID</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">reqpairid</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Pair ID</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>145</YCoordinate>
    <FunctionName argument="5">getdyn</FunctionName>
  </Port>
  <Port>
    <Description>Pair ID</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="7">getdyn</FunctionName>
  </Port>
</Component>
