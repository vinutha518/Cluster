

#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "dr7f701461.dvf.h"

#include "r_bsp_api.h"         /* Board support package */
#include "sound_pcm.h"
#include "UDSS.h"
#include "my_can.h"
#include "r_typedefs.h"
#include "sound_pcm.h"
#include "r_dev_api.h"
#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"
#include "TLC6C_S_P.h"
#include "r_gpio_api.h"
/*#include "r_taub_isr.h"*/
/*#include "r_taub_main.h"*/
#include "r_taub_api.h"
#include "r_tauj_api.h"
/*#include "r_taub_sys.h"*/
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "init.h"
#include "swsc.h"
#include "digitalGuage.h"
#include "tachometer.h"
#include "drome.h"
#include "druserconf.h"
#include "my_can.h"
#include "fuel.h"
#include "parallel_to_serial.h"
#include "serial_to_parallel.h"
#include "r_test_ism.h"
#include "alert.h"
#include "AFE_3.h"                  /* Model's header file */
#include "IFE_3_Code.h"
#include "DTE_V_1.h"
#include "VIN_MISMATCH.h"
//#include "rtwtypes.h"
//#include "rtwtypes1.h"
#include "batt.h"
#include "Memmory_Map.h"
#include "gui.h"
#include "r_cg_serial_user.h"
#include "IVN_SecurityAlgorithm.h"
#include "ADC.h"
#include "UDS_DTC.h"
#include "gearIndication.h"
#include "sound_pcm.h"
#ifdef SPRINT4
#include "TDMS.h"
#endif
/************** W501 ***********************/

static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
	}bits;
	unsigned char byte;

}three_bit_AFE;


uint16_t gcsk_flag;
uint16_t timer_flag;

#ifdef GRAYSCALE_TFT
#define Welcome_indexrate 140
#else
#define Welcome_indexrate 220
#endif

void my_tau0_channel0_interrupt(void);
void CAN_message_Timeout(void);

_Bool flag_DTE_Enable;
unsigned short P2CAN_ECU_Cntr;
uint64_t S3ECU_cntr,NBs_cntr;
uint64_t first_screen_timeout=0;
uint64_t start_alart_timeout_5sec=0;
uint64_t start_alert_timeout_1sec = 0;
uint64_t Settingmenu_timeout_10sec = 0;
uint64_t BS6_alert_timeout_4sec = 0;
#ifdef COLOR_TFT
uint64_t start_alart_timeout_5sec_MAX=46000;//50000
#else
uint64_t start_alart_timeout_5sec_MAX=36000;//50000
#endif

#ifdef GRAYSCALE_TFT
uint64_t start_setting_timeout_10sec_MAX=73000;
#else
uint64_t start_setting_timeout_10sec_MAX=87000;
#endif

uint64_t start_alart_timeout_1sec_MAX=10000;
uint64_t start_alart_timeout_4sec_MAX=28800;//40000
uint64_t regen_req_page_timeout=0;
uint8_t regen_req_page_timeout_reached=0;
uint8_t dont_throw_alerts_alert_on_screen_present=0;
uint8_t dont_throw_doorclose_on_screen_present = 0;
uint8_t first_screen_timeout_start=0;
uint8_t regen_page_active=0;
uint8_t regen_page_started=0;
uint8_t regen_page_notstarted=0;
uint8_t regen_reqpage_to_notstarted = 0;
//ETM
uint64_t ETM_test_start_timeout = 0;
uint64_t ETM_test_start_timeout_max = 15000;//1.5 sec
_Bool set_key_short = 0;
_Bool Up_key_Short = 0;
_Bool Down_key_Short = 0;
_Bool MODE_BUTTON_STUCK_FLAG = 0;
_Bool SET_BUTTON_STUCK_FLAG = 0;

uint8_t IGNOFF_display_timeout = 0;
uint8_t two_sec_count = 0;//VIN Mismatch.
uint8_t  U8_Reset_DTE = 0U;
_Bool DTE_Update = 0;//Update DTE after Fuel fill in IGN off condition and then IGN on
uint8_t  U8_Reset_AFE = 0U;

unsigned short Counter_1S;
unsigned short Counter_25mS;
unsigned short Counter_15mS;
unsigned int Counter_100ms;
uint16_t Counter_100milis;
unsigned short Counter_200mS;
unsigned int Counter_500ms;
unsigned int Counter_250ms;
unsigned int CounterTT_1000ms_on;
unsigned int CounterTT_1000ms_off;
unsigned int CounterTT_500ms_TPMS_on;
unsigned int CounterTT_500ms_TPMS_off;

unsigned int CounterTT_500ms_on;
unsigned int CounterTT_500ms_off;
unsigned int CounterTT_70ms;
unsigned int CounterTT_150ms;
unsigned int CounterTT_90ms;
unsigned int CounterTT_1sec;
unsigned int CounterTT_90sec;
unsigned int CounterTTP_50ms;
unsigned int CounterTT_250ms;
unsigned int CounterTTP_1000ms;
unsigned int CounterTTD_250ms;
unsigned int Counter_Gear_250ms;
unsigned int CounterTT_10ms;
unsigned int counterLRBuzz_4ms;
unsigned int counterLR_4ms=0;
unsigned char turn_activate=0;
unsigned int CounterTT_15ms;
unsigned int CounterADC_100ms;
unsigned int CounterTT_100ms;
unsigned int CounterBz_500ms;
unsigned int CounterosBz_757ms;
unsigned int CounterTT_3sec;
unsigned int Counterblink_500ms;
unsigned int IGNOFF_Counterblink_500ms;
unsigned int Counter_IMMO_40ms;
unsigned int Counter_IMMO_1000ms;
unsigned int Counter_IMMO_80ms;
unsigned int Counter_IMMO_2000ms;
unsigned char row=0;
unsigned char col=0;
unsigned int BootCounterTT_10ms;
unsigned int two_sec_fuel_cnt;

unsigned int AVS_DT_counter_sec;
uint64_t TPMS_ENTER_CONT = 0;
//#ifdef GRAYSCALE_TFT
uint8_t flag_grayscale;
uint8_t refresh_rate;
extern _Bool RPAS_Error_flag;/*to resolve bug id 21 this flag added*/
uint8_t sound_rpas_fail_once = 0;
//#endif
/****************chime counters*****************************************/
unsigned int counter757ms;
unsigned int pattern2_counter50ms;
unsigned int message_counter1sec;
unsigned int reverse_counter100msec;
unsigned int Eng_runn_5sec;
unsigned int Overspdl1_counter60sec;
unsigned int tick_tock_counter30msec;
uint8_t reverse_cnt = 13;
/***********************************************************************/
uint8_t welcomecounter = 0;
uint8_t welcome_index = 0;
uint8_t scan_rate = 0;
/***********************************************************************/
uint8_t dooropencounter = 0;
uint16_t ignOFF_dooropencounter = 0;
uint8_t decrement_trend = 0;
uint8_t wait_10sec = 0;
unsigned int Regen_10seccnt;
uint8_t regen_req_start = 0;
/***********************************************************************/
extern _Bool Drive_summery_tout_flag;
extern _Bool Drive_summery_tout_flag1;
uint64_t start_driveSum_timeout_3sec = 0;
_Bool write_eeprom_flag = 0;
uint64_t start_driveSum_timeout_3sec_MAX=30000;
/***********************************************************************/
//Buzz
uint8_t Counter_50mS=0;
uint8_t on_counter=0;
uint16_t key_press_counter_tpms =0;

extern uint8_t key_press_bfr_ign;
unsigned short Counter4wdTT_500ms;
unsigned short Counter4wdTT_1500ms;
unsigned short Counter4wdTT_1000ms;
unsigned short Counter4wdTT2_1000ms;
unsigned short Counter4wdTT_10s;

uint32_t Counter_1000ms;
uint8_t Counter_1ms;
unsigned short Counter_3Sec,Counter_5Sec;
unsigned short P2CAN_ECU_TIMEOUT = 500;  /* 50ms/100us */
uint64_t S3ECU_TIMEOUT=50000;
uint64_t NBs_TIMEOUT=10000;
#ifdef U350
uint64_t first_screen_timeout_MAX=20000;
#else
uint64_t first_screen_timeout_MAX=20000;//19000;//20000;//22000;
#endif
uint64_t regen_req_page_timeout_MAX=100000;
uint8_t Counter_600uS;
static unsigned short cntr_100us;
unsigned int Counter_3sec;
uint64_t Counter_IC1_100;
uint64_t Counter_IC_CAN_tx;
uint8_t cntr_IC7_1000_TEST;
uint8_t IC10_200_periodicity;
_Bool zpd_done=0;
_Bool Update_ign_Flag;
_Bool REQUEST_3SEC;
_Bool REQUEST_5SEC;
_Bool UDS_TASK;
_Bool Update_FStepper_Flag;
_Bool Read_ADC_Flag;
_Bool Read_TT_flag;
_Bool Counter_1000ms_flag;
_Bool Counter_500ms_popup;
_Bool WatchDog_flag;
_Bool telltale_blink_flag;
_Bool Flag_4WHTT_500ms_start=0;
_Bool Flag_4WHTT_1000ms_start=0;
_Bool Flag_4WHTT2_1000ms_start=0;
_Bool TT4WH_500ms_cmplt_flg;
_Bool Flag_4WHTT_1500ms_start=0;
_Bool TT4WH_1500ms_cmplt_flg;
_Bool TT4WH_1000ms_cmplt_flg;
_Bool TT24WH_1000ms_cmplt_flg;
_Bool Flag_4WHTT_10s_start=0;
_Bool TT4WH_10s_cmplt_flg;

//Buzzer
extern _Bool Buzz_High_Temp;
extern uint8_t Buzz_High_Temp_cntr;
extern _Bool Buzz_High_Temp_compl;
extern _Bool Buzz_Low_Eng_oilPre;
extern uint8_t Buzz_Low_Eng_oilPre_cntr;
extern _Bool Buzz_Low_Eng_oilPre_compl;
extern _Bool Buzz_Low_Brk_fld;
extern uint8_t Buzz_Low_Brk_fld_cntr;
extern _Bool Buzz_Low_Brk_fld_compl;
extern _Bool Buzz_Park_brake;
extern uint8_t Buzz_Park_brake_cntr;
extern _Bool Buzz_Park_brake_compl;
extern _Bool Buzz_Door_open;
extern uint8_t Buzz_Door_open_cntr;
extern _Bool Buzz_Door_open_compl;
extern _Bool Buzz_Low_fuel;
extern uint8_t Buzz_Low_fuel_cntr;
extern _Bool Buzz_Low_fuel_compl;
extern _Bool Buzz_Turn_LR;
extern uint8_t Buzz_Turn_LR_cntr;
extern _Bool Buzz_Turn_LR_compl;
extern _Bool Buzz_Rev_Alrm;
extern uint8_t Buzz_Rev_Alrm_cntr;
extern _Bool Buzz_Rev_Alrm_compl;
extern _Bool Buzz_SBR_lv2;
extern uint8_t Buzz_SBR_lv2_cntr;
extern _Bool Buzz_SBR_lv2_compl;
extern _Bool Buzz_rever_gr;
extern uint8_t Buzz_rever_gr_cntr;
extern _Bool Buzz_rever_gr_compl;
extern uint8_t BATT_DISCONNECTION;

extern _Bool Buzz_WIF;
extern uint8_t Buzz_WIF_cntr;
extern _Bool Buzz_WIF_compl;
extern _Bool Buzz_OS_L1;
extern _Bool Buzz_OS_L2;
extern uint8_t Buzz_OS_cntr;
extern _Bool Buzz_OS_compl;
uint32_T Buzz_OS_off_tmr;
uint32_t CounterBz_3sec;
_Bool RPAS_LEVEL1_PRECON;
extern _Bool RPAS_ERROR_TONE;
extern _Bool RPAS_ERROR_TONE_compl;
uint8_t Buzz_RPAS_ERR_TONE_ctr;
extern _Bool RPAS_LEVEL_1_BUZ;
extern _Bool RPAS_LEVEL_1_TONE_compl;
extern _Bool RPAS_LEVEL_2_BUZ;
extern _Bool RPAS_LEVEL_2_TONE_compl;
extern _Bool RPAS_LEVEL_3_BUZ;
extern _Bool RPAS_LEVEL_3_TONE_compl;
extern _Bool RPAS_LEVEL_4_BUZ;
extern _Bool RPAS_LEVEL_4_TONE_compl;

extern _Bool Buzz_KIR_compl;
extern uint8_t Buzz_KIR_cntr;

extern _Bool buzzParkLamp;			/* @@@ Park Lamp Buzz Status */
extern uint8_t buzzParkLampCntr;	/* @@@ Park Lamp Buzz Counter */
extern _Bool EEPROM_readwrite_para_started;

uint8_t RPAS_LEVEL_1_TONE_ctr;
uint8_t RPAS_LEVEL_2_TONE_ctr;
uint8_t RPAS_LEVEL_3_TONE_ctr;
uint8_t RPAS_LEVEL_4_TONE_ctr;
uint8_t RPAS_COUNTER3;
uint8_t RPAS_130_CTR;
uint16_t RPAS_430_CTR;
uint32_t Counter_RPAS_80MS;
uint32_t Counter_RPAS_30MS;
uint32_t Counter_RPAS_500MS;
_Bool U350_RPAS_ERROR_TONE_ONGOING;
_Bool RPAS_LEVEL2_BUZ_ONGOING;
_Bool RPAS_LEVEL3_BUZ_ONGOING;
_Bool RPAS_LEVEL4_BUZ_ONGOING;
_Bool RPAS_SELF_BUZ_ONGOING;
_Bool W501_LOW_RPAS_SELF_BUZ_ONGOING;
extern _Bool RPAS_SELF_CHECK_TONE;
extern _Bool RPAS_SELF_CHECK_TONE_compl;
uint8_t Buzz_RPAS_SELFCHECK_TONE_ctr;
uint16_t DID_RPAS_SELF_CHECK;
uint32_t DID_RPAS_ERROR_TONE_CHECK;

/************************************/
extern _Bool Error_tone_Active;
extern _Bool RPAS_Present_flag;
extern _Bool RPAS_STAYBACK_FLAG;
/***********************************/
unsigned short telltale_blink_flag_patt3;
unsigned short telltale_blink_flag_patt4;
unsigned short telltale_blink_flag_patt7;
unsigned short telltale_blink_flag_patt8;
unsigned short telltale_blink_flag_patt9;
unsigned short telltale_blink_flag_patt10;
_Bool telltale_blink_bool3;
_Bool telltale_blink_bool7;
_Bool telltale_blink_bool4;
_Bool telltale_blink_bool5;
_Bool telltale_blink_at_1sec;
_Bool Timer_ON_3sec;
_Bool Counter_3sec_Flag;
extern _Bool ninty_sec_start_flg;
extern _Bool ninty_sec_cmplt_flg;
extern RX_DATA CAN_IC_NSM[8];

extern _Bool NVM_SERVICE_DONE_flag;
extern _Bool TPMS_ENTER_PR;
_Bool ok_modelong_press = 0;
_Bool lightonoff_modelong_press = 0;
extern _Bool AFE_start;
extern _Bool IMMO_TT_WAKEUP_FLAG;
extern _Bool TT_IGN_OFF;
/*************************************************/
extern uint8_t empty_blinkpattern_flag;
extern uint8_t full_blinkpattern_flag;
extern uint8_t toggle_fuel;
extern uint8_t toggle_temp;
extern uint8_t Temp_blinkpattern_flag;
extern uint8_t Temp_blinkpattern_flag_7;//blink 7 bars
extern uint8_t Temp_blinkpattern_flag_8;//blink 8 bars
extern uint8_t toggle_timedots;
extern uint32_t Sound_Bit;
extern uint8_t new_fuel_val;
extern uint8_t fuel_widget;
/*************************************************/
extern uint8_t shortckt__30sec_start;
extern uint8_t recovery__30sec_start;
extern uint8_t openckt__30sec_start;
extern uint8_t timer_30sec_completed;
unsigned int  shortckt__30sec = 0;
unsigned int  recovery__30sec = 0;
unsigned int  openckt__30sec = 0;
/*************************************************/
extern _Bool ModelRunCheck_flag; /*Check DTC calculate done */
/*************RPAS Variables********************/
uint16_t AT_Reversegear_timespan = 0;
uint8_t Stayback_timespan = 0;
_Bool AT_Reverse_detected = 0;
extern uint8_t BATT_DISCONNECTION1;
extern uint8_t bufferinit_flag;
extern uint8_t A_flag;
extern uint32_t dromecall_timeout;
/*************************************************/

#define IGN 	R_GPIO_PinRead(0,1)//	PORT_AWOP0_1  /* Ignition */
#define SET		1U
#define CLEAR		0U
#define COUNT_25MS	250U
#define COUNT_200MS	2000U
#define COUNT_1SEC	10000U
#define	COUNT_100uS	5U	//10U
#define COUNT_500MS 5000U
#define COUNT_250MS 2500U

#define ADC_LOOP_COUNTER  10
#define RPAS_ErrorTone_factor 0.7F

#ifdef COLOR_TFT
#define Eng_running_time 50000
#else
#define Eng_running_time 30000
#endif
/*******************************************/
/* ***************** Below lines added by VireshW for Fuel ****************** */
#if 1
uint64_t Counter_30sec = 0;

uint64_t Counter_10sec = 0;//r

uint64_t Counter_13sec = 0;

uint64_t Counter_13sec_ign = 0;

uint64_t Recovery_Counter_10sec = 0;

uint64_t Counter_13sec_fd = 0;//r

uint64_t Counter_05sec = 0;//r
uint64_t FF_Counter_05sec = 0;
//uint8_t  Counter_13sec_Flag = 0;
//uint8_t  Timer_ON_13sec     = 0;

uint64_t Counter_300msec       = 0;
uint64_t Counter_330msec		=0;
//uint8_t  ADC_Read_300msec_Flag = 0;
//uint8_t  Timer_ON_300msec      = 0;

uint64_t Counter_100msec  = 0;
//uint8_t  ADC_Read_Flag    = 0;
//uint8_t  Timer_ON_100msec = 0;
uint64_t Counter_10ms = 0;
uint64_t Counter_5min       = 0;
uint64_t Counter_5min_spd	= 0;

uint64_t Counter_Per_Liter  = 0;
uint64_t Counter_3min       = 0;
//uint8_t  Counter_5min_Flag  = 0;
//uint8_t  Timer_ON_5min      = 0;
/***************************************/
uint32_t Water_In_Fuel_Timer_count = 0;
uint32_t eng_start_Counter_5sec = 0;//EBD_alert_Counter_5sec = 0;
#endif
uint16_t Fuel_consm_rate;
_Bool Flag_AFE_reset_trigger;
_Bool Flag_AFE_reset;
_Bool ODO_write;
_Bool resetackTx=0;
_Bool resetAFE=0;
#if 0
//For SWSC
uint16_t  Counter_OK_KEY_100usec = 0;
uint8_t  OK_KEY_Flag            = 0;

uint16_t  Counter_UP_KEY_100usec = 0;
uint8_t  UP_KEY_Flag            = 0;

uint16_t  Counter_DOWN_KEY_100usec = 0;
uint8_t  DOWN_KEY_Flag            = 0;

uint16_t  Counter_MENU_KEY_100usec = 0;
uint8_t  MENU_KEY_Flag            = 0;

#endif
//extern U8 button_input;
extern _Bool ABS_Notification;
extern  U8  diagTTflag ;
uint16_t int_20ms_AFE=0;
uint8_t int_20ms_IFE=0;
uint8_t int_50ms_IFE=0;
uint8_t int_20ms_IVN=0;

uint8_t int_20ms_DTE=0;
extern uint16_t Proveout_Count;

//DTE
extern ExtU rtU;
extern ExtY rtY;

extern ExtU_AFE_3_T AFE_3_U;
extern ExtY_AFE_3_T AFE_3_Y;

extern uint16_t final_vechile_speed;
extern uint32_t final_odomter_distance;
extern _Bool LBF_3Sec_cntr_start;
extern _Bool LBF_3Sec_cntr_done;

SplitWord Fuel_scale;
Split32 FUEL_IN_IGN;
_Bool LVSD_SET_Flag=0;
_Bool Buzz_ON=0;
_Bool Turn_BuzzON = 0;
_Bool Enter_while_flag;

_Bool Enginerunningflag=0;
_Bool Enginerun_chm=0;
_Bool Low_fuel_ongoing;
_Bool chime_start1;
_Bool Sound_playing=0;
void RPAS_LV_Chime_Play(uint8_t level);
void RPAS_LV_Chime_Dispacher(void);
_Bool Low_engine_oil_pressure;
_Bool Seatbelt_ongoing;
_Bool High_temp_ongoing;
_Bool dooropen_ongoing;
_Bool Lowbrakefluid_ongoing;
_Bool dooropen_ongoing;
_Bool Parkbrake_ongoing;
_Bool Lowfuel_ongoing;
_Bool Reversegear_ongoing;
_Bool reverse_chm_on = 0;
_Bool level1_ongoing;
_Bool level2_ongoing;
_Bool WIF_ongoing;
_Bool Tick_ongoing;
_Bool Tock_ongoing;

_Bool Buzz_Tick;
_Bool Buzz_Tick_compl;

_Bool Buzz_Tock;
_Bool Buzz_Tock_compl;


_Bool EMS_message_Node_absent_flag;
_Bool ESC_message_Node_absent_flag;
_Bool IS_message_Node_absent_flag;
_Bool MBFM_message_Node_absent_flag;
_Bool TCU_message_Node_absent_flag;
_Bool PKE_message_Node_absent_flag;
_Bool SRS_message_Node_absent_flag;
_Bool RPAS_message_Node_absent_flag;
/**********************************************/
uint8_t cycle133_cntr=0;
uint8_t cycleHightemp_cntr = 0;
uint8_t cycleLowengineoil_cntr = 0;
uint8_t cycleLowbrakefluid_cntr = 0;
uint8_t cycledooropen_cntr = 0;
uint8_t cycleparkbrake_cntr = 0;
uint8_t cyclelowfuel_cntr = 0;
uint8_t cycleReversegear_cntr = 0;
uint8_t cyclemsgnote_cntr = 0;
unsigned int  overspeedlevel1_cntr = 0;
unsigned int  overspeedlevel2_cntr = 0;
uint8_t cycleWIF_cntr = 0;

/**********************************************/
uint8_t cycle133_completed;
uint8_t cycle_Hightemp_completed;
uint8_t cycle_Lowengineoil_completed;
uint8_t cycle_Lowbrakefluid_completed;
uint8_t cycle_dooropen_completed;
uint8_t cycle_parkbrake_completed;
uint8_t cycle_lowfuel_completed;
uint8_t cycle_Reversegear_completed;
uint8_t cycle_Overspdl1_completed;
uint8_t cycle_Overspdl2_completed;
uint8_t cycle_waterinfuel_completed;
uint8_t cycle_tick_completed;
uint8_t cycle_tock_completed;

/**********************************************/
uint8_t Seatbelt_missed = 0;
uint8_t Hightemp_missed = 0;
uint8_t Lowengineoil_missed = 0;
uint8_t Lowbrakefluid_missed = 0;
uint8_t Dooropen_missed = 0;
uint8_t parkbrake_missed = 0;
uint8_t lowfuel_missed = 0;
uint8_t Reversegear_missed = 0;
uint8_t Overspdl1_missed = 0;
uint8_t Overspdl2_missed = 0;
uint8_t WIF_missed = 0;
uint8_t u8_App_DTE_Reset = 0;
uint8_t u8_App_AFE_Reset = 0;
/**********************************************/
uint8_t OverSpeedLv1repeatcnt;
uint8_t OverSpeedLv2repeatcnt;
/**********************************************/
extern uint8_t OverSpeedLevel1Type;
extern uint8_t OverSpeedLv1RepeatIntrval;
extern uint8_t OverSpeedLevel2Type;
extern uint8_t OverSpeedLv2RepeatIntrval;
extern _Bool Next_Right_Turn;
/**********************************************/
uint16_t engRunCntr=0;
uint16_t LOP_7Sec_counter=0;
uint16_t LBF_3Sec_counter=0;
uint8_t counter3 =0;
uint8_t OS_counter3 =0;

uint8_t counter3ParkLamp = 0;	/* @@@ Park lamp Counter*/

uint8_t counter7 =0;
uint16_t CalAvgKmplAFE_GD;       /* '<Root>/U16_CalAvgKmplAFE_GD' */
uint32_t AccFuelConsp;           /* '<Root>/U32_AccFuelConsp' */
uint32_t DrvAccFuelConspml;      /* '<Root>/U32_AccFuelConsp' */
uint32_t AccDistAFE;             /* '<Root>/U32_AccDistAFE' */
uint32_t DrvAccFuelConsp;        /* '<Root>/U32_DrvAccFuelConsp' */
uint16_t AFEDispVal;             /* '<Root>/U16_AFEDispVal' */
Split16 AFEDispVal_EEPROM;
Split16 DTEDispVal_EEPROM;
uint16_t CalAvgKmplAFE;          /* '<Root>/U16_CalAvgKmplAFE' */
uint16_t CalAvgKmplAFEtx;        /* '<Root>/U16_CalAvgKmplAFE' */
uint16_t AvgKmplAFE_GD;          /* '<Root>/U16_AvgKmplAFE_GD'*/
_Bool  B_AFEResetACK;            /* '<Root>/B_AFEResetACK' */
uint32_t AccFuelConspReset;      /* '<Root>/U32_AccFuelConspReset' */
_Bool AFECalTimer;               /* '<Root>/b_AFECalTimer' */
uint32_t OdoDispReset;           /* '<Root>/U32_OdoDispReset' */
uint32_t OdoDispGDReset;         /* '<Root>/U32_OdoDispGDReset' */
uint32_t AccFuelConspGDReset;    /* '<Root>/U32_AccFuelConspGDReset' */
uint32_t AccFuelConspGD;         /* '<Root>/U32_AccFuelConspGD' */
uint32_t AccDistAFEGD;           /* '<Root>/U32_AccDistAFEGD' */

uint32_t WDT_Counter=0;

uint32_t AFE_rst_odo_dist = 0U;
uint32_t AFE_diff_odo_dist = 0U;

//IFE
uint32_t IFE_Out=0,IFE_Out_DISPLAY = 0, IFE_out_CAN = 0,IFE_out_CAN1 = 0, IFE_Out_DISPLAY1 = 0;

//SBR_CHIME
uint32_t prev_sbrchm_odo = 0U;
uint32_t current_sbrchm_odo = 0U;
_Bool rpm8000_arrived=0,SBR_chime_condition_arrived=0,spd_2km_arrived=0;
_Bool ODO_arrived=0;
_Bool Less_tn_thrhld_rpm = 0;
uint32_t one_MIN_rpm_counter;
uint32_t one_MIN_spd_counter;
_Bool SBR_Start = 0;
//IVN

_Bool IVN_TxMAC_Indicator;
uint8_t IVN_CANSignal_State;
uint8_t IVN_TransmitCANFrame;
Split32 IVN_MacTxBuffer;
_Bool IVN_LimpHome_flg;
Split32 IVN_Mac2TxBuffer;
_Bool IVN_FcErrorStatusBit;
_Bool IVN_NsmDTCStatus;

_Bool NotdisplayAFE  = 0U;	/*AFE issue related to IGN OFF-ON showing wrong value*/
_Bool AFE_signal_invalid = 0U;
extern _Bool Speed_Missing;
extern _Bool LOP_7Sec_cntr_start;
extern _Bool LOP_7Sec_cntr_done;
extern uint8_t Engine_Temp_High_popup_shown;
_Bool PROVEOUT_OK = 0;
extern uint8_t Diag_RW_Min_VehSpeed_Threshold;

extern uint16_t Diag_RW_IFE_Accum_Time;
extern uint16_t Diag_RW_IFE_Sample_Time;
extern uint8_t	Diag_RW_IFE_Sample_Size;
extern uint16_t Diag_RW_IFE_Display_Time;
extern uint8_t	Diag_RW_IFE_Units;
extern uint8_t	Diag_RW_IFE_Max;

extern float Fuel_Consp_Factor;
extern uint8_t AFE_CALC_INTERVAL;
extern uint8_t AFE_DISPLAY_INTERVAL;
extern uint8_t AFE_DISPLAY_FILTER_A;
extern uint16_t AFE_DISPLAY_FILTER_B;
extern uint8_t AFE_DRIVE_CYCLE_RESET_ENABLE;
extern uint8_t AFE_DRIVE_CYCLE_RESET_INTERVAL;
extern uint8_t AFE_INIT_THRESHOLD_DISTANCE;


extern uint8_t DTE_AFE_AVG_KM;
extern uint8_t DTE_DEFAULT_FE;
extern uint8_t DTE_DISPLAY_FILTER_C;
extern uint8_t DTE_DISPLAY_FILTER_B;
extern uint8_t DTE_DISPLAY_FILTER_A;
extern uint8_t DTE_AFE_MAX ;
extern uint8_t DTE_AFE_MIN;
extern uint8_t Diag_Unit_Selection ;
extern _Bool DTE_Signal_invalid;
//DTE
uint32_t DTE_Disp_Val;
float32_t DTE_RAFE_Avg_Val;
float32_t DTE_1km;
float32_t DTE_RAFE_Val;
uint16_t Display_DTE;

//IVN

_Bool IVN_RxMAC_Indicator=0;
Split32 IVN_SecurityKeyConst=0;
Split32 IVN_SecurityKey_1=0;
Split32 IVN_SecurityKey_2 = 0;
uint32_t IVN_EEFreshnessCounter = 0;
Split32 IVN_MacRxBuffer;
uint8_t IVN_RxNSMMsgArrive = 0;
_Bool IVN_SecurityEnableFlag = 0;
uint8_t IVN_RxSynchMsgArrive = 0;
uint8_t IVN_ConfigStepTime = 10;
uint32_t CounterSwitchms;
uint32_t CounterSwitchmsU;
uint16_t IVN_cfg_FCIncCntrTiTh_C = 0; 	/* XLX 010B 0  */
uint8_t IVN_cfg_FCDevTh_C = 0;			/* XLX 010B 1  */
uint8_t IVN_cfg_FcDifErrReptTh_C = 0;   /* XLX 010B 2  */
uint8_t IVN_cfg_FcErrReptInc_C = 0;		/* XLX 010B 3  */
uint8_t IVN_cfg_FcErrReptDec_C = 0;		/* XLX 010B 4  */
uint8_t IVN_cfg_FcErrHealDec_C = 0;		/* XLX 010B 5  */
uint8_t IVN_cfg_ResyncCntrTh_C = 0;		/* XLX 010B 6  */
uint16_t IVN_cfg_ResyncTiTh_C = 0;		/* XLX 010B 7  */
uint8_t IVN_cfg_MAC2ErrCntrTh_C = 0;	/* XLX 010B 8  */
uint16_t IVN_cfg_SynchWndTiTh_C = 0;  	/* XLX 010B 9  */
uint8_t IVN_cfg_SynchMsgTxCntr_C = 0;	/* XLX 010B 10  */
uint32_t key;
Split32 IVN_MAC0RxBuffer;
uint8_t int_100ms_IVN;
uint32_t U32_DTE_Step_Cntr;
/*ETM Entry*/
extern _Bool ETM_Mode_Enter_timer;
_Bool ETM_10_sec_Timer = 0;
_Bool ETM_30_sec_Timer = 0;
uint32_t ETM_30_sec_counter = 0;
uint32_t ETM_10_sec_counter = 0;
_Bool ETM_10_sec_exit_flag = 0;
_Bool ETM_30_sec_exit_flag = 0;
_Bool Arrowinit_flg = 0;

uint32_t ETM_Entry_timer;
_Bool mode_release = 0;
_Bool Down_release = 0;
uint8_t ETM_Mode_count = 0;
uint8_t Proveout_temp =0;
extern _Bool ADC_Timer1secStart;
extern uint32_t Diag_RW_Eng_Run_threshold_RPM;
extern uint8_t Diag_RW_Vehicle_Moving_threshold_time;
extern uint8_t Diag_RW_Eng_Run_threshold_time;
extern uint16_t Diag_RW_Vehicle_Moving_threshold_distance;

extern uint8_t Diag_RW_Final_chime_duration;
extern _Bool LVSD_SET_init;
_Bool GI_CONTROL_FLAG;
extern uint32_t display_tachometer_speed;
extern uint8_t animation_start;
extern uint32_t startkey;
//uint8_t EEPROM_LOG = 0; //for EEPROM Read
extern _Bool IMMO_IGN_OFF;
extern _Bool fuel_enable_tt;
uint8_t TURN_TT_TIMEOUT=0;
extern _Bool IGN_OFF_TT;
/************************************************************/
/*RPAS_W501*/
uint16_t W501_HIGH_DID_RPAS_ERROR_TONE_CHECK;
uint16_t W501_HIGH_DID_RPAS_SELF_CHECK;
extern _Bool RPASS_self_check_ongoing;
extern _Bool cycle_RPASS_SELF_CHECK_completed;
extern _Bool RPAS_SELF_CHECK_ENABLE;
unsigned int  RPAS_self_check_counter500msec;
uint8_t cycle_RPAS_self_check_cntr;
_Bool RPAS_self_check_missed;
_Bool W501_RPAS_LV1_PRECONDITION;
_Bool Error_soundstart = 0;
unsigned int RPAS_LV1_COUNTER=0;
unsigned int RPAS_LV2_COUNTER=0;
unsigned int RPAS_LV3_COUNTER=0;
unsigned int RPAS_LV4_COUNTER=0;
uint8_t LEVEL_CNT;
unsigned int RPAS_ERROR_COUNTER;
extern _Bool W501_RPAS_LEVEL_1_BUZ;
extern _Bool W501_RPAS_LEVEL_2_BUZ;
extern _Bool W501_RPAS_LEVEL_3_BUZ;
extern _Bool W501_RPAS_LEVEL_4_BUZ;


_Bool W501_RPAS_LV2_PRECONDITION;
uint8_t W501_RPAS_LV3_CNTR;

_Bool W501_RPAS_LV3_PRECONDITION;

uint8_t W501_RPAS_LV4_CNTR;
_Bool W501_RPAS_LV4_PRECONDITION;
extern _Bool w501_RPAS_FAIL_CHIME;

