/*
 ****************************************************************************
Project : Audio generation demo
FILE    : $Id: rtc.c 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================
DESCRIPTION
Audio generation demo Test Application
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
  Section: Includes
 */

#include "r_typedefs.h"
#include "dr7f701461.dvf.h"
#include "r_rtca_api.h"
#include "r_gpio_api.h"
#include "rtc.h"
//#include "state.h"
//#include "math.h"
#include "config.h"
#include "r_dev_api.h"
#include "r_bsp_stdio_api.h"
#include "digitalGuage.h"
//#include "r_awot_api.h"

#include "memmory_map.h"
#include "r_cg_serial_user.h"
#include "init.h"
#include "gui.h"
#include "digi_clk.h"
/***********************************************************
  Section: Local Defines
 */


/***********************************************************
  Define: LOC_MAX_SAMPLE_ERROR_PERCENT

  Time and alarm parameters limits
 */

#define LOC_NUM_YEARS                100
#define LOC_NUM_MONTHS               12
#define LOC_NUM_DAYS                 31
#define LOC_NUM_HOURS                24
#define LOC_NUM_MINUTES              60
#define LOC_NUM_SECONDS              60


/***********************************************************
  Section: Variables
 */

uint8_t disp_sec,disp_min,disp_hour,disp_day,disp_month,disp_year,time_format,rtc_pm;

extern uint8_t count_start;
extern uint8_t count_start_temp;
extern uint8_t count_start_temp_B;
extern uint8_t AVS_sec_count_temp;
extern uint8_t count_start_B;
extern uint8_t AVS_sec_count_temp_B;
extern uint8_t try_switching;
_Bool format_changed;
extern uint8_t hrs;
extern uint8_t min;
extern uint8_t sec;
extern uint8_t test_rtc_reset_flag;
extern uint32_t wakeup_factor;
extern unsigned int READ_RTC_RESET;
extern uint8_t Read_RTC_Format(void);

/* Assign x1 to section .data1 */
#pragma ghs section data=".rdata"
extern uint8_t *RRAM_hrs;
extern uint8_t *RRAM_min;
extern uint8_t *RRAM_sec;

/* Now go back to default rules */
#pragma ghs section data=default

/***********************************************************
  Variable: locIntervalCount

  test variable ISR counters.
 */

static uint32_t locIntervalCount;


/***********************************************************
  Variable: locAlarmCount

  test variable ISR counters.
 */

static uint32_t locAlarmCount;


/***********************************************************
  Variable: locSecondCount

  test variable ISR counters.
 */

static uint32_t locSecondCount;
static uint32_t locPreSecondCount;


/***********************************************************
  Variable: RTCConfig

  RTC configuration.
 */

static r_rtca_Parameter_t RTCConfig;


/***********************************************************
  Variable: RTCData

  RTC Data to store.
 */

static r_rtca_Data_t RTCData;
static r_rtca_Data_t RTC_change_data;

/***********************************************************
  Variable: RTCReadData

  RTC buffer for reading.
 */

static r_rtca_Data_t RTCReadData;


/***********************************************************
  Variable: RTCAlarm

  RTC alarm data to store.
 */

static r_rtca_Alarm_t RTCAlarm;


/***********************************************************
  Variable: RTCIntConfig

  RTC interrupt configuration.
 */

static r_rtca_IntConf_t RTCIntConfig;


/***********************************************************
  Section: Local Functions
 */


/***********************************************************
  Function: locIntIsrCallback
 */
static void locIntIsrCallback()
{
	locIntervalCount++;
}


/***********************************************************
  Function: locAlmIsrCallback
 */
static void locAlmIsrCallback()
{
	locAlarmCount++;
}


/***********************************************************
  Function: locSecIsrCallback
 */
static void locSecIsrCallback()
{
	locSecondCount++;
}


