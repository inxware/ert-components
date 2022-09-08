<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DemultiplexTwoOutputBool</Class>
  <Block>
    <Type>Data_Processor</Type>
    <Text>DEMUX</Text>
    <TextX>7</TextX>
    <TextY>40</TextY>
    <TextScale>1.0</TextScale>
  </Block>
  <Menu>Data Utilities
    <Menu>Switcher
      <Menu>Demux2
        <Menu>bool</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 2-output boolean demultiplexer.</LongDescription>
  <Port>
    <DataType>B</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="1">Run_Output1</FunctionName>
    <FunctionName argument="1">Run_Output2</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>o1</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="2">Run_Output1</FunctionName>
  </Port>
  <Port>
    <DataType>B</DataType>
    <Description>o2</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="2">Run_Output2</FunctionName>
  </Port>
  <Port>
    <Description>s1</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="0">Run_Output1</FunctionName>
  </Port>
  <Port>
    <Description>s2</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="0">Run_Output2</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <FunctionName argument="3">Run_Output1</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="3">Run_Output2</FunctionName>
  </Port>
</Component>
