/*
 * Filename : vgDrawableKHR.h
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
#ifndef _VGDRAWABLEKHR_H
#define _VGDRAWABLEKHR_H

#ifdef __cplusplus
extern "C" {
#endif

/** \file vgDrawableKHR.h
 * Drawables are here defined as OpenVG images that can be used as render targets for OpenVG functions.
 * Drawables provide "off-screen" or temporary render targets required to provide some functionality
 * in the BFSVGT 1.2 core. Some SVG features that require drawables include group opacity, masking and
 * patterns. The BFSVGT core also supports render caching of aggregate graphical elements using drawables,
 * providing significant performance enhancement in many cases.<br>
 *
 * This extension is necessary since OpenVG does not directly specify a native graphics interface, including
 * support for contexts or rendering targets. EGL is available and suggested in the OpenVG spec to provide
 * the needed functionality but other interfaces can be used instead. For an external OpenVG implementation
 * (i.e. BF_CFG_USE_EXTERNAL_OPENVG is defined in BFConfig.h) if this extension
 * is not present then the BFSVGT core uses EGL and OpenVG functions to provide
 * the drawables functionality. If EGL is not used in conjunction with the external
 * OpenVG implementation then this drawables extension MUST be implemented. To configure the BFSVGT core to
 * use this extension define BF_CFG_USE_EXTERNAL_OPENVG_DRAWABLE_EXT in BFConfig.h and implement the
 * functions listed below as part of the external OpenVG implementation linked with the BFSVGT core.<br>
 *
 * The drawable interface specified here is based on a draft proposal (OVG_KHR_drawable_object.7.txt) of the
 * OpenVG Working Group.
 *
 */
#define VG_DEFAULT_DRAWABLE_KHR ((VGHandle)1)   /**< an implied drawable representing the initial render target,
                                                     when the OpenVG context is created/bound to the native graphics
                                                 */

typedef VGHandle VGDrawableKHR; /**< definition if opaque type for a drawable */

/** an enumeration of the possible types of images that can make up a drawable */
typedef enum
{
    VG_DRAWABLE_ATTACHMENT_COLOR_KHR             = 0x2400,  /**< indicates a color image, to be used as a render target */
    VG_DRAWABLE_ATTACHMENT_MASK_KHR              = 0x2401   /**< indicates an alpha mask image, currently not supported */
} VGDrawableAttachmentKHR;


/**
vgCreateDrawableKHR

vgCreateDrawableKHR creates a drawable object from the
      attachment points and corresponding VGImage objects. An opaque handle to the drawable
      object is returned. See OVG_KHR_drawable_object.7.txt
      for a full specification of this function. Note that the BFSVGT core makes no use
      of alpha masks hence support for the VG_DRAWABLE_ATTACHMENT_MASK_KHR enumerant is not
      strictly required.
      \param count          number of attachment points and associated images
                            to make up the drawable created
      \param attachments    array of count VGDrawableAttachmentKHR enumerants
      \param images         array of count VGImage handles.
      \return               the handle to the created drawable if successful, otherwise
                            VG_INVALID_HANDLE
 */
VGDrawableKHR vgCreateDrawableKHR(VGint count, const VGDrawableAttachmentKHR* attachments, const VGImage* images);

/**
vgDestroyDrawableKHR

vgDestroyDrawableKHR destroys a VGDrawableKHR object
      previously created with a call to the function vgCraeteDrawableKHR.
      In destroying the VGDrawableKHR object the function will release
      the state needed to represent the drawable, but will not
      destroy the VGImage objects that were used to create it. See
      OVG_KHR_drawable_object.7.txt for a full specification of this function.
      \param drawable   the drawable to destroy.
 */
void vgDestroyDrawableKHR(VGDrawableKHR drawable);

/**
vgValidDrawableFormatKHR
      vgValidDrawableFormatKHR takes a combination of VGDrawableAttachmentKHR
      enumerants, and of VGImageFormat enumerants, and returns whether it is
      supported by the current OpenVG context. See
      OVG_KHR_drawable_object.7.txt for a full specification of this function.
      Note that the BFSVGT core does not call this function, its specification
      is included only for completeness. The BFSVGT core expects the
      VG_DRAWABLE_ATTACHMENT_COLOR_KHR attachment to be supported using the
      VG_sARGB_8888 and VG_sARGB_8888_PRE image formats.
      \param count          number of attachment points and associated images
                            to check for format support.
      \param attachments    array of count VGDrawableAttachmentKHR enumerants
      \param formats        array of count VGImageFormat values.
      \return               VG_TRUE if all image formats are supported for
                            the corresponding attachment points.
 */
VGboolean vgValidDrawableFormatKHR(VGint count, const VGDrawableAttachmentKHR* attachments, const VGImageFormat* formats);

/**
vgBindDrawableKHR
      vgBindDrawableKHR replaces the current drawable in
      the current OpenVG context with the specified one, making it the render
      target. The special handle VG_DEFAULT_DRAWABLE_KHR may be used to bind
      the drawable that was initially provided by the native graphics interface. See
      OVG_KHR_drawable_object.7.txt for a full specification of this function.
      \param drawable   the drawable to bind as the render target.
      \return           VG_TRUE if the drawable was successfully bound to the
                        current OpenVG context, otherwise VG_FALSE.
 */
VGboolean vgBindDrawableKHR(VGDrawableKHR drawable);

/**
vgGetBoundDrawableKHR
vgGetBoundDrawableKHR returns a VGDrawableKHR handle to the drawable
      object that is bound to the current OpenVG context. The default drawable
      handle (VG_DEFAULT_DRAWABLE_KHR) is used to represent the drawable
      provided by the native graphics interface. See
      OVG_KHR_drawable_object.7.txt for a full specification of this function.
      \return   handle to the currently bound drawable.
 */
VGDrawableKHR vgGetBoundDrawableKHR(void);

/**
vgGetDrawableAttachmentKHR
      vgGetdrawableAttachmentKHR returns a handle to the VGImage
      object that was attached in the given drawable object when it was created.
      See OVG_KHR_drawable_object.7.txt for a full specification of this function.
      \param drawable   the drawable to retrieve the desired VGImage handle from.
      \param attachment the attachment point specifying which VGImage handle to
                        retrieve.
      \return           the desired VGImage handle.
 */
VGImage vgGetDrawableAttachmentKHR(VGDrawableKHR drawable, VGDrawableAttachmentKHR attachment);


#ifdef __cplusplus
}
#endif

#endif

