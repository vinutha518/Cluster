/*
****************************************************************************
PROJECT : VLIB Macro Driver 
FILE    : $Id: r_wdta_main.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic WDTA driver
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
Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/


#ifndef WDTA_MAIN_H_
#define WDTA_MAIN_H_

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
#ifndef WDTA_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif

/*******************************************************************************
  Section: Global Types
*/

/***********************************************************
  Type: r_wdta_Dev_t

  Device instance specific data.
  
  All instance specific data is kept within that structure.

  Members:
  BaseAddr      - Basis address
  IsrCallback   - Pointer to Isr callback functions
  VacRef        - Expected Ref Value

*/

typedef struct {
    uint32_t    BaseAddr;
    void (*IsrCallback [R_WDTA_INT_NUM])(void);
    uint8_t     VacRef;
} r_wdta_Dev_t;


/***********************************************************
  Variable: R_WDTA_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ r_wdta_Dev_t R_WDTA_Dev[R_WDTA_MACRO_NUM];


/***********************************************************
  Section: Global Functions
*/


/***********************************************************
  Function: R_WDTA_CalculateVacValue

  Central error function. 
  
  Calculates the VAC value from the reference value.
     
  Parameters:
  Unit              - Instance number
  ucVACRefValue     - value to be calculated  
      
  Returns: 
  ucCalculatedValue - recalculated VAC value
*/

uint8_t R_WDTA_CalculateVacValue(uint32_t Unit, uint8_t VacRefValue);



/***********************************************************
  Function: R_WDTA_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  R_WDTA_ErrorHandler will call this user function. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_wdta_Error_t>
  
  Returns:
  void
*/

void R_WDTA_ErrorHandler(uint32_t Unit, r_wdta_Error_t Error);




#ifdef __cplusplus
}
#endif

#endif /* WDTA_MAIN_H_  */
