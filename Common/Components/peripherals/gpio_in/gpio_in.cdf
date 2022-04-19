<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>gpio_in.bmp</Icon>
    <Class>gpio_in</Class>
    <Module>peripherals</Module>
    <Menu>Peripherals<Menu>GPIO<Menu>GPIO In</Menu></Menu></Menu>
    <ShortDescription>GPIO Input</ShortDescription>
    <LongDescription>GPIO Input</LongDescription>
	<Parameter>
		<Name>Pin ID</Name>
		<DataType>I</DataType>
		<MinValue>0</MinValue>
		<MaxValue>32767</MaxValue>
		<Description>Pin ID</Description>
		<ListPlacement>1</ListPlacement>
		<ArgPlacement>1</ArgPlacement>
	</Parameter>
	<Port>
        <CName>read</CName>
        <Description>read</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>5</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">read</FunctionName>
    </Port>
	<Port>
        <CName>finishevent</CName>
        <Description> </Description>
        <PortType>FinishPort</PortType>
        <XCoordinate>43</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <Wcet>0</Wcet>
        <FunctionName argument="1">read</FunctionName>
    </Port>
	<Port>
		<DataType>B</DataType>
        <CName>value</CName>
        <Description> </Description>
        <PortType>OutputPort</PortType>
        <XCoordinate>43</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">read</FunctionName>
    </Port>
</Component>
