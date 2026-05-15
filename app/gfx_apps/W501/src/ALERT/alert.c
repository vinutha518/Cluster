/* **********************************************************************************************************************
File                    :       alert.c
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */

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
#include "TLC6C_S_P.h"
#include "serial_to_parallel.h"
#include "parallel_to_serial.h"
#include "UDSS_APP.h"
#include "r_cg_serial_user.h"
#include "swsc.h"
#include "dr7f701461.dvf.h"
#include "r_test_ism.h"
#include "my_can.h"
#include "UDS_DTC.h"
#include "lcbi_non_tft.h"
#include "sound_pcm.h"
#include "r_taub_api.h"
#include "gearIndication.h"
#include "FOURxFOUR.h"
#include "TDMS.h"

void Get_Default_Service_Slab(void);
void Door_Open(void);
void Engine_oil_pressure_low(void);
void Check_ABS(void);
void Seat_belt(void);
void High_Temp(void);
void Low_Brake_Fluid(void);
void Fuel_Low(void);
void DEF_Level(void);
void DPF_Filter_Clogged(void);
void REGEN_Alert(void);
void DEF_alerts(void);

void Park_Break_alert(void);
void Water_In_Fuel_alert(void);
void Overspeed_alert(void);
void Check_EBD_alert(void);
void Check_ESP_alert(void);
void RKE_Low_Battery_alert(void);
void Check_Engine_alert(void);
void Service_reminder(void);
void Cruise_alerts(void);
void Check_EXT_Lamp(void);
void ECO_Mode_alerts(void);
void PAB_Off_alerts(void);
void Trailer_SW_On_alerts(void);
void Horn_failure_alerts(void);
void DRL_failure_alerts(void);
void LEFT_Turn_Lamp_failure_alerts(void);
void RIGHT_Turn_Lamp_failure_alerts(void);
void Tail_Lamp_failure_alerts(void);
void RPAS_Supply_Failure_alerts(void);
void Key_In_Reminder(RX_DATA *can_buffer);
void Func_safety(void);
void Stop_for_setting(void);
void ETM_LED_test(void);
void HDC_alert(void);
void AT_gear_alert(void);
/*************************************************************/
void RPAS_Precondition_U350(void);
void RPAS_Distance_Display_U350(void);
void RPAS_Status_Display_U350(void);
void RPAS_Screen_Activation_U350(void);
/*************************************************************/
/*************************************************************/
void RPAS_Precondition_W501(void);
void RPAS_Distance_Display_W501(void);
void RPAS_Status_Display_W501(void);
void RPAS_Screen_Activation_W501(void);
/*************************************************************/
void Warning_History_dooropen(void);

void IO_Control_Message_activation(void);
void IO_Control_TFT_Test_Grayscale(void);
void IO_Control_TFT_Test_ColourTFT(void);
void Door_open_animation_parareset(void);
void Handle_Inactive_alerts(void);
//uint8_t Alert_priority = 1;
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

}DID_TT_bit_data_set1;

extern _Bool fuel_alert_on_flag;
extern _Bool Low_fuel_ongoing;
extern uint8_t cycle_lowfuel_completed;
_Bool dpf_alert_flag;
extern _Bool Passenger_Seat_belt_alert_flag_ON;
extern _Bool Driver_Seat_belt_alert_flag_ON;
extern _Bool Enginerun_chm;
extern _Bool vehicle_moving_state;
extern _Bool ok_modelong_press;
extern _Bool set_key_short;
extern _Bool Up_key_Short;
extern _Bool Down_key_Short;
extern uint16_t Proveout_Count;
extern uint32_t display_tachometer_speed;
extern _Bool ETM_10_sec_exit_flag;
/**************RPAS Variables****************/
uint8_t RPAS_display_dist = 0;
uint8_t RPAS__update_display_dist = 0;
/*_Bool*/uint8_t RPAS_Precondition_Active = 2;
_Bool staybacktime_active = 0;
_Bool RPAS_ERRORtext_display = 0;
uint8_t RPAS_DIST_text_display = 0;
uint16_t staybacktime_count = 0;
uint8_t RPAS_RLeft_imageIndex = 0;
uint8_t RPAS_RRight_imageIndex = 0;
uint8_t RPAS_RCenter_imageIndex = 0;
uint8_t RPAS_LCenter_imageIndex = 0;
uint8_t RPAS_ERROR = 0;
_Bool sector_Zone_error;
uint8_t RPAS_All_sensor_error = 0;
uint8_t RPAS_Error_detected = 0;
extern uint8_t Gear_config;
extern uint16_t AT_Reversegear_timespan;
extern _Bool AT_Reverse_detected;
_Bool RPAS_ACTIVE_STS_flag = 0;
/********************************************/
uint8_t Engine_Temp_High_popup_shown=0;
uint8_t Low_Fuel_popup_shown=0;
uint8_t Check_ABS_popup_shown=0;
uint8_t Low_Brake_Fluid_popup_shown=0;
uint8_t Park_brake_popup_shown=0;
uint8_t Low_Engine_Oil_Pressure_popup_shown=0;
uint8_t Seat_Belt_popup_shown=0;
uint8_t DPF_Clogged_popup_shown=0;
uint8_t Water_In_Fuel_popup_shown = 0;
uint8_t Overspeed_popup_shown = 0;
uint8_t EBD_popup_shown = 0;
uint8_t ESP_popup_shown = 0;
uint8_t Low_Battery_popup_shown = 0;
uint8_t Check_Engine_popup_shown = 0;
uint8_t Service_Reminder_popup_shown = 0;
uint8_t	Cruise_alerts_popup_shown = 0;
_Bool CruiseOFF_alerts_popup_shown = 0;
_Bool CruiseON_alerts_popup_shown = 0;
uint8_t	Check_EXT_Lamp_popup_shown = 0;
uint8_t Eco_mode_popup_shown = 0;
_Bool EcoOFF_mode_popup_shown = 0;
_Bool EcoON_mode_popup_shown = 0;
uint8_t PAB_Off_popup_shown = 0;
_Bool Trailer_SW_on_popup_shown = 0;
_Bool Trailer_SW_off_popup_shown = 0;
uint8_t Horn_Fail_popup_shown = 0;
_Bool Left_DRL_failure_popup_shown = 0;
_Bool Right_DRL_failure_popup_shown = 0;
_Bool Left_Turn_failure_popup_shown = 0;
_Bool Right_Turn_failure_popup_shown = 0;
_Bool Tail_Lamp_failure_popup_shown = 0;
_Bool RPAS_Supply_failure_popup_shown = 0;
_Bool Key_In_Alert_popup_shown = 0;
_Bool Func_safety_popup_shown = 0;
_Bool RPAS_ACTIVE_popup_shown = 0;
_Bool stop_vehicle_setting_shown = 0;
_Bool HDC_Ready_popup_shown = 0;
_Bool HDC_Active_popup_shown = 0;
_Bool HDC_Passive_popup_shown = 0;

_Bool ALERT_CHECK;
uint32_t Alert_key = 0;
/************************************************/
uint8_t Dooropen_IGNOFF_popup_shown = 0;
_Bool Doorclose_IGNOFF_popup_shown = 0;
uint8_t Check_EXT_Lamp_IGNOFF_popup_shown = 0;
uint8_t	Dooropen_active_IGNOFF = 0;
uint8_t	Checklamp_active_IGNOFF = 0;
uint8_t	Dooropen_active_IGNOFF_Alert = 0;
uint8_t	Checklamp_active_IGNOFF_Alert = 0;
uint8_t	Keyinreminder_IGNOFF_Alert = 0;
uint8_t	IGNOFF_Dooropen_check_done = 0;
uint8_t	IGNOFF_ExtLamp_check_done = 0;
uint8_t	IGNOFF_keyinremind_check_done = 0;
extern uint8_t IGNOFF_display_timeout;
extern uint8_t flag_grayscale;
extern uint8_t refresh_rate;
uint8_t	wait_state = 0;
uint8_t	Not_IGN_Wakeup = 0;
/************************************************/
_Bool PARK_LAMP_IGN_OFF;
_Bool DOOR_OPEN_IGN_OFF;
_Bool IMMO_IGN_OFF;
_Bool IMMO_Prev_State;
extern _Bool telltale_blink_bool4;
extern _Bool telltale_blink_bool5;
_Bool previous_state_immobilizer;
_Bool previous_state_immobilizer_blink4;
_Bool previous_state_immobilizer_blink5;
extern _Bool IMMO_TT_WAKEUP_FLAG;
uint16_t IMMO_TT_OFF_CNTR;
/*******************************************************/
_Bool RPAS_STAYBACK_FLAG=0;
 uint8_t rpas_stay_counter=1;
 _Bool debounce_compare_flag=0;
/******************************************************/

uint8_t Dooropen_popup_shown = 0;
uint8_t Codriver_flag = 0;
uint8_t Driver_flag = 0;
uint8_t Rearleft_flag = 0;
uint8_t RearRight_flag = 0;
uint8_t Hood_flag = 0;
_Bool Back_flag = 0;
_Bool Dooropen_detected = 1;
_Bool IGNOFF_Dooropen_detected = 1;
uint8_t Doorclose_popup_shown = 0;
uint8_t prev_STS_Door_data = 0;
uint8_t IGNOFFprev_STS_Door_data = 0;

uint8_t DoorClose_ON=0;
uint32_t DoorClose_1sec=0;
uint8_t STS_Door_data = 0;
uint8_t Dooropen_active = 0;
uint8_t All_doorclose = 0;
_Bool All_doorclose_timeout_start = 0;
/*Service Reminder*/
uint32_t dummy_odo = 0;
_Bool NVM_SERVICE_DONE_flag = 0;
uint8_t service_reminder = 0;
uint8_t show_service_reminder = 0;
uint8_t memory_read	=	0;//memory read for every IGN cycle
uint32_t SERVICE_SLAB_START[53];
//_Bool service_slab_write = 0;

/*uint32_t SERVICE_SLAB_START[] = {0,801,9801,19801,29801,39801,49801,59801,69801,79801,89801,
								99801,109801,119801,129801,139801,149801,159801,169801,179801,
								189801,199801,209801,219801,229801,239801,249801,259801,269801,
								279801,289801,299801,309801,319801,329801,339801,349801,359801,
								369801,379801,389801,399801,409801,419801,429801,439801,449801,
								459801,469801,479801,489801,499801,500000};
*/
uint32_t SERVICE_SLAB_END[] = 	{800,9800,19800,29800,39800,49800,59800,69800,79800,89800};

uint32_t IGN_start_time = 0;//timer count for 5sec after IGN on
uint8_t IGN_timer_5s = 0;
uint8_t WIF_5sec_done = 0;

//uint8_t Diag_RW_OilPressure_Warn_Enable    = 1U;
uint8_t Engine_oil_pressure_low_Timer_ON_flag = 0;
uint32_t Engine_oil_pressure_low_Timer_count = 0;

//uint8_t Diag_RW_ABS_Warn_Enable = 1;
uint8_t Check_ABS_Timer_ON_flag = 0;
//uint32_t Check_ABS_Timer_count = 0;

//uint8_t Diag_RW_SeatBelt_Warn_Enable = 1;

//uint8_t Diag_RW_EngineTempHigh_Warn_Enable = 1;
//uint32_t High_Temp_Timer_count = 0;
uint8_t High_Temp_Timer_ON_flag = 0;

//uint8_t Diag_RW_LowBrakeFluid_Warn_Enable = 1;
//uint8_t Diag_RW_ParkBrake_Warn_Enable	=	1;
//uint8_t Diag_RW_WIF_Warn_Enable	=	1;
//uint8_t Diag_RW_OverSpeed_Warn_Enable = 1;
//uint8_t Diag_RW_EBD_Warn_Enable = 1;
//uint8_t Diag_RW_RKE_Low_battery_Warn_Enable	= 1;
//uint8_t Diag_RW_CheckEngine_Warn_Enable = 1;

/*OVERSPEED_VALUES*/
extern _Bool Speed_Missing;
uint8_t Over_speed_flag = 0;

_Bool eng_start_timer_on = 0;
uint8_t eng_start_flag = 0;
uint8_t LowBrakeFluidSt_Hw_Input = 0;
uint32_t Low_Brake_Fluid_Timer_count = 0;
uint32_t PAB_Timer_count = 0;
uint8_t Low_Brake_Fluid_Timer_ON_flag = 0;
uint8_t Water_In_Fuel_Timer_ON = 0;
//uint8_t Diag_RW_LowFuel_Warn_Enable = 1;
//uint32_t Fuel_Low_Timer_count = 0;
_Bool PAB_Timer_ON = 0;
_Bool PAB_Time_Done = 0;
uint8_t Fuel_Low_Timer_ON_flag = 0;
uint8_t Check_Engine_flag = 0;

uint32_t DPF_Clogged_Timer_count = 0;
uint8_t DPF_Clogged_Timer_ON_flag = 0;

uint8_t cruise_sts = 0;
uint8_t Eco_sts = 0;
uint8_t HDC_STS = 0;
_Bool IGN_eco = 0;
_Bool IGN_cruise = 0;
uint8_t Trailer_sts = 0;
uint8_t skip_alert_Enable = 0;
_Bool IGN_HDC = 0;

extern _Bool LOP_7Sec_cntr_done;
extern _Bool LBF_3Sec_cntr_done;

extern uint8_t OverSpeedLevel2Val,OverSpeedHystresis;
extern uint32_t key;
extern uint32_t Sound_Bit;

/*_Bool*/ uint8_t alerts_history[40]	=	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/***************************Alert_skip*******************************/

static uint8_t reverse_debounce_cntr=0;
 uint16_t rpas_stayback_cntr=0;
_Bool rpasstay_compare_flag=0;
uint8_t rpas1_stayback_cntr=0;
_Bool compare_flag=0;

extern uint8_t regen_req_start;
//_Bool Overspeed_popup_skip = 0;
//_Bool Seat_Belt_popup_skip = 0;
//_Bool Check_EXT_Lamp_popup_skip = 0;
//_Bool RPAS_ACTIVE_popup_skip = 0;
_Bool Low_Engine_Oil_Pressure_skip = 0;
_Bool Engine_Temp_High_skip = 0;
_Bool Low_Brake_Fluid_skip = 0;
_Bool Park_brake_popup_skip = 0;
_Bool PAB_Off_popup_skip = 0;
_Bool HDC_popup_skip = 0;
_Bool DPF_Clogged_popup_skip = 0;
_Bool Dooropen_popup_skip = 0;
_Bool Check_Engine_popup_skip = 0;
_Bool Low_Fuel_popup_skip = 0;
_Bool EBD_popup_skip = 0;
_Bool Check_ABS_popup_skip = 0;
_Bool ESP_popup_skip = 0;
_Bool RPAS_Supply_failure_popup_skip = 0;
_Bool Low_Battery_popup_skip = 0;
_Bool Water_In_Fuel_popup_skip = 0;
_Bool Service_Reminder_popup_skip = 0;
//_Bool Cruise_alerts_popup_skip = 0;
//_Bool Eco_mode_popup_skip = 0;
_Bool Func_safety_popup_skip = 0;
//_Bool Trailer_SW_popup_skip = 0;
_Bool Left_Turn_failure_popup_skip = 0;
_Bool Right_Turn_failure_popup_skip = 0;
_Bool Left_DRL_failure_popup_skip = 0;
_Bool Right_DRL_failure_popup_skip = 0;
_Bool Tail_Lamp_failure_popup_skip = 0;
_Bool Horn_Fail_popup_skip = 0;
//#ifdef SPRINT4
_Bool TDMS_popup_skip = 0;
//#endif
_Bool Key_In_Alert_popup_skip = 0;


_Bool skip_flag = 0;

_Bool SBR_Alert_check = 0;
_Bool Ign_on_SBR_alert_trigger = 0;
extern _Bool dpf_check;
extern uint8_t decrement_trend;
uint8_t IO_Control_TFT = 0;
_Bool Priority_flag = 0;
_Bool DEF_priority_flag = 1;
_Bool REGEN_priority_flag = 1;
_Bool checkRegenDEF_flag = 0;
uint8_t Speed_present = 0;
_Bool Notification_chime;
_Bool KIR_Chime;

_Bool SBR_IGN_ON = 0;
_Bool parkLampChime;	/* @@@ Flag for Park Lamp Chime Status */
_Bool WH_backdoor = 0;
_Bool WH_Hooddoor = 0;
_Bool WH_codriverdoor = 0;
_Bool WH_driverdoor = 0;
_Bool WH_Rearleftdoor = 0;
_Bool WH_Rearrightdoor = 0;

_Bool AT_gear_alert_active = 0;
_Bool ATgear_popup_shown = 0;
_Bool AT_gear_debounce_start = 0;
_Bool AT_gearalert_reverse_display = 0;
_Bool AT_gear_IGN_ON = 0;
_Bool AT_gear_IGM_ON_detected = 0;
uint8_t	AT_Gear_display = 0;
uint8_t	PREV_TGS_LEVER = 0;
uint8_t TGS_LEVER1 = 0U;
uint8_t PREV_TGS_LEVER1 = 0;
uint32_t AT_gear_debounce_cnt = 0;
uint32_t AT_gear_update_cnt = 0;
uint32_t start_AT_alert_timeout_cnt = 0;

extern uint8_t cycleParkLamp_cntr;
extern uint8_t cycleKIR_cntr;
uint8_t priorityPark_Keyin;

extern uint8_t cycle133_cntr;
extern uint8_t cycle133_completed;
extern unsigned int RPAS_LV1_COUNTER;
extern unsigned int RPAS_LV2_COUNTER;
extern unsigned int RPAS_LV3_COUNTER;
extern unsigned int RPAS_LV4_COUNTER;

extern _Bool W501_LOW_RPAS_LEVEL1_PRECON;
extern uint8_t W501_LOW_RPAS_COUNTER2;
extern uint8_t W501_LOW_RPAS_COUNTER3;
extern uint8_t W501_LOW_RPAS_COUNTER4;
extern uint8_t Gear_Configuration;

#ifdef U350
extern _Bool RPAS_LEVEL1_PRECON;
extern _Bool RPAS_LEVEL2_BUZ_ONGOING;
extern _Bool RPAS_LEVEL3_BUZ_ONGOING;
extern _Bool RPAS_LEVEL4_BUZ_ONGOING;
#endif

extern uint8_t *RRAM_IMMO_PATT4;
extern uint8_t *RRAM_IMMO_PATT5;

#define KEY_IN 1
#define PARK_LAMP 2

/**********************/
#ifdef COLOR_TFT
#define RPAS_DEBOUNCE 0.750F
#endif
/******************************************************************************************/
static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
	}bits;
	unsigned char byte;

}REGEN_bit_data;


static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
	}bits;
	unsigned char byte;

}DEF_bit_data;
/******************************************************************************************/
/********************************************************************/
//char alerts_cmd[5]	=		{"drome_InEvent_Low_Engine_Oil_Pressure",
//							 "drome_InEvent_Check_ABS",
//							 "drome_InEvent_Seat_Belt",
//							 "drome_InEvent_Engine_Temp_High"
//							 "drome_InEvent_Low_Brake_Fluid"};

/* ***********************************************************************************************
* Function Name: Get_Default_Service_Slab
* Description  : This function used for Get_Default_Service_Slab value.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Get_Default_Service_Slab(void)
{
/*	uint8_t k;
	SERVICE_SLAB_START[0] = 0;
	for(k=0;k<8;k++)
	{
		SERVICE_SLAB_START[k+1] = (DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[k] + SERVICE_SLAB_END[k-1] + 1);
		SERVICE_SLAB_END[k] = DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[k] + SERVICE_SLAB_END[k-1];
	}
*/
	uint8_t j=1,i=0;
	SERVICE_SLAB_START[0] = 0;
	SERVICE_SLAB_START[1]=(DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0]*100)+1;
	for(i=2;i<52;i++)
	{
		if(i<8)
		{
			SERVICE_SLAB_START[i] = SERVICE_SLAB_START[i-1]+(DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[j])*100;
			j++;
		}
		else
		{
			SERVICE_SLAB_START[i] = SERVICE_SLAB_START[i-1]+(DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[7])*100;
		}
	}
	SERVICE_SLAB_START[52] = (DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[8]*10000);
}
/* ***********************************************************************************************
* Function Name: Door_Open
* Description  : This function used for showing Door open warning.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Door_Open(void)
{
	/*uint8_t STS_Door_data = 0,Dooropen_active,All_doorclose;*/

	uint8_t STS_Door_bit0 = 0;
	uint8_t STS_Door_bit1 = 1;
	uint8_t STS_Door_bit2 = 2;
	uint8_t STS_Door_bit3 = 3;
	uint8_t STS_Door_bit4 = 4;
	uint8_t STS_Door_bit5 = 5;

	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(MBFM1_100_message_timeout_flag == CLEAR)
			{
				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_DoorOpen_Warn_Enable == 1))
					/* Door Open warning Functionality enable */
				{
					key = drome_getCurrentViewKey();

					STS_Door_data = (CAN_MBFM1_100[4].byte) & 0X3F;

					/* Storing indiviual bit of STS_Eng_data */
					STS_Door_bit0 = ((STS_Door_data >> STS_Door_bit0) & 1);
					STS_Door_bit1 = ((STS_Door_data >> STS_Door_bit1) & 1);
					STS_Door_bit2 = ((STS_Door_data >> STS_Door_bit2) & 1);
					STS_Door_bit3 = ((STS_Door_data >> STS_Door_bit3) & 1);
					STS_Door_bit4 = ((STS_Door_data >> STS_Door_bit4) & 1);
					STS_Door_bit5 = ((STS_Door_data >> STS_Door_bit5) & 1);

					if(prev_STS_Door_data != STS_Door_data)
					{
						//					Dooropen_popup_shown=0;
						if(Dooropen_popup_shown == 1)
						{
							start_alart_timeout_5sec=1;
						}
						Codriver_index = 0;
						Driver_index = 0;
						Rearleft_index = 0;
						RearRight_index = 0;
						Backd_index = 0;
						Dooropen_detected = 1;
					}
					prev_STS_Door_data = STS_Door_data;

#if 1
					if((STS_Door_bit0 == 0x01) || (STS_Door_bit1 == 0x01) || (STS_Door_bit2 == 0x01)
							|| (STS_Door_bit3 == 0x01) || (STS_Door_bit4 == 0x01) || (STS_Door_bit5 == 0x01))
					{
						if(((Dooropen_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(Dooropen_detected == 1)&&(skip_flag == 0))||
								((Dooropen_popup_shown == 0)&&(Dooropen_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Door_Open);
							Dooropen_popup_shown=1;
							Doorclose_popup_shown=0;
							Dooropen_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							Dooropen_active = 1;
							alerts_history[20] = 1;
							//						Doorclose_popup_shown = 0;
						}
						/* Display "Door Open" */
						//					DoorClose_ON=1;
					}
					else if((STS_Door_bit0 == 0) && (STS_Door_bit1 == 0) && (STS_Door_bit2 == 0)
							&& (STS_Door_bit3 == 0) && (STS_Door_bit4 == 0) && (STS_Door_bit5 == 0))
					{
						alerts_history[20] = 0;
						if(Dooropen_active == 1)
						{
							All_doorclose = 1;
							//						Dooropen_active = 0;
						}
						else
						{
							Dooropen_popup_shown=0;
							Dooropen_active = 0;
							All_doorclose = 0;
						}
						/*Dooropen_popup_shown=0;
					Dooropen_active = 0;*/
						/****/
						if((Doorclose_popup_shown==0)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/&&(dont_throw_doorclose_on_screen_present==0)&&(All_doorclose == 1)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))
						{
							/*(void)drome_sendEventToDrome((U32)drome_InEvent_Alldoorclose);*/
							(void)drome_sendEventToDrome((U32)drome_InEvent_Door_Open);
							Doorclose_popup_shown=1;
							All_doorclose_timeout_start = 1;
							dont_throw_alerts_alert_on_screen_present = 1;
							dont_throw_doorclose_on_screen_present = 1;
							/*Dooropen_active = 0;*/
						}
						/*if(DoorClose_ON == 1)
					{
						DoorClose_ON=2;
					}*/
						/******/
#if 0
						DoorClose_ON=1;
						if(DoorClose_1sec >= 10000)
						{
							/* Display "Door close" */
							DoorClose_1sec = 0;
							DoorClose_ON = 0;
						}
#endif
					}
#if 0
					if(prev_STS_Door_data != STS_Door_data)
					{
						Dooropen_popup_shown=0;
					}
					prev_STS_Door_data = STS_Door_data;
#endif

					/*prev_STS_Door_data = STS_Door_data;*/
#endif

#if 1
					if(((Dooropen_active == 1)||(All_doorclose == 1))/*&&(Alert_priority >= 2)*/)
					{
						/* checking back door open or not */
						if((STS_Door_bit0 == 0x01)&&(Backd_index != 4)&&(Backdoor_set != 1)&&(Back_flag != 1))
						{
							/* "Back_door = ON;" */
							(void)drome_sendEventToDrome((U32)drome_InEvent_Backdoor_dooropen);
							Back_flag = 1;
							Backd_index = 4;
						}
						else if((STS_Door_bit0 == 0x00)&&(Back_flag == 1)&&(Backdoor_set != 0))
						{
							/* Back_door = OFF */
							(void)drome_sendEventToDrome((U32)drome_InEvent_Backdoor_doorClose);
							Back_flag = 0;
							Backd_index = 0;
						}

						/* checking Hood_door open or not */
						if(STS_Door_bit1 == 0x01)
						{
							//Hood_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Hood_dooropen);
							Hood_flag = 1;
							Hood_index = 0;
						}
						else
						{
							//Hood_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Hood_doorclose);
							Hood_flag = 0;
							Hood_index = 1;
						}

						/* checking Co_Driver_door open or not */
						if((STS_Door_bit2 == 0x01)&&(Codriver_index != 4)&&(Codoor_set != 1)&&(Codriver_flag != 1))
						{
							//Co_Driver_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Codriver_dooropen);
							Codriver_flag = 1;
							Codriver_index = 4;
						}
						else if((STS_Door_bit2 == 0x00)&&(Codriver_flag == 1)&&(Codoor_set != 0))
						{
							//Co_Driver_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Codriver_doorclose);
							Codriver_flag = 0;
							Codriver_index = 0;
						}

						/* checking Driver_door open or not */
						if((STS_Door_bit3 == 0x01)&&(Driver_index != 4)&&(driverdoor_set != 1)&&(Driver_flag != 1))
						{
							//Driver_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_driver_dooropen);
							Driver_flag = 1;
							Driver_index = 4;
						}
						else if((STS_Door_bit3 == 0x00)&&(Driver_flag == 1)&&(driverdoor_set != 0))
						{
							//Driver_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_driver_doorclose);
							Driver_flag = 0;
							Driver_index = 0;
						}

						/* checking Rear_Left_door open or not */
						if((STS_Door_bit4 == 0x01)&&(Rearleft_index != 4)&&(Rearleftdoor_set != 1)&&(Rearleft_flag != 1))
						{
							//Rear_Left_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Rearleft_dooropen);
							Rearleft_flag = 1;
							Rearleft_index = 4;

						}
						else if((STS_Door_bit4 == 0x00)&&(Rearleft_flag == 1)&&(Rearleftdoor_set != 0))
						{
							//Rear_Left_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Rearleft_doorclose);
							Rearleft_flag = 0;
							Rearleft_index = 0;
						}

						/* checking Rear_Right_door open or not */
						if((STS_Door_bit5 == 0x01)&&(RearRight_index != 4)&&(RearRightdoor_set != 1)&&(RearRight_flag != 1))
						{
							//Rear_Right_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_RearRight_dooropen);
							RearRight_flag = 1;
							RearRight_index = 4;
						}
						else if((STS_Door_bit5 == 0x00)&&(RearRight_flag == 1)&&(RearRightdoor_set != 0))
						{
							//Rear_Right_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_RearRight_doorclose);
							RearRight_flag = 0;
							RearRight_index = 0;
						}
					}

					if(((Doorclose_popup_shown == 0)&&(Dooropen_popup_shown == 0))
							||((Doorclose_popup_shown == 1)||(Dooropen_popup_shown == 1)))
					{
						//Alert_priority = 3;
					}

					if((Doorclose_popup_shown == 1)&&(Dooropen_popup_skip == 1))
					{
						skip_flag = 0;
					}
#endif
				}
				else
				{	/* Door Open warning Functionality disable */
					if(Dooropen_popup_skip == 1)
					{
						skip_flag = 0;
						Dooropen_popup_skip = 0;
					}
					Dooropen_popup_shown=0;
					Dooropen_active = 0;
					alerts_history[20] = 0;
					//	DoorClose_ON=0;
				}

			}
			else
			{

				if(Dooropen_popup_skip == 1)
				{
					skip_flag = 0;
					Dooropen_popup_skip = 0;
				}
				Dooropen_popup_shown=0;
				Dooropen_active = 0;
				//Alert_priority = 3;
				alerts_history[20] = 0;
				if(key == 0x22014) /*Can missing-alert timeout*/
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
				}
				Dooropen_detected = 1;			/*Door Open alertcoming after Msg missing condition*/
				prev_STS_Door_data = 0;

			}

		}
		else
		{
			/* disable door open alert */
			if(Dooropen_popup_skip == 1)
			{
				skip_flag = 0;
				Dooropen_popup_skip = 0;
			}
			Dooropen_popup_shown=0;
			Dooropen_active = 0;
//			alerts_history[20] = 0;
//			DoorClose_ON=0;
		}

	}
	else
	{
		if(Dooropen_popup_skip == 1)
		{
			skip_flag = 0;
			Dooropen_popup_skip = 0;
		}
		Dooropen_popup_shown=0;
		Dooropen_active = 0;
		alerts_history[20] = 0;
		/* display off */
	}
}


/* ***********************************************************************************************
* Function Name: Engine_oil_pressure_low
* Description  : This function used for showing Engine oil pressure low warning.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Engine_oil_pressure_low()
{
	uint8_t ENG_oil_pressure_data = 0U;
    uint8_t STS_Eng_data          = 0U;

	uint8_t ENG_oil_pressure_bit0 = 6;

#if 1
		/* ENG oil pressure Telltale ON */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if(Low_Oil_press_TT == 0xFFF)
		{
			if(Diag_RW_OilPressure_Warn_Enable == 1U)
			/* Enable  Engine oil pressure low Warning Functionality */
			{
				if((EMS4_20_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0))
				{
					STS_Eng_data = CAN_EMS1_10[6].byte;
					STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
					if(STS_Eng_data == 2)
					{
						Engine_oil_pressure_low_Timer_ON_flag = 1;
						//		if(Engine_oil_pressure_low_Timer_count >= 70000) LOP_7Sec_cntr_done
						if(LOP_7Sec_cntr_done == 1)
						{
							/* display "Engine oil pressure low Alert ON" if t > 7sec */
							if(((Low_Engine_Oil_Pressure_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Low_Engine_Oil_Pressure_popup_shown == 0)&&(Low_Engine_Oil_Pressure_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Engine_Oil_Pressure);
								Low_Engine_Oil_Pressure_popup_shown=1;
								dont_throw_alerts_alert_on_screen_present=1;
								Low_Engine_Oil_Pressure_skip = 0;
								skip_alert_Enable = 1;
								alerts_history[1] = 1;
							}
						}
						else
						{
							/* display "Engine oil pressure low Alert OFF" */
							Low_Engine_Oil_Pressure_popup_shown=0;
							alerts_history[1] = 0;
						}
					}
					else if(STS_Eng_data != 2)
					{
						/* display "Engine oil pressure low Alert OFF" */
						if(Low_Engine_Oil_Pressure_skip == 1)
						{
							skip_flag = 0;
							Low_Engine_Oil_Pressure_skip = 0;
						}
						Engine_oil_pressure_low_Timer_count = 0;
						Engine_oil_pressure_low_Timer_ON_flag = 0;
						//					alerts_history[1] = 0;
					}
				}
				else
				{
					if(EMS4_20_message_timeout_flag == 1)
					{
						if((Low_Engine_Oil_Pressure_popup_shown == 1)&&(Alert_key == 0x22004))	/*Alert Disable*/
						{
							start_alart_timeout_5sec=0;
							Priority_flag = 0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						if(Low_Engine_Oil_Pressure_skip == 1)
						{
							skip_flag = 0;
							Low_Engine_Oil_Pressure_skip = 0;
						}
						Low_Engine_Oil_Pressure_popup_shown=0;
						alerts_history[1] = 0;
					}
				}
			}
			else
			{
				/* Disable  Engine oil pressure low Warning Functionality */
				if(Low_Engine_Oil_Pressure_skip == 1)
				{
					skip_flag = 0;
					Low_Engine_Oil_Pressure_skip = 0;
				}
				Engine_oil_pressure_low_Timer_count = 0;
				Engine_oil_pressure_low_Timer_ON_flag = 0;
				alerts_history[1] = 0;
			}
		}
		else
		{
			/* display "Engine oil pressure low Alert OFF" */
			/*For EOPL alert re-display without engine stop*/

			if((Low_Engine_Oil_Pressure_popup_shown == 1)&&(Alert_key == 0x22004))	/*Alert Disable*/
			{
				start_alart_timeout_5sec=0;
				Priority_flag = 0;
				dont_throw_alerts_alert_on_screen_present=0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
			}
			Low_Engine_Oil_Pressure_popup_shown = 0;

			if(Low_Engine_Oil_Pressure_skip == 1)
			{
				skip_flag = 0;
				Low_Engine_Oil_Pressure_skip = 0;
			}
			Engine_oil_pressure_low_Timer_count = 0;
			Engine_oil_pressure_low_Timer_ON_flag = 0;
			alerts_history[1] = 0;
		}
	}

#endif

}



