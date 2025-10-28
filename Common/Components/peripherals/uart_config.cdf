<?xml version="1.0" encoding="UTF-8"?><Component>
	<Module>usercomponents</Module>
	<CDFInfo>
		<Version>3</Version>
		<CreationDate>2023-04-18T13:32:37Z</CreationDate>
		<UpdatedDate>2024-05-23T15:43:53Z</UpdatedDate>
	</CDFInfo>
	<Description>
		<ShortDescription>uart_config</ShortDescription>
		<LongDescription>uart_config</LongDescription>
        <UserName/>
		<Menu>
			Peripherals
			<Menu>
				Serial
				<Menu>UART Config</Menu>
			</Menu>
		</Menu>
	</Description>
	<Block>
		<Type>IO</Type>
		<Width>95</Width>
		<Height>90</Height>
		<Text>UART conf</Text>
		<TextX>5</TextX>
		<TextY>5</TextY>
		<TextScale>1.5</TextScale>
		<TextVertical>0</TextVertical>
		<LocationX>0</LocationX>
		<LocationY>-15</LocationY>
	</Block>
	<FBID>
		<ERT1_ID>1</ERT1_ID>
		<Class>uart_config</Class>
	</FBID>
	<Hashes>
		<NameHash_CRC16>0xD172</NameHash_CRC16>
		<FbApiDescriptorHash_CRC32>331957e0</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
	</Hashes>
	<Parameters>
		<Parameter>
			<Name>Port</Name>
			<DataType>I</DataType>
			<DefaultValue>0</DefaultValue>
			<MinValue>0</MinValue>
			<MaxValue>100</MaxValue>
			<Description>UART port</Description>
			<ListPlacement>1</ListPlacement>
			<ArgPlacement>1</ArgPlacement>
			<DisplayOnWidget>
				<x>20</x>
				<y>20</y>
			</DisplayOnWidget>
		</Parameter>
		<Parameter>
			<Name>Baud</Name>
			<DataType>I</DataType>
			<DefaultValue>115200</DefaultValue>
			<MinValue>110</MinValue>
			<MaxValue>921600</MaxValue>
			<Description>Baudrate of the UART</Description>
			<ListPlacement>2</ListPlacement>
			<ArgPlacement>2</ArgPlacement>
		</Parameter>
		<Parameter>
			<Name>Data Length</Name>
			<DataType>I</DataType>
			<DefaultValue>8</DefaultValue>
			<MinValue>5</MinValue>
			<MaxValue>8</MaxValue>
			<Description>UART data length</Description>
			<ListPlacement>3</ListPlacement>
			<ArgPlacement>3</ArgPlacement>
		</Parameter>
		<Parameter>
			<Name>Parity</Name>
			<DataType>I</DataType>
			<DefaultValue>0</DefaultValue>
			<MinValue>0</MinValue>
			<MaxValue>2</MaxValue>
			<Description>Parity of UART. 0 for No parity, 1 for odd parity, 2 for even parity.</Description>
			<ListPlacement>4</ListPlacement>
			<ArgPlacement>4</ArgPlacement>
		</Parameter>
		<Parameter>
			<Name>Stop bits</Name>
			<DataType>I</DataType>
			<DefaultValue>0</DefaultValue>
			<MinValue>0</MinValue>
			<MaxValue>2</MaxValue>
			<Description>UART stop bit count. 0 for 1 bit, 1 for 1.5 bits, 2 for 2 bits.</Description>
			<ListPlacement>5</ListPlacement>
			<ArgPlacement>5</ArgPlacement>
		</Parameter>
		<Parameter>
			<Name>Hardware Control</Name>
			<DataType>B</DataType>
			<DefaultValue>0</DefaultValue>
			<MinValue>0</MinValue>
			<MaxValue>1</MaxValue>
			<Description>Whether to enable hardware control of UART</Description>
			<ListPlacement>6</ListPlacement>
			<ArgPlacement>6</ArgPlacement>
		</Parameter>
	</Parameters>
	<Functions>
		<Function>
			<name>enable</name>
			<ID>
				<ERT1_ID>1</ERT1_ID>
			</ID>
		</Function>
	</Functions>
	<Ports>
		<Port>
			<DataType>I</DataType>
			<Description>baudrate</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>20</YCoordinate>
			<CName>baudrate</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>stop bits</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>30</YCoordinate>
			<CName>stopbits</CName>
			<Function argument="2">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>parity</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>40</YCoordinate>
			<CName>parity</CName>
			<Function argument="3">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>--</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>90</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<Wcet>0</Wcet>
			<CName>finishenable</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>B</DataType>
			<Description>HW ctrl</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>60</YCoordinate>
			<CName>hw_ctrl</CName>
			<Function argument="4">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>set</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<CName>set</CName>
			<Function argument="0">
				<Function_ERT1_ID>1</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>errno</Description>
			<PortType>OutputPort</PortType>
			<XCoordinate>90</XCoordinate>
			<YCoordinate>20</YCoordinate>
			<CName>Errno</CName>
			<Function argument="1">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>data length</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>50</YCoordinate>
			<CName>length</CName>
			<Function argument="5">
				<Function_ERT1_ID>1</Function_ERT1_ID>
			</Function>
		</Port>
	</Ports>
</Component>
