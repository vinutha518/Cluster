/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_g3dma_api.h 4630 2015-01-13 16:48:47Z golczewskim $
============================================================================ 
DESCRIPTION
Generic part of the macro driver
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

#ifndef G3DMA_API_H_
#define G3DMA_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: G3DMA Driver API 
  
  An application using G3DMA should only need to include r_jcu_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_G3DMA_VERSION_HI and R_G3DMA_VERSION_LO

  Driver version information  
*/

#define R_G3DMA_VERSION_HI 0
#define R_G3DMA_VERSION_LO 1



/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_g3dma_Error_t

  G3DMA macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_G3DMA_ERR_OK           - No error
  R_G3DMA_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_G3DMA_ERR_OK         = 0x00,
    R_G3DMA_ERR_NG         ,
    R_G3DMA_ERR_RANGE      ,
    R_G3DMA_ERR_LOCKED     ,
    R_G3DMA_ERR_NOTLOCKED  
} r_g3dma_Error_t;


/***********************************************************
  Enum: r_g3dma_Interrupt_t

  G3DMAC macro interrupt type. List of all available interrupts for this macro
  There is only one interrupt vector for both G3DMA interrupts.
  Driver makes a desision, which case occured, according to the
  values in status registers

  Values:
    R_EE_G3DMAC_INT_G3DMAC    - Transfer Completed
    R_EE_G3DMAC_INT_CT      - Counter compare
*/

typedef enum 
{
    R_EE_G3DMAC_INT_G3DMA,
    R_EE_G3DMAC_INT_CT 
} r_g3dma_Interrupt_t;

/*******************************************************************************
  Enum: r_g3dma_AddrCntDir_t

  G3DMA address count direction type

  Members:
  R_G3DMA_FIXED_T       - Address count fixed
  R_G3DMA_INCREMENT_T   - Address count increment
*/

typedef enum 
{
    R_G3DMA_INCREMENT_T = 0u,
    R_G3DMA_FIXED_T     = 2
} r_g3dma_AddrCntDir_t;

/*******************************************************************************
  Enum: r_g3dma_TransDataSize_t

  G3DMA data size  

  Members:
  R_G3DMA_8BIT_T     - Data size is 8   bit 
  R_G3DMA_16BIT_T    - Data size is 16  bit 
  R_G3DMA_32BIT_T    - Data size is 32  bit 
  R_G3DMA_128BIT_T   - Data size is 128 bit  
  
*/

typedef enum 
{
    R_G3DMA_8BIT_T   = 0u,
    R_G3DMA_16BIT_T  = 1u,
    R_G3DMA_32BIT_T  = 2u,
    R_G3DMA_64BIT_T  = 3u,    
    R_G3DMA_128BIT_T = 4u
} r_g3dma_TransDataSize_t;

/*******************************************************************************
  Enum: r_g3dma_Mode_t

  G3DMA trigger type

  Members:
    R_G3DMA_MODE_S   - single transfer mode 
    R_G3DMA_MODE_B1  - block transfer mode 1
    R_G3DMA_MODE_B2  - block transfer mode 2

*/

typedef enum 
{
    R_G3DMA_MODE_S  = 0u,
    R_G3DMA_MODE_B1 = 1u,
    R_G3DMA_MODE_B2 = 2u
} r_g3dma_Mode_t;

/*******************************************************************************
  Enum: r_g3dma_ChainMode_t

  Channel Chain operation mode 

  Members:
  R_G3DMA_CHAIN_OFF     - Disabled 
  R_G3DMA_CHAIN_END     - Chain at the last transfer
                          A chain request is generated at the completion of the DMA cycle 
                          in which the remaining transfer count is one.
  R_G3DMA_CHAIN_ALLWAYS - Always chain
                          A chain request is generated at the completion of every DMA cycle.

*/

typedef enum 
{
    R_G3DMA_CHAIN_OFF       = 0x0u,
    R_G3DMA_CHAIN_END       = 0x1u,
    R_G3DMA_CHAIN_ALLWAYS   = 0x3u
} r_g3dma_ChainMode_t;


