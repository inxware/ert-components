/*
 * Filename : BFSVGDOM.h
 *
 *  Version : 6.4.0_2271s
 *     Date : 20080508-20.49.08
 *
 *
 * (c) COPYRIGHT 2002- 2008 Open Text Corp. - BitFlash Division. ALL RIGHTS RESERVED
 *
 *
 * This software is the property and confidential information of Open Text and is furnished under
 * license by BitFlash Inc.  This software may be used only in accordance with the terms of said license.
 *
 * This software may not be copied, transmitted, provided to or otherwise made available to any other person,
 * company, corporation or other entity except as specified in the terms of said license.
 *
 * This copyright notice may not be removed or altered without the prior written permission of Open Text.
 *
 *
 */
/** \file BFSVGDOM.h
 *
 * This file defines the uDOM implementation of SVG features.
 * Initial Author: Andrew Emmons
 *
*/


#ifndef BITFLASH_SDVG_DOM_H_
#define BITFLASH_SDVG_DOM_H_


#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/* undefine to save a little footprint if ascii traits not needed */
#define BFDOM_HAS_ASCII_TRAITS


/**
 * \addtogroup BitFlash Module:BitFlash
 * @{
 */

/**
 * \defgroup DOMFloat DOMFloat
 *
 * @{
 * This is a 16.16 fixed-point 32-bit integer. Conversion functions are present to convert from
 *    fixed point to float, if <b>BF_FLOAT</b> is defined.
 */

/** BFDOM_Float is a 16.16 fixed point value
 */
typedef BFFixedPoint BFDOM_Float;

/** <b>binding-specific</b> - returns a BFDOM_Float along with a possible exception
 */
typedef struct
{
    BFDOM_Float             value;
    BFDOM_DOMException      exceptionCode;

} BFDOM_FloatEx;


/** <b>binding-specific</b> - get the integer portion of the fixed point float value
 * \param f a fixed-point number
 * \return the integer value of the fixed-point number
 */
BF_PLAYER_IMPORT BF_INT16             BFDOM_Float_getInt( BFDOM_Float f );

/** <b>binding-specific</b> - get the fraction portion of the fixed point float value.
 * \param f a fixed-point number
 * \return the fraction value of the fixed-point number. The fraction is the returned value/65535
 */
BF_PLAYER_IMPORT BF_UINT16            BFDOM_Float_getFraction( BFDOM_Float f );

/** <b>binding-specific</b> - only available when BF_FLOAT is defined.
 * \param uNumber a floating-point number
 * \return uNumber converted into a fixed-point value
 */
BF_PLAYER_IMPORT BFDOM_Float          BFDOM_Float_CreateFromFloat( float uNumber );

/** <b>binding-specific</b> - only available when BF_FLOAT is defined.
 * \param uNumber a double floating-point number
 * \return uNumber converted into a fixed-point value
 */
BF_PLAYER_IMPORT BFDOM_Float          BFDOM_Float_CreateFromDouble( double uNumber );

/** <b>binding-specific</b> - only available when BF_FLOAT is defined.
 * \param uNumber a fixed-point number
 * \return uNumber converted into a floating-point value
 */
BF_PLAYER_IMPORT float                BFDOM_Float_GetFloat( BFDOM_Float uNumber );

/** <b>binding-specific</b> - returns float seconds as UINT32 ms
 * \param f a fixed-point number representing seconds
 * \return f converted into an integer value represneting ms
 */
BF_PLAYER_IMPORT BF_UINT32            BFDOM_Float_getMs( BFDOM_Float f );
/** @} */
/** @} Module:BitFlash */

/**
 * \defgroup smil Module:smil
 * @{
 */

/**
* \defgroup ElementTimeControl ElementTimeControl
* <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#smil__ElementTimeControl"><b>See</b> <i>W3C SVGT uDOM</i></A>
* @{
*/
typedef BFDOM_Element       BFDOM_ElementTimeControl;

