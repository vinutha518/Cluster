/*
****************************************************************************
PROJECT : DMA driver API
FILE    : $Id: r_dma_api.h 3935 2014-09-23 08:52:32Z golczewskim $
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



#ifndef DMA_API_H_
#define DMA_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
  Enum: r_dma_Interrupt_t

  DMAC macro interrupt type. List of all available interrupts for this macro
  There is only one interrupt vector for both DMA interrupts.
  Driver makes a desision, which case occured, according to the
  values in status registers

  Values:
    R_EE_DMAC_INT_DMAC    - Transfer Completed
    R_EE_DMAC_INT_CT      - Counter compare
*/

typedef enum 
{
    R_EE_DMAC_INT_DMA,
    R_EE_DMAC_INT_CT 
} r_dma_Interrupt_t;

/*******************************************************************************
  Enum: r_dma_AddrCntDir_t

  DMA address count direction type

  Members:
  R_DMA_FIXED_T       - Address count fixed
  R_DMA_INCREMENT_T   - Address count increment
*/

typedef enum 
{
    R_DMA_INCREMENT = 0u,
    R_DMA_FIXED     = 2
} r_dma_AddrCntDir_t;

/*******************************************************************************
  Enum: r_dma_TransDataSize_t

  DMA data size  

  Members:
  R_DMA_8BIT_T     - Data size is 8   bit 
  R_DMA_16BIT_T    - Data size is 16  bit 
  R_DMA_32BIT_T    - Data size is 32  bit 
  R_DMA_128BIT_T   - Data size is 128 bit  
  
*/

typedef enum 
{
    R_DMA_8BIT   = 0u,
    R_DMA_16BIT  = 1u,
    R_DMA_32BIT  = 2u,
	R_DMA_64BIT  = 3u,
    R_DMA_128BIT = 4u
} r_dma_TransDataSize_t;

/*******************************************************************************
  Enum: r_dma_TrgType_t

  DMA trigger type

  Members:
  R_DMA_TRG_HW_T   - DMA transfer is controlled by hardware
  R_DMA_TRG_SW_T   - DMA transfer is controlled by software
*/

typedef enum 
{
    R_DMA_TRG_SW  = 0u,
    R_DMA_TRG_HW  = 1
} r_dma_TrgType_t;

/*******************************************************************************
  Enum: r_dma_TransEndStatType_t

  DMA Transfer Status type

  Members:
  R_DMA_TRANS_COMP_T        - DMA transfer completed
  R_DMA_TRANS_NOT_COMP_T    - DMA transfer not completed
*/

typedef enum 
{
    R_DMA_TRANS_COMP      = 0u,
    R_DMA_TRANS_NOT_COMP  = 1
} r_dma_TransEndStatType_t;

/*******************************************************************************
  Enum: r_dma_TransErrStatType_t

  DMA Transfer Error Status

  Members:
  R_DMA_TRANS_ERR_T        - DMA transfer error
  R_DMA_TRANS_NO_ERR_T     - DMA transfer no error
*/

typedef enum 
{
    R_DMA_TRANS_ERR      = 0u,
    R_DMA_TRANS_NO_ERR   = 1
} r_dma_TransErrStatType_t;

/*******************************************************************************
  Type: r_dma_ChCfg_t

  DMA channel property type

  Members:
  Unit             -  DMA unit
  ChannelId        -  DMA channel number in the unit
  SrcAddr          -  Source address
  SrcCS            -  Source CS 1: extern, 2 intern (Code Flash, RAM)   
  DstAddr          -  Destination address
  DstCS            -  Destination CS  1: extern, 2 intern (Code Flash, RAM)  
  TransferCount    -  Transfer count
  TransferDataSize -  Transfer data size 
  SrcAddrCntDir    -  Source address count direction
  DstAddrCntDir    -  Destination address count direction
  LoopEnable       -  0: Clears the DTSnDTE bit upon completion of DMA transfer
                      1: Does not clear the DTSnDTE bit upon completion of DMA transfer.
  DMATriggerFactor -  DMA trigger factor
  InterruptAct     -  1: interrupt enable, 0: interrupt disable
*/

typedef struct {
    uint8_t               Unit;
    uint32_t              ChannelId;
    uint32_t              SrcAddr;
    uint8_t               SrcCS;
    uint32_t              DstAddr;
    uint8_t               DstCS;
    uint32_t              TransferCount;
    r_dma_TransDataSize_t TransferDataSize;
    r_dma_AddrCntDir_t    SrcAddrCntDir;
    r_dma_AddrCntDir_t    DstAddrCntDir;
    uint32_t              LoopEnable;
    uint32_t              DMATriggerFactor;
    uint32_t              InterruptAct;
} r_dma_ChCfg_t;

