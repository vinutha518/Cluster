/*
 * UDS_DTC.c
 *
 *  Created on: Feb 27, 2019
 *      Author: MSWELT12
 */

/* **********************************************************************************************************************
   File                    :       UDS_DTC.c
   Author                  :       Akshay Wankhade
   Model                   :       Mahindra W501
   Revision Author         :
   Reviewed By             :
   Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

   /***********************************************************************************************************************
   Includes
   ********************************************************************************************************************** */
#include "dr7f701461.dvf.h"
#include "r_typedefs.h"
#include "my_can.h"
#include "UDSS.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "UDS_DTC.h"
#include "Memmory_Map.h"
#include "my_SeedKey.h"
#include "r_cg_serial_user.h"
#include "digitalGuage.h"
#include "ADC.h"

#define DTC_CLEAR 0
#define FREEZE_FRAME_DATA_SIZE 		12

_Bool clear_dtc_log_flag;
extern int8_t Engine_Speed ;
extern _Bool EEPROM_IN_USE;
uint8_t freeze_frame_array[12];
uint32_t batt_vtg;//16
uint16_t Diag_Ignition_counter;
_Bool CAN_Bus_Off = CLEAR;

extern _Bool MODE_BUTTON_STUCK_FLAG;
extern _Bool SET_BUTTON_STUCK_FLAG;
typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}TimeStamp;
Private SplitWord aging_flag_0;
Private SplitWord aging_flag_1;
Private SplitWord aging_flag_2;
Private SplitWord aging_flag_3;
Private SplitWord aging_flag_4;
Private SplitWord aging_flag_5;
Private SplitWord aging_flag_6;
Private SplitWord aging_flag_7;
Private SplitWord aging_flag_8;
Private SplitWord aging_flag_9;
Private SplitWord aging_flag_10;
Private SplitWord aging_flag_11;
Private SplitWord aging_flag_12;
Private SplitWord aging_flag_13;
Private SplitWord aging_flag_14;
Private SplitWord aging_flag_15;
Private SplitWord aging_flag_16;
Private SplitWord aging_flag_17;
Private SplitWord aging_flag_18;
Private SplitWord aging_flag_19;
Private SplitWord aging_flag_20;
Private SplitWord aging_flag_21;
Private SplitWord aging_flag_22;
Private SplitWord aging_flag_23;
Private SplitWord aging_flag_24;
Private SplitWord aging_flag_25;
Private SplitWord aging_flag_26;
Private SplitWord aging_flag_27;
Private SplitWord aging_flag_28;
Private SplitWord aging_flag_29;
Private SplitWord aging_flag_30;
Private SplitWord aging_flag_31;
Private SplitWord aging_flag_32;
Private SplitWord aging_flag_33;
Private SplitWord aging_flag_34;
Private SplitWord aging_flag_35;
Private SplitWord aging_flag_36;
Private SplitWord aging_flag_37;
Private SplitWord aging_flag_38;
Private SplitWord aging_flag_39;
Private SplitWord aging_flag_40;
Private SplitWord aging_flag_41;
Private SplitWord aging_flag_42;
Private SplitWord aging_flag_43;
Private SplitWord aging_flag_44;
Private SplitWord aging_flag_45;
Private SplitWord aging_flag_46;
Private SplitWord aging_flag_47;
Private SplitWord aging_flag_48;
Private SplitWord aging_flag_49;
Private SplitWord aging_flag_50;
Private SplitWord aging_flag_51;
Private SplitWord aging_flag_52;
Private SplitWord aging_flag_53;
Private SplitWord aging_flag_54;
Private SplitWord aging_flag_55;
Private SplitWord aging_flag_56;
Private SplitWord aging_flag_57;
Private SplitWord aging_flag_58;
Private SplitWord aging_flag_59;
Private SplitWord aging_flag_60;
Private SplitWord aging_flag_61;
Private SplitWord aging_flag_62;
Private SplitWord aging_flag_63;
Private SplitWord aging_flag_64;
Private SplitWord aging_flag_65;
Private SplitWord aging_flag_66;
Private SplitWord aging_flag_67;
Private SplitWord aging_flag_68;
Private SplitWord aging_flag_69;
Private SplitWord aging_flag_70;
Private SplitWord aging_flag_71;
Private SplitWord aging_flag_72;
Private SplitWord aging_flag_73;
Private SplitWord aging_flag_74;
Private SplitWord aging_flag_75;
Private SplitWord aging_flag_76;
Private SplitWord aging_flag_77;
Private SplitWord aging_flag_78;
Private SplitWord aging_flag_79;
Private SplitWord aging_flag_80;
Private SplitWord aging_flag_81;
Private SplitWord aging_flag_82;
Private SplitWord aging_flag_83;
Private SplitWord aging_flag_84;
Private SplitWord aging_flag_85;
Private SplitWord aging_flag_86;
Private SplitWord aging_flag_87;
Private SplitWord aging_flag_88;
Private SplitWord aging_flag_89;
Private SplitWord aging_flag_90;
Private SplitWord aging_flag_91;
Private SplitWord aging_flag_92;
Private SplitWord aging_flag_93;
Private SplitWord aging_flag_94;
Private SplitWord aging_flag_95;
Private SplitWord aging_flag_96;
Private SplitWord aging_flag_97;
Private SplitWord aging_flag_98;
Private SplitWord aging_flag_99;
Private SplitWord aging_flag_100;
Private SplitWord aging_flag_101;
Private SplitWord aging_flag_102;
Private SplitWord aging_flag_103;
Private SplitWord aging_flag_104;
Private SplitWord aging_flag_105;
Private SplitWord aging_flag_106;
Private SplitWord aging_flag_107;
Private SplitWord aging_flag_108;
Private SplitWord aging_flag_109;
Private SplitWord aging_flag_110;
Private SplitWord aging_flag_111;
Private SplitWord aging_flag_112;
Private SplitWord aging_flag_113;
Private SplitWord aging_flag_114;
Private SplitWord aging_flag_115;

Private SplitWord counter_0;
Private SplitWord counter_1;
Private SplitWord counter_2;
Private SplitWord counter_3;
Private SplitWord counter_4;
Private SplitWord counter_5;
Private SplitWord counter_6;
Private SplitWord counter_7;
Private SplitWord counter_8;
Private SplitWord counter_9;
Private SplitWord counter_10;
Private SplitWord counter_11;
Private SplitWord counter_12;
Private SplitWord counter_13;
Private SplitWord counter_14;
Private SplitWord counter_15;
Private SplitWord counter_16;
Private SplitWord counter_17;
Private SplitWord counter_18;
Private SplitWord counter_19;
Private SplitWord counter_20;
Private SplitWord counter_21;
Private SplitWord counter_22;
Private SplitWord counter_23;
Private SplitWord counter_24;
Private SplitWord counter_25;
Private SplitWord counter_26;
Private SplitWord counter_27;
Private SplitWord counter_28;
Private SplitWord counter_29;
Private SplitWord counter_30;
Private SplitWord counter_31;
Private SplitWord counter_32;
Private SplitWord counter_33;
Private SplitWord counter_34;
Private SplitWord counter_35;
Private SplitWord counter_36;
Private SplitWord counter_37;
Private SplitWord counter_38;
Private SplitWord counter_39;
Private SplitWord counter_40;
Private SplitWord counter_41;
Private SplitWord counter_42;
Private SplitWord counter_43;
Private SplitWord counter_44;
Private SplitWord counter_45;
Private SplitWord counter_46;
Private SplitWord counter_47;
Private SplitWord counter_48;
Private SplitWord counter_49;
Private SplitWord counter_50;
Private SplitWord counter_51;
Private SplitWord counter_52;
Private SplitWord counter_53;
Private SplitWord counter_54;
Private SplitWord counter_55;
Private SplitWord counter_56;
Private SplitWord counter_57;
Private SplitWord counter_58;
Private SplitWord counter_59;
Private SplitWord counter_60;
Private SplitWord counter_61;
Private SplitWord counter_62;
Private SplitWord counter_63;
Private SplitWord counter_64;
Private SplitWord counter_65;
Private SplitWord counter_66;
Private SplitWord counter_67;
Private SplitWord counter_68;
Private SplitWord counter_69;
Private SplitWord counter_70;
Private SplitWord counter_71;
Private SplitWord counter_72;
Private SplitWord counter_73;
Private SplitWord counter_74;
Private SplitWord counter_75;
Private SplitWord counter_76;
Private SplitWord counter_77;
Private SplitWord counter_78;
Private SplitWord counter_79;
Private SplitWord counter_80;
Private SplitWord counter_81;
Private SplitWord counter_82;
Private SplitWord counter_83;
Private SplitWord counter_84;
Private SplitWord counter_85;
Private SplitWord counter_86;
Private SplitWord counter_87;
Private SplitWord counter_88;
Private SplitWord counter_89;
Private SplitWord counter_90;
Private SplitWord counter_91;
Private SplitWord counter_92;
Private SplitWord counter_93;
Private SplitWord counter_94;
Private SplitWord counter_95;
Private SplitWord counter_96;
Private SplitWord counter_97;
Private SplitWord counter_98;
Private SplitWord counter_99;
Private SplitWord counter_100;
Private SplitWord counter_101;
Private SplitWord counter_102;
Private SplitWord counter_103;
Private SplitWord counter_104;
Private SplitWord counter_105;
Private SplitWord counter_106;
Private SplitWord counter_107;
Private SplitWord counter_108;
Private SplitWord counter_109;
Private SplitWord counter_110;
Private SplitWord counter_111;
Private SplitWord counter_112;
Private SplitWord counter_113;
Private SplitWord counter_114;
Private SplitWord counter_115;
Private TimeStamp time_stamp;

//TT SHORT TO BATTERY
DTC DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_Buff;    /*0*/
DTC DTC_MIL_TT_STB_A003_Buff;					/*1*/
DTC DTC_ENG_ST_SP_TT_STB_A009_Buff;
DTC DTC_ABS_TT_STB_A00A_Buff;
DTC DTC_CRUISE_TT_STB_A00B_Buff;
DTC DTC_AIRBAG_TT_STB_A00C_Buff;
DTC DTC_PARKBRAKE_TT_STB_A00D_Buff;
DTC DTC_EPS_TT_STB_A00E_Buff;
DTC DTC_ATFAIL_TT_STB_A010_Buff;
DTC DTC_ESP_TT_STB_A01A_Buff;
//DTC_ESP_ACTIVE_TT_FAIL_A01A_Buff;

//TT SHORT TO GROUND
DTC DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_Buff;
DTC DTC_MIL_TT_STG_A003_Buff;
DTC DTC_ENG_ST_SP_TT_STG_A009_Buff;
DTC DTC_ABS_TT_STG_A00A_Buff;
DTC DTC_CRUISE_TT_STG_A00B_Buff;
DTC DTC_AIRBAG_TT_STG_A00C_Buff;
DTC DTC_PARKBRAKE_TT_STG_A00D_Buff;
DTC DTC_EPS_TT_STG_A00E_Buff;
DTC DTC_ATFAIL_TT_STG_A010_Buff;
DTC DTC_ESP_TT_STG_A01A_Buff;

//lvsd/hvsd
DTC DTC_HVSD_A000_Buff;
DTC DTC_LVSD_A000_Buff;

//Switch
//DTC DTC_BUTTON_STUCK_A000_Buff;
DTC DTC_MODE_SWITCH_STUCK_A01C_Buff;
DTC DTC_OK_SWITCH_STUCK_A01D_Buff;
DTC DTC_UP_SWITCH_STUCK_A01E_Buff;
DTC DTC_DOWN_SWITCH_STUCK_A01F_Buff;
DTC DTC_SET_SWITCH_STUCK_A022_Buff;

//MEMEORY
DTC DTC_ROM_FAIL_A001_Buff;
DTC DTC_RAM_FAIL_A001_Buff;
DTC DTC_EEPROM_FAIL_A001_Buff;

//FUEL
DTC DTC_FUEL_OC_FAIL_A004_Buff;
DTC DTC_FUEL_SC_FAIL_A004_Buff;

//WDT
DTC DTC_WDT_RST_FAIL_A012_Buff;

//CONFIGURATION
DTC DTC_CONFIG_CURR_FAIL_A014_Buff;

DTC DTC_HELTH_MONTR_FAIL_A015_Buff;

//Clock oscillator outside acceptable range
DTC DTC_CLOCK_OCC_FAIL_A016_Buff;

//NODE ABSENT
DTC DTC_EMS_NODEABSENT_D101_Buff;
DTC DTC_TCU_NODEABSENT_D102_Buff;
DTC DTC_ABS_ESC_NODEABSENT_D103_Buff;
DTC DTC_MBFM_NODEABSENT_D104_Buff;
DTC DTC_SRS_NODEABSENT_D106_Buff;
DTC DTC_IS_NODEABSENT_D108_Buff;
DTC DTC_SAS_NODEABSENT_D110_Buff;
DTC DTC_ICU_NODEABSENT_D117_Buff;
DTC DTC_PKE_NODEABSENT_D130_Buff;
DTC DTC_RPAS_NODEABSENT_D112_Buff;

//MESSAGE TIMEOUT
DTC DTC_EMS_MSGTIMEOUT_D001_Buff;
DTC DTC_TCU_MSGTIMEOUT_D002_Buff;
DTC DTC_ABS_ESC_MSGTIMEOUT_D003_Buff;
DTC DTC_MBFM_MSGTIMEOUT_D004_Buff;
DTC DTC_IS_MSGTIMEOUT_D008_Buff;
DTC DTC_PKE_MSGTIMEOUT_D030_Buff;
DTC DTC_RPAS_MSGTIMEOUT_D012_Buff;

//BUS OFF
DTC DTC_IC_BUSOFF_D205_Buff;

//MESSAGE CONTENT FAILURE
DTC DTC_EMS1_10_MSGCONTFAIL_E401_Buff;
DTC DTC_EMS6_500_MSGCONTFAIL_E406_Buff;
DTC DTC_EMS3_10_MSGCONTFAIL_E403_Buff;
DTC DTC_EMS4_20_MSGCONTFAIL_E404_Buff;
DTC DTC_EMS8_10_MSGCONTFAIL_E407_Buff;
DTC DTC_EMS12_200_MSGCONTFAIL_E409_Buff;
DTC DTC_EMS29_100_MSGCONTFAIL_E43A_Buff;
DTC DTC_EMS36_10_MSGCONTFAIL_E4A2_Buff;
DTC DTC_ESC2_10_MSGCONTFAIL_E40D_Buff;
DTC DTC_ESC5_10_MSGCONTFAIL_E420_Buff;
DTC DTC_ESC12_10_MSGCONTFAIL_E4AE_Buff;
DTC DTC_MBFM1_100_MSGCONTFAIL_E40F_Buff;
DTC DTC_MBFM5_100_MSGCONTFAIL_E410_Buff;
DTC DTC_SRS1_20_MSGCONTFAIL_E415_Buff;
DTC DTC_IS1_100_MSGCONTFAIL_E416_Buff;
DTC DTC_IS3_500_MSGCONTFAIL_E485_Buff;
DTC DTC_IS4_500_MSGCONTFAIL_E4AD_Buff;
DTC DTC_TCU5_10_MSGCONTFAIL_E4AF_Buff;
DTC DTC_TCU6_20_MSGCONTFAIL_E4B0_Buff;
DTC DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_Buff;
DTC DTC_SAS1_10_MSGCONTFAIL_E426_Buff;
DTC DTC_ESC7_20_MSGCONTFAIL_E484_Buff;
DTC DTC_MBFM6_100_MSGCONTFAIL_E411_Buff;
DTC DTC_MBFM7_100_MSGCONTFAIL_E412_Buff; 
DTC DTC_MBFM9_500_MSGCONTFAIL_E4BA_Buff; 
DTC DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_Buff;
DTC DTC_RPAS1_20_MSGCONTFAIL_E437_Buff;
DTC DTC_RPAS2_100_MSGCONTFAIL_E4384_Buff;

//IVN
DTC DTC_IVN_MISMATCH_D305_Buff;
DTC DTC_IVN_VARIENT_ERR_D505_Buff;
DTC DTC_IVN_FEATURE_ERR_E805_Buff; 
DTC DTC_IVN_NETSECFAULT_E5FF_Buff; 
DTC DTC_IVN_NETSECCONFFAULT_E5FE_Buff;

//SIGNAL CONTENT FAILURE
DTC DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_Buff;
DTC DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_Buff;
DTC DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_Buff;
DTC DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_Buff;
DTC DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_Buff;
DTC DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_Buff;
DTC DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_Buff;
DTC DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_Buff;
DTC DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_Buff;
DTC DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_Buff;
DTC DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_Buff;
DTC DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_Buff;
DTC DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_Buff;
DTC DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_Buff;
DTC DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_Buff;
DTC DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_Buff; 
DTC DTC_YAW_RATE_SIGNAL_CONTFAIL_E059_Buff;
DTC DTC_LONG_ACCEL_SIGNAL_CONTFAIL_E054_Buff;
DTC DTC_LATTERAL_ACCEL_SIGNAL_CONTFAIL_E053_Buff; 
DTC DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_Buff; 
DTC DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_Buff;
DTC DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_Buff; 
DTC DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_Buff; 
DTC DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_Buff; 
DTC DTC_FL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D6_Buff;
DTC DTC_FR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D7_Buff;
DTC DTC_RL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D8_Buff; 
DTC DTC_RR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D9_Buff; 
DTC DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_Buff; 

//BUS INTRGRITY FAILURE
DTC DTC_MBFM_BUS_INTRGRITY_FAIL_D801_Buff;
DTC DTC_PKE_BUS_INTRGRITY_FAIL_D800_Buff;

typedef struct
{
    DTC_ID      dtc_id_name;
    uint16_t const DTC_OFFSET;
    uint16_t const DTC_AGING_COUNTER_OFFSET;
    DTC  *dtc_Buffer;
    SplitWord *dtc_aging_counter;
    SplitWord * aging_flag;
}DTC_PARAM;

