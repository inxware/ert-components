<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2025-10-28T16:46:40Z</CreationDate>
        <UpdatedDate>2025-10-29T09:57:03Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Machine Learning Inference for Images</ShortDescription>
        <LongDescription>Inference of data input based on loaded model with vaiants like yolov8-seg. Hardware Acceleration is available for supported targets.</LongDescription>
        <UserName/>
        <Menu>
            Machine Learning
            <Menu>Image Inference</Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data</Type>
        <Height>125</Height>
        <Text>IMG Infer</Text>
        <TextX>25</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>ml_image_inference</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xF512</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>9f224da7</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Model Type</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>4001</MaxValue>
            <Description>The type of model loaded. Used to select the post-processing pipeline for the inference output. 0 = auto/generic (raw inference run, no model-specific decode; output will be empty). Non-zero = filter: only this model type is accepted (e.g. 1003 for YOLOv5, 1006 for YOLOv8).</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Model Format</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>4</MaxValue>
            <Description>The format of the loaded model file. 0 = auto-detect from file extension. 1 = TFLite (`.tflite` / `.tfl`), 2 = ONNX (`.onnx` / `.onn`), 3 = TensorFlow (`.pb`), 4 = Hailo (`.hef`). Non-zero acts as a filter: load fails if the file extension does not match.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Data Type</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>4</MaxValue>
            <Description>The Data type of model input/ouput. 0 for auto-detection, 1 for 8-bit, 2 for 16-bit, 3 for 32-bit, 4 for 64-bit</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>HW Accelerate</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Whether to enable the hardware acceleration. Some platforms might not support it and error will occur when this is True. Some platforms require a specific Model file format, where this block would emit error if wrong model file format is loaded while this is True.</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Conf Thres</Name>
            <DataType>F</DataType>
            <DefaultValue>0.5</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>The confidence Threshold for the output data. The range is from 0 to 1.</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Thread Number</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>16</MaxValue>
            <Description>The number of threads for inference. If the platform does not support threading, this parameter will be ignored.</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Use Application Dir</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Whether to use application directory as the root of model path. If not, the User directory will be used.</Description>
            <ListPlacement>7</ListPlacement>
            <ArgPlacement>7</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>JSON Format</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Output JSON structure. 0 = object-based (default): each detection is an object in a "res" array, e.g. {"det_cnt":N,"res":[{"cls":0,"cnf":0.92,"x":...},...]}. 1 = flat: all detection fields at the top level with numeric suffixes, e.g. {"det_cnt":N,"cls0":0,"cnf0":0.92,"x0":...}.</Description>
            <ListPlacement>8</ListPlacement>
            <ArgPlacement>8</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Coord Format</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Bounding box coordinate format in JSON output. 0 = model-input pixel values (default): x, y, w, h are in pixels relative to the model input dimensions (e.g. 640x640). 1 = normalised 0-1: coordinates are divided by the model input width/height, giving values in the range [0, 1] regardless of model input size.</Description>
            <ListPlacement>9</ListPlacement>
            <ArgPlacement>9</ArgPlacement>
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
            <name>inference</name>
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
            <YCoordinate>15</YCoordinate>
            <CName>load</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>path</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <CName>model_path</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>load_done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>25</YCoordinate>
            <Wcet>0</Wcet>
            <CName>load_err</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>errno</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>load_errno</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>model info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>model_info</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>do</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>inference</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <Wcet>0</Wcet>
            <CName>inference_done</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>stream_id</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>stream_id</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>err</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>65</YCoordinate>
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
            <XCoordinate>85</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>inference_errno</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>json</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>85</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>json</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