unsigned int W501_RPAS_FAIL_check_counter500msec;
uint8_t Errorcnt = 13;//9;
extern _Bool W501_RPASS_fail_check_ongoing;
extern _Bool W501_cycle_RPASS_FAIL_CHECK_completed;
extern _Bool RPAS_fail_check_missed;
uint8_t W501_cycle_RPAS_fail_check_cntr;
extern _Bool Dooropen_detected;
extern _Bool IGNOFF_Dooropen_detected;
extern _Bool Odo_reset_8sec_on;
extern Split32 Default_pagekey;
uint32_t Odo_reset_8sec_count;
uint8_t Odo_rst_count;
extern uint16_t AFEDispVal_init;
extern _Bool Batt_disconnect_AFE_RST;

extern _Bool previous_state_immobilizer_blink4;//Need to test
extern _Bool previous_state_immobilizer_blink5;//Need to test
extern _Bool drive_summery_start;
extern _Bool drive_summery_page_draw;
extern _Bool AT_gearalert_reverse_display;
uint32_t AVG_50KM_AFE; //For PVT
/* ************************************************************************** */
#ifdef GRAYSCALE_TFT
#ifndef U350

extern _Bool W501_LOW_RPAS_SELF_CHECK_TONE;
uint8_t W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr;
extern _Bool W501_LOW_RPAS_SELF_CHECK_TONE_compl;
uint16_t W501_LOW_DID_RPAS_SELF_CHECK;
uint32_t W501_LOW_Counter_RPAS_500MS;
/*RPAS_ERROR_TONE*/
extern _Bool W501_LOW_RPAS_ERROR_TONE_compl;
extern _Bool W501_LOW_RPAS_ERROR_TONE;
uint8_t W501_LOW_Buzz_RPAS_ERR_TONE_ctr;
uint16_t W501_LOW_DID_RPAS_ERROR_TONE_CHECK;
uint32_t W501_LOW_CounterBz_3sec;
_Bool W501_LOW_RPAS_ERROR_TONE_ONGOING;
/*LEVEL1*/
extern _Bool W501_LOW_RPAS_LEVEL_1_TONE_compl;
extern _Bool W501_LOW_RPAS_LEVEL_1_BUZ;
_Bool W501_LOW_RPAS_LEVEL1_PRECON = 0;
/*LEVEL2*/
uint32_t W501_LOW_Counter_RPAS_80MS;
uint8_t W501_LOW_RPAS_COUNTER3 = 0;

uint8_t W501_LOW_RPAS_COUNTER2 = 0;
_Bool W501_LOW_RPAS_LEVEL2_BUZ_ONGOING;
extern _Bool W501_LOW_RPAS_LEVEL_2_BUZ;
extern _Bool W501_LOW_RPAS_LEVEL_2_TONE_compl;
/*LEVEL3*/
_Bool W501_LOW_RPAS_LEVEL3_BUZ_ONGOING;
uint16_t W501_LOW_RPAS_130_CTR;
extern _Bool W501_LOW_RPAS_LEVEL_3_BUZ;
extern _Bool W501_LOW_RPAS_LEVEL_3_TONE_compl;
/*LEVEL4*/
_Bool W501_LOW_RPAS_LEVEL4_BUZ_ONGOING;
extern _Bool W501_LOW_RPAS_LEVEL_4_BUZ;
uint8_t W501_LOW_RPAS_COUNTER4 = 0;
uint16_t W501_LOW_RPAS_430_CTR;
extern _Bool W501_LOW_RPAS_LEVEL_4_TONE_compl;
#endif
#endif
extern void delay_time(uint16_t time_end);
uint16_t tt_out_cnt;
uint32_t ms_8_counter=0;
uint32_t raw_adc_count=0;
uint32_t FUEL_LEVEL_RAW_ADC;

uint16_t IGN_OFF_KIR_Counter = 0;//KIR = Key in Reminder

uint16_t IgnOFF_KIR_Counter;
uint16_t IGN_OFF_10ms;
uint8_t cycleKIR_cntr = 0;

uint16_t IgnOFF_ParkLamp_Counter = 0;	/*@@@*/
uint8_t cycleParkLamp_cntr = 0;		/*@@@*/
extern uint8_t priorityPark_Keyin;
#ifdef U350
uint8_t Proveout_down = 37;
#else
uint8_t Proveout_down = PROVEOUT_DOWN;
#endif
//uint16_t test_cnt = 1000;
//uint8_t test_cnt1 = 10;
#ifdef GRAYSCALE_TFT
uint8_t TPMS_TT_Blink_Duration = 63;//65
#else
uint8_t TPMS_TT_Blink_Duration = 90;
#endif

uint8_t SBR_Off_time = 0;
#define DTE_ACCRCY_FACT 0.71F

#ifdef COLOR_TFT
#define AFE_ACCRCY_FACT 0.75235F //0.893F	 //0.752347926F correction factor updated on 10-06-20
#else
#define AFE_ACCRCY_FACT 0.90845F//0.893F 	 //0.752347926F correction factor updated on 10-06-20
#endif
/********************************************************************************/
void taub_isr();
void taub1_isr();

void taub_isr_Master_TFT(void);
void taub_isr_Slave_TFT(void);

void taub_isr_Master_GCLK(void);
void taub_isr_Slave_GCLK(void);


void taub_grayscale_isr();
extern void loc_WriteImage_fast(uint32_t,uint32_t,uint16_t *ptrImage);
extern U16 buffer[];
extern uint8_t bufferinit_flag;

extern _Bool Read_odo_flag;
_Bool TX_ODO_Flag = 0;
extern _Bool IGNOFF_eeprom_read;
#ifdef GRAYSCALE_TFT
void taub2_isr();
void taub_isr_Master(void);
void taub_isr_Slave(void);

void taub_isr_Master()
{
	asm("nop");
}

void taub_isr_Slave()
{
	asm("nop");
}

void taub2_isr()
{
	asm("nop");
}
#endif

void taub_isr_Master_TFT()
{
	asm("nop");
}

void taub_isr_Slave_TFT()
{
	asm("nop");
}


void taub_isr_Master_GCLK()
{
	asm("nop");
}

void taub_isr_Slave_GCLK()
{
	asm("nop");
}


void taub_isr()
{
	my_tau0_channel0_interrupt();

	//asm("nop");

	if(((timer_flag == 0)||(timer_flag > 2000)) && (WatchDog_flag == 0))  /* Watchdog Timer clock pulse*/
	{
#ifdef PCBrevB
		R_GPIO_TogglePin(0,4);
#else
#ifdef DEBUG
#else
		R_GPIO_TogglePin(42,14);
#endif
#endif
		timer_flag=1;
	}
	timer_flag++;
	//WatchDog Timer
	if(WDT_Counter < 100000)
	{
		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
		WDT_Counter++;
	}

	//my_tau0_channel0_interrupt();  r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
	/*PORT_ISOPNOT16=0x02u;*/

#ifndef U350
	if((ETM_Mode_Enter_timer == 1)&&(logo_completed==1))
	{
		ETM_Entry_timer++;	//timer for 3 sec
		if(ETM_Entry_timer <= 20500)
		{
			if(((SWSC_ADC_value >= ADC_COUNT_DOWN_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_DOWN_KEY_OFF))&&(Down_release == 0))
			{
				Down_release = 1;
				ETM_Mode_count++;
			}
			else if(SWSC_ADC_value >= ADC_COUNT_OPEN_KEY_ON)
			{
				Down_release = 0;
			}
		}
		else
		{
			ETM_Mode_Enter_timer = 0;
		}
		if(ETM_Mode_count == 4/*3*/)
		{

			(void)drome_sendEventToDrome((U32)drome_InEvent_ETM_Entry);//ETM Entry point
			ETM_10_sec_Timer = 1;
			ETM_Mode_Enter_timer = 0;
			ETM_Mode_count = 0;//reset count
		}
	}
	else
	{

	}
#endif
#ifdef U350
	if((ETM_Mode_Enter_timer == 1)&&(logo_completed==1))
	{
		ETM_Entry_timer++;	//timer for 3 sec
		if(ETM_Entry_timer <= 20500)
		{
			if((R_GPIO_PinRead(17,4) == 0)&&(mode_release == 0))
			{
				mode_release = 1;
				ETM_Mode_count++;
			}
			else if(R_GPIO_PinRead(17,4) == 1)
			{
				mode_release = 0;
			}
		}
		else
		{
			ETM_Mode_Enter_timer = 0;
		}
		if(ETM_Mode_count == 4/*3*/)
		{

			(void)drome_sendEventToDrome((U32)drome_InEvent_ETM_Entry);//ETM Entry point
			ETM_10_sec_Timer = 1;
			ETM_Mode_Enter_timer = 0;
			ETM_Mode_count = 0;//reset count
		}
	}
	else
	{

	}

	if((LVSD_flag == 0U) && (HVSD_flag == 0U))
	{
		GI_off_SW_PRESS();
		if((R_GPIO_PinRead(17,4) == 0)&&(logo_completed==1))//Mode key press
		{
			if(skip_flag == 1)
			{
				start_alart_timeout_5sec=1;
				BS6_alert_timeout_4sec = 0;
			}
			CounterSwitchms++;
			if((CounterSwitchms >17000/*34000*/)&&(MODE_key_press == 0))//Long_press 5 sec	/*17000 - 2.5sec*/
			{
				//CounterSwitchms = 0;
				if(((dont_throw_alerts_alert_on_screen_present ==0)&&(dont_throw_alerts_BS6_alert_on_screen_present == 0))||(skip_flag == 1))
				{
					(void)drome_sendEventToDrome((U32)drome_KB_OK);
				}
				Settingmenu_timeout_10sec = 0;
				ok_modelong_press = 1;
				MODE_key_press = 1;								//For Odo Reset
				WH_dooropen_present = 0;	/*wARNING HISTORY PARAMETERS RESET WHEN SCROLLING*/
				WH_backdoor = 0;
				WH_Hooddoor = 0;
				WH_codriverdoor = 0;
				WH_driverdoor = 0;
				WH_Rearleftdoor = 0;
				WH_Rearrightdoor = 0;
				key = drome_getCurrentViewKey();
				if(key == 0x11021)
				{
					lightonoff_modelong_press = 1;
				}
			}
			else if(CounterSwitchms > 393000)
			{
				//key stuck MODE
				//Failure_DTC_Set(DTC_BUTTON_STUCK_A000);
				CounterSwitchms =393000+1;
				MODE_BUTTON_STUCK_FLAG=1;
				if(skip_flag == 1)
				{
					start_alart_timeout_5sec=1;
					BS6_alert_timeout_4sec = 0;
				}
				//	Failure_DTC_Set(DTC_MODE_SWITCH_STUCK_A01C);
			}
		}
		else if((CounterSwitchms > 300) && (CounterSwitchms <= 17000/*34000*/))//Short_press for < 2.5 sec /*17000 - 2.5sec*/
		{
			CounterSwitchms = 0;
			Settingmenu_timeout_10sec = 0;
			if(((dont_throw_alerts_alert_on_screen_present ==0)&&(dont_throw_alerts_BS6_alert_on_screen_present == 0))||(skip_flag == 1))
			{
				(void)drome_sendEventToDrome((U32)drome_KB_MENU);
			}
			if(skip_flag == 1)
			{
				start_alart_timeout_5sec=1;
				BS6_alert_timeout_4sec = 0;
			}
		}
		//	else if((CounterSwitchms >34000)&&(MODE_key_press = 0))//Long_press 5 sec
		//	{
		////		CounterSwitchms = 0;
		//		(void)drome_sendEventToDrome((U32)drome_KB_OK);
		//		ok_modelong_press = 1;
		//		MODE_key_press = 1;								//For Odo Reset
		//	}
		//	else if(CounterSwitchms > 393000)
		//	{
		//		//key stuck
		//	}
		else
		{
			CounterSwitchms = 0;
			OK_pressed = 0;
			ok_modelong_press = 0;
			MODE_key_press = 0;
			MODE_BUTTON_STUCK_FLAG=0;
			//Failure_DTC_Clear(DTC_BUTTON_STUCK_A000);
			Failure_DTC_Clear(DTC_MODE_SWITCH_STUCK_A01C);
		}

		if((R_GPIO_PinRead(17,5) == 0)&&(logo_completed==1))//Set key press
		{
			CounterSwitchmsU++;
			Arrowfill_flag = 1;
			if(skip_flag == 1)
			{
				start_alart_timeout_5sec=1;
				BS6_alert_timeout_4sec = 0;
			}
			if((CounterSwitchmsU >17000/*34000*/)&&((SET_key_press == 0)/*||(SET_5seckey_press == 0)*/))//Long_press for > 2.5 sec
			{
				SET_key_press = 1;				//For Odo Reset
				key = drome_getCurrentViewKey();
#if 0
				if((CounterSwitchmsU >34000)&&(SET_5seckey_press == 0))	/*Long_press for > 5 sec*/
				{
					SET_5seckey_press = 1;		/*USED FOR REGEN ENTRY */
					if((key == 0x22042)||(key == 0x22043)||(key == 0x22044)//incorrect DEF
							||(key == 0x22046)||(key == 0x22047)||(key == 0x22048)//Dosing Malfunction
							||(key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);
						/*dont_throw_alerts_BS6_alert_on_screen_present = 0;*/
						avoid_trip_reset = 1;
					}
					if(skip_flag == 1)
					{
						start_alart_timeout_5sec=1;
						BS6_alert_timeout_4sec = 0;
					}

				}
#endif
				if((key == 0x11001)&&(avoid_trip_reset == 0))
				{
					reset_tripmeterA();
				}
				else if((key == 0x11002)&&(avoid_trip_reset == 0))
				{
					reset_tripmeterB();
				}
				else if((key == 0x11003)&&(avoid_trip_reset == 0))
				{
					reset_AC_tripmeter();
				}
				else if(((key == 0x11011)||(key == 0x11014))&&(avoid_trip_reset == 0)/*&&(AFE_selected == 1)*/)	/*AFE can reset by button for all Setting options*/
				{
					Flag_AFE_reset = 1;
				}
			}

			else if((CounterSwitchmsU >34000)&&(SET_5seckey_press == 0))	/*Long_press for > 5 sec*/
			{
				key = drome_getCurrentViewKey();
				SET_5seckey_press = 1;		/*USED FOR REGEN ENTRY */
				if((key == 0x22042)||(key == 0x22043)||(key == 0x22044)//incorrect DEF
						||(key == 0x22046)||(key == 0x22047)||(key == 0x22048)//Dosing Malfunction
						||(key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);
					/*dont_throw_alerts_BS6_alert_on_screen_present = 0;*/
					avoid_trip_reset = 1;
					SET_5seckey_press = 0; //to avoid regen Entry in permanent alert skip
					CounterSwitchmsU = 0;
				}
				if(skip_flag == 1)
				{
					start_alart_timeout_5sec=1;
					BS6_alert_timeout_4sec = 0;
				}
			}
			else if(CounterSwitchmsU > 393000)
			{
				//key stuck ANY
				CounterSwitchmsU = 393000+1;
				SET_BUTTON_STUCK_FLAG = 1;
				//	Failure_DTC_Set(DTC_SET_SWITCH_STUCK_A022);
			}
		}
		else if((CounterSwitchmsU > 600/*300*/) && (CounterSwitchmsU <= 17000/*34000*/)&&(avoid_trip_reset==0))//Short_press for < 2.5 sec
		{																				/*FOR ARROW FILL/DEFILL ISSUE - 600 COUNTS*/
			set_key_short = 1;//only used in ETM mode
			CounterSwitchmsU = 0;
			//			Arrowfill_flag = 1;
			//			Arrowinit_flg = 1;
			Settingmenu_timeout_10sec = 0;

			/*
		if(skip_alert_Enable == 1)
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
			skip_alert_Enable = 2;
		}
		else if(skip_alert_Enable == 3)
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
			skip_alert_Enable = 0;
		}
		else
			 */
			if((startkey >= 0x11059)&&(startkey <= 0x11094))
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);	/*warning history when scrolling doors inactive*/

			}
			WH_dooropen_present = 0;	/*wARNING HISTORY PARAMETERS RESET WHEN SCROLLING*/
			WH_backdoor = 0;
			WH_Hooddoor = 0;
			WH_codriverdoor = 0;
			WH_driverdoor = 0;
			WH_Rearleftdoor = 0;
			WH_Rearrightdoor = 0;
			if(((dont_throw_alerts_BS6_alert_on_screen_present == 0)||(dont_throw_alerts_alert_on_screen_present==1))||(skip_flag == 1))//to avoid scrolling if BS6 alert ON
			{
				(void)drome_sendEventToDrome((U32)drome_KB_DOWN);//DOWN
			}
			Arrowfill_flag = 1;
			A_flag = 3;
		}
		//	else if((CounterSwitchmsU >34000)&&(SET_key_press == 0))//Long_press for > 5 sec
		//	{
		//		SET_key_press = 1;				//For Odo Reset
		//		key = drome_getCurrentViewKey();
		////		CounterSwitchmsU = 0;
		//		if((key == 0x22042)||(key == 0x22043)||(key == 0x22044)//incorrect DEF
		//				||(key == 0x22046)||(key == 0x22047)||(key == 0x22048)//Dosing Malfunction
		//				||(key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
		//		{
		//			(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);
		//			dont_throw_alerts_BS6_alert_on_screen_present = 0;
		//			avoid_trip_reset = 1;
		//		}
		//		else if((key == 0x11001)&&(avoid_trip_reset == 0))
		//		{
		//			reset_tripmeterA();
		//		}
		//		else if((key == 0x11002)&&(avoid_trip_reset == 0))
		//		{
		//			reset_tripmeterB();
		//		}
		//		else if((key == 0x11003)&&(avoid_trip_reset == 0))
		//		{
		//			reset_AC_tripmeter();
		//		}
		//		else if((key == 0x11011)&&(avoid_trip_reset == 0))
		//		{
		//			Flag_AFE_reset = 1;
		//		}
		//	}
		//	else if(CounterSwitchmsU > 393000)
		//	{
		//		//key stuck
		//	}
		else
		{
			OK_pressed = 0;
			CounterSwitchmsU = 0;
			SET_key_press = 0;
			SET_5seckey_press = 0;
			SET_BUTTON_STUCK_FLAG = 0;
			avoid_trip_reset = 0;//Allow Trip Reset after skipping BS6 alert and Set key release.
			Failure_DTC_Clear(DTC_SET_SWITCH_STUCK_A022);
			//			Arrowfill_flag = 0;
		}
		if(MODE_key_press==1 && SET_key_press==1)
		{
			CounterSwitchms = 0;
			CounterSwitchmsU = 0;
			//		odometer_reset_flag = eeprom_read(ODO_RST_FLAG_OFFSET);
			reset_odometer();
			MODE_key_press=0;
			SET_key_press=0;
		}
		if(MODE_BUTTON_STUCK_FLAG==1)
		{
			Failure_DTC_Set(DTC_MODE_SWITCH_STUCK_A01C);
		}
		if(SET_BUTTON_STUCK_FLAG ==1)
		{
			Failure_DTC_Set(DTC_SET_SWITCH_STUCK_A022);
		}
	}
#endif
}

void taub_grayscale_isr()
{
	ms_8_counter++;

	tt_out_cnt++;
	if(tt_out_cnt >= 260)
	{
		tt_out_cnt=0;
		if(turn_activate>0&&turn_activate<20)
			turn_activate++;
		if((IGN_ON_flag == CLEAR && TT_IGN_OFF==1)||(IGN_ON_flag == CLEAR && IGN_OFF_TT==1))
		{
			DIAG_TT_init();
			//#ifdef GRAYSCALE_TFT

			if(Diag_RW_TurnSignal_LR_IGN_OFF_Chime_Enable == 1)
			{
				if(((Turn_left_TT==0xfff)&&(Turn_right_TT==0xfff))||((Turn_left_TT==0)&&(Turn_right_TT==0))) // Hazard condition occurred
				{
//					DIAG_TT_init();
					if(((Turn_left_TT == 0xfff) || (Turn_right_TT == 0xfff)) && (Next_Right_Turn == 0))
					{
						if(Sound_Bit == 0x0 || Sound_Bit == 0x200)
						{
							Next_Right_Turn = 1;
							//Tick
							Sound_Bit = 0x200;
							Buzz_Tick = 1;
							Buzz_Tick_compl = 0;

							Buzz_Tock = 0;
							Buzz_Tock_compl = 0;
						}
					}
					else if(((Turn_left_TT == 0) && (Turn_right_TT == 0)) && (Next_Right_Turn == 1))
					{
						if(Sound_Bit == 0x0 || Sound_Bit == 0x200)
						{
							Next_Right_Turn = 0;
							//Tock
							Sound_Bit = 0x200;
							Buzz_Tock = 1;
							Buzz_Tock_compl = 0;

							Buzz_Tick = 0;
							Buzz_Tick_compl = 0;
						}
					}
				}
			}
			//#endif
		}

		if((IGN_ON_flag == SET) && (Proveout_Count > PROVEOUT_DONE))
		{
			if((LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				if((DC_data_process_flag==0)&&(IO_Parameter_Request_Status!=3)&&(IO_Parameter_Request_Status!=4))
				{
					DIAG_TT_init();					
				}

				//#ifdef GRAYSCALE_TFT

				if(Diag_RW_TurnSignal_LR_IGN_ON_Chime_Enable == 1)
				{
					if(((Turn_left_TT == 0xfff) || (Turn_right_TT == 0xfff)) && (Next_Right_Turn == 0))
					{
						if(Sound_Bit == 0x0 || Sound_Bit == 0x200)
						{
							Next_Right_Turn = 1;
							//Tick
							Sound_Bit = 0x200;
							Buzz_Tick = 1;
							Buzz_Tick_compl = 0;

							Buzz_Tock = 0;
							Buzz_Tock_compl = 0;
						}
					}
					else if(((Turn_left_TT == 0) && (Turn_right_TT == 0)) && (Next_Right_Turn == 1))
					{
						if(Sound_Bit == 0x0 || Sound_Bit == 0x200)
						{
							Next_Right_Turn = 0;
							//Tock
							Sound_Bit = 0x200;
							Buzz_Tock = 1;
							Buzz_Tock_compl = 0;

							Buzz_Tick = 0;
							Buzz_Tick_compl = 0;
						}
					}
				}
				//#endif

			}
		}
	}



	if(ms_8_counter>=20)
	{
		ms_8_counter=0;


		//	R_GPIO_TogglePin(17,8);
		/*	if(logo_completed==1)*/
#ifdef GRAYSCALE_TFT

		flag_grayscale++;/*30 means 240 ms*/
		if(animation_start == 1)
		{
			scan_rate = 10;
		}
		else
		{
			scan_rate = 30;
		}
		if((flag_grayscale>refresh_rate/*31*//*50*/)&&(Turn_BuzzON == 0))
		{
			if(HANDLE_DROME_START == 0)
			{
				return;
			}

			startkey = drome_getCurrentViewKey();
			if((startkey != 0x00)&&(skip_redraw == 0)&&(ret_redraw != 0))
			{
				//R_GPIO_TogglePin(0,4);
				//			loop_Counter_grayscale = 0;
				//draw_grayscale();
				bufferinit_flag=0xA5;
				flag_grayscale = 0;
				/*	init_grayscale = 0;*/
			}
			else if(bufferinit_flag == 0)
			{

				HANDLE_DROME_START = 0;
			}

		}
#endif
	}
#ifdef GRAYSCALE_TFT
	if(/*Counter_Image_150us>=3 && */(bufferinit_flag==0xA5)&&(Turn_BuzzON == 0))
	{
		//IMMOBIIZER_TT1=1;
		//		Counter_Image_150us=0;
		Counter_IC_CAN_tx+=2;
		Counter_10ms+=2;
		draw_grayscale(row,col);
		//loc_WriteImage_fast(row,col,buffer);
		col = col + IMG_STEP;
		if(col >= 240)
		{
			col = 0;
			row++;
		}
		if(row>=160)
		{
			row =0;
			bufferinit_flag=0;
			HANDLE_DROME_START = 0;
			A_flag++;
			U_flag++;
			if((drive_summery_start == 1)&&((startkey == 0x22039)||(startkey == 0x22040)))
			{
				drive_summery_page_draw = 1;
			}
		}
		//IMMOBIIZER_TT1=0;
	}
#endif
#ifdef COLOR_TFT
	startkey = drome_getCurrentViewKey();
#endif
#ifdef SPRINT4
	if((startkey == 0x11001)/*||(startkey == 0x11011)*/||(startkey == 0x11021)||(startkey == 0x11051)||(startkey == 0x11057)
			||(startkey == 0x11011)||(startkey == 0x11014)||(startkey == 0x11016)||(startkey == 0x11012)||(startkey == 0x11013)
			||(startkey == 0x11002)||(startkey == 0x11003)||(startkey == 0x11004)||(startkey == 0x11005)||(startkey == 0x11006)
			||(startkey == 0x11007)||(startkey == 0x11008)||(startkey == 0x11009)||(startkey == 0x11010)||(startkey == 0x11020)||(startkey == 0x11202)||(startkey == 0x11206))
#else
if((startkey == 0x11001)/*||(startkey == 0x11011)*/||(startkey == 0x11021)||(startkey == 0x11051)||(startkey == 0x11057)
			||(startkey == 0x11011)||(startkey == 0x11014)||(startkey == 0x11016)||(startkey == 0x11012)||(startkey == 0x11013)
			||(startkey == 0x11002)||(startkey == 0x11003)||(startkey == 0x11004)||(startkey == 0x11005)||(startkey == 0x11006)
			||(startkey == 0x11007)||(startkey == 0x11008)||(startkey == 0x11009)||(startkey == 0x11010)||(startkey == 0x11020)||(startkey == 0x11202))
#endif			
	{
		Default_pagekey.u32 = startkey;
	}
#ifdef GRAYSCALE_TFT
	if((A_flag >= 5)&&(Arrowfill_flag == 1))
	{
		A_flag = 3;
		Arrowfill_flag = 0;
	}

	if((U_flag >= 5)&&(ArrowUpfill_flag == 1))
	{
		U_flag = 3;
		ArrowUpfill_flag = 0;
	}
#endif
}




void taub1_isr()
{
	uint8_t IMMO_CAN_DATA = 0;
	uint8_t IMMO_DATA_BIT4 = 4;
	uint8_t IMMO_DATA_BIT5 = 5;

	//R_GPIO_TogglePin(17,8);
	//static SW_loading;
	CounterADC_100ms++;
	if(AFE_start == 1)
	{
		int_20ms_AFE++;
	}

	if((IGN_ON_flag == SET) && (zpd_done==1))
	{
		if((CounterTT_15ms > 300) /*&& (Buzz_ON==1)*/)
		{
			if(GI_CONTROL_FLAG==1)
			{
				GI_Control();
			}
			/*SW_loading=!SW_loading;
			R_GPIO_WritePin(17, 10, SW_loading);
			R_GPIO_WritePin(16, 10, SW_loading);*/
			CounterTT_15ms=0;
			//		R_GPIO_TogglePin(42,13);
			//#ifdef GRAYSCALE_TFT
			//			my_can0_rx();
			//#endif
			/*			if(Network_Message_Monitor_Start == SET)
 - 			{
 - 				my_can0_rx();//for test
 - 			}*/

			if(Proveout_Count > PROVEOUT_DONE)
			{
				if(PROVEOUT_OK == 0)
				{
					init_ISM();//check it
					PROVEOUT_OK = 1;
				}
				speedometer();
				tachometer();
			}
			else
			{
				if(Proveout_Count < PROVEOUT_UP)
				{
					if(Proveout_Count > 0)
					{
						Proveout_temp ++;

						if(Proveout_temp > 35)
						{
							Proveout_temp = 0;
							Proveout_Count++;
						}
					}

				}
				else if(Proveout_Count < PROVEOUT_DOWN)
				{
					display_speedometer_speed=0;
					final_vechile_speed_gauge_raw=0;
					display_tachometer_speed=0;
					Proveout_Count++;
				}
				else
				{
					Proveout_Count = PROVEOUT_DOWN+4; /*no need for this statement, good to be safe*/
				}
			}
			loc_Ism_TC0004();/*RPM and Speedo movement*/
			CounterTT_100ms++;
		}
		CounterTT_15ms++;

		if(CounterTT_100ms>7)
		{
#ifdef U350
			//	SWSC_Mono_Condition();// call each 100ms
#else
			SWSC_Condition();
#endif
			CounterTT_100ms=0;
		}
	}

	if(CounterADC_100ms >= 2000)
	{
		Battery_Condition();
		CounterADC_100ms=0;
		if(TURN_TT_TIMEOUT > 0)
		{
			TURN_TT_TIMEOUT--;
		}
		else
		{
			Turn_left_TT = 0;
			Turn_right_TT = 0;
		}
	}

	if((IGN_ON_flag == CLEAR && TT_IGN_OFF==1)||(IGN_ON_flag == CLEAR && IGN_OFF_TT==1))
	{
		//#ifdef GRAYSCALE_TFT
		counterLRBuzz_4ms++;
		if(counterLRBuzz_4ms >= 40)
		{

			/*Left right indicator */
#ifdef GRAYSCALE_TFT
			if((Buzz_Tick == 1) && (Buzz_Tick_compl == 0)&&(EEPROM_readwrite_para_started==0))
#else
				if((Buzz_Tick == 1) && (Buzz_Tick_compl == 0))
#endif
				{
					if((Buzz_ON == 0))
					{
						Buzz_ON=1;
#ifdef GRAYSCALE_TFT

						//r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
						SoundPCM_Turn_Buzz();
#endif

#ifdef COLOR_TFT
						SoundPCM_tick();
#endif
					}
					else
					{
#ifdef GRAYSCALE_TFT
						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
#endif
						Buzz_ON=0;
						Buzz_Tick_compl=1;
						Buzz_Tick=0;
						//Sound_Bit=0x0;


					}
				}
#ifdef GRAYSCALE_TFT
				else if(EEPROM_readwrite_para_started==1)
				{
					//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
				}
#endif

#ifdef GRAYSCALE_TFT
			if((Buzz_Tock == 1) && (Buzz_Tock_compl == 0)&&(EEPROM_readwrite_para_started==0))
#else
				if((Buzz_Tock == 1) && (Buzz_Tock_compl == 0))
#endif
				{

					if((Buzz_ON == 0))

					{
						Buzz_ON=1;
#ifdef GRAYSCALE_TFT

						//r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
						SoundPCM_Turn_Buzz();
#endif

#ifdef COLOR_TFT
						SoundPCM_tock();
#endif
					}
					else
					{
						Buzz_ON=0;
#ifdef GRAYSCALE_TFT

						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
#endif
						Buzz_Tock_compl=1;
						Buzz_Tock=0;
						//Sound_Bit=0x0;

					}
				}
#ifdef GRAYSCALE_TFT
				else if(EEPROM_readwrite_para_started==1)
				{
					//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
				}
#endif
			if((TURN_TT_TIMEOUT==0) && (Sound_Bit==0x200))
			{
				Sound_Bit=0x0;
			}
			counterLRBuzz_4ms=0;

		}
		//#endif
	}

	if((IGN_ON_flag == SET)&& (Proveout_Count > PROVEOUT_DONE))
	{





		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{


			CounterTT_10ms++;
			counterLRBuzz_4ms++;
			//counterLR_4ms++;

#ifdef COLOR_TFT
			/*if(counterLR_4ms >= 80)
			{
				if(turn_activate>0&&turn_activate<7)
					turn_activate++;
				counterLR_4ms = 0;
			}*/


			//			if(Proveout_Count > PROVEOUT_DONE)
			//			{
#endif

			if(counterLRBuzz_4ms >= 40)
			{
				//__DI();
				/*Left right indicator */
				//				for(U8 i=0;i<2;i++)
				//				{
#ifdef GRAYSCALE_TFT
				if((Buzz_Tick == 1) && (Buzz_Tick_compl == 0)&&(EEPROM_readwrite_para_started==0))
#else
					if((Buzz_Tick == 1) && (Buzz_Tick_compl == 0))
#endif
					{
						if((Buzz_ON == 0))
						{
							Buzz_ON=1;
							Turn_BuzzON = 1;
#ifdef GRAYSCALE_TFT
							SoundPCM_Turn_Buzz();
#endif
							//r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
#ifdef COLOR_TFT
							SoundPCM_tick();
#endif
						}
						else
						{
							//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							Buzz_ON=0;
							Buzz_Tick_compl=1;
							Buzz_Tick=0;
							Turn_BuzzON = 0;
							//Sound_Bit=0x0;


						}
					}
#ifdef GRAYSCALE_TFT
					else if(EEPROM_readwrite_para_started==1)
					{
						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					}
#endif

#ifdef GRAYSCALE_TFT
				if((Buzz_Tock == 1) && (Buzz_Tock_compl == 0)&&(EEPROM_readwrite_para_started==0))
#else
					if((Buzz_Tock == 1) && (Buzz_Tock_compl == 0))
#endif
					{
						if((Buzz_ON == 0))
						{
							Buzz_ON=1;
							Turn_BuzzON = 1;
							//r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
#ifdef GRAYSCALE_TFT
							SoundPCM_Turn_Buzz();
#endif
#ifdef COLOR_TFT
							SoundPCM_tock();
#endif
						}
						else
						{
							Buzz_ON=0;
							Turn_BuzzON = 0;
							//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							Buzz_Tock_compl=1;
							Buzz_Tock=0;
							//Sound_Bit=0x0;

						}
					}
#ifdef GRAYSCALE_TFT
					else if(EEPROM_readwrite_para_started==1)
					{
						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					}
#endif
				//				else
				//				{
				//					Turn_BuzzON = 0;
				//				}
				//				if(i==0)
				//				{
				//IMMOBIIZER_TT1=1;
				//				delay_time(50000); /*2.5 ms*/
				//				delay_time(50000); /*2.5 ms*/
				//IMMOBIIZER_TT1=0;
				//				}
				//				}
				if((TURN_TT_TIMEOUT==0) && (Sound_Bit==0x200))
				{
					Sound_Bit=0x0;
					Turn_BuzzON = 0;
				}
				counterLRBuzz_4ms=0;
				//__EI();
			}

#ifdef GRAYSCALE_TFT
			if(Proveout_Count > PROVEOUT_DONE)
			{
				if(Counter_RPAS_80MS >=2)
				{
					if((RPAS_LEVEL_1_BUZ==1)&&(RPAS_LEVEL_1_TONE_compl == 0)&&(RPAS_SELF_CHECK_TONE_compl == 1))
					{
						if(RPAS_LEVEL1_PRECON==0)
						{
							SoundPCM_RPAS_LV1_Buzz();
							RPAS_LEVEL1_PRECON=1;
							RPAS_LEVEL2_BUZ_ONGOING = 0;
							RPAS_LEVEL3_BUZ_ONGOING = 0;
							RPAS_LEVEL4_BUZ_ONGOING = 0;
							Counter_RPAS_80MS = 0;// to add delay between 2 chimes
						}
					}
				}
			/*	if(RPAS_LEVEL1_PRECON==1 )
				{
					if(RPAS_LEVEL_1_BUZ==0)
					{
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						RPAS_LEVEL1_PRECON=0;
					}

				}
			*/}

#endif

#ifdef GRAYSCALE_TFT
#ifndef U350

			if((W501_LOW_RPAS_LEVEL_1_BUZ==1)&&(W501_LOW_RPAS_LEVEL_1_TONE_compl == 0))
			{
//				r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
				if(W501_LOW_RPAS_LEVEL1_PRECON == 0)
				{
					SoundPCM_RPAS_LV1_Buzz();
					W501_LOW_RPAS_LEVEL1_PRECON=1;
				}
			}
			/*if(W501_LOW_RPAS_LEVEL1_PRECON==1 )
			{
				if(W501_LOW_RPAS_LEVEL_1_BUZ==0)
				{
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					W501_LOW_RPAS_LEVEL1_PRECON=0;
				}

			}*/
#endif
#endif


			if(CounterTT_10ms >= 200)
			{
				Counter_Gear_250ms++;
				CounterTTD_250ms++;
				CounterTT_500ms_on++;
				CounterTT_500ms_off++;
				CounterTT_1000ms_on++;
				CounterTT_1000ms_off++;
				CounterTT_500ms_TPMS_on++;
				CounterTT_500ms_TPMS_off++;
				CounterTTP_50ms++;
				CounterTT_1sec++;
				CounterTT_3sec++;
				CounterTT_70ms++;
				CounterTT_150ms++;
				Counter_50mS++;
				CounterTT_90ms++;
				CounterBz_500ms++;
				Counterblink_500ms++;
				CounterosBz_757ms++;
				counter757ms++;
				pattern2_counter50ms++;
				message_counter1sec++;
				reverse_counter100msec++;
				tick_tock_counter30msec++;
				AVS_DT_counter_sec++;
				dooropencounter++;
				/*				welcomecounter++;*/
				CounterBz_3sec++;
				Counter_RPAS_80MS++;
				Counter_RPAS_500MS++;
				RPAS_self_check_counter500msec++;
				W501_RPAS_FAIL_check_counter500msec++;
				//				RPAS_LV1_COUNTER++;
				//RPAS_LV2_COUNTER++;
				//RPAS_LV3_COUNTER++;
				//RPAS_LV4_COUNTER++;
				two_sec_fuel_cnt++;
				RPAS_ERROR_COUNTER++;
#ifdef SPRINT4
				/* Timers used for TDMS */
				TDMS_Alert_Screen_SlewTimer++;
				TDMS_Slew_Timer_5s++;
				TDMS_Main_Screen_SlewTimer++;
				Condition_Timer++;
#endif
#ifdef GRAYSCALE_TFT
#ifndef U350

				W501_LOW_Counter_RPAS_500MS++;
				W501_LOW_CounterBz_3sec++;
				W501_LOW_Counter_RPAS_80MS++;
#endif
#endif
				/*pattern 3 500 ms ON- 500 ms OFF*/

				seat_belt_chime_condition();

				if(CounterTT_500ms_on >= (Diag_RW_ESC_ON_Time/10))//50
				{
					//	telltale_blink_flag_patt3 = ~telltale_blink_flag_patt3;
					if(telltale_blink_flag_patt3 == 0x000)
					{
						telltale_blink_flag_patt3 = 0xfff;
						telltale_blink_bool3 =1;
						CounterTT_500ms_off=0;
					}
					CounterTT_500ms_on = 0;
				}
				if(CounterTT_500ms_off >= (Diag_RW_ESC_OFF_Time/10))//50
				{
					if(telltale_blink_flag_patt3 == 0xfff)
					{
						telltale_blink_flag_patt3 = 0x000;
						telltale_blink_bool3 =0;
						CounterTT_500ms_on = 0;
					}
					CounterTT_500ms_off=0;
				}
				/*blink_patt_8*/
				if(CounterTT_1000ms_on >= (Diag_RW_TT_PATTERN8_TPMS_ON_Time/10))//100
				{
					if(telltale_blink_flag_patt8 == 0x000)
					{
						telltale_blink_flag_patt8 = 0xfff;
						CounterTT_1000ms_off	=	0;
					}
					CounterTT_1000ms_on	=	0;
				}
				if(CounterTT_1000ms_off >= (Diag_RW_TT_PATTERN8_TPMS_OFF_Time/10))//100
				{

					if(telltale_blink_flag_patt8 == 0xfff)
					{
						telltale_blink_flag_patt8 = 0x000;
						CounterTT_1000ms_on	=	0;
					}
					CounterTT_1000ms_off	=	0;
				}
				/*Blink_patt_9*/
				if(CounterTT_500ms_TPMS_on >= (Diag_RW_TT_PATTERN9_TPMS_ON_Time/10))//50
				{
					//	telltale_blink_flag_patt3 = ~telltale_blink_flag_patt3;
					if(telltale_blink_flag_patt9 == 0x000)
					{
						telltale_blink_flag_patt9 = 0xfff;
						CounterTT_500ms_TPMS_off=0;
					}
					CounterTT_500ms_TPMS_on = 0;
				}
				if(CounterTT_500ms_TPMS_off >= (Diag_RW_TT_PATTERN9_TPMS_OFF_Time/10))//50
				{
					if(telltale_blink_flag_patt9 == 0xfff)
					{
						telltale_blink_flag_patt9 = 0x000;
						CounterTT_500ms_TPMS_on = 0;
					}
					CounterTT_500ms_TPMS_off=0;
				}

#ifdef GRAYSCALE_TFT
				/* Chimes Start */
				if(CounterTT_70ms >= 7)
				{
					/*High Temp, Low engOilPres,LowBrkFld,*//*70ms ON, 210 sec OFF */
					if((Buzz_High_Temp == 1) && (Buzz_High_Temp_compl == 0))
					{
						if((Buzz_ON == 0) && (counter3 >= 3))
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							counter3=0;
							Buzz_High_Temp_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							counter3++;
							if(Buzz_High_Temp_cntr > 3)
							{
								Buzz_High_Temp_compl=1;
								Buzz_High_Temp=0;
								Sound_Bit=0x0;
							}
						}
					}
					else if((Buzz_Low_Eng_oilPre == 1 ) && (Buzz_Low_Eng_oilPre_compl == 0))
					{
						if((Buzz_ON == 0) && (counter3 >= 3))
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							counter3=0;
							Buzz_Low_Eng_oilPre_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							counter3++;
							if(Buzz_Low_Eng_oilPre_cntr > 3)
							{
								Buzz_Low_Eng_oilPre_compl=1;
								Buzz_Low_Eng_oilPre=0;
								Sound_Bit = 0x00;
								Buzz_Low_Eng_oilPre_cntr=0;
							}
						}
					}
					else if((Buzz_Low_Brk_fld == 1)  && (Buzz_Low_Brk_fld_compl == 0))
					{
						if((Buzz_ON == 0) && (counter3 >= 3))
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							counter3=0;
							Buzz_Low_Brk_fld_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							counter3++;
							if(Buzz_Low_Brk_fld_cntr > 3)
							{
								Buzz_Low_Brk_fld_compl=1;
								Buzz_Low_Brk_fld=0;
								Sound_Bit = 0x0;
							}
						}

					}
					//WIF
					else if((Buzz_WIF == 1)  && (Buzz_WIF_compl == 0))
					{
						if((Buzz_ON == 0) && (counter3 >= 3))
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							counter3=0;
							Buzz_WIF_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							counter3++;
							if(Buzz_WIF_cntr > 3)
							{
								Buzz_WIF_compl=1;
								Buzz_WIF=0;
								Sound_Bit = 0x0;
							}
						}
					}
					/*Park Brake */
					else if((Buzz_Park_brake == 1) && (Buzz_Park_brake_compl == 0))  /* 70ms ON, 70ms OFF*/
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							Buzz_Park_brake_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);

							if(Buzz_Park_brake_cntr > 3)	/*30 replaced by 3*/
							{
								Buzz_Park_brake_compl=1;
								Buzz_Park_brake=0;
								Sound_Bit = 0x0;
							}
						}
					}

					CounterTT_70ms=0;
				}
				if(CounterTT_150ms >= 15)
				{
					/*Door Open,LOw Fuel 150ms ON , 150MS OFF*/
					if((Buzz_Door_open == 1) && (Buzz_Door_open_compl == 0))
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							Buzz_Door_open_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);

							if(Buzz_Door_open_cntr > 3)
							{
								Buzz_Door_open_compl=1;
								Buzz_Door_open=0;
								Sound_Bit = 0x0;
								Buzz_Door_open_cntr=0;
							}
						}
					}
					else if((Buzz_Low_fuel == 1) && (Buzz_Low_fuel_compl == 0))
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							Buzz_Low_fuel_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);

							if(Buzz_Low_fuel_cntr > 3)
							{
								Buzz_Low_fuel_compl=1;
								Buzz_Low_fuel=0;
								Sound_Bit = 0x0;
							}
						}
					}
					CounterTT_150ms=0;
				}
				/*SBR lv*/
				if(CounterTT_90ms >= 8)//9
				{
					if((Buzz_SBR_lv2 == 1) && (Buzz_SBR_lv2_compl == 0))
					{
						if((Buzz_ON == 1) && (counter7 >= 6))//7
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							SBR_Off_time++;
							if(SBR_Off_time >= 2)
							{
								counter7=0;
								if(Buzz_SBR_lv2_cntr > Diag_RW_Final_chime_duration)// 133
								{
									Buzz_SBR_lv2_compl=1;
									Buzz_SBR_lv2=0;
			/*set Over speed sound bit after completing SBR Chime if overspeed condition present*/
									if(OS_L2_Start==1)
									{
										Sound_Bit = 0x2000;
									}
									else
									{
										Sound_Bit = 0x0;
									}
								}
								else
								{
									Buzz_SBR_lv2_cntr++;
								}
							}

						}
						else
						{
							if(SBR_Off_time >= 2)
							{
								SBR_Off_time = 0;

								r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							}
							counter7++;
							Buzz_ON=1;
						}
					}
					CounterTT_90ms=0;
				}
				if(CounterBz_500ms >= 30)//50
				{
					if((Buzz_rever_gr == 1) && (Buzz_rever_gr_compl == 0))
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							Buzz_rever_gr_cntr++;
							reverse_chm_on = 1;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							reverse_chm_on = 0;
							if(Buzz_rever_gr_cntr >= 1)
							{
								Buzz_rever_gr_compl=1;
								Buzz_rever_gr=0;
								Sound_Bit = 0x0;
								Buzz_rever_gr_cntr = 0;
							}
						}
					}
					else if((reverse_chm_on == 1)&&(Buzz_rever_gr_compl == 0))
					{
						Buzz_ON=0;
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						reverse_chm_on = 0;
						//if(Buzz_rever_gr_cntr >= 1)
						{
							Buzz_rever_gr_compl = 1;
							Buzz_rever_gr = 0;
							Sound_Bit = 0x0;
						}
					}
					CounterBz_500ms=0;
					/*testing*/
					if(Reverse_gear_active == 0)
					{
						Buzz_rever_gr_compl = 0;
					}
				}

				//OverSpeed

				if(CounterosBz_757ms >= 55)//	46
				{
					if((Buzz_OS_L1 == 1) && (Buzz_OS_compl == 0))
					{
						if(((Buzz_ON == 0) && (Sound_Bit == 0x0))||((Buzz_ON == 0) && ((Sound_Bit == 0x200)||(Sound_Bit == 0x2000))))
						{
							Sound_Bit = 0x2000;
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							Buzz_OS_cntr++;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);

							if(Buzz_OS_cntr >= 1)
							{
								Buzz_OS_compl=1;
								Buzz_OS_L1=0;
								Sound_Bit = 0x0;
							}
						}
					}
					else if((Buzz_OS_L2 == 1) && (Buzz_OS_compl == 0))
					{
						if((/*(Buzz_ON == 0) &&*/ (Sound_Bit == 0x0))||(/*(Buzz_ON == 0) &&*/ ((Sound_Bit == 0x200)||(Sound_Bit == 0x2000))))
						{
							if((Buzz_ON == 0) && (OS_counter3 >= OverSpeedLv2RepeatIntrval+1))  //counter for 2 sec OFF
							{
								Buzz_ON=1;
								r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
								OS_counter3=0;
								Buzz_OS_off_tmr=0;
							}
							else
							{
								Buzz_ON=0;
								OS_counter3++;
								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							}
						}
					}
					else
					{
						OS_counter3 = 3;//Load Counter to immediate response overspeed
					}

					if((Buzz_OS_off_tmr >= (OverSpeedLv1RepeatIntrval*1.256/*32*/)) && (OverSpeedLv1repeatcnt == 1))//1.2 factor to get excact 60 sec
					{
						Buzz_OS_L1=1;
						if(OverSpeedLevel1Type==2)//repeat after time interval
						{
							Buzz_OS_compl=0;
						}
						Buzz_OS_off_tmr=0;
					}
					if((OverSpeedLv1repeatcnt == 0)&&(OverSpeedLv2repeatcnt == 0))//reset timer interval if speed below 80kmph
					{
						Buzz_OS_off_tmr = 0;
					}
					CounterosBz_757ms=0;
					Buzz_OS_off_tmr++;
				}

