/*
****************************************************************************
PROJECT : Csig driver
FILE    : $Id: r_csig_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================
DESCRIPTION
Driver for CSIG macro
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

#ifndef CSIG_API_H_
#define CSIG_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: CSIG API

  An application using CSIG should include this header file.

*/


/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_csig_Error_t

  CSIG macro driver error code.

  Values:
  R_CSIG_ERR_OK          - No error (0)
  R_CSIG_ERR_NG          - Unspecified error
  R_CSIG_ERR_PARAM       - Wrong parameters were past
  R_CSIG_ERR_RBINIT      - Error when initialising a Ring Buffer
  R_CSIG_ERR_BUFOVERRUN  - Ring Buffer overflow, data was lost
  R_CSIG_ERR_EDL_SIZE    - Error on EDL size
  R_CSIG_ERR_EDL         - Error on EDL
  R_CSIG_ERR_NUM         - Error
  R_CSIG_ERR_BUFO        - Error Buffer
  R_CSIG_ERR_LAST        - Delimeter
*/

typedef enum 
{
    R_CSIG_ERR_OK         = 0x00u,
    R_CSIG_ERR_NG,
    R_CSIG_ERR_PARAM,
    R_CSIG_ERR_RBINIT,
    R_CSIG_ERR_BUFOVERRUN,
    R_CSIG_ERR_EDL_SIZE,
    R_CSIG_ERR_EDL,
    R_CSIG_ERR_NUM,
    R_CSIG_ERR_BUFO,
    R_CSIG_ERR_BUSY
} r_csig_Error_t;



/*******************************************************************************
  Type: r_csig_StatErrorBit_t

  CSIG macro status error codes.

  Members:
  OvrErr      - Error flow detected
  DcErr       - Data Consistency error
  PaErr       - Parity error
*/

typedef struct 
{
    uint8_t PaErr  : 1;
    uint8_t DcErr  : 1;
    uint8_t OvrErr : 1;
    uint8_t unused : 5;
} r_csig_StatErrorBit_t;


/*******************************************************************************
  Type: r_csig_StatError_t

  Union describing CSIG macro status error.

  Members:
  Bit      - status error code of type <r_csig_StatErrorBit_t>
  Byte     - status error value
  
*/
typedef union 
{
    r_csig_StatErrorBit_t Bit;
    uint8_t Byte;
} r_csig_StatError_t;


/*******************************************************************************
  Enum: r_csig_Interrupt_t

  CSIG macro interrupt type. List of all available interrupts for this macro

  Values:
    R_CSIG_INT_TIRE        - Error interrupt
    R_CSIG_INT_TIR         - Reception interrupt
    R_CSIG_INT_TIC         - Transmission interrupt
    R_CSIG_INT_NUM         - Delimeter
*/

typedef enum 
{
    R_CSIG_INT_TIRE,
    R_CSIG_INT_TIR ,
    R_CSIG_INT_TIC ,
    R_CSIG_INT_NUM
} r_csig_Interrupt_t;


/*******************************************************************************
  Enum: r_csig_Mode_t

  CSIG memory mode configuration.
  Configuration parameter for CSIG init. See: <r_csig_InitCfg_t>

  Values:
  R_CSIG_MASTER        - Master mode
  R_CSIG_SLAVE         - Slave mode

*/

typedef enum
{
    R_CSIG_MASTER,
    R_CSIG_SLAVE
} r_csig_Mode_t;

/*******************************************************************************
  Enum: r_csig_Parity_t

  Parity config for CS.
  Configuration parameter of CS for CSIG init. See: <r_csig_OptFeat_t>

  Values:
    R_CSIG_PARITY_NONE     - No parity
    R_CSIG_PARITY_FIXED_0  - Extra bit fixed to 0 is added
    R_CSIG_PARITY_ODD      - Extra parity bit, Odd cfg
    R_CSIG_PARITY_EVEN     - Extra parity bit, Odd cfg
    R_CSIG_PARITY_NUM      - Delimiter
*/

typedef enum
{
    R_CSIG_PARITY_NONE     = 0u,
    R_CSIG_PARITY_FIXED_0  = 1u,
    R_CSIG_PARITY_ODD      = 2u,
    R_CSIG_PARITY_EVEN     = 3u,
    R_CSIG_PARITY_NUM
} r_csig_Parity_t;



