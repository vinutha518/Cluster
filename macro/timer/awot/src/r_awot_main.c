/*
****************************************************************************
PROJECT : AWOT driver
AUTHOR  : Michael Golczewski
FILE    : $Id: r_awot_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for AWOT macro

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

****************************************************************************
*/


/*******************************************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_awot_api.h"
#include "r_awot_sys.h"
#include "r_config_awot.h"
#include "r_awot_regs.h"
#define AWOT_MAIN_GLOBALS_
#include "r_awot_main.h"


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void ( *loc_ErrorHandler )(uint32_t Unit, r_awot_Error_t Error) = 0u;


/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_AWOT_ErrorHandler

  For API documentation see file awot_main.h
*/

void R_AWOT_ErrorHandler(uint32_t Unit, r_awot_Error_t Error) 
{
    
    if (loc_ErrorHandler != 0u) 
    {   /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, Error);   /* ... and we gonna use it!*/
    }
    else 
    {                                /* we don't have a handler */
        for (;;)                     /* so let's wait here ... */
        {
        }
    }
}


/*******************************************************************************
  Section: Global API Functions

  For API documentation see file <awot_api.h>
*/

/*******************************************************************************
  Function: R_AWOT_Init

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_Init(uint32_t Unit, r_awot_Parameter_t *Config) 
{
    uint32_t        base;
    R_AWOT_RegCtl_t regCTLval = {0};
    uint32_t        regTOEval = 0;
    uint32_t        regCCRval = 0;
    uint32_t        regCMP0val = 0;
    uint32_t        regCMP1val = 0;
    
    if (Unit >= R_AWOT_MACRO_NUM) 
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else if(R_AWOT_MODE_NUM <= Config->Mode)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else if(R_AWOT_INPUT_EDGE_NUM <= Config->InputEdge)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else if(R_AWOT_OUTPUT_MODE_NUM <= Config->OutputMode)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else if( (R_AWOT_MODE_INTERVAL == Config->Mode) && (0 != Config->DutyCycle) )
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else if( (R_AWOT_MODE_CAPTURE == Config->Mode) && (0 != Config->DutyCycle) )
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else if( (R_AWOT_MODE_PWM == Config->Mode) && ( (0xFFFF < Config->DutyCycle) || (0xFFFF < Config->CmpVal) ) )
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {

        R_AWOT_Dev[Unit].BaseAddr = R_SYS_AWOT_GetBaseAddr(Unit);
        base = R_AWOT_Dev[Unit].BaseAddr;
        
        regCTLval.Bit.MD = Config->Mode;
        regCTLval.Bit.TIS = Config->InputEdge;
        
        regTOEval = Config->OutputMode;
        
        if ( R_AWOT_MODE_INTERVAL == Config->Mode)
        {
            regCCRval = Config->CmpVal;
        }
        else if ( R_AWOT_MODE_PWM == Config->Mode)
        {
            regCMP0val = Config->DutyCycle;
            regCMP1val = Config->CmpVal;
        }
        else 
        {
            
        } 
        
        
        R_AWOT_WRITE_REG(32, base + R_AWOT_CTL,  regCTLval.Byte);
        R_AWOT_WRITE_REG(32, base + R_AWOT_TOE,  regTOEval);
        R_AWOT_WRITE_REG(32, base + R_AWOT_STC,  0x80);
        /**/
        if ( R_AWOT_MODE_PWM != Config->Mode)
        {
            /* Writing to this register is prohibited in PWM mode */
            R_AWOT_WRITE_REG(32, base + R_AWOT_CCR,  regCCRval);
        }
        else
        {
            /* Writing to these registers is prohibited in other than PWM mode */
            R_AWOT_WRITE_REG(32, base + R_AWOT_CMP0, regCMP0val);
            R_AWOT_WRITE_REG(32, base + R_AWOT_CMP1, regCMP1val);
        }
        R_AWOT_WRITE_REG(32, base + R_AWOT_EMU,  0u);
    } 

    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_DeInit

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_DeInit(uint32_t Unit)
{
    uint32_t base;
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        R_AWOT_Dev[Unit].BaseAddr = R_SYS_AWOT_GetBaseAddr(Unit);
        base = R_AWOT_Dev[Unit].BaseAddr;

        R_AWOT_WRITE_REG(32, base + R_AWOT_CTL,  0u);
        R_AWOT_WRITE_REG(32, base + R_AWOT_TOE,  0u);
        R_AWOT_WRITE_REG(32, base + R_AWOT_STC,  0x80);
        R_AWOT_WRITE_REG(32, base + R_AWOT_CCR,  0u);
        R_AWOT_WRITE_REG(32, base + R_AWOT_CMP0, 0u);
        R_AWOT_WRITE_REG(32, base + R_AWOT_CMP1, 0u);
        R_AWOT_WRITE_REG(32, base + R_AWOT_EMU,  0u);
    }   
    R_SYS_AWOT_DisableInt(Unit, R_AWOT_INT_TINT);
    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_GetStatus

  For API documentation see file <awot_api.h>
