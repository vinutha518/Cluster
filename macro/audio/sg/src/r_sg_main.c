/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_sg_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for the SG macro
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
  Title: SG Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_sg.h"
#include "r_sg_regs.h"
#include "r_sg_api.h"
#define SG_MAIN_GLOBALS_
#include "r_sg_main.h"
#include "r_sg_sys.h"



/***********************************************************
  Section: Local Defines
*/

/***********************************************************
  Section: Local Variables
*/

/***********************************************************
  Variable: _UserErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*_UserErrorHandler )(uint32_t Unit, uint32_t Error) = R_NULL;

/***********************************************************
  Section: Local Functions
*/


/***********************************************************
  Section: Global API Functions
  
  Comments see sg_api.h
*/


/***********************************************************
  Function: R_SG_ErrorHandler
  
  Comment see sg_main.h
*/

void R_SG_ErrorHandler(uint32_t Unit, r_sg_Error_t Error) {
    
    if (_UserErrorHandler != R_NULL) 
    {      /* we've got a registered error handler here ...*/
        _UserErrorHandler(Unit, (uint32_t)Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for (;;)
        {}                           /* we don't have a handler */
                                            /* so, let's wait here */
    }
}



/***********************************************************
  Function: R_SG_Enable
*/

r_sg_Error_t R_SG_Enable(
    uint32_t   Unit, 
    uint32_t   Enable) 
{
    uint32_t        base;
    R_SG_RegCtl_t   RegCtl;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegCtl.Word = R_SG_READ_REG(32, base + R_SG_SGCTL);
        if (Enable != 0) {
            RegCtl.Bit.En = 1; 
        }
        else {
            RegCtl.Bit.En = 0; 
        }
        R_SG_WRITE_REG(32, base + R_SG_SGCTL, RegCtl.Word);  
    }
    return R_SG_ERR_OK;
}    

/***********************************************************
  Function: R_SG_SetClockDivider
*/

r_sg_Error_t R_SG_SetClockDivider(
    uint32_t Unit, 
    uint8_t  ClkDiv)
{
    uint32_t        base;
    R_SG_RegCtl_t   RegCtl;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        if ((ClkDiv < 1) || (ClkDiv > 8))
        {
            return R_SG_ERR_RANGE;
        }
        base = R_SG_Dev[Unit].BaseAddr;
        RegCtl.Word = R_SG_READ_REG(32, (base + R_SG_SGCTL));
        RegCtl.Bit.ClkDiv = ClkDiv - 1; 
        R_SG_WRITE_REG(32, base + R_SG_SGCTL, RegCtl.Word);  
        return R_SG_ERR_OK;
    }
}




/***********************************************************
  Function: R_SG_IsRunning
*/

uint32_t R_SG_IsRunning(uint32_t Unit) 
{
    uint32_t        base;
    R_SG_RegStat_t  RegStat;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        R_SG_ErrorHandler(Unit, R_SG_ERR_INSTANCE);
        return 0; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegStat.Word = R_SG_READ_REG(32, base + R_SG_SGSTAT);
        if (0 != RegStat.Bit.Run) 
        {
            return 1; 
        }
        else 
        {
            return 0; 
        }
    }
}

/***********************************************************
  Function: R_SG_SetMode
*/

r_sg_Error_t R_SG_SetMode
(
    uint32_t    Unit, 
    r_sg_Mode_t Mode)
{
    uint32_t        base;
    R_SG_RegConf_t  RegConf;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        if (Mode > R_SG_MODE_ALD)
        {
            return R_SG_ERR_RANGE;
        }
        base = R_SG_Dev[Unit].BaseAddr;
        RegConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        RegConf.Bit.Mode = Mode; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, RegConf.Word);  
    }
    return R_SG_ERR_OK;
}

/***********************************************************
  Function: R_SG_SelectSoundOutput
*/

