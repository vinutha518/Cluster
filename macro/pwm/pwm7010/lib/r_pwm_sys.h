/*
****************************************************************************
PROJECT : PWM driver
FILE    : $Id: r_pwm_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
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
/******************************************************************************/

#ifndef R_SYS_PWM_H
#define R_SYS_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: PWM7010 API support functions (device dependant part)
*/

/*******************************************************************************
  Section: Global Functions  
*/

/*******************************************************************************
  Function: R_SYS_PWBA_GetBaseAddr

  Returns the base address of the dedicated macro .
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_PWBA_GetBaseAddr(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_PWGA_GetBaseAddr

  Returns the base address of the dedicated macro .
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_PWGA_GetBaseAddr(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_PWSA_GetBaseAddr

  Returns the base address of the dedicated macro .
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_PWSA_GetBaseAddr(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_PWM_MaxPhyChannelNum

  Get the Maximum ADC Channel Number for each Unit(for PWGA Configuration).
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  uint8_t
*/
uint8_t R_SYS_PWM_MaxPhyChannelNum(uint32_t AdcUnit);

/*******************************************************************************
  Function: R_SYS_PWM_EmuBrkInit
  
  Enable the SVSTOP
  
  Returns:
  void
*/

void R_SYS_PWM_EmuBrkInit (r_pwm_Emu_t PWM_Emu);



#ifdef __cplusplus
}
#endif

#endif /* R_SYS_PWM_H */
