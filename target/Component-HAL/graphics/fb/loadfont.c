/******************************************************************************

    File Name   : loadfont.c

    Description : Utility functions to load STGFX fonts from host files
                    (previously STGFX_Load/FreeFont)

******************************************************************************/
#define LOAD_FONT_FROM_FILE
/* Includes ---------------------------------------------------------------- */

#include <string.h> /* stdlib.h */
#include <stdio.h>
#include <ctype.h>  /*lint --e(123) */
#include <sys/stat.h>
#include <stddefs.h>
#include "typedefs.h"
//#include "gfx.h"
#ifndef  LOAD_FONT_FROM_FILE
#include "courb18.h"
#endif

#if !defined(MODULE)
/* *********** File manipulation ***************** */
/*
 * debugfilesize
 */
long int debugfilesize (long int FileDescriptor)
{
    struct stat StatBuf;

    fstat (FileDescriptor, &StatBuf);

    return (StatBuf.st_size);

}
#endif

#ifdef DEBUG
#define AlePrint(x)         //STTBX_Print(x)
#else
#define AlePrint(x)
#endif

#ifdef LOADFONT_PRINT
#define LOADFONT_Print(x)   STTBX_Print(x)
#else
#define LOADFONT_Print(x)
#endif

/* Private Types ----------------------------------------------------------- */

/* Private Constants ------------------------------------------------------- */

/* There is simply no point loading glyphs with index > 256,
  because char string elements can't address them */
#define NUM_OF_SUPPORTED_CHARS  256
#define DEFAULT_CHARACTER        35 /* default default character */
#define MAX_STRING_LENGTH        64

#define MAX_SUPPORTED_CHAR  255


#define BDF_EXT ".bdf"


/* Private Variables ------------------------------------------------------- */

/* Private Macros ---------------------------------------------------------- */

/* Private Function Prototypes --------------------------------------------- */

ST_ErrorCode_t LoadBitmappedFont(ST_Partition_t* Partition_p,
                                 U8* FileBuf_p, U32 BufSize,STGFX_BitmappedFont_t* Font_p);
ST_ErrorCode_t FreeBitmappedFont(ST_Partition_t* Partition_p,
                                 STGFX_BitmappedFont_t* Font_p);




static void GetString(U8** bufPtr, char* buffer);
static U8   CharToHex(char* source);


/* Functions --------------------------------------------------------------- */


/******************************************************************************
Function Name : GFX_LoadFont
  Description : Used to be STGFX_LoadFont
   Parameters : partition from which to allocate memory
                filename to load from (extension indicates font type)
                pointer through which to return pointer to structures allocated
                record number to load (pfr fonts only)
      Returns : ST_NO_ERROR (success)
                ST_ERROR_BAD_PARAMETER (NULL input/unrecognised extension)
                ST_ERROR_NO_MEMORY (memory allocation failed)
                GFX_ERROR_OPENING_FONT_FILE (couldn't open/read file)
                STGFX_ERROR_INVALID_FONT (bad file extension/format)
                GFX_ERROR_INVALID_RECNUM (record number not found in pfr font)
                In all error cases, all resources are freed
******************************************************************************/
ST_ErrorCode_t GFX_LoadFont(ST_Partition_t*     Partition_p,
                            const ST_String_t   FileName,
                            STGFX_Font_t**      Font_p_p,
                            U8                  NumRecord)
{
    ST_ErrorCode_t   Err = ST_NO_ERROR;
    STGFX_Font_t*    Font_p;
    U8*              Mem_p;
    U32              FSize;
#ifdef  LOAD_FONT_FROM_FILE
    long int         FileDescriptor;
#endif


    if ((FileName == NULL) || (Font_p_p == NULL))
    {
        return(ST_ERROR_BAD_PARAMETER);
    }

    *Font_p_p = NULL;

    Font_p = (STGFX_Font_t*) memory_allocate(Partition_p,
             sizeof(STGFX_Font_t));
    if(Font_p == NULL)
    {
        return(ST_ERROR_NO_MEMORY);
    }
    memset(Font_p, 0, sizeof(STGFX_Font_t));


#ifdef  LOAD_FONT_FROM_FILE
    FileDescriptor = debugopen(FileName,"rb");
    if (FileDescriptor < 0)
    {
        memory_deallocate(Partition_p, Font_p);
        return(GFX_ERROR_OPENING_FONT_FILE);
    }
    FSize=(U32)debugfilesize(FileDescriptor);


    Mem_p = (U8*)memory_allocate(Partition_p,FSize);
    if (Mem_p == NULL)
    {
        /* Font_p freed later */
        Err=ST_ERROR_NO_MEMORY;
    }
    else
    {
        if (debugread(FileDescriptor,Mem_p,FSize) != FSize)
        {
            Err = GFX_ERROR_OPENING_FONT_FILE;
        }
        else
        {
            if (strstr(FileName,BDF_EXT))
            {
#else
    Mem_p = FontData;
    FSize = sizeof(FontData);
#endif
                Font_p->FontData.BitmappedFontData_p = (STGFX_BitmappedFont_t*)
                                                       memory_allocate(Partition_p,sizeof(STGFX_BitmappedFont_t));
                if (Font_p->FontData.BitmappedFontData_p == NULL)
                {
                    Err = ST_ERROR_NO_MEMORY;
                }
                else
                {
                    memset(Font_p->FontData.BitmappedFontData_p,0,
                           sizeof(STGFX_BitmappedFont_t));
                    Err = LoadBitmappedFont(Partition_p,Mem_p,FSize,
                                            Font_p->FontData.BitmappedFontData_p);
                    if (Err == ST_NO_ERROR)
                    {
                        Font_p->FontType = STGFX_BITMAPPED_FONT;
                    }
                    else
                    {
                        memory_deallocate(Partition_p,Font_p->FontData.BitmappedFontData_p);
                    }
                }
#ifdef  LOAD_FONT_FROM_FILE
            }

            else
            {
                Err=ST_ERROR_BAD_PARAMETER;
            }
        }
        LOADFONT_Print(("Deallocate memory image of file..."));
        memory_deallocate(Partition_p, Mem_p);
        LOADFONT_Print(("Done\n"));
    }

    LOADFONT_Print(("Close file..."));

    (void)debugclose(FileDescriptor);

    LOADFONT_Print(("Done\n"));
#endif

    if(Err == ST_NO_ERROR)
    {
        *Font_p_p = Font_p;
    }
    else
    {
        memory_deallocate(Partition_p, Font_p);
    }

    return ( Err );
}


