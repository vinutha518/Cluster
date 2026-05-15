/*
 * parallel_to_serial.c
 *
 *  Created on: 28-Jan-2019
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
#include "gearIndication.h"
#include "UDSS_APP.h"
#include "digitalGuage.h"
#include "alert.h"
#include "swsc.h"
#include "UDS_DTC.h"
#include "tachometer.h"
#include "sound_pcm.h"



#define		ENGINE_HIGHTEMP_TELLTALE_CONT_OFF	temp_show_TT[0]-40
#define		ENGINE_HIGHTEMP_TELLTALE_CONT_ON 	temp_show_TT[1]-40
#define		ENGINE_HIGHTEMP_TELLTALE_BLINK_OFF	temp_show_TT[2]-40
#define     ENGINE_HIGHTEMP_TELLTALE_BLINK_ON	temp_show_TT[3]-40

//#define		ENGINE_HIGHTEMP_TELLTALE_CONT_OFF	0x6E
//#define		ENGINE_HIGHTEMP_TELLTALE_CONT_ON 	0x72
//#define		ENGINE_HIGHTEMP_TELLTALE_BLINK_OFF	0X6A
//#define     ENGINE_HIGHTEMP_TELLTALE_BLINK_ON	0X6E

#define     LOW_FUEL_TELLTALE_OPEN				3505
#define	  	LOW_FUEL_TELLTALE_SHORT	     		490


//#define 	LOW_FUEL_TELLTALE_CONT_OFF			Fuel_level_TT[0]
//#define		LOW_FUEL_TELLTALE_CONT_ON			Fuel_level_TT[1]
//#define  	LOW_FUEL_TELLTALE_BLINK_OFF			Fuel_level_TT[2]
//#define	  	LOW_FUEL_TELLTALE_BLINK_ON			Fuel_level_TT[3]

#define	  	OVERSPEED_LVL2_VALUE				OverSpeedLevel2Val/*120*/
#define	  	OVERSPEED_HYSTERESIS				OverSpeedHystresis/*5*/

#define	  	OVERSPEED_LVL1_VALUE				OverSpeedLevel1Val/*120*/

#define 	TurnLTT    PORTJ_AWOJPPR0_2
#define 	ParkLAMP   PORTJ_AWOJPPR0_1
#define 	FrontFOG   PORTJ_AWOJPPR0_5



/* *********Variables Definition************************************************************************ */

extern unsigned short telltale_blink_flag_patt3;
extern unsigned short telltale_blink_flag_patt4;
extern unsigned short telltale_blink_flag_patt7;
extern unsigned short telltale_blink_flag_patt8;
extern unsigned short telltale_blink_flag_patt9;
extern unsigned short telltale_blink_flag_patt10;
extern _Bool telltale_blink_bool3;
extern _Bool telltale_blink_bool7;
extern _Bool Timer_ON_3sec;
extern _Bool Counter_3sec_Flag;
extern unsigned int CounterTT_90sec;
extern _Bool Flag_4WHTT_500ms_start;
extern _Bool Flag_4WHTT_1500ms_start;
extern _Bool TT4WH_500ms_cmplt_flg;
extern _Bool Flag_4WHTT_1000ms_start;
extern _Bool Flag_4WHTT2_1000ms_start;
extern _Bool TT4WH_1000ms_cmplt_flg;
extern _Bool TT24WH_1000ms_cmplt_flg;
extern _Bool TT4WH_1500ms_cmplt_flg;
extern _Bool  telltale_blink_at_1sec;
extern _Bool Flag_4WHTT_10s_start;
extern _Bool TT4WH_10s_cmplt_flg;
extern _Bool Enginerunningflag;
extern _Bool dpf_alert_flag;
extern uint32_t display_speedometer_speed;
extern _Bool Buzz_ON;
extern _Bool rpm8000_arrived,Less_tn_thrhld_rpm;
extern _Bool SBR_chime_condition_arrived;
extern _Bool spd_2km_arrived;
extern _Bool ODO_arrived;
extern _Bool ALERT_CHECK;
extern uint64_t start_alart_timeout_5sec;

extern uint8_t cycleLowengineoil_cntr;
//Buzzer
_Bool HCT_missing = 0;
_Bool Buzz_High_Temp;
uint8_t Buzz_High_Temp_cntr;
_Bool Buzz_High_Temp_compl;
_Bool Buzz_Low_Eng_oilPre;
uint8_t Buzz_Low_Eng_oilPre_cntr;
_Bool Buzz_Low_Eng_oilPre_compl;
_Bool Buzz_Low_Brk_fld;
uint8_t Buzz_Low_Brk_fld_cntr;
_Bool Buzz_Low_Brk_fld_compl;
_Bool Buzz_Park_brake;
uint8_t Buzz_Park_brake_cntr;
_Bool Buzz_Park_brake_compl;
_Bool Buzz_Door_open;
uint8_t Buzz_Door_open_cntr;
_Bool Buzz_Door_open_compl;
_Bool Buzz_Low_fuel;
uint8_t Buzz_Low_fuel_cntr;
_Bool Buzz_Low_fuel_compl;
_Bool Buzz_Turn_LR;
uint8_t Buzz_Turn_LR_cntr;
_Bool Buzz_Turn_LR_compl;
_Bool Buzz_Rev_Alrm;
uint8_t Buzz_Rev_Alrm_cntr;
_Bool Buzz_Rev_Alrm_compl;
_Bool Buzz_SBR_lv2;
uint8_t Buzz_SBR_lv2_cntr;
_Bool Buzz_SBR_lv2_compl;
_Bool Buzz_rever_gr;
_Bool reverse_played = 0;
_Bool Buzz_rever_gr_SB;
uint8_t Buzz_rever_gr_cntr;
_Bool Buzz_rever_gr_compl;

uint8_t Buzz_KIR_cntr;
_Bool Buzz_KIR_compl;

_Bool buzzParkLamp;			/* @@@ Park Lamp Buzz Status */
uint8_t buzzParkLampCntr;	/* @@@ Park Lamp Buzz Counter */

_Bool Buzz_OS_L1;
_Bool Buzz_OS_L2;
_Bool OS_L2_Start = 0;
uint8_t Buzz_OS_cntr;
_Bool Buzz_OS_compl;
extern uint32_t Buzz_OS_off_tmr;

_Bool SBR_chime_on = 0,SBR_Speed_Activation = 0;
extern uint8_t Diag_RW_Final_chime_duration;
uint32_t Sound_Bit =0x00000000;

_Bool Buzz_WIF;
uint8_t Buzz_WIF_cntr;
_Bool Buzz_WIF_compl;

extern uint32_t CounterBz_3sec;
_Bool RPAS_ERROR_TONE;
_Bool error_cnt = 0;
_Bool W501LOW_error_cnt = 0;
_Bool W501LOW_error_cnt2 = 0;
_Bool W501LOW_selfcheck_cnt = 0;
W501LOW_error_cnt_msgmiss = 0;
_Bool RPAS_ERROR_TONE_compl;
extern uint8_t Buzz_RPAS_ERR_TONE_ctr;
 _Bool RPAS_SELF_CHECK_TONE;
 _Bool RPAS_SELF_CHECK_TONE_compl;
extern uint8_t Buzz_RPAS_SELFCHECK_TONE_ctr;

_Bool RPAS_LEVEL_1_BUZ;
_Bool RPAS_LEVEL_1_TONE_compl;
_Bool RPAS_LEVEL_2_BUZ;
_Bool RPAS_LEVEL_2_TONE_compl;
_Bool RPAS_LEVEL_3_BUZ;
_Bool RPAS_LEVEL_3_TONE_compl;
_Bool RPAS_LEVEL_4_BUZ;
_Bool RPAS_LEVEL_4_TONE_compl;
extern uint8_t RPAS_LEVEL_1_TONE_ctr;
extern uint8_t RPAS_LEVEL_2_TONE_ctr;
extern uint8_t RPAS_LEVEL_3_TONE_ctr;
extern uint8_t RPAS_LEVEL_4_TONE_ctr;
extern uint8_t/*_Bool*/ RPAS_Precondition_Active;

_Bool ABS_Notification=0;
_Bool first_flag=0;
_Bool OS_Flag = 0;
_Bool ECT_ON_flag=0;
_Bool ECT_Blink_flag=0;
_Bool Driver_Seat_belt_flag,Passenger_Seat_belt_flag,BuckledFaulty,Psngr_BuckledFaulty;
_Bool ninty_sec_start_flg=0;
_Bool ninty_sec_cmplt_flg=0;
//4wd
_Bool bit6_4WHD=0;
_Bool bit7_4WHD=0;
_Bool prev_2WHD=0;
_Bool prev_4WDlow=0;
_Bool prev_4WDhigh=0;
_Bool IGN_4WD_STS = 0;

extern _Bool previous_state_immobilizer;
_Bool LOP_7Sec_cntr_start=0;
_Bool LOP_7Sec_cntr_done=0;
_Bool LBF_3Sec_cntr_start=0;
_Bool LBF_3Sec_cntr_done=0;

_Bool Passenger_Seat_belt_alert_flag_ON;
_Bool Driver_Seat_belt_alert_flag_ON;

_Bool Next_Right_Turn;
_Bool Next_Left_Trun;
_Bool Current_Left_Trun;
_Bool Current_Right_Turn;

_Bool Invalid_left_RPAS;
_Bool Invalid_right_RPAS;
_Bool Invalid_center_RPAS;
_Bool priority_lv3;
_Bool priority_lv2;



_Bool RPAS_Present_flag=0;


extern _Bool Buzz_Tick;
extern _Bool Buzz_Tick_compl;

extern _Bool Buzz_Tock;
extern _Bool Buzz_Tock_compl;

extern _Bool Low_fuel_ongoing;
extern _Bool Low_engine_oil_pressure;
extern _Bool chime_start1;
extern _Bool Seatbelt_ongoing;
extern _Bool High_temp_ongoing;
extern _Bool dooropen_ongoing;
extern _Bool Lowbrakefluid_ongoing;
extern _Bool Parkbrake_ongoing;
extern _Bool level1_ongoing;
extern _Bool level2_ongoing;
extern _Bool WIF_ongoing;
extern _Bool Tick_ongoing;
extern _Bool Tock_ongoing;

_Bool fuel_tt_blink_flag;
_Bool fuel_tt_on_flag;
/******************************************************/
extern uint8_t cycle133_completed;
extern uint8_t cycle_Hightemp_completed;
extern uint8_t cycle_Lowengineoil_completed;
extern uint8_t cycle_Lowbrakefluid_completed;
extern uint8_t cycle_dooropen_completed;
extern uint8_t cycle_parkbrake_completed;
extern uint8_t cycle_lowfuel_completed;
extern uint8_t cycle_Overspdl1_completed;
extern uint8_t cycle_Overspdl2_completed;
extern uint8_t cycle_waterinfuel_completed;
extern uint8_t cycle_tick_completed;
extern uint8_t cycle_tock_completed;
extern _Bool Enginerun_chm;
/*****************************************************/
extern uint8_t Dooropen_missed;
extern uint8_t parkbrake_missed;
extern uint8_t lowfuel_missed;
extern uint8_t Seatbelt_missed;
extern uint8_t Hightemp_missed;
extern uint8_t Lowengineoil_missed;
extern uint8_t Lowbrakefluid_missed;
extern uint8_t Overspdl1_missed;
extern uint8_t Overspdl2_missed;
extern uint8_t WIF_missed;
/***************************************************/
#define ECUIdentifier_0x04F7_ARRY_SIZE       7U
extern uint8_t OverSpeedLv1repeatcnt;
extern uint8_t OverSpeedLv2repeatcnt;
extern uint8_t  DID_OVERSPEED_0x04F7_arr[ECUIdentifier_0x04F7_ARRY_SIZE];
extern unsigned int  overspeedlevel1_cntr;
extern unsigned int  overspeedlevel2_cntr;
extern uint8_t Engine_Temp_High_popup_shown;
extern unsigned int RPAS_LV1_COUNTER;
extern unsigned int RPAS_LV2_COUNTER;
extern unsigned int RPAS_LV3_COUNTER;
extern unsigned int RPAS_LV4_COUNTER;
extern _Bool W501_LOW_RPAS_LEVEL1_PRECON;

extern uint8_t W501_LOW_RPAS_COUNTER2;
extern uint8_t W501_LOW_RPAS_COUNTER3;
extern uint8_t W501_LOW_RPAS_COUNTER4;
/*****************************************************/
uint8_t Check_engine_TT_LOD;
uint8_t Check_engine_TT_LSD;
uint8_t MIL_TT_LOD;
uint8_t MIL_TT_LSD;
uint8_t AIR_BAG_TT_LOD;
uint8_t AIR_BAG_TT_LSD;
uint8_t CRUISE_TT_LOD;
uint8_t CRUISE_TT_LSD;
uint8_t ESP_MALFUNCTION_TT_LOD;
uint8_t ESP_MALFUNCTION_TT_LSD;
uint8_t ABS_TT_LOD;
uint8_t ABS_TT_LSD;
uint8_t PARK_BRAKE_TT_LOD;
uint8_t PARK_BRAKE_TT_LSD;
uint8_t AUTO_START_STOP_TT_LOD;
uint8_t AUTO_START_STOP_TT_LSD;
uint8_t AT_FAIL_TT_LOD;
uint8_t AT_FAIL_TT_LSD;

uint16_t Fuel_level_TT[4];
uint16_t temp_show_TT[4];

/***************************************************/

unsigned char prev_MBFM5_100_message_timeout_flag;
uint8_t logo_completed=0;
uint8_t Incorrect_DEF_Detected_popup_shown=0;
uint8_t Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
uint8_t No_restart_Check_DEF_popup_shown=0;
uint8_t No_start_Check_DEF_popup_shown=0;
uint8_t DEF_Dosing_Malfunction_popup_shown=0;
uint8_t DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
uint8_t No_restart_Dose_Malfunction_popup_shown=0;
uint8_t No_start_Dose_Malfunction_popup_shown=0;
uint8_t DEF_Level_Low_popup_shown=0;
uint8_t DEF_level_Low_Engine_Not_Start_popup_shown=0;
uint8_t No_restart_Fill_DEF_popup_shown=0;
uint8_t No_start_Fill_DEF_popup_shown=0;
//uint16_t Eng_No_Start=0;
Split16 Eng_No_Start=0;
_Bool fuel_alert_on_flag;

uint8_t REGEN_Required_popup_shown = 0;
uint8_t REGEN_completed_popup_shown = 0;
uint8_t REGEN_aboarted_popup_shown = 0;
uint8_t REGEN_notstarted_popup_shown = 0;
uint8_t REGEN_started_popup_shown = 0;
uint8_t REGEN_requested_popup_shown = 0;
uint8_t Main_page_Regen = 0;
_Bool HoldOk_Regen_text = 0;
Led_state lp1,lp2,lp1_1,lp2_1;
uint8_t TURN_RIGHT_CPU,HIGH_BEAM_CPU,REAR_FOG_CPU,FOUR_WD_OUTPUT_CPU,PARALLEL_IN_1,PARALLEL_IN_2;
/**********overspeed chime parameters*****************/
uint8_t OverSpeedLevel1Val;
uint8_t OverSpeedLevel2Val;
uint8_t OverSpeedLevel1Type;
uint8_t OverSpeedLv1RepeatIntrval;
uint8_t OverSpeedLevel2Type;
uint8_t OverSpeedLv2RepeatIntrval;
uint8_t OverSpeedHystresis;
/*****************************************************/
uint8_t init_regen_required = 0;	/*Regen Required Alert initiated*/
uint8_t init_regen_aboarted = 0;	/*Regen Aboarted Alert disabled*/
uint8_t init_regen_started = 0;		/*Regen Started Alert disabled*/
uint8_t init_regen_not_started = 0;	/*Regen Not started Alert disabled*/
uint8_t init_regen_completed = 0;	/*Regen Completed Alert Enabled*/
uint8_t REGEN_TT_ON_flag = 0;
_Bool IGN_ON_regen_init = 0;
uint32_t display_key;
uint8_t Regencnt = 0;
uint8_t Regencnt1 = 0;
uint8_t regen_page_skip = 0;
extern uint8_t wait_10sec;
uint8_t ENTRY_DNY_FLAG_TEMP;
uint8_t ENTRY_DNY_FLAG;
extern uint8_t Diag_RW_Vehicle_Moving_final_speed;
extern uint8_t SET_key_press;
extern _Bool gs_complete;
extern uint8_t counter7;
uint8_t STS_Eng_data = 0;
uint8_t STS_Engine= 0; //For RPAS W501
_Bool fuel__temp_udc_enable;
extern uint32_t one_MIN_rpm_counter;
extern uint32_t one_MIN_spd_counter;
/*****************************************************/
_Bool REGEN_Required_popup_skip = 0;
_Bool REGEN_completed_popup_skip = 0;
_Bool REGEN_aboarted_popup_skip = 0;
_Bool REGEN_notstarted_popup_skip = 0;
_Bool REGEN_started_popup_skip = 0;
_Bool REGEN_requested_popup_skip = 0;
/*****************************************************/
_Bool Incorrect_DEF_Detected_popup_skip=0;
_Bool Incorrect_DEF_Detected_Engine_Not_Start_popup_skip=0;
_Bool No_restart_Check_DEF_popup_skip=0;
_Bool No_start_Check_DEF_popup_skip=0;
_Bool DEF_Dosing_Malfunction_popup_skip=0;
_Bool DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip=0;
_Bool No_restart_Dose_Malfunction_popup_skip=0;
_Bool No_start_Dose_Malfunction_popup_skip=0;
_Bool DEF_Level_Low_popup_skip=0;
_Bool DEF_level_Low_Engine_Not_Start_popup_skip=0;
_Bool No_restart_Fill_DEF_popup_skip=0;
_Bool No_start_Fill_DEF_popup_skip=0;
_Bool BS6_permanent_alert = 0;

_Bool check_tt_io_control_once;
_Bool io_pointer_ill_flag;
extern _Bool tst_tt_flag;
_Bool dpf_check = 0;				//for DPF priority
extern _Bool gs_complete_test;
/*****************************************************/
/*rpas_w501*/
_Bool RPASS_self_check_ongoing;
_Bool cycle_RPASS_SELF_CHECK_completed;
_Bool RPAS_SELF_CHECK_ENABLE;
extern _Bool RPAS_self_check_missed;
_Bool W501_RPAS_LEVEL_1_BUZ;
_Bool W501_RPAS_LEVEL_2_BUZ;
_Bool W501_RPAS_LEVEL_3_BUZ;
_Bool W501_RPAS_LEVEL_4_BUZ;
_Bool w501_RPAS_FAIL_CHIME;
_Bool W501_cycle_RPASS_FAIL_CHECK_completed;
_Bool RPAS_fail_check_missed;
_Bool W501_RPASS_fail_check_ongoing;
_Bool DEF_priority_check = 0;
_Bool Error_tone_Active = 0;
_Bool Error_tone_Active_msgmissing = 0;
_Bool Error_tone_Active_msgmissing2 = 0;
_Bool Self_tone_Active = 0;
_Bool Alert_receive_flag = 0;
uint8_t gap_cnt = 100;
extern unsigned int W501_RPAS_FAIL_check_counter500msec;
extern unsigned int  RPAS_self_check_counter500msec;
extern _Bool Error_soundstart;
/****************************************************/
_Bool RPAS_Error_flag=0; /*bug id 21*/
_Bool fuel_enable_tt;

_Bool High_Beam_sts = 1;
extern unsigned int Counter_500ms;
_Bool update_count = 0;
#ifdef GRAYSCALE_TFT
#ifndef U350

_Bool W501_LOW_RPAS_SELF_CHECK_TONE;
extern uint8_t W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr;
_Bool W501_LOW_RPAS_SELF_CHECK_TONE_compl;
/*RPAS_ERROR_TONE*/
_Bool W501_LOW_RPAS_ERROR_TONE_compl = 0;
_Bool W501_LOW_RPAS_ERROR_TONE;
extern uint8_t W501_LOW_Buzz_RPAS_ERR_TONE_ctr;
/*LEVEL1*/
_Bool W501_LOW_RPAS_LEVEL_1_TONE_compl;
_Bool W501_LOW_RPAS_LEVEL_1_BUZ;

/*LEVEL2*/
_Bool W501_LOW_RPAS_LEVEL_2_BUZ;
_Bool W501_LOW_RPAS_LEVEL_2_TONE_compl;
_Bool W501_LOW_priority_lv2;
extern uint8_t W501_LOW_RPAS_LEVEL_2_TONE_ctr;
/*LEVEL3*/
_Bool W501_LOW_RPAS_LEVEL_3_BUZ;
_Bool W501_LOW_RPAS_LEVEL_3_TONE_compl;
_Bool W501_LOW_priority_lv3;
/*LEVEL4*/
_Bool W501_LOW_RPAS_LEVEL_4_BUZ;
_Bool W501_LOW_RPAS_LEVEL_4_TONE_compl;
#endif
#endif
void fuel_did(void)
{
	uint8_t k;
	uint16_t ADC_Count_TT;
	//if(ENTRY_DNY_FLAG==0)
	{
		ENTRY_DNY_FLAG=1;
		for(k=0;k<4;k++)
		{
			Fuel_level_TT[k] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[k];
			ADC_Count_TT = -(0.0833* Fuel_level_TT[k]*Fuel_level_TT[k]) + (32.739*Fuel_level_TT[k]) + 186.15;
			Fuel_level_TT[k] = ADC_Count_TT;
		}
	}

}

void TEMP_did(void)
{
	uint8_t m;

	//if(ENTRY_DNY_FLAG_TEMP==0)
	{
		ENTRY_DNY_FLAG_TEMP=1;
		for(m=0;m<4;m++)
		{
			temp_show_TT[m] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[m];

		}
	}

}
static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
	}bits;
	unsigned char byte;

}two_bit_data;


static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
	}bits;
	unsigned char byte;

}three_bit_data;

static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
		unsigned char  Rx_4:1;
	}bits;
	unsigned char byte;

}four_bit_data_left,four_bit_data_right,four_bit_data_center,four_bit_data_RLC,four_bit_data_RRC,four_bit_data_RL,four_bit_data_RR;

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
		unsigned char  Rx_7:1;
		unsigned char  Rx_8:1;
	}bits;
	unsigned char byte;

}DID_TT_bit_data_set1,DID_TT_bit_data_set2,DID_TT_bit_data_set3,DID_TT_bit_data_set4,DID_TT_bit_data_set5,DID_TT_bit_data_set6;

void IP_SHIFT_DELAY (void)
{
	uint8_t cnts=250;
	for(cnts=250;cnts>0;cnts--);
}
#if 0
void input_shift_register (void)
{
	uint16_t ii=0;

	//R_GPIO_WritePin(10, 8, 0);      /*SHIFT_PL_LOW*/
	//R_GPIO_WritePin(10, 8, 0);      /*SHIFT_PL_LOW*/
	R_GPIO_WritePin(10, 8, 0);      /*SHIFT_PL_LOW*/

	IP_SHIFT_DELAY();
	R_GPIO_WritePin(10, 9, 0);       /*SHIFT_CP_LOW*/
	R_GPIO_WritePin(10, 10, 0);       /*SHIFT_CE_LOW*/
	R_GPIO_WritePin(10, 8, 1);      /*SHIFT_PL_HIGH*/

	lp1_1.state=0;
	lp2_1.state=0;

	//	Ip_shift_reg_1 = 0;
	//	Ip_shift_reg_2 = 0;

	for(ii=0;ii<8;ii++)
	{
		R_GPIO_WritePin(10, 9, 0);       /*SHIFT_CP_LOW*/
		R_GPIO_WritePin(10, 9, 0);       /*SHIFT_CP_LOW*/
		IP_SHIFT_DELAY();
		R_GPIO_WritePin(10, 9, 1);       /*SHIFT_CP_HIGH*/
		R_GPIO_WritePin(10, 9, 1);       /*SHIFT_CP_HIGH*/

		IP_SHIFT_DELAY();
		PARALLEL_IN_1=R_GPIO_PinRead(10,7);
		PARALLEL_IN_2=R_GPIO_PinRead(10,6);

		lp1_1.state = lp1_1.state << 1;
		lp2_1.state = lp2_1.state << 1;

		if(PARALLEL_IN_1)
		{
			lp1_1.state = lp1_1.state | 1;
		}
		if(PARALLEL_IN_2)
		{
			lp2_1.state = lp2_1.state | 1;
		}
		IP_SHIFT_DELAY();
	}

	Ip_shift_reg_1 = lp1_1.state;
	Ip_shift_reg_2 = lp2_1.state;

	R_GPIO_WritePin(10, 10, 1);       /*SHIFT_CE_HIGH*/
}
#endif
void input_shift_register (void)
{
	uint16_t ii=0;
	R_GPIO_WritePin(10, 8, 0);      /*SHIFT_PL_LOW*/
	R_GPIO_WritePin(10, 8, 0);      /*SHIFT_PL_LOW*/
	R_GPIO_WritePin(10, 8, 0);      /*SHIFT_PL_LOW*/

	IP_SHIFT_DELAY();
	R_GPIO_WritePin(10, 9, 1);       /*SHIFT_CP_HIGH*/
	R_GPIO_WritePin(10, 8, 1);      /*SHIFT_PL_HIGH*/
	IP_SHIFT_DELAY();
	R_GPIO_WritePin(10, 10, 0);       /*SHIFT_CE_LOW*/
	R_GPIO_WritePin(10, 9, 0);       /*SHIFT_CP_LOW*/

	Ip_shift_reg_1 = 0;
	Ip_shift_reg_2 = 0;

	for(ii=0;ii<8;ii++)
	{
		Ip_shift_reg_1 = Ip_shift_reg_1 << 1;
		Ip_shift_reg_2 = Ip_shift_reg_2 << 1;

		PARALLEL_IN_1=R_GPIO_PinRead(10, 7);
		PARALLEL_IN_2=R_GPIO_PinRead(42,15);

		if(PARALLEL_IN_1)
		{
			Ip_shift_reg_1 = Ip_shift_reg_1 | 1;
		}
		if(PARALLEL_IN_2)
		{
			Ip_shift_reg_2 = Ip_shift_reg_2 | 1;
		}
		R_GPIO_WritePin(10, 9, 0);       /*SHIFT_CP_LOW*/
		R_GPIO_WritePin(10, 9, 0);       /*SHIFT_CP_LOW*/
		IP_SHIFT_DELAY();
		R_GPIO_WritePin(10, 9, 1);       /*SHIFT_CP_HIGH*/
		R_GPIO_WritePin(10, 9, 1);       /*SHIFT_CP_HIGH*/

		IP_SHIFT_DELAY();
	}
	R_GPIO_WritePin(10, 10, 1);       /*SHIFT_CE_HIGH*/
	ALERT_CHECK=1;
}


void CHECK_SID_DATA (void)
{
	if(Check_Engine_TT == 0xfff ||MIL_TT == 0xfff||AIR_BAG_TT == 0xfff||CRUISE_TT == 0xfff||ESP_MALFUNCTION_TT == 0xfff||
					ABS_TT == 0xfff||PARK_BRAKE_LOW_FLUID_EBD_TT == 0xfff||AUTO_START_STOP_TT ==  0xfff ||AT_FAIL_TT == 0xfff)
			{
				SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
				SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
				cascade_TT_send_data();
				ReadDCData();
				gs_complete_test=0;
			}
			if(Check_Engine_TT==0)
			{
				read_SID[548]=0;
				read_SID[452]=0;
			}
			if(MIL_TT==0)
			{
				read_SID[248]=0;
				read_SID[152]=0;
			}
			if(AIR_BAG_TT==0)
			{
				read_SID[533]=0;
				read_SID[437]=0;
			}
			if(CRUISE_TT==0)
			{
				read_SID[241]=0;
				read_SID[145]=0;
			}
			if(ESP_MALFUNCTION_TT==0)
			{
				read_SID[251]=0;
				read_SID[155]=0;
			}
			if(ABS_TT==0)
			{
				read_SID[550]=0;
				read_SID[454]=0;
			}
			if(PARK_BRAKE_LOW_FLUID_EBD_TT==0)
			{
				read_SID[258]=0;
				read_SID[162]=0;
			}
			if(AUTO_START_STOP_TT==0)
			{
				read_SID[540]=0;
				read_SID[444]=0;
			}
			if(AT_FAIL_TT==0)
			{
				read_SID[549]=0;
				read_SID[453]=0;
			}
	}

void scan_TT_status (void)
{
	STS_Engine = CAN_EMS1_10[6].byte;
	STS_Engine = ((STS_Engine & 0xE0) >> 5);
	uint32_t key;
	uint8_t DEF_LEVEL;
	uint8_t INCORRECT_DEF;
	uint8_t DEF_DOSING_MALFUNC;
	Split16 DIST_DEF_EMPTY;
	//TURN_RIGHT_CPU=R_GPIO_PinRead(0, 2);
	//HIGH_BEAM_CPU=R_GPIO_PinRead(0, 3);
	REAR_FOG_CPU=R_GPIO_PinRead(0, 5);
	_Bool Seat_Belt_Chime=0;
	Alert_receive_flag = 1;				/*Alert_receive_flag added due to DPF alert depends on TT So after scan TT all alert will displayed below DPF*/
	if(sw_press_ok == 0)
	{
		S_LD1_TT=0xfff;
		S_LD2_TT=0xfff;
		S_LD3_TT=0xfff;
		S_LD4_TT=0xfff;
		S_LD5_TT=0xfff;
		S_LD6_TT=0xfff;
		R_LD1_TT=0xfff;
		R_LD2_TT=0xfff;
		R_LD3_TT=0xfff;
		R_LD4_TT=0xfff;
		R_LD5_TT=0xfff;
		R_LD6_TT=0xfff;

		SP_LD1_TT=0xfff;
		SP_LD2_TT=0xfff;
		SP_LD3_TT=0xfff;
		RP_LD1_TT=0xfff;
		RP_LD2_TT=0xfff;
		RP_LD3_TT=0xfff;

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_LD8_TT=0xfff;
			R_LD7_TT=0x000;

		}
		else
		{
			/* GAsoline*/
			R_LD8_TT=0;
			R_LD7_TT=0xfff;
		}
	}

