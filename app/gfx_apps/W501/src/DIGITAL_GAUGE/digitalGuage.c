/* **********************************************************************************************************************
File                    :       digitalGuage.c
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */

/* =========================================================================
 Includes
============================================================================ */
#include "init.h"
#include "r_typedefs.h"
#include "digitalGuage.h"
#include "UDSS.h"
#include "my_can.h"
#include "batt.h"
#include "gui.h"
#include "drome.h"
#include "druserconf.h"
#include "odometer.h"
#include "AFE_3.h"
#include "dr7f701461.dvf.h"
#include "UDSS_APP.h"
#include "swsc.h"
#include "Memmory_Map.h"
#include "r_cg_serial_user.h"
#include "UDS_DTC.h"
enum
{
	Temp_Jump,
	Temp_Return,
	Temp_NONE
};

SplitWord down_scale;

typedef union
{
	uint32_t w;
	uint8_t  b[4];
}SplitWord1;
typedef union
{
   U32 u32;
   U16 u16[2];
   U8  u8[4];
} ODO32;
/* *********************************************************************************************** */
/* Variable Declaration */
/*km/mile selection*/
uint8_t Dist_Unit;// = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[7];								/*Distance Unit - km / mile*/

/* speedo */
uint16_t final_vechile_speed = 0U;
uint16_t final_vechile_speed_gauge_raw = 0U;
SplitWord1 previous_odo;
/* odo */
uint32_t previous_odometer_distance = 0U;
uint32_t final_odomter_distance = 0U;
/***************** Drive Summery *********************/
uint32_t Final_Drivesum_odo_distance = 0U;
uint32_t Final_Drivesum_2WD_distance = 0U;
uint32_t Init_2WD_odo_distance = 0U;
uint32_t Prev_2WD_odo_distance = 0U;
uint32_t Final_Drivesum_DT_sec_count = 0U;
uint16_t DSum_Display_Hr = 0;
uint16_t DSum_Display_Min = 0;
uint32_t Dsum_Avg_vehicle_speed = 0U;
uint32_t Dsum_Display_Average_speed = 0;
extern uint32_t Init_Drivesum_odo_distance;
extern uint32_t Init_Drivesum_DT_sec_count;
extern _Bool Flag_AFE_reset;
/*****************************************************/


/*odo Error*/
uint8_t ODO_ERROR_DTC = 0;
uint8_t odo_error_flag = 0;

_Bool Speed_Missing=0;
uint32_t old_odometer_distance = 0;
uint32_t display_speedometer_speed = 0;
uint8_t odo_flag = 1U;
uint16_t odo_increment = 0;
uint8_t odo_65_complete = 1;
uint8_t a=0,b=0;
uint32_t Send_odometer_data=0;
/* trip_A */
uint32_t previous_tripmeter_distance  = 0U;
uint32_t final_tripmeter_distance     = 0U;
/************************************************/
uint32_t Accumulated_distance_A = 0U;
uint32_t Accumulated_distance_B = 0U;
uint32_t init_Accumulated_distance_A = 0U;
uint32_t init_Accumulated_distance_B = 0U;
uint16_t Display_Average_speed_A = 0;
uint16_t Display_Average_speed_B = 0;
uint64_t Avg_vehicle_speed1 = 0,Avg_vehicle_speed2 = 0;
//uint8_t AVS_sample_sec_A;
uint32_t AVS_sample_sec_A1;
//uint8_t AVS_sample_sec_B;
extern uint8_t  DID_AVG_SPEED_0x0511_arr[];
extern uint8_t Diag_RW_Min_VehSpeed_Threshold;

uint8_t HMI_AVS_DASH1 = 0;
uint8_t HMI_AVS_DASH2 = 0;
uint8_t AVS_SAMPLE_TIME = 0;
uint8_t AVS_initial_cnt_A = 1;
uint8_t AVS_initial_cnt_B = 1;
uint16_t AVS_INITIAL_SAMPLE_DISTANCE = 0;
uint16_t Display_Hr_A = 0;
uint16_t Display_Min_A = 0;
uint16_t Display_Hr_B = 0;
uint16_t Display_Min_B = 0;
/******************************/
Split32 AVS_sec_count_A = 0;
uint8_t count_start = 0;
uint8_t count_start_temp = 0;
uint8_t count_start_temp_B = 0;
uint8_t AVS_sec_count_temp = 0;
Split32 AVS_sec_count_B = 0;
Split32 DT_sec_count_A = 0;
Split32 DT_sec_count_B = 0;
uint8_t count_start_B = 0;
uint8_t AVS_sec_count_temp_B = 0;
uint8_t drive_count_start = 0;
uint8_t drivesum_count_start = 0;
uint8_t Initial_entry_A = 1;
uint8_t Initial_entry_B = 1;
/************************************************/
uint8_t previous_trip1 = 0;
uint8_t previous_trip2 = 0;
uint8_t trip_flag = 1U;
uint16_t trip_increment = 1;
uint32_t after_dec_val = 0;//uint8_t
uint8_t previous_dec_val = 0;
uint16_t over_flag = 0;

/* AC trip */
uint32_t final_ACtripmeter_distance = 0U;
uint32_t final_ACtripmeter_distance1 = 0U;
uint32_t previous_odo_distance      = 0U; /* used for AC tripmeter */
uint32_t display_ACtripmeter_speed  = 0U;
uint8_t previous_ACtrip1 = 0;
uint8_t previous_ACtrip2 = 0;
uint8_t AC_Trip_1min_ON_flag = 0;
uint32_t Counter_1min        = 0;
uint32_t previous_ACtrip = 0;
uint8_t sts_ac_compressor_flag = 0;
int32_t TRIP_AC_OFFSET = 0;
uint8_t ACtrip_flag = 1U;
uint8_t speed_absent_flag;
uint8_t from_speed_zero = 0;
uint8_t init_odo;
uint32_t Initial_odo;
uint32_t ac_off_odo_run;
#ifdef GRAYSCALE_TFT
#define Trip_AC_1min_Count 410000
#endif
#ifdef COLOR_TFT
#define Trip_AC_1min_Count 530000
#endif

_Bool AC_ON;
_Bool speed;
_Bool copy_odo = 0;
_Bool vehicle_moving_state=0;
_Bool Speed_Invalid_Default = 0;
_Bool Dist_Invalid_Default = 0;
_Bool Trip_AC_Rollover = 0;

#if 0//commented because values getting from UDS
/* temp ON bar value used for Engine Coolant Temperature */
uint8_t TEMP_BAR_1_ON_VALUE = 10;
uint8_t TEMP_BAR_2_ON_VALUE = 50;
uint8_t TEMP_BAR_3_ON_VALUE = 70;
uint8_t TEMP_BAR_4_ON_VALUE = 90;
uint8_t TEMP_BAR_5_ON_VALUE = 101;
uint8_t TEMP_BAR_6_ON_VALUE = 106;
uint8_t TEMP_BAR_7_ON_VALUE = 110;
uint8_t TEMP_BAR_8_ON_VALUE = 114;

/*Temp bar off*/
uint8_t TEMP_BAR_1_OFF_VALUE = 6;
uint8_t TEMP_BAR_2_OFF_VALUE = 46;
uint8_t TEMP_BAR_3_OFF_VALUE = 66;
uint8_t TEMP_BAR_4_OFF_VALUE = 86;
uint8_t TEMP_BAR_5_OFF_VALUE = 97;
uint8_t TEMP_BAR_6_OFF_VALUE = 102;
uint8_t TEMP_BAR_7_OFF_VALUE = 106;
uint8_t TEMP_BAR_8_OFF_VALUE = 110;
#endif

uint8_t segment1 = 0U;
uint8_t segment2 = 0U;
uint8_t segment3 = 0U;
uint8_t segment4 = 0U;
uint8_t segment5 = 0U;
uint8_t segment6 = 0U;
uint8_t segment7 = 0U;
uint8_t segment8 = 0U;
uint8_t pre_temp_val = 0;
uint8_t new_temp_val = 0;
uint8_t new_temp_value = 0;
uint8_t pre_temp_value = 0;
uint8_t temp_widget = 0;
uint8_t blink_temp = 0;//detection of blinking
uint8_t temperature_value = 2;
_Bool IGN_temp_0 = 1;

/* trip_B */
uint32_t previous_tripmeter_B_distance  = 0U;
uint32_t final_tripmeter_B_distance     = 0U;
uint8_t previous_trip_B1 = 0;
uint8_t previous_trip_B2 = 0;
uint8_t trip_B_flag = 1U;
uint16_t trip_B_increment = 1;
uint8_t after_dec_val_B = 0;
uint8_t previous_dec_val_B = 0;
uint16_t over_flag_B = 0;

/* reset */
uint8_t odometer_reset_flag           = 0U;
uint8_t odometer_reset_flag_1           = 0U;
int8_t Engine_Speed ;
/*************/
uint8_t Temp_blinkpattern_flag;
uint8_t Temp_blinkpattern_flag_7;
uint8_t Temp_blinkpattern_flag_8;
/**************************************************************************************************/
Split32 DriveSumsec_count = 0;
_Bool Init_DT_flag = 0;
extern uint8_t U8_Reset_DTE;
//extern uint8_t	invalid_ODO_input;
/**************************************************************************************************/

/* Function Declaration */
void speedometer(void);
void odometer(void);
void tripmeter(void);
void AC_tripmeter(void);
void Engine_Coolant_Temperature(void);
void tripmeter_B(void);

void reset_odometer(void);
void reset_tripmeterA(void);
void reset_tripmeterB(void);
void reset_AC_tripmeter(void);
void Average_vehicle_speed(void);
void drive_time(void);
void Drive_summery(void);

