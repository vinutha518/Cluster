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

#ifndef GFXUTILS_SAVEIMAGE_H_
#define GFXUTILS_SAVEIMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif
 
/*******************************************************************************
  Title: r_gfxutils_saveimage.h

  .
*/

/*******************************************************************************
  Function: R_GFXUTILS_WriteBitmapFromSurface

  Description:
  Saves a BMP from provided <Surface> as <Name> into Multi's current target directory
  
  Parameters:
  Name         - Name of the BMP file including extension
  Surface      - Surface to be saved to HDD.
  
  Returns      =  0: On Success
                 -1: On Error
*/
int32_t R_GFXUTILS_WriteBitmapFromSurface(char *Name, r_gfxutils_Surface_t  *Surface);


/* YET EXPERIMENTAL, 
   Set globally, e.g. in Makefile or Project-Settings */
#ifdef USE_GFXUTILS_PNG

/*******************************************************************************
  Function: R_GFXUTILS_WritePNGFromSurface

  Description:
  Saves a PNG from provided <Surface> as <Name> into Multi's current target directory
  
  Parameters:
  Name         - Name of the PNG file including extension
  Surface      - Surface to be saved to HDD.
  
  Returns      =  0: On Success
                 >0: On Error
*/
  uint32_t R_GFXUTILS_WritePNGFromSurface(char *Name, r_gfxutils_Surface_t  *Surface);


/*******************************************************************************
  Function: R_GFXUTILS_SetPNGAllocator_Default

  Description:
  The PNG library needs to allocate memory for image compression.
  Setting the Default allocator will use malloc/realloc/free.
  
  Parameters:
  Nothing
  
  Returns      = Nothing
*/
  void R_GFXUTILS_SetPNGAllocator_Default(void);

  #ifdef USE_CDI
/*******************************************************************************
  Function: R_GFXUTILS_SetPNGAllocator_CDI

  Description:
  The PNG library needs to allocate memory for image compression.
  Setting the CDI allocator will use R_CDI_Alloc/R_CDI_Free.
  
  Parameters:
  Cdi_Heap     - CDI Heap to be used for allocation
  
  Returns      = Nothing
*/
  void R_GFXUTILS_SetPNGAllocator_CDI(r_cdi_Heap_t * Cdi_Heap);
  #endif

  #ifdef USE_GFXUTILS
/*******************************************************************************
  Function: R_GFXUTILS_SetPNGAllocator_GFXUTILS

  Description:
  The PNG library needs to allocate memory for image compression.
  Setting the GFXUTILS allocator will use R_GFXUTILS_Alloc.
  
  Parameters:
  Cdi_Heap     - CDI Heap to be used for allocation
  
  Returns      = Nothing
*/
  void R_GFXUTILS_SetPNGAllocator_GFXUTILS(void * Gfxutils_Heap);
  #endif

#endif /* USE_GFXUTILS_PNG */



#ifdef __cplusplus
}
#endif
#endif /* GFXUTILS_SAVEIMAGE_H_ */