#ifdef U350

	/*RPAS FAIL CHIME FUNCTION LOGIC*/
		if(Diag_RW_RPASS_Fail_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(RPAS2_100_message_timeout_flag == 0)
				{
					five_bit_data.bits.Rx_1 = CAN_RPAS2_100[0].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_RPAS2_100[0].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_RPAS2_100[0].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_RPAS2_100[0].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_RPAS2_100[0].bits.Rx_5;
					if(five_bit_data.byte==0)
					{
						RPAS_ERROR_TONE=0;
						Buzz_RPAS_ERR_TONE_ctr=0;
						RPAS_ERROR_TONE_compl=0;
						error_cnt = 0;
					}
					if(five_bit_data.byte!=0)
					{
						if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000) && RPAS_ERROR_TONE_compl == 0)
						{
							Sound_Bit = 0x4000;
							RPAS_ERROR_TONE=1;
//							error_cnt = 0;

							if(error_cnt == 0)
							{
								error_cnt = 1;
								Buzz_ON=0;
								CounterBz_3sec = 200;
							}
						}
						else
						{
							RPAS_ERROR_TONE=0;
							error_cnt = 0;
						}

					}
					else
					{
						if(Sound_Bit == 0x4000)
						{
							Sound_Bit = 0x0;
						}

						RPAS_ERROR_TONE=0;
						Buzz_RPAS_ERR_TONE_ctr=0;
						RPAS_ERROR_TONE_compl=0;
						error_cnt = 0;
					}
				}
				else
				{
					if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000) && RPAS_ERROR_TONE_compl == 0)
					{
						Sound_Bit = 0x4000;
						RPAS_ERROR_TONE=1;
					}
					else
					{
						RPAS_ERROR_TONE=0;
					}
				}

			}
			else
			{
				if(Sound_Bit == 0x4000)
				{
					Sound_Bit = 0x0;
				}
				RPAS_ERROR_TONE=0;
				Buzz_RPAS_ERR_TONE_ctr=0;
				RPAS_ERROR_TONE_compl=0;
				error_cnt = 0;
			}
		}
		else
		{
			RPAS_ERROR_TONE=0;
			Buzz_RPAS_ERR_TONE_ctr=0;
			RPAS_ERROR_TONE_compl=0;
		}

		/*RPAS_SELF_CHECK*/
			if(Diag_RW_RPASS_Chime_Enable==1)
				{
					if(RPAS_Precondition_Active==1)
					{
						if(RPAS2_100_message_timeout_flag == 0)
						{
							five_bit_data.bits.Rx_1 = CAN_RPAS2_100[0].bits.Rx_1;
							five_bit_data.bits.Rx_2 = CAN_RPAS2_100[0].bits.Rx_2;
							five_bit_data.bits.Rx_3 = CAN_RPAS2_100[0].bits.Rx_3;
							five_bit_data.bits.Rx_4 = CAN_RPAS2_100[0].bits.Rx_4;
							five_bit_data.bits.Rx_5 = CAN_RPAS2_100[0].bits.Rx_5;

							if(five_bit_data.byte!=0)
							{
								/*chime off*/
								RPAS_SELF_CHECK_TONE=0;
							}
							if(five_bit_data.byte==0 && CAN_RPAS2_100[0].bits.Rx_6==1)
							{
								if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x8000 || Sound_Bit == 0x10000 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000 ) && RPAS_SELF_CHECK_TONE_compl == 0)
								{
									Sound_Bit = 0x8000;
									RPAS_SELF_CHECK_TONE=1;
								}
								else
								{
									Sound_Bit = 0x00;
									RPAS_SELF_CHECK_TONE=0;
								}

							}
							else
							{
								if(Sound_Bit == 0x8000)
								{
									Sound_Bit = 0x0;
								}

								RPAS_SELF_CHECK_TONE=0;
								Buzz_RPAS_SELFCHECK_TONE_ctr=0;
								RPAS_SELF_CHECK_TONE_compl=0;
							}
						}

						}
						else
						{
							/*chime_off*/
							if(Sound_Bit == 0x8000)
							{
								Sound_Bit = 0x0;
							}

							RPAS_SELF_CHECK_TONE=0;
							Buzz_RPAS_SELFCHECK_TONE_ctr=0;
							RPAS_SELF_CHECK_TONE_compl=0;
	//						RPAS_SELF_CHECK_TONE=0;
						}


					}
					else
					{
						/*chime_off*/
						RPAS_SELF_CHECK_TONE=0;
					}

		/*RPAS STATUS CHIME FUNCTIONAL LOGIC*/

		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if((RPAS2_100_message_timeout_flag != 0 ) && (RPAS1_20_message_timeout_flag != 0))
				{
				if(RPAS_LEVEL_1_BUZ  ==0 && RPAS_LEVEL1_PRECON == 1)
				{
					if((Sound_Bit == 0x10000)||(Sound_Bit == 0x4000))//sound bit 4000 to allow level1 chime stop
					{
						RPAS_LEVEL1_PRECON = 0;
						if(Sound_Bit == 0x10000)//to avoid resetting of sound bit if RPAS ERROR is ON
						{
							Sound_Bit = 0x0;
						}
						SoundPCM_RPAS_buzz_LV1_STOP();
					}

				}
				//if(RPAS_LEVEL_2_BUZ==0 && RPAS_LEVEL2_BUZ_ONGOING == 1)
				{
					if(Sound_Bit == 0x20000)
					{
						RPAS_LEVEL2_BUZ_ONGOING=0;
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV2_STOP();
					}
				}
				//if(RPAS_LEVEL_3_BUZ==0 && RPAS_LEVEL3_BUZ_ONGOING==1)
				{
					if(Sound_Bit == 0x30000)
					{
						RPAS_LEVEL3_BUZ_ONGOING=0;
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV3_STOP();
					}
				}
				//if(RPAS_LEVEL_4_BUZ==0 && RPAS_LEVEL4_BUZ_ONGOING==1)
				{
					if(Sound_Bit == 0x40000)
					{
						RPAS_LEVEL4_BUZ_ONGOING=0;
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV4_STOP();
					}
				}
					/*chime off*/

					RPAS_LEVEL_1_BUZ=0;
					RPAS_LEVEL_2_BUZ=0;
					RPAS_LEVEL_3_BUZ=0;
					RPAS_LEVEL_4_BUZ=0;
				}
				if((RPAS2_100_message_timeout_flag == 0 ) && (RPAS1_20_message_timeout_flag == 0))
				{
					five_bit_data.bits.Rx_1 = CAN_RPAS2_100[0].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_RPAS2_100[0].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_RPAS2_100[0].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_RPAS2_100[0].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_RPAS2_100[0].bits.Rx_5;
					if(five_bit_data.byte!=0)
					{
						/*chime off*/

						RPAS_LEVEL_1_BUZ=0;
						RPAS_LEVEL_2_BUZ=0;
						RPAS_LEVEL_3_BUZ=0;
						RPAS_LEVEL_4_BUZ=0;
					}
					if(five_bit_data.byte == 0)
					{

						four_bit_data_left.bits.Rx_1 = CAN_RPAS1_20[2].bits.Rx_5;
						four_bit_data_left.bits.Rx_2 = CAN_RPAS1_20[2].bits.Rx_6;
						four_bit_data_left.bits.Rx_3 = CAN_RPAS1_20[2].bits.Rx_7;
						four_bit_data_left.bits.Rx_4 = CAN_RPAS1_20[2].bits.Rx_8;

						four_bit_data_right.bits.Rx_1 = CAN_RPAS1_20[3].bits.Rx_5;
						four_bit_data_right.bits.Rx_2 = CAN_RPAS1_20[3].bits.Rx_6;
						four_bit_data_right.bits.Rx_3 = CAN_RPAS1_20[3].bits.Rx_7;
						four_bit_data_right.bits.Rx_4 = CAN_RPAS1_20[3].bits.Rx_8;

						four_bit_data_center.bits.Rx_1 = CAN_RPAS1_20[3].bits.Rx_1;
						four_bit_data_center.bits.Rx_2 = CAN_RPAS1_20[3].bits.Rx_2;
						four_bit_data_center.bits.Rx_3 = CAN_RPAS1_20[3].bits.Rx_3;
						four_bit_data_center.bits.Rx_4 = CAN_RPAS1_20[3].bits.Rx_4;


						if((four_bit_data_left.byte == 4 || four_bit_data_right.byte == 4 || four_bit_data_center.byte == 4)&& CAN_RPAS2_100[0].bits.Rx_6==1)
						{
							/*chime level_1*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								RPAS_LEVEL_1_BUZ=1;
								RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								RPAS_LEVEL_1_BUZ=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x10000)
							{
								//r Sound_Bit=0x0;
								RPAS_LEVEL_1_TONE_compl=0;
							}
							RPAS_LEVEL_1_BUZ=0;
							RPAS_LEVEL_1_TONE_ctr=0;
						}

						if((four_bit_data_left.byte == 3 || four_bit_data_right.byte==3 || four_bit_data_center.byte==3)&& CAN_RPAS2_100[0].bits.Rx_6==1)
						{
							/*chime level_2*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								RPAS_LEVEL_2_BUZ=1;
								RPAS_LEVEL_2_TONE_compl=0;
								priority_lv2=1;
							}
						}
						else
						{
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								RPAS_LEVEL_2_TONE_compl=0;
							}
							RPAS_LEVEL_2_BUZ=0;
							RPAS_LEVEL_2_TONE_ctr=0;
							priority_lv2=0;
						}

						if((four_bit_data_left.byte == 2 || four_bit_data_right.byte == 2 || four_bit_data_center.byte == 2)&& (CAN_RPAS2_100[0].bits.Rx_6==1)&&(priority_lv2==0))
						{
							/*chime level_3*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								RPAS_LEVEL_3_BUZ=1;
								RPAS_LEVEL_3_TONE_compl=0;
								priority_lv3=1;
							}

						}
						else
						{
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								RPAS_LEVEL_3_TONE_compl=0;
							}
							RPAS_LEVEL_3_BUZ=0;
							RPAS_LEVEL_3_TONE_ctr=0;
							priority_lv3=0;
						}
						if((four_bit_data_left.byte == 1 || four_bit_data_right.byte == 1 || four_bit_data_center.byte == 1)&& CAN_RPAS2_100[0].bits.Rx_6==1 && priority_lv3==0 &&priority_lv2==0)
						{
							/*chime level_4*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000)
							{
								Sound_Bit = 0x40000;
								RPAS_LEVEL_4_BUZ=1;
								RPAS_LEVEL_4_TONE_compl=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit=0x0;
								RPAS_LEVEL_4_TONE_compl=0;
							}
							RPAS_LEVEL_4_BUZ=0;
							RPAS_LEVEL_4_TONE_ctr=0;
						}


					}

					if(four_bit_data_left.byte == 0x5 ||four_bit_data_left.byte == 0x6 ||four_bit_data_left.byte == 0x7 || four_bit_data_left.byte == 0x8
							||four_bit_data_left.byte == 0x9 ||four_bit_data_left.byte == 0xA ||four_bit_data_left.byte == 0xB||four_bit_data_left.byte == 0xC
							|| four_bit_data_left.byte == 0xD ||four_bit_data_left.byte == 0xE ||four_bit_data_left.byte == 0xF)
					{
						/*chime off*/
					/*	RPAS_LEVEL_1_BUZ=0;
						RPAS_LEVEL_2_BUZ=0;
						RPAS_LEVEL_3_BUZ=0;
						RPAS_LEVEL_4_BUZ=0;
					*/	Invalid_left_RPAS=1;

					}
					else
					{
						Invalid_left_RPAS = 0;
					}
					if( four_bit_data_right.byte == 0x5 ||four_bit_data_right.byte == 0x6 ||four_bit_data_right.byte == 0x7 || four_bit_data_right.byte == 0x8
							||four_bit_data_right.byte == 0x9 ||four_bit_data_right.byte == 0xA ||four_bit_data_left.byte == 0xB||four_bit_data_right.byte == 0xC
							|| four_bit_data_right.byte == 0xD ||four_bit_data_right.byte == 0xE ||four_bit_data_right.byte == 0xF)
					{
						/*chime off*/
					/*	RPAS_LEVEL_1_BUZ=0;
						RPAS_LEVEL_2_BUZ=0;
						RPAS_LEVEL_3_BUZ=0;
						RPAS_LEVEL_4_BUZ=0;
					*/	Invalid_right_RPAS=1;

					}
					else
					{
						Invalid_right_RPAS = 0;
					}
					if( four_bit_data_center.byte == 0x5 ||four_bit_data_center.byte == 0x6 ||four_bit_data_center.byte == 0x7 || four_bit_data_center.byte == 0x8
							||four_bit_data_center.byte == 0x9 ||four_bit_data_center.byte == 0xA ||four_bit_data_center.byte == 0xB||four_bit_data_center.byte == 0xC
							|| four_bit_data_center.byte == 0xD ||four_bit_data_center.byte == 0xE ||four_bit_data_center.byte == 0xF)
					{
						/*chime off*/
					/*	RPAS_LEVEL_1_BUZ=0;
						RPAS_LEVEL_2_BUZ=0;
						RPAS_LEVEL_3_BUZ=0;
						RPAS_LEVEL_4_BUZ=0;
					*/	Invalid_center_RPAS=1;

					}
					else
					{
						Invalid_center_RPAS = 0;
					}
					if((four_bit_data_left.byte == 0x0 && four_bit_data_right.byte == 0x0 && four_bit_data_center.byte == 0x0)
							||(Invalid_left_RPAS == 1 && Invalid_right_RPAS == 1 && Invalid_center_RPAS == 1))
						/*Chime will be off only if all bars are 0 or all are invalid */
					{
						/*chime off*/
						RPAS_LEVEL_1_BUZ=0;
						RPAS_LEVEL_2_BUZ=0;
						RPAS_LEVEL_3_BUZ=0;
						RPAS_LEVEL_4_BUZ=0;
					}

				}
				if(RPAS2_100_message_timeout_flag != 0 )
				{
					if(RPAS_LEVEL_1_BUZ  ==0 && RPAS_LEVEL1_PRECON == 1)
					{
						if((Sound_Bit == 0x10000)||(Sound_Bit == 0x4000))//sound bit 4000 to allow level1 chime stop
						{
							RPAS_LEVEL1_PRECON = 0;
							if(Sound_Bit == 0x10000)//to avoid resetting of sound bit if RPAS ERROR is ON
							{
								Sound_Bit = 0x0;
							}
							SoundPCM_RPAS_buzz_LV1_STOP();
						}
					}
					//if(RPAS_LEVEL_2_BUZ==0 && RPAS_LEVEL2_BUZ_ONGOING == 1)
					{
						if(Sound_Bit == 0x20000)
						{
							RPAS_LEVEL2_BUZ_ONGOING=0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV2_STOP();
						}
					}
					//if(RPAS_LEVEL_3_BUZ==0 && RPAS_LEVEL3_BUZ_ONGOING==1)
					{
						if(Sound_Bit == 0x30000)
						{
							RPAS_LEVEL3_BUZ_ONGOING=0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV3_STOP();
						}
					}
					//if(RPAS_LEVEL_4_BUZ==0 && RPAS_LEVEL4_BUZ_ONGOING==1)
					{
						if(Sound_Bit == 0x40000)
						{
							RPAS_LEVEL4_BUZ_ONGOING=0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV4_STOP();
						}
					}

					/*chime off*/
					RPAS_LEVEL_1_BUZ=0;
					RPAS_LEVEL_2_BUZ=0;
					RPAS_LEVEL_3_BUZ=0;
					RPAS_LEVEL_4_BUZ=0;
				}

			}
			else
			{
				if(Sound_Bit == 0x10000)
				{
				//r	Sound_Bit=0x0;
					RPAS_LEVEL_1_TONE_compl=0;
				}
				RPAS_LEVEL_1_BUZ=0;
				RPAS_LEVEL_1_TONE_ctr=0;

				if(Sound_Bit == 0x20000)
				{
					Sound_Bit=0x0;
					RPAS_LEVEL_2_TONE_compl=0;
				}
				RPAS_LEVEL_2_BUZ=0;
				RPAS_LEVEL_2_TONE_ctr=0;
				priority_lv2=0;

				if(Sound_Bit == 0x30000)
				{
					Sound_Bit=0x0;
					RPAS_LEVEL_3_TONE_compl=0;
				}
				RPAS_LEVEL_3_BUZ=0;
				RPAS_LEVEL_3_TONE_ctr=0;
				priority_lv3=0;

				if(Sound_Bit == 0x40000)
				{
					Sound_Bit=0x0;
					RPAS_LEVEL_4_TONE_compl=0;
				}
				RPAS_LEVEL_4_BUZ=0;
				RPAS_LEVEL_4_TONE_ctr=0;

//				RPAS_LEVEL_1_BUZ=0;
//				RPAS_LEVEL_2_BUZ=0;
//				RPAS_LEVEL_3_BUZ=0;
//				RPAS_LEVEL_4_BUZ=0;
					/*chime off*/
			}
		}
		else
		{
			RPAS_LEVEL_1_BUZ=0;
			RPAS_LEVEL_2_BUZ=0;
			RPAS_LEVEL_3_BUZ=0;
			RPAS_LEVEL_4_BUZ=0;
			/*chime off*/
		}
#endif


#ifdef COLOR_TFT

	/*RPAS CHIMES-W501*/
	/**************************RPAS_FAIL_CHM_FUNCTION_LOGIC*************************/

		if(Diag_RW_RPASS_Fail_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)
				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;
//					W501_RPAS_FAIL_check_counter500msec = 0;

					if((five_bit_data.byte!=0)/*&&(W501_RPASS_fail_check_ongoing == 0)*//*&&(chime_start1 == 0)&&(W501_cycle_RPASS_FAIL_CHECK_completed==0)*/)
					{
						RPAS_LV1_COUNTER = 0;
						RPAS_LV2_COUNTER = 0;
						RPAS_LV3_COUNTER = 0;
						RPAS_LV4_COUNTER = 0;
						W501_RPAS_LEVEL_1_BUZ=0;
						W501_RPAS_LEVEL_2_BUZ=0;
						W501_RPAS_LEVEL_3_BUZ=0;
						W501_RPAS_LEVEL_4_BUZ=0;
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV3_STOP();
						}
						if(Sound_Bit == 0x40000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV4_STOP();
						}

						if(Error_tone_Active == 0)
						{
							W501_RPAS_FAIL_check_counter500msec = 0;
							W501_cycle_RPASS_FAIL_CHECK_completed = 0;
							Error_tone_Active = 1;
						}

						if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&(W501_cycle_RPASS_FAIL_CHECK_completed == 0))
						{
							Sound_Bit = 0x4000;
							W501_RPASS_fail_check_ongoing=1;
							chime_start1=1;
							RPAS_fail_check_missed=0;
							RPAS_Error_flag=1;
							Error_tone_Active_msgmissing = 0;

						}
						else
						{
							RPAS_Error_flag=0;
							W501_RPASS_fail_check_ongoing=0;
							RPAS_fail_check_missed=0;
	//						Error_tone_Active = 0;
						}

					}

					else if(five_bit_data.byte==0)
					{/*chime off*/
						RPAS_fail_check_missed=1;
						W501_RPASS_fail_check_ongoing=0;
						W501_cycle_RPASS_FAIL_CHECK_completed=0;
						   RPAS_Error_flag=0;
						if(Sound_Bit == 0x4000)
						{
							Sound_Bit = 0x00;						/**Sound bit CLEAR**/
							SoundPCM_Reverse_cont_STOP();
						}
						Error_tone_Active = 0;
						Error_soundstart = 0;

					}
				}
				else
				{
					RPAS_LV1_COUNTER = 0;
					RPAS_LV2_COUNTER = 0;
					RPAS_LV3_COUNTER = 0;
					RPAS_LV4_COUNTER = 0;
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
					W501_RPAS_LEVEL_4_BUZ=0;
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV3_STOP();
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV4_STOP();
					}
					if(Error_tone_Active_msgmissing == 0)
					{
						W501_RPAS_FAIL_check_counter500msec = 0;
						W501_cycle_RPASS_FAIL_CHECK_completed = 0;
						Error_tone_Active_msgmissing = 1;
					}
					if(((Sound_Bit == 0x0) || Sound_Bit == 0x200 || (Sound_Bit == 0x4000)|| (Sound_Bit == 0x8000)|| (Sound_Bit == 0x10000)|| (Sound_Bit == 0x20000)|| (Sound_Bit == 0x30000)|| (Sound_Bit == 0x40000))&&(W501_cycle_RPASS_FAIL_CHECK_completed == 0)&&((reverse_played == 1)||(STS_Engine!=2)))
					{
						Sound_Bit = 0x4000;
						W501_RPASS_fail_check_ongoing=1;
						/*chime_start1=1;*/
						RPAS_fail_check_missed=0;
						RPAS_Error_flag=1;/* at message time out the rpas error tone required*/
						Error_tone_Active = 0;
					}
					else
					{
						W501_RPASS_fail_check_ongoing=0;
						RPAS_fail_check_missed=0;
					}
				}

			}
			else
			{
				if(Sound_Bit == 0x4000)
				{
					Sound_Bit = 0x0;
					SoundPCM_Reverse_cont_STOP();
				}
				W501_cycle_RPASS_FAIL_CHECK_completed = 0;
				W501_RPASS_fail_check_ongoing=0;
				RPAS_Error_flag=0;
				Error_tone_Active_msgmissing = 0;
				Error_tone_Active = 0;
			}
		}
		else
		{
			W501_RPASS_fail_check_ongoing=0;
			RPAS_Error_flag=0;
		}

		/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........RPAS self check tone*/
		if(Diag_RW_RPASS_Self_check_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)

			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{

					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;
					if(five_bit_data.byte==0)
					{
						W501_RPAS_FAIL_check_counter500msec = 0;
					}
					if((five_bit_data.byte==0)&&(RPASS_self_check_ongoing == 0)/*&&(chime_start1 == 0)*/&&(cycle_RPASS_SELF_CHECK_completed == 0))
					{

						if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x8000)&&((reverse_played == 1)||(STS_Engine!=2)))
						{
							Sound_Bit = 0x8000;
							RPASS_self_check_ongoing=1;
//							chime_start1=1;
							RPAS_self_check_missed=0;
							if(Self_tone_Active == 0)
							{
								RPAS_self_check_counter500msec = (Diag_RW_RPAS_self_check_chime_Enable/10)-5;
								Self_tone_Active = 1;
							}
						}
						else
						{
							RPASS_self_check_ongoing=0;
							RPAS_self_check_missed=0;
						}
					}
					else if(five_bit_data.byte!=0)
					{
						/*chime off*/
						RPAS_self_check_missed=1;
						RPASS_self_check_ongoing=0;
						cycle_RPASS_SELF_CHECK_completed=0;
						cycle_RPAS_self_check_cntr = 0;
						if(Sound_Bit == 0x8000)
						{
							Sound_Bit = 0x00;						/**Sound bit CLEAR**/
							SoundPCM_Reverse_cont_STOP();
						}
						Self_tone_Active = 0;
					}
				}
				else
				{
					if(Sound_Bit == 0x8000)
					{
						Sound_Bit = 0x0;
						SoundPCM_Reverse_cont_STOP();
					}

					RPASS_self_check_ongoing=0;
					RPAS_self_check_missed=1;
					cycle_RPASS_SELF_CHECK_completed = 0;
					cycle_RPAS_self_check_cntr = 0;
				}
			}
			else
			{
				if(Sound_Bit == 0x8000)
				{
					Sound_Bit = 0x0;
					SoundPCM_Reverse_cont_STOP();
				}

				RPASS_self_check_ongoing=0;
				RPAS_self_check_missed=1;
				cycle_RPASS_SELF_CHECK_completed = 0;
			}
		}
		else
		{
			RPASS_self_check_ongoing=0;
			RPAS_self_check_missed=1;
			cycle_RPASS_SELF_CHECK_completed = 0;
		}




	/******RPAS STATUS CHIME FUNCTIONAL LOGIC*******/
	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X01*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 1)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)
				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
						four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
						four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
						four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;


						if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
						{
							/*chime level_1*/
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
								}
								W501_RPAS_LEVEL_1_BUZ=0;
								RPAS_LV1_COUNTER = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
							}
							W501_RPAS_LEVEL_1_BUZ=0;
							RPAS_LV1_COUNTER = 0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit = 0x0;

							}
						}

						if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_LV2_STOP();
								}
								W501_RPAS_LEVEL_2_BUZ=0;
								RPAS_LV2_COUNTER = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_LV2_STOP();
							}
							W501_RPAS_LEVEL_2_BUZ=0;
							RPAS_LV2_COUNTER = 0;
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit = 0x0;

							}
						}

						if(four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_LV3_STOP();
								}
								W501_RPAS_LEVEL_3_BUZ=0;
								RPAS_LV3_COUNTER = 0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_LV3_STOP();
							}
							W501_RPAS_LEVEL_3_BUZ=0;
							RPAS_LV3_COUNTER = 0;
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit = 0x0;

							}
						}
					}
					else
					{
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV3_STOP();
						}
						RPAS_LV1_COUNTER = 0;
						RPAS_LV2_COUNTER = 0;
						RPAS_LV3_COUNTER = 0;
						W501_RPAS_LEVEL_1_BUZ=0;
						W501_RPAS_LEVEL_2_BUZ=0;
						W501_RPAS_LEVEL_3_BUZ=0;
					}
				}
				else
				{
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV3_STOP();
					}
					RPAS_LV1_COUNTER = 0;
					RPAS_LV2_COUNTER = 0;
					RPAS_LV3_COUNTER = 0;
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
				}
			}
			else
			{
				RPAS_LV1_COUNTER = 0;
				RPAS_LV2_COUNTER = 0;
				RPAS_LV3_COUNTER = 0;
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV2_STOP();
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV3_STOP();
				}
				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
			}
		}
		else
		{
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
		}
	}

	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X02*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 2)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)
				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
						four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
						four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
						four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;

						if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
						{
							/*chime level_1*/
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
								}
								W501_RPAS_LEVEL_1_BUZ=0;
								RPAS_LV1_COUNTER = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
							}
							W501_RPAS_LEVEL_1_BUZ=0;
							RPAS_LV1_COUNTER = 0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit = 0x0;

							}
						}
						/*chime level_2*/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_LV2_STOP();
								}
								W501_RPAS_LEVEL_2_BUZ=0;
								RPAS_LV2_COUNTER = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_LV2_STOP();
							}
							W501_RPAS_LEVEL_2_BUZ=0;
							RPAS_LV2_COUNTER = 0;
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit = 0x0;

							}
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_LV3_STOP();
								}
								W501_RPAS_LEVEL_3_BUZ=0;
								RPAS_LV3_COUNTER = 0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_LV3_STOP();
							}
							W501_RPAS_LEVEL_3_BUZ=0;
							RPAS_LV3_COUNTER = 0;
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit = 0x0;

							}
						}
						/*chime level_4*/
						if(four_bit_data_RLC.byte==4 || four_bit_data_RRC.byte==4)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x40000;
								W501_RPAS_LEVEL_4_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_4_BUZ==1)
								{
									SoundPCM_RPAS_LV4_STOP();
								}
								W501_RPAS_LEVEL_4_BUZ=0;
								RPAS_LV4_COUNTER = 0;
								if(Sound_Bit == 0x40000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_4_BUZ==1)
							{
								SoundPCM_RPAS_LV4_STOP();
							}
							W501_RPAS_LEVEL_4_BUZ=0;
							RPAS_LV4_COUNTER = 0;
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x0;

							}
						}
					}
					else
					{
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV3_STOP();
						}
						if(Sound_Bit == 0x40000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV4_STOP();
						}

						RPAS_LV1_COUNTER = 0;
						RPAS_LV2_COUNTER = 0;
						RPAS_LV3_COUNTER = 0;
						RPAS_LV4_COUNTER = 0;
						W501_RPAS_LEVEL_1_BUZ=0;
						W501_RPAS_LEVEL_2_BUZ=0;
						W501_RPAS_LEVEL_3_BUZ=0;
						W501_RPAS_LEVEL_4_BUZ=0;
					}
				}
				else
				{
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV3_STOP();
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV4_STOP();
					}

					RPAS_LV1_COUNTER = 0;
					RPAS_LV2_COUNTER = 0;
					RPAS_LV3_COUNTER = 0;
					RPAS_LV4_COUNTER = 0;
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
					W501_RPAS_LEVEL_4_BUZ=0;
				}
			}
			else
			{
				RPAS_LV1_COUNTER = 0;
				RPAS_LV2_COUNTER = 0;
				RPAS_LV3_COUNTER = 0;
				RPAS_LV4_COUNTER = 0;
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV2_STOP();
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV3_STOP();
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV4_STOP();
				}

				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
				W501_RPAS_LEVEL_4_BUZ=0;
			}
		}
		else
		{
			RPAS_LV1_COUNTER = 0;
			RPAS_LV2_COUNTER = 0;
			RPAS_LV3_COUNTER = 0;
			RPAS_LV4_COUNTER = 0;
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
			W501_RPAS_LEVEL_4_BUZ=0;
		}
	}

	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X03*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 3)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							/*chime level_1*/
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
								}
								W501_RPAS_LEVEL_1_BUZ=0;
								RPAS_LV1_COUNTER = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
							}
							RPAS_LV1_COUNTER = 0;
							W501_RPAS_LEVEL_1_BUZ=0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit = 0x0;

							}
						}
						/*chime level_2*/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;


							}
							else
							{
								if(W501_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_LV2_STOP();
								}
								W501_RPAS_LEVEL_2_BUZ=0;
								RPAS_LV2_COUNTER = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_LV2_STOP();
							}
							W501_RPAS_LEVEL_2_BUZ=0;
							RPAS_LV2_COUNTER = 0;
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit = 0x0;
							}
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_LV3_STOP();
								}
								W501_RPAS_LEVEL_3_BUZ=0;
								RPAS_LV3_COUNTER = 0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_LV3_STOP();
							}
							W501_RPAS_LEVEL_3_BUZ=0;
							RPAS_LV3_COUNTER = 0;
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit = 0x0;

							}
						}
					}
					else
					{
						RPAS_LV1_COUNTER = 0;
						RPAS_LV2_COUNTER = 0;
						RPAS_LV3_COUNTER = 0;
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV3_STOP();
						}
						W501_RPAS_LEVEL_1_BUZ=0;
						W501_RPAS_LEVEL_2_BUZ=0;
						W501_RPAS_LEVEL_3_BUZ=0;
					}
				}
				else
				{
					RPAS_LV1_COUNTER = 0;
					RPAS_LV2_COUNTER = 0;
					RPAS_LV3_COUNTER = 0;
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV3_STOP();
					}
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
				}
			}
			else
			{
				RPAS_LV1_COUNTER = 0;
				RPAS_LV2_COUNTER = 0;
				RPAS_LV3_COUNTER = 0;
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV2_STOP();
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV3_STOP();
				}
				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
			}
		}
		else
		{
			RPAS_LV1_COUNTER = 0;
			RPAS_LV2_COUNTER = 0;
			RPAS_LV3_COUNTER = 0;
			if(Sound_Bit == 0x10000)
			{
				Sound_Bit = 0x0;
				SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
			}
			if(Sound_Bit == 0x20000)
			{
				Sound_Bit = 0x0;
				SoundPCM_RPAS_LV2_STOP();
			}
			if(Sound_Bit == 0x30000)
			{
				Sound_Bit = 0x0;
				SoundPCM_RPAS_LV3_STOP();
			}
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
		}
	}
	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X04*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 4)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							/*chime level_1*/
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
								}
								W501_RPAS_LEVEL_1_BUZ=0;
								RPAS_LV1_COUNTER = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;

								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
							}
							RPAS_LV1_COUNTER = 0;
							W501_RPAS_LEVEL_1_BUZ=0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit = 0x0;

							}

						}
						/*chime level_2*/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_LV2_STOP();
								}
								W501_RPAS_LEVEL_2_BUZ=0;
								RPAS_LV2_COUNTER = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_LV2_STOP();
							}
							W501_RPAS_LEVEL_2_BUZ=0;
							RPAS_LV2_COUNTER = 0;
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit = 0x0;

							}
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_LV3_STOP();
								}
								W501_RPAS_LEVEL_3_BUZ=0;
								RPAS_LV3_COUNTER = 0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_LV3_STOP();
							}
							W501_RPAS_LEVEL_3_BUZ=0;
							RPAS_LV3_COUNTER = 0;
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit = 0x0;

							}
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==4 || four_bit_data_RL.byte==4 || four_bit_data_RR.byte==4)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x40000;
								W501_RPAS_LEVEL_4_BUZ=1;
							}
							else
							{
								if(W501_RPAS_LEVEL_4_BUZ==1)
								{
									SoundPCM_RPAS_LV4_STOP();
								}
								W501_RPAS_LEVEL_4_BUZ=0;
								RPAS_LV4_COUNTER = 0;
								if(Sound_Bit == 0x40000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							if(W501_RPAS_LEVEL_4_BUZ==1)
							{
								SoundPCM_RPAS_LV4_STOP();
							}
							W501_RPAS_LEVEL_4_BUZ=0;
							RPAS_LV4_COUNTER = 0;
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x0;

							}
						}
					}
					else
					{
						RPAS_LV1_COUNTER = 0;
						RPAS_LV2_COUNTER = 0;
						RPAS_LV3_COUNTER = 0;
						RPAS_LV4_COUNTER = 0;
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV3_STOP();
						}
						if(Sound_Bit == 0x40000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_LV4_STOP();
						}
						W501_RPAS_LEVEL_1_BUZ=0;
						W501_RPAS_LEVEL_2_BUZ=0;
						W501_RPAS_LEVEL_3_BUZ=0;
						W501_RPAS_LEVEL_4_BUZ = 0;
					}
				}
				else
				{
					RPAS_LV1_COUNTER = 0;
					RPAS_LV2_COUNTER = 0;
					RPAS_LV3_COUNTER = 0;
					RPAS_LV4_COUNTER = 0;
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV3_STOP();
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_LV4_STOP();
					}
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
					W501_RPAS_LEVEL_4_BUZ = 0;
				}
			}
			else
			{
				RPAS_LV1_COUNTER = 0;
				RPAS_LV2_COUNTER = 0;
				RPAS_LV3_COUNTER = 0;
				RPAS_LV4_COUNTER = 0;
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV2_STOP();
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV3_STOP();
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_LV4_STOP();
				}
				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
				W501_RPAS_LEVEL_4_BUZ = 0;
			}
		}
		else
		{
			RPAS_LV1_COUNTER = 0;
			RPAS_LV2_COUNTER = 0;
			RPAS_LV3_COUNTER = 0;
			RPAS_LV4_COUNTER = 0;
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
			W501_RPAS_LEVEL_4_BUZ = 0;
		}
	}
#endif


