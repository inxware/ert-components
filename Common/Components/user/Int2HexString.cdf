<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-12-21T11:28:43Z</CreationDate>
        <UpdatedDate>2023-12-21T11:30:05Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Convert an integer to the string that contains hex data, e.g. 10 to "0A".</ShortDescription>
        <LongDescription>Convert an integer to the string that contains hex data, e.g. 10 to "0A".</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                Conversion
                <Menu>
                    integer-&gt;
                    <Menu>string (hex)</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Text>~Hex</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>0</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>Int2HexString</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x0FE6</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>42bd2e13</FbApiDescriptorHash_CRC32>
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
            <Description/>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>start</CName>
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
            <YCoordinate>25</YCoordinate>
            <CName>input</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>30</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>finish</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>30</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>output</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
