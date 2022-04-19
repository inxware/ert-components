/** @file messages.h
 * All messages are gathered into a single place (this file) to support global changes,
 * setting specific message styles, and to support internationalization.
 * 
 * @author: inx limited
 * @version: $Revision: 4748 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2006. All rights reserved.
 */

/**
 * @page Verification Verification report
 * @section messages_h
 * @anchor messages_h
 * @subsection misra MISRA compliance:
 * Demonstrated MISRA compliant on 
 * Last modified on $Date:$
 *
 * This file contained the following derogations to the MISRA standard:
 * - none.
 */

#ifndef EHS_MESSAGES_H
#define EHS_MESSAGES_H


#include "hal_console.h"
#include "hal_logger.h"


/* #define EhsError(err) EhsConsolePrintf("**Error at: %s:%d",__FILE__,__LINE__,);EhsConsolePrintf(err); */

/* Console messages */
#define EHS_MSG_STATENAME_READY "Ready\n"
#define EHS_MSG_STATENAME_RUNNING "Running\n"
#define EHS_MSG_STATENAME_EMPTY "Empty\n"

#define EHS_MSG_DEBUG_ON "=+ (on)\n"
#define EHS_MSG_DEBUG_OFF "=- (off)\n"

#define EHS_MSG_ERROR_SET_INVALID_TRIGGER(x) "**Error: no such trigger id, %d",x
#define EHS_MSG_ERROR_SET_INVALID_DATA(x,y) "**Error: no such data line %d,%c",x,y
#define EHS_MSG_ERROR_TRIGGER_IDX_INVALID "**Error: trigger index invalid - IDF finish port doesn't match source code"
#define EHS_MSG_ERROR_SET_EVENT_PARAM "**Error: Set event requires integer as trigger id"
#define EHS_MSG_ERROR_SET_EVENT_STATE "**Error: Set event does not work with no application loaded"
#define EHS_MSG_ERROR_SET_DATA_PARAM "**Error: Set data requires =D<id:int>,<type:char>=<value:*>"
#define EHS_MSG_ERROR_SET_DATA_STATE "**Error: Set data does not work with no application loaded"
#define EHS_MSG_ERROR_SET_MONITOR_PARAM "**Error: Set monitor lines requires =M+<id:int>,<type:char> or =M-<id:int>,<type:char>"
#define EHS_MSG_ERROR_SET_MONITOR_STATE "**Error: Set monitor line works only in debug mode"
#define EHS_MSG_ERROR_GET_UNRECOG "**Error: Unrecognised option to Get command"
#define EHS_MSG_ERROR_SET_UNRECOG "**Error: Unrecognised option to Set command"
#define EHS_MSG_ERROR_ALREADY_RUNNING "**Error: Already running"
#define EHS_MSG_ERROR_NO_APP "**Error: No application loaded"
#define EHS_MSG_ERROR_SET_INVALID "**Error: Unrecognised option to modify breakpoints command"
#define EHS_MSG_ERROR_NOT_RUNNING "**Error: Application not running"
#define EHS_MSG_ERROR_LOAD_FAILED "**Error: Failed to load file"
#define EHS_MSG_ERROR_LOAD_GIF_FAILED(x) "**Error: Failed to load gif %s",x
#define EHS_MSG_ERROR_INVALIDFILENAME "**Error: Invalid filename"
#define EHS_MSG_ERROR_EVENT_QUEUE_EMPTY(x) "**Error: EHS event queue unexpectedly empty for group %-d",x
#define EHS_MSG_ERROR_FILE_NOT_OPEN(x) "**Error: Failed to open file %s",x
#define EHS_MSG_ERROR_FILE_NOT_CLOSE(x) "**Error: Close failed for file %s",x
#define EHS_MSG_ERROR_FILE_NOT_WRITE(x) "**Error: Write failed for file %s",x
#define EHS_MSG_ERROR_FILE_TIMEOUT(name, nSizeRemaining, nSize) "**Error: Timeout in receiving file %s -- %d bytes (%d%%) remaining",name,nSizeRemaining,((nSizeRemaining*100u)/nSize)
#define EHS_MSG_ERROR_MEMORY "**Error: Out of memory in EHS"
#define EHS_MSG_ERROR_MEMORY_INIT "**Error: Out of memory when initialising components"
#define EHS_MSG_ERROR_MEMORY_ALLOC_DURING_RUN "**Error: Can't allocate readonly or writeable memory in run mode"
#define EHS_MSG_ERROR_INIT_FAIL(x) "**Error: Initialization failed for block %s",x
#define EHS_MSG_ERROR_TOOLKITS "**Error: Too many toolkits (%d) - increase EHS_MAX_TOOLKITS if necessary"
#define EHS_MSG_ERROR_CONFIG_FILE "**Error: Couldn't create config file - using default values"
#define EHS_MSG_ERROR_CONFIG_IDENTITY "**Error: Couldn't read identity from config file - using default value"
#define EHS_MSG_FATAL_NO_STATE "**FATAL: EHS State is unknown"
#define EHS_MSG_ERROR_INVALID_FORMAT(x) "**Error: Invalid format to printf(%s)",x
#define EHS_MSG_ERROR_MEMORY_OVERWRITE "**Error: Attempt to overwrite beyond allocated memory"
#define EHS_MSG_ERROR_NOT_YET_IMPLEMENTED(x) "**Error: Feature %s not yet implemented",x
#define EHS_MSG_ERROR_INVALID_STRING_CONNECTION(y) "**Error: Invalid string connection at %x (%s)",y,y
#define EHS_MSG_ERROR_HS_UTF32_FAILED(x) "**Error: Could not convert utf32 character in string (%s)",x
#define EHS_MSG_ERROR_HS_UTF32_RANGE(x) "**Error: utf-8 character out of range in string (%s)",x

