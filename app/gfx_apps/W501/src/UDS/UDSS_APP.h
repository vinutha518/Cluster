/*
 * UDSS_APP.h
 *
 *  Created on: Jan 14, 2019
 *      Author: MSWELT12
 */

#ifndef UDSS_APP_H_
#define UDSS_APP_H_

/* **********************************************************************************************************************
File            :       UDSS_APP.h
Author          :       Akshay Wankhade
Model           :       Mahindra W501
Revision Author :
Reviewed By     :
Company         :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune
/* **********************************************************************************************************************/

/***************************DEFINATIONS********************************************/
//#define VARIANT_CODE_ARR_SIZE 9
//#define FUEL_LEVEL_ARR[];
//#define FEATURE_CODE_ARR[9];
 void Write_Default_Data(void);
 PUBLIC void Write_VAR_FEATURE_Code_Default_Data(void);
 PUBLIC void Write_FF_TT_BAR_Default_Data(void);
 void Write_FF_TT_Tell_Tale_Default_Data(void);
 void read_BOOT_SOFTWARE_IDENTIFICATION(uint8_t *SOFTWARE_IDENTIFICATION);
 PRIVATE void read_GRAPHIC_VERSION(uint8_t *GRAPHIC_VERSION);
 PRIVATE void read_BOOT_FLAG(uint8_t *BOOT_FLAG);
 PRIVATE void read_ACTIVE_DIAG_SESSION(uint8_t *ACTIVE_DIAG_SESSION);
 PRIVATE void read_SYSTEM_SUPPLIER_PART_NUMBER(uint8_t *SYSTEM_SUPPLIER_PART_NUMBER);
 PRIVATE void read_ECU_SOFTWARE_VERSION_NUMBER(uint8_t *ECU_SOFTWARE_VERSION_NUMBER);
 PRIVATE void read_ECU_SOFTWARE_NUMBER(uint8_t *ECU_SOFTWARE_NUMBER);
 PRIVATE void read_ECU_HARDWARE_VERSION_NUMBER(uint8_t *ECU_HARDWARE_VERSION_NUMBER);
 PRIVATE void read_ECU_HARDWARE_NUMBER(uint8_t *ECU_HARDWARE_NUMBER);
 PRIVATE void read_ECU_SERIAL_NUMBER(uint8_t *ECU_SERIAL_NUMBER);
 PRIVATE void read_ECU_MANUFACTURING_DATE(uint8_t *ECU_MANUFACTURING_DATE);
 PRIVATE void read_Supplier_identification_arr(uint8_t *SUPPLIER_IDENTIFICATION);
 PRIVATE void read_DATA_IDENTIFICATION(uint8_t *DATA_IDENTIFICATION);
 PRIVATE void read_APP_SOFTWARE_IDENTIFICATION(uint8_t *APP_SOFTWARE_IDENTIFICATION);
 PRIVATE void read_SPAREPART_NUMBER(uint8_t *SPAREPART_NUMBER);
 extern void read_feature_code(uint8_t *feature_code);
 extern void read_varient_code(uint8_t *varient_code);

extern void Write_UDS_Data(void);
extern void Read_UDS_EEPROM_Data(void);
extern void write_history_zone(uint8_t *history_data);
//extern void write_VMS1(uint8_t *VMS1_data);
extern void read_history_zone(uint8_t *history_data); //,uint8_t history_zone_no);
extern _Bool REQUEST_3SEC;
extern _Bool REQUEST_5SEC;


#define VARIANT_CODE_ARR_SIZE  						20
#define NS_VARIANT_CODE_ARR_SIZE  					20
#define MODEL_NUMBER_ARR_SIZE 						18
#define SYSTEM_SUPPLIER_PART_NUMBER_ARR_SIZE 		13
#define ECU_INSTALLATION_DATE_ARR_SIZE 				6
#define PROGRAMMING_DATE_ARR_SIZE   				6
#define TESTER_SERIAL_NUMBER_ARR_SIZE  				16
#define ENGINE_TYPE_ARR_SIZE 						16
#define ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE 		8
#define ECU_SOFTWARE_NUMBER_ARR_SIZE 				8
#define ECU_HARDWARE_VERSION_NUMBER_ARR_SIZE  		4
#define ECU_HARDWARE_NUMBER_ARR_SIZE  				4
#define VIN_RW_ARR_SIZE 							17
#define ECU_SERIAL_NUMBER_ARR_SIZE  				8
#define MANUFACTURING_DATE_RW_ARR_SIZE 				6
#define SUPPLIER_IDENTIFICATION_ARR_SIZE 			16
#define SPAREPART_NUMBER_ARR_SIZE 					16
#define APP_DATA_IDENTIFICATION_ARR_SIZE 			8
#define BOOT_SOFTWARE_IDENTIFICATION_ARR_SIZE 		8
#define DID_APP_SOFTWARE_IDENTIFICATION_SIZE 		8
#define GRAPHICS_VERSION_SIZE						8

#define FEATURE_CODE_SIZE 				20 // add 20 byte data // 3 changes for W501
#define FUEL_LEVEL_SIZE  		 		7
#define VMS_1_RW_ARR_SIZE         		8
#define VMS_2_RW_ARR_SIZE         		6
#define BOOT_FLAG_ARR_SIZE        		1  //7 changes for W501
#define ACTIVE_DIAG_SESSION_SIZE        1 // 7 changes for W501
#define ENGINE_RPM_SIZE            	 	7
#define DEF_BAR_NUM_ARR_SIZE       	 	7
#define DEF_LEVEL_PER_ARR_SIZE   	 	7
#define BATTERY_VOLTAGE_ARR_SIZE	 	7

#define HISTORY_ZONE_RW_ARRAY_SIZE	 		25U
#define VIN_ZONE_RW_ARRAY_SIZE	         	17U
#define TESTER_SR_NUMBER_RW_ARRAY_SIZE   	16U
#define FEATURE_CODE_RW_ARRAY_SIZE	 		20  //3U  add 20byte data
#define VARIENT_CODE_RW_ARRAY_SIZE       	20 //5U  add 20 byte
#define ECU_SR_NUMBER_RW_ARRAY_SIZE	 		8U

/********************************* W501 ECUIdentifier array size *****************************/
#define GRAPHIC_VERSION_ARRY_SIZE    		 9U  /* W501 standard DID graphic version */

