/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_g3dma_isr.c 2110 2014-03-05 15:01:52Z michael.golczewski $
============================================================================ 
DESCRIPTION
Generic G3DMA driver
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
  Title: G3DMA Interrupt Functions

  Handle the G3DMA interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_g3dma.h"
#include "r_dma_api.h"
#include "r_g3dma_api.h"
#include "r_g3dma_sys.h"
#include "r_g3dma_main.h"

/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <g3dma_api.h>
*/


/*******************************************************************************
  Function: R_G3DMA_EnableInt
*/

r_g3dma_Error_t R_G3DMA_EnableInt(uint32_t Unit, uint32_t ChId) 
{
    r_g3dma_Error_t err; 
    err = R_G3DMA_CheckMacroParams(Unit);
    if (R_G3DMA_ERR_OK != err) 
    {
        R_G3DMA_ErrorHandler(Unit, R_G3DMA_ERR_RANGE); 
        return err; 
    } 
    R_G3DMA_SYS_EnableInt(Unit, ChId); 
    return err; 
}


/*******************************************************************************
  Function: R_G3DMA_DisableInt
*/

r_g3dma_Error_t R_G3DMA_DisableInt(uint32_t Unit, uint32_t ChId) 
{
    r_g3dma_Error_t err; 
    err = R_G3DMA_CheckMacroParams(Unit);
    if (R_G3DMA_ERR_OK != err) 
    {
        R_G3DMA_ErrorHandler(Unit, R_G3DMA_ERR_RANGE); 
        return err; 
    } 
    R_G3DMA_SYS_DisableInt(Unit, ChId); 
    return err; 
}