#ifdef GRAYSCALE_TFT
#ifndef U350
	/*RPAS CHIMES-W501*/



	/*RPAS_FAIL_CHIME_FUNCTION*/

	if(Diag_RW_RPASS_Fail_Chime_Enable==1)
	{
		if(RPAS_Precondition_Active==1)
		{
			if(MBFM_PAS1_50_message_timeout_flag == 0)
			{
				five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
				five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
				five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
				five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
				five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

				if(five_bit_data.byte==0)
				{
					RPAS_ERROR_TONE=0;
					Buzz_RPAS_ERR_TONE_ctr=0;
					RPAS_ERROR_TONE_compl=0;
				}
				if(five_bit_data.byte!=0)
				{
					W501_LOW_RPAS_LEVEL1_PRECON = 0;
					W501_LOW_RPAS_COUNTER2 = 0;
					W501_LOW_RPAS_COUNTER3 = 0;
					W501_LOW_RPAS_COUNTER4 = 0;
					W501_LOW_RPAS_LEVEL_1_BUZ=0;
					W501_LOW_RPAS_LEVEL_2_BUZ=0;
					W501_LOW_RPAS_LEVEL_3_BUZ=0;
					W501_LOW_RPAS_LEVEL_4_BUZ=0;
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV1_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV3_STOP();
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV4_STOP();
					}

					if(W501LOW_error_cnt == 0)
					{

						W501LOW_error_cnt = 1;
						W501LOW_error_cnt2 = 1;
	//					Buzz_ON=0;
						W501_LOW_CounterBz_3sec = W501_LOW_DID_RPAS_ERROR_TONE_CHECK - gap_cnt;
						W501_LOW_RPAS_ERROR_TONE_compl = 0;
						W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;

					}
					if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)) && (W501_LOW_RPAS_ERROR_TONE_compl == 0))
					{
						Sound_Bit = 0x4000;
						W501_LOW_RPAS_ERROR_TONE=1;
						Error_tone_Active_msgmissing = 0;

						if(W501LOW_error_cnt2 == 1)
						{
							W501LOW_error_cnt2 = 0;
							Buzz_ON=1;
							W501_LOW_CounterBz_3sec = W501_LOW_DID_RPAS_ERROR_TONE_CHECK - gap_cnt;
						}
					}
					else
					{
						W501_LOW_RPAS_ERROR_TONE=0;
//						W501LOW_error_cnt = 0;
					}

				}
				else
				{
					if(Sound_Bit == 0x4000)
					{
						Sound_Bit = 0x0;
					}

					W501_LOW_RPAS_ERROR_TONE=0;
					W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
					W501_LOW_RPAS_ERROR_TONE_compl=0;
					W501LOW_error_cnt = 0;
				}
			}
			else
			{
				W501_LOW_RPAS_LEVEL1_PRECON = 0;
				W501_LOW_RPAS_COUNTER2 = 0;
				W501_LOW_RPAS_COUNTER3 = 0;
				W501_LOW_RPAS_COUNTER4 = 0;
				W501_LOW_RPAS_LEVEL_1_BUZ=0;
				W501_LOW_RPAS_LEVEL_2_BUZ=0;
				W501_LOW_RPAS_LEVEL_3_BUZ=0;
				W501_LOW_RPAS_LEVEL_4_BUZ=0;
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV1_STOP();
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV2_STOP();
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV3_STOP();
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV4_STOP();
				}

				if(Error_tone_Active_msgmissing == 0)
				{
					Error_tone_Active_msgmissing = 1;
					Error_tone_Active_msgmissing2 = 1;
		//			Buzz_ON=0;
					W501_LOW_CounterBz_3sec = W501_LOW_DID_RPAS_ERROR_TONE_CHECK - gap_cnt;
					W501_LOW_RPAS_ERROR_TONE_compl = 0;
					W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
				}
				if(((Sound_Bit == 0x0) || (Sound_Bit == 0x200) || (Sound_Bit == 0x4000)) && (W501_LOW_RPAS_ERROR_TONE_compl == 0)&&((reverse_played == 1)||(STS_Engine!=2)))
				{
					Sound_Bit = 0x4000;
					W501_LOW_RPAS_ERROR_TONE=1;
					W501LOW_error_cnt = 0;
					if(Error_tone_Active_msgmissing2 == 1)
					{
						Error_tone_Active_msgmissing2 = 0;
						Buzz_ON=1;
						W501_LOW_CounterBz_3sec = W501_LOW_DID_RPAS_ERROR_TONE_CHECK - gap_cnt;
					}

				}
				else
				{
					W501_LOW_RPAS_ERROR_TONE=0;
					W501LOW_error_cnt = 0;
				}
			}

		}
		else
		{
			if(Sound_Bit == 0x4000)
			{
				Sound_Bit = 0x0;
			}

			W501_LOW_RPAS_ERROR_TONE=0;
			W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
			W501_LOW_RPAS_ERROR_TONE_compl=0;
			W501LOW_error_cnt = 0;
			Error_tone_Active_msgmissing = 0;

		}
	}
	else
	{
		W501_LOW_RPAS_ERROR_TONE=0;
		W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
		W501_LOW_RPAS_ERROR_TONE_compl=0;
	}


	/*RPAS self check tone*/
	if(Diag_RW_RPASS_Self_check_Chime_Enable==1)
	{
		if(RPAS_Precondition_Active==1)
		{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{

					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;


					if(five_bit_data.byte==0)
					{
						if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x8000) && (W501_LOW_RPAS_SELF_CHECK_TONE_compl == 0)&&((reverse_played == 1)||(STS_Engine!=2)))
						{
							Sound_Bit = 0x8000;
							W501_LOW_RPAS_SELF_CHECK_TONE=1;
							if(W501LOW_selfcheck_cnt == 0)
							{
								W501LOW_selfcheck_cnt = 1;
								Buzz_ON=0;
								W501_LOW_Counter_RPAS_500MS = W501_LOW_DID_RPAS_SELF_CHECK - 10;
							}
						}
						else
						{
							if(Sound_Bit == 0x8000)
							{
								Sound_Bit = 0x0;
							}

							W501_LOW_RPAS_SELF_CHECK_TONE=0;
							W501LOW_selfcheck_cnt = 0;
						}

					}
					else
					{
						if(Sound_Bit == 0x8000)
						{
							Sound_Bit = 0x0;
						}

						W501_LOW_RPAS_SELF_CHECK_TONE=0;
						W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
						W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
						W501LOW_selfcheck_cnt = 0;
					}

				}
				else
				{
					if(Sound_Bit == 0x8000)
					{
						Sound_Bit = 0x0;
					}
					W501_LOW_RPAS_SELF_CHECK_TONE=0;
					W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
					W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
				}

			}
			else
			{
				if(Sound_Bit == 0x8000)
				{
					Sound_Bit = 0x0;
				}

				W501_LOW_RPAS_SELF_CHECK_TONE=0;
				W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
				W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
			}
		}
		else
		{
			W501_LOW_RPAS_SELF_CHECK_TONE=0;
			W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
			W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
		}

			/******RPAS STATUS CHIME FUNCTIONAL LOGIC*******/
				/********RPAS_SECTOR_ZONE_CALLIBRATION----0X01*********/

				if(Diag_RW_RPAS_Sector_Zone_Calibration == 1)
				{
					if(Diag_RW_RPASS_Chime_Enable==1)
					{
						if(RPAS_Precondition_Active==1)
						{
							if(MBFM_PAS1_50_message_timeout_flag == 0)

							{
								five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
								five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
								five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
								five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
								five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

								if(five_bit_data.byte==0)
								{

									four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
									four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
									four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
									four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

									four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
									four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
									four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
									four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;

									/*chime level_1*/
									if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
									{
										if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
										{
											Sound_Bit = 0x10000;
											W501_LOW_RPAS_LEVEL_1_BUZ=1;
											W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

										}
										else
										{
											if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
											{
												SoundPCM_RPAS_buzz_LV1_STOP();
											}
											W501_LOW_RPAS_LEVEL_1_BUZ=0;
											W501_LOW_RPAS_LEVEL1_PRECON = 0;
											if(Sound_Bit == 0x10000)
											{
												Sound_Bit = 0x0;
											}
										}

									}
									else
									{
										if(Sound_Bit == 0x10000)
										{
											Sound_Bit=0x0;
											W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
										}
										if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
										{
											SoundPCM_RPAS_buzz_LV1_STOP();
										}
										W501_LOW_RPAS_LEVEL_1_BUZ=0;
										W501_LOW_RPAS_LEVEL1_PRECON = 0;

									}

									/*******LEVEL2**********/
									if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
									{
										if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)&&((reverse_played == 1)||(STS_Engine!=2)))
										{
											Sound_Bit = 0x20000;
											W501_LOW_RPAS_LEVEL_2_BUZ=1;
											W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
											W501_LOW_priority_lv2=1;
										}
										else
										{
											if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
											{
												SoundPCM_RPAS_buzz_LV2_STOP();
											}
											W501_LOW_RPAS_LEVEL_2_BUZ=0;
											W501_LOW_RPAS_COUNTER2 = 0;
											if(Sound_Bit == 0x20000)
											{
												Sound_Bit = 0x0;
											}
										}
									}
									else
									{
										if(Sound_Bit == 0x20000)
										{
											Sound_Bit=0x0;
											W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
										}
										if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
										{
											SoundPCM_RPAS_buzz_LV2_STOP();
										}
										W501_LOW_RPAS_LEVEL_2_BUZ=0;
										W501_LOW_RPAS_COUNTER2 = 0;
										//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
										W501_LOW_priority_lv2=0;
									}

									/*LEVEL3*/
									if((four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)&&W501_LOW_priority_lv2==0)
									{

										if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
										{
											Sound_Bit = 0x30000;
											W501_LOW_RPAS_LEVEL_3_BUZ=1;
											W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
											W501_LOW_priority_lv3=1;
										}
										else
										{
											if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
											{
												SoundPCM_RPAS_buzz_LV3_STOP();
											}
											W501_LOW_RPAS_LEVEL_3_BUZ=0;
											W501_LOW_RPAS_COUNTER3 = 0;
											if(Sound_Bit == 0x30000)
											{
												Sound_Bit = 0x0;
											}
										}

									}
									else
									{
										if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
										{
											SoundPCM_RPAS_buzz_LV3_STOP();
										}
										W501_LOW_RPAS_LEVEL_3_BUZ=0;
										W501_LOW_RPAS_COUNTER3 = 0;

										if(Sound_Bit == 0x30000)
										{
											Sound_Bit=0x0;
											W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
										}

										W501_LOW_priority_lv3=0;
									}
								}
								else
								{
									if(Sound_Bit == 0x10000)
									{
										W501_LOW_RPAS_LEVEL1_PRECON = 0;
										Sound_Bit = 0x0;
										SoundPCM_RPAS_buzz_LV1_STOP();

										W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
									}
									if(Sound_Bit == 0x20000)
									{
										W501_LOW_RPAS_COUNTER2 = 0;
										Sound_Bit = 0x0;
										SoundPCM_RPAS_buzz_LV2_STOP();

										W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
									}
									if(Sound_Bit == 0x30000)
									{
										W501_LOW_RPAS_COUNTER3 = 0;
										Sound_Bit = 0x0;
										SoundPCM_RPAS_buzz_LV3_STOP();
										W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
									}


									if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_1_BUZ = 0;
									}
									else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_2_BUZ = 0;
									}
									else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_3_BUZ = 0;
									}
									else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_4_BUZ = 0;
									}
								}
							}
							else
							{
								if(Sound_Bit == 0x10000)
								{
									W501_LOW_RPAS_LEVEL1_PRECON = 0;
									Sound_Bit = 0x0;
									SoundPCM_RPAS_buzz_LV1_STOP();

									W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
								}
								if(Sound_Bit == 0x20000)
								{
									W501_LOW_RPAS_COUNTER2 = 0;
									Sound_Bit = 0x0;
									SoundPCM_RPAS_buzz_LV2_STOP();

									W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								}
								if(Sound_Bit == 0x30000)
								{
									W501_LOW_RPAS_COUNTER3 = 0;
									Sound_Bit = 0x0;
									SoundPCM_RPAS_buzz_LV3_STOP();
									W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								}

								W501_LOW_RPAS_LEVEL_1_BUZ=0;
								W501_LOW_RPAS_LEVEL_2_BUZ=0;
								W501_LOW_RPAS_LEVEL_3_BUZ=0;
							}
						}
						else
						{

							if(Sound_Bit == 0x10000)
							{
								W501_LOW_RPAS_LEVEL1_PRECON = 0;
								Sound_Bit = 0x0;
								SoundPCM_RPAS_buzz_LV1_STOP();

								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}
							if(Sound_Bit == 0x20000)
							{
								W501_LOW_RPAS_COUNTER2 = 0;
								Sound_Bit = 0x0;
								SoundPCM_RPAS_buzz_LV2_STOP();

								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}
							if(Sound_Bit == 0x30000)
							{
								W501_LOW_RPAS_COUNTER3 = 0;
								Sound_Bit = 0x0;
								SoundPCM_RPAS_buzz_LV3_STOP();
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
						}
					}
					else
					{
						W501_LOW_RPAS_LEVEL_1_BUZ=0;
						W501_LOW_RPAS_LEVEL_2_BUZ=0;
						W501_LOW_RPAS_LEVEL_3_BUZ=0;
					}
				}

	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X02*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 2)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
						four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
						four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
						four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;

						/*chime level_1*/
						if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_LOW_RPAS_LEVEL_1_BUZ=1;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV1_STOP();
								}
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
								W501_LOW_RPAS_LEVEL1_PRECON = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;
									W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
								}
							}

						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV1_STOP();
							}
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL1_PRECON = 0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit = 0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}
						}

						/*******LEVEL2**********/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_LOW_RPAS_LEVEL_2_BUZ=1;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								W501_LOW_priority_lv2=1;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV2_STOP();
								}
								W501_LOW_RPAS_LEVEL_2_BUZ=0;
								W501_LOW_RPAS_COUNTER2 = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
									W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								}
							}
						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV2_STOP();
							}
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_COUNTER2 = 0;

							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}
							//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
							W501_LOW_priority_lv2=0;
						}

						/*LEVEL3*/
						if((four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)&&(W501_LOW_priority_lv2==0))
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_LOW_RPAS_LEVEL_3_BUZ=1;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								W501_LOW_priority_lv3=1;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV3_STOP();
								}
								W501_LOW_RPAS_LEVEL_3_BUZ=0;
								W501_LOW_RPAS_COUNTER3 = 0;

								if(Sound_Bit == 0x30000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								}
							}
						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV3_STOP();
							}
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_COUNTER3 = 0;

							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_priority_lv3=0;
						}

						/*LEVEL4*/

						/*chime level_4*/
						if((four_bit_data_RLC.byte==4 || four_bit_data_RRC.byte==4)&&(W501_LOW_priority_lv2==0)&&(W501_LOW_priority_lv3==0))
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x40000;
								W501_LOW_RPAS_LEVEL_4_BUZ=1;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_4_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV4_STOP();
								}
								W501_LOW_RPAS_LEVEL_4_BUZ=0;
								W501_LOW_RPAS_COUNTER4 = 0;

								if(Sound_Bit == 0x40000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
								}
							}
						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_4_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV4_STOP();
							}
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
							W501_LOW_RPAS_COUNTER4 = 0;
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}
							//RPAS_LEVEL_4_TONE_ctr=0;
						}
					}
					else
					{
						W501_LOW_RPAS_LEVEL1_PRECON = 0;
						W501_LOW_RPAS_COUNTER2 = 0;
						W501_LOW_RPAS_COUNTER3 = 0;
						W501_LOW_RPAS_COUNTER4 = 0;
						W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
						W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
						W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
						W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV1_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV3_STOP();
						}
						if(Sound_Bit == 0x40000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV4_STOP();
						}

						if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_1_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_2_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_3_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_4_BUZ = 0;
						}
					}
				}
				else
				{
					W501_LOW_RPAS_LEVEL1_PRECON = 0;
					W501_LOW_RPAS_COUNTER2 = 0;
					W501_LOW_RPAS_COUNTER3 = 0;
					W501_LOW_RPAS_COUNTER4 = 0;

					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV1_STOP();
						W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV2_STOP();
						W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV3_STOP();
						W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV4_STOP();
						W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
					}
					W501_LOW_RPAS_LEVEL_1_BUZ=0;
					W501_LOW_RPAS_LEVEL_2_BUZ=0;
					W501_LOW_RPAS_LEVEL_3_BUZ=0;
					W501_LOW_RPAS_LEVEL_4_BUZ=0;
				}
			}
			else
			{
				W501_LOW_RPAS_LEVEL1_PRECON = 0;
				W501_LOW_RPAS_COUNTER2 = 0;
				W501_LOW_RPAS_COUNTER3 = 0;
				W501_LOW_RPAS_COUNTER4 = 0;

				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV1_STOP();
					W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV2_STOP();
					W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV3_STOP();
					W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV4_STOP();
					W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
				}

				W501_LOW_RPAS_LEVEL_1_BUZ=0;
				W501_LOW_RPAS_LEVEL_2_BUZ=0;
				W501_LOW_RPAS_LEVEL_3_BUZ=0;
				W501_LOW_RPAS_LEVEL_4_BUZ=0;
			}
		}
		else
		{
			W501_LOW_RPAS_LEVEL_1_BUZ=0;
			W501_LOW_RPAS_LEVEL_2_BUZ=0;
			W501_LOW_RPAS_LEVEL_3_BUZ=0;
			W501_LOW_RPAS_LEVEL_4_BUZ=0;
		}
	}

	/********/
	if(Diag_RW_RPAS_Sector_Zone_Calibration == 3)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)
				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						/*chime level_1*/
						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_LOW_RPAS_LEVEL_1_BUZ=1;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV1_STOP();
								}
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
								W501_LOW_RPAS_LEVEL1_PRECON = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
								}
							}
						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV1_STOP();
							}
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL1_PRECON = 0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}
						}

						/*******LEVEL2**********/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_LOW_RPAS_LEVEL_2_BUZ=1;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								W501_LOW_priority_lv2=1;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV2_STOP();
								}
								W501_LOW_RPAS_LEVEL_2_BUZ=0;
								W501_LOW_RPAS_COUNTER2 = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								}
							}
						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV2_STOP();
							}
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_COUNTER2 = 0;
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}
							//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
													W501_LOW_priority_lv2=0;
						}


						/*LEVEL3*/
						if((four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)&&(W501_LOW_priority_lv2==0))
						{

							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_LOW_RPAS_LEVEL_3_BUZ=1;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								W501_LOW_priority_lv3=1;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV3_STOP();
								}
								W501_LOW_RPAS_LEVEL_3_BUZ=0;
								W501_LOW_RPAS_COUNTER3 = 0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								}
							}


						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV3_STOP();
							}
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_COUNTER3 = 0;
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_priority_lv3=0;
						}
				}
					else
					{
						W501_LOW_RPAS_LEVEL1_PRECON = 0;
						W501_LOW_RPAS_COUNTER2 = 0;
						W501_LOW_RPAS_COUNTER3 = 0;
						W501_LOW_RPAS_COUNTER4 = 0;
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV1_STOP();
						}
						if(Sound_Bit == 0x20000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV2_STOP();
						}
						if(Sound_Bit == 0x30000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV3_STOP();
						}
						if(Sound_Bit == 0x40000)
						{
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV4_STOP();
						}
						if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_1_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_2_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_3_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_4_BUZ = 0;
						}
						W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
						W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
						W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
						W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
					}
				}
				else
				{
					W501_LOW_RPAS_LEVEL1_PRECON = 0;
					W501_LOW_RPAS_COUNTER2 = 0;
					W501_LOW_RPAS_COUNTER3 = 0;
					W501_LOW_RPAS_COUNTER4 = 0;
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV1_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV3_STOP();
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV4_STOP();
					}
					W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
					W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
					W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
					W501_LOW_RPAS_LEVEL_4_TONE_compl=0;

				}
		}
		else
		{
			W501_LOW_RPAS_LEVEL1_PRECON = 0;
			W501_LOW_RPAS_COUNTER2 = 0;
			W501_LOW_RPAS_COUNTER3 = 0;
			W501_LOW_RPAS_COUNTER4 = 0;
			if(Sound_Bit == 0x10000)
			{
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV1_STOP();
			}
			if(Sound_Bit == 0x20000)
			{
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV2_STOP();
			}
			if(Sound_Bit == 0x30000)
			{
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV3_STOP();
			}
			if(Sound_Bit == 0x40000)
			{
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV4_STOP();
			}
			W501_LOW_RPAS_LEVEL_1_BUZ = 0;
			W501_LOW_RPAS_LEVEL_2_BUZ = 0;
			W501_LOW_RPAS_LEVEL_3_BUZ = 0;
			W501_LOW_RPAS_LEVEL_4_BUZ = 0;
			W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
			W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
			W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
			W501_LOW_RPAS_LEVEL_4_TONE_compl=0;

			}
		}
		else
		{
			W501_LOW_RPAS_LEVEL_1_BUZ=0;
			W501_LOW_RPAS_LEVEL_2_BUZ=0;
			W501_LOW_RPAS_LEVEL_3_BUZ=0;
		}
	}
/***********************/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 4)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						/*chime level_1*/
						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x10000;
								W501_LOW_RPAS_LEVEL_1_BUZ=1;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV1_STOP();
								}
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
								W501_LOW_RPAS_LEVEL1_PRECON = 0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
								}
							}

						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_1_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV1_STOP();
							}
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL1_PRECON = 0;
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}
						}
						/*******LEVEL2**********/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x20000;
								W501_LOW_RPAS_LEVEL_2_BUZ=1;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								W501_LOW_priority_lv2=1;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV2_STOP();
								}
								W501_LOW_RPAS_LEVEL_2_BUZ=0;
								W501_LOW_RPAS_COUNTER2 = 0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								}
							}
						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_2_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV2_STOP();
							}
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_COUNTER2 = 0;
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}

							//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
							W501_LOW_priority_lv2=0;
						}

						/*LEVEL3*/
						if((four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)&&(W501_LOW_priority_lv2==0))
						{

							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x30000;
								W501_LOW_RPAS_LEVEL_3_BUZ=1;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								W501_LOW_priority_lv3=1;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV3_STOP();
								}
								W501_LOW_RPAS_LEVEL_3_BUZ=0;
								W501_LOW_RPAS_COUNTER3 = 0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								}
							}

						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_3_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV3_STOP();
							}
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_COUNTER3 = 0;
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_priority_lv3=0;
						}


						/*chime level_4*/
						if((four_bit_data_RLC.byte==4 || four_bit_data_RL.byte==4 || four_bit_data_RR.byte==4)&&(W501_LOW_priority_lv2==0)&&(W501_LOW_priority_lv3==0))
						{
							if(((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000))&&((reverse_played == 1)||(STS_Engine!=2)))
							{
								Sound_Bit = 0x40000;
								W501_LOW_RPAS_LEVEL_4_BUZ=1;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}
							else
							{
								if(W501_LOW_RPAS_LEVEL_4_BUZ==1)
								{
									SoundPCM_RPAS_buzz_LV4_STOP();
								}
								W501_LOW_RPAS_LEVEL_4_BUZ=0;
								W501_LOW_RPAS_COUNTER4 = 0;
								if(Sound_Bit == 0x40000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
								}
							}

						}
						else
						{
							if(W501_LOW_RPAS_LEVEL_4_BUZ==1)
							{
								SoundPCM_RPAS_buzz_LV4_STOP();
							}
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
							W501_LOW_RPAS_COUNTER4 = 0;
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}
							//RPAS_LEVEL_4_TONE_ctr=0;
						}
					}
					else
					{
						if(Sound_Bit == 0x10000)
						{
							Sound_Bit=0x0;
							W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
						}

						if(Sound_Bit == 0x20000)
						{
							Sound_Bit=0x0;
							W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
						}

						if(Sound_Bit == 0x30000)
						{
							Sound_Bit=0x0;
							W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
						}
						if(Sound_Bit == 0x40000)
						{
							Sound_Bit=0x0;
							W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
						}
						if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_1_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_2_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_3_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_4_BUZ = 0;
						}
					}
				}
				else
				{
					W501_LOW_RPAS_LEVEL1_PRECON = 0;
					W501_LOW_RPAS_COUNTER2 = 0;
					W501_LOW_RPAS_COUNTER3 = 0;
					W501_LOW_RPAS_COUNTER4 = 0;
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV1_STOP();
					}
					if(Sound_Bit == 0x20000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV2_STOP();
					}
					if(Sound_Bit == 0x30000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV3_STOP();
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit = 0x0;
						SoundPCM_RPAS_buzz_LV4_STOP();
					}
					W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
					W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
					W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
					W501_LOW_RPAS_LEVEL_4_TONE_compl=0;

					W501_LOW_RPAS_LEVEL_1_BUZ=0;
					W501_LOW_RPAS_LEVEL_2_BUZ=0;
					W501_LOW_RPAS_LEVEL_3_BUZ=0;
					W501_LOW_RPAS_LEVEL_4_BUZ=0;
				}
			}
			else
			{
				W501_LOW_RPAS_LEVEL1_PRECON = 0;
				W501_LOW_RPAS_COUNTER2 = 0;
				W501_LOW_RPAS_COUNTER3 = 0;
				W501_LOW_RPAS_COUNTER4 = 0;
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV1_STOP();
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV2_STOP();
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV3_STOP();
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
					SoundPCM_RPAS_buzz_LV4_STOP();
				}
				W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
				W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
				W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
				W501_LOW_RPAS_LEVEL_4_TONE_compl=0;

				W501_LOW_RPAS_LEVEL_1_BUZ=0;
				W501_LOW_RPAS_LEVEL_2_BUZ=0;
				W501_LOW_RPAS_LEVEL_3_BUZ=0;
				W501_LOW_RPAS_LEVEL_4_BUZ=0;
			}
		}
		else
		{
			W501_LOW_RPAS_LEVEL_1_BUZ=0;
			W501_LOW_RPAS_LEVEL_2_BUZ=0;
			W501_LOW_RPAS_LEVEL_3_BUZ=0;
			W501_LOW_RPAS_LEVEL_4_BUZ=0;
		}
	}
#endif
#endif




#if 0
	/* LOW FUEL TT BASED ON ADC VALUE */
	if(fuel__temp_udc_enable==1)
	{
		if(fuel_fault_TT_blink == 1)
		{
			LOW_FUEL_TT=telltale_blink_flag_patt10;
			CAN_IC1_100[3].bits.Rx_8=1;
			fuel_alert_on_flag=1;
		}
		else if((New_fuel_read_ad1 >= Fuel_level_TT[1])&& (New_fuel_read_ad1 <= Fuel_level_TT[3]))
		{
			LOW_FUEL_TT=1;
			CAN_IC1_100[3].bits.Rx_8=1;
			fuel_alert_on_flag=1;
			fuel_tt_on_flag=1;
			fuel_tt_blink_flag=0;
		}
		else if((fuel_tt_on_flag==1)&&(New_fuel_read_ad1 > Fuel_level_TT[0])&&(New_fuel_read_ad1 < Fuel_level_TT[1]))
		{
			LOW_FUEL_TT=1;
			CAN_IC1_100[3].bits.Rx_8=0;
			fuel_alert_on_flag=1;

			fuel_tt_blink_flag=0;
		}
		else if((New_fuel_read_ad1>=Fuel_level_TT[3]) &&(New_fuel_read_ad1 <= LOW_FUEL_TELLTALE_OPEN))
		{
			LOW_FUEL_TT=telltale_blink_bool7;
			CAN_IC1_100[3].bits.Rx_8=1;
			fuel_alert_on_flag=1;
			fuel_tt_blink_flag=1;

		}
		else if((fuel_tt_blink_flag==1)&&(fuel_tt_on_flag==1)&&(New_fuel_read_ad1>=Fuel_level_TT[2])&&(New_fuel_read_ad1 < Fuel_level_TT[3]))
		{
			LOW_FUEL_TT=telltale_blink_bool7;
			CAN_IC1_100[3].bits.Rx_8=1;
			fuel_alert_on_flag=1;
			//fuel_tt_on_flag=0;
		}
		else if(New_fuel_read_ad1 <= Fuel_level_TT[0])
		{
			LOW_FUEL_TT=0;
			CAN_IC1_100[3].bits.Rx_8=0;
			fuel_alert_on_flag=0;
			fuel_tt_blink_flag=0;
			fuel_tt_on_flag=0;
		}
		else
		{
			LOW_FUEL_TT=0;
			CAN_IC1_100[3].bits.Rx_8=0;
			fuel_alert_on_flag=0;
			fuel_tt_blink_flag=0;
			fuel_tt_on_flag=0;
		}
		DID_TT_bit_data_set3.bits.Rx_7=LOW_FUEL_TT;
	}

#endif

if(fuel_enable_tt==1)
{
	/*LOW FUEL VALUE BASED ON BAR*/
	if(fuel_fault_TT_blink == 1)
	{
//		LOW_FUEL_TT=telltale_blink_flag_patt10;
		CAN_IC1_100[3].bits.Rx_8=1;
		fuel_alert_on_flag=1;
	}
	else if(fuel_widget == 1 )
	{
		LOW_FUEL_TT=1;
		CAN_IC1_100[3].bits.Rx_8=1;
		fuel_alert_on_flag=1;
		fuel_tt_on_flag=1;
		fuel_tt_blink_flag=0;

	}
	else if((fuel_tt_on_flag==1)&&(fuel_widget == 1 ))
	{
		LOW_FUEL_TT=1;
		CAN_IC1_100[3].bits.Rx_8=1;
		fuel_alert_on_flag=1;
		fuel_tt_blink_flag=0;
	}
	else if(fuel_widget==0 && fuel_fault_TT_blink == 0)
	{
		LOW_FUEL_TT=telltale_blink_bool7;
		CAN_IC1_100[3].bits.Rx_8=1;
		fuel_alert_on_flag=1;
		fuel_tt_blink_flag=1;
	}
	else if((fuel_tt_blink_flag==1)&&(fuel_widget==0))
	{
		LOW_FUEL_TT=telltale_blink_bool7;
		CAN_IC1_100[3].bits.Rx_8=1;
		fuel_alert_on_flag=1;
	}
	else if(fuel_widget >= 2)
	{
		LOW_FUEL_TT=0;
		CAN_IC1_100[3].bits.Rx_8=0;
		fuel_alert_on_flag=0;
		fuel_tt_blink_flag=0;
		fuel_tt_on_flag=0;
	}
	else
	{
		LOW_FUEL_TT=0;
		CAN_IC1_100[3].bits.Rx_8=0;
		fuel_alert_on_flag=0;
		fuel_tt_blink_flag=0;
		fuel_tt_on_flag=0;
	}
	DID_TT_bit_data_set3.bits.Rx_7=LOW_FUEL_TT;
}

#ifdef GRAYSCALE_TFT
	if(Diag_RW_LowFuel_Chime_Enable==1)
	{
		if((CAN_IC1_100[3].bits.Rx_8 == 1) && (Enginerun_chm == 1))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x20 || Sound_Bit == 0x200) && Buzz_Low_fuel_compl == 0)
			{
				Buzz_Low_fuel=1;
				Sound_Bit = 0x20;
			}
			else
			{
				Buzz_Low_fuel=0;
			}
		}
		else if((Buzz_Low_fuel_compl == 1) )
		{
			if(Sound_Bit == 0x20)
			{
				Sound_Bit = 0x0;
			}
			Buzz_Low_fuel=0;
			Buzz_Low_fuel_cntr=0;
			Buzz_Low_fuel_compl=0;
		}
	}
#endif

#ifdef COLOR_TFT
	if(Diag_RW_LowFuel_Chime_Enable==1)
	{
		if((CAN_IC1_100[3].bits.Rx_8 == 1) && (Enginerun_chm == 1)&&(Low_fuel_ongoing == 0)&&(chime_start1 == 0)&&(cycle_lowfuel_completed == 0))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x20 || Sound_Bit == 0x200)  && (cycle_lowfuel_completed == 0))
			{
				Sound_Bit = 0x20;							/*0010 0000 - Low Fuel sound bit*/
				//		SoundPCM_message();
				Low_fuel_ongoing = 1;
				chime_start1 = 1;
				lowfuel_missed = 0;
			}
			else
			{
				Low_fuel_ongoing = 0;
				lowfuel_missed = 0;
			}
		}
		else if(CAN_IC1_100[3].bits.Rx_8 == 0)
		{
			if(Sound_Bit == 0x20)
			{
				Sound_Bit = 0x0;							/**Sound bit CLEAR**/
			}
			Low_fuel_ongoing = 0;
			cycle_lowfuel_completed = 0;
			lowfuel_missed = 1;
		}
	}
	/*	else if(chime_start1 == 0)
	{
		Low_fuel_ongoing = 0;
	}*/
