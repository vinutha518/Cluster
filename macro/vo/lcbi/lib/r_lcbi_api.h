/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_lcbi_api.h 4366 2014-11-19 09:43:51Z golczewskim $
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

#ifndef LCBI_API_H_
#define LCBI_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: LCBI Driver API 
  
  An application using LCBI should only need to include this r_lcbi_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_LCBI_VERSION_HI and R_LCBI_VERSION_LO

  Driver version information  
*/

#define R_LCBI_VERSION_HI 0
#define R_LCBI_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_lcbi_Error_t

  LCBI macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_LCBI_ERR_OK           - No error
  R_LCBI_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_LCBI_ERR_OK         = 0x0,
    R_LCBI_ERR_FIFOOVERF  ,
    R_LCBI_ERR_FIFOUNDR   ,
    R_LCBI_ERR_NG         ,
    R_LCBI_ERR_RANGE      ,
    R_LCBI_ERR_LOCKED     ,
    R_LCBI_ERR_NOTLOCKED  
} r_lcbi_Error_t;

/*******************************************************************************
  Enum: r_lcbi_Interrupt_t

  Lcbi macro interrupt type. 
  
  List of all available interrupts for this macro.

  Values:
  R_LCBI_INT_RDY        - Ready Interrupt
  R_LCBI_INT_EMPT       - Empty Interrupt
  R_LCBI_INT_QTR        - Quarter Interrupt
  R_LCBI_INT_HALF       - Half Interrupt
  R_LCBI_INT_3QTR       - Three-Quarter Interrupt
  R_LCBI_INT_FULL       - Full  
  R_LCBI_INT_NUM        - Delimiter 
*/

typedef enum
{
    R_LCBI_INT_RDY,
    R_LCBI_INT_EMPT,
    R_LCBI_INT_QTR,
    R_LCBI_INT_HALF,
    R_LCBI_INT_3QTR,
    R_LCBI_INT_FULL,
    R_LCBI_INT_NUM
} r_lcbi_Interrupt_t;


/***********************************************************
  Enum: r_lcbi_BusCycle_Type_t
 
  External bus interface mode.

  Values:
  R_LCBI_BUSCYCLE_RAM    - Control signals are ~WR and ~RD
  R_LCBI_BUSCYCLE_ETYPE     - Control signals are E and R/W
*/

typedef enum
{
    R_LCBI_BUSCYCLE_RAM    = 0,
    R_LCBI_BUSCYCLE_ETYPE
} r_lcbi_BusCycle_Type_t;


/***********************************************************
  Enum: r_lcbi_WM_t
 
  LCBI working mode.

  Values:
  R_LCBI_WM_STOP         - LCBI Stopped
  R_LCBI_WM_NONTFT       - non-TFT mode
  R_LCBI_WM_TFT          - TFT mode
  R_LCBI_WM_CLUT_DEF     - CLUT definition mode
*/

typedef enum
{
    R_LCBI_WM_STOP         = 0,
    R_LCBI_WM_NONTFT,
    R_LCBI_WM_TFT,
    R_LCBI_WM_CLUT_DEF,
} r_lcbi_WM_t;


/***********************************************************
  Enum: r_lcbi_BusCycle_Speed_t
 
  LCDBIF bus cycle speed

  Values:
  R_LCBI_BUSCYCLE_SLOW  - slow bus cycle mode
  R_LCBI_BUSCYCLE_FAST  - fast bus cycle mode
*/

typedef enum
{
    R_LCBI_BUSCYCLE_SLOW  = 0,
    R_LCBI_BUSCYCLE_FAST
} r_lcbi_BusCycle_Speed_t;


/***********************************************************
  Enum: r_lcbi_TransferMode_t
 
  direct and indirect access mode

  Values:
  R_LCBI_DIRECT    - Direct reading and writing mode
  R_LCBI_INDIRECT  - Indirect writing mode
*/

typedef enum
{
    R_LCBI_DIRECT    = 0,
    R_LCBI_INDIRECT
} r_lcbi_TransferMode_t;