/*******************************************************************************
  Enum: r_csig_Dir_t

  Direction config for CS.
  Configuration parameter of CS for CSIG init. See: <r_csig_InitCfg_t>

  MSB or LSB mode

  Values:
    R_CSIG_MSB    - Data is sent MSB first
    R_CSIG_LSB    - Data is sent LSB first
*/

typedef enum
{
    R_CSIG_MSB = 0u,
    R_CSIG_LSB
} r_csig_Dir_t;


/*******************************************************************************
  Enum: r_csig_ClkPhase_t

  Clock Phase config for CS.
  Configuration parameter of CS for CSIG init. See: <r_csig_InitCfg_t>

  Clock is active low or high

  Values:
    R_CSIG_CLK_ACTIVE_LOW  - Clock is active low (idle level is high)
    R_CSIG_CLK_ACTIVE_HIGH - Clock is active high (idle level is low)
*/

typedef enum
{
    R_CSIG_CLK_ACTIVE_LOW = 0u,
    R_CSIG_CLK_ACTIVE_HIGH
} r_csig_ClkPhase_t;


/*******************************************************************************
  Enum: r_csig_DataPhase_t

  Clock Phase config for CS.
  Configuration parameter of CS for CSIG init. See: <r_csig_InitCfg_t>

  Clock is active low or high

  Values:
    R_CSIG_DATA_PHASE_RISE - Data valid on rising clock edge
    R_CSIG_DATA_PHASE_FALL - Data valid on falling clock edge
*/

typedef enum
{
    R_CSIG_DATA_PHASE_RISE = 0u,
    R_CSIG_DATA_PHASE_FALL
} r_csig_DataPhase_t;


/*******************************************************************************
  Enum: r_csig_TxRxMode_t

  CSIG Tx and Rx configuration

  Values:
    R_CSIG_TX_ONLY   - Tx-only mode
    R_CSIG_RX_ONLY   - Rx-only mode
    R_CSIG_TX_RX     - Tx and Rx enabled mode
    R_CSIG_TX_RX_OFF - Tx and Rx disabled (idle/init state)
*/

typedef enum
{
    R_CSIG_TX_ONLY,
    R_CSIG_RX_ONLY,
    R_CSIG_TX_RX,
    R_CSIG_TX_RX_OFF
} r_csig_TxRxMode_t;



/*******************************************************************************
  Type: r_csig_InitCfg_t

  Csig macro configuration data (master mode only)

  Members:
    BaudRate      - Baudrate value (for master mode)
    Mode          - Master or slave. See: <r_csig_Mode_t>
    DataLength    - data length (uint8_t)
    Direction     - MSB or LSB. See: <r_csig_Dir_t>
    ClockPhase    - Clock Phase Selection see: <r_csig_ClkPhase_t>
    DataPhase     - Data Phase selection see: <r_csig_DataPhase_t>
*/

typedef struct
{
    uint32_t            Baudrate;
    r_csig_Mode_t       Mode;
    uint8_t             DataLength;
    r_csig_Dir_t        Direction;
    r_csig_ClkPhase_t   ClockPhase;
    r_csig_DataPhase_t  DataPhase;
} r_csig_InitCfg_t;


/*******************************************************************************
  Type: r_csig_OptFeat_t

  CSIG macro driver error code (master mode only)

  Members:
    Parity                    - Parity selection. See: <r_csig_Parity_t>
    R_CSIG_JOB,              - Job mode enable (autosar & CIRE & TIJC interrupt)
    eeCsiG_Edl                - Extended data length feature enable
    ee_Csig_Dcc               - Data consistency check feature enable
    R_CSIG_CS_RET_INACT      - Chips Select return to inactive if no more data
    ee_Csig_IntDelayed        - Delay 1/2 SCLK the interrupt generation
    ee_Csig_TicNormal         - TIC int. happens after transfer in direct access mode (cont trans)
    ee_Csig_Hs                - Hand shake feature enable
    ee_Csig_Ss                - Slave Selection feature enable

*/

typedef struct
{
    r_csig_Parity_t Parity;
    uint8_t       ExtDatLengE;
    uint8_t       DataConsCheckE;
    uint8_t       TicNormal;
    uint8_t       IntDelayedE;
    uint8_t       HandShakeE;
    uint8_t       SlaveSelE;
} r_csig_OptFeat_t;



