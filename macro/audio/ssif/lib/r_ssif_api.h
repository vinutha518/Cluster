/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_ssif_api.h 4630 2015-01-13 16:48:47Z golczewskim $
============================================================================ 
DESCRIPTION
Generic part of the macro driver
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

#ifndef SSIF_API_H_
#define SSIF_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: SSIF Driver API 
  
  An application using SSIF should only need to include this r_ssif_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_SSIF_VERSION_HI and R_SSIF_VERSION_LO

  Driver version information  
*/

#define R_SSIF_VERSION_HI 0
#define R_SSIF_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_ssif_Error_t

  SSIF macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_SSIF_ERR_OK           - No error
  R_SSIF_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_SSIF_ERR_OK         = 0x0,
    R_SSIF_ERR_NG         ,
    R_SSIF_ERR_RANGE      ,
    R_SSIF_ERR_LOCKED     ,
    R_SSIF_ERR_NOTLOCKED  
} r_ssif_Error_t;

/*******************************************************************************
  Enum: r_ssif_Interrupt_t

  Ssif macro interrupt type. 
  
  List of all available interrupts for this macro.

  Values:
  R_SSIF_INT_TXI          - Transmission data empty Int. (INTSSIF0/1TX; DMA trigger ID 121/123)
  R_SSIF_INT_RXI          - Reception data full Int. (INTSSIF0/1RX; DMA trigger ID 122/124)
  R_SSIF_INT_IRQ          - Multi-purpose interrupt (INTSSIF0/1), all sub-types
  R_SSIF_INT_IRQ_IDLE     - Multi-purpose interrupt (INTSSIF0/1), sub-type idle mode detected
  R_SSIF_INT_IRQ_RXOVER   - Multi-purpose interrupt (INTSSIF0/1), sub-type RX FIFO overrun detected
  R_SSIF_INT_IRQ_RXUNDER  - Multi-purpose interrupt (INTSSIF0/1), sub-type RX FIFO underrun detected
  R_SSIF_INT_IRQ_TXOVER   - Multi-purpose interrupt (INTSSIF0/1), sub-type TX FIFO overrun detected
  R_SSIF_INT_IRQ_TXUNDER  - Multi-purpose interrupt (INTSSIF0/1), sub-type TX FIFO underrun detected
  R_SSIF_INT_NUM          - Delimiter 
*/

typedef enum
{
    R_SSIF_INT_TXI,
    R_SSIF_INT_RXI,
    R_SSIF_INT_IRQ,
    R_SSIF_INT_IRQ_IDLE,
    R_SSIF_INT_IRQ_RXOVER,
    R_SSIF_INT_IRQ_RXUNDER,
    R_SSIF_INT_IRQ_TXOVER,
    R_SSIF_INT_IRQ_TXUNDER,
    R_SSIF_INT_NUM
} r_ssif_Interrupt_t;


typedef enum
{    
    R_SSIF_MODE_IIS = 0,
    R_SSIF_MODE_TDM = 1u
} r_ssif_Mode_t;


typedef enum
{    
    /* IIS mode */
    R_SSIF_1CHANNEL_PER_SYSWORD = 0x0,
    R_SSIF_2CHANNEL_PER_SYSWORD = 0x1,
    R_SSIF_3CHANNEL_PER_SYSWORD = 0x2,
    R_SSIF_4CHANNEL_PER_SYSWORD = 0x3,
    /* TDM mode */
    R_SSIF_4CHANNEL_PER_TDMFRAME = 0x1,
    R_SSIF_6CHANNEL_PER_TDMFRAME = 0x2,
    R_SSIF_8CHANNEL_PER_TDMFRAME = 0x3
} r_ssif_Channels_t;

typedef enum
{    
    R_SSIF_SCK_DIR_IN = 0,
    R_SSIF_SCK_DIR_OUT  = 1u
} r_ssif_SckDir_t;

typedef enum
{    
    R_SSIF_SCK_POL_FALLING = 0,
    R_SSIF_SCK_POL_RISING  = 1u
} r_ssif_SckPol_t;

typedef enum
{    
    R_SSIF_WS_DIR_IN = 0,
    R_SSIF_WS_DIR_OUT  = 1u
} r_ssif_WsDir_t;

typedef enum
{    
    R_SSIF_WS_POL_FIRST_LOW = 0,
    R_SSIF_WS_POL_FIRST_HIGH  = 1u
} r_ssif_WsPol_t;

typedef enum
{
    R_SSIF_SALIGN_PADDING_FIRST     = 0x001u,
    R_SSIF_SALIGN_PADDING_LAST      = 0x002u,

    R_SSIF_PALIGN_8OR16_ALIGN_LOWBYTE_FIRST = 0x010u,
    R_SSIF_PALIGN_8OR16_ALIGN_LOWBYTE_LAST  = 0x020u,

    R_SSIF_PALIGN_18TO24_ALIGN_LEFT  = 0x100u,
    R_SSIF_PALIGN_18TO24_ALIGN_RIGHT = 0x200u
} r_ssif_DataAlign_t;

typedef enum
{    
    R_SSIF_DATA_WORD_LENGTH_8BIT  = 0x0u,
    R_SSIF_DATA_WORD_LENGTH_16BIT = 0x1u,
    R_SSIF_DATA_WORD_LENGTH_18BIT = 0x2u,
    R_SSIF_DATA_WORD_LENGTH_20BIT = 0x3u,
    R_SSIF_DATA_WORD_LENGTH_22BIT = 0x4u,
    R_SSIF_DATA_WORD_LENGTH_24BIT = 0x5u,
    R_SSIF_DATA_WORD_LENGTH_32BIT = 0x6u
} r_ssif_DataWordLength_t;

