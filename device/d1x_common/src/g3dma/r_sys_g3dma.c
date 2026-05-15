/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_g3dma.c 4630 2015-01-13 16:48:47Z golczewskim $
============================================================================ 
DESCRIPTION
G3DMA system functions for d1x_scit
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
  Title: dev_g3dma G3DMA device functions

  Implementation of the G3DMA support functions for the 
  device d1x_scit.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_dma_api.h"
#include "r_g3dma_api.h"
#include "r_g3dma_sys.h"
#include "r_config_g3dma.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif



/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_IntSel_t loc_IntAddress[R_DMA_CHANNEL_NUM] = 
{
    R_DEV_INT_DMA0,
    R_DEV_INT_DMA1,
    R_DEV_INT_DMA2,
    R_DEV_INT_DMA3,
    R_DEV_INT_DMA4,
    R_DEV_INT_DMA5,
    R_DEV_INT_DMA6,
    R_DEV_INT_DMA7,
}; 

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_G3DMA_SYS_BaseAddr
  
  See: r_g3dma_sys.h for details
*/

uint32_t R_G3DMA_SYS_BaseAddr(uint32_t Unit) 
{
        return R_G3DMA_BASE;
}

/*******************************************************************************
  Function: R_G3DMA_SYS_HardwareInit
  
  See: r_g3dma_sys.h for details
*/

r_g3dma_Error_t R_G3DMA_SYS_HardwareInit(uint32_t Unit) 
{
    return R_G3DMA_ERR_OK;
}
/*******************************************************************************
  Function: R_G3DMA_SYS_HardwareDeInit
  
  See: r_g3dma_sys.h for details
*/

r_g3dma_Error_t R_G3DMA_SYS_HardwareDeInit(uint32_t Unit) 
{
    return R_G3DMA_ERR_OK;
}

/*******************************************************************************
  Function: R_G3DMA_SYS_EnableInt
*/

void R_G3DMA_SYS_EnableInt(uint32_t Unit, uint32_t ChId) 
{
    switch (Unit)
    {
    case 0:
        /* check ChId ?! */
        /**/
        R_DEV_IntEnable(loc_IntAddress[ChId], 1); 
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "G3DMA: We do not have this Unit");      
        break; 
    }    
}


/*******************************************************************************
  Function: R_G3DMA_SYS_DisableInt
*/

void R_G3DMA_SYS_DisableInt(uint32_t Unit, uint32_t ChId) 
{
    switch (Unit)
    {
    case 0:
        /* check ChId ?! */
        /**/
        R_DEV_IntEnable(loc_IntAddress[ChId], 1); 
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "G3DMA: We do not have this Unit");      
        break; 
   }    
}

/***********************************************************
  Function: R_G3DMA_SYS_GetDmaBaseAddr
*/

uint32_t R_G3DMA_SYS_GetDmaBaseAddr(uint32_t Unit, uint32_t ChId )
{
    uint32_t addr;    

    addr = (ChId * R_DMA_CHANNEL_OFFSET) + R_G3DMA_BASE;
    return addr;
}

/***********************************************************
  Function: R_G3DMA_SYS_GetNoOfChannels
*/

uint32_t R_G3DMA_SYS_GetNoOfChannels(uint32_t Unit) 
{
    uint32_t addr;    
    addr = R_DMA_CHANNEL_NUM;
    return addr;
}



