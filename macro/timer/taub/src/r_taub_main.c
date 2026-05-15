/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_taub_main.c 7171 2016-01-06 17:52:36Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the TAUB macro
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
  Title: TAUB Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_taub.h"
#include "r_taub_regs.h"
#include "r_taub_api.h"
#define TAUB_MAIN_GLOBALS_
#include "r_taub_main.h"
#include "r_taub_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

#define CHANNEL_ALL                     (0xFFFF)
extern _Bool Turn_BuzzON;

/*******************************************************************************
  Constant: loc_TaubVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_TaubDrvVer[] = R_VERSION(TAUB, R_TAUB_VERSION_HI, R_TAUB_VERSION_LO);



/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Section: Local Functions
*/
static uint32_t loc_GetChannelAddress(r_taub_Channel_t Channel);  
static uint16_t loc_SetBits16(uint16_t RegValue, uint16_t BitValue, uint8_t BitPos, uint8_t BitWidth) ; 
static uint8_t loc_SetBits8(uint8_t RegValue, uint16_t BitValue, uint8_t BitPos, uint8_t BitWidth); 
static uint16_t loc_GetChannelID(r_taub_Channel_t Channel);
static void loc_InitRegs(uint32_t Unit); 

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;


/***********************************************************
  Function: loc_GetChannelAddress

  converts the channel to an offset address and assumes registers
  are spaced by an equal offset of 4 bytes

  Parameters:
  Channel        - TAUB channel number <r_taub_Channel_t>

   Returns:
  ChannelAddress - Address offset of four added to each channel

*/
static uint32_t loc_GetChannelAddress(r_taub_Channel_t Channel) 
{
    uint32_t ChannelAddress = 0;

    if (Channel  < R_TAUB_CHANNEL_ALL) 
    {
        ChannelAddress = (uint32_t) (Channel * 4);
    } 
    else 
    { /* error */
        ChannelAddress = 0xFFFFFFFFu;
    }

    return ChannelAddress;
}

/*******************************************************************************
  Function: loc_SetBits16
*/

static uint16_t loc_SetBits16(uint16_t RegValue, uint16_t BitValue, uint8_t BitPos, uint8_t BitWidth) 
{
    uint16_t RegMask   = 0x0000;
    uint16_t RegValMod = 0x0000;
    uint8_t  i;

    /* generate mask*/
    for (i = 0u; i < BitWidth; i++) 
    {
        RegMask = (uint16_t)(RegMask << 1) | 1u;
    }
    RegMask = (uint16_t)(RegMask << BitPos);

    /* clear bits*/
    RegValMod = RegValue & (uint16_t)(~RegMask);

    /* set bits*/
    RegValMod |= (uint16_t)(BitValue << BitPos);

    return RegValMod;
}

/*******************************************************************************
  Function: loc_SetBits8
*/

static uint8_t loc_SetBits8(uint8_t RegValue, uint16_t BitValue, uint8_t BitPos, uint8_t BitWidth) 
{
    uint8_t RegMask   = 0x00;
    uint8_t RegValMod = 0x00;
    uint8_t i;

    /* generate mask*/
    for (i = 0u; i < BitWidth; i++) 
    {
        RegMask = (uint16_t)((uint16_t)RegMask << 1) | 1u;
    }
    RegMask = (uint16_t)((uint16_t)RegMask << BitPos);

    /* clear bits*/
    RegValMod = RegValue & (uint16_t)(~((uint16_t)RegMask));

    /* set bits*/
    RegValMod |= (uint16_t)((uint16_t)BitValue << BitPos);

    return RegValMod;
}


/***********************************************************
  Function: loc_GetChannelID

  converts the channel from an enum to an uint16_t

  Parameters:
  Channel        - TAUB channel number <r_taub_Channel_t>

  Returns:
  u16_channel    - uint16_t version of the enum

*/

static uint16_t loc_GetChannelID(r_taub_Channel_t Channel)
{
    uint16_t u16_channel = 0x0000;

    if (Channel  < R_TAUB_CHANNEL_ALL) 
    {
        u16_channel = (uint16_t)Channel;
    }
    else
    {
        /* error */
        u16_channel = 0xFFFF;
    }

    return u16_channel;
}

