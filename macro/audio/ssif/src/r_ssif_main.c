/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_ssif_main.c 7171 2016-01-06 17:52:36Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the SSIF macro
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
  Title: SSIF Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_ssif.h"
#include "r_tick_api.h"
#include "r_ssif_api.h"
#define SSIF_MAIN_GLOBALS_
#include "r_ssif_main.h"
#include "r_ssif_regs.h"
#include "r_ssif_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_SsifVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_SsifDrvVer[] = R_VERSION(SSIF, R_SSIF_VERSION_HI, R_SSIF_VERSION_LO);



/*******************************************************************************
  Section: Local Variables
*/
/*******************************************************************************
  Variable: locAudioDivSetting

  Available audio clock settings (register & divider)   
*/
uint32_t locAudioDivSetting[14][2] =
{
/* Divider, RegSetting */
  {  1,      0x0},
  {  2,      0x1},
  {  4,      0x2},
  {  6,      0x8},
  {  8,      0x3},
  { 12,      0x9},
  { 16,      0x4},
  { 24,      0xA},
  { 32,      0x5},
  { 48,      0xB},
  { 64,      0x6},
  { 96,      0xC},
  {128,      0x7}
};


/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;

/*******************************************************************************
  Section: Local Function declarations 
*/
static uint32_t locGetSerialClockOversamplingRatioRegVal(uint32_t Unit, uint32_t Ratio); 

/*******************************************************************************
  Section: Local Functions
*/


/*******************************************************************************
  Function: locGetSerialClockOversamplingRatioRegVal
  
  Get the best divider for the Serial Oversampling Clock ratio.
  
  Parameters:
  Unit       - Instance number 
  Ratio      - Intended Ratio
  
  Return: register value to set this Ratio.
*/
static uint32_t locGetSerialClockOversamplingRatioRegVal(uint32_t Unit, uint32_t Ratio)
{
    uint32_t i;
    for (i=0; i<14; i++)
    {
        if (Ratio <= locAudioDivSetting[i][0])
        {
            break;  /* found fitting divider */
        }
        i++;
    }
    return locAudioDivSetting[i][1];
}



/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_SSIF_CheckMacroParams
  
  See: <ssif_main.h>
*/

r_ssif_Error_t R_SSIF_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_SSIF_MACRO_NUM <= Unit)
    {
        return  R_SSIF_ERR_RANGE; 
    }
    return R_SSIF_ERR_OK;  
}

/*******************************************************************************
  Function: R_SSIF_ErrorHandler
  
  See: <ssif_main.h>
*/

void R_SSIF_ErrorHandler(uint32_t Unit, r_ssif_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        R_SSIF_SYS_ErrorHandler(Unit, Error); /* Error happened call error handler */
    }
}

/*******************************************************************************
  Section: Global API Functions
  
  See: <ssif_api.h>
*/

/*******************************************************************************
  Function: R_SSIF_GetVersionStr

  See: <ssif_api.h> 
*/
const int8_t * R_SSIF_GetVersionStr(void) 
{
    return (const int8_t *) loc_SsifDrvVer;
}

/*******************************************************************************
  Function: R_SSIF_GetMajorVersion

  See: <ssif_api.h> 
*/
const uint16_t R_SSIF_GetMajorVersion(void) 
{
    return (uint16_t)R_SSIF_VERSION_HI; 
}

/*******************************************************************************
  Function: R_SSIF_GetMinorVersion

  See: <ssif_api.h> 
*/
const uint16_t R_SSIF_GetMinorVersion(void) 
{
    return (uint16_t)R_SSIF_VERSION_LO; 
}


/*******************************************************************************
  Function: R_SSIF_Init

  See: <ssif_api.h> 
*/

