/*
****************************************************************************
PROJECT : debug support  
FILE    : $Id: r_dbg_main.c 3935 2014-09-23 08:52:32Z golczewskim $
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
  Title:  Debug Support API implementation

  Implementation of the debug support functions 
*/

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"
#include "r_dbg_api.h"
#include "r_dbg_sys.h"

/* That's maybe not nice - but this is a debug module and it's much easier this way */
#include <stdio.h>

/*******************************************************************************
  Section: Local Typedefs 
*/


/*******************************************************************************
  Section: Local Defines
*/


/*******************************************************************************
  Section: local Constants 
*/

/*******************************************************************************
  Section: Global API Functions
  
  Comments see R_TB_Dbgapi.h
*/

/*******************************************************************************
  Function: R_DBG_PrintRegCmd
*/

void R_DBG_PrintRegCmd(uint32_t Size, uint32_t Addr, uint32_t Val) 
{
    char * reg_name = 0u; 
    reg_name = R_SYS_DBG_FindName(Addr); 
    if (reg_name != 0u) 
    {
        switch (Size)
        {
        case 8:
            printf("%16s = 0x%02x;       /* *((volatile uint%d_t* )(0x%08x)) = 0x%0x */\n",reg_name,Val, Size, Addr, Val);
            break; 
        case 16: 
            printf("%16s = 0x%04x;     /* *((volatile uint%d_t*)(0x%08x)) = 0x%04x */\n",reg_name,Val, Size, Addr, Val);
            break; 
        case 32: 
            printf("%16s = 0x%08x; /* *((volatile uint%d_t*)(0x%08x)) = 0x%08x */\n",reg_name,Val, Size, Addr, Val);
            break; 
        default:     
            printf("%16s = 0x%08x; /* *((volatile uint%d_t*)(0x%08x)) = 0x%08x */\n",reg_name,Val, Size, Addr, Val);
            break;
        }     
    }
    else
    {
        printf("%16s = 0x%08x; /* *((volatile uint%d_t*)(0x%08x)) = 0x%08x */ /* Unknown Address */ \n","UNKNOWN", Val, Size, Addr,Val ) ;    
    }
}
 
 /*******************************************************************************
  Function: R_DBG_PrintRegRead32
*/

uint32_t R_DBG_PrintRegRead32(uint32_t Addr)
{
    return R_SYS_DBG_PrintRegRead32(Addr); 
}

 /*******************************************************************************
  Function: R_DBG_PrintRegRead32
*/

uint16_t R_DBG_PrintRegRead16(uint32_t Addr)
{
    return R_SYS_DBG_PrintRegRead16(Addr); 
}

 /*******************************************************************************
  Function: R_DBG_PrintRegRead32
*/

uint8_t R_DBG_PrintRegRead8(uint32_t Addr) 
{
    return R_SYS_DBG_PrintRegRead8(Addr); 
}

 /*******************************************************************************
  Function: R_DBG_PrintBitSetCmd
*/

void R_DBG_PrintBitSetCmd(uint32_t Addr, R_DBG_DebugBits_t BitNum)
{
    char * reg_name = 0u; 
    reg_name = R_SYS_DBG_FindName(Addr); 
    if (reg_name != 0u) 
    {
        printf("%s bit %d set\n",reg_name,BitNum);
    }
    else
    {
        printf("RegAddr: 0x%x bit %d set\n",Addr,BitNum ) ;    
    }
}

/*******************************************************************************
  Function: R_DBG_PrintBitSetCmd
*/

void R_DBG_PrintBitClearCmd(uint32_t Addr, R_DBG_DebugBits_t BitNum)
 {
    char * reg_name = 0u; 
    reg_name = R_SYS_DBG_FindName(Addr); 
    if (reg_name != 0u)
    {
        printf("%s bit %d cleared\n",reg_name,BitNum);
    }
    else
    {
        printf("RegAddr: 0x%x bit %d cleared\n",Addr,BitNum ) ;    
    }
}


 /*******************************************************************************
  Function: R_DBG_InitSVSTOP
*/

void R_DBG_InitSVSTOP(r_dbg_EPC_t EpcEn) 
{
    if (0 != EpcEn)
    {
        R_SYS_DBG_EmuPeripheralCtl();
    }
}
