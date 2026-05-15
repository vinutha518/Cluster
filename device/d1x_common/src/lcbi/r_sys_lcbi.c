/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_lcbi.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
LCBI system functions for r7f701412
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
  Title: r7f701412 LCBI device functions

  Implementation of the LCBI support functions for the 
  device r7f701412.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_ddb_api.h"
#include "r_lcbi_api.h"
#include "r_lcbi_sys.h"
#include "r_config_lcbi.h"

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
  Variable: loc_LcbiIntAddress

  Stores the Addresses of the Interrupt control registers for each unit
  The order of the adresses for each interrupt type schould be the same
  as defined in lcbi_api. See <r_lcbi_Interrupt_t>

*/

static r_dev_IntSel_t loc_LcbiIntAddress[R_LCBI_MACRO_NUM][R_LCBI_INT_NUM] = 
{
/*                R_LCBI_INT_RDY        R_LCBI_INT_EMPT         R_LCBI_INT_QTR      */
/*                    R_LCBI_INT_HALF       R_LCBI_INT_3QTR         R_LCBI_INT_FULL   */
/* unit 0 */    { R_DEV_INT_LCDBIFRDY,  R_DEV_INT_LCDBIFEMPTY,  R_DEV_INT_LCDBIFQTR,
                      R_DEV_INT_LCDBIFHALF, R_DEV_INT_LCDBIF3QTR,   R_DEV_INT_LCDBIFFULL},
};


/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_LCBI_SYS_BaseAddr
  
  See: r_lcbi_sys.h for details
*/

uint32_t R_LCBI_SYS_BaseAddr(uint32_t Unit) 
{
    uint32_t addr;

    switch(Unit) 
    {
        case 0:
            addr = R_LCBI_BASE;
            break;
        default:
            addr = (uint32_t)-1;
            break;
    }
    return addr;
}

/*******************************************************************************
  Function: R_LCBI_SYS_HardwareInit
  
  See: r_lcbi_sys.h for details
*/

r_lcbi_Error_t R_LCBI_SYS_HardwareInit(uint32_t Unit) 
{
    return R_LCBI_ERR_OK; 
}
/*******************************************************************************
  Function: R_LCBI_SYS_HardwareDeInit
  
  See: r_lcbi_sys.h for details
*/

r_lcbi_Error_t R_LCBI_SYS_HardwareDeInit(uint32_t Unit) 
{
    /* ensure all interrupts are disabled */
    R_LCBI_SYS_DisableInt(Unit, R_LCBI_INT_RDY);
    R_LCBI_SYS_DisableInt(Unit, R_LCBI_INT_EMPT);
    R_LCBI_SYS_DisableInt(Unit, R_LCBI_INT_QTR);
    R_LCBI_SYS_DisableInt(Unit, R_LCBI_INT_HALF);
    R_LCBI_SYS_DisableInt(Unit, R_LCBI_INT_3QTR);
    R_LCBI_SYS_DisableInt(Unit, R_LCBI_INT_FULL);
    
    return R_LCBI_ERR_OK; 
}

/*******************************************************************************
  Function: R_LCBI_SYS_EnableInt
*/

void R_LCBI_SYS_EnableInt(uint32_t Unit, r_lcbi_Interrupt_t Interrupt) 
{
    if (Unit >= R_LCBI_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "LCBI: We do not have this Unit"); 
    }

    /* Enable the interrupts */
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][Interrupt], 1u);
}


/*******************************************************************************
  Function: R_LCBI_SYS_DisableInt
*/

void R_LCBI_SYS_DisableInt(uint32_t Unit, r_lcbi_Interrupt_t Interrupt) 
{
    if (Unit >= R_LCBI_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "LCBI: We do not have this Unit"); 
    }

    /* Enable the interrupts */
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][Interrupt], 0u);
}


/*******************************************************************************
  Function: R_LCBI_SYS_IntCInit

*/
void R_LCBI_SYS_IntCInit(uint32_t Unit) 
{
    if (Unit >= R_LCBI_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "LCBI: We do not have this Unit"); 
    }
    
    /* Set Prio */
    R_DEV_IntSetPrio(loc_LcbiIntAddress[Unit][R_LCBI_INT_RDY],  3u);
    R_DEV_IntSetPrio(loc_LcbiIntAddress[Unit][R_LCBI_INT_EMPT], 4u);
    R_DEV_IntSetPrio(loc_LcbiIntAddress[Unit][R_LCBI_INT_QTR],  5u);
    R_DEV_IntSetPrio(loc_LcbiIntAddress[Unit][R_LCBI_INT_HALF], 6u);
    R_DEV_IntSetPrio(loc_LcbiIntAddress[Unit][R_LCBI_INT_3QTR], 7u);
    R_DEV_IntSetPrio(loc_LcbiIntAddress[Unit][R_LCBI_INT_FULL], 8u);
    
    /* Enable the interrupts */
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][R_LCBI_INT_RDY],   1u);
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][R_LCBI_INT_EMPT],  1u);
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][R_LCBI_INT_QTR],   1u);
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][R_LCBI_INT_HALF],  1u);
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][R_LCBI_INT_3QTR],  1u);
    R_DEV_IntEnable(loc_LcbiIntAddress[Unit][R_LCBI_INT_FULL],  1u);
}


/*******************************************************************************
  Function: R_LCBI_SYS_GetClock
*/
uint32_t R_LCBI_SYS_GetClock(uint32_t Unit)
{
    uint32_t val;                                 
                                                                                       
    if(Unit == 0)
    {
        val = R_DEV_ClkFrequencyHz(R_DEV_CKS_LCBI);
    }
    else
    {
        val = 0;
    }
    return val;                                   
}