/***********************************************************
  Enum: r_dma_Error_t

  DMA macro driver error code.

  Values:
  R_DMA_ERR_OK        - No error
  R_DMA_ERR_INSTANCE  - wrong instance (unit) number
  R_DMA_ERR_CHANNEL   - Wrong (not existing) channel number
  R_DMA_ERR_RANGE     - Parameter out of range
  R_DMA_ERR_LAST      - Delimeter 
*/

typedef enum {
    R_DMA_ERR_OK         = 0x00u,
    R_DMA_ERR_INSTANCE,
    R_DMA_ERR_CHANNEL,
    R_DMA_ERR_RANGE,
    R_DMA_ERR_LAST
} r_dma_Error_t;

/*******************************************************************************
  Function: R_DMA_Init

  Initialise DMA register's to default state.

  Parameters:
  Unit       - Instance number
 
  Returns:
  see: <r_dma_Error_t>
*/
void R_DMA_Init (uint32_t Unit);

/***********************************************************
  Function: R_DMA_SetIsrCallback
  
  Assign a user callback to the DMA interrupt

  Parameters:
  Unit       - Instance number
  Channel    - DMA channel Id
  Isr        - pointer to the callback    
  
  Returns:
  == 0, OK , see <r_dma_Error_t>
  != 0  NG, , see <r_dma_Error_t>

*/
r_dma_Error_t R_DMA_SetIsrCallback( uint32_t Unit, int32_t Channel, void (*Isr)(void)); 

/***********************************************************
  Function: R_DMA_EnableInt

  Enable the interrupt for a given instance and channel 
    
  Parameters:
  Unit       - Instance number
  Channel    - DMA channel Id
  Isr        - pointer to the callback    
  
  Returns:
  == 0, OK , see <r_dma_Error_t>
  != 0  NG, , see <r_dma_Error_t>

*/
r_dma_Error_t R_DMA_EnableInt(uint32_t Unit, int32_t Channel); 

/***********************************************************
  Function: R_DMA_DisableInt

  Disable the interrupt for a given instance and channel 


  Parameters:
  Unit       - Instance number
  Channel    - DMA channel Id
  Isr        - pointer to the callback    
  
  Returns:
  == 0, OK , see <r_dma_Error_t>
  != 0  NG, , see <r_dma_Error_t>

*/
r_dma_Error_t R_DMA_DisableInt(uint32_t Unit, int32_t Channel); 

/***********************************************************
  Function: R_DMA_Isr

  Interrupt service for a given instance and channel 


  Parameters:
  Unit       - Instance number
  Channel    - DMA channel Id
 
  Returns:
  void 

*/
void R_DMA_Isr(uint32_t Unit, int32_t Channel);

/*******************************************************************************
  Function: R_DMA_LoadChannel

  Load DMA channels with the configuration data.

  Parameters:
  ChCfg       - DMA channel configuration
  NoOfCh      - Number of DMA channels need to be configured
  
  Returns:
  void
*/
void R_DMA_LoadChannel(r_dma_ChCfg_t *ChCfg, uint32_t NoOfCh);

/*******************************************************************************
  Function: R_DMA_SwTrigger

  SW trigger.

  Parameters:
  Unit        - DMA unit (normally 0)
  ChId        - DMA channel Id
  
  Returns:
  void
*/
void R_DMA_SwTrigger(uint32_t Unit, uint32_t ChId);


/*******************************************************************************
  Function: R_DMA_TransEndStat

  DMA transfer status.

  Parameters:
  Unit        - DMA unit (normally 0)
  ChId        - DMA channel Id

  Returns: 
  Transfer status , see <r_dma_TransEndStatType_t> 
*/
r_dma_TransEndStatType_t R_DMA_TransEndStat(uint32_t Unit, uint32_t ChId);

/*******************************************************************************
  Function: R_DMA_TransErrStat

  DMA transfer status.

  Parameters:
  Unit        - DMA unit (normally 0)
  ChId        - DMA channel Id
  Returns: 

  Transfer status , see <r_dma_TransEndStatType_t> 
*/
r_dma_TransErrStatType_t R_DMA_TransErrStat(uint32_t Unit, uint32_t ChId);

#ifdef __cplusplus
}
#endif

#endif /* DMA_API_H_  */

