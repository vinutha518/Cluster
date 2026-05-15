/*
****************************************************************************
PROJECT : Application test framework
FILE    : $Id: r_atf_main.c 7662 2016-02-15 14:59:20Z matthias.nippert $
============================================================================ 
DESCRIPTION
API&main for the framework
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
  Title: ATF Main Module

  Implementation of the application test framework API.
*/

/*******************************************************************************
  Section: Includes
*/

#include <string.h>

#include "r_typedefs.h"

#include "r_bsp_stdio_api.h"
#include <r_bsp_api.h>
#include "r_dev_api.h"
//#include "r_tick_api.h"

//#define OSTM_MAIN_GLOBALS_
#include "r_adc_api.h"
#include "r_adc_main.h"
#include "r_adc_sys.h"
//#include "r_rtca_api.h"
//#include "r_rtca_sys.h"
#include "r_gpio_api.h"

#include "ADC.h"
#include "r_adc7010_regs.h"
#include "swsc.h"
/*******************************************************************************
  Section: Local Functions
*/

/* ****************************************************************************** */
#if 0
#include "ADC.h"
#define MAX_NOF_CHANNELS  3U
#define MAX_NOF_SAMPELS	  10U

uint16_t ADC_Sample[10][3]={0};

uint8_t Channel_No = 0;
uint8_t Sample_No  = 0;
#endif
/* ****************************************************************************** */

void R_ADC_Create(void);

