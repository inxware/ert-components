/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the MPL2.0 license. You should have received a copy of the
 * MPL2.0 (Mozilla Public License2.0) license with this file. If
 * not, please visit
 *	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/


/** @file widget_image.c
 * Definitions for the image widget handling functions
 *
 * @author: inx limited
 *
 */

//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "widget.h"
#include "messages.h"
#include "hal_string.h"
#include "hal_process.h"
#include "hal_mem.h"


/**
 * Insert a widget itno the Z-ordered list of widgets. This is called
 * after a widget has been initialized, but before the index to the widget
 * table has been updated. This is only used by subclasses of EhsWidget.
 */
EHS_GLOBAL void EhsWidgetTable_updateZOrder(EhsWidgetTableClass* pWidgetTable, EhsWidgetClass* pWidget);

/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create via the widget's
 * pfCreate function.
 */
EHS_LOCAL ehs_bool EhsWidgetImage_create(struct EhsWidgetStruct* pWidget);

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy via the widget's
 * pfDestroy function.
 */
EHS_LOCAL void EhsWidgetImage_destroy(struct EhsWidgetStruct* pWidget);

/**
 * Load the image-specific data into the widget from the specified file
 */
EHS_LOCAL ehs_bool EhsWidgetImage_load(EhsWidgetClass* pWidget);

/**
 * Fade the image according to an opacity value
 *
 * @return true if the image needs to be redrawn (because it's appearance has changed)
 */
EHS_LOCAL ehs_bool EhsWidgetImage_fade(EhsWidgetClass* pWidget, ehs_uint8 nOpacity);

/**
 * Initialise the widget with the general widget and image specific properties, including
 * position, dimensions, z-ordering and filename containing image.
 *
 * @param nX X-coordinate for initial position of widget
 * @param nY Y-coordinate for initial position of widget
 * @param nZ Z order for widget
 * @param szFilename Name of file containing widget image data
 * @return Pointer to initialised widget, or NULL
 */
EhsWidgetClass* EhsWidgetImage_init(EhsGraphicsRectangleClass* pRect, ehs_uint16 nZ, ehs_uint8 nImageAlpha, const ehs_char* szFilename, ehs_bool bLoadImageFromAppDir)
{
    EhsWidgetClass* pWidget = EhsWidgetTable_new(&EhsWidgetTable);//printf("Initing filename=%s\n",szFilename);
    if (pWidget)
    {
        /* init the generic part */
        EhsWidget_init(pWidget,pRect, nZ, nImageAlpha);

        /* Do image specific widget properties */
        pWidget->eWidgetKind = EHS_WIDGET_KIND_IMAGE;
        EHS_WIDGET_IMAGE(pWidget).nOrigImageAlpha = nImageAlpha;
        EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha = nImageAlpha;
        pWidget->pfCreateFunc = EhsWidgetImage_create;
        pWidget->pfDestroyFunc = EhsWidgetImage_destroy;
        pWidget->pfFadeFunc = EhsWidgetImage_fade;
        if (szFilename && EhsStrlen(szFilename))
        {
            EhsWidgetImage_setFilename(pWidget, szFilename, EHS_FALSE); /* sets .szFilename to the statc (backup filename)  */
            // filename passed in here to init, is the filename as set in IGB, of an image that will be transmitted with the SODL to the app folder in EHS
            // we will store this filename as the backup image, for use when a filepath specified at load time cannot be found

            if (EHS_WIDGET_IMAGE(pWidget).szFilename)
            {
                EHS_WIDGET_IMAGE(pWidget).szBackupFilename = EhsHMem_tempAlloc(EhsStrlen(szFilename)+1);
                if (EHS_WIDGET_IMAGE(pWidget).szBackupFilename)
                    EhsStrcpy(EHS_WIDGET_IMAGE(pWidget).szBackupFilename, EHS_WIDGET_IMAGE(pWidget).szFilename);
                else
                    EHS_WIDGET_IMAGE(pWidget).szBackupFilename = NULL;
            }
            else
                EHS_WIDGET_IMAGE(pWidget).szFilename = NULL;

        }
        else     /* shouldn't get here if we trust the client */
        {
            //if (EHS_WIDGET_IMAGE(pWidget).szFilename) EhsHMem_tempFree(EHS_WIDGET_IMAGE(pWidget).szFilename);
            EHS_WIDGET_IMAGE(pWidget).szFilename = NULL;
            //if (EHS_WIDGET_IMAGE(pWidget).szBackupFilename) EhsHMem_tempFree(EHS_WIDGET_IMAGE(pWidget).szBackupFilename);
            EHS_WIDGET_IMAGE(pWidget).szBackupFilename = NULL;
        }

        EHS_WIDGET_IMAGE(pWidget).bLoadImageFromAppDir = bLoadImageFromAppDir; //initially the default is to load from the app directory
        pWidget->nState = EHS_WIDGET_STATE_INIT;
    }
    return pWidget;
}



