/*
****************************************************************************
PROJECT : DMA driver
FILE    : $Id: r_sys_dma.c 4630 2015-01-13 16:48:47Z golczewskim $
============================================================================ 
DESCRIPTION
DMA system functions for upd7010352
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


/***********************************************************
  Title: upd7010352 DMA Support Functions

  Implementation of the DMA support functions for the 
  d1x_scit device.
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dma_api.h"
#include "r_g3dma_api.h"
#include "r_dma_sys.h"
#include "r_config_dma.h"
#include "r_dev_api.h"

/***********************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_IntAddress

  Stores the Addresses of the Interrupt control registers for each unit
  The order of the adresses for each interrupt type schould be the same 
  as defined in r_dma_api.
   
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

/***********************************************************
  Section: Global Functions
*/



/***********************************************************
  Function: R_SYS_DMA_GetDmaBaseAddr
*/

uint32_t R_SYS_DMA_GetDmaBaseAddr(uint32_t Unit, uint32_t ChId )
{
    uint32_t addr;    
    if (ChId <= 7)
    {
        addr = (ChId * R_DMA_CHANNEL_OFFSET) + R_DMA_BASE_ADDRESS;
    }
    else
    {
        addr = (ChId * R_DMA_CHANNEL_OFFSET) + R_DMA_CHANNEL_OFFSET_GAP + R_DMA_BASE_ADDRESS;
    }          
    return addr;
}

/***********************************************************
  Function: R_SYS_DMA_GetDtfrBaseAddr
*/

uint32_t R_SYS_DMA_GetDtfrBaseAddr(uint32_t Unit) 
{
    uint32_t addr;    
    addr = R_DMA_DTFR_BASE_ADDRESS;
    return addr;
}

/***********************************************************
  Function: R_SYS_DMA_GetNoOfChannels
*/

uint32_t R_SYS_DMA_GetNoOfChannels(uint32_t Unit) 
{
    uint32_t addr;    
    addr = R_DMA_CHANNEL_NUM;
    return addr;
}

/*******************************************************************************
  Function: R_SYS_DMA_EnableInt
*/
void R_SYS_DMA_EnableInt(uint32_t Unit, int32_t ChId) 
{
    R_DEV_IntEnable(loc_IntAddress[ChId], 1); 
}

/*******************************************************************************
  Function: R_SYS_DMA_DisableInt
*/
void R_SYS_DMA_DisableInt(uint32_t Unit, int32_t ChId) 
{
   R_DEV_IntEnable(loc_IntAddress[ChId], 0); 
}

/*******************************************************************************
  Function: R_DMA_SYS_HardwareInit
  
  see: <r_dma_api.h> for details
*/
r_dma_Error_t R_DMA_SYS_HardwareInit(uint32_t Unit)
{
    return (r_dma_Error_t) R_G3DMA_Init(Unit); 
}


/*******************************************************************************
  Function: R_DMA_SYS_EnableInt
*/
void R_DMA_SYS_EnableInt(uint32_t Unit, uint32_t ChId) 
{
    R_G3DMA_EnableInt(Unit, ChId); 
}

/*******************************************************************************
  Function: R_DMA_SYS_DisableInt
*/
void R_DMA_SYS_DisableInt(uint32_t Unit, uint32_t ChId) 
{
   R_G3DMA_DisableInt(Unit, ChId); 
}


/*******************************************************************************
  Function: R_DMA_SYS_LoadChannel
  
  see: <r_dma_api.h> for details
*/
void R_DMA_SYS_LoadChannel(r_dma_ChCfg_t *ChCfg, uint32_t NoOfCh)
{
  r_g3dma_ChCfg_t own_cfg;   

  own_cfg.Unit           = ChCfg->Unit;
  own_cfg.ChannelId      = ChCfg->ChannelId;   
  own_cfg.SrcAddr	     = ChCfg->SrcAddr;     
  own_cfg.DstAddr	     = ChCfg->DstAddr;     
  own_cfg.TransferCount	     = ChCfg->TransferCount;
  
  /* R_DMA & R_G3DMA data sizes are NOT bit-compatible by default! */
    
  switch (ChCfg->TransferDataSize) {
      case R_DMA_8BIT  : own_cfg.TransferDataSize = R_G3DMA_8BIT_T;   break;
      case R_DMA_16BIT : own_cfg.TransferDataSize = R_G3DMA_16BIT_T;  break;
      case R_DMA_32BIT : own_cfg.TransferDataSize = R_G3DMA_32BIT_T;  break;
      case R_DMA_64BIT : own_cfg.TransferDataSize = R_G3DMA_64BIT_T; break;
      case R_DMA_128BIT: own_cfg.TransferDataSize = R_G3DMA_128BIT_T; break;
  	  default: own_cfg.TransferDataSize = R_G3DMA_8BIT_T; break;
  }
    
  own_cfg.SrcAddrCntDir	     = (r_g3dma_AddrCntDir_t)ChCfg->SrcAddrCntDir;
  own_cfg.DstAddrCntDir	     = (r_g3dma_AddrCntDir_t)ChCfg->DstAddrCntDir;
  own_cfg.LoopEnable	     = ChCfg->LoopEnable;  
  own_cfg.G3DMATriggerFactor = ChCfg->DMATriggerFactor; 
  own_cfg.InterruptAct	     = ChCfg->InterruptAct;
  own_cfg.Mode               = R_G3DMA_MODE_B1; 
  own_cfg.CountCompare       = 0;    /* compare count */
  own_cfg.NextChnCh          = 0;    /* Next Channel in Chain */
  own_cfg.ChainMode          = R_G3DMA_CHAIN_OFF; 
  own_cfg.Rel1Mode           = R_G3DMA_RELOAD_OFF; 
  own_cfg.Rel2Mode           = R_G3DMA_RELOAD_OFF; 


  R_G3DMA_LoadChannel(&own_cfg, NoOfCh); 
}


/*******************************************************************************
  Function: R_DMA_SYS_SwTrigger
  
  see: <r_dma_api.h> for details
*/
void R_DMA_SYS_SwTrigger(uint32_t Unit, uint32_t ChId)
{
    R_G3DMA_SwTrigger(Unit, ChId); 
}


/*******************************************************************************
  Function: R_DMA_SYS_TransEndStat
  
  see: <r_dma_api.h> for details
*/
r_dma_TransEndStatType_t R_DMA_SYS_TransEndStat(uint32_t Unit, uint32_t ChId)
{
    return R_G3DMA_TransEndStat(Unit, ChId);
}

/*******************************************************************************
  Function: R_DMA_SYS_TransErrStat
  
  see: <r_dma_api.h> for details
*/
r_dma_TransErrStatType_t R_DMA_SYS_TransErrStat(uint32_t Unit, uint32_t ChId)
{
    return(R_G3DMA_TransErrStat(Unit, ChId));
}