#endif

	/*battery_charge_tt*/

	if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1 && FEATURE_CODE_BATTERY_TT_TYPE==0)
	{
		if(EMS12_200_message_timeout_flag  == CLEAR)
		{
			two_bit_data.bits.Rx_1 = CAN_EMS12_200[3].bits.Rx_4;  /* configure according to  */
			two_bit_data.bits.Rx_2 = CAN_EMS12_200[3].bits.Rx_5;

			if(two_bit_data.byte == 1 ||two_bit_data.byte == 3 )
			{
				Batt_chrg_TT = 0xfff;
			}
			else if(two_bit_data.byte == 0 ||two_bit_data.byte == 2)
			{
				Batt_chrg_TT = 0x000;
			}
		}
		else
		{
			Batt_chrg_TT = 0xfff;
		}
	}

	else if(FEATURE_CODE_BATTERY_TT_TYPE==1)
	{
		if(BATTERY_CHARGE_CPU == 0)
		{
			Batt_chrg_TT = 0xfff;
		}
		else
		{
			Batt_chrg_TT = 0x000;
		}
	}
	else
	{
		Batt_chrg_TT = 0x000;
	}

	if(Batt_chrg_TT == 0xfff)
	{
		DID_TT_bit_data_set3.bits.Rx_6=1;
	}
	else
	{
		DID_TT_bit_data_set3.bits.Rx_6=0;
	}

	//TT on EMS 4 20//

	if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
	{
		if(EMS4_20_message_timeout_flag == CLEAR)
		{
			two_bit_data.bits.Rx_1 = CAN_EMS4_20[5].bits.Rx_4;
			two_bit_data.bits.Rx_2 = CAN_EMS4_20[5].bits.Rx_5;

			/* Check Engine Oil */
			if(two_bit_data.byte == 2 ||two_bit_data.byte == 3)
			{
				Check_Engine_TT = 0xfff;
				Check_Engine_flag = 1;
				DID_TT_bit_data_set1.bits.Rx_1=1;

				/*Check engine telltale feedback*/
				if(read_SID[548]==1 || read_SID[452]==1)
				{
					CAN_IC1_100[5].bits.Rx_5=1;    //FEEDBK_CHECK_ENGINE
					if(read_SID[548]==1)
					{
						Check_engine_TT_LOD=1;
					}
					else
					{
						Check_engine_TT_LOD=0;
					}
					if(read_SID[452]==1)
					{
						Check_engine_TT_LSD=1;
					}
					else
					{
						Check_engine_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC1_100[5].bits.Rx_5=0;
					Check_engine_TT_LOD=0;
					Check_engine_TT_LSD=0;
				}

			}
			else if(two_bit_data.byte == 0)
			{
				Check_Engine_TT = 0x000;
				Check_Engine_flag = 0;
				DID_TT_bit_data_set1.bits.Rx_1=0;
			}
			else if(two_bit_data.byte == 1)
			{
				Check_Engine_TT = telltale_blink_flag_patt3;
				Check_Engine_flag = 0;
			}
			else
			{
				Check_Engine_TT = 0x000;
				Check_Engine_flag = 0;
				CAN_IC1_100[5].bits.Rx_5=0;
				Check_engine_TT_LOD=0;
				Check_engine_TT_LSD=0;
				DID_TT_bit_data_set1.bits.Rx_1=0;
			}



			/* MIL TT */
			two_bit_data.bits.Rx_1 = CAN_EMS4_20[5].bits.Rx_6;
			two_bit_data.bits.Rx_2 = CAN_EMS4_20[5].bits.Rx_7;

			if(two_bit_data.byte == 2 ||two_bit_data.byte == 3)
			{
				MIL_TT = 0xfff;
				DID_TT_bit_data_set2.bits.Rx_8=1;
				/*feedback_msg*/
				if(read_SID[248]==1 ||read_SID[152]==1)
				{
					CAN_IC1_100[1].bits.Rx_8=1;    //FEEDBK_MIL

					if(read_SID[248]==1)
					{
						MIL_TT_LOD=1;
					}
					else
					{
						MIL_TT_LOD=0;
					}
					if(read_SID[152]==1)
					{
						MIL_TT_LSD=1;
					}
					else
					{
						MIL_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC1_100[1].bits.Rx_8=0;    //FEEDBK_MIL
					MIL_TT_LOD=0;
					MIL_TT_LSD=0;
				}
			}
			else if(two_bit_data.byte == 0)
			{
				MIL_TT = 0x000;
				DID_TT_bit_data_set2.bits.Rx_8=0;
			}
			else if(two_bit_data.byte == 1)
			{
				MIL_TT = telltale_blink_flag_patt3;
			}
			else
			{
				MIL_TT = 0x000;
				CAN_IC1_100[1].bits.Rx_8=0;    //FEEDBK_MIL
				MIL_TT_LOD=0;
				MIL_TT_LSD=0;
				DID_TT_bit_data_set2.bits.Rx_8=0;
			}

			/* Engine Oil pressure Low */
			if(CAN_EMS4_20[5].bits.Rx_2== 1)
			{
				Low_Oil_press_TT = 0xfff;
				DID_TT_bit_data_set1.bits.Rx_3=1;
			}
			else if(CAN_EMS4_20[5].bits.Rx_2== 0)
			{
				Low_Oil_press_TT = 0x000;
				DID_TT_bit_data_set1.bits.Rx_3=0;
			}
			else
			{
				Low_Oil_press_TT = 0x000;
			}
			//Engine oil pressure chime

			if((Low_Oil_press_TT == 0xFFF))
			{
				LOP_7Sec_cntr_start=1;
			}
			else
			{
				LOP_7Sec_cntr_start=0;
			}



#ifdef GRAYSCALE_TFT
			if(Diag_RW_LowOilPressure_Chime_Enable==1)
			{
				if((Low_Oil_press_TT == 0xFFF) && (LOP_7Sec_cntr_done == 1))
				{
					if((Sound_Bit == 0x0 || Sound_Bit == 0x2 || Sound_Bit == 0x200) && Buzz_Low_Eng_oilPre_compl == 0)
					{
						Sound_Bit = 0x2;
						Buzz_Low_Eng_oilPre = 1;
					}
					else
					{
						Buzz_Low_Eng_oilPre = 0;
					}
				}
				else if((Buzz_Low_Eng_oilPre_compl == 1))
				{
					if(Sound_Bit == 0x2)
					{
						Sound_Bit = 0x0;
					}
					Buzz_Low_Eng_oilPre=0;
					Buzz_Low_Eng_oilPre_compl=0;
					Buzz_Low_Eng_oilPre_cntr=0;
				}
			}
#endif
			//Engine oil pressure chime
#ifdef COLOR_TFT
			if((Diag_RW_LowOilPressure_Chime_Enable==1)&&(EMS1_10_message_timeout_flag == CLEAR))
			{
				if((Low_Oil_press_TT == 0xFFF) && (LOP_7Sec_cntr_done == 1)&&(Low_engine_oil_pressure == 0)/*&&(chime_start1 == 0)*/&&(cycle_Lowengineoil_completed == 0))
				{
					if((Sound_Bit == 0x0 || Sound_Bit == 0x02 || Sound_Bit == 0x200)  && (cycle_Lowengineoil_completed == 0))
					{
						Sound_Bit = 0x02;							/*0000 0010 - Engine oil pressure sound bit*/
						//SoundPCM_RPAS_St3();
						chime_start1 = 1;
						Low_engine_oil_pressure = 1;
						Lowengineoil_missed = 0;
					}
					else
					{
						Low_engine_oil_pressure = 0;
						Lowengineoil_missed = 0;
					}
				}
				else if(Low_Oil_press_TT != 0xFFF)
				{
					if(Sound_Bit == 0x2)
					{
						Sound_Bit = 0x0;							/**Sound bit CLEAR**/
					}
					Low_engine_oil_pressure = 0;
					cycle_Lowengineoil_completed = 0;
					Lowengineoil_missed = 1;
				}
			}
			else if(EMS1_10_message_timeout_flag != CLEAR)
			{
				Low_engine_oil_pressure = 0;
				LOP_7Sec_cntr_start = 0;
				cycle_Lowengineoil_completed = 0;
			}
			/*else if(chime_start1 == 0)
		{
			Low_engine_oil_pressure = 0;
		}*/
#endif

			/* DPF TT */
			if(FEATURE_CODE_DPF_TT_CONFIG == 1)
			{
//				dpf_check = 1;		//DPF_priority_flag

				two_bit_data.bits.Rx_1 = CAN_EMS4_20[6].bits.Rx_4;
				two_bit_data.bits.Rx_2 = CAN_EMS4_20[6].bits.Rx_5;

				if(two_bit_data.byte == 0 ||two_bit_data.byte == 3)//for DPF_REGEN=3/0 is reserved/Lamp off
				{
					DPF_TT = 0x000;
					dpf_alert_flag=0;
					DID_TT_bit_data_set6.bits.Rx_1=0;
				}
				else if(two_bit_data.byte == 1)//For DPF_REGEN=1 is Lamp on
				{
					DPF_TT = 0xfff;

					dpf_alert_flag=1;
					DID_TT_bit_data_set6.bits.Rx_1=1;
				}
				else if(two_bit_data.byte == 2)//For DPF_REGEN=2 is Lamp blink
				{
					DPF_TT = telltale_blink_flag_patt3;
					dpf_alert_flag=0;
					DID_TT_bit_data_set6.bits.Rx_1=1;
				}
				else
				{
					DPF_TT = 0x000;
					DID_TT_bit_data_set6.bits.Rx_1=0;
				}
			}
			else
			{
//				dpf_check = 1;		//DPF_priority_flag
				DPF_TT = 0x000;
				DID_TT_bit_data_set6.bits.Rx_1=0;
			}

			/* ECO TT */
			if(FEATURE_CODE_ECO_TT_CONFIG == 1)
			{
				three_bit_data.bits.Rx_1 = CAN_EMS4_20[6].bits.Rx_6;
				three_bit_data.bits.Rx_2 = CAN_EMS4_20[6].bits.Rx_7;
				three_bit_data.bits.Rx_3 = CAN_EMS4_20[6].bits.Rx_8;

				if(three_bit_data.byte == 1)
				{
					ECO_TT = 1;
					DID_TT_bit_data_set4.bits.Rx_3=1;
				}
				else if(three_bit_data.byte != 1)
				{
					ECO_TT = 0;
					DID_TT_bit_data_set4.bits.Rx_3=0;
				}
				else
				{
					ECO_TT = 0;
				}
			}
			else
			{
				ECO_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_3=0;
			}
		}
		else
		{
			Check_Engine_TT = 0;
			Check_Engine_flag = 0;
			DID_TT_bit_data_set1.bits.Rx_1=0;
			MIL_TT = 0;

			DPF_TT=0;
			ECO_TT = 0;
			CAN_IC1_100[1].bits.Rx_8=0;    //FEEDBK_MIL
			MIL_TT_LOD=0;
			MIL_TT_LSD=0;
			CAN_IC1_100[5].bits.Rx_5=0;
			Check_engine_TT_LOD=0;
			Check_engine_TT_LSD=0;
			DID_TT_bit_data_set4.bits.Rx_3=0;
			DID_TT_bit_data_set1.bits.Rx_3=1;
			DID_TT_bit_data_set2.bits.Rx_8=0;
			Buzz_Low_Eng_oilPre_compl=0;
			dpf_alert_flag=0;

/******************************low fuel can missing condition*******************************/

			Low_fuel_ongoing = 0;
			cycle_lowfuel_completed = 0;
			chime_start1=0;
			Low_Oil_press_TT = 0xfff;
			LOP_7Sec_cntr_start=0;
			Buzz_Low_Eng_oilPre_compl = 0;/*Low end*/
			cycle_Lowengineoil_completed=0;/*High_End invictus added to fix the bug*/
			Low_engine_oil_pressure	=0;
			cycleLowengineoil_cntr = 0;
/*******************************************************************************************/
		}


	}

	else
	{
		Low_Oil_press_TT = 0;
		Check_Engine_TT = 0;
		MIL_TT = 0;
		DPF_TT=0;
		ECO_TT = 0;
		CAN_IC1_100[5].bits.Rx_5=0;    //FEEDBK_SYSLAMP
		CAN_IC1_100[1].bits.Rx_8=0;    //FEEDBK_MIL

	}

	/*Air_bag_TT*/
	if(VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM==1)
	{
		if(SRS1_20_message_timeout_flag  == CLEAR)
		{
			two_bit_data.bits.Rx_1 = CAN_SRS1_20[0].bits.Rx_1;  /* make chang later */
			two_bit_data.bits.Rx_2 = CAN_SRS1_20[0].bits.Rx_2;
			int count = 0;

			CAN_SRS1_20_ext[0].byte = CAN_SRS1_20[0].byte;

			while(CAN_SRS1_20_ext[0].byte)
			{
				if( (CAN_SRS1_20_ext[0].byte & 0x1) == 1)
					count++;
				CAN_SRS1_20_ext[0].byte = CAN_SRS1_20_ext[0].byte >> 1;
			}

			if(count == 1 ||count == 3||count == 5 ||count == 7)
			{
				AIR_BAG_TT = 0;
			}
			else
			{
				if(two_bit_data.byte == 1 ||two_bit_data.byte == 2)
				{
					AIR_BAG_TT =0xfff;

					/*Airbag telltale feedback*/
					if(read_SID[533]==1 ||read_SID[437]==1)
					{
						CAN_IC1_100[5].bits.Rx_2=1;    //FEEDBK_SRS_LAMP
						if(read_SID[533]==1)
						{
							AIR_BAG_TT_LOD=1;
						}
						else
						{
							AIR_BAG_TT_LOD=0;
						}
						if(read_SID[437]==1)
						{
							AIR_BAG_TT_LSD=1;
						}
						else
						{
							AIR_BAG_TT_LSD=0;
						}
					}
					else
					{
						CAN_IC1_100[5].bits.Rx_2=0;    //FEEDBK_SRS_LAMP
						AIR_BAG_TT_LOD=0;
						AIR_BAG_TT_LSD=0;
					}
				}
				else if(two_bit_data.byte == 0)
				{
					AIR_BAG_TT = 0;
				}
				else if(two_bit_data.byte == 3)
				{
					AIR_BAG_TT = telltale_blink_flag_patt3;
				}
			}

			if(AIR_BAG_TT == 0xfff)
			{
				DID_TT_bit_data_set2.bits.Rx_1=1;
			}
			else
			{
				DID_TT_bit_data_set2.bits.Rx_1=0;
			}

			two_bit_data.bits.Rx_1 = CAN_SRS1_20[3].bits.Rx_1;  /* seat_belt_tt*/
			two_bit_data.bits.Rx_2 = CAN_SRS1_20[3].bits.Rx_2;


			if(two_bit_data.byte == 0)
			{
				BuckledFaulty = 0;
				Driver_Seat_belt_flag=1;
				//Driver_Seat_belt_alert_flag_ON=1;
			}
			else if(two_bit_data.byte == 2)
			{
				BuckledFaulty = 1;
				Driver_Seat_belt_flag=1;
				//Driver_Seat_belt_alert_flag_ON=0;
			}
			else if(two_bit_data.byte == 1 || two_bit_data.byte == 3)
			{
				BuckledFaulty = 0;
				Driver_Seat_belt_flag=0;
				//Driver_Seat_belt_alert_flag_ON=0;
			}

			two_bit_data.bits.Rx_1 = CAN_SRS1_20[3].bits.Rx_3;  /* seat_belt_tt*/	/**Passenger seat belt**/
			two_bit_data.bits.Rx_2 = CAN_SRS1_20[3].bits.Rx_4;

			if((two_bit_data.byte == 0)&& FEATURE_CODE_CODRIVER_SBR_TT_CONFIG==1)
			{
				Psngr_BuckledFaulty = 0;
				Passenger_Seat_belt_flag=1;

			}
			else if((two_bit_data.byte == 2)&& FEATURE_CODE_CODRIVER_SBR_TT_CONFIG==1)
			{
				Psngr_BuckledFaulty = 1;
				Passenger_Seat_belt_flag=1;

			}
			else if((two_bit_data.byte == 1 || two_bit_data.byte == 3)&& FEATURE_CODE_CODRIVER_SBR_TT_CONFIG==1)
			{
				Psngr_BuckledFaulty = 0;
				Passenger_Seat_belt_flag=0;

			}

			if(Driver_Seat_belt_flag==0 && Passenger_Seat_belt_flag==0)
			{
				SEAT_BELT_TT = 0;
				SBR_IGN_ON = 0;
			}
			else if(Driver_Seat_belt_flag==0 && Passenger_Seat_belt_flag==1)
			{
				SEAT_BELT_TT = 1;
			}
			else if(Driver_Seat_belt_flag==1 && Passenger_Seat_belt_flag==0)
			{
				SEAT_BELT_TT = 1;
			}
			else if(Driver_Seat_belt_flag==1 && Passenger_Seat_belt_flag==1)
			{
				SEAT_BELT_TT = 1;
			}
			DID_TT_bit_data_set1.bits.Rx_6=SEAT_BELT_TT;
			/*fOR cHIME on or off*/
			STS_Eng_data = CAN_EMS1_10[6].byte;
			STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);

			if((((CAN_SRS1_20[3].bits.Rx_1 == 0) && (CAN_SRS1_20[3].bits.Rx_2==0)) ||( (CAN_SRS1_20[3].bits.Rx_3 == 0) && (CAN_SRS1_20[3].bits.Rx_4==0)))
				&& (Reverse_gear_active == 0) && (STS_Eng_data == 2))
			{
				SBR_chime_condition_arrived = 1;
				if((rpm8000_arrived == 1)||(SBR_Speed_Activation==1)||(display_speedometer_speed > Diag_RW_Vehicle_Moving_final_speed)||(spd_2km_arrived == 1)||(ODO_arrived ==1))
				{
					Seat_Belt_Chime =1;
					SBR_chime_condition_arrived = 1;
					SBR_chime_on = 1;
					SBR_Speed_Activation = 1;
//					if((Buzz_SBR_lv2_cntr < 133)||(Buzz_SBR_lv2_cntr == 140))//added for to stop chime after 133 cycle or fast toggle
					if((Buzz_SBR_lv2_cntr < Diag_RW_Final_chime_duration)||(Buzz_SBR_lv2_cntr == Diag_RW_Final_chime_duration+5))//added for to stop chime after 133 cycle or fast toggle
					{
						Buzz_SBR_lv2_compl = 0;
					}
					spd_2km_arrived = 1;
				}
			}
			else
			{
				if(SBR_chime_on == 1 )//Turn off SBR buzzer instantly.
				{
					SBR_chime_on = 0;
					Buzz_SBR_lv2 = 1;
					Buzz_ON = 1;
					counter7 = 8;
					Buzz_SBR_lv2_cntr = Diag_RW_Final_chime_duration+5;//140
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					Buzz_SBR_lv2_compl = 0;
				}
				SBR_Speed_Activation = 0;
				Seat_Belt_Chime =0;
				SBR_chime_condition_arrived = 0;
				ODO_arrived=0;
				spd_2km_arrived=0;
				one_MIN_rpm_counter=0;
				one_MIN_spd_counter=0;
				//buzzer off

				Passenger_Seat_belt_alert_flag_ON=0;
//r				Sound_Bit = 0x0;
//r				Buzz_SBR_lv2=0;
				//buzzer off

				if(Sound_Bit == 0x80)
				{
					rpm8000_arrived=0;
				}
				/*Clear all SBR chime condition*/
				one_MIN_rpm_counter = 0;
				rpm8000_arrived = 0;//
				one_MIN_spd_counter = 0;
				spd_2km_arrived = 0;//

			}

			if((Tacho_invalid_default == 1||Speed_Invalid_Default == 1||((Less_tn_thrhld_rpm == 1)&&((spd_2km_arrived == 0)/*&&(display_speedometer_speed < Diag_RW_Vehicle_Moving_final_speed)*/)))||(EMS1_10_message_timeout_flag != 0 || ((ESC2_10_message_timeout_flag != 0)&&(FEATURE_CODE_SPEED_INPUT ==0)) || ((ESC12_10_message_timeout_flag != 0)&&(FEATURE_CODE_SPEED_INPUT ==1))))
			/*((Speed or RPM invalid/Default or(RPM<800 &((speed>2km not for 1min)&(speed<25kmph))))or(signal missing)) */
			{
				Seat_Belt_Chime =0;
				//SBR_chime_condition_arrived = 0;
				ODO_arrived=0;
				spd_2km_arrived=0;
				if(Sound_Bit == 0x80)
				{
					rpm8000_arrived=0;
				}
			}
		  if(Tacho_invalid_default == 1||Speed_Invalid_Default == 1)
			{
				SBR_chime_condition_arrived = 0;//clear this bit only if Invalid RPM or speed present
			}
		}
		else
		{
			AIR_BAG_TT = 0xfff;
			SEAT_BELT_TT=0;
			Seat_Belt_Chime =0;
			SBR_chime_condition_arrived = 0;
			CAN_IC1_100[5].bits.Rx_2=0;    //FEEDBK_SRS_LAMP
			AIR_BAG_TT_LOD=0;
			AIR_BAG_TT_LSD=0;
			DID_TT_bit_data_set1.bits.Rx_6=0;
			DID_TT_bit_data_set2.bits.Rx_1=1;
			
			if(SBR_chime_on == 1 )//Turn off SBR buzzer instantly on message missing
			{
				SBR_chime_on = 0;
				Buzz_SBR_lv2 = 1;
				Buzz_ON = 1;
				counter7 = 8;
				Buzz_SBR_lv2_cntr = Diag_RW_Final_chime_duration+5;//140
				r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
			}
		}

	}
	else
	{
		AIR_BAG_TT = 0;
		SEAT_BELT_TT=0;
		CAN_IC1_100[5].bits.Rx_2=0;    //FEEDBK_SRS_LAMP
		DID_TT_bit_data_set1.bits.Rx_6=0;
		DID_TT_bit_data_set2.bits.Rx_1=0;
	}



#ifdef GRAYSCALE_TFT
	/*Seat BElt*/
	if((Diag_RW_SeatBelt_Chime_Enable==1)&&(VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM == 1))
	{
		if((Seat_Belt_Chime == 1) && ((rpm8000_arrived == 1)
				|| (spd_2km_arrived == 1) || (ODO_arrived == 1)||(SBR_Speed_Activation==1) || ((display_speedometer_speed > Diag_RW_Vehicle_Moving_final_speed) && (vehicle_moving_state == 1))) && (Buzz_rever_gr_SB != 1))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x80 || Sound_Bit == 0x200 || Sound_Bit == 0x2000)  && (Buzz_SBR_lv2_compl == 0))
			{
				Sound_Bit = 0x80;
				Buzz_SBR_lv2=1;
				Passenger_Seat_belt_alert_flag_ON=1;
			}
			else
			{
				Buzz_SBR_lv2=0;
//				Passenger_Seat_belt_alert_flag_ON=0;		/*commented - After chime off alert missing from Warning History*/
			}
		}
		else if(((Buzz_SBR_lv2 == 1) || (Buzz_SBR_lv2_compl==1)) && (Buzz_ON == 0))/* if(Buzz_SBR_lv2_compl == 1)*/
		{
			if(Sound_Bit == 0x80)
			{
				Sound_Bit = 0x0;
				rpm8000_arrived=0;
			}
			Buzz_SBR_lv2=0;
			Buzz_SBR_lv2_compl=0;
			Buzz_SBR_lv2_cntr=0;
			Passenger_Seat_belt_alert_flag_ON=0;
		}
	}
#endif

#ifdef COLOR_TFT
	/*Seat BElt*/
	if((Diag_RW_SeatBelt_Chime_Enable==1)&&(VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM == 1))
	{
/*		if((Seat_Belt_Chime == 1) && (BuckledFaulty == 0 && Psngr_BuckledFaulty == 0) && (Enginerunningflag == 1) && (display_speedometer_speed > 25) &&(Buzz_rever_gr_SB != 1)&&(Seatbelt_ongoing == 0)&&(chime_start1 == 0)&&(cycle133_completed == 0))*/
		if((Seat_Belt_Chime == 1) && ((rpm8000_arrived == 1)
				|| (spd_2km_arrived == 1) || (ODO_arrived == 1)||(SBR_Speed_Activation==1) || ((display_speedometer_speed > Diag_RW_Vehicle_Moving_final_speed) && (vehicle_moving_state == 1))) && (Buzz_rever_gr_SB != 1))

		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x80 || Sound_Bit == 0x200 || Sound_Bit == 0x2000)  && (cycle133_completed == 0))
			{
				//SoundPCM_Seatbelt_Lv0cont();
				Sound_Bit = 0x80;							/*1000 0000 - High Temp sound bit*/
				chime_start1 = 1;
				Seatbelt_ongoing = 1;
				Seatbelt_missed = 0;
				Passenger_Seat_belt_alert_flag_ON=1;
			}
			else
			{
				Seatbelt_missed = 0;
				Seatbelt_ongoing = 0;
//				Passenger_Seat_belt_alert_flag_ON=0;		/*commented - After chime off alert missing from Warning History*/
			}
		}
		else if((Seat_Belt_Chime != 1)||(Buzz_rever_gr_SB == 1))
		{
			Seatbelt_ongoing = 0;
			cycle133_completed = 0;
			Seatbelt_missed = 1;
			Passenger_Seat_belt_alert_flag_ON=0;

			if(Sound_Bit == 0x80)
			{
				Sound_Bit = 0x0;							/*Sound bit CLEAR*/
			}
		}
	}
	/*else if(chime_start1 == 0)
	{
		Seatbelt_ongoing = 0;
	}*/
#endif


	if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1 && EMS1_10_message_timeout_flag == CLEAR)
	{
		if(fuel__temp_udc_enable==1)
		{
			//ECT_high_TT//
			if(CAN_EMS1_10[7].byte == 0xFF)
			{
				ECT_high_TT= telltale_blink_flag_patt7;
				CAN_IC1_100[5].bits.Rx_7=0;
				ECT_ON_flag=0;
				ECT_Blink_flag=1;//0
			}
			else if((CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_ON )) && (CAN_EMS1_10[7].byte < (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_ON )) && ECT_ON_flag == 0 )
			//else if((CAN_EMS1_10[7].byte >= (ENGINE_HIGHTEMP_TELLTALE_BLINK_ON )) && (CAN_EMS1_10[7].byte < (ENGINE_HIGHTEMP_TELLTALE_CONT_ON )) && ECT_ON_flag == 0 )
			{
				ECT_high_TT= telltale_blink_flag_patt7;//up trend
				CAN_IC1_100[5].bits.Rx_7=0;
				ECT_Blink_flag=1;
			}
			else if(CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_ON ))
			//else if(CAN_EMS1_10[7].byte >= ENGINE_HIGHTEMP_TELLTALE_CONT_ON )
			{
				ECT_high_TT=0xfff;
				CAN_IC1_100[5].bits.Rx_7=1;
				ECT_ON_flag=1;
				ECT_Blink_flag=0;
				DID_TT_bit_data_set3.bits.Rx_8=1;
			}
			else if((CAN_EMS1_10[7].byte > (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_OFF )) && (CAN_EMS1_10[7].byte < (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_ON )) && ECT_ON_flag == 1 )
//			else if((CAN_EMS1_10[7].byte > (ENGINE_HIGHTEMP_TELLTALE_CONT_OFF )) && (CAN_EMS1_10[7].byte < (ENGINE_HIGHTEMP_TELLTALE_CONT_ON )) && ECT_ON_flag == 1 )

			{
				ECT_high_TT=0xfff;
				CAN_IC1_100[5].bits.Rx_7=1;
				DID_TT_bit_data_set3.bits.Rx_8=1;
			}

			else if((CAN_EMS1_10[7].byte <= (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_ON )) && (CAN_EMS1_10[7].byte > (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_OFF )) && ((ECT_Blink_flag == 1) || (ECT_ON_flag == 1)) )
//			else if((CAN_EMS1_10[7].byte <= (ENGINE_HIGHTEMP_TELLTALE_BLINK_ON )) && (CAN_EMS1_10[7].byte > (ENGINE_HIGHTEMP_TELLTALE_BLINK_OFF )) && ((ECT_Blink_flag == 1) || (ECT_ON_flag == 1)) )
			{
				ECT_high_TT= telltale_blink_flag_patt7;//down trend
				CAN_IC1_100[5].bits.Rx_7=0;
				ECT_ON_flag=0;
				ECT_Blink_flag=1;
			}
			else
			{
				ECT_high_TT= 0x000;
				ECT_ON_flag=0;
				ECT_Blink_flag=0;
				CAN_IC1_100[5].bits.Rx_7=0;
				DID_TT_bit_data_set3.bits.Rx_8=0;
			}
		}
		/*		//ECT_high_TT//
		if(CAN_EMS1_10[7].byte == 0xFF)
		{
			ECT_high_TT= telltale_blink_flag_patt7;
			CAN_IC1_100[5].bits.Rx_7=0;
			ECT_ON_flag=0;
			ECT_Blink_flag=0;
		}
		else if(CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_ON ))
		{
			ECT_high_TT=0xfff;
			CAN_IC1_100[5].bits.Rx_7=1;
			ECT_ON_flag=1;
			ECT_Blink_flag=0;
		}
		else if((CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_OFF )) && (CAN_EMS1_10[7].byte < (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_ON )) && ECT_ON_flag == 1 )
		{
			ECT_high_TT=0xfff;
			CAN_IC1_100[5].bits.Rx_7=1;
		}
		else if((CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_ON )) && (CAN_EMS1_10[7].byte < (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_ON )) && ECT_ON_flag == 0 )
		{
			ECT_high_TT= telltale_blink_flag_patt7;
			CAN_IC1_100[5].bits.Rx_7=0;
			ECT_Blink_flag=1;
		}
		else if((CAN_EMS1_10[7].byte < (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_ON )) && (CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_OFF )) && ECT_Blink_flag == 1 )
		{
			ECT_high_TT= telltale_blink_flag_patt7;
			CAN_IC1_100[5].bits.Rx_7=0;
			ECT_ON_flag=0;
		}
		else if((CAN_EMS1_10[7].byte < (0x28 + ENGINE_HIGHTEMP_TELLTALE_CONT_OFF )) && (CAN_EMS1_10[7].byte >= (0x28 + ENGINE_HIGHTEMP_TELLTALE_BLINK_OFF )) && ((ECT_Blink_flag == 1) || (ECT_ON_flag == 1)) )
		{
			ECT_high_TT= telltale_blink_flag_patt7;
			CAN_IC1_100[5].bits.Rx_7=0;
		}
		else
		{
			ECT_high_TT= 0x000;
			ECT_ON_flag=0;
			ECT_Blink_flag=0;
			CAN_IC1_100[5].bits.Rx_7=0;
		}
		 */
#ifdef GRAYSCALE_TFT
		if(Diag_RW_HighTemp_Chime_Enable==1)
		{
			if((CAN_IC1_100[5].bits.Rx_7 == 1) && (Enginerun_chm == 1))
			{
				if((Sound_Bit == 0x0 || Sound_Bit == 0x1 || Sound_Bit == 0x200) && Buzz_High_Temp_compl == 0)
				{
					Sound_Bit = 0x1;
					Buzz_High_Temp=1;
				}
				else
				{
					Buzz_High_Temp=0;
				}
			}
			else if(Buzz_High_Temp_compl == 1)
			{
				if(Sound_Bit == 0x1)
				{
					Sound_Bit=0x0;
				}
				Buzz_High_Temp=0;
				Buzz_High_Temp_compl=0;
				Buzz_High_Temp_cntr=0;
			}
		}
#endif

#ifdef COLOR_TFT
		if(Diag_RW_HighTemp_Chime_Enable==1)
		{
			if((CAN_IC1_100[5].bits.Rx_7 == 1) && (Enginerun_chm == 1)&&(High_temp_ongoing == 0)&&(chime_start1 == 0)&&(cycle_Hightemp_completed==0))
			{
				//			SoundPCM_RPAS_St3();
				if((Sound_Bit == 0x0 || Sound_Bit == 0x1 || Sound_Bit == 0x200) && cycle_Hightemp_completed == 0)
				{
					Sound_Bit = 0x1;					/*0000 0000 0001 - High Temp sound bit*/
					chime_start1 = 1;
					High_temp_ongoing = 1;
					Hightemp_missed = 0;
				}
				else
				{
					High_temp_ongoing = 0;
					Hightemp_missed = 0;
				}
			}
			else if(CAN_IC1_100[5].bits.Rx_7 != 1)
			{
				if(Sound_Bit == 0x1)
				{
					Sound_Bit=0x0;						/*Sound bit CLEAR*/
				}
				High_temp_ongoing = 0;
				cycle_Hightemp_completed = 0;
				Hightemp_missed = 1;
			}
		}
		/*else if(chime_start1 == 0)
		{
			High_temp_ongoing = 0;
		}*/
#endif

		/* GLOW PLUG TT */
		if(FEATURE_CODE_FUEL_TYPE==1 && FEATURE_CODE_GLOWPLUG_TT_CONFIG==1)
		{
			three_bit_data.bits.Rx_1 = CAN_EMS1_10[6].bits.Rx_3;
			three_bit_data.bits.Rx_2 = CAN_EMS1_10[6].bits.Rx_4;
			three_bit_data.bits.Rx_3 = CAN_EMS1_10[6].bits.Rx_5;

			if(three_bit_data.byte == 1)		/* Glow_Plug_TT Telltale */
			{
				Glow_Plug_TT = 1;
				DID_TT_bit_data_set1.bits.Rx_2=1;
			}
			else if(three_bit_data.byte > 2 || three_bit_data.byte <8)
			{
				Glow_Plug_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_2=0;
			}
			else if(three_bit_data.byte == 0)
			{
				Glow_Plug_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_2=0;
			}
		}
		else
		{
			Glow_Plug_TT = 0;
			DID_TT_bit_data_set1.bits.Rx_2=0;

		}

		if(FEATURE_CODE_FUEL_TYPE==1)
		{
			if(CAN_EMS1_10[5].bits.Rx_5 ==1)
			{
				Water_in_fuel_TT =1;
				DID_TT_bit_data_set2.bits.Rx_6=1;
			}
			else if(CAN_EMS1_10[5].bits.Rx_5 ==0)
			{
				Water_in_fuel_TT =0;
				DID_TT_bit_data_set2.bits.Rx_6=0;
			}
		}
		else
		{
			Water_in_fuel_TT =0;
			DID_TT_bit_data_set2.bits.Rx_6=0;
		}


#ifdef GRAYSCALE_TFT
		if(Diag_RW_WIF_Chime_Enable==1)
		{
			if((Water_in_fuel_TT== 1) && (Enginerun_chm == 1))
			{
				WIF_5sec_done = 1;
				if((Sound_Bit == 0x0 || Sound_Bit == 0x1000 || Sound_Bit == 0x200) && Buzz_WIF_compl == 0)
				{
					Sound_Bit = 0x1000;
					Buzz_WIF=1;
				}
				else
				{
					Buzz_WIF=0;
				}

			}
			else if(Buzz_WIF_compl == 1)
			{
				if(Sound_Bit == 0x1000)
				{
					Sound_Bit = 0x0;
				}
				Buzz_WIF=0;
				Buzz_WIF_compl=0;
				Buzz_WIF_cntr=0;
			}
		}
#endif

#ifdef COLOR_TFT
		if(Diag_RW_WIF_Chime_Enable==1)
		{
			if((Water_in_fuel_TT == 1)&&(WIF_ongoing == 0)&&(chime_start1 == 0)&&(cycle_waterinfuel_completed == 0)&&(Enginerun_chm == 1))
			{
				WIF_5sec_done = 1;
				if((Sound_Bit == 0x0 || Sound_Bit == 0x1000 || Sound_Bit == 0x200) && cycle_waterinfuel_completed == 0)
				{
					Sound_Bit = 0x1000;				/*0001 0000 0000 0000 - Water In Fuel sound bit*/
					WIF_ongoing = 1;
					chime_start1 = 1;
					WIF_missed = 0;
				}
				else
				{
					WIF_ongoing = 0;
					WIF_missed = 0;
				}
			}
			else if(Water_in_fuel_TT != 1)	/*Signal Missed / Signal Inactive condition*/
			{
				if(Sound_Bit == 0x1000)
				{
					Sound_Bit=0x0;
				}
				WIF_ongoing = 0;
				cycle_waterinfuel_completed = 0;
				WIF_missed = 1;
			}
			else if (WIF_5sec_done == 0)
			{
				WIF_ongoing = 0;
				cycle_waterinfuel_completed = 0;
			}
		}
#endif

		if(FEATURE_CODE_CRUISE_TT_CONFIG==1)
		{
			two_bit_data.bits.Rx_1 = CAN_EMS1_10[4].bits.Rx_7;  /* cruise_tt*/
			two_bit_data.bits.Rx_2 = CAN_EMS1_10[4].bits.Rx_8;

			/* if(two_bit_data.byte== 0x3)   // to log signal invalid DTC
			{
				Failure_DTC_Set(DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055);
			}
			else
			{
				Failure_DTC_Clear(DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055);
			} */

			if(two_bit_data.byte == 1)
			{
				//cruise_tt=1
				CRUISE_TT=0xfff;
				DID_TT_bit_data_set2.bits.Rx_2=1;
				/*Cruise telltale feedback*/
				if(read_SID[241]==1 || read_SID[145]==1)
				{
					CAN_IC2_100[4].bits.Rx_1=1;    //FEEDBK_CRUISE_LAMP
					if(read_SID[241]==1)
					{
						CRUISE_TT_LOD=1;
					}
					else
					{
						CRUISE_TT_LOD=0;
					}
					if(read_SID[145]==1)
					{
						CRUISE_TT_LSD=1;
					}
					else
					{
						CRUISE_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[4].bits.Rx_1=0;    //FEEDBK_CRUISE_LAMP
					CRUISE_TT_LOD=0;
					CRUISE_TT_LSD=0;
				}
			}
			else if(two_bit_data.byte !=1)
			{
				CRUISE_TT = 0x000;
				DID_TT_bit_data_set2.bits.Rx_2=0;
			}
		}
		else
		{
			CRUISE_TT= 0x000;
			CAN_IC2_100[4].bits.Rx_1=0;    //FEEDBK_CRUISE_LAMP
			DID_TT_bit_data_set2.bits.Rx_2=0;
			CRUISE_TT_LOD=0;
			CRUISE_TT_LSD=0;
		}
		HCT_missing = 0;
	}
	else
	{
		CRUISE_TT=0;
		Glow_Plug_TT = 0;
		Water_in_fuel_TT =0;
/**/
		if(Sound_Bit == 0x1000)
		{
			Sound_Bit = 0x0;
			Buzz_WIF = 0;
		}
/**/
		ECT_high_TT= 0x000;
		CAN_IC2_100[4].bits.Rx_1=0;    //FEEDBK_CRUISE_LAMP
		DID_TT_bit_data_set1.bits.Rx_2=0;
		DID_TT_bit_data_set2.bits.Rx_2=0;
		DID_TT_bit_data_set3.bits.Rx_8=0;
		DID_TT_bit_data_set2.bits.Rx_6=0;
/*******************************************************************engine oil pressure*****************invictus**********/
			Low_fuel_ongoing = 0;
			cycle_lowfuel_completed = 0;   //added for testing only
			chime_start1=0;
//			Low_Oil_press_TT = 0xfff;		/*Telltale off*/
			cycle_Lowengineoil_completed=0;/*invictus added to fix the bug*/
			Low_engine_oil_pressure	=0;
			cycleLowengineoil_cntr = 0;
/******************************************************************************************************************/
		//for chime
#ifdef GRAYSCALE_TFT
		Buzz_High_Temp=0;
		Buzz_High_Temp_compl=0;
		Buzz_High_Temp_cntr=0;
		Buzz_WIF_compl=0;/*added to fix the bug msg recivery condition ems1_10*/
		Buzz_WIF_cntr=0;/*added to fix the bug w501L buzz not play after can recovery*/ 
		if(HCT_missing == 0)
		{
			r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
			HCT_missing = 1;
		}
#endif
		Engine_Temp_High_popup_shown=0;

	}

	if(EMS29_100_message_timeout_flag == CLEAR && VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
	{
		if(FEATURE_CODE_REGEN==1)
		{
			two_bit_data.bits.Rx_1 = CAN_EMS29_100[3].bits.Rx_7;  /* regen_tt*/
			two_bit_data.bits.Rx_2 = CAN_EMS29_100[3].bits.Rx_8;
			if(two_bit_data.byte == 0 || two_bit_data.byte == 3)
			{
				REGEN_TT=0;
				DID_TT_bit_data_set6.bits.Rx_4=0;
			}
			else if(two_bit_data.byte ==1)
			{
				REGEN_TT=1;
				DID_TT_bit_data_set6.bits.Rx_4=1;

			}
			else if(two_bit_data.byte ==2)
			{
				REGEN_TT=telltale_blink_bool3;
				DID_TT_bit_data_set6.bits.Rx_4=1;
			}
		}
		else
		{
			REGEN_TT=0;
			DID_TT_bit_data_set6.bits.Rx_4=0;
		}

		if(FEATURE_CODE_DEF_TT_CONFIG==1)
		{
			if(CAN_EMS29_100[0].byte==0)
			{
				DEF_TT =  0x000;
				DID_TT_bit_data_set6.bits.Rx_2=0;
			}
			if(CAN_EMS29_100[0].byte!=0)
			{
				DEF_TT = 0xfff;
				DID_TT_bit_data_set6.bits.Rx_2=1;
			}
		}
	}
	else
	{
		DEF_TT = 0;
		REGEN_TT=0;
	}

	if(FEATURE_CODE_HIGHBEAM_TT_TYPE==0 && MBFM1_100_message_timeout_flag == CLEAR && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)
	{
		if(CAN_MBFM1_100[5].bits.Rx_6==0)
		{
			High_beam_TT =  0x000;
			DID_TT_bit_data_set4.bits.Rx_4=0;
		}
		if(CAN_MBFM1_100[5].bits.Rx_6==1)
		{
			High_beam_TT = 0xfff;
			DID_TT_bit_data_set4.bits.Rx_4=1;
		}
	}
	else if(FEATURE_CODE_HIGHBEAM_TT_TYPE==0 && MBFM1_100_message_timeout_flag != CLEAR && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)
	{
		High_beam_TT =  0x000;
		DID_TT_bit_data_set4.bits.Rx_4=0;
	}
#if 0
	else if(FEATURE_CODE_HIGHBEAM_TT_TYPE ==1)
	{
		if(HIGH_BEAM_CPU == 1)

		{
			High_beam_TT = 0xfff;
			DID_TT_bit_data_set4.bits.Rx_4=1;
		}
		else if(HIGH_BEAM_CPU == 0)

		{
			High_beam_TT =  0x000;
			DID_TT_bit_data_set4.bits.Rx_4=0;
		}
	}
#endif
	else if(FEATURE_CODE_HIGHBEAM_TT_TYPE == 1)
	{
		//if(HIGH_BEAM_CPU == 0)
		if(!(R_GPIO_PinRead(0, 3)))
		{
			High_beam_TT =  0x000;
			DID_TT_bit_data_set4.bits.Rx_4=0;
		}
		else if(R_GPIO_PinRead(0, 3))
		{
			if(High_Beam_sts == 1)
			{
				High_beam_TT = 0xfff;
			}
			DID_TT_bit_data_set4.bits.Rx_4=1;
			High_Beam_sts = 0;
		}
	}

	if(FEATURE_CODE_PARK_TT_TYPE==0 && MBFM1_100_message_timeout_flag == CLEAR && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)
	{
		two_bit_data.bits.Rx_1 =CAN_MBFM1_100[0].bits.Rx_5;  /* PARKLAMP_TT*/
		two_bit_data.bits.Rx_2 =CAN_MBFM1_100[0].bits.Rx_6;

		if(two_bit_data.byte == 0 || two_bit_data.byte == 2||two_bit_data.byte == 3)
		{
			PARK_LAMP_TT=0;
			if(update_count == 1)
			{
				Counter_500ms = 4900;
				update_count = 0;
				SendGSData(GSdata1);
				output_shift_register();
			}
		}
		else if(two_bit_data.byte ==1)
		{
			PARK_LAMP_TT=1;
			if(update_count == 0)
			{
				Counter_500ms = 4900;
				update_count = 1;
				SendGSData(GSdata1);
				output_shift_register();
			}
		}
		DID_TT_bit_data_set3.bits.Rx_2=PARK_LAMP_TT;
	}
	else if(FEATURE_CODE_PARK_TT_TYPE==1)
	{
		if(ParkLAMP == 1)
		{
			PARK_LAMP_TT = 1; /* enabled */
/*
			if(update_count == 0)
			{
				Counter_500ms = 4900;
				update_count = 1;
				SendGSData(GSdata1);
				output_shift_register();
			}
*/
		}
		else
		{
			PARK_LAMP_TT = 0;
/*
			if(update_count == 1)
			{
				Counter_500ms = 4900;
				update_count = 0;
				SendGSData(GSdata1);
				output_shift_register();
			}
*/
		}
		DID_TT_bit_data_set3.bits.Rx_2=PARK_LAMP_TT;
	}
	else
	{
		PARK_LAMP_TT=0;
		DID_TT_bit_data_set3.bits.Rx_2=PARK_LAMP_TT;
	}

	if(FEATURE_CODE_FRONT_FOG_TT_TYPE==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1 && MBFM1_100_message_timeout_flag == CLEAR)
	{
		if(CAN_MBFM1_100[0].bits.Rx_1==1)
		{
			FRONT_FOG_TT = 1;
		}
		if(CAN_MBFM1_100[0].bits.Rx_1==0)
		{
			FRONT_FOG_TT = 0;
		}
		DID_TT_bit_data_set4.bits.Rx_7=FRONT_FOG_TT;
	}
	else if(FEATURE_CODE_FRONT_FOG_TT_TYPE==1)
	{
		if(FrontFOG == 1)
		{
			FRONT_FOG_TT = 1;
		}
		else
		{
			FRONT_FOG_TT = 0;
		}
		DID_TT_bit_data_set4.bits.Rx_7=FRONT_FOG_TT;
	}
	else
	{
		FRONT_FOG_TT = 0;
		DID_TT_bit_data_set4.bits.Rx_7=FRONT_FOG_TT;
	}

	if(FEATURE_CODE_REAR_FOG_TT_TYPE==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1 && MBFM1_100_message_timeout_flag == CLEAR)
	{
		if(CAN_MBFM1_100[0].bits.Rx_2==1)
		{
			REAR_FOG_TT = 1;
		}
		else if(CAN_MBFM1_100[0].bits.Rx_2==0)
		{
			REAR_FOG_TT = 0;
		}
		DID_TT_bit_data_set4.bits.Rx_8=REAR_FOG_TT;
	}
	else if(FEATURE_CODE_REAR_FOG_TT_TYPE==1)
	{
		if(REAR_FOG_CPU == 1)
		{
			REAR_FOG_TT = 1;

		}
		else if(REAR_FOG_CPU == 0)
		{
			REAR_FOG_TT = 0;
		}
		DID_TT_bit_data_set4.bits.Rx_8=REAR_FOG_TT;
	}
	else
	{
		REAR_FOG_TT = 0;
		DID_TT_bit_data_set4.bits.Rx_8=REAR_FOG_TT;
	}

	if(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1 && MBFM1_100_message_timeout_flag == CLEAR)
	{
		if(CAN_MBFM1_100[4].bits.Rx_1==1 ||CAN_MBFM1_100[4].bits.Rx_2==1 || CAN_MBFM1_100[4].bits.Rx_3==1||CAN_MBFM1_100[4].bits.Rx_4==1|| CAN_MBFM1_100[4].bits.Rx_5==1|| CAN_MBFM1_100[4].bits.Rx_6==1)
		{
			DOOR_OPEN_TT = 1;
			DID_TT_bit_data_set1.bits.Rx_5=1;
		}
		else
		{
			DOOR_OPEN_TT = 0;
			DID_TT_bit_data_set1.bits.Rx_5=0;
		}
	}
	else
	{
		DOOR_OPEN_TT = 0;          //DOOR_OPEN_TT
		DID_TT_bit_data_set1.bits.Rx_5=0;
	}
#ifdef GRAYSCALE_TFT
	/*Door Open*/
	if(Diag_RW_DoorOpen_Chime_Enable==1)
	{
		if((DOOR_OPEN_TT == 1) && (vehicle_moving_state == 1))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x10 || Sound_Bit == 0x200) && Buzz_Door_open_compl == 0)
			{
				Sound_Bit = 0x10;
				Buzz_Door_open=1;
			}
			else
			{
				Buzz_Door_open=0;
			}
		}
		else if(Buzz_Door_open_compl == 1)
		{
			if(Sound_Bit == 0x10)
			{
				Sound_Bit = 0x0;
			}
			Buzz_Door_open=0;
			Buzz_Door_open_compl=0;
			Buzz_Door_open_cntr=0;
		}
		if(MBFM1_100_message_timeout_flag != 0)
		{
			Buzz_Door_open_compl=0;
		}
	}
