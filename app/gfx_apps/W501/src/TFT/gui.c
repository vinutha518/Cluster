/*
 ***************************************************************************************
 ***************************************************************************************
 ***
 ***     File: gui.c
 ***
 ***     Project: YMER
 ***
 ***     Copyright (C) Stoneridge Electronics AB, 1990 - $Date:: 2015#$
 ***
 ***************************************************************************************
 **************************************************************************************/
/** \file

   \ingroup    RT

   \Subject    Display handling

   \Reference  YMER SW ARCHITECTURE DESIGN

   \Abstract

   This module implements the drome task and runs the Drome graphic platform. Look in
   drappif.h to see how the functions work.


   \Version Id:

   "$Id: gui.c 592 2015-09-09 13:52:21Z STH $"


@{
 ***************************************************************************************
 */


/*
 **=====================================================================================
 ** File id for debug prints and error management
 **=====================================================================================
 */

/*#define FILE_ID   GUI_C
#ifndef GUI_DEBUG_ON
   #define DEBUG_ON  0
#else
   #define DEBUG_ON  GUI_DEBUG_ON
#endif*/
#include "dbgmacro.h"
#include "errmgr.h"
//#include "r_wm_api.h"

#define PSH(X) (*(buf++)=(X))
#define PSH1(X) (*(buf--)=(X))
#define PEEK() buf[-1]
#define POP() *(--buf) = '\0'

/*
 **=====================================================================================
 ** Imported definitions
 **=====================================================================================
 */

#include "drhost.h"
#include "gui.h"
//#include "dl.h"
#include "drome.h"
#include "druserconf.h"
#include "digitalGuage.h"
#include "parallel_to_serial.h"
#include "digi_clk.h"
#include "r_cg_timer.h"
//#include  "rtwtypes.h"
#include "my_can.h"
#include "UDSS.h"
#include "UDSS_APP.h"
#include "UDS_DTC.h"
#include "init.h"
#include "strings.h"
#include "dr7f701461.dvf.h"
#include "swsc.h"
#include "rtc.h"
#include "alert.h"
#include "fuel.h"
#include "Memmory_Map.h"
#include "ADC.h"
#include "batt.h"
#include "gearIndication.h"
#include "FOURxFOUR.h"
#include "memmory_map.h"
#include "r_cg_serial_user.h"
#ifdef SPRINT4
#include "TDMS.h"
#endif
//#include "lcddrv.h"
//#include "ostimer.h"
//#include "timeconv.h"
//#include "i2cdrv.h"
//#include "v850incs.h"

/*changed for w501*/
#ifdef COLOR_TFT
#define ROM_HMI_METADATA_APP_IMAGE_START	0x10000000 //0x1C0000
#else
#define ROM_HMI_METADATA_APP_IMAGE_START	0x1C0000
#endif
#ifdef COLOR_TFT
#define ROM_EXTFLASH_OFFSET 0x10030000	//0x220000
#else
#ifdef U350
#define ROM_EXTFLASH_OFFSET 0x260000
#else
#define ROM_EXTFLASH_OFFSET 0x260000
#endif
#endif

#define ROM_HMI_GRAPHICS_APP_IMAGE_START	0x000000

#define DROME_DB_ADDRESS          ((U32*)((U32)ROM_HMI_METADATA_APP_IMAGE_START))
#define DROME_IMG_ADDRESS         ((U32*)((U32)ROM_EXTFLASH_OFFSET + (U32)ROM_HMI_GRAPHICS_APP_IMAGE_START))
#define DROME_LANG_PACK_ADDRESS   (0ul)
/*
 **====================================================================================
 ** Public constant definitions
 **====================================================================================
 */


/*
 **====================================================================================
 ** Private type definitions
 **====================================================================================
 */

typedef enum
{
	DROME_STATE_DO_INIT,
	DROME_STATE_OFF,
	DROME_STATE_INIT_STARTED,
	DROME_STATE_RUNNING,
	DROME_STATE_REINIT
} DromeStateEnum;




enum {
	NULL_EVENT = 0,
	GAUGE_TEST = 6,
	HMI_TEST = 7,
	SOUND_TEST = 8,
	TELLTALE_TEST = 9
};


/*typedef struct {
   ButtonValueEnum button_value;
   dl_DataId button_id;
} ButtonType;*/			/*changes for w501*/

typedef void   (*NumericFnptrType)(U8 index, U32 *arg);
typedef void   (*StringFnptrType)(U8 index, U16 *arg);

/*lint -save -e754 */
#if 0
typedef struct {
	U8 dp_id;
	NumericFnptrType numeric_fnptr;
	StringFnptrType string_fnptr;
	dl_DataId dl_parameter;
	dl_DataType dl_parameter_size;
	U32 dp_max_val;
} DpType;
#endif

U8 button_value = BUTTON_NONE;
uint8_t empty_blinkpattern_flag;
uint8_t full_blinkpattern_flag;
uint8_t toggle_fuel;
uint8_t toggle_temp;
uint8_t toggle_timedots;
_Bool Dashes_colon = 0;
uint8_t Ambient_temp;
int Ambient_temp1;
uint8_t Ambient_temp2;
uint8_t codoor_index = 0;
uint8_t Hood_index = 0;
uint8_t Codriver_index = 0;
_Bool Codoor_set = 0;
_Bool driverdoor_set = 0;
_Bool Rearleftdoor_set = 0;
_Bool RearRightdoor_set = 0;
_Bool Backdoor_set = 0;
_Bool Codoor_display = 0;
_Bool Driver_display = 0;
_Bool Rearleft_display = 0;
_Bool Rearright_display = 0;
_Bool Back_display = 0;
_Bool TimeF_selection = 0;
_Bool WH_dooropen_present = 0;
uint8_t Driver_index = 0;
uint8_t Rearleft_index = 0;
uint8_t RearRight_index = 0;
uint8_t Backd_index = 0;
uint8_t RowValue = 0;
uint8_t AFE_selected = 0;
uint8_t TimeF_selected = 0;
uint8_t TempU_selected = 0;
uint8_t PAB_selected = 0;
uint8_t PAB_Enable_Yes_No_select = 0;
uint8_t PAB_Disable_Yes_No_select = 0;
int8_t Hr1_value;
int8_t Hr0_value;
int8_t Min1_value;
int8_t Min0_value;
uint8_t AMPM_value;
_Bool Timesetting_completed;
uint8_t size = 0;
uint8_t No_warning_index = 0;
uint8_t Display_alert = 0;
uint8_t filter[40/*10*//*5*/] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t temp_size;
uint8_t once_enter;
uint8_t WH_scan;
uint8_t ETM_TFT_test_index = 0;

uint32_t Trip_AC = 0;
uint32_t Trip_AC_old = 0;
uint32_t Trip_AC_temp = 0;
_Bool IGN_ON_AC_Read = 0;
_Bool Bootup_Trigger = 0;
uint8_t HANDLE_DROME_START = 0;
uint16_t AFE_display;
uint8_t STS_Eng = 0U;
extern uint16_t AFEDispVal_init;
extern _Bool set_key_short;
extern _Bool Up_key_Short;
extern _Bool Down_key_Short;
extern _Bool Speed_Missing;
/************************************/
extern _Bool NotdisplayAFE;
extern _Bool Speed_Missing;
extern uint8_t new_fuel_val;
extern uint8_t fuel_widget;
extern uint32_t IFE_Out_DISPLAY;
extern uint32_t IFE_Out;
extern uint16_t Display_Average_speed_A;
extern uint16_t Display_Average_speed_B;
extern uint32_t final_ACtripmeter_distance1;
extern uint8_t Diag_Unit_Selection;
extern uint8_t regen_req_start;

extern uint32_t DTE_Disp_Val;
extern uint8_t animation_start;
extern uint8_t	Diag_RW_IFE_Max;
extern uint8_t bufferinit_flag;
extern _Bool entry_drivesummery;
extern _Bool gui_shutdown;
/**************************************************************/
uint32_t Calculated_TripA = 0U;
uint32_t Calculated_TripB = 0U;
uint32_t Initial_TripA = 0U;
uint32_t Initial_TripB = 0U;
_Bool Initial_TripA_flag=0;
_Bool Initial_TripB_flag=0;
_Bool Arrowfill_flag = 0;
uint8_t A_flag = 0;
uint8_t U_flag = 0;
_Bool ArrowUpfill_flag = 0;
extern uint64_t start_driveSum_timeout_3sec;
extern uint8_t welcome_index;
uint8_t skip_redraw = 0;
_Bool ret_redraw = 0;
_Bool draw_viewkey;
extern uint8_t flag_grayscale;
extern uint8_t refresh_rate;
_Bool BS6_permanent_timeout = 0;
_Bool BS6_permanent_skip = 0;
uint8_t TimeSet_BackIndex = 0;
uint8_t TimeSet_ConfirmIndex = 0;
uint8_t Warning_entry = 0;
_Bool WH_Dooropen_active = 0;
_Bool SYNC_MODE = 0;
_Bool TEMP_MODE = 0;
uint8_t TEMP_FEATURE_CODE_AFE_RESET_TYPES = 0;
_Bool Setting_timeout_start = 0;
_Bool DTE_Signal_invalid = 0;
uint8_t default_page = 0;
uint8_t temp_default_page = 0;
uint8_t temp2_default_page = 0;
_Bool scroll_completed = 0;
uint32_t drome_count1us = 0;
uint8_t BSVI_errorcode = 0;
uint8_t WH_BSVI_errorcode = 2;
uint8_t WH_BS_VI_Errortext_E12 = 2;
uint8_t WH_BS_VI_Errortext_E22 = 2;
uint8_t WH_BS_VI_Errortext_E32 = 2;
uint8_t index_4X4_Text = 0;
_Bool dynamic_image_display = 0;
//extern uint8_t test_cnt;
/**************************************************************/
/*
 **====================================================================================
 ** Private function prototypes
 **====================================================================================
 */

/*Private*/ Public void handleButtonPress(void);
/*Private*/ Public void handleDromeState(void);
#if 0
Private void configureVideoIn(void);
Private void doInitVideoIn(void);
Private void enableVideoIn(void);
Private void setResetVideoIn(Boolean reset);
#endif

#if 0	/*chnaged for W501*/
Private void convertTimeToDpString(U8 index, U16 *arg);

Private void getAPG1value(U8 index, U32 *arg);
/*Private void getAPG2value(U8 index, U32 *arg);*/
Private void getAdBluevalue(U8 index, U32 *arg);
Private void getAdBlueWarning(U8 index, U32 *arg);
Private void getDisplayBrightness(U8 index, U32 *arg);
Private void getVehicleSpeed(U8 index, U32 *arg);
Private void getTelltaleStatus(U8 index, U32 *arg);
#endif	/*chnaged for W501*/
/*Private void getTelltaleStatus1(U8 index, U32 *arg);*/
/*
 **====================================================================================
 ** Private constant definitions
 **====================================================================================
 */

/*Private const ButtonType BUTTON_TABLE[] = {
   { BUTTON_UP,      dl_BUTTON_1    },
   { BUTTON_RIGHT,   dl_BUTTON_2    },
   { BUTTON_DOWN,    dl_BUTTON_3    },
   { BUTTON_LEFT,    dl_BUTTON_4    },
   { BUTTON_NONE,    dl_NULL        }
};*/	/*changes for w501*/

extern uint8_t HMI_AVS_DASH1,HMI_AVS_DASH2;

#if 0
Private const DpType DP_TABLE[] =
{
		/* {(U8)drome_ZERO, NULL, NULL, (dl_DataId)0U, (dl_DataType)0U, 0U},  */ /* Always returns zero */

		/*	{(U8)drome_Speedo_value, getVehicleSpeed, NULL, dl_SPEED, dl_TYPE_U16, 180U},

 {(U8)drome_CurrentDate, NULL, convertTimeToDpString, dl_TIME_DATE, dl_TYPE_U32, 0U},
   {(U8)drome_APG1Value, getAPG1value, NULL, dl_APG1, dl_TYPE_U8, 100U},
   {(U8)drome_APG2Value, getAPG1value, NULL, dl_APG2, dl_TYPE_U8, 100U},
   {(U8)drome_AdBlueValue, getAdBluevalue, NULL, dl_AdBlue, dl_TYPE_U8, 100U},
   {(U8)drome_AdBlueWarning_Visibility, getAdBlueWarning, NULL, dl_AdBlue, dl_TYPE_U8, 1U},
   {(U8)drome_DisplayBrightness, getDisplayBrightness, NULL, dl_DISPLAY_BRIGHTNESS, dl_TYPE_U8, 100U},
   {(U8)drome_VehicleSpeed, getVehicleSpeed, NULL, dl_SPEED, dl_TYPE_U16, 100U},
   {(U8)drome_TT_ReducedSpeed_Visiblity, getTelltaleStatus, NULL, dl_TT_CAB_TILT, dl_TYPE_2BIT, 1U},
   {(U8)drome_TT_LowAdBlue_Visiblity, getTelltaleStatus, NULL, dl_TT_CAB_TILT, dl_TYPE_2BIT, 1U},
   {(U8)drome_TT_Retarder_Visiblity, getTelltaleStatus, NULL, dl_TT_CAB_TILT, dl_TYPE_2BIT, 1U},
   {(U8)drome_TT_HighRangeGearIndicator_Visiblity, getTelltaleStatus, NULL, dl_TT_CAB_TILT, dl_TYPE_2BIT, 1U},
   {(U8)drome_TT_CabTilt_Visiblity, getTelltaleStatus, NULL, dl_TT_CAB_TILT, dl_TYPE_2BIT, 1U},*/

#if 0
		{(U8)drome_APG2Value, getAPG2value, NULL, dl_APG2, dl_TYPE_U16, 100U},
		{(U8)drome_DisplayBrightness, getDisplayBrightness, NULL, dl_DISPLAY_BRIGHTNESS, dl_TYPE_U8, 100U}
		{(U8)drome_OdoValue, getOdoValue, NULL, dl_ODO, dl_TYPE_U32, 999999U},
		{(U8)drome_TripValue, getTripValue, NULL, dl_TRIP, dl_TYPE_U32, 9999U},
		{(U8)drome_VehicleSpeed, getVehicleSpeed, NULL, dl_SPEED, dl_TYPE_U16, 120U}
#endif
};
#endif

#define SIZEOF_DP_TABLE (sizeof(DP_TABLE) / (sizeof(DpType)))

Private U16 priv_video_scaling[] =
{
		0u,   0u,   /* Scale index 0 - Unscaled */
		800u, 600u, /* Scale index 1 - SIZE H/V */
		800u, 480u, /* Scale index 2 - SIZE H/V */
		640u, 480u, /* Scale index 3 - SIZE H/V */
		400u, 240u, /* Scale index 4 - SIZE H/V */
		320u, 240u, /* Scale index 5 - SIZE H/V */
		240u, 180u, /* Scale index 6 - SIZE H/V */
		200u, 150u  /* Scale index 7 - SIZE H/V */
};
/*display time changes for Drome 3G*/
Private const r_ddb_Timing_t dispParams[]=
{"TRULY_240x320",
		240u, 320u,
		{320u, 80u, 16u, 48u},
		{338u,  18u, 6u, 6u},
		R_DDB_DISP_FLAG_NONE,
		6489600u};

#define No_of_text_list_text 		11

Private const U8 dynamic_text1[] = {"AFE Reset\0"};
Private const U8 dynamic_text2[] = {"Power Saver Mode\0"};
Private const U8 dynamic_text3[] = {"DRL Control\0"};
Private const U8 dynamic_text4[] = {"Passenger Airbag\0"};
Private const U8 dynamic_text5[] = {"Clock\0"};
Private const U8 dynamic_text6[] = {"Temperature Unit\0"};
Private const U8 dynamic_text7[] = {"Welcome Chime\0"};
Private const U8 dynamic_text8[] = {"Selective Unlock\0"};
Private const U8 dynamic_text9[] = {"Car Color Select\0"};
Private const U8 dynamic_text10[] = {"Selectable Screen\0"};
Private const U8 dynamic_text11[] = {"Back\0"};
Private const U16 * const dynamic_val_ptr[11] = {0,0,3,4,4,0,0,0,0,0,0};

Private const U8 * const dynamic_text_ptr[No_of_text_list_text] =
{
	dynamic_text1, dynamic_text2, dynamic_text3, dynamic_text4, dynamic_text5,
	dynamic_text6,dynamic_text7,dynamic_text8,dynamic_text9,dynamic_text10,dynamic_text11,
};
/*{"TOPPOLY_240x320",
      240u, 320u,
      {280u, (280-240), (280-270), (10-0)},
      {328u, (328-320), (328-324), (2-0)},
      R_DDB_DISP_FLAG_NEGCLK,
      7056000u};*/	/*No HDMI Initialise*/
/*{"NEC_320x240",
      320u, 240u,
      {404u, 84u, 7u, 36u},
      {262u, 22u, 1u,  9u},
      R_DDB_DISP_FLAG_NONE,
      7056000u};*/	 /*No HDMI initialise*/
/*{"TCON_SHARP_240x320",
    240u,320u,
    {283u,43u,31u,5u},
    {330u,10u,2u,2u},
    ((uint32_t)R_DDB_DISP_FLAG_VOEN
      | (uint32_t)R_DDB_DISP_FLAG_HSYNC_ACTHI
      | (uint32_t)R_DDB_DISP_FLAG_VSYNC_ACTHI),
    7000000u};*/ /*No HDMI initialise*/
/*{"TRULY_QVGA_TOUCH_240x320",
     240u, 320u,
     {(240+10+10+20), (10+10+20), 10u, 10u},
     {(320+ 4+ 2+ 2),  (4+ 2+ 2),  4u,  2u},
     R_DDB_DISP_FLAG_DESYNC_ACTHI,
     5640000u};*//*No HDMI initialise*/
/*{"HITACHI_QVGA_TFT_240x320",
     240u, 320u,
     {273u, 33u, 11u, 5u},
     {327u,  7u,  2u, 1u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     5356260u}; *//*No HDMI initialise*/