#define ECUIdentifier_0x0503_ARRY_SIZE       10U
#define ECUIdentifier_0x0504_ARRY_SIZE       15U/*check size of array*/
#define ECUIdentifier_0x0507_ARRY_SIZE       7U
#define ECUIdentifier_0x0508_ARRY_SIZE       1U
#define ECUIdentifier_0x0509_ARRY_SIZE       1U
#define ECUIdentifier_0x0510_ARRY_SIZE       5U
#define ECUIdentifier_0x0511_ARRY_SIZE       2U
#define ECUIdentifier_0x0520_ARRY_SIZE       5U
#define ECUIdentifier_0x0521_ARRY_SIZE       8U
#define ECUIdentifier_0x0522_ARRY_SIZE       8U
#define ECUIdentifier_0x0523_ARRY_SIZE       10U
#define ECUIdentifier_0x0524_ARRY_SIZE       10U
#define ECUIdentifier_0x0525_ARRY_SIZE       9U
#define ECUIdentifier_0x050B_ARRY_SIZE       16U
#define ECUIdentifier_0x050C_ARRY_SIZE       16U
#define ECUIdentifier_0x050D_ARRY_SIZE       4U
#define ECUIdentifier_0x050E_ARRY_SIZE       4U
#define SKC_0x0108_arr_ARRY_SIZE			 4U
#define SKC1_0x0109_arr_ARRY_SIZE			 4U
#define SKC2_0x010A_arr_ARRY_SIZE			 4U
#define NSC_0x010B_arr_ARRY_SIZE			11U
#define NSC_0x0104_arr_ARRY_SIZE			5U
#define NSC_0x0105_arr_ARRY_SIZE			30U
#define NSC_0x0103_arr_ARRY_SIZE			18U
#define NSC_0x0106_ARRY_SIZE                 8U
#define NSC_0x010E_arr_ARRY_SIZE                 2U
#define CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE 17U
#define ECUIdentifier_0x050F_ARRY_SIZE       9U
#define ECUIdentifier_0x04CF_ARRY_SIZE       26U
#define ECUIdentifier_0x04D0_ARRY_SIZE       6U
#define ECUIdentifier_0x04D1_ARRY_SIZE       3U
#define ECUIdentifier_0x04D2_ARRY_SIZE       8U
#define ECUIdentifier_0x04D5_ARRY_SIZE       1U
#define ECUIdentifier_0x04F3_ARRY_SIZE       1U
#define ECUIdentifier_0x04F4_ARRY_SIZE       1U
#define ECUIdentifier_0x04F6_ARRY_SIZE       1U
#define ECUIdentifier_0x04F7_ARRY_SIZE       7U
#define ECUIdentifier_0x04F8_ARRY_SIZE       1U
#define ECUIdentifier_0x04F9_ARRY_SIZE       2U
#define ECUIdentifier_0xF07F_ARRY_SIZE       1U
#define ECUIdentifier_0xF080_ARRY_SIZE       1U
#define ECUIdentifier_0xF081_ARRY_SIZE       1U
#define ECUIdentifier_0xF08E_ARRY_SIZE       1U
#define ECUIdentifier_0xF08F_ARRY_SIZE       1U
#define ECUIdentifier_0xF082_ARRY_SIZE       4U
#define ECUIdentifier_0xF084_ARRY_SIZE       6U
#define ECUIdentifier_0xF083_ARRY_SIZE       1U
#define ECUIdentifier_0xF085_ARRY_SIZE       2U
#define ECUIdentifier_0xF096_ARRY_SIZE       1U
#define ECUIdentifier_0xF1B0_ARRY_SIZE       3U
#define ECUIdentifier_0xF1B1_ARRY_SIZE       2U
#define ECUIdentifier_0xF1B2_ARRY_SIZE       2U
#define ECUIdentifier_0xF1B3_ARRY_SIZE       2U
#define ECUIdentifier_0xF1B4_ARRY_SIZE       5U
#define ECUIdentifier_0xF1B5_ARRY_SIZE       2U
#define ECUIdentifier_0xF1B6_ARRY_SIZE       2U
#define ECUIdentifier_0xF1B7_ARRY_SIZE       8U
//ETM
#define ECUIdentifier_0x0500_ARRY_SIZE       1U

//#define ECUIdentifier_0x0519_ARRY_SIZE       1U
#define ECUIdentifier_0x0502_ARRY_SIZE       4U
#define ECUIdentifier_0x04FB_ARRY_SIZE       4U
#define ECUIdentifier_0x04FC_ARRY_SIZE       4U
#define ECUIdentifier_0x04FD_ARRY_SIZE       4U
#define ECUIdentifier_0x04FE_ARRY_SIZE       4U
#define ECUIdentifier_0x0533_ARRY_SIZE		 2U
#define ECUIdentifier_0x0543_ARRY_SIZE		 2U

//#define ECUIdentifier_0x04C9_ARRY_SIZE       3U
//#define ECUIdentifier_0x04CA_ARRY_SIZE       2U
//#define ECUIdentifier_0x04CB_ARRY_SIZE       1U
//#define ECUIdentifier_0x04CC_ARRY_SIZE       2U
//#define ECUIdentifier_0x04FA_ARRY_SIZE       1U
//#define ECUIdentifier_0x050A_ARRY_SIZE       1U

/*****************Flags Defination********************************/
struct FLAGS {

_Bool F_Update_history_zone:1;
_Bool F_Varient_code:1;
_Bool F_NS_Varient_code:1;
_Bool F_Model_number:1;
_Bool F_Ecu_installation_date:1;
_Bool F_NS_NSConfiguration_0x010B:1;
_Bool F_System_supplier_part_number:1;
_Bool F_Programming_date:1;
_Bool F_tester_serial_number:1;
_Bool F_Engine_type:1;
_Bool F_Ecu_software_version_number:1;
_Bool F_Ecu_software_number:1;
_Bool F_Ecu_hardware_version_number:1;

_Bool F_VIN:1;
_Bool F_Ecu_serial_number:1;
_Bool F_Manufacturing_date:1;
_Bool F_Supplier_identification:1;
_Bool F_Sparepart_number:1;
_Bool F_App_data_identification:1;
_Bool F_Boot_software_identification:1;
_Bool F_DID_ENGINE_RUN_HOUR:1;
_Bool F_FEATURE_CODE:1;
_Bool F_VMS_1_RW:1;
_Bool F_VMS_2_RW:1;
_Bool F_ECU_MAN_DATE:1;


/****************************** W501 ECU Identifier**********************************/
_Bool F_CHIME_ACTIVATION_0x0508:1;
_Bool F_CHIME_ACTIVATION_0x0509:1;
_Bool F_AVG_SPEED:1;
_Bool F_GEAR_STATUS:1;
_Bool F_RPAS_MSG_STAYBACK_TIME:1;
_Bool F_FUEL_DEAD_VOLUME:1;
_Bool F_NVM_SERVICE_DONE:1;
_Bool F_ODOMETER_RAW:1;
_Bool F_ODOMETER_PERCENTAGE:1;
_Bool F_TELL_TALE:1;
_Bool F_IVN_TEST_MSG_CONFIG:1;
_Bool F_SAFETY:1;
_Bool F_BAR_GRAPH_STATUS:1;
//ETM
_Bool F_ETM_CONFIG:1;

/********Multiframe DID************/
_Bool F_CHIMES_CONFIG:1;
_Bool F_MESSAGE_CONFIG:1;
_Bool F_SEAT_BELT_REMAINDER:1;
_Bool F_TIRE_DIRE_MNGT:1;
_Bool F_IFE_ECUIdentifier_0x0520:1;
_Bool F_OSEK_NM:1;
_Bool F_AFE_ECUIdentifier_0x0525:1;
_Bool F_FUEL_BAR_VALUE_CONFIG:1;
_Bool F_TEMP_BAR_VALUE_CONFIG:1;
_Bool F_FUEL_TT_VALUE_CONFIG:1;
_Bool F_TEMP_TT_VALUE_CONFIG:1;
_Bool F_NS_SKC_0108_CONFIG:1;
_Bool F_NS_SKC1_0109_CONFIG:1;
_Bool F_NS_SKC2_010A_CONFIG:1;
_Bool F_SERVICE_DIST_SAP_CONFIG:1;
_Bool F_TELL_TALE_BLINK_PAT3:1;
_Bool F_TELL_TALE_BLINK_PAT4:1;
_Bool F_TELL_TALE_BLINK_PAT5:1;
_Bool F_TELL_TALE_BLINK_PAT8:1;
_Bool F_TELL_TALE_BLINK_PAT9:1;
_Bool F_DTE_AFE_ECUIdentifier_0x04D2:1;
_Bool F_DTE_AFE_OVERSPEED:1;
_Bool F_TELL_TALE_STATUS_SET1:1;
_Bool F_TELL_TALE_STATUS_SET2:1;
_Bool F_TELL_TALE_STATUS_SET3:1;
_Bool F_TELL_TALE_STATUS_SET4:1;
_Bool F_TELL_TALE_STATUS_SET5:1;
_Bool F_GAUGES_CONFIG_0xF082:1;
_Bool F_ODOMETER_CONFIG_0xF1B0:1;
_Bool F_ROLL_PITCH_DET_0x0533:1;
_Bool F_NS_NW_Mgmt_Config_0x0103:1;
_Bool F_NS_CAN_WakeUp_Config_0x0104:1;
_Bool F_NS_CAN_WU_Config_PKE_0x010D:1;
_Bool F_NS_Network_MSG_Config_0x010E:1;
_Bool F_DID_AT_GEAR_ALERT_0x0543:1;
};