/* Graphics subsystem */
#define EHS_MSG_ERROR_WIDGET_NOT_INIT "**Error: Widget hasn't been initialised"
#define EHS_MSG_ERROR_WIDGET_TYPE_NOT_RECOG(x) "**Error: Widget file type isn't recognised (%s)",x
#define EHS_MSG_ERROR_WIDGET_TOO_MANY "**Error: Too many widgets allocated"
#define EHS_MSG_WARNING_HGHTML_IGNORED_INVALID_TAG "**Warning: Ignored invalid tag in HTML"
#define EHS_MSG_WARNING_HGHTML_MISSING_PARAM "**Warning: Missing parameter from HTML tag"
#define EHS_MSG_WARNING_HGHTML_NO_MEM "**Warning: no memory available to hold HTML word"
#define EHS_MSG_FONT_NON_MONOTONIC(nLast,nPrev) "**Error: Glyph identifiers in font aren't in order (found at %d, after %d)",nLast,nPrev
#define EHS_MSG_FONT_DEFAULT(nId) "**Warning: default glyph not found in font. Using char %s",nId
#define EHS_MSG_WARNING_HGHTML_IGNORED_PARAMS(szParams) "**Warning: Ignored parameters \"%s\" in HTML tag"
#define EHS_MSG_WARNING_HGHTML_NO_SPECIAL(szEntityName) "**Warning: No special entity is defined with name %s",szEntityName
#define EHS_MSG_WARNING_HGHTML_TRUNC_PARAM(szTagString) "**Warning: HTML tag has been truncated: %s",szTagString
#define EHS_MSG_ERROR_WIDGET_FILEOPEN(x) "**Error: File open failed for %s",x
#define EHS_MSG_ERROR_WIDGET_LOADFAILED(x) "**Error: Loading failed for %s",x
/* Calculator messages */
#define EHS_MSG_ERROR_DIV0 "**Error: Attempt to divide by zero - result set to maxint"

#define EHS_MSG_CMD_RESP "> \n"

