/*
 * Filename : BFPlayerTypes.h
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

/** \file BFPlayerTypes.h
 *
 * This file defines the commands and requests to interact with the BitFlash player.
 *
 * The listed commands in this file can be executed through the BFprocessCommand
 * function. Some of these commands require a parameter which can be a value or
 * a pointer to a more complex structure defined in this file.
 *
 * Some of the commands, even though they are listed, may not have an implementation
 * depending on the current Client SDK configuration (Refer to BFprocessCommand
 * description).
 */


/* History
 *
 * 2004/11/03 SH : Removed "BF_PLAYER_COMMAND_HANDLED" return code.
 *                 Now all commands return BF_NO_ERROR or an error code
 * 2004/11/03 SH : Added "bHandled" return field to BFPointerEvent, BFPointerClickEvent,
 *                 BFPointerPickEvent, BFIDClickEvent, BFActivateEvent
 * 2004/11/04 SH : Changed param types for the following commands :
 *                 BF_COMMAND_DOCUMENT_PAUSE,
 *                 BF_COMMAND_DOCUMENT_SEEK_TIME,
 *                 BF_COMMAND_DOCUMENT_SET_CURRENT_PAGE,
 *                 BF_COMMAND_VIEW_TEXTONLY,
 *                 BF_COMMAND_VIEW_ROTATE
 *                 Now taking pointer instead of value.
 * 2004/11/05 SH : Added new potential return codes to all commands :
 *                 BF_PLAYER_NEED_PARAM
 *                 BF_PLAYER_NEED_DOCUMENT
 *                 BF_PLAYER_NEED_PLAYER
 * 2004/11/09 SH : More consistent return of OUT_OF_MEMORY error in low memory conditions
 * 2004/11/09 AE : Changed BF_COMMAND_DOCUMENT_NEW to delete document on memory error
 * 2004/11/12 SH : When using asynchronous image decoders, out-of-mem conditions
 *                 that occured during image decoding will result into a
 *                 BF_PLAYER_OUT_OF_MEMORY error for the next command sent to the player.
 * 2004/11/19 SH : Now the doc completeness callback also returns a status.
 *                 Removed bDocumentComplete from BFNewDocument and BFRequestDone
 * 2004/11/25 AE:  Removed old rotation commands and replaced with SVG 1.2-complient command and behaviour
 * 2004/11/30 SH : Now the doc completeness callback also returns the nb of async images.
 * 2004/12/13 SH : New return code : BF_ABORTED. Changed The interrupt callback return code.
 * 2004/12/16 SH : Added field bManualImgDecoding to the BFPreferences
 *                 Added field uNbImagesToDecode to BFNewDocument / BFRequestDone
 *                 Added command BF_COMMAND_DOCUMENT_DECODE_NEXT_BITMAP
 * 2004/12/23 SH : Added safeguard mechanism to return BF_PLAYER_ILLEGAL_COMMAND
 *                 when a command is called from a callback called itself from a command
 * 2005/01/11 SH : Added new return code BF_PLAYER_ZOOM_LIMIT_REACHED returned by
 *                 BF_COMMAND_VIEW_REDRAW or BF_COMMAND_VIEW_ZOOM
 *                 When an error happens during rendering with interrupt mechanism,
 *                 BF_INTERRUPT is returned until the command completes, then
 *                 the error code is returned.
 * 2005/01/18 AE : Removed binary transparency support. Added alpha channel support via the
 *                 BF_COMMAND_VIEW_ENABLE_ALPHA_CHANNEL command.
 * 2005/01/26 AE : Removed pApplicationData from DCOUMENT_NEW command.
 * 2005/01/26 AE : Removed automatic paint on REDRAW. PAINT command now needs to be explicit
 * 2005/01/27 AE : Added field bUnknownOpcode to BFNewDocument
 * 2005/02/01 SH : Added field bAbort to BFContinue
 * 2005/02/16 SH : Removed BF_COMMAND_DOCUMENT_PAUSE, BF_COMMAND_XOR_REGION_DRAW
 * 2005/03/29 AE : Removed BF_COMMAND_FOCUS_NEXT/PREV/BEGIN/END
 * 2005/03/29 AE : Added BF_COMMAND_FOCUS_MOVE
 * 2005/03/29 AE : Change BF_COMMAND_DOCUMENT_ADVANCE_TIME behaviour - no longer redraws, or returns list of regions
                   No longer required to call BFFreeCommand or check for INTERRUPT return
 * 2005/03/29 AE : Changed BF_COMMAND_VIEW_PAN to never redraw - if fast pan is used a clipping region is returned
                   This region is then used in BF_COMMAND_VIEW_REDRAW
 * 2005/03/29 AE : Added clipping rect optional paramater for VIEW_REDRAW
 * 2005/04/01 JE : Added BF_COMMAND_DOCUMENT_STREAM_NEW, renamed BF_COMMAND_DOCUMENT_DATA_FETCHED to BF_COMMAND_DOCUMENT_STREAM_FETCH,
                   renamed BF_COMMAND_DOCUMENT_DONE to BF_COMMAND_DOCUMENT_STREAM_DONE. Use BF_COMMAND_DOCUMENT_NEW when loading
                   entire document from a single buffer. Use BF_COMMAND_DOCUMENT_STREAM_NEW, BF_COMMAND_DOCUMENT_STREAM_FETCH, and
                   BF_COMMAND_DOCUMENT_STREAM_DONE when loading document in chunks from multiple buffers (streaming).
                   The bUseStreamingParser member of BFPreferences is no longer relevant and has been removed.
 * 2005/04/06 JE : renamed BF_COMMAND_VIEW_SET_VIEWPORT to BF_COMMAND_VIEW_FIT_TO_VIEWPORT for clarity.
 * 2005/04/15 SH : added BFViewParams, BF_COMMAND_VIEW_GET, BF_COMMAND_VIEW_SET
 * 2005/05/03 SH : removed commands BF_COMMAND_VIEW_EXPORT and BF_COMMAND_PREFERRED_FONT
 * 2005/05/05 SH : added pfnClearCallback and pClearUserData fields to BFFrameBuffer
 * 2005/05/21 AE : Changed BF_COMMAND_DOCUMENT_NEW and BF_COMMAND_DOCUMENT_REQUEST to not return requests
 * 2005/05/21 AE : Added new command, BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST, which returns the next pending request
 * 2005/05/21 AE : Added new command, BF_COMMAND_DOCUMENT_SET_NEW_REQUEST_CB, which gets triggered when a new request is pending
 * 2005/05/25 AE : Added new command BF_COMMAND_DOCUMENT_GET_PENDING_REQUEST_COUNT
 * 2005/05/30 AE : Renamed BFSoundObject to BFAudioObject, and corresponding commands
 * 2005/05/30 AE : Added 2-stage intialization for BFVideoObject and BFAudioObject. The commands BF_COMMAND_AUDIO_OBJECT_COMPLETE_CALLBACK
 *                 and BF_COMMAND_VIDEO_OBJECT_COMPLETE_CALLBACK.
 * 2005/06/20 SH : Added BF_COMMAND_VIEW_GET_VIEWPORT command to the list of streaming-friendly commands
 * 2005/06/20 SH : Removed BF_COMMAND_POINTER_LEAVE
 * 2005/06/28 SH : Removed uRequestId from BFRequestDone. Now the request are identified by filename
 * 2005/07/06 SH : Added BF_COMMAND_DOCUMENT_SET_ANIMATION_NOTIFY_CB command and BFSetAnimationNotifyCallback
 *                 structure. Removed bAnimated and bBackwardSeekingAllowed from BFNewDocument
 * 2005/07/06 JE:  Cleaned up callback define, typedef and struct member naming: defines all end with _CB, typedef's end with
 *                 'Callback' and struct members are always called pfnCB and pUserData. Also rearranged and subgrouped some
 *                 of the BF_COMMAND enumerations. Ensure binaries are up to date with this header file as the enumeration
 *                 values have changed.
 * 2005/07/21 AE:  Removed bUnknownOpcode from BFNewDocument. Added new command BF_COMMAND_DOCUMENT_VERIFY which checks for
                   optional conformance criteria - bUnknownOpcode, bMissingRequiredAttribute, bCircularRef.
 * 2005/07/26 AE:  Added bRequired to BFNextRequest
 * 2005/07/26 SH:  Doc general cleanup
 *                 Removed commands BF_COMMAND_DOCUMENT_GET_TOTAL_PAGES,
 *                 BF_COMMAND_DOCUMENT_SET_CURRENT_PAGE and BF_COMMAND_DOCUMENT_GET_CURRENT_PAGE
 *                 Renamed command BF_COMMAND_AUDIO_OBJECT to BF_COMMAND_AUDIO_SETUP
 *                 Renamed command BF_COMMAND_AUDIO_OBJECT_COMPLETE_CB to BF_COMMAND_AUDIO_INIT_COMPLETE
 *                 Renamed command BF_COMMAND_VIDEO_OBJECT to BF_COMMAND_VIDEO_SETUP
 *                 Renamed command BF_COMMAND_VIDEO_OBJECT_COMPLETE_CB to BF_COMMAND_VIDEO_INIT_COMPLETE
 * 2005/07/26 SH:  Removed commands BF_COMMAND_VIEW_USER_TO_DEVICE_SPACE and
 *                 BF_COMMAND_VIEW_DEVICE_TO_USER_SPACE
 *                 Removed bImagesToDecode from BFNextRequest.
 *                 Added BF_COMMAND_DOCUMENT_SET_NEW_IMAGE_CB to notify of new pending image decoding requests
 * 2005/08/23 AE:  Removed POINTER_OVER command - consolidated with POINTER_MOVE
 * 2005/09/01 SH:  Moved BFForeignObjectContext to BFBasicType's BFForeignObject
 * 2005/08/26 AE : Added ECMAScript plugin.
 * 2005/09/20 AE : Added command BF_COMMAND_DOM_GET_FOCUS to allow quick determination of focused element bbox.
 * 2005/10/17 AE : Removed BF_COMMAND_AUDIO_INIT_COMPLETE, BF_COMMAND_VIDEO_INIT_COMPLETE
 * 2005/10/17 AE : Added BF_COMMAND_AUDIO_SET_DURATION, BF_COMMAND_VIDEO_SET_DURATION
 * 2005/10/31 SH : Added bCenterLocation to BFRotateData
 * 2005/12/18 AE : Added BF_COMMAND_DOCUMENT_SET_GOTO_LOCATION_CB command and BFSetGotoLocationCallback
 *                 structure.
 * 2005/12/18 AE : Added BF_COMMAND_DOM_GET_SVGGLOBAL_OBJECT
 * 2006/01/10 SH : BF_COMMAND_VIEW_FIT_TO_VIEWPORT does not take a parameter anymore.
 * 2006/01/15 AE : Added 'Ajax' commands:
                       BF_COMMAND_DOCUMENT_SET_GETURL_CB
                       BF_COMMAND_DOCUMENT_SET_POSTURL_CB
                       BF_COMMAND_DOCUMENT_HTTP_REQUEST_COMPLETE
 * 2006/02/13 AE : Added text editing paramaters and BF_COMMAND_COMPLETE_TEXT_EDIT.
 * 2006/02/15 SH : Added field bClosingSVGTagFound to BFNewDocument
 * 2006/02/23 SH : removed BF_COMMAND_VIEW_PAINT. No more internally created surfaces.
 * 2006/02/23 SH : removed BF_COMMAND_DOCUMENT_GET_PENDING_REQUEST_COUNT.
 * 2006/03/14 SH : added font setting data BFPreferences
 * 2006/04/06 AE : added bNullFocus to BFPreferences
 * 2006/05/09 SH : added uLinePadding to BFFrameBuffer
 * 2006/05/18 AE : BF_COMMAND_KEY_DOWN, BF_COMMAND_KEY_UP now take BFKeyIdentifier instead of uDeviceKey
 * 2006/05/18 AE : BF_COMMAND_TEXT_INPUT now takes BFTextEvent structure.
 * 2006/05/18 AE : removed BF_COMMAND_DOCUMENT_SET_KEY_MAPPING_CB
 * 2006/05/19 SH : added uDpi to BFFrameBuffer
 * 2006/05/30 SH : renamed BF_COMMAND_DOCUMENT_GET_ANIMATION_STATE to BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS.
 *                 Now takes new struct BFDocumentRedrawStatus as parameter
 * 2006/06/16 SH : removed bHandled from BFPointerPickEvent.
 * 2006/07/07 SH : added fpRawWidth / fpRawHeight to BFPlayerExtent
 * 2006/07/13 SH : VIEW_BUFFER does not reset the view anymore if the buffer size does not change
 * 2006/07/15 AE : added bEnableJSR226 to BFPreferences due to divergence with 1.2 and JSR-226 specifications
 * 2006/07/25 AE : removed bHandled from BF_COMMAND_POINTER_CLICK,
                   BF_COMMAND_POINTER_DOWN, BF_COMMAND_POINTER_MOVE, BF_COMMAND_POINTER_UP,
                   BF_COMMAND_FOCUS_ACTIVATE. Use BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS instead.
 * 2006/07/30 AE : removed bRedraw and uDocumentTime from BFPlayerAdvanceTime.
 * 2006/08/29 JE : added pEditableText to BFPointerClickEvent, so that pointer clicks (via BF_COMMAND_POINTER_CLICK)
 *                 can be used to activate editable text, in the same manner as BF_COMMAND_FOCUS_ACTIVATE.
 * 2006/09/08 SH : renamed BFFrameBuffer's uLinePadding to iLinePadding. Now negative values accepted
 * 2006/09/08 AE : added BF_COMMAND_DOCUMENT_GET_CURRENT_TIME
 * 2006/09/18 AE : removed bEnableJSR226 to BFPreferences - all differences can be done at 226 level
 * 2006/11/03 AE : added bDisable12Focus to BFPreferences
 * 2006/11/28 AE : added bAnimationElementsFound and bEventTriggeredAnimationsFound to BFNewDocument
 * 2007/04/10 JE : added BF_COMMAND_VIEW_BUFFER_DISABLE
 * 2007/05/24 AE : Removed BF_COMMAND_DOM_GET_SVGGLOBAL_OBJECT - now part of uDOM Document interface
 * 2007/07/11 SH : Added BF_COMMAND_FONT_ENGINE_SETUP, BF_COMMAND_FONT_SET_DEFAULT_FAMILY
 * 2007/08/08 JE : updated BF_COMMAND_VIEW_GET_VIEWPORT to retrieve the original intrinsic document dimensions,
 *                 including units (in, cm, mm, pc, pt, px, %)
 * 2007/08/15 SH : Added BFKeyModifier to BFKeyEvent
 * 2007/08/20 LM : Added return of SVG end tag postion in document info
 * 2007/09/05 SH : Added WIDGET commands / types
 * 2007/10/23 JE : Added BF_COMMAND_CACHE_CONTROL
 * 2007/11/29 SH : Updated BFRequestDone to accept pre-decoded images
 * 2007/12/10 JE : Added alpha field to BFBackgroundColor. If alpha < 255 then the rendered result from the player
                   should be considered an intermediate image, in premultiplied alpha format, intended for blending
                   with a suitable background.
 * 2007/12/10 JE : BF_COMMAND_VIEW_ENABLE_ALPHA_CHANNEL has been deprecated, it now provides no functionality as
                   alpha is always enabled since the introduction of premultiplied alpha pixel format.
 * 2008/03/18 DJ : Added BF_COMMAND_IMAGE_DECODER_REGISTER.
 * 2008/03/24 JE : Added bDisableImplicit field to BFCacheControl for disabling implicit caching (defaults 'static' to
 *                 false rather than auto)
 * 2008/04/09 AE : Reorganized for Documentation.
 */


