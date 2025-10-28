

# @file config.mk 
# inxware ERT configuration file for XXXXXXXXXXXXXXXXXXXXXXXXX
# @author: XXXXXXXXXXXXXXXXXXXXXXXx

#################################################################################################################
# Set general architecture and OS version 
#################################################################################################################
EHS_ARCH=amd64
EHS_OS=linux
# TOOLCHAIN_NAME is an optional alternative location to find the toolchain. 
# Toolchain path defaults ../ert-build-support/<BUILD HOST TYPE>/$EHS_ 
TOOLCHAIN_NAME=HOST


# SYSTEM_VARIANT optionally indicates specific target environment confgurations.
# See target/envbuildscripts/targetenv_hacks_*.sh scripts).  
# SYSTEM_VARIANT=


################################################################################################################
# Configure debug/production levels
################################################################################################################
# Set ALL debug use this:
EHS_DEBUGALL=yes
# Or use one of the more fine-grained debug congurations
# Or enable only stdout & serial console logging
#DEFS += EHS_RUNTIME_LOGGER_ENABLED
#enable TCPIP debugger connections (Do not enable for secure production builds)

################################################################################################################
# Enable or disable non-compoent networking support (e.g. socket debugging or Devman or none)
################################################################################################################
EHS_NETWORKING_SUPPORT=all


#unset EHS_DEVMAN_SUPPORT to disable the OS-level Devman monitoring features 
EHS_DEVMAN_SUPPORT=http

################################################################################################################
# Select which source of contributed library dependencies are used to build the target
################################################################################################################
# COMPONENT_VARIANT allows a specific variant of contributed ert-contrib-middleware/build directory 
# libraries to be used. The path is defined as follows (without delimietrs if options are not set:)
# $(EHS_GNU_OS_ARCH)_$(COMPONENT_VARIANT)-$(TOOLCHAIN_NAME) 
COMPONENT_VARIANT=gtk_gst

# For non-conformal paths to component libraries (e.g. those wrenched from pre-built platforms  rather than built in ert-ccontriib-middleware).:
#COMPONENT_BASE_TECHNOLOGIES_OVERRIDE_PATH=

################################################################################################################
# Select which toolboxes and supporting middleware options should be used (this guides the conditional build or ert-component porting layers)
################################################################################################################
# To enable  IO features "netx" DCC=1)  (e.g. GPIO, ADC.DAC, serial, user inputs etc. set  EHS_PERIPHERAL_DEVICE_SUPPORT )                                          #
EHS_PERIPHERAL_DEVICE_SUPPORT=all

# To enable  basic networking toolbox ("netx" DCC=2)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                          #
 EHS_COMPONENTBASIC_NETWORKING_SUPPORT=all    #note thi is not yet implemented
 
# To enable full TCPIP networking toolbox ("netx" DCC=3)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                   #
EHS_COMPONENT_NETWORKING_SUPPORT=all

# To enable UI  support ("ui", DCC=4)  set  EHS_GUI_SUPPORT to {gtk, framebuffer, OpenGLE1_1, android_stub}, depending support for your target   #
##EHS_GUI_SUPPORT=gtk
# To enable AV media  support ("media", DCC=5)  set  EHS_GUI_SUPPORT to {gst,vlc}, depending support for your target                                                   #
##EHS_AV_SUPPORT=gst10
# Set EHS_VIDEO_SUPPORT to "no" to disable video rndering support in the media payer (e.g. for audio only devies) 
##EHS_VIDEO_SUPPORT=yes
# This  is set to include the rendering features in eRT. It is  nearly always set, so should be removed (default on) and specific platforme xceptionsset instead
##EHS_MEDIA_SUPPORT=all
# set EHS_DEBUG_AV for verbose debugg from the media sub system
#DEFS += EHS_DEBUG_AV for more verbose debugging of AV media susbsystems

# The following toolbox contains legacy components that are no longer supported in the main toolsboxes and can b relegacted here in case               #
# backward compatability with previous apps  is required. Note this requires the toolbox hash checks to be  disabled                                                        #
##EHS_TOOLKIT_DEPRECATED=yes

################################### END OF TOOLBOX CONFIGURATION ###################################################


################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################

# place to incude overrides for variables suhc as devman servers URLs that might not be defined as required in the target hack files.
# XXXXX=

################################################################################################################
# Define any OS install scripts that should be run on first install
################################################################################################################
# see target/envtree/$EHS_OS-$EHS_ARCH/ for scripts available for configurating host OS's on first boot.
# HOST_OS_CONFIG_SCRIPTS+= \
#    0450-update-Realtek-net-firmware \