Private const DTC_PARAM dtc_param[]=
    {
	{  DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002,         DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_OFFSET,       		DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_AGING_COUNTER_OFFSET,      		&DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_Buff,          &counter_0,		&aging_flag_0},
	{  DTC_MIL_TT_STB_A003,          				DTC_MIL_TT_STB_A003_OFFSET,       						DTC_MIL_TT_STB_A003_AGING_COUNTER_OFFSET,      						&DTC_MIL_TT_STB_A003_Buff,          				&counter_1,		&aging_flag_1},
	{  DTC_ENG_ST_SP_TT_STB_A009,          			DTC_ENG_ST_SP_TT_STB_A009_OFFSET,       				DTC_ENG_ST_SP_TT_STB_A009_AGING_COUNTER_OFFSET,      				&DTC_ENG_ST_SP_TT_STB_A009_Buff,          			&counter_2,		&aging_flag_2},
	{  DTC_ABS_TT_STB_A00A,          				DTC_ABS_TT_STB_A00A_OFFSET,       						DTC_ABS_TT_STB_A00A_AGING_COUNTER_OFFSET,      						&DTC_ABS_TT_STB_A00A_Buff,          				&counter_3,		&aging_flag_3},
	{  DTC_CRUISE_TT_STB_A00B,           			DTC_CRUISE_TT_STB_A00B_OFFSET,        					DTC_CRUISE_TT_STB_A00B_AGING_COUNTER_OFFSET,      					&DTC_CRUISE_TT_STB_A00B_Buff,          				&counter_4,		&aging_flag_4},
	{  DTC_AIRBAG_TT_STB_A00C,            			DTC_AIRBAG_TT_STB_A00C_OFFSET,         					DTC_AIRBAG_TT_STB_A00C_AGING_COUNTER_OFFSET,      					&DTC_AIRBAG_TT_STB_A00C_Buff,          				&counter_5,		&aging_flag_5},
	{  DTC_PARKBRAKE_TT_STB_A00D,           		DTC_PARKBRAKE_TT_STB_A00D_OFFSET,       		 		DTC_PARKBRAKE_TT_STB_A00D_AGING_COUNTER_OFFSET,      				&DTC_PARKBRAKE_TT_STB_A00D_Buff,          			&counter_6,		&aging_flag_6},
	{  DTC_EPS_TT_STB_A00E,            				DTC_EPS_TT_STB_A00E_OFFSET,         					DTC_EPS_TT_STB_A00E_AGING_COUNTER_OFFSET,      						&DTC_EPS_TT_STB_A00E_Buff,          				&counter_7,		&aging_flag_7},
	{  DTC_ATFAIL_TT_STB_A010,           			DTC_ATFAIL_TT_STB_A010_OFFSET,        					DTC_ATFAIL_TT_STB_A010_AGING_COUNTER_OFFSET,     					&DTC_ATFAIL_TT_STB_A010_Buff,          				&counter_8,		&aging_flag_8},
	{  DTC_ESP_TT_STB_A01A,           				DTC_ESP_TT_STB_A01A_OFFSET,        						DTC_ESP_TT_STB_A01A_AGING_COUNTER_OFFSET,     						&DTC_ESP_TT_STB_A01A_Buff,          				&counter_9,		&aging_flag_9},
#if 0
	{  DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002,         DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_OFFSET,        		DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_AGING_COUNTER_OFFSET,     		&DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_Buff,          &counter_10,	&aging_flag_10},
	{  DTC_MIL_TT_STG_A003,           				DTC_MIL_TT_STG_A003_OFFSET,        						DTC_MIL_TT_STG_A003_AGING_COUNTER_OFFSET,     						&DTC_MIL_TT_STG_A003_Buff,          				&counter_11,	&aging_flag_11},
	{  DTC_ENG_ST_SP_TT_STG_A009,           		DTC_ENG_ST_SP_TT_STG_A009_OFFSET,        				DTC_ENG_ST_SP_TT_STG_A009_AGING_COUNTER_OFFSET,     				&DTC_ENG_ST_SP_TT_STG_A009_Buff,          			&counter_12,	&aging_flag_12},
	{  DTC_ABS_TT_STG_A00A,           				DTC_ABS_TT_STG_A00A_OFFSET,        						DTC_ABS_TT_STG_A00A_AGING_COUNTER_OFFSET,     						&DTC_ABS_TT_STG_A00A_Buff,          				&counter_13,    &aging_flag_13},
	{  DTC_CRUISE_TT_STG_A00B,              		DTC_CRUISE_TT_STG_A00B_OFFSET,           				DTC_CRUISE_TT_STG_A00B_AGING_COUNTER_OFFSET,     					&DTC_CRUISE_TT_STG_A00B_Buff,          				&counter_14,    &aging_flag_14},
	{  DTC_AIRBAG_TT_STG_A00C,              		DTC_AIRBAG_TT_STG_A00C_OFFSET,           				DTC_AIRBAG_TT_STG_A00C_AGING_COUNTER_OFFSET,     					&DTC_AIRBAG_TT_STG_A00C_Buff,          				&counter_15,    &aging_flag_15},
	{  DTC_PARKBRAKE_TT_STG_A00D,               	DTC_PARKBRAKE_TT_STG_A00D_OFFSET,             			DTC_PARKBRAKE_TT_STG_A00D_AGING_COUNTER_OFFSET,     				&DTC_PARKBRAKE_TT_STG_A00D_Buff,          			&counter_16,    &aging_flag_16},
	{  DTC_EPS_TT_STG_A00E,                			DTC_EPS_TT_STG_A00E_OFFSET,              				DTC_EPS_TT_STG_A00E_AGING_COUNTER_OFFSET,     						&DTC_EPS_TT_STG_A00E_Buff,          				&counter_17,    &aging_flag_17},
	{  DTC_ATFAIL_TT_STG_A010,                		DTC_ATFAIL_TT_STG_A010_OFFSET,              			DTC_ATFAIL_TT_STG_A010_AGING_COUNTER_OFFSET,     					&DTC_ATFAIL_TT_STG_A010_Buff,          				&counter_18,    &aging_flag_18},
	{  DTC_ESP_TT_STG_A01A,               			DTC_ESP_TT_STG_A01A_OFFSET,             				DTC_ESP_TT_STG_A01A_AGING_COUNTER_OFFSET,     						&DTC_ESP_TT_STG_A01A_Buff,          				&counter_19,    &aging_flag_19},
#endif
	{  DTC_HVSD_A000,               				DTC_HVSD_A000_OFFSET,             						DTC_HVSD_A000_AGING_COUNTER_OFFSET,     							&DTC_HVSD_A000_Buff,          						&counter_20,    &aging_flag_20},
	{  DTC_LVSD_A000,               				DTC_LVSD_A000_OFFSET,             						DTC_LVSD_A000_AGING_COUNTER_OFFSET,     							&DTC_LVSD_A000_Buff,          						&counter_21,    &aging_flag_21},
	//		{  DTC_BUTTON_STUCK_A000,               		DTC_BUTTON_STUCK_A000_OFFSET,             				DTC_BUTTON_STUCK_A000_AGING_COUNTER_OFFSET,     					&DTC_BUTTON_STUCK_A000_Buff,          				&counter_22,    &aging_flag_22},
	{  DTC_MODE_SWITCH_STUCK_A01C,               	DTC_MODE_SWITCH_STUCK_A01C_OFFSET,             			DTC_MODE_SWITCH_STUCK_A01C_AGING_COUNTER_OFFSET,     				&DTC_MODE_SWITCH_STUCK_A01C_Buff,          			&counter_22,    &aging_flag_22},
	{  DTC_OK_SWITCH_STUCK_A01D,     				DTC_OK_SWITCH_STUCK_A01D_OFFSET,   						DTC_OK_SWITCH_STUCK_A01D_AGING_COUNTER_OFFSET,     					&DTC_OK_SWITCH_STUCK_A01D_Buff,          			&counter_23,    &aging_flag_23},
	{  DTC_UP_SWITCH_STUCK_A01E,       				DTC_UP_SWITCH_STUCK_A01E_OFFSET,    					DTC_UP_SWITCH_STUCK_A01E_AGING_COUNTER_OFFSET,     					&DTC_UP_SWITCH_STUCK_A01E_Buff,          			&counter_24,    &aging_flag_24},
	{  DTC_DOWN_SWITCH_STUCK_A01F,       			DTC_DOWN_SWITCH_STUCK_A01F_OFFSET,    					DTC_DOWN_SWITCH_STUCK_A01F_AGING_COUNTER_OFFSET,     				&DTC_DOWN_SWITCH_STUCK_A01F_Buff,          			&counter_25,    &aging_flag_25},
	{  DTC_SET_SWITCH_STUCK_A022,					DTC_SET_SWITCH_STUCK_A022_OFFSET, 						DTC_SET_SWITCH_STUCK_A022_AGING_COUNTER_OFFSET,						&DTC_SET_SWITCH_STUCK_A022_Buff, 					&counter_26,    &aging_flag_26},
	{  DTC_ROM_FAIL_A001,       					DTC_ROM_FAIL_A001_OFFSET,    							DTC_ROM_FAIL_A001_AGING_COUNTER_OFFSET,     						&DTC_ROM_FAIL_A001_Buff,          					&counter_27,    &aging_flag_27},
	{  DTC_RAM_FAIL_A001,            				DTC_RAM_FAIL_A001_OFFSET,          						DTC_RAM_FAIL_A001_AGING_COUNTER_OFFSET,     						&DTC_RAM_FAIL_A001_Buff,          					&counter_28,    &aging_flag_28},
	{  DTC_EEPROM_FAIL_A001,                		DTC_EEPROM_FAIL_A001_OFFSET,              				DTC_EEPROM_FAIL_A001_AGING_COUNTER_OFFSET,     						&DTC_EEPROM_FAIL_A001_Buff,          				&counter_29,    &aging_flag_29},
	{  DTC_FUEL_OC_FAIL_A004,               		DTC_FUEL_OC_FAIL_A004_OFFSET,             				DTC_FUEL_OC_FAIL_A004_AGING_COUNTER_OFFSET,     					&DTC_FUEL_OC_FAIL_A004_Buff,          				&counter_30,    &aging_flag_30},
	{  DTC_FUEL_SC_FAIL_A004,               		DTC_FUEL_SC_FAIL_A004_OFFSET,             				DTC_FUEL_SC_FAIL_A004_AGING_COUNTER_OFFSET,     					&DTC_FUEL_SC_FAIL_A004_Buff,          				&counter_31,    &aging_flag_31},
	{  DTC_WDT_RST_FAIL_A012,               		DTC_WDT_RST_FAIL_A012_OFFSET,             				DTC_WDT_RST_FAIL_A012_AGING_COUNTER_OFFSET,     					&DTC_WDT_RST_FAIL_A012_Buff,          				&counter_32,    &aging_flag_32},
	{  DTC_CONFIG_CURR_FAIL_A014,               	DTC_CONFIG_CURR_FAIL_A014_OFFSET,             			DTC_CONFIG_CURR_FAIL_A014_AGING_COUNTER_OFFSET,     				&DTC_CONFIG_CURR_FAIL_A014_Buff,          			&counter_33,    &aging_flag_33},
	{  DTC_HELTH_MONTR_FAIL_A015,               	DTC_HELTH_MONTR_FAIL_A015_OFFSET,             			DTC_HELTH_MONTR_FAIL_A015_AGING_COUNTER_OFFSET,     				&DTC_HELTH_MONTR_FAIL_A015_Buff,          			&counter_34,    &aging_flag_34},
	{  DTC_CLOCK_OCC_FAIL_A016,     				DTC_CLOCK_OCC_FAIL_A016_OFFSET,   						DTC_CLOCK_OCC_FAIL_A016_AGING_COUNTER_OFFSET,     					&DTC_CLOCK_OCC_FAIL_A016_Buff,          			&counter_35,    &aging_flag_35},
	{  DTC_EMS_NODEABSENT_D101,       				DTC_EMS_NODEABSENT_D101_OFFSET,    						DTC_EMS_NODEABSENT_D101_AGING_COUNTER_OFFSET,     					&DTC_EMS_NODEABSENT_D101_Buff,          			&counter_36,    &aging_flag_36},
	{  DTC_TCU_NODEABSENT_D102,       				DTC_TCU_NODEABSENT_D102_OFFSET,    						DTC_TCU_NODEABSENT_D102_AGING_COUNTER_OFFSET,     					&DTC_TCU_NODEABSENT_D102_Buff,          			&counter_37,    &aging_flag_37},
	{  DTC_ABS_ESC_NODEABSENT_D103,       			DTC_ABS_ESC_NODEABSENT_D103_OFFSET,    					DTC_ABS_ESC_NODEABSENT_D103_AGING_COUNTER_OFFSET,     				&DTC_ABS_ESC_NODEABSENT_D103_Buff,          		&counter_38,    &aging_flag_38},
	{  DTC_MBFM_NODEABSENT_D104,            		DTC_MBFM_NODEABSENT_D104_OFFSET,          				DTC_MBFM_NODEABSENT_D104_AGING_COUNTER_OFFSET,     					&DTC_MBFM_NODEABSENT_D104_Buff,          			&counter_39,    &aging_flag_39},
	{  DTC_SRS_NODEABSENT_D106,               		DTC_SRS_NODEABSENT_D106_OFFSET,             			DTC_SRS_NODEABSENT_D106_AGING_COUNTER_OFFSET,     					&DTC_SRS_NODEABSENT_D106_Buff,          			&counter_40,    &aging_flag_40},
	{  DTC_IS_NODEABSENT_D108,     					DTC_IS_NODEABSENT_D108_OFFSET,   						DTC_IS_NODEABSENT_D108_AGING_COUNTER_OFFSET,     					&DTC_IS_NODEABSENT_D108_Buff,          				&counter_41,    &aging_flag_41},
	{  DTC_SAS_NODEABSENT_D110,       				DTC_SAS_NODEABSENT_D110_OFFSET,    						DTC_SAS_NODEABSENT_D110_AGING_COUNTER_OFFSET,     					&DTC_SAS_NODEABSENT_D110_Buff,          			&counter_42,    &aging_flag_42},
	{  DTC_ICU_NODEABSENT_D117,       				DTC_ICU_NODEABSENT_D117_OFFSET,    						DTC_ICU_NODEABSENT_D117_AGING_COUNTER_OFFSET,     					&DTC_ICU_NODEABSENT_D117_Buff,          			&counter_43,    &aging_flag_43},
	{  DTC_PKE_NODEABSENT_D130,       				DTC_PKE_NODEABSENT_D130_OFFSET,    						DTC_PKE_NODEABSENT_D130_AGING_COUNTER_OFFSET,     					&DTC_PKE_NODEABSENT_D130_Buff,          			&counter_44,    &aging_flag_44},
	{  DTC_RPAS_NODEABSENT_D112,					DTC_RPAS_NODEABSENT_D112_OFFSET,						DTC_RPAS_NODEABSENT_D112_AGING_COUNTER_OFFSET,						&DTC_RPAS_NODEABSENT_D112_Buff,						&counter_93,	&aging_flag_93},
	{  DTC_EMS_MSGTIMEOUT_D001,            			DTC_EMS_MSGTIMEOUT_D001_OFFSET,          				DTC_EMS_MSGTIMEOUT_D001_AGING_COUNTER_OFFSET,     					&DTC_EMS_MSGTIMEOUT_D001_Buff,          			&counter_45,    &aging_flag_45},
	{  DTC_TCU_MSGTIMEOUT_D002,                		DTC_TCU_MSGTIMEOUT_D002_OFFSET,              			DTC_TCU_MSGTIMEOUT_D002_AGING_COUNTER_OFFSET,     					&DTC_TCU_MSGTIMEOUT_D002_Buff,          			&counter_46,    &aging_flag_46},
	{  DTC_ABS_ESC_MSGTIMEOUT_D003,               	DTC_ABS_ESC_MSGTIMEOUT_D003_OFFSET,             		DTC_ABS_ESC_MSGTIMEOUT_D003_AGING_COUNTER_OFFSET,     				&DTC_ABS_ESC_MSGTIMEOUT_D003_Buff,          		&counter_47,    &aging_flag_47},
	{  DTC_MBFM_MSGTIMEOUT_D004,               		DTC_MBFM_MSGTIMEOUT_D004_OFFSET,             			DTC_MBFM_MSGTIMEOUT_D004_AGING_COUNTER_OFFSET,     					&DTC_MBFM_MSGTIMEOUT_D004_Buff,          			&counter_48,    &aging_flag_48},
	{  DTC_IS_MSGTIMEOUT_D008,			DTC_IS_MSGTIMEOUT_D008_OFFSET,							DTC_IS_MSGTIMEOUT_D008_AGING_COUNTER_OFFSET,						&DTC_IS_MSGTIMEOUT_D008_Buff,						&counter_12,	&aging_flag_12},
	{  DTC_PKE_MSGTIMEOUT_D030,               		DTC_PKE_MSGTIMEOUT_D030_OFFSET,             			DTC_PKE_MSGTIMEOUT_D030_AGING_COUNTER_OFFSET,     					&DTC_PKE_MSGTIMEOUT_D030_Buff,          			&counter_49,    &aging_flag_49},
	{  DTC_RPAS_MSGTIMEOUT_D012, 					DTC_RPAS_MSGTIMEOUT_D012_OFFSET,						DTC_RPAS_MSGTIMEOUT_D012_AGING_COUNTER_OFFSET,						&DTC_RPAS_MSGTIMEOUT_D012_Buff, 					&counter_115,	&aging_flag_115},
	{  DTC_IC_BUSOFF_D205,               			DTC_IC_BUSOFF_D205_OFFSET,             					DTC_IC_BUSOFF_D205_AGING_COUNTER_OFFSET,     						&DTC_IC_BUSOFF_D205_Buff,          					&counter_50,    &aging_flag_50},
	{  DTC_EMS1_10_MSGCONTFAIL_E401,               	DTC_EMS1_10_MSGCONTFAIL_E401_OFFSET,             		DTC_EMS1_10_MSGCONTFAIL_E401_AGING_COUNTER_OFFSET,     				&DTC_EMS1_10_MSGCONTFAIL_E401_Buff,          		&counter_51,    &aging_flag_51},
	{  DTC_EMS6_500_MSGCONTFAIL_E406,     			DTC_EMS6_500_MSGCONTFAIL_E406_OFFSET,   				DTC_EMS6_500_MSGCONTFAIL_E406_AGING_COUNTER_OFFSET,     			&DTC_EMS6_500_MSGCONTFAIL_E406_Buff,          		&counter_52,    &aging_flag_52},
	{  DTC_EMS3_10_MSGCONTFAIL_E403,       			DTC_EMS3_10_MSGCONTFAIL_E403_OFFSET,    				DTC_EMS3_10_MSGCONTFAIL_E403_AGING_COUNTER_OFFSET,     				&DTC_EMS3_10_MSGCONTFAIL_E403_Buff,          		&counter_53,    &aging_flag_53},
	{  DTC_EMS4_20_MSGCONTFAIL_E404,       			DTC_EMS4_20_MSGCONTFAIL_E404_OFFSET,    				DTC_EMS4_20_MSGCONTFAIL_E404_AGING_COUNTER_OFFSET,     				&DTC_EMS4_20_MSGCONTFAIL_E404_Buff,          		&counter_54,    &aging_flag_54},
	{  DTC_EMS8_10_MSGCONTFAIL_E407,       			DTC_EMS8_10_MSGCONTFAIL_E407_OFFSET,    				DTC_EMS8_10_MSGCONTFAIL_E407_AGING_COUNTER_OFFSET,     				&DTC_EMS8_10_MSGCONTFAIL_E407_Buff,          		&counter_55,    &aging_flag_55},
	{  DTC_EMS12_200_MSGCONTFAIL_E409,              DTC_EMS12_200_MSGCONTFAIL_E409_OFFSET,             		DTC_EMS12_200_MSGCONTFAIL_E409_AGING_COUNTER_OFFSET,     			&DTC_EMS12_200_MSGCONTFAIL_E409_Buff,          		&counter_56,    &aging_flag_56},
	{  DTC_EMS29_100_MSGCONTFAIL_E43A,     			DTC_EMS29_100_MSGCONTFAIL_E43A_OFFSET,   				DTC_EMS29_100_MSGCONTFAIL_E43A_AGING_COUNTER_OFFSET,     			&DTC_EMS29_100_MSGCONTFAIL_E43A_Buff,          		&counter_57,    &aging_flag_57},
	{  DTC_EMS36_10_MSGCONTFAIL_E4A2,       		DTC_EMS36_10_MSGCONTFAIL_E4A2_OFFSET,    				DTC_EMS36_10_MSGCONTFAIL_E4A2_AGING_COUNTER_OFFSET,     			&DTC_EMS36_10_MSGCONTFAIL_E4A2_Buff,          		&counter_58,    &aging_flag_58},
	{  DTC_ESC2_10_MSGCONTFAIL_E40D,       			DTC_ESC2_10_MSGCONTFAIL_E40D_OFFSET,    				DTC_ESC2_10_MSGCONTFAIL_E40D_AGING_COUNTER_OFFSET,     				&DTC_ESC2_10_MSGCONTFAIL_E40D_Buff,         	 	&counter_59,    &aging_flag_59},
	{  DTC_ESC5_10_MSGCONTFAIL_E420,       			DTC_ESC5_10_MSGCONTFAIL_E420_OFFSET,    				DTC_ESC5_10_MSGCONTFAIL_E420_AGING_COUNTER_OFFSET,     				&DTC_ESC5_10_MSGCONTFAIL_E420_Buff,          		&counter_60,    &aging_flag_60},
	{  DTC_ESC12_10_MSGCONTFAIL_E4AE,            	DTC_ESC12_10_MSGCONTFAIL_E4AE_OFFSET,          			DTC_ESC12_10_MSGCONTFAIL_E4AE_AGING_COUNTER_OFFSET,     			&DTC_ESC12_10_MSGCONTFAIL_E4AE_Buff,          		&counter_61,    &aging_flag_61},
	{  DTC_MBFM1_100_MSGCONTFAIL_E40F,              DTC_MBFM1_100_MSGCONTFAIL_E40F_OFFSET,              	DTC_MBFM1_100_MSGCONTFAIL_E40F_AGING_COUNTER_OFFSET,     			&DTC_MBFM1_100_MSGCONTFAIL_E40F_Buff,          		&counter_62,    &aging_flag_62},
	{  DTC_MBFM5_100_MSGCONTFAIL_E410,              DTC_MBFM5_100_MSGCONTFAIL_E410_OFFSET,             		DTC_MBFM5_100_MSGCONTFAIL_E410_AGING_COUNTER_OFFSET,     			&DTC_MBFM5_100_MSGCONTFAIL_E410_Buff,          		&counter_63,    &aging_flag_63},
	{  DTC_SRS1_20_MSGCONTFAIL_E415,                DTC_SRS1_20_MSGCONTFAIL_E415_OFFSET,             		DTC_SRS1_20_MSGCONTFAIL_E415_AGING_COUNTER_OFFSET,     				&DTC_SRS1_20_MSGCONTFAIL_E415_Buff,          		&counter_64,    &aging_flag_64},
	{  DTC_IS1_100_MSGCONTFAIL_E416,                DTC_IS1_100_MSGCONTFAIL_E416_OFFSET,             		DTC_IS1_100_MSGCONTFAIL_E416_AGING_COUNTER_OFFSET,     				&DTC_IS1_100_MSGCONTFAIL_E416_Buff,          		&counter_65,    &aging_flag_65},
	{  DTC_IS3_500_MSGCONTFAIL_E485,                DTC_IS3_500_MSGCONTFAIL_E485_OFFSET,             		DTC_IS3_500_MSGCONTFAIL_E485_AGING_COUNTER_OFFSET,     				&DTC_IS3_500_MSGCONTFAIL_E485_Buff,          		&counter_66,    &aging_flag_66},
	{  DTC_IS4_500_MSGCONTFAIL_E4AD,                DTC_IS4_500_MSGCONTFAIL_E4AD_OFFSET,             		DTC_IS4_500_MSGCONTFAIL_E4AD_AGING_COUNTER_OFFSET,     				&DTC_IS4_500_MSGCONTFAIL_E4AD_Buff,          		&counter_67,    &aging_flag_67},
	{  DTC_TCU5_10_MSGCONTFAIL_E4AF,     			DTC_TCU5_10_MSGCONTFAIL_E4AF_OFFSET,   					DTC_TCU5_10_MSGCONTFAIL_E4AF_AGING_COUNTER_OFFSET,     				&DTC_TCU5_10_MSGCONTFAIL_E4AF_Buff,          		&counter_68,    &aging_flag_68},
	{  DTC_TCU6_20_MSGCONTFAIL_E4B0,       			DTC_TCU6_20_MSGCONTFAIL_E4B0_OFFSET,    				DTC_TCU6_20_MSGCONTFAIL_E4B0_AGING_COUNTER_OFFSET,     				&DTC_TCU6_20_MSGCONTFAIL_E4B0_Buff,          		&counter_69,    &aging_flag_69},
	{  DTC_PKE_ICU2_100_MSGCONTFAIL_E418,       	DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_OFFSET,    			DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_AGING_COUNTER_OFFSET,     		&DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_Buff,          	&counter_70,    &aging_flag_70},
	{  DTC_SAS1_10_MSGCONTFAIL_E426,       			DTC_SAS1_10_MSGCONTFAIL_E426_OFFSET,    				DTC_SAS1_10_MSGCONTFAIL_E426_AGING_COUNTER_OFFSET,     				&DTC_SAS1_10_MSGCONTFAIL_E426_Buff,          		&counter_71,    &aging_flag_71},
	{  DTC_ESC7_20_MSGCONTFAIL_E484, 				DTC_ESC7_20_MSGCONTFAIL_E484_OFFSET,                    DTC_ESC7_20_MSGCONTFAIL_E484_AGING_COUNTER_OFFSET, 					&DTC_ESC7_20_MSGCONTFAIL_E484_Buff, 				&counter_72,		&aging_flag_72},
	{  DTC_MBFM6_100_MSGCONTFAIL_E411, 				DTC_MBFM6_100_MSGCONTFAIL_E411_OFFSET,					DTC_MBFM6_100_MSGCONTFAIL_E411_AGING_COUNTER_OFFSET, 				&DTC_MBFM6_100_MSGCONTFAIL_E411_Buff, 				&counter_73,    &aging_flag_73},
	{  DTC_MBFM7_100_MSGCONTFAIL_E412, 				DTC_MBFM7_100_MSGCONTFAIL_E412_OFFSET, 					DTC_MBFM7_100_MSGCONTFAIL_E412_AGING_COUNTER_OFFSET, 				&DTC_MBFM7_100_MSGCONTFAIL_E412_Buff, 				&counter_74,    &aging_flag_74},
	{  DTC_MBFM9_500_MSGCONTFAIL_E4BA, 				DTC_MBFM9_500_MSGCONTFAIL_E4BA_OFFSET, 					DTC_MBFM9_500_MSGCONTFAIL_E4BA_AGING_COUNTER_OFFSET, 				&DTC_MBFM9_500_MSGCONTFAIL_E4BA_Buff, 				&counter_75,    &aging_flag_75},
	{  DTC_MBFM_PAS1_50_MSGCONTFAIL_E4B8, 			DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_OFFSET, 				DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_AGING_COUNTER_OFFSET, 			&DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_Buff, 			&counter_76,    &aging_flag_76},
	{  DTC_RPAS1_20_MSGCONTFAIL_E437, 				DTC_RPAS1_20_MSGCONTFAIL_E437_OFFSET, 					DTC_RPAS1_20_MSGCONTFAIL_E437_AGING_COUNTER_OFFSET, 				&DTC_RPAS1_20_MSGCONTFAIL_E437_Buff, 				&counter_77,    &aging_flag_77},
	{  DTC_RPAS2_100_MSGCONTFAIL_E438, 				DTC_RPAS2_100_MSGCONTFAIL_E4384_OFFSET, 				DTC_RPAS2_100_MSGCONTFAIL_E4384_AGING_COUNTER_OFFSET, 				&DTC_RPAS2_100_MSGCONTFAIL_E4384_Buff, 				&counter_78,    &aging_flag_78},
	{  DTC_IVN_MISMATCH_D305,       				DTC_IVN_MISMATCH_D305_OFFSET,    						DTC_IVN_MISMATCH_D305_AGING_COUNTER_OFFSET,     					&DTC_IVN_MISMATCH_D305_Buff,          				&counter_79,    &aging_flag_79},
	{  DTC_IVN_VARIENT_ERR_D505,            		DTC_IVN_VARIENT_ERR_D505_OFFSET,          				DTC_IVN_VARIENT_ERR_D505_AGING_COUNTER_OFFSET,     					&DTC_IVN_VARIENT_ERR_D505_Buff,          			&counter_80,    &aging_flag_80},
	{  DTC_IVN_FEATURE_ERR_E805,                	DTC_IVN_FEATURE_ERR_E805_OFFSET,              			DTC_IVN_FEATURE_ERR_E805_AGING_COUNTER_OFFSET,     					&DTC_IVN_FEATURE_ERR_E805_Buff,          			&counter_81,    &aging_flag_81},
	{  DTC_IVN_NETSECFAULT_E5FF, 					DTC_IVN_NETSECFAULT_E5FF_OFFSET, 						DTC_IVN_NETSECFAULT_E5FF_AGING_COUNTER_OFFSET, 						&DTC_IVN_NETSECFAULT_E5FF_Buff, 					&counter_82,    &aging_flag_82},
	{  DTC_IVN_NETSECCONFFAULT_E5FE, 				DTC_IVN_NETSECCONFFAULT_E5FE_OFFSET, 					DTC_IVN_NETSECCONFFAULT_E5FE_AGING_COUNTER_OFFSET, 					&DTC_IVN_NETSECCONFFAULT_E5FE_Buff, 				&counter_83,    &aging_flag_83},
	{  DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003,           DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_OFFSET,             	DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_AGING_COUNTER_OFFSET,     		&DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_Buff,          	&counter_84,    &aging_flag_84},
	{  DTC_EMS_RPM_SIGNAL_CONTFAIL_E004,            DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_OFFSET,             	DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_AGING_COUNTER_OFFSET,     			&DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_Buff,         	&counter_85,    &aging_flag_85},
	{  DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008,       DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_OFFSET,           DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_AGING_COUNTER_OFFSET,     	&DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_Buff,        &counter_86,    &aging_flag_86},
	{  DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009,          DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_OFFSET,             	DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_AGING_COUNTER_OFFSET,     		&DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_Buff,          	&counter_87,    &aging_flag_87},
	{  DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074,    DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_OFFSET,        DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_AGING_COUNTER_OFFSET,      &DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_Buff,     &counter_88,    &aging_flag_88},
	{  DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055,    DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_OFFSET,   		DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_AGING_COUNTER_OFFSET,      &DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_Buff,     &counter_89,    &aging_flag_89},
	{  DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE, DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_OFFSET,    	DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_AGING_COUNTER_OFFSET,   &DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_Buff,  &counter_90,    &aging_flag_90},
	{  DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013,      DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_OFFSET,    		DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_AGING_COUNTER_OFFSET,     	&DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_Buff,       &counter_91,    &aging_flag_91},
	{  DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014,       DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_OFFSET,    		DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_AGING_COUNTER_OFFSET,     	&DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_Buff,        &counter_92,    &aging_flag_92},
	//		{  DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015, DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_OFFSET,    	DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_AGING_COUNTER_OFFSET,   &DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_Buff,  &counter_93,    &aging_flag_93},
	{  DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018,        DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_OFFSET,          	DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_AGING_COUNTER_OFFSET,     		&DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_Buff,         &counter_94,    &aging_flag_94},
	{  DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072,    DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_OFFSET,        DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_AGING_COUNTER_OFFSET,     	&DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_Buff,     &counter_95,    &aging_flag_95},
	{  DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073,      DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_OFFSET,          DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_AGING_COUNTER_OFFSET,     	&DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_Buff,       &counter_96,    &aging_flag_96},
	{  DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058,  DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_OFFSET,      DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_AGING_COUNTER_OFFSET,    &DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_Buff,   &counter_97,    &aging_flag_97},
	{  DTC_ENG_TRQ_AFTER_RED1_SIGNAL_CONTFAIL_E011, DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_OFFSET, 		DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_AGING_COUNTER_OFFSET, 	&DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_Buff, 	&counter_98,    &aging_flag_98},
	{  DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B, 	DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_OFFSET, 		DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_AGING_COUNTER_OFFSET, 		&DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_Buff, 		&counter_99,    &aging_flag_99},
	{  DTC_YAW_RATE_SIGNAL_CONTFAIL_E059,      		DTC_YAW_RATE_SIGNAL_CONTFAIL_E059_OFFSET, 				DTC_YAW_RATE_SIGNAL_CONTFAIL_E059_AGING_COUNTER_OFFSET, 			&DTC_YAW_RATE_SIGNAL_CONTFAIL_E059_Buff, 			&counter_100,    &aging_flag_100},
	{  DTC_LONG_ACCEL_SIGNAL_CONTFAIL_E054, 		DTC_LONG_ACCEL_SIGNAL_CONTFAIL_E054_OFFSET, 			DTC_LONG_ACCEL_SIGNAL_CONTFAIL_E054_AGING_COUNTER_OFFSET, 			&DTC_LONG_ACCEL_SIGNAL_CONTFAIL_E054_Buff, 			&counter_101,    &aging_flag_101},
	{  DTC_LATTERAL_ACCEL_SIGNAL_CONTFAIL_E053, 	DTC_LATTERAL_ACCEL_SIGNAL_CONTFAIL_E053_OFFSET, 		DTC_LATTERAL_ACCEL_SIGNAL_CONTFAIL_E053_AGING_COUNTER_OFFSET, 		&DTC_LATTERAL_ACCEL_SIGNAL_CONTFAIL_E053_Buff, 		&counter_102,    &aging_flag_102},
	{  DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3, 	DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_OFFSET, 		DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_AGING_COUNTER_OFFSET, 		&DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_Buff, 		&counter_103,    &aging_flag_103},
	{  DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0, 			DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_OFFSET, 				DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_AGING_COUNTER_OFFSET, 			&DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_Buff, 			&counter_104,    &aging_flag_104},
	{  DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0, 			DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_OFFSET, 				DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_AGING_COUNTER_OFFSET, 			&DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_Buff, 			&counter_105,    &aging_flag_105},
	{  DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA, 		DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_OFFSET, 			DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_AGING_COUNTER_OFFSET, 			&DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_Buff, 		&counter_106,    &aging_flag_106},
	{  DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF, 	DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_OFFSET, 		DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_AGING_COUNTER_OFFSET, 		&DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_Buff, 	&counter_107,    &aging_flag_107},
	{  DTC_FL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D6, 	DTC_FL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D6_OFFSET, 		DTC_FL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D6_AGING_COUNTER_OFFSET, 	&DTC_FL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D6_Buff, 	&counter_108,    &aging_flag_108},
	{  DTC_FR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D7, 	DTC_FR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D7_OFFSET, 		DTC_FR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D7_AGING_COUNTER_OFFSET, 	&DTC_FR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D7_Buff, 	&counter_109,    &aging_flag_109},
	{  DTC_RL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D8, 	DTC_RL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D8_OFFSET, 		DTC_RL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D8_AGING_COUNTER_OFFSET, 	&DTC_RL_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D8_Buff, 	&counter_110,    &aging_flag_110},
	{  DTC_RR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D9, 	DTC_RR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D9_OFFSET, 		DTC_RR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D9_AGING_COUNTER_OFFSET, 	&DTC_RR_TYRE_PRESSURE_SIGNAL_CONTFAIL_E0D9_Buff, 	&counter_111,    &aging_flag_111},
	{  DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD, 		DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_OFFSET, 			DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_AGING_COUNTER_OFFSET, 		&DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_Buff, 		&counter_112,    &aging_flag_112},
	{  DTC_MBFM_BUS_INTRGRITY_FAIL_D801,            DTC_MBFM_BUS_INTRGRITY_FAIL_D801_OFFSET,             	DTC_MBFM_BUS_INTRGRITY_FAIL_D801_AGING_COUNTER_OFFSET,     			&DTC_MBFM_BUS_INTRGRITY_FAIL_D801_Buff,          	&counter_113,    &aging_flag_113},
	{  DTC_PKE_BUS_INTRGRITY_FAIL_D800,             DTC_PKE_BUS_INTRGRITY_FAIL_D800_OFFSET,             	DTC_PKE_BUS_INTRGRITY_FAIL_D800_AGING_COUNTER_OFFSET,     			&DTC_PKE_BUS_INTRGRITY_FAIL_D800_Buff,          	&counter_114,    &aging_flag_114}
    };

