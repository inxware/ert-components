/*
 * Filename : vgReadPathData.h
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
#ifndef _VGREADPATHDATA_H
#define _VGREADPATHDATA_H

#ifdef __cplusplus
extern "C" {
#endif

/** \file vgReadPathData.h  Reading Path Data
 *
 * This OpenVG extension satisfies the uDOM requirement of the SVGT 1.2 specification
 * for reading path data (http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__SVGPath)
 * For an external OpenVG implementation (i.e. BF_CFG_USE_EXTERNAL_OPENVG is defined
 * in BFConfig.h)
 * if this extension is not present then the BFSVGT core uses components of the internal
 * OpenVG implementation to satisfy this requirement. This incurs overhead in memory
 * footprint and performance. To configure the BFSVGT core to use this extension define
 * BF_CFG_USE_EXTERNAL_OPENVG_READPATH_EXT in BFConfig.h and implement vgReadPathData
 * according to the following description as part of the external OpenVG implementation
 * linked with the BFSVGT core. This will eliminate the inclusion of components of the
 * internal OpenVG implementation in the BFSVGT core.
 *
 */

/** vgReadPathData reads the segment and coordinate data at segmentIndex of dstPath.
 *
 * The segment command at segmentIndex is copied to pathSegment and the corresponding
 * path data is copied to pathData in the same format as the original coordinate data.
 * pathData must point to a buffer large enough to hold the maximum amount of coordinate
 * data for any single segment. The segment and path data returned are expected to be
 * those which result from the usual path normalization as described in the OpenVG
 * 1.0.1 specification.
 *
 * The pathData pointer must be aligned on a 1-, 2-, or 4-byte boundary
 * depending on the size of the coordinate datatype (as returned by querying the
 * path’s VG_PATH_DATATYPE parameter using vgGetParameteri). The
 * VG_PATH_CAPABILITY_READ capability must be enabled for path.
 *
 * \param dstPath       the path to read data from
 * \param segmentIndex  the index of the segment to read the data from
 * \param pathSegment   pointer to a VGubyte into which the requested path segment command
 *                      (of type VGPathSegment) is copied into.
 * \param pathData      pointer to a block of memory into which the path data corresponding
 *                      to the path segment pathSegment is copied.
 */
void vgReadPathData(VGPath dstPath,  VGint segmentIndex, VGubyte * pathSegment, void * pathData);

#ifdef __cplusplus
}
#endif

#endif

