/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file libvlc_dll.h
 * description
 *
 * @author: inx limited
 *
 */

#ifndef EHS_LIBVLC_H
#define EHS_LIBVLC_H


/*****************************************************************************/
/* Included files */

#include <windows.h>

#define VLC_PUBLIC_API extern
typedef short bool;

#include "globals.h"
#include "vlcinc/vlc/libvlc.h"
//#include "vlcinc/vlc_block.h"

/*****************************************************************************/
/* Define macros  */

/**
 * Helper macro for declaring DLL functions.
 */
#define DeclareLibFunction(returnType, name, params) \
	extern returnType (*DLL##name) params; \

#define EHSTV_LIBVLC_PATH "C:\\Program Files\\VideoLAN\\VLC\\libvlc.dll"

#define VOUT_MAX_PLANES 5		/**< From vlc_config.h */
#define VOUT_MAX_PICTURES 8		/**< From vlc_config.h */
#define VOUT_FPS_SAMPLES 20		/**< From vlc_config.h */

#define VLC_OBJECT_INPUT (-7)
/* Plane indices */
#define Y_PLANE      0		/**< From vlc_video.h */
#define U_PLANE      1		/**< From vlc_video.h */
#define V_PLANE      2		/**< From vlc_video.h */
#define A_PLANE      3		/**< From vlc_video.h */

/* Shortcuts */
#define Y_PIXELS     p[Y_PLANE].p_pixels	/**< From vlc_video.h */
#define Y_PITCH      p[Y_PLANE].i_pitch		/**< From vlc_video.h */
#define U_PIXELS     p[U_PLANE].p_pixels	/**< From vlc_video.h */
#define U_PITCH      p[U_PLANE].i_pitch		/**< From vlc_video.h */
#define V_PIXELS     p[V_PLANE].p_pixels	/**< From vlc_video.h */
#define V_PITCH      p[V_PLANE].i_pitch		/**< From vlc_video.h */
#define A_PIXELS     p[A_PLANE].p_pixels	/**< From vlc_video.h */
#define A_PITCH      p[A_PLANE].i_pitch		/**< From vlc_video.h */

#define FIND_ANYWHERE       0x0003		/**< From vlc_objects.h */

#define VLC_OBJECT_VOUT       (-9)		/**< From vlc_objects.h */

#define VLC_FOURCC(a,b,c,d)  ( ((uint32_t)a) | ( ((uint32_t)b) << 8 ) \
	| ( ((uint32_t)c) << 16 ) | ( ((uint32_t)d) << 24 ) )		/**< From vlc_common.h */

/*****************************************************************************/
/* Define types */

typedef unsigned char uint8_t;			/**< From vlc_common.h */
typedef unsigned short uint16_t;		/**< From vlc_common.h */
typedef unsigned int uint32_t;			/**< From vlc_common.h */
typedef unsigned long long int64_t;		/**< From vlc_common.h */
typedef int vlc_bool_t;					/**< From vlc_common.h */
typedef int64_t mtime_t;				/**< From vlc_common.h */
typedef unsigned long count_t;			/**< From vlc_common.h */

typedef uint32_t vlc_fourcc_t;			/**< From vlc_common.h */

typedef struct intf_thread_t intf_thread_t;		/**< From vlc_common.h */
typedef struct module_t module_t;				/**< From vlc_common.h */
typedef struct picture_heap_t picture_heap_t;	/**< From vlc_common.h */
typedef struct picture_sys_t picture_sys_t;		/**< From vlc_common.h */
typedef struct spu_t spu_t;						/**< From vlc_common.h */
typedef struct subpicture_t subpicture_t;		/**< From vlc_common.h */
typedef struct vout_sys_t vout_sys_t;			/**< From vlc_common.h */
typedef struct vout_thread_t vout_thread_t;		/**< From vlc_common.h */
typedef struct vlc_object_internals_t vlc_object_internals_t;	/**< From vlc_common.h */
typedef struct libvlc_int_t libvlc_int_t;		/**< From vlc_common.h */
typedef struct text_style_t text_style_t;		/**< From vlc_common.h */
typedef struct subpicture_sys_t subpicture_sys_t;	/**< From vlc_common.h */

