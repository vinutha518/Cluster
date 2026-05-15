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
#include "r_cdi_api.h"
#include "r_bsp_stdio_api.h"

#ifdef USE_VDCO
  #include "r_vdco_api.h"
  #define R_GFXUTILS_CONVERT_USE_VDCO
#endif  /* USE_VDCO */
#ifdef USE_VDCE
  #include "r_vdce_api.h"
  #define R_GFXUTILS_CONVERT_USE_VDCE
#endif  /* USE_VDCE */
#ifdef USE_WM
  #include "r_cdi_api.h"
  #include "r_wm_api.h"
  #define R_GFXUTILS_CONVERT_USE_WM
#endif /* USE_WM */

#include "r_gfxutils_api.h"
#include "r_gfxutils_convert.h"



/*******************************************************************************
  Section: LocalTypes
*/

#ifdef USE_WM
typedef struct {
    r_wm_WinColorFmt_t  WmFmt;
    r_gfxutils_Format_t GfxutilsFmt;
} Wm2InternalFmt_t;

Wm2InternalFmt_t Wm2InternalFmtTable[] = {
    {R_WM_COLORFMT_RGB565,          R_GFXUTILS_FORMAT_RGB565},
    {R_WM_COLORFMT_ARGB1555,        R_GFXUTILS_FORMAT_ARGB1555},
    {R_WM_COLORFMT_ARGB4444,        R_GFXUTILS_FORMAT_ARGB4444},
    {R_WM_COLORFMT_RGB0888,         R_GFXUTILS_FORMAT_RGB0888},
    {R_WM_COLORFMT_ARGB8888,        R_GFXUTILS_FORMAT_ARGB8888},
    {R_WM_COLORFMT_RGBA5551,        R_GFXUTILS_FORMAT_RGBA5551},
    {R_WM_COLORFMT_RGBA8888,        R_GFXUTILS_FORMAT_RGBA8888},
    {R_WM_COLORFMT_CLUT8,           R_GFXUTILS_FORMAT_CLUT8},
    {R_WM_COLORFMT_CLUT4,           R_GFXUTILS_FORMAT_CLUT4},
    {R_WM_COLORFMT_CLUT1,           R_GFXUTILS_FORMAT_CLUT1},
    {R_WM_COLORFMT_YUV_YUYV,        R_GFXUTILS_FORMAT_YUV_YUYV},
    {R_WM_COLORFMT_YUV_UYVY,        R_GFXUTILS_FORMAT_YUV_UYVY},
    {R_WM_COLORFMT_YUV_YVYU,        R_GFXUTILS_FORMAT_YUV_YVYU},
    {R_WM_COLORFMT_YUV_VYUY,        R_GFXUTILS_FORMAT_YUV_VYUY},
    {R_WM_COLORFMT_YCBCR_422,       R_GFXUTILS_FORMAT_YCBCR_422},
    {R_WM_COLORFMT_YCBCR_444,       R_GFXUTILS_FORMAT_YCBCR_444},
        
    /* Not available in r_gfxutils_Format_t */
    {R_WM_COLORFMT_RGB0444,         R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RGBA4444,        R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE24ARGB8888,   R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE18ARGB8888,   R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE24RGB0888,    R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE18RGB0888,    R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE8CLUT8,       R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE8CLUT4,       R_GFXUTILS_FORMAT_UNSUPPORTED},
    {R_WM_COLORFMT_RLE8CLUT1,       R_GFXUTILS_FORMAT_UNSUPPORTED},

    /* Not available in r_wm_WinColorFmt_t */
    {R_WM_COLORFMT_LAST, R_GFXUTILS_FORMAT_RGB666},
};
#endif /* USE_WM */

#ifdef USE_VDCO
typedef struct {
    r_vdco_Format_t     VdcoFmt;
    r_gfxutils_Format_t GfxutilsFmt;
} Vdco2InternalFmt_t;

