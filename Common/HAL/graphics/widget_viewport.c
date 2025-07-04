/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
****************************************************************/


/** @file widget_viewport.c
 * Definitions for the patch widget handling functions
 *
 * @author: inx limited
 *
 */
//#define EHSL_MODULE_ID (EHSH_LOG_MODULE_GRAPHICS)

#include "widget.h"
#include "messages.h"

#include "hal_viewport.h"
#include "hal-api.h"

#ifdef EHS_GUI_SUPPORT_MODE_B
#include "target_viewport_modeB.h"
#endif

/**
 * Define the global viewport info
 */
EhsViewportInformation EhsPrimaryViewportInfo= {0};

#define EHSGRAPHICSINFOFILE "graphics.nfo"
#define EHSGRAPHICS_XCOORD "xcoord"
#define EHSGRAPHICS_YCOORD "ycoord"
#define EHSGRAPHICS_WIDTH "width"
#define EHSGRAPHICS_HEIGHT "height"
#define EHSGRAPHICS_RED "red"
#define EHSGRAPHICS_GREEN "green"
#define EHSGRAPHICS_BLUE "blue"
#define EHSGRAPHICS_ALPHA "alpha"
#define EHSGRAPHICS_HASFRAME "hasFrame"
#define EHSGRAPHICS_ZORDER "zorder"

/**
 * Create a new primary viewport with the system default settings.
 * These are read from file.
 * @todo move the file reading somewhere else. We don't want a dependency on a file system and file in the viewport code.
 */
void EhsPrimaryViewportInfo_new()
{
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_FALSE; //dktodo - remove, not needed now use file
    EhsPrimaryViewportInfo.nX = 0;
    EhsPrimaryViewportInfo.nY = 0;
    EhsPrimaryViewportInfo.nWidth  = EHS_CONFIG_DISPLAY_WIDTH; 	//these are defined in the platform config header file
    EhsPrimaryViewportInfo.nHeight = EHS_CONFIG_DISPLAY_HEIGHT; //these are defined in the platform config header file
    EhsPrimaryViewportInfo.nColour.sComp.nRed = 20;				//dk - gstreamer is making keying out rgb=0,0,0 and making window transparent
    EhsPrimaryViewportInfo.nColour.sComp.nGreen = 20;
    EhsPrimaryViewportInfo.nColour.sComp.nBlue = 20;
    EhsPrimaryViewportInfo.nColour.sComp.nAlpha = 255;			//always black and opaque defaults
    EhsPrimaryViewportInfo.bHasFrame = EHS_FALSE;
    EhsPrimaryViewportInfo.nZOrder = 2;							// default to top-most
}

/* To depricate: this to read from an XML string not a file. In fact we don't really want a dependency on XML either, so we should change to a
 * a C API function for configuration otherwise.
 */
void getEHSDefaultConfigFromFile()
{
    ehs_FILE* EhsGraphicsFile;
    EhsGraphicsFile = Ehs_SysFopen(EHSGRAPHICSINFOFILE,"r"); /* if sysdata doesn't exist baarf */
    if (!EhsGraphicsFile)
    {
        EHSH_LOG_INFO("Can't open graphics info file, using EHS defaults instead");
    }
    else
    {
        ehs_char returndata[EHS_STRING_LENGTH_MAX];
        char * element_start;
        char element_cropped[EHS_STRING_LENGTH_MAX];
        ehs_uint16 nVal;
        ehs_uint16 nRed = -1;
        ehs_uint16 nGreen = -1;
        ehs_uint16 nBlue = -1;

        // read to end of file
        while (EhsFgets(returndata,EHS_STRING_LENGTH_MAX,EhsGraphicsFile) != NULL)
        {

            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_XCOORD)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setX(nVal);

                }
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_YCOORD)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setY(nVal);
                   
                }
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_WIDTH)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setWidth(nVal);
                    
                }
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_HEIGHT)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setHeight(nVal);
                    
                }
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_RED)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsGetUint16FromString(&nRed,element_cropped);
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_GREEN)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsGetUint16FromString(&nGreen,element_cropped);
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_BLUE)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                EhsGetUint16FromString(&nBlue,element_cropped);
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_ALPHA)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setAlpha(nVal);
                    
                }
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_HASFRAME)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setHasFrame(nVal);
                }
            }
            if ((element_start = Ehs_ReadXMLTag(returndata, EHSGRAPHICS_ZORDER)))
            {
                Ehs_CopyXMLTagElement(element_cropped, element_start, EHS_STRING_LENGTH_MAX, EHS_TRUE);
                if (EhsGetUint16FromString(&nVal,element_cropped))
                {
                    EhsPrimaryViewportInfo_setZOrder(nVal);
                }
            }

        }

       
        if ((nRed >= 0) && (nGreen >= 0) && (nBlue >= 0))
        {
            EhsPrimaryViewportInfo_setColourRGB(nRed, nGreen, nBlue);
        }
        EhsFclose(EhsGraphicsFile);
    }
}