/*******************************************************************************
  Section: Global API Functions

  Group: Initialisation API

*/

/*******************************************************************************
  Function: R_CSIG_Init

  Driver init function.

  Parameters:
  Unit       - Instance number
  Config     - see: <r_csig_InitCfg_t>

  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_Init(uint32_t Unit,
                        r_csig_InitCfg_t *Config,
                        r_csig_OptFeat_t *OptFeat);

/*******************************************************************************
  Function: R_CSIG_DeInit

  Driver de-init function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIG_Read

  Write one value to the CSIG TX register.

  The function waits until any ongoing transmission is completed
  and writes then the data to the TX register.

  Parameters:
  Unit         - Instance number
  Buffer       - Destination array for the Data to be read
  MaxNum       - Max number of data that can be read

  Returns:
  see: <r_csig_Error_t>
*/

uint32_t R_CSIG_Read(uint32_t Unit, void *Buffer, uint32_t MaxNum);


/*******************************************************************************
  Function: R_CSIG_Write

  Write one value to the CSIG TX register.

  The function waits until any ongoing transmission is completed
  and writes then the data to the TX register.

  Parameters:
  Unit         - Instance number
  Buffer       - Data to be copied
  Num          - Number of data
  CopyAll      - Return only when all data are copied to driver's internal buffer


  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_Write(uint32_t Unit, void *Buffer, uint32_t Num, uint8_t CopyAll);

/*******************************************************************************
  Function: R_CSIG_ReadDCE

  Reads and checks DCS bit set.

  Parameters:
  Unit         - Instance number


  Returns: 
  uint32_t     - DCE bit status
*/

