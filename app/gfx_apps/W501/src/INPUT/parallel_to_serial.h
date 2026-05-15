/*
 * parallel_to_serial.h
 *
 *  Created on: 21-Jan-2019
 *      Author: MSWELT19
 */
#ifndef PARALLEL_TO_SERIAL_H_
#define PARALLEL_TO_SERIAL_H_
#define TURN_LEFT_CPU_PORT			0u
#define TURN_LEFT_CPU_PIN			2u
#define 	TurnLTT    PORTJ_AWOJPPR0_2
#define PARK_LAMP_CPU				JP0.1
#define	FRONT_FOG_CPU				JP0.5

#define Ip_shift_reg_1 				lp1.state
#define FOUR_WD_HIGH_CPU			lp1.us.bit0
#define FOUR_WD_LOW_CPU				lp1.us.bit1
#define ECO_CPU						lp1.us.bit2
#define HYBRID_CPU			 		lp1.us.bit3
#define PARK_BRAKE_CPU				lp1.us.bit4
#define DRIVER_SEAT_BELT_CPU 		lp1.us.bit5
#define LOW_BRAKE_FLUID_CPU		 	lp1.us.bit6
#define BATTERY_CHARGE_CPU		 	lp1.us.bit7

#define Ip_shift_reg_2 				lp2.state
#define DIGIN_SPARE_CPU 			lp2.us.bit0
#define TT_SPARE_CPU 				lp2.us.bit1
#define TT_SPARE_CPU1				lp2.us.bit2
#define TT_SPARE_CPU2				lp2.us.bit3
#define CO_DRIVER_SEAT_BELT_CPU 	lp2.us.bit4
#define CAN1_WAKEUP_CPU 			lp2.us.bit5
#define CAN2_WAKEUP_CPU 			lp2.us.bit6
#define FOUR_WD_FEEDBACK_CPU		lp2.us.bit7

#define 	IMMOBIIZER_TT		PORT_AWOP0_4
//#define 	IMMOBIIZER_TT		PORT_ISOP16_7 //dummy

extern uint8_t TURN_RIGHT_CPU,HIGH_BEAM_CPU,REAR_FOG_CPU,FOUR_WD_OUTPUT_CPU,PARALLEL_IN_1,PARALLEL_IN_2;
extern uint8_t dont_throw_alerts_alert_on_screen_present;
extern uint8_t dont_throw_doorclose_on_screen_present;

extern uint8_t regen_req_page_timeout_reached;
//extern uint16_t Eng_No_Start;
extern Split16 Eng_No_Start;


