<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>This is an event controlled by a boolean.</LongDescription>
        <UserName/>
        <Menu>
            Event
            <Menu>
                Boolean Control
                <Menu>Bool Control</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Event_Processor</Type>
        <Height>30</Height>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>EventBooleanControl</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xA177</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>215c094d</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>Update</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <DataType>B</DataType>
            <Description/>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <CName/>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>T </Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>18</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <Wcet>0</Wcet>
            <CName>T_</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>F</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>18</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <Wcet>0</Wcet>
            <CName>F</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