/* *********************************************************************************************** */
//uint8_t LVSD_flag = 0; /* Need to remove after adding batt.h file */
//uint8_t HVSD_flag = 0; /* Need to remove after adding batt.h file */
//uint8_t FEATURE_CODE_SPEED_INPUT = 0;		/*defined in UDSS.h*/
//uint8_t VAR_CODING_ELECTRONICS_STABILITY_CONTROL  = 0U;		/*defined in UDSS.h*/
//uint8_t VAR_CODING_ANTILOCK_BRAKING_SYSTEM        = 1U;		/*defined in UDSS.h*/
//uint8_t FEATURE_CODE_AC_TRIP_METER = 1;
/* *********************************************************************************************** */

/* ***********************************************************************************************
 * Function Name: speedometer
 * Description  : This function used for speedometer of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void speedometer(void)
{
	_Bool Veh_speed_missing=0;
	down_scale.w=0;


	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			//			if(FEATURE_CODE_DIGITAL_SPEED_CONFIG  == 1U)
			//			{
			if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==0 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==1)||// && FEATURE_CODE_DIGITAL_SPEED_CONFIG==1)||
					(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1&& VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0))// && FEATURE_CODE_DIGITAL_SPEED_CONFIG==1))
			{
				if(((FEATURE_CODE_SPEED_INPUT ==0) && (ESC2_10_message_timeout_flag != 0)) ||
						((FEATURE_CODE_SPEED_INPUT ==1 )&&(ESC12_10_message_timeout_flag != 0)))
				{
					Veh_speed_missing = 1;
					Speed_Missing=1;
				}
				else
				{
					Veh_speed_missing = 0;
					Speed_Missing=0;
				}

				if(Veh_speed_missing == 0)
				{
					switch(FEATURE_CODE_SPEED_INPUT)
					{
					case 0:
						/* ESC2_10 message */
						down_scale.b[0] = CAN_ESC2_10[0].byte;
						down_scale.b[1] = CAN_ESC2_10[1].byte;

						if((down_scale.w == 0xFFFE) || (down_scale.w == 0xFFFF))
						{
							//Failure_DTC_Set(DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013);
							Speed_Invalid_Default = 1;
							vehicle_moving_state=0;
						}
						else
						{
							//Failure_DTC_Clear(DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013);
							Speed_Invalid_Default = 0;
						}

						final_vechile_speed = (uint16_t) (down_scale.w * 0.015625);
						final_vechile_speed_gauge_raw = down_scale.w;

						if((display_speedometer_speed >= Diag_RW_Speed_Min_for_Chime_activation /*Diag_RW_Min_VehSpeed_Threshold*/) && (down_scale.w < 0x4B01))/*(down_scale.w != 0xFFFE)&& (down_scale.w != 0xFFFF))*/
						{
							vehicle_moving_state=1;
						}
						else
						{
							vehicle_moving_state=0;
						}
						break;

					case 1:
						/* ESC12_10 message */
						down_scale.b[0] = CAN_ESC12_10[2].byte;
						down_scale.b[1] = CAN_ESC12_10[3].byte;

						if((down_scale.w == 0xFFFE) || (down_scale.w == 0xFFFF))
						{
							Failure_DTC_Set(DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013);
							Speed_Invalid_Default = 1;
							vehicle_moving_state=0;
						}
						else
						{
							Failure_DTC_Clear(DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013);
							Speed_Invalid_Default = 0;
						}


						final_vechile_speed =(uint16_t)  (down_scale.w * 0.015625);
						final_vechile_speed_gauge_raw = down_scale.w;

						if((display_speedometer_speed >= Diag_RW_Speed_Min_for_Chime_activation /*Diag_RW_Min_VehSpeed_Threshold*/) && (down_scale.w < 0x4B01))/*(down_scale.w != 0xFFFE)&& (down_scale.w != 0xFFFF))*/
						{
							vehicle_moving_state=1;
						}
						else
						{
							vehicle_moving_state=0;
						}

						break;

					default:
						/* nothing */
						break;
					}

					/* checking vechile speed and displaying respective speed */


					//if(final_vechile_speed <= 1) /* 1=0x0040 */
					//{
					/* indicate 0 kmph on display */
					//	display_speedometer_speed = 0U;
					//}
					//else
					//{
					display_speedometer_speed=(ISM0VAR4CFG0*100)/(254 * 153 * 64);
					//}
#if 0
					else if((final_vechile_speed >= (2)) && (final_vechile_speed < (10))) /* 2=0x0080, 10=0x0280 */
					{
						/* indicate actual speed on display as per calculation */
						display_speedometer_speed = final_vechile_speed;
					}

					else if((final_vechile_speed >= (10)) && (final_vechile_speed < (130))) /* 10=0x0080, 130= 0x2080 */
					{
						/* indicate actual speed on display as per calculation */
						display_speedometer_speed = final_vechile_speed + 3;
					}

					else if((final_vechile_speed >= (130)) && (final_vechile_speed <= (180))) /*130=0x2080, 180=0x2D00 */
					{
						/* indicate actual speed on display as per calculation */
						display_speedometer_speed = final_vechile_speed + 4;
					}

					else if((final_vechile_speed > 180) && (final_vechile_speed_gauge_raw < 0x4B01)) /* 180=0x2D00, 300=0x4B00 */
					{
						/* for >180 speed displayed 180 kmph */
						display_speedometer_speed = 184;
					}

					//					else if(final_vechile_speed_gauge_raw > 0x4B00) /* 300=0x4B00 */
					//					{
					//						display_speedometer_speed = 184;
					//					}
					//
					else
					{
						/* nothing */
					}
#endif
				}
				else if(Veh_speed_missing == 1)
				{
					display_speedometer_speed=176;
					final_vechile_speed_gauge_raw=175*64;//176*64;
					vehicle_moving_state=0;
					final_vechile_speed = 0;//For IPTL
//					display_speedometer_speed = 180;
//					vehicle_moving_state=0;
//					final_vechile_speed_gauge_raw = 180*64;
				}
				else
				{
					/* nothing */
				}
			}
			else
			{
				/* disable digital speedometer */
				display_speedometer_speed = 0;
				vehicle_moving_state=0;
				final_vechile_speed_gauge_raw=0;
			}
		}
		else
		{
			/* disable digital speedometer */
			display_speedometer_speed = 0;
			vehicle_moving_state=0;
			final_vechile_speed_gauge_raw=0;
		}
	}
	/*else*/
	{
		/* display off */
		/*display_speedometer_speed = 0;*/
	}

	down_scale.b[1]=  (display_speedometer_speed) & (0xFF00);
	down_scale.b[0]=  (display_speedometer_speed) & (0x00FF);
}



/* ***********************************************************************************************
 * Function Name: odometer
 * Description  : This function used for odometer of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void odometer(void)
{
	SplitWord1 down_scale1;

	uint32_t display_odometer_speed;
	uint8_t j = 0;
	ODO32 DID_ODO;
	DID_ODO.u32 = 0x00;			//Clear DID_ODO union

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if( ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1))
					|| ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0)) )
			{
				if(((FEATURE_CODE_SPEED_INPUT ==0) && (ESC2_10_message_timeout_flag == 0)) ||
						((FEATURE_CODE_SPEED_INPUT ==1 )&&(ESC12_10_message_timeout_flag == 0)))
				{
					switch(FEATURE_CODE_SPEED_INPUT)
					{
					case 0:
						/* ESC2_10 message */
						down_scale1.w       = 0x00; /* clearing previous stored value */
						down_scale1.b[0]    = CAN_ESC2_10[2].byte; /* copied 2nd byte data*/
						down_scale1.b[1]    = CAN_ESC2_10[3].byte; /* copied 3rd byte data*/
						//down_scale1.w = (down_scale1.w) / (1000);

						if((down_scale1.w == 0xFFFF))
						{
							//Failure_DTC_Set(DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014);
							Dist_Invalid_Default = 1;
						}
						else
						{
							//Failure_DTC_Clear(DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014);
							Dist_Invalid_Default = 0;
						}

						break;

					case 1:
						/* ESC12_10 message */
						down_scale1.w     = 0x00; /* clearing previous stored value */
						down_scale1.b[0]  = CAN_ESC12_10[0].byte; /* copied 0th byte data*/
						down_scale1.b[1]  = CAN_ESC12_10[1].byte; /* copied 1st byte data*/
						//down_scale1.w = (down_scale1.w) / (1000);

						if((down_scale1.w == 0xFFFF))
						{
							Dist_Invalid_Default = 1;
//							Failure_DTC_Set(DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014);
						}
						else
						{
							Dist_Invalid_Default = 0;
//							Failure_DTC_Clear(DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014);
						}

						break;

					default:
						/* nothing */
						break;
					}

					if((down_scale1.w-previous_odo.w)<3)
					{
						final_odomter_distance+=down_scale1.w-previous_odo.w;
					}
					else
					{
						final_odomter_distance+=2;
					}
					if(final_odomter_distance>999999000)
					{
						final_odomter_distance=999999000;
					}
					previous_odo.w=down_scale1.w;

					if(final_odomter_distance>old_odometer_distance)
					{
						if((final_odomter_distance-old_odometer_distance)>100)/*write odo on 100m*/
						{
							eeprom_writeOdometer(final_odomter_distance);
							old_odometer_distance = final_odomter_distance;
						}

					}
					if((CAN_EMS3_10[1].bits.Rx_7 == 1)&&(EMS3_10_message_timeout_flag == 0))
					{
						odo_error_flag = 1;
						ODO_ERROR_DTC = 1;
					}
					else if((CAN_EMS3_10[1].bits.Rx_7 == 0)&&(EMS3_10_message_timeout_flag == 0))
					{
						ODO_ERROR_DTC = 0;
						odo_error_flag = 0;
					}

					DID_ODO.u32 = final_odomter_distance/1000;
					for(uint8_t k=0;k<3;k++)
					{
						DID_ODOMETER_0xF1B0_read_arr[k] = 0;			//Clear array before updating current ODO.
					}
					for(uint8_t i=3;i>=1;i--)
					{
//r						if(DID_ODO.u8[i-1] != 0)
						{
							DID_ODOMETER_0xF1B0_read_arr[j] = DID_ODO.u8[i-1];
							j++;
						}
					}