/***********************************************************
  Function: loc_InitRegs

  Updates all the writeable registers in the TAUB with their default
  reset values

  Parameters:
  Channel        - TAUB channel number <r_taub_Channel_t>

  Returns:
  none

*/
static void loc_InitRegs(uint32_t Unit) 
{
    uint32_t i              = 0;
    uint32_t base_address   = 0;
    uint32_t offset_address = 0;

    base_address = r_taub_Dev[Unit].BaseAddr;

    /* channels should be stopped first */
    /* update the TT Channel Stop Trigger Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TT, 0xFFFF);

    /* update the TOE Channel Output Enable Register*/
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TOE, 0);

    for(i = 0; i < R_TAUB_NUM_CHANNELS; i++) 
    {
        /* update the CMOR Channel Mode OS Register */
        offset_address = R_TAUB_CMOR0 + loc_GetChannelAddress((r_taub_Channel_t) i);
        R_TAUB_WRITE_REG(16, base_address + offset_address, 0);

        /* update the CDR Channel Data Register */
        offset_address = R_TAUB_CDR0 + loc_GetChannelAddress((r_taub_Channel_t) i);
        R_TAUB_WRITE_REG(16, base_address + offset_address, 0);

        /* update the CMUR TAU Channel Mode User Register */
        offset_address = R_TAUB_CMUR0 + loc_GetChannelAddress((r_taub_Channel_t) i);
        R_TAUB_WRITE_REG(8, base_address + offset_address, 0);

        /* update the CSC Channel Status Clear Register*/
        offset_address = R_TAUB_CSC0 + loc_GetChannelAddress((r_taub_Channel_t) i);
        R_TAUB_WRITE_REG(8, base_address + offset_address, 1);
    }

    /* update the TPS Prescaler Clock Select Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TPS, 0xFFFFu);

    /* update the TO Channel Output Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TO, 0);

    /* update the TDE Channel Dead Time Output Enable Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TDE, 0);

    /* update the TDL Channel Dead Time Output Level Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TDL, 0);

    /* update the RDE Channel Reload Data Enable Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDE, 0);

    /* update the RDM Channel Reload Data Mode Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDM, 0);

    /* update the RDS Channel Reload Data Control CH Select Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDS, 0);

    /* update the RDC Channel Reload Data Control Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDC, 0);

    /* update the RDT CChannel Reload Data Trigger Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDT, 0);

    /* update the TS Channel Start Trigger Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TS, 0);

    /* update the TOM Channel Output Mode Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TOM, 0);

    /* update the TOC Channel Output Configuration Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TOC, 0);

    /* update the TOL Channel Output Level Register */
    R_TAUB_WRITE_REG(16, base_address + R_TAUB_TOL, 0);

    /* update the EMU Register */
    R_TAUB_WRITE_REG(8, base_address + R_TAUB_EMU, SVSTOP);
}



/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_TAUB_CheckMacroParams
  
  See: <taub_main.h>
*/
r_taub_Error_t R_TAUB_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_TAUB_MACRO_NUM <= Unit)
    {
        return  R_TAUB_ERR_RANGE; 
    }
    return R_TAUB_ERR_OK;  
}

/*******************************************************************************
  Function: R_TAUB_ErrorHandler
  
  See: <taub_main.h>
*/

void R_TAUB_ErrorHandler(uint32_t Unit, r_taub_Error_t Error) 
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
  
  See: <taub_api.h>
*/

/*******************************************************************************
  Function: R_TAUB_GetVersionStr

  See: <taub_api.h> 
*/
const int8_t * R_TAUB_GetVersionStr(void) 
{
    return (const int8_t *) loc_TaubDrvVer;
}

/*******************************************************************************
  Function: R_TAUB_GetMajorVersion

  See: <taub_api.h> 
*/
const uint16_t R_TAUB_GetMajorVersion(void) 
{
    return (uint16_t)R_TAUB_VERSION_HI; 
}

/*******************************************************************************
  Function: R_TAUB_GetMinorVersion

  See: <taub_api.h> 
*/
const uint16_t R_TAUB_GetMinorVersion(void) 
{
    return (uint16_t)R_TAUB_VERSION_LO; 
}



