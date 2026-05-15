/*
****************************************************************************
PROJECT : debug support  
FILE    : $Id: r_dbg_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
AUTHOR  : Michael Golczewski
============================================================================ 
DESCRIPTION
Debug support API 
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

/*******************************************************************************
  Title: Device support for debug API

  Declaration of the device functions required for the debug API
*/


#ifndef DEBUG_SYS_H_
#define DEBUG_SYS_H_


/*******************************************************************************
  Section: Global Functions  

*/

/******************************************************************************* 
  Function: R_SYS_DBG_FindName 

  This function searches for a register name for the given Address
  and returns it   
  Parameters:
  Addr      - register address   
  
  Returns:
  == register name (string) -  if name found  
  == 0                      -  if address unknown            
*/ 
char *  R_SYS_DBG_FindName(uint32_t Addr);

/******************************************************************************* 
  Function: R_SYS_DBG_PrintRegRead32 
  Function: R_SYS_DBG_PrintRegRead16 
  Function: R_SYS_DBG_PrintRegRead8 

  This function returns the read value from the given register
  
  The read result also als printed to stio
 
  Parameters:
  Addr      - register address   
  
  Returns:
  == register value 
*/ 

uint32_t R_SYS_DBG_PrintRegRead32(uint32_t Addr); 
uint16_t R_SYS_DBG_PrintRegRead16(uint32_t Addr); 
uint8_t  R_SYS_DBG_PrintRegRead8(uint32_t Addr); 

/******************************************************************************* 
  Function: R_SYS_DBG_EmuPeripheralCtl
 
  This function sets the peripheral break for debugger,
  the macro will also be stopped by the break points. 
*/ 

void R_SYS_DBG_EmuPeripheralCtl (void);

#endif  /* _DEBUG_SYS_H_ */
