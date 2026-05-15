/*
****************************************************************************
PROJECT : AWOT driver
FILE    : $Id: r_awot_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for AWOT macro
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

#ifndef AWOT_API_H_
#define AWOT_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: AWOT API

  An application using AWOT should include this header file.

*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_awot_Error_t

  AWOT macro driver error code.

  Values:
  R_AWOT_ERR_OK          - No error
  R_AWOT_ERR_NG          - Unspecified error
  R_AWOT_ERR_PORTINIT    - AWOT port init failed
  R_AWOT_ERR_LAST        - Delimiter 
*/

typedef enum
{
    R_AWOT_ERR_OK         = 0x00u,
    R_AWOT_ERR_NG         = 0x01u,
    R_AWOT_ERR_RANGE      = 0x02u,
    R_AWOT_ERR_RUNNING,
    R_AWOT_ERR_PORTINIT,
    R_AWOT_ERR_LAST
} r_awot_Error_t;


/*******************************************************************************
  Enum: r_awot_Interrupt_t

  AWOT macro interrupt type. List of all available interrupts for this macro

  Values:
    R_AWOT_INT_TINT       - Overflow Interrupt 
    R_AWOT_INT_NUM        - Delimiter 
*/

typedef enum
{
    R_AWOT_INT_TINT = 0 ,
    R_AWOT_INT_NUM
} r_awot_Interrupt_t;


/*******************************************************************************
  Enum: r_awot_Mode_t

  AWOT macro send direction.

  Configuration parameter for Uart init.

  Values:
    R_AWOT_INTERVAL        - Macro is in interval mode
    R_AWOT_CAPTURE         - Macro is in capture mode
    R_AWOT_FREE_RUN        - Macro is in free-run mode
*/

typedef enum
{
    R_AWOT_MODE_INTERVAL = 0u,
    R_AWOT_MODE_CAPTURE,
    R_AWOT_MODE_PWM,
    R_AWOT_MODE_NUM
} r_awot_Mode_t;


/*******************************************************************************
  Enum: r_awot_InputEdge_t

  Capture input AWOTIN valid edge selection

  Values:
    
    R_AWOT_INPUT_EDGE_FALLING   - Capture on falling edge
    R_AWOT_INPUT_EDGE_RISING    - Capture on rising edge
*/

typedef enum
{    
    R_AWOT_INPUT_EDGE_FALLING = 0u,
    R_AWOT_INPUT_EDGE_RISING,
    R_AWOT_INPUT_EDGE_NUM
} r_awot_InputEdge_t;


/*******************************************************************************
  Enum: r_awot_OutputMode_t

  AWOT macro send direction.

  Configuration parameter for Uart init.

  Values:
    R_AWOT_OUTPUT_LOW      - Output level low
    R_AWOT_TOGGLE_OUTPUT   - Output level toggle
*/

typedef enum
{
    R_AWOT_OUTPUT_MODE_LOW = 0u,
    R_AWOT_OUTPUT_MODE_TOGGLE,
    R_AWOT_OUTPUT_MODE_NUM
} r_awot_OutputMode_t;



/*******************************************************************************
  Enum: r_awot_Parameter_t

  Ostm macro configuration data

  CmpVal       - Comare value
  Mode         - Operating Mode, see: <r_awot_Mode_t>
  IntCtl       - Configures Interrupt at counter start, see: <r_awot_IntCtl_t>

*/     

typedef struct {
    r_awot_Mode_t              Mode;
    r_awot_InputEdge_t         InputEdge;
    r_awot_OutputMode_t        OutputMode;
    uint32_t                   CmpVal;
    uint32_t                   DutyCycle;
} r_awot_Parameter_t;



/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_AWOT_Init

  Driver init function.

  Parameters:
  Unit       - Instance number
  Config     - see: <r_awot_Parameter_t>
 
  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_Init(uint32_t Unit, r_awot_Parameter_t *Config);


/*******************************************************************************
  Function: R_AWOT_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_GetStatus

  Gives information if counter is enabled or disabled

  Parameters:
  Unit       - Instance number

  Returns:
  Status of Counter 
*/

 uint8_t R_AWOT_GetStatus(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_ClearStatus

  Gives information if counter is enabled or disabled

  Parameters:
  Unit       - Instance number

  Returns:
  Status of Counter 
*/

 uint8_t R_AWOT_ClearStatus(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_GetCaptureRegValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 uint32_t R_AWOT_GetCaptureRegValue(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_GetCompareRegValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 uint32_t R_AWOT_GetCompareRegValue(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_GetDutyCycleValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 uint32_t R_AWOT_GetDutyCycleValue(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_SetCaptureRegValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_SetCaptureRegValue(uint32_t Unit, uint32_t Val);


/*******************************************************************************
  Function: R_AWOT_GetCompareRegValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_SetCompareRegValue(uint32_t Unit, uint32_t Val);


/*******************************************************************************
  Function: R_AWOT_SetDutyCycleValue

  Returns the value of the copmare register

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_SetDutyCycleValue(uint32_t Unit, uint32_t Val);


/*******************************************************************************
  Function: R_AWOT_Start

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_Start(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_Stop

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_Stop(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_OutputMode

  Sets the configuration of the output 

  Parameters:
  Unit       - Instance number
  OutputParams

  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_SetOutputMode(uint32_t Unit, r_awot_OutputMode_t Mode);


/*******************************************************************************
  Function: R_AWOT_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback) 
                  with two paramerters (unit and error number) 
  
  Returns:
  void
 
*/

void R_AWOT_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, r_awot_Error_t Error));


/*******************************************************************************
  Section: Ostm ISR Functions
*/

/*******************************************************************************
  Function: R_AWOT_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - What kind of interrupt; see: <r_awot_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_SetIsrCallback(uint32_t Unit, r_awot_Interrupt_t IntType, 
                                 void (*Isr)(void));


/*******************************************************************************
  Function: R_AWOT_TintIsr

  Driver OV interrupt service routine. 
  This function shall be called by device interrupt handler 
  (<devicename>_isr.c) and can be found in ..\timer\awot\src\awot_isr.c.

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_AWOT_TintIsr(uint32_t Unit);


/*******************************************************************************
  Function: R_AWOT_EnableInt

  Enable the Ostm interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_awot_Interrupt_t>
  
  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_EnableInt(uint32_t Unit, r_awot_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_AWOT_DisableInt

  Disable the Ostm interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_awot_Interrupt_t>
  
  Returns:
  see: <r_awot_Error_t>
*/

 r_awot_Error_t R_AWOT_DisableInt(uint32_t Unit, r_awot_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_AWOT_GetClock

  Gets the clock for the selected Ostm unit

  Parameters:
  Unit          Ostm unit

  Returns:
  uint32_t      clock frequency

*/
 uint32_t R_AWOT_GetClock(uint32_t Unit);


#ifdef __cplusplus
}
#endif

#endif /* AWOT_API_H_  */