/* ***********************************************************************************************
* Function Name: Check_ABS
* Description  : This function used for showing Check ABS warning.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Check_ABS(void)
{
	uint8_t STS_ABS_data = 0U;
	uint8_t STS_Eng_data = 0U;
	uint8_t STS_ABS_data_bit0 = 2;

	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if( ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1)
					&& (Diag_RW_ABS_Warn_Enable == 1)) || ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1)
							&& (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0) && (Diag_RW_ABS_Warn_Enable == 1)))
			{
				/*Check CAN Signal not missing*/
				if((ESC2_10_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0))
				{
					STS_ABS_data      = CAN_ESC2_10[4].byte;
					STS_ABS_data = ((STS_ABS_data >> STS_ABS_data_bit0) & 1);
					if((STS_ABS_data == 0U)&&(CAN_ESC2_10[4].bits.Rx_4 == 1))//ABS == 0 && EBD == 1
					{
						STS_Eng_data = CAN_EMS1_10[6].byte;
						STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
						if((STS_Eng_data == 2) && (Enginerun_chm ==1))
						{
							/* display "ABS Alert ON" if t>5sec */
							Check_ABS_Timer_ON_flag = 1;
							//						if(Check_ABS_Timer_count >= 50000)
							if(IGN_timer_5s == 1)
							{
								/* display "ABS Alert ON" if t>5sec */
								if(((Check_ABS_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
										/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Check_ABS_popup_shown == 0)&&(Check_ABS_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									Priority_flag = 1;
									Notification_chime = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Check_ABS);
									Check_ABS_popup_shown=1;
									Check_ABS_popup_skip = 0;
									dont_throw_alerts_alert_on_screen_present=1;
									skip_alert_Enable = 1;
									alerts_history[24] = 1;
								}
							}
							else
							{
								/* display "ABS Alert OFF" if t<5sec */
								Check_ABS_popup_shown=0;
								alerts_history[24] = 0;
							}
						}
						else if(STS_Eng_data != 2 || (Enginerun_chm == 0))
						{
							//Check_ABS_alert = OFF
//							Check_ABS_Timer_count = 0;
//							IGN_start_time = 0;
							Check_ABS_popup_shown=0;
							Check_ABS_Timer_ON_flag = 0; /* clear ABS timer flag */
							if(Check_ABS_popup_skip == 1)
							{
								skip_flag = 0;
								Check_ABS_popup_skip = 0;
							}
						}

					}
					else
					{
						if((Check_ABS_popup_shown == 1)&&(Alert_key == 0x22002))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						//Check_ABS_alert = OFF
//						Check_ABS_Timer_count = 0;
//						IGN_start_time = 0;
						Check_ABS_popup_shown=0;
						Check_ABS_Timer_ON_flag = 0; /* clear ABS timer flag */
						alerts_history[24] = 0;
						if(Check_ABS_popup_skip == 1)
						{
							skip_flag = 0;
							Check_ABS_popup_skip = 0;
						}
					}

				}
				else
				{
					//Check_ABS_alert = OFF
					//Check_ABS_Timer_count = 0;
					//IGN_start_time = 0;
					Check_ABS_popup_shown=0;
					Check_ABS_Timer_ON_flag = 0; /* clear ABS timer flag */
					alerts_history[24] = 0;
					if(Check_ABS_popup_skip == 1)
					{
						skip_flag = 0;
						Check_ABS_popup_skip = 0;
					}
				}
			}
		}
		else
		{
			/* disable Engine oil pressure low alert */
			//Check_ABS_alert = OFF
//			Check_ABS_Timer_count = 0;
//			IGN_start_time = 0;
			Check_ABS_Timer_ON_flag = 0; /* clear ABS timer flag */
			if(Check_ABS_popup_skip == 1)
			{
				skip_flag = 0;
				Check_ABS_popup_skip = 0;
			}
		}
	}
	else
	{
		/* display off */
		//Check_ABS_alert = OFF
//		Check_ABS_Timer_count = 0;
		IGN_start_time = 0;
		Check_ABS_Timer_ON_flag = 0; /* clear ABS timer flag */
		if(Check_ABS_popup_skip == 1)
		{
			skip_flag = 0;
			Check_ABS_popup_skip = 0;
		}
	}
}



/* ***********************************************************************************************
* Function Name: Seat_belt
* Description  : This function used for showing Seatbelt alert.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Seat_belt(void)
{
	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if((VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM == 1)
			   && (Diag_RW_SeatBelt_Warn_Enable == 1))
			{
				/* enable seat belt alert functionality */
				if(SBR_Alert_check == 1)//flag set only after IGN
				{
					if(SRS1_20_message_timeout_flag == 0)
					{
						if(((CAN_SRS1_20[3].bits.Rx_1 == 0) && (CAN_SRS1_20[3].bits.Rx_2==0)) ||( (CAN_SRS1_20[3].bits.Rx_3 == 0) && (CAN_SRS1_20[3].bits.Rx_4==0)))
						{
							Ign_on_SBR_alert_trigger = 1;
							SBR_Alert_check = 0;//will enter in this loop only after IGN on(not STS_ENG=2)
							SBR_IGN_ON = 1;
						}
						else
						{
							Ign_on_SBR_alert_trigger = 0;
							SBR_Alert_check = 0;
							SBR_IGN_ON = 0;
						}
					}
				}

				if((Passenger_Seat_belt_alert_flag_ON==1)||(Ign_on_SBR_alert_trigger == 1))
				{
					/* not buckled */
					if((Seat_Belt_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)/*&&(Alert_priority >= 1)*/)
					{
						Priority_flag = 1;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Seat_Belt);
						Seat_Belt_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
						alerts_history[0] = 1;
						Ign_on_SBR_alert_trigger = 0;
					}
					else
					{
						//Alert_priority = 2;
//						Seat_Belt_popup_shown=0;
					}
					//Seat_belt_alert = ON
				}
				else
				{
					if((Seat_Belt_popup_shown == 1)&&(Alert_key == 0x22001))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					Seat_Belt_popup_shown=0;
					if(SBR_IGN_ON == 0)
					{
						alerts_history[0] = 0;
					}
					//Alert_priority = 2;
					/* buckled */
					//Seat_belt_alert = OFF
				}

			}
			else
			{
				/* disable seat belt alert functionality */
				//Seat_belt_alert = OFF;
				alerts_history[0] = 0;
				//Alert_priority = 2;
			}

		}
		else
		{
			/* disable Seat belt alert */
			//Seat_belt_alert = OFF;
			alerts_history[0] = 0;
			//Alert_priority = 1;
			SoundPCM_Seatbelt_Lv0cont_STOP();
			cycle133_completed = 0;
			cycle133_cntr = 0;
			Seat_Belt_popup_shown = 0;
			//Sound_Bit = 0x0;
		}
	}
	else
	{
		/* display off */
		//Seat_belt_alert = OFF;
		alerts_history[0] = 0;
		//Alert_priority = 1;
	}

}



/* ***********************************************************************************************
* Function Name: High_Temp
* Description  : This function used for showing High temperature alert.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void High_Temp(void)
{
	uint8_t ENG_coolant_temp_data = 0U;
	uint8_t STS_Eng_data          = 0U;

	uint8_t ENG_coolant_temp_data_bit0 = 6U;

	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (EMS1_10_message_timeout_flag == 0)
			&& (Diag_RW_EngineTempHigh_Warn_Enable ==1))   /* disable High temp alert */
			{
				ENG_coolant_temp_data      = CAN_IC1_100[5].byte;
				ENG_coolant_temp_data_bit0 = ((ENG_coolant_temp_data >> ENG_coolant_temp_data_bit0) & 1);
				if(ENG_coolant_temp_data_bit0 == 1)
				{

					STS_Eng_data = CAN_EMS1_10[6].byte; /* stored 6th byte of EMS1_10 */
					STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5); /* using Higher nibble 1st 3 bit */
					if(STS_Eng_data == 2)
					{
						eng_start_timer_on = 1;				//5 sec timer
						/* Check ABS Alert ON */
//						High_Temp_Timer_ON_flag = 1;
//						if(High_Temp_Timer_count >= 50000)
						if(eng_start_flag == 1)
						{
							/* High_temp Alert ON */
							if(((Engine_Temp_High_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Engine_Temp_High_popup_shown == 0)&&(Engine_Temp_High_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Engine_Temp_High);
								Engine_Temp_High_popup_shown=1;
								dont_throw_alerts_alert_on_screen_present=1;
								skip_alert_Enable = 1;
								Engine_Temp_High_skip = 0;
								alerts_history[2] = 1;
							}

						}
						else
						{
							/* High_temp Alert OFF */
							Engine_Temp_High_popup_shown=0;

						}
					}
					else if(STS_Eng_data != 2)
					{
						//High_Temp_alert = OFF
//						High_Temp_Timer_count = 0;
//						IGN_start_time = 0;
						eng_start_timer_on = 0;				//5 sec timer
						High_Temp_Timer_ON_flag = 0;
						Engine_Temp_High_popup_shown=0;
						if(Engine_Temp_High_skip == 1)
						{
							Engine_Temp_High_skip = 0;
							skip_flag = 0;
						}
					}
				}
				else
				{
					if((Engine_Temp_High_popup_shown == 1)&&(Alert_key == 0x22005))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					//High_Temp_alert = OFF
					//High_Temp_Timer_count = 0;
					//IGN_start_time = 0;
					Engine_Temp_High_popup_shown = 0;
					High_Temp_Timer_ON_flag = 0;
					alerts_history[2] = 0;
					if(Engine_Temp_High_skip == 1)
					{
						Engine_Temp_High_skip = 0;
						skip_flag = 0;
					}

				}


			}
			else
			{
				/* disable High temp alert */
				//High_Temp_alert = OFF
//				High_Temp_Timer_count = 0;
				IGN_start_time = 0;
				High_Temp_Timer_ON_flag = 0;
				alerts_history[2] = 0;
				if(Engine_Temp_High_skip == 1)
				{
					Engine_Temp_High_skip = 0;
					skip_flag = 0;
				}
			}

		}
		else
		{
			/* disable High temp alert */
			//High_Temp_alert = OFF
//			High_Temp_Timer_count = 0;
//			IGN_start_time = 0;
			High_Temp_Timer_ON_flag = 0;
//			alerts_history[2] = 0;
			if(Engine_Temp_High_skip == 1)
			{
				Engine_Temp_High_skip = 0;
				skip_flag = 0;
			}
		}
	}
	else
	{
		/* display off */
		//High_Temp_alert = OFF
//		High_Temp_Timer_count = 0;
		IGN_start_time = 0;
		High_Temp_Timer_ON_flag = 0;
		alerts_history[2] = 0;
		if(Engine_Temp_High_skip == 1)
		{
			Engine_Temp_High_skip = 0;
			skip_flag = 0;
		}
	}
}



/* ***********************************************************************************************
* Function Name: Low_Brake_Fluid
* Description  : This function used for showing Low_Brake_Fluid alert.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Low_Brake_Fluid(void)
{
	uint8_t STS_Eng_data = 0U;

	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(Diag_RW_LowBrakeFluid_Warn_Enable == 1) /* Enable */
			{
				if(LowBrakeFluidSt_Hw_Input == 1)
				{
					STS_Eng_data = CAN_EMS1_10[6].byte;
					STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
					if(STS_Eng_data == 2)
					{
						Low_Brake_Fluid_Timer_ON_flag = 1;
				//		if(Low_Brake_Fluid_Timer_count > 30000)
						if(LBF_3Sec_cntr_done == 1)
						{
							/* display Low_Brake_Fluid Alert ON */
							if(((Low_Brake_Fluid_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Low_Brake_Fluid_popup_shown == 0)&&(Low_Brake_Fluid_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Brake_Fluid);
								Low_Brake_Fluid_popup_shown=1;
								dont_throw_alerts_alert_on_screen_present=1;
								Low_Brake_Fluid_skip = 0;
								skip_alert_Enable = 1;
								alerts_history[3] = 1;
							}
						}
						else
						{
							/* display Low_Brake_Fluid Alert OFF */
							Low_Brake_Fluid_popup_shown=0;
						}
					}
					else if(STS_Eng_data != 2)
					{
						Low_Brake_Fluid_Timer_count = 0;
						Low_Brake_Fluid_Timer_ON_flag = 0;
						if(Low_Brake_Fluid_skip == 1)
						{
							skip_flag = 0;
							Low_Brake_Fluid_skip = 0;
						}
					}

				}
				else if(LowBrakeFluidSt_Hw_Input == 0)
				{
					if((Low_Brake_Fluid_popup_shown == 1)&&(Alert_key == 0x22003))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					Low_Brake_Fluid_popup_shown = 0;	/*Added*/
					Low_Brake_Fluid_Timer_count = 0;
					Low_Brake_Fluid_Timer_ON_flag = 0;
					alerts_history[3] = 0;
					if(Low_Brake_Fluid_skip == 1)
					{
						skip_flag = 0;
						Low_Brake_Fluid_skip = 0;
					}
				}

			}
			else
			{
				/* Disable */

				Low_Brake_Fluid_Timer_count = 0;
				Low_Brake_Fluid_Timer_ON_flag = 0;
				alerts_history[3] = 0;
				if(Low_Brake_Fluid_skip == 1)
				{
					skip_flag = 0;
					Low_Brake_Fluid_skip = 0;
				}
			}

		}
		else
		{
			/* disable Low_Brake_Fluid alert */

			Low_Brake_Fluid_Timer_count = 0;
			Low_Brake_Fluid_Timer_ON_flag = 0;
//			alerts_history[3] = 0;
			if(Low_Brake_Fluid_skip == 1)
			{
				skip_flag = 0;
				Low_Brake_Fluid_skip = 0;
			}
		}

	}
	else
	{
		/* display off */
		//Low_Brake_Fluid_alert OFF
		Low_Brake_Fluid_Timer_count = 0;
		Low_Brake_Fluid_Timer_ON_flag = 0;
		alerts_history[3] = 0;
		if(Low_Brake_Fluid_skip == 1)
		{
			skip_flag = 0;
			Low_Brake_Fluid_skip = 0;
		}
	}

}



/* ***********************************************************************************************
* Function Name: Fuel_Low
* Description  : This function used for showing Fuel Low alert.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Fuel_Low(void)
{
	uint8_t STS_Eng_data = 0U;

#if 1
		/* After checking Low fuel Telltale ON condition */
		if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (Diag_RW_LowFuel_Warn_Enable == 1) )
		{
			if(EMS1_10_message_timeout_flag == 0)
			{
				STS_Eng_data = CAN_EMS1_10[6].byte;
				STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
				if((fuel_alert_on_flag == 1) && (STS_Eng_data == 2))
				{
					/* Check Fuel Low Alert ON */
					eng_start_timer_on = 1;				//5 sec timer

					/* display Fuel Low Alert ON */
						if(logo_completed==1)
						{
							if(eng_start_flag == 1)
							{
								if(((Low_Fuel_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)/*&&(Alert_priority >= 3)*/
										&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||((Low_Fuel_popup_shown == 0)&&(Low_Fuel_popup_skip == 1)))
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								{
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Fuel);
									Low_Fuel_popup_shown=1;
									Low_Fuel_popup_skip = 0;
									dont_throw_alerts_alert_on_screen_present=1;
									skip_alert_Enable = 1;
									alerts_history[22] = 1;
								}
							}
						}

				}
				else if((STS_Eng_data != 2) || (fuel_alert_on_flag == 0))
				{
					/* display Fuel Low Alert OFF */
//					IGN_start_time = 0;
					/*eng_start_timer_on = 0;*///not required it will become 0 at other location if STS_Eng !=2
					Low_Fuel_popup_shown = 0;
					Fuel_Low_Timer_ON_flag = 0;
					alerts_history[22] = 0;
					cycle_lowfuel_completed = 0;//to play chime after change in STS_ENG
					Low_fuel_ongoing = 0;//r
					if(Low_Fuel_popup_skip == 1)
					{
						skip_flag = 0;
						Low_Fuel_popup_skip = 0;
					}
				}
			}
			else
			{
				Fuel_Low_Timer_ON_flag = 0;
				Low_Fuel_popup_shown = 0;
				alerts_history[22] = 0;
				if(Low_Fuel_popup_skip == 1)
				{
					skip_flag = 0;
					Low_Fuel_popup_skip = 0;
				}
			}

		}
		else
		{

			Fuel_Low_Timer_ON_flag = 0;
			Low_Fuel_popup_shown = 0;
			alerts_history[22] = 0;
			if(Low_Fuel_popup_skip == 1)
			{
				skip_flag = 0;
				Low_Fuel_popup_skip = 0;
			}
//			IGN_start_time = 0;
//			Fuel_Low_Timer_count = 0;
		}


#endif

#if 0
	/* checking ignition ON or OFF */
	if(IGN == ON)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if((VAR_coding_engine_management_system_flag == 1) && (Diag_RW_LowFuel_Warn_Enable_flag == 1))
			{
				/* checking Low fuel Telltale condition */
				if(Low_fuel_Telltale_Condition == ON)
				{
					STS_Eng_data = CAN_EMS1_10[6];
					STS_Eng_data = (STS_Eng_data && 0x07);
					if(STS_Eng_data == 2)
					{
						/* Check Fuel Low Alert ON */
						Fuel_Low_Timer_ON_flag = SET;
					}
					else if(STS_Eng_data != 2)
					{
						Fuel_Low_alert = OFF;
					}

				}
				else
				{
					Fuel_Low_alert = OFF;
				}

			}
			else
			{
				Fuel_Low_alert = OFF;
			}

		}
		else
		{
			/* disable Fuel_Low alert */
			Fuel_Low_alert = OFF;
		}
	}
	else
	{
		/* display off */
		Fuel_Low_alert = OFF;
	}
#endif

}



/* ***********************************************************************************************
* Function Name: DEF_Level
* Description  : This function used for showing DEF Level.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void DEF_Level(void)
{
	uint8_t DEF_LEVEL;
	uint8_t INCORRECT_DEF;
	uint8_t DEF_DOSING_MALF_UNCTION;
	uint16_t DIST_DEF_EMPTY = 0x00;

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/* checking DEF Warning Functionality */
			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_DEF_TT_CONFIG == 1))
			{
				INCORRECT_DEF = CAN_EMS29_100[3].byte;
				INCORRECT_DEF = ((INCORRECT_DEF & 0x38) >> 3); /* for getting 27,28 & 29th bit */

				DIST_DEF_EMPTY = DIST_DEF_EMPTY + CAN_EMS29_100[1].byte;
				DIST_DEF_EMPTY = (DIST_DEF_EMPTY << 8);
				DIST_DEF_EMPTY = DIST_DEF_EMPTY + CAN_EMS29_100[2].byte;

				if( (INCORRECT_DEF == 0U) || (INCORRECT_DEF == 5) ||
				    (INCORRECT_DEF == 6) || (INCORRECT_DEF == 7) ) /* 5=0x14, 6=0x18, 7=0x1C */
				{
					DEF_DOSING_MALF_UNCTION = CAN_EMS29_100[3].byte;
					DEF_DOSING_MALF_UNCTION = (DEF_DOSING_MALF_UNCTION & 0x07); /* for getting 24,25 & 26th bit */

					if( (DEF_DOSING_MALF_UNCTION == 0U) || (DEF_DOSING_MALF_UNCTION == 5)
					    || (DEF_DOSING_MALF_UNCTION == 7) || (DEF_DOSING_MALF_UNCTION == 6) ) /* 5=0xA0, 6=0xC0, 7=0xE0 */
					{
						DEF_LEVEL = CAN_EMS29_100[4].byte;
						DEF_LEVEL = (DEF_LEVEL & 0x07); /* for getting 32,33 & 34th bit */

						if( (DEF_LEVEL == 0U) || (DEF_LEVEL == 5) || (DEF_LEVEL == 6)
							|| (DEF_LEVEL == 7) )          /* 5=0xA0, 6=0xC0, 7=0xE0 */
						{
							/* No display */

						}
						else if(DEF_LEVEL == 1) /* 1 =0x20 */
						{
							/* DEF LEVEL LOW (5 secs) */
						}
						else if( (DEF_LEVEL == 2) && (DIST_DEF_EMPTY != 0xFFFF)) /* 2= 0x40 */
						{
							/* DEF LEVEL LOW. NO START 800KM */
						}
						else if( (DEF_LEVEL == 2) && (DIST_DEF_EMPTY == 0xFFFF)) /* 2= 0x40 */
						{
							/* DEF LEVEL LOW NO START ---KM */
						}
						else if(DEF_LEVEL == 3) /* 3= 0x60 */
						{
							/* RESTART DENIED FILL DEF */
						}
						else if(DEF_LEVEL == 4) /* 4= 0x80 */
						{
							/* START DENIED Fill DEF */
						}
						else
						{
							/* nothing */
						}

					}
					else if(DEF_DOSING_MALF_UNCTION == 1) /* 1= 0x20 */
					{
						/* DEF DOSING MALFUNCTIONING (5 secs) */
					}
					else if( (DEF_DOSING_MALF_UNCTION == 2) && (DIST_DEF_EMPTY != 0xFFFF)) /* 2= 0x40 */
					{
						/* DEF ERROR NO START 800KM */
					}
					else if( (DEF_DOSING_MALF_UNCTION == 2) && (DIST_DEF_EMPTY == 0xFFFF)) /* 2= 0x40 */
					{
						/* DEF ERROR NO START ---KM */
					}
					else if(DEF_DOSING_MALF_UNCTION == 3) /* 3= 0x60 */
					{
						/* RESTART DENIED DOSE MALFUNCTION */
					}
					else if(DEF_DOSING_MALF_UNCTION == 4) /*4= 0x40 */
					{
						/* START DENIED DOSE MALFUNCITON */
					}
					else
					{
						/* nothing */
					}


				}
				else if(INCORRECT_DEF == 1) /* 1= 0x04  */
				{
					/* INCORRECT DEF DETECTED (5 secs) */
				}
				else if( (INCORRECT_DEF == 2) && (DIST_DEF_EMPTY != 0xFFFF)) /* 2= 0x08  */
				{
					/* INCORRECT DEF NO START 800KM */
				}
				else if( (INCORRECT_DEF == 2) && (DIST_DEF_EMPTY == 0xFFFF)) /* 2= 0x08  */ /* invalid: 0xFFFF */
				{
					/* INCORRECT DEF. NO START ---KM */
				}
				else if(INCORRECT_DEF == 3) /* 3= 0x0C  */
				{
					/* RESTART DENIED CHECK DEF */
				}
				else if(INCORRECT_DEF == 4) /* 4= 0x10  */
				{
					/* START DENIED CHECK DEF */
				}
				else
				{
					/* nothing */
				}

			}
			else
			{
				/* disable DEF level */

			}
		}
		else
		{
			/* DEF level warning condition Not available */

		}
	}
	else
	{
		/* display off */
	}

}

/* ***********************************************************************************************
* Function Name: DPF_Filter_Clogged
* Description  : This function used for showing DPF Clogged Alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void DPF_Filter_Clogged(void)
{
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if(EMS4_20_message_timeout_flag == 0)
				{
		if(dpf_alert_flag==1 && Diag_RW_DPF_Clogged_warn_Enable==1)
		{
//			DPF_Clogged_Timer_ON_flag = 1;
//			if(DPF_Clogged_Timer_count >= 50000)		immediate display after input
			{
				/* DPF Filter Clogged Alert ON */
				if(((DPF_Clogged_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
						/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
						((DPF_Clogged_popup_shown == 0)&&(DPF_Clogged_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
				{
					Priority_flag = 1;
					(void)drome_sendEventToDrome((U32)drome_InEvent_DPF_Clogged);
					DPF_Clogged_popup_shown=1;
//					dpf_alert_flag=0;
					DPF_Clogged_popup_skip = 0;
					dont_throw_alerts_alert_on_screen_present=1;
					skip_alert_Enable = 1;
					alerts_history[6] = 1;
					dpf_check = 1;
				}
			}
		}
		else
		{
			/*  DPF Filter Clogged Alert OFF */
			if((DPF_Clogged_popup_shown == 1)&&(Alert_key == 0x22053))
			{
				Priority_flag = 0;
				start_alart_timeout_5sec=0;
				dont_throw_alerts_alert_on_screen_present=0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
			}
			dpf_check = 1;
			DPF_Clogged_popup_shown=0;
			alerts_history[6] = 0;
			if(DPF_Clogged_popup_skip == 1)
			{
				skip_flag = 0;
				DPF_Clogged_popup_skip = 0;
			}
		}
	}
		else
		{
			if(DPF_Clogged_popup_skip == 1)
			{
				skip_flag = 0;
				DPF_Clogged_popup_skip = 0;
			}
			DPF_Clogged_popup_shown=0;
			alerts_history[6] = 0;
		}
	}
	else
	{
		if(DPF_Clogged_popup_skip == 1)
		{
			skip_flag = 0;
			DPF_Clogged_popup_skip = 0;
		}
	}

}