#ifndef BITFLASH_PLAYERTYPES_H_
#define BITFLASH_PLAYERTYPES_H_

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

#ifndef BF_DOXYGEN_SKIP
/* Flags internally used by the player */
#define BF_FLAG_NEED_PDATA      0x40000000L
#define BF_FLAG_NEED_DOCUMENT   0x20000000L
#endif /* BF_DOXYGEN_SKIP */

/** The current version of the BFSVGT Engine in string format. Can be useful for run-time detection of version in about boxes, etc */
#define BF_VERSION_STRING "6.4.0_2271s"


/** Commands accepted by the player */
typedef BF_UINT32 BFPlayerCommand;

/**
 * List of commands accepted by the player. Depending on
 * the configuration, the implementation of some of the commands may not be available
 * and therefore will be ignored by the player.<br>
 * \internal
 * For structure/parameter details, refer to (1).<br>
 * For command descriptions, refer to (2).
 */
enum
{
    /* Command name                      Required parameters                  */

    /********** Document commands **********/
    /**
       Creates the DOM representation of a document from a buffer which contains
       SVG or zipped SVG.
       Parameters is a pointer to a  BFNewDocument structure.<br>
       The active document is deleted and replaced by the parsed document. Note that
       the entire SVG or zipped SVG must reside in the input buffer when using
       BF_COMMAND_DOCUMENT_NEW.
       If the user agent wants to use the streaming aspect of the parser (and input the SVG or zipped
       SVG in chunks), use the set of commands
       BF_COMMAND_DOCUMENT_STREAM_NEW, BF_COMMAND_DOCUMENT_STREAM_FETCH, and
       BF_COMMAND_DOCUMENT_STREAM_DONE.

       Returned information includes some error reporting and document related information
       (does it allow pan and zoom operations?).  If the user provided buffer contains
       a top level SVG close tag, the returned document information will have the bClosingSVGTagFound
       flag set and uDocumentEndOffset will contain the offset from the start of the buffer to the
       first byte after the close tag.  Any other data is allowed to follow the closing SVG tag,
       providing the data begins with a well formed XML start tag (not necessarily SVG).
       uDocumentEndOffset is only valid for user buffers containing uncompressed content.

       In order to retreive external data requests form the new document, the command
       BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST is called. Once data for each request has
       been retreived, a call is made to BF_COMMAND_DOCUMENT_REQUEST to satisfy the request.

       The field bUnknownOpcode in BFNewDocument is set to BF_TRUE if the content contains
       any elements or attributes which are not part of tiny.
    */
    BF_COMMAND_DOCUMENT_NEW           /* BFNewDocument                        */
    = 0 | BF_FLAG_NEED_PDATA,
    /**
       Initiates input streaming for the creation of the DOM representation of a document
       from an SVG or zipped SVG.
       Parameter is a pointer to a  BFNewDocument structure.
       If the user agent doesn't want to use the streaming aspect of the parser (and instead wants to input the SVG or zipped
       SVG in one chunk), it should use the command BF_COMMAND_DOCUMENT_NEW.

       This command does not read or process
       any SVG data. Chunks of data has to be passed in through BF_COMMAND_DOCUMENT_STREAM_FETCH.
       In streaming context, once
       BF_COMMAND_DOCUMENT_STREAM_NEW has been called, only BF_COMMAND_DOCUMENT_STREAM_FETCH
       plus a limited set of progressive-rendering-friendly commands can be called until
       BF_COMMAND_DOCUMENT_STREAM_DONE is called.

       It is therefore not possible to open two document simultaneously in the same player.
       BF_COMMAND_DOCUMENT_STREAM_DONE has to be called, regardless
       of the status returned by BF_COMMAND_DOCUMENT_STREAM_NEW or
       BF_COMMAND_DOCUMENT_STREAM_FETCH.
       When opening gzip or zlib data in a streamable way, the first data chunk
       passed in through BF_COMMAND_DOCUMENT_STREAM_NEW has to be big enough to contain
       the whole gzip/zlib header (at least 10 bytes, potentially more for gzip,
       depending of the variable length of data stored in the header).
       <br><br>
    */
    BF_COMMAND_DOCUMENT_STREAM_NEW    /* BFNewDocument                        */
    = 1 | BF_FLAG_NEED_PDATA,
    /**
       To be called by the user agent when more data is available for streaming parser.
       This command can be called only after BF_COMMAND_DOCUMENT_STREAM_NEW has been called in a
       streaming parser context. The data passed in can be a chunk of raw SVG or of a zlib-compressed
       SVG.
       Parameter is a pointer to a BFNewDocument structure. Only pBuffer and uBufferSize fields are used.

       This command also supports returning document information as with the NEW_DOCUMENT command.
       The uDocumentEndOffset value returned during streaming is only valid on the first occurrence
       of bClosingSVGTagFound returnung true; if the caller continues to stream data to the same document after
       bClosingSVGTagFound has been set, the offset will return zero and no data will be processed.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_STREAM_FETCH  /* BFNewDocument                        */
    = 2 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       To be called by the user agent when all data for streaming parsing has be fed to the player.
       This command finalizes document building.
       This command can be called only after BF_COMMAND_DOCUMENT_STREAM_NEW has been called in a
       streaming parser context.
       Parameter is a pointer to a BFNewDocument structure. pBuffer and uBufferSize fields are not used, only
       output fields are set with return status of the document creation.
       When parsing error or warnings are generated, only the portion of the context available in
       the data chunk in which they occured can be quoted. In other words, if an error happens
       only a few characters after the beginning of a chunk, the error context might be very terse.
       If the user agent doesn't want to use the streaming aspect of the parser, it doesn't need this function (see
       BF_COMMAND_DOCUMENT_NEW).
       <br><br>
    */
    BF_COMMAND_DOCUMENT_STREAM_DONE   /* BFNewDocument                        */
    = 3 | BF_FLAG_NEED_PDATA,
    /**
       Deletes the currently active document, including the entire DOM representation (No parameters).
       <br><br>
    */    BF_COMMAND_DOCUMENT_DELETE        /* No parameters                        */
    = 4 | BF_FLAG_NEED_DOCUMENT,
    /**
       Sends the result of the external data requests (see BF_COMMAND_DOCUMENT_NEW ).
       Parameter is a pointer to a  BFRequestDone structure.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_REQUEST       /* BFRequestDone                        */
    = 5 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Gets the next pending external data request. The structure member
       bRequestsToService will be BF_TRUE if a new request exists. The typical use
       case is for implementations to loop through, calling BF_COMMAND_DOCUMENT_REQUEST until
       no requests are left.

