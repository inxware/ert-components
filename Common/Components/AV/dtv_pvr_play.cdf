<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DtvPvrPlay1</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>Deprecated
    <Menu>DTV
      <Menu>Play</Menu>
    </Menu>
  </Menu>
  <LongDescription>Provides facilities to control PVR playback in terms of trick modes, play, pause and stop. </LongDescription>
  <Port>
    <Description>set</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">set</FunctionName>
  </Port>
  <Port>
    <Description>get</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">get</FunctionName>
  </Port>
  <Port>
    <Description>play</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">play</FunctionName>
  </Port>
  <Port>
    <Description>pause</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">pause</FunctionName>
  </Port>
  <Port>
    <Description>rev</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">reverse</FunctionName>
  </Port>
  <Port>
    <Description>faster</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">faster</FunctionName>
  </Port>
  <Port>
    <Description>slower</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">slower</FunctionName>
  </Port>
  <Port>
    <Description>stop</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">stop</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>name</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="1">set</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>pos</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="2">set</FunctionName>
  </Port>
  <Port>
    <Description>set</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="4">set</FunctionName>
  </Port>
  <Port>
    <Description>get</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="4">get</FunctionName>
  </Port>
  <Port>
    <Description>play</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">play</FunctionName>
  </Port>
  <Port>
    <Description>pause</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">pause</FunctionName>
  </Port>
  <Port>
    <Description>rev</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">reverse</FunctionName>
  </Port>
  <Port>
    <Description>faster</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">faster</FunctionName>
  </Port>
  <Port>
    <Description>slower</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">slower</FunctionName>
  </Port>
  <Port>
    <Description>stop</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">stop</FunctionName>
  </Port>
  <Port>
    <Description>end</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">end</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>pos</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="2">get</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>speed</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>125</YCoordinate>
    <FunctionName argument="3">get</FunctionName>
  </Port>
  <Port>
    <Description>err</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <FunctionName argument="3">faster</FunctionName>
    <FunctionName argument="5">get</FunctionName>
    <FunctionName argument="3">pause</FunctionName>
    <FunctionName argument="3">play</FunctionName>
    <FunctionName argument="3">reverse</FunctionName>
    <FunctionName argument="5">set</FunctionName>
    <FunctionName argument="3">slower</FunctionName>
    <FunctionName argument="3">stop</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>errNo</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>100</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="1">faster</FunctionName>
    <FunctionName argument="1">get</FunctionName>
    <FunctionName argument="1">pause</FunctionName>
    <FunctionName argument="1">play</FunctionName>
    <FunctionName argument="1">reverse</FunctionName>
    <FunctionName argument="6">set</FunctionName>
    <FunctionName argument="1">slower</FunctionName>
    <FunctionName argument="1">stop</FunctionName>
  </Port>
  <Port>
    <Description>End port</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">end</FunctionName>
  </Port>
</Component>