BF_PLAYER_IMPORT BF_BOOL              BFDOM_ElementTimeControl_beginElement( BFDOM_ElementTimeControl *pElement );
BF_PLAYER_IMPORT BF_BOOL              BFDOM_ElementTimeControl_beginElementAt( BFDOM_ElementTimeControl *pElement, BFDOM_Float offset );
BF_PLAYER_IMPORT BF_BOOL              BFDOM_ElementTimeControl_endElement( BFDOM_ElementTimeControl *pElement );
BF_PLAYER_IMPORT BF_BOOL              BFDOM_ElementTimeControl_endElementAt( BFDOM_ElementTimeControl *pElement, BFDOM_Float offset );
/** @} */

/**
 * \defgroup TimeEvent TimeEvent
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__TimeEvent"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef BFDOM_Event BFDOM_TimeEvent;

BF_PLAYER_IMPORT BF_UINT32                  BFDOM_TimeEvent_GetDetail( BFDOM_TimeEvent *pEvent );
/** @} */

/** @} Module:smil */


/**
 * \defgroup global Module:global
 * @{
 */

/**
 * \addtogroup GlobalException
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#global__GlobalException"><b>See</b> <i>W3C SVGT uDOM<i></A>
 * There is a special exception value GLOBAL_NO_EXCEPTION which is part of the W3C exception enumeration. This
 *   is used to indicate that no exception has occurred.
 * @{
 */
#define     GLOBAL_NOT_CONNECTED_ERR              1
#define     GLOBAL_ENCODING_ERR                   2
#define     GLOBAL_DENIED_ERR                     3
#define     GLOBAL_UNKNOWN_ERR                    4
#define     GLOBAL_NO_EXCEPTION                  99

typedef BF_UINT8 BFDOM_GlobalException;
/** @} */

/** @} Module:global */




/**
 * \defgroup svg Module:SVG
 * @{
 */


/**
 * \addtogroup AsyncURLStatus
 * @{
 */
typedef struct
{
    BF_BOOL         success;
    BFDOM_String    contentType;
    BFDOM_String    content;

}  BFDOM_AsyncURLStatus;
/** @} */


/**
 * \addtogroup AsyncStatusCallback
 * @{
 */
typedef void (*BFDOM_AsyncStatusCallback)( void *pCallbackData, BFDOM_AsyncURLStatus *pStatus );
/** @} */


/**
 * \addtogroup SVGGlobal
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGGlobal"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef BFSVG_Global        BFDOM_SVGGlobal;
/** @} */


/**
 * \addtogroup SVGRGBColor
 * @{
 */
typedef struct
{
    BF_UINT32      m_red;
    BF_UINT32      m_green;
    BF_UINT32      m_blue;

} BFDOM_SVGRGBColor;

/** <b>binding-specific</b> - returns a BFDOM_SVGRGBColor along with a possible exception */
typedef struct
{
    BFDOM_SVGRGBColor       *pColor;
    BFDOM_DOMException      exceptionCode;

} BFDOM_SVGRGBColorEx;
/** @} */


/**
 * \addtogroup SVGRect SVGRect
 * @{
 */
typedef struct
{
    BFDOM_Float    m_x;
    BFDOM_Float    m_y;
    BFDOM_Float    m_width;
    BFDOM_Float    m_height;

} BFDOM_SVGRect;

/** <b>binding-specific</b> - returns a BFDOM_SVGRect along with a possible exception */
typedef struct
{
    BFDOM_SVGRect           *pRect;
    BFDOM_DOMException      exceptionCode;

} BFDOM_SVGRectEx;
/** @} */


/**
 * \addtogroup SVGPoint
 * @{
 */
typedef struct
{
    BFDOM_Float    m_x;
    BFDOM_Float    m_y;

} BFDOM_SVGPoint;

/** <b>binding-specific</b> - returns a BFDOM_SVGPoint along with a possible exception */
typedef struct
{
    BFDOM_SVGPoint           *pPoint;
    BFDOM_DOMException      exceptionCode;

} BFDOM_SVGPointEx;
/** @} */


/**
 * \addtogroup SVGPath SVGPath
 * @{
 */
typedef enum
{
    BFDOM_SVGPATH_MOVE_TO = 77,  /* 'M' */
    BFDOM_SVGPATH_LINE_TO = 76,  /* 'L' */
    BFDOM_SVGPATH_CURVE_TO = 67, /* 'C' */
    BFDOM_SVGPATH_QUAD_TO = 81,  /* 'Q' */
    BFDOM_SVGPATH_CLOSE = 90     /* 'Z' */

} BFDOM_SVGPathSegment;