/*******************************************************************************
  Function: R_TAUB_Init

  See: <taub_api.h> 
*/

 r_taub_Error_t R_TAUB_Init(uint32_t Unit) 
 {
    r_taub_Error_t err;
    err = R_TAUB_CheckMacroParams(Unit); 
    if (R_TAUB_ERR_OK != err) 
    {
        return err; 
    }

    if (0 == r_taub_Dev[Unit].BaseAddr)
    {      
        /* update the BaseAddr */
        r_taub_Dev[Unit].BaseAddr = R_TAUB_SYS_BaseAddr(Unit);
        /* initialize registers */
        loc_InitRegs(Unit);

        err = R_TAUB_SYS_HardwareInit(Unit);
    }
    else 
    { /* please, no double-init*/
        err = R_TAUB_ERR_LOCKED; 
    }
    return err;
 }


/*******************************************************************************
  Function: R_TAUB_DeInit

  See: <taub_api.h> 
*/

 r_taub_Error_t R_TAUB_DeInit(uint32_t Unit)
 {
    r_taub_Error_t err;

    err = R_TAUB_CheckMacroParams(Unit); 

    if (R_TAUB_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_taub_Dev[Unit].BaseAddr)
    {
        
        r_taub_Dev[Unit].BaseAddr = 0;
        err = R_TAUB_SYS_HardwareDeInit(Unit);
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_TAUB_ERR_NOTLOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_TAUB_SetErrorCallback
*/

void R_TAUB_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}


/***********************************************************
  Function: r_taub_Start
*/

r_taub_Error_t r_taub_Start(uint32_t Unit, r_taub_Channel_t Channel) 
{
    r_taub_Error_t error_mode   = R_TAUB_ERR_OK;
    uint16_t       u16_channel  = 0;
    uint32_t       base_address = 0;
    uint16_t       u16_temp     = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM)) 
    {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the TAUB0TS: TAU Channel Start Trigger register */

        /* check the range for the channels */
        if (Channel > R_TAUB_CHANNEL_ALL) 
        {
            error_mode = R_TAUB_ERR_RANGE;
        } 
        else 
        {
            base_address = r_taub_Dev[Unit].BaseAddr;

            /* enable all channels */
            if (Channel == R_TAUB_CHANNEL_ALL) 
            {
                u16_temp = CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUB_SET_BIT(u16_temp, u16_channel);
            }

            /* update the TS Channel Start Trigger Register */
            R_TAUB_WRITE_REG (16, base_address + R_TAUB_TS, u16_temp);
        }
    }

    return error_mode;
}


/***********************************************************
  Function: r_taub_Stop
*/

r_taub_Error_t r_taub_Stop(uint32_t Unit, r_taub_Channel_t Channel) 
{
    uint16_t u16_channel  = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp     = 0;

    r_taub_Error_t error_mode = R_TAUB_ERR_OK;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM)) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    { /* set the channel to be stopped in the TAUB0TT: 
           TAU Channel Stop Trigger Register */

        /* check the range for the channels */
        if (Channel > R_TAUB_CHANNEL_ALL) 
        {
            error_mode = R_TAUB_ERR_RANGE;
        } 
        else 
        {
            /* enable all channels */
            if (Channel == R_TAUB_CHANNEL_ALL)
            {
                u16_temp = CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUB_SET_BIT(u16_temp, u16_channel);
            }

            base_address = r_taub_Dev[Unit].BaseAddr;

            /* update the TT Channel Stop Trigger Register (Write only) */
            R_TAUB_WRITE_REG (16, base_address + R_TAUB_TT, u16_temp);
        }
    }

    return error_mode;
}


/***********************************************************
  Function: r_taub_SetMode
*/