r_sg_Error_t R_SG_SelectSoundOutput(uint32_t Unit, 
              r_sg_SoundOutSelection_t    SoundOut)
{
    uint32_t        base;
    R_SG_RegConf_t  RegConf;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        RegConf.Bit.Os = SoundOut; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, RegConf.Word);  
    }
    return R_SG_ERR_OK;
}

/***********************************************************
  Function: R_SG_SetInvOutput
*/

r_sg_Error_t R_SG_SetInvOutput(uint32_t Unit, 
    r_sg_InvOutSelection_t  InvOut)
{
    uint32_t        base;
    R_SG_RegConf_t  RegConf;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        RegConf.Bit.Si = InvOut; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, RegConf.Word);  
    }
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_SetBuffering
*/

r_sg_Error_t R_SG_SetBuffering(
    uint32_t                Unit, 
    r_sg_RegBuffering_t     RegBuffering)
{
    uint32_t        base;
    R_SG_RegConf_t  RegConf;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        RegConf.Bit.Be = RegBuffering; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, RegConf.Word);  
    }
    return R_SG_ERR_OK;
}

/***********************************************************
  Function: R_SG_SetPWMFrequency
*/

r_sg_Error_t R_SG_SetPWMFrequency(
    uint32_t    Unit, 
    uint16_t    PWMFrequency)
{
    uint32_t        base;
    uint32_t        divider;
    uint16_t        fl;
    R_SG_RegCtl_t   RegCtl;
    R_SG_RegFl_t    RegFl;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        if ((PWMFrequency < R_SG_MIN_PWM_FREQ) || (PWMFrequency > R_SG_MAX_PWM_FREQ))
        {
            return R_SG_ERR_RANGE;
        }
        base = R_SG_Dev[Unit].BaseAddr;
        RegCtl.Word = R_SG_READ_REG(32, base + R_SG_SGCTL);
        
        divider = 1;
        do 
        {
            fl = R_SG_Dev[Unit].Pclk / (PWMFrequency * divider);
            if (fl > 0x1ffu) 
            {
                divider++;
            }    
        }while(fl > 0x1ffu );
        
        RegCtl.Bit.ClkDiv = divider - 1;
        
        R_SG_Dev[Unit].Divider = divider;
        
        R_SG_WRITE_REG(32, base + R_SG_SGCTL, RegCtl.Word);       

        R_SG_Dev[Unit].Fl = fl;
        
        RegFl.Word = fl - 1;
        R_SG_Dev[Unit].PWMFreq = (R_SG_Dev[Unit].Pclk ) / (fl * divider);
        
        R_SG_WRITE_REG(32, base + R_SG_SGFL, RegFl.Word);  
    }
    return R_SG_ERR_OK;
}    


/***********************************************************
  Function: R_SG_SetOptimalFreqs
*/

