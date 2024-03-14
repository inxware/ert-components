<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>This object is asserted an event an edge is detected on a boolean.</LongDescription>
        <UserName/>
        <Menu>
            Event
            <Menu>
                Boolean Control
                <Menu>Edge Detect</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Event_Processor</Type>
        <Width>30</Width>
        <Height>45</Height>
        <Text>+/-E</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>EventEdgeDetect</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x3C33</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>7a8c59ce</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Boolean Initialvalue</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>initial value</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>Test</name>
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
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
