/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_g3dma_main.c 7171 2016-01-06 17:52:36Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the G3DMA macro
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
  Title: G3DMA Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_g3dma.h"
#include "r_dma_api.h"
#include "r_dma_sys.h"
#include "r_g3dma_api.h"
#include "r_g3dma_regs.h"
#define G3DMA_MAIN_GLOBALS_
#include "r_g3dma_main.h"
#include "r_g3dma_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_G3DMAVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_G3DMADrvVer[] = R_VERSION(G3DMA, R_G3DMA_VERSION_HI, R_G3DMA_VERSION_LO);



/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_G3DMA_CheckMacroParams
  
  See: <g3dma_main.h>
*/
r_g3dma_Error_t R_G3DMA_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_G3DMA_MACRO_NUM <= Unit)
    {
        return  R_G3DMA_ERR_RANGE; 
    }
    return R_G3DMA_ERR_OK;  
}

/*******************************************************************************
  Function: R_G3DMA_ErrorHandler
  
  See: <g3dma_main.h>
*/

void R_G3DMA_ErrorHandler(uint32_t Unit, r_g3dma_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for(;;) 
        {        /* we don't have a handler */
        }        /* so, let's wait here */
    }
}

/*******************************************************************************
  Section: Global API Functions
  
  See: <g3dma_api.h>
*/

/*******************************************************************************
  Function: R_G3DMA_GetVersionStr

  See: <g3dma_api.h> 
*/
const int8_t * R_G3DMA_GetVersionStr(void) 
{
    return (const int8_t *) loc_G3DMADrvVer;
}

/*******************************************************************************
  Function: R_G3DMA_GetMajorVersion

  See: <g3dma_api.h> 
*/
const uint16_t R_G3DMA_GetMajorVersion(void) 
{
    return (uint16_t)R_G3DMA_VERSION_HI; 
}

/*******************************************************************************
  Function: R_G3DMA_GetMinorVersion

  See: <g3dma_api.h> 
*/
const uint16_t R_G3DMA_GetMinorVersion(void) 
{
    return (uint16_t)R_G3DMA_VERSION_LO; 
}



/*******************************************************************************
  Function: R_G3DMA_Init

  See: <g3dma_api.h> 
*/

 r_g3dma_Error_t  R_G3DMA_Init(uint32_t Unit) 
{
    r_g3dma_Error_t err;
    uint32_t        base_addr;
    uint32_t        loc_no_channels;
    uint32_t        cnt;

    loc_no_channels  = R_G3DMA_SYS_GetNoOfChannels(Unit);
    base_addr        = R_G3DMA_SYS_GetDmaBaseAddr(Unit, 0);

    /*** setup overall DMA operations ***/
       /*  TBD (if necessary ;) */
    
    /* clear status */
    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_CMVC_OFFSET), R_DMA_CMVC_DEFAULT);

    /*** setup DMA channel operations ***/
    if (Unit < R_G3DMA_MACRO_NUM)
    {
        for (cnt = 0u; cnt < loc_no_channels; cnt++)
        {
            base_addr = R_G3DMA_SYS_GetDmaBaseAddr(Unit, cnt);

            /* channel setting */
	    /* mandatory */
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DSA_OFFSET),  R_DMA_DSA_DEFAULT);   
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DDA_OFFSET),  R_DMA_DDA_DEFAULT);     
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTC_OFFSET),  R_DMA_DTC_DEFAULT);     
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTCT_OFFSET), R_DMA_DTCT_DEFAULT);     
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DRSA_OFFSET), R_DMA_DRSA_DEFAULT);     
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DRDA_OFFSET), R_DMA_DRDA_DEFAULT);     
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DRTC_OFFSET), R_DMA_DRTC_DEFAULT);     
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTCC_OFFSET), R_DMA_DTCC_DEFAULT);
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTFR_OFFSET), R_DMA_DTFR_DEFAULT);     
	        
	        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCST_OFFSET),   R_DMA_DCST_DEFAULT);
	        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCSTS_OFFSET),  R_DMA_DCSTS_DEFAULT);
	        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTFRRQ_OFFSET), R_DMA_DTFRRQ_DEFAULT);

            /* Disable All DMA interrupt */
            R_G3DMA_SYS_DisableInt(Unit, cnt);

	        /* clear status */
            R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCSTC_OFFSET),   R_DMA_DCSTC_DEFAULT);
	        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTFRRQC_OFFSET), R_DMA_DTFRRQC_DEFAULT);

	        /* channel operation disable */
	        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCEN_OFFSET), R_DMA_DCEN_DEFAULT);     
	    }
    }

    /*** setup error handling ***/
    err = R_G3DMA_CheckMacroParams(Unit); 
    if (R_G3DMA_ERR_OK != err) 
    {
        return err; 
    }

    return err;
}


