/*
****************************************************************************
PROJECT : AWOT driver
FILE    : $Id: r_awot_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for AWOT macro
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


/*******************************************************************************
  Title: AWOT Interrupt Functions

  Handle the AWOT interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_awot_api.h"

#include "r_awot_sys.h"
#include "r_config_awot.h"
#include "r_awot_regs.h"
#include "r_awot_main.h"

/*******************************************************************************
  Section: Global Functions
*/
  

/*******************************************************************************
  Section: Global API Functions
  
  For API documentation see file awot_api.h
*/

/*******************************************************************************
  Function: R_AWOT_SetIsrCallback
*/

 r_awot_Error_t R_AWOT_SetIsrCallback(uint32_t Unit, r_awot_Interrupt_t IntType, 
                                      void (*Isr)(void)) 
{
                                    
    if (Unit >= R_AWOT_MACRO_NUM) {
        return R_AWOT_ERR_RANGE; 
    } 

    R_AWOT_Dev[Unit].IsrCallback[IntType] = Isr;
    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_TintIsr
*/

void R_AWOT_TintIsr(uint32_t Unit) {
    /* Applications interrupt handling */
    if (R_AWOT_Dev[Unit].IsrCallback[R_AWOT_INT_TINT] != 0u) {
        R_AWOT_Dev[Unit].IsrCallback[R_AWOT_INT_TINT](); 
    }
}

