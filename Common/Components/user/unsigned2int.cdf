<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-09-10T16:58:48Z</CreationDate>
        <UpdatedDate>2024-09-11T11:39:38Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Convert signed integer with any length to a supported signed integer.</ShortDescription>
        <LongDescription>Convert signed integer with any length to a signed integer in the current platform.</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                Conversion
                <Menu>Integer Expander</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Height>55</Height>
        <Text>Word~</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>Unsigned2Int</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x4F75</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>33e74f57</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>bit_width</Name>
            <DataType>I</DataType>
            <DefaultValue>8</DefaultValue>
            <MinValue>2</MinValue>
            <MaxValue>1073741824</MaxValue>
            <Description>The bit width of the signed input</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
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
            <Description/>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>convert</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description/>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>value_in</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>55</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>55</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>value_out</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>e</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>55</XCoordinate>
            <YCoordinate>32</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
