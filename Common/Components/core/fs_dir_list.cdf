<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Allows a directory path to be set and outputs files contained in the directory on each get. Separate events are asserted for each type of file or directory.</LongDescription>
        <UserName/>
        <Menu>
            File System
            <Menu>
                Directories
                <Menu>List Entries</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>65</Width>
        <Height>102</Height>
        <Text>FS List</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>FileSystemDirList1</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x4186</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>7bd78af2</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Directory</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Media Directory</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use Application Dir</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Find Files in Application Directory rather than the user data directory</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>set</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>get</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>rel</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>set dir</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>set_dir</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>dir</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>dir</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>ready</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>ready</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>27</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>get entry</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>get_entry</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>67</YCoordinate>
            <CName>name</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>file</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <Wcet>0</Wcet>
            <CName>file</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>dir</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>47</YCoordinate>
            <Wcet>0</Wcet>
            <CName>dir</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>no more</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>57</YCoordinate>
            <Wcet>0</Wcet>
            <CName>no_more</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>release</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>77</YCoordinate>
            <CName>release</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>77</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