/** <b>binding-specific</b> - returns a BFDOM_SVGPathSegment along with a possible exception */
typedef struct
{
    BFDOM_SVGPathSegment    value;
    BFDOM_DOMException      exceptionCode;

} BFDOM_SVGPathSegmentEx;

/** Alias to internal BF Type - path structure identical to the OpenVG standard */
typedef BFPathObject      *BFDOM_SVGPath;

/** <b>binding-specific</b> - returns a BFDOM_SVGPath along with a possible exception */
typedef struct
{
    BFDOM_SVGPath           path;
    BFDOM_DOMException      exceptionCode;

} BFDOM_SVGPathEx;
/** @} */


/**
 * \addtogroup SVGMatrix
 * @{
 * Memory management: Any method returning #BFDOM_SVGMatrix must free the returned
 *    values when not needed.
 */
typedef BFMatrix2D*         BFDOM_SVGMatrix;

/** <b>binding-specific</b> - returns a BFDOM_SVGMatrixEx along with a possible exception */
typedef struct
{
    BFDOM_SVGMatrix         matrix;
    BFDOM_DOMException      exceptionCode;

} BFDOM_SVGMatrixEx;

typedef enum
{
    BFDOM_SVGMATRIX_INVALID = -1,  /* this is necessary to ensure the enum type is signed and thus allow comparisons
                                    * like 'i > BFDOM_SVGMATRIX_A' to compile without warning (i.e. RVCT) */
    BFDOM_SVGMATRIX_A,
    BFDOM_SVGMATRIX_B,
    BFDOM_SVGMATRIX_C,
    BFDOM_SVGMATRIX_D,
    BFDOM_SVGMATRIX_E,
    BFDOM_SVGMATRIX_F

} BFDOM_SVGMatrixComponent;

/** <b>binding-specific</b> - to reduce stack usage */
typedef struct
{
    BFDOM_Float a;
    BFDOM_Float b;
    BFDOM_Float c;
    BFDOM_Float d;
    BFDOM_Float e;
    BFDOM_Float f;

} BFDOM_SVGMatrixCreate;
/** @} */


/**
 * \addtogroup SVGElement SVGElement
 * @{
 */
typedef BFDOM_Element       BFDOM_SVGElement;

/** @} */


/**
 * \defgroup SVGException SVGException
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGException"><b>See</b> <i>W3C SVGT uDOM<i></A>
 * There is a special exception value SVG_NO_EXCEPTION which is part of the W3C exception enumeration. This
 *   is used to indicate that no exception has occurred.
 * @{
 */
#define     SVG_WRONG_TYPE_ERR              0
#define     SVG_INVALID_VALUE_ERR           1
#define     SVG_MATRIX_NOT_INVERTABLE       2
#define     SVG_NO_EXCEPTION               99

typedef BF_UINT8 BFDOM_SVGException;
/** @} */


/**
 * \defgroup SVGDocument SVGDocument
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGDocument"><b>See</b><i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef BFDOM_Document      BFDOM_SVGDocument;


/**
 * <b>binding-specific</b> - get SVGGlobal object
 * \param pDocument a document pointer
 * \return the SVGGlobal interface
 */
BF_PLAYER_IMPORT BFDOM_SVGGlobal *BFDOM_SVGDocument_getSVGGlobal( BFDOM_SVGDocument *pDocument );
/** @} */


/**
 * \defgroup SVGElementInstance SVGElementInstance
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGElementInstance"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef struct
{
    BFDOM_SVGElement    *correspondingElement;
    BFDOM_SVGElement    *correspondingUseElement;

} BFDOM_SVGElementInstance;


/** <b>binding-specific</b> - returns a BFDOM_SVGElementInstance along with a possible exception */
typedef struct
{
    BFDOM_SVGElementInstance  *pElementInstance;
    BFDOM_DOMException        exceptionCode;

} BFDOM_SVGElementInstanceEx;


/** <b>binding-specific</b> - returns a pointer to SVGElementInstance. Must be called from within an event listener handler.
 * \param pMemCtx a memory context
 * \param pEvent an event pointer
 * \return a pointer to the SVGElementInstance interface, which must be freed. Returns 0 if event was not triggered by a used element.
*/
BF_PLAYER_IMPORT BFDOM_SVGElementInstanceEx  BFDOM_SVGElementInstance_getInterface( BFHeapContext *pMemCtx, BFDOM_Event *pEvent );
/** @} */


