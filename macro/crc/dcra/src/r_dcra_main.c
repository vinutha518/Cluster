/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_dcra_main.c 7171 2016-01-06 17:52:36Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the DCRA macro
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
  Title: DCRA Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_dcra.h"
#include "r_dcra_api.h"
#define DCRA_MAIN_GLOBALS_
#include "r_dcra_main.h"
#include "r_dcra_sys.h"
#include "r_dcra_regs.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_DcraVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_DcraDrvVer[] = R_VERSION(DCRA, R_DCRA_VERSION_HI, R_DCRA_VERSION_LO);



/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_DCRA_CheckMacroParams
  
  See: <dcra_main.h>
*/
r_dcra_Error_t R_DCRA_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_DCRA_MACRO_NUM <= Unit)
    {
        return  R_DCRA_ERR_RANGE; 
    }
    return R_DCRA_ERR_OK;  
}

/*******************************************************************************
  Function: R_DCRA_ErrorHandler
  
  See: <dcra_main.h>
*/

void R_DCRA_ErrorHandler(uint32_t Unit, r_dcra_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for(;;) 
        {        /* we don't have a handler */
        }        /* so, let's wait here */
    }
}

/*******************************************************************************
  Section: Global API Functions
  
  See: <dcra_api.h>
*/

/*******************************************************************************
  Function: R_DCRA_GetVersionStr

  See: <dcra_api.h> 
*/
const int8_t * R_DCRA_GetVersionStr(void) 
{
    return (const int8_t *) loc_DcraDrvVer;
}

/*******************************************************************************
  Function: R_DCRA_GetMajorVersion

  See: <dcra_api.h> 
*/
const uint16_t R_DCRA_GetMajorVersion(void) 
{
    return (uint16_t)R_DCRA_VERSION_HI; 
}

/*******************************************************************************
  Function: R_DCRA_GetMinorVersion

  See: <dcra_api.h> 
*/
const uint16_t R_DCRA_GetMinorVersion(void) 
{
    return (uint16_t)R_DCRA_VERSION_LO; 
}



/*******************************************************************************
  Function: R_DCRA_Init

  See: <dcra_api.h> 
*/

r_dcra_Error_t R_DCRA_Init(uint32_t Unit, r_dcra_Parameter_t *Config)
{
    r_dcra_Error_t err;
    uint32_t value;
    err = R_DCRA_CheckMacroParams(Unit); 
    if (R_DCRA_ERR_OK != err) 
    {
        return err; 
    }
    if (0 == r_dcra_Dev[Unit].BaseAddr)
    {
        
        r_dcra_Dev[Unit].BaseAddr = R_DCRA_SYS_BaseAddr(Unit);
        err = R_DCRA_SYS_HardwareInit(Unit);
    }
    else 
    {   
        /* please, no double-init*/
        err = R_DCRA_ERR_LOCKED;
        return err;
    }
    
    /* set DCRA control register */
    value = 0;
    value |= ((uint32_t)(Config->InputWidth)<<1u);
    value |= ((uint32_t)(Config->Polynom));
    value &= (0x7u);
    R_DCRA_WRITE_REG(8, (r_dcra_Dev[Unit].BaseAddr + R_DCRA_REG_CTL), value)
    /* set DCRA CRC start value */
    R_DCRA_SetStartValue(Unit, Config->CrcStartValue);
    
    return err;
}


/*******************************************************************************
  Function: R_DCRA_DeInit

  See: <dcra_api.h> 
*/

r_dcra_Error_t R_DCRA_DeInit(uint32_t Unit)
{
    r_dcra_Error_t err;

    err = R_DCRA_CheckMacroParams(Unit);

    if (R_DCRA_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_dcra_Dev[Unit].BaseAddr)
    {
        
        r_dcra_Dev[Unit].BaseAddr = 0;
        err = R_DCRA_SYS_HardwareDeInit(Unit);
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_DCRA_ERR_NOTLOCKED; 
    }
    
    return err;
}


/*******************************************************************************
  Function: R_DCRA_SetStartValue

  See: <dcra_api.h> 
*/

r_dcra_Error_t R_DCRA_SetStartValue(uint32_t Unit, uint32_t Value)
{
    r_dcra_Error_t err;
    err = R_DCRA_CheckMacroParams(Unit); 
    if (R_DCRA_ERR_OK != err) 
    {
        return err; 
    }
    
    /* write CRC start value to DCRAnCOUT register */
    R_DCRA_WRITE_REG(32, (r_dcra_Dev[Unit].BaseAddr + R_DCRA_REG_COUT), Value);
    
    return err;
}


/*******************************************************************************
  Function: R_DCRA_Calculate

  See: <dcra_api.h> 
*/

r_dcra_Error_t R_DCRA_Calculate(uint32_t Unit, uint32_t Value)
{
    r_dcra_Error_t err;
    err = R_DCRA_CheckMacroParams(Unit); 
    if (R_DCRA_ERR_OK != err) 
    {
        return err; 
    }
    
    /* write consecutive value (that CRC shall be calcuated of) to DCRAnCIN register */
    R_DCRA_WRITE_REG(32, (r_dcra_Dev[Unit].BaseAddr + R_DCRA_REG_CIN), Value);
    
    return err;
}


/*******************************************************************************
  Function: R_DCRA_GetResult

  See: <dcra_api.h> 
*/

uint32_t R_DCRA_GetResult(uint32_t Unit)
{
    uint32_t dcraCrcResult;
    uint32_t mode;
    
    /* read CRC result from DCRAnCOUT register */
    dcraCrcResult = R_DCRA_READ_REG(32, (r_dcra_Dev[Unit].BaseAddr + R_DCRA_REG_COUT));
    
    /* Perform XOR operation to DCRAnCOUT register value to obtain final result */
    /* The read value of this register is a value obtained
        by performing EXOR calculation for the following value:
        + For 32-bit Ethernet polynomial: FFFF FFFFH
        + For 16-bit CCITT polynomial: 0000H
        
        For example, when DCRAnCOUT = 5555 5555H
        for the 32-bit Ethernet polynomial, AAAA AAAAH is read. 
    */
    mode = R_DCRA_READ_REG(8, (r_dcra_Dev[Unit].BaseAddr + R_DCRA_REG_CTL));
    mode = ((mode & 0x01u) >> 0);    /* mask out DCRAnPOL bit */
    if (R_DCRA_POL_32ETH_POLY == mode)
    {

        dcraCrcResult = (dcraCrcResult ^ 0xFFFFFFFFu);
    }
    if (R_DCRA_POL_16CCIT_POLY == mode)
    {
        dcraCrcResult = (dcraCrcResult ^ 0x0000u);
    }
    
    return dcraCrcResult;
}


/*******************************************************************************
  Function: R_DCRA_SetErrorCallback
*/

void R_DCRA_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error))
{
    loc_ErrorHandler = ErrorCallback;
}


