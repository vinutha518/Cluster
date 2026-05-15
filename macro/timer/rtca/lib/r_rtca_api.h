/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_rtca_api.h 2385 2014-04-11 15:12:34Z michael.golczewski $
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

#ifndef RTCA_API_H_
#define RTCA_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: RTCA Driver API 
  
  An application using RTCA should only need to include this r_rtca_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_RTCA_VERSION_HI and R_RTCA_VERSION_LO

  Driver version information  
*/

#define R_RTCA_VERSION_HI 0
#define R_RTCA_VERSION_LO 1


/*******************************************************************************
  Section: Global Types
*/

/* option to use SVSTOP */
#define USE_SVSTOP        (0x0000)
#define DO_NOT_USE_SVSTOP (0x0080)
#define SVSTOP            (USE_SVSTOP)


/*******************************************************************************
  Enum: r_rtca_Error_t

  RTCA macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
    R_RTCA_ERR_OK         - No error
    R_RTCA_ERR_NG         - Unspecified error
    R_RTCA_ERR_INSTANCE   - Unit, instance not supported
  */

typedef enum
{
    R_RTCA_ERR_OK         = 0x00,
    R_RTCA_ERR_NG         ,
    R_RTCA_ERR_RANGE      ,
    R_RTCA_ERR_INSTANCE   ,
    R_RTCA_ERR_LOCKED     ,
    R_RTCA_ERR_NOTLOCKED  
} r_rtca_Error_t;


/*******************************************************************************
  Enum: r_rtca_Interrupt_t

  Rtca macro interrupt type. 
  
  List of all available interrupts for this macro.

  Values:
    R_RTCA_INT_1S         - 1 Second Interval Interrupt
    R_RTCA_INT_AL         - Alarm Interrupt
    R_RTCA_INT_R          - Fixed Interval Interrupt
    R_RTCA_INT_NUM        - Delimiter 
  */

typedef enum
{
    R_RTCA_INT_1S = 0,
    R_RTCA_INT_AL,
    R_RTCA_INT_R,
    R_RTCA_INT_NUM
} r_rtca_Interrupt_t;


/***********************************************************
  Enum: r_rtca_Mode_t

  RTCA macro modes

  Values:
    R_RTCA_32K_MODE    - 32768 Hz mode
    R_RTCA_FREQ_MODE   - Frequency mode

*/

typedef enum 
{    
    R_RTCA_32K_MODE = 0,
    R_RTCA_FREQ_MODE
} r_rtca_Mode_t;


/***********************************************************
  Enum: r_rtca_CounterStatus_t

  RTCA macro counter status.

  Indicates that the counter is either running or has been stopped.

  Values:
    R_RTCA_CNT_RUN     - All Clock Counters running
    R_RTCA_CNT_STOPPED - All Clock Counters stopped
*/

typedef enum 
{    
    R_RTCA_CNT_RUN = 0,
    R_RTCA_CNT_STOPPED
} r_rtca_CounterStatus_t;


/***********************************************************
  Enum: r_rtca_HourFormat_t

  RTCA macro hour format.

  Either 12 or 24 hour format is used.

  Values:
    R_RTCA_12H     - 12 hour format
    R_RTCA_24H     - 24 hour format
*/

typedef enum 
{    
    R_RTCA_12H = 0,
    R_RTCA_24H,
    R_RTCA_NON
} r_rtca_HourFormat_t;


/***********************************************************
  Enum: r_rtca_State_t

  Common type to set and erase flags.

  Values:
    R_RTCA_DISABLE    - Disable
    R_RTCA_ENABLE     - Enable
*/

typedef enum 
{    
    R_RTCA_DISABLE = 0,
    R_RTCA_ENABLE
} r_rtca_State_t;


/***********************************************************
  Enum: r_rtca_AmPm_t

  RTCA macro AM/PM detection.

  AM/PM detection in 12 hour format mode.

  Values:
    R_RTCA_AM     - AM
    R_RTCA_PM     - PM

*/

typedef enum 
{    
    R_RTCA_AM = 0,
    R_RTCA_PM,
} r_rtca_AmPm_t;