/*{"KOE_TFT_800_480",
     800u, 480u,
      {1056u, 256u, 40u, 128u},
      {525u,  45u, 10u, 2u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      33300000u};*/ /*HDMI initialise*/
/*{"TRULY_WVGA_TFT_800x480",
		800u,480u,
        {928,128u,40u,48u},
        {525u,45u,13u,3u},
        ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
        30000000u};*/
/*{
{"VESA_320x240_50Hz",
320u, 240u,
{800u, 160u, 16u, 96u},
{525u, 45u, 10u, 2u},
((uint32_t) R_DDB_DISP_FLAG_VOEN
| (uint32_t) R_DDB_DISP_FLAG_DESYNC_ACTHI),
25175000u}
};*/

/*
 **====================================================================================
 ** Functions for use only by Drome!
 **====================================================================================
 */

Public void gui_applicationEvent(U32 event);
Public void gui_getDPValue(U16 data_handle, U32* val_ptr, U16 argument);
Public void gui_setDPValue(U16 data_handle, U32 val, U16 argument);
Public void gui_getDPValueMax(U16 data_handle, U32* val_ptr, U16 argument);
Public void gui_getDPValueText(U16 data_handle, drome_DPStrFormat* format, U16* str_ptr, U16 str_len, U16 argument);
Public void gui_getDPValueVector(U16 data_handle, const void* set_ptr, void* dest_ptr, U16 dest_size, U16 argument);
Public void gui_loadNVData(U16 data_handle, U8* data_ptr, U16 max_size );
Public void gui_saveNVData(U16 data_handle, const U8* data_ptr, U16 size);
Public void gui_loadHistoryStatesData(U32* data_ptr, U16 max_size);
Public void gui_saveHistoryStatesData(const U32* data_ptr, U16 size);
Public void gui_getExternalText(U16 text_handle, U16 *str_ptr, U16 str_len);
Public void gui_handleDromeError(drome_ErrorCode error_code);
Public void gui_handleDromeCodeError(U32 arg1, U32 arg2);
Public void gui_loadHistoryStates(U32* arg1, U16 arg2);
Public void gui_saveHistoryStates(const U32* arg1, U16 arg2);
Public U32  gui_getUserConfChecksum(void);
Public U32  gui_getNullEvent(void);
Public U32  gui_getLastInEvent(void);
Public U32  gui_getMaxEvents(void);
Public U32 gui_getTimestamp(void);
Public U32 gui_getTimestampMS(void);
Public Boolean gui_i2cOpen(void);
Public void gui_i2cClose(void);
Public Boolean gui_i2cWriteStart(U8 adr, const U8* txbuf, U16 txnum);
Public Boolean gui_i2cReadStart(U8 adr, const U8* txbuf, U16 txnum, U8* rxbuf, U16 rxnum);
Public void gui_i2cIssueStop(void);
Public Boolean gui_getScaledVideoSize(U8 scale_index, U16* scaled_width_ref, U16* scaled_height_ref);
Public const r_ddb_Timing_t * app_getDispParams(void);
Private const r_wm_OutColorFmt_t app_getOutputColorFormat(void);
Public void gui_taskInit(void);



/*
 **====================================================================================
 ** Private variable declarations
 **====================================================================================
 */

Private DromeStateEnum priv_drome_state;
//Private U8 priv_i2c_hdl;			/*changes done for w501*/
U8 button_input = 0u;
Private drome_CallbackFunctPtrs priv_drome_CallbackFunctPtrs;

/*************************************************************************************/
Public
void
gui_taskInit(
		void)

/** Task initialization run before the os is started. Interrupts are disabled.
 ** \note gui_start() must be called to complete the start of the component.
 **************************************************************************************/
{
	//	r_wm_Error_t wmErr;
	/* Set up Drome callback functions */
	priv_drome_CallbackFunctPtrs.getOutputColorFormat = app_getOutputColorFormat;
	priv_drome_CallbackFunctPtrs.detectedDromeError = gui_handleDromeError;
	priv_drome_CallbackFunctPtrs.detectedCodeError = gui_handleDromeCodeError;
	priv_drome_CallbackFunctPtrs.setDPValAsNumeric = gui_setDPValue;
	priv_drome_CallbackFunctPtrs.getDPValAsNumeric = gui_getDPValue;
	priv_drome_CallbackFunctPtrs.getDPValAsString = gui_getDPValueText;
	priv_drome_CallbackFunctPtrs.getTimeInUs = gui_getTimestamp;
	priv_drome_CallbackFunctPtrs.getTimeInMs = gui_getTimestampMS;
	priv_drome_CallbackFunctPtrs.getUserConfChecksum = gui_getUserConfChecksum;
	/* Optional Drome callback functions */
	//  priv_drome_CallbackFunctPtrs.loadNVData = gui_loadNVData;
	//  priv_drome_CallbackFunctPtrs.saveNVData = gui_saveNVData;
	priv_drome_CallbackFunctPtrs.loadHistoryStates = gui_loadHistoryStates;
	priv_drome_CallbackFunctPtrs.saveHistoryStates = gui_saveHistoryStates;
	priv_drome_CallbackFunctPtrs.sendEventToApplication = gui_applicationEvent;
	priv_drome_CallbackFunctPtrs.getNullEvent = gui_getNullEvent;
	priv_drome_CallbackFunctPtrs.getLastInEvent = gui_getLastInEvent;
	priv_drome_CallbackFunctPtrs.getMaxEvents = gui_getMaxEvents;
	priv_drome_CallbackFunctPtrs.getDPValMaxValue = gui_getDPValueMax;
	priv_drome_CallbackFunctPtrs.getDisplayParams = app_getDispParams;
	priv_drome_CallbackFunctPtrs.getExternalText = NULL;
	priv_drome_CallbackFunctPtrs.getCTTValueVector = NULL;
	priv_drome_CallbackFunctPtrs.updateDisplay = NULL;
	priv_drome_CallbackFunctPtrs.i2cOpen = gui_i2cOpen;
	priv_drome_CallbackFunctPtrs.i2cClose = gui_i2cClose;
	priv_drome_CallbackFunctPtrs.i2cWriteStart = gui_i2cWriteStart;
	priv_drome_CallbackFunctPtrs.i2cReadStart = gui_i2cReadStart;
	priv_drome_CallbackFunctPtrs.i2cIssueStop = gui_i2cIssueStop;
	priv_drome_CallbackFunctPtrs.getScaledVideoSize = gui_getScaledVideoSize;
	/* wmErr = R_WM_ScreenColorFormatSet(FLASHDASH_WM_UNIT, (R_WM_OUTCOLORFMT_RGB666| R_WM_OUTCOLORFMT_FLAG_SWAP_BR));	// output is 666 and RGB is swapped to become BGR
       if(R_WM_ERR_OK != wmErr)
       {
          //FlashDash_Error();
       }*/
	//   lcddrv_doInit();
	priv_drome_state = DROME_STATE_DO_INIT;
}

#if 0
/*************************************************************************************/
Public
void
gui_taskEntry(
		void)

/** Task entry function
 **************************************************************************************/
{
	U8 sys_state;
	U8 ack;
	static Boolean lcd_initialized;

	(void)dl_GET_DATA(dl_SYSTEM_STATE, dl_DR_ANONYMOUS, &sys_state);
	switch (sys_state) {
	case dldef_SS_STARTUP:
		(void)lcddrv_doLcdInitSequence();
		break;

	case dldef_SS_OPERATING:
		if (!lcd_initialized) {
			lcd_initialized = lcddrv_doLcdInitSequence();
		}
		if (lcd_initialized) {
			handleDromeState();
			handleButtonPress();
		}
		break;

	case dldef_SS_LIMITED:
		break;

	case dldef_ST_STARTUP_TO_OPERATING:
	case dldef_ST_LIMITED_TO_OPERATING:
		(void)dl_GET_DATA(dl_GUI_SYS_STATE_TRANSITION_ACK, dl_DR_ANONYMOUS, &ack);
		if (ack != dldef_TRANSITION_TO_OPERATING_COMPLETE) {
			ack = dldef_TRANSITION_TO_OPERATING_COMPLETE;
			dl_PUT_DATA(dl_GUI_SYS_STATE_TRANSITION_ACK, ack);
		}
		(void)lcddrv_doLcdInitSequence();
		break;

	case dldef_ST_STARTUP_TO_LIMITED:
	case dldef_ST_OPERATING_TO_LIMITED:
		(void)dl_GET_DATA(dl_GUI_SYS_STATE_TRANSITION_ACK, dl_DR_ANONYMOUS, &ack);
		if (ack != dldef_TRANSITION_TO_LIMITED_COMPLETE) {
			ack = dldef_TRANSITION_TO_LIMITED_COMPLETE;
			dl_PUT_DATA(dl_GUI_SYS_STATE_TRANSITION_ACK, ack);
		}
		break;

	case dldef_ST_STARTUP_TO_SHUTDOWN:
	case dldef_ST_OPERATING_TO_SHUTDOWN:
	case dldef_ST_LIMITED_TO_SHUTDOWN:
		(void)dl_GET_DATA(dl_GUI_SYS_STATE_TRANSITION_ACK, dl_DR_ANONYMOUS, &ack);
		if (ack != dldef_TRANSITION_TO_SHUTDOWN_COMPLETE) {
			ack = dldef_TRANSITION_TO_SHUTDOWN_COMPLETE;
			dl_PUT_DATA(dl_GUI_SYS_STATE_TRANSITION_ACK, ack);
		}
		break;

	default:
		errmgr_CODE_ERROR(errmgr_RESET);
		break;
	}
}
#endif

/*************************************************************************************/
Public
void 
gui_setDPValue(
		U16 data_handle,  /**<[in] A handle to the data pool parameter. */
		U32 val,          /**<[in] Value to store in datapool parameter */
		U16 argument)     /**<[in] Optional argument, parameter specific meaning */

/** Function called from dromelib to set a datapool value.
 **************************************************************************************/
{
	(void)data_handle;
	(void)val;
	(void)argument;
}


/*************************************************************************************/
Public
void 
gui_getDPValueMax(
		U16 data_handle,  /**<[in] A handle to the data pool parameter. \todo U16 shall be replaced with enumeration!*/
		U32* val_ptr,     /**<[out] pointer to store data pool parameter value to */
		U16 argument)     /**<[in] Optional argument, parameter specific meaning */

/** Function called from dromelib to get a datapool maximum value
 ** \todo Do something useful in this function.
 **************************************************************************************/
{
	(void)data_handle;
	(void)argument;

	/* if((data_handle == (U8)drome_APG1Value) || (data_handle == (U8)drome_APG2Value) )
      {
	 *val_ptr = 100u;
      }*/

	if(data_handle == (U8)drome_Speedo_value)
	{
		*val_ptr = 180u;
	}

	if(data_handle == (U8)drome_Odo_value)
	{
		*val_ptr = 999999u;
	}
	if(data_handle == (U8)drome_TripA_value)
	{
		*val_ptr = 9999u;
	}
	if(data_handle == (U8)drome_TripB_value)
	{
		*val_ptr = 9999u;
	}
	if(data_handle == (U8)drome_TripAC_value)
	{
		*val_ptr = 9999u;
	}
	if(data_handle == drome_Fuel_Bargraph_value)
	{
		*val_ptr = 8u;//new_fuel_val;
	}
	if(data_handle == drome_Temp_Bargraph_value)
	{
		*val_ptr = 8u;
	}
	if(data_handle == drome_IFE_value)
	{
		//	*val_ptr = 255u;//30u;
			if(Diag_RW_IFE_Max <= 255)
				{
				*val_ptr = Diag_RW_IFE_Max;//255u;
				}
				else
				{
					*val_ptr = 255u;
				}
	}

	if(data_handle == drome_Setting_Timevalue_HR_1)
	{
			*val_ptr = 23u;
	}
	/*if(data_handle == drome_Setting_Timevalue_HR_0)
	{
			*val_ptr = 9u;
	}*/
	if(data_handle == drome_Setting_Timevalue_MIN_1)
	{
			*val_ptr = 59u;
	}
	if(data_handle == drome_DTE_value)
	{
		*val_ptr = 999u;
	}
	if(data_handle == drome_IFE_MaxDisplay_value)
	{
		if(Diag_RW_IFE_Max <= 255)
		{
		*val_ptr = Diag_RW_IFE_Max;//255u;
		}
		else
		{
			*val_ptr = 255u;
		}
	}
	if(data_handle == drome_IFE_MidDisplay_value)
	{
		*val_ptr = Diag_RW_IFE_Max/2;//128u;
	}

	if(data_handle == drome_AFE_value)
	{
		*val_ptr = 999;//128u;
	}


	/*if(data_handle == drome_Setting_Timevalue_MIN_0)
	{
			*val_ptr = 9u;
	}*/


}

/*************************************************************************************/
Private
Boolean
findIndexDpTable(
		U16 data_handle,
		U8 *index)

/** Function to get find index in datapool table
 **************************************************************************************/
{
	U8 i = 0u;
	Boolean valid_dp_parameter = FALSE;

	/* Find index of table. */
#if 0	/*changes done for w501*/ 
	while ((i <= (SIZEOF_DP_TABLE-1u)) && (!valid_dp_parameter))
	{
		if (DP_TABLE[i].dp_id == data_handle)
		{
			valid_dp_parameter = TRUE;
			*index = i;
		}
		else
		{
			i++;
		}
	}

	return valid_dp_parameter;
#endif   
}


#pragma optimize=none 				/*lint !e975*/
/*************************************************************************************/
Public
void 
gui_getDPValue(
		U16 data_handle,  /**<[in] A handle to the data pool parameter. \todo U16 shall be replaced with enumeration!*/
		U32* val_ptr,     /**<[out] pointer to store data pool parameter value to */
		U16 argument)     /**<[in] Optional argument, parameter specific meaning */

/** Function called from dromelib to get a datapool value
 **************************************************************************************/
{
	U8 index = 0u;
	(void)argument;
	*val_ptr = 0UL;

	if (data_handle < (U8)(drome_MAX_DATAPOOL))
	{
		if(data_handle == drome_MT_AT_value)
		{
			if(IO_Parameter_Request_Status !=3)
			{
				val_ptr[0] = gear_to_HMI;
				//val_ptr[1] = '\0';
			}
			else
			{
				val_ptr[0] = IO_Control_Gear_indication();
#if 0
				if(Gear_config == 1)//MT
				{
					if(((IO_Control_Enable_Gear_IND.byte & 0x0F) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x00))
					{
						val_ptr[0] = 0;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x01))
					{
						val_ptr[0] = 1;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x02))
					{
						val_ptr[0] = 2;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x03))
					{
						val_ptr[0] = 3;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x04))
					{
						val_ptr[0] = 4;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x05))
					{
						val_ptr[0] = 5;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x06))
					{
						val_ptr[0] = 6;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x07))
					{
						val_ptr[0] = 7;
					}
				}

				if(Gear_config == 2)
				{
					if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x08))
					{
						val_ptr[0] = 8;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x09))
					{
						val_ptr[0] = 9;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x0A))
					{
						val_ptr[0] = 10;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x0B))
					{
						val_ptr[0] = 11;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x0C))
					{
						val_ptr[0] = 12;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x0D))
					{
						val_ptr[0] = 13;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x0E))
					{
						val_ptr[0] = 14;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x0F))
					{
						val_ptr[0] = 15;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x10))
					{
						val_ptr[0] = 16;
					}
					else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x11))
					{
						val_ptr[0] = 17;
					}
				}
				else if(((IO_Control_Enable_Gear_IND.byte & 0xFF) ==0x01)&&((IO_Control_State_Gear_IND_Status_Byte0.byte & 0xFF) == 0x12))
				{
					val_ptr[0] = 18;
				}
				else
				{
					val_ptr[0] = 55;
				}