/**
 * Create the widget. This is a necessary step prior to showing the widget. In the
 * case of the images, this parses the image file into the image stbDynamicFilenameructures.
 * NOTE: This must only be called by EhsWidget_create. All widget creation should be
 * handled by EhsWidget_create.
 */
ehs_bool EhsWidgetImage_create(EhsWidgetClass* pWidget)
{
    // load the image specified at design time in IGB
    ehs_bool bRet;
    if (EHS_WIDGET_IMAGE(pWidget).szFilename)
    {
        bRet = EhsWidgetImage_loadImage(pWidget);
    }
    else
    {
        bRet=EHS_TRUE;// This might be OK!
    }
    return bRet;
}

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy. All widget destruction should be
 * handled by EhsWidget_destroy.
 */
void EhsWidgetImage_destroy(EhsWidgetClass* pWidget)
{
    switch(EHS_WIDGET_IMAGE(pWidget).eImageFormat)
    {
    case EHS_WIDGET_IMAGE_FORMAT_GIF:
        EhsWidgetImageGif_destroy(pWidget);
        break;
    case EHS_WIDGET_IMAGE_FORMAT_JPG:
        EhsWidgetImageJpg_destroy(pWidget);
        break;
    case EHS_WIDGET_IMAGE_FORMAT_PNG:
        EhsWidgetImagePng_destroy(pWidget);
        break;
    /*case EHS_WIDGET_IMAGE_FORMAT_SVG:
    	EhsWidgetImageSvg_destroy(pWidget);
    	break;*/
    default:
        break;
    }
}

/**
 * Set the current filename - creates EHS_WIDGET_IMAGE(pWidget).szFilename from szfilename
 *
 * @todo the dynamic bit here seems to be totally useless - remove ?
 */
void EhsWidgetImage_setFilename(EhsWidgetClass* pWidget, const ehs_char* szFilename, ehs_bool dynamicFilename)
{
    /* release old filename - if we have one */
    if ( EHS_WIDGET_IMAGE(pWidget).szFilename)
    {
        EhsHMem_tempFree(EHS_WIDGET_IMAGE(pWidget).szFilename);
    }
    EHS_WIDGET_IMAGE(pWidget).szFilename = NULL;
    /* invariant: pWidget->szFilename = NULL */

    /* allocate new filename - if we have one */
    EHS_WIDGET_IMAGE(pWidget).bDynamicFilename = dynamicFilename;
    if (szFilename)
    {
        EHS_WIDGET_IMAGE(pWidget).szFilename = EhsHMem_tempAlloc(EhsStrlen(szFilename)+1);
        EhsStrcpy(EHS_WIDGET_IMAGE(pWidget).szFilename,szFilename);
    }

}

/**
 * returns TRUE if image should be loaded from the app dir, FALSE if should be loaded from the user dir
 */
ehs_bool EhsWidgetImage_loadFileFromAppDir(EhsWidgetClass* pWidget)
{
    ehs_bool bLoadFromAppDir;

    // use parameter unless the filename is specified in IGB (instead of at runtime) in which case always load from app dir
    bLoadFromAppDir = EHS_WIDGET_IMAGE(pWidget).bLoadImageFromAppDir;
    if (!EHS_WIDGET_IMAGE(pWidget).bDynamicFilename)
    {
        bLoadFromAppDir = EHS_TRUE;
    }
    return bLoadFromAppDir;
}

/**
 * Load the image-specific data into the widget from the specified file.
 *
 * Updated the source rectangle info
 */
