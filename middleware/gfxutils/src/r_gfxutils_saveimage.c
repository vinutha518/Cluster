/*
****************************************************************************
PROJECT : Application test framework
FILE    : $Id: r_util.c 1083 2013-07-15 15:50:54Z sven.grundmann $
============================================================================ 
DESCRIPTION
API&main for the framework
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2013
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

/*******************************************************************************
  Title: util.c

  Implementation some helper functions for the vdc test functions.
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_bsp_stdio_api.h"

#include "r_cdi_api.h"      /* R_CDI_HeapAlloc, R_CDI_HeapFree */
#include "stdlib.h"         /* malloc, free */

#ifdef USE_VDCO
 #include "r_ddb_api.h"     /* Required by "r_vdco_api.h" */
 #include "r_vdco_api.h"
#endif  /* USE_VDCO */

#include "r_gfxutils_api.h"
#include "r_gfxutils_saveimage.h"

#include <stdio.h>
#include <string.h>

#ifdef USE_GFXUTILS_PNG
#include "lodepng.h"
#endif /* USE_GFXUTILS_PNG */

/*******************************************************************************
  Section: LocalTypes
*/

#ifdef USE_GFXUTILS_PNG
typedef enum PNGAllocatorType
{
  PNG_ALLOC_DEFAULT = 0, 
  PNG_ALLOC_CDI = 1, 
  PNG_ALLOC_GFXUTILS = 2, 
} PNGAllocatorType_t;

PNGAllocatorType_t  PNGAllocator = PNG_ALLOC_DEFAULT;
void *              PNGAllocator_Gfxutils_Heap = 0;
r_cdi_Heap_t *      PNGAllocator_Cdi_Heap = 0;
#endif /* USE_GFXUTILS_PNG */


/*******************************************************************************
  Section: Local Functions
*/


/* list of functions */
void GFXUTILS_ReadPixel(uint32_t Address, r_gfxutils_ARGB_t *Colour, r_gfxutils_Format_t format);

#ifdef USE_GFXUTILS_PNG
void* lodepng_malloc(size_t size);
void* lodepng_realloc(void* ptr, size_t new_size);
void lodepng_free(void* ptr);


void* lodepng_malloc(size_t size)
{
    void * address = 0;
    
    switch (PNGAllocator)
    {
    case PNG_ALLOC_DEFAULT:
        address = malloc(size);
        break;
        
    case PNG_ALLOC_CDI:
        address = (void*)R_CDI_Alloc(PNGAllocator_Cdi_Heap, size);
        break;
        
    case PNG_ALLOC_GFXUTILS:
        address = (void*)R_GFXUTILS_Alloc(PNGAllocator_Gfxutils_Heap, size);
        break;
        
    default:
        address = 0;
        break;
    }
    
    return address;
}

void* lodepng_realloc(void* ptr, size_t new_size)
{
    void * address = 0;
    
    switch (PNGAllocator)
    {
    case PNG_ALLOC_DEFAULT:
        address = realloc(ptr, new_size);
        break;

    case PNG_ALLOC_CDI:
        /* Get new memory area */
        address = (void*)R_CDI_Alloc(PNGAllocator_Cdi_Heap, new_size);
        
        /* In case that ptr is a null pointer, the function behaves like malloc, 
        assigning a new block of size bytes and returning a pointer to its beginning. */
        if (0 != ptr)
        {
            /* Copy from old to new - As we don't know the size of the source, 
               we'll just copy as much as is fitting into the newly allocated memory */
            memcpy(address, ptr, new_size);
            
            /* Release old area */
            R_CDI_Free((uint32_t)ptr, PNGAllocator_Cdi_Heap);
        }
        break;

    case PNG_ALLOC_GFXUTILS:
        /* Get new memory area */
        address = (void*)R_GFXUTILS_Alloc(PNGAllocator_Gfxutils_Heap, new_size);

        /* In case that ptr is a null pointer, the function behaves like malloc, 
        assigning a new block of size bytes and returning a pointer to its beginning. */
        if (0 != ptr)
        {
            /* Copy from old to new - As we don't know the size of the source, 
               we'll just copy as much as is fitting into the newly allocated memory */
            memcpy(address, ptr, new_size);
        
            /* Release old area */
            /* Not supported by GFXUTILS */
        }
        break;
        
    default:
        address = 0;
        break;
    }
    
    return address;
}

