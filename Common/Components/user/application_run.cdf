<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Runs an app</ShortDescription>
        <LongDescription>Runs an app</LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>
                Local
                <Menu>Run Application</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>100</Width>
        <Height>50</Height>
        <Text>Run Application</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>application_run</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xB273</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>89314fa9</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>runApp</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>Run App</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>Run_App</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>---[run]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>____run_</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error[run]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_run_</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>Name</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