#endif
			}
		}
		if(data_handle == drome_Gear_shift_value)
		{
			if(IO_Parameter_Request_Status !=3)
			{
				val_ptr[0] = gear_shift_to_HMI;
				//val_ptr[1] = '\0';
			}
			else
			{
				val_ptr[0] = IO_Control_Gear_shift_indication();

#if 0
				if(((IO_Control_Enable_Gear_IND.byte & 0xFF))&&((IO_Control_State_Gear_IND_Status_Byte1.byte & 0x01) == 0x01))
				{
					val_ptr[0] = 0;//shift Up
				}
				else if(((IO_Control_Enable_Gear_IND.byte & 0xFF))&&((IO_Control_State_Gear_IND_Status_Byte1.byte & 0x02) == 0x02))
				{
					val_ptr[0] = 1;//shift down
				}
				else
				{
					val_ptr[0] = 2;//Shift Blank
				}
#endif
			}
		}
		if(data_handle == drome_Avg_Speed_logo_value)
		{
			val_ptr[0] = 0;
			//val_ptr[1] = '\0';
		}
		if(data_handle == drome_Driving_Time_logo_value)
		{
			val_ptr[0] = 0;
			//val_ptr[1] = '\0';
		}
		if(data_handle == drome_Time_Notation_logo_value)
		{
			val_ptr[0] = 0;
			//val_ptr[1] = '\0';
		}
		if(data_handle == drome_Avg_Speed_Unit_value)
		{
			val_ptr[0] = 0;
			//val_ptr[1] = '\0';
		}
		if(data_handle == drome_Driving_Time_Unit_value)
		{
			val_ptr[0] = 0;
			//val_ptr[1] = '\0';
		}
		if(data_handle == drome_FEATURE_CODE_REGEN_KB_DOWN)
		{
//			if(FEATURE_CODE_REGEN == 1)
			if((REGEN_TT_ON_flag == 1)||(REGEN_TT_ON_flag == 2))
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
			//val_ptr[1] = '\0';
		}
		if(data_handle == drome_Page_scrolling_value)
		{
//			if(FEATURE_CODE_REGEN == 1)
			if(REGEN_TT_ON_flag == 1)
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
			//val_ptr[1] = '\0';
		}

		if(data_handle == drome_Empty_Fuel)
		{
			val_ptr[0] = toggle_fuel;
		}

		if(data_handle == drome_Full_Fuel)
		{
			val_ptr[0] = toggle_fuel;

		}
		if((data_handle == drome_Full_temp)||(data_handle == drome_conditional_barblink))
		{
			val_ptr[0] = toggle_temp;

		}
		if((data_handle == drome_Digital_Clock_Dots_value)||(data_handle == drome_Time_Notation_logo_value))
		{
			if(Dashes_colon == 1)	//GG-131/1
			{
				val_ptr[0] = 1;
			}
			else
			{

				val_ptr[0] = toggle_timedots;
			}

		}
		if(data_handle == drome_AMPM_Unit_value)
		{
			if(Dashes_colon == 1)	//GG-131/1
			{
				val_ptr[0] = 2;
			}
			else
			{
				val_ptr[0] = AM_flag;
			}
		}

		if(data_handle == drome_Fuel_Bargraph_value)
		{
		if(IO_Parameter_Request_Status !=3)
			{
				val_ptr[0] = fuel_widget;//new_fuel_val;
			}
		else
			{
			if((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)
			{
				if(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) <= 0x08)
				{
					fuel_widget =((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4);//need to check impact
					val_ptr[0] = fuel_widget;
				}
				else
				{
					val_ptr[0] = 8;
				}
			}
			else
			{
				val_ptr[0] = 0;
			}
#if 0
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x00))
				{
					val_ptr[0] = 0;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x01))
				{
					val_ptr[0] = 1;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x02))
				{
					val_ptr[0] = 2;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x03))
				{
					val_ptr[0] = 3;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x04))
				{
					val_ptr[0] = 4;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x05))
				{
					val_ptr[0] = 5;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x06))
				{
					val_ptr[0] = 6;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x07))
				{
					val_ptr[0] = 7;
				}
			if(((IO_Control_Enable_Bar_Graph.byte & 0x02) ==0x02)&&(((IO_Control_State_Bar_Graph_Status.byte & 0xF0)>>4) == 0x08))
				{
					val_ptr[0] = 8;
				}
#endif
			}
		}

		if(data_handle == drome_Temp_Bargraph_value)
		{
		if(IO_Parameter_Request_Status !=3)
			{	
				val_ptr[0] = temp_widget;//new_temp_value;
			}
		else
			{

				if((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)
				{
					if(((IO_Control_State_Bar_Graph_Status.byte & 0x0F)>>4) <= 0x08)
					{
						temp_widget = (IO_Control_State_Bar_Graph_Status.byte & 0x0F);//need to check impact
						val_ptr[0] = temp_widget;
					}
					else
					{
						val_ptr[0] = 8;
					}
				}
				else
				{
					val_ptr[0] = 0;
				}
#if 0
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F) == 0x00))
				{
					val_ptr[0] = 0;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x01))
				{
					val_ptr[0] = 1;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x02))
				{
					val_ptr[0] = 2;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x03))
				{
					val_ptr[0] = 3;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x04))
				{
					val_ptr[0] = 4;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x05))
				{
					val_ptr[0] = 5;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x06))
				{
					val_ptr[0] = 6;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x07))
				{
					val_ptr[0] = 7;
				}
				if(((IO_Control_Enable_Bar_Graph.byte & 0x01) ==0x01)&&((IO_Control_State_Bar_Graph_Status.byte & 0x0F)  == 0x08))
				{
					val_ptr[0] = 8;
				}
#endif
			}
		}
		if(data_handle == drome_IFE_value)
		{
			val_ptr[0] = IFE_Out_DISPLAY;
		}
#if 1
		/********Fuel computer page handled through feature code*********************************/
		if(data_handle == drome_Feature_Code_AFE_IFE)
		{
			if((FEATURE_CODE_AFE_IND == 1) && (FEATURE_CODE_IFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				val_ptr[0] = 1;
			}
			else if((FEATURE_CODE_AFE_IND == 1) && (FEATURE_CODE_IFE_IND == 0) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				val_ptr[0] = 2;
			}
			else if((FEATURE_CODE_AFE_IND == 0) && (FEATURE_CODE_IFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				val_ptr[0] = 3;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}
		if(data_handle == drome_Feature_Code_DTE)
		{

//			val_ptr[0] = 0;
#if 1
			if((FEATURE_CODE_DTE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (LVSD_flag == 0U) && (HVSD_flag == 0U))
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
#endif
		}
		if(data_handle == drome_Feature_Code_EcoScore)
		{

			val_ptr[0] = 0;
#if 0
			if(FEATURE_CODE_EcoScore == 1)
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
#endif
		}
		if(data_handle == drome_Feature_Code_TripA_AvgSpeed_Drivetime)
		{
			if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1)	&& (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_DRIVING_TIME_CONFIG == 1))
			{
				val_ptr[0] = 1;
			}
			else if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				val_ptr[0] = 2;
			}
			else if(FEATURE_CODE_DRIVING_TIME_CONFIG == 1)
			{
				val_ptr[0] = 3;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}
		if(data_handle == drome_Feature_Code_TripB_AvgSpeed_Drivetime)
		{
			if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1)	&& (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (FEATURE_CODE_DRIVING_TIME_CONFIG == 1))
			{
				val_ptr[0] = 1;
			}
			else if((FEATURE_CODE_AVERAGE_SPEED_CONFIG == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				val_ptr[0] = 2;
			}
			else if(FEATURE_CODE_DRIVING_TIME_CONFIG == 1)
			{
				val_ptr[0] = 3;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}
		if(data_handle == drome_Feature_Code_Digital_Speedo)
		{

			if((VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 1)&&(VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 0)&&(FEATURE_CODE_DIGITAL_SPEED_CONFIG == 1))
			{
				val_ptr[0] = 1;
			}
			else if((VAR_CODING_ANTILOCK_BRAKING_SYSTEM == 0)&&(VAR_CODING_ELECTRONICS_STABILITY_CONTROL == 1)&&(FEATURE_CODE_DIGITAL_SPEED_CONFIG == 1))
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}
		if(data_handle == drome_Setting_Speed_present)
		{
			/*if((vehicle_moving_state == 1)&&(Speed_Missing == 0)&&(startkey != 0x22023))//speed present = 1 && MSG_Missing=0		//(final_vechile_speed_gauge_raw >  0x40)
			{
				val_ptr[0] = 1;
			}
			else if(startkey != 0x22023)
			{
				val_ptr[0] = 0;
			}*/
			val_ptr[0] =Speed_present;
		}
		if(data_handle == drome_Feature_Code_Trip_AC)	/*Trip AC on feature code*/
		{
			if(FEATURE_CODE_AC_TRIP_METER == 1)
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}

		if(data_handle == drome_Diag_Unit_Selection)	/*Unit Selection*/
		{
			if(Diag_Unit_Selection == 0)
				val_ptr[0] = 0;
			else if(Diag_Unit_Selection == 1)
				val_ptr[0] = 1;
			else if(Diag_Unit_Selection == 2)
				val_ptr[0] = 2;
			else if(Diag_Unit_Selection == 3)
				val_ptr[0] = 3;
			else
				val_ptr[0] = 0;
		}

		if(data_handle == drome_AFE_row_selection)	/*Unit Selection*/
		{
			val_ptr[0] = AFE_selected;
		}

		if(data_handle == drome_AMPM_UnitSet_value)	/*AM PM Selection*/
		{
			val_ptr[0] = AMPM_value;
		}
		if(data_handle == drome_No_warning_index)
		{
			val_ptr[0] = No_warning_index;
		}

		if(data_handle == drome_WH_Page1_imageIndex)	/*Warning History_Page1*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}

			val_ptr[0] = filter[0];

			if(val_ptr[0] == 20)
						{
							WH_dooropen_present = 1;


						}
						else
						{
							WH_dooropen_present = 0;

						}
			Warning_History_dooropen();

			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
			{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
			}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}
			/*if(val_ptr[0] == 20)						//DEF ERROR CODE in warning history
			{
				Warning_History_dooropen();
			}
			else
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}*/

		}
		if(data_handle == drome_WH_Page2_imageIndex)	/*Warning History_Page2*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}

			val_ptr[0] = filter[1];

			if(val_ptr[0] == 20)
						{
							WH_dooropen_present = 1;


						}
						else
						{
							WH_dooropen_present = 0;

						}
			Warning_History_dooropen();

			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
			{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
			}

			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}

			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}


		}
		if(data_handle == drome_WH_Page3_imageIndex)	/*Warning History_Page3*/
		{

			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}

			val_ptr[0] = filter[2];
			if(val_ptr[0] == 20)
						{
							WH_dooropen_present = 1;


						}
						else
						{
							WH_dooropen_present = 0;

						}
			Warning_History_dooropen();
			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
			{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
			}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}



		}
		if(data_handle == drome_WH_Page4_imageIndex)	/*Warning History_Page4*/
		{

			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}

			val_ptr[0] = filter[3];

			if(val_ptr[0] == 20)
						{
							WH_dooropen_present = 1;


						}
						else
						{
							WH_dooropen_present = 0;

						}
			Warning_History_dooropen();

			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
			{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
			}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}


		}
		if(data_handle == drome_WH_Page5_imageIndex)	/*Warning History_Page5*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}
			val_ptr[0] = filter[4];

			if(val_ptr[0] == 20)
						{
							WH_dooropen_present = 1;


						}
						else
						{
							WH_dooropen_present = 0;

						}
			Warning_History_dooropen();
			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
			{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
			}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}

		}
		if(data_handle == drome_WH_Page6_imageIndex)	/*Warning History_Page5*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}
			val_ptr[0] = filter[5];

			if(val_ptr[0] == 20)
			{
				WH_dooropen_present = 1;
			}
			else
			{
				WH_dooropen_present = 0;
			}
			Warning_History_dooropen();
			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
					{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
					}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}

		}
		if(data_handle == drome_WH_Page7_imageIndex)	/*Warning History_Page5*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}
			val_ptr[0] = filter[6];

			if(val_ptr[0] == 20)
			{
				WH_dooropen_present = 1;


			}
			else
			{
				WH_dooropen_present = 0;

			}
			Warning_History_dooropen();

			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
					{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
					}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}

		}
		if(data_handle == drome_WH_Page8_imageIndex)	/*Warning History_Page5*/
		{	if(WH_dooropen_present == 0)						/*door open alert in warning history*/
		{
			WH_Dooropen_active = 0;
			(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
		}
		val_ptr[0] = filter[7];

		if(val_ptr[0] == 20)
		{
			WH_dooropen_present = 1;

		}
		else
		{
			WH_dooropen_present = 0;
		}
		Warning_History_dooropen();

		if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
		{
			if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
			{
				WH_BSVI_errorcode = 1;			/*- - -*/
			}
			else
			{
				WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
			}
			dynamic_image_display = 1;
		}
		else if(dynamic_image_display == 1)
		{
			dynamic_image_display = 0;
			WH_BSVI_errorcode = 2;				/*Patch*/
		}
		if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
		{
			if(WH_BSVI_errorcode == 1)
			{
				WH_BS_VI_Errortext_E32 = 1;
			}
			else
			{
				WH_BS_VI_Errortext_E32 = 0;
			}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}

		}
		if(data_handle == drome_WH_Page9_imageIndex)	/*Warning History_Page5*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}
			val_ptr[0] = filter[8];

			if(val_ptr[0] == 20)
			{
				WH_dooropen_present = 1;
			}
			else
			{
				WH_dooropen_present = 0;
			}
			Warning_History_dooropen();

			if((val_ptr[0] == 8)||(val_ptr[0] == 9)||(val_ptr[0] == 13)||(val_ptr[0] == 17))						/*DEF ERROR CODE in warning history*/
			{
				if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
				{
					WH_BSVI_errorcode = 1;			/*- - -*/
				}
				else
				{
					WH_BSVI_errorcode = 0;			/*Transperant image to show value*/
				}
				dynamic_image_display = 1;
			}
			else if(dynamic_image_display == 1)
			{
				dynamic_image_display = 0;
				WH_BSVI_errorcode = 2;				/*Patch*/
			}
			if(val_ptr[0] == 9)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E32 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E32 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E32 = 2;
			}

			if(val_ptr[0] == 13)						/*DEF ERROR CODE in warning history*/
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E12 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E12 = 0;
				}

			}
			else
			{
				WH_BS_VI_Errortext_E12 = 2;
			}

			if(val_ptr[0] == 17)
			{
				if(WH_BSVI_errorcode == 1)
				{
					WH_BS_VI_Errortext_E22 = 1;
				}
				else
				{
					WH_BS_VI_Errortext_E22 = 0;
				}
			}
			else
			{
				WH_BS_VI_Errortext_E22 = 2;
			}
		}
		if(data_handle == drome_WH_Page10_imageIndex)	/*Warning History_Page5*/
		{
			if(WH_dooropen_present == 0)						/*door open alert in warning history*/
			{
				WH_Dooropen_active = 0;
				(void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);
			}
			val_ptr[0] = filter[9];

			if(val_ptr[0] == 20)
			{
				WH_dooropen_present = 1;
			}
			else
			{
				WH_dooropen_present = 0;
			}

			Warning_History_dooropen();

		}

		if(data_handle == drome_WH_Page11_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[10];
		}

		if(data_handle == drome_WH_Page12_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[11];
		}

		if(data_handle == drome_WH_Page13_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[12];

		}

		if(data_handle == drome_WH_Page14_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[13];

		}

		if(data_handle == drome_WH_Page15_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[14];

		}

		if(data_handle == drome_WH_Page16_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[15];

		}

		if(data_handle == drome_WH_Page17_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[16];

		}

		if(data_handle == drome_WH_Page18_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[17];

		}

		if(data_handle == drome_WH_Page19_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[18];

		}


		if(data_handle == drome_WH_Page20_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[19];

		}

		if(data_handle == drome_WH_Page21_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[20];

		}

		if(data_handle == drome_WH_Page22_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[21];

		}

		if(data_handle == drome_WH_Page23_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[22];

		}

		if(data_handle == drome_WH_Page24_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[23];

		}

		if(data_handle == drome_WH_Page25_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[24];

		}

		if(data_handle == drome_WH_Page26_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[25];
		}

		if(data_handle == drome_WH_Page27_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[26];

		}


		if(data_handle == drome_WH_Page28_imageIndex)	/*Warning History_Page5*/
		{
			val_ptr[0] = filter[27];
		}


		if(data_handle == drome_Next_WH_Pages)				/*Warning History_Page5*/
		{
			val_ptr[0] = size;
		}

		if(data_handle == drome_Temp_Unit_CF_value)			/*degC/degF*/
		{
			val_ptr[0] = TempU_selected;
		}

		if(data_handle == drome_ETM_TFT_Test_Index)			/*TFT_Test*/
		{
			val_ptr[0] = ETM_TFT_test_index;
		}
		/**********/
		if(data_handle == drome_RPAS_RLeft_imageIndex)		/*RPAS_RLeft*/
		{
			val_ptr[0] = RPAS_RLeft_imageIndex;
		}

		if(data_handle == drome_RPAS_RCenter_imageIndex)	/*RPAS_RCenter*/
		{
			val_ptr[0] = RPAS_RCenter_imageIndex;
		}

		if(data_handle == drome_RPAS_LCenter_imageIndex)	/*RPAS_LCenter*/
		{
			val_ptr[0] = RPAS_LCenter_imageIndex;
		}

		if(data_handle == drome_RPAS_RRight_imageIndex)		/*RPAS_RRight*/
		{
			val_ptr[0] = RPAS_RRight_imageIndex;
		}

		if(data_handle == drome_RPAS_All_sensor_error)		/*RPAS_RRight*/
		{
			val_ptr[0] = RPAS_All_sensor_error;
		}
		if(data_handle == drome_RPAS_DIST_text_display)		/*RPAS_RRight*/
		{
			val_ptr[0] = RPAS_DIST_text_display;
		}

		if(data_handle == drome_RPAS_sect_zone_calb)	/*RPAS_sect_zone calb factor*/
		{
			if((Diag_RW_RPAS_Sector_Zone_Calibration <= 4)&&(Diag_RW_RPAS_Sector_Zone_Calibration > 0))
			{
				val_ptr[0] = Diag_RW_RPAS_Sector_Zone_Calibration;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}
		/***************/
		if(data_handle == drome_Feature_Code_AFER_PAB)
		{
			TEMP_FEATURE_CODE_AFE_RESET_TYPES = 0;	/*As PER CR, AFE AUTO RESET PERMENANTLY DISABLE FROM MENU ITEMS - 28-05-20*/
			if((FEATURE_CODE_CLOCK_CONFIG==1) && (VARIANT_CODING_INFOTAINMENT_SYSTEM==1))
			{
				SYNC_MODE = 0;		/*- based on Infotainment*/
			}
			else if((FEATURE_CODE_CLOCK_CONFIG==1) && (VARIANT_CODING_INFOTAINMENT_SYSTEM==0))
			{
				SYNC_MODE = 0;		/*- based on Infotainment*/
			}
			else
			{
				SYNC_MODE = 1;		/*Standalone - based on Setting*/
			}

			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1)&&(FEATURE_CODE_OAT_CONFIG == 1))
			{
				TEMP_MODE = 1;		/*Temp option available in setting*/
			}
			else
			{
				TEMP_MODE = 0;
			}

			if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 1)&&(TEMP_MODE == 1))		/*AFE Reset + CLOCK + PAB + Temp only*/
			{
				val_ptr[0] = 1;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 1)&&(TEMP_MODE == 1))	/*CLOCK + PAB + Temp only*/
			{
				val_ptr[0] = 2;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 1)&&(TEMP_MODE == 1))	/*AFE Reset + CLOCK + PAB + Temp only*/
			{
				val_ptr[0] = 3;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 1)&&(TEMP_MODE == 1))	/*CLOCK+ Temp only*/
			{
				val_ptr[0] = 4;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 0)&&(TEMP_MODE == 1))	/*AFE Reset + PAB + Temp only*/
			{
				val_ptr[0] = 5;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 0)&&(TEMP_MODE == 1))	/*PAB + Temp only*/
			{
				val_ptr[0] = 6;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 0)&&(TEMP_MODE == 1))	/*AFE Reset + Temp only*/
			{
				val_ptr[0] = 7;
			}

			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 0)&&(TEMP_MODE == 1))	/*Temp only*/
			{
				val_ptr[0] = 8;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 1)&&(TEMP_MODE == 0))	/*Clock only*/
			{
				val_ptr[0] = 9;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 0)&&(TEMP_MODE == 0))	/*PAB only*/
			{
				val_ptr[0] = 10;
			}

			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 0)&&(TEMP_MODE == 0))	/*AFE Reset only*/
			{
				val_ptr[0] = 11;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 0)&&(SYNC_MODE == 1)&&(TEMP_MODE == 0))	/*AFE Reset + Clock only*/
			{
				val_ptr[0] = 12;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 0)&&(TEMP_MODE == 0))	/*AFE Reset + PAB only*/
			{
				val_ptr[0] = 13;
			}

			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 0)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 1)&&(TEMP_MODE == 0))	/*CLOCK + PAB only*/
			{
				val_ptr[0] = 14;
			}
			else if((TEMP_FEATURE_CODE_AFE_RESET_TYPES == 1)&&(FEATURE_CODE_PAB_SETTING == 1)&&(SYNC_MODE == 1)&&(TEMP_MODE == 0))	/*AFE Reset + CLOCK + PAB only*/
			{
				val_ptr[0] = 15;
			}

			else
			{
				val_ptr[0] = 16;		/*Only Back option*/
			}


		}

		if(data_handle == drome_PAB_Disable_select_Index)
		{
			val_ptr[0] = PAB_Disable_Yes_No_select;
		}

		if(data_handle == drome_PAB_Enable_select_Index)
		{
			val_ptr[0] = PAB_Enable_Yes_No_select;
		}

		if(data_handle == drome_Regen_page_index)
		{
			val_ptr[0] = Main_page_Regen;
		}
		if(data_handle == drome_REGEN_HoldOk_imageIndex)
		{
			val_ptr[0] = HoldOk_Regen_text;
		}
		if(data_handle == drome_Odoerror_imageIndex)
		{
			val_ptr[0] = odo_error_flag;
		}

		if(data_handle == drome_Odoerror_TripA_imageIndex)
		{
			val_ptr[0] = odo_error_flag;
		}

		if(data_handle == drome_Odoerror_TripB_imageIndex)
		{
			val_ptr[0] = odo_error_flag;
		}
		if(data_handle == drome_Aerrow_Down_Index)
		{
			val_ptr[0] = Arrowfill_flag;
		}
		if(data_handle == drome_Aerrow_UP_Index)
		{
			val_ptr[0] = ArrowUpfill_flag;
		}
		if(data_handle == drome_Welcome_imageIndex)
		{
			val_ptr[0] = welcome_index;
		}
		if(data_handle == drome_TimeSet_BackIndex)
		{
			val_ptr[0] = TimeSet_BackIndex;
		}
		if(data_handle == drome_TimeSet_ConfirmIndex)
		{
			val_ptr[0] = TimeSet_ConfirmIndex;
		}

		/*if(data_handle == drome_Time_SHr_Zeroconcat_Index)
		{
			val_ptr[0] = Hr_zeroconcat;
		}

		if(data_handle == drome_Time_SMin_Zeroconcat_Index)
		{
			val_ptr[0] = Min_zeroconcat;
		}*/
		if(data_handle == drome_IOcontrol_TFT_Index)
		{
			val_ptr[0] = IO_Control_TFT;
		}
		if((data_handle == drome_Default_page)/*&&(scroll_completed == 1)*/)
		{
			val_ptr[0] = default_page;
			scroll_completed = 0;
		}

		if(data_handle == drome_BSVI_ImageIndex)
		{
			val_ptr[0] = BSVI_errorcode;
		}

		if(data_handle == drome_WH_BSVI_Errorcode)
		{
			val_ptr[0] = WH_BSVI_errorcode;
		}

		if(data_handle == drome_BS_VI_E12E13)
		{
			val_ptr[0] = WH_BS_VI_Errortext_E12;
		}
		if(data_handle == drome_BS_VI_E22E23)
			{
				val_ptr[0] = WH_BS_VI_Errortext_E22;
			}
		if(data_handle == drome_BS_VI_E32E33)
			{
				val_ptr[0] = WH_BS_VI_Errortext_E32;
			}

		if(data_handle == drome_4WD_Framerate)
			{
				val_ptr[0] = var_4x4_frameRate;
			}
		if(data_handle == drome_4WD_Visiblity)
		{
			val_ptr[0] = var_4x4_4WD_visible;
		}
		if(data_handle == drome_2WD_Visiblity)
		{
			val_ptr[0] = var_4x4_2WD_visible;
		}

		if(data_handle == drome_4X4_Text_Index)
		{
			val_ptr[0] = index_4X4_Text;
		}

		if(data_handle == drome_VI_2WD_Visiblity)
		{
			val_ptr[0] = VI_2WD_Visiblity;
		}

		if(data_handle == drome_VI_4WD_Visiblity)
		{
			val_ptr[0] = VI_4WD_Visiblity;
		}

		if(data_handle == drome_VI_4WD_Framerate)
				{
					val_ptr[0] = VI_4WD_Framerate;
				}

		if(data_handle == drome_Feature_Code_VI_4X4)		/*4WD main screen ENABLE / DISABLE*/
		{
			if((FEATURE_CODE_DISPLAY_4WD == 1)&&(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1))
			{
				val_ptr[0] = 1;
#ifdef COLOR_TFT
				default_page = 27;	/*Required for sprint4 implementation */
#endif
			}
			else
			{
				val_ptr[0] = 0;
			}
		}

		if(data_handle == drome_Menu_Bar_FC_Index)		/*Iconbar (vehicle info / media) ENABLE / DISABLE on */
		{
#ifdef SPRINT4
if(((FEATURE_CODE_DISPLAY_4WD == 1)&&(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1))||(FEATURE_CODE_TDMS_DISPLAY == 1))
#else
			if((FEATURE_CODE_DISPLAY_4WD == 1)&&(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1))
#endif			
			{
				val_ptr[0] = 1;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}


//#ifdef COLOR_TFT
		if(data_handle == drome_Door_Codriver_Index)
		{
			if(Codoor_display == 1)
			{
			val_ptr[0] = Codriver_index;//codoor_index;
			}
			else
			{
				val_ptr[0] = 4;
			}
		}

		if(data_handle == drome_Door_Driver_Index)
		{
			if(Driver_display == 1)
			{
				val_ptr[0] = Driver_index;//codoor_index;
			}
			else
			{
				val_ptr[0] = 4;
			}
		}
		if(data_handle == drome_Door_RearLeft_Index)
		{
			if(Rearleft_display == 1)
			{
				val_ptr[0] = Rearleft_index;//codoor_index;
			}
			else
			{
				val_ptr[0] = 4;
			}
		}
		if(data_handle == drome_Door_RearRight_Index)
		{
			if(Rearright_display == 1)
			{
				val_ptr[0] = RearRight_index;//codoor_index;
			}
			else
			{
				val_ptr[0] = 4;
			}
		}
		if(data_handle == drome_Door_Back_Index)
		{
			if(Back_display == 1)
			{
				val_ptr[0] = Backd_index;//codoor_index;
			}
			else
			{
				val_ptr[0] = 4;
			}
		}
		if(data_handle ==  drome_Door_Hood_Index)
		{
			val_ptr[0] = Hood_index;//codoor_index;
		}
#ifdef GRAYSCALE_TFT
		if(data_handle ==  drome_Bootup_Trigger)
		{
			val_ptr[0] = Bootup_Trigger;	/*Bootup_Trigger = 1 ->Boot up animation start*/
		}
#endif
#ifdef SPRINT4
		/* used for TDMS */
		if(data_handle ==  drome_Feature_Code_TDMS)
		{
			if(FEATURE_CODE_TDMS_DISPLAY == 1)
			{
				val_ptr[0] = 1;
				default_page = 32;
			}
			else
			{
				val_ptr[0] = 0;
			}
		}
		
		if(data_handle ==  drome_TDMS_Error_Code)
		{
			val_ptr[0] = TDMS_Error_Code;
		}
		if(data_handle ==  drome_TDMS_Rotation_Angle)
		{
			val_ptr[0] = TDMS_Rotation_Angle;
		}
		if(data_handle ==  drome_TDMS_Turn_Side)
		{
			val_ptr[0] = TDMS_Turn_Side;
		}
		/* Used mainly for Center level */
		if(data_handle ==  drome_TDMS_Alert_Zero_Degree_imageIndex)
		{
			val_ptr[0] = TDMS_Alert_Zero_Degree_imageIndex;
		}
		if(data_handle ==  drome_TDMS_MainScreen_Zero_Degree_imageIndex)
		{
			val_ptr[0] = TDMS_MainScreen_Zero_Degree_imageIndex;
		}

		if(data_handle ==  drome_TDMS_Left_Wheel_X)
		{
			val_ptr[0] = TDMS_Left_Wheel_X;
		}

		if(data_handle ==  drome_TDMS_Left_Wheel_Y)
		{
			val_ptr[0] = TDMS_Left_Wheel_Y;
		}

		if(data_handle ==  drome_TDMS_Right_Wheel_X)
		{
			val_ptr[0] = TDMS_Right_Wheel_X;
		}

		if(data_handle ==  drome_TDMS_Right_Wheel_Y)
		{
			val_ptr[0] = TDMS_Right_Wheel_Y;
		}

		if(data_handle ==  drome_TDMS_Left_Wheel_Alert_X)
		{
			val_ptr[0] = TDMS_Left_Wheel_Alert_X;
		}

		if(data_handle ==  drome_TDMS_Left_Wheel_Alert_Y)
		{
			val_ptr[0] = TDMS_Left_Wheel_Alert_Y;
		}

		if(data_handle ==  drome_TDMS_Right_Wheel_Alert_X)
		{
			val_ptr[0] = TDMS_Right_Wheel_Alert_X;
		}

		if(data_handle ==  drome_TDMS_Right_Wheel_Alert_Y)
		{
			val_ptr[0] = TDMS_Right_Wheel_Alert_Y;
		}



		if(data_handle ==  drome_Vehicle_Info_UP_Down_Arrow_Visible)
		{
			if(((FEATURE_CODE_DISPLAY_4WD == 1)&&(FEATURE_CODE_TRANSFER_MODE_CONFIG == 1))&&(FEATURE_CODE_TDMS_DISPLAY == 1))
			{
				val_ptr[0] = 1;	/*UP/Down arrow enable*/
			}
			else
			{
				val_ptr[0] = 0;	/*UP/Down arrow disable*/
			}
		}


		if(data_handle ==  drome_AT_Alert_imageIndex)
		{
			val_ptr[0] = AT_Gear_display;
#if 0
			if(TGS_LEVER1 == 0x09)	/*P*/
			{
				val_ptr[0] = 0;
			}
			else if(TGS_LEVER1 == 0x08) /*R*/
			{
				val_ptr[0] = 1;
			}
			else if(TGS_LEVER1 == 0x07)	/*N*/
			{
				val_ptr[0] = 2;
			}
			else if(TGS_LEVER1 == 0x06)	/*D*/
			{
				val_ptr[0] = 3;
			}
			else if(TGS_LEVER1 == 0x0C)	/*M*/
			{
				val_ptr[0] = 4;
			}
#endif
		}
#endif		
#endif


	}
}