r_sg_Error_t R_SG_SetOptimalFreqs(uint32_t Unit, uint16_t OutputFrequency)
{
    
    uint32_t        base;
    uint32_t        divider;
    uint32_t        tmp_pwm;
    uint16_t        tmp_fl;
    uint16_t        tmp_fh;
    uint32_t        sgClk;
#ifdef CODE_FINALIZED 
    uint32_t        tmp_tone;
    uint16_t        tone_dif;
    uint16_t        fl;
    uint16_t        tmp_tone_dif;
#endif 
    R_SG_RegCtl_t   RegCtl;
    R_SG_RegFl_t    RegFl;
    R_SG_RegFh_t    RegFh;    

        base = R_SG_Dev[0].BaseAddr;
        RegCtl.Word = R_SG_READ_REG(32, base + R_SG_SGCTL);
        divider = RegCtl.Bit.ClkDiv + 1;

        divider = 1;
        if(0 == R_SG_Dev[0].Pclk)
        {
            R_SG_Dev[0].Pclk = R_SYS_SG_GetClock(Unit);
        }
                
        sgClk = R_SG_Dev[0].Pclk / divider;
        
        while(sgClk > 40000000)
        {
            divider++;
            sgClk = R_SG_Dev[0].Pclk / divider;
        }
        
        if(0 == R_SG_Dev[0].PWMFreq)
        {
            R_SG_Dev[0].PWMFreq = 32000;
        }
        
        tmp_fl = sgClk / R_SG_Dev[0].PWMFreq;
        
        while (tmp_fl > 0x1FF ) 
        {
                divider++;
                sgClk = R_SG_Dev[0].Pclk / divider;
                tmp_fl = sgClk / R_SG_Dev[0].PWMFreq;
        }
        
        if (divider > 8)
        {
            return R_SG_ERR_RANGE;
        }
            
        R_SG_Dev[0].Divider = divider;
        tmp_pwm = sgClk / tmp_fl;
        tmp_fh = tmp_pwm / (OutputFrequency * 2);
      
        
        /* $$$$ following procedure to get optimal counter value for sound Freq - to be improved  */
        
#ifdef CODE_FINALIZED 
        tmp_tone = (tmp_pwm / tmp_fh) / 2;
        tone_dif = tmp_tone - OutputFrequency;
        tmp_tone_dif = tone_dif;  
        tmp_tone_dif = 0;
            
        while (tone_dif > 3)
        {
            
            if(tmp_tone_dif >= tone_dif)
            {
                tmp_fl++;
            } 
            else 
            {
                tmp_fl--;
            }
            
            tmp_tone_dif = tone_dif;            
            
            tmp_pwm  = fclk / tmp_fl;
            tmp_fh   = tmp_pwm / (tmp_tone * 2);
            tmp_tone = (tmp_pwm / tmp_fh) / 2;
            tone_dif = tmp_tone - OutputFrequency;        
            
        }
#endif 
        RegCtl.Bit.ClkDiv = divider - 1;

        R_SG_WRITE_REG(32, base + R_SG_SGCTL, RegCtl.Word);       
                          
        RegFl.Word = 0;
        RegFh.Word = 0;
        
        RegFl.Bit.Fl = tmp_fl - 1;
        RegFh.Bit.Fh = tmp_fh - 1;
       
        R_SG_Dev[0].PWMFreq = (R_SG_Dev[0].Pclk ) / (tmp_fl * divider);
        R_SG_Dev[0].Fl = tmp_fl;
        R_SG_Dev[0].Fh = tmp_fh;
        R_SG_Dev[0].ToneFreq = R_SG_Dev[0].PWMFreq / (R_SG_Dev[0].Fh *2 );
        R_SG_Dev[0].Divider = divider; 
        R_SG_WRITE_REG(32, base + R_SG_SGFL, RegFl.Word);  
        R_SG_WRITE_REG(32, base + R_SG_SGFH, RegFh.Word); 
        
    return R_SG_ERR_OK;
}
   
/***********************************************************
  Function: R_SG_SetToneFrequency
*/
                    
r_sg_Error_t R_SG_SetToneFrequency(
    uint32_t    Unit, 
    uint16_t    ToneFrequency)
{
    uint32_t        base;
    uint32_t        divider;
    R_SG_RegCtl_t   RegCtl;
    R_SG_RegFl_t    RegFl;
    R_SG_RegFh_t    RegFh;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        if ((ToneFrequency < 100) || (ToneFrequency > 6000))
        {
            return R_SG_ERR_RANGE;
        }
        base = R_SG_Dev[Unit].BaseAddr;
        RegFl.Word = R_SG_READ_REG(32, base + R_SG_SGFL);
        if(0 == RegFl.Word)
        {
            R_SG_SetOptimalFreqs(Unit, ToneFrequency);
        } 
        else 
        {
            RegCtl.Word = R_SG_READ_REG(32, base + R_SG_SGCTL);
            divider = RegCtl.Bit.ClkDiv + 1;
            divider = divider * (RegFl.Bit.Fl + 1);
            divider = divider * 2 * ToneFrequency;
            RegFh.Word = ( R_SYS_SG_GetClock(Unit) + (divider / 2 )) / (divider - 1);
            R_SG_WRITE_REG(32, base + R_SG_SGFH, RegFh.Word);
        }
    }
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_GetMaximalPwm
*/

