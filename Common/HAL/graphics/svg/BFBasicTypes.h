/*
 * Filename : BFBasicTypes.h
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
/**
 * \file BFBasicTypes.h
 *
 * This file defines all basic types.
 *
 * January 20, 2002
 */

#ifndef BF_BASICTYPES_H_
#define BF_BASICTYPES_H_

/** Allows source to be compiled C compiler and used within a C++ compiler */
#define BF_BEGIN_EXTERN_C     extern "C" {

/** Allows source to be compiled by a C++ compiler */
#define BF_END_EXTERN_C       }

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif


/*****************************************************************************
 * Common Platform Types, Constants and Definitions
 *****************************************************************************/


/*****************************************************************************
 * Basic types definition
 *****************************************************************************/

typedef char                    BF_CHAR;
typedef unsigned short          BF_WCHAR;

typedef signed char             BF_INT8;
typedef signed short            BF_INT16;
typedef signed long             BF_INT32;

typedef unsigned char           BF_UINT8;
typedef unsigned short          BF_UINT16;
typedef unsigned long           BF_UINT32;

typedef signed long             BFInteger;
typedef unsigned long           BFUInteger;

typedef BF_INT32                BFFixedPoint;

typedef unsigned char           BF_BOOL;
#define BF_FALSE 0
#define BF_TRUE  1

#define BF_CHAR_BIT 8   /** Bits per char */

/** BitFlash heap context */
typedef struct BF__HeapContext  BFHeapContext;

/** Return codes as defined in BFError.h */
typedef BF_UINT16 BFStatus;

/** To allow pointer/integer arguments */
typedef union
{
    BF_UINT32   u;		/** the argument represented as a BF_UINT32 value */
    void       *p;		/** the argument represented as a void* value */
}
BF_INT_VOID;



/** <BR>See <A HREF="http://www.w3.org/TR/SVGMobile12/interact.html#specifyingnavigation">W3C SVGT 1.2</A> */
typedef enum
{
    /* This list should be in the same order as the enumerations in our dtd (see the sys:param element) */
    BF_NAV_NEXT        = 2,		/** move focus in the nav-next direction */
    BF_NAV_PREV        = 3,		/** move focus in the nav-prev direction */
    BF_NAV_UP          = 4,		/** move focus in the nav-up direction */
    BF_NAV_UP_RIGHT    = 5,		/** move focus in the nav-up-right direction */
    BF_NAV_RIGHT       = 6,		/** move focus in the nav-right direction */
    BF_NAV_DOWN_RIGHT  = 7,		/** move focus in the nav-down-right direction */
    BF_NAV_DOWN        = 8,		/** move focus in the nav-down direction */
    BF_NAV_DOWN_LEFT   = 9,		/** move focus in the nav-down-left direction */
    BF_NAV_LEFT        = 10,	/** move focus in the nav-left direction */
    BF_NAV_UP_LEFT     = 11		/** move focus in the nav-up-left direction */
}
BFFocusCommand;


/** Return value for interrupt request callback. See #BF_COMMAND_DOCUMENT_SET_INTERRUPT_CB */
typedef enum
{
    BF_PLAYER_INTERRUPT_CONTINUE,	/** to resume the current operation immediately */
    BF_PLAYER_INTERRUPT_PAUSE,		/** to pause the current operation, returning immediately from the call stack */
    BF_PLAYER_INTERRUPT_ABORT		/** to abort the current operation and return immediately from the call stack */
} BFInterruptAction;


/** <BR>See <A HREF="http://www.w3.org/TR/DOM-Level-3-Events/keyset.html#KeySet-Set">W3C DOM 3 Events</A>
  * Used in keyBoard events dispatched with the #BF_COMMAND_KEY_DOWN/UP commands.
  * These enumerations coorespond to keyIdentifier strings defined by the W3C DOM 3 Events specification.
  * The User Agent must map key down and up events from the operating system to a specific BFKeyIdentifier value.
  */
typedef enum
{
    /* This list should be in the same order as the enumerations in our dtd (see the sys:param element) */
    BF_KEY_UNDEFINED,

    BF_KEY_LEFT, BF_KEY_RIGHT, BF_KEY_UP, BF_KEY_DOWN,

    BF_KEY_OK,

    BF_KEY_SOFT1, BF_KEY_SOFT2,

    BF_KEY_SHIFT, BF_KEY_ALT, BF_KEY_CONTROL, BF_KEY_ENTER, BF_KEY_BACKSPACE,

    BF_KEY_SPACE, BF_KEY_EXCLAMATION, BF_KEY_DOUBLEQUOTE, BF_KEY_POUND, BF_KEY_DOLLAR, BF_KEY_PERCENT, BF_KEY_AMPERSAND, BF_KEY_APOSTROPHE, BF_KEY_LEFTPAREN, BF_KEY_RIGHTPAREN, BF_KEY_ASTERISK, BF_KEY_PLUS, BF_KEY_COMMA, BF_KEY_MINUS, BF_KEY_DOT, BF_KEY_SLASH,

    BF_KEY_ZERO, BF_KEY_ONE, BF_KEY_TWO, BF_KEY_THREE, BF_KEY_FOUR, BF_KEY_FIVE, BF_KEY_SIX, BF_KEY_SEVEN, BF_KEY_EIGHT, BF_KEY_NINE,

    BF_KEY_COLON, BF_KEY_SEMICOLON, BF_KEY_LESSTHAN, BF_KEY_EQUAL, BF_KEY_GREATERTHAN, BF_KEY_QUESTION, BF_KEY_AT,

    BF_KEY_A, BF_KEY_B, BF_KEY_C, BF_KEY_D, BF_KEY_E, BF_KEY_F, BF_KEY_G, BF_KEY_H, BF_KEY_I, BF_KEY_J, BF_KEY_K, BF_KEY_L, BF_KEY_M, BF_KEY_N, BF_KEY_O, BF_KEY_P, BF_KEY_Q, BF_KEY_R, BF_KEY_S, BF_KEY_T, BF_KEY_U, BF_KEY_V, BF_KEY_W, BF_KEY_X, BF_KEY_Y, BF_KEY_Z,

    BF_KEY_LEFTSQUARE, BF_KEY_BACKSLASH, BF_KEY_RIGHTSQUARE, BF_KEY_CIRCUMFLEX, BF_KEY_UNDERSCORE, BF_KEY_GRAVE, BF_KEY_LEFTCURLY, BF_KEY_VERTICALLINE, BF_KEY_RIGHTCURLY,

    BF_KEY_TILDE
}
BFKeyIdentifier;

