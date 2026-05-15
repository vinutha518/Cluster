/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_lcbi_isr.c 7329 2016-01-22 10:40:45Z matthias.nippert $
============================================================================ 
DESCRIPTION
Generic LCBI driver
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


/*******************************************************************************
  Title: LCBI Interrupt Functions

  Handle the LCBI interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_lcbi.h"
#include "r_ddb_api.h"
#include "r_lcbi_api.h"
#include "r_lcbi_sys.h"
#include "r_lcbi_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <lcbi_api.h>
*/


/*******************************************************************************
  Function: R_LCBI_EnableInt
*/

void R_LCBI_EnableInt(uint32_t Unit, r_lcbi_Interrupt_t Interrupt) 
{
    r_lcbi_Error_t err; 
    err = R_LCBI_CheckMacroParams(Unit);
    if ((R_LCBI_ERR_OK != err) || (Interrupt >= R_LCBI_INT_NUM)) 
    {
        R_LCBI_ErrorHandler(Unit, R_LCBI_ERR_RANGE); 
        return; 
    } 
    R_LCBI_SYS_EnableInt(Unit, Interrupt); 
}


/*******************************************************************************
  Function: R_LCBI_DisableInt
*/

void R_LCBI_DisableInt(uint32_t Unit, r_lcbi_Interrupt_t Interrupt) 
{
    r_lcbi_Error_t err; 
    err = R_LCBI_CheckMacroParams(Unit);
    if ((R_LCBI_ERR_OK != err) || (Interrupt >= R_LCBI_INT_NUM)) 
    {
        R_LCBI_ErrorHandler(Unit, R_LCBI_ERR_RANGE); 
        return; 
    } 
    R_LCBI_SYS_DisableInt(Unit, Interrupt);    
}

/*******************************************************************************
  Function: R_LCBI_SetIsrCallback
*/

r_lcbi_Error_t R_LCBI_SetIsrCallback(uint32_t Unit, 
                                              r_lcbi_Interrupt_t Interrupt,     
                                              void (*Isr)(void)) 
{
    r_lcbi_Error_t err; 
    err = R_LCBI_CheckMacroParams(Unit);
    if ((R_LCBI_ERR_OK != err) || (Interrupt >= R_LCBI_INT_NUM)) 
    {
        R_LCBI_ErrorHandler(Unit, R_LCBI_ERR_RANGE); 
        return R_LCBI_ERR_RANGE; 
    } 
        
    r_lcbi_Dev[Unit].IsrCallback[Interrupt] = Isr;
    return R_LCBI_ERR_OK;
}


/************************************************************
  Function: R_LCBI_Isr
*/

void R_LCBI_Isr(uint32_t Unit, r_lcbi_Interrupt_t Interrupt)
{
    if (r_lcbi_Dev[Unit].IsrCallback[Interrupt] != R_NULL)
    {
        r_lcbi_Dev[Unit].IsrCallback[Interrupt]();
    }
}