typedef struct libvlc_input_t libvlc_input_t;	/**< from libvlc_internal.h */

#include "vlc_threads.h"

/******************************* from vlc_es.h *******************************/

struct video_palette_t
{
    int i_entries;
    uint8_t palette[256][4];
};

typedef struct video_palette_t video_palette_t;

/**
 * video format description
 */
struct video_format_t
{
    vlc_fourcc_t i_chroma;                               /**< picture chroma */
    unsigned int i_aspect;                                 /**< aspect ratio */

    unsigned int i_width;                                 /**< picture width */
    unsigned int i_height;                               /**< picture height */
    unsigned int i_x_offset;               /**< start offset of visible area */
    unsigned int i_y_offset;               /**< start offset of visible area */
    unsigned int i_visible_width;                 /**< width of visible area */
    unsigned int i_visible_height;               /**< height of visible area */

    unsigned int i_bits_per_pixel;             /**< number of bits per pixel */

    unsigned int i_sar_num;                   /**< sample/pixel aspect ratio */
    unsigned int i_sar_den;

    unsigned int i_frame_rate;                     /**< frame rate numerator */
    unsigned int i_frame_rate_base;              /**< frame rate denominator */

    int i_rmask, i_gmask, i_bmask;          /**< color masks for RGB chroma */
    video_palette_t *p_palette;              /**< video palette from demuxer */
};

typedef struct video_format_t video_frame_format_t;
typedef struct video_format_t video_format_t;
typedef struct subpicture_region_t subpicture_region_t;

/**
 * from vlc_video.h
 */
struct plane_t
{
    uint8_t* p_pixels;
    int i_lines;
    int i_pitch;
    int i_pixel_pitch;
    int i_visible_lines;
    int i_visible_pitch;
};

/**
 * from vlc_video.h
 */
typedef struct plane_t plane_t;
/**
 * from vlc_video.h
 */
typedef struct picture_t picture_t;

/**
 * from vlc_video.h
 * Video picture heap, either render (to store pictures used
 * by the decoder) or output (to store pictures displayed by the vout plugin)
 */
struct picture_heap_t
{
    int i_pictures;                                   /**< current heap size */

    /* \name Picture static properties
     * Those properties are fixed at initialization and should NOT be modified
     * @{
     */
    unsigned int i_width;                                 /**< picture width */
    unsigned int i_height;                               /**< picture height */
    vlc_fourcc_t i_chroma;                               /**< picture chroma */
    unsigned int i_aspect;                                 /**< aspect ratio */
    /**@}*/

    /* Real pictures */
    picture_t*      pp_picture[VOUT_MAX_PICTURES];             /**< pictures */
    int             i_last_used_pic;              /**< last used pic in heap */
    vlc_bool_t      b_allow_modify_pics;

    /* Stuff used for truecolor RGB planes */
    uint32_t i_rmask;
    int i_rrshift, i_lrshift;
    uint32_t i_gmask;
    int i_rgshift, i_lgshift;
    uint32_t i_bmask;
    int i_rbshift, i_lbshift;

    /** Stuff used for palettized RGB planes */
    void (* pf_setpalette) ( vout_thread_t *, uint16_t *, uint16_t *, uint16_t * );
};

/**
 * Video picture
 * from vlc_video.h
 *
 * Any picture destined to be displayed by a video output thread should be
 * stored in this structure from it's creation to it's effective display.
 * Picture type and flags should only be modified by the output thread. Note
 * that an empty picture MUST have its flags set to 0.
 */
struct picture_t
{
    /**
     * The properties of the picture
     */
    video_frame_format_t format;

    /** Picture data - data can always be freely modified, but p_data may
     * NEVER be modified. A direct buffer can be handled as the plugin
     * wishes, it can even swap p_pixels buffers. */
    uint8_t        *p_data;
    void           *p_data_orig;                /**< pointer before memalign */
    plane_t         p[ VOUT_MAX_PLANES ];     /**< description of the planes */
    int             i_planes;                /**< number of allocated planes */