r_ssif_Error_t R_SSIF_Init(uint32_t Unit) 
{
    r_ssif_Error_t err;
    err = R_SSIF_CheckMacroParams(Unit); 
    if (R_SSIF_ERR_OK != err) 
    {
        return err; 
    }
    if (0 == r_ssif_Dev[Unit].BaseAddr)
    {        
        r_ssif_Dev[Unit].BaseAddr = R_SSIF_SYS_BaseAddr(Unit);
        err = R_SSIF_SYS_HardwareInit(Unit);
    }
    else 
    {   
        /* please, no double-init*/
        err = R_SSIF_ERR_LOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_SSIF_DeInit

  See: <ssif_api.h> 
*/

 r_ssif_Error_t R_SSIF_DeInit(uint32_t Unit)
 {
    r_ssif_Error_t err;

    err = R_SSIF_CheckMacroParams(Unit); 

    if (R_SSIF_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_ssif_Dev[Unit].BaseAddr)
    {
        err = R_SSIF_SYS_HardwareDeInit(Unit);        
        r_ssif_Dev[Unit].BaseAddr = 0;
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_SSIF_ERR_NOTLOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_SSIF_Config
*/

r_ssif_Error_t R_SSIF_Config(uint32_t Unit, r_ssif_Parameter_t *Config) 
{
    r_ssif_Error_t err;
    uint32_t base;
    uint32_t regval32;
    uint32_t clkPerSysword;
    uint32_t audioClk;       /* ACK in Hz, means MCLK @LM49450 */
    uint32_t serialClk;      /* SCK in Hz, means I2C_CLK @LM49450 */
    uint32_t audioPhi;       /* clock ratio @D1M2, not 'over sampling' */
    /* temporary fixed, move to parameters... */
    uint32_t rDivRatio = 2;  /* pre-divider ratio of MCLK to 'point B' @LM49450 */

    err = R_SSIF_CheckMacroParams(Unit); 
    if (R_SSIF_ERR_OK != err) 
    {
        return err; 
    } else
    {
        /* setup audio clock frequency (of internal audio clock) */
        /*  -> need to calcualte clocks per WS cycle (2 syswords in I2S mode)
            -> to calculate audio clock from baudrate */
        switch (Config->SystemWordLength)
        {
            case R_SSIF_SYS_WORD_LENGTH_8BIT:
                clkPerSysword = 8;
                break;
            case R_SSIF_SYS_WORD_LENGTH_16BIT:
                clkPerSysword = 16;
                break;
            case R_SSIF_SYS_WORD_LENGTH_24BIT:
                clkPerSysword = 24;
                break;
            case R_SSIF_SYS_WORD_LENGTH_32BIT:
                clkPerSysword = 32;
                break;
            case R_SSIF_SYS_WORD_LENGTH_48BIT:
            case R_SSIF_SYS_WORD_LENGTH_64BIT:
                clkPerSysword = 64;
                break;
            case R_SSIF_SYS_WORD_LENGTH_128BIT:
                clkPerSysword = 128;
                break;
            case R_SSIF_SYS_WORD_LENGTH_256BIT:
                clkPerSysword = 256;
                break;
            default:
                clkPerSysword = 48;
                break;
        }

        /* serial clock = Baudrate * Bit per SysWord * 2, 
           because of two SysWords per period */
        serialClk = 2 * (Config->Baudrate * clkPerSysword);
        audioClk  = rDivRatio * 2 * (Config->Baudrate * Config->OverSampleRate);
        R_SSIF_SYS_SetClock(Unit, audioClk);
        
        /* Select internal audio clock
            -> bit 2 PRMR0ISSA set 0: SSIACKINT input.
           Select to Continue audio clock and serial bit clock during SVSTOP of debugger.
            -> bit 1 PRMR0ISMA set 0: Continue AUDIO clock.
            -> bit 0 PRMR0ISMS set 0: Continue SSISCK clock.
        */
        regval32 = 0x00000000;
        R_SSIF_WRITE_REG(32, R_SSIF_REG_FCLKCFG, regval32);
        
        base = r_ssif_Dev[Unit].BaseAddr;
        regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_CR);
        regval32 &= ~0x00FFFFFFu; /* reset all bits we will touch first */

        regval32 |= (uint32_t)((uint32_t)(Config->Channels) << 22u);
        regval32 |= (uint32_t)((uint32_t)(Config->DataWordLength)   << 19u);
        regval32 |= (uint32_t)((uint32_t)(Config->SystemWordLength) << 16u);

        /* SCK direction */
        if (Config->SckDirection == R_SSIF_SCK_DIR_OUT)
        {
            regval32 |= (uint32_t)(1uL << 15u);
        } 
        else
        {
            regval32 &= (uint32_t)(~(0uL << 15u));
        }
        
        /* WS direction */
        if (Config->WsDirection == R_SSIF_WS_DIR_OUT)
        {
            regval32 |= (uint32_t)(1uL << 14u);
        } 
        else
        {
            regval32 &= (uint32_t)(~(0uL  << 14u));
        }
        
        /* SCK polarity */
        if (Config->SckPolarity == R_SSIF_SCK_POL_RISING)
        {
            regval32 |= (uint32_t)(1uL  << 13u);
        } 
        else
        {
            regval32 &= (uint32_t)(~(0uL  << 13u));
        }

        /* WS polarity */
        if ( (Config->WsPolarity == R_SSIF_WS_POL_FIRST_HIGH)
            && (Config->Mode == R_SSIF_MODE_IIS) )
        {
            regval32 |= (uint32_t)(1uL  << 12);
        } 
        else
        {
            regval32 &= (uint32_t)(~(0uL  << 12u));
        }
        
        /* padding polarity fixed to low */
        regval32 &= (uint32_t)(~(0uL  << 11u));

        if (0 != ((uint32_t)Config->DataAlignment & (uint32_t)R_SSIF_SALIGN_PADDING_FIRST))
        {
            regval32 |= (uint32_t)(1uL  << 10u);
        } 
        else
        {
            regval32 &= (uint32_t)(~(0uL  << 10u));
        }

        if ( (0 != ((uint32_t)Config->DataAlignment & (uint32_t)R_SSIF_PALIGN_18TO24_ALIGN_RIGHT))
            || (0 != ((uint32_t)Config->DataAlignment & (uint32_t)R_SSIF_PALIGN_8OR16_ALIGN_LOWBYTE_LAST)) )
        {
            regval32 |= (uint32_t)(1uL  << 9u);
        } 
        else
        {
            regval32 &= (uint32_t)(~(0uL  << 9u));
        }

        if (Config->Mode == R_SSIF_MODE_IIS)
        {
            /* Use for IIS mode:
                Serial Data Delay
                0: 1 clock cycle delay between SSIWS and SSIDATA */
            regval32 &= (uint32_t)(~(0uL  << 8u));
        }
        if (Config->Mode == R_SSIF_MODE_TDM)
        {
            /* Use for TDM mode:
                Serial Data Delay
                1: No delay between SSIWS and SSIDATA */
            regval32 |= (uint32_t)(1uL  << 8u);
        }
        
        /* Set the divider for the Serial Oversampling Clock ratio */
        audioPhi  = audioClk / serialClk;
        regval32 |= (uint32_t)(locGetSerialClockOversamplingRatioRegVal(Unit, audioPhi) << 4);
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_CR, regval32);


        regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_TDMR);
        regval32 &= ~0x00020101uL ; /* reset all bits we will touch first */        
        /* Receive Direct Data Mute Setting
           bit 17 RXDMUTE always set 0: The receive data is output without change. */

        /* WS Continue Mode
           bit 8 CONT always set 1: Enables WS continue mode. */
        regval32 |= (uint32_t)(1uL  << 8u);
        
        /* TDM mode selection */
        regval32 |= (uint32_t)(Config->Mode);
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_TDMR, regval32);

        
        /* disables FC (frequency change detection) feature at all */
        regval32 = 0x00000000;
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_FCCR, regval32);
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_FCMR, regval32);
        
        /* FIFO Control Register (SSIFCR) - write default value */
        regval32 = Config->FifoCtrl;
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_FCR, regval32);
    }
    return err;
}


