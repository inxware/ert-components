<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Switches Event based on text cases</LongDescription>
        <UserName/>
        <Menu>
            Event
            <Menu>
                Combiners/Flow
                <Menu>TextSwitch</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>30</Width>
        <Height>185</Height>
        <Text>TxSw</Text>
        <TextX>0</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>TextCaseDemultiplexer_Event</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xCF87</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>2acf08d7</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>case 1</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 1 string</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 2</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 2 string</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 3</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 3 string</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 4</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 4 string</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 5</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 5 string</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 6</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 6 string</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 7</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 7 string</Description>
            <ListPlacement>7</ListPlacement>
            <ArgPlacement>7</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>case 8</Name>
            <DataType>S</DataType>
            <DefaultValue/>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>case 8 string</Description>
            <ListPlacement>8</ListPlacement>
            <ArgPlacement>8</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>Run</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>go</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>go</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>test</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>test</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>1</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>1</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>2</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>2</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>3</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>3</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>4</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>4</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>5</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>5</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>6</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>6</CName>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>7</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <CName>7</CName>
            <Function argument="8">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>8</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>145</YCoordinate>
            <CName>8</CName>
            <Function argument="9">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>145</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_</CName>
            <Function argument="8">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>deflt</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <Wcet>0</Wcet>
            <CName>deflt</CName>
            <Function argument="9">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
