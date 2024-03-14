<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-12-19T12:23:59Z</CreationDate>
        <UpdatedDate>2023-12-19T12:29:11Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>This will convert the string that contains the hex to the string that contains the actual characters.</ShortDescription>
        <LongDescription>This will convert the string that contains the hex to the string that contains the actual characters. The odd number of characters in the hex string will be prepended with 0.</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                String Functions
                <Menu>
                    Raw Data Conversion
                    <Menu>Hex String to String</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>HexString2String</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x4746</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>7cc09a6d</FbApiDescriptorHash_CRC32>
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
            <DataType>S</DataType>
            <Description/>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>27</YCoordinate>
            <CName>hexStringInput</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
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
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>40</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>OK</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>40</XCoordinate>
            <YCoordinate>27</YCoordinate>
            <CName>stringOutput</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>40</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>length</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
