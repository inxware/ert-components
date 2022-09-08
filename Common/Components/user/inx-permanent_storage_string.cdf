<?xml version="1.0" encoding="UTF-8"?><Component>
    <Class>permanent_storage_string</Class>
    <FbApiDescriptorHash>todo Api Descriptor</FbApiDescriptorHash>
    <Hashes>
        <NameHash_CRC16>0x5332</NameHash_CRC16>
        <FbApiDescriptorHash_CRC16>todo Api Descriptor hash</FbApiDescriptorHash_CRC16>
    </Hashes>
    <Module>usercomponents</Module>
    <Menu>
        System
        <Menu>Permanent Storage String</Menu>
    </Menu>
    <ShortDescription>permanent_storage_string</ShortDescription>
    <LongDescription>permanent_storage_string</LongDescription>
    <Port>
        <CName>write</CName>
        <Description>Write</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0x44d8" hash_CRC16="0x44d8">write</FunctionName>
    </Port>
    <Port>
        <CName>read</CName>
        <Description>Read</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0" hash="0x1c23" hash_CRC16="0x1c23">read</FunctionName>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>id</CName>
        <Description>id</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1" hash="0x44d8" hash_CRC16="0x44d8">write</FunctionName>
        <FunctionName argument="1" hash="0x1c23" hash_CRC16="0x1c23">read</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <FunctionName argument="2" hash="0x44d8" hash_CRC16="0x44d8">write</FunctionName>
    </Port>
    <Port>
        <CName>finishwrite</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>65</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0x44d8" hash_CRC16="0x44d8">write</FunctionName>
    </Port>
    <Port>
        <CName>finishread</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>65</XCoordinate>
        <YCoordinate>20</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1" hash="0x1c23" hash_CRC16="0x1c23">read</FunctionName>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>value</CName>
        <Description>value</Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>65</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1" hash="0x1c23" hash_CRC16="0x1c23">read</FunctionName>
    </Port>
    <Port>
        <CName>finisherr</CName>
        <Description>read error</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>65</XCoordinate>
        <YCoordinate>40</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="2" hash="0x1c23" hash_CRC16="0x1c23">read</FunctionName>
    </Port>
</Component>
