/*
****************************************************************************
PROJECT : PCMP driver
FILE ID : $Id: r_pcmp_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for PCMP macro
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


/***********************************************************
  Title: Hi Speed (PCM) PWM  API

  outputs PCM audio samples as PWM to drive speakers.

  Implementation of the API and other global functions.
*/


/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_config_pcmp.h"
#include "r_pcmp_api.h"
#include "r_pcmp_sys.h"
#define PCMP_MAIN_GLOBALS_
#include "r_pcmp_main.h"
#include "r_pcmp_regs.h"


/***********************************************************
  Section: Local Defines
*/


/***********************************************************
  Section: Local Functions
*/
r_pcmp_Error_t R_PCMP_SetActiveLevel(uint32_t Unit, r_pcmp_Parameters_t *Config); 


/***********************************************************
  Variable: _UserErrorHandler

  A function pointer to a user callback (Applications error handler)

  The value is assigned by the application using the driver
*/

static void (*_UserErrorHandler )(uint32_t Unit, uint32_t Error) = R_NULL;



void R_PCMP_ErrorHandler(uint32_t Unit, r_pcmp_Error_t Error)
{
    if (_UserErrorHandler != R_NULL) 
    {      /* we've got a registered error handler here ...*/
        _UserErrorHandler(Unit, (uint32_t)Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for(;;) 
        {                          /* we don't have a handler */
        }                                    /* so, let's wait here */
    }
}


/***********************************************************
  Function: R_PCMP_Init
*/

r_pcmp_Error_t R_PCMP_Init(uint32_t Unit, r_pcmp_Parameters_t  *Config)
{
    R_PCMP_RegCkSelBit_t Pcm_RegCkSel;
    R_PCMP_RegCtlBit_t   Pcm_RegCtl;
 
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    /* update the base address */
    R_PCMP_Dev[Unit].BaseAddr = R_SYS_PCMP_GetBaseAddr(Unit);

    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address    = R_PCMP_Dev[Unit].BaseAddr;
        
        Pcm_RegCtl.Word = R_PCMP_READ_REG(32, (base_address + R_PCMP_PCMP0CTL));

        /* clear the flags incase they are set */
        R_PCMP_ClearBufferFlag(Unit, R_PCMP_BUFFER_OVERFLOW);
        R_PCMP_ClearBufferFlag(Unit, R_PCMP_BUFFER_UNDERRUN);
        
        /* update clock divider */
        offset_address = R_PCMP_PCMP0CKSEL;
        
        /* update the local structure */
        Pcm_RegCkSel.Bit.CkSel = Config->ClockDiv;

        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + offset_address, Pcm_RegCkSel.Word);
        
        /* update the local structure */               
        Pcm_RegCtl.Bit.Dom = Config->DoubleMode;
        Pcm_RegCtl.Bit.Stereo = Config->StereoMode;
        Pcm_RegCtl.Bit.Mode = Config->OperationMode;

        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + R_PCMP_PCMP0CTL, Pcm_RegCtl.Word);

        /* update the level for the static output */       
        R_PCMP_WRITE_REG(32, base_address + R_PCMP_PCMP0STLV, Config->PinMaskOutputs);

        /* update the inverted level for the static output */       
        offset_address = R_PCMP_PCMP0INV;
        R_PCMP_WRITE_REG(32, base_address + offset_address, Config->PinMaskInverted);

        /* update the enable for the static output */       
        offset_address = R_PCMP_PCMP0STEN;
        R_PCMP_WRITE_REG(32, base_address + offset_address, Config->PinMaskStaticLevelActive);
    }
    return error_mode;
}

/***********************************************************
  Function: R_PCMP_SetActiveLevel
*/

r_pcmp_Error_t R_PCMP_SetActiveLevel(
    uint32_t Unit, 
    r_pcmp_Parameters_t  *Config)
{
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    /* update the base address */
    R_PCMP_Dev[Unit].BaseAddr = R_SYS_PCMP_GetBaseAddr(Unit);

    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* first disable the macro */
  
        /* update the level for the static output */       
        offset_address = R_PCMP_PCMP0STLV;
        R_PCMP_WRITE_REG(32, base_address + offset_address, Config->PinMaskOutputs);

        /* update the inverted level for the static output */       
        offset_address = R_PCMP_PCMP0INV;
        R_PCMP_WRITE_REG(32, base_address + offset_address, Config->PinMaskInverted);

        /* update the enable for the static output */       
        offset_address = R_PCMP_PCMP0STEN;
        R_PCMP_WRITE_REG(32, base_address + offset_address, Config->PinMaskStaticLevelActive);
    }

    return error_mode;
}


/***********************************************************
  Function: R_PCMP_Enable
*/