typedef enum
{    
    R_SSIF_SYS_WORD_LENGTH_8BIT   = 0x0u,
    R_SSIF_SYS_WORD_LENGTH_16BIT  = 0x1u,
    R_SSIF_SYS_WORD_LENGTH_24BIT  = 0x2u,
    R_SSIF_SYS_WORD_LENGTH_32BIT  = 0x3u,
    R_SSIF_SYS_WORD_LENGTH_48BIT  = 0x4u,
    R_SSIF_SYS_WORD_LENGTH_64BIT  = 0x5u,
    R_SSIF_SYS_WORD_LENGTH_128BIT = 0x6u,
    R_SSIF_SYS_WORD_LENGTH_256BIT = 0x7u
} r_ssif_SystemWordLength_t;

typedef enum
{    
    R_SSIF_TX_DISABLED = 0,
    R_SSIF_TX_ENABLED  = 1u
} r_ssif_TxE_t;

typedef enum
{    
    R_SSIF_RX_DISABLED = 0,
    R_SSIF_RX_ENABLED  = 1u
} r_ssif_RxE_t;


/*******************************************************************************
  Type: r_ssif_Parameter_t

  SSIF macro configuration data

  Members:
    r_ssif_Mode_t               Mode
    r_ssif_Channels_t           Channels
    r_ssif_SckDir_t             SckDirection
    r_ssif_SckPol_t             SckPolarity
    r_ssif_WsDir_t              WsDirection
    r_ssif_WsPol_t              WsPolarity
    r_ssif_DataAlign_t          DataAlignment
    uint32_t                    Baudrate   
    r_ssif_DataWordLength_t     DataWordLength
    r_ssif_SystemWordLength_t   SystemWordLength
    FifoCtrl                    Fifo settings 
*/     

typedef struct {
    r_ssif_Mode_t               Mode;
    r_ssif_Channels_t           Channels;
    r_ssif_SckDir_t             SckDirection;
    r_ssif_SckPol_t             SckPolarity;
    r_ssif_WsDir_t              WsDirection;
    r_ssif_WsPol_t              WsPolarity;
    r_ssif_DataAlign_t          DataAlignment;
    uint32_t                    Baudrate;   
    uint32_t                    OverSampleRate;   
    r_ssif_DataWordLength_t     DataWordLength;
    r_ssif_SystemWordLength_t   SystemWordLength;
    uint32_t                    FifoCtrl; 
} r_ssif_Parameter_t;


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_SSIF_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_SSIF_GetVersionStr(void); 

/*******************************************************************************
  Function: R_SSIF_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_SSIF_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_SSIF_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_SSIF_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_SSIF_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_ssif_Error_t>
*/

r_ssif_Error_t R_SSIF_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_ssif_Error_t>
*/

 r_ssif_Error_t R_SSIF_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_Config

  Driver configuration function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_ssif_Error_t>
*/

r_ssif_Error_t R_SSIF_Config(uint32_t Unit, r_ssif_Parameter_t *Config);


/*******************************************************************************
  Function: R_SSIF_TransferData

  Transfer (send) Data by SSIF.

  Parameters:
  Unit          - Macro instance number
  Data          - Data to be send
 
  Returns:
  see: <r_ssif_Error_t>
*/

uint32_t R_SSIF_TransferData(uint32_t Unit, uint32_t Data);


/*******************************************************************************
  Function: R_SSIF_TransferEnable

  Enable transfer (send data) by SSIF.

  Parameters:
  Unit          - Macro instance number
  TxEnable      - Enable transfer / Disable transfer; see <r_ssif_TxE_t>
 
  Returns:
  see: <r_ssif_Error_t>
*/

uint32_t R_SSIF_TransferEnable(uint32_t Unit, r_ssif_TxE_t TxEnable);


/*******************************************************************************
  Function: R_SSIF_ReceiveEnable

  Enable receive (get data) by SSIF.

  Parameters:
  Unit          - Macro instance number
  RxEnable      - Enable receive / Disable receive; see <r_ssif_RxE_t>
 
  Returns:
  see: <r_ssif_Error_t>
*/

uint32_t R_SSIF_ReceiveEnable(uint32_t Unit, r_ssif_RxE_t RxEnable);


/*******************************************************************************
  Function: R_SSIF_GetClock
  
  Get the macro audio clock frequency of the SSIF (SSIACKINT)
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Audio clock frequency. (SSIACKINT clock)
*/

uint32_t R_SSIF_GetClock(uint32_t Unit);


/*******************************************************************************
  Group: SSIF Error and ISR Functions
*/

/*******************************************************************************
  Function: R_SSIF_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_SSIF_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));

/*******************************************************************************
  Function: R_SSIF_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Interrupt       - What interrupt,  see: <r_ssif_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_ssif_Error_t>
*/

 r_ssif_Error_t R_SSIF_SetIsrCallback(uint32_t Unit, r_ssif_Interrupt_t Interrupt, 
                                 void (*Isr)(void));
                                 

/*******************************************************************************
  Function: R_SSIF_IsrIRQ/RX/TX

  Driver IRQ / RX / TX interrupt service routine. 
  
  This function shall be called by device interrupt handler 
  (xxxxxx_isr.c) and can be found in ssif\src\ssif_isr.c. 

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SSIF_IsrIRQ(uint32_t Unit);
void R_SSIF_IsrRX(uint32_t Unit);
void R_SSIF_IsrTX(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_EnableInt

  Enable the SSIF interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ssif_Interrupt_t>
  
  Returns:
  void
*/

void R_SSIF_EnableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SSIF_DisableInt

  Disable the SSIF interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_ssif_Interrupt_t>
  
  Returns:
  void
*/

void R_SSIF_DisableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt);




#ifdef __cplusplus
}
#endif

#endif /* SSIF_API_H_  */