uint16_t R_SG_GetMaximalPwm(
    uint32_t     Unit)
{
    uint32_t        base;
    R_SG_RegFl_t    RegFl;

    base = R_SG_Dev[Unit].BaseAddr;
    RegFl.Word = R_SG_READ_REG(32, base + R_SG_SGFL);
    return RegFl.Bit.Fl + 1;
}


/***********************************************************
  Function: R_SG_SetPWM
*/
r_sg_Error_t R_SG_SetPWM(
    uint32_t    Unit, 
    uint16_t    Pwm)
{
    uint32_t        base;
    R_SG_RegPwm_t   regPwm;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        regPwm.Word = R_SG_READ_REG(32, base + R_SG_SGPWM);
        regPwm.Bit.Pwm = Pwm;
        R_SG_WRITE_REG(32, base + R_SG_SGPWM, regPwm.Word);  
    }
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_SetVolume
*/

r_sg_Error_t R_SG_SetVolume(
    uint32_t    Unit, 
    uint8_t     Percent)
{
    uint32_t        base;
    R_SG_RegFl_t    RegFl;
    R_SG_RegPwm_t   RegPwm;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegFl.Word = R_SG_READ_REG(32, base + R_SG_SGFL);
        
        RegPwm.Bit.Pwm = ((RegFl.Bit.Fl + 1) * Percent) / 100;
        R_SG_WRITE_REG(32, base + R_SG_SGPWM, RegPwm.Bit.Pwm);  
    }
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_SetDuration
*/

r_sg_Error_t R_SG_SetDuration(uint32_t Unit, uint32_t Dfactor, int16_t Stepsize)
{
    uint32_t        base;
    uint32_t        maxPwmVal;
    R_SG_RegFl_t    regFl;
    R_SG_RegAdi_t   regAdi;
    R_SG_RegDf_t    regDf;

    int16_t         tmp;  
    uint16_t        tmp_abs;  
                 
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {

        base       = R_SG_Dev[Unit].BaseAddr;
        regFl.Word = R_SG_READ_REG(32, base + R_SG_SGFL);
        maxPwmVal  = regFl.Bit.Fl;
        
        /* Set ADI   */        
        regAdi.Word = R_SG_READ_REG(32, base + R_SG_SGADI);
             
        if(Stepsize < 0)
        {
        
            tmp = ((uint16_t)(~(uint16_t)Stepsize) + 1u);
            tmp_abs = ((uint16_t)tmp * maxPwmVal) / 100;
            tmp = (uint16_t)~(tmp_abs - 1u);
            
            regAdi.Bit.Adi = ((uint16_t)~(uint16_t)tmp + 1u) | 0x100u; 
       
       } 
       else 
       {
            regAdi.Bit.Adi = (maxPwmVal * Stepsize) / 100; 
       }  
     
    /* Set DF   */
        R_SG_WRITE_REG(32, base + R_SG_SGADI, regAdi.Word); 
       
        regDf.Word = R_SG_READ_REG(32, base + R_SG_SGDF);
        
        /* Duration factor 0 means divided / 1 ! */
        regDf.Bit.Df = Dfactor - 1;
        R_SG_WRITE_REG(32, base + R_SG_SGDF, regDf.Word);  
        
    }
    return R_SG_ERR_OK;
}

/***********************************************************
  Function: R_SG_SetVolume
*/

uint16_t R_SG_GetToneFreq(
    uint32_t         Unit)
{
    uint16_t        tone;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        tone = R_SG_Dev[Unit].ToneFreq;
    }
    return tone;
}

/***********************************************************
  Function: R_SG_SetInterruptThreshold
*/