/***********************************************************
  Enum: r_rtca_IntInterv_t

  RTCA macro interval interrupt generation

  Values:
    R_RTCA_NO_INT          - No interrupt request generation
    R_RTCA_025SEC_INT      - Every 0.25 seconds
    R_RTCA_05SEC_INT       - Every 0.5 seconds
    R_RTCA_1SEC_INT        - Every second
    R_RTCA_1MIN_INT        - Every minute
    R_RTCA_1H_INT          - Every hour
    R_RTCA_1DAY_INT        - Every day 
    R_RTCA_1MON_INT        - Every month
*/

typedef enum 
{    
    R_RTCA_NO_INT = 0,
    R_RTCA_025SEC_INT,
    R_RTCA_05SEC_INT,
    R_RTCA_1SEC_INT,
    R_RTCA_1MIN_INT,
    R_RTCA_1H_INT,
    R_RTCA_1DAY_INT,
    R_RTCA_1MON_INT
} r_rtca_IntInterv_t;


/***********************************************************
  Enum: r_rtca_CntReg_t
  
  Kind of time information
  
  Values:
    R_RTCA_DIRECT  - Counter register
    R_RTCA_BUFFER  - Counter Buffer register

*/

typedef enum 
{    
    R_RTCA_DIRECT = 0,
    R_RTCA_BUFFER
} r_rtca_CntReg_t;


/***********************************************************
  Enum: r_rtca_Method_t
  
  Kind of time information
  
  Values:
    R_RTCA_SINGLE 
    R_RTCA_GROUP  

*/

typedef enum 
{    
    R_RTCA_SINGLE = 0,
    R_RTCA_GROUP
} r_rtca_Method_t;


/***********************************************************
  Enum: r_rtca_CtrGroup_t
  
  Time or Date selection
  
  Values:
    R_RTCA_TIMEVAL - Only time information
    R_RTCA_DATE    - Only date information      
    R_RTCA_BOTH    - Time and Date

*/

typedef enum 
{    
    R_RTCA_TIMEVAL = 0,
    R_RTCA_DATE,
    R_RTCA_BOTH  
} r_rtca_CtrGroup_t;


/***********************************************************
  Struct: r_rtca_Time_t

  Time data
    Sec     - Seconds
    Min     - Minutes
    H       - Hours
    Format  - 24 / 12h Format see: <r_rtca_HourFormat_t>
    AmPm    - Hours are AM or PM see: <r_rtca_AmPm_t> 
    Method  - Single or Group format see: <r_rtca_Method_t>
    RegType - Direct counter or counter Buffer see: <r_rtca_CntReg_t>

*/     

typedef struct 
{
    uint8_t             Sec;
    uint8_t             Min;
    uint8_t             H;
    r_rtca_HourFormat_t Format;
    r_rtca_AmPm_t       AmPm;
    r_rtca_Method_t     Method;
    r_rtca_CntReg_t     RegType;
} r_rtca_Time_t;


/***********************************************************
  Struct: r_rtca_Date_t

  Date data_
    Week        - Week of the month
    Day         - Day of the Week       
    Month       - Month  
    Year        - Year    
    Method      - Single or Group format see: <r_rtca_Method_t>
    RegType     - Direct counter or counter Buffer see: <r_rtca_CntReg_t>

*/     

typedef struct 
{
    uint8_t             Week;
    uint8_t             Day;
    uint8_t             Month;
    uint8_t             Year;
    r_rtca_Method_t     Method;
    r_rtca_CntReg_t     RegType;
} r_rtca_Date_t;


/***********************************************************
  Struct: r_rtca_Data_t

  Time and Date information (DEC coded)
  
  Parameters:
    
    Sec     - Seconds
    Min     - Minutes
    H       - Hours
    Week    - 
    Day     - 
    Month   - 
    Method  - 
    Group   - 

*/     

typedef struct 
{
    uint8_t             Sec;
    uint8_t             Min;
    uint8_t             H;
    uint8_t             Week;
    uint8_t             Day;
    uint8_t             Month;    
    uint8_t             Year;
    r_rtca_HourFormat_t Format;
    r_rtca_AmPm_t       AmPm;
    r_rtca_Method_t     Method;
    r_rtca_CtrGroup_t   Group; 
} r_rtca_Data_t;


