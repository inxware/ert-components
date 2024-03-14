<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>event counter providing option to set a maximum and minimum threshold with an input value.</LongDescription>
        <UserName/>
        <Menu>
            Event
            <Menu>
                Counters/Iterators
                <Menu>Counter Versatile</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Event_Processor</Type>
        <Width>65</Width>
        <Height>175</Height>
        <Text>Counter</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.5</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>EventCounterVersatile1</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xD5F3</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>60f87356</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Enable at Init</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <Description>Enable the counter at initialisation time if TRUE, else counter is disabled at initialisation time.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Initial Value</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-2147483648</MinValue>
            <MaxValue>2147483647</MaxValue>
            <Description>Initial Value for count.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Step Size</Name>
            <DataType>I</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>1</MinValue>
            <MaxValue>2147483647</MaxValue>
            <Description>Value incremented/decremented on each count.</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Max. Threshold</Name>
            <DataType>I</DataType>
            <DefaultValue>1000</DefaultValue>
            <MinValue>-2147483648</MinValue>
            <MaxValue>2147483647</MaxValue>
            <Description>Threshold value for ovf event.</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Min. Threshold</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-2147483648</MinValue>
            <MaxValue>2147483647</MaxValue>
            <Description>Threshold value for ovf event.</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>Run_StartEventCounter</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Run_CountEventCounter</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Run_DecrementEventCounter</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Run_ResetEventCounter</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>Run_StopEventCounter</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>enabl</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>enabl</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>inc</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>inc</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>dec</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>dec</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>rst</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>rst</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>disabl</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>disabl</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>max</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>max</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>min</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>min</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ini</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>ini</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="3">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="3">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>step</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <CName>step</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="4">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="4">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>cnt</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>cnt</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>ovf</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <Wcet>0</Wcet>
            <CName>ovf</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>ovf</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>ovf</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