/**
 * \defgroup SVGSVGElement SVGSVGElement
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGSVGElement"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef BFDOM_Element       BFDOM_SVGSVGElement;

#define BFDOM_NAV_AUTO         1
#define BFDOM_NAV_NEXT         2
#define BFDOM_NAV_PREV         3
#define BFDOM_NAV_UP           4
#define BFDOM_NAV_UP_RIGHT     5
#define BFDOM_NAV_RIGHT        6
#define BFDOM_NAV_DOWN_RIGHT   7
#define BFDOM_NAV_DOWN         8
#define BFDOM_NAV_DOWN_LEFT    9
#define BFDOM_NAV_LEFT        10
#define BFDOM_NAV_UP_LEFT     11

BF_PLAYER_IMPORT BFDOM_Float          BFDOM_SVGSVGElement_getCurrentScale( BFDOM_SVGSVGElement *pElement );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGSVGElement_setCurrentScale( BFDOM_SVGSVGElement *pElement, BFDOM_Float scale );
BF_PLAYER_IMPORT BFDOM_SVGPointEx     BFDOM_SVGSVGElement_getCurrentTranslate( BFDOM_SVGSVGElement *pElement);
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGSVGElement_setCurrentTranslate( BFDOM_SVGSVGElement *pElement, BFDOM_SVGPoint *translate );
BF_PLAYER_IMPORT BFDOM_Float          BFDOM_SVGSVGElement_getCurrentRotate( BFDOM_SVGSVGElement *pElement);
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGSVGElement_setCurrentRotate( BFDOM_SVGSVGElement *pElement, BFDOM_Float rotate );
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_SVGSVGElement_getViewport( BFDOM_SVGSVGElement *pElement );
BF_PLAYER_IMPORT BFDOM_Float          BFDOM_SVGSVGElement_getCurrentTime( BFDOM_SVGSVGElement *pElement );
BF_PLAYER_IMPORT void                 BFDOM_SVGSVGElement_setCurrentTime( BFDOM_SVGSVGElement *pElement, BFDOM_Float newTime );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGSVGElement_moveFocus( BFDOM_SVGSVGElement *pElement, BF_UINT8 motionType );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGSVGElement_setFocus( BFDOM_SVGSVGElement *pElement, BFDOM_Node *pObject );
BF_PLAYER_IMPORT BFDOM_Node          *BFDOM_SVGSVGElement_getCurrentFocusedObject( BFDOM_SVGSVGElement *pElement );

/** <b>binding-specific</b> ignores focusability - this is incompatible with SVGT 1.2, but not 1.1 and JSR-226
 * \param pElement an SVGSVGelement pointer
 * \param pObject a node pointer
 * \return BFDOM_DOMException
*/
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGSVGElement_focusOn( BFDOM_SVGSVGElement *pElement, BFDOM_Node *pObject );

/**
 * <b>binding-specific</b> - BFDOM_SVGMatrixCreate was added to facilitate less arguments to BFDOM_SVGSVGElement_createSVGMatrixComponents.
 * This is not conformant to the w3c spec but reduces stack usage.
 * \param pElement an SVGSVGelement pointer
 * \param pComponents a components structure
 * \return a created SVGMatrix. Must be freed with a call
 * to BFmemFree(), using the same memory context that was used to create the Player
*/
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_SVGSVGElement_createSVGMatrixComponents( BFDOM_SVGSVGElement *pElement, BFDOM_SVGMatrixCreate *pComponents );

/** SVGPath created with BFDOM_SVGSVGElement_createSVGPath must be deleted with BFDOM_SVGSVGElement_deleteSVGPath. */
BF_PLAYER_IMPORT BFDOM_SVGPathEx      BFDOM_SVGSVGElement_createSVGPath( BFDOM_SVGSVGElement *pElement );

BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_SVGSVGElement_createSVGRect( BFDOM_SVGSVGElement *pElement );

