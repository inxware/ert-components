<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DevmanPlayer</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>AV
    <Menu>Media Management
      <Menu>Devman Content Manager</Menu>
    </Menu>
  </Menu>
  <LongDescription>This component provides contact with a devman server to enable the exchange of playlists and player status information. The component detects new playlists made available on the devman server, downloads them and notifies the application that it is available and where it is stored. The function block is not playlist format aware and does not parse the playlists to identify tracks that need to be downloaded. This Function block is neither responsible for downloading content references in the playlist. This is done by the playlist manager in conjunction with a downloader function block. </LongDescription>
  <Parameter>
    <Name>Devman Server URL</Name>
    <DataType>S</DataType>
    <Description>devman server url</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Playlist Path</Name>
    <DataType>S</DataType>
    <Description>playlist path</Description>
    <ListPlacement>2</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Devman Ping Period</Name>
    <DataType>I</DataType>
    <DefaultValue>60</DefaultValue>
    <MinValue>1</MinValue>
    <MaxValue>3600</MaxValue>
    <Description>devman ping period in seconds</Description>
    <ListPlacement>3</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Use Devman Core URL</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Use the URL specified by devman.url</Description>
    <ListPlacement>4</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Start on Init</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Start checking on init</Description>
    <ListPlacement>5</ListPlacement>
  </Parameter>
  <Parameter>
    <Name>Load Stored playlist at start.</Name>
    <DataType>B</DataType>
    <DefaultValue>1</DefaultValue>
    <Description>Loads the last downloaded Media URL path etc.</Description>
    <ListPlacement>6</ListPlacement>
  </Parameter>
  <Port>
    <Description>Activate</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">startchecking</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="2">startchecking</FunctionName>
  </Port>
  <Port>
    <Description>De-activate</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">stopchecking</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="1">stopchecking</FunctionName>
  </Port>
  <Port>
    <Description>Set url</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">changeurl</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>URL</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="1">changeurl</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="3">changeurl</FunctionName>
    <FunctionName argument="22">out</FunctionName>
    <FunctionName argument="3">startchecking</FunctionName>
  </Port>
  <Port>
    <Description>Config</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">reconfigure</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>plylst path</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="1">reconfigure</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>update rate</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="2">reconfigure</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="3">reconfigure</FunctionName>
  </Port>
  <Port>
    <Description>track info</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">trackchanged</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>plyr mode</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>195</YCoordinate>
    <FunctionName argument="1">trackchanged</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>curr. media</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>210</YCoordinate>
    <FunctionName argument="2">trackchanged</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>start time</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>225</YCoordinate>
    <FunctionName argument="3">trackchanged</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>media duration</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>240</YCoordinate>
    <FunctionName argument="4">trackchanged</FunctionName>
  </Port>
  <Port>
    <Description>internal</Description>
    <PortType>InternalPort</PortType>
    <FunctionName argument="0">out</FunctionName>
  </Port>
  <Port>
    <Description>new playlist</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="14">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>plylst URL</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="1">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>media/ URL</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="2">out</FunctionName>
  </Port>
  <Port>
    <Description>rem. list</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="16">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>files</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>160</YCoordinate>
    <FunctionName argument="3">out</FunctionName>
  </Port>
  <Port>
    <Description>rem. unused</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>170</YCoordinate>
    <FunctionName argument="15">out</FunctionName>
  </Port>
  <Port>
    <Description>rem. all</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <FunctionName argument="17">out</FunctionName>
  </Port>
  <Port>
    <Description>Override</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>200</YCoordinate>
    <FunctionName argument="18">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>track url</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>215</YCoordinate>
    <FunctionName argument="4">out</FunctionName>
  </Port>
  <Port>
    <Description>media parms</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>230</YCoordinate>
    <FunctionName argument="19">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>playerJSON</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>245</YCoordinate>
    <FunctionName argument="5">out</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>channels</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>260</YCoordinate>
    <FunctionName argument="6">out</FunctionName>
  </Port>
  <Port>
    <Description>set mute</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>275</YCoordinate>
    <FunctionName argument="20">out</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>on/off</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>290</YCoordinate>
    <FunctionName argument="7">out</FunctionName>
  </Port>
  <Port>
    <Description>status</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>300</YCoordinate>
    <FunctionName argument="21">out</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>state</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>310</YCoordinate>
    <FunctionName argument="8">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>media path</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>125</YCoordinate>
    <FunctionName argument="9">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>playlist id</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>180</YCoordinate>
    <FunctionName argument="5">trackchanged</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>player state</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>255</YCoordinate>
    <FunctionName argument="6">trackchanged</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>X-playlist</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>105</YCoordinate>
    <FunctionName argument="10">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Raw XML</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>320</YCoordinate>
    <FunctionName argument="11">out</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>URL</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="2">changeurl</FunctionName>
    <FunctionName argument="12">out</FunctionName>
    <FunctionName argument="1">startchecking</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>PlayerJSON</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>270</YCoordinate>
    <FunctionName argument="7">trackchanged</FunctionName>
  </Port>
  <Port>
    <Description>Rst media src</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">reset</FunctionName>
  </Port>
  <Port>
    <Description>Loaded Stored</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>135</YCoordinate>
    <FunctionName argument="23">out</FunctionName>
  </Port>
  <Port>
    <Description>getNext</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>332</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">passthru_next</FunctionName>
  </Port>
  <Port>
    <Description>sendNext</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>342</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="0">passthru_send</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>sendJSON</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>352</YCoordinate>
    <FunctionName argument="1">passthru_send</FunctionName>
  </Port>
  <Port>
    <Description>passThru</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>332</YCoordinate>
    <FunctionName argument="24">out</FunctionName>
    <FunctionName argument="2">passthru_next</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>passJSON</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>342</YCoordinate>
    <FunctionName argument="13">out</FunctionName>
    <FunctionName argument="1">passthru_next</FunctionName>
  </Port>
</Component>
