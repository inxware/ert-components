<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Provides facilities to control PVR playback in terms of trick modes, play, pause and stop. </LongDescription>
        <UserName/>
        <Menu>
            Deprecated
            <Menu>
                DTV
                <Menu>Play</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>DtvPvrPlay1</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x823B</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>dcd285b6</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>set</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>get</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>play</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>pause</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>reverse</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>faster</name>
            <ID>
                <ERT1_ID>6</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>slower</name>
            <ID>
                <ERT1_ID>7</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>stop</name>
            <ID>
                <ERT1_ID>8</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>end</name>
            <ID>
                <ERT1_ID>9</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>set</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>set</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>get</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>get</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>play</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>play</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>pause</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>pause</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>rev</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>rev</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>faster</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>faster</CName>
            <Function argument="0">
                <Function_ERT1_ID>6</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>slower</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>slower</CName>
            <Function argument="0">
                <Function_ERT1_ID>7</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>stop</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <CName>stop</CName>
            <Function argument="0">
                <Function_ERT1_ID>8</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>name</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>pos</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>pos</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>set</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>get</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <Wcet>0</Wcet>
            <CName>get</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>play</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <Wcet>0</Wcet>
            <CName>play</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>pause</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <Wcet>0</Wcet>
            <CName>pause</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>rev</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <Wcet>0</Wcet>
            <CName>rev</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>faster</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>faster</CName>
            <Function argument="1">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>slower</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <Wcet>0</Wcet>
            <CName>slower</CName>
            <Function argument="1">
                <Function_ERT1_ID>7</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>stop</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <Wcet>0</Wcet>
            <CName>stop</CName>
            <Function argument="1">
                <Function_ERT1_ID>8</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>end</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <Wcet>0</Wcet>
            <CName>end</CName>
            <Function argument="1">
                <Function_ERT1_ID>9</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>pos</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>pos</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>speed</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>125</YCoordinate>
            <CName>speed</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <Wcet>0</Wcet>
            <CName>err</CName>
            <Function argument="2">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>7</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>8</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errNo</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>errNo</CName>
            <Function argument="1">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>7</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>8</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>End port</Description>
            <PortType>InternalPort</PortType>
            <XCoordinate>-1</XCoordinate>
            <YCoordinate>-1</YCoordinate>
            <CName>End_port</CName>
            <Function argument="0">
                <Function_ERT1_ID>9</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
