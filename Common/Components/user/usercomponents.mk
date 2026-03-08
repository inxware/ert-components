#---------------------------------------------------------------
# Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
# You may use, distribute and modify this code under the terms 
# of the LGPLv3 license. You should have received a copy of the 
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If 
# not, please visit 
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------#

#Not sure why this is in user cmponents if this is theg global hash of all toolbox hashes.
EHS_TOOLBOX_HASHES:=$(EHS_TOOLBOX_HASHES)"0x583cfb49 "

VPATH+=: $(EHS_COMMON_COMPONENTS_PATH)/user
OBJECTS += usercomponents.$(OBJ)
OBJECTS += inx-ehs_controller.$(OBJ)

# We need to use the fllowing construct for all entries 
#ifdef EHS_XXXXXX_SUPPORT
#ifneq ($(EHS_XXXXX_SUPPORT),none)
# ...
#endif
#endif

# @TODO - These components moved to networking folder.
# EHS_NETWORKING_SUPPORT or EHS_COMPONENTS_NETWORK_TCPIP_SOCKET
# Socket components moved to networking/ directory

# @TODO - move to PERIPHERALS or other toolbox
ifeq ($(EHS_PERIPHERALS_BACKLIGHT_SUPPORT),)
	EHS_PERIPHERALS_BACKLIGHT_SUPPORT=stubbed
endif
ifdef EHS_PERIPHERALS_BACKLIGHT_SUPPORT
# backlight is using stub if not implementd for target, so should be safe to just define it
DEFS += EHS_PERIPHERALS_BACKLIGHT_SUPPORT=1
endif

include $(EHS_COMMON_HAL_PATH)/json/json.mk

ifdef EHS_NXP_SUPPORT
OBJECTS += inx-rtc.$(OBJ)
# inx-pwm moved to peripherals/ directory
OBJECTS += inx-numeric_display_char.$(OBJ)
OBJECTS += inx-permanent_storage.$(OBJ)
endif

ifdef EHS_LORAWAN_SUPPORT
ifneq ($(EHS_LORAWAN_SUPPORT),none)
DEFS += EHS_LORAWAN_SUPPORT
OBJECTS += inx-lorawan.$(OBJ)
endif
endif

ifdef EHS_AUDIO_INPUT_LEVEL_SUPPORT
ifneq ($(EHS_AUDIO_INPUT_LEVEL_SUPPORT),none)
DEFS += EHS_AUDIO_INPUT_LEVEL_SUPPORT
OBJECTS += inx-audio_input_level.$(OBJ)
endif
endif

#TODO2025 These need to be moved to the Core toolbox

OBJECTS += inx-hex2string.$(OBJ)
OBJECTS += inx-Int2HexString.$(OBJ)

ifdef EHS_SCHEDULER_SUPPORT
ifneq ($(EHS_SCHEDULER_SUPPORT),none)
OBJECTS += inx-scheduler.$(OBJ)
DEFS += EHS_SCHEDULER_SUPPORT
endif
endif

ifdef EHS_PID_SUPPORT
ifneq ($(EHS_PID_SUPPORT),none)
DEFS += EHS_PID_SUPPORT
OBJECTS += inx-PID.$(OBJ)
OBJECTS += inx-calibrate.$(OBJ)
OBJECTS += inx-pid_relay_config.$(OBJ)
OBJECTS += inx-pid_hdwr_config_int.$(OBJ)
endif
endif



# Manage the component parts of OTA support (not the HAL)
# Presumable thhis is the OTA writer component, though the name doesn't suggest this.
ifdef EHS_OTA_SUPPORT
	ifneq ($(EHS_OTA_SUPPORT),none)
	        OBJECTS += inx-ota.$(OBJ)
		DEFS+=EHS_OTA_SUPPORT
	else 
# Temporarrilly doing this to make builds work
	    EHS_OTA_SUPPORT=stubbed
	endif
endif

# We always provide the OTA parser
OBJECTS += inx-ota_data_parser.$(OBJ)


OBJECTS += inx-reboot.$(OBJ)
# inx-cgi2json moved to core/ directory
OBJECTS += inx-stringdivader.$(OBJ)

#TODO Change this to stub version later
ifdef EHS_PERIPHERALS_BACKLIGHT_SUPPORT
OBJECTS += inx-display_backlight.$(OBJ)
endif

# TODO move these to the core directory
OBJECTS += inx-num_demux.$(OBJ)
OBJECTS += inx-num_mux.$(OBJ)
OBJECTS += inx-rtinfo2.$(OBJ)
OBJECTS += inx-map_int.$(OBJ)
# Indexed mux/demux components moved to core/ directory
OBJECTS += inx-binary2decimal8.$(OBJ)

OBJECTS += inx-sample2str.$(OBJ)
OBJECTS += inx-key_value.$(OBJ)

OBJECTS += inx-unsigned2int.$(OBJ)
OBJECTS += inx-SineInt.$(OBJ)
OBJECTS += inx-CosineInt.$(OBJ)
OBJECTS += inx-TanInt.$(OBJ)

OBJECTS += inx-key_value_int.$(OBJ)
OBJECTS += inx-key_value_real.$(OBJ)
OBJECTS += inx-key_value_bool.$(OBJ)

# ADC configuration components moved to peripherals/ directory
# move this to the core directory.
ifdef EHS_COMPONENTS_CONSOLE_IO
	OBJECTS += inx-console_print.$(OBJ)
	DEFS += EHS_COMPONENTS_CONSOLE_IO
endif

# PWM support moved to peripherals/ directory

ifndef EHS_EXCLUDE_XML_PARSER
# inx-xml_stream moved to core/ directory
#@todo this is for the xml stream parser - should be out somewhere more sensible..
#ifeq ($(EHS_GNU_OS),$(filter $(EHS_GNU_OS),mingw32msvc w64-mingw32))
	ifeq ($(EHS_GNU_OS),mingw32posix)
		LIB+= expat
	else
    	ifeq ($(SYSTEM_VARIANT),RASPBERRYPI)
    		LIB+= expatw
      	else
        	ifdef EHS_ANDROID
           		LIB+=:libexpat.a
        	else
           		LIB+= expat
        	endif
    	endif
    endif
else
   DEFS += EHS_EXCLUDE_XML_PARSER=1
endif

#TODO2025 all rhis needs tidying up. If this is for expat put it where expat is a dependecy
ifndef EHS_SKIP_GNULIBRARIES
	# yajl=based inx-json_stream moved to core/ directory???
	OBJECTS += inx-JSONObjectFunctionBlock.$(OBJ)
	#This prolly needs the XML parser
	OBJECTS += inx-application_run.$(OBJ)#

	ifndef EHS_SKIP_APPLICATION_INFO_GETTER
	ifndef EHS_NO_LIBXML2_SUPPORT
		OBJECTS += inx-application_info_getter.$(OBJ)
	endif
	endif
endif