/*******************************EXTERN DEFINATIONS***********************************/
//
extern uint8_t Varient_code_arr[VARIANT_CODE_ARR_SIZE];
extern uint8_t NS_Varient_code_arr[VARIANT_CODE_ARR_SIZE];
extern uint8_t Model_number_arr[MODEL_NUMBER_ARR_SIZE];
extern uint8_t Ecu_installation_date_arr[ECU_INSTALLATION_DATE_ARR_SIZE];
extern uint8_t System_supplier_part_number_arr[SYSTEM_SUPPLIER_PART_NUMBER_ARR_SIZE];
extern uint8_t Programming_date_arr[PROGRAMMING_DATE_ARR_SIZE];
extern uint8_t tester_serial_number_arr[TESTER_SERIAL_NUMBER_ARR_SIZE];
extern uint8_t Engine_type_arr[ENGINE_TYPE_ARR_SIZE];
extern uint8_t Ecu_software_version_number_arr[ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE];
extern uint8_t Ecu_software_number_arr[ECU_SOFTWARE_NUMBER_ARR_SIZE];
extern uint8_t Ecu_hardware_version_number_arr[ECU_HARDWARE_VERSION_NUMBER_ARR_SIZE];
extern uint8_t Ecu_hardware_number_arr[ECU_HARDWARE_NUMBER_ARR_SIZE];
extern uint8_t VIN_arr[VIN_RW_ARR_SIZE];
extern uint8_t Ecu_serial_number_arr[ECU_SERIAL_NUMBER_ARR_SIZE];
extern uint8_t Manufacturing_date_arr[MANUFACTURING_DATE_RW_ARR_SIZE];
extern uint8_t Supplier_identification_arr[SUPPLIER_IDENTIFICATION_ARR_SIZE];
extern uint8_t Sparepart_number_arr[SPAREPART_NUMBER_ARR_SIZE];
extern uint8_t App_data_identification_arr[APP_DATA_IDENTIFICATION_ARR_SIZE];
extern uint8_t Boot_software_identification_arr[BOOT_SOFTWARE_IDENTIFICATION_ARR_SIZE];
extern uint8_t App_software_Identification[DID_APP_SOFTWARE_IDENTIFICATION_SIZE];
extern uint8_t Feature_code_arr[FEATURE_CODE_SIZE];
extern uint8_t Fuel_level_arr[FUEL_LEVEL_SIZE];
extern uint8_t Graphics_version[GRAPHICS_VERSION_SIZE];
extern uint8_t Graphics_version_read[GRAPHICS_VERSION_SIZE];
extern uint8_t System_Supplier_Part_No[13];

extern uint8_t Vms_1_RW_arr[VMS_1_RW_ARR_SIZE];
extern uint8_t Vms_2_RW_arr[VMS_2_RW_ARR_SIZE];
extern uint8_t Boot_flag_arr[BOOT_FLAG_ARR_SIZE];
extern uint8_t Active_diag_sesion_arr[ACTIVE_DIAG_SESSION_SIZE];
extern uint8_t Engine_rpm_arr[ENGINE_RPM_SIZE];
extern uint8_t Def_bar_num_arr[DEF_BAR_NUM_ARR_SIZE];

extern uint8_t Def_level_per_arr[DEF_LEVEL_PER_ARR_SIZE];
extern uint8_t Battery_vtg_arr[BATTERY_VOLTAGE_ARR_SIZE];
/******************Network message configuration********************/

extern uint8_t Msg_enable_IC4_1000_Test;
extern uint8_t	Msg_enable_IC6_1000_Test;
extern uint8_t	Msg_enable_IC7_1000_Test;
extern uint8_t	Msg_enable_IC_NMV_Test;

/*******************************************************************/

/*****************TT blinking configurable pattern********************/
extern uint16_t Diag_RW_ESC_ON_Time;
extern uint16_t Diag_RW_ESC_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN4_Immo_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN4_Immo_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN5_Immo_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN5_Immo_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN8_TPMS_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN8_TPMS_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN9_TPMS_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN9_TPMS_OFF_Time;
/*******************************************************************/
/**********************************************************************/

extern uint8_t Diag_RW_Speed_Min_for_Chime_activation;

extern uint32_t Diag_RW_AVS_Initial_Sample_Distance_Config;
extern uint8_t	 Diag_RW_AVS_SAMPLE_TIME;

extern uint8_t Diag_RW_Level1_Threshold_Angle;
extern uint8_t Diag_RW_Level2_Threshold_Angle;
extern uint8_t Diag_RW_Level3_Threshold_Angle;
extern uint8_t Diag_RW_Level4_Threshold_Angle;
extern uint8_t Diag_RW_Level5_Threshold_Angle;

extern uint8_t Diag_RW_Trajectory_Calibration;

extern uint8_t Diag_RW_Temp_Bar1_ON_value;
extern uint8_t	Diag_RW_Temp_Bar2_ON_value;
extern uint8_t	Diag_RW_Temp_Bar3_ON_value;
extern uint8_t	Diag_RW_Temp_Bar4_ON_value;
extern uint8_t	Diag_RW_Temp_Bar5_ON_value;
extern uint8_t	Diag_RW_Temp_Bar6_ON_value;
extern uint8_t	Diag_RW_Temp_Bar7_ON_value;
extern uint8_t	Diag_RW_Temp_Bar8_ON_value;
extern uint8_t	Diag_RW_Temp_Bar1_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar2_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar3_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar4_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar5_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar6_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar7_OFF_value;
extern uint8_t	Diag_RW_Temp_Bar8_OFF_value;


extern uint8_t Diag_RW_Fuel_Bar1_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar2_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar3_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar4_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar5_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar6_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar7_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar8_ON_value;
extern uint8_t	Diag_RW_Fuel_Bar1_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar2_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar3_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar4_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar5_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar6_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar7_OFF_value;
extern uint8_t	Diag_RW_Fuel_Bar8_OFF_value;

extern uint8_t	Diag_RW_Fuel_TT_Continuous_OFF_Value;
extern uint8_t	Diag_RW_Fuel_TT_Continuous_ON_Value;
extern uint8_t	Diag_RW_Fuel_TT_Blink_OFF_Value;
extern uint8_t	Diag_RW_Fuel_TT_Blink_ON_Value;

extern uint8_t	Diag_RW_Temp_TT_Continuous_OFF_Value;
extern uint8_t	Diag_RW_Temp_TT_Continuous_ON_Value;
extern uint8_t	Diag_RW_Temp_TT_Blink_OFF_Value;
extern uint8_t	Diag_RW_Temp_TT_Blink_ON_Value;

//extern uint16_t Diag_RW_ESC_ON_Time;
//uint16_t Diag_RW_ESC_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN4_Immo_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN4_Immo_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN5_Immo_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN5_Immo_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN8_TPMS_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN8_TPMS_OFF_Time;

extern uint16_t Diag_RW_TT_PATTERN9_TPMS_ON_Time;
extern uint16_t Diag_RW_TT_PATTERN9_TPMS_OFF_Time;