/**
 * gets the x coord of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getX()
{
    ehs_uint16 nX;

    nX = EhsPrimaryViewportInfo.nX;
    return nX;
}

/**
 * sets the x coord of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setX(ehs_uint16 nX)
{

    EhsPrimaryViewportInfo.nX = nX;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the y coord of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getY()
{
    ehs_uint16 nY;

    nY = EhsPrimaryViewportInfo.nY;
    return nY;
}

/**
 * sets the y coord of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setY(ehs_uint16 nY)
{

    EhsPrimaryViewportInfo.nY = nY;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the width of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getWidth()
{
    ehs_uint16 nWidth;

    nWidth = EhsPrimaryViewportInfo.nWidth;
    return nWidth;
}

/**
 * sets the width of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setWidth(ehs_uint16 nWidth)
{

    EhsPrimaryViewportInfo.nWidth = nWidth;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the height of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getHeight()
{
    ehs_uint16 nHeight;

    nHeight = EhsPrimaryViewportInfo.nHeight;
    return nHeight;
}

/**
 * sets the height of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setHeight(ehs_uint16 nHeight)
{

    EhsPrimaryViewportInfo.nHeight = nHeight;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the colour of the primary viewport - currently there is only 1 viewport
 */
EhsGraphicsColourClass EhsPrimaryViewportInfo_getColour()
{
    EhsGraphicsColourClass nColour;

    nColour = EhsPrimaryViewportInfo.nColour;
    return nColour;
}

/**
 * sets the colour of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setColour(EhsGraphicsColourClass nColour)
{

    EhsPrimaryViewportInfo.nColour = nColour;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * sets the colour of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setColourRGB(ehs_uint16 nRed, ehs_uint16 nGreen, ehs_uint16 nBlue)
{

    EhsPrimaryViewportInfo.nColour.sComp.nRed = nRed;
    EhsPrimaryViewportInfo.nColour.sComp.nGreen = nGreen;
    EhsPrimaryViewportInfo.nColour.sComp.nBlue = nBlue;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the alpha transparency of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getAlpha()
{
    ehs_uint16 nAlpha;

    nAlpha = EhsPrimaryViewportInfo.nColour.sComp.nAlpha;
    return nAlpha;
}

/**
 * sets the alpha transparency of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setAlpha(ehs_uint16 nAlpha)
{

    EhsPrimaryViewportInfo.nColour.sComp.nAlpha = nAlpha;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the hasFrame of the primary viewport - currently there is only 1 viewport
 */
ehs_bool EhsPrimaryViewportInfo_getHasFrame()
{
    ehs_bool bHasFrame;

    bHasFrame = EhsPrimaryViewportInfo.bHasFrame;
    return bHasFrame;
}

/**
 * sets the hasFrame of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setHasFrame(ehs_bool bHasFrame)
{

    EhsPrimaryViewportInfo.bHasFrame = bHasFrame;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}

/**
 * gets the ZOrder of the primary viewport - currently there is only 1 viewport
 */
ehs_uint16 EhsPrimaryViewportInfo_getZOrder()
{
    ehs_uint16 nZOrder;

    nZOrder = EhsPrimaryViewportInfo.nZOrder;
    return nZOrder;
}

/**
 * sets the ZOrder of the primary viewport - currently there is only 1 viewport
 */
void EhsPrimaryViewportInfo_setZOrder(ehs_uint16 nZOrder)
{

    EhsPrimaryViewportInfo.nZOrder = nZOrder;
    EhsPrimaryViewportInfo.bDefaultsChanged = EHS_TRUE;
}



/**
 * Insert a widget itno the Z-ordered list of widgets. This is called
 * after a widget has been initialized, but before the index to the widget
 * table has been updated
 */
EHS_GLOBAL void EhsWidgetTable_updateZOrder(EhsWidgetTableClass* pWidgetTable, EhsWidgetClass* pWidget);

/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create via the widget's
 * pfCreate function.
 */
EHS_LOCAL ehs_bool EhsWidgetViewport_create(struct EhsWidgetStruct* pWidget);

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy via the widget's
 * pfDestroy function.
 */
EHS_LOCAL void EhsWidgetViewport_destroy(struct EhsWidgetStruct* pWidget);

/**
 * Fade the patch according to an opacity value
 *
 * @return true if the textbox needs to be redrawn (because it's appearance has changed)
 */
EHS_GLOBAL ehs_bool EhsWidgetViewport_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity, ehs_uint8 nRed, ehs_uint8 nGreen, ehs_uint8 nBlue);

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 *
 * NOTE: This function must only be called by EhsWidget_draw via the widget's
 * pfDraw function.
 * callers listed below.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */
