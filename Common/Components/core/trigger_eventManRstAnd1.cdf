<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>EventOneInputManRstAnd</Class>
  <Menu>Event
    <Menu>Combiners/Flow
      <Menu>Manual AND
        <Menu>AND1</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 1-input event AND with an external manual reset. It is asserted once and will not fire again until manually reset.</LongDescription>
  <Port>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <MandatoryFlag>1</MandatoryFlag>
    <FunctionName argument="0">And1</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="1">And1</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>15</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="1">Reset</FunctionName>
  </Port>
  <Port>
    <Description>R</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="0">Reset</FunctionName>
  </Port>
</Component>