/******************************************************************************
Function Name : GFX_FreeFont
  Description : Used to be STGFX_FreeFont
   Parameters : Partition in which font was allocated, font that was loaded
      Returns : ST_NO_ERROR (success),
                ST_ERROR_BAD_PARAMETER (NULL parameter, nothing done),
                ST_ERROR_INVALID_FONT (structure damaged, may not have freed
                all memory)
******************************************************************************/
ST_ErrorCode_t GFX_FreeFont(ST_Partition_t*     Partition_p,
                            STGFX_Font_t*       Font_p)
{
    ST_ErrorCode_t Err = ST_NO_ERROR;

    if((Partition_p == NULL) || (Font_p == NULL))
    {
        return ST_ERROR_BAD_PARAMETER;
    }

    switch (Font_p->FontType)
    {

    case STGFX_BITMAPPED_FONT:
        if (Font_p->FontData.BitmappedFontData_p)
        {
            Err=FreeBitmappedFont(Partition_p,Font_p->FontData.BitmappedFontData_p);
            memory_deallocate(Partition_p,Font_p->FontData.BitmappedFontData_p);
        }
        else
        {
            Err=STGFX_ERROR_INVALID_FONT;
        }
        break;

    default:
        Err=STGFX_ERROR_INVALID_FONT;
        break;
    }

    if(Font_p->CharMapping.Entries_p)
    {
        /* zeroed initially by a memset, but may have something placed later
        (currently in LoadOutlineFont) */
        memory_deallocate(Partition_p,Font_p->CharMapping.Entries_p);
    }

    memory_deallocate(Partition_p,Font_p);
    return(Err);
}

/******************************************************************************
Function Name : LoadBitmappedFont
  Description : Used toload Bitmapped Font
   Parameters : partition from which to allocate memory
                filename to load from (extension indicates font type)
                filesize
                pointer through which to return pointer to structures allocated

      Returns : ErrCode
******************************************************************************/

