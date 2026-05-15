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

#ifndef GFXUTILS_CONVERT_H_
#define GFXUTILS_CONVERT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  USAGE:
  
  To use the functions declared here, make sure to include dependant header 
  files before, then activate the required functions by defining the USE switches.
  
  You can only use converter functions, if the respective driver is available.
  
  Switches:
   * R_GFXUTILS_CONVERT_USE_WM
   * R_GFXUTILS_CONVERT_USE_VDCO
   * R_GFXUTILS_CONVERT_USE_VDCE
   
  Example:
    #include "r_wm_api.h"
    #include "r_gfxutils_api.h"
    #define R_GFXUTILS_CONVERT_USE_WM
    #include "r_gfxutils_convert.h"

*/


#ifdef R_GFXUTILS_CONVERT_USE_WM
/*******************************************************************************
  Function: R_GFXUTILS_Surface_Wm2Internal

  Description:
  Get a GFXUTILS Surfrace from a Window Manager's Window.
  
  Parameters:
  Unit         - Unit of the Window Manager.
  Window       - Window to be converted to a Surface.
  
  Returns      = GFXUTILS Surface struct.
                 No allocation used, no deallocation required.
*/
r_gfxutils_Surface_t 
    R_GFXUTILS_Surface_Wm2Internal  (uint32_t Unit, r_wm_Window_t *Window);

/*******************************************************************************
  Function: R_GFXUTILS_Surface_WmSprite2Internal
*/
r_gfxutils_Surface_t 
    R_GFXUTILS_Surface_WmSprite2Internal (uint32_t Unit, r_wm_Sprite_t *Sprite, uint32_t SpriteNo);

/*******************************************************************************
  Function: R_GFXUTILS_Format_Wm2Internal

  Description:
  Convert WindowManager ColorFormat to internal color format
  
  Parameters:
  ColorFmt     - Color format of the Window Manager.
  
  Returns      = Gfxutils color format.
*/
r_gfxutils_Format_t 
    R_GFXUTILS_Format_Wm2Internal   (r_wm_WinColorFmt_t ColorFmt);
    
/*******************************************************************************
  Function: R_GFXUTILS_Format_Internal2Wm

  Description:
  Convert internal color format to WindowManager ColorFormat.
  
  Parameters:
  ColorFmt     - Color format of gfxutils.
  
  Returns      = Window Manager color format.
*/
r_wm_WinColorFmt_t 
    R_GFXUTILS_Format_Internal2Wm(r_gfxutils_Format_t ColorFmt);

#endif

#ifdef R_GFXUTILS_CONVERT_USE_VDCO
/*******************************************************************************
  Function: R_GFXUTILS_Format_Vdco2Internal

  Description:
  Convert VDCO ColorFormat to internal color format
  
  Parameters:
  ColorFmt     - Color format of the VDCO.
  
  Returns      = Gfxutils color format.
*/
r_gfxutils_Format_t 
    R_GFXUTILS_Format_Vdco2Internal (r_vdco_Format_t ColorFmt);

/*******************************************************************************
  Function: R_GFXUTILS_Format_Internal2Vdco

  Description:
  Convert internal color format to VDCO ColorFormat.
  
  Parameters:
  ColorFmt     - Color format of gfxutils.
  
  Returns      = VDCO color format.
*/
r_vdco_Format_t 
    R_GFXUTILS_Format_Internal2Vdco (r_gfxutils_Format_t ColorFmt);
#endif /* USE_VDCO */

#ifdef R_GFXUTILS_CONVERT_USE_VDCE
/*******************************************************************************
  Function: R_GFXUTILS_Format_Vdce2Internal

  Description:
  Convert VDCE ColorFormat to internal color format
  
  Parameters:
  ColorFmt     - Color format of the VDCE.
  
  Returns      = Gfxutils color format.
*/
r_gfxutils_Format_t 
    R_GFXUTILS_Format_Vdce2Internal (r_vdce_Format_t ColorFmt);

/*******************************************************************************
  Function: R_GFXUTILS_Format_Internal2Vdce

  Description:
  Convert internal color format to VDCE ColorFormat.
  
  Parameters:
  ColorFmt     - Color format of gfxutils.
  
  Returns      = VDCE color format.
*/
r_vdce_Format_t 
    R_GFXUTILS_Format_Internal2Vdce (r_gfxutils_Format_t ColorFmt);
#endif /* USE_VDCE */


/*******************************************************************************
  Function: R_GFXUTILS_Format2BPP
  
  Description:
  Get the BitsPerPixels for the supplied color format
  
  Parameters:
  ColorFmt     - Color format of gfxutils.
  
  Returns      = Bits per pixel.
*/
uint32_t 
    R_GFXUTILS_Format2BPP           (r_gfxutils_Format_t Format);


#ifdef __cplusplus
}
#endif
#endif /* GFXUTILS_CONVERT_H_ */

