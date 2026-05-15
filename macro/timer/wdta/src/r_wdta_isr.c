/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_wdta_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic WDTA driver
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
  Title: WDTA Interrupt Functions

  Handle the WDTA interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_wdta.h"
#include "r_wdta_api.h"
#include "r_wdta_sys.h"
#include "r_wdta_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <wdta_api.h>
*/


/***********************************************************
  Function: R_WDTA_SetIsrCallback
*/

r_wdta_Error_t R_WDTA_SetIsrCallback(uint32_t Unit, r_wdta_Interrupt_t IntType, 
                                   void (*Isr)(void)) {
                                    
    if (Unit >= R_WDTA_MACRO_NUM) {
        return R_WDTA_ERR_RANGE; 
    } else {
        R_WDTA_Dev[Unit].IsrCallback[IntType] = Isr;
    }
    return R_WDTA_ERR_OK;
}

/***********************************************************
  Function: R_WDTA_NmiIsr
*/

void R_WDTA_NmiIsr(uint32_t Unit) {
 
    if (R_WDTA_Dev[Unit].IsrCallback[R_WDTA_INT_NMI] != 0) {
        R_WDTA_Dev[Unit].IsrCallback[R_WDTA_INT_NMI]();
    }
}

/***********************************************************
  Function: R_WDTA_Isr
*/

void R_WDTA_Isr(uint32_t Unit) {
 
    if (R_WDTA_Dev[Unit].IsrCallback[R_WDTA_INT_75_PERCENT] != 0) {
        R_WDTA_Dev[Unit].IsrCallback[R_WDTA_INT_75_PERCENT]();
    }
}

/***********************************************************
  Function: R_WDTA_EnableInt
*/

void R_WDTA_EnableInt(uint32_t Unit) {
    R_SYS_WDTA_EnableInt(Unit);
}


/***********************************************************
  Function: R_WDTA_DisableInt
*/

void R_WDTA_DisableInt(uint32_t Unit) {
    R_SYS_WDTA_DisableInt(Unit);
}