Vdco2InternalFmt_t Vdco2InternalFmtTable[] = {
    {R_VDCO_RGB565,      R_GFXUTILS_FORMAT_RGB565},
    {R_VDCO_RGB0888,     R_GFXUTILS_FORMAT_RGB0888},
    {R_VDCO_ARGB1555,    R_GFXUTILS_FORMAT_ARGB1555},
    {R_VDCO_ARGB4444,    R_GFXUTILS_FORMAT_ARGB4444},
    {R_VDCO_ARGB8888,    R_GFXUTILS_FORMAT_ARGB8888},
    {R_VDCO_CLUT8,       R_GFXUTILS_FORMAT_CLUT8},
    {R_VDCO_CLUT4,       R_GFXUTILS_FORMAT_CLUT4},
    {R_VDCO_CLUT1,       R_GFXUTILS_FORMAT_CLUT1},
    {R_VDCO_YCBCR_422,   R_GFXUTILS_FORMAT_YCBCR_422},
    {R_VDCO_YCBCR_444,   R_GFXUTILS_FORMAT_YCBCR_444},
    {R_VDCO_RGBA5551,    R_GFXUTILS_FORMAT_RGBA5551},
    {R_VDCO_RGBA8888,    R_GFXUTILS_FORMAT_RGBA8888},
    {R_VDCO_YUV_YUYV,    R_GFXUTILS_FORMAT_YUV_YUYV},
    {R_VDCO_YUV_UYVY,    R_GFXUTILS_FORMAT_YUV_UYVY},
    {R_VDCO_YUV_YVYU,    R_GFXUTILS_FORMAT_YUV_YVYU},
    {R_VDCO_YUV_VYUY,    R_GFXUTILS_FORMAT_YUV_VYUY},
    {R_VDCO_RGB666,      R_GFXUTILS_FORMAT_RGB666},
    {R_VDCO_FORMAT_LAST, R_GFXUTILS_FORMAT_UNSUPPORTED},
};
#endif /* USE_VDCO */

#ifdef USE_VDCE
typedef struct {
    r_vdce_Format_t     VdceFmt;
    r_gfxutils_Format_t GfxutilsFmt;
} Vdce2InternalFmt_t;

Vdce2InternalFmt_t Vdce2InternalFmtTable[] = {
    {R_VDCE_RGB565,      R_GFXUTILS_FORMAT_RGB565},
    {R_VDCE_RGB0888,     R_GFXUTILS_FORMAT_RGB0888},
    {R_VDCE_ARGB1555,    R_GFXUTILS_FORMAT_ARGB1555},
    {R_VDCE_ARGB4444,    R_GFXUTILS_FORMAT_ARGB4444},
    {R_VDCE_ARGB8888,    R_GFXUTILS_FORMAT_ARGB8888},
    {R_VDCE_CLUT8,       R_GFXUTILS_FORMAT_CLUT8},
    {R_VDCE_CLUT4,       R_GFXUTILS_FORMAT_CLUT4},
    {R_VDCE_CLUT1,       R_GFXUTILS_FORMAT_CLUT1},
    {R_VDCE_YCBCR_422,   R_GFXUTILS_FORMAT_YCBCR_422},
    {R_VDCE_YCBCR_444,   R_GFXUTILS_FORMAT_YCBCR_444},
    {R_VDCE_RGBA5551,    R_GFXUTILS_FORMAT_RGBA5551},
    {R_VDCE_RGBA8888,    R_GFXUTILS_FORMAT_RGBA8888},
    {R_VDCE_YUV_YUYV,    R_GFXUTILS_FORMAT_YUV_YUYV},
    {R_VDCE_YUV_UYVY,    R_GFXUTILS_FORMAT_YUV_UYVY},
    {R_VDCE_YUV_YVYU,    R_GFXUTILS_FORMAT_YUV_YVYU},
    {R_VDCE_YUV_VYUY,    R_GFXUTILS_FORMAT_YUV_VYUY},
    {R_VDCE_FORMAT_LAST, R_GFXUTILS_FORMAT_UNSUPPORTED},
};
#endif /* USE_VDCE */


/*******************************************************************************
  Section: Local Functions
*/

/* list of function headers */


/*******************************************************************************
  Section: Global Functions
*/

#ifdef USE_WM

/*******************************************************************************
  Function: R_GFXUTILS_Convert_SurfaceFromWmWindow
*/
r_gfxutils_Surface_t R_GFXUTILS_Surface_Wm2Internal(uint32_t Unit, r_wm_Window_t *Window)
{
    r_gfxutils_Surface_t Surface = {0};
    
    /* crash/stop here for unsupported surface format */
    R_GFXUTILS_ASSERT(R_WM_WINMODE_FB == Window->Mode);
    

    Surface.Height  = Window->Height;
    Surface.Width   = Window->Width;
    Surface.Stride  = Window->Pitch;
    Surface.Format  = R_GFXUTILS_Format_Wm2Internal(Window->ColorFmt);
    Surface.BPP     = R_GFXUTILS_Format2BPP(Surface.Format);
    Surface.Size    = (Surface.Height * Surface.Stride) * (Surface.BPP / 8u);
    
    /* Request current drawing buffer from WM */
    Surface.Base    = (uint32_t)R_WM_WindowCurrentDrawBufGet(Unit, Window);
    /* If there is no current drawing buffer yet, request a new one */
    if (0 == Surface.Base)
    {
        Surface.Base    = (uint32_t)R_WM_WindowNewDrawBufGet(Unit, Window);
    }
    
    return Surface;
}

