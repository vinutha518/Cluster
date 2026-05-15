/*
****************************************************************************
PROJECT : PCMP driver
FILE ID : $Id: r_pcmp_main.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for PCMP macro
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2014
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


#ifndef PCMP_MAIN_H_
#define PCMP_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

#undef R_EXTERN
#ifndef PCMP_MAIN_GLOBALS_
#define R_EXTERN extern 
#else
#define R_EXTERN
#endif

/***********************************************************
  Section: Global Macros
*/

/***********************************************************
  Section: Global Types
*/

/***********************************************************
  Type: r_pcmp_Dev_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  BaseAddr      -   Basis address
  IsrCallback   -   Pointer to Isr callback functions
*/

typedef struct {
    uint32_t     BaseAddr;
    void (*IsrCallback[R_PCMP_INT_NUM])(void);
} r_pcmp_Dev_t;

/***********************************************************
  Variable: void R_PCMP_Dev

  Array for device instance specific data.
*/

R_EXTERN r_pcmp_Dev_t R_PCMP_Dev[R_PCMP_MACRO_NUM];

/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_PCMP_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  eeUartE_ErrorHandler will call this user function. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <void r_pcmp_Error_t>
  
  Returns:
  void
*/

void R_PCMP_ErrorHandler(uint32_t Unit, r_pcmp_Error_t Error);


#ifdef __cplusplus
}
#endif

#endif /* PCMP_MAIN_H_  */
