/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_lcbi_main.c 7329 2016-01-22 10:40:45Z matthias.nippert $
============================================================================ 
DESCRIPTION
Driver for the LCBI macro
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
  Title: LCBI Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_lcbi.h"
#include "r_ddb_api.h"
#include "r_lcbi_api.h"
#define LCBI_MAIN_GLOBALS_
#include "r_lcbi_main.h"
#include "r_lcbi_regs.h"
#include "r_lcbi_sys.h"
#include <math.h>

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_LcbiVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_LcbiDrvVer[] = R_VERSION(LCBI, R_LCBI_VERSION_HI, R_LCBI_VERSION_LO);



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
  Section: Local Functions
*/
/* Local Function Headers */
static uint32_t loc_calcClockDiv(uint32_t clkin, uint32_t clkout);
void R_LCBI_WaitWorkModeToZero(uint32_t Unit);

/*******************************************************************************
  Function: loc_calcClockDiv
  
  calculate best divider for given input and output clock frequency
*/
static uint32_t loc_calcClockDiv(uint32_t clkin, uint32_t clkout)
{
    uint32_t div;
    uint32_t divfrq[2];
    
    /* calc near divider */
    div = clkin / clkout;
    
    /* test if next divider is better, if yes choose next divider */
    divfrq[0] = clkin / div;
    divfrq[1] = clkin / (div+1);
    if ( (divfrq[0] - clkout) > (clkout - divfrq[1]))
    {
        div += 1;
    }
    
    /* divider value 0 is invalid, choose 1 instead */
    if (div == 0)
    {
        div = 1;
    }
    return div;
}


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_LCBI_CheckMacroParams
  
  See: <lcbi_main.h>
*/
r_lcbi_Error_t R_LCBI_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_LCBI_MACRO_NUM <= Unit)
    {
        return  R_LCBI_ERR_RANGE; 
    }
    return R_LCBI_ERR_OK;  
}

/*******************************************************************************
  Function: R_LCBI_ErrorHandler
  
  See: <lcbi_main.h>
*/

void R_LCBI_ErrorHandler(uint32_t Unit, r_lcbi_Error_t Error) 
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


/***********************************************************
  Function: R_LCBI_WaitWorkModeToZero

  Poll the operation mode and ensure it has gone to zero.

  Parameters:
  Unit  - Instance number
  
  Returns:
  void  -
*/  

void R_LCBI_WaitWorkModeToZero(uint32_t Unit)
{
    r_lcbi_RegOpmode_t RegOpmode;
	
    do {
        RegOpmode.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_OPMODE);
    }while(RegOpmode.Bits.Opm_ode != (uint8_t)(0));
}


/*******************************************************************************
  Section: Global API Functions
  
  See: <lcbi_api.h>
*/

/*******************************************************************************
  Function: R_LCBI_GetVersionStr

  See: <lcbi_api.h> 
*/
const int8_t * R_LCBI_GetVersionStr(void) 
{
    return (const int8_t *) loc_LcbiDrvVer;
}

/*******************************************************************************
  Function: R_LCBI_GetMajorVersion

  See: <lcbi_api.h> 
*/
const uint16_t R_LCBI_GetMajorVersion(void) 
{
    return (uint16_t)R_LCBI_VERSION_HI; 
}

/*******************************************************************************
  Function: R_LCBI_GetMinorVersion

  See: <lcbi_api.h> 
*/
const uint16_t R_LCBI_GetMinorVersion(void) 
{
    return (uint16_t)R_LCBI_VERSION_LO; 
}