extern uint16_t Diag_RW_Fuel_Dead_Volume;
extern uint8_t Diag_RW_NVM_SERV_DONE;
extern uint8_t Diag_RW_ODO_RAW_DISPLAYABLE;
extern uint8_t Diag_RW_ODO_DISPLAYABEL_PERCENTAGE;
extern uint8_t Diag_RW_TPMS_Blink_Total_Duration_Setting;

extern uint8_t Diag_RW_Level1_Speed;
extern uint8_t	Diag_RW_Level2_Speed;
extern uint8_t	Diag_RW_Level1_Chime_Type;
extern uint8_t	Diag_RW_Level1_Chime_Repetition_Interval;
extern uint8_t	Diag_RW_Level2_Chime_Type;
extern uint8_t	Diag_RW_Level2_Chime_Repetition_Interval;
extern uint8_t	Diag_RW_Chime_Hysterisis;
extern uint32_t Diag_RW_Odometer;
extern uint8_t Diag_RW_Engineering_Test_Mode;

extern uint8_t Diag_RW_Role_Angle;
extern uint8_t Diag_RW_Role_Pitch;

extern uint16_t Diag_AT_GEAR_ALERT_Duration;
extern uint16_t Diag_AT_GEAR_ALERT_Debounce;

/*temp variable purpose is final variable need to replace */
extern uint16_t Diag_RW_Displayed_Value;
extern uint16_t Diag_RW_Calculated_Value;
extern uint32_t Diag_RW_Accumulated_Fuel; 
extern uint16_t Diag_RW_Accumulated_Distance;

extern uint16_t Diag_RW_AFE_GD_Displayed_Value;
extern uint16_t Diag_RW_AFE_GD_Calculated_Value;
extern uint32_t Diag_RW_AFE_GD_Accumulated_Fuel; 
extern uint16_t Diag_RW_AFE_GD_Accumulated_Distance;

extern uint16_t Diag_RW_TT_FB_ADC_Park_Brake_TT;
extern uint16_t Diag_RW_TT_FB_ADC_Check_Engine_TT;
extern uint16_t Diag_RW_TT_FB_ADC_MIL_Lamp_TT; 
extern uint16_t Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT;
extern uint16_t Diag_RW_TT_FB_ADC_ABS_TT_Failure;
extern uint16_t Diag_RW_TT_FB_ADC_Cruise_TT;
extern uint16_t Diag_RW_TT_FB_ADC_Airbag_TT; 
extern uint16_t Diag_RW_TT_FB_ADC_Fuel_Delivery_Module;
extern uint16_t Diag_RW_TT_FB_ADC_AT_Fail_TT;
extern uint16_t Diag_RW_TT_FB_ADC_ESP_Malfunction_TT;
extern uint16_t Diag_RW_TT_FB_ADC_ESC_OFF; 
extern uint16_t Diag_RW_TT_FB_ADC_HDC_Malfunction;

extern uint8_t  Diag_RW_DTE_RAFE;
extern uint16_t Diag_RW_DTE_RAFE_Accumulated_Fuel;
extern uint8_t  Diag_RW_DTE_RAFE_Accumulated_Distance;
extern uint16_t Diag_RW_DTE;

extern uint16_t Diag_RW_Fuel_consumption_accumulated;

extern uint8_t Diag_RW_TT_SET1_Check_Engine;
extern uint8_t Diag_RW_TT_SET1_Glow_Plug;
extern uint8_t Diag_RW_TT_SET1_Oil_Pressure_Low; 
extern uint8_t Diag_RW_TT_SET1_4WD_High;
extern uint8_t Diag_RW_TT_SET1_Door_Open;
extern uint8_t Diag_RW_TT_SET1_Seat_Belt;
extern uint8_t Diag_RW_TT_SET1_Immo; 
extern uint8_t Diag_RW_TT_SET1_Park_Brake;

extern uint8_t Diag_RW_TT_SET2_Airbag;
extern uint8_t Diag_RW_TT_SET2_Cruise;
extern uint8_t Diag_RW_TT_SET2_AT_Fail; 
extern uint8_t Diag_RW_TT_SET2_TPMS;
extern uint8_t Diag_RW_TT_SET2_ABS;
extern uint8_t Diag_RW_TT_SET2_Water_in_Fuel;
extern uint8_t Diag_RW_TT_SET2_ESS; 
extern uint8_t Diag_RW_TT_SET2_MIL_Lamp;

extern uint8_t Diag_RW_TT_SET3_ESC_Off;
extern uint8_t Diag_RW_TT_SET3_Park_Lamp;
extern uint8_t Diag_RW_TT_SET3_HHC_Malfunction; 
extern uint8_t Diag_RW_TT_SET3_HDC_Malfunction;
extern uint8_t Diag_RW_TT_SET3_ESC_Malfunction;
extern uint8_t Diag_RW_TT_SET3_Battery_Charge;
extern uint8_t Diag_RW_TT_SET3_Low_Fuel; 
extern uint8_t Diag_RW_TT_SET3_High_Temp;

extern uint8_t Diag_RW_TT_SET5_EPS_Malfunction;
extern uint8_t Diag_RW_TT_SET5_Overspeed;
extern uint8_t Diag_RW_TT_SET5_Reserved;

extern uint16_t Diag_RW_Speedometer_status;
extern uint16_t Diag_RW_Tachometer_status;
extern uint16_t Diag_RW_Tachometer_Redline;

extern uint8_t Diag_RW_LCD_Illumination_Status;
extern uint8_t Diag_RW_Dial_Illumination_Status;
extern uint8_t Diag_RW_Pointer_Illumination_Status;

extern uint8_t Diag_RW_Temperature_bar;
extern uint8_t Diag_RW_Fuel_Bar;

extern uint8_t Diag_RW_Major_Gear;
extern uint8_t Diag_RW_Minor_Gear;
extern uint8_t Diag_RW_Up_Indication_Gear;
extern uint8_t Diag_RW_Down_Indication_Gear;

extern uint8_t Diag_RW_Diesel_Particulate_Filter;
extern uint8_t Diag_RW_Diesel_Exhaust_system_Fluid;
extern uint8_t Diag_RW_4WD_Low;
extern uint8_t Diag_RW_Regeneration_Tell_Tale;

extern float32_t Diag_RW_Battery_Voltage;
extern uint32_t Diag_RW_Vehicle_Speed;
extern uint16_t Diag_RW_Engine_Speed;
extern uint16_t Diag_RW_Ignition_Counter;
extern uint16_t Diag_RW_Reset_Counter;
extern uint32_t Diag_RW_EEPROM_Layout_Version;
extern uint8_t Diag_RW_TT_SET4_ECO;
extern uint8_t Diag_RW_TT_SET4_High_Beam;
extern uint8_t Diag_RW_TT_SET4_Turn_Left;
extern uint8_t Diag_RW_TT_SET4_Turn_Right;
extern uint8_t Diag_RW_TT_SET4_Front_Fog;
extern uint8_t Diag_RW_TT_SET4_Rear_Fog;
extern uint8_t Diag_RW_External_reset_signal;
extern uint8_t Diag_RW_Watch_dog_reset;
extern uint8_t Diag_RW_PowerOn_Clear_Resets;


extern uint16_t Diag_RW_Fuel_level_raw;
extern uint16_t Diag_RW_Current_Gear_shift;

