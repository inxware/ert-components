<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-03-16T00:00:00Z</CreationDate>
        <UpdatedDate>2026-03-16T00:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Accelerometer</ShortDescription>
        <LongDescription>DEPRECATED — use the accel_gyro block instead. The accel_gyro block is a superset of this block and supports both accelerometer and gyroscope via the same HAL switching mechanism. This block remains available for backward compatibility only.</LongDescription>
        <UserName/>
        <Menu>
            Deprecated
            <Menu>Accelerometer</Menu>

        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>50</Width>
        <Height>90</Height>
        <Text>Accel</Text>
        <TextX>8</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>accelerometer</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x69DD</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>enable</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>disable</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>read</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>enable</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>enable</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>enabled</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>disable</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>disable</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <Wcet>0</Wcet>
            <CName>disabled</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>error id</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>32</YCoordinate>
            <CName>error_id</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>read</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>47</YCoordinate>
            <CName>read</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>47</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>X (mg)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>57</YCoordinate>
            <CName>x</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>Y (mg)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>67</YCoordinate>
            <CName>y</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>Z (mg)</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>77</YCoordinate>
            <CName>z</CName>
            <Function argument="3">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