/*******************************************************************************
  Function: R_G3DMA_DeInit

  See: <g3dma_api.h> 
*/

r_g3dma_Error_t R_G3DMA_DeInit(uint32_t Unit)
{
    r_g3dma_Error_t err;

    err = R_G3DMA_CheckMacroParams(Unit); 

    if (R_G3DMA_ERR_OK != err) 
    {
        return err; 
    }

    return err;
}

/*******************************************************************************
  Function: R_G3DMA_LoadChannel
  
  see: <r_dma_api.h> for details
*/
void R_G3DMA_LoadChannel(r_g3dma_ChCfg_t *ChCfg, uint32_t NoOfCh)
{
    uint32_t cnt;
    uint32_t ch_id;
    uint32_t reg_dtct;
    uint32_t reg_dtfr;
    uint32_t base_addr;
    uint32_t unit;
    
    for (cnt = 0u; cnt < NoOfCh; cnt++)
    {
        reg_dtct  = 0u;
        unit      = ChCfg[cnt].Unit;
        ch_id     = ChCfg[cnt].ChannelId;
        base_addr = R_G3DMA_SYS_GetDmaBaseAddr(unit, ch_id);

	    /* channel operation disable */
	    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCEN_OFFSET), 0x00000000u);     

        /* configure DMA source address register */
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DSA_OFFSET), (ChCfg[cnt].SrcAddr & 0xFFFFFFFFu));

        /* configure DMA destination address register */
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DDA_OFFSET), (ChCfg[cnt].DstAddr & 0xFFFFFFFFu));
        /* unused:
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DDC_OFFSET), (ChCfg[cnt].DstCS & 0x3u));*/

        /* configure DMA transfer count register */
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTC_OFFSET), ChCfg[cnt].TransferCount);

        /* Transfer Count compare value (for the transfer count match interrupt) */
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTCC_OFFSET), (ChCfg[cnt].CountCompare & 0xFFFFFFFFu));

        /* configure DMA transfer control register */
	    reg_dtct = (((uint32_t)(ChCfg[cnt].NextChnCh )       & 0x7u) << 18u) |
	               (((uint32_t)(ChCfg[cnt].ChainMode)        & 0x3u) << 16u) |
	               (((uint32_t)(ChCfg[cnt].LoopEnable)       & 0x1u) << 13u) |
	               (((uint32_t)(ChCfg[cnt].Rel2Mode)         & 0x3u) << 11u) |
	               (((uint32_t)(ChCfg[cnt].Rel1Mode)         & 0x3u) <<  9u) |
	               (((uint32_t)(ChCfg[cnt].DstAddrCntDir)    & 0x3u) <<  7u) |
	               (((uint32_t)(ChCfg[cnt].SrcAddrCntDir)    & 0x3u) <<  5u) |
	               (((uint32_t)(ChCfg[cnt].TransferDataSize) & 0x7u) <<  2u) |
                   ( (uint32_t)(ChCfg[cnt].Mode)             & 0x3u);

        if (0 < ChCfg[cnt].InterruptAct)
	    {
            /* Enable respective DMA interrupt */
            R_G3DMA_SYS_EnableInt(unit, ch_id);
	        /* Enable Transfer Completion Interrupt */
            reg_dtct |= (((uint32_t)(ChCfg[cnt].InterruptAct) & 0x3u) << 14u) ;
	    }
	
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTCT_OFFSET), reg_dtct);

        /* configure DMA trigger factor register, HW DMA disabled */
        reg_dtfr = ((ChCfg[cnt].G3DMATriggerFactor & 0x7Fu) << 1u);  
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTFR_OFFSET), reg_dtfr);
        
	    /* clear status */
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCSTC_OFFSET),   R_DMA_DCSTC_DEFAULT);
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTFRRQC_OFFSET), R_DMA_DTFRRQC_DEFAULT);

	    /* channel operation enable (bit DTE moved from DTS to DCEN)*/
	    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCEN_OFFSET), 0x00000001u);     

    }
}


