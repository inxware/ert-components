<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription/>
        <UserName/>
        <Menu>
            Software Structure
            <Menu>
                State Machine
                <Menu>State Debug</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>State_Debug</Type>
        <Width>65</Width>
        <Height>50</Height>
        <Text>FSM Debug</Text>
        <TextX>7</TextX>
        <TextY>5</TextY>
        <TextScale>1.0</TextScale>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>state_debug</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x37D5</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>8c5ca04d</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>Run</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>internal</Description>
            <PortType>InternalPort</PortType>
            <XCoordinate>-1</XCoordinate>
            <YCoordinate>-1</YCoordinate>
            <CName>internal</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description/>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