r_pcmp_Error_t R_PCMP_SetSvstop(uint32_t Unit, uint32_t Enable)
{
    uint32_t base_address = 0;
    uint32_t value        = 0;
    
    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
       return R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        if(Enable == R_TRUE)
        {
            value = 0x00;
        }
        else
        {
            value = 0x80;
        }
        
        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + R_PCMP_PCMP0EMU, value);
    }

    return R_PCMP_ERR_OK;
}


/***********************************************************
  Function: R_PCMP_Enable
*/

r_pcmp_Error_t R_PCMP_Enable(uint32_t Unit, uint32_t Enable)
{
    R_PCMP_RegCtlBit_t   Pcm_RegCtl;
    
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    
    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update the register */
        offset_address = R_PCMP_PCMP0CTL;
        
        /* get the current value */
        Pcm_RegCtl.Word = R_PCMP_READ_REG(32, (base_address + offset_address));
        
        if(Enable == R_TRUE)
        {
            Pcm_RegCtl.Bit.Enab = 1;
        }
        else
        {
            Pcm_RegCtl.Bit.Enab = 0;
        }
        
        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + offset_address, Pcm_RegCtl.Word);
    }

    return error_mode;
}


/***********************************************************
  Function: R_PCMP_GetBufferCount
*/

r_pcmp_Error_t R_PCMP_GetBufferCount(uint32_t Unit, uint8_t *SampleCnt)
{
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    R_PCMP_RegStRBit_t Pcm_RegStRBit;
    
       /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update the register */
        offset_address = R_PCMP_PCMP0STR;
        
        /* get the current value */     
        Pcm_RegStRBit.Word = R_PCMP_READ_REG(32, (base_address + offset_address));
        
        /* No. of samples in the FIFO*/
        *SampleCnt = Pcm_RegStRBit.Bit.Fifo;
    }

    return error_mode;
}


/***********************************************************
  Function: R_PCMP_GetBufferFlag
*/

uint32_t R_PCMP_GetBufferFlag(uint32_t Unit, r_pcmp_BufferFlag_t  Flag)
{
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    R_PCMP_RegStRBit_t Pcm_RegStRBit;
    uint32_t retval = R_FALSE;

       /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update the register */
        offset_address = R_PCMP_PCMP0STR;
        
        /* get the current value */     
        Pcm_RegStRBit.Word = R_PCMP_READ_REG(32, (base_address + offset_address));
        
        if(Flag == R_PCMP_BUFFER_UNDERRUN)
        {
            if(Pcm_RegStRBit.Bit.Ur == 1)
            {
                retval = R_TRUE;
            }
            else
            {
                retval = R_FALSE;
            }           
        }
        else if(Flag == R_PCMP_BUFFER_OVERFLOW)
        {
            if(Pcm_RegStRBit.Bit.Ov == 1)
            {
                retval = R_TRUE;
            }
            else
            {
                retval = R_FALSE;
            }           
        }
        else
        {
            /* error */
            error_mode = R_PCMP_ERR_RANGE;
            error_mode = error_mode;
        }
    }

    return retval;
}


/***********************************************************
  Function: R_PCMP_ClearBufferFlag
*/

r_pcmp_Error_t R_PCMP_ClearBufferFlag(uint32_t Unit, r_pcmp_BufferFlag_t Flag)
{
    
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    R_PCMP_RegStCBit_t Pcm_RegStCBit;

       /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update the register */
        offset_address = R_PCMP_PCMP0STC;
        
        Pcm_RegStCBit.Word = 0;
        
        if(Flag == R_PCMP_BUFFER_UNDERRUN)
        {
            Pcm_RegStCBit.Bit.Ur = 1;
        }
        else if(Flag == R_PCMP_BUFFER_OVERFLOW)
        {
            Pcm_RegStCBit.Bit.Ov = 1;
        }
        else
        {
            /* error */
            error_mode = R_PCMP_ERR_RANGE;
        }

        R_PCMP_WRITE_REG(32, base_address + offset_address, Pcm_RegStCBit.Word);
    }
    
    return error_mode;
}


/***********************************************************
  Function: R_PCMP_SetOffset
*/

r_pcmp_Error_t R_PCMP_SetOffset(uint32_t Unit, uint16_t Offset)
{
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    
    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update the  */
        offset_address = R_PCMP_PCMP0OFFS;

        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + offset_address, Offset);
    }
    
    return error_mode;
}

/***********************************************************
  Function: R_PCMP_WriteSamples
*/

r_pcmp_Error_t R_PCMP_WriteSamples(uint32_t Unit, uint16_t Sample1, uint16_t Sample2)
{
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t Sample = 0;
    
       /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } else {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
       /* Sample 1 is the left and sample 2 is the right */
        Sample = ((uint32_t)Sample1 << 16u) | Sample2;


       /*In order to sign extend the nagative values*/
       if(0 != (Sample1 & 0x80u))
       {
            Sample |= 0xff000000u;    
       }
        if(0 != (Sample2 & 0x80u))
        {
            Sample |= 0xff00u;            
        } 
    
        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + R_PCMP_PCMP0AUSA, Sample);
    }
    
    return error_mode;
}


