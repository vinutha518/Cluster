/*
****************************************************************************
PROJECT : VLIB Macro Driver 
FILE    : $Id: r_sg_main.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic SG driver
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2012
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


#ifndef SG_MAIN_H_
#define SG_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

#undef  DEF_EXTERN_
#ifndef SG_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: R_SG_Dev_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  BasAddr       - Macro register base address
  Locked        - if set, the device is already initialised 
  Settings      - Some settings  -  Just a placeholder/example -  
  Config        - Some config    - don't need to use both 
  IsrCallback   - Pointer table to Isr callback functions
*/

typedef struct {
    
    uint32_t     BaseAddr;
    uint32_t     Pclk;
    uint16_t     Fl;
    uint16_t     Fh;
    uint16_t     Divider;
    uint16_t     ToneFreq;
    uint32_t     PWMFreq;
    void         (*IsrCallback)(void);

} R_SG_Dev_t;


/*******************************************************************************
  Variable: R_SG_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ R_SG_Dev_t R_SG_Dev[R_SG_MACRO_NUM];



/*******************************************************************************
  Function: R_SG_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_sg_Error_t>
  
  Returns:
  void
*/

void R_SG_ErrorHandler(uint32_t Unit, r_sg_Error_t Error);


#ifdef __cplusplus
}
#endif

#endif /* SG_MAIN_H_  */