r_taub_Error_t r_taub_SetMode(uint32_t Unit, r_taub_Channel_t Channel, r_taub_Mode_t Mode) 
{
    r_taub_Error_t  error_mode      = R_TAUB_ERR_OK;
    uint32_t        base_address    = 0;
    uint32_t        offset_address  = 0;
    uint16_t        r_taub_reg_CMOR = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM)) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    }
    else 
    { /* ok, update the CMOR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMOR = R_TAUB_READ_REG (16, base_address + offset_address);

        /* timer interval */
        if (Mode == R_TAUB_INTERVAL_TIMER_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_INTERVAL_TIMER_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_JUDGE_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_JUDGE_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_CAPTURE_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CAPTURE_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_EVENT_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_EVENT_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_ONE_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_ONE_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_CAPTURE_ONE_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CAPTURE_ONE_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_JUDGE_ONE_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_JUDGE_ONE_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_UP_DOWN_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_UP_DOWN_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_PULSE_ONE_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_PULSE_ONE_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_COUNT_CAPTURE_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_COUNT_CAPTURE_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_GATE_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_GATE_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else if (Mode == R_TAUB_CAPTURE_GATE_COUNT_MODE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CAPTURE_GATE_COUNT_MODE, 
                                            R_TAUB_CMOR_MD_POS, R_TAUB_CMOR_MD_WID); 
        } 
        else 
        { /* error */
            error_mode = R_TAUB_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    { /* update the CMOR Channel Mode OS Register */
        base_address   = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG (16, base_address + offset_address, r_taub_reg_CMOR);
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_EnableIntAtStartUp
*/

r_taub_Error_t r_taub_EnableIntAtStartUp(uint32_t Unit, r_taub_Channel_t Channel, uint8_t IntAtStartUp)
{
    r_taub_Error_t error_mode      = R_TAUB_ERR_OK;
    uint32_t       base_address    = 0;
    uint32_t       offset_address  = 0;
    uint16_t       r_taub_reg_CMOR = 0;

    /* check the instance is allowed */
    if (Unit > R_TAUB_MACRO_NUM) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    { /* ok, update the CMOR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMOR = R_TAUB_READ_REG (16, base_address + offset_address);

        /* timer interval */
        if (IntAtStartUp == R_TRUE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, 1u, 
                                            R_TAUB_CMOR_MD0_POS, R_TAUB_CMOR_MD0_WID); 
        } 
        else 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, 0u, 
                                            R_TAUB_CMOR_MD0_POS, R_TAUB_CMOR_MD0_WID); 
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    { /* update the CMOR Channel Mode OS Register */
        base_address   = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG (16, base_address + offset_address, r_taub_reg_CMOR);
    }

    return error_mode;
}



/***********************************************************
  Function: r_taub_SetMaster
*/

r_taub_Error_t r_taub_SetMaster(uint32_t Unit, r_taub_Channel_t Channel, uint8_t Master) 
{
    r_taub_Error_t error_mode      = R_TAUB_ERR_OK;
    uint32_t       base_address    = 0;
    uint32_t       offset_address  = 0;
    uint16_t       r_taub_reg_CMOR = 0;

    /* check the instance is allowed */
    if (Unit > R_TAUB_MACRO_NUM) 
    {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMOR = R_TAUB_READ_REG (16, base_address + offset_address);

        /* timer interval */
        if (Master == R_TRUE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, 1u, R_TAUB_CMOR_MAS_POS, 
                                            R_TAUB_CMOR_MAS_WID); 
        }
        else 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, 0u, R_TAUB_CMOR_MAS_POS, 
                                            R_TAUB_CMOR_MAS_WID); 
        }
    }

    /* if no error then update the register */
    if(error_mode == R_TAUB_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address   = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG (16, base_address + offset_address, r_taub_reg_CMOR);
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_SetTrigger
*/

r_taub_Error_t r_taub_SetTrigger (uint32_t Unit, r_taub_Channel_t Channel,r_taub_Trigger_t Trigger)
{
    r_taub_Error_t error_mode      = R_TAUB_ERR_OK;
    uint32_t       base_address    = 0;
    uint32_t       offset_address  = 0;
    uint16_t       r_taub_reg_CMOR = 0;

    /* check the instance is allowed */
    if (Unit > R_TAUB_MACRO_NUM) 
    {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMOR = R_TAUB_READ_REG(16, base_address + offset_address);

        /* timer interval */
        if (Trigger == R_TAUB_NONE) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_NONE, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else if (Trigger == R_TAUB_VALID_EDGE_TIN) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_VALID_EDGE_TIN, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else if (Trigger == R_TAUB_VALID_EITHER_TIN) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_VALID_EITHER_TIN, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else if (Trigger == R_TAUB_INT_FROM_MASTER) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_INT_FROM_MASTER, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else if (Trigger == R_TAUB_INT_FROM_UPPER) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_INT_FROM_UPPER, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else if (Trigger == R_TAUB_TOUT_DEAD_TIME) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_TOUT_DEAD_TIME, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else if (Trigger == R_TAUB_UP_DOWN_FROM_MASTER) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_UP_DOWN_FROM_MASTER, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        } 
        else 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, 0u, 
                                            R_TAUB_CMOR_STS_POS, R_TAUB_CMOR_STS_WID);  
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG (16, base_address + offset_address, r_taub_reg_CMOR);
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_SetCountClock
*/

r_taub_Error_t r_taub_SetCountClock(uint32_t Unit, r_taub_Channel_t Channel, r_taub_CountClock_t CountClock)
{
    r_taub_Error_t error_mode      = R_TAUB_ERR_OK;
    uint32_t       base_address    = 0;
    uint32_t       offset_address  = 0;
    uint16_t       r_taub_reg_CMOR = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM)) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    { /* ok, update the CMOR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMOR = R_TAUB_READ_REG(16, base_address + offset_address);

        /* timer interval */
        if (CountClock == R_TAUB_COUNT_CLK_PRESCALER) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_COUNT_CLK_PRESCALER, 
                                            R_TAUB_CMOR_CCS0_POS, R_TAUB_CMOR_CCS0_WID);  
        } 
        else if (CountClock == R_TAUB_COUNT_CLK_TIN) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_COUNT_CLK_TIN, 
                                            R_TAUB_CMOR_CCS0_POS, R_TAUB_CMOR_CCS0_WID);  
        } 
        else 
        {
            error_mode = R_TAUB_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    { /* update the CMOR Channel Mode OS Register */
        base_address   = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG (16, base_address + offset_address, r_taub_reg_CMOR);
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_SetClkSource
*/

r_taub_Error_t r_taub_SetClkSource(uint32_t Unit, r_taub_Channel_t Channel, r_taub_Clock_t Clock) 
{
    r_taub_Error_t error_mode      = R_TAUB_ERR_OK;
    uint32_t       base_address    = 0;
    uint32_t       offset_address  = 0;
    uint16_t       r_taub_reg_CMOR = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM)) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    }
    else 
    { /* ok, update the CMOR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMOR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMOR = R_TAUB_READ_REG (16, base_address + offset_address);

        if (Clock == R_TAUB_CK_0) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CK_0, 
                                            R_TAUB_CMOR_CKS_POS, R_TAUB_CMOR_CKS_WID); 
        } 
        else if (Clock == R_TAUB_CK_1) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CK_1, 
                                            R_TAUB_CMOR_CKS_POS, R_TAUB_CMOR_CKS_WID); 
        } 
        else if (Clock == R_TAUB_CK_2) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CK_2, 
                                            R_TAUB_CMOR_CKS_POS, R_TAUB_CMOR_CKS_WID); 
        } 
        else if (Clock == R_TAUB_CK_3) 
        {
            r_taub_reg_CMOR = loc_SetBits16(r_taub_reg_CMOR, R_TAUB_CK_3, 
                                            R_TAUB_CMOR_CKS_POS, R_TAUB_CMOR_CKS_WID); 
        } 
        else 
        { /* error */
            error_mode = R_TAUB_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    { /* update the CMOR Channel Mode OS Register */
        R_TAUB_WRITE_REG (16, base_address + offset_address, r_taub_reg_CMOR);
    }
    return error_mode;
}


