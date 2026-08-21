<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-08-10T09:00:00Z</CreationDate>
        <UpdatedDate>2026-08-10T09:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Download an eSIM profile over GSMA SGP.32 RSP</ShortDescription>
        <LongDescription>Downloads a new subscription profile onto an eSIM (eUICC) using GSMA SGP.32 remote SIM provisioning, and configures the eIM address that provisioning runs against.

THIS IS THE ONE eSIM OPERATION THAT NEEDS A SERVER. The download is a mutually authenticated session with an SM-DP+ / eIM, and the resulting profile package is cryptographically bound to this specific eSIM and to that session - it cannot be fetched now and installed later, nor prepared for one device and applied to another.

The session needs IP reachability to the server, but NOT necessarily over the cellular link: a factory line can carry it over Ethernet, Wi-Fi or a test fixture, which is what breaks the circular 'need a network to get a SIM' problem.

Managing profiles that are already installed needs no server at all - use the eSIM Profiles block for that.

Requires an eSIM chip and an SGP.32 provisioning stack. No currently supported target has either, so every operation reports not-supported.</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                Cellular
                <Menu>
                    SIM Provisioning
                    <Menu>SGP.32 Download</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>130</Width>
        <Height>103</Height>
        <Text>SGP.32 Download</Text>
        <TextX>11</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>sgp32_download</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x0C2B</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32/>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>do_set_eim</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_download</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>SetEIM</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>set_eim</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>eimAddress</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>eim_address</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>EDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>set_eim_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>EFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>set_eim_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Download</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>download</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>activationCode</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>activation_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>DDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName>dl_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>DFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <Wcet>0</Wcet>
            <CName>dl_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>dl_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>dlICCID</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>dl_iccid</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
