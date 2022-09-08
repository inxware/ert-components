#!/system/bin/sh

source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"

GPIO_PATH="/sys/class/gpio"

ExportGpio(){
    gpio=$1
    echo "$gpio" > "$GPIO_PATH/export"
}

WriteGpioDirection(){
    gpio=$1
    direction=$2
    if [ -n "$direction" ]; then
        echo "$direction" > "$GPIO_PATH/gpio$gpio/direction"

        # @TODO - implement input handler in set as 'in'
        #chmod 777 "$GPIO_PATH/gpio$gpio/active_low"
        #chmod 777 "$GPIO_PATH/gpio$gpio/uevent"
    fi
}

WriteGpioValue(){
    gpio=$1
    value=$2
    if [ -n "$value" ]; then
        echo "$value" > "$GPIO_PATH/gpio$gpio/value"
    fi
}

EnableGpio(){
    gpio=$1         # number
    direction=$2    # 'out' or 'in' 
    value=$3        # high='1' or low='0'
    ExportGpio "$gpio"
    chmod 777 "$GPIO_PATH/gpio$gpio"
    chmod 777 "$GPIO_PATH/gpio$gpio/value"
    chmod 777 "$GPIO_PATH/gpio$gpio/direction"

    WriteGpioDirection "$gpio" "$direction"
    WriteGpioValue "$gpio" "$value"   
}

# override
SetupDeviceGpio(){
    SupervisorLog "GPIO setup [Start]"
    # GPIO ID         # Pi2 bus

    # Node Channel
    EnableGpio "32" "out" "0" # low
    EnableGpio "33" "out" "0" # low
    EnableGpio "34" "out" "0" # low
    EnableGpio "35" "out" "1" # high
    EnableGpio "200" "out" "0" # low
    EnableGpio "201" "out" "0" # low
    EnableGpio "202" "out" "0" # low

    # Supervisor Channel
    EnableGpio "224" "out" "0" # low
    EnableGpio "227" "out" "0" # low
    EnableGpio "225" "out" "0" # low
    EnableGpio "226" "out" "0" # low
    EnableGpio "194" "out" "1" # high
    EnableGpio "228" "out" "0" # low
    EnableGpio "229" "out" "0" # low
    EnableGpio "192" "out" "1" # high
    EnableGpio "196" "out" "1" # high
    EnableGpio "193" "out" "1" # high
    EnableGpio "195" "out" "1" # high
    EnableGpio "197" "out" "1" # high

    # Other unintallised
    # EnableGpio "362"  # 7 - used by the OS
    EnableGpio "72"     # 12
    EnableGpio "233"    # 13
    EnableGpio "76"     # 15
    EnableGpio "77"     # 16
    EnableGpio "78"     # 18
    EnableGpio "79"     # 22

    SupervisorLog "GPIO setup [End]"
}

