<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>AppInfo</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>System
    <Menu>Local
      <Menu>Current App Info</Menu>
    </Menu>
  </Menu>
  <LongDescription>Returns the app automatic version number, data stamp and given name,    </LongDescription>
  <Parameter>
    <Name>Default App Name</Name>
    <DataType>S</DataType>
    <Description>Name of app to identify. Blank gives currently loaded application info. Use "dvm://devman-default/" for the default devman app server or full url dvm:// to get version info.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>get info</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">getinfo</FunctionName>
  </Port>
  <Port>
    <Description>---</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="9">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>version</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>build no </Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="3">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>date</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="4">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>min EHS</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="5">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>dependencies</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="6">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>processing groups</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="7">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>name</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>app name</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="8">getinfo</FunctionName>
  </Port>
</Component>