#define NOF_DTC (sizeof(dtc_param)/sizeof(dtc_param[0]))

PUBLIC void Dtc_init()
{
    DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_Buff.BYTEs[0] = 0xA0;
    DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_Buff.BYTEs[1] = 0x02;
    DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002_Buff.BYTEs[2] = 0x02;

    DTC_MIL_TT_STB_A003_Buff.BYTEs[0] = 0xA0;
    DTC_MIL_TT_STB_A003_Buff.BYTEs[1] = 0x03;
    DTC_MIL_TT_STB_A003_Buff.BYTEs[2] = 0x02;

    DTC_ENG_ST_SP_TT_STB_A009_Buff.BYTEs[0] = 0xA0;
    DTC_ENG_ST_SP_TT_STB_A009_Buff.BYTEs[1] = 0x09;
    DTC_ENG_ST_SP_TT_STB_A009_Buff.BYTEs[2] = 0x02;

    DTC_ABS_TT_STB_A00A_Buff.BYTEs[0] = 0xA0;
    DTC_ABS_TT_STB_A00A_Buff.BYTEs[1] = 0x0A;
    DTC_ABS_TT_STB_A00A_Buff.BYTEs[2] = 0x02;

    DTC_CRUISE_TT_STB_A00B_Buff.BYTEs[0] = 0xA0;
    DTC_CRUISE_TT_STB_A00B_Buff.BYTEs[1] = 0x0B;
    DTC_CRUISE_TT_STB_A00B_Buff.BYTEs[2] = 0x02;

    DTC_AIRBAG_TT_STB_A00C_Buff.BYTEs[0] = 0xA0;
    DTC_AIRBAG_TT_STB_A00C_Buff.BYTEs[1] = 0x0C;
    DTC_AIRBAG_TT_STB_A00C_Buff.BYTEs[2] = 0x02;

    DTC_PARKBRAKE_TT_STB_A00D_Buff.BYTEs[0] = 0xA0;
    DTC_PARKBRAKE_TT_STB_A00D_Buff.BYTEs[1] = 0x0D;
    DTC_PARKBRAKE_TT_STB_A00D_Buff.BYTEs[2] = 0x02;

    DTC_EPS_TT_STB_A00E_Buff.BYTEs[0] = 0xA0;
    DTC_EPS_TT_STB_A00E_Buff.BYTEs[1] = 0x0E;
    DTC_EPS_TT_STB_A00E_Buff.BYTEs[2] = 0x02;

    DTC_ATFAIL_TT_STB_A010_Buff.BYTEs[0] = 0xA0;
    DTC_ATFAIL_TT_STB_A010_Buff.BYTEs[1] = 0x10;
    DTC_ATFAIL_TT_STB_A010_Buff.BYTEs[2] = 0x02;

    DTC_ESP_TT_STB_A01A_Buff.BYTEs[0] = 0xA0;
    DTC_ESP_TT_STB_A01A_Buff.BYTEs[1] = 0x1A;
    DTC_ESP_TT_STB_A01A_Buff.BYTEs[2] = 0x02;
#if 0
    DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_Buff.BYTEs[0] = 0xA0;
    DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_Buff.BYTEs[1] = 0x00;
    DTC_SYSTEM_CHECK_ENGINE_TT_STG_A002_Buff.BYTEs[2] = 0x16;

    DTC_MIL_TT_STG_A003_Buff.BYTEs[0] = 0xA0;
    DTC_MIL_TT_STG_A003_Buff.BYTEs[1] = 0x00;
    DTC_MIL_TT_STG_A003_Buff.BYTEs[2] = 0x00;

    DTC_ENG_ST_SP_TT_STG_A009_Buff.BYTEs[0] = 0xA0;
    DTC_ENG_ST_SP_TT_STG_A009_Buff.BYTEs[1] = 0x01;
    DTC_ENG_ST_SP_TT_STG_A009_Buff.BYTEs[2] = 0x44;

    DTC_ABS_TT_STG_A00A_Buff.BYTEs[0] = 0xA0;
    DTC_ABS_TT_STG_A00A_Buff.BYTEs[1] = 0x01;
    DTC_ABS_TT_STG_A00A_Buff.BYTEs[2] = 0x46;

    DTC_CRUISE_TT_STG_A00B_Buff.BYTEs[0] = 0xA0;
    DTC_CRUISE_TT_STG_A00B_Buff.BYTEs[1] = 0x03;
    DTC_CRUISE_TT_STG_A00B_Buff.BYTEs[2] = 0x02;

    DTC_AIRBAG_TT_STG_A00C_Buff.BYTEs[0] = 0xA0;
    DTC_AIRBAG_TT_STG_A00C_Buff.BYTEs[1] = 0x04;
    DTC_AIRBAG_TT_STG_A00C_Buff.BYTEs[2] = 0x13;

    DTC_PARKBRAKE_TT_STG_A00D_Buff.BYTEs[0] = 0xA0;
    DTC_PARKBRAKE_TT_STG_A00D_Buff.BYTEs[1] = 0x06;
    DTC_PARKBRAKE_TT_STG_A00D_Buff.BYTEs[2] = 0x00;

    DTC_EPS_TT_STG_A00E_Buff.BYTEs[0] = 0xA0;
    DTC_EPS_TT_STG_A00E_Buff.BYTEs[1] = 0x07;
    DTC_EPS_TT_STG_A00E_Buff.BYTEs[2] = 0x00;

    DTC_ATFAIL_TT_STG_A010_Buff.BYTEs[0] = 0xA0;
    DTC_ATFAIL_TT_STG_A010_Buff.BYTEs[1] = 0x08;
    DTC_ATFAIL_TT_STG_A010_Buff.BYTEs[2] = 0x00;

    DTC_ESP_TT_STG_A01A_Buff.BYTEs[0] = 0xA0;
    DTC_ESP_TT_STG_A01A_Buff.BYTEs[1] = 0x1A;
    DTC_ESP_TT_STG_A01A_Buff.BYTEs[2] = 0x02;
#endif
    DTC_HVSD_A000_Buff.BYTEs[0] = 0xA0;
    DTC_HVSD_A000_Buff.BYTEs[1] = 0x00;
    DTC_HVSD_A000_Buff.BYTEs[2] = 0x17;

    DTC_LVSD_A000_Buff.BYTEs[0] = 0xA0;
    DTC_LVSD_A000_Buff.BYTEs[1] = 0x00;
    DTC_LVSD_A000_Buff.BYTEs[2] = 0x16;

    //	DTC_BUTTON_STUCK_A000_Buff.BYTEs[0] = 0xA0;
    //	DTC_BUTTON_STUCK_A000_Buff.BYTEs[1] = 0x00;
    //	DTC_BUTTON_STUCK_A000_Buff.BYTEs[2] = 0x00;

    DTC_MODE_SWITCH_STUCK_A01C_Buff.BYTEs[0] = 0xA0;
    DTC_MODE_SWITCH_STUCK_A01C_Buff.BYTEs[1] = 0x1C;
    DTC_MODE_SWITCH_STUCK_A01C_Buff.BYTEs[2] = 0x00;

    DTC_OK_SWITCH_STUCK_A01D_Buff.BYTEs[0] = 0xA0;
    DTC_OK_SWITCH_STUCK_A01D_Buff.BYTEs[1] = 0x1D;
    DTC_OK_SWITCH_STUCK_A01D_Buff.BYTEs[2] = 0x00;

    DTC_UP_SWITCH_STUCK_A01E_Buff.BYTEs[0] = 0xA0;
    DTC_UP_SWITCH_STUCK_A01E_Buff.BYTEs[1] = 0x1E;
    DTC_UP_SWITCH_STUCK_A01E_Buff.BYTEs[2] = 0x00;

    DTC_DOWN_SWITCH_STUCK_A01F_Buff.BYTEs[0] = 0xA0;
    DTC_DOWN_SWITCH_STUCK_A01F_Buff.BYTEs[1] = 0x1F;
    DTC_DOWN_SWITCH_STUCK_A01F_Buff.BYTEs[2] = 0x00;

    DTC_SET_SWITCH_STUCK_A022_Buff.BYTEs[0] = 0xA0;
    DTC_SET_SWITCH_STUCK_A022_Buff.BYTEs[1] = 0x22;
    DTC_SET_SWITCH_STUCK_A022_Buff.BYTEs[2] = 0x00;

    DTC_ROM_FAIL_A001_Buff.BYTEs[0] = 0xA0;
    DTC_ROM_FAIL_A001_Buff.BYTEs[1] = 0x01;
    DTC_ROM_FAIL_A001_Buff.BYTEs[2] = 0x45;

    DTC_RAM_FAIL_A001_Buff.BYTEs[0] = 0xA0;
    DTC_RAM_FAIL_A001_Buff.BYTEs[1] = 0x01;
    DTC_RAM_FAIL_A001_Buff.BYTEs[2] = 0x44;

    DTC_EEPROM_FAIL_A001_Buff.BYTEs[0] = 0xA0;
    DTC_EEPROM_FAIL_A001_Buff.BYTEs[1] = 0x01;
    DTC_EEPROM_FAIL_A001_Buff.BYTEs[2] = 0x46;

    DTC_FUEL_OC_FAIL_A004_Buff.BYTEs[0] = 0xA0;
    DTC_FUEL_OC_FAIL_A004_Buff.BYTEs[1] = 0x04;
    DTC_FUEL_OC_FAIL_A004_Buff.BYTEs[2] = 0x13;

    DTC_FUEL_SC_FAIL_A004_Buff.BYTEs[0] = 0xA0;
    DTC_FUEL_SC_FAIL_A004_Buff.BYTEs[1] = 0x04;
    DTC_FUEL_SC_FAIL_A004_Buff.BYTEs[2] = 0x12;

    DTC_WDT_RST_FAIL_A012_Buff.BYTEs[0] = 0xA0;
    DTC_WDT_RST_FAIL_A012_Buff.BYTEs[1] = 0x12;
    DTC_WDT_RST_FAIL_A012_Buff.BYTEs[2] = 0x47;

    DTC_CONFIG_CURR_FAIL_A014_Buff.BYTEs[0] = 0xA0;
    DTC_CONFIG_CURR_FAIL_A014_Buff.BYTEs[1] = 0x14;
    DTC_CONFIG_CURR_FAIL_A014_Buff.BYTEs[2] = 0x00;

    DTC_HELTH_MONTR_FAIL_A015_Buff.BYTEs[0] = 0xA0;
    DTC_HELTH_MONTR_FAIL_A015_Buff.BYTEs[1] = 0x15;
    DTC_HELTH_MONTR_FAIL_A015_Buff.BYTEs[2] = 0x00;

    DTC_CLOCK_OCC_FAIL_A016_Buff.BYTEs[0] = 0xA0;
    DTC_CLOCK_OCC_FAIL_A016_Buff.BYTEs[1] = 0x16;
    DTC_CLOCK_OCC_FAIL_A016_Buff.BYTEs[2] = 0x44;

    DTC_EMS_NODEABSENT_D101_Buff.BYTEs[0] = 0xD1;
    DTC_EMS_NODEABSENT_D101_Buff.BYTEs[1] = 0x01;
    DTC_EMS_NODEABSENT_D101_Buff.BYTEs[2] = 0x00;

    DTC_TCU_NODEABSENT_D102_Buff.BYTEs[0] = 0xD1;
    DTC_TCU_NODEABSENT_D102_Buff.BYTEs[1] = 0x02;
    DTC_TCU_NODEABSENT_D102_Buff.BYTEs[2] = 0x00;

    DTC_ABS_ESC_NODEABSENT_D103_Buff.BYTEs[0] = 0xD1;
    DTC_ABS_ESC_NODEABSENT_D103_Buff.BYTEs[1] = 0x03;
    DTC_ABS_ESC_NODEABSENT_D103_Buff.BYTEs[2] = 0x00;

    DTC_MBFM_NODEABSENT_D104_Buff.BYTEs[0] = 0xD1;
    DTC_MBFM_NODEABSENT_D104_Buff.BYTEs[1] = 0x04;
    DTC_MBFM_NODEABSENT_D104_Buff.BYTEs[2] = 0x00;

    DTC_SRS_NODEABSENT_D106_Buff.BYTEs[0] = 0xD1;
    DTC_SRS_NODEABSENT_D106_Buff.BYTEs[1] = 0x06;
    DTC_SRS_NODEABSENT_D106_Buff.BYTEs[2] = 0x00;

    DTC_IS_NODEABSENT_D108_Buff.BYTEs[0] = 0xD1;
    DTC_IS_NODEABSENT_D108_Buff.BYTEs[1] = 0x08;
    DTC_IS_NODEABSENT_D108_Buff.BYTEs[2] = 0x00;

    DTC_SAS_NODEABSENT_D110_Buff.BYTEs[0] = 0xD1;
    DTC_SAS_NODEABSENT_D110_Buff.BYTEs[1] = 0x10;
    DTC_SAS_NODEABSENT_D110_Buff.BYTEs[2] = 0x00;

    DTC_ICU_NODEABSENT_D117_Buff.BYTEs[0] = 0xD1;
    DTC_ICU_NODEABSENT_D117_Buff.BYTEs[1] = 0x17;
    DTC_ICU_NODEABSENT_D117_Buff.BYTEs[2] = 0x00;

    DTC_PKE_NODEABSENT_D130_Buff.BYTEs[0] = 0xD1;
    DTC_PKE_NODEABSENT_D130_Buff.BYTEs[1] = 0x30;
    DTC_PKE_NODEABSENT_D130_Buff.BYTEs[2] = 0x00;
	
	DTC_RPAS_NODEABSENT_D112_Buff.BYTEs[0] = 0xD1;
	DTC_RPAS_NODEABSENT_D112_Buff.BYTEs[1] = 0x12;
	DTC_RPAS_NODEABSENT_D112_Buff.BYTEs[2] = 0x00;

    DTC_EMS_MSGTIMEOUT_D001_Buff.BYTEs[0] = 0xD0;
    DTC_EMS_MSGTIMEOUT_D001_Buff.BYTEs[1] = 0x01;
    DTC_EMS_MSGTIMEOUT_D001_Buff.BYTEs[2] = 0x00;

    DTC_TCU_MSGTIMEOUT_D002_Buff.BYTEs[0] = 0xD0;
    DTC_TCU_MSGTIMEOUT_D002_Buff.BYTEs[1] = 0x02;
    DTC_TCU_MSGTIMEOUT_D002_Buff.BYTEs[2] = 0x00;

    DTC_ABS_ESC_MSGTIMEOUT_D003_Buff.BYTEs[0] = 0xD0;
    DTC_ABS_ESC_MSGTIMEOUT_D003_Buff.BYTEs[1] = 0x03;
    DTC_ABS_ESC_MSGTIMEOUT_D003_Buff.BYTEs[2] = 0x00;

    DTC_MBFM_MSGTIMEOUT_D004_Buff.BYTEs[0] = 0xD0;
    DTC_MBFM_MSGTIMEOUT_D004_Buff.BYTEs[1] = 0x04;
    DTC_MBFM_MSGTIMEOUT_D004_Buff.BYTEs[2] = 0x00;

    DTC_IS_MSGTIMEOUT_D008_Buff.BYTEs[0] = 0xD0;
    DTC_IS_MSGTIMEOUT_D008_Buff.BYTEs[1] = 0x08;
    DTC_IS_MSGTIMEOUT_D008_Buff.BYTEs[2] = 0x00;

    DTC_PKE_MSGTIMEOUT_D030_Buff.BYTEs[0] = 0xD0;
    DTC_PKE_MSGTIMEOUT_D030_Buff.BYTEs[1] = 0x30;
    DTC_PKE_MSGTIMEOUT_D030_Buff.BYTEs[2] = 0x00;
	
	DTC_RPAS_MSGTIMEOUT_D012_Buff.BYTEs[0] = 0xD0;
	DTC_RPAS_MSGTIMEOUT_D012_Buff.BYTEs[1] = 0x12;
	DTC_RPAS_MSGTIMEOUT_D012_Buff.BYTEs[2] = 0x00;

    DTC_IC_BUSOFF_D205_Buff.BYTEs[0] = 0xD2;
    DTC_IC_BUSOFF_D205_Buff.BYTEs[1] = 0x05;
    DTC_IC_BUSOFF_D205_Buff.BYTEs[2] = 0x00;

    DTC_EMS1_10_MSGCONTFAIL_E401_Buff.BYTEs[0] = 0xE4;
    DTC_EMS1_10_MSGCONTFAIL_E401_Buff.BYTEs[1] = 0x01;
    DTC_EMS1_10_MSGCONTFAIL_E401_Buff.BYTEs[2] = 0x00;

    DTC_EMS6_500_MSGCONTFAIL_E406_Buff.BYTEs[0] = 0xE4;
    DTC_EMS6_500_MSGCONTFAIL_E406_Buff.BYTEs[1] = 0x06;
    DTC_EMS6_500_MSGCONTFAIL_E406_Buff.BYTEs[2] = 0x00;

    DTC_EMS3_10_MSGCONTFAIL_E403_Buff.BYTEs[0] = 0xE4;
    DTC_EMS3_10_MSGCONTFAIL_E403_Buff.BYTEs[1] = 0x03;
    DTC_EMS3_10_MSGCONTFAIL_E403_Buff.BYTEs[2] = 0x00;

    DTC_EMS4_20_MSGCONTFAIL_E404_Buff.BYTEs[0] = 0xE4;
    DTC_EMS4_20_MSGCONTFAIL_E404_Buff.BYTEs[1] = 0x04;
    DTC_EMS4_20_MSGCONTFAIL_E404_Buff.BYTEs[2] = 0x00;

    DTC_EMS8_10_MSGCONTFAIL_E407_Buff.BYTEs[0] = 0xE4;
    DTC_EMS8_10_MSGCONTFAIL_E407_Buff.BYTEs[1] = 0x07;
    DTC_EMS8_10_MSGCONTFAIL_E407_Buff.BYTEs[2] = 0x00;

    DTC_EMS12_200_MSGCONTFAIL_E409_Buff.BYTEs[0] = 0xE4;
    DTC_EMS12_200_MSGCONTFAIL_E409_Buff.BYTEs[1] = 0x09;
    DTC_EMS12_200_MSGCONTFAIL_E409_Buff.BYTEs[2] = 0x00;

    DTC_EMS29_100_MSGCONTFAIL_E43A_Buff.BYTEs[0] = 0xE4;
    DTC_EMS29_100_MSGCONTFAIL_E43A_Buff.BYTEs[1] = 0x3A;
    DTC_EMS29_100_MSGCONTFAIL_E43A_Buff.BYTEs[2] = 0x00;

    DTC_EMS36_10_MSGCONTFAIL_E4A2_Buff.BYTEs[0] = 0xE4;
    DTC_EMS36_10_MSGCONTFAIL_E4A2_Buff.BYTEs[1] = 0xA2;
    DTC_EMS36_10_MSGCONTFAIL_E4A2_Buff.BYTEs[2] = 0x00;

    DTC_ESC2_10_MSGCONTFAIL_E40D_Buff.BYTEs[0] = 0xE4;
    DTC_ESC2_10_MSGCONTFAIL_E40D_Buff.BYTEs[1] = 0x0D;
    DTC_ESC2_10_MSGCONTFAIL_E40D_Buff.BYTEs[2] = 0x00;

    DTC_ESC5_10_MSGCONTFAIL_E420_Buff.BYTEs[0] = 0xE4;
    DTC_ESC5_10_MSGCONTFAIL_E420_Buff.BYTEs[1] = 0x20;
    DTC_ESC5_10_MSGCONTFAIL_E420_Buff.BYTEs[2] = 0x00;

    DTC_ESC12_10_MSGCONTFAIL_E4AE_Buff.BYTEs[0] = 0xE4;
    DTC_ESC12_10_MSGCONTFAIL_E4AE_Buff.BYTEs[1] = 0xAE;
    DTC_ESC12_10_MSGCONTFAIL_E4AE_Buff.BYTEs[2] = 0x00;

    DTC_MBFM1_100_MSGCONTFAIL_E40F_Buff.BYTEs[0] = 0xE4;
    DTC_MBFM1_100_MSGCONTFAIL_E40F_Buff.BYTEs[1] = 0x0F;
    DTC_MBFM1_100_MSGCONTFAIL_E40F_Buff.BYTEs[2] = 0x00;

    DTC_MBFM5_100_MSGCONTFAIL_E410_Buff.BYTEs[0] = 0xE4;
    DTC_MBFM5_100_MSGCONTFAIL_E410_Buff.BYTEs[1] = 0x10;
    DTC_MBFM5_100_MSGCONTFAIL_E410_Buff.BYTEs[2] = 0x00;

    DTC_SRS1_20_MSGCONTFAIL_E415_Buff.BYTEs[0] = 0xE4;
    DTC_SRS1_20_MSGCONTFAIL_E415_Buff.BYTEs[1] = 0x15;
    DTC_SRS1_20_MSGCONTFAIL_E415_Buff.BYTEs[2] = 0x00;

    DTC_IS1_100_MSGCONTFAIL_E416_Buff.BYTEs[0] = 0xE4;
    DTC_IS1_100_MSGCONTFAIL_E416_Buff.BYTEs[1] = 0x16;
    DTC_IS1_100_MSGCONTFAIL_E416_Buff.BYTEs[2] = 0x00;

    DTC_IS3_500_MSGCONTFAIL_E485_Buff.BYTEs[0] = 0xE4;
    DTC_IS3_500_MSGCONTFAIL_E485_Buff.BYTEs[1] = 0x85;
    DTC_IS3_500_MSGCONTFAIL_E485_Buff.BYTEs[2] = 0x00;

    DTC_IS4_500_MSGCONTFAIL_E4AD_Buff.BYTEs[0] = 0xE4;
    DTC_IS4_500_MSGCONTFAIL_E4AD_Buff.BYTEs[1] = 0xAD;
    DTC_IS4_500_MSGCONTFAIL_E4AD_Buff.BYTEs[2] = 0x00;

    DTC_TCU5_10_MSGCONTFAIL_E4AF_Buff.BYTEs[0] = 0xE4;
    DTC_TCU5_10_MSGCONTFAIL_E4AF_Buff.BYTEs[1] = 0xAF;
    DTC_TCU5_10_MSGCONTFAIL_E4AF_Buff.BYTEs[2] = 0x00;

    DTC_TCU6_20_MSGCONTFAIL_E4B0_Buff.BYTEs[0] = 0xE4;
    DTC_TCU6_20_MSGCONTFAIL_E4B0_Buff.BYTEs[1] = 0xB0;
    DTC_TCU6_20_MSGCONTFAIL_E4B0_Buff.BYTEs[2] = 0x00;	

    DTC_SRS1_20_MSGCONTFAIL_E415_Buff.BYTEs[0] = 0xE4;
    DTC_SRS1_20_MSGCONTFAIL_E415_Buff.BYTEs[1] = 0x15;
    DTC_SRS1_20_MSGCONTFAIL_E415_Buff.BYTEs[2] = 0x00;

    DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_Buff.BYTEs[0] = 0xE4;
    DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_Buff.BYTEs[1] = 0xB8;
    DTC_MBFM_PAS1_100_MSGCONTFAIL_E4B8_Buff.BYTEs[2] = 0x00;

    DTC_MBFM9_500_MSGCONTFAIL_E4BA_Buff.BYTEs[0] = 0xE4;
    DTC_MBFM9_500_MSGCONTFAIL_E4BA_Buff.BYTEs[1] = 0xBA;
    DTC_MBFM9_500_MSGCONTFAIL_E4BA_Buff.BYTEs[2] = 0x00;

    DTC_RPAS1_20_MSGCONTFAIL_E437_Buff.BYTEs[0] = 0xE4;
    DTC_RPAS1_20_MSGCONTFAIL_E437_Buff.BYTEs[1] = 0x37;
    DTC_RPAS1_20_MSGCONTFAIL_E437_Buff.BYTEs[2] = 0x00;

    DTC_RPAS2_100_MSGCONTFAIL_E4384_Buff.BYTEs[0] = 0xE4;
    DTC_RPAS2_100_MSGCONTFAIL_E4384_Buff.BYTEs[1] = 0x38;
    DTC_RPAS2_100_MSGCONTFAIL_E4384_Buff.BYTEs[2] = 0x00;

    DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_Buff.BYTEs[0] = 0xE4;
    DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_Buff.BYTEs[1] = 0x18;
    DTC_PKE_ICUA2_100_MSGCONTFAIL_E418_Buff.BYTEs[2] = 0x00;

    DTC_SAS1_10_MSGCONTFAIL_E426_Buff.BYTEs[0] = 0xE4;
    DTC_SAS1_10_MSGCONTFAIL_E426_Buff.BYTEs[1] = 0x26;
    DTC_SAS1_10_MSGCONTFAIL_E426_Buff.BYTEs[2] = 0x00;

    DTC_IVN_MISMATCH_D305_Buff.BYTEs[0] = 0xD3;
    DTC_IVN_MISMATCH_D305_Buff.BYTEs[1] = 0x05;
    DTC_IVN_MISMATCH_D305_Buff.BYTEs[2] = 0x00;

    DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[0] = 0xD5;
    DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[1] = 0x05;
    DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[2] = 0x00;

    DTC_IVN_FEATURE_ERR_E805_Buff.BYTEs[0] = 0xE8;
    DTC_IVN_FEATURE_ERR_E805_Buff.BYTEs[1] = 0x05;
    DTC_IVN_FEATURE_ERR_E805_Buff.BYTEs[2] = 0x00;

    DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_Buff.BYTEs[0] = 0xE0;
    DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_Buff.BYTEs[1] = 0x03;
    DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003_Buff.BYTEs[2] = 0x00;

    DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_Buff.BYTEs[0] = 0xE0;
    DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_Buff.BYTEs[1] = 0x04;
    DTC_EMS_RPM_SIGNAL_CONTFAIL_E004_Buff.BYTEs[2] = 0x00;

    DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_Buff.BYTEs[0] = 0xE0;
    DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_Buff.BYTEs[1] = 0x08;
    DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008_Buff.BYTEs[2] = 0x00;

    DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_Buff.BYTEs[0] = 0xE0;
    DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_Buff.BYTEs[1] = 0x09;
    DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009_Buff.BYTEs[2] = 0x00;

    DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_Buff.BYTEs[0] = 0xE0;
    DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_Buff.BYTEs[1] = 0x74;
    DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074_Buff.BYTEs[2] = 0x00;

    DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_Buff.BYTEs[0] = 0xE0;
    DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_Buff.BYTEs[1] = 0x55;
    DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055_Buff.BYTEs[2] = 0x00;

    DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_Buff.BYTEs[0] = 0xE0;
    DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_Buff.BYTEs[1] = 0xBE;
    DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE_Buff.BYTEs[2] = 0x00;

    DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_Buff.BYTEs[0] = 0xE0;
    DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_Buff.BYTEs[1] = 0x13;
    DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013_Buff.BYTEs[2] = 0x00;

    DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_Buff.BYTEs[0] = 0xE0;
    DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_Buff.BYTEs[1] = 0x14;
    DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014_Buff.BYTEs[2] = 0x00;

    DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_Buff.BYTEs[0] = 0xE0;
    DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_Buff.BYTEs[1] = 0x15;
    DTC_AIRBAG_EVEN_PARITY_SIGNAL_CONTFAIL_E015_Buff.BYTEs[2] = 0x00;

    DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_Buff.BYTEs[0] = 0xE0;
    DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_Buff.BYTEs[1] = 0x18;
    DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018_Buff.BYTEs[2] = 0x00;

    DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_Buff.BYTEs[0] = 0xE0;
    DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_Buff.BYTEs[1] = 0x72;
    DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072_Buff.BYTEs[2] = 0x00;

    DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_Buff.BYTEs[0] = 0xE0;
    DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_Buff.BYTEs[1] = 0x73;
    DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073_Buff.BYTEs[2] = 0x00;

    DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_Buff.BYTEs[0] = 0xE0;
    DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_Buff.BYTEs[1] = 0x58;
    DTC_SAS_ABOLUTE_ANGLE_SIGNAL_CONTFAIL_E058_Buff.BYTEs[2] = 0x00;

    DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_Buff.BYTEs[0] = 0xE0;
    DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_Buff.BYTEs[1] = 0x11;
    DTC_ENG_TRQ_AFT_RED1_SIGNAL_CONTFAIL_E011_Buff.BYTEs[2] = 0x00;

    DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_Buff.BYTEs[0] = 0xE0;
    DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_Buff.BYTEs[1] = 0x9B;
    DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B_Buff.BYTEs[2] = 0x00;

    DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_Buff.BYTEs[0] = 0xE0;
    DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_Buff.BYTEs[1] = 0xAD;
    DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD_Buff.BYTEs[2] = 0x00;

    DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_Buff.BYTEs[0] = 0xE0;
    DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_Buff.BYTEs[1] = 0xCA;
    DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA_Buff.BYTEs[2] = 0x00;

    DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_Buff.BYTEs[0] = 0xE0;
    DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_Buff.BYTEs[1] = 0xE0;
    DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0_Buff.BYTEs[2] = 0x00;

    DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_Buff.BYTEs[0] = 0xE0;
    DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_Buff.BYTEs[1] = 0xDF;
    DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF_Buff.BYTEs[2] = 0x00;

    DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_Buff.BYTEs[0] = 0xE0;
    DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_Buff.BYTEs[1] = 0xD0;
    DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0_Buff.BYTEs[2] = 0x00;

    DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_Buff.BYTEs[0] = 0xE0;
    DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_Buff.BYTEs[1] = 0xE3;
    DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3_Buff.BYTEs[2] = 0x00;

    DTC_MBFM_BUS_INTRGRITY_FAIL_D801_Buff.BYTEs[0] = 0xD8;
    DTC_MBFM_BUS_INTRGRITY_FAIL_D801_Buff.BYTEs[1] = 0x01;
    DTC_MBFM_BUS_INTRGRITY_FAIL_D801_Buff.BYTEs[2] = 0x00;

    DTC_PKE_BUS_INTRGRITY_FAIL_D800_Buff.BYTEs[0] = 0xD8;
    DTC_PKE_BUS_INTRGRITY_FAIL_D800_Buff.BYTEs[1] = 0x00;
    DTC_PKE_BUS_INTRGRITY_FAIL_D800_Buff.BYTEs[2] = 0x00;
}


