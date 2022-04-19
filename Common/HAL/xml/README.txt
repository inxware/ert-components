This directory shall contain the XML library Glue code and make file entry.

XML is initially only used in a Function blocks, however it will later be used for sdl also.

The glue code should be any internal , initialisation, system management, or APIs
 implemented (if necessary) for EHS as required.
 The make file and library link commands should be contained in this directory also.
 
 At some point this code module can be made excludable from the 
 the build with a switch in the platform/config.mk files - howeer this is not necessary.
 @todo (note to self) the config.mk files should be structured to allow profile sets to be stacked up to avoid having to edit all the mk files belonging to a defined profile.
   

See the The module code common/components for the Function blocks that use XML

