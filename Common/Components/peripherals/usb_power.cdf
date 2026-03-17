<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-03-16T00:00:00Z</CreationDate>
        <UpdatedDate>2026-03-16T00:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>USB Power</ShortDescription>
        <LongDescription>Software-controlled USB port power switching. Enables or disables power to an individual USB host port and reads the current power state.</LongDescription>
        <UserName/>
        <Menu>
            Peripherals
            <Menu>
                USB
                <Menu>USB Power</Menu>
            </Menu>

        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>50</Width>
        <Height>65</Height>
        <Text>USB</Text>
        <TextX>13</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>usb_power</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x0000</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Port</Name>
            <DataType>I</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>1</MinValue>
            <MaxValue>4</MaxValue>
            <Description>USB port number to control (1-4).</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
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
            <name>read_status</name>
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
            <YCoordinate>10</YCoordinate>
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
            <YCoordinate>10</YCoordinate>
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
            <YCoordinate>20</YCoordinate>
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
            <YCoordinate>20</YCoordinate>
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
            <YCoordinate>30</YCoordinate>
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
            <YCoordinate>45</YCoordinate>
            <CName>read_status</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName>status_done</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>powered</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>powered</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
