<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:36Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:36Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription/>
        <LongDescription>Provides the means to download, install &amp; run applications from the application server.    </LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>
                Remote
                <Menu>Application Server</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>65</Width>
        <Height>200</Height>
        <Text>App Server</Text>
        <TextX>2</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>AppGet</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x78CF</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>421345b7</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>Server</Name>
            <DataType>S</DataType>
            <DefaultValue>devman.inx-systems.com</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>0</MaxValue>
            <Description>Base URL of devman server hosting applications.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Install and Run</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Run the application after it has been installed.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Install as Default</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <Description>Set the application as the Default App when it is installed.</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>Install Mode</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>2</MaxValue>
            <Description>Install Mode, 0 install as standard App in canonical directory, 1 install as Home App, 2 install as Temp App.</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>listremote</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>getinfo</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>getinstall</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>get list</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <CName>get_list</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>keywords</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>keywords</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>server</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <CName>server</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>list</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>list</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>errors</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>175</YCoordinate>
            <CName>errors</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error </Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>install</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <CName>install</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <CName>name</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>run</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>65</YCoordinate>
            <CName>run</CName>
            <Function argument="3">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>default</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>75</YCoordinate>
            <CName>default</CName>
            <Function argument="4">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>mode</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>mode</CName>
            <Function argument="5">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>45</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>55</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>info</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>info</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>name</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>info</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <CName>name</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>cmrcl name</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>130</YCoordinate>
            <CName>cmrcl_name</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>version</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>version</CName>
            <Function argument="5">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>descr</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>descr</CName>
            <Function argument="6">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description/>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <Wcet>0</Wcet>
            <CName/>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>60</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
