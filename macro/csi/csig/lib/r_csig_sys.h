/*
****************************************************************************
PROJECT : CSIG driver
FILE ID : $Id: r_csig_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
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

#ifndef CSIG_SYS_H_
#define CSIG_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: CSIG Support Functions

  The generic CSIG driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SYS_CSIG_GetCsigBaseAddr

  Gives back base address of dedicated csig macro channel.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_ t  - Macro base address
*/

uint32_t R_SYS_CSIG_GetCsigBaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIG_GetBaseAddr

  Gives back base address of dedicated chba macro channel.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_ t  - Macro base address
*/

uint32_t R_SYS_CSIG_GetChbbBaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIG_GetClock

  Gives back clock frequency of dedicated clock input. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_ t  - Clock frequency
*/

uint32_t R_SYS_CSIG_GetClock(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_CSIG_IntCInit

  Init of CSIG Interrupt for dedicated channel
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_CSIG_IntCInit(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_CSIG_IntCDeInit

  DeInit (disable all) of CSIG Interrupt for dedicated channel
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/


void R_SYS_CSIG_IntCDeInit(uint32_t Unit) ; 


/*******************************************************************************
  Function: R_SYS_CSIG_EnableInt

  Enable the interrupt controller for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_CSIG_EnableInt(uint32_t Unit, r_csig_Interrupt_t Interrupt);

/*******************************************************************************
  Function: R_SYS_CSIG_GetIntMask

  Get the masking status of the interrupt 'Interrupt'.

  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>

  Returns:
  == 1            - Interrupt masked (disabled) 
  != 1            - Interrupt enabled 
*/

uint8_t R_SYS_CSIG_GetIntMask(uint32_t Unit, r_csig_Interrupt_t Interrupt);

/*******************************************************************************
  Function: R_SYS_CSIG_DisableInt

  Disable the interrupt controller for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csig_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_CSIG_DisableInt(uint32_t Unit, r_csig_Interrupt_t Interrupt);




void R_SYS_CSIG_ForceClearInt(uint32_t Unit, r_csig_Interrupt_t Interrupt, uint8_t Set);

/*******************************************************************************
  Function: R_SYS_CSIG_SetFilterBypass

  To enable filter bypass
  Parameters:
  Unit       - Instance number
  
  Returns:
  1 - unit not available
  0 - enable
*/
uint32_t R_SYS_CSIG_SetFilterBypass(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIG_EnableDTSInt

  Enable DTS interrupt
  
  Parameters:
  Unit         - CsiG unit
  
  Returns:
  void
*/
void R_SYS_CSIG_EnableDTSInt (uint32_t Unit);

#ifdef __cplusplus
}
#endif


#endif /* CSIG_SYS_H_ */
