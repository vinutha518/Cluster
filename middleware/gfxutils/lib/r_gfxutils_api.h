/*
****************************************************************************
PROJECT : Application test framework
FILE    : $Id: r_gfxutils_api.h 679 2014-04-22 12:39:11Z a5077318 $
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

#ifndef GFXUTILS_API_H_
#define GFXUTILS_API_H_

#ifdef __cplusplus
extern "C" {
#endif
 
/*******************************************************************************
  Title: util.h

  Implementation of the application tests for vdc5 video output (VDCO).
  Exports them to other file.
*/

#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs__ */
#define R_GFXUTILS_ASSERT(arg)                                         \
{        int32_t once = 1;                                        \
    for(;!(arg);){                                                   \
        if(once != 0) {                                                \
            once = 0;                                                   \
            R_BSP_STDIO_Printf("Assertation failed in file %s line %d halting.\n",__FILE__, __LINE__); \
        }                                                               \
    }   /* Wait here forever because of error, if arg == 0  */          \
}
#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs__ */

/*******************************************************************************
  Section: Global types
*/

#if 0
/*******************************************************************************
  Enum: r_vdco_Format_t

  supported pixel formats:

    (code)
    R_VDCO_RGB565      16bit                   rrrrrggggggbbbbb
    R_VDCO_ARGB1555    16bit                   arrrrrgggggbbbbb
    R_VDCO_ARGB4444    16bit                   aaaarrrrggggbbbb                
    R_VDCO_RGB0888     32bit   --------rrrrrrrrggggggggbbbbbbbb
    R_VDCO_ARGB8888    32bit   aaaaaaaarrrrrrrrggggggggbbbbbbbb
    R_VDCO_YUV_YUYV    32bit   vvvvvvvvyyyyyyyyuuuuuuuuyyyyyyyy  2 pixel YUV(4:2:2)
    R_VDCO_YUV_UYVY    32bit   yyyyyyyyvvvvvvvvyyyyyyyyuuuuuuuu  2 pixel YUV(4:2:2)
    R_VDCO_YUV_YVYU    32bit   uuuuuuuuyyyyyyyyvvvvvvvvyyyyyyyy  2 pixel YUV(4:2:2)
    R_VDCO_YUV_VYUY    32bit   yyyyyyyyuuuuuuuuyyyyyyyyvvvvvvvv  2 pixel YUV(4:2:2)
    R_VDCO_RGBA5551    16bit                   rrrrrgggggbbbbba
    R_VDCO_RGBA4444    16bit                   rrrrggggbbbbaaaa
    R_VDCO_RGBA8888    32bit   rrrrrrrrggggggggbbbbbbbbaaaaaaaa
    (end)

  Description:
  See the format descriptions in the document VDCO manual in chapter "Pixel format list" 
  for more details on the pixel formats.

  YUV <-> RGB conversion:
  ITU-R BT.601 conversion 1
  >   |R|   | 1.0 (1024),  0.0               (0),  1.40234375   (1436) |   |(Y-0)|
  >   |G| = | 1.0 (1024), -0.333984375    (-342), -0.713867187  (-731) | * |(U-128)|
  >   |B|   | 1.0 (1024),  1.772460938    (1815),  0.0             (0) |   |(V-128)|

  inverse ITU-R BT.601 conversion 1
  >   | (Y-0) |   | 0.299887,   0.589108,   0.111005|   |R|
  >   |(U-128)| = |-0.169192,  -0.332367,   0.501559| * |G|
  >   |(V-128)|   | 0.499245,  -0.420088,  -0.079157|   |B|


  ITU-R BT.601 conversion 2
  >    |R|   |1.1640625 (1192),  0.0            (0),  1.595703125 (1634)|   |(Y-16)|
  >    |G| = |1.1640625 (1192), -0.39199435  (-401), -0.8125      (-832)| * |(U-128)|
  >    |B|   |1.1640625 (1192),  2.017578125 (2066),  0.0            (0)|   |(V-128)|

  inverse ITU-R BT.601 conversion 2
  >    | (Y-16)|   | 0.256780,   0.504300,   0.097980|   |R|
  >    |(U-128)| = |-0.148152,  -0.290961,   0.439113| * |G|
  >    |(V-128)|   | 0.439363,  -0.367886,  -0.071476|   |B|
*/
#ifndef USE_VDCO
typedef enum {
    R_VDCO_RGB565     =  0 ,
    R_VDCO_RGB0888    =  1 ,
    R_VDCO_ARGB1555   =  2 ,
    R_VDCO_ARGB4444   =  3 ,
    R_VDCO_ARGB8888   =  4 ,
    R_VDCO_CLUT8      =  5 ,
    R_VDCO_CLUT4      =  6 ,
    R_VDCO_CLUT1      =  7 ,
    R_VDCO_YCBCR_422  =  8 ,
    R_VDCO_YCBCR_444  =  9 ,
    R_VDCO_RGBA5551   = 10 ,
    R_VDCO_RGBA8888   = 11 ,
    R_VDCO_YUV_YUYV   = 12 ,
    R_VDCO_YUV_UYVY   = 13 ,
    R_VDCO_YUV_YVYU   = 14 ,
    R_VDCO_YUV_VYUY   = 15 ,
    R_VDCO_RGB666     = 16
} r_vdco_Format_t;
#endif  /* USE_VDCO */
#endif /* 0 */

