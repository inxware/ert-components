<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>PlayManager</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>AV
    <Menu>Media Management
      <Menu>SMIL Playlist Parser</Menu>
    </Menu>
  </Menu>
  <LongDescription>Manages parsing SMIL-based playlists and controlling media players and graphics.</LongDescription>
  <Parameter>
    <Name>Start Behaviour</Name>
    <DataType>I</DataType>
    <DefaultValue>0</DefaultValue>
    <MinValue>0</MinValue>
    <MaxValue>2</MaxValue>
    <Description>0:CLEAR_ALL, 1:KEEP_EARLIERTHANNEW, 2:KEEP_ALL  </Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Wait for downloads before start</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>set to true if play events should only active after all media has been downloaded.  </Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Download Grace Period</Name>
    <DataType>I</DataType>
    <DefaultValue>60</DefaultValue>
    <MinValue>-1</MinValue>
    <MaxValue>36000</MaxValue>
    <Description>number of seconds waited after a new play list before Play events can occur    </Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Port>
    <DataType>S</DataType>
    <Description>Playlist path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">Next_Playlist</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Media File</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Tick</FunctionName>
  </Port>
  <Port>
    <Description>New Playlist</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Next_Playlist</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Volume</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="11">Tick</FunctionName>
  </Port>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">Tick</FunctionName>
  </Port>
  <Port>
    <Description>Last Ended</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Played</FunctionName>
  </Port>
  <Port>
    <Description>--------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="6">Next_Playlist</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Playlist path</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="2">Next_Playlist</FunctionName>
  </Port>
  <Port>
    <Description>Play Now</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <FunctionName argument="14">Tick</FunctionName>
  </Port>
  <Port>
    <Description>Next URL</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>145</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Next_URL</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>145</YCoordinate>
    <FunctionName argument="4">Next_URL</FunctionName>
  </Port>
  <Port>
    <Description>Get</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>155</YCoordinate>
    <FunctionName argument="7">Next_Playlist</FunctionName>
    <FunctionName argument="5">Next_URL</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>URL</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <FunctionName argument="3">Next_Playlist</FunctionName>
    <FunctionName argument="1">Next_URL</FunctionName>
  </Port>
  <Port>
    <Description>Got</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Got</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <FunctionName argument="6">Got</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>URL</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>205</YCoordinate>
    <FunctionName argument="1">Got</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>------------</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>205</YCoordinate>
    <FunctionName argument="4">Got</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Local File</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>215</YCoordinate>
    <FunctionName argument="2">Got</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>------------</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>215</YCoordinate>
    <FunctionName argument="5">Got</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Local File</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>195</YCoordinate>
    <FunctionName argument="4">Next_Playlist</FunctionName>
    <FunctionName argument="2">Next_URL</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>DL URL</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>250</YCoordinate>
    <FunctionName argument="1">Set_Paths</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>DL local</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>260</YCoordinate>
    <FunctionName argument="2">Set_Paths</FunctionName>
  </Port>
  <Port>
    <Description>Set Paths</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>240</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Set_Paths</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>240</YCoordinate>
    <FunctionName argument="6">Set_Paths</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>-------------</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>250</YCoordinate>
    <FunctionName argument="4">Set_Paths</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>--------------</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>260</YCoordinate>
    <FunctionName argument="5">Set_Paths</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>start mode</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>270</YCoordinate>
    <FunctionName argument="3">Set_Paths</FunctionName>
  </Port>
  <Port>
    <Description>play remote</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="13">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>start time</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>290</YCoordinate>
    <FunctionName argument="6">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>class</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="2">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Text</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>125</YCoordinate>
    <FunctionName argument="3">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Type</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="4">Tick</FunctionName>
  </Port>
  <Port>
    <Description>Started Play</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Started</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ID</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="1">Started</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ID</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="1">Played</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ID</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>195</YCoordinate>
    <FunctionName argument="3">Got</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ID</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>65</YCoordinate>
    <FunctionName argument="5">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>ID</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="5">Next_Playlist</FunctionName>
    <FunctionName argument="3">Next_URL</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Stop time</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>300</YCoordinate>
    <FunctionName argument="7">Tick</FunctionName>
  </Port>
  <Port>
    <Description>Earliest</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>280</YCoordinate>
    <FunctionName argument="15">Tick</FunctionName>
  </Port>
  <Port>
    <Description>Tried all</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>225</YCoordinate>
    <FunctionName argument="8">Next_Playlist</FunctionName>
    <FunctionName argument="6">Next_URL</FunctionName>
  </Port>
  <Port>
    <Description>Play Stop</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="16">Tick</FunctionName>
  </Port>
  <Port>
    <Description>New Layout</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>315</YCoordinate>
    <FunctionName argument="17">Tick</FunctionName>
  </Port>
  <Port>
    <DataType></DataType>
    <Description>Region JSON</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>325</YCoordinate>
    <FunctionName argument="8">Tick</FunctionName>
  </Port>
  <Port>
    <DataType></DataType>
    <Description>AR JSON</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>335</YCoordinate>
    <FunctionName argument="9">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Region</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="10">Tick</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Fade</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="12">Tick</FunctionName>
  </Port>
</Component>
