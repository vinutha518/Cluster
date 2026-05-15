/************************************************************************************************
PROJECT : d1mx_mfd_demo
FILE    : $Id: app_init.c 4340 2014-11-14 16:45:29Z miodrag.stankovic $
============================================================================
DESCRIPTION
============================================================================
C O P Y R I G H T
============================================================================
Copyright (c) 2013 - 2014
by
Renesas Electronics (Europe) GmbH.
Arcadiastrasse 10
D-40472 Duesseldorf
Germany
All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.
 ****************************************************************************
 */

/*******************************************************************************
Section: Includes
 */
#define drome
//#define /*GRAYSCALE_TFT*/ COLOR_TFT
#include "dbgmacro.h"
#include "errmgr.h"
#include "dr7f701461.dvf.h"
#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "sound_pcm.h"
#include "UDSS.h"
#include "r_cg_timer.h"
#include "r_gpio_api.h"
#include "r_test_ism.h"
#include "Memmory_Map.h"
#include "fuel.h"
#include "odometer.h"
#include "alert.h"
#include "r_awot_api.h"
#ifdef GRAYSCALE_TFT
#include "lcbi_non_tft.h"
#endif

/*#include "r_ddb_api.h"
#include "r_cdi_api.h"
#include "r_wm_api.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
 */
/*#include "mem_init.h"
#include "drw_wrap.h"
#include "state_machine.h"
#include "sprite.h"
#include "wm.h"
#include "car_model.h"
#include "load_to_ram.h"
#include "r_tick_api.h"
#include "util.h"
#include "standby.h"
#include "my_tell_tale.h"*/
#include "my_can.h"

#include "FOURxFOUR.h"
#ifdef SPRINT4
#include "TDMS.h"
#endif

#include "r_typedefs.h"
#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "AFE_3.h"                  /* Model's header file */
#include "IFE_3_Code.h"
//#include "rtwtypes.h"


#include "sound_pcm.h"
#include "r_dev_api.h"
#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"

#include "r_gpio_api.h"
/*#include "r_taub_isr.h"*/
/*#include "r_taub_main.h"*/
#include "r_taub_api.h"
#include "r_tauj_api.h"
/*#include "r_taub_sys.h"*/
/*Drome Header Files*/
#include "drconf.h"
#include "druserconf.h"
#include "drapp.h"
#include "gui.h"
#include "drome.h"
#include "serial_to_parallel.h"
#include "parallel_to_serial.h"
#include "TLC6C_S_P.h"
#include "UDS_DTC.h"
#include "r_cg_adc.h"
#include "ADC.h"
#include "r_cg_serial_user.h"
#include "r_tick_api.h"
#include "UDSS_APP.h"
#include "gearIndication.h"
#include "digi_clk.h"
#include "rtc.h"
#include "IVN_SecurityAlgorithm.h"
#include "VIN_MISMATCH.h"
#include "DTE_V_1.h"
#include "rtwtypes.h"
#ifdef PRODUCTION
#ifndef U350
#include "EEPROM_defalut_W501.h"
#else
#include "EEPROM_defalut_U350.h"
#endif
#endif
/*******************************************************************************
  Title: ATF Main Module

  Implementation of the application test framework API.
 */

/*******************************************************************************
  Section: Includes
 */

#include <string.h>
#include "digitalGuage.h"
#include "tachometer.h"
#include "swsc.h"
#include "batt.h"
#include "r_dev_api.h"
#include "r_atf_api.h"
#include "r_bsp_stdio_api.h"
#include "r_dev_api.h"
#include "UDSS_APP.h"
#include "r_wdta_api.h"
#ifndef drome/*Non drome files*/
#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "r_bsp_hmi_api.h"     /* Board support package - HMI (Buttons + Rotary) */
#include "r_ddb_api.h"
#include "r_cdi_api.h"
#include "r_wm_api.h"
#include "davehd_driver.h"
#include "r_tick_api.h"
//#include "main.h"
#include "wm.h"
#include "mem_init.h"
#include "r_util_dhd.h"
#include "init.h"
#include "config.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"

#include "r_jcua_api.h"
#include "r_bsp_stdio_api.h"
#include  "TLC6C_S_P.h"
#include "my_tell_tale.h"
#endif

#include "carmodel.h"
#define my_protected_write(preg,reg,value)   preg=0xa5;\
		reg=value;\
		reg=~value;\
		reg=value;
#define protected_write(preg,pstatus,reg,value)   do{\
		(preg)=0xa5u;\
		(reg)=(value);\
		(reg)=~(value);\
		(reg)=(value);\
}while((pstatus)==1u)
#pragma ghs section rodata = ".mysection"
//const unsigned char my_check[]={0x01,0x23,0x45,0x67, 0x01,0x23,0x45,0x67, 0x01,0x23,0x45,0x67, 0x01,0x23,0x45,0x67};
const char validstring[16] = "MSILMAHINDRAW501";
#pragma ghs section rodata = default
#ifdef GRAYSCALE_TFT
drome_Colour buffer[/*240*/320*240] = {0U};
//U8 buffer_grayscale[/*240*/320*240] = {0U};
#endif
/*******************************************************************************
  Section: Local Constants
 */

/*******************************************************************************
  Constant: LOC_USER_MENU

  Define used to specify the actually active menu (loc_Menu or automatic) 
 */
#define LOC_USER_MENU  0u

/*******************************************************************************
  Constant: LOC_AUTO_MENU

  Define used to specify the actually active menu (loc_Menu or automatic) 
 */
#define LOC_AUTO_MENU  1u


/*******************************************************************************
  Section: Local Variables
 */

/*******************************************************************************
  Variable: loc_Menu[]

  Main application manu (constant array). 
 */

static const r_atf_Menu_t* loc_Menu = 0u;

/*******************************************************************************
  Variable: loc_Auto[]

 Automatic execution (constant array) 
 */

static const r_atf_Automatic_t* loc_Auto = 0u;


/*******************************************************************************
  Variable: loc_MenuPreamble

 Informative text regarding this test application. To be printed below the menu header. 
 */

static char * loc_MenuPreamble = 0u;


/*******************************************************************************
  Variable: loc_AutoStart

  In case when  loc_Menu _AND_ loc_Auto are set, this variable decides if the 
  loc_Auto will be handled/executed after loc_Menu is done. This is provided 
  for cases where the automatic execution shall be called from within loc_Menu and/not
  autonomously 
 */
static uint8_t loc_AutoStart             = 1u; 


/*******************************************************************************
  Variable: loc_UserAutoConfig

  A function pointer to a user function (configuration for automated excution)  

  The value is assigned (or not) by the application initialisation 
 */

static void (*loc_UserAutoConfig )(void) = 0u;


/*******************************************************************************
  Variable: loc_ActiveEntry

 Stores the index of the Menu entry of the currently executed function 
 */

static uint32_t loc_ActiveEntry = 0u;

/*******************************************************************************
  Variable: loc_ActiveMenu

 Stores the inormation which  Menu is being currently handled  
 */

static uint8_t loc_ActiveMenu   = 0u;

/*******************************************************************************
  Variable: loc_SkipEpilogue

  A flag used to disable any Epilogue (wait for int8_t) functionalty   
  It is always set when ececuting automatic functions and can be additionally 
  set and reset by application (see R_ATF_SetAutoConfig()) 

 */

static uint8_t loc_SkipEpilogue          = 0u; 

extern void Check_Valid_Feature_Code(void);
extern void Check_Vaild_Variant_Code(void);

void Scheduler(void);
void proveout_routine(void);
void ETM_Guage_Test(void);
Public void draw_grayscale(uint32_t r,uint32_t c);
void Init_read_parameters(void);
unsigned char checkvalidgraphicsappln(unsigned long appAddr);
void api_fcl_read(unsigned long adr, unsigned long count,unsigned char *data);
extern void BOOTUP_WriteImage(void);
extern void Mahindra_TextImage(void);
void Standby_Mode(void);
uint8_t Read_RTC_Format(void);
void FOUR_WD_INIT_STS(void);

uint8_t BATT_DISCONNECTION;
_Bool Batt_disconnect_AFE_RST = 0;
uint8_t BATT_DISCONNECTION1 = 0;
uint8_t immo_TT_config=0;
uint16_t AFEDispVal_init;             /* '<Root>/U16_AFEDispVal' */
_Bool AFE_calstart = 0;
_Bool ModelRunCheck_flag = 0; /*Check DTC calculate done */

extern _Bool zpd_done;
extern _Bool UDS_TASK;
extern _Bool Update_FStepper_Flag;
extern _Bool Read_ADC_Flag;
extern _Bool Read_TT_flag;
extern _Bool Counter_1000ms_flag;
extern  _Bool Update_ign_Flag;
extern  _Bool WatchDog_flag;
extern uint16_t gcsk_flag;
extern uint32_t display_tachometer_speed;
uint16_t Ign_Off_Counter;

uint32_t wakeup_factor;
extern _Bool set_key_short;
extern uint32_t Trip_AC;
extern uint32_t prev_sbrchm_odo;
extern uint16_t key_press_counter_tpms;
//extern _Bool EEPROM_IN_USE;
extern _Bool PARK_LAMP_IGN_OFF;
extern _Bool DOOR_OPEN_IGN_OFF;
extern _Bool IMMO_IGN_OFF;
_Bool AFE_start = 0;
extern uint8_t TURN_TT_TIMEOUT;
extern uint16_t Display_DTE;
extern _Bool flag_DTE_Enable;
extern _Bool NotdisplayAFE;
extern unsigned int RPAS_LV1_COUNTER;
extern unsigned int RPAS_LV2_COUNTER;
extern unsigned int RPAS_LV3_COUNTER;
extern unsigned int RPAS_LV4_COUNTER;
extern _Bool W501_RPAS_LEVEL_1_BUZ;
extern _Bool W501_LOW_RPAS_LEVEL1_PRECON;
extern uint8_t W501_LOW_RPAS_COUNTER2;
extern uint8_t W501_LOW_RPAS_COUNTER3;
extern uint8_t W501_LOW_RPAS_COUNTER4;

extern unsigned char turn_activate;

extern _Bool parkLampChime;	/* @@@ Flag for Park Lamp Chime Status */
extern _Bool buzzParkLamp;			/* @@@ Park Lamp Buzz Status */
extern uint8_t buzzParkLampCntr;	/* @@@ Park Lamp Buzz Counter */
extern uint8_t priorityPark_Keyin;
extern uint8_t Buzz_KIR_cntr;
extern _Bool KIR_Chime;
extern _Bool Buzz_KIR_compl;

#define LOC_SOFTWARE_VERSION    "V1.0"
#define IGN 	R_GPIO_PinRead(0,1)//	PORT_AWOP0_1  /* Ignition */
#define CANWAKEUP 	R_GPIO_PinRead(0,0)//	PORT_AWOP0_1  /* INH */
//uint8_t IGN;

/* test pins for measurment */
#define LOC_LED0_PORT    16u
#define LOC_LED0_PIN      0u
#define LOC_LED1_PORT    16u
#define LOC_LED1_PIN      1u
#define LOC_LED0_PORTW    0u
#define LOC_LED0_PINW      4u
#define LOC_LED0_PORTL    16u
#define LOC_LED0_PINWL      7u

#define LOC_LED0_PORTCNW    3u
#define LOC_LED0_PINWCNW      4u
#define LOC_LED0_PORTCNSL    16u
#define LOC_LED0_PINWCNSL      8u

#define CAN_RX_PORT        0u
#define CAN_RX_PIN         9u

#define CAN_TX_PORT        0u
#define CAN_TX_PIN         8u
#define LED_SI_PORT   	 		11u
#define LED_SI_PIN     		 	3u
#define LED_CLOCK_PORT    		11u
#define LED_CLOCK_PIN      		2u
#define LATCH_CLR_PORT    		11u
#define LATCH_CLR_PIN     	 	0u
#define LED_ENABLE_PORT    		11u
#define LED_ENABLE_PIN      	1u
#define LATCH_ENABLE_PORT    	10u
#define LATCH_ENABLE_PIN      	11u
/*IP_SHIFT_REG_PIN*/
#define PARALLEL_IN_1_PORT   	10u			/*SHIFT_DATA1*/
#define PARALLEL_IN_1_PIN  		7u
#ifdef PCBrevB
#define PARALLEL_IN_2_PORT    	10u
#define PARALLEL_IN_2_PIN   	6u
#else
#define PARALLEL_IN_2_PORT    	42u
#define PARALLEL_IN_2_PIN   	15u
#endif
#define SHIFT_PL_PORT           10u
#define SHIFT_PL_PIN            8u
#define SHIFT_CP_PORT           10u
#define SHIFT_CP_PIN            9u
#define SHIFT_CE_PORT           10u
#define SHIFT_CE_PIN            10u
/*DIAGN_OP_SHIFT_REG_PIN*/
#define DLED_GCLK_PORT			17u
#define DLED_GCLK_PIN			8u
#ifdef PCBrevB
#define DLED_ERRO_PORT			1u
#define DLED_ERRO_PIN			5u
//Diad Shift Reg
#define DLED_SDO_PORT           1u
#define DLED_SDO_PIN            0u
#define DLED_SDI_PORT	   	 	1u
#define DLED_SDI_PIN	    	1u
#else
#define DLED_ERR1_PORT			1u
#define DLED_ERR1_PIN			3u
#define DLED_ERR0_PORT			1u
#define DLED_ERR0_PIN			5u
//Diad Shift Reg
#define DLED_SDO_PORT           1u
#define DLED_SDO_PIN            1u
#define DLED_SDI_PORT	   	 	1u
#define DLED_SDI_PIN	    	0u
#endif
#define DLED_SCL_PORT	    	1u
#define DLED_SCL_PIN	    	2u
#define DLED_LATCH_PORT			1u
#define DLED_LATCH_PIN			4u
#define DLED_BLANK_PORT			17u
#define DLED_BLANK_PIN			7u
/*TT_IO-PIN*/
#define TURN_RIGHT_CPU_PORT			0u
#define TURN_RIGHT_CPU_PIN			2u
#define HIGH_BEAM_CPU_PORT			0u
#define HIGH_BEAM_CPU_PIN			3u
#define	REAR_FOG_CPU_PORT			0u
#define	REAR_FOG_CPU_PIN			5u
#define FOUR_WD_OUTPUT_CPU_PORT			17u
#define FOUR_WD_OUTPUT_CPU_PIN		10u

#define FOUR_WDT_OUTPUT_CPU_PORT	42u
#define FOUR_WDT_OUTPUT_CPU_PIN		14u

/* for ADC */
#define ADCE0I0_FUEL_PORT   10U
#define ADCE0I0_FUEL_PIN     0U
#define ADCE0I2_SWSC_PORT   10U
#define ADCE0I2_SWSC_PIN     2U
#define ADCE0I4_BATT_PORT   10U
#define ADCE0I4_BATT_PIN     4U

/* for TFT HDMI */
#define TFT_HDMI_44PORT     44U
#define TFT_HDMI_0PIN        0U
#define TFT_HDMI_1PIN        1U
#define TFT_HDMI_2PIN        2U
#define TFT_HDMI_3PIN        3U
#define TFT_HDMI_4PIN        4U
#define TFT_HDMI_5PIN        5U
#define TFT_HDMI_6PIN        6U
#define TFT_HDMI_7PIN        7U
#define TFT_HDMI_8PIN        8U
#define TFT_HDMI_9PIN        9U
#define TFT_HDMI_10PIN      10U
#define TFT_HDMI_11PIN      11U

#ifdef COLOR_TFT
unsigned long String_Graphics_Address = 0x10000030;//0x1C0030;
#else
unsigned long String_Graphics_Address = 0x1C0030;
#endif

uint8_t rtc_wakeup = R_FALSE;
uint8_t IGN_ON_flag = 0;
uint8_t IGN_ON_fuel = 0;
_Bool update_fuel_Bar = 0;
uint8_t IGN_ON_for_fuel = 0;
uint16_t Proveout_Count =0;
uint8_t INIT_DONE_flag = 0;
uint8_t IGN_toggle_flag = 0;
_Bool Off_diagTTflag;
_Bool LVSD_SET_init=0;
_Bool LVSD_Clear_init=0;
uint8_t LVHV_detect=0;
_Bool welcomedone=0;
extern _Bool Enter_while_flag;
_Bool TPMS_ENTER_PR = 0;//r
extern uint16_t final_vechile_speed_gauge_raw;
/*rtc*/
uint8_t hrs;
uint8_t min;
uint8_t sec;
uint8_t test_rtc_reset_flag;
_Bool execute_ones_flag;
extern uint8_t Diag_Error,diagTTflag;
/*Service_reminder Ack*/
uint8_t key_press_bfr_ign = 0;
_Bool Ok_Pressed = 0;
/*Odo Reset*/
_Bool Menu_Pressed = 0;
/*ETM Entry*/
_Bool ETM_Mode_Enter_timer = 0;
/*AFE DATA variable*/
extern uint32_t AccFuelConsp;  ;
extern uint32_t AccFuelConspGD;
extern uint32_t OdoDispReset;          /* '<Root>/U32_OdoDispReset' */
extern uint32_t OdoDispGDReset;
/*AFE DATA end*/
extern _Bool Counter_500ms_popup;
extern unsigned int Counter_500ms;
//extern ExtU rtU;
extern uint32_t IVN_EEFreshnessCounter;
extern ExtU_AFE_3_T AFE_3_U;
extern ExtY_AFE_3_T AFE_3_Y;
extern _Bool PROVEOUT_OK;
extern _Bool GI_CONTROL_FLAG;
_Bool Park_lamp_TT_IGN_Off;
_Bool IMMO_TT_WAKEUP_FLAG;
/******************************************************************************/
uint32_t Init_Drivesum_odo_distance = 0U;
uint32_t Init_Drivesum_DT_sec_count = 0U;
_Bool Drive_summery_tout_flag = 0;
uint32_t dromecall_timeout = 0;
_Bool Drive_summery_tout_flag1 = 0;
_Bool parameter_writeflag = 0;
extern _Bool write_eeprom_flag;
extern uint64_t start_driveSum_timeout_3sec;
uint32_t move_out_of_standby;
uint32_t move_out_of_standby_park;
extern uint8_t IGNOFF_display_timeout;
extern uint8_t welcome_index;
uint32_t startkey = 0;
Split32 Default_pagekey = 0;
extern uint32_t AFE_rst_odo_dist;	/*AFE issue related to IGN OFF-ON showing wrong value*/
extern Split16 AFEDispVal_EEPROM;
extern Split16 DTEDispVal_EEPROM;
_Bool EEPROM_readwrite_para_started = 0;
_Bool EEPROM_readwrite_para_started_on = 0;
_Bool IGNOFF_eeprom_read = 0;
_Bool gui_shutdown = 0;

extern _Bool Buzz_ON;
_Bool One_time_Buzz_OFF=0;

uint32_t fuel_in_ign_on;
extern Split32 FUEL_IN_IGN;
_Bool Read_odo_flag,Read_Para_Flag = 0;
_Bool TT_IGN_OFF;
_Bool IGN_OFF_TT;
extern _Bool tst_tt_flag;
extern _Bool tst_tt_flag_1;
extern _Bool tst_tt_flag_2;
extern _Bool tst_tt_flag_3;
extern _Bool tst_tt_flag_4;
extern _Bool tst_tt_flag_5;
extern _Bool tst_tt_flag_6;
extern _Bool tst_tt_flag_7;
extern _Bool tst_tt_flag_8;
extern _Bool tst_tt_flag_9;
extern _Bool tst_tt_flag_10;
extern uint8_t AFE_DRIVE_CYCLE_RESET_INTERVAL;
extern _Bool Flag_AFE_reset;
extern uint8_t flag_grayscale;
extern uint8_t refresh_rate;
extern uint8_t Diag_Unit_Selection;
extern uint32_t DTE_Disp_Val;
uint8_t STS_Eng_data1 = 0;
extern uint8_t HMI_AVS_DASH1,HMI_AVS_DASH2;
extern _Bool TT4WH_1000ms_cmplt_flg;
extern _Bool TT24WH_1000ms_cmplt_flg;

extern uint8_t currIllumination;	/* @@@ This Variable Stores the Current Value of Illumination Status from NVM */
uint8_t first_tt_input=0;
extern uint16_t tt_out_cnt;

extern uint16_t AvgKmplAFE_GD;
/******************************************************************************/
//extern uint8_t EEPROM_LOG;//delete before release
static const r_dev_PinConfig_t loc_PinConfig_TFT[] =
{{17, 3,  2u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1} /*TFT Backlight*/};
uint8_t animation_start = 0;
extern uint16_t Diag_Ignition_counter;
uint16_t reset_counter;//cluster reset count
uint8_t bufferinit_flag=0;
_Bool Odo_reset_8sec_on = 0;
uint16_t AFE_RST_Time = 0;//Ignition off time to reset AFE
uint16_t AFE_RST_Time_IGN_off = 0;//count of minute
uint16_t AFE_RST_Time_IGN_on = 0;//count of minute
_Bool Graphics_Loaded;
extern uint64_t Counter_13sec_ign;
_Bool display_start = 0;
_Bool door_statusflag = 0;
_Bool drive_sum_flag = 0;
_Bool entry_drivesummery = 0;
_Bool drive_summery_start = 0;
_Bool drive_summery_page_draw = 0;
uint8_t STS_4WD = 0;
//uint8_t test_cnt;
unsigned int READ_RTC_RESET;
uint8_t WDTA1_INIT_COMPLETE=0;

/* Assign x1 to section .data1 */
#pragma ghs section data=".rdata"
uint8_t *RRAM_hrs;
uint8_t *RRAM_min;
uint8_t *RRAM_sec;

/*IMMO*/
uint8_t *RRAM_IMMO_PATT4;
uint8_t *RRAM_IMMO_PATT5;
extern _Bool previous_state_immobilizer_blink4;
extern _Bool previous_state_immobilizer_blink5;

/* Now go back to default rules */
#pragma ghs section data=default



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
/******************************************************/
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

	}IGNoff_status;
/******************************************************/
	static union
	{
		struct{
			unsigned char  Rx_1:1;
			unsigned char  Rx_2:1;
		}bits;
		unsigned char byte;

	}Regen_data;
/********************************************************/