typedef enum {
    R_GFXUTILS_FORMAT_RGB565      =  0 ,
    R_GFXUTILS_FORMAT_RGB0888     =  1 ,
    R_GFXUTILS_FORMAT_ARGB1555    =  2 ,
    R_GFXUTILS_FORMAT_ARGB4444    =  3 ,
    R_GFXUTILS_FORMAT_ARGB8888    =  4 ,
    R_GFXUTILS_FORMAT_CLUT8       =  5 ,
    R_GFXUTILS_FORMAT_CLUT4       =  6 ,
    R_GFXUTILS_FORMAT_CLUT1       =  7 ,
    R_GFXUTILS_FORMAT_YCBCR_422   =  8 ,
    R_GFXUTILS_FORMAT_YCBCR_444   =  9 ,
    R_GFXUTILS_FORMAT_RGBA5551    = 10 ,
    R_GFXUTILS_FORMAT_RGBA8888    = 11 ,
    R_GFXUTILS_FORMAT_YUV_YUYV    = 12 ,
    R_GFXUTILS_FORMAT_YUV_UYVY    = 13 ,
    R_GFXUTILS_FORMAT_YUV_YVYU    = 14 ,
    R_GFXUTILS_FORMAT_YUV_VYUY    = 15 ,
    R_GFXUTILS_FORMAT_RGB666      = 16 ,
    R_GFXUTILS_FORMAT_UNSUPPORTED = 17
} r_gfxutils_Format_t;


typedef struct {
    uint32_t Base;
    uint32_t Width;
    uint32_t Height;
    uint32_t Stride;
    uint32_t Size;
    uint32_t BPP;
    r_gfxutils_Format_t Format;
} r_gfxutils_Surface_t;

typedef struct {
    uint8_t A;
    uint8_t R;
    uint8_t G;
    uint8_t B;
} r_gfxutils_ARGB_t;

typedef struct {
    uint8_t Y;
    uint8_t U;
    uint8_t V;
} r_gfxutils_YUV_t;

typedef struct {
    int64_t A;
    int64_t R;
    int64_t G;
    int64_t B;
} r_gfxutils_dARGB_t;

typedef struct {
    int64_t Y;
    int64_t U;
    int64_t V;
} r_gfxutils_dYUV_t;


/*******************************************************************************
  Section: Global API functions
*/

void    *R_GFXUTILS_InitAlloc(uint32_t Base, uint32_t MaxSize);
void     R_GFXUTILS_DeleteObject(void *AllocData_p);
void     R_GFXUTILS_DeleteObjects(void *object, ...);
uint32_t R_GFXUTILS_Alloc(void *AllocData_parm, uint32_t Size);
uint32_t R_GFXUTILS_AllocAlign(void *AllocData_parm, uint32_t Size, uint32_t Mask);

r_gfxutils_Surface_t *R_GFXUTILS_CreateSurface(void    *AllocData_parm,
                                               uint32_t Stride,
                                               uint32_t Width,
                                               uint32_t Height,
                                               r_gfxutils_Format_t Format);

uint32_t R_GFXUTILS_AllocSurface(r_cdi_Heap_t        *MemMgr, 
                                uint32_t              Stride,   
                                uint32_t              Width,  
                                uint32_t              Height,  
                                r_gfxutils_Format_t   Format,  
                                r_gfxutils_Surface_t *Surface); 


uint32_t R_GFXUTILS_SetSurface(  uint32_t               Stride,                   
                                 uint32_t               Width,
                                 uint32_t               Height,
                                 r_gfxutils_Format_t    Format, 
                                 r_gfxutils_Surface_t  *Surface);

r_gfxutils_Surface_t *R_GFXUTILS_CreateBuffer(void    *AllocData_parm,
                                                        uint32_t Width,
                                                        uint32_t Height,
                                                        r_gfxutils_Format_t Format); 

uint32_t R_GFXUTILS_AllocBuffer(r_cdi_Heap_t         *MemMgr, 
                                uint32_t              Width,   
                                uint32_t              Height,  
                                r_gfxutils_Format_t   Format,  
                                r_gfxutils_Surface_t *Surface); 




                                                        
