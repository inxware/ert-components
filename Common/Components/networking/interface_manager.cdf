<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2025-10-03T15:45:56Z</CreationDate>
        <UpdatedDate>2025-10-03T15:49:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Manage the network interface</ShortDescription>
        <LongDescription>This manages the network interface. It can switch the interface between Ethernet and Wi-Fi.</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>Interface Manager</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Height>60</Height>
        <Text>NetIF Config</Text>
        <TextX>7</TextX>
        <TextY>5</TextY>
        <TextScale>1.3</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>interface_manager</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xF2F0</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>8fd1ddb9</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>configure</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>config</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>config</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>configured</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>wifi</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>wifi_enable</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>ethernet</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>eth_enable</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>err</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>errorCode</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
