/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_wdta_api.h 3935 2014-09-23 08:52:32Z golczewskim $
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

#ifndef WDTA_API_H_
#define WDTA_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: WDTA Driver API 
  
  An application using WDTA should only need to include r_jcu_api.h
*/

/*******************************************************************************
  Section: Global Types
*/

/***********************************************************
  Title: Timer WDT API

  An application using Wdta should include this header file.
  
  General information:
  The Wdta macro provides comprehensive watch dog functionalty for devices. 
  There are two main modes of operation either using the the VAC 
  (Varying Activation Code) or as in the usual way writing a constant 
  value to trigger (or kick) the watch dog.  In either case it is necessary 
  to write the Mode register first (one time register from reset).  The mode 
  register provides the main setup and includes:
  - Timer interval period slelection
  - Watchdog 75% interrupt setings (enabled or disabled)
  - Error setting (either NMI or Reset)
  - Window size (25, 50, 75 or 100%)
  
  The Wdta can also be largely setup from an external input configuration 
  inputs and thus require minimal software overhead (see below).  

*/


/***********************************************************
  Section: Global Types
*/

/***********************************************************
  Definition of the watch dog trigger register value
*/  
#define R_WDTA_TRIGGER_VALUE    0xAC

/***********************************************************
  Enum: r_wdta_Error_t

  Wdta macro driver error code.

  Values:
  R_WDTA_ERR_OK           - No error
  R_WDTA_ERR_INVALID_MODE - option not allowed in this mode
  R_WDTA_ERR_RANGE        - Unspecified error
  R_WDTA_ERR_INSTANCE     - Unit, instance not supported
  R_WDTA_ERR_PORTINIT     - port init failed
  R_WDTA_ERR_LAST         - Delimeter 
*/

typedef enum {
    R_WDTA_ERR_OK           = 0x00,
    R_WDTA_ERR_INVALID_MODE = 0x01,
    R_WDTA_ERR_RANGE        = 0x02,
    R_WDTA_ERR_INSTANCE     = 0x03,
    R_WDTA_ERR_PORTINIT,
    R_WDTA_ERR_LAST
} r_wdta_Error_t;

/***********************************************************
  Enum: r_wdta_Interrupt_t

  Wdta macro interrupt type. List of all available interrupts for this macro

  Values:
    R_WDTA_INT_75_PERCENT - Interrupt when the WD count value reaches 75 percent
                             of its range 
    R_WDTA_INT_NMI        - Non Maskable interrupt
*/

typedef enum {
    R_WDTA_INT_75_PERCENT = 0,
    R_WDTA_INT_NMI,
    R_WDTA_INT_NUM
} r_wdta_Interrupt_t;

/***********************************************************
  Enum: r_wdta_Mode_t

  Wdta macro mode type to set the main mode of operation for the WD.
  The Timer WDT can operate in two modes either using the the VAC
  (Varying Activation Code) or as in the usual way writing a constant
  value.  The VAC requires a sequence of bytes to be written in the 
  correct order.

  Configuration parameter for init.

  Values:
  R_Timer_WDTA_MODE_VAC_OFF        - No VAC is used
  R_Timer_WDTA_MODE_VAC_ON         - VAC is used 
*/

typedef enum {    
    R_WDTA_MODE_VAC_OFF = 0,
    R_WDTA_MODE_VAC_ON
} r_wdta_Mode_t;


/***********************************************************
  Enum: r_wdta_TimeInterval_t

  Wdta macro time interval selection.  The period depends on the clock freguency 
  selected externally and will further divide the frequency by the factor below.
  
  i.e. assume WD clock is 100MHz externally
                 100MHz / 2^9 about 195.3KHz about 5.12us                                                                                                                         
                         100MHz / 2^16 about 1.525KHz about 0.6ms       
                         
  Configuration parameter for watch dog time interval.

  Values:
  R_WDTA_TIME_INTERVAL_9   - WD time interval 2^9
  R_WDTA_TIME_INTERVAL_10  - WD time interval 2^10
  R_WDTA_TIME_INTERVAL_11  - WD time interval 2^11
  R_WDTA_TIME_INTERVAL_12  - WD time interval 2^12
  R_WDTA_TIME_INTERVAL_13  - WD time interval 2^13
  R_WDTA_TIME_INTERVAL_14  - WD time interval 2^14
  R_WDTA_TIME_INTERVAL_15  - WD time interval 2^15
  R_WDTA_TIME_INTERVAL_16  - WD time interval 2^16
*/