    /** \name Type and flags
     * Should NOT be modified except by the vout thread
     * @{*/
    int             i_status;                             /**< picture flags */
    int             i_type;                /**< is picture a direct buffer ? */
    vlc_bool_t      b_slow;                 /**< is picture in slow memory ? */
    int             i_matrix_coefficients;   /**< in YUV type, encoding type */
    /**@}*/

    /** \name Picture management properties
     * These properties can be modified using the video output thread API,
     * but should never be written directly */
    /**@{*/
    int             i_refcount;                  /**< link reference counter */
    mtime_t         date;                                  /**< display date */
    vlc_bool_t      b_force;
    /**@}*/

    /** \name Picture dynamic properties
     * Those properties can be changed by the decoder
     * @{
     */
    vlc_bool_t      b_progressive;          /**< is it a progressive frame ? */
    unsigned int    i_nb_fields;                  /**< # of displayed fields */
    vlc_bool_t      b_top_field_first;             /**< which field is first */
    /**@}*/

    /** The picture heap we are attached to */
    picture_heap_t* p_heap;

    /* Some vouts require the picture to be locked before it can be modified */
    int (* pf_lock) ( vout_thread_t *, picture_t * );
    int (* pf_unlock) ( vout_thread_t *, picture_t * );

    /** Private data - the video output plugin might want to put stuff here to
     * keep track of the picture */
    picture_sys_t * p_sys;

    /** This way the picture_Release can be overloaded */
    void (*pf_release)( picture_t * );

    /** Next picture in a FIFO a pictures */
    struct picture_t *p_next;
};

/**
 * Video subtitle region
 * from vlc_video.h
 *
 * A subtitle region is defined by a picture (graphic) and its rendering
 * coordinates.
 * Subtitles contain a list of regions.
 */
struct subpicture_region_t
{
    video_format_t  fmt;                          /**< format of the picture */
    picture_t       picture;             /**< picture comprising this region */

    int             i_x;                             /**< position of region */
    int             i_y;                             /**< position of region */
    int             i_align;                  /**< alignment within a region */

    char            *psz_text;       /**< text string comprising this region */
    text_style_t    *p_style;  /* a description of the text style formatting */

    subpicture_region_t *p_next;                /**< next region in the list */
    subpicture_region_t *p_cache;       /**< modified version of this region */
};

/**
 * Video subtitle
 * from vlc_video.h
 *
 * Any subtitle destined to be displayed by a video output thread should
 * be stored in this structure from it's creation to it's effective display.
 * Subtitle type and flags should only be modified by the output thread. Note
 * that an empty subtitle MUST have its flags set to 0.
 */
struct subpicture_t
{
    /** \name Channel ID */
    /**@{*/
    int             i_channel;                    /**< subpicture channel ID */
    /**@}*/

    /** \name Type and flags
       Should NOT be modified except by the vout thread */
    /**@{*/
    int             i_type;                                        /**< type */
    int             i_status;                                     /**< flags */
    subpicture_t *  p_next;               /**< next subtitle to be displayed */
    /**@}*/

    /** \name Date properties */
    /**@{*/
    mtime_t         i_start;                  /**< beginning of display date */
    mtime_t         i_stop;                         /**< end of display date */
    vlc_bool_t      b_ephemer;    /**< If this flag is set to true the subtitle
                                will be displayed untill the next one appear */
    vlc_bool_t      b_fade;                               /**< enable fading */
    vlc_bool_t      b_pausable;               /**< subpicture will be paused if
                                                            stream is paused */
    /**@}*/

    subpicture_region_t *p_region;  /**< region list composing this subtitle */

    /** \name Display properties
     * These properties are only indicative and may be
     * changed by the video output thread, or simply ignored depending of the
     * subtitle type. */
    /**@{*/
    int          i_x;                    /**< offset from alignment position */
    int          i_y;                    /**< offset from alignment position */
    int          i_width;                                 /**< picture width */
    int          i_height;                               /**< picture height */
    int          i_alpha;                                  /**< transparency */
    int          i_original_picture_width;  /**< original width of the movie */
    int          i_original_picture_height;/**< original height of the movie */
    vlc_bool_t   b_absolute;                       /**< position is absolute */
    int          i_flags;                                /**< position flags */
    /**@}*/

