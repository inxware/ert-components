<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-08-14T10:46:06Z</CreationDate>
        <UpdatedDate>2025-06-12T17:40:50Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Machine learning Tensorflow Lite model from image frame.</ShortDescription>
        <LongDescription>Pass image frame to machine learning Tensorflow Lite model. Supports yolov5 ...</LongDescription>
        <UserName/>
        <Menu>
            Machine Learning
            <Menu>Tensorflow Lite from frame</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Height>125</Height>
        <Text>ML TFLite</Text>
        <TextX>25</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>TF_Lite_from_frame</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xFB15</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>93145718</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Model Type</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>99999999</MaxValue>
            <Description>Model Type</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Conf Thres</Name>
            <DataType>F</DataType>
            <DefaultValue>0.5</DefaultValue>
            <MinValue>0.0</MinValue>
            <MaxValue>1.0</MaxValue>
            <Description>Confidence Threshold</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Thread Count</Name>
            <DataType>I</DataType>
            <DefaultValue>2</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>20</MaxValue>
            <Description>Number of model processing  threads</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Flat JSON</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Use a flat JSON output format</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use Application Dir</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Load model in Application Directory rather than the user data directory</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>load_model</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>do_inference</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>load</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>load_model</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>model</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>model_file_path</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>load_ok</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <Wcet>0</Wcet>
            <CName>load_error</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errno</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>32</YCoordinate>
            <CName>load_errno</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>42</YCoordinate>
            <CName>model_info</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>do</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>57</YCoordinate>
            <CName>do_inference</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done_inference</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>json</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>output</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>stream_id</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>67</YCoordinate>
            <CName>frame_id</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <Wcet>0</Wcet>
            <CName>inference_error</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errno</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>inference_errno</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        
    </Ports>
</Component>
