<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>XML Stream Parser</ShortDescription>
        <LongDescription>Parsers XML as a stream generating events for the XML chunks it sees.</LongDescription>
        <UserName/>
        <Menu>
            Data Utilities
            <Menu>
                XML
                <Menu>XML Stream Parser</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>XML Parser</Type>
        <Height>305</Height>
        <Text>XML Parser</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.5</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>xml_stream_parser</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x4AFF</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>7f5c680a</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>ignore_script</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Ignore content of script tags.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>ignore_entities</Name>
            <DataType>S</DataType>
            <DefaultValue>&amp;</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Replace &amp; before expat sees it. Try to put it back on the way out.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>eos</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>read</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>parse</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>streamFinished</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>reset</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>reset</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>read</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>read</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>data</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>data</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>next</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>next</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>startElem</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <Wcet>0</Wcet>
            <CName>start_element</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>finishElem</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>90</YCoordinate>
            <Wcet>0</Wcet>
            <CName>finish_element</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>name</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>reset_finish</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <Wcet>0</Wcet>
            <CName>read_finish</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>--</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>data_out</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>next_finish</CName>
            <Function argument="3">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>charData</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <Wcet>0</Wcet>
            <CName>character_data</CName>
            <Function argument="4">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>charData</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>cdata</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        
        <Port>
            <Description>attribute</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <Wcet>0</Wcet>
            <CName>attribute</CName>
            <Function argument="5">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>attrName</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>attr_name</CName>
            <Function argument="3">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>attrValue</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>attr_value</CName>
            <Function argument="4">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="6">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>procAvail</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>170</YCoordinate>
            <Wcet>0</Wcet>
            <CName>processing_finish</CName>
            <Function argument="7">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>procTarget</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>180</YCoordinate>
            <CName>processing_target</CName>
            <Function argument="5">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>procInstr</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>190</YCoordinate>
            <CName>processing_instruction</CName>
            <Function argument="6">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>comment</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <CName>comment</CName>
            <Function argument="7">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>comment</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>210</YCoordinate>
            <Wcet>0</Wcet>
            <CName>comment_finish</CName>
            <Function argument="8">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>startCDATA</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>230</YCoordinate>
            <Wcet>0</Wcet>
            <CName>cdata_start_finish</CName>
            <Function argument="9">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>finishCDATA</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>240</YCoordinate>
            <Wcet>0</Wcet>
            <CName>cdata_end_finish</CName>
            <Function argument="10">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>ignore_script</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>ignore_script</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>ignore_entities</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <CName>ignore_entities</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>allDone</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>260</YCoordinate>
            <Wcet>0</Wcet>
            <CName>finished</CName>
            <Function argument="11">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>streamEnded</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>275</YCoordinate>
            <CName>Stream_Finishe</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description> -----</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>275</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