extern uint16_t IVN_cfg_Message_ID1;
extern uint8_t IVN_cfg_Occurance_Count_Msg1;
extern uint8_t IVN_cfg_NS_Status_Msg1; 
extern uint16_t IVN_cfg_Message_ID2;
extern uint8_t IVN_cfg_Occurance_Count_Msg2;
extern uint8_t IVN_cfg_NS_Status_Msg2; 
extern uint16_t IVN_cfg_Message_ID3;
extern uint8_t IVN_cfg_Occurance_Count_Msg3;
extern uint8_t IVN_cfg_NS_Status_Msg3;
extern uint16_t IVN_cfg_Message_ID4;
extern uint8_t IVN_cfg_Occurance_Count_Msg4;
extern uint8_t IVN_cfg_NS_Status_Msg4;
extern uint16_t IVN_cfg_Message_ID5;
extern uint8_t IVN_cfg_Occurance_Count_Msg5;
extern uint8_t IVN_cfg_NS_Status_Msg5;
extern uint16_t IVN_cfg_Message_ID6;
extern uint8_t IVN_cfg_Occurance_Count_Msg6;
extern uint8_t IVN_cfg_NS_Status_Msg6;
extern uint16_t IVN_cfg_Message_ID7;
extern uint8_t IVN_cfg_Occurance_Count_Msg7;
extern uint8_t IVN_cfg_NS_Status_Msg7;
extern uint16_t IVN_cfg_Message_ID8;
extern uint8_t IVN_cfg_Occurance_Count_Msg8;
extern uint8_t IVN_cfg_NS_Status_Msg8;
extern uint16_t IVN_cfg_Message_ID9;
extern uint8_t IVN_cfg_Occurance_Count_Msg9;
extern uint8_t IVN_cfg_NS_Status_Msg9;
extern uint16_t IVN_cfg_Message_ID10;
extern uint8_t IVN_cfg_Occurance_Count_Msg10;
extern uint8_t IVN_cfg_NS_Status_Msg10;
   
extern uint8_t  IVN_Cfg_T_MBFM_PKE_COMMAND_TIME;
extern uint32_t IVN_Cfg_T_WAKEUP_WAIT_Time;
extern uint32_t IVN_Cfg_T_WAKEUP_SYNC_Time;
extern uint32_t IVN_Cfg_T_SLEEP_WAIT1_Time;
extern uint32_t IVN_Cfg_T_KEEP_NORMAL_MIN_TIMEOUT; 
extern uint32_t IVN_Cfg_T_START_WAKEUPTX_Time;
extern uint32_t IVN_Cfg_T_WAKEUPTX_Time;
extern uint32_t IVN_Cfg_T_START_ACTIVETX_Time;
extern uint32_t IVN_Cfg_T_ACTIVETX_Time;
/**********************************************************************/

/* DID 1004 */
extern uint8_t IVN_cfg_CAN_Wakeup_Msg_Count;   
extern uint16_t	IVN_cfg_Quick_Recovery_timeout_period;
extern uint8_t	IVN_cfg_Quick_Recovery_retry_limit;
extern uint16_t	IVN_cfg_Slow_Recovery_timeout_period;
extern uint8_t	IVN_cfg_Slow_Recovery_retry_limit;
/* DID 0103 */
extern uint8_t  IVN_cfg_NA_Detection_Threshold;   
extern uint8_t	IVN_cfg_NA_Recovery_Threshold;
extern uint8_t	IVN_cfg_Msg_Timeout_Detection_Threshold;
extern uint8_t	IVN_cfg_Msg_Timeout_Recovery_Threshold;
extern uint8_t	IVN_cfg_Msg_Content_Error_Detection_Threshold;
extern uint8_t  IVN_cfg_Msg_Content_Error_Recovery_Threshold;  
extern uint8_t	IVN_cfg_NM_Drive_Cnt_Clear_DTC;
extern uint16_t	IVN_cfg_Busoff_Fast_Recovery_Time;
extern uint8_t	IVN_cfg_Fast_Bus_off_Recovery_Count;
extern uint16_t	IVN_cfg_Busoff_Slow_Recovery_Time;
extern uint16_t  IVN_cfg_NM_IGN_On_Startup_Delay;  
extern uint16_t	IVN_cfg_NM_Restart_Dly_Time_Under_Vol_Recovery;
extern uint16_t	IVN_cfg_NA_NM_Restart_Dly_Time_Over_Vol_Recovery;
extern uint16_t	IVN_cfg_NA_NM_Restart_Dly_Time_Bus_Off_recovery;
extern uint8_t	IVN_cfg_NA_Signal_Content_Fail_Detect_Count;
extern uint8_t	IVN_cfg_NA_Signal_Content_Recov_Detect_Count;
extern uint8_t	IVN_cfg_NA_CAN_Wakeup_Feature_Enable;
extern uint16_t	IVN_cfg_NA_NM_Restart_Dly_Time_Cranking;
/*0106*/
extern uint8_t	IVN_cfg_FC_Not_matching;
extern uint8_t	IVN_cfg_RX_msg_Periodicity;
extern uint8_t	IVN_cfg_TX_msg_Received;
extern uint8_t	IVN_cfg_NSM_Msg_Timout;
extern uint8_t	IVN_cfg_SK2_Not_matching;
extern uint8_t	IVN_cfg_Play_protection;
extern uint8_t	IVN_cfg_Reserved;

/* DID 0107 */
extern uint8_t IVN_cfg_NS_variant_code;   
extern uint8_t	IVN_cfg_NS_EMS;
extern uint8_t	IVN_cfg_NS_ABS;
extern uint8_t	IVN_cfg_NS_ESC;
extern uint8_t	IVN_cfg_NS_SAS;
extern uint8_t IVN_cfg_NS_PKE;  
extern uint8_t	IVN_cfg_NS_ICU;
extern uint8_t	IVN_cfg_NS_SRS;
extern uint8_t	IVN_cfg_NS_TCU;
extern uint8_t	IVN_cfg_NS_IC;
extern uint8_t IVN_cfg_NS_MBFM;  
extern uint8_t	IVN_cfg_NS_Infotainment_System;

/*DID 010E*/

extern uint8_t Msg_enable_IC4_1000_Test;
extern uint8_t	Msg_enable_IC6_1000_Test;
extern uint8_t	Msg_enable_IC7_1000_Test;
extern uint8_t	Msg_enable_IC_NMV_Test;

/* IVN Message timeout and Node absent */
extern Message_Timeout_t EMS3_10_Message;	//NA
extern Message_Timeout_t EMS36_10_Message;
extern Message_Timeout_t EMS1_10_Message;
extern Message_Timeout_t EMS8_10_Message;
extern Message_Timeout_t EMS4_20_Message;
extern Message_Timeout_t EMS29_100_Message;
extern Message_Timeout_t EMS6_500_Message;
extern Message_Timeout_t EMS12_200_Message;
extern Message_Timeout_t EMS5_500_Message;
extern Message_Timeout_t EMS_NSM_Message;	//100ms

extern Message_Timeout_t ESC5_10_Message;	//NA
extern Message_Timeout_t ESC2_10_Message;
extern Message_Timeout_t ESC12_10_Message;
extern Message_Timeout_t ESC_NSM_Message;	//100ms
//#ifdef SPRINT4
/*TDMS */
extern Message_Timeout_t SAS1_10_Message;
//#endif
extern Message_Timeout_t TCU5_10_Message;	//NA
extern Message_Timeout_t TCU6_20_Message;
extern Message_Timeout_t TCU_NSM_Message;	//100ms

extern Message_Timeout_t MBFM1_100_Message;	//NA
extern Message_Timeout_t MBFM5_100_Message;
extern Message_Timeout_t MBFM9_500_Message;
extern Message_Timeout_t MBFM_PAS1_50_Message;

extern Message_Timeout_t SRS1_20_Message;	//NA

extern Message_Timeout_t PKE_ICU2_100_Message;	//NA

extern Message_Timeout_t RPAS1_20_Message;	//NA
extern Message_Timeout_t RPAS2_100_Message;