#endif

#ifdef U350
				/*RPAS_ERROR_TONE*/
				DID_RPAS_ERROR_TONE_CHECK=Diag_RW_RPAS_Error_Chime_Enable/10;

				if(CounterBz_3sec>=(DID_RPAS_ERROR_TONE_CHECK*RPAS_ErrorTone_factor))//300
				{
					if((RPAS_ERROR_TONE==1)&&(RPAS_ERROR_TONE_compl == 0))
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							Buzz_RPAS_ERR_TONE_ctr++;
							U350_RPAS_ERROR_TONE_ONGOING=1;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							if(Buzz_RPAS_ERR_TONE_ctr>=1)
							{
								RPAS_ERROR_TONE_compl=1;
								RPAS_ERROR_TONE=0;
								Sound_Bit = 0x0;
							}
						}


					}
					CounterBz_3sec=0;
				}
				if(U350_RPAS_ERROR_TONE_ONGOING==1 && RPAS_ERROR_TONE==0)
				{
					U350_RPAS_ERROR_TONE_ONGOING=0;
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
				}

				/*RPAS_SELFCHECK_TONE*/

				DID_RPAS_SELF_CHECK=Diag_RW_RPAS_self_check_chime_Enable/10;
				if(Diag_RW_RPASS_Self_check_Chime_Enable==1)
				{
					if(Counter_RPAS_500MS >= (DID_RPAS_SELF_CHECK*0.6))//0.6 is self-check tone factor
					{
						if((RPAS_SELF_CHECK_TONE==1)&&(RPAS_SELF_CHECK_TONE_compl == 0))
						{
							if(Buzz_ON == 0)
							{
								Buzz_ON=1;
								r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
								Buzz_RPAS_SELFCHECK_TONE_ctr++;
								RPAS_SELF_BUZ_ONGOING = 1;
							}
							else
							{
								Buzz_ON=0;
								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
								if(Buzz_RPAS_SELFCHECK_TONE_ctr>=1)
								{
									RPAS_SELF_CHECK_TONE_compl=1;
									RPAS_SELF_CHECK_TONE=0;
									Counter_RPAS_80MS = 0;//to add delay between selftone and bar chime
									//Sound_Bit = 0x0;
								}
								RPAS_SELF_BUZ_ONGOING = 0;
							}
							Buzz_RPAS_SELFCHECK_TONE_ctr++;

						}
						Counter_RPAS_500MS=0;

						if(RPAS_SELF_CHECK_TONE==0 && RPAS_SELF_BUZ_ONGOING == 1)
						{
							RPAS_SELF_BUZ_ONGOING=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						}

					}
				}
				else
				{
					RPAS_SELF_CHECK_TONE_compl=1;
					RPAS_SELF_CHECK_TONE=0;
					RPAS_SELF_BUZ_ONGOING = 0;
				}
				if(Counter_RPAS_80MS >=3)				//30ms
						{
					/*RPAS_LV2*/
					if((RPAS_LEVEL_2_BUZ == 1)&&(RPAS_LEVEL2_BUZ_ONGOING == 0)&&(RPAS_SELF_CHECK_TONE_compl == 1))
					{
					/*
						if((Buzz_ON == 0))
						{
							RPAS_COUNTER3++;
							if(RPAS_COUNTER3>=2)
							{
								Buzz_ON=1;
								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);

								RPAS_COUNTER3=0;
								RPAS_LEVEL2_BUZ_ONGOING=0;
								//							RPAS_LEVEL_2_BUZ=0;
								//							Sound_Bit = 0x0;
								RPAS_LEVEL_2_TONE_compl=1;
							}
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							RPAS_LEVEL_2_TONE_ctr++;
							RPAS_LEVEL2_BUZ_ONGOING=1;
						}
					*/
						RPAS_LEVEL1_PRECON = 0;
						RPAS_LEVEL2_BUZ_ONGOING = 1;
						RPAS_LEVEL3_BUZ_ONGOING = 0;
						RPAS_LEVEL4_BUZ_ONGOING = 0;
						SoundPCM_RPAS_LV2_Buzz();
					}
					/*RPAS_LV3*/
					if((RPAS_LEVEL_3_BUZ==1)&&(RPAS_LEVEL3_BUZ_ONGOING == 0)&&(RPAS_SELF_CHECK_TONE_compl == 1))
					{
					/*	if((Buzz_ON == 0))
						{
							RPAS_COUNTER3++;
							if(RPAS_COUNTER3>=2)
							{
								Buzz_ON=1;
								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
								RPAS_LEVEL_3_TONE_compl=1;
								RPAS_COUNTER3=0;
								RPAS_LEVEL3_BUZ_ONGOING=0;
								//							RPAS_LEVEL_3_BUZ=0;
								//							Sound_Bit = 0x0;
							}
						}
						else
						{
							RPAS_130_CTR++;
							if(RPAS_130_CTR>=3)
							{
								Buzz_ON=0;
								r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
								RPAS_LEVEL_3_TONE_ctr++;
								RPAS_130_CTR=0;
								RPAS_LEVEL3_BUZ_ONGOING=1;
							}
						}
					*/
						RPAS_LEVEL1_PRECON = 0;
						RPAS_LEVEL2_BUZ_ONGOING = 0;
						RPAS_LEVEL3_BUZ_ONGOING = 1;
						RPAS_LEVEL4_BUZ_ONGOING = 0;
						SoundPCM_RPAS_LV3_Buzz();
					}
					/*RPAS_LV4*/
					if((RPAS_LEVEL_4_BUZ==1)&&(RPAS_LEVEL4_BUZ_ONGOING == 0)&&(RPAS_SELF_CHECK_TONE_compl == 1))
					{
					/*	if((Buzz_ON == 0))
						{
							RPAS_COUNTER3++;
							if(RPAS_COUNTER3>=2)
							{
								Buzz_ON=1;
								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
								RPAS_LEVEL_3_TONE_compl=1;
								RPAS_COUNTER3=0;
								RPAS_LEVEL4_BUZ_ONGOING=0;
								//							RPAS_LEVEL_3_BUZ=0;
								//							Sound_Bit = 0x0;
							}
						}
						else
						{
							RPAS_430_CTR++;
							if(RPAS_430_CTR>=13)
							{
								Buzz_ON=0;
								r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
								RPAS_LEVEL_3_TONE_ctr++;
								RPAS_430_CTR=0;
								RPAS_LEVEL4_BUZ_ONGOING=1;
							}
						}
						*/
						RPAS_LEVEL1_PRECON = 0;
						RPAS_LEVEL2_BUZ_ONGOING = 0;
						RPAS_LEVEL3_BUZ_ONGOING = 0;
						RPAS_LEVEL4_BUZ_ONGOING = 1;
						SoundPCM_RPAS_LV4_Buzz();
					}
					Counter_RPAS_80MS=0;
					if(RPAS_LEVEL_1_BUZ  ==0 && RPAS_LEVEL1_PRECON == 1)
					{
						//if(Sound_Bit == 0x10000)
						{
							RPAS_LEVEL1_PRECON = 0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV1_STOP();
						}

					}
					if(RPAS_LEVEL_2_BUZ==0 && RPAS_LEVEL2_BUZ_ONGOING == 1)
					{
						//if(Sound_Bit == 0x20000)
						{
							RPAS_LEVEL2_BUZ_ONGOING=0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV2_STOP();
						}
						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					}
					if(RPAS_LEVEL_3_BUZ==0 && RPAS_LEVEL3_BUZ_ONGOING==1)
					{
						//if(Sound_Bit == 0x30000)
						{
							RPAS_LEVEL3_BUZ_ONGOING=0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV3_STOP();
						}
						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					}
					if(RPAS_LEVEL_4_BUZ==0 && RPAS_LEVEL4_BUZ_ONGOING==1)
					{
						//if(Sound_Bit == 0x40000)
						{
							RPAS_LEVEL4_BUZ_ONGOING=0;
							Sound_Bit = 0x0;
							SoundPCM_RPAS_buzz_LV4_STOP();
						}
						//r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					}
						}



#endif



#ifdef GRAYSCALE_TFT
#ifndef U350
				/****************RPAS_SELF_CHECK_W501_LOW_END**************************/


				W501_LOW_DID_RPAS_SELF_CHECK=Diag_RW_RPAS_self_check_chime_Enable/10;

				if(W501_LOW_Counter_RPAS_500MS >= W501_LOW_DID_RPAS_SELF_CHECK)
				{
					if((W501_LOW_RPAS_SELF_CHECK_TONE==1)&&(W501_LOW_RPAS_SELF_CHECK_TONE_compl == 0))
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr++;
							W501_LOW_RPAS_SELF_BUZ_ONGOING = 1;
						}
						else
						{
							Buzz_ON=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
							if(W501_LOW_Buzz_RPAS_SELFCHECK_TONE_ctr>=1)
							{
								W501_LOW_RPAS_SELF_CHECK_TONE_compl=1;
								W501_LOW_RPAS_SELF_CHECK_TONE=0;
								Sound_Bit = 0x0;
							}
							//							W501_LOW_RPAS_SELF_BUZ_ONGOING = 1;
						}


					}
					W501_LOW_Counter_RPAS_500MS=0;
					if(W501_LOW_RPAS_SELF_CHECK_TONE==0 && W501_LOW_RPAS_SELF_BUZ_ONGOING == 1)
					{
						W501_LOW_RPAS_SELF_BUZ_ONGOING=0;
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					}
				}

				/*RPAS_ERROR_TONE*/


				W501_LOW_DID_RPAS_ERROR_TONE_CHECK=Diag_RW_RPAS_Error_Chime_Enable/10;

				//	if(W501_LOW_CounterBz_3sec>=(W501_LOW_DID_RPAS_ERROR_TONE_CHECK * RPAS_ErrorTone_factor))//300
				{
					if((W501_LOW_RPAS_ERROR_TONE==1)&&(W501_LOW_RPAS_ERROR_TONE_compl == 0))
					{
						if(Buzz_ON == 0)
						{
							Buzz_ON=1;
							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
							W501_LOW_Buzz_RPAS_ERR_TONE_ctr++;
							W501_LOW_RPAS_ERROR_TONE_ONGOING=1;
							W501_LOW_CounterBz_3sec=0;

						}
						else
						{
							if(W501_LOW_CounterBz_3sec>=(W501_LOW_DID_RPAS_ERROR_TONE_CHECK * RPAS_ErrorTone_factor))//300
									{
								Buzz_ON=0;
								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
								if(W501_LOW_Buzz_RPAS_ERR_TONE_ctr>=1)
								{
									W501_LOW_RPAS_ERROR_TONE_compl=1;
									W501_LOW_RPAS_ERROR_TONE=0;
									Sound_Bit = 0x0;
								}
									}
						}
					}
					//W501_LOW_CounterBz_3sec=0;
				}

				if(W501_LOW_RPAS_ERROR_TONE_ONGOING==1 && W501_LOW_RPAS_ERROR_TONE==0)
				{
					W501_LOW_RPAS_ERROR_TONE_ONGOING=0;
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
				}
				/***********LEVEL2_W501_LOW*************/
				if(W501_LOW_RPAS_ERROR_TONE == 0)
				{
					if(W501_LOW_Counter_RPAS_80MS >=3)				//30ms
					{
						/*RPAS_LV2*/
						if((W501_LOW_RPAS_LEVEL_2_BUZ==1))
						{
							if(W501_LOW_RPAS_COUNTER2 == 0)
							{
								SoundPCM_RPAS_LV2_Buzz();
								W501_LOW_RPAS_COUNTER2 = 1;
							}
							/*if((Buzz_ON == 0))
							{
								W501_LOW_RPAS_COUNTER3++;
								if(W501_LOW_RPAS_COUNTER3>=2)
								{
									Buzz_ON=1;
									//							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
									r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
									W501_LOW_RPAS_COUNTER3=0;
									W501_LOW_RPAS_LEVEL2_BUZ_ONGOING=0;
									//								W501_LOW_RPAS_LEVEL_2_BUZ=0;
								}
							}
							else
							{
								Buzz_ON=0;
								//							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
								r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
								W501_LOW_RPAS_LEVEL_2_TONE_compl=1;
								W501_LOW_RPAS_LEVEL2_BUZ_ONGOING=1;
							}*/
						}
						/*RPAS_LV3*/

						if((W501_LOW_RPAS_LEVEL_3_BUZ==1))
						{
							if(W501_LOW_RPAS_COUNTER3 == 0)
							{
								SoundPCM_RPAS_LV3_Buzz();
								W501_LOW_RPAS_COUNTER3 = 1;
							}
							/*if((Buzz_ON == 0))
							{
								W501_LOW_RPAS_COUNTER3++;
								if(W501_LOW_RPAS_COUNTER3>=2)
								{
									Buzz_ON=1;
									//						r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
									r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
									//RPAS_LEVEL_3_TONE_ctr++;
									W501_LOW_RPAS_COUNTER3=0;
									W501_LOW_RPAS_LEVEL3_BUZ_ONGOING=0;
									//								W501_LOW_RPAS_LEVEL_3_BUZ=0;
								}
							}
							else
							{
								W501_LOW_RPAS_130_CTR++;

								if(W501_LOW_RPAS_130_CTR>=3)
								{
									Buzz_ON=0;
									//								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
									r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);

									W501_LOW_RPAS_LEVEL_3_TONE_compl=1;
									W501_LOW_RPAS_130_CTR=0;
									W501_LOW_RPAS_LEVEL3_BUZ_ONGOING=1;
								}
							}*/
						}

						/*RPAS_LV4*/
						if((W501_LOW_RPAS_LEVEL_4_BUZ==1))
						{
							if(W501_LOW_RPAS_COUNTER4 == 0)
							{
								SoundPCM_RPAS_LV4_Buzz();
								W501_LOW_RPAS_COUNTER4 = 1;
							}
							/*if((Buzz_ON == 0))
							{
								W501_LOW_RPAS_COUNTER3++;
								if(W501_LOW_RPAS_COUNTER3>=2)
								{
									Buzz_ON=1;
									//							r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
									r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
									//RPAS_LEVEL_3_TONE_ctr++;
									W501_LOW_RPAS_COUNTER3=0;
									W501_LOW_RPAS_LEVEL4_BUZ_ONGOING=0;
									//								W501_LOW_RPAS_LEVEL_4_BUZ=0;
								}
							}
							else
							{
								W501_LOW_RPAS_430_CTR++;
								if(W501_LOW_RPAS_430_CTR>=13)
								{
									Buzz_ON=0;
									//								r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
									r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
									//								W501_LOW_RPAS_COUNTER3++;
									W501_LOW_RPAS_LEVEL_4_TONE_compl=1;
									//W501_LOW_RPAS_LEVEL_3_BUZ=0;
									W501_LOW_RPAS_430_CTR=0;
									W501_LOW_RPAS_LEVEL4_BUZ_ONGOING=1;
								}
							}*/
						}
						W501_LOW_Counter_RPAS_80MS=0;
						if((W501_LOW_RPAS_LEVEL_2_BUZ==0) && (W501_LOW_RPAS_LEVEL2_BUZ_ONGOING == 1))
						{
							W501_LOW_RPAS_LEVEL2_BUZ_ONGOING=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						}
						if((W501_LOW_RPAS_LEVEL_3_BUZ==0) && (W501_LOW_RPAS_LEVEL3_BUZ_ONGOING==1))
						{
							W501_LOW_RPAS_LEVEL3_BUZ_ONGOING=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						}
						if((W501_LOW_RPAS_LEVEL_4_BUZ==0) && (W501_LOW_RPAS_LEVEL4_BUZ_ONGOING==1))
						{
							W501_LOW_RPAS_LEVEL4_BUZ_ONGOING=0;
							r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						}

					}
				}

#endif
#endif



#ifdef COLOR_TFT
				if(counter757ms >= 87)//76
				{
					if((Seatbelt_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle133_completed == 0))
					{

						if(cycle133_cntr==0)
						{
							SoundPCM_Seatbelt_Lv0cont(Diag_RW_Final_chime_duration);
							SBR_Start = 1;
						}
						cycle133_cntr++;
					}
					else
					{
						if((cycle133_cntr!=0)||(SBR_Start == 1))	//cycle133_cntr become 0 after complition of cycle time. and so chime not stop.
						{
							SoundPCM_Seatbelt_Lv0cont_STOP();
							SBR_Start = 0;
						}
					}

					if(cycle133_cntr >=Diag_RW_Final_chime_duration)//133
					{
						cycle133_cntr = 0;
						cycle133_completed = 1;
						Sound_Bit = 0x0;			/**Sound bit clear**/
					}
					else if((cycle133_cntr > 0)&&(cycle133_cntr < Diag_RW_Final_chime_duration)&&(Seatbelt_missed == 1))//133
					{
						chime_start1 = 0;
						Seatbelt_missed = 0;
						cycle133_cntr = 0;

					}
					//					else if((cycle133_cntr > 0)&&(cycle133_cntr < 133)&&()
					counter757ms = 0;
				}
#endif
#ifdef COLOR_TFT
				if(pattern2_counter50ms >= 5)	/*As per pattern 2......4 cycles*/
				{
					/*High temp chime*/
					if((High_temp_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle_Hightemp_completed == 0)&&(RPAS_Present_flag==0))	/*High temp*/
					{
						if((cycleHightemp_cntr == 0)||(cycleHightemp_cntr == 4)||(cycleHightemp_cntr == 8)||(cycleHightemp_cntr == 12))
						{
							SoundPCM_RPAS_St3();
						}
						cycleHightemp_cntr++;
					}
					if(cycleHightemp_cntr >=13)
					{
						cycleHightemp_cntr = 0;
						cycle_Hightemp_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycleHightemp_cntr > 0)&&(cycleHightemp_cntr < 13)&&(Hightemp_missed == 1))
					{
						chime_start1 = 0;
						Hightemp_missed = 0;
						cycleHightemp_cntr = 0;
					}

					// Check if message timeout has occurred or Check Engine Running Status stopped
					if( (EMS1_10_message_timeout_flag != 0)||(Enginerun_chm != 1) )
					{
						High_temp_ongoing = 0;
						cycle_Hightemp_completed = 0;
						Hightemp_missed = 1;
						if(Sound_Bit == 0x1)
						{
							Sound_Bit=0x0;						/*Sound bit CLEAR*/
						}
					}

					/*****************/
					/*Low Engine oil pressure chime*/
					if((Low_engine_oil_pressure == 1)/*&&(chime_start1 == 1)*/&&(cycle_Lowengineoil_completed == 0))	/*Low Engine Oil pressure*/
					{
						if((cycleLowengineoil_cntr == 0)||(cycleLowengineoil_cntr == 4)||(cycleLowengineoil_cntr == 8)||(cycleLowengineoil_cntr == 12))
						{
							SoundPCM_RPAS_St3();
						}
						cycleLowengineoil_cntr++;
					}
					if(cycleLowengineoil_cntr >=13)
					{
						cycleLowengineoil_cntr = 0;
						cycle_Lowengineoil_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycleLowengineoil_cntr > 0)&&(cycleLowengineoil_cntr < 13)&&(Lowengineoil_missed == 1))
					{
						chime_start1 = 0;
						Lowengineoil_missed = 0;
						cycleLowengineoil_cntr = 0;
					}
					/*******************************/
					/*Low Brake fluid chime*/

					if((Lowbrakefluid_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle_Lowbrakefluid_completed == 0))	/*Low Brake fluid chime*/
					{
						if((cycleLowbrakefluid_cntr == 0)||(cycleLowbrakefluid_cntr == 4)||(cycleLowbrakefluid_cntr == 8)||(cycleLowbrakefluid_cntr == 12))
						{
							SoundPCM_RPAS_St3();
						}
						cycleLowbrakefluid_cntr++;
					}
					if(cycleLowbrakefluid_cntr >=13)
					{
						cycleLowbrakefluid_cntr = 0;
						cycle_Lowbrakefluid_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycleLowbrakefluid_cntr > 0)&&(cycleLowbrakefluid_cntr < 13)&&(Lowbrakefluid_missed == 1))
					{
						chime_start1 = 0;
						Lowbrakefluid_missed = 0;
						cycleLowbrakefluid_cntr = 0;
					}
					/************************/
					/*Water In fuel chime*/

					if((WIF_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle_waterinfuel_completed == 0)&&(RPAS_Present_flag==0))	/*Low Brake fluid chime*/
					{
						if((cycleWIF_cntr == 0)||(cycleWIF_cntr == 4)||(cycleWIF_cntr == 8)||(cycleWIF_cntr == 12))
						{
							SoundPCM_RPAS_St3();
						}
						cycleWIF_cntr++;
					}
					if(cycleWIF_cntr >=13)
					{
						cycleWIF_cntr = 0;
						cycle_waterinfuel_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycleWIF_cntr > 0)&&(cycleWIF_cntr < 13)&&(WIF_missed == 1))
					{
						chime_start1 = 0;
						Lowbrakefluid_missed = 0;
						cycleWIF_cntr = 0;
					}
					/************************/

					pattern2_counter50ms = 0;
				}