/* **********************************************************************************************************************
 * Function Name: Read_DTC_Status
 * Description  : to read DTC status from EEPROM.
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void Read_DTC_Status(void)
{
	uint8_t i=0;
	for (i = 0; i < NOF_DTC; i++)
	{

		dtc_param[i].dtc_Buffer->BYTEs[3] =  eeprom_DTC_read(dtc_param[i].DTC_OFFSET);
		if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
		{
			dtc_param[i].dtc_aging_counter->w = eeprom_DTC_read(dtc_param[i].DTC_AGING_COUNTER_OFFSET);
			dtc_param[i].dtc_aging_counter->w++;
			dtc_param[i].aging_flag->w = SET;
		}

		//Read Freeze frame occurance count
	}
}
/* **********************************************************************************************************************
 * Function Name: All_DTC_Write
 * Description  : to write DTC status in EEPROM at IGN off.
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void All_DTC_Write(void)
{
	uint8_t i=0;
	for (i = 0; i < NOF_DTC; i++)
	{
		if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)||(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL))
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET,0x08);
			delay();
			delay();
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			delay();
		}
	}
	Clear_IVN_Variables();
}


PUBLIC void read_dtcs(void)
{

	uint32_t dtc_Mask_Record;
	uint16_t dtc_recorded_count = CLEAR,Array_Count = CLEAR, Freeze_Frame_Offset = CLEAR;
	uint8_t i,dtc_buff[4],freeze_frame_buff[12];
	DTC RequestedDTC;

	if(Functional_Addressing_Flag == CLEAR)
	{
		if(prog_session_active == CLEAR)
		{				
			switch(UDS_Buffer[2])
			{
			case 0x01:
					if(UDS_Buffer[0] == READ_DTC_LENGTH)
					{
		//				if(UDS_Buffer[3] == 1)      //commented t avoid NRC 31 in negative response 
						{
							TX_DATA[0] = 0x06;
							TX_DATA[1] = 0x59;
							TX_DATA[2] = UDS_Buffer[2];
							TX_DATA[3] = UDS_Buffer[3];
							TX_DATA[4] = 0x01;

							for (i = 0; i < NOF_DTC; i++)
							{
								dtc_buff[0]=(dtc_param[i].dtc_Buffer->BYTEs[0]);/*msb dtc*/
								dtc_buff[1]=(dtc_param[i].dtc_Buffer->BYTEs[1]);/*lsb dtc*/
								dtc_buff[2]=(dtc_param[i].dtc_Buffer->BYTEs[2]);/*ftb*/
								dtc_buff[3]=(dtc_param[i].dtc_Buffer->BYTEs[3]);/*fault*/


								if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF) && (UDS_Buffer[3] == DTC_CNF))
								{
									dtc_recorded_count++;
								}
								else if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL) && (UDS_Buffer[3] == DTC_TEST_FAIL))
								{
								    dtc_recorded_count++;
								}
								else if((UDS_Buffer[3] & DTC_CNF_TEST_FAIL) == DTC_CNF_TEST_FAIL)
								{
								    if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF) || (dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL))
								    {
									dtc_recorded_count++;
								    }
								}
							}

							TX_DATA[5] = (uint8_t)(dtc_recorded_count >> 8);/*dtc_recorded_count is the count of DTC match with the status Mask */
							TX_DATA[6] = (uint8_t)dtc_recorded_count;
							Respond_Single_Frame(READ_DTC);
						}
