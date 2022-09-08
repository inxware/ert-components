<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>AppGet</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>System
    <Menu>Remote
      <Menu>Application Server</Menu>
    </Menu>
  </Menu>
  <LongDescription>Provides the means to download, install &amp; run applications from the application server.    </LongDescription>
  <Parameter>
    <Name>Server</Name>
    <DataType>S</DataType>
    <DefaultValue>devman.inx-systems.com</DefaultValue>
    <Description>Base URL of devman server hosting applications.</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Install and Run</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Run the application after it has been installed.</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Install as Default</Name>
    <DataType>B</DataType>
    <DefaultValue>0</DefaultValue>
    <Description>Set the application as the Default App when it is installed.</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Install Mode</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>2</MaxValue>
    <Description>Install Mode, 0 install as standard App in canonical directory, 1 install as Home App, 2 install as Temp App.</Description>
    <ListPlacement>4</ListPlacement>
  </Parameter>
  <Port>
    <Description>get list</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">listremote</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>keywords</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">listremote</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>server</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="2">getinfo</FunctionName>
    <FunctionName argument="2">getinstall</FunctionName>
    <FunctionName argument="2">listremote</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>list</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="3">listremote</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>errors </Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="8">getinfo</FunctionName>
    <FunctionName argument="6">getinstall</FunctionName>
    <FunctionName argument="4">listremote</FunctionName>
  </Port>
  <Port>
    <Description>------ </Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="5">listremote</FunctionName>
  </Port>
  <Port>
    <Description>error </Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="6">listremote</FunctionName>
  </Port>
  <Port>
    <Description>install</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">getinstall</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>app name</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="1">getinstall</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>run</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>65</YCoordinate>
    <FunctionName argument="3">getinstall</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>default</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="4">getinstall</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>mode</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="5">getinstall</FunctionName>
  </Port>
  <Port>
    <Description>-----</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="7">getinstall</FunctionName>
  </Port>
  <Port>
    <Description>error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="8">getinstall</FunctionName>
  </Port>
  <Port>
    <Description>app info</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>app name</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="1">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>info</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="3">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>name</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="4">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>commercial name</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>130</YCoordinate>
    <FunctionName argument="5">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>version</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <FunctionName argument="6">getinfo</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>description</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="7">getinfo</FunctionName>
  </Port>
  <Port>
    <Description>-----</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <FunctionName argument="9">getinfo</FunctionName>
  </Port>
  <Port>
    <Description>error</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>160</YCoordinate>
    <FunctionName argument="10">getinfo</FunctionName>
  </Port>
</Component>
