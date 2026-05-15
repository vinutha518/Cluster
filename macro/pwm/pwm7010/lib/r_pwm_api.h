/*
****************************************************************************
PROJECT : PWM driver
FILE    : $Id: r_pwm_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================
DESCRIPTION
Driver for PWM macro
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

#ifndef PWM_API_H
#define PWM_API_H



#ifdef __cplusplus
extern "C" {
#endif

/* This is a temporary solution for some golbal variables used by many modules 
   The goal shuld be to provide read/write functions instead */
#undef R_EXTERN
#ifndef  PWM_MAIN_GLOBALS_
#define R_EXTERN extern 
#else
#define R_EXTERN
#endif

#include "r_config_pwm.h"
/*******************************************************************************
  Title: PWM7010 API Functions
*/

/*******************************************************************************
  Section: Global constants
*/
/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_pwm_Error_t

  ADCA macro driver error code.

  Values:
   R_PWM_ERR_OK              - No error
   R_PWM_ERR_NG              - Unspecified error   
   R_PWM_ERR_RANGE           - Range error
   R_PWM_UNINIT              - API service called without module initialization
   R_PWM_BUSY                - start conversion called when the conversion is already running
   R_PWM_ALREADY_INITIALIZED - Init called while ADC is already initialized
   R_PWM_PARAM_CONFIG        - Init called with incorrect configuration
   R_PWM_WRONG_TRIGG_SRC     - trigger service function does not match configuration
   R_PWM_BUFFER_UNINIT       - uninitialised result buffer 
   R_PWM_ERR_LAST            - Delimiter 
*/

typedef enum 
{
    R_PWM_ERR_OK                    = 0x00u,
    R_PWM_ERR_NG                    = 0x01u,
    R_PWM_ERR_RANGE                 = 0x02u,
    R_PWM_UNINIT                    = 0x0Au,
    R_PWM_BUSY                      = 0x0Bu,
    R_PWM_ALREADY_INITIALIZED       = 0x0Du,
    R_PWM_PARAM_CONFIG              = 0x0Eu,
    R_PWM_WRONG_TRIGG_SRC           = 0x17u,
    R_PWM_BUFFER_UNINIT             = 0x19u,
    R_PWM_INVALID_DATABASE          = 0xEFu,
    R_PWM_ERR_LAST
} r_pwm_Error_t;


/*******************************************************************************
  Type: r_pwm_InitFlag_t

  Device Initialization Flag

  Members:
  R_PWM_NO_INIT      - No initialization for ADC
  R_PWM_INIT         - ADC is initialized

*/

typedef enum
{
    R_PWM_NO_INIT = 0u,
    R_PWM_INIT
} r_pwm_InitFlag_t;


/*******************************************************************************
  Enum: r_pwm_Counter_t

  PWM macro Counters.

  Values: 
    R_PWM_CNT_0      - PWM Counter 0
    R_PWM_CNT_1      - PWM Counter 1
    R_PWM_CNT_2      - PWM Counter 2
    R_PWM_CNT_3      - PWM Counter 3
    R_PWM_CNT_LAST   - Maximum Number for PWM Counter

*/

typedef enum
{
    R_PWM_CNT_0 = 0u,
    R_PWM_CNT_1,
    R_PWM_CNT_2,
    R_PWM_CNT_3,
    R_PWM_CNT_LAST
} r_pwm_Counter_t;


/*******************************************************************************
  Enum: r_pwm_CntMode_t

  PWM macro Counter Mode.

  Values:
  R_PWM_CNT_DISABLE       - Disable Couter
  R_PWM_CNT_ENABLE        - Enable Couter
*/

typedef enum
{    
    R_PWM_CNT_DISABLE = 0,
    R_PWM_CNT_ENABLE
} r_pwm_CntMode_t;

/*******************************************************************************
  Enum: r_pwm_Emu_t

  PWM macro Stops/Continous during emulation break.

  Values:
  R_PWM_SVSDIS_EFECTIVE   - halts with debugger break
  R_PWM_SVSDIS_IGNORED    - continues                
*/

typedef enum
{    
    R_PWM_SVSDIS_EFECTIVE = 0,
    R_PWM_SVSDIS_IGNORED
} r_pwm_Emu_t;

/*******************************************************************************
  Struct: r_pwm_CounterCfg_t

  PWM Counter Configuration.

  Values:
    CounterMode           - Counter infomation
    ClkCyc_Hz             - Couter Clock
*/