typedef enum {    
    R_WDTA_TIME_INTERVAL_9 = 0,
    R_WDTA_TIME_INTERVAL_10,
    R_WDTA_TIME_INTERVAL_11,
    R_WDTA_TIME_INTERVAL_12,
    R_WDTA_TIME_INTERVAL_13,
    R_WDTA_TIME_INTERVAL_14,
    R_WDTA_TIME_INTERVAL_15,
    R_WDTA_TIME_INTERVAL_16,
} r_wdta_TimeInterval_t;


/***********************************************************
  Enum: r_wdta_Interrupt75_Percent_t

  Wdta macro interrupt activation.  Enables or disables the 75% 
  interrup.  If this option is required then use the R_WDTA_INT_75_ACTIVATE   

  Values:
    R_WDTA_INT_75_ACTIVATE   - Interrupt when the WD count value reaches 75 percent
                                of its count range
    R_WDTA_INT_75_DEACTIVATE - Interrupt is not generated                                   
                                   
*/

typedef enum {
    R_WDTA_INT_75_DEACTIVATE = 0,
    R_WDTA_INT_75_ACTIVATE
} r_wdta_Interrupt75_Percent_t;


/***********************************************************
  Enum: r_wdta_ErrorMode_t

  Wdta macro error mode setting, this determines what action
  is taken in the event of a WD timeout.  If the NMI is selected then this   
  interrupt will be generated.  If the reset is selected then this   
  will be generated.  
  
  Values:
  R_WDTA_INT_NMI - NMI Interrupt generated
  R_WDTA_RESET   - Reset is activated
                                
*/

typedef enum {
    R_WDTA_ERROR_INT_NMI = 0,
    R_WDTA_ERROR_RESET
} r_wdta_ErrorMode_t;


/***********************************************************
  Enum: r_wdta_WindowSize_t

  Wdta macro window size, this determines the size of the open window.
 
  Values:
  R_WDTA_WINDOW_SIZE_25_PERCENT    - Open window size 25% (after 75% of counter max.)
  R_WDTA_WINDOW_SIZE_50_PERCENT    - Open window size 50%
  R_WDTA_WINDOW_SIZE_75_PERCENT    - Open window size 75%
  R_WDTA_WINDOW_SIZE_100_PERCENT   - Open window size 100% (complies to disabled Window function)
                                   
*/

typedef enum {
    R_WDTA_WINDOW_SIZE_25_PERCENT = 0,
    R_WDTA_WINDOW_SIZE_50_PERCENT,    
    R_WDTA_WINDOW_SIZE_75_PERCENT,
    R_WDTA_WINDOW_SIZE_100_PERCENT
} r_wdta_WindowSize_t;


/***********************************************************
  Enum: r_wdta_Parameter_t

  Timer_WDT macro configuration data

   r_wdta_TimeInterval             - see <r_wdta_TimeInterval_t> 
   r_wdta_Interrupt75_Percent_t    - see <Interrupt75_Percent>
   r_wdta_ErrorMode_t              - see <ErrorMode>
   r_wdta_WindowSize_t             - see <WindowSize>                                    
*/     

typedef struct {
    r_wdta_TimeInterval_t           TimeInterval; 
    r_wdta_Interrupt75_Percent_t    Interrupt75;
    r_wdta_ErrorMode_t              ErrorMode;
    r_wdta_WindowSize_t             WindowSize;
                                  
} r_wdta_Parameter_t;

/***********************************************************
  Section: Global API Functions
*/

/***********************************************************
  Function: R_WDTA_Init

  Driver init function. Provides a complete initialization of the the WD.
  Sets the configuration options as defined in the parameter structure.

  Parameters:
  Unit       - Instance number
  Config     - see: <r_wdta_Parameter_t>
 
  Returns:
  see: <r_wdta_Error_t>
*/

