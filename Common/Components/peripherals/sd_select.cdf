<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-03-16T00:00:00Z</CreationDate>
        <UpdatedDate>2026-03-16T00:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>SD Card Select</ShortDescription>
        <LongDescription>Dual SD card slot selector. Switches the active boot/storage SD slot and reads the currently active slot. Requires a system reboot to take effect.</LongDescription>
        <UserName/>
        <Menu>
            Peripherals
            <Menu>
                Storage
                <Menu>SD Card</Menu>
            </Menu>

        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>50</Width>
        <Height>55</Height>
        <Text>SD</Text>
        <TextX>18</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>sd_select</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x0000</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>00000000</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>select</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>read_active</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>select</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>select</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>slot (0 or 1)</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>slot</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>error id</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>error_id</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>read active</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>read_active</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <Wcet>0</Wcet>
            <CName>read_done</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>active slot</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>50</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>active_slot</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
