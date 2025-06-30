<?xml version="1.0" encoding="UTF-8"?><Component>
	<Module>usercomponents</Module>
	<CDFInfo>
		<Version>3</Version>
		<CreationDate>2023-04-18T13:32:37Z</CreationDate>
		<UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
	</CDFInfo>
	<Description>
        <ShortDescription/>
		<LongDescription>Manages parsing SMIL-based playlists and controlling media players and graphics.</LongDescription>
        <UserName/>
		<Menu>
			AV
			<Menu>
				Media Management
				<Menu>SMIL Playlist Parser</Menu>
			</Menu>
		</Menu>
	</Description>
	<Block>
		<Type>IO</Type>
	</Block>
	<FBID>
		<ERT1_ID>0</ERT1_ID>
		<Class>PlayManager</Class>
	</FBID>
	<Hashes>
		<NameHash_CRC16>0xB6D9</NameHash_CRC16>
		<FbApiDescriptorHash_CRC32>4237a878</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
	</Hashes>
	<Parameters>
		<Parameter>
			<Name>Start Behaviour</Name>
			<DataType>I</DataType>
			<DefaultValue>0</DefaultValue>
			<MinValue>0</MinValue>
			<MaxValue>2</MaxValue>
			<Description>0:CLEAR_ALL, 1:KEEP_EARLIERTHANNEW, 2:KEEP_ALL</Description>
			<ListPlacement>1</ListPlacement>
			<ArgPlacement>1</ArgPlacement>
		</Parameter>
		<Parameter>
			<Name>Start After Downloaded</Name>
			<DataType>B</DataType>
			<DefaultValue>1</DefaultValue>
			<Description>set to true if play events should only active after all media has been downloaded.</Description>
			<ListPlacement>2</ListPlacement>
			<ArgPlacement>2</ArgPlacement>
		</Parameter>
		<Parameter>
			<Name>Download Grace Period</Name>
			<DataType>I</DataType>
			<DefaultValue>60</DefaultValue>
			<MinValue>-1</MinValue>
			<MaxValue>36000</MaxValue>
			<Description>number of seconds waited after a new play list before Play events can occur</Description>
			<ListPlacement>3</ListPlacement>
			<ArgPlacement>3</ArgPlacement>
		</Parameter>
	</Parameters>
	<Functions>
		<Function>
			<name>Next_Playlist</name>
			<ID>
				<ERT1_ID>1</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Tick</name>
			<ID>
				<ERT1_ID>2</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Played</name>
			<ID>
				<ERT1_ID>3</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Next_URL</name>
			<ID>
				<ERT1_ID>4</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Got</name>
			<ID>
				<ERT1_ID>5</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Set_Paths</name>
			<ID>
				<ERT1_ID>6</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Started</name>
			<ID>
				<ERT1_ID>7</ERT1_ID>
			</ID>
		</Function>
	</Functions>
	<Ports>
		<Port>
			<DataType>S</DataType>
			<Description>playlist path</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>20</YCoordinate>
			<CName>Playlist_path</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>media file</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>75</YCoordinate>
			<CName>Media_File</CName>
			<Function argument="1">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>new playlist</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<CName>New_Playlist</CName>
			<Function argument="0">
				<Function_ERT1_ID>1</Function_ERT1_ID>
				<AtomicFlag>1</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>volume</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>95</YCoordinate>
			<CName>Volume</CName>
			<Function argument="11">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>internal</Description>
			<PortType>InternalPort</PortType>
			<XCoordinate>-1</XCoordinate>
			<YCoordinate>-1</YCoordinate>
			<CName>internal</CName>
			<Function argument="0">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>last ended</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>80</YCoordinate>
			<CName>Last_Ended</CName>
			<Function argument="0">
				<Function_ERT1_ID>3</Function_ERT1_ID>
				<AtomicFlag>1</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>--------------</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<Wcet>0</Wcet>
			<CName>______________</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>playlist path</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>20</YCoordinate>
			<CName>Playlist_path</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>play now</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>35</YCoordinate>
			<Wcet>0</Wcet>
			<CName>Play_Now</CName>
			<Function argument="2">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>next URL</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>145</YCoordinate>
			<CName>Next_URL</CName>
			<Function argument="0">
				<Function_ERT1_ID>4</Function_ERT1_ID>
				<AtomicFlag>1</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>------------</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>145</YCoordinate>
			<Wcet>0</Wcet>
			<CName>____________</CName>
			<Function argument="1">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>get</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>155</YCoordinate>
			<Wcet>0</Wcet>
			<CName>Get</CName>
			<Function argument="2">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
			<Function argument="2">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>URL</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>165</YCoordinate>
			<CName>URL</CName>
			<Function argument="2">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
			<Function argument="1">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>got</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>185</YCoordinate>
			<CName>Got</CName>
			<Function argument="0">
				<Function_ERT1_ID>5</Function_ERT1_ID>
				<AtomicFlag>1</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>------------</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>185</YCoordinate>
			<Wcet>0</Wcet>
			<CName>____________</CName>
			<Function argument="1">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>URL</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>205</YCoordinate>
			<CName>URL</CName>
			<Function argument="1">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>------------</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>205</YCoordinate>
			<CName>____________</CName>
			<Function argument="1">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>local file</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>215</YCoordinate>
			<CName>Local_File</CName>
			<Function argument="2">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>------------</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>215</YCoordinate>
			<CName>____________</CName>
			<Function argument="2">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>local file</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>195</YCoordinate>
			<CName>Local_File</CName>
			<Function argument="3">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
			<Function argument="2">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>DL URL</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>250</YCoordinate>
			<CName>DL_URL</CName>
			<Function argument="1">
				<Function_ERT1_ID>6</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>DL local</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>260</YCoordinate>
			<CName>DL_local</CName>
			<Function argument="2">
				<Function_ERT1_ID>6</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>set paths</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>240</YCoordinate>
			<CName>Set_Paths</CName>
			<Function argument="0">
				<Function_ERT1_ID>6</Function_ERT1_ID>
				<AtomicFlag>1</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>------------</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>240</YCoordinate>
			<Wcet>0</Wcet>
			<CName>____________</CName>
			<Function argument="1">
				<Function_ERT1_ID>6</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>-------------</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>250</YCoordinate>
			<CName>_____________</CName>
			<Function argument="1">
				<Function_ERT1_ID>6</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>--------------</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>260</YCoordinate>
			<CName>______________</CName>
			<Function argument="2">
				<Function_ERT1_ID>6</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>start mode</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>270</YCoordinate>
			<CName>start_mode</CName>
			<Function argument="3">
				<Function_ERT1_ID>6</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>play remote</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>45</YCoordinate>
			<Wcet>0</Wcet>
			<CName>play_remote</CName>
			<Function argument="1">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>start time</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>290</YCoordinate>
			<CName>start_time</CName>
			<Function argument="6">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>class</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>115</YCoordinate>
			<CName>class</CName>
			<Function argument="2">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>text</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>125</YCoordinate>
			<CName>Text</CName>
			<Function argument="3">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>type</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>85</YCoordinate>
			<CName>Type</CName>
			<Function argument="4">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>started play</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>45</YCoordinate>
			<CName>Started_Play</CName>
			<Function argument="0">
				<Function_ERT1_ID>7</Function_ERT1_ID>
				<AtomicFlag>1</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>ID</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>55</YCoordinate>
			<CName>ID</CName>
			<Function argument="1">
				<Function_ERT1_ID>7</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>ID</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>90</YCoordinate>
			<CName>ID</CName>
			<Function argument="1">
				<Function_ERT1_ID>3</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>ID</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>195</YCoordinate>
			<CName>ID</CName>
			<Function argument="3">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>ID</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>65</YCoordinate>
			<CName>ID</CName>
			<Function argument="5">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>ID</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>175</YCoordinate>
			<CName>ID</CName>
			<Function argument="4">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
			<Function argument="3">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>stop time</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>300</YCoordinate>
			<CName>Stop_time</CName>
			<Function argument="7">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>earliest</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>280</YCoordinate>
			<Wcet>0</Wcet>
			<CName>Earliest</CName>
			<Function argument="3">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>tried all</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>225</YCoordinate>
			<Wcet>0</Wcet>
			<CName>Tried_all</CName>
			<Function argument="3">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
			<Function argument="3">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>play stop</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>55</YCoordinate>
			<Wcet>0</Wcet>
			<CName>Play_Stop</CName>
			<Function argument="4">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>new layout</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>315</YCoordinate>
			<Wcet>0</Wcet>
			<CName>New_Layout</CName>
			<Function argument="5">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>region JSON</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>325</YCoordinate>
			<CName>Region_JSON</CName>
			<Function argument="8">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>AR JSON</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>335</YCoordinate>
			<CName>AR_JSON</CName>
			<Function argument="9">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>region</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>135</YCoordinate>
			<CName>Region</CName>
			<Function argument="10">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>fade</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>105</YCoordinate>
			<CName>Fade</CName>
			<Function argument="12">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>file count</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>130</XCoordinate>
			<YCoordinate>347</YCoordinate>
			<CName>File_Count</CName>
			<Function argument="5">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
			<Function argument="4">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
		</Port>
	</Ports>
</Component>
