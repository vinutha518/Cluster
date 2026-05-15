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

#include "r_ddb_api.h"     /* Required by VDCO, VDCE and WM */

#include "r_bsp_stdio_api.h"

#include "r_cdi_api.h"      /* R_CDI_HeapAlloc, R_CDI_HeapFree */
#include "stdlib.h"         /* malloc, free */

#ifdef USE_VDCO
  #include "r_vdco_api.h"
  #define R_GFXUTILS_CONVERT_USE_VDCO
#endif  /* USE_VDCO */
#ifdef USE_VDCE
  #include "r_vdce_api.h"
  #define R_GFXUTILS_CONVERT_USE_VDCE
#endif  /* USE_VDCE */
#ifdef USE_WM
  #include "r_wm_api.h"
  #define R_GFXUTILS_CONVERT_USE_WM
#endif

#include "r_gfxutils_api.h"
#include "r_gfxutils_bitmapfont6x8.h"
#include "r_gfxutils_convert.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>


/*******************************************************************************
  Section: LocalTypes
*/

/*******************************************************************************
  typedef: r_gfxutils_AllocData_t

  Description:
  Struct holding the data for memory allocation. It is returned from ResetAlloc and can be Reset again by
  giving it to ResetAlloc function again.

  R/G/B -    The individual color components of the color.
*/

typedef struct {
    uint32_t Base;
    uint32_t Current;
    uint32_t Max;
} r_gfxutils_AllocData_t;

typedef union {
    uint8_t byte;
    struct { 
        uint8_t b0:4;
        uint8_t b1:4;
    } clut4;
    struct {
        uint8_t b0:1;
        uint8_t b1:1;
        uint8_t b2:1;
        uint8_t b3:1;
        uint8_t b4:1;
        uint8_t b5:1;
        uint8_t b6:1;
        uint8_t b7:1;
    } clut1;
} r_gfxutils_CLUT_t;

/*******************************************************************************
  Section: Local Functions
*/

/* list of functions */
void GFXUTILS_WritePixel(uint32_t Address, r_gfxutils_ARGB_t Colour, r_gfxutils_Format_t format);
void GFXUTILS_WritePixelNew(uint32_t x, uint32_t y, r_gfxutils_Surface_t *Surface_p, r_gfxutils_ARGB_t Colour);
void GFXUTILS_ReadPixel(uint32_t Address, r_gfxutils_ARGB_t *Colour, r_gfxutils_Format_t format);
static uint32_t loc_Random( void );
static uint32_t loc_SwapBits(uint32_t no, uint32_t noOfBits);
static r_gfxutils_ARGB_t loc_HSL2RGB(float32_t h, float32_t sl, float32_t l);