/***********************************************************
  Enum: r_lcbi_INDPalette_t
  
  Palette bits in indirect access mode
 
  Values:
  R_LCBI_IND_8BP  - 8-Bit palette for CLUT operation
  R_LCBI_IND_4BP  - 4-Bit palette for CLUT operation
*/

typedef enum
{
    R_LCBI_IND_8BP  = 0,
    R_LCBI_IND_4BP
} r_lcbi_INDPalette_t;  


/***********************************************************
  Enum: r_lcbi_AccessBit_t
 
  Access mode (8, 9, 16, 18-bit access mode)

  Values:
  R_LCBI_8BIT   - 8-bit mode
  R_LCBI_16BIT  - 16-bit mode
  R_LCBI_9BIT   - 9-bit mode
  R_LCBI_18BIT  - 18-bit mode
*/

typedef enum
{
    R_LCBI_8BIT   = 0,
    R_LCBI_16BIT,
    R_LCBI_9BIT,
    R_LCBI_18BIT
} r_lcbi_AccessBit_t;


/***********************************************************
  Enum: r_lcbi_ShiftBit_t
  
  Shift bits, depending on these bits, values are transferred
  on certain ranges on LCD Bus.

  Values:
  R_LCBI_SHIFT0
  R_LCBI_SHIFT1
  R_LCBI_SHIFT2
  R_LCBI_SHIFT3
*/

typedef enum
{
    R_LCBI_SHIFT0  = 0,
    R_LCBI_SHIFT1,
    R_LCBI_SHIFT2,
    R_LCBI_SHIFT3
} r_lcbi_ShiftBit_t;


/***********************************************************
  Enum: r_lcbi_TransferCtrl_t
 
  transfer mode

  Values:
  R_LCBI_D_8BIT_S0    
  R_LCBI_D_8BIT_S1    
  R_LCBI_D_8BIT_S2    
  R_LCBI_D_8BIT_S3    
  R_LCBI_D_9BIT       
  R_LCBI_D_16BIT_S1   
  R_LCBI_D_16BIT_S2   
  R_LCBI_D_18BIT      
  R_LCBI_IND_9BIT_4BP 
  R_LCBI_IND_9BIT_8BP 
  R_LCBI_IND_18BIT_4BP
  R_LCBI_IND_18BIT_8BP
*/

typedef enum
{
    R_LCBI_D_8BIT_S0     = 0x00000000,
    R_LCBI_D_8BIT_S1     = 0x00000040,
    R_LCBI_D_8BIT_S2     = 0x00000080,
    R_LCBI_D_8BIT_S3     = 0x000000C0,
    R_LCBI_D_9BIT        = 0x00000020,
    R_LCBI_D_16BIT_S1    = 0x00000050,
    R_LCBI_D_16BIT_S2    = 0x00000090,
    R_LCBI_D_18BIT       = 0x00000030,
    R_LCBI_IND_9BIT_4BP  = 0x00000023,
    R_LCBI_IND_9BIT_8BP  = 0x00000021,
    R_LCBI_IND_18BIT_4BP = 0x00000033,
    R_LCBI_IND_18BIT_8BP = 0x00000031
} r_lcbi_TransferCtrl_t;
    

/***********************************************************
  Enum: r_lcbi_CLKDIV_t
 
  set the (LCBI internal) clock divider

  Values:
  R_LCBI_CLKDIV_1  - clock division 1/1
  R_LCBI_CLKDIV_2  - clock division 1/2
  R_LCBI_CLKDIV_4  - clock division 1/4
  R_LCBI_CLKDIV_8  - clock division 1/8
  R_LCBI_CLKDIV_16  - clock division 1/16
  R_LCBI_CLKDIV_32  - clock division 1/32
  R_LCBI_CLKDIV_64  - clock division 1/64
*/

typedef enum
{
    R_LCBI_CLKDIV_1 = 0x00,
    R_LCBI_CLKDIV_2,
    R_LCBI_CLKDIV_4,    
    R_LCBI_CLKDIV_8,
    R_LCBI_CLKDIV_16,
    R_LCBI_CLKDIV_32,
    R_LCBI_CLKDIV_64
} r_lcbi_CLKDIV_t;