/** <BR>See <A HREF="http://www.w3.org/TR/DOM-Level-3-Events/events.html#Events-KeyboardEvents-Interfaces">W3C DOM 3 Events</A>.
  * Used in keyBoard events dispatched with the #BF_COMMAND_KEY_DOWN/UP commands.
  *
  */
typedef enum
{
    BF_KEY_MOD_NONE        = 0,			/** no modifier */
    BF_KEY_MOD_ALT         = 0x01,		/** keyboardEvent.altKey */
    BF_KEY_MOD_ALTGRAPH    = 0x02,		/** unused - for future expansion */
    BF_KEY_MOD_CONTROL     = 0x04,		/** keyboardEvent.ctrlKey */
    BF_KEY_MOD_META        = 0x08,		/** keyboardEvent.metaKey */
    BF_KEY_MOD_SHIFT       = 0x10,		/** keyboardEvent.shiftKey */
    BF_KEY_MOD_LONGREPEAT  = 0x20,		/** unused - for future expansion */
    BF_KEY_MOD_SHORTREPEAT = 0x40		/** unused - for future expansion */
} BFKeyModifier;

/** Logging message type */
typedef enum
{
    BF_LOGGING_NONE        = 0,
    BF_LOGGING_ANIMATION   = 1<<1,
    BF_LOGGING_VIDEO       = 1<<2,
    BF_LOGGING_AUDIO       = 1<<3,
    BF_LOGGING_DOM         = 1<<4,
    BF_LOGGING_COMMANDS    = 1<<5,
    BF_LOGGING_ALL         = 0xff
} BFLoggingType;

/** <BR>See <A HREF="http://www.w3.org/TR/SVG11/coords.html#Units">W3C SVG 1.1</A> */
typedef enum
{
    BF_UNITS_INCH,
    BF_UNITS_CM,
    BF_UNITS_MM,
    BF_UNITS_POINT,
    BF_UNITS_PICA,
    BF_UNITS_PIXEL,
    BF_UNITS_PERCENT

} BFUnitsType;

/** Service Type */
typedef enum
{
    BF_ID_MIN = 0,
    BF_ID_Audio,
    BF_ID_Video,
    BF_ID_SCXML,
    BF_ID_DataIf,
    BF_ID_ForeignObject,
    BF_ID_Script,
    BF_ID_MAX

} BFServiceID;

/** <BR>See <A HREF="http://www.w3.org/TR/SVGMobile12/painting.html#systemPaint">W3C SVGT 1.2</A>
	The text after BF_SYSTEM_COLOR_ maps directly to the SVG 1.2 specification names for system paints
	which are allowed in content. Refer to the system paint section for names and meaning of each
	system color. This enumeration is used to map system colors to ARGB colors ( see BF_COMMAND_DOCUMENT_SET_SYSTEM_COLOR ).*/
typedef enum
{
    BF_SYSTEM_COLOR_ActiveBorder = 0,
    BF_SYSTEM_COLOR_ActiveCaption,
    BF_SYSTEM_COLOR_AppWorkspace,
    BF_SYSTEM_COLOR_Background,
    BF_SYSTEM_COLOR_ButtonFace,
    BF_SYSTEM_COLOR_ButtonHighlight,
    BF_SYSTEM_COLOR_ButtonShadow,
    BF_SYSTEM_COLOR_ButtonText,
    BF_SYSTEM_COLOR_CaptionText,
    BF_SYSTEM_COLOR_GrayText,
    BF_SYSTEM_COLOR_Highlight,
    BF_SYSTEM_COLOR_HighlightText,
    BF_SYSTEM_COLOR_InactiveBorder,
    BF_SYSTEM_COLOR_InactiveCaption,
    BF_SYSTEM_COLOR_InactiveCaptionText,
    BF_SYSTEM_COLOR_InfoBackground,
    BF_SYSTEM_COLOR_InfoText,
    BF_SYSTEM_COLOR_Menu,
    BF_SYSTEM_COLOR_MenuText,
    BF_SYSTEM_COLOR_Scrollbar,
    BF_SYSTEM_COLOR_ThreeDDarkShadow,
    BF_SYSTEM_COLOR_ThreeDFace,
    BF_SYSTEM_COLOR_ThreeDHighlight,
    BF_SYSTEM_COLOR_ThreeDLightShadow,
    BF_SYSTEM_COLOR_ThreeDShadow,
    BF_SYSTEM_COLOR_Window,
    BF_SYSTEM_COLOR_WindowFrame,
    BF_SYSTEM_COLOR_WindowText,
    BF_SYSTEM_COLOR_MAX
} BFSystemColor;

/** The maximum string size of a textInput character and keyIdentifier string */
#define BF_MAX_KEY_CHARACTER_LEN      10

/** An array of UNICODE characters identifying the character which was pressed. See textInput event from
  * DOM Level 3 events and the #BF_COMMAND_TEXT_INPUT command. */
typedef BF_WCHAR   BFKeyCharacters[BF_MAX_KEY_CHARACTER_LEN];

/** Called during serialization
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param pString string representation of DOM element or attribute */
typedef void (*BFSerializeCallback)( BF_INT_VOID pUserData, BF_CHAR const *pString );