/* ***********************************************************************************************
* Function Name: Park_Break_alert
* Description  : This function used for showing Park_Break_alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Park_Break_alert(void)
{
	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(Diag_RW_ParkBrake_Warn_Enable == 1) /* Enable */
			{
				if((PARK_BRAKE_CPU== 0) && (vehicle_moving_state == 1)&&(ALERT_CHECK==1))
				{
					/* display Park_Break Alert ON */
					if(((Park_brake_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
							/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
							((Park_brake_popup_shown == 0)&&(Park_brake_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
					{
						Priority_flag = 1;
						//Insert Event for Park Brake
						(void)drome_sendEventToDrome((U32)drome_InEvent_parkbrake);
						Park_brake_popup_shown=1;
						Park_brake_popup_skip = 0;
						dont_throw_alerts_alert_on_screen_present=1;
						skip_alert_Enable = 1;
						alerts_history[4] = 1;
					}
				}
				else
				{
					if((Park_brake_popup_shown == 1)&&(Alert_key == 0x22009))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					/* display Low_Brake_Fluid Alert OFF */
					Park_brake_popup_shown=0;
					alerts_history[4] = 0;
					if(Park_brake_popup_skip == 1)
					{
						skip_flag = 0;
						Park_brake_popup_skip = 0;

					}
				}
			}
			else
			{
				if(Park_brake_popup_skip == 1)
				{
					skip_flag = 0;
					Park_brake_popup_skip = 0;

				}
			}
		}
		else
		{
			if(Park_brake_popup_skip == 1)
			{
				skip_flag = 0;
				Park_brake_popup_skip = 0;

			}
		}
	}
}

/* ***********************************************************************************************
* Function Name: Water_In_Fuel_alert
* Description  : This function used for showing Water_In_Fuel_alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Water_In_Fuel_alert(void)
{
	uint8_t STS_Eng_data = 0U;
	if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1) && (Diag_RW_WIF_Warn_Enable == 1) && (FEATURE_CODE_FUEL_TYPE==1))
	{
		STS_Eng_data = CAN_EMS1_10[6].byte;
		STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
		if((CAN_EMS1_10[5].bits.Rx_5 == 1)&&(Water_in_fuel_TT == 1)&&(STS_Eng_data == 2))
		{
			Water_In_Fuel_Timer_ON = 1;
			if(WIF_5sec_done == 1)
			{
				/* display Low_Brake_Fluid Alert ON */
				if(((Water_In_Fuel_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
						/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
						((Water_In_Fuel_popup_shown == 0)&&(Water_In_Fuel_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
				{
					//Insert Event for Park Brake
					Priority_flag = 1;
					(void)drome_sendEventToDrome((U32)drome_InEvent_Water_in_Fuel);
					Water_In_Fuel_popup_shown=1;
					Water_In_Fuel_popup_skip = 0;
					dont_throw_alerts_alert_on_screen_present=1;
					skip_alert_Enable = 1;
					alerts_history[28] = 1;
				}
			}
			else
			{
				/* display Low_Brake_Fluid Alert OFF */
				Water_In_Fuel_popup_shown=0;
				alerts_history[28] = 0;
			}
		}
		else
		{
			if(STS_Eng_data != 2)
			{
				alerts_history[28] = 0;
				Water_In_Fuel_Timer_ON = 0;
				Water_In_Fuel_popup_shown = 0;
				WIF_5sec_done = 0;
				if(Water_In_Fuel_popup_skip == 1)
				{
					Water_In_Fuel_popup_skip = 0;
					skip_flag = 0;
				}
			}
			if((Water_In_Fuel_popup_shown == 1)&&(Alert_key == 0x22007))
			{
				Priority_flag = 0;
				start_alart_timeout_5sec=0;
				dont_throw_alerts_alert_on_screen_present=0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
			}

			alerts_history[28] = 0;
			Water_In_Fuel_Timer_ON = 0;
			Water_In_Fuel_popup_shown = 0;//r
			//WIF_5sec_done = 0;//r commented to sync alert with chime(To solve alert 5 sec delay issue)
			if(Water_In_Fuel_popup_skip == 1)
			{
				Water_In_Fuel_popup_skip = 0;
				skip_flag = 0;
			}
		}
	}
//	else
//	{
//		Water_In_Fuel_popup_shown = 0;
//	}
}

/* ***********************************************************************************************
* Function Name: Overspeed_alert
* Description  : This function used for showing Water_In_Fuel_alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Overspeed_alert(void)
{
	down_scale.w=0;
	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(FEATURE_CODE_SPEED_INPUT==1)
			{
				down_scale.b[0] = CAN_ESC2_10[0].byte;
				down_scale.b[1] = CAN_ESC2_10[1].byte;
			}
		else if(FEATURE_CODE_SPEED_INPUT==0)
			{
				down_scale.b[0] = CAN_ESC12_10[2].byte;
				down_scale.b[1] = CAN_ESC12_10[3].byte;
			}

			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (Diag_RW_OverSpeed_Warn_Enable == 1)) /* Enable */
			{
				if(Speed_Missing == 0)//CAN msg not missing
				{
					if(Over_speed_flag == 1)
					{
						/* display Over_speed Alert ON */
						if((Overspeed_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
						{
							//Insert Event for Over speed warning
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Overspeed);
							Overspeed_popup_shown=1;//1
							dont_throw_alerts_alert_on_screen_present=1;
						}

					}
					else if((Over_speed_flag == 0)&&((display_speedometer_speed < OverSpeedLevel2Val)||(down_scale.w < 0x4B01)))
					//else if((Over_speed_flag == 0)&&((display_speedometer_speed < (OverSpeedLevel2Val-OverSpeedHystresis))||(down_scale.w < 0x4B01)))
					{
						if((Overspeed_popup_shown == 1)&&(Alert_key == 0x22008))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						//Overspeed warning OFF
						Overspeed_popup_shown=0;
					}
				}
				else
				{
					if((Overspeed_popup_shown == 1)&&(Alert_key == 0x22008))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					//Overspeed warning OFF
					Overspeed_popup_shown=0;
				}
			}
			else
				{
					//Overspeed warning OFF
					Overspeed_popup_shown=0;
				}
		}
		else
			{
				//Overspeed warning OFF
				Overspeed_popup_shown=0;
			}
	}
	else
	{
		//Overspeed warning OFF
		Overspeed_popup_shown=0;
	}
}

/* ***********************************************************************************************
* Function Name: Check_EBD_alert
* Description  : This function used for showing Check_EBD_alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Check_EBD_alert(void)
{
	uint8_t STS_Eng_data = 0U;
	STS_Eng_data = CAN_EMS1_10[6].byte;
	STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check EBD warning Functionality*/
			if( ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1)
			&& (Diag_RW_EBD_Warn_Enable == 1)) || ((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1)
			&& (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0) && (Diag_RW_EBD_Warn_Enable == 1)))
			{
				/*Check CAN Signal not missing*/
				if((ESC2_10_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0))
				{
					if(STS_Eng_data == 2)//if(CAN_ESC2_10[4].bits.Rx_4 == 0)
					{
						eng_start_timer_on = 1;				//5 sec timer
						if(CAN_ESC2_10[4].bits.Rx_4 == 0)//if(STS_Eng_data == 2)
						{
//							EBD_alert_timer_on = 1;				//5 sec timer
							if(eng_start_flag == 1)
							{
								/* EBD Alert ON */
								if(((EBD_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
										/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
										((EBD_popup_shown == 0)&&(EBD_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									//Insert Event for EBD Alert warning
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Check_EBD);
									EBD_popup_shown=1;
									Notification_chime = 1;
									EBD_popup_skip = 0;
									dont_throw_alerts_alert_on_screen_present=1;
									skip_alert_Enable = 1;
									alerts_history[23] = 1;
								}

							}
						}
						else
						{
							if((EBD_popup_shown == 1)&&(Alert_key == 0x22011))
							{
								Priority_flag = 0;

								start_alart_timeout_5sec=0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}

							//EBD_alert_timer_on = 0;
							EBD_popup_shown = 0;
							alerts_history[23] = 0;
							if(EBD_popup_skip == 1)
							{
								skip_flag = 0;
								EBD_popup_skip = 0;
							}

						}
					}
					/*Redisplay after change in ESC2_10 bit*/
					else
					{
						//EBD_alert_timer_on = 0;
						eng_start_timer_on = 0;				//5 sec timer reset
						EBD_popup_shown = 0;
						alerts_history[23] = 0;
						if(EBD_popup_skip == 1)
						{
							skip_flag = 0;
							EBD_popup_skip = 0;
						}
					}

				}
				else
				{
					if((EBD_popup_shown == 1)&&(Alert_key == 0x22011))
					{
						Priority_flag = 0;

						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					EBD_popup_shown = 0;
					alerts_history[23] = 0;
					if(EBD_popup_skip == 1)
					{
						skip_flag = 0;
						EBD_popup_skip = 0;
					}
				}
			}
			else
			{
				EBD_popup_shown = 0;
				alerts_history[23] = 0;
				if(EBD_popup_skip == 1)
				{
					skip_flag = 0;
					EBD_popup_skip = 0;
				}
			}

		}
		else
		{
			EBD_popup_shown = 0;
			alerts_history[23] = 0;
			if(EBD_popup_skip == 1)
			{
				skip_flag = 0;
				EBD_popup_skip = 0;
			}
		}
	}
	else
	{
		eng_start_timer_on = 0;
		alerts_history[23] = 0;
		if(EBD_popup_skip == 1)
		{
			skip_flag = 0;
			EBD_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: Check_ESP_alert
* Description  : This function used for showing Check_ESP_alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Check_ESP_alert(void)
{
	uint8_t STS_Eng_data = 0U;
	STS_Eng_data = CAN_EMS1_10[6].byte;
	STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);
	/* checking ignition ON or OFF */
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check ESC warning Functionality*/
			if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1) && (VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0) && (Diag_RW_EBD_Warn_Enable == 1))//Table 2
			{
				/*Check CAN Signal not missing*/
				if((ESC5_10_message_timeout_flag == 0) && (ESC2_10_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0))
				{
					if(STS_Eng_data == 2)//if(CAN_ESC2_10[4].bits.Rx_4 == 0)
					{
						eng_start_timer_on = 1;				//5 sec timer
						//ESC == 0 && ABS == 1 && EBD == 1
						if((CAN_ESC5_10[3].byte == 0)&&(CAN_ESC2_10[4].bits.Rx_3 == 1)&&(CAN_ESC2_10[4].bits.Rx_4 == 1))//to solve ESC Alert get active even if sts_ESC == 2
//						if((CAN_ESC5_10[3].bits.Rx_7 == 0)&&(CAN_ESC2_10[4].bits.Rx_3 == 1)&&(CAN_ESC2_10[4].bits.Rx_4 == 1))
						{
							if(eng_start_flag == 1)
							{
								/* EBD Alert ON */
								if(((ESP_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
										/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
										((ESP_popup_shown == 0)&&(ESP_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									//Insert Event for ESC Alert warning
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Check_ESP);
									Notification_chime = 1;
									ESP_popup_shown=1;
									ESP_popup_skip = 0;
									dont_throw_alerts_alert_on_screen_present=1;
									skip_alert_Enable = 1;
									alerts_history[25] = 1;
								}

							}
						}
						else
						{
							if((ESP_popup_shown == 1)&&(Alert_key == 0x22015))
							{
								Priority_flag = 0;
								start_alart_timeout_5sec=0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}

							alerts_history[25] = 0;
							ESP_popup_shown = 0;
							if(ESP_popup_skip == 1)
							{
								skip_flag = 0;
								ESP_popup_skip = 0;
							}
						}
					}
					else
					{
						alerts_history[25] = 0;
						ESP_popup_shown = 0;
						eng_start_timer_on = 0;
						if(ESP_popup_skip == 1)
						{
							skip_flag = 0;
							ESP_popup_skip = 0;
						}
					}
				}
				else
				{
					if((ESP_popup_shown == 1)&&(Alert_key == 0x22015))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					alerts_history[25] = 0;
					ESP_popup_shown = 0;
					if(ESP_popup_skip == 1)
					{
						skip_flag = 0;
						ESP_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[25] = 0;
				ESP_popup_shown = 0;
				if(ESP_popup_skip == 1)
				{
					skip_flag = 0;
					ESP_popup_skip = 0;
				}
			}
		}
		else
		{
			alerts_history[25] = 0;
			ESP_popup_shown = 0;
			if(ESP_popup_skip == 1)
			{
				skip_flag = 0;
				ESP_popup_skip = 0;
			}
		}
	}
	else
	{
		alerts_history[25] = 0;
		ESP_popup_shown = 0;
		if(ESP_popup_skip == 1)
		{
			skip_flag = 0;
			ESP_popup_skip = 0;
		}
	}
}

/* ***********************************************************************************************
* Function Name: RKE_Low_Battery_alert	//RKE_Battery
* Description  : This function used for showing RKE_Low_Battery_alert	//RKE_Battery
* Arguments    : None
* Return Value : None
************************************************************************************************ */

void RKE_Low_Battery_alert(void)//RKE_Battery
{
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check Low_Battery warning Functionality*/
			if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_RKE_Low_battery_warn_Enable == 1))
			{
				/*Check CAN Signal not missing*/
				if(MBFM1_100_message_timeout_flag == 0)
				{
					if(CAN_MBFM1_100[4].bits.Rx_7 == 1)
					{
						/* Low_Batt. Alert ON */
						if(((Low_Battery_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((Low_Battery_popup_shown == 0)&&(Low_Battery_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Battery);
							Low_Battery_popup_shown=1;
							Low_Battery_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							alerts_history[27] = 1;
						}
					}
					else
					{
						if((Low_Battery_popup_shown == 1)&&(Alert_key == 0x22010))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						Low_Battery_popup_shown = 0;
						alerts_history[27] = 0;
						if(Low_Battery_popup_skip == 1)
						{
							skip_flag = 0;
							Low_Battery_popup_skip = 0;
						}
					}
				}
				else
				{
					if((Low_Battery_popup_shown == 1)&&(Alert_key == 0x22010))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					Low_Battery_popup_shown = 0;
					alerts_history[27] = 0;
					if(Low_Battery_popup_skip == 1)
					{
						skip_flag = 0;
						Low_Battery_popup_skip = 0;
					}
				}
			}
			else
			{
				Low_Battery_popup_shown = 0;
				alerts_history[27] = 0;
				if(Low_Battery_popup_skip == 1)
				{
					skip_flag = 0;
					Low_Battery_popup_skip = 0;
				}
			}
		}
		else
		{
			Low_Battery_popup_shown = 0;
			alerts_history[27] = 0;
			if(Low_Battery_popup_skip == 1)
			{
				skip_flag = 0;
				Low_Battery_popup_skip = 0;
			}
		}
	}
	else
	{
		Low_Battery_popup_shown = 0;
		alerts_history[27] = 0;
		if(Low_Battery_popup_skip == 1)
		{
			skip_flag = 0;
			Low_Battery_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: Check_Engine_alert
* Description  : This function used for showing Check_Engine_alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */

void Check_Engine_alert(void)
{
	uint8_t STS_Eng_data = 0U;
	STS_Eng_data = CAN_EMS1_10[6].byte;
	STS_Eng_data = ((STS_Eng_data & 0xE0) >> 5);

	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(Diag_RW_CheckEngine_Warn_Enable == 1)
			{
				if((STS_Eng_data == 2)&&(Check_Engine_flag == 1))//if(CAN_ESC2_10[4].bits.Rx_4 == 0)
				{
					eng_start_timer_on = 1;				//5 sec timer

					if(eng_start_flag == 1)
					{
						/*Check_Engine Alert ON */
						if(((Check_Engine_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((Check_Engine_popup_shown == 0)&&(Check_Engine_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
							Priority_flag = 1;
							//Insert Event for Check_Engine Alert warning
							(void)drome_sendEventToDrome((U32)drome_InEvent_check_engine);
																{
							Notification_chime = 1;
																}
							Check_Engine_popup_shown=1;
							Check_Engine_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							alerts_history[21] = 1;
						}

					}
					else
					{
						Check_Engine_popup_shown = 0;
						alerts_history[21] = 0;
					}
				}
				else
				{

					if((Check_Engine_popup_shown == 1)&&(Alert_key == 0x22012))
					{
						Priority_flag = 0;

						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					//eng_start_timer_on = 0;
					Check_Engine_popup_shown = 0;
					alerts_history[21] = 0;
					if(Check_Engine_popup_skip == 1)
					{
						skip_flag = 0;
						Check_Engine_popup_skip = 0;
					}
				}
			}
			else
			{
				Check_Engine_popup_shown = 0;
				alerts_history[21] = 0;
				if(Check_Engine_popup_skip == 1)
				{
					skip_flag = 0;
					Check_Engine_popup_skip = 0;
				}
			}
		}
		else
		{
			Check_Engine_popup_shown = 0;
			alerts_history[21] = 0;
			if(Check_Engine_popup_skip == 1)
			{
				skip_flag = 0;
				Check_Engine_popup_skip = 0;
			}
		}
	}
	else
	{
		Check_Engine_popup_shown = 0;
		alerts_history[21] = 0;
		if(Check_Engine_popup_skip == 1)
		{
			skip_flag = 0;
			Check_Engine_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: Service_reminder
* Description  : This function used for showing Service_reminder
* Arguments    : None
* Return Value : None
************************************************************************************************ */
/*New for 0.5 million km*/
void Service_reminder(void)
{
/*For testing only*/
//	NVM_SERVICE_DONE_flag = DID_NVM_SERVICE_DONE_0x04F3_arr[1]; commented for testing
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/*Commented for Alert in IGN cycle*/
		if((service_reminder == 0)&&(memory_read == 0))
		{
			memory_read++;											//to reduce memory use cycles in initial 800kms
			service_reminder = eeprom_read(SERVICE_REMINDER_COUNT);//read previous service count
			show_service_reminder = eeprom_read(SERVICE_REMINDER_ALERT_COUNT);//read alert display count
			if((final_odomter_distance/1000) <= 800)
			{
				service_reminder = 0;
				show_service_reminder = 0;
				eeprom_write(SERVICE_REMINDER_COUNT, service_reminder);//Write new service count
				eeprom_write (SERVICE_REMINDER_ALERT_COUNT, show_service_reminder);
			}
		}
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			NVM_SERVICE_DONE_flag = DID_NVM_SERVICE_DONE_0x04F3_arr[0]; //commented for testing
			if(SERVICE_SLAB_START[show_service_reminder]<500001)
			//if(service_reminder < 10)
			{
//				if(SERVICE_SLAB_START[show_service_reminder+1]<= dummy_odo)//(final_odomter_distance/1000))
				if((SERVICE_SLAB_START[show_service_reminder+1]<=(final_odomter_distance/1000)) && (FEATURE_CODE_SERVICE_REMINDER_CONFIG == 1))
				{
//					NVM_SERVICE_DONE_flag = 0;	//service required
//
//					/*make NVM_SERVICE_DONE DID Para. = 0 ie. Service required*/
//					eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);//DID_04F3_OFFSET=0x0826
				  
				    if(show_service_reminder==service_reminder)
				    {
				    	if(service_reminder < 7)
				    	{
				    		service_reminder++;//r
				    		NVM_SERVICE_DONE_flag = 0;	//service required
				    		DID_NVM_SERVICE_DONE_0x04F3_arr[0] = 0;
				    		/*make NVM_SERVICE_DONE DID Para. = 0 ie. Service required*/
				    		eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);//DID_04F3_OFFSET=0x0826
				    		Service_Reminder_popup_shown = 0;//to display new alert upto servoce slab 7
				    	}
				    	else
				    	{
				    		service_reminder = 7;
				    		Service_Reminder_popup_shown = 0;//to display new alert upto service slab max
				    	}
				    }
				    NVM_SERVICE_DONE_flag = 0;	//service required
				    eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);//DID_04F3_OFFSET=0x0826(need to test)
				    show_service_reminder++;//increment only alert display
//				    Service_Reminder_popup_shown = 0;//to display new alert upto service slab max
				}
				else if(FEATURE_CODE_SERVICE_REMINDER_CONFIG == 0)
				{
					service_reminder = 0;//send service_reminder = 0 on CAN
				}
			}
			/*Service Reminder Config. Enable*/
			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_SERVICE_REMINDER_CONFIG == 1)
					&& (Diag_RW_ServiceReminder_Warn_Enable == 1))
			{
				if((NVM_SERVICE_DONE_flag == 0)&&(service_reminder > 0))//if service slab completed
				{
//					service_reminder = eeprom_read(SERVICE_REMINDER_COUNT);//read previous service count
					if(SERVICE_SLAB_START[show_service_reminder+1]<500001)
//					if(/*(service_reminder > 0)&&*/(show_service_reminder < 8))
					{
						//eeprom_write(SERVICE_REMINDER_COUNT, service_reminder);//Write new service count
						if(((Service_Reminder_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((Service_Reminder_popup_shown == 0)&&(Service_Reminder_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
						        eeprom_write(SERVICE_REMINDER_COUNT, service_reminder);//Write new service count
						        eeprom_write (SERVICE_REMINDER_ALERT_COUNT, show_service_reminder);// count here=(to avoid alert redisplay)
							//Insert Event for Check_Engine Alert warning
						        Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Service_Due);
							Notification_chime = 1;
							Service_Reminder_popup_shown=1;
							Service_Reminder_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							alerts_history[29] = 1;
						}

					}
					else
					{
						alerts_history[29] = 0;
						Service_Reminder_popup_shown = 0;
						if(Service_Reminder_popup_skip == 1)
						{
							skip_flag = 0;
							Service_Reminder_popup_skip = 0;
						}
					}
				}
				else
				{
					alerts_history[29] = 0;
					Service_Reminder_popup_shown = 0;
					if(Service_Reminder_popup_skip == 1)
					{
						skip_flag = 0;
						Service_Reminder_popup_skip = 0;
					}
				}

			}
			else
			{
				alerts_history[29] = 0;
				Service_Reminder_popup_shown = 0;
				if(Service_Reminder_popup_skip == 1)
				{
					skip_flag = 0;
					Service_Reminder_popup_skip = 0;
				}
			}

		}
		else
		{
			alerts_history[29] = 0;
			Service_Reminder_popup_shown = 0;
			if(Service_Reminder_popup_skip == 1)
			{
				skip_flag = 0;
				Service_Reminder_popup_skip = 0;
			}
		}

	}
	else
	{
		alerts_history[29] = 0;
		Service_Reminder_popup_shown = 0;
		if(Service_Reminder_popup_skip == 1)
		{
			skip_flag = 0;
			Service_Reminder_popup_skip = 0;
		}
	}

}

/* ***********************************************************************************************
* Function Name: Cruise_alerts
* Description  : This function used for showing Cruise_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Cruise_alerts(void)
{
	uint8_t cruise_mode = 0U;
	cruise_mode = CAN_EMS1_10[4].byte;
	cruise_mode = ((cruise_mode & 0xC0) >> 6);

	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if(IGN_cruise == 0)
		{
			cruise_sts = cruise_mode;
			IGN_cruise = 1;
		}
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check Cruise_alerts Functionality*/
			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_CRUISE_TT_CONFIG == 1) && (Diag_RW_Cruise_Warn_Enable == 1))
			{
				/*Check for CAN Missing*/
				if(EMS1_10_message_timeout_flag == 0)
				{
					if(cruise_mode == 1)//((CAN_EMS1_10[4].bits.Rx_7 == 1) && (CAN_EMS1_10[4].bits.Rx_8 == 0))	//cruise on
					{
						if(cruise_sts == 0)
						{
							/* Cruise_alerts. Alert ON */
							if((CruiseON_alerts_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0))
							{
//								Add Cruise On alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Cruise_ON);//for testing only
								CruiseON_alerts_popup_shown=1;
								CruiseOFF_alerts_popup_shown=0;
								dont_throw_alerts_alert_on_screen_present=1;
								cruise_sts = 1;
								skip_alert_Enable = 3;
								//alerts_history[21] = 1;
							}

						}
						else
						{
//							Cruise_alerts_popup_shown = 0;
							CruiseOFF_alerts_popup_shown=0;
							//alerts_history[21] = 0;
						}
					}
					else if(cruise_mode == 0)//((CAN_EMS1_10[4].bits.Rx_7 == 0) && (CAN_EMS1_10[4].bits.Rx_8 == 0))	//cruise off
					{
						if(cruise_sts == 1)
						{
							/* Cruise_alerts. Alert Off */
							if((CruiseOFF_alerts_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0))
							{
//								Add Cruise Off alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Cruise_OFF);//for testing only
//								Cruise_alerts_popup_shown=1;
								CruiseOFF_alerts_popup_shown=1;
								CruiseON_alerts_popup_shown=0;

								dont_throw_alerts_alert_on_screen_present=1;
								cruise_sts = 0;
								skip_alert_Enable = 3;
//								alerts_history[22] = 1;
							}

						}
						else
						{
//							Cruise_alerts_popup_shown = 0;
							CruiseON_alerts_popup_shown=0;
//							alerts_history[22] = 0;
						}
					}
					else
					{
						CruiseOFF_alerts_popup_shown=0;
														CruiseON_alerts_popup_shown=0;
					}

					if((CruiseON_alerts_popup_shown == 1)&&(Alert_key == 0x22016)&&(cruise_mode == 0))
					{
						Priority_flag = 0;

						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						CruiseON_alerts_popup_shown = 0;
					}
					else if((CruiseOFF_alerts_popup_shown == 1)&&(Alert_key == 0x22017)&&(cruise_mode == 1))
						//if((EcoON_mode_popup_shown == 1)&&(Alert_key == 0x22019))
					{
						Priority_flag = 0;

						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						CruiseOFF_alerts_popup_shown = 0;
					}

				}
				else
				{
					//					Cruise_alerts_popup_shown = 0;
					CruiseOFF_alerts_popup_shown=0;
					CruiseON_alerts_popup_shown=0;
					//					alerts_history[21] = 0;
					//					alerts_history[22] = 0;
				}
			}
			else
			{
				//				Cruise_alerts_popup_shown = 0;
				CruiseOFF_alerts_popup_shown=0;
				CruiseON_alerts_popup_shown=0;
				//				alerts_history[21] = 0;
				//				alerts_history[22] = 0;
			}
		}
		else
		{
			//			Cruise_alerts_popup_shown = 0;
			CruiseOFF_alerts_popup_shown=0;
			CruiseON_alerts_popup_shown=0;
			//			alerts_history[21] = 0;
			//			alerts_history[22] = 0;
		}
	}
	else
	{
		//		Cruise_alerts_popup_shown = 0;
		CruiseOFF_alerts_popup_shown=0;
		CruiseON_alerts_popup_shown=0;
	}
}


/* ***********************************************************************************************
* Function Name: Check_EXT_Lamp
* Description  : This function used for showing Check External Lamp
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Check_EXT_Lamp(void)
{
	if(IGN_ON_flag == 0)
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check Check_EXT_Lamp Functionality*/
			if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Park_Lamp_Warn_Enable == 1) && (FEATURE_CODE_PARK_LAMP_ON_CONFIG == 1))
			{
				/*Check CAN Signal not missing*/
				if(MBFM1_100_message_timeout_flag == 0)
				{
					if(CAN_MBFM1_100[1].bits.Rx_5 == 1)
					{
						/* Check_EXT_Lamp Alert ON */
						if((Check_EXT_Lamp_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present == 0))
						{
							(void)drome_sendEventToDrome((U32)drome_InEvent_External_Lamp);//
							Check_EXT_Lamp_popup_shown = 1;
							dont_throw_alerts_alert_on_screen_present = 1;
						}
					}
					else
					{
						Check_EXT_Lamp_popup_shown = 0;
					}
				}
				else
				{
					Check_EXT_Lamp_popup_shown = 0;
				}
			}
			else
			{
				Check_EXT_Lamp_popup_shown = 0;
			}
		}
		else
		{
			Check_EXT_Lamp_popup_shown = 0;
		}
	}
	else
	{
		Check_EXT_Lamp_popup_shown = 0;
	}
}

/* ***********************************************************************************************
* Function Name: ECO_Mode_alerts
* Description  : This function used for showing ECO_Mode_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void ECO_Mode_alerts(void)
{
	uint8_t ECO_Mode = 0U;
	ECO_Mode = CAN_EMS4_20[6].byte;
	ECO_Mode = ((ECO_Mode & 0xE0) >> 5);

	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if(IGN_eco==0)
		{
			Eco_sts = ECO_Mode;
			IGN_eco = 1;
		}
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check ECO_Mode_alerts Functionality*/
			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_ECO_TT_CONFIG == 1) && (Diag_RW_ECO_Warn_Enable == 1))
			{
				/*Check for CAN Missing*/
				if(EMS4_20_message_timeout_flag == 0)
				{
					if(ECO_Mode == 1)
					{
						ECO_Mode = 1;
						if(Eco_sts == 0)
						{
							/* ECO_Mode_alerts. Alert ON */
							if((EcoON_mode_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0))
							{
//								Add ECO_Mode On alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_ECO_ON);//for testing only
								EcoON_mode_popup_shown = 1;
								EcoOFF_mode_popup_shown = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								Eco_sts = 1;
								skip_alert_Enable = 3;
//								alerts_history[23] = 1;
							}

						}
						else
						{
//							EcoON_mode_popup_shown = 0;
							EcoOFF_mode_popup_shown = 0;
//							alerts_history[23] = 0;
						}
					}
					else if(ECO_Mode == 0)	//ECO off
					{
						ECO_Mode = 0;
						if(Eco_sts == 1)
						{
							/* ECO_Mode_alerts. Alert Off */
							if((EcoOFF_mode_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
							{
//								Add ECO_Mode Off alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_ECO_OFF);//for testing only
								EcoOFF_mode_popup_shown = 1;
								EcoON_mode_popup_shown = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								Eco_sts = 0;
								skip_alert_Enable = 3;
//								alerts_history[24] = 1;
							}

						}
						else
						{
//							EcoOFF_mode_popup_shown = 0;
							EcoON_mode_popup_shown = 0;
//							alerts_history[24] = 0;
						}
					}
					else
					{

						EcoOFF_mode_popup_shown = 0;
						EcoON_mode_popup_shown = 0;
//						alerts_history[23] = 0;
//						alerts_history[24] = 0;
					}

					if((EcoON_mode_popup_shown == 1)&&(Alert_key == 0x22019)&&(ECO_Mode == 0))
					{
						Priority_flag = 0;

						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						EcoON_mode_popup_shown = 0;
					}
					else if((EcoOFF_mode_popup_shown == 1)&&(Alert_key == 0x22031)&&(ECO_Mode == 1))
						//if((EcoON_mode_popup_shown == 1)&&(Alert_key == 0x22019))
					{
						Priority_flag = 0;

						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						EcoOFF_mode_popup_shown = 0;
					}
				}
				else
				{
					//					Eco_mode_popup_shown = 0;
					EcoOFF_mode_popup_shown = 0;
					EcoON_mode_popup_shown = 0;
					//					alerts_history[23] = 0;
					//					alerts_history[24] = 0;
					//Eco_sts = 0;
				}
			}
			else
			{
//				Eco_mode_popup_shown = 0;
				EcoOFF_mode_popup_shown = 0;
										EcoON_mode_popup_shown = 0;
//				alerts_history[23] = 0;
//				alerts_history[24] = 0;
			}
		}
		else
		{
//			Eco_mode_popup_shown = 0;
			EcoOFF_mode_popup_shown = 0;
									EcoON_mode_popup_shown = 0;
//			alerts_history[23] = 0;
//			alerts_history[24] = 0;
		}
	}
	else
	{
		Eco_mode_popup_shown = 0;
		EcoOFF_mode_popup_shown = 0;
								EcoON_mode_popup_shown = 0;
	}
}

