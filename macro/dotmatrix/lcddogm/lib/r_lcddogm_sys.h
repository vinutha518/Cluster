/*
****************************************************************************
PROJECT : lcd display driver for dogm displays (DX4 applboard)
FILE    : $Id: r_lcddogm_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Low Level Driver API for LCD Dogm Display
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2013
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

#ifndef  LCDDOGM_SYS_H_
#define  LCDDOGM_SYS_H_

/***********************************************************
  Title: LcdDogm Support Functions

  The generic LcdDogm driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/***********************************************************
  Function: R_LCDDOGM_Sys_PortInit
  
  Initializes LcdDogm 
  
  Parameters:
  portdata              - Contains the Pin configuration for
                          the LCD Display                         
  Returns:
  see: <r_lcdDogm_Error_t>

*/
r_lcdDogm_Error_t R_LCDDOGM_Sys_PortInit(r_lcdDogm_PortConfig_t pconf);

/***********************************************************
  Function: R_LCDDOGM_Sys_SetDirection
  
  Sets LcdDogm direction (intput, output)
  
  Parameters:
  dir       - Direction, see <r_lcdDogm_Direction_t>
  
  Returns:
  see: <r_lcdDogm_Error_t>

*/
r_lcdDogm_Error_t R_LCDDOGM_Sys_SetDirection(r_lcdDogm_Direction_t dir);

/***********************************************************
  Function: R_LCDDOGM_Sys_PutByte
  
   Set Byte with given value
  
  Parameters:
  value    - Value to be written.
            
  Returns:              
  see: <r_lcdDogm_Error_t>

*/
r_lcdDogm_Error_t R_LCDDOGM_Sys_PutByte(uint8_t value);

/***********************************************************
  Function: R_LCDDOGM_Sys_GetByte
  
  Get Byte current value 
  
  Parameters:
  void
  
  Returns: 
  current byte value

*/
uint8_t R_LCDDOGM_Sys_GetByte(void);

/***********************************************************
  Function: R_LCDDOGM_Sys_SetControl
  
  Set LCDdogm control mode 
  
  Parameters:
  ctrl         - see <r_lcdDogm_Control_t>
                                           
  Returns:             
  see: <r_lcdDogm_Error_t>

*/
r_lcdDogm_Error_t R_LCDDOGM_Sys_SetControl(r_lcdDogm_Control_t ctrl);


#endif /* _LCDDOGM_SYS_H_ */