#if 0
					for(uint8_t i=0;i<3;i++)
					{
					 DID_ODOMETER_0xF1B0_arr[i] = DID_ODO.u8[i];
					}
#endif

#if 0
					if((a != down_scale1.b[0]) || (b != down_scale1.b[1]) ) //checking previous & current value
					{
						a = down_scale1.b[0];
						b = down_scale1.b[1];

						//down_scale1.w = (down_scale1.w) / (1000);
						//if(final_odomter_distance == (65 * odo_65_complete))
						if(down_scale1.w == 65535)
						{
							//odo_65_complete++;
							odo_increment++;
						}
						if(final_odomter_distance == 999999)
						{
							odo_flag = 2;
						}


						if((down_scale1.w < 65535 ) && (odo_flag == 1)) /* checking odo value is < 65535 m */
						{
							if((previous_odometer_distance <= down_scale1.w)) /* if previous odo value < current odo value */
							{
								/* displaying current odo value */
								down_scale1.b[3] = 0x00;
								down_scale1.b[2] = 0x00;
								previous_odometer_distance = down_scale1.w;
							}
							else if((previous_odometer_distance > down_scale1.w)) /* if previous odo value > current odo value */
							{
								/* displaying previous odo value */
								down_scale1.w = previous_odometer_distance;
								down_scale1.b[3] = 0x00;
								down_scale1.b[2] = 0x00;
							}
						}
						else if(((down_scale1.w >= 65535 ) && (down_scale1.w <= 999999)) || (odo_flag == 0)) /* if odo value is > 65535m */
						{
							odo_flag=0;

							if((down_scale1.w == 65535 ))
							{
								previous_odometer_distance = 0x00;
								down_scale1.w = 65535 * odo_increment;
							}
							else if((previous_odometer_distance <= down_scale1.w))
							{
								/* displaying (current odo value + (65535 * odo_increment)) */
								down_scale1.b[3] = 0x00;
								down_scale1.b[2] = 0x00;
								previous_odometer_distance = down_scale1.w;
								down_scale1.w = (65535 * odo_increment) + down_scale1.w;
							}
							else if((previous_odometer_distance > down_scale1.w))
							{
								/* displaying (previous odo value + (65535 * odo_increment)) */
								down_scale1.w = (65535 * odo_increment) + previous_odometer_distance;
								//down_scale1.b[3] = 0x00;
								//down_scale1.b[2] = 0x00;
							}
						}
						else if((final_odomter_distance == 999999) && (odo_flag == 2)) /* if odo value is 999999 */
						{
							/* displaying 999999km */
							down_scale1.w = (999999*1000);
						}
						down_scale1.w = (down_scale1.w) / (1000);

						if(final_odomter_distance < (down_scale1.w))
						{
							final_odomter_distance = down_scale1.w;
						}

						if(final_odomter_distance>old_odometer_distance)
						{
							if((final_odomter_distance-old_odometer_distance)>10)
							{
								eeprom_writeOdometer(final_odomter_distance);
								eeprom_write_Trip1_Odometer(final_tripmeter_distance);
								eeprom_write_Trip2_Odometer(final_tripmeter_B_distance);
								old_odometer_distance = final_odomter_distance;
							}

						}

					}
#endif

				}
				else
				{
					//if msg is missed don't increment ODO.
				}
			}
			else
			{
				/* nothing*/
			}
		}
		else
		{
			/* disable digital odometer */
		}
	}
	else
	{
		/* display off */
	}

	//final_odomter_distance = down_scale1.w;
}


/* ***********************************************************************************************
 * Function Name: tripmeter
 * Description  : This function used for tripmeter of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void tripmeter(void)//trip A
{
	SplitWord1 down_scale_trip;
	uint32_t display_tripmeter_speed = 0U;

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if( ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1))
					|| ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0)) )
			{
				switch(FEATURE_CODE_SPEED_INPUT)
				{
				case 0:
					/* ESC2_10 message */
					down_scale_trip.w    = 0x00; /* clearing previous stored value */
					down_scale_trip.b[0] = CAN_ESC2_10[2].byte; /* copied 2nd byte data*/
					down_scale_trip.b[1] = CAN_ESC2_10[3].byte; /* copied 3rd byte data*/

					break;

				case 1:
					/* ESC12_10 message */
					down_scale_trip.w    = 0x00; /* clearing previous stored value */
					down_scale_trip.b[0] = CAN_ESC12_10[0].byte; /* copied 1st byte data*/
					down_scale_trip.b[1] = CAN_ESC12_10[1].byte; /* copied 2nd byte data*/

					break;

				default:
					/* nothing */
					break;
				}


				if((previous_trip1 != down_scale_trip.b[0]) || (previous_trip2 != down_scale_trip.b[1]) ) //checking previous & current value
				{
					previous_trip1 = down_scale_trip.b[0];
					previous_trip2 = down_scale_trip.b[1];

					//after_dec_val = (( (down_scale_trip.w) % (1000) ) / (100));
					//down_scale_trip.w = ((down_scale_trip.w) / (1000));

					if(final_tripmeter_distance == 9999) /* final trip value == 9999km */
					{
						trip_flag = 2;
					}

					if((down_scale_trip.w < 65535 ) && (trip_flag == 1)) /* checking trip value is < 65535 m */
					{
						if((previous_tripmeter_distance <= down_scale_trip.w)) /* if previous trip value < current trip value */
						{
							previous_tripmeter_distance = down_scale_trip.w;
						}
						else if((previous_tripmeter_distance > down_scale_trip.w)) /* if previous trip value > current trip value */
						{
							down_scale_trip.w = previous_tripmeter_distance;
						}

					}
					else if(((down_scale_trip.w == 65535 ) && (final_tripmeter_distance < 9999)) || (trip_flag == 0)) /* if trip value is >= 65535m */
					{
						trip_flag=0;

						if((down_scale_trip.w == 65535 ))
						{
							over_flag++; // after every 65535m this variable is incremented
							if(over_flag >= 2)
							{
								trip_increment++;
							}

							previous_tripmeter_distance = 0x00;
							down_scale_trip.w = 65535 * trip_increment;

						}
						else if((previous_tripmeter_distance <= down_scale_trip.w))
						{
							/* displaying (current odo value + (65535 * trip_increment)) */
							down_scale_trip.b[3] = 0x00;
							down_scale_trip.b[2] = 0x00;

							previous_tripmeter_distance = down_scale_trip.w;
							down_scale_trip.w = (65535 * trip_increment) + down_scale_trip.w;
						}
						else if((previous_tripmeter_distance > down_scale_trip.w))
						{
							/* displaying (previous odo value + (65 * trip_increment)) */
							down_scale_trip.w = (65535 * trip_increment) + previous_tripmeter_distance;
						}
					}
					else if((final_tripmeter_distance == 9999) && (trip_flag == 2)) /* if odo value is 999999 */
					{
						down_scale_trip.w = 0;
					}

					after_dec_val = (( (down_scale_trip.w) % (1000) ) / (100)); // this variable gives after decimal value i.e. from 0 to 9
					down_scale_trip.w = ((down_scale_trip.w) / (100));

					if(final_tripmeter_distance < (down_scale_trip.w))
					{
						final_tripmeter_distance = down_scale_trip.w;
					}

				}

			}
			else
			{
				/* nothing*/
			}
		}
		else
		{
			/* disable digital tripmeter */
		}
	}
	else
	{
		/* display off */
	}

}


