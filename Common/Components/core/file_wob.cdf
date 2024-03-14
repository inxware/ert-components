<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Writes boolean data to a file. If a field separator parameter is specified then then this will be used inserted between data points. Fixed width is not available for this data type. The append option specified if the file is truncated to an empty file when opened or new data is appended to existing data.</LongDescription>
        <UserName/>
        <Menu>
            File System
            <Menu>
                File Access
                <Menu>
                    File Output
                    <Menu>bool</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>65</Width>
        <Height>120</Height>
        <Text>File WO</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>FILE_WriteOnly_Bool</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xEA51</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>d2a9128b</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>File name</Name>
            <DataType>S</DataType>
            <DefaultValue>out.txt</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>output file name</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Delimiter</Name>
            <DataType>S</DataType>
            <DefaultValue>( )</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Boolean Field Delimiter Character</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Append</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Append rather than overwrite.</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>Run_OpenFILE_WriteOnly_Bool</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Run_CloseFILE_WriteOnly_Bool</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Run_WriteFILE_WriteOnly_Bool</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>open</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>open</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>path</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>path</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>close</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>close</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>write</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>write</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>data</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>data</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>err code</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <Wcet>0</Wcet>
            <CName>err</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName>err</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <Wcet>0</Wcet>
            <CName>err</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
