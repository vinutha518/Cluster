/* **********************************************************************************************************************
File                    :       alert.h
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */

#include "r_typedefs.h"
#include "my_can.h"


extern void Get_Default_Service_Slab(void);
extern void Door_Open(void);
extern void Engine_oil_pressure_low(void);
extern void Check_ABS(void);
extern void Seat_belt(void);
extern void High_Temp(void);
extern void Low_Brake_Fluid(void);
extern void Fuel_Low(void);
extern void DEF_Level(void);
extern void DPF_Filter_Clogged(void);
extern void REGEN_Alert(void);
extern void DEF_alerts(void);
extern void Park_Break_alert(void);
extern void Water_In_Fuel_alert(void);
extern void Overspeed_alert(void);
extern void Check_EBD_alert(void);
extern void Check_ESP_alert(void);
extern void RKE_Low_Battery_alert(void);
extern void Check_Engine_alert(void);
extern void Service_reminder(void);
extern void Cruise_alerts(void);
extern void Check_EXT_Lamp(void);
extern void ECO_Mode_alerts(void);
extern void PAB_Off_alerts(void);
extern void Trailer_SW_On_alerts(void);
extern void Horn_failure_alerts(void);
extern void DRL_failure_alerts(void);
extern void LEFT_Turn_Lamp_failure_alerts(void);
extern void RIGHT_Turn_Lamp_failure_alerts(void);
extern void Tail_Lamp_failure_alerts(void);
extern void RPAS_Supply_Failure_alerts(void);
//extern void Key_In_Reminder(void);
extern void Func_safety(void);
extern void Stop_for_setting(void);
extern void ETM_LED_test(void);
extern void ETM_Guage_Test(void);
/*************************************************************/
extern void RPAS_Precondition_U350(void);
extern void RPAS_Distance_Display_U350(void);
extern void RPAS_Status_Display_U350(void);
extern void RPAS_Screen_Activation_U350(void);
/*************************************************************/
/*************************************************************/
extern void RPAS_Precondition_W501(void);
extern void RPAS_Distance_Display_W501(void);
extern void RPAS_Status_Display_W501(void);
extern void RPAS_Screen_Activation_W501(void);
extern void Handle_Inactive_alerts(void);
/*************************************************************/
extern void IGN_OFF_CHECKLAMP_ALERT(RX_DATA *can_buffer);
extern void IGN_OFF_DOOROPEN_ALERT(RX_DATA *can_buffer);
extern void IGN_OFF_IMMO(RX_DATA *can_buffer);
extern void Warning_History_dooropen(void);
extern void IO_Control_Message_activation(void);
extern void IO_Control_TFT_Test_Grayscale(void);
extern void IO_Control_TFT_Test_ColourTFT(void);
extern void Door_open_animation_parareset(void);
extern void  IGN_OFF_PARK_LAMP(RX_DATA *can_buffer);
extern void Key_In_Reminder(RX_DATA *can_buffer);
extern void HDC_alert(void);
extern void AT_gear_alert(void);
//extern uint8_t dont_throw_alerts_alert_on_screen_present;

extern uint8_t DoorClose_ON;
extern uint32_t DoorClose_1sec;
extern uint8_t Engine_oil_pressure_low_Timer_ON_flag;
extern uint32_t Engine_oil_pressure_low_Timer_count;
extern uint8_t Check_ABS_Timer_ON_flag;
//extern uint32_t Check_ABS_Timer_count;
//extern uint32_t High_Temp_Timer_count;
extern uint8_t High_Temp_Timer_ON_flag;
extern uint32_t Low_Brake_Fluid_Timer_count;
extern uint32_t PAB_Timer_count;
extern uint8_t Low_Brake_Fluid_Timer_ON_flag;
extern uint8_t Water_In_Fuel_Timer_ON;
//extern uint32_t Fuel_Low_Timer_count;
extern uint8_t Fuel_Low_Timer_ON_flag;
extern _Bool PAB_Timer_ON;
extern _Bool PAB_Time_Done;
extern uint32_t DPF_Clogged_Timer_count;
extern uint8_t DPF_Clogged_Timer_ON_flag;
extern uint8_t LowBrakeFluidSt_Hw_Input;

