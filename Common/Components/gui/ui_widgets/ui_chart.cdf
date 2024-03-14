<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2024-02-15T06:15:41Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>User interface plot chart widget</LongDescription>
        <UserName/>
        <Menu>
            User Interface
            <Menu>
                Graphics
                <Menu>
                    Advanced
                    <Menu>Chart</Menu>
                </Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Text>Chart</Text>
        <TextX>48</TextX>
        <TextY>160</TextY>
        <TextScale>1.8</TextScale>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>ui_chart</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x839F</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>b99d231</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Widget Tag</Name>
            <DataType>WT</DataType>
            <DefaultValue>widget</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>widget tag</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Widget-Group</Name>
            <DataType>ST</DataType>
            <DefaultValue>default</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>widget group</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Title</Name>
            <DataType>S</DataType>
            <DefaultValue>Chart</DefaultValue>
            <MinValue/>
            <MaxValue/>
            <Description/>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Data1 Size</Name>
            <DataType>I</DataType>
            <DefaultValue>10</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1000</MaxValue>
            <Description>Size of data1</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Data2 Size</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1000</MaxValue>
            <Description>Size of data2</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>X Max</Name>
            <DataType>I</DataType>
            <DefaultValue>10</DefaultValue>
            <MinValue>-65536</MinValue>
            <MaxValue>65536</MaxValue>
            <Description>Maximum x-axis range value</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>X Min</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-65536</MinValue>
            <MaxValue>65536</MaxValue>
            <Description>Minimum x-axis range value</Description>
            <ListPlacement>7</ListPlacement>
            <ArgPlacement>7</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Y Max</Name>
            <DataType>I</DataType>
            <DefaultValue>100</DefaultValue>
            <MinValue>-65536</MinValue>
            <MaxValue>65536</MaxValue>
            <Description>Maximum y-axis range value</Description>
            <ListPlacement>8</ListPlacement>
            <ArgPlacement>8</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Y Min</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-65536</MinValue>
            <MaxValue>65536</MaxValue>
            <Description>Minimum y-axis range value</Description>
            <ListPlacement>9</ListPlacement>
            <ArgPlacement>9</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>create</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>destroy</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>show</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>hide</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>update</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>data</name>
            <ID>
                <ERT1_ID>6</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>create</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <MandatoryFlag>1</MandatoryFlag>
            <CName>create</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>destroy</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>destroy</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>show</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <CName>show</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>hide</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>hide</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>update</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>update</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>off.x</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>72</YCoordinate>
            <CName>off_x</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>off.y</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>82</YCoordinate>
            <CName>off_y</CName>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>off.wid</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>92</YCoordinate>
            <CName>off_wid</CName>
            <Function argument="3">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>off.ht</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>102</YCoordinate>
            <CName>off_ht</CName>
            <Function argument="4">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>alpha</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>112</YCoordinate>
            <CName>alpha</CName>
            <Function argument="5">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>abs x</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>77</YCoordinate>
            <CName>abs_x</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>abs y</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>87</YCoordinate>
            <CName>abs_y</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>wid</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>97</YCoordinate>
            <CName>wid</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="3">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ht</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>107</YCoordinate>
            <CName>ht</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="4">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>35</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>62</YCoordinate>
            <Wcet>0</Wcet>
            <CName>__</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>data</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>127</YCoordinate>
            <CName>data</CName>
            <Function argument="0">
                <Function_ERT1_ID>6</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>100</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <Wcet>0</Wcet>
            <CName>___</CName>
            <Function argument="1">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>data1</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>data1</CName>
            <Function argument="1">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>data2</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>data2</CName>
            <Function argument="2">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>data1</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>162</YCoordinate>
            <CName>data1_sample</CName>
            <Function argument="3">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>data2</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>172</YCoordinate>
            <CName>data2_sample</CName>
            <Function argument="4">
                <Function_ERT1_ID>6</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
