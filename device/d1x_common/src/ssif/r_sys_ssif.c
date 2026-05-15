/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_ssif.c 4403 2014-11-25 13:53:27Z tobyas.hennig $
============================================================================ 
DESCRIPTION
SSIF system functions for r7f701412
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
  Title: dev_ssif SSIF device functions

  Implementation of the SSIF support functions for the 
  device dev_ssif.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_ssif_api.h"
#include "r_ssif_sys.h"
#include "r_config_ssif.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif



/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Variable: loc_SsifIntAddress

  Stores the Addresses of the Interrupt control registers for each unit
  The order of the adresses for each interrupt type schould be the same
  as defined in lcbi_api. See <r_Ssif_Interrupt_t>

*/

static r_dev_IntSel_t loc_SsifIntAddress[R_SSIF_MACRO_NUM][R_SSIF_INT_NUM] = 
{
/*                R_SSIF_INT_TXI        R_SSIF_INT_RXI          R_SSIF_INT_IRQ */
/* unit 0 */    { R_DEV_INT_SSIF0TX,    R_DEV_INT_SSIF0RX,      R_DEV_INT_SSIF0},
/* unit 1 */    { R_DEV_INT_SSIF1TX,    R_DEV_INT_SSIF1RX,      R_DEV_INT_SSIF1}
};

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_SSIF_SYS_BaseAddr
  
  See: r_ssif_sys.h for details
*/

uint32_t R_SSIF_SYS_BaseAddr(uint32_t Unit) 
{
    uint32_t addr;
    
    switch(Unit) 
    {
        case 0:
            addr = R_SSIF0_BASE;
            break;
        case 1:
            addr = R_SSIF1_BASE;
            break;
        default:
            addr = (uint32_t)-1;
            break;
    }
    return addr;
}

/*******************************************************************************
  Function: R_SSIF_SYS_HardwareInit
  
  See: r_ssif_sys.h for details
*/

r_ssif_Error_t R_SSIF_SYS_HardwareInit(uint32_t Unit) 
{
    if (Unit >= R_SSIF_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "SSIF: We do not have this Unit"); 
    }
    
    /* Set Prio */
    R_DEV_IntSetPrio(loc_SsifIntAddress[Unit][R_SSIF_INT_IRQ],  3u);
    R_DEV_IntSetPrio(loc_SsifIntAddress[Unit][R_SSIF_INT_RXI],  4u);
    R_DEV_IntSetPrio(loc_SsifIntAddress[Unit][R_SSIF_INT_TXI],  5u);

    return R_SSIF_ERR_OK; 
}

/*******************************************************************************
  Function: R_SSIF_SYS_HardwareDeInit
  
  See: r_ssif_sys.h for details
*/

r_ssif_Error_t R_SSIF_SYS_HardwareDeInit(uint32_t Unit) 
{
    return R_SSIF_ERR_OK; 
}

/*******************************************************************************
  Function: R_SSIF_SYS_EnableInt
*/

void R_SSIF_SYS_EnableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt) 
{
    if (Unit >= R_SSIF_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "SSIF: We do not have this Unit"); 
    }

    /* Enable the interrupts */
    R_DEV_IntEnable(loc_SsifIntAddress[Unit][Interrupt], 1u);
}


/*******************************************************************************
  Function: R_SSIF_SYS_DisableInt
*/

void R_SSIF_SYS_DisableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt) 
{
    if (Unit >= R_SSIF_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "SSIF: We do not have this Unit"); 
    }

    /* Disable the interrupts */
    R_DEV_IntEnable(loc_SsifIntAddress[Unit][Interrupt], 0u);
}


/*******************************************************************************
  Function: R_SSIF_SYS_GetClock
*/

uint32_t R_SSIF_SYS_GetClock(uint32_t Unit)
{
    uint32_t val;
    
    if(Unit < R_SSIF_MACRO_NUM)
    {
        val = R_DEV_ClkFrequencyHz(R_DEV_CKS_SSIF);
    }
    else
    {
        val = 0;
    }
    return val;
}


/*******************************************************************************
  Function: R_SSIF_SYS_SetClock
*/

uint32_t R_SSIF_SYS_SetClock(uint32_t Unit, uint32_t Frequency)
{
    uint32_t val;
    r_dev_ClkSelConfig_t ssifClkSelCfg;
    
    if(Unit < R_SSIF_MACRO_NUM)
    {
        val = R_DEV_ClkFrequencyHz(R_DEV_CKS_PLLFIX);
        if ((val == 0) || (val < Frequency))
        {
            return 0;
        }
        
        ssifClkSelCfg.Div = (val / Frequency);
        if ((ssifClkSelCfg.Div < 10) || (ssifClkSelCfg.Div > 255))
        {
            ssifClkSelCfg.Div = 10;
        }
        
        ssifClkSelCfg.Cks = R_DEV_CKS_SSIF;
        ssifClkSelCfg.SrcId = R_DEV_CKS_SRC_PLLFIX;
        ssifClkSelCfg.StpReqMsk = 0;
        R_DEV_ClkIdSet(&ssifClkSelCfg);
        val = R_SSIF_SYS_GetClock(Unit);
    }
    else
    {
        val = 0;
    }
    return val;
}


/*******************************************************************************
  Function: R_SSIF_SYS_ErrorHandler
  See: r_ssif_sys.h for details
*/

void R_SSIF_SYS_ErrorHandler(uint32_t Unit, r_ssif_Error_t Error) 
{
    R_DEV_ERROR(Unit, (uint32_t)Error, "SSIF: Error occured");
}