typedef struct
{    
    r_pwm_CntMode_t   CounterMode;
    uint32_t          ClkCyc_Hz;
} r_pwm_CounterCfg_t;


/*******************************************************************************
  Enum: r_pwm_ChannleMode_t

  PWGA output channels en/disable.

  Values:
  R_PWM_Channel_DISABLE     - Disable PWGA Channel
  R_PWM_Channel_ENABLE      - Enable PWGA Channel
*/

typedef enum
{    
    R_PWM_Channel_DISABLE    = 0,
    R_PWM_Channel_ENABLE
} r_pwm_ChannleMode_t;


/*******************************************************************************
  Enum: r_pwm_SimultaneousRw_t

  PWGA Simultaneous rewrite.

  Values:
    R_PWM_SRWR_ON    - Simultaneous rewrite is on
    R_PWM_SRWR_OFF   - Simultaneous rewrite is off, wait for complation
*/

typedef enum
{    
    R_PWM_SRWR_OFF   = 0,
    R_PWM_SRWR_ON
} r_pwm_SimultaneousRw_t;

/*******************************************************************************
  Struct: r_pwm_ChannelCfg_t

  PWM Channel Configure.

  Members:
    ChMode       - see <r_pwm_ChannleMode_t>
    Delay_100    - Proportion of the PWGA signal * 100
    Duty         - Proportion of the PWGA signal of each period
    TriggerPro   - Proportion for Trigger of the PWGA signal
    Counter      - Couter selsction (PWBA) of the PWGA signal, see <r_pwm_Counter_t>
    SRwr         - PWGA Simultaneous rewrite, see <r_pwm_SimultaneousRw_t>
*/

typedef struct
{    
    r_pwm_ChannleMode_t     ChMode;
    uint32_t                Delay_100;
    uint32_t                Duty;       
    uint32_t                TriggerPro;   
    r_pwm_Counter_t         Counter;
    r_pwm_SimultaneousRw_t  SRwr;
} r_pwm_ChannelCfg_t;


/*******************************************************************************
  Enum: r_pwm_UllCh_t

  PWM macro Counters.

  Values: 
    R_PWM_NO_ULL     - No Upper Lower Limit
    R_PWM_ULL_0      - ADC Upper Lower Limit Check 0
    R_PWM_ULL_1      - ADC Upper Lower Limit Check 1
    R_PWM_ULL_2      - ADC Upper Lower Limit Check 2
    R_PWM_CNT_LAST   - Maximum Number for ADC Upper Lower Limit

*/

typedef enum
{
    R_PWM_NO_ULL = 0,
    R_PWM_ULL_0,
    R_PWM_ULL_1,
    R_PWM_ULL_2,
    R_PWM_ULL_LAST
} r_pwm_UllCh_t;


/*******************************************************************************
  Enum: r_pwm_AdcaUnit_t

  PWM macro Counters.

  Values: 
    R_PWM_ADC_0      - ADCA 0
    R_PWM_ADC_1      - ADCA 1

*/

typedef enum
{
    R_PWM_ADC_0 = 0,
    R_PWM_ADC_1  
} r_pwm_AdcaUnit_t;

/*******************************************************************************
  Enum: r_pwm_MpxMode_t

  PWSA Multiplexer en/disable.

  Values:
    R_PWM_MPX_DIS   - Multiplexer is not used
    R_PWM_MPX_EN    - Multiplexer is used
*/

typedef enum
{    
    R_PWM_MPX_DIS   = 0,
    R_PWM_MPX_EN
} r_pwm_MpxMode_t;

/*******************************************************************************
  Enum: r_pwm_TrgMode_t

  PWSA en/disable.

  Values:
    R_PWM_TRG_OFF   - PWSA is disabled
    R_PWM_TRG_ON    - PWSA is enabled
*/

typedef enum
{    
    R_PWM_TRG_OFF   = 0,
    R_PWM_TRG_ON
} r_pwm_TrgMode_t;

/*******************************************************************************
  Struct: r_pwm_DiagCfg_t

  PWM Channel Configure.

  Members:
    UllCheck    - Upper Lower Limit for Diag Signal
    AdcaUnit    - ADCA Unit which is connected to
    MpxMode     - If Multiplexer is used
    MpxAdr      - Multiplexer Address
    PhyChannel  - Connected Physical Channel in ADCA 
*/

typedef struct
{    
    r_pwm_UllCh_t     UllCheck;  
    r_pwm_AdcaUnit_t  AdcaUnit;
    r_pwm_MpxMode_t   MpxMode;
    uint16_t          MpxAdr;       
    uint16_t          PhyChannel;
} r_pwm_DiagCfg_t;

