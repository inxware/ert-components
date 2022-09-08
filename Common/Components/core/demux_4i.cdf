<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>DemultiplexFourOutputInt</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Switcher
      <Menu>Demux4
        <Menu>int</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>This is a 4-output integer demultiplexer.</LongDescription>
  <Port>
    <DataType>I</DataType>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>75</YCoordinate>
    <FunctionName argument="1">Run_Output1</FunctionName>
    <FunctionName argument="1">Run_Output2</FunctionName>
    <FunctionName argument="1">Run_Output3</FunctionName>
    <FunctionName argument="1">Run_Output4</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>o1</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>25</YCoordinate>
    <FunctionName argument="2">Run_Output1</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>o2</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="2">Run_Output2</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>o3</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>95</YCoordinate>
    <FunctionName argument="2">Run_Output3</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>o4</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>130</YCoordinate>
    <FunctionName argument="2">Run_Output4</FunctionName>
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
    <Description>s3</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="0">Run_Output3</FunctionName>
  </Port>
  <Port>
    <Description>s4</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>55</YCoordinate>
    <FunctionName argument="0">Run_Output4</FunctionName>
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
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="3">Run_Output3</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>115</YCoordinate>
    <FunctionName argument="3">Run_Output4</FunctionName>
  </Port>
</Component>