/*******************************************************************************
  Function: R_LCBI_Init

  See: <lcbi_api.h> 
*/

 r_lcbi_Error_t R_LCBI_Init(uint32_t Unit) 
 {
    r_lcbi_Error_t err;
    err = R_LCBI_CheckMacroParams(Unit); 
    if (R_LCBI_ERR_OK != err) 
    {
        return err; 
    }
    if (0 == r_lcbi_Dev[Unit].BaseAddr)
    {
        
        r_lcbi_Dev[Unit].BaseAddr = R_LCBI_SYS_BaseAddr(Unit);
        err = R_LCBI_SYS_HardwareInit(Unit);
        R_LCBI_SYS_IntCInit(Unit);
        R_LCBI_ResetLcdBIF(Unit);
    }
    else 
    {   
        /* please, no double-init*/
        err = R_LCBI_ERR_LOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_LCBI_DeInit

  See: <lcbi_api.h> 
*/

 r_lcbi_Error_t R_LCBI_DeInit(uint32_t Unit)
 {
    r_lcbi_Error_t err;

    err = R_LCBI_CheckMacroParams(Unit); 

    if (R_LCBI_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_lcbi_Dev[Unit].BaseAddr)
    {
        err = R_LCBI_SYS_HardwareDeInit(Unit);
        r_lcbi_Dev[Unit].BaseAddr = 0;
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_LCBI_ERR_NOTLOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_LCBI_SetErrorCallback
*/

void R_LCBI_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}


/***********************************************************
  Function: R_LCBI_GetFIFOFillLevel
*/

uint8_t R_LCBI_GetFIFOFillLevel(uint32_t Unit)
{
    r_lcbi_RegState_t state;
    
    state.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_STATE);
    
    return state.Bits.Ffl;
}


/***********************************************************
  Function: R_LCBI_StopLcdBIF
*/

void R_LCBI_StopLcdBIF(uint32_t Unit)
{
    r_lcbi_RegOpmode_t RegOpmode;
    
	RegOpmode.DWord = 0;
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_OPMODE, RegOpmode.DWord);
    R_LCBI_WaitWorkModeToZero(Unit);
}


/************************************************************
  Function: R_LCBI_GetFSMState
*/

uint8_t R_LCBI_GetFSMState(uint32_t Unit)
{
    r_lcbi_RegState_t state;
    
    state.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_STATE);
    return state.Bits.State;
}


/***********************************************************
  Function: R_LCBI_Check_TC_LOCK
*/

uint8_t R_LCBI_Check_TcLock(uint32_t Unit)
{
    r_lcbi_RegStatus_t status;
	
    status.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_STATUS);
    return status.Bits.Tc_Lock;
}


/************************************************************
  Function: R_LCBI_IsReadPending
*/

uint8_t R_LCBI_IsReadPending(uint32_t Unit)
{
    r_lcbi_RegStatus_t status;
	
    status.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_STATUS);
    return status.Bits.Tc_Rpg;
}


/************************************************************
  Function: R_LCBI_WaitBusBusy
*/

void R_LCBI_WaitTransferring(uint32_t Unit)
{    
    r_lcbi_RegStatus_t status;
		
    do {
        status.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_STATUS);
    }while (status.Bits.Tc_Idle != (uint8_t)1);
}


/************************************************************
  Function: R_LCBI_SetClock
*/

void R_LCBI_SetClock(uint32_t Unit, r_lcbi_CLKDIV_t ClockDiv)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_CKSEL, ClockDiv);
}


/************************************************************
  Function: R_LCBI_InvertOutput
*/

void R_LCBI_InvertOutput(uint32_t Unit, r_lcbi_Output_Active_t Val)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_OUTLEV, Val);
}


/************************************************************
  Function: R_LCBI_SetBusCycle
*/

void R_LCBI_SetBusCycle(uint32_t Unit, r_lcbi_BusCycle_Type_t BusCycle)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_BCYCT, (uint32_t)BusCycle);
}


/************************************************************
  Function: R_LCBI_SetNonTFTCycleSpec
*/

void R_LCBI_SetNonTFTCycleSpec(uint32_t Unit, uint32_t NonTFTSpec)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_BCYC, NonTFTSpec);
}


/************************************************************
  Function: R_LCBI_SetTFTPrescaler
*/

void R_LCBI_SetTFTPrescaler(uint32_t Unit, uint32_t TFTPrs)
{
    if (TFTPrs <= 1)
    {
        TFTPrs = 2;
    }
    TFTPrs -= 1;
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_TFTPRS, TFTPrs);
}


/************************************************************
  Function: R_LCBI_SetTFTCycleC
*/

void R_LCBI_SetTFTCycleC(uint32_t Unit, uint32_t TFTCycleC)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_TFTCYC0, TFTCycleC);
}


/************************************************************
  Function: R_LCBI_SetTFTCycleV
*/

void R_LCBI_SetTFTCycleV(uint32_t Unit, uint32_t TFTCycleV)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_TFTCYC1, TFTCycleV);
}


/************************************************************
  Function: R_LCBI_SetTFTCycleH
*/

void R_LCBI_SetTFTCycleH(uint32_t Unit, uint32_t TFTCycleH)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_TFTCYC2, TFTCycleH);
}


/************************************************************
  Function: R_LCBI_GetRegister
*/

uint32_t R_LCBI_GetRegister(uint32_t Unit, uint32_t Addr)
{
    uint32_t val;
    
    val = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + Addr);
    return val;
}


/************************************************************
  Function: R_LCBI_ResetLcdBIF
*/

