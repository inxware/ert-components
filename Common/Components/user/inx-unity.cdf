<?xml version="1.0" encoding="UTF-8"?>
<Component>
<Icon>inx-unity.bmp</Icon>
<Class>unity</Class>
<Module>usercomponents</Module>
<Menu>System<Menu>Local<Menu>Runtime Unity</Menu></Menu></Menu>
<ShortDescription>System operations to control Unity </ShortDescription>
<LongDescription>Controls the current Unity window</LongDescription>
<Port>
<CName/>
<Description>write</Description>
<PortType>StartPort</PortType>
<XCoordinate>0</XCoordinate>
<YCoordinate>10</YCoordinate>
<AtomicFlag>1</AtomicFlag>
<FunctionName argument="0">write</FunctionName>
</Port>
<Port>
<CName/>
<Description>-----[write]</Description>
<PortType>FinishPort</PortType><XCoordinate>85</XCoordinate><YCoordinate>10</YCoordinate><Wcet>0</Wcet><FunctionName argument="1">write</FunctionName></Port><Port><DataType>I</DataType><CName/><Description>Number</Description><PortType>InputPort</PortType><XCoordinate>0</XCoordinate><YCoordinate>20</YCoordinate>
<FunctionName argument="1">write</FunctionName>
</Port>
</Component>