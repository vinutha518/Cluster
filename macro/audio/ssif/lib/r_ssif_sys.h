/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_ssif_sys.h 4630 2015-01-13 16:48:47Z golczewskim $
============================================================================ 
DESCRIPTION
Generic SSIF driver
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

#ifndef SSIF_SYS_H_
#define SSIF_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic SSIF Support Functions

  The generic SSIF driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SSIF_SYS_HardwareInit

  Setup the HW part for the generic SSIF.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ssif_Error_t>
*/

r_ssif_Error_t R_SSIF_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_SYS_HardwareDeInit

  Deinit of the HW part for the generic SSIF.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_ssif_Error_t>
*/

r_ssif_Error_t R_SSIF_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SSIF_SYS_BaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_SYS_EnableInt

  Enable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eessif_Interrupt_t>
  
  Returns:
  void
*/

void R_SSIF_SYS_EnableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SSIF_SYS_DisableInt

  Disable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eessif_Interrupt_t>
  
  Returns:
  void
*/

void R_SSIF_SYS_DisableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SSIF_SYS_GetClock
  
  Get the macro audio clock frequency of the SSIF
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Frequency in MHz
*/

uint32_t R_SSIF_SYS_GetClock(uint32_t Unit);


/*******************************************************************************
  Function: R_SSIF_SYS_SetClock
  
  Set the macro audio clock frequency of the SSIF
  
  Parameters:
  Unit       - Instance number
  Frequency  - Frequency to set (in MHz)
  
  Returns:
  void
*/

uint32_t R_SSIF_SYS_SetClock(uint32_t Unit, uint32_t Frequency);


/*******************************************************************************
  Function: R_SSIF_SYS_ErrorHandler
  Low level error handler, called in case there is no user error handled was 
  assigned for this macro 
  
  This function will just call R_DEV_ErrrorHandler (avoid having to assing 
  a dedicated low level error handler for all the macros)    
    
  Parameters:
  Unit       - Instance number
  Error      - detected error    
  
  Returns:
  void  
*/

void R_SSIF_SYS_ErrorHandler(uint32_t Unit, r_ssif_Error_t Error);


#ifdef __cplusplus
}
#endif


#endif /* SSIF_SYS_H_ */