*/

uint8_t R_AWOT_GetStatus(uint32_t Unit)
{
    uint32_t base;
    uint8_t  regval;
    
    regval = 0u;
    
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base   = R_AWOT_Dev[Unit].BaseAddr;
        regval = R_AWOT_READ_REG(8, base + R_AWOT_FLG);
    }
    return regval;
}



/*******************************************************************************
  Function: R_AWOT_ClearStatus

  For API documentation see file <awot_api.h>
*/

uint8_t R_AWOT_ClearStatus(uint32_t Unit)
{
    uint32_t base;
    uint8_t  regval;
    
    regval = 0u;
    
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base   = R_AWOT_Dev[Unit].BaseAddr;
        R_AWOT_WRITE_REG(32, base + R_AWOT_STC,  0x80);
    }
    return regval;
}



/*******************************************************************************
  Function: R_AWOT_GetCaptureRegValue

  For API documentation see file <awot_api.h>
*/

uint32_t R_AWOT_GetCaptureRegValue(uint32_t Unit)
{
    uint32_t base;
    uint32_t  regval;
    
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base   = R_AWOT_Dev[Unit].BaseAddr;
        regval = R_AWOT_READ_REG(32, base + R_AWOT_CCR);
    }

    return regval;
}



/*******************************************************************************
  Function: R_AWOT_GetCompareRegValue

  For API documentation see file <awot_api.h>
*/

uint32_t R_AWOT_GetCompareRegValue(uint32_t Unit)
{
    uint32_t base;
    uint32_t  regval;
    
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base   = R_AWOT_Dev[Unit].BaseAddr;
        regval = R_AWOT_READ_REG(32, base + R_AWOT_CMP0);
    }

    return regval;
}



/*******************************************************************************
  Function: R_AWOT_GetDutyCycleValue

  For API documentation see file <awot_api.h>
*/

uint32_t R_AWOT_GetDutyCycleValue(uint32_t Unit)
{
    uint32_t base;
    uint32_t  regval;
    
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base   = R_AWOT_Dev[Unit].BaseAddr;
        regval = R_AWOT_READ_REG(32, base + R_AWOT_CMP1);
    }

    return regval;
}



