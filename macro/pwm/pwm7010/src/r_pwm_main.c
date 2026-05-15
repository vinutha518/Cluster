/*
****************************************************************************
PROJECT : PWM driver
FILE    : $Id: r_pwm_main.c 4016 2014-10-01 11:20:52Z golczewskim $
============================================================================
DESCRIPTION
Driver for PWM macro
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
  Title: PWM7010 Main Module

  Implementation of the macro driver ADC support functions 
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_pwm7010_regs.h"
#include "r_config_pwm.h"
#define PWM_MAIN_GLOBALS_
#include "r_pwm_api.h"
#include "r_pwm_main.h"
#undef PWM_MAIN_GLOBALS_
#include "r_pwm_sys.h"

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0;

/*******************************************************************************
  Function: R_PWM_ErrorHandler
  
  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_pwm_Error_t>
  
  Returns:
  void
*/

void R_PWM_ErrorHandler(uint32_t Unit, r_pwm_Error_t Error) 
{
    if (loc_ErrorHandler != 0) 
    {      
        /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t)Error);      
    } 
    else 
    {
        for (;;) 
        {                          /* we don't have a handler */
        }                          /* so, let's wait here */
    }
}

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_PWM_Init
  
  see: <r_pwm_api.h> for details
*/
r_pwm_Error_t R_PWM_Init(uint32_t Unit, r_pwm_Config_t * PwmCfg)
{
    uint8_t            ChNum;
    r_pwm_Error_t      ret_val                    = R_PWM_ERR_NG;
    
    if (r_pwm_InitFlag[Unit] == R_PWM_NO_INIT)
    {
        if (Unit < R_PWM_MACRO_NUM )
        {
            r_pwm_Unit[Unit].PWBA_BaseAddr            = R_SYS_PWBA_GetBaseAddr(Unit);
            r_pwm_Unit[Unit].PWSA_BaseAddr            = R_SYS_PWSA_GetBaseAddr(Unit);
            for (ChNum = 0; ChNum < R_PWM_PWGA_NUM; ChNum++)
            {
                r_pwm_Unit[Unit].PWGA_BaseAddr[ChNum] = R_SYS_PWGA_GetBaseAddr(ChNum);
            }
            R_PWM_PWBAConfig(Unit, PwmCfg);
            R_PWM_PWGAConfig(Unit, PwmCfg);
            R_PWM_PWSAConfig(Unit, PwmCfg);
            r_pwm_InitFlag[Unit] = R_PWM_INIT;
        }
        else
        {
            ret_val = R_PWM_ERR_RANGE;
        }
    }
    else
    {
        ret_val    = R_PWM_ALREADY_INITIALIZED;
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_PWM_ResetPWBACfg
  
  see: <r_pwm_api.h> for details
*/
void R_PWM_ResetPWBACfg (r_pwm_Config_t * PwmCfg)
{
    uint32_t            Unit;
    r_pwm_Counter_t CntNum;
    for (Unit = 0; Unit < R_PWM_PWBA_NUM; Unit++)
    {
        PwmCfg->PWM_Emu                        = R_PWM_SVSDIS_EFECTIVE;
        for (CntNum = R_PWM_CNT_0; CntNum < R_PWM_CNT_LAST; CntNum++)
        {
            PwmCfg->CntCfg[CntNum].CounterMode = R_PWM_CNT_DISABLE;
            PwmCfg->CntCfg[CntNum].ClkCyc_Hz   = 0;
        }
    }
}

/*******************************************************************************
  Function: R_PWM_ResetPWGACfg
  
  see: <r_pwm_api.h> for details
*/
void R_PWM_ResetPWGACfg (r_pwm_Config_t * PwmCfg)
{
    uint8_t  ChNum;
    for (ChNum = 0; ChNum < R_PWM_PWGA_NUM; ChNum++)
    {
        PwmCfg->ChCfg[ChNum].ChMode      = R_PWM_Channel_DISABLE;
        PwmCfg->ChCfg[ChNum].Delay_100   = 0;
        PwmCfg->ChCfg[ChNum].Duty        = 0;
        PwmCfg->ChCfg[ChNum].TriggerPro  = 0;
        PwmCfg->ChCfg[ChNum].Counter     = R_PWM_CNT_0;
        PwmCfg->ChCfg[ChNum].SRwr        = R_PWM_SRWR_OFF;
    }
}

/*******************************************************************************
  Function: R_PWM_ResetPWSACfg
  
  see: <r_pwm_api.h> for details
*/
void R_PWM_ResetPWSACfg (r_pwm_Config_t * PwmCfg)
{
    uint32_t     Unit;
    uint8_t  ChNum;
    for (Unit = 0; Unit < R_PWM_PWSA_NUM; Unit++)
    {
        PwmCfg->PWSAMode                       = R_PWM_TRG_OFF;
        for (ChNum = 0; ChNum < R_PWM_PWGA_NUM; ChNum++)
        {
            PwmCfg->DiagCfg[ChNum].UllCheck    = R_PWM_NO_ULL;
            PwmCfg->DiagCfg[ChNum].AdcaUnit    = R_PWM_ADC_0;
            PwmCfg->DiagCfg[ChNum].MpxMode     = R_PWM_MPX_DIS;
            PwmCfg->DiagCfg[ChNum].MpxAdr      = 0;
            PwmCfg->DiagCfg[ChNum].PhyChannel  = 0;
        }
    }
}

/*******************************************************************************
  Function: R_PWM_ResetCfg
  
  see: <r_pwm_api.h> for details
*/
void R_PWM_ResetCfg (r_pwm_Config_t * PwmCfg)
{
    R_PWM_ResetPWBACfg (PwmCfg);
    R_PWM_ResetPWGACfg (PwmCfg);
    R_PWM_ResetPWSACfg (PwmCfg);
}

/*******************************************************************************
  Function: R_PWM_DeInit

  see: <r_pwm_api.h> for details
*/

r_pwm_Error_t R_PWM_DeInit(uint32_t Unit)
{
    uint32_t         addr    = 0;
    uint32_t         RegNum;
    uint32_t         MaxRegNum;
    r_pwm_Config_t   PwmCfg;
    r_pwm_Error_t    ret_val = R_PWM_ERR_NG;
    
    if (Unit < R_PWM_PWBA_NUM)
    {
        R_PWM_ResetCfg(&PwmCfg);
        MaxRegNum  = R_PWM_PWGA_NUM / 2;
        
        /* Stop the Generator and Timer */
        R_PWM_Str(Unit,&PwmCfg);
        R_PWM_PWBAStop(Unit,R_PWM_CNT_0);
        R_PWM_PWBAStop(Unit,R_PWM_CNT_1);
        R_PWM_PWBAStop(Unit,R_PWM_CNT_2);
        R_PWM_PWBAStop(Unit,R_PWM_CNT_3);
        
        R_PWM_PWBAConfig(Unit,&PwmCfg);
        R_PWM_PWGAConfig(Unit,&PwmCfg);
        for (RegNum = 0; RegNum < MaxRegNum; RegNum++)
        {
            addr    = r_pwm_Unit[Unit].PWSA_BaseAddr + R_PWSA_PVCR(RegNum);
            R_PWM_WRITE_REG(32, addr, 0x0);
        }
        r_pwm_InitFlag[Unit]      = R_PWM_NO_INIT;
        ret_val     = R_PWM_ERR_OK;
    }
    else
    {
        ret_val = R_PWM_ERR_RANGE;
    }
    
    return ret_val;
}

/*******************************************************************************
  Function: R_PWM_PWBAConfig

  see: <r_pwm_api.h> for details
*/
r_pwm_Error_t R_PWM_PWBAConfig(uint32_t Unit, r_pwm_Config_t * PwmCfg)
{
    uint32_t         addr    = 0;
    uint16_t         reg_val = 0;
    uint32_t         pwm_pclk;
    r_pwm_Counter_t  CntNum;
    uint16_t         ClkCyc_BR;
    r_pwm_Error_t    ret_val = R_PWM_ERR_NG;

    if (Unit < R_PWM_PWBA_NUM)
    {
        for (CntNum = R_PWM_CNT_0; CntNum < R_PWM_CNT_LAST; CntNum++)
        {
            addr    = r_pwm_Unit[Unit].PWBA_BaseAddr + R_PWBA_BRS(CntNum);
            reg_val = PwmCfg->CntCfg[CntNum].ClkCyc_Hz;
            
            pwm_pclk    = R_DEV_ClkFrequencyHz(R_DEV_CKS_PWBA);
            ClkCyc_BR   = (uint16_t)(pwm_pclk / (PwmCfg->CntCfg[CntNum].ClkCyc_Hz) / 4096/2);
            reg_val     = ClkCyc_BR;
            R_PWM_WRITE_REG(16, addr, reg_val);
            
            addr    = r_pwm_Unit[Unit].PWBA_BaseAddr + R_PWBA_TS;
            reg_val = (PwmCfg->CntCfg[CntNum].CounterMode) << CntNum;
            R_PWM_WRITE_REG(8, addr, reg_val);
        }
        ret_val = R_PWM_ERR_OK;
    }
    else
    {
        ret_val = R_PWM_ERR_RANGE;
    }
    return ret_val; 
}

/*******************************************************************************
  Function: R_PWM_PWGAConfig

  see: <r_pwm_api.h> for details
*/
r_pwm_Error_t R_PWM_PWGAConfig(uint32_t Unit, r_pwm_Config_t * PwmCfg)
{
    uint32_t         addr    = 0;
    uint16_t         reg_val = 0;
    uint8_t          ChNum;
    uint16_t         temp;
    uint32_t         RisingEdge;
    uint32_t         FallingEdge;
    uint32_t         TrgPeriod;
    r_pwm_Error_t    ret_val = R_PWM_ERR_NG;    
    
    if (Unit < R_PWM_MACRO_NUM)
    {
        for (ChNum = R_PWM_CNT_0; ChNum < R_PWM_PWGA_NUM; ChNum++)
        {
            addr    = r_pwm_Unit[Unit].PWGA_BaseAddr[ChNum] + R_PWGA_CTL;
            reg_val = PwmCfg->ChCfg[ChNum].Counter;
            R_PWM_WRITE_REG(8, addr, reg_val);
            
            temp = ((PwmCfg->ChCfg[ChNum].Delay_100) * 4096 % 10000) / 1000;
            if (temp < 5)
            {
                RisingEdge = ((PwmCfg->ChCfg[ChNum].Delay_100) * 4096) / 10000;
            }
            else
            {
                RisingEdge = (((PwmCfg->ChCfg[ChNum].Delay_100) * 4096) / 10000) - 1;
            }
            reg_val    = (uint16_t)(RisingEdge);
            addr       = r_pwm_Unit[Unit].PWGA_BaseAddr[ChNum] + R_PWGA_CSDR;
            R_PWM_WRITE_REG(16, addr, reg_val);
            
            TrgPeriod    = (PwmCfg->ChCfg[ChNum].Duty) * 4096 / 100;
            FallingEdge  = TrgPeriod + RisingEdge;
            reg_val  = (uint16_t)FallingEdge;
            addr     = r_pwm_Unit[Unit].PWGA_BaseAddr[ChNum] + R_PWGA_CRDR;
            if (PwmCfg->ChCfg[ChNum].Duty == 100)
            {
                R_PWM_WRITE_REG(16, addr, 0x1000);
            }
            else if (FallingEdge <= 0xfff)
            {                
                R_PWM_WRITE_REG(16, addr, reg_val);
            }
            else if (FallingEdge <= 0x1fff)
            {
                reg_val -= 4096;
                R_PWM_WRITE_REG(16, addr, reg_val);
            }
            else
            {
                ret_val  = R_PWM_PARAM_CONFIG;
            }
            
       
            if (PwmCfg->ChCfg[ChNum].TriggerPro <= 100)
            {
                addr         = r_pwm_Unit[Unit].PWGA_BaseAddr[ChNum] + R_PWGA_CTDR;
                TrgPeriod    = TrgPeriod * (PwmCfg->ChCfg[ChNum].TriggerPro) / 100;
                reg_val      = (uint16_t)(TrgPeriod  + RisingEdge);
                R_PWM_WRITE_REG(16, addr, reg_val);
            }
            else
            {
                ret_val = R_PWM_PARAM_CONFIG;
            }
            
            reg_val = PwmCfg->ChCfg[ChNum].SRwr;
            addr    = r_pwm_Unit[Unit].PWGA_BaseAddr[ChNum] + R_PWGA_RDT;
            R_PWM_WRITE_REG(8, addr, reg_val);
            ret_val = R_PWM_ERR_OK;
        }
    }
    else
    {
        ret_val = R_PWM_ERR_RANGE;
    }
    return ret_val; 
}

/*******************************************************************************
  Function: R_PWM_PWSAConfig

  see: <r_pwm_api.h> for details
*/
r_pwm_Error_t R_PWM_PWSAConfig(uint32_t Unit, r_pwm_Config_t * PwmCfg)
{
    uint32_t         addr        = 0;
    uint32_t         reg_val     = 0;
    uint32_t         new_reg_val = 0;
    uint8_t          ChNum;
    uint8_t          OddCh;
    r_pwm_Error_t    ret_val     = R_PWM_ERR_NG;  
    
    if (Unit < R_PWM_MACRO_NUM)
    {
        for (ChNum = R_PWM_CNT_0; ChNum < R_PWM_PWGA_NUM; ChNum++)
        {
            OddCh  = ChNum % 2;
            addr   = r_pwm_Unit[Unit].PWSA_BaseAddr + R_PWSA_PVCR((ChNum / 2));
            new_reg_val = PwmCfg->DiagCfg[ChNum].PhyChannel;
            if (new_reg_val < R_SYS_PWM_MaxPhyChannelNum(PwmCfg->DiagCfg[ChNum].AdcaUnit) && PwmCfg->DiagCfg[ChNum].UllCheck < R_PWM_ULL_LAST)
            {
                new_reg_val |= (PwmCfg->DiagCfg[ChNum].UllCheck << 6);
                new_reg_val |= (PwmCfg->DiagCfg[ChNum].AdcaUnit << 12);
                if (PwmCfg->DiagCfg[ChNum].MpxMode)
                {
                    new_reg_val |= (0x1 << 11);
                }
                if (PwmCfg->DiagCfg[ChNum].MpxAdr < 8)
                {
                    new_reg_val |= (PwmCfg->DiagCfg[ChNum].MpxAdr << 8);
                }
                else
                {
                    ret_val = R_PWM_PARAM_CONFIG;
                }
            }
            if (OddCh)
            {
                reg_val  = R_PWM_READ_REG(32, addr);
                reg_val |= (new_reg_val << 16);
            }
            else
            {
                reg_val  = new_reg_val;
            }
            R_PWM_WRITE_REG(32, addr, reg_val);
            ret_val = R_PWM_ERR_OK;
        }
    }
    else
    {
        ret_val = R_PWM_ERR_RANGE;
    }
    return ret_val; 
}

/*******************************************************************************
  Function: R_PWM_DbgSVSDIS
  
  see: <r_pwm_api.h> for details
*/
void R_PWM_DbgSVSDIS (uint32_t Unit, r_pwm_Config_t * PwmCfg)
{
    uint32_t  addr    = 0;
    
    addr =  r_pwm_Unit[Unit].PWBA_BaseAddr + R_PWBA_EMU;
    if (PwmCfg->PWM_Emu)
    {
        R_PWM_WRITE_REG(8, addr, 0x80);
    }
}

/*******************************************************************************
  Function: R_PWM_PWBAStop

  see: <r_pwm_api.h> for details
*/
r_pwm_Error_t R_PWM_PWBAStop(uint32_t Unit, r_pwm_Counter_t CntNum)
{
    uint32_t         addr    = 0;
    uint32_t         reg_val = 0;
    r_pwm_Error_t    ret_val = R_PWM_ERR_NG;
    
    if (Unit < R_PWM_PWBA_NUM)
    {
        if (CntNum < R_PWM_CNT_LAST)
        {
            addr    = r_pwm_Unit[Unit].PWBA_BaseAddr + R_PWBA_TT;
            reg_val = 0x1 << CntNum;
            R_PWM_WRITE_REG(8, addr, reg_val);
            ret_val = R_PWM_ERR_OK;
        }
        else
        {
             ret_val = R_PWM_PARAM_CONFIG;
        }
    }
    else
    {
        ret_val = R_PWM_ERR_RANGE;
    }
    return ret_val;
}

/*******************************************************************************
  Function: R_PWM_Str

  see: <r_pwm_api.h> for details
*/
r_pwm_Error_t R_PWM_Str(uint32_t Unit, r_pwm_Config_t * PwmCfg)
{
    uint32_t         addr     = 0;
    uint32_t         reg_val  = 0;
    uint32_t         reg_temp = 0;
    uint32_t         Sl_num   = 0;
    uint8_t          ChNum;
    uint8_t          LastCh;
    r_pwm_Error_t    ret_val = R_PWM_ERR_NG;
    
    for (ChNum = 0; ChNum < R_PWM_PWGA_NUM; ChNum++)
    {
        Sl_num  = ChNum / 32;
        LastCh  = ChNum % 32;
        if (LastCh == 0 && ChNum != 0)
        {
            R_PWM_WRITE_REG(8, addr, reg_val);
            reg_temp = 0;
        }
        addr    = R_PWGA_SLPWGA(Sl_num);
        reg_val = PwmCfg->ChCfg[ChNum].ChMode;
        switch (reg_val)
        {
        case R_PWM_Channel_DISABLE: 
            reg_val  = reg_temp & (~(0x1 << (ChNum - (Sl_num*32))));
            reg_temp = reg_val;
            ret_val  = R_PWM_ERR_OK;
            break;
        case R_PWM_Channel_ENABLE : 
            reg_val  = reg_temp | (0x1 << (ChNum - (Sl_num*32)));
            reg_temp = reg_val;
            ret_val  = R_PWM_ERR_OK;
            break;
        default                   : 
            /* PWM: Invalid Start/Stop Trigger */
            R_PWM_ErrorHandler(Unit, R_PWM_ERR_RANGE); 
        }
        if (ChNum == (R_PWM_PWGA_NUM - 1))
        {
            R_PWM_WRITE_REG(32, addr, reg_val);
        }
    }
    
    addr    = r_pwm_Unit[Unit].PWSA_BaseAddr + R_PWSA_CTL;
    reg_val = PwmCfg->PWSAMode;
    R_PWM_WRITE_REG(8, addr, reg_val);
    ret_val = R_PWM_ERR_OK;
    
    return ret_val;
}



/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