uint16_t R_SG_SetIntLevel(
    uint32_t    Unit, 
    uint8_t     Level)
{
    
    uint32_t        base;
    R_SG_RegIth_t   regIth;
    uint16_t        maxPwm;
    uint16_t        threshold;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return 0; 
    } 
    else 
    {
        base           = R_SG_Dev[Unit].BaseAddr;
        regIth.Word    = R_SG_READ_REG(32, base + R_SG_SGITH);
        maxPwm         = R_SG_GetMaximalPwm(Unit);
        threshold      = (maxPwm * Level)/ 100; 
        regIth.Bit.Ith = threshold;
        R_SG_WRITE_REG(32, base + R_SG_SGITH, regIth.Word);  
    }
    return threshold;
} 


/***********************************************************
  Function: R_SG_SetInterruptThreshold
*/

r_sg_Error_t R_SG_SetInterruptThreshold(
    uint32_t    Unit, 
    uint16_t    Threshold)
{
    
    uint32_t        base;
    R_SG_RegIth_t   regIth;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        regIth.Word = R_SG_READ_REG(32, base + R_SG_SGITH);
        regIth.Bit.Ith = Threshold;
        R_SG_WRITE_REG(32, base + R_SG_SGITH, regIth.Word);  
    }
    return R_SG_ERR_OK;
}     


/***********************************************************
  Function: R_SG_Init
*/

r_sg_Error_t R_SG_Init(
    uint32_t            Unit, 
    r_sg_Parameters_t   *Config) 
{
      
    R_SG_Dev[Unit].BaseAddr = R_SYS_SG_GetBaseAddr(Unit);
    R_SG_Dev[Unit].Pclk     = R_SYS_SG_GetClock(Unit);
    R_SG_Enable(0, Config->Enabled);
    R_SG_SetMode(Unit, Config->Mode);
    R_SG_SelectSoundOutput(Unit, Config->SoundOut);
    R_SG_SetInvOutput(Unit, Config->InvOut);
    R_SG_SetBuffering(Unit, Config->Buffering);
    R_SG_BreakEnable(Unit, Config->EnableBreak);
    R_SYS_SG_IntCInit(Unit);
    
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_PlaySound
*/

r_sg_Error_t R_SG_PlaySound(
    uint32_t    Unit, 
    uint16_t    AmplitudePerc,
    uint16_t    Duration,
    uint8_t     DurationFactor)
{
    uint32_t        base;
    R_SG_RegAdi_t   regAdi;
    R_SG_RegDf_t    regDf;
    R_SG_RegConf_t  regConf;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
    /* Set Mode   */
        regConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        regConf.Bit.Mode = R_SG_MODE_ADC; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, regConf.Word);  
    /* Set ADI   */
        regAdi.Word = R_SG_READ_REG(32, base + R_SG_SGADI);
        regAdi.Bit.Adi = Duration;
        R_SG_WRITE_REG(32, base + R_SG_SGADI, regAdi.Word);  
    /* Set DF   */
        regDf.Word = R_SG_READ_REG(32, base + R_SG_SGDF);
        /* Duration factor 0 means divider = 1 ! */
        regDf.Bit.Df = DurationFactor - 1;
        R_SG_WRITE_REG(32, base + R_SG_SGDF, regDf.Word);  
    /* Set Amplitude in %   */
        R_SG_SetVolume(Unit, AmplitudePerc);
    }    
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_FadeSoundLin
*/

