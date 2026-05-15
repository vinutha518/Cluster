/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_wdta.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
WDTA system functions for d1x_scit
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
  Title: dev_wdta WDTA device functions

  Implementation of the WDTA support functions for the 
  device dev_wdta.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_wdta_api.h"
#include "r_wdta_sys.h"
#include "r_config_wdta.h"



/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_SYS_WDTA_GetBaseAddr
  
  See: r_wdta_sys.h for details
*/

uint32_t R_SYS_WDTA_GetBaseAddr(uint32_t Unit) 
{
    uint32_t retval = 0; 
    switch (Unit)
    {
    case 0:
        retval = R_WDTA0_BASE;
        break;

    case 1:
        retval = R_WDTA1_BASE;
        break;
    default:
        /*return R_WDTA_ERR_RANGE;*/ 
        retval = 0; 
        break;
    }
    return retval;     
}

/*******************************************************************************
  Function: R_SYS_WDTA_HardwareInit
  
  See: r_wdta_sys.h for details
*/

r_wdta_Error_t R_SYS_WDTA_HardwareInit(uint32_t Unit) 
{
    if (Unit >= R_WDTA_MACRO_NUM) 
    {
        return R_WDTA_ERR_RANGE; 
    }
    else 
    {
        return R_WDTA_ERR_OK;
    } 
}

/*******************************************************************************
  Function: R_SYS_WDTA_HardwareDeInit
  
  See: r_wdta_sys.h for details
*/

r_wdta_Error_t R_SYS_WDTA_HardwareDeInit(uint32_t Unit) 
{
    if (Unit >= R_WDTA_MACRO_NUM) 
    {
        return R_WDTA_ERR_RANGE; 
    }
    else 
    {
        return R_WDTA_ERR_OK;
    } 
}

/*******************************************************************************
  Function: R_SYS_WDTA_EnableInt
*/

void R_SYS_WDTA_EnableInt(uint32_t Unit) 
{
    switch (Unit)
    {
    case 0:
        R_DEV_IntEnable(R_DEV_INT_WDTA0 ,1); 
        break;

    case 1:
        R_DEV_IntEnable(R_DEV_INT_WDTA1 ,1); 
        break;

    default:
      R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "WDTA: We do not have this Unit");      
      break; 
    }    
}


/*******************************************************************************
  Function: R_SYS_WDTA_DisableInt
*/

void R_SYS_WDTA_DisableInt(uint32_t Unit) 
{
    switch (Unit)
    {
    case 0:
        R_DEV_IntEnable(R_DEV_INT_WDTA0, 0); 
        break;

    case 1:
        R_DEV_IntEnable(R_DEV_INT_WDTA1, 0); 
        break;

    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "WDTA: We do not have this Unit");      
        break; 
    }    
}



