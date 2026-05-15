/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_taub_isr.c 2296 2014-04-08 09:07:36Z michael.golczewski $
============================================================================ 
DESCRIPTION
Generic TAUB driver
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
  Title: TAUB Interrupt Functions

  Handle the TAUB interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_taub.h"
#include "r_taub_api.h"
#include "r_taub_sys.h"
#include "r_taub_main.h"

#include "r_gpio_api.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <taub_api.h>
*/


/*******************************************************************************
  Function: R_TAUB_EnableInt
*/

void R_TAUB_EnableInt(uint32_t Unit, r_taub_Interrupt_t Interrupt) 
{
    r_taub_Error_t err; 
    err = R_TAUB_CheckMacroParams(Unit);
    if ((R_TAUB_ERR_OK != err) || (Interrupt >= R_TAUB_INT_NUM)) 
    {
        R_TAUB_ErrorHandler(Unit, R_TAUB_ERR_RANGE); 
        return; 
    } 
    R_TAUB_SYS_EnableInt(Unit, Interrupt); 
}


/*******************************************************************************
  Function: R_TAUB_DisableInt
*/

void R_TAUB_DisableInt(uint32_t Unit, r_taub_Interrupt_t Interrupt) 
{
    r_taub_Error_t err; 
    err = R_TAUB_CheckMacroParams(Unit);
    if ((R_TAUB_ERR_OK != err) || (Interrupt >= R_TAUB_INT_NUM)) 
    {
        R_TAUB_ErrorHandler(Unit, R_TAUB_ERR_RANGE); 
        return; 
    } 
    R_TAUB_SYS_DisableInt(Unit, Interrupt);    
}

/*******************************************************************************
  Function: R_TAUB_SetIsrCallback
*/

 r_taub_Error_t R_TAUB_SetIsrCallback(uint32_t Unit, 
                                              r_taub_Interrupt_t Interrupt,     
                                              void (*Isr)(void)) 
{
    r_taub_Error_t err; 
    err = R_TAUB_CheckMacroParams(Unit);
    if ((R_TAUB_ERR_OK != err) || (Interrupt >= R_TAUB_INT_NUM)) 
    {
        R_TAUB_ErrorHandler(Unit, R_TAUB_ERR_RANGE); 
        return R_TAUB_ERR_RANGE; 
    } 
        
    r_taub_Dev[Unit].IsrCallback[Interrupt] = Isr;
    return R_TAUB_ERR_OK;
}


/***********************************************************
  Function: R_TAUB_Isr
*/

void R_TAUB_Isr(uint32_t Unit, r_taub_Interrupt_t Interrupt) 
{
    if (r_taub_Dev[Unit].IsrCallback[Interrupt] != R_NULL) 
    {
        r_taub_Dev[Unit].IsrCallback[Interrupt]();
    }

}
