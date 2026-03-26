<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2026-03-26T00:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>ADC Read (Legacy)</ShortDescription>
        <LongDescription>Legacy ADC read block. Use adc_config + adc_read_single or adc_read_continuous for new designs.</LongDescription>
        <UserName/>
        <Menu>
            Deprecated
            <Menu>
                ADC
                <Menu>ADC Read (Legacy)</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>75</Width>
        <Height>75</Height>
        <Text>ADC Legacy</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>adc_read</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xA05C</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>6ce9f97d</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>channel</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>4096</MaxValue>
            <Description>ADC channel</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
            <DisplayOnWidget>
                <x>45</x>
                <y>5</y>
            </DisplayOnWidget>
        </Parameter>
        <Parameter>
            <Name>enable_continuous</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Enable continuous mode clock</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>clock_rate_hz</Name>
            <DataType>F</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>9000000000</MaxValue>
            <Description>Optional clock rate (Hz) for continous mode</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Average</Name>
            <DataType>I</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1000000</MaxValue>
            <Description>Acquisitions to average</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Bias</Name>
            <DataType>F</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-100000</MinValue>
            <MaxValue>100000</MaxValue>
            <Description>The bias to be taken from the average</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>read</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>sample</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>Set Channel</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>read</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>channel</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>channel</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Sample</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>37</YCoordinate>
            <CName>sample</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>value</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>50</YCoordinate>
            <CName>value</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <Wcet>0</Wcet>
            <CName>finishread</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