/* ***********************************************************************************************
* Function Name: PAB_Off_alerts
* Description  : This function used for showing PAB_Off_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void PAB_Off_alerts(void)
{
	uint8_t INDC_PADL_STS = 0U;
	INDC_PADL_STS = CAN_SRS1_20[0].bits.Rx_5;
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			PAB_Timer_ON = 1;
			/*Check PAB_Off_alerts Functionality*/
			if((VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM == 1) && (Diag_RW_PABOFF_Warn_Enable == 1))
			{
				/*Check for CAN Missing*/
				if(SRS1_20_message_timeout_flag == 0)
				{
					if((INDC_PADL_STS == 1)&&(PAB_Time_Done == 1))
					{
						if(((PAB_Off_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((PAB_Off_popup_shown == 0)&&(PAB_Off_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
//								Add ECO_Mode Off alert
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_PAB_OFF);//for testing only
							PAB_Off_popup_shown = 1;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							PAB_Off_popup_skip = 0;
							alerts_history[5] = 1;
						}
					}
					else
					{
						if((PAB_Off_popup_shown == 1)&&(Alert_key == 0x22020))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						alerts_history[5] = 0;
						PAB_Off_popup_shown = 0;
					}
				}
				else
				{
					PAB_Off_popup_shown = 0;
					alerts_history[5] = 0;
					if(PAB_Off_popup_skip == 1)
					{
						PAB_Off_popup_skip = 0;
						skip_flag = 0;
					}
				}
			}
			else
			{
				PAB_Off_popup_shown = 0;
				alerts_history[5] = 0;
				if(PAB_Off_popup_skip == 1)
				{
					PAB_Off_popup_skip = 0;
					skip_flag = 0;
				}
			}
		}
		else
		{
			PAB_Off_popup_shown = 0;
			alerts_history[5] = 0;
			if(PAB_Off_popup_skip == 1)
			{
				PAB_Off_popup_skip = 0;
				skip_flag = 0;
			}
		}
	}
	else
	{
		PAB_Timer_ON = 0;
		PAB_Off_popup_shown = 0;
		alerts_history[5] = 0;
		if(PAB_Off_popup_skip == 1)
		{
			PAB_Off_popup_skip = 0;
			skip_flag = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: Trailer_SW_On/Off_alerts
* Description  : This function used for showing Trailer_SW_On/Off_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Trailer_SW_On_alerts(void)
{
	uint8_t STS_Trailer = 0U;
	STS_Trailer = CAN_MBFM1_100[5].bits.Rx_5;
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check Trailer_SW_On/Off_alerts Functionality*/
			if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Trailer_SW_ON_Warn_Enable == 1))
			{
				/*Check CAN Signal not missing*/
				if(MBFM1_100_message_timeout_flag == 0)
				{
					if(STS_Trailer == 1)
					{
						if(Trailer_sts == 0)
						{
							/* Trailer_SW_On/Off_alerts. Alert ON */

							if((Trailer_SW_on_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0))
							{
								Priority_flag = 1;
//								Add Trailer_SW_On/Off_alerts On alert
								(void)drome_sendEventToDrome((U32)drome_InEvent_Trailer_SW_ON);//for testing only
								Trailer_SW_on_popup_shown = 1;
								Trailer_SW_off_popup_shown = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								Trailer_sts = 1;
								skip_alert_Enable=3;
//								alerts_history[26] = 1;
							}
						}
						else
						{
							Trailer_SW_off_popup_shown = 0;
							//alerts_history[26] = 0;		not required
						}
					}
					else if(STS_Trailer == 0)	//ECO off
					{
						if(Trailer_sts == 1)
						{
							/* Trailer_SW_On/Off_alerts. Alert Off */

							if((Trailer_SW_off_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
							{
//								Add Trailer_SW_On/Off_alerts Off alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Trailer_SW_OFF);//for testing only
								Trailer_SW_off_popup_shown = 1;
								Trailer_SW_on_popup_shown = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								Trailer_sts = 0;
//								alerts_history[27] = 1;
							}
						}
						else
						{
							Trailer_SW_on_popup_shown = 0;
						//	Trailer_SW_off_popup_shown = 0;
							//alerts_history[27] = 0;		not required
						}
					}
					else
					{
						Trailer_SW_on_popup_shown = 0;
						Trailer_SW_off_popup_shown = 0;
//						alerts_history[27] = 0;
//						alerts_history[27] = 0;
					}
					if((Trailer_SW_off_popup_shown == 1)&&(Alert_key == 0x22022)&&(STS_Trailer == 1))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					else if((Trailer_SW_on_popup_shown == 1)&&(Alert_key == 0x22021)&&(STS_Trailer == 0))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
				}
				else
				{
					Trailer_sts = 0;
					Trailer_SW_on_popup_shown = 0;
					Trailer_SW_off_popup_shown = 0;
//					alerts_history[27] = 0;
//					alerts_history[27] = 0;
				}
			}
			else
			{
				Trailer_SW_on_popup_shown = 0;
				Trailer_SW_off_popup_shown = 0;
//				alerts_history[27] = 0;
//				alerts_history[27] = 0;
			}
		}
		else
		{
			Trailer_SW_on_popup_shown = 0;
			Trailer_SW_off_popup_shown = 0;
//			alerts_history[27] = 0;
//			alerts_history[27] = 0;
		}
	}
	else
	{
		Trailer_SW_on_popup_shown = 0;
		Trailer_SW_off_popup_shown = 0;
	}
}

/* ***********************************************************************************************
* Function Name: Horn_failure_alerts
* Description  : This function used for showing Horn_failure_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Horn_failure_alerts(void)
{
	uint8_t Horn_fail = 0U;
	Horn_fail = (CAN_MBFM9_500[6].bits.Rx_1);//MBFM9_500  Bit 5

	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check Trailer_SW_On/Off_alerts Functionality*/
			if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Horn_failure_Warn_Enable == 1))
			{
				/*Check CAN Signal not missing*/
				if(MBFM9_500_message_timeout_flag == 0)
				{
					if(Horn_fail == 1)
					{
						/* Horn_Fail_alerts. Alert ON */
						if(((Horn_Fail_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((Horn_Fail_popup_shown == 0)&&(Horn_Fail_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
//								Add Trailer_SW_On/Off_alerts On alert
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Horn_Failure);//for testing only
							Horn_Fail_popup_shown = 1;
							Horn_Fail_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							alerts_history[36] = 1;
						}

					}
					else
					{
						if((Horn_Fail_popup_shown == 1)&&(Alert_key == 0x22024))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						alerts_history[36] = 0;
						Horn_Fail_popup_shown = 0;
						if(Horn_Fail_popup_skip == 1)
						{
							skip_flag = 0;
							Horn_Fail_popup_skip = 0;
						}

					}
				}
				else
				{
					if((Horn_Fail_popup_shown == 1)&&(Alert_key == 0x22024))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					alerts_history[36] = 0;
					Horn_Fail_popup_shown = 0;
					if(Horn_Fail_popup_skip == 1)
					{
						skip_flag = 0;
						Horn_Fail_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[36] = 0;
				Horn_Fail_popup_shown = 0;
				if(Horn_Fail_popup_skip == 1)
				{
					skip_flag = 0;
					Horn_Fail_popup_skip = 0;
				}
			}
		}
		else
		{
			alerts_history[36] = 0;
			Horn_Fail_popup_shown = 0;
			if(Horn_Fail_popup_skip == 1)
			{
				skip_flag = 0;
				Horn_Fail_popup_skip = 0;
			}
		}
	}
	else
	{
		alerts_history[36] = 0;
		Horn_Fail_popup_shown = 0;
		if(Horn_Fail_popup_skip == 1)
		{
			skip_flag = 0;
			Horn_Fail_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: DRL_failure_alerts
* Description  : This function used for showing DRL_failure_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void DRL_failure_alerts(void)
{
	_Bool Left_DRL_fail = 0U;
	_Bool Right_DRL_fail = 0U;
	Left_DRL_fail = (CAN_MBFM9_500[5].bits.Rx_6);//MBFM9_500  Bit 2
	Right_DRL_fail = (CAN_MBFM9_500[5].bits.Rx_7);//MBFM9_500  Bit 3

	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check DRL_failure_alerts Functionality*/
			if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Park_lamp_failure_Warn_Enable == 1))
			{
				/*Check CAN Signal not missing*/
				if(MBFM9_500_message_timeout_flag == 0)
				{
					if(Left_DRL_fail == 1)
					{
						/* DRL_failure_alerts. Alert ON */
						if(((Left_DRL_failure_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((Left_DRL_failure_popup_shown == 0)&&(Left_DRL_failure_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
//								Add Trailer_SW_On/Off_alerts On alert
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Left_DRL_Lamp_Failure);//for testing only
							Left_DRL_failure_popup_shown = 1;
							Left_DRL_failure_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							alerts_history[33] = 1;
						}

					}
					else if(Left_DRL_fail == 0)
					{
						alerts_history[33] = 0;
						if((Left_DRL_failure_popup_shown == 1)&&(Alert_key == 0x22025))
								{
									Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						Left_DRL_failure_popup_shown = 0;
						if(Left_DRL_failure_popup_skip == 1)
						{
							skip_flag = 0;
							Left_DRL_failure_popup_skip = 0;
						}
					}
					if(Right_DRL_fail == 1)
					{
						/* DRL_failure_alerts. Alert ON */
						if(((Right_DRL_failure_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
								/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
								((Right_DRL_failure_popup_shown == 0)&&(Right_DRL_failure_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
						{
//								Add Trailer_SW_On/Off_alerts On alert
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Right_DRL_Lamp_Failure);//for testing only
							Right_DRL_failure_popup_shown = 1;
							Right_DRL_failure_popup_skip = 0;
							dont_throw_alerts_alert_on_screen_present=1;
							skip_alert_Enable = 1;
							alerts_history[34] = 1;
						}

					}
					else if(Right_DRL_fail == 0)
					{
						alerts_history[34] = 0;
						if((Right_DRL_failure_popup_shown == 1)&&(Alert_key == 0x22033))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						Right_DRL_failure_popup_shown = 0;
						if(Right_DRL_failure_popup_skip == 1)
						{
							skip_flag = 0;
							Right_DRL_failure_popup_skip = 0;
						}
					}

				}
				else
				{
					if((Left_DRL_failure_popup_shown == 1)&&(Alert_key == 0x22025))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					if((Right_DRL_failure_popup_shown == 1)&&(Alert_key == 0x22033))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					alerts_history[33] = 0;
					alerts_history[34] = 0;
					Left_DRL_failure_popup_shown = 0;
					Right_DRL_failure_popup_shown = 0;
					if(Right_DRL_failure_popup_skip == 1)
					{
						skip_flag = 0;
						Right_DRL_failure_popup_skip = 0;
					}
					else if(Left_DRL_failure_popup_skip == 1)
					{
						skip_flag = 0;
						Left_DRL_failure_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[33] = 0;
				alerts_history[34] = 0;
				Left_DRL_failure_popup_shown = 0;
				Right_DRL_failure_popup_shown = 0;
				if(Right_DRL_failure_popup_skip == 1)
				{
					skip_flag = 0;
					Right_DRL_failure_popup_skip = 0;
				}
				else if(Left_DRL_failure_popup_skip == 1)
				{
					skip_flag = 0;
					Left_DRL_failure_popup_skip = 0;
				}
			}
		}
		else
		{
			alerts_history[33] = 0;
			alerts_history[34] = 0;
			Left_DRL_failure_popup_shown = 0;
			Right_DRL_failure_popup_shown = 0;
			if(Right_DRL_failure_popup_skip == 1)
			{
				skip_flag = 0;
				Right_DRL_failure_popup_skip = 0;
			}
			else if(Left_DRL_failure_popup_skip == 1)
			{
				skip_flag = 0;
				Left_DRL_failure_popup_skip = 0;
			}
		}
	}
	else
	{
		alerts_history[33] = 0;
		alerts_history[34] = 0;
		Left_DRL_failure_popup_shown = 0;
		Right_DRL_failure_popup_shown = 0;
		if(Right_DRL_failure_popup_skip == 1)
		{
			skip_flag = 0;
			Right_DRL_failure_popup_skip = 0;
		}
		else if(Left_DRL_failure_popup_skip == 1)
		{
			skip_flag = 0;
			Left_DRL_failure_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: LEFT_Turn_Lamp_failure_alerts
* Description  : This function used for showing LEFT_Turn_Lamp_failure_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void LEFT_Turn_Lamp_failure_alerts(void)
{
	_Bool Left_Lamp_fail = 0U;
	Left_Lamp_fail = (CAN_MBFM9_500[5].bits.Rx_4);//MBFM9_500  Bit 0

	if((IGN_ON_flag == 1)&&(logo_completed==1))
		{
			if((LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				/*Check LEFT_Turn_Lamp_failure_alerts Functionality*/
				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Left_turn_lamp_failure_Warn_Enable == 1))
				{
					/*Check CAN Signal not missing*/
					if(MBFM9_500_message_timeout_flag == 0)
					{
						/* Left_turn_lamp_alerts. Alert ON */
						if(Left_Lamp_fail == 1)
						{
							if(((Left_Turn_failure_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Left_Turn_failure_popup_shown == 0)&&(Left_Turn_failure_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
	//								Add Trailer_SW_On/Off_alerts On alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Left_Turn_Lamp_Failure);//for testing only
								Left_Turn_failure_popup_shown = 1;
								Left_Turn_failure_popup_skip = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								skip_alert_Enable = 1;
								alerts_history[31] = 1;
							}
						}
						else
						{
							if((Left_Turn_failure_popup_shown == 1)&&(Alert_key == 0x22026))
							{
								Priority_flag = 0;
								start_alart_timeout_5sec=0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}

							alerts_history[31] = 0;
							Left_Turn_failure_popup_shown = 0;
							if(Left_Turn_failure_popup_skip == 1)
							{
								skip_flag = 0;
								Left_Turn_failure_popup_skip = 0;
							}
						}
					}
					else
					{
						if((Left_Turn_failure_popup_shown == 1)&&(Alert_key == 0x22026))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						alerts_history[31] = 0;
						Left_Turn_failure_popup_shown = 0;
						if(Left_Turn_failure_popup_skip == 1)
						{
							skip_flag = 0;
							Left_Turn_failure_popup_skip = 0;
						}
					}
				}
				else
				{
					alerts_history[31] = 0;
					Left_Turn_failure_popup_shown = 0;
					if(Left_Turn_failure_popup_skip == 1)
					{
						skip_flag = 0;
						Left_Turn_failure_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[31] = 0;
				Left_Turn_failure_popup_shown = 0;
				if(Left_Turn_failure_popup_skip == 1)
				{
					skip_flag = 0;
					Left_Turn_failure_popup_skip = 0;
				}
			}
		}
	else
	{
		Left_Turn_failure_popup_shown = 0;
		if(Left_Turn_failure_popup_skip == 1)
		{
			skip_flag = 0;
			Left_Turn_failure_popup_skip = 0;
		}
	}
}

/* ***********************************************************************************************
* Function Name: RIGHT_Turn_Lamp_failure_alerts
* Description  : This function used for showing LEFT_Turn_Lamp_failure_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RIGHT_Turn_Lamp_failure_alerts(void)
{
	_Bool Right_Lamp_fail = 0U;
	Right_Lamp_fail = (CAN_MBFM9_500[5].bits.Rx_5);//MBFM9_500  Bit 1

	if((IGN_ON_flag == 1)&&(logo_completed==1))
		{
			if((LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				/*Check LEFT_Turn_Lamp_failure_alerts Functionality*/
				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Right_turn_lamp_failure_Warn_Enable == 1))
				{
					/*Check CAN Signal not missing*/
					if(MBFM9_500_message_timeout_flag == 0)
					{
						/* Left_turn_lamp_alerts. Alert ON */
						if(Right_Lamp_fail == 1)
						{
							if(((Right_Turn_failure_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Right_Turn_failure_popup_shown == 0)&&(Right_Turn_failure_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
	//								Add Trailer_SW_On/Off_alerts On alert
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Right_Turn_Lamp_Failure);
								Right_Turn_failure_popup_shown = 1;
								Right_Turn_failure_popup_skip = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								skip_alert_Enable = 1;
								alerts_history[32] = 1;
							}
						}
						else
						{
							if((Right_Turn_failure_popup_shown == 1)&&(Alert_key == 0x22027))
							{
								Priority_flag = 0;
								start_alart_timeout_5sec=0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}

							alerts_history[32] = 0;
							Right_Turn_failure_popup_shown = 0;
							if(Right_Turn_failure_popup_skip == 1)
							{
								skip_flag = 0;
								Right_Turn_failure_popup_skip = 0;
							}
						}
					}
					else
					{
						if((Right_Turn_failure_popup_shown == 1)&&(Alert_key == 0x22027))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						alerts_history[32] = 0;
						Right_Turn_failure_popup_shown = 0;
						if(Right_Turn_failure_popup_skip == 1)
						{
							skip_flag = 0;
							Right_Turn_failure_popup_skip = 0;
						}
					}
				}
				else
				{
					alerts_history[32] = 0;
					Right_Turn_failure_popup_shown = 0;
					if(Right_Turn_failure_popup_skip == 1)
					{
						skip_flag = 0;
						Right_Turn_failure_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[32] = 0;
				Right_Turn_failure_popup_shown = 0;
				if(Right_Turn_failure_popup_skip == 1)
				{
					skip_flag = 0;
					Right_Turn_failure_popup_skip = 0;
				}
			}
		}
	else
	{
		alerts_history[32] = 0;
		Right_Turn_failure_popup_shown = 0;
		if(Right_Turn_failure_popup_skip == 1)
		{
			skip_flag = 0;
			Right_Turn_failure_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: Tail_Lamp_failure_alerts
* Description  : This function used for showing Tail_Lamp_failure_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Tail_Lamp_failure_alerts(void)
{
	_Bool Tail_Lamp_fail = 0U;
	Tail_Lamp_fail = (CAN_MBFM9_500[5].bits.Rx_8);//MBFM9_500  Bit 4

	if((IGN_ON_flag == 1)&&(logo_completed==1))
		{
			if((LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				/*Check LEFT_Turn_Lamp_failure_alerts Functionality*/
				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Tail_lamp_warn_Enable == 1))
				{
					/*Check CAN Signal not missing*/
					if(MBFM9_500_message_timeout_flag == 0)
					{
						/* Tail_Lamp_fail_alerts. Alert ON */
						if(Tail_Lamp_fail == 1)
						{
							if(((Tail_Lamp_failure_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((Tail_Lamp_failure_popup_shown == 0)&&(Tail_Lamp_failure_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Tail_Lamp_Failure);//for testing only
								Tail_Lamp_failure_popup_shown = 1;
								Tail_Lamp_failure_popup_skip = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								skip_alert_Enable = 1;
								alerts_history[35] = 1;
							}
						}
						else
						{
							if((Tail_Lamp_failure_popup_shown == 1)&&(Alert_key == 0x22028))
							{
								Priority_flag = 0;
								start_alart_timeout_5sec=0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}

							alerts_history[35] = 0;
							Tail_Lamp_failure_popup_shown = 0;
							if(Tail_Lamp_failure_popup_skip == 1)
							{
								skip_flag = 0;
								Tail_Lamp_failure_popup_skip = 0;
							}
						}
					}
					else
					{
						if((Tail_Lamp_failure_popup_shown == 1)&&(Alert_key == 0x22028))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						alerts_history[35] = 0;
						Tail_Lamp_failure_popup_shown = 0;
						if(Tail_Lamp_failure_popup_skip == 1)
						{
							skip_flag = 0;
							Tail_Lamp_failure_popup_skip = 0;
						}
					}
				}
				else
				{
					alerts_history[35] = 0;
					Tail_Lamp_failure_popup_shown = 0;
					if(Tail_Lamp_failure_popup_skip == 1)
					{
						skip_flag = 0;
						Tail_Lamp_failure_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[35] = 0;
				Tail_Lamp_failure_popup_shown = 0;
				if(Tail_Lamp_failure_popup_skip == 1)
				{
					skip_flag = 0;
					Tail_Lamp_failure_popup_skip = 0;
				}
			}
		}
	else
	{
		alerts_history[35] = 0;
		Tail_Lamp_failure_popup_shown = 0;
		if(Tail_Lamp_failure_popup_skip == 1)
		{
			skip_flag = 0;
			Tail_Lamp_failure_popup_skip = 0;
		}
	}
}


/* ***********************************************************************************************
* Function Name: RPAS_Supply_Failure_alerts
* Description  : This function used for showing RPAS_Supply_Failure_alerts
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Supply_Failure_alerts(void)
{
	_Bool RPAS_supply_Fail = 0U;
	RPAS_supply_Fail = (CAN_MBFM9_500[6].bits.Rx_2);//MBFM9_500  Bit 6
	if((IGN_ON_flag == 1)&&(logo_completed==1))
		{
			if((LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				/*Check RPAS_Supply_failure_alerts Functionality*/
				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_RPAS_supply_warn_Enable == 1))
				{
					/*Check CAN Signal not missing*/
					if(MBFM9_500_message_timeout_flag == 0)
					{
						/* RPAS_supply_Fail Alert ON */
						if(RPAS_supply_Fail == 1)
						{
							if(((RPAS_Supply_failure_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
									/*((skip_alert_Enable==2)&&(dont_throw_alerts_alert_on_screen_present==0))*/
									((RPAS_Supply_failure_popup_shown == 0)&&(RPAS_Supply_failure_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_Supply_Failure);//for testing only
								RPAS_Supply_failure_popup_shown = 1;
								RPAS_Supply_failure_popup_skip = 0;
								dont_throw_alerts_alert_on_screen_present=1;
								skip_alert_Enable = 1;
								alerts_history[26] = 1;
							}
						}
						else
						{
							if((RPAS_Supply_failure_popup_shown == 1)&&(Alert_key == 0x22029))
							{
								Priority_flag = 0;
								start_alart_timeout_5sec=0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}

							alerts_history[26] = 0;
							RPAS_Supply_failure_popup_shown = 0;
							if(RPAS_Supply_failure_popup_skip == 1)
							{
								skip_flag = 0;
								RPAS_Supply_failure_popup_skip = 0;
							}
						}
					}
					else
					{
						if((RPAS_Supply_failure_popup_shown == 1)&&(Alert_key == 0x22029))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}

						alerts_history[26] = 0;
						RPAS_Supply_failure_popup_shown = 0;
						if(RPAS_Supply_failure_popup_skip == 1)
						{
							skip_flag = 0;
							RPAS_Supply_failure_popup_skip = 0;
						}
					}
				}
				else
				{
					alerts_history[26] = 0;
					RPAS_Supply_failure_popup_shown = 0;
					if(RPAS_Supply_failure_popup_skip == 1)
					{
						skip_flag = 0;
						RPAS_Supply_failure_popup_skip = 0;
					}
				}
			}
			else
			{
				alerts_history[26] = 0;
				RPAS_Supply_failure_popup_shown = 0;
				if(RPAS_Supply_failure_popup_skip == 1)
				{
					skip_flag = 0;
					RPAS_Supply_failure_popup_skip = 0;
				}
			}
		}
	else
	{
		alerts_history[26] = 0;
		RPAS_Supply_failure_popup_shown = 0;
		if(RPAS_Supply_failure_popup_skip == 1)
		{
			skip_flag = 0;
			RPAS_Supply_failure_popup_skip = 0;
		}
	}
}

/* ***********************************************************************************************
* Function Name: Func_safety
* Description  : This function used for showing Func_safety alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Func_safety(void)
{
	_Bool FEEDBK_ABS_LAMP,FEEDBK_MIL,FEEDBK_ESS_LAMP,FEEDBK_AMT,FEEDBK_EPS;
	FEEDBK_ABS_LAMP = (CAN_IC2_100[4].bits.Rx_3);
	FEEDBK_MIL = (CAN_IC1_100[1].bits.Rx_8);//1,8
	FEEDBK_ESS_LAMP = (CAN_IC2_100[4].bits.Rx_4);//4,4
	FEEDBK_AMT = (CAN_IC2_100[5].bits.Rx_3);//5,3
	FEEDBK_EPS = (CAN_IC2_100[5].bits.Rx_1);//5,1

	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check Key_In_Reminder Functionality*/
			if(FEATURE_CODE_FUNC_SAFETY == 1)
			{
				if(FEEDBK_ABS_LAMP == 1 || FEEDBK_MIL == 1 || FEEDBK_ESS_LAMP == 1 || FEEDBK_AMT == 1 || FEEDBK_EPS == 1)
				{
					if(((Func_safety_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))||
							((Func_safety_popup_shown == 0)&&(Func_safety_popup_skip == 1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
					{
						Priority_flag = 1;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Func_safety);//for testing only
						Func_safety_popup_shown = 1;
						dont_throw_alerts_alert_on_screen_present=1;
						alerts_history[30] = 1;
					}
				}
				else
				{
					if((Func_safety_popup_shown == 1)&&(Alert_key == 0x22032))
					{
						Priority_flag = 0;
						start_alart_timeout_5sec=0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					alerts_history[30] = 0;
					Func_safety_popup_shown = 0;
				}
			}
			else
			{
				alerts_history[30] = 0;
				Func_safety_popup_shown = 0;
			}
		}
		else
		{
			alerts_history[30] = 0;
			Func_safety_popup_shown = 0;
		}
	}
	else
	{
		alerts_history[30] = 0;
		Func_safety_popup_shown = 0;
	}
}

/* ***********************************************************************************************
* Function Name: Stop_for_setting
* Description  : This function used for showing Stop_for_setting alert
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Stop_for_setting(void)
{
	uint32_t key;
	 key = drome_getCurrentViewKey();
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if(((LVSD_flag == 0U) && (HVSD_flag == 0U))&&(Diag_RW_StopSetting_Warn_Enable == 1))
		{
			if((vehicle_moving_state == 1) && (key == 0x11051) && (ok_modelong_press == 1) &&(dont_throw_alerts_alert_on_screen_present == 0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&&(skip_flag == 0))
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_Stop_Vehicle_For_Setting);
//				alerts_history[25] = 1;
				ok_modelong_press = 0;
				dont_throw_alerts_alert_on_screen_present = 1;
			}
			else
			{
				ok_modelong_press = 0;
			}
		}
		else
		{
//			alerts_history[25] = 0;
		}
	}

	if((vehicle_moving_state == 1)&&(Speed_Missing == 0))
	{
		Speed_present = 1;
	}
	else if(key != 0x22023)
	{
		Speed_present = 0;
	}
}

/* ***********************************************************************************************
* Function Name: REGEN ALERTS
* Description  : This function used for showing all regen alert functionality
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void REGEN_Alert(void)
{
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(EMS29_100_message_timeout_flag == CLEAR && VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
			{
				if(FEATURE_CODE_REGEN==1)
				{
					checkRegenDEF_flag = 1;

					REGEN_bit_data.bits.Rx_1 = CAN_EMS29_100[3].bits.Rx_7;  /* regen_tt*/
					REGEN_bit_data.bits.Rx_2 = CAN_EMS29_100[3].bits.Rx_8;

					display_key = drome_getCurrentViewKey();
					if(REGEN_bit_data.byte == 0 || REGEN_bit_data.byte == 3)
					{
						REGEN_priority_flag = 0;
						IGN_ON_regen_init = 0;
						alerts_history[7] = 0;
						//				REGEN_TT=0;
						//				DID_TT_bit_data_set6.bits.Rx_4=0;
						REGEN_TT_ON_flag = 0;
						if(REGEN_bit_data.byte == 0)
							init_regen_required = 1;	/*Regen Required Alert initiated*/
						else
							init_regen_required = 0;	/*Regen Required Alert disabled*/
						init_regen_aboarted = 0;	/*Regen Aboarted Alert disabled*/
						init_regen_started = 0;		/*Regen Started Alert disabled*/
						init_regen_not_started = 0;	/*Regen Not started Alert disabled*/

						display_key = drome_getCurrentViewKey();
						if(display_key == 0x11005)		/*display_key changed to U32*/
						{	Regencnt1++;
						if(Regencnt1 == 1)
						{
							/*To avoid temp alert skip*/
							(void)drome_sendEventToDrome((U32)drome_KB_DOWN);	// when Regen Page Active & Regen TT - OFF then it jumps to next screen
						}
						}
						else
							Regencnt1 = 0;
						if((Diag_RW_Regen_Completed_warn_Enable == 1) &&(logo_completed==1))
						{
							if(init_regen_completed == 1)	/*Regen Completed Alert Enabled*/
							{
								//								init_regen_completed = 0;
								if(((REGEN_completed_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
								   ((REGEN_completed_popup_shown==0)&&(REGEN_completed_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									if(REGEN_completed_popup_skip != 1)
									{
										init_regen_completed = 0;
									}
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_completed);
									REGEN_completed_popup_shown = 1;
									Main_page_Regen = 0;	/*Regen in Background*/
									REGEN_completed_popup_skip = 0;
									dont_throw_alerts_alert_on_screen_present = 1;
									/*here whenever regen TT blinking to off message received then REGEN COMPLETED alert for 5 sec*/
								}
								else
								{
									REGEN_Required_popup_shown = 0;
									REGEN_aboarted_popup_shown = 0;
									REGEN_notstarted_popup_shown = 0;
									if((display_key == 0x11015)&&(REGEN_started_popup_shown == 1))
									{
										start_alart_timeout_5sec=0;
										Priority_flag = 0;
										dont_throw_alerts_alert_on_screen_present=0;
										(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
									}
									REGEN_started_popup_shown = 0;
									REGEN_requested_popup_shown = 0;
								}
							}
							else
							{
								if(((display_key == 0x11031)&&(REGEN_Required_popup_shown == 1))||((display_key == 0x11015)&&(REGEN_started_popup_shown == 1))||((display_key == 0x11033)&&(REGEN_notstarted_popup_shown == 1))||((display_key == 0x11034)&&(REGEN_aboarted_popup_shown == 1))||((display_key == 0x11032)&&(REGEN_requested_popup_shown == 1)))
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
									//REGEN_Required_popup_shown = 0;
								}

								REGEN_Required_popup_shown = 0;
								REGEN_aboarted_popup_shown = 0;
								REGEN_notstarted_popup_shown = 0;
								REGEN_started_popup_shown = 0;
								REGEN_requested_popup_shown = 0;
							}
						}
						else
						{
							if(((display_key == 0x11031)&&(REGEN_Required_popup_shown == 1))||((display_key == 0x11015)&&(REGEN_started_popup_shown == 1))||((display_key == 0x11033)&&(REGEN_notstarted_popup_shown == 1))||((display_key == 0x11034)&&(REGEN_aboarted_popup_shown == 1))||((display_key == 0x11032)&&(REGEN_requested_popup_shown == 1)))
							{
								start_alart_timeout_5sec=0;
								Priority_flag = 0;
								REGEN_priority_flag = 0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								//REGEN_Required_popup_shown = 0;
							}
							REGEN_Required_popup_shown = 0;
							REGEN_aboarted_popup_shown = 0;
							REGEN_notstarted_popup_shown = 0;
							REGEN_started_popup_shown = 0;
							REGEN_requested_popup_shown = 0;
						}
					}

					else if(REGEN_bit_data.byte ==1)
					{
						//				REGEN_TT=1;
						//				DID_TT_bit_data_set6.bits.Rx_4=1;
						REGEN_TT_ON_flag = 1;
						HoldOk_Regen_text = 0;
						init_regen_started = 1;				/*Regen started Alert initiated */
						init_regen_not_started = 1;			/*Regen not started Alert initiated */
						init_regen_completed = 0;			/*Regen Completed Alert initiated*/
						if((startkey == 0x11005)&&(REGEN_notstarted_popup_shown == 1))
						{
							REGEN_requested_popup_shown = 0;
						}
						if((Diag_RW_Regen_Required_warn_Enable == 1)&&(logo_completed==1))
						{
							if(((init_regen_required == 1)||(IGN_ON_regen_init == 1))&&(dpf_check == 1))	/*Regen Required Alert Enabled*/
							{
								//							init_regen_required = 0;
								REGEN_priority_flag = 1;
								if(((REGEN_Required_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
								   ((REGEN_Required_popup_shown==0)&&(REGEN_Required_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									if(REGEN_Required_popup_skip != 1)
									{
										//								init_regen_required = 0;
										//								IGN_ON_regen_init = 0;
									}
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_Required);
									REGEN_Required_popup_shown = 1;
									Main_page_Regen = 0;
									REGEN_priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present = 1;
									REGEN_Required_popup_skip = 0;
									alerts_history[7] = 1;
								}
								else
								{
									REGEN_aboarted_popup_shown = 0;
									REGEN_completed_popup_shown = 0;
									REGEN_started_popup_shown = 0;
									REGEN_notstarted_popup_shown = 0;
									REGEN_requested_popup_shown = 0;
								}
							}
							else
							{
								//						REGEN_aboarted_popup_shown = 0;
								//						REGEN_completed_popup_shown = 0;
								if(((display_key == 0x11015)&&(REGEN_started_popup_shown == 1))||((display_key == 0x11030)&&(REGEN_completed_popup_shown == 1)))
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									REGEN_priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								}
								REGEN_started_popup_shown = 0;
								REGEN_completed_popup_shown = 0;
								REGEN_priority_flag = 0;
								//						REGEN_notstarted_popup_shown = 0;
								//						REGEN_requested_popup_shown = 0;
							}
						}
						else
						{
							REGEN_aboarted_popup_shown = 0;
							REGEN_completed_popup_shown = 0;
							REGEN_started_popup_shown = 0;
							REGEN_notstarted_popup_shown = 0;
							REGEN_requested_popup_shown = 0;
						}

						if((Diag_RW_Regen_Aborted_warn_Enable == 1)&&(logo_completed==1))
						{
							if(init_regen_aboarted == 1)	/*Regen Aboarted Alert Enabled*/
							{
								alerts_history[7] = 0;
								REGEN_priority_flag = 0;
								//							init_regen_aboarted = 0;
								regen_page_started=0;
								IGN_ON_regen_init = 0;
								if(((REGEN_aboarted_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
								   ((REGEN_aboarted_popup_shown==0)&&(REGEN_aboarted_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									if(REGEN_aboarted_popup_skip != 1)
									{
										//								init_regen_aboarted = 0;

									}
									init_regen_not_started = 0;
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_aborted);
									REGEN_aboarted_popup_shown = 1;
									Main_page_Regen = 0;		/*Regen in Background*/
									REGEN_aboarted_popup_skip = 0;
									dont_throw_alerts_alert_on_screen_present = 1;
								}
								else
								{
									REGEN_completed_popup_shown = 0;
									REGEN_started_popup_shown = 0;
									REGEN_notstarted_popup_shown = 0;
									REGEN_Required_popup_shown = 0;
									REGEN_requested_popup_shown = 0;
								}
							}
							else
							{
								if(((display_key == 0x11015)&&(REGEN_started_popup_shown == 1))||((display_key == 0x11030)&&(REGEN_completed_popup_shown == 1)))	/*Regen Started alert disable*/
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									REGEN_priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								}
								REGEN_started_popup_shown = 0;
								REGEN_completed_popup_shown = 0;
								//						REGEN_notstarted_popup_shown = 0;
								//						REGEN_Required_popup_shown = 0;
								//						REGEN_requested_popup_shown = 0;
							}
						}
						else
						{
							REGEN_completed_popup_shown = 0;
							REGEN_started_popup_shown = 0;
							REGEN_notstarted_popup_shown = 0;
							REGEN_Required_popup_shown = 0;
							REGEN_requested_popup_shown = 0;
						}

						if((Diag_RW_Regen_Not_started_warn_Enable == 1)&&(logo_completed==1))
						{
							if((init_regen_not_started == 1)&&(wait_10sec == 1))				/*Regen started Alert initiated */
							{REGEN_priority_flag = 0;
							alerts_history[7] = 0;
							IGN_ON_regen_init = 0;
							if(((REGEN_notstarted_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
							   ((REGEN_notstarted_popup_shown==0)&&(REGEN_notstarted_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
							{
								if(REGEN_notstarted_popup_skip != 1)
								{
									//							init_regen_not_started = 0;
									//							wait_10sec = 0;
								}
								/*init_regen_not_started = 0;
						wait_10sec = 0;*/
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_not_started);/**/
								REGEN_notstarted_popup_shown = 1;
								Main_page_Regen = 0;//2;//1;		/*Regen Required in Background*/
								REGEN_notstarted_popup_skip = 0;
								dont_throw_alerts_alert_on_screen_present = 1;
							}
							else
							{
								REGEN_Required_popup_shown = 0;
								REGEN_aboarted_popup_shown = 0;
								REGEN_completed_popup_shown = 0;
								REGEN_started_popup_shown = 0;
								REGEN_requested_popup_shown = 0;
							}
							}
							else
							{
								//						REGEN_Required_popup_shown = 0;
								//						REGEN_aboarted_popup_shown = 0;
								REGEN_completed_popup_shown = 0;
								if((display_key == 0x11015)&&(REGEN_started_popup_shown == 1))
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									REGEN_priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								}
								REGEN_started_popup_shown = 0;
								//						REGEN_requested_popup_shown = 0;
							}

						}
						else
						{
							REGEN_Required_popup_shown = 0;
							REGEN_aboarted_popup_shown = 0;
							REGEN_completed_popup_shown = 0;
							REGEN_started_popup_shown = 0;
							REGEN_requested_popup_shown = 0;
						}
						display_key = drome_getCurrentViewKey();
						if((Diag_RW_Regen_Requested_warn_Enable == 1)&&(logo_completed==1))
						{
							if(((/*SET_key_press*/SET_5seckey_press == 1)||(Ok_key_LongPress == 1))&&(display_key == 0x11005))
							{
								REGEN_priority_flag = 0;
								alerts_history[7] = 0;
							//	if((REGEN_requested_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
								if((REGEN_requested_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))  //skip_flag == 0 added to avoid Regen requested if any BS6 alert skipped

								{
									Priority_flag = 1;//need to confirm
									(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_Requested);
									REGEN_requested_popup_shown = 1;
									//						Main_page_Regen = 2;				/*Regen is in Progress in Background*/
									dont_throw_alerts_alert_on_screen_present = 1;
									REGEN_notstarted_popup_shown = 0;
								}
								else
								{
									REGEN_Required_popup_shown = 0;
									REGEN_aboarted_popup_shown = 0;
									REGEN_completed_popup_shown = 0;
									REGEN_started_popup_shown = 0;
									REGEN_notstarted_popup_shown = 0;
								}
								if((REGEN_requested_popup_shown==0)&&(skip_flag == 1))//need to check use of (REGEN_requested_popup_shown == 0)
								{
									CAN_IC2_100[5].bits.Rx_2 = 1;					//Start CAN TX if Regen req after skipping any alert.
									regen_req_start = 1;							//Start 10 Sec TX timer
								}
							}
							else
							{
								//						REGEN_Required_popup_shown = 0;
								//						REGEN_aboarted_popup_shown = 0;
								REGEN_completed_popup_shown = 0;
								//						REGEN_started_popup_shown = 0;
								//						REGEN_notstarted_popup_shown = 0;
							}
						}
						else
						{
							REGEN_Required_popup_shown = 0;
							REGEN_aboarted_popup_shown = 0;
							REGEN_completed_popup_shown = 0;
							REGEN_started_popup_shown = 0;
							REGEN_notstarted_popup_shown = 0;
						}
					}
					else if(REGEN_bit_data.byte ==2)
					{
						REGEN_priority_flag = 0;
						//				REGEN_TT=telltale_blink_bool3;
						//				DID_TT_bit_data_set6.bits.Rx_4=1;
						alerts_history[7] = 0;
						REGEN_TT_ON_flag = 2;
						Main_page_Regen = 2;//0;		/*Regen*/
						HoldOk_Regen_text = 1;
						IGN_ON_regen_init = 0;
						init_regen_aboarted = 1;	/*Regen Aboarted Alert initiated*/
						init_regen_required = 0;	/*Regen Required Alert disabled*/
						init_regen_completed = 1;	/*Regen Completed Alert initiated*/
						init_regen_not_started = 0;	/*Regen Not started Alert disabled*/

						if((Diag_RW_Regen_Started_warn_Enable == 1)&&(logo_completed==1))
						{
							if(init_regen_started == 1)				/*Regen started Alert Enabled */
							{
								REGEN_priority_flag = 0;
								//							init_regen_started = 0;
								if(((REGEN_started_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0))||
								   ((REGEN_started_popup_shown==0)&&(REGEN_started_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									if(REGEN_started_popup_skip != 1)
									{
										//								init_regen_started = 0;
									}
									//							init_regen_started = 0;
									display_key = drome_getCurrentViewKey();
									if(display_key == 0x11005)
									{
										regen_page_skip = 1;
									}
									else
									{
										regen_page_skip = 0;
									}
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_started);
									REGEN_started_popup_shown = 1;
									Main_page_Regen = 2;//0;		/*Regen*/
									dont_throw_alerts_alert_on_screen_present = 1;
								}
								else
								{
									if(((display_key == 0x11031)&&(REGEN_Required_popup_shown == 1))||((display_key == 0x11033)&&(REGEN_notstarted_popup_shown == 1))||((display_key == 0x11034)&&(REGEN_aboarted_popup_shown == 1))||((display_key == 0x11032)&&(REGEN_requested_popup_shown == 1)))
									{
										start_alart_timeout_5sec=0;
										Priority_flag = 0;
										REGEN_priority_flag = 0;
										dont_throw_alerts_alert_on_screen_present=0;
										(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
									}

									REGEN_Required_popup_shown = 0;
									REGEN_completed_popup_shown = 0;
									REGEN_aboarted_popup_shown = 0;
									REGEN_notstarted_popup_shown = 0;
								}
							}
							else
							{
								if(((display_key == 0x11031)&&(REGEN_Required_popup_shown == 1))||((display_key == 0x11033)&&(REGEN_notstarted_popup_shown == 1))||((display_key == 0x11034)&&(REGEN_aboarted_popup_shown == 1))||((display_key == 0x11032)&&(REGEN_requested_popup_shown == 1)))
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
									//REGEN_Required_popup_shown = 0;
								}
								REGEN_Required_popup_shown = 0;
								REGEN_completed_popup_shown = 0;
								REGEN_aboarted_popup_shown = 0;
								REGEN_notstarted_popup_shown = 0;
							}

						}
						else
						{
							REGEN_Required_popup_shown = 0;
							REGEN_completed_popup_shown = 0;
							REGEN_aboarted_popup_shown = 0;
							REGEN_notstarted_popup_shown = 0;
						}

					}
				}
				else
				{
					REGEN_priority_flag = 0;
				}

			}
			else
			{

				REGEN_TT_ON_flag = 0;
				IGN_ON_regen_init = 0;
				REGEN_priority_flag = 0;
				skip_flag = 0;
				alerts_history[7] = 0;			/*REGEN alert disabled for warning history*/

				if((startkey == 0x11030)||(startkey == 0x11015)||(startkey == 0x11031)||(startkey == 0x11033)||(startkey == 0x11034)||(startkey == 0x11032))
				{
					start_alart_timeout_5sec=0;
					Priority_flag = 0;
					REGEN_priority_flag = 0;
					dont_throw_alerts_alert_on_screen_present=0;
					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
				}
				/****************/
				REGEN_Required_popup_shown = 0;
				REGEN_aboarted_popup_shown = 0;
				REGEN_completed_popup_shown = 0;
				REGEN_started_popup_shown = 0;
				REGEN_notstarted_popup_shown = 0;
				REGEN_requested_popup_shown = 0;
				/**/

				if(startkey == 0x11005)
				{
					Regencnt++;
					if((Regencnt == 1)&&(startkey == 0x11005))
					{
						/*To avoid temp alert skip*/
						(void)drome_sendEventToDrome((U32)drome_KB_DOWN);
					}
				}
				else
				{
					Regencnt = 0;
				}
			}
		}
	}
}
/* ***********************************************************************************************
* Function Name: DEF ALERTS
* Description  : This function used for showing all BS6 - DEF alert functionality
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void DEF_alerts(void)
{
	uint8_t DEF_LEVEL;
	uint8_t INCORRECT_DEF;
	uint8_t DEF_DOSING_MALFUNC;
	Split16 DIST_DEF_EMPTY;
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(EMS29_100_message_timeout_flag == CLEAR && VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
			{
				if(FEATURE_CODE_DEF_TT_CONFIG==1)
				{
					DEF_bit_data.bits.Rx_1 = CAN_EMS29_100[4].bits.Rx_1;
					DEF_bit_data.bits.Rx_2 = CAN_EMS29_100[4].bits.Rx_2;
					DEF_bit_data.bits.Rx_3 = CAN_EMS29_100[4].bits.Rx_3;
					DEF_LEVEL=DEF_bit_data.byte;
					DEF_bit_data.bits.Rx_1 = CAN_EMS29_100[3].bits.Rx_4;
					DEF_bit_data.bits.Rx_2 = CAN_EMS29_100[3].bits.Rx_5;
					DEF_bit_data.bits.Rx_3 = CAN_EMS29_100[3].bits.Rx_6;
					INCORRECT_DEF=DEF_bit_data.byte;
					DEF_bit_data.bits.Rx_1 = CAN_EMS29_100[3].bits.Rx_1;
					DEF_bit_data.bits.Rx_2 = CAN_EMS29_100[3].bits.Rx_2;
					DEF_bit_data.bits.Rx_3 = CAN_EMS29_100[3].bits.Rx_3;
					DEF_DOSING_MALFUNC=DEF_bit_data.byte;
					DIST_DEF_EMPTY.u8[0]=CAN_EMS29_100[1].byte;
					DIST_DEF_EMPTY.u8[1]=CAN_EMS29_100[2].byte;
					Eng_No_Start=DIST_DEF_EMPTY;

					if(logo_completed==1)
					{
						key = drome_getCurrentViewKey();
						if(Diag_RW_Incorrect_DEF_warn_Enable == 1)//as per ALT -75/1 table 1
						{
							if((INCORRECT_DEF==1)&&((DEF_DOSING_MALFUNC>=0)||(DEF_LEVEL>=0))&&(dpf_check == 1))
							{
								if(Incorrect_DEF_Detected_popup_shown == 0)
								{
									DEF_priority_flag = 1;
								}
								/*If permenant alert display present - level 2, level3, level 4*/
								/*LEVEL 2*/												/*LEVEL 3*/									/*LEVEL 4*/
								if((Incorrect_DEF_Detected_Engine_Not_Start_popup_shown == 1)||(No_restart_Check_DEF_popup_shown  == 1)||(No_start_Check_DEF_popup_shown  == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((Incorrect_DEF_Detected_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((Incorrect_DEF_Detected_popup_shown==0)&&(Incorrect_DEF_Detected_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Incorrect_DEF_Detected);
									Incorrect_DEF_Detected_popup_shown = 1;
									if(Incorrect_DEF_Detected_popup_skip == 1)
									{
										Incorrect_DEF_Detected_popup_skip = 0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 0;

									for(uint8_t i = 8;i<12; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[12] = 1;

									for(uint8_t i = 13;i<20; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									//						Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_Engine_Not_Start_popup_skip == 1)||(No_restart_Check_DEF_popup_skip)||(No_start_Check_DEF_popup_skip == 1)||(DEF_Dosing_Malfunction_popup_skip == 1)||
										(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if((INCORRECT_DEF==2)&&((DEF_DOSING_MALFUNC>=0)||(DEF_LEVEL>=0))&&(dpf_check == 1))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level3, level 4*/
								/*LEVEL 1*/												/*LEVEL 3*/									/*LEVEL 4*/
								if((Incorrect_DEF_Detected_popup_shown == 1)||(No_restart_Check_DEF_popup_shown  == 1)||(No_start_Check_DEF_popup_shown  == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((Incorrect_DEF_Detected_Engine_Not_Start_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((Incorrect_DEF_Detected_Engine_Not_Start_popup_shown==0)&&(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									Eng_No_Start=DIST_DEF_EMPTY;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Incorrect_DEF_Detected_Engine_Not_Start);
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=1;
									//r dont_throw_alerts_alert_on_screen_present=1;
									BS6_permanent_timeout = 0;
									if(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip == 1)
									{
										Incorrect_DEF_Detected_Engine_Not_Start_popup_skip=0;
										skip_flag = 0;
									}
									BS6_permanent_skip = 0;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									for(uint8_t i = 8;i<13; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[13] = 1;

									for(uint8_t i = 14;i<20; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									//						Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(No_restart_Check_DEF_popup_skip)||(No_start_Check_DEF_popup_skip == 1)||(DEF_Dosing_Malfunction_popup_skip == 1)||
										(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}

							}
							else if((INCORRECT_DEF==3)&&((DEF_DOSING_MALFUNC>=0)||(DEF_LEVEL>=0))&&(dpf_check == 1))
							{
								DEF_priority_flag = 0;

								/*If permenant alert display present - level 1, level2, level 4*/
								/*LEVEL 1*/												/*LEVEL 2*/													/*LEVEL 4*/
								if((Incorrect_DEF_Detected_popup_shown == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_shown  == 1)||(No_start_Check_DEF_popup_shown  == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((No_restart_Check_DEF_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((No_restart_Check_DEF_popup_shown==0)&&(No_restart_Check_DEF_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									(void)drome_sendEventToDrome((U32)drome_InEvent_No_restart_Check_DEF);
									No_restart_Check_DEF_popup_shown=1;
									if(No_restart_Check_DEF_popup_skip==1)
									{
										skip_flag = 0;
										No_restart_Check_DEF_popup_skip=0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									for(uint8_t i = 8;i<14; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[14] = 1;

									for(uint8_t i = 15;i<20; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									//						No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_start_Check_DEF_popup_skip == 1)||(DEF_Dosing_Malfunction_popup_skip == 1)||
										(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if((INCORRECT_DEF==4)&&((DEF_DOSING_MALFUNC>=0)||(DEF_LEVEL>=0))&&(dpf_check == 1))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level2, level 3*/
								/*LEVEL 1*/												/*LEVEL 2*/													/*LEVEL 3*/
								if((Incorrect_DEF_Detected_popup_shown == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_shown  == 1)||(No_restart_Check_DEF_popup_shown  == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}

								if(((No_start_Check_DEF_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((No_start_Check_DEF_popup_shown==0)&&(No_start_Check_DEF_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									(void)drome_sendEventToDrome((U32)drome_InEvent_No_start_Check_DEF);
									No_start_Check_DEF_popup_shown=1;
									if(No_start_Check_DEF_popup_skip == 1)
									{
										No_start_Check_DEF_popup_skip=0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									for(uint8_t i = 8;i<15; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[15] = 1;

									for(uint8_t i = 16;i<20; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									//						No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(DEF_Dosing_Malfunction_popup_skip == 1)||
										(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else
							{
								//clear alert from screen
								if((key == 0x22042)||(key == 0x22043)||(key == 0x22044))//incorrect DEF
								{
									/*(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);*/
									(void)drome_sendEventToDrome((U32)drome_InEvent_BS6_permanent_alert_inactive);
									dont_throw_alerts_alert_on_screen_present = 0;
									dont_throw_alerts_BS6_alert_on_screen_present = 0;
								}
								if((key == 0x22041)&&((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))&&((DEF_DOSING_MALFUNC>=0)||(DEF_LEVEL>=0)))
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									Incorrect_DEF_Detected_popup_shown = 0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
								}
							}
						}
						else
						{
							DEF_priority_flag = 0;
							//do nothing
						}
						if(Diag_RW_DEF_Dosing_warn_Enable == 1)//as per ALT -75/1 table 2
						{
							if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))&&(DEF_DOSING_MALFUNC==1)&&(dpf_check == 1))//(INCORRECT_DEF=reserved)&&(DEF_DOSING_MALFUNC==1)
								//				else if((DEF_DOSING_MALFUNC==1)&&((INCORRECT_DEF>=0)||(DEF_LEVEL>=0)))//INCORRECT_DEF=reserved && Rajat else
							{ 							/*If permenant alert display present - level 2, level3, level 4*/
								if(DEF_Dosing_Malfunction_popup_shown == 0)
								{
									DEF_priority_flag = 1;
								}
								/*DEF LEVEL 2*/												/*DEF LEVEL 3*/						/*DEF LEVEL 4*/
								if((DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown == 1)||(No_restart_Dose_Malfunction_popup_shown == 1)||(No_start_Dose_Malfunction_popup_shown == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((DEF_Dosing_Malfunction_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((DEF_Dosing_Malfunction_popup_shown==0)&&(DEF_Dosing_Malfunction_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_DEF_Dosing_Malfunction);
									DEF_Dosing_Malfunction_popup_shown = 1;
									if(DEF_Dosing_Malfunction_popup_skip == 1)
									{
										DEF_Dosing_Malfunction_popup_skip = 0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 0;
									alerts_history[8] = 0;
									alerts_history[9] = 0;
									for(uint8_t i = 8;i<16; i++)	/*DEF Dosing all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[16] = 1;

									for(uint8_t i = 17;i<20; i++)	/*DEF Dosing all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}

								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									//						DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))&&(DEF_DOSING_MALFUNC==2)&&(dpf_check == 1))
								//				else if((DEF_DOSING_MALFUNC==2)&&((INCORRECT_DEF>=0)||(DEF_LEVEL>=0)))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level3, level 4*/
								/* LEVEL 1*/									/* LEVEL 3*/						/*LEVEL 4*/
								if((DEF_Dosing_Malfunction_popup_shown == 1)||(No_restart_Dose_Malfunction_popup_shown == 1)||(No_start_Dose_Malfunction_popup_shown == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown==0)&&(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									Eng_No_Start=DIST_DEF_EMPTY;
									(void)drome_sendEventToDrome((U32)drome_InEvent_DEF_Dosing_Malfunction_Engine_Not_Start);
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=1;
									//r dont_throw_alerts_alert_on_screen_present = 1;
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									if(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)
									{
										DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
										skip_flag = 0;
									}
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									for(uint8_t i = 8;i<17; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[17] = 1;

									for(uint8_t i = 18;i<20; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									//						DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))&&(DEF_DOSING_MALFUNC==3)&&(dpf_check == 1))
								//				else if((DEF_DOSING_MALFUNC==3)&&((INCORRECT_DEF>=0)||(DEF_LEVEL>=0)))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level2, level 4*/
								/* LEVEL 1*/											/* LEVEL 2*/										/*LEVEL 4*/
								if((DEF_Dosing_Malfunction_popup_shown == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown == 1)||(No_start_Dose_Malfunction_popup_shown == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((No_restart_Dose_Malfunction_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((No_restart_Dose_Malfunction_popup_shown==0)&&(No_restart_Dose_Malfunction_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									(void)drome_sendEventToDrome((U32)drome_InEvent_No_restart_Dose_Malfunction);
									No_restart_Dose_Malfunction_popup_shown=1;
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									if(No_restart_Dose_Malfunction_popup_skip == 1)
									{
										No_restart_Dose_Malfunction_popup_skip = 0;
										skip_flag = 0;
									}
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									for(uint8_t i = 8;i<18; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[18] = 1;

									for(uint8_t i = 19;i<20; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									//						No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))&&(DEF_DOSING_MALFUNC==4)&&(dpf_check == 1))
								//				else if((DEF_DOSING_MALFUNC==4)&&((INCORRECT_DEF>=0)||(DEF_LEVEL>=0)))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level2, level 3*/
								/* LEVEL 1*/											/* LEVEL 2*/										/*LEVEL 3*/
								if((DEF_Dosing_Malfunction_popup_shown == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown == 1)||(No_restart_Dose_Malfunction_popup_shown == 1))
								{
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								if(((No_start_Dose_Malfunction_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((No_start_Dose_Malfunction_popup_shown==0)&&(No_start_Dose_Malfunction_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									(void)drome_sendEventToDrome((U32)drome_InEvent_No_start_Dose_Malfunction);
									No_start_Dose_Malfunction_popup_shown=1;
									if(No_start_Dose_Malfunction_popup_skip == 1)
									{
										No_start_Dose_Malfunction_popup_skip = 0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									for(uint8_t i = 8;i<19; i++)	/*DEF all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
									alerts_history[19] = 1;
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									//						No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(DEF_Level_Low_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else
							{
								//clear alert from screen
								if((key == 0x22046)||(key == 0x22047)||(key == 0x22048))//Dosing Malfunction
								{
									/*(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);*/
									(void)drome_sendEventToDrome((U32)drome_InEvent_BS6_permanent_alert_inactive);
									dont_throw_alerts_alert_on_screen_present = 0;
									dont_throw_alerts_BS6_alert_on_screen_present = 0;
								}

								if((key == 0x22045)&&((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7)))
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									DEF_Dosing_Malfunction_popup_shown = 0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
								}
							}
						}
						else
						{
							DEF_priority_flag = 0;
							//do nothing
						}
						if(Diag_RW_DEF_Level_warn_Enable == 1)//as per ALT -75/1 table 3
						{
							if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))
									&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7))
									&&(DEF_LEVEL==1)&&(dpf_check == 1))
								//				else if ((DEF_LEVEL==1)&&((INCORRECT_DEF>=0)||(DEF_DOSING_MALFUNC>=0)))
							{	/*If permenant alert display present - level 2, level3, level 4*/
								DEF_priority_check = 1;
								if((DEF_Level_Low_popup_shown == 0)&&(DEF_priority_check == 1)&&(dont_throw_alerts_alert_on_screen_present==0))
								{
									DEF_priority_flag = 1;
								}
								/*DEF LEVEL 2*/												/*DEF LEVEL 3*/						/*DEF LEVEL 4*/
								if((DEF_level_Low_Engine_Not_Start_popup_shown == 1) || (No_restart_Fill_DEF_popup_shown == 1)|| (No_start_Fill_DEF_popup_shown == 1))
								{
									/*dont_throw_alerts_BS6_alert_on_screen_present=0;*/
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								/********/
								if(((DEF_Level_Low_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((DEF_Level_Low_popup_shown==0)&&(DEF_Level_Low_popup_skip==1)/*&&(dont_throw_alerts_alert_on_screen_present==0)*/))
								{
									DEF_priority_flag = 1;
									Priority_flag = 1;
									(void)drome_sendEventToDrome((U32)drome_InEvent_DEF_Level_Low);
									DEF_Level_Low_popup_shown = 1;
									if(DEF_Level_Low_popup_skip == 1)
									{
										DEF_Level_Low_popup_skip = 0;
										skip_flag = 0;
									}
									dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 0;

									alerts_history[8] = 1;
									for(uint8_t i = 9;i<20; i++)	/*DEF LOW all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									//						DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||
										(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))
									&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7))
									&&(DEF_LEVEL==2)&&(dpf_check == 1))
								//				 else if((DEF_LEVEL==2)&&((INCORRECT_DEF>=0)||(DEF_DOSING_MALFUNC>=0)))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level3, level 4*/
								/*DEF LEVEL 1*/								/*DEF LEVEL 3*/						 /*DEF LEVEL 4*/
								if((DEF_Level_Low_popup_shown == 1) || (No_restart_Fill_DEF_popup_shown == 1)|| (No_start_Fill_DEF_popup_shown == 1))
								{
									//dont_throw_alerts_BS6_alert_on_screen_present=0;
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								/********/
								if(((DEF_level_Low_Engine_Not_Start_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((DEF_level_Low_Engine_Not_Start_popup_shown==0)&&(DEF_level_Low_Engine_Not_Start_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									Eng_No_Start=DIST_DEF_EMPTY;
									(void)drome_sendEventToDrome((U32)drome_InEvent_DEF_level_Low_Engine_Not_Start);
									DEF_level_Low_Engine_Not_Start_popup_shown=1;
									if(DEF_level_Low_Engine_Not_Start_popup_skip == 1)
									{
										DEF_level_Low_Engine_Not_Start_popup_skip=0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									alerts_history[8] = 0;
									alerts_history[9] = 1;
									for(uint8_t i = 10;i<20; i++)	/*DEF LOW all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									//						DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;

								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||
										(DEF_Level_Low_popup_skip == 1)||(No_restart_Fill_DEF_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))
									&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7))
									&&(DEF_LEVEL==3)&&(dpf_check == 1))
								//				else if((DEF_LEVEL==3)&&((INCORRECT_DEF>=0)||(DEF_DOSING_MALFUNC>=0)))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level2, level 4*/
								/*DEF LEVEL 1*/								/*DEF LEVEL 2*/						 /*DEF LEVEL 4*/
								if((DEF_Level_Low_popup_shown == 1) || (DEF_level_Low_Engine_Not_Start_popup_shown == 1)|| (No_start_Fill_DEF_popup_shown == 1))
								{
									//dont_throw_alerts_BS6_alert_on_screen_present=0;
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								/********/
								if(((No_restart_Fill_DEF_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((No_restart_Fill_DEF_popup_shown==0)&&(No_restart_Fill_DEF_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									(void)drome_sendEventToDrome((U32)drome_InEvent_No_restart_Fill_DEF);
									No_restart_Fill_DEF_popup_shown=1;
									if(No_restart_Fill_DEF_popup_skip == 1)
									{
										No_restart_Fill_DEF_popup_skip=0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									alerts_history[8] = 0;
									alerts_history[9] = 0;
									alerts_history[10] = 1;
									for(uint8_t i = 11;i<20; i++)	/*DEF LOW all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									//						No_restart_Fill_DEF_popup_shown=0;
									No_start_Fill_DEF_popup_shown=0;
								}

								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||
										(DEF_Level_Low_popup_skip == 1)||(DEF_level_Low_Engine_Not_Start_popup_skip == 1)||(No_start_Fill_DEF_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))
									&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7))
									&&(DEF_LEVEL==4)&&(dpf_check == 1))
								//				else if((DEF_LEVEL==4)&&((INCORRECT_DEF>=0)||(DEF_DOSING_MALFUNC>=0)))
							{
								DEF_priority_flag = 0;
								/*If permenant alert display present - level 1, level2, level 4*/
								/*DEF LEVEL 1*/									/*DEF LEVEL 2*/						 			/*DEF LEVEL 3*/
								if((DEF_Level_Low_popup_shown == 1) || (DEF_level_Low_Engine_Not_Start_popup_shown == 1)|| (No_restart_Fill_DEF_popup_shown == 1))
								{
									//dont_throw_alerts_BS6_alert_on_screen_present=0;
									BS6_permanent_alert = 1;
									dont_throw_alerts_alert_on_screen_present = 0;
								}
								else
								{
									BS6_permanent_alert = 0;
								}
								/********/
								if(((No_start_Fill_DEF_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((dont_throw_alerts_BS6_alert_on_screen_present==0)||(BS6_permanent_alert == 1))&&(skip_flag == 0))||
										((No_start_Fill_DEF_popup_shown==0)&&(No_start_Fill_DEF_popup_skip==1)&&(dont_throw_alerts_alert_on_screen_present==0)&&(BS6_permanent_timeout == 1)))
								{
									(void)drome_sendEventToDrome((U32)drome_InEvent_No_start_Fill_DEF);
									No_start_Fill_DEF_popup_shown=1;
									if(No_start_Fill_DEF_popup_skip == 1)
									{
										No_start_Fill_DEF_popup_skip = 0;
										skip_flag = 0;
									}
									BS6_permanent_timeout = 0;
									BS6_permanent_skip = 0;
									//r dont_throw_alerts_alert_on_screen_present=1;
									dont_throw_alerts_BS6_alert_on_screen_present = 1;

									alerts_history[8] = 0;
									alerts_history[9] = 0;
									alerts_history[10] = 0;
									alerts_history[11] = 1;
									for(uint8_t i = 12;i<20; i++)	/*DEF LOW all other alert disabled for warning history*/
									{
										alerts_history[i] = 0;
									}
								}
								else
								{
									Incorrect_DEF_Detected_popup_shown=0;
									Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
									No_restart_Check_DEF_popup_shown=0;
									No_start_Check_DEF_popup_shown=0;
									DEF_Dosing_Malfunction_popup_shown=0;
									DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
									No_restart_Dose_Malfunction_popup_shown=0;
									No_start_Dose_Malfunction_popup_shown=0;
									DEF_Level_Low_popup_shown=0;
									DEF_level_Low_Engine_Not_Start_popup_shown=0;
									No_restart_Fill_DEF_popup_shown=0;
								}
								if((Incorrect_DEF_Detected_popup_skip == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_skip)||(No_restart_Check_DEF_popup_skip == 1)||(No_start_Check_DEF_popup_skip == 1)||
										(DEF_Dosing_Malfunction_popup_skip == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip == 1)||(No_restart_Dose_Malfunction_popup_skip == 1)||(No_start_Dose_Malfunction_popup_skip == 1)||
										(DEF_Level_Low_popup_skip == 1)||(DEF_level_Low_Engine_Not_Start_popup_skip == 1))
								{
									skip_flag = 0;
								}
							}
							else
							{
								//clear alert from screen
								if((key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
								{
									/*(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);*/
									(void)drome_sendEventToDrome((U32)drome_InEvent_BS6_permanent_alert_inactive);
									dont_throw_alerts_alert_on_screen_present = 0;
									dont_throw_alerts_BS6_alert_on_screen_present = 0;
								}

								if((key == 0x22049)&&((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))
										&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7))
										&&((DEF_LEVEL==0)||(DEF_LEVEL==5)||(DEF_LEVEL==6)||(DEF_LEVEL==7)))	/*key = 0x22049 - DEF LEVEL LOW*/
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									DEF_Level_Low_popup_shown = 0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
								}
							}
						}
						else
						{
							DEF_priority_flag = 0;
							//do nothing
						}
						if(((INCORRECT_DEF==0)||(INCORRECT_DEF==5)||(INCORRECT_DEF==6)||(INCORRECT_DEF==7))
								&&((DEF_DOSING_MALFUNC==0)||(DEF_DOSING_MALFUNC==5)||(DEF_DOSING_MALFUNC==6)||(DEF_DOSING_MALFUNC==7))
								&&((DEF_LEVEL==0)||(DEF_LEVEL==5)||(DEF_LEVEL==6)||(DEF_LEVEL==7)))	/*All Reserved*/
						{
							Incorrect_DEF_Detected_popup_shown=0;
							Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
							No_restart_Check_DEF_popup_shown=0;
							No_start_Check_DEF_popup_shown=0;
							DEF_Dosing_Malfunction_popup_shown=0;
							DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
							No_restart_Dose_Malfunction_popup_shown=0;
							No_start_Dose_Malfunction_popup_shown=0;
							DEF_Level_Low_popup_shown=0;
							DEF_level_Low_Engine_Not_Start_popup_shown=0;
							No_restart_Fill_DEF_popup_shown=0;
							No_start_Fill_DEF_popup_shown=0;
							DEF_priority_flag = 0;
							/*clear alert from screen will implement after test*/

							/*	dont_throw_alerts_alert_on_screen_present = 0;*/
							dont_throw_alerts_BS6_alert_on_screen_present = 0;

							for(uint8_t i = 8;i<20; i++)	/*DEF all other alert disabled for warning history*/
							{
								alerts_history[i] = 0;
							}
						}
						else
						{
							//do nothing
						}
					}
				}
				else
				{
					DEF_priority_flag = 0;
				}
			}
			else
			{
				DEF_priority_flag = 0;
				/*Cleare alert can missing */
				Incorrect_DEF_Detected_popup_shown=0;
				Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;
				No_restart_Check_DEF_popup_shown=0;
				No_start_Check_DEF_popup_shown=0;
				DEF_Dosing_Malfunction_popup_shown=0;
				DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;
				No_restart_Dose_Malfunction_popup_shown=0;
				No_start_Dose_Malfunction_popup_shown=0;
				DEF_Level_Low_popup_shown=0;
				DEF_level_Low_Engine_Not_Start_popup_shown=0;
				No_restart_Fill_DEF_popup_shown=0;
				No_start_Fill_DEF_popup_shown=0;
				/*dont_throw_alerts_alert_on_screen_present = 0;*/
				dont_throw_alerts_BS6_alert_on_screen_present = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);

				Incorrect_DEF_Detected_popup_skip=0;
				Incorrect_DEF_Detected_Engine_Not_Start_popup_skip=0;
				No_restart_Check_DEF_popup_skip=0;
				No_start_Check_DEF_popup_skip=0;
				DEF_Dosing_Malfunction_popup_skip=0;
				DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip=0;
				No_restart_Dose_Malfunction_popup_skip=0;
				No_start_Dose_Malfunction_popup_skip=0;
				DEF_Level_Low_popup_skip=0;
				DEF_level_Low_Engine_Not_Start_popup_skip=0;
				No_restart_Fill_DEF_popup_skip=0;
				No_start_Fill_DEF_popup_skip=0;
				skip_flag = 0;
				for(uint8_t i = 8;i<20; i++)	/*DEF all other alert disabled for warning history*/
				{
					alerts_history[i] = 0;
				}
				/****************/
			}
		}
	}
}


/* ***********************************************************************************************
* Function Name: ETM_LED_test
* Description  : This function used for ETM_LED_test
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void ETM_LED_test(void)
{
	uint32_t key;
	key = drome_getCurrentViewKey();
	while(key==0x23003)
	{
		Check_TT_ETM_Mode();
		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
		IO_Parameter_Request_Status = 4;
		if((set_key_short == 1)||(Up_key_Short == 1)||(Down_key_Short == 1)||(ETM_10_sec_exit_flag==1))
		{
			set_key_short = 0;
			Up_key_Short = 0;
			Down_key_Short = 0;
			IO_Parameter_Request_Status = 0;
			break;
		}
	}
}

/* ***********************************************************************************************
* Function Name: ETM_Guage_Test
* Description  : This function used for ETM_Guage_Test
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void ETM_Guage_Test(void)
{
	uint32_t key;
	key = drome_getCurrentViewKey();
	while(key == 0x23001)//0x23001
	{
		if(Proveout_Count < PROVEOUT_UP) /*wait for 15ms x 70 (PROVEOUT_UP), which is around 1sec*/
		{
			display_speedometer_speed=176;
			final_vechile_speed_gauge_raw=176*64;
			display_tachometer_speed=6950;
			Proveout_Count++;
		}
		else if(Proveout_Count < PROVEOUT_DOWN)/*wait for 15ms x 70 (PROVEOUT_DOWN), which is around 1sec*/
		{
			/*if((Proveout_Count == PROVEOUT_UP)&&(BATT_DISCONNECTION==1))
				{
					ISM0VAR4CFG0=5120000;
					ISM0VAR4CFG1=5120000;
				}
			 */
			display_speedometer_speed=0;
			final_vechile_speed_gauge_raw=0;
			display_tachometer_speed=0;
			Proveout_Count++;
			//All_Telltale_OFF();
		}
		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
		if((set_key_short == 1)||(Up_key_Short == 1)||(Down_key_Short == 1)||(ETM_10_sec_exit_flag==1))
		{
			set_key_short = 0;
			Up_key_Short = 0;
			Down_key_Short = 0;
			break;
		}
	}
}

/* ***********************************************************************************************
* Function Name: RPAS_Precondition_U350
* Description  : This function used for showing RPAS Precondition in U350.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Precondition_U350(void)
{
#ifdef U350
	if(IGN_ON_flag == 1)
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(FEATURE_CODE_RPAS_IND == 1)
			{
				if(Gear_config == 1)		/*MT*/
				{
					if(EMS1_10_message_timeout_flag == 0)	/*EMS1_10 CAN Message Missing/Message Timeout*/
					{
						if(((CAN_EMS1_10[5].byte)& 0x0F) == 0x07)		/*MT - Reverse gear Detected*/
						{
							RPAS_Precondition_Active = 1;
							staybacktime_active = 1;
						}
						else
						{
							RPAS_Precondition_Active = 0;
						}
					}
					else
					{
						RPAS_Precondition_Active = 0;
					}
				}
				else if(Gear_config == 2)	/*AT*/
				{
					if(TCU6_20_message_timeout_flag == 0)
					{
						if(((CAN_TCU6_20[2].byte) & 0x0F) == 0x08)		/*AT - Reverse gear Detected*/
						{
							AT_Reverse_detected = 1;
							if((AT_Reversegear_timespan*10) >= Diag_RW_AT_Reverse_Gear_Debounce_Time)
							{
								RPAS_Precondition_Active = 1;
								staybacktime_active = 1;
							}
							else
							{
//								RPAS_Precondition_Active = 0;
							}
						}
						else
						{
							RPAS_Precondition_Active = 0;
							AT_Reverse_detected = 0;
							AT_Reversegear_timespan=0;/*added for testing debounce time related error*/
						}
					}
					else
					{
						RPAS_Precondition_Active = 0;
						AT_Reverse_detected = 0;
					}
				}
			}
			else
			{
				RPAS_Precondition_Active = 0;
				AT_Reverse_detected = 0;
			}
		}
		else
		{
			//if(RPAS_LEVEL_1_BUZ  ==0 && RPAS_LEVEL1_PRECON == 1)
			{
				if(Sound_Bit == 0x10000)
				{
					RPAS_LEVEL1_PRECON = 0;
					Sound_Bit = 0x0;
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
		}
	}
#endif
}
/* ***********************************************************************************************
* Function Name: RPAS_Distance_Display_U350
* Description  : This function used for showing RPAS Distance Display in U350.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Distance_Display_U350(void)
{

	uint8_t STS_RPAS_DISP_data = 0;
	uint8_t STS_RPAS_DISP_bit5 = 5;
	_Bool PREV_RPAS_DIST_text_display = 0;

	RPAS_display_dist = CAN_RPAS1_20[7].byte;

	STS_RPAS_DISP_data = CAN_RPAS2_100[0].byte;
	STS_RPAS_DISP_bit5 = ((STS_RPAS_DISP_data >> STS_RPAS_DISP_bit5) & 1);

	if(RPAS_ERRORtext_display == 1)
	{
		RPAS_DIST_text_display = 1;				/*Display distance - Error display*/
	}
	else
	{
		if((RPAS_display_dist == 0x0)||(RPAS_display_dist == 0xFF))
		{
			RPAS_DIST_text_display = 3;			/*No distance display*/
		}
		else if((RPAS_display_dist >= 0xFB)&&(RPAS_display_dist <= 0xFE))
		{
			RPAS_DIST_text_display = 3;			/*No display of distance - Greater than 250 cm*/
		}

		else if((RPAS_display_dist > 0x00)&&(RPAS_display_dist <= 0xFB)&&(STS_RPAS_DISP_bit5 != 0x01))
		{
			RPAS_DIST_text_display = 3;			/*No display of distance - Greater than 250 cm*/
		}

		if(STS_RPAS_DISP_bit5 == 0x01)
		{
			RPAS__update_display_dist = RPAS_display_dist;
			if((RPAS__update_display_dist >= 0x1)&&(RPAS__update_display_dist <= Diag_RW_Stop_Dist))
			{
				RPAS_DIST_text_display = 2;		/*STOP display*/
			}
			else if((Diag_RW_Stop_Dist < RPAS__update_display_dist)&&(RPAS__update_display_dist < 0xFB))
			{
				RPAS_DIST_text_display = 0;		/*distance info display*/
			}
		}
		else
		{
			PREV_RPAS_DIST_text_display = RPAS_DIST_text_display;
			if(PREV_RPAS_DIST_text_display != 1)
			{
				RPAS_DIST_text_display = PREV_RPAS_DIST_text_display;
			}
		}
	}
}
/* ***********************************************************************************************
* Function Name: RPAS_Status_Display_U350
* Description  : This function used for showing RPAS status display - Sector / Zone configuration in U350.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Status_Display_U350(void)
{
	uint8_t STS_RPAS_DISP_data = 0;
	uint8_t STS_RPAS_DISP_bit5 = 5;
	uint8_t BAR_SECT_LEFT = 0;
	uint8_t BAR_SECT_RIGHT = 0;
	uint8_t BAR_SECT_CENTER = 0;


	STS_RPAS_DISP_data = CAN_RPAS2_100[0].byte;
	STS_RPAS_DISP_bit5 = ((STS_RPAS_DISP_data >> STS_RPAS_DISP_bit5) & 1);


	if(STS_RPAS_DISP_bit5 == 1)
	{
		BAR_SECT_LEFT = CAN_RPAS1_20[2].byte;
		BAR_SECT_LEFT = ((BAR_SECT_LEFT & 0xF0) >> 4);

		BAR_SECT_RIGHT = CAN_RPAS1_20[3].byte;
		BAR_SECT_RIGHT = ((BAR_SECT_RIGHT & 0xF0) >> 4);

		BAR_SECT_CENTER = CAN_RPAS1_20[3].byte;
		BAR_SECT_CENTER = (BAR_SECT_CENTER & 0x0F);

		if((RPAS_ERROR != 0x01)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x05)&&(RPAS_ERROR != 0x07)&&(RPAS_ERROR != 0x09))
		{
			switch (BAR_SECT_LEFT)
			{
			case 0x01:
				RPAS_RLeft_imageIndex = 1; /*RL BAR 1,2,3 - OFF | BAR 4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RLeft_imageIndex = 2; /*RL BAR 1,2 - OFF | BAR 3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RLeft_imageIndex = 3; /*RL BAR 1 - OFF | BAR 2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RLeft_imageIndex = 4; /*RL  BAR 1,2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RLeft_imageIndex = 0; /*RL BAR 1,2,3,4 - OFF*/
					RPAS_All_sensor_error = 0;
			break;
			}
		}
		if((RPAS_ERROR != 0x08)&&(RPAS_ERROR != 0x09)&&(RPAS_ERROR != 0x10)&&(RPAS_ERROR != 0x12)&&(RPAS_ERROR != 0x14))
		{
			switch (BAR_SECT_RIGHT)
			{
			case 0x01:
				RPAS_RRight_imageIndex = 1;/*RR BAR 1,2,3 - OFF | BAR 4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RRight_imageIndex = 2; /*RR BAR 1,2 - OFF | BAR 3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RRight_imageIndex = 3; /*RR BAR 1 - OFF | BAR 2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RRight_imageIndex = 4; /*RR  BAR 1,2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RRight_imageIndex = 0; /*RL BAR 1,2,3,4 - OFF*/
					RPAS_All_sensor_error = 0;

			break;
			}
		}
		if((RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x05)&&(RPAS_ERROR != 0x07)&&(RPAS_ERROR != 0x0A/*0x10*/)&&(RPAS_ERROR != 0x0C/*0x12*/)
				&&(RPAS_ERROR != 0x0E/*0x14*/)&&(RPAS_ERROR != 0x02)&&(RPAS_ERROR != 0x04)&&(RPAS_ERROR != 0x06))
		{
			switch (BAR_SECT_CENTER)
			{
			case 0x01:
				RPAS_RCenter_imageIndex = 1;/*RR BAR 1,2,3 - OFF | BAR 4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RCenter_imageIndex = 2; /*RR BAR 1,2 - OFF | BAR 3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RCenter_imageIndex = 3; /*RR BAR 1 - OFF | BAR 2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RCenter_imageIndex = 4; /*RR  BAR 1,2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RCenter_imageIndex = 0; /*RL BAR 1,2,3,4 - OFF*/
					RPAS_All_sensor_error = 0;
			break;
			}
		}


	}
	else
	{
		if(RPAS_RLeft_imageIndex != 5)
		{
			RPAS_RLeft_imageIndex = 0;	/*ALL Sectors - all bar off*/
		}

		if(RPAS_RRight_imageIndex != 5)
		{
			RPAS_RRight_imageIndex = 0;	/*ALL Sectors - all bar off*/
		}

		if(RPAS_RCenter_imageIndex != 5)
		{
			RPAS_RCenter_imageIndex = 0;	/*ALL Sectors - all bar off*/
		}


	}


}
/************************************************************************************************
* Function Name: RPAS_Screen_Activation_U350
* Description  : This function used for showing RPAS_Screen_Activation_U350.
* 				 RPAS Screen Activation in 3 stages
* 				 1. Error Screen in CAN Missing
* 				 2.	Error Screen in CAN Present
* 				 3.	RPAS Status activation
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Screen_Activation_U350(void)
{
	uint8_t RPAS2_100_ERROR_data = 0;

	uint8_t RPAS2_100_ERROR_bit0 = 0;
	uint8_t RPAS2_100_ERROR_bit1 = 1;
	uint8_t RPAS2_100_ERROR_bit2 = 2;
	uint8_t RPAS2_100_ERROR_bit3 = 3;
	uint8_t RPAS2_100_ERROR_bit4 = 4;

	RPAS_Precondition_U350();
	if((Diag_RW_RPAS_Error_Warn_Enable == 1)&&(RPAS2_100_message_timeout_flag != 0))
	{
		if(RPAS_Precondition_Active == 1)
		{

			if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
				RPAS_ACTIVE_popup_shown=1;
				dont_throw_alerts_alert_on_screen_present=1;
			}

			if(RPAS_ACTIVE_popup_shown == 1)
			{
				RPAS_ERRORtext_display = 1;		/*Error condition detected*/
//				RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
				RPAS_Distance_Display_U350();
				RPAS_Error_detected = 1;
				RPAS_RLeft_imageIndex = 5;
				RPAS_RRight_imageIndex = 5;
				RPAS_RCenter_imageIndex = 5;
			}
			else
			{
				RPAS_Error_detected = 0;
			}

		}
		else
		{
			if(staybacktime_active == 1)
			{

				if(staybacktime_count <= Diag_RW_RPASStayBackTimeVal)
				{

					if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
						RPAS_ACTIVE_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
					}

					if(RPAS_ACTIVE_popup_shown == 1)
					{
						/*all Error condition detected*/
						RPAS_ERRORtext_display = 1;
//						RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
						RPAS_RLeft_imageIndex = 5;
							RPAS_RRight_imageIndex = 5;
							RPAS_RCenter_imageIndex = 5;
						RPAS_Distance_Display_U350();
					}
				}
				else
				{
					if(RPAS_ACTIVE_popup_shown == 1)
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
						RPAS_ACTIVE_popup_shown=0;
						dont_throw_alerts_alert_on_screen_present=0;
						staybacktime_count = 0;
						staybacktime_active = 0;
					}
				}

			}

			if(RPAS_Error_detected == 1)
			{
				if(RPAS_ACTIVE_popup_shown == 1)
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
					RPAS_ACTIVE_popup_shown=0;
					dont_throw_alerts_alert_on_screen_present=0;
					staybacktime_count = 0;
					staybacktime_active = 0;
				}
				RPAS_Error_detected = 0;
			}

		}



		/*Error screen Activate*/
	}
	else if((Diag_RW_RPAS_Error_Warn_Enable == 1)&&(RPAS2_100_message_timeout_flag == 0))
	{
		if(RPAS_Precondition_Active == 1)
		{
			RPAS2_100_ERROR_data = CAN_RPAS2_100[0].byte;
			RPAS_ERROR = (RPAS2_100_ERROR_data) & 0x1F;

			if((RPAS_ERROR == 0x10)||(RPAS_ERROR == 0xF)||(RPAS_ERROR == 0xB)||(RPAS_ERROR == 0xD))
			{
				if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
					RPAS_ACTIVE_popup_shown=1;
					dont_throw_alerts_alert_on_screen_present=1;
				}

				if(RPAS_ACTIVE_popup_shown == 1)
				{
					RPAS_ERRORtext_display = 1;		/*All Signal Error condition detected*/
//					RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
					RPAS_RLeft_imageIndex = 5;
						RPAS_RRight_imageIndex = 5;
						RPAS_RCenter_imageIndex = 5;

					RPAS_Distance_Display_U350();
				}
			}
		}
		else
		{
			if(RPAS_ACTIVE_popup_shown == 1)
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
				RPAS_ACTIVE_popup_shown=0;
				dont_throw_alerts_alert_on_screen_present=0;
				staybacktime_active = 0;
			}
		}
	}

	if((Diag_RW_PAS_Status_Warn_Enable == 1)&&(RPAS2_100_message_timeout_flag == 0))
	{
		if(RPAS_Precondition_Active == 1)
		{
			RPAS2_100_ERROR_data = CAN_RPAS2_100[0].byte;
			RPAS_ERROR = (RPAS2_100_ERROR_data) & 0x1F;

			if((RPAS_ERROR != 0x10)&&(RPAS_ERROR != 0xF)&&(RPAS_ERROR != 0xB)&&(RPAS_ERROR != 0xD))
			{

				if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
					RPAS_ACTIVE_popup_shown=1;
					dont_throw_alerts_alert_on_screen_present=1;
				}
				if(RPAS_ACTIVE_popup_shown == 1)
				{
					RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/

					if(RPAS_ERROR == 0x00)
					{
						RPAS_ERRORtext_display = 0;
						RPAS_RLeft_imageIndex = 0; /*RL Sensor Not Fail -ERROR*/
						RPAS_RRight_imageIndex = 0;/*RR Sensor Not Fail -ERROR*/
						RPAS_RCenter_imageIndex = 0;/*RC Sensor Not Fail -ERROR*/
					}
					else
					{
						RPAS_ERRORtext_display = 1;
						if((RPAS_ERROR >= 0x10)&&(RPAS_ERROR <= 0x1F))
						{
							RPAS_ERRORtext_display = 1;
//							RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
							RPAS_RLeft_imageIndex = 5;
							RPAS_RRight_imageIndex = 5;
							RPAS_RCenter_imageIndex = 5;
						}
						else
						{

						if((RPAS_ERROR == 0x01)||(RPAS_ERROR == 0x03)||(RPAS_ERROR == 0x05)||(RPAS_ERROR == 0x07)||(RPAS_ERROR == 0x09))
						{
							RPAS_RLeft_imageIndex = 5; /*RL Sensor Fail -ERROR*/
							RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
						}
						else
						{
							RPAS_RLeft_imageIndex = 0; /*RL Sensor Fail -ERROR*/
						}
						if((RPAS_ERROR == 0x08)||(RPAS_ERROR == 0x09)||(RPAS_ERROR == 0x10)||(RPAS_ERROR == 0x12)||(RPAS_ERROR == 0x14))
						{
							RPAS_RRight_imageIndex = 5;/*RR Sensor Fail -ERROR*/
							RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
						}
						else
						{
							RPAS_RRight_imageIndex = 0;/*RR Sensor Fail -ERROR*/
						}
							if((RPAS_ERROR == 0x03)||(RPAS_ERROR == 0x05)||(RPAS_ERROR == 0x07)||(RPAS_ERROR == 0x0A/*0x10*/)||(RPAS_ERROR == 0x0C/*0x12*/)
									||(RPAS_ERROR == 0x0E/*0x14*/)||(RPAS_ERROR == 0x02)||(RPAS_ERROR == 0x04)||(RPAS_ERROR == 0x06))	/*Bug in v209 is solved - ERROR not display to 0x0A,0x0C,0x0E*/
						{
							RPAS_RCenter_imageIndex = 5;/*RC Sensor Fail -ERROR*/
							RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
						}
						else
						{
							RPAS_RCenter_imageIndex = 0;/*RC Sensor Fail -ERROR*/
							}
						}

					}
					/*RPAS condition Activated*/
					RPAS_Status_Display_U350();
					RPAS_Distance_Display_U350();
				}

			}

		}
		else
		{
			if(RPAS_ACTIVE_popup_shown == 1)
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
				RPAS_ACTIVE_popup_shown=0;
				dont_throw_alerts_alert_on_screen_present=0;
				staybacktime_active = 0;
			}
		}
	}
}
/* ***********************************************************************************************
* Function Name: RPAS_Precondition_W501
* Description  : This function used for showing RPAS Precondition in W501.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Precondition_W501(void)
{
	_Bool RPAS_ACTIVE_STS = 0U;

	if(IGN_ON_flag == 1)
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if((FEATURE_CODE_RPAS_IND == 1)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1))
			{
				/*Diag_RW_RPAS_Display_Screen_source - 0*/
				if(Diag_RW_RPAS_Display_Screen_source == 0)
				{
					if(Gear_config == 1)		/*MT*/
					{
						if(EMS1_10_message_timeout_flag == 0)				/*EMS1_10 CAN Message Missing/Message Timeout*/
						{
							if(((CAN_EMS1_10[5].byte)& 0x0F) == 0x07)		/*MT - Reverse gear Detected*/
							{
								RPAS_Precondition_Active = 1;
								staybacktime_active = 1;
							}
							else
							{
								RPAS_Precondition_Active = 0;
							}
						}
						else
						{
							RPAS_Precondition_Active = 0;
						}
					}
					else if(Gear_config == 2)	/*AT*/
					{
						if(TCU6_20_message_timeout_flag == 0)
						{
							if(((CAN_TCU6_20[2].byte) & 0x0F) == 0x08)		/*AT - Reverse gear Detected*/
							{
								reverse_debounce_cntr++;/*counts 500 ms*/

								AT_Reverse_detected =1;
#ifdef GRAYSCALE_TFT
								if((reverse_debounce_cntr*100) >=(Diag_RW_AT_Reverse_Gear_Debounce_Time*0.60)&&(debounce_compare_flag==0))/*500 removed for testing*/
#endif

#ifdef COLOR_TFT
									if((reverse_debounce_cntr*100) >=(Diag_RW_AT_Reverse_Gear_Debounce_Time*RPAS_DEBOUNCE)&&(debounce_compare_flag==0))
#endif
									{
#ifdef SPRINT4
										if(AT_gearalert_reverse_display == 0)
#endif
										{
											debounce_compare_flag=1;
											RPAS_Precondition_Active =1;
											staybacktime_active = 1;
											reverse_debounce_cntr=0;
											//AT_Reversegear_timespan =0;/*testing*/
										}
									}
							}
							else
							{
								RPAS_Precondition_Active = 0;
								AT_Reverse_detected = 0;
								AT_Reversegear_timespan = 0;
								debounce_compare_flag=0;/*gear not a reverse gear*/
								reverse_debounce_cntr=0;
							}
						}
						else
						{
							RPAS_Precondition_Active = 0;
							AT_Reverse_detected = 0;
							reverse_debounce_cntr=0;
							debounce_compare_flag=0;/*gear not a reverse gear*/
						}
					}
					RPAS_ACTIVE_STS_flag = 0x0;
				}
				else if(Diag_RW_RPAS_Display_Screen_source == 1)
				{
					if(MBFM_PAS1_50_message_timeout_flag == 0)
					{
						RPAS_ACTIVE_STS = (CAN_MBFM_PAS1_50[4].bits.Rx_6);	/* MBFM_PAS1_50 -> RPAS_ACTIVE_STS */
						if(RPAS_ACTIVE_STS == 0x1)
						{
							reverse_played = 1;
							RPAS_Precondition_Active = 1;
							RPAS_ACTIVE_STS_flag = 0x1;
							staybacktime_active = 1;/*stay back flag included for adding the stay back timing*/
						}
						else
						{
							reverse_played = 0;
							RPAS_Precondition_Active = 0;
							RPAS_ACTIVE_STS_flag = 0x0;
						}
					}
					else
					{
						if(Gear_config == 1)									/*MT*/
						{
							if(EMS1_10_message_timeout_flag == 0)				/*EMS1_10 CAN Message Missing/Message Timeout*/
							{
								if(((CAN_EMS1_10[5].byte)& 0x0F) == 0x07)		/*MT - Reverse gear Detected*/
								{
									RPAS_Precondition_Active = 1;
									staybacktime_active = 1;
								}
								else
								{
									RPAS_Precondition_Active = 0;
								}
							}
							else
							{
								RPAS_Precondition_Active = 0;
							}
						}
						else if(Gear_config == 2)	/*AT*/
						{
							if(TCU6_20_message_timeout_flag == 0)
							{

								if(((CAN_TCU6_20[2].byte) & 0x0F) == 0x08)			/*AT - Reverse gear Detected*/
								{
									reverse_debounce_cntr++;/*counts 500 ms*/
									AT_Reverse_detected = 1;
#ifdef GRAYSCALE_TFT
									if((reverse_debounce_cntr*100 >= Diag_RW_AT_Reverse_Gear_Debounce_Time)&&(debounce_compare_flag==0))
#endif
#ifdef COLOR_TFT
										if((reverse_debounce_cntr*100 >= Diag_RW_AT_Reverse_Gear_Debounce_Time*RPAS_DEBOUNCE)&&(debounce_compare_flag==0))
#endif
										{
#ifdef SPRINT4
											if(AT_gearalert_reverse_display == 0)
#endif
											{
												debounce_compare_flag=1;
												RPAS_Precondition_Active =1;
												staybacktime_active = 1;
												reverse_debounce_cntr=0;
												//AT_Reversegear_timespan =0;/*testing*/
											}
										}
								}
								else
								{
									RPAS_Precondition_Active = 0;
									AT_Reverse_detected = 0;
									debounce_compare_flag=0;
									reverse_debounce_cntr=0;
								}
							}
							else
							{
								RPAS_Precondition_Active = 0;
								AT_Reverse_detected = 0;
								reverse_debounce_cntr=0;/*message recovery condition from can missing*/
							}
						}
						else
						{
							RPAS_Precondition_Active = 0;
							reverse_played = 0;
						}
					}
				}
				else
				{
					RPAS_Precondition_Active = 0;
					AT_Reverse_detected = 0;
					RPAS_ACTIVE_STS_flag = 0x0;
				}
			}
			else
			{
				RPAS_Precondition_Active = 0;
				AT_Reverse_detected = 0;
			}
		}
		else
		{
			//Stop rpas chime
#ifdef COLOR_TFT
			if(Sound_Bit == 0x10000)
			{
				RPAS_LV1_COUNTER = 0;
				Sound_Bit = 0x0;
				SoundPCM_4_rec_RPAS_St1_St2_cont_STOP();
			}
			if(Sound_Bit == 0x20000)
			{
				RPAS_LV2_COUNTER = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_LV2_STOP();
			}
			if(Sound_Bit == 0x30000)
			{
				RPAS_LV3_COUNTER = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_LV3_STOP();
			}
			if(Sound_Bit == 0x40000)
			{
				RPAS_LV4_COUNTER = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_LV4_STOP();
			}
			if(Sound_Bit==0x4000)
			{
				Sound_Bit = 0x0;
				SoundPCM_Reverse_cont_STOP();
			}
			if(Sound_Bit==0x8000)
			{
				Sound_Bit = 0x0;
				SoundPCM_Reverse_cont_STOP();
			}
#endif

#ifdef GRAYSCALE_TFT
#ifndef U350
			if(Sound_Bit == 0x10000)
			{
				W501_LOW_RPAS_LEVEL1_PRECON = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV1_STOP();
			}
			if(Sound_Bit == 0x20000)
			{
				W501_LOW_RPAS_COUNTER2 = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV2_STOP();
			}
			if(Sound_Bit == 0x30000)
			{
				W501_LOW_RPAS_COUNTER3 = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV3_STOP();
			}
			if(Sound_Bit == 0x40000)
			{
				W501_LOW_RPAS_COUNTER4 = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV4_STOP();
			}
#endif
#endif

		}
	}
}

/* ***********************************************************************************************
* Function Name: RPAS_Distance_Display_W501
* Description  : This function used for showing RPAS Distance Display in W501.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Distance_Display_W501(void)
{

	uint8_t STS_RPAS_DISP_data = 0;
	uint8_t STS_RPAS_DISP_bit5 = 5;
	uint8_t MBFM_PAS1_50_ERROR_data;
	uint8_t RPAS_ERROR;
	_Bool PREV_RPAS_DIST_text_display = 0;

	RPAS_display_dist = CAN_MBFM_PAS1_50[7].byte;

	MBFM_PAS1_50_ERROR_data = CAN_MBFM_PAS1_50[4].byte;
	RPAS_ERROR = (MBFM_PAS1_50_ERROR_data) & 0x1F;

	/*STS_RPAS_DISP_data = CAN_RPAS2_100[0].byte;
	STS_RPAS_DISP_bit5 = ((STS_RPAS_DISP_data >> STS_RPAS_DISP_bit5) & 1);*/

	if(RPAS_ERRORtext_display == 1)
	{
		RPAS_DIST_text_display = 1;				/*Display distance - Error display*/
	}
	else
	{
		if((RPAS_display_dist == 0x0)||(RPAS_display_dist == 0xFF))
		{
			RPAS_DIST_text_display = 3;			/*No distance display*/
		}
		else if((RPAS_display_dist >= 0xFB)&&(RPAS_display_dist <= 0xFE))
		{
			RPAS_DIST_text_display = 3;			/*No display of distance - Greater than 250 cm*/
		}

		else if((RPAS_display_dist > 0x00)&&(RPAS_display_dist <= 0xFB)/*&&(STS_RPAS_DISP_bit5 != 0x01)*/)
		{
			RPAS_DIST_text_display = 3;			/*No display of distance - Greater than 250 cm*/
		}

		/*if(STS_RPAS_DISP_bit5 == 0x01)
		{*/
			RPAS__update_display_dist = RPAS_display_dist;
			if((RPAS__update_display_dist >= 0x1)&&(RPAS__update_display_dist <= Diag_RW_Stop_Dist))
			{
				RPAS_DIST_text_display = 2;		/*STOP display*/
			}
			else if((Diag_RW_Stop_Dist < RPAS__update_display_dist)&&(RPAS__update_display_dist < 0xFB))
			{
				RPAS_DIST_text_display = 0;		/*distance info display*/
			}
		}
		/*else
		{
			PREV_RPAS_DIST_text_display = RPAS_DIST_text_display;
			if(PREV_RPAS_DIST_text_display != 1)
			{
				RPAS_DIST_text_display = PREV_RPAS_DIST_text_display;
			}
		}*/
	}
/* ***********************************************************************************************
* Function Name: RPAS_Status_Display_W501
* Description  : This function used for showing RPAS status display - Sector / Zone configuration in W501.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void RPAS_Status_Display_W501(void)
{
	uint8_t STS_RPAS_DISP_data = 0;
	uint8_t STS_RPAS_DISP_bit5 = 5;
	uint8_t BAR_SECT_LEFT = 0;
	uint8_t BAR_SECT_RIGHT = 0;
	uint8_t BAR_SECT_CENTER = 0;
	uint8_t BAR_SECT_RRC = 0;
	uint8_t BAR_SECT_RLC = 0;

	/*****************************************************************************************************/
	if(Diag_RW_RPAS_Sector_Zone_Calibration == 1)	/*2 sector ... 3 zones calibration*/
	{
		BAR_SECT_RRC = CAN_MBFM_PAS1_50[6].byte;
		BAR_SECT_RRC = ((BAR_SECT_RRC & 0xF0) >> 4);

		BAR_SECT_RLC = CAN_MBFM_PAS1_50[6].byte;
		BAR_SECT_RLC = (BAR_SECT_RLC & 0x0F);
		if((RPAS_ERROR != 0x04)&&(RPAS_ERROR != 0x05)&&(RPAS_ERROR != 0x06)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x0C)&&(RPAS_ERROR != 0x0D)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RRC)
			{
			case 0x01:
				RPAS_RCenter_imageIndex = 1; /*RRC BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RCenter_imageIndex = 2; /*RRC BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RCenter_imageIndex = 3; /*RRC BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RCenter_imageIndex = 0; /*RRC BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RCenter_imageIndex = 0; /*RRC BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}

		if((RPAS_ERROR != 0x02)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x06)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x0A)&&(RPAS_ERROR != 0x0B)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RLC)
			{
			case 0x01:
				RPAS_LCenter_imageIndex = 1; /*RLC BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_LCenter_imageIndex = 2; /*RLC BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_LCenter_imageIndex = 3; /*RLC BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_LCenter_imageIndex = 0; /*RLC BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_LCenter_imageIndex = 0; /*RLC BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}
	}
	/*****************************************************************************************************/
	/*****************************************************************************************************/
	else if(Diag_RW_RPAS_Sector_Zone_Calibration == 2)	/*2 sector ... 4 zones calibration*/
	{
		BAR_SECT_RRC = CAN_MBFM_PAS1_50[6].byte;
		BAR_SECT_RRC = ((BAR_SECT_RRC & 0xF0) >> 4);

		BAR_SECT_RLC = CAN_MBFM_PAS1_50[6].byte;
		BAR_SECT_RLC = (BAR_SECT_RLC & 0x0F);
		if((RPAS_ERROR != 0x04)&&(RPAS_ERROR != 0x05)&&(RPAS_ERROR != 0x06)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x0C)&&(RPAS_ERROR != 0x0D)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RRC)
			{
			case 0x01:
				RPAS_RCenter_imageIndex = 1; /*RRC BAR 1,2,3,4 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RCenter_imageIndex = 2; /*RRC BAR 1 - OFF | BAR 2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RCenter_imageIndex = 3; /*RRC BAR 1,2 - OFF | BAR 3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RCenter_imageIndex = 4; /*RRC BAR 1,2,3 - OFF | BAR 4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x05:
				RPAS_RCenter_imageIndex = 0; /*RRC BAR 1,2,3,4 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RCenter_imageIndex = 0; /*RRC BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}

		if((RPAS_ERROR != 0x02)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x06)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x0A)&&(RPAS_ERROR != 0x0B)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RLC)
			{
			case 0x01:
				RPAS_LCenter_imageIndex = 1; /*RLC BAR 1,2,3,4 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_LCenter_imageIndex = 2; /*RLC BAR 1 - OFF | BAR 2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_LCenter_imageIndex = 3; /*RLC BAR 1,2 - OFF | BAR 3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_LCenter_imageIndex = 4; /*RLC BAR 1,2,3 - OFF | BAR 4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x05:
				RPAS_LCenter_imageIndex = 0; /*RLC BAR 1,2,3,4 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_LCenter_imageIndex = 0; /*RLC BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}
	}
	/*****************************************************************************************************/
	else if(Diag_RW_RPAS_Sector_Zone_Calibration == 3)	/*3 sector ... 3 zones calibration*/
	{
		BAR_SECT_RIGHT = CAN_MBFM_PAS1_50[5].byte;
		BAR_SECT_RIGHT = ((BAR_SECT_RIGHT & 0xF0) >> 4);

		BAR_SECT_LEFT = CAN_MBFM_PAS1_50[5].byte;
		BAR_SECT_LEFT = (BAR_SECT_LEFT & 0x0F);

		BAR_SECT_RLC = CAN_MBFM_PAS1_50[6].byte;
		BAR_SECT_RLC = (BAR_SECT_RLC & 0x0F);

		if((RPAS_ERROR != 0x08)&&(RPAS_ERROR != 0x09)&&(RPAS_ERROR != 0x0A)&&(RPAS_ERROR != 0x0B)
				&&(RPAS_ERROR != 0x0C)&&(RPAS_ERROR != 0x0D)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RIGHT)
			{

			case 0x01:
				RPAS_RRight_imageIndex = 1; 	/*RR BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RRight_imageIndex = 2; 	/*RR BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RRight_imageIndex = 3; 	/*RR BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RRight_imageIndex = 0; 	/*RR BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RRight_imageIndex = 0; /*RR BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;

			}
		}

		if((RPAS_ERROR != 0x01)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x05)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x09)&&(RPAS_ERROR != 0x0B)&&(RPAS_ERROR != 0x0D)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_LEFT)
			{
			case 0x01:
				RPAS_RLeft_imageIndex = 1; 		/*RL BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RLeft_imageIndex = 2; 		/*RL BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RLeft_imageIndex = 3; 		/*RL BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RLeft_imageIndex = 0; 		/*RL BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RLeft_imageIndex = 0; 	/*RL BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}

		if((RPAS_ERROR != 0x02)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x06)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x0A)&&(RPAS_ERROR != 0x0B)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RLC)
			{
			case 0x01:
				RPAS_LCenter_imageIndex = 1; 		/*RL BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_LCenter_imageIndex = 2; 		/*RL BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_LCenter_imageIndex = 3; 		/*RL BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_LCenter_imageIndex = 0; 		/*RL BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_LCenter_imageIndex = 0; 	/*RL BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}
	}
	/*****************************************************************************************************/
	/*****************************************************************************************************/
	else if(Diag_RW_RPAS_Sector_Zone_Calibration == 4)	/*3 sector ... 4 zones calibration*/
	{
		BAR_SECT_RIGHT = CAN_MBFM_PAS1_50[5].byte;
		BAR_SECT_RIGHT = ((BAR_SECT_RIGHT & 0xF0) >> 4);

		BAR_SECT_LEFT = CAN_MBFM_PAS1_50[5].byte;
		BAR_SECT_LEFT = (BAR_SECT_LEFT & 0x0F);

		BAR_SECT_RLC = CAN_MBFM_PAS1_50[6].byte;
		BAR_SECT_RLC = (BAR_SECT_RLC & 0x0F);

		if((RPAS_ERROR != 0x08)&&(RPAS_ERROR != 0x09)&&(RPAS_ERROR != 0x0A)&&(RPAS_ERROR != 0x0B)
				&&(RPAS_ERROR != 0x0C)&&(RPAS_ERROR != 0x0D)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RIGHT)
			{

			case 0x01:
				RPAS_RRight_imageIndex = 1; 	/*RR BAR 1,2,3,4 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RRight_imageIndex = 2; 	/*RR BAR 1 - OFF | BAR 2,3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RRight_imageIndex = 3; 	/*RR BAR 1,2 - OFF | BAR 3,4 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RRight_imageIndex = 4; 	/*RR BAR 1,2,3 - OFF | BAR 4 ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x05:
				RPAS_RRight_imageIndex = 0; 	/*RR BAR 1,2,3,4 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RRight_imageIndex = 0; /*RR BAR 1,2,3,4 - OFF */
			RPAS_All_sensor_error = 0;
			break;

			}
		}

		if((RPAS_ERROR != 0x01)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x05)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x09)&&(RPAS_ERROR != 0x0B)&&(RPAS_ERROR != 0x0D)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_LEFT)
			{
			case 0x01:
				RPAS_RLeft_imageIndex = 1; 		/*RL BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_RLeft_imageIndex = 2; 		/*RL BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_RLeft_imageIndex = 3; 		/*RL BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_RLeft_imageIndex = 4; 		/*RL BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0x05:
				RPAS_RLeft_imageIndex = 0; 		/*RL BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_RLeft_imageIndex = 0; 	/*RL BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}

		if((RPAS_ERROR != 0x02)&&(RPAS_ERROR != 0x03)&&(RPAS_ERROR != 0x06)&&(RPAS_ERROR != 0x07)
				&&(RPAS_ERROR != 0x0A)&&(RPAS_ERROR != 0x0B)&&(RPAS_ERROR != 0x0E)&&(RPAS_ERROR != 0x0F)&&(RPAS_ERROR < 0x10))

		{
			switch (BAR_SECT_RLC)
			{
			case 0x01:
				RPAS_LCenter_imageIndex = 1; 		/*RLC BAR 1,2,3 - ON */
				RPAS_All_sensor_error = 0;
				break;

			case 0x02:
				RPAS_LCenter_imageIndex = 2; 		/*RLC BAR 1 - OFF | BAR 2,3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x03:
				RPAS_LCenter_imageIndex = 3; 		/*RLC BAR 1,2 - OFF | BAR 3 ON*/
				RPAS_All_sensor_error = 0;
				break;

			case 0x04:
				RPAS_LCenter_imageIndex = 4; 		/*RLC BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0x05:
				RPAS_LCenter_imageIndex = 0; 		/*RLC BAR 1,2,3 - OFF */
				RPAS_All_sensor_error = 0;
				break;

			case 0:
			default:RPAS_LCenter_imageIndex = 0; 	/*RLC BAR 1,2,3,4 - OFF*/
			RPAS_All_sensor_error = 0;
			break;
			}
		}
	}
	/*****************************************************************************************************/
	else
	{
		if(RPAS_RLeft_imageIndex != 5)
		{
			RPAS_RLeft_imageIndex = 0;	/*ALL Sectors - all bar off*/
		}

		if(RPAS_RRight_imageIndex != 5)
		{
			RPAS_RRight_imageIndex = 0;	/*ALL Sectors - all bar off*/
		}

		if(RPAS_RCenter_imageIndex != 5)
		{
			RPAS_RCenter_imageIndex = 0;	/*ALL Sectors - all bar off*/
		}

	}


}
/************************************************************************************************
 * Function Name: RPAS_Screen_Activation_W501
 * Description  : This function used for showing RPAS_Screen_Activation_W501.
 * 				 RPAS Screen Activation in 3 stages
 * 				 1. Error Screen in CAN Missing
 * 				 2.	Error Screen in CAN Present
 * 				 3.	RPAS Status activation
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void RPAS_Screen_Activation_W501(void)
{
	uint8_t MBFM_PAS1_50_ERROR_data = 0;

	RPAS_Precondition_W501();

	if((Diag_RW_RPAS_Sector_Zone_Calibration == 1)||(Diag_RW_RPAS_Sector_Zone_Calibration == 2))
	{
		sector_Zone_error = 0;
	}
	else if((Diag_RW_RPAS_Sector_Zone_Calibration == 3)||(Diag_RW_RPAS_Sector_Zone_Calibration == 4))
	{
		sector_Zone_error = 1;
	}

	if((Diag_RW_RPAS_Error_Warn_Enable == 1)&&(MBFM_PAS1_50_message_timeout_flag != 0))
	{
		if(RPAS_Precondition_Active == 1)
		{

		    RPAS_STAYBACK_FLAG=0;/*stay back condition */
		    
			if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
				RPAS_ACTIVE_popup_shown=1;
				dont_throw_alerts_alert_on_screen_present=1;
				rpas_stay_counter=1;
			}

			if(RPAS_ACTIVE_popup_shown == 1)
			{
				RPAS_ERRORtext_display = 1;		/*Error condition detected*/
				//RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
				/******************************************/
				/*RPAS_Distance_Display_U350();*/
				RPAS_Distance_Display_W501();
				/******************************************/
				RPAS_Error_detected = 1;
				RPAS_RLeft_imageIndex = 5;
				RPAS_RRight_imageIndex = 5;
				RPAS_RCenter_imageIndex = 5;
				RPAS_LCenter_imageIndex = 5;
			}
			else
			{
				RPAS_Error_detected = 0;
			}

		}
		else
		{
			if(staybacktime_active == 1)
			{

				rpas_stayback_cntr++;
				/***added to resolve the time delay issue between geayscal tft and color tft both are not taken the same time delay****/
#ifdef GRAYSCALE_TFT

				if(rpas_stayback_cntr*150 >= Diag_RW_RPASStayBackTimeVal*1000)/*graycsale TFT  the time delay is different*/

#endif


#ifdef COLOR_TFT

					if(rpas_stayback_cntr*120 >= Diag_RW_RPASStayBackTimeVal*1000)

#endif
						/*******************************************************************************/
					{
						rpasstay_compare_flag=0;
						rpas_stayback_cntr=0;

						// rpas_stayback_cntr =0;
						if(RPAS_ACTIVE_popup_shown == 1)/*stay back count !=0 added for testing invictus*/
						{
							rpas_stay_counter=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
							RPAS_ACTIVE_popup_shown=0;
							dont_throw_alerts_alert_on_screen_present=0;
							staybacktime_count = 0;
							staybacktime_active = 0;
						}

						RPAS_ACTIVE_popup_shown =0;
					}

			}

			else if(RPAS_Error_detected == 1)
			{
				if(RPAS_ACTIVE_popup_shown == 1)
				{
					rpas_stay_counter=0;
					(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
					RPAS_ACTIVE_popup_shown=0;
					dont_throw_alerts_alert_on_screen_present=0;
					//staybacktime_count = 0; /*reas stay back error invictus*/
					//staybacktime_active = 0;
				}
				RPAS_Error_detected = 0;
			}



		}



		/*Error screen Activate*/
	}
	else if((Diag_RW_RPAS_Error_Warn_Enable == 1)&&(MBFM_PAS1_50_message_timeout_flag == 0))
	{
		if(RPAS_Precondition_Active == 1)
		{
			MBFM_PAS1_50_ERROR_data = CAN_MBFM_PAS1_50[4].byte;
			RPAS_ERROR = (MBFM_PAS1_50_ERROR_data) & 0x1F;

			if(sector_Zone_error == 0)	/* 2 sector calibration */
			{
				if((RPAS_ERROR == 0x06)||(RPAS_ERROR == 0x7)||(RPAS_ERROR == 0xE)||(RPAS_ERROR == 0xF))
				{
					if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
						RPAS_ACTIVE_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
					}

					if(RPAS_ACTIVE_popup_shown == 1)
					{
						RPAS_ERRORtext_display = 1;		/*All Signal Error condition detected*/
	//					RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
						/*RPAS_RLeft_imageIndex = 5;*/
						RPAS_LCenter_imageIndex = 5;
						RPAS_RCenter_imageIndex = 5;
						/******************************************/
						/*RPAS_Distance_Display_U350();*/
						RPAS_Distance_Display_W501();
						/******************************************/
					}
				}
			}
			else if(sector_Zone_error == 1)	/* 3 sector calibration */
			{
				if((RPAS_ERROR == 0x10)||(RPAS_ERROR == 0xB)||(RPAS_ERROR == 0xF))
				{
					if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
						RPAS_ACTIVE_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
					}

					if(RPAS_ACTIVE_popup_shown == 1)
					{
						RPAS_ERRORtext_display = 1;		/*All Signal Error condition detected*/
						//					RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
						RPAS_RLeft_imageIndex = 5;
						RPAS_RRight_imageIndex = 5;
						RPAS_LCenter_imageIndex = 5;
						/******************************************/
						/*RPAS_Distance_Display_U350();*/
						RPAS_Distance_Display_W501();
						/******************************************/
					}
				}
			}
		}
		else
		{

#if 0
			/*implement the rpas time out using stayback time    invictus*/
/****************************************************************rpas stay back time***********************/
			if(staybacktime_active == 1)/*in message error condition the reverse gear dis engaged at the time it works*/
				{
				   rpas_stayback_cntr++;/*100 ms counter*/
/*******************************stayback time delay between wo501H/L*********************************/
                    #ifdef GRAYSCALE_TFT
				        if(rpas_stayback_cntr*150 >= Diag_RW_RPASStayBackTimeVal*1000)
					#else
				        if(rpas_stayback_cntr*170 >= Diag_RW_RPASStayBackTimeVal*1000)/*for color tft the timer multiplication factor changed*/
					#endif
/*****************************************************************************************************/
				        {
							    RPAS_STAYBACK_FLAG=0;
							    rpasstay_compare_flag=0;
							    rpas_stayback_cntr=0;
							   // rpas_stayback_cntr =0;
								if(RPAS_ACTIVE_popup_shown == 1)/*stay back count !=0 added for testing invictus*/
								{
								        rpas_stay_counter=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
									RPAS_ACTIVE_popup_shown=0;
									dont_throw_alerts_alert_on_screen_present=0;
									staybacktime_count = 0;
									staybacktime_active = 0;
								}

								RPAS_ACTIVE_popup_shown =0;
							}

						}
			
#endif



/**************************************************************************************************************/


/*
 - 			if(RPAS_ACTIVE_popup_shown == 1)
 - 			{
 - 				(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
 - 				RPAS_ACTIVE_popup_shown=0;
 - 				dont_throw_alerts_alert_on_screen_present=0;
 - 				staybacktime_active = 0;
 - 			}*/

		}

	}

	if((Diag_RW_PAS_Status_Warn_Enable == 1)&&(MBFM_PAS1_50_message_timeout_flag == 0))
	{
		if(RPAS_Precondition_Active == 1)
		{
			/*MBFM_PAS1_50_ERROR_data = CAN_RPAS2_100[0].byte;
			RPAS_ERROR = (MBFM_PAS1_50_ERROR_data) & 0x1F;*/
			MBFM_PAS1_50_ERROR_data = CAN_MBFM_PAS1_50[4].byte;
						RPAS_ERROR = (MBFM_PAS1_50_ERROR_data) & 0x1F;

			if((Diag_RW_RPAS_Sector_Zone_Calibration == 1)||(Diag_RW_RPAS_Sector_Zone_Calibration == 2))
			{
				sector_Zone_error = 0;
			}
			else if((Diag_RW_RPAS_Sector_Zone_Calibration == 3)||(Diag_RW_RPAS_Sector_Zone_Calibration == 4))
			{
				sector_Zone_error = 1;
			}
			if(sector_Zone_error == 0)
			{
				if((RPAS_ERROR != 0x6)&&(RPAS_ERROR != 0x7)&&(RPAS_ERROR != 0xE)&&(RPAS_ERROR != 0xF))
				{

					if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
						RPAS_ACTIVE_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
					}
					if(RPAS_ACTIVE_popup_shown == 1)
					{
						RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/

						if(RPAS_ERROR == 0x00)
						{
							RPAS_ERRORtext_display = 0;
							//						RPAS_RLeft_imageIndex = 0; /*RL Sensor Not Fail -ERROR*/
							//						RPAS_RRight_imageIndex = 0;/*RR Sensor Not Fail -ERROR*/
/*********************************For 2 sector configuration***************************************************/
							RPAS_RCenter_imageIndex = 0;/*RC Sensor Not Fail -ERROR*/
							RPAS_LCenter_imageIndex = 0;/*RC Sensor Not Fail -ERROR*/
						}
						else
						{
							RPAS_ERRORtext_display = 1;
							if((RPAS_ERROR >= 0x10)&&(RPAS_ERROR <= 0x1F))
							{
								RPAS_ERRORtext_display = 1;
								//							RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
								/*RPAS_RLeft_imageIndex = 5;*/
								/*RPAS_RRight_imageIndex = 5;*/
								RPAS_RCenter_imageIndex = 5;/*RRC Sensor Not Fail -ERROR*/
								RPAS_LCenter_imageIndex = 5;/*RLC Sensor Not Fail -ERROR*/
							}
							else
							{

								if((RPAS_ERROR == 0x04)||(RPAS_ERROR == 0x05)||(RPAS_ERROR == 0x06)||(RPAS_ERROR == 0x07)||(RPAS_ERROR == 0x0C)||(RPAS_ERROR == 0x0D)||(RPAS_ERROR == 0x0F))
								{
									RPAS_RCenter_imageIndex = 5; /*RRC Sensor Fail -ERROR*/
									RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
								}
								else
								{
									RPAS_RCenter_imageIndex = 0; /*RRC Sensor Fail -ERROR*/
								}
								if((RPAS_ERROR == 0x02)||(RPAS_ERROR == 0x03)||(RPAS_ERROR == 0x06)||(RPAS_ERROR == 0x07)||(RPAS_ERROR == 0x0A)||(RPAS_ERROR == 0x0B)||(RPAS_ERROR == 0x0F))
								{
									RPAS_LCenter_imageIndex = 5;/*RLC Sensor Fail -ERROR*/
									RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
								}
								else
								{
									RPAS_LCenter_imageIndex = 0;/*RLC Sensor Fail -ERROR*/
								}
							}

						}
						/*RPAS condition Activated*/
						/**********************************************/
						/*RPAS_Status_Display_U350();*/
						RPAS_Status_Display_W501();
						/*RPAS_Distance_Display_U350();*/
						RPAS_Distance_Display_W501();
						/**********************************************/
					}

				}
			}
			else if(sector_Zone_error == 1)
			{
//				if((RPAS_ERROR != 0x6)&&(RPAS_ERROR != 0x7)&&(RPAS_ERROR != 0xD)&&(RPAS_ERROR != 0xF))
				if((RPAS_ERROR != 0x10)||(RPAS_ERROR != 0xB)||(RPAS_ERROR != 0xF))
				{

					if((RPAS_ACTIVE_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
						RPAS_ACTIVE_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
					}
					if(RPAS_ACTIVE_popup_shown == 1)
					{
						RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/

						if(RPAS_ERROR == 0x00)
						{
							RPAS_ERRORtext_display = 0;
							//						RPAS_RLeft_imageIndex = 0; /*RL Sensor Not Fail -ERROR*/
							//						RPAS_RRight_imageIndex = 0;/*RR Sensor Not Fail -ERROR*/
							/*For 2 sector configuration*/
							RPAS_RCenter_imageIndex = 0;/*RC Sensor Not Fail -ERROR*/
							RPAS_LCenter_imageIndex = 0;/*RC Sensor Not Fail -ERROR*/
						}
						else
						{
							RPAS_ERRORtext_display = 1;
							if((RPAS_ERROR >= 0x10)&&(RPAS_ERROR <= 0x1F))
							{
								RPAS_ERRORtext_display = 1;
								//							RPAS_All_sensor_error = 1;		/*All Sectors showing 'X'*/
								RPAS_RLeft_imageIndex = 5;
								RPAS_RRight_imageIndex = 5;
								RPAS_RCenter_imageIndex = 5;
								RPAS_LCenter_imageIndex = 5;
							}
							else
							{

								if((RPAS_ERROR == 0x01)||(RPAS_ERROR == 0x03)||(RPAS_ERROR == 0x05)||(RPAS_ERROR == 0x07)||(RPAS_ERROR == 0x09)||(RPAS_ERROR == 0xB)||(RPAS_ERROR == 0xD)||(RPAS_ERROR == 0xF))
								{
									RPAS_RLeft_imageIndex = 5; 		/*RL Sensor Fail -ERROR*/
									RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
								}
								else
								{
									RPAS_RLeft_imageIndex = 0; /*RL Sensor Fail -ERROR*/
								}
								if((RPAS_ERROR == 0x08)||(RPAS_ERROR == 0x09)||(RPAS_ERROR == 0xA)||(RPAS_ERROR == 0xB)||(RPAS_ERROR == 0xC)||(RPAS_ERROR == 0xD)||(RPAS_ERROR == 0xE)||(RPAS_ERROR == 0xF))
								{
									RPAS_RRight_imageIndex = 5;/*RR Sensor Fail -ERROR*/
									RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
								}
								else
								{
									RPAS_RRight_imageIndex = 0;/*RR Sensor Fail -ERROR*/
								}
								if((RPAS_ERROR == 0x02)||(RPAS_ERROR == 0x03)||(RPAS_ERROR == 0x06)||(RPAS_ERROR == 0x07)||(RPAS_ERROR == 0x0A)||(RPAS_ERROR == 0x0B)||(RPAS_ERROR == 0x0E)||(RPAS_ERROR == 0x0F))	/*Bug in v209 is solved - ERROR not display to 0x0A,0x0C,0x0E*/
								{
									RPAS_LCenter_imageIndex = 5;/*RC Sensor Fail -ERROR*/
									RPAS_All_sensor_error = 0;		/*All Sectors not showing 'X'*/
								}
								else
								{
									RPAS_LCenter_imageIndex = 0;/*RC Sensor Fail -ERROR*/
								}
							}

						}
						/*RPAS condition Activated*/
						/**********************************************/
						/*RPAS_Status_Display_U350();*/
						RPAS_Status_Display_W501();
						/*RPAS_Distance_Display_U350();*/
						RPAS_Distance_Display_W501();
						/**********************************************/
					}

				}
			}

		}
		else
		{
			/**********************************************rpas stay bak time from DID invictus************************/
			if((staybacktime_active == 1)/*&&(compare_flag==1)*/)/*in message error condition the reverse gear dis engaged at the time it works*/
						{
			 rpas1_stayback_cntr++;/*500 ms counter*/ /*invictus commented for testing*/
                         #ifdef GRAYSCALE_TFT
	                if(rpas1_stayback_cntr*150 >= Diag_RW_RPASStayBackTimeVal*1000)/*graycsale TFT  the time delay is different*/    
			 #endif
                        #ifdef COLOR_TFT
		        if(rpas1_stayback_cntr*120 >= Diag_RW_RPASStayBackTimeVal*1000)			   
			#endif
							    {
								   RPAS_STAYBACK_FLAG=0;
							           rpasstay_compare_flag=0;
							     rpas1_stayback_cntr=0;
							   // rpas_stayback_cntr =0;
								if(RPAS_ACTIVE_popup_shown == 1)/*stay back count !=0 added for testing invictus*/
								{
								        rpas_stay_counter=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
									RPAS_ACTIVE_popup_shown=0;
									dont_throw_alerts_alert_on_screen_present=0;
									staybacktime_count = 0;
									staybacktime_active = 0;
								}

								RPAS_ACTIVE_popup_shown =0;
							}
						}

			/***********************************************************************************************************/

/*			if(RPAS_ACTIVE_popup_shown == 1) /*commented for testing
 - 			{
 - 				(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
 - 				RPAS_ACTIVE_popup_shown=0;
 - 				dont_throw_alerts_alert_on_screen_present=0;
 - 				staybacktime_active = 0;
 - 			}*/
		}
	}
#if 0
	if((Diag_RW_RPAS_Display_Screen_source == 1)&&(MBFM_PAS1_50_message_timeout_flag != 0)&&(RPAS_Precondition_Active == 0)) /*CAN MISSING @ Diag_RW_RPAS_Display_Screen_source == 1*/
	{
		if(RPAS_ACTIVE_popup_shown == 1)
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_timeout);
			RPAS_ACTIVE_popup_shown=0;
			dont_throw_alerts_alert_on_screen_present=0;
			staybacktime_active = 0;
		}
	}
#endif
}
/* ***********************************************************************************************
 * Function Name: IGN_OFF_ALERT
 * Description  : This function used for showing Func_safety alert
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */

void IGN_OFF_CHECKLAMP_ALERT(RX_DATA *can_buffer)
{

	/* checking ignition ON or OFF */

	{

		if((LVSD_flag == 0U) && (HVSD_flag == 0U))	/* checking LVSD & HVSD flag */
		{
			if(IGNOFF_MBFM1_100_message_timeout_flag == 0)
			{

				/***********************Ignition Off Check External Lamp Start****************************************************/
				/*Check Check_EXT_Lamp Functionality*/
				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Park_Lamp_Warn_Enable == 1) && (FEATURE_CODE_PARK_LAMP_ON_CONFIG == 1))
				{
					if(can_buffer[0].bits.Rx_5 == 1)
					{
						Checklamp_active_IGNOFF = 1;
						/* Check_EXT_Lamp Alert ON */
						if((Check_EXT_Lamp_IGNOFF_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present == 0)&&((startkey == 0x11108)||(Not_IGN_Wakeup != 1)))
						{
							Checklamp_active_IGNOFF_Alert = 1;

							Check_EXT_Lamp_IGNOFF_popup_shown = 1;
							//						Checklamp_active_IGNOFF = 1;
							dont_throw_alerts_alert_on_screen_present = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_External_Lamp_IGNOFF);
							IGNOFF_ExtLamp_check_done = 0;
						}
						else if((dont_throw_alerts_alert_on_screen_present == 1)&&((Dooropen_IGNOFF_popup_shown == 1)||(Key_In_Alert_popup_shown == 1)))
						{
							wait_state = 1;
						}
						else
						{
							wait_state = 0;
						}
					}
					else
					{
						if((Check_EXT_Lamp_IGNOFF_popup_shown == 1)&&((startkey == 0x22035)||(startkey == 0x11108)))
						{
							Priority_flag = 0;
							start_alart_timeout_5sec=1;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							wait_state = 0;//test
						}
						Check_EXT_Lamp_IGNOFF_popup_shown = 0;
						Checklamp_active_IGNOFF = 0;
						IGNOFF_ExtLamp_check_done = 1;
						Checklamp_active_IGNOFF_Alert = 0;
					}

				}
				else
				{
					Check_EXT_Lamp_IGNOFF_popup_shown = 0;
					Checklamp_active_IGNOFF = 0;
					IGNOFF_ExtLamp_check_done = 1;
					Checklamp_active_IGNOFF_Alert = 0;
				}
				/***********************Ignition Off Check External Lamp END****************************************************/
			}
			else
			{
				if(startkey == 0x22018) /*Can missing-alert timeout*/
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
					IGNOFF_display_timeout = 1;
#ifdef GRAYSCALE_TFT
						clear_screen();
#endif
				}
				Check_EXT_Lamp_IGNOFF_popup_shown = 0;
				Checklamp_active_IGNOFF = 0;
				IGNOFF_ExtLamp_check_done = 1;
				Checklamp_active_IGNOFF_Alert = 0;
			}
		}
		else
		{
			Dooropen_IGNOFF_popup_shown = 0;
			Check_EXT_Lamp_IGNOFF_popup_shown = 0;
			Checklamp_active_IGNOFF = 0;
			IGNOFF_Dooropen_check_done = 1;
			IGNOFF_ExtLamp_check_done = 1;
		}

		/*		}
		else
		{
			Checklamp_active_IGNOFF = 0;
			IGNOFF_ExtLamp_check_done = 1;
		}*/

	}


	/* Implementation of  ParkLamp Chime  */
	if((0 == LVSD_flag ) && (0 == HVSD_flag))
	{
		if((1 == VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE) && (1 == Diag_RW_Park_Lamp_reminder_Chime_Enable)&&(1 == FEATURE_CODE_PARK_LAMP_ON_CONFIG))
		{
			if(0 == IGNOFF_MBFM1_100_message_timeout_flag)
			{
				if(KEY_IN != priorityPark_Keyin)	/* This is for Setting Priority , which should be played(key-in/park lamp) when both occur same time */
				{
				if(can_buffer[0].bits.Rx_5 == 1)
				{

					if(0x200 == Sound_Bit)	/* When Park Lamp chime to be played and if hazard is being Played
												Set Sound Bit to 0 to play remaining */
					{
						Sound_Bit = 0x00;
					}
						if(((Sound_Bit == 0x00)||(Sound_Bit == 0x100000))&&(buzzParkLamp == 0))
						{
							parkLampChime = 1;		/* Setting Flag for Park Lamp Chime to Play */
							Sound_Bit = 0x100000;   /* Setting Sound bit */
							priorityPark_Keyin = PARK_LAMP;
						}
						else
						{
							/* Clearing Flags to re-enter */
							parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
						}
				}
				else
				{
					/* Clearing Flags to re-enter */
					parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
					buzzParkLamp = 0;
					buzzParkLampCntr = 0;
					priorityPark_Keyin = 0;
					if(Sound_Bit == 0x100000)
					{
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						Sound_Bit = 0x0;
					}
					if(2 > cycleParkLamp_cntr)	/* if Message is missed  before completing 2 cycles , leave Sound bit*/
					{
						cycleParkLamp_cntr = 0;
						Sound_Bit = 0x00;
					}
				}
				}

			}
			else
			{			/* Clearing Flags to re-enter */
				if(Sound_Bit == 0x100000)
				{
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
					Sound_Bit = 0x0;
				}
				//	parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
				//buzzParkLamp = 0;
				//buzzParkLampCntr = 0;
				//priorityPark_Keyin = 0;

			}
		}
		else
		{	/* Clearing Flags to re-enter */
		//	parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
		//	buzzParkLamp = 0;
		//	buzzParkLampCntr = 0;
		//	priorityPark_Keyin = 0;

		}
	}
	else
	{	/* Clearing Flags to re-enter */
	//	parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
	//	buzzParkLamp = 0;
	//	buzzParkLampCntr = 0;
	//	priorityPark_Keyin = 0;

	}
}

void IGN_OFF_DOOROPEN_ALERT(RX_DATA *can_buffer)
{
	uint8_t /*STS_Door_data = 0,*/Dooropen_active;
	uint8_t STS_Door_bit0 = 0;
	uint8_t STS_Door_bit1 = 1;
	uint8_t STS_Door_bit2 = 2;
	uint8_t STS_Door_bit3 = 3;
	uint8_t STS_Door_bit4 = 4;
	uint8_t STS_Door_bit5 = 5;

	/* checking ignition ON or OFF */

	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))	/* checking LVSD & HVSD flag */

		{
			if(IGNOFF_MBFM1_100_message_timeout_flag == 0)
			{
				STS_Door_data = can_buffer[0].byte & 0X3F;
				/* Storing indiviual bit of STS_Eng_data */
				STS_Door_bit0 = ((STS_Door_data >> STS_Door_bit0) & 1);
				STS_Door_bit1 = ((STS_Door_data >> STS_Door_bit1) & 1);
				STS_Door_bit2 = ((STS_Door_data >> STS_Door_bit2) & 1);
				STS_Door_bit3 = ((STS_Door_data >> STS_Door_bit3) & 1);
				STS_Door_bit4 = ((STS_Door_data >> STS_Door_bit4) & 1);
				STS_Door_bit5 = ((STS_Door_data >> STS_Door_bit5) & 1);

				if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_DoorOpen_Warn_Enable == 1))	/* Door Open warning Functionality enable */
				{
					if(IGNOFFprev_STS_Door_data != STS_Door_data)
					{
//						Dooropen_IGNOFF_popup_shown=0;
						if(Dooropen_IGNOFF_popup_shown == 1)
						{
							start_alart_timeout_5sec=1;
						}
						Codriver_index = 0;
						Driver_index = 0;
						Rearleft_index = 0;
						RearRight_index = 0;
						Backd_index = 0;
						IGNOFF_Dooropen_detected = 1;
					}

					IGNOFFprev_STS_Door_data = STS_Door_data;
#if 1
					if((STS_Door_bit0 == 0x01) || (STS_Door_bit1 == 0x01) || (STS_Door_bit2 == 0x01)
							|| (STS_Door_bit3 == 0x01) || (STS_Door_bit4 == 0x01) || (STS_Door_bit5 == 0x01))
					{
						Dooropen_active_IGNOFF = 1;
						Doorclose_IGNOFF_popup_shown=0;
						All_doorclose_timeout_start = 0;
						if((Dooropen_IGNOFF_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)/*&&(dont_throw_alerts_BS6_alert_on_screen_present==0)*/&&(IGNOFF_Dooropen_detected == 1)&&((startkey == 0x11108)||(Not_IGN_Wakeup != 1)))
						{
							Dooropen_active_IGNOFF_Alert = 1;
							/*(void)drome_sendEventToDrome((U32)drome_InEvent_Door_Open);*/
							(void)drome_sendEventToDrome((U32)drome_InEvent_Dooropen_IGNOFF);
							Dooropen_IGNOFF_popup_shown=1;
							Doorclose_IGNOFF_popup_shown=0;
							dont_throw_alerts_alert_on_screen_present=1;
							IGNOFF_Dooropen_check_done = 0;

							//						Doorclose_popup_shown = 0;
						}
						else if((dont_throw_alerts_alert_on_screen_present == 1)&&((Key_In_Alert_popup_shown == 1)||(Check_EXT_Lamp_IGNOFF_popup_shown == 1)))
						{
							wait_state = 1;
						}
						else
						{
							wait_state = 0;
						}
						/* Display "Door Open" */
						//					DoorClose_ON=1;
					}
					else if((STS_Door_bit0 == 0) && (STS_Door_bit1 == 0) && (STS_Door_bit2 == 0)
							&& (STS_Door_bit3 == 0) && (STS_Door_bit4 == 0) && (STS_Door_bit5 == 0))
					{
						if(Dooropen_active_IGNOFF == 1)
						{
							All_doorclose = 1;
	//						Dooropen_active_IGNOFF = 0;
	//						Dooropen_active = 0;
						}
						else
						{
							Dooropen_IGNOFF_popup_shown=0;
							Dooropen_active_IGNOFF = 0;
							All_doorclose = 0;
						}


//						Dooropen_IGNOFF_popup_shown=0;
//						Dooropen_active_IGNOFF = 0;
//						IGNOFF_Dooropen_check_done = 1;
//						Dooropen_active_IGNOFF_Alert = 0;
						/****
					if((Doorclose_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alldoorclose);
						Doorclose_popup_shown=1;
						dont_throw_alerts_alert_on_screen_present=1;
						Dooropen_active_IGNOFF = 0;
					}
					if(DoorClose_ON == 1)
					{
						DoorClose_ON=2;
					}
						 ******/

						if(((Doorclose_IGNOFF_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0)/*&&(dont_throw_doorclose_on_screen_present==0)*/&&(All_doorclose == 1))||
								((Doorclose_IGNOFF_popup_shown==0)&&(startkey==0x22034)/*&&(dont_throw_doorclose_on_screen_present==0)*/&&(All_doorclose == 1)))
						{
							/*(void)drome_sendEventToDrome((U32)drome_InEvent_Alldoorclose);*/
							(void)drome_sendEventToDrome((U32)drome_InEvent_Dooropen_IGNOFF);
							Doorclose_IGNOFF_popup_shown=1;
							All_doorclose_timeout_start = 1;
							dont_throw_alerts_alert_on_screen_present = 1;
							dont_throw_doorclose_on_screen_present = 1;
							decrement_trend = 0;	/*Door close*/
							codoor_index = 0;
							Dooropen_active_IGNOFF_Alert = 1;
							display_active = 1;
							/*Dooropen_active = 0;*/
						}
#if 0
						DoorClose_ON=1;
						if(DoorClose_1sec >= 10000)
						{
							/* Display "Door close" */
							DoorClose_1sec = 0;
							DoorClose_ON = 0;
						}
#endif
					}
#if 0
					if(IGNOFFprev_STS_Door_data != STS_Door_data)
					{
						Dooropen_IGNOFF_popup_shown=0;
					}


					IGNOFFprev_STS_Door_data = STS_Door_data;
#endif
#endif

#if 1
					if(Dooropen_active_IGNOFF_Alert == 1)
					{
						if((STS_Door_bit0 == 0x01)&&(Backd_index != 4)&&(Backdoor_set != 1)&&(Back_flag != 1))
						{
							/* "Back_door = ON;" */
							(void)drome_sendEventToDrome((U32)drome_InEvent_Backdoor_dooropen);
							Back_flag = 1;
							Backd_index = 4;
						}
						else if((STS_Door_bit0 == 0x00)&&(Back_flag == 1)&&(Backdoor_set != 0))
						{
							/* Back_door = OFF */
							(void)drome_sendEventToDrome((U32)drome_InEvent_Backdoor_doorClose);
							Back_flag = 0;
							Backd_index = 0;
						}

						/* checking Hood_door open or not */
						if(STS_Door_bit1 == 0x01)
						{
							//Hood_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Hood_dooropen);
							Hood_flag = 1;
							Hood_index = 0;
						}
						else
						{
							//Hood_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Hood_doorclose);
							Hood_flag = 0;
							Hood_index = 1;
						}

						/* checking Co_Driver_door open or not */
						if((STS_Door_bit2 == 0x01)&&(Codriver_index != 4)&&(Codoor_set != 1)&&(Codriver_flag != 1))
						{
							//Co_Driver_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Codriver_dooropen);
							Codriver_flag = 1;
							Codriver_index = 4;
						}
						else if((STS_Door_bit2 == 0x00)&&(Codriver_flag == 1)&&(Codoor_set != 0))
						{
							//Co_Driver_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Codriver_doorclose);
							Codriver_flag = 0;
							Codriver_index = 0;
						}

						/* checking Driver_door open or not */
						if((STS_Door_bit3 == 0x01)&&(Driver_index != 4)&&(driverdoor_set != 1)&&(Driver_flag != 1))
						{
							//Driver_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_driver_dooropen);
							Driver_flag = 1;
							Driver_index = 4;
						}
						else if((STS_Door_bit3 == 0x00)&&(Driver_flag == 1)&&(driverdoor_set != 0))
						{
							//Driver_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_driver_doorclose);
							Driver_flag = 0;
							Driver_index = 0;
						}

						/* checking Rear_Left_door open or not */
						if((STS_Door_bit4 == 0x01)&&(Rearleft_index != 4)&&(Rearleftdoor_set != 1)&&(Rearleft_flag != 1))
						{
							//Rear_Left_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Rearleft_dooropen);
							Rearleft_flag = 1;
							Rearleft_index = 4;

						}
						else if((STS_Door_bit4 == 0x00)&&(Rearleft_flag == 1)&&(Rearleftdoor_set != 0))
						{
							//Rear_Left_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Rearleft_doorclose);
							Rearleft_flag = 0;
							Rearleft_index = 0;
						}

						/* checking Rear_Right_door open or not */
						if((STS_Door_bit5 == 0x01)&&(RearRight_index != 4)&&(RearRightdoor_set != 1)&&(RearRight_flag != 1))
						{
							//Rear_Right_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_RearRight_dooropen);
							RearRight_flag = 1;
							RearRight_index = 4;
						}
						else if((STS_Door_bit5 == 0x00)&&(RearRight_flag == 1)&&(RearRightdoor_set != 0))
						{
							//Rear_Right_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_RearRight_doorclose);
							RearRight_flag = 0;
							RearRight_index = 0;
						}
#if 0
						/* checking back door open or not */
						if(STS_Door_bit0 == 0x01)
						{
							/* "Back_door = ON;" */
							(void)drome_sendEventToDrome((U32)drome_InEvent_Backdoor_dooropen);

						}
						else
						{
							/* Back_door = OFF */
							(void)drome_sendEventToDrome((U32)drome_InEvent_Backdoor_doorClose);
						}

						/* checking Hood_door open or not */
						if(STS_Door_bit1 == 0x01)
						{
							//Hood_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Hood_dooropen);
							Hood_flag = 1;
						}
						else
						{
							//Hood_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Hood_doorclose);
							Hood_flag = 0;
						}

						/* checking Co_Driver_door open or not */
						if(STS_Door_bit2 == 0x01)
						{
							//Co_Driver_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Codriver_dooropen);
							Codriver_flag = 1;
						}
						else
						{
							//Co_Driver_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Codriver_doorclose);
							Codriver_flag = 0;
						}

						/* checking Driver_door open or not */
						if(STS_Door_bit3 == 0x01)
						{
							//Driver_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_driver_dooropen);
							Driver_flag = 1;
						}
						else
						{
							//Driver_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_driver_doorclose);
							Driver_flag = 0;
						}

						/* checking Rear_Left_door open or not */
						if(STS_Door_bit4 == 0x01)
						{
							//Rear_Left_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_Rearleft_dooropen);
							Rearleft_flag = 1;

						}
						else
						{
							//Rear_Left_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_Rearleft_doorclose);
							Rearleft_flag = 0;
						}

						/* checking Rear_Right_door open or not */
						if(STS_Door_bit5 == 0x01)
						{
							//Rear_Right_door = ON
							(void)drome_sendEventToDrome((U32)drome_InEvent_RearRight_dooropen);
							RearRight_flag = 1;
						}
						else
						{
							//Rear_Right_door = OFF
							(void)drome_sendEventToDrome((U32)drome_InEvent_RearRight_doorclose);
							RearRight_flag = 0;
						}
#endif
					}
					else
					{
											IGNOFF_Dooropen_check_done = 1;
					}
#endif

				}
				else
				{
					Dooropen_active_IGNOFF = 0;		/* Door Open warning Functionality disable */
					Dooropen_IGNOFF_popup_shown=0;
					IGNOFF_Dooropen_check_done = 1;
					//				DoorClose_ON=0;
				}
				if(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1)
				{
					if((STS_Door_bit0 == 0x01) || (STS_Door_bit1 == 0x01) || (STS_Door_bit2 == 0x01)
							|| (STS_Door_bit3 == 0x01) || (STS_Door_bit4 == 0x01) || (STS_Door_bit5 == 0x01))

					{
						DOOR_OPEN_TT = 1;
						DOOR_OPEN_IGN_OFF=1;
						DID_TT_bit_data_set1.bits.Rx_5=1;
					}
					else
					{
						DOOR_OPEN_TT = 0;
						DID_TT_bit_data_set1.bits.Rx_5=0;
//						DOOR_OPEN_IGN_OFF=0;
					}
					//output_shift_register();

				}
				else
				{
					DOOR_OPEN_TT = 0;
					DOOR_OPEN_IGN_OFF=0;
					DID_TT_bit_data_set1.bits.Rx_5=0;
				}
				/***********************Ignition Off Door Open END****************************************************/
			}
			else
			{
				if(key == 0x22034) /*Can missing-alert timeout*/
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
					IGNOFF_display_timeout = 1;
#ifdef GRAYSCALE_TFT
						clear_screen();
#endif
				}
				Dooropen_active_IGNOFF = 0;
				IGNOFF_Dooropen_check_done = 1;
				Dooropen_active_IGNOFF_Alert = 0;
				Dooropen_IGNOFF_popup_shown = 0;
				IGNOFF_Dooropen_detected = 1;
			}
		}
		else
		{
			DOOR_OPEN_TT = 0;			//Turn off TT on Abnormal voltage in IGN off
			Dooropen_active_IGNOFF = 0;
			IGNOFF_Dooropen_check_done = 1;
			Dooropen_active_IGNOFF_Alert = 0;
			Dooropen_IGNOFF_popup_shown = 0;
			DID_TT_bit_data_set1.bits.Rx_5=0;
		}

	}
}

void  IGN_OFF_IMMO(RX_DATA *can_buffer)
{
	uint8_t IMMO_CAN_DATA = 0;
	uint8_t IMMO_DATA_BIT4 = 4;
	uint8_t IMMO_DATA_BIT5 = 5;

	IMMO_CAN_DATA=can_buffer[0].byte;

	IMMO_DATA_BIT4 = ((IMMO_CAN_DATA >> IMMO_DATA_BIT4) & 1);
	IMMO_DATA_BIT5 = ((IMMO_CAN_DATA >> IMMO_DATA_BIT5) & 1);

		if(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0)		//config_0
		{
			IMMOBIIZER_TT=0;
			IMMO_TT_WAKEUP_FLAG=0;

		} //this is done in init.c
	if((VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))
	{
		if(IGNOFF_MBFM_CAN_ON==0)//(IGNOFF_MBFM1_100_message_timeout_flag != 0)
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

		else if(IGNOFF_MBFM_CAN_ON==1)//(IGNOFF_MBFM1_100_message_timeout_flag == 0)
		{
			if(IMMO_DATA_BIT4==0 && IMMO_DATA_BIT5==0)
			{
				IMMOBIIZER_TT=0;
				previous_state_immobilizer_blink4=0;
				previous_state_immobilizer_blink5=0;
				IMMO_TT_WAKEUP_FLAG=0;
				*RRAM_IMMO_PATT4 = 0;
				*RRAM_IMMO_PATT5 = 0;
			}
			else if(IMMO_DATA_BIT4==1 && IMMO_DATA_BIT5==0)
			{
				IMMOBIIZER_TT=telltale_blink_bool4;
				previous_state_immobilizer_blink4=1;
				previous_state_immobilizer_blink5=0;
				*RRAM_IMMO_PATT4 = 1;
				*RRAM_IMMO_PATT5 = 0;
			}
			else if(IMMO_DATA_BIT4==0 && IMMO_DATA_BIT5==1)
			{

				IMMOBIIZER_TT=telltale_blink_bool5;		//blink_patt5
				previous_state_immobilizer_blink5=1;
				previous_state_immobilizer_blink4=0;
				*RRAM_IMMO_PATT4 = 0;
				*RRAM_IMMO_PATT5 = 1;

			}
			else if(IMMO_DATA_BIT4==1 && IMMO_DATA_BIT5==1)
			{
				if(previous_state_immobilizer_blink4==1)
				{
					IMMOBIIZER_TT=telltale_blink_bool4;
					*RRAM_IMMO_PATT4 = 1;
					*RRAM_IMMO_PATT5 = 0;
				}
				else if(previous_state_immobilizer_blink5==1)
				{
					IMMOBIIZER_TT=telltale_blink_bool5;
					*RRAM_IMMO_PATT4 = 0;
					*RRAM_IMMO_PATT5 = 1;

				}

				else
				{
					IMMOBIIZER_TT=IMMOBIIZER_TT;
				}
			}

			IMMO_IGN_OFF=1;
		}

	}
	else
	{
		IMMO_TT_WAKEUP_FLAG=0;
	}


}



void  IGN_OFF_PARK_LAMP(RX_DATA *can_buffer)
{
	uint8_t PARK_LAMP_CAN_DATA = 0;
		uint8_t PARK_LAMP_DATA_BIT4 = 4;
		uint8_t PARK_LAMP_DATA_BIT5 = 5;

		PARK_LAMP_CAN_DATA=can_buffer[0].byte;

		PARK_LAMP_DATA_BIT4 = ((PARK_LAMP_CAN_DATA >> PARK_LAMP_DATA_BIT4) & 1);
		PARK_LAMP_DATA_BIT5 = ((PARK_LAMP_CAN_DATA >> PARK_LAMP_DATA_BIT5) & 1);

		if((LVSD_flag == 0U) && (HVSD_flag == 0U))	/* checking LVSD & HVSD flag */
		{
			if(FEATURE_CODE_PARK_TT_TYPE==0 && /* MBFM1_100_message_timeout_flag == CLEAR &&*/ VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)
			{
				if(IGNOFF_MBFM1_100_message_timeout_flag == 0)
				{
					if(((PARK_LAMP_DATA_BIT4 == 0) && (PARK_LAMP_DATA_BIT5==0)) || ((PARK_LAMP_DATA_BIT4 == 0) && (PARK_LAMP_DATA_BIT5==1))|| ((PARK_LAMP_DATA_BIT4 == 1) && (PARK_LAMP_DATA_BIT5==1)))
					{
						PARK_LAMP_TT=0;
						PARK_LAMP_IGN_OFF=0;
					}
					else if((PARK_LAMP_DATA_BIT4 == 1 && PARK_LAMP_DATA_BIT5==0))
					{
						PARK_LAMP_TT=1;
						PARK_LAMP_IGN_OFF=1;
					}
				}
				else
				{
					PARK_LAMP_TT=0;
					PARK_LAMP_IGN_OFF=0;
				}
			}
			else
			{

//				PARK_LAMP_TT=0;
				PARK_LAMP_IGN_OFF=0;

			}
		}
		else
		{
			PARK_LAMP_TT=0;				//Turn off TT on Abnormal voltage in IGN off
			PARK_LAMP_IGN_OFF=0;
		}

}
/* ***********************************************************************************************
* Function Name: Key_In_Reminder
* Description  : This function used for showing Key_In_Reminder
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Key_In_Reminder(RX_DATA *can_buffer)
{
//	_Bool Key_In_Alert = 0;
//	Key_In_Alert = (CAN_MBFM1_100[1].bits.Rx_4);
//	if(IGN_ON_flag == 0)
		{
			if((LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				/*Check Key_In_Reminder Functionality*/
				if(IGNOFF_MBFM1_100_message_timeout_flag == 0)
				{
					if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_Key_in_reminder_Chime_Enable == 1)&&(FEATURE_CODE_KEY_IN_REMINDER_CONFIG ==1))
					{
						if(PARK_LAMP != priorityPark_Keyin)	/* This is for Setting Priority , which should be played(key-in/park lamp) when both occur same time */
						{
							if(0x200 == Sound_Bit)	/* When Park Lamp chime to be played and if hazard is being Played
																				Set Sound Bit to 0 to play remaining */
							{
								Sound_Bit = 0x00;
							}
							if(can_buffer[0].bits.Rx_4 == 1)
							{
								if((Sound_Bit == 0x00||Sound_Bit == 0x80000)&&(Buzz_KIR_compl == 0))
								{
									KIR_Chime = 1;
									priorityPark_Keyin = KEY_IN;

								}
								else
								{
									KIR_Chime = 0;
								}
							}
							else
							{
								Buzz_KIR_cntr = 0;
								KIR_Chime = 0;
								Buzz_KIR_compl = 0;
								priorityPark_Keyin = 0;
								if(Sound_Bit == 0x80000) 		/*Signal disable */
								{
									r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
									Sound_Bit = 0x00;
								}
								if(2 > cycleKIR_cntr)	/* if Message is missed  before completing 2 cycles , leave Sound bit*/
								{
									cycleKIR_cntr=0;
									Sound_Bit = 0x00;
								}

							}
						}
					}
					if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE == 1) && (Diag_RW_RKE_Key_in_reminder_warn_Enable == 1))
					{
//						Key_In_Alert = can_buffer[1].byte & 0X08;
						/*Check CAN Signal not missing*/

						if(can_buffer[0].bits.Rx_4 == 1)
						{
							if((Key_In_Alert_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&((startkey == 0x11108)||(Not_IGN_Wakeup != 1)))
							{
								Keyinreminder_IGNOFF_Alert = 1;
								Priority_flag = 1;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Key_In_Reminder);//for testing only
								Key_In_Alert_popup_shown = 1;
								dont_throw_alerts_alert_on_screen_present=1;
								IGNOFF_keyinremind_check_done = 0;
								//								alerts_history[23] = 1;
							}
							else if((dont_throw_alerts_alert_on_screen_present == 1)&&((Dooropen_IGNOFF_popup_shown == 1)||(Check_EXT_Lamp_IGNOFF_popup_shown == 1)))
							{
								wait_state = 1;
							}
							else
							{
								wait_state = 0;
							}
						}
						else
						{
							if((Key_In_Alert_popup_shown == 1)&&((startkey == 0x22030)||(startkey == 0x11108)))
							{
								Priority_flag = 0;
								start_alart_timeout_5sec= 0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								wait_state = 0;//test
							}
							Key_In_Alert_popup_shown = 0;
							Keyinreminder_IGNOFF_Alert = 0;
							IGNOFF_keyinremind_check_done = 1;

						}
					}
					else
					{
						Key_In_Alert_popup_shown = 0;
						Keyinreminder_IGNOFF_Alert = 0;
						IGNOFF_keyinremind_check_done = 1;
					}
				}
				else
				{
					if(Sound_Bit == 0x80000) 		/*Signal disable */
					{
						r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
						Sound_Bit = 0x00;
					}
					if(startkey == 0x22030) /*Can missing-alert timeout*/
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
						IGNOFF_display_timeout = 1;
#ifdef GRAYSCALE_TFT
						clear_screen();
#endif
					}
					Key_In_Alert_popup_shown = 0;
					Keyinreminder_IGNOFF_Alert = 0;
					IGNOFF_keyinremind_check_done = 1;
				}
			}
			else
			{
				Key_In_Alert_popup_shown = 0;
				Keyinreminder_IGNOFF_Alert = 0;
				IGNOFF_keyinremind_check_done = 1;
			}
		}
}
/* ***********************************************************************************************
 * Function Name: HDC_alert
 * Description  : This function used for showing HDC_alert
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void HDC_alert()
{
	uint8_t HDC_Active = 0U;
	HDC_Active = (((CAN_ESC2_10[4].byte) & 0x30)>>4);
	if((IGN_ON_flag == 1)&&(logo_completed==1))
	{
		if(IGN_HDC == 0)
		{
			HDC_STS = HDC_Active;
			IGN_HDC = 1;
		}
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			/*Check PAB_Off_alerts Functionality*/
			if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1)&&(VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0) && (Diag_RW_HDC_Status_Enable == 1))
			{
				/*Check for CAN Missing*/
				if(ESC2_10_message_timeout_flag == 0)
				{
					if(HDC_Active == 1)
					{
						/* HDC_Ready. Alert ON */
						if((HDC_Ready_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0))
						{
							//								Add HDC_Active On alert
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_HDC_Ready);//for testing only
							HDC_Ready_popup_shown = 1;
							HDC_Active_popup_shown = 0;
							HDC_Passive_popup_shown = 0;
							dont_throw_alerts_alert_on_screen_present=1;
//							HDC_STS = 1;
//							skip_alert_Enable = 3;
						}
					}
					else if(HDC_Active == 2)
					{
						/* HDC_active. Alert Off */
						if((HDC_Active_popup_shown==0)&&(dont_throw_alerts_alert_on_screen_present==0))
						{
							Priority_flag = 1;
							(void)drome_sendEventToDrome((U32)drome_InEvent_HDC_Active);//for testing only
							HDC_Ready_popup_shown = 0;
							HDC_Active_popup_shown = 1;
							HDC_Passive_popup_shown = 0;
							dont_throw_alerts_alert_on_screen_present=1;
//							HDC_STS = 0;
//							skip_alert_Enable = 3;
						}

					}
					else if(HDC_Active == 3)
					{
						HDC_Ready_popup_shown = 0;
						HDC_Active_popup_shown = 0;
						HDC_Passive_popup_shown = 0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}

					if((HDC_Active == 0)&&(HDC_Passive_popup_shown == 0)&&((HDC_Ready_popup_shown ==1)||(HDC_Active_popup_shown == 1)))
					{
						Priority_flag = 1;
						(void)drome_sendEventToDrome((U32)drome_InEvent_HDC_Passive);//for testing only
						HDC_Ready_popup_shown = 0;
						HDC_Active_popup_shown = 0;
						HDC_Passive_popup_shown = 1;
						dont_throw_alerts_alert_on_screen_present=1;
//						HDC_STS = 0;
//						skip_alert_Enable = 3;
					}
					if((HDC_Ready_popup_shown == 1)&&(HDC_Active != 1)&&(Alert_key == 0x22061))
					{
						Priority_flag = 0;
						dont_throw_alerts_alert_on_screen_present=0;
						HDC_Ready_popup_shown = 0;
					}
					else if((HDC_Active_popup_shown == 1)&&(HDC_Active != 2)&&(Alert_key == 0x22066))
					{
						Priority_flag = 0;
						dont_throw_alerts_alert_on_screen_present=0;
						HDC_Active_popup_shown = 0;
					}
					else if((HDC_Passive_popup_shown == 1)&&(HDC_Active != 3)&&(Alert_key == 0x22067))
					{
						Priority_flag = 0;
						dont_throw_alerts_alert_on_screen_present=0;
						HDC_Passive_popup_shown = 0;
					}
				}
				else
				{
					HDC_Ready_popup_shown = 0;
					HDC_Active_popup_shown = 0;
					HDC_Passive_popup_shown = 0;
				}
			}
		}
	}
}


/* ***********************************************************************************************
 * Function Name: Warning_History_dooropen
 * Description  : This function used for showing Door status updated in warning history
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */

void Warning_History_dooropen(void)
{
	uint8_t WH_Door_data;
	uint8_t WH_Door_bit0 = 0;
	uint8_t WH_Door_bit1 = 1;
	uint8_t WH_Door_bit2 = 2;
	uint8_t WH_Door_bit3 = 3;
	uint8_t WH_Door_bit4 = 4;
	uint8_t WH_Door_bit5 = 5;
//if(WH_Dooropen_active == 1)
//{
	WH_Door_data = (CAN_MBFM1_100[4].byte) & 0X3F;

					/* Storing indidual bit of STS_Eng_data */
	WH_Door_bit0 = ((WH_Door_data >> WH_Door_bit0) & 1);
	WH_Door_bit1 = ((WH_Door_data >> WH_Door_bit1) & 1);
	WH_Door_bit2 = ((WH_Door_data >> WH_Door_bit2) & 1);
	WH_Door_bit3 = ((WH_Door_data >> WH_Door_bit3) & 1);
	WH_Door_bit4 = ((WH_Door_data >> WH_Door_bit4) & 1);
	WH_Door_bit5 = ((WH_Door_data >> WH_Door_bit5) & 1);

	if((WH_Door_bit0 == 0x01)&&(WH_backdoor == 0)&&((WH_dooropen_present == 1)))
	{
		WH_backdoor = 1;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Backdoor_dooropen);

	}
	else if((WH_Door_bit0 == 0x00)||(WH_dooropen_present == 0))
	{
		/* Back_door = OFF */
//		WH_backdoor = 0;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Backdoor_doorClose);
		if(WH_Door_bit0 == 0x00)
		{
			WH_backdoor = 0;
		}
	}

	/* checking Hood_door open or not */
	if((WH_Door_bit1 == 0x01)&&(WH_Hooddoor == 0)&&(WH_dooropen_present == 1))
	{
		//Hood_door = ON
		WH_Hooddoor = 1;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Hood_dooropen);

	}
	else if((WH_Door_bit1 == 0x00)||(WH_dooropen_present == 0))
	{
		//Hood_door = OFF
//		WH_Hooddoor = 0;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Hood_doorclose);
		if(WH_Door_bit1 == 0x00)
		{
			WH_Hooddoor = 0;
		}
	}

	/* checking Co_Driver_door open or not */
	if((WH_Door_bit2 == 0x01)&&(WH_codriverdoor == 0)&&(WH_dooropen_present == 1))
	{
		//Co_Driver_door = ON
		WH_codriverdoor = 1;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Codriver_dooropen);

	}
	else if((WH_Door_bit2 == 0x00)||(WH_dooropen_present == 0))
	{
		//Co_Driver_door = OFF
//		WH_codriverdoor = 0;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Codriver_doorclose);
		if(WH_Door_bit2 == 0x00)
		{
			WH_codriverdoor = 0;
		}

	}

	/* checking Driver_door open or not */
	if((WH_Door_bit3 == 0x01)&&(WH_driverdoor == 0)&&(WH_dooropen_present == 1))
	{
		//Driver_door = ON
		WH_driverdoor = 1;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_driver_dooropen);

	}
	else if((WH_Door_bit3 == 0x00)||(WH_dooropen_present == 0))
	{
		//Driver_door = OFF

		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_driver_doorclose);
		if(WH_Door_bit3 == 0x00)
		{
			WH_driverdoor = 0;
		}

	}

	/* checking Rear_Left_door open or not */
	if((WH_Door_bit4 == 0x01)&&(WH_Rearleftdoor == 0)&&(WH_dooropen_present == 1))
	{
		//Rear_Left_door = ON
		WH_Rearleftdoor = 1;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Rearleft_dooropen);


	}
	else if((WH_Door_bit4 == 0x00)||(WH_dooropen_present == 0))
	{
		//Rear_Left_door = OFF

		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Rearleft_doorclose);
		if(WH_Door_bit4 == 0x00)
		{
			WH_Rearleftdoor = 0;
		}
	}

	/* checking Rear_Right_door open or not */
	if((WH_Door_bit5 == 0x01)&&(WH_Rearrightdoor == 0)&&(WH_dooropen_present == 1))
	{
		//Rear_Right_door = ON
		WH_Rearrightdoor = 1;
		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_RearRight_dooropen);

	}
	else if((WH_Door_bit5 == 0x00)||(WH_dooropen_present == 0))
	{
		//Rear_Right_door = OFF

		(void)drome_sendEventToDrome((U32)drome_InEvent_WH_RearRight_doorclose);
		if(WH_Door_bit5 == 0x00)
		{
			WH_Rearrightdoor = 0;
		}
	}
//}
/*else
{
	(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Backdoor_doorClose);
	(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Hood_doorclose);
	(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Codriver_doorclose);
	(void)drome_sendEventToDrome((U32)drome_InEvent_WH_driver_doorclose);
	(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Rearleft_doorclose);
	(void)drome_sendEventToDrome((U32)drome_InEvent_WH_RearRight_doorclose);
	}*/
}
#ifdef SPRINT4
/* ***********************************************************************************************
 * Function Name: AT_gear_alert
 * Description  : This function used for activation of Auto gear alert on display
 * Arguments    : FEATURE_CODE_AT_GEAR_ALERT / Diag_RW_AT_gear_alert_duration / Diag_RW_Debounce_to_detect_gear_position
 * Return Value : None
 ************************************************************************************************ */
void AT_gear_alert(void)
{
	uint8_t STS_ENG;

	if(IGN_ON_flag == 1)
	{
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if((FEATURE_CODE_AT_GEAR_ALERT == 1)&&(Gear_Configuration == 3)&&(Diag_AT_GEAR_ALERT_Duration != 0))		/*AT*/
			{
				if((TCU6_20_message_timeout_flag == 0)&& (EMS1_10_message_timeout_flag == 0))
				{
					STS_ENG = CAN_EMS1_10[6].byte; /* stored 6th byte of EMS1_10 */
					STS_ENG = ((STS_ENG & 0xE0) >> 5); /* using Higher nibble 1st 3 bit */

					if(STS_ENG == 2)
					{
						TGS_LEVER1 = ((CAN_TCU6_20[2].byte) & (0x0F));/* storing Lower nibble of  */
						if(PREV_TGS_LEVER != TGS_LEVER1)
						{
							if(((TGS_LEVER1 >= 0)&&(TGS_LEVER1 <= 5))||(TGS_LEVER1 == 0x0A)||(TGS_LEVER1 == 0x0B)||(TGS_LEVER1 > 0x0C))	/*Reserved - Middle Position - Signal Invalid - Default*/
							{
								AT_gear_debounce_start = 0;
								AT_gear_alert_active = 0;
								AT_gearalert_reverse_display = 0;
								AT_gear_IGN_ON = 0;
							}
							else if(((PREV_TGS_LEVER == 0x0A)||(PREV_TGS_LEVER == 0x0B))&&(TGS_LEVER1 == 0x0C))	/*Tip+ tp  M & Tip- to M */
							{
								AT_gear_debounce_start = 0;
								AT_gear_alert_active = 0;
								AT_gearalert_reverse_display = 0;
								AT_gear_IGN_ON = 0;
							}
							else				/*P - R - N - D - M*/
							{
								if(TGS_LEVER1 == 0x08)
								{
									AT_gearalert_reverse_display = 1;	/*Flag set when reverse gear detected & hold RPAS till AT_gear timeout*/
								}
								else
								{
									AT_gearalert_reverse_display = 0;
								}
								AT_gear_debounce_start = 1;				/*AT_gear debounce timer start*/
								if(AT_gear_IGM_ON_detected == 1)
								{
									AT_gear_IGN_ON = 1;					/*Used for IGN ON 1st AT gear will come*/
									AT_gear_IGM_ON_detected = 0;
								}

								if(/*(ATgear_popup_shown == 1)||*/(startkey == 0x22073))	/*need to review*/
								{
									start_AT_alert_timeout_cnt=1;			/*If alert is on screen & gear is changed then timeout timer reset*/
									AT_gear_update_cnt = 0;
								}
								ATgear_popup_shown = 0;
							}
						}
						PREV_TGS_LEVER = TGS_LEVER1;					/*Copy the current gear to prev. gear variable */
						if(AT_gear_alert_active == 1)					/*After debounce timer completed - set AT_gear_alert_active*/
						{
							if((ATgear_popup_shown == 0))
							{
								Priority_flag = 1;						/*Flag set for no any other will come to take priority*/
								if((dont_throw_alerts_alert_on_screen_present==0)&&(skip_flag == 0)&&(logo_completed==1))
								{
									//(void)drome_sendEventToDrome((U32)drome_InEvent_Door_Open);
									(void)drome_sendEventToDrome((U32)drome_InEvent_AT_Alert_Detected);
									ATgear_popup_shown = 1;
									dont_throw_alerts_alert_on_screen_present=1;
									AT_gear_debounce_start = 0;
									AT_gear_IGN_ON = 0;
								}
							}
						}
						else
						{
							//#if 0
							if(/*(ATgear_popup_shown == 1)&&*/(startkey == 0x22073))	/*if getting invalid gear condition - alert immediately timeout*/
							{
								Priority_flag = 0;
								start_AT_alert_timeout_cnt = 0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_AT_Alert_Timeout);
								//								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								AT_gear_IGN_ON = 0;
							}
							//#endif
						}
					}
					else
					{
						if(/*(ATgear_popup_shown == 1)&&*/(startkey == 0x22073))	/*if getting invalid gear condition - alert immediately timeout*/
						{
							Priority_flag = 0;
							start_AT_alert_timeout_cnt = 0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_AT_Alert_Timeout);
							//								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						AT_gear_IGN_ON = 0;
						PREV_TGS_LEVER = 0;			/*Reset to alert redisplay condition*/
						AT_gearalert_reverse_display = 0;
					}
				}
				else
				{
					if(/*(ATgear_popup_shown == 1)&&*/(startkey == 0x22073))	/*if getting invalid gear condition - alert immediately timeout*/
					{
						Priority_flag = 0;
						start_AT_alert_timeout_cnt = 0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_AT_Alert_Timeout);
						//							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					AT_gear_IGN_ON = 0;
					PREV_TGS_LEVER = 0;			/*Reset to alert redisplay condition*/
					AT_gearalert_reverse_display = 0;
				}
			}
			else
			{
				AT_gear_IGN_ON = 0;
				AT_gear_debounce_start = 0;
				AT_gear_alert_active = 0;
				AT_gearalert_reverse_display = 0;
			}
		}
	}
}
#endif
/* ***********************************************************************************************
 * Function Name: IO_Control_Message_activation
 * Description  : This function used for activation of Message by IO control
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */

void IO_Control_Message_activation(void)
{
	/*BYTE 0*/
	if( (IO_Control_Enable_Message_Activation[0].byte & 0x01) && (IO_Control_State_Message_Activation[0].byte & 0x01) )
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Seat_Belt);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x02)&&(IO_Control_State_Message_Activation[0].byte &0x02))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Engine_Temp_High);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x04)&&(IO_Control_State_Message_Activation[0].byte &0x04))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Engine_Oil_Pressure);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x08)&&(IO_Control_State_Message_Activation[0].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Brake_Fluid);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x10)&&(IO_Control_State_Message_Activation[0].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_parkbrake);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x20)&&(IO_Control_State_Message_Activation[0].byte &0x20))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Door_Open);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x40)&&(IO_Control_State_Message_Activation[0].byte &0x40))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_screen_ACTIVE);
	}
	else if((IO_Control_Enable_Message_Activation[0].byte &0x80)&&(IO_Control_State_Message_Activation[0].byte &0x80))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Fuel);
	}

	/*BYTE 1*/
	if( (IO_Control_Enable_Message_Activation[1].byte & 0x01) && (IO_Control_State_Message_Activation[1].byte & 0x01) )
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Water_in_Fuel);
	}
	else if((IO_Control_Enable_Message_Activation[1].byte &0x02)&&(IO_Control_State_Message_Activation[1].byte &0x02))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Service_Due);
	}
	else if((IO_Control_Enable_Message_Activation[1].byte &0x04)&&(IO_Control_State_Message_Activation[1].byte &0x04))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Overspeed);
	}
	else if((IO_Control_Enable_Message_Activation[1].byte &0x08)&&(IO_Control_State_Message_Activation[1].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_check_engine);
	}
	else if((IO_Control_Enable_Message_Activation[1].byte &0x10)&&(IO_Control_State_Message_Activation[1].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_PAB_OFF);
	}
	else if((IO_Control_Enable_Message_Activation[1].byte &0x20)&&(IO_Control_State_Message_Activation[1].byte &0x20))
	{
		//Add TDMS alert
	}

	/*BYTE 2*/
	if( (IO_Control_Enable_Message_Activation[2].byte & 0x01) && (IO_Control_State_Message_Activation[2].byte & 0x01) )
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Cruise_ON);
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x02)&&(IO_Control_State_Message_Activation[2].byte &0x02))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_ECO_ON);
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x04)&&(IO_Control_State_Message_Activation[2].byte &0x04))
	{
		//Add HHC alert
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x08)&&(IO_Control_State_Message_Activation[2].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Check_ESP);
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x10)&&(IO_Control_State_Message_Activation[2].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Check_ABS);
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x20)&&(IO_Control_State_Message_Activation[2].byte &0x20))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Check_EBD);
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x40)&&(IO_Control_State_Message_Activation[2].byte &0x40))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_External_Lamp);
	}
	else if((IO_Control_Enable_Message_Activation[2].byte &0x80)&&(IO_Control_State_Message_Activation[2].byte &0x80))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Trailer_SW_ON);
	}

	/*BYTE 3*/
	if((IO_Control_Enable_Message_Activation[3].byte &0x04)&&(IO_Control_State_Message_Activation[3].byte &0x04))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Stop_Vehicle_For_Setting);
	}
	else if((IO_Control_Enable_Message_Activation[3].byte &0x08)&&(IO_Control_State_Message_Activation[3].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Func_safety);
	}
	else if((IO_Control_Enable_Message_Activation[3].byte &0x10)&&(IO_Control_State_Message_Activation[3].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Left_DRL_Lamp_Failure);
	}
	else if((IO_Control_Enable_Message_Activation[3].byte &0x20)&&(IO_Control_State_Message_Activation[3].byte &0x20))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Left_Turn_Lamp_Failure);
	}
	else if((IO_Control_Enable_Message_Activation[3].byte &0x40)&&(IO_Control_State_Message_Activation[3].byte &0x40))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Right_Turn_Lamp_Failure);
	}
	else if((IO_Control_Enable_Message_Activation[3].byte &0x80)&&(IO_Control_State_Message_Activation[3].byte &0x80))
	{
		//PAS status
	}

	/*BYTE 4*/
	if((IO_Control_Enable_Message_Activation[4].byte &0x01)&&(IO_Control_State_Message_Activation[4].byte &0x01))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Horn_Failure);
	}

	/*BYTE 5*/
	if((IO_Control_Enable_Message_Activation[5].byte &0x08)&&(IO_Control_State_Message_Activation[5].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Low_Battery);
	}

	/*BYTE 6*/
	if((IO_Control_Enable_Message_Activation[6].byte &0x08)&&(IO_Control_State_Message_Activation[6].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Tail_Lamp_Failure);
	}
	else if((IO_Control_Enable_Message_Activation[6].byte &0x10)&&(IO_Control_State_Message_Activation[6].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_RPAS_Supply_Failure);
	}

	/*BYTE 7*/
	if( (IO_Control_Enable_Message_Activation[7].byte & 0x01) && (IO_Control_State_Message_Activation[7].byte & 0x01) )
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Incorrect_DEF_Detected);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x02)&&(IO_Control_State_Message_Activation[7].byte &0x02))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_DEF_Dosing_Malfunction);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x04)&&(IO_Control_State_Message_Activation[7].byte &0x04))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_DEF_Level_Low);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x08)&&(IO_Control_State_Message_Activation[7].byte &0x08))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_DPF_Clogged);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x10)&&(IO_Control_State_Message_Activation[7].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_started);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x20)&&(IO_Control_State_Message_Activation[7].byte &0x20))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_not_started);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x40)&&(IO_Control_State_Message_Activation[7].byte &0x40))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_completed);
	}
	else if((IO_Control_Enable_Message_Activation[7].byte &0x80)&&(IO_Control_State_Message_Activation[7].byte &0x80))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_aborted);
	}

	/*BYTE 8*/
	if( (IO_Control_Enable_Message_Activation[8].byte & 0x01) && (IO_Control_State_Message_Activation[8].byte & 0x01) )
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_Requested);
	}
	else if((IO_Control_Enable_Message_Activation[8].byte &0x04)&&(IO_Control_State_Message_Activation[8].byte &0x04))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_REGEN_Required);
	}

	/*BYTE 9*/
	if((IO_Control_Enable_Message_Activation[9].byte &0x40)&&(IO_Control_State_Message_Activation[9].byte &0x40))
	{
		/*TPMS Low Tyre Pressure*/
	}
	else if((IO_Control_Enable_Message_Activation[9].byte &0x80)&&(IO_Control_State_Message_Activation[9].byte &0x80))
	{
		/*TPMS High Tyre Pressure*/
	}

	/*BYTE 10*/
	if( (IO_Control_Enable_Message_Activation[10].byte & 0x01) && (IO_Control_State_Message_Activation[10].byte & 0x01) )
	{
		/*TPMS Sys Fault*/
	}
	else if((IO_Control_Enable_Message_Activation[10].byte &0x02)&&(IO_Control_State_Message_Activation[10].byte &0x02))
	{
		/*TPMS Tyre Leakage*/
	}
	else if((IO_Control_Enable_Message_Activation[10].byte &0x04)&&(IO_Control_State_Message_Activation[10].byte &0x04))
	{
		/*TPMS High Temp*/
	}
	else if((IO_Control_Enable_Message_Activation[10].byte &0x08)&&(IO_Control_State_Message_Activation[10].byte &0x08))
	{
		/*TPMS Low Temp*/
	}
	else if((IO_Control_Enable_Message_Activation[10].byte &0x10)&&(IO_Control_State_Message_Activation[10].byte &0x10))
	{
		(void)drome_sendEventToDrome((U32)drome_InEvent_Key_In_Reminder);
	}

	/*BYTE 11*/
	if((IO_Control_Enable_Message_Activation[11].byte &0x40)&&(IO_Control_State_Message_Activation[11].byte &0x40))
	{
		/*4WD Change Animation*/
	}

	/*BYTE 12*/
	if((IO_Control_Enable_Message_Activation[12].byte &0x04)&&(IO_Control_State_Message_Activation[12].byte &0x04))
	{
		/*HDC Status*/
	}
	else if((IO_Control_Enable_Message_Activation[12].byte &0x10)&&(IO_Control_State_Message_Activation[12].byte &0x10))
	{
		/*Roll Alert*/
	}
	else if((IO_Control_Enable_Message_Activation[12].byte &0x20)&&(IO_Control_State_Message_Activation[12].byte &0x20))
	{
		/*Pitch Alert*/
	}
}