#endif

				three_bit_AFE.bits.Rx_1 = CAN_EMS1_10[6].bits.Rx_6;  //ENG running status
				three_bit_AFE.bits.Rx_2 = CAN_EMS1_10[6].bits.Rx_7;
				three_bit_AFE.bits.Rx_3 = CAN_EMS1_10[6].bits.Rx_8;

				//Low Engine Oil pressure
				if((LOP_7Sec_cntr_start == 1) && (three_bit_AFE.byte == 2))
				{
					if(LOP_7Sec_counter > 450)//700
					{
						LOP_7Sec_cntr_done = 1;
					}
					else
					{
						LOP_7Sec_counter++;
					}
				}
				else
				{
					LOP_7Sec_cntr_done=0;
					LOP_7Sec_counter=0;
					LOP_7Sec_cntr_start=0;
					Low_engine_oil_pressure = 0;//r
					cycle_Lowengineoil_completed = 0;//r
				}

				//Low Brake fluid
				if((LBF_3Sec_cntr_start == 1) && (three_bit_AFE.byte == 2))
				{
					if(LBF_3Sec_counter > 190)//300
					{
						LBF_3Sec_cntr_done = 1;
					}
					else
					{
						LBF_3Sec_counter++;
					}
				}
				else
				{
					LBF_3Sec_cntr_done=0;
					LBF_3Sec_counter=0;
					LBF_3Sec_cntr_start=0;
				}

#ifdef COLOR_TFT
				/*7_rec_RSST_nSndID_Message Pattern*/
				if(message_counter1sec >= 100)			/*Door open*/
				{
					/*door open chime - 1000ms -period.....1000ms -ON Time......4 cycles*/


					if(cycledooropen_cntr >= 4)
					{
						cycledooropen_cntr = 0;
						cycle_dooropen_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycledooropen_cntr>0) && (cycledooropen_cntr<4)&&(Dooropen_missed == 1))
					{
						chime_start1 = 0;
						cycledooropen_cntr = 0;
						Dooropen_missed = 0;
					}

					if((dooropen_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle_dooropen_completed == 0)&&(RPAS_Present_flag==0))	/*door open*/
					{
						SoundPCM_message();
						cycledooropen_cntr++;
					}
					/*******************/
					/*Park brake chime- 1000ms -period.....1000ms -ON Time......4 cycles*/

					if(cycleparkbrake_cntr >= 4)
					{
						cycleparkbrake_cntr = 0;
						cycle_parkbrake_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycleparkbrake_cntr > 0)&&(cycleparkbrake_cntr < 4)&&(parkbrake_missed == 1))
					{
						chime_start1 = 0;
						cycleparkbrake_cntr = 0;
						parkbrake_missed = 0;
					}

					if((Parkbrake_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle_parkbrake_completed == 0))
					{
						SoundPCM_message();
						cycleparkbrake_cntr++;
					}
					// Adding my logic here to fix park brake check Vehicle stopped condition here
					if((Parkbrake_ongoing == 1) &&((vehicle_moving_state!=1)||(ESC2_10_message_timeout_flag != 0) || (ESC12_10_message_timeout_flag != 0)))
					{
						Parkbrake_ongoing=0;
						parkbrake_missed=1;
						cycle_parkbrake_completed = 1;//r_test
						if(Sound_Bit == 0x08)
						{
							Sound_Bit=0x0;				/*Sound bit CLEAR*/
						}
					}

					/*******************/
					/*Low Fuel chime- 1000ms -period.....1000ms -ON Time......4 cycles*/

					if(cyclelowfuel_cntr >= 4)
					{
						cyclelowfuel_cntr = 0;
						cycle_lowfuel_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cyclelowfuel_cntr > 0)&&(cyclelowfuel_cntr < 4)&&(lowfuel_missed == 1))
					{
						chime_start1 = 0;
						cyclelowfuel_cntr = 0;
						lowfuel_missed = 0;
					}
					if((Low_fuel_ongoing == 1)/*&&(chime_start1 == 1)*/&&(cycle_lowfuel_completed == 0))	/*Low Fuel*/
					{
						SoundPCM_message();
						cyclelowfuel_cntr++;
					}
					/*******************/
					/****Message Alarm - 1000ms -period........1 Cycle****/

					//					if(((Check_ABS_popup_shown == 1 && ABS_Notification == 1)||(EBD_popup_shown==1)||(Check_Engine_popup_shown==1)||(Service_Reminder_popup_shown ==1)||(ESP_popup_shown==1))&&(Notification_chime == 1))
					if((Notification_chime == 1)&&(Sound_Bit == 0x0|| Sound_Bit == 0x200))
					{
						Sound_Bit = 0x90000;
						cyclemsgnote_cntr++;
						if(cyclemsgnote_cntr == 1)
						{
							ABS_Notification=0;
							Notification_chime = 0;
							SoundPCM_message();
						}
						else
						{
							cyclemsgnote_cntr = 2;
						}

					}
					else
					{
						cyclemsgnote_cntr = 0;
						if(Sound_Bit == 0x90000)
						{
							Sound_Bit = 0x0;//clear bit
						}
					}

					message_counter1sec = 0;
				}
				/*Reverse Gear Chime - 100ms- Period.......100ms- ON Time.....5 cycles*/
				if(reverse_counter100msec >= reverse_cnt/*10*/)
				{
					if((Reversegear_ongoing == 1)&&(cycle_Reversegear_completed == 0))
					{
						if(cycleReversegear_cntr==0)
						{
							SoundPCM_Reverse_cont(5);
						}
						cycleReversegear_cntr++;
					}
					if(cycleReversegear_cntr >= 5)
					{
						cycleReversegear_cntr = 0;
						cycle_Reversegear_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycleReversegear_cntr > 0)&&(cycleReversegear_cntr < 5)&&(Reversegear_missed == 1))
					{
						chime_start1 = 0;
						cycleReversegear_cntr = 0;
						Reversegear_missed = 0;
					}
					reverse_counter100msec = 0;
				}

				/************RPAS_SELF_CHECK****************/
				W501_HIGH_DID_RPAS_SELF_CHECK=Diag_RW_RPAS_self_check_chime_Enable/10;

				if(RPAS_self_check_counter500msec >= W501_HIGH_DID_RPAS_SELF_CHECK * RPAS_ErrorTone_factor)
				{
					if((RPASS_self_check_ongoing == 1)&&(cycle_RPASS_SELF_CHECK_completed == 0))
					{
						if(cycle_RPAS_self_check_cntr==0)
						{
							SoundPCM_Reverse_cont(Diag_RW_RPAS_self_check_chime_Enable/100);
						}
						cycle_RPAS_self_check_cntr++;
						W501_cycle_RPASS_FAIL_CHECK_completed = 0;
					}
					if(cycle_RPAS_self_check_cntr > 4)
					{
						cycle_RPAS_self_check_cntr = 0;
						cycle_RPASS_SELF_CHECK_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((cycle_RPAS_self_check_cntr > 0)&&(cycle_RPAS_self_check_cntr < 5)&&(RPAS_self_check_missed == 1))
					{
						chime_start1 = 0;
						cycle_RPAS_self_check_cntr = 0;
						RPAS_self_check_missed = 0;
						W501_cycle_RPASS_FAIL_CHECK_completed = 0;
					}
					RPAS_self_check_counter500msec = 0;
				}


				/***************RPAS_FAIL_CHECK***********/
				W501_HIGH_DID_RPAS_ERROR_TONE_CHECK=Diag_RW_RPAS_Error_Chime_Enable/10;
#if 0
				if(W501_RPAS_FAIL_check_counter500msec >= (W501_HIGH_DID_RPAS_ERROR_TONE_CHECK * RPAS_ErrorTone_factor))
				{
					if((W501_RPASS_fail_check_ongoing == 1)&&(W501_cycle_RPASS_FAIL_CHECK_completed == 0))
					{
						SoundPCM_RPAS_ST1_ST2();
						W501_cycle_RPAS_fail_check_cntr++;
					}
					if(W501_cycle_RPAS_fail_check_cntr >= 5)
					{
						W501_cycle_RPAS_fail_check_cntr = 0;
						W501_cycle_RPASS_FAIL_CHECK_completed = 1;
						Sound_Bit=0x0;					/**Sound bit clear**/
					}
					else if((W501_cycle_RPAS_fail_check_cntr > 0)&&(W501_cycle_RPAS_fail_check_cntr < 5)&&(RPAS_fail_check_missed == 1))
					{
						chime_start1 = 0;
						W501_cycle_RPAS_fail_check_cntr = 0;
						RPAS_fail_check_missed = 0;
					}
					W501_RPAS_FAIL_check_counter500msec = 0;
				}
#endif
				if(W501_RPAS_FAIL_check_counter500msec < W501_HIGH_DID_RPAS_ERROR_TONE_CHECK)
				{
					if(RPAS_ERROR_COUNTER >= Errorcnt)
					{
						if((W501_RPASS_fail_check_ongoing == 1)&&(W501_cycle_RPASS_FAIL_CHECK_completed == 0)/*&&((Reversegear_ongoing == 1)||(RPAS_ACTIVE_STS_flag == 0x1))*/)
						{

							if(sound_rpas_fail_once==0)
							{
								sound_rpas_fail_once =1;
								SoundPCM_Reverse_cont(Diag_RW_RPAS_Error_Chime_Enable/100);
							}
							Error_soundstart = 1;
							cycle_RPASS_SELF_CHECK_completed = 0;
						}
						else if(W501_RPASS_fail_check_ongoing == 0)
						{
							sound_rpas_fail_once =0;
						}

						RPAS_ERROR_COUNTER = 0;
					}
				}
				else
				{
					if(Error_soundstart == 1)
					{
						Error_soundstart = 0;
						W501_cycle_RPASS_FAIL_CHECK_completed = 1;
						cycle_RPASS_SELF_CHECK_completed = 0;
						sound_rpas_fail_once=0;
						if(Sound_Bit==0x4000)
						{

							SoundPCM_Reverse_cont_STOP();
							Sound_Bit=0x0;					/**Sound bit clear**/
						}
					}
					W501_RPAS_FAIL_check_counter500msec = 0;
					//				 W501_cycle_RPASS_FAIL_CHECK_completed = 1;
				}

				/***********RPAS STATUS CHIME*********/

				/*RPAS_LEVEL1*/
				//		 if(RPAS_LV1_COUNTER >= reverse_cnt/*38*/)//75)
				{
					if((W501_RPAS_LEVEL_1_BUZ==1)&&(RPAS_Error_flag==0))
					{
						if(RPAS_LV1_COUNTER == 0)
						{
							RPAS_LV1_COUNTER = 1;
							//					 		SoundPCM_Reverse();
							//SoundPCM_4_rec_RPAS_St1_St2_cont();
							RPAS_LV_Chime_Play(0x01);
							W501_RPAS_LV1_PRECONDITION=1;
						}
					}

					if(W501_RPAS_LV1_PRECONDITION==1 && W501_RPAS_LEVEL_1_BUZ==0)
					{
						//					 Sound_Bit = 0x0;
						W501_RPAS_LV1_PRECONDITION=0;

					}
					//			 RPAS_LV1_COUNTER=0;
				}


				/*RPAS_LEVEL2*/
				//if(RPAS_LV2_COUNTER >= 16)
				{
					if((W501_RPAS_LEVEL_2_BUZ==1)&&(RPAS_Error_flag==0))
					{
						if(RPAS_LV2_COUNTER == 0)
						{
							RPAS_LV2_COUNTER=1;
							//SoundPCM_RPAS_LV2();
							RPAS_LV_Chime_Play(0x02);
							W501_RPAS_LV2_PRECONDITION=1;
						}
					}
					if(W501_RPAS_LV2_PRECONDITION==1 && W501_RPAS_LEVEL_2_BUZ==0)
					{
						//					 Sound_Bit = 0x0;
						W501_RPAS_LV2_PRECONDITION=0;

					}
					//				 RPAS_LV2_COUNTER=0;

				}
				/*RPAS_LEVEL3*/
				// if(RPAS_LV3_COUNTER >= 17)
				{

					if((W501_RPAS_LEVEL_3_BUZ==1 && W501_RPAS_LV3_CNTR==0)&&(RPAS_Error_flag==0))
					{
						if(RPAS_LV3_COUNTER == 0)
						{
							RPAS_LV3_COUNTER=1;
							//SoundPCM_Reverse();
							//SoundPCM_RPAS_LV3();
							RPAS_LV_Chime_Play(0x03);
							W501_RPAS_LV3_PRECONDITION=1;
							// W501_RPAS_LV3_CNTR++;
						}
					}

					//	 else if(W501_RPAS_LV3_CNTR==1)
					{
						// Sound_Bit = 0x0;
						//		 W501_RPAS_LV3_CNTR=0;
					}
					if(W501_RPAS_LV3_PRECONDITION==1 && W501_RPAS_LEVEL_3_BUZ==0)
					{
						//				 Sound_Bit = 0x0;
						W501_RPAS_LV3_PRECONDITION=0;

					}
					//			 RPAS_LV3_COUNTER=0;
				}

				/*RPAS_LEVEL4*/
				//	 if(RPAS_LV4_COUNTER >= 33)
				{

					if((W501_RPAS_LEVEL_4_BUZ==1 && W501_RPAS_LV4_CNTR==0)&&(RPAS_Error_flag==0))
					{
						if(RPAS_LV4_COUNTER == 0)
						{
							RPAS_LV4_COUNTER=1;
							//SoundPCM_RPAS_LV4();
							RPAS_LV_Chime_Play(0x04);
							W501_RPAS_LV4_PRECONDITION=1;
							// W501_RPAS_LV4_CNTR++;
						}
					}
					//		 else if(W501_RPAS_LV4_CNTR==1)
					{
						//Sound_Bit = 0x0;
						//			 W501_RPAS_LV4_CNTR=0;
					}
					if(W501_RPAS_LV4_PRECONDITION==1 && W501_RPAS_LEVEL_4_BUZ==0)
					{
						//Sound_Bit = 0x0;
						W501_RPAS_LV4_PRECONDITION=0;

					}
					//	 RPAS_LV4_COUNTER=0;
				}

				RPAS_LV_Chime_Dispacher();
				/***************overspeed level 1******************/
				if(OverSpeedLv1repeatcnt == 1)
				{
					overspeedlevel1_cntr++;
					if(OverSpeedLevel1Type == 1)		 /*Once*/
					{
						if((overspeedlevel1_cntr == 1)&&(level1_ongoing == 1)&&(cycle_Overspdl1_completed == 0))
						{
							Sound_Bit = 0x2000;
							SoundPCM_OverSpeed();
							//							overspeedlevel1_cntr = 0;
							//							cycle_Overspdl1_completed = 1;
						}
						else
						{
							if(overspeedlevel1_cntr>=40)//time for complete Level 1 chime in hazard on condition
							{
								if(Sound_Bit == 0x2000)
								{
									Sound_Bit = 0x0;
								}
							}
							//							cycle_Overspdl1_completed = 1;
							//							overspeedlevel1_cntr = 0;
						}

					}
					else if(OverSpeedLevel1Type == 2)	/*repeatative*/
					{
						if(overspeedlevel1_cntr == 1)	/*initially*/
						{
							if((level1_ongoing == 1)&&(cycle_Overspdl1_completed == 0))
							{
								Sound_Bit = 0x2000;
								SoundPCM_OverSpeed();
								//									Sound_Bit = 0x0;
							}

						}
						if(overspeedlevel1_cntr >= ((OverSpeedLv1RepeatIntrval*100)*0.87))	/*n1*100*/ // *0.87 to remove offset
						{
							if((level1_ongoing == 1)/*&&(cycle_Overspdl1_completed == 0)*/)
							{
								Sound_Bit = 0x2000;
								SoundPCM_OverSpeed();
								cycle_Overspdl1_completed = 1;//test
							}
							else if((overspeedlevel1_cntr > 0)&&(overspeedlevel1_cntr < ((OverSpeedLv1RepeatIntrval*100)*0.87))&&(Overspdl1_missed == 1))
							{
								chime_start1 = 0;
								overspeedlevel1_cntr = 0;
								Overspdl1_missed = 0;
							}
							overspeedlevel1_cntr = 0;
						}
						else
						{
							if(overspeedlevel1_cntr>=40)//time for complete Level 1 chime in hazard on condition
							{
								if(Sound_Bit == 0x2000)
								{
									Sound_Bit = 0x0;
								}
							}
							//							cycle_Overspdl1_completed = 1;

						}

					}

				}
				else
				{
					cycle_Overspdl1_completed = 0;//test
				}

				/*****overspeed level 2********/
				if(OverSpeedLv2repeatcnt == 1)
				{
					overspeedlevel2_cntr++;
					if(OverSpeedLevel2Type == 1)		 /*Once*/
					{
						if((overspeedlevel2_cntr == 1)&&(level2_ongoing == 1)&&(cycle_Overspdl2_completed == 0))
						{
							SoundPCM_OverSpeed();
							Sound_Bit = 0x0;			/**Sound bit clear**/
							//							overspeedlevel1_cntr = 0;
							//							cycle_Overspdl2_completed = 1;
						}
						else
						{
							//							cycle_Overspdl2_completed = 1;
							//							overspeedlevel2_cntr = 0;
						}

					}
					else if(OverSpeedLevel2Type == 2)	/*repeatative*/
					{
						if(overspeedlevel2_cntr == 1)	/*initially*/
						{
							if((level2_ongoing == 1)&&(cycle_Overspdl2_completed == 0))
							{
								SoundPCM_OverSpeed();
							}

						}
						if(overspeedlevel2_cntr >= (OverSpeedLv2RepeatIntrval*100))	/*n1*1000*/
						{
							if((level2_ongoing == 1)&&(cycle_Overspdl2_completed == 0))
							{
								SoundPCM_OverSpeed();
							}
							else if((overspeedlevel2_cntr > 0)&&(overspeedlevel2_cntr < (OverSpeedLv2RepeatIntrval*100))&&(Overspdl2_missed == 1))
							{
								chime_start1 = 0;
								overspeedlevel2_cntr = 0;
								Overspdl2_missed = 0;
							}
							overspeedlevel2_cntr = 0;
						}
						else
						{

						}

					}

				}
#if 0
				/* */
				if(tick_tock_counter30msec >= 3)		/*30 msec tick - ON......tock - ON*/
				{
					if((Tick_ongoing == 1)&&(cycle_tick_completed == 0))
					{
						SoundPCM_tick();								/*tick sound*/
						Next_Right_Turn = 1;
						cycle_tick_completed = 1;
						Sound_Bit=0x0;									/**Sound bit clear**/
					}
					else if((Tock_ongoing == 1)&&(cycle_tock_completed == 0))
					{
						SoundPCM_tock();								/*tock sound*/
						Next_Right_Turn = 0;
						cycle_tock_completed = 1;
						Sound_Bit=0x0;									/**Sound bit clear**/
					}
					else
					{
						cycle_tock_completed = 0;
						cycle_tick_completed = 0;
						Sound_Bit=0x0;
					}
					tick_tock_counter30msec = 0;
				}
#endif
#endif
				/******************************************************************************/
				//#ifdef COLOR_TFT
				if(dooropencounter >= 10/*15*/)				/*Door open Animation - 150 ms for image change*/
				{
					/*if((Codriver_flag == 1)||(Driver_flag == 1)||(Rearleft_flag == 1)||(RearRight_flag == 1)||(Hood_flag == 1)||(Back_flag == 1))
					{
						if(decrement_trend == 0)
						{
							codoor_index++;
							if(codoor_index > 4)
							{
								//							   codoor_index = 0;
								decrement_trend = 1;
							}
						}
						if(decrement_trend == 1)
						{
							codoor_index--;
							if(codoor_index == 0)
							{
								codoor_index = 0;
								decrement_trend = 0;
							}
						}
					}*/
					if((Codriver_flag == 1)&&(Dooropen_popup_shown ==1)/*&&(codoor_index == 0)*/)
					{
						if((Codriver_index != 0))
						{
							Codriver_index--;
							//Codriver_index = codoor_index;
							//Codoor_set = 0;
						}
						if(Codriver_index == 0)
						{
							Codriver_index = 0;
							Codoor_set = 1;
						}
						Codoor_display = 1;

					}
					else if((Codriver_flag == 0)&&(Codoor_set == 1))
					{
						if((Codriver_index != 4))
						{
							Codriver_index++;
							//Codriver_index = codoor_index;
							//Codoor_set = 0;
						}
						if(Codriver_index == 4)
						{
							Codriver_index = 4;
							Codoor_set = 0;
						}
						Codoor_display = 1;
					}
					else
					{
						Codoor_display = 0;
					}

					if((Driver_flag == 1)&&(Dooropen_popup_shown ==1)/*&&(codoor_index == 0)*/)
					{
						if((Driver_index != 0))
						{
							Driver_index--;
							//Codriver_index = codoor_index;
							//Codoor_set = 0;
						}
						if(Driver_index == 0)
						{
							Driver_index = 0;
							driverdoor_set = 1;
						}
						Driver_display = 1;
					}
					else if((Driver_flag == 0)&&(driverdoor_set == 1))
					{
						if((Driver_index != 4))
						{
							Driver_index++;
							//Codriver_index = codoor_index;
							//Codoor_set = 0;
						}
						if(Driver_index == 4)
						{
							Driver_index = 4;
							driverdoor_set = 0;
						}
						Driver_display = 1;
					}
					else
					{
						Driver_display = 0;
					}

					if((Rearleft_flag == 1)&&(Dooropen_popup_shown ==1)/*&&(codoor_index == 0)*/)
					{
						if((Rearleft_index != 0))
						{
							Rearleft_index--;

						}
						if(Rearleft_index == 0)
						{
							Rearleft_index = 0;
							Rearleftdoor_set = 1;
						}
						Rearleft_display = 1;

					}
					else if((Rearleft_flag == 0)&&(Rearleftdoor_set == 1))
					{
						if((Rearleft_index != 4))
						{
							Rearleft_index++;
							//Codriver_index = codoor_index;
							//Codoor_set = 0;
						}
						if(Rearleft_index == 4)
						{
							Rearleft_index = 4;
							Rearleftdoor_set = 0;
						}
						Rearleft_display = 1;
					}
					else
					{
						Rearleft_display = 0;
					}

					if((RearRight_flag == 1)&&(Dooropen_popup_shown ==1)/*&&(codoor_index == 0)*/)
					{
						if((RearRight_index != 0))
						{
							RearRight_index--;

						}
						if(RearRight_index == 0)
						{
							RearRight_index = 0;
							RearRightdoor_set = 1;
						}
						Rearright_display = 1;

					}
					else if((RearRight_flag == 0)&&(RearRightdoor_set == 1))
					{
						if((RearRight_index != 4))
						{
							RearRight_index++;
							//Codriver_index = codoor_index;
							//Codoor_set = 0;
						}
						if(RearRight_index == 4)
						{
							RearRight_index = 4;
							RearRightdoor_set = 0;
						}
						Rearright_display = 1;
					}
					else
					{
						Rearright_display = 0;
					}

					if((Back_flag == 1)&&(Dooropen_popup_shown ==1)/*&&(codoor_index == 0)*/)
					{
						if((Backd_index != 0))
						{
							Backd_index--;

						}
						if(Backd_index == 0)
						{
							Backd_index = 0;
							Backdoor_set = 1;
						}
						Back_display = 1;

					}
					else if((Back_flag == 0)&&(Backdoor_set == 1))
					{
						if((Backd_index != 4))
						{
							Backd_index++;
						}
						if(Backd_index == 4)
						{
							Backd_index = 4;
							Backdoor_set = 0;
						}
						Back_display = 1;
					}
					else
					{
						Back_display = 0;
					}



					dooropencounter = 0;
				}
				//#endif
				/************************************************************************************/
#if 0
				if((welcomecounter >= 10)&&(startkey == 0x13001))			/*Door open Animation - 150 ms for image change*/
				{
					welcome_index++;

					if(welcome_index >= 9)
					{
						welcome_index = 9;
						if((first_screen_timeout>=first_screen_timeout_MAX) && (Proveout_Count > PROVEOUT_DONE))
						{
							first_screen_timeout=0;
							(void)drome_sendEventToDrome((U32)drome_InEventMain_screen_timeout);/*send timeout to drome to print next screen after logo*/
							animation_start = 0;
						}
						//							   codoor_index = 0;

					}
					welcomecounter = 0;
				}
#endif

				if(regen_req_start == 1)
				{
					Regen_10seccnt++;
					if(Regen_10seccnt >= 900) 	 /*10 sec*/
					{
						CAN_IC2_100[5].bits.Rx_2 = 0;		/*Issue Modified for VPsprint2*/
						wait_10sec = 1;
						Regen_10seccnt = 0;
					}
				}
				else
				{
					Regen_10seccnt = 0;
					wait_10sec = 0;
				}

				/******************************************************************************/
				if(AVS_DT_counter_sec >= 100)	/* 1 Sec*/
				{
					Average_vehicle_speed();
					drive_time();
					AVS_DT_counter_sec = 0;
				}
				/******************************************************************************/
				/******************************************************************************/
				if(AT_Reverse_detected == 1)
				{
					AT_Reversegear_timespan++;
					if(AT_Reversegear_timespan >=350/*300*/) 	 	/*03 sec*/
					{
						AT_Reversegear_timespan = 0;
					}
				}
				/******************************************************************************/
				/******************************************************************************/

				if(RPAS_STAYBACK_FLAG==1)
				{
					Stayback_timespan++;
					if(Stayback_timespan >= 50) /*100*/	 	/*01 sec*/
					{
						Stayback_timespan = 0;
						staybacktime_count++;
						if(staybacktime_count > 5)		/*05 sec*/
						{
							staybacktime_count = 0;
						}

					}
				}

				else
				{
					staybacktime_count =0;
					Stayback_timespan =0;
				}
				/******************************************************************************/
				/* Chimes END */
				if(CounterTT_1sec >= 100)
				{
					if(telltale_blink_at_1sec == 0)
					{
						telltale_blink_at_1sec =1;
					}
					else
					{
						telltale_blink_at_1sec = 0;
					}

					//TPMS TT
					if(ninty_sec_start_flg==1)
					{
						CounterTT_90sec++;

						if(CounterTT_90sec >= TPMS_TT_Blink_Duration)//90
						{
							ninty_sec_cmplt_flg=1;
						}
					}
					else
					{
						CounterTT_90sec=0;

					}

					//4WD TT 10s
					if(Flag_4WHTT_10s_start == 1)
					{
						Counter4wdTT_10s++;

						if(Counter4wdTT_10s >= 10)
						{
							TT4WH_10s_cmplt_flg=1;
						}
					}
					else
					{
						Counter4wdTT_10s=0;
					}

					CounterTT_1sec=0;

					//scan_TT_status();
					//		DIAG_TT_init();
					//output_shift_register();
				}
				/************************/

				if(shortckt__30sec_start == 1)
				{
					shortckt__30sec++;
					if(shortckt__30sec >= 2600)	/*3000 = 30sec*/
					{
						timer_30sec_completed = 1;
						shortckt__30sec = 0;
					}
					recovery__30sec = 0;
					openckt__30sec = 0;
				}
				else if(recovery__30sec_start == 1)
				{
					recovery__30sec++;
					if(recovery__30sec >= 2600)	/*1900 = 22sec*/
					{
						timer_30sec_completed = 1;
						recovery__30sec = 0;
					}
					openckt__30sec = 0;
					shortckt__30sec = 0;
				}
				else if(openckt__30sec_start == 1)
				{
					openckt__30sec++;
					if(openckt__30sec >= 2600)	/*1900 = 30sec*/
					{
						timer_30sec_completed = 1;
						openckt__30sec = 0;
					}
					recovery__30sec = 0;
					shortckt__30sec = 0;
				}
				else
				{
					recovery__30sec = 0;
					shortckt__30sec = 0;
					openckt__30sec = 0;
				}
				/**************Blinks start*************************************/
				if(CounterTT_3sec >= 300)	/*Blinking pattern 3sec ON......3sec OFF...@1/6Hz*/
				{
					if((empty_blinkpattern_flag == 1)||(full_blinkpattern_flag==1))
					{
						//					toggle_fuel = toggle_fuel ^ 0x01;//Normal Bar
						fuel_widget = fuel_widget  & 0x08;//widget
						fuel_widget = fuel_widget  ^ 0x08;//widget
						if(fuel_widget == 8)//to sync TT with bar
						{
							LOW_FUEL_TT = 1;
						}
						else
						{
							LOW_FUEL_TT = 0;
						}
					}

					if(telltale_blink_flag_patt10==0)
					{
						telltale_blink_flag_patt10=1;
					}
					else
					{
						telltale_blink_flag_patt10=0;
					}

					CounterTT_3sec = 0;
				}

				if(two_sec_fuel_cnt >= 200)
				{
					fuel_enable_tt=1;
				}
				/************************/

				if(CounterTTD_250ms >= 25)	/*Blinking pattern 500ms ON......500ms OFF...@2Hz*/
				{

					if(Temp_blinkpattern_flag_7 == 1)
					{
						temp_widget = temp_widget  & 0x07;//widget
						temp_widget = temp_widget  ^ 0x07;//widget
						blink_temp = 1;
					}

					else if(Temp_blinkpattern_flag_8 == 1)
					{
						//						toggle_temp = toggle_temp ^ 0x01;//normal bar
						temp_widget = temp_widget  & 0x08;//widget
						temp_widget = temp_widget  ^ 0x08;//widget
						blink_temp = 1;

					}

					//					CounterTTD_250ms = 0;
				}
				if(Counterblink_500ms >= 50)
				{
					toggle_timedots = toggle_timedots ^ 0x01;
					Counterblink_500ms = 0;
				}


				/*pattern 7 250 ms ON- 250ms OFF*/
				if(CounterTTD_250ms >= 25)
				{
					input_shift_register();
					if(telltale_blink_flag_patt7 == 0x000)
					{
						telltale_blink_flag_patt7 = 0xfff;
						telltale_blink_bool7 =1;
					}
					else
					{
						telltale_blink_flag_patt7 = 0x000;
						telltale_blink_bool7 =0;
					}

					//4WD TT 500ms
					if(Flag_4WHTT_500ms_start == 1)
					{
						Counter4wdTT_500ms++;

						if(Counter4wdTT_500ms >= 2)
						{
							TT4WH_500ms_cmplt_flg=1;
						}
					}
					else
					{
						Counter4wdTT_500ms=0;
					}
					//4WD TT 1500ms
					if(Flag_4WHTT_1500ms_start == 1)
					{
						Counter4wdTT_1500ms++;

						if(Counter4wdTT_1500ms >= 6)
						{
							TT4WH_1500ms_cmplt_flg=1;
						}
					}
					else
					{
						Counter4wdTT_1500ms=0;
					}
					//4WD TT 1000ms
					if(Flag_4WHTT_1000ms_start == 1)
					{
						Counter4wdTT_1000ms++;

						if(Counter4wdTT_1000ms >= 4)
						{
							TT4WH_1000ms_cmplt_flg=1;
						}
					}
					else
					{
						Counter4wdTT_1000ms=0;
					}


					//4WD TT2 1000ms
					if(Flag_4WHTT2_1000ms_start == 1)
					{
						Counter4wdTT2_1000ms++;

						if(Counter4wdTT2_1000ms >= 4)
						{
							TT24WH_1000ms_cmplt_flg=1;
						}
					}
					else
					{
						Counter4wdTT2_1000ms=0;
					}

					//	telltale_blink_flag_patt7 = ~telltale_blink_flag_patt7;
					if((IO_Parameter_Request_Status!=3)&&(IO_Parameter_Request_Status!=4))
					{
						scan_TT_status();
						//DIAG_TT_init();
						output_shift_register();
					}
					if(IO_Parameter_Request_Status==3)
					{
						IO_CONTROL_TT_FUNCTIONALITY();
					}
					//Illumination_steps();
					CounterTTD_250ms=0;
				}
				if(Counter_Gear_250ms >= 15)    //15 for testing changed to 30
				{
					if(IO_Parameter_Request_Status !=3)
					{
						Current_Gear_and_Gear_Shift_Indication();
#ifdef SPRINT4
						AT_gear_alert();
#endif
					}
					else
					{
							IO_Control_Gear_indication();
							IO_Control_Gear_shift_indication();
					}
					Counter_Gear_250ms = 0;
				}
#ifdef SPRINT4
/************************************ AT gear debounce **********************************/
				if(AT_gear_debounce_start == 1)
				{
					AT_gear_debounce_cnt++;
					if((AT_gear_debounce_cnt >= (Diag_AT_GEAR_ALERT_Debounce/10))||(Diag_AT_GEAR_ALERT_Debounce == 0))
					{
//						ATgear_popup_shown = 0;
						TGS_LEVER1 = ((CAN_TCU6_20[2].byte) & (0x0F));/* storing Lower nibble of  */
						if(TGS_LEVER1 == 0x09)	/*P*/
						{
							AT_Gear_display = 0;
						}
						if(TGS_LEVER1 == 0x08) /*R*/
						{
							AT_Gear_display = 1;
						}
						if(TGS_LEVER1 == 0x07)	/*N*/
						{
							AT_Gear_display = 2;
						}
						if(TGS_LEVER1 == 0x06)	/*D*/
						{
							AT_Gear_display = 3;
						}
						if(TGS_LEVER1 == 0x0C)	/*M*/
						{
							AT_Gear_display = 4;
						}
						AT_gear_alert_active = 1;			/*AT_gear Alert is active*/
						AT_gear_debounce_cnt = 0;
					}
				}
				else
				{
//					AT_gear_alert_active = 0;
					AT_gear_debounce_cnt = 0;
				}
/************************************ AT gear debounce for gear updation**********************************/
				if((startkey == 0x22073)/*&&(gear_change_debounce_flag == 1)*/)				/* AT gear aler screen*/
				{
					TGS_LEVER1 = ((CAN_TCU6_20[2].byte) & (0x0F));/* storing Lower nibble of  */
					if(PREV_TGS_LEVER1 != TGS_LEVER1)
					{
						AT_gear_update_cnt = 0;
					}
					PREV_TGS_LEVER1 = TGS_LEVER1;
					AT_gear_update_cnt++;
					if(AT_gear_update_cnt >= (Diag_AT_GEAR_ALERT_Debounce/10))
					{
//						TGS_LEVER1 = ((CAN_TCU6_20[2].byte) & (0x0F));/* storing Lower nibble of  */
						if(TGS_LEVER1 == 0x09)	/*P*/
						{
							AT_Gear_display = 0;
						}
						if(TGS_LEVER1 == 0x08) /*R*/
						{
							AT_Gear_display = 1;
						}
						if(TGS_LEVER1 == 0x07)	/*N*/
						{
							AT_Gear_display = 2;
						}
						if(TGS_LEVER1 == 0x06)	/*D*/
						{
							AT_Gear_display = 3;
						}
						if(TGS_LEVER1 == 0x0C)	/*M*/
						{
							AT_Gear_display = 4;
						}
//						start_AT_alert_timeout_cnt=1;			/*If alert is on screen & gear is changed then timeout timer reset*/
						AT_gear_update_cnt = 0;
					}
				}

/************************************ AT gear timeout duration **********************************/
				if(start_AT_alert_timeout_cnt > 0)
				{
					start_AT_alert_timeout_cnt++;	/*Each count increment @ 10 ms counter*/
				}

				if((start_AT_alert_timeout_cnt>=(Diag_AT_GEAR_ALERT_Duration/10))&&(Diag_AT_GEAR_ALERT_Duration!=0))
				{
					start_AT_alert_timeout_cnt=0;
					Priority_flag = 0;
					ATgear_popup_shown = 1;
					dont_throw_alerts_alert_on_screen_present=0;
					AT_gear_IGN_ON = 0;
					AT_gear_alert_active = 0;			/*AT_gear Alert is inactive*/
					AT_gearalert_reverse_display = 0;	/*Flag disable to allow RPAS on display*/
					(void)drome_sendEventToDrome((U32)drome_InEvent_AT_Alert_Timeout);
//					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
				}
#endif
/*****************************************************************************************/
#if 0
				/*pattern 4 50 ms ON- 1000 ms OFF*/
				if(CounterTTP_50ms >= 5)
				{
					if((telltale_blink_flag_patt4 == 0) && (CounterTTP_1000ms != 0))
					{
						telltale_blink_flag_patt4 = ~telltale_blink_flag_patt4;
						scan_TT_status();
						DIAG_TT_init();
						output_shift_register();
					}

					CounterTTP_1000ms++;
					if(CounterTTP_1000ms >= 20)
					{
						telltale_blink_flag_patt4=0x00;
						scan_TT_status();
						DIAG_TT_init();
						CounterTTP_1000ms=0;
					}
					CounterTTP_50ms=0;
				}
#endif
//input_shift_register();
				//	scan_TT_status();
				//	DIAG_TT_init();

				CounterTT_10ms   = CLEAR;

#if 1
				int_20ms_IFE++;
				int_50ms_IFE++;

				//IFE
				if((EMS4_20_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0)
						&& (AFE_3_U.U8_STS_Eng == 2)	&& (Speed_Missing == 0))
				{
					if(int_20ms_IFE >= 1)
					{
						int_20ms_IFE=0;

						// IFE claculation
						if(vehicle_moving_state == 1)  //vehicle moving status
						{

							//IFE_1_U.B_EMSCode = 1 ; //VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM;                 /* '<Root>/B_EMSCode' */
							IFE_3_Code_U.B_EMSCode = VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM ;
							IFE_3_Code_U.U8_IgnMode=1;                  /* '<Root>/U8_IgnMode' */
							IFE_3_Code_U.U16_FuelConsRate=(uint16_t)(Fuel_consm_rate * Fuel_Consp_Factor);           /* '<Root>/U16_FuelConsRate' */
							IFE_3_Code_U.U32_OdoDisp=final_odomter_distance;                /* '<Root>/U32_OdoDisp' */
							IFE_3_Code_U.U16_VehSpeed=final_vechile_speed;               /* '<Root>/U16_VehSpeed' */
							IFE_3_Code_U.U8_STS_Eng=three_bit_AFE.byte;                  /* '<Root>/U8_STS_Eng' */
							IFE_3_Code_U.U16_IFECalTIme= Diag_RW_IFE_Sample_Time;             /* '<Root>/U16_IFECalTIme' */
							IFE_3_Code_U.U16_IFESampTime=Diag_RW_IFE_Sample_Time;            /* '<Root>/U16_IFESampTime' */
							IFE_3_Code_U.U16_IFEAccTime=Diag_RW_IFE_Accum_Time;             /* '<Root>/U16_IFEAccTime' */
							IFE_3_Code_U.U8_IFEUnit= 0; //Diag_RW_IFE_Units;                  /* '<Root>/U8_IFEUnit' */
							IFE_3_Code_U.U8_IFEMax=Diag_RW_IFE_Max;                   /* '<Root>/U8_IFEMax' */
							IFE_3_Code_U.U8_IFESampSize=Diag_RW_IFE_Sample_Size;              /* '<Root>/U8_IFESampSize' */
							IFE_3_Code_U.U8_LHVSD=0;                  /* '<Root>/U8_LHVSD' */
							IFE_3_Code_U.U16_IFEDispTime=Diag_RW_IFE_Display_Time;            /* '<Root>/U16_IFEDispTime' */
							IFE_3_Code_U.B_IFEEnable =FEATURE_CODE_IFE_IND;               /* '<Root>/B_IFEEnable' */
							IFE_3_Code_U.T_stepTime = 20;
							IFE_3_Code_U.U8_Eng_RPM =1 ;//FEATURE_CODE_IFE_IND;
							IFE_3_Code_U.U32_Diag_RW_Fuel_Bias =1 ;//FEATURE_CODE_IFE_IND;
							IFE_3_Code_U.U8_RW_IFE_Unit =1 ;//FEATURE_CODE_IFE_IND;

							//IFE_1_U.B_EMSCode1 = VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM ;
							IFE_3_Code_step();
							//	rt_OneStep();

							IFE_Out = IFE_3_Code_Y.U32_IFEAvgVal;
							if(Diag_Unit_Selection == 1)
							{
								IFE_Out = (100/IFE_Out);	/*L/100km*/
							}
							else if(Diag_Unit_Selection == 2)	/*MPG US*/
							{
								IFE_Out = ((real32_T)IFE_Out * (real32_T) 2.3521);
							}
							else if (Diag_Unit_Selection == 3) /*MPG UK*/
							{
								IFE_Out = ((real32_T)IFE_Out * (real32_T) 2.8248);
							}

							if(int_50ms_IFE >= 5)
							{
								int_50ms_IFE=0;

								if(IFE_3_Code_Y.U32_IFE_Disp_Val == IFE_Out_DISPLAY1)
								{
									IFE_Out_DISPLAY1 = IFE_3_Code_Y.U32_IFE_Disp_Val;
								}
								else if(IFE_3_Code_Y.U32_IFE_Disp_Val > IFE_Out_DISPLAY1)
								{
									IFE_Out_DISPLAY1  ++;
								}
								else if(IFE_3_Code_Y.U32_IFE_Disp_Val < IFE_Out_DISPLAY1)
								{
									IFE_Out_DISPLAY1  --;
								}
								IFE_out_CAN = IFE_Out_DISPLAY;
							}
							if(Diag_Unit_Selection == 1)
							{
								if(IFE_Out_DISPLAY1 < 1)
								{
									IFE_Out_DISPLAY = 1;
								}
								else
								{
									IFE_Out_DISPLAY = (100/IFE_Out_DISPLAY1);	/*L/100km*/
								}

							}
							else if(Diag_Unit_Selection == 2)	/*MPG US*/
							{
								if(IFE_Out_DISPLAY1 < 1)
								{
									IFE_Out_DISPLAY = 1;
								}
								else
								{
									IFE_Out_DISPLAY = ((real32_T)IFE_Out_DISPLAY1 * (real32_T) 2.3521);
								}
							}
							else if (Diag_Unit_Selection == 3) /*MPG UK*/
							{
								if(IFE_Out_DISPLAY1 < 1)
								{
									IFE_Out_DISPLAY = 1;
								}
								else
								{
									IFE_Out_DISPLAY = ((real32_T)IFE_Out_DISPLAY1 * (real32_T) 2.8248);
								}
							}
							else
							{
								if(IFE_Out_DISPLAY1 < 1)
								{
									IFE_Out_DISPLAY = 1;
								}
								else
								{
									IFE_Out_DISPLAY = (real32_T)IFE_Out_DISPLAY1;
								}
							}
							//IFE_Out_DISPLAY = IFE1_Y.U32_IFE_DispVal;
						}
						else
						{
							IFE_Out =0x0;
							IFE_out_CAN = 0x0;
							if(Diag_Unit_Selection == 1)
							{
								IFE_Out_DISPLAY = Diag_RW_IFE_Max;
							}
							else
							{
								IFE_Out_DISPLAY = 1;
							}
						}
					}
				}
				else
				{
					IFE_Out = 0x1FF;
					IFE_out_CAN = 0x1FF;
					IFE_Out_DISPLAY = 0;
					IFE_Out_DISPLAY1 = 0;
				}
				// IC1_100: tx IFE
				if((IFE_out_CAN > 300) && (IFE_out_CAN != 0x1FF))
				{
					IFE_out_CAN=300;
				}
				if((EMS4_20_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0)
						&& (AFE_3_U.U8_STS_Eng == 2)	&& (Speed_Missing == 0))
				{
					if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM ==1 )
					{
						IFE_out_CAN1 = IFE_out_CAN*10;

						if((Diag_Unit_Selection == 1)||(Diag_Unit_Selection == 0))
						{
							if(IFE_out_CAN1 >= 300)
							{
								IFE_out_CAN1 = 300;
							}
						}
						else if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))
						{
							if(IFE_out_CAN1 >= 500)
							{
								IFE_out_CAN1 = 500;
							}
						}
						CAN_IC1_100[2].byte = (IFE_out_CAN1/**10*/);
						if((IFE_out_CAN1/**10*/) & 0x100)
						{
							CAN_IC1_100[3].bits.Rx_1 = 1;
						}
						else
						{
							CAN_IC1_100[3].bits.Rx_1 = 0;
						}
					}
				}
				else
				{
					CAN_IC1_100[2].byte = 0xFF;
					CAN_IC1_100[3].bits.Rx_1 = 1;
				}
