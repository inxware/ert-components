<?xml version="1.0" encoding="UTF-8"?><Component>
    <Module>usercomponents</Module>
    <CDFInfo>
        <Version>3</Version>
        <CreationDate>2024-12-30T11:19:08Z</CreationDate>
        <UpdatedDate>2026-03-26T00:00:00Z</UpdatedDate>
    </CDFInfo>
    <Description>
        <ShortDescription>Configure the ADC unit</ShortDescription>
        <LongDescription>Configure the ADC unit either from parameters or JSON string</LongDescription>
        <UserName/>
        <Menu>
            Peripherals
            <Menu>
                ADC
                <Menu>ADC Config</Menu>
            </Menu>
        </Menu>
    </Description>
    <Block>
        <Type>IO</Type>
        <Width>75</Width>
        <Height>85</Height>
        <Text>ADC Config</Text>
        <TextX>5</TextX>
        <TextY>5</TextY>
        <TextScale>1.25</TextScale>
        <TextVertical>0</TextVertical>
        <LocationX>0</LocationX>
        <LocationY>-15</LocationY>
    </Block>
    <FBID>
        <ERT1_ID>1</ERT1_ID>
        <Class>adc_config</Class>
    </FBID>
    <Hashes>
        <NameHash_CRC16>0x566F</NameHash_CRC16>
        <FbApiDescriptorHash_CRC32>e778f0d5</FbApiDescriptorHash_CRC32>
        <FbApiDescriptorHash/>
    </Hashes>
    <Parameters>
        <Parameter>
            <Name>device_id</Name>
            <DataType>I</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>9</MaxValue>
            <Description>The ADC Device number to be configured. For internal ADCs, this is the unit. For external ADCs, this is the address enumeration.</Description>
            <ListPlacement>1</ListPlacement>
            <ArgPlacement>1</ArgPlacement>
            <DisplayOnWidget>
                <x>30</x>
                <y>20</y>
            </DisplayOnWidget>
        </Parameter>
        <Parameter>
            <Name>f_s_hz</Name>
            <DataType>I</DataType>
            <DefaultValue>1000</DefaultValue>
            <MinValue>100</MinValue>
            <MaxValue>1000000000</MaxValue>
            <Description>Total sampling frequency in Hz. With n channels enabled, the sampling frequency of each channel is f_s_hz/n Hz.</Description>
            <ListPlacement>2</ListPlacement>
            <ArgPlacement>2</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>mode</Name>
            <DataType>B</DataType>
            <DefaultValue>0</DefaultValue>
            <MinValue>0</MinValue>
            <MaxValue>1</MaxValue>
            <Description>The ADC conversion mode. False for Single-shot. True for Continuous.</Description>
            <ListPlacement>3</ListPlacement>
            <ArgPlacement>3</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_0</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>4</ListPlacement>
            <ArgPlacement>4</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_1</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>5</ListPlacement>
            <ArgPlacement>5</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_2</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>6</ListPlacement>
            <ArgPlacement>6</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_3</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>7</ListPlacement>
            <ArgPlacement>7</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_4</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>8</ListPlacement>
            <ArgPlacement>8</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_5</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>9</ListPlacement>
            <ArgPlacement>9</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_6</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>10</ListPlacement>
            <ArgPlacement>10</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_7</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>11</ListPlacement>
            <ArgPlacement>11</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_8</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>12</ListPlacement>
            <ArgPlacement>12</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_9</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>13</ListPlacement>
            <ArgPlacement>13</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_10</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>14</ListPlacement>
            <ArgPlacement>14</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_11</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>15</ListPlacement>
            <ArgPlacement>15</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_12</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>16</ListPlacement>
            <ArgPlacement>16</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_13</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>17</ListPlacement>
            <ArgPlacement>17</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_14</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>18</ListPlacement>
            <ArgPlacement>18</ArgPlacement>
        </Parameter>
        <Parameter>
            <Name>channel_15</Name>
            <DataType>I</DataType>
            <DefaultValue>-1</DefaultValue>
            <MinValue>-1</MinValue>
            <MaxValue>999</MaxValue>
            <Description>Pin mapping for individual channels of this ADC instance/unit. For fixed-mapped ADC, any non-negative number means enabled channel.</Description>
            <ListPlacement>19</ListPlacement>
            <ArgPlacement>19</ArgPlacement>
        </Parameter>
    </Parameters>
    <Functions>
        <Function>
            <name>config</name>
            <ID>
                <ERT1_ID>1</ERT1_ID>
            </ID>
        </Function>
    </Functions>
    <Ports>
        <Port>
            <Description>init</Description>
            <PortType>StartPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <CName>init</CName>
            <Function argument="0">
                <Function_ERT1_ID>1</Function_ERT1_ID>
                <AtomicFlag>0</AtomicFlag>
            </Function>
        </Port>
        <Port>
            <DataType>S</DataType>
            <Description>JSON</Description>
            <PortType>InputPort</PortType>
            <XCoordinate>0</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <CName>JSON</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>--</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>12</YCoordinate>
            <Wcet>0</Wcet>
            <CName>finish</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <Description>error</Description>
            <PortType>FinishPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>22</YCoordinate>
            <Wcet>0</Wcet>
            <CName>error</CName>
            <Function argument="2">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
        <Port>
            <DataType>I</DataType>
            <Description>err id</Description>
            <PortType>OutputPort</PortType>
            <XCoordinate>70</XCoordinate>
            <YCoordinate>32</YCoordinate>
            <CName>error_id</CName>
            <Function argument="1">
                <Function_ERT1_ID>1</Function_ERT1_ID>
            </Function>
        </Port>
    </Ports>
</Component>