uint8_t BCDToDecimal (uint8_t bcdByte)
{
	return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

uint8_t DecimalToBcd(uint8_t decimal)
{
	    return (uint8_t) ((decimal / 10)*16)+(decimal % 10);
}
/***********************************************************
  Function: eeRtc_Init
 */
void eeRtc_Init(uint8_t Wakeup)
{
	static uint8_t monitor_rtc_output = R_FALSE;
	static uint8_t hrs_temp;
	r_dev_ClkSelConfig_t ClkSelCfg;
	r_Error_t error;
uint8_t pm_10_11_12_flag;

	/* option to monitor the RTCA ref clock on FOUT output, pin is configured in the BSP */
	if(monitor_rtc_output)
	{
		ClkSelCfg.Cks       = R_DEV_CKS_FOUT;
		ClkSelCfg.SrcId     = R_DEV_CKS_SRC_SOSC;
		ClkSelCfg.Div       = 1;
		ClkSelCfg.StpReqMsk = 1;
		error = R_DEV_ClkIdSet(&ClkSelCfg);

		if(error == R_ERR_OK)
		{
			/* do nothing */
		}
		else
		{
			// R_BSP_STDIO_Printf("Clock config error: %d\n", error);
		}
	}

	/* if we are waking up then the RTC will not require complete initialisation
       we will get the configuration from the Back up RAM
	 */
#if 1
	if(Wakeup)
	{

	}
	else /* reset */
	{
		/* select the sub oscillator */
		ClkSelCfg.Cks       = R_DEV_CKS_RTC;
		ClkSelCfg.SrcId     = R_DEV_CKS_SRC_SOSC;
		ClkSelCfg.Div       = 1;
		ClkSelCfg.StpReqMsk = 1; /* 1 = keep alive in deep stop */
		error = R_DEV_ClkIdSet(&ClkSelCfg);

		//   R_BSP_STDIO_Printf("RTCATCKI clock frequency '%dHz' \n", R_DEV_ClkFrequencyHz(R_DEV_CKS_RTC));
	}
#endif
	/* both wakeup and reset */
	locIntervalCount = 0;
	locAlarmCount  = 0;
	locSecondCount  = 0;

	RTCConfig.Mode     = R_RTCA_FREQ_MODE;

	//RTCConfig.Format   = R_RTCA_24H;
	RTCConfig.CompVal  = 32768 - 1;


	RTCConfig.CorrectVal.Val = 0;
	RTCConfig.CorrectVal.Sign = 0;
	RTCConfig.CorrectVal.Interv = 0;

		/* if we have woken up then get the alarm settings from the device */
		if(Wakeup)
		{
			/* nothing */
		}
		else /* reset */
		{
			/* init the Alarm configuration */
			RTCAlarm.Minute      = 5;
			RTCAlarm.AmPm        = R_RTCA_AM;
			RTCAlarm.Hour        = 1;
			RTCAlarm.Day         = 0x7f; /* all days */
			RTCAlarm.Format      = R_RTCA_24H;
		}

		/* configure the interrupts */
		RTCIntConfig.AlarmInt  = R_RTCA_ENABLE;
		RTCIntConfig.IntervInt = R_RTCA_1MIN_INT;
		RTCIntConfig.SecInt    = R_RTCA_ENABLE;

		/* set up the interrupts */
		R_RTCA_SetIsrCallback(RTCA_UNIT, R_RTCA_INT_R, &locIntIsrCallback);
		R_RTCA_SetIsrCallback(RTCA_UNIT, R_RTCA_INT_AL, &locAlmIsrCallback);
		R_RTCA_SetIsrCallback(RTCA_UNIT, R_RTCA_INT_1S, &locSecIsrCallback);




#if 1
	if( wakeup_factor==0)
	{

if(TimeF_selected == 1)
		{
			//hrs=BCDToDecimal(hrs);
			hrs_temp = hrs;
			sec=BCDToDecimal(sec);
			min=BCDToDecimal(min);

			if(hrs==0x30)
			{
				hrs=10;
				pm_10_11_12_flag=1;
			}
			if(hrs==0x31)
			{
				hrs=11;
				pm_10_11_12_flag=1;
			}
			if(hrs==0x32)
			{
				hrs=12;
				pm_10_11_12_flag=1;
			}

			if(hrs==0x10 || hrs==0x11 || hrs==0x12)
			{
				hrs=BCDToDecimal(hrs);
			}
			RTCData.Format     = R_RTCA_12H;
			RTCConfig.Format   = R_RTCA_12H;
			RTCData.H          = hrs;

			if((hrs_temp &(1<<5))==0x20)
			{
				AM_flag=0;
			}
			else
			{
				AM_flag=1;
			}

//			if(pm_10_11_12_flag==1)
//			{
//				AM_flag=0;
//			}
		}
		else if(TimeF_selected == 2)
		{
			hrs=BCDToDecimal(hrs);
			sec=BCDToDecimal(sec);
			min=BCDToDecimal(min);
			RTCData.Format     = R_RTCA_24H;
			RTCConfig.Format   = R_RTCA_24H;
		}

		RTCData.Sec        = sec;
		RTCData.Min        = min;
		RTCData.H          = hrs;
		RTCData.Week       = 3;
		RTCData.Day        = 13;
		RTCData.Month      = 3;
		RTCData.Year       = 19;


		RTCData.Method     = R_RTCA_SINGLE;
		RTCData.Group      = R_RTCA_BOTH;

				if(AM_flag == 1)  //TAKE FLAG AS USED IN GUI.C
				{
					RTCData.AmPm       = R_RTCA_AM;
				}
				else
				{
					RTCData.AmPm       = R_RTCA_PM;
				}




				/* for wakeup */
					if(Wakeup)
					{
						/* need to enable the interrupts in the INT controller the RTC interrupts will already
				           be enabled but it will not be a problem to write them again
						 */

						RTCData.Format     =	Read_RTC_Format();
						R_RTCA_SetInt(RTCA_UNIT, RTCIntConfig);
					}
					else /* reset */
					{
						/* configure the RTC */
						R_RTCA_CtrCtl(RTCA_UNIT, R_RTCA_DISABLE);
						R_RTCA_Init(RTCA_UNIT, &RTCConfig, &RTCData);
						R_RTCA_OutputCtl(RTCA_UNIT, R_RTCA_ENABLE);
						R_RTCA_SetInt(RTCA_UNIT, RTCIntConfig);
						R_RTCA_CtrCtl(RTCA_UNIT, R_RTCA_ENABLE);
					}

					if(READ_RTC_RESET!=0)
					{
						R_RTCA_UpdateTime(RTCA_UNIT,&RTCData);
					}
					RTCReadData=RTCData;
	}

	/* get the latest time */
	R_RTCA_ReadCounterBuf(RTCA_UNIT, &RTCReadData);
	if(RTCData.Format == R_RTCA_24H)
	{
		time_format=0;			//24hr_mode
	}
	else if(RTCData.Format == R_RTCA_12H)
	{
		time_format=1;				//12hr_mode
	}
#endif
	locSecondCount = 0;
	locPreSecondCount = 255;
}


/***********************************************************
  Function: RTCProcessing
 */
void eeRtc_Processing(void)
{
	if((Timesetting_completed==1)||(TimeF_selection == 1))
	{

		format_changed=1;

		//				try_switching=0;
		RTCData.Sec        = 00;
		RTCData.Min        = changed_min;
		//RTCData.H          = changed_hr;
		RTCData.Week       = 3;
		RTCData.Day        = 13;
		RTCData.Month      = 3;
		RTCData.Year       = 19;


		RTCData.Method     = R_RTCA_SINGLE;
		RTCData.Group      = R_RTCA_BOTH;





		if(TimeF_selected == 1)
		{
			RTCData.Format     = R_RTCA_12H;
			RTCConfig.Format   = R_RTCA_12H;
			if(changed_hr==12)
			{
				RTCData.H          = 0;
			}
			else
			{
				RTCData.H          = changed_hr;
			}
			eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected);//write time format as soon as time format changes
		}
		else if(TimeF_selected == 2)
		{
			RTCData.Format     = R_RTCA_24H;
			RTCConfig.Format   = R_RTCA_24H;
			RTCData.H          = changed_hr;
			eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected);//write time format as soon as time format changes
		}

		if(AM_flag == 1)  //TAKE FLAG AS USED IN GUI.C
		{
			RTCData.AmPm       = R_RTCA_AM;
		}
		else
		{
			RTCData.AmPm       = R_RTCA_PM;
		}
		/*eeprom write AM flag*/
		eeprom_write(AMPM_FLAG_OFFSET,AM_flag);

		if(RTCData.Format == R_RTCA_24H)
		{
			time_format=0;			//24hr_mode
		}
		else if(RTCData.Format == R_RTCA_12H)
		{
			time_format=1;				//12hr_mode
		}
		if(Timesetting_completed == 1)
		{
			R_RTCA_CtrCtl(RTCA_UNIT, R_RTCA_DISABLE);
			R_RTCA_Init(RTCA_UNIT, &RTCConfig, &RTCData);
			R_RTCA_OutputCtl(RTCA_UNIT, R_RTCA_ENABLE);
			R_RTCA_SetInt(RTCA_UNIT, RTCIntConfig);
			R_RTCA_CtrCtl(RTCA_UNIT, R_RTCA_ENABLE);
			R_RTCA_UpdateTime(RTCA_UNIT,&RTCData);
		}
		RTCReadData=RTCData;

		Timesetting_completed = 0;
		TimeF_selection = 0;
	}

	R_RTCA_ReadCounterBuf(RTCA_UNIT, &RTCReadData);

	/* print out the current time every change */
	if(locSecondCount != locPreSecondCount)
	{
		/* get the latest time */
		R_RTCA_ReadCounterBuf(RTCA_UNIT, &RTCReadData);

//		R_BSP_STDIO_Printf("Time %02d:%02d:%02d, Date %02d.%02d.20%02d\n", RTCReadData.H,\
//				RTCReadData.Min, RTCReadData.Sec, RTCReadData.Day,\
				RTCReadData.Month, RTCReadData.Year); /*Due to printf interrupt disabled*/
		disp_sec=RTCReadData.Sec;
		disp_min=RTCReadData.Min;
		disp_hour=RTCReadData.H;
		disp_day=RTCReadData.Day;
		disp_month=RTCReadData.Month;
		disp_year=RTCReadData.Year;

		RTCA0WAIT = 1;
		*RRAM_hrs = RTCA0HOURC;
		*RRAM_min = RTCA0MINC;
		*RRAM_sec = RTCA0SECC;
		RTCA0WAIT = 0;
		/* update */
		locPreSecondCount = locSecondCount;
		/********Average speed counter for Trip A************/
		/*if(count_start == 1)
		{
			AVS_sec_count_A.u32++;
		}*/
		if(count_start_temp == 1)
		{
			AVS_sec_count_temp++;
		}

		/********Average speed counter for Trip B***********/
		/*if(count_start_B == 1)
		{
			AVS_sec_count_B.u32++;
		}*/
		if(count_start_temp_B == 1)
		{
			AVS_sec_count_temp_B++;
		}
		/*********Driving Time counter for Trip A & B*******/
		if(drive_count_start == 1)
		{
			DT_sec_count_A.u32++;
			DT_sec_count_B.u32++;
			AVS_sec_count_A.u32++;
			AVS_sec_count_B.u32++;
		}
		if(drivesum_count_start == 1)
		{
			DriveSumsec_count.u32++;
		}
		/* inform the state manager of a timer event */
		//eeStateManager_Processing(E_RTC_TICK);
	}
	else
	{
		/* nothing */
	}
}



