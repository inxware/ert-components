<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2026-08-11T09:00:00Z</CreationDate>
        <UpdatedDate>2026-08-11T09:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Install a pre-built eSIM profile package locally, with no network</ShortDescription>
        <LongDescription>Installs a Bound Profile Package that the factory's equipment has already obtained, streaming it into the eSIM over a local link. The DEVICE needs no network connection at all.

PROCESS. Trigger ReadEID and hand the EID to the factory equipment; it requests from the operator a package bound to that specific eSIM. Then Begin, PushSeg once per segment in order, and Commit. Commit reports the ICCID installed, which the eSIM Profiles block then enables.

NO OFFSET, AND NO RETRY OF A SINGLE SEGMENT. The eSIM sequences segments by their block number and reassembles by concatenation, so they must arrive in order and exactly once; there is no addressing and no seeking. Because each segment's authentication code is chained to the previous one, a rejected segment cannot be re-sent - Abort and restart the whole package.

DO NOT ADD A CHECKSUM. Every segment carries a cryptographic authentication code that is verified inside the eSIM, and no part of this path holds the keys to forge one. Corruption introduced anywhere - by the equipment, the link, or this device - fails that check. An application-level checksum would catch nothing extra and would imply a guarantee it does not give.

THE PACKAGE IS NEVER STORED. Segments are decoded, passed to the eSIM and wiped; nothing is written to the filesystem, so no subscription material is left on the device after provisioning.

segData carries one segment as HEX, because a string port is NUL-terminated and cannot carry raw binary. How many bytes fit per push is a property of the platform, not of the eSIM: a hex segment costs two characters per byte, so a target whose string ports hold 256 characters takes 127 bytes per push. That is smaller than the eSIM's own 255-byte transport block, so a push is not the same thing as a block - the driver reassembles. Feed segments in order and let it do that.

Requires an eSIM chip and a vendor-supplied local install path. No currently supported target has either, so every operation reports not-supported.</LongDescription>
        <UserName/>
        <Menu>
            Networking
            <Menu>
                Cellular
                <Menu>
                    SIM Provisioning
                    <Menu>Factory Install</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>130</Width>
        <Height>243</Height>
        <Text>Factory Install</Text>
        <TextX>11</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>esim_factory_prov</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x4604</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32/>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>do_read_eid</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_begin_install</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_push_segment</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_commit_install</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_abort_install</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>ReadEID</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>read_eid</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>RDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>read_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>RFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>read_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>EID</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>eid</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>read_err_code</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>maxSegLen</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>max_seg_len</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Begin</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>begin_install</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>BDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <Wcet>0</Wcet>
            <CName>begin_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>BFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <Wcet>0</Wcet>
            <CName>begin_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>95</YCoordinate>
            <CName>begin_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>PushSeg</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <CName>push_segment</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>segData</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>125</YCoordinate>
            <CName>seg_data</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>segLen</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>seg_len</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>PDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>115</YCoordinate>
            <Wcet>0</Wcet>
            <CName>push_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>PFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>125</YCoordinate>
            <Wcet>0</Wcet>
            <CName>push_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>push_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Commit</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>155</YCoordinate>
            <CName>commit_install</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>CDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>155</YCoordinate>
            <Wcet>0</Wcet>
            <CName>commit_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>CFail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>165</YCoordinate>
            <Wcet>0</Wcet>
            <CName>commit_fail</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>instICCID</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <CName>inst_iccid</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>185</YCoordinate>
            <CName>commit_err_code</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>Abort</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>205</YCoordinate>
            <CName>abort_install</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>ADone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>205</YCoordinate>
            <Wcet>0</Wcet>
            <CName>abort_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errCode</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>125</XCoordinate>
            <YCoordinate>215</YCoordinate>
            <CName>abort_err_code</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