ST_ErrorCode_t LoadBitmappedFont(ST_Partition_t* Partition_p,
                                 U8* FileBuf_p, U32 BufSize,
                                 STGFX_BitmappedFont_t* Font_p)
{
    U8*            TmpPtr;
    char	         String[256];
    S32	         NumOfChars; /* actual number of glyphs the font file has */
    S32	         i,j,k;
    S32	         Position;
    S32	         BitmapSize;
    S32	         StringPos;
    BOOL           StartFont = FALSE;
    BOOL           EndFont = FALSE;
    ST_ErrorCode_t Err = ST_NO_ERROR;

    Font_p->DefaultChar = DEFAULT_CHARACTER;

    /* whilst glyphs always come in position order, there can be
    gaps, so without an initial pass we need to allocate space for
    all 256 possible positions, and mark NumOfGlyphs accordingly */

    AlePrint(("\nLoadBitmappedFont(): Allocate memory..."));
    Font_p->Glyph_p = (STGFX_BitmappedGlyph_t*)
                      memory_allocate(Partition_p,
                                      NUM_OF_SUPPORTED_CHARS*sizeof(STGFX_BitmappedGlyph_t));
    if(!Font_p->Glyph_p)
        return ST_ERROR_NO_MEMORY;
    AlePrint(("\nLoadBitmappedFont(): Memory Allocated."));

    memset(Font_p->Glyph_p,0,(NUM_OF_SUPPORTED_CHARS*
                              sizeof(STGFX_BitmappedGlyph_t)));
    Font_p->NumOfGlyphs = NUM_OF_SUPPORTED_CHARS;

    TmpPtr = FileBuf_p;
    while(TmpPtr<(FileBuf_p+BufSize))
    {
        GetString(&TmpPtr, String);
        if(strstr(String,"STARTFONT"))
        {
            AlePrint(("\nLoadBitmappedFont(): STARTFONT."));
            StartFont=TRUE;
        }
        else if(!strcmp(String,"FONT_ASCENT"))
        {
            AlePrint(("\nLoadBitmappedFont(): FONT_ASCENT."));
            GetString(&TmpPtr,String);
            Font_p->FontAscent = (S32)atoi(String);
        }
        else if(!strcmp(String,"FONT_DESCENT"))
        {
            AlePrint(("\nLoadBitmappedFont(): FONT_DESCENT."));
            GetString(&TmpPtr,String);
            Font_p->FontDescent = (S32)atoi(String);
        }
        else if(!strcmp(String,"DEFAULT_CHAR"))
        {
            AlePrint(("\nLoadBitmappedFont(): DEFAULT_CHAR."));
            GetString(&TmpPtr,String);
            Font_p->DefaultChar = (S32)atoi(String);
        }
        else if(!strcmp(String,"CHARS"))
        {
            AlePrint(("\nLoadBitmappedFont(): CHARS."));
            GetString(&TmpPtr,String);
            NumOfChars = (S32)atoi(String);
            AlePrint(("\nLoadBitmappedFont(): Number of CHARS = %u.",NumOfChars));
            /*if(NumOfChars > NUM_OF_SUPPORTED_CHARS)
            NumOfChars = NUM_OF_SUPPORTED_CHARS;*/
            /*Font_p->NumOfGlyphs = NUM_OF_SUPPORTED_CHARS;*/
            /*Font_p->NumOfGlyphs = NumOfChars; - now set above */

            /* note this code will overrun the source buffer if the file has
            fewer valid character entries than it claims */
            for(i = 0; i<NumOfChars ; i++)
            {
                AlePrint(("\nLoadBitmappedFont(): CHAR number %u.", i));
                for(;;)
                {
                    GetString(&TmpPtr,String);
                    if(!strcmp(String,"STARTCHAR")) break;
                }
                GetString(&TmpPtr,String);
                for(;;)
                {
                    GetString(&TmpPtr,String);
                    if(!strcmp(String,"ENCODING")) break;
                }
                GetString(&TmpPtr,String);
                Position = (S32)atoi(String);
                AlePrint((" Position: %s (%u).", String, Position));
                if((Position < 0) || (Position > (NUM_OF_SUPPORTED_CHARS-1)))
                {
                    /* position is -1 for If the character is not in the Adobe Standard Encoding */
                    /* assume no characters within the Position range we want will
                    come after this point (ie characters in Position order) */

                    LOADFONT_Print(("Halting load of bdf font - hit character %u\n", Position));
                    return ST_NO_ERROR; /* accept font at this stage */ /*break*/
                }
                for(;;)
                {
                    GetString(&TmpPtr,String);
                    if(!strcmp(String,"DWIDTH"))
                        break;
                }
                GetString(&TmpPtr,String);
                Font_p->Glyph_p[Position].Width = (S32)atoi(String);
                GetString(&TmpPtr,String);
                Font_p->Glyph_p[Position].Height = (S32)atoi(String);
                for(;;)
                {
                    GetString(&TmpPtr,String);
                    if(!strcmp(String,"BBX"))
                        break;
                }
                GetString(&TmpPtr,String);
                Font_p->Glyph_p[Position].BoundingBoxW = (S32)atoi(String);
                GetString(&TmpPtr,String);
                Font_p->Glyph_p[Position].BoundingBoxH = (S32)atoi(String);
                GetString(&TmpPtr,String);
                Font_p->Glyph_p[Position].BoundingBoxX = (S32)atoi(String);
                GetString(&TmpPtr,String);
                Font_p->Glyph_p[Position].BoundingBoxY = (S32)atoi(String);
                BitmapSize = (S32)(((7+Font_p->Glyph_p[Position].BoundingBoxW)>>3)*
                                   Font_p->Glyph_p[Position].BoundingBoxH);
                if(BitmapSize)
                {
                    Font_p->Glyph_p[Position].GlyphData_p = (U8*)
                                                            memory_allocate(Partition_p, (BitmapSize > 16)?BitmapSize:16);
                    AlePrint(("\nLoadBitmappedFont(): Memory for Glyph allocated."));
                    if (Font_p->Glyph_p[Position].GlyphData_p)
                    {
                        for(;;)
                        {
                            GetString(&TmpPtr,String);
                            if(!strcmp(String,"BITMAP")) break;
                            AlePrint(("\nLoadBitmappedFont(): Looking for BITMAP ..."));
                        }
                        for (j=0; j<Font_p->Glyph_p[Position].BoundingBoxH; j++)
                        {
                            GetString(&TmpPtr,String);
                            StringPos = ((7+Font_p->Glyph_p[Position].BoundingBoxW)>>3);
                            for(k=0; k<StringPos; k++)
                                Font_p->Glyph_p[Position].GlyphData_p[k + StringPos*j]
                                    = CharToHex(String+k*2);
                        }
                    }
                    else
                    {
                        AlePrint(("\nnLoadBitmappedFont(): ST_ERROR_NO_MEMORY"));
                        Err=FreeBitmappedFont(Partition_p, Font_p);
                        return ST_ERROR_NO_MEMORY;
                    }
                }
                else
                {
                    Font_p->Glyph_p[Position].GlyphData_p=NULL;
                }
            }
        }
        else if(!strcmp(String,"ENDFONT"))
        {
            EndFont=TRUE;
            break;
        }
    }

    if (StartFont && EndFont)
    {
        Err = ST_NO_ERROR;
    }
    else
    {
        AlePrint(("\nLoadBitmappedFont(): STGFX_ERROR_INVALID_FONT."));
        Err=FreeBitmappedFont(Partition_p,Font_p);
        Err=STGFX_ERROR_INVALID_FONT;
    }

    return(Err);
}
/******************************************************************************
Function Name : FreeBitmappedFont
  Description : Used to free Bitmapped Font
   Parameters : partition from which to free memory
                pointer to be freed

      Returns : ErrCode
******************************************************************************/
ST_ErrorCode_t FreeBitmappedFont(ST_Partition_t* Partition_p,
                                 STGFX_BitmappedFont_t* Font_p)
{
    S32 i;

    for (i=0; i<Font_p->NumOfGlyphs; i++)
    {
        if(Font_p->Glyph_p[i].GlyphData_p != NULL)
            memory_deallocate(Partition_p, Font_p->Glyph_p[i].GlyphData_p);
    }
    if(Font_p->Glyph_p != NULL)
    {
        memory_deallocate(Partition_p, Font_p->Glyph_p);
        Font_p->Glyph_p = NULL;
    }

    return(ST_NO_ERROR);
}