#endif

#ifdef COLOR_TFT
	/*Door Open*/
	if(Diag_RW_DoorOpen_Chime_Enable==1)
	{
		if((DOOR_OPEN_TT == 1) && (vehicle_moving_state == 1)&&(dooropen_ongoing == 0)&&(chime_start1 == 0)&&(cycle_dooropen_completed == 0))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x10 || Sound_Bit == 0x200) && cycle_dooropen_completed == 0)
			{
				//		SoundPCM_message();
				Sound_Bit = 0x10;				/*0001 0000 - Door Open sound bit*/
				dooropen_ongoing = 1;
				chime_start1 = 1;
				Dooropen_missed = 0;
			}
			else
			{
				dooropen_ongoing = 0;
				Dooropen_missed = 0;
			}
		}
		else if(DOOR_OPEN_TT != 1)	/*Signal Missed / Signal Inactive condition*/
		{
			if(Sound_Bit == 0x10)
			{
				Sound_Bit=0x0;
			}
			dooropen_ongoing = 0;
			cycle_dooropen_completed = 0;
			Dooropen_missed = 1;
		}
		else if(vehicle_moving_state == 0)
		{
			if(Sound_Bit == 0x10)
			{
				Sound_Bit=0x0;
			}
			dooropen_ongoing = 0;
			cycle_dooropen_completed = 0;
			Dooropen_missed = 0;
		}
	}
	/*else if(chime_start1 == 0)
	{
		dooropen_ongoing = 0;
	}*/
#endif
	/** 4WD_HIGH_LOW_TT** read srd for f & h condition**/
	if(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1)
	{
		if(FEATURE_CODE_4WD_LOGIC==0)
		{
			/*After IGN OFF --> ON with 4WD H/L Engaged*/
			if((FOUR_WD_HIGH_CPU ==1 && FOUR_WD_FEEDBACK_CPU==1 && IGN_4WD_STS == 0)||(FOUR_WD_LOW_CPU== 1 && FOUR_WD_FEEDBACK_CPU==1 && IGN_4WD_STS == 0))
			{
				prev_2WHD = 1;
				IGN_4WD_STS = 1;
			}
			/* 2WD to 4WD start */
			/* sr.1 */
			if((FOUR_WD_LOW_CPU == 0 && FOUR_WD_HIGH_CPU == 1) || (FOUR_WD_LOW_CPU == 1 && FOUR_WD_HIGH_CPU == 0))
			{
				Flag_4WHTT2_1000ms_start=0;
				Flag_4WHTT_1000ms_start=1;
				TT24WH_1000ms_cmplt_flg=0;

				if(TT4WH_1000ms_cmplt_flg == 0)
				{
					//FOUR_WD_OUTPUT_CPU=1;
					R_GPIO_WritePin(17, 10, 0);
				}
				else
				{
					//FOUR_WD_OUTPUT_CPU=0;
					//	TT4WH_1000ms_cmplt_flg=1;
					R_GPIO_WritePin(17, 10, 1);
				}
			}
			else if((FOUR_WD_LOW_CPU == 1 && FOUR_WD_HIGH_CPU == 1) || (FOUR_WD_LOW_CPU == 0 && FOUR_WD_HIGH_CPU == 0))
			{
				Flag_4WHTT_1000ms_start=0;
				Flag_4WHTT2_1000ms_start=1;
				TT4WH_1000ms_cmplt_flg=0;

				if(TT24WH_1000ms_cmplt_flg == 0)
				{
					//FOUR_WD_OUTPUT_CPU=1;
					if(first_flag==0)
					{
						first_flag=1;
						//			R_GPIO_WritePin(17, 10, 0);
					}
					else
					{
						//		R_GPIO_WritePin(17, 10, 1);
					}
				}
				else
				{
					//	TT4WH_1000ms_cmplt_flg=1;
					//FOUR_WD_OUTPUT_CPU=0;
					R_GPIO_WritePin(17, 10, 0);
				}
			}
			else
			{
				Flag_4WHTT_1000ms_start=0;
				TT4WH_1000ms_cmplt_flg=0;
				Flag_4WHTT2_1000ms_start=0;
				TT24WH_1000ms_cmplt_flg=0;
				R_GPIO_WritePin(17, 10, 0);
			}

			if(FOUR_WD_LOW_CPU== 1 && FOUR_WD_HIGH_CPU==1 && FOUR_WD_FEEDBACK_CPU==0)
			{
				FOUR_WD_LOW_TT=0;
				FOUR_WD_HIGH_TT=0;

				//	FOUR_WD_OUTPUT_CPU=1;
				//	R_GPIO_WritePin(17, 10, 0);

				CAN_IC2_100[3].bits.Rx_6=0;
				CAN_IC2_100[3].bits.Rx_7=0;

				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				//	Flag_4WHTT_1000ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=0;
				prev_4WDlow=0;
				prev_2WHD=1;
				DID_TT_bit_data_set6.bits.Rx_3=0;
			}
			/* sr.2.a */
			else if((FOUR_WD_LOW_CPU == 0 && FOUR_WD_HIGH_CPU ==1 && FOUR_WD_FEEDBACK_CPU==0) && (prev_2WHD==1))
			{
				FOUR_WD_LOW_TT =telltale_blink_at_1sec;
				FOUR_WD_HIGH_TT=0;
				/*
				Flag_4WHTT_1000ms_start=1;

				if(TT4WH_1000ms_cmplt_flg == 0)
				{
					//FOUR_WD_OUTPUT_CPU=1;
					R_GPIO_WritePin(17, 10, 0);
				}
				else
				{
					//FOUR_WD_OUTPUT_CPU=0;
					R_GPIO_WritePin(17, 10, 1);
				}
				 */
				CAN_IC2_100[3].bits.Rx_6 = bit6_4WHD;
				CAN_IC2_100[3].bits.Rx_7 = bit7_4WHD;

				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=0;
				prev_4WDlow=0;
				DID_TT_bit_data_set6.bits.Rx_3=1;
			}
			/* sr.2.b */
			else if((FOUR_WD_LOW_CPU== 0 && FOUR_WD_HIGH_CPU ==1 && FOUR_WD_FEEDBACK_CPU==1) && (prev_2WHD==1))
			{
				FOUR_WD_LOW_TT=1;
				FOUR_WD_HIGH_TT=0;
				//FOUR_WD_OUTPUT_CPU=0;
				//		R_GPIO_WritePin(17, 10, 1);
				DID_TT_bit_data_set1.bits.Rx_4=0;
				CAN_IC2_100[3].bits.Rx_6=0;
				CAN_IC2_100[3].bits.Rx_7=1;
				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				//		Flag_4WHTT_1000ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=0;
				prev_4WDlow=1;
				prev_2WHD=0;
				DID_TT_bit_data_set6.bits.Rx_3=1;
			}
			/* sr.3.a */
			else if((FOUR_WD_LOW_CPU== 1 && FOUR_WD_HIGH_CPU ==0 && FOUR_WD_FEEDBACK_CPU ==0) && (prev_2WHD==1))
			{
				FOUR_WD_LOW_TT=0;
				FOUR_WD_HIGH_TT=telltale_blink_at_1sec;
				DID_TT_bit_data_set1.bits.Rx_4=0;
				/*after delay of 1sec*/
				/*			Flag_4WHTT_1000ms_start=1;

				if(TT4WH_1000ms_cmplt_flg == 0)
				{
					//	FOUR_WD_OUTPUT_CPU=1;
					R_GPIO_WritePin(17, 10, 0);
				}
				else
				{
					//	FOUR_WD_OUTPUT_CPU=0;
					R_GPIO_WritePin(17, 10, 1);
				}
				 */
				CAN_IC2_100[3].bits.Rx_6 = bit6_4WHD;
				CAN_IC2_100[3].bits.Rx_7 = bit7_4WHD;

				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=0;
				prev_4WDlow=0;
			}
			/* sr.3.b */
			else if((FOUR_WD_LOW_CPU== 1 && FOUR_WD_HIGH_CPU == 0 && FOUR_WD_FEEDBACK_CPU ==1) && (prev_2WHD==1))
			{
				FOUR_WD_LOW_TT=0;
				FOUR_WD_HIGH_TT=1;
				DID_TT_bit_data_set1.bits.Rx_4=1;
				//	FOUR_WD_OUTPUT_CPU=0;
				//		R_GPIO_WritePin(17, 10, 1);

				CAN_IC2_100[3].bits.Rx_6=1;
				CAN_IC2_100[3].bits.Rx_7=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				//			Flag_4WHTT_1000ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				prev_2WHD=0;
				prev_4WDhigh=1;
				prev_4WDlow=0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
			}
			/* 2WD to 4WD end */
			/* 4WD to 4WD start */
			/* sr.4 */
			else if((FOUR_WD_LOW_CPU== 0 && FOUR_WD_HIGH_CPU ==1 && FOUR_WD_FEEDBACK_CPU==1/*0*/) && (prev_4WDhigh==1))
			{
				/* sr.4 */
				//for first 500 msec
				Flag_4WHTT_500ms_start=1;
				Flag_4WHTT_1500ms_start=1;

				if(TT4WH_500ms_cmplt_flg == 0)
				{
					CAN_IC2_100[3].bits.Rx_6 = bit6_4WHD;
					CAN_IC2_100[3].bits.Rx_7 = bit7_4WHD;
				}
				else
				{
					CAN_IC2_100[3].bits.Rx_6 = 0;
					CAN_IC2_100[3].bits.Rx_7 = 1;
				}

				//for  1.5 seconds
				if(TT4WH_1500ms_cmplt_flg == 0)
				{
					FOUR_WD_LOW_TT = 0;
					FOUR_WD_HIGH_TT = 0;

					//	FOUR_WD_OUTPUT_CPU=0;
					//		R_GPIO_WritePin(17, 10, 1);

					prev_4WDhigh=1;
					prev_4WDlow=0;
					prev_2WHD=0;

				}/* sr.4.a */
				else
				{
					//after 1.5 seconds
					FOUR_WD_LOW_TT = 1;
					FOUR_WD_HIGH_TT = 0;

					//	FOUR_WD_OUTPUT_CPU=0;
					//			R_GPIO_WritePin(17, 10, 1);

					prev_4WDhigh=0;
					prev_4WDlow=1;
					prev_2WHD=0;
					DID_TT_bit_data_set6.bits.Rx_3=1;
					DID_TT_bit_data_set1.bits.Rx_4=0;
				}
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
			}
			/* sr.4.a */
			else if((FOUR_WD_LOW_CPU== 0 && FOUR_WD_HIGH_CPU == 1 && FOUR_WD_FEEDBACK_CPU ==1) && (prev_4WDlow == 1))
			{
				FOUR_WD_LOW_TT = 1;
				FOUR_WD_HIGH_TT = 0;

				//	FOUR_WD_OUTPUT_CPU=0;
				//		R_GPIO_WritePin(17, 10, 1);

				CAN_IC2_100[3].bits.Rx_6 = 0;
				CAN_IC2_100[3].bits.Rx_7 = 1;

				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=0;
				prev_4WDlow=1;
				prev_2WHD=0;
				DID_TT_bit_data_set6.bits.Rx_3=1;
				DID_TT_bit_data_set1.bits.Rx_4=0;
			}
			/* sr.5 */
			else if((FOUR_WD_LOW_CPU== 1 && FOUR_WD_HIGH_CPU == 0 && FOUR_WD_FEEDBACK_CPU ==1) && (prev_4WDlow==1))
			{
				/* sr.5 */
				//for first 500 msec
				Flag_4WHTT_500ms_start=1;
				Flag_4WHTT_1500ms_start=1;

				if(TT4WH_500ms_cmplt_flg == 0)
				{
					CAN_IC2_100[3].bits.Rx_6 = bit6_4WHD;
					CAN_IC2_100[3].bits.Rx_7 = bit7_4WHD;
				}
				else
				{
					CAN_IC2_100[3].bits.Rx_6 = 1;
					CAN_IC2_100[3].bits.Rx_7 = 0;
				}

				//for  1.5 seconds
				if(TT4WH_1500ms_cmplt_flg == 0)
				{
					FOUR_WD_HIGH_TT = 0;  //remaining 500 msec
					FOUR_WD_LOW_TT = 0;

					//		FOUR_WD_OUTPUT_CPU=0;
					//			R_GPIO_WritePin(17, 10, 1);

					prev_4WDhigh=0;
					prev_4WDlow=1;
					prev_2WHD=0;
					DID_TT_bit_data_set6.bits.Rx_3=0;
				}
				/* sr.5.a */ //after 1.5 seconds
				else
				{
					FOUR_WD_LOW_TT = 0;
					FOUR_WD_HIGH_TT = 1;

					//	FOUR_WD_OUTPUT_CPU=0;
					//			R_GPIO_WritePin(17, 10, 1);

					prev_4WDhigh=1;
					prev_4WDlow=0;
					prev_2WHD=0;
					DID_TT_bit_data_set6.bits.Rx_3=0;
					DID_TT_bit_data_set1.bits.Rx_4=1;
				}
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
			}
			/* sr.5.a */
			else if((FOUR_WD_LOW_CPU== 1 && FOUR_WD_HIGH_CPU == 0 && FOUR_WD_FEEDBACK_CPU ==1) && (prev_4WDhigh == 1))
			{
				FOUR_WD_LOW_TT = 0;
				FOUR_WD_HIGH_TT = 1;

				//	FOUR_WD_OUTPUT_CPU=0;
				//		R_GPIO_WritePin(17, 10, 1);

				CAN_IC2_100[3].bits.Rx_6 = 1;
				CAN_IC2_100[3].bits.Rx_7 = 0;

				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				Flag_4WHTT_10s_start=0;
				TT4WH_10s_cmplt_flg=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=1;
				prev_4WDlow=0;
				prev_2WHD=0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
				DID_TT_bit_data_set1.bits.Rx_4=1;
			}
			else if(FOUR_WD_LOW_CPU== 0 && FOUR_WD_HIGH_CPU==0 )
			{
				Flag_4WHTT_10s_start=1;

				if(TT4WH_10s_cmplt_flg == 0)
				{
					FOUR_WD_LOW_TT=telltale_blink_at_1sec;
					FOUR_WD_HIGH_TT=telltale_blink_at_1sec;
				}
				else
				{
					FOUR_WD_LOW_TT=0;
					FOUR_WD_HIGH_TT=0;
				}

				//		FOUR_WD_OUTPUT_CPU=1;
				//		R_GPIO_WritePin(17, 10, 0);

				CAN_IC2_100[3].bits.Rx_6 = 0;
				CAN_IC2_100[3].bits.Rx_7 = 0;

				Flag_4WHTT_500ms_start=0;
				Flag_4WHTT_1500ms_start=0;
				TT4WH_500ms_cmplt_flg=0;
				TT4WH_1500ms_cmplt_flg=0;
				prev_4WDhigh=0;
				prev_4WDlow=0;
				prev_2WHD=0;
			}

			bit6_4WHD=CAN_IC2_100[3].bits.Rx_6;
			bit7_4WHD=CAN_IC2_100[3].bits.Rx_7;
		}

		else if(FEATURE_CODE_4WD_LOGIC == 1)
		{
			/* sr. 1 */
			if(FOUR_WD_HIGH_CPU == 1 && FOUR_WD_LOW_CPU==1 && FOUR_WD_FEEDBACK_CPU==0)
			{
				FOUR_WD_LOW_TT=0;
				FOUR_WD_HIGH_TT=0;
				CAN_IC2_100[3].bits.Rx_6 = 0;
				CAN_IC2_100[3].bits.Rx_7 = 0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
				DID_TT_bit_data_set1.bits.Rx_4=0;
			}
			/* sr. 2b */
			else if(FOUR_WD_HIGH_CPU== 0 && FOUR_WD_LOW_CPU==1 && FOUR_WD_FEEDBACK_CPU==1)
			{
				FOUR_WD_LOW_TT=0;
				FOUR_WD_HIGH_TT=1;
				CAN_IC2_100[3].bits.Rx_6 = 1;
				CAN_IC2_100[3].bits.Rx_7 = 0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
				DID_TT_bit_data_set1.bits.Rx_4=1;
			}
			/* sr. 3b */
			else if(FOUR_WD_HIGH_CPU== 1 && FOUR_WD_LOW_CPU==0 && FOUR_WD_FEEDBACK_CPU==1)
			{
				FOUR_WD_LOW_TT=1;
				FOUR_WD_HIGH_TT=0;
				CAN_IC2_100[3].bits.Rx_6 = 0;
				CAN_IC2_100[3].bits.Rx_7 = 1;
				DID_TT_bit_data_set6.bits.Rx_3=1;
				DID_TT_bit_data_set1.bits.Rx_4=0;
			}
#if 1
			/* 2a */
			else if(FOUR_WD_HIGH_CPU== 0 && FOUR_WD_LOW_CPU==1 && FOUR_WD_FEEDBACK_CPU==0)
			{
				FOUR_WD_HIGH_TT=1;
				FOUR_WD_LOW_TT=0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
				DID_TT_bit_data_set1.bits.Rx_4=1;
			}
			/* 3a */
			else if(FOUR_WD_HIGH_CPU == 1 && FOUR_WD_LOW_CPU==0 && FOUR_WD_FEEDBACK_CPU==0)
			{
				FOUR_WD_HIGH_TT=0;
				FOUR_WD_LOW_TT=1;
				DID_TT_bit_data_set6.bits.Rx_3=1;
				DID_TT_bit_data_set1.bits.Rx_4=0;
			}
			/* 5a */
			else if(FOUR_WD_HIGH_CPU== 0 && FOUR_WD_LOW_CPU==0 && FOUR_WD_FEEDBACK_CPU==1)
			{
				FOUR_WD_HIGH_TT=1;
				FOUR_WD_LOW_TT=1;
				DID_TT_bit_data_set6.bits.Rx_3=1;
				DID_TT_bit_data_set1.bits.Rx_4=1;
			}
#endif
			else
			{
				FOUR_WD_LOW_TT=0;
				FOUR_WD_HIGH_TT=0;
				CAN_IC2_100[3].bits.Rx_6 = 0;
				CAN_IC2_100[3].bits.Rx_7 = 0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
				DID_TT_bit_data_set1.bits.Rx_4=0;
			}
		}
		/*1,2B,3BCOMPLETED,remaining do later*/
		else
		{
			FOUR_WD_HIGH_TT=0;
			FOUR_WD_LOW_TT=0;
			DID_TT_bit_data_set6.bits.Rx_3=0;
			DID_TT_bit_data_set1.bits.Rx_4=0;
		}
		DID_TT_bit_data_set1.bits.Rx_4=FOUR_WD_HIGH_TT;
		DID_TT_bit_data_set6.bits.Rx_3=FOUR_WD_LOW_TT;
	}
	else if(FEATURE_CODE_TRANSFER_MODE_CONFIG == 0)
	{
		FOUR_WD_HIGH_TT=0;
		FOUR_WD_LOW_TT=0;
		DID_TT_bit_data_set1.bits.Rx_4=FOUR_WD_HIGH_TT;
		DID_TT_bit_data_set6.bits.Rx_3=FOUR_WD_LOW_TT;
	}
	else
	{
		FOUR_WD_HIGH_TT=0;
		FOUR_WD_LOW_TT=0;
		DID_TT_bit_data_set1.bits.Rx_4=FOUR_WD_HIGH_TT;
		DID_TT_bit_data_set6.bits.Rx_3=FOUR_WD_LOW_TT;
	}

	/*ESP_OFF_TT  & ESP_MALFUNCTION_TT */
	if(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0)
	{
		if(ESC5_10_message_timeout_flag != 0)
		{
			ESP_MALFUNCTION_TT =0xfff;
			ESP_OFF_TT= 0x000;
		}
		else if(ESC5_10_message_timeout_flag==CLEAR)
		{
			two_bit_data.bits.Rx_1 =CAN_ESC5_10[3].bits.Rx_7;  /* ESP_OFF_TT*/
			two_bit_data.bits.Rx_2 =CAN_ESC5_10[3].bits.Rx_8;

			if(two_bit_data.byte == 1)
			{
				ESP_OFF_TT= 0x000;
				if(CAN_ESC5_10[1].bits.Rx_4==0)
				{
					ESP_MALFUNCTION_TT = 0x000;
				}
				if(CAN_ESC5_10[1].bits.Rx_4==1)
				{
					ESP_MALFUNCTION_TT = telltale_blink_flag_patt3;
				}
			}
			else if(two_bit_data.byte ==2)
			{
				ESP_OFF_TT=0xfff;
				ESP_MALFUNCTION_TT = 0x000;
			}
			else if(two_bit_data.byte == 0)
			{
				ESP_MALFUNCTION_TT =0xfff;
				ESP_OFF_TT= 0x000;
				/*ESP_MULFUNCTION telltale feedback*/
				if(read_SID[251]==1 || read_SID[155]==1)
				{
					CAN_IC2_100[4].bits.Rx_4=1;    //FEEDBK_ESC_MALFUNC

					if(read_SID[251]==1)
					{
						ESP_MALFUNCTION_TT_LOD=1;
					}
					else
					{
						ESP_MALFUNCTION_TT_LOD=0;
					}
					if(read_SID[155]==1)
					{
						ESP_MALFUNCTION_TT_LSD=1;
					}
					else
					{
						ESP_MALFUNCTION_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[4].bits.Rx_4=0;    //FEEDBK_ESC_MALFUNC
					ESP_MALFUNCTION_TT_LOD=0;
					ESP_MALFUNCTION_TT_LSD=0;
				}
			}
			else if(two_bit_data.byte == 3)
			{
				ESP_MALFUNCTION_TT =0xfff;
				ESP_OFF_TT=0xfff;
				/*ESP_MULFUNCTION telltale feedback*/
				if(read_SID[251]==1 || read_SID[155]==1)
				{
					CAN_IC2_100[4].bits.Rx_4=1;    //FEEDBK_ESC_MALFUNC

					if(read_SID[251]==1)
					{
						ESP_MALFUNCTION_TT_LOD=1;
					}
					else
					{
						ESP_MALFUNCTION_TT_LOD=0;
					}
					if(read_SID[155]==1)
					{
						ESP_MALFUNCTION_TT_LSD=1;
					}
					else
					{
						ESP_MALFUNCTION_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[4].bits.Rx_4=0;    //FEEDBK_ESC_MALFUNC
					ESP_MALFUNCTION_TT_LOD=0;
					ESP_MALFUNCTION_TT_LSD=0;
				}
			}

		}
		if( ESP_OFF_TT==0xfff)
		{
			DID_TT_bit_data_set3.bits.Rx_1=1;
		}
		else
		{
			DID_TT_bit_data_set3.bits.Rx_1=0;
		}

		if( ESP_MALFUNCTION_TT==0xfff)
		{
			DID_TT_bit_data_set3.bits.Rx_5=1;
		}
		else
		{
			DID_TT_bit_data_set3.bits.Rx_5=0;
		}

	}
	else
	{
		ESP_MALFUNCTION_TT =0x000;
		ESP_OFF_TT=0x000;
		CAN_IC2_100[4].bits.Rx_4=0;    //FEEDBK_ESC_MALFUNC
		ESP_MALFUNCTION_TT_LOD=0;
		ESP_MALFUNCTION_TT_LSD=0;
		DID_TT_bit_data_set3.bits.Rx_5=0;
		DID_TT_bit_data_set3.bits.Rx_1=0;
	}


	if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==0&& VAR_CODING_ANTILOCK_BRAKING_SYSTEM==1)||
			(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1&& VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0))
	{
		uint8_t t;  //check this t variable from SRD

		if(ESC2_10_message_timeout_flag != 0)
		{
			PARK_BRAKE_LOW_FLUID_EBD_TT = 0xfff;
		}
		else if(ESC2_10_message_timeout_flag ==0)
		{
#if 0
			if(CAN_ESC2_10[4].bits.Rx_4==0 &&(PARK_BRAKE_CPU == 1 ||PARK_BRAKE_CPU == 0)&&
					(LOW_BRAKE_FLUID_CPU == 1 || LOW_BRAKE_FLUID_CPU == 0))
			{
				PARK_BRAKE_LOW_FLUID_EBD_TT = 0xfff;
			}
#endif
			if(LOW_BRAKE_FLUID_CPU == 0)
			{
				LowBrakeFluidSt_Hw_Input = 1;
			}
			else
			{
				LowBrakeFluidSt_Hw_Input = 0;
			}

			if((CAN_ESC2_10[4].bits.Rx_4 == 0) || (PARK_BRAKE_CPU == 0))
			{
				PARK_BRAKE_LOW_FLUID_EBD_TT = 0xfff;
				/*PARK_BRAKE telltale feedback*/
				if(read_SID[258]==1 || read_SID[162]==1)
				{
					CAN_IC2_100[3].bits.Rx_1=1;    //FEEDBK_PARK_BRK
					if(read_SID[258]==1)
					{
						PARK_BRAKE_TT_LOD=1;
					}
					else
					{
						PARK_BRAKE_TT_LOD=0;
					}
					if(read_SID[162]==1)
					{
						PARK_BRAKE_TT_LSD=1;
					}
					else
					{
						PARK_BRAKE_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[3].bits.Rx_1=0;    //FEEDBK_PARK_BRK
					PARK_BRAKE_TT_LOD=0;
					PARK_BRAKE_TT_LSD=0;
				}
			}
			if((LOW_BRAKE_FLUID_CPU == 0))		//LOW_BRAKE_FLUID_CPU input is active low
			{
				//				LowBrakeFluidSt_Hw_Input = 1;
				Timer_ON_3sec = 1;

				if(LBF_3Sec_cntr_done == 0)//Counter_3sec_Flag
				{
					/*To avoid TT off for 3sec. if PARK_Break or EBD is on and LBF IP given
					//PARK_BRAKE_LOW_FLUID_EBD_TT =  0x000;
					*/
					CAN_IC2_100[3].bits.Rx_3=0;
				}
				else if(LBF_3Sec_cntr_done==1)    //Counter_3sec_Flag	LOW_BRAKE_FLUID_TT_IP=0
				{
					PARK_BRAKE_LOW_FLUID_EBD_TT = 0xfff;
					CAN_IC2_100[3].bits.Rx_3=1;
				}
				else
				{
					PARK_BRAKE_LOW_FLUID_EBD_TT =  0x000;
					Timer_ON_3sec=0;
				}
			}
			else if((CAN_ESC2_10[4].bits.Rx_4==1) && (PARK_BRAKE_CPU == 1) && (LOW_BRAKE_FLUID_CPU == 1))
			{
				//				LowBrakeFluidSt_Hw_Input = 0;
				PARK_BRAKE_LOW_FLUID_EBD_TT =  0x000;
				Timer_ON_3sec=0;
			}
			else
			{
				//				LowBrakeFluidSt_Hw_Input = 0;
				//PARK_BRAKE_LOW_FLUID_EBD_TT =  0x000;	commented to avoid TT off
				Timer_ON_3sec=0;
			}
		}
        if(PARK_BRAKE_LOW_FLUID_EBD_TT ==  0xfff)
        {
        	DID_TT_bit_data_set1.bits.Rx_8=1;
        }
        else
        {
        	DID_TT_bit_data_set1.bits.Rx_8=0;
        }
	}
	else
	{
		PARK_BRAKE_LOW_FLUID_EBD_TT =  0x000;
		Timer_ON_3sec=0;
		CAN_IC2_100[3].bits.Rx_1=0;    //FEEDBK_PARK_BRK
		PARK_BRAKE_TT_LOD=0;
		PARK_BRAKE_TT_LSD=0;
		DID_TT_bit_data_set1.bits.Rx_8=0;
	}


	//Low Brake Fluid

	if((LOW_BRAKE_FLUID_CPU == 0))
	{
		LBF_3Sec_cntr_start=1;
	}
	else
	{
		LBF_3Sec_cntr_start=0;
	}


#ifdef GRAYSCALE_TFT
	if(Diag_RW_LowBrakeFluid_Chime_Enable==1)
	{
		if((LOW_BRAKE_FLUID_CPU==0) && (LBF_3Sec_cntr_done == 1))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x4 || Sound_Bit == 0x200) && Buzz_Low_Brk_fld_compl == 0)
			{
				Sound_Bit = 0x4;
				Buzz_Low_Brk_fld=1;
			}
			else
			{
				Buzz_Low_Brk_fld=0;
			}

		}
		else if(Buzz_Low_Brk_fld_compl == 1)
		{
			if(Sound_Bit == 0x4)
			{
				Sound_Bit = 0x0;
			}
			Buzz_Low_Brk_fld=0;
			Buzz_Low_Brk_fld_compl=0;
			Buzz_Low_Brk_fld_cntr=0;
		}
	}
	/*Park Brake*/
	if(Diag_RW_ParkBrake_Chime_Enable==1)
	{
		if((PARK_BRAKE_CPU == 0) && (vehicle_moving_state == 1))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x8 || Sound_Bit == 0x200) && Buzz_Park_brake_compl == 0)//Buzz_Low_Brk_fld_compl
			{
				Sound_Bit = 0x8;
				Buzz_Park_brake=1;
			}
			else if(Buzz_Park_brake_compl == 1)
			{
				if(Sound_Bit == 0x8)
				{
					Sound_Bit = 0x0;
				}
			}
		}
		else if(((Buzz_ON == 0) && (Buzz_Park_brake == 1))||(PARK_BRAKE_CPU == 1))
		{
			if(Sound_Bit == 0x8)
			{
				Sound_Bit = 0x0;
				r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);//r
			}
			Buzz_Park_brake=0;
			Buzz_Park_brake_compl=0;
			Buzz_Park_brake_cntr=0;
		}
		else if(vehicle_moving_state == 0)
		{
			if(Sound_Bit == 0x8)
			{
				Sound_Bit = 0x0;
				r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);//r
			}
			Buzz_Park_brake_compl=0;//to reactivate Chime
			Buzz_Park_brake=0;
			Buzz_Park_brake_cntr=0;
		}
	}
#endif