/***********************************************************
  Enum: r_lcbi_Output_Active_t
 
  set the active level

  Values:
  R_LCBI_XX_ACTIVELOW  - active low for signal
  R_LCBI_XX_ACTIVELOW  - active high
    -> signals in TFT-mode:     HS (hsync), VS (vsync), DE (data enable)
    -> signals in non TFT-mode: RDZE (RD strobe / E-Type mode)
*/

typedef enum
{
    R_LCBI_ACTIVE_DEFAULT = 0x00000000,
    
    R_LCBI_HS_ACTIVELOW  = 0x00000000,
    R_LCBI_HS_ACTIVEHIGH = (1ul<<8u),
    R_LCBI_VS_ACTIVELOW  = 0x00000000,
    R_LCBI_VS_ACTIVEHIGH = (1ul<<9u),
    R_LCBI_DE_ACTIVELOW  = 0x00000000,
    R_LCBI_DE_ACTIVEHIGH = (1ul<<10u),
    
    R_LCBI_RDZE_ACTIVELOW  = 0x00000000,
    R_LCBI_RDZE_ACTIVEHIGH = (1ul<<3u)
} r_lcbi_Output_Active_t;


/***********************************************************
  Type: r_lcbi_Parameter_t

  LCDBIF macro configuration data

  Members:
  WorkMode   - see <r_lcbi_WM_t>               
  CycleSpeed - see <r_lcbi_Bus_Cycle_Speed_t>
  CycleType  - see <r_lcbi_Bus_Cycle_Type_t> 
  TransMode  - see <r_lcbi_Transfer_Mode_t>  
  IndPalette - see <r_lcbi_IND_Palette_t>   
  AccessBit  - see <r_lcbi_AccessBit_t>     
  ShiftBit   - see <r_lcbi_ShiftBit_t>       
*/

typedef struct
{
    r_lcbi_WM_t              WorkMode;
    r_lcbi_BusCycle_Speed_t  CycleSpeed;
    r_lcbi_BusCycle_Type_t   CycleType;
    r_lcbi_TransferMode_t    TransMode;
    r_lcbi_INDPalette_t      IndPalette;
    r_lcbi_AccessBit_t       AccessBit;
    r_lcbi_ShiftBit_t        ShiftBit;
} r_lcbi_Parameter_t;


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_LCBI_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_LCBI_GetVersionStr(void); 

/*******************************************************************************
  Function: R_LCBI_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_LCBI_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_LCBI_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_LCBI_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_LCBI_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_lcbi_Error_t>
*/

 r_lcbi_Error_t R_LCBI_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_LCBI_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_lcbi_Error_t>