r_sg_Error_t R_SG_FadeSoundLin(
    uint32_t    Unit,
    int16_t     StartAmplitudePerc,
    uint8_t     Divider,
    int16_t     AmplitudeStep)
{
    uint32_t        base;
    R_SG_RegAdi_t   regAdi;
    R_SG_RegDf_t    regDf;
    R_SG_RegConf_t  regConf;
    R_SG_RegFl_t    regFl;
    uint16_t        maxPwmVal;
    int16_t         tmp;  
    uint16_t         tmp_abs;  
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        /* Set Mode   */
        regConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        regConf.Bit.Mode = R_SG_MODE_ADI; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, regConf.Word);  

        regFl.Word = R_SG_READ_REG(32, base + R_SG_SGFL);
        maxPwmVal  = regFl.Bit.Fl;
        
        /* Set ADI   */        
        regAdi.Word = R_SG_READ_REG(32, base + R_SG_SGADI);
             
        if(AmplitudeStep < 0)
        {
        
            tmp = ((uint16_t)(~(uint16_t)AmplitudeStep) + 1);
        
            tmp_abs = ((uint16_t)tmp * maxPwmVal)/ 100;
            tmp = (uint16_t)(~(tmp_abs - 1u));
            regAdi.Bit.Adi = ((uint16_t)~(uint16_t)tmp + 1u) | 0x100u; 
       
        }
        else
        {
            regAdi.Bit.Adi = (maxPwmVal * AmplitudeStep) / 100; 
        }  

    /* Set ADI   */
        R_SG_WRITE_REG(32, base + R_SG_SGADI, regAdi.Word);
    /* Set DF   */
        regDf.Word = R_SG_READ_REG(32, base + R_SG_SGDF);
        /* Duration factor 0 means divider = 1 ! */
        regDf.Bit.Df = Divider - 1;
        R_SG_WRITE_REG(32, base + R_SG_SGDF, regDf.Word);
    /* Set Amplitude in %   */
        R_SG_SetVolume(Unit, StartAmplitudePerc);
    }    
    return R_SG_ERR_OK;
} 


/***********************************************************
  Function: R_SG_FadeOutSoundLog
*/

r_sg_Error_t R_SG_FadeOutSoundLog(
    uint32_t    Unit,
    int16_t     StartAmplitudePerc,
    uint8_t     Divider)
{
    uint32_t        base;
    R_SG_RegDf_t    regDf;
    R_SG_RegConf_t  regConf;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
    /* Set Mode   */
        regConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        regConf.Bit.Mode = R_SG_MODE_ALD; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, regConf.Word);  
    /* Set DF   */
        regDf.Word = R_SG_READ_REG(32, base + R_SG_SGDF);
        /* Duration factor 0 means divider = 1 ! */
        regDf.Bit.Df = Divider - 1;
        R_SG_WRITE_REG(32, base + R_SG_SGDF, regDf.Word);
    /* Set Amplitude in %   */
        R_SG_SetVolume(Unit, StartAmplitudePerc);
    }    
    return R_SG_ERR_OK;
} 


/***********************************************************
  Function: R_SG_FadeSoundLin
*/

r_sg_Error_t R_SG_LinFade(
    uint32_t    Unit,
    int16_t     StartAmplitudePerc,
    uint8_t     Divider,
    int16_t     Stepsize)
{
    uint32_t        base;
    R_SG_RegAdi_t   regAdi;
    R_SG_RegDf_t    regDf;
    R_SG_RegConf_t  regConf;
    uint16_t        maxPwmVal;
    uint16_t        tmp;
    uint16_t        tmp_abs;
    
    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE; 
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        
        maxPwmVal = R_SG_Dev[Unit].Fl;
        
    /* Set Mode   */
        regConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
        regConf.Bit.Mode = R_SG_MODE_ADI; 
        R_SG_WRITE_REG(32, base + R_SG_SGCONF, regConf.Word);  
    /* adapt sign */
        regAdi.Word = R_SG_READ_REG(32, base + R_SG_SGADI);

        if(Stepsize < 0)
        {
            tmp = ((uint16_t)(~(uint16_t)Stepsize) + 1);
            tmp_abs = (tmp * maxPwmVal) / 100;
            tmp =(uint16_t) ~(tmp_abs - 1u);
            regAdi.Bit.Adi = ((uint16_t)~(uint16_t)tmp + 1u) | 0x100u; 
       } 
       else 
       {
            regAdi.Bit.Adi = (maxPwmVal * Stepsize) / 100; 
       } 
#if 0
        if (AmplitudeStep < 0) {
            regAdi.Bit.Adi = ((~AmplitudeStep) + 1) | 0x100u;
        }
        else {
            regAdi.Bit.Adi = AmplitudeStep;
        }
#endif     
        
    /* Set ADI   */
        R_SG_WRITE_REG(32, base + R_SG_SGADI, regAdi.Word);
    /* Set DF   */
        regDf.Word = R_SG_READ_REG(32, base + R_SG_SGDF);
        /* Duration factor 0 means divider = 1 ! */
        regDf.Bit.Df = Divider - 1;
        R_SG_WRITE_REG(32, base + R_SG_SGDF, regDf.Word);
    /* Set Amplitude in %   */
        R_SG_SetVolume(Unit, StartAmplitudePerc);
    }    
    return R_SG_ERR_OK;
} 


