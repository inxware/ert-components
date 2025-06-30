<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2025-03-11T11:32:01Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Prints string to the operating systems console (if available)</ShortDescription>
        <LongDescription>Prints string to the operating systems console (if available)</LongDescription>
        <UserName/>
        <Menu>
            User Interface
            <Menu>Console Print</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Text>Print</Text>
        <TextX>42</TextX>
        <TextY>20</TextY>
        <TextScale>1.5</TextScale>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>console_print</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xF458</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>b1c99bc6</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>New Line</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Append 'New Line' at the end of the text.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>PrintFunc</name>
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
            <YCoordinate>12</YCoordinate>
            <CName>write</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>75</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>data</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>27</YCoordinate>
            <CName>data</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
