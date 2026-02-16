/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the LGPLv3 license. You should have received a copy of the
* LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
* not, please visit
*	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/

/**
 * @file guiimage.c
 *
 * generic processing for guiimage (bitmap) functions.
 * target specific handling is defined elsewhere
 *
 *
 * @author: inx limited
 *
 */



/**
 * @page Verification Verification report
 * @section hal_graphics_font
 * @anchor hal_graphics_font
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */
#ifdef EHS_GUI_SUPPORT

#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "guiparams.h"
#include "globals.h" /* EHS_STRING_LENGTH_MAX */
#include "hal_string.h"
#include "font.h"
#include "hal_logger.h" /* EHSH_LOG_INFO, EHSH_LOG_WARNING */

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/**
 * Maximum number of parameters that the GUI file can support
 */
#define EHS_PARAM_COUNT_MAX 80

/*****************************************************************************/
/* Declare prototypes of local functions */

/**
 * Parse a rectangle
 * @param pParam[in] points to the start of each parameter
 * @param pRect[out] points to the Rectangle
 * @param nParamsRead[in] Number of parameters read so far.
 * @return number of parameters read by this function (0 if this function failed)
 */
EHS_LOCAL ehs_uint16 EhsLParse_rectangle(const char** pParam, EhsGraphicsRectangleClass* pRect, ehs_uint16 nParamsRead);

/**
 * Parse a colour (ARGB)
 * @param pParam[in] points to the start of each parameter
 * @param pColour[out] points to the colour to load
 * @param nParamsRead Number of parameters read so far.
 * @return number of parameters read by this function (0 if this function failed)
 */
EHS_LOCAL ehs_uint16 EhsLParse_colourARGB(const char** pParam, EhsGraphicsColourClass* pColour, ehs_uint16 nParamsRead);

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

/**
 * Parse a rectangle
 * @param pParam[in] points to the start of each parameter
 * @param pRect[out] points to the Rectangle
 * @param nParamsRead[in] Number of parameters read so far.
 * @return number of parameters read by this function (0 if this function failed)
 */
EHS_LOCAL ehs_uint16 EhsLParse_rectangle(const char** pParam, EhsGraphicsRectangleClass* pRect, ehs_uint16 nParamsRead)
{
    ehs_uint16 nParsed = 0u; /* assume function fails */

    if (nParamsRead >= 4u)
    {
        ehs_sint32 nLeft, nTop, nWidth, nHeight;
        /* get common parameters from input */
        EhsGetSint32FromString(&nLeft,pParam[nParsed++]);
        EhsGetSint32FromString(&nTop,pParam[nParsed++]);
        EhsGetSint32FromString(&nWidth,pParam[nParsed++]);
        EhsGetSint32FromString(&nHeight,pParam[nParsed++]);
        //printf("x=%d, y=%d, w=%d, h=%d \n", nLeft, nTop, nWidth, nHeight);
        pRect->nLeft = (ehs_coord)nLeft;
        pRect->nTop = (ehs_coord)nTop;
        pRect->nWidth = (ehs_coord)nWidth;
        pRect->nHeight = (ehs_coord)nHeight;
    }

    return nParsed;
}

/**
 * Parse a colour (ARGB)
 * @param pParam[in] points to the start of each parameter
 * @param pColour[out] points to the colour to load
 * @param nParamsRead Number of parameters read so far.
 * @return number of parameters read by this function (0 if this function failed)
 */
EHS_LOCAL ehs_uint16 EhsLParse_colourARGB(const char** pParam, EhsGraphicsColourClass* pColour, ehs_uint16 nParamsRead)
{
    ehs_uint16 nParsed = 0u; /* assume function fails */

    if (nParamsRead >= 4u) /* common params successfully extracted */
    {
        EhsGetUint8FromString(&(pColour->sComp.nAlpha),pParam[nParsed++]);
        EhsGetUint8FromString(&(pColour->sComp.nRed),pParam[nParsed++]);
        EhsGetUint8FromString(&(pColour->sComp.nGreen),pParam[nParsed++]);
        EhsGetUint8FromString(&(pColour->sComp.nBlue),pParam[nParsed++]);
    }

    return nParsed;
}

/**
 * Parse the parameters common to all widget types
 * @param pParam points to the start of each parameter
 * @param pGuiParams points to the GUI parameters structure
 * @param nVersion Version number of the current parameters file
 * @param nParamsRead Number of parameters read so far.
 * @return number of parameters read by this function (0 if this function failed)
 */