/*******************************************************************************
  Function: R_GFXUTILS_Surface_WmSprite2Internal
*/
r_gfxutils_Surface_t R_GFXUTILS_Surface_WmSprite2Internal(uint32_t Unit, r_wm_Sprite_t *Sprite, uint32_t SpriteNo)
{
    r_gfxutils_Surface_t Surface = {0};
    
    /* crash/stop here for unsupported surface format */
    R_GFXUTILS_ASSERT(0 != Sprite->Window);
    R_GFXUTILS_ASSERT(R_WM_WINMODE_SPRITES == Sprite->Window->Mode);
    
    /* Get sprite no */
    for (;SpriteNo>0;SpriteNo--)
    {
        R_GFXUTILS_ASSERT(0 != Sprite->Next);
        Sprite = Sprite->Next;
    }
    
    Surface.Height  = Sprite->Height;
    Surface.Width   = Sprite->Width;
    Surface.Stride  = Sprite->Width;
    Surface.Format  = R_GFXUTILS_Format_Wm2Internal(Sprite->Window->ColorFmt);
    Surface.BPP     = R_GFXUTILS_Format2BPP(Surface.Format);
    Surface.Size    = (Surface.Height * Surface.Stride) * (Surface.BPP / 8u);
    
    /* This might be zero! */
    Surface.Base    = (uint32_t)Sprite->Data;
    
    return Surface;
}