extern _Bool eng_start_timer_on;
extern uint8_t eng_start_flag;

extern uint8_t Check_Engine_flag;

extern uint32_t IGN_start_time;//timer count for 5sec after IGN on
extern uint8_t IGN_timer_5s;
extern uint8_t WIF_5sec_done;
extern uint8_t Over_speed_flag;

extern _Bool NVM_SERVICE_DONE_flag;
extern uint8_t service_reminder;
extern uint8_t skip_alert_Enable;
extern uint8_t Codriver_flag;
extern uint8_t Driver_flag;
extern uint8_t Rearleft_flag;
extern uint8_t RearRight_flag;
extern uint8_t Hood_flag;
extern _Bool Back_flag;
extern uint8_t alerts_history[25];
/**************************************************************/
extern _Bool staybacktime_active;
extern _Bool RPAS_ERRORtext_display;
extern uint8_t staybacktime_count;
extern uint8_t RPAS_display_dist;
extern uint8_t RPAS_RLeft_imageIndex;
extern uint8_t RPAS_RRight_imageIndex;
extern uint8_t RPAS_RCenter_imageIndex;
extern uint8_t RPAS_LCenter_imageIndex;
extern uint8_t RPAS_DIST_text_display;
extern uint8_t RPAS_All_sensor_error;
extern uint8_t RPAS__update_display_dist;
/******************************************/
extern uint8_t	Dooropen_active_IGNOFF;
extern uint8_t	Checklamp_active_IGNOFF;
extern uint8_t	IGNOFF_Dooropen_check_done;
extern uint8_t	IGNOFF_ExtLamp_check_done;
extern uint8_t	IGNOFF_keyinremind_check_done;
extern uint8_t	Checklamp_active_IGNOFF_Alert;
extern uint8_t	Dooropen_active_IGNOFF_Alert;
extern uint8_t	Keyinreminder_IGNOFF_Alert;
/******************************************/
/***************************Alert_skip*******************************/
extern _Bool Low_Engine_Oil_Pressure_skip;
extern _Bool Engine_Temp_High_skip;
extern _Bool Low_Fuel_popup_skip;
extern _Bool Check_Engine_popup_skip;
extern _Bool skip_flag;
extern _Bool Low_Brake_Fluid_skip;
extern _Bool Park_brake_popup_skip;
extern _Bool PAB_Off_popup_skip;
extern _Bool DPF_Clogged_popup_skip;
extern _Bool Dooropen_popup_skip;
extern _Bool EBD_popup_skip;
extern _Bool Check_ABS_popup_skip;
extern _Bool ESP_popup_skip;
extern _Bool RPAS_Supply_failure_popup_skip;
extern _Bool Low_Battery_popup_skip;
extern _Bool Water_In_Fuel_popup_skip;
extern _Bool Service_Reminder_popup_skip;
extern _Bool Func_safety_popup_skip;
extern _Bool Left_Turn_failure_popup_skip;
extern _Bool Right_Turn_failure_popup_skip;
extern _Bool Left_DRL_failure_popup_skip;
extern _Bool Right_DRL_failure_popup_skip;
extern _Bool Tail_Lamp_failure_popup_skip;
extern _Bool Horn_Fail_popup_skip;
extern _Bool Key_In_Alert_popup_skip;
extern _Bool REGEN_Required_popup_skip;
//#ifdef SPRINT4
extern _Bool TDMS_popup_skip;
//#endif
extern uint8_t	Check_Engine_popup_shown;
extern uint8_t	Low_Fuel_popup_shown;
extern uint8_t	Engine_Temp_High_popup_shown;
extern uint8_t	Low_Engine_Oil_Pressure_popup_shown;
extern uint8_t Check_ABS_popup_shown;
extern uint8_t Low_Brake_Fluid_popup_shown;
extern uint8_t Park_brake_popup_shown;
extern uint8_t Seat_Belt_popup_shown;
extern uint8_t DPF_Clogged_popup_shown;
extern uint8_t Water_In_Fuel_popup_shown;
extern uint8_t Overspeed_popup_shown;
extern uint8_t EBD_popup_shown;
extern uint8_t ESP_popup_shown;
extern uint8_t Low_Battery_popup_shown;
extern uint8_t Service_Reminder_popup_shown;
extern uint8_t	Cruise_alerts_popup_shown;
extern uint8_t	Check_EXT_Lamp_popup_shown;
extern uint8_t Eco_mode_popup_shown;
extern uint8_t PAB_Off_popup_shown;
extern _Bool Trailer_SW_on_popup_shown;
extern _Bool Trailer_SW_off_popup_shown;
extern uint8_t Horn_Fail_popup_shown;
extern _Bool Left_DRL_failure_popup_shown;
extern _Bool Right_DRL_failure_popup_shown;
extern _Bool Left_Turn_failure_popup_shown;
extern _Bool Right_Turn_failure_popup_shown;
extern _Bool Tail_Lamp_failure_popup_shown;
extern _Bool RPAS_Supply_failure_popup_shown;
extern _Bool Key_In_Alert_popup_shown;
extern _Bool Func_safety_popup_shown;
extern _Bool RPAS_ACTIVE_popup_shown;
extern uint8_t Dooropen_IGNOFF_popup_shown;
extern uint8_t Check_EXT_Lamp_IGNOFF_popup_shown;
extern uint8_t Doorclose_popup_shown;
extern uint8_t Dooropen_popup_shown;
extern _Bool Doorclose_IGNOFF_popup_shown;
/********************************************************************/
extern uint8_t STS_Door_data;
extern uint8_t All_doorclose;
extern _Bool All_doorclose_timeout_start;
extern uint8_t IO_Control_TFT;
extern _Bool SBR_Alert_check;
extern _Bool Priority_flag;
extern _Bool DEF_priority_flag;
extern uint8_t prev_STS_Door_data;
extern uint8_t IGNOFFprev_STS_Door_data;
extern _Bool DOOR_OPEN_IGN_OFF;
extern _Bool REGEN_priority_flag;
extern _Bool checkRegenDEF_flag;
extern /*_Bool*/uint8_t RPAS_Precondition_Active;
extern uint8_t Speed_present;
extern _Bool skip_flag;
extern _Bool Notification_chime;
extern _Bool KIR_Chime;
extern _Bool parkLampChime;		/* @@@ Flag for Park Lamp Chime Status */
extern _Bool RPAS_ACTIVE_STS_flag;
extern _Bool SBR_IGN_ON;
extern uint8_t	wait_state;
extern uint8_t	Not_IGN_Wakeup;

/**********************************************************************/
extern _Bool WH_backdoor;
extern _Bool WH_Hooddoor;
extern _Bool WH_codriverdoor;
extern _Bool WH_driverdoor;
extern _Bool WH_Rearleftdoor;
extern _Bool WH_Rearrightdoor;
/************************************************************************/
extern _Bool AT_gear_alert_active;
extern _Bool ATgear_popup_shown;
extern _Bool AT_gear_debounce_start;
extern uint8_t	PREV_TGS_LEVER;
extern uint32_t AT_gear_debounce_cnt;
extern uint32_t AT_gear_update_cnt;
extern uint32_t start_AT_alert_timeout_cnt;
extern uint8_t TGS_LEVER1;
extern uint8_t	AT_Gear_display;
extern _Bool AT_gear_IGN_ON;
extern _Bool AT_gear_IGM_ON_detected;
extern uint8_t PREV_TGS_LEVER1;
/************************************************************************/

#ifdef U350
extern _Bool RPAS_LEVEL1_PRECON;
extern _Bool RPAS_LEVEL2_BUZ_ONGOING;
extern _Bool RPAS_LEVEL3_BUZ_ONGOING;
extern _Bool RPAS_LEVEL4_BUZ_ONGOING;
#endif