    /** Pointer to function that renders this subtitle in a picture */
    void ( *pf_render )  ( vout_thread_t *, picture_t *, const subpicture_t * );
    /** Pointer to function that cleans up the private data of this subtitle */
    void ( *pf_destroy ) ( subpicture_t * );

    /** Pointer to functions for region management */
    subpicture_region_t * ( *pf_create_region ) ( vlc_object_t *,
            video_format_t * );
    subpicture_region_t * ( *pf_make_region ) ( vlc_object_t *,
            video_format_t *, picture_t * );
    void ( *pf_destroy_region ) ( vlc_object_t *, subpicture_region_t * );

    /** Private data - the subtitle plugin might want to put stuff here to
     * keep track of the subpicture */
    subpicture_sys_t *p_sys;                              /* subpicture data */
};

/**
 * from libvlc_internal.h
 */
struct libvlc_instance_t
{
    /* vlc_t*		*/ void* p_vlc;
    /* playlist_t*	*/ void* p_playlist;
    /* vlm_t*		*/ void* p_vlm;
    int i_vlc_id;
};

/**
 * Chroma conversion function
 * from video_output.h
 *
 * This is the prototype common to all conversion functions.
 * \param p_vout video output thread
 * \param p_source source picture
 * \param p_dest destination picture
 * Picture width and source dimensions must be multiples of 16.
 */
typedef void (vout_chroma_convert_t)( vout_thread_t *,
                                      picture_t *, picture_t * );

/*
 * from video_output.h
 */
typedef struct vout_chroma_t
{
    /** conversion functions */
    vout_chroma_convert_t *pf_convert;

    /** Private module-dependant data */
    /* chroma_sys_t *  */ void*     p_sys;                               /* private data */

    /** Plugin used and shortcuts to access its capabilities */
    module_t * p_module;

} vout_chroma_t;



/**
 * Video output thread descriptor
 * from video_output.h
 *
 * Any independant video output device, such as an X11 window or a GGI device,
 * is represented by a video output thread, and described using the following
 * structure.
 */
struct vout_thread_t
{
    /** \name VLC_COMMON_MEMBERS
     * these members are common for all vlc objects
     */
    /**@{*/
    vlc_object_internals_t *p_internals;
    short   i_object_id;   /* ACC int -> short, to fit memory */
    short   i_object_type; /* ACC int -> short, to fit memory */
    const char *psz_object_type;
    const char *psz_object_name;

    /* Messages header */
    char *psz_header;
    short  i_flags;  /* ACC int -> short, to fit memory */

    /* Object access lock */
    vlc_mutex_t  object_lock;
    vlc_cond_t   object_wait;

    /* Object properties */
    volatile bool b_error;                  /**< set by the object */
    volatile bool b_die;                   /**< set by the outside */
    volatile bool b_dead;                   /**< set by the object */
    bool b_force;      /**< set by the outside (eg. module_Need()) */

    /* Stuff related to the libvlc structure */
    libvlc_int_t *p_libvlc;                  /**< (root of all evil) - 1 */

    vlc_object_t *  p_parent;                            /**< our parent */
    vlc_object_t ** pp_children;                       /**< our children */
    volatile int    i_children;

    /* Private data */
    void *          p_private;

    /** Just a reminder so that people don't cast garbage */
    int be_sure_to_add_VLC_COMMON_MEMBERS_to_struct;
    /**@} End of VLC_COMMON_MEMBERS */

    /** \name Thread properties and locks */
    /**@{*/
    vlc_mutex_t         picture_lock;                 /**< picture heap lock */
    vlc_mutex_t         subpicture_lock;           /**< subpicture heap lock */
    vlc_mutex_t         change_lock;                 /**< thread change lock */
    vout_sys_t *        p_sys;                     /**< system output method */
    /**@}*/