/* Parser messages */
#define EHS_MSG_PARSER_FAILED "***** Failed to load SODL *****"
#define EHS_MSG_PARSER_NO_OPEN(x,y) "**Error: Couldn't open SODL file %s because %s",x,y
#define EHS_MSG_PARSER_TOO_BIG "**Error: SODL file too large to hold in memory"
#define EHS_MSG_PARSER_NO_OBJECTS "**Error: No objects found in SODL file"
#define EHS_MSG_PARSER_TOO_MANY_GROUPS "**Error: Too many groups defined in SODL file"
#define EHS_MSG_PARSER_DUPLICATE_GROUPID "**Error: Group ID duplicated in SODL file"
#define EHS_MSG_PARSER_SINT32_RANGE(x) "**Error: Signed value out of range, value: %d",x
#define EHS_MSG_PARSER_UINT32_RANGE(x,c) "**Error: Value out of range, value: %d%d...",x,c
#define EHS_MSG_PARSER_SINT16_RANGE(x) "**Error: signed 16-bit value out of range, value: %d",x
#define EHS_MSG_PARSER_UINT16_RANGE(x) "**Error: unsigned 16-bit value out of range, value: %d",x
#define EHS_MSG_PARSER_UINT8_RANGE(x) "**Error: unsigned 8-bit value out of range, value: %d",x
#define EHS_MSG_PARSER_GROUP_FAILED(x) "**Error: Failed to read %dth group",x
#define EHS_MSG_PARSER_INVALID_GROUP(x,y,z) "**Error: Group %d defined with period (%d) < allocation (%d)",x,y,z
#define EHS_MSG_PARSER_MINIMUM_PERIOD(x) "**Error: Period for group %d too small",x
#define EHS_MSG_PARSER_PARAMS_NO_OPEN(x) "**Error: Couldn't open params file %s",x
#define EHS_MSG_PARSER_PARAMS_MISSING "**Error: Parameters missing when parameter file was expected"
#define EHS_MSG_PARSER_PARAMS_TAG_NOT_FOUND(x) "**Error: Parameter file didn't contain tag %s",x
#define EHS_MSG_PARSER_PARAMS_TAG_MULTIPLE(x) "**Error: Parameter file contained multiple instances of tag %s",x
#define EHS_MSG_PARSER_BLOCK_NAME_INVALID(x) "**Error: Function block name %s not recognised",x
#define EHS_MSG_PARSER_BLOCK_VERS_INVALID(x,vers) "**Error: Function block name %s found but vers %d not recognised",x
#define EHS_MSG_PARSER_FUNCTION_NAME_INVALID(x,y) "**Error: Function name [%s] not recognised in [%s]",x,y
#define EHS_MSG_PARSER_ATOMIC_FLAG_MISSING "**Error: Atomic flag missing from function definition"
#define EHS_MSG_PARSER_GROUPID_MISSING "**Error: Group ID missing from function definition"
#define EHS_MSG_PARSER_GROUPID_INVALID(x) "**Error: Group ID %-d specified by function doesn't exist",x
#define EHS_MSG_PARSER_TRIGGERID_MISSING "**Error: Trigger ID missing from function definition"
#define EHS_MSG_PARSER_NUM_INPUTS_MISSING "**Error: Number of inputs missing from function definition"
#define EHS_MSG_PARSER_TOO_MANY_TRIGGERS "**Error: Too many triggers declared in SODL"
#define EHS_MSG_PARSER_TOO_MANY_FUNCTIONS "**Error: Too many functions declared in SODL"
#define EHS_MSG_PARSER_TOO_MANY_BLOCKS "**Error: Too many function blocks declared in SODL"
#define EHS_MSG_PARSER_P1EOF "**Error: Unexpected EOF in SODL file"
#define EHS_MSG_PARSER_P1GRPTBLINIT "**Error: Group table didn't initialise"
#define EHS_MSG_PARSER_P1FIDTCBINIT(nf) "**Error: Function instance data for callbacks didn't initialise (%d callbacks)",nf
#define EHS_MSG_PARSER_P1FIDTFINIT(nf) "**Error: Function instance data for functions didn't initialise (%d callbacks)",nf
#define EHS_MSG_PARSER_P1TRIGINIT(nt) "**Error: Trigger table didn't initialise (%d triggers)",nt
#define EHS_MSG_PARSER_P1CONINIT(ni,nb,ns,nf) "**Error: Data connections didn't initialise (%d int, %d bool, %d strings, %d floats)",ni,nb,ns,nf
#define EHS_MSG_PARSER_P1GRPPARSE "**Error: couldn't parse number of groups"
#define EHS_MSG_PARSER_P1NOBEGIN(x) "**Error: BEGIN missing at line %d",x
#define EHS_MSG_PARSER_P1MEM(x) "**Error: Out of memory when allocating %s",x
#define EHS_MSG_PARSER_P1FUNCTIONSYNTAX(x) "**Error: Syntax error in function at line %d",x
#define EHS_MSG_PARSER_P1FUNCTIONTRUNC(x) "**Error: Function definition ends unexpectedly at line %d",x
/* font error messages */
#define EHS_MSG_FONT_NAME_MISSING(fname)"**Error: Font in file %s has no font name following FONT statement",fname
#define EHS_MSG_FONT_TOO_MANY(fname) "**Effor: Too many fonts loaded - can't load %s. Increase EHS_MAX_FONTS in target_config.h",fname
#define EHS_MSG_FONT_NUMGLYPHS_MISSING(fname) "**Error: Font in file %s has no number of glyphs following CHARS statement",fname
#define EHS_MSG_FONT_CANT_OPEN(fname) "**Error: Can't open font file %s",fname
#define EHS_MSG_FONT_BITMAP(nId) "**Error: .bdf file Problem with bitmap for character %x",nId
#define EHS_MSG_FONT_MISSING_BBX_PARAM(nId) "**Error: .bdf file Missing BBX data for character %x",nId
#define EHS_MSG_FONT_MISSING_CODE "**Error: .bdf file missing parameter for ENCODING statement"
#define EHS_MSG_FONT_MISSING_DWIDTH_PARAM(nId) "**Error: .bdf file DWIDTH statement missing parameters in character %x",nId