/* ***********************************************************************************************
 * Function Name: IO_Control_TFT_Test_Grayscale
 * Description  : This function used for activation of Message by IO control for W501- Low End
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */

void IO_Control_TFT_Test_Grayscale(void)
{
	if(IO_TFT_TEST==1)
	{
		if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 0))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels OFF
			IO_Control_TFT = 0;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 1))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels ON
			IO_Control_TFT = 1;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 2))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels WHITE
			IO_Control_TFT = 2;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 3))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels Checker
			IO_Control_TFT = 3;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 4))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels Inverted checker
			IO_Control_TFT = 4;
		}
	}
}

/* ***********************************************************************************************
 * Function Name: IO_Control_TFT_Test_ColourTFT
 * Description  : This function used for activation of Message by IO control for W501- High End
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */

void IO_Control_TFT_Test_ColourTFT(void)
{
	if(IO_TFT_TEST==1)
	{
		if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 0))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels OFF
			IO_Control_TFT = 0;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 1))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels ON
			IO_Control_TFT = 1;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 2))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels WHITE
			IO_Control_TFT = 2;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 3))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels Checker
			IO_Control_TFT = 3;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 4))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels Inverted checker
			IO_Control_TFT = 4;
		}
		else if(((IO_Control_Enable_Display.byte & 0x01) == 1)&&(IO_Control_State_Display_Status == 5))
		{
			(void)drome_sendEventToDrome((U32)drome_InEvent_IOcontrol_TFT);//All Pixels Inverted checker
			IO_Control_TFT = 5;
		}
	}
}