EHS_LOCAL void EhsWidgetViewport_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect);

/**
 * Initialise the widget with the general widget and textbox specific properties including
 * position, dimensions, z-ordering and colour.
 *
 * @param[in] pBounds Bounding rectangle for the textbox
 * @param[in] nZ Z order for widget
 * @param[in] xColour Widget's colour
 * @return pointer to initialised widget, or NULL
 */
ehs_bool created = 0;

/**
 * Obtain the textbox specific properties of EhsWidgetTextbox
 */
//#define EHS_WIDGET_VIEWPORT(pWidget) ((pWidget)->specific.textbox) //@todo this needs to go back to patch from textbox



EhsWidgetClass* EhsWidgetViewport_init(const EhsGraphicsRectangleClass* pBounds, ehs_uint16 nZ, EhsGraphicsColourClass xColour)
{
    EhsWidgetClass* pWidget = EhsWidgetTable_new(&EhsWidgetTable);

    if (pWidget)
    {
#ifndef EHS_GUI_SUPPORT_MODE_B
        pWidget->eWidgetKind = EHS_WIDGET_KIND_VIEWPORT; //this stops it being composited onto itself
#endif
        pWidget->nState = EHS_WIDGET_STATE_INIT;
        EhsMemcpy(&(pWidget->xOrigRect),pBounds,sizeof(EhsGraphicsRectangleClass));
        EhsMemcpy(&(pWidget->xCurRect),pBounds,sizeof(EhsGraphicsRectangleClass));
        EhsMemcpy(&(pWidget->xDesignRect),pBounds,sizeof(EhsGraphicsRectangleClass));

        pWidget->nZ = nZ;
        pWidget->pfCreateFunc = EhsWidgetViewport_create; //@todo these should be removed?
        pWidget->pfDestroyFunc = EhsWidgetViewport_destroy; //@todo these should be removed?
        pWidget->pfFadeFunc = (fadeFunc_t) EhsWidgetViewport_fade;
        pWidget->pfDrawFunc = EhsWidgetViewport_draw;
        pWidget->specificWidgetType.patch.xColour = xColour;
        pWidget->specificWidgetType.patch.nBaseAlpha = xColour.sComp.nAlpha; /* @todo this needs to revert patch DROP the BG part of the elements */
        EhsWidgetTable_updateZOrder(&EhsWidgetTable, pWidget);
        pWidget->bMaintainAspectRatio = EHS_FALSE; // not implemented for viewport
        pWidget->bCaptureClicksIgnoringZOrder = EHS_FALSE; // not implemented for viewport
    }

    return pWidget;
}



/**
 * Create the widget. This is a necessary step prior to showing the widget.
 * NOTE: This must only be called by EhsWidget_create. All widget creation should be
 * handled by EhsWidget_create.
 */
ehs_bool EhsWidgetViewport_create(EhsWidgetClass* pWidget)
{
//#ifdef NOT_DEFINED // This sgould be a visibility thing for now
    if(!created)
    {
        created = 1;
    }
//#endif
    return EHS_TRUE;
}

/**
 * Destroy the widget. Required after the widget has been finished with.
 * NOTE: This must only be called by EhsWidget_destroy. All widget destruction should be
 * handled by EhsWidget_destroy.
 */
void EhsWidgetViewport_destroy(EhsWidgetClass* pWidget)
{
    /* no specific widget destruction activities are required */
}

/**
 * Cause the widget to be displayed (assuming it's already been created). If
 * it hasn't been created, there are no ill effects.
 *
 * NOTE: This function must only be called by EhsWidget_draw.
 *
 * @param pWidget. Widget to display.
 * @param pViewport. Viewport to display the widget on.
 * @param pClipRect Specifies the bounds for drawing the widget
 */
void EhsWidgetViewport_draw(struct EhsWidgetStruct* pWidget, EhsTVClass* pViewport, EhsGraphicsRectangleClass* pClipRect)
{

}

/**
 * Fade the viewport (and change any colours) according to a set of colour values
 *
 * @return true if successful.
 */