//commented t avoid NRC 31 in negative response
						//else
						//{
						//	Save_NegResponse(READ_DTC, NRC_ROFR_0x31);
					//		Respond_Single_Frame(READ_DTC);								  
					//	}
					}
					else
					{
						Save_NegResponse(READ_DTC,NRC_ICDL_0x13);
						Respond_Single_Frame(READ_DTC);
					}
			break;
		
			case 0x02:          /*DTCByStatusMask*/
					if(UDS_Buffer[0] == READ_DTC_LENGTH)
					{
	//					if(UDS_Buffer[3] == 1)       //commented t avoid NRC 31 in negative response
						{

							Array_Count = 5;

							for (i = 0; i < NOF_DTC; i++)
							{
								if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF) && (UDS_Buffer[3] == DTC_CNF))
								{
								    memcpy(&UDS_TX_DATA[Array_Count], &dtc_param[i].dtc_Buffer->BYTEs[0], 4);
								    Array_Count += 4;
								}
								else if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL) && (UDS_Buffer[3] == DTC_TEST_FAIL))
								{
								    memcpy(&UDS_TX_DATA[Array_Count], &dtc_param[i].dtc_Buffer->BYTEs[0], 4);
								    Array_Count += 4;
								}
								else if((UDS_Buffer[3] & DTC_CNF_TEST_FAIL) == DTC_CNF_TEST_FAIL)
								{
								    if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF) || (dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL))
								    {
									memcpy(&UDS_TX_DATA[Array_Count], &dtc_param[i].dtc_Buffer->BYTEs[0], 4);
									Array_Count += 4;
								    }
								}
							}
							/*Array_Count(length) is increase with DTC match with statusMask*/
							if(Array_Count > 9)
							{
							    UDS_TX_DATA[0] = 0x10;
							    //UDS_TX_DATA[1] = 0x60;
							    UDS_TX_DATA[2] = 0x59;
							    UDS_TX_DATA[3] = UDS_Buffer[2];
							    UDS_TX_DATA[4] = UDS_Buffer[3];
							    UDS_TX_DATA[1] = Array_Count - 2; /*Array count increase when DTC match with the Status Mask*/
							    Respond_Multiple_Frames(Array_Count);
							}
							else if(Array_Count > 5)
							{
							    TX_DATA[0] = Array_Count - 2;
							    TX_DATA[1] = 0x59;
							    TX_DATA[2] = UDS_Buffer[2];
							    TX_DATA[3] = UDS_Buffer[3];							    
							    TX_DATA[4] = UDS_TX_DATA[5];							    
							    TX_DATA[5] = UDS_TX_DATA[6];							    
							    TX_DATA[6] = UDS_TX_DATA[7];							    
							    TX_DATA[7] = UDS_TX_DATA[8];
							    Respond_Single_Frame(READ_DTC);
							}
							else
							{
							    TX_DATA[0] = Array_Count - 2;
							    TX_DATA[1] = 0x59;
							    TX_DATA[2] = UDS_Buffer[2];
							    TX_DATA[3] = UDS_Buffer[3];							    
							    TX_DATA[4] = 0;							    
							    TX_DATA[5] = 0;							    
							    TX_DATA[6] = 0;							    
							    TX_DATA[7] = 0;
							    Respond_Single_Frame(READ_DTC);
							}

						}
//commented t avoid NRC 31 in negative response
		//				else
		//				{
			//				Save_NegResponse(READ_DTC, NRC_ROFR_0x31);
			//				Respond_Single_Frame(READ_DTC);								  
			//			}
					}
					else
					{
						Save_NegResponse(READ_DTC,NRC_ICDL_0x13);
						Respond_Single_Frame(READ_DTC);
					}					
			break;	
		
			case 0x04: /* Freeze frame data */
					if(UDS_Buffer[0] == READ_DTC_FREEZE_FRAME_LENGTH)
					{						
						RequestedDTC.BYTEs[0] = UDS_Buffer[3];
						RequestedDTC.BYTEs[1] = UDS_Buffer[4];
						RequestedDTC.BYTEs[2] = UDS_Buffer[5];
						RequestedDTC.BYTEs[3] = 0;
						for(i = 0; i < NOF_DTC; i++)
						{
							if(RequestedDTC.DTC_STATUS == (dtc_param[i].dtc_Buffer->DTC_STATUS & 0x00FFFFFF))
							{
								break;								
							}
						}
					
						if(((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF) || 
							(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)) && 
							(i < NOF_DTC) && 
							(UDS_Buffer[6] == 0x01))
						{
							UDS_TX_DATA[0] = 0x10;
							UDS_TX_DATA[1] = 0x0A;
							UDS_TX_DATA[2] = 0x59;
							UDS_TX_DATA[3] = UDS_Buffer[2];
							UDS_TX_DATA[4] = dtc_param[i].dtc_Buffer->BYTEs[0];
							UDS_TX_DATA[5] = dtc_param[i].dtc_Buffer->BYTEs[1];
							UDS_TX_DATA[6] = dtc_param[i].dtc_Buffer->BYTEs[2];
							UDS_TX_DATA[7] = dtc_param[i].dtc_Buffer->BYTEs[3];
							UDS_TX_DATA[8] = UDS_Buffer[6];
							UDS_TX_DATA[9] = 0x01;
							UDS_TX_DATA[10] = 0x05;
							UDS_TX_DATA[11] = 0xBD;

							Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
							
							eeprom_memread_32bytes(Freeze_Frame_Offset, freeze_frame_buff, FREEZE_FRAME_DATA_SIZE);
							memcpy(&UDS_TX_DATA[12], freeze_frame_buff, FREEZE_FRAME_DATA_SIZE);
							UDS_TX_DATA[1] += FREEZE_FRAME_DATA_SIZE;
							UDS_TX_DATA[13] = (dtc_param[i].dtc_aging_counter->w & 0x00ff);
							
							Array_Count = 24;
						
							Respond_Multiple_Frames(Array_Count);
						}
						else
						{
							Save_NegResponse(READ_DTC,NRC_ROFR_0x31);
							Respond_Single_Frame(READ_DTC);
						}
					}
					else
					{
						Save_NegResponse(READ_DTC,NRC_ICDL_0x13);
						Respond_Single_Frame(READ_DTC);
					}
			break;
				
		
			default:
				Save_NegResponse(READ_DTC,NRC_SFNC_0x12);
				Respond_Single_Frame(READ_DTC);
				break;
			}
		}
		else
		{
			Save_NegResponse(READ_DTC, NRC_SNS_0x7F);
			Respond_Single_Frame(READ_DTC);
		}
	}
}


/***************************************************************************************************************************
    ## Module:      UDSS.c
    ## Function:    clear_dtcs()
    ## Description: clear all DTCs .
    ## Parameter:   None
    ## Return??     void
 ************************************************************************************************************************/
void clear_dtcs(void)
{
	if(prog_session_active == CLEAR)
	{
			if(UDS_Buffer[0] == CLEAR_ALL_DTC_LENGTH)
			{
					if((UDS_Buffer[2] == 0xFF) && (UDS_Buffer[3] == 0xFF) && (UDS_Buffer[4] == 0xFF))
					{
						/* clear all DTCs */
						clear_dtc_log_flag = SET;

						Save_NegResponse(CLEAR_DTC, NRC_WAIT_0x78);
						//clear_dtc_snapshot();
					}
					else
					{
						Save_NegResponse(CLEAR_DTC, NRC_ROFR_0x31);
					}
			}
			else
			{
				Save_NegResponse(CLEAR_DTC,NRC_ICDL_0x13);
			}
   }
   else
   {
	   Save_NegResponse(CLEAR_DTC, NRC_SNS_0x7F);
   }
	Respond_Single_Frame(CLEAR_DTC);
}

