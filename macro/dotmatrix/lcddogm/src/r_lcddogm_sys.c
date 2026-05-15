/*
****************************************************************************
PROJECT : lcd display driver for dogm displays (DX4 applboard)
FILE    : $Id: r_lcddogm_sys.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Low level Driver functions for LCD Dogm
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

#include "r_typedefs.h"
#include "r_lcddogm_api.h"
#include "r_lcddogm_sys.h"

#include "r_gpio_api.h"


/***********************************************************
  Section: Local Variables
*/

static r_lcdDogm_PortConfig_t loc_LCDPort;


/***********************************************************
  Section: Local Functions
*/

void loc_WriteCS(uint8_t value) {
    R_GPIO_WritePin(loc_LCDPort.cs_port,loc_LCDPort.cs_pin, value);
}

void loc_WriteRW(uint8_t value) {
    R_GPIO_WritePin(loc_LCDPort.rw_port,loc_LCDPort.rw_pin, value);
}

void loc_WriteRS(uint8_t value) {
    R_GPIO_WritePin(loc_LCDPort.rs_port,loc_LCDPort.rs_pin, value);
}

void loc_WriteEN(uint8_t value) {
    R_GPIO_WritePin(loc_LCDPort.en_port,loc_LCDPort.en_pin, value);
}

void loc_WriteData(uint8_t bit, uint8_t value) {
    R_GPIO_WritePin(loc_LCDPort.data_port[bit],loc_LCDPort.data_pin[bit], value);
}

                               
                               
/***********************************************************
  Section: Global Functions
*/

            
r_lcdDogm_Error_t R_LCDDOGM_Sys_PortInit(r_lcdDogm_PortConfig_t pconf) {
  
  uint32_t   i;

  loc_LCDPort.rw_port = pconf.rw_port;
  loc_LCDPort.rw_pin  = pconf.rw_pin;
  
  loc_LCDPort.cs_port = pconf.cs_port;
  loc_LCDPort.cs_pin = pconf.cs_pin;
  
  loc_LCDPort.en_port = pconf.en_port;
  loc_LCDPort.en_pin = pconf.en_pin;
  
  loc_LCDPort.rs_port = pconf.rs_port;
  loc_LCDPort.rs_pin = pconf.rs_pin;
      
  for(i = 0; i < R_LCDDOGM_DWIDTH; i++) {
      loc_LCDPort.data_port[i] = pconf.data_port[i];
      loc_LCDPort.data_pin[i]  = pconf.data_pin[i];
  }
    
  return R_LCDDOGM_OK;
}


r_lcdDogm_Error_t R_LCDDOGM_Sys_SetDirection(r_lcdDogm_Direction_t dir) {

  uint8_t i;

  for(i = 0; i < R_LCDDOGM_DWIDTH; i++) {
    R_GPIO_PinDirection(loc_LCDPort.data_port[i], loc_LCDPort.data_pin[i], (dir==R_LCDDOGM_DIR_INPUT)?1:0);
  }
  
  return R_LCDDOGM_OK;
}

r_lcdDogm_Error_t R_LCDDOGM_Sys_PutByte(uint8_t value) {
  
  uint8_t i;
  uint8_t val;
  
  for(i = 0; i < R_LCDDOGM_DWIDTH; i++) {

      val = (value >> i) & 0x1; 
      loc_WriteData(i, val);
  }
   
  /* eePortC_MaskedWritePort(loc_LCDPort.data_port,0xFF00,(value<<8)); */
  
  return R_LCDDOGM_OK;
}

uint8_t R_LCDDOGM_Sys_GetByte(void) {

  uint8_t i;
  uint8_t  val;
  uint8_t  byte;
  
  byte = 0;

  for(i = 0; i < R_LCDDOGM_DWIDTH; i++) {
     val = (uint8_t)R_GPIO_PinRead(loc_LCDPort.data_port[i], loc_LCDPort.data_pin[i]);
     val &= 0x01;

     byte |= val << i;
  }
  
/*  
   regval = eePortC_ReadReg(loc_LCDPort.data_port,EE_PORTC_PPR,0xFF00);
   byte = (uint8_t)(regval>>8);
*/

  return byte ;
}

r_lcdDogm_Error_t R_LCDDOGM_Sys_SetControl(r_lcdDogm_Control_t ctrl) {

  switch(ctrl) {
    case R_LCDDOGM_READ_DATA:
      
      loc_WriteCS(0);
      loc_WriteRW(1);
      loc_WriteRS(1);
      loc_WriteEN(1);
      
      break;
    case R_LCDDOGM_READ_CTRL:
      
      loc_WriteCS(0);
      loc_WriteRW(1);
      loc_WriteRS(0);
      loc_WriteEN(1);
      
      break;
    case R_LCDDOGM_WRITE_DATA:
      
      loc_WriteCS(0);
      loc_WriteRW(0);
      loc_WriteRS(1);
      loc_WriteEN(1);
      
      break;
    case R_LCDDOGM_WRITE_CTRL:
      
      loc_WriteCS(0);
      loc_WriteRW(0);
      loc_WriteRS(0);
      loc_WriteEN(1);
      
      break;
    case R_LCDDOGM_FALL_EDGE:
      
      loc_WriteCS(0);
      loc_WriteEN(0);
      
      break;      
    case R_LCDDOGM_END:
      
      loc_WriteCS(1);
      
      break;      
    default :
      return R_LCDDOGM_ERR_PARA;
  }
  
  return R_LCDDOGM_OK;      
}
