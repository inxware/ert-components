/*
 * Filename : vgPick.h
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
#ifndef _VGPICK_H
#define _VGPICK_H

#ifdef __cplusplus
extern "C" {
#endif

/** \file vgPick.h <b>Path and Image Picking</b>
 *
 * This OpenVG extension satisfies the SVGT 1.2 requirement for pointer events
 * (http://www.w3.org/TR/SVGMobile12/interact.html#PointerEventsProperty),
 * specifically whether a pointer event has occurred within the extent of a
 * path (stroke and/or fill) or image. For an external OpenVG implementation
 * (i.e. BF_CFG_USE_EXTERNAL_OPENVG is defined in BFConfig.h) if this extension
 * is not present then the SVG Player uses drawables (which in turn
 * by default rely on EGL, see vgDrawableKHR.h) and OpenVG
 * functions to provide the path picking functionality. The default method for
 * path picking is to simply render the path to an off-screen render target and
 * check to see if the pixel in question is set. This is not necessarily the
 * most efficient way to perform picking...
 *
 * If EGL is not used in conjunction with the external
 * OpenVG implementation then either this picking extension and/or the drawable
 * extension MUST be implemented. Also, even if EGL is used this picking extension
 * could be implemented in a more efficient manner, depending on the external
 * OpenVG implementation, than is implemented by default
 * in the SVG Player.
 *
 * To configure the SVG Player to use this extension define
 * BF_CFG_USE_EXTERNAL_OPENVG_PICK_EXT in BFConfig.h and implement vgPickPath and
 * vgPickImage according to the following description as part of the external
 * OpenVG implementation linked with the BFSVGT core.
 *
 * Note also that if the SVG Player is not configured to handle pointer events,
 * if the target platform does not support a pointing device, then this extension
 * could be implemented by simply having the functions return VG_FALSE.
 */

/**
 * vgPickPath indicates whether a point specified in transformed coordinates lies
 * within a path. VG_TRUE is returned if the point is within the path. Note that
 * the stroke and fill paint properties are ignored, the test for inclusion of the
 * point is carried out as if the stroke and fill were painted with a solid colour.
 * \param path          the path to check the point against for inclusion.
 * \param paintModes    specifies whether the path to be checked for inclusion of
 *                      the point is stroked (VG_STROKE_PATH) or filled (VG_FILL_PATH)
 *                      or both (VG_STROKE_PATH|VG_FILL_PATH).
 * \param x             horizontal position of the point to be checked for inclusion,
 *                      in transformed space.
 * \param y             vertical position of the point to be checked for inclusion,
 *                      in transformed space.
 * \return              VG_TRUE if the path is "picked" by the specified point,
 *                      otherwise VG_FALSE
 */
VGboolean vgPickPath(VGPath path, VGbitfield paintModes, VGint x, VGint y);

/**
 * vgPickImage indicates whether a point specified in transformed coordinates lies
 * within an image. VG_TRUE is returned if the point is within the image. Note that
 * the image opacity must be considered. If the specified point lies within
 * the image bounds and the image opacity at the specified point is zero then the
 * image is considered to not be "picked", and VG_FALSE is returned.
 * \param image         the image to check the point against for inclusion.
 * \param x             horizontal position of the point to be checked for inclusion,
 *                      in transformed space.
 * \param y             vertical position of the point to be checked for inclusion,
 *                      in transformed space.
 * \return              VG_TRUE if the image is "picked" by the specified point,
 *                      otherwise VG_FALSE
 */
VGboolean vgPickImage(VGImage image, VGint x, VGint y);

#ifdef __cplusplus
}
#endif

#endif

