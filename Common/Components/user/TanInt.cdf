<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-10-11T15:00:00Z</CreationDate>
        <UpdatedDate>2024-10-11T15:00:44Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Tangent function with integer input</ShortDescription>
        <LongDescription>This component performs the tan function on the integer input and presents the result to the data output. Please note that the data input is in degrees or radians in a 1000th set by the Boolean parameter. The output is a thousand times the actual value.

The tangent of an angle is the ratio of the length of the opposite side to the length of the adjacent side, of a right angled triangle.

The mathematical operation is performed when the input event is triggered and the result of the mathematical operation is presented to the data output. The output event is triggered to signal the completion of the mathematical operation.</LongDescription>
        <UserName/>
        <Menu>
            Math
            <Menu>
                Functions
                <Menu>
                    trigonometry
                    <Menu>
                        tan (int)
                    </Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>30</Width>
        <Height>35</Height>
        <Text>tan</Text>
        <TextX>18</TextX>
        <TextY>20</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>1</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>0</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>TanInt</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x6551</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>b1befb32</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>unit</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>Select the unit of input. True for radians, False for degree.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>calc</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description/>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>calc</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description/>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>input</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>done</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description/>
            <PortType>OutputPort</PortType>
            <XCoordinate>25</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>output</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