BF_PLAYER_IMPORT BFDOM_SVGRGBColorEx  BFDOM_SVGSVGElement_createSVGRGBColor( BFDOM_SVGSVGElement *pElement, BF_UINT32 red, BF_UINT32 green, BF_UINT32 blue );

BF_PLAYER_IMPORT BFDOM_SVGPointEx      BFDOM_SVGSVGElement_createSVGPoint( BFDOM_SVGSVGElement *pElement );

/** <b>binding-specific</b> - deletes path data created by BFDOM_SVGSVGElement_createSVGPath.
 * \param pMemCtx a memory context
 * \param path a SVGPath pointer
 * \return none
 */
BF_PLAYER_IMPORT void                 BFDOM_SVGSVGElement_deleteSVGPath( BFHeapContext *pMemCtx, BFDOM_SVGPath path );
/** @} */


/**
 * \defgroup SVGRGBColor SVGRGBcolor
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGRGBColor"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
/** @} */


/**
 * \defgroup SVGRect SVGRect
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGRect"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
/** @} */

/**
 * \defgroup SVGPoint SVGPoint
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGPoint"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 * <b>WARNING:</b> The matrixTransform function <b>modifies</b> the BFDOM_SVGPoint object pointed to by the
 * first parameter. The returned BFDOM_SVGPoint object is the same as this pointer. <b>No new BFDOM_SVGPoint object is created.</b>
 */
BF_PLAYER_IMPORT BFDOM_SVGPointEx     BFDOM_SVGPoint_matrixTransform( BFDOM_SVGPoint *pPoint, BFDOM_SVGMatrix matrix );
/** @} */

/**
 * \defgroup SVGPath SVGPath
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGPath"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 * Memory management: Any method returning #BFDOM_SVGPath must free the returned
 *    values when not needed.
 */
BF_PLAYER_IMPORT BF_UINT32            BFDOM_SVGPath_getNumberOfSegments( BFDOM_SVGPath path );
BF_PLAYER_IMPORT BFDOM_SVGPathSegmentEx BFDOM_SVGPath_getSegment( BFDOM_SVGPath path, BF_UINT32 index );
BF_PLAYER_IMPORT BFDOM_FloatEx        BFDOM_SVGPath_getSegmentParam( BFDOM_SVGPath path, BF_UINT32 cmdIndex, BF_UINT32 paramIndex );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGPath_moveTo( BFHeapContext *pMemCtx, BFDOM_SVGPath path, BFDOM_Float x, BFDOM_Float y );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGPath_lineTo( BFHeapContext *pMemCtx, BFDOM_SVGPath path, BFDOM_Float x, BFDOM_Float y );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGPath_quadTo( BFHeapContext *pMemCtx, BFDOM_SVGPath path, BFDOM_Float x1, BFDOM_Float y1, BFDOM_Float x2, BFDOM_Float y2 );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGPath_curveTo( BFHeapContext *pMemCtx, BFDOM_SVGPath path, BFDOM_Float x1, BFDOM_Float y1, BFDOM_Float x2, BFDOM_Float y2, BFDOM_Float x3, BFDOM_Float y3 );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGPath_close( BFHeapContext *pMemCtx, BFDOM_SVGPath path);
/** @} */




/**
 * \defgroup SVGMatrix SVGMatrix
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGMatrix"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 * <b>WARNING:</b> Matrix functions for invert, transform, scale and rotate <b>modify</b> the matrix pointed to by the
 * First parameter. The returned matrix is the same as this pointer. <b>No new matrix is created.</b>
 */
BF_PLAYER_IMPORT BFDOM_FloatEx        BFDOM_SVGMatrix_getComponent( BFDOM_SVGMatrix matrix, BFDOM_SVGMatrixComponent index );
BF_PLAYER_IMPORT BFDOM_SVGMatrix      BFDOM_SVGMatrix_multiply( BFDOM_SVGMatrix matrix, BFDOM_SVGMatrix secondMatrix );
BF_PLAYER_IMPORT BFDOM_SVGException   BFDOM_SVGMatrix_inverse( BFDOM_SVGMatrix matrix );
BF_PLAYER_IMPORT BFDOM_SVGMatrix      BFDOM_SVGMatrix_translate( BFDOM_SVGMatrix matrix, BFDOM_Float x, BFDOM_Float y );
BF_PLAYER_IMPORT BFDOM_SVGMatrix      BFDOM_SVGMatrix_scale( BFDOM_SVGMatrix matrix, BFDOM_Float scaleFactor );
BF_PLAYER_IMPORT BFDOM_SVGMatrix      BFDOM_SVGMatrix_rotate( BFDOM_SVGMatrix matrix, BFDOM_Float angle );
/** <b>binding-specific</b> - creates a copy of BFDOM_SVGMatrix - return val must be freed */
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_SVGMatrix_copy( BFHeapContext *pMemCtx, BFDOM_SVGMatrix matrixSrc );
/** @} */


