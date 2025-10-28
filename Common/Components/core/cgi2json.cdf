<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-02-23T09:46:47Z</CreationDate>
        <UpdatedDate>2024-04-09T22:35:21Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>CGI to JSON</ShortDescription>
        <LongDescription>Converts CGI (Common Gateway Interface) string to JSON string</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                JSON
                <Menu>CGI to JSON</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Height>65</Height>
        <Text>cgi2json</Text>
        <TextX>17</TextX>
        <TextY>5</TextY>
        <TextScale>1.2</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>cgi2json</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x1920</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>e96d385b</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>convert</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>done</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>json</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>json</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>cgi</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>cgi</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>run</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>run</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>len</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>cgi_len</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>len</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>json_len</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
