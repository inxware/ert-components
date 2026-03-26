<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-03-16T00:00:00Z</CreationDate>
        <UpdatedDate>2026-03-16T00:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Buzzer</ShortDescription>
        <LongDescription>Piezo buzzer control. Plays a tone at a specified frequency for a specified duration, or stops an ongoing tone.</LongDescription>
        <UserName/>
        <Menu>
            Peripherals
            <Menu>Buzzer</Menu>

        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>75</Width>
        <Height>73</Height>
        <Text>Buzz</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>buzzer</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x3D16</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Frequency (Hz)</Name>
            <DataType>I</DataType>
            <DefaultValue>1000</DefaultValue>
            <MinValue>100</MinValue>
            <MaxValue>10000</MaxValue>
            <Description>Default tone frequency in Hz.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Duration (ms)</Name>
            <DataType>I</DataType>
            <DefaultValue>200</DefaultValue>
            <MinValue>1</MinValue>
            <MaxValue>60000</MaxValue>
            <Description>Default tone duration in milliseconds. 0 = continuous until stop.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>beep</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>stop</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>beep</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>beep</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>freq Hz</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>freq_hz</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>duration ms</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>duration_ms</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>error num</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>error_num</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>stop</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>stop</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName>stopped</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