/*******************************************************************************
  Function: R_G3DMA_SetReloadConfig
  
  see: <r_dma_api.h> for details
*/
void R_G3DMA_SetReloadConfig(uint32_t Unit, uint32_t ChId, 
                          uint32_t RelSrc, uint32_t RelDst, uint32_t RelCnt)
{
    uint32_t base_addr;
    
    base_addr = R_G3DMA_SYS_GetDmaBaseAddr(Unit, ChId);
    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DRSA_OFFSET), RelSrc);
    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DRDA_OFFSET), RelDst);
    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DRTC_OFFSET), RelCnt);
}

/*******************************************************************************
  Function: R_G3DMA_TransEndStat
*/
r_dma_TransEndStatType_t R_G3DMA_TransEndStat(uint32_t Unit, uint32_t ChId)
{
    uint32_t base_addr;
    uint32_t reg_dcst;
    r_dma_TransEndStatType_t val;

    if (Unit < R_G3DMA_MACRO_NUM)
    {
        base_addr = R_G3DMA_SYS_GetDmaBaseAddr(Unit, ChId);

        /* writable bit SR bit moved from DTS to DCSTS.SRS
           Additionaly, SW is set as source of DMA transfer request in DTCT.DRS */
        reg_dcst = R_G3DMA_READ_REG(32, (base_addr + R_DMA_DCST_OFFSET));

        /* check the TC bit */
        if(0 != (reg_dcst & 0x00000010u))
        {
            val = R_DMA_TRANS_COMP;
        }
        else
        {
            val = R_DMA_TRANS_NOT_COMP;            
        }
    }

    return val;
}

/*******************************************************************************
  Function: R_G3DMA_TransErrStat
*/
r_dma_TransErrStatType_t R_G3DMA_TransErrStat(uint32_t Unit, uint32_t ChId)
{
    return R_DMA_TRANS_NO_ERR; 
}


/*******************************************************************************
  Function: R_G3DMA_SwTrigger
  
  see: <r_dma_api.h> for details
*/
void R_G3DMA_SwTrigger(uint32_t Unit, uint32_t ChId)
{
    uint32_t base_addr;
    uint32_t reg_dtct;

    if (Unit < R_G3DMA_MACRO_NUM)
    {
        base_addr = R_G3DMA_SYS_GetDmaBaseAddr(Unit, ChId);

        /* writable bit SR bit moved from DTS to DCSTS.SRS
           Additionaly, SW is set as source of DMA transfer request in DTCT.DRS */
        reg_dtct = R_G3DMA_READ_REG(32, (base_addr + R_DMA_DTCT_OFFSET));
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTCT_OFFSET), (reg_dtct & 0xFBFFFFFFu));     
	    R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DCSTS_OFFSET), 0x00000001u);
    }
}


/*******************************************************************************
  Function: R_G3DMA_HwEnable
  
  see: <r_dma_api.h> for details
*/
void R_G3DMA_HwEnable(uint32_t Unit, uint32_t ChId)
{
    uint32_t base_addr;
    uint32_t reg_dtfr;
    uint32_t reg_dtct;

    if (Unit < R_G3DMA_MACRO_NUM)
    {
        base_addr = R_G3DMA_SYS_GetDmaBaseAddr(Unit, ChId);

        reg_dtfr = R_G3DMA_READ_REG(32, (base_addr + R_DMA_DTFR_OFFSET));

        /* the Hardware DMA transfer source will already be set up so we just need 
           to enable it
        */
        reg_dtfr |= 0x01u; 

        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTFR_OFFSET), reg_dtfr);

        /* Now select the Hardware DMA option, DMAC Transfer Control 
           Register DRS bit
        */
        reg_dtct = R_G3DMA_READ_REG(32, (base_addr + R_DMA_DTCT_OFFSET));
        reg_dtct |= 0x04000000u;
        R_G3DMA_WRITE_REG(32, (base_addr + R_DMA_DTCT_OFFSET), reg_dtct);
    }
}


/*******************************************************************************
  Function: R_G3DMA_SetErrorCallback
*/

void R_G3DMA_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}


