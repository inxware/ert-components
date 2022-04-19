/**
 * fileFunctions.h
 *
 * function prototypes for fileFunctions.c - deals with all types
 *
 * @author: MDD
 * @version: $Revision: 4495 $
 * @date: $Date: 2006-11-06 16:22:28 +0000 (Mon, 06 Nov 2006) $
 * 
 * Copyright (c) inx limited, 2007. All rights reserved.
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
#define EHS_FB_FILE_READONLY_INT_NAME "FILE_ReadOnly_Int"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Int);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Int);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Int);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Int_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_Int)

/******************************************************************************/
/* Declare integer file, writeonly */
#define EHS_FB_FILE_WRITEONLY_INT_NAME "FILE_WriteOnly_Int"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Int);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Int);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Int);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Int_Write);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Run_SetName_Int);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_Int)

/******************************************************************************/
/* Declare boolean file, readonly */
#define EHS_FB_FILE_READONLY_BOOL_NAME "FILE_ReadOnly_Bool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Bool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Bool);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Bool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Bool_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_Bool)

/******************************************************************************/
/* Declare boolean file, writeonly */
#define EHS_FB_FILE_WRITEONLY_BOOL_NAME "FILE_WriteOnly_Bool"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Bool);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Bool);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Bool);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Bool_Write);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Run_SetName_Bool);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_Bool)

/******************************************************************************/
/* Declare string file, readonly */
#define EHS_FB_FILE_READONLY_STRING_NAME "FILE_ReadOnly_String"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_String);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_ReadOnly_String);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_String);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_String_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_String)

/******************************************************************************/
/* Declare string file, writeonly */
#define EHS_FB_FILE_WRITEONLY_STRING_NAME "FILE_WriteOnly_String"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_String);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_WriteOnly_String);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_String);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_String_Write);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Run_SetName_String);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_String)

#ifdef EHS_TARGET_FP_SUPPORT
/******************************************************************************/
/* Declare float file, readonly */
#define EHS_FB_FILE_READONLY_FLOAT_NAME "FILE_ReadOnly_Real"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_ReadOnly_Float);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_ReadOnly_Float);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_ReadOnly_Float);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_ReadOnly_Float_Read);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_ReadOnly_Float)

/******************************************************************************/
/* Declare float file, writeonly */
#define EHS_FB_FILE_WRITEONLY_FLOAT_NAME "FILE_WriteOnly_Real"
EHS_GLOBAL EHS_FB_IDENTIFY_FUNCTION(FILE_WriteOnly_Float);
EHS_GLOBAL EHS_FB_INIT_FUNCTION(FILE_WriteOnly_Float);
EHS_GLOBAL EHS_FB_DESTROY_FUNCTION(FILE_WriteOnly_Float);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Open);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Close);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(FILE_WriteOnly_Float_Write);
EHS_GLOBAL EHS_FB_RUN_FUNCTION(Run_SetName_Float_Write);

/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(FILE_WriteOnly_Float)
#endif /* EHS_TARGET_FP_SUPPORT */


#endif /* EHS_FILE_FUNCTIONS_H */