void R_LCBI_ResetLcdBIF(uint32_t Unit)
{
	uint8_t val;
	
	val = 0x01;
	R_LCBI_WRITE_REG(8, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_SRESET, val);
}


/************************************************************
  Function: R_LCBI_WriteData8_A0S
*/

void R_LCBI_WriteData8_A0S(uint32_t Unit, uint8_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0S_W8, Data);
}


/************************************************************
  Function: R_LCBI_WriteData8_A0C
*/

void R_LCBI_WriteData8_A0C(uint32_t Unit, uint8_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0C_W8, Data);
}


/************************************************************
  Function: R_LCBI_WriteData16_A0S
*/

void R_LCBI_WriteData16_A0S(uint32_t Unit, uint16_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0S_W16, Data);
}


/************************************************************
  Function: R_LCBI_WriteData16_A0C
*/

void R_LCBI_WriteData16_A0C(uint32_t Unit, uint16_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0C_W16, Data);
}


/************************************************************
  Function: R_LCBI_WriteData32_A0S
*/

void R_LCBI_WriteData32_A0S(uint32_t Unit, uint32_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0S_W32, Data);
}


/************************************************************
  Function: R_LCBI_WriteData32_A0C
*/

void R_LCBI_WriteData32_A0C(uint32_t Unit, uint32_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0C_W32, Data);
}


/************************************************************
  Function: R_LCBI_ReadData8_A0S
*/

uint8_t R_LCBI_ReadData8_A0S(uint32_t Unit)
{
    uint8_t result;
    
    result = (uint8_t)(R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0S_RT8) & 0x000000FFu);
    
    return result;
}



/************************************************************
  Function: R_LCBI_ReadData8_A0C
*/
uint8_t R_LCBI_ReadData8_A0C(uint32_t Unit)
{
    uint8_t result;
    
    result = (uint8_t)(R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0C_RT8) & 0x000000FFu);
    
    return result;
}


/************************************************************
  Function: R_LCBI_ReadData16_A0S
*/

uint16_t R_LCBI_ReadData16_A0S(uint32_t Unit)
{
    uint16_t result;
    
    result = (uint16_t)(R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0S_RT16) & 0x0000FFFFu);
    
    return result;
}



/************************************************************
  Function: R_LCBI_ReadData16_A0C
*/
uint16_t R_LCBI_ReadData16_A0C(uint32_t Unit)
{
    uint16_t result;
    
    result = (uint16_t)(R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0C_RT16) & 0x0000FFFFu);
    
    return result;
}


/************************************************************
  Function: R_LCBI_ReadData32_A0S
*/

uint32_t R_LCBI_ReadData32_A0S(uint32_t Unit)
{
    uint32_t result;
    
    result = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0S_RT32);
    
    return result;
}



/************************************************************
  Function: R_LCBI_ReadData32_A0C
*/
uint32_t R_LCBI_ReadData32_A0C(uint32_t Unit)
{
    uint32_t result;
    
    result = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_DATA_A0C_RT32);
    
    return result;
}


/************************************************************
  Function: R_LCBI_WriteCLUT
*/

void R_LCBI_WriteCLUT(uint32_t Unit, uint32_t Offset, uint32_t Data)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_CDATA_BASE + (Offset * 4), Data);
}


/************************************************************
  Function: R_LCBI_ReadCLUT
*/

uint32_t R_LCBI_ReadCLUT(uint32_t Unit, uint32_t Offset)
{
    return R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_CDATA_BASE + (Offset * 4));
}



/************************************************************
  Function: R_LCBI_SetCLUTOffset
*/

void R_LCBI_SetCLUTOffset(uint32_t Unit, uint32_t Offset)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_CLUTOFFS, Offset);
}


/************************************************************
  Function: R_LCBI_GetWorkingMode
*/

uint8_t R_LCBI_GetWorkingMode(uint32_t Unit)
{
    r_lcbi_RegOpmode_t RegOpmode;
    
    RegOpmode.DWord = R_LCBI_READ_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_OPMODE);
    return (uint8_t)RegOpmode.Bits.Opm_ode;
}


/************************************************************
  Function: R_LCBI_SetWorkingParam
*/

void R_LCBI_SetWorkingParam(uint32_t Unit, r_lcbi_Parameter_t* Param)
{
    r_lcbi_RegOpmode_t opmode;
    
    opmode.DWord = 0;    
    opmode.Bits.Opm_ode  = (uint8_t)Param->WorkMode;
    opmode.Bits.Opm_fast = (uint8_t)Param->CycleSpeed;
    
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_OPMODE, opmode.DWord);
}