/** Called during frame buffer clearing
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param x1 left start of cleared rectangle
 * \param y1 top  start of cleared rectangle
 * \param x2 right end of cleared rectangle
 * \param y2 left end of cleared rectangle
 */
typedef void (*BFClearCallback)( BF_INT_VOID pUserData, BFUInteger x1, BFUInteger y1, BFUInteger x2, BFUInteger y2 );


/** Called when the document is complete and ready to be rendered
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param uAsyncDecodeStatus status code ( see BF_COMMAND_DOCUMENT_SET_COMPLETENESS_CB)
 * \param uNbAsyncImages the total number of asynchronous images
   that have been decoded (successfully or not)
 */
typedef void (*BFCompletenessCallback)( BF_INT_VOID pUserData, BFStatus uAsyncDecodeStatus, BF_UINT32 uNbAsyncImages  );

/** Used to indicate to the User Agent that a new request is pending
 * \param pUserData contains what the user data specified in pUserData of this struct */
typedef void (*BFNewRequestCallback)( BF_INT_VOID pUserData );

/** Used to indicate to the User Agent that an image has been parsed and ready for Asynchronous decoding by the UA
 * \param pUserData contains what the user data specified in pUserData of this struct */
typedef void (*BFNewImageCallback)( BF_INT_VOID pUserData );


/** Used to notify if a parsed document contains animation
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param bBackwardSeekingAllowed indicates whether the SVG document supports seeking backwards in the timeline */
typedef void (*BFAnimationNotifyCallback)( BF_INT_VOID pUserData, BF_BOOL bBackwardSeekingAllowed );

/** Called when the SVGGlobal.gotoLocation() method is used.
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param pURI the URI to navigate to */
typedef void (*BFGotoLocationCallback)( BF_INT_VOID pUserData, BF_CHAR* pURI );

/** Called when the SVGGloval.getURL() method is used.
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param requestHandle internal handle idenfitying the outstanding HTTP request
 * \param pURI the URI to retreive */
typedef void (*BFGetURLCallback)( BF_INT_VOID pUserData, BF_UINT32 requestHandle, BF_CHAR* pURI );

/** Called when the SVGGloval.postURL() method is used.
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param requestHandle internal handle idenfitying the outstanding HTTP request
 * \param pURI the URI to post to
 * \param pData 0-terminated 16-bit string containing data to post
 * \param pType string for the content type of the POST request
 * \param pEncoding string for the encoding of the POST request */
typedef void (*BFPostURLCallback)( BF_INT_VOID pUserData, BF_UINT32 requestHandle, BF_CHAR* pURI, BF_WCHAR *pData, BF_WCHAR *pType, BF_WCHAR *pEncoding );


/** Get script resource, usually external file. Must return UTF-8 encoded text.
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param pURI the URI to get
 * \param puLength the size of the returned buffer
 * \return UTF-8 encoded text. Must be allocated with player memory context, after callbacl player will free the memory */
typedef BF_UINT8* (*BFGetScriptResource)( BF_INT_VOID pUserData, BF_CHAR *pURI, BF_UINT32 *puLength );


/** Called when a log message requires output.
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param pMsg log message string */
typedef void (*BFLoggingCallback)( BF_INT_VOID pUserData, const BF_CHAR* pMsg);

/** Called when a service needs to be initialized.
 * \param pUserData contains what the user data specified in pUserData of this struct
 * \param serviceID the service to initialze */
typedef void (*BFServiceSetupCallback)( BF_INT_VOID pUserData, const BFServiceID serviceID);


/** Mapping between a system color define and real RGB color */
typedef struct
{
    BFSystemColor   systemColor;	/** the system color type */
    BF_UINT32       value;			/** the color value description of the color. The value is an ARGB 32-bit color */
} BFColorMapping;

/** List of color mappings */
typedef struct
{
    BFColorMapping  *pColorMapping;		/** a pointer to an array of BFColorMapping structures */
    BF_UINT32       uLength;			/** the length of the BFColorMapping array */
} BFColorMappings;

/** BFAudioProperties is used to pass information about the audio media between the SVG
 * core library and the application’s native audio playback mechanism. */
typedef struct
{
    /** In: The intial volume of the audio - a value between 0-255 */
    BF_UINT8 uInitialVolume;

    /** Out: duration of audio, in milliseconds (optional; can be set later using BF_COMMAND_AUDIO_SET_DURATION) */
    BF_UINT32 uDuration;

} BFAudioProperties;