/* ***********************************************************************************************
 * Function Name: ACtripmeter
 * Description  : This function used for AC trip meter of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void AC_tripmeter(void)
{
	SplitWord1 down_scale_ACtrip;


	uint32_t current_odo_distance      = 0U;

	//	sts_ac_compressor_flag = CAN_ESC2_10[5].byte; /* temporarily used, need to be change according to DBC */
	if(EMS1_10_message_timeout_flag == 0)
	{
	sts_ac_compressor_flag = CAN_EMS1_10[6].byte;//.bits.Rx_1; /* we check only last bit  */
	sts_ac_compressor_flag = sts_ac_compressor_flag & (0x03);
	}
	else
	{
		sts_ac_compressor_flag = 0;
	}
	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if( (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1U) && (FEATURE_CODE_AC_TRIP_METER == 1U) )
			{
//				if(((ESC2_10_message_timeout_flag == 0)||(ESC12_10_message_timeout_flag == 0))&&(EMS1_10_message_timeout_flag == 0)) //not required flag position changed
				{
					switch(FEATURE_CODE_SPEED_INPUT)
					{
					case 0:
						/* ESC2_10 message */
						down_scale_ACtrip.w    = 0x00; /* clearing previous stored value */
						down_scale_ACtrip.b[0] = CAN_ESC2_10[2].byte; /* copied 2nd byte data*/
						down_scale_ACtrip.b[1] = CAN_ESC2_10[3].byte; /* copied 3rd byte data*/

						current_odo_distance = down_scale_ACtrip.w; /* stored current_odo value */

						break;

					case 1:
						/* ESC12_10 message */
						down_scale_ACtrip.w    = 0x00; /* clearing previous stored value */
						down_scale_ACtrip.b[0] = CAN_ESC12_10[0].byte; /* copied 0th byte data*/
						down_scale_ACtrip.b[1] = CAN_ESC12_10[1].byte; /* copied 1st byte data*/

						current_odo_distance = down_scale_ACtrip.w; /* stored current_odo value */

#if 0
						if((sts_ac_compressor_flag == 0x1) && (((CAN_ESC12_10[2].byte)!=0) || ((CAN_ESC12_10[3].byte)!=0)))
						{
							//final_ACtripmeter_distance = (current_odo_distance - previous_odo_distance) + final_ACtripmeter_distance;
							AC_Trip_1min_ON_flag = 0;
						}
						else if((sts_ac_compressor_flag == 0x1) && (((CAN_ESC12_10[2].byte) == 0) || ((CAN_ESC12_10[3].byte) == 0)))
						{
							AC_Trip_1min_ON_flag = 1;
						}
						else
						{
							AC_Trip_1min_ON_flag = 0;
							/* AC trip should not increment */
						}
#endif

						break;

					default:
						/* nothing */
						break;
					}


#if 0
					if((previous_ACtrip1 != down_scale_ACtrip.b[0]) || (previous_ACtrip2 != down_scale_ACtrip.b[1]) ) //checking previous & current value
					{
						if((sts_ac_compressor_flag == 0x1) && ( ((CAN_ESC2_10[0].byte)!=0) || ((CAN_ESC2_10[1].byte)!=0) || ((CAN_ESC12_10[2].byte)!=0) || ((CAN_ESC12_10[3].byte)!=0) ) )
						{
							AC_ON = 1;
							speed = 1;
							/*If speed present and AC on*/
							previous_ACtrip1 = down_scale_ACtrip.b[0];
							previous_ACtrip2 = down_scale_ACtrip.b[1];

							//final_ACtripmeter_distance = (current_odo_distance - previous_odo_distance) + final_ACtripmeter_distance;
							AC_Trip_1min_ON_flag = 0;

							if((down_scale_ACtrip.w < 65535 ) && (ACtrip_flag == 1)) /* checking ACtrip value is < 65535 m */
							{
								if((previous_ACtrip <= down_scale_ACtrip.w)) /* if previous trip value < current trip value */
								{
									//previous_ACtrip = down_scale_ACtrip.w;
									down_scale_ACtrip.w = ((down_scale_ACtrip.w) - previous_ACtrip);
									previous_ACtrip     = current_odo_distance;
								}
								else if((previous_ACtrip > down_scale_ACtrip.w)) /* if previous trip value > current trip value */
								{
									down_scale_ACtrip.w = previous_ACtrip;
									down_scale_ACtrip.w = ((down_scale_ACtrip.w) - previous_ACtrip);
								}
							}

							else if((down_scale_ACtrip.w == 65535 ) || (ACtrip_flag == 0)) /* if trip value is >= 65535m */
							{
								ACtrip_flag=0;

								if((down_scale_ACtrip.w == 65535 ))
								{
									down_scale_ACtrip.w = ((down_scale_ACtrip.w) - previous_ACtrip);
									previous_ACtrip = 0x00;
								}
								else if((previous_ACtrip <= down_scale_ACtrip.w))
								{
									/* displaying (current odo value + (65535 * trip_increment)) */
									down_scale_ACtrip.b[3] = 0x00;
									down_scale_ACtrip.b[2] = 0x00;

									down_scale_ACtrip.w = (down_scale_ACtrip.w) - previous_ACtrip;
									previous_ACtrip     = current_odo_distance;
								}
								else if((previous_ACtrip > down_scale_ACtrip.w))
								{
									/* displaying (previous odo value + (65 * trip_increment)) */
									down_scale_ACtrip.w = previous_ACtrip;
									down_scale_ACtrip.w = (down_scale_ACtrip.w - previous_ACtrip);
								}
							}

							else if(display_ACtripmeter_speed == 9999) /* if odo value is 9999km */
							{
								down_scale_ACtrip.w = 0;
								final_ACtripmeter_distance = (9999 * 1000);
							}

							final_ACtripmeter_distance = down_scale_ACtrip.w + final_ACtripmeter_distance;
							display_ACtripmeter_speed  = (final_ACtripmeter_distance / 100);//r 1000
						}

						else if(sts_ac_compressor_flag == 0x0)
						{
							/*If AC off*/
							previous_ACtrip1 = down_scale_ACtrip.b[0];
							previous_ACtrip2 = down_scale_ACtrip.b[1];

							/****************************************/
							AC_offset  = current_odo_distance - previous_ACtrip;//Currunt ODO value (AC Off) - ODO value (AC On)
							final_ACtripmeter_distance = down_scale_ACtrip.w + final_ACtripmeter_distance;
							final_ACtripmeter_distance  = (final_ACtripmeter_distance - AC_offset);//r 1000

						}
						if(FEATURE_CODE_SPEED_INPUT == 0)
						{
							/*If speed not present and AC on*/
							if((sts_ac_compressor_flag == 0x1) && ((CAN_ESC2_10[0].byte) == 0) && ((CAN_ESC2_10[1].byte) == 0) )
							{
								AC_ON = 1;
								speed = 0;
								AC_Trip_1min_ON_flag = 1;
							}

							else if(sts_ac_compressor_flag == 0x0) /* ACtrip is off */
							{
								AC_ON = 0;
								AC_Trip_1min_ON_flag = 0;
							}
						}
						if(FEATURE_CODE_SPEED_INPUT == 1)
						{
							/*If speed not present and AC on*/
							if((sts_ac_compressor_flag == 0x1) &&  ((CAN_ESC12_10[2].byte)==0) && ((CAN_ESC12_10[3].byte)==0))
							{
								AC_ON = 1;
								speed = 0;
								AC_Trip_1min_ON_flag = 1;
							}

							else if(sts_ac_compressor_flag == 0x0) /* ACtrip is off */
							{
								AC_ON = 0;
								AC_Trip_1min_ON_flag = 0;
							}
						}


					}
#endif
					/*If speed not present and AC on*/
					if((FEATURE_CODE_SPEED_INPUT == 0)&&(ESC2_10_message_timeout_flag == 0))
					{
						/*If speed not present and AC on*/
						if((sts_ac_compressor_flag == 0x1) && ((CAN_ESC2_10[0].byte) == 0) && ((CAN_ESC2_10[1].byte) == 0) )
						{
							AC_ON = 1;
							speed = 0;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 1;
						}
						/*If speed not present and AC off*/
						else if((sts_ac_compressor_flag != 0x1) && ((CAN_ESC2_10[0].byte) == 0) && ((CAN_ESC2_10[1].byte) == 0) )
						{
							AC_ON = 0;
							speed = 0;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 1;
						}
						/*speed present and AC off */
						else if((sts_ac_compressor_flag != 0x1) &&  (((CAN_ESC2_10[0].byte)!=0) || ((CAN_ESC2_10[1].byte)!=0))) /* ACtrip is off */
						{
							AC_ON = 0;
							speed = 1;
							if(copy_odo == 0)
							{
								copy_odo = 1;
								Initial_odo = final_odomter_distance;
								final_ACtripmeter_distance = final_ACtripmeter_distance1;
							}
							//						ac_off_odo_run = final_odomter_distance - Initial_odo;
							final_ACtripmeter_distance1 = final_ACtripmeter_distance + (final_odomter_distance - Initial_odo);//ac_off_odo_run;
							AC_Trip_1min_ON_flag = 1;//Not to reset Timer In Traffic scenarios
						}
						/*Speed present and AC ON*/
						else if((sts_ac_compressor_flag == 0x1) &&  (((CAN_ESC2_10[0].byte)!=0) || ((CAN_ESC2_10[1].byte)!=0)))
						{
							AC_ON = 1;
							speed = 1;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 1;
						}
						else
						{
							/*if odo is incrementing AC off and speed is 0*/
							AC_ON = 0;
							speed = 1;
							if(copy_odo == 0)
							{
								copy_odo = 1;
								Initial_odo = final_odomter_distance;
								final_ACtripmeter_distance = final_ACtripmeter_distance1;
							}
							//						ac_off_odo_run = final_odomter_distance - Initial_odo;
							final_ACtripmeter_distance1 = final_ACtripmeter_distance + (final_odomter_distance - Initial_odo);//ac_off_odo_run;
							AC_Trip_1min_ON_flag = 0;
							/*
							AC_ON = 0;
							speed = 0;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 0;
							*/
						}

					}
					if((FEATURE_CODE_SPEED_INPUT == 1)&&(ESC12_10_message_timeout_flag == 0))
					{

						/*If speed not present and AC on*/
						if((sts_ac_compressor_flag == 0x1) &&  ((CAN_ESC12_10[2].byte)==0) && ((CAN_ESC12_10[3].byte)==0))
						{
							AC_ON = 1;
							speed = 0;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 1;
						}
						/*speed present and AC off */
						else if((sts_ac_compressor_flag != 0x1) &&  (((CAN_ESC12_10[2].byte)!=0) || ((CAN_ESC12_10[3].byte)!=0))) /* ACtrip is off */
						{
							AC_ON = 0;
							speed = 1;
							if(copy_odo == 0)
							{
								copy_odo = 1;
								Initial_odo = final_odomter_distance;
								final_ACtripmeter_distance = final_ACtripmeter_distance1;
							}
							//						ac_off_odo_run = final_odomter_distance - Initial_odo;
							final_ACtripmeter_distance1 = final_ACtripmeter_distance + (final_odomter_distance - Initial_odo);//ac_off_odo_run;
							AC_Trip_1min_ON_flag = 1;//Not to reset Timer In Traffic scenarios
						}
						/*Speed present and AC ON*/
						else if((sts_ac_compressor_flag == 0x1) &&  (((CAN_ESC12_10[2].byte)!=0) || ((CAN_ESC12_10[3].byte)!=0)))
						{
							AC_ON = 1;
							speed = 1;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 1;
						}
						else
						{
							/*if odo is incrementing AC off and speed is 0*/
							AC_ON = 0;
							speed = 1;
							if(copy_odo == 0)
							{
								copy_odo = 1;
								Initial_odo = final_odomter_distance;
								final_ACtripmeter_distance = final_ACtripmeter_distance1;
							}
							//						ac_off_odo_run = final_odomter_distance - Initial_odo;
							final_ACtripmeter_distance1 = final_ACtripmeter_distance + (final_odomter_distance - Initial_odo);//ac_off_odo_run;
							AC_Trip_1min_ON_flag = 0;
							/*
							AC_ON = 0;
							speed = 0;
							copy_odo = 0;
							AC_Trip_1min_ON_flag = 0;
							*/
						}


					}
					//#endif

#if 0
					if(sts_ac_compressor_flag == 1)
					{
						if((speed_absent_flag == 0)||(from_speed_zero == 1))
						{
							speed_absent_flag = 1;
							from_speed_zero = 2;
							init_odo = current_odo_distance;
						}
						if( ((CAN_ESC2_10[0].byte)!=0) || ((CAN_ESC2_10[1].byte)!=0) || ((CAN_ESC12_10[2].byte)!=0) || ((CAN_ESC12_10[3].byte)!=0) )//speed present
						{
							AC_Trip_1min_ON_flag = 0;
							if(from_speed_zero == 0)
							{
								from_speed_zero = 1;
							}
							if(current_odo_distance >= init_odo+100)
							{
								display_ACtripmeter_speed = display_ACtripmeter_speed + 1;
							}
						}
						else
						{
							speed_absent_flag = 1;
							AC_Trip_1min_ON_flag = 1;
							if(Counter_1min >= 53000) /*600000 AC = ON, speed < 0 then ACtrip increment by 1km for 1min */
							{
								display_ACtripmeter_speed = display_ACtripmeter_speed + 1;
								final_ACtripmeter_distance = display_ACtripmeter_speed * 100;//update final_ACtripmeter_distance every 6 sec
								//					final_ACtripmeter_distance = display_ACtripmeter_speed;//r
								Counter_1min = 0;
							}

						}
					}
#endif

#if 1
					if(Counter_1min >= Trip_AC_1min_Count) /*390000 AC = ON, speed < 0 then ACtrip increment by 1km for 1min */
					{
						//					display_ACtripmeter_speed = display_ACtripmeter_speed + 1;
						//					final_ACtripmeter_distance = display_ACtripmeter_speed * 100;//update final_ACtripmeter_distance every 6 sec
						//					final_ACtripmeter_distance = display_ACtripmeter_speed;//r
						TRIP_AC_OFFSET = TRIP_AC_OFFSET + 1000;
						Counter_1min = 0;
					}
#endif
					if((display_ACtripmeter_speed > 9999)) /* if ACtrip > 9999 */
					{
						display_ACtripmeter_speed = 0;
						//					final_ACtripmeter_distance = display_ACtripmeter_speed;//r
					}

				}
			 if(((ESC2_10_message_timeout_flag != 0)&&(FEATURE_CODE_SPEED_INPUT == 0))||((ESC12_10_message_timeout_flag != 0)&&(FEATURE_CODE_SPEED_INPUT == 1))||(EMS1_10_message_timeout_flag != 0))

				{
					AC_Trip_1min_ON_flag = 0;//clear timer if CAN missing
				}
			}
			else
			{
				/* Retain Previous Value */
				AC_ON = 0;
				speed = 1;
				if(copy_odo == 0)
				{
					copy_odo = 1;
					Initial_odo = final_odomter_distance;
					final_ACtripmeter_distance = final_ACtripmeter_distance1;
				}
				//						ac_off_odo_run = final_odomter_distance - Initial_odo;
				final_ACtripmeter_distance1 = final_ACtripmeter_distance + (final_odomter_distance - Initial_odo);//ac_off_odo_run;
				AC_Trip_1min_ON_flag = 0;
			}
		}
		else
		{
			/* disable digital tripmeter */
			AC_Trip_1min_ON_flag = 0;
		}
	}
	else
	{
		/* display off */
	}

}