    /** \name Current display properties */
    /**@{*/
    uint16_t            i_changes;          /**< changes made to the thread.
                                                      \see \ref vout_changes */
    float               f_gamma;                                  /**< gamma */
    vlc_bool_t          b_grayscale;         /**< color or grayscale display */
    vlc_bool_t          b_info;            /**< print additional information */
    vlc_bool_t          b_interface;                   /**< render interface */
    vlc_bool_t          b_scale;                  /**< allow picture scaling */
    vlc_bool_t          b_fullscreen;         /**< toogle fullscreen display */
    uint32_t            render_time;           /**< last picture render time */
    unsigned int        i_window_width;              /**< video window width */
    unsigned int        i_window_height;            /**< video window height */
    unsigned int        i_alignment;          /**< video alignment in window */
    unsigned int        i_par_num;           /**< monitor pixel aspect-ratio */
    unsigned int        i_par_den;           /**< monitor pixel aspect-ratio */

    intf_thread_t       *p_parent_intf;   /**< parent interface for embedded
                                                               vout (if any) */
    /**@}*/

    /** \name Plugin used and shortcuts to access its capabilities */
    /**@{*/
    module_t *   p_module;
    int       ( *pf_init )       ( vout_thread_t * );
    void      ( *pf_end )        ( vout_thread_t * );
    int       ( *pf_manage )     ( vout_thread_t * );
    void      ( *pf_render )     ( vout_thread_t *, picture_t * );
    void      ( *pf_display )    ( vout_thread_t *, picture_t * );
    void      ( *pf_swap )       ( vout_thread_t * );         /* OpenGL only */
    int       ( *pf_lock )       ( vout_thread_t * );         /* OpenGL only */
    void      ( *pf_unlock )     ( vout_thread_t * );         /* OpenGL only */
    int       ( *pf_control )    ( vout_thread_t *, int, va_list );
    /**@}*/

    /** \name Statistics
     * These numbers are not supposed to be accurate, but are a
     * good indication of the thread status */
    /**@{*/
    count_t       c_fps_samples;                         /**< picture counts */
    mtime_t       p_fps_sample[VOUT_FPS_SAMPLES];     /**< FPS samples dates */
    /**@}*/

    /** \name Video heap and translation tables */
    /**@{*/
    int                 i_heap_size;                          /**< heap size */
    picture_heap_t      render;                       /**< rendered pictures */
    picture_heap_t      output;                          /**< direct buffers */
    vlc_bool_t          b_direct;            /**< rendered are like direct ? */
    vout_chroma_t       chroma;                      /**< translation tables */

    video_format_t      fmt_render;      /* render format (from the decoder) */
    video_format_t      fmt_in;            /* input (modified render) format */
    video_format_t      fmt_out;     /* output format (for the video output) */
    /**@}*/

    /* Picture heap */
    picture_t           p_picture[2*VOUT_MAX_PICTURES+1];      /**< pictures */

    /* Subpicture unit */
    spu_t            *p_spu;

    /* Statistics */
    count_t          c_loops;
    count_t          c_pictures, c_late_pictures;
    mtime_t          display_jitter;    /**< average deviation from the PTS */
    count_t          c_jitter_samples;  /**< number of samples used
                                           for the calculation of the
                                           jitter  */
    /** delay created by internal caching */
    int                 i_pts_delay;

    /* Filter chain */
    char *psz_filter_chain;
    vlc_bool_t b_filter_change;

    /* Misc */
    vlc_bool_t       b_snapshot;     /**< take one snapshot on the next loop */
};

/*****************************************************************************/
/* Declare global variables */


/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Find a typed object and increment its refcount.
 * @see DLL__vlc_object_release */
DeclareLibFunction(vout_thread_t*,__vlc_object_find, (vlc_object_t* p_this, int i_type, int i_mode))

/**
 * Decrement object's refcount, release if it gets to zero.
 * @see DLL__vlc_object_find
 */
DeclareLibFunction(void, __vlc_object_release, (vlc_object_t*))

/**
 * Send a message to the screen at a fixed position. Hide after a given time
 */
DeclareLibFunction(void,__vout_OSDMessage,(vlc_object_t *, int, char *, ...))

/**
 * Destroy a libvlc instance
 */
DeclareLibFunction(void, libvlc_destroy,( libvlc_instance_t *p_instance ))

