/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_wdta_main.c 5028 2015-04-15 12:20:02Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for the WDTA macro
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
Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

/*******************************************************************************
  Title: WDTA Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_wdta.h"
#include "r_wdta_api.h"
#define WDTA_MAIN_GLOBALS_
#include "r_wdta_main.h"
#include "r_wdta_sys.h"
#include "r_wdta_regs.h"

/*******************************************************************************
  Section: Local Variables
*/
/*******************************************************************************
  Section: Local Functions
*/
static uint8_t loc_Wdta_GetVacReferenceValue(uint32_t Unit); 
static uint8_t loc_Wdta_CalculateVacValue(uint32_t Unit, uint8_t VacRefValue); 
static void loc_Wdta_TriggerWriteNonVac(uint32_t Unit);
static void loc_Wdta_TriggerWriteVac(uint32_t Unit);

/* not "opened" in the api .h */
uint8_t R_WDTA_READ_REGEvacRegister(uint32_t Unit); 
uint8_t R_WDTA_READ_REGRefRegister(uint32_t Unit); 
uint32_t R_WDTA_GetStatus(uint32_t Unit); 

/***********************************************************
  Function: loc_Wdta_GetVacReferenceValue

  Retreives the VAC reference value.
   
  Parameters: 
   
      
  Returns: 
  ucReferenceValue - VAC reference value
  
*/

static uint8_t loc_Wdta_GetVacReferenceValue(uint32_t Unit)
{
    uint8_t ucReferenceValue;
    uint32_t uiBaseAddress;

    /* Get the BASE address */

    uiBaseAddress = R_SYS_WDTA_GetBaseAddr(Unit);
    
    ucReferenceValue = R_WDTA_READ_REG(8, uiBaseAddress + R_WDTA_REF);
    
    return ucReferenceValue;
}

/***********************************************************
  Function: eeWdta_CalculateVACValue

  Calculates the VAC value from the reference value.
   
  Parameters: 
  VACRefValue - value to be calculated  
      
  Returns: 
  ucCalculatedValue - recalculated VAC value
  
*/
static uint8_t loc_Wdta_CalculateVacValue(uint32_t Unit, uint8_t VacRefValue)
{   /* May be add VAC-Number  */
    uint8_t ucCalculatedValue = 0;
    
    ucCalculatedValue = (uint8_t) (R_WDTA_TRIGGER_VALUE - VacRefValue);
    
    return ucCalculatedValue;
}

    
/***********************************************************
  Function: eeWdta_TriggerWriteVAC

  Gets the VAC reference value, recalculates it and writes it to the resister.
   
  Parameters: 
    Unit    - macro instance
      
  Returns: none
  
*/

static void loc_Wdta_TriggerWriteVac(uint32_t Unit)
{    
    uint8_t vacRefValue;
    uint8_t calculateVacValue;
    uint32_t base;

    /* get the reference value, recalculate and update register */
    base = R_WDTA_Dev[Unit].BaseAddr;
    
    vacRefValue = loc_Wdta_GetVacReferenceValue(Unit);
    
    calculateVacValue = loc_Wdta_CalculateVacValue(Unit, vacRefValue);

    /* update register */             
    R_WDTA_WRITE_REG(8, base + R_WDTA_EVAC, calculateVacValue);            
}


/***********************************************************
  Function: loc_Wdta_TriggerWriteNonVAC

  Trigger for the watch dog in non VAC mode.
   
  Parameters: 
  
      
  Returns: none
  
*/

static void loc_Wdta_TriggerWriteNonVac(uint32_t Unit)
{
    uint32_t baseAddress = 0;

    /* Get the BASE address */
    baseAddress = R_WDTA_Dev[Unit].BaseAddr;
       
    /* trigger the WD with the constant value */   
    R_WDTA_WRITE_REG(8, baseAddress + R_WDTA_WDTE, 0xAC);      
}

/***********************************************************
  Section: Global API Functions
  
  Comments see wdta_api.h
*/


/***********************************************************
  Function: R_WDTA_Init

*/

r_wdta_Error_t R_WDTA_Init(uint32_t Unit, r_wdta_Parameter_t *Config)
{
    r_wdta_RegMd_t params;
    uint32_t base;
       
    if (Unit < R_WDTA_MACRO_NUM)
    {       
        R_WDTA_Dev[Unit].BaseAddr = R_SYS_WDTA_GetBaseAddr(Unit);
        
        base = R_WDTA_Dev[Unit].BaseAddr;
                
        /* write the mode register */
        params.Bit.Ovf = (uint8_t)Config->TimeInterval;
        params.Bit.Wie = (uint8_t)Config->Interrupt75;
        params.Bit.Erm = (uint8_t)Config->ErrorMode;    
        params.Bit.Ws  = (uint8_t)Config->WindowSize;
        
        R_WDTA_WRITE_REG(8, base + R_WDTA_MD, params.Byte);    
        /*  eeIntc_SetWdtaNmiCallback(eeWdta_NmiIsr);  */
        return R_WDTA_ERR_OK;   
    } 
    else 
    {
        return R_WDTA_ERR_RANGE;   
    }
}


