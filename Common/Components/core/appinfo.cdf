<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Returns the app automatic version number, data stamp and given name,    </LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>
                Local
                <Menu>App Info</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>100</Width>
        <Height>105</Height>
        <Text>App Info</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>AppInfo</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xF207</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>e9f0ed18</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Default App Name</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Name of app to identify. Blank gives currently loaded application info. Use "dvm://devman-default/" for the default devman app server or full url dvm:// to get version info.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>getinfo</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>get info</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <CName>get_info</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>version</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>version</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>build no </Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>build_no_</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>date</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>date</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>min EHS</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>min_EHS</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>depndcs</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>depndcs</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>proc grps</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <CName>proc_grps</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>name</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName/>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
