<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Device ID: MAC address, Network Address: IP addr, SODL Type: SODL format version, SODL name: Name of application,    </LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>
                Local
                <Menu>Runtime Info</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>100</Width>
        <Height>205</Height>
        <Text>Runtime Info</Text>
        <TextX>20</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>RuntimeInfo</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x895B</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>ae227010</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>getstat</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>getdyn</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>reqpairid</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>GetStatic</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>GetStatic</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>MiscApp Name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>GetStatic</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Device ID</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>Device_ID</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RAM (KB) </Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>RAM__KB__</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Stor (KB)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>Stor__KB_</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>EHS version</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>EHS_version</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>ModuleList</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>ModuleList</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Install Dir</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>Install_Dir</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>GetDynamic</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>GetDynamic</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>IP Addr</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>IP_Addr</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RAM avail kB</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>RAM_Avail__KB_</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Store avail kB</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>Stor_Avail__KB_</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>cpu (eRT)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>125</YCoordinate>
            <CName>CPU</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RAM (eRT)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>CPU</CName>
            <Function argument="6">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>cpu (misc)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>145</YCoordinate>
            <CName>CPU</CName>
            <Function argument="7">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RAM (misc)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>155</YCoordinate>
            <CName>CPU</CName>
            <Function argument="8">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Req. Pair ID</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>170</YCoordinate>
            <CName>Req__Pair_ID</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Pair ID</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>180</YCoordinate>
            <CName>Pair_ID</CName>
            <Function argument="5">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Pair ID</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>170</YCoordinate>
            <Wcet>0</Wcet>
            <CName>Pair_ID</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
