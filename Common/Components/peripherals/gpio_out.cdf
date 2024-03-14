<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>GPIO Output</ShortDescription>
        <LongDescription>GPIO Output</LongDescription>
        <UserName/>
        <Menu>
            Peripherals
            <Menu>
                GPIO
                <Menu>GPIO Out</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>50</Width>
        <Height>45</Height>
        <Text>GPIO</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>gpio_out</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x87EF</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>b9d78b3f</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Pin ID</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>32767</MaxValue>
            <Description>Pin ID</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Intial Value</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>2</MaxValue>
            <Description>Intial Value (2 Hi Z) </Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
	 <Parameter>
            <Name>Open Drain</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Logic High = Hi Z) </Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>write</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>write</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>write</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description/>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>value</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