void Clear_EEPROM_DTC_Memory(void)
{
    static uint8_t DTC_Count = 0;
    uint16_t Freeze_Frame_Offset = 0;
    Failure_DTC_Clear(DTC_MODE_SWITCH_STUCK_A01C);

    while(DTC_Count < NOF_DTC)
    {
    	if((DTC_Count==12)||(DTC_Count==16))
    	{
    		DTC_Count++;
    	}
	if((dtc_param[DTC_Count].dtc_Buffer->BYTEs[3] == DTC_CNF) || (dtc_param[DTC_Count].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL))
	{
	    eeprom_write(dtc_param[DTC_Count].DTC_OFFSET,DTC_CLEAR);
	    dtc_param[DTC_Count].dtc_Buffer->BYTEs[3] = DTC_CLEAR;
	    Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + DTC_Count * FREEZE_FRAME_DATA_SIZE);
	    eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	    break;
	}
	DTC_Count++;
    }

    if(DTC_Count >= NOF_DTC)
    {
	TX_DATA[0] = 0x01;
	TX_DATA[1] = 0x54;
	TX_DATA[2] = 0x00;
	TX_DATA[3] = 0x00;
	TX_DATA[4] = 0x00;
	TX_DATA[5] = 0x00;
	TX_DATA[6] = 0x00;
	TX_DATA[7] = 0x00;

	MODE_BUTTON_STUCK_FLAG = 0;
	SET_BUTTON_STUCK_FLAG = 0;
	eeprom_write(RESET_BOOT_FLAG,0x00);
	Clear_IVN_Variables();
	Respond_Single_Frame(CLEAR_DTC);
	clear_dtc_log_flag = CLEAR;
	DTC_Count = 0;
    }
}

/* **********************************************************************************************************************
 * Function Name: telltale_failure_DTC_Set
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */
void telltale_failure_DTC_Set(uint16_t i)
{
	if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
			delay();
			dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		}
	}

	if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
	{
		dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
		dtc_param[i].dtc_aging_counter->w = CLEAR;
		Freeze_Frame_log(i);
	}

	if(dtc_param[i].aging_flag->w == CLEAR)
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			dtc_param[i].aging_flag->w = SET;
		}
	}
}

/* **********************************************************************************************************************
 * Function Name: telltale_failure_DTC_Clear
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */

void telltale_failure_DTC_Clear(uint16_t i)
{
    uint16_t Freeze_Frame_Offset = 0;
    
    if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
    {
	dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
	dtc_param[i].dtc_aging_counter->w = SET;
	dtc_param[i].aging_flag->w = SET;
    }

    if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC) && (dtc_param[i].aging_flag->w == SET)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
    {
	if(EEPROM_IN_USE == 0)
	{
	    eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
	    delay();
	    dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
	    dtc_param[i].aging_flag->w = CLEAR;
	    Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	    eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	    delay();
	}
    }
}


/* **********************************************************************************************************************
 * Function Name: message_timeout_DTC_Set
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */
void message_timeout_DTC_Set(uint16_t i)
{
	if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
			delay();
			dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		}
	}

	if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
	{
		dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
		dtc_param[i].dtc_aging_counter->w = CLEAR;		
		Freeze_Frame_log(i);
	}

	if(dtc_param[i].aging_flag->w == CLEAR)
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			dtc_param[i].aging_flag->w = SET;
		}
	}
}

/* **********************************************************************************************************************
 * Function Name: message_timeout_DTC_Clear
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */

void message_timeout_DTC_Clear(uint16_t i)
{
    uint16_t Freeze_Frame_Offset = 0;
    
    if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
    {
	dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
	dtc_param[i].dtc_aging_counter->w = SET;
	dtc_param[i].aging_flag->w = SET;
    }

    if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC) && (dtc_param[i].aging_flag->w == SET)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
    {
	if(EEPROM_IN_USE == 0)
	{
	    eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
	    delay();
	    dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
	    dtc_param[i].aging_flag->w = CLEAR;
	    Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	    eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	    delay();
	}
    }
}


/* **********************************************************************************************************************
 * Function Name: Node_Absent_DTC_Set
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */
void Node_Absent_DTC_Set(uint16_t i)
{
	if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
			delay();
			dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		}
	}

	if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
	{
		dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
		dtc_param[i].dtc_aging_counter->w = CLEAR;
		Freeze_Frame_log(i);
	}

	if(dtc_param[i].aging_flag->w == CLEAR)
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			dtc_param[i].aging_flag->w = SET;
		}
	}
}

/* **********************************************************************************************************************
 * Function Name: message_timeout_DTC_Clear
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */

void Node_Absent_DTC_Clear(uint16_t i)
{
    uint16_t Freeze_Frame_Offset = 0;
    
    if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
    {
	dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
	dtc_param[i].dtc_aging_counter->w = SET;
	dtc_param[i].aging_flag->w = SET;
    }

    if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC) && (dtc_param[i].aging_flag->w == SET)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
    {
	if(EEPROM_IN_USE == 0)
	{
	    eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
	    delay();
	    dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
	    dtc_param[i].aging_flag->w = CLEAR;
	    Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	    eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	    delay();
	}
    }
}

/* **********************************************************************************************************************
 * Function Name: Message_Content_Failure_DTC_Set
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */
void Message_Content_Failure_DTC_Set(uint16_t i)
{
	if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
			delay();
			dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		}
	}

	if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
	{
		dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
		dtc_param[i].dtc_aging_counter->w = CLEAR;
		Freeze_Frame_log(i);
	}

	if(dtc_param[i].aging_flag->w == CLEAR)
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			dtc_param[i].aging_flag->w = SET;
		}
	}
}

/* **********************************************************************************************************************
 * Function Name: Message_Content_Failure_DTC_Clear
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */

void Message_Content_Failure_DTC_Clear(uint16_t i)
{
    uint16_t Freeze_Frame_Offset = 0;
    
    if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
    {
	dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
	dtc_param[i].dtc_aging_counter->w = SET;
	dtc_param[i].aging_flag->w = SET;
    }

    if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC) && (dtc_param[i].aging_flag->w == SET)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
    {
	if(EEPROM_IN_USE == 0)
	{
	    eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
	    delay();
	    dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
	    dtc_param[i].aging_flag->w = CLEAR;
	    Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	    eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	    delay();
	}
    }
}


/* **********************************************************************************************************************
 * Function Name: voltage_Failure_DTC_Set
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */
void voltage_Failure_DTC_Set(uint16_t i)
{
	if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
			delay();
			dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		}
	}
	if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
	{
		dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
		dtc_param[i].dtc_aging_counter->w = CLEAR;
		Freeze_Frame_log(i);
	}

	if(dtc_param[i].aging_flag->w == CLEAR)
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			dtc_param[i].aging_flag->w = SET;
		}
	}
}

/* **********************************************************************************************************************
 * Function Name: voltage_Failure_DTC_Clear
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */

void voltage_Failure_DTC_Clear(uint16_t i)
{
    uint16_t Freeze_Frame_Offset = 0;
    
    if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
    {
	dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
	dtc_param[i].dtc_aging_counter->w = SET;
	dtc_param[i].aging_flag->w = SET;
    }

    if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC) && (dtc_param[i].aging_flag->w == SET)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
    {
	eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
	delay();
	dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
	dtc_param[i].aging_flag->w = CLEAR;
	Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	delay();
    }
}


/* **********************************************************************************************************************
 * Function Name: Failure_DTC_Set
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */
void Failure_DTC_Set(uint16_t i)
{
	if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
			delay();
			dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		}
	}

	if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
	{
		dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
		dtc_param[i].dtc_aging_counter->w = CLEAR;
		Freeze_Frame_log(i);
	}

	if(dtc_param[i].aging_flag->w == CLEAR)
	{
		if(EEPROM_IN_USE == 0)
		{
			eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
			delay();
			dtc_param[i].aging_flag->w = SET;
		}
	}
}

/* **********************************************************************************************************************
 * Function Name: Failure_DTC_Clear
 * Description  : .
 * Arguments    :
 * Return Value : None
 ********************************************************************************************************************** */

void Failure_DTC_Clear(uint16_t i)
{
    uint16_t Freeze_Frame_Offset = 0;
    
    if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
    {
	dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
	dtc_param[i].dtc_aging_counter->w = SET;
	dtc_param[i].aging_flag->w = SET;
    }

    if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC) && (dtc_param[i].aging_flag->w == SET)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
    {
	if(EEPROM_IN_USE == 0)
	{
	    eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
	    delay();
	    dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
	    dtc_param[i].aging_flag->w = CLEAR;
	    Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	    eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
	    delay();
	}
    }
}

/* **********************************************************************************************************************
 * Function Name: can_bus_off_failure
 * Description  : .
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
void can_bus_off_failure_detection(void)
{
	uint8_t  BSUOFF=0;

	if(CAN_Bus_Off == CLEAR)
	{
	    BSUOFF = (RSCAN0C2STSLL & 0x10);

	    if(BSUOFF == 0x10)
	    {
		CAN_Bus_Off = SET;
	    }
	}
	if(CAN_Bus_Off == SET)
	{
	    BSUOFF = (RSCAN0C2STSLL & 0xFF);

	    if(BSUOFF == 0x80)
	    {
		CAN_Bus_Off = CLEAR;
	    }
	}
/*	else
 - 	{
 - 	    BSUOFF = (RSCAN0C2ERFLLL & 0x10);
 - 
 - 	    if(BSUOFF == 0x10)
 - 	    {
 - 		CAN_Bus_Off = CLEAR;
 - 	    }
 - 	}*/
}

/* **********************************************************************************************************************
 * Function Name: can_bus_off_failure
 * Description  : .
 * Arguments    : None
 * Return Value : None
 ********************************************************************************************************************** */
void can_bus_off_failure_DTC(uint16_t i)
{	
	uint16_t Freeze_Frame_Offset = 0;
    
	if(CAN_Bus_Off == SET)
	{
		if((dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CLEAR)||(dtc_param[i].dtc_Buffer->BYTEs[3] == 0XFF)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
		{
			if(EEPROM_IN_USE == 0)
			{
				eeprom_DTC_write(dtc_param[i].DTC_OFFSET, DTC_CNF);
				delay();
				dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
			}
		}
	
		if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF)
		{
			dtc_param[i].dtc_Buffer->BYTEs[3] |= DTC_TEST_FAIL;
			dtc_param[i].dtc_aging_counter->w = CLEAR;
			Freeze_Frame_log(i);
		}

		if(dtc_param[i].aging_flag->w == CLEAR)
		{
			if(EEPROM_IN_USE == 0)
			{
				eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
				delay();
				dtc_param[i].aging_flag->w = SET;
			}
		}
	}
	else
	{
		if(dtc_param[i].dtc_Buffer->BYTEs[3] == DTC_CNF_TEST_FAIL)
		{
		    dtc_param[i].dtc_Buffer->BYTEs[3] = DTC_CNF;
		    dtc_param[i].dtc_aging_counter->w = SET;
		    dtc_param[i].aging_flag->w = SET;
		}

		if(dtc_param[i].aging_flag->w == SET)
		{
		    eeprom_DTC_write(dtc_param[i].DTC_AGING_COUNTER_OFFSET,dtc_param[i].dtc_aging_counter->w);
		    delay();
		    dtc_param[i].aging_flag->w = CLEAR;
		}

		if((dtc_param[i].dtc_aging_counter->w >= IVN_cfg_NM_Drive_Cnt_Clear_DTC)) /* 0x00 - if DTC is cleared and 0xFF - as EEPROM values are 0xFF inoitially*/
		{
			if(EEPROM_IN_USE == 0)
			{
				eeprom_DTC_write(dtc_param[i].DTC_OFFSET, CLEAR);
				delay();
				dtc_param[i].dtc_Buffer->BYTEs[3] = CLEAR;
				dtc_param[i].aging_flag->w = CLEAR;
				Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
				eeprom_write(Freeze_Frame_Offset, DTC_CLEAR);
				delay();
			}
		}
	}
}
/* **********************************************************************************************************************
* Function Name: Freeze_Frame_log
* Description  : .
* Arguments    : None
* Return Value : None
********************************************************************************************************************** */
void Freeze_Frame_log(uint16_t i)
{
	uint16_t Freeze_Frame_Offset = 0;
	uint32_t Odo_dist_in_km = 0;
	
	Freeze_Frame_Offset = (DTC_DATA_START_ADDRESS + i * FREEZE_FRAME_DATA_SIZE);
	
	batt_vtg=(Batt_ADC_value*100)/191;//need to check calculation for vtg
	Odo_dist_in_km = (final_odomter_distance / 1000);
	
	freeze_frame_array[0] = eeprom_DTC_read(Freeze_Frame_Offset);//no of occurrence of DTC
	if(freeze_frame_array[0] == 0xFF)
	{
	    freeze_frame_array[0] = 0;
	}
	freeze_frame_array[0]++;
	
	freeze_frame_array[1] = (dtc_param[i].dtc_aging_counter->w & 0x00ff);
	freeze_frame_array[9] = ((Odo_dist_in_km >> 16) & 0xFF);//ODO when first time DTC set
	freeze_frame_array[10] = ((Odo_dist_in_km >> 8) & 0xFF);
	freeze_frame_array[11] = (Odo_dist_in_km & 0xFF);

	if(freeze_frame_array[0] == 1)
	{
	    if(FEATURE_CODE_SPEED_INPUT == 0)
	    {
		freeze_frame_array[2] = CAN_ESC2_10[1].byte;
		freeze_frame_array[3] = CAN_ESC2_10[0].byte;
	    }
	    else
	    {
		freeze_frame_array[2] = CAN_ESC12_10[1].byte;
		freeze_frame_array[3] = CAN_ESC12_10[0].byte;
	    }
	    freeze_frame_array[4] = ((batt_vtg & 0xFF00) >> 8);
	    freeze_frame_array[5] = (batt_vtg & 0x00FF);
	    freeze_frame_array[6] = ((Odo_dist_in_km >> 16) & 0xFF);//ODO when first time DTC set
	    freeze_frame_array[7] = ((Odo_dist_in_km >> 8) & 0xFF);
	    freeze_frame_array[8] = (Odo_dist_in_km & 0xFF);
	}
	else
	{
	    freeze_frame_array[2] = eeprom_DTC_read(Freeze_Frame_Offset + 2);
	    freeze_frame_array[3] = eeprom_DTC_read(Freeze_Frame_Offset + 3);
	    freeze_frame_array[4] = eeprom_DTC_read(Freeze_Frame_Offset + 4);
	    freeze_frame_array[5] = eeprom_DTC_read(Freeze_Frame_Offset + 5);
	    freeze_frame_array[6] = eeprom_DTC_read(Freeze_Frame_Offset + 6);
	    freeze_frame_array[7] = eeprom_DTC_read(Freeze_Frame_Offset + 7);
	    freeze_frame_array[8] = eeprom_DTC_read(Freeze_Frame_Offset + 8);
	}

	for(uint8_t loop_count = 0;loop_count<12;loop_count++)
	{
		eeprom_write(Freeze_Frame_Offset+loop_count, freeze_frame_array[loop_count]);
	}
}

void Set_Clear_IVN_DTC(void)
{
    can_bus_off_failure_DTC(DTC_IC_BUSOFF_D205);
    
    if(EMS3_10_Message.Message_Timeout_Status == SET)
    {
	   Node_Absent_DTC_Set(DTC_EMS_NODEABSENT_D101);
	   EMS3_10_Message.Message_Timeout_Status = 2;
    }

    if(EMS3_10_Message.Message_Timeout_Status == CLEAR)
    {
	   Node_Absent_DTC_Clear(DTC_EMS_NODEABSENT_D101);
	   //EMS3_10_Message.Message_Recovery_Status = CLEAR;
    }
    
    if(EMS36_10_Message.Message_Timeout_Status == SET)
    {
	   message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
	   EMS36_10_Message.Message_Timeout_Status = 2;
    }

/*     if(EMS36_10_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS36_10_Message.Message_Recovery_Status = CLEAR;
    }  */   
    
    if(EMS1_10_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
		EMS1_10_Message.Message_Timeout_Status = 2;
    }

/*     if(EMS1_10_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS1_10_Message.Message_Recovery_Status = CLEAR;
    }   */ 
    
    if(EMS8_10_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
		EMS8_10_Message.Message_Timeout_Status = 2;
    }

/*     if(EMS8_10_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS8_10_Message.Message_Recovery_Status = CLEAR;
    } */   
    
    if(EMS4_20_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
		EMS4_20_Message.Message_Timeout_Status = 2;
    }

/*     if(EMS4_20_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS4_20_Message.Message_Recovery_Status = CLEAR;
    } */   
    
    if(EMS29_100_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
		EMS29_100_Message.Message_Timeout_Status = 2;
    }

/*     if(EMS29_100_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS29_100_Message.Message_Recovery_Status = CLEAR;
    }  */  
    
    if(EMS6_500_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
		EMS6_500_Message.Message_Timeout_Status = 2;
    }

/*     if(EMS6_500_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS6_500_Message.Message_Recovery_Status = CLEAR;
    } */   
    if(FEATURE_CODE_BATTERY_TT_TYPE==CLEAR)
	{
		if(EMS12_200_Message.Message_Timeout_Status == SET)
		{
			message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
			EMS12_200_Message.Message_Timeout_Status = 2;
		}
	}

/*     if(EMS12_200_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS12_200_Message.Message_Recovery_Status = CLEAR;
    } */   
    
    if(EMS_NSM_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_EMS_MSGTIMEOUT_D001);
		EMS_NSM_Message.Message_Timeout_Status = 2;
    }

    if((EMS_NSM_Message.Message_Timeout_Status == CLEAR) &&
	  (EMS12_200_Message.Message_Timeout_Status == CLEAR) &&
	  (EMS6_500_Message.Message_Timeout_Status == CLEAR) &&
	  (EMS29_100_Message.Message_Timeout_Status == CLEAR) &&
	  (EMS4_20_Message.Message_Timeout_Status == CLEAR) &&
	  (EMS8_10_Message.Message_Timeout_Status == CLEAR) &&
	  (EMS1_10_Message.Message_Timeout_Status == CLEAR) && 
	  (EMS36_10_Message.Message_Timeout_Status == CLEAR))
    {
		message_timeout_DTC_Clear(DTC_EMS_MSGTIMEOUT_D001);
		//EMS_NSM_Message.Message_Recovery_Status = CLEAR;
    }

    if(ESC5_10_Message.Message_Timeout_Status == SET)
    {
		Node_Absent_DTC_Set(DTC_ABS_ESC_NODEABSENT_D103);
		ESC5_10_Message.Message_Timeout_Status = 2;
    }

    if(ESC5_10_Message.Message_Timeout_Status == CLEAR)
    {
		Node_Absent_DTC_Clear(DTC_ABS_ESC_NODEABSENT_D103);
		//ESC5_10_Message.Message_Recovery_Status = CLEAR;
    }
    
    if(ESC2_10_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_ABS_ESC_MSGTIMEOUT_D003);
		ESC2_10_Message.Message_Timeout_Status = 2;
    }

