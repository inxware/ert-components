<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2024-08-22T19:40:58Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Storing real value samples in a string buffer.</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                Num
                <Menu>Samples to String</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Text>Sam2Str</Text>
        <TextX>13</TextX>
        <TextY>5</TextY>
        <TextScale>1.3</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>0</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>sample2str</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xE2FC</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>76855e16</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Sample Count</Name>
            <DataType>I</DataType>
            <DefaultValue>10</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>255</MaxValue>
            <Description>Number of samples to be stored in the output string</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Decimal Places</Name>
            <DataType>I</DataType>
            <DefaultValue>2</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>6</MaxValue>
            <Description>Max number of decimal places</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>Run_Input</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Clear</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>o</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>o</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>i</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>i</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>i1</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>i1</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>i2</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>i2</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>i3</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>i3</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>o1</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>o1</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>o2</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>o2</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>o3</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>o3</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>clear</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>62</YCoordinate>
            <CName>clear</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>62</YCoordinate>
            <Wcet>0</Wcet>
            <CName>cleared</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