ehs_uint16 EhsParseGuiParameters_common(const char** pParam, EhsGuiParamsType* pGuiParams, ehs_uint16 nVersion, ehs_uint16 nParamsRead,const char* szObjectName)
{
    ehs_uint16 nParsed = 0; /* assume function fails */
#ifdef EHS_STORE_WIDGET_NAMES
    EhsStrncpy(pGuiParams->widgetName, szObjectName, EHS_OBJECT_NAME_MAX_SIZE);
#endif

    nParsed += EhsLParse_rectangle(pParam, &(pGuiParams->xRect), nParamsRead);

    if (nParsed != 0)
    {

        if (nVersion == 0)
        {
            if (nParamsRead > 6)
            {
                nParsed += 2; /* skip two dummy parameters */
            }
            else
            {
                nParsed = 0; /* invalid input format */
            }
        }
        else
        {
            if (nParamsRead > 5)
            {
                /* get z-order into pParams->nZorder without conversion - we can address
                 * the conversion issue later */
                EhsGetSint32FromString((ehs_sint32*)&(pGuiParams->nZorder),pParam[nParsed++]);
            }
            else
            {
                nParsed = 0; /* invalid input format */
            }
        }
    }
    return nParsed;

}

/**
 * Parse the specific parameters for a bitmap
 * @param pParam points to the start of each parameter
 * @param pGuiParams points to the GUI parameters structure
 * @param nVersion Version number of the current parameters file
 * @param nParamsRead Number of parameters read so far.
 * @return true if parsing is successful
 */
ehs_bool EhsParseGuiParameters_bitmap(const char** pParam, EhsGuiParamsType* pGuiParams, ehs_uint16 nVersion, ehs_uint16 nParamsRead,const char* szObjectName)
{
    ehs_bool bRet = EHS_FALSE; /* assume failure */
    char* pTmp; /* Used to find the end of the params string */
    ehs_uint16 nParsed = 0; /* how many parameters have been read so far */

    nParsed += EhsParseGuiParameters_common(pParam, pGuiParams, nVersion, nParamsRead, szObjectName);

    if (nParsed > 0) /* common params successfully extracted */
    {
        if (nVersion == 0)
        {
            if (nParamsRead >= (2+nParsed))
            {
                EhsGetWordFromString(pGuiParams->uClass.xBitmap.szBitmapName, pParam[nParsed++]);
                pTmp = strchr(pGuiParams->uClass.xBitmap.szBitmapName,EHS_PARAM_SEPARATOR);
                *pTmp = '\0';
                /* get z-order into pParams->nZorder without conversion - we can address
                 * the conversion issue later */
                EhsGetSint32FromString((ehs_sint32*)&(pGuiParams->nZorder),pParam[nParsed++]);
                bRet = EHS_TRUE;
            }
        }
        else
        {
            if (nParamsRead >= (2+nParsed))
            {
                /**
                 * DK
                 * note - an extra parameter has been added to the gui parameter file, lockAspectRatio, after the bitmap name parameter.
                 * At this time it is only used by IGB and ignored by EHS.
                 * The version number of the gui file has been left as 1.3 to reflect this
                 */
                EhsGetUint8FromString(&(pGuiParams->uClass.xBitmap.nImageAlpha),pParam[nParsed++]);
                EhsGetWordFromString(pGuiParams->uClass.xBitmap.szBitmapName, pParam[nParsed++]);
                pTmp = strchr(pGuiParams->uClass.xBitmap.szBitmapName,EHS_PARAM_SEPARATOR);
                if (pTmp)
                {
                    *pTmp = '\0';
                }
                bRet = EHS_TRUE;
            }
        }
    }

    return bRet;
}


/**
 * Parse the parameters for a textbox
 * @param pParam points to the start of each parameter
 * @param pGuiParams points to the GUI parameters structure
 * @param nVersion Version number of the current parameters file
 * @param nParamsRead Number of parameters read so far.
 * @return true if parsing is successful
 */