r_wdta_Error_t R_WDTA_Init (uint32_t Unit, r_wdta_Parameter_t *Config);



/***********************************************************
  Function: R_WDTA_Calc_RefValue

  This function can be used to control the WDTA_REF register.
  
  An one Bit shift left is made and the new ref value will be returned. 

  Parameters:
  Unit       - Instance number
  VacValue   - last VAC trigger value
 
  Returns:
  New REF value
*/
uint8_t R_WDTA_Calc_RefValue (uint32_t Unit, uint8_t VacValue);

/***********************************************************
  Function: R_WDTA_Trigger

  Provides the nexcessary trigger for the WD.  If enabled for the VAC 
  then this will provide the necessary sequence data as well.
  
  Parameters:
  Unit       - Instance number
  VACEnabled - VAC option enabled
  

  Returns:
  see: <r_wdta_Error_t>
*/
r_wdta_Error_t R_WDTA_Trigger (uint32_t Unit, uint8_t VacEnabled);


/***********************************************************
  Function: R_WDTA_CalcRefValue
  
  Calculates new reference Value out of last VAC value
  Parameters:
  Unit          - Instance number
  VacValue      - Last VAC value 
  
  Returns:
  see: <r_wdta_Error_t>
*/
uint8_t R_WDTA_CalcRefValue(uint32_t Unit, uint8_t VacValue);


/***********************************************************
  Function: R_WDTA_Isr
*/
void R_WDTA_Isr(uint32_t Unit);


/***********************************************************
  Function: R_WDTA_NmiIsr
  
 WDTA0 NMI interrupt handling 
 
  Parameters:
  Unit - instance

  Returns:
  void
*/
void R_WDTA_NmiIsr(uint32_t Unit);


/***********************************************************
  Function: R_WDTA_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback) 
                  with two paramerters (unit and error number) 
  
  Returns:
  void
 
*/
void R_WDTA_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error));


/***********************************************************
  Function: R_WDTA_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - What kind of interrupt; see: <r_wdta_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_wdta_Error_t>
*/
r_wdta_Error_t R_WDTA_SetIsrCallback(uint32_t Unit, r_wdta_Interrupt_t IntType, 
                                 void (*Isr)(void));
                           
                           
/***********************************************************
 Function: R_WDTA_DisableInt
  Disable WDT interrupt
  Parameters:
  Unit          - Instance number
  Returns: none
*/ 
void R_WDTA_DisableInt(uint32_t Unit);


/***********************************************************
 Function: R_WDTA_EnableInt
  Enable WDT interrupt
  Parameters:
  Unit          - Instance number
  Returns: none
*/  
void R_WDTA_EnableInt(uint32_t Unit);  
   
   
/***********************************************************
 Function: R_WDTA_WrongTriggerNonVac
  Trigger WDT with wrong trigger value when VAC is not active
  Parameters:
  Unit          - Instance number
  Returns: none
*/    
void R_WDTA_WrongTriggerNonVac(uint32_t Unit);

/***********************************************************
 Function: R_WDTA_WrongTriggerVac
  Trigger WDT with wrong trigger value when VAC is active
  Parameters:
  Unit          - Instance number
  value         - trigger value to be written
  Returns: none
*/    
void R_WDTA_WrongTriggerVac(uint32_t Unit, uint32_t Value);


/***********************************************************
 Function: R_WDTA_ReadEvacRegister
  To read EVAC register
  Parameters:
  Unit          - Instance number
  Returns: uint8_t
*/ 
uint8_t R_WDTA_ReadEvacRegister(uint32_t Unit);


/***********************************************************
 Function: R_WDTA_ReadRefRegister
  To read REF register
  Parameters:
  Unit          - Instance number
  Returns: uint8_t
*/ 
uint8_t R_WDTA_ReadRefRegister(uint32_t Unit);

#ifdef __cplusplus
}
#endif

#endif /* WDTA_API_H_  */