/***********************************************************
  Function: R_PCMP_Write2Buffer
*/

r_pcmp_Error_t R_PCMP_Write2Buffer(
    uint32_t Unit, uint16_t Sample1, uint16_t Sample2,
    r_pcmp_Format_t DataInfo)
{
    uint32_t base_address = 0;
    uint32_t sample = 0;
    int32_t  i;
    uint32_t smpl1;
    uint32_t smpl2;
    uint16_t mask;
    
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;

    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } 
    else 
    {    
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        mask = 0; 
        if (DataInfo.Format == R_PCMP_FORMAT_2KOMP) 
        {    
            /* Sample 1 is the left and sample 2 is the right */
            sample = ((uint32_t)Sample1 << 16u) | Sample2;
                    
        } 
        else 
        {
            
            for (i = 0; i <= DataInfo.BitLength; i++)
            {
                mask |= (uint16_t)(0x1ul << (16u - i));
            }
            
            /* convert wav-pcm data to 2-complement */
            
            /* cast to 32 Bit and ensure only 16 Bits are valid*/
            smpl1 = ((uint32_t)Sample1 & 0xffffu);
            if ( 0 != (smpl1 & (0x1ul << (16u - DataInfo.BitLength))))
            {
                smpl1 &= ~((0x1ul << (16u - DataInfo.BitLength-1)));
            } 
            else 
            {
                smpl1 |= ((0x1ul << (16u - DataInfo.BitLength-1)));                
            }
 
            /*eliminate former sign extention*/
            smpl1 &= ~(uint32_t)mask;
          
            /* Add 2-comp sign extention*/
            if( 0 != (smpl1 & (0x1ul << (16u - DataInfo.BitLength - 1u))))
            {
                smpl1 |= (uint32_t)mask;                
            }
            
            smpl2 = (Sample2 ) & 0xffffu;
 
            if (0 != (smpl2 & (0x1ul << (16u - DataInfo.BitLength))))
            {
                smpl2 &= ~(0x1ul << ((16u - DataInfo.BitLength-1u)));
            } 
            else 
            {
                smpl2 |= (0x1ul << ((16u - DataInfo.BitLength-1u)));                
            } 
            smpl2 &= ~(uint32_t)mask; 
                                   
            if(0 != (smpl2 & (0x1ul << (16u - DataInfo.BitLength - 1u))))
            {
                
                smpl2 |= (uint32_t)mask;        
            }  
            
            sample = smpl1 | (smpl2 << 16u);
        }    
        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + R_PCMP_PCMP0AUSA, sample);
    }
    return error_mode;
}

/***********************************************************
  Function: R_PCMP_SetClockDivider
*/

r_pcmp_Error_t R_PCMP_SetClockDivider(uint32_t Unit, r_pcmp_ClockDivider_t ClockDiv)
{
    R_PCMP_RegCkSelBit_t Pcm_RegCkSelBit;
     
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    /* update the base address */
    R_PCMP_Dev[Unit].BaseAddr = R_SYS_PCMP_GetBaseAddr(Unit);

    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } 
    else 
    {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update clock divider */
        offset_address = R_PCMP_PCMP0CKSEL;
        
        /* update the local structure */
        Pcm_RegCkSelBit.Bit.CkSel = ClockDiv;

        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + offset_address, Pcm_RegCkSelBit.Word);
    }

    return error_mode;
}


/***********************************************************
  Function: R_PCMP_SetPwmPeriod
*/

r_pcmp_Error_t R_PCMP_SetPwmPeriod(uint32_t Unit, uint16_t Period)
{
    r_pcmp_Error_t error_mode = R_PCMP_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    
    /* check the instance is allowed */
    if ( Unit > (R_PCMP_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_PCMP_ERR_INSTANCE;
    } 
    else 
    {
        /* get the base address for the selected unit */ 
        base_address = R_PCMP_Dev[Unit].BaseAddr;
        
        /* update the  */
        offset_address = R_PCMP_PCMP0TPWM;

        /* write to the register */
        R_PCMP_WRITE_REG(32, base_address + offset_address, Period);
    }
    
    return error_mode;
}


/***********************************************************
  Function: R_PCMP_SetErrorCallback
*/

void R_PCMP_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error))
{
    _UserErrorHandler = ErrorCallback;
}


/***********************************************************
  Function: R_PCMP_GetClock
*/

uint32_t R_PCMP_GetClock(uint32_t Unit)
{
    return R_SYS_PCMP_GetClock(Unit);
}