       Parameter is a pointer to a BFNextRequest structure.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST /* BFNextRequest            */
    = 6 | BF_FLAG_NEED_DOCUMENT | BF_FLAG_NEED_PDATA,
    /**
       Setup a callback that will be called when the player adds a new request.
       This callback is called whenever a new request has been posted to the requests list.
       See BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST for details.
       These commands should *not* be called from within the callback; instead,
       a boolean should be set or a message posted to an event queue, indicating that there
       are file requests to service.
       This callback might be called many times consecutively during document parsing but those
       requests should be serviced *after* parsing completes.
       This callback is also called when an xlink:href attribute is dynamically set to a new
       external resource value using the uDOM.
       It could also be called when the attribute xlink:href is being animated.
       If the callback is not registered, the user agent will not be notified of new requests
       but the pending requests can be retrieved using BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST.
       Parameter is a pointer to a BFSetNewRequestCallback
       <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_NEW_REQUEST_CB /* BFSetNewRequestCallback         */
    = 7 | BF_FLAG_NEED_PDATA,
    /**
       Makes the player decode the next pending bitmap image attached to the
       current document. This command should be called repetedly until it returns
       BF_PLAYER_ILLEGAL_COMMAND, indicating that there are no more images to decode.
       On platforms supporting asynchronous image decoding, this commands
       only *launches* the image decoding process, returning quickly while the
       OS decodes the image in the background.

       Warning : if this command returns an error, the user might still want to
       resume decoding the remaining bitmaps in order to reach the "document
       complete" status (see BF_COMMAND_DOCUMENT_SET_COMPLETENESS_CB).

       No parameter.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_DECODE_NEXT_BITMAP /* No parameters                   */
    = 9 | BF_FLAG_NEED_DOCUMENT,
    /**
       Specify this callback if the user agent wants to be notified when the document is complete and
       ready to be rendered.

       This callback is called at most once for each document and can be called only when
       the following criteria are met :
       - there are no pending images to decode
       - there are no pending asynchronous image being decoded
       - there are no pending file requests

       The document completeness is checked (i.e. the callback can be called) in the following
       circumstances :
       - *just before* BFprocessCommand returns from commands BF_COMMAND_DOCUMENT_NEW
         (if not using streaming parser) or BF_COMMAND_DOCUMENT_STREAM_DONE or
         BF_COMMAND_DOCUMENT_REQUEST or BF_COMMAND_DOCUMENT_DECODE_NEXT_BITMAP
       - When the last asynchronous image finishes decoding

       When an incomplete document is replaced with another one while asynchronous
       images are still being decoded, the callback is not called for the old document.

       Typically, use this callback if the user agent needs to start rendering the document
       only once all images have been decoded, i.e. if the images
       shall not show up with some delay : in the callback, simply set a flag indicating that
       the document complete and ready to be rendered once the current function exits.
       It is not possible to call any player command from this callback to avoid call stack overloading.
       The document can be rendered before this callback has been called, but it will not
       contain all he images at that time.

       When the callback is called, the parameter uDecodeStatus is set to
       - BF_NO_ERROR if asynchronous image decoding completed successfully
       - BF_PLAYER_OUT_OF_MEMORY if at least one of them ran out of memory
       - BF_PLAYER_IMAGE_DECODER_ERROR if at least one of them failed decoding
       When both BF_PLAYER_OUT_OF_MEMORY and BF_PLAYER_IMAGE_DECODER_ERROR occur
       during image decoding, BF_PLAYER_OUT_OF_MEMORY is returned.
       The parameter uNbAsyncImages is set to the total number of asynchronous images
       that have been decoded (successfully or not).

       Parameter is a pointer to a BFSetCompletenessCallback structure.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_COMPLETENESS_CB /* BFSetCompletenessCallback      */
    = 10 | BF_FLAG_NEED_PDATA,
    /**
       Sets the new document time in milliseconds (allows to forward/rewind animations).
       This command is not intended to advance time in a normal manner<br>
       use BF_COMMAND_DOCUMENT_ADVANCE_TIME for this case. This command is<br>
       used to get a snapshot of the animation at a given time.<br>
       Parameter is a pointer to an integer containing the new time in milliseconds (BF_UINT32).<br>
       Time equal to 0 means that the animation is rewound to the start.<br>
       <br><br>
       <hr>
    */
    BF_COMMAND_DOCUMENT_SEEK_TIME     /* BF_UINT32                            */
    = 11 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Advances the document time by a given delta value. Typically called
       with the delay provided by a call to BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS.

       Usually called before the next redraw or before calling user event commands ( see
       BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS ).

       Parameter is a pointer to a  BFPlayerAdvanceTime structure.<br>
       <br><br>
    */
    BF_COMMAND_DOCUMENT_ADVANCE_TIME  /* BFPlayerAdvanceTime                  */
    = 12 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Retreives the snapShotTime attribute of the top-most SVG element.
       The value is in MS, and is 0 if no attribute is found.
       This can be used with BF_COMMAND_DOCUMENT_SEEK_TIME to create a
       snapshot of the SVG file for print-preview, testing, etc.
       Parameter is a pointer to a BF_UINT32 for the return of the snaphsot time value;
       <br><br>
    */
    BF_COMMAND_DOCUMENT_GET_SNAPSHOT_TIME  /* BF_UINT32                       */
    = 13 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Returns the redraw status of the current document.
       Parameter is a pointer to a BFDocumentRedrawStatus enum.

       The field uNextSignificantTime gives information about when the
       next frame is required:

       - If uNextSignificantTime == 0, it means that the document requires an
         immediate redraw.
       - If uNextSignificantTime == (BF_UINT32)-1, it means that animations and script are currently
         idled, but might start again at a later, unknown time. This typically happens
         upon user events.
       - If uNextSignificantTime > 0, it means that a redraw is needed deterministically in
         uNextSignificantTime milliseconds.

       Typically, any action which may change the status of the document redraw state
       should call BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS. These are any commands which are
       responses to user events:

       - BF_COMMAND_POINTER_CLICK,
       - BF_COMMAND_KEY_DOWN,
       - BF_COMMAND_KEY_UP,
       - BF_COMMAND_TEXT_INPUT,
       - BF_COMMAND_POINTER_DOWN,
       - BF_COMMAND_POINTER_MOVE,
       - BF_COMMAND_POINTER_UP

       Also, scripting can be registered on events triggered by the following commands:

       - BF_COMMAND_ZOOM,
       - BF_COMMAND_PAN,
       - BF_COMMAND_ROTATE