/** Sound object callbacks */
typedef struct
{
    /** Plays the sound object from the beginning.
     * The callback function should read the audio media file, and begin playback.
     * The function should return a pointer to the native audio object, which will
     * be used as a handle to control the audio playback in future callbacks.
     * BFAudioProperties may be set before exiting this callback, or set later with
     * a call to BF_COMMAND_AUDIO_SET_DURATION. This object is primarily used as a
     * parameter to the player command BF_COMMAND_AUDIO_SETUP.
     * <br><br>
     * <b>WARNING</b> : this function deals with pointers to internal DOM elements (pAudioElement).
     * If the player's document are to be destroyed, for whatever reason, the application
     * HAS TO discard any such pointer because they then point to an invalid memory areas !
     * \param pUserData     The application-specific data, initially provided in BFAudioObject during BF_COMMAND_AUDIO_SETUP.
     * \param pAudioElement A pointer to the “audio” SVG DOM element.
     * \param pFilename     The filename of the audio media file.
     * \param pTypeStr      The “type” attribute of the “audio” SVG DOM element.
     * \param pProperties   A pointer to a BFAudioProperties object.
     * \return A pointer to the native audio object.  This pointer will be used as a handle to control the audio playback in future callbacks.
     */
    void *(*pfnPlay)  ( void *pUserData,  void *pAudioElement, const BF_CHAR *pFilename, const BF_WCHAR *pTypeStr, BFAudioProperties *pProperties );

    /** Stops playing the sound.
     * \param pUserData The application-specific data, initially provided in BFAudioObject during BF_COMMAND_AUDIO_SETUP.
     * \param pSound The audio object handle, initially returned from pfnPlay().
     */
    void (*pfnStop)  ( void *pUserData, void *pSound );

    /** Pauses and unpauses the sound
     * \param pUserData The application-specific data, initially provided in BFAudioObject during BF_COMMAND_AUDIO_SETUP.
     * \param pSound    The audio object handle, initially returned from pfnPlay().
     * \param bPaused   If true, the audio will be paused. If false, the audio will be unpaused.
     */
    void (*pfnPause)  ( void *pUserData, void *pSound, BF_BOOL bPaused );

    /** Sets the sound's volume
     * \param pUserData The application-specific data, initially provided in BFAudioObject during BF_COMMAND_AUDIO_SETUP.
     * \param pSound    The audio object handle, initially returned from pfnPlay().
     * \param uVolume   The desired volume (0 = mute, 255 = max).
     */
    void (*pfnSetVolume)  ( void *pUserData, void *pSound, BF_UINT8 uVolume );

    /** Called each animation update to syncronize timeline
     * \param pUserData The application-specific data, initially provided in BFAudioObject during BF_COMMAND_AUDIO_SETUP.
     * \param pSound    The audio object handle, initially returned from pfnPlay().
     * \return The current audio playback time position, in milliseconds.
     */
    BF_INT32 (*pfnGetCurrentTime)  ( void *pUserData, void *pSound );

    /** A pointer to some application-specific data. Passed back to all the functions when invoked */
    void *pUserData;

} BFAudioObject;


/** BFVideoProperties is used to pass information about the video media between the SVG core
 * library and the application’s native video playback mechanism.
 */
typedef struct
{
    /** In: A flag indicates whether disable rendering the current video.
    If the transform behavior of the video is geometric, a value of zero
    of width or Height shall disable rendering of the video element. **/
    BF_BOOL disableRendering;

    /** In: The intial volume of the audio - a value between 0-255 */
    BF_UINT8 uInitialVolume;

    /** Out: duration of video (optional; can be set later using BF_COMMAND_VIDEO_SET_DURATION) */
    BF_UINT32 uDuration;

    /** Out: width of video (optional; can be set later using BF_COMMAND_VIDEO_SET_DIMENSIONS) */
    BF_UINT32 uWidth;

    /** Out: height of video (optional; can be set later using BF_COMMAND_VIDEO_SET_DIMENSIONS) */
    BF_UINT32 uHeight;

} BFVideoProperties;


/** BFVideoTransformParams defines the transformation parameters for the video media. */
typedef struct
{
    /** Screen coordinates */
    BF_INT32  iX;

    /** Screen coordinates */
    BF_INT32  iY;

    /** Screen coordinates */
    BF_INT32  iWidth;

    /** Screen coordinates */
    BF_INT32  iHeight;

    /** Scale X ( 100 is 1:1 scale ) */
    BF_INT16  iScaleX;

    /** Scale Y ( 100 is 1:1 scale ) */
    BF_INT16  iScaleY;

    /** Rotation ( degrees ) */
    BF_INT16  iRotation;

} BFVideoTransformParams;


/** BFVIDEO_HAS_TRANSFORM_VIDEO indicates that the native playback interface can
 * transform video.
 * This flag is used as a bitmask in BFVideoObject's uCapabilities.
 * <br><br>
 * If this flag is not set, the video’s transformBehaviour attribute will be
 * overridden to “pinned.”
 */
#define BFVIDEO_HAS_TRANSFORM_VIDEO     0x01

/** BFVIDEO_HAS_COMPOSE_VIDEO specifies whether the native playback interface
 * can compose video.
 * This flag is used as a bitmask in BFVideoObject's uCapabilities.
 * <br><br>
 * If this flag is set, the SVG library will request for video frames from the
 * native interface, by calling pfnGetFrame().
 * If this flag is not set, the native playback interface is expected to draw the
 * video frames directly to the render buffer; the SVG library will not call
 * pfnGetFrame().  If the native playback interface does not support compose,
 * the SVG library will pass down any tranform parameters by calling the
 * pfnSetTransform() callback function.
 * On fast machines, such as desktops, where it is feasible to embed the video
 * into the SVG canvas, this flag can be set.
 * However, some small devices rely on hardware acceleration to render directly
 * to the buffer.  On such systems, it is not feasible to composite the
 * video into a SVG canvas; in this case, this flag should not be set.
 */
#define BFVIDEO_HAS_COMPOSE_VIDEO       0x02

