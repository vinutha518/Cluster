/*
****************************************************************************
PROJECT : DMA driver
FILE    : $Id: r_dma_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================
DESCRIPTION
Driver for DMA macro
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

#include "r_typedefs.h"
#include "r_cdi_api.h"
#include "r_dma_regs.h"
#include "r_dma_api.h"
#include "r_dma_sys.h"
#include "r_config_dma.h"
#define DMA_MAIN_GLOBALS_
#include "r_dma_main.h"


/*******************************************************************************
  Section: Global API Functions
*/


/*******************************************************************************
  Function: R_DMA_Init
  
  see: <r_dma_api.h> for details
*/
void R_DMA_Init (uint32_t Unit)
{
    uint32_t BaseAddr;
    uint32_t DTFRBaseAddr;
    uint32_t cnt;
    volatile uint32_t dummy;
    uint32_t loc_NoChannels;


    loc_NoChannels  = R_SYS_DMA_GetNoOfChannels(Unit);
    DTFRBaseAddr    = R_SYS_DMA_GetDtfrBaseAddr(Unit);
    BaseAddr        = R_SYS_DMA_GetDmaBaseAddr(Unit, 0);
    /* configure DMA transfer request control register 0 */
    R_DMA_WRITE_REG(8, BaseAddr + R_DMA_DTRC0_OFFSET, R_DMA_DTRC0_DEFAULT);
    /* configure DMA channel master setting register 0 */
    R_DMA_WRITE_REG(16, BaseAddr + R_DMA_DMCM0_OFFSET, R_DMA_DMCM0_DEFAULT);

    if(Unit < R_DMA_MACRO_NUM)
    {
        for(cnt = 0u; cnt < loc_NoChannels; cnt++)
        {
            BaseAddr = R_SYS_DMA_GetDmaBaseAddr(Unit, cnt);
            
            /* configure DMA source address register */
            R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DSA_OFFSET), R_DMA_DSA_DEFAULT);
            R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DSC_OFFSET), R_DMA_DSC_DEFAULT);
            /* configure DMA destination address register */
            R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DDA_OFFSET), R_DMA_DDA_DEFAULT);
            R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DDC_OFFSET), R_DMA_DDC_DEFAULT);
            /* configure DMA transfer count register */
            R_DMA_WRITE_REG(16, (BaseAddr + R_DMA_DTC_OFFSET), R_DMA_DTC_DEFAULT);
            /* configure DMA transfer control register */
            R_DMA_WRITE_REG(16, (BaseAddr + R_DMA_DTCT_OFFSET), R_DMA_DTCT_DEFAULT);
            /* configure DMA transfer status register */
            /* The status register MUST be read first, before the status can be cleared */
            dummy = R_DMA_READ_REG(8, BaseAddr + R_DMA_DTS_OFFSET);
            if (R_DMA_DTS_DEFAULT != dummy) 
            {
                R_DMA_WRITE_REG(8, (BaseAddr + R_DMA_DTS_OFFSET), R_DMA_DTS_DEFAULT);
            }
            /* configure DMA trigger factor register */
            R_DMA_WRITE_REG(16, (DTFRBaseAddr + R_DMA_DTFR_OFFSET)+(cnt*R_DTFR_CHANNEL_OFFSET), R_DMA_DTFR_DEFAULT);
            /* Disable All DMA interrupt */
            R_SYS_DMA_DisableInt(Unit, cnt);
        }
        /* DMA request clear register */
        R_DMA_WRITE_REG(16, (DTFRBaseAddr + R_DMA_DRQCLR_OFFSET), R_DMA_DRQCLR_DEFAULT);
    }
}