#ifdef COLOR_TFT	/*Low brake fluid chime*/
	if(Diag_RW_LowBrakeFluid_Chime_Enable==1)
	{
		if((LOW_BRAKE_FLUID_CPU==0) && (LBF_3Sec_cntr_done == 1)&&(Lowbrakefluid_ongoing == 0)&&(chime_start1 == 0)&&(cycle_Lowbrakefluid_completed == 0))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x04 || Sound_Bit == 0x200) && cycle_Lowbrakefluid_completed == 0)
			{
				Sound_Bit = 0x04;				/*0000 0000 0100 - Low Brake Fluid sound bit*/
				//		SoundPCM_RPAS_St3();
				Lowbrakefluid_ongoing = 1;
				chime_start1 = 1;
				Lowbrakefluid_missed = 0;
			}
			else
			{
				Lowbrakefluid_ongoing = 0;
				Lowbrakefluid_missed = 0;
			}
		}
		else if(LOW_BRAKE_FLUID_CPU!=0)
		{
			if(Sound_Bit == 0x04)
			{
				Sound_Bit = 0x0;				/*Sound bit CLEAR*/
			}
			Lowbrakefluid_ongoing = 0;
			cycle_Lowbrakefluid_completed = 0;
			Lowbrakefluid_missed = 1;
		}
	}
	/*else if(chime_start1 == 0)
	{
		Lowbrakefluid_ongoing = 0;
	}*/
	/*Park Brake chime*/
	if(Diag_RW_ParkBrake_Chime_Enable==1)
	{
		if((PARK_BRAKE_CPU == 0) && (vehicle_moving_state == 1) && (Parkbrake_ongoing == 0)&&(chime_start1 == 0)&&(cycle_parkbrake_completed == 0))
		{
			if((Sound_Bit == 0x0 || Sound_Bit == 0x08 || Sound_Bit == 0x200) && cycle_parkbrake_completed == 0)
			{
				Sound_Bit = 0x08;				/*0000 0000 1000 - Park Brake sound bit*/
				//		SoundPCM_message();
				chime_start1 = 1;
				Parkbrake_ongoing = 1;
				parkbrake_missed = 0;
			}
		}
		else if((PARK_BRAKE_CPU != 0)||(vehicle_moving_state == 0))
		{
			if(Sound_Bit == 0x08)
			{
				Sound_Bit = 0x0;				/*Sound bit CLEAR*/
			}
			Parkbrake_ongoing = 0;
			cycle_parkbrake_completed = 0;
			parkbrake_missed = 1;
		}
	}
	/*else if(chime_start1 == 0)
	{
		Parkbrake_ongoing = 0;
	}*/
#endif


	/*feed_back_msg_park_brake*/

	if(PARK_BRAKE_CPU == 0)
	{
		CAN_IC1_100[3].bits.Rx_7=1;
	}
	else if(PARK_BRAKE_CPU == 1)
	{
		CAN_IC1_100[3].bits.Rx_7=0;
	}
#if 0
	if(LOW_BRAKE_FLUID_CPU == 0 && Counter_3sec_Flag==0)
	{
		CAN_IC2_100[3].bits.Rx_3=0;
	}
	else if(LOW_BRAKE_FLUID_CPU == 0 && Counter_3sec_Flag==1)
	{
		CAN_IC2_100[3].bits.Rx_3=1;
	}
	else
#endif
		if(LOW_BRAKE_FLUID_CPU ==1)
		{
			CAN_IC2_100[3].bits.Rx_3=0;
		}

	/*OVER_SPEED_TT*/
	//	if(FEATURE_CODE_OVERSPEED_TT_CONFIG==1 && FEATURE_CODE_SPEED_INPUT==0)
	if(FEATURE_CODE_SPEED_INPUT==0)
	{
		/*************************Overspeed Chime conditions*************************************************
		Diag RW OverSpeed Chime Enable (0,1)
		DiagRWOverSpeedLevel1Val  (Default:80 kmph) -min:40 max:100
		DiagRWOverSpeedLevel2Val  (Default:120 kmph) -min:101 max:220
		DiagRWOverSpeedLevel1Type (0-Off ,1-Play once,2- repeated every n1 sec) -Def:1 min:0 max:2
		DiagRWOverSpeedLevel1RepeatationInterval  ( “n1”) -Def:60sec min:0sec max:255sec
		DiagRWOverSpeedLevel2Type (0-Off ,1-Play once,2- repeated every n2 sec) -Def:2 min:0 max:2
		DiagRWOverSpeedLevel2RepeatationInterval   ( “n2”) -Def:60sec min:0sec max:255sec
		DiagRWOverSpeedHystresis (Default : 5%) -Def:5% min:0% max:30%
		 *****************************************************************************************************/
		OverSpeedLevel1Val	 	 = DID_OVERSPEED_0x04F7_arr[0];
		OverSpeedLevel2Val   	 = DID_OVERSPEED_0x04F7_arr[1];
		OverSpeedLevel1Type  	 = DID_OVERSPEED_0x04F7_arr[2];
		OverSpeedLv1RepeatIntrval= DID_OVERSPEED_0x04F7_arr[3];
		OverSpeedLevel2Type		 = DID_OVERSPEED_0x04F7_arr[4];
		OverSpeedLv2RepeatIntrval= DID_OVERSPEED_0x04F7_arr[5];
		OverSpeedHystresis		 = DID_OVERSPEED_0x04F7_arr[6];
		//		if(FEATURE_CODE_OVERSPEED_TT_CONFIG==1)
		//		{
		if(ESC2_10_message_timeout_flag ==CLEAR)
		{
			uint32_t   KMMPer_hr_value = 0;
			uint32_t   overspeed2_value = 0,overspeed1_value = 0;
			uint32_t   overspeed2_decresing = 0,overspeed1_decresing = 0;
			uint32_t   overspeed2_hyst=0,overspeed1_hyst = 0;
			uint32_t   Speed_on_CAN = 0;
			down_scale.w=0;

			overspeed2_value = OVERSPEED_LVL2_VALUE;// * 64;							/*Level2 Increasing trend*/
			overspeed2_hyst = (OVERSPEED_LVL2_VALUE*OVERSPEED_HYSTERESIS)/100;
			overspeed2_decresing = (OVERSPEED_LVL2_VALUE - overspeed2_hyst);// * 64;	/*Level2 Decreasing trend*/

			overspeed1_value = OVERSPEED_LVL1_VALUE;// * 64;							/*Level1 Increasing trend*/
			overspeed1_hyst = (OVERSPEED_LVL1_VALUE*OVERSPEED_HYSTERESIS)/100;
			overspeed1_decresing = (OVERSPEED_LVL1_VALUE - overspeed1_hyst);// * 64;	/*Level1 Decreasing trend*/

			//			KMMPer_hr_value=CAN_ESC2_10[1].byte;
			//			KMMPer_hr_value <<= 8;
			//			KMMPer_hr_value |= CAN_ESC2_10[0].byte;

			down_scale.b[0] = CAN_ESC2_10[0].byte;
			down_scale.b[1] = CAN_ESC2_10[1].byte;
			Speed_on_CAN = (down_scale.w * 0.015625);


			KMMPer_hr_value = display_speedometer_speed;//r
//			if((FEATURE_CODE_OVERSPEED_TT_CONFIG==1)&&(((KMMPer_hr_value-Speed_on_CAN)<5)||(/*(Speed_on_CAN >=184)&&*/(down_scale.w < 0x4B01))))
			if((FEATURE_CODE_OVERSPEED_TT_CONFIG==1)&&(down_scale.w < 0x4B01))

			{
				if((KMMPer_hr_value >= overspeed2_value)&&((Speed_on_CAN+3) >= overspeed2_value)&&(Speed_Invalid_Default == 0)&&(vehicle_moving_state == 1))//(KMMPer_hr_value < 0x4B01)
				{
					OVER_SPEED_TT = telltale_blink_bool3;
					Over_speed_flag = 1;
					OS_Flag=1;
					Buzz_OS_compl = 0;	//To avoid OS buzz off in 120kmph speed
					DID_TT_bit_data_set5.bits.Rx_7=1;
				}
				else if((KMMPer_hr_value > overspeed2_decresing) && (KMMPer_hr_value < overspeed2_value) && ((Speed_on_CAN+3) > overspeed2_decresing) && (Speed_on_CAN < overspeed2_value) && (OS_Flag == 1) && (Buzz_OS_L2 == 1))
				{
					OVER_SPEED_TT = telltale_blink_bool3;
					Over_speed_flag = 1;
					OS_Flag=1;
					Buzz_OS_compl = 0;	//To avoid OS buzz off in 120kmph speed
					DID_TT_bit_data_set5.bits.Rx_7=1;
				}
				else if((KMMPer_hr_value <= overspeed2_decresing) && (OS_Flag == 1))
				{
					OVER_SPEED_TT = 0;
					Over_speed_flag = 0;
					OS_Flag=0;
					DID_TT_bit_data_set5.bits.Rx_7=0;
				}
				else
				{
				//r	Over_speed_flag = 0;
					OVER_SPEED_TT = 0;
				}
			}
			else if((FEATURE_CODE_OVERSPEED_TT_CONFIG==0)&&(down_scale.w < 0x4B01))
			{
				if((KMMPer_hr_value >= overspeed2_value)&&((Speed_on_CAN+3) >= overspeed2_value)&&(Speed_Invalid_Default == 0)&&(vehicle_moving_state == 1))//(KMMPer_hr_value < 0x4B01)
				{
					Over_speed_flag = 1;
					OS_Flag=1;
					Buzz_OS_compl = 0;	//To avoid OS buzz off in 120kmph speed
				}
				else if((KMMPer_hr_value > overspeed2_decresing) && (KMMPer_hr_value < overspeed2_value) && ((Speed_on_CAN+3) > overspeed2_decresing) && (Speed_on_CAN < overspeed2_value) && (OS_Flag == 1) && (Buzz_OS_L2 == 1))
				{
					Over_speed_flag = 1;
					OS_Flag=1;
					Buzz_OS_compl = 0;	//To avoid OS buzz off in 120kmph speed
				}
				else if((KMMPer_hr_value <= overspeed2_decresing) && (OS_Flag == 1))
				{
					Over_speed_flag = 0;
					OS_Flag=0;
				}
				else
				{
					//r	Over_speed_flag = 0;
					OVER_SPEED_TT = 0;
				}
				OVER_SPEED_TT = 0;		//TT off as feature code is disable
			}
			else
			{
				Over_speed_flag = 0;
				OVER_SPEED_TT = 0;
				DID_TT_bit_data_set5.bits.Rx_7=0;
			}
			/*************************Overspeed Chime conditions***************************************/
			if((Diag_RW_OverSpeed_Chime_Enable == 1)&&(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
//			if((Diag_RW_OverSpeed_Chime_Enable == 1)&&(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1)&&(((KMMPer_hr_value-Speed_on_CAN)<5)||((Speed_on_CAN >=184)&&(down_scale.w < 0x4B01))))
			{
				if((KMMPer_hr_value >= overspeed1_value)&&(KMMPer_hr_value < overspeed2_value)&&(((Speed_on_CAN+3) >= overspeed1_value)&&(Speed_on_CAN < overspeed2_value))&&(Speed_Invalid_Default == 0)&&(level2_ongoing == 0))//(KMMPer_hr_value < 0x4B01)
				{
					if(OverSpeedLevel1Type)
					{
						if((Sound_Bit == 0x0) || (Sound_Bit == 0x2000) || (Sound_Bit == 0x200))
						{
							OverSpeedLv1repeatcnt = 1;
							OverSpeedLv2repeatcnt = 0;
							level1_ongoing = 1;					/*Level 1 chime ON*/
							level2_ongoing = 0;
							overspeedlevel2_cntr = 0;

							//Sound_Bit = 0x2000;
							Buzz_OS_L1=1;
							Buzz_OS_L2=0;
						}
				/*****************TO AVOID OVERSPEED TO STOP SEATBELT******************/
						else if(Sound_Bit == 0x80)
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;

							if(Buzz_ON == 0)
							{
								Buzz_OS_L1=0;
								Buzz_OS_L2=0;
								Buzz_OS_compl=0;
							}
						}
				/************************************************************************/
					}
					OS_L2_Start = 0;
				}
//				else if((KMMPer_hr_value >= overspeed2_value)&&(Speed_Invalid_Default == 0))
				else if((KMMPer_hr_value >= overspeed2_value)&&(Speed_Invalid_Default == 0)&&(vehicle_moving_state == 1)&&((Speed_on_CAN+3) >= overspeed2_value))
				{
					/*OverSpeedLv2repeatcnt = 1;
					OverSpeedLv1repeatcnt = 0;
					level1_ongoing = 0;					Level 2 chime ON
					level2_ongoing = 1;
					overspeedlevel1_cntr = 0;*/
					if(OverSpeedLevel2Type)
					{
						if((Sound_Bit == 0x0) || (Sound_Bit == 0x2000)||(Sound_Bit == 0x200))
						{
							if(Buzz_OS_L1==1)
							{
								Buzz_OS_compl=0;
							}
							Buzz_OS_L1=0;
							Buzz_OS_L2=1;
							Sound_Bit = 0x2000;
							/*******W501_High end*************/
							OverSpeedLv2repeatcnt = 1;
							OverSpeedLv1repeatcnt = 0;
							level1_ongoing = 0;					/*Level 2 chime ON*/
							level2_ongoing = 1;
							overspeedlevel1_cntr = 0;
							/************************/
						}
						else
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;
						}
					}
					OS_L2_Start = 1;

				}
//				else if((KMMPer_hr_value > overspeed2_decresing)&&(Speed_Invalid_Default == 0)&&(level2_ongoing == 1))
				else if((Speed_on_CAN > overspeed2_decresing)&&(KMMPer_hr_value > overspeed2_decresing)&&(Speed_Invalid_Default == 0)&&(level2_ongoing == 1)&&(vehicle_moving_state == 1))
				{
					/*OverSpeedLv2repeatcnt = 1;
					OverSpeedLv1repeatcnt = 0;
					level1_ongoing = 0;
					level2_ongoing = 1;					//Level 2 chime ON
					overspeedlevel1_cntr = 0;*/
					if(OverSpeedLevel2Type)
					{
						if((Sound_Bit == 0x0) || (Sound_Bit == 0x2000)||(Sound_Bit == 0x200))
						{
							if(Buzz_OS_L1==1)
							{
								Buzz_OS_compl=0;
							}
							Buzz_OS_L1=0;
							Buzz_OS_L2=1;
							Sound_Bit = 0x2000;
							/*******W501_High end*************/
							OverSpeedLv2repeatcnt = 1;
							OverSpeedLv1repeatcnt = 0;
							level1_ongoing = 0;
							level2_ongoing = 1;					//Level 2 chime ON
							overspeedlevel1_cntr = 0;
							/************************/
						}
						else
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;
						}
					}
					OS_L2_Start = 1;
				}
				else if((KMMPer_hr_value > overspeed1_decresing)&&(KMMPer_hr_value <= overspeed2_decresing)&&(Speed_on_CAN > overspeed1_decresing)&&(Speed_on_CAN <= overspeed2_decresing)&&(Speed_Invalid_Default == 0)&&((level1_ongoing == 1)||level2_ongoing == 1))
				{
					if(OverSpeedLevel1Type)
					{
						if((Sound_Bit == 0x0 || Sound_Bit == 0x2000) || (Sound_Bit == 0x200))
						{
							OverSpeedLv1repeatcnt = 1;
							OverSpeedLv2repeatcnt = 0;
							level1_ongoing = 1;				   	/*Level 1 chime ON*/
							level2_ongoing = 0;
							overspeedlevel2_cntr = 0;
							//Sound_Bit = 0x2000;
							Buzz_OS_L1=1;
							Buzz_OS_L2=0;
						}
				/*****************TO AVOID OVERSPEED TO STOP SEATBELT******************/
						else if(Sound_Bit == 0x80)
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;

							if(Buzz_ON == 0)
							{
								Buzz_OS_L1=0;
								Buzz_OS_L2=0;
								Buzz_OS_compl=0;
							}
						}
				/************************************************************************/
					}
					OS_L2_Start = 0;
				}
//				else if((KMMPer_hr_value <= overspeed1_decresing)||(Speed_Invalid_Default == 1))//(KMMPer_hr_value >= 0x4B01)
				else if((KMMPer_hr_value <= overspeed1_decresing)||(Speed_Invalid_Default == 1)||(vehicle_moving_state == 0))//(KMMPer_hr_value >= 0x4B01)
				{
					level1_ongoing = 0;					/*Level 1 chime OFF*/
					level2_ongoing = 0;					/*Level 2 chime OFF*/
					OverSpeedLv1repeatcnt = 0;
					OverSpeedLv2repeatcnt = 0;
					overspeedlevel2_cntr = 0;
					overspeedlevel1_cntr = 0;

					if(Buzz_ON == 0)
					{
						Buzz_OS_L1=0;
						Buzz_OS_L2=0;
						Buzz_OS_compl=0;
					}

					if((Sound_Bit == 0x0 || Sound_Bit == 0x2000) || (Sound_Bit == 0x200))
					{
						Sound_Bit = 0x0;
					}
					OS_L2_Start = 0;
				}
			}
			else
			{
				level1_ongoing = 0;						/*Level 1 chime OFF*/
				level2_ongoing = 0;						/*Level 2 chime OFF*/
				OverSpeedLv1repeatcnt = 0;
				OverSpeedLv2repeatcnt = 0;
				overspeedlevel2_cntr = 0;
				overspeedlevel1_cntr = 0;

				if(Buzz_ON == 0)
				{
					Buzz_OS_L1=0;
					Buzz_OS_L2=0;
					Buzz_OS_compl=0;
				}
				if(Sound_Bit == 0x0 || Sound_Bit == 0x2000)
				{
					Sound_Bit = 0x0;
				}
			}
			/******************************************************************************************/
		}
		else
		{
			/*Turn off OS buzzer in message missing condition*/
			level1_ongoing = 0;						/*Level 1 chime OFF*/
			level2_ongoing = 0;						/*Level 2 chime OFF*/
			OverSpeedLv1repeatcnt = 0;
			OverSpeedLv2repeatcnt = 0;
			overspeedlevel2_cntr = 0;
			overspeedlevel1_cntr = 0;
			OS_L2_Start = 0;

			if(Buzz_ON == 0)
			{
				Buzz_OS_L1=0;
				Buzz_OS_L2=0;
				Buzz_OS_compl=0;
			}
			if(Sound_Bit == 0x0 || Sound_Bit == 0x2000)
			{
				Sound_Bit = 0x0;
			}
			/*Turn off OS buzzer in message missing condition*/
			Over_speed_flag = 0;
			OVER_SPEED_TT = 0;
		}
	}

	//	else if(FEATURE_CODE_OVERSPEED_TT_CONFIG==1 && FEATURE_CODE_SPEED_INPUT==1)
	else if(FEATURE_CODE_SPEED_INPUT==1)
	{
		/*************************Overspeed Chime conditions*************************************************
		Diag RW OverSpeed Chime Enable (0,1)
		DiagRWOverSpeedLevel1Val  (Default:80 kmph) -min:40 max:100
		DiagRWOverSpeedLevel2Val  (Default:120 kmph) -min:101 max:220
		DiagRWOverSpeedLevel1Type (0-Off ,1-Play once,2- repeated every n1 sec) -Def:1 min:0 max:2
		DiagRWOverSpeedLevel1RepeatationInterval  ( “n1”) -Def:60sec min:0sec max:255sec
		DiagRWOverSpeedLevel2Type (0-Off ,1-Play once,2- repeated every n2 sec) -Def:2 min:0 max:2
		DiagRWOverSpeedLevel2RepeatationInterval   ( “n2”) -Def:60sec min:0sec max:255sec
		DiagRWOverSpeedHystresis (Default : 5%) -Def:5% min:0% max:30%
		 *****************************************************************************************************/
		OverSpeedLevel1Val	 	 = DID_OVERSPEED_0x04F7_arr[0];
		OverSpeedLevel2Val   	 = DID_OVERSPEED_0x04F7_arr[1];
		OverSpeedLevel1Type  	 = DID_OVERSPEED_0x04F7_arr[2];
		OverSpeedLv1RepeatIntrval= DID_OVERSPEED_0x04F7_arr[3];
		OverSpeedLevel2Type		 = DID_OVERSPEED_0x04F7_arr[4];
		OverSpeedLv2RepeatIntrval= DID_OVERSPEED_0x04F7_arr[5];
		OverSpeedHystresis		 = DID_OVERSPEED_0x04F7_arr[6];

		if(ESC12_10_message_timeout_flag == CLEAR)
		{
			uint32_t   KMMPer_hr_value = 0;
			uint32_t   overspeed2_value = 0,overspeed1_value = 0;
			uint32_t   overspeed2_decresing = 0,overspeed1_decresing = 0;
			uint32_t   overspeed2_hyst=0,overspeed1_hyst = 0;
			uint32_t   Speed_on_CAN = 0;
			down_scale.w=0;

			overspeed2_value = OVERSPEED_LVL2_VALUE;// * 64;							/*Level2 Increasing trend*/
			overspeed2_hyst = (OVERSPEED_LVL2_VALUE*OVERSPEED_HYSTERESIS)/100;
			overspeed2_decresing = (OVERSPEED_LVL2_VALUE - overspeed2_hyst);// * 64;	/*Level2 Decreasing trend*/

			overspeed1_value = OVERSPEED_LVL1_VALUE;// * 64;							/*Level1 Increasing trend*/
			overspeed1_hyst = (OVERSPEED_LVL1_VALUE*OVERSPEED_HYSTERESIS)/100;
			overspeed1_decresing = (OVERSPEED_LVL1_VALUE - overspeed1_hyst);// * 64;	/*Level1 Decreasing trend*/
			//			KMMPer_hr_value=CAN_ESC12_10[3].byte;
			//			KMMPer_hr_value <<= 8;
			//			KMMPer_hr_value |= CAN_ESC12_10[2].byte;
			down_scale.b[0] = CAN_ESC12_10[2].byte;
			down_scale.b[1] = CAN_ESC12_10[3].byte;
			Speed_on_CAN = (down_scale.w * 0.015625);


			KMMPer_hr_value = display_speedometer_speed;//r
//			if((FEATURE_CODE_OVERSPEED_TT_CONFIG==1)&&(((KMMPer_hr_value-Speed_on_CAN)<5)||(/*(Speed_on_CAN >=184)&&*/(down_scale.w < 0x4B01))))
			if((FEATURE_CODE_OVERSPEED_TT_CONFIG==1)&&(down_scale.w < 0x4B01))
			{
				if((KMMPer_hr_value >= overspeed2_value)&&((Speed_on_CAN+3) >= overspeed2_value)&&(Speed_Invalid_Default == 0)&&(vehicle_moving_state == 1))//(KMMPer_hr_value < 0x4B01)
				{
					OVER_SPEED_TT = telltale_blink_bool3;
					Over_speed_flag = 1;
					OS_Flag=1;
					DID_TT_bit_data_set5.bits.Rx_7=1;
				}
				else if((KMMPer_hr_value > overspeed2_decresing) && (KMMPer_hr_value < overspeed2_value) && ((Speed_on_CAN+3) > overspeed2_decresing) && (Speed_on_CAN < overspeed2_value) && (OS_Flag == 1) && (Buzz_OS_L2 == 1))
				{
					OVER_SPEED_TT = telltale_blink_bool3;
					Over_speed_flag = 1;
					OS_Flag=1;
					DID_TT_bit_data_set5.bits.Rx_7=1;
				}
				else if((KMMPer_hr_value <= overspeed2_decresing) && (OS_Flag == 1))
				{
					OVER_SPEED_TT = 0;
					Over_speed_flag = 0;
					OS_Flag=0;
					DID_TT_bit_data_set5.bits.Rx_7=0;
				}
				else
				{
					//r	Over_speed_flag = 0;
					OVER_SPEED_TT = 0;
					DID_TT_bit_data_set5.bits.Rx_7=0;
				}
			}
			else if((FEATURE_CODE_OVERSPEED_TT_CONFIG==0)&&(down_scale.w < 0x4B01))
			{
				if((KMMPer_hr_value >= overspeed2_value)&&((Speed_on_CAN+3) >= overspeed2_value)&&(Speed_Invalid_Default == 0)&&(vehicle_moving_state == 1))//(KMMPer_hr_value < 0x4B01)
				{
					Over_speed_flag = 1;
					OS_Flag=1;
					Buzz_OS_compl = 0;	//To avoid OS buzz off in 120kmph speed
				}
				else if((KMMPer_hr_value > overspeed2_decresing) && (KMMPer_hr_value < overspeed2_value) && ((Speed_on_CAN+3) > overspeed2_decresing) && (Speed_on_CAN < overspeed2_value) && (OS_Flag == 1) && (Buzz_OS_L2 == 1))
				{
					Over_speed_flag = 1;
					OS_Flag=1;
					Buzz_OS_compl = 0;	//To avoid OS buzz off in 120kmph speed
				}
				else if((KMMPer_hr_value <= overspeed2_decresing) && (OS_Flag == 1))
				{
					Over_speed_flag = 0;
					OS_Flag=0;
				}
				else
				{
					//r	Over_speed_flag = 0;
					OVER_SPEED_TT = 0;
				}
				OVER_SPEED_TT = 0;		//TT off as feature code is disable
			}
			else
			{
				Over_speed_flag = 0;
				OVER_SPEED_TT = 0;
				DID_TT_bit_data_set5.bits.Rx_7=0;
			}
			/*************************Overspeed Chime conditions***************************************/
			if((Diag_RW_OverSpeed_Chime_Enable == 1)&&(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
//			if((Diag_RW_OverSpeed_Chime_Enable == 1)&&(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1)&&(((KMMPer_hr_value-Speed_on_CAN)<5)||((Speed_on_CAN >=184)&&(down_scale.w < 0x4B01))))
			{
				if((KMMPer_hr_value >= overspeed1_value)&&(KMMPer_hr_value < overspeed2_value)&&(((Speed_on_CAN+3) >= overspeed1_value)&&(Speed_on_CAN < overspeed2_value))&&(Speed_Invalid_Default == 0)&&(level2_ongoing == 0))//(KMMPer_hr_value < 0x4B01)
				{
					if(OverSpeedLevel1Type)
					{
						if((Sound_Bit == 0x0) || (Sound_Bit == 0x2000) || (Sound_Bit == 0x200))
						{
							OverSpeedLv1repeatcnt = 1;
							OverSpeedLv2repeatcnt = 0;
							level1_ongoing = 1;					/*Level 1 chime ON*/
							level2_ongoing = 0;
							overspeedlevel2_cntr = 0;


							Buzz_OS_L1=1;
							Buzz_OS_L2=0;
						}
				/*****************TO AVOID OVERSPEED TO STOP SEATBELT******************/
						else if(Sound_Bit == 0x80)
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;

							if(Buzz_ON == 0)
							{
								Buzz_OS_L1=0;
								Buzz_OS_L2=0;
								Buzz_OS_compl=0;
							}
						}
				/************************************************************************/
					}
					OS_L2_Start = 0;
				}
//				else if((KMMPer_hr_value >= overspeed2_value)&&(Speed_Invalid_Default == 0))
				else if((KMMPer_hr_value >= overspeed2_value)&&(Speed_Invalid_Default == 0)&&(vehicle_moving_state == 1)&&((Speed_on_CAN+3) >= overspeed2_value))
				{
					/*OverSpeedLv2repeatcnt = 1;
					OverSpeedLv1repeatcnt = 0;
					level1_ongoing = 0;					//Level 2 chime ON
					level2_ongoing = 1;
					overspeedlevel1_cntr = 0;*/
					OS_L2_Start = 1;
					if(OverSpeedLevel2Type)
					{
						if((Sound_Bit == 0x0) || (Sound_Bit == 0x2000)||(Sound_Bit == 0x200))
						{
							if(Buzz_OS_L1==1)
							{
								Buzz_OS_compl=0;
							}
							Buzz_OS_L1=0;
							Buzz_OS_L2=1;
							Sound_Bit = 0x2000;
							/*******W501_High end*************/
							OverSpeedLv2repeatcnt = 1;
							OverSpeedLv1repeatcnt = 0;
							level1_ongoing = 0;					/*Level 2 chime ON*/
							level2_ongoing = 1;
							overspeedlevel1_cntr = 0;
							/************************/
						}
						else
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;
						}
					}

				}
//				else if((KMMPer_hr_value > overspeed2_decresing)&&(Speed_Invalid_Default == 0)&&(level2_ongoing == 1))
				else if((Speed_on_CAN > overspeed2_decresing)&&(KMMPer_hr_value > overspeed2_decresing)&&(Speed_Invalid_Default == 0)&&(level2_ongoing == 1)&&(vehicle_moving_state == 1))
				{
					/*OverSpeedLv2repeatcnt = 1;
					OverSpeedLv1repeatcnt = 0;
					level1_ongoing = 0;
					level2_ongoing = 1;					//Level 2 chime ON
					overspeedlevel1_cntr = 0;*/
					OS_L2_Start = 1;
					if(OverSpeedLevel2Type)
					{
						if((Sound_Bit == 0x0) || (Sound_Bit == 0x2000)||(Sound_Bit == 0x200))
						{
							if(Buzz_OS_L1==1)
							{
								Buzz_OS_compl=0;
							}
							Buzz_OS_L1=0;
							Buzz_OS_L2=1;
							Sound_Bit = 0x2000;
							/*******W501_High end*************/
							OverSpeedLv2repeatcnt = 1;
							OverSpeedLv1repeatcnt = 0;
							level1_ongoing = 0;
							level2_ongoing = 1;					//Level 2 chime ON
							overspeedlevel1_cntr = 0;
							/************************/
						}
						else
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;
						}
					}

				}
				else if((KMMPer_hr_value > overspeed1_decresing)&&(KMMPer_hr_value <= overspeed2_decresing)&&(Speed_on_CAN > overspeed1_decresing)&&(Speed_on_CAN <= overspeed2_decresing)&&(Speed_Invalid_Default == 0)&&((level1_ongoing == 1)||level2_ongoing == 1))
				{
					if(OverSpeedLevel1Type)
					{
						if((Sound_Bit == 0x0 || Sound_Bit == 0x2000) || (Sound_Bit == 0x200))
						{
							OverSpeedLv1repeatcnt = 1;
							OverSpeedLv2repeatcnt = 0;
							level1_ongoing = 1;				   	/*Level 1 chime ON*/
							level2_ongoing = 0;
							overspeedlevel2_cntr = 0;

							Buzz_OS_L1=1;
							Buzz_OS_L2=0;
						}
				/*****************TO AVOID OVERSPEED TO STOP SEATBELT******************/
						else if(Sound_Bit == 0x80)
						{
							level1_ongoing = 0;					/*Level 1 chime OFF*/
							level2_ongoing = 0;					/*Level 2 chime OFF*/
							OverSpeedLv1repeatcnt = 0;
							OverSpeedLv2repeatcnt = 0;
							overspeedlevel2_cntr = 0;
							overspeedlevel1_cntr = 0;

							if(Buzz_ON == 0)
							{
								Buzz_OS_L1=0;
								Buzz_OS_L2=0;
								Buzz_OS_compl=0;
							}
						}
				/************************************************************************/
					}
					OS_L2_Start = 0;
				}
//				else if((KMMPer_hr_value <= overspeed1_decresing)||(Speed_Invalid_Default == 1))//(KMMPer_hr_value >= 0x4B01)
				else if((KMMPer_hr_value <= overspeed1_decresing)||(Speed_Invalid_Default == 1)||(vehicle_moving_state == 0))//(KMMPer_hr_value >= 0x4B01)
				{
					level1_ongoing = 0;					/*Level 1 chime OFF*/
					level2_ongoing = 0;					/*Level 2 chime OFF*/
					OverSpeedLv1repeatcnt = 0;
					OverSpeedLv2repeatcnt = 0;
					overspeedlevel2_cntr = 0;
					overspeedlevel1_cntr = 0;
					OS_L2_Start = 0;
					if(Buzz_ON == 0)
					{
						Buzz_OS_L1=0;
						Buzz_OS_L2=0;
						Buzz_OS_compl=0;
					}

					if((Sound_Bit == 0x0 || Sound_Bit == 0x2000) || (Sound_Bit == 0x200))
					{
						Sound_Bit = 0x0;
					}
				}
			}
			else
			{
				level1_ongoing = 0;						/*Level 1 chime OFF*/
				level2_ongoing = 0;						/*Level 2 chime OFF*/
				OverSpeedLv1repeatcnt = 0;
				OverSpeedLv2repeatcnt = 0;
				overspeedlevel2_cntr = 0;
				overspeedlevel1_cntr = 0;

				if(Buzz_ON == 0)
				{
					Buzz_OS_L1=0;
					Buzz_OS_L2=0;
					Buzz_OS_compl=0;
				}
				if(Sound_Bit == 0x0 || Sound_Bit == 0x2000)
				{
					Sound_Bit = 0x0;
				}
			}
			/******************************************************************************************/
		}
		else
		{
			/*Turn off OS buzzer in message missing condition*/
			level1_ongoing = 0;						/*Level 1 chime OFF*/
			level2_ongoing = 0;						/*Level 2 chime OFF*/
			OverSpeedLv1repeatcnt = 0;
			OverSpeedLv2repeatcnt = 0;
			overspeedlevel2_cntr = 0;
			overspeedlevel1_cntr = 0;
			OS_L2_Start = 0;

			if(Buzz_ON == 0)
			{
				Buzz_OS_L1=0;
				Buzz_OS_L2=0;
				Buzz_OS_compl=0;
			}
			if(Sound_Bit == 0x0 || Sound_Bit == 0x2000)
			{
				Sound_Bit = 0x0;
			}
			/*Turn off OS buzzer in message missing condition*/
			Over_speed_flag = 0;
			OVER_SPEED_TT = 0;
		}
	}
	else
	{
		OVER_SPEED_TT = 0;
		Over_speed_flag = 0;
	}


	/*HHC_MALFUNCTION_TT  & HDC_TT */


	if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0))
	{
		if(ESC2_10_message_timeout_flag == 0)
		{
			/*HHC TT*/
			if(CAN_ESC2_10[5].bits.Rx_1==0)
			{
				HHC_MALFUNCTION_TT = 0xfff;
			}
			else if(CAN_ESC2_10[5].bits.Rx_1==1)
			{
				HHC_MALFUNCTION_TT =  0x000;
			}
			else
			{
				HHC_MALFUNCTION_TT =  0x000;
			}

			/*HDC TT*/
			if(CAN_ESC2_10[4].bits.Rx_8==0)
			{
				HDC_TT = 0xfff;
			}
			else if(CAN_ESC2_10[4].bits.Rx_8==1)
			{
				HDC_TT =  0x000;
			}
			else
			{
				HDC_TT =  0x000;
			}
		}
		else
		{
			HDC_TT =  0x000;
			HHC_MALFUNCTION_TT = 0xfff;

		}
		if(HHC_MALFUNCTION_TT == 0xfff)
		{
			DID_TT_bit_data_set3.bits.Rx_3=1;
		}
		else
		{
			DID_TT_bit_data_set3.bits.Rx_3=0;
		}

		if(HDC_TT == 0xfff)
		{
			DID_TT_bit_data_set3.bits.Rx_4=1;
		}
		else
		{
			DID_TT_bit_data_set3.bits.Rx_4=0;
		}

	}
	else
	{
		HDC_TT =  0x000;
		HHC_MALFUNCTION_TT = 0x000;
		DID_TT_bit_data_set3.bits.Rx_3=0;
		DID_TT_bit_data_set3.bits.Rx_4=0;

	}
	/*ABS*/

	if(((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==0) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM==1)) ||
			((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0)))
	{
		if(ESC2_10_message_timeout_flag == 0)
		{
			/*ABS TT*/
			if(CAN_ESC2_10[4].bits.Rx_3==0)
			{
				ABS_TT=0xfff;
				DID_TT_bit_data_set2.bits.Rx_5=1;
				/*ABS telltale feedback*/
				if(read_SID[550]==1 || read_SID[454]==1)
				{
					CAN_IC2_100[4].bits.Rx_3=1;    //FEEDBK_ABS_LAMP
					if(read_SID[550]==1)
					{
						ABS_TT_LOD=1;
					}
					else
					{
						ABS_TT_LOD=0;
					}
					if(read_SID[454]==1)
					{
						ABS_TT_LSD=1;
					}
					else
					{
						ABS_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[4].bits.Rx_3=0;    //FEEDBK_ABS_LAMP
					ABS_TT_LOD=0;
					ABS_TT_LSD=0;
				}
			}
			else if(CAN_ESC2_10[4].bits.Rx_3==1)
			{
				ABS_TT= 0x000;
				DID_TT_bit_data_set2.bits.Rx_5=0;
			}
			else
			{
				ABS_TT= 0x000;
			}
		}
		else
		{
			ABS_TT= 0xfff;
			DID_TT_bit_data_set2.bits.Rx_5=1;
		}


	}
	else
	{
		ABS_TT= 0x000;
		CAN_IC2_100[4].bits.Rx_3=0;    //FEEDBK_ABS_LAMP
		//CAN_IC2_100[4].bits.Rx_3=0;    //FEEDBK_ABS_LAMP
		ABS_TT_LOD=0;
		ABS_TT_LSD=0;
		DID_TT_bit_data_set2.bits.Rx_5=0;
	}

	//ABS Alert
	if(ABS_TT == 0xfff)
	{
		ABS_Notification =1;
	}
	else
	{
		ABS_Notification= 0;
	}
	/*AT_FAIL_TT*/

	if(VAR_CODING_TRANSMISSION_CONTROL_UNIT == 1)
	{
		if(TCU6_20_message_timeout_flag != 0)
		{
			AT_FAIL_TT = 0xfff;
		}

		if(TCU6_20_message_timeout_flag==CLEAR)
		{
			two_bit_data.bits.Rx_1 =CAN_TCU6_20[2].bits.Rx_7;
			two_bit_data.bits.Rx_2 =CAN_TCU6_20[2].bits.Rx_8;
			if(two_bit_data.byte == 0)
			{
				AT_FAIL_TT =  0x000;
			}
			else if(two_bit_data.byte == 1)
			{
				AT_FAIL_TT = 0xfff;
				/*AT_FAIL telltale feedback*/
				if(read_SID[549]==1 || read_SID[453]==1)
				{
					CAN_IC2_100[5].bits.Rx_3=1;    //FEEDBK_AMT
					if(read_SID[549]==1)
					{
						AT_FAIL_TT_LOD=1;
					}
					else
					{
						AT_FAIL_TT_LOD=0;
					}
					if(read_SID[453]==1)
					{
						AT_FAIL_TT_LSD=1;
					}
					else
					{
						AT_FAIL_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[5].bits.Rx_3=0;    //FEEDBK_AMT
					AT_FAIL_TT_LOD=0;
					AT_FAIL_TT_LSD=0;
				}
			}
			else if(two_bit_data.byte == 2)
			{
				AT_FAIL_TT = telltale_blink_flag_patt3;
			}
			else if(two_bit_data.byte == 3)
			{
				AT_FAIL_TT =  0x000;
			}
			else
			{
				AT_FAIL_TT =  0x000;
			}
		}
		if(AT_FAIL_TT == 0xfff)
		{
			DID_TT_bit_data_set2.bits.Rx_3=1;
		}
		else
		{
			DID_TT_bit_data_set2.bits.Rx_3=0;
		}
	}
	else
	{
		AT_FAIL_TT =  0x000;
		CAN_IC2_100[5].bits.Rx_3=0;    //FEEDBK_AMT
		DID_TT_bit_data_set2.bits.Rx_3=0;
	}

	/*AUTO_START_STOP_TT*/

	if(FEATURE_CODE_START_STOP_TT_CONFIG ==1 && VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
	{
		if(EMS3_10_message_timeout_flag==CLEAR)
		{
			three_bit_data.bits.Rx_1 = CAN_EMS3_10[7].bits.Rx_1;
			three_bit_data.bits.Rx_2 = CAN_EMS3_10[7].bits.Rx_2;
			three_bit_data.bits.Rx_3 = CAN_EMS3_10[7].bits.Rx_3;

			if(three_bit_data.byte == 0||three_bit_data.byte == 5 ||three_bit_data.byte == 6 ||three_bit_data.byte == 7)
			{
				AUTO_START_STOP_TT =  0x000;
				DID_TT_bit_data_set2.bits.Rx_7=0;
			}
			else if(three_bit_data.byte==1)
			{
				AUTO_START_STOP_TT = 0xfff;
				DID_TT_bit_data_set2.bits.Rx_7=1;
				/*AUTO_START_STOP telltale feedback*/
				if(read_SID[540]==1 || read_SID[444]==1)
				{
					CAN_IC2_100[4].bits.Rx_2=1;
					if(read_SID[540]==1)
					{
						AUTO_START_STOP_TT_LOD=1;
					}
					else
					{
						AUTO_START_STOP_TT_LOD=0;
					}
					if(read_SID[444]==1)
					{
						AUTO_START_STOP_TT_LSD=1;
					}
					else
					{
						AUTO_START_STOP_TT_LSD=0;
					}
				}
				else
				{
					CAN_IC2_100[4].bits.Rx_2=0;
					AUTO_START_STOP_TT_LSD=0;
					AUTO_START_STOP_TT_LSD=0;
				}
			}
			else if(three_bit_data.byte==2)
			{
				AUTO_START_STOP_TT= telltale_blink_flag_patt3;
			}
			else if(three_bit_data.byte==3)
			{
				AUTO_START_STOP_TT = telltale_blink_flag_patt7;
			}
			else if(three_bit_data.byte==4)
			{
				AUTO_START_STOP_TT = telltale_blink_flag_patt3;
			}
		}
		else
		{
			AUTO_START_STOP_TT =  0x000;
			DID_TT_bit_data_set2.bits.Rx_7=0;
		}

	}

	else
	{
		AUTO_START_STOP_TT =  0x000;
		DID_TT_bit_data_set2.bits.Rx_7=0;
		CAN_IC2_100[4].bits.Rx_2=0;

		AUTO_START_STOP_TT_LSD=0;
		AUTO_START_STOP_TT_LSD=0;
	}

	/*Global TPMS Malfunction*/
	if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1) && (FEATURE_CODE_TPMS_TT_CONFIG==1))
	{
		if(MBFM5_100_message_timeout_flag == CLEAR)
		{
			three_bit_data.bits.Rx_1 = CAN_MBFM5_100[7].bits.Rx_1;
			three_bit_data.bits.Rx_2 = CAN_MBFM5_100[7].bits.Rx_2;
			three_bit_data.bits.Rx_3 = CAN_MBFM5_100[7].bits.Rx_3;

			if(prev_MBFM5_100_message_timeout_flag != 0)
			{
				CounterTT_90sec=0;
				ninty_sec_cmplt_flg=0;
			}
			prev_MBFM5_100_message_timeout_flag=0;

			if(three_bit_data.byte == 3)
			{
				GLOBAL_TPMS_MULFUNCTION_TT=0xfff;
				ninty_sec_start_flg = 0;
				ninty_sec_cmplt_flg = 0;
			}
			else if(three_bit_data.byte==0)
			{
				ninty_sec_start_flg = 0;
				ninty_sec_cmplt_flg = 0;
				GLOBAL_TPMS_MULFUNCTION_TT = 0x000;
			}
			else if(three_bit_data.byte==1 )
			{
				GLOBAL_TPMS_MULFUNCTION_TT= telltale_blink_flag_patt8;  //patt8
				ninty_sec_start_flg = 0;
				ninty_sec_cmplt_flg = 0;
			}
			else if (three_bit_data.byte==2)
			{
				GLOBAL_TPMS_MULFUNCTION_TT= telltale_blink_flag_patt9;//patt9
				ninty_sec_start_flg = 0;
				ninty_sec_cmplt_flg = 0;
			}
			else if (three_bit_data.byte==4 ||three_bit_data.byte==5 ||three_bit_data.byte==6 || three_bit_data.byte==7)
			{
				ninty_sec_start_flg=1;
				if(ninty_sec_cmplt_flg == 0)
				{
					GLOBAL_TPMS_MULFUNCTION_TT = telltale_blink_flag_patt9;//patt9
				}
				else if(ninty_sec_cmplt_flg == 1)
				{
					GLOBAL_TPMS_MULFUNCTION_TT = 0xfff;
				}
				else
				{
					GLOBAL_TPMS_MULFUNCTION_TT = 0;
				}
			}
			else
			{
				GLOBAL_TPMS_MULFUNCTION_TT = 0;
				ninty_sec_start_flg = 0;
				ninty_sec_cmplt_flg = 0;
			}
		}
		else if(MBFM5_100_message_timeout_flag != 0)
		{
			if(prev_MBFM5_100_message_timeout_flag == 0)
			{
				CounterTT_90sec=0;
				ninty_sec_cmplt_flg=0;
			}
			prev_MBFM5_100_message_timeout_flag=1;

			ninty_sec_start_flg=1;
			if(ninty_sec_cmplt_flg == 0)
			{
				GLOBAL_TPMS_MULFUNCTION_TT = telltale_blink_flag_patt9;//patt9
			}
			else if(ninty_sec_cmplt_flg == 1)
			{
				GLOBAL_TPMS_MULFUNCTION_TT = 0xfff;
			}
			else
			{
				GLOBAL_TPMS_MULFUNCTION_TT = 0;
			}
		}
		else
		{
			GLOBAL_TPMS_MULFUNCTION_TT = 0;
		}

		if(GLOBAL_TPMS_MULFUNCTION_TT == 0xfff)
		{
			DID_TT_bit_data_set2.bits.Rx_4=1;
		}
		else
		{
			DID_TT_bit_data_set2.bits.Rx_4=0;
		}
	}
	else
	{
		GLOBAL_TPMS_MULFUNCTION_TT = 0;
		DID_TT_bit_data_set2.bits.Rx_4=0;
	}


	/*IMMOBILIZER_TT*/
	if(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0)  //config_0
	{
		IMMOBIIZER_TT=0;   				//R_GPIO_WritePin(16, 10, 0); //IMMOBILIZER_TT=0
		DID_TT_bit_data_set1.bits.Rx_7=IMMOBIIZER_TT;
	}
	else if(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)		//config_2
	{
		IMMOBIIZER_TT=0;				//R_GPIO_WritePin(16, 10, 0); //IMMOBILIZER_TT=0
		DID_TT_bit_data_set1.bits.Rx_7=IMMOBIIZER_TT;
	}

	else if((IGN_ON_flag == 1) && ((VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)))
	{
		if(PKE_ICU2_100_message_timeout_flag != 0)			//config_1,3
		{
			IMMOBIIZER_TT=1;
		}
		else if(PKE_ICU2_100_message_timeout_flag == 0)
		{

			if(CAN_PKE_ICU2_100[0].bits.Rx_1==0 && CAN_PKE_ICU2_100[0].bits.Rx_2==0)
			{
				IMMOBIIZER_TT=0;
				previous_state_immobilizer=0;

			}
			else if(CAN_PKE_ICU2_100[0].bits.Rx_1==1 && CAN_PKE_ICU2_100[0].bits.Rx_2==0)
			{
				IMMOBIIZER_TT=telltale_blink_bool3;
				previous_state_immobilizer=1;
			}
			else if(CAN_PKE_ICU2_100[0].bits.Rx_1==0 && CAN_PKE_ICU2_100[0].bits.Rx_2==1)
			{
				IMMOBIIZER_TT=1;
				previous_state_immobilizer=0;

			}
			else if(CAN_PKE_ICU2_100[0].bits.Rx_1==1 && CAN_PKE_ICU2_100[0].bits.Rx_2==1)
			{
				if(previous_state_immobilizer==1)
				{
					IMMOBIIZER_TT=telltale_blink_bool3;

				}
				else
				{
					IMMOBIIZER_TT= IMMOBIIZER_TT;
				}
			}
		}
		DID_TT_bit_data_set1.bits.Rx_7=IMMOBIIZER_TT;
	}
	else
	{
		IMMOBIIZER_TT=0;
		DID_TT_bit_data_set1.bits.Rx_7=IMMOBIIZER_TT;
	}

	/************************************************************************************/
	if(IGN_ON_flag == 0)
	{
		AEBS_OFF_TT      = 0;
		SEAT_BELT_TT     = 0;
		LOW_FUEL_TT      = 0;
		BSG_TT           = 0;
		FRONT_FOG_TT     = 0;
		REAR_FOG_TT      = 0;
		PARK_LAMP_TT     = 0;
		OVER_SPEED_TT    = 0;
		FOUR_WD_LOW_TT   = 0;
		FOUR_WD_HIGH_TT  = 0;
		Water_in_fuel_TT = 0;
		TEMP_TT          = 0;
		Glow_Plug_TT     = 0;
		REGEN_TT         = 0;
		DOOR_OPEN_TT     = 0;
		ECO_TT           = 0;
	}

	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0]=DID_TT_bit_data_set1.byte;
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[0]=DID_TT_bit_data_set2.byte;
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[0]=DID_TT_bit_data_set3.byte;
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[0]=DID_TT_bit_data_set4.byte;
	DID_TELL_TALE_STATUS_SET5_0xF08F_arr[0]=DID_TT_bit_data_set5.byte;
	DID_TELL_TALE_STATUS_SET6_0xF096_arr[0]=DID_TT_bit_data_set6.byte;

}