/*******************************************************************************
  Enum: r_g3dma_ReloadMode_t

  Reload function operation mode 

  Members:
  R_G3DMA_RELOAD_OFF     - Disabled 
  R_G3DMA_RELOAD_SRC     - Reload source and transfer (or address reload) count   
  R_G3DMA_RELOAD_DST     - Reload destination and transfer (or address reload) count   
  R_G3DMA_RELOAD_BOTH    - Reload source, destination and transfer (or address reload) count   

*/

typedef enum 
{
    R_G3DMA_RELOAD_OFF   = 0x0u,  
    R_G3DMA_RELOAD_SRC   = 0x1u,
    R_G3DMA_RELOAD_DST   = 0x2u,
    R_G3DMA_RELOAD_BOTH  = 0x3u
} r_g3dma_ReloadMode_t;


/*******************************************************************************
  Enum: r_g3dma_TrgType_t

  G3DMA transfer mode type 

  Members:
  R_G3DMA_TRG_HW_T   - G3DMA transfer is controlled by hardware
  R_G3DMA_TRG_SW_T   - G3DMA transfer is controlled by software
*/

typedef enum 
{
    R_G3DMA_TRG_SW_T  = 0u,
    R_G3DMA_TRG_HW_T  = 1
} r_g3dma_TrgType_t;


/*******************************************************************************
  Type: r_g3dma_ChCfg_t

  G3DMA channel property type

  Members:
  Unit               -  G3DMA unit
  ChannelId          -  G3DMA channel number in the unit
  SrcAddr            -  Source address
  SrcCS              -  Source CS 1: extern, 2 intern (Code Flash, RAM)   
  DstAddr            -  Destination address
  DstCS              -  Destination CS  1: extern, 2 intern (Code Flash, RAM)  
  TransferCount      -  Transfer count (ARC[15:0] | TRC[15:0]) 
  TransferDataSize   -  Transfer data size,. see <r_g3dma_TransDataSize_t> 
  SrcAddrCntDir      -  Source address count direction, see <r_g3dma_AddrCntDir_t>
  DstAddrCntDir      -  Destination address count direction, see <r_g3dma_AddrCntDir_t>
  LoopEnable         -  0: Clears the DTSnDTE bit upon completion of G3DMA transfer
                        1: Does not clear the DTSnDTE bit upon completion of G3DMA transfer.
  G3DMATriggerFactor -  G3DMA trigger factor
  InterruptAct       -  0: both interrupts disable
                        1: transfer complete interrupt enable, 
                        2: transfer count match enable,
                        3: both interrupts enable   
  Mode               -  transfer mode: 0: single / 1: block 1 /2: block 2, see <r_g3dma_Mode_t>
  CountCompare       -  Compare value for the count match interrupt 
  NextChnCh          -  next Channel in chain 
  ChainMode          -  Chain mode, see <r_g3dma_ChainMode_t>    
  Rel1Mode           -  Reload 1 function operation mode , see <r_g3dma_ReloadMode_t>   
  Rel2Mode           -  Reload 2 function operation mode,  see <r_g3dma_ReloadMode_t>
*/

typedef struct {
    uint8_t                 Unit;
    uint32_t                ChannelId;
    uint32_t                SrcAddr;
    uint32_t                DstAddr;
    uint32_t                TransferCount;
    r_g3dma_TransDataSize_t TransferDataSize;
    r_g3dma_AddrCntDir_t    SrcAddrCntDir;
    r_g3dma_AddrCntDir_t    DstAddrCntDir;
    uint32_t                LoopEnable;
    uint32_t                G3DMATriggerFactor;
    uint32_t                InterruptAct;
    r_g3dma_Mode_t          Mode;
    uint16_t                CountCompare;
    uint8_t                 NextChnCh; 
    r_g3dma_ChainMode_t     ChainMode; 
    r_g3dma_ReloadMode_t    Rel1Mode;  
    r_g3dma_ReloadMode_t    Rel2Mode;   
} r_g3dma_ChCfg_t;


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_G3DMA_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_G3DMA_GetVersionStr(void); 