#define EHSH_LOGGER_INVALID_NAMES_TABLE "**Error: Logger invalid names"
/* Target-specific error messages */
#define EHS_MSG_TGT_LINE_LENGTH "**Error: Line length read exceeds buffer size. Truncating line"
#define EHS_MSG_TGT_INPUT_QUEUE_FULL "**Error: Console input queue is full"
#define EHS_MSG_TGT_FILE_WRITE_FAILED(fname) "**Error: Attempt to write to file %s failed",fname
#define EHS_MSG_TGT_FILE_CLOSE_FAILED(fname) "**Error: Attempt to close file %s failed",fname
#define EHS_MSG_TGT_FILE_OPEN_APPEND(fname) "**Error: Attempted to open file %s using append mode -- not supported",fname
#define EHS_MSG_TGT_FILE_TOO_MANY "**Error: Too many files open, can't open any more"
#define EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_MODE(mode) "**Error: Unsupported graphics mode (%s)",mode
#define EHS_MSG_TGT_GRAPHICS_UNSUPPORTED_FORMAT(fn,fmt) "**Error: Unsupported graphics format in function %s(%d)",fn,fmt
#define EHS_MSG_TGT_STAPI(module,error) "**Error: STAPI problem in function %s: %s",module,error
#define EHS_MSG_TGT_WIN32(module,error) "**Error: Win32 problem in function %s: %s",module,error
#define EHS_MSG_TGT_NO_PALETTE "**Error: No palette supplied"
#define EHS_MSG_TGT_GRAPHICS_SURFACE_LIMIT "**Error: Attempt to create surface with width or height 0"
#define EHS_TGT_FATAL_TOOLKIT(x) "**Fatal: Couldn't load toolkit %s",x
#define EHS_MSG_ERR_HMPOOL "**Error: Can't switch to system memory pool"
#define EHS_MSG_ERR_HMPRE_CORRUPT(x) "**Error: Allocated memory before %x corrupted",x
#define EHS_MSG_ERR_HMPOST_CORRUPT(x) "**Error: Allocated memory after %x corrupted",x
#endif /* EHS_MESSAGES_H */

