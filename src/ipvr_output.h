/*
 * Copyright (c) 2014 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Yao Cheng <yao.cheng@intel.com>
 *
 */

#ifndef _IPVR_OUTPUT_H_
#define _IPVR_OUTPUT_H_
#include <inttypes.h>
#include <va/va.h>
#include "ipvr_drv_video.h"
#include "ipvr_surface.h"
#include "hwdefs/img_types.h"

#define IPVR_MAX_IMAGE_FORMATS      4 /* sizeof(ipvr__CreateImageFormat)/sizeof(VAImageFormat) */
#define IPVR_MAX_SUBPIC_FORMATS     3 /* sizeof(ipvr__SubpicFormat)/sizeof(VAImageFormat) */
#define IPVR_MAX_DISPLAY_ATTRIBUTES 14     /* sizeof(ipvr__DisplayAttribute)/sizeof(VADisplayAttribute) */

#define ipvr__ImageNV12                          \
{                                               \
    VA_FOURCC_NV12,                             \
    VA_LSB_FIRST,                               \
    16,                                         \
    0,                                          \
    0,                                          \
    0,                                          \
    0,                                          \
    0                                           \
}

VAStatus ipvr__destroy_subpicture(ipvr_driver_data_p driver_data, object_subpic_p obj_subpic);
VAStatus ipvr__destroy_image(ipvr_driver_data_p driver_data, object_image_p obj_image);

/*
 * VAImage call these buffer routines
 */
VAStatus ipvr__CreateBuffer(
    ipvr_driver_data_p driver_data,
    object_context_p obj_context,   /* in */
    VABufferType type,      /* in */
    unsigned int size,      /* in */
    unsigned int num_elements, /* in */
    unsigned char *data,             /* in */
    VABufferID *buf_desc    /* out */
);

VAStatus ipvr_DestroyBuffer(
    VADriverContextP ctx,
    VABufferID buffer_id
);

VAStatus ipvr_QueryImageFormats(
    VADriverContextP ctx,
    VAImageFormat *format_list,        /* out */
    int *num_formats           /* out */
);

VAStatus ipvr_CreateImage(
    VADriverContextP ctx,
    VAImageFormat *format,
    int width,
    int height,
    VAImage *image     /* out */
);

VAStatus ipvr_DeriveImage(
    VADriverContextP ctx,
    VASurfaceID surface,
    VAImage *image     /* out */
);

VAStatus ipvr_DestroyImage(
    VADriverContextP ctx,
    VAImageID image
);


VAStatus ipvr_SetImagePalette(
    VADriverContextP ctx,
    VAImageID image,
    /*
     * pointer to an array holding the palette data.  The size of the array is
     * num_palette_entries * entry_bytes in size.  The order of the components
     * in the palette is described by the component_order in VAImage struct
     */
    unsigned char *palette
);

VAStatus ipvr_GetImage(
    VADriverContextP ctx,
    VASurfaceID surface,
    int x,     /* coordinates of the upper left source pixel */
    int y,
    unsigned int width, /* width and height of the region */
    unsigned int height,
    VAImageID image
);

VAStatus ipvr_PutImage(
    VADriverContextP ctx,
    VASurfaceID surface,
    VAImageID image,
    int src_x,
    int src_y,
    unsigned int src_width,
    unsigned int src_height,
    int dest_x,
    int dest_y,
    unsigned int dest_width,
    unsigned int dest_height
);

VAStatus ipvr_QuerySubpictureFormats(
    VADriverContextP ctx,
    VAImageFormat *format_list,        /* out */
    unsigned int *flags,       /* out */
    unsigned int *num_formats  /* out */
);

VAStatus ipvr_CreateSubpicture(
    VADriverContextP ctx,
    VAImageID image,
    VASubpictureID *subpicture   /* out */
);

VAStatus ipvr_DestroySubpicture(
    VADriverContextP ctx,
    VASubpictureID subpicture
);

VAStatus ipvr_SetSubpictureImage(
    VADriverContextP ctx,
    VASubpictureID subpicture,
    VAImageID image
);


VAStatus ipvr_SetSubpictureChromakey(
    VADriverContextP ctx,
    VASubpictureID subpicture,
    unsigned int chromakey_min,
    unsigned int chromakey_max,
    unsigned int chromakey_mask
);

VAStatus ipvr_SetSubpictureGlobalAlpha(
    VADriverContextP ctx,
    VASubpictureID subpicture,
    float global_alpha
);

VAStatus ipvr_AssociateSubpicture(
    VADriverContextP ctx,
    VASubpictureID subpicture,
    VASurfaceID *target_surfaces,
    int num_surfaces,
    short src_x, /* upper left offset in subpicture */
    short src_y,
    unsigned short src_width,
    unsigned short src_height,
    short dest_x, /* upper left offset in surface */
    short dest_y,
    unsigned short dest_width,
    unsigned short dest_height,
    /*
     * whether to enable chroma-keying or global-alpha
     * see VA_SUBPICTURE_XXX values
     */
    unsigned int flags
);

VAStatus ipvr_DeassociateSubpicture(
    VADriverContextP ctx,
    VASubpictureID subpicture,
    VASurfaceID *target_surfaces,
    int num_surfaces
);

void ipvr_SurfaceDeassociateSubpict(
    ipvr_driver_data_p driver_data,
    object_surface_p obj_surface
);

/*
 * Query display attributes
 * The caller must provide a "attr_list" array that can hold at
 * least vaMaxNumDisplayAttributes() entries. The actual number of attributes
 * returned in "attr_list" is returned in "num_attributes".
 */
VAStatus ipvr_QueryDisplayAttributes(
    VADriverContextP ctx,
    VADisplayAttribute *attr_list,      /* out */
    int *num_attributes         /* out */
);

/*
 * Get display attributes
 * This function returns the current attribute values in "attr_list".
 * Only attributes returned with VA_DISPLAY_ATTRIB_GETTABLE set in the "flags" field
 * from vaQueryDisplayAttributes() can have their values retrieved.
 */
VAStatus ipvr_GetDisplayAttributes(
    VADriverContextP ctx,
    VADisplayAttribute *attr_list,      /* in/out */
    int num_attributes
);

/*
 * Set display attributes
 * Only attributes returned with VA_DISPLAY_ATTRIB_SETTABLE set in the "flags" field
 * from vaQueryDisplayAttributes() can be set.  If the attribute is not settable or
 * the value is out of range, the function returns VA_STATUS_ERROR_ATTR_NOT_SUPPORTED
 */
VAStatus ipvr_SetDisplayAttributes(
    VADriverContextP ctx,
    VADisplayAttribute *attr_list,
    int num_attributes
);
#endif /* _IPVR_OUTPUT_H_ */