/***********************************************************
  Function: R_SG_BreakEnable
*/

r_sg_Error_t R_SG_BreakEnable(
    uint32_t    Unit,
    uint32_t    BreakEnabled)
{
    uint32_t            base;
    R_SG_RegSvsDis_t    RegSvsDis;    

    if (Unit >= R_SG_MACRO_NUM) 
    {
        return R_SG_ERR_INSTANCE;
    } 
    else 
    {
        base = R_SG_Dev[Unit].BaseAddr;
        RegSvsDis.Byte = 0;
        RegSvsDis.Bit.SvsDis = (0 == BreakEnabled)?1:0; 
        R_SG_WRITE_REG(32, base + R_SG_SGEMU, RegSvsDis.Byte);
    }
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_SetRegisters
*/

r_sg_Error_t R_SG_SetRegisters(
    uint32_t             Unit,
    r_sg_RegisterSet_t   RegisterSet) 
{

    uint32_t        base;
    R_SG_RegAdi_t   _RegAdi;
    R_SG_RegDf_t    _RegDf;
    R_SG_RegConf_t  _RegConf;
    R_SG_RegIth_t   _RegIth;
    R_SG_RegFl_t    _RegFl;
    R_SG_RegFh_t    _RegFh;
    R_SG_RegPwm_t   _RegPwm;

    base = R_SG_Dev[Unit].BaseAddr;
    /* Set Mode   */
    
    _RegConf.Word = R_SG_READ_REG(32, base + R_SG_SGCONF);
    _RegConf.Bit.Mode = RegisterSet.Control; 

    R_SG_WRITE_REG(32, base + R_SG_SGCONF, _RegConf.Word);  
    /* Set DF   */
    /* Duration factor 0 means divider = 1 ! */
    _RegDf.Bit.Df = RegisterSet.Df - 1;
    R_SG_WRITE_REG(32, base + R_SG_SGDF, _RegDf.Word);
    
    /* Set ADI   */
    if (RegisterSet.Adi < 0) 
    {
        _RegAdi.Bit.Adi = ((~(uint32_t)RegisterSet.Adi) + 1) | 0x100u;
    }
    else 
    {
        _RegAdi.Bit.Adi = RegisterSet.Adi;
    }
    R_SG_WRITE_REG(32, base + R_SG_SGADI, _RegAdi.Word);
    /* Set Interrupt Threshold   */
    _RegIth.Bit.Ith = RegisterSet.Ith;
    R_SG_WRITE_REG(32, base + R_SG_SGITH, _RegIth.Word);
    /* Set FL */
    _RegFl.Bit.Fl = RegisterSet.Fl;
    R_SG_WRITE_REG(32, base + R_SG_SGFL, _RegFl.Word);
    /* Set FH */
    _RegFh.Bit.Fh = RegisterSet.Fh;
    R_SG_WRITE_REG(32, base + R_SG_SGFH, _RegFh.Word);
    /* Set Amplitude (PWM) */
    _RegPwm.Bit.Pwm = RegisterSet.Pwm;
    R_SG_WRITE_REG(32, base + R_SG_SGPWM, _RegPwm.Bit.Pwm);
    
    return R_SG_ERR_OK;
}


/***********************************************************
  Function: R_SG_GetConfRegAddress
*/

uint32_t R_SG_GetConfRegAddress(uint32_t Unit)
{
    return R_SG_Dev[Unit].BaseAddr + R_SG_SGCONF;
}