/* ***********************************************************************************************
 * Function Name: EngineCoolantTemperature
 * Description  : This function used for Engine Coolant Temperature of vechile (Temperature Gauge).
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void Engine_Coolant_Temperature(void)
{
	int16_t ENG_temp = 0U;//uint8_t

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 0U)
			{
				/* display empty guage */
				segment1 = 0;
				segment2 = 0;
				segment3 = 0;
				segment4 = 0;
				segment5 = 0;
				segment6 = 0;
				segment7 = 0;
				segment8 = 0;
				new_temp_val = 0;
				new_temp_value =0;
				Temp_blinkpattern_flag_7 = 0;
				Temp_blinkpattern_flag_8 = 0;
				//				Temp_blinkpattern_flag=0;
			}
			else
			{
				/* EMS1_10 message */
				ENG_temp = ((CAN_EMS1_10[7].byte) - 40); /* copied engine temperature from EMS1_10 & -40 has done due to offset */

				Engine_Speed = ENG_temp;

				if(EMS1_10_message_timeout_flag != 0)
				{
					//					temperature_value = 1;
					segment1 = 0;
					segment2 = 0;
					segment3 = 0;
					segment4 = 0;
					segment5 = 0;
					segment6 = 0;
					segment7 = 0;
					segment8 = 0;
					new_temp_val = 0;
					new_temp_value =0;
					Temp_blinkpattern_flag_7 = 0;
					Temp_blinkpattern_flag_8 = 0;
					temp_widget = 0;
					//				Temp_blinkpattern_flag=0;
				}

				else
				{
					if((EMS1_10_message_timeout_flag == 0)&&(ENG_temp > 214 ))//215 = 0xff = invalid
					{
						/* displaying all segment */
						segment1 = 1;
						segment2 = 1;
						segment3 = 1;
						segment4 = 1;
						segment5 = 1;
						segment6 = 1;
						segment7 = 1;
						segment8 = 1;
						new_temp_val = 8;
						new_temp_value =8;
						Temp_blinkpattern_flag_7 = 0;
						Temp_blinkpattern_flag_8 = 0;
						//				Temp_blinkpattern_flag=0;
						if(logo_completed==1)
						{
							(void)drome_sendEventToDrome((U32)drome_InEvent_Invalid_tempbar);
						}
					}
					else
					{
//						if((ENG_temp <= TEMP_BAR_1_ON_VALUE))
//						{
//							segment1 = 0;
//							segment2 = 0;
//							segment3 = 0;
//							segment4 = 0;
//							segment5 = 0;
//							segment6 = 0;
//							segment7 = 0;
//							segment8 = 0;
//							new_temp_val = 0;
//							//						new_temp_value = 0;
//						}

						if(ENG_temp >= Temp_Bar_Status_1st_byte.byte)//TEMP_BAR_1_ON_VALUE
						{
							segment1 = 1;
							segment2 = 0;
							segment3 = 0;
							segment4 = 0;
							segment5 = 0;
							segment6 = 0;
							segment7 = 0;
							segment8 = 0;
							new_temp_val = 1;
							new_temp_value = 1;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							IGN_temp_0 = 0;
							//				Temp_blinkpattern_flag=0;
						}
						/*Bar 1 off*/
						if((ENG_temp <= Temp_Bar_Status_9th_byte.byte) && (ENG_temp >= -40))//TEMP_BAR_1_OFF_VALUE
						{
							new_temp_val = 0;
							new_temp_value =0;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							if((new_temp_val == 0)&&(IGN_temp_0==1))
							{
								temp_widget = 0;
								IGN_temp_0 = 0;
							}
						}

						if(ENG_temp >= Temp_Bar_Status_2nd_byte.byte)//TEMP_BAR_2_ON_VALUE
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 0;
							segment4 = 0;
							segment5 = 0;
							segment6 = 0;
							segment7 = 0;
							segment8 = 0;
							new_temp_val = 2;
							new_temp_value = 2;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;
						}
						/*Bar 2 off*/
						//						if((ENG_temp <= TEMP_BAR_2_OFF_VALUE) && (ENG_temp > TEMP_BAR_1_OFF_VALUE))
						if((ENG_temp <= Temp_Bar_Status_10th_byte.byte) && (ENG_temp > Temp_Bar_Status_9th_byte.byte))
						{
							new_temp_value =1;
							Temp_blinkpattern_flag_7 = 0;
							if(new_temp_val > 1)
							{
								new_temp_val =1;
							}
						}

						//						if(ENG_temp >= TEMP_BAR_3_ON_VALUE)
						if(ENG_temp >= Temp_Bar_Status_3rd_byte.byte)
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 1;
							segment4 = 0;
							segment5 = 0;
							segment6 = 0;
							segment7 = 0;
							segment8 = 0;
							new_temp_val = 3;
							new_temp_value = 3;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;
						}
						/*Bar 3 off*/
						//						if((ENG_temp <= TEMP_BAR_3_OFF_VALUE) && (ENG_temp > TEMP_BAR_2_OFF_VALUE))
						if((ENG_temp <= Temp_Bar_Status_11th_byte.byte) && (ENG_temp > Temp_Bar_Status_10th_byte.byte))
						{
							new_temp_value =2;
						}

						/* displaying as per calculation */
						//						if(ENG_temp >= TEMP_BAR_4_ON_VALUE)
						if(ENG_temp >= Temp_Bar_Status_4th_byte.byte)
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 1;
							segment4 = 1;
							segment5 = 0;
							segment6 = 0;
							segment7 = 0;
							segment8 = 0;
							new_temp_val = 4;
							new_temp_value = 4;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;
						}
						/*Bar 4 off*/
						//						if((ENG_temp <= TEMP_BAR_4_OFF_VALUE) && (ENG_temp > TEMP_BAR_3_OFF_VALUE))
						if((ENG_temp <= Temp_Bar_Status_12th_byte.byte) && (ENG_temp > Temp_Bar_Status_11th_byte.byte))
						{
							new_temp_value =3;
						}

						//						if(ENG_temp >= TEMP_BAR_5_ON_VALUE)
						if(ENG_temp >= Temp_Bar_Status_5th_byte.byte)
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 1;
							segment4 = 1;
							segment5 = 1;
							segment6 = 0;
							segment7 = 0;
							segment8 = 0;
							new_temp_val = 5;
							new_temp_value = 5;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;
						}
						/*Bar 5 off*/
						//						if((ENG_temp <= TEMP_BAR_5_OFF_VALUE)&&(ENG_temp > TEMP_BAR_4_OFF_VALUE))
						if((ENG_temp <= Temp_Bar_Status_13th_byte.byte)&&(ENG_temp > Temp_Bar_Status_12th_byte.byte))
						{
							new_temp_value =4;
						}

						//						if(ENG_temp >= TEMP_BAR_6_ON_VALUE)
						if(ENG_temp >= Temp_Bar_Status_6th_byte.byte)
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 1;
							segment4 = 1;
							segment5 = 1;
							segment6 = 1;
							segment7 = 0;
							segment8 = 0;
							new_temp_val = 6;
							new_temp_value = 6;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;
						}
						/*Bar 6 off*/
						//						if((ENG_temp <= TEMP_BAR_6_OFF_VALUE)&&(ENG_temp > TEMP_BAR_5_OFF_VALUE))
						if((ENG_temp <= Temp_Bar_Status_14th_byte.byte)&&(ENG_temp > Temp_Bar_Status_13th_byte.byte))
						{
							new_temp_value =5;
						}


						//						if(ENG_temp >= TEMP_BAR_7_ON_VALUE)
						if(ENG_temp >= Temp_Bar_Status_7th_byte.byte)
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 1;
							segment4 = 1;
							segment5 = 1;
							segment6 = 1;
							segment7 = 1;
							segment8 = 0;
							new_temp_val = 7;
							new_temp_value = 7;
							Temp_blinkpattern_flag_7 = 1;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;

						}
						/*Bar 7 off*/
						//						if((ENG_temp <= TEMP_BAR_7_OFF_VALUE)&&(ENG_temp > TEMP_BAR_6_OFF_VALUE))
						if((ENG_temp <= Temp_Bar_Status_15th_byte.byte)&&(ENG_temp > Temp_Bar_Status_14th_byte.byte))
						{
							new_temp_value = 6;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;
							//				Temp_blinkpattern_flag=0;
						}

						//						if(ENG_temp >= TEMP_BAR_8_ON_VALUE)//
						if(ENG_temp >= Temp_Bar_Status_8th_byte.byte)//
						{
							segment1 = 1;
							segment2 = 1;
							segment3 = 1;
							segment4 = 1;
							segment5 = 1;
							segment6 = 1;
							segment7 = 1;
							segment8 = 1;
							new_temp_val = 8;
							new_temp_value = 8;
							Temp_blinkpattern_flag_7 = 0;
							Temp_blinkpattern_flag_8 = 0;//1
							//				Temp_blinkpattern_flag=1;

							if(logo_completed==1)
							{
								(void)drome_sendEventToDrome((U32)drome_InEvent_Invalid_toNormal_temp);
							}
						}
						/*Bar 8 off*/
						//						if((ENG_temp <= TEMP_BAR_8_OFF_VALUE) && (ENG_temp > TEMP_BAR_7_OFF_VALUE))//
						if((ENG_temp <= Temp_Bar_Status_16th_byte.byte) && (ENG_temp > Temp_Bar_Status_15th_byte.byte))//
						{
							if(pre_temp_val > new_temp_val)/*condition only for bar return from full*/
							{
								new_temp_value = 7;
								Temp_blinkpattern_flag_7 = 1;
								Temp_blinkpattern_flag_8 = 0;
								//				Temp_blinkpattern_flag=0;
							}
						}

						/*Bar no off*/
						//						if((ENG_temp <= TEMP_BAR_8_ON_VALUE) && (ENG_temp > TEMP_BAR_8_OFF_VALUE))//
						if((ENG_temp <= Temp_Bar_Status_8th_byte.byte) && (ENG_temp > Temp_Bar_Status_16th_byte.byte))//
						{
							if(pre_temp_val > new_temp_val)/*condition only for bar return from full*/
							{
								new_temp_value = 8;
								Temp_blinkpattern_flag_7 = 0;
								Temp_blinkpattern_flag_8 = 0;//1
								//				Temp_blinkpattern_flag=1;
							}
							if(logo_completed==1)
							{
								(void)drome_sendEventToDrome((U32)drome_InEvent_Invalid_toNormal_temp);
							}
						}

					}

				}

			}

		}
		else
		{
			/* display off */
		}

	}
	else
	{
		/* display off */
	}