       The sequence of events for determining redraw status and maintaining the timeline are
       as follows:
       1) Application receives key down
       2) ADVANCE_TIME is called, to advance the time from last update and top ensure the
          document has the most up-to-date time
       3) KEY_DOWN command is called
       4) GET_REDRAW_STATUS is called and a redraw is scheduled, if needed
       5) after each redraw, GET_REDRAW_STATUS is called to determine when, if at all, another
          redraw is required.

       <br><br>
    */
    BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS /* BFDocumentRedrawStatus      */
    = 14 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       This method is depreciated and is replaced by the BFNewDocument.bAnimationElementsFound and
       BFNewDocument.bUserEventAnimationTriggersFound members.

       Setup a callback that will be called when a document is found to contain
       animations. The user agent should use this notify to start driving the animation
       (i.e. call BF_COMMAND_DOCUMENT_ADVANCE_TIME and BF_COMMAND_VIEW_REDRAW on a regular
       basis). These commands should *not* be called from within the callback; instead
       timers or event-based mechanism should be started by the user agent, if they have
       not been started already.
       This callback is typically called during document parse time, either of
       the main document or of the external resources (BF_COMMAND_DOCUMENT_REQUEST).
       This callback might be called multiple times during a single command.
       If this callback is not registered, the user agent will not be notified whether
       the document contains animations but it can still systematically drive the
       animation by calling the corresponding commands, at the expense of CPU (and thus
       battery, if relevant).
       When the callback is called, the parameter bBackwardSeekingAllowed indicates
       whether the SVG document supports seeking backwards in the timeline.

       Parameter is a pointer to a BFSetAnimationNotifyCallback
       <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_ANIMATION_NOTIFY_CB /* BFSetAnimationNotifyCallback */
    = 15 | BF_FLAG_NEED_PDATA,
    /**
       Specifies a callback which is called regularly during document rendering. When
       registered, the callback is called after every single SVG element.

       The callback function must return one of the following values :
       - BF_PLAYER_INTERRUPT_CONTINUE, to resume the current operation immediately
       - BF_PLAYER_INTERRUPT_PAUSE, to pause the current operation, returning
         immediately from the call stack. Operation will have to be resumed using
         BF_COMMAND_DOCUMENT_CONTINUE. This gives a chance to the user agent to
         process incoming events or do other tasks before resuming the CPU-intensive
         SVG rendering.
       - BF_PLAYER_INTERRUPT_ABORT, to abort the current operation and return
         immediately from the call stack. Operation cannot be resumed and
         returns BF_ABORTED.
       Parameter is a pointer to a BFSetInterruptCallback structure.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_INTERRUPT_CB /* BFSetInterruptCallback            */
    = 16 | BF_FLAG_NEED_PDATA,
    /**
       Resumes a command previously interrupted (which has returned BF_INTERRUPT).
       Parameter is a pointer to a BFContinue structure

       If the bAbort field is set to BF_TRUE, this forces the continued command to be aborted asap,
       as if the continue callback had returned BF_PLAYER_INTERRUPT_ABORT, even though this
       callback is not called anymore (see BF_COMMAND_DOCUMENT_SET_INTERRUPT_CB).
       <br><br>
    */
    BF_COMMAND_DOCUMENT_CONTINUE      /* BFContinue                           */
    = 17 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       This command checks the document for additional conformance criteria. Used mainly for additional
       conformance placed on SVG by the JSR-226 specification.
       Should be used whenever a new document is created, or a new fragment is added to the document - ie
       after all BF_COMMAND_DOCUMENT_REQUEST commands.

       Parameter is a pointer to a BFDocumentVerify structure
       <br><br>
    */
    BF_COMMAND_DOCUMENT_VERIFY        /* BFDocumentVerify                     */
    = 21 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Setup a callback that will be called when the player comes across a new image
       that needs to be decoded.
       This callback is only called if bManualImgDecoding is set to BF_TRUE in the preferences
       (see BF_COMMAND_PREFERENCES). When this callback is called, it means that there are
       new images waiting to be decoded (through BF_COMMAND_DOCUMENT_DECODE_NEXT_BITMAP).

       The callback is called every time an image decode request is added to the internal
       image decode requests list, so it can be called multiple times repetedly. The user agent
       cannot call any command from within this callback; instead it should post messages
       to its own messages queue or use any other technique to decode the images asynchronously.

       Even if the callback is not registered, the user agent can still process manual image
       decodings by calling BF_COMMAND_DOCUMENT_DECODE_NEXT_BITMAP anytime.

       Typically, there can be new images to be decoded after the main SVG document finishes parsing,
       and after a file request has been serviced, but with the uDOM, it is also possible to
       dynamically assign a b64 value to an xlink:href attribute, generating a new decode request,
       hence the need for a callback.

       Parameter is a pointer to a BFSetNewImageCallback
       <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_NEW_IMAGE_CB /* BFSetNewImageCallback             */
    = 22 | BF_FLAG_NEED_PDATA,
    /**
       Setup a callback that will be called when the user agent needs to
       load a new document. Called in response to a SVGGlobal.gotoLocation uDOM
       call.

       User Agents implementing this callback should take care not to load a new document
       until the current player command is complete.

       Parameter is a pointer to a BFSetGotoLocationCallback
       <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_GOTO_LOCATION_CB /* BFSetGotoLocationCallback     */
    = 23 | BF_FLAG_NEED_PDATA,
    /**
        This command will setup a callback which will be triggered when the user agent needs to perform a
        getURL request. This is usually triggered via the uDOM. Once the request is complete, the command
        BF_COMMAND_DOCUMENT_HTTP_REQUEST_COMPLETE is used to send the response to the SVG player.
        <br>
        Parameter is a pointer to a BFHTTPRequestComplete structure.
        <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_GETURL_CB /* BFSetGetURLCallback                  */
    = 24 | BF_FLAG_NEED_PDATA,
    /**
        This command will setup a callback which will be triggered when the user agent needs to perform a
        postURL request. This is usutally triggered via the uDOM. Once the request is complete, the command
        BF_COMMAND_DOCUMENT_HTTP_REQUEST_COMPLETE is used to send the response to the SVG player.
        <br>
        Parameter is a pointer to a BFSetPostURLCallback structure.
        <br><br>
    */
    BF_COMMAND_DOCUMENT_SET_POSTURL_CB /* BFSetPostURLCallback                */
    = 25 | BF_FLAG_NEED_PDATA,
    /**
       This command returns a HTTP response back to the player.
        <br>
        Parameter is a pointer to a BFSetPostURLCallback structure.
       <br><br>
    */
    BF_COMMAND_DOCUMENT_HTTP_REQUEST_COMPLETE /* BFHTTPRequestComplete        */
    = 26 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Retreives the current time. This is the master sync time and is not the same
       as individual time container times. For exmaple, the sync time is always updated
       with a SEEK_TIME or ADVACNE_TIME call, but timeline containers like the top-level SVG,
       animation, audio and video elements may not be the same time as the master sync time,
       depending on sync status.
       Parameter is a pointer to a BF_UINT32 for the return of the time value;
       <br><br>
    */
    BF_COMMAND_DOCUMENT_GET_CURRENT_TIME  /* *BF_UINT32                       */
    = 27 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,

    /**
        Set the values of system colors, the default value is 0x000000

        Parameter is a pointer to a BFColorMappings structure

    */
    BF_COMMAND_DOCUMENT_SET_SYSTEM_COLOR /* BFColorMappings                  */
    = 45 | BF_FLAG_NEED_PDATA,
    /**
        Used to initialze player services dynamically based on content. Typically a player
        is initialized with services before loading a SVG document.

        This mechanism can be used to initialze various services only if required by the content,
        during paring.

        Parameter is a pointer to a BFSetServiceSetupCallback structure
    */

    /**
        Specifies a callback that will be called when player needs to resolve
        an external script. Cannot be used with normal resource loading since the
        script needs to be resolved while parsing, within the context of th eplayer

        Paramter is a pointer to a BFSetGetScriptResourceCallback structure
    */
    BF_COMMAND_SET_GET_SCRIPT_RESOURCE_CB /* BFSetGetScriptResourceCallback                  */
    = 46 | BF_FLAG_NEED_PDATA,


    BF_COMMAND_SERVICE_SETUP_CB         /* BFSetServiceSetupCallback         */
    = 98 | BF_FLAG_NEED_PDATA,
    /**
        Specifies a callback that will be called when player logging any status
        information. When registered, the callback is called during the logging
        process by player according to logging type settings.

        Paramter is a pointer to a BFSetLoggingCallback structure
    */
    BF_COMMAND_DOCUMENT_SET_LOGGING_CB /* BFSetLoggingCallback                  */
    = 99 | BF_FLAG_NEED_PDATA,


    /************ View commands ************/
    /**
       Set the internal render buffer. The buffer may have a different size from the previous
       buffer set with this function or from the size specified at player creation time.
       This commands resets the current zoom, pan and rotate factors unless the new buffer
       has exactly the same size as the current one.

       The BF_COMMAND_VIEW_REDRAW must be sent
       after this command in order to ensure consistent animation behaviour.

       If the command fails with BF_PLAYER_OUT_OF_MEMORY, the application shall assume that no
       render buffer is assigned to the player anymore. BF_COMMAND_VIEW_REDRAW will thus
       return without rendering anything. The only way to recover from this error is to call
       the command again with a smaller buffer size. Though the command doesn't allocate the
       buffer, it allocates some data structures whose sizes are proportional to the buffer
       width and height, hence the potential memory failure with large buffers.

       Parameter is a pointer to a BFFrameBuffer structure, defining a render buffer of
       the specified size. This buffer is made of a succession of lines following each other in memory.
       Each line has to be double-word aligned (padding might thus be necessary for pixel sizes
       smaller than 4 bytes). Pixel size must be the one statically supported by the implementation
       of the target platform.

       An optional iLinePadding can be provided. This value represents the number of pixels to
       skip at the end of a line to reach the beginning of the next line. This allows to use
       only a rectangular sub-region of a larger buffer. With a negative value, this allows,
       when used in conjunction with appropriate clipping region, to render into an area
       larger than the physically allocated buffer.

       uDpi is the buffer resolution in dots per inch. This is used to compute the actual
       size of the viewport when it is specified with "human" units like 'cm'. If this parameter
       is left to 0, the value 72dpi is used instead, which is a common DPI value for
       desktop system screens.

       If either the width or the height of the supplied buffer are zero, the command
       returns with BF_NO_ERROR but doesn't do anything.

       An optional callback can be registered using pfnClearCallback / pClearUserData fields.
       If specified, this callback is being called each time a region of the buffer has to
       be cleared. This gives a chance to the caller to fill the buffer with background data
       which is an efficient alternative to full-scale buffer transparency.
       <br><br>
    */
    BF_COMMAND_VIEW_BUFFER            /* BFFrameBuffer                        */
    = 100 | BF_FLAG_NEED_PDATA,
    /**
       Specify the default color used to initialize the SVG viewport
       Parameter is a pointer to a BFBackgroundColor structure.
       <br><br>
    */
    BF_COMMAND_VIEW_SET_BACKGROUND_COLOR /* BFBackgroundColor                 */
    = 101 | BF_FLAG_NEED_PDATA,
    /**
       Specify the default color used to initialize the player buffer area.
       The player buffer area will only be visible before a SVG document
       is opened or when an opened SVG document has a viewport smaller than this area.
       Parameter is a pointer to a BFBackgroundColor structure
       <br><br>
    */
    BF_COMMAND_VIEW_SET_OVERFLOW_COLOR /* BFBackgroundColor                   */
    = 102 | BF_FLAG_NEED_PDATA,
    /**
       Works only with 32-bit color formats. Enables the alpha channel so that the
       alpha values are retained in the buffer. Slows down rendering by at least 20%
       due to the additional math per pixel.

       Parameter is a pointer to a BF_BOOL.
       <br><br>
    */
    BF_COMMAND_VIEW_ENABLE_ALPHA_CHANNEL /* BF_BOOL                           */
    = 103 | BF_FLAG_NEED_PDATA,
    /**
       Magnifies the current view by the specified zoom factor (expressed in
       16.16 fixed point).
       Parameter is a pointer to a BFZoomEvent structure.<br>
       The zoom factor is always positive and operates as a percentage.<br>
       For example:
       - zooming in by 2:  specify 0.5 (equivalent to 50% of the view)
       - zooming out by 2: specify 2.0 (equivalent to 200% of the view)<br>
       The region that remains within the view area depends on the values
       of iZoomX and iZoomY. If either iZoomX or iZoomY is equal to BF_FLAG_ZOOM_USE_CENTER
       then the point that was at the centre of the view area before the
       zoom will still be at the centre of the view area after the zoom.
       Otherwise iZoomX and iZoomY are treated as screen coordinates within
       the view area and how they affect the positioning after the zoom
       depends upon bCenterLocation. If bCenterLocation is BF_TRUE then the
       point (iZoomX, iZoomY) will be at the centre of the view area after
       the zoom. If bCenterLocation is BF_FALSE then the point (iZoomX, iZoomY)
       will be at the origin (top left corner) of the view area after the
       zoom.
       <br><br>
    */
    BF_COMMAND_VIEW_ZOOM              /* BFZoomEvent                          */
    = 104 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Pans the view by the given dx and dy.
       Parameter is a pointer to a BFPanEvent structure.<br>
       If bFastScroll is BF_TRUE, provides an instant feedback of the panning
       action by panning the buffer. The missing strips of the image needs to be
       redrawn with a BF_COMMAND_VIEW_REDRAW command, using the clipping region supplied.
       If both dx and dy are non-zero, the entire region is redrawn. To effect fast scrolling
       when both dx and dy are non-zero do the pan/redraw steps twice, for (dx,0) and (0,dy)
       NOTE: due to precision considerations the dx and dy members of the BFPanEvent may be
       modified upon return from processing this command. If bFastScroll is BF_TRUE and in
       the event that dx and dy are both zero on output the clipping region supplied
       will not be applicable (no panning will have occurred) and a redraw using
       BF_COMMAND_VIEW_REDRAW can be avoided.

       <br><br>
    */
    BF_COMMAND_VIEW_PAN               /* BFPanEvent                           */
    = 105 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Resets the zoom, scale and rotation factors to re-display the original view.
       No parameter.
       <br><br>
    */
    BF_COMMAND_VIEW_RESET             /* No parameters                        */
    = 106,
    /**
       This command forces the player to ignore the documents' width/height attributes, making its content
       fit to the player viewport (when a viewBox is specified).
       <br>
        No parameter.
        <br>
    */
    BF_COMMAND_VIEW_FIT_TO_VIEWPORT   /* No parameters                        */
    = 107,
    /**
       Rotates the document by the given angle around the center position.
       The rotation angle is a relative angle, meaning that several calls of this
       command cumulate the rotation angle applied to the document.
       This ONLY applies the rotation  paramaters - a redraw must be done by the caller.
       Parameter is a BFRotateData structure
       <br><br>
    */
    BF_COMMAND_VIEW_ROTATE            /* BFRotateData                         */
    = 108 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Traverses the DOM tree and updates the Render Buffer.
       This command acknowledges any action such as zooming, panning, animation time update, focus change, etc.
       It is certainly the most CPU-intensive command of this API but can be interrupted if necessary
       (see BF_COMMAND_DOCUMENT_SET_INTERRUPT_CB for details).

       Parameter is an optional BFPlayerRegion to be used as clipping region
       <br><br>
    */
    BF_COMMAND_VIEW_REDRAW            /* BFPlayerRegion (optional )           */
    = 109 | BF_FLAG_NEED_DOCUMENT,
    /**
       Returns the viewport used by the current SVG document
       Parameter is a pointer to a BFPlayerExtent structure (return value).
       <br><br>
    */
    BF_COMMAND_VIEW_GET_VIEWPORT      /* BFPlayerExtent                       */
    = 112 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Returns the parameters of the current view (zoom / pan / rotate).
       Parameter is a pointer to a BFViewParams structure (return values).
       <br><br>
    */
    BF_COMMAND_VIEW_GET                /* BFViewParams                        */
    = 113  | BF_FLAG_NEED_PDATA,
    /**
       Specifies the parameters of the current view (zoom / pan / rotate).
       Only call the command with a view that has been obtained through BF_COMMAND_VIEW_GET
       If this command is called on a document that has a different size or aspect ratio
       than the one on which BF_COMMAND_VIEW_GET was called, the resulting view might,
       understandably, not be exactly the same.
       Parameter is a pointer to a BFViewParams structure.
       <br><br>
    */
    BF_COMMAND_VIEW_SET                /* BFViewParams                        */
    = 114 | BF_FLAG_NEED_PDATA,
    /**
       Displays the text only view provided the use of the BitFlash
       Document Viewing System (DVS).
       Parameter is a pointer to a boolean (BF_BOOL).
       <br><br>
    */
    BF_COMMAND_VIEW_TEXTONLY          /* *BF_BOOL                             */
    = 117 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
      Disables or enables the use of the view buffer. If the view buffer is disabled no
      rendering to the buffer will occur that would normally be the result of other player
      commands. This is useful in environments where the memory associated with a view
      buffer may be relocatable and the caller wishes to ensure that the player, through its
      commands or user-supplied callbacks, does not try to access the memory while it
      potentially could be relocated.

      Parameter is a pointer to a BF_BOOL, where BF_TRUE indicates disabling of the view buffer.
       <br><br>
    */
    BF_COMMAND_VIEW_BUFFER_DISABLE    /* BF_BOOL                              */
    = 118 | BF_FLAG_NEED_PDATA,


    /********** Pointer commands ***********/
    /**
       Propagates a Pointer Down event (mouse or pen down) to the DOM.
       Parameter is a pointer to a BFPointerEvent structure.
       <br><br>
    */
    BF_COMMAND_POINTER_DOWN           /* BFPointerEvent                       */
    = 200 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Propagates a Pointer Move event (mouse or pen move) to the DOM.
       Parameter is a pointer to a BFPointerEvent structure.
       Triggers mousemove, mouseover and mouseout events
       <br><br>
    */
    BF_COMMAND_POINTER_MOVE           /* BFPointerEvent                       */
    = 201 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Propagates a Pointer Up event (mouse or pen up) to the DOM.
       Parameter is a pointer to a BFPointerEvent structure.
       <br><br>
    */
    BF_COMMAND_POINTER_UP             /* BFPointerEvent                       */
    = 203 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Propagates a Pointer Click event (mouse or pen click) to the DOM.
       Parameter is a pointer to a BFPointerClickEvent structure.
       <br><br>
    */
    BF_COMMAND_POINTER_CLICK          /* BFPointerClickEvent                  */
    = 204 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Retrieves a list of SVG elements at a given location.
       The element list contains at least one element but may contain more
       if the element picked is rendered within a shadow tree ( use, animation element )
       Parameter is a pointer to a BFPointerPickEvent structure.
       Must call BFFreeCommand to free the returned list
       <br><br>
    */
    BF_COMMAND_POINTER_PICK           /* BFPointerPickEvent                   */
    = 206 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Send a custom event to the DOM.
       Parameter is a pointer to a BFCustomEvent structure.
       <br><br>
    */
    BF_COMMAND_SEND_CUSTOM_EVENT      /* BFCustomEvent                        */
    = 208 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,


    /*********** Key commands **************/
    /**
       Propagates a device key event to the DOM.
       Parameter is a pointer to a BFTextEvent structure.
       Device keys must be translated into a key character.
       <br><br>
    */
    BF_COMMAND_TEXT_INPUT             /* BFTextEvent                          */
    = 300 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Propagates a device key down event to the DOM.
       Parameter is a pointer to a BFKeyEvent structure.
       Device keys must be translated into a BFKeyIdentifier type. This enumeration is
       a subset of appendix A: KeyBoard events and key identifiers of the DOM Level 3 events spec
       http://www.w3.org/TR/2003/NOTE-DOM-Level-3-Events-20031107/keyset.html#KeySet.
       <br><br>
    */
    BF_COMMAND_KEY_DOWN             /* BFKeyEvent                             */
    = 301 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,

    /**
       Propagates a device key up event to the DOM.
       Parameter is a pointer to a BFKeyEvent structure.
       Device keys must be translated into a BFKeyIdentifier type. This enumeration is
       a subset of appendix A: KeyBoard events and key identifiers of the DOM Level 3 events spec
       http://www.w3.org/TR/2003/NOTE-DOM-Level-3-Events-20031107/keyset.html#KeySet.
       <br><br>
    */
    BF_COMMAND_KEY_UP               /* BFKeyEvent                             */
    = 302 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,


    /*********** Focus commands ************/
    /**
        Activates the element that currently has focus.
        Parameter is a pointer to a BFActivateEvent structure for hyperlink processing.
        <br><br>
    */
    BF_COMMAND_FOCUS_ACTIVATE         /* BFActivateEvent                      */
    = 500 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Changes the focus to the next element in the direction specified
       which accepts it.
       As per the SVG spec, an event DOMFocusOut is triggered on the current
       focus and an event DOMFocusIn is triggered on the new focus.
       Parameter is a pointer to a BFMoveFocusEvent structure.
       <br><br>
    */
    BF_COMMAND_FOCUS_MOVE             /* BFMoveFocusEvent                     */
    = 501 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,

    /**
       Will change the user agent pan coordinates such that any currently focused element
       is in view. The view will not change if there is no focused element or if the currently
       focused element is already in the viewport.
       <br>
       No parameter.
       <br><br>
    */
    BF_COMMAND_FOCUS_LOCATE           /* */
    = 502 | BF_FLAG_NEED_DOCUMENT,
    /**
       This command returns edited text back to the player. This command replaces the text contained in the
       last activated element which contained editable text.
        <br>
        Parameter is a pointer to a BFCompleteTextEdit structure.
       <br><br>
    */
    BF_COMMAND_COMPLETE_TEXT_EDIT     /* BFCompleteTextEdit */
    = 503 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,


    /*********** Player commands ***********/
    /**
       Sets Player preferences.

       The fields bSetFont, pFontData and uFontDataSize allow to assign a new default font to
       the player. This only happens when bSetFont is set to BF_TRUE. All text that does not
       specifically use SVG fonts will then be rendered using this font.
       The provided font must conform to the BFSF format.
       BF_NO_ERROR is returned if the font was read correctly, BF_PLAYER_PARSE_ERROR otherwise.
       The player does not make a copy of the provided data but points directly to it for
       performances reason.
       It is possible to restore the original internal player font by calling this command with
       a null-pointer data buffer. The internal font is also restored when the operation fails
       with BF_PLAYER_PARSE_ERROR.

       Parameter is a pointer to a BFPreferences structure.
       <br><br>
    */
    BF_COMMAND_PREFERENCES            /* BFPreferences                        */
    = 600 | BF_FLAG_NEED_PDATA,


    /********** DOM API commands ***********/
    /**
       Retrieves the w3c DOM::SVGDocument class (Defined in BFSVGDOM.H).
       Parameter is a pointer to a (BFDOM_SVGDocument*) pointer which returns a reference to
       the document.
       This always retrieves the root primary document.
       Can return 0 when no document exists or out of memory.
       <br><br>
    */
    BF_COMMAND_DOM_GET_DOCUMENT       /* BFDOM_SVGDocument*                   */
    = 700 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
       Sets the callback to be called when a XML event is triggered. From this callback,
       the handler element can be used via the uDOM to handle the event in any way -
       either using the uDOM through a specific application, calling JavaScript, Java, etc.
       Parameter is a pointer to a BFSetXMLEventCallback structure.
       <br><br>
    */
    BF_COMMAND_DOM_SET_XMLEVENT_CB  /* BFSetXMLEventCallback                  */
    = 701 | BF_FLAG_NEED_PDATA,
    /**
       This command will return the focus information as of the last redraw. It should normally be issued after a
       completed redraw. One should not keep the uDOM pointer around since the node could be deleted,
       either by uDOM calls or the discard element. The bounding box returned is not clipped and could potentially
       be outside of the screen, either -ve values or larger values.
       Parameter is a pointer to a BFCurrentFocus structure.
       <br><br>
    */
    BF_COMMAND_DOM_GET_FOCUS        /* BFCurrentFocus                         */
    = 702 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,

    /******* Foreign object commands ********/
    /**
       Allows the DOM to hand over foreign information to the user agent so
       that it gets parsed, executed and destroyed.
       Parameters is a pointer to a BFForeignObject structure.
       <br><br>
    */
    BF_COMMAND_FOREIGN_OBJECT         /* BFForeignObject                      */
    = 800 | BF_FLAG_NEED_PDATA,

    /******* Sound object commands ********/
    /**
       Registers a set of callback to allows the DOM to hand over audio information
       to the user agent so that it gets parsed, executed, played and destroyed.
       Parameters is a pointer to a BFAudioObject structure.
       <br><br>
    */
    BF_COMMAND_AUDIO_SETUP         /* BFAudioObject                           */
    = 801 | BF_FLAG_NEED_PDATA,
    /**
       Called by the application to set the audio duration. This command is only necessary
       if the duration could not be set during the video initialization (i.e. via
       (*pfnPlay)() member of BFAudioObject). If not called, the duration is indefinate
       unless specified in the content

       Parameters is a pointer to a BFAudioSetDuration structure.
       <br><br>
    */
    BF_COMMAND_AUDIO_SET_DURATION  /* BFAudioSetDuration                      */

    = 802 | BF_FLAG_NEED_PDATA,

    /******* Video object commands ********/
    /**
       Registers a set of callbacks to allow the DOM to hand over video information
       to the user agent so that it gets parsed, executed, played and destroyed.
       Parameters is a pointer to a BFVideoObject structure.

       There are two ways to use the video playback API.

       The first way to is to use the native video decoder to feed the video frames
       to the SVG library.  The frames are passed back to the library when the SVG
       library calls the pfnGetFrame() callback function.  The SVG library will be
       responsible for drawing the video, along with other SVG graphics.  The advantage
       of this method is that the SVG library can easily apply transformation to the
       video.  However, the video playback may experience some performance degradation,
       especially on low-powered devices.  One possible way to increase performance is
       to set the “transformBehaviour” attribute of the video element to “geometric.”
       This will ensure that no transformation is applied to the video, which may
       increase performance but limit functionality.

       The second way is to allow the native video decoder to draw the video directly.
       The SVG graphics must be “blitted” to the overlay surface of the video.  This
       method is less flexible than the first, since the SVG library cannot apply
       transformation to the video.  However, applications may gain some performance
       benefits.  When using this method, it is important to perform the following
       steps during setup, to avoid unnecessary redraws and to  maximize performance:

       1. Do not set BFVIDEO_HAS_COMPOSE_VIDEO flag during video setup.
       2. On the application side, set pfnGetFrame() to return 0 immediately
       <br><br>
    */
    BF_COMMAND_VIDEO_SETUP         /* BFVideoObject                           */
    = 803 | BF_FLAG_NEED_PDATA,
    /**
       Called by the application to set the video duration. This command is only necessary
       if the duration could not be set during the video initialization (i.e. via
       (*pfnPlay)() member of BFVideoObject). If not called, the duration is indefinite
       unless already specified during the video initialization or specified in the content.
       Parameters is a pointer to a BFVideoSetDuration structure.

       <br><br>
    */
    BF_COMMAND_VIDEO_SET_DURATION   /* BFVideoSetDuration                     */
    = 804 | BF_FLAG_NEED_PDATA,
    /**
       Called by the application to set the video dimensions. This command is only necessary
       if the dimensions could not be set during the video initialization (i.e. via
       (*pfnPlay)() member of BFVideoObject). If not called, the dimensions are indefinite
       unless already specified during the video initialization or specified in the content.
       Parameters is a pointer to a BFVideoSetDimensions structure.

       <br><br>
    */
    BF_COMMAND_VIDEO_SET_DIMENSIONS /* BFVideoSetDimensions                   */
    = 805 | BF_FLAG_NEED_PDATA,
    /**
       Registers a set of callback to allows the DOM to hand over scripting information
       to the user agent.
       Parameters is a pointer to a BFScriptObject structure.
       <br><br>
    */
    BF_COMMAND_ECMASCRIPT_SETUP       /* BFScriptObject                       */
    = 806 | BF_FLAG_NEED_PDATA,

    /******* External Font Engine interface ********/
    /**
       Registers a third-party external font engine to support various fonts defined
       at the system level. Upon completion, this command returns uFontEngineID
       a unique identifier associated to the newly registered font engine. User
       agents can register several font engines with the SVG player. A typical
       usecase is to register a TrueType font renderer like FreeType.

       Parameter is a pointer to a BFFontEngineObject structure.
       <br><br>
    */
    BF_COMMAND_FONT_ENGINE_SETUP   /* BFFontEngineObject                      */
    = 901 | BF_FLAG_NEED_PDATA,

    /**
       Sets the name of the default font to be used as initial value for the
       font-family attribute.
       Parameter is a pointer to a BF_CHAR, the 0-terminated string of the font name.
       <br><br>
    */
    BF_COMMAND_FONT_SET_DEFAULT_FAMILY  /* BF_CHAR                            */
    = 903 | BF_FLAG_NEED_PDATA,

    /******* External image decoder interface ********/
    /**
       Registers a third-party external image decoder for particular image type. User
       agents can register multiple image decoders with the SVG player, but only one
       decoder per image type.  The SVG player only support JPG and PNG image types.
       Parameter is a pointer to a BFImageDecoder structure.

       <br><br>
    */
    BF_COMMAND_IMAGE_DECODER_REGISTER   /* BFImageDecoderRegister                 */
    = 1001 | BF_FLAG_NEED_PDATA,




