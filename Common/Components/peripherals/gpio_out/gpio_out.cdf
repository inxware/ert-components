<?xml version="1.0" encoding="UTF-8"?>
<Component>
    <Icon>gpio_out.bmp</Icon>
    <Class>gpio_out</Class>
    <Module>peripherals</Module>
    <Menu>Peripherals<Menu>GPIO<Menu>GPIO Out</Menu></Menu></Menu>
    <ShortDescription>GPIO Output</ShortDescription>
    <LongDescription>GPIO Output</LongDescription>
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
        <CName>write</CName>
        <Description>write</Description>
        <PortType>StartPort</PortType>
        <XCoordinate>5</XCoordinate>
        <YCoordinate>10</YCoordinate>
        <AtomicFlag>1</AtomicFlag>
        <FunctionName argument="0">write</FunctionName>
    </Port>
	<Port>
		<DataType>B</DataType>
        <CName>value</CName>
        <Description> </Description>
        <PortType>InputPort</PortType>
        <XCoordinate>5</XCoordinate>
        <YCoordinate>30</YCoordinate>
        <FunctionName argument="1">write</FunctionName>
    </Port>
</Component>