ehs_bool EhsWidgetImage_loadImage(EhsWidgetClass* pWidget)
{
    ehs_FILE *file;
    ehs_bool bRet = EHS_FALSE;
    ehs_char szExtension[5] = {'\0'};
    ehs_uint32 nStrLen, i;
    ehs_char szFilename[EHS_FILENAME_SIZE];
    EhsStrcpy(szFilename, EHS_WIDGET_IMAGE(pWidget).szFilename);

    // if filename is set at runtime, try to open file
    if (EHS_WIDGET_IMAGE(pWidget).bDynamicFilename)
    {
        if (EHS_WIDGET_IMAGE(pWidget).bLoadImageFromAppDir)
        {
            file = Ehs_AppFopen(szFilename, "rb");
        }
        else
        {
            file = Ehs_UserFopen(szFilename, "rb");
        }
        // if no file, fallback to backup file
        if (file == NULL)
        {
            EHS_WIDGET_IMAGE(pWidget).bDynamicFilename = EHS_FALSE;
            if (EHS_WIDGET_IMAGE(pWidget).szFilename) EhsHMem_tempFree(EHS_WIDGET_IMAGE(pWidget).szFilename);
            EHS_WIDGET_IMAGE(pWidget).szFilename = EhsHMem_tempAlloc(EhsStrlen(EHS_WIDGET_IMAGE(pWidget).szBackupFilename)+1);
            if (EHS_WIDGET_IMAGE(pWidget).szFilename)
            {
                EhsStrcpy(EHS_WIDGET_IMAGE(pWidget).szFilename, EHS_WIDGET_IMAGE(pWidget).szBackupFilename);
                EhsStrcpy(szFilename, EHS_WIDGET_IMAGE(pWidget).szBackupFilename);
            }
            else
            {
                EhsStrcpy(szFilename, "");
                EHS_WIDGET_IMAGE(pWidget).szFilename=NULL;

            }
        }
        else
        {
            // close file
            EhsFclose(file);
        }
    }
    nStrLen = strlen(szFilename);
    for (i=nStrLen-4; i<nStrLen; i++)
    {
        szExtension[i-(nStrLen-4)] = szFilename[i];
    }

    if (szExtension != NULL)
    {
        if (EhsStricmp(".gif",szExtension) == 0)
        {
            EHS_WIDGET_IMAGE(pWidget).eImageFormat = EHS_WIDGET_IMAGE_FORMAT_GIF;
            pWidget->pfDrawFunc = EhsWidgetImageGif_draw;
            bRet = EhsWidgetImageGif_load(pWidget,szFilename);
        }
        else if (EhsStricmp(".png",szExtension) == 0)
        {
            EHS_WIDGET_IMAGE(pWidget).eImageFormat = EHS_WIDGET_IMAGE_FORMAT_PNG;
            pWidget->pfDrawFunc = EhsWidgetImagePng_draw;
            bRet = EhsWidgetImagePng_load(pWidget,szFilename);
        }
        else if (EhsStricmp(".jpg",szExtension) == 0)
        {
            EHS_WIDGET_IMAGE(pWidget).eImageFormat = EHS_WIDGET_IMAGE_FORMAT_JPG;
            pWidget->pfDrawFunc = EhsWidgetImageJpg_draw;
            bRet = EhsWidgetImageJpg_load(pWidget,szFilename);
        }
        //@todo re-add svg support
        /*else if (EhsStricmp(".svg",szExtension) == 0) todo: re-enable svg; leaving this in without svg breaks linking
        {
        	EHS_WIDGET_IMAGE(pWidget).eImageFormat = EHS_WIDGET_IMAGE_FORMAT_SVG;
        	pWidget->pfDrawFunc = EhsWidgetImageSvg_draw;
        	bRet = EhsWidgetImageSvg_load(pWidget,szFilename);
        }*/
        else
        {
            EhsError(EHS_MSG_ERROR_WIDGET_TYPE_NOT_RECOG(szExtension));
            bRet = EHS_FALSE;
        }
    }
    else
    {
        EhsError(EHS_MSG_ERROR_WIDGET_TYPE_NOT_RECOG(szFilename));
        bRet = EHS_FALSE;
    }

    /* load was successful, update the z-order */
    if (bRet)
    {
        {
            EhsWidgetTable_updateZOrder(&EhsWidgetTable, pWidget);
        }

    }
    return bRet;
}

/**
 * Fade the image according to an opacity value
 *
 * @return true if the image needs to be redrawn (because it's appearance has changed)
 */
ehs_bool EhsWidgetImage_fade(EhsWidgetClass* pWidget, ehs_uint8 nOpacity)
{
    ehs_bool bChanged = EHS_FALSE;

    /* determine the new opacity required by this widget */
    ehs_uint16 nCalcOpacity = ((ehs_uint16)(EHS_WIDGET_IMAGE(pWidget).nOrigImageAlpha * nOpacity)) / 256; //PPP: this was a division by 255

    /* is it any different to the opacity we have already? */
    if (nCalcOpacity != EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha)
    {
        EHS_WIDGET_IMAGE(pWidget).nCurrentImageAlpha = (ehs_uint8)nCalcOpacity;
        bChanged = EHS_TRUE;
    }

    return bChanged;
}

/**
 * Convert pixels from ARGB to RGBA
 */
void EhsWidgetImage_RGBA_to_BGRA(ehs_uint8* pPixels, ehs_uint32 numPixels)
{
    ehs_uint8 red;
    ehs_uint32 i;

    /*Iterate through all pixels*/
    for(i=0; i<numPixels*4; i=i+4)
    {
        red = pPixels[i]; /*Copy red pixel*/
        pPixels[i] = pPixels[i+2]; /*Set red pixel to blue pixel*/
        pPixels[i+2] = red; /*Set blue pixel to red pixel*/
    }
}


