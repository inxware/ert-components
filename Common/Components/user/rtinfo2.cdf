<?xml version="1.0" encoding="UTF-8"?><Component>
	<Module>usercomponents</Module>
	<CDFInfo>
		<Version>3</Version>
		<CreationDate>2023-04-18T13:32:36Z</CreationDate>
		<UpdatedDate>2024-06-07T08:48:44Z</UpdatedDate>
	</CDFInfo>
	<Description>
        <ShortDescription/>
        <LongDescription>Device ID: MAC address, Network Address: IP addr, SODL Type: SODL format version, SODL name: Name of application,    </LongDescription>
        <UserName/>
		<Menu>
			System
			<Menu>
				Local
				<Menu>Runtime Info</Menu>
			</Menu>
		</Menu>
	</Description>
	<Block>
		<Type>IO</Type>
		<Width>100</Width>
		<Height>235</Height>
		<Text>Runtime Info</Text>
		<TextX>20</TextX>
		<TextY>5</TextY>
		<TextScale>1.25</TextScale>
		<TextVertical>0</TextVertical>
		<LocationX>0</LocationX>
		<LocationY>-15</LocationY>
	</Block>
	<FBID>
		<ERT1_ID>1</ERT1_ID>
		<Class>RuntimeInfo2</Class>
	</FBID>
	<Hashes>
		<NameHash_CRC16>0x2E49</NameHash_CRC16>
		<FbApiDescriptorHash_CRC32>7326c473</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
	</Hashes>
    <Parameters/>
	<Functions>
		<Function>
			<name>getstat</name>
			<ID>
				<ERT1_ID>1</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>getdyn</name>
			<ID>
				<ERT1_ID>2</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>reqpairid</name>
			<ID>
				<ERT1_ID>3</ERT1_ID>
			</ID>
		</Function>
	</Functions>
	<Ports>
		<Port>
			<Description>getStatic</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<CName>GetStatic</CName>
			<Function argument="0">
				<Function_ERT1_ID>1</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>miscApp name</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>20</YCoordinate>
			<CName>GetStatic</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
            <Description/>
			<PortType>FinishPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<Wcet>0</Wcet>
			<CName></CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>device ID</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>20</YCoordinate>
			<CName>Device_ID</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>RAM (kB)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>30</YCoordinate>
			<CName>RAM__KB__</CName>
			<Function argument="2">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>stor (kB)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>40</YCoordinate>
			<CName>Stor__KB_</CName>
			<Function argument="3">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>EHS version</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>50</YCoordinate>
			<CName>EHS_version</CName>
			<Function argument="4">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>moduleList</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>60</YCoordinate>
			<CName>ModuleList</CName>
			<Function argument="5">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>install dir</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>70</YCoordinate>
			<CName>Install_Dir</CName>
			<Function argument="6">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>getDynamic</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>85</YCoordinate>
			<CName>GetDynamic</CName>
			<Function argument="0">
				<Function_ERT1_ID>2</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
            <Description/>
			<PortType>FinishPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>85</YCoordinate>
			<Wcet>0</Wcet>
            <CName/>
			<Function argument="1">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>S</DataType>
			<Description>IP addr</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>95</YCoordinate>
			<CName>IP_Addr</CName>
			<Function argument="1">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>RAM avail kB</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>105</YCoordinate>
			<CName>RAM_Avail__KB_</CName>
			<Function argument="2">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>store avail kB</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>115</YCoordinate>
			<CName>Stor_Avail__KB_</CName>
			<Function argument="3">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>cpu (eRT)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>125</YCoordinate>
			<CName>CPU_eRT</CName>
			<Function argument="4">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>RAM (eRT)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>135</YCoordinate>
			<CName>RAM_eRT</CName>
			<Function argument="5">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>cpu (misc)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>145</YCoordinate>
			<CName>CPU</CName>
			<Function argument="6">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>RAM (misc)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>155</YCoordinate>
			<CName>RAM_Misc</CName>
			<Function argument="7">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>temp (cpu)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>165</YCoordinate>
			<CName>TEMP_CPU</CName>
			<Function argument="8">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>temp (op)</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>175</YCoordinate>
			<CName>TEMP_OP</CName>
			<Function argument="9">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>req. pair ID</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>200</YCoordinate>
			<CName>Req__Pair_ID</CName>
			<Function argument="0">
				<Function_ERT1_ID>3</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>pair ID</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>210</YCoordinate>
			<CName>Pair_ID</CName>
			<Function argument="10">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>pair ID</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>95</XCoordinate>
			<YCoordinate>200</YCoordinate>
			<Wcet>0</Wcet>
			<CName>Pair_ID</CName>
			<Function argument="2">
				<Function_ERT1_ID>2</Function_ERT1_ID>
			</Function>
		</Port>
	</Ports>
</Component>