/**
 * Get exception message
 */
DeclareLibFunction(const char *,libvlc_exception_get_message,( const libvlc_exception_t *p_exception ))

/**
 * Test if an exception has been raised
 */
DeclareLibFunction(int, libvlc_exception_raised,( const libvlc_exception_t *p_exception ))

/**
 * Initialise an exception structure. This can be called several times to reuse an exception structure.
 */
DeclareLibFunction(void,libvlc_exception_init,( libvlc_exception_t *p_exception ))

/**
 * Get the current play position for the currently playing video
 */
DeclareLibFunction(float,libvlc_input_get_position, ( libvlc_input_t *p_input, libvlc_exception_t *p_e ))

/**
 * Create an initialised libvlc instance
 */
DeclareLibFunction(libvlc_instance_t *,libvlc_new,( int, const char *const *, libvlc_exception_t *));

/**
 * Set the default parent drawable for video outputs
 */
DeclareLibFunction(void,libvlc_video_set_parent,(  	libvlc_instance_t *   	,
                   libvlc_drawable_t  	,
                   libvlc_exception_t *
                                                ))

/**
 * Add an item to the playlist
 */
DeclareLibFunction(int,libvlc_playlist_add,(libvlc_instance_t*, const char*, const char*, libvlc_exception_t*))

/**
 * Get the input instance pointer
 */
DeclareLibFunction(libvlc_input_t*,	libvlc_playlist_get_input,(libvlc_instance_t*, libvlc_exception_t* p_e))

/**
 * Pause/resume the current item
 */
DeclareLibFunction(void,				libvlc_playlist_pause, (libvlc_instance_t *, libvlc_exception_t*))

/**
 * Play the current playlist
 */
DeclareLibFunction(void,libvlc_playlist_play,(libvlc_instance_t *, int, int, char**,libvlc_exception_t*))

/**
 * Set/change the default size for video outputs
 */
DeclareLibFunction(void, libvlc_video_set_size,( libvlc_instance_t *, int, int, libvlc_exception_t * ))

/**
 * High precision date
 */
DeclareLibFunction(mtime_t, mdate, (void))

/**
 * Allocate a subpicture in the spu heap
 */
DeclareLibFunction(subpicture_t*, spu_CreateSubpicture,(spu_t* p_spu))

/**
 * Display a subpicture
 */
DeclareLibFunction(void, spu_DisplaySubpicture, (spu_t* p_spu, subpicture_t* p_subpic))

/**
 * Cause one of four specific icons to be displayed for a short period of time
 */
DeclareLibFunction(void,vout_OSDIcon,(vlc_object_t *, int i_channel, short i_type))

/**
 * Return the number of seconds in the specified item
 */
DeclareLibFunction(int,VLC_LengthGet,(int i_object))

/**
 * Returns the fraction of the current item that has been played
 */
DeclareLibFunction(float,VLC_PositionGet,(int i_object))

/**
 * Get the version id of VLC (deprecated)
 */
DeclareLibFunction(char const*,VLC_Version,(void ))

/*
 * The following functions are not exported by the libvlc DLL.
 */
DeclareLibFunction(void,libvlc_media_release,( libvlc_media_t *p_meta_desc ))

DeclareLibFunction(libvlc_media_t *,libvlc_media_new,(
                       libvlc_instance_t *p_instance,
                       const char * psz_mrl,
                       libvlc_exception_t *p_e ))

DeclareLibFunction(libvlc_media_player_t *, libvlc_media_player_new_from_media,( libvlc_media_t *, libvlc_exception_t * ))
DeclareLibFunction(void,libvlc_media_player_set_drawable, ( libvlc_media_player_t *, libvlc_drawable_t, libvlc_exception_t * ))
DeclareLibFunction(void,libvlc_media_player_release,( libvlc_media_player_t * ))
DeclareLibFunction(void,libvlc_media_player_stop,( libvlc_media_player_t *, libvlc_exception_t * ))
DeclareLibFunction(void,libvlc_media_player_play,( libvlc_media_player_t *, libvlc_exception_t * ))
DeclareLibFunction(void,libvlc_release,( libvlc_instance_t * ))

