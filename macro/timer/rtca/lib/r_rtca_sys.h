/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_rtca_sys.h 2385 2014-04-11 15:12:34Z michael.golczewski $
============================================================================ 
DESCRIPTION
Generic RTCA driver
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

#ifndef RTCA_SYS_H_
#define RTCA_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic RTCA Support Functions

  The generic RTCA driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_RTCA_SYS_HardwareInit

  Setup the HW part for the generic RTCA.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_RTCA_SYS_HardwareDeInit

  Deinit of the HW part for the generic RTCA.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_RTCA_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_RTCA_SYS_BaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_RTCA_SYS_EnableInt

  Enable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eertca_Interrupt_t>
  
  Returns:
  void
*/

void R_RTCA_SYS_EnableInt(uint32_t Unit, r_rtca_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_RTCA_SYS_DisableInt

  Disable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eertca_Interrupt_t>
  
  Returns:
  void
*/

void R_RTCA_SYS_DisableInt(uint32_t Unit, r_rtca_Interrupt_t Interrupt);


#ifdef __cplusplus
}
#endif


#endif /* RTCA_SYS_H_ */
