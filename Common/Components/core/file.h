/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * fileFunctions.h
 *
 * function prototypes for fileFunctions.c - deals with all types
 *
 * @author: inx limited
 *
 */

#ifndef EHS_FILE_FUNCTIONS_H
#define EHS_FILE_FUNCTIONS_H
#include "ehs_fb_types.h"
#include "globals.h"
#include "hal_file.h"



#define MAX_SEP_STRING_LEN 4  // biggest is "( )".
#define MAX_INTEGER_LEN 11
#define MAX_APPEND_LEN 2
#define DEFAULT_DELIMITER_INT ' '
#define MAX_READ_CHARS 32

#define NO_READ_ERRORS						0
#define READ_ERROR_EOF						1
#define READ_ERROR_INVALID_PARAM			2
#define READ_ERROR_INVALID_FILE_DESCRIPTOR	3

// real write format: -d.dddddde-ddd   i.e. actual width = MIN_REAL_WRITE_WIDTH + precision.
//                     ->      <-  precision.
#define MIN_REAL_WRITE_WIDTH 7
#define MAX_REAL_PRECISION   15

// error values for all file functions.
#define ERR_NO_ERROR							0
#define ERR_END_OF_FILE							1
#define ERR_FAILED_TO_OPEN_FILE					2
#define ERR_CLOSE_FILE_FAILED					3
#define ERR_INVALID_DESCRIPTOR					4
#define ERR_FILE_READ_ERROR						5
#define ERR_INVALID_FORMAT						6
#define ERR_INVALID_PRECISION					7
#define ERR_STRING_COPY_FAILED					8
#define ERR_NO_DATA_SIZE_IN_BIN_MODE			9
#define ERR_DATA_SIZE_TOO_LARGE			        10

/* utility function prototypes */
int GetFilename(const char *pParams, char *szFileName,char *szFileTempExtension);
int GetWidth(const char *pParams, int nIndex, int *pWidth);
int GetPrecision(const char *pParams, int nIndex, int *nPrecision);
int GetSeparator(const char *pParams, int nIndex, char *cSeparator);
int GetAppend(const char *pParams, int nIndex, int *pAppend);
int GetFixedWidthField( char *szBuffer, int *nWidth, ehs_FILE* sFile, int *nError );
int GetFixedWidthFieldReal( char *szBuffer, int *nWidth, ehs_FILE* sFile, int *nError );
int GetFilenameSplit_allowSpaces(const char *FullFilename, char *szFileName, char *tempExtension);

/******************************************************************************/
/* Declare integer file, readonly */
#define EHS_FB_NAME_FILE_ReadOnly_Int "FILE_ReadOnly_Int"
#define EHS_FB_ID_FILE_ReadOnly_Int 0xDA57

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_Int)

/******************************************************************************/
/* Declare integer file, writeonly */

#define EHS_FB_NAME_FILE_WriteOnly_Int "FILE_WriteOnly_Int"
#define EHS_FB_ID_FILE_WriteOnly_Int 0x3355  // this was duplicate of EHS_FB_ID_DemultiplexTwoOutputString

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Int);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Write);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Run_SetName_Int);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_Int)

/******************************************************************************/
/* Declare boolean file, readonly */

#define EHS_FB_NAME_FILE_ReadOnly_Bool "FILE_ReadOnly_Bool"
#define EHS_FB_ID_FILE_ReadOnly_Bool 0xC7C2

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_Bool)

/******************************************************************************/
/* Declare boolean file, writeonly */

#define EHS_FB_NAME_FILE_WriteOnly_Bool "FILE_WriteOnly_Bool"
#define EHS_FB_ID_FILE_WriteOnly_Bool 0xEA51

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Bool);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Write);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Run_SetName_Bool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_Bool)

/******************************************************************************/
/* Declare string file, readonly */

#define EHS_FB_NAME_FILE_ReadOnly_String "FILE_ReadOnly_String"
#define EHS_FB_ID_FILE_ReadOnly_String 0x560C

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_String);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_ReadOnly_String);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_String);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_String)

/******************************************************************************/
/* Declare string file, writeonly */

#define EHS_FB_NAME_FILE_WriteOnly_String "FILE_WriteOnly_String"
#define EHS_FB_ID_FILE_WriteOnly_String 0xBBA0

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_String);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_WriteOnly_String);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_String);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Write);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Run_SetName_String);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_String)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare float file, readonly */

#define EHS_FB_NAME_FILE_ReadOnly_Real "FILE_ReadOnly_Real"
#define EHS_FB_ID_FILE_ReadOnly_Real 0x65E2

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_Float)

/******************************************************************************/
/* Declare float file, writeonly */
#define EHS_FB_NAME_FILE_WriteOnly_Real "FILE_WriteOnly_Real"
#define EHS_FB_ID_FILE_WriteOnly_Real 0x4871

EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Float);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Open);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Close);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Write);
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(Run_SetName_Float_Write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_Float)
#endif /* EHS_TARGET_FP_SUPPORT */


#endif /* EHS_FILE_FUNCTIONS_H */



