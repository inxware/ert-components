<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>adc_read</Class>
    <FbApiDescriptorHash>todo Api Descriptor</FbApiDescriptorHash>
    <Hashes>
        <NameHash_CRC16>0xA05C</NameHash_CRC16>
        <FbApiDescriptorHash_CRC16>todo Api Descriptor hash</FbApiDescriptorHash_CRC16>
    </Hashes>
    <Module>usercomponents</Module>
    <Menu>
        Peripherals
        <Menu>ADC Read</Menu>
    </Menu>
    <ShortDescription>adc_read</ShortDescription>
    <LongDescription>adc_read</LongDescription>
    <Parameter>
        <Name>channel</Name>
        <DataType>I</DataType>
        <DefaultValue>0</DefaultValue>
        <MinValue>0</MinValue>
        <MaxValue>4096</MaxValue>
        <Description>ADC channel</Description>
        <DisplayOnWidget>
            <x>10</x>
            <y>20</y>
        </DisplayOnWidget>
        <ListPlacement>1</ListPlacement>
        <ArgPlacement>1</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>enable_continuous</Name>
        <DataType>B</DataType>
        <DefaultValue>0</DefaultValue>
        <MinValue/>
        <MaxValue/>
        <Description>Enable continuous mode clock</Description>
        <ListPlacement>2</ListPlacement>
        <ArgPlacement>2</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>clock_rate_hz</Name>
        <DataType>F</DataType>
        <DefaultValue>1</DefaultValue>
        <MinValue/>
        <MaxValue/>
        <Description>Optional clock rate (Hz) for continous mode</Description>
        <ListPlacement>3</ListPlacement>
        <ArgPlacement>3</ArgPlacement>
    </Parameter>
    <Parameter>
        <Name>Average</Name>
        <DataType>I</DataType>
        <DefaultValue/>
        <MinValue>0</MinValue>
        <MaxValue>1000000</MaxValue>
        <Description>Acquisitions to average</Description>
        <ListPlacement>4</ListPlacement>
        <ArgPlacement>4</ArgPlacement>
    </Parameter>
    <Port>
        <CName>read</CName>
        <Description>Set Channel</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash_8bit="0" hash_CRC16="0x1C23">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>channel</CName>
        <Description>channel</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <FunctionName argument="1" hash_8bit="0" hash_CRC16="0x1C23">read</FunctionName>
    </Port>
    <Port>
        <CName>sample</CName>
        <Description>Sample</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>37</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash_8bit="1" hash_CRC16="0x9746">sample</FunctionName>
    </Port>
    <Port>
        <DataType>F</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <FunctionName argument="1" hash_8bit="0" hash_CRC16="0x1C23">read</FunctionName>
        <FunctionName argument="1" hash_8bit="1" hash_CRC16="0x9746">sample</FunctionName>
    </Port>
    <Port>
        <CName>finishread</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>37</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash_8bit="0" hash_CRC16="0x1C23">read</FunctionName>
        <FunctionName argument="1" hash_8bit="1" hash_CRC16="0x9746">sample</FunctionName>
    </Port>
</Component>