#ifndef BF_CFG_REMOVE_RENDERCACHE
    /******* CACHE commands ********/
    /**
        Called to retreive current caching matrix values.

        Parameter is a pointer to a BFCacheStatistics structure

    */
    BF_COMMAND_CACHE_GET_STATISTICS /* BFCacheStatistics                       */
    = 4002 | BF_FLAG_NEED_PDATA | BF_FLAG_NEED_DOCUMENT,
    /**
        Controls caching. There are two forms - explicit and implicit.
        Explicit caching is done when a container has the static='true' property.
        The container contents are cached into an off-screen buffer for later rendering. If
        animations or changes to the DOM occur via scripting, the cache is flushed and the contents
        are re-cached.
        Implicit caching looks at svg ( top-level ) and group
        elements and attempts to cache the container if the following criteria is met -
        the container does not contain any descendants which: are participating in active
        animations or are being used in a shadow tree by a use element.

        Parameter is a pointer to a BFCacheControl structure

    */
    BF_COMMAND_CACHE_CONTROL        /* BFCacheControl                          */
    = 4003 | BF_FLAG_NEED_PDATA,
#endif

#ifndef BF_DOXYGEN_SKIP
    BF_COMMAND_LAST                 = 20000
#endif /* BF_DOXYGEN_SKIP */
};