static const r_dev_PinConfig_t loc_PinConfig[] =
		/*   Port                 Pin              Func  Dir               Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
		/* CAN */
		{0,   9,  1u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1}, /* CAN2RX */
		{0,   8,  0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1}, /* CAN2TX */
		{LOC_LED0_PORTCNW,    LOC_LED0_PINWCNW,   0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{LOC_LED0_PORTCNSL,   LOC_LED0_PINWCNSL,  0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{0, 6, 4u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN1TX */
		{0, 7, 4u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN1RX */
		//{3, 4, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN2 EN */
		{3, 6, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN1 FLAG */
		{16, 8,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN2 Standby */
		{16, 9,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN2 FLAG */

		/* Port configuration for ADC */
		{ADCE0I0_FUEL_PORT,   ADCE0I0_FUEL_PIN,    0u,     R_DEV_PIN_IN,     0u,    R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1}, /* Fuel */
		{ADCE0I2_SWSC_PORT,   ADCE0I2_SWSC_PIN,    0u,     R_DEV_PIN_IN,     0u,    R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1}, /* SWSC */
		{ADCE0I4_BATT_PORT,   ADCE0I4_BATT_PIN,    0u,     R_DEV_PIN_IN,     0u,    R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1}, /* BATT */
		{10, 1, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* FUEL_INPUT_REF */
		{10, 3, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* TFT_NTC+ */

#ifdef COLOR_TFT
		/* TFT HDMI PIN */
		/*commented*//*{TFT_HDMI_44PORT, TFT_HDMI_0PIN,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, */ /* VDCE0 Out Data 23 */
		/*commented*//*{TFT_HDMI_44PORT, TFT_HDMI_1PIN,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, */
		/*commented*//*{TFT_HDMI_44PORT, TFT_HDMI_2PIN,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, */
		///*changed*/{TFT_HDMI_44PORT, TFT_HDMI_3PIN,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
		///*changed*/{TFT_HDMI_44PORT, TFT_HDMI_4PIN,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
		/*ok*/{44, 6,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*B7*/
		/*ok*/{44, 7,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*B6*/
		/*ok*/{44, 8,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*B5*/
		/*ok*/{44, 9,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*B4*/
		/*ok*/{44, 10, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*B3*/
		/*ok*/{44, 11, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*B2*/
		/*{45, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, *//* VDCE0 Out CLKN */
		/*ok*/{45, 2,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*G7*/
		/*ok*/{45, 3,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*G6*/
		/*ok*/{45, 4,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*G5*/
		/*ok*/{45, 5,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*G4*/
		/*ok*/{45, 6,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*G3*/
		/*ok*/{45, 7,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*G2*/
		/*ok*/{45, 8,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*R7*/
		/*ok*/{45, 9,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*R6*/
		/*ok*/{45, 10, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*R5*/
		/*ok*/{45, 11, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*R4*/
		/*ok*/{45, 12, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*R3*/
		/*ok*/{45, 13, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*R2*/
		/*ok*/{45, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out CLKP */
		/*ok*/{43, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON2 HS */
		/*ok*/{43, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON0 VS */
		/*ok*/{45, 1,  4u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*Enable. Tried changing to VDCE0_VO_CLKN and VDCE0_VO_TCON3*/
		/*ok*/{44, 5,  0u/*0u*/, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*Reset make low to high*/
		/*ok*/{44, 3,  0u/*0u*/, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},/*Change for PCB rev B*/
#endif
#ifdef GRAYSCALE_TFT
		/* Mono TFT */
		/**/{44, 3,  3u/*0u*//*4u*/, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* CS */
		/**/{44, 4,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* XWR */
		/**/{44, 1,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* XRD */
		/**/{45, 1,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* A0 */
		/**/{44, 6,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D0 */
		/**/{44, 7,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D1 */
		/**/{44, 8,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D2 */
		/**/{44, 9,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D3 */
		/**/{44,10,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D4 */
		/**/{44,11,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D5 */
		/**/{45, 2,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D6 */
		/**/{45, 3,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* D7 */
		/**/{44, 5,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* reset */
		//{17, 3,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* backlight */
#endif

		{42, 12, 0u, R_DEV_PIN_OUT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CE for input shift reg */
		/* Serial Flash */
		{21, 0, 1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serail Flash_SCK */
		{21, 1, 1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serail Flash_CE */
		{21, 2, 1u, R_DEV_PIN_DIRECT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash1_SI (IO0) */
		{21, 3, 1u, R_DEV_PIN_DIRECT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash1_SO(IO1) */
		{21, 4, 1u, R_DEV_PIN_DIRECT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash1_WP# (IO2) */
		{21, 5, 1u, R_DEV_PIN_DIRECT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash1_HOLD#(IO3) */
#if 0
		{21, 6, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash2_SI (IO0) */
		{21, 7, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash2_SO(IO1) */
		{21, 8, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash2_WP# (IO2) */
		{21, 9, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash2_HOLD#(IO3) */
		{42, 0, 6u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SF1_SCK */
		{42, 1, 6u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SF1_CE# */
		{42, 2, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash3_SI (IO0) */
		{42, 3, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash3_SO(IO1) */
		{42, 4, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash3_WP# (IO2) */
		{42, 5, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash3_HOLD#(IO3) */
		{42, 6, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash4_SI (IO0) */
		{42, 7, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash4_SO(IO1) */
		{42, 8, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash4_WP# (IO2) */
		{42, 9, 6u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Serial Flash4_HOLD#(IO3) */
#endif
		/* Motor => Speed, RPM */
		{16, 0,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SPEEDO_P1 */
		{16, 1,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SPEEDO_N1 */
		{16, 2,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SPEEDO_P2 */
		{16, 3,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SPEEDO_N2 */
		{16, 4,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* RPM_P1 */
		{16, 5,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* RPM_N1 */
		{16, 6,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* RPM_P2 */
		{16, 7,  1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* RPM_N2 */

		/* GI_CONTROL */
		{16, 11, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* GI_CONTROL */
		{17, 0,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* GI_CONTROL1 */
		{17, 1,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* GI_CONTROL2 */
		{17, 2,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* GI_CONTROL_POINTER */
#ifdef GRAYSCALE_TFT
		{17, 3,  2u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /*TFT Backlight*/
#else
		{17, 3,  2u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /*TFT Backlight*/
#endif
		/* Output Control */
#ifdef PCBrevB
		{17, 9,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* 3.3V_REG_EN */
		{17, 11, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* FUEL_CONTROL */
		{1, 3, 0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},    /* 5V CONTROL SWSC*/
		/*	{0, 4, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  */ /* WDI */
#else
		{0, 4,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* 3.3V_REG_EN */
		{17, 11, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* FUEL_CONTROL */
		{1 ,3,       0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},/* 5V CONTROL SWSC*/ /*DLED_ERR1*/
		{1, 5, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  /*DLED_ERR0*/
#endif

		{LOC_LED0_PORTW,      LOC_LED0_PINW,   0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  /* WDI */

		/* EEPROM => I2C */
		{3, 0, 0u, R_DEV_PIN_OUT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* IGN_SENSE */
		{3, 1, 0u, R_DEV_PIN_OUT,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* IGN_SENSE */

		/* Speaker */
		{3, 2, 1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* PCMPTPWMAP0 */
		{3, 3, 1u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* PCMPTPWMAN0 */
		{17, 6,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Audio_shutdown */


		/* TT Sense */
#ifdef PCBrevB
		{3, 8, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* E_Trip Switch */
		{3, 9, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* E_SET Switch */
		{10, 5, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* 5V Control TT Driver */
#else
		{3, 9, 0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},    /* 5V CONTROL SWSC*/
		{3, 8, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* 5V Control TT Driver */
#endif
#ifdef U350
		{17, 4,  0u, R_DEV_PIN_IN, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Mode Switch */
		{17, 5,  0u, R_DEV_PIN_IN, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SET Switch */
#else/*set pins to output when in W501*/
		{17, 4,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Mode Switch */
		{17, 5,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* SET Switch */
#endif
		/* Shift reg */
		/*{10, 6, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* SHIFT_DATA2 */
		/*{10, 7, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* SHIFT_DATA1 */
		/*IP_SHIFT_REG_PIN*/
		{PARALLEL_IN_1_PORT ,PARALLEL_IN_1_PIN,  0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{PARALLEL_IN_2_PORT ,PARALLEL_IN_2_PIN,  0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{SHIFT_PL_PORT ,	SHIFT_PL_PIN,        0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{SHIFT_CP_PORT ,	SHIFT_CP_PIN,        0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{SHIFT_CE_PORT ,	SHIFT_CE_PIN,        0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		/*{10, 8, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* SHIFT_PL */
		/*{10, 9, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* SHIFT_CP */
		/*{10,10, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* SHIFT_CE */


		/* PWM O/P => Parallal to Serial */

		/*NON_DIAGN_OP_SHIFT_REG_PIN*/
		{LATCH_ENABLE_PORT,   LATCH_ENABLE_PIN, 0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{LED_SI_PORT,         LED_SI_PIN,       0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{LED_CLOCK_PORT,      LED_CLOCK_PIN,    0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{LATCH_CLR_PORT,      LATCH_CLR_PIN,    0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{LED_ENABLE_PORT,     LED_ENABLE_PIN,   0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		/*{11, 0, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* LED_CLEAR */
		/*{11, 1, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* LED_ENABLE */
		/*{11, 2, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* LED_CLK */
		/*{11, 3, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* LED_SI */
		/*{10,11, 0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* LED_RCK */

		/*TT_IO-PIN*/
		//{TURN_RIGHT_CPU_PORT,TURN_RIGHT_CPU_PIN, 0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{HIGH_BEAM_CPU_PORT ,HIGH_BEAM_CPU_PIN,  1u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{REAR_FOG_CPU_PORT ,REAR_FOG_CPU_PIN, 	 0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{ FOUR_WD_OUTPUT_CPU_PORT,FOUR_WD_OUTPUT_CPU_PIN, 0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		//{16, 10, 0u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* IMMOBILIZER_LED */
		/*{17, 10, 2u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DIGI_OUTPUT */
		//	{ FOUR_WDT_OUTPUT_CPU_PORT,FOUR_WDT_OUTPUT_CPU_PIN, 0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},

#ifdef GRAYSCALE_TFT
		/* Buzzer for mono */
		{42,13, 3u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* Buzzer */
#endif

		/* DLED Driver => Serial to Parallal */

		/*DIAGN_OP_SHIFT_REG_PIN*/
		{DLED_GCLK_PORT ,	DLED_GCLK_PIN,       2u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{DLED_BLANK_PORT ,	DLED_BLANK_PIN,      0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
#ifdef PCBrevB
		{DLED_ERRO_PORT ,	DLED_ERRO_PIN,       0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
#else
		{DLED_ERR0_PORT ,	DLED_ERR0_PIN,       0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{DLED_ERR1_PORT ,	DLED_ERR1_PIN,       0u,   R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
#endif
		{DLED_SDI_PORT ,	DLED_SDI_PIN,        0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{DLED_SCL_PORT ,	DLED_SCL_PIN,        0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{DLED_LATCH_PORT ,	DLED_LATCH_PIN,      0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
		{DLED_SDO_PORT,     DLED_SDO_PIN, 		 0u,   R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* DLED_SDO */

		/*{1, 1, 2u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DLED_SDI */
		/*{1, 2, 2u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DLED_SLK */
		/*{1, 4, 4u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DLED_LATCH */
		/*{1, 5, 0u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DLED_ERR0 */
		/*{17, 7,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DLED_BLANK */
		/*{17, 8,  0u, R_DEV_PIN_OUT,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* DLED_GCLK */

		/* Interrupt */
		{0, 0, 4u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* CAN_WAKEUP */
		{0, 1, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* IGN_SENSE */
		{0, 2, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},/* TURN RIGHT*/
		/*{0, 2, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* TURN_RIGHT */
		/*{0, 3, 1u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* HIGH BEAM */
		/*{0, 5, 4u, R_DEV_PIN_IN,   0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},*/ /* REAR_FOG */
		/*to be removed*/
		/*{LOC_LED0_PORT,       LOC_LED0_PIN,    0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
				{LOC_LED1_PORT,       LOC_LED1_PIN,    0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
				{LOC_LED0_PORTW,      LOC_LED0_PINW,   0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},
				{LOC_LED0_PORTL,      LOC_LED0_PINWL,  0u,   R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},*/
		/* delimiter - do not remove */
		{0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}
};

/*static ee_Wakeup_t locWakeup;*/

void jp02_isr()
{
	if(turn_activate >= 3 && EEPROM_readwrite_para_started_on ==0)
	{
		if( PORTJ_AWOJPPR0_2 == 1)
		{
			Turn_left_TT = 0xfff;
			tt_out_cnt=257;
			//DID_TT_bit_data_set4.bits.Rx_6=1;
		}
		else
		{
			Turn_left_TT = 0;
			tt_out_cnt=257;
			//DID_TT_bit_data_set4.bits.Rx_6=0;
		}
		if(PROVEOUT_OK==0 && IGN_ON_flag == SET)
		{
					Turn_left_TT = 0;
		}
	}
	TURN_TT_TIMEOUT=5;
	INTC1.EIC13.BIT.EIRF13 =0;
}


void init_port_jp0_2(void) {                 /* initialize ports */


	R_GPIO_SetIntCallback(7,jp02_isr );
	R_GPIO_EnableInt(7);
	PORTJ_AWOPCR_JPMC0_2 =1;
	PORTJ_AWOPCR_JPM0_2 =1;

	PORTJ_AWOPCR_JPD0_2 =1; // 1= onchip pull down if board doesn't have Pull down
	// PORTJ_AWOPCR_JPU0_2 =1;
	PORTJ_AWOPCR_JPFC0_2 =0;
	PORTJ_AWOPCR_JPFCE0_2=0;

	PORTJ_AWOJPIBC0_2 =0;
	//FCLA1CTL2 = 0x02; // falling edge
	FCLA1CTL0 = 0x03; // Rising edge
	INTC1EIP13 = 0;   // INTP7
	INTC1.EIC13.BIT.EIRF13 =0;
}


void jp01_isr()
{


	if(FEATURE_CODE_PARK_TT_TYPE == 1)//only execute in HW configuration only
	{
		if(PORTJ_AWOJPPR0_1 == 1)
		{
			PARK_LAMP_TT = 1;
			if(update_count == 0)
			{
				Counter_500ms = 4900;
				update_count = 1;
				SendGSData(GSdata1);
				output_shift_register();
			}
		}
		else
		{
			PARK_LAMP_TT = 0;
			if(update_count == 1)
			{
				Counter_500ms = 4900;
				update_count = 0;
				SendGSData(GSdata1);
				output_shift_register();
			}
		}


	}
	INTC1.EIC15.BIT.EIRF15 =0;
}

void init_port_jp0_1(void) {                 /* initialize ports */


	R_GPIO_SetIntCallback(9,jp01_isr );
	R_GPIO_EnableInt(9);
	PORTJ_AWOPCR_JPMC0_1 =1;
	PORTJ_AWOPCR_JPM0_1 =1;

	PORTJ_AWOPCR_JPD0_1 =1; // 1= onchip pull down if board doesn't have Pull down
	// PORTJ_AWOPCR_JPU0_2 =1;
	PORTJ_AWOPCR_JPFC0_1 =0;
	PORTJ_AWOPCR_JPFCE0_1=0;

	PORTJ_AWOJPIBC0_1 =0;
	//FCLA1CTL2 = 0x02; // falling edge
	FCLA1CTL2 = 0x03; // Rising edge
	INTC1EIP15 = 0;   // INTP7
	INTC1.EIC15.BIT.EIRF15 =0;
}

#if 1
/*************/
void p02_isr()
{
	if(turn_activate >= 3 && EEPROM_readwrite_para_started_on ==0)
	{
		if(R_GPIO_PinRead(0, 2))
		{
			Turn_right_TT = 0xfff;
			tt_out_cnt=257;
			//DID_TT_bit_data_set4.bits.Rx_6=1;
			//IMMOBIIZER_TT=1;
		}
		else
		{
			Turn_right_TT = 0;
			tt_out_cnt=257;
			//DID_TT_bit_data_set4.bits.Rx_6=0;
			//IMMOBIIZER_TT=0;
		}
		if(PROVEOUT_OK==0 && IGN_ON_flag == SET)
		{
				Turn_right_TT = 0;
		}
	}
	TURN_TT_TIMEOUT=5;
	INTC1.EIC7.BIT.EIRF7 =0;
}

#endif

#if 0
uint16_t turn_watchdog = 0;
uint16_t turn_watchdog_on = 0;

/*************/
void p02_isr()
{
	if(turn_activate >= 3 && EEPROM_readwrite_para_started_on ==0)
	{
		if(R_GPIO_PinRead(0, 2))
		{
			Turn_right_TT = 0xfff;
			tt_out_cnt=257;
			//DID_TT_bit_data_set4.bits.Rx_6=1;
			//IMMOBIIZER_TT=1;
			if(IGN==0)
			{
				turn_watchdog_on=0;
				turn_watchdog++;
				if(turn_watchdog>=90)
					{
					while(1);

					}
			}
			else
			{
				turn_watchdog=0;
				turn_watchdog_on++;
				if(turn_watchdog_on>=240)
					{

					while(1);

					}
			}
		}
		else
		{
			Turn_right_TT = 0;
			tt_out_cnt=257;
			//DID_TT_bit_data_set4.bits.Rx_6=0;
			//IMMOBIIZER_TT=0;
		}
		if(PROVEOUT_OK==0 && IGN_ON_flag == SET)
		{
				Turn_right_TT = 0;
		}
	}
	TURN_TT_TIMEOUT=5;
	INTC1.EIC7.BIT.EIRF7 =0;
}

#endif




void init_port_p0_2(void) {                 /* initialize ports */


	R_GPIO_SetIntCallback(1,p02_isr );
	R_GPIO_EnableInt(1);
	PORTJ_AWOPCR_JPMC0_2 =1;
	PORTJ_AWOPCR_JPM0_2 =1;

	PORTJ_AWOPCR_JPD0_2 =1; // 1= onchip pull down if board doesn't have Pull down
	PORTJ_AWOPCR_JPFC0_2 =0;
	PORTJ_AWOPCR_JPFCE0_2=0;

	PORTJ_AWOJPIBC0_2 =0;
	//FCLA1CTL2 = 0x02; // falling edge
	FCLA0CTL2 = 0x03; // Rising edge
	INTC1EIP7 = 0;   // INTP7
	INTC1.EIC7.BIT.EIRF7 =0;
}

/******************************************************************************/
/************************************high_beam******************************************/

void p03_isr()
{
	if((IGN_ON_flag == 1) && (LVSD_flag == 0U) && (HVSD_flag == 0U))
	{
		if(FEATURE_CODE_HIGHBEAM_TT_TYPE == 1)//only execute in HW configuration only
		{
			if(R_GPIO_PinRead(0, 3))
			{
				High_beam_TT = 0xfff;
			}
			else
			{
				High_beam_TT = 0;
			}


		}
		INTC1.EIC8.BIT.EIRF8 =0;
	}
	else
	{
		High_beam_TT = 0;		//turn off High beam TT
	}
}


void init_port_p0_3(void) {                 /* initialize ports */


  R_GPIO_SetIntCallback(2,p03_isr );
  R_GPIO_EnableInt(2);

  //FCLA1CTL2 = 0x02; // falling edge
  FCLA0CTL3 = 0x03; // Rising edge		0x04
  INTC1EIP8 = 0;   // INTP8
 INTC1.EIC8.BIT.EIRF8 =0;
}
/*************************************************************/
static void locInit(void)
{
	uint8_t wakeup = R_FALSE;

#ifdef GEN_SOUND_HEADER
//	R_BSP_STDIO_Printf("Generating sound file...\n");
	GenSoundHeader();
#endif

	//R_BSP_STDIO_Printf("*****************************************************\n");
	//R_BSP_STDIO_Printf("Audio generation demo using board %s, version %s\n", R_BSP_GetBoardName(), LOC_SOFTWARE_VERSION);

	/* wakeup or reset */
	/*if((locWakeup == E_WAKEUP_INTERVAL) || (locWakeup == E_WAKEUP_ALARM) || (locWakeup == E_WAKEUP_EXPIN))
    {
        wakeup = R_TRUE;
    }*/

	/* Release IO hold incase it was set */
	/*R_STBC_SetIoHold(0);*/

	/* init standby */
	/*eeStandby_Init();*/

	/* from wakeup so limited initialisation */
	/*eeRtc_Init(wakeup);*/

	/* init the HMI */
	/*eeHmi_Init();*/

	/* init the state manager */
	/*eeStateManager_Init();*/

	/*#ifndef CONFIG_SSIF*/
	/* init the sound generator */
	/*SoundSG_Init(wakeup);
    SoundSG_Enable();
	 */
	/* init the sound PCM PWM */
	SoundPCM_Init(wakeup);
	SoundPCM_Enable();
	/*#else*/
	/* init the sound SSIF */
	/* SoundSSIF_Init(wakeup);
    SoundSSIF_Enable();*/
	/*#endif*/


#ifdef USE_RS232_TERMINAL
	R_BSP_STDIO_SetComPort(UART_0);
#endif
	R_BSP_STDIO_Init();
}
/***********************************************************/

void IGNOFF_commoninit(void)
{

	unsigned int i=0;

	protected_write(SYSPROTCMD0,SYSPROTS0,SYSIOHOLD,0x00);/*release iohold*/
	init_port_jp0_2();
	init_port_p0_2();
	init_port_jp0_1();
	if(turn_activate<2)
		turn_activate=1;
	R_TAUB_EnableInt(0, R_TAUB_INT_CH0);
	Read_UDS_EEPROM_Data();
	R_TAU0_Create();  /*call timer 100us*/
	R_TAU1_Create();
	R_TAU2_PWM_TFT();
	R_TAU2_PWM_GCLK();

#ifdef GRAYSCALE_TFT
	R_TAU2_PWM();
#endif
	R_TAU2_Create();

	R_GPIO_WritePin(17,8, 1);
	r_taub_OutputEnable	(2,R_TAUB_CHANNEL_11);

	R_GPIO_WritePin(16, 8, 1);
	R_GPIO_WritePin(3, 4, 1);



	R_GPIO_WritePin(17, 7, 0);
	R_GPIO_PinDirection(17,8,0);		//DLED_GCLK
	R_GPIO_WritePin(17, 7, 1);
	R_GPIO_WritePin(3, 8, 1);  /*5V_CONTROL_TT_DRIVER*/
	R_GPIO_WritePin(42, 13, 1);

	SendDCData(0xA5C, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1); /* reset command*/
	SendDCData(0xA5C, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1); /* reset command*/
	cascade_TT_send_data();

	diagTTflag=0;
	gcsk_flag=0;

	SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
	SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
	cascade_TT_send_data();

	TT_IGN_OFF=1;
#ifdef COLOR_TFT

	//----TFT RESET signal
	PORT_ISOPMC44_5 = 0;
	PORT_ISOPM44_5 = 0;
	PORT_ISOP44_5 = 1;

	/*change for PCB rev B*/
	PORT_ISOPMC44_3 = 0;
	PORT_ISOPM44_3 = 0;
	PORT_ISOP44_3 = 1;


	for(i = 0; i < 0xffff; i++);
	PORT_ISOP44_5 = 0;
	for(i = 0; i < 0xffff; i++);

	PORT_ISOP44_5 = 1;
	/*TFT init*/
#endif
#ifdef GRAYSCALE_TFT
	Lcbi_Non_Tft_Init();
#endif
	//SoundPCM_FIRST();	/*removed for every wakeup -> sleep transition Noise generated - 12/06/20 -> W501H234 */
	//R_GPIO_WritePin(17, 6, 1);//Audio amplifier
#ifdef COLOR_TFT
	locInit();/*sound*/
#endif
	Read_UDS_EEPROM_Data();

	Check_Valid_Feature_Code();
	Check_Vaild_Variant_Code();

	read_feature_code(&Feature_code_arr[0]);
	read_varient_code(&Varient_code_arr[0]);
	Bit_flag_Access();

	gui_taskInit();
	first_screen_timeout=1;
	handleDromeState();	/*function renaming*/
	handleDromeState();	/*function renaming*/
	//	handleDromeState();	/*function renaming*/
#if 0
#ifdef GRAYSCALE_TFT
	if(first_screen_timeout_start >=1)
	{
		draw_grayscale();
	}
#endif
#endif
	can_IGNOFF_init();

	/****************/
	move_out_of_standby=0;


	R_ADC_Create();
	Battery_Condition();				/*Check LVSD or HVSD flag*/
	//All_Telltale_OFF();
	IGNOFF_eeprom_read = 1;				/*Acknowledgement of FC VC read*/

}

/*******************************************************************************
  Section: Local Functions
 */
#ifdef ISM
static void loc_MenuHeader(void); 
static void loc_PrintMainMenu(void); 
static void loc_PrintSubMenu(int8_t ParentChoice, uint8_t SubId);  
static void loc_SelectSubMenu(int8_t ItemId, uint8_t SubId); 
static void loc_MenuHandler(void); 
void *main_atf(void *Arg);
#endif

#ifndef drome


/***********************************************************
  Section: Local Defines
 */

/* bitmap widths */
#define LOC_KING_W    (30)
#define LOC_GEAR_W    (104)
/* bitmap heights */
#define LOC_KING_H    (30)
#define LOC_GEAR_H    (104)
/* bitmap sizes */
#define LOC_KING_SIZE (LOC_KING_W * LOC_KING_H * 4)
#define LOC_GEAR_SIZE (LOC_GEAR_W * LOC_GEAR_H * 4)

#define LOC_SF_BMP_W   (64)
#define LOC_SF_BMP_H   (64)


#if (defined(BSP_INIT_OCTA_FLASH_R) || defined(BSP_INIT_OCTA_RAM_RW) || defined(BSP_INIT_OCTA_PIN))
/* OCTA RAM/Flash */
#define LOC_EX_MEM_TYPE   (1)
#elif (defined(BSP_INIT_HYPB_FLASH_R) || defined(BSP_INIT_HYPB_RAM_RW) || defined(BSP_INIT_HYPB_PIN))
/* Hyper RAM/Flash */
#define LOC_EX_MEM_TYPE   (2)
#else
/* Serial Flash */
#define LOC_EX_MEM_TYPE   (0)
#endif


/***********************************************************
  Section: Local Variables
 */

/***********************************************************

  Variable: loc_WmWindow

  Pointer to the Window handles
 */
r_wm_Window_t loc_WmWindow;
static r_wm_Window_t loc_SpriteWindow;


/***********************************************************

  Variable: loc_Drw2dDev

  Pointer to the Drw2D device handle
 */

static r_drw2d_Device_t loc_Drw2dDev;

/***********************************************************
  Variable: loc_Fb

  Framebuffer information for Drw2D
 */
static r_drw2d_Framebuffer_t loc_Fb;

/***********************************************************
  Variable: g_BlackKingBmp

  the Bitmap (external)
 */
extern const uint8_t g_BlackKingBmp[LOC_KING_SIZE];
extern const uint8_t g_GearBmp[LOC_GEAR_SIZE];
extern const unsigned char my_logo[];

/***********************************************************
  Variable: loc_lRAM_heap, loc_VRAM_heap

  control structures for CDI heaps
 */
extern r_cdi_Heap_t       loc_lRAM_heap;
extern r_cdi_Heap_t       loc_VRAM_heap;

/***********************************************************
  Variable: loc_DHDDev

  DHD Drawing handle
 */
static struct r_drw2d_DeviceDHD_s loc_DHDDev;

/***********************************************************
  Variable: loc_ExitFlag

  flag to test exit pathes
 */
static int8_t loc_ExitFlag = R_FALSE;

/***********************************************************
  Variable: loc_GPU_Memory_Addr

  dedicated memory area for GPU (DaveHD)
 */
extern uint32_t loc_GPU_Memory_Addr;


/***********************************************************
  Section: Local Functions
 */

/***********************************************************
 Function: loc_DRW2DErrorHandler

 Error handler used for Drw2d errors
 */
static r_drw2d_Boolean_t loc_DRW2DErrorHandler(r_drw2d_Error_t  Error, void *UserData)
{
	loc_Error(1);
	return R_TRUE;
}


/***********************************************************
  Section: Local Functions
 */

/*******************************************************************************
  Function: locBtnOkPress + locRotaryCenterPress

  Button and Rotary press callback functions
 */
static void locBtnOkPress(void) {
	loc_ExitFlag = R_TRUE;
}

static void locRotaryCenterPress(void) {
	loc_ExitFlag = R_TRUE;
}


/*******************************************************************************
  Function: loc_RenderClear

  Render function called each frame
 */
static void loc_RenderClear(int32_t Frame)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;

	/* Set green-ish clear color and fill framebuffer */
	ret |= R_DRW2D_CtxBgColor(loc_Drw2dDev, 0xff00ff00);
	ret |= R_DRW2D_FramebufferClear(loc_Drw2dDev);

	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
}

/*******************************************************************************
  Function: loc_RenderPrimitives

  Render function called each frame
 */
static void loc_RenderPrimitives(int32_t Frame)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Rect_t  rect;
	r_drw2d_Point_t triangle[3];

	/* triangle */
	ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
	ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
	ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, 0xffff0000u);
	triangle[0].X = R_DRW2D_2X(4);
	triangle[0].Y = R_DRW2D_2X(10);
	triangle[1].X = R_DRW2D_2X(66);
	triangle[1].Y = R_DRW2D_2X(124);
	triangle[2].X = R_DRW2D_2X(100);
	triangle[2].Y = R_DRW2D_2X(10);
	ret |= R_DRW2D_DrawTriangles(loc_Drw2dDev, triangle, 1, 0);

	/* rect */
	ret |= R_DRW2D_CtxFgColor(loc_Drw2dDev, 0xff0000ffu);
	rect.Pos.X       = R_DRW2D_2X((LOC_DISPLAY_WIDTH  - 100) / 2);
	rect.Pos.Y       = R_DRW2D_2X(200);
	rect.Size.Width  = R_DRW2D_2X(100);
	rect.Size.Height = R_DRW2D_2X(100);
	ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);

	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
}

/*******************************************************************************
  Function: loc_RenderTexture_intROM

  Render function called each frame
 */
static void loc_RenderTexture_intROM(int32_t Frame)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Rect_t  rect;
	r_drw2d_Texture_t tex_argb32;

	tex_argb32.Handle             = R_NULL;
	tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
	tex_argb32.Buffer.Pitch       = LOC_GEAR_W;
	tex_argb32.Buffer.Size.Width  = LOC_GEAR_W;
	tex_argb32.Buffer.Size.Height = LOC_GEAR_H;
	tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
	tex_argb32.Buffer.Data        = (void*)g_GearBmp;

	rect.Pos.X       = R_DRW2D_2X(130);
	rect.Pos.Y       = R_DRW2D_2X(24);
	rect.Size.Width  = R_DRW2D_2X(LOC_GEAR_W);
	rect.Size.Height = R_DRW2D_2X(LOC_GEAR_H);

	/* Set texture */
	ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
	ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
	ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);

	/* rotate and translate */
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, rect.Pos.X+rect.Size.Width/2, rect.Pos.Y+rect.Size.Height/2);
	ret |= R_DRW2D_CtxTextureRotate(loc_Drw2dDev, R_DRW2D_2X(Frame * (360/LOC_DISPLAY_WIDTH*3)));
	ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, -(rect.Size.Width/2), -(rect.Size.Height/2));

	ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_CtxTranslate(loc_Drw2dDev, rect.Pos.X+rect.Size.Width/2, rect.Pos.Y+rect.Size.Height/2, 0);
	ret |= R_DRW2D_CtxRotate(loc_Drw2dDev, R_DRW2D_2X(Frame * (360/LOC_DISPLAY_WIDTH*3)));
	ret |= R_DRW2D_CtxTranslate(loc_Drw2dDev, -(rect.Pos.X+rect.Size.Width/2), -(rect.Pos.Y+rect.Size.Height/2), 0);

	/* Draw texture into rectangle */
	ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);

	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
}

static void my_render_logo(void)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Rect_t  rect;
	r_drw2d_Texture_t tex_argb32;

	tex_argb32.Handle             = R_NULL;
	tex_argb32.Flags              = (r_drw2d_TextureFlags_t)0;
	//tex_argb32.Flags              = R_DRW2D_TEX_VT | R_DRW2D_TEX_BILINEAR;
	tex_argb32.Buffer.Pitch       = 240;
	tex_argb32.Buffer.Size.Width  = 240;
	tex_argb32.Buffer.Size.Height = 320;
	tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
	tex_argb32.Buffer.Data        = (void*)my_logo;

	rect.Pos.X       = R_DRW2D_2X(0);
	rect.Pos.Y       = R_DRW2D_2X(0);
	rect.Size.Width  = R_DRW2D_2X(240);
	rect.Size.Height = R_DRW2D_2X(320);

	/* Set texture */
	ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
	ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
	ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);


	/* Draw texture into rectangle */
	ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);

	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
}


/*******************************************************************************
  Function: loc_RenderTexture_SFlash

  Render function called each frame
 */
static void loc_RenderTexture_SFlash(int32_t Frame, void* TextureAddr)
{
	r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
	r_drw2d_Rect_t  rect;
	r_drw2d_Texture_t tex_argb32;
	uint32_t xPos, yPos;

	tex_argb32.Handle             = R_NULL;
	tex_argb32.Flags              = (r_drw2d_TextureFlags_t)0;
	tex_argb32.Buffer.Pitch       = LOC_SF_BMP_W;
	tex_argb32.Buffer.Size.Width  = LOC_SF_BMP_W;
	tex_argb32.Buffer.Size.Height = LOC_SF_BMP_H;
	tex_argb32.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
	tex_argb32.Buffer.Data        = (void*)TextureAddr;

	/* Set texture */
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_CtxIdentity(loc_Drw2dDev);
	ret |= R_DRW2D_CtxTextureSet(loc_Drw2dDev, 0, &tex_argb32);
	ret |= R_DRW2D_CtxFillMode(loc_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
	ret |= R_DRW2D_CtxBlendMode(loc_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);
	xPos = R_DRW2D_2X((LOC_DISPLAY_WIDTH  - 100) / 2 + Frame);
	yPos = R_DRW2D_2X(LOC_DISPLAY_HEIGHT - 100);
	ret |= R_DRW2D_CtxTextureTranslate(loc_Drw2dDev, xPos, yPos);

	/* Draw texture into rectangle */
	rect.Pos.X       = R_DRW2D_2X(Frame);
	rect.Pos.Y       = R_DRW2D_2X(LOC_DISPLAY_HEIGHT - 100);
	rect.Size.Width  = R_DRW2D_2X(LOC_SF_BMP_W);
	rect.Size.Height = R_DRW2D_2X(LOC_SF_BMP_H);

	ret |= R_DRW2D_DrawRect(loc_Drw2dDev, &rect);
	ret |= R_DRW2D_CtxTextureIdentity(loc_Drw2dDev);

	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
}

/*******************************************************************************
  Function: loc_App

  The render loop
 */
static void loc_App(void)
{
	uint32_t frame;
	r_wm_Sprite_t sprite;
	int32_t ret = R_DRW2D_ERR_OK;
	r_wm_Error_t wmErr;

	void*   textureAddr = R_NULL;
#if (LOC_VOWE_ENABLE == 1) && (LOC_DL_CHANGE != 0)
	int32_t dl_cnt = 1;
#endif /* LOC_DL_CHANGE */
#if (LOC_EX_MEM_TYPE == 1)
	//textureAddr = loc_StartOCTA();
#elif (LOC_EX_MEM_TYPE == 2)
	//textureAddr = loc_StartHYPB();
#else
	//textureAddr = loc_StartSFMA();
#endif
	/* Init framebuffer struct */
	loc_Fb.Flags  = (r_drw2d_FramebufferFlags_t)0;
	loc_Fb.Handle = 0; /* not used currently */
	loc_Fb.Buffer.Pitch       = LOC_DISPLAY_STRIDE;
	loc_Fb.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
	loc_Fb.Buffer.Size.Width  = LOC_DISPLAY_WIDTH;
	loc_Fb.Buffer.Size.Height = LOC_DISPLAY_HEIGHT;
	loc_Fb.Buffer.Data        = 0;

	my_tell_tale_init();

	loc_InitWM(&loc_lRAM_heap, &loc_VRAM_heap);
	wmErr = R_WM_FrameEndMark(LOC_WM_UNIT, 0);
	if(R_WM_ERR_OK != wmErr)
	{
		loc_Error(1);
	}
	wmErr = R_WM_FrameWait(LOC_WM_UNIT, 0);
	if(R_WM_ERR_OK != wmErr)
	{
		loc_Error(1);
	}

	R_BSP_STDIO_Printf("Now using VOWE, JCUA and WM\n");
#if LOC_VOWE_ENABLE == 1
	//loc_VoweStart();
#endif

	loc_CreateWindow(R_WM_WINMODE_FB,
			0,
			0,
			0,
			LOC_DISPLAY_WIDTH,
			LOC_DISPLAY_HEIGHT,
			LOC_DISPLAY_STRIDE,
			R_WM_COLORFMT_ARGB8888,
			&loc_WmWindow);
	// loc_CreateWindow(R_WM_WINMODE_SPRITES,
	// 0,
	// 0,
	// 1,
	// 240,//LOC_KING_W,
	// 320,//LOC_KING_H,
	// 2048, /* HW restriction */
	// R_WM_COLORFMT_ARGB8888,
	// &loc_SpriteWindow);

	//loc_JcuaRun();
#if 0
	/* assign texture and show sprite */
	sprite.Data   = (void*)g_BlackKingBmp;
	sprite.PosX   = 0;
	sprite.PosY   = 0;
	sprite.PosZ   = 0;
	sprite.Width  = LOC_KING_W;
	sprite.Height = LOC_KING_H;
	sprite.Window = &loc_SpriteWindow;
	wmErr = R_WM_SpriteCreate(LOC_WM_UNIT, &sprite);
	if(R_WM_ERR_OK != wmErr)
	{
		loc_Error(1);
	}
	wmErr = R_WM_SpriteEnable(LOC_WM_UNIT, &sprite);
	if(R_WM_ERR_OK != wmErr)
	{
		loc_Error(1);
	}
#endif

	/* start up gfx engine */
	ret |= R_DRW2D_Init();
	ret |= R_DRW2D_GlobalErrCallbackSet(&loc_DRW2DErrorHandler, R_NULL);
	ret |= R_DRW2D_Open(LOC_DRW2D_UNIT, R_DRW2D_UNIT_DHD0, &loc_DHDDev, &loc_Drw2dDev);

	loc_ExitFlag = R_FALSE;

	for (frame = 0; loc_ExitFlag == R_FALSE; frame++)
	{
		loc_Fb.Buffer.Data = R_WM_WindowNewDrawBufGet(LOC_WM_UNIT, &loc_WmWindow);
		ret |= R_DRW2D_FramebufferSet(loc_Drw2dDev, &loc_Fb);

		/* render onto this backbuffer and wait for GPU to finish*/
		loc_RenderClear(frame);
		//loc_RenderPrimitives(frame);
		//loc_RenderTexture_intROM(frame);
		my_render_logo();
		// loc_RenderTexture_SFlash(frame, textureAddr);

		R_DRW2D_GpuFinish(loc_Drw2dDev, R_DRW2D_FINISH_WAIT);

		/* let WM care about the handling of the buffer swap */
		wmErr = R_WM_WindowSwap(LOC_WM_UNIT, &loc_WmWindow);
		if(R_WM_ERR_OK != wmErr)
		{
			loc_Error(1);
		}

		/* animate the sprite window, too */
		// wmErr = R_WM_WindowMove(LOC_WM_UNIT, &loc_SpriteWindow, frame, loc_SpriteWindow.PosY, loc_SpriteWindow.PosZ);
		// if(R_WM_ERR_OK != wmErr)
		// {
		// loc_Error(1);
		// }

		if (frame == LOC_DISPLAY_WIDTH)
		{
#if LOC_VOWE_ENABLE == 1
#if LOC_DL_CHANGE != 0
			/* change Display List */
			//loc_Vowe_DLChange(dl_cnt);
			dl_cnt++;
#endif /* LOC_DL_CHANGE */
#endif

			frame = 0;
		}

		wmErr = R_WM_FrameEndMark(LOC_WM_UNIT, 0);
		if(R_WM_ERR_OK != wmErr)
		{
			loc_Error(1);
		}
		wmErr = R_WM_FrameWait(LOC_WM_UNIT, 0);
		//        while(1);
		if(R_WM_ERR_OK != wmErr)
		{
			loc_Error(1);
		}
		if (R_DRW2D_ERR_OK != ret)
		{
			loc_Error(ret);
		}
		loc_ExitFlag = R_TRUE;

	}
#if 0
	ret |= R_WM_WindowDisable(LOC_WM_UNIT, &loc_SpriteWindow);
	ret |= R_WM_WindowDisable(LOC_WM_UNIT, &loc_WmWindow);
	ret |= R_WM_FrameEndMark(LOC_WM_UNIT, 0);
	ret |= R_WM_FrameWait(LOC_WM_UNIT, 0);

	R_BSP_STDIO_Printf("Closing  DRW2D and VOWE\n");
	ret |= R_DRW2D_Close(loc_Drw2dDev);
	ret |= R_DRW2D_Exit();
#if LOC_VOWE_ENABLE == 1
	//loc_VoweStop();
#endif

	R_BSP_STDIO_Printf("Closing  WM and SPEA\n");
	ret |= R_WM_SpriteDisable(LOC_WM_UNIT, &sprite);
	ret |= R_WM_SpriteDelete(LOC_WM_UNIT, &sprite);
	ret |= R_WM_WindowDelete(LOC_WM_UNIT, &loc_SpriteWindow);
	ret |= R_WM_WindowDelete(LOC_WM_UNIT, &loc_WmWindow);
	ret |= R_WM_FrameEndMark(LOC_WM_UNIT, 0);
	ret |= R_WM_FrameWait(LOC_WM_UNIT, 0);
	ret |= R_WM_DevDeinit(LOC_WM_UNIT);


#if (LOC_EX_MEM_TYPE == 1)
	loc_StopOCTA();
#elif (LOC_EX_MEM_TYPE == 2)
	loc_StopHYPB();
#else
	//loc_StopSFMA();
#endif

	if (R_DRW2D_ERR_OK != ret)
	{
		loc_Error(ret);
	}
#endif
}

/*******************************************************************************
  Function: loc_Error

  This function is called when an error occurs.
  The application will hang.
 */

void loc_Error(int32_t Err)
{
	while (Err)
	{
	}
}
#endif /*Non drome functions*/
#ifdef ISM
/*******************************************************************************
  Function: loc_MenuHeader

  Print32_t the Main header

  Parameters:
  void

  Returns:
  void
 */
static void loc_MenuHeader(void) 
{
	const char  *device = R_DEV_GetDevName();

	R_BSP_STDIO_Printf("\n\n");
	R_BSP_STDIO_Printf("-----------------------------------------------------------------\n");
	R_BSP_STDIO_Printf("-----------------------------------------------------------------\n");
	R_BSP_STDIO_Printf("------------------     R E N E S A S        ---------------------\n");
	R_BSP_STDIO_Printf("------------------ Demo & Test Application  ---------------------\n");
	R_BSP_STDIO_Printf("------------------ Device:%16s  ---------------------\n", device);
	R_BSP_STDIO_Printf("-----------------------------------------------------------------\n");
	R_BSP_STDIO_Printf("-----------------------------------------------------------------\n\n");
}


/*******************************************************************************
  Function: loc_PrintMainMenu

  Dispay the main menu.  
  Only entries with menu_id 0 ("root" level) will be printed 

  Parameters:
  void  -

  Returns:
  void  -
 */

static void loc_PrintMainMenu(void) 
{
	uint32_t n;

	R_BSP_STDIO_Printf("\n\n--------------------------- Main Menu ---------------------------\n");

	for (n = 0u; (loc_Menu[n].item_desc != 0u); n++)
	{
		if (loc_Menu[n].menu_id == 0u)
		{
			R_BSP_STDIO_Printf("----- \"%c\": %s \n", loc_Menu[n].item_id, loc_Menu[n].item_desc);
		}
	}

	R_BSP_STDIO_Printf("----- \"x\": Exit Application\n");
	R_BSP_STDIO_Printf("-----------------------------------------------------------------\n");
	R_BSP_STDIO_Printf("\n");

}


/*******************************************************************************
  Function: loc_PrintSubMenu

  Print32_t the submenu for the given ID

  Parameters:
  ParentChoice  -  the selection that led to the current meanu,
                    it's required to retrieve and display the correct title of the parent
  SubId         -  id of the chosen sub menu 

  Returns:
  void  -
 */

static void loc_PrintSubMenu(int8_t ParentChoice, uint8_t SubId) 
{
	uint32_t n;
	uint32_t choice = 0u;
	/* First, find sub menu title */
	for (n = 0u; ((n < 255u) && (loc_Menu[n].item_desc != 0u)); n++)
	{
		if ((loc_Menu[n].child_id == SubId) && (loc_Menu[n].item_id == ParentChoice))
		{
			R_BSP_STDIO_Printf("\n+------------------- %s -----------------+\n", loc_Menu[n].item_desc);
			choice = n;
			n = 255u;
		}
	}
	/* ... then print32_t menu */
	for (n = 0u; (loc_Menu[n].item_desc != 0u); n++)
	{
		if (loc_Menu[n].menu_id == SubId)
		{
			R_BSP_STDIO_Printf("+++++ \"%c\": %s \n", loc_Menu[n].item_id, loc_Menu[n].item_desc);
		}
	}

	R_BSP_STDIO_Printf("+++++ \"x\": go back\n");
	R_BSP_STDIO_Printf("+------------------- %s -----------------+\n", loc_Menu[choice].item_desc);
	R_BSP_STDIO_Printf("\n");

}

/*******************************************************************************
  Function: loc_SelectSubMenu

  Select the item from submenu based on user input (recursive function) 

  Parameters:
  ItemId     -  the selection tha led to the current meanu,
                 it's required print32_t the sub menu
  SubId     -  id of the selected sub menu 

  Returns:
  void  -
 */

static void loc_SelectSubMenu(int8_t ItemId, uint8_t SubId) 
{
	int8_t   in_char;
	uint32_t n;

	do
	{
		loc_PrintSubMenu(ItemId, SubId);
		R_BSP_STDIO_Printf("Your Command:");
		R_BSP_STDIO_ReceiveChar(&in_char);

		for (n = 0u; (loc_Menu[n].item_desc != 0u); n++)
		{
			if ((loc_Menu[n].menu_id == SubId) && (in_char == loc_Menu[n].item_id))
			{
				R_BSP_STDIO_Printf("\n");
				/* First, execute any specified function */
				if  (loc_Menu[n].item_function != 0u)
				{
					loc_ActiveEntry = n;
					loc_ActiveMenu  = LOC_USER_MENU;
					(loc_Menu[n].item_function)();
				}
				/* ... second, check for another  */
				if (loc_Menu[n].child_id != 0u)
				{
					loc_SelectSubMenu(in_char, loc_Menu[n].child_id);

				}
				break;
			}
		}

	} while (in_char != 'x');

}

/*******************************************************************************
  Function: loc_MenuHandler

  The main menu handler. 

  Search the item (function) from main menu based on user input and execute it

  Parameters:
  void  -

  Returns:
  void  -
 */

static void loc_MenuHandler(void) 
{
	int8_t     in_char;
	uint32_t n;

	/* Print32_t header */
	loc_MenuHeader();

	if (0 != loc_MenuPreamble)
	{
		R_BSP_STDIO_Printf(loc_MenuPreamble);
	}

	do
	{
		loc_PrintMainMenu();

		R_BSP_STDIO_Printf("Your Command:");
		R_BSP_STDIO_ReceiveChar(&in_char);

		if (in_char == 'x')
		{
			break;
		}

		for (n = 0u; (loc_Menu[n].item_desc != 0u); n++)
		{
			if ((loc_Menu[n].menu_id == 0u) && (in_char == loc_Menu[n].item_id))
			{
				R_BSP_STDIO_Printf("\n");
				/* First, execute any specified function */
				if  (loc_Menu[n].item_function != 0u)
				{
					loc_ActiveEntry = n;
					loc_ActiveMenu  = LOC_USER_MENU;
					(loc_Menu[n].item_function)();
				}
				/* ... second, check for sub menu */
				if (loc_Menu[n].child_id != 0u)
				{
					loc_SelectSubMenu(in_char, loc_Menu[n].child_id);
				}
				break;
			}
		}
	}while (in_char != 'x');
}


/*******************************************************************************
  Section: Global Functions
 */

/*******************************************************************************
  Function: R_ATF_WaitForChar
 */

void R_ATF_WaitForChar(int8_t character)
{
	int8_t in_char;
	do
	{
		R_BSP_STDIO_ReceiveChar(&in_char);
	} while(character != in_char);
}


/*******************************************************************************
  Function: R_ATF_Confirm
 */

int8_t R_ATF_Confirm(char * Message, char * PositiveChars, char * NegativeChars)
{
	int8_t in_char;
	int8_t ret_val = -1;

	char *loc_positiveChars = "yY";
	char *loc_negativeChars = "nN";

	if (0 != PositiveChars)
	{
		loc_positiveChars = PositiveChars;
	}
	if (0 != NegativeChars)
	{
		loc_negativeChars = NegativeChars;
	}


	R_BSP_STDIO_Printf("%s\n", Message);
	do
	{
		if ( (0 == PositiveChars) && (0 == NegativeChars) )
		{
			R_BSP_STDIO_Printf("Input answer to proceed (y/n)\n");
		}
		else
		{
			R_BSP_STDIO_Printf("Input answer to proceed (%s/%s)\n", loc_positiveChars, loc_negativeChars);
		}
		R_BSP_STDIO_ReceiveChar(&in_char);

		if (0 != strchr(loc_positiveChars, in_char))
		{
			ret_val = 1;
			R_BSP_STDIO_Printf("Positive.\n\n");
		}
		else if (0 != strchr(loc_negativeChars, in_char))
		{
			ret_val = 0;
			R_BSP_STDIO_Printf("Negative.\n\n");
		}
		else
		{
			/* Invalid char: Don't leave this function, wait for next input */
			ret_val = -1;
		}
	} while(-1 == ret_val);

	return ret_val;
}

/*******************************************************************************
  Function: R_ATF_Prologue
 */

void R_ATF_Prologue(char * Description)
{
	R_BSP_STDIO_Printf("\n*****************************************************************\n");
	R_BSP_STDIO_Printf("*\n");
	R_BSP_STDIO_Printf("* ");

	/* Use Custom or standard description?
       0 means: use default  */
	if (0u == Description)
	{
		switch (loc_ActiveMenu)
		{
		case LOC_USER_MENU:
			R_BSP_STDIO_Printf("%s", loc_Menu[loc_ActiveEntry].item_desc);
			break;
		case LOC_AUTO_MENU:
			R_BSP_STDIO_Printf("%s", loc_Menu[loc_ActiveEntry].item_desc);
			break;
		default:
			break;
		}
	}
	else
	{
		R_BSP_STDIO_Printf((char *)Description);
	}

	R_BSP_STDIO_Printf("\n");
	R_BSP_STDIO_Printf("*\n");
	R_BSP_STDIO_Printf("*****************************************************************\n");
	R_BSP_STDIO_Printf("\n\n");
}


/*******************************************************************************
  Function: R_ATF_Epilogue
 */


void R_ATF_Epilogue(void)
{
	if (0u == loc_SkipEpilogue)
	{
		R_BSP_STDIO_Printf("\nPress \"x\" to proceed \n");
		R_ATF_WaitForChar('x');
	}
}


/*******************************************************************************
  Function: R_ATF_Control
 */

void R_ATF_Control(char Character, char * Description)
{
	R_BSP_STDIO_Printf("Press \"%c\" %s\n\n", Character, Description);
	R_ATF_WaitForChar(Character);
}

/*******************************************************************************
  Function: R_ATF_MenuSelect
 */

uint8_t R_ATF_MenuSelect(const r_atf_MenuItem_t * MenuItems)
{
	uint32_t n;
	int8_t   in_char;

	/* print32_t out the menu items and generate ID-s */
	in_char = (int8_t) '1';
	for (n = 0u; (MenuItems[n].item_desc != 0u); n++)
	{
		R_BSP_STDIO_Printf("Insert \"%c\" for %s \n", in_char, MenuItems[n].item_desc);
		if (in_char == (int8_t)'9')
		{
			in_char = (int8_t)'a';
		}
		else
		{
			in_char++;
		}
	}
	R_BSP_STDIO_Printf("------------------------------------\n");

	/* input choice and convert to order number starting with 1 */
	R_BSP_STDIO_ReceiveChar(&in_char);
	while ((in_char == (int8_t)'\r') || (in_char == (int8_t)'\n'))
	{
		R_BSP_STDIO_ReceiveChar(&in_char);
	}
	if (in_char >= (int8_t)'a')
	{
		return ((10u + (uint8_t)(in_char)) - (uint8_t)'a');
	}
	else if (in_char >= (int8_t)'1')
	{
		return ((uint8_t)(in_char) - (uint8_t)'0');
	}
	else
	{
		return 0u;
	}
}    

/*******************************************************************************
  Function: R_ATF_ValueSelect
 */

uint32_t R_ATF_ValueSelect(const r_atf_ValueItem_t * ValueItems)
{
	uint8_t   n;
	int8_t    in_char;

	/* print32_t out the menu items and generate ID-s */
	in_char = (int8_t)'1';
	for (n = 0u; (ValueItems[n].item_desc != 0u); n++)
	{
		R_BSP_STDIO_Printf("Insert \"%c\" for %s \n", in_char, ValueItems[n].item_desc);
		if (in_char == (int8_t)'9')
		{
			in_char = (int8_t)'a';
		}
		else {
			in_char++;
		}
	}

	R_BSP_STDIO_Printf("------------------------------------\n");
	/* input choice and convert to order number starting with 1 */
	R_BSP_STDIO_ReceiveChar(&in_char);
	while ((in_char == (int8_t)'\r') || (in_char == (int8_t)'\n'))
	{
		R_BSP_STDIO_ReceiveChar(&in_char);
	}
	R_BSP_STDIO_Printf("\n");

	if (in_char >= (int8_t)'a')
	{
		n = (10u + (uint8_t)(in_char)) - (uint8_t)'a';
	}
	else if (in_char >= (int8_t)'1')
	{
		n = (uint8_t)(in_char) - (uint8_t)'0';
	}
	else
	{
		n = 0u;
	}

	if ((n > 0u)  )
	{
		return ((uint32_t)(ValueItems[n-1u].item_value));
	}
	else
	{
		return 0u;
	}
}


/*******************************************************************************
  Function: R_ATF_SetMenu

  see r_atf_api.h  
 */
void R_ATF_SetMenu(const r_atf_Menu_t * TestMenu) 
{
	loc_Menu        = TestMenu;
}

/*******************************************************************************
  Function: R_ATF_SetMenuPreamble

  see r_atf_api.h  
 */
void R_ATF_SetMenuPreamble(char * MenuPreamble) 
{
	loc_MenuPreamble        = MenuPreamble;
}

/*******************************************************************************
  Function: R_ATF_SetAutomatic

  see r_atf_api.h
 */
void R_ATF_SetAutomatic(const r_atf_Automatic_t * Auto, uint8_t AutoStart) 
{
	loc_Auto        = Auto;
	loc_AutoStart   = AutoStart;
}


/*******************************************************************************
  Function: R_ATF_SetAutoConfig

  see r_atf_api.h
 */
void R_ATF_SetAutoConfig(void (*AutoConf) (void)) 
{
	loc_UserAutoConfig     = AutoConf;
}

/*******************************************************************************
  Function: R_ATF_Automatic
 */

void R_ATF_Automatic(void) 
{
	uint16_t i = 0u;
	uint16_t total_OK  = 0u;
	uint16_t total_err = 0u;
	if (loc_Auto == 0u)
	{
		R_BSP_STDIO_Printf("That's not going to work!\n");
		for ( ; ; ){}
	}
	/* Disable (temporaliy) any epilogue */
	loc_SkipEpilogue = 1u;
	/* prints prologue in formatted output*/
	R_ATF_Prologue(" ***** Starting Automatic Execution ***** ");

	if (0u != loc_UserAutoConfig)
	{
		loc_UserAutoConfig();
	}
	/* process the table until an "empty" function is found  */
	while (loc_Auto[i].Desc != 0u)
	{
		R_BSP_STDIO_Printf("**********  %s: \n", loc_Auto[i].Desc);
		if(loc_Auto[i].Func == 0u)
		{
			/* An "empty" function having a description -> OK */
			R_BSP_STDIO_Printf("OK!\n");
			/* continue */
			i++;
		}
		else
		{
			loc_ActiveEntry = i;
			loc_ActiveMenu  = LOC_AUTO_MENU;
			if (loc_Auto[i].Func() == R_ERR_OK)
			{
				/* Successfull execution  */
				R_BSP_STDIO_Printf("OK!\n");
				total_OK++;
				/* continue */
				i++;
			}
			else
			{
				total_err++;
				R_BSP_STDIO_Printf("FAIL! \n");
				if (R_ATF_NO_RETRY == loc_Auto[i].ContAfterError)
				{
					R_BSP_STDIO_Printf("... Aborting here! \n");
					R_BSP_STDIO_Printf("\n*** Summary *** \n");
					R_BSP_STDIO_Printf("Total successes: %d\n", total_OK);
					R_BSP_STDIO_Printf("Total failures : %d\n", total_err);
					R_BSP_STDIO_Printf("Achieved %d%% success\n", (100u * total_OK)/(total_err + total_OK));
					return;

				}
				else
				{
					R_BSP_STDIO_Printf("... Another try ... \n");
					/* new starting point32_t */
					i =  loc_Auto[i].ContAfterError;
				}
			}
		}
	}
	if (0u != total_err)
	{
		R_BSP_STDIO_Printf("Testing done with Errors!\n");
		R_BSP_STDIO_Printf("\n*** Summary *** \n");
		R_BSP_STDIO_Printf("Total successes: %d\n", total_OK);
		R_BSP_STDIO_Printf("Total failures : %d\n", total_err);
		R_BSP_STDIO_Printf("Achieved %d%% success\n", (100u * total_OK)/(total_err + total_OK));
	}
	else
	{
		R_BSP_STDIO_Printf("\nCongratulations, All tests passed!\n");
	}
	/* Enable Epilogue again */
	loc_SkipEpilogue = 0u;
}    


void *main_atf(void *Arg)
{
	/*  Application dependent Init */
	R_ATF_TestInit();

	/* We've got a menu,  */
	if (loc_Menu != 0u)
	{
		loc_MenuHandler();
		if ((0u != loc_Auto) && (0u != loc_AutoStart))
		{
			R_ATF_Automatic();
		}
	}
	/* ... or some automatic execution,  */
	else if (loc_Auto != 0u)
	{
		R_ATF_Automatic();
	}
	/* ... or else, ...*/
	else
	{
		/* No Menu defined, we stay right here */
		R_BSP_STDIO_Printf("WTF?!\nNuttin' to do?");
		for ( ; ; ){}
	}
	/* All done */
	R_BSP_STDIO_Printf("All Done! Exiting now...\n");

	return 0;
}
#endif

#if 0
static void locInit(void)
{
	uint8_t wakeup = R_FALSE;

#ifdef GEN_SOUND_HEADER
	R_BSP_STDIO_Printf("Generating sound file...\n");
	GenSoundHeader();
#endif

	//R_BSP_STDIO_Printf("*****************************************************\n");
	//R_BSP_STDIO_Printf("Audio generation demo using board %s, version %s\n", R_BSP_GetBoardName(), LOC_SOFTWARE_VERSION);

	/* wakeup or reset */
	/*if((locWakeup == E_WAKEUP_INTERVAL) || (locWakeup == E_WAKEUP_ALARM) || (locWakeup == E_WAKEUP_EXPIN))
    {
        wakeup = R_TRUE;
    }*/

	/* Release IO hold incase it was set */
	/*R_STBC_SetIoHold(0);*/

	/* init standby */
	/*eeStandby_Init();*/

	/* from wakeup so limited initialisation */
	/*eeRtc_Init(wakeup);*/

	/* init the HMI */
	/*eeHmi_Init();*/

	/* init the state manager */
	/*eeStateManager_Init();*/

	/*#ifndef CONFIG_SSIF*/
	/* init the sound generator */
	/*SoundSG_Init(wakeup);
    SoundSG_Enable();
	 */
	/* init the sound PCM PWM */
	SoundPCM_Init(wakeup);
	SoundPCM_Enable();
	/*#else*/
	/* init the sound SSIF */
	/* SoundSSIF_Init(wakeup);
    SoundSSIF_Enable();*/
	/*#endif*/


#ifdef USE_RS232_TERMINAL
	R_BSP_STDIO_SetComPort(UART_0);
#endif
	R_BSP_STDIO_Init();
}
#endif

/***********************************************************
  Function: eeTask_Processing
 */
void eeTask_Processing(void)
{
	/* process the HMI (user controls i.e. buttons) */
	/*eeHmi_Processing();*/

	/* process the RTC */
	/* eeRtc_Processing();*/

	/* process the sound, sound generator, PCM and SSIF */
	/*SoundSG_Processing();*/
	//SoundPCM_Processing();
	/*SoundSSIF_Processing();*/
}
#if 0
void MY_CS(U8 cmd)
{

}

void MY_SDA(U8 cmd)
{
	R_GPIO_WritePin(1, 1, cmd);		/*DLED_SDI=1;//GSSIN - High*/
}

void MY_SCK(U8 cmd)
{
	R_GPIO_WritePin(1, 2, cmd);		/*DLED_SCL=0;//GSSCK-Low*/
}

void Delayus(U8 d)
{

}

void MY_3SPI(U16 data)
{
	unsigned short MB=data;
	int i=0;
	/*SET_MY_SCK_OUTPUT();//SCL
		SET_MY_SDA_OUTPUT();//SDA
		SET_MY_CS_OUTPUT(); //CS*/
	//SET_MY_SD0_INTPUT();//SDA
	MY_CS(0);
	Delayus(5);
	MY_SDA(1);
	MY_SCK(1);
	Delayus(10);
	for(i=0;i<9;i++)
	{
		MY_SCK(0);
		if(MB&0x100)
		{
			MY_SDA(1);
		}
		else{
			MY_SDA(0);
		}
		Delayus(10);
		MY_SCK(1);
		Delayus(10);
		MB<<=1;
	}
	MY_CS(1);
	MY_SCK(1);
	Delayus(10);
}

void lcdICINIT(void)
{
#if 1
	MY_3SPI(0x011);//Display off
	MY_3SPI(0x1A5);
	Delayus(20);
	MY_3SPI(0x061);//0x61h
	MY_3SPI(0x188);//1
	MY_3SPI(0x100);//2
	MY_3SPI(0x101);//3
	MY_3SPI(0x110);//4
	MY_3SPI(0x100);//5
	MY_3SPI(0x100);//6
	MY_3SPI(0x100);//7
	MY_3SPI(0x108);//8

	MY_3SPI(0x100);//9
	Delayus(5);
	MY_3SPI(0x071);//0x71h lower temperature
	MY_3SPI(0x137);//1 VCOM
	MY_3SPI(0x109);//2 VONREG=3.5V,VGH 16V
	MY_3SPI(0x108);//3 VGL:08 -9V
	MY_3SPI(0x11F);//4 VDDHSP 5.7
	MY_3SPI(0x11F);//5 VDDHSN -5.7
	MY_3SPI(0x11F);//6 VDDRHP 5.5
	MY_3SPI(0x11F);//7 VDDRMP 2.75
	MY_3SPI(0x11F);//8 VDDRMN -2.75
	Delayus(5);
	MY_3SPI(0x072); //0x72h room temperature
	MY_3SPI(0x137);//1 VCOM
	MY_3SPI(0x109);//2 VONREG=3.5V,VGH 16V
	MY_3SPI(0x108);//3 VGL:08 -9V
	MY_3SPI(0x11F);//4 VDDHSP 5.7
	MY_3SPI(0x11F);//5 VDDHSN -5.7
	MY_3SPI(0x11F);//6 VDDRHP 5.5
	MY_3SPI(0x11F);//7 VDDRMP 2.75
	MY_3SPI(0x11F);//8 VDDRMN -2.75
	Delayus( 5 );

	MY_3SPI(0x073); // 0x73h higher temperature
	MY_3SPI(0x137);//1 VCOM
	MY_3SPI(0x109);//2 VONREG=3.5V,VGH 16V
	MY_3SPI(0x108);//3 VGL:08 -9V
	MY_3SPI(0x11F);//4 VDDHSP 5.7
	MY_3SPI(0x11F);//5 VDDHSN -5.7
	MY_3SPI(0x11F);//6 VDDRHP 5.5
	MY_3SPI(0x11F);//7 VDDRMP 2.75
	MY_3SPI(0x11F);//8 VDDRMN -2.75
	Delayus( 5 );
#if 1 //Gamma
	MY_3SPI(0x081); //RED
	MY_3SPI(0x100); //
	MY_3SPI(0x134); //
	MY_3SPI(0x133); //
	MY_3SPI(0x144) ;//
	MY_3SPI(0x133); //
	MY_3SPI(0x133); //
	MY_3SPI(0x100); //
	MY_3SPI(0x166); //
	MY_3SPI(0x145); //
	MY_3SPI(0x122); //
	MY_3SPI(0x111); //
	MY_3SPI(0x122); //
	MY_3SPI(0x100); //
	Delayus( 5 );

	MY_3SPI(0x082); //GREEN
	MY_3SPI(0x100); //
	MY_3SPI(0x144); //
	MY_3SPI(0x133); //
	MY_3SPI(0x133) ;//
	MY_3SPI(0x133); //
	MY_3SPI(0x134); //
	MY_3SPI(0x100); //
	MY_3SPI(0x177); //

	MY_3SPI(0x144); //
	MY_3SPI(0x122); //
	MY_3SPI(0x111); //
	MY_3SPI(0x122); //
	MY_3SPI(0x100); //
	Delayus( 5 );
	MY_3SPI(0x083); //BLUE
	MY_3SPI(0x100); //
	MY_3SPI(0x134); //
	MY_3SPI(0x133); //
	MY_3SPI(0x133) ;//
	MY_3SPI(0x133); //
	MY_3SPI(0x133); //
	MY_3SPI(0x100); //
	MY_3SPI(0x166); //
	MY_3SPI(0x144); //
	MY_3SPI(0x123); //
	MY_3SPI(0x111); //
	MY_3SPI(0x121); //
	MY_3SPI(0x100); //
	Delayus(5);

#endif
	MY_3SPI(0x091);
	MY_3SPI(0x100);//101
	Delayus(5);
	MY_3SPI(0x092);
	MY_3SPI(0x100);//
	Delayus(5);
	MY_3SPI(0x093);
	MY_3SPI(0x170);//RGB888:170
	Delayus(5);
	MY_3SPI(0x094);//RGB Normally Black
	MY_3SPI(0x100);
	MY_3SPI(0x178);
	MY_3SPI(0x101);//
	MY_3SPI(0x1FF);//320Line
	MY_3SPI(0x101);
	MY_3SPI(0x10C);
	MY_3SPI(0x140);
	MY_3SPI(0x101);
	Delayus(5);

	MY_3SPI(0x095); //(35) Timing control
	MY_3SPI(0x126); //P1
	MY_3SPI(0x14D);
	MY_3SPI(0x114);
	MY_3SPI(0x100); //P4
	MY_3SPI(0x126);
	MY_3SPI(0x155); //P6
	MY_3SPI(0x10C);
	MY_3SPI(0x101);
	MY_3SPI(0x1D0); //P9
	MY_3SPI(0x101);
	MY_3SPI(0x1DE);
	MY_3SPI(0x120); //P12
	MY_3SPI(0x122); //P13

	MY_3SPI(0x0A7);
	MY_3SPI(0x100);//0x10F
	Delayus(30);
	MY_3SPI(0x096);
	MY_3SPI(0x101); //1:Invalid
	Delayus(5);
	MY_3SPI(0x0A1);//
	MY_3SPI(0x12A);
	MY_3SPI(0x156);
	Delayus(5);
	MY_3SPI(0x01d);//temp sensor off
	MY_3SPI(0x1A5);
	Delayus(5);
	MY_3SPI(0x012);//Display on
	MY_3SPI(0x1A5);
	Delayms(100);//100ms,>=6
#endif
}
#endif
void IVT_reloc(void)
{
#pragma asm
	//changing INTBP
	MOV 0x020200, R6
	LDSR 	R6,intbp,1

#pragma endasm
}
#ifdef GRAYSCALE_TFT
Public void draw_grayscale(uint32_t r,uint32_t c)
{
	drome_SetDispReadOutParamStat status;
#if 1
	//if(logo_completed==1)
	{
		status = drome_setScreenAreaReadOut( c, (r*2), IMG_STEP, 2, buffer);
		//status = drome_setScreenAreaReadOut( c, (r*2)+1, IMG_STEP, 1, (buffer+240));
		if( drome_SDROPS_OK == status )
		{
			while(drome_DROS_NOT_FINISHED == drome_doScreenAreaReadOut()){}
			//bufferinit_flag=0xA5;
			loc_WriteImage_fast(r,c,buffer);
//			loc_WriteImage(buffer);
		}
		else if( drome_SDROPS_BAD_STATE == status )
		{
			//TRY AGAIN
		}
		else
		{
			//ERROR SCREEN AREA OUT OF RANGE
		}
	}
#endif
	//status = drome_GreyTFTBufDump(240,320,buffer_grayscale);
/*	loc_WriteImage(buffer);*/
}
#endif

#define R_STBC_WUL_INTP9_BIT10        (1 << 10)
#define R_STBC_WUL_INTP1_BIT2        (1 << 2) /* Port 0 pin 2 INTP1 Turn right*/
#define R_STBC_WUL_INTP0_BIT1        (1 << 1) /* Port */
#define R_STBC_WUL_INTP5_BIT6        (1 << 6) /* Port 0 pin 0 INTP5 CAN*/
#define R_STBC_WUL_INTAWOT0_BIT16        (1 << 16) /* Port */
#define R_STBC_WUL_INTP7_BIT8				(1 << 8)
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)); }

/* control registers */
#define STBC_WUF0     (0xFFF80400u) /* enable, active high */
#define STBC_WUFMSK0  (0xFFF80404u) /* mask, active low */
#define STBC_WUFC0    (0xFFF80408u) /* wakeup factor clear register */
#define STBC_STBC0PSC (0xFFF80100u) /* standby register */
#define STBC_IOHOLD   (0xFFF80B00u) /* IO hold */
/* options */
#define STBC_DEEPSTOP (0x00000002u) /* Deepstop mode */

/* protected registers */
#define STBC_PROTCMD0 (0xfff80000u)

static void locRtKnobAAction(void)
{
	//protected_write(SYSPROTCMD0,SYSPROTS0,SYSSWRESA,0x01);/*Software reset*/
	//locPinA = 1;
	/* (re)start the "debouncing" timer channel */
	//  R_TAUJ_Start(0, R_TAUJ_CHANNEL_0);
}


static void TurnRISR(void)
{

	uint8_t int_turnR_flag;
	int_turnR_flag = 1;
}

/***********************************************************
Section: Local Variables
 */

/*******************************************************************************
Function: main

entry after startup
 */
#ifdef COLOR_TFT
#ifdef SPRINT4
unsigned char App_Vesion[8]="W501H239";
#else
unsigned char App_Vesion[8]="W501H242";
#endif
unsigned char HWC_Version[4]="358E";
#endif

#ifdef GRAYSCALE_TFT
unsigned char App_Vesion[8]="W501L243";
unsigned char HWG_Version[4]="358E";
#endif

#ifdef U350
unsigned char App_Vesion_U350[8]="U350L230";
unsigned char HWU_Version[4]="358E";
#endif

r_awot_Parameter_t awot;
PRIVATE uint8_t awot_flag_50ms;

/*void jp0_isr1()
{
    //R_GPIO_TogglePin(17,1);
    INTC1.EIC15.BIT.EIRF15 =0;
}

void init_port_jp0_1(void) {                  initialize ports


  R_GPIO_SetIntCallback(9,jp0_isr1 );
  R_GPIO_EnableInt(9);
    PORTJ_AWOPCR_JPMC0_1 =1;
  PORTJ_AWOPCR_JPM0_1 =1;
 //PORTJ_AWOPCR_JPU0_1 =1; //onchip pull up because board doesn't have
  PORTJ_AWOPCR_JPD0_1 =1; // 1= onchip pull down if board doesn't have Pull down
  PORTJ_AWOPCR_JPFC0_1 =0;
  PORTJ_AWOPCR_JPFCE0_1 =0;

  PORTJ_AWOJPIBC0_1 =0;
  //FCLA1CTL2 = 0x02; // falling edge
  FCLA1CTL2 = 1; // Rising edge
  INTC1EIP15 = 0;   // INTP9
  INTC1.EIC15.BIT.EIRF15 =0;
}*/

#if 0
void awot_isr1()
{
	asm("nop");
	if(awot_flag_50ms==1)
	{
		awot_flag_50ms=0;
		R_GPIO_WritePin(0, 4, 0);
		//R_AWOT_create();
	}
	//R_AWOT_Stop(0);
	//	R_GPIO_TogglePin(0,4);
}

void R_AWOT_create()/*1000ms*/
{
	awot_flag_50ms=0;
	awot.Mode = R_AWOT_MODE_INTERVAL;
	awot.CmpVal = 0x8340;
	//    awot.IntCtl = 1;

	R_AWOT_GetClock(0);
	R_AWOT_Init(0,&awot);
	R_AWOT_EnableInt(0, R_AWOT_INT_TINT);
	R_AWOT_SetIsrCallback(0,R_AWOT_INT_TINT,awot_isr1);


	//R_AWOT_Start(0);
}

void R_AWOT_create1()/*50ms*/
{
	awot_flag_50ms=1;
	awot.Mode = R_AWOT_MODE_INTERVAL;
	awot.CmpVal = 0x640;

	R_AWOT_GetClock(0);
	R_AWOT_Init(0,&awot);
	R_AWOT_EnableInt(0, R_AWOT_INT_TINT);
	R_AWOT_SetIsrCallback(0,R_AWOT_INT_TINT,awot_isr1);


	if(R_AWOT_Start(0) != R_AWOT_ERR_OK)
	{
		while(1);
	}
}

#endif

r_awot_Parameter_t rt_awot_Config;
unsigned char my_50msec_flag;
unsigned char my_count;


void my_AWOT_isr()
{
	asm("nop");
	R_GPIO_TogglePin(0,4);
	my_50msec_flag = 1;

	my_count++;

}

void my_awot_start(void)
{
	if(R_AWOT_Start(0) != R_AWOT_ERR_OK)
	{
		while(1);
	}

}
void my_awot_init(void)
{
	rt_awot_Config.Mode = R_AWOT_MODE_INTERVAL;
	rt_awot_Config.CmpVal = 4000000;                                     // interval time is 0.5 sec --> 4000000/8MHz --> 0.5sec

	R_AWOT_GetClock(0);
	R_AWOT_Init(0,(r_awot_Parameter_t *)&rt_awot_Config);
	R_AWOT_EnableInt(0,R_AWOT_INT_TINT);
	R_AWOT_SetIsrCallback(0,R_AWOT_INT_TINT,&my_AWOT_isr);

}

void my_awot_init50ms(void)
{
	rt_awot_Config.Mode = R_AWOT_MODE_INTERVAL;
	rt_awot_Config.CmpVal = 400000;                                       // interval time is 50msec --> 400000/8MHz --> 0.05sec

	R_AWOT_GetClock(0);
	R_AWOT_Init(0,(r_awot_Parameter_t *)&rt_awot_Config);
	//R_AWOT_EnableInt(0,R_AWOT_INT_TINT);
	R_AWOT_SetIsrCallback(0,R_AWOT_INT_TINT,&my_AWOT_isr);

	if(R_AWOT_Start(0) != R_AWOT_ERR_OK)
	{
		while(1);
	}
}

void my_awot_init1sec(void)
{
	rt_awot_Config.Mode = R_AWOT_MODE_INTERVAL;
	rt_awot_Config.CmpVal = 7200000;			// interval time is 1 sec --> 8000000/8MHz --> 1sec

	R_AWOT_GetClock(0);
	R_AWOT_Init(0,(r_awot_Parameter_t *)&rt_awot_Config);
	R_AWOT_EnableInt(0,R_AWOT_INT_TINT);
	R_AWOT_SetIsrCallback(0,R_AWOT_INT_TINT,&my_AWOT_isr);

	if(R_AWOT_Start(0) != R_AWOT_ERR_OK)
	{
		while(1);
	}
}
enum
{
	WAKEUP_IMMO=1,
	WAKEUP_TURNR,
	WAKEUP_TURNL,
	WAKEUP_POSITIONLAMP,
	WAKEUP_CAN,
	WAKEUP_IGN,
};
/*****Port Hiz *****/
void portHiz()
{
	// protected_write(SYSPROTCMD0,SYSIOHOLD,0x0000007F);

	PORT_ISOPM1 |= 0xFFFF;
	PORT_ISOPMC1 &= 0x0000;
	PORT_ISOPIBC1 &= 0x0000;

	PORT_ISOPM3 |= 0xFFFF;
	PORT_ISOPMC3 &= 0x0000;
	PORT_ISOPIBC3 &= 0x0000;

	PORT_ISOPM10 |= 0xFFFF;
	PORT_ISOPMC10 &= 0x0000;
	PORT_ISOPIBC10 &= 0x0000;

	PORT_ISOPM11 |= 0xFFFF;
	PORT_ISOPMC11 &= 0x0000;
	PORT_ISOPIBC11 &= 0x0000;

	PORT_ISOPM16 |= 0xFFFF;
	PORT_ISOPMC16 &= 0x0000;
	PORT_ISOPIBC16 &= 0x0000;

	PORT_ISOPM17 |= 0xFFFE;
	PORT_ISOPMC17 &= 0x0000;
	PORT_ISOPIBC17 &= 0x0000;

	PORT_ISOPM21 |= 0xFFFF;
	PORT_ISOPMC21 &= 0x0000;
	PORT_ISOPIBC21 &= 0x0000;

	PORT_ISOPM22 |= 0xFFFF;
	PORT_ISOPMC22 &= 0x0000;
	PORT_ISOPIBC22 &= 0x0000;

	PORT_ISOPM42 |= 0xFFFF;
	PORT_ISOPMC42 &= 0x0000;
	PORT_ISOPIBC42 &= 0x0000;

	PORT_ISOPM43 |= 0xFFFF;
	PORT_ISOPMC43 &= 0x0000;
	PORT_ISOPIBC43 &= 0x0000;

	PORT_ISOPM44 |= 0xFFFF;
	PORT_ISOPMC44 &= 0x0000;
	PORT_ISOPIBC44 &= 0x0000;

	PORT_ISOPM45 |= 0xFFFF;
	PORT_ISOPMC45 &= 0x0000;
	PORT_ISOPIBC45 &= 0x0000;

}

/************Goto Stop mode******/
void gotoDeepStop()
{

	/*       if (SYS.RESF.BIT.RESF10 == 1)	{
 -         if (SYS.WUF0.BIT.WUF07 != 1)	{
 -         // if ((SYS.WUF0.BIT.WUF07 | SYS.WUF0.BIT.WUF00) != 1)	{
 -             while(1);   // Error
 -         }
 -     }*/
#if 0
	PBG.FSGD0BPROT0.UINT32	= 0x07FFFFFF;   // unlock the PBUS Guard

	do {
		WRITE_PROTECTED_MACRO(SYS.PROTCMDMRST.UINT32, SYS.MRSTC.UINT32, 0x00003FFF);	// MRSTC reset release
	}while (SYS.PROTSMRST.UINT32 == 1);
#endif


	//R_UTIL_DHD_DeInit(LOC_DHD_UNIT);
	asm("di");

	do
	{
		SYSWUFC0 = 0xFFFFFFFF;
	}while(SYSWUF0 !=0);

	INTC1.EIC12.BIT.EIMK12	= 1;            // INTP6 interrupt masked

	// P0_5/CAN0RX/INTP6(alt 4)  setting
	PORT_AWO.PFC0.BIT.PFC0_5	= 1;
	PORT_AWO.PFCE0.BIT.PFCE0_5	= 1;
	PORT_AWO.PMC0.BIT.PMC0_5	= 1;
	PORT_AWO.PM0.BIT.PM0_5		= 1;
	PORT_AWO.PIBC0.BIT.PIBC0_5  = 0;	// input buffer disabled. PIBCn_m to be set to 0 when S/W I/O control alternative input (PMCn_m=1, PMn_m=1)
	FCLA0.CTL7.UINT8	= 0x2;		 // INTP6 setting: Level detection disabled, Falling edge detection enabled, Rising edge detection disabled

	INTC1.EIC12.BIT.EIRF12      = 0;        // INTP6 interrupt request cleared
	SYS.WUFMSK0.BIT.WUFMSK07	= 0;        // INTP6 wakeup factor enabled(WUF0[7] unmasked)
	SYS.WUFC0.BIT.WUFC07        = 1;        // WUFC07 wake up factor flag cleared

	do {
		my_protected_write(SYS.PROTCMD0.UINT32, SYS.STBC0PSC.UINT32, 0x00000002);	// Entering Deepstop
	}while (SYS.PROTS0.UINT32 == 1);


}


void WDTA1_ISR()
{

	if(WDTA1_INIT_COMPLETE!=0xA5)
	{
		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
	}
	eeprom_write(rtc_hrs,RTCA0HOURC);
	eeprom_write(rtc_min,RTCA0MINC );
	eeprom_write(rtc_sec,RTCA0SECC);
}



void CheckAWOT()
{


WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR

#if 0
	RRAM_hrs = (uint32_t*)0x3FCE4000;
	RRAM_min = (uint32_t*)0x3FCE4004;
	RRAM_sec = (uint32_t*)0x3FCE4008;

	if(SYSRESF & 0X00000001)
	{
		/*nothing to do*/
	}
	else
	{
		RTCA0WAIT = 1;
		*RRAM_hrs = RTCA0HOURC;
		*RRAM_min = RTCA0MINC;
		*RRAM_sec = RTCA0SECC;
		RTCA0WAIT = 0;
	}

#endif
	WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR


#ifdef DEBUG
#else
	uint32_t my_wake_factor;
	uint32_t my_rosc_mask;
	uint32_t my_wake_mask;
	uint8_t i;
	uint32_t my_read_awot_clk_select, my_read_awot_clk_dvd, my_read_awot_stop;

	my_read_awot_clk_select = SYSCKSC_AAWOTS_CTL;
	my_read_awot_clk_dvd = SYSCKSC_AAWOTD_CTL;
	my_read_awot_stop = SYSCKSC_AAWOTD_STPM;

	SYSCKSC_AAWOTD_STPM = 0x03;

	my_read_awot_stop = SYSCKSC_AAWOTD_STPM;

	my_rosc_mask = SYSROSCSTPM;
	my_wake_factor = SYSWUF0;
	if((SYSIOHOLD == 0x7E)||(SYSIOHOLD == 0x7F)||(SYSIOHOLD == 0x3E))
	{
		my_wake_factor=0x00010000;
	}
	else
	{
		my_wake_factor=0x00000002;
	}
	if(my_wake_factor == 0x00000002)
	{
		i=1;
	}
	else if(my_wake_factor == 0x00010000)
	{
		SYSROSCSTPM = 0x00000000;
		my_50msec_flag = 0;
		R_GPIO_WritePin(0, 4, 1);
		my_awot_init50ms();

		while(my_50msec_flag == 0)
		{
			if( INTC1.EIC5.BIT.EIRF5 == 1)
			{
				my_50msec_flag=1;
				INTC1.EIC5.BIT.EIRF5=0;
			}
		};

		R_AWOT_Stop(0);

		R_GPIO_WritePin(0, 4, 0);

		SYSWUFC0 = 0xffffffff;   // clear
		SYSWUFMSK16 = 0;
		my_wake_mask = SYSWUFMSK0;

		my_awot_init1sec();

		/*portHiz();
                      gotoDeepStop();*/
		do
		{
			my_protected_write(SYSPROTCMD0, SYSSTBC0PSC, 0x02);
		}while(SYSPROTERR == 1);


		/*  do
                          {
                                         my_protected_write(SYSPROTCMD0, SYSSTBC0PSC, 0x02);
                          }while(SYSPROTERR == 1);*/

		while(1);
	}

#if 0
	if((my_wake_factor && 0x10000) == 1)
	{
		SYSROSCSTPM = 0x00000000;
		my_50msec_flag = 0;
		R_GPIO_WritePin(17, 0, 1);
		my_awot_init50ms();

		while(my_50msec_flag == 0)
		{
			if( INTC1.EIC5.BIT.EIRF5 == 1)
			{
				my_50msec_flag=1;
				INTC1.EIC5.BIT.EIRF5=0;
			}
		};

		R_AWOT_Stop(0);

		R_GPIO_WritePin(17, 0, 1);

		SYSWUFC0 = 0xffffffff;   // clear
		SYSWUFMSK16 = 0;
		my_wake_mask = SYSWUFMSK0;

		my_awot_init1sec();

		portHiz();
		gotoDeepStop();

		/*                         do
-                          {
-                                         protected_write(SYSPROTCMD0, SYSSTBC0PSC, 0x02);
-                          }while(SYSPROTERR == 1);*/

		while(1);
	}
#endif
#endif
}
uint8_t wakeup_factor_flag,TurnLR_TT_Off = 0;
uint32_t my_read_awot_stop,my_rosc_mask,my_wake_factor,my_wake_mask;

int32_t main(void)
{

	static volatile uint32_t my_switch = 0;
	static char main_screen_printed_once;
	uint32_t i,j, display_key,spdgauge_position,tacogauge_position;
	static uint16_t y_start, y_end;
	uint32_t *awot_wakeup_factor, wakeup_factor,wakeup_factor3;

	WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
	RRAM_hrs = (uint8_t*)0x3FCE4000;
	RRAM_min = (uint8_t*)0x3FCE4004;
	RRAM_sec = (uint8_t*)0x3FCE4008;
	RRAM_IMMO_PATT4 =(uint8_t*)0x3FCE4010;
	RRAM_IMMO_PATT5 =(uint8_t*)0x3FCE4011;


#ifdef PRODUCTION
	unsigned char read_EE_default[EEPROM_BYTES];
#endif
	//	protected_write(SYSPROTCMD0,SYSPROTS0,SYSIOHOLD,0x00);/*release iohold*/




	RESET:
//	unsigned int READ_RESET=SYSRESF;   // Uncomment register to see processor reset status

	IGN_ON_flag = 0;
	INIT_DONE_flag = 0;
	IGNOFF_MBFM_CAN_ON = 0;
	IVT_reloc();


	WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
	R_WDTA_SetIsrCallback(1,R_WDTA_INT_75_PERCENT,&WDTA1_ISR);
	R_WDTA_EnableInt(1);
	WDTA1_INIT_COMPLETE = 0xA5;

	R_BSP_Init();
	R_DEV_PinInit (loc_PinConfig);
	welcome_index = 0;
	//R_GPIO_WritePin(0, 4, 1);
	//R_GPIO_WritePin(0, 4, 0);
	/* do{
	    my_protected_write(SYSPROTCMD0,SYSCKSC_AAWOTS_CTL,0x00000003);
	    }while (SYSCKSC_AAWOTS_ACT==0);*/
	//R_AWOT_create1();


	awot_wakeup_factor = (uint32_t *)(STBC_WUF0);

	wakeup_factor = *awot_wakeup_factor & 0xFFFFFFFF/*0x00010000*/;

	/*Renesas*/
	SYSCKSC_AAWOTD_STPM = 0x03;

	my_read_awot_stop = SYSCKSC_AAWOTD_STPM;

	my_rosc_mask = SYSROSCSTPM;
	my_wake_factor = SYSWUF0;
	unsigned int READ_RESET = SYSRESF & 0X00000200;
	unsigned int READ_ALL_RESET = SYSRESF & 0X0000420F;
	READ_RTC_RESET = READ_ALL_RESET;
		if(READ_RESET != 0)		/*For Battery Reset Condition*/
		{
			if(turn_activate<2)
			turn_activate=1;
			BATT_DISCONNECTION = 1;
			eeprom_writebt(BATT_DISCONNECTION_OFFSET,1); //Write Battery disconnection
			*RRAM_hrs=0;
			*RRAM_min=0;
			*RRAM_sec=0;
		}

		rtc_wakeup = 1;//To avoid Time format change
		if(READ_ALL_RESET!=0)
		{

			/*if((eeprom_read(RESET_R_FLAG_OFFSET))==0x55)
			{
				hrs = eeprom_read(rtc_hrs);
				min = eeprom_read(rtc_min);
				sec = eeprom_read(rtc_sec);
				eeprom_write(RESET_R_FLAG_OFFSET,0x00);
			}
			else*/
			{
				hrs = *RRAM_hrs;
				min = *RRAM_min;
				sec = *RRAM_sec;

			}

			rtc_wakeup = 0;
		}
		else
		{
			RTCA0WAIT = 1;
			hrs = RTCA0HOURC;
			min = RTCA0MINC;
			sec = RTCA0SECC;
			RTCA0WAIT = 0;

			*RRAM_hrs = hrs;
			*RRAM_min = min;
			*RRAM_sec = sec;
		}
		TimeF_selected = eeprom_read(SETTING_TIMEFRMT_OFFSET);
		/*eeprom read AM flag*/
		if((hrs &(1<<5))==0x20)
		{
			AM_flag=0;
		}
		else
		{
			AM_flag=1;
		}
		//AM_flag=eeprom_read(AMPM_FLAG_OFFSET);
#ifndef DEBUG
		wakeup_factor = 0x0;
#endif
		eeRtc_Init(rtc_wakeup);

	if(IGN==1)
	{
		// wakeup_factor_flag = WAKEUP_IGN;
		if(turn_activate==0)
			turn_activate=1;
		//EEPROM_readwrite_para_started_on =1;
		my_can0_init();//r instant init of CAN after ign on NOK
		my_can0_tx_IC10_200(IC10_200,8,CAN_IC10_200);// Transmit IC10_200 message
		R_GPIO_WritePin(16, 8, 1);
		R_GPIO_WritePin(3, 4, 1);
		protected_write(SYSPROTCMD0,SYSPROTS0,SYSIOHOLD,0x00);/*release iohold*/

		test_rtc_reset_flag=1;


		init_port_jp0_2();
		init_port_p0_2();
		init_port_jp0_1();
//		if(turn_activate<5)
//		turn_activate=1;
		IGN_ON_flag = 1;
		INIT_DONE_flag = 1;
#ifdef PRODUCTION
		for(uint32_t i=0;i<=EEPROM_BYTES;i++)
		{
			if((i>=0x858)&&(i <= 0x860))
			{
				//do nothing
			}
			else
			{
				eeprom_writebt(i, ee_default[i]);//i is memory location
				WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
				IMMOBIIZER_TT = 1;
			}
		}
/*Test Only*/
//		for(uint32_t j=0;j<=EEPROM_BYTES;j++)
//				{
//					read_EE_default[j] = eeprom_read(j);
//					 WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
//				}
/**/
		for(uint32_t i=0x858;i<=0x860;i++)
		{
			eeprom_writebt(i, ee_default[i]);//i is memory location
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			IMMOBIIZER_TT = 1;
		}

		IMMOBIIZER_TT = 0;
#endif
	}


	Split32 wakeup_factor2,wakeup_factor1;
	wakeup_factor2.u32 = wakeup_factor;
	delay_time(100);
	delay_time(100);
	delay_time(100);
	//eeprom_memwrite_32bytes(IMO_TT_OFFSET, wakeup_factor2.u8, 4);
	delay_time(100);
	delay_time(100);
	delay_time(100);
	eeprom_memread_32bytes(IMO_TT_OFFSET,wakeup_factor1.u8, 4);
	delay_time(100);
	delay_time(100);
	delay_time(100);
	wakeup_factor3=wakeup_factor1.u32;
#ifdef DEBUG
	wakeup_factor3=wakeup_factor;				/*Only use in debug mode*/
#endif
	if(((wakeup_factor3 == 0x00000002)||(IGN==1))&&(IGN_ON_flag == 1))
	{
		wakeup_factor_flag = WAKEUP_IGN;
		my_can0_init();//r instant init of CAN after ign on NOK
		my_can0_tx_IC10_200(IC10_200,8,CAN_IC10_200);// Transmit IC10_200 message
		IGN_ON_flag = 1;
		INIT_DONE_flag = 1;
		Bootup_Trigger = 1;	/*Boot up animation start*/
	}
	else if((wakeup_factor3 == 0x00000004)||(wakeup_factor == 0x00010004)||(wakeup_factor3 == 0x00000104)||(wakeup_factor == 0x00010104)||(wakeup_factor3 == 0x00000100))
	{
		wakeup_factor_flag = WAKEUP_TURNR;
		IMMO_TT_WAKEUP_FLAG = 1;
		previous_state_immobilizer_blink4= *RRAM_IMMO_PATT4;	/*Immo status carry on when pseudo sleep to standby by TurnLR wakeup -> 12/06/20 ->W501H234*/
		previous_state_immobilizer_blink5= *RRAM_IMMO_PATT5;
	}

	else if(wakeup_factor3 == 0x40)
	{
		wakeup_factor_flag = WAKEUP_CAN;
		/*For Immo Issue Test*/
		IMMO_TT_WAKEUP_FLAG = 1;
		previous_state_immobilizer_blink4= *RRAM_IMMO_PATT4;	/*Immo status carry on when pseudo sleep to standby by CAN wakeup */
		previous_state_immobilizer_blink5= *RRAM_IMMO_PATT5;
	}
	else if((wakeup_factor3 == 0x00000400)||(wakeup_factor == 0x00010400))
	{
		wakeup_factor_flag = WAKEUP_POSITIONLAMP;
		IMMO_TT_WAKEUP_FLAG = 1;
		previous_state_immobilizer_blink4= *RRAM_IMMO_PATT4;	/*Immo status carry on when pseudo sleep to standby by Position lamp wakeup -> 12/06/20 ->W501H234*/
		previous_state_immobilizer_blink5= *RRAM_IMMO_PATT5;
	}

	else
	{
		wakeup_factor_flag = 0;
	}



	if((wakeup_factor_flag == WAKEUP_CAN)||(wakeup_factor_flag == WAKEUP_TURNR)||(wakeup_factor_flag == WAKEUP_POSITIONLAMP))
	{
		if(IGN==0)		/*Other than Immo*/
		{
			EEPROM_readwrite_para_started_on =0;
			IGNOFF_eeprom_read = 0;	/*Acknowledgement of FC VC read*/
			IGNOFF_commoninit();
			Standby_Mode();

		}
}
#if 1
	else if(wakeup_factor_flag==WAKEUP_IMMO)
	{
		my_50msec_flag = 0;
		R_GPIO_WritePin(0, 4, 1);
		my_awot_init50ms();

		while(my_50msec_flag == 0);

		R_AWOT_Stop(0);

		R_GPIO_WritePin(0, 4, 0);
		SYSWUFC0 = 0xffffffff;   // clear
		SYSWUFMSK16 = 0;
		my_wake_mask = SYSWUFMSK0;

		my_awot_init1sec();
		//my_awot_start();

		//for(i = 0; i < 0xfffe; i++);

		do
		{
			my_protected_write(SYSPROTCMD0, SYSSTBC0PSC, 0x02);
		}while(SYSPROTERR == 1);

		while(1);
	}
#endif



	protected_write(SYSPROTCMD0,SYSPROTS0,SYSIOHOLD,0x00);/*release iohold*/
	init_port_jp0_2();
	init_port_p0_2();
	init_port_jp0_1();
//	if(turn_activate<5)
//	turn_activate=1;
	welcomedone=0;
	Proveout_Count =0;
	WatchDog_flag = 0; /* pulse to watchdog */
	/* configure GPIO for time measurement */
	Off_diagTTflag = 0;
	R_GPIO_WritePin(3, 0, 1);
//	R_DEV_PinInit (loc_PinConfig);//rajat
	WDTA1WDTE = 0xAC;
#if 0
	test_rtc_reset_flag=1;
	if(READ_ALL_RESET!=0)
	{
		hrs = eeprom_read(rtc_hrs);
		min = eeprom_read(rtc_min);
		sec = eeprom_read(rtc_sec);
		TimeF_selected = eeprom_read(SETTING_TIMEFRMT_OFFSET);
		wakeup_factor = 0x0;
	}
	eeRtc_Init(rtc_wakeup);
#endif
	LVHV_detect = eeprom_read(LVHV_Detect_OFFSET);
	//eeprom_write(LVHV_Detect_OFFSET,0x11);
	WDTA1WDTE = 0xAC;

	/*@@@@@@@@@@@@@@@@@@@@*CAN_Buffer_fill*@@@@@@@@@@@@@@@@@@@@*/
	/************************AFE_Read***************************/
	eeprom_memread_32bytes(AFE_DISP_OFFSET,AFEDispVal_EEPROM.u8, 2);
	/**********************AFE_GD_read**************************/
	eeprom_memread_32bytes(AVG_KMPL_GD_AFE_OFFSET,&AFE_3_U.U16_AFE_GD_LastDispVal_EE,2);
	/**********************DTE_read**************************/
	eeprom_memread_32bytes(APP_S_EE_Last_Caltd_DTE,&rtU.S_EE_Last_Caltd_DTE, 4);
	/***********************ODO_Read*************************/
	final_odomter_distance = eeprom_readOdometer();
	old_odometer_distance = final_odomter_distance;
	/***********************PAB_Status************************/
	PAB_selected = eeprom_read(SETTING_PAB_OFFSET); /* read PAB_selected*/
	/***********************4WD_Status************************/
	STS_4WD = eeprom_read(STS_4WD_OFFSET);
	/***********************Engine_Temp************************/
	/*Not able to receive msg due to network monitoring start after 2 sec.*/
//	for(uint8_t i = 0; i < 4; i++)
//	{
//		temp_show_TT[i] = eeprom_read(DID_050E_OFFSET + i);
//	}

	/***********************Park_Break************************/
	input_shift_register();//to take valid input from Shift register
	if(PARK_BRAKE_CPU == 0)
	{
		CAN_IC1_100[3].bits.Rx_7=1;
	}
	else if(PARK_BRAKE_CPU == 1)
	{
		CAN_IC1_100[3].bits.Rx_7=0;
	}
	/**********************AFE_Val**************************/
	AFE_3_U.U16_AFE_LastDispVal_EE = AFEDispVal_EEPROM.u16;
	AFE_3_Y.U16_AFEDispVal = AFEDispVal_EEPROM.u16;
	AFEDispVal = AFEDispVal_EEPROM.u16;				/*Update after every IGN ON*/
	if(AFEDispVal != 0)
	{
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
	else
	{
		CAN_IC1_100[4].byte = 0xFF;		/*After AFE resetting CAN TX -> 0x1ff*/
		CAN_IC1_100[5].bits.Rx_1 = 1;
	}
	/**********************AFE_GD_Val**************************/

	AvgKmplAFE_GD = AFE_3_Y.U16_AvgKmplAFE_GD;//need to test
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
	/**********************DTE_Val*****************************/

	DTE_Disp_Val = rtU.S_EE_Last_Caltd_DTE;
	rtY.S_DTE_Disp_Val = rtU.S_EE_Last_Caltd_DTE;

	CAN_IC1_100[0].byte = DTE_Disp_Val;
	CAN_IC1_100[1].bits.Rx_1 = DTE_Disp_Val>>8;
	CAN_IC1_100[1].bits.Rx_2 = DTE_Disp_Val>>9;

	/************************Temp******************************/

	/*Not able to receive msg due to network monitoring start after 2 sec.*/

//	temp_show_TT[1] = eeprom_read(DID_050E_OFFSET + 1);
//
//	if(CAN_EMS1_10[7].byte >= (0x28 + (temp_show_TT[1]-40)))
//	{
//		CAN_IC1_100[5].bits.Rx_7=1;
//	}
//	else
//	{
//		CAN_IC1_100[5].bits.Rx_7=0;
//	}

	/**************************PAB******************************/
//	if(FEATURE_CODE_PAB_SETTING == 1)
//	{
		CAN_IC2_100[5].bits.Rx_4 = PAB_selected;		/*PAB Status*/
//	}
//	else
//	{
//		PAB_selected = 0;
//		CAN_IC2_100[5].bits.Rx_4 = PAB_selected;		/*PAB Status*/
//	}

	/**************************4WD******************************/
//		CAN_IC2_100[3].byte=(STS_4WD&0x60);
//		FOUR_WD_INIT_STS();
#if 0
		if(STS_4WD == 0)//test
		{
			R_GPIO_WritePin(17, 10, 1);//pin high for 2WD on
		}
		else
		{
			R_GPIO_WritePin(17, 10, 0);
		}
#endif
	/************************************************************/

			Read_odo_flag = 1;
	/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



#ifdef PCBrevB
	R_GPIO_WritePin(1, 3, 1); /* 5V_SWSC enable */
#else
	R_GPIO_WritePin(3, 9, 1); /* 5V_SWSC enable */
#endif
	R_GPIO_WritePin(17, 11, 1); /* Fuel Control enable */

	PORTJ_AWOJPM0_2 = 0x1;/*TURN_LEFT input*/
	PORTJ_AWOJPM0_1 = 0x1;/*Park Lamp input*/
	PORTJ_AWOJPM0_5 = 0x1;/*Front fog input*/

	/*Input buffer enable for JTAG pins*/
	PORTJ_AWOJPIBC0_1=1;
	PORTJ_AWOJPIBC0_2=1;
	PORTJ_AWOJPIBC0_5=1;
	/*IMMOBILIZER_TT*/
	PORT_ISOPMC16_10 = 0;
	PORT_ISOPM16_10 = 0;
	PORT_ISOP16_10 = 0;
	Enter_while_flag=0;
	/*tft without drome*/
	/*
	loc_SetupMemManager();
	PORTJ_AWOJPM0_2 = 0x1;
    R_UTIL_DHD_Init(LOC_DHD_UNIT);
    R_UTIL_DHD_Config((dhd_gpu_ptr_t)loc_GPU_Memory_Addr, DHD_MEMORY_SIZE, &loc_lRAM_heap);

	loc_App();*/
	// R_UTIL_DHD_DeInit(LOC_DHD_UNIT);
	/**/
#ifdef PCBrevB
	R_GPIO_WritePin(10, 5, 1);
#else
	R_GPIO_WritePin(3, 8, 1);  /*5V_CONTROL_TT_DRIVER*/

#endif
	//WUFMSK0[2]=0x00;
	//R_STBC_EnableWakeUpFactor(R_STBC_WUL_INTP1_BIT1);
	/* clear the WUFMSK0 — Wake-up factor mask register
	       for each wakeup set the bit to 0
	 */
	R_GPIO_WritePin(1, 1, 0);
		R_GPIO_WritePin(1, 2, 0);
		R_GPIO_WritePin(1, 4, 0);
		R_GPIO_WritePin(17, 7, 0);
		//R_GPIO_PinDirection(17,8,0);		//DLED_GCLK
		//	R_GPIO_PinDirection(17,10,0);		//DLED_GCLK
		R_GPIO_WritePin(17, 7, 1);
		//WDT
		//	PORT_ISOPM42_14=1;
		//	PORT_ISOPMC42_14=0;
		//	PORT_ISOPCR42_14=0;
	#ifdef DEBUG
	#else
		PORT_ISOPMC42_14=0;   //Set port mode as GPIO
		PORT_ISOPM42_14=0;    //WDT as output
	#endif
		R_TAUB_EnableInt(0, R_TAUB_INT_CH0);
		R_TAU0_Create();  /*call timer 100us*/
		INTC1.EIC0.BIT.EIP0=0;
		INTC1.EIC17.BIT.EIP17=1;

		R_TAU1_Create();
		INTC2.EIC34.BIT.EIP34 =2;
		 R_TAU2_PWM_TFT();
		R_TAU2_PWM_GCLK();

		R_TAU2_Create();
		INTC2.EIC51.BIT.EIP51=3;

	#ifdef GRAYSCALE_TFT
		R_TAU2_PWM();
	#endif
		TimerJ_Init();
		if(IGN_ON_flag	== SET)
		{
#if 1
		SendDCData(0xA5C, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* reset command*/
#ifdef PCBrevB
#else
		SendDCData(0xA5C, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* reset command*/
		cascade_TT_send_data();

#endif
		diagTTflag=0;
		gcsk_flag=0;
		/////////////
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
#ifdef PCBrevB
		Bit_flag_Access();

		R_GPIO_WritePin(17, 2, 1);/*Gauge pointer illumination*/
		R_GPIO_WritePin(16, 11, 1);/*GI_CONTROL*/

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
			R_GPIO_WritePin(17, 1, 1);/*GI_CONTROL2*/

		}
		else
		{
			/* GAsoline*/
			R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
			R_GPIO_WritePin(17, 0, 1);/*GI_CONTROL1*/
		}

#else
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
		cascade_TT_send_data();
#endif
#endif
/*For Test*/
			/******************************************************************/
#ifdef COLOR_TFT

			//----TFT RESET signal
			PORT_ISOPMC44_5 = 0;
			PORT_ISOPM44_5 = 0;
			PORT_ISOP44_5 = 1;

			/*change for PCB rev B*/
			PORT_ISOPMC44_3 = 0;
			PORT_ISOPM44_3 = 0;
			PORT_ISOP44_3 = 1;


			for(i = 0; i < 0xffff; i++);
			PORT_ISOP44_5 = 0;
			for(i = 0; i < 0xffff; i++);

			PORT_ISOP44_5 = 1;
			/*TFT init*/
#endif
#ifdef GRAYSCALE_TFT
			Lcbi_Non_Tft_Init();
#endif
#ifdef COLOR_TFT
			gui_taskInit();
			dromecall_timeout = 1;
			do
			{
				WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
				handleDromeState();	/*drome call till it will not receive drive summary timeout black screen(startkey = 0x11108)*/
			}while((startkey != 0x11108)&&(dromecall_timeout <= 5000));
			dromecall_timeout = 0;
			(void)drome_sendEventToDrome((U32)drome_KB_DOWN);

			dromecall_timeout = 1;
			do
			{
				WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
				handleDromeState();	/*drome call till it will not receive drive summary timeout black screen(startkey = 0x11108)*/
			}while((startkey != 0x13000)&&(dromecall_timeout <= 5000));
			dromecall_timeout = 0;
			(void)drome_sendEventToDrome((U32)drome_KB_DOWN);

#endif
#ifdef U350
			if(LVHV_detect==0x11)//need to test
			{
				PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
				PORT_ISOPM17_3 = 0;
				PORT_ISOP17_3 = 1;
			}
#endif
			//BOOTUP_WriteImage();
			 Graphics_Loaded = checkvalidgraphicsappln(String_Graphics_Address);
			 Bootup_Trigger = 1;	/*Boot up animation start*/
#if 0
#ifdef U350
//			if(wakeup_factor_flag==WAKEUP_IGN)
		       
			if(((wakeup_factor3 == 0x00000002)||(IGN==1))&&(LVHV_detect==0x11)&&(Graphics_Loaded == 1))
			{
				Mahindra_TextImage();
				Bootup_Trigger = 1;	/*Boot up animation start*/
			}
#endif
#endif
			R_GPIO_WritePin(17,8, 1);
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_11);
			if(LVHV_detect==0x11)//need to test
			{
				//R_GPIO_WritePin(17,8, 1);
				//r_taub_OutputEnable	(2,R_TAUB_CHANNEL_11);
				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;
				GI_Control();	/*GI_Control called for @ IGN ON Gi turn ON immediately*/

				SendGSData(GSdata1);
			}
#ifdef COLOR_TFT
					handleDromeState();
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 1;
#endif
		}


		Write_UDS_Data();	//write default value in EEPROM before read
		updateAppVersion();  						 //Update App version to EEPROM
		updateHWVersion();  						 //Update HW version to EEPROM
		Read_UDS_EEPROM_Data();
		WatchDog_Clear();

		if(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1)
		{
			if(FEATURE_CODE_4WD_LOGIC==0)
			{
				if((FOUR_WD_HIGH_CPU ==1)&&(FOUR_WD_LOW_CPU == 0))		/*4WDL*/
				{
					R_GPIO_WritePin(17, 10, 0);	//pin high for 4WD on
				}
				else if((FOUR_WD_HIGH_CPU ==0)&&(FOUR_WD_LOW_CPU == 1))	/*4WDH*/
				{
					R_GPIO_WritePin(17, 10, 0);	//pin high for 4WD on
				}
				else																				/*2WD/ANY MODE/DEFAULT*/
				{
					R_GPIO_WritePin(17, 10, 1);//pin high for 2WD on
				}
			}
		}

	///////
	r_gpio_IntConfig_t int_cfg;
	int_cfg.Port     = 0;
	int_cfg.Pin      = 1;
	//	int_cfg.Trigger  = R_GPIO_INT_BOTH_EDGES;
	int_cfg.Trigger  = R_GPIO_INT_LEVEL_HIGH;
	int_cfg.Callback = &locRtKnobAAction;
	R_GPIO_InitInt(0, &int_cfg);
	R_GPIO_EnableInt(0);

	/*******P0.0 - CAN1_WAKE UP SIGNAL -INH - Interrupt Config.********/
	r_gpio_IntConfig_t int_cfg1;
	int_cfg1.Port     = 0;
	int_cfg1.Pin      = 0;
	//	int_cfg.Trigger  = R_GPIO_INT_BOTH_EDGES;
	int_cfg1.Trigger  = R_GPIO_INT_LEVEL_HIGH;
	int_cfg1.Callback = &locRtKnobAAction;
	R_GPIO_InitInt(5, &int_cfg1);
	R_GPIO_EnableInt(5);
	/******************************************************************/
	/*******P0.2 - Turn right  - Interrupt Config.********/
	r_gpio_IntConfig_t int_cfg2;
	int_cfg2.Port     = 0;
	int_cfg2.Pin      = 2;
	//	int_cfg.Trigger  = R_GPIO_INT_BOTH_EDGES;
	int_cfg2.Trigger  = R_GPIO_INT_BOTH_EDGES;
	int_cfg2.Callback = &TurnRISR;
	R_GPIO_InitInt(1, &int_cfg2);
	//R_GPIO_SetIntCallback(1,&TurnRISR);
	R_GPIO_EnableInt(1);
	/******************************************************************/
	////
#if 0
	if(STS_4WD != 0)//test
	{
		R_GPIO_WritePin(17, 10, 0);  //Keep it high for 4 WD output (Active Low)
	}
#endif
	R_GPIO_WritePin(3, 0, 1);
	diagTTflag=1;
	/*if(my_switch){
        my_read(0);
        my_write(0,0);
        QosSlowDave();
        QosFastDave();
    }*/

	/*R_DEV_PinInit (loc_PinConfig);*/
	init_port_jp0_2();
	init_port_jp0_1();
	init_port_p0_2();
	init_port_p0_3();
//	if(turn_activate<5)
//	turn_activate=1;

/***********************************************************************/
#if 0
#ifdef COLOR_TFT

	//----TFT RESET signal
	PORT_ISOPMC44_5 = 0;
	PORT_ISOPM44_5 = 0;
	PORT_ISOP44_5 = 1;

	/*change for PCB rev B*/
	PORT_ISOPMC44_3 = 0;
	PORT_ISOPM44_3 = 0;
	PORT_ISOP44_3 = 1;


	for(i = 0; i < 0xffff; i++);
	PORT_ISOP44_5 = 0;
	for(i = 0; i < 0xffff; i++);

	PORT_ISOP44_5 = 1;
	/*TFT init*/
#endif
#ifdef GRAYSCALE_TFT
	Lcbi_Non_Tft_Init();
#endif
#endif
/*************************************************************************/
	/************************/

	/****Non_drome TFT*********/
#ifndef drome
	R_BSP_SetButtonCallback(BSP_BTN_OK_PRESS,       locBtnOkPress);
	R_BSP_SetButtonCallback(BSP_BTN_CENTER_PRESS,   locRotaryCenterPress);

	loc_SetupMemManager();
	R_UTIL_DHD_Init(LOC_DHD_UNIT);
	R_UTIL_DHD_Config((dhd_gpu_ptr_t)loc_GPU_Memory_Addr, DHD_MEMORY_SIZE, &loc_lRAM_heap);

	loc_App();
	R_UTIL_DHD_DeInit(LOC_DHD_UNIT);
#endif
	/*************************************************/
#ifdef ISM
	//	 main_atf(0);/*comment to disable ISM*/
#endif
	R_GPIO_PinDirection(16,7,0);
#ifdef PCBrevB
	R_GPIO_PinDirection(0,4,0);
#else
	R_GPIO_PinDirection(17,9,0);
#endif
	//R_GPIO_PinDirection(3,5,0);//CAN 1
	//R_GPIO_PinDirection(3,7,0);

	R_GPIO_PinDirection(16,8,0);//CAN 1 enable
	R_GPIO_PinDirection(3,4,0);
	/*
	R_GPIO_PinDirection(1,1,0);   //DLED_SDI
	R_GPIO_PinDirection(1,2,0);		//DLED_SCL
	R_GPIO_PinDirection(1,4,0);		//DLED_LATCH
	R_GPIO_PinDirection(17,7,0);		//DLED_BLANK*/


	/*     loc_InitSFMAMango80MHz(); */
	//	my_IIC0_Init(); 	/*commented IIC init*/
	/*my_tell_tale_init();*/
	//R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	SoundPCM_FIRST();

	tx_done = 1;
	//my_can0_tx();
	rx_flag=1;
	R_ADC_Create();
#ifdef COLOR_TFT
	locInit();/*sound*/
#endif
	R_GPIO_Init();
#ifdef GRAYSCALE_TFT
	gui_taskInit();
#endif
	animation_start = 1;

	if(IGN_ON_flag	== SET)
	{

#if 0
		SendDCData(0xA5C, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* reset command*/
#ifdef PCBrevB
#else
		SendDCData(0xA5C, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* reset command*/
		cascade_TT_send_data();

#endif
		diagTTflag=0;
		gcsk_flag=0;
		/////////////
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
#ifdef PCBrevB
		Bit_flag_Access();

		R_GPIO_WritePin(17, 2, 1);/*Gauge pointer illumination*/
		R_GPIO_WritePin(16, 11, 1);/*GI_CONTROL*/

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
			R_GPIO_WritePin(17, 1, 1);/*GI_CONTROL2*/

		}
		else
		{
			/* GAsoline*/
			R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
			R_GPIO_WritePin(17, 0, 1);/*GI_CONTROL1*/
		}

#else
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
		cascade_TT_send_data();
#endif

/*For Test*/
		R_GPIO_WritePin(17,8, 1);
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_11);
					SP_LD1_TT=0xfff;
					SP_LD2_TT=0xfff;
					SP_LD3_TT=0xfff;
					RP_LD1_TT=0xfff;
					RP_LD2_TT=0xfff;
					RP_LD3_TT=0xfff;
					SendGSData(GSdata1);
#endif
		// R_GPIO_PinDirection(1,0,0);
		R_GPIO_WritePin(16, 8, 1);
		R_GPIO_WritePin(3, 4, 1);
		R_GPIO_WritePin(42, 13, 1);
		/*Increment Ignition counter*/
		Diag_Ignition_counter = (eeprom_read(IGN_COUNTER_OFFSET) << 8);
		Diag_Ignition_counter |= eeprom_read(IGN_COUNTER_OFFSET + 1);
		Diag_Ignition_counter++;
		eeprom_writebt(IGN_COUNTER_OFFSET,(Diag_Ignition_counter >> 8));
		eeprom_writebt((IGN_COUNTER_OFFSET + 1),(Diag_Ignition_counter & 0x00ff));
		/*Reset reason counter*/
		reset_counter = eeprom_read(RESET_COUNTER_OFFSET);
		if(reset_counter==0xff)//first time EEPROM location is 0
		{
			reset_counter=0;
		}
		uint8_t reset_reg = 0;
		reset_reg =SYSRESF & 0X00000004;
		if(reset_reg == 0x4)//0x02		Watchdog Reset counter
		{
			reset_counter++;
			eeprom_writebt(RESET_COUNTER_OFFSET,reset_counter);
		}
		/***********************/
#ifndef U350
		if(((SWSC_ADC_value >= ADC_COUNT_UP_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_UP_KEY_OFF)))
		{
			key_press_counter_tpms=1;
		}
		if(((SWSC_ADC_value >= ADC_COUNT_MENU_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_MENU_KEY_OFF))&&(Menu_Pressed == 0))
		{
			//key_press_bfr_ign = 1;
			Menu_Pressed=1;
		}
#endif
		Read_UDS_EEPROM_Data();
		Dtc_init();
		Read_DTC_Status();
		Check_Valid_Feature_Code();
		Check_Vaild_Variant_Code();
		//r my_can0_init();

		//init_ISM();//init_ISM_proveout();//init_ISM();/*init ISM*/
		init_ISM_proveout();
		loc_Ism_TC0001();/*ZPD*/
//		for(uint8_t i=0;i<=5;i++)
		{
//			BATT_DISCONNECTION = eeprom_read(BATT_DISCONNECTION_OFFSET);
		}
//		eeprom_writebt(BATT_DISCONNECTION_OFFSET,1); //Write Battery disconnection

//		eeprom_writebt(MEMORY_READ_TEST_OFFSET,0xaa); //just for memory read test.

		Battery_Condition();//Check LVSD or HVSD flag
		/*if(BATT_DISCONNECTION)
		{
			manual_zpd(1);
			R_TICK_WaitMS(0u, 3000u);
		}
		else*/
		{
			//	manual_zpd(0);
		}
		//loc_Ism_TC0001();/*ZPD*/
		//	Read_UDS_EEPROM_Data();
		read_feature_code(&Feature_code_arr[0]);
		read_varient_code(&Varient_code_arr[0]);
		Bit_flag_Access();
		zpd_done=1;

#if 0
		SendDCData(0xA5C, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1); /* reset command*/
#ifdef PCBrevB
#else
		SendDCData(0xA5C, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1); /* reset command*/
		cascade_TT_send_data();

#endif
		diagTTflag=0;
		gcsk_flag=0;
		/////////////
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
#ifdef PCBrevB
		Bit_flag_Access();

		R_GPIO_WritePin(17, 2, 1);/*Gauge pointer illumination*/
		R_GPIO_WritePin(16, 11, 1);/*GI_CONTROL*/

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
			R_GPIO_WritePin(17, 1, 1);/*GI_CONTROL2*/

		}
		else
		{
			/* GAsoline*/
			R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
			R_GPIO_WritePin(17, 0, 1);/*GI_CONTROL1*/
		}

#else
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
		cascade_TT_send_data();
#endif
#endif

/*For Test*/
		/********************************************/
//		do
		while((IGN_ON_flag	== SET) && ((LVSD_flag_1 == 1) || (HVSD_flag_1 == 1)))
		{
			/*Execute if LVSD or HVSD condition present on IGN on*/
			WatchDog_Clear();//Stay here if voltage is abnormal
			PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at low Vtg*/
			PORT_ISOPM17_3 = 0;
			PORT_ISOP17_3 = 0;
#ifdef GRAYSCALE_TFT
			clear_screen();
#endif
			All_Telltale_OFF();
			IGN_ON_flag = IGN;
//			LVHV_detect = 0x55;
//			eeprom_write(LVHV_Detect_OFFSET,LVHV_detect);
			if(IGN ==0)
			{
				break;
			}
		}
//		while((IGN_ON_flag	== SET) && ((LVSD_flag_1 == 1) || (HVSD_flag_1 == 1)));

		if((IGN_ON_flag	== SET) && (LVSD_flag_1 == 0) && (HVSD_flag_1 == 0))//instant value used for voltage detection
		{

			//	Read_UDS_EEPROM_Data();
			//	handleDromeState();

			R_GPIO_WritePin(17,3, 1);
			All_Telltale_ON();
			if(turn_activate<2)
			turn_activate=1;
/*TPMS Learning Started*/
			switch(FEATURE_CODE_SPEED_INPUT)
			{
			case 0:
				/* ESC2_10 message */
				down_scale.b[0] = CAN_ESC2_10[0].byte;
				down_scale.b[1] = CAN_ESC2_10[1].byte;

				final_vechile_speed = (down_scale.w * 0.015625);
				final_vechile_speed_gauge_raw = down_scale.w;
				break;

			case 1:
				/* ESC12_10 message */
				down_scale.b[0] = CAN_ESC12_10[2].byte;
				down_scale.b[1] = CAN_ESC12_10[3].byte;

				final_vechile_speed = (down_scale.w * 0.015625);
				final_vechile_speed_gauge_raw = down_scale.w;
				break;

			default:
				/* nothing */
				break;
			}
#ifdef U350
			if((R_GPIO_PinRead(17,4) == 0)&&(key_press_bfr_ign == 0))//Mode Key press from IGN Off--> IGN On
			{

				key_press_bfr_ign = 1;
				if(Diag_RW_ETM_enable == 1)
				{
					ETM_Mode_Enter_timer = 1;//ETM_Mode_Enter_timer flag set after Mode Key press from IGN Off--> IGN On for 5 sec
				}
				else
				{
					ETM_Mode_Enter_timer = 0;
				}
				if(final_vechile_speed_gauge_raw < 0x1C0)
				{
					TPMS_ENTER_PR = 1;
				}
				else
				{
					TPMS_ENTER_PR = 0;
				}
			}

			if((R_GPIO_PinRead(17,5) == 0)&&(key_press_bfr_ign == 0))//Set key press from IGN Off--> IGN On
			{
				key_press_bfr_ign = 1;
				NVM_SERVICE_DONE_flag = 1;
				eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);
				DID_NVM_SERVICE_DONE_0x04F3_arr[0] = eeprom_read(DID_04F3_OFFSET);
			}
#endif
#ifndef U350
			/*Down key press from IGN Off--> IGN On*/
			if(((SWSC_ADC_value >= ADC_COUNT_DOWN_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_DOWN_KEY_OFF))&&(key_press_bfr_ign == 0))
			{
				key_press_bfr_ign = 1;
				if(Diag_RW_ETM_enable == 1)
				{
					ETM_Mode_Enter_timer = 1;//ETM_Mode_Enter_timer flag set after Mode Key press from IGN Off--> IGN On for 5 sec
				}
				else
				{
					ETM_Mode_Enter_timer = 0;
				}
			}

			/*OK key press from IGN Off--> IGN On*/
			if(((SWSC_ADC_value >= ADC_COUNT_OK_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_OK_KEY_OFF))&&(key_press_bfr_ign == 0))
			{
				key_press_bfr_ign = 1;
				NVM_SERVICE_DONE_flag = 1;
				eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);
				DID_NVM_SERVICE_DONE_0x04F3_arr[0] = eeprom_read(DID_04F3_OFFSET);
			}

#endif

/*TPMS Learning End*/
#if 0
			PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
			PORT_ISOPM17_3 = 0;
			PORT_ISOP17_3 = 1;
#endif
			CHECK_SID_DATA();
			GI_CONTROL_FLAG=1;
					/*do{

						handleDromeState();
						draw_grayscale();
					}while(animation_start == 1);*/
#if AFE
	AFE_3_initialize();
#endif
			proveout_routine();
			/****************************************************************************/
			if(Read_Para_Flag == 0)
			{
				Init_read_parameters();
			}

			handleDromeState();
			handleDromeState();
			/*************************************************************/
			//			handleDromeState();	/*function renaming*/
			//			handleDromeState();	/*function renaming*/
			//			handleDromeState();	/*function renaming*/
			/*************************************************************/
			//-------- TFT Backlight
			//			PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
			//			PORT_ISOPM17_3 = 0;
			//			PORT_ISOP17_3 = 1;

			R_GPIO_WritePin(17,3, 1);
			r_taub_SetPeriod		(2,R_TAUB_CHANNEL_5,300/1);
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
			//r_taub_OutputEnable	(2,R_TAUB_CHANNEL_11);
#if 0
#ifdef GRAYSCALE_TFT
			if(first_screen_timeout_start >=1)
			{
				/*y_start=0;
			y_end=320;
			for(i=0;i<320;i=i+8)
			{
				draw_grayscale(i,i+2);

			}*/
				//PORT_AWOP0_4 = 1;

				draw_grayscale();
				//PORT_AWOP0_4 = 0;
			}
#endif
#endif
			//			proveout_routine();
			/************************************************/
			//			All_Telltale_ON();
			//			GI_CONTROL_FLAG=1;
			/************************************************/
			//	Illumination_Proveout();
		}
		//Read_UDS_EEPROM_Data();
	}
	/**/
#if 0

	if(Read_odo_flag == 0)
	{
		Init_read_parameters();
	}
#endif

	/**/
#if 0
#if AFE
	AFE_3_initialize();
#endif
#endif
	//IFE1_initialize();
	IVN_SecurityAlgorithm_initialize();
	//main_atf(0);/*comment to disable ISM*/
	if(IGN_ON_flag	== SET)
	{
		/*Turn ON Illumination*/
		/*Turn ON ALL Tell Tale*/
		/*Output_shift_Register() */
		/*Stepper_InIt()*/
		/*Clear Battery ON flag*/
		/*RESET Watchdog*/
		AFE_3_U.U8_IgnMode=1;
		IGN_ON_flag = 1;
		IGN_ON_for_fuel = 1;
		go_to_sleep = 0;		// clear go to sleep flag when ignition on

#ifdef U350
#if 0
		if((R_GPIO_PinRead(17,5) == 0)&&(key_press_bfr_ign == 0))//Set key press from IGN Off--> IGN On
		{
			key_press_bfr_ign = 1;
			NVM_SERVICE_DONE_flag = 1;
			eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);
			DID_NVM_SERVICE_DONE_0x04F3_arr[0] = eeprom_read(DID_04F3_OFFSET);
		}
#endif
		if((R_GPIO_PinRead(17,4) == 0)&&(key_press_bfr_ign == 0))//Mode Key press from IGN Off--> IGN On
		{

			key_press_bfr_ign = 1;
			if(Diag_RW_ETM_enable == 1)
			{
				ETM_Mode_Enter_timer = 1;//ETM_Mode_Enter_timer flag set after Mode Key press from IGN Off--> IGN On for 5 sec
			}
			else
			{
				ETM_Mode_Enter_timer = 0;
			}
		}
#endif
#if 0
		/*OK key press from IGN Off--> IGN On*/
		if(((SWSC_ADC_value >= ADC_COUNT_OK_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_OK_KEY_OFF))&&(key_press_bfr_ign == 0))
		{
			key_press_bfr_ign = 1;
			NVM_SERVICE_DONE_flag = 1;
			eeprom_write(DID_04F3_OFFSET,NVM_SERVICE_DONE_flag);
			DID_NVM_SERVICE_DONE_0x04F3_arr[0] = eeprom_read(DID_04F3_OFFSET);
		}
#endif
#if 1
		/*Menu key press from IGN Off--> IGN On*/
		if((Menu_Pressed==1)&&(key_press_bfr_ign == 0))
		{
			key_press_bfr_ign = 1;
			Odo_reset_8sec_on=1;
		}
#endif
		key_press_bfr_ign = 1;

		for(uint8_t i=0;i<10;i++)
		{
			odometer_reset_flag = eeprom_read(ODO_RST_FLAG_OFFSET);
			odometer_reset_flag_1 = eeprom_read(ODO_RST_FLAG_OFFSET_1);
		}
		if((odometer_reset_flag == 0xff)&&(odometer_reset_flag_1 == 0xff))
		{
			odometer_reset_flag = 0;
			odometer_reset_flag_1 = 0;
			eeprom_writebt(ODO_RST_FLAG_OFFSET, odometer_reset_flag);
			eeprom_writebt(ODO_RST_FLAG_OFFSET_1, odometer_reset_flag_1);
		}

	}

	WatchDog_Clear();

/****************************************************************************/
#if 0
	final_odomter_distance = eeprom_readOdometer();
	old_odometer_distance = final_odomter_distance;
	Init_Drivesum_odo_distance = final_odomter_distance;
	prev_sbrchm_odo = final_odomter_distance;	//store odo value after IGN on
	/*AFE_rst_odo_dist = final_odomter_distance;*/
	final_tripmeter_distance = eeprom_readTrip1_Odometer();
	final_tripmeter_B_distance = eeprom_readTrip2_Odometer();
#endif
/****************************************************************************/
	//	final_ACtripmeter_distance = eeprom_readTrip_AC_Odometer();
	//	display_ACtripmeter_speed = eeprom_readTrip_AC_Odometer();
/****************************************************************************/
	#if 0
	final_ACtripmeter_distance = eeprom_readTrip_AC_Odometer();
	final_ACtripmeter_distance1 = eeprom_readTrip_AC_Odometer1();
	TRIP_AC_OFFSET = eeprom_readTrip_AC_Offset();
#endif
/****************************************************************************/
	eeprom_memread_32bytes(FUEL_AFTER_IGNOFF_OFFSET,FUEL_IN_IGN.u8, 4);
	fuel_in_ign_on = FUEL_IN_IGN.u32;
	DID_NVM_SERVICE_DONE_0x04F3_arr[0] = eeprom_read(DID_04F3_OFFSET);

/****************************************************************************/
	Init_2WD_odo_distance = final_odomter_distance;//reset 2WD on IGN

/****************************************************************************/
#if 0
/*check for value greater than 9999km*/
	if(final_odomter_distance == 0)
	{
		if(final_ACtripmeter_distance1 == 65535)
		{
			final_ACtripmeter_distance1 = 0;
			eeprom_write_Trip_AC_Odometer1(final_ACtripmeter_distance1);//write 0 if value is 0xFFFF
		}
		if(TRIP_AC_OFFSET == 65535)
		{
			TRIP_AC_OFFSET = 0;
			eeprom_write_Trip_AC_Offset(TRIP_AC_OFFSET);//write 0 if value is 0xFFFF
		}
	}
#endif
/****************************************************************************/
#if 0
	Trip_AC = ((final_odomter_distance-(final_ACtripmeter_distance1 - TRIP_AC_OFFSET))/1000)/*%100000*/;
	if(Trip_AC > 9999)
	{
		Trip_AC = 0;
	}
#endif
/***********************************************/
//	Present_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET); /* read fuel bar from memory & store in Present_level */
//	displayed_fuel_level = eeprom_read(FUEL_BAR_OFFSET); /* read  last fuel bar which is displayed on LCD & store in "displayed_fuel_level"  */

	/*Trip A,B,AC reset on battery disconnection*/
#if 0
	if(BATT_DISCONNECTION == 1)
	{
		reset_tripmeterA();
		reset_tripmeterB();
		//AC_RESET
		Trip_AC_Rollover = 1;
		reset_AC_tripmeter();
		//reset RTC after battery disconnect
		hrs = 0;
		min = 0;
		sec = 0;
		TimeF_selected = 2;
		eeprom_write(rtc_hrs,0);
		eeprom_write(rtc_min,0);
		eeprom_write(rtc_sec,0);
		eeprom_write(SETTING_TIMEFRMT_OFFSET,2);
	}
	else
	{
		//do_nothing
	}
#endif

	/* Read AFE DATA */
	AccFuelConsp = eeprom_read_AFE_data(AFE_ACC_FUEL_CONSP_OFFSET);
	if(AccFuelConsp == 0xFFFFFFFF)
	{
		AccFuelConsp = 0;
	}
	AccFuelConspGD = eeprom_read_AFE_data(AFE_ACC_FUEL_CONSP_GD_OFFSET);
	if(AccFuelConspGD == 0xFFFFFFFF)
	{
		AccFuelConspGD = 0;
	}
	OdoDispReset = eeprom_read_AFE_data(AFE_ODO_DISP_RESET_OFFSET);          /* '<Root>/U32_OdoDispReset' */
	if(OdoDispReset == 0xFFFFFFFF)
	{
		OdoDispReset = 0;
	}
	OdoDispGDReset = eeprom_read_AFE_data(AFE_ODO_DISP_RESET_GD_OFFSET);
	if(OdoDispGDReset == 0xFFFFFFFF)
	{
		OdoDispGDReset = 0;
	}
#if 0
	eeprom_memread_32bytes(AFE_DISP_OFFSET,AFEDispVal_EEPROM.u8, 2);
	AFEDispVal_init = AFEDispVal_EEPROM.u16;
	if(AFEDispVal_init == 0xFFFF)
	{
		AFEDispVal_init = 0;
	}
	AFE_3_Y.U16_AFEDispVal=AFEDispVal_init;
#endif
#if 0
	eeprom_memread_32bytes(AFE_DISP_OFFSET,AFEDispVal_EEPROM.u8, 2);
				AFE_3_U.U16_AFE_LastDispVal_EE = AFEDispVal_EEPROM.u16;
#endif
	// Reading ALL the DTE values as per Sprint 4 new requirements changed at IGN ON
		// 268 bytes data
		    eeprom_memread_32bytes(APP_B_EEPROM_DTE_1km_Compn_Flag,&rtU.B_EEPROM_DTE_1km_Compn_Flag, 1);
			eeprom_memread_32bytes(APP_U8_EE_DTE_RAFE_Inst_1km_Cnt,&rtU.U8_EE_DTE_RAFE_Inst_1km_Cnt, 1);
			eeprom_memread_32bytes(APP_S_EE_DTE_Iddle_FC_Acc,&rtU.U32_EE_DTE_Iddle_FC_Acc, 4);
			eeprom_memread_32bytes(APP_U32_EE_DTE_PrevOdoDisp,&rtU.U32_EE_DTE_PrevOdoDisp, 4);
			eeprom_memread_32bytes(APP_B_EE_Odo_500m_Flag,&rtU.B_EE_Odo_500m_Flag, 1);
#if 0
			eeprom_memread_32bytes(APP_S_EE_Last_Caltd_DTE,&rtU.S_EE_Last_Caltd_DTE, 4);
#endif
			eeprom_memread_32bytes(APP_U32_EE_500mRoll_FC_Acc,&rtU.U32_EE_500mRoll_FC_Acc, 4);
			eeprom_memread_32bytes(APP_U32_EE_DTE_FC_Acc_1st_500m,&rtU.U32_EE_DTE_FC_Acc_1st_500m, 4);
			eeprom_memread_32bytes(APP_U32_EE_DTE_FC_Acc_2nd_500m,&rtU.U32_EE_DTE_FC_Acc_2nd_500m, 4);
			eeprom_memread_32bytes(APP_B_EE_shift_RAFE_Roll,&rtU.B_EE_shift_RAFE_Roll, 1);
			eeprom_memread_32bytes(RESET_DTE_OFFSET, &U8_Reset_DTE, 1);
			/*Read AFE_GD_disp value, AFE and AFE_GD disp update flag After IGN on */
			eeprom_memread_32bytes(AVG_KMPL_GD_AFE_OFFSET,&AFE_3_U.U16_AFE_GD_LastDispVal_EE,2);
			eeprom_memread_32bytes(DISP_UPDATED_AFE_OFFSET,&AFE_3_U.B_DispUpdated_Flag_EE,1);
			eeprom_memread_32bytes(DISP_UPDATED_GD_AFE_OFFSET,&AFE_3_U.B_DispUpdated_GD_Flag_EE,1);
			eeprom_memread_32bytes(AFE_1km_DIST_FLAG_OFFSET,&AFE_3_U.B_1Km_Dist_Flag_EE,1);
			eeprom_memread_32bytes(AFE_1km_DIST_FLAG_GD_OFFSET,&AFE_3_U.B_1Km_Dist_Flag_GD_EE,1);
			uint32_t init_odometer=final_odomter_distance;
			//			rtU.S_EE_Last_Caltd_DTE;  // Last odometer value read from eeprom IGN ON
			//check if Values are greater than 255 round it up
			for(int i=0;i<60;i++)
			{
				uint8_t result;
				eeprom_memread_32bytes(APP_S_EEPROM_RAFE_Read+i,&result,1); // read from offset

				if(result!=0 && result!=0xFF)
					rtU.S_EEPROM_RAFE_Read[i]=(float)result/(float)10;
				else
					rtU.S_EEPROM_RAFE_Read[i]=(float)14;
			}
			// Check Prev odo display flag condition
			if(rtU.U32_EE_DTE_PrevOdoDisp==0)
			{
				rtU.U32_EE_DTE_PrevOdoDisp=final_odomter_distance;
				rtDW.u32_Fuel_PrevOdoDisp = final_odomter_distance;
			}
			// check if vehicle is fast moving condition
			if(vehicle_moving_state == 1)  //vehicle moving status
			{
				rtU.B_DTE_Fast_Flng_Mode=0;
			}
			flag_DTE_Enable=SET;

			// Read DTE value here at IGN ON
			eeprom_memread_32bytes(DTE_DISP_OFFSET,DTEDispVal_EEPROM.u8, 2);
			Display_DTE = DTEDispVal_EEPROM.u16;
			if(Display_DTE == 0xFFFF)
			{
				Display_DTE = 0;
			}

			/*Crevavi*/
			eeprom_memread_32bytes(PREVIOUS_ALERT_CONDITION_TRANSFER_MODE,&Previous_Alert_Condition_Transfer_Mode, 1);	//AVS A counter

#if 0
			/****AVS counter****/
			eeprom_memread_32bytes(AVS_SEC_OFFSET_A,AVS_sec_count_A.u8, 4);	//AVS A counter

			eeprom_memread_32bytes(AVS_SEC_OFFSET_B,AVS_sec_count_B.u8, 4);	//AVS B counter

			eeprom_memread_32bytes(DT_SEC_OFFSET_A,DT_sec_count_A.u8, 4);	//DT A counter

			eeprom_memread_32bytes(DT_SEC_OFFSET_B,DT_sec_count_B.u8, 4);	//DT B counter
#endif
			/********/

			Init_Drivesum_DT_sec_count = DT_sec_count_A.u32;
			/**************** Setting Parameter ***************/
	AFE_selected = eeprom_read(SETTING_AFE_OFFSET); 		/* read AFE_selected */
	if((AFE_selected > 3)||(AFE_selected == 0))
	{
		AFE_selected = 1;									/*default - user Reset*/
	}
	if(AFE_selected == 2)									/*AFE reset condition for IGN Cycle option*/
	{
		if(AFE_RST_Time>= AFE_DRIVE_CYCLE_RESET_INTERVAL*60)//reset AFE if IGN if off for AFE_DRIVE_CYCLE_RESET_INTERVAL
		{
			Flag_AFE_reset = 1;								//flag to reset AFE
		}
	}
	TimeF_selected = eeprom_read(SETTING_TIMEFRMT_OFFSET); /* read TimeF_selected*/
	if((TimeF_selected > 2)||(TimeF_selected == 0))
	{
		TimeF_selected = 1;								   /*default - 12Hr*/
	}
/****************************************************************************************/
#if 0
	TempU_selected = eeprom_read(SETTING_TEMPUNIT_OFFSET); /* read TempU_selected*/
	if(TempU_selected > 1)
	{
		TempU_selected = 0;								   /*default - degC*/
	}
#endif
/***************************************************************************************/
	PAB_selected = eeprom_read(SETTING_PAB_OFFSET); /* read PAB_selected*/
	if(PAB_selected > 1)
	{
		PAB_selected = 1;								   /*default - 1*/
	}
/***************************************************/
	if((AccFuelConsp == 0xFFFFFFFF)&&(AccFuelConspGD == 0xFFFFFFFF)&&(OdoDispReset == 0xFFFFFFFF)&&(OdoDispGDReset == 0xFFFFFFFF))
	{
		AccFuelConsp = 0;
		AccFuelConspGD = 0;
		OdoDispReset = 0;          /* '<Root>/U32_OdoDispReset' */
		OdoDispGDReset = 0;
		AFEDispVal = 0;

		eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_OFFSET,AccFuelConsp);
		eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_GD_OFFSET,AccFuelConspGD);
		eeprom_write_AFE_data(AFE_ODO_DISP_RESET_OFFSET,OdoDispReset);
		eeprom_write_AFE_data(AFE_ODO_DISP_RESET_GD_OFFSET,OdoDispGDReset);
		eeprom_write_AFE_data(AFE_DISP_OFFSET,AFEDispVal);
	}

	AFE_3_U.U32_AccFuelConspEE = AccFuelConsp;         /* '<Root>/U32_AccFuelConspEE' */
	AFE_3_U.U32_AccFuelConspGDEE= AccFuelConspGD;       /* '<Root>/U32_AccFuelConspGDEE' */
	AFE_3_U.U32_OdoDispResetEE= OdoDispReset;         /* '<Root>/U32_OdoDispResetEE' */
	AFE_3_U.U32_OdoDispGDResetEE= OdoDispGDReset;       /* '<Root>/U32_OdoDispGDResetEE' */
	/*AFE_3_Y.U16_AFEDispVal = AFEDispVal;*/
	AFE_start = 1;
	/* Read AFE DATA end*/
	//Read IVN FC data
	IVN_EEFreshnessCounter= eeprom_read_IVN_FC_data(IVN_FC_COUNTER_OFFSET);
	if(IVN_EEFreshnessCounter == 0xFFFFFFFF)
	{
		IVN_EEFreshnessCounter=0;
		eeprom_write_IVN_FC_data(IVN_FC_COUNTER_OFFSET,IVN_EEFreshnessCounter);
	}

	/*Added to check EEPROM status*/
	//R_GPIO_PinDirection(3,0,0);
	//R_DEV_PinInit (ICC_PinConfigSDA_out);
	//R_GPIO_PinDirection(3,1,0);
	I2C_Init();
#if 0
	updateAppVersion();  						 //Update App version to EEPROM
	updateHWVersion();  						 //Update HW version to EEPROM
#endif
	//delay_time(6000);
	//EEPROM_init();

	//R_TAU0_Create();  /*call timer 100us */

	if(IGN_ON_flag == CLEAR)
	{
		Bootup_Trigger = 0;	/*Boot up animation not start - ghost issue solved*/

		Ign_Off_Counter =SET; /* This flag SET for perform operation after IGN off condition. Counter SET value set by According Requirement*/

		Update_ign_Flag=SET;  /*This flag SET for perform operation after IGN off condition.*/

		IGN_ON_flag = 0;
		AFE_3_U.U8_IgnMode=0;
		WatchDog_Clear();
	}
/*	Read_UDS_EEPROM_Data();
 - 	Dtc_init();
 - 	Read_DTC_Status();*/
	// DIAG Telltale flags
	Write_Default_Data();

#if 0
	SendDCData(0xA5C, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1); /* reset command*/
#ifdef PCBrevB
#else
	SendDCData(0xA5C, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1); /* reset command*/
	cascade_TT_send_data();

#endif
	diagTTflag=0;
	gcsk_flag=0;
	/////////////
	SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
#ifdef PCBrevB
	Bit_flag_Access();

	R_GPIO_WritePin(17, 2, 1);/*Gauge pointer illumination*/
	R_GPIO_WritePin(16, 11, 1);/*GI_CONTROL*/

	if(FEATURE_CODE_FUEL_TYPE == 1)
	{
		/* Diesel*/
		R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
		R_GPIO_WritePin(17, 1, 1);/*GI_CONTROL2*/

	}
	else
	{
		/* GAsoline*/
		R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
		R_GPIO_WritePin(17, 0, 1);/*GI_CONTROL1*/
	}

#else
	SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1); /* Send FCBCDC data*/
	cascade_TT_send_data();
#endif
#endif
	Bit_flag_Access();


	if(IGN_ON_flag==SET)
	{
		//GI_Control();
		//All_Telltale_ON();
#ifdef PCBrevB
		SendDCData(0x535,0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
		SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
		ReadDCData();

#else

		//GI_Control();
#endif
	}
	/*Start service reminder off from here*/

	/*Stop service reminder off from here*/
	while(1)
	{
		WatchDog_Clear();

		eeRtc_Processing();
		digi_clk_can();
		if((IGN == 1)&&(INIT_DONE_flag == 1))
		{
			IGN_ON_flag =  1;
		}
		else
		{
			IGN_ON_flag = 0;
		}
		//	Battery_Condition();
		//IGN_mode();//call each 1000ms
#ifdef COLOR_TFT
//		my_can0_rx();
#endif
		//	my_can0_tx();
		//rx_flag = 0;
#if 0
		if(LVSD_flag ==0 && HVSD_flag == 0 && Read_TT_flag == SET && IGN_ON_flag==1) //call each 500ms
		{
			//input_shift_register();
			//	scan_TT_status();
			//	DIAG_TT_init();
			//output_shift_register();
		}
#endif
		if(LVSD_flag ==1 || HVSD_flag ==1)// call each 1000ms
		{
			LVSD_Clear_init=0;
			if(LVSD_SET_init == 0)
			{
				LVSD_SET_init=1;
				r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);//disable chime
				High_Beam_sts = 1;
				//All_Telltale_OFF();
#ifdef U350
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
#endif
#ifdef GRAYSCALE_TFT
				ret_redraw = 0;			/*before backlight off restrict to draw on TFT*/
				clear_screen();
				r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);
				PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
				PORT_ISOPM17_3 = 0;
				PORT_ISOP17_3 = 0;

				//For clear screen in Over voltage
/*
				for(uint8_t i = 0; i < 0xffff; i++);
	             PORT_ISOP44_5 = 0;
	            for(uint8_t i = 0; i < 0xffff; i++);
	             PORT_ISOP44_5 = 1;

	             Lcbi_WriteControl( 0x11);
	             Lcbi_WriteData8(0xA5);
*/
#endif
#ifdef COLOR_TFT
				R_GPIO_WritePin(17,3, 0);
				r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);
				PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at high voltage*/
				PORT_ISOPM17_3 = 0;
				PORT_ISOP17_3 = 0;
#endif


#ifdef PCBrevB
				R_GPIO_WritePin(17, 2, 0);/*Gauge pointer illumination*/
				R_GPIO_WritePin(16, 11, 0);/*GI_CONTROL*/
				R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
				R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
#else
				All_Telltale_OFF();
#endif
#ifdef GRAYSCALE_TFT
				while((LVSD_flag_1 == 1)||(HVSD_flag_1 == 1))
				{
					clear_screen();
					WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR

					if(IGN ==0)
					{
						break;
					}
				}
#endif
			}
		}
		else if((LVSD_flag == 0 && HVSD_flag ==0)&&(IGN==1))// call each 1000ms
		{
			LVSD_SET_init=0;
			if(LVSD_Clear_init == 0)
			{
				LVSD_Clear_init=1;
				//R_GPIO_WritePin(17,3, 1);

				PORT_ISOPMC17_3 = 1;      /*to solve TFT backlight ON at IGN OFF*/
				PORT_ISOPM17_3 = 0;
				PORT_ISOP17_3 = 0;
				 R_TAU2_PWM_TFT();
				 r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
				 tst_tt_flag_1=0;
				 tst_tt_flag_2=0;
				 tst_tt_flag_3=0;
				 tst_tt_flag_4=0;
				 tst_tt_flag_5=0;
				 tst_tt_flag_6=0;
				 tst_tt_flag_7=0;
				 tst_tt_flag_8=0;
				 tst_tt_flag_9=0;
				 tst_tt_flag_10=0;
				 tst_tt_flag=0;

#ifdef PCBrevB
				R_GPIO_WritePin(17, 2, 1);/*Gauge pointer illumination*/
				R_GPIO_WritePin(16, 11, 1);/*GI_CONTROL*/
				//	R_GPIO_WritePin(17, 0, 1);/*GI_CONTROL1*/
				//	R_GPIO_WritePin(17, 1, 1);/*GI_CONTROL2*/

				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
					R_GPIO_WritePin(17, 1, 1);/*GI_CONTROL2*/

				}
				else
				{
					/* GAsoline*/
					R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
					R_GPIO_WritePin(17, 0, 1);/*GI_CONTROL1*/
				}
#else

#endif
			}
		}

		//SWSC_Condition();// call each 100ms
		if(IGN_ON_flag == CLEAR)
		{
			EEPROM_readwrite_para_started_on =0;
#ifdef COLOR_TFT
			RPAS_LV1_COUNTER = 0;
			RPAS_LV2_COUNTER = 0;
			RPAS_LV3_COUNTER = 0;
			RPAS_LV4_COUNTER = 0;

			W501_RPAS_LEVEL_1_BUZ=0;
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

			SoundPCM_Seatbelt_Lv0cont_STOP();
#endif
#ifdef GRAYSCALE_TFT
#ifndef U350
			W501_LOW_RPAS_LEVEL1_PRECON = 0;
			W501_LOW_RPAS_COUNTER2 = 0;
			W501_LOW_RPAS_COUNTER3 = 0;
			W501_LOW_RPAS_COUNTER4 = 0;
			/*Stop RPAS DMA*/
			SoundPCM_RPAS_buzz_LV1_STOP();
			SoundPCM_RPAS_buzz_LV2_STOP();
			SoundPCM_RPAS_buzz_LV3_STOP();
			SoundPCM_RPAS_buzz_LV4_STOP();
#endif
#ifdef U350
			if(Sound_Bit == 0x10000)
			{
				RPAS_LEVEL1_PRECON = 0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV1_STOP();
			}
			if(Sound_Bit == 0x20000)
			{
				RPAS_LEVEL2_BUZ_ONGOING=0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV2_STOP();
			}
			if(Sound_Bit == 0x30000)
			{
				RPAS_LEVEL3_BUZ_ONGOING=0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV3_STOP();
			}
			if(Sound_Bit == 0x40000)
			{
				RPAS_LEVEL4_BUZ_ONGOING=0;
				Sound_Bit = 0x0;
				SoundPCM_RPAS_buzz_LV4_STOP();
			}
#endif
#endif
			Sound_Bit = 0x0;
			Door_open_animation_parareset();		/*Door open animation parameter reset some time car body not displaying related issue solved*/
			All_Telltale_OFF();
			IGN_OFF_TT=1;
			display_speedometer_speed=0;
			display_tachometer_speed=0;
			final_vechile_speed_gauge_raw=0;
			loc_Ism_TC0004();/*RPM and Speedo movement*/
			if(startkey == 0)
			{
				while((ISM0VAR4CFG0 > 10000) && (ISM0VAR4CFG1 > 10000));
			}
#ifdef GRAYSCALE_TFT
			r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);	//disable buzzer at IGN off
#endif


#if 0
			if(TURN_TT_TIMEOUT==0)
			{


#ifdef PCBrevB
			R_GPIO_WritePin(1, 3, 0); /* 5V_SWSC enable */

			R_GPIO_WritePin(10, 5, 0); /* TT_DRIVER */
#else
			R_GPIO_WritePin(3, 9, 0); /* 5V_SWSC enable */
			//R_GPIO_WritePin(10, 10, 1); /* CE for input shift reg*/

			R_GPIO_WritePin(3, 8, 0); /* TT_DRIVER */
#endif
			R_GPIO_WritePin(42, 12, 1);//parallel to serial CE
			}
#endif

			/*************************************************/
			if(wakeup_factor_flag == WAKEUP_IGN)
			{
				refresh_rate = REFRESH_RATE_FAST;
				flag_grayscale = refresh_rate - 3;
//				ret_redraw = 1;
				handleDromeState();	/*function renaming*/
				handleDromeState();	/*function renaming*/
				handleDromeState();	/*function renaming*/
				Drive_summery();
				write_eeprom_flag = 1;
				Timer_ON_13sec_ign = 1;
				if(start_driveSum_timeout_3sec == 0)
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Drive_Summary);
					dont_throw_alerts_alert_on_screen_present=1;
					drive_summery_start = 1;
				}

				/*dynamic state call*/
				/*display parameter to be update*/
				do
				{
					WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
#ifdef GRAYSCALE_TFT
					if(drive_sum_flag == 0)
					{
						drive_sum_flag = 1;
						flag_grayscale = refresh_rate - 3;
//						HANDLE_DROME_START = 0;
						entry_drivesummery = 1;
					}
					else
					{
//						entry_drivesummery = 0;
					}
#endif
					handleDromeState();	/*function renaming*/
					handleDromeState();	/*function renaming*/
					R_TAU1_Create();
					/*For IGN off to on fuel Change*/
					Timer_ON_100msec = 1;//calculate fuel ADC after every 100ms
					ADC_Avg();//need to check impact
					/******************************/
					/***************************/
					display_key  = drome_getCurrentViewKey();
					if(((display_key == 0x22039)||(display_key == 0x22040))/*&&(parameter_writeflag == 0)&&(write_eeprom_flag == 1)*/)
					{
						handleDromeState();	/*function renaming*/
					}
#ifdef GRAYSCALE_TFT
					if((parameter_writeflag == 0)&&(write_eeprom_flag == 1)&&(drive_summery_page_draw == 1))
#else
						if((parameter_writeflag == 0)&&(write_eeprom_flag == 1))
#endif
						{
							//EEPROM_readwrite_para_started = 1;
							Vin_Clear_Variables();//to be called at IGN off

							eeprom_writeOdometer(final_odomter_distance);
							eeprom_write_Trip_AC_Odometer(final_ACtripmeter_distance);
							eeprom_write_Trip_AC_Odometer1(final_ACtripmeter_distance1);
							eeprom_write_Trip_AC_Offset(TRIP_AC_OFFSET);
							eeprom_write(FAST_FILL_ENTRY_OFFSET, Fast_fill_Started);
							WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
							All_DTC_Write();/*Write DTC at IGN off*/
							handleDromeState();	/*function renaming*/
							//			eeprom_write_Trip_AC_Odometer(display_ACtripmeter_speed);//Store AC trip value after IGN off
							//AFE DATA Write
							eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_OFFSET,AccFuelConsp);
							eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_GD_OFFSET,AccFuelConspGD);
							eeprom_write_AFE_data(AFE_ODO_DISP_RESET_OFFSET,OdoDispReset);
							eeprom_write_AFE_data(AFE_ODO_DISP_RESET_GD_OFFSET,OdoDispGDReset);
							/*****/
							eeprom_write(ILLUM_NVM_OFFSET, currIllumination); /* store displayed_fuel_level in memory */

#if 0
							if(NotdisplayAFE == 1)
							{
								AFEDispVal_EEPROM.u16 = 0;
								New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);
							}
							else if(AFE_calstart == 1)
							{
								AFEDispVal_EEPROM.u16 = AFEDispVal;
								New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);
							}
#endif
							/*eeprom_write_AFE_data(AFE_DISP_OFFSET,AFEDispVal);*/
							/*****/
							//AFE DATA Write end
							//Write IVN FC Counter
							WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR

							// Write all the DTE related 268 bytes of data as per new Sprint 4 req
							// Write DTE Avg Data Values at IGN off here
							// Write all the DTE related 268 bytes of data as per new Sprint 4 req
							if(ModelRunCheck_flag == 1)
							{
								New_eeprom_memwrite_32bytes(APP_B_EEPROM_DTE_1km_Compn_Flag, &rtY.S_DTE_1km_Completion_Flag, 1);
								New_eeprom_memwrite_32bytes(APP_U8_EE_DTE_RAFE_Inst_1km_Cnt,&rtY.U8_DTE_RAFE_Inst_1km_Cnt , 1);
								//New_eeprom_memwrite_32bytes(APP_S_EE_DTE_Iddle_FC_Acc,&rtY.S_Idle_DTE_FC_Acc , 4);
								New_eeprom_memwrite_32bytes(APP_S_EE_DTE_Iddle_FC_Acc,&rtY.U32_Idle_DTE_FC_Acc , 4);
								New_eeprom_memwrite_32bytes(APP_U32_EE_DTE_PrevOdoDisp,&rtY.U32_DTE_Prev_Odo , 4);
								New_eeprom_memwrite_32bytes(APP_B_EE_Odo_500m_Flag,&rtY.B_Odo_500m_Flag , 1);
								//New_eeprom_memwrite_32bytes(APP_S_EE_Last_Caltd_DTE,&rtY.S_Last_Caltd_DTE , 4);
								New_eeprom_memwrite_32bytes(APP_S_EE_Last_Caltd_DTE,&rtY.S_DTE_Disp_Val , 4);
								New_eeprom_memwrite_32bytes(APP_U32_EE_500mRoll_FC_Acc,&rtY.U32_O_500mRoll_FC_Acc, 4);
								New_eeprom_memwrite_32bytes(APP_U32_EE_DTE_FC_Acc_1st_500m,&rtY.U32_O_DTE_FC_Acc_1st_500m , 4);
								New_eeprom_memwrite_32bytes(APP_U32_EE_DTE_FC_Acc_2nd_500m,&rtY.U32_O_DTE_FC_Acc_2nd_500m , 4);
								New_eeprom_memwrite_32bytes(APP_B_EE_shift_RAFE_Roll,&rtY.B_DTE_RAFE_Shift,1);
								New_eeprom_memwrite_32bytes(RESET_DTE_OFFSET, &U8_Reset_DTE, 1);
								New_eeprom_memwrite_32bytes(AVG_KMPL_GD_AFE_OFFSET, &AFE_3_Y.U16_AvgKmplAFE_GD, 2);
								New_eeprom_memwrite_32bytes(DISP_UPDATED_AFE_OFFSET, &AFE_3_Y.B_DispUpdated_Flag, 1);
								New_eeprom_memwrite_32bytes(DISP_UPDATED_GD_AFE_OFFSET, &AFE_3_Y.B_DispUpdated_GD_Flag, 1);
								New_eeprom_memwrite_32bytes(AFE_1km_DIST_FLAG_OFFSET, &AFE_3_Y.B_1Km_Dist_Flag, 1);
								New_eeprom_memwrite_32bytes(AFE_1km_DIST_FLAG_GD_OFFSET, &AFE_3_Y.B_1Km_Dist_Flag_GD, 1);

								AFEDispVal_EEPROM.u16 = AFEDispVal;
								New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);

								//check if Values are greater than 255 round it up
								for(int i=0;i<60;i++)
								{
									uint8_t result;
									uint16_t temp;
									temp=rtY.S_DTE_RAFE_Inst_1km_Roll[i]*10;
									if(temp>255)
										result=254;
									else
										result=temp;
									New_eeprom_memwrite_32bytes(APP_S_EEPROM_RAFE_Read+i,&result,1);
								}
								// Write DTE Data Values at IGN off here
								DTEDispVal_EEPROM.u16 = Display_DTE;
								New_eeprom_memwrite_32bytes(DTE_DISP_OFFSET, DTEDispVal_EEPROM.u8, 2);
								flag_DTE_Enable=CLEAR;
							}
							eeprom_write_IVN_FC_data(IVN_FC_COUNTER_OFFSET,IVN_EEFreshnessCounter);

							New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_A, AVS_sec_count_A.u8, 4);	//AVS time counter;
							New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_B, AVS_sec_count_B.u8, 4);	//AVS time counter B;
							New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_A, DT_sec_count_A.u8, 4);		//AVS time counter B;
							New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_B, DT_sec_count_B.u8, 4);		//AVS time counter B;

							New_eeprom_memwrite_32bytes(FUEL_AFTER_IGNOFF_OFFSET,FUEL_IN_IGN.u8,4);	//Store fuel level after IGN off

							Ign_Off_Counter =SET; /* This flag SET for perform operation after IGN off condition. Counter SET value set by According Requirement*/
							Update_ign_Flag=SET;  /*This flag SET for perform operation after IGN off condition.*/
							IGN_ON_flag = 0;

							/*Crevavi*/
							New_eeprom_memwrite_32bytes(PREVIOUS_ALERT_CONDITION_TRANSFER_MODE, &Previous_Alert_Condition_Transfer_Mode, 1);	//AVS time counter;


							eeprom_write(IGN_TOGGLE_FLAG, 0x55); /* store IGN_Toggle_flag in eeprom */
							WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
							if(Fuel_init_done == 1)
							{
								displayed_fuel_level = Current_Fuel_Bar();
								eeprom_write(FUEL_BAR_OFFSET, displayed_fuel_level); /* store displayed_fuel_level in memory */
								eeprom_memwrite_32bytes(Filter_ADC_Count_OFFSET,&New_fuel_read_ad1,2);
							}
							else
							{
								//no task
							}
							/*4WD status*/
							STS_4WD = (CAN_IC2_100[3].byte & 0x60);
							eeprom_write(STS_4WD_OFFSET,STS_4WD);
							/*************/
							eeprom_write(SETTING_AFE_OFFSET, AFE_selected);        /* store AFE selected setting in EEPROM */
							eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected); /* store TimeF_selected in EEPROM */
							eeprom_write(SETTING_TEMPUNIT_OFFSET, TempU_selected); /* store TempU_selected in EEPROM */
							eeprom_write(SETTING_PAB_OFFSET, PAB_selected); /* store TempU_selected in EEPROM */

							AFE_RST_Time_IGN_off = ((disp_hour*60)+disp_min);
							New_eeprom_memwrite_32bytes(IGN_OFF_Time_OFFSET,&AFE_RST_Time_IGN_off,2);

							New_eeprom_memwrite_32bytes(DEFAULT_PAGE_OFFSET, Default_pagekey.u8, 4);	//Default page;

							if((Dooropen_active_IGNOFF == 1)&&(All_doorclose != 1))
							{
								IGNoff_status.byte = 0x01;					/*ON WAKE UP can BUS OFF & DOOR STATUS IS OPEN THEN STORE DOOR STATUS*/
								New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
							}
							else if(Dooropen_active_IGNOFF != 1)
							{
								IGNoff_status.byte = 0x00;
								New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
							}
							Prev_2WD_odo_distance = Final_Drivesum_2WD_distance;
							New_eeprom_memwrite_32bytes(FOURWD_NOTENGAGED_OFFSET,&Prev_2WD_odo_distance,4);/*Drive summary 4x4 logic changed as per CR (prev 4wd not engaged value saved in eeprom)- 26-05-20 for W501Hv232*/
//							eeprom_write(LIGHT_ONOFFSTATUS_OFFSET, sw_press_ok);        /* LIGHT ON-OFF STATUS TO RETAIN THE STATUS AFTER IGN ON -> OFF */
							Timer_ON_100msec = 0; /* from IGN ON condition */
							parameter_writeflag = 1;
							write_eeprom_flag = 0;
//							drive_summery_page_draw = 0;
						}

					EEPROM_readwrite_para_started = 0;
					if((Drive_summery_tout_flag1 == 1)&&(display_key == 0x11108))
					{
						Drive_summery_tout_flag = 1;
						handleDromeState();
						handleDromeState();
					}
					/**************************/
				}while((Drive_summery_tout_flag != 1)&&(IGN == 0)&&(Graphics_Loaded==1));
				EEPROM_readwrite_para_started = 0;

#ifdef GRAYSCALE_TFT
				if((parameter_writeflag == 0)&&(write_eeprom_flag == 1)&&(drive_summery_page_draw == 0))	/*Protection added for AFE showing 30 issue when fast IGN OFF - ON it skips drive summery due to that all parameter skips data writing - dated 18-07-20*/
				{																							/*In that condition drive_summery_page_draw not set to 1*/						
					//EEPROM_readwrite_para_started = 1;
					Vin_Clear_Variables();//to be called at IGN off

					eeprom_writeOdometer(final_odomter_distance);
					eeprom_write_Trip_AC_Odometer(final_ACtripmeter_distance);
					eeprom_write_Trip_AC_Odometer1(final_ACtripmeter_distance1);
					eeprom_write_Trip_AC_Offset(TRIP_AC_OFFSET);
					eeprom_write(FAST_FILL_ENTRY_OFFSET, Fast_fill_Started);
					WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
					All_DTC_Write();/*Write DTC at IGN off*/
					handleDromeState();	/*function renaming*/
					//			eeprom_write_Trip_AC_Odometer(display_ACtripmeter_speed);//Store AC trip value after IGN off
					//AFE DATA Write
					eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_OFFSET,AccFuelConsp);
					eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_GD_OFFSET,AccFuelConspGD);
					eeprom_write_AFE_data(AFE_ODO_DISP_RESET_OFFSET,OdoDispReset);
					eeprom_write_AFE_data(AFE_ODO_DISP_RESET_GD_OFFSET,OdoDispGDReset);
					/*****/
					eeprom_write(ILLUM_NVM_OFFSET, currIllumination); /* store displayed_fuel_level in memory */

#if 0
					if(NotdisplayAFE == 1)
					{
						AFEDispVal_EEPROM.u16 = 0;
						New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);
					}
					else if(AFE_calstart == 1)
					{
						AFEDispVal_EEPROM.u16 = AFEDispVal;
						New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);
					}
#endif
					/*eeprom_write_AFE_data(AFE_DISP_OFFSET,AFEDispVal);*/
					/*****/
					//AFE DATA Write end
					//Write IVN FC Counter
					WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR

					// Write all the DTE related 268 bytes of data as per new Sprint 4 req
					// Write DTE Avg Data Values at IGN off here
					// Write all the DTE related 268 bytes of data as per new Sprint 4 req
					if(ModelRunCheck_flag == 1)
					{
						New_eeprom_memwrite_32bytes(APP_B_EEPROM_DTE_1km_Compn_Flag, &rtY.S_DTE_1km_Completion_Flag, 1);
						New_eeprom_memwrite_32bytes(APP_U8_EE_DTE_RAFE_Inst_1km_Cnt,&rtY.U8_DTE_RAFE_Inst_1km_Cnt , 1);
						//New_eeprom_memwrite_32bytes(APP_S_EE_DTE_Iddle_FC_Acc,&rtY.S_Idle_DTE_FC_Acc , 4);
						New_eeprom_memwrite_32bytes(APP_S_EE_DTE_Iddle_FC_Acc,&rtY.U32_Idle_DTE_FC_Acc , 4);
						New_eeprom_memwrite_32bytes(APP_U32_EE_DTE_PrevOdoDisp,&rtY.U32_DTE_Prev_Odo , 4);
						New_eeprom_memwrite_32bytes(APP_B_EE_Odo_500m_Flag,&rtY.B_Odo_500m_Flag , 1);
						//New_eeprom_memwrite_32bytes(APP_S_EE_Last_Caltd_DTE,&rtY.S_Last_Caltd_DTE , 4);
						New_eeprom_memwrite_32bytes(APP_S_EE_Last_Caltd_DTE,&rtY.S_DTE_Disp_Val , 4);
						New_eeprom_memwrite_32bytes(APP_U32_EE_500mRoll_FC_Acc,&rtY.U32_O_500mRoll_FC_Acc, 4);
						New_eeprom_memwrite_32bytes(APP_U32_EE_DTE_FC_Acc_1st_500m,&rtY.U32_O_DTE_FC_Acc_1st_500m , 4);
						New_eeprom_memwrite_32bytes(APP_U32_EE_DTE_FC_Acc_2nd_500m,&rtY.U32_O_DTE_FC_Acc_2nd_500m , 4);
						New_eeprom_memwrite_32bytes(APP_B_EE_shift_RAFE_Roll,&rtY.B_DTE_RAFE_Shift,1);
						New_eeprom_memwrite_32bytes(RESET_DTE_OFFSET, &U8_Reset_DTE, 1);
						New_eeprom_memwrite_32bytes(AVG_KMPL_GD_AFE_OFFSET, &AFE_3_Y.U16_AvgKmplAFE_GD, 2);
						New_eeprom_memwrite_32bytes(DISP_UPDATED_AFE_OFFSET, &AFE_3_Y.B_DispUpdated_Flag, 1);
						New_eeprom_memwrite_32bytes(DISP_UPDATED_GD_AFE_OFFSET, &AFE_3_Y.B_DispUpdated_GD_Flag, 1);
						New_eeprom_memwrite_32bytes(AFE_1km_DIST_FLAG_OFFSET, &AFE_3_Y.B_1Km_Dist_Flag, 1);
						New_eeprom_memwrite_32bytes(AFE_1km_DIST_FLAG_GD_OFFSET, &AFE_3_Y.B_1Km_Dist_Flag_GD, 1);

						AFEDispVal_EEPROM.u16 = AFEDispVal;
						New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);

						//check if Values are greater than 255 round it up
						for(int i=0;i<60;i++)
						{
							uint8_t result;
							uint16_t temp;
							temp=rtY.S_DTE_RAFE_Inst_1km_Roll[i]*10;
							if(temp>255)
								result=254;
							else
								result=temp;
							New_eeprom_memwrite_32bytes(APP_S_EEPROM_RAFE_Read+i,&result,1);
						}
						// Write DTE Data Values at IGN off here
						DTEDispVal_EEPROM.u16 = Display_DTE;
						New_eeprom_memwrite_32bytes(DTE_DISP_OFFSET, DTEDispVal_EEPROM.u8, 2);
						flag_DTE_Enable=CLEAR;
					}
					eeprom_write_IVN_FC_data(IVN_FC_COUNTER_OFFSET,IVN_EEFreshnessCounter);

					New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_A, AVS_sec_count_A.u8, 4);	//AVS time counter;
					New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_B, AVS_sec_count_B.u8, 4);	//AVS time counter B;
					New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_A, DT_sec_count_A.u8, 4);		//AVS time counter B;
					New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_B, DT_sec_count_B.u8, 4);		//AVS time counter B;

					New_eeprom_memwrite_32bytes(FUEL_AFTER_IGNOFF_OFFSET,FUEL_IN_IGN.u8,4);	//Store fuel level after IGN off

					Ign_Off_Counter =SET; /* This flag SET for perform operation after IGN off condition. Counter SET value set by According Requirement*/
					Update_ign_Flag=SET;  /*This flag SET for perform operation after IGN off condition.*/
					IGN_ON_flag = 0;

					/*Crevavi*/
					New_eeprom_memwrite_32bytes(PREVIOUS_ALERT_CONDITION_TRANSFER_MODE, &Previous_Alert_Condition_Transfer_Mode, 1);	//AVS time counter;


					eeprom_write(IGN_TOGGLE_FLAG, 0x55); /* store IGN_Toggle_flag in eeprom */
					WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
					if(Fuel_init_done == 1)
					{
						displayed_fuel_level = Current_Fuel_Bar();
						eeprom_write(FUEL_BAR_OFFSET, displayed_fuel_level); /* store displayed_fuel_level in memory */
						eeprom_memwrite_32bytes(Filter_ADC_Count_OFFSET,&New_fuel_read_ad1,2);
					}
					else
					{
						//no task
					}
					/*4WD status*/
					STS_4WD = (CAN_IC2_100[3].byte & 0x60);
					eeprom_write(STS_4WD_OFFSET,STS_4WD);
					/*************/
					eeprom_write(SETTING_AFE_OFFSET, AFE_selected);        /* store AFE selected setting in EEPROM */
					eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected); /* store TimeF_selected in EEPROM */
					eeprom_write(SETTING_TEMPUNIT_OFFSET, TempU_selected); /* store TempU_selected in EEPROM */
					eeprom_write(SETTING_PAB_OFFSET, PAB_selected); /* store TempU_selected in EEPROM */

					AFE_RST_Time_IGN_off = ((disp_hour*60)+disp_min);
					New_eeprom_memwrite_32bytes(IGN_OFF_Time_OFFSET,&AFE_RST_Time_IGN_off,2);

					New_eeprom_memwrite_32bytes(DEFAULT_PAGE_OFFSET, Default_pagekey.u8, 4);	//Default page;

					if((Dooropen_active_IGNOFF == 1)&&(All_doorclose != 1))
					{
						IGNoff_status.byte = 0x01;					/*ON WAKE UP can BUS OFF & DOOR STATUS IS OPEN THEN STORE DOOR STATUS*/
						New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
					}
					else if(Dooropen_active_IGNOFF != 1)
					{
						IGNoff_status.byte = 0x00;
						New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
					}
					Prev_2WD_odo_distance = Final_Drivesum_2WD_distance;
					New_eeprom_memwrite_32bytes(FOURWD_NOTENGAGED_OFFSET,&Prev_2WD_odo_distance,4);/*Drive summary 4x4 logic changed as per CR (prev 4wd not engaged value saved in eeprom)- 26-05-20 for W501Hv232*/
//							eeprom_write(LIGHT_ONOFFSTATUS_OFFSET, sw_press_ok);        /* LIGHT ON-OFF STATUS TO RETAIN THE STATUS AFTER IGN ON -> OFF */
					Timer_ON_100msec = 0; /* from IGN ON condition */
					parameter_writeflag = 1;
					write_eeprom_flag = 0;
					drive_summery_page_draw = 0;
				}
#endif

#ifdef COLOR_TFT				
				if(Drive_summery_tout_flag != 1)
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Drive_Sum_Timeout);
					dromecall_timeout = 1;
					do
					{
						handleDromeState();	/*drome call till it will not receive drive summary timeout black screen(startkey = 0x11108)*/
					}while((startkey != 0x11108)&&(dromecall_timeout <= 10000));
					dromecall_timeout = 0;
				}
#endif				
				Drive_summery_tout_flag = 1;
				dont_throw_alerts_alert_on_screen_present=0;

				if(Fuel_init_done == 1)
				{
					/*No need to write present bar immidiate after IGN off hence commented*/
//					New_fuel_read_ad1 = New_fuel_read_avg;
//					Present_level = Current_Fuel_Bar(); /*  New_fuel_read_ad1; */
//					eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level); /* store Present_level in memory */
				}
				if(Drive_summery_tout_flag == 1)
				{
					//eeprom_write(IGN_TOGGLE_FLAG, 0xAA); /* store IGN_Toggle_flag in eeprom */
				}
//				Counter_13sec_ign = 100000;//jump counter after Drive summery.
				Bootup_Trigger = 0;	/*Boot up animation not start - ghost issue solved*/
			}
			else
			{
				Drive_summery_tout_flag = 1;
				Timer_ON_13sec_ign = 0;

			}

			if(IGN == 1)
			{
				go_to_sleep	= 1;
#ifdef COLOR_TFT
				handleDromeState();
				handleDromeState();
				handleDromeState();
#endif
			}

			/**********changes for system state*************/
	//		Timer_ON_13sec_ign = 1;

			go_to_sleep	= 1;
			ret_redraw = 0;
			Not_IGN_Wakeup = 1;

			/***********************************************/

#ifdef GRAYSCALE_TFT
//					r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 0;
					/*TFT off(reset pin low)*/
//					PORT_ISOP44_5 = 0;
#endif
#ifdef COLOR_TFT
//					PORT_ISOP44_3 = 0;
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 0;
#endif

#ifdef GRAYSCALE_TFT
			clear_screen();

#endif
			/******************************************************/



			//PORT_ISOP17_3 = 0; /*TFT Backlight */  /*to solve TFT backlight OFF at IGN OFF*/
#if 0
			r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);

			PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
			PORT_ISOPM17_3 = 0;
			PORT_ISOP17_3 = 0;
			/*TFT off(reset pin low)*/
			PORT_ISOP44_5 = 0;


#ifdef COLOR_TFT
			PORT_ISOP44_3 = 0;
#endif
#endif
#if 0
			if(parameter_writeflag != 1)
			{
			Vin_Clear_Variables();//to be called at IGN off
			eeprom_writeOdometer(final_odomter_distance);
			eeprom_write_Trip_AC_Odometer(final_ACtripmeter_distance);
			eeprom_write_Trip_AC_Odometer1(final_ACtripmeter_distance1);
			eeprom_write_Trip_AC_Offset(TRIP_AC_OFFSET);

			All_DTC_Write();/*Write DTC at IGN off*/

			//			eeprom_write_Trip_AC_Odometer(display_ACtripmeter_speed);//Store AC trip value after IGN off
			//AFE DATA Write
			eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_OFFSET,AccFuelConsp);
			eeprom_write_AFE_data(AFE_ACC_FUEL_CONSP_GD_OFFSET,AccFuelConspGD);
			eeprom_write_AFE_data(AFE_ODO_DISP_RESET_OFFSET,OdoDispReset);
			eeprom_write_AFE_data(AFE_ODO_DISP_RESET_GD_OFFSET,OdoDispGDReset);
			/*****/
			AFEDispVal_EEPROM.u16 = AFEDispVal;
			New_eeprom_memwrite_32bytes(AFE_DISP_OFFSET, AFEDispVal_EEPROM.u8, 2);
			/*eeprom_write_AFE_data(AFE_DISP_OFFSET,AFEDispVal);*/
			/*****/
			//AFE DATA Write end
			//Write IVN FC Counter
			eeprom_write_IVN_FC_data(IVN_FC_COUNTER_OFFSET,IVN_EEFreshnessCounter);

			New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_A, AVS_sec_count_A.u8, 4);	//AVS time counter;
			New_eeprom_memwrite_32bytes(AVS_SEC_OFFSET_B, AVS_sec_count_B.u8, 4);	//AVS time counter B;
			New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_A, DT_sec_count_A.u8, 4);		//AVS time counter B;
			New_eeprom_memwrite_32bytes(DT_SEC_OFFSET_B, DT_sec_count_B.u8, 4);		//AVS time counter B;


			Ign_Off_Counter =SET; /* This flag SET for perform operation after IGN off condition. Counter SET value set by According Requirement*/
			Update_ign_Flag=SET;  /*This flag SET for perform operation after IGN off condition.*/
			IGN_ON_flag = 0;

			Timer_ON_100msec = 0; /* from IGN ON condition */

			eeprom_write(FUEL_BAR_OFFSET, displayed_fuel_level); /* store displayed_fuel_level in memory */

			eeprom_write(SETTING_AFE_OFFSET, AFE_selected);        /* store AFE selected setting in EEPROM */
			eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected); /* store TimeF_selected in EEPROM */
			eeprom_write(SETTING_TEMPUNIT_OFFSET, TempU_selected); /* store TempU_selected in EEPROM */
			eeprom_write(SETTING_PAB_OFFSET, PAB_selected); /* store TempU_selected in EEPROM */
		}
#endif
			/**/
			/**********changes for system state*************/
			/*Timer_ON_13sec_ign = 1;*/
//			go_to_sleep	= 1;
			/***********************************************/
			//			if(Counter_13sec_ign_Flag == 1)	//after completing 13 sec from ignition off take 10 samples of 300ms
			//			{
			//				Timer_ON_13sec_ign = 0;//r
			//				Timer_ON_300msec = 1;
			//				ADC_Avg();
			//				Present_level = Current_Fuel_Bar(); /*  New_fuel_read_ad1; */
			//				eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level); /* store Present_level in memory */
			//				go_to_sleep	= 1;			//allow cluster to go in deep sleep
			//			}

			/**/
			display_speedometer_speed=0;
			display_tachometer_speed=0;
			final_vechile_speed_gauge_raw=0;
			loc_Ism_TC0004();/*RPM and Speedo movement*/


			R_GPIO_WritePin(17, 2, 0);/*Gauge pointer illumination*/
			R_GPIO_WritePin(16, 11, 0);/*GI_CONTROL*/
			R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
			R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/



			handleDromeState();

			//R_GPIO_WritePin(42, 12, 1);//parallel to serial CE
			IGN_mode();
//			R_GPIO_WritePin(17, 11, 0); /* Fuel Control disable */		//disable after go_to_sleep == 1
/*
#ifdef GRAYSCALE_TFT
			clear_screen();

#endif
*/


#if 0
			PORT_AWOPM0_4=1;//PM0.PM0_4 = 1;
			PORT_AWOPMC0_4=0;//PMC0.PMC0_4 = 0;
			PORT_AWOPCR_PIBC0_4=0;//			PIBC0.PIBC0_4 = 0;
#endif
			spdgauge_position = ISM0VAR4CFG0;
			tacogauge_position = ISM0VAR4CFG1;

			while((ISM0VAR4CFG0 > 10000) || (ISM0VAR4CFG1 > 10000));//&&

			if(INIT_DONE_flag==1)
			{

				if(TURN_TT_TIMEOUT!=0)
				{
					if(turn_activate==0)
						turn_activate=1;
				}
					can_IGNOFF_init();
					wakeup_factor_flag = WAKEUP_TURNR;
					IGNOFF_eeprom_read = 1;
					Standby_Mode();

			}

//			gui_shutdown = 1;
//			handleDromeState();
//			handleDromeState();

			/***********************************************/

#ifdef GRAYSCALE_TFT
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 0;
					/*TFT off(reset pin low)*/
					PORT_ISOP44_5 = 0;
#endif
#ifdef COLOR_TFT
					PORT_ISOP44_3 = 0;
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 0;
#endif

#ifdef GRAYSCALE_TFT
			clear_screen();

#endif
			/******************************************************/
			R_GPIO_WritePin(3, 9, 0); /* 5V_SWSC enable */
			//R_GPIO_WritePin(10, 10, 1); /* CE for input shift reg*/
			R_GPIO_WritePin(3, 8, 0); /* TT_DRIVER */
			R_GPIO_WritePin(42, 12, 1);//parallel to serial CE
/**/
			R_GPIO_WritePin(17, 6, 0);//Audio amplifier
			R_GPIO_WritePin(16, 8, 0);//can_standby
			R_GPIO_WritePin(3, 4, 1);//can_enable
			R_TICK_WaitMS(0u, 10u);  /* delay 3 sec */
			R_GPIO_WritePin(16, 8, 0);//can_standby
			//			PORT_ISOP17_3 = 0; /*TFT Backlight */
			//			/*TFT off(reset pin low)*/
			//			PORT_ISOP44_5 = 0;
			//#ifdef COLOR_TFT
			//			PORT_ISOP44_3 = 0;
			//#endif


			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			R_TICK_WaitMS(0u,150u);  /* delay 150 msec */  //NEED TO CHECK
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			R_GPIO_WritePin(3, 4, 0);//can_enable

			//	R_TICK_WaitMS(0, 2500);/*give time for pointers to reach 0 when at max*/
			//__DI();
			R_GPIO_WritePin(21,1,1);/*flash disable*/

//			eeprom_writebt(BATT_DISCONNECTION_OFFSET,0); //Write Battery disconnection

			do{
				WatchDog_Clear();

				handleDromeState();
										handleDromeState();
				if((go_to_sleep == 1)&&(Drive_summery_tout_flag == 1))		//allow cluster to go in deep sleep after writing present fuel level in memory
				{
#if 0
#ifdef GRAYSCALE_TFT
					r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 0;
					/*TFT off(reset pin low)*/
					PORT_ISOP44_5 = 0;
#endif
#ifdef COLOR_TFT
					PORT_ISOP44_3 = 0;
					PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
					PORT_ISOPM17_3 = 0;
					PORT_ISOP17_3 = 0;
#endif

#ifdef GRAYSCALE_TFT
			clear_screen();

#endif
#endif

			R_GPIO_WritePin(17, 11, 0); /* Fuel Control disable */
#if 1/*port to high impedance*/
					/*PORT_ISOPM1 |= 0xFFFF;
					PORT_ISOPMC1 &= 0x0000;
					PORT_ISOPIBC1 &= 0x0000;

					PORT_ISOPM3 |= 0xFFEF; // P3_4 in output mode.
					PORT_ISOPMC3 &= 0x0000;
					PORT_ISOPIBC3 &= 0x0000;*/

					PORT_ISOPM10 |= 0xFFFF;
					PORT_ISOPMC10 &= 0x0000;
					PORT_ISOPIBC10 &= 0x0000;

					PORT_ISOPM11 |= 0xFFFF;
					PORT_ISOPMC11 &= 0x0000;
					PORT_ISOPIBC11 &= 0x0000;

					PORT_ISOPM16 |= 0xFFFF;
					PORT_ISOPMC16 &= 0x0000;
					PORT_ISOPIBC16 &= 0x0000;

					/*PORT_ISOPM17 |= 0xFFFF;
					PORT_ISOPMC17 &= 0x0000;
					PORT_ISOPIBC17 &= 0x0000;*/

					PORT_ISOPM21 |= 0xFFFF;
					PORT_ISOPMC21 &= 0x0000;
					PORT_ISOPIBC21 &= 0x0000;

					PORT_ISOPM22 |= 0xFFFF;
					PORT_ISOPMC22 &= 0x0000;
					PORT_ISOPIBC22 &= 0x0000;

					PORT_ISOPM42 |= 0xEFFF;
					PORT_ISOPMC42 &= 0x0000;
					PORT_ISOPIBC42 &= 0x0000;

					PORT_ISOPM43 |= 0xFFFF;
					PORT_ISOPMC43 &= 0x0000;
					PORT_ISOPIBC43 &= 0x0000;

					PORT_ISOPM44 |= 0xFFFF;
					PORT_ISOPMC44 &= 0x0000;
					PORT_ISOPIBC44 &= 0x0000;

					PORT_ISOPM45 |= 0xFFFF;
					PORT_ISOPMC45 &= 0x0000;
					PORT_ISOPIBC45 &= 0x0000;
#endif

					protected_write(SYSPROTCMD0,SYSPROTS0,SYSIOHOLD,/*0x7E*/0x7E);/**/

					PORT_ISOPM3 |= 0xFFEF; // P3_4 in output mode.
					PORT_ISOPMC3 &= 0x0000;
					PORT_ISOPIBC3 &= 0x0000;

					PORT_ISOPM42 |= 0xEFFF;
					PORT_ISOPMC42 &= 0x0000;
					PORT_ISOPIBC42 &= 0x0000;
					//SYSIOHOLD |= 0xFFFFFFFF;


					//asm("halt");
					if(TURN_TT_TIMEOUT==0)
					{
						__DI();

					/*Interrupt req clear*/
					INTC1EIRF0	=	0;
					INTC1EIRF1	=	0;
					INTC1EIRF2	=	0;
					INTC1EIRF3	=	0;
					INTC1EIRF4	=	0;
					INTC1EIRF5	=	0;
					INTC1EIRF6	=	0;
					INTC1EIRF7	=	0;
					INTC1EIRF8	=	0;
					INTC1EIRF9	=	0;
					INTC1EIRF10	=	0;
					INTC1EIRF11	=	0;
					INTC1EIRF12	=	0;
					INTC1EIRF13	=	0;
					INTC1EIRF14	=	0;
					INTC1EIRF15	=	0;
					INTC1EIRF16	=	0;
					INTC1EIRF17	=	0;
					INTC1EIRF18	=	0;
					INTC1EIRF19	=	0;
					INTC1EIRF20	=	0;
					INTC1EIRF21	=	0;
					INTC1EIRF22	=	0;
					INTC1EIRF23	=	0;
					INTC1EIRF24	=	0;
					INTC1EIRF25	=	0;
					INTC1EIRF26	=	0;
					INTC1EIRF27	=	0;
					INTC1EIRF28	=	0;
					INTC1EIRF29	=	0;
					INTC1EIRF30	=	0;
					INTC1EIRF31	=	0;
					}
					SYSRESFC=0xFFFFFFFF;/*clear reset factor*/

					//R_AWOT_create();
//
					R_DEV_IntEnable(R_DEV_INT_P0, 1);/*IGN interrupt*/
					R_DEV_IntEnable(R_DEV_INT_P5, 1);/*CAN wakeup(INH) interrupt*/
					R_DEV_IntEnable(R_DEV_INT_P1, 1);/*CAN wakeup(INH) interrupt*/
					//init_port_jp0_1();
					//init_port_jp0_2();

					uint32_t val;
					if((VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))/*Immo telltele config 1 for pseudo sleep*/
					{
						if((*RRAM_IMMO_PATT4 == 1)||(*RRAM_IMMO_PATT5 == 1)) /*only allow for Pattern4 & Pattern 5*/
						{
							immo_TT_config=1;
						}
					}
					//immo_TT_config=0;
					if(immo_TT_config==1)
					{
						val = (uint32_t) (~(R_STBC_WUL_INTP0_BIT1 | R_STBC_WUL_INTP1_BIT2 | R_STBC_WUL_INTAWOT0_BIT16 | R_STBC_WUL_INTP5_BIT6 |R_STBC_WUL_INTP9_BIT10 |R_STBC_WUL_INTP7_BIT8) & 0xFFFFFFFF);
					}
					else
					{
						val = (uint32_t) (~(R_STBC_WUL_INTP0_BIT1 | R_STBC_WUL_INTP1_BIT2 | R_STBC_WUL_INTP9_BIT10 | R_STBC_WUL_INTP5_BIT6 |R_STBC_WUL_INTP7_BIT8) & 0xFFFFFFFF);
						//val = (uint32_t) (~R_STBC_WUL_INTAWOT0_BIT16 & 0xFFFFFFFF);
					}


					R_DEV_WRITE_REG(32, STBC_WUFMSK0, val);
					uint32_t u32_temp;
					r_Error_t error = R_ERR_OK;
#if 1 /*RTC in deepstop*/
					u32_temp = R_DEV_ClkFrequencyHz(R_DEV_CKS_RTC);
#define EE_PCLK_CKS_MOSC_8     (8000000uL)
#define EE_CLK_SOSC_FRQ        (32768uL)
					if (u32_temp == EE_CLK_SOSC_FRQ)
					{
						/* RTC clock is using the subosc so stop the MOSC in standby */
						error += R_DEV_SetGenStopMask(R_DEV_MOSC, 0/*STBC_DISABLE_DEEPSTOP*/);
					}
					else
					{
						/* RTC clock is using the master osc so enable the MOSC in standby */
						error += R_DEV_SetGenStopMask(R_DEV_MOSC, 1/*STBC_ENABLE_DEEPSTOP*/);
					}


					/* note:
		 				        The PLL is not stopped before entering Standby this is because during
		 				        BSP init the PLL's are stopped and restarted again.  If you stop the PLL
		 				        here then anyone that uses the PLL (i.e. CPU) needs to be switched to
		 				        another clock before the PLL is stopped	otherwise the STBC will not receive
		 				        an acknowledge from it.
		 				        If the PLL is not stopped before it enters standby it may report incorrect
		 				        status after wakeup, PLL must be stopped and restarted after wake up then.
		 				        this is done in the BSP currently
					 */

					/* clocks operating in standby */
					/* sub osc - always operating */

					/* clocks stopped operating in standby */
					error += R_DEV_SetGenStopMask(R_DEV_HS_RING, 0/*STBC_DISABLE_DEEPSTOP*/);


					/* PLL0, PLL1 and 2 are configured to stop in the BSP */

					/* check all the clocks have stopped */
					if(R_ERR_OK == error)
					{
						/* do nothing */
					}
					else
					{
						/* error something went wrong */
						while(1);
					}
					/* ensure the RTC clock is enabled in the AWO area in deepstop */
#define CKSC_ARTCAD_STPM    0xFFF82418
#define RTCAD_STP_MSK       0x00000003

					R_DEV_WRITE_REG(32, CKSC_ARTCAD_STPM, RTCAD_STP_MSK);
#endif
					/* ensure the AWOT clock is enabled in the AWO area in deepstop */

#define CKSC_AAWOTD_STPM    0xFFF82218
					R_DEV_WRITE_REG(32, CKSC_AAWOTD_STPM, 0x01);



					//		eeRtc_Processing();
					//		protected_write(SYSPROTCMD0,SYSPROTS0,SYSSTBC0PSC,0x02);/*deep sleep*/
					//		SYSSTBC0DISTRG = 1; /* sleep mode */

					R_DEV_WRITE_REG(32, STBC_WUFC0, 0xFFFFFFFFu);
					/* set deepstop */


					/*if(R_AWOT_Start(0) != R_AWOT_ERR_OK)
		 				    {
		 				    	while(1);
		 				    }*/
					//R_TICK_WaitMS(0, 2500);
					for(i = 0; i < 0xffff; i++);		//need to check

					//SYSROSCSTPM=0x01;

					R_AWOT_Stop(0);

					//SYSWUFC0 = 0xffffffff;   // clear
					//SYSWUFMSK16 = 0;
					if(immo_TT_config==1)
					{
						my_awot_init1sec();
					}
					R_GPIO_WritePin(0, 4, 0);
					for(i = 0; i < 0xfffe; i++);     //need to check
					WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
					do
					{
						R_DEV_WRITE_PROT_REG(STBC_PROTCMD0, STBC_STBC0PSC, STBC_DEEPSTOP);
					}while(SYSPROTERR == 1);


					Enter_while_flag=0;
				}
			}while((IGN==0)&&(Timer_ON_13sec_ign == 1));

			//__EI();
			//R_EIC0_Start(); /* clears interrupt flag and enables interrupt mask */
			//R_EIC0_Stop(); /* disable Interrupt mask and enable Interrupt request flag */
			R_GPIO_PinDirection(17,9,0); /* watchDog output */
			WatchDog_flag = 1; /* No pulse to watchdog */
			//while(1);
			uint32_t val;
			R_DEV_IntEnable(R_DEV_INT_P0, 1);/*IGN interrupt*/
			val = (uint32_t) (~(R_STBC_WUL_INTP0_BIT1 /*| R_STBC_WUL_INTP5_BIT6*/) & 0xFFFFFFFF);
			R_DEV_WRITE_REG(32, STBC_WUFMSK0, val);
			R_DEV_WRITE_REG(32, STBC_WUFC0, 0xFFFFFFFFu);
			/* set deepstop */
			do
			{
				R_DEV_WRITE_PROT_REG(STBC_PROTCMD0, STBC_STBC0PSC, STBC_DEEPSTOP);
			}while(SYSPROTERR == 1);

			//protected_write(SYSPROTCMD0,SYSPROTS0,SYSSWRESA,0x01);/*Software reset*/
			while(1);
			/*#pragma asm
	//changing INTBP
			.extern _RESET
			 jr _RESET

#pragma endasm
			 */
			//			goto RESET;
			//Turn off SMPS
		}

		//	ADC_Start_Conversion();
		//Write_UDS_Data(); //call each 500ms
		Scheduler(); /*Schedule ALL Task*/
#ifdef COLOR_TFT
		eeTask_Processing();//Sound
#endif
		handleDromeState();	/*function renaming*/
		if((logo_completed==1)&&(IGN_ON_for_fuel == 1))
		{
			IGN_ON_fuel = 1;
			update_fuel_Bar = 1;//test only
		}
#ifdef GRAYSCALE_TFT
		if(first_screen_timeout_start >=1)
		{
			//PORT_AWOP0_4 = 1;
			//__DI();
			//R_GPIO_WritePin(0, 4, 1);
			//draw_grayscale();
			//PORT_AWOP0_4 = 0;
			//R_GPIO_WritePin(0, 4, 0);
			//__EI();
		}
#endif

		//handleButtonPress();
		//Engine_Coolant_Temperature(); // call each 100ms
		//Display_Fuel_Bar();
	}

	/*FlashDash_InitMem();
    FlashDash_InitWm();
    FlashDash_InitDrwDriver();
    LoadToRam();
    R_TICK_Init(0);
    CarModelInit();

    FlashDash_CreateUi();
    FlashDash_Run();
    FlashDash_DeinitDrwDriver();
	 */
	return(0);
}

PRIVATE void Scheduler(void)
{
    static Reset_delay = 0;
    
	WatchDog_Clear();
//	proveout_routine();
	//static char SW_loading;
	/*Check UDS Flag and process for UDS*/
#if 1
	if(UDS_TASK == SET) /* 1 msec */
	{
		//Process_UDSData();
	    if(wdt_reset_cntr == SET)
	    {
		Reset_delay++;
		if(Reset_delay > 10)
		{
		    wdt_reset_cntr = CLEAR;
			eeprom_write(rtc_hrs,RTCA0HOURC);
			eeprom_write(rtc_min,RTCA0MINC );
			eeprom_write(rtc_sec,RTCA0SECC);
		    protected_write(SYSPROTCMD0,SYSPROTS0,SYSSWRESA,0x01);/*Software reset*/
		}
	    }
	    if(dtc_log_lock_flag == CLEAR)
		{
			Set_Clear_IVN_DTC();
		}

	    if(clear_dtc_log_flag == SET)
	    {
		Clear_EEPROM_DTC_Memory();
	    }
	    UDS_TASK = CLEAR;
	}
#endif
	EEPROM_readwrite_para_started_on = 0;
	Enter_while_flag=1;
#if 0
	if(Update_FStepper_Flag == SET) /*&& (Over_volt_flag == CLEAR)*/ /* 15ms */
	{
		_Bool prove_temp;

		/*SW_loading=!SW_loading;
		R_GPIO_WritePin(17, 10, SW_loading);
		R_GPIO_WritePin(16, 10, SW_loading);*/
		/* feed the dog */
		Update_FStepper_Flag= CLEAR;
		if(Proveout_Count < PROVEOUT_UP) /*wait for 15ms x 70 (PROVEOUT_UP), which is around 1sec*/
		{
			if(prove_temp==0)
			{
				GI_Control();
				prove_temp=1;
			}
			display_speedometer_speed=180;
			final_vechile_speed_gauge_raw=180*64;
			display_tachometer_speed=7000;
			Proveout_Count++;
			All_Telltale_ON();
			if(execute_ones_flag==0)
			{
				execute_ones_flag=1;
				SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
				SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
				cascade_TT_send_data();
				ReadDCData();
			}


#ifdef COLOR_TFT
			if(first_screen_timeout >=1 && welcomedone == 0)
			{
				welcomedone=1;
				SoundPCM_Welcome();
			}
#endif
		}
		else if(Proveout_Count < PROVEOUT_DOWN)/*wait for 15ms x 70 (PROVEOUT_DOWN), which is around 1sec*/
		{
			/*if((Proveout_Count == PROVEOUT_UP)&&(BATT_DISCONNECTION==1))
			{
				ISM0VAR4CFG0=5120000;
				ISM0VAR4CFG1=5120000;
			}*/
			display_speedometer_speed=0;
			final_vechile_speed_gauge_raw=0;
			display_tachometer_speed=0;
			Proveout_Count++;
			//All_Telltale_OFF();
		}
		else
		{
			//speedometer();
			//tachometer();
			Proveout_Count = PROVEOUT_DOWN+4; /*no need for this statement, good to be safe*/
		}

		//		loc_Ism_TC0004();/*RPM and Speedo movement*/

		/* Update Fuel value */
	}
#endif 0
	if(Read_ADC_Flag == SET)                                    /* 25mS */
	{
		/* feed the dog */
		Read_ADC_Flag = CLEAR;
		/* Update ADC value */
	}


	if(Read_TT_flag == SET)                                    /* 100mS */
	{
		/* feed the dog */
		Read_TT_flag = CLEAR;
		Engine_Coolant_Temperature(); // call each 100ms

		/*#ifdef U350
		SWSC_Mono_Condition();// call each 100ms
#else*/
//#ifdef COLOR_TFT
//		SWSC_Condition();
//#endif
//#ifndef U350
//		SWSC_Condition();//only applicable for W501 H/L
//#endif
		/* Update TT status */
	}
	if(Counter_1000ms_flag == SET)
	{
		Counter_1000ms_flag=CLEAR;
		IGN_mode();
#if 0
		if(LVSD_flag ==0 && HVSD_flag == 0 && Read_TT_flag == SET && IGN_ON_flag==1) //call each 500ms
		{
			input_shift_register();
			//scan_TT_status();
			//DIAG_TT_init();
			output_shift_register();
		}
		if(LVSD_flag ==1 || HVSD_flag ==1)// call each 1000ms
		{
			All_Telltale_OFF();
		}
#endif
		Write_UDS_Data(); //call each 500ms
		//		Current_Gear_and_Gear_Shift_Indication();
		//Door_Open();
		//		Engine_oil_pressure_low();
		//		Check_ABS();
		///		Seat_belt();
		//		High_Temp();
		//		Low_Brake_Fluid();
		//		Fuel_Low();
		//		DPF_Filter_Clogged();
		///		digi_clk_can();
		//DEF_Level();
	}
	//tripmeter_B();

	//	SWSC_Condition();
	if(Counter_500ms_popup == SET)
	{

		Counter_500ms_popup=CLEAR;
		//Current_Gear_and_Gear_Shift_Indication();
		if(IO_Parameter_Request_Status !=3)
		{
#ifdef U350
			RPAS_Screen_Activation_U350();//4
#else
			//RPAS_Screen_Activation_W501();//4
//			W501_RPAS_CHIME_ENABLE();
#endif
//			GI_off_SW_PRESS();		/*light ON/OFF*/
#ifdef SPRINT4
			if((Priority_flag == 0)&&(RPAS_Precondition_Active == 0)&&(AT_gear_IGN_ON == 0))
#else
			if((Priority_flag == 0)&&(RPAS_Precondition_Active == 0))
#endif
			{
				Overspeed_alert();//1
				Seat_belt();//2
#ifdef SPRINT4
//#ifdef COLOR_TFT
				if((TDMS_alert_active == 1)&&(TDMS_popup_shown == 0)&&(logo_completed == 1))
				{
					TDMS_Alert_Screen_Display();
				}
//#endif
#endif
				Check_EXT_Lamp();//3
				Engine_oil_pressure_low();//5
				High_Temp();//6
				Low_Brake_Fluid();//7
				Park_Break_alert();//8	Execute after very 500ms
				PAB_Off_alerts();//9
				if(Alert_receive_flag == 1)
				{
					DPF_Filter_Clogged();//10
					REGEN_Alert();
					DEF_alerts();
					if((DEF_priority_flag == 0)&&(REGEN_priority_flag == 0)/*&&(checkRegenDEF_flag == 1)*/&&(Priority_flag == 0))
					{
						Door_Open();//8
						Check_Engine_alert();//9
						Fuel_Low();//44
						Check_EBD_alert();//45
						Check_ABS();//46
						Check_ESP_alert();//47
						RPAS_Supply_Failure_alerts();//48
						RKE_Low_Battery_alert();//49	RKE_Battery
						Water_In_Fuel_alert();//50
						Service_reminder();//51
#ifndef U350
						Alert_Condition_4WD_Meter();
#endif
						Cruise_alerts();//53
						ECO_Mode_alerts();//54
						HDC_alert();//55	Need to add alert screen
						Stop_for_setting();//56		//////////////remaining
						Func_safety();//57
						Trailer_SW_On_alerts();//58
						LEFT_Turn_Lamp_failure_alerts();//59
						RIGHT_Turn_Lamp_failure_alerts();//60
						DRL_failure_alerts();//61
						Tail_Lamp_failure_alerts();//62
						Horn_failure_alerts();//63

					}
				}
			}
			if(RPAS_Precondition_Active == 1)
			{
				STS_Eng_data1 = CAN_EMS1_10[6].byte; /* stored 6th byte of EMS1_10 */
				STS_Eng_data1 = ((STS_Eng_data1 & 0xE0) >> 5); /* using Higher nibble 1st 3 bit */
				if(STS_Eng_data1 == 2)
				{
					eng_start_timer_on = 1;
				}
				else
				{
					eng_start_timer_on = 0;
				}
			}

			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1) && (Diag_RW_WIF_Warn_Enable == 1) && (FEATURE_CODE_FUEL_TYPE==1)&&(RPAS_Precondition_Active == 1))
			{
				if((CAN_EMS1_10[5].bits.Rx_5 == 1)&&(Water_in_fuel_TT == 1))
				{
					if(eng_start_timer_on == 1)
					{
						Water_In_Fuel_Timer_ON = 1;
					}
					else
					{
						Water_In_Fuel_Timer_ON = 0;
						//WIF_5sec_done = 0;
					}
				}
				else
				{
					//WIF_5sec_done = 0;
				}
			}


			if(Dooropen_popup_shown == 1)	// For Update the door status when alert active
			{
				Door_Open();//8
			}
			Handle_Inactive_alerts();
#ifndef U350
			Main_Screen_Display_4WD();
#endif
#ifdef SPRINT4
//#ifdef COLOR_TFT
			TDMS_Display_Meter();
//#endif
#endif
			STS_4WD = (CAN_IC2_100[3].byte & 0x60);
			if(STS_4WD != 0)
			{
				Init_2WD_odo_distance = final_odomter_distance;
				Prev_2WD_odo_distance = 0;
			}

//			}
			ETM_LED_test();	//not working as per requirement
			ETM_Guage_Test();
		}
		else
		{
			IO_Control_Message_activation();
			//IO_Control_Gear_indication();
			//IO_Control_Gear_shift_indication();
#ifdef GRAYSCALE_TFT
			IO_Control_TFT_Test_Grayscale();
#endif

#ifdef COLOR_TFT
			IO_Control_TFT_Test_ColourTFT();
#endif
		}
		digi_clk_can();
		AC_tripmeter();//for try

//		if(EEPROM_IN_USE == 0)
//		{
//			EEPROM_LOG = eeprom_read(MEMORY_READ_TEST_OFFSET);
//		}
		//DEF_Level();
	}
	//tripmeter_B();

	//	SWSC_Condition();



}

void FlashDash_Error(void)
{
	for (;;)
	{
	}
}

void proveout_routine(void)
{
	//execute_ones_flag=1;
	if(Update_FStepper_Flag == SET) /*&& (Over_volt_flag == CLEAR)*/ /* 15ms */
	{
		_Bool prove_temp;

		/*SW_loading=!SW_loading;
		R_GPIO_WritePin(17, 10, SW_loading);
		R_GPIO_WritePin(16, 10, SW_loading);*/
		/* feed the dog */
		Update_FStepper_Flag= CLEAR;
		if(Proveout_Count < PROVEOUT_UP) /*wait for 15ms x 70 (PROVEOUT_UP), which is around 1sec*/
		{
			display_speedometer_speed=176;
			final_vechile_speed_gauge_raw=176*64;
			display_tachometer_speed=6950;
			Proveout_Count++;



#if 0
#ifdef COLOR_TFT
			if(first_screen_timeout >=1 && welcomedone == 0)
			{
				welcomedone=1;
				SoundPCM_Welcome();
			}
#endif
#endif

		}
		else if(Proveout_Count < PROVEOUT_DOWN)/*wait for 15ms x 70 (PROVEOUT_DOWN), which is around 1sec*/
		{
			/*if((Proveout_Count == PROVEOUT_UP)&&(BATT_DISCONNECTION==1))
			{
				ISM0VAR4CFG0=5120000;
				ISM0VAR4CFG1=5120000;
			}*/
	//		display_speedometer_speed=0;
	//		final_vechile_speed_gauge_raw=0;
	//		display_tachometer_speed=0;
	//		Proveout_Count++;
			//All_Telltale_OFF();
		}
		else
		{
			//speedometer();
			//tachometer();
	//		Proveout_Count = PROVEOUT_DOWN+4; /*no need for this statement, good to be safe*/
		}

		//		loc_Ism_TC0004();/*RPM and Speedo movement*/

		/* Update Fuel value */
/*For LVSD HVSD TFT backlight and GI off prevention*/

		PORT_ISOPMC17_3 = 0;      /*to solve TFT backlight ON at IGN OFF*/
		PORT_ISOPM17_3 = 0;
		PORT_ISOP17_3 = 1;
/**/
if(LVHV_detect == 0x55)//try with comment and uncomment
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
		/**/
#ifdef GRAYSCALE_TFT
refresh_rate = REFRESH_RATE_FAST;
#endif
		handleDromeState();
#ifdef GRAYSCALE_TFT
		flag_grayscale = refresh_rate - 3;
#endif
		Bootup_Trigger = 1;	/*Boot up animation start*/
		handleDromeState();
		handleDromeState();
#ifdef COLOR_TFT
		(void)drome_sendEventToDrome((U32)drome_KB_DOWN);
#endif
		handleDromeState();
//#ifdef COLOR_TFT
		(void)drome_sendEventToDrome((U32)drome_InEvent_start_animation);
//#endif

		while((animation_start==1)&&(LVSD_flag_1 == 0) && (HVSD_flag_1 == 0)&&(IGN==1))
		{
			handleDromeState();
			startkey = drome_getCurrentViewKey();
#if 0
			#ifdef COLOR_TFT
			if(first_screen_timeout >=1 && welcomedone == 0)
			{
				welcomedone=1;
				SoundPCM_Welcome();
			}
			#endif
#endif
			Init_read_parameters();
			Regen_data.bits.Rx_1 = CAN_EMS29_100[3].bits.Rx_7;  /* regen_tt*/
			Regen_data.bits.Rx_2 = CAN_EMS29_100[3].bits.Rx_8;
			if(Regen_data.byte == 2)
			{
				Main_page_Regen = 2;//0;		/*Regen*/
				HoldOk_Regen_text = 1;
			}
			FOUR_WD_INIT_STS();
#ifdef SPRINT4			
			if(default_page == 32)
			{TDMS_Main_Screen_SlewTimer = 20;
			TDMS_Main_Screen();
			}
			AT_gear_alert();
#endif
		}
		flag_grayscale = refresh_rate - 3;
		handleDromeState();
		handleDromeState();
//		logo_completed=1;
		//	refresh_rate = REFRESH_RATE;



	}
}

/* **********************************************************************************************************************
 * Function Name: checkvalidgraphicsappln
 * Description  : checks valid application is presesnt
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************** */
unsigned char checkvalidgraphicsappln(unsigned long appAddr)
{
	//WDTA1WDTE = 0xAC;				//wATCHDOG cLEAR
	// These are the 3 possible graphics header file versions available
	/*	char validstring[8] = "U350GL10";
	char validstring1[8] = "W501GL10";
	char validstring2[8] = "W501GH10";*/

	unsigned char readBuffer_u8[9];
	unsigned char ret=SET,count;
	memset(readBuffer_u8, 0, sizeof(readBuffer_u8));
	api_fcl_read(appAddr , 9 , &readBuffer_u8[0]);

	for(count = 0; count < 8; count++)
	{
		Graphics_version_read[count]=readBuffer_u8[count+1];

		if( (readBuffer_u8[count+1] != Graphics_version[count]) )
		{
			ret = CLEAR;
		}
	}

	return ret;
}

void api_fcl_read(unsigned long adr, unsigned long count,unsigned char *data)
{
	static unsigned long Count = 0;
	for(Count = 0 ; Count < count ; Count++)
	{
		//data =   (unsigned char)(*((unsigned char*)(adr + Count)));
		data[Count] =   (unsigned char)(*((unsigned char*)(adr + Count)));
	}
}
/*
 * void Init_read_parameters(void):
 * Read write parameters at startup from eeprom and load them into local variables.
 * also initiate key variables.
 */

void Init_read_parameters(void)
{

	WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR


	IGN_ON_regen_init = 1;


	eeRtc_Processing();
	digi_clk_can();

	Timer_ON_100msec = 1;//fuel read ADC

	if(Read_Para_Flag == 0)//Read_odo_flag
	{
		IGN_toggle_flag = eeprom_read(IGN_TOGGLE_FLAG); /* read ignition toggle flag */
		eeprom_write(IGN_TOGGLE_FLAG, 0x55); /* store IGN_Toggle_flag in eeprom */
		TempU_selected = eeprom_read(SETTING_TEMPUNIT_OFFSET); /* read TempU_selected*/
		if(TempU_selected > 1)
		{
			TempU_selected = 0;								   /*default - degC*/
		}
		final_odomter_distance = eeprom_readOdometer();
		old_odometer_distance = final_odomter_distance;
		Read_odo_flag = 1;
		Init_Drivesum_odo_distance = final_odomter_distance;
		prev_sbrchm_odo = final_odomter_distance;	//store odo value after IGN on
		/*AFE_rst_odo_dist = final_odomter_distance;*/
		final_tripmeter_distance = eeprom_readTrip1_Odometer();
		final_tripmeter_distance = eeprom_readTrip1_Odometer();

		final_tripmeter_B_distance = eeprom_readTrip2_Odometer();
		final_tripmeter_B_distance = eeprom_readTrip2_Odometer();

		final_ACtripmeter_distance = eeprom_readTrip_AC_Odometer();
		final_ACtripmeter_distance1 = eeprom_readTrip_AC_Odometer1();
		TRIP_AC_OFFSET = eeprom_readTrip_AC_Offset();
		/*Fuel data Read*/
		Present_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET); /* read fuel bar from memory & store in Present_level */
		displayed_fuel_level = eeprom_read(FUEL_BAR_OFFSET); /* read  last fuel bar which is displayed on LCD & store in "displayed_fuel_level"  */
		/*Fuel data Read*/

		/*check for value greater than 9999km*/
		if(final_odomter_distance == 0)
		{
			if(final_ACtripmeter_distance1 == 65535)
			{
				final_ACtripmeter_distance1 = 0;
				eeprom_write_Trip_AC_Odometer1(final_ACtripmeter_distance1);//write 0 if value is 0xFFFF
			}
			if(TRIP_AC_OFFSET == 65535)
			{
				TRIP_AC_OFFSET = 0;
				eeprom_write_Trip_AC_Offset(TRIP_AC_OFFSET);//write 0 if value is 0xFFFF
			}
		}

		/****AVS counter****/
		eeprom_memread_32bytes(AVS_SEC_OFFSET_A,AVS_sec_count_A.u8, 4);	//AVS A counter

		eeprom_memread_32bytes(AVS_SEC_OFFSET_B,AVS_sec_count_B.u8, 4);	//AVS B counter

		eeprom_memread_32bytes(DT_SEC_OFFSET_A,DT_sec_count_A.u8, 4);	//DT A counter

		eeprom_memread_32bytes(DT_SEC_OFFSET_B,DT_sec_count_B.u8, 4);	//DT B counter


		if(DT_sec_count_A.u32 == 0xFFFFFFFF)
		{
			DT_sec_count_A.u32 = 0;
		}
		if(DT_sec_count_B.u32 == 0xFFFFFFFF)
		{
			DT_sec_count_B.u32 = 0;
		}
		if(AVS_sec_count_A.u32 == 0xFFFFFFFF)
		{
			AVS_sec_count_A.u32 = 0;
		}
		if(AVS_sec_count_B.u32 == 0xFFFFFFFF)
		{
			AVS_sec_count_B.u32 = 0;
		}

		eeprom_memread_32bytes(APP_S_EE_Last_Caltd_DTE,&rtU.S_EE_Last_Caltd_DTE, 4);
		DTE_Disp_Val = rtU.S_EE_Last_Caltd_DTE;
		rtY.S_DTE_Disp_Val = rtU.S_EE_Last_Caltd_DTE;

		eeprom_memread_32bytes(AFE_DISP_OFFSET,AFEDispVal_EEPROM.u8, 2);
		AFE_3_U.U16_AFE_LastDispVal_EE = AFEDispVal_EEPROM.u16;
		AFE_3_Y.U16_AFEDispVal = AFEDispVal_EEPROM.u16;
		AFEDispVal = AFEDispVal_EEPROM.u16;				/*Update after every IGN ON*/

		AFE_RST_Time_IGN_on =  ((disp_hour*60)+disp_min);
		eeprom_memread_32bytes(IGN_OFF_Time_OFFSET,&AFE_RST_Time_IGN_off,2);
		AFE_RST_Time = (AFE_RST_Time_IGN_on - AFE_RST_Time_IGN_off);
		/****************************/
		/*read Drive summary 4x4 not engaged value saved in eeprom as per CR - 26-05-20 for W501Hv232*/
		eeprom_memread_32bytes(FOURWD_NOTENGAGED_OFFSET,&Prev_2WD_odo_distance,4);
		/**************/
#if 0
		eeprom_memread_32bytes(AFE_DISP_OFFSET,AFEDispVal_EEPROM.u8, 2);
		AFEDispVal_init = AFEDispVal_EEPROM.u16;
		if(AFEDispVal_init == 0xFFFF)
		{
			AFEDispVal_init = 0;
		}
		AFE_3_Y.U16_AFEDispVal=AFEDispVal_init;
#endif
		/****************************/
#if 0
		eeprom_memread_32bytes(AFE_DISP_OFFSET,AFEDispVal_EEPROM.u8, 2);
		AFE_3_U.U16_AFE_LastDispVal_EE = AFEDispVal_EEPROM.u16;
#endif

		eeprom_memread_32bytes(DEFAULT_PAGE_OFFSET, Default_pagekey.u8, 4);	//Default page;

		if(Default_pagekey.u32 == 0x11001)	/*Trip - A*/
		{
			default_page = 21;
		}
		else if(Default_pagekey.u32 == 0x11011)	/*AFE - IFE*/
		{
			if((FEATURE_CODE_AFE_IND == 1) && (FEATURE_CODE_IFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				default_page = 22;
			}
			else
			{
				default_page = 21;// Trip - A
			}
		}
		else if(Default_pagekey.u32 == 0x11021)	/*Speedo*/
		{
			if((VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1)&&(VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0)&&(FEATURE_CODE_DIGITAL_SPEED_CONFIG == 1))
			{
				default_page = 33;
			}
			else if((VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0)&&(VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1)&&(FEATURE_CODE_DIGITAL_SPEED_CONFIG == 1))
			{
				default_page = 33;
			}
			else
			{
				default_page = 21;// Trip - A
			}

		}
		else if(Default_pagekey.u32 == 0x11051)	/*Setting Main Page*/
		{
			default_page = 55;
		}
		else if(Default_pagekey.u32 == 0x11057)	/*Warning History Main Page*/
		{
			default_page = 44;
		}
		/************/
		else if(Default_pagekey.u32 == 0x11013)	/*ECO Score Page*/
		{
//			if(FEATURE_CODE_EcoScore == 1)
			{
				default_page = 26;
			}
			/*else
			{
				default_page = 21;// Trip - A
			}*/
		}
		else if(Default_pagekey.u32 == 0x11014) /*Only AFE Page*/
		{
			if((FEATURE_CODE_AFE_IND == 1) && (FEATURE_CODE_IFE_IND == 0) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				default_page = 23;
			}
			else
			{
				default_page = 21;// Trip - A
			}
		}
		else if(Default_pagekey.u32 == 0x11016) /*Only IFE Page*/
		{
			if((FEATURE_CODE_AFE_IND == 0) && (FEATURE_CODE_IFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				default_page = 24;
			}
			else
			{
				default_page = 21;// Trip - A
			}
		}
		else if(Default_pagekey.u32 == 0x11012)	/*DTE Page*/
		{
			if((FEATURE_CODE_DTE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				default_page = 25;
			}
			else
			{
				default_page = 21;// Trip - A
			}
		}
		else if(Default_pagekey.u32 == 0x11002)	/*Trip - B*/
		{
			default_page = 14;
		}
		else if(Default_pagekey.u32 == 0x11003) /*Trip - AC*/
		{
			if(FEATURE_CODE_AC_TRIP_METER == 1)
			{
				default_page = 18;//23;
			}
			else
			{
				default_page = 21;// Trip - A
			}
		}
		else if(Default_pagekey.u32 == 0x11004) /*Digital Clock*/
		{
			default_page = 19;//4;
		}
		else if(Default_pagekey.u32 == 0x11005) /*REGEN*/
		{
			Regen_data.bits.Rx_1 = CAN_EMS29_100[3].bits.Rx_7;  /* regen_tt*/
			Regen_data.bits.Rx_2 = CAN_EMS29_100[3].bits.Rx_8;
			if((FEATURE_CODE_REGEN == 1)&&((Regen_data.byte == 1)||(Regen_data.byte == 2)))
			{
				default_page = 20;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11006)	/*Trip A - A.V. Speed - D Time*/
		{
			if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1)	&& (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_DRIVING_TIME_CONFIG == 1))
			{
				default_page = 11;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11007)	/*Trip B - A.V. Speed - D Time*/
		{
			if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1)	&& (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_DRIVING_TIME_CONFIG == 1))
			{
				default_page = 15;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11008)	/*Trip A - A.V. Speed*/
		{
			if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				default_page = 12;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11009)	/*Trip A - D. Time*/
		{
			if(FEATURE_CODE_DRIVING_TIME_CONFIG == 1)
			{
				default_page = 13;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11010)	/*Trip B - A.V. Speed*/
		{
			if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				default_page = 11;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11020)	/*Trip B - D. Time*/
		{
			if(FEATURE_CODE_DRIVING_TIME_CONFIG == 1)
			{
				default_page = 15;//23;
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
		else if(Default_pagekey.u32 == 0x11202)	/*4WD*/
		{
			if((FEATURE_CODE_DISPLAY_4WD == 1)&&(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1))
			{
#ifdef COLOR_TFT
				default_page = 27;
#else
				default_page = 61;
#endif
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
#ifdef SPRINT4
		else if(Default_pagekey.u32 == 0x11206)	/*TDMS Main screen*/
		{
			if((FEATURE_CODE_TDMS_DISPLAY==1)&&(VARIANT_CODING_INFOTAINMENT_SYSTEM == 1))
			{
#ifdef COLOR_TFT
				default_page = 32;
#else
				default_page = 66;
#endif
			}
			else
			{
				default_page = 21;		//Trip A
			}
		}
#endif

		else
		{
			Default_pagekey.u32 = 0x11001;			/*Trip - A*/
			default_page = 21;
		}

		Current_Gear_and_Gear_Shift_Indication();
#ifdef SPRINT4
		AT_gear_IGM_ON_detected = 1;
		TDMS_Init();
#endif		
		Read_Para_Flag = 1;//	Read_odo_flag
		SBR_Alert_check = 1;

		//			fuel_widget =  Current_Fuel_Bar();
		//			temp_widget = 8;//r

		/*Trip A,B,AC reset on battery disconnection*/
#if 1
		BATT_DISCONNECTION = eeprom_read(BATT_DISCONNECTION_OFFSET);
		if(BATT_DISCONNECTION > 1) BATT_DISCONNECTION = 0;

		if(BATT_DISCONNECTION == 1)
		{
			BATT_DISCONNECTION1 = 1;
			Batt_disconnect_AFE_RST = 1;
			reset_tripmeterA();
			reset_tripmeterB();
			//AC_RESET
			Trip_AC_Rollover = 1;
			reset_AC_tripmeter();
			//reset RTC after battery disconnect
			hrs = 0;
			min = 0;
			sec = 0;
			*RRAM_hrs=0;
			*RRAM_min=0;
			*RRAM_sec=0;
			TimeF_selected = 2;
			eeprom_write(rtc_hrs,0);
			eeprom_write(rtc_min,0);
			eeprom_write(rtc_sec,0);
			eeprom_write(SETTING_TIMEFRMT_OFFSET,2);
			Timesetting_completed = 1;
			//rtY.S_DTE_1km_Completion_Flag=0;
			//rtY.U8_DTE_RAFE_Inst_1km_Cnt=0;
			U8_Reset_DTE = 1U;
			AFE_3_U.U16_AFE_LastDispVal_EE = 0;
			rtU.S_EE_Last_Caltd_DTE = 0;
			AFEDispVal_EEPROM.u16 = 0;
			AFE_3_Y.U16_AFEDispVal = 0;
			AFEDispVal = 0;
			default_page = 21;		//Trip A

			New_eeprom_memwrite_32bytes(RESET_DTE_OFFSET, &U8_Reset_DTE, 1);

			eeprom_writebt(BATT_DISCONNECTION_OFFSET,0); //Write Battery disconnection
		}
		else
		{
			//do_nothing
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
		Accumulated_distance_A = final_odomter_distance - final_tripmeter_distance ; 		/*Accumulated distance for Trip A*/
		if((DT_sec_count_A.u32 != 0)&&(Accumulated_distance_A != 0))
		{
//			Accumulated_distance_A = final_odomter_distance - final_tripmeter_distance ; 		/*Accumulated distance for Trip A*/
			Avg_vehicle_speed1 = (((Accumulated_distance_A)*100) / AVS_sec_count_A.u32)/**100)*/;/*AVS_sample_sec_A1*/;
			Avg_vehicle_speed1 = (Avg_vehicle_speed1 * 3.6)/100;
		}
		else
		{
			HMI_AVS_DASH1 = 1;
		}
		Accumulated_distance_B = final_odomter_distance - final_tripmeter_B_distance ; 		/*Accumulated distance  for Trip B*/
		if((DT_sec_count_B.u32 != 0)&&(Accumulated_distance_B != 0))
		{
//			Accumulated_distance_B = final_odomter_distance - final_tripmeter_B_distance ; 		/*Accumulated distance  for Trip B*/
			Avg_vehicle_speed2 = (((Accumulated_distance_B)*100) / AVS_sec_count_B.u32)/**100)*/;
			Avg_vehicle_speed2 = (Avg_vehicle_speed2 * 3.6)/100;
		}
		else
		{
			HMI_AVS_DASH2 = 1;
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
			if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))										/*Dist_Unit - km -> AVS unit - km/h*/
			{
				Display_Average_speed_B = Avg_vehicle_speed2;
			}
			else if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))														/*Dist_Unit - mile -> AVS unit - mph*/
			{
				Display_Average_speed_B = Avg_vehicle_speed2 * 0.621371;					/*1km= 0.621371 Mile*/
			}

			if(Display_Average_speed_B >= 300)
			{
				Display_Average_speed_B = 300;
			}
		}

		flag_grayscale = refresh_rate - 3;

#endif

		currIllumination = eeprom_read(ILLUM_NVM_OFFSET);	/* @@@ Read last used Illumination Value from NVM */
//		sw_press_ok = eeprom_read(LIGHT_ONOFFSTATUS_OFFSET);

	}
}

void Standby_Mode(void)
{
	uint32_t i;

	//	if((wakeup_factor_flag == WAKEUP_CAN)||(wakeup_factor_flag == WAKEUP_TURNR)||(wakeup_factor_flag == WAKEUP_POSITIONLAMP))
	{
		//		if(IGN==0)		/*Other than Immo*/
		{
			//			IGNOFF_commoninit();
			EEPROM_readwrite_para_started = 0;
			EEPROM_readwrite_para_started_on = 0;
			eeprom_memread_32bytes(IGNOFF_status_OFFSET,&IGNoff_status.byte, 1);

			if(IGNoff_status.byte == 0xFF)
			{
				Dooropen_active_IGNOFF = 0;
				IGNoff_status.byte = 0;
			}

			if((IGNoff_status.bits.Rx_1) == 1)
			{
				Dooropen_active_IGNOFF = 1;
			}
			flag_grayscale = refresh_rate - 3;
			handleDromeState();	/*function renaming*/
			handleDromeState();	/*function renaming*/
			refresh_rate = REFRESH_RATE_FAST;
//			ret_redraw = 1;
			display_start = 0;
		}

		while(IGN==0)
		{
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			eeRtc_Processing();
			if(wakeup_factor_flag==WAKEUP_TURNR)
			{

				if(move_out_of_standby<=100)
				{
					if(TURN_TT_TIMEOUT>0)
					{
						move_out_of_standby=0;
					}
					else
					{
						move_out_of_standby++;
					}
					R_TICK_WaitMS(0u, 100u);
					//SendGSData(GSdata1);
				}
				//	if(move_out_of_standby_park<=100)
				if(FEATURE_CODE_PARK_TT_TYPE == 1)
				{
					if(PORTJ_AWOJPPR0_1/*ParkLAMP*/ == 1)
					{

						PARK_LAMP_TT = 1;
						move_out_of_standby_park=0;

					}
					else
					{
						PARK_LAMP_TT = 0;
						move_out_of_standby_park++;
					}

					R_TICK_WaitMS(0u, 100u);
					//output_shift_register();

				}
				if(IGNOFF_MBFM_CAN_ON == 1)
				{
					IMMO_TT_WAKEUP_FLAG=1;
					if(door_statusflag == 0)
					{
						eeprom_memread_32bytes(IGNOFF_status_OFFSET,&IGNoff_status.byte, 1);
						if((IGNoff_status.bits.Rx_1) == 1)
						{
							Dooropen_active_IGNOFF = 1;
						}
						door_statusflag = 1;
					}
					if(display_active == 1)
					{
						handleDromeState();	/*function renaming*/
						handleDromeState();	/*function renaming*/
						handleDromeState();	/*function renaming*/
						/*if(ret_redraw != 0)
						{*/
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 1;
						//}
						if(display_start == 0)
						{
							display_start = 1;
							flag_grayscale = refresh_rate - 3;
						}

					}
					else
					{

						if(IGNOFF_display_timeout == 1)
						{
							handleDromeState();
							handleDromeState();
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							/*draw_grayscale();*/
							IGNOFF_display_timeout = 0;
						}
						else
						{
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
					}
				}
				else
				{
//					IMMO_TT_WAKEUP_FLAG = 0;
					if(IGNOFF_MBFM1_100_message_timeout_flag != 0)
					{
						/* @@@ Clearing Park Lamp and Key in Values when CAN message is absent, to play it again when Message is present  */
						parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
						buzzParkLamp = 0;
						buzzParkLampCntr = 0;
						priorityPark_Keyin = 0;
						Buzz_KIR_cntr = 0;
						KIR_Chime = 0;
						Buzz_KIR_compl = 0;

						IGNOFF_keyinremind_check_done = 1;
						IGNOFF_Dooropen_check_done = 1;
						IGNOFF_ExtLamp_check_done = 1;

						if(IGNOFF_display_timeout == 1)
						{
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
							dromecall_timeout = 1;
							do
							{
								WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
								handleDromeState();	/*drome call till it will not receive drive summary timeout black screen(startkey = 0x11108)*/
							}while((startkey != 0x11108)&&(dromecall_timeout <= 5000));
							dromecall_timeout = 0;
							handleDromeState();
							handleDromeState();
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
							/*draw_grayscale();*/
							IGNOFF_display_timeout = 0;
						}
						else
						{
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
						Dooropen_IGNOFF_popup_shown=0;
						Check_EXT_Lamp_IGNOFF_popup_shown = 0;
						DOOR_OPEN_TT = 0;
						DID_TT_bit_data_set1.bits.Rx_5=0;
						DOOR_OPEN_IGN_OFF=0;
						PARK_LAMP_IGN_OFF=0;
						if((FEATURE_CODE_PARK_TT_TYPE==0)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))
						{
							PARK_LAMP_TT=0;
							PARK_LAMP_IGN_OFF=0;
							Park_lamp_TT_IGN_Off=1;
						}
						else if((FEATURE_CODE_PARK_TT_TYPE==0)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0))
						{
							PARK_LAMP_TT=0;
							PARK_LAMP_IGN_OFF=0;
							Park_lamp_TT_IGN_Off=1;
						}

						if((Dooropen_active_IGNOFF == 1)&&(All_doorclose != 1))
						{
							IGNoff_status.byte = 0x01;					/*ON WAKE UP can BUS OFF & DOOR STATUS IS OPEN THEN STORE DOOR STATUS*/
							New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
						}
						else if(Dooropen_active_IGNOFF != 1)
						{
							IGNoff_status.byte = 0x00;
							New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
						}
						door_statusflag = 0;
					}
				}
				output_shift_register();
			}
			else if(wakeup_factor_flag == WAKEUP_CAN)
			{
				if(IGNOFF_MBFM_CAN_ON == 1)
				{
					IMMO_TT_WAKEUP_FLAG=1;

					if(door_statusflag == 0)
					{
						eeprom_memread_32bytes(IGNOFF_status_OFFSET,&IGNoff_status.byte, 1);
						if((IGNoff_status.bits.Rx_1) == 1)
						{
							Dooropen_active_IGNOFF = 1;
						}
						door_statusflag = 1;
					}
					if(display_active == 1)
					{
						handleDromeState();	/*function renaming*/
						handleDromeState();	/*function renaming*/
						handleDromeState();	/*function renaming*/
						/*if(ret_redraw != 0)
						{*/
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 1;
//						}
						if(display_start == 0)
						{
							display_start = 1;
							flag_grayscale = refresh_rate - 3;
						}

					}
					else
					{
						if(IGNOFF_display_timeout == 1)
						{
							handleDromeState();
							handleDromeState();
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							IGNOFF_display_timeout = 0;
						}
						else
						{
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
					}

				}
				else
				{
//					IMMO_TT_WAKEUP_FLAG = 0;
					if(IGNOFF_MBFM1_100_message_timeout_flag != 0)
					{
						/* @@@ Clearing Park Lamp and Key in Values when CAN message is absent, to play it again when Message is present  */
						parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
						buzzParkLamp = 0;
						buzzParkLampCntr = 0;
						priorityPark_Keyin = 0;
						Buzz_KIR_cntr = 0;
						KIR_Chime = 0;
						Buzz_KIR_compl = 0;

						if(IGNOFF_display_timeout == 1)
						{
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
							dromecall_timeout = 1;
							do
							{
								WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
								handleDromeState();	/*drome call till it will not receive drive summary timeout black screen(startkey = 0x11108)*/
							}while((startkey != 0x11108)&&(dromecall_timeout <= 5000));
							dromecall_timeout = 0;
							handleDromeState();
							IGNOFF_display_timeout = 0;
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
						else
						{
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
						Dooropen_IGNOFF_popup_shown=0;
						Check_EXT_Lamp_IGNOFF_popup_shown = 0;
						DOOR_OPEN_TT = 0;
						DID_TT_bit_data_set1.bits.Rx_5=0;
						DOOR_OPEN_IGN_OFF=0;
						PARK_LAMP_IGN_OFF=0;
						if((FEATURE_CODE_PARK_TT_TYPE==0)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))
						{
							PARK_LAMP_TT=0;
							PARK_LAMP_IGN_OFF=0;
							Park_lamp_TT_IGN_Off=1;
						}
						else if((FEATURE_CODE_PARK_TT_TYPE==0)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0))
						{
							PARK_LAMP_TT=0;
							PARK_LAMP_IGN_OFF=0;
							Park_lamp_TT_IGN_Off=1;
						}
						if((Dooropen_active_IGNOFF == 1)&&(All_doorclose != 1))
						{
							IGNoff_status.byte = 0x01;					/*ON WAKE UP can BUS OFF & DOOR STATUS IS OPEN THEN STORE DOOR STATUS*/
							New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
						}
						else if(Dooropen_active_IGNOFF != 1)
						{
							IGNoff_status.byte = 0x00;
							New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
						}
						door_statusflag = 0;
					}
				}
				//if(move_out_of_standby<=100)
				{
					if(TURN_TT_TIMEOUT>0)
					{
						move_out_of_standby=0;
					}
					else
					{
						move_out_of_standby++;
					}
#if 0
					TURN_RIGHT_CPU=R_GPIO_PinRead(0, 2);

					if(TURN_RIGHT_CPU == 1)
					{
						Turn_right_TT = 0xfff;
						move_out_of_standby=0;
					}
					else
					{
						move_out_of_standby++;
						Turn_right_TT = 0;
					}
					if(PORTJ_AWOJPPR0_2/*TurnLTT*/ == 1)
					{
						Turn_left_TT = 0xfff;
					}
					else
					{
						Turn_left_TT = 0;
					}
#endif
					R_TICK_WaitMS(0u, 100u);
					//SendGSData(GSdata1);
				}

				//	if(move_out_of_standby_park<=100/*10sec timeout*/)
				if(FEATURE_CODE_PARK_TT_TYPE == 1)
				{
					if(PORTJ_AWOJPPR0_1/*ParkLAMP*/ == 1)
					{

						PARK_LAMP_TT = 1;
						move_out_of_standby_park=0;
						//High_beam_TT = 0xfff;
					}
					else
					{
						PARK_LAMP_TT = 0;
						move_out_of_standby_park++;
					}


					R_TICK_WaitMS(0u, 100u);

				}
				output_shift_register();
			}


			else if((wakeup_factor_flag == WAKEUP_POSITIONLAMP)&&(FEATURE_CODE_PARK_TT_TYPE == 1))
			{
				if(move_out_of_standby_park<=100/*10sec timeout*/)
				{
					if(PORTJ_AWOJPPR0_1/*ParkLAMP*/ == 1)
					{

						PARK_LAMP_TT = 1;
						move_out_of_standby_park=0;

					}
					else
					{
						PARK_LAMP_TT = 0;
						move_out_of_standby_park++;
					}
					R_TICK_WaitMS(0u, 100u);
					//output_shift_register();
				}
				//	if(move_out_of_standby<=100)
				{
					if(TURN_TT_TIMEOUT>0)
					{
						move_out_of_standby=0;
					}
					else
					{
						move_out_of_standby++;
					}
#if 0
					TURN_RIGHT_CPU=R_GPIO_PinRead(0, 2);

					if(TURN_RIGHT_CPU == 1)
					{
						Turn_right_TT = 0xfff;
						move_out_of_standby=0;
					}
					else
					{
						move_out_of_standby++;
						Turn_right_TT = 0;
					}
					if(PORTJ_AWOJPPR0_2/*TurnLTT*/ == 1)
					{
						Turn_left_TT = 0xfff;
					}
					else
					{
						Turn_left_TT = 0;
					}
#endif
					R_TICK_WaitMS(0u, 100u);
					//SendGSData(GSdata1);

				}
				if(IGNOFF_MBFM_CAN_ON == 1)
				{
					IMMO_TT_WAKEUP_FLAG=1;
					if(door_statusflag == 0)
					{
						eeprom_memread_32bytes(IGNOFF_status_OFFSET,&IGNoff_status.byte, 1);
						if((IGNoff_status.bits.Rx_1) == 1)
						{
							Dooropen_active_IGNOFF = 1;
						}
						door_statusflag = 1;
					}
					if(display_active == 1)
					{
						handleDromeState();	/*function renaming*/
						handleDromeState();	/*function renaming*/
						handleDromeState();	/*function renaming*/
						PORT_ISOPMC17_3 = 0;
						PORT_ISOPM17_3 = 0;
						PORT_ISOP17_3 = 1;
						if(display_start == 0)
						{
							display_start = 1;
							flag_grayscale = refresh_rate - 3;
						}

					}
					else
					{
						if(IGNOFF_display_timeout == 1)
						{
							handleDromeState();
							handleDromeState();
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
							IGNOFF_display_timeout = 0;
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
						}
						else
						{
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
					}

				}
				else
				{
//					IMMO_TT_WAKEUP_FLAG = 0;
					if(IGNOFF_MBFM1_100_message_timeout_flag != 0)
					{
						/* @@@ Clearing Park Lamp and Key in Values when CAN message is absent, to play it again when Message is present  */
						parkLampChime = 0;		/* Setting Flag for Park Lamp Chime to Stop */
						buzzParkLamp = 0;
						buzzParkLampCntr = 0;
						priorityPark_Keyin = 0;
						Buzz_KIR_cntr = 0;
						KIR_Chime = 0;
						Buzz_KIR_compl = 0;
						if(IGNOFF_display_timeout == 1)
						{
#ifdef GRAYSCALE_TFT
							clear_screen();
#endif
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
							dromecall_timeout = 1;
							do
							{
								WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
								handleDromeState();	/*drome call till it will not receive drive summary timeout black screen(startkey = 0x11108)*/
							}while((startkey != 0x11108)&&(dromecall_timeout <= 5000));
							dromecall_timeout = 0;
							handleDromeState();
							handleDromeState();
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
							IGNOFF_display_timeout = 0;
						}
						else
						{
							PORT_ISOPMC17_3 = 0;
							PORT_ISOPM17_3 = 0;
							PORT_ISOP17_3 = 0;
						}
						Dooropen_IGNOFF_popup_shown=0;
						Check_EXT_Lamp_IGNOFF_popup_shown = 0;
						DOOR_OPEN_TT = 0;
						DOOR_OPEN_IGN_OFF=0;
						PARK_LAMP_IGN_OFF=0;
						DID_TT_bit_data_set1.bits.Rx_5=0;
						if((FEATURE_CODE_PARK_TT_TYPE==0)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))
						{
							PARK_LAMP_TT=0;
							PARK_LAMP_IGN_OFF=0;
							Park_lamp_TT_IGN_Off=1;
						}
						else if((FEATURE_CODE_PARK_TT_TYPE==0)&&(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0))
						{
							PARK_LAMP_TT=0;
							PARK_LAMP_IGN_OFF=0;
							Park_lamp_TT_IGN_Off=1;
						}

						if((Dooropen_active_IGNOFF == 1)&&(All_doorclose != 1))
						{
							IGNoff_status.byte = 0x01;					/*ON WAKE UP can BUS OFF & DOOR STATUS IS OPEN THEN STORE DOOR STATUS*/
							New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
						}
						else if(Dooropen_active_IGNOFF != 1)
						{
							IGNoff_status.byte = 0x00;
							New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
						}
						door_statusflag = 0;
					}
				}
				output_shift_register();
			}
			else if((wakeup_factor_flag == WAKEUP_POSITIONLAMP)&&(FEATURE_CODE_PARK_TT_TYPE == 0))
			{
				move_out_of_standby_park = 100;//to enter sleep
				move_out_of_standby = 100;
			}

			if((Dooropen_active_IGNOFF_Alert == 1)||(Checklamp_active_IGNOFF_Alert == 1)||(Keyinreminder_IGNOFF_Alert == 1))
			{
				display_active = 1;
			}

			if((IGNOFF_ExtLamp_check_done == 1)&&(IGNOFF_Dooropen_check_done == 1)&&(IGNOFF_keyinremind_check_done == 1)&&(All_doorclose_timeout_start != 1)&&(wait_state == 0)&&(startkey == 0x11108))
			{
				display_active = 0;
				dont_throw_alerts_alert_on_screen_present=0;
				IGNOFF_display_timeout = 1;
			}

			if(move_out_of_standby >= 100)
			{
				TurnLR_TT_Off = 1;

			}
			if(move_out_of_standby_park >= 100)
			{
				Park_lamp_TT_IGN_Off=1;
			}
			if((display_active == 0)&&(TurnLR_TT_Off == 1)&&(DOOR_OPEN_IGN_OFF==0)&&(IMMO_IGN_OFF==0)&&(Park_lamp_TT_IGN_Off==1)&&(PARK_LAMP_IGN_OFF==0))
			{
				break;
			}

		}


		if((IGN==0)||(IGN_ON_flag == CLEAR))
		{

			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			if((Dooropen_active_IGNOFF == 1)&&(All_doorclose != 1))
			{
				IGNoff_status.byte = 0x01;					/*ON WAKE UP can BUS OFF & DOOR STATUS IS OPEN THEN STORE DOOR STATUS*/
				New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
			}
			else if(Dooropen_active_IGNOFF != 1)
			{
				IGNoff_status.byte = 0x00;
				New_eeprom_memwrite_32bytes(IGNOFF_status_OFFSET, &IGNoff_status.byte, 1);
			}


			R_GPIO_WritePin(17, 2, 0);/*Gauge pointer illumination*/
			R_GPIO_WritePin(16, 11, 0);/*GI_CONTROL*/
			R_GPIO_WritePin(17, 0, 0);/*GI_CONTROL1*/
			R_GPIO_WritePin(17, 1, 0);/*GI_CONTROL2*/
			r_taub_OutputDisable(2,R_TAUB_CHANNEL_5);
#ifdef PCBrevB
			R_GPIO_WritePin(1, 3, 0); /* 5V_SWSC enable */

			R_GPIO_WritePin(10, 5, 0); /* TT_DRIVER */
#else
			R_GPIO_WritePin(3, 9, 0); /* 5V_SWSC enable */
			//R_GPIO_WritePin(10, 10, 1); /* CE for input shift reg*/

			R_GPIO_WritePin(3, 8, 0); /* TT_DRIVER */
#endif


			R_GPIO_WritePin(17, 6, 0);//Audio amplifier

			R_GPIO_WritePin(16, 8, 0);//can_standby
			R_GPIO_WritePin(3, 4, 1);//can_enable
			R_GPIO_WritePin(16, 8, 0);//can_standby
			PORT_ISOP17_3 = 0; /*TFT Backlight */
			/*TFT off(reset pin low)*/
			PORT_ISOP44_5 = 0;
#ifdef COLOR_TFT
			PORT_ISOP44_3 = 0;
#endif
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			R_TICK_WaitMS(0u,150u);  /* delay 150 msec */  //NEED TO CHECK
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			R_GPIO_WritePin(3, 4, 0);//can_enable
			R_GPIO_WritePin(42, 12, 1);//parallel to serial CE
			R_GPIO_WritePin(17, 11, 0); /* Fuel Control enable */
			R_GPIO_WritePin(21,1,1);/*flash disable*/

#if 1/*port to high impedance*/
			/*PORT_ISOPM1 |= 0xFFFF;
					PORT_ISOPMC1 &= 0x0000;
					PORT_ISOPIBC1 &= 0x0000;

					PORT_ISOPM3 |= 0xFFEF; // P3_4 in output mode.
					PORT_ISOPMC3 &= 0x0000;
					PORT_ISOPIBC3 &= 0x0000;*/

			PORT_ISOPM10 |= 0xFFFF;
			PORT_ISOPMC10 &= 0x0000;
			PORT_ISOPIBC10 &= 0x0000;

			PORT_ISOPM11 |= 0xFFFF;
			PORT_ISOPMC11 &= 0x0000;
			PORT_ISOPIBC11 &= 0x0000;

			PORT_ISOPM16 |= 0xFFFF;
			PORT_ISOPMC16 &= 0x0000;
			PORT_ISOPIBC16 &= 0x0000;

			/*PORT_ISOPM17 |= 0xFFFF;
					PORT_ISOPMC17 &= 0x0000;
					PORT_ISOPIBC17 &= 0x0000;*/

			PORT_ISOPM21 |= 0xFFFF;
			PORT_ISOPMC21 &= 0x0000;
			PORT_ISOPIBC21 &= 0x0000;

			PORT_ISOPM22 |= 0xFFFF;
			PORT_ISOPMC22 &= 0x0000;
			PORT_ISOPIBC22 &= 0x0000;

			PORT_ISOPM42 |= 0xEFFF;
			PORT_ISOPMC42 &= 0x0000;
			PORT_ISOPIBC42 &= 0x0000;

			PORT_ISOPM43 |= 0xFFFF;
			PORT_ISOPMC43 &= 0x0000;
			PORT_ISOPIBC43 &= 0x0000;

			PORT_ISOPM44 |= 0xFFFF;
			PORT_ISOPMC44 &= 0x0000;
			PORT_ISOPIBC44 &= 0x0000;

			PORT_ISOPM45 |= 0xFFFF;
			PORT_ISOPMC45 &= 0x0000;
			PORT_ISOPIBC45 &= 0x0000;
#endif

			protected_write(SYSPROTCMD0,SYSPROTS0,SYSIOHOLD,/*0x7E*/0x7E);/**/

			PORT_ISOPM3 |= 0xFFEF; // P3_4 in output mode.
			PORT_ISOPMC3 &= 0x0000;
			PORT_ISOPIBC3 &= 0x0000;

			PORT_ISOPM42 |= 0xEFFF;
			PORT_ISOPMC42 &= 0x0000;
			PORT_ISOPIBC42 &= 0x0000;
			__DI();
			INTC1EIRF0	=	0;
			INTC1EIRF1	=	0;
			INTC1EIRF2	=	0;
			INTC1EIRF3	=	0;
			INTC1EIRF4	=	0;
			INTC1EIRF5	=	0;
			INTC1EIRF6	=	0;
			INTC1EIRF7	=	0;
			INTC1EIRF8	=	0;
			INTC1EIRF9	=	0;
			INTC1EIRF10	=	0;
			INTC1EIRF11	=	0;
			INTC1EIRF12	=	0;
			INTC1EIRF13	=	0;
			INTC1EIRF14	=	0;
			INTC1EIRF15	=	0;
			INTC1EIRF16	=	0;
			INTC1EIRF17	=	0;
			INTC1EIRF18	=	0;
			INTC1EIRF19	=	0;
			INTC1EIRF20	=	0;
			INTC1EIRF21	=	0;
			INTC1EIRF22	=	0;
			INTC1EIRF23	=	0;
			INTC1EIRF24	=	0;
			INTC1EIRF25	=	0;
			INTC1EIRF26	=	0;
			INTC1EIRF27	=	0;
			INTC1EIRF28	=	0;
			INTC1EIRF29	=	0;
			INTC1EIRF30	=	0;
			INTC1EIRF31	=	0;

			SYSRESFC=0xFFFFFFFF;

			//R_AWOT_create();
			R_DEV_IntEnable(R_DEV_INT_P0, 1);/*IGN interrupt*/
			R_DEV_IntEnable(R_DEV_INT_P5, 1);/*CAN wakeup(INH) interrupt*/
			R_DEV_IntEnable(R_DEV_INT_P1, 1);/*CAN wakeup(INH) interrupt*/
			uint32_t val;


			if((VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==0)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==0 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)||(VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1 && VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1))/*Immo telltele config 1 for pseudo sleep*/
			{
				if((*RRAM_IMMO_PATT4 == 1)||(*RRAM_IMMO_PATT5 == 1))	/*only allow for Pattern4 & Pattern 5*/
				{
					immo_TT_config=1;
				}

			}
			//immo_TT_config=1;
			if(immo_TT_config==1)
			{
				val = (uint32_t) (~(R_STBC_WUL_INTP0_BIT1 | R_STBC_WUL_INTP1_BIT2 | R_STBC_WUL_INTAWOT0_BIT16 | R_STBC_WUL_INTP5_BIT6 |R_STBC_WUL_INTP9_BIT10 |R_STBC_WUL_INTP7_BIT8) & 0xFFFFFFFF);
			}
			else
			{
				val = (uint32_t) (~(R_STBC_WUL_INTP0_BIT1 | R_STBC_WUL_INTP1_BIT2 | R_STBC_WUL_INTP9_BIT10 | R_STBC_WUL_INTP5_BIT6 | R_STBC_WUL_INTP7_BIT8) & 0xFFFFFFFF);
				//val = (uint32_t) (~R_STBC_WUL_INTAWOT0_BIT16 & 0xFFFFFFFF);
			}


			R_DEV_WRITE_REG(32, STBC_WUFMSK0, val);
			uint32_t u32_temp;
			r_Error_t error = R_ERR_OK;
#if 1 /*RTC in deepstop*/
			u32_temp = R_DEV_ClkFrequencyHz(R_DEV_CKS_RTC);
#define EE_PCLK_CKS_MOSC_8     (8000000uL)
#define EE_CLK_SOSC_FRQ        (32768uL)
			if (u32_temp == EE_CLK_SOSC_FRQ)
			{
				/* RTC clock is using the subosc so stop the MOSC in standby */
				error += R_DEV_SetGenStopMask(R_DEV_MOSC, 0/*STBC_DISABLE_DEEPSTOP*/);
			}
			else
			{
				/* RTC clock is using the master osc so enable the MOSC in standby */
				error += R_DEV_SetGenStopMask(R_DEV_MOSC, 1/*STBC_ENABLE_DEEPSTOP*/);
			}


			/* note:
				 				        The PLL is not stopped before entering Standby this is because during
				 				        BSP init the PLL's are stopped and restarted again.  If you stop the PLL
				 				        here then anyone that uses the PLL (i.e. CPU) needs to be switched to
				 				        another clock before the PLL is stopped	otherwise the STBC will not receive
				 				        an acknowledge from it.
				 				        If the PLL is not stopped before it enters standby it may report incorrect
				 				        status after wakeup, PLL must be stopped and restarted after wake up then.
				 				        this is done in the BSP currently
			 */

			/* clocks operating in standby */
			/* sub osc - always operating */

			/* clocks stopped operating in standby */
			error += R_DEV_SetGenStopMask(R_DEV_HS_RING, 0/*STBC_DISABLE_DEEPSTOP*/);


			/* PLL0, PLL1 and 2 are configured to stop in the BSP */

			/* check all the clocks have stopped */
			if(R_ERR_OK == error)
			{
				/* do nothing */
			}
			else
			{
				/* error something went wrong */
				while(1);
			}
			/* ensure the RTC clock is enabled in the AWO area in deepstop */
#define CKSC_ARTCAD_STPM    0xFFF82418
#define RTCAD_STP_MSK       0x00000003

			R_DEV_WRITE_REG(32, CKSC_ARTCAD_STPM, RTCAD_STP_MSK);
#endif

#define CKSC_AAWOTD_STPM    0xFFF82218
			R_DEV_WRITE_REG(32, CKSC_AAWOTD_STPM, 0x01);



			//		eeRtc_Processing();
			//		protected_write(SYSPROTCMD0,SYSPROTS0,SYSSTBC0PSC,0x02);/*deep sleep*/
			//		SYSSTBC0DISTRG = 1; /* sleep mode */

			R_DEV_WRITE_REG(32, STBC_WUFC0, 0xFFFFFFFFu);
			/* set deepstop */


			/*if(R_AWOT_Start(0) != R_AWOT_ERR_OK)
										{
											while(1);
										}*/
			//R_TICK_WaitMS(0, 2500);
			for(i = 0; i < 0xffff; i++);  //NEED TO CHECK

			//SYSROSCSTPM=0x01;

			R_AWOT_Stop(0);

			//SYSWUFC0 = 0xffffffff;   // clear
			//SYSWUFMSK16 = 0;
			if(immo_TT_config==1)
			{
				my_awot_init1sec();
			}
			R_GPIO_WritePin(0, 4, 0);
			for(i = 0; i < 0xfffe; i++);   //NEED TO CHECK
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			do
			{
				R_DEV_WRITE_PROT_REG(STBC_PROTCMD0, STBC_STBC0PSC, STBC_DEEPSTOP);
			}while(SYSPROTERR == 1);



		}
}
}

uint8_t Read_RTC_Format(void)
{
	if((RTCA0CTL0 & 0x20))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void FOUR_WD_INIT_STS(void)
{
	if(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1)
	{
		if(FEATURE_CODE_4WD_LOGIC==0)
		{
			if((FOUR_WD_HIGH_CPU ==1)&&(FOUR_WD_LOW_CPU == 0))		/*4WDL*/
			{
				R_GPIO_WritePin(17, 10, 1);	//pin high for 4WD on
				TT4WH_1000ms_cmplt_flg = 1;
			}
			else if((FOUR_WD_HIGH_CPU ==0)&&(FOUR_WD_LOW_CPU == 1))	/*4WDH*/
			{
				R_GPIO_WritePin(17, 10, 1);	//pin high for 4WD on
				TT4WH_1000ms_cmplt_flg = 1;
			}
			else																				/*2WD/ANY MODE/DEFAULT*/
			{
				R_GPIO_WritePin(17, 10, 0);//pin lOW for 2WD on
				TT24WH_1000ms_cmplt_flg = 1;
			}
		}
	}

	if((FOUR_WD_HIGH_CPU ==1)&&(FOUR_WD_LOW_CPU == 0)&&(FOUR_WD_FEEDBACK_CPU==1))		/*4WDL*/
	{
		CAN_IC2_100[3].bits.Rx_6 = 0;
		CAN_IC2_100[3].bits.Rx_7 = 1;
		STS_4WD = 1;						/*For digital output status*/
	}
	else if((FOUR_WD_HIGH_CPU ==0)&&(FOUR_WD_LOW_CPU == 1)&&(FOUR_WD_FEEDBACK_CPU==1))	/*4WDH*/
	{
		CAN_IC2_100[3].bits.Rx_6 = 1;
		CAN_IC2_100[3].bits.Rx_7 = 0;
		STS_4WD = 1;						/*For digital output status*/
	}
	else																				/*2WD/ANY MODE/DEFAULT*/
	{
		CAN_IC2_100[3].bits.Rx_6 = 0;
		CAN_IC2_100[3].bits.Rx_7 = 0;
		STS_4WD = 0;						/*For digital output status*/
	}
}
//void ETM_Guage_Test(void)
//{
//	uint32_t key;
//	key = drome_getCurrentViewKey();
//	while(key == 0x23001)
//	{
//		if(Proveout_Count < PROVEOUT_UP) /*wait for 15ms x 70 (PROVEOUT_UP), which is around 1sec*/
//		{
//			display_speedometer_speed=176;
//			final_vechile_speed_gauge_raw=176*64;
//			display_tachometer_speed=6950;
//			Proveout_Count++;
//		}
//		else if(Proveout_Count < PROVEOUT_DOWN)/*wait for 15ms x 70 (PROVEOUT_DOWN), which is around 1sec*/
//		{
//			/*if((Proveout_Count == PROVEOUT_UP)&&(BATT_DISCONNECTION==1))
//				{
//					ISM0VAR4CFG0=5120000;
//					ISM0VAR4CFG1=5120000;
//				}
//			 */
//			display_speedometer_speed=0;
//			final_vechile_speed_gauge_raw=0;
//			display_tachometer_speed=0;
//			Proveout_Count++;
//			//All_Telltale_OFF();
//		}
//		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
//		if(set_key_short == 1)
//		{
//			set_key_short = 0;
//			break;
//		}
//	}
//}