void R_GFXUTILS_CreateMonoBar(r_gfxutils_Surface_t *Surface_p,
                              uint32_t barXpos,  uint32_t barYpos,
                              uint32_t barWidth, uint32_t barHeight,
                              r_gfxutils_ARGB_t barColour);
void R_GFXUTILS_CreateVerticalGradient(r_gfxutils_Surface_t *Surface_p, int32_t factor);
void R_GFXUTILS_CreateVerticalGradientMask(r_gfxutils_Surface_t *Surface_p, int32_t factor,
                                           uint32_t Mask);
void R_GFXUTILS_CreateHorizontalGradient(r_gfxutils_Surface_t *Surface_p, int32_t factor);
/*   R_GFXUTILS_CreateVerticalGradient*/

void R_GFXUTILS_CreateCrossRGB (r_gfxutils_Surface_t *Surface_p);
void R_GFXUTILS_CreateCrossRGB_Pattern(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern);
void R_GFXUTILS_CreateCrossRGB_Pattern_Stepsize(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern, uint32_t Stepsize);
void R_GFXUTILS_CreateBorderRGB(r_gfxutils_Surface_t *Surface_p);
void R_GFXUTILS_CreateBorderRGB_Pattern(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern);
void R_GFXUTILS_CreateBorderRGB_Pattern_Stepsize(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern, uint32_t Stepsize);

void R_GFXUTILS_CreateCheckerBoard(r_gfxutils_Surface_t *Surface_p, int32_t tile_width,
                                   int32_t tile_height, r_gfxutils_ARGB_t colour1,
                                   r_gfxutils_ARGB_t colour2 );
void R_GFXUTILS_CreateCheckerBoardHSL(r_gfxutils_Surface_t *Surface_p, int32_t tile_width,
                                      int32_t tile_height );


void R_GFXUTILS_Rgb2yuv_d_i(r_gfxutils_dARGB_t *in,
                            r_gfxutils_YUV_t *out);
void R_GFXUTILS_Rgb2yuv_i_i(r_gfxutils_ARGB_t *in, 
                            r_gfxutils_YUV_t *out);
void R_GFXUTILS_Rgb2yuv_i_d(r_gfxutils_ARGB_t *in,
                            r_gfxutils_dYUV_t *out);
void R_GFXUTILS_Rgb2yuv_d_d(r_gfxutils_dARGB_t *in, 
                            r_gfxutils_dYUV_t *out);


void R_GFXUTILS_SurfaceFill(r_gfxutils_Surface_t *Surface_p,
                            uint32_t xOffs,  uint32_t yOffs,
                            r_gfxutils_ARGB_t Colour);
                            
void R_GFXUTILS_SurfaceFillS(r_gfxutils_Surface_t *Surface_p, 
                            uint32_t xOffs,  uint32_t yOffs, uint32_t width, uint32_t height,
                            r_gfxutils_ARGB_t Colour);

void R_GFXUTILS_RemoveAlpha(r_gfxutils_Surface_t *Surface_p, uint8_t alphaValue);

void R_GFXUTILS_BlitCopy(r_gfxutils_Surface_t *SrcSurface_p,
                            r_gfxutils_Surface_t *DstSurface_p,
                            uint32_t xOffs,  uint32_t yOffs);

void R_GFXUTILS_BlitBlend(r_gfxutils_Surface_t *SrcSurface_p,
                            r_gfxutils_Surface_t *DstSurface_p,
                            uint32_t xOffs,  uint32_t yOffs);

void R_GFXUTILS_BlitBlendA(r_gfxutils_Surface_t *SrcSurface_p,
                            r_gfxutils_Surface_t *DstSurface_p,
                            uint32_t xOffs,  uint32_t yOffs, uint32_t alpha);

void R_GFXUTILS_CreateDither(r_gfxutils_Surface_t *Surface_p, int32_t factor);

void R_GFXUTILS_CreateVertical(r_gfxutils_Surface_t *Surface_p, r_gfxutils_ARGB_t standard);

void R_GFXUTILS_SwapBits(r_gfxutils_Surface_t *Surface_p, uint32_t bitsToStart, uint32_t noOfBits);
void R_GFXUTILS_CutBits(r_gfxutils_Surface_t *Surface_p, uint32_t bitsToStart, uint32_t noOfBits, uint32_t mask);

/* Codepage 850 Bitmap Font. */
/* Flags:
    1: Transparency. Do not write background colour.
    2: Linebreak.    Process Linebreaks in the string.
    4: Autobreak.    Automatically break when reaching the end of the screen. No word wrap.
 */
void R_GFXUTILS_WriteString(r_gfxutils_Surface_t *Surface_p, 
                            uint32_t xOffs, uint32_t yOffs, 
                            uint8_t *string, uint32_t Flags);
#ifdef __cplusplus
}
#endif
#endif /* GFXUTILS_API_H_ */
