<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-02-23T09:46:47Z</CreationDate>
        <UpdatedDate>2024-09-09T11:46:15Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>PID Hardware Config int</ShortDescription>
        <LongDescription>Configure PID controller hardware Integer</LongDescription>
        <UserName/>
        <Menu>
            Control Systems
            <Menu>Hardware Config (int)</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Height>60</Height>
        <Text>PID HW Conf</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>pid_hdwr_config_int</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x53FD</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>bfcc1e79</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>id</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>10000</MaxValue>
            <Description>config id</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>value</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-2147483647</MinValue>
            <MaxValue>2147483647</MaxValue>
            <Description>config value</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
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
            <YCoordinate>12</YCoordinate>
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
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>configured</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>id</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>id</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>value</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>value</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errno</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>errno</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>80</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>err</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