ehs_bool EhsParseGuiParameters_textbox(const char** pParam, EhsGuiParamsType* pGuiParams, ehs_uint16 nVersion, ehs_uint16 nParamsRead,const char* szObjectName)
{
    ehs_bool bRet = EHS_FALSE; /* assume failure */
    //char szObjectName[EHS_STRING_LENGTH_MAX]; /* object name - discarded once it's been read */ //Let's use this as a max for anything to do with GUIs.

    ehs_uint16 nParsed = EhsParseGuiParameters_common(pParam, pGuiParams, nVersion, nParamsRead, szObjectName);
    /* Note for the QY version we don't need any of this, because we never render or configure anything graphical.*/
    EhsStrcpy((pGuiParams->uClass.xTextbox.szFontName),EHSHG_FONT_DEFAULT);

    if (nParsed > 0) /* common params successfully extracted */
    {
        if (nVersion == 0)
        {
            if (nParamsRead >= (8+nParsed))
            {

                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xBgColour.sComp.nRed),pParam[nParsed++]);
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xBgColour.sComp.nGreen),pParam[nParsed++]);
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xBgColour.sComp.nBlue),pParam[nParsed++]);
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xFgColour.sComp.nRed),pParam[nParsed++]);
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xFgColour.sComp.nGreen),pParam[nParsed++]);
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xFgColour.sComp.nBlue),pParam[nParsed++]);
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xBgColour.sComp.nAlpha),"0");
                EhsGetUint8FromString(&(pGuiParams->uClass.xTextbox.xFgColour.sComp.nAlpha),"255");
                /* get z-order into pParams->nZorder without conversion - we can address
                 * the conversion issue later */
                EhsGetSint32FromString((ehs_sint32*)&(pGuiParams->nZorder),pParam[nParsed++]);
                bRet = EHS_TRUE;
            }
        }
        else
        {
            if (nParamsRead >= (8+nParsed))
            {
                nParsed += EhsLParse_colourARGB(&(pParam[nParsed]),&(pGuiParams->uClass.xTextbox.xFgColour),nParamsRead-nParsed);
                nParsed += EhsLParse_colourARGB(&(pParam[nParsed]),&(pGuiParams->uClass.xTextbox.xBgColour),nParamsRead-nParsed);

                if (nVersion >= 130)
                {
                    if (nParamsRead >= (6+nParsed))
                    {
                        EhsGetWordFromString((pGuiParams->uClass.xTextbox.szFontName),pParam[nParsed++]);
                        EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nIndentL),pParam[nParsed++]);
                        EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nIndentR),pParam[nParsed++]);
                        EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nIndentT),pParam[nParsed++]);
                        EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nIndentB),pParam[nParsed++]);
                        EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nLineSep),pParam[nParsed++]);
                        if (nVersion >= 140)
                        {
                            if (nParamsRead >= (2+nParsed))
                            {
                                EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nType),pParam[nParsed++]);
                                EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nProp),pParam[nParsed++]);
                                if (nParamsRead >= (2+nParsed))
                                {
                                    EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nCurve),pParam[nParsed++]);
                                    EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nParent),pParam[nParsed++]);
                                }
                            }
                        }
                        bRet = EHS_TRUE;
                    }
                }
                else
                {
                    bRet = EHS_TRUE;
                }
            }
        }
    }

    return bRet;
}

/**
 * Parse the parameters for a patch
 * @param pParam points to the start of each parameter
 * @param pGuiParams points to the GUI parameters structure
 * @param nVersion Version number of the current parameters file
 * @param nParamsRead Number of parameters read so far.
 * @return true if parsing is successful
 */
ehs_bool EhsParseGuiParameters_patch(const char** pParam, EhsGuiParamsType* pGuiParams, ehs_uint16 nVersion, ehs_uint16 nParamsRead,const char* szObjectName)
{
    ehs_bool bRet = EHS_FALSE; /* assume failure */

    ehs_uint16 nParsed = EhsParseGuiParameters_common(pParam, pGuiParams, nVersion, nParamsRead, szObjectName);

    if (nParsed > 0) /* common params successfully extracted */
    {
        if (nParamsRead >= (4+nParsed))
        {
            nParsed += EhsLParse_colourARGB(&(pParam[nParsed]),&(pGuiParams->uClass.xTextbox.xFgColour),nParamsRead-nParsed);
            if (nParamsRead >= (1+nParsed))
            {
                EhsGetUint16FromString(&(pGuiParams->uClass.xTextbox.nCurve),pParam[nParsed++]);
            }
            bRet = EHS_TRUE;
        }
    }

    return bRet;
}

/* Checks if widget is textbox type and outputs type if so ... That's great, but what is this for? 
*/
ehs_bool EhsParseGuiParametersTextBox2Type(const char* szObjectType, ehs_sint16* type)
{
    *type = -1;
    if(0 == EhsStricmp(szObjectType,"gui_textbox2")){
        *type = 0;
        return EHS_TRUE;
    }else if(0 == EhsStricmp(szObjectType,"gui_boolbox2")){
        *type = 1;
        return EHS_TRUE;
    }else if(0 == EhsStricmp(szObjectType,"gui_intbox2")){
        *type = 2;
        return EHS_TRUE;
    }else if(0 == EhsStricmp(szObjectType,"gui_realbox2")){
        *type = 3;
        return EHS_TRUE;
    }
    return EHS_FALSE;
}