/*******************************************************************************
  Function: R_AWOT_Start

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_Start(uint32_t Unit)
{
    uint32_t base;
    uint32_t value;

    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base = R_AWOT_Dev[Unit].BaseAddr;
        value = R_AWOT_READ_REG(8, base + R_AWOT_CTL);
        value |= 0x80u;
        R_AWOT_WRITE_REG(8, base + R_AWOT_CTL, value);
    }
    return R_AWOT_ERR_OK;

}


/*******************************************************************************
  Function: R_AWOT_Stop

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_Stop(uint32_t Unit)
{
    uint32_t base;
    uint32_t value;
    
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base = R_AWOT_Dev[Unit].BaseAddr;
        value = R_AWOT_READ_REG(8, base + R_AWOT_CTL);
        value &= ~0x80ul;
        R_AWOT_WRITE_REG(8, base + R_AWOT_CTL, value);
    }
    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_SetOutputMode

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_SetOutputMode(uint32_t Unit, r_awot_OutputMode_t Mode)
{
    uint32_t base;
        
    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base = R_AWOT_Dev[Unit].BaseAddr;
        if ( R_AWOT_OUTPUT_MODE_LOW == Mode )
        {
            R_AWOT_WRITE_REG(8, base + R_AWOT_TOE, 0x00);
        }
        else
        {
            R_AWOT_WRITE_REG(8, base + R_AWOT_TOE, 0x01);
        }
    }
    return R_AWOT_ERR_OK;
}



/*******************************************************************************
  Function: R_AWOT_SetCaptureRegValue

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_SetCaptureRegValue(uint32_t Unit, uint32_t Val)
{
    uint32_t base;
    R_AWOT_RegCtl_t regCTLval = {0};

    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base = R_AWOT_Dev[Unit].BaseAddr;
        
        regCTLval.Byte = R_AWOT_READ_REG(8, base + R_AWOT_CTL);
        if (1 == regCTLval.Bit.TE)
        {
            /* This register may only be written when the AWOT is disabled  (AWOTnCTL.AWOTnTE = 0). */
            R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RUNNING);
        }
        else
        {
            R_AWOT_WRITE_REG(32, base + R_AWOT_CCR, Val);
        }
    }
    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_SetCompareRegValue

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_SetCompareRegValue(uint32_t Unit, uint32_t Val)
{
    uint32_t base;
    R_AWOT_RegCtl_t regCTLval = {0};

    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base = R_AWOT_Dev[Unit].BaseAddr;
        
        regCTLval.Byte = R_AWOT_READ_REG(8, base + R_AWOT_CTL);
        if (1 == regCTLval.Bit.TE)
        {
            /* This register may only be written when the AWOT is disabled  (AWOTnCTL.AWOTnTE = 0). */
            R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RUNNING);
        }
        else
        {
            R_AWOT_WRITE_REG(32, base + R_AWOT_CMP0, Val);
        }
    }
    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_SetDutyCycleValue

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_SetDutyCycleValue(uint32_t Unit, uint32_t Val)
{
    uint32_t base;
    R_AWOT_RegCtl_t regCTLval = {0};

    if (Unit >= R_AWOT_MACRO_NUM)
    {
        R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RANGE);
    }
    else
    {
        base = R_AWOT_Dev[Unit].BaseAddr;
        
        regCTLval.Byte = R_AWOT_READ_REG(8, base + R_AWOT_CTL);
        if (1 == regCTLval.Bit.TE)
        {
            /* This register may only be written when the AWOT is disabled  (AWOTnCTL.AWOTnTE = 0). */
            R_AWOT_ErrorHandler(Unit, R_AWOT_ERR_RUNNING);
        }
        else
        {
            R_AWOT_WRITE_REG(32, base + R_AWOT_CMP1, Val);
        }
    }
    return R_AWOT_ERR_OK;
}


/*******************************************************************************
  Function: R_AWOT_EnableInt

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_EnableInt(uint32_t Unit, r_awot_Interrupt_t Interrupt)
{
    /* Check range */
    if ((Unit >= R_AWOT_MACRO_NUM) || (Interrupt >= R_AWOT_INT_NUM))
    {
        return R_AWOT_ERR_RANGE; 
    }
    R_SYS_AWOT_EnableInt(Unit, Interrupt); 
    return R_AWOT_ERR_OK; 
}


/*******************************************************************************
  Function: R_AWOT_DisableInt

  For API documentation see file <awot_api.h>
*/

r_awot_Error_t R_AWOT_DisableInt(uint32_t Unit, r_awot_Interrupt_t Interrupt)
{
    
    /* Check range */
    if ((Unit >= R_AWOT_MACRO_NUM) || (Interrupt >= R_AWOT_INT_NUM))
    {
        return R_AWOT_ERR_RANGE; 
    }

    R_SYS_AWOT_DisableInt(Unit, Interrupt); 
    return R_AWOT_ERR_OK; 
}


/*******************************************************************************
  Function: R_AWOT_SetErrorCallback

  For API documentation see file <awot_api.h>
*/

void R_AWOT_SetErrorCallback(void (*ErrorCallback)(uint32_t Unit, r_awot_Error_t Error))
{
    loc_ErrorHandler = ErrorCallback; 
}


/*******************************************************************************
  Function: R_AWOT_GetPClock

  For API documentation see file <awot_api.h>
*/

uint32_t R_AWOT_GetClock(uint32_t Unit)
{
    return R_SYS_AWOT_GetClock(Unit);
}