extern Message_Timeout_t IS1_100_Message;	//NA
extern Message_Timeout_t IS3_500_Message;
extern Message_Timeout_t IS_NSM_Message;	//100ms

#define EMS36_10_message_timeout_flag		(EMS36_10_Message.Message_Timeout_Status | EMS36_10_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS1_10_message_timeout_flag		(EMS1_10_Message.Message_Timeout_Status | EMS1_10_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS3_10_message_timeout_flag		(EMS3_10_Message.Message_Timeout_Status | EMS3_10_Message_Content.Failure_Status | CAN_Bus_Off)
#define EMS8_10_message_timeout_flag		(EMS8_10_Message.Message_Timeout_Status | EMS8_10_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS4_20_message_timeout_flag		(EMS4_20_Message.Message_Timeout_Status | EMS4_20_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS6_500_message_timeout_flag		(EMS6_500_Message.Message_Timeout_Status | EMS6_500_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS12_200_message_timeout_flag		(EMS12_200_Message.Message_Timeout_Status | EMS12_200_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS29_100_message_timeout_flag		(EMS29_100_Message.Message_Timeout_Status | EMS29_100_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS5_500_message_timeout_flag		(EMS5_500_Message.Message_Timeout_Status | EMS5_500_Message_Content.Failure_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define EMS_NSM_message_timeout_flag		(EMS_NSM_Message.Message_Timeout_Status | EMS3_10_Message.Message_Timeout_Status | CAN_Bus_Off)

#define ESC2_10_message_timeout_flag		(ESC2_10_Message.Message_Timeout_Status | ESC2_10_Message_Content.Failure_Status | ESC5_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define ESC5_10_message_timeout_flag		(ESC5_10_Message.Message_Timeout_Status | ESC5_10_Message_Content.Failure_Status | CAN_Bus_Off)
#define ESC12_10_message_timeout_flag		(ESC12_10_Message.Message_Timeout_Status | ESC12_10_Message_Content.Failure_Status | ESC5_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define ESC_NSM_message_timeout_flag		(ESC_NSM_Message.Message_Timeout_Status | ESC5_10_Message.Message_Timeout_Status | CAN_Bus_Off)
//#ifdef SPRINT4
/*TDMS */
#define SAS1_10_message_timeout_flag		(SAS1_10_Message.Message_Timeout_Status | SAS1_10_Message_Content.Failure_Status |  CAN_Bus_Off)
//#endif
#define TCU5_10_message_timeout_flag		(TCU5_10_Message.Message_Timeout_Status | TCU5_10_Message_Content.Failure_Status | CAN_Bus_Off)
#define TCU6_20_message_timeout_flag		(TCU6_20_Message.Message_Timeout_Status | TCU6_20_Message_Content.Failure_Status | TCU5_10_Message.Message_Timeout_Status | CAN_Bus_Off)
#define TCU_NSM_message_timeout_flag		(TCU_NSM_Message.Message_Timeout_Status | TCU5_10_Message.Message_Timeout_Status | CAN_Bus_Off)

#define MBFM5_100_message_timeout_flag		(MBFM5_100_Message.Message_Timeout_Status | MBFM5_100_Message_Content.Failure_Status | MBFM1_100_Message.Message_Timeout_Status | CAN_Bus_Off)
#define MBFM1_100_message_timeout_flag		(MBFM1_100_Message.Message_Timeout_Status | MBFM1_100_Message_Content.Failure_Status | CAN_Bus_Off)
#define MBFM9_500_message_timeout_flag		(MBFM9_500_Message.Message_Timeout_Status | MBFM9_500_Message_Content.Failure_Status | MBFM1_100_Message.Message_Timeout_Status | CAN_Bus_Off)
#define MBFM_PAS1_50_message_timeout_flag	(MBFM_PAS1_50_Message.Message_Timeout_Status | MBFM_PAS1_50_Message_Content.Failure_Status | MBFM1_100_Message.Message_Timeout_Status | CAN_Bus_Off)

#define SRS1_20_message_timeout_flag		(SRS1_20_Message.Message_Timeout_Status | SRS1_20_Message_Content.Failure_Status | CAN_Bus_Off)

#define PKE_ICU2_100_message_timeout_flag	(PKE_ICU2_100_Message.Message_Timeout_Status | PKE_ICU2_100_Message_Content.Failure_Status | CAN_Bus_Off)

#define IS3_500_message_timeout_flag		(IS3_500_Message.Message_Timeout_Status | IS3_500_Message_Content.Failure_Status | IS1_100_Message.Message_Timeout_Status | CAN_Bus_Off)
#define IS1_100_message_timeout_flag		(IS1_100_Message.Message_Timeout_Status | IS1_100_Message_Content.Failure_Status | CAN_Bus_Off)
#define IS_NSM_message_timeout_flag		(IS_NSM_Message.Message_Timeout_Status | IS1_100_Message.Message_Timeout_Status | CAN_Bus_Off)

#define RPAS1_20_message_timeout_flag		(RPAS1_20_Message.Message_Timeout_Status | RPAS1_20_Message_Content.Failure_Status | CAN_Bus_Off)
#define RPAS2_100_message_timeout_flag		(RPAS2_100_Message.Message_Timeout_Status | RPAS2_100_Message_Content.Failure_Status | RPAS1_20_Message.Message_Timeout_Status | CAN_Bus_Off)

/******************RPAS Diagnostics Parameters**********************/

extern uint8_t Diag_RW_RPAS_Display_Screen_source;
extern uint8_t Diag_RW_RPAS_Sector_Zone_Calibration;
extern uint16_t Diag_RW_AT_Reverse_Gear_Debounce_Time;
extern uint16_t Diag_RW_RPASStayBackTimeVal;
extern uint16_t Diag_RW_RPAS_Error_Chime_Enable;
extern uint16_t Diag_RW_RPAS_self_check_chime_Enable;
extern uint8_t Diag_RW_Stop_Dist;
/*******************************************************************/
/****************************** W501 ECU Identifier **********************************************/
extern uint8_t  DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[ECUIdentifier_0x050B_ARRY_SIZE];
extern uint8_t  DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[ECUIdentifier_0x050C_ARRY_SIZE];
extern uint8_t  DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[ECUIdentifier_0x050D_ARRY_SIZE];
extern uint8_t  DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[ECUIdentifier_0x050E_ARRY_SIZE];
extern const uint8_t  DID_FUEL_BAR_DEFAULT_VALUE_0x050B_arr[ECUIdentifier_0x050B_ARRY_SIZE];
extern const uint8_t  DID_TEMP_BAR_DEFAULT_VALUE_0x050C_arr[ECUIdentifier_0x050C_ARRY_SIZE];
extern const uint8_t  DID_FUEL_TT_DEFAULT_VALUE_0x050D_arr[ECUIdentifier_0x050D_ARRY_SIZE];
extern const uint8_t  DID_TEMP_TT_DEFAULT_VALUE_0x050E_arr[ECUIdentifier_0x050E_ARRY_SIZE];



