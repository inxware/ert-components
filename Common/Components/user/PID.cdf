<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2023-12-28T16:37:23Z</CreationDate>
        <UpdatedDate>2024-02-01T11:49:19Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>PID controller</ShortDescription>
        <LongDescription>A PID controller with tailored specific configurations.</LongDescription>
        <UserName/>
        <Menu>
            System
            <Menu>PID Controller</Menu>
        </Menu>
    </Description>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>PID</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0xB237</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>4e2dba16</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>PIDNo</Name>
            <DataType>I</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>1</MinValue>
            <MaxValue>3</MaxValue>
            <Description>The channel number of this PID controller</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
            <DisplayOnWidget>
                <x>50</x>
                <y>20</y>
            </DisplayOnWidget>
        </Parameter>
        <Parameter>
            <Name>ISR Mode</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>If Non-ISR, the value will be updated manually by setting the "measured" port. If the mode is enabled, it will poll value automatically from the ADC set by "ChADC" port.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>P Default</Name>
            <DataType>F</DataType>
            <DefaultValue>1</DefaultValue>
            <MinValue>-100000</MinValue>
            <MaxValue>100000</MaxValue>
            <Description>Default value of P</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>I Default</Name>
            <DataType>F</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-100000</MinValue>
            <MaxValue>100000</MaxValue>
            <Description>Default value of I</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>D Default</Name>
            <DataType>F</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>-100000</MinValue>
            <MaxValue>100000</MaxValue>
            <Description>Default value of D</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>set_value</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>set_config</name>
            <ID>
                <ERT1_ID>2</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>control_cb</name>
            <ID>
                <ERT1_ID>3</ERT1_ID>
            </ID>
        </Function>
        <Function>
            <name>get_value</name>
            <ID>
                <ERT1_ID>4</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>set</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <CName>setValue</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>setpoint</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>27</YCoordinate>
            <CName>setpoint</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>measured</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>40</YCoordinate>
            <CName>measurement</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>set</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>60</YCoordinate>
            <CName>setConfig</CName>
            <Function argument="0">
                <Function_ERT1_ID>2</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>P</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>97</YCoordinate>
            <CName>configP</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>I</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>110</YCoordinate>
            <CName>configI</CName>
            <Function argument="2">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>F</DataType>
            <Description>D</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>122</YCoordinate>
            <CName>configD</CName>
            <Function argument="3">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ChADC</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>85</YCoordinate>
            <CName>ChADC</CName>
            <Function argument="4">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>EnRelayCheck</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>190</YCoordinate>
            <CName>EnRelayCheck</CName>
            <Function argument="5">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>max</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>202</YCoordinate>
            <CName>MaxGlobalValue</CName>
            <Function argument="6">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>15</YCoordinate>
            <Wcet>0</Wcet>
            <CName>valueSetOK</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>----</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>70</YCoordinate>
            <Wcet>0</Wcet>
            <CName>ConfigSetOK</CName>
            <Function argument="1">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>control</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>152</YCoordinate>
            <Wcet>0</Wcet>
            <CName>control</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>%OUT</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>165</YCoordinate>
            <CName>outInPercent</CName>
            <Function argument="1">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>trigger</Description>
            <PortType>InternalPort</PortType>
            <XCoordinate>-1</XCoordinate>
            <YCoordinate>-1</YCoordinate>
            <CName>trig_control_cb</CName>
            <Function argument="0">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>error</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>217</YCoordinate>
            <CName>errorValue</CName>
            <Function argument="2">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>PWM2 Out</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>190</YCoordinate>
            <CName>PWM2Out</CName>
            <Function argument="3">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>PWM1 Out</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>177</YCoordinate>
            <CName>PWM1Out</CName>
            <Function argument="4">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>PWM3 Out</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>202</YCoordinate>
            <CName>PWM3Out</CName>
            <Function argument="5">
                <Function_ERT1_ID>3</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ChOutput1</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>147</YCoordinate>
            <CName>ChGPIO1</CName>
            <Function argument="7">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ChOutput2</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>160</YCoordinate>
            <CName>ChGPIO2</CName>
            <Function argument="8">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ChOutput3</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>172</YCoordinate>
            <CName>ChGPIO3</CName>
            <Function argument="9">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>Output Mode</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>135</YCoordinate>
            <CName>OutputMode</CName>
            <Function argument="10">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>B</DataType>
            <Description>Enable</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>72</YCoordinate>
            <CName>EnableControl</CName>
            <Function argument="11">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>max I</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>214</YCoordinate>
            <CName>maxi</CName>
            <Function argument="12">
                <Function_ERT1_ID>2</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>get</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>232</YCoordinate>
            <CName>get_val</CName>
            <Function argument="0">
                <Function_ERT1_ID>4</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <Description>----</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>232</YCoordinate>
            <Wcet>0</Wcet>
            <CName>got_val</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>ADC</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>140</XCoordinate>
            <YCoordinate>245</YCoordinate>
            <CName>adc_value</CName>
            <Function argument="1">
                <Function_ERT1_ID>4</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
