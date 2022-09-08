/******************************************************************************

File Name : loadfont.h

Description: Utility functions to load STGFX fonts from host files
                    (previously STGFX_Load/FreeFont)

******************************************************************************/

#ifndef  LOAD_FONT_H
#define  LOAD_FONT_H

/* Includes ------------------------------------------------------------ */

/* Exported Types ------------------------------------------------------ */

/* Exported Constants -------------------------------------------------- */

/* To simplify porting from STGFX_Load/FreeFont, an ST_ErrorCode_t is still
  used, with the following extending the STGFX error space: */
enum
{
    GFX_ERROR_OPENING_FONT_FILE = STGFX_DRIVER_BASE + 0x100,
    GFX_ERROR_INVALID_RECNUM
};

/* Exported Variables -------------------------------------------------- */

/* Exported Macros ----------------------------------------------------- */

/* Exported Function Prototypes ---------------------------------------- */

ST_ErrorCode_t GFX_LoadFont(ST_Partition_t*     Partition_p,
                            const ST_String_t   FileName,
                            STGFX_Font_t**      Font_p_p,
                            U8                  NumRecord);

ST_ErrorCode_t GFX_FreeFont(ST_Partition_t*     Partition_p,
                            STGFX_Font_t*       Font_p);


#endif /* LOAD_FONT_H */