/***********************************************************
  Function: r_taub_SetClockPeriod
*/

r_taub_Error_t r_taub_SetClockPeriod (uint32_t Unit, r_taub_Clock_t Clock, r_taub_TimeInterval_t TimeInterval)
{
    uint8_t        ucTimeInterval = 0;
    uint32_t       base_address   = 0;
    r_taub_Error_t error_mode     = R_TAUB_ERR_OK;
    uint16_t       r_taub_reg_TPS = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM))
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    { /* ok, update the TPS register value */
        base_address = r_taub_Dev[Unit].BaseAddr;

        /* get the current value */
        r_taub_reg_TPS = R_TAUB_READ_REG (16, base_address + R_TAUB_TPS);
        if (TimeInterval <= R_TAUB_TIME_INTERVAL_15) 
        {
            ucTimeInterval = TimeInterval; 
        }
        else 
        { /* error */
            error_mode = R_TAUB_ERR_RANGE;
        }

        /* if a valid time interval the write the clock */
        if (error_mode == R_TAUB_ERR_OK) 
        {
            if (Clock == R_TAUB_CK_0) 
            {
                r_taub_reg_TPS = loc_SetBits16(r_taub_reg_TPS, ucTimeInterval, 
                                               R_TAUB_TPS_PRS0_POS, R_TAUB_TPS_PRS0_WID); 
            } 
            else if (Clock == R_TAUB_CK_1) 
            {
                r_taub_reg_TPS = loc_SetBits16(r_taub_reg_TPS, ucTimeInterval, 
                                               R_TAUB_TPS_PRS1_POS, R_TAUB_TPS_PRS1_WID); 
            } 
            else if (Clock == R_TAUB_CK_2) 
            {
                r_taub_reg_TPS = loc_SetBits16(r_taub_reg_TPS, ucTimeInterval, 
                                               R_TAUB_TPS_PRS2_POS, R_TAUB_TPS_PRS2_WID); 
            } 
            else if (Clock == R_TAUB_CK_3) 
            {
                r_taub_reg_TPS = loc_SetBits16(r_taub_reg_TPS, ucTimeInterval, 
                                               R_TAUB_TPS_PRS3_POS, R_TAUB_TPS_PRS3_WID); 
            } 
            else 
            { /* error */
                error_mode = R_TAUB_ERR_RANGE;
            }
        } 
        else 
        {
            /* error do not write anything, the return value will indicate the error condition */
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    {
        /* update the TPS Prescaler Clock Select Register */
        R_TAUB_WRITE_REG (16, base_address + R_TAUB_TPS, r_taub_reg_TPS);
    }

    return error_mode;
}



/***********************************************************
  Function: r_taub_OutputEnable
*/

r_taub_Error_t r_taub_OutputEnable (uint32_t Unit, r_taub_Channel_t Channel) 
{
    r_taub_Error_t error_mode   = R_TAUB_ERR_OK;
    uint16_t       u16_channel  = 0;
    uint32_t       base_address = 0;
    uint16_t       u16_temp     = 0;

    /* check the instance is allowed */
    if (Unit > R_TAUB_MACRO_NUM) 
    {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the TAUB0TOE: TAU Channel Output Enable register */

        /* check the range for the channels */
        if (Channel > R_TAUB_CHANNEL_ALL) 
        {
            error_mode = R_TAUB_ERR_RANGE;
        }
        else 
        {
            base_address = r_taub_Dev[Unit].BaseAddr;
            u16_temp     = R_TAUB_READ_REG(16, base_address + R_TAUB_TOE);

            /* enable all channels */
            if (Channel == R_TAUB_CHANNEL_ALL) 
            {
                u16_temp = CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUB_SET_BIT(u16_temp, u16_channel);
            }

            /* update the TOE */
            R_TAUB_WRITE_REG (16, base_address + R_TAUB_TOE, u16_temp);

            /* verify the register write was succesfull */
            if (u16_temp != R_TAUB_READ_REG (16, base_address + R_TAUB_TOE)) 
            {
                /* error_mode = R_TAUB_ERR_REG_UPDATE_FAIL; */
                error_mode = R_TAUB_ERR_LOCKED;
                /* _UserErrorHandler(Unit, error_mode); */
            }
        }
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_OutputDisable
*/

r_taub_Error_t r_taub_OutputDisable(uint32_t Unit, r_taub_Channel_t Channel) 
{
    r_taub_Error_t error_mode   = R_TAUB_ERR_OK;
    uint16_t       u16_channel  = 0;
    uint32_t       base_address = 0;
    uint16_t       u16_temp     = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM - 1)) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    { /* ok, update the TAUB0TOE: TAU Channel Output Enable register */

        /* check the range for the channels */
        if (Channel > R_TAUB_CHANNEL_ALL) 
        {
            error_mode = R_TAUB_ERR_RANGE;
        } 
        else 
        {
            base_address = r_taub_Dev[Unit].BaseAddr;
            u16_temp     = R_TAUB_READ_REG(16, base_address + R_TAUB_TOE);

            /* enable all channels */
            if (Channel == R_TAUB_CHANNEL_ALL) 
            {
                u16_temp = 0;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUB_CLR_BIT(u16_temp, u16_channel);
            }

            /* update the TOE */
            R_TAUB_WRITE_REG (16, base_address + R_TAUB_TOE, u16_temp);
        }
    }
    Turn_BuzzON = 0;
    return error_mode;
}


/************************************************************
  Function: r_taub_SetPeriod
*/

r_taub_Error_t r_taub_SetPeriod(uint32_t Unit, r_taub_Channel_t Channel, uint16_t Period)   
{
    r_taub_Error_t error_mode     = R_TAUB_ERR_OK;
    uint32_t       base_address   = 0;
    uint32_t       offset_address = 0;

    /* check the instance is allowed */
    if (Unit > R_TAUB_MACRO_NUM) 
    {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } 
    else 
    { /* ok, update the CDR register value */
        base_address   = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CDR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG(16, base_address + offset_address, Period);
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_SetTinEdge
*/

r_taub_Error_t r_taub_SetTinEdge(uint32_t Unit, r_taub_Channel_t Channel, r_taub_TinEdge_t Edge) 
{
    r_taub_Error_t error_mode      = R_TAUB_ERR_OK;
    uint32_t       base_address    = 0;
    uint32_t       offset_address  = 0;
    uint16_t       r_taub_reg_CMUR = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUB_MACRO_NUM)) 
    { /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    }
    else 
    { /* ok, update the CMUR register value */
        base_address    = r_taub_Dev[Unit].BaseAddr;
        offset_address  = R_TAUB_CMUR0 + loc_GetChannelAddress(Channel);
        r_taub_reg_CMUR = R_TAUB_READ_REG(8, base_address + offset_address);

        /* timer interval */
        if (Edge == R_TAUB_FALLING_EDGE) 
        {
            r_taub_reg_CMUR = loc_SetBits8(r_taub_reg_CMUR, R_TAUB_FALLING_EDGE, 
                                           R_TAUB_CMUR_TIS_POS, R_TAUB_CMUR_TIS_WID);  
        } 
        else if (Edge == R_TAUB_RISING_EDGE) 
        {
            r_taub_reg_CMUR = loc_SetBits8(r_taub_reg_CMUR, R_TAUB_RISING_EDGE, 
                                           R_TAUB_CMUR_TIS_POS, R_TAUB_CMUR_TIS_WID);  
        } 
        else if (Edge == R_TAUB_BOTH_LOW_WIDTH) 
        {
            r_taub_reg_CMUR = loc_SetBits8(r_taub_reg_CMUR, R_TAUB_BOTH_LOW_WIDTH, 
                                           R_TAUB_CMUR_TIS_POS, R_TAUB_CMUR_TIS_WID);  
        } 
        else if (Edge == R_TAUB_BOTH_HIGH_WIDTH) 
        {
            r_taub_reg_CMUR = loc_SetBits8(r_taub_reg_CMUR, R_TAUB_BOTH_HIGH_WIDTH, 
                                           R_TAUB_CMUR_TIS_POS, R_TAUB_CMUR_TIS_WID);  
        } 
        else 
        {
            error_mode = R_TAUB_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUB_ERR_OK) 
    { /* update the CMUR Channel Mode OS Register */
        base_address   = r_taub_Dev[Unit].BaseAddr;
        offset_address = R_TAUB_CMUR0 + loc_GetChannelAddress(Channel);
        R_TAUB_WRITE_REG (8, base_address + offset_address, r_taub_reg_CMUR);
    }

    return error_mode;
  }


/***********************************************************
  Function: r_taub_ClearOverflow
*/

r_taub_Error_t r_taub_ClearOverflow(uint32_t Unit, r_taub_Channel_t Channel) 
{
    r_taub_Error_t error_mode     = R_TAUB_ERR_OK;
    uint32_t       base_address   = 0;
    uint32_t       offset_address = 0;
    uint8_t        r_taub_reg_CSC = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUB_MACRO_NUM)) 
    {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    }
    else 
    {
        if (Channel == R_TAUB_CHANNEL_ALL) 
        {
            error_mode = R_TAUB_ERR_RANGE;
        } 
        else 
        {
            base_address   = r_taub_Dev[Unit].BaseAddr;
            offset_address = R_TAUB_CSC0 + loc_GetChannelAddress(Channel);
            r_taub_reg_CSC = loc_SetBits8(r_taub_reg_CSC, 1u, 
                                          R_TAUB_CSC_CLOV_POS, R_TAUB_CSC_CLOV_WID); 

            /* update the CSC Channel Status Clear Register */
            R_TAUB_WRITE_REG (8, base_address + offset_address, r_taub_reg_CSC);
        }
    }

    return error_mode;
}