/*     if(ESC2_10_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_ABS_ESC_MSGTIMEOUT_D003);
		//ESC2_10_Message.Message_Recovery_Status = CLEAR;
    } */    
    
    if(ESC12_10_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_ABS_ESC_MSGTIMEOUT_D003);
		ESC12_10_Message.Message_Timeout_Status = 2;
    }

/*     if(ESC12_10_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_ABS_ESC_MSGTIMEOUT_D003);
		//ESC12_10_Message.Message_Recovery_Status = CLEAR;
    } */   
    
    if(ESC_NSM_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_ABS_ESC_MSGTIMEOUT_D003);
		ESC_NSM_Message.Message_Timeout_Status = 2;
    }

    if((ESC_NSM_Message.Message_Timeout_Status == CLEAR) &&
	  (ESC12_10_Message.Message_Timeout_Status == CLEAR) &&
	  (ESC2_10_Message.Message_Timeout_Status == CLEAR))
    {
		message_timeout_DTC_Clear(DTC_ABS_ESC_MSGTIMEOUT_D003);
		//ESC_NSM_Message.Message_Recovery_Status = CLEAR;
    }

    if(TCU5_10_Message.Message_Timeout_Status == SET)
    {
		Node_Absent_DTC_Set(DTC_TCU_NODEABSENT_D102);
		TCU5_10_Message.Message_Timeout_Status = 2;
    }

    if(TCU5_10_Message.Message_Timeout_Status == CLEAR)
    {
		Node_Absent_DTC_Clear(DTC_TCU_NODEABSENT_D102);
		//TCU5_10_Message.Message_Recovery_Status = CLEAR;
    }
    
    if(TCU6_20_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_TCU_MSGTIMEOUT_D002);
		TCU6_20_Message.Message_Timeout_Status = 2;
    }

/*     if(TCU6_20_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_TCU_MSGTIMEOUT_D002);
		//TCU6_20_Message.Message_Recovery_Status = CLEAR;
    } */    
    
    if(TCU_NSM_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_TCU_MSGTIMEOUT_D002);
		TCU_NSM_Message.Message_Timeout_Status = 2;
    }

    if((TCU_NSM_Message.Message_Timeout_Status == CLEAR) &&
	  (TCU6_20_Message.Message_Timeout_Status == CLEAR))
    {
		message_timeout_DTC_Clear(DTC_TCU_MSGTIMEOUT_D002);
		//TCU_NSM_Message.Message_Recovery_Status = CLEAR;
    }

    if(MBFM1_100_Message.Message_Timeout_Status == SET)
    {
		Node_Absent_DTC_Set(DTC_MBFM_NODEABSENT_D104);
		MBFM1_100_Message.Message_Timeout_Status = 2;
    }

    if(MBFM1_100_Message.Message_Timeout_Status == CLEAR)
    {
		Node_Absent_DTC_Clear(DTC_MBFM_NODEABSENT_D104);
		//MBFM1_100_Message.Message_Recovery_Status = CLEAR;
    }
#ifndef U350    
    if(MBFM5_100_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_MBFM_MSGTIMEOUT_D004);
		MBFM5_100_Message.Message_Timeout_Status = 2;
    }

 /*   if(MBFM5_100_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_MBFM_MSGTIMEOUT_D004);
		//MBFM5_100_Message.Message_Recovery_Status = CLEAR;
    }   */
    
    if(MBFM9_500_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_MBFM_MSGTIMEOUT_D004);
		MBFM9_500_Message.Message_Timeout_Status = 2;
    }

/*     if(MBFM9_500_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_MBFM_MSGTIMEOUT_D004);
		//MBFM9_500_Message.Message_Recovery_Status = CLEAR;
    } */   
    
    if(MBFM_PAS1_50_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_MBFM_MSGTIMEOUT_D004);
		MBFM_PAS1_50_Message.Message_Timeout_Status = 2;
    }

    if((MBFM_PAS1_50_Message.Message_Timeout_Status == CLEAR) &&
	  (MBFM9_500_Message.Message_Timeout_Status == CLEAR) &&
	  (MBFM5_100_Message.Message_Timeout_Status == CLEAR))
    {
		message_timeout_DTC_Clear(DTC_MBFM_MSGTIMEOUT_D004);
		//MBFM_PAS1_50_Message.Message_Recovery_Status = CLEAR;
    }
#endif
    if(SRS1_20_Message.Message_Timeout_Status == SET)
    {
		Node_Absent_DTC_Set(DTC_SRS_NODEABSENT_D106);
		SRS1_20_Message.Message_Timeout_Status = 2;
    }

    if(SRS1_20_Message.Message_Timeout_Status == CLEAR)
    {
		Node_Absent_DTC_Clear(DTC_SRS_NODEABSENT_D106);
		//SRS1_20_Message.Message_Recovery_Status = CLEAR;
    }

    if(PKE_ICU2_100_Message.Message_Timeout_Status == SET)
    {
		if(VAR_CODING_PASSIVE_KEY_ENTRY_SMART_KEY_MODULE == SET)
		{
			Node_Absent_DTC_Set(DTC_PKE_NODEABSENT_D130);
		}
		else if(VAR_CODING_IMMOBILIZER_CONTROL_UNIT == SET)
		{
			Node_Absent_DTC_Set(DTC_ICU_NODEABSENT_D117);
		}
		PKE_ICU2_100_Message.Message_Timeout_Status = 2;
    }

    if(PKE_ICU2_100_Message.Message_Timeout_Status == CLEAR)
    {
		if(VAR_CODING_PASSIVE_KEY_ENTRY_SMART_KEY_MODULE == SET)
		{
			Node_Absent_DTC_Clear(DTC_PKE_NODEABSENT_D130);
		}
		else if(VAR_CODING_IMMOBILIZER_CONTROL_UNIT == SET)
		{
			Node_Absent_DTC_Clear(DTC_ICU_NODEABSENT_D117);
		}
		//PKE_ICU2_100_Message.Message_Recovery_Status = CLEAR;
    }
#ifdef U350
    if(RPAS1_20_Message.Message_Timeout_Status == SET)
    {
		Node_Absent_DTC_Set(DTC_RPAS_NODEABSENT_D112);
		RPAS1_20_Message.Message_Timeout_Status = 2;
    }

    if(RPAS1_20_Message.Message_Timeout_Status == CLEAR)
    {
		Node_Absent_DTC_Clear(DTC_RPAS_NODEABSENT_D112);
		//RPAS1_20_Message.Message_Recovery_Status = CLEAR;
    }    
    
    if(RPAS2_100_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_RPAS_MSGTIMEOUT_D012);
		RPAS2_100_Message.Message_Timeout_Status = 2;
    }

    if(RPAS2_100_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_RPAS_MSGTIMEOUT_D012);
		//RPAS2_100_Message.Message_Recovery_Status = CLEAR;
    }
#endif
    if(IS1_100_Message.Message_Timeout_Status == SET)
    {
		Node_Absent_DTC_Set(DTC_IS_NODEABSENT_D108);
		IS1_100_Message.Message_Timeout_Status = 2;
    }

    if(IS1_100_Message.Message_Timeout_Status == CLEAR)
    {
		Node_Absent_DTC_Clear(DTC_IS_NODEABSENT_D108);
		//IS1_100_Message.Message_Recovery_Status = CLEAR;
    }    
#ifndef U350    
    if(IS3_500_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_IS_MSGTIMEOUT_D008);
		IS3_500_Message.Message_Timeout_Status = 2;
    }

/*     if(IS3_500_Message.Message_Timeout_Status == CLEAR)
    {
		message_timeout_DTC_Clear(DTC_IS_MSGTIMEOUT_D008);
		//IS3_500_Message.Message_Recovery_Status = CLEAR;
    } */    
#endif    
    if(IS_NSM_Message.Message_Timeout_Status == SET)
    {
		message_timeout_DTC_Set(DTC_IS_MSGTIMEOUT_D008);
		IS_NSM_Message.Message_Timeout_Status = 2;
    }

    
#ifndef U350
    if((IS_NSM_Message.Message_Timeout_Status == CLEAR) && 
	(IS3_500_Message.Message_Timeout_Status == CLEAR))
#else
	if(IS_NSM_Message.Message_Timeout_Status == CLEAR)
#endif
    {
		message_timeout_DTC_Clear(DTC_IS_MSGTIMEOUT_D008);
		//IS_NSM_Message.Message_Recovery_Status = CLEAR;
    }

    if(EMS1_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS1_10_MSGCONTFAIL_E401);
	EMS1_10_Message_Content.Failure_Status = 2;
    }

    if(EMS1_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS1_10_MSGCONTFAIL_E401);
	//EMS1_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS36_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS36_10_MSGCONTFAIL_E4A2);
	EMS36_10_Message_Content.Failure_Status = 2;
    }

    if(EMS36_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS36_10_MSGCONTFAIL_E4A2);
	//EMS36_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS3_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS3_10_MSGCONTFAIL_E403);
	EMS3_10_Message_Content.Failure_Status = 2;
    }

    if(EMS3_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS3_10_MSGCONTFAIL_E403);
	//EMS3_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS8_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS8_10_MSGCONTFAIL_E407);
	EMS8_10_Message_Content.Failure_Status = 2;
    }

    if(EMS8_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS8_10_MSGCONTFAIL_E407);
	//EMS8_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS4_20_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS4_20_MSGCONTFAIL_E404);
	EMS4_20_Message_Content.Failure_Status = 2;
    }

    if(EMS4_20_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS4_20_MSGCONTFAIL_E404);
	//EMS4_20_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS29_100_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS29_100_MSGCONTFAIL_E43A);
	EMS29_100_Message_Content.Failure_Status = 2;
    }

    if(EMS29_100_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS29_100_MSGCONTFAIL_E43A);
	//EMS29_100_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS6_500_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS6_500_MSGCONTFAIL_E406);
	EMS6_500_Message_Content.Failure_Status = 2;
    }

    if(EMS6_500_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS6_500_MSGCONTFAIL_E406);
	//EMS6_500_Message_Content.Recovery_Status = CLEAR;
    }

    if(EMS12_200_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_EMS12_200_MSGCONTFAIL_E409);
	EMS12_200_Message_Content.Failure_Status = 2;
    }

    if(EMS12_200_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_EMS12_200_MSGCONTFAIL_E409);
	//EMS12_200_Message_Content.Recovery_Status = CLEAR;
    }

    if(ESC5_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_ESC5_10_MSGCONTFAIL_E420);
	ESC5_10_Message_Content.Failure_Status = 2;
    }

    if(ESC5_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_ESC5_10_MSGCONTFAIL_E420);
	//ESC5_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(ESC2_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_ESC2_10_MSGCONTFAIL_E40D);
	ESC2_10_Message_Content.Failure_Status = 2;
    }

    if(ESC2_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_ESC2_10_MSGCONTFAIL_E40D);
	//ESC2_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(ESC12_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_ESC12_10_MSGCONTFAIL_E4AE);
	ESC12_10_Message_Content.Failure_Status = 2;
    }

    if(ESC12_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_ESC12_10_MSGCONTFAIL_E4AE);
	//ESC12_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(TCU5_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_TCU5_10_MSGCONTFAIL_E4AF);
	TCU5_10_Message_Content.Failure_Status = 2;
    }

    if(TCU5_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_TCU5_10_MSGCONTFAIL_E4AF);
	//TCU5_10_Message_Content.Recovery_Status = CLEAR;
    }

    if(TCU6_20_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_TCU6_20_MSGCONTFAIL_E4B0);
	TCU6_20_Message_Content.Failure_Status = 2;
    }

    if(TCU6_20_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_TCU6_20_MSGCONTFAIL_E4B0);
	//TCU6_20_Message_Content.Recovery_Status = CLEAR;
    }

    if(MBFM1_100_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_MBFM1_100_MSGCONTFAIL_E40F);
	MBFM1_100_Message_Content.Failure_Status = 2;
    }

    if(MBFM1_100_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_MBFM1_100_MSGCONTFAIL_E40F);
	//MBFM1_100_Message_Content.Recovery_Status = CLEAR;
    }

    if(MBFM5_100_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_MBFM5_100_MSGCONTFAIL_E410);
	MBFM5_100_Message_Content.Failure_Status = 2;
    }

    if(MBFM5_100_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_MBFM5_100_MSGCONTFAIL_E410);
	//MBFM5_100_Message_Content.Recovery_Status = CLEAR;
    }

    if(MBFM9_500_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_MBFM9_500_MSGCONTFAIL_E4BA);
	MBFM9_500_Message_Content.Failure_Status = 2;
    }

    if(MBFM9_500_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_MBFM9_500_MSGCONTFAIL_E4BA);
	//MBFM9_500_Message_Content.Recovery_Status = CLEAR;
    }

    if(MBFM_PAS1_50_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_MBFM_PAS1_50_MSGCONTFAIL_E4B8);
	MBFM_PAS1_50_Message_Content.Failure_Status = 2;
    }

    if(MBFM_PAS1_50_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_MBFM_PAS1_50_MSGCONTFAIL_E4B8);
	//MBFM_PAS1_50_Message_Content.Recovery_Status = CLEAR;
    }

    if(SRS1_20_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_SRS1_20_MSGCONTFAIL_E415);
	SRS1_20_Message_Content.Failure_Status = 2;
    }

    if(SRS1_20_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_SRS1_20_MSGCONTFAIL_E415);
	//SRS1_20_Message_Content.Recovery_Status = CLEAR;
    }

    if(PKE_ICU2_100_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_PKE_ICU2_100_MSGCONTFAIL_E418);
	PKE_ICU2_100_Message_Content.Failure_Status = 2;
    }

    if(PKE_ICU2_100_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_PKE_ICU2_100_MSGCONTFAIL_E418);
	//PKE_ICU2_100_Message_Content.Recovery_Status = CLEAR;
    }
#ifdef SPRINT4
	/*TDMS*/
	if(SAS1_10_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_SAS1_10_MSGCONTFAIL_E426);
	SAS1_10_Message_Content.Failure_Status = 2;
    }

    if(SAS1_10_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_SAS1_10_MSGCONTFAIL_E426);
	//SAS1_10_Message_Content.Recovery_Status = CLEAR;
    }
#endif
#ifdef U350
    if(RPAS1_20_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_RPAS1_20_MSGCONTFAIL_E437);
	RPAS1_20_Message_Content.Failure_Status = 2;
    }

    if(RPAS1_20_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_RPAS1_20_MSGCONTFAIL_E437);
	//RPAS1_20_Message_Content.Recovery_Status = CLEAR;
    }

    if(RPAS2_100_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_RPAS2_100_MSGCONTFAIL_E438);
	RPAS2_100_Message_Content.Failure_Status = 2;
    }

    if(RPAS2_100_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_RPAS2_100_MSGCONTFAIL_E438);
	//RPAS2_100_Message_Content.Recovery_Status = CLEAR;
    }
#endif
    if(IS1_100_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_IS1_100_MSGCONTFAIL_E416);
	IS1_100_Message_Content.Failure_Status = 2;
    }

    if(IS1_100_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_IS1_100_MSGCONTFAIL_E416);
	//IS1_100_Message_Content.Recovery_Status = CLEAR;
    }

    if(IS3_500_Message_Content.Failure_Status == SET)
    {
	Message_Content_Failure_DTC_Set(DTC_IS3_500_MSGCONTFAIL_E485);
	IS3_500_Message_Content.Failure_Status = 2;
    }

    if(IS3_500_Message_Content.Failure_Status == CLEAR)
    {
	Message_Content_Failure_DTC_Clear(DTC_IS3_500_MSGCONTFAIL_E485);
	//IS3_500_Message_Content.Recovery_Status = CLEAR;
    }

	
	/*if(EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Failure_Status == 2)
	{
		Failure_DTC_Set(DTC_ENG_TRQ_AFTER_RED1_SIGNAL_CONTFAIL_E011);
		EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Failure_Status = 3;
	}
	

    if(EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_ENG_TRQ_AFTER_RED1_SIGNAL_CONTFAIL_E011);
	//EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Recovery_Status = CLEAR;
    }*/

    if(EMS1_10_Temp_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008);
	EMS1_10_Temp_Sig_Content.Failure_Status = 3;
    }

    if(EMS1_10_Temp_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_EMS_ENG_TEMP_SIGNAL_CONTFAIL_E008);
	//EMS1_10_Temp_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS1_10_RPM_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_EMS_RPM_SIGNAL_CONTFAIL_E004);
	EMS1_10_RPM_Sig_Content.Failure_Status = 3;
    }

    if(EMS1_10_RPM_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_EMS_RPM_SIGNAL_CONTFAIL_E004);
	//EMS1_10_RPM_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS1_10_GEAR_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
	EMS1_10_GEAR_Sig_Content.Failure_Status = 3;
    }

    if(EMS1_10_GEAR_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
	//EMS1_10_GEAR_Sig_Content.Recovery_Status = CLEAR;
    }

	if(FEATURE_CODE_CRUISE_TT_CONFIG==SET)
	{
		if(EMS1_10_INDC_CRUISE_Sig_Content.Failure_Status == 2)
		{
		Failure_DTC_Set(DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055);
		EMS1_10_INDC_CRUISE_Sig_Content.Failure_Status = 3;
		}
    }

    if(EMS1_10_INDC_CRUISE_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_EMS_INDC_CRUISE_SIGNAL_CONTFAIL_E055);
	//EMS1_10_INDC_CRUISE_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS3_10_CLUCH_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
	EMS3_10_CLUCH_Sig_Content.Failure_Status = 3;
    }

    if(EMS3_10_CLUCH_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
	//EMS3_10_CLUCH_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS3_10_STS_ESS_INDC_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD);
	EMS3_10_STS_ESS_INDC_Sig_Content.Failure_Status = 3;
    }

    if(EMS3_10_STS_ESS_INDC_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_STS_ESS_INDC_SIGNAL_CONTFAIL_E0AD);
	//EMS3_10_STS_ESS_INDC_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS8_10_TARGET_GEAR_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
	EMS8_10_TARGET_GEAR_Sig_Content.Failure_Status = 3;
    }

    if(EMS8_10_TARGET_GEAR_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
	//EMS8_10_TARGET_GEAR_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS4_20_ENG_DRIVE_MODE_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B);
	EMS4_20_ENG_DRIVE_MODE_Sig_Content.Failure_Status = 3;
    }

    if(EMS4_20_ENG_DRIVE_MODE_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_ENG_DRIVE_MODE_SIGNAL_CONTFAIL_E09B);
	//EMS4_20_ENG_DRIVE_MODE_Sig_Content.Recovery_Status = CLEAR;
    }

    if(TCU5_10_GEAR_ACTUAL_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072);
	TCU5_10_GEAR_ACTUAL_Sig_Content.Failure_Status = 3;
    }

    if(TCU5_10_GEAR_ACTUAL_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072);
	//TCU5_10_GEAR_ACTUAL_Sig_Content.Recovery_Status = CLEAR;
    }

    if(TCU5_10_GEAR_TARGET_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA);
	TCU5_10_GEAR_TARGET_Sig_Content.Failure_Status = 3;
    }

    if(TCU5_10_GEAR_TARGET_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_GEAR_TARGET_SIGNAL_CONTFAIL_E0CA);
	//TCU5_10_GEAR_TARGET_Sig_Content.Recovery_Status = CLEAR;
    }

    if(TCU6_20_TGS_LEVER_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073);
	TCU6_20_TGS_LEVER_Sig_Content.Failure_Status = 3;
    }

    if(TCU6_20_TGS_LEVER_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073);
	//TCU6_20_TGS_LEVER_Sig_Content.Recovery_Status = CLEAR;
    }

    if(TCU6_20_MODE_SIGNAL_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0);
	TCU6_20_MODE_SIGNAL_Sig_Content.Failure_Status = 3;
    }

    if(TCU6_20_MODE_SIGNAL_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_TGS_MODE_SIGNAL_CONTFAIL_E0E0);
	//TCU6_20_MODE_SIGNAL_Sig_Content.Recovery_Status = CLEAR;
    }

    if(TCU6_20_INDC_AT_MALFUNC_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF);
	TCU6_20_INDC_AT_MALFUNC_Sig_Content.Failure_Status = 3;
    }

    if(TCU6_20_INDC_AT_MALFUNC_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_INDC_AT_MALFUNC_SIGNAL_CONTFAIL_E0DF);
	//TCU6_20_INDC_AT_MALFUNC_Sig_Content.Recovery_Status = CLEAR;
    }

    if(SRS1_20_INDC_SRSC_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0);
	SRS1_20_INDC_SRSC_Sig_Content.Failure_Status = 3;
    }

    if(SRS1_20_INDC_SRSC_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_INDC_SRS_SIGNAL_CONTFAIL_E0D0);
	//SRS1_20_INDC_SRSC_Sig_Content.Recovery_Status = CLEAR;
    }

    if(EMS6_500_DISP_AMBT_TEMP_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE);
	EMS6_500_DISP_AMBT_TEMP_Sig_Content.Failure_Status = 3;
    }

    if(EMS6_500_DISP_AMBT_TEMP_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_DISP_AMBT_TEMP_EMS_SIGNAL_CONTFAIL_E0BE);
	//EMS6_500_DISP_AMBT_TEMP_Sig_Content.Recovery_Status = CLEAR;
    }

    if(MBFM1_100_MBFM_ENGOFF_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018);
	MBFM1_100_MBFM_ENGOFF_Sig_Content.Failure_Status = 3;
    }

    if(MBFM1_100_MBFM_ENGOFF_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_MBFM_ENGOFF_SIGNAL_CONTFAIL_E018);
	//MBFM1_100_MBFM_ENGOFF_Sig_Content.Recovery_Status = CLEAR;
    }

    if(RPAS1_20_DISP_DIST_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3);
	RPAS1_20_DISP_DIST_Sig_Content.Failure_Status = 3;
    }

    if(RPAS1_20_DISP_DIST_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_RPAS_DISP_DIST_SIGNAL_CONTFAIL_E0E3);
	//RPAS1_20_DISP_DIST_Sig_Content.Recovery_Status = CLEAR;
    }

    if(ESC2_10_ABS_VEH_SPEED_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013);
	ESC2_10_ABS_VEH_SPEED_Sig_Content.Failure_Status = 3;
    }

    if(ESC2_10_ABS_VEH_SPEED_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_ABS_VEH_SPEED_SIGNAL_CONTFAIL_E013);
	//ESC2_10_ABS_VEH_SPEED_Sig_Content.Recovery_Status = CLEAR;
    }

    if(ESC2_10_ABS_ODO_DIST_Sig_Content.Failure_Status == 2)
    {
	Failure_DTC_Set(DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014);
	ESC2_10_ABS_ODO_DIST_Sig_Content.Failure_Status = 3;
    }

    if(ESC2_10_ABS_ODO_DIST_Sig_Content.Failure_Status == CLEAR)
    {
	Failure_DTC_Clear(DTC_ABS_ODO_DIST_SIGNAL_CONTFAIL_E014);
	//ESC2_10_ABS_ODO_DIST_Sig_Content.Recovery_Status = CLEAR;
    }
}