void IO_CONTROL_TT_FUNCTIONALITY(void)
{
	if(IO_Parameter_Request_Status==3)
	{

		if(check_tt_io_control_once==0)
		{
			Turn_left_TT =0;
			MIL_TT 	=0;
			AEBS_MULFUNCTION_ON_TT	=0;
			CRUISE_TT 	=0;
			GLOBAL_TPMS_MULFUNCTION_TT 	=0;
			EPS_MALFUNCTION_TT 	=0;
			HDC_TT 		=0;
			ESP_OFF_TT	=0;
			PARK_BRAKE_LOW_FLUID_EBD_TT	=0;
			ECT_high_TT		=0;
			ESP_MALFUNCTION_TT	=0;
			HHC_MALFUNCTION_TT =0;
			PRESS_BRAKE_PEDAL_TT=0;
			DEF_TT=0;
			DPF_TT	=0;
			High_beam_TT=0;
			Low_Oil_press_TT=0;
			AUTO_START_STOP_TT=0;
			Check_Engine_TT	=0;
			AIR_BAG_TT	=0;
			Turn_right_TT =0;
			AT_FAIL_TT	=0;
			Batt_chrg_TT	=0;
			SPEAR_TT1		=0;
			ABS_TT	=0;
			AEBS_OFF_TT      = 0;
			SEAT_BELT_TT     = 0;
			LOW_FUEL_TT      = 0;
			BSG_TT           = 0;
			FRONT_FOG_TT     = 0;
			REAR_FOG_TT      = 0;
			PARK_LAMP_TT     = 0;
			OVER_SPEED_TT    = 0;
			FOUR_WD_LOW_TT   = 0;
			FOUR_WD_HIGH_TT  = 0;
			Water_in_fuel_TT = 0;
			TEMP_TT          = 0;
			Glow_Plug_TT     = 0;
			REGEN_TT         = 0;
			DOOR_OPEN_TT     = 0;
			ECO_TT           = 0;
			IMMOBIIZER_TT     =0;
			ESP_MALFUNCTION_TT=0;
			ESP_OFF_TT=0;
			SendGSData(GSdata1);
			output_shift_register();
			check_tt_io_control_once=1;
		}
		if(IO_CONTROL_SET1_TT==1)
		{
			/*check_engine*/
			if((((IO_Control_Enable_TT_SET1.byte)& 1)==1) &&(((IO_Control_State_TT_SET1.byte)& 1)==1))
			{
				Check_Engine_TT = 0xfff;
				DID_TT_bit_data_set1.bits.Rx_1=1;
			}
			else
			{
				Check_Engine_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_1=0;
			}
			/*glow_plug*/
			if((((IO_Control_Enable_TT_SET1.byte)& (1<<1))==2) &&(((IO_Control_State_TT_SET1.byte)& (1<<1))==2))
			{
				Glow_Plug_TT = 1;
				DID_TT_bit_data_set1.bits.Rx_2=1;
			}
			else
			{
				Glow_Plug_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_2=0;
			}
			/*engine_oil_pressure*/
			if((((IO_Control_Enable_TT_SET1.byte)& (1<<2))==4) &&(((IO_Control_State_TT_SET1.byte)& (1<<2))==4))
			{
				Low_Oil_press_TT = 0xfff;
				DID_TT_bit_data_set1.bits.Rx_3=1;
			}
			else
			{
				Low_Oil_press_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_3=0;
			}
			/*4WD_HIGH*/
			if((FEATURE_CODE_TRANSFER_MODE_CONFIG == 1)&&((FEATURE_CODE_4WD_LOGIC==0)||(FEATURE_CODE_4WD_LOGIC==1)))
			{
				if((((IO_Control_Enable_TT_SET1.byte)& (1<<3))==8) &&(((IO_Control_State_TT_SET1.byte)& (1<<3))==8))
				{
					FOUR_WD_HIGH_TT=1;
					DID_TT_bit_data_set1.bits.Rx_4=1;
				}
				else
				{
					FOUR_WD_HIGH_TT=0;
					DID_TT_bit_data_set1.bits.Rx_4=0;
				}
			}
			else
			{
				FOUR_WD_HIGH_TT=0;
				DID_TT_bit_data_set1.bits.Rx_4=0;
			}
			/*Door_open*/
			if((((IO_Control_Enable_TT_SET1.byte)& (1<<4))==16) &&(((IO_Control_State_TT_SET1.byte)& (1<<4))==16))
			{
				DOOR_OPEN_TT = 1;
				DID_TT_bit_data_set1.bits.Rx_5=1;
			}
			else
			{
				DOOR_OPEN_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_5=0;
			}
			/*seat_belt*/
			if((((IO_Control_Enable_TT_SET1.byte)& (1<<5))==32) &&(((IO_Control_State_TT_SET1.byte)& (1<<5))==32))
			{
				SEAT_BELT_TT = 1;
				DID_TT_bit_data_set1.bits.Rx_6=1;
			}
			else
			{
				SEAT_BELT_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_6=0;
			}
			/*Immobilizer*/
			if((((IO_Control_Enable_TT_SET1.byte)& (1<<6))==64) &&(((IO_Control_State_TT_SET1.byte)& (1<<6))==64))
			{
				IMMOBIIZER_TT=1;
				DID_TT_bit_data_set1.bits.Rx_7=1;
			}
			else
			{
				IMMOBIIZER_TT=0;
				DID_TT_bit_data_set1.bits.Rx_7=0;
			}
			/*PARK_BRAKE_LOW_FLUID_EBD*/
			if((((IO_Control_Enable_TT_SET1.byte)& (1<<7))==128) &&(((IO_Control_State_TT_SET1.byte)& (1<<7))==128))
			{
				PARK_BRAKE_LOW_FLUID_EBD_TT = 0xfff;
				DID_TT_bit_data_set1.bits.Rx_8=1;
			}
			else
			{
				PARK_BRAKE_LOW_FLUID_EBD_TT = 0;
				DID_TT_bit_data_set1.bits.Rx_8=0;
			}

			SendGSData(GSdata1);
			output_shift_register();
		}

		if(IO_CONTROL_SET2_TT==1)
		{
			/*Airbag*/
			if((((IO_Control_Enable_TT_SET2.byte)& 1)==1) &&(((IO_Control_State_TT_SET2.byte)& 1)==1))
			{
				AIR_BAG_TT =0xfff;
				DID_TT_bit_data_set2.bits.Rx_1=1;
			}
			else
			{
				AIR_BAG_TT =0;
				DID_TT_bit_data_set2.bits.Rx_1=0;
			}
			/*cruise*/
			if((((IO_Control_Enable_TT_SET2.byte)& (1<<1))==2) &&(((IO_Control_State_TT_SET2.byte)& (1<<1))==2))
			{
				CRUISE_TT=0xfff;
				DID_TT_bit_data_set2.bits.Rx_2=1;
			}
			else
			{
				CRUISE_TT=0;
				DID_TT_bit_data_set2.bits.Rx_2=0;
			}
			/*AT_fail*/
			if(VAR_CODING_TRANSMISSION_CONTROL_UNIT == 1)
			{
				if((((IO_Control_Enable_TT_SET2.byte)& (1<<2))==4) &&(((IO_Control_State_TT_SET2.byte)& (1<<2))==4))
				{
					AT_FAIL_TT = 0xfff;
					DID_TT_bit_data_set2.bits.Rx_3=1;
				}
				else
				{
					AT_FAIL_TT = 0;
					DID_TT_bit_data_set2.bits.Rx_3=0;
				}
			}
			else
			{
				AT_FAIL_TT = 0;
				DID_TT_bit_data_set2.bits.Rx_3=0;
			}
			/*TPMS*/
			if((((IO_Control_Enable_TT_SET2.byte)& (1<<3))==8) &&(((IO_Control_State_TT_SET2.byte)& (1<<3))==8))
			{
				GLOBAL_TPMS_MULFUNCTION_TT=0xfff;
				DID_TT_bit_data_set2.bits.Rx_4=1;
			}
			else
			{
				GLOBAL_TPMS_MULFUNCTION_TT=0;
				DID_TT_bit_data_set2.bits.Rx_4=0;
			}
			/*ABS*/
			if((((IO_Control_Enable_TT_SET2.byte)& (1<<4))==16) &&(((IO_Control_State_TT_SET2.byte)& (1<<4))==16))
			{
				ABS_TT=0xfff;
				DID_TT_bit_data_set2.bits.Rx_5=1;
			}
			else
			{
				ABS_TT=0;
				DID_TT_bit_data_set2.bits.Rx_5=0;
			}
			/*Water_in_fuel*/
			if((((IO_Control_Enable_TT_SET2.byte)& (1<<5))==32) &&(((IO_Control_State_TT_SET2.byte)& (1<<5))==32))
			{
				Water_in_fuel_TT =1;
				DID_TT_bit_data_set2.bits.Rx_6=1;
			}
			else
			{
				Water_in_fuel_TT =0;
				DID_TT_bit_data_set2.bits.Rx_6=0;
			}
			/*ESS-AUTO_START_STOP*/
			if((((IO_Control_Enable_TT_SET2.byte)& (1<<6))==64) &&(((IO_Control_State_TT_SET2.byte)& (1<<6))==64))
			{
				AUTO_START_STOP_TT = 0xfff;
				DID_TT_bit_data_set2.bits.Rx_7=1;
			}
			else
			{
				AUTO_START_STOP_TT = 0;
				DID_TT_bit_data_set2.bits.Rx_7=0;
			}
			/*MIL*/
			if((((IO_Control_Enable_TT_SET2.byte)& (1<<7))==128) &&(((IO_Control_State_TT_SET2.byte)& (1<<7))==128))
			{
				MIL_TT = 0xfff;
				DID_TT_bit_data_set2.bits.Rx_8=1;
			}
			else
			{
				MIL_TT = 0;
				DID_TT_bit_data_set2.bits.Rx_8=0;
			}

			SendGSData(GSdata1);
			output_shift_register();
		}

		if(IO_CONTROL_SET3_TT==1)
		{
			/*ESC_OFF*/
			if((((IO_Control_Enable_TT_SET3.byte)& 1)==1) &&(((IO_Control_State_TT_SET3.byte)& 1)==1))
			{
				ESP_OFF_TT=0xfff;
				DID_TT_bit_data_set3.bits.Rx_1=1;
			}
			else
			{
				ESP_OFF_TT=0x0;
				DID_TT_bit_data_set3.bits.Rx_1=0;
			}
			/*PARK_LAMP*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<1))==2) &&(((IO_Control_State_TT_SET3.byte)& (1<<1))==2))
			{
				PARK_LAMP_TT=1;
				DID_TT_bit_data_set3.bits.Rx_2=1;
			}
			else
			{
				PARK_LAMP_TT=0;
				DID_TT_bit_data_set3.bits.Rx_2=0;
			}
			/*HHC_MALFUNCTION*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<2))==4) &&(((IO_Control_State_TT_SET3.byte)& (1<<2))==4))
			{
				HHC_MALFUNCTION_TT = 0xfff;
				DID_TT_bit_data_set3.bits.Rx_3=1;
			}
			else
			{
				HHC_MALFUNCTION_TT = 0;
				DID_TT_bit_data_set3.bits.Rx_3=0;
			}
			/*HDC_MALFUNCTION*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<3))==8) &&(((IO_Control_State_TT_SET3.byte)& (1<<3))==8))
			{
				HDC_TT = 0xfff;
				DID_TT_bit_data_set3.bits.Rx_4=1;
			}
			else
			{
				HDC_TT = 0;
				DID_TT_bit_data_set3.bits.Rx_4=0;
			}
			/*ESC ACTIVE*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<4))==16) &&(((IO_Control_State_TT_SET3.byte)& (1<<4))==16))
			{
				ESP_MALFUNCTION_TT=0xfff;
				DID_TT_bit_data_set3.bits.Rx_5=1;
			}
			else
			{
				ESP_MALFUNCTION_TT=0x0;
				DID_TT_bit_data_set3.bits.Rx_5=0;
			}
			/*BATTERY_CHARGE*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<5))==32) &&(((IO_Control_State_TT_SET3.byte)& (1<<5))==32))
			{
				Batt_chrg_TT = 0xfff;
				DID_TT_bit_data_set3.bits.Rx_6=1;
			}
			else
			{
				Batt_chrg_TT = 0;
				DID_TT_bit_data_set3.bits.Rx_6=0;
			}
			/*LOW_FUEL*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<6))==64) &&(((IO_Control_State_TT_SET3.byte)& (1<<6))==64))
			{
				LOW_FUEL_TT=1;
				DID_TT_bit_data_set3.bits.Rx_7=1;
			}
			else
			{
				LOW_FUEL_TT=0;
				DID_TT_bit_data_set3.bits.Rx_7=0;
			}
			/*HIGH_TEMP*/
			if((((IO_Control_Enable_TT_SET3.byte)& (1<<7))==128) &&(((IO_Control_State_TT_SET3.byte)& (1<<7))==128))
			{
				ECT_high_TT=0xfff;
				DID_TT_bit_data_set3.bits.Rx_8=1;
			}
			else
			{
				ECT_high_TT=0;
				DID_TT_bit_data_set3.bits.Rx_8=0;
			}

			SendGSData(GSdata1);
			output_shift_register();
		}

		if(IO_CONTROL_SET4_TT==1)
		{
			/*BSG*/
			/*if((((IO_Control_Enable_TT_SET4.byte)& 1)==1) &&(((IO_Control_State_TT_SET4.byte)& 1)==1))
			{
				BSG_TT=1;
				DID_TT_bit_data_set4.bits.Rx_1=1;
			}
			else
			{
				BSG_TT=0;
				DID_TT_bit_data_set4.bits.Rx_1=0;
			}			*/
			/*CO_DRIVER_SEAT_BELT*/
		/*	if((((IO_Control_Enable_TT_SET4.byte)& (1<<1))==2) &&(((IO_Control_State_TT_SET4.byte)& (1<<1))==2))
			{

			}
			else
			{

			}
			/*ECO*/
			if((((IO_Control_Enable_TT_SET4.byte)& (1<<2))==4) &&(((IO_Control_State_TT_SET4.byte)& (1<<2))==4))
			{
				ECO_TT = 1;
				DID_TT_bit_data_set4.bits.Rx_3=1;
			}
			else
			{
				ECO_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_3=0;
			}
			/*HIGH_BEAM*/
			if((((IO_Control_Enable_TT_SET4.byte)& (1<<3))==8) &&(((IO_Control_State_TT_SET4.byte)& (1<<3))==8))
			{
				High_beam_TT = 0xfff;
				DID_TT_bit_data_set4.bits.Rx_4=1;
			}
			else
			{
				High_beam_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_4=0;
			}
			/*TURN_LEFT*/
			if((((IO_Control_Enable_TT_SET4.byte)& (1<<4))==16) &&(((IO_Control_State_TT_SET4.byte)& (1<<4))==16))
			{
				Turn_left_TT = 0xfff;
				DID_TT_bit_data_set4.bits.Rx_5=1;
			}
			else
			{
				Turn_left_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_5=0;
			}
			/*TURN_RIGHT*/
			if((((IO_Control_Enable_TT_SET4.byte)& (1<<5))==32) &&(((IO_Control_State_TT_SET4.byte)& (1<<5))==32))
			{
				Turn_right_TT = 0xfff;
				DID_TT_bit_data_set4.bits.Rx_6=1;
			}
			else
			{
				Turn_right_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_6=0;
			}
			/*FRONT_FOG*/
			if((((IO_Control_Enable_TT_SET4.byte)& (1<<6))==64) &&(((IO_Control_State_TT_SET4.byte)& (1<<6))==64))
			{
				FRONT_FOG_TT = 1;
				DID_TT_bit_data_set4.bits.Rx_7=1;
			}
			else
			{
				FRONT_FOG_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_7=0;
			}
			/*REAR_FOG*/
			if((((IO_Control_Enable_TT_SET4.byte)& (1<<7))==128) &&(((IO_Control_State_TT_SET4.byte)& (1<<7))==128))
			{
				REAR_FOG_TT = 1;
				DID_TT_bit_data_set4.bits.Rx_8=1;
			}
			else
			{
				REAR_FOG_TT = 0;
				DID_TT_bit_data_set4.bits.Rx_8=0;
			}

			SendGSData(GSdata1);
			output_shift_register();
		}

		if(IO_CONTROL_SET5_TT==1)
		{
			/*EPS*/
		/*	if((((IO_Control_Enable_TT_SET5.byte)& 1)==1) &&(((IO_Control_State_TT_SET5.byte)& 1)==1))
			{
				EPS_MALFUNCTION_TT=0xfff;
				DID_TT_bit_data_set5.bits.Rx_1=1;
			}
			else
			{
				EPS_MALFUNCTION_TT=0x000;
				DID_TT_bit_data_set5.bits.Rx_1=0;
			}*/
			/*UREA_LEVEL*/
			if((((IO_Control_Enable_TT_SET5.byte)& (1<<1))==2) &&(((IO_Control_State_TT_SET5.byte)& (1<<1))==2))
			{

			}
			else
			{

			}
			/*KEY_NOT_AVAILABLE*/
			if((((IO_Control_Enable_TT_SET5.byte)& (1<<2))==4) &&(((IO_Control_State_TT_SET5.byte)& (1<<2))==4))
			{

			}
			else
			{

			}
			/*PRESS_CLUTCH_PEDAL*/
			if((((IO_Control_Enable_TT_SET5.byte)& (1<<3))==8) &&(((IO_Control_State_TT_SET5.byte)& (1<<3))==8))
			{

			}
			else
			{

			}
			/*STEERING_LOCKED*/
			if((((IO_Control_Enable_TT_SET5.byte)& (1<<4))==16) &&(((IO_Control_State_TT_SET5.byte)& (1<<4))==16))
			{

			}
			else
			{

			}
			/*PRESS_BRAKE_PEDAL*/
			/*if((((IO_Control_Enable_TT_SET5.byte)& (1<<5))==32) &&(((IO_Control_State_TT_SET5.byte)& (1<<5))==32))
			{
				PRESS_BRAKE_PEDAL_TT=0xfff;
			}
			else
			{
				PRESS_BRAKE_PEDAL_TT=0;
			}*/
			/*OVERSPEED*/
			if((((IO_Control_Enable_TT_SET5.byte)& (1<<6))==64) &&(((IO_Control_State_TT_SET5.byte)& (1<<6))==64))
			{
				OVER_SPEED_TT = 1;
				DID_TT_bit_data_set5.bits.Rx_7=1;
			}
			else
			{
				OVER_SPEED_TT = 0;
				DID_TT_bit_data_set5.bits.Rx_7=0;
			}
			/*RESRVED*/
			if((((IO_Control_Enable_TT_SET5.byte)& (1<<7))==128) &&(((IO_Control_State_TT_SET5.byte)& (1<<7))==128))
			{

			}
			else
			{

			}

			SendGSData(GSdata1);
			output_shift_register();
		}

		if(IO_CONTROL_SET6_TT==1)
		{
			/*DPF*/
			if((((IO_Control_Enable_TT_SET6.byte)& 1)==1) &&(((IO_Control_State_TT_SET6.byte)& 1)==1))
			{
				DPF_TT = 0xfff;
				DID_TT_bit_data_set6.bits.Rx_1=1;
			}
			else
			{
				DPF_TT = 0;
				DID_TT_bit_data_set6.bits.Rx_1=0;
			}
			/*DEF*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<1))==2) &&(((IO_Control_State_TT_SET6.byte)& (1<<1))==2))
			{
				DEF_TT = 0xfff;
				DID_TT_bit_data_set6.bits.Rx_2=1;
			}
			else
			{
				DEF_TT = 0;
				DID_TT_bit_data_set6.bits.Rx_2=0;
			}
			/*4WD_LOW*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<2))==4) &&(((IO_Control_State_TT_SET6.byte)& (1<<2))==4))
			{
				FOUR_WD_LOW_TT=1;
				DID_TT_bit_data_set6.bits.Rx_3=1;
			}
			else
			{
				FOUR_WD_LOW_TT=0;
				DID_TT_bit_data_set6.bits.Rx_3=0;
			}
			/*REGEN*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<3))==8) &&(((IO_Control_State_TT_SET6.byte)& (1<<3))==8))
			{
				REGEN_TT=1;
				DID_TT_bit_data_set6.bits.Rx_4=1;
			}
			else
			{
				REGEN_TT=0;
				DID_TT_bit_data_set6.bits.Rx_4=0;
			}
			/*AUTOMATIC_EMERGENCY_BRAKING_MALFUNCTION*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<4))==16) &&(((IO_Control_State_TT_SET6.byte)& (1<<4))==16))
			{

			}
			else
			{

			}
			/*AUTOMATIC_EMERGENCY_BRAKING_OFF*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<5))==32) &&(((IO_Control_State_TT_SET6.byte)& (1<<5))==32))
			{

			}
			else
			{

			}
			/*LKAS/LWDS*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<6))==64) &&(((IO_Control_State_TT_SET6.byte)& (1<<6))==64))
			{

			}
			else
			{

			}
			/*HBA*/
			if((((IO_Control_Enable_TT_SET6.byte)& (1<<7))==128) &&(((IO_Control_State_TT_SET6.byte)& (1<<7))==128))
			{

			}
			else
			{

			}
			SendGSData(GSdata1);
			output_shift_register();
		}

		if(IO_Control_Illumination_set==1)
		{
			static uint8_t illumination_dc_data;
			static uint8_t illumination_dc_data_pointer;
			uint16_t lcd_illumination;
			if(io_pointer_ill_flag==0)
			{
				S_LD1_TT=0;
				S_LD2_TT=0;
				S_LD3_TT=0;
				S_LD4_TT=0;
				S_LD5_TT=0;
				S_LD6_TT=0;
				R_LD1_TT=0;
				R_LD2_TT=0;
				R_LD3_TT=0;
				R_LD4_TT=0;
				R_LD5_TT=0;
				R_LD6_TT=0;
				SP_LD1_TT=0;
				SP_LD2_TT=0;
				SP_LD3_TT=0;
				RP_LD1_TT=0;
				RP_LD2_TT=0;
				RP_LD3_TT=0;
				R_LD8_TT=0;
				R_LD7_TT=0;
				SendGSData(GSdata1);
				io_pointer_ill_flag=1;
			}
			if(((IO_Control_Enable_Illumination.byte) & 1)==1)/*lcd_illumination*/

			{
				lcd_illumination=IO_Control_State_LCD_Illumination_Status*3;
				r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,lcd_illumination/1);   //90%TFT_ILLUMINATION
				r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
				DID_ILLUMINATION_STATUS_0xF084_arr[0]=IO_Control_State_LCD_Illumination_Status;

			}
			if(((IO_Control_Enable_Illumination.byte) & (1<<1))==2)/*dial_illumination*/
			{

				static uint8_t changed_dc_data;
				changed_dc_data=illumination_dc_data;
				illumination_dc_data=IO_Control_State_Dial_Illumination_Status*1.27;
				DID_ILLUMINATION_STATUS_0xF084_arr[1]=IO_Control_State_Dial_Illumination_Status;
				if(changed_dc_data!=illumination_dc_data)
				{
					DCdata1[0]=illumination_dc_data;
					DCdata1[1]=illumination_dc_data;
					DCdata1[2]=illumination_dc_data;
					DCdata1[3]=illumination_dc_data;
					DCdata1[4]=illumination_dc_data;
					DCdata1[5]=illumination_dc_data;


					DCdata2[15]=illumination_dc_data;
					DCdata2[16]=illumination_dc_data;
					DCdata2[18]=illumination_dc_data;
					DCdata2[19]=illumination_dc_data;
					DCdata2[20]=illumination_dc_data;
					DCdata2[21]=illumination_dc_data;
					DCdata2[22]=illumination_dc_data;
					DCdata2[23]=illumination_dc_data;

					SendDCData(0x000, 0x3, 0x00, 0, 0, 0, 0, 1, 255, 255, 255, DCdata1);
					SendDCData(0x000, 0x3, 0x00, 0, 0, 0, 0, 1, 255, 255, 255, DCdata2);

					cascade_TT_send_data();

					S_LD1_TT=0xfff;
					S_LD2_TT=0xfff;
					S_LD3_TT=0xfff;
					S_LD4_TT=0xfff;
					S_LD5_TT=0xfff;
					S_LD6_TT=0xfff;
					R_LD1_TT=0xfff;
					R_LD2_TT=0xfff;
					R_LD3_TT=0xfff;
					R_LD4_TT=0xfff;
					R_LD5_TT=0xfff;
					R_LD6_TT=0xfff;
					if(FEATURE_CODE_FUEL_TYPE == 1)
					{
						/* Diesel*/
						R_LD8_TT=0xfff;
						R_LD7_TT=0x000;

					}
					else
					{
						/* GAsoline*/
						R_LD8_TT=0;
						R_LD7_TT=0xfff;
					}

				}
				SendGSData(GSdata1);
			}

			if(((IO_Control_Enable_Illumination.byte) & (1<<2))==4)/*pointer_illumination*/
			{
				static uint8_t changed_dc_data_pointer;
				changed_dc_data_pointer=illumination_dc_data_pointer;

				illumination_dc_data_pointer=IO_Control_State_Pointer_Illumination_Status*1.27;
				DID_ILLUMINATION_STATUS_0xF084_arr[2]=IO_Control_State_Pointer_Illumination_Status;
				if(changed_dc_data_pointer!=illumination_dc_data_pointer)
				{
				DCdata1[6]=illumination_dc_data_pointer;/*pointer*/
				DCdata1[7]=illumination_dc_data_pointer;
				DCdata1[8]=illumination_dc_data_pointer;

				DCdata2[12]=illumination_dc_data_pointer;/*pointer*/
				DCdata2[13]=illumination_dc_data_pointer;
				DCdata2[17]=illumination_dc_data_pointer;

				SendDCData(0x000, 0x3, 0x00, 0, 0, 0, 0, 1, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 0, 0, 0, 1, 255, 255, 255, DCdata2);

				cascade_TT_send_data();

				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			tst_tt_flag=0;
		}

			DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0]=DID_TT_bit_data_set1.byte;
			DID_TELL_TALE_STATUS_SET2_0xF080_arr[0]=DID_TT_bit_data_set2.byte;
			DID_TELL_TALE_STATUS_SET3_0xF081_arr[0]=DID_TT_bit_data_set3.byte;
			DID_TELL_TALE_STATUS_SET4_0xF08E_arr[0]=DID_TT_bit_data_set4.byte;
			DID_TELL_TALE_STATUS_SET5_0xF08F_arr[0]=DID_TT_bit_data_set5.byte;
			DID_TELL_TALE_STATUS_SET6_0xF096_arr[0]=DID_TT_bit_data_set6.byte;
	}

}

