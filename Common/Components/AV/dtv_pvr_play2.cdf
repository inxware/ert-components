<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DtvPvrPlay2</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>AV
    <Menu>Players
      <Menu>URLPlayer</Menu>
    </Menu>
  </Menu>
  <LongDescription>Plays local files &amp; streams remote URLs, presents to the video viewport. 
 Ports:-
Setinput: loads the input URL and configures the decoder.
setpos: Plyback position in % 
settime set playback position on seconds 
get: get the current input, position, time track length, and playback speed.</LongDescription>
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
  <Port>
    <Description>set input</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">setinput</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>path/URL</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="1">setinput</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">setinput</FunctionName>
  </Port>
  <Port>
    <Description>set pos</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">setpos</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>(%)</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="1">setpos</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">setpos</FunctionName>
  </Port>
  <Port>
    <Description>set time</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">settime</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>(s)</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="1">settime</FunctionName>
  </Port>
  <Port>
    <Description>------------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="2">settime</FunctionName>
  </Port>
  <Port>
    <Description>getinfo</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">get</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>meta</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">get</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>---info</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="2">get</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>---info </Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>85</YCoordinate>
    <FunctionName argument="3">get</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>length info</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>175</YCoordinate>
    <FunctionName argument="4">get</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>speed info</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>185</YCoordinate>
    <FunctionName argument="5">get</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>165</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="6">get</FunctionName>
  </Port>
  <Port>
    <Description>play</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">play</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>35</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">play</FunctionName>
  </Port>
  <Port>
    <Description>pause</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">pause</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">pause</FunctionName>
  </Port>
  <Port>
    <Description>reverse</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">reverse</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">reverse</FunctionName>
  </Port>
  <Port>
    <Description>faster</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">faster</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">faster</FunctionName>
  </Port>
  <Port>
    <Description>slower</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>130</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">slower</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>130</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">slower</FunctionName>
  </Port>
  <Port>
    <Description>stop</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">stop</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="1">stop</FunctionName>
  </Port>
  <Port>
    <Description>ended</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <AtomicFlag>0</AtomicFlag>
    <FunctionName argument="2">play</FunctionName>
    <FunctionName argument="3">setinput</FunctionName>
  </Port>
  <Port>
    <Description>setVol</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>280</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">vol</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>(%)</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>290</YCoordinate>
    <FunctionName argument="1">vol</FunctionName>
  </Port>
  <Port>
    <Description>setWindow</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>200</YCoordinate>
    <FunctionName argument="0">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.x</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>210</YCoordinate>
    <FunctionName argument="1">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>off.y</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>220</YCoordinate>
    <FunctionName argument="2">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>w</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>230</YCoordinate>
    <FunctionName argument="3">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>h</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>240</YCoordinate>
    <FunctionName argument="4">setWindow</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>290</YCoordinate>
    <FunctionName argument="9">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs x</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>210</YCoordinate>
    <FunctionName argument="5">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>abs y</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>220</YCoordinate>
    <FunctionName argument="6">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>w</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>230</YCoordinate>
    <FunctionName argument="7">setWindow</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>h</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>240</YCoordinate>
    <FunctionName argument="8">setWindow</FunctionName>
  </Port>
  <Port>
    <Description>to front</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>255</YCoordinate>
    <FunctionName argument="0">toFront</FunctionName>
  </Port>
  <Port>
    <Description>to back</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>265</YCoordinate>
    <FunctionName argument="0">toBack</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>255</YCoordinate>
    <FunctionName argument="1">toFront</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>265</YCoordinate>
    <FunctionName argument="1">toBack</FunctionName>
  </Port>
  <Port>
    <Description>----------</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>130</XCoordinate>
    <YCoordinate>280</YCoordinate>
    <FunctionName argument="2">vol</FunctionName>
  </Port>
</Component>
