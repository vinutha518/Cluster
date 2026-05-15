/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_sg_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic SG driver
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

#ifndef SG_SYS_H_
#define SG_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic SG Support Functions

  The generic SG driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/***********************************************************
  Section: Local Defines
*/

#define R_SG_MIN_PWM_FREQ 32000
#define R_SG_MAX_PWM_FREQ 64000


/***********************************************************
  Title: SG Support Functions

  The generic SG driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/



/***********************************************************
  Section: Global Functions
*/


/***********************************************************
  Function: R_SG_SYS_GetSignalName

  Gives back device dependent Signal name to driver wraper function.
  
  Parameters:
  Unit              - Instance number
  eeIis_Signal_t    - Signal Enum see:<eeIis_Signal_t>
  
  Returns:
  Signal name string
*/

char * R_SYS_SG_GetSignalName(uint32_t Unit, r_sg_Signal_t Signal);


/***********************************************************
  Function: R_SG_SYS_GetSignalDir

  Gives back device port direction to driver wraper function.
  
  Parameters:
  Unit              - Instance number
  eeIis_Signal_t    - Signal Enum see:<eeIis_Signal_t>
  
  Returns:
  Signal name string
*/

int32_t R_SYS_SG_GetSignalDir(uint32_t Unit, r_sg_Signal_t Signal);


/***********************************************************
  Function: R_SG_SYS_PortInit

  Configurates ports for SG. 
  
  Parameters:
  Unit       - Instance number
  Connection - alternative pin connection
  
  Returns:
  see: <eeSg_Error_t>
*/
r_sg_Error_t R_SYS_SG_PortInit(uint32_t Unit, int32_t Connection);

/***********************************************************
  Function: R_SG_SYS_PortDeInit

  Deinit of SG pins.
  
  Sets port pins of dedicated channel back to default value.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <eeSg_Error_t>
*/

r_sg_Error_t R_SYS_SG_PortDeInit(uint32_t Unit);



/***********************************************************
  Function: R_SG_SYS_GetClock

  Gives back clock frequency of dedicated clock input. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Clock frequency
*/

uint32_t R_SYS_SG_GetClock(uint32_t Unit);


/***********************************************************
  Function: R_SG_SYS_IntCInit

  Init of SG Interrupt for dedicated channel
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_SG_IntCInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SG_SYS_HardwareInit

  Setup the HW part for the generic SG.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SYS_SG_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SG_SYS_HardwareDeInit

  Deinit of the HW part for the generic SG.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SYS_SG_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SG_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_SG_GetBaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SG_SYS_EnableInt

  Enable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eesg_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_SG_EnableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SG_SYS_DisableInt

  Disable the interrupt handling for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <eesg_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_SG_DisableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt);


#ifdef __cplusplus
}
#endif


#endif /* SG_SYS_H_ */
