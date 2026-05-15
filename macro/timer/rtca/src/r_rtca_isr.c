/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_rtca_isr.c 2385 2014-04-11 15:12:34Z michael.golczewski $
============================================================================ 
DESCRIPTION
Generic RTCA driver
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
  Title: RTCA Interrupt Functions

  Handle the RTCA interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_rtca.h"
#include "r_rtca_api.h"
#include "r_rtca_sys.h"
#include "r_rtca_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <rtca_api.h>
*/


/*******************************************************************************
  Function: R_RTCA_EnableInt
*/

void R_RTCA_EnableInt(uint32_t Unit, r_rtca_Interrupt_t Interrupt) 
{
    r_rtca_Error_t err; 
    err = R_RTCA_CheckMacroParams(Unit);
    if ((R_RTCA_ERR_OK != err) || (Interrupt >= R_RTCA_INT_NUM)) 
    {
        R_RTCA_ErrorHandler(Unit, R_RTCA_ERR_RANGE); 
        return; 
    } 
    R_RTCA_SYS_EnableInt(Unit, Interrupt); 
}


/*******************************************************************************
  Function: R_RTCA_DisableInt
*/

void R_RTCA_DisableInt(uint32_t Unit, r_rtca_Interrupt_t Interrupt) 
{
    r_rtca_Error_t err; 
    err = R_RTCA_CheckMacroParams(Unit);
    if ((R_RTCA_ERR_OK != err) || (Interrupt >= R_RTCA_INT_NUM)) 
    {
        R_RTCA_ErrorHandler(Unit, R_RTCA_ERR_RANGE); 
        return; 
    } 
    R_RTCA_SYS_DisableInt(Unit, Interrupt);    
}

/*******************************************************************************
  Function: R_RTCA_SetIsrCallback
*/

r_rtca_Error_t R_RTCA_SetIsrCallback(uint32_t Unit, r_rtca_Interrupt_t Interrupt,     
                                     void (*Isr)(void)) 
{
    r_rtca_Error_t err; 
    err = R_RTCA_CheckMacroParams(Unit);
    if ((R_RTCA_ERR_OK != err) || (Interrupt >= R_RTCA_INT_NUM)) 
    {
        R_RTCA_ErrorHandler(Unit, R_RTCA_ERR_RANGE); 
        return R_RTCA_ERR_RANGE; 
    } 
        
    r_rtca_Dev[Unit].IsrCallback[Interrupt] = Isr;
    return R_RTCA_ERR_OK;
}


/***********************************************************
  Function: R_RTCA_Isr
*/

void R_RTCA_Isr(uint32_t Unit, r_rtca_Interrupt_t Interrupt) 
{
    if (r_rtca_Dev[Unit].IsrCallback[Interrupt] != R_NULL) 
    {
        r_rtca_Dev[Unit].IsrCallback[Interrupt]();
    }
}