/*************************************************************************************/
void tostring(U16 str[], int num, int l)
{
	int i, rem, len = 0, n;

	for (i = 0; i < l-1; i++)
	{
		str[l]= ' ';
	}
	str[l] = '\0';

	n = num;

	if(n==0)
	{
		str[0]='0';
		str[1] = '\0';
	}
	else
	{
		while (n != 0)
		{
			len++;
			n /= 10;
		}
		for (i = 0; i < len; i++)
		{
			rem = num % 10;
			num = num / 10;
			str[len - (i + 1)] = rem + '0';
		}
		str[len] = '\0';
	}
}

void tostring_decimal(U16 str[], int num, int l)
{
	int i, rem, len = 0, n;

	for (i = 0; i < l-1; i++)
	{
		str[l]= ' ';
	}
	str[l] = '\0';

	n = num;
	if(n<10)
	{
		str[0]='0';
		str[1]='.';
		str[2]= n +'0';
		str[3]=	'\0';
	}
	else
	{
		while (n != 0)
		{
			len++;
			n /= 10;
		}
		for (i = 0; i < len; i++)
		{
			rem = num % 10;
			num = num / 10;
			str[len - (i + 1)] = rem + '0';
		}
		str[len] = '\0';

		str[len+1]=str[len];
		str[len]=str[len-1];
		str[len-1]= '.';
	}




}
Public
void
gui_getDPValueText(
		U16 data_handle,           /**<[in] A handle to the data pool parameter. \todo U16 shall be replaced with enumeration! */
		drome_DPStrFormat* format, /**<[out] Struct with formatting data for the data value (num digits, decimals, unit etc) */
		U16* str_ptr,              /**<[out] pointer to store string to */
		U16 str_len,               /**<[in] Maximum length in bytes of string to store, including string termination */
		U16 argument)              /**<[in] Optional argument, parameter specific meaning */

/** Function called from dromelib to get a datapool parameter as a string.
 ** The string may include unit, depending on the autoUnit parameter. The string
 ** shall be in UTF8 format. If the parameter is not available, FALSE shall be
 ** returned and the string shall be NULL terminated.
 **************************************************************************************/
{
	U8 index = 0u;
	(void)format;
	(void)str_len;
	(void)argument;

	*str_ptr = 0u;

	/* if (data_handle < (U8)(drome_MAX_DATAPOOL))
   {
      if (findIndexDpTable(data_handle, &index))
      {
#if 0
	  if (DP_TABLE[index].string_fnptr != NULL)
         {
            DP_TABLE[index].string_fnptr(index, str_ptr);
         }
#endif	  
      }
   }*/

	if(data_handle == drome_Speedo_value)
	{
		if(FEATURE_CODE_DIGITAL_SPEED_CONFIG == 1)
		{
			if((Speed_Missing == 0) && (final_vechile_speed_gauge_raw < 0x4B01))//0x4B01 to display '---' for speed > 300kmph
			{
				if(ISM0VAR4CFG0<50050/*41666*/)/*0x40*//*less than 1.5 kmph displays 0*/
				{
					display_speedometer_speed=0;
				}
				else if((ISM0VAR4CFG0>=50050)&&(ISM0VAR4CFG0<66818))/*between 1.5 to 2kmph*/
				{
					display_speedometer_speed=1;
				}

				if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
				{
					tostring(str_ptr,(display_speedometer_speed* 0.621371),3);/*display_speedometer_speed;*/
				}
				else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
				{
					tostring(str_ptr,display_speedometer_speed,3);/*display_speedometer_speed;*/
				}
				else															//display in KM
				{
					tostring(str_ptr,display_speedometer_speed,3);/*display_speedometer_speed;*/
				}
			}
			else
			{
				str_ptr[0] = '-';
				str_ptr[1] = '-';
//				str_ptr[2] = '-';
				str_ptr[2] =  '\0';
			}
		}
		else
		{
			/*Add Digital Speedo off*/
		}

	}
	else if(data_handle == drome_Odo_value)
	{
		/*odo error*/
		if(odo_error_flag == 1)
		{
			//			str_ptr[0] = '\0';
			//			str_ptr[1] = 'r';
			//			str_ptr[2] = 'o';
			//			str_ptr[3] = 'r';
			//			str_ptr[4] = 'r';
			//			str_ptr[5] = 'E';
			//			str_ptr[6] = '\0';
					/*	str_ptr[0] = 'E';//'E';
						str_ptr[1] = 'r';//'r';
						str_ptr[2] = 'r';//'r';
						str_ptr[3] = 'o';//'o';
						str_ptr[4] = 'r';//'r';
						str_ptr[5] = '\0';*/
			//			str_ptr[6] = '\0';
		}
		else if(odo_error_flag == 0)
		{
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring(str_ptr,(uint32_t)((final_odomter_distance/1000)* 0.621371),6);
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring(str_ptr,(uint32_t)(final_odomter_distance/1000),6);
			}
			else															//display in KM
			{
				tostring(str_ptr,(uint32_t)(final_odomter_distance/1000),6);
			}
		}
	}
	else if(data_handle == drome_TripA_value)
	{

/*		if(odo_error_flag == 1)
		{
			str_ptr[0] = 'E';//'E';
			str_ptr[1] = 'r';//'r';
			str_ptr[2] = 'r';//'r';
			str_ptr[3] = 'o';//'o';
			str_ptr[4] = 'r';//'r';
			str_ptr[5] = '\0';
		}
		else
		{*/
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring_decimal(str_ptr,(uint32_t)((((final_odomter_distance-final_tripmeter_distance)/100)%100000)* 0.621371),6/*5*/);/**str_ptr = final_tripmeter_distance;*/
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring_decimal(str_ptr,(uint32_t)(((final_odomter_distance-final_tripmeter_distance)/100)%100000),6/*5*/);/**str_ptr = final_tripmeter_distance;*/
			}
			else															//display in KM
			{
				tostring_decimal(str_ptr,(uint32_t)(((final_odomter_distance-final_tripmeter_distance)/100)%100000),6/*5*/);/**str_ptr = final_tripmeter_distance;*/
			}