/**
 * \defgroup SVGLocatable SVGLocatable
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGLocatable"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_SVGLocatable_getBBox( BFDOM_SVGElement *pElement );
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_SVGLocatable_getScreenCTM( BFDOM_SVGElement *pElement );
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_SVGLocatable_getScreenBBox( BFDOM_SVGElement *pElement );
/** @} */


/**
 * \defgroup TraitAccess TraitAccess
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__TraitAccess"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getTraitNS( BFDOM_SVGElement *pElement, BFDOM_String namespaceURI, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_FloatEx        BFDOM_TraitAccess_getFloatTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_TraitAccess_getMatrixTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_TraitAccess_getRectTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGPathEx      BFDOM_TraitAccess_getPathTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGRGBColorEx  BFDOM_TraitAccess_getRGBColorTrait( BFDOM_SVGElement *pElement, BFDOM_String name );

BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getPresentationTraitNS( BFDOM_SVGElement *pElement, BFDOM_String namespaceURI, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getPresentationTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_FloatEx        BFDOM_TraitAccess_getFloatPresentationTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_TraitAccess_getMatrixPresentationTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_TraitAccess_getRectPresentationTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGPathEx      BFDOM_TraitAccess_getPathPresentationTrait( BFDOM_SVGElement *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_SVGRGBColorEx  BFDOM_TraitAccess_getRGBColorPresentationTrait( BFDOM_SVGElement *pElement, BFDOM_String name );

BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setTraitNS( BFDOM_SVGElement *pElement, BFDOM_String namespaceURI, BFDOM_String name, BFDOM_String value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setTrait( BFDOM_SVGElement *pElement,  BFDOM_String name, BFDOM_String value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setFloatTrait( BFDOM_SVGElement *pElement,  BFDOM_String name, BFDOM_Float value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setMatrixTrait( BFDOM_SVGElement *pElement,  BFDOM_String name, BFDOM_SVGMatrix value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setRectTrait( BFDOM_SVGElement *pElement,  BFDOM_String name, BFDOM_SVGRect *pValue );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setPathTrait( BFDOM_SVGElement *pElement,  BFDOM_String name, BFDOM_SVGPath value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setRGBColorTrait( BFDOM_SVGElement *pElement,  BFDOM_String name, BFDOM_SVGRGBColor *pValue );
/**
 * \defgroup TraitAccessB binding-specific
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__TraitAccess"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 * Non-standard extension for all trait getters/setters are provided to
 *    handle 8-bit CHAR. These are suffixed with an A ( for example, ( #BFDOM_TraitAccess_getTraitA ) ).
 */

#ifdef BFDOM_HAS_ASCII_TRAITS

BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getTraitNSA( BFDOM_SVGElement *pElement, BF_CHAR *namespaceURI, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_FloatEx        BFDOM_TraitAccess_getFloatTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_TraitAccess_getMatrixTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_TraitAccess_getRectTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGRGBColorEx  BFDOM_TraitAccess_getRGBColorTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGPathEx      BFDOM_TraitAccess_getPathTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getPresentationTraitNSA( BFDOM_SVGElement *pElement, BF_CHAR *namespaceURI, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_TraitAccess_getPresentationTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_FloatEx        BFDOM_TraitAccess_getFloatPresentationTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGMatrixEx    BFDOM_TraitAccess_getMatrixPresentationTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGRectEx      BFDOM_TraitAccess_getRectPresentationTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGPathEx      BFDOM_TraitAccess_getPathPresentationTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_SVGRGBColorEx  BFDOM_TraitAccess_getRGBColorPresentationTraitA( BFDOM_SVGElement *pElement, BF_CHAR *name );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setTraitA( BFDOM_SVGElement *pElement,  BF_CHAR *name, BF_CHAR *value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setTraitNSA( BFDOM_SVGElement *pElement, BF_CHAR *namespaceURI, BF_CHAR *name, BF_CHAR *value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setFloatTraitA( BFDOM_SVGElement *pElement,  BF_CHAR *name, BFDOM_Float value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setMatrixTraitA( BFDOM_SVGElement *pElement,  BF_CHAR *name, BFDOM_SVGMatrix value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setRectTraitA( BFDOM_SVGElement *pElement,  BF_CHAR *name, BFDOM_SVGRect *pValue );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setPathTraitA( BFDOM_SVGElement *pElement,  BF_CHAR *name, BFDOM_SVGPath value );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_TraitAccess_setRGBColorTraitA( BFDOM_SVGElement *pElement,  BF_CHAR *name, BFDOM_SVGRGBColor *pValue );

#endif  /* BFDOM_HAS_ASCII_TRAITS */

/** @} */
/** @} */


/**
 * \defgroup SVGElement SVGElement
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGElement"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
BF_PLAYER_IMPORT BFDOM_StringEx       BFDOM_SVGElement_getId( BFDOM_SVGElement *pElement );
BF_PLAYER_IMPORT BFDOM_DOMException   BFDOM_SVGElement_setId( BFDOM_SVGElement *pElement, BFDOM_String id );

/** <b>binding-specific</b> - returns element's type
 * \param pElement an element pointer
 * \return a UINT8 indicating the type of element. The value returned is not useful unless the internal element enumeration is known.
 */
BF_PLAYER_IMPORT BF_UINT8 BFDOM_SVGElement_getType( BFDOM_SVGElement *pElement );

/** <b>binding-specific</b> - checks if an SVGElement has an ID
 * \param pElement an element pointer
 * \return BF_TRUE if the element contains an ID
 */
BF_PLAYER_IMPORT BF_BOOL              BFDOM_SVGElement_hasId( BFDOM_SVGElement *pElement );
/** @} */


/**
 * \defgroup SVGTimedElement SVGTimedElement
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGTimedElement"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef BFDOM_Element       BFDOM_SVGTimedElement;

BF_PLAYER_IMPORT void           BFDOM_SVGTimedElement_pauseElement( BFDOM_SVGTimedElement *pElement );
BF_PLAYER_IMPORT void           BFDOM_SVGTimedElement_resumeElement( BFDOM_SVGTimedElement *pElement );
BF_PLAYER_IMPORT BF_BOOL        BFDOM_SVGTimedElement_getIsPaused( BFDOM_SVGTimedElement *pElement );
/** @} */


/**
 * \defgroup SVGGlobal SVGGlobal
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGGlobal"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
BF_PLAYER_IMPORT BFDOM_DOMException BFDOM_SVGGlobal_gotoLocation( BFDOM_SVGGlobal *pGlobal, BFDOM_String newURI );

BF_PLAYER_IMPORT BFDOM_DOMException BFDOM_SVGGlobal_getURL( BFDOM_SVGGlobal *pGlobal, BFDOM_String URI, BFDOM_AsyncStatusCallback callback, void *pCallbackData, BFDOM_Document *contextDoc );

BF_PLAYER_IMPORT BFDOM_DOMException BFDOM_SVGGlobal_postURL( BFDOM_SVGGlobal *pGlobal, BFDOM_String URI, BFDOM_String data, BFDOM_String type, BFDOM_String encoding, BFDOM_AsyncStatusCallback callback, void *pCallbackData, BFDOM_Document *contextDoc );

BF_PLAYER_IMPORT BFDOM_Node *BFDOM_SVGGlobal_parseXML( BFDOM_SVGGlobal *pGlobal, BFDOM_String data, BFDOM_Document *contextDoc );
/** @} */


/**
 * \defgroup AsyncStatusCallback AsyncStatusCallback
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__AsyncStatusCallback"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
/** @} */


/**
 * \defgroup AsyncURLStatus AsyncURLStatus
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__AsyncURLStatus"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
/** @} */
/** @} Module:SVG */






#ifdef __cplusplus
BF_END_EXTERN_C
#endif


#endif