/** Video object callbacks */
typedef struct
{
    /** Plays the video object.
     * The callback function should read the video media file, and begin playback.
     * The function should return a pointer to the native video object, which will
     * be used as a handle to control the video playback in future callbacks.
     * BFVideoProperties may be set before exiting this callback, or set later with
     * a call to BF_COMMAND_VIDEO_SET_DURATION, etc.
     * <br><br>
     * <b>WARNING</b>: This function deals with pointers to internal DOM elements (pVideoElement).
     * If the player's document are to be destroyed, for whatever reason, the application
     * HAS TO discard any such pointer because they then point to an invalid memory areas !
     * \param pUserData     The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideoElement A pointer to the “video” SVG DOM element.
     * \param pFilename     The filename of the video media file.
     * \param pTypeStr      The “type” attribute of the “video” SVG DOM element.
     * \param pProperties   A pointer to a BFVideoProperties object.
     * \return A pointer to the native video object.  This pointer will be used as a handle to control the video playback in future callbacks.
     */
    void *(*pfnPlay)  ( void *pUserData, void *pVideoElement, const BF_CHAR *pFilename, const BF_WCHAR *pTypeStr, BFVideoProperties *pProperties );

    /** Stops playing video
     * \param pUserData The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideo    The video object handle, initially returned from pfnPlay().
     */
    void (*pfnStop)  ( void *pUserData, void *pVideo );

    /** Pauses and unpauses the video
     * \param pUserData The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideo    The video object handle, initially returned from pfnPlay().
     * \param bPaused   If true, the video will be paused. If false, the video will be unpaused.
     */
    void (*pfnPause)  ( void *pUserData, void *pVideo, BF_BOOL bPaused );

    /** Sets the video's volume
     * \param pUserData The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideo    The video object handle, initially returned from pfnPlay().
     * \param uVolume   The desired volume  (0 = mute, 255 = max).
     */
    void (*pfnSetVolume)  ( void *pUserData, void *pVideo, BF_UINT8 uVolume );

    /** Returns a buffer to the current frame, in the same pixel format as the render buffer
     * \param pUserData The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideo    The video object handle, initially returned from pfnPlay().
     * \return Pointer to the video frame buffer, in the same pixel format as the render buffer.
     */
    BF_UINT8* (*pfnGetFrame) ( void *pUserData, void *pVideo );

    /** Set transform parameters.
     * For devices which do not support BFVIDEO_HAS_COMPOSE_VIDEO, allows the native video
     * plug-in control over position, scale and rotation of video.
     * \param pUserData         The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideo            The video object handle, initially returned from pfnPlay().
     * \param pTransformData    A pointer to a BFVideoTransformParams object.
     */
    void (*pfnSetTransform) ( void *pUserData, void *pVideo, BFVideoTransformParams *pTransformData );

    /** Called each animation update to syncronize timeline
     * \param pUserData The application-specific data, initially provided in BFVideoObject during BF_COMMAND_VIDEO_SETUP.
     * \param pVideo    The video object handle, initially returned from pfnPlay().
     * \return The current playback time position in milliseconds
     */
    BF_INT32 (*pfnGetCurrentTime)  ( void *pUserData, void *pVideo );

    /** A pointer to some application-specific data. Passed back to all the functions when invoked
     */
    void              * pUserData;

    /** A bitmask, representing the native video decoder’s capabilities.
     * Valid flags: BFVIDEO_HAS_TRANSFORM_VIDEO, BFVIDEO_HAS_COMPOSE_VIDEO
     */
    BF_UINT8          fCapabilities;

} BFVideoObject;



/** Foreign object callbacks */
typedef struct
{
    /** Called when the foreignObject element finishes parsing.
       pUserData contains what the user data specified in pUserData of this struct
       pXlinkHref is the resolved URI from the xlink:href attribute of foreignObject
       pForeigObject is a pointer to the actual foreignObject (use uDOM to access its children)
       This callback should return a pointer that will be used to reference later this object later. */
    void * (*pfnCreate) ( void *pUserData, const BF_CHAR *pXlinkHref, void *pForeigObject );

    /** Called when the foreignObject needs to be rendered
       pUserData contains what the user specified in pUserData of this struct
       id is the pointer returned by pfnCreate
       fpX, fpY, fpWidth, fpHeight : x/y/width/height of the foreignObject as specified in the file. 16.16 fixed point format
       fpA, fpB, fpC, fpD, fpE, fpF, fpG : current transformation matrix to be applied to the object. 16.16 fixed point format */
    void (*pfnExecute)  ( void *pUserData, void *id,
                          BF_INT32 fpX, BF_INT32 fpY, BF_INT32 fpWidth, BF_INT32 fpHeight,
                          BF_INT32 fpA, BF_INT32 fpB, BF_INT32 fpC, BF_INT32 fpD, BF_INT32 fpE, BF_INT32 fpF );

    /** Called when the ForeignObject is being destroyed.
       pUserData contains what the user specified in pUserData of this struct
       id is the pointer returned by pfnCreate */
    void (*pfnDestroy)  ( void *pUserData, void *id );

    /** In: Value passed back to all the functions when invoked */
    void              * pUserData;

} BFForeignObject;


/** Initializes and external scripting engine */
typedef struct
{
    /** Initializes the script object. Called once per player initialization */
    void (*pfnInitialize) ( void *pUserData );

    /** Starts new document */
    /** when uDOM in included, cast to BFDOM_Document* */
    /** ppDocumentUserData is the pointer to the script object reprosenting the document, and
       is required to be non-null if this function return success */
    /** returns success or failure */
    BF_BOOL (*pfnNewDocument) ( void *pUserData, void *pDocumnent, void **ppDocumentUserData );

    /** Deletes document */
    void (*pfnDeleteDocument) ( void *pDocumentUserData );

    /** Deletes an individual element */
    void (*pfnDeleteElement) ( void *pDocumentUserData, void *pElement );

    /** Evaluate Script */
    void (*pfnEvaluate)  ( void *pDocumentUserData, BF_WCHAR *pScript, BF_UINT32 uLen, void *pEvent, BF_UINT32 uLineNimber );

    /** Called to shutdown the external scripting engine */
    void (*pfnShutdown)  ( void *pUserData );

    /** Value passed back to all the functions when invoked */
    void              * pUserData;

    /** used internally for lazy Instantiation */
    BF_BOOL           bInitialized;

} BFScriptObject;



/** bounding box values are scaled to font size, and express distances from the glyph's origin */
typedef struct
{
    BF_INT32   iMinX;		/**Upper-left abscissae, in 16.16FP */
    BF_INT32   iMaxX;		/**Lower-right abscissae, in 16.16FP */
    BF_INT32   iMinY;		/**Upper-left ordinate, in 16.16FP */
    BF_INT32   iMaxY;		/**Lower-right ordinate, in 16.16FP */
} BFGlyphBBox;