/*******************************************************************************
  Function: R_SSIF_TransferData
*/

uint32_t R_SSIF_TransferData(uint32_t Unit, uint32_t Data)
{
    r_ssif_Error_t err;
    uint32_t base;
    uint32_t regval32;
    err = R_SSIF_CheckMacroParams(Unit); 
    if (R_SSIF_ERR_OK != err) 
    {
        return err; 
    } else
    {
        base = r_ssif_Dev[Unit].BaseAddr;
        do
        {   /* Wait until Transmit Data Empty (TDE) bit in
                FIFO Status Register (SSIFSR) is set. */
            regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_FSR);

        /* wait for about half bautrate's time */
        R_TICK_WaitUS(0u, 20u);


        /* condition: until TDE = 1, means TX FIFO is empty */
        } while(0x0 == (regval32 | (1uL  << 16u)));

        /* Write the data (into the Tx FIFO) */
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_FTDR, Data);
    }
    return err;
}


/*******************************************************************************
  Function: R_SSIF_TransferEnable
*/

uint32_t R_SSIF_TransferEnable(uint32_t Unit, r_ssif_TxE_t TxEnable)
{
    r_ssif_Error_t err;
    uint32_t base;
    uint32_t regval32;
    err = R_SSIF_CheckMacroParams(Unit); 
    if (R_SSIF_ERR_OK != err) 
    {
        return err; 
    } 
    else
    {
        base = r_ssif_Dev[Unit].BaseAddr;
        regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_CR);
        regval32 &= (uint32_t)~0x00000002uL; /* reset all bits we will touch first */
        regval32 |= (uint32_t)((uint32_t)(TxEnable) << 1u);
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_CR, regval32);
    }
    return err;
}


/*******************************************************************************
  Function: R_SSIF_ReceiveEnable
*/

uint32_t R_SSIF_ReceiveEnable(uint32_t Unit, r_ssif_RxE_t RxEnable)
{
    r_ssif_Error_t err;
    uint32_t base;
    uint32_t regval32;
    err = R_SSIF_CheckMacroParams(Unit); 
    if (R_SSIF_ERR_OK != err) 
    {
        return err; 
    } else
    {
        base = r_ssif_Dev[Unit].BaseAddr;
        regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_CR);
        regval32 &= (uint32_t)~0x00000001uL; /* reset all bits we will touch first */
        regval32 |= (uint32_t)RxEnable;
        R_SSIF_WRITE_REG(32, base + R_SSIF_REG_CR, regval32);
    }
    return err;
}


/*******************************************************************************
  Function: R_SSIF_GetClock
*/

uint32_t R_SSIF_GetClock(uint32_t Unit)
{
    uint32_t val;                                 
                                                                                       
    if(Unit == 0)
    {
        val = R_SSIF_SYS_GetClock(Unit);
    }
    else
    {
        val = 0;
    }
    return val;    
}


/*******************************************************************************
  Function: R_SSIF_SetErrorCallback
*/

void R_SSIF_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}

