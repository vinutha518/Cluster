/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_dcra.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
DCRA system functions for D1M2(H) device (r7f701412)
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
  Title: dev_dcra DCRA device functions

  Implementation of the DCRA support functions for the 
  device dev_dcra.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_dcra_api.h"
#include "r_dcra_sys.h"
#include "r_config_dcra.h"

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
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_DCRA_SYS_BaseAddr
  
  See: r_dcra_sys.h for details
*/

uint32_t R_DCRA_SYS_BaseAddr(uint32_t Unit)
{
    uint32_t addr;
    
    if (Unit >= R_DCRA_MACRO_NUM)
    {
        return 0; 
    }

    if (Unit == 0)
    {
        addr = R_DCRA0_BASE;
    }
    
    return addr;
}

/*******************************************************************************
  Function: R_DCRA_SYS_HardwareInit
  
  See: r_dcra_sys.h for details
*/

r_dcra_Error_t R_DCRA_SYS_HardwareInit(uint32_t Unit) 
{
    return R_DCRA_ERR_OK; 
}
/*******************************************************************************
  Function: R_DCRA_SYS_HardwareDeInit
  
  See: r_dcra_sys.h for details
*/

r_dcra_Error_t R_DCRA_SYS_HardwareDeInit(uint32_t Unit) 
{
    return R_DCRA_ERR_OK; 
}


