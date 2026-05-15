/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_sg_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
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


/*******************************************************************************
  Title: SG Interrupt Functions

  Handle the SG interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_sg.h"
#include "r_sg_api.h"
#include "r_sg_sys.h"
#include "r_sg_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <sg_api.h>
*/


/*******************************************************************************
  Function: R_SG_EnableInt
*/

void R_SG_EnableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt) 
{
    
    if (Interrupt >= R_SG_INT_NUM) 
    {
        R_SG_ErrorHandler(Unit, R_SG_ERR_RANGE); 
        return; 
    } 
    R_SYS_SG_EnableInt(Unit, Interrupt); 
}


/*******************************************************************************
  Function: R_SG_DisableInt
*/

void R_SG_DisableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt) 
{

    if (Interrupt >= R_SG_INT_NUM) 
    {
        R_SG_ErrorHandler(Unit, R_SG_ERR_RANGE); 
        return; 
    } 
    R_SYS_SG_DisableInt(Unit, Interrupt);    
}

/*******************************************************************************
  Function: R_SG_SetIsrCallback
*/
    
r_sg_Error_t R_SG_SetIsrCallback(uint32_t Unit, 
                                              r_sg_Interrupt_t Interrupt,     
                                              void (*Isr)(void)) 
{
    if (Interrupt >= R_SG_INT_NUM) 
    {
        R_SG_ErrorHandler(Unit, R_SG_ERR_RANGE); 
        return R_SG_ERR_RANGE; 
    } 
        
    R_SG_Dev[Unit].IsrCallback = Isr;
    return R_SG_ERR_OK;
}

void R_SG_Isr(uint32_t Unit)
{
    if (R_SG_Dev[Unit].IsrCallback!= R_NULL) {
        R_SG_Dev[Unit].IsrCallback();
    }
}