//		}
	}
	else if(data_handle == drome_TripB_value)
	{
/*		if(odo_error_flag == 1)
		{
			str_ptr[0] = 'E';//'E';
			str_ptr[1] = 'r';//'r';
			str_ptr[2] = 'r';//'r';
			str_ptr[3] = 'o';//'o';
			str_ptr[4] = 'r';//'r';
			str_ptr[5] = '\0';
		}
		else
		{*/
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring_decimal(str_ptr,(uint32_t)((((final_odomter_distance-final_tripmeter_B_distance)/100)%100000)* 0.621371),6/*5*/);/**str_ptr = final_tripmeter_distance;*/
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring_decimal(str_ptr,(uint32_t)(((final_odomter_distance-final_tripmeter_B_distance)/100)%100000),6/*5*/);
			}
			else														//display in KM
			{
				tostring_decimal(str_ptr,(uint32_t)(((final_odomter_distance-final_tripmeter_B_distance)/100)%100000),6/*5*/);
			}
//		}
	}
	else if((data_handle == drome_TripAC_value)&&(FEATURE_CODE_AC_TRIP_METER == 1U))
	{
			Trip_AC_temp = ((final_odomter_distance-(final_ACtripmeter_distance1 - TRIP_AC_OFFSET))/1000)/*%100000*/;

			if((sts_ac_compressor_flag ==  0x1)||(IGN_ON_AC_Read == 0))
			{
				IGN_ON_AC_Read = 1;
				Trip_AC_old	= Trip_AC_temp;
			}

			if(Trip_AC_temp <= (Trip_AC_old+1))
			{
				if(Trip_AC_temp == 0)
				{
					Trip_AC = 0;
				}
				else
				{
					Trip_AC = (Trip_AC_old+Trip_AC_temp)/2;
				}
			}
			if(Trip_AC > 9999)
			{
				Trip_AC_Rollover = 1;
				reset_AC_tripmeter();
			}
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				/*tostring_decimal*/tostring(str_ptr,(uint32_t)((Trip_AC)* 0.621371),4/*6*/);
//				/*tostring_decimal*/tostring(str_ptr,(uint32_t)((((final_odomter_distance-(final_ACtripmeter_distance1 - TRIP_AC_OFFSET))/1000)/*%100000*/)* 0.621371),4/*6*/);
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				/*tostring_decimal*/tostring(str_ptr,(uint32_t)(Trip_AC),4/*6*/);
//				/*tostring_decimal*/tostring(str_ptr,(uint32_t)(((final_odomter_distance-(final_ACtripmeter_distance1 - TRIP_AC_OFFSET))/1000)/*%100000*/),4/*6*/);
			}
			else															//display in KM
			{
				/*tostring_decimal*/tostring(str_ptr,(uint32_t)(Trip_AC),4/*6*/);
//				/*tostring_decimal*/tostring(str_ptr,(uint32_t)(((final_odomter_distance-(final_ACtripmeter_distance1 - TRIP_AC_OFFSET))/1000)/*%100000*/),4/*6*/);
			}
//		}
	}
	else if(data_handle == drome_Avg_Speedvalue_A)
	{
		/**str_ptr = 120u;*/
		/*str_ptr[0] = '0';
   		   		   str_ptr[1] = '0'; 
   		   		   str_ptr[2] = '0'; 
		 */
		/*str_ptr[0] = '1';
		str_ptr[1] = '0';
		str_ptr[2] = '0';
		str_ptr[3] = '\0';*/
		if(HMI_AVS_DASH1 == 1)
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
		}
		else
		{
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring(str_ptr,(Display_Average_speed_A* 0.621371),3);/*Display_Average_speed_B;*/
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring(str_ptr,Display_Average_speed_A,3);/*Display_Average_speed_B;*/
			}
			else
			{
				tostring(str_ptr,Display_Average_speed_A,3);/*Display_Average_speed_B;*/
			}

		}

	}
	else if(data_handle == drome_Avg_Speedvalue_B)
	{
		/**str_ptr = 120u;*/
		/*str_ptr[0] = '0';
   		   		   str_ptr[1] = '0';
   		   		   str_ptr[2] = '0';
		 */
		/*str_ptr[0] = '1';
		str_ptr[1] = '0';
		str_ptr[2] = '0';
		str_ptr[3] = '\0';*/
		if(HMI_AVS_DASH2 == 1)
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
		}
		else
		{
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring(str_ptr,(Display_Average_speed_B* 0.621371),3);/*Display_Average_speed_B;*/
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring(str_ptr,Display_Average_speed_B,3);/*Display_Average_speed_B;*/
			}
			else
			{
				tostring(str_ptr,Display_Average_speed_B,3);/*Display_Average_speed_B;*/
			}
		}
//		tostring(str_ptr,Display_Average_speed_B,3);/*Display_Average_speed_B;*/

	}
	else if(data_handle == drome_TimeHour_value)
	{
		if(Dashes_colon == 1)
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
		}
		else
		{
		if(hour<=9)
		{

			str_ptr[0] = '0';
			if(hour==0)
			{
				str_ptr[1] = '0';
			}

			if(hour==1)
			{
				str_ptr[1] = '1';
			}
			else if(hour==2)
			{
				str_ptr[1] = '2';
			}
			else if(hour==3)
			{
				str_ptr[1] = '3';
			}
			else if(hour==4)
			{
				str_ptr[1] = '4';
			}
			else if(hour==5)
			{
				str_ptr[1] = '5';
			}
			else if(hour==6)
			{
				str_ptr[1] = '6';
			}
			else if(hour==7)
			{
				str_ptr[1] = '7';
			}
			else if(hour==8)
			{
				str_ptr[1] = '8';
			}
			else if(hour==9)
			{
				str_ptr[1] = '9';
			}
		}
		else
			tostring(str_ptr,(uint32_t)(hour),2);
	}
	}
	else if(data_handle == drome_TimeMin_value)
	{
		if(Dashes_colon == 1)
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
		}
		else
		{
		if(minute<=9)
		{

			str_ptr[0] = '0';
			if(minute==0)
			{
				str_ptr[1] = '0';
			}

			if(minute==1)
			{
				str_ptr[1] = '1';
			}
			else if(minute==2)
			{
				str_ptr[1] = '2';
			}
			else if(minute==3)
			{
				str_ptr[1] = '3';
			}
			else if(minute==4)
			{
				str_ptr[1] = '4';
			}
			else if(minute==5)
			{
				str_ptr[1] = '5';
			}
			else if(minute==6)
			{
				str_ptr[1] = '6';
			}
			else if(minute==7)
			{
				str_ptr[1] = '7';
			}
			else if(minute==8)
			{
				str_ptr[1] = '8';
			}
			else if(minute==9)
			{
				str_ptr[1] = '9';
			}
		}
		else
			tostring(str_ptr,(uint32_t)(minute),2);
		}
	}
	else if(data_handle == drome_DT_Minvalue_TripA)
	{
//		str_ptr[0] = '0';
//		str_ptr[1] = '0';
//		str_ptr[2] = '\0';
		if(Display_Min_A < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(Display_Min_A),2);
		}
		else
		{
			tostring(str_ptr,(uint8_t)(Display_Min_A),2);
		}
	}
	else if(data_handle == drome_DT_Hrvalue_TripA)
	{
//		str_ptr[0] = '0';
//		str_ptr[1] = '0';
//		str_ptr[2] = '\0';
		if(Display_Hr_A < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(Display_Hr_A),2);
		}
		else
		{
			tostring(str_ptr,(uint8_t)(Display_Hr_A),2);
		}
	}
	else if(data_handle == drome_DT_Minvalue_TripB)
	{
//		str_ptr[0] = '0';
//		str_ptr[1] = '0';
//		str_ptr[2] = '\0';
		if(Display_Min_B < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(Display_Min_B),2);
		}
		else
		{
			tostring(str_ptr,(uint8_t)(Display_Min_B),2);
		}
	}
	else if(data_handle == drome_DT_Hrvalue_TripB)
	{
//		str_ptr[0] = '0';
//		str_ptr[1] = '0';
//		str_ptr[2] = '\0';
		if(Display_Hr_B < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(Display_Hr_B),2);
		}
		else
		{
			tostring(str_ptr,(uint8_t)(Display_Hr_B),2);
		}
	}
#if 1
	else if(data_handle == drome_OAT_value)
	{
//		tostring(str_ptr,(int)(test_cnt),3);
		if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1)&&(FEATURE_CODE_OAT_CONFIG == 1))
		{
			if((EMS6_500_message_timeout_flag != 0)||(CAN_EMS6_500[1].byte == 0xFF))
			{
				str_ptr[0] = '-';
				str_ptr[1] = '-';
				str_ptr[2] = '-';
				str_ptr[3] = '\0';
			}
			else
			{
				if(CAN_EMS6_500[1].byte >= 40)
				{
					Ambient_temp = CAN_EMS6_500[1].byte - 40;
					if(TempU_selected == 1)	/*Farad*/
					{
						//					Ambient_temp = CAN_EMS6_500[1].byte - 40;
						Ambient_temp1 = ((Ambient_temp*9)/5)+32;
						tostring(str_ptr,(int)(Ambient_temp1),3);
					}
					else
					{
						Ambient_temp1 = Ambient_temp;
						//					Ambient_temp = CAN_EMS6_500[1].byte - 40;
						tostring(str_ptr,(int)(Ambient_temp1),3);
					}
				}

				else															/*degC negative*/
				{
					Ambient_temp = 40 - CAN_EMS6_500[1].byte;
					if(TempU_selected == 1)	/*Farad*/
					{
						Ambient_temp1 = 0 - Ambient_temp;
						Ambient_temp1=((Ambient_temp1*9)/5)+32;
						if(Ambient_temp1 < 0)
						{
							Ambient_temp1 = Ambient_temp1*(-1);
							str_ptr[0] = '-';

							tostring(str_ptr+1,(int)(Ambient_temp1),2);
						}
						else
						{
							//						Ambient_temp2 = Ambient_temp1;
							tostring(str_ptr,(int)(Ambient_temp1),3);
						}
					}
					else
					{
						//					Ambient_temp = 40 - CAN_EMS6_500[1].byte;
						str_ptr[0] = '-';
						tostring(str_ptr+1,(uint8_t)(Ambient_temp),2);
					}
				}
			}
		}
		else
		{
			/*Hide OAT on screen*/
			TempU_selected = 2;
		}
		/*str_ptr[0] = '0';
		str_ptr[1] = '0';
		str_ptr[2] = '\0';*/
	}
#endif




	else if(data_handle == drome_DEF_Engine_NotStart_value)
	{
		if(Eng_No_Start.u16 >= 0X961)//	0Xfffe
		{

			/*str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';*/
			BSVI_errorcode = 1;
		}
		else
		{
			tostring(str_ptr,(uint32_t)(Eng_No_Start.u16),5);
			BSVI_errorcode = 0;
		}
	}

	else if(data_handle == drome_AFE_value)
	{
		if((NotdisplayAFE == 0) && (FEATURE_CODE_AFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1)&&((EMS4_20_message_timeout_flag == 0)&&(Speed_Missing == 0)))
		{
			if(AFEDispVal > 999)
			{
				AFEDispVal = 0;
			}
#if 0
			STS_Eng = CAN_EMS1_10[6].byte;
			STS_Eng = ((STS_Eng & 0xE0) >> 5);
			if(STS_Eng != 2)
			{
				if(Diag_Unit_Selection == 1)
				{
					AFE_display = (uint32_t)(10000/AFEDispVal_init);
					if(AFE_display <= 999)
					{
						if(AFE_display == 0)
						{
							str_ptr[0] = '-';
							str_ptr[1] = '-';
							str_ptr[2] = '.';
							str_ptr[3] = '-';
							str_ptr[4] = '\0';
						}
						else
						{
							tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*L/100km = *str_ptr = AFEDispVal;*//*1l/100km = 100/1 km/l for display resolution purpose multiply by 10*/
						}
					}
					else
					{
						AFE_display = 999;
						tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*L/100km = *str_ptr = AFEDispVal;*//*1l/100km = 100/1 km/l for display resolution purpose multiply by 10*/
					}
				}
				else if(Diag_Unit_Selection == 2)
				{
					AFE_display = (uint32_t)(AFEDispVal_init * 2.3521);
					if(AFE_display <= 999)
					{
						if(AFE_display == 0)
						{
							str_ptr[0] = '-';
							str_ptr[1] = '-';
							str_ptr[2] = '.';
							str_ptr[3] = '-';
							str_ptr[4] = '\0';
						}
						else
						{
							tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*MPG US = *str_ptr = AFEDispVal;*//*1 mpg us = 1 km/l * 2.3521 for display resolution purpose multiply by 10*/
						}
					}
					else
					{
						AFE_display = 999;
						tostring_decimal(str_ptr,(uint32_t)AFE_display,4);
					}
				}
				else if(Diag_Unit_Selection == 3)
				{


					AFE_display = (uint32_t)(AFEDispVal_init * 2.8248);
					if(AFE_display <= 999)
					{
						if(AFE_display == 0)
						{
							str_ptr[0] = '-';
							str_ptr[1] = '-';
							str_ptr[2] = '.';
							str_ptr[3] = '-';
							str_ptr[4] = '\0';
						}
						else
						{
							tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*MPG UK =  *str_ptr = AFEDispVal;*/ /*1 mpg uk = 1 km/l * 2.8248 for display resolution purpose multiply by 10*/
						}
					}
					else
					{
						AFE_display = 999;
						tostring_decimal(str_ptr,(uint32_t)AFE_display,4);
					}
				}
				else
				{
					if(AFEDispVal_init == 0)
					{
						str_ptr[0] = '-';
						str_ptr[1] = '-';
						str_ptr[2] = '.';
						str_ptr[3] = '-';
						str_ptr[4] = '\0';
					}
					else
					{
						tostring_decimal(str_ptr,(uint32_t)AFEDispVal_init,4);/*km/l - default *str_ptr = AFEDispVal;*/
					}
				}
			}
			else
#endif
			{
//				AFEDispVal_init = AFEDispVal;
			if(Diag_Unit_Selection == 1)
			{
				AFE_display = (uint32_t)(10000/AFEDispVal);
				if(AFE_display <= 999)
				{
					if(AFE_display == 0)
					{
						str_ptr[0] = '-';
						str_ptr[1] = '-';
						str_ptr[2] = '.';
						str_ptr[3] = '-';
						str_ptr[4] = '\0';
					}
					else
					{
					tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*L/100km = *str_ptr = AFEDispVal;*//*1l/100km = 100/1 km/l for display resolution purpose multiply by 10*/
					}
				}
				else
				{
					AFE_display = 999;
					tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*L/100km = *str_ptr = AFEDispVal;*//*1l/100km = 100/1 km/l for display resolution purpose multiply by 10*/
				}
			}
			else if(Diag_Unit_Selection == 2)
			{
				AFE_display = (uint32_t)(AFEDispVal * 2.3521);
				if(AFE_display <= 999)
				{
					if(AFE_display == 0)
					{
						str_ptr[0] = '-';
						str_ptr[1] = '-';
						str_ptr[2] = '.';
						str_ptr[3] = '-';
						str_ptr[4] = '\0';
					}
					else
					{
					tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*MPG US = *str_ptr = AFEDispVal;*//*1 mpg us = 1 km/l * 2.3521 for display resolution purpose multiply by 10*/
					}
				}
				else
				{
					AFE_display = 999;
					tostring_decimal(str_ptr,(uint32_t)AFE_display,4);
				}
			}
			else if(Diag_Unit_Selection == 3)
			{


				AFE_display = (uint32_t)(AFEDispVal * 2.8248);
				if(AFE_display <= 999)
				{
					if(AFE_display == 0)
					{
						str_ptr[0] = '-';
						str_ptr[1] = '-';
						str_ptr[2] = '.';
						str_ptr[3] = '-';
						str_ptr[4] = '\0';
					}
					else
					{
					tostring_decimal(str_ptr,(uint32_t)AFE_display,4);/*MPG UK =  *str_ptr = AFEDispVal;*/ /*1 mpg uk = 1 km/l * 2.8248 for display resolution purpose multiply by 10*/
					}
				}
				else
				{
					AFE_display = 999;
					tostring_decimal(str_ptr,(uint32_t)AFE_display,4);
				}
			}
			else
			{
				if(AFEDispVal == 0)
				{
					str_ptr[0] = '-';
					str_ptr[1] = '-';
					str_ptr[2] = '.';
					str_ptr[3] = '-';
					str_ptr[4] = '\0';
				}
				else
				{
					tostring_decimal(str_ptr,(uint32_t)AFEDispVal,4);/*km/l - default *str_ptr = AFEDispVal;*/
				}
			}
			}

		}
		else
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '.';
			str_ptr[3] = '-';
			str_ptr[4] = '\0';
		}
	}
	else if(data_handle == drome_DTE_value)//dte
	{

		if((fuel_fault_30sec == 1)||(Speed_Invalid_Default == 1)||(Dist_Invalid_Default == 1)||((CAN_EMS4_20[2].byte == 0xFF)&&(CAN_EMS4_20[3].byte == 0xFF))||(EMS4_20_message_timeout_flag != 0)||(Speed_Missing == 1))
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
		}
		else if((fuel_widget == 0)&&(fuel_fault_30sec == 0)&&(Fuel_init_done == 1))
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
		}
		else
		{

		if((FEATURE_CODE_DTE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
//		if((NotdisplayAFE == 0) && (FEATURE_CODE_AFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
		{
			if(DTE_Disp_Val > 999)
			{
				DTE_Disp_Val = 999;//0	DTE Roll over not req.
			}
			else
			{
				if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
				{
					tostring(str_ptr,(uint32_t)(DTE_Disp_Val* 0.621371),3);/**str_ptr = DTE_Disp_Val;*/
				}
				else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
				{
					tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
				}
				else
				{
					tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
				}
				//			tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
			}

			if((DTE_Disp_Val == 0)||((Fuel_init_done == 0)&&(displayed_fuel_level==0)))
			{
				str_ptr[0] = '-';
				str_ptr[1] = '-';
				str_ptr[2] = '-';
				str_ptr[3] = '\0';
			}
		}
		else
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
		}
		}

	}
	else if(data_handle == drome_Setting_textDynamic)
	{
		const U8 * text_ptr	= dynamic_text_ptr[argument];

		while(*text_ptr != '\0')
		{
			*str_ptr++ = (U16)*text_ptr++;
		}
		*str_ptr = 0;
	}


	else if(data_handle == drome_Setting_Timevalue_HR_1)	/*Unit Selection*/
	{
//		val_ptr[0] = Hr1_value;
		if(Hr1_value < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(Hr1_value),2);
		}
		else
		{
		tostring(str_ptr,(uint8_t)(Hr1_value),2);
		}
	}


	else if(data_handle == drome_Setting_Timevalue_MIN_1)	/*Unit Selection*/
	{
		if(Min1_value < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(Min1_value),2);
		}
		else{
//		val_ptr[0] = Min1_value;
		tostring(str_ptr,(uint8_t)(Min1_value),2);
		}
	}


	else if(data_handle == drome_Total_WH_Pages1)
	{
		tostring(str_ptr,(uint8_t)(size),1);
	}