/*******************************************************************************
  Function: R_DMA_LoadChannel
  
  see: <r_dma_api.h> for details
*/
void R_DMA_LoadChannel(r_dma_ChCfg_t *ChCfg, uint32_t NoOfCh)
{
    uint32_t           cnt;
    uint32_t           ChId;
    eeDma_RegDtct_t    RegDtct;
    eeDma_RegDts_t     RegDts;
    uint32_t           BaseAddr;
    uint32_t           DTFRBaseAddr;
    uint32_t           Unit;
    
    for(cnt = 0u; cnt < NoOfCh; cnt++)
    {
        RegDtct.Word = 0u;
        RegDts.Byte  = 0u;
        Unit         = ChCfg[cnt].Unit;
        DTFRBaseAddr = R_SYS_DMA_GetDtfrBaseAddr(Unit);
        ChId = ChCfg[cnt].ChannelId;
        BaseAddr = R_SYS_DMA_GetDmaBaseAddr(Unit, ChId);
        /* configure DMA source address register */
        R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DSA_OFFSET), (ChCfg[cnt].SrcAddr & 0x1FFFFFFFu));
        R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DSC_OFFSET), (ChCfg[cnt].SrcCS & 0x3u));
        /* configure DMA destination address register */
        R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DDA_OFFSET), (ChCfg[cnt].DstAddr & 0x1FFFFFFFu));
        R_DMA_WRITE_REG(32, (BaseAddr + R_DMA_DDC_OFFSET), (ChCfg[cnt].DstCS & 0x3u));
        /* configure DMA transfer count register */
        R_DMA_WRITE_REG(16, (BaseAddr + R_DMA_DTC_OFFSET), (ChCfg[cnt].TransferCount&0x7FFFu));
        RegDtct.Bit.Ds   = ChCfg[cnt].TransferDataSize;
        RegDtct.Bit.le   = ChCfg[cnt].LoopEnable;
        RegDtct.Bit.SacM = ChCfg[cnt].SrcAddrCntDir;
        RegDtct.Bit.DacM = ChCfg[cnt].DstAddrCntDir;
        /* configure DMA transfer control register */
        R_DMA_WRITE_REG(16, (BaseAddr + R_DMA_DTCT_OFFSET), RegDtct.Word);
        RegDts.Bit.Dte   = 1u;
        /* configure DMA transfer status register */
        R_DMA_WRITE_REG(8, (BaseAddr + R_DMA_DTS_OFFSET), RegDts.Byte);
        /* configure DMA trigger factor register */
        R_DMA_WRITE_REG(16, 
                        DTFRBaseAddr + R_DMA_DTFR_OFFSET +(ChId * R_DTFR_CHANNEL_OFFSET), 
                        (uint16_t)(ChCfg[cnt].DMATriggerFactor|0x8000u));
        
        if(1 == ChCfg[cnt].InterruptAct)
        {
            /* Enable respective DMA interrupt */
            R_SYS_DMA_EnableInt(Unit, ChId);
        }
    }
}


/*******************************************************************************
  Function: R_DMA_SwTrigger
  
  see: <r_dma_api.h> for details
*/
void R_DMA_SwTrigger(uint32_t Unit, uint32_t ChId)
{
    eeDma_RegDts_t  RegDts;
    uint32_t        BaseAddr;
    if(Unit < R_DMA_MACRO_NUM)
    {
        /*uint32_t        DTFRBaseAddr;*/
        BaseAddr        = R_SYS_DMA_GetDmaBaseAddr(Unit, ChId);
        /*DTFRBaseAddr    = R_SYS_DMA_GetDtfrBaseAddr(Unit);*/
        RegDts.Byte     = R_DMA_READ_REG(8, BaseAddr + R_DMA_DTS_OFFSET);
        RegDts.Bit.Sr   = 1u;
        /* configure DMA transfer status register */
        R_DMA_WRITE_REG(8, (BaseAddr + R_DMA_DTS_OFFSET)+(ChId*R_DMA_CHANNEL_OFFSET), RegDts.Byte);
    }
}


/*******************************************************************************
  Function: R_DMA_TransEndStat
  
  see: <r_dma_api.h> for details
*/
r_dma_TransEndStatType_t R_DMA_TransEndStat(uint32_t Unit, uint32_t ChId)
{
    eeDma_RegDts_t  RegDts;
    uint32_t        BaseAddr;
    r_dma_TransEndStatType_t TransStat = R_DMA_TRANS_NOT_COMP;
    if(Unit < R_DMA_MACRO_NUM)
    {
        /*uint32_t        DTFRBaseAddr;*/
        BaseAddr        = R_SYS_DMA_GetDmaBaseAddr(Unit, ChId);
        /*DTFRBaseAddr    = R_SYS_DMA_GetDtfrBaseAddr(Unit);*/
        RegDts.Byte     = R_DMA_READ_REG(8, BaseAddr + R_DMA_DTS_OFFSET);
        if (1 == RegDts.Bit.Tc)
        {
            TransStat = R_DMA_TRANS_COMP;
        }
        else
        {
            TransStat = R_DMA_TRANS_NOT_COMP;
        }
    }
    return(TransStat);
}


/*******************************************************************************
  Function: R_DMA_TransErrStat
  
  see: <r_dma_api.h> for details
*/
r_dma_TransErrStatType_t R_DMA_TransErrStat(uint32_t Unit, uint32_t ChId)
{
    eeDma_RegDts_t  RegDts;
    uint32_t        BaseAddr;

    r_dma_TransErrStatType_t TransStat = R_DMA_TRANS_ERR;
    if(Unit < R_DMA_MACRO_NUM)
    {
        /*uint32_t        DTFRBaseAddr;*/
        BaseAddr        = R_SYS_DMA_GetDmaBaseAddr(Unit, ChId);
        /*DTFRBaseAddr    = R_SYS_DMA_GetDtfrBaseAddr(Unit);*/
        RegDts.Byte     = R_DMA_READ_REG(8, BaseAddr + R_DMA_DTS_OFFSET);
        if (1 == RegDts.Bit.Er)
        {
            TransStat = R_DMA_TRANS_ERR;
        }
        else
        {
            TransStat = R_DMA_TRANS_NO_ERR;
        }
    }
    return(TransStat);
}