/** all metrics are scaled to font size, and express distances from the glyph's origin */
typedef struct
{
    BFFixedPoint   fpWidth;          /** width of the glyph (scaled bounding box's width) */
    BFFixedPoint   fpHeight;         /** height of the glyph (scaled bounding box's width) */

    BFFixedPoint   fpHoriAdvance;    /** horizontal advance of the glyph */
    BFFixedPoint   fpHoriBearingX;   /** left bearing of the glyph in horizontal layout */
    BFFixedPoint   fpHoriBearingY;   /** top bearing of the glyph in horizontal layout */

    BFFixedPoint   fpVertAdvance;    /** vertical advance of the glyph */
    BFFixedPoint   fpVertBearingX;   /** left bearing of the glyph in vertical layout */
    BFFixedPoint   fpVertBearingY;   /** top bearing of the glyph in vertical layout */

} BFGlyphMetrics;

/** If the glyph data is bitmap, BFGlyphBitmapPixelMode describes the pixel format the bitmap is*/
typedef enum
{
    BF_GLYPH_BITMAP_MONO,  /** 1 bit per pixel */
    BF_GLYPH_BITMAP_GRAY,  /** 1 byte per pixel */
    BF_GLYPH_BITMAP_OTHER
} BFGlyphBitmapPixelMode;


typedef struct
{
    BF_UINT8       *pBitmapData;     /** glyph data, a grayscale 8bit bitmap */
    BF_UINT32       uBitmapWidth;    /** bitmap width, in pixels */
    BF_UINT32       uBitmapHeight;   /** bitmap height, in pixels */
    BF_INT32        iBitmapPitch;    /** number of bytes per bitmap row */
    BF_INT32        iBitmapLeft;     /** bitmap's left bearing */
    BF_INT32        iBitmapTop;      /** bitmap's top bearing (positive means upwards) */
    BF_INT32        fpGlyphOffsetX;  /** glyph X position in the bitmap, in 16.16FP */
    BF_INT32        fpGlyphOffsetY;  /** glyph Y position in the bitmap, in 16.16FP */
    BFGlyphBitmapPixelMode pixelMode;
} BFGlyphBitmap;


typedef struct
{
    BFFixedPoint fpX;		/** X coordinate, in 16.16FP */
    BFFixedPoint fpY;		/** Y coordinate, in 16.16FP */
} BFGlyphOutlinePoint;


typedef struct
{
    BF_UINT16            uNbContours;  /** number of contours (sub-paths) */
    BF_UINT16           *pContours;    /** index of the last point of each contour */

    BF_UINT16            uNbPoints;    /** number of points in the outline */
    BFGlyphOutlinePoint *pPoints;      /** points */
    BF_UINT8            *pTags;        /** points' flags : bit 0 means "point is on curve" when set
                                          (control point otherwise), bit 1 means "cubic curve control point"
                                          when set (conic curve control point otherwise) */
} BFGlyphOutline;


/** <BR>See <A HREF="http://www.w3.org/Graphics/SVG/Group/repository/spec/mobile/1.2/1.2NG/publish/fonts.html#FontFaceElement">W3C SVGT 1.2</A>.
 * Indicates indicies into the baseline table for a font
*/
typedef enum
{
    BF_BASELINE_ALPHABETIC = 0,			/** Index into a baseline table to provide the alignment coordinate for
										the font to achieve alphabetic baseline alignment */
    BF_BASELINE_IDEOGRAPHIC,	        /** Index into a baseline table to provide the alignment coordinate for
										the font to achieve ideographic baseline alignment */
    BF_BASELINE_HANGING,				/** Index into a baseline table to provide the alignment coordinate for
										the font to achieve hanging baseline alignment */
    BF_BASELINE_COUNT					/** the total baseline table count */
} BFFontBaseline;


/** <BR>See <A HREF="http://www.w3.org/Graphics/SVG/Group/repository/spec/mobile/1.2/1.2NG/publish/fonts.html#FontFaceElement">W3C SVGT 1.2</A>.
 * 	The style of the font
*/
typedef enum
{
    BF_FONT_STYLE_ALL,			/** Specifies a font classified as normal, italic and oblique */
    BF_FONT_STYLE_NORMAL,		/** Specifies a font that is classified as 'normal' in the UA's font database */
    BF_FONT_STYLE_ITALIC,		/** Specifies a font that is classified as 'italic' in the UA's font database */
    BF_FONT_STYLE_OBLIQUE		/** Specifies a font that is classified as 'oblique' in the UA's font database */
} BFFontStyle;


/** <BR>See <A HREF="http://www.w3.org/Graphics/SVG/Group/repository/spec/mobile/1.2/1.2NG/publish/fonts.html#FontFaceElement">W3C SVGT 1.2</A>.
 * 	This is an indication of whether this font is the small-caps variant of a font
*/
typedef enum
{
    BF_FONT_VARIANT_NORMAL,     /** Specifies a font that is not labeled as a small-caps font. */
    BF_FONT_VARIANT_SMALLCAPS	/** Specifies a font that is labeled as a small-caps font. */
} BFFontVariant;


/** A font face description used to identify font faces in the BFFontEngineObject API */
typedef struct BF__FontFaceDesc
{
    BF_UINT32     uFontEngineID;    /** the font engine that this face has to be used with */
    void         *pPrivateFaceData; /** private font engine specific data associated with the face */
    BF_CHAR      *pFamilyName;      /** name of the font family */
    BF_CHAR      *pFileName;        /** file containing the font face */
    BF_UINT32     uFaceIndex;       /** index of this face in the file it comes from */

    BF_BOOL       bScalable;        /** is this a scalable font (as opposed to a bitmap font */
    BF_UINT8      uWeight;          /** ranging from 1 (lightest) to 9 (boldest) with 4 being "normal". 0 means "all" */
    BFFontStyle   style;            /** face's style */
    BFFontVariant variant;          /** face's variant */

    struct BF__FontFaceDesc *pNext;  /** next handle in a chained list */

} BFFontFaceDesc;


