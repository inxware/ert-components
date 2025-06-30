<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-05-13T11:43:55Z</CreationDate>
        <UpdatedDate>2024-05-13T11:56:46Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Display backlight intensity</ShortDescription>
        <LongDescription>Set the dislpay backlight intensity</LongDescription>
        <UserName/>
        <Menu>
            User Interface
            <Menu>Backlight Intensity</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Text>Backlight</Text>
        <TextX>15</TextX>
        <TextY>2</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>0</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>Backlight</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x37A7</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>45c495b9</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>set_intensity</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>set</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>set</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>65</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>intensity</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>27</YCoordinate>
            <CName>intensity</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