#if 1
	if(pre_temp_val < new_temp_val)
	{
		for(;pre_temp_val < new_temp_val;pre_temp_val++)
		{
			temp_widget++;
		}
	}
	//	if(pre_temp_val>=7 && new_temp_value==6)
	//	{
	//		if(temp_widget > 8)
	//		{
	//			temp_widget = 8;
	//		}
	//		temp_widget=temp_widget-2;
	//		pre_temp_val = 6;
	//
	//	}

	else if(pre_temp_val > new_temp_value)
	{
		for(;pre_temp_val > new_temp_value;pre_temp_val--)
		{
			temp_widget--;
		}
	}
	if((temp_widget > 8)||(pre_temp_val >= 8))
	{
		temp_widget = pre_temp_val;
		//		pre_temp_val = 8;
	}
#endif
#if 0
	if(pre_temp_val < new_temp_val)
	{
		temperature_value = 0;
	}
	else if(pre_temp_val > new_temp_value)
	{
		temperature_value = 1;
	}
	else if(pre_temp_val == new_temp_value)//||(pre_temp_val == new_temp_val))
	{
		temperature_value = 2;
	}
	if(logo_completed==1)
	{
		switch (temperature_value)
		{
		case Temp_Jump:
			(void)drome_sendEventToDrome((U32)drome_InEvent_Temp_Jump);
			pre_temp_val++;
			break;
		case Temp_Return:
			(void)drome_sendEventToDrome((U32)drome_InEvent_Temp_Return);
			pre_temp_val--;
			break;
		case Temp_NONE:
		default:
			break;
		}
	}
	if(pre_temp_val >= 8)
	{
		pre_temp_val = 8;
	}
#endif
}



/* ***********************************************************************************************
 * Function Name: tripmeter_B
 * Description  : This function used for tripmeter_B of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void tripmeter_B(void)
{
	SplitWord1 down_scale_trip;
	uint32_t display_tripmeter_speed = 0U;

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if( ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1))
					|| ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0)) )
			{
				switch(FEATURE_CODE_SPEED_INPUT)
				{
				case 0:
					/* ESC2_10 message */
					down_scale_trip.w    = 0x00; /* clearing previous stored value */
					down_scale_trip.b[0] = CAN_ESC2_10[2].byte; /* copied 2nd byte data*/
					down_scale_trip.b[1] = CAN_ESC2_10[3].byte; /* copied 3rd byte data*/

					break;

				case 1:
					/* ESC12_10 message */
					down_scale_trip.w    = 0x00; /* clearing previous stored value */
					down_scale_trip.b[0] = CAN_ESC12_10[0].byte; /* copied 1st byte data*/
					down_scale_trip.b[1] = CAN_ESC12_10[1].byte; /* copied 2nd byte data*/

					break;

				default:
					/* nothing */
					break;
				}


				if((previous_trip_B1 != down_scale_trip.b[0]) || (previous_trip_B2 != down_scale_trip.b[1]) ) //checking previous & current value
				{
					previous_trip_B1 = down_scale_trip.b[0];
					previous_trip_B2 = down_scale_trip.b[1];

					//after_dec_val = (( (down_scale_trip.w) % (1000) ) / (100));
					//down_scale_trip.w = ((down_scale_trip.w) / (1000));

					if(final_tripmeter_B_distance == 9999) /* final trip value == 9999km */
					{
						trip_B_flag = 2;
					}

					if((down_scale_trip.w < 65535 ) && (trip_B_flag == 1)) /* checking trip value is < 65535 m */
					{
						if((previous_tripmeter_B_distance <= down_scale_trip.w)) /* if previous trip value < current trip value */
						{
							previous_tripmeter_B_distance = down_scale_trip.w;
						}
						else if((previous_tripmeter_B_distance > down_scale_trip.w)) /* if previous trip value > current trip value */
						{
							down_scale_trip.w = previous_tripmeter_B_distance;
						}

					}
					else if(((down_scale_trip.w == 65535 ) && (final_tripmeter_B_distance < 9999)) || (trip_B_flag == 0)) /* if trip value is >= 65535m */
					{
						trip_B_flag=0;

						if((down_scale_trip.w == 65535 ))
						{
							over_flag_B++; // after every 65535m this variable is incremented
							if(over_flag_B >= 2)
							{
								trip_B_increment++;
							}

							previous_tripmeter_B_distance = 0x00;
							down_scale_trip.w = 65535 * trip_B_increment;

						}
						else if((previous_tripmeter_B_distance <= down_scale_trip.w))
						{
							/* displaying (current odo value + (65535 * trip_increment)) */
							down_scale_trip.b[3] = 0x00;
							down_scale_trip.b[2] = 0x00;

							previous_tripmeter_B_distance = down_scale_trip.w;
							down_scale_trip.w = (65535 * trip_B_increment) + down_scale_trip.w;
						}
						else if((previous_tripmeter_B_distance > down_scale_trip.w))
						{
							/* displaying (previous odo value + (65 * trip_increment)) */
							down_scale_trip.w = (65535 * trip_B_increment) + previous_tripmeter_B_distance;
						}
					}
					else if((final_tripmeter_B_distance == 9999) && (trip_B_flag == 2)) /* if odo value is 999999 */
					{
						down_scale_trip.w = 0;
					}

					after_dec_val_B = (( (down_scale_trip.w) % (1000) ) / (100)); // this variable gives after decimal value i.e. from 0 to 9
					down_scale_trip.w = ((down_scale_trip.w) / (100));

					if(final_tripmeter_B_distance < (down_scale_trip.w))
					{
						final_tripmeter_B_distance = down_scale_trip.w;
					}

				}

			}
			else
			{
				/* nothing*/
			}
		}
		else
		{
			/* disable digital tripmeter */
		}
	}
	else
	{
		/* display off */
	}

}