/***********************************************************
  Function: r_taub_EnableSimultaneousRewrite
*/

r_taub_Error_t r_taub_EnableSimultaneousRewrite(
                    uint32_t Unit, 
                    r_taub_Channel_t Channel,
                    r_taub_SimultaneousTrigger_t SimultaneousTriggerSource)
{
    r_taub_Error_t error_mode = R_TAUB_ERR_OK;
    uint32_t base_address = 0;
    uint16_t u16_RDE = 0;
    uint16_t u16_RDS = 0;
    uint16_t u16_channel = 0;

    /* check the instance is allowed */
    if(Unit > (R_TAUB_MACRO_NUM)) {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } else {
        /* ok, update the Channel Reload Data Enable Register (RDE) */
        base_address = r_taub_Dev[Unit].BaseAddr;
        u16_RDE = R_TAUB_READ_REG(16, base_address + R_TAUB_RDE);

        /* enable all channels */
        if(Channel == R_TAUB_CHANNEL_ALL) {
            u16_RDE = CHANNEL_ALL;
        } else { /* just one channel */
            u16_channel = loc_GetChannelID(Channel);
            R_TAUB_SET_BIT(u16_RDE, u16_channel);
        }

        /* update the RDS */
        u16_RDS = R_TAUB_READ_REG(16, base_address + R_TAUB_RDS);

        if(SimultaneousTriggerSource == R_TAUB_FROM_MASTER) {
             R_TAUB_CLR_BIT(u16_RDS, u16_channel);
        } else if(SimultaneousTriggerSource == R_TAUB_FROM_UPPER) {
             R_TAUB_SET_BIT(u16_RDS, u16_channel);
        } else {
            error_mode = R_TAUB_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if(error_mode == R_TAUB_ERR_OK) {
        /* update the CMOR Channel Mode OS Register */
        base_address = r_taub_Dev[Unit].BaseAddr;
        R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDE, u16_RDE);
        R_TAUB_WRITE_REG(16, base_address + R_TAUB_RDS, u16_RDS);
    }

    return error_mode;
}

/***********************************************************
  Function: r_taub_EnableSynchronousChannelOperation
*/

r_taub_Error_t r_taub_EnableSynchronousChannelOperation(uint32_t Unit, r_taub_Channel_t Channel)
{
    r_taub_Error_t error_mode = R_TAUB_ERR_OK;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;
    uint16_t u16_channel = 0;

    /* check the instance is allowed */
    if(Unit > (R_TAUB_MACRO_NUM)) {
        /* instance too high */
        error_mode = R_TAUB_ERR_INSTANCE;
    } else {
        /* ok */
        base_address = r_taub_Dev[Unit].BaseAddr;
        u16_temp = R_TAUB_READ_REG(16, base_address + R_TAUB_TOM);

        /* enable all channels */
        if(Channel == R_TAUB_CHANNEL_ALL) {
            u16_temp = CHANNEL_ALL;
        } else { /* just one channel */
            u16_channel = loc_GetChannelID(Channel);
            R_TAUB_SET_BIT(u16_temp, u16_channel);
        }

        /* update the TOM */
        R_TAUB_WRITE_REG(16, base_address + R_TAUB_TOM, u16_temp);

        /* verify the register write was succesfull */
        if(u16_temp != (R_TAUB_READ_REG(16, base_address + R_TAUB_TOM))) {
            error_mode = R_TAUB_ERR_REG_UPDATE_FAIL;
        }
    }

    return error_mode;
}

/***********************************************************
  Function: R_TAUJ_GetCaptureData
*/

uint16_t r_taub_GetCaptureData(uint32_t Unit, r_taub_Channel_t Channel) 
{
    uint16_t u16_temp = 0;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    base_address = r_taub_Dev[Unit].BaseAddr;

    /* read the CDR Channel Counter Register */
    offset_address = R_TAUB_CDR0 + loc_GetChannelAddress(Channel);
    u16_temp = R_TAUB_READ_REG(16, base_address + offset_address);

    return u16_temp;
}