/**
 * Reads the SODL parms (not the GUI paramters) initially and then finds the gui params.
 * Convert parameters text into an instance of EhsGuiParamsType.
 * No error checking implemented here.
 * @todo implement error checking.
 * @todo compress 32-bit z-order into 16-bit z-order
 * @todo use macro names instead of strings
 *
 * @param szParamsText[in] "raw" parameter text
 * @param pParams[out] Pointer to EhsGuiParamsType
 */
void EhsParseGuiParameters(const char* szParamsText, EhsGuiParamsType* pParams)
{
    const char* pParam[EHS_PARAM_COUNT_MAX]; /* points to the start of each parameter */
    char szObjectType[EHS_OBJECT_NAME_MAX_SIZE]; /* object type - used to determine eClass *///TODO:STRING LENGTH IS PROLLY TOO LONG. We can make this smaeer??
    char szObjectName[EHS_OBJECT_NAME_MAX_SIZE]; /* object names that we now store */
    ehs_uint16 nParam;
    ehs_uint16 nParamsRead; /* number of parameters that we've read so far */
    ehs_uint16 nVersion = 0; /* format of the parameters */
    ehs_bool bRet = EHS_FALSE; /* Assume the function fails */
    ehs_sint16 nTextBox2Type = -1; /* output from EhsParseGuiParametersTextBox2Type */
    char *pTmp; /* used to terminate strings */

    //printf("\n*** szParamsText=[%s]***\n", szParamsText ? szParamsText : "(null)");
    // fflush(stdout);
    //EHSH_LOG_INFO("EhsParseGuiParameters: ENTRY - parsing widget parameters");
    //EHSH_LOG_INFO("  Raw params text (first 100 chars): %.100s", szParamsText ? szParamsText : "(null)");

    /* split the params text into the params structure */
    nParamsRead = 0;
    pParam[nParamsRead++] = szParamsText;
    while (*szParamsText && nParamsRead < EHS_PARAM_COUNT_MAX)
    {
        if (*szParamsText == EHS_PARAM_SEPARATOR)
        {
            pParam[nParamsRead++] = ++szParamsText;
        }
        else
        {
            szParamsText++;
        }
    }

    //printf("\n*** QT DEBUG: Parameters split into %u tokens\n", nParamsRead);
    EHSH_LOG_INFO("  Parameters split into %u tokens", nParamsRead);

    if (nParamsRead > 1)
    {
        
        // fflush(stdout);
        /* determine version number */
        nParam = 0;
        if (EhsStrnicmp("1.0.0",pParam[nParam],5) == 0)
        {
            nVersion = 100;
        }
        else if (EhsStrnicmp("1.3",pParam[nParam],3) == 0)
        {
            nVersion = 130;
        }
        else if (EhsStrnicmp("1.4",pParam[nParam],3) == 0)
        {
            nVersion = 140;
        }
        nParam++;

        //printf("\n*** QT DEBUG: GUI file format version: %u\n", nVersion);
        // fflush(stdout);
        EHSH_LOG_INFO("  GUI file format version: %u", nVersion);

        EhsGetWordFromString(szObjectName,pParam[nParam]);
        //printf("\n*** all paramters are [%s\n] index 2 is [%s]",   pParam[nParam],pParam[0]);
        nParam++;
        //printf("\n*** OBJECTNAME: [%s]\n",szObjectName);
        /* identify widget type & load params */
        EhsGetWordFromString(szObjectType,pParam[nParam++]);
        pTmp = strchr(szObjectType,EHS_PARAM_SEPARATOR);
        if (pTmp)
        {
            *pTmp = '\0';
        }

        printf("\n*** DEBUG: Widget type: '%s'\n", szObjectType);
        // fflush(stdout);
        EHSH_LOG_INFO("  Widget type: '%s'", szObjectType);

        pParams->eClass = EHS_WIDGET_CLASS_INVALID;
        if ((0 == EhsStricmp(szObjectType,"gui_bitmap")) ||
                (0 == EhsStricmp(szObjectType,"gui_image1")))
        {
            printf("\n*** QT DEBUG: Matched BITMAP widget type\n");
            // fflush(stdout);
            EHSH_LOG_INFO("  -> Parsing as BITMAP widget");
            if (EhsParseGuiParameters_bitmap(&(pParam[nParam]),pParams, nVersion, nParamsRead-nParam,szObjectName))
            {
                pParams->eClass = EHS_WIDGET_CLASS_BITMAP;
                EHSH_LOG_INFO("  (ok) bitmap parsing succeeded");
            }
            else
            {
                EHSH_LOG_WARNING("  (fail) BITMAP parsing FAILED");
            }
        }
        else if ((0 == EhsStricmp(szObjectType,"gui_textbox")) ||
                 EhsParseGuiParametersTextBox2Type(szObjectType, &nTextBox2Type))
        {
            // printf("\n*** QT DEBUG: Matched TEXTBOX widget type\n");
            // fflush(stdout);
            EHSH_LOG_INFO("  -> Parsing as TEXTBOX widget");
            if (EhsParseGuiParameters_textbox(&(pParam[nParam]),pParams, nVersion, nParamsRead-nParam,szObjectName))
            {
                pParams->eClass = EHS_WIDGET_CLASS_TEXTBOX;
                EHSH_LOG_INFO("  (ok) textbox parsing succeeded");
            }
            else
            {
                EHSH_LOG_WARNING("  (fail) TEXTBOX parsing FAILED");
            }
        }
        else if (0 == EhsStricmp(szObjectType,"gui_patch")) //We use this for the viewport types also
        {
            EHSH_LOG_INFO("  -> Parsing as PATCH widget");
            if (EhsParseGuiParameters_patch(&(pParam[nParam]), pParams, nVersion, nParamsRead-nParam,szObjectName))
            {
                pParams->eClass = EHS_WIDGET_CLASS_PATCH;
                EHSH_LOG_INFO("  (ok) patch parsing succeeded");
            }
            else
            {
                EHSH_LOG_WARNING("  (fail) PATCH parsing FAILED");
            }
        }
        else if (0 == EhsStricmp(szObjectType,"gui_viewport"))
        {
            EHSH_LOG_INFO("  -> Parsing as VIEWPORT widget");
            if (EhsParseGuiParameters_patch(&(pParam[nParam]), pParams, nVersion, nParamsRead-nParam,szObjectName))
            {
                pParams->eClass = EHS_WIDGET_CLASS_VIEWPORT;
                EHSH_LOG_INFO("  (ok) viewport parsing succeeded");
            }
            else
            {
                EHSH_LOG_WARNING("  (fail) VIEWPORT parsing FAILED");
            }
        }
        else if (0 == EhsStricmp(szObjectType,"gui_video_port"))
        {
            EHSH_LOG_INFO("  -> Parsing as VIDEO_PORT widget");
            if(EhsParseGuiParameters_patch(&(pParam[nParam]), pParams, nVersion, nParamsRead-nParam,szObjectName))
            {
                pParams->eClass = EHS_WIDGET_CLASS_VIDEO_PORT;
                EHSH_LOG_INFO("  (ok) video_port parsing succeeded");
            }
            else
            {
                EHSH_LOG_WARNING("  (fail) VIDEO_PORT parsing FAILED");
            }
        }
        else
        {
            // printf("\n*** QT DEBUG: NO MATCH for widget type '%s'\n", szObjectType);
            // fflush(stdout);
            EHSH_LOG_WARNING("  (fail) Unknown widget type '%s' - class remains INVALID", szObjectType);
        }

        /* convert sint32 to uint32 by adding 2^31 - means min(sint32) => 0, max(sint32) > max(uint32)
         * preserves monotonicity of zOrdering */
        pParams->nZorder = (0x80000000 ^ pParams->nZorder);

        // printf("\n*** QT DEBUG: EhsParseGuiParameters EXIT - eClass=%d, nZorder=%u\n", pParams->eClass, pParams->nZorder);
        // fflush(stdout);
        EHSH_LOG_INFO("EhsParseGuiParameters: EXIT - eClass=%d, nZorder=%u", pParams->eClass, pParams->nZorder);
    }
    else
    {
        // printf("\n*** QT DEBUG: nParamsRead=%u (too few parameters)\n", nParamsRead);
        // fflush(stdout);
        EHSH_LOG_WARNING("EhsParseGuiParameters: EXIT - insufficient parameters (nParamsRead=%u)", nParamsRead);
    }
    // printf("\n*** QT DEBUG: EhsParseGuiParameters COMPLETE\n");
    // fflush(stdout);
}

#endif /* EHS_GUI_SUPPORT */