#endif
				//IFE End

				//IVN Started
				int_20ms_IVN++;

				if(int_20ms_IVN >= 1)
				{
					int_20ms_IVN=0;
					int_100ms_IVN++;

					//Inputs
					IVN_SecurityAlgorithm_U.B_RxMAC_Indicator = IVN_RxMAC_Indicator;         	/* CAN boolean_T'<Root>/B_RxMAC_Indicator' */
					IVN_SecurityAlgorithm_U.U32_SecurityKeyConst = IVN_SecurityKeyConst.u32;     	/* XLX 0108 uint32_T '<Root>/U32_SecurityKeyConst' */
					IVN_SecurityAlgorithm_U.U32_SecurityKey_1 = IVN_SecurityKey_1.u32;        	  	/* XLX 0109  uint32_T '<Root>/U32_SecurityKey_1' */
					IVN_SecurityAlgorithm_U.U32_SecurityKey_2 = IVN_SecurityKey_2.u32;          	/* XLX 010A uint32_T '<Root>/U32_SecurityKey_2' */
					IVN_SecurityAlgorithm_U.U8_IgnMode = 1;                						/* boolean_T '<Root>/U8_IgnMode' */
					IVN_SecurityAlgorithm_U.U32_EEFreshnessCounter = IVN_EEFreshnessCounter;    /* EEPROM uint32_T'<Root>/U32_EEFreshnessCounter' */
					IVN_SecurityAlgorithm_U.U32_MacRxBuffer = IVN_MacRxBuffer.u32;          	/* CAN uint32_T '<Root>/U32_MacRxBuffer' */
					IVN_SecurityAlgorithm_U.B_SecurityEnableFlag = IVN_NS_VAR_CODING_STATUS_OF_VARIANT_CODE;      /* XLX 0107 boolean_T '<Root>/B_SecurityEnableFlag' */
					IVN_SecurityAlgorithm_U.U8_RxNSMMsgArrive = IVN_RxNSMMsgArrive;           	/* CAN uint8_T'<Root>/U8_RxNSMMsgArrive' */
					IVN_SecurityAlgorithm_U.U8_RxSynchMsgArrive = IVN_RxSynchMsgArrive;         /* CAN uint8_T'<Root>/U8_RxSynchMsgArrive' */
					IVN_SecurityAlgorithm_U.U8_ConfigStepTime = IVN_ConfigStepTime;          	/* 10 ms uint16_T '<Root>/U8_ConfigStepTime' */
					IVN_SecurityAlgorithm_U.U8_cfg_FCIncCntrTiTh_C = IVN_cfg_FCIncCntrTiTh_C;   /* XLX 010B 0 uint8_T '<Root>/U8_cfg_FCIncCntrTiTh_C' */
					IVN_SecurityAlgorithm_U.U8_cfg_SynchWndTiTh_C = IVN_cfg_SynchWndTiTh_C;     /* XLX 010B 9 uint8_T '<Root>/U8_cfg_SynchWndTiTh_C' */
					IVN_SecurityAlgorithm_U.U8_cfg_FCDevTh_C = IVN_cfg_FCDevTh_C;            	/* XLX 010B 1 uint8_T '<Root>/U8_cfg_FCDevTh_C' */
					IVN_SecurityAlgorithm_U.U8_cfg_FcErrReptDec_C = IVN_cfg_FcErrReptDec_C;     /* XLX 010B 4 uint8_T'<Root>/U8_cfg_FcErrReptDec_C' */
					IVN_SecurityAlgorithm_U.U8_cfg_FcErrHealDec_C = IVN_cfg_FcErrHealDec_C;     /* XLX 010B 5 uint8_T '<Root>/U8_cfg_FcErrHealDec_C' */
					IVN_SecurityAlgorithm_U.U8_cfg_FcDifErrReptTh_C = IVN_cfg_FcDifErrReptTh_C; /* XLX 010B 2 uint8_T '<Root>/U8_cfg_FcDifErrReptTh_C' */
					IVN_SecurityAlgorithm_U.U8_cfg_FcErrReptInc_C = IVN_cfg_FcErrReptInc_C;     /* XLX 010B 3 uint8_T'<Root>/U8_cfg_FcErrReptInc_C' */
					IVN_SecurityAlgorithm_U.U32_MAC0RxBuffer = IVN_MAC0RxBuffer.u32;           	/* CAN Sync uint32_T'<Root>/U32_MAC0RxBuffer' */

					IVN_SecurityAlgorithm_step();

					//Outputs
					IVN_EEFreshnessCounter = IVN_SecurityAlgorithm_Y.U32_FreshnessCounter;        /* uint32_T'<Root>/U32_FreshnessCounter' */
					IVN_TxMAC_Indicator = IVN_SecurityAlgorithm_Y.B_TxMAC_Indicator;         	/* boolean_T'<Root>/B_TxMAC_Indicator' */
					IVN_CANSignal_State = IVN_SecurityAlgorithm_Y.U8_CANSignal_State;           /* uint8_T'<Root>/U8_CANSignal_State' */
					IVN_TransmitCANFrame = IVN_SecurityAlgorithm_Y.B_TransmitCANFrame;          /* boolean_T'<Root>/B_TransmitCANFrame' */
					IVN_MacTxBuffer.u32 = IVN_SecurityAlgorithm_Y.U32_MacTxBuffer;              /* uint32_T '<Root>/U32_MacTxBuffer' */
					IVN_LimpHome_flg = IVN_SecurityAlgorithm_Y.B_Ivn_LimpHome;                  /* boolean_T'<Root>/B_Ivn_LimpHome' */
					IVN_Mac2TxBuffer.u32 = IVN_SecurityAlgorithm_Y.U32_Mac2TxBuffer;            /* uint32_T '<Root>/U32_Mac2TxBuffer' */
					IVN_FcErrorStatusBit = IVN_SecurityAlgorithm_Y.B_FcErrorStatusBit;          /* boolean_T'<Root>/B_FcErrorStatusBit' */
					IVN_NsmDTCStatus = IVN_SecurityAlgorithm_Y.B_NsmDTCStatus;                  /* boolean_T '<Root>/B_NsmDTCStatus' */

					//	Store in EEPROM
					//	IVN_FreshnessCounter

					if(int_100ms_IVN >= 9)
					{
						if((IVN_TransmitCANFrame == 1))
						{
							if(IVN_TxMAC_Indicator == 0)
							{
								//tx --  U32_MacTxBuffer
								CAN_IC_NSM[4].bits.Rx_1=IVN_TxMAC_Indicator;

								CAN_IC_NSM[0].byte = IVN_MacTxBuffer.u8[0];
								CAN_IC_NSM[1].byte = IVN_MacTxBuffer.u8[1];
								CAN_IC_NSM[2].byte = IVN_MacTxBuffer.u8[2];
								CAN_IC_NSM[3].byte = IVN_MacTxBuffer.u8[3];
							}
							else if(IVN_TxMAC_Indicator == 1)
							{
								//tx --  		U32_Mac2TxBuffer
								CAN_IC_NSM[4].bits.Rx_1=IVN_TxMAC_Indicator;

								CAN_IC_NSM[0].byte = IVN_Mac2TxBuffer.u8[0];
								CAN_IC_NSM[1].byte = IVN_Mac2TxBuffer.u8[1];
								CAN_IC_NSM[2].byte = IVN_Mac2TxBuffer.u8[2];
								CAN_IC_NSM[3].byte = IVN_Mac2TxBuffer.u8[3];
							}
							else
							{
								memset(CAN_IC_NSM, 0, sizeof(CAN_IC_NSM));
							}
						}
						else
						{
							memset(CAN_IC_NSM, 0, sizeof(CAN_IC_NSM));
						}
						/*						if((disable_can_tx_flag_req==CLEAR)&&(disable_can_tx_flag==CLEAR))
						{
							my_can0_tx_IC_NSM(IC_NSM,8,CAN_IC_NSM);
							int_100ms_IVN=0;
						}
						 */
					}
				}

				LVSD_SET_Flag=0;
				//IVN END

				//DTE Started
				int_20ms_DTE++;
#if 1
				if((int_20ms_DTE >= 1)/*&&(IGN_ON_flag == 1)*/&&(logo_completed==1))
				{
					uint16_t Calculated_DTE_1km;
					int_20ms_DTE=0;
					//input

					rtU.U16_FuelConsRate= (uint16_t)(((Fuel_consm_rate)/**(1.11)*/*(Fuel_Consp_Factor))* DTE_ACCRCY_FACT);  /* '<Root>/U16_FuelConsRate' */
					rtU.U32_OdoDisp= final_odomter_distance;                /* '<Root>/U32_OdoDisp' */
					rtU.U8_IgnMode= IGN_ON_flag;  //1                /* '<Root>/U8_IgnMode' */
					rtU.U8_STS_Eng= three_bit_AFE.byte;                  /* '<Root>/U8_STS_Eng' */
					rtU.U16_VehSpeed= final_vechile_speed;               /* '<Root>/U16_VehSpeed' */
					rtU.U8_Default_RAFE= DTE_DEFAULT_FE;              /* '<Root>/U8_Defaul_RAFE' */
					rtU.U8_RAFE_MAX_ROL_DIST= DTE_AFE_AVG_KM;//0;//0 is dummy value        /* '<Root>/U8_RAFE_MAX_ROL_DIST' */
					rtU.U8_DTE_FILTER_C= DTE_DISPLAY_FILTER_C;             /* '<Root>/U8_DTE_FILTER_C' */
					rtU.U8_DTE_FILTER_B= DTE_DISPLAY_FILTER_B;             /* '<Root>/U8_DTE_FILTER_B' */
					rtU.U8_DTE_FILTER_A= DTE_DISPLAY_FILTER_A;            /* '<Root>/U8_DTE_FILTER_A' */
					rtU.B_LHVSD= 0;                   /* '<Root>/B_LHVSD' */
					rtU.B_VAR_CODE_EMS= VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM;            /* '<Root>/B_VAR_CODE_EMS' */
					rtU.B_FEATR_CODE_DTE_IN= FEATURE_CODE_DTE_IND;       /* '<Root>/B_FEATR_CODE_DTE_IN' */
					//					rtU.U8_Fuel_Sendr_AD_In= ;         /* '<Root>/U8_Fuel_Sendr_AD_In' */	Not required
					//					rtU.B_DTE_Reset_Trig= 0;          /* '<Root>/B_DTE_Reset_Trig' */	Not required
					//					rtU.U8_Fuel_Used_Bias= ;           /* '<Root>/U8_Fuel_Used_Bias' */ Not required
					rtU.S_Fuel_Level_Ind= (float)(FUEL_LEVEL_FOR_DTE)/(float)(1000);           /* '<Root>/S_Fuel_Level_Ind' */
					rtU.S_Fuel_Dead_Vol= (Diag_RW_Fuel_Dead_Volume/1000);            /* '<Root>/S_Fuel_Dead_Vol' */	//by_defaul_5
					rtU.B_DTE_Fast_Flng_Mode= (Fast_fill_Started|fast_drain|DTE_Update);      /* '<Root>/B_DTE_Fast_Flng_Mode' */
					rtU.B_DTE_CAN_Msg_Misng= EMS4_20_message_timeout_flag;       /* '<Root>/B_DTE_CAN_Msg_Misng' */
					rtU.U32_DTE_StepTimeContr = U32_DTE_Step_Cntr;
					rtU.U16_FAST_FIL_SPD_LMT = 0;	//limit is less than 1

					if((flag_DTE_Enable==TRUE) && (u8_DTE_Fuel_En == 1U) )
					{
						if(u8_App_DTE_Reset == 0U)
						{
							if(U8_Reset_DTE != 0U)
							{
								U8_Reset_DTE = 1U;
							}
							else
							{
								U8_Reset_DTE = 0U;
							}
							rtU.B_DTE_Reset_Trig = U8_Reset_DTE;
							u8_App_DTE_Reset = 1U;
						}
						else
						{
							U8_Reset_DTE = 0U;
							rtU.B_DTE_Reset_Trig = 0U;
						}

						//WDTA1WDTE = 0xAC;
						//WDT_Counter = 0;
						if((from_recovery == 1)&&(Display_DTE == 0))//To reset DTE After recovery from Open short conditions
						{
							U8_Reset_DTE = 1U;
							rtU.B_DTE_Reset_Trig = U8_Reset_DTE;
						}
						else
						{
						/*not req as its alredy there after DTE reset*/
//							U8_Reset_DTE = 0U;
//							rtU.B_DTE_Reset_Trig = U8_Reset_DTE;
						}
						ModelRunCheck_flag = 0;
						DTE_V_1_step();
						ModelRunCheck_flag = 1;
						DTE_Update = 0;//update DTE set once after fuel bar changes in IGN off to On.
						U8_Reset_DTE = 0;
						rtU.B_DTE_Reset_Trig = U8_Reset_DTE;
						//New_eeprom_memwrite_32bytes(RESET_DTE_OFFSET, &U8_Reset_DTE, 1);
					}
					rtU.U32_DTE_StepTimeContr = 0U;
					U32_DTE_Step_Cntr = 0;
					//output

					DTE_RAFE_Val = rtY.S_Cur_1km_DTE_RAFE_Val;     /* '<Root>/S_Cur_1km_DTE_RAFE_Val' */
					DTE_1km = rtY.S_DTE_1km;                  /* '<Root>/S_DTE_1km' */
					DTE_RAFE_Avg_Val = rtY.S_DTE_RAFE_Avg_Val;         /* '<Root>/S_DTE_RAFE_Avg_Val' */
					Display_DTE = rtY.S_DTE_Disp_Val;             /* '<Root>/S_DTE_Disp_Val' */
					DTE_Disp_Val = Display_DTE;
					//Display_DTE = rtY.S_DTE_1km;  		// editedd for testing 031219
					//					if(((Display_DTE >0)&&(Display_DTE < 1000)) /*&& ( rtY.S_DTE_1km_Completion_Flag == 1U)*/)
					//					{
					//						DTE_Disp_Val = Display_DTE;
					//					}
					//					else
					//					{
					//						DTE_Disp_Val = ((FUEL_LEVEL_FOR_DTE/1000)- 5)*DTE_DEFAULT_FE;              /* '<Root>/U8_Defaul_RAFE' */
					//
					//					}
					FUEL_IN_IGN.u32 = FUEL_LEVEL_FOR_DTE;

					if((fuel_fault_30sec == 1)||(Speed_Invalid_Default == 1)||(Dist_Invalid_Default == 1)||((CAN_EMS4_20[2].byte == 0xFF)&&(CAN_EMS4_20[3].byte == 0xFF))||(EMS4_20_message_timeout_flag != 0)||(Speed_Missing == 1))
					{
						DTE_Signal_invalid = 1;
					}
					//else if((fuel_widget == 0)&&(fuel_fault == 0))
					else if((fuel_widget == 0)&&(fuel_fault_30sec == 0)&&(Fuel_init_done == 1))
					{
						DTE_Signal_invalid = 1;
					}
					else
					{
						DTE_Signal_invalid = 0;
					}

					if(DTE_Signal_invalid == 1)
					{
						CAN_IC1_100[0].byte = 0xFF;
						CAN_IC1_100[1].bits.Rx_1 = 0x01;
						CAN_IC1_100[1].bits.Rx_2 = 0x01;
					}
					else
					{
						if(DTE_Disp_Val > 999)
						{
							DTE_Disp_Val = 999;//0	DTE Roll over not req.
						}
						CAN_IC1_100[0].byte = DTE_Disp_Val;
						CAN_IC1_100[1].bits.Rx_1 = DTE_Disp_Val>>8;
						CAN_IC1_100[1].bits.Rx_2 = DTE_Disp_Val>>9;
					}

					//Calculated_DTE_1km = (uint8_t)DTE_1km*1;//1.11;				//multiplication factor for time match;
					Calculated_DTE_1km = (uint16_t)rtY.S_DTE_1km*1;//1.11;				//multiplication factor for time match;

					if(Diag_Unit_Selection == 1)
					{
						Calculated_DTE_1km = (100/Calculated_DTE_1km);	/*L/100km*/
					}
					else if(Diag_Unit_Selection == 2)	/*MPG US*/
					{
						Calculated_DTE_1km = ((real32_T)Calculated_DTE_1km * (real32_T) 2.3521);
					}
					else if (Diag_Unit_Selection == 3) /*MPG UK*/
					{
						Calculated_DTE_1km = ((real32_T)Calculated_DTE_1km * (real32_T) 2.8248);
					}

					// Check the Min Max limit for the variables
					//					  if(Calculated_DTE_1km>DTE_AFE_MAX)
					//					  {
					//						  Calculated_DTE_1km=DTE_AFE_MAX;
					//					  }

					if(fuel_error == 1)
					{
						Calculated_DTE_1km = 0x3FF;// send Fail safe 0x3FF on CAN
					}
					else if(Calculated_DTE_1km>999)
					{
						Calculated_DTE_1km = 999;
					}
					CAN_IC7_1000_TEST[3].bits.Rx_2 = Calculated_DTE_1km;
					CAN_IC7_1000_TEST[3].bits.Rx_3 = Calculated_DTE_1km>>1;
					CAN_IC7_1000_TEST[3].bits.Rx_4 = Calculated_DTE_1km>>2;
					CAN_IC7_1000_TEST[3].bits.Rx_5 = Calculated_DTE_1km>>3;
					CAN_IC7_1000_TEST[3].bits.Rx_6 = Calculated_DTE_1km>>4;
					CAN_IC7_1000_TEST[3].bits.Rx_7 = Calculated_DTE_1km>>5;
					CAN_IC7_1000_TEST[3].bits.Rx_8 = Calculated_DTE_1km>>6;
					CAN_IC7_1000_TEST[4].bits.Rx_1 = Calculated_DTE_1km>>7;
					CAN_IC7_1000_TEST[4].bits.Rx_2 = Calculated_DTE_1km>>8;
					CAN_IC7_1000_TEST[4].bits.Rx_3 = Calculated_DTE_1km>>9;
				}
				//DTE Ended