/***********************************************************
  Struct: r_rtca_Correction_t

  Correction data in 32768 Hz mode 

  Parameters:

  Val       - Correction value (6Bit)
  Interv    - Correction interval - 20sec(0) / 60sec(1)
  Sign      - Increment(0) / Decrement(1)
  
*/     

typedef struct 
{
    uint8_t    Val;
    uint8_t    Interv;
    uint8_t    Sign;   
} r_rtca_Correction_t;


/***********************************************************
  Struct: r_rtca_IntConf_t

  Interrupt configuration Parameter

  Parameters:

  AlarmInt   - Alarm interrupt
  SecInt     - 1 second interrupt
  IntervInt  - Interval interrupt

*/     

typedef struct 
{    
    r_rtca_State_t      AlarmInt;
    r_rtca_State_t      SecInt;
    r_rtca_IntInterv_t  IntervInt;
} r_rtca_IntConf_t;


/***********************************************************
  Struct: r_rtca_Parameter_t

  Rtca macro configuration data

  Mode          - Operating Mode see:  <r_rtca_Mode_t>
  Hours         - Setup options see:   <r_rtca_Option_t>
  CompVal       - Compare value (frequency mode)
  CorrectVal    - 32768 Hz correction values see:   <r_rtca_Correction_t>
  
*/     

typedef struct 
{
    r_rtca_Mode_t           Mode;  
    r_rtca_HourFormat_t     Format;
    uint32_t                CompVal;
    r_rtca_Correction_t     CorrectVal; 
} r_rtca_Parameter_t;


/***********************************************************
  Struct: r_rtca_Alarm_t

  Alarm time setting
    
     Minute     - Minute of Alarm
     Hour       - Hour of Alarm
     Day        - Days of week: Day0 0x01, Day4 0x04...
*/      

typedef struct 
{  
    uint8_t             Minute;     
    uint8_t             Hour;
    uint8_t             Day; 
    r_rtca_HourFormat_t Format;
    r_rtca_AmPm_t       AmPm;
} r_rtca_Alarm_t;


/*******************************************************************************
  Section: Global API Functions
*/

/***********************************************************
  Function: R_RTCA_CtrCtl

  Start / Stop sub-counter

  Parameters:
  Unit       - Instance number
  EnDis      - 

  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_CtrCtl(uint32_t Unit, r_rtca_State_t EnDis);


/***********************************************************
  Function: R_RTCA_SetTime

  Central function to set a time registers.

  Parameters:
  Unit       - Instance number
  Data       - Time and Date information, see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SetTime(uint32_t Unit, r_rtca_Data_t *Data);


/***********************************************************
  Function: R_RTCA_UpdateTime

  Central function to update all time registers.

  Parameters:
  Unit       - Instance number
  Data       - Time and Date information, see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_UpdateTime(uint32_t Unit, r_rtca_Data_t *Data);


/***********************************************************
  Function: R_RTCA_ReadCounterBuf

  Read value of various time buffer registers.

  Parameters:
  Unit       - Instance number
  Data       - Time and Date information, see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_ReadCounterBuf(uint32_t Unit, r_rtca_Data_t *Data);


/***********************************************************
  Function: R_RTCA_ReadCounter

  Read value of various time registers.

  Parameters:
  Unit       - Instance number
  Data       - Time and Date information, see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_ReadCounter(uint32_t Unit, r_rtca_Data_t *Data);


/***********************************************************
  Function: R_RTCA_Set

  This function writes BCD coded data to various time registers.

  Parameters:
  Unit       - Instance number
  Data       - see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_Set(uint32_t Unit, r_rtca_Data_t *Data);


/***********************************************************
  Function: R_RTCA_Get

  This function reads BCD coded data from various time registers.

  Parameters:
  Unit       - Instance number
  Data       - see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_Get(uint32_t Unit, r_rtca_Data_t *Data);


/***********************************************************
  Function: R_RTCA_GetSubCounterVal

  Gives back SubCounter value.

  Parameters:
  Unit       - Instance number

  Returns:
  SubCounter value
*/