/* ***********************************************************************************************
 * Function Name: reset_odometer
 * Description  : This function used for reseting odometer of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
/* "previous_odometer_distance" this variable need to be reset for odo */
void reset_odometer(void)
{
	//uint8_t Switch_val = 0;  /* LongPress value assign to this variable */

	//uint8_t temp_odometer_reset_flag;
	//temp_odometer_reset_flag = odometer_reset_flag;
//	for(uint8_t i=0;i<3;i++)
//	{
//		odometer_reset_flag = eeprom_read(ODO_RST_FLAG_OFFSET);
//		odometer_reset_flag_1 = eeprom_read(ODO_RST_FLAG_OFFSET_1);
//	}
	/* checking odo is less than 255 */

	if((final_odomter_distance <= 255000))
	{

		/* checking reset_counter_flag */
//		odometer_reset_flag = eeprom_read(ODO_RST_FLAG_OFFSET);
//		odometer_reset_flag_1 = eeprom_read(ODO_RST_FLAG_OFFSET_1);
//		if((odometer_reset_flag == 0xff)&&(odometer_reset_flag_1 == 0xff))
//		{
//			odometer_reset_flag = 0;
//			odometer_reset_flag_1 = 0;
//			eeprom_writebt(ODO_RST_FLAG_OFFSET, odometer_reset_flag);
//			eeprom_writebt(ODO_RST_FLAG_OFFSET_1, odometer_reset_flag_1);
//		}
		for(uint8_t i=0;i<10;i++)
		{
			odometer_reset_flag = eeprom_read(ODO_RST_FLAG_OFFSET);
			odometer_reset_flag_1 = eeprom_read(ODO_RST_FLAG_OFFSET_1);
		}
		if((odometer_reset_flag < 2) && (odometer_reset_flag_1 < 2))
		{

			/* reset odometer, trip A & trip B */
			final_odomter_distance      = 0; /* reset odo */
			old_odometer_distance = 0;
			a=0;b=0;
			previous_odometer_distance  = 0; /* reset odo */
			for(uint8_t i=0; i<25 ;i++)
			{
				eeprom_writeOdometer(final_odomter_distance);
			}
			reset_tripmeterA();
			previous_tripmeter_distance = 0; /* reset trip */
			final_tripmeter_distance    = 0; /* reset tripA */
			previous_trip1 = 0;
			previous_trip2 = 0;

			reset_tripmeterB();
			previous_tripmeter_B_distance = 0;
			final_tripmeter_B_distance  = 0; /* reset tripB */
			previous_trip_B1 = 0;
			previous_trip_B2 = 0;

			reset_AC_tripmeter();

			odometer_reset_flag++;
			odometer_reset_flag_1++;
			eeprom_writebt(ODO_RST_FLAG_OFFSET, odometer_reset_flag);
			eeprom_writebt(ODO_RST_FLAG_OFFSET_1, odometer_reset_flag_1);
			Flag_AFE_reset= 1;			/*Reset AFE*/
			U8_Reset_DTE = 1U;

			Init_2WD_odo_distance = final_odomter_distance;/*reset 4WD not engaged distance in drive summary - 27-05-20*/
			Prev_2WD_odo_distance = 0;

			/*temp_odometer_reset_flag = temp_odometer_reset_flag++;
					odometer_reset_flag      = temp_odometer_reset_flag;

					odometer_reset_completed_flag = 1;*/ /* this flag is giving indication to reset_tripmeter */
		}

	}

	/*temp_final_odomter_distance = temp_final_odomter_distance + 1;
	final_odomter_distance      = temp_final_odomter_distance;*/

}



/* ***********************************************************************************************
 * Function Name: reset_tripmeter
 * Description  : This function used for reseting tripmeter of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void reset_tripmeterB()
{
	uint8_t Switch_val = 0;  /* LongPress value assign to this variable */
	final_tripmeter_B_distance=final_odomter_distance;
	eeprom_write_Trip2_Odometer(final_tripmeter_B_distance);
	AVS_initial_cnt_B = 0;
	AVS_sec_count_B.u32 = 0;
	DT_sec_count_B.u32 = 0;
	HMI_AVS_DASH2 = 1;
	New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_B, AVS_sec_count_B.u8, 4);	//AVS time counter B;
	New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_B, DT_sec_count_B.u8, 4);		//AVS time counter B;

}

void reset_tripmeterA()
{
	uint8_t Switch_val = 0;  /* LongPress value assign to this variable */
	final_tripmeter_distance=final_odomter_distance;
	eeprom_write_Trip1_Odometer(final_tripmeter_distance);
	AVS_initial_cnt_A = 0;
	AVS_sec_count_A.u32 = 0;
	DT_sec_count_A.u32 = 0;
	HMI_AVS_DASH1 = 1;
	New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_A, DT_sec_count_A.u8, 4);		//AVS time counter B;
	New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_A, AVS_sec_count_A.u8, 4);	//AVS time counter;
}

void reset_AC_tripmeter()
{
	if((FEATURE_CODE_TRIP_AC_RESET == 1)||(Trip_AC_Rollover == 1))
	{
		uint8_t Switch_val = 0;  /* LongPress value assign to this variable */
		Trip_AC_Rollover = 0;
		TRIP_AC_OFFSET = 0;
		Counter_1min = 0;//reset minute counter
		final_ACtripmeter_distance = final_odomter_distance;
		final_ACtripmeter_distance1 = final_ACtripmeter_distance;
		eeprom_write_Trip_AC_Odometer(final_ACtripmeter_distance);
		eeprom_write_Trip_AC_Odometer1(final_ACtripmeter_distance1);
		eeprom_write_Trip_AC_Offset(TRIP_AC_OFFSET);
		Initial_odo = final_ACtripmeter_distance;//(added to solve 429...0 to 0 toggle)
		//		eeprom_write_Trip_AC_Odometer(display_ACtripmeter_speed);// final_ACtripmeter_distance
		//		display_ACtripmeter_speed=0;
		//		final_ACtripmeter_distance = 0;
	}
}
#if 0
/* Check switch combination met */
if(Switch_val == 1)
{
	previous_tripmeter_distance = 0; /* reset trip */
	final_tripmeter_distance    = 0; /* reset tripA */
	previous_trip1 = 0;
	previous_trip2 = 0;

	previous_tripmeter_B_distance = 0;
	final_tripmeter_B_distance  = 0; /* reset tripB */
	previous_trip_B1 = 0;
	previous_trip_B2 = 0;
}
/* checking odometer reset is done or not */
else if(odometer_reset_flag < 2U)
{
	//odometer_reset_completed_flag = 0;

	previous_tripmeter_distance = 0; /* reset trip */
	final_tripmeter_distance    = 0; /* reset tripA */
	previous_trip1 = 0;
	previous_trip2 = 0;

	previous_tripmeter_B_distance = 0;
	final_tripmeter_B_distance  = 0; /* reset tripB */
	previous_trip_B1 = 0;
	previous_trip_B2 = 0;
}
/* checking odometer write has done or not */
else if(final_odomter_distance > 0) /* odometer write done */
{
	previous_tripmeter_distance = 0; /* reset trip */
	final_tripmeter_distance    = 0; /* reset tripA */
	previous_trip1 = 0;
	previous_trip2 = 0;

	previous_tripmeter_B_distance = 0;
	final_tripmeter_B_distance  = 0; /* reset tripB */
	previous_trip_B1 = 0;
	previous_trip_B2 = 0;
}
/* checking tripmeter rollover 9999 to 0 km */
else if(final_tripmeter_distance >= 9999)
{
	/* reset condition met */
	previous_tripmeter_distance = 0; /* reset trip */
	final_tripmeter_distance    = 0; /* reset tripA */
	previous_trip1 = 0;
	previous_trip2 = 0;

	previous_tripmeter_B_distance = 0;
	final_tripmeter_B_distance  = 0; /* reset tripB */
	previous_trip_B1 = 0;
	previous_trip_B2 = 0;
}
else
{
	/* reset condition not met */
}

#endif
/* ***********************************************************************************************
 * Function Name: Average Vehicle speed
 * Description  : This function used for Average Vehicle speed.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */

