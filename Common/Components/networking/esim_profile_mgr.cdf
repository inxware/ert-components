<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-08-10T09:00:00Z</CreationDate>
        <UpdatedDate>2026-08-10T09:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Local eSIM profile lifecycle (list, enable, delete)</ShortDescription>
        <LongDescription>Manages the subscription profiles already present on an eSIM (eUICC): list them, read their details, and enable, disable or delete them.

EVERY OPERATION HERE IS LOCAL. These are GSMA ES10c commands exchanged directly with the eSIM chip, so none of them needs a network of any kind - not cellular, not IP. That makes this block usable on a factory line with no connectivity, to select and verify the correct profile on a device whose profiles were loaded during manufacture.

Local profile management is common to GSMA SGP.22 and SGP.32 - it is not specific to either. Downloading a NEW profile is the one operation that needs a server, and it lives in the separate SGP.32 Download block.

Requires an eSIM chip. No currently supported target has one populated, so every operation reports not-supported until eSIM hardware is available.</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                Cellular
                <Menu>
                    SIM Provisioning
                    <Menu>eSIM Profiles</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>115</Width>
        <Height>263</Height>
        <Text>eSIM Profiles</Text>
        <TextX>10</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>esim_profile_mgr</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x6004</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32/>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>do_list_profiles</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_get_profile</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_enable_profile</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_disable_profile</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_delete_profile</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>ListProfs</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>list_profiles</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>LDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>list_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>LFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>list_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>profCount</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>profile_count</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>list_err_code</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>GetProfile</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>get_profile</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>index</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>profile_index</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>GDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <Wcet>0</Wcet>
            <CName>get_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>GFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <Wcet>0</Wcet>
            <CName>get_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>profICCID</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>profile_iccid</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>profName</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>profile_name</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>isEnabled</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>105</YCoordinate>
            <CName>profile_enabled</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>get_err_code</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Enable</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>enable_profile</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>ICCID</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>145</YCoordinate>
            <CName>enable_iccid</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>NDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <Wcet>0</Wcet>
            <CName>enable_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>NFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>145</YCoordinate>
            <Wcet>0</Wcet>
            <CName>enable_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>155</YCoordinate>
            <CName>enable_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Disable</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <CName>disable_profile</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>ICCID</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>185</YCoordinate>
            <CName>disable_iccid</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>BDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <Wcet>0</Wcet>
            <CName>disable_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>BFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>185</YCoordinate>
            <Wcet>0</Wcet>
            <CName>disable_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>195</YCoordinate>
            <CName>disable_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Delete</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>215</YCoordinate>
            <CName>delete_profile</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>ICCID</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>225</YCoordinate>
            <CName>delete_iccid</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>XDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>215</YCoordinate>
            <Wcet>0</Wcet>
            <CName>delete_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>XFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>225</YCoordinate>
            <Wcet>0</Wcet>
            <CName>delete_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>110</XCoordinate>
            <YCoordinate>235</YCoordinate>
            <CName>delete_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