/************************************************************
  Function: R_LCBI_SetTConParam
*/

void R_LCBI_SetTConParam(uint32_t Unit, r_lcbi_Parameter_t* Param)
{
    r_lcbi_RegTcon_t   tcon;
    
    tcon.Bits.Dtc_c = (uint8_t)Param->TransMode;
    tcon.Bits.Dtc_p = (uint8_t)Param->IndPalette;
    tcon.Bits.Dtc_m = (uint8_t)Param->AccessBit;
    tcon.Bits.Dtc_s = (uint8_t)Param->ShiftBit;
    
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + R_LCBI_TCONTROL, tcon.DWord);
}


/************************************************************
  Function: R_LCBI_WriteReg
*/

void R_LCBI_WriteReg(uint32_t Unit, uint32_t Data, uint32_t Offset)
{
    R_LCBI_WRITE_REG(32, r_lcbi_Dev[Unit].BaseAddr + Offset, Data);
}

/*******************************************************************************
  Function: R_LCBI_GetClock
*/
uint32_t R_LCBI_GetClock(uint32_t Unit)
{
    uint32_t val;                                 
                                                                                       
    if(Unit == 0)
    {
        val = R_LCBI_SYS_GetClock(Unit);
    }
    else
    {
        val = 0;
    }
    return val;                                   
}

/*******************************************************************************
  Function: R_LCBI_DisplayTimingSet
*/

r_lcbi_Error_t R_LCBI_DisplayTimingSet(uint32_t Unit, r_ddb_Timing_t *timing)
{
    r_lcbi_Error_t err;
    uint32_t clkdiv, clkapb, clkop, clkpix;
    uint32_t cyc;
    r_lcbi_Output_Active_t pol;
    
    err = R_LCBI_CheckMacroParams(Unit); 
    if (R_LCBI_ERR_OK != err) 
    {
        return err;
    }
    
    R_LCBI_StopLcdBIF(Unit);
    clkapb = R_LCBI_GetClock(Unit);
    clkpix = timing->PixelClock;

    clkop = clkapb / ((uint32_t)pow(2, (R_LCBI_CLKDIV_2)));
    
    R_LCBI_SetClock(Unit, R_LCBI_CLKDIV_2);
    clkdiv = loc_calcClockDiv(clkop, clkpix);
    R_LCBI_SetTFTPrescaler(Unit, clkdiv);
    
    /* pixel clock active time setup.
       (Where pixel clock cycle depends on operation clock of LCBI.) */
    cyc = ( (((clkop / clkpix) / 4uL) << 24)
            | (0uL << 16) 
            | (8uL <<  0) );
    R_LCBI_SetTFTCycleC(Unit, cyc);
    cyc  = (  ((timing->V.FrontPorch - 1uL) << 26)
            | ((timing->V.Total - timing->V.BlankWidth - 1uL) << 16)
            | ((timing->V.BlankWidth - timing->V.FrontPorch - timing->V.SyncWidth - 1uL) << 8)
            | ((timing->V.SyncWidth - 1uL)  << 0) );
    R_LCBI_SetTFTCycleV(Unit, cyc);
    cyc  = (  ((timing->H.FrontPorch - 1uL) << 26)
            | ((timing->H.Total - timing->H.BlankWidth - 1uL) << 16)
            | ((timing->H.BlankWidth - timing->H.FrontPorch - timing->H.SyncWidth - 1uL) << 8)
            | ((timing->H.SyncWidth - 1uL)  << 0) );
    R_LCBI_SetTFTCycleH(Unit, cyc);
    
    pol = R_LCBI_ACTIVE_DEFAULT;
    if (0 != (timing->Flags & R_DDB_DISP_FLAG_HSYNC_ACTHI) )
    {
        pol = (r_lcbi_Output_Active_t) ((uint32_t)pol | (uint32_t)R_LCBI_HS_ACTIVEHIGH );
    }
    if (0 != (timing->Flags & R_DDB_DISP_FLAG_VSYNC_ACTHI) )
    {
        pol = (r_lcbi_Output_Active_t) ((uint32_t)pol | (uint32_t)R_LCBI_VS_ACTIVEHIGH );
    }
    if (0 != (timing->Flags & R_DDB_DISP_FLAG_DESYNC_ACTHI) )
    {
        pol = (r_lcbi_Output_Active_t) ((uint32_t)pol | (uint32_t)R_LCBI_DE_ACTIVEHIGH );
    }
    R_LCBI_InvertOutput(Unit, pol);
    
    return err;
}

