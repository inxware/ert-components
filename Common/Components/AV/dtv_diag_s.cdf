<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DtvDiagnosticString</Class>
  <Block>
    <Type>IO</Type>
  </Block>
  <Menu>System
    <Menu>Host OS
      <Menu>Write STDIO</Menu>
    </Menu>
  </Menu>
  <LongDescription>Writes the input string to the stdout context that EHS is running in</LongDescription>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">run</FunctionName>
  </Port>
</Component>