void lodepng_free(void* ptr)
{
    switch (PNGAllocator)
    {
    case PNG_ALLOC_DEFAULT:
        free(ptr);
        break;

    case PNG_ALLOC_CDI:
        /* Release area */
        R_CDI_Free((uint32_t)ptr, PNGAllocator_Cdi_Heap);
        break;

    case PNG_ALLOC_GFXUTILS:
        /* Release area */
        /* Not supported by GFXUTILS */
        break;
        
    default:
        break;
    }
}
#endif /* USE_GFXUTILS_PNG */


/*******************************************************************************
  Section: Global Functions
*/

#define BITMAP_SUPPORT_ALPHA_CHANNEL    (1u)
#define BITMAP_WRITE_BUFFER_SIZE        (1024u)
int32_t R_GFXUTILS_WriteBitmapFromSurface(char *Name,
                                           r_gfxutils_Surface_t  *Surface)
{
    const uint32_t biSize     = 0x02;
    const uint32_t biWidth    = 0x12;/* - 0x0E; */
    const uint32_t biHeight   = 0x16;/* - 0x0E; */
    const uint32_t biBitCount = 0x1C;/* - 0x0E; */
    const uint32_t biImgSize  = 0x22;/* - 0x0E; */
    
    const uint32_t headerSize = 14u + 40u;
    
    const uint32_t write_buffer_threshold = BITMAP_WRITE_BUFFER_SIZE-4u;
    
    uint8_t fileheader[14+40] = {
        0x42, 0x4D,             /* Magic Number*/    
        0x4C, 0x00, 0x00, 0x00, /* Bitmap Size */
        0x00, 0x00, 0x00, 0x00, /* Reserved */
        0x36, 0x00, 0x00, 0x00, /* Offset of Image Data */
    #if 0 /* Combine arrays to a single header block */
    };
    uint8_t infoheader[40] = {
    #endif
        0x28, 0x00, 0x00, 0x00, /* Size of InfoHeader */
        0x00, 0x00, 0x00, 0x00, /* Width */
        0x00, 0x00, 0x00, 0x00, /* Height */
        0x01, 0x00,             /* Planes */
        0x18, 0x00,             /* BitCount */
        0x00, 0x00, 0x00, 0x00, /* Compression */
        0x00, 0x00, 0x00, 0x00, /* Bitmap Size, may be zero in uncompressed mode */
        0x00, 0x00, 0x00, 0x00, /* XPixelsPerMeter */
        0x00, 0x00, 0x00, 0x00, /* YPixelsPerMeter */
        0x00, 0x00, 0x00, 0x00, /* Colour Table Entries */
        0x00, 0x00, 0x00, 0x00, /* Colours used in Image, may be zero */
    };
    
    uint32_t            bitmap_size  = 0;
    FILE               *bitmap_file  = 0;

    uint32_t            read_pos  = 0;
    uint32_t            write_pos = 0;
    uint8_t             write_buffer[BITMAP_WRITE_BUFFER_SIZE];
    
    r_gfxutils_ARGB_t   pixel;
    uint32_t            bytesPerPixel, bytesPerLine;
    int32_t             x, y;
    
    float32_t           progress_max  = 20.0f;
    float32_t           progress_now  = 0.0f;
    uint32_t            progress_last = 0;
    
    /* Check variables */
    if ((0 == Name) || (0 == Surface))
    {
        return -1;
    }
    
    /* Open file on host system */
    bitmap_file = fopen(Name, "w");
    if (0 == bitmap_file)
    {
        return -1;
    }
    
/* ### PREPARE HEADERS ### */
    
#if BITMAP_SUPPORT_ALPHA_CHANNEL
    if (32 == Surface->BPP)
    {
        bytesPerPixel = 4;  /* 32BPP */
        fileheader[biBitCount] = 32;
    }
    else
#endif
    {
        bytesPerPixel = 3; /* 24BPP */
        fileheader[biBitCount] = 24;
    }
    
    /* Bitmap Size including header */
    bitmap_size = ((Surface->Width * Surface->Height) * bytesPerPixel) + 54;
    fileheader[biSize+0u] = (bitmap_size>> 0u)&0xFFu; 
    fileheader[biSize+1u] = (bitmap_size>> 8u)&0xFFu; 
    fileheader[biSize+2u] = (bitmap_size>>16u)&0xFFu;
    fileheader[biSize+3u] = (bitmap_size>>24u)&0xFFu;
    
    fileheader[biWidth+0u] = (Surface->Width>> 0u)&0xFFu;
    fileheader[biWidth+1u] = (Surface->Width>> 8u)&0xFFu;
    fileheader[biWidth+2u] = (Surface->Width>>16u)&0xFFu;
    fileheader[biWidth+3u] = (Surface->Width>>24u)&0xFFu;
    
    fileheader[biHeight+0u] = (Surface->Height>> 0u)&0xFFu;
    fileheader[biHeight+1u] = (Surface->Height>> 8u)&0xFFu;
    fileheader[biHeight+2u] = (Surface->Height>>16u)&0xFFu;
    fileheader[biHeight+3u] = (Surface->Height>>24u)&0xFFu;
    
    /* Bitmap Size excluding header */
    bitmap_size = Surface->Width * Surface->Height * bytesPerPixel;
    fileheader[biImgSize+0u] = (bitmap_size>> 0u)&0xFFu; 
    fileheader[biImgSize+1u] = (bitmap_size>> 8u)&0xFFu; 
    fileheader[biImgSize+2u] = (bitmap_size>>16u)&0xFFu;
    fileheader[biImgSize+3u] = (bitmap_size>>24u)&0xFFu;

/* ### WRITE BITMAP ### */
    
    /* Progress Info */
    R_BSP_STDIO_Printf("Saving BMP: \"%s\"\n0%% ------------ 100%%\n", Name);
    
    /* Fileheader */
    read_pos = 0;
    while (read_pos < headerSize)
    {
        write_buffer[write_pos] = fileheader[read_pos];
        write_pos++;
        read_pos++;
    }

    /* Image Data */
    bytesPerPixel = Surface->BPP/8;
    
    /* Iterate of all lines from bottom to top */
    for (y=Surface->Height-1; y>=0; y--)
    {
        /* Iterate over a single line */
        for (x=0; x<Surface->Width; x++)
        {
            read_pos = (Surface->Base + ((y * Surface->Stride) * bytesPerPixel)) + (x * bytesPerPixel);
            GFXUTILS_ReadPixel(read_pos, &pixel, Surface->Format);
            write_buffer[write_pos] = pixel.B;
            write_pos++;
            write_buffer[write_pos] = pixel.G;
            write_pos++;
            write_buffer[write_pos] = pixel.R;
            write_pos++;
#if BITMAP_SUPPORT_ALPHA_CHANNEL
            if (4 == bytesPerPixel)
            {
                write_buffer[write_pos] = pixel.A;
                write_pos++;
            }
#endif
            
            if (write_buffer_threshold <= write_pos)
            {
                fwrite(write_buffer, 1, write_pos, bitmap_file);
                write_pos = 0;
            }
        }
        
        /* The number of bytes per line must be a multiple of four */
        bytesPerLine = x * bytesPerPixel;
        while (0 != (bytesPerLine & 0x3u))
        {
            write_buffer[write_pos] = 0;
            write_pos++;
            bytesPerLine++;
        }
        
        /* Progress Info */
        progress_now = ((float32_t)(Surface->Height-y)/Surface->Height) * progress_max;
        if ( progress_now > progress_last)
        {
            R_BSP_STDIO_Printf(".");
            progress_last++;
        }
        
    }
    write_buffer[write_pos] = 0;
    write_pos++;
    write_buffer[write_pos] = 0;
    write_pos++;

    fwrite(write_buffer, 1, write_pos, bitmap_file);
    fclose(bitmap_file);
    
    R_BSP_STDIO_Printf("\n");
   
    return 0;
}


#ifdef USE_GFXUTILS_PNG

uint32_t R_GFXUTILS_WritePNGFromSurface(char *Name,
                                           r_gfxutils_Surface_t  *Surface)
{
    uint32_t error;
    error = lodepng_encode_file(Name, (uint8_t*)Surface->Base, Surface->Stride, Surface->Height, LCT_RGBA, 8);
    R_BSP_STDIO_Printf("Error %02u: %s", error, lodepng_error_text(error));
    
    return error;
}

void R_GFXUTILS_SetPNGAllocator_Default(void)
{
    PNGAllocator = PNG_ALLOC_DEFAULT;
}

void R_GFXUTILS_SetPNGAllocator_GFXUTILS(void * Gfxutils_Heap)
{
    PNGAllocator = PNG_ALLOC_GFXUTILS;
    PNGAllocator_Gfxutils_Heap = Gfxutils_Heap;
}

void R_GFXUTILS_SetPNGAllocator_CDI(r_cdi_Heap_t * Cdi_Heap)
{
    PNGAllocator = PNG_ALLOC_CDI;
    PNGAllocator_Cdi_Heap = Cdi_Heap;
}

#endif /* USE_GFXUTILS_PNG */