void Clear_IVN_Variables(void)
{
	
		EMS3_10_Message.Timeout_Counter = 0;
		EMS3_10_Message.Recovery_Counter = 0;
		EMS3_10_Message.Message_Received_Counter = 0;
		if(EMS3_10_Message.Message_Timeout_Status ==2)
		{
			EMS3_10_Message.Message_Timeout_Status = 1;
		}
		else
		{
			EMS3_10_Message.Message_Timeout_Status = 0;
		}
		EMS3_10_Message.Message_Recovery_Status = 0;
	

    EMS36_10_Message.Timeout_Counter = 0;
    EMS36_10_Message.Recovery_Counter = 0;
    EMS36_10_Message.Message_Received_Counter = 0;
	if(EMS36_10_Message.Message_Timeout_Status==2)
	{
		EMS36_10_Message.Message_Timeout_Status=1;
	}
	else
	{
		EMS36_10_Message.Message_Timeout_Status = 0;
	}
    EMS36_10_Message.Message_Recovery_Status = 0;

    EMS1_10_Message.Timeout_Counter = 0;
    EMS1_10_Message.Recovery_Counter = 0;
    EMS1_10_Message.Message_Received_Counter = 0;
	if(EMS1_10_Message.Message_Timeout_Status==2)
	{
		EMS1_10_Message.Message_Timeout_Status = 1;
	}
	else
	{
		EMS1_10_Message.Message_Recovery_Status = 0;
	}

    EMS8_10_Message.Timeout_Counter = 0;
    EMS8_10_Message.Recovery_Counter = 0;
    EMS8_10_Message.Message_Received_Counter = 0;
	if(EMS8_10_Message.Message_Timeout_Status==2)
	{
		EMS8_10_Message.Message_Timeout_Status = 1;
	}
	else
	{
		EMS8_10_Message.Message_Timeout_Status = 0;
	}
    EMS8_10_Message.Message_Recovery_Status = 0;

    EMS4_20_Message.Timeout_Counter = 0;
    EMS4_20_Message.Recovery_Counter = 0;
    EMS4_20_Message.Message_Received_Counter = 0;
	if(EMS4_20_Message.Message_Timeout_Status==2)
	{
		EMS4_20_Message.Message_Timeout_Status=1;
	}
	else
	{
		EMS4_20_Message.Message_Timeout_Status = 0;
	}
    EMS4_20_Message.Message_Recovery_Status = 0;

    EMS29_100_Message.Timeout_Counter = 0;
    EMS29_100_Message.Recovery_Counter = 0;
    EMS29_100_Message.Message_Received_Counter = 0;
	if(EMS29_100_Message.Message_Timeout_Status==2)
	{
		EMS29_100_Message.Message_Timeout_Status=1;
	}
	else
	{
		EMS29_100_Message.Message_Timeout_Status = 0;
	}
    EMS29_100_Message.Message_Recovery_Status = 0;

    EMS6_500_Message.Timeout_Counter = 0;
    EMS6_500_Message.Recovery_Counter = 0;
    EMS6_500_Message.Message_Received_Counter = 0;
	if(EMS6_500_Message.Message_Timeout_Status==2)
	{
		EMS6_500_Message.Message_Timeout_Status = 1;
	}
	else
	{
		EMS6_500_Message.Message_Timeout_Status = 0;
	}
    EMS6_500_Message.Message_Recovery_Status = 0;

    EMS12_200_Message.Timeout_Counter = 0;
    EMS12_200_Message.Recovery_Counter = 0;
    EMS12_200_Message.Message_Received_Counter = 0;

	if((EMS12_200_Message.Message_Timeout_Status==2)&&(FEATURE_CODE_BATTERY_TT_TYPE==CLEAR))
	{
		EMS12_200_Message.Message_Timeout_Status=1;
	}
	else
	{
		EMS12_200_Message.Message_Timeout_Status = 0;
	}
    EMS12_200_Message.Message_Recovery_Status = 0;

    EMS_NSM_Message.Timeout_Counter = 0;
    EMS_NSM_Message.Recovery_Counter = 0;
    EMS_NSM_Message.Message_Received_Counter = 0;
	if(EMS_NSM_Message.Message_Timeout_Status ==2)
	{
		EMS_NSM_Message.Message_Timeout_Status = 1;
	}
	else
	{
		EMS_NSM_Message.Message_Timeout_Status = 0;
	}
    EMS_NSM_Message.Message_Recovery_Status = 0;

    ESC5_10_Message.Timeout_Counter = 0;
    ESC5_10_Message.Recovery_Counter = 0;
    ESC5_10_Message.Message_Received_Counter = 0;
	if(ESC5_10_Message.Message_Timeout_Status ==2)
	{
		ESC5_10_Message.Message_Timeout_Status = 1;
	}
	else
	{
		ESC5_10_Message.Message_Timeout_Status = 0;
	}
    ESC5_10_Message.Message_Recovery_Status = 0;

    ESC2_10_Message.Timeout_Counter = 0;
    ESC2_10_Message.Recovery_Counter = 0;
    ESC2_10_Message.Message_Received_Counter = 0;
	if(ESC2_10_Message.Message_Timeout_Status ==2)
	{
		ESC2_10_Message.Message_Timeout_Status = 1;
	}
	else
	{
		ESC2_10_Message.Message_Timeout_Status = 0;
	}
    ESC2_10_Message.Message_Recovery_Status = 0;

    ESC12_10_Message.Timeout_Counter = 0;
    ESC12_10_Message.Recovery_Counter = 0;
    ESC12_10_Message.Message_Received_Counter = 0;
	if(ESC12_10_Message.Message_Timeout_Status ==2)
	{
		ESC12_10_Message.Message_Timeout_Status = 1;
	}
	else
	{
		ESC12_10_Message.Message_Timeout_Status = 0;
	}
    ESC12_10_Message.Message_Recovery_Status = 0;

    ESC_NSM_Message.Timeout_Counter = 0;
    ESC_NSM_Message.Recovery_Counter = 0;
    ESC_NSM_Message.Message_Received_Counter = 0;
	if(ESC_NSM_Message.Message_Timeout_Status ==2)
	{
		ESC_NSM_Message.Message_Timeout_Status =1;
	}
	else
	{
		ESC_NSM_Message.Message_Timeout_Status = 0;
	}
    ESC_NSM_Message.Message_Recovery_Status = 0;

    TCU5_10_Message.Timeout_Counter = 0;
    TCU5_10_Message.Recovery_Counter = 0;
    TCU5_10_Message.Message_Received_Counter = 0;
	if(TCU5_10_Message.Message_Timeout_Status ==2)
	{
		TCU5_10_Message.Message_Timeout_Status = 1;
	}
	else
	{
		TCU5_10_Message.Message_Timeout_Status = 0;
	}
    TCU5_10_Message.Message_Recovery_Status = 0;

    TCU6_20_Message.Timeout_Counter = 0;
    TCU6_20_Message.Recovery_Counter = 0;
    TCU6_20_Message.Message_Received_Counter = 0;
	if(TCU6_20_Message.Message_Timeout_Status ==2)
	{
		TCU6_20_Message.Message_Timeout_Status = 1;
	}
	else
	{
		TCU6_20_Message.Message_Timeout_Status = 0;
	}
    TCU6_20_Message.Message_Recovery_Status = 0;

    TCU_NSM_Message.Timeout_Counter = 0;
    TCU_NSM_Message.Recovery_Counter = 0;
    TCU_NSM_Message.Message_Received_Counter = 0;
	if(TCU_NSM_Message.Message_Timeout_Status ==2)
	{
		TCU_NSM_Message.Message_Timeout_Status = 1;
	}
	else
	{
		TCU_NSM_Message.Message_Timeout_Status = 0;
	}
    TCU_NSM_Message.Message_Recovery_Status = 0;

    MBFM1_100_Message.Timeout_Counter = 0;
    MBFM1_100_Message.Recovery_Counter = 0;
    MBFM1_100_Message.Message_Received_Counter = 0;
	if(MBFM1_100_Message.Message_Timeout_Status ==2)
	{
		MBFM1_100_Message.Message_Timeout_Status = 1;
	}
	else
	{
		MBFM1_100_Message.Message_Timeout_Status = 0;
	}
    MBFM1_100_Message.Message_Recovery_Status = 0;

    MBFM5_100_Message.Timeout_Counter = 0;
    MBFM5_100_Message.Recovery_Counter = 0;
    MBFM5_100_Message.Message_Received_Counter = 0;
	if(MBFM5_100_Message.Message_Timeout_Status ==2)
	{
		MBFM5_100_Message.Message_Timeout_Status = 1;
	}
	else
	{
		MBFM5_100_Message.Message_Timeout_Status = 0;
	}
    MBFM5_100_Message.Message_Recovery_Status = 0;

    MBFM9_500_Message.Timeout_Counter = 0;
    MBFM9_500_Message.Recovery_Counter = 0;
    MBFM9_500_Message.Message_Received_Counter = 0;
    if(MBFM9_500_Message.Message_Timeout_Status ==2)
	{
		MBFM9_500_Message.Message_Timeout_Status = 1;
	}
	else
	{
		MBFM9_500_Message.Message_Timeout_Status = 0;
	}
    MBFM9_500_Message.Message_Recovery_Status = 0;

    MBFM_PAS1_50_Message.Timeout_Counter = 0;
    MBFM_PAS1_50_Message.Recovery_Counter = 0;
    MBFM_PAS1_50_Message.Message_Received_Counter = 0;
	if(MBFM_PAS1_50_Message.Message_Timeout_Status ==2)
	{
		MBFM_PAS1_50_Message.Message_Timeout_Status = 1;
	}
	else
	{
		MBFM_PAS1_50_Message.Message_Timeout_Status = 0;
	}
    MBFM_PAS1_50_Message.Message_Recovery_Status = 0;

    SRS1_20_Message.Timeout_Counter = 0;
    SRS1_20_Message.Recovery_Counter = 0;
    SRS1_20_Message.Message_Received_Counter = 0;
	if(SRS1_20_Message.Message_Timeout_Status ==2)
	{
		SRS1_20_Message.Message_Timeout_Status = 1;
	}
	else
	{
		SRS1_20_Message.Message_Timeout_Status = 0;
	}
    SRS1_20_Message.Message_Recovery_Status = 0;

    PKE_ICU2_100_Message.Timeout_Counter = 0;
    PKE_ICU2_100_Message.Recovery_Counter = 0;
    PKE_ICU2_100_Message.Message_Received_Counter = 0;
	if(PKE_ICU2_100_Message.Message_Timeout_Status ==2)
	{
		PKE_ICU2_100_Message.Message_Timeout_Status = 1;
	}
	else
	{
		PKE_ICU2_100_Message.Message_Timeout_Status = 0;
	}
    PKE_ICU2_100_Message.Message_Recovery_Status = 0;

    RPAS1_20_Message.Timeout_Counter = 0;
    RPAS1_20_Message.Recovery_Counter = 0;
    RPAS1_20_Message.Message_Received_Counter = 0;
	if(RPAS1_20_Message.Message_Timeout_Status ==2)
	{
		RPAS1_20_Message.Message_Timeout_Status = 1;
	}
	else
	{
		RPAS1_20_Message.Message_Timeout_Status = 0;
	}
    RPAS1_20_Message.Message_Recovery_Status = 0;

    RPAS2_100_Message.Timeout_Counter = 0;
    RPAS2_100_Message.Recovery_Counter = 0;
    RPAS2_100_Message.Message_Received_Counter = 0;
	if(RPAS2_100_Message.Message_Timeout_Status==2)
	{
		RPAS2_100_Message.Message_Timeout_Status=1;
	}
	else
	{
		RPAS2_100_Message.Message_Timeout_Status = 0;
	}
    RPAS2_100_Message.Message_Recovery_Status = 0;

    IS1_100_Message.Timeout_Counter = 0;
    IS1_100_Message.Recovery_Counter = 0;
    IS1_100_Message.Message_Received_Counter = 0;
	if(IS1_100_Message.Message_Timeout_Status ==2)
	{
		IS1_100_Message.Message_Timeout_Status = 1;
	}
	else
	{
		IS1_100_Message.Message_Timeout_Status = 0;
	}
    IS1_100_Message.Message_Recovery_Status = 0;

    IS3_500_Message.Timeout_Counter = 0;
    IS3_500_Message.Recovery_Counter = 0;
    IS3_500_Message.Message_Received_Counter = 0;
    if(IS3_500_Message.Message_Timeout_Status ==2)
	{
		IS3_500_Message.Message_Timeout_Status = 1;
	}
	else
	{
		IS3_500_Message.Message_Timeout_Status = 0;
	}
    IS3_500_Message.Message_Recovery_Status = 0;

    IS_NSM_Message.Timeout_Counter = 0;
    IS_NSM_Message.Recovery_Counter = 0;
    IS_NSM_Message.Message_Received_Counter = 0;
	if(IS_NSM_Message.Message_Timeout_Status==2)
	{
		IS_NSM_Message.Message_Timeout_Status=1;
	}
	else
	{
		IS_NSM_Message.Message_Timeout_Status = 0;
	}
    IS_NSM_Message.Message_Recovery_Status = 0;

    memset(&EMS3_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS36_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS1_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS8_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS4_20_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS29_100_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS6_500_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS12_200_Message_Content, 0, sizeof(Content_Failure_t));
    
    memset(&ESC5_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&ESC2_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&ESC12_10_Message_Content, 0, sizeof(Content_Failure_t));
    
    memset(&TCU5_10_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&TCU6_20_Message_Content, 0, sizeof(Content_Failure_t));
    
    memset(&MBFM1_100_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&MBFM5_100_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&MBFM9_500_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&MBFM_PAS1_50_Message_Content, 0, sizeof(Content_Failure_t));
    
    memset(&SRS1_20_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&PKE_ICU2_100_Message_Content, 0, sizeof(Content_Failure_t));
#ifdef SPRINT4
    /* TDMS SAS1_10 */
	memset(&SAS1_10_Message_Content, 0, sizeof(Content_Failure_t));
#endif    
	memset(&RPAS1_20_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&RPAS2_100_Message_Content, 0, sizeof(Content_Failure_t));
    
    memset(&IS1_100_Message_Content, 0, sizeof(Content_Failure_t));
    memset(&IS3_500_Message_Content, 0, sizeof(Content_Failure_t));
    
    memset(&EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS1_10_Temp_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS1_10_RPM_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS1_10_GEAR_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS1_10_INDC_CRUISE_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS3_10_CLUCH_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS3_10_STS_ESS_INDC_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS8_10_TARGET_GEAR_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS4_20_ENG_DRIVE_MODE_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&TCU5_10_GEAR_ACTUAL_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&TCU5_10_GEAR_TARGET_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&TCU6_20_TGS_LEVER_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&TCU6_20_MODE_SIGNAL_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&TCU6_20_INDC_AT_MALFUNC_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&SRS1_20_INDC_SRSC_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&EMS6_500_DISP_AMBT_TEMP_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&MBFM1_100_MBFM_ENGOFF_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&RPAS1_20_DISP_DIST_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&ESC2_10_ABS_VEH_SPEED_Sig_Content, 0, sizeof(Content_Failure_t));
    memset(&ESC2_10_ABS_ODO_DIST_Sig_Content, 0, sizeof(Content_Failure_t));
#ifdef SPRINT4
	/* TDMS SAS1_10 */
	memset(&SAS1_10_ABOLUTE_ANGLE_Sig_Content, 0, sizeof(Content_Failure_t));
#endif	
    
    Load_NM_Configuration();
}
