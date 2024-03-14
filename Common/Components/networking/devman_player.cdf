<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>This component provides contact with a devman server to enable the exchange of playlists and player status information. The component detects new playlists made available on the devman server, downloads them and notifies the application that it is available and where it is stored. The function block is not playlist format aware and does not parse the playlists to identify tracks that need to be downloaded. This Function block is neither responsible for downloading content references in the playlist. This is done by the playlist manager in conjunction with a downloader function block. </LongDescription>
        <UserName/>
        <Menu>
            AV
            <Menu>
                Media Management
                <Menu>Devman Content Manager</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>DevmanPlayer</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xB0BC</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>e22c9a2d</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Devman Server URL</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>devman server url</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Playlist Path</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>playlist path</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Devman Ping Period</Name>
            <DataType>I</DataType>
            <DefaultValue>60</DefaultValue>
            <MinValue>1</MinValue>
            <MaxValue>3600</MaxValue>
            <Description>devman ping period in seconds</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use Devman Core URL</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Use the URL specified by devman.url</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Start on Init</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Start checking on init</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Load Stored playlist at start.</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Loads the last downloaded Media URL path etc.</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>startchecking</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>stopchecking</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>changeurl</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>out</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>reconfigure</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>trackchanged</name>
            <ID>
                <ERT1_ID>6</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>reset</name>
            <ID>
                <ERT1_ID>7</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>passthru_next</name>
            <ID>
                <ERT1_ID>8</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>passthru_send</name>
            <ID>
                <ERT1_ID>9</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>Activate</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>Activate</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>De-activate</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>De_activate</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Set url</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>Set_url</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>URL</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>URL</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="9">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Config</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>Config</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>plylst path</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>90</YCoordinate>
            <CName>plylst_path</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>update rate</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>update_rate</CName>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>track info</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>165</YCoordinate>
            <CName>track_info</CName>
            <Function argument="0">
                <Function_ERT1_ID>6</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>plyr mode</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>195</YCoordinate>
            <CName>plyr_mode</CName>
            <Function argument="1">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>curr. media</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>210</YCoordinate>
            <CName>curr__media</CName>
            <Function argument="2">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>start time</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>225</YCoordinate>
            <CName>start_time</CName>
            <Function argument="3">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>media duration</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>240</YCoordinate>
            <CName>media_duration</CName>
            <Function argument="4">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>internal</Description>
            <PortType>InternalPort</PortType>
            <XCoordinate>-1</XCoordinate>
            <YCoordinate>-1</YCoordinate>
            <CName>internal</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>new playlist</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <Wcet>0</Wcet>
            <CName>new_playlist</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>plylst URL</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>plylst_URL</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>media/ URL</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>media__URL</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>rem. list</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <Wcet>0</Wcet>
            <CName>rem__list</CName>
            <Function argument="3">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>files</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <CName>files</CName>
            <Function argument="3">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>rem. unused</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>170</YCoordinate>
            <Wcet>0</Wcet>
            <CName>rem__unused</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>rem. all</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>185</YCoordinate>
            <Wcet>0</Wcet>
            <CName>rem__all</CName>
            <Function argument="4">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Override</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>200</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Override</CName>
            <Function argument="5">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>track url</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>215</YCoordinate>
            <CName>track_url</CName>
            <Function argument="4">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>media parms</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>230</YCoordinate>
            <Wcet>0</Wcet>
            <CName>media_parms</CName>
            <Function argument="6">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>playerJSON</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>245</YCoordinate>
            <CName>playerJSON</CName>
            <Function argument="5">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>channels</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>260</YCoordinate>
            <CName>channels</CName>
            <Function argument="6">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set mute</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>275</YCoordinate>
            <Wcet>0</Wcet>
            <CName>set_mute</CName>
            <Function argument="7">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>on/off</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>290</YCoordinate>
            <CName>on_off</CName>
            <Function argument="7">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>status</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>300</YCoordinate>
            <Wcet>0</Wcet>
            <CName>status</CName>
            <Function argument="8">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>state</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>310</YCoordinate>
            <CName>state</CName>
            <Function argument="8">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>media path</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>125</YCoordinate>
            <CName>media_path</CName>
            <Function argument="9">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>playlist id</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>180</YCoordinate>
            <CName>playlist_id</CName>
            <Function argument="5">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>player state</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>255</YCoordinate>
            <CName>player_state</CName>
            <Function argument="6">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>X-playlist</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>X_playlist</CName>
            <Function argument="10">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Raw XML</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>320</YCoordinate>
            <CName>Raw_XML</CName>
            <Function argument="11">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>URL</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>URL</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="12">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>PlayerJSON</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>270</YCoordinate>
            <CName>PlayerJSON</CName>
            <Function argument="7">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Rst media src</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>Rst_media_src</CName>
            <Function argument="0">
                <Function_ERT1_ID>7</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>Loaded Stored</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Loaded_Stored</CName>
            <Function argument="10">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>getNext</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>332</YCoordinate>
            <CName>getNext</CName>
            <Function argument="0">
                <Function_ERT1_ID>8</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>sendNext</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>342</YCoordinate>
            <CName>sendNext</CName>
            <Function argument="0">
                <Function_ERT1_ID>9</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>sendJSON</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>352</YCoordinate>
            <CName>sendJSON</CName>
            <Function argument="1">
                <Function_ERT1_ID>9</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>passThru</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>332</YCoordinate>
            <Wcet>0</Wcet>
            <CName>passThru</CName>
            <Function argument="11">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>8</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>passJSON</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>342</YCoordinate>
            <CName>passJSON</CName>
            <Function argument="13">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>8</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