/*****************************************************************************
 * (1) Declarations of the structures used as parameters of the commands
 *     or requests.
 *****************************************************************************/

/** Types of documents supported by the player. */
typedef enum
{
    BF_NO_MIMETYPE,                         /**< MIME obtained from the file name */
    BF_SVG_MOBILE                           /**< SVG document */
}
BFMimeType;



/* Defines input to load/parse and gets properties of document after building */
#define BF_ERROR_CONTEXT_MSG_SIZE 128		/** The maximum size of the error message return buffer */
#define BF_TITLE_SIZE 128					/** The maximum size of the document title. */


/** Parameters of #BF_COMMAND_DOCUMENT_NEW. */
typedef struct
{
    const BF_CHAR     * szFileName;         /**< In:  Filename (Absolute path or URL) */
    const BF_UINT8    * pBuffer;            /**< In:  Document text description */
    BF_UINT32           uBufferSize;        /**< In:  Size of the document input, in bytes */
    BFMimeType          uMimeType;          /**< In:  MIME type of the docuemnt to parse/decode */
    void              * pViewData;          /**< In:  svgView data - data returned by a BF_COMMAND_POINTER_CLICK or BF_COMMAND_FOCUS_ACTIVATE command */
    BF_CHAR             szErrorContext[BF_ERROR_CONTEXT_MSG_SIZE];   /**< Out: Error/status context message. Must be first in the Out partion.
																	 If returned status from BFprocessCommand different than BF_NO_ERROR,
																	 szErrorContext will contain the error context
																	*/
    BF_UINT32           uParseInputLine;    /**< Out: Line number of parse context */
    BF_CHAR             szTitle[BF_TITLE_SIZE];   /**< Out: Document title from the SVG content. If the top-level SVG element has a title element as a child, the title string is returned here. */
    BF_BOOL             bZoomPanAllowed;      /**< Out: SVG document can receive zoom and pan events */
    BF_BOOL             bTextOnlyAllowed;     /**< Out: SVG document supports text only mode  */
    BF_BOOL             bClosingSVGTagFound;  /**< Out: set to BF_TRUE when a call to STREAM_FETCH reaches the end of the top-level SVG tag */
    BF_UINT32           uDocumentEndOffset;   /**< Out: Returns offset (from start of current parse buffer) of byte following the closing tag of the top-level SVG document. Set when bClosingSVGTagFound is true */
    BF_BOOL             bAnimationElementsFound;   /**< Out: set to BF_TRUE when the file contains animation elements. To be used with caution since the SVG file can change with scripting/discard/etc */
    BF_BOOL             bUserEventAnimationTriggersFound;   /**< Out: set to BF_TRUE when the file contains animation which is triggered from user events - focus, mouse, key events. To be used with caution since the SVG file can change with scripting/discard/etc */
}
BFNewDocument;


/** Parameters of #BF_COMMAND_DOCUMENT_VERIFY. */
typedef struct
{
    BF_BOOL             bUnknownOpcode;     /**< Out: BF_TRUE if data contained unknown elements or attributes */
    BF_BOOL             bMissingRequiredAttribute; /**< Out: BF_TRUE if a required attribute is missing from an element */
    BF_BOOL             bCircularRef; /**< Out: BF_TRUE if a circular reference is present */
    BF_BOOL             bLocalImageRef; /**< Out: BF_TRUE if the document contains a local image reference */
    BF_BOOL             bDuplicateID; /**< Out: BF_TRUE if the document contains a duplicate ID */
    BF_BOOL             bErrorInAnimation; /**< Out: BF_TRUE if the document contains an error in declarative animation */
    BF_BOOL             bUnsupportedValue; /**< Out: BF_TRUE if the document contains an element with an unsupported value */
}
BFDocumentVerify;

/** Included in #BFRequestDone. */
typedef enum
{
    BF_REQUEST_DATA_FILE,				/** indicates that the request is a data file */
    BF_REQUEST_DATA_BITMAP				/** indicates that the request is a bitmap file */
} BFRequestDoneType;

/** Included in #BFFileOrBitmapData. */
typedef struct
{
    const BF_UINT8     *pBuffer;        /**< In: Pointer to the file buffer */
    BF_UINT32           uBufferSize;    /**< In: Size of the the file buffer */
} BFFileData;

/** Included in #BFFileOrBitmapData. */
typedef struct
{
    const BF_UINT32    *pBuffer;  /** < In: The bitmap buffer data. Pixels are in 32bits ARGB format */
    BF_UINT16           uWidth;   /** < In: Bitmap width in pixels */
    BF_UINT16           uHeight;  /** < In: Bitmap height in pixels */
} BFBitmapData;


/** Included in #BFRequestDone. */
typedef union
{
    BFFileData    file;		      /** < In: The details for file data */
    BFBitmapData  bitmap;		  /** < In: The details for bitmap data */
} BFFileOrBitmapData;


/** Parameters of #BF_COMMAND_DOCUMENT_REQUEST. */
typedef struct
{
    const BF_CHAR      *szFileName;    /**< In:  Identifier of the request (as returned by BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST) */
    BFRequestDoneType   type;          /**< In:  type of the returned data */
    BFFileOrBitmapData  data;          /**< In:  the returned data */
}
BFRequestDone;