ehs_bool EhsWidgetViewport_fade(struct EhsWidgetStruct* pWidget, ehs_uint8 nOpacity, ehs_uint8 nRed, ehs_uint8 nGreen, ehs_uint8 nBlue)
{
 #ifdef EHS_DOES_THIS_WORK
    ehs_bool bChanged = EHS_FALSE;
    EhsGraphicsColourClass nColour;
    /* determine the new foreground and background opacity required by this widget */
    ehs_uint16 nCalcOpacity = ((pWidget->specificWidgetType.patch.nBaseAlpha* nOpacity) / 255);
    /* scale down to 8-bit */
    ehs_uint8 nOpacityByte = (ehs_uint8)nCalcOpacity;

    /*Put 8 bit colour values together in to one 32bit value*/
    nColour.sComp.nRed = nRed;
    nColour.sComp.nGreen = nGreen;
    nColour.sComp.nBlue = nBlue;
    nColour.sComp.nAlpha = nOpacityByte;
  
    /*If any of the values are different to what is already store then tell the viewport to change colour */  
    if ((nOpacityByte != pWidget->specificWidgetType.patch.xColour.sComp.nAlpha) 
        || (nRed !=      pWidget->specificWidgetType.patch.xColour.sComp.nRed) 
        || (nGreen !=    pWidget->specificWidgetType.patch.xColour.sComp.nGreen) 
        || (nBlue !=    pWidget->specificWidgetType.patch.xColour.sComp.nBlue))
    {
        pWidget->specificWidgetType.patch.xColour.sComp.nAlpha = nOpacityByte;
        pWidget->specificWidgetType.patch.xColour.sComp.nRed = nRed;
        pWidget->specificWidgetType.patch.xColour.sComp.nGreen = nGreen;
        pWidget->specificWidgetType.patch.xColour.sComp.nBlue = nBlue;
        bChanged = EHS_TRUE;
    }
    EhsTV_fade(&EhsTV, pWidget->specificWidgetType.patch.xColour);
    #endif
    return EHS_TRUE;
}

/**
 * Request that the viewport be moved/resized
 */
ehs_bool EhsWidgetViewport_update(EhsWidgetClass* pWidget, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nDeltaWid, EhsDataflowIntType nDeltaHt)
{
    EhsTV_move(&EhsTV, nX, nY, nDeltaWid, nDeltaHt);
    return EHS_TRUE;
}


/* This sets the window to absolute reference position */
ehs_bool EhsWidgetViewport_setwindow(EhsWidgetClass* pWidget, EhsDataflowIntType nX, EhsDataflowIntType nY, EhsDataflowIntType nWid, EhsDataflowIntType nHt)
{
    EhsTV_setwindow(&EhsTV, EHS_TRUE, nX, nY, nWid, nHt);
    return EHS_TRUE;
}

/* The following function sets the viewport to the default value in case a viewport widget is not in the application
 * This may not be needed as the hardware init should use defaults and create a window in any case.
 */

ehs_bool EhsWidgetViewport_default_config()
{

    // NB - target_viewport:EhsTV_init() defines viewRectangle which uses the same defaults to set paintable area in the viewport
    EhsPrimaryViewportInfo_new();
#ifndef EHS_ESP32_SUPPORT
    getEHSDefaultConfigFromFile();
#endif
    EhsTV_setwindow(&EhsTV, EHS_FALSE, EhsPrimaryViewportInfo.nX, EhsPrimaryViewportInfo.nY, EhsPrimaryViewportInfo.nWidth, EhsPrimaryViewportInfo.nHeight);
    EhsTV_fade(&EhsTV, EhsPrimaryViewportInfo.nColour);
    EhsTV_showFrame(&EhsTV, EhsPrimaryViewportInfo.bHasFrame);
    EhsTV_setZOrder(EhsPrimaryViewportInfo.nZOrder);
    return EHS_TRUE;
}




/**
 * Request that the viewport be hidden.
 */
ehs_bool EhsWidgetViewport_hide(struct EhsWidgetStruct* pWidget)
{
    EhsTV_hideViewport();
    return EHS_TRUE;
}

/**
 * Request that the viewport be shown.
 * @todo nothing calls this?
 */
ehs_bool EhsWidgetViewport_show(struct EhsWidgetStruct* pWidget)
{
    EhsTV_showViewport(pWidget->nZ);
    return EHS_TRUE;
}


/** @brief sets the targetview parameters to full screen and returns the full screen size in parameters.
 * this does not do any actual graphical updates
 */
ehs_bool EhsWidgetsetToScreenSize(ehs_sint32 *nScreenWidth, ehs_sint32 *nScreenHeight)
{

    ehs_bool ret = EhsTV_getScreenSize(nScreenWidth,  nScreenHeight);

    if (ret)
    {
        EhsPrimaryViewportInfo_setWidth((ehs_uint16) *nScreenWidth);
        EhsPrimaryViewportInfo_setHeight((ehs_uint16) *nScreenHeight);
    }
    return ret;

}

EHS_GLOBAL ehs_bool EhsWidgetViewport_cleanup(struct EhsWidgetStruct* pWidget)
{
    if(pWidget)
    {
#ifdef EHS_GUI_SUPPORT_MODE_B
        EhsTargetWidgetUi_viewport_cleanup(pWidget);
#endif
        return EHS_TRUE;
    }
    return EHS_FALSE;
}