uint32_t R_CSIG_ReadDCE(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_ReadOvr

  Reads and checks overwrite error bit set.

  Parameters:
  Unit         - Instance number


  Returns: 
  uint32_t     - OVR bit status
*/

uint32_t R_CSIG_ReadOvr(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_GetTransferStat

  Reads and checks transfer status bit set.

  Parameters:
  Unit         - Instance number


  Returns: 
  uint32_t     - transfer bit status
*/

uint32_t R_CSIG_GetTransferStat(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_ReadErx

  Reads eumlation reception register value.

  Parameters:
  Unit         - Instance number


  Returns: 
  uint16_t     - emulation reception register value
*/

uint16_t R_CSIG_ReadErx(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_SlaveRxEnable

  Enables the reception in slave reception only mode

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_SlaveRxEnable(int32_t  Unit );

/*******************************************************************************
  Function: R_CSIG_SlaveRxDisable

  Disables the reception in slave reception only mode

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_SlaveRxDisable(int32_t  Unit );


/*******************************************************************************
  Function: R_CSIG_Enable

  Powers on the macro.

  Parameters:
  Unit       - Instance number
  Mode       - Tx only, Rx only, Tx and Rx; see <r_csig_TxRxMode_t>

  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_Enable(uint32_t Unit, r_csig_TxRxMode_t Mode);


/*******************************************************************************
  Function: R_CSIG_Disable

  Unpowers on the macro.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_csig_Error_t>
*/

int32_t R_CSIG_Disable(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_TrigReception

  Trigger reception.

  Parameters:
  Unit       - Instance number
  Num        - Number of data to be received

  Returns:
  see: <r_csig_StatError_t>
*/
int32_t R_CSIG_TrigReception(uint32_t Unit, uint32_t Num);


/*******************************************************************************
  Function: R_CSIG_BrkPtCfg

  Configures the macro to stop or not at a debug break point

  Parameters:
  Unit       - Instance number
  Stop       - 1: stop ; 0: do not stop

  Returns:
  see: <r_csig_Error_t>
*/
r_csig_Error_t R_CSIG_BrkPtCfg(uint32_t Unit, uint8_t Stop);


/*******************************************************************************
  Function: R_CSIG_LoopBackE

  Configures the macro to Loopback mode

  Parameters:
  Unit       - Instance number
  Enable     - 1: set ; 0: unset

  Returns:
  see: <r_csig_Error_t>
*/
r_csig_Error_t R_CSIG_LoopBackE(uint32_t Unit, uint8_t Enable);


/*******************************************************************************
  Function: R_CSIG_GetNbRec

  Returns the number of available received data

  Parameters:
  Unit       - Instance number

  Returns:
  Number of data
*/
uint8_t R_CSIG_GetNbRec(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIG_SetFilterBypass

  To enable filter bypass

  Parameters:
  Unit       - Instance number

  Returns:
  1 - Unit not available (fail)
  0 - enabled filter bypass
*/
uint32_t R_CSIG_SetFilterBypass(uint32_t Unit);



/*******************************************************************************
  Function: R_CSIG_GetClock

  Gets the clock for the selected CsiG unit

  Parameters:
  Unit         - CsiG unit

  Returns:
  uint32_t     - clock frequency

*/
uint32_t R_CSIG_GetClock(uint32_t Unit);


/*******************************************************************************
  Group: Error and interrupts
*/

/*******************************************************************************
  Function: R_CSIG_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback)
                  with two paramerters (unit and error number)

  Returns:
  void

*/

void R_CSIG_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, int32_t Error));


/*******************************************************************************
  Function: R_CSIG_SetIsrCallback

  Assign a user callback for the given interrupt of a unit.

  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - What kind of interrupt; see: <r_csig_Interrupt_t>
  Isr           - Isr function pointer

  Returns:
  see: <r_csig_Error_t>
*/

r_csig_Error_t R_CSIG_SetIsrCallback(uint32_t Unit, r_csig_Interrupt_t IntType,
                                 void (*Isr)(void));


/*******************************************************************************
  Function: R_CSIG_IsrTic

  Driver TIC interrupt service routine.
  This function shall be called by device interrupt handler
  (dfxxxx_isr.c) and can be found in ..\csig\src\csig_isr.c.


  Parameters:
  Unit       - Instance number

  Returns:
  void
*/

void R_CSIG_IsrTic(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_IsrTir

  Driver TIR interrupt service routine.
  This function shall be called by device interrupt handler
  (dfxxxx_isr.c) and can be found in ..\csig\src\csig_isr.c.

  Parameters:
  Unit       - Instance number

  Returns:
  void
*/

void R_CSIG_IsrTir(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIG_IsrTire

  Driver TIRE interrupt service routine.
  This function shall be called by device interrupt handler
  (dfxxxx_isr.c) and can be found in ..\csig\src\csig_isr.c.

  Parameters:
  Unit       - Instance number

  Returns:
  void
*/

void R_CSIG_IsrTire(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIG_EnableInt

  Enable the Csig interrupt 'Interrupt'.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>

  Returns:
  see: <r_csig_Error_t>
*/

r_csig_Error_t R_CSIG_EnableInt(uint32_t Unit, r_csig_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIG_DisableInt

  Disable the Csig interrupt 'Interrupt'.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>

  Returns:
  see: <r_csig_Error_t>
*/

r_csig_Error_t R_CSIG_DisableInt(uint32_t Unit, r_csig_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIG_GetIntMask

  Get the masking status of the interrupt 'Interrupt'.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>

  Returns:
  == 1            - Interrupt masked (disabled) 
  != 1            - Interrupt enabled 
*/

uint8_t R_CSIG_GetIntMask(uint32_t Unit, r_csig_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIG_ForceInt

  Sets the flag of the requested interrupt.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>

  Returns:
  see: <r_csig_Error_t>
*/
void R_CSIG_ForceInt(uint32_t Unit, r_csig_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIG_GetStatError

  Get Error Status

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_csig_StatError_t>
*/
r_csig_StatError_t R_CSIG_GetStatError(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIG_ClearStatError

  Clear status flags

  Parameters:
  Unit       - Instance number
  Error      - see <r_csig_StatError_t>

  Returns:
  void
*/
void R_CSIG_ClearStatError(uint32_t Unit, r_csig_StatError_t Error);


/*******************************************************************************
  Function: R_CSIG_EnableDTSInt
  
  Enable DTS interrupt
  
  Parameters:
  Unit         - CsiG unit  
  
  Returns:
  void
*/

void R_SYS_CSIG_EnableDTSInt(uint32_t Unit);

#ifdef __cplusplus
}
#endif

#endif /* CSIG_API_H_  */