/** Parameters of #BF_COMMAND_DOCUMENT_GET_NEXT_REQUEST. */
typedef struct
{
    BF_BOOL            bRequestsToService; /**< Out:  True if there are requests to be serviced. If BF_FALSE,
                                                      szFileName is invalid */
    const BF_CHAR      *szFileName;        /**< Out:  Filename - does not need to be freed */
    BF_BOOL            bRequired;          /**< Out: is BF_TRUE of this resource is used by an element which has
                                                     the externalResourcesRequired attribute set to BF_TRUE. Used as a
                                                     hint to implementations. If progressive rendering is being used,
                                                     the element will be 'closed' only once the external resource is satisfied */
}
BFNextRequest;


/** Parameters of #BF_COMMAND_DOCUMENT_SET_NEW_REQUEST_CB. */
typedef struct
{
    BFNewRequestCallback  pfnCB;			/**< In:  pointer to callback */
    BF_INT_VOID           pUserData;		/**< In:  pointer to user data which will be passed to the callback */
}
BFSetNewRequestCallback;


/** Parameters of #BF_COMMAND_DOCUMENT_SET_NEW_IMAGE_CB. */
typedef struct
{
    BFNewImageCallback  pfnCB;				/**< In:  pointer to callback */
    BF_INT_VOID         pUserData;			/**< In:  pointer to user data which will be passed to the callback */
}
BFSetNewImageCallback;



/** Parameters of #BF_COMMAND_DOCUMENT_SET_ANIMATION_NOTIFY_CB. */
typedef struct
{
    BFAnimationNotifyCallback  pfnCB;		/**< In:  pointer to callback */
    BF_INT_VOID                pUserData;	/**< In:  pointer to user data which will be passed to the callback */
}
BFSetAnimationNotifyCallback;


/** Parameters of #BF_COMMAND_DOCUMENT_SET_GOTO_LOCATION_CB. */
typedef struct
{
    BFGotoLocationCallback     pfnCB;		/**< In:  pointer to callback */
    BF_INT_VOID                pUserData;	/**< In:  pointer to user data which will be passed to the callback */
}
BFSetGotoLocationCallback;


/** Parameters of #BF_COMMAND_DOCUMENT_SET_GETURL_CB. */
typedef struct
{
    BFGetURLCallback           pfnCB;		/**< In:  pointer to callback */
    BF_INT_VOID                pUserData;	/**< In:  pointer to user data which will be passed to the callback */
}
BFSetGetURLCallback;


/** Parameters of #BF_COMMAND_DOCUMENT_SET_POSTURL_CB. */
typedef struct
{
    BFPostURLCallback          pfnCB;		/**< In:  pointer to callback */
    BF_INT_VOID                pUserData;	/**< In:  pointer to user data which will be passed to the callback */
}
BFSetPostURLCallback;


/** Parameters of #BF_COMMAND_DOCUMENT_HTTP_REQUEST_COMPLETE. */
typedef struct
{
    BF_UINT32                  uRequestHandle;  /**< In: handle to http transaction provided in BFPostURLCallback or BFGetURLCallback */
    BF_WCHAR                   *pContentType;   /**< In: pointer to content-type string */
    BF_WCHAR                   *pContent;       /**< In: pointer to content */
    BF_BOOL                    bSuccess;        /**< In: status */
}
BFHTTPRequestComplete;



/** Parameters of #BF_COMMAND_DOCUMENT_SET_LOGGING_CB. */
typedef struct
{
    BFLoggingCallback           pfnCB;			/**< In:  pointer to callback */
    BF_INT_VOID                 pUserData;		/**< In:  pointer to user data which will be passed to the callback */
    BFLoggingType               type;			/**< In:  a bitmask indicating the components to be logged  */
}
BFSetLoggingCallback;

/** Parameters of #BF_COMMAND_SERVICE_SETUP_CB. */
typedef struct
{
    BFServiceSetupCallback      pfnCB;			/**< In:  pointer to callback */
    BF_INT_VOID                 pUserData;		/**< In:  pointer to user data which will be passed to the callback */
}
BFSetServiceSetupCallback;


/** Parameters of #BF_COMMAND_SET_GET_SCRIPT_RESOURCE_CB. */
typedef struct
{
    BFGetScriptResource         pfnCB;			/**< In:  pointer to callback */
    BF_INT_VOID                 pUserData;		/**< In:  pointer to user data which will be passed to the callback */
}
BFSetGetScriptResourceCallback;


/** Parameters of #BF_COMMAND_VIEW_BUFFER. */
typedef struct
{
    void            *pBuffer;                   /**< In: pointer to an ARGB 32-bit buffer */
    BF_UINT16        uWidth;                    /**< In: width of the frame buffer */
    BF_UINT16        uHeight;                   /**< In: height of the frame buffer */
    BF_INT16         iLinePadding;              /**< In: the delta between data stride and width */
    BFClearCallback  pfnClearCB;                /**< In: called each time the buffer is cleared. */
    BF_INT_VOID      pClearUserData;            /**< In: User data for clear callback */
    BF_UINT16        uDpi;                      /**< In: uDpi is the buffer resolution in dots per inch */
}
BFFrameBuffer;



/** Parameters of #BF_COMMAND_VIEW_SET_BACKGROUND_COLOR and #BF_COMMAND_VIEW_SET_OVERFLOW_COLOR. */
typedef struct
{
    BF_UINT8    a;                         /**< the alpha channel - between 0 and 255 */
    BF_UINT8    r;                         /**< the red channel - between 0 and 255 */
    BF_UINT8    g;                         /**< the green channel - between 0 and 255 */
    BF_UINT8    b;                         /**< the blue channel - between 0 and 255 */
}
BFBackgroundColor;

/** Generic rectangle region */
typedef struct
{
    BF_INT32            x1;                 /**< In:  Upper-left abscissae, in screen coordinates */
    BF_INT32            y1;                 /**< In:  Upper-left ordinate, in screen coordinates */
    BF_INT32            x2;                 /**< In:  Lower-right abscissae, in screen coordinates */
    BF_INT32            y2;                 /**< In:  Lower-right ordinate, in screen coordinates */
}
BFPlayerRegion;


/** Parameters of #BF_COMMAND_DOCUMENT_CONTINUE */
typedef struct
{
    BF_BOOL            bAbort;              /**< In:  Set to BF_TRUE if the user agent wants to abort the current command asap */
}
BFContinue;


/** Paramaters of #BF_COMMAND_DOCUMENT_ADVANCE_TIME */
typedef struct
{
    BF_UINT32       uElapsedTime;           /**< In: Elapsed time ( ms ) since this command was last sent */
}
BFPlayerAdvanceTime;


/** Parameters of #BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS */
typedef struct
{
    BF_UINT32 uNextSignificantTime;         /**< Out: the next significant time - see the description for #BF_COMMAND_DOCUMENT_GET_REDRAW_STATUS */
}
BFDocumentRedrawStatus;




/** Parameters of #BF_COMMAND_VIEW_GET_VIEWPORT */
typedef struct
{

    BF_INT32 width;     /**< Out: computed width, in pixels, based on the current client region (percentages applied to that region) */
    BF_INT32 height;    /**< Out: computed height, in pixels, based on the current client region (percentages applied to that region) */

    BF_INT32 fpIntrinsicWidth;      /**< Out: intrinsic width as sepcified by the attributes width and height on the top-level SVG element. 16.16 fixed point values */
    BF_INT32 fpIntrinsicHeight;     /**< Out: intrinsic height as sepcified by the attributes width and height on the top-level SVG element. 16.16 fixed point values */

    BFUnitsType intrinsicWidthUnits;        /**< Out: units applied to the intrinsic width. */
    BFUnitsType intrinsicHeightUnits;       /**< Out: units applied to the intrinsic height. */

} BFPlayerExtent;

/** Included in BFSetInterruptCallback*/
typedef BFInterruptAction (*BFInterruptCallback)( BF_INT_VOID pUserData );

/** Parameters of #BF_COMMAND_DOCUMENT_SET_INTERRUPT_CB */
typedef struct
{
    BFInterruptCallback pfnCB;				/**< In:  pointer to callback */
    BF_INT_VOID         pUserData;			/**< In:  pointer to user data which will be passed to the callback */

} BFSetInterruptCallback;

/** Parameters of #BF_COMMAND_DOCUMENT_SET_COMPLETENESS_CB */
typedef struct
{
    BFCompletenessCallback pfnCB;		/**< In:  pointer to callback */
    BF_INT_VOID            pUserData;	/**< In:  pointer to user data which will be passed to the callback */

} BFSetCompletenessCallback;



/** Included in #BFSetXMLEventCallback.
 * A callback function for handling XML Events */
typedef void (*BFHandleXMLEventCallback)( BF_INT_VOID pUserData, BFDOM_Document *pDocument, BFDOM_Event *pEvent, BFDOM_Element *pHandler );

/** Parameters of #BF_COMMAND_DOM_SET_XMLEVENT_CB */
typedef struct
{
    BFHandleXMLEventCallback pfnCB;			/**< In:  pointer to callback */
    BF_INT_VOID              pUserData;		/**< In:  pointer to user data which will be passed to the callback */

} BFSetXMLEventCallback;


/** Parameters of #BF_COMMAND_DOM_GET_FOCUS */
typedef struct
{
    BFDOM_Node              *pNode;         /**< Out: Will be 0 if there is no focused element */
    BFPlayerRegion          boundingBox;    /**< Out: Bounding box in Screen coordinates */
    BF_BOOL                 bDrawFocus;     /**< Out: True if focusHighlight attribute of element evaluates to 'auto' */

} BFCurrentFocus;


/** Parameters of #BF_COMMAND_AUDIO_SET_DURATION. */
typedef struct
{
    void*                    pAudioElement; /**< In: The audio element which to set the duration, initially specified by the
                                                     SVG library during a call to pfnPlay() */
    BF_UINT32                uDuration;     /**< In: The new duration, in milliseconds */

} BFAudioSetDuration;


/** Parameters of #BF_COMMAND_VIDEO_SET_DURATION */
typedef struct
{
    void*                    pVideoElement; /**< In: The video element which to set the duration, initially specified by the
                                                      SVG library during a call to pfnPlay(). */
    BF_UINT32                uDuration;     /**< In: The new duration, in milliseconds */

} BFVideoSetDuration;


/** Parameters of #BF_COMMAND_VIDEO_SET_DIMENSIONS */
typedef struct
{
    void*                    pVideoElement; /**< In: The video element which to set the dimensions, initially specified by the
                                                     SVG library during a call to pfnPlay(). */
    BF_UINT32                uWidth;        /**< In: The new  width */
    BF_UINT32                uHeight;       /**< In: The new height */

} BFVideoSetDimensions;


/** Parameters of #BF_COMMAND_POINTER_CLICK.
 *  If szFileName != 0, a link has been selected
 *  If pEditableText != 0, an editable text or textArea element has been activated */
