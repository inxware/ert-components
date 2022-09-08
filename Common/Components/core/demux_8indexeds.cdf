<?xml version="1.0" encoding="UTF-8"?>
<Component>
  <Class>IndexedDemultiplexer_String</Class>
  <Block>
    <Type>Data_Processor</Type>
  </Block>
  <Menu>Data Utilities
    <Menu>Switcher
      <Menu>Indexed
        <Menu>string</Menu>
      </Menu>
    </Menu>
  </Menu>
  <LongDescription>Distributes Data to Indexed Output</LongDescription>
  <Parameter>
    <Name>Index start</Name>
    <DataType>I</DataType>
    <DefaultValue>1</DefaultValue>
    <MinValue>-2147483648</MinValue>
    <MaxValue>2147483647</MaxValue>
    <Description>Value for firing port 1</Description>
    <ListPlacement>1</ListPlacement>
  </Parameter>
  <Port>
    <Description>Go</Description>
    <PortType>StartPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>10</YCoordinate>
    <AtomicFlag>1</AtomicFlag>
    <FunctionName argument="0">Run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Ind</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="2">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>20</YCoordinate>
    <FunctionName argument="12">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>1</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>30</YCoordinate>
    <FunctionName argument="3">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>Val</Description>
    <PortType>InputPort</PortType>
    <XCoordinate>0</XCoordinate>
    <YCoordinate>45</YCoordinate>
    <FunctionName argument="1">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>40</YCoordinate>
    <FunctionName argument="13">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>60</YCoordinate>
    <FunctionName argument="14">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>80</YCoordinate>
    <FunctionName argument="15">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>100</YCoordinate>
    <FunctionName argument="16">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>120</YCoordinate>
    <FunctionName argument="17">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>140</YCoordinate>
    <FunctionName argument="18">Run</FunctionName>
  </Port>
  <Port>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>160</YCoordinate>
    <FunctionName argument="19">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>2</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>50</YCoordinate>
    <FunctionName argument="4">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>3</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>70</YCoordinate>
    <FunctionName argument="5">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>4</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>90</YCoordinate>
    <FunctionName argument="6">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>5</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>110</YCoordinate>
    <FunctionName argument="7">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>6</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>130</YCoordinate>
    <FunctionName argument="8">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>7</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>150</YCoordinate>
    <FunctionName argument="9">Run</FunctionName>
  </Port>
  <Port>
    <DataType>S</DataType>
    <Description>8</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>170</YCoordinate>
    <FunctionName argument="10">Run</FunctionName>
  </Port>
  <Port>
    <Description>Overflow</Description>
    <PortType>FinishPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>190</YCoordinate>
    <FunctionName argument="20">Run</FunctionName>
  </Port>
  <Port>
    <DataType>I</DataType>
    <Description>Offset</Description>
    <PortType>OutputPort</PortType>
    <XCoordinate>50</XCoordinate>
    <YCoordinate>210</YCoordinate>
    <FunctionName argument="11">Run</FunctionName>
  </Port>
</Component>