/******************************************************************************
Function Name : GetString
  Description : Extract the next token from a text string, where tokens are
                delineated by non-graphic characters. Note there is no check
                against overrunning the end of the target buffer
   Parameters : source and destination buffers. The former pointer is updated
                to the first character not copied
******************************************************************************/
static void GetString(U8** bufPtr, char* buffer)
{
    S32    i;
    char   c;

    c = **bufPtr;
    while (!isgraph(c))
    {
        (*bufPtr)++;
        c = **bufPtr;
    }
    for(i = 0; isgraph(c); i++)
    {
        buffer[i] = c;
        (*bufPtr)++;
        c = **bufPtr;
    }
    buffer[i] ='\0';
}

/******************************************************************************
Function Name : GetString
  Description : Convert two hex characters in a string
                to the corresponding U8 value
   Parameters : source buffer
******************************************************************************/
static U8 CharToHex(char* source)
{
    U8 dest = 0;

    if((*source>47)&&(*source<58))               /* number */
        dest=(*source-48)<< 4;
    else if((*source>64)&&(*source<71))          /* letter A-F(upper case) */
        dest=(*source-55)<<4;
    else if((*source>96)&&(*source<103))         /* letter a-f(lower case) */
        dest=(*source-87)<<4;

    if((*(source+1)>47)&&(*(source+1)<58))       /* number */
        dest|=(*(source+1)-48);
    else if((*(source+1)>64)&&(*(source+1)<71))  /* letter A-F(upper case) */
        dest|=(*(source+1)-55);
    else if((*(source+1)>96)&&(*(source+1)<103)) /* letter a-f(lower case) */
        dest |= (*(source+1)-87);

    return dest;
}
/*EOF------------------------------------------------------------------------*/

