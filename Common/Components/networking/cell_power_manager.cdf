<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-08-06T09:00:00Z</CreationDate>
        <UpdatedDate>2026-08-06T09:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Cellular power saving (PSM / eDRX / RAI)</ShortDescription>
        <LongDescription>Requests the cellular power-saving modes and reports what the network actually granted.

The distinction matters: you request, the network grants, and the grant is frequently different from the request or refused outright. Always read the granted values rather than assuming the request took effect.

A device in Power Saving Mode is still registered and resumes data on wake - it is not disconnected, and the Modem Manager block reports it as still connected.</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                Cellular
                <Menu>Power Manager</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>95</Width>
        <Height>143</Height>
        <Text>Cell Power</Text>
        <TextX>11</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>cell_power_manager</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x0322</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32/>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>psmEnable</Name>
            <DataType>B</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Request Power Saving Mode. The modem sleeps deeply between periodic wake-ups; the device stays registered.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>tauSeconds</Name>
            <DataType>I</DataType>
            <DefaultValue>3600</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>35712000</MaxValue>
            <Description>Requested periodic tracking-area-update interval in seconds - how long the device may sleep before it must contact the network.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>activeSeconds</Name>
            <DataType>I</DataType>
            <DefaultValue>60</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>11160</MaxValue>
            <Description>Requested active time in seconds - how long the device stays reachable after each transmission.</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>edrxEnable</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Request extended Discontinuous Reception, which lengthens the paging interval so the device can be reached with less power than always-on but more often than PSM.</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>edrxCycleMs</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>2621440</MaxValue>
            <Description>Requested eDRX cycle in milliseconds. 0 lets the modem choose.</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>asRai</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Request Access Stratum release assistance, which tells the network the device has finished sending so the radio can release sooner.</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>cpRai</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Request Control Plane release assistance.</Description>
            <ListPlacement>7</ListPlacement>
            <ArgPlacement>7</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>do_apply</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_read</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>Apply</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>apply</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>ADone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>apply_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>AFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>apply_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Read</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>read</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>RDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName>read_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>psmGranted</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>psm_granted</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>grantedTau</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>granted_tau</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>grantedActive</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>granted_active</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>edrxGranted</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>edrx_granted</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>grantedEdrxMs</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>granted_edrx_ms</CName>
            <Function argument="5">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>grantedPtwMs</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>granted_ptw_ms</CName>
            <Function argument="6">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>raiOk</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>90</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>rai_ok</CName>
            <Function argument="7">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