extern uint8_t  DID_CHIMES_CONFIG_0x0503_arr[ECUIdentifier_0x0503_ARRY_SIZE];
extern uint8_t  DID_MESSAGE_CONFIG_0x0504_arr[ECUIdentifier_0x0504_ARRY_SIZE];
extern uint8_t  DID_SEAT_BELT_REMAINDER_0x0507_arr[ECUIdentifier_0x0507_ARRY_SIZE];
extern uint8_t  DID_CHIME_ACTIVATION_0x0508_arr[ECUIdentifier_0x0508_ARRY_SIZE];
extern uint8_t  DID_CHIME_ACTIVATION_0x0509_arr[ECUIdentifier_0x0509_ARRY_SIZE];
extern uint8_t  DID_TIRE_DIRE_MNGT_0x0510_arr[ECUIdentifier_0x0510_ARRY_SIZE];
extern uint8_t  DID_AVG_SPEED_0x0511_arr[ECUIdentifier_0x0511_ARRY_SIZE];
extern uint8_t  DID_IFE_ECUIdentifier_0x0520_arr[ECUIdentifier_0x0520_ARRY_SIZE];
extern uint8_t  DID_OSEK_NM_0x0521_arr[ECUIdentifier_0x0521_ARRY_SIZE];
//extern uint8_t  DID_GEAR_STATUS_0x0519_arr[ECUIdentifier_0x0519_ARRY_SIZE];
extern uint8_t  DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[ECUIdentifier_0x0522_ARRY_SIZE];
extern uint8_t  DID_AFE_ECU_IDENTIFIER_0x0523_arr[ECUIdentifier_0x0523_ARRY_SIZE];
extern uint8_t  DID_AFE_GD_ECUIdentifier_0x0524_arr[ECUIdentifier_0x0524_ARRY_SIZE];
extern uint8_t  DID_AFE_ECUIdentifier_0x0525_arr[ECUIdentifier_0x0525_ARRY_SIZE];
extern uint8_t  DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[ECUIdentifier_0x050F_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[ECUIdentifier_0x04CF_ARRY_SIZE];
extern uint8_t  DID_DTE_ECUIdentifier_0x04D0_arr[ECUIdentifier_0x04D0_ARRY_SIZE];
extern uint8_t  DID_FUEL_GEAN_INDICATOR_0x04D1_arr[ECUIdentifier_0x04D1_ARRY_SIZE];
extern uint8_t  DID_DTE_AFE_ECUIdentifier_0x04D2_arr[ECUIdentifier_0x04D2_ARRY_SIZE];
extern uint8_t  DID_FUEL_DEAD_VOLUME_0x04D5_arr[ECUIdentifier_0x04D5_ARRY_SIZE];
extern uint8_t  DID_NVM_SERVICE_DONE_0x04F3_arr[ECUIdentifier_0x04F3_ARRY_SIZE];
extern uint8_t  DID_ODOMETER_RAW_0x04F4_arr[ECUIdentifier_0x04F4_ARRY_SIZE];
extern uint8_t  DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[ECUIdentifier_0x04F6_ARRY_SIZE];
extern uint8_t  DID_OVERSPEED_0x04F7_arr[ECUIdentifier_0x04F7_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_0x04F8_arr[ECUIdentifier_0x04F8_ARRY_SIZE];
extern uint8_t  DID_AFE_ECUIdentifier_0x04F9_arr[ECUIdentifier_0x04F9_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET1_0xF07F_arr[ECUIdentifier_0xF07F_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET2_0xF080_arr[ECUIdentifier_0xF080_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET3_0xF081_arr[ECUIdentifier_0xF081_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET4_0xF08E_arr[ECUIdentifier_0xF08E_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET5_0xF08F_arr[ECUIdentifier_0xF08F_ARRY_SIZE];
extern uint8_t  DID_GAUGES_0xF082_arr[ECUIdentifier_0xF082_ARRY_SIZE];
extern uint8_t  DID_ILLUMINATION_STATUS_0xF084_arr[ECUIdentifier_0xF084_ARRY_SIZE];
//uint8_t  DID_RESET_REASON_CONFIG_0xF1B4_arr[ECUIdentifier_0xF184_ARRY_SIZE];
extern uint8_t  DID_BAR_GRAPH_STATUS_0xF083_arr[ECUIdentifier_0xF083_ARRY_SIZE];
extern uint8_t  DID_GEAR_INDICATION_STATUS_0xF085_arr[ECUIdentifier_0xF085_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET6_0xF096_arr[ECUIdentifier_0xF096_ARRY_SIZE];
extern uint8_t  DID_ODOMETER_0xF1B0_arr[ECUIdentifier_0xF1B0_ARRY_SIZE];
extern uint8_t  DID_ODOMETER_0xF1B0_read_arr[ECUIdentifier_0xF1B0_ARRY_SIZE];
extern uint8_t  DID_BATTERY_VOLTAGE_0xF1B1_arr[ECUIdentifier_0xF1B1_ARRY_SIZE];
extern uint8_t  DID_VEHICLE_SPEED_0xF1B2_arr[ECUIdentifier_0xF1B2_ARRY_SIZE];
extern uint8_t  DID_ENGINE_SPEED_0xF1B3_arr[ECUIdentifier_0xF1B3_ARRY_SIZE];
extern uint8_t  DID_RESET_REASON_0xF1B4_arr[ECUIdentifier_0xF1B4_ARRY_SIZE];
extern uint8_t  DID_RESET_COUNTER_0xF1B5_arr[ECUIdentifier_0xF1B5_ARRY_SIZE];
extern uint8_t  DID_IGNITION_COUNTER_0xF1B6_arr[ECUIdentifier_0xF1B6_ARRY_SIZE];
extern uint8_t  DID_EEPROM_LAYOUT_0xF1B7_arr[ECUIdentifier_0xF1B7_ARRY_SIZE];
//ETM
extern uint8_t  DID_ETM_0x0500_arr[ECUIdentifier_0x0500_ARRY_SIZE];

extern uint8_t DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[ECUIdentifier_0x0502_ARRY_SIZE];
extern uint8_t DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[ECUIdentifier_0x04FB_ARRY_SIZE];
extern uint8_t DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[ECUIdentifier_0x04FC_ARRY_SIZE];
extern uint8_t DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[ECUIdentifier_0x04FD_ARRY_SIZE];
extern uint8_t DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[ECUIdentifier_0x04FE_ARRY_SIZE];
extern uint8_t DID_Roll_Pitch_Detection_0x0533_arr[ECUIdentifier_0x0533_ARRY_SIZE];
extern uint8_t DID_AT_GEAR_ALERT_0x0543_arr[ECUIdentifier_0x0543_ARRY_SIZE];
extern uint8_t NS_Can_Wakeup_Configuration_0x0104_arr[NSC_0x0104_arr_ARRY_SIZE];
extern uint8_t NS_NM_Configuration_0x0103_arr[NSC_0x0103_arr_ARRY_SIZE];
extern uint8_t NS_Fault_Type_Status_Info_0x0106_arr[NSC_0x0106_ARRY_SIZE];
extern uint8_t NS_Fault_Msg_Details_0x0105_arr[NSC_0x0105_arr_ARRY_SIZE];
extern uint8_t NS_NW_Msg_config_0x010E_arr[NSC_0x010E_arr_ARRY_SIZE];
extern uint8_t NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE];

extern void Load_NM_Configuration(void);
/************************** Function declaration*********************************/
extern void write_CHIME_ACTIVATION_0x0508(uint8_t *CHIME_ACTIVATION_0x0508);			//
extern void write_CHIME_ACTIVATION_0x0509(uint8_t *CHIME_ACTIVATION_0x0509);			//
void write_AVG_SPEED(uint8_t *AVG_SPEED);										//
void write_GEAR_STATUS(uint8_t *GEAR_STATUS);									//
void write_FUEL_DEAD_VOLUME(uint8_t *FUEL_DEAD_VOLUME);							//
void write_NVM_SERVICE_DONE(uint8_t *NVM_SERVICE_DONE);							//
void write_ODOMETER_RAW(uint8_t *ODOMETER_RAW);									//
void write_ODOMETER_PERCENTAGE(uint8_t *ODOMETER_PERCENTAGE);					//
void Write_TELL_TALE_0x04F8(uint8_t *TELL_TALE);								//
void write_ODOMETER_CONFIG_0xF1B0(uint8_t *DID_ODOMETER_0xF1B0);
void write_CHIMES_CONFIG(uint8_t *CHIMES_CONFIG);
void write_F_MESSAGE_CONFIG(uint8_t *MESSAGE_CONFIG);
void write_SEAT_BELT_REMAINDER(uint8_t *SEAT_BELT_REMAINDER);
void write_FUEL_BAR_VALUE_CONFIG(uint8_t *FUEL_BAR_VALUE_CONFIG);
void write_F_TIRE_DIRE_MNGT(uint8_t *TIRE_DIRE_MNGT);
void write_IFE_ECUIdentifier_0x0520(uint8_t *IFE_DID_0x0520);
void write_F_OSEK_NM(uint8_t *OSEK_NM);
void write_RPAS_MSG_STAYBACK_TIME(uint8_t *RPAS_MSG_STAYBACK_TIME);
void write_AFE_ECUIdentifier_0x0525(uint8_t *AFE_DID_0x0525);
void write_SERVICE_DIST_SAP_CONFIG(uint8_t *SERVICE_DIST_SAP_CONFIG);
void write_DTE_AFE_ECUIdentifier_0x04D2(uint8_t *AFE_DID_0x04D2);
void write_DTE_AFE_OVERSPEED(uint8_t *AFE_DID_0x04F7);
void write_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(uint8_t *TT_BLINK_PATTERN3);

void Write_TEMP_BAR_VALUE_CONFIG_DATA(uint8_t *TEMP_BAR_VALUE_CONFIG);
void Write_FUEL_TT_VALUE_CONFIG_DATA(uint8_t *FUEL_TT_VALUE_CONFIG);
void Write_TEMP_TT_VALUE_CONFIG_DATA(uint8_t *TEMP_TT_VALUE_CONFIG);
void Write_TELL_TALE_BLINK_PAT3_DATA(uint8_t *TELL_TALE_BLINK_PAT3);
void Write_TELL_TALE_BLINK_PAT4_DATA(uint8_t *TELL_TALE_BLINK_PAT4);
void Write_TELL_TALE_BLINK_PAT5_DATA(uint8_t *TELL_TALE_BLINK_PAT5);
void Write_TELL_TALE_BLINK_PAT8_DATA(uint8_t *TELL_TALE_BLINK_PAT8);
void Write_TELL_TALE_BLINK_PAT9_DATA(uint8_t *TELL_TALE_BLINK_PAT9);
void Write_TELL_TALE_FEEDBACK_ADC_VALUE_DATA(uint8_t *TELL_TALE_FEEDBACK_ADC_VALUE);
void Write_ODOMETER_0x04C9_DATA(uint8_t *ODOMETER_0x04C9);
void Write_FEUL_GEAN_INDICATOR_DATA(uint8_t *FEUL_GEAN_INDICATOR);
void Write_OVERSPEED_DATA(uint8_t *OVERSPEED);
void Write_IVN_TEST_MSG_CONFIG_DATA(uint8_t *IVN_TEST_MSG);
void Write_SAFETY_DATA(uint8_t *SAFETY_DATA);
//ETM
void Write_ETM_CONFIG(uint8_t *ETM_MODE);

/*********************W501 ECU Identifier read data by Identifier*******************************************/
void read_GRAPHICS_VERSION_DATA(uint8_t *Graphic_Version);  /*W501 standard DID*/

void read_CHIMES_CONFIGURATION_DATA(uint8_t *Chimes_config);
void read_MESSAGE_CONFIGURATION_DATA(uint8_t *MESSAGE_CONFIG);
void read_SEAT_BELT_REMAINDER(void);
void read_CHIME_ACTIVATION_0x0508(void);
void read_CHIME_ACTIVATION_0x0509(void);
void read_TIRE_DIRE_MNGT(uint8_t *TIRE_DIRE_MNGT);
void read_AVG_SPEED(void);

void read_IFE_ECUIdentifier_0x0520(void);
//void read_OSEK_NM_DATA(uint8_t *OSEK_NM);                    NA for W501
void read_GEAR_STATUS_DATA(uint8_t *GEAR_STATUS);
void read_RPAS_MSG_STAYBACK_TIME_DATA(uint8_t *RPAS_MSG_STAYBACK_TIME);
void read_AFE_ECU_IDENTIFIER_0x0523_DATA(void);
void read_AFE_GD_ECUIdentifier_0x0524_DATA(uint8_t *AFE_DID_0x0524);
void read_AFE_ECUIdentifier_0x0525_DATA(uint8_t *AFE_DID_0x0525);
void read_FUEL_BAR_VALUE_CONFIG_DATA(uint8_t *FUEL_BAR_VALUE_CONFIG);
void read_TEMP_BAR_VALUE_CONFIG_DATA(uint8_t *TEMP_BAR_VALUE_CONFIG);
void read_FUEL_TT_VALUE_CONFIG_DATA(uint8_t *FUEL_TT_VALUE_CONFIG);
void read_TEMP_TT_VALUE_CONFIG_DATA(uint8_t *TEMP_TT_VALUE_CONFIG);
void read_SERVICE_DIST_SAP_CONFIG_DATA(uint8_t *SERVICE_DIST_SAP_CONFIG);
void read_TELL_TALE_BLINK_PAT3_DATA(uint8_t *TELL_TALE_BLINK_PAT3);
void read_TELL_TALE_BLINK_PAT4_DATA(uint8_t *TELL_TALE_BLINK_PAT4);
void read_TELL_TALE_BLINK_PAT5_DATA(uint8_t *TELL_TALE_BLINK_PAT5);
void read_TELL_TALE_BLINK_PAT8_DATA(uint8_t *TELL_TALE_BLINK_PAT8);
void read_TELL_TALE_BLINK_PAT9_DATA(uint8_t *TELL_TALE_BLINK_PAT9);
void read_TELL_TALE_FEEDBACK_ADC_VALUE_DATA(uint8_t *TELL_TALE_FEEDBACK_ADC_VALUE);

void read_ODOMETER_0x04C9_DATA(uint8_t *ODOMETER_0x04C9);
void read_ENGINE_SPEED_DATA(uint8_t *ENGINE_SPEED);
void read_BATTERY_VOLTAGE_DATA(uint8_t *BATTERY_VOLTAGE);
void read_VEHICLE_SPEED_DATA(uint8_t *VEHICLE_SPEED);
void read_DTE_ECUIdentifier_0x04D0_DATA(void);
void read_FEUL_GEAN_INDICATOR_DATA(uint8_t *FEUL_GEAN_INDICATOR);
void read_AFE_ECUIdentifier_0x04D2_DATA(void);
void read_NVM_SERVICE_DONE_DATA(uint8_t *NVM_SERVICE_DONE);
void read_ODOMETER_RAW_DATA(uint8_t *ODOMETER_RAW);
void read_ODOMETER_PERCENTAGE_DATA(uint8_t *ODOMETER_PERCENTAGE);
void read_OVERSPEED_DATA(uint8_t *OVERSPEED);
void read_TELL_TALE_0x04F8_DATA(uint8_t *TELL_TALE);
void read_IVN_TEST_MSG_CONFIG_DATA(uint8_t *IVN_TEST_MSG);
void read_SAFETY_DATA(uint8_t *SAFETY_DATA);

//ETM
void read_ETM_CONFIG_DATA(uint8_t *ETM_MODE);

void read_ILLUMINATION_STATUS_DATA(uint8_t *ILLUMINATION_STATUS);
void read_BAR_GRAPH_STATUS_DATA(uint8_t *BAR_GRAPH_STATUS);
void read_GEAR_INDICATION_STATUS_DATA(uint8_t *GEAR_INDICATION_STATUS);
#endif /* UDSS_APP_H_ */