extern uint8_t Check_engine_TT_LOD;
extern uint8_t Check_engine_TT_LSD;
extern uint8_t MIL_TT_LOD;
extern uint8_t MIL_TT_LSD;
extern uint8_t AIR_BAG_TT_LOD;
extern uint8_t AIR_BAG_TT_LSD;
extern uint8_t CRUISE_TT_LOD;
extern uint8_t CRUISE_TT_LSD;
extern uint8_t ESP_MALFUNCTION_TT_LOD;
extern uint8_t ESP_MALFUNCTION_TT_LSD;
extern uint8_t ABS_TT_LOD;
extern uint8_t ABS_TT_LSD;
extern uint8_t PARK_BRAKE_TT_LOD;
extern uint8_t PARK_BRAKE_TT_LSD;
extern uint8_t AUTO_START_STOP_TT_LOD;
extern uint8_t AUTO_START_STOP_TT_LSD;
extern uint8_t AT_FAIL_TT_LOD;
extern uint8_t AT_FAIL_TT_LSD;
/**********************************************************************************************************/
extern uint8_t init_regen_required;			/*Regen Required Alert initiated*/
extern uint8_t init_regen_aboarted;			/*Regen Aboarted Alert disabled*/
extern uint8_t init_regen_started;			/*Regen Started Alert disabled*/
extern uint8_t init_regen_not_started;		/*Regen Not started Alert disabled*/
extern uint8_t init_regen_completed;		/*Regen Completed Alert Enabled*/
extern uint8_t REGEN_TT_ON_flag;
extern uint8_t regen_page_skip;
extern uint32_t display_key;
extern uint8_t Main_page_Regen;
/**********************************************************************************************************/
extern uint8_t REGEN_Required_popup_shown;
extern uint8_t REGEN_completed_popup_shown;
extern uint8_t REGEN_aboarted_popup_shown;
extern uint8_t REGEN_notstarted_popup_shown;
extern uint8_t REGEN_started_popup_shown;
extern uint8_t REGEN_requested_popup_shown;
/*****************************************************/
extern _Bool REGEN_Required_popup_skip;
extern _Bool REGEN_completed_popup_skip;
extern _Bool REGEN_aboarted_popup_skip;
extern _Bool REGEN_notstarted_popup_skip;
extern _Bool REGEN_started_popup_skip;
extern _Bool REGEN_requested_popup_skip;
/*****************************************************/
extern uint8_t Incorrect_DEF_Detected_popup_shown;
extern uint8_t Incorrect_DEF_Detected_Engine_Not_Start_popup_shown;
extern uint8_t No_restart_Check_DEF_popup_shown;
extern uint8_t No_start_Check_DEF_popup_shown;
extern uint8_t DEF_Dosing_Malfunction_popup_shown;
extern uint8_t DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown;
extern uint8_t No_restart_Dose_Malfunction_popup_shown;
extern uint8_t No_start_Dose_Malfunction_popup_shown;
extern uint8_t DEF_Level_Low_popup_shown;
extern uint8_t DEF_level_Low_Engine_Not_Start_popup_shown;
extern uint8_t No_restart_Fill_DEF_popup_shown;
extern uint8_t No_start_Fill_DEF_popup_shown;
/*****************************************************/
extern _Bool Incorrect_DEF_Detected_popup_skip;
extern _Bool Incorrect_DEF_Detected_Engine_Not_Start_popup_skip;
extern _Bool No_restart_Check_DEF_popup_skip;
extern _Bool No_start_Check_DEF_popup_skip;
extern _Bool DEF_Dosing_Malfunction_popup_skip;
extern _Bool DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip;
extern _Bool No_restart_Dose_Malfunction_popup_skip;
extern _Bool No_start_Dose_Malfunction_popup_skip;
extern _Bool DEF_Level_Low_popup_skip;
extern _Bool DEF_level_Low_Engine_Not_Start_popup_skip;
extern _Bool No_restart_Fill_DEF_popup_skip;
extern _Bool No_start_Fill_DEF_popup_skip;
extern _Bool BS6_permanent_alert;
extern uint8_t Regencnt;
extern uint8_t Regencnt1;
extern uint8_t wait_10sec;
extern _Bool check_tt_io_control_once;
extern _Bool IGN_ON_regen_init;
extern _Bool HoldOk_Regen_text;
extern _Bool dpf_check;
extern _Bool DEF_priority_check;

extern uint8_t Buzz_KIR_cntr;
extern _Bool Buzz_KIR_compl;

extern _Bool buzzParkLamp;			/* @@@ Park Lamp Buzz Status */
extern uint8_t buzzParkLampCntr;	/* @@@ Park Lamp Buzz Counter */

extern uint16_t W501_LOW_DID_RPAS_ERROR_TONE_CHECK;
extern uint32_t W501_LOW_CounterBz_3sec;
extern uint32_t W501_LOW_Counter_RPAS_500MS;
extern uint16_t W501_LOW_DID_RPAS_SELF_CHECK;
extern uint8_t cycle_RPAS_self_check_cntr;
extern uint32_t Sound_Bit;
extern _Bool Alert_receive_flag;
/*****************************************************/
/***********************************************************************************************************/
void input_shift_register (void);
void IP_SHIFT_DELAY (void);
void scan_TT_status (void);
void IMMOBOLIZER_TT_IGN_OFF(void);

void fuel_did(void);
void TEMP_did(void);

void IO_CONTROL_TT_FUNCTIONALITY(void);
void W501_RPAS_CHIME_ENABLE(void);
void CHECK_SID_DATA (void);

typedef union{
	struct {
		uint8_t bit0 : 1;
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
        uint8_t bit7 : 1;
	}us;
	uint8_t state;
}Led_state;
extern Led_state lp1,lp2;

extern _Bool update_count;
extern _Bool High_Beam_sts;
extern uint16_t temp_show_TT[4];
extern _Bool OS_L2_Start;//SBR-->OS-->other chime priority
#endif /* PARALLEL_TO_SERIAL_H_ */
