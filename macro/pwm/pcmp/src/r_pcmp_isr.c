/*
****************************************************************************
PROJECT : PCMP driver
FILE ID : $Id: r_pcmp_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
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


/***********************************************************
  Title: PCM Interrupt Functions

  Handle the PCM interrupts.
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_config_pcmp.h"
#include "r_pcmp_api.h"
#include "r_pcmp_main.h"
#include "r_pcmp_regs.h"
#include "r_pcmp_sys.h"

#include "r_dev_api.h"

/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Section: Global API Functions

  For comments see file r_pcmp_api.h
*/

/***********************************************************
  Function: R_PCMP_SetIsrCallback
*/
r_pcmp_Error_t R_PCMP_SetIsrCallback(uint32_t Unit, r_pcmp_Interrupt_t IntType,
                                   void (*Isr)(void))
{
    if (Unit >= R_PCMP_MACRO_NUM) {
        return R_PCMP_ERR_RANGE;
    } else {
        R_PCMP_Dev[Unit].IsrCallback[IntType] = Isr;
    }
    return R_PCMP_ERR_OK;
}


/***********************************************************
  Function: R_PCMP_EnableInt

*/
r_pcmp_Error_t R_PCMP_EnableInt(uint32_t Unit, r_pcmp_Interrupt_t Interrupt)
{
    /* Check range */
    if ((Unit >= R_PCMP_MACRO_NUM) || (Interrupt >= R_PCMP_INT_NUM))
    {
        return R_PCMP_ERR_RANGE;
    }
    R_SYS_PCMP_EnableInt(Unit, Interrupt);

    return R_PCMP_ERR_OK;
}


/***********************************************************
  Function: R_PCMP_DisableInt

*/
r_pcmp_Error_t R_PCMP_DisableInt(uint32_t Unit, r_pcmp_Interrupt_t Interrupt)
{
    /* Check range */
    if ((Unit >= R_PCMP_MACRO_NUM) || (Interrupt >= R_PCMP_INT_NUM))
    {
        return R_PCMP_ERR_RANGE;
    }
    R_SYS_PCMP_DisableInt(Unit, Interrupt);

    return R_PCMP_ERR_OK;
}


/***********************************************************
  Function: R_PCMP_Isr
*/

void R_PCMP_Isr(uint32_t Unit, r_pcmp_Interrupt_t Interrupt)
{
    if (R_PCMP_Dev[Unit].IsrCallback[Interrupt] != R_NULL) 
    {
        R_PCMP_Dev[Unit].IsrCallback[Interrupt]();
    }
}