static void ( *loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;

r_adc_Config_t my_adca_config;
r_adc_ResParams_t my_adca_result;
uint16_t my_adc_result;

uint16_t my_adc_result1,my_adc_result2,my_adc_result3;

uint16_t Counter_3_5usec = 0;

void my_adc_isr()  /* This executes at every 3.5usec  */
{
	asm("nop");
	//R_GPIO_TogglePin(16,8);
	R_ADC_ReadRes(0,&my_adca_result);
	//r_adc_CovResult[R_ADC_MACRO_NUM][R_ADC_CG_PWM][R_ADC_CHANNEL_MAX_NUM];
	//my_adc_result = r_adc_CovResult[0][R_ADC_CG_0][0x12];
	//my_adc_result = r_adc_CovResult[0][R_ADC_CG_0][my_adca_config.VCh[1].phyChannel];

#if 1
	my_adc_result1 = r_adc_CovResult[0][R_ADC_CG_1][0x12]; //SWSC
	SWSC_ADC_value = my_adc_result1;

	if(Counter_3_5usec == 1) /* Updated fuel & BATT ADC value at every 100msec  */
	{
		Counter_3_5usec = 0;
		my_adc_result = r_adc_CovResult[0][R_ADC_CG_1][0x10]; //Fuel
		Fuel_ADC_value = my_adc_result;

		my_adc_result2 = r_adc_CovResult[0][R_ADC_CG_1][0x14]; //BATT
		Batt_ADC_value = my_adc_result2;

		my_adc_result3 = r_adc_CovResult[0][R_ADC_CG_1][0x11]; //Fuel_EXVref
		Fuel_EXVrefADC_value = my_adc_result3;
	}

#endif

#if 0
	/********* Store value in Array *********/
    ADC_Sample[Sample_No][Channel_No] = my_adc_result;

    Channel_No++;

    if(Channel_No > MAX_NOF_CHANNELS)
    {
            Channel_No = 0;
            Sample_No++;
            if(Sample_No > MAX_NOF_SAMPELS)
            {
                    Sample_No = 0;
            }
    }

    R_ADC_Stop();
    AD_Done_u1g = 1;
#endif
}

/***********************************************************************************************************************
* Function Name: R_ADC_Create
* Description  : This function initializes the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/*#define R_TAUB_TPS 0x0240 --> CK0-2^0,CK1-2^4,CK2-2^2,CK3-2^0*/
void R_ADC_Create(void)  
{
	my_adca_config.Res = R_ADC_12BIT_RES;
	my_adca_config.ResTreat = R_ADC_RESULT_RETAIN;
	my_adca_config.Align = R_ADC_RIGHT_ALIGNED;
	my_adca_config.SuspendMode = R_ADC_SYNC_SUSPEND;
	my_adca_config.SDVol_En = R_ADC_SD_OFF;
	my_adca_config.SmpTime = 0x18;
	my_adca_config.ULL[R_ADC_ULL_0].Upper_Limit = 0xfff;//[R_ADC_ULL_LAST] =
	my_adca_config.ULL[R_ADC_ULL_0].Lower_Limit = 0x00;//
	my_adca_config.ULL[R_ADC_ULL_1].Upper_Limit = 0xfff;//[R_ADC_ULL_LAST] =
	my_adca_config.ULL[R_ADC_ULL_1].Lower_Limit = 0x00;//
	my_adca_config.ULL[R_ADC_ULL_2].Upper_Limit = 0xfff;//[R_ADC_ULL_LAST] =
	my_adca_config.ULL[R_ADC_ULL_2].Lower_Limit = 0x00;//

	my_adca_config.ULErrInfo = R_ADC_DONT_GENERATE_ERR;//R_ADC_GENERATE_ERR;
	my_adca_config.OWErrInfo = R_ADC_DONT_GENERATE_ERR;

#if 0
	my_adca_config.Group[R_ADC_CG_1].Mode = R_ADC_CONTINUOUS_MODE;
	my_adca_config.Group[R_ADC_CG_1].SGNum = R_ADC_SGCONV_ONCE;
	my_adca_config.Group[R_ADC_CG_1].Trigger = R_ADC_SW_TRIGGER;
	my_adca_config.Group[R_ADC_CG_1].Int_SGEnd = R_ADC_COVEND_EFECTIVE;
	my_adca_config.Group[R_ADC_CG_1].VcStart = 0x00;
	my_adca_config.Group[R_ADC_CG_1].VcEnd = 0x00;
	my_adca_config.Group[R_ADC_CG_1].ConvNum = R_ADC_CONVERT_ONCE;

	my_adca_config.VCh[0].MPXEnable = R_ADC_NO_MPX;	//multiplexer not used	//[R_ADC_CHANNEL_MAX_NUM] =
	my_adca_config.VCh[0].MPXAddr = 0x00;	//npo multiplezer used, hence no address for multiplexer
	my_adca_config.VCh[0].INT_VCEnd = R_ADC_COVEND_IGNORED;
	my_adca_config.VCh[0].ULLCheck = R_ADC_NO_ULL;
	my_adca_config.VCh[0].phyChannel = 0x12;
	my_adca_config.VCh[0].phyChPD = R_ADC_PULLDN_OFF;
#endif

#if 1
	my_adca_config.Group[R_ADC_CG_1].Mode = R_ADC_CONTINUOUS_MODE;
	my_adca_config.Group[R_ADC_CG_1].SGNum = R_ADC_SGCONV_ONCE;
	my_adca_config.Group[R_ADC_CG_1].Trigger = R_ADC_SW_TRIGGER;
	my_adca_config.Group[R_ADC_CG_1].Int_SGEnd = R_ADC_COVEND_EFECTIVE;
	my_adca_config.Group[R_ADC_CG_1].VcStart = 0x00;
	my_adca_config.Group[R_ADC_CG_1].VcEnd = 0x03;
	my_adca_config.Group[R_ADC_CG_1].ConvNum = R_ADC_CONVERT_ONCE;

	my_adca_config.VCh[0].MPXEnable = R_ADC_NO_MPX;	//multiplexer not used	//[R_ADC_CHANNEL_MAX_NUM] =
	my_adca_config.VCh[0].MPXAddr = 0x00;	//npo multiplezer used, hence no address for multiplexer
	my_adca_config.VCh[0].INT_VCEnd = R_ADC_COVEND_IGNORED;
	my_adca_config.VCh[0].ULLCheck = R_ADC_NO_ULL;
	my_adca_config.VCh[0].phyChannel = 0x10;
	my_adca_config.VCh[0].phyChPD = R_ADC_PULLDN_ON;
	// my_adca_config.VCh[0].SDparams.SDmode = R_ADC_SD_COV;
	// my_adca_config.VCh[0].SDparams.SDinput = R_ADC_SD_ANIN;


	my_adca_config.VCh[1].MPXEnable = R_ADC_NO_MPX;	//multiplexer not used	//[R_ADC_CHANNEL_MAX_NUM] =
	my_adca_config.VCh[1].MPXAddr = 0x00;	//npo multiplezer used, hence no address for multiplexer
	my_adca_config.VCh[1].INT_VCEnd = R_ADC_COVEND_IGNORED;
	my_adca_config.VCh[1].ULLCheck = R_ADC_NO_ULL;
	my_adca_config.VCh[1].phyChannel = 0x12;
	my_adca_config.VCh[1].phyChPD = R_ADC_PULLDN_ON;
	// my_adca_config.VCh[1].SDparams.SDmode = R_ADC_SD_COV;
	// my_adca_config.VCh[1].SDparams.SDinput = R_ADC_SD_ANIN;

	my_adca_config.VCh[2].MPXEnable = R_ADC_NO_MPX;	//multiplexer not used	//[R_ADC_CHANNEL_MAX_NUM] =
	my_adca_config.VCh[2].MPXAddr = 0x00;	//npo multiplezer used, hence no address for multiplexer
	my_adca_config.VCh[2].INT_VCEnd = R_ADC_COVEND_IGNORED;
	my_adca_config.VCh[2].ULLCheck = R_ADC_NO_ULL;
	my_adca_config.VCh[2].phyChannel = 0x14;
	my_adca_config.VCh[2].phyChPD = R_ADC_PULLDN_ON;
	// my_adca_config.VCh[2].SDparams.SDmode = R_ADC_SD_COV;
	// my_adca_config.VCh[2].SDparams.SDinput = R_ADC_SD_ANIN;

	my_adca_config.VCh[3].MPXEnable = R_ADC_NO_MPX;	//multiplexer not used	//[R_ADC_CHANNEL_MAX_NUM] =
	my_adca_config.VCh[3].MPXAddr = 0x00;	//npo multiplezer used, hence no address for multiplexer
	my_adca_config.VCh[3].INT_VCEnd = R_ADC_COVEND_IGNORED;
	my_adca_config.VCh[3].ULLCheck = R_ADC_NO_ULL;
	my_adca_config.VCh[3].phyChannel = 0x11;
	my_adca_config.VCh[3].phyChPD = R_ADC_PULLDN_ON;

#endif

	my_adca_result.ScanGroup = R_ADC_CG_1;

	R_ADC_Init(0,&my_adca_config);
	R_ADC_SetIsrCallback(0,R_ADC_INT_CG0,&my_adc_isr);
	//R_ADC_SetIsrCallback(0,R_ADC_INT_TSN,&my_adc_tsn_isr);

	R_SYS_ADC_EnableInt(0,R_ADC_INT_CG0);

	R_ADC_StartGroupConversion(0,R_ADC_CG_1);


#if 0
	R_ADC_Init(1,&my_adca_config);
	R_ADC_SetIsrCallback(0,R_ADC_INT_CG1,&my_adc_isr);
	R_SYS_ADC_EnableInt(0,R_ADC_INT_CG1);
	R_ADC_StartGroupConversion(0,R_ADC_CG_2);
#endif

}