#endif
			}
			/*****************************************************/
			/*if(AFE_start == 1)*/
			{
				//				int_20ms_AFE++;
				//				if((EMS4_20_message_timeout_flag == 0) && (Speed_Missing == 0)&&(EMS1_10_message_timeout_flag == 0))
				if((Speed_Invalid_Default == 1)||(Dist_Invalid_Default == 1)||((CAN_EMS4_20[2].byte == 0xFF)&&(CAN_EMS4_20[3].byte == 0xFF))||(EMS4_20_message_timeout_flag != 0)||(Speed_Missing == 1))
				{
					AFE_3_U.B_CanRxFlag = 0U;//1;  //  GG-108/1 a. Calculation shall pause if any of the inputs (distance or fuel consumption or speed) is missing or Invalid and 0x1FF shall be sent on AVG_KMPL. “- - . -”
					NotdisplayAFE=1;
					AFE_signal_invalid = 1;
				}
				else if(((EMS1_10_message_timeout_flag != 0)||(Tacho_invalid_default	== 1)||(final_tachometer_speed > 7999))&&(NotdisplayAFE != 1))
				{
					AFE_3_U.B_CanRxFlag = 0U;//0U;	// GG-108/1 b.AFE data accumulation shall NOT happen if engine RPM is missing, invalid or out of range
					NotdisplayAFE=0;
					AFE_signal_invalid = 0;
				}
				else
				{
					AFE_3_U.B_CanRxFlag = 1U;//0U;	// Integrated as for missing conditions not given  031219
					//					NotdisplayAFE=0;
					AFE_signal_invalid = 0;
				}

				if(int_20ms_AFE >= 250)	/*250 = 20ms*/
				{
					/*AFE_COUNTER++;
						if(AFE_COUNTER >= 500)
						{
							AFE_COUNTER = 0;
						}*/
					int_20ms_AFE=0;
					//AFE data
					AFE_3_U.U8_LHVSD=LVSD_SET_init;  // temp
					//	rtU.B_EnbleAFE = FEATURE_CODE_AFE_IND;
					AFE_3_U.B_EnbleAFE = FEATURE_CODE_AFE_IND; /*******/
					//						AFE_3_U.B_EnbleAFE = 1;
					AFE_3_U.B_EMSCode = VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM;
					AFE_3_U.U16_VehSpeed = final_vechile_speed;
					AFE_3_U.U8_IgnMode = IGN_ON_flag;	// integrated as it was given hardcoded value. 031219
					AFE_3_U.U32_OdoDisp = final_odomter_distance;
					//AFE_3_U.B_CanRxFlag = 1;
					Fuel_scale.b[0] = CAN_EMS4_20[2].byte;
					Fuel_scale.b[1] = CAN_EMS4_20[3].byte;
					Fuel_consm_rate = Fuel_scale.w;
					/*AFE_3_U.U16_FuelConsRate = (uint16_t)(((Fuel_consm_rate)*(1.11)*(Fuel_Consp_Factor))/AFE_ACCRCY_FACT);*/
					AFE_3_U.U16_FuelConsRate = (uint16_t)(((Fuel_consm_rate)*(Fuel_Consp_Factor)) * AFE_ACCRCY_FACT);
					/*if(AFE_3_U.U16_FuelConsRate < 1U)
					{
						AFE_3_U.U16_FuelConsRate = 1U;	//Commented on 15-06-2020
					}*/

					//					rtU.B_InfoResetAFE = CAN_IS1_100[7].bits.Rx_8;    //IS1_100: RESET_AFE
					if((CAN_IS1_100[7].bits.Rx_8 == 1)||(Flag_AFE_reset == 1))
					{

						//							AFE_3_U.B_InfoResetAFE = 1;
						AFE_3_U.B_ReqTripResetAFE = 1;
					}
					else
					{
						//							AFE_3_U.B_InfoResetAFE = 0;
						AFE_3_U.B_ReqTripResetAFE = 0;
					}
					//					rtU.B_ReqTripResetAFE = CAN_IS1_100[7].bits.Rx_1; //IS1_100: REQ_TRIP_RESET
					if((CAN_IS1_100[7].bits.Rx_1 == 1)||(Flag_AFE_reset == 1))
					{
						//							AFE_3_U.B_ReqTripResetAFE = 1;
						AFE_3_U.B_InfoResetAFE = 1;
					}
					else
					{
						//							AFE_3_U.B_ReqTripResetAFE = 0;
						AFE_3_U.B_InfoResetAFE = 0;
					}
					AFE_3_U.B_AFEResetTrg = Flag_AFE_reset;					//AFE KEY reset input

					if(ODO_write == 1)//added For AFE 30 after EEPROM write and Engine crank
					{
						AFE_3_U.U32_OdoDispResetEE = final_odomter_distance;
						AFE_3_U.U32_OdoDispGDResetEE = final_odomter_distance;
						AFE_3_Y.U32_OdoDispReset = final_odomter_distance;
						AFE_3_Y.U32_OdoDispGDReset = final_odomter_distance;
						//ODO_write = 0;
					}

					if((AFE_3_U.B_InfoResetAFE ==1 ) || (AFE_3_U.B_ReqTripResetAFE == 1) || (AFE_3_U.B_AFEResetTrg == 1)||(ODO_write == 1))
					{
						AFE_3_U.U32_OdoDispResetEE= AFE_3_Y.U32_OdoDispReset;          /* '<Root>/U32_OdoDispReset' */
						AFE_3_U.U32_OdoDispGDResetEE= AFE_3_Y.U32_OdoDispGDReset;		 /* '<Root>/U32_OdoDispGDReset' */
						eeprom_write_AFE_data(AFE_ODO_DISP_RESET_OFFSET,AFE_3_Y.U32_OdoDispReset);
						eeprom_write_AFE_data(AFE_ODO_DISP_RESET_GD_OFFSET,AFE_3_Y.U32_OdoDispGDReset);
						ODO_write = 0;//odo write and IGN ENG on issue solve
					}
					//	rtU.U8_STS_Eng = three_bit_AFE.byte & 0xE0;
					AFE_3_U.U8_STS_Eng = three_bit_AFE.byte;

					if((AFE_3_U.U8_STS_Eng == 2) && engRunCntr >= 250)//150
					{
						Enginerunningflag=1;
						Enginerun_chm=1;
						engRunCntr=251;//151
					}
					else if(AFE_3_U.U8_STS_Eng == 2 && engRunCntr < 250)//150
					{
						Enginerunningflag=0;
						engRunCntr++;
					}
					else
					{
						Enginerunningflag=0;
						engRunCntr=0;
						Enginerun_chm=0;
						engRunCntr = 0;
					}

					AFE_3_U.U8_AfeCalcInterv = AFE_CALC_INTERVAL;            /* Calculation interval of AFE*/
					AFE_3_U.U8_AfeDispIntev = AFE_DISPLAY_INTERVAL;         /* Display Interval of AFE */
					AFE_3_U.U8_AfeMIn = DTE_AFE_MIN;                   /* '<Root>/U8_AfeMIn' */
					AFE_3_U.U8_AfeMax = DTE_AFE_MAX;                   /* '<Root>/U8_AfeMax' */
					AFE_3_U.U8_AfeDispFilterA = AFE_DISPLAY_FILTER_A;///10;//model calculation is in KM           /* '<Root>/U8_AfeDispFilterA' */
					AFE_3_U.U8_AfeDispFilterB = AFE_DISPLAY_FILTER_B;           /* '<Root>/U8_AfeDispFilterB' */

					AFE_3_U.U8_AFE_DIST_TH = AFE_INIT_THRESHOLD_DISTANCE;              /* '<Root>/U8_AFE_DIST_TH' */

					AFE_3_step();

					CalAvgKmplAFE_GD = AFE_3_Y.U16_CalAvgKmplAFE_GD;       /* '<Root>/U16_CalAvgKmplAFE_GD' */
//					if(AFE_3_Y.U32_AccFuelConsp == 0)
//					{
//						AFE_3_Y.U32_AccFuelConsp = 1;
//					}
					AccFuelConsp = AFE_3_Y.U32_AccFuelConsp;           /* '<Root>/U32_AccFuelConsp' */
					AccDistAFE = AFE_3_Y.U32_AccDistAFE;             /* '<Root>/U32_AccDistAFE' */
					DrvAccFuelConsp=  AFE_3_Y.U32_DrvAccFuelConsp;        /* '<Root>/U32_DrvAccFuelConsp' */
					AFEDispVal = AFE_3_Y.U16_AFEDispVal;             /* '<Root>/U16_AFEDispVal' */
					CalAvgKmplAFE = AFE_3_Y.U16_CalAvgKmplAFE;          /* '<Root>/U16_CalAvgKmplAFE' */
					AvgKmplAFE_GD = AFE_3_Y.U16_AvgKmplAFE_GD;          /* '<Root>/U16_AvgKmplAFE_GD'*/
					B_AFEResetACK = AFE_3_Y.B_AFEResetACK;             /* '<Root>/B_AFEResetACK' */
					AccFuelConspReset = AFE_3_Y.U32_AccFuelConspReset;      /* '<Root>/U32_AccFuelConspReset' */
					AFECalTimer = AFE_3_Y.b_AFECalTimer;             /* '<Root>/b_AFECalTimer' */
					OdoDispReset = AFE_3_Y.U32_OdoDispReset;           /* '<Root>/U32_OdoDispReset' */
					OdoDispGDReset = AFE_3_Y.U32_OdoDispGDReset;         /* '<Root>/U32_OdoDispGDReset' */
					AccFuelConspGDReset = AFE_3_Y.U32_AccFuelConspGDReset;    /* '<Root>/U32_AccFuelConspGDReset' */
//					if(AFE_3_Y.U32_AccFuelConspGD == 0)
//					{
//						AFE_3_Y.U32_AccFuelConspGD = 1;
//					}
					AccFuelConspGD = AFE_3_Y.U32_AccFuelConspGD;         /* '<Root>/U32_AccFuelConspGD' */
					AccDistAFEGD = AFE_3_Y.U32_AccDistAFEGD;           /* '<Root>/U32_AccDistAFEGD' */

					// IC2_100: Displayed AVG_KMPL_GD
					if(AvgKmplAFE_GD != 0)
					{
						CAN_IC2_100[6].byte = AvgKmplAFE_GD;
						if(AvgKmplAFE_GD & 0x100)
						{
							CAN_IC2_100[7].bits.Rx_1 = 1;
						}
						else
						{
							CAN_IC2_100[7].bits.Rx_1 = 0;
						}
					}
					else
					{
						CAN_IC2_100[6].byte = 0xFF;		/*After AFE resetting CAN TX -> 0x1ff*/
						CAN_IC2_100[7].bits.Rx_1 = 1;
					}
					// IC7_1000_TEST: CALCULATED_AFE
					if(CalAvgKmplAFE > 511)
					{
						CalAvgKmplAFEtx = 511;
					}
					else
					{
						CalAvgKmplAFEtx = CalAvgKmplAFE;
					}
					if(NotdisplayAFE == 0)
					{
						CAN_IC7_1000_TEST[2].byte = CalAvgKmplAFEtx;

						if(CalAvgKmplAFEtx & 0x100)
						{
							CAN_IC7_1000_TEST[3].bits.Rx_1 = 1;
						}
						else
						{
							CAN_IC7_1000_TEST[3].bits.Rx_1 = 0;
						}
					}
					else
					{
						CAN_IC7_1000_TEST[2].byte = 0xFF;
						CAN_IC7_1000_TEST[3].bits.Rx_1 = 1;
					}
					// IC6_1000_TEST:CALCULATED_AFE_GD
					/*
								CAN_IC7_1000_TEST[2].byte = CalAvgKmplAFE;
								if(CalAvgKmplAFE & 0x100)
								{
									CAN_IC7_1000_TEST[3].bits.Rx_1 = 1;
								}
								else
								{
									CAN_IC7_1000_TEST[3].bits.Rx_1 = 0;
								}
					 */
					// IC4_1000_TEST:AFE_ACCUMULATED_FUEL_CONSMP
					//	AccFuelConspml = AccFuelConsp/1000;
					CAN_IC4_1000_TEST[5].byte = AccFuelConsp>>24;
					CAN_IC4_1000_TEST[4].byte = AccFuelConsp>>16;
					CAN_IC4_1000_TEST[3].byte = AccFuelConsp>>8;
					CAN_IC4_1000_TEST[2].byte = AccFuelConsp;

					// IC4_1000_TEST:AFE_ACCUMULATED_DISTANCE
					CAN_IC4_1000_TEST[1].byte = AccDistAFE>>8;
					CAN_IC4_1000_TEST[0].byte = AccDistAFE;

					// IC4_1000_TEST: FUEL_CONSMP_ACCUMULATED
					DrvAccFuelConspml = DrvAccFuelConsp/1000;

					CAN_IC4_1000_TEST[7].byte = DrvAccFuelConspml>>8;
					CAN_IC4_1000_TEST[6].byte = DrvAccFuelConspml;

					// IC1_100: RST_ACK_AFE

					// IC1_100: RST_ACK_AFE
					// Adding logic here to send AFE unit conversion values
					// Also need to consider Green Drive trigger condition from IS
					if(Diag_Unit_Selection == 1)
					{
						CalAvgKmplAFE_GD = (100/CalAvgKmplAFE_GD);	//L/100km
					}
					else if(Diag_Unit_Selection == 2)	// MPG US
					{
						CalAvgKmplAFE_GD = ((real32_T)CalAvgKmplAFE_GD * (real32_T) 2.3521);
					}
					else if (Diag_Unit_Selection == 3) // MPG UK
					{
						CalAvgKmplAFE_GD = ((real32_T)CalAvgKmplAFE_GD * (real32_T) 2.8248);
					}
					// Check the Min Max limit for the variables
					//						  if(CalAvgKmplAFE_GD>DTE_AFE_MAX)		not required for calculated value
					//						  {
					//							  CalAvgKmplAFE_GD=DTE_AFE_MAX;
					//						  }
					if(NotdisplayAFE == 0)
					{
						CAN_IC6_1000_TEST[6].bits.Rx_5 = CalAvgKmplAFE_GD;
						CAN_IC6_1000_TEST[6].bits.Rx_6 = CalAvgKmplAFE_GD>>1;
						CAN_IC6_1000_TEST[6].bits.Rx_7 = CalAvgKmplAFE_GD>>2;
						CAN_IC6_1000_TEST[6].bits.Rx_8 = CalAvgKmplAFE_GD>>3;
						CAN_IC6_1000_TEST[7].bits.Rx_1 = CalAvgKmplAFE_GD>>4;
						CAN_IC6_1000_TEST[7].bits.Rx_2 = CalAvgKmplAFE_GD>>5;
						CAN_IC6_1000_TEST[7].bits.Rx_3 = CalAvgKmplAFE_GD>>6;
						CAN_IC6_1000_TEST[7].bits.Rx_4 = CalAvgKmplAFE_GD>>7;
						CAN_IC6_1000_TEST[7].bits.Rx_5 = CalAvgKmplAFE_GD>>8;
					}
					if(((Flag_AFE_reset == 1)&&(NotdisplayAFE == 1))||(AFE_3_U.B_InfoResetAFE == 1))
					{
						CalAvgKmplAFE_GD = 0;
						AFE_3_Y.U16_CalAvgKmplAFE_GD = 0;
						CAN_IC6_1000_TEST[6].bits.Rx_5 = CalAvgKmplAFE_GD;
						CAN_IC6_1000_TEST[6].bits.Rx_6 = CalAvgKmplAFE_GD>>1;
						CAN_IC6_1000_TEST[6].bits.Rx_7 = CalAvgKmplAFE_GD>>2;
						CAN_IC6_1000_TEST[6].bits.Rx_8 = CalAvgKmplAFE_GD>>3;
						CAN_IC6_1000_TEST[7].bits.Rx_1 = CalAvgKmplAFE_GD>>4;
						CAN_IC6_1000_TEST[7].bits.Rx_2 = CalAvgKmplAFE_GD>>5;
						CAN_IC6_1000_TEST[7].bits.Rx_3 = CalAvgKmplAFE_GD>>6;
						CAN_IC6_1000_TEST[7].bits.Rx_4 = CalAvgKmplAFE_GD>>7;
						CAN_IC6_1000_TEST[7].bits.Rx_5 = CalAvgKmplAFE_GD>>8;
					}

					if(AFE_signal_invalid == 0)
					{
						if(B_AFEResetACK == 1)
						{
							resetackTx=1;
							Flag_AFE_reset = 0;
							AFE_rst_odo_dist = final_odomter_distance;
							NotdisplayAFE =1;
							AFEDispVal_init = 0;
							BATT_DISCONNECTION1 = 0;
						}
						else if(BATT_DISCONNECTION1 == 1)
						{
							Flag_AFE_reset= 1;

						}
						else
						{
							AFE_diff_odo_dist = final_odomter_distance - AFE_rst_odo_dist;

							if(AFE_diff_odo_dist <= 1000)
							{
								NotdisplayAFE =1;
							}
							else
							{
								NotdisplayAFE=0;
								AFE_diff_odo_dist=0;
							}
						}
					}

					if((NotdisplayAFE == 0)&&(AFEDispVal != 0))
					{
						// IC1_100: Displayed AVG_KMPL
						CAN_IC1_100[4].byte = AFEDispVal;
						if(AFEDispVal & 0x100)
						{
							CAN_IC1_100[5].bits.Rx_1 = 1;
						}
						else
						{
							CAN_IC1_100[5].bits.Rx_1 = 0;
						}
					}
					else if(NotdisplayAFE == 1)
					{
						CAN_IC1_100[4].byte = 0xFF;
						CAN_IC1_100[5].bits.Rx_1 = 1;
					}
					else
					{

					}

					//AFE Displayed Value
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[0] = CAN_IC1_100[4].byte;
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[1] = CAN_IC1_100[5].bits.Rx_1;
					//AFE Calculated Value
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[2] = CAN_IC7_1000_TEST[2].byte;
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[3] = CAN_IC7_1000_TEST[3].bits.Rx_1;

					DID_AFE_ECU_IDENTIFIER_0x0523_arr[4] = AccFuelConsp;
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[5] = AccFuelConsp>>8;
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[6] = AccFuelConsp>>16;
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[7] = AccFuelConsp>>24;	/*AccFuelConsp >> 32 corrected to >>24*/

					DID_AFE_ECU_IDENTIFIER_0x0523_arr[8] = AccDistAFE;
					DID_AFE_ECU_IDENTIFIER_0x0523_arr[9] = AccDistAFE>>8;

					//AFE GD Displayed Value
					DID_AFE_GD_ECUIdentifier_0x0524_arr[0] = CAN_IC2_100[6].byte;
					DID_AFE_GD_ECUIdentifier_0x0524_arr[1] = CAN_IC2_100[7].bits.Rx_1;
					//AFE Calculated Value
					DID_AFE_GD_ECUIdentifier_0x0524_arr[2] = CalAvgKmplAFE_GD;
					DID_AFE_GD_ECUIdentifier_0x0524_arr[3] = CalAvgKmplAFE_GD>>8;

					DID_AFE_GD_ECUIdentifier_0x0524_arr[4] = AccFuelConspGD;
					DID_AFE_GD_ECUIdentifier_0x0524_arr[5] = AccFuelConspGD>>8;
					DID_AFE_GD_ECUIdentifier_0x0524_arr[6] = AccFuelConspGD>>16;
					DID_AFE_GD_ECUIdentifier_0x0524_arr[7] = AccFuelConspGD>>24;	/*AccFuelConspGD >> 32 corrected to >>24*/

					DID_AFE_GD_ECUIdentifier_0x0524_arr[8] = AccDistAFEGD;
					DID_AFE_GD_ECUIdentifier_0x0524_arr[9] = AccDistAFEGD>>8;



					//AFE END
				}
#if 0
				else
				{
					NotdisplayAFE=1;
				}
#endif
			}
			/*****************************************************/
			//			}
		}
		else if(Proveout_Count > PROVEOUT_DONE)
		{
			AFE_3_U.U8_LHVSD=1;

			if(LVSD_SET_Flag == 0)
			{
				All_Telltale_OFF();
				LVSD_SET_Flag=1;
			}
		}
	}
	if((IGN_ON_flag == SET) && (LVSD_flag == 0U) && (HVSD_flag == 0U))
	{
		BootCounterTT_10ms++;
		if(BootCounterTT_10ms >= Welcome_indexrate/*180*//*200*/)
		{
			welcomecounter++;
			/************************************************************************************/
//			if((welcomecounter >= 10)&&(startkey == 0x13001))			/*Boot Up Animation - 100 ms for image change*/
			{
//				welcome_index++;

//				if(welcome_index >= 9)
				{
//					(void)drome_sendEventToDrome((U32)drome_InEvent_Start_animation);
					welcome_index = /*9*/30;

					if((first_screen_timeout>=first_screen_timeout_MAX) && (Proveout_Count > PROVEOUT_DONE/*Proveout_down*/))
					{
						first_screen_timeout=0;
//						(void)drome_sendEventToDrome((U32)drome_InEventMain_screen_timeout);	/*send timeout to drome to print next screen after logo*/
						(void)drome_sendEventToDrome((U32)drome_InEventMain_screen_timeout);	/*send timeout to drome to print next screen after logo*/
						animation_start = 0;
//						logo_completed=1;
					}


				}
				welcomecounter = 0;
			}
			BootCounterTT_10ms = 0;
		}
	}

	if(IGN_ON_flag == CLEAR)
	{
		ignOFF_dooropencounter++;
		IGN_OFF_10ms++;
		if(IGN_OFF_10ms >= 200)
		{
#ifdef GRAYSCALE_TFT
			IgnOFF_KIR_Counter++;
			if(IgnOFF_KIR_Counter >= 7)
			{
				if((KIR_Chime == 1)&&(Buzz_KIR_compl == 0))//key in reminder bit
				{
					if((Buzz_ON == 0) && (counter3 >= 4))//3
					{
						Buzz_ON=1;
						r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
						Sound_Bit = 0x80000;
						counter3=0;
						Buzz_KIR_cntr++;
					}
					else
					{
						Buzz_ON=0;
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						counter3++;
						if(Buzz_KIR_cntr > 9)//12
						{
							Buzz_KIR_compl=1;
							counter3 =0;
							//Buzz_High_Temp=0;
							Sound_Bit=0x0;
							priorityPark_Keyin = 0;

						}
					}
				}
				else if((KIR_Chime == 0)&&(Sound_Bit == 0x80000))
				{
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					Buzz_KIR_compl=1;
					//Buzz_High_Temp=0;
					Sound_Bit=0x0;
				}
				IgnOFF_KIR_Counter = 0;
			}
#endif
#ifdef COLOR_TFT
			IgnOFF_KIR_Counter++;
			if(IgnOFF_KIR_Counter >= 170)	/*Added to componsate for 4 sec time out of SRD */
			{
				if((KIR_Chime == 1)&&(Buzz_KIR_compl == 0)&&(cycleKIR_cntr < 2))	/*KIR*/
				{
					if(0 == chime_start1)	/*wait, if any chime is being Played */
					{
						SoundPCM_KEYIN_SUNROOF();
						chime_start1 = 1;		/* Set Flag to denote Chime play has been stared */
						Sound_Bit = 0x80000;
						cycleKIR_cntr++;
					}
				}
				IgnOFF_KIR_Counter = 0;

			}
			if(cycleKIR_cntr >= 2)
			{
				if(0 == chime_start1)	/* is last chime played , release Sound bit only after last chime is played */
				{
					cycleKIR_cntr = 0;
					Buzz_KIR_compl = 1;
					Sound_Bit=0x0;					/**Sound bit clear**/
					priorityPark_Keyin = 0;
				}
			}
			//			else if((cycleKIR_cntr > 0)&&(cycleKIR_cntr < 4))
			//			{
			//				chime_start1 = 0;
			//				cycleKIR_cntr = 0;
			//				lowfuel_missed = 0;
			//			}
#endif

			/*@@@ For Park Lamp ON Status*/
#ifdef GRAYSCALE_TFT
			IgnOFF_ParkLamp_Counter++;
			if(IgnOFF_ParkLamp_Counter >= 7/*9*/)		/* Count should be 70 ms ,but for req hardware output given as 90ms*/
			{
				if((parkLampChime == 1)&&(buzzParkLamp == 0))
				{
					if((Buzz_ON == 0) && (counter3ParkLamp >= 4))//3
					{
						Buzz_ON=1;
						r_taub_OutputEnable(2,R_TAUB_CHANNEL_13);
						Sound_Bit = 0x100000;
						counter3ParkLamp = 0;
						buzzParkLampCntr++;
					}
					else
					{
						Buzz_ON=0;
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						counter3ParkLamp++;
						if(buzzParkLampCntr > 9)//12
						{
							buzzParkLamp=1;
							counter3ParkLamp = 0;
							//Buzz_High_Temp=0;
							//buzzParkLampCntr = 0;
							Sound_Bit=0x0;
							priorityPark_Keyin = 0;

						}
					}
				}
				else if((parkLampChime == 0)&&(Sound_Bit == 0x100000))
				{
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					buzzParkLamp=1;
					//Buzz_High_Temp=0;
					Sound_Bit=0x0;

				}
				IgnOFF_ParkLamp_Counter = 0;
			}
#endif
#ifdef COLOR_TFT

			IgnOFF_ParkLamp_Counter++;
			if(IgnOFF_ParkLamp_Counter >= 170)
			{
				if((parkLampChime == 1)&&(buzzParkLamp == 0)&&(cycleParkLamp_cntr < 2))	/* Park Lamp Chime */
				{
					if(0 == chime_start1)	/*wait, if any chime is being Played */
					{
						SoundPCM_KEYIN_SUNROOF();
						chime_start1 = 1;		/* Set Flag to denote Chime play has been stared */
						Sound_Bit = 0x100000;
						cycleParkLamp_cntr++;
					}
				}
				IgnOFF_ParkLamp_Counter = 0;
			}
			if(cycleParkLamp_cntr >= 2)
			{
				if(0 == chime_start1)	/* is last chime played , release Sound bit only after last chime is played */
				{
					cycleParkLamp_cntr = 0;
					buzzParkLamp = 1;
					Sound_Bit=0x0;					/**Sound bit clear**/
					priorityPark_Keyin = 0;
				}

			}


			//			else if((cycleKIR_cntr > 0)&&(cycleKIR_cntr < 4))
			//			{
			//				chime_start1 = 0;
			//				cycleKIR_cntr = 0;
			//				lowfuel_missed = 0;
			//			}
#endif
			IGN_OFF_10ms = 0;
		}
		if(ignOFF_dooropencounter >= 2000/*15*/)				/*Door open Animation - 150 ms for image change*/
		{
			/***************************************************************************/
			if((Codriver_flag == 1)&&(Dooropen_IGNOFF_popup_shown ==1)/*&&(codoor_index == 0)*/)
			{
				if((Codriver_index != 0))
				{
					Codriver_index--;
					//Codriver_index = codoor_index;
					//Codoor_set = 0;
				}
				if(Codriver_index == 0)
				{
					Codriver_index = 0;
					Codoor_set = 1;
				}
				Codoor_display = 1;

			}
			else if((Codriver_flag == 0)&&(Codoor_set == 1))
			{
				if((Codriver_index != 4))
				{
					Codriver_index++;
					//Codriver_index = codoor_index;
					//Codoor_set = 0;
				}
				if(Codriver_index == 4)
				{
					Codriver_index = 4;
					Codoor_set = 0;
				}
				Codoor_display = 1;
			}
			else
			{
				Codoor_display = 0;
			}

			if((Driver_flag == 1)&&(Dooropen_IGNOFF_popup_shown ==1)/*&&(codoor_index == 0)*/)
			{
				if((Driver_index != 0))
				{
					Driver_index--;
					//Codriver_index = codoor_index;
					//Codoor_set = 0;
				}
				if(Driver_index == 0)
				{
					Driver_index = 0;
					driverdoor_set = 1;
				}
				Driver_display = 1;
			}
			else if((Driver_flag == 0)&&(driverdoor_set == 1))
			{
				if((Driver_index != 4))
				{
					Driver_index++;
					//Codriver_index = codoor_index;
					//Codoor_set = 0;
				}
				if(Driver_index == 4)
				{
					Driver_index = 4;
					driverdoor_set = 0;
				}
				Driver_display = 1;
			}
			else
			{
				Driver_display = 0;
			}

			if((Rearleft_flag == 1)&&(Dooropen_IGNOFF_popup_shown ==1)/*&&(codoor_index == 0)*/)
			{
				if((Rearleft_index != 0))
				{
					Rearleft_index--;

				}
				if(Rearleft_index == 0)
				{
					Rearleft_index = 0;
					Rearleftdoor_set = 1;
				}
				Rearleft_display = 1;

			}
			else if((Rearleft_flag == 0)&&(Rearleftdoor_set == 1))
			{
				if((Rearleft_index != 4))
				{
					Rearleft_index++;
					//Codriver_index = codoor_index;
					//Codoor_set = 0;
				}
				if(Rearleft_index == 4)
				{
					Rearleft_index = 4;
					Rearleftdoor_set = 0;
				}
				Rearleft_display = 1;
			}
			else
			{
				Rearleft_display = 0;
			}

			if((RearRight_flag == 1)&&(Dooropen_IGNOFF_popup_shown ==1)/*&&(codoor_index == 0)*/)
			{
				if((RearRight_index != 0))
				{
					RearRight_index--;

				}
				if(RearRight_index == 0)
				{
					RearRight_index = 0;
					RearRightdoor_set = 1;
				}
				Rearright_display = 1;

			}
			else if((RearRight_flag == 0)&&(RearRightdoor_set == 1))
			{
				if((RearRight_index != 4))
				{
					RearRight_index++;
					//Codriver_index = codoor_index;
					//Codoor_set = 0;
				}
				if(RearRight_index == 4)
				{
					RearRight_index = 4;
					RearRightdoor_set = 0;
				}
				Rearright_display = 1;
			}
			else
			{
				Rearright_display = 0;
			}

			if((Back_flag == 1)&&(Dooropen_IGNOFF_popup_shown ==1)/*&&(codoor_index == 0)*/)
			{
				if((Backd_index != 0))
				{
					Backd_index--;

				}
				if(Backd_index == 0)
				{
					Backd_index = 0;
					Backdoor_set = 1;
				}
				Back_display = 1;

			}
			else if((Back_flag == 0)&&(Backdoor_set == 1))
			{
				if((Backd_index != 4))
				{
					Backd_index++;
				}
				if(Backd_index == 4)
				{
					Backd_index = 4;
					Backdoor_set = 0;
				}
				Back_display = 1;
			}
			else
			{
				Back_display = 0;
			}



			ignOFF_dooropencounter = 0;
		}
		/*****************************************************************************/
		if(IMMO_TT_WAKEUP_FLAG==1)
		{
			if(IGNOFF_eeprom_read == 1)	/*Acknowledgement of FC VC read*/
			{
				IGN_OFF_IMMO(&UDS_Buffer[3]);//commented for testing
			}
		}
		else
		{
			IMMO_IGN_OFF=0;
			if((VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))
			{

				IMMO_CAN_DATA = CAN_MBFM1_100[3].byte;//can_buffer[0].byte;
				IMMO_DATA_BIT4 = ((IMMO_CAN_DATA >> IMMO_DATA_BIT4) & 1);
				IMMO_DATA_BIT5 = ((IMMO_CAN_DATA >> IMMO_DATA_BIT5) & 1);

				if(IMMO_DATA_BIT4==0 && IMMO_DATA_BIT5==0)
				{
					IMMOBIIZER_TT=0;
					previous_state_immobilizer_blink4=0;
					previous_state_immobilizer_blink5=0;
					IMMO_TT_WAKEUP_FLAG=0;
				}
				else if(IMMO_DATA_BIT4==1 && IMMO_DATA_BIT5==0)
				{
					IMMOBIIZER_TT=telltale_blink_bool4;
					previous_state_immobilizer_blink4=1;
					previous_state_immobilizer_blink5=0;
				}
				else if(IMMO_DATA_BIT4==0 && IMMO_DATA_BIT5==1)
				{
					IMMOBIIZER_TT=telltale_blink_bool5;		//blink_patt5
					previous_state_immobilizer_blink5=1;
					previous_state_immobilizer_blink4=0;
				}
				else if(IMMO_DATA_BIT4==1 && IMMO_DATA_BIT5==1)
				{
					if(previous_state_immobilizer_blink4==1)
					{
						IMMOBIIZER_TT=telltale_blink_bool4;
					}
					else if(previous_state_immobilizer_blink5==1)
					{
						IMMOBIIZER_TT=telltale_blink_bool5;
					}
					else
					{
						IMMOBIIZER_TT=IMMOBIIZER_TT;
					}
				}
			}
		}
		//		IGN_OFF_IMMO(&UDS_Buffer[3]);//for testing only


		if(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0)
		{
			IMMOBIIZER_TT=telltale_blink_bool4;
		}
		IGNOFF_Counterblink_500ms++;
		Counter_IMMO_40ms++;
		Counter_IMMO_80ms++;

		if(IGNOFF_Counterblink_500ms >= 10000)
		{
			toggle_timedots = toggle_timedots ^ 0x01;
			IGNOFF_Counterblink_500ms = 0;
		}
		//if(Counter_IMMO_40ms >= 700)
		if(Counter_IMMO_40ms >= Diag_RW_TT_PATTERN4_Immo_ON_Time*20)			/*50*20=1000  ,1000 cnt for 50ms*/
		{
			Counter_IMMO_1000ms++;
			if(telltale_blink_bool4 == 1)
			{

				telltale_blink_bool4 =0;
			}
			//if(Counter_IMMO_1000ms>=25)
			if(Counter_IMMO_1000ms>=Diag_RW_TT_PATTERN4_Immo_OFF_Time/50)			/*1000/50=20,   cnt for 1000ms*/
			{
				if(telltale_blink_bool4 == 0)
				{

					telltale_blink_bool4 =1;
				}
				Counter_IMMO_1000ms=0;

			}

			Counter_IMMO_40ms=0;
		}
		//if(Counter_IMMO_80ms >= 1600)
		if(Counter_IMMO_80ms >= Diag_RW_TT_PATTERN5_Immo_ON_Time*20)               /* 80*20=1600,cnt for 80ms*/
		{
			Counter_IMMO_2000ms++;
			if(telltale_blink_bool5 == 1)
			{

				telltale_blink_bool5 =0;
			}
			//	if(Counter_IMMO_2000ms>=25)
			if(Counter_IMMO_2000ms>=Diag_RW_TT_PATTERN5_Immo_OFF_Time/80)		/* 2000/80=25,cnt for 2000ms*/
			{
				if(telltale_blink_bool5 == 0)
				{

					telltale_blink_bool5 =1;
				}
				Counter_IMMO_2000ms=0;

			}

			Counter_IMMO_80ms=0;
		}
	}
}

void locTauj0ch0Isr()
{
	drome_count1us++;
}
/***********************************************************************************************************************
 * Function Name: my_tau0_channel1_interrupt
 * Description  : This function is INTTM01 interrupt service routine.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void my_tau0_channel0_interrupt(void)
{

	static uint8_t ifuel=0;
	uint32_t IC7_1000_Test_speedometer_speed;

	//Counter_25mS++;
	Counter_1S++;
	cntr_100us++;
	Counter_600uS++;
	Counter_10ms++;
	Counter_15mS++;
	Counter_25mS++;         /* Increment 25mS Counter */
	Counter_100ms++;
	Counter_100milis++;
	Counter_500ms++;
	Counter_250ms++;
	Counter_IC1_100++;
	Counter_1000ms++;
	Counter_1ms++;
	Counter_IC_CAN_tx++;	//for 95 ms period
	/*
	if(Counter_25mS >= COUNT_25MS)
	{
		Counter_25mS  = CLEAR;
		Read_ADC_Flag = SET;
		//Battery_Read();
	}

	if(Counter_200mS >= COUNT_200MS)
	{
		Counter_200mS   = CLEAR;
		//Read_ADC_Flag = SET;
		Batt_read       = SET;
	}

	if(Counter_1S > COUNT_1SEC)
	{
		Counter_1S          = CLEAR;
		CAN_TX_ONE_SEC_FLAG = SET;
	}

	if(++CAN_transmit_timeout >= CAN_TX_TIMEOUT)
	{
		CAN_transmit_timeout = 10U;
	}
	 */
	if(Counter_IC_CAN_tx >= (877))//939,853
	{
		IC10_200_periodicity ++;
		if((disable_can_tx_flag_req==CLEAR)&&(disable_can_tx_flag==CLEAR)&&(IGN_ON_flag == 1))
		{
			if(TX_ODO_Flag==1)
			{
				my_can0_tx(  8, CAN_IC2_100  );   // Transmit IC2_100 message
			}
			my_can0x211_tx(8 , CAN_IC1_100 ); // Transmit IC1_100 message
			my_can0_tx_IC_NSM(IC_NSM,8,CAN_IC_NSM); // Transmit IC_NSM message
			if(IC10_200_periodicity >= 2)
			{
				delay_time(1);						//1ms delay before next message tx.
				my_can0_tx_IC10_200(IC10_200,8,CAN_IC10_200);// Transmit IC10_200 message
				IC10_200_periodicity = 0;
			}
		}
		Counter_IC_CAN_tx = 0;
	}

	if(AC_Trip_1min_ON_flag == 1)
	{
		if((AC_ON == 1)&&(speed == 0))
		{
			Counter_1min++;
		}
		else if((AC_ON == 0)&&(speed == 0))
		{
			Counter_1min = Counter_1min;//don't increment counter
		}

	}
	else if( AC_Trip_1min_ON_flag == 0)
	{
		Counter_1min = 0;
	}
	if(Counter_600uS >= COUNT_600uS)		/* Check whether 600uS Time elasped or not */
	{                                   /*Stepper Motor flag*/
		Counter_600uS = CLEAR;
	}
	if(Counter_15mS >= COUNT_15mS)
	{
		Counter_15mS= CLEAR;
		Update_FStepper_Flag = SET;
	}

	if(Counter_25mS >= COUNT_25MS)
	{
		Counter_25mS  = CLEAR;
		Read_ADC_Flag = SET;       /* SET ADC flag*/
	}


	if((Counter_100ms >= COUNT_100MS)&&(logo_completed==1))
	{
#ifndef U350
		RPAS_Screen_Activation_W501();/*functioncall */
#endif
		Counter_100ms = CLEAR;
		Read_TT_flag  = SET;
	}
	/******************************/
	if(Counter_1ms >= COUNT_1MS)
	{
		U32_DTE_Step_Cntr = U32_DTE_Step_Cntr + 2U;
		Counter_1ms = 0;
		can_bus_off_failure_detection();

	}
	if(Counter_10ms >= 100)
	{
		Counter_10ms=CLEAR;
		//IMMOBIIZER_TT_1=~IMMOBIIZER_TT_1;
		if((U8g_Block_All_ECU_CAN_MSG == CLEAR) && (CAN_init_done == SET) && (dtc_log_lock_flag == CLEAR) && 
				(disable_can_rx_flag_req  == CLEAR))
		{
			if(Network_Message_Monitor_Start == SET)
			{
				my_can0_rx();//for test
			}
			CAN_message_Timeout(); /* message timeout function calls at every 100usec */
		}
	}
	/******************************/

	if(Counter_500ms >= COUNT_500MS)
	{
		Counter_500ms=CLEAR;
		Counter_500ms_popup=1;
		if(IGN_ON_flag == 1)
		{
			two_sec_count++;
			if(two_sec_count >= 4)
			{
				two_sec_count = 4;
			}
			if((two_sec_count == 4)&&(SYS_Fun_Flag == 0)&&(Limp_home_mode == 0))
			{
				if((U8g_Block_All_ECU_CAN_MSG == CLEAR) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR)
						&& (CAN_Bus_Off == CLEAR) && (EMS3_10_message_timeout_flag == CLEAR))
				{
					//VIN_Mismatch(&CAN_EMS5_500);
					if((LVSD_CAN==0)&&(HVSD_CAN==0))
					{
						VIN_flg_check();
					}
				}
			}
		}
		else
		{
			two_sec_count = 0;
		}
		if((PROVEOUT_OK==1)&&(IO_Parameter_Request_Status!=3)&&(LVSD_flag ==0)&&(HVSD_flag == 0))
		{
			Illumination_steps();
		}
	}

	if(Counter_1000ms >= COUNT_1SEC)
	{
		Counter_1000ms=CLEAR;
		Counter_1000ms_flag=1;
	}
	if(Counter_1S > COUNT_1SEC)
	{
		Counter_1S   = CLEAR;
		//		button_input = SET;
	}
	/* start of UDS timing counter */
	if(REQUEST_3SEC >= 1)
	{
		Counter_3Sec++;
		if(Counter_3Sec>(30000*0.71))    /* 3 sec */
		{
			Counter_3Sec=30010;
			REQUEST_3SEC =0;
		}
	}
	else
	{
		Counter_3Sec =0;
	}

	if(REQUEST_5SEC >= 1)
	{
		Counter_5Sec++;
		if(Counter_5Sec>(50000*0.71))      /* 5 sec */
		{
			Counter_5Sec=50010;
			REQUEST_5SEC =0;
		}
	}
	else
	{
		Counter_5Sec =0;
	}

	if(cntr_100us >= COUNT_100uS)
	{

		cntr_100us = CLEAR;
		UDS_TASK = SET;
		Process_UDSData();
		// memset(UDS_Buffer, 0, sizeof(UDS_Buffer));
	}

	if(P2CAN_ECU_Cntr > 0)
	{
		P2CAN_ECU_Cntr++;
	}

	if(P2CAN_ECU_Cntr >= (P2CAN_ECU_TIMEOUT*0.75))//r
	{
		P2CAN_ECU_Cntr = CLEAR;
	}


	if(S3ECU_cntr > 0)
	{
		S3ECU_cntr++;
	}

	if(S3ECU_cntr >= (S3ECU_TIMEOUT*0.75))//r
	{
		S3ECU_cntr = CLEAR;
		Clear_UDS_variables();
	}

	if(NBs_cntr > 0)
	{
		NBs_cntr++;
	}

	if(NBs_cntr >= (NBs_TIMEOUT*0.75))//r
	{
		NBs_cntr   = CLEAR;
		S3ECU_cntr = SET;
	}

	if(STmin_cntr > 0)
	{
		STmin_cntr++;
	}

	if((STmin_cntr) && (STmin_cntr >= ((STminval*0.71)/ 100)))
	{
		STmin_cntr = CLEAR;
		Respond_Multiple_Frames(0xFF);
	}
	/* end of uds timing counter */
	if(Counter_IC1_100 == 1000 )
	{
		Counter_IC1_100 = CLEAR ;
		if(Read_odo_flag == 1)
		{
			Send_odometer_data = final_odomter_distance/1000;
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//Unit selection mile
			{
				Send_odometer_data = Send_odometer_data* 0.621371;
			}
			CAN_IC2_100[2].byte = Send_odometer_data>>16;
			CAN_IC2_100[1].byte = Send_odometer_data>>8;
			CAN_IC2_100[0].byte = Send_odometer_data;
			TX_ODO_Flag = 1;
		}
		//		CAN_IC2_100[5].bits.Rx_4 = PAB_selected;		/*PAB Status*/
		if(FEATURE_CODE_PAB_SETTING == 1)
		{
			CAN_IC2_100[5].bits.Rx_4 = PAB_selected;		/*PAB Status*/
		}
		else
		{
			PAB_selected = 0;
			CAN_IC2_100[5].bits.Rx_4 = PAB_selected;		/*PAB Status*/
		}
		if(animation_start == 0)
		{
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				CAN_IC10_200[0].byte = (display_speedometer_speed*0.621371);
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				CAN_IC10_200[0].byte = display_speedometer_speed;
			}

		}
		else//to send 0 on IC tx while Guage proveout
		{
			CAN_IC10_200[0].byte = 0;
		}
		//		Fuel_ml add can msg here
		if(fuel_error == 1)
		{
			CAN_IC1_100[7].byte = 0xFF;
			CAN_IC1_100[6].byte = 0xFF;
		}
		else
		{
			CAN_IC1_100[7].byte = FUEL_LEVEL_FOR_DTE>>8;//Fuel_ml_DTE>>8;//Fuel_ml
			CAN_IC1_100[6].byte = FUEL_LEVEL_FOR_DTE;//Fuel_ml_DTE;//Fuel_ml
		}
		/*INDC_high_speed*/
		CAN_IC1_100[5].bits.Rx_8 =	Over_speed_flag;
