<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Plays local files &amp; streams remote URLs, presents to the video viewport. 
 Ports:-
Setinput: loads the input URL and configures the decoder.
setpos: Plyback position in % 
settime set playback position on seconds 
get: get the current input, position, time track length, and playback speed.</LongDescription>
        <UserName/>
        <Menu>
            AV
            <Menu>
                Players
                <Menu>URLPlayer</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>DtvPvrPlay2</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x837B</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>84b7fc1b</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Widget Tag</Name>
            <DataType>WT</DataType>
            <DefaultValue>widget</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>widget tag</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Widget-Group</Name>
            <DataType>ST</DataType>
            <DefaultValue>default</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>widget group</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>setinput</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>setpos</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>settime</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>get</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>play</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>pause</name>
            <ID>
                <ERT1_ID>6</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>reverse</name>
            <ID>
                <ERT1_ID>7</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>faster</name>
            <ID>
                <ERT1_ID>8</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>slower</name>
            <ID>
                <ERT1_ID>9</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>stop</name>
            <ID>
                <ERT1_ID>10</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>vol</name>
            <ID>
                <ERT1_ID>11</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>setWindow</name>
            <ID>
                <ERT1_ID>12</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>toFront</name>
            <ID>
                <ERT1_ID>13</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>toBack</name>
            <ID>
                <ERT1_ID>14</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>set input</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <CName>set_input</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>path/URL</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <CName>path_URL</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set pos</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>set_pos</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>(%)</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>___</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set time</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>set_time</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>(s)</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>_s_</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>------------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____________</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>getinfo</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>165</YCoordinate>
            <CName>getinfo</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>meta</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>meta</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>---info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>___info</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>---info </Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>___info_</CName>
            <Function argument="3">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>length info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <CName>length_info</CName>
            <Function argument="4">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>speed info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>185</YCoordinate>
            <CName>speed_info</CName>
            <Function argument="5">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>165</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>play</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>play</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>pause</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>pause</CName>
            <Function argument="0">
                <Function_ERT1_ID>6</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>reverse</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>reverse</CName>
            <Function argument="0">
                <Function_ERT1_ID>7</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>7</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>faster</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>faster</CName>
            <Function argument="0">
                <Function_ERT1_ID>8</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>8</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>slower</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <CName>slower</CName>
            <Function argument="0">
                <Function_ERT1_ID>9</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>9</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>stop</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>stop</CName>
            <Function argument="0">
                <Function_ERT1_ID>10</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>10</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>ended</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <Wcet>0</Wcet>
            <CName>ended</CName>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>setVol</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>280</YCoordinate>
            <CName>setVol</CName>
            <Function argument="0">
                <Function_ERT1_ID>11</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>(%)</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>290</YCoordinate>
            <CName>___</CName>
            <Function argument="1">
                <Function_ERT1_ID>11</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>setWindow</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>200</YCoordinate>
            <CName>setWindow</CName>
            <Function argument="0">
                <Function_ERT1_ID>12</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>off.x</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>210</YCoordinate>
            <CName>off_x</CName>
            <Function argument="1">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>off.y</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <CName>off_y</CName>
            <Function argument="2">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>w</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>230</YCoordinate>
            <CName>w</CName>
            <Function argument="3">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>h</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>240</YCoordinate>
            <CName>h</CName>
            <Function argument="4">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>290</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>abs x</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>210</YCoordinate>
            <CName>abs_x</CName>
            <Function argument="1">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>abs y</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <CName>abs_y</CName>
            <Function argument="2">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>w</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>230</YCoordinate>
            <CName>w</CName>
            <Function argument="3">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>h</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>240</YCoordinate>
            <CName>h</CName>
            <Function argument="4">
                <Function_ERT1_ID>12</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>to front</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>255</YCoordinate>
            <CName>to_front</CName>
            <Function argument="0">
                <Function_ERT1_ID>13</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>to back</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>265</YCoordinate>
            <CName>to_back</CName>
            <Function argument="0">
                <Function_ERT1_ID>14</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>255</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>13</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>265</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>14</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>130</XCoordinate>
            <YCoordinate>280</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__________</CName>
            <Function argument="1">
                <Function_ERT1_ID>11</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
