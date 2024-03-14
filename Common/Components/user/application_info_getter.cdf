<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-04-18T13:32:37Z</CreationDate>
        <UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Gets information about locally installed apps.</ShortDescription>
        <LongDescription>Will retrieve the list of locally installed apps along with details of them such as name, description and icons.</LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>
                Local
                <Menu>Installed Apps</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>Data_Processor</Type>
        <Width>100</Width>
        <Height>270</Height>
        <Text>Installed Apps</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>0</ERT1_ID>
        <Class>application_info_getter</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x83EE</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>10bfd777</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters/>
    <Functions>
        <Function>
            <name>getList</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>getInfo</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>setDefault</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>deleteApp</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>getDefault</name>
            <ID>
                <ERT1_ID>5</ERT1_ID>
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
            <DataType>I</DataType>
            <Description>amount</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>30</YCoordinate>
            <CName>amount</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>list</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>20</YCoordinate>
            <CName>list</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>errors</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>240</YCoordinate>
            <CName>errors</CName>
            <Function argument="3">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>app info</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>app_info</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>app name[info]</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>app_name_info_</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>-----[list]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>10</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______list_</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error[list]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_list_</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>-----[info]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______info_</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>name</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>80</YCoordinate>
            <CName>name</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>commercial Name</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>90</YCoordinate>
            <CName>commercial_Name</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>version</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>100</YCoordinate>
            <CName>version</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>description</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>description</CName>
            <Function argument="5">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>info</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <CName>info</CName>
            <Function argument="6">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error[info]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>120</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_info_</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>get default</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <CName>get_default</CName>
            <Function argument="0">
                <Function_ERT1_ID>5</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>-----[getDefault]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>140</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______getDefault_</CName>
            <Function argument="1">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>app name[getDefault]</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>150</YCoordinate>
            <CName>app_name_getDefault_</CName>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error[getDefault]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_getDefault_</CName>
            <Function argument="2">
                <Function_ERT1_ID>5</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set default</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>180</YCoordinate>
            <CName>set_default</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>-----[setDefault]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>180</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______setDefault_</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error[setDefault]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>190</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_setDefault_</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>app name[setDefault]</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>190</YCoordinate>
            <CName>app_name_setDefault_</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>app name[delete]</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <CName>app_name_delete_</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>-----[delete]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>210</YCoordinate>
            <Wcet>0</Wcet>
            <CName>______delete_</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error[delete]</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>95</XCoordinate>
            <YCoordinate>220</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error_delete_</CName>
            <Function argument="2">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>uninstall</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>210</YCoordinate>
            <CName>uninstall</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>1</AtomicFlag>
            </Function>
        </Port>
    </Ports>
</Component>
