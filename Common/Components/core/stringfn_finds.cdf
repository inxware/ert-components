<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>This function determines if a string s2 exists in string s1.</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                String Functions
                <Menu>find</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>55</Width>
        <Height>50</Height>
        <Text>find str</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>0</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>FindString</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x7D98</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>e46a2934</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Reverse Search</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Biases search for backward searching</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Find String (-&gt; s2)</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>String to compare to unless overridden by port s2</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>Run_FindString</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <DataType>S</DataType>
            <Description>s1</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>s1</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>s2</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>s2</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName/>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName/>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>index</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>index</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