/*
		//Test For fastfill flag
		CAN_IC1_100[5].bits.Rx_6 =	(Fast_fill_Started|fast_drain|DTE_Update);//test only DTE Related issue
*/
		/*TPMS_ENTER_PR*/

		on_counter++;

		if(on_counter<70)
		{
			if(((SWSC_ADC_value >= ADC_COUNT_UP_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_UP_KEY_OFF)))
			{
				if(key_press_counter_tpms)
					key_press_counter_tpms++;
				if(key_press_counter_tpms>100)
					key_press_counter_tpms =100;
			}
			else if(GI_CONTROL_FLAG==1)
			{



				if(final_vechile_speed_gauge_raw < 0x1C0 && key_press_counter_tpms<=50 && key_press_counter_tpms>=10)
				{
					TPMS_ENTER_PR = 1;
					key_press_counter_tpms =100;
				}

			}

		}
		else
		{
			on_counter=80;
		}

		if(FEATURE_CODE_TPMS_TT_CONFIG == 1)
		{
			if(TPMS_ENTER_CONT <= 52000)//Count for >5 sec
			{
				CAN_IC1_100[1].bits.Rx_3 =	TPMS_ENTER_PR;
			}
			else
			{
				TPMS_ENTER_PR = 0;
				CAN_IC1_100[1].bits.Rx_3 =	0;
			}
		}
		else
		{
			CAN_IC1_100[1].bits.Rx_3 =	0;
		}

		if(odo_error_flag == 1)
		{
			CAN_IC1_100[3].bits.Rx_2 = 0;
			CAN_IC1_100[3].bits.Rx_3 = 0>>1;
			CAN_IC1_100[3].bits.Rx_4 = 0>>2;
		}
		else
		{
			if((service_reminder < 8)&&(NVM_SERVICE_DONE_flag == 0))//
			{
				CAN_IC1_100[3].bits.Rx_2 = service_reminder;
				CAN_IC1_100[3].bits.Rx_3 = service_reminder>>1;
				CAN_IC1_100[3].bits.Rx_4 = service_reminder>>2;
			}
			else if(NVM_SERVICE_DONE_flag == 1)
			{
				CAN_IC1_100[3].bits.Rx_2 = 0;
				CAN_IC1_100[3].bits.Rx_3 = 0>>1;
				CAN_IC1_100[3].bits.Rx_4 = 0>>2;
			}
		}
		if(IGN_ON_flag == SET)
		{
			if((resetackTx == 1) || (B_AFEResetACK==1))
			{
				resetackTx = 0;
				CAN_IC1_100[1].bits.Rx_6 = 1;
				CAN_IC1_100[1].bits.Rx_7 = 0;
			}
			else
			{
				CAN_IC1_100[1].bits.Rx_6 = B_AFEResetACK;
				CAN_IC1_100[1].bits.Rx_7 = 0;
			}
			/*
			if((disable_can_tx_flag_req==CLEAR)&&(disable_can_tx_flag==CLEAR))
			{
				my_can0x211_tx(8 , CAN_IC1_100 ); // Transmit IC1_100 message
				my_can0_tx(  8, CAN_IC2_100  );   // Transmit IC2_100 message
			}
			 */
			cntr_IC7_1000_TEST++;
			if(cntr_IC7_1000_TEST >= 10)
			{
				if((Msg_enable_IC4_1000_Test == 1)&&(disable_can_tx_flag_req==CLEAR)&&(disable_can_tx_flag==CLEAR))
				{
					my_can0_tx_IC4TEST(IC4_1000_TEST,8,CAN_IC4_1000_TEST ); /* Transmit IC4_1000_TEST message*/
				}
				CAN_IC7_1000_TEST[4].bits.Rx_4 = FLOAT_SENSOR_RESISTANCE_RAW;//FLOAT_RESISTANCE_RAW;
				CAN_IC7_1000_TEST[4].bits.Rx_5 = FLOAT_SENSOR_RESISTANCE_RAW>>1;//FLOAT_RESISTANCE_RAW>>1;
				CAN_IC7_1000_TEST[4].bits.Rx_6 = FLOAT_SENSOR_RESISTANCE_RAW>>2;//FLOAT_RESISTANCE_RAW>>2;
				CAN_IC7_1000_TEST[4].bits.Rx_7 = FLOAT_SENSOR_RESISTANCE_RAW>>3;//FLOAT_RESISTANCE_RAW>>3;
				CAN_IC7_1000_TEST[4].bits.Rx_8 = FLOAT_SENSOR_RESISTANCE_RAW>>4;//FLOAT_RESISTANCE_RAW>>4;
				CAN_IC7_1000_TEST[5].bits.Rx_1 = FLOAT_SENSOR_RESISTANCE_RAW>>5;//FLOAT_RESISTANCE_RAW>>5;
				CAN_IC7_1000_TEST[5].bits.Rx_2 = FLOAT_SENSOR_RESISTANCE_RAW>>6;//FLOAT_RESISTANCE_RAW>>6;
				CAN_IC7_1000_TEST[5].bits.Rx_3 = FLOAT_SENSOR_RESISTANCE_RAW>>7;//FLOAT_RESISTANCE_RAW>>7;

				/*VEHICLE_SPEED_IC*/
				IC7_1000_Test_speedometer_speed = display_speedometer_speed*64;
				CAN_IC7_1000_TEST[0].byte	=	IC7_1000_Test_speedometer_speed;//final_vechile_speed_gauge_raw;
				CAN_IC7_1000_TEST[1].byte	=	IC7_1000_Test_speedometer_speed>>8;//final_vechile_speed_gauge_raw>>8;

				cntr_IC7_1000_TEST=0;
				if((Msg_enable_IC7_1000_Test == 1)&&(disable_can_tx_flag_req==CLEAR)&&(disable_can_tx_flag==CLEAR))
				{
					my_can0_tx_ICTEST(IC7_1000_TEST,8,CAN_IC7_1000_TEST ); /* Transmit IC7_1000_TEST message*/
				}
				//	my_can0_tx_ICTEST(IC6_1000_TEST,8,CAN_IC6_1000_TEST ); /* Transmit IC6_1000_TEST message*/

				/*CAN_IC6_1000_TEST for FLOAT_RESISTANCE_RAW and FLOAT_RESISTANCE_FILTER*/
				/**/
				/*
				CAN_IC6_1000_TEST[1].byte = EEPROM_LOG;//for EEPROM read test
				CAN_IC6_1000_TEST[0].byte = EEPROM_LOG;//for EEPROM read test
				 */
				uint32_t idle_FC = 0;
				idle_FC = (rtY.U32_Idle_DTE_FC_Acc);
				if(fuel_error == 1)
				{
					CAN_IC6_1000_TEST[0].byte =0xFF; //New_fuel_read_avg;//lower byte
					CAN_IC6_1000_TEST[1].byte =0xFF; //New_fuel_read_avg;//Higher byte
				}
				else
				{
					CAN_IC6_1000_TEST[0].byte =FUEL_LEVEL_RAW_ADC; //New_fuel_read_avg;//lower byte
					CAN_IC6_1000_TEST[1].byte =FUEL_LEVEL_RAW_ADC>>8; //New_fuel_read_avg;//Higher byte
				}
				/*Current fuel level Calculated by Formula*/
				/*********************************FOR_MAHINDRA_PVT**************************/
				AVG_50KM_AFE = (uint32_t)(DTE_RAFE_Avg_Val*10);
				CAN_IC6_1000_TEST[2].byte = AVG_50KM_AFE;//New_fuel_read_ad1;//lower byte
				CAN_IC6_1000_TEST[3].byte = AVG_50KM_AFE>> 8;//New_fuel_read_ad1;//Higher byte
/*
				if(fuel_error == 1)
				{
					CAN_IC6_1000_TEST[2].byte = 0xFF;//New_fuel_read_ad1;//lower byte
					CAN_IC6_1000_TEST[3].byte = 0xFF;//New_fuel_read_ad1;//Higher byte
				}
				else
				{
					CAN_IC6_1000_TEST[2].byte = FUEL_LEVEL_FOR_DTE;//New_fuel_read_ad1;//lower byte
					CAN_IC6_1000_TEST[3].byte = FUEL_LEVEL_FOR_DTE>> 8;//New_fuel_read_ad1;//Higher byte
				}
*/
				CAN_IC6_1000_TEST[4].bits.Rx_1 = FLOAT_RESISTANCE_RAW;
				CAN_IC6_1000_TEST[4].bits.Rx_2 = FLOAT_RESISTANCE_RAW>>1;
				CAN_IC6_1000_TEST[4].bits.Rx_3 = FLOAT_RESISTANCE_RAW>>2;
				CAN_IC6_1000_TEST[4].bits.Rx_4 = FLOAT_RESISTANCE_RAW>>3;
				CAN_IC6_1000_TEST[4].bits.Rx_5 = FLOAT_RESISTANCE_RAW>>4;
				CAN_IC6_1000_TEST[4].bits.Rx_6 = FLOAT_RESISTANCE_RAW>>5;
				CAN_IC6_1000_TEST[4].bits.Rx_7 = FLOAT_RESISTANCE_RAW>>6;
				CAN_IC6_1000_TEST[4].bits.Rx_8 = FLOAT_RESISTANCE_RAW>>7;
				CAN_IC6_1000_TEST[5].bits.Rx_1 = FLOAT_RESISTANCE_RAW>>8;
				CAN_IC6_1000_TEST[5].bits.Rx_2 = FLOAT_RESISTANCE_RAW>>9;


				CAN_IC6_1000_TEST[5].bits.Rx_3 =FLOAT_RESISTANCE_FILTER;
				CAN_IC6_1000_TEST[5].bits.Rx_4 = FLOAT_RESISTANCE_FILTER>>1;
				CAN_IC6_1000_TEST[5].bits.Rx_5 = FLOAT_RESISTANCE_FILTER>>2;
				CAN_IC6_1000_TEST[5].bits.Rx_6 = FLOAT_RESISTANCE_FILTER>>3;
				CAN_IC6_1000_TEST[5].bits.Rx_7 = FLOAT_RESISTANCE_FILTER>>4;
				CAN_IC6_1000_TEST[5].bits.Rx_8 = FLOAT_RESISTANCE_FILTER>>5;
				CAN_IC6_1000_TEST[6].bits.Rx_1 = FLOAT_RESISTANCE_FILTER>>6;
				CAN_IC6_1000_TEST[6].bits.Rx_2 = FLOAT_RESISTANCE_FILTER>>7;
				CAN_IC6_1000_TEST[6].bits.Rx_3 = FLOAT_RESISTANCE_FILTER>>8;
				CAN_IC6_1000_TEST[6].bits.Rx_4 = FLOAT_RESISTANCE_FILTER>>9;

				/**/
				if((Msg_enable_IC6_1000_Test == 1)&&(disable_can_tx_flag_req==CLEAR)&&(disable_can_tx_flag==CLEAR))
				{
					my_can0_tx_IC6TEST(IC6_1000_TEST,8,CAN_IC6_1000_TEST ); /* Transmit IC6_1000_TEST message*/
				}
			}

		}

	}



	if((first_screen_timeout>0)/*&&(first_screen_timeout_start==1)*/)
	{
		first_screen_timeout++;
	}
#if 0
	if((first_screen_timeout>=first_screen_timeout_MAX) && (Proveout_Count > PROVEOUT_DONE))
	{
		first_screen_timeout=0;
		(void)drome_sendEventToDrome((U32)drome_InEventMain_screen_timeout);/*send timeout to drome to print next screen after logo*/
	}
#endif
	//ETM
	if(ETM_test_start_timeout>0)
	{
		ETM_test_start_timeout++;
	}
	if(ETM_test_start_timeout>=ETM_test_start_timeout_max)
	{
		ETM_test_start_timeout = 1;
		ETM_TFT_test_index++;
		if(ETM_TFT_test_index > 5)
		{
			ETM_TFT_test_index = 0;
		}
	}

	if(Odo_reset_8sec_on == 1)
	{
		Odo_reset_8sec_count++;
		if(Odo_reset_8sec_count<=60000)//60000 for 8sec for W501 ODO reset
		{
			if(Odo_rst_count == 3)
			{
				reset_odometer();
				Odo_rst_count = 0;
			}
		}
		else
		{
			Odo_rst_count = 0;
			Odo_reset_8sec_on = 0;
		}
	}


	if(start_alart_timeout_5sec>0)
	{
		start_alart_timeout_5sec++;
	}
	if(start_alart_timeout_5sec>=start_alart_timeout_5sec_MAX)
	{
		start_alart_timeout_5sec=0;
		dont_throw_alerts_alert_on_screen_present=0;
		if(skip_flag == 1)
		{
			skip_flag = 0;
		}
		else
		{
//			dont_throw_alerts_alert_on_screen_present=0;
		}

		if(DEF_priority_flag == 1)
		{
			DEF_priority_flag = 0;
		}
		if(REGEN_priority_flag == 1)
		{
			REGEN_priority_flag = 0;
		}
		if((REGEN_Required_popup_skip != 1)&&(REGEN_Required_popup_shown == 1)) /*regen not skip by switch then only IGN_ON_regen_init = 0 */
		{
			IGN_ON_regen_init = 0;
			init_regen_required = 0;
		}
		else if((REGEN_aboarted_popup_skip != 1)&&(REGEN_aboarted_popup_shown == 1))
		{
			init_regen_aboarted = 0;

		}
		else if((REGEN_notstarted_popup_skip != 1)&&(REGEN_notstarted_popup_shown == 1))
		{
			init_regen_not_started = 0;
			wait_10sec = 0;
		}
		else if((REGEN_started_popup_skip != 1)&&(REGEN_started_popup_shown == 1))
		{
			init_regen_started = 0;
		}
		else if((REGEN_completed_popup_skip != 1)&&(REGEN_completed_popup_shown == 1))
		{
			init_regen_completed = 0;
		}



		Priority_flag = 0;
		if(Dooropen_popup_shown == 1)
		{
			Dooropen_popup_shown = 0;
			Dooropen_detected = 0;
			Codriver_index = 4;
			Driver_index = 4;
			Rearleft_index = 4;
			RearRight_index = 4;
			Backd_index = 4;
			Hood_index = 1;
		}
		if(Dooropen_IGNOFF_popup_shown == 1)
		{
			Dooropen_IGNOFF_popup_shown = 0;
			IGNOFF_Dooropen_detected = 0;
			Codriver_index = 4;
			Driver_index = 4;
			Rearleft_index = 4;
			RearRight_index = 4;
			Backd_index = 4;
			Hood_index = 1;
		}
		//		if(dont_throw_alerts_BS6_alert_on_screen_present == 0)
		display_key = drome_getCurrentViewKey();
		if((regen_page_skip == 1)&&((display_key == 0x11015)||(display_key == 0x11032)))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
			//			(void)drome_sendEventToDrome((U32)drome_KB_DOWN);
		}
		else
		{
			//		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
			IGNOFF_display_timeout = 1;
			if((skip_alert_Enable == 1)||(skip_alert_Enable == 3))	//if alert timeout after 5 sec then only chnage skip_alert_Enable to 0
			{
				skip_alert_Enable = 0;
			}
		}

		if(Dooropen_active_IGNOFF_Alert == 1)
		{
			IGNOFF_Dooropen_check_done = 1;
			Dooropen_active_IGNOFF_Alert = 0;
		}
		/*else
		{
			IGNOFF_Dooropen_check_done = 0;
		}*/
		if(Checklamp_active_IGNOFF_Alert == 1)
		{
			IGNOFF_ExtLamp_check_done = 1;
			Checklamp_active_IGNOFF_Alert = 0;
		}

		if(Keyinreminder_IGNOFF_Alert == 1)
		{
			IGNOFF_keyinremind_check_done = 1;
			Keyinreminder_IGNOFF_Alert = 0;
		}
		/*else
		{
			IGNOFF_ExtLamp_check_done = 0;
		}*/
		//		}
		if((IGNOFF_ExtLamp_check_done == 1)&&(IGNOFF_Dooropen_check_done == 1)&&(IGNOFF_keyinremind_check_done == 1)&&(wait_state == 0)&&(startkey == 0x11108))
		{
			display_active = 0;
			dont_throw_alerts_alert_on_screen_present=0;
		}

	}
	/***********************************************************************************************/
	if(start_driveSum_timeout_3sec>0)
	{
		start_driveSum_timeout_3sec++;
	}
	if(start_driveSum_timeout_3sec>=10000)
	{
		write_eeprom_flag = 1;
		if(start_driveSum_timeout_3sec>=start_driveSum_timeout_3sec_MAX)
		{
			start_driveSum_timeout_3sec = 0;
			dont_throw_alerts_alert_on_screen_present=0;

			(void)drome_sendEventToDrome((U32)drome_InEvent_Drive_Sum_Timeout);
			Drive_summery_tout_flag1 = 1;
		}
	}
	/************************** Drive summary timeout ***********************************************/
	/******************************************************************************/
	if(dromecall_timeout >= 1)	/* 1 Sec*/
	{
		dromecall_timeout++;
	}
	/***************************** Drome call timeout ***********************************************/

	/************************************************************************************************/
	/****************************** Door close timeout *********************************************/
	if(All_doorclose_timeout_start == 1)		/*Door close 1 sec timeout condition added*/
	{
		start_alert_timeout_1sec++;
	}
	if((start_alert_timeout_1sec>=start_alart_timeout_1sec_MAX)&&(All_doorclose == 1)&&((Doorclose_popup_shown == 1)||(Doorclose_IGNOFF_popup_shown == 1)))	/*Door close 1 sec timeout*/
	{
		Dooropen_popup_shown = 0;
		Dooropen_IGNOFF_popup_shown = 0;
		IGNOFF_Dooropen_detected = 0;
		Dooropen_detected = 0;
		Codriver_index = 4;
		Driver_index = 4;
		Rearleft_index = 4;
		RearRight_index = 4;
		Backd_index = 4;
		Hood_index = 1;
		Priority_flag = 0;
		prev_STS_Door_data = 0;
		IGNOFFprev_STS_Door_data = 0;
		STS_Door_data = 0;
		start_alert_timeout_1sec=0;
		//		All_doorclose_timeout_start = 0;
		//		IGNOFF_Dooropen_check_done = 1;
		dont_throw_alerts_alert_on_screen_present=0;
		dont_throw_doorclose_on_screen_present = 0;
		DOOR_OPEN_IGN_OFF=0;
		(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
		IGNOFF_display_timeout = 1;
		//		Dooropen_active_IGNOFF_Alert = 0;
		if(Dooropen_active_IGNOFF_Alert == 1)
		{
			IGNOFF_Dooropen_check_done = 1;
			Dooropen_active_IGNOFF_Alert = 0;
			All_doorclose_timeout_start = 0;
		}
		else
		{
			All_doorclose_timeout_start = 0;
		}
		/*else
		{
			IGNOFF_Dooropen_check_done = 0;
		}*/
		/*else
		{
			IGNOFF_ExtLamp_check_done = 0;
		}*/
		//		}
		if((IGNOFF_ExtLamp_check_done == 1)&&(IGNOFF_Dooropen_check_done == 1)&&(IGNOFF_keyinremind_check_done == 1)&&(startkey == 0x11108))
		{
			display_active = 0;
		}
	}
	/*********************************After skip by switch BS6 Timeout*********************************************/
	if(BS6_permanent_skip == 1)
	{
		BS6_alert_timeout_4sec++;
	}
	if(BS6_alert_timeout_4sec>=start_alart_timeout_4sec_MAX)
	{
		if(BS6_permanent_skip == 1)
		{
			BS6_permanent_skip = 0;
		}
		BS6_permanent_timeout = 1;
		BS6_alert_timeout_4sec = 0;
	}
	/*****************************************************************************************************/
	if(regen_req_page_timeout>0)
	{
		regen_req_page_timeout++;
//		CAN_IC2_100[2].bits.Rx_2=1;

#if 0
		if(regen_req_page_timeout==10000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==20000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==30000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==40000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==50000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==60000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==70000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==80000)
		{
			my_can0_tx();
		}
		else if(regen_req_page_timeout==90000)
		{
			my_can0_tx();
		}
#endif
	}
	if(regen_req_page_timeout==regen_req_page_timeout_MAX)
	{
//		CAN_IC2_100[2].bits.Rx_2=0;
		regen_req_page_timeout=0;
		regen_req_page_timeout_reached=1;
		//
	}
	/****************************** Setting Menu timeout *********************************************/
	if(Setting_timeout_start == 1)		/*Setting Menu 5 sec timeout condition added*/
	{
		Settingmenu_timeout_10sec++;
	}
	if(((Settingmenu_timeout_10sec>=start_setting_timeout_10sec_MAX)&&(Setting_timeout_start == 1))||(PAB_jump_driveinfo == 1))
	{
		key = drome_getCurrentViewKey();
		if((key == 0x22037) ||(key == 0x22038))
		{

			(void)drome_sendEventToDrome((U32)drome_InEvent_Setting_Timeout);
		}
		(void)drome_sendEventToDrome((U32)drome_InEvent_Setting_Timeout);
		Settingmenu_timeout_10sec=0;
		Setting_timeout_start = 0;
		PAB_jump_driveinfo = 0;
	}
	/*********************************After skip by switch BS6 Timeout*********************************************/



	/******************* used by Viresh ******************/
	if(Counter_100milis >= 1000)
	{
		Counter_100milis = 0;
		Counter_3_5usec  = 1;
	}

	/******************************************************/


#if 0
	if(CounterTT_250ms >= COUNT_250MS)
	{

		//				telltale_blink_flag_patt7 =0x000;
		//				telltale_blink_flag_patt7 =0xfff;
		telltale_blink_flag_patt7 = ~telltale_blink_flag_patt7;
		CounterTT_250ms   = CLEAR;
	}
#endif

	if(eng_start_timer_on == 1)
	{
		//EBD_alert_Counter_5sec++;
		eng_start_Counter_5sec++;
#ifdef COLOR_TFT
		if(eng_start_Counter_5sec >= 50000)
#else
			if(eng_start_Counter_5sec >= 30000)//50000 *Note: Consider Engine running state from Ignition ON
#endif
			{
				//			EBD_alert_flag = 1;
				eng_start_flag = 1;
				Enginerun_chm = 1;
			}

	}
	else
	{
		//EBD_alert_flag = 0;
		Enginerun_chm = 0;
		eng_start_flag = 0;
		//EBD_alert_Counter_5sec = 0;
		eng_start_Counter_5sec = 0;
	}

	if((Timer_ON_3sec == 1))
	{
		Counter_3sec++;
		if((Counter_3sec >= 30000))
		{
			Counter_3sec_Flag = 1;
		}
		else
		{
			Counter_3sec_Flag = 0;
		}
	}
	else
	{
		Counter_3sec = 0;
		Counter_3sec_Flag = 0;
	}

	if(TPMS_ENTER_PR == 1)
	{
		TPMS_ENTER_CONT++;
	}
	else
	{
		TPMS_ENTER_CONT = 0;
	}

#if 0
	if(write_seed > 0)
	{
		write_seed++;
	}

	if(write_seed == 4)
	{
		write_seed = CLEAR;
		//	eeprom_write(SEED_VALUE_OFFSET, Seed.Bytes_in_seed[0]);
		//	eeprom_write(SEED_VALUE_OFFSET + 1, Seed.Bytes_in_seed[1]);
		//	eeprom_write(SEED_VALUE_OFFSET + 2, Seed.Bytes_in_seed[2]);
		//	eeprom_write(SEED_VALUE_OFFSET + 3, Seed.Bytes_in_seed[3]);
	}
#endif
	/* ***************** Below lines added by VireshW for Fuel ****************** */
#if 1

	if((Timer_ON_30sec == 1))
	{
		Counter_30sec++;
		if((Counter_30sec >= 300000))
		{
			Counter_30sec = 0;
			Timer_ON_30sec = 0;
			Counter_30sec_Flag = 1;
		}
	}
	else
	{
		Counter_30sec = 0;
	}

	if((Timer_recovery_ON_10sec == 1))
	{
		Recovery_Counter_10sec++;
		if((Recovery_Counter_10sec >= 100000))
		{
			Recovery_Counter_10sec = 0;
			Timer_recovery_ON_10sec = 0;
			Recovery_Counter_10sec_Flag = 1;
		}
	}
	else
	{
		Recovery_Counter_10sec = 0;
	}


	if((Timer_ON_10sec == 1))
	{
		Counter_10sec++;
		if(Counter_10sec >= 67000)//56666 Changed to 10(85000) sec as per new requirement r15 before 150000	140000
		{
			Counter_10sec = 0;
			Timer_ON_10sec = 0;
			Counter_10sec_Flag = 1;
		}
	}
	else
	{
		Counter_10sec = 0;
	}


	if((Timer_ON_13sec == 1))
	{
		Counter_13sec++;
		if(Counter_13sec >= 75000)// before 130000 (110000)
		{
			Counter_13sec = 0;
			Timer_ON_13sec = 0;
			Counter_13sec_Flag = 1;
		}
	}
	else
	{
		Counter_13sec = 0;
	}

	/**/
	if((Timer_ON_13sec_ign == 1))//for IGN on to off timer
	{
		Counter_13sec_ign++;
		ADC_Avg();
		if((Counter_13sec_ign >= 100000))/*13+3 sec - 160000*/
		{
			copy_ADC = 0;	//calculate bar on (RAW ADC count ie. avg of 10 samples )
			Counter_13sec_ign = 0;
			Timer_ON_13sec_ign = 0;//r To avoid multiple time write Fuel Present_level in memory
			Counter_13sec_ign_Flag = 1;
			Timer_ON_300msec = 1;
			//			for(uint8_t i=0;i<11;i++)
			//			{
			//				ADC_Avg();
			//			}
			New_fuel_read_ad1 = New_fuel_read_avg;
			Present_level = Current_Fuel_Bar(); /*  New_fuel_read_ad1; */
			eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level); /* store Present_level in memory */
			//if(Counter_300msec_Flag == 1)		to avoid 13 sec time

			eeprom_write(IGN_TOGGLE_FLAG, 0xAA); /*To write 0xAA only after Writing of settled fuel level*/
			go_to_sleep	= 1;			//allow cluster to go in deep sleep

		}
	}
	else
	{
		Counter_13sec_ign = 0;
	}


	if((Timer_ON_5sec == 1))
	{
		Counter_05sec++;
		if((Counter_05sec >= 35000))//(33500)33333 changed to 5 (50000)sec as per new changes. previously = 280000 r 200000	156250 before 300000
		{
			Counter_05sec = 0;
			Timer_ON_5sec = 0;
			Counter_5sec_Flag = 1;
		}
	}
	else
	{
		Counter_05sec = 0;
	}


	if((Timer_fastfill_5sec == 1))//to decide entry in fast fill
	{
		FF_Counter_05sec++;
		ADC_Avg();// for test
		if((FF_Counter_05sec >= 35000))//(33500)33333 changed to 5 (50000)sec as per new changes. previously = 280000 r 200000	156250 before 300000
		{
			FF_Counter_05sec = 0;
			Timer_fastfill_5sec = 0;
			Counter_FF_5sec_Flag = 1;
		}
	}
	else
	{
		FF_Counter_05sec = 0;
	}
	/**/

	if((Timer_ON_300msec == 1))
	{
		Counter_300msec++;
		if((Counter_300msec >= 300))
		{
			Counter_300msec_Flag = 1;
			if(ifuel<10)
			{
				ADC_Array1[ifuel] = Fuel_ADC_value;
				ADC_Array2[ifuel] = Fuel_EXVrefADC_value;
				ifuel++;
			}
			else
			{
				ifuel=0;
			}
			Counter_300msec = 0;
			//Timer_ON_300msec =0;
			//r			ADC_Read_300msec_Flag = 1;
		}
	}
	else
	{
		Counter_300msec = 0;
	}
	/**/
	Counter_330msec++;
	if((Counter_330msec >= ADC_Update_Speed))
	{

		if(New_fuel_read_ad1 > raw_adc_count)//New_fuel_read_avg
		{
			New_fuel_read_ad1--;
		}
		else if(New_fuel_read_ad1 < raw_adc_count)//New_fuel_read_avg
		{
			New_fuel_read_ad1++;
		}
		Counter_330msec = 0;
		//r			ADC_Read_300msec_Flag = 1;
	}


	/**/
	if((Timer_ON_100msec == 1))
	{
		Counter_100msec++;
		if((Counter_100msec >= 100))/*Update fuel On IGN OFF -> ON(Rajat)*/
		{
			/*Process raw ADC value*/
#ifdef U350		/*RAW Fuel Level Calculation*/
			raw_adc_count = ((4096 * Fuel_ADC_value) / Fuel_EXVrefADC_value);
			if(FEATURE_CODE_FUEL_TANK_SELECT == 1)//60L tank Configuration.
			{
				//FUEL_LEVEL_RAW_ADC = (-(0.0125 * raw_adc_count * raw_adc_count) + (27.688 *raw_adc_count) + 43078);
				FUEL_LEVEL_RAW_ADC = ((-2.637*FLOAT_SENSOR_RESISTANCE_RAW_AVG*FLOAT_SENSOR_RESISTANCE_RAW_AVG)-(54.155*FLOAT_SENSOR_RESISTANCE_RAW_AVG)+61586);//Resistance based formula
			}
			else if(FEATURE_CODE_FUEL_TANK_SELECT == 0)//50L tank Configuration.
			{
				if(New_fuel_read_avg>=3000)//0L to 12L		//New_fuel_read_ad1
				{
					FUEL_LEVEL_RAW_ADC = (-(0.0946*raw_adc_count*raw_adc_count)+(516.72*raw_adc_count)-687084);
				}
				else if((New_fuel_read_avg<3000)&&(New_fuel_read_avg>=2000))//13L to 36L		//New_fuel_read_ad1
				{
					FUEL_LEVEL_RAW_ADC = (-(0.0046*raw_adc_count*raw_adc_count)+(0.9866*raw_adc_count)+52165);
				}
				else if((New_fuel_read_avg<2000)&&(New_fuel_read_avg>=930))//37L-50L		//New_fuel_read_ad1
				{
					FUEL_LEVEL_RAW_ADC = ((0.0027*raw_adc_count*raw_adc_count)-(17.822*raw_adc_count)+62431);
				}
				else if((New_fuel_read_avg<930)&&(New_fuel_read_avg>=630))		//New_fuel_read_ad1
				{
				//	FUEL_LEVEL_FOR_DTE = 50000;							//showing 50L up to 15 ohm
					FUEL_LEVEL_RAW_ADC = 50000;
				}
			}
#else
			raw_adc_count = ((4096 * Fuel_ADC_value) / Fuel_EXVrefADC_value);
//			FUEL_LEVEL_RAW_ADC = (-(0.0004 * raw_adc_count * raw_adc_count) - (13.591 *raw_adc_count) + 50850);
			FUEL_LEVEL_RAW_ADC  =	((-0.0044*raw_adc_count*raw_adc_count)-(5.0343*raw_adc_count)+62636);

#endif

			/***********************/

			if(ifuel<10)
			{
				ADC_Array1[ifuel] = Fuel_ADC_value;
				ADC_Array2[ifuel] = Fuel_EXVrefADC_value;
				ifuel++;
			}
			else
			{
				ifuel=0;
			}
			Counter_100msec = 0;
			Timer_ON_100msec = 0;
			//r			ADC_Read_Flag   = 1;
		}
	}
	else
	{
		Counter_100msec = 0;
	}


	if(Timer_ON_fast_drain == 1)
	{
		Counter_13sec_fd++;
		if(Counter_13sec_fd >= 90000)//130000
		{
			Counter_13sec_fd = 0;
			Timer_ON_fast_drain = 0;
			fast_drain_start = 1;
		}
	}
	else
	{
		Counter_13sec_fd = 0;
	}


	if((Timer_ON_5min == 1))
	{
		Counter_5min++;//2770000,,1650000
		if((Counter_5min == (2000000)))//using 277 sec to cancle out offset time to start timer after resistance change
		{
			Counter_5min   = 0;
			Timer_ON_5min = 0;
			Counter_5min_Flag = 1;
		}
	}
	else
	{
		Counter_5min = 0;
	}
	/**/
	if((Timer_ON_Per_Liter == 1))
	{
		Counter_Per_Liter++;//2770000
		if(Counter_Per_Liter >= Per_Minute_count)//
		{
			Counter_Per_Liter   = 0;
			Timer_ON_Per_Liter = 0;
			Per_Min_Counter_Flag = 1;
		}
	}
	else
	{
		Counter_Per_Liter = 0;
	}
	/**/
	if((Timer_ON_3min == 1))
	{
		Counter_3min++;//2770000
		if((Counter_3min == (1400000)))//
		{
			Counter_3min   = 0;
			Timer_ON_3min = 0;
			Counter_3min_Flag = 1;
		}
	}
	else
	{
		Counter_3min = 0;
	}


	/*******for 10 sec offset in speed***********/

	if((Timer_ON_5min_spd == 1))
	{
		Counter_5min_spd++;
		if(Counter_5min_spd == 2000000)//2900000,,1650000
		{
			Counter_5min_spd   = 0;
			Timer_ON_5min_spd = 0;
			Counter_5min_spd_flag = 1;
		}
	}
	else
	{
		Counter_5min_spd = 0;
	}
	/**********************************************/
	/*ETM Exit Timers*/
	if(ETM_10_sec_Timer == 1)
	{
		ETM_10_sec_counter++;
		if((set_key_short == 1)||(Up_key_Short == 1)||(Down_key_Short == 1))
		{
			ETM_10_sec_counter = 0;
		}
		if(ETM_10_sec_counter == 65000)
		{
			ETM_10_sec_counter = 0;
			ETM_10_sec_Timer = 0;
			ETM_10_sec_exit_flag = 1;
			(void)drome_sendEventToDrome((U32)drome_InEvent_ETM_11sec_timeout);//ETM Exit point
		}
	}
	else
	{
		ETM_10_sec_counter = 0;
	}

	if(ETM_30_sec_Timer == 1)
	{
		ETM_30_sec_counter++;

		if(ETM_30_sec_counter == 210000)
		{
			ETM_30_sec_counter = 0;
			ETM_30_sec_Timer = 0;
			ETM_30_sec_exit_flag = 1;
			(void)drome_sendEventToDrome((U32)drome_InEvent_ETM_30sec_timeout);//ETM Exit point
		}
	}
	else
	{
		ETM_30_sec_counter = 0;
	}
	/**********************************************/