*/

 r_lcbi_Error_t R_LCBI_DeInit(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_GetFIFOFillLevel

  Get the Fifo fill level value

  Parameters:
  Unit  - Instance number

  Returns:
  The fifo fill level value
*/

uint8_t R_LCBI_GetFIFOFillLevel(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_SetWorkingParam

  Set working mode parameters for LCDBIF

  Parameters:
  Unit  - Instance number
  Param - see <r_lcbi_Parameter_t>

  Returns:
  void
*/

void R_LCBI_SetWorkingParam(uint32_t Unit, r_lcbi_Parameter_t* Param);


/***********************************************************
  Function: R_LCBI_SetTConParam

  Set transfer control parameter for LCDBIF

  Parameters:
  Unit  - Instance number
  Param - see <r_lcbi_Parameter_t>

  Returns:
  void  -
*/

void R_LCBI_SetTConParam(uint32_t Unit, r_lcbi_Parameter_t* Param);


/***********************************************************
  Function: R_LCBI_StopLcdBIF

  Stop the LCDBIF

  Parameters:
  Unit  - Instance number

  Returns:
  void  -
*/

void R_LCBI_StopLcdBIF(uint32_t Unit);
 

/***********************************************************
  Function: R_LCBI_Check_TC_LOCK

  check the status of control registers (locked or unlocked)

  Parameters:
  Unit      - Instance number

  Returns:
  0  - control registers are unlocked
  1  - control registers are locked
*/

uint8_t R_LCBI_Check_TcLock(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_IsReadPending

  check the if there is a read access is pending  

  Parameters:
  Unit  - Instance number

  Returns:
  void  -
*/

uint8_t R_LCBI_IsReadPending(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_WaitTransferring

  wait until LCDBIF is idle

  Parameters:
  Unit      - Instance number

  Returns:
  void  -
*/

void R_LCBI_WaitTransferring(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_SetBusCycle

  Select the bus transfer type in non-TFT operation mode.

  Parameters:
  Unit  - Instance number
  BusCycle - bus cycle type, see <r_lcbi_Bus_Cycle_Type_t>

  Returns:
  void  -
*/        

void R_LCBI_SetBusCycle(uint32_t Unit, 
    r_lcbi_BusCycle_Type_t BusCycle);


/***********************************************************
  Function: R_LCBI_SetClock

  Select the clock source for the LCDBIF, an internal clocking
  is derived from this setting.

  Parameters:
  Unit  - Instance number
  ClockDiv - clock divider parameter

  Returns:
  void  -
*/

void R_LCBI_SetClock(
    uint32_t      Unit, 
    r_lcbi_CLKDIV_t ClockDiv);
    
    
/***********************************************************
  Function: ee_LcdBIF_InvertOutput

  Set the output level of LCD bus signals active high or low

  Parameters:
  Unit  - Instance number
  Val   - value to be write into the output level control register

  Returns:
  void  -
*/

void R_LCBI_InvertOutput(
    uint32_t      Unit,
    r_lcbi_Output_Active_t Val);

   
/***********************************************************
  Function: R_LCBI_SetNonTFTCycleSpec

  Define the bus cycle of non_TFT Modes by specifying the timing 
  steps in number of LCDBIF operation clocks.

  Parameters:
  Unit       - Instance number
  NonTFTSpec - Non TFT Mode bus cycle specification

  Returns:
  void  -
*/

void R_LCBI_SetNonTFTCycleSpec(
    uint32_t      Unit,
    uint32_t NonTFTSpec);


/***********************************************************
  Function: R_LCBI_SetTFTPrescaler

  Set prescaler for the pixel clock in TFT operation mode

  Parameters:
  Unit   - Instance number
  TFTPRS - TFT pixel clock prescaler value

  Returns:
  void  -
*/

void R_LCBI_SetTFTPrescaler(
    uint32_t      Unit,
    uint32_t TFTPrs);


/***********************************************************
  Function: R_LCBI_SetTFTCycleC

  Define the bus cycle of TFT operation mode (common phases)

  Parameters:
  Unit      - Instance number
  TFTCycleC - TFT cycle timing specification

  Returns:
  void  -
*/

void R_LCBI_SetTFTCycleC(
    uint32_t      Unit,
    uint32_t TFTCycleC);


/***********************************************************
  Function: R_LCBI_SetTFTCycleV

  Define the bus cycle of TFT operation mode (vertical phases)

  Parameters:
  Unit      - Instance number
  TFTCycleV - TFT cycle timing specification

  Returns:
  void  -
*/      

void R_LCBI_SetTFTCycleV(
    uint32_t      Unit,
    uint32_t TFTCycleV);


/***********************************************************
  Function: R_LCBI_SetTFTCycleH

  Define the bus cycle of TFT operation mode (horizontal phases)

  Parameters:
  Unit      - Instance number
  TFTCycleH - TFT cycle timing specification

  Returns:
  void  -
*/

void R_LCBI_SetTFTCycleH(
    uint32_t      Unit, 
    uint32_t TFTCycleH);


/***********************************************************
  Function: R_LCBI_WaitWorkModeStable

  Pool the operation mode and ensure it has gone to zero

  Parameters:
  Unit - Instance number

  Returns:
  void  -
*/

void R_LCBI_WaitWorkModeStable(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_WriteData8_A0S

  Write 8-bit with A0 set

  Parameters:
  Unit      - Instance number
  Data      - Data to be written
  
  Returns:
  void  -
*/

void R_LCBI_WriteData8_A0S(uint32_t Unit, uint8_t Data);


/***********************************************************
  Function: R_LCBI_WriteData8_A0C

  Write 8-bit with A0 clear

  Parameters:
  Unit      - Instance number
  Data      - Data to be written
  
  Returns:
  void  -
*/

void R_LCBI_WriteData8_A0C(uint32_t Unit, uint8_t Data);


/***********************************************************
  Function: R_LCBI_WriteData16_A0S

  Write 16-bit with A0 set

  Parameters:
  Unit      - Instance number
  Data      - Data to be written
  
  Returns:
  void  -
*/

void R_LCBI_WriteData16_A0S(uint32_t Unit, uint16_t Data);


/***********************************************************
  Function: R_LCBI_WriteData16_A0C

  Write 16-bit with A0 clear

  Parameters:
  Unit      - Instance number
  Data      - Data to be written
  
  Returns:
  void  -
*/

void R_LCBI_WriteData16_A0C(uint32_t Unit, uint16_t Data);


/***********************************************************
  Function: R_LCBI_WriteData32_A0S

  Write 32-bit with A0 set

  Parameters:
  Unit      - Instance number
  Data      - Data to be written
  
  Returns:
  void  -
*/

void R_LCBI_WriteData32_A0S(uint32_t Unit, uint32_t Data);


/***********************************************************
  Function: R_LCBI_WriteData32_A0C

  Write 32-bit with A0 clear

  Parameters:
  Unit      - Instance number
  Data      - Data to be written
  
  Returns:
  void  -
*/

void R_LCBI_WriteData32_A0C(uint32_t Unit, uint32_t Data);


/***********************************************************
  Function: R_LCBI_ReadData8_A0S

  Read 8-bit with cycle start trigger and A0 set
  
  Parameters:
  Unit - Instance number
  
  Returns:
  8-bit data read from LCDBIF
*/

uint8_t R_LCBI_ReadData8_A0S(uint32_t Unit); 


/***********************************************************
  Function: R_LCBI_ReadData8_A0C

  Read 8-bit with cycle start trigger and A0 clear
  
  Parameters:
  Unit - Instance number
  
  Returns:
  8-bit data read from LCDBIF
*/

uint8_t R_LCBI_ReadData8_A0C(uint32_t Unit); 


/***********************************************************
  Function: R_LCBI_ReadData16_A0S

  Read 16-bit with cycle start trigger and A0 set 
  
  Parameters:
  Unit - Instance number
    
  Returns:
  16-bit data read from LCDBIF
*/

uint16_t R_LCBI_ReadData16_A0S(uint32_t Unit); 


/***********************************************************
  Function: R_LCBI_ReadData16_A0C

  Read 16-bit with cycle start trigger and A0 clear 
  
  Parameters:
  Unit - Instance number
    
  Returns:
  16-bit data read from LCDBIF
*/

uint16_t R_LCBI_ReadData16_A0C(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_ReadData32_A0S

  Read 32-bit with cycle start trigger and A0 set

  Parameters:
  Unit - Instance number
  
  Returns:
  32-bit data read from LCDBIF
*/

uint32_t R_LCBI_ReadData32_A0S(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_ReadData32_A0C

  Read 32-bit with cycle start trigger and A0 clear

  Parameters:
  Unit - Instance number
  
  Returns:
  32-bit data read from LCDBIF
*/

uint32_t R_LCBI_ReadData32_A0C(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_WriteCLUT

  Define the Color Lookup Table

  Parameters:
  Unit   - Instance number
  Offset - Offset Address of the target register
  Data   - data to be written in the CLUT

  Returns:
  void  -
*/

void R_LCBI_WriteCLUT(
    uint32_t      Unit,
    uint32_t Offset, 
    uint32_t Data);
    
    
/***********************************************************
  Function: R_LCBI_ReadCLUT

  Read the Color Lookup Table

  Parameters:
  Unit   - Instance number
  Offset - Offset Address of the target register

  Returns:
  content of the register in CLUT with defined Offset
*/

uint32_t R_LCBI_ReadCLUT(
    uint32_t      Unit,
    uint32_t Offset);


/***********************************************************
  Function: R_LCBI_ResetLcdBIF

  Reset the LCDBIF

  Parameters:
  Unit - Instance number

  Returns:
  void  -
*/

void R_LCBI_ResetLcdBIF(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_GetRegister

  Get the content of a register

  Parameters:
  Unit - Instance number
  Addr - Offset address of the target register

  Returns:
  Content of the target register
*/

uint32_t R_LCBI_GetRegister(uint32_t Unit, uint32_t Addr);


/***********************************************************
  Function: R_LCBI_GetWorkingMode

  Get the working mode of LCDBIF

  Parameters:
  Unit - Instance number

  Returns:
  Working mode
*/

uint8_t R_LCBI_GetWorkingMode(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_SetCLUTOffset

  Set the 4-bit offset of CLUT address.

  Parameters:
  Unit   - Instance number
  Offset - Offset of CLUT address

  Returns:
  void  -
*/

void R_LCBI_SetCLUTOffset(uint32_t Unit, uint32_t Offset);


/***********************************************************
  Function: R_LCBI_GetFSMState

  Get the state of the internal FSM

  Parameters:
  Unit  - Instance number

  Returns:
  state of the internal FSM
*/

uint8_t R_LCBI_GetFSMState(uint32_t Unit);


/***********************************************************
  Function: R_LCBI_WriteReg

  Write a register

  Parameters:
  Unit   - Instance number
  Data   - data to be written
  Offset - offset address of the register

  Returns:
  void  -
*/

void R_LCBI_WriteReg(uint32_t Unit, uint32_t Data, uint32_t Offset);


/*******************************************************************************
  Group: LCBI Error and ISR Functions
*/

/*******************************************************************************
  Function: R_LCBI_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_LCBI_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));

/*******************************************************************************
  Function: R_LCBI_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Interrupt       - What interrupt,  see: <r_lcbi_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_lcbi_Error_t>
*/

 r_lcbi_Error_t R_LCBI_SetIsrCallback(uint32_t Unit, r_lcbi_Interrupt_t Interrupt, 
                                 void (*Isr)(void));


/***********************************************************
  Function: R_LCBI_Isr

  Device interrupt service routine.
  
  This function shall be called by device interrupt handler 
  (xxxxxx_isr.c) and can be found in lcbi\src\lcbi_isr.c.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type. see<r_lcbi_Interrupt_t>

  Returns:
  void  -
*/

void R_LCBI_Isr(uint32_t Unit, r_lcbi_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_LCBI_EnableInt

  Enable the LCBI interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_lcbi_Interrupt_t>
  
  Returns:
  void
*/

void R_LCBI_EnableInt(uint32_t Unit, r_lcbi_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_LCBI_DisableInt

  Disable the LCBI interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_lcbi_Interrupt_t>
  
  Returns:
  void
*/

void R_LCBI_DisableInt(uint32_t Unit, r_lcbi_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_LCBI_GetClock
  
  Get the macro clock frequency of the LCBI
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

uint32_t R_LCBI_GetClock(uint32_t Unit);


/*******************************************************************************
  Function: R_LCBI_DisplayTimingSet
  
  Set the LCBI timing settings and pixel clock (of TFT-mode)
  
  Parameters:
  Unit       - Instance number
  Timing     - see Display Database <r_ddb_Timing_t>
  
  Returns:
  r_lcbi_Error_t
*/

r_lcbi_Error_t R_LCBI_DisplayTimingSet(uint32_t Unit, r_ddb_Timing_t *timing);



#ifdef __cplusplus
}
#endif

#endif /* LCBI_API_H_  */