/** External Font engine registration */
typedef struct
{
    /** called when the SVG player is about to use a given font.
       pFontFaceDesc is one of the face handles returned by the font manager
       (pfnListExternalFonts()).
       The SVG player only forwards to this font engine the requests pertaining
       to fonts it owns. (i.e. pfnCommitFont() only passes in a pFontFaceDesc
       when fontEngineIDs of font and font engine match).
       This function shall return a boolean indicating success.
       In case of failure, no text relying on this font will be rendered. This
       callback shall assign the pointer pPrivateFaceData if it needs to store
       some private data for use with pfnReleaseFont() */
    BF_BOOL (*pfnCommitFont) ( void *pUserData, BFFontFaceDesc *pFontFaceDesc );

    /** called when the SVG player does not actively need a font
       face anymore. This gives a chance to the font engine to free up some resource
       for that face handle, though it might be used again later on (after another
       call to pfnCommitFont(). */
    void (*pfnReleaseFont) ( void *pUserData, BFFontFaceDesc *pFontFaceDesc );

    /** called when the SVG player needs to specify the virtual font size.
       fpSize is a 16.16 fixed point value representing a conceptual
       target size of the current font. This values is *not* necessarily the
       final pixel size, which depends on subsequent transformations applied to the glyph.
       The function shall set the return value *pActualSize. If the current font is
       scalable, the requested font size can always be honored, and the function shall
       return the requested fpSize, otherwise it returns the font size actually
       selected.
       This function shall return a boolean indicating success. In case of failure,
       further rendering of text using the current fonr at the requested size will be
       disabled. */
    BF_BOOL (*pfnSetFontSize) ( void *pUserData, BFFixedPoint fpSize, BFFixedPoint *pFPActualSize, BFFixedPoint *pBaselines );

    /** called when the SVG player needs the baseline table from the current font.
       pBaselines is an array of BF_BASELINE_COUNT entries, which shall be filled
       with baseline offsets, scaled to the font size specified with pfnSetFontSize().
       The function shall calculate baselines using heuristics if actual values are not
       available from the font.
       This function shall return a boolean indicating success. In case of failure,
       All baseline offsets are assumed to be zero. */
    BF_BOOL (*pfnGetFontBaselines) ( void *pUserData, BFFixedPoint *pBaselines );

    /** called when the SVG player needs to find out whether the current
       face contains a glyph for the specified unicode character. If the current
       face has a generic missing glyph but does not specifically support the
       requested character, it shall return BF_FALSE. */
    BF_BOOL (*pfnHasGlyphForChar) ( void *pUserData, BFUInteger uChar );

    /** called after pfnCommitFont() when the SVG player needs to access
       a glyph corresponding to a specific unicode code (uGlyph) from the current
       face.
       This function shall return a boolean indicating success. If successful,
       the glyph becomes the "current glyph" which will referred to by subsequent
       calls to pfnGetGlyphMetrics(), pfnGetGlyphBBox(), pfnRenderGlyph() etc.
       When the SVG player is done with this glyph, it will call pfnDoneGlyph().
       If the callback returns BF_FALSE, rendering of this glyph will be bypassed. */
    BF_BOOL (*pfnSelectGlyph) ( void *pUserData, BFUInteger uChar );

    /** called when the SVG player is done with the current glyph. This callback
       is always called after a successful call to pfnSelectGlyph(), and only in
       this case. It shall free system resources allocated during potential calls
       to pfnGetGlyphBBox(), pfnGetGlyphMetrics(), pfnRenderGlyph() etc.*/
    void (*pfnDoneGlyph) ( void *pUserData );

    /** called when the SVG player needs to retrieve metrics of the current glyph
       (as specified by pfnSelectGlyph()). The implementation
       of this callback shall fill the provided BFGlyphMetrics structure.
       This function shall return a boolean indicating success.
       In case of failure, rendering of this glyph will be bypassed. */
    BF_BOOL (*pfnGetGlyphMetrics)  ( void *pUserData, BFGlyphMetrics *pGlyphMetrics );

    /** called when the SVG player needs to retrieve the bounding box of
       the current glyph. The function shall fill the
       provided BFGlyphBBox structure.
       The returned bounding box values shall be scaled according to the font size
       returned by pfnSetFontSize() and are relative to the glyph's origin.
       It has to fully contain the glyph but doesn't have to be the the snugmost
       bounding box to fullfill this criteria (though the snugger, the better).
       This function shall return a boolean indicating success.
       In case of failure, rendering of this glyph will be bypassed. */
    BF_BOOL (*pfnGetGlyphBBox)  ( void *pUserData, BFGlyphBBox *pGlyphBBox );

    /** called when the SVG player needs to specify a transformation to be applied
       to the current glyph. This callback will only be called if the current
       face is scalable.
       Parameters are expressed in 16.16 fixed point format and represent the
       following matrix :
         | fpXX  fpXY |
         | fpYX  fpYY |
       This function shall return a boolean indicating success.
       In case of failure, rendering of this glyph will be bypassed. */
    BF_BOOL (*pfnTransformGlyph) ( void *pUserData, BFFixedPoint fpXX, BFFixedPoint fpXY, BFFixedPoint fpYX, BFFixedPoint fpYY );

    /** called when the SVG player needs the font engine to render the current
       glyph into an offscreen bitmap. This function shall fill the provided
       BFGlyphBitmap structure; it will only be called if the current face is
       *not* scalable (i.e. a bitmap face)
       This function shall return a boolean indicating success.
       In case of failure, rendering of this glyph will be bypassed. */
    BF_BOOL (*pfnRenderGlyph) ( void *pUserData, BFFixedPoint fpX, BFFixedPoint fpY, BFGlyphBitmap *pGlyphBitmap );

    /** called when the SVG player needs the font engine to return the vector
       outline of the current glyph. This function shall fill the provided
       BFGlyphOutline structure. The returned coordinates shall be scaled
       according to the font size returned by pfnSetFontSize() and are relative
       to the glyph's origin. The callback will only be called if the current face is
       *scalable*.
       This function shall return a boolean indicating success.
       In case of failure, rendering of this glyph will be bypassed. */
    BF_BOOL (*pfnGetGlyphOutline) ( void *pUserData, BFGlyphOutline *pGlyphOutline );

    /** called by the SVG player to obtain a list of all the system fonts
       available for text rendering.
       The return value shall be a chained list of available font faces.
       The SVG player will create copies of font information and will call
       pfnDoneListExternalFonts(), passing it back this pointer for cleanup purposes.
       A null return value indicates that there is no available external fonts. */
    BFFontFaceDesc *(*pfnListExternalFonts)  ( void *pUserData );

    /** called by the SVG player after a successful call to pfnListExternalFonts()
       to release recources allocated during that function. A successful call
       to pfnListExternalFonts() is always followed by a call to
       pfnDoneListExternalFonts().
       pFirstFontFaceDesc is the (unaltered) pointer that was returned from
       the pfnListExternalFonts() call. */
    void (*pfnDoneListExternalFonts)  ( void *pUserData, BFFontFaceDesc *pFirstFontFaceDesc );

    /** Opaque pointer passed back to all the functions when invoked */
    void              *pUserData;

} BFFontEngineObject;