//ETM
//	else if(data_handle == drome_ETM_SW_ROM_Version_Index)
//	{
//		tostring(str_ptr,(uint32_t)(END_MEMORY_OFFSET),3);
//	}
	else if(data_handle == drome_ETM_SW_NVM_Version_Index)
	{
		tostring(str_ptr,(uint32_t)(END_MEMORY_OFFSET),3);
		if(set_key_short == 1)
		{
			set_key_short = 0;

		}
	}
	else if(data_handle == drome_ETM_Fuel_Rad_Count_index)
	{
//		FLOAT_SENSOR_RESISTANCE_RAW = (0.00001 * New_fuel_read_ad1 * New_fuel_read_ad1) + (0.0101 * New_fuel_read_ad1) + 7.2125;//(Fuel_ADC_value/10.5);
		FLOAT_SENSOR_RESISTANCE_RAW = (0.00001 * New_fuel_read_avg * New_fuel_read_avg) + (0.0101 * New_fuel_read_avg) + 7.2125;//undamped Resistance value
		tostring(str_ptr,(uint32_t)(FLOAT_SENSOR_RESISTANCE_RAW),3);
		if(set_key_short == 1)
		{
			set_key_short = 0;

		}
	}
	else if(data_handle == drome_ETM_Fuel_RAW_ADC_Index)
	{
		tostring(str_ptr,(uint32_t)(Fuel_ADC_value),3);//Fuel RAW ADC count
		if(set_key_short == 1)
		{
			set_key_short = 0;

		}
	}
	else if(data_handle == drome_ETM_Fuel_Supply_ADC_index)
	{
		tostring(str_ptr,(uint32_t)(Fuel_EXVrefADC_value),3);//Fuel ADC vref value
		if(set_key_short == 1)
		{
			set_key_short = 0;

		}
	}
	else if(data_handle == drome_ETM_BATT_SUPPLY_ADC_Index)//battery supply
	{
		tostring(str_ptr,(uint32_t)(Batt_ADC_value),3);
		if(set_key_short == 1)
		{
			set_key_short = 0;

		}
	}
	else if(data_handle == drome_RPAS_DIST_value)				/*RPAS display distance*/
	{

			tostring(str_ptr,(uint8_t)(RPAS__update_display_dist),3);
	}
	else if(data_handle == drome_Drive_Sum_AvgSpeed)				/*Drive Summary AvgSpeed*/
	{
		if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
		{
			tostring(str_ptr,(Dsum_Display_Average_speed* 0.621371),3);/*Display_Average_speed;*/
		}
		else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
		{
			tostring(str_ptr,Dsum_Display_Average_speed,3);/*Display_Average_speed_B;*/
		}
		else
		{
			tostring(str_ptr,Dsum_Display_Average_speed,3);/*Display_Average_speed_B;*/
		}

//		tostring(str_ptr,(uint32_t)Dsum_Display_Average_speed,3);

	}
	else if(data_handle == drome_Drive_Sum_Distance)				/*Drive Summary Distance*/
	{
		if(Final_Drivesum_odo_distance >9999000)
		{
			Final_Drivesum_odo_distance = 0;
		}
		else
		{
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring(str_ptr,(uint32_t)((Final_Drivesum_odo_distance/1000)* 0.621371),6);
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring(str_ptr,(uint32_t)(Final_Drivesum_odo_distance/1000),6);
			}
			else															//display in KM
			{
				tostring(str_ptr,(uint32_t)(Final_Drivesum_odo_distance/1000),6);
			}
		}

	}
	else if(data_handle == drome_4WD_not_engaged)
	{
		if(Final_Drivesum_2WD_distance > 999999000)	/*Drive summary 4x4 logic changed as per CR - 26-05-20 for W501Hv232*/
		{
			Final_Drivesum_2WD_distance = 999999000;
		}
		else
		{
			if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
			{
				tostring(str_ptr,(uint32_t)((Final_Drivesum_2WD_distance/1000)* 0.621371),6);
			}
			else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
			{
				tostring(str_ptr,(uint32_t)(Final_Drivesum_2WD_distance/1000),6);
			}
			else															//display in KM
			{
				tostring(str_ptr,(uint32_t)(Final_Drivesum_2WD_distance/1000),6);
			}
		}
	}
	else if(data_handle == drome_Drive_Sum_DTE)						/*Drive Summary DTE*/
	{
		if((fuel_fault_30sec == 1)||(Speed_Invalid_Default == 1)||(Dist_Invalid_Default == 1)||((CAN_EMS4_20[2].byte == 0xFF)&&(CAN_EMS4_20[3].byte == 0xFF))||(EMS4_20_message_timeout_flag != 0)||(Speed_Missing == 1))
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
//			DTE_Disp_Val  = 0x03FF;
		}
		else if((fuel_widget == 0)&&(fuel_fault_30sec == 0))
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '-';
			str_ptr[3] = '\0';
//			DTE_Disp_Val  = 0x03FF;
		}
		else
		{

			if((FEATURE_CODE_DTE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
				//		if((NotdisplayAFE == 0) && (FEATURE_CODE_AFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			{
				if(DTE_Disp_Val > 999)
				{
					DTE_Disp_Val = 999;//0	DTE Roll over not req.
				}

				if((Diag_Unit_Selection == 2)||(Diag_Unit_Selection == 3))//display in  mile
				{
					tostring(str_ptr,(uint32_t)(DTE_Disp_Val* 0.621371),3);/**str_ptr = DTE_Disp_Val;*/
				}
				else if((Diag_Unit_Selection == 0)||(Diag_Unit_Selection == 1))//display in KM
				{
					tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
				}
				else
				{
					tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
				}
				//			tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
			}
			else
			{
				str_ptr[0] = '-';
				str_ptr[1] = '-';
				str_ptr[2] = '.';
				str_ptr[3] = '-';
				str_ptr[4] = '\0';
			}
		}
#if 0
		if((FEATURE_CODE_DTE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
			//		if((NotdisplayAFE == 0) && (FEATURE_CODE_AFE_IND == 1) && (VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1))
		{
			tostring(str_ptr,(uint32_t)DTE_Disp_Val,3);/**str_ptr = DTE_Disp_Val;*/
		}
		else
		{
			str_ptr[0] = '-';
			str_ptr[1] = '-';
			str_ptr[2] = '.';
			str_ptr[3] = '-';
			str_ptr[4] = '\0';
			DTE_Disp_Val  = 0x03FF; /* FOR TESTING */
		}
#endif
	}
	else if(data_handle == drome_Drive_Sum_DT_Hrvalue)				/*Drive Summary DT Hour*/
	{

		if(DSum_Display_Hr < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(DSum_Display_Hr),2);
		}
		else
		{
			tostring(str_ptr,(uint8_t)(DSum_Display_Hr),2);
		}

	}
	else if(data_handle == drome_Drive_Sum_DT_Minvalue)				/*Drive Summary DT Minute*/
	{

		if(DSum_Display_Min < 10)
		{
			str_ptr[0] = '0';
			tostring(str_ptr+1,(uint8_t)(DSum_Display_Min),2);
		}
		else
		{
			tostring(str_ptr,(uint8_t)(DSum_Display_Min),2);
		}
	}

	else if(data_handle == drome_IFE_MaxDisplay_value)				/*IFE_MaxDisplay_value*/
	{
		tostring(str_ptr,(uint8_t)(Diag_RW_IFE_Max),3);
	}
	else if(data_handle == drome_IFE_MidDisplay_value)				/*IFE_MidDisplay_value*/
	{
		tostring(str_ptr,(uint8_t)(Diag_RW_IFE_Max/2),3);
	}


#ifdef SPRINT4
/* Used for TDMS angle */
	else if(data_handle == drome_TDMS_Rotation_Value)				/* TDMS value */
	{
		tostring(str_ptr,(uint16_t)(TDMS_Rotation_Value),2);
	}
#endif
}

/*************************************************************************************/
Public
void
gui_getDPValueVector(
		U16 data_handle,           /**<[in] A handle to the data pool parameter. \todo U16 shall
                                    be replaced with enumeration! */
		const void* set_ptr,       /**<[out] A pointer to a vector of U16’s containing the ID’s
                                     of telltales relevant for the widget, the vector
                                     shall be terminated by a null value. */
		void* dest_ptr,            /**<[out] A pointer to a buffer of U16’s in RAM where the ID’s
                                     of the active telltales shall be written */
		U16 dest_size,             /**<[in] The maximum number of active telltale ID’s to return,
                                    i.e. the number of available positions in the widget,
                                    i.e. drapp_CTT_MAX_TT_COUNT */
		U16 argument)              /**<[in] Optional argument, parameter specific meaning */

/** This application function is used by DROME to retrieve a vector of data values from
 **  the application.
 **************************************************************************************/
{
	(void)data_handle;
	(void)set_ptr;
	(void)dest_ptr;
	(void)dest_size;
	(void)argument;
}

/*************************************************************************************/
Public
void
gui_loadHistoryStatesData(U32* data_ptr,
		U16 max_size)
/** Called during initialization. Should either be used to load history state data that
 *  have been stored in non-volatile memory or reset the buffer
 **************************************************************************************/
{
	(void)data_ptr;
	(void)max_size;
}

/*************************************************************************************/
Public
void
gui_saveHistoryStatesData(const U32* data_ptr,
		U16 size)
/** This application function is used by DROME to save initialisation data, regarding
 *  history states, in non-volatile memory during Drome shut-down (i.e. execution of
 *  drome_shutDown()).If history state data is not required to be stored in non-volatile
 *  memory the application can ignore the call.
 **************************************************************************************/
{
	(void)data_ptr;
	(void)size;
}


/*************************************************************************************/
Public
void
gui_getExternalText(U16 text_handle,
		U16 *str_ptr,
		U16 str_len)
/** This function is only used when dromelib has been configured to use external texts
 **************************************************************************************/
{
	(void)text_handle;
	(void)str_ptr;
	(void)str_len;
}

#if 0	/*changes for w501*/
/*************************************************************************************/
Public
void
gui_loadNVData(
		U16 data_handle,     /**< A value indicating what data is requested. */
		U8* data_ptr,        /**< Pointer to buffer where the app. shall store the data. */
		U16 max_size)        /**< Maximum / requested size of data to store. */

/** This application function is used by DROME to load initialisation data stored
 **  in non-volatile memory during Drome initialisation.
 **************************************************************************************/
{
	(void)data_handle;
	(void)data_ptr;
	(void)max_size;
}


/*************************************************************************************/
Public
void
gui_saveNVData(
		U16 data_handle,     /**< A value indicating what data that shall be saved. */
		const U8* data_ptr,  /**< Pointer to buffer where the app. shall get the data. */
		U16 size)            /**< Number of bytes that shall be saved. */
/**  This application function is used by DROME to save initialisation data in
 **   non-volatile memory during Drome shut-down.
 **************************************************************************************/
{
	(void)data_handle;
	(void)data_ptr;
	(void)size;
}
#endif

//#pragma optimize=none 				/*lint !e975*/
/*************************************************************************************/
Public
void 
gui_applicationEvent(
		U32 event)  /**<[in] Drome event */