/* ***********************************************************************************************
 * Function Name: Door_open_animation_parareset
 * Description  : This function used for IGN ON -> OFF door open animation parameters reset.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void Door_open_animation_parareset(void)
{
	Codriver_flag = 0;
	Codoor_set = 0;
	Codriver_index = 0;

	Driver_flag = 0;
	driverdoor_set = 0;
	Driver_index = 0;

	Rearleft_flag = 0;
	Rearleftdoor_set = 0;
	Rearleft_index = 0;

	RearRight_flag = 0;
	RearRightdoor_set = 0;
	RearRight_index = 0;

	Back_flag = 0;
	Backdoor_set = 0;
	Backd_index = 0;
}

/* ***********************************************************************************************
 * Function Name: Handle_Inactive_alerts
 * Description  : This function used for disable displayed alerts when deactivate condition appear.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void Handle_Inactive_alerts(void)
{
	Alert_key = drome_getCurrentViewKey();

	if(Overspeed_popup_shown == 1)
	{
		Overspeed_alert();//1
	}
	if(Seat_Belt_popup_shown == 1)
	{
		Seat_belt();//2
	}
	if(Low_Engine_Oil_Pressure_popup_shown == 1)
	{
		Engine_oil_pressure_low();//5
	}
	if(Engine_Temp_High_popup_shown == 1)
	{
		High_Temp();//6
	}
	if(Low_Brake_Fluid_popup_shown == 1)
	{
		Low_Brake_Fluid();//7
	}
	if(Park_brake_popup_shown == 1)
	{
		Park_Break_alert();//8	Execute after very 500ms
	}
	if(PAB_Off_popup_shown == 1)
	{
		PAB_Off_alerts();//9
	}
	if(DPF_Clogged_popup_shown == 1)
	{
		DPF_Filter_Clogged();//10
	}
	if(Check_Engine_popup_shown == 1)
	{
		Check_Engine_alert();//9
	}
	if(EBD_popup_shown == 1)
	{
		Check_EBD_alert();//45
	}
	if(Check_ABS_popup_shown == 1)
	{
		Check_ABS();//46
	}
	if(ESP_popup_shown == 1)
	{
		Check_ESP_alert();//47
	}
	if(RPAS_Supply_failure_popup_shown == 1)
	{
		RPAS_Supply_Failure_alerts();//48
	}
	if(Low_Battery_popup_shown == 1)
	{
		RKE_Low_Battery_alert();//49	RKE_Battery
	}
	if(Water_In_Fuel_popup_shown == 1)
	{
		Water_In_Fuel_alert();//50
	}

	Stop_for_setting();//56		//////////////remaining

	if(Func_safety_popup_shown == 1)
	{
		Func_safety();//57
	}
	if(Left_Turn_failure_popup_shown == 1)
	{
		LEFT_Turn_Lamp_failure_alerts();//59
	}
	if(Right_Turn_failure_popup_shown == 1)
	{
		RIGHT_Turn_Lamp_failure_alerts();//60
	}
	if(Tail_Lamp_failure_popup_shown == 1)
	{
		Tail_Lamp_failure_alerts();//62
	}
	if(Horn_Fail_popup_shown == 1)
	{
		Horn_failure_alerts();//63
	}
	if((Left_DRL_failure_popup_shown == 1)||(Right_DRL_failure_popup_shown == 1))
	{
		DRL_failure_alerts();
	}
	if((EcoON_mode_popup_shown == 1)||(EcoOFF_mode_popup_shown == 1))
	{
		ECO_Mode_alerts();//54
	}
	if((CruiseON_alerts_popup_shown == 1)||(CruiseOFF_alerts_popup_shown == 1))
	{
		Cruise_alerts();
	}
	if((HDC_Ready_popup_shown == 1)||(HDC_Active_popup_shown == 1)||(HDC_Passive_popup_shown == 1))
	{
		HDC_alert();
	}
	if((Trailer_SW_on_popup_shown == 1)||(Trailer_SW_off_popup_shown == 1))
	{
		Trailer_SW_On_alerts();//58
	}

	if((Previous_Alert_Condition_Transfer_Mode == 1)||(Previous_Alert_Condition_Transfer_Mode == 2))
	{
		Alert_Condition_4WD_Meter();
	}

	if((REGEN_Required_popup_shown == 1)||(REGEN_aboarted_popup_shown == 1)||(REGEN_completed_popup_shown == 1)||(REGEN_started_popup_shown == 1)||(REGEN_notstarted_popup_shown == 1)||(REGEN_requested_popup_shown == 1))
	{
		REGEN_Alert();
	}

	if((Incorrect_DEF_Detected_popup_shown == 1)||(Incorrect_DEF_Detected_Engine_Not_Start_popup_shown == 1)||(No_restart_Check_DEF_popup_shown == 1)||(No_start_Check_DEF_popup_shown == 1)||
		(DEF_Dosing_Malfunction_popup_shown == 1)||(DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown == 1)||(No_restart_Dose_Malfunction_popup_shown == 1)||(No_start_Dose_Malfunction_popup_shown == 1)||
		(DEF_Level_Low_popup_shown == 1)||(DEF_level_Low_Engine_Not_Start_popup_shown == 1)||(No_restart_Fill_DEF_popup_shown == 1)||(No_start_Fill_DEF_popup_shown == 1))
	{
		DEF_alerts();
	}
#ifdef SPRINT4
	if(TDMS_popup_shown == 1)
	{
		TDMS_Alert_Screen_Display();
	}
#endif



}