/*******************************************************************************
  Struct: r_pwm_Config_t

  PWM Channel Configure.

  Members:
    PWM_Emu                   - see <r_pwm_Emu_t>
    CntCfg[R_PWM_CNT_LAST]    - Counter Configuration            
    ChCfg[R_PWM_PWGA_NUM]     - PWGA Output Channel Configuration
    DiagCfg[R_PWM_PWGA_NUM]   - PWM Diagnose Configuration
    PWSAMode                  - PWSA trigger, see <r_pwm_TrgMode_t>
*/

typedef struct
{    
    r_pwm_Emu_t          PWM_Emu;
    r_pwm_CounterCfg_t   CntCfg[R_PWM_CNT_LAST];
    r_pwm_ChannelCfg_t   ChCfg[R_PWM_PWGA_NUM];
    r_pwm_DiagCfg_t      DiagCfg[R_PWM_PWGA_NUM];
    r_pwm_TrgMode_t      PWSAMode;
} r_pwm_Config_t;

/*******************************************************************************
  Section: Global Variables
*/
/*******************************************************************************
  Variable: r_pwm_InitFlag

  PWM Initialization Flag.
*/
R_EXTERN r_pwm_InitFlag_t r_pwm_InitFlag[R_PWM_MACRO_NUM];


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_PWM_Init

  Driver init function.

  Parameters:
  PwmCfg       - Pointer to the PWM configuration, see <r_pwm_Config_t>
 
  Returns:
  see: <r_pwm_Error_t>
*/

r_pwm_Error_t R_PWM_Init(uint32_t Unit, r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_ResetPWBACfg
  
  Initialize the PWBA configure parameters
*/
void R_PWM_ResetPWBACfg (r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_ResetPWGACfg
  
  Initialize the PWGA configure parameters
*/
void R_PWM_ResetPWGACfg (r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_ResetPWSACfg
  
  Initialize the PWSA configure parameters
*/
void R_PWM_ResetPWSACfg (r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_ResetCfg
  
  Initialize the configure parameters
*/
void R_PWM_ResetCfg (r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_DeInit

  Driver deinit function.

  Parameters:

  Returns:
  see: <r_pwm_Error_t>
*/

r_pwm_Error_t R_PWM_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_PWM_PWBAConfig

  Config the PWM macro PWBA Registers.

  Parameters:
    CntCfg     - Pointer to the PWM configuraton, see <r_pwm_Config_t>
 
  Returns:
  see: <r_pwm_Error_t>
*/
r_pwm_Error_t R_PWM_PWBAConfig(uint32_t Unit, r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_PWGAConfig

  Config the PWM macro PWGA Registers.

  Parameters:
    CntCfg     - Pointer to the configuraton, see <r_pwm_CounterCfg_t>
 
  Returns:
  see: <r_pwm_Error_t>
*/
r_pwm_Error_t R_PWM_PWGAConfig(uint32_t Unit, r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_PWSAConfig

  Config the PWM macro PWSA Registers.

  Parameters:
    CntCfg     - Pointer to the configuraton, see <r_pwm_CounterCfg_t>
 
  Returns:
  see: <r_pwm_Error_t>
*/
r_pwm_Error_t R_PWM_PWSAConfig(uint32_t Unit, r_pwm_Config_t * PwmCfg);


/*******************************************************************************
  Function: R_PWM_DbgSVSDIS
  
  En/Disable the SVSTOP
  
  Parameters:
  Unit          - Instance number
  ADC_Emu       - En/Disable SVSTOP
  
  Returns:
  void
*/
void R_PWM_DbgSVSDIS (uint32_t Unit, r_pwm_Config_t * PwmCfg);

/*******************************************************************************
  Function: R_PWM_PWBAStop

  Stop Counter.

  Parameters:
    CntNum     - Counter Number, see <r_pwm_Counter_t>
 
  Returns:
  see: <r_pwm_Error_t>
*/
r_pwm_Error_t R_PWM_PWBAStop(uint32_t Unit, r_pwm_Counter_t CntNum);

/*******************************************************************************
  Function: R_PWM_Str

  Start/Stop PWM Output Signal.

  Parameters:
    Unit      - Channel Number.
 
  Returns:
  see: <r_pwm_Error_t>
*/
r_pwm_Error_t R_PWM_Str(uint32_t Unit, r_pwm_Config_t * PwmCfg);



#ifdef __cplusplus
}
#endif

#endif /* R_PWM_API_H */