/*******************************************************************************
  Function: R_G3DMA_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_G3DMA_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_G3DMA_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_G3DMA_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_G3DMA_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_g3dma_Error_t>
*/

 r_g3dma_Error_t R_G3DMA_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_G3DMA_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_g3dma_Error_t>
*/

 r_g3dma_Error_t R_G3DMA_DeInit(uint32_t Unit);

/*******************************************************************************
  Function: R_G3DMA_LoadChannel

  Load G3DMA channels with the configuration data.

  Parameters:
  ChCfg       - DMA (generic) channel configuration, see <r_dma_ChCfg_t>
  NoOfCh      - Number of G3DMA channels need to be configured
  
  Returns:
  void
*/
void R_G3DMA_LoadChannel(r_g3dma_ChCfg_t *ChCfg, uint32_t NoOfCh);

/*******************************************************************************
  Function: R_G3DMA_SetReloadConfig
  
   Load G3DMA Reload operation data (source, destination and transfer counter)

  Parameters:
  Unit        - G3DMA unit (normally 0)
  ChId        - G3DMA channel Id
  RelSrc      - Source address to be reloaded   
  RelDst      - Destination address to be reloaded 
  RelCnt      - Counter value to be reloaded 
  
  Returns:
  void
*/
void R_G3DMA_SetReloadConfig(uint32_t Unit, uint32_t ChId, 
                          uint32_t RelSrc, uint32_t RelDst, uint32_t RelCnt); 

/*******************************************************************************
  Function: R_G3DMA_SwTrigger

  SW trigger.

  Parameters:
  Unit        - G3DMA unit (normally 0)
  ChId        - G3DMA channel Id
  
  Returns:
  void
*/
void R_G3DMA_SwTrigger(uint32_t Unit, uint32_t ChId);


/*******************************************************************************
  Function: R_G3DMA_HwEnable

  HW enable.

  Parameters:
  Unit        - G3DMA unit (normally 0)
  ChId        - G3DMA channel Id
  
  Returns:
  void
*/
void R_G3DMA_HwEnable(uint32_t Unit, uint32_t ChId);



/*******************************************************************************
  Function: R_G3DMA_TransEndStat

  G3DMA transfer status.

  Parameters:
  Unit        - G3DMA unit (normally 0)
  ChId        - G3DMA channel Id

  Returns: 
  Transfer status , see <r_g3dma_TransEndStatType_t> 
*/
r_dma_TransEndStatType_t R_G3DMA_TransEndStat(uint32_t Unit, uint32_t ChId);

/*******************************************************************************
  Function: R_G3DMA_TransErrStat

  G3DMA transfer error status.

  Parameters:
  Unit        - G3DMA unit (normally 0)
  ChId        - G3DMA channel Id
  Returns: 

  Transfer status , see <r_g3dma_TransEndStatType_t> 
*/
r_dma_TransErrStatType_t R_G3DMA_TransErrStat(uint32_t Unit, uint32_t ChId);

/*******************************************************************************
  Group: G3DMA Error and ISR Functions
*/

/*******************************************************************************
  Function: R_G3DMA_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_G3DMA_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));

/***********************************************************
  Function: R_G3DMA_EnableInt

  Enable the interrupt for a given instance and channel 
    
  Parameters:
  Unit       - Instance number
  ChId       - G3DMA channel Id
  
  Returns:
  == 0, OK , see <r_g3dma_Error_t>
  != 0  NG, , see <r_g3dma_Error_t>

*/
r_g3dma_Error_t R_G3DMA_EnableInt(uint32_t Unit, uint32_t ChId); 

/***********************************************************
  Function: R_G3DMA_DisableInt

  Disable the interrupt for a given instance and channel 


  Parameters:
  Unit       - Instance number
  ChId       - G3DMA channel Id
  
  Returns:
  == 0, OK , see <r_g3dma_Error_t>
  != 0  NG, , see <r_g3dma_Error_t>

*/
r_g3dma_Error_t R_G3DMA_DisableInt(uint32_t Unit, uint32_t ChId); 




#ifdef __cplusplus
}
#endif

#endif /* G3DMA_API_H_  */
