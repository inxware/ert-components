/*
 * Filename : BFError.h
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
 * \file BFError.h
 *
 * This file defines error codes that the BitFlash SVG Player can return.
 *
 * created January 20, 2002
 */


#ifndef BITFLASH_ERROR_H_
#define BITFLASH_ERROR_H_


#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif


/**
    The command was successul.
*/
#define BF_NO_ERROR                        0x0001


/**
    The command was interrupted.
*/
#define BF_INTERRUPT                       0x0002


/**
    The command was aborted.
*/
#define BF_ABORTED                         0x0003


/** Mask for player error types. */
#define BF_PLAYER_ERROR                    0x0200


/* Define component specific error/status */


/**
 * Returned when illegal XML or SVG syntax is found.
 * Also returned when attempting to open other than SVG or SVGz files
 */
#define BF_PLAYER_PARSE_ERROR                   (BF_PLAYER_ERROR | 1)


/* BF_PLAYER_PARSE_WARNING (deprecated)         (BF_PLAYER_ERROR | 2) */
/* BF_PLAYER_INVALID_SVG_TYPE (deprecated)      (BF_PLAYER_ERROR | 3) */
/* BF_PLAYER_COMMAND_HANDLED (deprecated)       (BF_PLAYER_ERROR | 4) */


/**
 * Returned anytime at least one memory allocation failure happened.
 * If this error happens while opening a document, the document is being
 * detroyed before BFprocessCommand returns. If this error happens
 * while rendering, rendering is given up, leaving the render buffer
 * in a partially rendered state, probably with seemingly erroneous
 * graphical data
 */
#define BF_PLAYER_OUT_OF_MEMORY                 (BF_PLAYER_ERROR | 5)


/**
 * Used only in builds that support scripting
 */
#define BF_PLAYER_SCRIPT_ERROR                  (BF_PLAYER_ERROR | 6)


/* BF_PLAYER_INVALID_LAYER (deprectaed)         (BF_PLAYER_ERROR | 7) */


/**
 * Returned when a mathematical overflow happens while rendering.
 * This leaves the render buffer is a partially rendered state,
 * probably with seemingly erroneous graphical data
 */
#define BF_PLAYER_NUMERIC_OVERFLOW              (BF_PLAYER_ERROR | 8)


/**
 * Returned when a command is called while the player is in a special
 * processing mode where only certain commands can be accepted :
 * - Once a document began loading with the streaming parser,
 *   only BF_COMMAND_DOCUMENT_STREAM_FETCH or BF_COMMAND_DOCUMENT_STREAM_DONE
 *   can be called
 * - When a command has been interrupted (BF_INTERRUPT returned), only
 *   BF_COMMAND_DOCUMENT_CONTINUE can be called
 * - BF_COMMAND_VIEW_BUFFER can only be called on a player that has
 *   been created with the user buffer option
 */
#define BF_PLAYER_ILLEGAL_COMMAND               (BF_PLAYER_ERROR | 9)


/**
 * Returned when a command expects a parameter and none is provided
 * (pData == 0)
 */
#define BF_PLAYER_NEED_PARAM                    (BF_PLAYER_ERROR | 10)


/**
 * Returned when a command expects a document to be present in the player
 * and none is present
 */
#define BF_PLAYER_NEED_DOCUMENT                 (BF_PLAYER_ERROR | 11)


/**
 * Returned when a command is called with pPlayer == 0
 */
#define BF_PLAYER_NEED_PLAYER                   (BF_PLAYER_ERROR | 12)


/**
 * Returned when internal or native image decoders didn't manage to
 * decode a PNG or JPG image
 */
#define BF_PLAYER_IMAGE_DECODER_ERROR           (BF_PLAYER_ERROR | 13)


/**
 * Returned by BF_COMMAND_VIEW_REDRAW or BF_COMMAND_VIEW_ZOOM
 * when a zoom limit has been reached (max or min), provoking the
 * previous zoom level to be reapplied
 */
#define BF_PLAYER_ZOOM_LIMIT_REACHED            (BF_PLAYER_ERROR | 14)


/**
 * Returned when invalid values are detected in the command's
 * params structure.
 */
#define BF_PLAYER_INVALID_PARAMS                (BF_PLAYER_ERROR | 15)



#ifdef __cplusplus
BF_END_EXTERN_C
#endif


#endif  /* BITFLASH_ERROR_H_ */