DeclareLibFunction(libvlc_media_player_t *, libvlc_media_player_new,( libvlc_instance_t *, libvlc_exception_t * ))
DeclareLibFunction(libvlc_media_list_player_t *, libvlc_media_list_player_new, ( libvlc_instance_t * p_instance, libvlc_exception_t * p_e ))
DeclareLibFunction(float, libvlc_media_player_get_position, ( libvlc_media_player_t *, libvlc_exception_t *))
DeclareLibFunction(void, libvlc_media_player_set_position, ( libvlc_media_player_t *, float, libvlc_exception_t *))
DeclareLibFunction(libvlc_time_t, libvlc_media_player_get_time, ( libvlc_media_player_t *, libvlc_exception_t *))
DeclareLibFunction(void, libvlc_media_player_set_time, ( libvlc_media_player_t *, libvlc_time_t, libvlc_exception_t *))
DeclareLibFunction(libvlc_time_t, libvlc_media_player_get_length, ( libvlc_media_player_t *, libvlc_exception_t *))
DeclareLibFunction(float, libvlc_media_player_get_rate, ( libvlc_media_player_t *, libvlc_exception_t *))
DeclareLibFunction(void, libvlc_media_player_set_rate, ( libvlc_media_player_t *, float, libvlc_exception_t *))
DeclareLibFunction(void, libvlc_media_player_pause, ( libvlc_media_player_t *, libvlc_exception_t * ))
DeclareLibFunction(char *, libvlc_media_get_mrl, ( libvlc_media_t * p_md, libvlc_exception_t * p_e ))
DeclareLibFunction(libvlc_media_t *, libvlc_media_player_get_media, ( libvlc_media_player_t *, libvlc_exception_t * ))
DeclareLibFunction(void, libvlc_media_player_set_media, ( libvlc_media_player_t *, libvlc_media_t *, libvlc_exception_t * ))
DeclareLibFunction(int, libvlc_media_get_id, (libvlc_media_player_t *, libvlc_exception_t *))
DeclareLibFunction(libvlc_media_player_t *, libvlc_playlist_get_media_player, (libvlc_instance_t *, libvlc_exception_t *))
DeclareLibFunction(int, libvlc_playlist_isplaying, (libvlc_media_player_t *, libvlc_exception_t *))

/********************* Declarations of global variables **********************/
/**
 * Records the playlist id of the video we want to use in the background.
 */
EHS_GLOBAL ehs_sint32 EhsTV_backgroundId;

/**
 * Reference of the libvlc instance
 */
EHS_GLOBAL libvlc_instance_t * EhsTV_pLibvlc;
EHS_GLOBAL libvlc_media_player_t* pMediaPlayer;
EHS_GLOBAL libvlc_media_list_t* pMediaList;
EHS_GLOBAL libvlc_media_list_player_t* pMediaListPlayer;
EHS_GLOBAL libvlc_event_manager_t* pEventManager;
EHS_GLOBAL libvlc_instance_t * EhsTV_pLibvlc;

/**
 * Exception handler for libvlc functions
 */
EHS_GLOBAL libvlc_exception_t EhsTV_excp;

EHS_GLOBAL void EhsTV_quit_on_exception( libvlc_exception_t * excp );
EHS_GLOBAL float EhsTV_getPosition();
EHS_GLOBAL void EhsTV_setPosition(float nPos);
EHS_GLOBAL libvlc_time_t EhsTV_getTime();
EHS_GLOBAL void EhsTV_setTime(libvlc_time_t nTime);
EHS_GLOBAL libvlc_time_t EhsTV_getLength();
EHS_GLOBAL float EhsTV_getSpeed();
EHS_GLOBAL void EhsTV_setSpeed(float nSpeed);
EHS_GLOBAL void EhsTV_pause();
EHS_GLOBAL void EhsTV_stop();

static int indexToPlay;

/**
 * Display resolution
 */
#define EHS_TV_RES_X (544*2)
#define EHS_TV_RES_Y (288*2)
#endif /*EHS_LIBVLC_H*/