uint32_t  R_RTCA_GetSubCounterVal(uint32_t Unit);


/***********************************************************
  Function: R_RTCA_SetInt

  Enable dedecated Interrupt

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SetInt(uint32_t Unit, r_rtca_IntConf_t IntConf);


/***********************************************************
  Function: R_RTCA_OutputCtl

  Enable output pin

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_OutputCtl(uint32_t Unit, r_rtca_State_t State);


/***********************************************************
  Function: R_RTCA_SetCorrection

  Set Counter compare value in 32.768 Hz mode
  
  Parameters:
  Unit          - Instance number
  Val           - Correction value

  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SetCorrection(uint32_t Unit, r_rtca_Correction_t Val);


/***********************************************************
  Function: R_RTCA_SetCompare

  Set Counter compare value in 32.768 Hz mode
  
  Parameters:
  Unit          - Instance number
  CompVal       - Compare value in frequency mode

  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SetCompare(uint32_t Unit, uint32_t CompVal);


/***********************************************************
  Function: R_RTCA_SetAlarm

  Set type of alarm.
  
  Parameters:
  Unit          - Instance number
  Alarm         - Type of alarm, see : <r_rtca_Alarm_t>

  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SetAlarm(uint32_t Unit, r_rtca_Alarm_t Alarm);


/***********************************************************
  Function: R_RTCA_ReadSubCntBuf

  Reads Sub-counter buffer value while counter is running
  
  Parameters:
  Unit       - Instance number

  Returns:
  Value of sub-counter  buffer
*/

uint32_t R_RTCA_ReadSubCntBuf(uint32_t Unit);


/***********************************************************
  Function:R_RTCA_SetSvstop

  Reads Sub-counter buffer value while counter is running
  
  Parameters:
  Unit      - Instance number
  Endis     - Usage of SvStop (1:counter can be stopped) see: <r_rtca_State_t>
  
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SetSvstop(uint32_t Unit, r_rtca_State_t Endis );


/*******************************************************************************
  Function: R_RTCA_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_RTCA_GetVersionStr(void); 


/*******************************************************************************
  Function: R_RTCA_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_RTCA_GetMajorVersion(void); 


/*******************************************************************************
  Function: R_RTCA_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_RTCA_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_RTCA_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_rtca_Error_t>
*/

 r_rtca_Error_t R_RTCA_Init(uint32_t Unit, r_rtca_Parameter_t *Config, r_rtca_Data_t *Data);


/*******************************************************************************
  Function: R_RTCA_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_rtca_Error_t>
*/

 r_rtca_Error_t R_RTCA_DeInit(uint32_t Unit);


/*******************************************************************************
  Group: RTCA Error and ISR Functions
*/

/*******************************************************************************
  Function: R_RTCA_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_RTCA_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));


/*******************************************************************************
  Function: R_RTCA_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Interrupt       - What interrupt,  see: <r_rtca_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_rtca_Error_t>
*/

 r_rtca_Error_t R_RTCA_SetIsrCallback(uint32_t Unit, r_rtca_Interrupt_t Interrupt, 
                                      void (*Isr)(void));
                                 

/***********************************************************
  Function:  R_RTCA_Isr

  Driver interrupt service routine.

  This function shall be called by sevice interrupt handler

  Parameters:
  Unit       - Instance number
  Interrupt  - channel interrupt

  Returns:
  void
*/

void R_RTCA_Isr(uint32_t Unit, r_rtca_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_RTCA_EnableInt

  Enable the RTCA interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_rtca_Interrupt_t>
  
  Returns:
  void
*/

void R_RTCA_EnableInt(uint32_t Unit, r_rtca_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_RTCA_DisableInt

  Disable the RTCA interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_rtca_Interrupt_t>
  
  Returns:
  void
*/

void R_RTCA_DisableInt(uint32_t Unit, r_rtca_Interrupt_t Interrupt);


#ifdef __cplusplus
}
#endif


#endif /* RTCA_API_H_  */
