<?xml version="1.0" encoding="UTF-8"?><Component>
	<Module>usercomponents</Module>
	<CDFInfo>
		<Version>3</Version>
		<CreationDate>2023-04-18T13:32:37Z</CreationDate>
		<UpdatedDate>2023-04-18T13:32:37Z</UpdatedDate>
	</CDFInfo>
	<Description>
        <ShortDescription/>
		<LongDescription>event counter providing option to set threshold with an input value.</LongDescription>
        <UserName/>
		<Menu>
			Deprecated
			<Menu>Event Counter Versatile</Menu>
		</Menu>
	</Description>
	<Block>
		<Type>Event_Processor</Type>
	</Block>
	<FBID>
		<ERT1_ID>0</ERT1_ID>
		<Class>EventCounterVersatile</Class>
	</FBID>
	<Hashes>
		<NameHash_CRC16>0x7203</NameHash_CRC16>
		<FbApiDescriptorHash_CRC32>401d3651</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
	</Hashes>
	<Parameters>
		<Parameter>
			<Name>Threshold</Name>
			<DataType>I</DataType>
			<DefaultValue>1000</DefaultValue>
			<MinValue>0</MinValue>
			<MaxValue>2147483647</MaxValue>
			<Description>Threshold value for ovf event.</Description>
			<ListPlacement>1</ListPlacement>
			<ArgPlacement>1</ArgPlacement>
		</Parameter>
	</Parameters>
	<Functions>
		<Function>
			<name>Run_StartEventCounter</name>
			<ID>
				<ERT1_ID>1</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Run_StopEventCounter</name>
			<ID>
				<ERT1_ID>2</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Run_ResetEventCounter</name>
			<ID>
				<ERT1_ID>3</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Run_CountEventCounter</name>
			<ID>
				<ERT1_ID>4</ERT1_ID>
			</ID>
		</Function>
		<Function>
			<name>Run_DecrementEventCounter</name>
			<ID>
				<ERT1_ID>5</ERT1_ID>
			</ID>
		</Function>
	</Functions>
	<Ports>
		<Port>
			<Description>start</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>10</YCoordinate>
			<CName>start</CName>
			<Function argument="0">
				<Function_ERT1_ID>1</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>stop</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>75</YCoordinate>
			<CName>stop</CName>
			<Function argument="0">
				<Function_ERT1_ID>2</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>rst</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>60</YCoordinate>
			<CName>rst</CName>
			<Function argument="0">
				<Function_ERT1_ID>3</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<Description>inc.</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>40</YCoordinate>
			<CName>Inc_</CName>
			<Function argument="0">
				<Function_ERT1_ID>4</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
			<Description>thresh</Description>
			<PortType>InputPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>25</YCoordinate>
			<CName>thresh</CName>
			<Function argument="1">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
			<Function argument="1">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<DataType>I</DataType>
            <Description/>
			<PortType>OutputPort</PortType>
			<XCoordinate>50</XCoordinate>
			<YCoordinate>60</YCoordinate>
            <CName/>
			<Function argument="1">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
			<Function argument="1">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
			<Function argument="1">
				<Function_ERT1_ID>3</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
            <Description/>
			<PortType>FinishPort</PortType>
			<XCoordinate>50</XCoordinate>
			<YCoordinate>40</YCoordinate>
			<Wcet>0</Wcet>
            <CName/>
			<Function argument="1">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
			<Function argument="1">
				<Function_ERT1_ID>3</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>ovf</Description>
			<PortType>FinishPort</PortType>
			<XCoordinate>50</XCoordinate>
			<YCoordinate>75</YCoordinate>
			<Wcet>0</Wcet>
			<CName>ovf</CName>
			<Function argument="2">
				<Function_ERT1_ID>4</Function_ERT1_ID>
			</Function>
			<Function argument="2">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
            <Description/>
			<PortType>FinishPort</PortType>
			<XCoordinate>50</XCoordinate>
			<YCoordinate>50</YCoordinate>
			<Wcet>0</Wcet>
            <CName/>
			<Function argument="1">
				<Function_ERT1_ID>5</Function_ERT1_ID>
			</Function>
			<Function argument="2">
				<Function_ERT1_ID>3</Function_ERT1_ID>
			</Function>
		</Port>
		<Port>
			<Description>dec</Description>
			<PortType>StartPort</PortType>
			<XCoordinate>0</XCoordinate>
			<YCoordinate>50</YCoordinate>
			<CName>Dec</CName>
			<Function argument="0">
				<Function_ERT1_ID>5</Function_ERT1_ID>
				<AtomicFlag>0</AtomicFlag>
			</Function>
		</Port>
	</Ports>
</Component>
