<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-08-06T09:00:00Z</CreationDate>
        <UpdatedDate>2026-08-06T09:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Cellular signal quality and data usage</ShortDescription>
        <LongDescription>Reads live link quality and metered-link accounting in one place.

In full mode this also performs a pre-transmit link evaluation, which answers whether sending right now is worth the battery BEFORE any data is queued. That is a cellular-specific capability with no Wi-Fi equivalent, and is the main reason to use this block on a battery device.

Values that the modem cannot supply are reported as 32767 rather than 0, so an unavailable reading cannot be mistaken for a real one. evalOk is false when the full evaluation is unavailable, which is normal when not registered or while the radio is active.</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                Cellular
                <Menu>Status</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>95</Width>
        <Height>183</Height>
        <Text>Cell Status</Text>
        <TextX>8</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>cell_status</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x0A36</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32/>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>mode</Name>
            <DataType>I</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>0 reads signal strength only. 1 additionally performs the full pre-transmit link evaluation, which requires modem support and is only valid while registered and idle.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>do_read</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_reset_counters</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>Read</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>read</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>Done</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>read_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RSRP</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>rsrp</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RSRQ</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>rsrq</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>SNR</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>snr</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>CELevel</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>ce_level</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>TXPower</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>tx_power</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>TXReps</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>tx_reps</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RXReps</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>rx_reps</CName>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>DLPathloss</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>dl_pathloss</CName>
            <Function argument="8">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Energy</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>energy_estimate</CName>
            <Function argument="9">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>evalOk</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>eval_ok</CName>
            <Function argument="10">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>TXBytes</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>125</YCoordinate>
            <CName>tx_bytes</CName>
            <Function argument="11">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>RXBytes</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>rx_bytes</CName>
            <Function argument="12">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>ResetCounters</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>155</YCoordinate>
            <CName>reset_counters</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>RDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>155</YCoordinate>
            <Wcet>0</Wcet>
            <CName>reset_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
