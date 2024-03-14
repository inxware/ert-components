<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2024-01-15T10:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>CSets and Retrieves the current system time.</LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>
                Local
                <Menu>Date &amp; Time</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Event_Processor</Type>
        <Height>195</Height>
        <Text>Clock</Text>
        <TextX>15</TextX>
        <TextY>5</TextY>
        <TextScale>1.5</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>wall_clock</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xED30</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>76aa654f</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>format</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>3</MaxValue>
            <Description>Date output format</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
            <DisplayOnWidget>
                <x>20</x>
                <y>20</y>
            </DisplayOnWidget>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>get</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>set</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>get</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>get</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>Date String</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>Date_String</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>------</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>set</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>time</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>32</YCoordinate>
            <CName>time</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>-----</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>_____</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Mins 1970</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>90</YCoordinate>
            <CName>Mins_1970</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>&amp; secs</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>__secs</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>unix time</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>unix_time</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>timezone</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>42</YCoordinate>
            <CName>timezone</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>unix time</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <CName>unix_time_out</CName>
            <Function argument="4">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Year</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>Year</CName>
            <Function argument="5">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>MDay</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <CName>MDay</CName>
            <Function argument="6">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>WDay</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>WDay</CName>
            <Function argument="7">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Hour</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>Hour</CName>
            <Function argument="8">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Minute</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <CName>Minute</CName>
            <Function argument="9">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Second</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>170</YCoordinate>
            <CName>Second</CName>
            <Function argument="10">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Month</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>Month</CName>
            <Function argument="11">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
