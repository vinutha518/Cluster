/*
****************************************************************************
PROJECT : PWM driver
FILE    : $Id: r_sys_pwm.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================
DESCRIPTION
PWM driver system functions for r7f7010352
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
  Title: PWM7010 Support Functions

  Implementation of the PWM support functions for the 
  device r7f701412.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_pwm_api.h"
#include "r_pwm_sys.h"
#include "r_dev_api.h"
#include "r_config_pwm.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_SYS_PWBA_GetBaseAddr
*/

uint32_t R_SYS_PWBA_GetBaseAddr(uint32_t Unit) 
{
    uint32_t addr = 0;
    
    if (Unit >= R_PWM_PWBA_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "PWM: invalid PWGA unit"); 
    }
    else
    {
        addr = R_PWM_PWBA0;
    }
    return addr;
}

/*******************************************************************************
  Function: R_SYS_PWGA_GetBaseAddr
*/

uint32_t R_SYS_PWGA_GetBaseAddr(uint32_t Unit) 
{
    uint32_t addr = 0;
    
    if (Unit >= R_PWM_PWGA_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "PWM: invalid PWGA unit"); 
    }
    else
    {
        addr = R_PWM_PWGA0 + 0x100 * Unit;
    }
    return addr;
}

/*******************************************************************************
  Function: R_SYS_PWSA_GetBaseAddr
*/

uint32_t R_SYS_PWSA_GetBaseAddr(uint32_t Unit) 
{
    uint32_t addr = 0;
    
    if (Unit >= R_PWM_PWBA_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "PWM: invalid PWGA unit"); 
    }
    else
    {
        addr = R_PWM_PWSA0;
    }
    return addr;
}

/*******************************************************************************
  Function: R_SYS_PWM_MaxPhyChannelNum
*/

uint8_t R_SYS_PWM_MaxPhyChannelNum(uint32_t AdcUnit)
{
    uint8_t       Max_ChNum;
    switch (AdcUnit)
    {
        case 0 :  Max_ChNum = R_PWM_ADC0PhyCh_MAX_NUM;
                  break;
        case 1 :  Max_ChNum = R_PWM_ADC1PhyCh_MAX_NUM;
                  break;
        default:  R_DEV_ERROR(AdcUnit, R_DEV_ERR_UNIT, "PWM: Invalid ADC Unit occurs");
    }
    return        Max_ChNum;
}


/*******************************************************************************
  Function: R_SYS_PWM_EmuBrkInit
  
  see: <r_pwm_sys.h> for details
*/
void R_SYS_PWM_EmuBrkInit (r_pwm_Emu_t PWM_Emu)
{
    if (PWM_Emu == R_PWM_SVSDIS_EFECTIVE)
    {
#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)    	
        R_DBG_InitSVSTOP(R_DBG_SVSTOP_EFECTIVE);
#endif
    }
}
