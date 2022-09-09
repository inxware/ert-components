<?xml version="1.0" encoding="UTF-8"?><Component>
    <CDFInfo>
        <Version>2</Version>
        <CreationDate>2022-08-25T11:00:20Z</CreationDate>
        <UpdatedDate>2022-08-25T11:01:05Z</UpdatedDate>
    </CDFInfo>
    <ID>
        <ERT1_ID>1</ERT1_ID>
    </ID>
    <Class>mqtt_publish</Class>
    <FbApiDescriptorHash>todo Api Descriptor</FbApiDescriptorHash>
    <Hashes>
        <NameHash_CRC16>0x16F8</NameHash_CRC16>
        <FbApiDescriptorHash_CRC16>todo Api Descriptor hash</FbApiDescriptorHash_CRC16>
    </Hashes>
    <Module>usercomponents</Module>
    <Menu>
        Networking
        <Menu>MQTT Publish</Menu>
    </Menu>
    <ShortDescription>mqtt_publish</ShortDescription>
    <LongDescription>mqtt_publish</LongDescription>
    <Functions>
        <Function>
            <name>publish</name>
            <ID>
                <ERT1_ID>0</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Port>
        <CName>publish</CName>
        <Description>publish</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <Function argument="0">
            <Function_ERT1_ID>0</Function_ERT1_ID>
        </Function>
    </Port>
    <Port>
        <CName>finishpublish</CName>
        <Description>--</Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>130</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <Function argument="1">
            <Function_ERT1_ID>0</Function_ERT1_ID>
        </Function>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>topic</CName>
        <Description>topic name</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <Function argument="1">
            <Function_ERT1_ID>0</Function_ERT1_ID>
        </Function>
    </Port>
    <Port>
        <DataType>S</DataType>
        <CName>payload</CName>
        <Description>Payload</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>50</YCoordinate>
        <Function argument="2">
            <Function_ERT1_ID>0</Function_ERT1_ID>
        </Function>
    </Port>
    <Port>
        <DataType>I</DataType>
        <CName>qos</CName>
        <Description>qos</Description>
        <PortType>InputPort</PortType>
        <XCoordinate>0</XCoordinate>
        <YCoordinate>70</YCoordinate>
        <Function argument="3">
            <Function_ERT1_ID>0</Function_ERT1_ID>
        </Function>
    </Port>
</Component>
