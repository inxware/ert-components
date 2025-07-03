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

######################################################################################################
# EnableGpio <gpio number> [<direction> [<initial value>]]
#  Description:
#   Enable the GPIO with given direction and initial output value. Will skip if it's already enabled
#  Usage:
#   <gpio number>    The GPIO number to be enabled
#   <direction>      (Optional) The direction of GPIO. "in" or "out". Default to "in".
#   <initial value>  (Optional) The initial output of the enabled GPIO. "1" = high, "0" = low
#                        Not required for "in" direction.
#                        Required for "out" direction. When not specified, direction is set to "in"
######################################################################################################
EnableGpio(){
    gpio=$1         # GPIO number
    # If the GPIO has not already been exported
    if [ ! -d "$GPIO_PATH/gpio$gpio" ] ; then
        direction="in"  # 'out' or 'in'. Default to "in" 
        value_set=0
        # If more than GPIO number argument, then set the direction
        if [ "$#" -ge 2 ]; then
            value_set=1
            # If the GPIO is set to output
            if [ "$2" = "out" ]; then
                # When no initial value specified, force direction to "in"
                if [ "$#" -eq 2 ]; then
                    direction="in"  # Uncertain GPIO output set as input
                else
                    # Set default output value according to the initial value
                    if [ $3 -eq 0 ]; then
                        direction="low"
                    else
                        direction="high"
                    fi
                fi
            else
                direction="in"
            fi
        fi

        # Export GPIO
        ExportGpio "$gpio"

        # Give essential file read/writing permissions
        chmod 777 "$GPIO_PATH/gpio$gpio"
        chmod 777 "$GPIO_PATH/gpio$gpio/value"
        chmod 777 "$GPIO_PATH/gpio$gpio/direction"

        # If set direction
        if [ $value_set -eq 1 ]; then
            WriteGpioDirection "$gpio" "$direction"
        fi
    fi
}

#Remove the 2nd and 3rd arguments for all these as above
# [MSG-200 GPIO Mapping](https://docs.google.com/spreadsheets/d/1ydhp-P5zUvuCvrHoMTm6WUHOT1Xp-Ul_pFZZ6QjWrmQ/edit#gid=0)
# [MSG-300 GPIO Mapping](https://docs.google.com/spreadsheets/d/18Tzi2PqAi2eVs6dxil2n_gX5qnGeKkVGzroJZ25hMbQ/edit#gid=0)
# override
SetupDeviceGpio(){
    SupervisorLog "GPIO setup [Start]"
    # GPIO ID         # Pi2 bus

    
    if [ -e "/storage/emulated/0/Android/data/com.inx.ehs/files/userdata/msg300.nfo" ] ; then
        # MSG-300 GPIO
        ## Reset lines
        EnableGpio "226" # CH7 reset CH1 (re-exported)
        EnableGpio "227" # CH7 reset CH2 (re-exported)
        EnableGpio "229" # CH7 reset CH3 (re-exported)
        EnableGpio "230" # CH7 reset CH4
        EnableGpio "232" # CH7 reset CH5
        EnableGpio "233" # CH7 reset CH6, CH6 reset CH7

        ## Mute Button input
        EnableGpio "33" "in"

        ## LED Status
        EnableGpio "32" # Red Mute
        EnableGpio "35" # Green Active
        EnableGpio "34" "out" "0" # Blue booting LED turned on with startup
    else
        # MSG-200 GPIO
        ## Node Channel
        EnableGpio "200"
        EnableGpio "201"
        EnableGpio "202"
        EnableGpio "32" # Red Mute
        EnableGpio "33"
        EnableGpio "35" # Green Active
        EnableGpio "34" "out" "1" # Blue booting LED turned on with startup
    fi

    ## Supervisor Channel (Needed for this target? This should be inclusive to the Armbian target only)
    # EnableGpio "224"
    # EnableGpio "225"
    # EnableGpio "226"
    # EnableGpio "227"
    # EnableGpio "228"
    # EnableGpio "229"

    # EnableGpio "192" # "out" "1" # high - reset lines
    # EnableGpio "193" # "out" "1" # high - reset lines
    # EnableGpio "194" # "out" "1" # high - reset lines
    # EnableGpio "195" # "out" "1" # high - reset lines
    # EnableGpio "196" # "out" "1" # high - reset lines
    # EnableGpio "197" # "out" "1" # high - reset lines

    

    # Other unintallised (Why enabling those?)
    # EnableGpio "362"  # 7 - used by the OS
    # EnableGpio "72"     # 12
    # EnableGpio "233"    # 13
    # EnableGpio "76"     # 15
    # EnableGpio "77"     # 16
    # EnableGpio "78"     # 18
    # EnableGpio "79"     # 22

    SupervisorLog "GPIO setup [End]"
}