void Average_vehicle_speed(void)
{
	SplitWord1 down_scale_AVS;
	//	uint64_t Avg_vehicle_speed1 = 0,Avg_vehicle_speed2 = 0;
	uint8_t STS_Eng_Val = 0U;

	if(EMS1_10_message_timeout_flag == 0)
	{
		STS_Eng_Val = CAN_EMS1_10[6].byte;
		STS_Eng_Val = ((STS_Eng_Val & 0xE0) >> 5);
	}
	else
	{
		STS_Eng_Val = 0;
		HMI_AVS_DASH1 = 1;
		HMI_AVS_DASH2 = 1;
	}
	//	uint16_t AVS_INITIAL_SAMPLE_DISTANCE = 0;

	if(IGN_ON_flag == 1)																			/* checking ignition ON or OFF */
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))													/* checking LVSD & HVSD flag */
		{

			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1U ) && (FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1U))
			{
				if((STS_Eng_Val == 2) || ((STS_Eng_Val == 3)&&(FEATURE_CODE_START_STOP_TT_CONFIG == 1)))	/*Issue Modified for VPsprint2- Engine stopped due to ESS => STS_ENG = 3*/
				{
					if((Speed_Missing == 1)||(Speed_Invalid_Default == 1))														/*vehicle speed missing*/
					{
						HMI_AVS_DASH1 = 1;
						HMI_AVS_DASH2 = 1;
						count_start = 0;
					}
					else
					{
//						HMI_AVS_DASH1 = 0;															/*vehicle speed present*/
//						HMI_AVS_DASH2 = 0;

						if(AVS_initial_cnt_A == 0)
						{
							AVS_initial_cnt_A++;
							init_Accumulated_distance_A = final_odomter_distance - final_tripmeter_distance ; 		/*Accumulated distance for Trip A*/
							count_start = 1;
						}

						if(AVS_initial_cnt_B == 0)
						{
							AVS_initial_cnt_B++;
							init_Accumulated_distance_B = final_odomter_distance - final_tripmeter_B_distance ; 	/*Accumulated distance  for Trip B*/
							count_start_B = 1;
						}


					}
					/***Average speed for Trip A***/
					Accumulated_distance_A = final_odomter_distance - final_tripmeter_distance ; 		/*Accumulated distance for Trip A*/
					Accumulated_distance_B = final_odomter_distance - final_tripmeter_B_distance ; 		/*Accumulated distance  for Trip B*/

					AVS_INITIAL_SAMPLE_DISTANCE = DID_AVG_SPEED_0x0511_arr[0]*10 ; 						/*Initial sample distance*/
					AVS_SAMPLE_TIME = DID_AVG_SPEED_0x0511_arr[1] ;										/*Initial sample time*/
					Dist_Unit = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[7];								/*Distance Unit - km / mile*/

					if(Accumulated_distance_A >= (init_Accumulated_distance_A + AVS_INITIAL_SAMPLE_DISTANCE))
					{
						if((Speed_Missing == 0)&&(Speed_Invalid_Default == 0))														/*vehicle speed missing*/
						{
							HMI_AVS_DASH1 = 0;
						}
						count_start_temp = 1;
						if((AVS_sec_count_temp >= AVS_SAMPLE_TIME)||(Initial_entry_A == 1))
						{
							Avg_vehicle_speed1 = (((Accumulated_distance_A)*100) / AVS_sec_count_A.u32)/**100)*/;/*AVS_sample_sec_A1*/;
							Avg_vehicle_speed1 = (Avg_vehicle_speed1 * 3.6)/100;									//(3600/1000) =  3.6 conversion for m/sec -> km/h
							AVS_sec_count_temp = 0;
							Initial_entry_A = 0;
						}
					}
					else
					{
						HMI_AVS_DASH1 = 1;
						count_start_temp = 0;
					}

					/***Average speed for Trip B***/
					if(Accumulated_distance_B >= (init_Accumulated_distance_B + AVS_INITIAL_SAMPLE_DISTANCE))
					{
//						HMI_AVS_DASH2 = 0;
						count_start_temp_B = 1;
						if((AVS_sec_count_temp_B >= AVS_SAMPLE_TIME)||(Initial_entry_B == 1))
						{
							if((Speed_Missing == 0)&&(Speed_Invalid_Default == 0))														/*vehicle speed missing*/
							{
								HMI_AVS_DASH2 = 0;
							}
							Avg_vehicle_speed2 = (((Accumulated_distance_B)*100) / AVS_sec_count_B.u32)/**100)*/;
							Avg_vehicle_speed2 = (Avg_vehicle_speed2 * 3.6)/100;								//(3600/1000) =  3.6 conversion for m/sec -> km/h
							AVS_sec_count_temp_B = 0;
							Initial_entry_B = 0;
						}
					}

					else
					{
						HMI_AVS_DASH2 = 1;
						count_start_temp_B = 0;
					}

				}

			}
			else
			{

			}
			if(HMI_AVS_DASH1 == 0)
			{
				if((Dist_Unit == 0)||(Dist_Unit == 1))										/*Dist_Unit - km -> AVS unit - km/h*/
				{
					Display_Average_speed_A = Avg_vehicle_speed1;
				}
				else if(Dist_Unit == 2)														/*Dist_Unit - mile -> AVS unit - mph*/
				{
					Display_Average_speed_A = Avg_vehicle_speed1 * 0.621371;					/*1km= 0.621371 Mile*/
				}

				if(Display_Average_speed_A >= 300)
				{
					Display_Average_speed_A = 300;
				}
			}

			if(HMI_AVS_DASH2 == 0)
			{
				if((Dist_Unit == 0)||(Dist_Unit == 1))										/*Dist_Unit - km -> AVS unit - km/h*/
				{
					Display_Average_speed_B = Avg_vehicle_speed2;
				}
				else if(Dist_Unit == 2)														/*Dist_Unit - mile -> AVS unit - mph*/
				{
					Display_Average_speed_B = Avg_vehicle_speed2 * 0.621371;					/*1km= 0.621371 Mile*/
				}

				if(Display_Average_speed_B >= 300)
				{
					Display_Average_speed_B = 300;
				}
			}


		}
	}
	else
	{

	}
}

void drive_time(void)
{
	uint8_t STS_Eng_Val = 0U;

	if(EMS1_10_message_timeout_flag == 0)
	{
		STS_Eng_Val = CAN_EMS1_10[6].byte;
		STS_Eng_Val = ((STS_Eng_Val & 0xE0) >> 5);
	}
	else
	{
		STS_Eng_Val = 0;
	}
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if( FEATURE_CODE_DRIVING_TIME_CONFIG==1)
			{
				if((STS_Eng_Val == 2) || ((STS_Eng_Val == 3)&&(FEATURE_CODE_START_STOP_TT_CONFIG == 1)))	/*Issue Modified for VPsprint2- Engine stopped due to ESS => STS_ENG = 3*/
				{
					drive_count_start = 1;
				}
				else
				{
					drive_count_start = 0;
				}
				Display_Hr_A = DT_sec_count_A.u32 / 3600;
				Display_Min_A=(DT_sec_count_A.u32-(3600*Display_Hr_A))/60;
				if(Display_Hr_A > 99)
				{
					Display_Hr_A = 0;
					DT_sec_count_A.u32 = 0;
				}

				Display_Hr_B = DT_sec_count_B.u32 / 3600;
				Display_Min_B=(DT_sec_count_B.u32-(3600*Display_Hr_B))/60;
				if(Display_Hr_B > 99)
				{
					Display_Hr_B = 0;
					DT_sec_count_B.u32 = 0;
				}
			}
/***************** Time count for drive summary calculation *******************/
			if((STS_Eng_Val == 2) || ((STS_Eng_Val == 3)&&(FEATURE_CODE_START_STOP_TT_CONFIG == 1)))	/*Issue Modified for VPsprint2- Engine stopped due to ESS => STS_ENG = 3*/
			{
				drivesum_count_start = 1;
			}
			else
			{
				drivesum_count_start = 0;
			}

		}

	}

}
/* ***********************************************************************************************
 * Function Name: Drive_summery
 * Description  : This function used for Drive_summery.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void Drive_summery(void)
{
/****************** Distance covered in IGN Cycle ******************/
	Final_Drivesum_odo_distance = final_odomter_distance - Init_Drivesum_odo_distance;

/****************** Distance covered with 2WD in IGN Cycle + PREV STORED DATA ******************/
	Final_Drivesum_2WD_distance = ((final_odomter_distance - Init_2WD_odo_distance)+Prev_2WD_odo_distance);	/*Drive summary 4x4 logic changed as per CR - 26-05-20 for W501Hv232*/
	if(Final_Drivesum_2WD_distance >= 999999000)
	{
		Final_Drivesum_2WD_distance = 999999000;
	}
/****************** Time covered in IGN Cycle *********************/
	/*Final_Drivesum_DT_sec_count = DT_sec_count_A.u32 - Init_Drivesum_DT_sec_count;*/
	Final_Drivesum_DT_sec_count = DriveSumsec_count.u32;
	DSum_Display_Hr  = Final_Drivesum_DT_sec_count / 3600;
	DSum_Display_Min =(Final_Drivesum_DT_sec_count-(3600*DSum_Display_Hr))/60;
	if(DSum_Display_Hr >= 99)
	{
		DSum_Display_Hr = 0;
		Final_Drivesum_DT_sec_count = 0;
	}
/************** Average speed covered in IGN Cycle ****************/
	Dsum_Avg_vehicle_speed = ((Final_Drivesum_odo_distance*100) / Final_Drivesum_DT_sec_count);
	Dsum_Avg_vehicle_speed = (Dsum_Avg_vehicle_speed * 3.6)/100;

	if((Dist_Unit == 0)||(Dist_Unit == 1))										/*Dist_Unit - km -> AVS unit - km/h*/
	{
		Dsum_Display_Average_speed = Dsum_Avg_vehicle_speed;
	}
	else if((Dist_Unit == 2)||(Dist_Unit == 3))									/*Dist_Unit - mile -> AVS unit - mph*/
	{
		Dsum_Display_Average_speed = Dsum_Avg_vehicle_speed * 0.621371;			/*1km= 0.621371 Mile*/
	}

	if(Dsum_Display_Average_speed >= 999)
	{
		Dsum_Display_Average_speed = 999;
	}

}