typedef struct
{
    BF_INT16            x;                  /**< In:  Pointer position, in screen coordinates */
    BF_INT16            y;                  /**< In:  Pointer position, in screen coordinates */
    BF_UINT8            button;             /**< In:  Optional button number - per XML Events spec */
    BF_CHAR           * szFileName;         /**< Out: Requested file. This memory must be freed by a call
                                                    to #BFfreeCommand. */
    BF_CHAR           * szTargetName;       /**< Out: target name as per SVG 1.1 spec, a null values indicates default _replace */
    void              * pViewData;          /**< Out: svgView data - to be freed by BFfreeCommand */
    BF_WCHAR          * pEditableText;      /**< Out: pointer to text that requires editing - must be freed by #BFfreeCommand */
    /**<      update edited text by calling BF_COMMAND_COMPLETE_TEXT_EDIT */
}
BFPointerClickEvent;


#define BF_FLAG_ZOOM_USE_CENTER                 -1		/**< indicates that the zoom event should be zoomed into the center of the viewport */
#define BF_ZOOM_UNIT                    (0x00010000L)   /**< represents 1.0 in 16.16 fixed point format */

/** Parameters of #BF_COMMAND_VIEW_ZOOM. */
typedef struct
{
    BF_UINT32           fpFactor;           /**< In:  Zoom factor expressed in 16.16 fixed point */
    BF_INT16            iZoomX;             /**< In:  Zoom position, in screen coordinates.
                                                      can be the define above to indicate translation behaviour */
    BF_INT16            iZoomY;             /**< In:  Zoom position, in screen coordinates
                                                      can be the define above to indicate translation behaviour */
    BF_BOOL             bCenterLocation;    /**< In:  Center given location in view after zoom */
}
BFZoomEvent;


/** Parameters of #BF_COMMAND_VIEW_PAN. */
typedef struct
{
    BF_INT16            dx;                 /**< In:  Desired horizontal pan. Out: Actual horizontal pan */
    BF_INT16            dy;                 /**< In:  Desired vertical pan.   Out: Actual vertical pan */
    BF_BOOL             bFastScroll;        /**< In:  Apply the fast scroll method (just redraw strips) */
    BFPlayerRegion      clippingRegion;     /**< Out: If using FastScroll, contains the clipping region to use in a redraw */
}
BFPanEvent;


/** Parameters of #BF_COMMAND_VIEW_ROTATE. */
typedef struct
{
    BF_INT32            cx;                 /**< In:  X center of rotation */
    BF_INT32            cy;                 /**< In:  Y center of rotation */
    BF_INT32            angle;              /**< In:  angle in degrees */
    BF_BOOL             bCenterLocation;    /**< In:  if BF_TRUE, cx/cy are ignored and the center of the viewPort is used instead */
}
BFRotateData;


/** Parameters of #BF_COMMAND_KEY_DOWN. */
/** Parameters of #BF_COMMAND_KEY_UP. */
typedef struct
{
    BFKeyIdentifier      eKeyIdentifier;     /**< In:  key identifier pressed */
    BF_UINT8             eKeyModifiers;      /**< In:  Reserved for future use */
    BF_BOOL              bHandled;           /**< Out: has the event been handled by content */

}
BFKeyEvent;

/** Parameters of #BF_COMMAND_TEXT_INPUT. */
typedef struct
{
    BFKeyCharacters      keyCharacters;       /**< In:  key character value pressed */
    BF_BOOL              bHandled;           /**< Out: has the event been handled by content */
}
BFTextEvent;

/** Parameters of #BF_COMMAND_PREFERENCES. */
typedef struct
{
    const BF_CHAR     * szLanguage;         /**< In:  Reference an RFC3066 language identifier string */
    BF_BOOL             bUserAlerts;        /**< In:  Player User Alert messages
                                                 NOTE: This parameter may have been disabled at compile time. */
    BF_BOOL             bManualImgDecoding; /**< In: BF_TRUE to manually request image decoding when document finishes building */
    BF_BOOL             bSetFont;           /**< In: set to BF_TRUE if the font data info has to be used */
    const BF_UINT8     *pFontData;          /**< In: pointer to a buffer containing a BFSF font */
    BF_UINT32           uFontDataSize;      /**< In: size of this data in bytes */
    BF_BOOL             bNullFocus;         /**< In: BF_TRUE to allow player to set null focus. Only stand-alone UAs require a valid focus at all times */
    BF_BOOL             bSupportJSR226;    /**< In: BF_TRUE to support JSR226 */
}
BFPreferences;



/** Parameters of #BF_COMMAND_POINTER_DOWN, #BF_COMMAND_POINTER_MOVE,
    #BF_COMMAND_POINTER_UP. */
typedef struct
{
    BF_INT16            x;                  /**< In:  Pointer position, in screen coordinates */
    BF_INT16            y;                  /**< In:  Pointer position, in screen coordinates */
    BF_UINT8            button;             /**< In:  Optional button number - per XML Events spec */
}
BFPointerEvent;

/** Param for command #BF_COMMAND_POINTER_PICK */
typedef struct
{
    BF_INT16            x;                  /**< In:  Pointer position, in screen coordinates */
    BF_INT16            y;                  /**< In:  Pointer position, in screen coordinates */
    BFDOM_SVGElement    **ppPickedElements; /**> Out: a pointer to a list of BFDOM_SVGElement pointers
                                                      the list cotains any element directly affected by the
                                                      pick, which includes any shadow tree parents like use elements */
    BF_UINT32           uNumPickedElements; /**> Out: the number of elements in the list pointed to by ppPickedElements */
}
BFPointerPickEvent;



/** Param for command #BF_COMMAND_SEND_CUSTOM_EVENT */
typedef struct
{
    BF_WCHAR const *szName;    /**< In: custom event name */
    BF_WCHAR const *szNS;      /**< In: custom event namespace */

} BFCustomEvent;





#define BF_GENERIC_FAMILY_SERIF       0 /** list of generic font family IDs
										<BR>See <A HREF="http://www.w3.org/TR/REC-CSS2/fonts.html#generic-font-families">W3C CSS2</A> */
#define BF_GENERIC_FAMILY_SANS_SERIF  1 /** list of generic font family IDs
										<BR>See <A HREF="http://www.w3.org/TR/REC-CSS2/fonts.html#generic-font-families">W3C CSS2</A> */
#define BF_GENERIC_FAMILY_CURSIVE     2 /** list of generic font family IDs
										<BR>See <A HREF="http://www.w3.org/TR/REC-CSS2/fonts.html#generic-font-families">W3C CSS2</A> */
#define BF_GENERIC_FAMILY_FANTASY     3 /** list of generic font family IDs
										<BR>See <A HREF="http://www.w3.org/TR/REC-CSS2/fonts.html#generic-font-families">W3C CSS2</A> */
#define BF_GENERIC_FAMILY_MONOSPACE   4 /** list of generic font family IDs
										<BR>See <A HREF="http://www.w3.org/TR/REC-CSS2/fonts.html#generic-font-families">W3C CSS2</A> */


/** Parameters of #BF_COMMAND_VIEW_GET / #BF_COMMAND_VIEW_SET. */
typedef struct
{
    BF_INT32 viewParams[9];             /**< internal representation of transformation matrix. Used for 'saving' current user transform but should but be modified */
}
BFViewParams;


/** Parameters of #BF_COMMAND_COMPLETE_TEXT_EDIT */
typedef struct
{
    BF_WCHAR        * pNewText;           /**< In: a pointer to text which is to be set into the editable text or textArea element */
}
BFCompleteTextEdit;


/** Parameters of #BF_COMMAND_FOCUS_ACTIVATE.
 *  If szFileName != 0, a link has been selected
 *  If pEditableText != 0, an editable text or textArea element has been activated
 */
typedef struct
{
    BF_CHAR           * szFileName;         /**< Out: Requested file. This memory must be freed by a call
                                                    to BFfreeCommand. */
    BF_CHAR           * szTargetName;       /**< Out: target name as per SVG 1.1 spec, a null values indicates default _replace */
    void              * pViewData;          /**< Out: svgView data - to be freed by BFfreeCommand */
    BF_WCHAR          * pEditableText;      /**< Out: pointer to text that requires editing - must be freed by BFfreeCommand */
    /**<      update edited text by calling BF_COMMAND_COMPLETE_TEXT_EDIT */
}
BFActivateEvent;


/** Parameters of #BF_COMMAND_FOCUS_MOVE
 */
typedef struct
{
    BFFocusCommand      direction;          /**< In: The direction to change the focus */
    BF_BOOL             bFocusMoved;        /**< Out: BF_TRUE if a new element was found in the given direction and
                                                      focus was changed */
}
BFMoveFocusEvent;


#ifndef BF_CFG_REMOVE_RENDERCACHE
/** Parameters of #BF_COMMAND_CACHE_GET_STATISTICS
*/
typedef struct
{
    BF_UINT32   uNumCachedElements;     /**< Out: The current total number of elements which are being rendered by a cache. */
    BF_UINT32   uNumCachesCreated;      /**< Out: The overall number of caches created. This value should not change very often, only when a cache is flushed or new cache is created. */
    BF_UINT32   uNumCachesRendered;     /**< Out: The overall number of caches rendered. This value should increase each frame, if caches have been created. */
}
BFCacheStatistics;

/** Parameters of #BF_COMMAND_CACHE_CONTROL
*/
typedef struct
{
    BF_BOOL bDisable;	        /**< In: whether caching corresponding to 'static' property (=true or auto) is disabled. Note only
									 caching for 'static' property is affected, caching due to 'opacity' always occurs,
									 provided enough memory is physically available. */
    BF_BOOL	bDisableImplicit;   /**< In: whether implicit caching is disabled. According to spec if the 'static' property is not
                                     specified it defaults to auto which corresponds to implicit caching enabled. By setting
                                     bDisableImplicit to true this disables implicit caching, which is then equivalent to the
                                     default behaviour of 'static' being false. Also, if 'static' is explicitly set to auto it
                                     will be treated as false. */
    BF_UINT32 uMaxBytes;        /**< In: if bDisable is false uMaxBytes corresponds to the maximum memory that will be used for
							         caching. A value of 0 indicates no limit is imposed. Note only caching for 'static'
									 property is affected, caching due to 'opacity' always occurs, provided enough memory is
									 physically available). Memory used for caching 'opacity' IS included in the tally of
									 memory currently used for caching */
    BF_UINT32 uMaxCaches;       /**< In: if bDisable is false uMaxCaches corresponds to the maximum number of elements that will be
									 cached at any given time. A value of 0 indicates no limit is imposed. Note only caching
									 for 'static' property is affected, caching due to 'opacity' always occurs, provided enough
									 memory is physically available). Caches used for 'opacity' ARE included in the tally of
									 number of caches currently used. */
}
BFCacheControl;
#endif



#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif  /* BITFLASH_PLAYERTYPES_H_ */