/***********************************************************
  Function: R_WDTA_Trigger

  Trigger the watch dog, either with the VCA or not.
   
  Parameters:
  Unit        - Instance number
  bVACEnabled - option to select VAC (TRUE) or not (FALSE)
  
  Returns:
  see: <r_wdta_Error_t>
*/

r_wdta_Error_t R_WDTA_Trigger(uint32_t Unit, uint8_t VacEnabled)
{
    r_wdta_Error_t tErrorMode = R_WDTA_ERR_OK;
		
    /* check the instance is in range */
    
    R_WDTA_Dev[Unit].BaseAddr = R_SYS_WDTA_GetBaseAddr(Unit);
    
    if(Unit >= R_WDTA_MACRO_NUM)
    {
        tErrorMode = R_WDTA_ERR_INSTANCE;
    }
    else
    {
        if(VacEnabled == 1)
        {
            loc_Wdta_TriggerWriteVac(Unit);
        }
        else
        {
            loc_Wdta_TriggerWriteNonVac(Unit);
        }
    }
    return tErrorMode;
}


/***********************************************************
  Function: R_WDTA_CalcRefValue
*/

uint8_t R_WDTA_CalcRefValue(uint32_t Unit, uint8_t VacValue)
{
    uint8_t expectTmp;
    uint8_t wdtaRef;
    
    expectTmp  = VacValue;
    wdtaRef = (uint8_t)(VacValue << 1u);
    if (0 != (expectTmp & 0x80u))
    {
        wdtaRef |= 0x01u;    
    }
    R_WDTA_Dev[Unit].VacRef = wdtaRef;
    return wdtaRef;
}    

/***********************************************************
  Function: R_WDTA_WrongTriggerNonVac

  Trigger for the watch dog in non VAC mode with wrong trigger value.
   
  Parameters: 
  
      
  Returns: none
  
*/
void R_WDTA_WrongTriggerNonVac(uint32_t Unit)
{
    uint32_t baseAddress = 0;

    /* Get the BASE address */
   
    baseAddress = R_WDTA_Dev[Unit].BaseAddr;
       
    /* trigger the WD with the a wrong trigger value i.e, other that 0xAC */   
    R_WDTA_WRITE_REG(8, baseAddress + R_WDTA_WDTE, 0x12);      
}

/***********************************************************
  Function: R_WDTA_WrongTriggerVac

  Trigger for the watch dog in VAC mode with wrong trigger value.
   
  Parameters: 
  Unit - unit
  value - value to be written
  Returns: none
  
*/
void R_WDTA_WrongTriggerVac(uint32_t Unit, uint32_t Value)
{
    uint32_t baseAddress = 0;

    /* Get the BASE address */
   
    baseAddress = R_WDTA_Dev[Unit].BaseAddr;
       
    /* trigger the WD with the a wrong trigger value */   
    R_WDTA_WRITE_REG(8, baseAddress + R_WDTA_EVAC, Value);      
}

/***********************************************************
  Function: R_WDTA_READ_REGEvacRegister

  Read EVAC register.
   
  Parameters: 
  Unit - unit  
      
  Returns: EVAC register content
  
*/
uint8_t R_WDTA_READ_REGEvacRegister(uint32_t Unit)
{
    uint32_t baseAddress = 0;
    uint8_t data;
    /* Get the BASE address */
   
    baseAddress = R_WDTA_Dev[Unit].BaseAddr;
       
    /* Read EVAC register */   
    data = R_WDTA_READ_REG(8, baseAddress + R_WDTA_EVAC);
    
    return (data);
}


/***********************************************************
  Function: R_WDTA_READ_REGRefRegister

  Read REF register.
   
  Parameters: 
  Unit - unit  
      
  Returns: REF register content
  
*/
uint8_t R_WDTA_READ_REGRefRegister(uint32_t Unit)
{
    uint32_t baseAddress = 0;
    uint8_t data;
    /* Get the BASE address */
   
    baseAddress = R_WDTA_Dev[Unit].BaseAddr;
       
    /* Read EVAC register */   
    data = R_WDTA_READ_REG(8, baseAddress + R_WDTA_REF);
    
    return (data);
}

/***********************************************************
  Function: R_WDTA_GetStatus

  Read REF register.
   
  Parameters: 
  Unit - unit  
      
  Returns: REF register content
  
*/
uint32_t R_WDTA_GetStatus(uint32_t Unit)
{
    uint32_t baseAddress = 0;
    uint32_t reg         = 0;
    uint32_t data        = 0;
    /* Get the BASE address */
   
    baseAddress = R_SYS_WDTA_GetBaseAddr(Unit);
     
    /* Read EVAC register */   
    reg = (uint32_t)R_WDTA_READ_REG(8, baseAddress + R_WDTA_WDTE);
    data = reg; 
    reg = (uint32_t)R_WDTA_READ_REG(8, baseAddress + R_WDTA_EVAC);
    data |= reg<<8; 
    reg = (uint32_t)R_WDTA_READ_REG(8, baseAddress + R_WDTA_REF);
    data |= reg<<16; 
    reg = (uint32_t)R_WDTA_READ_REG(8, baseAddress + R_WDTA_MD);
    data |= reg<<24; 
    return (data);
}



