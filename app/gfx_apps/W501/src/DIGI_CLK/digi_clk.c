/*
 * digi_clk.c
 *
 *  Created on: 07-Mar-2019
 *      Author: MSWELT19
 */

#include "dr7f701461.dvf.h"
#include "r_typedefs.h"
#include "my_can.h"
#include "serial_to_parallel.h"
#include "parallel_to_serial.h"
#include "TLC6C_S_P.h"
#include "dbgmacro.h"
#include "errmgr.h"
#include "dr7f701461.dvf.h"
#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "sound_pcm.h"
#include "UDSS.h"
#include "r_cg_timer.h"
#include "r_gpio_api.h"
#include "ADC.h"
#include "batt.h"
#include "gui.h"
#include "drome.h"
#include "druserconf.h"
#include "sound_pcm.h"
#include "r_dev_api.h"
#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"
#include "r_gpio_api.h"
#include "r_taub_api.h"
#include "r_tick_api.h"
#include "gui.h"
#include "init.h"
#include "fuel.h"
#include "digi_clk.h"
#include "rtc.h"
#include "r_rtca_api.h"
#include "swsc.h"
#include "UDSS_APP.h"
#include "UDS_DTC.h"
#include "memmory_map.h"
#include "r_cg_serial_user.h"

uint8_t AM_flag,PM_flag;
uint8_t hour,minute;
uint8_t try_switching;
uint8_t changed_hr,changed_min;
extern _Bool Timesetting_completed;
_Bool twelve_hr_set;

static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
		unsigned char  Rx_4:1;
		unsigned char  Rx_5:1;
	}bits;
	unsigned char byte;

}five_bit_data;

static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
		unsigned char  Rx_4:1;
		unsigned char  Rx_5:1;
		unsigned char  Rx_6:1;
	}bits;
	unsigned char byte;

}six_bit_data;


void digi_clk_can(void)
{
	if((FEATURE_CODE_CLOCK_CONFIG==1) && (VARIANT_CODING_INFOTAINMENT_SYSTEM==1))
	{
		if(IS3_500_message_timeout_flag==0)
		{
			Dashes_colon = 0;
			five_bit_data.bits.Rx_1 = CAN_IS3_500[3].bits.Rx_1;
			five_bit_data.bits.Rx_2 = CAN_IS3_500[3].bits.Rx_2;
			five_bit_data.bits.Rx_3 = CAN_IS3_500[3].bits.Rx_3;
			five_bit_data.bits.Rx_4 = CAN_IS3_500[3].bits.Rx_4;
			five_bit_data.bits.Rx_5 = CAN_IS3_500[3].bits.Rx_5;

			if(CAN_IS3_500[1].bits.Rx_5==0)  /*12hr mode_default*/
			{
				if(five_bit_data.byte==0)
				{
					hour=12;
					AM_flag=1;
					PM_flag=0;
					/*Meridian = AM */
				}
				if(five_bit_data.byte < 12 && five_bit_data.byte!=0)
				{
					AM_flag=1;
					PM_flag=0;
					hour=five_bit_data.byte;
					/*Meridian = AM */
				}
				if(five_bit_data.byte == 12)
				{
					hour=five_bit_data.byte;
					/*Meridian = PM */
					AM_flag=0;
					PM_flag=1;
				}
				if(five_bit_data.byte > 12)
				{
					hour=five_bit_data.byte -12;
					/*Meridian = PM */
					AM_flag=0;
					PM_flag=1;
				}

			}

			else if(CAN_IS3_500[1].bits.Rx_5==1)/*24hr_mode*/
			{
					hour=five_bit_data.byte;
					AM_flag=2;

			}

			six_bit_data.bits.Rx_1 = CAN_IS3_500[4].bits.Rx_1;
			six_bit_data.bits.Rx_2 = CAN_IS3_500[4].bits.Rx_2;
			six_bit_data.bits.Rx_3 = CAN_IS3_500[4].bits.Rx_3;
			six_bit_data.bits.Rx_4 = CAN_IS3_500[4].bits.Rx_4;
			six_bit_data.bits.Rx_5 = CAN_IS3_500[4].bits.Rx_5;
			six_bit_data.bits.Rx_6 = CAN_IS3_500[4].bits.Rx_6;
			minute = six_bit_data.byte;
		}
		else
		{
			Dashes_colon = 1;/*colon shall be remain on without blink*/
		}
	}

	else if(FEATURE_CODE_CLOCK_CONFIG==0)
	{
		uint8_t temp;
		Dashes_colon = 0;
		temp=RTCA0HOURC;
#ifdef U350
		if(SET_key_press == 1)
		{
		 try_switching=1;
		}
#endif
		/*internal RTC*/
		/* R_BSP_STDIO_Printf("Time %02d:%02d:%02d, Date %02d.%02d.20%02d\n", disp_hour,\
				   disp_min,  disp_sec, disp_day,\
				   disp_month,disp_year);*/
		if(time_format==0)      //24hr_mode
		{
			if(disp_hour < 24)
			{
				hour=disp_hour;
			}
			minute=disp_min;
			AM_flag=2;

		}

		if(time_format==1 )      //12hr_mode
		{
			if(format_changed==1)
			{
				hour=disp_hour;
				minute=disp_min;
//				AM_flag=AM_flag;
				format_changed=0;
			}
			else if(format_changed==0)
			{
				if(disp_hour==0)
				{
//					AM_PM_chnage_flag = 1;
						hour=12;
						disp_hour = 12;

						/*if((AM_flag == 1)&&(AM_PM_chnage_flag == 1))
						{
							AM_flag = 0;
							AM_PM_chnage_flag++;
						}
						else if((AM_flag == 0)&&(AM_PM_chnage_flag == 1))
						{
							AM_flag = 1;
							AM_PM_chnage_flag++;
						}*/
				}
//				if(disp_hour!=0)
				else
				{
					hour=disp_hour;
					twelve_hr_set=0;
//					AM_PM_chnage_flag = 0;
					//AM_flag=1;
				}

				if((temp &(1<<5))==0x20)
				{
					if(AM_flag!=0)
					{
						AM_flag=0;
						/*eeprom write AM flag*/
						eeprom_write(AMPM_FLAG_OFFSET,AM_flag);
					}
				}
				else
				{
					if(AM_flag!=1)
					{
						AM_flag=1;
						/*eeprom write AM flag*/
						eeprom_write(AMPM_FLAG_OFFSET,AM_flag);
					}

				}


				minute=disp_min;
				AM_flag=AM_flag;
			}
		}

	}

	else
	{
		/*colon shall be remain on without blink*/
	}
}