/*******************************************************************************
  Function: R_GFXUTILS_Format_Wm2Internal
*/
r_gfxutils_Format_t R_GFXUTILS_Format_Wm2Internal(r_wm_WinColorFmt_t ColorFmt) 
{
    r_gfxutils_Format_t Format;
    uint32_t tableEntries = sizeof(Wm2InternalFmtTable) / sizeof(Wm2InternalFmt_t);
    uint32_t i = 0;
    
    /* Search Format */
    for (i = 0; i<tableEntries; i++)
    {
        if ( Wm2InternalFmtTable[i].WmFmt == ColorFmt )
        {
            Format = Wm2InternalFmtTable[i].GfxutilsFmt;
            break;
        }
    }
    /* Not found */
    if ((i == tableEntries) || (R_GFXUTILS_FORMAT_UNSUPPORTED == Format))
    {
        /* Stop here for unsupported format. Below code is equivalent to ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(ColorFmt == R_WM_COLORFMT_RGB565);
    }
    
    return Format;
}

/*******************************************************************************
  Function: R_GFXUTILS_Format_Internal2Wm
*/
r_wm_WinColorFmt_t R_GFXUTILS_Format_Internal2Wm(r_gfxutils_Format_t ColorFmt) 
{
    r_wm_WinColorFmt_t Format;
    uint32_t tableEntries = sizeof(Wm2InternalFmtTable) / sizeof(Wm2InternalFmt_t);
    uint32_t i = 0;
    
    /* Search Format */
    for (i = 0; i<tableEntries; i++)
    {
        if ( Wm2InternalFmtTable[i].GfxutilsFmt == ColorFmt )
        {
            Format = Wm2InternalFmtTable[i].WmFmt;
            break;
        }
    }
    /* Not found */
    if ((i == tableEntries) || (R_WM_COLORFMT_LAST == Format))
    {
        /* Stop here for unsupported format. Below code is equivalent to ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(ColorFmt == R_GFXUTILS_FORMAT_RGB565);
    }
    
    return Format;
}

#endif /* USE_WM */


#ifdef USE_VDCO
/*******************************************************************************
  Function: R_GFXUTILS_Format_Vdco2Internal
*/
r_gfxutils_Format_t R_GFXUTILS_Format_Vdco2Internal(r_vdco_Format_t ColorFmt) 
{
    r_gfxutils_Format_t Format;
    uint32_t tableEntries = sizeof(Vdco2InternalFmtTable) / sizeof(Vdco2InternalFmt_t);
    uint32_t i = 0;
    
    /* Search Format */
    for (i = 0; i<tableEntries; i++)
    {
        if ( Vdco2InternalFmtTable[i].VdcoFmt == ColorFmt )
        {
            Format = Vdco2InternalFmtTable[i].GfxutilsFmt;
            break;
        }
    }
    /* Not found */
    if ((i == tableEntries) || (R_GFXUTILS_FORMAT_UNSUPPORTED == Format))
    {
        /* Stop here for unsupported format. Below code is equivalent to ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(ColorFmt == R_VDCO_RGB565);
    }
    
    return Format;
}


/*******************************************************************************
  Function: R_GFXUTILS_Format_Vdco2Internal
*/
r_vdco_Format_t R_GFXUTILS_Format_Internal2Vdco(r_gfxutils_Format_t ColorFmt) 
{
    r_vdco_Format_t Format;
    uint32_t tableEntries = sizeof(Vdco2InternalFmtTable) / sizeof(Vdco2InternalFmt_t);
    uint32_t i = 0;
    
    /* Search Format */
    for (i = 0; i<tableEntries; i++)
    {
        if ( Vdco2InternalFmtTable[i].GfxutilsFmt == ColorFmt )
        {
            Format = Vdco2InternalFmtTable[i].VdcoFmt;
            break;
        }
    }
    /* Not found */
    if (i == tableEntries)
    {
        /* Stop here for unsupported format. Below code is equivalent to ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(ColorFmt == R_GFXUTILS_FORMAT_RGB565);
    }
    
    return Format;
}
#endif /* USE_VDCO */


#ifdef USE_VDCE
/*******************************************************************************
  Function: R_GFXUTILS_Format_Vdco2Internal
*/
r_gfxutils_Format_t R_GFXUTILS_Format_Vdce2Internal(r_vdce_Format_t ColorFmt) 
{
    r_gfxutils_Format_t Format;
    uint32_t tableEntries = sizeof(Vdce2InternalFmtTable) / sizeof(Vdce2InternalFmt_t);
    uint32_t i = 0;
    
    /* Search Format */
    for (i = 0; i<tableEntries; i++)
    {
        if ( Vdce2InternalFmtTable[i].VdceFmt == ColorFmt )
        {
            Format = Vdce2InternalFmtTable[i].GfxutilsFmt;
            break;
        }
    }
    /* Not found */
    if ((i == tableEntries) || (R_GFXUTILS_FORMAT_UNSUPPORTED == Format))
    {
        /* Stop here for unsupported format. Below code is equivalent to ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(ColorFmt == R_VDCE_RGB565);
    }
    
    return Format;
}


/*******************************************************************************
  Function: R_GFXUTILS_Format_Vdco2Internal
*/
r_vdce_Format_t R_GFXUTILS_Format_Internal2Vdce(r_gfxutils_Format_t ColorFmt) 
{
    r_vdce_Format_t Format;
    uint32_t tableEntries = sizeof(Vdce2InternalFmtTable) / sizeof(Vdce2InternalFmt_t);
    uint32_t i = 0;
    
    /* Search Format */
    for (i = 0; i<tableEntries; i++)
    {
        if ( Vdce2InternalFmtTable[i].GfxutilsFmt == ColorFmt )
        {
            Format = Vdce2InternalFmtTable[i].VdceFmt;
            break;
        }
    }
    /* Not found */
    if ((i == tableEntries) || (R_VDCE_FORMAT_LAST == Format))
    {
        /* Stop here for unsupported format. Below code is equivalent to ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(ColorFmt == R_GFXUTILS_FORMAT_RGB565);
    }
    
    return Format;
}
#endif /* USE_VDCE */


/*******************************************************************************
  Function: R_GFXUTILS_Convert_BPPFromFormat
*/
uint32_t R_GFXUTILS_Format2BPP(r_gfxutils_Format_t Format) 
{
    uint32_t bpp = 0;
    
    switch(Format) 
    {
    case R_GFXUTILS_FORMAT_RGB0888:
    case R_GFXUTILS_FORMAT_ARGB8888:
    case R_GFXUTILS_FORMAT_RGBA8888:
    case R_GFXUTILS_FORMAT_YCBCR_444:
    case R_GFXUTILS_FORMAT_RGB666:
        bpp = 32;
        break;
    case R_GFXUTILS_FORMAT_RGB565:
    case R_GFXUTILS_FORMAT_ARGB1555:
    case R_GFXUTILS_FORMAT_ARGB4444:
    case R_GFXUTILS_FORMAT_YCBCR_422:
    case R_GFXUTILS_FORMAT_RGBA5551:
    case R_GFXUTILS_FORMAT_YUV_YUYV:
    case R_GFXUTILS_FORMAT_YUV_UYVY:
    case R_GFXUTILS_FORMAT_YUV_YVYU:
    case R_GFXUTILS_FORMAT_YUV_VYUY:
        bpp = 16;
        break;
    case R_GFXUTILS_FORMAT_CLUT8:
        bpp = 8;
        break;
    case R_GFXUTILS_FORMAT_CLUT4:
        bpp = 4;
        break;
    case R_GFXUTILS_FORMAT_CLUT1:
        bpp = 1;
        break;
    default:
        /* crash/stop here for unsupported colour format */
        /* here Format is never R_GFXUTILS_FORMAT_ARGB8888, so the below code is equivalent to 
         R_GFXUTILS_ASSERT(0) and MISRA compliant ;) */
        R_GFXUTILS_ASSERT(Format == R_GFXUTILS_FORMAT_ARGB8888);
        break; 
    }
    
    return bpp;
}