/** Image decoder types */
typedef enum
{
    /** supported image types */
    BF_IMAGE_DECODER_TYPE_PNG     = 0,
    BF_IMAGE_DECODER_TYPE_JPG,

    /** number of supported types */
    BF_IMAGE_DECODER_TYPE_NUM_SUPPORTED,

    /** unsupported image types */
    BF_IMAGE_DECODER_TYPE_BMP,
    BF_IMAGE_DECODER_TYPE_GIF,
    BF_IMAGE_DECODER_TYPE_XBM,
    BF_IMAGE_DECODER_TYPE_WBMP,

    BF_IMAGE_DECODER_TYPE_INVALID = 0xff
} BFImageDecoderType;


/** Image decode status */
typedef enum
{
    BF_IMAGE_DECODER_STATUS_OK,
    BF_IMAGE_DECODER_STATUS_OUT_OF_MEMORY,
    BF_IMAGE_DECODER_STATUS_ERROR,
    BF_IMAGE_DECODER_STATUS_FORMAT_NOT_SUPPORTED
} BFImageDecoderStatus;


/** Image decoder data struct */
typedef struct
{

    /** Opaque data passed back to SVG library when callback functions are invoked */
    void  *m_pDocument;

    /** Opaque data passed back to SVG library when callback functions are invoked */
    void  *m_pExtData;

    /** Native data */
    void  *m_pNativeData;

    /** Private decoder-specific data, passed back to decoder when pfnImageDecode() is invoked */
    void *m_pPrivateDecoderData;

    /**
     * Callback invoked when an asynchronous image decoding starts.
     * pDocument must be the original pointer provided by the SVG
     * library when it invoked m_pfnDecodeImage().
     */
    BF_BOOL (*m_pfnAsyncStart) ( void *pDocument, BFImageDecoderType const uDecoderID, void *pDecodeInstanceData );

    /**
     * Callback invoked when an asynchronous image decoding ends.
     * pDocument must be the original pointer provided by the SVG
     * library when it invoked m_pfnDecodeImage().
     */
    void (*m_pfnAsyncEnd) ( void *pDocument, BFImageDecoderStatus uDecodeStatus, void *pDecodeInstanceData );

    /**
     * Callback invoked when the image has been decoded.
     * The image decoder is responsible for deleting the input buffer used for the decoding.
     * pDocument and pExtData must be the original pointers provided by the SVG library when it
     * invoked m_pfnDecodeImage().
     * Note that decoders must callback m_pfnDone() at some point after m_pfnDecodeImage()
     * has been invoked.  Otherwise, there will be a memory leak inside the SVG library.
     */
    void (*m_pfnDone) ( void *pDocument, void *pExtData, void *pBitmap );

} BFImageDecoderData;


/** Image decoder registration struct */
typedef struct
{

    /**
     * Type of image decoder
     */
    BFImageDecoderType  m_uDecoderID;

    /**
     * Private decoder-specific data.  External decoders can use this to
     * register private data that will be required during decoding.  This
     * context will be passed from the SVG library to the decoder when it
     * invokes one of the decoder functions.  The contents of this structure
     * is transparent to the SVG library.
     */
    void               *m_pPrivateDecoderData;

    /**
     * Pointer to the image decoding callback function.
     * pCtx is the original decoder context provided by the external image decoder
     * during registration.  The SVG library does not know the details of this structure.
     * pData contains callbacks and some data that is internal to the SVG library.
     * It must be passed back to the SVG library when the decoder invokes callbacks.
     */
    BFImageDecoderStatus (*m_pfnDecodeImage) ( BFImageDecoderData *pData,
            BF_UINT8 const     *pBuffer,
            BF_UINT32           uBufferSize );

    /**
     * Pointer to the asynchronous abort callback function.
     * pDecodeInstanceData will be the pointer provided by the decoder when it invoked
     * m_pfnAsyncStart().
     */
    void (*m_pfnAbortDecodeAsync) ( void *pDecodeInstanceData );

} BFImageDecoder;








/*****************************************************************************
 * DLL exports configuration
 *****************************************************************************/
#ifdef BF_DLL_EXPORTS_SYMBIAN
#include <e32Def.h>                 /* for IMPORT_C, etc. */
#define BF_PLAYER_EXPORT EXPORT_C
#define BF_PLAYER_IMPORT IMPORT_C
#else

#ifdef BF_WINCE
#define BF_PLAYER_EXPORT _declspec(dllexport)
#define BF_PLAYER_IMPORT _declspec(dllexport)
#else

/** Platform-dependant. Placed on methods which have the potential to be exported to a DLL. Is empty by default */
#define BF_PLAYER_EXPORT

/** Platform-dependant. Placed on methods which have the potential to be used by a DLL. Is empty by default */
#define BF_PLAYER_IMPORT

#endif

#endif



#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif /* BF_BASICTYPES_H_ */