#endif

#ifdef U350
	/*FOR U350 SWSC*/
	if(MODE_KEY_Flag==1)
	{
		Counter_MODE_KEY_100usec++;
		Counter_MODE_KEY_LongPress_100usec++;
		if((Counter_MODE_KEY_LongPress_100usec >= 34000)/*&&(MODE_key_press == 0)*/)//34000 for 5 sec.
		{
			MODE_key_press = 1;
			button_value = 0;		//OK function for Mode long press
			//			MODE_key_ShortPress = 0;
			//			Counter_MODE_KEY_LongPress_100usec=0;
		}
		if(Counter_MODE_KEY_LongPress_100usec >= 398000)//398000 for 1 min count
		{
			Mode_Key_stuck = 1;
		}

	}
	else
	{
		//		if((Counter_MODE_KEY_LongPress_100usec >= 50000)&&(button_value != 0))
		//		{
		//			//			MODE_key_LongPress= 1;
		//			button_value = 0;		//OK function for Mode long press
		//			MODE_key_ShortPress = 0;
		//
		//		}
		Mode_Key_stuck = 0;
		if((Counter_MODE_KEY_100usec >= 1000/*5000*/) && (Counter_MODE_KEY_100usec < 34000) /*&& (MODE_key_press == 0)*/)
		{
			//			MODE_key_ShortPress = 1;
			button_value = 1;		//Menu function for Mode Short press
			Counter_MODE_KEY_100usec=0;
			//			Counter_MODE_KEY_LongPress_100usec=0;
			//			MODE_key_press = 1;
			//			Timer_ON_1sec = 1;

		}
		else if(Counter_MODE_KEY_100usec >= 34000)
		{
			Counter_MODE_KEY_100usec = 0;
		}
		else
		{
			Counter_MODE_KEY_100usec = 0;
			Counter_MODE_KEY_LongPress_100usec = 0;
		}
	}

	if(SET_KEY_Flag==1)
	{
		Counter_SET_KEY_100usec++;
		Counter_SET_KEY_LongPress_100usec++;
		if(Counter_SET_KEY_LongPress_100usec>= 34000)//34000 for 5 sec.
		{
			SET_key_press = 1;
			OK_pressed = 1;
			button_value = 4;		//OK LONG function for SET long press
		}
		if((Counter_SET_KEY_LongPress_100usec>= 34000) && (avoid_trip_reset == 1))
		{
			Counter_SET_KEY_LongPress_100usec = 0;
		}
		if(Counter_SET_KEY_LongPress_100usec >= 398000)//398000 for 1 min count
		{
			Set_Key_stuck = 1;
		}
	}
	else
	{
		Set_Key_stuck = 0;
		if((Counter_SET_KEY_100usec >= 1000/*5000*/) && (Counter_SET_KEY_100usec < 34000))
		{
			button_value = 2;		//DOWN function for SET Short press
			Counter_SET_KEY_100usec=0;
		}
		else if(Counter_SET_KEY_100usec >= 34000)
		{
			Counter_SET_KEY_100usec = 0;
		}
		else
		{
			Counter_SET_KEY_100usec = 0;
			Counter_SET_KEY_LongPress_100usec = 0;
		}
	}

	/*
	if(SET_KEY_Flag==1)
	{
		Counter_SET_KEY_100usec++;
		Counter_SET_KEY_LongPress_100usec++;
		if((Counter_SET_KEY_LongPress_100usec>= 50000)&&(button_value != 4))
		{
			//			SET_key_LongPress = 1;
			button_value = 4;		//OK function for set long press
			SET_key_ShortPress = 0;

		}
//		else
//		{
//			SET_key_LongPress = 0;
//		}

		if((Counter_SET_KEY_100usec <= 1000)&&(button_value != 2))//2000//try button_value != 2
		{
			//			SET_key_ShortPress = 1;
			button_value = 2;		//Down function for set Short press
		}
		else
		{
			Counter_SET_KEY_100usec = 0;
		}
	}
	else
	{
		Counter_SET_KEY_100usec = 0;
		Counter_SET_KEY_LongPress_100usec = 0;
	}
	 */
#else
	if((OK_KEY_Flag == 1))
	{
		Counter_OK_KEY_100usec++;
		Counter_OK_KEY_LongPress_100usec++;
	}
	else
	{
		//		Counter_OK_KEY_100usec = 0;
		Counter_OK_KEY_LongPress_100usec = 0;
	}


	if((UP_KEY_Flag == 1))
	{
		Counter_UP_KEY_100usec++;
		//		Up_key_released = 0;
		//		if((Counter_UP_KEY_100usec >= 700/*2000*/)&&(Counter_UP_KEY_100usec <= 17000)&&(Up_key_released == 1)) /* checking "UP_KEY" is pressed for 500msec or not  */
		//		{
		//			Up_key_released = 0;
		//			if(Up_key_ShortPress == 0)
		//				/* UP_KEY is pressed for 500 msec */
		//			{
		//				button_value = 3;
		//				Up_key_ShortPress = 1;
		//				Counter_UP_KEY_100usec = 0;
		//			}
		//		}

	}
	else
	{
		//		if(Counter_UP_KEY_100usec >= 2000/*5000*/) /* checking "UP_KEY" is pressed for 500msec or not  */
		//		{
		//			/* UP_KEY is pressed for 500 msec */
		//			button_value = 3;
		//			Up_key_ShortPress = 1;
		//			Counter_UP_KEY_100usec = 0;
		//		}
		Up_key_ShortPress = 0;
		//		Counter_UP_KEY_100usec = 0;
		//		Up_key_released = 0;
	}
	if((Counter_UP_KEY_100usec >= 700/*2000*/)&&(Counter_UP_KEY_100usec <= 17000)&&(Up_key_released == 1)) /* checking "UP_KEY" is pressed for 500msec or not  */
	{
		Up_key_released = 0;
		if(Up_key_ShortPress == 0)
			/* UP_KEY is pressed for 500 msec */
		{
			if(((dont_throw_alerts_alert_on_screen_present ==0)&&(dont_throw_alerts_BS6_alert_on_screen_present == 0))||(skip_flag == 1))
			{
				button_value = 3;//don't change current page after pressing menu if alert present on screen
			}
			Up_key_ShortPress = 1;
			Up_key_Short	= 1;
			Counter_UP_KEY_100usec = 0;
		}
	}
	else if((Up_key_released == 1)&&((Counter_UP_KEY_100usec > 17000)||(Counter_UP_KEY_100usec < 700)))
	{
		Up_key_released = 0;
		Counter_UP_KEY_100usec = 0;
	}


	if((DOWN_KEY_Flag == 1))
	{
		Counter_DOWN_KEY_100usec++;
	}
	else
	{
		//		if(Counter_DOWN_KEY_100usec >= 2000/*5000*/) /* checking "DOWN_KEY" is pressed for 500msec or not  */
		//		{
		//			/* DOWN_KEY is pressed for 500 msec */
		//			button_value = 2;
		//			Counter_DOWN_KEY_100usec = 0;
		//		}
		Down_key_ShortPress = 0;
		//		Counter_DOWN_KEY_100usec = 0;
	}
	if((Counter_DOWN_KEY_100usec >= 700/*2000*/)&&(Counter_DOWN_KEY_100usec <= 17000)&&(Down_key_released == 1)) /* checking "DOWN_KEY" is pressed for 500msec or not  */
	{
		Down_key_released = 0;
		if(Down_key_ShortPress == 0)
			/* DOWN_KEY is pressed for 500 msec */
		{
			Arrowfill_flag = 1;
			if(((dont_throw_alerts_alert_on_screen_present ==0)&&(dont_throw_alerts_BS6_alert_on_screen_present == 0))||(skip_flag == 1))
			{
				button_value = 2;//don't change current page after pressing menu if alert present on screen
			}
			Down_key_ShortPress = 1;
			Down_key_Short = 1;
			Counter_DOWN_KEY_100usec = 0;
		}
	}
	else if((Down_key_released == 1)&&((Counter_DOWN_KEY_100usec > 17000)||(Counter_DOWN_KEY_100usec < 700)))
	{
		Down_key_released = 0;
		Counter_DOWN_KEY_100usec = 0;
	}


	if((MENU_KEY_Flag == 1))
	{
		Counter_MENU_KEY_100usec++;
	}
	else
	{

		Menu_key_ShortPress = 0;
		//		Counter_MENU_KEY_100usec = 0;
	}

	if((Counter_MENU_KEY_100usec >= 700/*2000*/)&&(Counter_MENU_KEY_100usec <= 17000)&&(Menu_key_released == 1)) /* checking "MENU_KEY" is pressed for 500msec or not  */
	{
		Menu_key_released = 0;
		if(Menu_key_ShortPress == 0)
			/* MENU_KEY is pressed for 500 msec */
		{
			if(((dont_throw_alerts_alert_on_screen_present ==0)&&(dont_throw_alerts_BS6_alert_on_screen_present == 0))||(skip_flag == 1))
			{
				button_value = 1;//don't change current page after pressing menu if alert present on screen
			}
			Menu_key_ShortPress = 1;
			if(Odo_reset_8sec_on == 1)//to avoid incrementing key counter after 8 sec of IGN on
			{
				Odo_rst_count++;
			}
			Counter_MENU_KEY_100usec = 0;
		}
	}
	else if((Menu_key_released == 1)&&((Counter_MENU_KEY_100usec > 17000)||(Counter_MENU_KEY_100usec < 700)))
	{
		Menu_key_ShortPress = 0;
		Counter_MENU_KEY_100usec = 0;
		Menu_key_released = 0;
	}

#endif
	//	if(MODE_KEY_Flag == 1)
	//	{
	//		Counter_MODE_KEY_100usec++;
	//		Counter_MODE_KEY_LongPress_100usec++;
	//	}
	//	else
	//	{
	//		Counter_MODE_KEY_100usec = 0;
	//		Counter_MODE_KEY_LongPress_100usec = 0;
	//	}
	//
	//	if(SET_KEY_Flag == 1)
	//	{
	//		Counter_SET_KEY_100usec++;
	//		Counter_SET_KEY_LongPress_100usec++;
	//	}
	//	else
	//	{
	//		Counter_SET_KEY_100usec = 0;
	//		Counter_SET_KEY_LongPress_100usec = 0;
	//	}


	/*For W501 SWSC*/

	if(IGN_ON_flag == 1)
	{
		IGN_start_time++;
		if(IGN_start_time >= 50000)
		{
			IGN_timer_5s = 1;
		}
	}

	if(DoorClose_ON == 1)
	{
		DoorClose_1sec++;
	}
	if(Engine_oil_pressure_low_Timer_ON_flag == 1)	//flag should get on after EOP TT on
	{
		Engine_oil_pressure_low_Timer_count++;
	}
	//	if(Check_ABS_Timer_ON_flag == 1)				//flag should get on after IGN on
	//	{
	//		Check_ABS_Timer_count++;
	//	}
	//	if(High_Temp_Timer_ON_flag == 1)				//flag should get on after IGN on
	//	{
	//		High_Temp_Timer_count++;
	//	}
	if(Low_Brake_Fluid_Timer_ON_flag == 1)			//flag should get on after getting HW I/P
	{
		Low_Brake_Fluid_Timer_count++;
	}
	//	if(Fuel_Low_Timer_ON_flag == 1)					//flag should get on after IGN on
	//	{
	//		Fuel_Low_Timer_count++;
	//	}

	if(PAB_Timer_ON == 1)
	{
		if(PAB_Timer_count < 40000)
		{
			PAB_Timer_count ++;
			PAB_Time_Done = 0;
		}
		else
		{
			PAB_Time_Done = 1;
		}
	}
	else
	{
		PAB_Timer_count = 0;
	}

	if(DPF_Clogged_Timer_ON_flag == 1)
	{
		DPF_Clogged_Timer_count++;
	}

	if(Water_In_Fuel_Timer_ON ==1)
	{
		Water_In_Fuel_Timer_count++;
		if(Water_In_Fuel_Timer_count >= Eng_running_time)
		{
			Water_In_Fuel_Timer_ON = 0;
			WIF_5sec_done = 1;
		}
	}
	else
	{
		Water_In_Fuel_Timer_count = 0;
	}
	/* ************************************************************************** */
}

void CAN_Message_Timeout_Recovery_Detection(uint8_t Msg_Status, 
		Message_Timeout_t * Msg_Timeout)
{
	if(Msg_Timeout->Message_Timeout_Status == CLEAR)
	{
		if(Msg_Status == CLEAR)
		{
			Msg_Timeout->Timeout_Counter++;
			if(Msg_Timeout->Timeout_Counter >= Msg_Timeout->Timeout_Threshold)
			{
				Msg_Timeout->Message_Timeout_Status = SET;
				Msg_Timeout->Timeout_Counter = 0;
				Msg_Timeout->Message_Received_Counter = 0;
				Msg_Timeout->Recovery_Counter = 0;
				Msg_Timeout->Message_Recovery_Status = CLEAR;
			}
		}
		else
		{
			Msg_Timeout->Message_Timeout_Status = CLEAR;
			Msg_Timeout->Timeout_Counter = 0;
		}
	}
	else
	{
		if(Msg_Status == SET)
		{
			Msg_Timeout->Message_Received_Counter++;
		}

		Msg_Timeout->Recovery_Counter++;

		if(Msg_Timeout->Recovery_Counter >= Msg_Timeout->Recovery_Threshold)
		{
			if(Msg_Timeout->Message_Received_Counter >= Msg_Timeout->Message_Received_Threshold)
			{
				Msg_Timeout->Message_Recovery_Status = SET;
				Msg_Timeout->Message_Timeout_Status = CLEAR;
				Msg_Timeout->Timeout_Counter = 0;
			}			

			Msg_Timeout->Message_Received_Counter = 0;
			Msg_Timeout->Recovery_Counter = 0;
		}
	}
}

void CAN_message_Timeout(void)
{
	if(IGN == SET)//IGN_ON_flag
	{
		if(Network_Message_Monitor_Start == 0)
		{
			Network_Message_Monitor_Counter++;

			if(Network_Message_Monitor_Counter >= (IVN_cfg_NM_IGN_On_Startup_Delay/10))
			{
				Network_Message_Monitor_Start = 1;
				Network_Message_Monitor_Counter = 0;
			}
		}
	}
	else
	{
		Network_Message_Monitor_Start = 0;
		Network_Message_Monitor_Counter = 0;
	}

	if((Network_Message_Monitor_Start == 1) && (CAN_Bus_Off == CLEAR) && (LVSD_CAN == 0)&&
			(HVSD_CAN == 0))
	{
		if(U8g_Block_EMS_ECU_CAN_MSG == CLEAR)
		{
			/* EMS3_10 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(EMS3_10_message_ON,&EMS3_10_Message);

			if(EMS3_10_Message.Message_Timeout_Status == CLEAR)
			{
				/* EMS36_10 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(EMS36_10_message_ON,&EMS36_10_Message);

				/* EMS1_10 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(EMS1_10_message_ON,&EMS1_10_Message);

				/* EMS8_10 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(EMS8_10_message_ON,&EMS8_10_Message);

				/* EMS4_20 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(EMS4_20_message_ON,&EMS4_20_Message);

				/* EMS29_100 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(EMS29_100_message_ON,&EMS29_100_Message);

				/* EMS6_500 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(EMS6_500_message_ON,&EMS6_500_Message);

				/* EMS12_200 Message Timeout */
				if(FEATURE_CODE_BATTERY_TT_TYPE==CLEAR)
				{
					CAN_Message_Timeout_Recovery_Detection(EMS12_200_message_ON,&EMS12_200_Message);
				}
				
				if(IVN_NS_VAR_CODING_ENGINE_MANAGEMENT_SYSTEM == SET)
				{
					/* EMS_NSM Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(EMS_NSM_message_ON,&EMS_NSM_Message);
				}
			}
			else
			{
				if(EMS3_10_Message.Message_Timeout_Status == SET)
				{
					/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
					EMS36_10_Message.Timeout_Counter = 0;
					EMS36_10_Message.Recovery_Counter = 0;
					EMS36_10_Message.Message_Received_Counter = 0;

					EMS1_10_Message.Timeout_Counter = 0;
					EMS1_10_Message.Recovery_Counter = 0;
					EMS1_10_Message.Message_Received_Counter = 0;

					EMS8_10_Message.Timeout_Counter = 0;
					EMS8_10_Message.Recovery_Counter = 0;
					EMS8_10_Message.Message_Received_Counter = 0;

					EMS4_20_Message.Timeout_Counter = 0;
					EMS4_20_Message.Recovery_Counter = 0;
					EMS4_20_Message.Message_Received_Counter = 0;

					EMS29_100_Message.Timeout_Counter = 0;
					EMS29_100_Message.Recovery_Counter = 0;
					EMS29_100_Message.Message_Received_Counter = 0;

					EMS6_500_Message.Timeout_Counter = 0;
					EMS6_500_Message.Recovery_Counter = 0;
					EMS6_500_Message.Message_Received_Counter = 0;

					EMS12_200_Message.Timeout_Counter = 0;
					EMS12_200_Message.Recovery_Counter = 0;
					EMS12_200_Message.Message_Received_Counter = 0;

					EMS5_500_Message.Timeout_Counter = 0;
					EMS5_500_Message.Recovery_Counter = 0;
					EMS5_500_Message.Message_Received_Counter = 0;

					EMS_NSM_Message.Timeout_Counter = 0;
					EMS_NSM_Message.Recovery_Counter = 0;
					EMS_NSM_Message.Message_Received_Counter = 0;
				}
			}
		}

		if((U8g_Block_ESC_ECU_CAN_MSG == CLEAR)  || (U8g_Block_ABS_ECU_CAN_MSG == CLEAR))
		{
			/* ECS5_10 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(ESC5_10_message_ON,&ESC5_10_Message);

			if(ESC5_10_Message.Message_Timeout_Status == CLEAR)
			{
				if(FEATURE_CODE_SPEED_INPUT == CLEAR)
				{
					/* ESC2_10 Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(ESC2_10_message_ON,&ESC2_10_Message);
				}
				else
				{
					/* ESC12_10 Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(ESC12_10_message_ON,&ESC12_10_Message);
				}

				if(IVN_NS_VAR_CODING_ELECTRONICS_STABILITY_CONTROL == SET)
				{
					/* ESC_NSM Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(ESC_NSM_message_ON,&ESC_NSM_Message);
				}
			}
			else
			{
				if(ESC5_10_Message.Message_Timeout_Status == SET)
				{
					/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
					ESC2_10_Message.Timeout_Counter = 0;
					ESC2_10_Message.Recovery_Counter = 0;
					ESC2_10_Message.Message_Received_Counter = 0;

					ESC12_10_Message.Timeout_Counter = 0;
					ESC12_10_Message.Recovery_Counter = 0;
					ESC12_10_Message.Message_Received_Counter = 0;

					ESC_NSM_Message.Timeout_Counter = 0;
					ESC_NSM_Message.Recovery_Counter = 0;
					ESC_NSM_Message.Message_Received_Counter = 0;
				}
			}
		}

		if(U8g_Block_TCU_ECU_CAN_MSG == CLEAR)
		{
			/* TCU5_10 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(TCU5_10_message_ON,&TCU5_10_Message);

			if(TCU5_10_Message.Message_Timeout_Status == CLEAR)
			{
				/* TCU6_20 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(TCU6_20_message_ON,&TCU6_20_Message);

				if(IVN_NS_VAR_CODING_TRANSMISSION_CONTROL_UNIT == SET)
				{
					/* TCU_NSM Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(TCU_NSM_message_ON,&TCU_NSM_Message);
				}
			}
			else
			{
				if(TCU5_10_Message.Message_Timeout_Status == SET)
				{
					/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
					TCU6_20_Message.Timeout_Counter = 0;
					TCU6_20_Message.Recovery_Counter = 0;
					TCU6_20_Message.Message_Received_Counter = 0;

					TCU_NSM_Message.Timeout_Counter = 0;
					TCU_NSM_Message.Recovery_Counter = 0;
					TCU_NSM_Message.Message_Received_Counter = 0;
				}
			}
		}

		if(U8g_Block_BCM_ECU_CAN_MSG == CLEAR)
		{
			/* MBFM1_100 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(MBFM1_100_message_ON,&MBFM1_100_Message);

			if(MBFM1_100_Message.Message_Timeout_Status == CLEAR)
			{
#ifndef U350
				/* MBFM5_100 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(MBFM5_100_message_ON,&MBFM5_100_Message);

				/* MBFM9_500 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(MBFM9_500_message_ON,&MBFM9_500_Message);

				/* MBFM_PAS1_50 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(MBFM_PAS1_50_message_ON,&MBFM_PAS1_50_Message);
#endif
			}
			else
			{
				if(MBFM1_100_Message.Message_Timeout_Status == SET)
				{
					/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
					MBFM5_100_Message.Timeout_Counter = 0;
					MBFM5_100_Message.Recovery_Counter = 0;
					MBFM5_100_Message.Message_Received_Counter = 0;

					MBFM9_500_Message.Timeout_Counter = 0;
					MBFM9_500_Message.Recovery_Counter = 0;
					MBFM9_500_Message.Message_Received_Counter = 0;

					MBFM_PAS1_50_Message.Timeout_Counter = 0;
					MBFM_PAS1_50_Message.Recovery_Counter = 0;
					MBFM_PAS1_50_Message.Message_Received_Counter = 0;
				}
			}
		}

			if(U8g_Block_RPAS_ECU_CAN_MSG==CLEAR)
			{
				/* RPAS1_20 Node Absent */
				CAN_Message_Timeout_Recovery_Detection(RPAS1_20_message_ON,&RPAS1_20_Message);

				if(RPAS1_20_Message.Message_Timeout_Status == CLEAR)
				{
					/* RPAS2_100 Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(RPAS2_100_message_ON,&RPAS2_100_Message);
				}
				else
				{
					if(RPAS1_20_Message.Message_Timeout_Status == SET)
					{
						/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
						RPAS2_100_Message.Timeout_Counter = 0;
						RPAS2_100_Message.Recovery_Counter = 0;
						RPAS2_100_Message.Message_Received_Counter = 0;
					}
				}
			}
#ifdef SPRINT4
		/* SAS msg modification */
		if(U8g_Block_SAS_ECU_CAN_MSG==CLEAR)
			{
				/* SAS1_10 Node Absent */
				CAN_Message_Timeout_Recovery_Detection(SAS1_10_message_ON,&SAS1_10_Message);
#if 0
			if(SAS1_10_Message.Message_Timeout_Status == CLEAR)
			{
				/* SAS1_10 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(SAS1_10_message_ON,&SAS1_10_Message);
			}
			else
			{
				if(SAS1_10_Message.Message_Timeout_Status == SET)
				{
					/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
					SAS1_10_Message.Timeout_Counter = 0;
					SAS1_10_Message.Recovery_Counter = 0;
					SAS1_10_Message.Message_Received_Counter = 0;
				}
			}
#endif
		}
#endif




		if(U8g_Block_SRS_ECU_CAN_MSG == CLEAR)
		{
			/* SRS1_20 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(SRS1_20_message_ON,&SRS1_20_Message);
		}

		if((U8g_Block_PKE_ECU_CAN_MSG == CLEAR) || (U8g_Block_ICU_ECU_CAN_MSG == CLEAR))
		{
			/* PKE_ICU2_100 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(PKE_ICU2_100_message_ON,&PKE_ICU2_100_Message);
		}

		if(U8g_Block_IS_ECU_CAN_MSG == CLEAR)
		{
			/* IS1_100 Node Absent */
			CAN_Message_Timeout_Recovery_Detection(IS1_100_message_ON,&IS1_100_Message);

			if(IS1_100_Message.Message_Timeout_Status == CLEAR)
			{
#ifndef U350
				/* IS3_500 Message Timeout */
				CAN_Message_Timeout_Recovery_Detection(IS3_500_message_ON,&IS3_500_Message);
#endif
				if(IVN_NS_VAR_CODING_INFOTAINMENT_SYSTEM == SET)
				{
					/* IS_NSM Message Timeout */
					CAN_Message_Timeout_Recovery_Detection(IS_NSM_message_ON,&IS_NSM_Message);
				}
			}
			else
			{
				if(IS1_100_Message.Message_Timeout_Status == SET)
				{
					/* Clear Other Message Timeout Variables to start afresh to determine message timeout */
					IS3_500_Message.Timeout_Counter = 0;
					IS3_500_Message.Recovery_Counter = 0;
					IS3_500_Message.Message_Received_Counter = 0;

					IS_NSM_Message.Timeout_Counter = 0;
					IS_NSM_Message.Recovery_Counter = 0;
					IS_NSM_Message.Message_Received_Counter = 0;
				}
			}
		}
	}

	if(ESC2_10_message_ON == 1)
	{
		ESC2_10_message_ON = 0;
	}

	if(ESC12_10_message_ON == 1)
	{
		ESC12_10_message_ON = 0;
	}

	IGNOFF_CAN_counter++;
	if(IGNOFF_CAN_counter >= 100)
	{
		IGNOFF_CAN_counter = 100;
		IGNOFF_MBFM_CAN_ON = 0;
		IGNOFF_display_timeout = 1; /*FOR alert IGN off alert exit state - 28-05-20*/

	}


	if(IGNOFF_MBFM_CAN_ON == 0)
	{
		IGNOFF_MBFM1_100_Message_Period_Counter++;
		if(IGNOFF_MBFM1_100_Message_Period_Counter >= (IVN_cfg_Msg_Timeout_Detection_Threshold *10)) /* timeout condition */
		{
			IGNOFF_MBFM1_100_Message_Period_Counter = 0;
			IGNOFF_MBFM1_100_message_timeout_flag = 1;
			IMMO_IGN_OFF = 0;//test
			//			display_active = 0;
		}
		IGNOFF_MBFM1_100_Message_Recovery_Counter = 0;
	}
	else
	{
		if(IGNOFF_MBFM1_100_message_timeout_flag == 1)
		{
			IGNOFF_MBFM1_100_Message_Recovery_Counter++;
			if (IGNOFF_MBFM1_100_Message_Recovery_Counter >= (IVN_cfg_Msg_Timeout_Recovery_Threshold))//removed multiplication factor
			{
				IGNOFF_MBFM1_100_Message_Period_Counter = 0;
				IGNOFF_MBFM1_100_message_timeout_flag = 0;
			}	
		}
		else
		{
			IGNOFF_MBFM1_100_Message_Period_Counter = 0;
			IGNOFF_MBFM1_100_message_timeout_flag = 0;
		}
	}

}

void WatchDog_Clear(void)
{
	WDTA1WDTE = 0xAC;
	WDT_Counter = 0;
}

void seat_belt_chime_condition(void)
{
	uint8_t STS_Eng_data = 0U;
	STS_Eng_data = CAN_EMS1_10[6].byte;
	STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
	if(rpm8000_arrived == 0)
	{
		if((final_tachometer_speed >= Diag_RW_Eng_Run_threshold_RPM) && (final_tachometer_speed != 0xFFFF) &&(STS_Eng_data == 2))
		{
			one_MIN_rpm_counter++;
			//	Diag_time/actual time = multiplication factor
#ifdef GRAYSCALE_TFT
			if(one_MIN_rpm_counter >= ((Diag_RW_Eng_Run_threshold_time*100)*0.68))//multiplication factor added to reduce timer offset 0.68
			{
				rpm8000_arrived = 1;
			}
			else
			{
				rpm8000_arrived = 0;
			}
#endif
#ifdef COLOR_TFT
			if(one_MIN_rpm_counter >= ((Diag_RW_Eng_Run_threshold_time*100)*0.90))//multiplication factor added to reduce timer offset 0.90
			{
				rpm8000_arrived = 1;
			}
			else
			{
				rpm8000_arrived = 0;
			}
#endif
		}
		else
		{
			one_MIN_rpm_counter  = 0;
		}
	}
	else if(final_tachometer_speed < Diag_RW_Eng_Run_threshold_RPM)
	{
		rpm8000_arrived=0;
		Less_tn_thrhld_rpm = 1;//if RPM less than threshold RPM bit get set
	}
	else
	{

	}
	if(final_tachometer_speed < Diag_RW_Eng_Run_threshold_RPM)
	{
		Less_tn_thrhld_rpm = 1;//if RPM less than threshold RPM bit get set
	}
	else
	{
		Less_tn_thrhld_rpm = 0;//if RPM greater than threshold RPM bit get reset
	}

	if(SBR_chime_condition_arrived == 1)
	{
		if(vehicle_moving_state == 1)
		{
			one_MIN_spd_counter++;
			//	Diag_time/actual time = multiplication factor
#ifdef GRAYSCALE_TFT
			if(one_MIN_spd_counter >= ((Diag_RW_Vehicle_Moving_threshold_time*100)*0.68))//multiplication factor added to reduce timer offset 0.68
			{
				spd_2km_arrived = 1;
			}
			else
			{
				spd_2km_arrived = 0;
			}
#endif
#ifdef COLOR_TFT
			if(one_MIN_spd_counter >= ((Diag_RW_Vehicle_Moving_threshold_time*100)*0.90))//multiplication factor added to reduce timer offset 0.90
			{
				spd_2km_arrived = 1;
			}
			else
			{
				spd_2km_arrived = 0;
			}
#endif
		}
		else
		{
			one_MIN_spd_counter  = 0;
			spd_2km_arrived=0;
		}

		// for ODO
		if(final_odomter_distance - prev_sbrchm_odo >= Diag_RW_Vehicle_Moving_threshold_distance)
		{
			ODO_arrived =1;
		}
		else
		{
			ODO_arrived =0;
		}
	}
	else
	{
		prev_sbrchm_odo=final_odomter_distance;
	}
}
uint8_t RPAS_level=0;

void RPAS_LV_Chime_Play(uint8_t level)
{
	RPAS_level = level;
}

void RPAS_LV_Chime_Dispacher(void)
{
	if(Sound_playing == 0)
	{
		switch(RPAS_level)
		{
		case 0x01:SoundPCM_4_rec_RPAS_St1_St2_cont();
		break;
		case 0x02:SoundPCM_RPAS_LV2();
		break;
		case 0x03:SoundPCM_RPAS_LV3();
		break;
		case 0x04:SoundPCM_RPAS_LV4();
		break;
		default:RPAS_level=0x00;
		break;
		};
		RPAS_level=0x00;
	}
	else
	{
		switch(RPAS_level)
		{
		case 0x01:SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
		break;
		case 0x02:SoundPCM_RPAS_LV2_STOP();
		break;
		case 0x03:SoundPCM_RPAS_LV3_STOP();
		break;
		case 0x04:SoundPCM_RPAS_LV4_STOP();
		break;
		default:RPAS_level=0x00;
		break;
		};

	}

}