void GFXUTILS_WritePixel(uint32_t Address, r_gfxutils_ARGB_t Colour, r_gfxutils_Format_t format)
{
    uint32_t value_32;
    uint16_t value_16;
    uint8_t value_8;
    r_gfxutils_ARGB_t Colour16;
    
    r_gfxutils_YUV_t ColourYUV;
    uint32_t aligned_address;
    switch (format) 
    {
    case R_GFXUTILS_FORMAT_ARGB8888:
    case R_GFXUTILS_FORMAT_RGB0888:
        value_32 = ((uint32_t)Colour.A << 24) + ((uint32_t)Colour.R << 16) + ((uint32_t)Colour.G << 8) + (uint32_t)Colour.B;
        *((uint32_t *)Address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_RGBA8888:
        value_32 = ((uint32_t)Colour.R << 24) + ((uint32_t)Colour.G << 16) + ((uint32_t)Colour.B << 8) + (uint32_t)Colour.A;
        *((uint32_t *)Address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_RGB565:
        Colour16.R = Colour.R >>3;
        Colour16.G = Colour.G >>2;
        Colour16.B = Colour.B >>3;
        value_16 = (uint16_t)((uint16_t)Colour16.R << 11) + (uint16_t)((uint16_t)Colour16.G << 5) + Colour16.B;
        *((uint16_t *)Address)=value_16;
        break;
    case R_GFXUTILS_FORMAT_RGB666:
        Colour16.R = Colour.R >>2;
        Colour16.G = Colour.G >>2;
        Colour16.B = Colour.B >>2;
        value_32 = (uint32_t)((uint32_t)Colour16.R << 12) + (uint16_t)((uint16_t)Colour16.G << 6) + Colour16.B;
        *((uint32_t *)Address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_CLUT8:
    case R_GFXUTILS_FORMAT_CLUT4:
    case R_GFXUTILS_FORMAT_CLUT1:
        value_8 = Colour.R;
        *((uint8_t *)Address)=value_8;
        break;

    case R_GFXUTILS_FORMAT_YCBCR_444:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);        
        value_32 = ((uint32_t)ColourYUV.V << 16) + ((uint32_t)ColourYUV.Y << 8) + ColourYUV.U;
        *((uint32_t *)Address)=value_32;
        break;

    case R_GFXUTILS_FORMAT_YCBCR_422: /* For all 422 formats write always 2 pixels at the same time. This is
                              not correct but this is only a function for testing and in this
                              context it is ok. The address has to be aligned to 32 bit address
                              then. */
    case R_GFXUTILS_FORMAT_YUV_YUYV:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
#if 1
        value_32 = ((uint32_t)ColourYUV.V << 24) + ((uint32_t)ColourYUV.Y << 16) + ((uint32_t)ColourYUV.U << 8) + ColourYUV.Y;
#else
        Colour.A = Colour.R = Colour.G = Colour.B = 0;
        R_GFXUTILS_Yuv2rgb_i_i(&Colour,&ColourYUV);
#endif
        aligned_address = Address & (~0x03uL); /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;        
    case R_GFXUTILS_FORMAT_YUV_UYVY:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
        value_32 = ((uint32_t)ColourYUV.Y << 24) + ((uint32_t)ColourYUV.V << 16) + ((uint32_t)ColourYUV.Y << 8) + ColourYUV.U;
        aligned_address = Address & ~0x03uL; /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_YUV_YVYU:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
        value_32 = ((uint32_t)ColourYUV.U << 24) + ((uint32_t)ColourYUV.Y << 16) + ((uint32_t)ColourYUV.V << 8) + ColourYUV.Y;
        aligned_address = Address & ~0x3uL; /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_YUV_VYUY:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
        value_32 = ((uint32_t)ColourYUV.Y << 24) + ((uint32_t)ColourYUV.U << 16) + ((uint32_t)ColourYUV.Y << 8) + ColourYUV.V;
        aligned_address = Address & ~0x3uL; /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_ARGB1555:
        Colour16.A = Colour.A >>7;
        Colour16.R = Colour.R >>3;
        Colour16.G = Colour.G >>3;
        Colour16.B = Colour.B >>3;
        value_16 = (uint16_t)((uint16_t)Colour16.A << 15) + (uint16_t)((uint16_t)Colour16.R << 10) + (uint16_t)((uint16_t)Colour16.G << 5) + Colour16.B;
        *((uint16_t *)Address)=value_16;
        break;
    case R_GFXUTILS_FORMAT_ARGB4444:
        Colour16.A = Colour.A >>4;
        Colour16.R = Colour.R >>4;
        Colour16.G = Colour.G >>4;
        Colour16.B = Colour.B >>4;
        value_16 = (uint16_t)((uint16_t)Colour16.A << 12) + (uint16_t)((uint16_t)Colour16.R << 8) + (uint16_t)((uint16_t)Colour16.G << 4) + Colour16.B;
        *((uint16_t *)Address)=value_16;
        break;
    case R_GFXUTILS_FORMAT_RGBA5551:
        Colour16.A = Colour.A >>7;
        Colour16.R = Colour.R >>3;
        Colour16.G = Colour.G >>3;
        Colour16.B = Colour.B >>3;
        value_16 = (uint16_t)((uint16_t)Colour16.R << 11) + (uint16_t)((uint16_t)Colour16.G << 6) + (uint16_t)((uint16_t)Colour16.B << 1) + Colour16.A;
        *((uint16_t *)Address)=value_16;
        break;
    default:
        /* here format is never R_GFXUTILS_FORMAT_ARGB8888, so the below code is equivalent to 
         R_GFXUTILS_ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(format == R_GFXUTILS_FORMAT_ARGB8888);
        break; 
    }
}

/*
    @TODO MNI: Use <GFXUTILS_WritePixelNew> for all functions and remove <GFXUTILS_WritePixel>
    
    Functions with "x" are already verified to work with <GFXUTILS_WritePixelNew>.
    
    x   R_GFXUTILS_WriteString
        R_GFXUTILS_CreateDither
        R_GFXUTILS_BlitBlendA
        R_GFXUTILS_BlitCopy
        R_GFXUTILS_RemoveAlpha
    x   R_GFXUTILS_SurfaceFillS
    x   R_GFXUTILS_CreateBorderRGB_Pattern_Stepsize
    x   R_GFXUTILS_CreateCrossRGB_Pattern_Stepsize
    x   R_GFXUTILS_CreateCheckerBoardHSL
        R_GFXUTILS_CreateCheckerBoard
    x   R_GFXUTILS_CreateVerticalGradientMask
        R_GFXUTILS_CreateHorizontalGradient
        R_GFXUTILS_CreateMonoBar

    For untested functions, the call to the old <GFXUTILS_WritePixel> still exists, but is commented out.
*/

void GFXUTILS_WritePixelNew(uint32_t x, uint32_t y, r_gfxutils_Surface_t *Surface_p, r_gfxutils_ARGB_t Colour)
{
    uint32_t value_32;
    uint16_t value_16;
    uint8_t value_8;
    r_gfxutils_ARGB_t Colour16;
    r_gfxutils_CLUT_t Clut8;
    
    r_gfxutils_YUV_t ColourYUV;
    uint32_t aligned_address;
    uint32_t pix_pos_word;
    
    uint32_t byte_Stride  = (Surface_p->Stride * Surface_p->BPP) / 8;
    uint32_t line_Address = Surface_p->Base + (y * byte_Stride);
    uint32_t /*pixel_*/Address = line_Address + ((x * Surface_p->BPP) / 8);
    
    /* For color formats that use less than <word_width>, we need to do Read-Modify-Write operations. */
    /* Smallest access size in bits, we want to handle with GFXUTILS */
    const uint32_t word_width = 8;
    /* Number of pixels that fit in a word with <word_width> bits */
    uint32_t pix_per_word = word_width / Surface_p->BPP;
    uint8_t pix_mask = (uint8_t)(1u << Surface_p->BPP) - 1u;

    /* Index of the current pixel in the word (Reversed due to endianness) */
    if(pix_per_word != 0)
    {
        pix_pos_word = (pix_per_word - (x % pix_per_word)) - 1;
    }
    else
    {
        pix_pos_word = 0;
    }
    /* Mask for RMW operation */
    

    switch (Surface_p->Format) 
    {
    case R_GFXUTILS_FORMAT_ARGB8888:
    case R_GFXUTILS_FORMAT_RGB0888:
        value_32 = ((uint32_t)Colour.A << 24) + ((uint32_t)Colour.R << 16) + ((uint32_t)Colour.G << 8) + (uint32_t)Colour.B;
        *((uint32_t *)Address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_RGBA8888:
        value_32 = ((uint32_t)Colour.R << 24) + ((uint32_t)Colour.G << 16) + ((uint32_t)Colour.B << 8) + (uint32_t)Colour.A;
        *((uint32_t *)Address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_RGB565:
        Colour16.R = Colour.R >>3;
        Colour16.G = Colour.G >>2;
        Colour16.B = Colour.B >>3;
        value_16 = (uint16_t)((uint16_t)Colour16.R << 11) + (uint16_t)((uint16_t)Colour16.G << 5) + Colour16.B;
        *((uint16_t *)Address)=value_16;
        break;
    case R_GFXUTILS_FORMAT_RGB666:
        Colour16.R = Colour.R >>2;
        Colour16.G = Colour.G >>2;
        Colour16.B = Colour.B >>2;
        value_32 = (uint32_t)((uint32_t)Colour16.R << 12) + (uint16_t)((uint16_t)Colour16.G << 6) + Colour16.B;
        *((uint32_t *)Address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_CLUT8:
        value_8 = Colour.R;
        *((uint8_t *)Address)=value_8;
        break;
        
    case R_GFXUTILS_FORMAT_CLUT4:
        /* Use the MSB as color bits and mask the rest */
        Colour.R = (Colour.R>>(word_width-Surface_p->BPP))&pix_mask;
        /* Read current value from RAM */
        Clut8.byte = *((uint8_t *)Address);
        
        /* Works, too, but switch() is easier to debug */
        #if 0
        /* Mask parts that we want to write now, keep the rest intact */
        Clut8.byte &= ~(pix_mask<<(Surface_p->BPP*pix_pos_word));
        /* Add our part to the existing value */
        Clut8.byte |= Colour.R<<(Surface_p->BPP*pix_pos_word);
        #endif

        switch (pix_pos_word)
        {
        case 0:  Clut8.clut4.b0 = Colour.R; break;
        case 1:  Clut8.clut4.b1 = Colour.R; break;
        default: R_GFXUTILS_ASSERT(2 > pix_pos_word); break;
        }
        
        /* Write it back */
        *((uint8_t *)Address)=Clut8.byte;
        break;
        
    case R_GFXUTILS_FORMAT_CLUT1:
        /* Use the MSB as color bits and mask the rest */
        Colour.R = (Colour.R>>(word_width-Surface_p->BPP))&pix_mask;
        /* Read current value from RAM */
        Clut8.byte = *((uint8_t *)Address);

        /* Works, too, but switch() is easier to debug */
        #if 0
        /* Mask parts that we want to write now, keep the rest intact */
        Clut8.byte &= ~(pix_mask<<(Surface_p->BPP*pix_pos_word));
        /* Add our part to the existing value */
        Clut8.byte |= Colour.R<<(Surface_p->BPP*pix_pos_word);
        #endif
        
        switch (pix_pos_word)
        {
        case 0:  Clut8.clut1.b0 = Colour.R; break;
        case 1:  Clut8.clut1.b1 = Colour.R; break;
        case 2:  Clut8.clut1.b2 = Colour.R; break;
        case 3:  Clut8.clut1.b3 = Colour.R; break;
        case 4:  Clut8.clut1.b4 = Colour.R; break;
        case 5:  Clut8.clut1.b5 = Colour.R; break;
        case 6:  Clut8.clut1.b6 = Colour.R; break;
        case 7:  Clut8.clut1.b7 = Colour.R; break;
        default: R_GFXUTILS_ASSERT(8 > pix_pos_word); break;
        }

        /* Write it back */
        *((uint8_t *)Address)=Clut8.byte;
        break;

    case R_GFXUTILS_FORMAT_YCBCR_444:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);        
        value_32 = ((uint32_t)ColourYUV.V << 16) + ((uint32_t)ColourYUV.Y << 8) + ColourYUV.U;
        *((uint32_t *)Address)=value_32;
        break;

    case R_GFXUTILS_FORMAT_YCBCR_422: /* For all 422 formats write always 2 pixels at the same time. This is
                              not correct but this is only a function for testing and in this
                              context it is ok. The address has to be aligned to 32 bit address
                              then. */
    case R_GFXUTILS_FORMAT_YUV_YUYV:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
#if 1
        value_32 = ((uint32_t)ColourYUV.V << 24) + ((uint32_t)ColourYUV.Y << 16) + ((uint32_t)ColourYUV.U << 8) + ColourYUV.Y;
#else
        Colour.A = Colour.R = Colour.G = Colour.B = 0;
        R_GFXUTILS_Yuv2rgb_i_i(&Colour,&ColourYUV);
#endif
        aligned_address = Address & (~0x03uL); /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;        
    case R_GFXUTILS_FORMAT_YUV_UYVY:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
        value_32 = ((uint32_t)ColourYUV.Y << 24) + ((uint32_t)ColourYUV.V << 16) + ((uint32_t)ColourYUV.Y << 8) + ColourYUV.U;
        aligned_address = Address & ~0x03uL; /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_YUV_YVYU:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
        value_32 = ((uint32_t)ColourYUV.U << 24) + ((uint32_t)ColourYUV.Y << 16) + ((uint32_t)ColourYUV.V << 8) + ColourYUV.Y;
        aligned_address = Address & ~0x3uL; /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_YUV_VYUY:
        R_GFXUTILS_Rgb2yuv_i_i(&Colour,&ColourYUV);
        value_32 = ((uint32_t)ColourYUV.Y << 24) + ((uint32_t)ColourYUV.U << 16) + ((uint32_t)ColourYUV.Y << 8) + ColourYUV.V;
        aligned_address = Address & ~0x3uL; /* align tp 32 bit */
        *((uint32_t *)aligned_address)=value_32;
        break;
    case R_GFXUTILS_FORMAT_ARGB1555:
        Colour16.A = Colour.A >>7;
        Colour16.R = Colour.R >>3;
        Colour16.G = Colour.G >>3;
        Colour16.B = Colour.B >>3;
        value_16 = (uint16_t)((uint16_t)Colour16.A << 15) + (uint16_t)((uint16_t)Colour16.R << 10) + (uint16_t)((uint16_t)Colour16.G << 5) + Colour16.B;
        *((uint16_t *)Address)=value_16;
        break;
    case R_GFXUTILS_FORMAT_ARGB4444:
        Colour16.A = Colour.A >>4;
        Colour16.R = Colour.R >>4;
        Colour16.G = Colour.G >>4;
        Colour16.B = Colour.B >>4;
        value_16 = (uint16_t)((uint16_t)Colour16.A << 12) + (uint16_t)((uint16_t)Colour16.R << 8) + (uint16_t)((uint16_t)Colour16.G << 4) + Colour16.B;
        *((uint16_t *)Address)=value_16;
        break;
    case R_GFXUTILS_FORMAT_RGBA5551:
        Colour16.A = Colour.A >>7;
        Colour16.R = Colour.R >>3;
        Colour16.G = Colour.G >>3;
        Colour16.B = Colour.B >>3;
        value_16 = (uint16_t)((uint16_t)Colour16.R << 11) + (uint16_t)((uint16_t)Colour16.G << 6) + (uint16_t)((uint16_t)Colour16.B << 1) + Colour16.A;
        *((uint16_t *)Address)=value_16;
        break;

    default:
        /* here format is never R_GFXUTILS_FORMAT_ARGB8888, so the below code is equivalent to 
         R_GFXUTILS_ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(Surface_p->Format == R_GFXUTILS_FORMAT_ARGB8888);
        break; 
    }
}


void GFXUTILS_ReadPixel(uint32_t Address, r_gfxutils_ARGB_t *Colour, r_gfxutils_Format_t format)
{
    uint32_t value_32;
    uint16_t value_16;
    /*uint8_t value_8;*/
    
    switch (format)
    {
    case R_GFXUTILS_FORMAT_ARGB8888:
        value_32 = *((uint32_t *)Address);
        Colour->A = ((value_32 & 0xFF000000u) >> 24);
        Colour->R = ((value_32 & 0x00FF0000u) >> 16);
        Colour->G = ((value_32 & 0x0000FF00u) >>  8);
        Colour->B = ((value_32 & 0x000000FFu) >>  0);
        break;
    case R_GFXUTILS_FORMAT_RGB0888:
        value_32 = *((uint32_t *)Address);
        Colour->A = 0xFF;
        Colour->R = ((value_32 & 0x00FF0000u) >> 16);
        Colour->G = ((value_32 & 0x0000FF00u) >>  8);
        Colour->B = ((value_32 & 0x000000FFu) >>  0);
        break;
    case R_GFXUTILS_FORMAT_RGBA8888:
        value_32 = *((uint32_t *)Address);
        Colour->R = ((value_32 & 0xFF000000u) >> 24);
        Colour->G = ((value_32 & 0x00FF0000u) >> 16);
        Colour->B = ((value_32 & 0x0000FF00u) >>  8);
        Colour->A = ((value_32 & 0x000000FFu) >>  0);
        break;
    case R_GFXUTILS_FORMAT_RGB565:
        value_16 = *((uint16_t *)Address);
        Colour->R = (((value_16 & 0xF800u) >> 11) | 0x03u);
        Colour->G = (((value_16 & 0x07E0u) >>  5) | 0x03u);
        Colour->B = ((uint16_t)((value_16 & 0x001Fu) <<  3) | 0x03u);
        break;
    case R_GFXUTILS_FORMAT_RGB666:
        value_32 = *((uint32_t *)Address);
        Colour->R = (((value_32 & 0x3F000u) >> 10) | 0x03u);
        Colour->G = (((value_32 & 0x00FC0u) >>  4) | 0x03u);
        Colour->B = ((uint32_t)((value_32 & 0x0003Fu) <<  2) | 0x03u);
        break;
    case R_GFXUTILS_FORMAT_ARGB4444:
        value_16 = *((uint16_t *)Address);
        Colour->A = ((value_16 & 0xF000u) >> 8u) + ((value_16 & 0xF000u) >> 12u);
        Colour->R = ((value_16 & 0x0F00u) >> 4u) + ((value_16 & 0x0F00u) >>  8u);
        Colour->G = ((value_16 & 0x00F0u) >> 0u) + ((value_16 & 0x00F0u) >>  4u);
        Colour->B = (uint16_t)((value_16 & 0x000Fu) << 4u) + ((value_16 & 0x000Fu) >>  0u);
        break;
    case R_GFXUTILS_FORMAT_CLUT8:      /* all format below are not implemented yet */
    case R_GFXUTILS_FORMAT_YCBCR_444:
    case R_GFXUTILS_FORMAT_YCBCR_422:
    case R_GFXUTILS_FORMAT_YUV_YUYV:
    case R_GFXUTILS_FORMAT_YUV_UYVY:
    case R_GFXUTILS_FORMAT_YUV_YVYU:
    case R_GFXUTILS_FORMAT_YUV_VYUY:
    case R_GFXUTILS_FORMAT_ARGB1555:
    case R_GFXUTILS_FORMAT_RGBA5551:
    case R_GFXUTILS_FORMAT_CLUT4:
    case R_GFXUTILS_FORMAT_CLUT1:
    default:
        /* here format is never R_GFXUTILS_FORMAT_ARGB8888, so the below code is equivalent to 
         R_GFXUTILS_ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(format == R_GFXUTILS_FORMAT_ARGB8888);
        break; 
    }
}


static uint32_t loc_Random( void )
{
    static uint32_t m_w = 0x3485u;    /* <choose-initializer> must not be zero */
    static uint32_t m_z = 0xf9b3u;    /* <choose-initializer> must not be zero */
    m_z = (36969u * (m_z & 65535u)) + (m_z >> 16);
    m_w = (18000u * (m_w & 65535u)) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
}


static uint32_t loc_SwapBits(uint32_t no, uint32_t noOfBits)
{
    uint32_t res = 0;
    
    for (;noOfBits>0; noOfBits--)
    {
        res <<= 1u;
        res |= (no & 1u);
        no >>= 1u;
    }
    
    return res;
}

/* http://www.geekymonkey.com/Programming/CSharp/RGB2HSL_HSL2RGB.htm */
/* Returns a Color (RGB struct) in range of 0-255 */
static r_gfxutils_ARGB_t loc_HSL2RGB(float32_t h, float32_t sl, float32_t l)
{
    float32_t v;
    float32_t r,g,b;
    r_gfxutils_ARGB_t rgb;
    
    /*As h can be seen as an angle, map number greater than 1 back to <0..1> */
    h -= (int32_t)h;
    
    r = l;   /* default to gray */
    g = l;
    b = l; 
    v = (l <= 0.5f) ? (l * (1.0f + sl)) : ((l + sl) - (l * sl));
    if (v > 0) 
    {
        float32_t m;
        float32_t sv;
        int32_t sextant;
        float32_t fract, vsf, mid1, mid2;

        m = (l + l) - v;
        sv = (v - m ) / v;
        h *= 6.0f;
        sextant = (int32_t)h; 
        fract = h - sextant;
        vsf = v * sv * fract;
        mid1 = m + vsf;
        mid2 = v - vsf;
        switch (sextant) 
        {
        case 0: 
            r = v; 
            g = mid1; 
            b = m; 
            break;
        case 1: 
            r = mid2; 
            g = v; 
            b = m; 
            break;
        case 2: 
            r = m; 
            g = v; 
            b = mid1; 
            break;
        case 3: 
            r = m; 
            g = mid2; 
            b = v; 
            break;
        case 4: 
            r = mid1; 
            g = m; 
            b = v; 
            break;
        case 5: 
            r = v; 
            g = m; 
            b = mid2; 
            break;
        default:
            /* Should not be executed if properly programmed */
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            break;
        }
    }

    r *= 255.0f;
    g *= 255.0f;
    b *= 255.0f;

    rgb.A = 255;
    rgb.R = (uint8_t)r;
    rgb.G = (uint8_t)g;
    rgb.B = (uint8_t)b;
    return rgb;
}

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_GFXUTILS_InitAlloc

  Description:
  This function initialiases the allocation of memory. This is a very simple allocation mechanism to 
  be used with the test functions.

  Parameters:
  Base         - Physical base address where the memory allocation should start.
  MaxSize      - Gives the maximum size of the available memory in bytes.
  
  Returns      = The handle to the allocation struct. The struct is only visible to the alloc functions.
                 application can not access it.
*/

void *R_GFXUTILS_InitAlloc(uint32_t Base, uint32_t MaxSize) 
{
    r_gfxutils_AllocData_t *AllocData_p;
#ifdef R_GFXUTILS_USE_CDI
    AllocData_p = (r_gfxutils_AllocData_t *)R_CDI_HeapAlloc(sizeof(r_gfxutils_AllocData_t), CPU);
#else
    AllocData_p = (r_gfxutils_AllocData_t *)malloc(sizeof(r_gfxutils_AllocData_t));
#endif
    
    
    
    while (0 == AllocData_p)
    {
        /* If no AllocData_p can be allocated stop here */
    }
    
    AllocData_p->Base    = Base;
    AllocData_p->Current = Base;
    AllocData_p->Max     = Base + MaxSize;
    
    return (void *)AllocData_p;
}

/*******************************************************************************
  Function: R_GFXUTILS_DeleteObject

  Description:
  This function cleans up gfxutils objects.

  Parameters:
  Object_p  - Pointer to an gfxutils object.
*/


void R_GFXUTILS_DeleteObject(void *AllocData_p) 
{
#ifdef R_GFXUTILS_USE_CDI
    R_CDI_HeapFree((uint32_t)AllocData_p, CPU);
#else
    free(AllocData_p);
#endif

    return;
}

/*******************************************************************************
  Function: R_GFXUTILS_DeleteObjects

  Description:
  This function cleans up gfxutils objects.

  Parameters:
  Object_p  - Pointer to an gfxutils object.
*/

/* va_start violates several misra roules - but it's compiler "standard" */
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

void R_GFXUTILS_DeleteObjects(void *object, ...)
{
    va_list ap;
    va_start(ap, object);
    while(object)
    {
        R_GFXUTILS_DeleteObject(object);
        object = va_arg(ap, void *);
    }
    va_end(ap);
    return;
}

#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs */


/*******************************************************************************
  Function: R_GFXUTILS_Alloc

  Description:
  This function allocates some memory from the heap.
 
  Parameters:
  AllocData_parm  - Pointer to the alloc data struct.
  Size            = Size of the allocation.
*/


uint32_t R_GFXUTILS_Alloc(void *AllocData_parm, uint32_t Size) 
{
    r_gfxutils_AllocData_t *AllocData_p = (r_gfxutils_AllocData_t *)AllocData_parm;
    uint32_t return_value;
    
    if ((AllocData_p->Current + Size) > AllocData_p->Max)
    {
        return 0;
    } 
    else 
    {
        return_value = AllocData_p->Current;
        AllocData_p->Current+=Size;        
        return return_value;
    }
}

/*******************************************************************************
  Function: R_GFXUTILS_AllocAlign

  Description:
  This function allocates some memory from the heap. It will be aligned to Mask.
 
  Parameters:
  AllocData_parm  - Pointer to the alloc data struct.
  Size            = Size of the allocation.
  Mask            - The mask is first added and then anded. This results in an address
                    that is larger than the unaligned address but aligned.
*/

uint32_t R_GFXUTILS_AllocAlign(void *AllocData_parm, uint32_t Size, uint32_t Mask) 
{
    r_gfxutils_AllocData_t *AllocData_p = (r_gfxutils_AllocData_t *)AllocData_parm;
    uint32_t return_value;
    
    return_value = (AllocData_p->Current + Mask) & ~Mask;
    if ((return_value + Size) > AllocData_p->Max)
    {
        return 0;
    } 
    else 
    {
        AllocData_p->Current= return_value + Size;
        return return_value;
    }
}
/* just crate a buffer, no stride consideration, but take warping into account */
r_gfxutils_Surface_t *R_GFXUTILS_CreateBuffer(void      *AllocData_parm,
                                                         uint32_t Width,
                                                         uint32_t Height,
                                                         r_gfxutils_Format_t Format)
{
    uint32_t size;    
    r_gfxutils_Surface_t *surface;
    uint32_t bpp;

   
#ifdef R_GFXUTILS_USE_CDI
    surface = (r_gfxutils_Surface_t *)R_CDI_HeapAlloc(sizeof(r_gfxutils_Surface_t), CPU);
#else
    surface = (r_gfxutils_Surface_t *)malloc(sizeof(r_gfxutils_Surface_t));
#endif
    R_GFXUTILS_ASSERT(0 != surface);

    bpp = R_GFXUTILS_Format2BPP(Format);
    R_GFXUTILS_ASSERT(bpp != 0);
    
    size = (Width * Height * bpp)/8;
    surface->Width  = Width;
    surface->Height = Height;
    surface->Stride = Width;
    surface->BPP    = bpp;
    surface->Format = Format;
    surface->Size   = size;
    /* allign to 64 bit (8 byte) */
    surface->Base = R_GFXUTILS_AllocAlign( AllocData_parm, size , 8-1);
    R_GFXUTILS_ASSERT(0 != surface->Base);
    
    return surface;

}     
/* just create a buffer, no stride consideration  */
uint32_t R_GFXUTILS_AllocBuffer(r_cdi_Heap_t         *MemMgr,
                                uint32_t              Width,
                                uint32_t              Height,
                                r_gfxutils_Format_t   Format,
                                r_gfxutils_Surface_t *Surface)
{
    uint32_t size;    
    uint32_t bpp;

    bpp = R_GFXUTILS_Format2BPP(Format);
    R_GFXUTILS_ASSERT(bpp != 0);
    
    size = (Width * Height * bpp)/8;
    Surface->Width  = Width;
    Surface->Height = Height;
    Surface->Stride = Width;
    Surface->BPP    = bpp;
    Surface->Format = Format;
    Surface->Size   = size;
    Surface->Base = (uint32_t) R_CDI_Alloc(MemMgr, size);
    R_GFXUTILS_ASSERT(0 != Surface->Base);
    return 0;
}                                               

uint32_t R_GFXUTILS_AllocSurface(r_cdi_Heap_t          *MemMgr,
                                 uint32_t               Stride,                   
                                 uint32_t               Width,
                                 uint32_t               Height,
                                 r_gfxutils_Format_t    Format, 
                                 r_gfxutils_Surface_t  *Surface)
{
    uint32_t Size;    
    uint32_t ByteStride;
    uint32_t AlignedByteStride;
    uint32_t BPP;

    Stride = (0 != Stride) ? Stride : Width;

    BPP = R_GFXUTILS_Format2BPP(Format);
    R_GFXUTILS_ASSERT(BPP != 0);
    
    ByteStride = ((Stride * BPP) / 8);
    AlignedByteStride = (ByteStride + 0x7fu) & ~(uint32_t)0x7fu; /* align lower 7 bits upwards */
    Stride = (AlignedByteStride * 8) / BPP;
    
    Size = Stride * Height * (BPP/8);
    Surface->Width  = Width;
    Surface->Height = Height;
    Surface->Stride = Stride;
    Surface->BPP    = BPP;
    Surface->Format = Format;
    Surface->Size   = Size;

    Surface->Base = (uint32_t) R_CDI_Alloc(MemMgr, Size);
    R_GFXUTILS_ASSERT(0 != Surface->Base);
    
    return 0;
}

uint32_t R_GFXUTILS_SetSurface(  uint32_t               Stride,                   
                                 uint32_t               Width,
                                 uint32_t               Height,
                                 r_gfxutils_Format_t    Format, 
                                 r_gfxutils_Surface_t  *Surface)
{
    uint32_t Size;    
    uint32_t ByteStride;
    uint32_t AlignedByteStride;
    uint32_t BPP;

    Stride = (0 != Stride) ? Stride : Width;

    BPP = R_GFXUTILS_Format2BPP(Format);
    R_GFXUTILS_ASSERT(BPP != 0);
    
    ByteStride = ((Stride * BPP) / 8);
    AlignedByteStride = (ByteStride + 0x7fu) & ~(uint32_t)0x7fu; /* align lower 7 bits upwards */
    Stride = (AlignedByteStride * 8) / BPP;
    
    Size = Stride * Height * (BPP/8);
    Surface->Width  = Width;
    Surface->Height = Height;
    Surface->Stride = Stride;
    Surface->BPP    = BPP;
    Surface->Format = Format;
    Surface->Size   = Size;

    Surface->Base   = 0;
    return 0;
}



r_gfxutils_Surface_t *R_GFXUTILS_CreateSurface(void *AllocData_parm,
                                               uint32_t Stride,                   
                                               uint32_t Width,
                                               uint32_t Height,
                                               r_gfxutils_Format_t Format) 
{
    uint32_t Size;    
    r_gfxutils_Surface_t *Surface;
    uint32_t ByteStride;
    uint32_t AlignedByteStride;
    uint32_t BPP;

    Stride = (0 != Stride) ? Stride : Width;
    
#ifdef R_GFXUTILS_USE_CDI
    Surface = (r_gfxutils_Surface_t *)R_CDI_HeapAlloc(sizeof(r_gfxutils_Surface_t), CPU);
#else
    Surface = (r_gfxutils_Surface_t *)malloc(sizeof(r_gfxutils_Surface_t));
#endif
    R_GFXUTILS_ASSERT(0 != Surface);

    BPP = R_GFXUTILS_Format2BPP(Format);
    R_GFXUTILS_ASSERT(BPP != 0);
    
    ByteStride = ((Stride * BPP) / 8);
    AlignedByteStride = (ByteStride + 0x7fu) & ~(uint32_t)0x7fu; /* align lower 7 bits upwards */
    Stride = (AlignedByteStride * 8) / BPP;
    
    Size = ((Stride * Height * BPP) / 8);
    Surface->Width  = Width;
    Surface->Height = Height;
    Surface->Stride = Stride;
    Surface->BPP    = BPP;
    Surface->Format = Format;
    Surface->Size   = Size;

    Surface->Base = R_GFXUTILS_AllocAlign( AllocData_parm, Size , 128-1);
    R_GFXUTILS_ASSERT(0 != Surface->Base);
    
    return Surface;
}


void R_GFXUTILS_CreateMonoBar(r_gfxutils_Surface_t *Surface_p,
                            uint32_t barXpos,  uint32_t barYpos,
                            uint32_t barWidth, uint32_t barHeight,
                            r_gfxutils_ARGB_t barColour)
{
    uint32_t x,y;
    /*uint32_t line_address, pixel_address, byte_stride, byte_step;*/
    
    /*byte_step    = Surface_p->BPP / 8;*/
    /*byte_stride  = Surface_p->Stride * byte_step;*/
    
    for(y = barYpos ; y < (barYpos+barHeight) ; y++)
    {
        /*line_address = Surface_p->Base + (y * byte_stride);*/
        for(x = barXpos ; x < (barXpos+barWidth) ; x++)
        {
            /*pixel_address = line_address + (x * byte_step);*/
            /*GFXUTILS_WritePixel(pixel_address, barColour, Surface_p->Format);*/
            GFXUTILS_WritePixelNew(x, y, Surface_p, barColour);
        }
    }
}

void R_GFXUTILS_CreateHorizontalGradient(r_gfxutils_Surface_t *Surface_p, int32_t factor) 
{
    uint32_t x,y;
    uint32_t line_address, pixel_address, byte_stride, byte_step;
    const int32_t max_colours=4;
    r_gfxutils_ARGB_t colour[]     = {
        {  0, 255, 255, 255},
        {255,   0,   0,   0},
        {255,   0,   0,   0},
        {255,   0,   0,   0},
        {  0,   0,   0,   0}}; /* Dummy */
    r_gfxutils_ARGB_t colour_inc[] = {
        {  1,   0,   0,   0},
        {  0,   1,   0,   0},
        {  0,   0,   1,   0},
        {  0,   0,   0,   1},
        {  0,   0,   0,   0}}; /* Dummy */

#if 0
         {255, 255, 255, 255}, 
         {  0,   1,   1,   1} 
#endif 

    int32_t colour_index, index_border;
    float64_t colour_step;
    float64_t temp = 0; /* MISRA */

    colour_index = 0;
    line_address = Surface_p->Base;
    byte_step    = Surface_p->BPP / 8;
    byte_stride  = Surface_p->Stride * byte_step;
    
    colour_step  = ( (float64_t)255 / (Surface_p->Width-1) ) * factor;
    index_border = Surface_p->Height / max_colours;
    
    for(y = 0 ; y < Surface_p->Height ; y++)
    {
        r_gfxutils_ARGB_t current_colour;
        pixel_address = line_address;
        for(x = 0 ; x < Surface_p->Width ; x++)
        {
            temp = (colour_inc[colour_index].A * colour_step * x) + colour[colour_index].A;
            current_colour.A = (uint8_t)temp; 
            temp = (colour_inc[colour_index].R * colour_step * x) + colour[colour_index].R;
            current_colour.R = (uint8_t)temp; 
            temp = (colour_inc[colour_index].G * colour_step * x) + colour[colour_index].G;
            current_colour.G = (uint8_t)temp;
            temp = (colour_inc[colour_index].B * colour_step * x) + colour[colour_index].B;
            current_colour.B = (uint8_t)temp;
            /*GFXUTILS_WritePixel(pixel_address, current_colour, Surface_p->Format);*/
            GFXUTILS_WritePixelNew(x, y, Surface_p, current_colour);
            pixel_address    += byte_step;
        }
        line_address += byte_stride;
        colour_index = y/index_border;
    }
}

void R_GFXUTILS_CreateVerticalGradient(r_gfxutils_Surface_t *Surface_p, int32_t factor) 
{
    R_GFXUTILS_CreateVerticalGradientMask(Surface_p, factor, 0xf);
}

/* The function writes vertical gradient. The mask indicates which stripe should be drawn.
   bit 0 <-> stripe 0
   bit 1 <-> stripe 1
   bit 2 <-> stripe 2
   bit 3 <-> stripe 3
   If the bit is set stripe will be drawn. If the bit is clear stripe will be set to 0.
   Careful 0 may not be transparent in non alpha modes.
*/

void R_GFXUTILS_CreateVerticalGradientMask(r_gfxutils_Surface_t *Surface_p,int32_t factor, 
                                           uint32_t Mask) 
{
    uint32_t x,y;
    const int32_t max_colours=4;
    r_gfxutils_ARGB_t zero = {0,0,0,0};
    r_gfxutils_ARGB_t colour[] = {
        {  0, 255, 255, 255},
        {255,   0,   0,   0},
        {255,   0,   0,   0},
        {255,   0,   0,   0},
        {  0,   0,   0,   0}}; /* Dummy */
    r_gfxutils_ARGB_t colour_inc[] = {
        {  1,   0,   0,   0},
        {  0,   1,   0,   0},
        {  0,   0,   1,   0},
        {  0,   0,   0,   1},
        {  0,   0,   0,   0}}; /* Dummy */

    uint32_t  colour_index;
    float64_t colour_step;
    float64_t intermediate_result;  /* For MISRA */

    for(colour_index = 0;colour_index < max_colours; colour_index++)
    {
        uint32_t current_mask = (Mask >> colour_index)&1u;        
        if (current_mask == 0)
        {
            colour[colour_index] = zero;
            colour_inc[colour_index] = zero;
        }
        if ((Surface_p->Format == R_GFXUTILS_FORMAT_CLUT8) ||
            (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT4) ||
            (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT1))
        {
            colour_inc[colour_index].R = /* replicate colour_inc from other channels */
                ( (0 != colour_inc[colour_index].A) ||
                  (0 != colour_inc[colour_index].G) ||
                  (0 != colour_inc[colour_index].B) ||
                  (0 != colour_inc[colour_index].R) ) ? 1 : 0;
            colour[colour_index] = zero; /* There is only one colour in 8 bpp mode, 
                                            all other colours have to be 0 */
        }    
    }
    

    colour_index = 0;
    
    colour_step  = ( (float64_t)255 / (Surface_p->Height-1) ) * factor;
    
    for(x = 0 ; x < Surface_p->Width ; x++)
    {
        r_gfxutils_ARGB_t current_colour;
        for(y = 0 ; y < Surface_p->Height ; y++)
        {
            intermediate_result = ((colour_inc[colour_index].A * colour_step * y) + .5 + colour[colour_index].A);
            current_colour.A = (uint8_t)intermediate_result;
            intermediate_result = ((colour_inc[colour_index].R * colour_step * y) + .5 + colour[colour_index].R);
            current_colour.R = (uint8_t)intermediate_result;
            intermediate_result = ((colour_inc[colour_index].G * colour_step * y) + .5 + colour[colour_index].G);
            current_colour.G = (uint8_t)intermediate_result;
            intermediate_result = ((colour_inc[colour_index].B * colour_step * y) + .5 + colour[colour_index].B);
            current_colour.B = (uint8_t)intermediate_result;
            GFXUTILS_WritePixelNew(x, y, Surface_p, current_colour);
        }
        colour_index = (x * max_colours) / Surface_p->Width;
    }
}




void R_GFXUTILS_CreateCheckerBoard(r_gfxutils_Surface_t *Surface_p, int32_t tile_width,
                                   int32_t tile_height, r_gfxutils_ARGB_t colour1,
                                   r_gfxutils_ARGB_t colour2 )
{
    uint32_t x,y;
    uint32_t line_address, pixel_address, byte_stride, byte_step, index;

    line_address = Surface_p->Base;
    byte_step    = Surface_p->BPP / 8;
    byte_stride  = Surface_p->Stride * byte_step;
    
    for(y = 0 ; y < Surface_p->Height ; y++)
    {
        r_gfxutils_ARGB_t current_colour;
        pixel_address = line_address;
        for(x = 0 ; x < Surface_p->Width ; x++)
        {
            index  = ( ( (x / tile_width) % 2) + ( (y / tile_height) % 2) ) % 2;
            if ( index == 0) 
            {
                current_colour = colour1;
            } 
            else 
            {
                current_colour = colour2;
            }
            /*GFXUTILS_WritePixel(pixel_address, current_colour, Surface_p->Format);*/
            GFXUTILS_WritePixelNew(x, y, Surface_p, current_colour);
            pixel_address    += byte_step;
        }
        line_address += byte_stride;
    }
}


void R_GFXUTILS_CreateCheckerBoardHSL(r_gfxutils_Surface_t *Surface_p, int32_t tile_width,
                                      int32_t tile_height )
{
    uint32_t x,y,index;

    for(y = 0 ; y < Surface_p->Height ; y++)
    {
        r_gfxutils_ARGB_t current_colour;
        for(x = 0 ; x < Surface_p->Width ; x++)
        {
            index  = ( ( (x / tile_width) % 2) + ( (y / tile_height) % 2) ) % 2;
            if ( index == 0) 
            {
                current_colour = loc_HSL2RGB((float32_t)y/Surface_p->Height,1.0f-((float32_t)x/Surface_p->Width), 0.5f);
            } 
            else 
            {
                current_colour = loc_HSL2RGB(0.5f+((float32_t)y/Surface_p->Height),(float32_t)x/Surface_p->Width, 0.5f);
            }
            GFXUTILS_WritePixelNew(x, y, Surface_p, current_colour);
        }
    }
}

void R_GFXUTILS_CreateCrossRGB(r_gfxutils_Surface_t *Surface_p) 
{
    R_GFXUTILS_CreateCrossRGB_Pattern(Surface_p, 1);
}

void R_GFXUTILS_CreateCrossRGB_Pattern(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern) 
{
    R_GFXUTILS_CreateCrossRGB_Pattern_Stepsize(Surface_p, Pattern, 1);
}

void R_GFXUTILS_CreateCrossRGB_Pattern_Stepsize(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern, uint32_t Stepsize) 
{
    uint32_t x,y,x_2;
    r_gfxutils_ARGB_t colourW={255,255,255,255};
    r_gfxutils_ARGB_t colourB={255,  0,  0,  0};
    uint32_t Step = 0;
    
    if (0 == Pattern)
    {
        colourB.R = 255;
        colourB.G = 255;
        colourB.B = 255;
    }


    for(x = 0 ; (x < Surface_p->Width) && (x < Surface_p->Height) ; x++)
    {
        if (0 == (x%Stepsize))
        {
            Step = 1-Step;  /* Toggle between 0 and 1 */
        }
        
        y = x;
        GFXUTILS_WritePixelNew(x, y, Surface_p, (1 == Step)?colourW:colourB);
        
        if( y < Surface_p->Height ) 
        {
            x_2 = Surface_p->Width-x-1;
            GFXUTILS_WritePixelNew(x_2, y, Surface_p, (1 == Step)?colourW:colourB);
        }
    }
}

void R_GFXUTILS_CreateBorderRGB(r_gfxutils_Surface_t *Surface_p) 
{
    R_GFXUTILS_CreateBorderRGB_Pattern(Surface_p, 1);
}

void R_GFXUTILS_CreateBorderRGB_Pattern(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern) 
{
    R_GFXUTILS_CreateBorderRGB_Pattern_Stepsize(Surface_p, Pattern, 1);
}

void R_GFXUTILS_CreateBorderRGB_Pattern_Stepsize(r_gfxutils_Surface_t *Surface_p, uint32_t Pattern, uint32_t Stepsize) 
{
    uint32_t x,y;
    r_gfxutils_ARGB_t colourW={255,255,255,255};
    r_gfxutils_ARGB_t colourB={255,  0,  0,  0};
    uint32_t Step = 0;

    if (0 == Pattern)
    {
        colourB.R = 255;
        colourB.G = 255;
        colourB.B = 255;
    }

    for(x = 0 ; x < Surface_p->Width ; x++)
    {
        if (0 == (x%Stepsize))
        {
            Step = 1 - Step;  /* Toggle between 0 and 1 */
        }
            
        y  = 0;
        GFXUTILS_WritePixelNew(x, y, Surface_p, (1 == Step)?colourW:colourB);

        y = Surface_p->Height - 1;
        GFXUTILS_WritePixelNew(x, y, Surface_p, (1 == Step)?colourW:colourB);
    }

    for(y = 0 ; y < Surface_p->Height ; y++)
    {
        if (0 == (y%Stepsize))
        {
            Step = 1 - Step;  /* Toggle between 0 and 1 */
        }

        x = 0;
        GFXUTILS_WritePixelNew(x, y, Surface_p, (1 == Step)?colourW:colourB);
            
        x = Surface_p->Width - 1;
        GFXUTILS_WritePixelNew(x, y, Surface_p, (1 == Step)?colourW:colourB);
    }
}


/*--------------------------------------------------------------------------
  Section: RGB -> YUV

     
  Conversion Matrix:
  >  Y  =      (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
  >  Cr = V =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128
  >  Cb = U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128
    

*/

void R_GFXUTILS_Rgb2yuv_d_i(r_gfxutils_dARGB_t *in,
                            r_gfxutils_YUV_t *out) 
{
    r_gfxutils_dYUV_t d_out;
    R_GFXUTILS_Rgb2yuv_d_d(in,&d_out);
    out->Y = (uint8_t)floor(d_out.Y + .5); 
    if (out->Y == 0) 
    {
        out->Y++; 
    }
    if (out->Y == 255u) 
    {    
        out->Y--;
    }
    out->U = (uint8_t)floor(d_out.U + .5);
    if (out->U == 0) 
    {
        out->U++; 
    }
    if (out->U == 255u) 
    {    
        out->U--;
    }
    out->V = (uint8_t)floor(d_out.V + .5); 
    if (out->V == 0) 
    {
        out->V++; 
    }
    if (out->V == 255u) 
    {    
        out->V--;
    }
}


void R_GFXUTILS_Rgb2yuv_i_i(r_gfxutils_ARGB_t *in, 
                            r_gfxutils_YUV_t *out)
{
    r_gfxutils_dARGB_t d_in;
    r_gfxutils_dYUV_t d_out;
    d_in.R = in->R;
    d_in.G = in->G;
    d_in.B = in->B;
    R_GFXUTILS_Rgb2yuv_d_d(&d_in,&d_out);
    out->Y = (uint8_t)floor(d_out.Y + .5); 
    if (out->Y == 0) 
    {
        out->Y++; 
    }
    if (out->Y == 255u) 
    {    
        out->Y--;
    }
    out->U = (uint8_t)floor(d_out.U + .5);
    if (out->U == 0) 
    {
        out->U++; 
    }
    if (out->U == 255u) 
    {    
        out->U--;
    }
    out->V = (uint8_t)floor(d_out.V + .5); 
    if (out->V == 0) 
    {
        out->V++; 
    }
    if (out->V == 255u) 
    {    
        out->V--;
    }
}

void R_GFXUTILS_Rgb2yuv_i_d(r_gfxutils_ARGB_t *in,
                            r_gfxutils_dYUV_t *out) 
{
    r_gfxutils_dARGB_t d_in;
    d_in.R=in->R;
    d_in.G=in->G;
    d_in.B=in->B;
    R_GFXUTILS_Rgb2yuv_d_d(&d_in,out);
}


void R_GFXUTILS_Rgb2yuv_d_d(r_gfxutils_dARGB_t *in, 
                            r_gfxutils_dYUV_t *out) {
    out->Y = (uint8_t)floor( (0.257 * in->R) + (0.504 * in->G) + (0.098 * in->B) +  16 + .5);
    out->U = (uint8_t)floor((-(0.148 * in->R) - (0.291 * in->G)) + (0.439 * in->B) + 128 + .5);
    out->V = (uint8_t)floor( ((0.439 * in->R) - (0.368 * in->G) - (0.071 * in->B)) + 128 + .5);
}

#if 0

void R_GFXUTILS_Yuv2rgb_i_i(r_gfxutils_dARGB_t *in, 
                            r_gfxutils_dYUV_t *out) {
    out->Y = floor( (1.16406 * (in->Y - 16)) + ( 0.00000 * (in->U - 128)) + 
                    (1.59570 * (in->V - 128)) + .5);
    out->U = floor(-(0.148 * in->R) - (0.291 * in->G) + (0.439 * in->B) + 128 + .5);
    out->V = floor( (0.439 * in->R) - (0.368 * in->G) - (0.071 * in->B) + 128 + .5);
    1.59570
        1.16406  -0.39199  -0.81250
        1.16406   2.01758   0.00000
        }

#endif          


void R_GFXUTILS_SurfaceFill(r_gfxutils_Surface_t *Surface_p, 
                            uint32_t xOffs,  uint32_t yOffs,
                            r_gfxutils_ARGB_t Colour)
{
    R_GFXUTILS_SurfaceFillS(Surface_p, 
        xOffs,  yOffs, Surface_p->Width-xOffs, Surface_p->Height-yOffs, Colour);
}

void R_GFXUTILS_SurfaceFillS(r_gfxutils_Surface_t *Surface_p, 
                            uint32_t xOffs,  uint32_t yOffs, uint32_t width, uint32_t height,
                            r_gfxutils_ARGB_t Colour)
{
    uint32_t x,y;
    
    for(y = yOffs; y < (yOffs+height); y++)
    {
        for(x = xOffs; x < (xOffs+width); x++)
        {
            GFXUTILS_WritePixelNew(x, y, Surface_p, Colour);
        }
    }
}

void R_GFXUTILS_RemoveAlpha(r_gfxutils_Surface_t *Surface_p, uint8_t alphaValue)
{
    uint32_t x,y;
    uint32_t line_Address, pixel_Address, byte_Stride, byte_Step;
    
    r_gfxutils_ARGB_t Colour;
    
    byte_Step    = Surface_p->BPP / 8;
    byte_Stride  = Surface_p->Stride * byte_Step;
    
    for(y = 0; y < Surface_p->Height ; y++)
    {
        line_Address = Surface_p->Base + (y * byte_Stride);
        for(x = 0; x < Surface_p->Width ; x++)
        {
            pixel_Address = line_Address + (x * byte_Step);
            GFXUTILS_ReadPixel (pixel_Address, &Colour, Surface_p->Format);
            Colour.A = alphaValue;
            /*GFXUTILS_WritePixel(pixel_Address, Colour,  Surface_p->Format);*/
            GFXUTILS_WritePixelNew(x, y, Surface_p, Colour);
        }
    }
}

void R_GFXUTILS_BlitCopy(r_gfxutils_Surface_t *SrcSurface_p,
                            r_gfxutils_Surface_t *DstSurface_p,
                            uint32_t xOffs,  uint32_t yOffs)
{
    uint32_t x,y;
    uint32_t line_SrcAddress, pixel_SrcAddress, byte_SrcStride, byte_SrcStep;
    /*uint32_t line_DstAddress, pixel_DstAddress, byte_DstStride, byte_DstStep;*/
    
    r_gfxutils_ARGB_t Colour;
    Colour.A = 0xFF; Colour.R = 255; Colour.G = 255; Colour.B = 255;
    
    /*byte_DstStep    = DstSurface_p->BPP / 8;*/
    /*byte_DstStride  = DstSurface_p->Stride * byte_DstStep;*/
    byte_SrcStep    = SrcSurface_p->BPP / 8;
    byte_SrcStride  = SrcSurface_p->Stride * byte_SrcStep;
    
    for(y = 0; y < SrcSurface_p->Height ; y++)
    {
        line_SrcAddress = SrcSurface_p->Base + (y * byte_SrcStride);
        /*line_DstAddress = DstSurface_p->Base + ((y+yOffs) * byte_DstStride);*/
        for(x = 0; x < SrcSurface_p->Width ; x++)
        {
            pixel_SrcAddress = line_SrcAddress + (x * byte_SrcStep);
            /*pixel_DstAddress = line_DstAddress + ((x+xOffs) * byte_DstStep);*/
            GFXUTILS_ReadPixel(pixel_SrcAddress, &Colour, SrcSurface_p->Format);
            /*GFXUTILS_WritePixel(pixel_DstAddress, Colour, DstSurface_p->Format);*/
            GFXUTILS_WritePixelNew(x+xOffs, y+yOffs, DstSurface_p, Colour);
        }
    }
}


void R_GFXUTILS_BlitBlend(r_gfxutils_Surface_t *SrcSurface_p,
                            r_gfxutils_Surface_t *DstSurface_p,
                            uint32_t xOffs,  uint32_t yOffs)
{
    R_GFXUTILS_BlitBlendA(SrcSurface_p, DstSurface_p, xOffs, yOffs, 0xFF);
}

void R_GFXUTILS_BlitBlendA(r_gfxutils_Surface_t *SrcSurface_p,
                            r_gfxutils_Surface_t *DstSurface_p,
                            uint32_t xOffs,  uint32_t yOffs,
                            uint32_t alpha)
{
    uint32_t x,y;
    uint32_t line_SrcAddress, pixel_SrcAddress, byte_SrcStride, byte_SrcStep;
    uint32_t line_DstAddress, pixel_DstAddress, byte_DstStride, byte_DstStep;
    r_gfxutils_ARGB_t ColourB, ColourF, Colour;
    
    byte_DstStep    = DstSurface_p->BPP / 8;
    byte_DstStride  = DstSurface_p->Stride * byte_DstStep;
    byte_SrcStep    = SrcSurface_p->BPP / 8;
    byte_SrcStride  = SrcSurface_p->Stride * byte_SrcStep;
    
    for(y = 0; y < SrcSurface_p->Height ; y++)
    {
        line_SrcAddress = SrcSurface_p->Base + (y * byte_SrcStride);
        line_DstAddress = DstSurface_p->Base + ((y+yOffs) * byte_DstStride);
        for(x = 0; x < SrcSurface_p->Width ; x++)
        {
            float32_t intermediate_value; /* For MISRA */
            pixel_SrcAddress = line_SrcAddress + (x * byte_SrcStep);
            pixel_DstAddress = line_DstAddress + ((x+xOffs) * byte_DstStep);
            GFXUTILS_ReadPixel(pixel_SrcAddress, &ColourF, SrcSurface_p->Format);
            GFXUTILS_ReadPixel(pixel_DstAddress, &ColourB, DstSurface_p->Format);
            intermediate_value = ColourF.A * ((float32_t)(alpha)/255.0f);
            ColourF.A = (uint32_t)intermediate_value;
            Colour.A = ColourB.A;
            Colour.R = (((255-ColourF.A)*ColourB.R) + ((ColourF.A)*ColourF.R))/255;
            Colour.G = (((255-ColourF.A)*ColourB.G) + ((ColourF.A)*ColourF.G))/255;
            Colour.B = (((255-ColourF.A)*ColourB.B) + ((ColourF.A)*ColourF.B))/255;
            /*GFXUTILS_WritePixel(pixel_DstAddress, Colour, DstSurface_p->Format);*/
            GFXUTILS_WritePixelNew(x+xOffs, y+yOffs, DstSurface_p, Colour);
        }
    }
}


void R_GFXUTILS_CreateDither(r_gfxutils_Surface_t *Surface_p, int32_t factor)
{
    uint32_t x,y;
    uint32_t line_Address, pixel_Address, byte_Stride, byte_Step;
    int32_t rnd;
    
    r_gfxutils_ARGB_t Colour;
    
    byte_Step    = Surface_p->BPP / 8;
    byte_Stride  = Surface_p->Stride * byte_Step;
    
    for(y = 0; y < Surface_p->Height ; y++)
    {
        line_Address = Surface_p->Base + (y * byte_Stride);
        for(x = 0; x < Surface_p->Width ; x++)
        {
            pixel_Address = line_Address + (x * byte_Step);
            GFXUTILS_ReadPixel(pixel_Address, &Colour, Surface_p->Format);
            rnd = ((loc_Random()%factor) - (factor/2));
            if (!(((Colour.R + rnd) > 255) || (((int32_t)Colour.R + rnd) < 0)))
            {
                Colour.R = (Colour.R + rnd);
            }
            rnd = ((loc_Random()%factor) - (factor/2));
            if (!(((Colour.G + rnd) > 255) || (((int32_t)Colour.G + rnd) < 0)))
            {
                Colour.G = (Colour.G + rnd);
            }
            rnd = ((loc_Random()%factor) - (factor/2));
            if (!(((Colour.B + rnd) > 255) || (((int32_t)Colour.B + rnd) < 0)))
            {
                Colour.B = (Colour.B + rnd);
            }
            /*GFXUTILS_WritePixel(pixel_Address, Colour, Surface_p->Format);*/
            GFXUTILS_WritePixelNew(x, y, Surface_p, Colour);
        }
    }
}


void R_GFXUTILS_SwapBits(r_gfxutils_Surface_t *Surface_p, uint32_t bitsToStart, uint32_t noOfBits)
{
    uint32_t x,y;
    uint32_t line_Address, pixel_Address, byte_Stride, byte_Step;
    uint32_t val, mask;
    
    mask = 0;
    for(x = 0; x<noOfBits; x++)
    {
        mask <<= 1u;
        mask |= 1u;
    }
    
    mask <<= bitsToStart;
    
    byte_Step    = Surface_p->BPP / 8;
    byte_Stride  = Surface_p->Stride * byte_Step;
    
    for(y = 0; y < Surface_p->Height ; y++)
    {
        line_Address = Surface_p->Base + (y * byte_Stride);
        for(x = 0; x < Surface_p->Width ; x++)
        {
            pixel_Address = line_Address + (x * byte_Step);
            
            val = (*(uint32_t*)pixel_Address) & mask;
            val >>= bitsToStart;
            val = loc_SwapBits(val, noOfBits);
            val <<= bitsToStart;
            
            *(uint32_t*)pixel_Address = ((*(uint32_t*)pixel_Address) & ~mask) | val;
        }
    }
}


void R_GFXUTILS_CutBits(r_gfxutils_Surface_t *Surface_p, uint32_t bitsToStart, uint32_t noOfBits, uint32_t mask)
{
    uint32_t x,y;
    uint32_t line_Address, pixel_Address, byte_Stride, byte_Step;
    uint32_t val, highVal;
    
    uint32_t lowMask;
    
    lowMask = 0;
    for(x = 0; x<bitsToStart; x++)
    {
        lowMask <<= 1u;
        lowMask |= 1u;
    }
    
    byte_Step    = Surface_p->BPP / 8;
    byte_Stride  = Surface_p->Stride * byte_Step;
    
    for(y = 0; y < Surface_p->Height ; y++)
    {
        line_Address = Surface_p->Base + (y * byte_Stride);
        for(x = 0; x < Surface_p->Width ; x++)
        {
            pixel_Address = line_Address + (x * byte_Step);
            
            val = (*(uint32_t*)pixel_Address) & mask;
            highVal = val >> noOfBits;
            val = (highVal & ~lowMask) | (val & lowMask);
            
            *(uint32_t*)pixel_Address = ((*(uint32_t*)pixel_Address) & ~mask) | val;
        }
    }
}



/* Codepage 850 Bitmap Font. */
/* Flags:
    1: Transparency. Do not write background colour.
    2: Linebreak.    Process Linebreaks in the string.
    4: Autobreak.    Automatically break when reaching the end of the screen. No word wrap.
    8:
   16: */
void R_GFXUTILS_WriteString(r_gfxutils_Surface_t *Surface_p, 
                            uint32_t xOffs, uint32_t yOffs, 
                            uint8_t *string, uint32_t Flags)
{
    const uint8_t font_width = 6;
    const uint8_t font_height = 8;
    
    uint32_t x,y;

    uint8_t chr;
    uint8_t data;
    uint32_t chr_cnt = 0;
    
    r_gfxutils_ARGB_t FGColour;
    r_gfxutils_ARGB_t BGColour;
    
    FGColour.A = 0xFF; FGColour.R = 255; FGColour.G = 255; FGColour.B = 255;
    BGColour.A = 0xFF; BGColour.R =   0; BGColour.G =   0; BGColour.B =   0;
    

    /* Iterate over supplied character string */
    for (chr_cnt = 0; *string != 0; string++)
    {
        chr = *string;
        
        /* Switch line when Line-Breaks are activated */
        /* Don't print an empty character then */
        if ( ('\n' == chr) && (0u != (Flags&2u)) )
        {
            chr_cnt = 0;
            yOffs += font_height;
        }
        /* Else: just print the characters*/
        else
        {
            /* Check if we leave the screen horizontally */
            if ( (((chr_cnt + 1) * font_width) + xOffs) > Surface_p->Width)
            {
                if (0 == (Flags&4u))
                {
                    /* Stop if string leaves screen */
                    break;
                }
                else
                {
                    /* Change line if string leaves screen */
                    chr_cnt = 0;
                    yOffs += font_height;
                }
            }
    
            for (y=0; y<font_height; y++)
            {   
                /* Check if we leave the screen vertically */
                if ( (y+yOffs) >= Surface_p->Height)
                {
                    break;
                }
                
                data = font6x8[chr][y];
                for (x=0; x<font_width; x++)
                {   
                    if (0 != (data&4u))
                    {
                        GFXUTILS_WritePixelNew((x+xOffs)+(chr_cnt*font_width), y+yOffs, Surface_p, FGColour);
                    }
                    else if (0 == (Flags&1u) )
                    {
                        GFXUTILS_WritePixelNew((x+xOffs)+(chr_cnt*font_width), y+yOffs, Surface_p, BGColour);
                    }
                    else
                    {
                        /* For MISRA */
                    }
                    data=data>>1u;
                }   
            }
            
            chr_cnt++;
        }
    }
}




#define WIDTH_COLOURS     (4u)
#define HEIGHT_COLOURS    (2u)
#define MAX_COLOURS       (WIDTH_COLOURS * HEIGHT_COLOURS)
void R_GFXUTILS_CreateVertical(r_gfxutils_Surface_t *Surface_p, r_gfxutils_ARGB_t standard)
{
    uint32_t x,y,y_colour;
    uint32_t column_address, pixel_address, byte_stride, byte_step;
    r_gfxutils_ARGB_t colour[MAX_COLOURS];
    uint32_t  colour_index = 0;
    uint32_t height_colours;

    /* White */
    colour[0].A = standard.A;
    colour[0].R = standard.R;
    colour[0].G = standard.G;
    colour[0].B = standard.B;
    /* Red */
    colour[1].A = standard.A;
    colour[1].R = standard.R;
    colour[1].G = 0;
    colour[1].B = 0;
    /* Green */
    colour[2].A = standard.A;
    colour[2].R = 0;
    colour[2].G = standard.G;
    colour[2].B = 0;
    /* Blue */
    colour[3].A = standard.A;
    colour[3].R = 0;
    colour[3].G = 0;
    colour[3].B = standard.B;
    /* Gray */
    colour[4].A = standard.A;
    colour[4].R = standard.R / 2;
    colour[4].G = standard.G / 2;
    colour[4].B = standard.B / 2;
    /* Yellow */
    colour[5].A = standard.A;
    colour[5].R = standard.R;
    colour[5].G = standard.G;
    colour[5].B = 0;
    /* Cyan */
    colour[6].A = standard.A;
    colour[6].R = 0;
    colour[6].G = standard.G;
    colour[6].B = standard.B;
    /* Magenta */
    colour[7].A = standard.A;
    colour[7].R = standard.R;
    colour[7].G = 0;
    colour[7].B = standard.B;

    byte_step    = Surface_p->BPP / 8;
    byte_stride  = Surface_p->Stride * byte_step;

    if (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT8)
    {
        colour[0].R = 0x0;
        colour[1].R = 0x1;
        colour[2].R = 0x2;
        colour[3].R = 0x3;
        colour[4].R = 0x4;
        colour[5].R = 0x5;
        colour[6].R = 0x6;
        colour[7].R = 0x7;
    }
    else if (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT4)
    {
        colour[0].R = 0x0;
        colour[1].R = 0x11;
        colour[2].R = 0x22;
        colour[3].R = 0x33;
        colour[4].R = 0x44;
        colour[5].R = 0x55;
        colour[6].R = 0x66;
        colour[7].R = 0x77;
        byte_step = 1;
        byte_stride = Surface_p->Stride / (8 / Surface_p->BPP);
    }
    else if (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT1)
    {
        colour[0].R = 0x0;
        colour[1].R = 0x0;
        colour[2].R = 0xff;
        colour[3].R = 0xff;
        colour[4].R = 0xff;
        colour[5].R = 0xff;
        colour[6].R = 0x0;
        colour[7].R = 0x0;
        byte_step = 1;
        byte_stride = Surface_p->Stride / (8 / Surface_p->BPP);
    }
    else
    {
        /* Do nothing */
    }

    height_colours = HEIGHT_COLOURS;
    if ((Surface_p->Height % HEIGHT_COLOURS) != 0)
    {
        height_colours = 1;
    }

    for(y_colour = 0 ; y_colour < height_colours ; y_colour++)
    {
        column_address = Surface_p->Base + ((byte_stride * (Surface_p->Height / height_colours)) * y_colour);
        for(x = 0 ; x < Surface_p->Width ;)
        {
            pixel_address = column_address;
            colour_index = ((x * WIDTH_COLOURS) / Surface_p->Width) + (WIDTH_COLOURS * y_colour);
            for(y = 0 ; y < (Surface_p->Height / height_colours) ; y++)
            {
                GFXUTILS_WritePixel(pixel_address, colour[colour_index], Surface_p->Format);
                /*GFXUTILS_WritePixelNew(x, y, Surface_p, colour[colour_index]);*/
                pixel_address += byte_stride;
            }
            column_address += byte_step;

            if (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT4)
            {
                x += 2;
            }
            else if (Surface_p->Format == R_GFXUTILS_FORMAT_CLUT1)
            {
                x += 8;
            }
            else
            {
                x++;
            }
        }
    }
}