/** Function called from dromelib to request an application action.
 **************************************************************************************/
{
	int j=0,i=0;
	if(event==drome_OutEvent_mainscreen)
	{
		first_screen_timeout_start=1;
		first_screen_timeout=1;
		//(void)drome_sendEventToDrome((U32)drome_InEventMain_screen_timeout);
	}
	if(event==drome_OutEvent_logo_completed)
	{
		logo_completed=1;
	}
	if((event == drome_OutEvent_Alert_started)||(event == drome_OutEvent_REGEN_completed_popup)||(event == drome_OutEvent_REGEN_aborted_popup))
	{
		start_alart_timeout_5sec=1;
	}
#ifdef SPRINT4
	if(event == drome_OutEvent_AT_Gear_Alert_Started)
	{
		start_AT_alert_timeout_cnt=1;
	}
#endif	
	
	
	
	/*else if(event==drome_OutEvent_REGEN)
{
	regen_page_active=1;
}*/
	else if(event == drome_OutEvent_REGEN_requested)
	{
		regen_req_page_timeout=1;
		regen_reqpage_to_notstarted = 1;
		regen_req_start = 1;
		start_alart_timeout_5sec=1;
		CAN_IC2_100[5].bits.Rx_2 = 1;
		//		CAN_IC2_100[6].bits.Rx_1 = 1;
	}
	else if(event == drome_OutEvent_REGEN_not_started)
	{
		regen_req_page_timeout=1;
		regen_page_notstarted=1;
	//	CAN_IC2_100[5].bits.Rx_2 = 0;
		if(REGEN_notstarted_popup_skip != 1)
		{
			regen_req_start = 0;
		}
		start_alart_timeout_5sec=1;
		CAN_IC2_100[5].bits.Rx_2 = 0;

	}
	else if(event==drome_OutEvent_REGEN_started)
	{
		regen_page_started=1;
		regen_req_page_timeout=1;
//		CAN_IC2_100[5].bits.Rx_2 = 1;
		regen_req_start = 0;
		start_alart_timeout_5sec=1;
		CAN_IC2_100[5].bits.Rx_2 = 0;

		/**/
	}
	/*else
	{
		regen_req_start = 0;
	}*/
/******************************* AFE RESET SETTING *************************************/
	if(event == drome_OutEvent_AFE_1st_row_selected)
	{
		AFE_selected = 1;						/*User Reset*/
	}
	else if(event == drome_OutEvent_AFE_2nd_row_selected)
	{
		AFE_selected = 2;						/*After Ignition*/
	}
	else if(event == drome_OutEvent_AFE_3rd_row_selected)
	{
		AFE_selected = 3;						/*After Refueling*/
	}
/*************************/
	if(event == drome_Out_AFEMenu_Entered)
	{
		switch (AFE_selected)
		{
		case 1:
			(void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
			break;
		case 2:
			(void)drome_sendEventToDrome((U32)drome_InEvent_2nd_detected);
			break;
		case 3:
			(void)drome_sendEventToDrome((U32)drome_InEvent_3rd_detected);
			break;
		case 0:
		default: (void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
			break;
		}
    }
/************************************ AFE RESET SETTING END ******************************************/
/*********************************** TIME FORMAT SELECTION START *************************************/
	if(event == drome_OutEvent_TimeF_1st_row_selected)
	{
		TimeF_selected = 1;					/*12 Hr*/
		if(hour>12)
		{
			//changed_hr = hour-12;
			changed_hr = hour;
			AM_flag = 0;
		}
		else if(hour < 12)
		{
			changed_hr = hour;
			AM_flag = 1;
		}
		else if(hour == 12)
		{
			changed_hr = hour;
			AM_flag=0;

		}
		else if(hour == 0)
		{
			changed_hr = hour;
			AM_flag=1;
		}

		changed_min = minute;
		RTCA0AMPM = 0;
		TimeF_selection = 1;
//		eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected);
//		format_changed  = 1;
//		time_format = 1;
//		Timesetting_completed = 1;
	}
	else if(event == drome_OutEvent_TimeF_2nd_row_selected)
	{
		TimeF_selected = 2;					/*24 Hr*/

		if(AM_flag==0)
		{
			if(hour==12)
			{
				changed_hr = hour;
			}
			else
			{
				changed_hr = hour+12;
				//changed_hr = hour;
			}
		}
		else if(AM_flag==1)
		{
			if(hour==12)
			{
				changed_hr=0;
			}
			else
			{
				changed_hr = hour;
			}
		}
		changed_min = minute;
		RTCA0AMPM = 1;
//		eeprom_write(SETTING_TIMEFRMT_OFFSET, TimeF_selected);
//		format_changed  = 1;
//		time_format = 0;
//		Timesetting_completed = 1;
		//AM_flag = 2;
		TimeF_selection = 1;
	}

	/*************************/
	if(event == drome_Out_TimeFMenu_Entered)
	{
		switch (TimeF_selected)
		{
		case 1:
			(void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
			break;
		case 2:
			(void)drome_sendEventToDrome((U32)drome_InEvent_2nd_detected);
			break;
		case 0:
		default: (void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
		break;
		}
	}
/*********************************** TIME FORMAT SELECTION END *************************************/
/************************************** TIME SETTING START *****************************************/
	if(event == drome_OutEvent_TimeSet_Entered)
	{
		Hr1_value = hour;// /10;
		/*Hr0_value = hour%10;*/
		Min1_value = minute; ///10;
		/*Min0_value = minute%10;*/
		AMPM_value = AM_flag;
		TimeSet_BackIndex = 1;
		TimeSet_ConfirmIndex = 0;
		switch (TimeF_selected)
		{
		case 1:
			(void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
			break;
		case 2:
			(void)drome_sendEventToDrome((U32)drome_InEvent_2nd_detected);
			break;
		case 0:
		default: (void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
		break;
		}
	}
/****************************************************************************************/
	if(event == drome_OutEvent_start_1st_increment)
	{
		TimeSet_BackIndex = 0;
		TimeSet_ConfirmIndex = 0;
		Hr1_value++;

		if(AM_flag < 2)	/*12Hr*/
		{

			if(Hr1_value >12)
			{
				Hr1_value = 1;
			}
		}
		else
		{
			if(Hr1_value >23)
			{
				Hr1_value = 0;
			}
		}
	}

	if(event == drome_OutEvent_start_1st_decrement)
	{
		TimeSet_BackIndex = 0;
		TimeSet_ConfirmIndex = 0;
		Hr1_value--;

		if(AM_flag < 2)	/*12Hr*/
		{

			if(Hr1_value < 1)
			{
				Hr1_value = 12;
			}
		}
		else
		{
			if(Hr1_value <0)
			{
				Hr1_value = 23;
			}
		}
	}
	if(event == drome_OutEvent_start_3rd_increment)
	{
		Min1_value++;

		if(Min1_value >59)
		{
			Min1_value = 0;
		}
	}

	if(event == drome_OutEvent_start_3rd_decrement)
	{
		Min1_value--;

		if(Min1_value < 0)
		{
			Min1_value = 59;
		}
	}


	if(event == drome_OutEvent_start_5th_increment)
	{
		AMPM_value++;
		if(TimeF_selected == 1)
		{
			if(AMPM_value >1)
			{
				AMPM_value = 0;
			}
		}
		else
		{
			AMPM_value = 2;
		}
	}

	if(event == drome_OutEvent_start_5th_decrement)
	{
		AMPM_value++;
		if(TimeF_selected == 1)
		{
			if(AMPM_value >1)
			{
				AMPM_value = 0;
			}
		}
		else
		{
			AMPM_value = 2;
		}
	}

	if(event == drome_OutEvent_Backsave_time)
	{
		//		Timesetting_completed = 1;
		changed_hr = Hr1_value;/*(Hr1_value*10)+Hr0_value;*/
		if((TimeF_selected == 2)&&(changed_hr >23))
		{
			Timesetting_completed = 0;
		}
		else
		{
			Timesetting_completed = 1;
		}
		changed_min = Min1_value;/*(Min1_value*10)+Min0_value;*/
		AM_flag = AMPM_value;
	}
	if(event == drome_OutEvent_TimeS_Highlight_Confirm)
	{
		TimeSet_BackIndex = 0;
		TimeSet_ConfirmIndex = 1;
	}
	if(event == drome_OutEvent_TimeS_Nohighlight_Confirm)
	{
		TimeSet_BackIndex = 1;
		TimeSet_ConfirmIndex = 0;
	}
	if(event == drome_OutEvent_TimeS_NoHighlight_Back)
	{
		TimeSet_BackIndex = 0;
		TimeSet_ConfirmIndex = 0;
	}
	/************************************** TIME SETTING END *****************************************/
	/************************************ WARNING HISTORY START **************************************/
	if(event == drome_OutEvent_WH_Page)
	{
		Warning_entry = 1;
		if(once_enter == 0)
		{
			once_enter++;

			for(WH_scan=0;WH_scan<40/*30*//*20*/;WH_scan++)
			{
				if(alerts_history[WH_scan]==1)
				{
					filter[temp_size]=WH_scan;
					temp_size++;
				}
			}
			for(temp_size=0;temp_size<37/*28*//*10*//*5*/;temp_size++)
			{
				if(filter[temp_size]!=0xFF)	/*Default*/
				{
					size++;
				}
			}

			if(size == 0)
			{
				No_warning_index = 1;
				filter[0]=37;//28;			/*Warning History Last image index for No warning*/

			}
			else
			{
				No_warning_index = 0;
			}
		}

	}

	if(event == drome_OutEvent_WH_Back)
	{
		once_enter = 0;
		i = 0;
		 for(i=0;i<37/*28*//*10*//*5*/;i++)
		 {
			 filter[i]=0xFF;

		 }
		 once_enter = 0;
		 size = 0;
		 temp_size = 0;
		 Warning_entry = 0;
	}
/*****************************WARNING HISTORY END**************************************************/

/***************************** TEMP UNIT SELECTION START ******************************************/

		if(event == drome_OutEvent_TempU_1st_row_selected)
		{
			TempU_selected = 0;					/*Deg C*/
		}
		else if(event == drome_OutEvent_TempU_2nd_row_selected)
		{
			TempU_selected = 1;					/*Deg F*/
		}

		/*************************/
		if(event == drome_OutEvent_TempU_Entered)
		{
			switch (TempU_selected)
			{
			case 0:
				(void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
				break;
			case 1:
				(void)drome_sendEventToDrome((U32)drome_InEvent_2nd_detected);
				break;
			default: (void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
			break;
			}
		}
/******************************** TEMP UNIT SELECTION END***************************************/
/********************************** ETM TFT TEST START *****************************************/

		if(event == drome_OutEvent_TFT_Check_start)
		{
			ETM_test_start_timeout = 1;//1.5sec
		}
		if(event == drome_OutEvent_TFT_Check_end)
		{
			ETM_test_start_timeout = 0;//1.5sec
			ETM_TFT_test_index = 0;
		}
/***************************** ETM TFT TEST END **********************************************/
/***************************** Passenger Airbag Setting START **********************************************/

		if(event == drome_OutEvent_PAB_selected)
		{
			PAB_selected = 0;					/*PAB Enable*/
		}
		else if(event == drome_OutEvent_PAB_deselected)
		{
			PAB_selected = 1;					/*PAB Disable*/

		}
		if(event == drome_OutEvent_Setting_Entered)
		{
			Setting_timeout_start = 1;
		}
		if(event == drome_OutEvent_PAB_Entered)
				{

			switch (PAB_selected)
			{
			case 0:
				(void)drome_sendEventToDrome((U32)drome_InEvent_1st_detected);
				break;
			case 1:
			default: (void)drome_sendEventToDrome((U32)drome_InEvent_2nd_detected);
			break;
			}

		}

/***************************** Passenger Airbag Setting END **********************************************/
		if(event == drome_OutEvent_PAB_enable_entered)
		{

			PAB_Enable_Yes_No_select = 1;
		}

		if(event == drome_OutEvent_PAB_disable_entered)
		{
			PAB_Disable_Yes_No_select = 1;
		}
		if(event == drome_OutEvent_PAB_disable_Yes_detected)
		{
			PAB_Disable_Yes_No_select = 1;
		}
		else if(event == drome_OutEvent_PAB_disable_No_detected)
		{
			PAB_Disable_Yes_No_select = 0;
		}

		if(event == drome_OutEvent_PAB_enable_Yes_detected)
		{
			PAB_Enable_Yes_No_select = 1;
		}
		else if(event == drome_OutEvent_PAB_enable_No_detected)
		{
			PAB_Enable_Yes_No_select = 0;
		}

/***************************** Passenger Airbag Setting END **********************************************/
/*********************************** Drive Summary *******************************************************/
		if(event == drome_OutEvent_Drive_Sum_Alert_started)
		{
			start_driveSum_timeout_3sec=1;
		}
/*********************************************************************************************************/
/*********************************** Alert Skip *******************************************************/
		if(event == drome_OutEvent_Skip_EngOilPressureLow)
		{
			Low_Engine_Oil_Pressure_popup_shown=0;
			Low_Engine_Oil_Pressure_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_EngTempHigh)
		{
			Engine_Temp_High_popup_shown=0;
			Engine_Temp_High_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_LowFuel)
		{
			Low_Fuel_popup_shown=0;
			Low_Fuel_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_CheckEng)
		{
			Check_Engine_popup_shown=0;
			Check_Engine_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_LowBrakefluid)
		{
			Low_Brake_Fluid_popup_shown=0;
			Low_Brake_Fluid_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_ParkBrake)
		{
			Park_brake_popup_shown=0;
			Park_brake_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_PABoff)
		{
			PAB_Off_popup_shown=0;
			PAB_Off_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_DPFclogged)
		{
			DPF_Clogged_popup_shown=0;
			DPF_Clogged_popup_skip = 1;
			skip_flag = 1;
//			dpf_check = 0;
		}
		else if(event == drome_OutEvent_Skip_RegenRequired)
		{
			REGEN_Required_popup_shown = 0;
			REGEN_Required_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RegenRequested)
		{
			//Check_Engine_popup_shown=0;
			//Check_Engine_popup_skip = 1;
			//skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RegenStarted)
		{
			REGEN_started_popup_shown=0;
			REGEN_started_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RegenNotStarted)
		{
			REGEN_notstarted_popup_shown=0;
			REGEN_notstarted_popup_skip = 1;
			regen_req_start = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RegenCompleted)
		{
			REGEN_completed_popup_shown=0;
			REGEN_completed_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RegenAborted)
		{
			REGEN_aboarted_popup_shown = 0;
			REGEN_aboarted_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_DoorOpen)
		{
			Dooropen_popup_shown=0;
			Dooropen_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_EBDmalfunction)
		{
			EBD_popup_shown=0;
			EBD_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_ABSmalfunction)
		{
			Check_ABS_popup_shown=0;
			Check_ABS_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_ESPmalfunction)
		{
			ESP_popup_shown=0;
			ESP_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RPASsupplyFailure)
		{
			RPAS_Supply_failure_popup_shown=0;
			RPAS_Supply_failure_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RKE_Battery_Low)
		{
			Low_Battery_popup_shown=0;
			Low_Battery_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_WaterInFuel)
		{
			Water_In_Fuel_popup_shown=0;
			Water_In_Fuel_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_ServiceReminder)
		{
			Service_Reminder_popup_shown=0;
			Service_Reminder_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_FuncSafety)
		{
			Func_safety_popup_shown=0;
			Func_safety_popup_skip = 1;
			skip_flag = 1;
		}
		/**/
		else if(event == drome_OutEvent_Skip_LeftDRL)
		{
			Left_DRL_failure_popup_shown = 0;
			Left_DRL_failure_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_LeftTurn)
		{
			Left_Turn_failure_popup_shown = 0;
			Left_Turn_failure_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RightDRL)
		{
			Right_DRL_failure_popup_shown = 0;
			Right_DRL_failure_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_RightTurn)
		{
			Right_Turn_failure_popup_shown = 0;
			Right_Turn_failure_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_TailLamp)
		{
			Tail_Lamp_failure_popup_shown = 0;
			Tail_Lamp_failure_popup_skip = 1;
			skip_flag = 1;
		}
		else if(event == drome_OutEvent_Skip_HornFailure)
		{
			Horn_Fail_popup_shown = 0;
			Horn_Fail_popup_skip = 1;
			skip_flag = 1;
		}
#ifdef SPRINT4
		else if(event == drome_OutEvent_Skip_TDMS)
		{	
			TDMS_popup_shown=0;
			TDMS_popup_skip = 1;
			skip_flag = 1;
		}
#endif

		/**/
/*DEF Skip alerts**/

		else if(event == drome_OutEvent_Skip_DEF_Dosing_Malfunction)		/*DEF DOSING LEVEL 1*/
		{
			DEF_Dosing_Malfunction_popup_shown=0;

			DEF_Dosing_Malfunction_popup_skip = 1;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			No_start_Dose_Malfunction_popup_skip = 0;

			DEF_Level_Low_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 0;
		}
		else if(event == drome_OutEvent_Skip_DEF_Dosing_Malfunction_Engine)	/*DEF DOSING LEVEL 2*/
		{
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_shown=0;

			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 1;
			DEF_Dosing_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			No_start_Dose_Malfunction_popup_skip = 0;

			DEF_Level_Low_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}
		else if(event == drome_OutEvent_Skip_No_restart_Dose_Malfunction)	/*DEF DOSING LEVEL 3*/
		{
			No_restart_Dose_Malfunction_popup_shown=0;

			No_restart_Dose_Malfunction_popup_skip = 1;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;
			No_start_Dose_Malfunction_popup_skip = 0;

			DEF_Level_Low_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}
		else if(event == drome_OutEvent_Skip_No_start_Dose_Malfunction)		/*DEF DOSING LEVEL 4*/
		{
			No_start_Dose_Malfunction_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 1;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			DEF_Level_Low_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}
		else if(event == drome_OutEvent_Skip_DEF_Level_Low)					/*DEF LEVEL 1*/
		{
			DEF_Level_Low_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			DEF_Level_Low_popup_skip = 1;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 0;
		}
		else if(event == drome_OutEvent_Skip_DEF_level_Low_Engine_Not_Start)/*DEF LEVEL 2*/
		{
			DEF_level_Low_Engine_Not_Start_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			DEF_level_Low_Engine_Not_Start_popup_skip = 1;
			DEF_Level_Low_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}


		else if(event ==  drome_OutEvent_Skip_No_restart_Fill_DEF)			/*DEF LEVEL 3*/
		{
			No_restart_Fill_DEF_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			No_restart_Fill_DEF_popup_skip = 1;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			DEF_Level_Low_popup_skip = 0;
			No_start_Fill_DEF_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}

		else if(event == drome_OutEvent_Skip_No_start_Fill_DEF)				/*DEF LEVEL 4*/
		{
			No_start_Fill_DEF_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			No_start_Fill_DEF_popup_skip = 1;
			No_restart_Fill_DEF_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			DEF_Level_Low_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 0;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}

		else if(event == drome_OutEvent_Skip_Incorrect_DEF_Detected)		/*INCORRECT LEVEL 1*/
		{
			Incorrect_DEF_Detected_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			No_start_Fill_DEF_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			DEF_Level_Low_popup_skip = 0;

			Incorrect_DEF_Detected_popup_skip = 1;
			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;


			skip_flag = 1;
			BS6_permanent_skip = 0;
		}
		else if(event == drome_OutEvent_Skip_Incorrect_DEF_Detected_Engine)	/*INCORRECT LEVEL 2*/
		{
			Incorrect_DEF_Detected_Engine_Not_Start_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			No_start_Fill_DEF_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			DEF_Level_Low_popup_skip = 0;

			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 1;
			Incorrect_DEF_Detected_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}
		else if(event == drome_OutEvent_Skip_No_restart_Check_DEF)			/*INCORRECT LEVEL 3*/
		{
			No_restart_Check_DEF_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			No_start_Fill_DEF_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			DEF_Level_Low_popup_skip = 0;

			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			Incorrect_DEF_Detected_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 1;


			skip_flag = 1;
			BS6_permanent_skip = 1;
		}
		else if(event == drome_OutEvent_Skip_No_start_Check_DEF)			/*INCORRECT LEVEL 4*/
		{
			No_start_Check_DEF_popup_shown=0;

			No_start_Dose_Malfunction_popup_skip = 0;
			No_restart_Dose_Malfunction_popup_skip = 0;
			DEF_Dosing_Malfunction_Engine_Not_Start_popup_skip = 0;
			DEF_Dosing_Malfunction_popup_skip = 0;

			No_start_Fill_DEF_popup_skip = 0;
			No_restart_Fill_DEF_popup_skip = 0;
			DEF_level_Low_Engine_Not_Start_popup_skip = 0;
			DEF_Level_Low_popup_skip = 0;

			Incorrect_DEF_Detected_Engine_Not_Start_popup_skip = 0;
			Incorrect_DEF_Detected_popup_skip = 0;
			No_restart_Check_DEF_popup_skip = 0;
			No_start_Check_DEF_popup_skip = 1;

			skip_flag = 1;
			BS6_permanent_skip = 1;
		}
		else
		{
//			skip_flag = 0;
//			BS6_permanent_skip = 0;
		}
/***************************************************************************************************************/
		if(event == drome_OutEvent_TripA)
		{
			temp_default_page = 21;
			default_page = 21;
		}
		else if(event == drome_OutEvent_TripA_AVS)
		{
			temp_default_page = 12;
			default_page = 12;
		}
		else if(event == drome_OutEvent_TripAC)
		{
			temp_default_page = 18;
			default_page = 18;
		}
		else if(event == drome_OutEvent_TripA_AVS_DT)
		{
			temp_default_page = 11;
			default_page = 11;
		}
		else if(event == drome_OutEvent_TripA_DT)
		{
			temp_default_page = 13;
			default_page = 13;
		}
		else if(event == drome_OutEvent_TripB)
		{
			temp_default_page = 14;
			default_page = 14;
		}
		else if(event == drome_OutEvent_TripB_AVS)
		{
			temp_default_page = 16;
			default_page = 16;
		}
		else if(event == drome_OutEvent_TripB_AVS_DT)
		{
			temp_default_page = 15;
			default_page = 15;
		}
		else if(event == drome_OutEvent_TripB_DT)
		{
			temp_default_page = 17;
			default_page = 17;
		}
		else if(event == drome_OutEvent_Digitalclock)
		{
			temp_default_page = 19;
			default_page = 19;
		}
		else if(event == drome_OutEvent_Regen)
		{
			temp_default_page = 20;
			default_page = 20;
		}

		/*else if(event == drome_OutEvent_FC_AFE)
		{
			temp2_default_page = 23;
			default_page = 23;
		}
		else if(event == drome_OutEvent_FC_AFE_IFE)
		{
			temp2_default_page = 22;
			default_page = 22;
		}
		else if(event == drome_OutEvent_FC_DTE)
		{
			temp2_default_page = 25;
			default_page = 25;
		}
		else if(event == drome_OutEvent_FC_ECO)
		{
			temp2_default_page = 26;
			default_page = 26;
		}
		else if(event == drome_OutEvent_FC_IFE)
		{
			temp2_default_page = 24;
			default_page = 24;
		}
		else if(event == drome_OutEvent_FC_Page)
		{
			default_page = temp2_default_page;
		}*/
#ifdef COLOR_TFT
		if(event == drome_OutEvent_Vehicle_Info_Exit)
		{
			if(temp_default_page <= 10)
			{
				temp_default_page = 21;			/*for scrolling from 4X4, default page not matches then go to Trip A*/
			}
			default_page = temp_default_page;
		}
#endif
		if(event == drome_OutEvent_Driveinfo_page)
		{
			default_page = temp_default_page;
			//			scroll_completed = 1;
		}

/***************************************************************************************************************/

/*********************************************************************************************************/
}

/*************************************************************************************/
Private
void
convertTimeToDpString(
		U8 index,    /**< index in DP_TABLE */
		U16 *arg)    /**< pointer to dp string */
/** Put rtc time in to dp string at correct format
 **************************************************************************************/
{
#if 0
	timeconv_TimeAndDate td;
	U8 temp;
	U32 time_sec;

	errmgr_ASSERT(errmgr_LOG, dl_getDataType(DP_TABLE[index].dl_parameter) == DP_TABLE[index].dl_parameter_size);

	(void)dl_GET_DATA(DP_TABLE[index].dl_parameter, dl_DR_ANONYMOUS, &time_sec);

	timeconv_convertSecToTimeAndDate(time_sec, &td);

	*arg = 0u;

	temp = td.hour / 10u;
	temp += 0x30u;
	*arg = temp;

	arg++;
	temp = td.hour % 10u;
	temp += 0x30u;
	*arg = temp;

	arg++;
	*arg = 0x003Au; /* : */

	arg++;
	temp = td.minute / 10u;
	temp += 0x30u;
	*arg = temp;

	arg++;
	temp = td.minute % 10u;
	temp += 0x30u;
	*arg = temp;

	arg++;
	*arg = 0u;
#endif
}
#if 0
/*************************************************************************************/
Public
void
handleBargraph(
		void)

/** Handles button presses and executes picture changes in gui.
 **************************************************************************************/
{

	//ButtonValueEnum button_value = BUTTON_RIGHT; /*BUTTON_NONE;*/ /*continue fetch Right Key*/
	U8 i = 0u;

	/* Check new button presses */
	/*while ((!button_input) && (BUTTON_TABLE[i].button_value != BUTTON_NONE))
   {
      if (!(dl_GET_DATA(BUTTON_TABLE[i].button_id, dl_DR_GUI, &button_input)))
      {
         button_input = 0u;
         button_value = BUTTON_NONE;
      }
      else
      {
         button_value = BUTTON_TABLE[i].button_value;
      }
      i++;
   }*/	/*change for w501*/

	/* The event handling */
	//if (button_input)
	// {

	switch (button_value)
	{
	case BUTTON_UP:
		(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_RIGHT:
		(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Return);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_DOWN:
		(void)drome_sendEventToDrome((U32)drome_InEvent_Temp_Jump);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_LEFT:
		(void)drome_sendEventToDrome((U32)drome_InEvent_Temp_Return);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_NONE:
	default:
		break;
	}

	// button_input = 0;
	// }
}
#endif

/*************************************************************************************/
Public
void
handleButtonPress(
		void)

/** Handles button presses and executes picture changes in gui.
 **************************************************************************************/
{

	//ButtonValueEnum button_value = BUTTON_RIGHT; /*BUTTON_NONE;*/ /*continue fetch Right Key*/
	U8 i = 0u;

	/* Check new button presses */
	/*while ((!button_input) && (BUTTON_TABLE[i].button_value != BUTTON_NONE))
   {
      if (!(dl_GET_DATA(BUTTON_TABLE[i].button_id, dl_DR_GUI, &button_input)))
      {
         button_input = 0u;
         button_value = BUTTON_NONE;
      }
      else
      {
         button_value = BUTTON_TABLE[i].button_value;
      }
      i++;
   }*/	/*change for w501*/

	/* The event handling */
	//if (button_input)
	// {
#if 0
	switch (button_value)
	{
	case BUTTON_UP:
		(void)drome_sendEventToDrome((U32)drome_KB_OK);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_RIGHT:
		(void)drome_sendEventToDrome((U32)drome_KB_MENU);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_DOWN:
		(void)drome_sendEventToDrome((U32)drome_KB_DOWN);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_LEFT:
		(void)drome_sendEventToDrome((U32)drome_KB_UP);
		button_value = BUTTON_NONE;
		break;
	case BUTTON_NONE:
	default:
		break;
	}
#endif
	// button_input = 0;
	// }
}


/*************************************************************************************/
Public 
void 
gui_handleDromeCodeError(
		U32 arg1,     /**<[in] arg1 not used */
		U32 arg2)     /**<[in] arg2 not used */
/** Not used function.
 **************************************************************************************/
{
	//  NOT_USED(arg1);
	//   NOT_USED(arg2);	/*changes for w501*/
}

/*************************************************************************************/
Public 
void 
gui_loadHistoryStates(
		U32* arg1,     /**<[in] arg1 not used */
		U16 arg2)      /**<[in] arg2 not used */
/** Not used function.
 **************************************************************************************/
{
	//  NOT_USED(arg1);
	//  NOT_USED(arg2);
}

/*************************************************************************************/
Public 
void 
gui_saveHistoryStates(
		const U32* arg1,     /**<[in] arg1 not used */
		U16 arg2)            /**<[in] arg2 not used */
/** Not used function.
 **************************************************************************************/
{
	//  NOT_USED(arg1);
	//  NOT_USED(arg2);
}

/*************************************************************************************/
Public 
U32 
gui_getUserConfChecksum(
		void)
/** \return CRC32-code of user config options at compile time. 
 **************************************************************************************/
{
	return drome_USER_CONFIG_CRC32;
}

/*************************************************************************************/
Public 
U32 
gui_getNullEvent(
		void)
/** \return start of drome_Event table.
 **************************************************************************************/
{
	return (U32)drome_NULL_EVENT;
}

/*************************************************************************************/
Public 
U32 
gui_getLastInEvent(
		void)
/** Get drome_LAST_IN_EVENT.
 **************************************************************************************/
{
	return (U32)drome_LAST_IN_EVENT;
}

/*************************************************************************************/
Public 
U32 
gui_getMaxEvents(
		void)
/** Get drome_MAX_EVENTS.
 **************************************************************************************/
{
	return (U32)drome_MAX_EVENTS;
}


/***********************************************************************************/
Public 
U32 
gui_getTimestamp(
		void)
/** \return time stamp in microseconds.
 **************************************************************************************/
{
/*changes for w501*/
//	return ostimer_getMicrosecondTick();
	return drome_count1us;
}

/***********************************************************************************/
Public 
U32 
gui_getTimestampMS(
		void)
/** \return time stamp in milliseconds. 
 **************************************************************************************/
{
//   return ostimer_getMillisecondTick();
	WDTA1WDTE = 0xAC;
	return drome_count1us*5;

}
/***********************************************************************************/
Public 
void 
gui_handleDromeError(
		drome_ErrorCode error_code)          /**<[in] The error code */
/** Drome error function.
 **************************************************************************************/
{
	if ((U32)error_code & (U32)drome_ERR__FATAL)
	{
		priv_drome_state = DROME_STATE_OFF;	/*changes for w501*/
	}

}

#if 0
/*************************************************************************************/
Private
void
configureVideoIn(
		void)
/** Configures the video if available and required.
 ** \return None
 **************************************************************************************/
{
#ifdef drome_CONF_WIDGET_VIDEO
	drome_enableVideo(TRUE);
#endif
}
#endif

/*************************************************************************************/
Public
Boolean
gui_i2cOpen(
		void)
/**
 **  This function request to acquire i2cdrv_RTC_LCD_ISOUND i2c bus.
 **
 ** \return None
 **************************************************************************************/
{
	/* Boolean return_value;

   priv_i2c_hdl = i2cdrv_open(i2cdrv_RTC_LCD_ISOUND);

   return_value=TRUE;
   if(priv_i2c_hdl == 0u)
   {
           return_value=FALSE;
   }

   return return_value;*/	/*changes for w501*/
}


/*************************************************************************************/
Public
void
gui_i2cClose(
		void)
/**
 **  This function request to release i2cdrv_RTC_LCD_ISOUND i2c bus.
 **
 ** \return None
 **************************************************************************************/
{
	/*i2cdrv_close(i2cdrv_RTC_LCD_ISOUND, priv_i2c_hdl);
   priv_i2c_hdl = 0u;*/	/*changes for w501*/
}


/*************************************************************************************/
Public
void
gui_i2cIssueStop(
		void)
/**
 **  This function issues a stop condition on the i2c bus.
 **
 ** \return None
 **************************************************************************************/
{
	//(void)i2cdrv_issueStop((U8)i2cdrv_RTC_LCD_ISOUND, priv_i2c_hdl);	/*changes for w501*/
}


/*************************************************************************************/
Public
Boolean
gui_i2cWriteStart(U8        adr,     /**<[in] I2C-adress */
		const U8* txbuf,   /**<[in] Data to write */
		U16       txnum)   /**<[in] Number of bytes to write */
/**
 **  This function starts a write on the i2c bus addressing the video decoder.
 **
 ** \return Boolean TRUE if write succeeded, otherwise FALSE.
 **************************************************************************************/
{
	// return i2cdrv_write(i2cdrv_RTC_LCD_ISOUND, priv_i2c_hdl, adr, txbuf, txnum, 0u);	/*changes for w501*/
}


/*************************************************************************************/
Public
Boolean
gui_i2cReadStart(U8         adr,     /**<[in] I2C-adress */
		const U8*  txbuf,   /**<[in] Data to write */
		U16        txnum,   /**<[in] Number of bytes to write */
		U8*        rxbuf,   /**<[out] Buffer for read data */
		U16        rxnum)   /**<[in] Number of bytes to read */
/**
 **  This function starts a read on the i2c bus addressing the video decoder.
 **
 ** \return Boolean TRUE if read succeeded, otherwise FALSE.
 **************************************************************************************/
{
	/*Boolean ret_val;

   ret_val = i2cdrv_write(i2cdrv_RTC_LCD_ISOUND, priv_i2c_hdl, adr, txbuf, txnum, 0u);

   if (ret_val)
   {
      ret_val = i2cdrv_read(i2cdrv_RTC_LCD_ISOUND, priv_i2c_hdl, adr, rxbuf, rxnum, 0u);
   }

   return ret_val;*/	/*changes for w501*/
}


/*************************************************************************************/
Public
Boolean
gui_getScaledVideoSize(U8   scale_index,         /**<[in] scale index */
		U16* scaled_width_ref,    /**<[out] scaled width */
		U16* scaled_height_ref)   /**<[out] scaled height */
/**
 **  This function returns the video scaling for selected index.
 **
 ** \return Boolean TRUE if scaling succeeded, otherwise FALSE.
 **************************************************************************************/
{
	Boolean scaling_ok = FALSE;

	if (((U16)scale_index * 2u) < ((U16)sizeof(priv_video_scaling) / (U16)sizeof(U16)))
	{
		*scaled_width_ref  = priv_video_scaling[scale_index * 2u];
		*scaled_height_ref = priv_video_scaling[(scale_index * 2u) + 1u];
		scaling_ok         = TRUE;
	}

	return scaling_ok;
}

#if 0
/*************************************************************************************/
Private
void
doInitVideoIn(
		void)
/**
 **  This function initializes the video interface input pins.
 **
 ** \return None
 **************************************************************************************/
{
	/*
	 * P28.02 DI_VIDEO_ITU0 VI0R3: ALT_IN2
	 * P28.03 DI_VIDEO_ITU1 VI0R4: ALT_IN2
	 * P28.04 DI_VIDEO_ITU2 VI0R5: ALT_IN2
	 * P28.05 DI_VIDEO_ITU3 VI0R6: ALT_IN2
	 * P28.06 DI_VIDEO_ITU4 VI0R7: ALT_IN2
	 * P28.07 DI_VIDEO_ITU5 VI0G2: ALT_IN2
	 * P28.08 DI_VIDEO_ITU6 VI0G3: ALT_IN2
	 * P28.09 DI_VIDEO_ITU7 VI0G4: ALT_IN2
	 */
	PMCSR28 = 0x03FC03FCul;
	PFC28 |= 0x03FCu;
	PMSR28 = 0x03FC03FCul;

	/*
	 * P28.00 DI_VIDEO_CLK VI0CLK: ALT_IN2
	 */
	PMCSR28 = 0x00010001ul;
	PFC28 |= 0x0001u;
	PMSR28 = 0x00010001ul;

	/*
	 * P1.04 DO_VIDEO_RESET_N P1.04 OUT
	 */
	PMCSR1 = 0x00100000ul;
	PMSR1 = 0x00100000ul;

	setResetVideoIn(TRUE);
}


/*************************************************************************************/
Private
void
enableVideoIn(
		void)
/**
 **  This function enables the video interface input.
 **
 ** \return None
 **************************************************************************************/
{
	/*
	 ** Video interface control register
	 ** Bit 3: 1 VI0 not in reset
	 ** Bit 0: 1 VI0 enable
	 */
	do {
		PROTCMD5 = 0xa5u;
		GSVCTRL = 0x00000009UL;
		GSVCTRL = ~(0x00000009UL);
		GSVCTRL = 0x00000009UL;
	} while (0x00u != PROTS5);

}


/*************************************************************************************/
Private
void
setResetVideoIn(
		Boolean reset)   /**<[in] reset */
/**
 **  This function controls the video decoder reset pin.
 **
 ** \return None
 **************************************************************************************/
{
	/* DO_VIDEO_DECODER_RESET P1.04*/
	if (reset)
	{
		P1 |= 0x0010u;
	}
	else
	{
		P1 &= ~(0x0010u);
	}
}
#endif
/***********************FOR MSIL HMI TESTING******************************************/
#if 1
Private void getAPG1value(U8 index, U32 *arg)	/*changes for w501*/
{
	/*U8 APG_value = 0u;

	errmgr_ASSERT(errmgr_LOG, dl_getDataType(DP_TABLE[index].dl_parameter) == DP_TABLE[index].dl_parameter_size);
#if 0
	(void)dl_GET_DATA(DP_TABLE[index].dl_parameter, dl_DR_ANONYMOUS, &APG_value);
#endif

	(void)dl_GET_DATA(DP_TABLE[index].dl_parameter, dl_DR_ANONYMOUS, &APG_value);

	if(APG_value > 100u)
	{
		APG_value = 0u;
	}
	else
	{
		APG_value++;
	}

	dl_PUT_DATA(DP_TABLE[index].dl_parameter, APG_value);
#if 1
	(void)dl_GET_DATA(DP_TABLE[index].dl_parameter, dl_DR_ANONYMOUS, &APG_value);
#endif
	 *arg = APG_value;*/
}
#endif

Public const r_ddb_Timing_t * app_getDispParams(void)
{
	return &dispParams[0];
}

Private const r_wm_OutColorFmt_t app_getOutputColorFormat(void)
{
	return (R_WM_OUTCOLORFMT_RGB666 | R_WM_OUTCOLORFMT_FLAG_SWAP_BR);
}
Public void handleDromeState(void)
{
	static U8 redraw_wait = 0u;
	drome_CyclicStat  ret_cyclic = 0;
	static Boolean once;
#ifdef GRAYSCALE_TFT
	if(HANDLE_DROME_START == 1)
	{
		return;
	}

	if(entry_drivesummery == 1)
	{
		redraw_wait = 5;
		entry_drivesummery = 0;
	}
#endif
	/*if(gui_shutdown == 1)
	{
		gui_shutdown = 0;
		priv_drome_state = DROME_STATE_REINIT;
	}*/
	switch (priv_drome_state) {
	case DROME_STATE_DO_INIT:
		/*   DEBUG_PRINT("handleDromeState(), state = DROME_STATE_DO_INIT");*/
#if 0
		doInitVideoIn();
		enableVideoIn();
		configureVideoIn();*/
#endif
		//			 drome_enableVideo(FALSE);
		drome_init(DROME_DB_ADDRESS, DROME_IMG_ADDRESS, &priv_drome_CallbackFunctPtrs, DROME_LANG_PACK_ADDRESS); /*lint !e923 Casting from integer to pointer is OK in this case. */ /*lint !e960 */
		priv_drome_state  = DROME_STATE_INIT_STARTED;
		break;

	case DROME_STATE_INIT_STARTED:
		/*if (!once) {
	            DEBUG_PRINT("handleDromeState(), state = DROME_STATE_INIT_STARTED");
	            once = TRUE;
	         }*/
		ret_cyclic = drome_doCyclic();

		if ((ret_cyclic == drome_CYCLIC_FINISHED) || (ret_cyclic == drome_CYCLIC_IDLE))
		{
			priv_drome_state = DROME_STATE_RUNNING;
			once = FALSE;
		}

		break;

	case DROME_STATE_RUNNING:
		if (!once) {
			/* DEBUG_PRINT("handleDromeState(), state = DROME_STATE_RUNNING");*/
			once = TRUE;
		}
		switch (drome_doCyclic()) {
		case drome_CYCLIC_ERROR:
			skip_redraw = 1;
			break;

		case drome_CYCLIC_IDLE:
		case drome_CYCLIC_FINISHED:
			skip_redraw = 1;
			ret_redraw = 0;
			redraw_wait++;
			WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
			if (redraw_wait >= 1U) {
				ret_redraw = /*(void)*/drome_redrawView();
				redraw_wait = 0U;
				skip_redraw = 0;
			}
			break;

		default:
			break;
		}
		break;

		case DROME_STATE_REINIT:
			/* DEBUG_PRINT("handleDromeState(), state = DROME_STATE_REINIT");*/
			if (drome_shutDown()) {
				drome_init(DROME_DB_ADDRESS, DROME_IMG_ADDRESS, &priv_drome_CallbackFunctPtrs, DROME_LANG_PACK_ADDRESS); /*lint !e923 Casting from integer to pointer is OK in this case. */ /*lint !e960 */
				priv_drome_state = DROME_STATE_RUNNING;
				once = FALSE;
			}
			break;

		default:
			break;
	}
#ifdef GRAYSCALE_TFT
	if(flag_grayscale > (refresh_rate - 4))
	{
		HANDLE_DROME_START = 1;
		/*start_drawgrayscale = 0;*/
	/*	init_grayscale = 0;*/
	}
#endif

}

/*************************************************************************************/
/** \}
 *********************************** End of file **************************************/