void W501_RPAS_CHIME_ENABLE(void)
{
#ifdef COLOR_TFT

	/*RPAS CHIMES-W501*/
	/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........RPAS self check tone*/
	if(Diag_RW_RPASS_Self_check_Chime_Enable==1)
	{
		if(RPAS_Precondition_Active==1)
		{
			if(MBFM_PAS1_50_message_timeout_flag == 0)

			{

				five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
				five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
				five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
				five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
				five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

				if((five_bit_data.byte==0)&&(RPASS_self_check_ongoing == 0)/*&&(chime_start1 == 0)*/&&(cycle_RPASS_SELF_CHECK_completed == 0))
				{
					if((Sound_Bit == 0x0 || Sound_Bit == 0x8000 || Sound_Bit == 0x200))
					{
						Sound_Bit = 0x8000;
						RPASS_self_check_ongoing=1;
						chime_start1=1;
						RPAS_self_check_missed=0;
						if(Self_tone_Active == 0)
						{
							RPAS_self_check_counter500msec = (Diag_RW_RPAS_self_check_chime_Enable/10)-5;
							Self_tone_Active = 1;
						}
					}
					else
					{
						RPASS_self_check_ongoing=0;
						RPAS_self_check_missed=0;
					}
				}
				else if(five_bit_data.byte!=0)
				{
					/*chime off*/
					RPAS_self_check_missed=1;
					RPASS_self_check_ongoing=0;
					cycle_RPASS_SELF_CHECK_completed=0;
					if(Sound_Bit == 0x8000)
					{
						Sound_Bit = 0x00;						/**Sound bit CLEAR**/
						SoundPCM_Reverse_cont_STOP();
					}
					Self_tone_Active = 0;
				}
			}
			else
			{
				if(Sound_Bit == 0x8000)
				{
					Sound_Bit = 0x0;
					SoundPCM_Reverse_cont_STOP();
				}

				RPASS_self_check_ongoing=0;
				RPAS_self_check_missed=1;
				cycle_RPASS_SELF_CHECK_completed = 0;
			}
		}
		else
		{
			if(Sound_Bit == 0x8000)
			{
				Sound_Bit = 0x0;
				SoundPCM_Reverse_cont_STOP();
			}

			RPASS_self_check_ongoing=0;
			RPAS_self_check_missed=1;
			cycle_RPASS_SELF_CHECK_completed = 0;
		}
	}
	else
	{
		RPASS_self_check_ongoing=0;
		RPAS_self_check_missed=1;
		cycle_RPASS_SELF_CHECK_completed = 0;
	}



	/**************************RPAS_FAIL_CHM_FUNCTION_LOGIC*************************/

		if(Diag_RW_RPASS_Fail_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)
				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if((five_bit_data.byte!=0)&&(W501_RPASS_fail_check_ongoing == 0)/*&&(chime_start1 == 0)*/&&(W501_cycle_RPASS_FAIL_CHECK_completed==0))
					{
						if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
						{
							Sound_Bit = 0x4000;
							W501_RPASS_fail_check_ongoing=1;
							chime_start1=1;
							RPAS_fail_check_missed=0;
							RPAS_Error_flag=1;
							if(Error_tone_Active == 0)
							{
								W501_RPAS_FAIL_check_counter500msec = 0;
								Error_tone_Active = 1;
							}
						}
						else
						{
						        RPAS_Error_flag=0;
							W501_RPASS_fail_check_ongoing=0;
							RPAS_fail_check_missed=0;
							Error_tone_Active = 0;
						}

					}

					else if(five_bit_data.byte==0)
					{/*chime off*/
						RPAS_fail_check_missed=1;
						W501_RPASS_fail_check_ongoing=0;
						W501_cycle_RPASS_FAIL_CHECK_completed=0;
						   RPAS_Error_flag=0;
						if(Sound_Bit == 0x4000)
						{
							Sound_Bit = 0x00;						/**Sound bit CLEAR**/
							SoundPCM_Reverse_cont_STOP();
						}
						Error_tone_Active = 0;
						Error_soundstart = 0;

					}
				}
				else
				{
					if((Sound_Bit == 0x0) || Sound_Bit == 0x200 || (Sound_Bit == 0x4000)|| (Sound_Bit == 0x8000)|| (Sound_Bit == 0x10000)|| (Sound_Bit == 0x20000)|| (Sound_Bit == 0x30000)|| (Sound_Bit == 0x40000))
					{
						Sound_Bit = 0x4000;
						W501_RPASS_fail_check_ongoing=1;
						/*chime_start1=1;*/
						RPAS_fail_check_missed=0;
						RPAS_Error_flag=1;/* at message time out the rpas error tone required*/
					}
					else
					{
						W501_RPASS_fail_check_ongoing=0;
						RPAS_fail_check_missed=0;
					}
				}

			}
			else
			{
				if(Sound_Bit == 0x4000)
				{
					Sound_Bit = 0x0;
					SoundPCM_Reverse_cont_STOP();
				}
				W501_cycle_RPASS_FAIL_CHECK_completed = 0;
				W501_RPASS_fail_check_ongoing=0;
				RPAS_Error_flag=0;
			}
		}
		else
		{
			W501_RPASS_fail_check_ongoing=0;
			RPAS_Error_flag=0;
		}


	/******RPAS STATUS CHIME FUNCTIONAL LOGIC*******/
	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X01*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 1)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
						four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
						four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
						four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;


						if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
						{
							/*chime level_1*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_1_BUZ=0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_1_BUZ=0;
						}

						if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_2_BUZ=0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_2_BUZ=0;
						}

						if(four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_3_BUZ=0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_3_BUZ=0;
						}

						/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
						}
						if(four_bit_data_RRC.byte==0 ||( four_bit_data_RRC.byte >3 && four_bit_data_RRC.byte <= 0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
						}*/
					}
				}
				else
				{
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
				}
			}
			else
			{

				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
				}
				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
			}
		}
		else
		{
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
		}
	}

	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X02*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 2)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
						four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
						four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
						four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;

						if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
						{
							/*chime level_1*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_1_BUZ=0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_1_BUZ=0;
						}
						/*chime level_2*/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_2_BUZ=0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_2_BUZ=0;
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_3_BUZ=0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_3_BUZ=0;
						}
						/*chime level_4*/
						if(four_bit_data_RLC.byte==4 || four_bit_data_RRC.byte==4)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x40000;
								W501_RPAS_LEVEL_4_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_4_BUZ=0;
								if(Sound_Bit == 0x40000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_4_BUZ=0;
						}
						/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							W501_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RRC.byte==0 ||( four_bit_data_RRC.byte >3 && four_bit_data_RRC.byte <= 0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							W501_RPAS_LEVEL_4_BUZ=0;
						}*/
					}
				}
				else
				{
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
					W501_RPAS_LEVEL_4_BUZ=0;
				}
			}
			else
			{
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
				}

				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
				W501_RPAS_LEVEL_4_BUZ=0;
			}
		}
		else
		{
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
			W501_RPAS_LEVEL_4_BUZ=0;
		}
	}

	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X03*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 3)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							/*chime level_1*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_1_BUZ=0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_1_BUZ=0;
						}
						/*chime level_2*/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_2_BUZ=0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_2_BUZ=0;
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_3_BUZ=0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_3_BUZ=0;
						}
						/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							//	W501_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RL.byte==0 ||( four_bit_data_RL.byte >3 && four_bit_data_RL.byte <= 0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							//	W501_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RR.byte==0 ||( four_bit_data_RR.byte >3 && four_bit_data_RR.byte <= 0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							//	W501_RPAS_LEVEL_4_BUZ=0;
						}*/
					}
				}
				else
				{
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
				}
			}
			else
			{
				if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;
								}
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
			}
		}
		else
		{
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
		}
	}
	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X04*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 4)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							/*chime level_1*/
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_RPAS_LEVEL_1_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_1_BUZ=0;
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_1_BUZ=0;
						}
						/*chime level_2*/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_RPAS_LEVEL_2_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_2_BUZ=0;
								if(Sound_Bit == 0x20000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_2_BUZ=0;
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_RPAS_LEVEL_3_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_3_BUZ=0;
								if(Sound_Bit == 0x30000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_3_BUZ=0;
						}
						/*chime level_3*/
						if(four_bit_data_RLC.byte==4 || four_bit_data_RL.byte==4 || four_bit_data_RR.byte==4)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x40000;
								W501_RPAS_LEVEL_4_BUZ=1;


							}
							else
							{
								W501_RPAS_LEVEL_4_BUZ=0;
								if(Sound_Bit == 0x40000)
								{
									Sound_Bit = 0x0;
								}
							}
						}
						else
						{
							W501_RPAS_LEVEL_4_BUZ=0;
						}
						/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							W501_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RL.byte==0 ||( four_bit_data_RL.byte >3 && four_bit_data_RL.byte <= 0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							W501_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RR.byte==0 ||( four_bit_data_RR.byte >3 && four_bit_data_RR.byte <= 0XF))
						{
							W501_RPAS_LEVEL_1_BUZ=0;
							W501_RPAS_LEVEL_2_BUZ=0;
							W501_RPAS_LEVEL_3_BUZ=0;
							W501_RPAS_LEVEL_4_BUZ=0;
						}*/
					}
				}
				else
				{
					W501_RPAS_LEVEL_1_BUZ=0;
					W501_RPAS_LEVEL_2_BUZ=0;
					W501_RPAS_LEVEL_3_BUZ=0;
					W501_RPAS_LEVEL_4_BUZ = 0;
				}
			}
			else
			{
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x20000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x30000)
				{
					Sound_Bit = 0x0;
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit = 0x0;
				}
				W501_RPAS_LEVEL_1_BUZ=0;
				W501_RPAS_LEVEL_2_BUZ=0;
				W501_RPAS_LEVEL_3_BUZ=0;
				W501_RPAS_LEVEL_4_BUZ = 0;
			}
		}
		else
		{
			W501_RPAS_LEVEL_1_BUZ=0;
			W501_RPAS_LEVEL_2_BUZ=0;
			W501_RPAS_LEVEL_3_BUZ=0;
			W501_RPAS_LEVEL_4_BUZ = 0;
		}
	}
#endif


#ifdef GRAYSCALE_TFT
#ifndef U350
	/*RPAS CHIMES-W501*/

	/*RPAS_FAIL_CHIME_FUNCTION*/

			if(Diag_RW_RPASS_Fail_Chime_Enable==1)
			{
				if(RPAS_Precondition_Active==1)
				{
					if(MBFM_PAS1_50_message_timeout_flag == 0)
					{
						five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
						five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
						five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
						five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
						five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

						if(five_bit_data.byte==0)
						{
							RPAS_ERROR_TONE=0;
							Buzz_RPAS_ERR_TONE_ctr=0;
							RPAS_ERROR_TONE_compl=0;
						}
						if(five_bit_data.byte!=0)
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000|| Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000) && W501_LOW_RPAS_ERROR_TONE_compl == 0)
							{
								Sound_Bit = 0x4000;
								W501_LOW_RPAS_ERROR_TONE=1;

								if(W501LOW_error_cnt == 0)
								{

									W501LOW_error_cnt = 1;
									Buzz_ON=0;
									W501_LOW_CounterBz_3sec = W501_LOW_DID_RPAS_ERROR_TONE_CHECK - 10;
								}
							}
							else
							{
								W501_LOW_RPAS_ERROR_TONE=0;
								W501LOW_error_cnt = 0;
							}

						}
						else
						{
							if(Sound_Bit == 0x4000)
							{
								Sound_Bit = 0x0;
							}

							W501_LOW_RPAS_ERROR_TONE=0;
							W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
							W501_LOW_RPAS_ERROR_TONE_compl=0;
							W501LOW_error_cnt = 0;
						}
					}
					else
					{
						if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x4000) && W501_LOW_RPAS_ERROR_TONE_compl == 0)
						{
							Sound_Bit = 0x4000;
							W501_LOW_RPAS_ERROR_TONE=1;
							if(W501LOW_error_cnt == 0)
							{

								W501LOW_error_cnt = 1;
								Buzz_ON=0;
								W501_LOW_CounterBz_3sec = W501_LOW_DID_RPAS_ERROR_TONE_CHECK - 10;
							}
						}
						else
						{
							W501_LOW_RPAS_ERROR_TONE=0;
							W501LOW_error_cnt = 0;
						}
					}

				}
				else
				{
					if(Sound_Bit == 0x4000)
					{
						Sound_Bit = 0x0;
					}

					W501_LOW_RPAS_ERROR_TONE=0;
					W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
					W501_LOW_RPAS_ERROR_TONE_compl=0;
				}
			}
			else
			{
				W501_LOW_RPAS_ERROR_TONE=0;
				W501_LOW_Buzz_RPAS_ERR_TONE_ctr=0;
				W501_LOW_RPAS_ERROR_TONE_compl=0;
			}


	/*RPAS self check tone*/
	if(Diag_RW_RPASS_Self_check_Chime_Enable==1)
	{
		if(RPAS_Precondition_Active==1)

		{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{

					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;


					if(five_bit_data.byte==0)
					{
						if((Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x8000) && W501_LOW_RPAS_SELF_CHECK_TONE_compl == 0)
						{
							Sound_Bit = 0x8000;
							W501_LOW_RPAS_SELF_CHECK_TONE=1;
							if(W501LOW_selfcheck_cnt == 0)
							{
								W501LOW_selfcheck_cnt = 1;
								Buzz_ON=0;
								W501_LOW_Counter_RPAS_500MS = W501_LOW_DID_RPAS_SELF_CHECK - 10;
							}
						}
						else
						{
							if(Sound_Bit == 0x8000)
							{
								Sound_Bit = 0x0;
							}
							W501_LOW_RPAS_SELF_CHECK_TONE=0;
							W501LOW_selfcheck_cnt = 0;
						}

					}
					else
					{
						if(Sound_Bit == 0x8000)
						{
							Sound_Bit = 0x0;
						}

						W501_LOW_RPAS_SELF_CHECK_TONE=0;
						W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
						W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
						W501LOW_selfcheck_cnt = 0;
					}

				}
				else
				{
					if(Sound_Bit == 0x8000)
								{
									Sound_Bit = 0x0;
								}
					W501_LOW_RPAS_SELF_CHECK_TONE=0;
					W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
					W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
				}

			}
			else
			{
				if(Sound_Bit == 0x8000)
				{
					Sound_Bit = 0x0;
				}

				W501_LOW_RPAS_SELF_CHECK_TONE=0;
				W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
				W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
			}
		}
		else
		{
			W501_LOW_RPAS_SELF_CHECK_TONE=0;
			W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr=0;
			W501_LOW_RPAS_SELF_CHECK_TONE_compl=0;
		}

			/******RPAS STATUS CHIME FUNCTIONAL LOGIC*******/
				/********RPAS_SECTOR_ZONE_CALLIBRATION----0X01*********/

				if(Diag_RW_RPAS_Sector_Zone_Calibration == 1)
				{
					if(Diag_RW_RPASS_Chime_Enable==1)
					{
						if(RPAS_Precondition_Active==1)
						{
							if(MBFM_PAS1_50_message_timeout_flag == 0)

							{
								five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
								five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
								five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
								five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
								five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

								if(five_bit_data.byte==0)
								{

									four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
									four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
									four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
									four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

									four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
									four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
									four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
									four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;

									/*chime level_1*/
									if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
									{
										if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
										{
											Sound_Bit = 0x10000;
											W501_LOW_RPAS_LEVEL_1_BUZ=1;
											W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

										}
										else
										{
											W501_LOW_RPAS_LEVEL_1_BUZ=0;
										}

									}
									else
									{
										if(Sound_Bit == 0x10000)
										{
											Sound_Bit=0x0;
											W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
										}

										W501_LOW_RPAS_LEVEL_1_BUZ=0;

									}

									/*******LEVEL2**********/
									if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
									{
										if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
										{
											Sound_Bit = 0x20000;
											W501_LOW_RPAS_LEVEL_2_BUZ=1;
											W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
											W501_LOW_priority_lv2=1;
										}
									}
									else
									{
										if(Sound_Bit == 0x20000)
										{
											Sound_Bit=0x0;
											W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
										}
										W501_LOW_RPAS_LEVEL_2_BUZ=0;
										//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
										W501_LOW_priority_lv2=0;
									}

									/*LEVEL3*/
									if((four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)&&W501_LOW_priority_lv2==0)
									{

										if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
										{
											Sound_Bit = 0x30000;
											W501_LOW_RPAS_LEVEL_3_BUZ=1;
											W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
											W501_LOW_priority_lv3=1;
										}

									}
									else
									{
										if(Sound_Bit == 0x30000)
										{
											Sound_Bit=0x0;
											W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
										}
										W501_LOW_RPAS_LEVEL_3_BUZ=0;

										W501_LOW_priority_lv3=0;
									}
									/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
									{
										W501_LOW_RPAS_LEVEL_1_BUZ=0;
										W501_LOW_RPAS_LEVEL_2_BUZ=0;
										W501_LOW_RPAS_LEVEL_3_BUZ=0;
									}
									if(four_bit_data_RRC.byte==0 ||( four_bit_data_RRC.byte >3 && four_bit_data_RRC.byte <= 0XF))
									{
										W501_LOW_RPAS_LEVEL_1_BUZ=0;
										W501_LOW_RPAS_LEVEL_2_BUZ=0;
										W501_LOW_RPAS_LEVEL_3_BUZ=0;
									}*/
								}
								else
								{
									if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_1_BUZ = 0;
									}
									else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_2_BUZ = 0;
									}
									else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_3_BUZ = 0;
									}
									else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
									{
										W501_LOW_RPAS_LEVEL_4_BUZ = 0;
									}
								}
							}
							else
							{
								if(Sound_Bit == 0x10000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
								}

								if(Sound_Bit == 0x20000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								}

								if(Sound_Bit == 0x30000)
								{
									Sound_Bit=0x0;
									W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								}
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
								W501_LOW_RPAS_LEVEL_2_BUZ=0;
								W501_LOW_RPAS_LEVEL_3_BUZ=0;
							}
						}
						else
						{
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}

							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}

							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}

							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
						}
					}
					else
					{
						W501_LOW_RPAS_LEVEL_1_BUZ=0;
						W501_LOW_RPAS_LEVEL_2_BUZ=0;
						W501_LOW_RPAS_LEVEL_3_BUZ=0;
					}
				}

	/********RPAS_SECTOR_ZONE_CALLIBRATION----0X02*********/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 2)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RRC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_5;
						four_bit_data_RRC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_6;
						four_bit_data_RRC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_7;
						four_bit_data_RRC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_8;

						/*chime level_1*/
						if(four_bit_data_RLC.byte==1 || four_bit_data_RRC.byte==1)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_LOW_RPAS_LEVEL_1_BUZ=1;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}

							W501_LOW_RPAS_LEVEL_1_BUZ=0;

						}

						/*******LEVEL2**********/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RRC.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_LOW_RPAS_LEVEL_2_BUZ=1;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								W501_LOW_priority_lv2=1;
							}
						}
						else
						{
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
							W501_LOW_priority_lv2=0;
						}

						/*LEVEL3*/
						if((four_bit_data_RLC.byte==3 || four_bit_data_RRC.byte==3)&&(W501_LOW_priority_lv2==0))
						{

							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_LOW_RPAS_LEVEL_3_BUZ=1;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								W501_LOW_priority_lv3=1;
							}

						}
						else
						{
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_3_BUZ=0;

							W501_LOW_priority_lv3=0;
						}

						/*LEVEL4*/

						/*chime level_4*/
						if((four_bit_data_RLC.byte==4 || four_bit_data_RRC.byte==4)&&(W501_LOW_priority_lv2==0)&&(W501_LOW_priority_lv3==0))
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x40000;
								W501_LOW_RPAS_LEVEL_4_BUZ=1;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
							//RPAS_LEVEL_4_TONE_ctr=0;
						}

					/*	if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RRC.byte==0 ||( four_bit_data_RRC.byte >3 && four_bit_data_RRC.byte <= 0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}*/
					}
					else
					{
						if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_1_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_2_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_3_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_4_BUZ = 0;
						}
					}
				}
				else
				{
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
					}

					if(Sound_Bit == 0x20000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
					}

					if(Sound_Bit == 0x30000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
					}

					if(Sound_Bit == 0x40000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
					}
					W501_LOW_RPAS_LEVEL_1_BUZ=0;
					W501_LOW_RPAS_LEVEL_2_BUZ=0;
					W501_LOW_RPAS_LEVEL_3_BUZ=0;
					W501_LOW_RPAS_LEVEL_4_BUZ=0;
				}
			}
			else
			{
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
				}

				if(Sound_Bit == 0x20000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
				}

				if(Sound_Bit == 0x30000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
				}

				if(Sound_Bit == 0x40000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
				}

				W501_LOW_RPAS_LEVEL_1_BUZ=0;
				W501_LOW_RPAS_LEVEL_2_BUZ=0;
				W501_LOW_RPAS_LEVEL_3_BUZ=0;
				W501_LOW_RPAS_LEVEL_4_BUZ=0;
			}
		}
		else
		{
			W501_LOW_RPAS_LEVEL_1_BUZ=0;
			W501_LOW_RPAS_LEVEL_2_BUZ=0;
			W501_LOW_RPAS_LEVEL_3_BUZ=0;
			W501_LOW_RPAS_LEVEL_4_BUZ=0;
		}
	}

	/********/
	if(Diag_RW_RPAS_Sector_Zone_Calibration == 3)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						/*chime level_1*/
						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_LOW_RPAS_LEVEL_1_BUZ=1;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}

							W501_LOW_RPAS_LEVEL_1_BUZ=0;

						}

						/*******LEVEL2**********/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_LOW_RPAS_LEVEL_2_BUZ=1;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								W501_LOW_priority_lv2=1;
							}
						}
						else
						{
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
							W501_LOW_priority_lv2=0;
						}

						/*LEVEL3*/
						if((four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)&&(W501_LOW_priority_lv2==0))
						{

							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_LOW_RPAS_LEVEL_3_BUZ=1;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								W501_LOW_priority_lv3=1;
							}

						}
						else
						{
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_3_BUZ=0;

							W501_LOW_priority_lv3=0;
						}
						/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							//W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RL.byte==0 ||( four_bit_data_RL.byte >3 && four_bit_data_RL.byte <= 0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							//W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RR.byte==0 ||( four_bit_data_RR.byte >3 && four_bit_data_RR.byte <= 0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							//W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}*/
					}
					else
					{
						if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_1_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_2_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_3_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_4_BUZ = 0;
						}
					}
				}
				else
				{
					if(Sound_Bit == 0x10000)
									{
										Sound_Bit=0x0;
										W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
									}

									if(Sound_Bit == 0x20000)
									{
										Sound_Bit=0x0;
										W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
									}

									if(Sound_Bit == 0x30000)
									{
										Sound_Bit=0x0;
										W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
									}
					W501_LOW_RPAS_LEVEL_1_BUZ=0;
					W501_LOW_RPAS_LEVEL_2_BUZ=0;
					W501_LOW_RPAS_LEVEL_3_BUZ=0;
				}
			}
			else
			{
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
				}

				if(Sound_Bit == 0x20000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
				}

				if(Sound_Bit == 0x30000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
				}

				W501_LOW_RPAS_LEVEL_1_BUZ=0;
				W501_LOW_RPAS_LEVEL_2_BUZ=0;
				W501_LOW_RPAS_LEVEL_3_BUZ=0;
			}
		}
		else
		{
			W501_LOW_RPAS_LEVEL_1_BUZ=0;
			W501_LOW_RPAS_LEVEL_2_BUZ=0;
			W501_LOW_RPAS_LEVEL_3_BUZ=0;
		}
	}
/***********************/

	if(Diag_RW_RPAS_Sector_Zone_Calibration == 4)
	{
		if(Diag_RW_RPASS_Chime_Enable==1)
		{
			if(RPAS_Precondition_Active==1)
			{
				if(MBFM_PAS1_50_message_timeout_flag == 0)

				{
					five_bit_data.bits.Rx_1 = CAN_MBFM_PAS1_50[4].bits.Rx_1;
					five_bit_data.bits.Rx_2 = CAN_MBFM_PAS1_50[4].bits.Rx_2;
					five_bit_data.bits.Rx_3 = CAN_MBFM_PAS1_50[4].bits.Rx_3;
					five_bit_data.bits.Rx_4 = CAN_MBFM_PAS1_50[4].bits.Rx_4;
					five_bit_data.bits.Rx_5 = CAN_MBFM_PAS1_50[4].bits.Rx_5;

					if(five_bit_data.byte==0)
					{

						four_bit_data_RLC.bits.Rx_1 = CAN_MBFM_PAS1_50[6].bits.Rx_1;
						four_bit_data_RLC.bits.Rx_2 = CAN_MBFM_PAS1_50[6].bits.Rx_2;
						four_bit_data_RLC.bits.Rx_3 = CAN_MBFM_PAS1_50[6].bits.Rx_3;
						four_bit_data_RLC.bits.Rx_4 = CAN_MBFM_PAS1_50[6].bits.Rx_4;

						four_bit_data_RL.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_1;
						four_bit_data_RL.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_2;
						four_bit_data_RL.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_3;
						four_bit_data_RL.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_4;

						four_bit_data_RR.bits.Rx_1 = CAN_MBFM_PAS1_50[5].bits.Rx_5;
						four_bit_data_RR.bits.Rx_2 = CAN_MBFM_PAS1_50[5].bits.Rx_6;
						four_bit_data_RR.bits.Rx_3 = CAN_MBFM_PAS1_50[5].bits.Rx_7;
						four_bit_data_RR.bits.Rx_4 = CAN_MBFM_PAS1_50[5].bits.Rx_8;

						/*chime level_1*/
						if(four_bit_data_RLC.byte==1 || four_bit_data_RL.byte==1 || four_bit_data_RR.byte==1)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x10000|| Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x10000;
								W501_LOW_RPAS_LEVEL_1_BUZ=1;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;

							}
							else
							{
								W501_LOW_RPAS_LEVEL_1_BUZ=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x10000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
							}

							W501_LOW_RPAS_LEVEL_1_BUZ=0;

						}

						/*******LEVEL2**********/
						if(four_bit_data_RLC.byte==2 || four_bit_data_RL.byte==2 || four_bit_data_RR.byte==2)
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x20000|| Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x20000;
								W501_LOW_RPAS_LEVEL_2_BUZ=1;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
								W501_LOW_priority_lv2=1;
							}
						}
						else
						{
							if(Sound_Bit == 0x20000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							//W501_LOW_RPAS_LEVEL_2_TONE_ctr=0;
							W501_LOW_priority_lv2=0;
						}

						/*LEVEL3*/
						if((four_bit_data_RLC.byte==3 || four_bit_data_RL.byte==3 || four_bit_data_RR.byte==3)&&(W501_LOW_priority_lv2==0))
						{

							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x30000|| Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x30000;
								W501_LOW_RPAS_LEVEL_3_BUZ=1;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
								W501_LOW_priority_lv3=1;
							}

						}
						else
						{
							if(Sound_Bit == 0x30000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_3_BUZ=0;

							W501_LOW_priority_lv3=0;
						}


						/*chime level_4*/
						if((four_bit_data_RLC.byte==4 || four_bit_data_RL.byte==4 || four_bit_data_RR.byte==4)&&(W501_LOW_priority_lv2==0)&&(W501_LOW_priority_lv3==0))
						{
							if(Sound_Bit == 0x0 || Sound_Bit == 0x200 || Sound_Bit == 0x200 || Sound_Bit == 0x40000)
							{
								Sound_Bit = 0x40000;
								W501_LOW_RPAS_LEVEL_4_BUZ=1;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}

						}
						else
						{
							if(Sound_Bit == 0x40000)
							{
								Sound_Bit=0x0;
								W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
							}
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
							//RPAS_LEVEL_4_TONE_ctr=0;
						}
						/*if(four_bit_data_RLC.byte==0 ||( four_bit_data_RLC.byte >3 && four_bit_data_RLC.byte <0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RL.byte==0 ||( four_bit_data_RL.byte >3 && four_bit_data_RL.byte <= 0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}
						if(four_bit_data_RR.byte==0 ||( four_bit_data_RR.byte >3 && four_bit_data_RR.byte <= 0XF))
						{
							W501_LOW_RPAS_LEVEL_1_BUZ=0;
							W501_LOW_RPAS_LEVEL_2_BUZ=0;
							W501_LOW_RPAS_LEVEL_3_BUZ=0;
							W501_LOW_RPAS_LEVEL_4_BUZ=0;
						}*/
					}
					else
					{
						if(W501_LOW_RPAS_LEVEL_1_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_1_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_2_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_2_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_3_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_3_BUZ = 0;
						}
						else if(W501_LOW_RPAS_LEVEL_4_BUZ == 1)
						{
							W501_LOW_RPAS_LEVEL_4_BUZ = 0;
						}
					}
				}
				else
				{
					if(Sound_Bit == 0x10000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
					}

					if(Sound_Bit == 0x20000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
					}

					if(Sound_Bit == 0x30000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
					}
					if(Sound_Bit == 0x40000)
					{
						Sound_Bit=0x0;
						W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
					}
					W501_LOW_RPAS_LEVEL_1_BUZ=0;
					W501_LOW_RPAS_LEVEL_2_BUZ=0;
					W501_LOW_RPAS_LEVEL_3_BUZ=0;
					W501_LOW_RPAS_LEVEL_4_BUZ=0;
				}
			}
			else
			{
				if(Sound_Bit == 0x10000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_1_TONE_compl=0;
				}

				if(Sound_Bit == 0x20000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_2_TONE_compl=0;
				}

				if(Sound_Bit == 0x30000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_3_TONE_compl=0;
				}
				if(Sound_Bit == 0x40000)
				{
					Sound_Bit=0x0;
					W501_LOW_RPAS_LEVEL_4_TONE_compl=0;
				}
				W501_LOW_RPAS_LEVEL_1_BUZ=0;
				W501_LOW_RPAS_LEVEL_2_BUZ=0;
				W501_LOW_RPAS_LEVEL_3_BUZ=0;
				W501_LOW_RPAS_LEVEL_4_BUZ=0;
			}
		}
		else
		{
			W501_LOW_RPAS_LEVEL_1_BUZ=0;
			W501_LOW_RPAS_LEVEL_2_BUZ=0;
			W501_LOW_RPAS_LEVEL_3_BUZ=0;
			W501_LOW_RPAS_LEVEL_4_BUZ=0;
		}
	}
#endif
#endif

}
