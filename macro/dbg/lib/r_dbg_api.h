/*
****************************************************************************
PROJECT : debug support  
FILE    : $Id: r_dbg_api.h 3935 2014-09-23 08:52:32Z golczewskim $
AUTHOR  : Michael Golczewski
============================================================================ 
DESCRIPTION
Debug support API 
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

/*******************************************************************************
  Title: Debug Support API

  Definition of the debug support API
*/



#ifndef R_DBG_API_H_
#define R_DBG_API_H_

typedef struct {
    char * RegName;
    uint32_t Addr; 
} r_dbg_DebugRegs_t; 



typedef enum  {
   bit00 = 0u,
   bit01,
   bit02,
   bit03,
   bit04,
   bit05,
   bit06,
   bit07,
   bit08,
   bit09,
   bit10,
   bit11,
   bit12,
   bit13,
   bit14,
   bit15
} R_DBG_DebugBits_t;


typedef enum
{  
    R_DBG_SVSTOP_IGNORED   = 0u,
    R_DBG_SVSTOP_EFECTIVE
} r_dbg_EPC_t;


/* 
R_DBG_MSG_ERR    - Important - error message 
R_DBG_MSG_WARN   - Warning level 
R_DBG_MSG_INFO   - Info message 
R_DBG_MSG_CODE   - sample code message 
R_DBG_MSG_DET    - additional details 

*/
#define R_DBG_MSG_ERR    0
#define R_DBG_MSG_WARN   1
#define R_DBG_MSG_INFO   2
#define R_DBG_MSG_CODE   3
#define R_DBG_MSG_DET    4

/*******************************************************************************
  Section: Global Functions  

*/
#ifdef R_DBG_PRINT_MSG
    /* The R_DBG_MSG_LEVEL defines the detail level 
       R_DBG_PRINT(lvl,.. will be ignored if lvl > R_DBG_MSG_LEVEL*/
    #define R_DBG_PRINT(lvl, txt) {int32_t valm = R_DBG_MSG_LEVEL;  \
                                   if ( valm >= (lvl)) \
                                   { printf("/*** %s ***/\n", (txt));};} 
#else
    #define R_DBG_PRINT(lvl, txt)  
#endif 
/*******************************************************************************
  Function: R_DBG_PrintRegCmd

  This function searches for a register name for the given Address
  and prints it (using printf()) together with the assigned value  

  The output looks like: 
  SG0CTL=0x1
  SG0CONF=0x0
  
  To use it for your driver 
  add: #include "debug_api.h" 
       to your config_xx.h nacro header  
 
  and modify register access macros like this: 
  #ifdef DBG_PRINT_ALL_REGS
    #define EE_SG_WRITE16(ADDR, VALUE) R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE)); *((volatile uint16_t*)(ADDR)) = (VALUE)
  #else
    #define EE_SG_WRITE16(ADDR, VALUE)  *((volatile uint16_t*)(ADDR)) = (VALUE)
  #endif
 
  Do not forget to add the debug.mk to your config_xxx makefile 
    
  Parameters:
  Addr      - Register address
  Val       - Value assigned     

  Returns:
  void
*/

void R_DBG_PrintRegCmd(uint32_t Size, uint32_t Addr, uint32_t Val); 
void R_DBG_PrintBitSetCmd(uint32_t Addr, R_DBG_DebugBits_t BitNum); 
void R_DBG_PrintBitClearCmd(uint32_t Addr, R_DBG_DebugBits_t BitNum);
uint32_t R_DBG_PrintRegRead32(uint32_t Addr); 
uint16_t R_DBG_PrintRegRead16(uint32_t Addr); 
uint8_t  R_DBG_PrintRegRead8(uint32_t Addr); 
 
/*******************************************************************************
  Function: R_DBG_InitSVSTOP
  
  This function sets the peripheral break for debugger,
  the macro will also be stopped by the break points.
*/

void R_DBG_InitSVSTOP(r_dbg_EPC_t EpcEn);


#endif  /* _R_DBG_API_H_ */
