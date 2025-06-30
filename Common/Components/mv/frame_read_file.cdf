<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-08-14T10:46:06Z</CreationDate>
        <UpdatedDate>2025-06-18T08:07:51Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Reads frame from a file</ShortDescription>
        <LongDescription>Reads frame from a file</LongDescription>
        <UserName/>
        <Menu>
            Machine Vision
            <Menu>Frame Read From File</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Height>100</Height>
        <Text>Frame From File</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>FrameReadFromFile</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x5772</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>9cb74091</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Use Application Dir</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>File in Application Directory rather than the user data directory</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>run_read_file</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>105</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>start_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>105</XCoordinate>
            <YCoordinate>42</YCoordinate>
            <Wcet>0</Wcet>
            <CName>start_error</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errno</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>105</XCoordinate>
            <YCoordinate>52</YCoordinate>
            <CName>start_errno</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>run</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>run_fmt</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>stream_id</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>105</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>formated_frame_id</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>filepath</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>filepath</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
