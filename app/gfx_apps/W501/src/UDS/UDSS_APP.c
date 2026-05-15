/*
 * UDSS_APP.c
 *
 *  Created on: Jan 14, 2019
 *      Author: MSWELT12
 */
/*
 * UDSS_APP.c
 *
 *  Created on: Jan 12, 2019
 *      Author: MSWELT12
 */
/* **********************************************************************************************************************
File                    :       UDSS_APP.c
Author                  :       Akshay Wankhade
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

/***********************************************************************************************************************
Includes
 ********************************************************************************************************************** */


//#include "define.h"
#include "r_typedefs.h"
#include "my_can.h"
#include "UDSS.h"
/*#include <stdlib.h>*/
#include <string.h>
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "Memmory_Map.h"
#include "r_cg_serial_user.h"
#include "fuel.h"
#include "parallel_to_serial.h"
#include "alert.h"
#include "digitalGuage.h"
#include "ADC.h"
//uint16_t eeprom_read(uint16_t Address );// w501

void Write_UDS_Data(void);
void write_history_zone(uint8_t *history_data);
void write_vin_zone(uint8_t *vin_data);
void write_vms1_zone(uint8_t *vms1_data);
void write_vms2_zone(uint8_t *vms2_data);
void write_programming_date_zone(uint8_t *date);
void write_model_number(uint8_t *model_number);
void write_ecu_installation_date(uint8_t *ecu_date);
void write_ecu_manf_date(uint8_t *ecu_manf_date);
void write_engine_name_type(uint8_t *eng_name_type);
void write_tester_serial_number(uint8_t *test_number);
void write_feature_code(uint8_t *feature_code);
void write_varient_code(uint8_t *varient_code);
void write_NS_varient_code(uint8_t *varient_code);
void write_Ecu_serial_number(uint8_t *Ecu_serial_number_code);
void write_Vehicle_Manufacturere_SparePart_Number(uint8_t *Manufacturer_number);
void write_System_Supplier_Identification(uint8_t *System_Supplier_ID);

void write_TEMP_TT_VALUE_CONFIG(uint8_t *TEMP_TT_VALUE_CONFIG);
void write_FUEL_TT_VALUE(uint8_t *FUEL_TT_VALUE);
void write_TEMP_BAR_VALUE(uint8_t *TEMP_BAR_VALUE);
void write_FUEL_BAR_VALUE(uint8_t *FUEL_BAR_VALUE);

void write_NS_Network_Security_key_SKC_0x0108(uint8_t *);
void write_NS_Network_Security_key_SKC1_0x0109(uint8_t *);
void write_NS_Network_Security_key_SKC2_0x010A(uint8_t *);
void write_NS_Network_Security_Configuration_0x010B(uint8_t *Security_key);
void write_NS_CAN_WEKUP_Configuration_PKE_0x010D(uint8_t *Security_key);
void write_NS_NW_Msg_Config_0x010E(uint8_t *Config_Value);
void write_NS_Can_Wakeup_Configuration_0x0104(uint8_t *Config_Value);
void write_NS_NM_Configuration_0x0103(uint8_t *Config_Value);

/**********************W501 ECU Identifier *****************/
void Write_CHIME_ACTIVATION_0x0508(uint8_t *CHIME_ACTIVATION_0x0508);
void Write_CHIME_ACTIVATION_0x0509(uint8_t *CHIME_ACTIVATION_0x0509);
void write_SEAT_BELT_REMAINDER(uint8_t *SEAT_BELT_REMAINDER);
void Write_FUEL_DEAD_VOLUME_DATA(uint8_t *FUEL_DEAD_VOLUME);
void write_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(uint8_t *TT_BLINK_PATTERN3);
void write_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(uint8_t *TT_BLINK_PATTERN4);
void write_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(uint8_t *TT_BLINK_PATTERN5);
void write_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(uint8_t *TT_BLINK_PATTERN8);
void write_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(uint8_t *TT_BLINK_PATTERN9);
void write_Roll_Pitch_Detection_0x0533_DATA(uint8_t *Detection);
void write_AT_GEAR_ALERT_0x0543_DATA(uint8_t *Detection);
/************************************************************/
void read_history_zone(uint8_t *history_data); //,uint8_t history_zone_no);
void Read_UDS_EEPROM_Data(void);
void read_vin_zone(uint8_t *vin_data);
void read_vms1_zone(uint8_t *vms1_data);
//void read_vms2_zone(uint8_t *vms2_data);
void read_programming_date_zone(uint8_t *date);
void read_model_number(uint8_t *model_number);
void read_ecu_installation_date(uint8_t *ecu_date);
void read_ecu_manf_date(uint8_t *ecu_manf_date);
void read_engine_name_type(uint8_t *eng_name_type);
void read_tester_serial_number(uint8_t *test_number);
void read_feature_code(uint8_t *feature_code);
void read_varient_code(uint8_t *varient_code);
void read_NS_varient_code(uint8_t *varient_code);
void read_Ecu_serial_number(uint8_t *Ecu_serial_number_code);

void read_TEMP_TT_VALUE_CONFIG(uint8_t *TEMP_TT_VALUE_CONFIG);
void read_FUEL_TT_VALUE(uint8_t *FUEL_TT_VALUE);
void read_TEMP_BAR_VALUE(uint8_t *TEMP_BAR_VALUE);
void read_FUEL_BAR_VALUE(uint8_t *FUEL_BAR_VALUE);
void read_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(uint8_t *TT_BLINK_PATTERN3);
void read_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(uint8_t *TT_BLINK_PATTERN4);
void read_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(uint8_t *TT_BLINK_PATTERN5);
void read_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(uint8_t *TT_BLINK_PATTERN8);
void read_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(uint8_t *TT_BLINK_PATTERN9);
void read_Roll_Pitch_Detection_0x0533_DATA(void);
//DID//
void read_CHIME_ACTIVATION_0x0508(void);
void read_CHIME_ACTIVATION_0x0509(void);
void read_AVG_SPEED(void);
void read_FUEL_DEAD_VOLUME(void);
void read_NVM_SERVICE_DONE(void);
void read_ODOMETER_RAW(void);
void read_ODOMETER_PERCENTAGE(void);
void read_TELL_TALE_0x04F8(void);
void read_CHIMES_CONFIG(void);
void read_F_MESSAGE_CONFIG(void);
void read_SEAT_BELT_REMAINDER(void);
void read_F_TIRE_DIRE_MNGT(void);
void read_IFE_ECUIdentifier_0x0520(void);
void read_F_OSEK_NM(void);
void read_RPAS_MSG_STAYBACK_TIME(void);
void read_AFE_ECUIdentifier_0x0525(void);
void read_SERVICE_DIST_SAP_CONFIG(void);
void read_DTE_AFE_ECUIdentifier_0x04D2(void);
void read_DTE_AFE_OVERSPEED(void);
void read_ODOMETER_CONFIG_0xF1B0(void);
void read_AT_GEAR_ALERT_0x0543_DATA(void);
void read_AFE_ECU_IDENTIFIER_0x0523_DATA(void);
void read_AFE_GD_ECU_IDENTIFIER_0x0524_DATA(void);
void read_TELL_TALE_FB_ADC_VALUE_0x04CF_DATA(void);
void read_DTE_ECUIdentifier_0x04D0_DATA(void);
void read_FUEL_GEAN_INDICATOR_0x04D1_DATA(void);
void read_AFE_ECUIdentifier_0x04F9_DATA(void);
void read_TELL_TALE_STATUS_SET1_0xF07F_DATA(void);
void read_TELL_TALE_STATUS_SET2_0xF080_DATA(void);
void read_TELL_TALE_STATUS_SET3_0xF081_DATA(void);
void read_TELL_TALE_STATUS_SET4_0xF08E_DATA(void);
void read_TELL_TALE_STATUS_SET5_0xF08F_DATA(void);
void read_GAUGES_0xF082_DATA(void);
void read_ILLUMINATION_STATUS_0xF084_DATA(void);
void read_BAR_GRAPH_STATUS_0xF083_DATA(void);
void read_GEAR_INDICATION_STATUS_0xF085_DATA(void);
void read_TELL_TALE_STATUS_SET6_0xF096_DATA(void);
void read_BATTERY_VOLTAGE_0xF1B1_DATA(void);
void read_VEHICLE_SPEED_0xF1B2_DATA(void);
void read_ENGINE_SPEED_0xF1B3_DATA(void);
void read_RESET_REASON_0xF1B4_DATA(void);
void read_RESET_COUNTER_0xF1B5_DATA(void);
void read_IGNITION_COUNTER_0xF1B6_DATA(void);
void read_EEPROM_LAYOUT_0xF1B7_DATA(void);
void read_NS_Network_Security_key_SKC_0x0108(void);
void read_NS_Network_Security_key_SKC1_0x0109(void);
void read_NS_Network_Security_key_SKC2_0x010A(void);
void read_NS_Network_Security_Configuration_0x010B(void);
void read_NS_Can_Wakeup_Configuration_0x0104(void);
void read_NS_NM_Configuration_0x0103(void);
void read_NS_Fault_Type_Status_Info_Configuration_0x0106(void);
void read_NS_Fault_Msg_Details_0x0105(void);
void read_NS_NW_Msg_Config_0x010E(void);
void read_NS_CAN_WEKUP_Configuration_PKE_0x010D(void);
//ETM
void read_ETM_CONFIG(void);
/*Read all EEPROM Parameters*/
void write_Default_PARA(void);
void Load_NM_Configuration(void);
/////

/**********************************************************************************************************************************/


uint8_t System_supplier_part_number_arr[SYSTEM_SUPPLIER_PART_NUMBER_ARR_SIZE];


//uint8_t Ecu_software_version_number_arr [ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE]= {'8','.','4',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
#ifdef COLOR_TFT
uint8_t Boot_software_identification_arr[BOOT_SOFTWARE_IDENTIFICATION_ARR_SIZE]= {'B','O','O','T','L','2','2','1'};
#else
uint8_t Boot_software_identification_arr[BOOT_SOFTWARE_IDENTIFICATION_ARR_SIZE]= {'B','O','O','T','L','2','2','0'};
#endif
#ifdef COLOR_TFT
#ifdef SPRINT4
uint8_t Ecu_software_version_number_arr [ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE]= {'W','5','0','1','H','2','3','9'};	/*Application version updated for Sprint4*/
#else
uint8_t Ecu_software_version_number_arr [ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE]= {'W','5','0','1','H','2','4','2'};	/*Application version updated for Sprint3*/
#endif
uint8_t Sparepart_number_arr[SPAREPART_NUMBER_ARR_SIZE] = {'1','3','0','1','A','W','5','0','0','0','1','1','N'};
uint8_t Graphics_version[GRAPHICS_VERSION_SIZE] = {'5','0','1','G','H','2','3','5'};
#endif
#ifdef GRAYSCALE_TFT
#ifdef U350
uint8_t Ecu_software_version_number_arr [ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE]= {'U','3','5','0','L','2','3','0'};
uint8_t Sparepart_number_arr[SPAREPART_NUMBER_ARR_SIZE] = {'1','3','0','1','A','U','3','0','0','1','1','1','N'};
uint8_t Graphics_version[GRAPHICS_VERSION_SIZE] = {'3','5','0','G','L','2','2','6'};
#else
uint8_t Ecu_software_version_number_arr [ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE]= {'W','5','0','1','L','2','4','3'};
uint8_t Sparepart_number_arr[SPAREPART_NUMBER_ARR_SIZE] = {'1','3','0','1','A','W','5','0','0','0','2','1','N'};
uint8_t Graphics_version[GRAPHICS_VERSION_SIZE] = {'5','0','1','G','L','2','3','5'};
#endif
#endif

uint8_t Graphics_version_read[GRAPHICS_VERSION_SIZE] = {'0','0','0','0','0','0','0','0'};
uint8_t Supplier_identification_arr[SUPPLIER_IDENTIFICATION_ARR_SIZE] = {'M','S','I','L',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
uint8_t Manufacturing_date_arr[MANUFACTURING_DATE_RW_ARR_SIZE] = {'3','1','0','8','1','9'};
uint8_t Ecu_serial_number_arr[ECU_SERIAL_NUMBER_ARR_SIZE] = {'0','8','1','9','0','0','0','1'};
uint8_t VIN_arr[VIN_RW_ARR_SIZE] = {'9','9','9','9','9','9','9','9','9','9','9','9','9','9','9','9','9'};
uint8_t Ecu_hardware_version_number_arr [ECU_HARDWARE_VERSION_NUMBER_ARR_SIZE] = {'3','5','8','E'};
uint8_t Engine_type_arr[ENGINE_TYPE_ARR_SIZE ] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
uint8_t tester_serial_number_arr[TESTER_SERIAL_NUMBER_ARR_SIZE] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
uint8_t Programming_date_arr[PROGRAMMING_DATE_ARR_SIZE] = {'0','0','0','0','0','0'};
uint8_t Ecu_installation_date_arr[ECU_INSTALLATION_DATE_ARR_SIZE] = {'0','0','0','0','0','0'};
uint8_t Model_number_arr[MODEL_NUMBER_ARR_SIZE] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
uint8_t Varient_code_arr[VARIANT_CODE_ARR_SIZE] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t Feature_code_arr[FEATURE_CODE_SIZE] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

uint8_t Ecu_software_number_arr[ECU_SOFTWARE_NUMBER_ARR_SIZE]={'8','.','4'};
//uint8_t Ecu_hardware_version_number_arr [ECU_HARDWARE_VERSION_NUMBER_ARR_SIZE] = {'2','3','3','1',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

//uint8_t Ecu_hardware_number_arr[ECU_HARDWARE_NUMBER_ARR_SIZE] = {'E','1','1','0','2','3','3','0','0','2','8','4',' ',' ',' ',' '};
uint8_t Ecu_hardware_number_arr[ECU_HARDWARE_NUMBER_ARR_SIZE] = {'E','1','1','0'};

uint8_t App_data_identification_arr[APP_DATA_IDENTIFICATION_ARR_SIZE ];
//uint8_t App_software_Identification[DID_APP_SOFTWARE_IDENTIFICATION_SIZE]= {'E','3','6','0','2','4','7','9','0','6','5','9','C'};
uint8_t App_software_Identification[DID_APP_SOFTWARE_IDENTIFICATION_SIZE]= {'E','3','6','0','2','4','7','9'};

uint8_t Fuel_level_arr[FUEL_LEVEL_SIZE];
uint8_t System_Supplier_Part_No[13] = {'6','2','0','1','0','0','0','0','0','2','3','E',' '};
/***************************************************************************************************/
uint8_t GRAPHIC_VERSION_NUMBER_ARR[GRAPHIC_VERSION_ARRY_SIZE];
uint8_t Vms_1_RW_arr[VMS_1_RW_ARR_SIZE ];
uint8_t Vms_2_RW_arr[VMS_2_RW_ARR_SIZE ];
uint8_t Boot_flag_arr[BOOT_FLAG_ARR_SIZE];
uint8_t Active_diag_sesion_arr[ACTIVE_DIAG_SESSION_SIZE];
uint8_t Engine_rpm_arr[ENGINE_RPM_SIZE];
uint8_t Def_bar_num_arr[DEF_BAR_NUM_ARR_SIZE];

uint8_t Def_level_per_arr[DEF_LEVEL_PER_ARR_SIZE];
uint8_t Battery_vtg_arr[BATTERY_VOLTAGE_ARR_SIZE];

//extern struct FLAGS EOL; w501
struct FLAGS EOL;
/************************************** W501 ECU Identifier array***************************************************/
uint8_t  DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[ECUIdentifier_0x050B_ARRY_SIZE];
uint8_t  DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[ECUIdentifier_0x050C_ARRY_SIZE];
uint8_t  DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[ECUIdentifier_0x050D_ARRY_SIZE];
uint8_t  DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[ECUIdentifier_0x050E_ARRY_SIZE];
uint8_t  DID_CHIMES_CONFIG_0x0503_arr[ECUIdentifier_0x0503_ARRY_SIZE];
uint8_t  DID_MESSAGE_CONFIG_0x0504_arr[ECUIdentifier_0x0504_ARRY_SIZE];
uint8_t  DID_SEAT_BELT_REMAINDER_0x0507_arr[ECUIdentifier_0x0507_ARRY_SIZE];
uint8_t  DID_CHIME_ACTIVATION_0x0508_arr[ECUIdentifier_0x0508_ARRY_SIZE];
uint8_t  DID_CHIME_ACTIVATION_0x0509_arr[ECUIdentifier_0x0509_ARRY_SIZE];
uint8_t  DID_TIRE_DIRE_MNGT_0x0510_arr[ECUIdentifier_0x0510_ARRY_SIZE];
uint8_t  DID_AVG_SPEED_0x0511_arr[ECUIdentifier_0x0511_ARRY_SIZE];
uint8_t  DID_IFE_ECUIdentifier_0x0520_arr[ECUIdentifier_0x0520_ARRY_SIZE];
uint8_t  DID_OSEK_NM_0x0521_arr[ECUIdentifier_0x0521_ARRY_SIZE];
//extern uint8_t  DID_GEAR_STATUS_0x0519_arr[ECUIdentifier_0x0519_ARRY_SIZE];
uint8_t  DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[ECUIdentifier_0x0522_ARRY_SIZE];
uint8_t  DID_AFE_GD_ECUIdentifier_0x0524_arr[ECUIdentifier_0x0524_ARRY_SIZE];
uint8_t  DID_AFE_ECUIdentifier_0x0525_arr[ECUIdentifier_0x0525_ARRY_SIZE];
uint8_t  DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[ECUIdentifier_0x050F_ARRY_SIZE];
uint8_t  DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[ECUIdentifier_0x04CF_ARRY_SIZE];
uint8_t  DID_DTE_ECUIdentifier_0x04D0_arr[ECUIdentifier_0x04D0_ARRY_SIZE];
uint8_t  DID_FUEL_GEAN_INDICATOR_0x04D1_arr[ECUIdentifier_0x04D1_ARRY_SIZE];
uint8_t  DID_DTE_AFE_ECUIdentifier_0x04D2_arr[ECUIdentifier_0x04D2_ARRY_SIZE];
uint8_t  DID_FUEL_DEAD_VOLUME_0x04D5_arr[ECUIdentifier_0x04D5_ARRY_SIZE];
uint8_t  DID_NVM_SERVICE_DONE_0x04F3_arr[ECUIdentifier_0x04F3_ARRY_SIZE];
uint8_t  DID_ODOMETER_RAW_0x04F4_arr[ECUIdentifier_0x04F4_ARRY_SIZE];
uint8_t  DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[ECUIdentifier_0x04F6_ARRY_SIZE];
uint8_t  DID_OVERSPEED_0x04F7_arr[ECUIdentifier_0x04F7_ARRY_SIZE];
uint8_t  DID_TELL_TALE_0x04F8_arr[ECUIdentifier_0x04F8_ARRY_SIZE];
uint8_t  DID_AFE_ECUIdentifier_0x04F9_arr[ECUIdentifier_0x04F9_ARRY_SIZE];
uint8_t  DID_TELL_TALE_STATUS_SET1_0xF07F_arr[ECUIdentifier_0xF07F_ARRY_SIZE];
uint8_t  DID_TELL_TALE_STATUS_SET2_0xF080_arr[ECUIdentifier_0xF080_ARRY_SIZE];
uint8_t  DID_TELL_TALE_STATUS_SET3_0xF081_arr[ECUIdentifier_0xF081_ARRY_SIZE];
uint8_t  DID_TELL_TALE_STATUS_SET4_0xF08E_arr[ECUIdentifier_0xF08E_ARRY_SIZE];
uint8_t  DID_TELL_TALE_STATUS_SET5_0xF08F_arr[ECUIdentifier_0xF08F_ARRY_SIZE];
uint8_t  DID_GAUGES_0xF082_arr[ECUIdentifier_0xF082_ARRY_SIZE];
uint8_t  DID_ILLUMINATION_STATUS_0xF084_arr[ECUIdentifier_0xF084_ARRY_SIZE];
//uint8_t  DID_RESET_REASON_CONFIG_0xF1B4_arr[ECUIdentifier_0xF184_ARRY_SIZE];
uint8_t  DID_BAR_GRAPH_STATUS_0xF083_arr[ECUIdentifier_0xF083_ARRY_SIZE];
uint8_t  DID_GEAR_INDICATION_STATUS_0xF085_arr[ECUIdentifier_0xF085_ARRY_SIZE];
uint8_t  DID_TELL_TALE_STATUS_SET6_0xF096_arr[ECUIdentifier_0xF096_ARRY_SIZE];
uint8_t  DID_ODOMETER_0xF1B0_arr[ECUIdentifier_0xF1B0_ARRY_SIZE];
uint8_t  DID_ODOMETER_0xF1B0_read_arr[ECUIdentifier_0xF1B0_ARRY_SIZE];
uint8_t  DID_BATTERY_VOLTAGE_0xF1B1_arr[ECUIdentifier_0xF1B1_ARRY_SIZE];
uint8_t  DID_VEHICLE_SPEED_0xF1B2_arr[ECUIdentifier_0xF1B2_ARRY_SIZE];
uint8_t  DID_ENGINE_SPEED_0xF1B3_arr[ECUIdentifier_0xF1B3_ARRY_SIZE];
uint8_t  DID_RESET_REASON_0xF1B4_arr[ECUIdentifier_0xF1B4_ARRY_SIZE];
uint8_t  DID_RESET_COUNTER_0xF1B5_arr[ECUIdentifier_0xF1B5_ARRY_SIZE];
uint8_t  DID_IGNITION_COUNTER_0xF1B6_arr[ECUIdentifier_0xF1B6_ARRY_SIZE];
uint8_t  DID_EEPROM_LAYOUT_0xF1B7_arr[ECUIdentifier_0xF1B7_ARRY_SIZE];
uint8_t  DID_AFE_ECU_IDENTIFIER_0x0523_arr[ECUIdentifier_0x0523_ARRY_SIZE];
uint8_t  DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[ECUIdentifier_0x0524_ARRY_SIZE];

uint8_t DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[ECUIdentifier_0x0502_ARRY_SIZE];
uint8_t DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[ECUIdentifier_0x04FB_ARRY_SIZE];
uint8_t DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[ECUIdentifier_0x04FC_ARRY_SIZE];
uint8_t DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[ECUIdentifier_0x04FD_ARRY_SIZE];
uint8_t DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[ECUIdentifier_0x04FE_ARRY_SIZE];
uint8_t DID_Roll_Pitch_Detection_0x0533_arr[ECUIdentifier_0x0533_ARRY_SIZE];
uint8_t DID_AT_GEAR_ALERT_0x0543_arr[ECUIdentifier_0x0543_ARRY_SIZE];
//Network_Security
uint8_t NS_Varient_code_arr[VARIANT_CODE_ARR_SIZE];
uint8_t  NS_Network_Security_key_SKC_0x0108_arr[SKC_0x0108_arr_ARRY_SIZE];
uint8_t  NS_Network_Security_key_SKC1_0x0109_arr[SKC1_0x0109_arr_ARRY_SIZE];
uint8_t  NS_Network_Security_key_SKC2_0x010A_arr[SKC2_0x010A_arr_ARRY_SIZE];
uint8_t  NS_Network_Security_Configuration_0x010B_arr[NSC_0x010B_arr_ARRY_SIZE];
uint8_t  NS_Can_Wakeup_Configuration_0x0104_arr[NSC_0x0104_arr_ARRY_SIZE];
uint8_t  NS_NM_Configuration_0x0103_arr[NSC_0x0103_arr_ARRY_SIZE];
uint8_t NS_Fault_Type_Status_Info_0x0106_arr[NSC_0x0106_ARRY_SIZE];
uint8_t NS_Fault_Msg_Details_0x0105_arr[NSC_0x0105_arr_ARRY_SIZE];
uint8_t NS_NW_Msg_config_0x010E_arr[NSC_0x010E_arr_ARRY_SIZE];
uint8_t NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE];
//
//ETM
uint8_t  DID_ETM_0x0500_arr[ECUIdentifier_0x0500_ARRY_SIZE];

uint16_t Diag_RW_IFE_Accum_Time;
uint16_t Diag_RW_IFE_Sample_Time;
uint8_t	Diag_RW_IFE_Sample_Size;
uint16_t Diag_RW_IFE_Display_Time;
uint8_t	Diag_RW_IFE_Units;
uint8_t	Diag_RW_IFE_Max;

float Fuel_Consp_Factor;
uint8_t AFE_CALC_INTERVAL;
uint8_t AFE_DISPLAY_INTERVAL;
uint8_t AFE_DISPLAY_FILTER_A;
uint16_t AFE_DISPLAY_FILTER_B;
uint8_t AFE_DRIVE_CYCLE_RESET_ENABLE;
uint8_t AFE_DRIVE_CYCLE_RESET_INTERVAL;
uint8_t AFE_INIT_THRESHOLD_DISTANCE;

uint8_t DTE_AFE_AVG_KM;
uint8_t DTE_DEFAULT_FE;
uint8_t DTE_DISPLAY_FILTER_C;
uint8_t DTE_DISPLAY_FILTER_B;
uint8_t DTE_DISPLAY_FILTER_A;
uint8_t DTE_AFE_MAX ;
uint8_t DTE_AFE_MIN;
uint8_t Diag_Unit_Selection ;

uint32_t Diag_RW_Eng_Run_threshold_RPM;
uint8_t Diag_RW_Eng_Run_threshold_time;
uint8_t Diag_RW_Min_VehSpeed_Threshold;
uint8_t Diag_RW_Vehicle_Moving_threshold_time;
uint16_t Diag_RW_Vehicle_Moving_threshold_distance;
uint8_t Diag_RW_Vehicle_Moving_final_speed;
uint8_t Diag_RW_Final_chime_duration;

uint8_t Diag_RW_Speed_Min_for_Chime_activation;

uint32_t Diag_RW_AVS_Initial_Sample_Distance_Config;
uint8_t	 Diag_RW_AVS_SAMPLE_TIME;

uint8_t Diag_RW_Level1_Threshold_Angle;
uint8_t Diag_RW_Level2_Threshold_Angle;
uint8_t Diag_RW_Level3_Threshold_Angle;
uint8_t Diag_RW_Level4_Threshold_Angle;
uint8_t Diag_RW_Level5_Threshold_Angle;

uint8_t Diag_RW_Trajectory_Calibration;

uint8_t Diag_RW_Temp_Bar1_ON_value;
uint8_t	Diag_RW_Temp_Bar2_ON_value;
uint8_t	Diag_RW_Temp_Bar3_ON_value;
uint8_t	Diag_RW_Temp_Bar4_ON_value;
uint8_t	Diag_RW_Temp_Bar5_ON_value;
uint8_t	Diag_RW_Temp_Bar6_ON_value;
uint8_t	Diag_RW_Temp_Bar7_ON_value;
uint8_t	Diag_RW_Temp_Bar8_ON_value;
uint8_t	Diag_RW_Temp_Bar1_OFF_value;
uint8_t	Diag_RW_Temp_Bar2_OFF_value;
uint8_t	Diag_RW_Temp_Bar3_OFF_value;
uint8_t	Diag_RW_Temp_Bar4_OFF_value;
uint8_t	Diag_RW_Temp_Bar5_OFF_value;
uint8_t	Diag_RW_Temp_Bar6_OFF_value;
uint8_t	Diag_RW_Temp_Bar7_OFF_value;
uint8_t	Diag_RW_Temp_Bar8_OFF_value;


uint8_t Diag_RW_Fuel_Bar1_ON_value;
uint8_t	Diag_RW_Fuel_Bar2_ON_value;
uint8_t	Diag_RW_Fuel_Bar3_ON_value;
uint8_t	Diag_RW_Fuel_Bar4_ON_value;
uint8_t	Diag_RW_Fuel_Bar5_ON_value;
uint8_t	Diag_RW_Fuel_Bar6_ON_value;
uint8_t	Diag_RW_Fuel_Bar7_ON_value;
uint8_t	Diag_RW_Fuel_Bar8_ON_value;
uint8_t	Diag_RW_Fuel_Bar1_OFF_value;
uint8_t	Diag_RW_Fuel_Bar2_OFF_value;
uint8_t	Diag_RW_Fuel_Bar3_OFF_value;
uint8_t	Diag_RW_Fuel_Bar4_OFF_value;
uint8_t	Diag_RW_Fuel_Bar5_OFF_value;
uint8_t	Diag_RW_Fuel_Bar6_OFF_value;
uint8_t	Diag_RW_Fuel_Bar7_OFF_value;
uint8_t	Diag_RW_Fuel_Bar8_OFF_value;

uint8_t	Diag_RW_Fuel_TT_Continuous_OFF_Value;
uint8_t	Diag_RW_Fuel_TT_Continuous_ON_Value;
uint8_t	Diag_RW_Fuel_TT_Blink_OFF_Value;
uint8_t	Diag_RW_Fuel_TT_Blink_ON_Value;

uint8_t	Diag_RW_Temp_TT_Continuous_OFF_Value;
uint8_t	Diag_RW_Temp_TT_Continuous_ON_Value;
uint8_t	Diag_RW_Temp_TT_Blink_OFF_Value;
uint8_t	Diag_RW_Temp_TT_Blink_ON_Value;

uint16_t Diag_RW_ESC_ON_Time;
uint16_t Diag_RW_ESC_OFF_Time;

uint16_t Diag_RW_TT_PATTERN4_Immo_ON_Time;
uint16_t Diag_RW_TT_PATTERN4_Immo_OFF_Time;

uint16_t Diag_RW_TT_PATTERN5_Immo_ON_Time;
uint16_t Diag_RW_TT_PATTERN5_Immo_OFF_Time;

uint16_t Diag_RW_TT_PATTERN8_TPMS_ON_Time;
uint16_t Diag_RW_TT_PATTERN8_TPMS_OFF_Time;

uint16_t Diag_RW_TT_PATTERN9_TPMS_ON_Time;
uint16_t Diag_RW_TT_PATTERN9_TPMS_OFF_Time;

uint16_t Diag_RW_Fuel_Dead_Volume;
uint8_t Diag_RW_NVM_SERV_DONE;
uint8_t Diag_RW_ODO_RAW_DISPLAYABLE;
uint8_t Diag_RW_ODO_DISPLAYABEL_PERCENTAGE;
uint8_t Diag_RW_TPMS_Blink_Total_Duration_Setting;

uint8_t Diag_RW_Level1_Speed;
uint8_t	Diag_RW_Level2_Speed;
uint8_t	Diag_RW_Level1_Chime_Type;
uint8_t	Diag_RW_Level1_Chime_Repetition_Interval;
uint8_t	Diag_RW_Level2_Chime_Type;
uint8_t	Diag_RW_Level2_Chime_Repetition_Interval;
uint8_t	Diag_RW_Chime_Hysterisis;
uint32_t Diag_RW_Odometer;
uint8_t Diag_RW_Engineering_Test_Mode;

uint8_t Diag_RW_Role_Angle;
uint8_t Diag_RW_Role_Pitch;

uint16_t Diag_AT_GEAR_ALERT_Duration =0;
uint16_t Diag_AT_GEAR_ALERT_Debounce =0;
/*temp variable purpose is final variable need to replace */
uint16_t Diag_RW_Displayed_Value;
uint16_t Diag_RW_Calculated_Value;
uint32_t Diag_RW_Accumulated_Fuel; 
uint16_t Diag_RW_Accumulated_Distance;

uint16_t Diag_RW_AFE_GD_Displayed_Value;
uint16_t Diag_RW_AFE_GD_Calculated_Value;
uint32_t Diag_RW_AFE_GD_Accumulated_Fuel; 
uint16_t Diag_RW_AFE_GD_Accumulated_Distance;

uint16_t Diag_RW_TT_FB_ADC_Park_Brake_TT;
uint16_t Diag_RW_TT_FB_ADC_Check_Engine_TT;
uint16_t Diag_RW_TT_FB_ADC_MIL_Lamp_TT; 
uint16_t Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT;
uint16_t Diag_RW_TT_FB_ADC_ABS_TT_Failure;
uint16_t Diag_RW_TT_FB_ADC_Cruise_TT;
uint16_t Diag_RW_TT_FB_ADC_Airbag_TT; 
uint16_t Diag_RW_TT_FB_ADC_Fuel_Delivery_Module;
uint16_t Diag_RW_TT_FB_ADC_AT_Fail_TT;
uint16_t Diag_RW_TT_FB_ADC_ESP_Malfunction_TT;
uint16_t Diag_RW_TT_FB_ADC_ESC_OFF; 
uint16_t Diag_RW_TT_FB_ADC_HDC_Malfunction;

uint8_t  Diag_RW_DTE_RAFE;
uint16_t Diag_RW_DTE_RAFE_Accumulated_Fuel;
uint8_t  Diag_RW_DTE_RAFE_Accumulated_Distance;
uint16_t Diag_RW_DTE;

uint16_t Diag_RW_Fuel_consumption_accumulated;

uint8_t Diag_RW_TT_SET1_Check_Engine;
uint8_t Diag_RW_TT_SET1_Glow_Plug;
uint8_t Diag_RW_TT_SET1_Oil_Pressure_Low; 
uint8_t Diag_RW_TT_SET1_4WD_High;
uint8_t Diag_RW_TT_SET1_Door_Open;
uint8_t Diag_RW_TT_SET1_Seat_Belt;
uint8_t Diag_RW_TT_SET1_Immo; 
uint8_t Diag_RW_TT_SET1_Park_Brake;

uint8_t Diag_RW_TT_SET2_Airbag;
uint8_t Diag_RW_TT_SET2_Cruise;
uint8_t Diag_RW_TT_SET2_AT_Fail; 
uint8_t Diag_RW_TT_SET2_TPMS;
uint8_t Diag_RW_TT_SET2_ABS;
uint8_t Diag_RW_TT_SET2_Water_in_Fuel;
uint8_t Diag_RW_TT_SET2_ESS; 
uint8_t Diag_RW_TT_SET2_MIL_Lamp;

uint8_t Diag_RW_TT_SET3_ESC_Off;
uint8_t Diag_RW_TT_SET3_Park_Lamp;
uint8_t Diag_RW_TT_SET3_HHC_Malfunction; 
uint8_t Diag_RW_TT_SET3_HDC_Malfunction;
uint8_t Diag_RW_TT_SET3_ESC_Malfunction;
uint8_t Diag_RW_TT_SET3_Battery_Charge;
uint8_t Diag_RW_TT_SET3_Low_Fuel; 
uint8_t Diag_RW_TT_SET3_High_Temp;

uint8_t Diag_RW_TT_SET5_EPS_Malfunction;
uint8_t Diag_RW_TT_SET5_Overspeed;
uint8_t Diag_RW_TT_SET5_Reserved;

uint16_t Diag_RW_Speedometer_status;
uint16_t Diag_RW_Tachometer_status;
uint16_t Diag_RW_Tachometer_Redline;

uint8_t Diag_RW_LCD_Illumination_Status;
uint8_t Diag_RW_Dial_Illumination_Status;
uint8_t Diag_RW_Pointer_Illumination_Status;

uint8_t Diag_RW_Temperature_bar;
uint8_t Diag_RW_Fuel_Bar;

uint8_t Diag_RW_Major_Gear;
uint8_t Diag_RW_Minor_Gear;
uint8_t Diag_RW_Up_Indication_Gear;
uint8_t Diag_RW_Down_Indication_Gear;

uint8_t Diag_RW_Diesel_Particulate_Filter;
uint8_t Diag_RW_Diesel_Exhaust_system_Fluid;
uint8_t Diag_RW_4WD_Low;
uint8_t Diag_RW_Regeneration_Tell_Tale;

float32_t Diag_RW_Battery_Voltage;
uint32_t Diag_RW_Vehicle_Speed;
uint16_t Diag_RW_Engine_Speed;
uint16_t Diag_RW_Ignition_Counter;
uint16_t Diag_RW_Reset_Counter;
uint32_t Diag_RW_EEPROM_Layout_Version;
uint8_t Diag_RW_TT_SET4_ECO;
uint8_t Diag_RW_TT_SET4_High_Beam;
uint8_t Diag_RW_TT_SET4_Turn_Left;
uint8_t Diag_RW_TT_SET4_Turn_Right;
uint8_t Diag_RW_TT_SET4_Front_Fog;
uint8_t Diag_RW_TT_SET4_Rear_Fog;
uint8_t Diag_RW_External_reset_signal;
uint8_t Diag_RW_Watch_dog_reset = 2;
uint8_t Diag_RW_PowerOn_Clear_Resets;


uint16_t Diag_RW_Fuel_level_raw;
uint16_t Diag_RW_Current_Gear_shift;

uint16_t IVN_cfg_Message_ID1;
uint8_t IVN_cfg_Occurance_Count_Msg1;
uint8_t IVN_cfg_NS_Status_Msg1; 
uint16_t IVN_cfg_Message_ID2;
uint8_t IVN_cfg_Occurance_Count_Msg2;
uint8_t IVN_cfg_NS_Status_Msg2; 
uint16_t IVN_cfg_Message_ID3;
uint8_t IVN_cfg_Occurance_Count_Msg3;
uint8_t IVN_cfg_NS_Status_Msg3;
uint16_t IVN_cfg_Message_ID4;
uint8_t IVN_cfg_Occurance_Count_Msg4;
uint8_t IVN_cfg_NS_Status_Msg4;
uint16_t IVN_cfg_Message_ID5;
uint8_t IVN_cfg_Occurance_Count_Msg5;
uint8_t IVN_cfg_NS_Status_Msg5;
uint16_t IVN_cfg_Message_ID6;
uint8_t IVN_cfg_Occurance_Count_Msg6;
uint8_t IVN_cfg_NS_Status_Msg6;
uint16_t IVN_cfg_Message_ID7;
uint8_t IVN_cfg_Occurance_Count_Msg7;
uint8_t IVN_cfg_NS_Status_Msg7;
uint16_t IVN_cfg_Message_ID8;
uint8_t IVN_cfg_Occurance_Count_Msg8;
uint8_t IVN_cfg_NS_Status_Msg8;
uint16_t IVN_cfg_Message_ID9;
uint8_t IVN_cfg_Occurance_Count_Msg9;
uint8_t IVN_cfg_NS_Status_Msg9;
uint16_t IVN_cfg_Message_ID10;
uint8_t IVN_cfg_Occurance_Count_Msg10;
uint8_t IVN_cfg_NS_Status_Msg10;
   
uint8_t  IVN_Cfg_T_MBFM_PKE_COMMAND_TIME = 0;
uint32_t IVN_Cfg_T_WAKEUP_WAIT_Time = 0;
uint32_t IVN_Cfg_T_WAKEUP_SYNC_Time = 0;
uint32_t IVN_Cfg_T_SLEEP_WAIT1_Time = 0;
uint32_t IVN_Cfg_T_KEEP_NORMAL_MIN_TIMEOUT = 0; 
uint32_t IVN_Cfg_T_START_WAKEUPTX_Time = 0;
uint32_t IVN_Cfg_T_WAKEUPTX_Time = 0;
uint32_t IVN_Cfg_T_START_ACTIVETX_Time = 0;
uint32_t IVN_Cfg_T_ACTIVETX_Time = 0;

extern _Bool fuel__temp_udc_enable;
/******************RPAS Diagnostics Parameters**********************/

uint8_t Diag_RW_RPAS_Display_Screen_source;
uint8_t Diag_RW_RPAS_Sector_Zone_Calibration;
uint16_t Diag_RW_AT_Reverse_Gear_Debounce_Time;
uint16_t Diag_RW_RPASStayBackTimeVal;
uint16_t Diag_RW_RPAS_Error_Chime_Enable;
uint16_t Diag_RW_RPAS_self_check_chime_Enable;
uint8_t Diag_RW_Stop_Dist;
uint8_t Diag_RW_RPAS_Error_Wrn_Enable;
/*******************************************************************/

#ifdef U350
const uint8_t  DID_FUEL_BAR_DEFAULT_VALUE_0x050B_arr[ECUIdentifier_0x050B_ARRY_SIZE]={0x84,0x7B,0x6C,0x5D,0x50,0x43,0x2F,0x1B,
																					  0x88,0x7F,0x72,0x64,0x55,0x48,0x3B,0x27};
#else
const uint8_t  DID_FUEL_BAR_DEFAULT_VALUE_0x050B_arr[ECUIdentifier_0x050B_ARRY_SIZE]={0x7A,0x6A,0x5B,0x4C,0x3C,0x2D,0x22,0x17,
																					  0x82,0x72,0x62,0x53,0x42,0x34,0x27,0x1C};
#endif
const uint8_t  DID_TEMP_BAR_DEFAULT_VALUE_0x050C_arr[ECUIdentifier_0x050C_ARRY_SIZE]={0xA,0x32,0x46,0x5A,0x65,0x6A,0x6E,0x72,
																						0x6,0x2E,0x42,0x56,0x61,0x66,0x6A,0x6E};
const uint8_t  DID_FUEL_TT_DEFAULT_VALUE_0x050D_arr[ECUIdentifier_0x050D_ARRY_SIZE]={0x76,0x7C,0x82,0x86};
const uint8_t  DID_TEMP_TT_DEFAULT_VALUE_0x050E_arr[ECUIdentifier_0x050E_ARRY_SIZE]={0x6E,0x72,0x6A,0x6E};

extern Split32 IVN_SecurityKeyConst;
extern Split32 IVN_SecurityKey_1;
extern Split32 IVN_SecurityKey_2;
extern uint16_t IVN_cfg_FCIncCntrTiTh_C; 	/* XLX 010B 0  */
extern uint8_t IVN_cfg_FCDevTh_C;			/* XLX 010B 1  */
extern uint8_t IVN_cfg_FcDifErrReptTh_C;   /* XLX 010B 2  */
extern uint8_t IVN_cfg_FcErrReptInc_C;		/* XLX 010B 3  */
extern uint8_t IVN_cfg_FcErrReptDec_C;		/* XLX 010B 4  */
extern uint8_t IVN_cfg_FcErrHealDec_C;		/* XLX 010B 5  */
extern uint8_t IVN_cfg_ResyncCntrTh_C;		/* XLX 010B 6  */
extern uint16_t IVN_cfg_ResyncTiTh_C;		/* XLX 010B 7  */
extern uint8_t IVN_cfg_MAC2ErrCntrTh_C;	/* XLX 010B 8  */
extern uint16_t IVN_cfg_SynchWndTiTh_C;  	/* XLX 010B 9  */
extern uint8_t IVN_cfg_SynchMsgTxCntr_C;	/* XLX 010B 10  */

uint8_t IVN_cfg_CAN_Wakeup_Msg_Count;   /* DID 1004 */
uint16_t	IVN_cfg_Quick_Recovery_timeout_period;
uint8_t	IVN_cfg_Quick_Recovery_retry_limit;
uint16_t	IVN_cfg_Slow_Recovery_timeout_period;
uint8_t	IVN_cfg_Slow_Recovery_retry_limit;
/* DID 0103 */
uint8_t  IVN_cfg_NA_Detection_Threshold;   
uint8_t	 IVN_cfg_NA_Recovery_Threshold;
uint8_t	 IVN_cfg_Msg_Timeout_Detection_Threshold;
uint8_t	 IVN_cfg_Msg_Timeout_Recovery_Threshold;
uint8_t	 IVN_cfg_Msg_Content_Error_Detection_Threshold;
uint8_t  IVN_cfg_Msg_Content_Error_Recovery_Threshold;  
uint8_t	 IVN_cfg_NM_Drive_Cnt_Clear_DTC;
uint16_t IVN_cfg_Busoff_Fast_Recovery_Time;
uint8_t	 IVN_cfg_Fast_Bus_off_Recovery_Count;
uint16_t IVN_cfg_Busoff_Slow_Recovery_Time;
uint16_t IVN_cfg_NM_IGN_On_Startup_Delay;  
uint16_t IVN_cfg_NM_Restart_Dly_Time_Under_Vol_Recovery;
uint16_t IVN_cfg_NA_NM_Restart_Dly_Time_Over_Vol_Recovery;
uint16_t IVN_cfg_NA_NM_Restart_Dly_Time_Bus_Off_recovery;
uint8_t	 IVN_cfg_NA_Signal_Content_Fail_Detect_Count;
uint8_t	 IVN_cfg_NA_Signal_Content_Recov_Detect_Count;
uint8_t	 IVN_cfg_NA_CAN_Wakeup_Feature_Enable;
uint16_t IVN_cfg_NA_NM_Restart_Dly_Time_Cranking;
/*0106*/
uint8_t	IVN_cfg_FC_Not_matching;
uint8_t	IVN_cfg_RX_msg_Periodicity;
uint8_t	IVN_cfg_TX_msg_Received;
uint8_t	IVN_cfg_NSM_Msg_Timout;
uint8_t	IVN_cfg_SK2_Not_matching;
uint8_t	IVN_cfg_Play_protection;
uint8_t	IVN_cfg_Reserved;

/* DID 0107 */
uint8_t IVN_cfg_NS_variant_code;   
uint8_t	IVN_cfg_NS_EMS;
uint8_t	IVN_cfg_NS_ABS;
uint8_t	IVN_cfg_NS_ESC;
uint8_t	IVN_cfg_NS_SAS;
uint8_t IVN_cfg_NS_PKE;  
uint8_t	IVN_cfg_NS_ICU;
uint8_t	IVN_cfg_NS_SRS;
uint8_t	IVN_cfg_NS_TCU;
uint8_t	IVN_cfg_NS_IC;
uint8_t IVN_cfg_NS_MBFM;  
uint8_t	IVN_cfg_NS_Infotainment_System;

/*DID 010E*/

uint8_t Msg_enable_IC4_1000_Test;
uint8_t	Msg_enable_IC6_1000_Test;
uint8_t	Msg_enable_IC7_1000_Test;
uint8_t	Msg_enable_IC_NMV_Test;

/* IVN Message Timeout and Node absent Structures*/
Message_Timeout_t EMS3_10_Message;	//NA
Message_Timeout_t EMS36_10_Message;
Message_Timeout_t EMS1_10_Message;
Message_Timeout_t EMS8_10_Message;
Message_Timeout_t EMS4_20_Message;
Message_Timeout_t EMS29_100_Message;
Message_Timeout_t EMS6_500_Message;
Message_Timeout_t EMS12_200_Message;
Message_Timeout_t EMS5_500_Message;
Message_Timeout_t EMS_NSM_Message;	//100ms

Message_Timeout_t ESC5_10_Message;	//NA
Message_Timeout_t ESC2_10_Message;
Message_Timeout_t ESC12_10_Message;
Message_Timeout_t ESC_NSM_Message;	//100ms
//#ifdef SPRINT4
/*TDMS*/
Message_Timeout_t SAS1_10_Message;
//#endif
Message_Timeout_t TCU5_10_Message;	//NA
Message_Timeout_t TCU6_20_Message;
Message_Timeout_t TCU_NSM_Message;	//100ms

Message_Timeout_t MBFM1_100_Message;	//NA
Message_Timeout_t MBFM5_100_Message;
Message_Timeout_t MBFM9_500_Message;
Message_Timeout_t MBFM_PAS1_50_Message;

Message_Timeout_t SRS1_20_Message;	//NA

Message_Timeout_t PKE_ICU2_100_Message;	//NA

Message_Timeout_t RPAS1_20_Message;	//NA
Message_Timeout_t RPAS2_100_Message;

Message_Timeout_t IS1_100_Message;	//NA
Message_Timeout_t IS3_500_Message;
Message_Timeout_t IS_NSM_Message;	//100ms


extern uint8_t gear_to_HMI;

/***********************************************************************************************************************
 * Function Name: Write_Default_Data
 * Description  : This function is used to write Default data to EEPROM.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
PUBLIC void Write_Default_Data(void)
{

	Write_VAR_FEATURE_Code_Default_Data();
	Write_FF_TT_BAR_Default_Data();
	Write_FF_TT_Tell_Tale_Default_Data();

}

/***********************************************************************************************************************
 * Function Name: Write_VAR_FEATURE_Code_Default_Data
 * Description  : This function is used to write Feature code Default data to EEPROM.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
PUBLIC void Write_VAR_FEATURE_Code_Default_Data(void)
{
	uint8_t Index,i, VAR_Defaultval_counter =0,FEATURE_code_counter=0,NS_VAR_Defaultval_counter=0;

	for(Index=0; Index<20; Index++)
	{
		if(Varient_code_arr[Index]== 0xFF)
		{
			//Varient_code_arr[Index]=0;
			//VAR_Defaultval_counter++ ;
		}

		if(VAR_Defaultval_counter == 20)
		{
			for(i=0;i<20; i++)
			{
				Varient_code_arr[i]=0;
			}
			write_varient_code(&Varient_code_arr[0]);
		}


		if(Feature_code_arr[Index]== 0xFF)
		{
			//Feature_code_arr[Index]=0;
			//FEATURE_code_counter++ ;

		}

		if(FEATURE_code_counter == 20)
		{
			for(i=0;i<20; i++)
			{
				Feature_code_arr[i]=0;
			}
			write_feature_code(&Feature_code_arr[0]);
		}

		/* if(NS_Varient_code_arr[Index]== 0xFF)
		{
			NS_VAR_Defaultval_counter++ ;
		}

		if(NS_VAR_Defaultval_counter == 20)
		{
			for(i=0;i<20; i++)
			{
				NS_Varient_code_arr[i]=0;
			}
			write_NS_varient_code(&NS_Varient_code_arr[0]);
		} */
	}
}

/***********************************************************************************************************************
 * Function Name: Write_FF_TT_BAR_Default_Data
 * Description  : This function is used to write Fuel BAR and Temp BAR data Default data to EEPROM.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
PUBLIC void Write_FF_TT_BAR_Default_Data(void)
{
	uint8_t Index,i, FF_BAR_counter =0, TT_BAR_counter=0;

	for(Index=0; Index<16; Index++)
	{
		if(DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[Index]== 0xFF)
		{
			//	DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[Index]=DID_FUEL_BAR_DEFAULT_VALUE_0x050B_arr[Index];
			//FF_BAR_counter++ ;
		}
		if(FF_BAR_counter == 16)
		{
			for(i=0;i<16;i++)
			{
				DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[i]=DID_FUEL_BAR_DEFAULT_VALUE_0x050B_arr[i];
			}
			write_FUEL_BAR_VALUE(&DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0]);
			Get_Default_ADCvalue();//r
		}


		if(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[Index]== 0xFF)
		{
			//	DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[Index]=DID_TEMP_BAR_DEFAULT_VALUE_0x050C_arr[Index];
			//TT_BAR_counter++ ;
		}

		if(TT_BAR_counter == 16)
		{
			for(i=0;i<16;i++)
			{
				DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[i]=DID_TEMP_BAR_DEFAULT_VALUE_0x050C_arr[i];
			}
			write_TEMP_BAR_VALUE(&DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0]);
		}
	}
}

/***********************************************************************************************************************
 * Function Name: Write_FF_TT_Tell_Tale_Default_Data
 * Description  : This function is used to write Fuel and Temp TT status Default data to EEPROM.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
PUBLIC void Write_FF_TT_Tell_Tale_Default_Data(void)
{
	uint8_t Index,i, FF_TT_counter =0, Temp_TT_counter=0;

	for(Index=0; Index<4; Index++)
	{
		if(DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[Index]== 0xFF)
		{
			//	DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[Index]=DID_FUEL_TT_DEFAULT_VALUE_0x050D_arr[Index];

			//FF_TT_counter++ ;
		}
		if(FF_TT_counter == 4)
		{
			for(i=0;i<4;i++)
			{
				DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[i]=DID_FUEL_TT_DEFAULT_VALUE_0x050D_arr[i];
			}
			write_FUEL_TT_VALUE(&DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0]);
			//fuel_did();
		}


		if(DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[Index]== 0xFF)
		{
			//	DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[Index] = DID_TEMP_TT_DEFAULT_VALUE_0x050E_arr[Index];
			//Temp_TT_counter++ ;
		}

		if(Temp_TT_counter == 4)
		{
			for(i=0;i<4;i++)
			{
				DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[i] = DID_TEMP_TT_DEFAULT_VALUE_0x050E_arr[i];
			}
			write_TEMP_TT_VALUE_CONFIG(&DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0]);
		}
	}
	fuel_did();
	TEMP_did();
	fuel__temp_udc_enable=1;
}







/***********************************************************************************************************************
 * Function Name: Write_UDS_Data
 * Description  : This function is used to write UDS data to EEPROM.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
PUBLIC void Write_UDS_Data(void)
{
	if(EOL.F_Update_history_zone == SET)
	{
		write_history_zone(&history_zone[0]);
		EOL.F_Update_history_zone = CLEAR;
	}

	if(EOL.F_VIN == SET)
	{
		write_vin_zone(&VIN_arr[0]);
		EOL.F_VIN = CLEAR;
	}

	if(EOL.F_VMS_1_RW == SET)
	{
		write_vms1_zone(&Vms_1_RW_arr[0]);
		EOL.F_VMS_1_RW = CLEAR;
	}
#if 0
	if(EOL.F_VMS_2_RW == SET)
	{
		write_vms2_zone(&Vms_2_RW_arr[0]);
		EOL.F_VMS_2_RW = CLEAR;
	}
#endif
	if(EOL.F_Programming_date == SET)
	{
		write_programming_date_zone(&Programming_date_arr[0]);
		EOL.F_Programming_date = CLEAR;
	}

	if(EOL.F_Model_number == SET)
	{
		write_model_number(&Model_number_arr[0]);
		EOL.F_Model_number = CLEAR;
	}

	if(EOL.F_Ecu_installation_date == SET)
	{
		write_ecu_installation_date(&Ecu_installation_date_arr[0]);
		EOL.F_Ecu_installation_date = CLEAR;
	}

	if(EOL.F_NS_NSConfiguration_0x010B == SET)
	{
		write_NS_Network_Security_Configuration_0x010B(&NS_Network_Security_Configuration_0x010B_arr[0]);
		EOL.F_NS_NSConfiguration_0x010B = CLEAR;
	}

	if(EOL.F_ECU_MAN_DATE == SET)
	{
		//write_ecu_manf_date(&Manufacturing_date_arr[0]);   //commented till EOL parameter is defined
		EOL.F_ECU_MAN_DATE = CLEAR;
	}

	if(EOL.F_Engine_type == SET)
	{
		write_engine_name_type(&Engine_type_arr[0]);
		EOL.F_Engine_type = CLEAR;
	}

	if(EOL.F_tester_serial_number == SET)
	{
		write_tester_serial_number(&tester_serial_number_arr[0]);
		EOL.F_tester_serial_number = CLEAR;
	}

	if(EOL.F_FEATURE_CODE == SET)
	{
		write_feature_code(&Feature_code_arr[0]);
		EOL.F_FEATURE_CODE = CLEAR;
	}

	if(EOL.F_Varient_code == SET)
	{
		write_varient_code(&Varient_code_arr[0]);
		EOL.F_Varient_code = CLEAR;
	}

	if(EOL.F_NS_Varient_code == SET)
	{
		write_NS_varient_code(&NS_Varient_code_arr[0]);
		EOL.F_NS_Varient_code = CLEAR;
	}

	if(EOL.F_Ecu_serial_number == SET)
	{
		//write_Ecu_serial_number(&Ecu_serial_number_arr[0]);  //commented till EOL parameter is defined
		EOL.F_Ecu_serial_number = CLEAR;
	}

	if(EOL.F_Supplier_identification == SET)
	{
		//write_System_Supplier_Identification(&Supplier_identification_arr[0]); //removed till EOL parameter is defined
		EOL.F_Supplier_identification = CLEAR;
	}

	if(EOL.F_FUEL_BAR_VALUE_CONFIG == SET)
	{
		write_FUEL_BAR_VALUE(&DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0]);
		EOL.F_FUEL_BAR_VALUE_CONFIG = CLEAR;
	}

	if(EOL.F_TEMP_BAR_VALUE_CONFIG == SET)
	{
		write_TEMP_BAR_VALUE(&DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0]);
		EOL.F_TEMP_BAR_VALUE_CONFIG = CLEAR;
	}

	if(EOL.F_FUEL_TT_VALUE_CONFIG == SET)
	{
		write_FUEL_TT_VALUE(&DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0]);
		EOL.F_FUEL_TT_VALUE_CONFIG = CLEAR;
	}

	if(EOL.F_TEMP_TT_VALUE_CONFIG == SET)
	{
		write_TEMP_TT_VALUE_CONFIG(&DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0]);
		EOL.F_TEMP_TT_VALUE_CONFIG = CLEAR;
	}

	if(EOL.F_NS_SKC_0108_CONFIG == SET)
	{
		write_NS_Network_Security_key_SKC_0x0108(&NS_Network_Security_key_SKC_0x0108_arr[0]);
		EOL.F_NS_SKC_0108_CONFIG = CLEAR;
	}

	if(EOL.F_NS_SKC1_0109_CONFIG == SET)
	{
		write_NS_Network_Security_key_SKC1_0x0109(&NS_Network_Security_key_SKC1_0x0109_arr[0]);
		EOL.F_NS_SKC1_0109_CONFIG = CLEAR;
	}

	if(EOL.F_NS_SKC2_010A_CONFIG == SET)
	{
		write_NS_Network_Security_key_SKC2_0x010A(&NS_Network_Security_key_SKC2_0x010A_arr[0]);
		EOL.F_NS_SKC2_010A_CONFIG = CLEAR;
	}
#if 0
	if(EOL.F_CHIME_ACTIVATION_0x0508 == SET)
	{
		write_CHIME_ACTIVATION_0x0508(&DID_CHIME_ACTIVATION_0x0508_arr[0]);
		EOL.F_CHIME_ACTIVATION_0x0508 = CLEAR;
	}
#endif

	if(EOL.F_CHIME_ACTIVATION_0x0509 == SET)
	{
		write_CHIME_ACTIVATION_0x0509(&DID_CHIME_ACTIVATION_0x0509_arr[0]);
		EOL.F_CHIME_ACTIVATION_0x0509 = CLEAR;
	}

	if(EOL.F_AVG_SPEED == SET)
	{
		write_AVG_SPEED(&DID_AVG_SPEED_0x0511_arr[0]);
		EOL.F_AVG_SPEED = CLEAR;
	}

	if(EOL.F_FUEL_DEAD_VOLUME == SET)
	{
		write_FUEL_DEAD_VOLUME(&DID_FUEL_DEAD_VOLUME_0x04D5_arr[0]);
		EOL.F_FUEL_DEAD_VOLUME = CLEAR;
	}

	if(EOL.F_NVM_SERVICE_DONE == SET)
	{
		write_NVM_SERVICE_DONE(&DID_NVM_SERVICE_DONE_0x04F3_arr[0]);
		EOL.F_NVM_SERVICE_DONE = CLEAR;
	}

	if(EOL.F_ODOMETER_RAW == SET)
	{
		write_ODOMETER_RAW(&DID_ODOMETER_RAW_0x04F4_arr[0]);
		EOL.F_ODOMETER_RAW = CLEAR;
	}

	if(EOL.F_ODOMETER_PERCENTAGE == SET)
	{
		write_ODOMETER_PERCENTAGE(&DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0]);
		EOL.F_ODOMETER_PERCENTAGE = CLEAR;
	}

	if(EOL.F_TELL_TALE == SET)
	{
		Write_TELL_TALE_0x04F8(&DID_TELL_TALE_0x04F8_arr[0]);
		EOL.F_TELL_TALE = CLEAR;
	}
	//ETM
	if(EOL.F_ETM_CONFIG == SET)
	{
		Write_ETM_CONFIG(&DID_ETM_0x0500_arr[0]);
		EOL.F_ETM_CONFIG = CLEAR;
	}
#if 0

	if(EOL.F_TELL_TALE_STATUS_SET2 == SET)
	{
		write_TELL_TALE_STATUS_SET2(&DID_TELL_TALE_STATUS_SET2_0xF080_arr[0]);
		EOL.F_TELL_TALE_STATUS_SET2 = CLEAR;
	}

	if(EOL.F_TELL_TALE_STATUS_SET3 == SET)
	{
		write_TELL_TALE_STATUS_SET3(&DID_TELL_TALE_STATUS_SET3_0xF081_arr[0]);
		EOL.F_TELL_TALE_STATUS_SET3 = CLEAR;
	}

	if(EOL.F_TELL_TALE_STATUS_SET4 == SET)
	{
		write_TELL_TALE_STATUS_SET4(&DID_TELL_TALE_STATUS_SET4_0xF08E_arr[0]);
		EOL.F_TELL_TALE_STATUS_SET4 = CLEAR;
	}

	if(EOL.F_TELL_TALE_STATUS_SET5 == SET)
	{
		write_TELL_TALE_STATUS_SET5(&DID_TELL_TALE_STATUS_SET5_0xF08F_arr[0]);
		EOL.F_TELL_TALE_STATUS_SET5 = CLEAR;
	}

#endif
	if(EOL.F_ODOMETER_CONFIG_0xF1B0 == SET)
	{
		write_ODOMETER_CONFIG_0xF1B0(&DID_ODOMETER_0xF1B0_arr[0]);
		EOL.F_ODOMETER_CONFIG_0xF1B0 = CLEAR;
	}

#if 0
	if(EOL.F_BAR_GRAPH_STATUS_CONFIG_0xF083 == SET)
	{
		write_BAR_GRAPH_STATUS_CONFIG_0xF083(&DID_BAR_GRAPH_STATUS_0xF083_arr[0]);
		EOL.F_BAR_GRAPH_STATUS_CONFIG_0xF083 = CLEAR;
	}

	if(EOL.F_GEAR_INDICATION_STATUS_CONFIG_0xF085 == SET)
	{
		write_GEAR_INDICATION_STATUS_CONFIG_0xF085(&DID_GEAR_INDICATION_STATUS_0xF085_arr[0]);
		EOL.F_GEAR_INDICATION_STATUS_CONFIG_0xF085 = CLEAR;
	}


	if(EOL.F_TELL_TALE_STATUS_SET6_CONFIG_0xF096 == SET)
	{
		write_TELL_TALE_STATUS_SET6_CONFIG_0xF096(&DID_TELL_TALE_STATUS_SET6_0xF096_arr[0]);
		EOL.F_TELL_TALE_STATUS_SET6_CONFIG_0xF096 = CLEAR;
	}

	if(EOL.F_BATTERY_VOLTAGE_CONFIG_0xF1B1 == SET)
	{
		write_BATTERY_VOLTAGE_CONFIG_0xF1B1(&DID_BATTERY_VOLTAGE_0xF1B1_arr[0]);
		EOL.F_BATTERY_VOLTAGE_CONFIG_0xF1B1 = CLEAR;
	}

	if(EOL.F_VEHICLE_SPEED_CONFIG_0xF1B2 == SET)
	{
		write_VEHICLE_SPEED_CONFIG_0xF1B2(&DID_BATTERY_VOLTAGE_0xF1B1_arr[0]);
		EOL.F_VEHICLE_SPEED_CONFIG_0xF1B2 = CLEAR;
	}

	if(EOL.F_ENGINE_SPEED_CONFIG_0xF1B3 == SET)
	{
		write_ENGINE_SPEED_CONFIG_0xF1B3(&DID_ENGINE_SPEED_0xF1B3_arr[0]);
		EOL.F_ENGINE_SPEED_CONFIG_0xF1B3 = CLEAR;
	}

	if(EOL.F_RESET_COUNTER_CONFIG_0xF1B5 == SET)
	{
		write_RESET_COUNTER_CONFIG_0xF1B5(&DID_RESET_COUNTER_0xF1B5_arr[0]);
		EOL.F_RESET_COUNTER_CONFIG_0xF1B5 = CLEAR;
	}

	if(EOL.F_IGNITION_COUNTER_CONFIG_0xF1B6 == SET)
	{
		write_IGNITION_COUNTER_CONFIG_0xF1B6(&DID_IGNITION_COUNTER_0xF1B6_arr[0]);
		EOL.F_IGNITION_COUNTER_CONFIG_0xF1B6 = CLEAR;
	}
#endif
	if(EOL.F_CHIMES_CONFIG == SET)
	{
		write_CHIMES_CONFIG(&DID_CHIMES_CONFIG_0x0503_arr[0]);
		EOL.F_CHIMES_CONFIG = CLEAR;
	}

	if(EOL.F_MESSAGE_CONFIG == SET)
	{
		write_F_MESSAGE_CONFIG(&DID_MESSAGE_CONFIG_0x0504_arr[0]);
		EOL.F_MESSAGE_CONFIG = CLEAR;
	}

	if(EOL.F_SEAT_BELT_REMAINDER == SET)
	{
		write_SEAT_BELT_REMAINDER(&DID_SEAT_BELT_REMAINDER_0x0507_arr[0]);
		EOL.F_SEAT_BELT_REMAINDER = CLEAR;
	}
	/*
	if(EOL.F_FUEL_BAR_VALUE_CONFIG == SET)
	{
		write_FUEL_BAR_VALUE_CONFIG(&DID_SEAT_BELT_REMAINDER_0x0507_arr[0]);
		EOL.F_FUEL_BAR_VALUE_CONFIG = CLEAR;
	}
	 */
	if(EOL.F_TIRE_DIRE_MNGT == SET)
	{
		write_F_TIRE_DIRE_MNGT(&DID_TIRE_DIRE_MNGT_0x0510_arr[0]);
		EOL.F_TIRE_DIRE_MNGT = CLEAR;
	}

	if(EOL.F_IFE_ECUIdentifier_0x0520 == SET)
	{
		write_IFE_ECUIdentifier_0x0520(&DID_IFE_ECUIdentifier_0x0520_arr[0]);
		EOL.F_IFE_ECUIdentifier_0x0520 = CLEAR;
	}
#if 0
	if(EOL.F_OSEK_NM == SET)
	{
		write_F_OSEK_NM(&DID_OSEK_NM_0x0521_arr[0]);
		EOL.F_OSEK_NM = CLEAR;
	}
#endif

	if(EOL.F_RPAS_MSG_STAYBACK_TIME == SET)
	{
		write_RPAS_MSG_STAYBACK_TIME(&DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0]);
		EOL.F_RPAS_MSG_STAYBACK_TIME = CLEAR;
	}

#if 0
	if(EOL.F_AFE_GD_CONFIG_0x0524 == SET)
	{
		write_AFE_GD_CONFIG_0x0524(&DID_AFE_GD_ECUIdentifier_0x0524_arr[0]);
		EOL.F_AFE_GD_CONFIG_0x0524 = CLEAR;
	}
#endif

	if(EOL.F_AFE_ECUIdentifier_0x0525 == SET)
	{
		write_AFE_ECUIdentifier_0x0525(&DID_AFE_ECUIdentifier_0x0525_arr[0]);
		EOL.F_AFE_ECUIdentifier_0x0525 = CLEAR;
	}

	if(EOL.F_SERVICE_DIST_SAP_CONFIG == SET)
	{
		write_SERVICE_DIST_SAP_CONFIG(&DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0]);
		EOL.F_SERVICE_DIST_SAP_CONFIG = CLEAR;
	}

	if(EOL.F_DTE_AFE_ECUIdentifier_0x04D2 == SET)
	{
		write_DTE_AFE_ECUIdentifier_0x04D2(&DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0]);
		EOL.F_DTE_AFE_ECUIdentifier_0x04D2 = CLEAR;
	}

	if(EOL.F_DTE_AFE_OVERSPEED == SET)
	{
		write_DTE_AFE_OVERSPEED(&DID_OVERSPEED_0x04F7_arr[0]);
		EOL.F_DTE_AFE_OVERSPEED = CLEAR;
	}
#if 0
	if(EOL.F_ILLUMINATION_STATUS_CONFIG_0xF084 == SET)
	{
		write_ILLUMINATION_STATUS_CONFIG_0xF084(&DID_ILLUMINATION_STATUS_0xF084_arr[0]);
		EOL.F_ILLUMINATION_STATUS_CONFIG_0xF084 = CLEAR;
	}
	/*
	if(EOL.F_RESET_REASON_CONFIG_0xF1B4 == SET)
	{
	write_RESET_REASON_CONFIG_0xF1B4(&DID_RESET_REASON_CONFIG_0xF1B4_arr[0]);
		EOL.F_RESET_REASON_CONFIG_0xF1B4 = CLEAR;
	}
	 */

	if(EOL.F_RESET_COUNTER_CONFIG_0xF1B7 == SET)
	{
		write_RESET_COUNTER_CONFIG_0xF1B7(&DID_EEPROM_LAYOUT_0xF1B7_arr[0]);
		EOL.F_RESET_COUNTER_CONFIG_0xF1B7 = CLEAR;
	}
#endif

	if(EOL.F_TELL_TALE_BLINK_PAT3 == SET)
	{
		write_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(&DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0]);
		EOL.F_TELL_TALE_BLINK_PAT3 = CLEAR;
	}
	
	if(EOL.F_TELL_TALE_BLINK_PAT4 == SET)
	{
		write_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(&DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0]);
		EOL.F_TELL_TALE_BLINK_PAT4 = CLEAR;
	}
	
	if(EOL.F_TELL_TALE_BLINK_PAT5 == SET)
	{
		write_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(&DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0]);
		EOL.F_TELL_TALE_BLINK_PAT5 = CLEAR;
	}
	
	if(EOL.F_TELL_TALE_BLINK_PAT8 == SET)
	{
		write_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(&DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0]);
		EOL.F_TELL_TALE_BLINK_PAT8 = CLEAR;
	}
	
	if(EOL.F_TELL_TALE_BLINK_PAT9 == SET)
	{
		write_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(&DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0]);
		EOL.F_TELL_TALE_BLINK_PAT9 = CLEAR;
	}
	
	if(EOL.F_ROLL_PITCH_DET_0x0533 == SET)
	{
		write_Roll_Pitch_Detection_0x0533_DATA(&DID_Roll_Pitch_Detection_0x0533_arr[0]);
		EOL.F_ROLL_PITCH_DET_0x0533 = CLEAR;
	}
#ifdef SPRINT4	
	if(EOL.F_DID_AT_GEAR_ALERT_0x0543 == SET)
	{
		write_AT_GEAR_ALERT_0x0543_DATA(&DID_AT_GEAR_ALERT_0x0543_arr[0]);
		EOL.F_DID_AT_GEAR_ALERT_0x0543 = CLEAR;
	}
#endif	
	
	if(EOL.F_NS_NW_Mgmt_Config_0x0103 == SET)
	{
		write_NS_NM_Configuration_0x0103(&NS_NM_Configuration_0x0103_arr[0]);
		EOL.F_NS_NW_Mgmt_Config_0x0103 = CLEAR;
	}
	
	if(EOL.F_NS_CAN_WakeUp_Config_0x0104 == SET)
	{
		write_NS_Can_Wakeup_Configuration_0x0104(&NS_Can_Wakeup_Configuration_0x0104_arr[0]);
		EOL.F_NS_CAN_WakeUp_Config_0x0104 = CLEAR;
	}
	
	if(EOL.F_NS_CAN_WU_Config_PKE_0x010D == SET)
	{
		write_NS_CAN_WEKUP_Configuration_PKE_0x010D(&NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0]);
		EOL.F_NS_CAN_WU_Config_PKE_0x010D = CLEAR;
	}
	
	if(EOL.F_NS_Network_MSG_Config_0x010E == SET)
	{
		write_NS_NW_Msg_Config_0x010E(&NS_NW_Msg_config_0x010E_arr[0]);
		EOL.F_NS_Network_MSG_Config_0x010E = CLEAR;
	}
}


/***********************************************************************************************************************
 * Function Name: Read_UDS_EEPROM_Data
 * Description  : This function is used to read UDS data to EEPROM.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
PUBLIC void Read_UDS_EEPROM_Data(void)
{
/*Need to varify before commite*/
	uint8_t read_para;
	read_para = eeprom_read(SET_DEFAULT_EEPROM_FLAG_OFFSET);
	if(read_para == 1)
	{
		write_Default_PARA();
		eeprom_writebt(SET_DEFAULT_EEPROM_FLAG_OFFSET,0);
	}
/*Need to varify before commite*/	
	read_history_zone(&History_zone_Full[0]);
	read_vin_zone(&VIN_arr[0]);
	read_vms1_zone(&Vms_1_RW_arr[0]);
	read_programming_date_zone(&Programming_date_arr[0]);
	read_model_number(&Model_number_arr[0]);
	read_ecu_installation_date(&Ecu_installation_date_arr[0]);
	//read_ecu_manf_date(&Manufacturing_date_arr[0]);  //commented till EOL parameter is defined
	read_engine_name_type(&Engine_type_arr[0]);
	read_tester_serial_number(&tester_serial_number_arr[0]);
	read_feature_code(&Feature_code_arr[0]);
	read_varient_code(&Varient_code_arr[0]);
	read_NS_varient_code(&NS_Varient_code_arr[0]);
	//read_Ecu_serial_number(&Ecu_serial_number_arr[0]);   //commented till EOL parameter is defined

	read_FUEL_BAR_VALUE(&DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0]);
	read_TEMP_BAR_VALUE(&DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0]);
	read_FUEL_TT_VALUE(&DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0]);
	read_TEMP_TT_VALUE_CONFIG(&DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0]);
	read_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(&DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0]);
	read_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(&DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0]);
	read_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(&DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0]);
	read_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(&DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0]);
	read_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(&DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0]);
	
	/*Read only parameter */
	//read_SPAREPART_NUMBER( &Sparepart_number_arr[0]); //Commented till EOL parameter is defined
	read_APP_SOFTWARE_IDENTIFICATION( &App_software_Identification[0]);
	read_DATA_IDENTIFICATION( &App_data_identification_arr[0]);
	//read_Supplier_identification_arr( Supplier_identification_arr[0]); //commented till EOL parameter is defined
	//read_ECU_MANUFACTURING_DATE( &Manufacturing_date_arr[0]);  //commented till EOL parameter is defined
	//read_ECU_SERIAL_NUMBER( &Ecu_serial_number_arr[0]);  //commented till EOL parameter is defined
	read_ECU_HARDWARE_NUMBER( Ecu_hardware_number_arr[0]);
	//read_ECU_HARDWARE_VERSION_NUMBER( &Ecu_hardware_version_number_arr[0]);  //commented till EOL parameter is defined
	read_ECU_SOFTWARE_NUMBER( &Ecu_software_number_arr[0]);
	read_ECU_SOFTWARE_VERSION_NUMBER( &Ecu_software_version_number_arr[0]);
	read_SYSTEM_SUPPLIER_PART_NUMBER( &System_supplier_part_number_arr[0]);
	read_ACTIVE_DIAG_SESSION( &Active_diag_sesion_arr[0]);
	read_BOOT_FLAG( &Boot_flag_arr[0]);
	read_GRAPHIC_VERSION( &GRAPHIC_VERSION_NUMBER_ARR[0]);
	read_BOOT_SOFTWARE_IDENTIFICATION( &Boot_software_identification_arr[0]);
	read_NS_Fault_Type_Status_Info_Configuration_0x0106();
	read_NS_Fault_Msg_Details_0x0105();

	//Read DID
	//read_CHIME_ACTIVATION_0x0508();
	read_CHIME_ACTIVATION_0x0509();
	read_AVG_SPEED();
	read_FUEL_DEAD_VOLUME();
	read_NVM_SERVICE_DONE();
	read_ODOMETER_RAW();
	read_ODOMETER_PERCENTAGE();
	read_TELL_TALE_0x04F8();
	read_CHIMES_CONFIG();
	read_F_MESSAGE_CONFIG();
	read_SEAT_BELT_REMAINDER();
	read_F_TIRE_DIRE_MNGT();
	read_IFE_ECUIdentifier_0x0520();
	//read_F_OSEK_NM();
	read_RPAS_MSG_STAYBACK_TIME();
	read_AFE_ECUIdentifier_0x0525();
	read_SERVICE_DIST_SAP_CONFIG();
	read_DTE_AFE_ECUIdentifier_0x04D2();
	read_DTE_AFE_OVERSPEED();
	read_ODOMETER_CONFIG_0xF1B0();
	read_AFE_ECU_IDENTIFIER_0x0523_DATA();
	read_AFE_GD_ECU_IDENTIFIER_0x0524_DATA();
	read_TELL_TALE_FB_ADC_VALUE_0x04CF_DATA();
	read_DTE_ECUIdentifier_0x04D0_DATA();
	read_FUEL_GEAN_INDICATOR_0x04D1_DATA();
	read_AFE_ECUIdentifier_0x04F9_DATA();
	//read_TELL_TALE_STATUS_SET1_0xF07F_DATA();
	//read_TELL_TALE_STATUS_SET2_0xF080_DATA();
	//read_TELL_TALE_STATUS_SET3_0xF081_DATA();
	//read_TELL_TALE_STATUS_SET4_0xF08E_DATA();
	//read_TELL_TALE_STATUS_SET5_0xF08F_DATA();
	//read_GAUGES_0xF082_DATA();
	read_ILLUMINATION_STATUS_0xF084_DATA();
	read_BAR_GRAPH_STATUS_0xF083_DATA();
	read_GEAR_INDICATION_STATUS_0xF085_DATA();
	//read_TELL_TALE_STATUS_SET6_0xF096_DATA();
	read_BATTERY_VOLTAGE_0xF1B1_DATA();
	read_VEHICLE_SPEED_0xF1B2_DATA();
	read_ENGINE_SPEED_0xF1B3_DATA();
	read_RESET_REASON_0xF1B4_DATA();
	read_RESET_COUNTER_0xF1B5_DATA();
	read_IGNITION_COUNTER_0xF1B6_DATA();
	read_EEPROM_LAYOUT_0xF1B7_DATA();
	read_NS_Network_Security_key_SKC_0x0108();
	read_NS_Network_Security_key_SKC1_0x0109();
	read_NS_Network_Security_key_SKC2_0x010A();
	read_NS_Network_Security_Configuration_0x010B();
	read_NS_Can_Wakeup_Configuration_0x0104();
	read_NS_NM_Configuration_0x0103();
	read_NS_NW_Msg_Config_0x010E();
	read_NS_CAN_WEKUP_Configuration_PKE_0x010D();
	read_Roll_Pitch_Detection_0x0533_DATA();
	read_AT_GEAR_ALERT_0x0543_DATA();
	
	//ETM
	read_ETM_CONFIG();

	Load_NM_Configuration();

	//delay_time(6000);
	//delay_time(6000);
}

void Load_NM_Configuration(void)
{
	if(IVN_cfg_Msg_Timeout_Recovery_Threshold >= 10)
	{
		IVN_cfg_Msg_Timeout_Recovery_Threshold = IVN_cfg_Msg_Timeout_Recovery_Threshold - 2;
	}
	
	if(IVN_cfg_NA_Recovery_Threshold >= 10)
	{
		IVN_cfg_NA_Recovery_Threshold = IVN_cfg_NA_Recovery_Threshold - 2;
	}
	
    /* Load IVN Message time out and Node absent Threshold values for CAN Messages*/
    EMS36_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*1*1);
    EMS36_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*1*1);
    EMS36_10_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS1_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*1*1);
    EMS1_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*1*1);
    EMS1_10_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);
#ifdef SPRINT4
/*TDMS testing*/
    SAS1_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*1*1);
    SAS1_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*1*1);
    SAS1_10_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);
#endif

    /* Node Absent key Message - EMS*/
    EMS3_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*1*1);
    EMS3_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*1*1);
    EMS3_10_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    EMS8_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*1*1);
    EMS8_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*1*1);
    EMS8_10_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS4_20_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*2*1);
    EMS4_20_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*2*1);
    EMS4_20_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS29_100_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    EMS29_100_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    EMS29_100_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS6_500_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*50*1);
    EMS6_500_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*50*1);
    EMS6_500_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS12_200_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*20*1);
    EMS12_200_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*20*1);
    EMS12_200_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS5_500_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*50*1);
    EMS5_500_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*50*1);
    EMS5_500_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    EMS_NSM_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    EMS_NSM_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    EMS_NSM_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    /* Node Absent Key Message - ESC*/
    ESC5_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*1*1);
    ESC5_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*1*1);
    ESC5_10_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    ESC2_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*1*1);
    ESC2_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*1*1);
    ESC2_10_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    ESC12_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*1*1);
    ESC12_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*1*1);
    ESC12_10_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    ESC_NSM_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    ESC_NSM_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    ESC_NSM_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    /* Node Absent Key Message - TCU*/
    TCU5_10_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*1*1);
    TCU5_10_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*1*1);
    TCU5_10_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    TCU6_20_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*2*1);
    TCU6_20_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*2*1);
    TCU6_20_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    TCU_NSM_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    TCU_NSM_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    TCU_NSM_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    /* Node Absent Key Message - MBFM*/
    MBFM1_100_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*10*1);
    MBFM1_100_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*10*1);
    MBFM1_100_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    MBFM5_100_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    MBFM5_100_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    MBFM5_100_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    MBFM9_500_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*50*1);
    MBFM9_500_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*50*1);
    MBFM9_500_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    MBFM_PAS1_50_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*5*1);
    MBFM_PAS1_50_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*5*1);
    MBFM_PAS1_50_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    /* Node Absent Key Message - MBFM*/
    SRS1_20_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*2*1);
    SRS1_20_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*2*1);
    SRS1_20_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    /* Node Absent Key Message - MBFM*/
    PKE_ICU2_100_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*10*1);
    PKE_ICU2_100_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*10*1);
    PKE_ICU2_100_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    /* Node Absent Key Message - MBFM*/
    RPAS1_20_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*2*1);
    RPAS1_20_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*2*1);
    RPAS1_20_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    RPAS2_100_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    RPAS2_100_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    RPAS2_100_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    /* Node Absent Key Message - MBFM*/
    IS1_100_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_NA_Detection_Threshold*10*1);
    IS1_100_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_NA_Recovery_Threshold*10*1);
    IS1_100_Message.Message_Received_Threshold = (IVN_cfg_NA_Recovery_Threshold);

    IS3_500_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*50*1);
    IS3_500_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*50*1);
    IS3_500_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    IS_NSM_Message.Timeout_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Detection_Threshold*10*1);
    IS_NSM_Message.Recovery_Threshold = (uint32_t)(IVN_cfg_Msg_Timeout_Recovery_Threshold*10*1);
    IS_NSM_Message.Message_Received_Threshold = (IVN_cfg_Msg_Timeout_Recovery_Threshold);

    Message_Content_Failure_Threshold = IVN_cfg_Msg_Content_Error_Detection_Threshold;
    Message_Content_Recovery_Threshold = IVN_cfg_Msg_Content_Error_Recovery_Threshold;
    Signal_Content_Failure_Threshold = IVN_cfg_NA_Signal_Content_Fail_Detect_Count;
    Signal_Content_Recovery_Threshold = IVN_cfg_NA_Signal_Content_Recov_Detect_Count;
}

/* **********************************************************************************************************************
 * Function Name: write_history_zone
 * Description  : This function writes history zone .
 * Arguments    : *history_data
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void write_history_zone(uint8_t *history_data)
{
	uint8_t write_node_index = 0;

	//   write_node_index = eeprom_read(HISTORY_ZONE_INDEX_OFFSET); w501
	write_node_index++;
	if((write_node_index > 5) || (write_node_index == 0))
	{
		write_node_index = SET;
	}
	/*       eeprom_memwrite_32bytes((HISTORY_ZONE_OFFSET1 + ((write_node_index - 1) * 32)), history_data, HISTORY_ZONE_RW_ARRAY_SIZE);

         eeprom_write(HISTORY_ZONE_INDEX_OFFSET, write_node_index); */ //w501
}

/* **********************************************************************************************************************
 * Function Name: read_history_zone
 * Description  : This function reads history zone .
 * Arguments    : *history_data
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void read_history_zone(uint8_t *history_data) //,uint8_t history_zone_no)
{
	uint8_t read_node_index = 0,counter ;
	uint8_t i = 0, j = 0;

	//RESET_WDT; w501
	read_node_index = eeprom_read(HISTORY_ZONE_INDEX_OFFSET);

	for(counter = 0; counter < 5; counter++)
	{
		read_node_index++;
		if((read_node_index == CLEAR) || (read_node_index > 5))
		{
			read_node_index = 1;
		}

		switch(read_node_index)
		{
		case 1:
			for(i = 0; i < HISTORY_ZONE_RW_ARRAY_SIZE; i++)
			{
				history_data[j] = eeprom_read(HISTORY_ZONE_OFFSET1 + i);
				j++;
			}
			break;

		case 2:
			for(i = 0; i < HISTORY_ZONE_RW_ARRAY_SIZE; i++)
			{
				history_data[j] = eeprom_read(HISTORY_ZONE_OFFSET2 + i);
				j++;
			}
			break;

		case 3:
			for(i = 0; i < HISTORY_ZONE_RW_ARRAY_SIZE; i++)
			{
				history_data[j] = eeprom_read(HISTORY_ZONE_OFFSET3 + i);
				j++;
			}
			break;

		case 4:
			for(i = 0; i < HISTORY_ZONE_RW_ARRAY_SIZE; i++)
			{
				history_data[j] = eeprom_read(HISTORY_ZONE_OFFSET4 + i);
				j++;
			}
			break;

		case 5:
			for(i = 0; i < HISTORY_ZONE_RW_ARRAY_SIZE; i++)
			{
				history_data[j] = eeprom_read(HISTORY_ZONE_OFFSET5 + i);
				j++;
			}
			break;

		default:
			break;
		}

	}
}

/* **********************************************************************************************************************
 * Function Name: write_vin_zone
 * Description  : This function writes vin_zone.
 * Arguments    : *vin_data
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_vin_zone(uint8_t *vin_data)
{
	New_eeprom_memwrite_32bytes(VIN_ZONE_OFFSET, vin_data, VIN_ZONE_RW_ARRAY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_vin_zone
 * Description  : This function reads vin_zone.
 * Arguments    : *vin_data
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_vin_zone(uint8_t *vin_data)
{
	uint8_t i = 0;

	for(i = 0; i < VIN_ZONE_RW_ARRAY_SIZE; i++)
	{
		vin_data[i] = eeprom_read(VIN_ZONE_OFFSET + i);
	}

	/*for(i = 0; i < VIN_ZONE_RW_ARRAY_SIZE;i++)
	{
	    if(vin_data[i] != 0xFF)
	    {
		break;
	    }

	    if(i == (VIN_ZONE_RW_ARRAY_SIZE - 1))
	    {
		memset(vin_data, 0x39, VIN_ZONE_RW_ARRAY_SIZE);
	    }
	}*/
}

/* **********************************************************************************************************************
 * Function Name: write_programming_date_zone
 * Description  : This function writes programmimng data.
 * Arguments    : *date
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_programming_date_zone(uint8_t *date)
{
	New_eeprom_memwrite_32bytes(PROG_DATE_OFFSET, date, PROGRAMMING_DATE_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_programming_date_zone
 * Description  : This function reads programming data vin_zone.
 * Arguments    : *date
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_programming_date_zone(uint8_t *date)
{
	uint8_t i = 0;

	for(i = 0; i < PROGRAMMING_DATE_ARR_SIZE; i++)
	{
		date[i] = eeprom_read(PROG_DATE_OFFSET + i);
	}

	/*for(i = 0; i < PROGRAMMING_DATE_ARR_SIZE;i++)
	{
	    if(date[i] != 0xFF)
	    {
		break;
	    }

	    if(i == (PROGRAMMING_DATE_ARR_SIZE - 1))
	    {
		memset(date, 0, PROGRAMMING_DATE_ARR_SIZE);
	    }
	}*/
}

/* **********************************************************************************************************************
 * Function Name: write_vms1_zone
 * Description  : This function writes vms1 data.
 * Arguments    : *vms1_data
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_vms1_zone(uint8_t *vms1_data)
{
	//       eeprom_memwrite_32bytes(VMS1_ZONE_OFFSET, vms1_data, VMS_1_RW_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_vms1_zone
 * Description  : This function reads vms1 data.
 * Arguments    : *vms1_data
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_vms1_zone(uint8_t *vms1_data)
{
	uint8_t i = 0;

	for(i = 0; i < VMS_1_RW_ARR_SIZE; i++)
	{
		vms1_data[i] = eeprom_read(VMS_ZONE_OFFSET + i);
	}
#if 0
	IGN_ON_MSG_START	   = vms1_data[0];
	CONF_MSG_TIMEOUT	   = vms1_data[1];
	CONF_HEAL_MSG_TIMEOUT      = vms1_data[2];
	CONF_KEY_MSG_ABSENT	   = vms1_data[3];
	CONF_QUICK_RECOV_TIME	   = vms1_data[4];
	CONF_HEAL_NODE_ABSENT	   = vms1_data[5];
	CONF_MSG_CONTENT_FAIL	   = vms1_data[6];
	CONF_HEAL_MSG_CONTENT_FAIL = vms1_data[7];
#endif
}
#if 0
/* **********************************************************************************************************************
 * Function Name: write_vms2_zone
 * Description  : This function writes vms2 data.
 * Arguments    : *vms2_data
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_vms2_zone(uint8_t *vms2_data)
{
	New_eeprom_memwrite_32bytes(VMS2_ZONE_OFFSET, vms2_data, VMS_2_RW_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_vms2_zone
 * Description  : This function reads vms2 data.
 * Arguments    : *vms2_data
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_vms2_zone(uint8_t *vms2_data)
{
	uint8_t i = 0;

	for(i = 0; i < VMS_2_RW_ARR_SIZE; i++)
	{
		vms2_data[i] = eeprom_read(VMS2_ZONE_OFFSET + i);
	}

	CONF_TIME_SLOW_RECOVERY       = vms2_data[0]; /*Time for which CAN communication to be passed during slow recovery mode (tR2)*/
	CONF_ATTEMPT_RECV_BUSOFF      = vms2_data[1]; /*No of attempts to recover from Bus Off in Quick recovery mode*/
	CONF_TIME_RECV_BUSOFF         = vms2_data[2];  /*  Time period to start monitoring messages upon returning to normal voltage from Bus Off*/
	CONF_TIME_UNDERV_TO_NORMALV   = vms2_data[3];   /* Time period to start monitoring messages upon returning to normal voltage from an under voltage condition*/
	CONF_TIME_OVERV_TO_NORMALV    = vms2_data[4];   /* Time period to start monitoring messages upon returning to normal voltage from an over voltage condition*/
	CONF_SUCCESS_DRIVE_NORMAL_ECU = vms2_data[5];  /*No of successive drive cycles with normal ECU  communication*/
	//STminval = vms2_data[6];
	//STmin_cntr = vms2_data[7];
}
#endif
/* **********************************************************************************************************************
 * Function Name: write_model_number
 * Description  : This function writes model number.
 * Arguments    : *model_number
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_model_number(uint8_t *model_number)
{
	New_eeprom_memwrite_32bytes(MODEL_NUMBER_OFFSET, model_number, MODEL_NUMBER_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_model_number
 * Description  : This function reads model number.
 * Arguments    : *model_number
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_model_number(uint8_t *model_number)
{
	uint8_t i = 0;

	for(i = 0; i < MODEL_NUMBER_ARR_SIZE; i++)
	{
		model_number[i] = eeprom_read(MODEL_NUMBER_OFFSET + i);
	}
	/*for(i = 0; i < MODEL_NUMBER_ARR_SIZE;i++)
	{
	    if(model_number[i] != 0xFF)
	    {
		break;
	    }

	    if(i == (MODEL_NUMBER_ARR_SIZE - 1))
	    {
		memset(model_number, 0, MODEL_NUMBER_ARR_SIZE);
	    }
	}*/
}

/* **********************************************************************************************************************
 * Function Name: write_ecu_installation_date
 * Description  : This function writes ecu installation data.
 * Arguments    : *ecu_date
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_ecu_installation_date(uint8_t *ecu_date)
{
	New_eeprom_memwrite_32bytes(ECU_INSTAL_DATE_OFFSET, ecu_date, ECU_INSTALLATION_DATE_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_ecu_installation_date
 * Description  : This function reads ecu installation data.
 * Arguments    : *ecu_date
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ecu_installation_date(uint8_t *ecu_date)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_INSTALLATION_DATE_ARR_SIZE; i++)
	{
		ecu_date[i] = eeprom_read(ECU_INSTAL_DATE_OFFSET + i);
	}

	/*for(i = 0; i < ECU_INSTALLATION_DATE_ARR_SIZE;i++)
	{
	    if(ecu_date[i] != 0xFF)
	    {
		break;
	    }

	    if(i == (ECU_INSTALLATION_DATE_ARR_SIZE - 1))
	    {
		memset(ecu_date, 0, ECU_INSTALLATION_DATE_ARR_SIZE);
	    }
	}*/
}

/* **********************************************************************************************************************
 * Function Name: write_ecu_manf_date
 * Description  : This function writes ecu installation data.
 * Arguments    : *ecu_manf_date
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_ecu_manf_date(uint8_t *ecu_manf_date)
{
	New_eeprom_memwrite_32bytes(ECU_MANF_DATE_OFFSET, ecu_manf_date, MANUFACTURING_DATE_RW_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_ecu_manf_date
 * Description  : This function read ecu installation data.
 * Arguments    : *ecu_manf_date
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ecu_manf_date(uint8_t *ecu_manf_date)
{
	uint8_t i = 0;

	for(i = 0; i < MANUFACTURING_DATE_RW_ARR_SIZE; i++)
	{
		ecu_manf_date[i] = eeprom_read(ECU_MANF_DATE_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: write_engine_name_type
 * Description  : This function writes ecu engine information.
 * Arguments    : *eng_name_type
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_engine_name_type(uint8_t *eng_name_type)
{
	New_eeprom_memwrite_32bytes(ENGINE_NAME_TYPE_OFFSET, eng_name_type,16);
}

/* **********************************************************************************************************************
 * Function Name: read_engine_name_type
 * Description  : This function read ecu engine information.
 * Arguments    : *eng_name_type
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_engine_name_type(uint8_t *eng_name_type)
{
	uint8_t i = 0;

	for(i = 0; i < 16; i++)
	{
		eng_name_type[i] = eeprom_read(ENGINE_NAME_TYPE_OFFSET + i);
	}
	/*for(i = 0; i < 16;i++)
	{
	    if(eng_name_type[i] != 0xFF)
	    {
		break;
	    }

	    if(i == 15)
	    {
		memset(eng_name_type, 0, 16);
	    }
	}*/
}

/* **********************************************************************************************************************
 * Function Name: write_tester_serial_number
 * Description  : This function writes test data.
 * Arguments    : *test_number
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_tester_serial_number(uint8_t *test_number)
{
	New_eeprom_memwrite_32bytes(TESTER_NUMBER_OFFSET, test_number,TESTER_SR_NUMBER_RW_ARRAY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_tester_serial_number
 * Description  : This function reads test data.
 * Arguments    : *test_number
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_tester_serial_number(uint8_t *test_number)
{
	uint8_t i = 0;

	for(i = 0; i < TESTER_SR_NUMBER_RW_ARRAY_SIZE; i++)
	{
		test_number[i] = eeprom_read(TESTER_NUMBER_OFFSET + i);
	}

	/*for(i = 0; i < 16;i++)
	{
	    if(test_number[i] != 0xFF)
	    {
		break;
	    }

	    if(i == 15)
	    {
		memset(test_number, 0, TESTER_SR_NUMBER_RW_ARRAY_SIZE);
	    }
	}*/
}

/* **********************************************************************************************************************
 * Function Name: write_feature_code
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_feature_code(uint8_t *feature_code)
{
	New_eeprom_memwrite_32bytes(FEATURE_CODE_OFFSET, feature_code, FEATURE_CODE_RW_ARRAY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_feature_code
 * Description  : This function reads feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
void read_feature_code(uint8_t *feature_code)
{
	uint8_t i = 0;

	for(i = 0; i < FEATURE_CODE_RW_ARRAY_SIZE; i++)
	{
		feature_code[i] = eeprom_read(FEATURE_CODE_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: write_varient_code
 * Description  : This function writes varient code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_varient_code(uint8_t *varient_code)
{
	New_eeprom_memwrite_32bytes(VARIENT_CODE_OFFSET, varient_code, VARIENT_CODE_RW_ARRAY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function reads varient code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
void read_varient_code(uint8_t *varient_code)
{
	uint8_t i = 0;

	for(i = 0; i < VARIENT_CODE_RW_ARRAY_SIZE; i++)
	{
		varient_code[i] = eeprom_read(VARIENT_CODE_OFFSET + i);
	}
}


/* **********************************************************************************************************************
 * Function Name: write_network security variant code_code
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_varient_code(uint8_t *feature_code)
{
	//eeprom_memwrite_32bytes(Network_Security_Variant_code_0107_OFFSET, feature_code, NS_VARIANT_CODE_ARR_SIZE);

	uint8_t i = 0;

	for(i = 0; i < NS_VARIANT_CODE_ARR_SIZE; i++)
	{
		eeprom_write((Network_Security_Variant_code_0107_OFFSET+i),feature_code[i]);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_network security variant code_code
 * Description  : This function reads varient code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_varient_code(uint8_t *varient_code)
{
	uint8_t i = 0,def_var_cntr= 0;

	for(i = 0; i < NS_VARIANT_CODE_ARR_SIZE; i++)
	{
		varient_code[i] = eeprom_read(Network_Security_Variant_code_0107_OFFSET + i);
	}
	
	for(i = 0; i < NS_VARIANT_CODE_ARR_SIZE; i++)
	{
		if(varient_code[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == NS_VARIANT_CODE_ARR_SIZE)	/*Default value write*/
		{
			memset(varient_code, 0x30, NS_VARIANT_CODE_ARR_SIZE);				
		
			write_NS_varient_code(&varient_code[0]);
		}
	}
	IVN_cfg_NS_variant_code = (varient_code[0] & 0x01);
	IVN_cfg_NS_EMS  = ((varient_code[0] & 0x02)>>1);
	IVN_cfg_NS_ABS  = ((varient_code[0] & 0x04)>>2);
	IVN_cfg_NS_ESC  = ((varient_code[0] & 0x08)>>3);
	IVN_cfg_NS_SAS  = ((varient_code[0] & 0x20)>>5);
	IVN_cfg_NS_PKE  = ((varient_code[0] & 0x40)>>6);
	IVN_cfg_NS_ICU  = ((varient_code[0] & 0x80)>>7);
	IVN_cfg_NS_SRS  = (varient_code[1] & 0x01);
	IVN_cfg_NS_TCU  = ((varient_code[1] & 0x10)>>4);
	IVN_cfg_NS_IC  = (varient_code[3] & 0x01);
	IVN_cfg_NS_MBFM  = ((varient_code[3] & 0x02)>>1);
	IVN_cfg_NS_Infotainment_System  = ((varient_code[5] & 0x20)>>5);


}
/* **********************************************************************************************************************
 * Function Name: write_Ecu_serial_number
 * Description  : This function writes ECU serial number data.
 * Arguments    : *Ecu_serial_number_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_Ecu_serial_number(uint8_t *Ecu_serial_number_code)
{
	New_eeprom_memwrite_32bytes(ECU_SERIAL_NUMBER_CODE_OFFSET, Ecu_serial_number_code, ECU_SR_NUMBER_RW_ARRAY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_Ecu_serial_number
 * Description  : This function writes ECU serial number data.
 * Arguments    : *Ecu_serial_number_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_Ecu_serial_number(uint8_t *Ecu_serial_number_code)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_SR_NUMBER_RW_ARRAY_SIZE; i++)
	{
		Ecu_serial_number_code[i] = eeprom_read(ECU_SERIAL_NUMBER_CODE_OFFSET + i);
	}
}



/* **********************************************************************************************************************
 * Function Name: write_varient_code
 * Description  : This function writes varient code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_FUEL_BAR_VALUE(uint8_t *FUEL_BAR_VALUE)
{
	New_eeprom_memwrite_32bytes(DID_050B_OFFSET, DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr, ECUIdentifier_0x050B_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function reads varient code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_FUEL_BAR_VALUE(uint8_t *FUEL_BAR_VALUE)
{
	uint8_t i = 0;

	for(i = 0; i < ECUIdentifier_0x050B_ARRY_SIZE; i++)
	{
		DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[i] = eeprom_read(DID_050B_OFFSET + i);
	}
	Get_Default_ADCvalue();//r
}


/* **********************************************************************************************************************
 * Function Name: write_TEMP_BAR_VALUE
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TEMP_BAR_VALUE(uint8_t *TEMP_BAR_VALUE)
{
	New_eeprom_memwrite_32bytes(DID_050C_OFFSET, DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr, ECUIdentifier_0x050C_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TEMP_BAR_VALUE code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TEMP_BAR_VALUE(uint8_t *TEMP_BAR_VALUE)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x050C_ARRY_SIZE; i++)
	{
		DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[i] = eeprom_read(DID_050C_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x050C_ARRY_SIZE; i++)
	{
		if(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x050C_ARRY_SIZE)	/*Default value write*/
		{
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0] = 10+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[1] = 50+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[2] = 70+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[3] = 90+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[4] = 101+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[5] = 106+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[6] = 110+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[7] = 114+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[8] = 6+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[9] = 46+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[10] = 66+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[11] = 86+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[12] = 97+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[13] = 102+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[14] = 106+40;
			DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[15] = 110+40;
			write_TEMP_BAR_VALUE(&DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0]);

		}
	}
	
	Diag_RW_Temp_Bar1_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0];
	Diag_RW_Temp_Bar2_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[1];
	Diag_RW_Temp_Bar3_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[2];
	Diag_RW_Temp_Bar4_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[3];
	Diag_RW_Temp_Bar5_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[4];
	Diag_RW_Temp_Bar6_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[5];
	Diag_RW_Temp_Bar7_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[6];
	Diag_RW_Temp_Bar8_ON_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[7];
	Diag_RW_Temp_Bar1_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[8];
	Diag_RW_Temp_Bar2_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[9];
	Diag_RW_Temp_Bar3_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[10];
	Diag_RW_Temp_Bar4_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[11];
	Diag_RW_Temp_Bar5_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[12];
	Diag_RW_Temp_Bar6_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[13];
	Diag_RW_Temp_Bar7_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[14];
	Diag_RW_Temp_Bar8_OFF_value = DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[15];
}



/* **********************************************************************************************************************
 * Function Name: write_FUEL_TT_VALUE
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_FUEL_TT_VALUE(uint8_t *FUEL_TT_VALUE)
{
	New_eeprom_memwrite_32bytes(DID_050D_OFFSET, DID_FUEL_TT_VALUE_CONFIG_0x050D_arr, ECUIdentifier_0x050D_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TEMP_BAR_VALUE code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_FUEL_TT_VALUE(uint8_t *FUEL_TT_VALUE)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x050D_ARRY_SIZE; i++)
	{
		DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[i] = eeprom_read(DID_050D_OFFSET + i);
	}
	fuel_did();
	for(i = 0; i < ECUIdentifier_0x050D_ARRY_SIZE; i++)
	{
		if(DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x050D_ARRY_SIZE)	/*Default value write*/
		{
			DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0] = 106;
			DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[1] = 114;
			DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[2] = 122;
			DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[3] = 130;
	
			write_FUEL_TT_VALUE(&DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0]);
		}
	}
	Diag_RW_Fuel_TT_Continuous_OFF_Value = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0];
	Diag_RW_Fuel_TT_Continuous_ON_Value = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[1];
	Diag_RW_Fuel_TT_Blink_OFF_Value = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[2];
	Diag_RW_Fuel_TT_Blink_ON_Value = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[3];
}


/* **********************************************************************************************************************
 * Function Name: write_TEMP_TT_VALUE_CONFIG
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TEMP_TT_VALUE_CONFIG(uint8_t *TEMP_TT_VALUE_CONFIG)
{
	New_eeprom_memwrite_32bytes(DID_050E_OFFSET, DID_TEMP_TT_VALUE_CONFIG_0x050E_arr, ECUIdentifier_0x050E_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TEMP_BAR_VALUE code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TEMP_TT_VALUE_CONFIG(uint8_t *TEMP_TT_VALUE_CONFIG)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x050E_ARRY_SIZE; i++)
	{
		DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[i] = eeprom_read(DID_050E_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x050E_ARRY_SIZE; i++)
	{
		if(DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x050E_ARRY_SIZE)	/*Default value write*/
		{
			DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0] = 110+40;
			DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[1] = 114+40;
			DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[2] = 106+40;
			DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[3] = 110+40;

			write_TEMP_TT_VALUE_CONFIG(&DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0]);
		}
	}
	Diag_RW_Temp_TT_Continuous_OFF_Value = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0];
	Diag_RW_Temp_TT_Continuous_ON_Value = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[1];
	Diag_RW_Temp_TT_Blink_OFF_Value = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[2];
	Diag_RW_Temp_TT_Blink_ON_Value = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[3];
}
/* **********************************************************************************************************************
 * Function Name: write_TELL_TALE_BLINK_PATTERN3_0x0502_DATA
 * Description  : This function writes TT_BLINK_PATTERN3  code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(uint8_t *TT_BLINK_PATTERN3)
{
	New_eeprom_memwrite_32bytes(DID_0502_OFFSET, DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr, ECUIdentifier_0x0502_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TT_BLINK_PATTERN3 code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(uint8_t *TT_BLINK_PATTERN3)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0502_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[i] = eeprom_read(DID_0502_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x0502_ARRY_SIZE; i++)
	{
		if(DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0502_ARRY_SIZE)	/*Default value write*/
		{
			DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0] = 0x01;
			DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[1] = 0xF4;
			DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[2] = 0x01;
			DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[3] = 0xF4;
	
			write_TELL_TALE_BLINK_PATTERN3_0x0502_DATA(&DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0]);
		}
	}
	Diag_RW_ESC_ON_Time = (DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[1])| (DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0]<<8);
	Diag_RW_ESC_OFF_Time = (DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[3])|(DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[2]<<8);
}
/* **********************************************************************************************************************
 * Function Name: write_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA
 * Description  : This function writes TT_BLINK_PATTERN4  code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(uint8_t *TT_BLINK_PATTERN4)
{
	New_eeprom_memwrite_32bytes(DID_04FB_OFFSET, DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr, ECUIdentifier_0x04FB_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TT_BLINK_PATTERN4 code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(uint8_t *TT_BLINK_PATTERN4)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x04FB_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[i] = eeprom_read(DID_04FB_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x04FB_ARRY_SIZE; i++)
	{
		if(DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04FB_ARRY_SIZE)	/*Default value write*/
		{
			DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0] = 0x00;//0x50
			DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[1] = 0x32;
			DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[2] = 0x03;
			DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[3] = 0xE8;
			write_TELL_TALE_BLINK_PATTERN4_0x04FB_DATA(&DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0]);
		}
	}
	Diag_RW_TT_PATTERN4_Immo_ON_Time = (DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[1])| (DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0]<<8);
	Diag_RW_TT_PATTERN4_Immo_OFF_Time = (DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[3])|(DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[2]<<8);
}
/* **********************************************************************************************************************
 * Function Name: write_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA
 * Description  : This function writes TT_BLINK_PATTERN5  code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(uint8_t *TT_BLINK_PATTERN5)
{
	New_eeprom_memwrite_32bytes(DID_04FC_OFFSET, DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr, ECUIdentifier_0x04FC_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TT_BLINK_PATTERN5 code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(uint8_t *TT_BLINK_PATTERN5)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x04FC_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[i] = eeprom_read(DID_04FC_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x04FC_ARRY_SIZE; i++)
	{
		if(DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04FC_ARRY_SIZE)	/*Default value write*/
		{
			DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0] = 0x00;//0x80
			DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[1] = 0x50;
			DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[2] = 0x07;
			DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[3] = 0xD0;
			write_TELL_TALE_BLINK_PATTERN5_0x04FC_DATA(&DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0]);
		}
	}
	Diag_RW_TT_PATTERN5_Immo_ON_Time = (DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[1])| (DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0]<<8);
	Diag_RW_TT_PATTERN5_Immo_OFF_Time = (DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[3])|(DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[2]<<8);
}

/* **********************************************************************************************************************
 * Function Name: write_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA
 * Description  : This function writes TT_BLINK_PATTERN8  code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(uint8_t *TT_BLINK_PATTERN8)
{
	New_eeprom_memwrite_32bytes(DID_04FD_OFFSET, DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr, ECUIdentifier_0x04FD_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TT_BLINK_PATTERN8 code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(uint8_t *TT_BLINK_PATTERN8)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x04FD_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[i] = eeprom_read(DID_04FD_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x04FD_ARRY_SIZE; i++)
	{
		if(DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04FD_ARRY_SIZE)	/*Default value write*/
		{
			DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0] = 0x03;
			DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[1] = 0xE8;
			DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[2] = 0x03;
			DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[3] = 0xE8;
			write_TELL_TALE_BLINK_PATTERN8_0x04FD_DATA(&DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0]);
		}
	}
	Diag_RW_TT_PATTERN8_TPMS_ON_Time = (DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[1])| (DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0]<<8);
	Diag_RW_TT_PATTERN8_TPMS_OFF_Time = (DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[3])|(DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[2]<<8);
}

/* **********************************************************************************************************************
 * Function Name: write_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA
 * Description  : This function writes TT_BLINK_PATTERN9  code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(uint8_t *TT_BLINK_PATTERN9)
{
	New_eeprom_memwrite_32bytes(DID_04FE_OFFSET, DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr, ECUIdentifier_0x04FE_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_TT_BLINK_PATTERN9 code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(uint8_t *TT_BLINK_PATTERN9)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x04FE_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[i] = eeprom_read(DID_04FE_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x04FE_ARRY_SIZE; i++)
	{
		if(DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04FE_ARRY_SIZE)	/*Default value write*/
		{
			DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0] = 0x01;
			DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[1] = 0xF4;
			DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[2] = 0x01;
			DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[3] = 0xF4;
			write_TELL_TALE_BLINK_PATTERN9_0x04FE_DATA(&DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0]);
		}
	}
	Diag_RW_TT_PATTERN9_TPMS_ON_Time = (DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[1])| (DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0]<<8);
	Diag_RW_TT_PATTERN9_TPMS_OFF_Time = (DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[3])|(DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[2]<<8);
}
/****************************** Read only UDS parameter **********************************/

/* **********************************************************************************************************************
 * Function Name: read_BOOT_SOFTWARE_IDENTIFICATION
 * Description  : This function BOOT SOFTWARE_IDENTIFICATION data.
 * Arguments    : *SOFTWARE_IDENTIFICATION
 * Return Value : None
 ********************************************************************************************************************** */
PUBLIC void read_BOOT_SOFTWARE_IDENTIFICATION(uint8_t *SOFTWARE_IDENTIFICATION)
{
	uint8_t i = 0;

	for(i = 0; i < BOOT_SOFTWARE_IDENTIFICATION_ARR_SIZE; i++)
	{
		Boot_software_identification_arr[i] = eeprom_read(BOOT_SOFT_ID_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_SOFTWARE_IDENTIFICATION
 * Description  : This function SOFTWARE_IDENTIFICATION data.
 * Arguments    : *SOFTWARE_IDENTIFICATION
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_APP_SOFTWARE_IDENTIFICATION(uint8_t *APP_SOFTWARE_IDENTIFICATION)
{
	uint8_t i = 0;

	for(i = 0; i < DID_APP_SOFTWARE_IDENTIFICATION_SIZE; i++)
	{
		App_software_Identification[i] = eeprom_read(APP_SOFT_ID_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_DATA_IDENTIFICATION
 * Description  : This function Application IDENTIFICATION data.
 * Arguments    : *DATA_IDENTIFICATION
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_DATA_IDENTIFICATION(uint8_t *DATA_IDENTIFICATION)
{
	uint8_t i = 0;

	for(i = 0; i < APP_DATA_IDENTIFICATION_ARR_SIZE; i++)
	{
		App_data_identification_arr[i] = eeprom_read(APP_DATA_ID_OFFSET + i);
	}
}


/* **********************************************************************************************************************
 * Function Name: read_Supplier_identification_arr
 * Description  : This function SUPPLIER IDENTIFICATION number data.
 * Arguments    : *SUPPLIER_IDENTIFICATION
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_Supplier_identification_arr(uint8_t *SUPPLIER_IDENTIFICATION)
{
	uint8_t i = 0;

	for(i = 0; i < SUPPLIER_IDENTIFICATION_ARR_SIZE; i++)
	{
		Supplier_identification_arr[i] = eeprom_read(SYS_SUP_ID_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: write_System_Supplier_Identification
 * Description  : This function write_System_Supplier_Identification.
 * Arguments    : *System_Supplier_ID
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_System_Supplier_Identification(uint8_t *System_Supplier_ID)
{
	New_eeprom_memwrite_32bytes(SYS_SUP_ID_OFFSET, System_Supplier_ID, SYSTEM_SUPPLIER_PART_NUMBER_ARR_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_ECU_MANUFACTURING_DATE
 * Description  : This function ECU_MANUFACTURING_DATE.
 * Arguments    : *ECU_MANUFACTURING_DATE
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ECU_MANUFACTURING_DATE(uint8_t *ECU_MANUFACTURING_DATE)
{
	uint8_t i = 0;

	for(i = 0; i < MANUFACTURING_DATE_RW_ARR_SIZE; i++)
	{
		Manufacturing_date_arr[i] = eeprom_read(ECU_MANF_DATE_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_ECU_SERIAL_NUMBER
 * Description  : This function ECU SERIAL NUMBER data.
 * Arguments    : *ECU_SERIAL_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ECU_SERIAL_NUMBER(uint8_t *ECU_SERIAL_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_SERIAL_NUMBER_ARR_SIZE; i++)
	{
		Ecu_serial_number_arr[i] = eeprom_read(ECU_SERIAL_NUMBER_CODE_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_ECU_HARDWARE_NUMBER
 * Description  : This function ECU HARDWARE NUMBER data.
 * Arguments    : *ECU_HARDWARE_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ECU_HARDWARE_NUMBER(uint8_t *ECU_HARDWARE_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_HARDWARE_NUMBER_ARR_SIZE; i++)
	{
		Ecu_hardware_number_arr[i] = eeprom_read(SYS_SUP_ECU_HARD_NO_OFFSET + i);
	}
}


/* **********************************************************************************************************************
 * Function Name: read_ECU_HARDWARE_VERSION_NUMBER
 * Description  : This function  ECU HARDWARE VERSION NUMBER data.
 * Arguments    : *ECU_HARDWARE_VERSION_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ECU_HARDWARE_VERSION_NUMBER(uint8_t *ECU_HARDWARE_VERSION_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_HARDWARE_VERSION_NUMBER_ARR_SIZE; i++)
	{
		Ecu_hardware_version_number_arr[i] = eeprom_read(SYS_SUP_ECU_HARD_VER_NO_OFFSET + i);
	}
}


/* **********************************************************************************************************************
 * Function Name: read_ECU_SOFTWARE_NUMBER
 * Description  : This function  ECU SOFTWARE NUMBER data.
 * Arguments    : *ECU_SOFTWARE_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ECU_SOFTWARE_NUMBER(uint8_t *ECU_SOFTWARE_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_SOFTWARE_NUMBER_ARR_SIZE; i++)
	{
		Ecu_software_number_arr[i] = eeprom_read(SYS_SUP_ECU_SOFT_NO_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_ECU_SOFTWARE_NUMBER
 * Description  : This function ECU SOFTWARE VERSION NUMBER data.
 * Arguments    : *ECU_SOFTWARE_VERSION_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ECU_SOFTWARE_VERSION_NUMBER(uint8_t *ECU_SOFTWARE_VERSION_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < ECU_SOFTWARE_VERSION_NUMBER_ARR_SIZE; i++)
	{
		Ecu_software_version_number_arr[i] = eeprom_read(APP_SOFT_ID_OFFSET + i);
	}
}


/* **********************************************************************************************************************
 * Function Name: read_SYSTEM_SUPPLIER_PART_NUMBER
 * Description  : This function SYSTEM SUPPLIER PART NUMBER data.
 * Arguments    : *SYSTEM_SUPPLIER_PART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_SYSTEM_SUPPLIER_PART_NUMBER(uint8_t *SYSTEM_SUPPLIER_PART_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < SYSTEM_SUPPLIER_PART_NUMBER_ARR_SIZE; i++)
	{
		System_supplier_part_number_arr[i] = eeprom_read(SYS_SUP_PART_NO_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_ACTIVE_DIAG_SESSION
 * Description  : This function ACTIVE DIAG SESSION data.
 * Arguments    : *ACTIVE_DIAG_SESSION
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ACTIVE_DIAG_SESSION(uint8_t *ACTIVE_DIAG_SESSION)
{
	uint8_t i = 0;

	for(i = 0; i < ACTIVE_DIAG_SESSION_SIZE; i++)
	{
		Active_diag_sesion_arr[i] = eeprom_read(ACTIVE_DIAG_SESS_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_BOOT_FLAG
 * Description  : This function BOOT FLAG data.
 * Arguments    : *BOOT_FLAG
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_BOOT_FLAG(uint8_t *BOOT_FLAG)
{
	uint8_t i = 0;

	for(i = 0; i < BOOT_FLAG_ARR_SIZE; i++)
	{
		Boot_flag_arr[i] = eeprom_read(BOOT_FLAG_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_GRAPHIC_VERSION
 * Description  : This function GRAPHIC VERSION data.
 * Arguments    : *GRAPHIC_VERSION
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_GRAPHIC_VERSION(uint8_t *GRAPHIC_VERSION)
{
	uint8_t i = 0;

	for(i = 0; i < GRAPHIC_VERSION_ARRY_SIZE; i++)
	{
		GRAPHIC_VERSION_NUMBER_ARR[i] = eeprom_read(GRAPHICS_VERSION + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_SPAREPART_NUMBER(uint8_t *SPAREPART_NUMBER)
{
	uint8_t i = 0;

	for(i = 0; i < SPAREPART_NUMBER_ARR_SIZE; i++)
	{
		Sparepart_number_arr[i] = eeprom_read(SPARE_PART_NUMBER_OFFSET + i);
	}
}
#if 0
/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0508
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_CHIME_ACTIVATION_0x0508(uint8_t *CHIME_ACTIVATION)
{
	eeprom_write(DID_0508_OFFSET,CHIME_ACTIVATION[0]);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_CHIME_ACTIVATION_0x0508(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0508_ARRY_SIZE; i++)
	{
		DID_CHIME_ACTIVATION_0x0508_arr[i] = eeprom_read(DID_0508_OFFSET + i);
	}

	for(i = 0; i < ECUIdentifier_0x0508_ARRY_SIZE; i++)
	{
		if(DID_CHIME_ACTIVATION_0x0508_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0508_ARRY_SIZE)	/*Default value write*/
		{
			//NS_Network_Security_key_SKC2_0x010A_arr.u32 = 0xFF001020;

			DID_CHIME_ACTIVATION_0x0508_arr[0] = 2;
			write_CHIME_ACTIVATION_0x0508(&DID_CHIME_ACTIVATION_0x0508_arr[0]);
		}
	}
	Diag_RW_Speed_Min_for_Chime_activation = DID_CHIME_ACTIVATION_0x0508_arr[0];
}

#endif

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_CHIME_ACTIVATION_0x0509(uint8_t *CHIME_ACTIVATION9)
{
	eeprom_write(DID_0509_OFFSET, CHIME_ACTIVATION9[0]);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_CHIME_ACTIVATION_0x0509(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0509_ARRY_SIZE; i++)
	{
		DID_CHIME_ACTIVATION_0x0509_arr[i] = eeprom_read(DID_0509_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x0509_ARRY_SIZE; i++)
	{
		if(DID_CHIME_ACTIVATION_0x0509_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0509_ARRY_SIZE)	/*Default value write*/
		{
			//NS_Network_Security_key_SKC2_0x010A_arr.u32 = 0xFF001020;

			DID_CHIME_ACTIVATION_0x0509_arr[0] = 2;
			write_CHIME_ACTIVATION_0x0509(&DID_CHIME_ACTIVATION_0x0509_arr[0]);
		}
	}
	Diag_RW_Speed_Min_for_Chime_activation = DID_CHIME_ACTIVATION_0x0509_arr[0];
}


/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_AVG_SPEED(uint8_t *AVG_SPEED)
{
	New_eeprom_memwrite_32bytes(DID_0511_OFFSET, DID_AVG_SPEED_0x0511_arr, ECUIdentifier_0x0511_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_AVG_SPEED(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x0511_ARRY_SIZE; i++)
	{
		DID_AVG_SPEED_0x0511_arr[i] = eeprom_read(DID_0511_OFFSET + i);
	}

	for(i = 0; i < ECUIdentifier_0x0511_ARRY_SIZE; i++)
	{
		if(DID_AVG_SPEED_0x0511_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0511_ARRY_SIZE)	/*Default value write*/
		{
			DID_AVG_SPEED_0x0511_arr[0] = 100;				//AVS_INITIAL_SAMPLE_DISTANCE
			DID_AVG_SPEED_0x0511_arr[1] = 10;				//AVS_SAMPLE_TIME

			write_AVG_SPEED(&DID_AVG_SPEED_0x0511_arr[0]);
		}
	}
	 Diag_RW_AVS_Initial_Sample_Distance_Config = DID_AVG_SPEED_0x0511_arr[0]*10;
	 Diag_RW_AVS_SAMPLE_TIME = DID_AVG_SPEED_0x0511_arr[0];
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_FUEL_DEAD_VOLUME(uint8_t *FUEL_DEAD)
{
	//eeprom_write(DID_04D5_OFFSET, FUEL_DEAD[0]);
	eeprom_write(DID_04D5_OFFSET, DID_FUEL_DEAD_VOLUME_0x04D5_arr[0]);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_FUEL_DEAD_VOLUME(void)
{
	uint8_t i = 0,def_var_cntr = 0;


	for(i = 0; i < ECUIdentifier_0x04D5_ARRY_SIZE; i++)
	{
		DID_FUEL_DEAD_VOLUME_0x04D5_arr[i] = eeprom_read(DID_04D5_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x04D5_ARRY_SIZE; i++)
	{
		if(DID_FUEL_DEAD_VOLUME_0x04D5_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04D5_ARRY_SIZE)	/*Default value write*/
		{
			DID_FUEL_DEAD_VOLUME_0x04D5_arr[0] = 50;			

			write_FUEL_DEAD_VOLUME(&DID_FUEL_DEAD_VOLUME_0x04D5_arr[0]);
		}
	}
	 Diag_RW_Fuel_Dead_Volume = DID_FUEL_DEAD_VOLUME_0x04D5_arr[0]*100;
	 
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NVM_SERVICE_DONE(uint8_t *NVM_SERVICE)
{
	//eeprom_write(DID_04F3_OFFSET, NVM_SERVICE[0]);
	eeprom_write(DID_04F3_OFFSET, DID_NVM_SERVICE_DONE_0x04F3_arr[0]);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NVM_SERVICE_DONE(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x04F3_ARRY_SIZE; i++)
	{
		DID_NVM_SERVICE_DONE_0x04F3_arr[i] = eeprom_read(DID_04F3_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x04F3_ARRY_SIZE; i++)
	{
		if(DID_NVM_SERVICE_DONE_0x04F3_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04F3_ARRY_SIZE)	/*Default value write*/
		{
			DID_NVM_SERVICE_DONE_0x04F3_arr[0] = 1;			
		
			//New_eeprom_memwrite_32bytes(DID_0511_OFFSET, DID_AVG_SPEED_0x0511_arr, ECUIdentifier_0x0511_ARRY_SIZE);
			write_NVM_SERVICE_DONE(&DID_NVM_SERVICE_DONE_0x04F3_arr[0]);
		}
	}
	 Diag_RW_NVM_SERV_DONE = DID_NVM_SERVICE_DONE_0x04F3_arr[0];
	
	
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_ODOMETER_RAW(uint8_t *ODOMETER_RAW)
{
	//eeprom_write(DID_04F4_OFFSET, ODOMETER_RAW[0]);
	eeprom_write(DID_04F4_OFFSET, DID_ODOMETER_RAW_0x04F4_arr[0]);
}
/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ODOMETER_RAW(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x04F4_ARRY_SIZE; i++)
	{
		DID_ODOMETER_RAW_0x04F4_arr[i] = eeprom_read(DID_04F4_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x04F4_ARRY_SIZE; i++)
	{
		if(DID_ODOMETER_RAW_0x04F4_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04F4_ARRY_SIZE)	/*Default value write*/
		{
			DID_ODOMETER_RAW_0x04F4_arr[0] = 0;			
		
			write_ODOMETER_RAW(&DID_ODOMETER_RAW_0x04F4_arr[0]);
		}
	}
	 Diag_RW_ODO_RAW_DISPLAYABLE = DID_ODOMETER_RAW_0x04F4_arr[0];
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_ODOMETER_PERCENTAGE(uint8_t *ODOMETER_PERCENTAGE)
{
	//eeprom_write(DID_04F6_OFFSET, ODOMETER_PERCENTAGE[0]);
	eeprom_write(DID_04F6_OFFSET, DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0]);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ODOMETER_PERCENTAGE(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x04F6_ARRY_SIZE; i++)
	{
		DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[i] = eeprom_read(DID_04F6_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x04F6_ARRY_SIZE; i++)
	{
		if(DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04F6_ARRY_SIZE)	/*Default value write*/
		{
			DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0] = 13;			
	
			write_ODOMETER_PERCENTAGE(&DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0]);
		}
	}
	 Diag_RW_ODO_DISPLAYABEL_PERCENTAGE = DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0]*0.2;
}
/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void Write_TELL_TALE_0x04F8(uint8_t *TELL_TALE)
{
	//eeprom_write(DID_04F8_OFFSET, TELL_TALE[0]);
	eeprom_write(DID_04F8_OFFSET, DID_TELL_TALE_0x04F8_arr[0]);
}


/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_TELL_TALE_0x04F8(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x04F8_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_0x04F8_arr[i] = eeprom_read(DID_04F8_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x04F8_ARRY_SIZE; i++)
	{
		if(DID_TELL_TALE_0x04F8_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04F8_ARRY_SIZE)	/*Default value write*/
		{
			DID_TELL_TALE_0x04F8_arr[0] = 90;			
		
			Write_TELL_TALE_0x04F8(&DID_TELL_TALE_0x04F8_arr[0]);
		}
	}
	 Diag_RW_TPMS_Blink_Total_Duration_Setting = DID_TELL_TALE_0x04F8_arr[0];
}
/* **********************************************************************************************************************
 * Function Name: write_ETM_ACTIVATION_0x0500
 * Description  : This function writes ETM_config.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void Write_ETM_CONFIG(uint8_t *ETM_CONFIG)
{
	//eeprom_write(DID_0500_OFFSET, ETM_CONFIG[0]);
	eeprom_write(DID_0500_OFFSET, DID_ETM_0x0500_arr[0]);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ETM_CONFIG(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x0500_ARRY_SIZE; i++)
	{
		DID_ETM_0x0500_arr[i] = eeprom_read(DID_0500_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x0500_ARRY_SIZE; i++)
	{
		if(DID_ETM_0x0500_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0500_ARRY_SIZE)	/*Default value write*/
		{
			DID_ETM_0x0500_arr[0] = 0;			
		
			Write_ETM_CONFIG(&DID_ETM_0x0500_arr[0]);
		}
	}
	 Diag_RW_Engineering_Test_Mode = DID_ETM_0x0500_arr[0];
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_CHIMES_CONFIG(uint8_t *CHIMES_CONFIG)
{
	New_eeprom_memwrite_32bytes(DID_0503_OFFSET, DID_CHIMES_CONFIG_0x0503_arr, ECUIdentifier_0x0503_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_CHIMES_CONFIG(void)
{
	uint8_t i = 0;

	for(i = 0; i < ECUIdentifier_0x0503_ARRY_SIZE; i++)
	{
		DID_CHIMES_CONFIG_0x0503_arr[i] = eeprom_read(DID_0503_OFFSET + i);
	}
}
/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_F_MESSAGE_CONFIG(uint8_t *MESSAGE_CONFIG)
{
	New_eeprom_memwrite_32bytes(DID_0504_OFFSET, DID_MESSAGE_CONFIG_0x0504_arr, ECUIdentifier_0x0504_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_F_MESSAGE_CONFIG(void)
{
	uint8_t i = 0;

	for(i = 0; i < ECUIdentifier_0x0504_ARRY_SIZE; i++)
	{
		DID_MESSAGE_CONFIG_0x0504_arr[i] = eeprom_read(DID_0504_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_SEAT_BELT_REMAINDER(uint8_t *SEAT_BELT)
{
	New_eeprom_memwrite_32bytes(DID_0507_OFFSET, DID_SEAT_BELT_REMAINDER_0x0507_arr, ECUIdentifier_0x0507_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_SEAT_BELT_REMAINDER(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0507_ARRY_SIZE; i++)
	{
		DID_SEAT_BELT_REMAINDER_0x0507_arr[i] = eeprom_read(DID_0507_OFFSET + i);

	//	if(DID_SEAT_BELT_REMAINDER_0x0507_arr[2] == 0xFF)
	//	{
	//		DID_SEAT_BELT_REMAINDER_0x0507_arr[2]=2;
	//		Diag_RW_Min_VehSpeed_Threshold = 2;
	//	}
	//	else
	//	{
	//		Diag_RW_Min_VehSpeed_Threshold=DID_SEAT_BELT_REMAINDER_0x0507_arr[2];
	//	}
	}

	for(i = 0; i < ECUIdentifier_0x0507_ARRY_SIZE; i++)
	{
		if(DID_SEAT_BELT_REMAINDER_0x0507_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0507_ARRY_SIZE)	/*Default value write*/
		{
			//NS_Network_Security_key_SKC2_0x010A_arr.u32 = 0xFF001020;

			DID_SEAT_BELT_REMAINDER_0x0507_arr[0] = 200;
			DID_SEAT_BELT_REMAINDER_0x0507_arr[1] = 60;
			DID_SEAT_BELT_REMAINDER_0x0507_arr[2] = 2;
			DID_SEAT_BELT_REMAINDER_0x0507_arr[3] = 60;
			DID_SEAT_BELT_REMAINDER_0x0507_arr[4] = 50;
			DID_SEAT_BELT_REMAINDER_0x0507_arr[5] = 25;
			DID_SEAT_BELT_REMAINDER_0x0507_arr[6] = 35;
			write_SEAT_BELT_REMAINDER(&DID_SEAT_BELT_REMAINDER_0x0507_arr[0]);
		}
	}
	Diag_RW_Eng_Run_threshold_RPM = DID_SEAT_BELT_REMAINDER_0x0507_arr[0]*50;
	Diag_RW_Eng_Run_threshold_time = DID_SEAT_BELT_REMAINDER_0x0507_arr[1];
	Diag_RW_Min_VehSpeed_Threshold = DID_SEAT_BELT_REMAINDER_0x0507_arr[2];
	Diag_RW_Vehicle_Moving_threshold_time = DID_SEAT_BELT_REMAINDER_0x0507_arr[3];
	Diag_RW_Vehicle_Moving_threshold_distance = DID_SEAT_BELT_REMAINDER_0x0507_arr[4]*10;
	Diag_RW_Vehicle_Moving_final_speed = DID_SEAT_BELT_REMAINDER_0x0507_arr[5];
	Diag_RW_Final_chime_duration = DID_SEAT_BELT_REMAINDER_0x0507_arr[6];
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_FUEL_BAR_VALUE_CONFIG(uint8_t *FUEL_BAR)
{
	//New_eeprom_memwrite_32bytes(DID_050D_OFFSET, DID_FUEL_TT_VALUE_CONFIG_0x050D_arr, ECUIdentifier_0x050D_ARRY_SIZE);
}


/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_F_TIRE_DIRE_MNGT(uint8_t *TIRE_DIRE)
{
	New_eeprom_memwrite_32bytes(DID_0510_OFFSET, DID_TIRE_DIRE_MNGT_0x0510_arr, ECUIdentifier_0x0510_ARRY_SIZE);
}


/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_F_TIRE_DIRE_MNGT(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0510_ARRY_SIZE; i++)
	{
		DID_TIRE_DIRE_MNGT_0x0510_arr[i] = eeprom_read(DID_0510_OFFSET + i);
	}
	
	for(i = 0; i < ECUIdentifier_0x0510_ARRY_SIZE; i++)
	{
		if(DID_TIRE_DIRE_MNGT_0x0510_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0510_ARRY_SIZE)	/*Default value write*/
		{

			DID_TIRE_DIRE_MNGT_0x0510_arr[0] = 30;
			DID_TIRE_DIRE_MNGT_0x0510_arr[1] = 100;
			DID_TIRE_DIRE_MNGT_0x0510_arr[2] = 100;
			DID_TIRE_DIRE_MNGT_0x0510_arr[3] = 100;
			DID_TIRE_DIRE_MNGT_0x0510_arr[4] = 100;
			write_F_TIRE_DIRE_MNGT(&DID_TIRE_DIRE_MNGT_0x0510_arr[0]);
		}
	}
	Diag_RW_Level1_Threshold_Angle = DID_TIRE_DIRE_MNGT_0x0510_arr[0];
	Diag_RW_Level2_Threshold_Angle = DID_TIRE_DIRE_MNGT_0x0510_arr[1];
	Diag_RW_Level3_Threshold_Angle = DID_TIRE_DIRE_MNGT_0x0510_arr[2];
	Diag_RW_Level4_Threshold_Angle = DID_TIRE_DIRE_MNGT_0x0510_arr[3];
	Diag_RW_Level5_Threshold_Angle = DID_TIRE_DIRE_MNGT_0x0510_arr[4];
	
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_IFE_ECUIdentifier_0x0520(uint8_t *IFE_ECU)
{
	New_eeprom_memwrite_32bytes(DID_0520_OFFSET, DID_IFE_ECUIdentifier_0x0520_arr, ECUIdentifier_0x0520_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_IFE_ECUIdentifier_0x0520(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0520_ARRY_SIZE; i++)
	{
		DID_IFE_ECUIdentifier_0x0520_arr[i] = eeprom_read(DID_0520_OFFSET + i);
	}

	for(i = 0; i < ECUIdentifier_0x0520_ARRY_SIZE; i++)
	{
		if(DID_IFE_ECUIdentifier_0x0520_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0520_ARRY_SIZE)	/*Default value write*/
		{
			DID_IFE_ECUIdentifier_0x0520_arr[0] = 20;	//This is the initial threshold time after which the calculation of IFE will begin
			DID_IFE_ECUIdentifier_0x0520_arr[1] = 1;	//This is the interval at which the fuel consumption and distance covered samples are taken
			DID_IFE_ECUIdentifier_0x0520_arr[2] = 2;	//Size of the rolling average window
			DID_IFE_ECUIdentifier_0x0520_arr[3] = 1;	//This is the interval of the IFE display
			DID_IFE_ECUIdentifier_0x0520_arr[4] = 30;	//This is the Max IFE Display value.

			write_IFE_ECUIdentifier_0x0520(&DID_IFE_ECUIdentifier_0x0520_arr[0]);

		}
	}

	Diag_RW_IFE_Accum_Time = DID_IFE_ECUIdentifier_0x0520_arr[0] * 100;
	Diag_RW_IFE_Sample_Time = DID_IFE_ECUIdentifier_0x0520_arr[1] * 100;
	Diag_RW_IFE_Sample_Size = DID_IFE_ECUIdentifier_0x0520_arr[2];
	Diag_RW_IFE_Display_Time = DID_IFE_ECUIdentifier_0x0520_arr[3] * 50;
	Diag_RW_IFE_Units = 0;
	Diag_RW_IFE_Max = DID_IFE_ECUIdentifier_0x0520_arr[4];
}

#if 0
/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_F_OSEK_NM(uint8_t *OSEK_NM)
{
	New_eeprom_memwrite_32bytes(DID_0521_OFFSET, DID_OSEK_NM_0x0521_arr, ECUIdentifier_0x0521_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_F_OSEK_NM(void)
{
	uint8_t i = 0;

	for(i = 0; i < ECUIdentifier_0x0521_ARRY_SIZE; i++)
	{
		DID_OSEK_NM_0x0521_arr[i] = eeprom_read(DID_0521_OFFSET + i);
	}
}

#endif

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_RPAS_MSG_STAYBACK_TIME(uint8_t *RPAS_MSG)
{
	New_eeprom_memwrite_32bytes(DID_0522_OFFSET, DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr, ECUIdentifier_0x0522_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_RPAS_MSG_STAYBACK_TIME(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x0522_ARRY_SIZE; i++)
	{
		DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[i] = eeprom_read(DID_0522_OFFSET + i);
	}

	for(i = 0; i < ECUIdentifier_0x0522_ARRY_SIZE; i++)
	{
		if(DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0522_ARRY_SIZE)	/*Default value write*/
		{
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0] = 0;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[1] = 1;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[2] = 0;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[3] = 5;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[4] = 0;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[5] = 5;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[6] = 30;
			DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[7] = 10;

			write_RPAS_MSG_STAYBACK_TIME(&DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0]);

		}
	}

	Diag_RW_RPAS_Display_Screen_source = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0];
	Diag_RW_RPAS_Sector_Zone_Calibration = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[1];
	Diag_RW_Trajectory_Calibration = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[2];
	Diag_RW_AT_Reverse_Gear_Debounce_Time = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[3] * 100;
	Diag_RW_RPASStayBackTimeVal = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[4];		/*To specify the delay for closing the RPAS message from Reverse disengage*/
	Diag_RW_RPAS_self_check_chime_Enable = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[5] * 100;
	Diag_RW_RPAS_Error_Chime_Enable  = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[6] * 100;
	Diag_RW_Stop_Dist = DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[7] * 5;
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_AFE_ECUIdentifier_0x0525(uint8_t *AFE_ECUIdentifier)
{
	New_eeprom_memwrite_32bytes(DID_0525_OFFSET, DID_AFE_ECUIdentifier_0x0525_arr, ECUIdentifier_0x0525_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_AFE_ECUIdentifier_0x0525(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0525_ARRY_SIZE; i++)
	{
		DID_AFE_ECUIdentifier_0x0525_arr[i] = eeprom_read(DID_0525_OFFSET + i);
	}

	for(i = 0; i < ECUIdentifier_0x0525_ARRY_SIZE; i++)
	{
		if(DID_AFE_ECUIdentifier_0x0525_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0525_ARRY_SIZE)	/*Default value write*/
		{
			DID_AFE_ECUIdentifier_0x0525_arr[0] = 100;	// FC_Factor
			DID_AFE_ECUIdentifier_0x0525_arr[1] = 5;	// AFE_CALC_INTERVAL
			DID_AFE_ECUIdentifier_0x0525_arr[2] = 60;	// AFE_DISPLAY_INTERVAL
			DID_AFE_ECUIdentifier_0x0525_arr[3] = 10;	// AFE_DISPLAY_FILTER_A
  			DID_AFE_ECUIdentifier_0x0525_arr[4] = 50;//@/@*$5$*@/@;	// AFE_DISPLAY_FILTER_B 0 to 7 bits	@/@*$defult 5 - 50*0.1 = 5$*@/@
			DID_AFE_ECUIdentifier_0x0525_arr[5] = 0;	// AFE_DISPLAY_FILTER_B 8 to 15 bits
			DID_AFE_ECUIdentifier_0x0525_arr[6] = 0;	// AFE_DRIVE_CYCLE_RESET_ENABLE.
			DID_AFE_ECUIdentifier_0x0525_arr[7] = 2;	// AFE_DRIVE_CYCLE_RESET_INTERVAL.
			DID_AFE_ECUIdentifier_0x0525_arr[8] = 10;	// AFE_INIT_THRESHOLD_DISTANCE.

			write_AFE_ECUIdentifier_0x0525(&DID_AFE_ECUIdentifier_0x0525_arr);

		}
	}

	Fuel_Consp_Factor = (float)(DID_AFE_ECUIdentifier_0x0525_arr[0] * 0.01);
	AFE_CALC_INTERVAL = DID_AFE_ECUIdentifier_0x0525_arr[1];/**100;*/ // previous implementation multiple factor is not there.
	AFE_DISPLAY_INTERVAL = DID_AFE_ECUIdentifier_0x0525_arr[2];
	AFE_DISPLAY_FILTER_A = DID_AFE_ECUIdentifier_0x0525_arr[3]/**100*/;	/*multiplication removed*/
	AFE_DISPLAY_FILTER_B = (float)(( (DID_AFE_ECUIdentifier_0x0525_arr[5]) | (DID_AFE_ECUIdentifier_0x0525_arr[4] << 8) )*0.1);
	AFE_DRIVE_CYCLE_RESET_ENABLE = DID_AFE_ECUIdentifier_0x0525_arr[6];
	AFE_DRIVE_CYCLE_RESET_INTERVAL =DID_AFE_ECUIdentifier_0x0525_arr[7];
	AFE_INIT_THRESHOLD_DISTANCE = DID_AFE_ECUIdentifier_0x0525_arr[8]/**100*/;
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_SERVICE_DIST_SAP_CONFIG(uint8_t *SERVICE_DIST)
{
	New_eeprom_memwrite_32bytes(DID_050F_OFFSET, DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr, ECUIdentifier_0x050F_ARRY_SIZE);
	Get_Default_Service_Slab();
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_SERVICE_DIST_SAP_CONFIG(void)
{
/*	uint8_t i = 0;
	for(i = 0; i < ECUIdentifier_0x050F_ARRY_SIZE; i++)
	{
		DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[i] = eeprom_read(DID_050F_OFFSET + i);
	}
	Get_Default_Service_Slab();
*/
	{
		uint8_t i = 0,def_var_cntr = 0;

		for(i = 0; i < ECUIdentifier_0x050F_ARRY_SIZE; i++)
		{
			DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[i] = eeprom_read(DID_050F_OFFSET + i);
		}
		for(i = 0; i < ECUIdentifier_0x050F_ARRY_SIZE; i++)
		{
			if(DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[i] == 0xFF)
			{
				//def_var_cntr++;
			}
		}
			if(def_var_cntr == ECUIdentifier_0x050F_ARRY_SIZE)	/*Default value write*/
			{
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0] = 8;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[1] = 90;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[2] = 100;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[3] = 100;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[4] = 100;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[5] = 100;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[6] = 100;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[7] = 100;
				DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[8] = 50;

				write_SERVICE_DIST_SAP_CONFIG(&DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0]);
//				Get_Default_Service_Slab();
			}
			Get_Default_Service_Slab();
	}
}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_DTE_AFE_ECUIdentifier_0x04D2(uint8_t *DTE_AFE_ECU)
{
	New_eeprom_memwrite_32bytes(DID_04D2_OFFSET, DID_DTE_AFE_ECUIdentifier_0x04D2_arr, ECUIdentifier_0x04D2_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_DTE_AFE_ECUIdentifier_0x04D2(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x04D2_ARRY_SIZE; i++)
	{
		DID_DTE_AFE_ECUIdentifier_0x04D2_arr[i] = eeprom_read(DID_04D2_OFFSET + i);
	}

	for(i = 0; i < ECUIdentifier_0x04D2_ARRY_SIZE; i++)
	{
		if(DID_DTE_AFE_ECUIdentifier_0x04D2_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04D2_ARRY_SIZE)	/*Default value write*/
		{
			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0] = 50;	// DTE_AFE_AVG_KM
			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[1] = 70;	// DTE_DEFAULT_FE
			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[2] = 5;	// DTE_DISPLAY_FILTER_A
			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[3] = 20;	// DTE_DISPLAY_FILTER_B
			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[4] = 50;	// DTE_DISPLAY_FILTER_C
  			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[5] = 230;//@/@*$30$*@/@	// DTE_AFE_MAX - due to offset addition 70
  			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[6] = 0;//@/@*$7$*@/@;	// DTE_AFE_MIN - due to offset addition 70
			DID_DTE_AFE_ECUIdentifier_0x04D2_arr[7] = 0;	// Unit Selection

			write_DTE_AFE_ECUIdentifier_0x04D2(&DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0]);

		}
	}

	DTE_AFE_AVG_KM = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0];
	DTE_DEFAULT_FE = (DID_DTE_AFE_ECUIdentifier_0x04D2_arr[1]+70)*0.1; //Offset 70 added// multiple factor is ADD NOW
	DTE_DISPLAY_FILTER_A = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[2];
	DTE_DISPLAY_FILTER_B = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[3];
	DTE_DISPLAY_FILTER_C = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[4];
	DTE_AFE_MAX = (DID_DTE_AFE_ECUIdentifier_0x04D2_arr[5]+70)*0.1; /*70 offset added*/  // multiple factor is ADD NOW
	DTE_AFE_MIN = (DID_DTE_AFE_ECUIdentifier_0x04D2_arr[6]+70)*0.1; /*70 offset added*/  // multiple factor is ADD NOW
	Diag_Unit_Selection = DID_DTE_AFE_ECUIdentifier_0x04D2_arr[7];
}


/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_DTE_AFE_OVERSPEED(uint8_t *DTE_AFE)
{
	New_eeprom_memwrite_32bytes(DID_04F7_OFFSET, DID_OVERSPEED_0x04F7_arr, ECUIdentifier_0x04F7_ARRY_SIZE);
}


/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_DTE_AFE_OVERSPEED(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0x04F7_ARRY_SIZE; i++)
	{
		DID_OVERSPEED_0x04F7_arr[i] = eeprom_read(DID_04F7_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x04F7_ARRY_SIZE; i++)
	{
		if(DID_OVERSPEED_0x04F7_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x04F7_ARRY_SIZE)	/*Default value write*/
		{
			DID_OVERSPEED_0x04F7_arr[0] = 80;	//OverSpeedLevel1Val
			DID_OVERSPEED_0x04F7_arr[1] = 120;	//OverSpeedLevel2Val
			DID_OVERSPEED_0x04F7_arr[2] = 1;	//OverSpeedLevel1Type
			DID_OVERSPEED_0x04F7_arr[3] = 60;	//OverSpeedLv1RepeatIntrval
			DID_OVERSPEED_0x04F7_arr[4] = 2;	//OverSpeedLevel2Type
			DID_OVERSPEED_0x04F7_arr[5] = 2;	//OverSpeedLv2RepeatIntrval
			DID_OVERSPEED_0x04F7_arr[6] = 5;	//OverSpeedHystresis

			write_DTE_AFE_OVERSPEED(&DID_OVERSPEED_0x04F7_arr[0]);
		}
	}
	Diag_RW_Level1_Speed 					 = DID_OVERSPEED_0x04F7_arr[0];
	Diag_RW_Level2_Speed 					 = DID_OVERSPEED_0x04F7_arr[1];
	Diag_RW_Level1_Chime_Type 				 = DID_OVERSPEED_0x04F7_arr[2];
	Diag_RW_Level1_Chime_Repetition_Interval = DID_OVERSPEED_0x04F7_arr[3];
	Diag_RW_Level2_Chime_Type 				 = DID_OVERSPEED_0x04F7_arr[4];
	Diag_RW_Level2_Chime_Repetition_Interval = DID_OVERSPEED_0x04F7_arr[5];
	Diag_RW_Chime_Hysterisis 				 = DID_OVERSPEED_0x04F7_arr[6];
	

}

/* **********************************************************************************************************************
 * Function Name: write_CHIME_ACTIVATION_0x0509
 * Description  : This function writes TEMP_BAR_VALUE code data.
 * Arguments    : *DID_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_ODOMETER_CONFIG_0xF1B0(uint8_t *ODOMETER_CONFIG)
{
	New_eeprom_memwrite_32bytes(DID_F1B0_OFFSET, DID_ODOMETER_0xF1B0_arr, ECUIdentifier_0xF1B0_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_ODOMETER_CONFIG_0xF1B0(void)
{
	uint8_t i = 0,def_var_cntr = 0;

	for(i = 0; i < ECUIdentifier_0xF1B0_ARRY_SIZE; i++)
	{
		DID_ODOMETER_0xF1B0_read_arr[i] = eeprom_read(DID_F1B0_OFFSET + i);
	}
}

/* **********************************************************************************************************************
 * Function Name: write_Roll_Pitch_Detection_0x0533_DATA
 * Description  : This function writes Roll_Pitch_Detection  code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_Roll_Pitch_Detection_0x0533_DATA(uint8_t *Detection)
{
	New_eeprom_memwrite_32bytes(DID_0533_OFFSET, DID_Roll_Pitch_Detection_0x0533_arr, ECUIdentifier_0x0533_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_Roll_Pitch_Detection code data.
 * Arguments    : *varient_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_Roll_Pitch_Detection_0x0533_DATA(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0533_ARRY_SIZE; i++)
	{
		DID_Roll_Pitch_Detection_0x0533_arr[i] = eeprom_read(DID_0533_OFFSET + i);
	}
	for(i = 0; i < ECUIdentifier_0x0533_ARRY_SIZE; i++)
	{
		if(DID_Roll_Pitch_Detection_0x0533_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == ECUIdentifier_0x0533_ARRY_SIZE)	/*Default value write*/
		{
			DID_Roll_Pitch_Detection_0x0533_arr[0] = 60; //default value not given so i given zero
			DID_Roll_Pitch_Detection_0x0533_arr[1] = 60; //default value not given so i given zero
			
			write_Roll_Pitch_Detection_0x0533_DATA(&DID_Roll_Pitch_Detection_0x0533_arr[0]);
		}
	}
	Diag_RW_Role_Angle = (DID_Roll_Pitch_Detection_0x0533_arr[0])*0.5;
	Diag_RW_Role_Pitch = (DID_Roll_Pitch_Detection_0x0533_arr[1])*0.5;
}
//#ifdef SPRINT4
/* **********************************************************************************************************************
 * Function Name: write_read_AT_GEAR_ALERT
 * Description  : This function writes read_AT_GEAR_ALERT  code data.
 * Arguments    : *DID_value code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_AT_GEAR_ALERT_0x0543_DATA(uint8_t *Detection)
{
	New_eeprom_memwrite_32bytes(DID_AT_GEAR_0543_OFFSET, DID_AT_GEAR_ALERT_0x0543_arr, ECUIdentifier_0x0543_ARRY_SIZE);
}

/* **********************************************************************************************************************
 * Function Name: read_varient_code
 * Description  : This function read_AT_GEAR_ALERT code data.
 * Arguments    : NOne
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_AT_GEAR_ALERT_0x0543_DATA(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < ECUIdentifier_0x0543_ARRY_SIZE; i++)
	{
		DID_AT_GEAR_ALERT_0x0543_arr[i] = eeprom_read(DID_AT_GEAR_0543_OFFSET + i);
	}

	Diag_AT_GEAR_ALERT_Duration = (DID_AT_GEAR_ALERT_0x0543_arr[0])*100;
	Diag_AT_GEAR_ALERT_Debounce = (DID_AT_GEAR_ALERT_0x0543_arr[1])*100;
}
//#endif
//--------> to comment from here
PRIVATE void read_AFE_ECU_IDENTIFIER_0x0523_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Displayed_Value;
	uint16_t Temp_Diag_RW_Calculated_Value;
	uint32_t Temp_Diag_RW_Accumulated_Fuel; 
	uint16_t Temp_Diag_RW_Accumulated_Distance;
	
	
	for(i = 0; i < ECUIdentifier_0x0523_ARRY_SIZE; i++)
	{
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[i] = eeprom_read(DID_0523_OFFSET + i);
	}

		Temp_Diag_RW_Displayed_Value = (Diag_RW_Displayed_Value *0.1);  // Diag_RW_Displayed_Value --final value should replace here
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[0]= (Temp_Diag_RW_Displayed_Value & 0xFF);
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[1]=((Temp_Diag_RW_Displayed_Value>>8)&0xFF);
		
		Temp_Diag_RW_Calculated_Value = (Diag_RW_Calculated_Value *0.1);// Diag_RW_Calculated_Value --final value should replace here
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[2]= (Temp_Diag_RW_Calculated_Value & 0xFF);
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[3]=((Temp_Diag_RW_Calculated_Value>>8)&0xFF);
		
		Temp_Diag_RW_Accumulated_Fuel = (Diag_RW_Accumulated_Fuel); // Diag_RW_Accumulated_Fuel --final value should replace here
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[4]= (Temp_Diag_RW_Accumulated_Fuel & 0xFF);
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[5]=((Temp_Diag_RW_Accumulated_Fuel>>8)&0xFF);
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[6]= ((Temp_Diag_RW_Accumulated_Fuel>>16)&0xFF);
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[7]=((Temp_Diag_RW_Accumulated_Fuel>>32)&0xFF);
	
		Temp_Diag_RW_Accumulated_Distance = (Diag_RW_Accumulated_Distance);// Diag_RW_Accumulated_Distance --final value should replace here
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[8]= (Temp_Diag_RW_Accumulated_Distance & 0xFF);
		DID_AFE_ECU_IDENTIFIER_0x0523_arr[9]=((Temp_Diag_RW_Accumulated_Distance>>8)&0xFF);
}

PRIVATE void read_AFE_GD_ECU_IDENTIFIER_0x0524_DATA(void)
{
	uint8_t i = 0;
	
	uint16_t Temp_Diag_RW_AFE_GD_Displayed_Value;
	uint16_t Temp_Diag_RW_AFE_GD_Calculated_Value;
	uint32_t Temp_Diag_RW_AFE_GD_Accumulated_Fuel; 
	uint16_t Temp_Diag_RW_AFE_GD_Accumulated_Distance;

	for(i = 0; i < ECUIdentifier_0x0524_ARRY_SIZE; i++)
	{
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[i] = eeprom_read(DID_0524_OFFSET + i);
	}
	
		Temp_Diag_RW_AFE_GD_Displayed_Value = (Diag_RW_AFE_GD_Displayed_Value *0.01);  // Diag_RW_AFE_GD_Displayed_Value --final value should replace here 
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[0]= (Temp_Diag_RW_AFE_GD_Displayed_Value & 0xFF);
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[1]=((Temp_Diag_RW_AFE_GD_Displayed_Value>>8)&0xFF);
		
		Temp_Diag_RW_AFE_GD_Calculated_Value = (Diag_RW_AFE_GD_Calculated_Value *0.01);// Diag_RW_AFE_GD_Calculated_Value --final value should replace here 
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[2]= (Temp_Diag_RW_AFE_GD_Calculated_Value & 0xFF);
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[3]=((Temp_Diag_RW_AFE_GD_Calculated_Value>>8)&0xFF);
		
		Temp_Diag_RW_AFE_GD_Accumulated_Fuel = (Diag_RW_AFE_GD_Accumulated_Fuel); // Diag_RW_AFE_GD_Accumulated_Fuel --final value should replace here
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[4]= (Temp_Diag_RW_AFE_GD_Accumulated_Fuel & 0xFF);
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[5]=((Temp_Diag_RW_AFE_GD_Accumulated_Fuel>>8)&0xFF);
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[6]= ((Temp_Diag_RW_AFE_GD_Accumulated_Fuel>>16)&0xFF);
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[7]=((Temp_Diag_RW_AFE_GD_Accumulated_Fuel>>32)&0xFF);
	
		Temp_Diag_RW_AFE_GD_Accumulated_Distance = (Diag_RW_AFE_GD_Accumulated_Distance);// Diag_RW_AFE_GD_Accumulated_Distance --final value should replace here
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[8]= (Temp_Diag_RW_AFE_GD_Accumulated_Distance & 0xFF);
		DID_AFE_GD_ECU_IDENTIFIER_0x0524_arr[9]=((Temp_Diag_RW_AFE_GD_Accumulated_Distance>>8)&0xFF);

}

PRIVATE void read_TELL_TALE_FB_ADC_VALUE_0x04CF_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_TT_FB_ADC_Park_Brake_TT;
	uint16_t Temp_Diag_RW_TT_FB_ADC_Check_Engine_TT;
	uint16_t Temp_Diag_RW_TT_FB_ADC_MIL_Lamp_TT; 
	uint16_t Temp_Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT;
	uint16_t Temp_Diag_RW_TT_FB_ADC_ABS_TT_Failure;
	uint16_t Temp_Diag_RW_TT_FB_ADC_Cruise_TT;
	uint16_t Temp_Diag_RW_TT_FB_ADC_Airbag_TT; 
	uint16_t Temp_Diag_RW_TT_FB_ADC_Fuel_Delivery_Module;
	uint16_t Temp_Diag_RW_TT_FB_ADC_AT_Fail_TT;
	uint16_t Temp_Diag_RW_TT_FB_ADC_ESP_Malfunction_TT;
	uint16_t Temp_Diag_RW_TT_FB_ADC_ESC_OFF; 
	uint16_t Temp_Diag_RW_TT_FB_ADC_HDC_Malfunction;

	for(i = 0; i < ECUIdentifier_0x04CF_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[i] = eeprom_read(DID_04CF_OFFSET + i);
	}
	
	Temp_Diag_RW_TT_FB_ADC_Park_Brake_TT = (Diag_RW_TT_FB_ADC_Park_Brake_TT);  // Diag_RW_TT_FB_ADC_Park_Brake_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[0]= (Temp_Diag_RW_TT_FB_ADC_Park_Brake_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[1]=((Temp_Diag_RW_TT_FB_ADC_Park_Brake_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_Check_Engine_TT = (Diag_RW_TT_FB_ADC_Check_Engine_TT);// Diag_RW_TT_FB_ADC_Check_Engine_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[2]= (Temp_Diag_RW_TT_FB_ADC_Check_Engine_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[3]=((Temp_Diag_RW_TT_FB_ADC_Check_Engine_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_MIL_Lamp_TT = (Diag_RW_TT_FB_ADC_MIL_Lamp_TT); // Diag_RW_TT_FB_ADC_MIL_Lamp_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[4]= (Temp_Diag_RW_TT_FB_ADC_MIL_Lamp_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[5]=((Temp_Diag_RW_TT_FB_ADC_MIL_Lamp_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT = (Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT); // Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[6]= (Temp_Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[7]=((Temp_Diag_RW_TT_FB_ADC_Engine_Start_Stop_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_ABS_TT_Failure = (Diag_RW_TT_FB_ADC_ABS_TT_Failure );// Diag_RW_TT_FB_ADC_ABS_TT_Failure --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[8]= (Temp_Diag_RW_TT_FB_ADC_ABS_TT_Failure & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[9]=((Temp_Diag_RW_TT_FB_ADC_ABS_TT_Failure>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_Cruise_TT = (Diag_RW_TT_FB_ADC_Cruise_TT);  // Diag_RW_TT_FB_ADC_Cruise_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[10]= (Temp_Diag_RW_TT_FB_ADC_Cruise_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[11]=((Temp_Diag_RW_TT_FB_ADC_Cruise_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_Airbag_TT = (Diag_RW_TT_FB_ADC_Airbag_TT);// Diag_RW_TT_FB_ADC_Airbag_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[12]= (Temp_Diag_RW_TT_FB_ADC_Airbag_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[13]=((Temp_Diag_RW_TT_FB_ADC_Airbag_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_Fuel_Delivery_Module = (Diag_RW_TT_FB_ADC_Fuel_Delivery_Module); // Diag_RW_TT_FB_ADC_Fuel_Delivery_Module --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[14]= (Temp_Diag_RW_TT_FB_ADC_Fuel_Delivery_Module & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[15]=((Temp_Diag_RW_TT_FB_ADC_Fuel_Delivery_Module>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_AT_Fail_TT = (Diag_RW_TT_FB_ADC_AT_Fail_TT); // Diag_RW_TT_FB_ADC_AT_Fail_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[18]= (Temp_Diag_RW_TT_FB_ADC_AT_Fail_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[19]=((Temp_Diag_RW_TT_FB_ADC_AT_Fail_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_ESP_Malfunction_TT = (Diag_RW_TT_FB_ADC_ESP_Malfunction_TT);// Diag_RW_TT_FB_ADC_ESP_Malfunction_TT --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[20]= (Temp_Diag_RW_TT_FB_ADC_ESP_Malfunction_TT & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[21]=((Temp_Diag_RW_TT_FB_ADC_ESP_Malfunction_TT>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_ESC_OFF = (Diag_RW_TT_FB_ADC_ESC_OFF); // Diag_RW_TT_FB_ADC_ESC_OFF --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[22]= (Temp_Diag_RW_TT_FB_ADC_ESC_OFF & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[23]=((Temp_Diag_RW_TT_FB_ADC_ESC_OFF>>8)&0xFF);
	
	Temp_Diag_RW_TT_FB_ADC_HDC_Malfunction = (Diag_RW_TT_FB_ADC_HDC_Malfunction );// Diag_RW_TT_FB_ADC_HDC_Malfunction --final value should replace here 
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[26]= (Temp_Diag_RW_TT_FB_ADC_HDC_Malfunction & 0xFF);
	DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[27]=((Temp_Diag_RW_TT_FB_ADC_HDC_Malfunction>>8)&0xFF);
}

PRIVATE void read_DTE_ECUIdentifier_0x04D0_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_DTE_RAFE;
	uint16_t Temp_Diag_RW_DTE_RAFE_Accumulated_Fuel;
	uint8_t Temp_Diag_RW_DTE_RAFE_Accumulated_Distance;
	uint16_t Temp_Diag_RW_DTE;

	for(i = 0; i < ECUIdentifier_0x04D0_ARRY_SIZE; i++)
	{
		DID_DTE_ECUIdentifier_0x04D0_arr[i] = eeprom_read(DID_04D0_OFFSET + i);
	}
	
	Temp_Diag_RW_DTE_RAFE = (Diag_RW_DTE_RAFE*10);// Diag_RW_DTE_RAFE_Accumulated_Distance --final value should replace here
	DID_DTE_ECUIdentifier_0x04D0_arr[0]=(Temp_Diag_RW_DTE_RAFE& 0xFF);
	
	Temp_Diag_RW_DTE_RAFE_Accumulated_Fuel = (Diag_RW_DTE_RAFE_Accumulated_Fuel);  // Diag_RW_DTE_RAFE_Accumulated_Fuel --final value should replace here 
	DID_DTE_ECUIdentifier_0x04D0_arr[1]= (Temp_Diag_RW_DTE_RAFE_Accumulated_Fuel & 0xFF);
	DID_DTE_ECUIdentifier_0x04D0_arr[2]=((Temp_Diag_RW_DTE_RAFE_Accumulated_Fuel>>8)&0xFF);
	
	Temp_Diag_RW_DTE_RAFE_Accumulated_Distance = (Diag_RW_DTE_RAFE_Accumulated_Distance);// Diag_RW_DTE_RAFE_Accumulated_Distance --final value should replace here 
	DID_DTE_ECUIdentifier_0x04D0_arr[3]=(Temp_Diag_RW_DTE_RAFE_Accumulated_Distance& 0xFF);
	
	Temp_Diag_RW_DTE = (Diag_RW_DTE);  // Diag_RW_DTE --final value should replace here 
	DID_DTE_ECUIdentifier_0x04D0_arr[4]= (Temp_Diag_RW_DTE & 0xFF);
	DID_DTE_ECUIdentifier_0x04D0_arr[5]=((Temp_Diag_RW_DTE>>8)&0xFF);
}

PRIVATE void read_FUEL_GEAN_INDICATOR_0x04D1_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Fuel_level_raw;
	uint16_t Temp_Diag_RW_Current_Gear_shift;
	Diag_RW_Fuel_level_raw = FUEL_LEVEL_FOR_DTE;
	for(i = 0; i < ECUIdentifier_0x04D1_ARRY_SIZE; i++)
	{
		DID_FUEL_GEAN_INDICATOR_0x04D1_arr[i] = eeprom_read(DID_04D1_OFFSET + i);
	}
	Temp_Diag_RW_Fuel_level_raw = (Diag_RW_Fuel_level_raw);  // Diag_RW_Fuel_level_raw --final value should replace here 
	DID_FUEL_GEAN_INDICATOR_0x04D1_arr[0]= (Temp_Diag_RW_Fuel_level_raw & 0xFF);
	DID_FUEL_GEAN_INDICATOR_0x04D1_arr[1]=((Temp_Diag_RW_Fuel_level_raw>>8)&0xFF);
	
	Temp_Diag_RW_Current_Gear_shift = (Diag_RW_Current_Gear_shift);// Diag_RW_Current_Gear_shift --final value should replace here 
	DID_FUEL_GEAN_INDICATOR_0x04D1_arr[2]=(Temp_Diag_RW_Current_Gear_shift& 0xFF);
}

PRIVATE void read_AFE_ECUIdentifier_0x04F9_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Fuel_consumption_accumulated;

	for(i = 0; i < ECUIdentifier_0x04F9_ARRY_SIZE; i++)
	{
		DID_AFE_ECUIdentifier_0x04F9_arr[i] = eeprom_read(DID_04F9_OFFSET + i);
	}
	
	Temp_Diag_RW_Fuel_consumption_accumulated = (Diag_RW_Fuel_consumption_accumulated);  // Diag_RW_Fuel_consumption_accumulated --final value should replace here 
	DID_AFE_ECUIdentifier_0x04F9_arr[0]= (Temp_Diag_RW_Fuel_consumption_accumulated & 0xFF);
	DID_AFE_ECUIdentifier_0x04F9_arr[1]=((Temp_Diag_RW_Fuel_consumption_accumulated>>8)&0xFF);
}

PRIVATE void read_TELL_TALE_STATUS_SET1_0xF07F_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_TT_SET1_Check_Engine;
	uint8_t Temp_Diag_RW_TT_SET1_Glow_Plug;
	uint8_t Temp_Diag_RW_TT_SET1_Oil_Pressure_Low; 
	uint8_t Temp_Diag_RW_TT_SET1_4WD_High;
	uint8_t Temp_Diag_RW_TT_SET1_Door_Open;
	uint8_t Temp_Diag_RW_TT_SET1_Seat_Belt;
	uint8_t Temp_Diag_RW_TT_SET1_Immo; 
	uint8_t Temp_Diag_RW_TT_SET1_Park_Brake;
	
	memset(&DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0], 0, ECUIdentifier_0xF07F_ARRY_SIZE);
	

	for(i = 0; i < ECUIdentifier_0xF07F_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_STATUS_SET1_0xF07F_arr[i] = eeprom_read(DID_F07F_OFFSET + i);
	}

	Temp_Diag_RW_TT_SET1_Check_Engine = (Diag_RW_TT_SET1_Check_Engine);  // Diag_RW_TT_SET1_Check_Engine --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Check_Engine);
	
	Temp_Diag_RW_TT_SET1_Glow_Plug = (Diag_RW_TT_SET1_Glow_Plug);// Diag_RW_TT_SET1_Glow_Plug --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Glow_Plug<<1);
	
	Temp_Diag_RW_TT_SET1_Oil_Pressure_Low = (Diag_RW_TT_SET1_Oil_Pressure_Low);// Diag_RW_TT_SET1_Oil_Pressure_Low --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Oil_Pressure_Low << 2);
	
	Temp_Diag_RW_TT_SET1_4WD_High = (Diag_RW_TT_SET1_4WD_High); // Diag_RW_TT_SET1_4WD_High --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_4WD_High <<3);
	
	Temp_Diag_RW_TT_SET1_Door_Open = (Diag_RW_TT_SET1_Door_Open); // Diag_RW_TT_SET1_Door_Open --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Door_Open <<4);
	
	Temp_Diag_RW_TT_SET1_Seat_Belt = (Diag_RW_TT_SET1_Seat_Belt); // Diag_RW_TT_SET1_Seat_Belt --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Seat_Belt <<5);
	
	Temp_Diag_RW_TT_SET1_Immo = (Diag_RW_TT_SET1_Immo); // Diag_RW_TT_SET1_Immo --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Immo <<6);
	
	Temp_Diag_RW_TT_SET1_Park_Brake = (Diag_RW_TT_SET1_Park_Brake );// Diag_RW_TT_SET1_Park_Brake --final value should replace here 
	DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0] |= (Temp_Diag_RW_TT_SET1_Park_Brake <<7);
}

PRIVATE void read_TELL_TALE_STATUS_SET2_0xF080_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_TT_SET2_Airbag;
	uint8_t Temp_Diag_RW_TT_SET2_Cruise;
	uint8_t Temp_Diag_RW_TT_SET2_AT_Fail; 
	uint8_t Temp_Diag_RW_TT_SET2_TPMS;
	uint8_t Temp_Diag_RW_TT_SET2_ABS;
	uint8_t Temp_Diag_RW_TT_SET2_Water_in_Fuel;
	uint8_t Temp_Diag_RW_TT_SET2_ESS; 
	uint8_t Temp_Diag_RW_TT_SET2_MIL_Lamp;

	memset(&DID_TELL_TALE_STATUS_SET2_0xF080_arr[0], 0, ECUIdentifier_0xF080_ARRY_SIZE);	

	for(i = 0; i < ECUIdentifier_0xF080_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_STATUS_SET2_0xF080_arr[i] = eeprom_read(DID_F080_OFFSET + i);
	}
	
	Temp_Diag_RW_TT_SET2_Airbag = (Diag_RW_TT_SET2_Airbag);  // Diag_RW_TT_SET2_Airbag --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= ((Temp_Diag_RW_TT_SET2_Airbag));
	
	Temp_Diag_RW_TT_SET2_Cruise = (Diag_RW_TT_SET2_Cruise);// Diag_RW_TT_SET2_Cruise --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= (Temp_Diag_RW_TT_SET2_Cruise <<1);
	
	Temp_Diag_RW_TT_SET2_AT_Fail = (Diag_RW_TT_SET2_AT_Fail);// Diag_RW_TT_SET2_AT_Fail --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= (Temp_Diag_RW_TT_SET2_AT_Fail <<2);
	
	Temp_Diag_RW_TT_SET2_TPMS = (Diag_RW_TT_SET2_TPMS); // Diag_RW_TT_SET2_TPMS --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= (Temp_Diag_RW_TT_SET2_TPMS <<3);
	
	Temp_Diag_RW_TT_SET2_ABS = (Diag_RW_TT_SET2_ABS); // Diag_RW_TT_SET2_ABS --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= ((Temp_Diag_RW_TT_SET2_ABS <<4));
	
	Temp_Diag_RW_TT_SET2_Water_in_Fuel = (Diag_RW_TT_SET2_Water_in_Fuel); // Diag_RW_TT_SET2_Water_in_Fuel --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= (Temp_Diag_RW_TT_SET2_Water_in_Fuel <<5);
	
	Temp_Diag_RW_TT_SET2_ESS = (Diag_RW_TT_SET2_ESS); // Diag_RW_TT_SET2_ESS --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= ((Temp_Diag_RW_TT_SET2_ESS <<6));
	
	Temp_Diag_RW_TT_SET2_MIL_Lamp = (Diag_RW_TT_SET2_MIL_Lamp );// Diag_RW_TT_SET2_MIL_Lamp --final value should replace here 
	DID_TELL_TALE_STATUS_SET2_0xF080_arr[1] |= ((Temp_Diag_RW_TT_SET2_MIL_Lamp <<7));
}

PRIVATE void read_TELL_TALE_STATUS_SET3_0xF081_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_TT_SET3_ESC_Off;
	uint8_t Temp_Diag_RW_TT_SET3_Park_Lamp;
	uint8_t Temp_Diag_RW_TT_SET3_HHC_Malfunction; 
	uint8_t Temp_Diag_RW_TT_SET3_HDC_Malfunction;
	uint8_t Temp_Diag_RW_TT_SET3_ESC_Malfunction;
	uint8_t Temp_Diag_RW_TT_SET3_Battery_Charge;
	uint8_t Temp_Diag_RW_TT_SET3_Low_Fuel; 
	uint8_t Temp_Diag_RW_TT_SET3_High_Temp;

	memset(&DID_TELL_TALE_STATUS_SET3_0xF081_arr[0], 0, ECUIdentifier_0xF081_ARRY_SIZE);
	
	for(i = 0; i < ECUIdentifier_0xF081_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_STATUS_SET3_0xF081_arr[i] = eeprom_read(DID_F081_OFFSET + i);
	}
	
	Temp_Diag_RW_TT_SET3_ESC_Off = (Diag_RW_TT_SET3_ESC_Off);  // Diag_RW_TT_SET3_ESC_Off --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_ESC_Off);
	
	Temp_Diag_RW_TT_SET3_Park_Lamp = (Diag_RW_TT_SET3_Park_Lamp);// Diag_RW_TT_SET3_Park_Lamp --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_Park_Lamp <<1);
	
	Temp_Diag_RW_TT_SET3_HHC_Malfunction = (Diag_RW_TT_SET3_HHC_Malfunction);// Diag_RW_TT_SET3_HHC_Malfunction --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_HHC_Malfunction <<2);
	
	Temp_Diag_RW_TT_SET3_HDC_Malfunction = (Diag_RW_TT_SET3_HDC_Malfunction); // Diag_RW_TT_SET3_HDC_Malfunction --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_HDC_Malfunction <<3);
	
	Temp_Diag_RW_TT_SET3_ESC_Malfunction = (Diag_RW_TT_SET3_ESC_Malfunction); // Diag_RW_TT_SET3_ESC_Malfunction --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_ESC_Malfunction <<4);
	
	Temp_Diag_RW_TT_SET3_Battery_Charge = (Diag_RW_TT_SET3_Battery_Charge); // Diag_RW_TT_SET3_Battery_Charge --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_Battery_Charge <<5);
	
	Temp_Diag_RW_TT_SET3_Low_Fuel = (Diag_RW_TT_SET3_Low_Fuel); // Diag_RW_TT_SET3_Low_Fuel --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_Low_Fuel <<6);
	
	Temp_Diag_RW_TT_SET3_High_Temp = (Diag_RW_TT_SET3_High_Temp );// Diag_RW_TT_SET3_High_Temp --final value should replace here 
	DID_TELL_TALE_STATUS_SET3_0xF081_arr[1] |= (Temp_Diag_RW_TT_SET3_High_Temp <<7);
}

PRIVATE void read_TELL_TALE_STATUS_SET4_0xF08E_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_TT_SET4_ECO;
	uint8_t Temp_Diag_RW_TT_SET4_High_Beam;
	uint8_t Temp_Diag_RW_TT_SET4_Turn_Left; 
	uint8_t Temp_Diag_RW_TT_SET4_Turn_Right;
	uint8_t Temp_Diag_RW_TT_SET4_Front_Fog;
	uint8_t Temp_Diag_RW_TT_SET4_Rear_Fog;
	
	memset(&DID_TELL_TALE_STATUS_SET4_0xF08E_arr[0], 0, ECUIdentifier_0xF08E_ARRY_SIZE);
	
	for(i = 0; i < ECUIdentifier_0xF08E_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_STATUS_SET4_0xF08E_arr[i] = eeprom_read(DID_F08E_OFFSET + i);
	}
	
	Temp_Diag_RW_TT_SET4_ECO = (Diag_RW_TT_SET4_ECO);// Diag_RW_TT_SET4_ECO --final value should replace here 
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[1] |= (Temp_Diag_RW_TT_SET4_ECO <<2);
	
	Temp_Diag_RW_TT_SET4_High_Beam = (Diag_RW_TT_SET4_High_Beam); // Diag_RW_TT_SET4_High_Beam --final value should replace here 
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[1] |= (Temp_Diag_RW_TT_SET4_High_Beam <<3);
	
	Temp_Diag_RW_TT_SET4_Turn_Left = (Diag_RW_TT_SET4_Turn_Left); // Diag_RW_TT_SET4_Turn_Left --final value should replace here 
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[1] |= (Temp_Diag_RW_TT_SET4_Turn_Left <<4);
	
	Temp_Diag_RW_TT_SET4_Turn_Right = (Diag_RW_TT_SET4_Turn_Right); // Diag_RW_TT_SET4_Turn_Right --final value should replace here 
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[1] |= (Temp_Diag_RW_TT_SET4_Turn_Right <<5);
	
	Temp_Diag_RW_TT_SET4_Front_Fog = (Diag_RW_TT_SET4_Front_Fog); // Diag_RW_TT_SET4_Front_Fog --final value should replace here 
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[1] |= (Temp_Diag_RW_TT_SET4_Front_Fog <<6);
	
	Temp_Diag_RW_TT_SET4_Rear_Fog = (Diag_RW_TT_SET4_Rear_Fog );// Diag_RW_TT_SET4_Rear_Fog --final value should replace here 
	DID_TELL_TALE_STATUS_SET4_0xF08E_arr[1] |= (Temp_Diag_RW_TT_SET4_Rear_Fog <<7);
}

PRIVATE void read_TELL_TALE_STATUS_SET5_0xF08F_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_TT_SET5_EPS_Malfunction;
	uint8_t Temp_Diag_RW_TT_SET5_Overspeed;
	uint8_t Temp_Diag_RW_TT_SET5_Reserved;
	
	memset(&DID_TELL_TALE_STATUS_SET5_0xF08F_arr[0], 0, ECUIdentifier_0xF08F_ARRY_SIZE);

	for(i = 0; i < ECUIdentifier_0xF08F_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_STATUS_SET5_0xF08F_arr[i] = eeprom_read(DID_F08F_OFFSET + i);
	}
	
	Temp_Diag_RW_TT_SET5_EPS_Malfunction = (Diag_RW_TT_SET5_EPS_Malfunction);  // Diag_RW_TT_SET5_EPS_Malfunction --final value should replace here 
	DID_TELL_TALE_STATUS_SET5_0xF08F_arr[1] |= (Temp_Diag_RW_TT_SET5_EPS_Malfunction);
	
	Temp_Diag_RW_TT_SET5_Overspeed = (Diag_RW_TT_SET5_Overspeed); // Diag_RW_TT_SET5_Overspeed --final value should replace here 
	DID_TELL_TALE_STATUS_SET5_0xF08F_arr[1] |= (Temp_Diag_RW_TT_SET5_Overspeed <<6);
	
	Temp_Diag_RW_TT_SET5_Reserved = (Diag_RW_TT_SET5_Reserved );// Diag_RW_TT_SET5_Reserved --final value should replace here 
	DID_TELL_TALE_STATUS_SET5_0xF08F_arr[1] |= (Temp_Diag_RW_TT_SET5_Reserved <<7);
	
}

PRIVATE void read_GAUGES_0xF082_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Speedometer_status;
	uint16_t Temp_Diag_RW_Tachometer_status;
	uint16_t Temp_Diag_RW_Tachometer_Redline;

	for(i = 0; i < ECUIdentifier_0xF082_ARRY_SIZE; i++)
	{
		DID_GAUGES_0xF082_arr[i] = eeprom_read(DID_F082_OFFSET + i);
	}
	
	Temp_Diag_RW_Speedometer_status = (Diag_RW_Speedometer_status*0.05);  // Diag_RW_Speedometer_status --final value should replace here
	DID_GAUGES_0xF082_arr[0]= (Temp_Diag_RW_Speedometer_status & 0xFF);
	DID_GAUGES_0xF082_arr[1]=((Temp_Diag_RW_Speedometer_status>>8)&0xFF);
	
	Temp_Diag_RW_Tachometer_status = (Diag_RW_Tachometer_status*0.05);  // Diag_RW_Tachometer_status --final value should replace here
	DID_GAUGES_0xF082_arr[2]= (Temp_Diag_RW_Tachometer_status & 0xFF);
	DID_GAUGES_0xF082_arr[3]=((Temp_Diag_RW_Tachometer_status>>8)&0xFF);
	
	Temp_Diag_RW_Tachometer_Redline = (Diag_RW_Tachometer_Redline);// Diag_RW_Tachometer_Redline --final value should replace here 
	DID_GAUGES_0xF082_arr[4]=(Temp_Diag_RW_Tachometer_Redline& 0xFF);
}

PRIVATE void read_ILLUMINATION_STATUS_0xF084_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_LCD_Illumination_Status;
	uint8_t Temp_Diag_RW_Dial_Illumination_Status;
	uint8_t Temp_Diag_RW_Pointer_Illumination_Status;

	for(i = 0; i < ECUIdentifier_0xF084_ARRY_SIZE; i++)
	{
		DID_ILLUMINATION_STATUS_0xF084_arr[i] = eeprom_read(DID_F084_OFFSET + i);
	}
	Temp_Diag_RW_LCD_Illumination_Status = (Diag_RW_LCD_Illumination_Status*0.005);  // Diag_RW_LCD_Illumination_Status --final value should replace here 
	DID_ILLUMINATION_STATUS_0xF084_arr[0]= (Temp_Diag_RW_LCD_Illumination_Status & 0xFF);
	
	Temp_Diag_RW_Dial_Illumination_Status = (Diag_RW_Dial_Illumination_Status*0.005);  // Diag_RW_Dial_Illumination_Status --final value should replace here 
	DID_ILLUMINATION_STATUS_0xF084_arr[1]= (Temp_Diag_RW_Dial_Illumination_Status & 0xFF);
	
	Temp_Diag_RW_Pointer_Illumination_Status = (Diag_RW_Pointer_Illumination_Status);// Diag_RW_Pointer_Illumination_Status --final value should replace here 
	DID_ILLUMINATION_STATUS_0xF084_arr[2]=(Temp_Diag_RW_Pointer_Illumination_Status& 0xFF);
}

PRIVATE void read_BAR_GRAPH_STATUS_0xF083_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_Temperature_bar;
	uint8_t Temp_Diag_RW_Fuel_Bar;
	

	for(i = 0; i < ECUIdentifier_0xF083_ARRY_SIZE; i++)
	{
		DID_BAR_GRAPH_STATUS_0xF083_arr[i] = eeprom_read(DID_F083_OFFSET + i);
	}
	
	Temp_Diag_RW_Temperature_bar = (temp_widget);  // Diag_RW_Temperature_bar --final value replaced here 
	DID_BAR_GRAPH_STATUS_0xF083_arr[0]= (Temp_Diag_RW_Temperature_bar & 0x0F);
	
	Temp_Diag_RW_Fuel_Bar = (fuel_widget); // Diag_RW_Fuel_Bar --final value replaced here 
	DID_BAR_GRAPH_STATUS_0xF083_arr[0]= ((Temp_Diag_RW_Fuel_Bar & 0xF0)>>4);
}

PRIVATE void read_GEAR_INDICATION_STATUS_0xF085_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_Major_Gear;
	uint8_t Temp_Diag_RW_Minor_Gear;
	uint8_t Temp_Diag_RW_Up_Indication_Gear;
	uint8_t Temp_Diag_RW_Down_Indication_Gear;
	Diag_RW_Major_Gear = gear_to_HMI;

	for(i = 0; i < ECUIdentifier_0xF085_ARRY_SIZE; i++)
	{
		DID_GEAR_INDICATION_STATUS_0xF085_arr[i] = eeprom_read(DID_F085_OFFSET + i);
	}
	
	Temp_Diag_RW_Major_Gear = (Diag_RW_Major_Gear);  // Diag_RW_Major_Gear --final value should replace here 
	DID_GEAR_INDICATION_STATUS_0xF085_arr[0]= (Temp_Diag_RW_Major_Gear & 0x0F);
	
	Temp_Diag_RW_Minor_Gear = (Diag_RW_Minor_Gear); // Diag_RW_Minor_Gear --final value should replace here 
	DID_GEAR_INDICATION_STATUS_0xF085_arr[0]= ((Temp_Diag_RW_Minor_Gear & 0xF0)>>4);
	
	Temp_Diag_RW_Up_Indication_Gear = (Diag_RW_Up_Indication_Gear);  // Diag_RW_Up_Indication_Gear --final value should replace here 
	DID_GEAR_INDICATION_STATUS_0xF085_arr[1]= (Temp_Diag_RW_Up_Indication_Gear & 0x01);
	
	Temp_Diag_RW_Down_Indication_Gear = (Diag_RW_Down_Indication_Gear); // Diag_RW_Down_Indication_Gear --final value should replace here 
	DID_GEAR_INDICATION_STATUS_0xF085_arr[1]= ((Temp_Diag_RW_Down_Indication_Gear & 0x02)>>1);
}

PRIVATE void read_TELL_TALE_STATUS_SET6_0xF096_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_Diesel_Particulate_Filter;
	uint8_t Temp_Diag_RW_Diesel_Exhaust_system_Fluid;
	uint8_t Temp_Diag_RW_4WD_Low;
	uint8_t Temp_Diag_RW_Regeneration_Tell_Tale;

	for(i = 0; i < ECUIdentifier_0xF096_ARRY_SIZE; i++)
	{
		DID_TELL_TALE_STATUS_SET6_0xF096_arr[i] = eeprom_read(DID_F096_OFFSET + i);
	}
	
	Temp_Diag_RW_Diesel_Particulate_Filter = (Diag_RW_Diesel_Particulate_Filter);  // Diag_RW_Diesel_Particulate_Filter --final value should replace here 
	DID_TELL_TALE_STATUS_SET6_0xF096_arr[1]|= ((Temp_Diag_RW_Diesel_Particulate_Filter));
	
	Temp_Diag_RW_Diesel_Exhaust_system_Fluid = (Diag_RW_Diesel_Exhaust_system_Fluid);// Diag_RW_Diesel_Exhaust_system_Fluid --final value should replace here 
	DID_TELL_TALE_STATUS_SET6_0xF096_arr[1]|= (Temp_Diag_RW_Diesel_Exhaust_system_Fluid >>1);
	
	Temp_Diag_RW_4WD_Low = (Diag_RW_4WD_Low);// Temp_Diag_RW_4WD_Low --final value should replace here 
	DID_TELL_TALE_STATUS_SET6_0xF096_arr[1]|= (Temp_Diag_RW_4WD_Low >>2);
	
	Temp_Diag_RW_Regeneration_Tell_Tale = (Diag_RW_Regeneration_Tell_Tale); // Diag_RW_Regeneration_Tell_Tale --final value should replace here 
	DID_TELL_TALE_STATUS_SET6_0xF096_arr[1]|= (Temp_Diag_RW_Regeneration_Tell_Tale >>3);
	
	
}

PRIVATE void read_BATTERY_VOLTAGE_0xF1B1_DATA(void)
{
	uint8_t i = 0;
	uint8_t Temp_Diag_RW_Battery_Voltage;

	for(i = 0; i < ECUIdentifier_0xF1B1_ARRY_SIZE; i++)
	{
		DID_BATTERY_VOLTAGE_0xF1B1_arr[i] = eeprom_read(DID_F1B1_OFFSET + i);
	}
	Diag_RW_Battery_Voltage  = (0.0052264808362369*Batt_ADC_value);
	Temp_Diag_RW_Battery_Voltage = (Diag_RW_Battery_Voltage/**0.01*/);  // Diag_RW_Battery_Voltage --final value should replace here
	DID_BATTERY_VOLTAGE_0xF1B1_arr[0]= (Temp_Diag_RW_Battery_Voltage & 0xFF);
	DID_BATTERY_VOLTAGE_0xF1B1_arr[1]=((Temp_Diag_RW_Battery_Voltage>>8)&0xFF);
}

PRIVATE void read_VEHICLE_SPEED_0xF1B2_DATA(void)
{
	uint8_t i = 0;
	uint32_t Temp_Diag_RW_Vehicle_Speed;

	for(i = 0; i < ECUIdentifier_0xF1B2_ARRY_SIZE; i++)
	{
		DID_VEHICLE_SPEED_0xF1B2_arr[i] = eeprom_read(DID_F1B2_OFFSET + i);
	}
	
	Temp_Diag_RW_Vehicle_Speed = (Diag_RW_Vehicle_Speed*0.015625);  // Diag_RW_Vehicle_Speed --final value should replace here 
	DID_VEHICLE_SPEED_0xF1B2_arr[0]= (Temp_Diag_RW_Vehicle_Speed & 0xFF);
	DID_VEHICLE_SPEED_0xF1B2_arr[1]=((Temp_Diag_RW_Vehicle_Speed>>8)&0xFF);
}

PRIVATE void read_ENGINE_SPEED_0xF1B3_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Engine_Speed;

	for(i = 0; i < ECUIdentifier_0xF1B3_ARRY_SIZE; i++)
	{
		DID_ENGINE_SPEED_0xF1B3_arr[i] = eeprom_read(DID_F1B3_OFFSET + i);
	}
	
	Temp_Diag_RW_Engine_Speed = (Diag_RW_Engine_Speed);  // Diag_RW_Engine_Speed --final value should replace here 
	DID_ENGINE_SPEED_0xF1B3_arr[0]= (Temp_Diag_RW_Engine_Speed & 0xFF);
	DID_ENGINE_SPEED_0xF1B3_arr[1]=((Temp_Diag_RW_Engine_Speed>>8)&0xFF);
}

PRIVATE void read_RESET_REASON_0xF1B4_DATA(void)
{
	uint8_t i = 0;
	uint32_t Temp_Diag_RW_Reset_Reasons;
	uint8_t  Temp_Diag_RW_External_reset_signal;
	uint8_t  Temp_Diag_RW_Watch_dog_reset;
	uint8_t  Temp_Diag_RW_PowerOn_Clear_Resets ;

	for(i = 0; i < ECUIdentifier_0xF1B4_ARRY_SIZE; i++)
	{
		DID_RESET_REASON_0xF1B4_arr[i] = eeprom_read(DID_F1B4_OFFSET + i);
	}
	
	Temp_Diag_RW_External_reset_signal = (Diag_RW_External_reset_signal); // Diag_RW_External_reset_signal --final value should replace here 
	DID_RESET_REASON_0xF1B4_arr[0]= (Temp_Diag_RW_External_reset_signal & 0xff);
	
	Temp_Diag_RW_Watch_dog_reset = (Diag_RW_Watch_dog_reset); // Diag_RW_Watch_dog_reset --final value should replace here 
	DID_RESET_REASON_0xF1B4_arr[1]=(Temp_Diag_RW_Watch_dog_reset & 0xff);
	
	Temp_Diag_RW_PowerOn_Clear_Resets = (Diag_RW_PowerOn_Clear_Resets); // Diag_RW_PowerOn_Clear_Resets --final value should replace here 
	DID_RESET_REASON_0xF1B4_arr[2]= (Temp_Diag_RW_PowerOn_Clear_Resets & 0xff);
	
}

PRIVATE void read_RESET_COUNTER_0xF1B5_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Reset_Counter;
	
	for(i = 0; i < ECUIdentifier_0xF1B5_ARRY_SIZE; i++)
	{
		DID_RESET_COUNTER_0xF1B5_arr[i] = eeprom_read(DID_F1B5_OFFSET + i);
	}
	Temp_Diag_RW_Reset_Counter = (Diag_RW_Reset_Counter); // Diag_RW_Reset_Counter --final value should replace here 
	DID_RESET_COUNTER_0xF1B5_arr[0]= (Temp_Diag_RW_Reset_Counter & 0xFF);
	DID_RESET_COUNTER_0xF1B5_arr[1]=((Temp_Diag_RW_Reset_Counter>>8)&0xFF);
	
	
}

PRIVATE void read_IGNITION_COUNTER_0xF1B6_DATA(void)
{
	uint8_t i = 0;
	uint16_t Temp_Diag_RW_Ignition_Counter;
	
	for(i = 0; i < ECUIdentifier_0xF1B6_ARRY_SIZE; i++)
	{
		DID_IGNITION_COUNTER_0xF1B6_arr[i] = eeprom_read(DID_F1B6_OFFSET + i);
	}
	
	Temp_Diag_RW_Ignition_Counter = (Diag_RW_Ignition_Counter); // Diag_RW_Ignition_Counter --final value should replace here 
	DID_IGNITION_COUNTER_0xF1B6_arr[0]= (Temp_Diag_RW_Ignition_Counter & 0xFF);
	DID_IGNITION_COUNTER_0xF1B6_arr[1]=((Temp_Diag_RW_Ignition_Counter>>8)&0xFF);
}

PRIVATE void read_EEPROM_LAYOUT_0xF1B7_DATA(void)
{
	uint8_t i = 0;
	uint32_t Temp_Diag_RW_EEPROM_Layout_Version;

	for(i = 0; i < ECUIdentifier_0xF1B7_ARRY_SIZE; i++)
	{
		DID_EEPROM_LAYOUT_0xF1B7_arr[i] = eeprom_read(DID_F1B7_OFFSET + i);
	}
	
	/*Temp_Diag_RW_EEPROM_Layout_Version = (Diag_RW_EEPROM_Layout_Version); // Diag_RW_EEPROM_Layout_Version --final value should replace here 
	DID_EEPROM_LAYOUT_0xF1B7_arr[0]= (Temp_Diag_RW_EEPROM_Layout_Version & 0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[1]=((Temp_Diag_RW_EEPROM_Layout_Version>>8)&0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[2]= ((Temp_Diag_RW_EEPROM_Layout_Version>>16)&0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[3]=((Temp_Diag_RW_EEPROM_Layout_Version>>32)&0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[4]=((Temp_Diag_RW_EEPROM_Layout_Version>>64)&0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[5]= ((Temp_Diag_RW_EEPROM_Layout_Version>>128)&0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[6]=((Temp_Diag_RW_EEPROM_Layout_Version>>256)&0xFF);
	DID_EEPROM_LAYOUT_0xF1B7_arr[7]=((Temp_Diag_RW_EEPROM_Layout_Version>>512)&0xFF);*/
}

//--------> to comment till here
/* **********************************************************************************************************************
 * Function Name: read_SPAREPART_NUMBER
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Network_Security_key_SKC_0x0108(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < SKC_0x0108_arr_ARRY_SIZE; i++)
	{
		NS_Network_Security_key_SKC_0x0108_arr[i] = eeprom_read(Neteork_Security_key_SKC_0108_OFFSET + i);
	}

	for(i = 0; i < SKC_0x0108_arr_ARRY_SIZE; i++)
	{
		if(NS_Network_Security_key_SKC_0x0108_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == SKC_0x0108_arr_ARRY_SIZE)	/*Default value write*/
		{

			NS_Network_Security_key_SKC_0x0108_arr[0]=0x00;
			NS_Network_Security_key_SKC_0x0108_arr[1]=0x00;
			NS_Network_Security_key_SKC_0x0108_arr[2]=0x00;
			NS_Network_Security_key_SKC_0x0108_arr[3]=0x00;

			//eeprom_memwrite_32bytes(Neteork_Security_key_SKC_0108_OFFSET, NS_Network_Security_key_SKC_0x0108_arr, SKC_0x0108_arr_ARRY_SIZE);
			write_NS_Network_Security_key_SKC_0x0108(&NS_Network_Security_key_SKC_0x0108_arr[0]);
		}
	}

	IVN_SecurityKeyConst.u8[0] = NS_Network_Security_key_SKC_0x0108_arr[0];
	IVN_SecurityKeyConst.u8[1] = NS_Network_Security_key_SKC_0x0108_arr[1];
	IVN_SecurityKeyConst.u8[2] = NS_Network_Security_key_SKC_0x0108_arr[2];
	IVN_SecurityKeyConst.u8[3] = NS_Network_Security_key_SKC_0x0108_arr[3];

}

/* **********************************************************************************************************************
 * Function Name: write_NS_Network_Security_key_SKC_0x0108
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_Network_Security_key_SKC_0x0108(uint8_t *Security_key)
{
	uint8_t i = 0;

	for(i = 0; i < SKC_0x0108_arr_ARRY_SIZE; i++)
	{
		eeprom_write((Neteork_Security_key_SKC_0108_OFFSET+i),Security_key[i]);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_NS_Network_Security_key_SKC1_0x0109
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Network_Security_key_SKC1_0x0109(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < SKC1_0x0109_arr_ARRY_SIZE; i++)
	{
		NS_Network_Security_key_SKC1_0x0109_arr[i] = eeprom_read(Neteork_Security_key_SKC1_0109_OFFSET + i);
	}

	for(i = 0; i < SKC1_0x0109_arr_ARRY_SIZE; i++)
	{
		if(NS_Network_Security_key_SKC1_0x0109_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == SKC1_0x0109_arr_ARRY_SIZE)	/*Default value write*/
		{
			NS_Network_Security_key_SKC1_0x0109_arr[0]=0x00;
			NS_Network_Security_key_SKC1_0x0109_arr[1]=0x00;
			NS_Network_Security_key_SKC1_0x0109_arr[2]=0x00;
			NS_Network_Security_key_SKC1_0x0109_arr[3]=0x00;

			write_NS_Network_Security_key_SKC1_0x0109(&NS_Network_Security_key_SKC1_0x0109_arr[0]);

		}
	}
	IVN_SecurityKey_1.u8[0] = NS_Network_Security_key_SKC1_0x0109_arr[0];
	IVN_SecurityKey_1.u8[1] = NS_Network_Security_key_SKC1_0x0109_arr[1];
	IVN_SecurityKey_1.u8[2] = NS_Network_Security_key_SKC1_0x0109_arr[2];
	IVN_SecurityKey_1.u8[3] = NS_Network_Security_key_SKC1_0x0109_arr[3];
}

/* **********************************************************************************************************************
 * Function Name: write_NS_Network_Security_key_SKC1_0x0109
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_Network_Security_key_SKC1_0x0109(uint8_t *Security_key)
{
	uint8_t i = 0;

	for(i = 0; i < SKC1_0x0109_arr_ARRY_SIZE; i++)
	{
		eeprom_write((Neteork_Security_key_SKC1_0109_OFFSET+i),Security_key[i]);
	}
}


/* **********************************************************************************************************************
 * Function Name: read_NS_Network_Security_key_SKC2_0x010A
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Network_Security_key_SKC2_0x010A(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < SKC2_0x010A_arr_ARRY_SIZE; i++)
	{
		NS_Network_Security_key_SKC2_0x010A_arr[i] = eeprom_read(Neteork_Security_key_SKC2_010A_OFFSET + i);
	}

	for(i = 0; i < SKC2_0x010A_arr_ARRY_SIZE; i++)
	{
		if(NS_Network_Security_key_SKC2_0x010A_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == SKC2_0x010A_arr_ARRY_SIZE)	/*Default value write*/
		{
			//NS_Network_Security_key_SKC2_0x010A_arr.u32 = 0xFF001020;

			NS_Network_Security_key_SKC2_0x010A_arr[0]=0x00;
			NS_Network_Security_key_SKC2_0x010A_arr[1]=0x00;
			NS_Network_Security_key_SKC2_0x010A_arr[2]=0x00;
			NS_Network_Security_key_SKC2_0x010A_arr[3]=0x00;

			write_NS_Network_Security_key_SKC2_0x010A(&NS_Network_Security_key_SKC2_0x010A_arr[0]);
		}
	}
	IVN_SecurityKey_2.u8[0] = NS_Network_Security_key_SKC2_0x010A_arr[0];
	IVN_SecurityKey_2.u8[1] = NS_Network_Security_key_SKC2_0x010A_arr[1];
	IVN_SecurityKey_2.u8[2] = NS_Network_Security_key_SKC2_0x010A_arr[2];
	IVN_SecurityKey_2.u8[3] = NS_Network_Security_key_SKC2_0x010A_arr[3];
}

/* **********************************************************************************************************************
 * Function Name: write_NS_Network_Security_key_SKC2_0x010A
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_Network_Security_key_SKC2_0x010A(uint8_t *Security_key)
{
	uint8_t i = 0;

	for(i = 0; i < SKC2_0x010A_arr_ARRY_SIZE; i++)
	{
		eeprom_write((Neteork_Security_key_SKC2_010A_OFFSET+i),Security_key[i]);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_NS_Network_Security_Configuration_0x010B
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Network_Security_Configuration_0x010B(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < NSC_0x010B_arr_ARRY_SIZE; i++)
	{
		NS_Network_Security_Configuration_0x010B_arr[i] = eeprom_read(Network_Secuirty_Configurations_OFFSET + i);
	}

	for(i = 0; i < NSC_0x010B_arr_ARRY_SIZE; i++)
	{
		if(NS_Network_Security_Configuration_0x010B_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == NSC_0x010B_arr_ARRY_SIZE)	/*Default value write*/
		{
			//NS_Network_Security_key_SKC2_0x010A_arr.u32 = 0xFF001020;

			NS_Network_Security_Configuration_0x010B_arr[0]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[1]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[2]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[3]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[4]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[5]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[6]=0x1;
			NS_Network_Security_Configuration_0x010B_arr[7]=0x0A;
			NS_Network_Security_Configuration_0x010B_arr[8]=0x5;
			NS_Network_Security_Configuration_0x010B_arr[9]=0x0A;
			NS_Network_Security_Configuration_0x010B_arr[10]=0x5;

			write_NS_Network_Security_Configuration_0x010B(&NS_Network_Security_Configuration_0x010B_arr[0]);
		}
	}
	IVN_cfg_FCIncCntrTiTh_C = NS_Network_Security_Configuration_0x010B_arr[0];
	IVN_cfg_FCDevTh_C = NS_Network_Security_Configuration_0x010B_arr[1];
	IVN_cfg_FcDifErrReptTh_C = NS_Network_Security_Configuration_0x010B_arr[2];
	IVN_cfg_FcErrReptInc_C = NS_Network_Security_Configuration_0x010B_arr[3];
	IVN_cfg_FcErrReptDec_C = NS_Network_Security_Configuration_0x010B_arr[4];
	IVN_cfg_FcErrHealDec_C = NS_Network_Security_Configuration_0x010B_arr[5];
	IVN_cfg_ResyncCntrTh_C = NS_Network_Security_Configuration_0x010B_arr[6];
	IVN_cfg_ResyncTiTh_C = NS_Network_Security_Configuration_0x010B_arr[7];
	IVN_cfg_MAC2ErrCntrTh_C = NS_Network_Security_Configuration_0x010B_arr[8];
	IVN_cfg_SynchWndTiTh_C = NS_Network_Security_Configuration_0x010B_arr[9];
	IVN_cfg_SynchMsgTxCntr_C = NS_Network_Security_Configuration_0x010B_arr[10];
}

/* **********************************************************************************************************************
 * Function Name: write_NS_Network_Security_Configuration_0x010B
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_Network_Security_Configuration_0x010B(uint8_t *Security_key)
{
	uint8_t i = 0;

	for(i = 0; i < NSC_0x010B_arr_ARRY_SIZE; i++)
	{
		eeprom_write((Network_Secuirty_Configurations_OFFSET+i),Security_key[i]);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_NS_NM_Configuration_0x0103
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_NM_Configuration_0x0103(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < NSC_0x0103_arr_ARRY_SIZE; i++)
	{
		NS_NM_Configuration_0x0103_arr[i] = eeprom_read(NM_Configurations_OFFSET + i);
	}

	for(i = 0; i < NSC_0x0103_arr_ARRY_SIZE; i++)
	{
		if(NS_NM_Configuration_0x0103_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == NSC_0x0103_arr_ARRY_SIZE)	/*Default value write*/
		{

			NS_NM_Configuration_0x0103_arr[0]=0x5;
			NS_NM_Configuration_0x0103_arr[1]=0x0A;
			NS_NM_Configuration_0x0103_arr[2]=0x0A;
			NS_NM_Configuration_0x0103_arr[3]=0x0A;
			NS_NM_Configuration_0x0103_arr[4]=0x0A;
			NS_NM_Configuration_0x0103_arr[5]=0x0A;
			NS_NM_Configuration_0x0103_arr[6]=0x28;
			NS_NM_Configuration_0x0103_arr[7]=0x0A;
			NS_NM_Configuration_0x0103_arr[8]=0x5;
			NS_NM_Configuration_0x0103_arr[9]=0x64;
			NS_NM_Configuration_0x0103_arr[10]=0xC8;
			NS_NM_Configuration_0x0103_arr[11]=0x32;
			NS_NM_Configuration_0x0103_arr[12]=0x32;
			NS_NM_Configuration_0x0103_arr[13]=0x64;
			NS_NM_Configuration_0x0103_arr[14]=0x0A;
			NS_NM_Configuration_0x0103_arr[15]=0x0A;
			NS_NM_Configuration_0x0103_arr[16]=0x1;
			NS_NM_Configuration_0x0103_arr[17]=0x64;

			write_NS_NM_Configuration_0x0103(&NS_NM_Configuration_0x0103_arr[0]);
		}
	}
	IVN_cfg_NA_Detection_Threshold = NS_NM_Configuration_0x0103_arr[0];
	IVN_cfg_NA_Recovery_Threshold = NS_NM_Configuration_0x0103_arr[1];
	IVN_cfg_Msg_Timeout_Detection_Threshold = NS_NM_Configuration_0x0103_arr[2];
	IVN_cfg_Msg_Timeout_Recovery_Threshold = NS_NM_Configuration_0x0103_arr[3];
	IVN_cfg_Msg_Content_Error_Detection_Threshold = NS_NM_Configuration_0x0103_arr[4];
	IVN_cfg_Msg_Content_Error_Recovery_Threshold = NS_NM_Configuration_0x0103_arr[5];
	IVN_cfg_NM_Drive_Cnt_Clear_DTC = NS_NM_Configuration_0x0103_arr[6];
	IVN_cfg_Busoff_Fast_Recovery_Time = NS_NM_Configuration_0x0103_arr[7]*10;
	IVN_cfg_Fast_Bus_off_Recovery_Count = NS_NM_Configuration_0x0103_arr[8];
	IVN_cfg_Busoff_Slow_Recovery_Time = NS_NM_Configuration_0x0103_arr[9]*10;
	IVN_cfg_NM_IGN_On_Startup_Delay = NS_NM_Configuration_0x0103_arr[10]*10;
	IVN_cfg_NM_Restart_Dly_Time_Under_Vol_Recovery = NS_NM_Configuration_0x0103_arr[11]*10;
	IVN_cfg_NA_NM_Restart_Dly_Time_Over_Vol_Recovery = NS_NM_Configuration_0x0103_arr[12]*10;
	IVN_cfg_NA_NM_Restart_Dly_Time_Bus_Off_recovery = NS_NM_Configuration_0x0103_arr[13]*10;
	IVN_cfg_NA_Signal_Content_Fail_Detect_Count = NS_NM_Configuration_0x0103_arr[14];
	IVN_cfg_NA_Signal_Content_Recov_Detect_Count = NS_NM_Configuration_0x0103_arr[15];
	IVN_cfg_NA_CAN_Wakeup_Feature_Enable = NS_NM_Configuration_0x0103_arr[16];
	IVN_cfg_NA_NM_Restart_Dly_Time_Cranking = NS_NM_Configuration_0x0103_arr[17]*10;
	
}

/* **********************************************************************************************************************
 * Function Name: write_NS_Network_Security_Configuration_0x010B
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_NM_Configuration_0x0103(uint8_t *Config_Value)
{
	uint8_t i = 0;

	for(i = 0; i < NSC_0x0103_arr_ARRY_SIZE; i++)
	{
		eeprom_write((NM_Configurations_OFFSET+i),Config_Value[i]);
	}
}

/* **********************************************************************************************************************
 * Function Name: read_NS_Can_Wakeup_Configuration_0x0104
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Can_Wakeup_Configuration_0x0104(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < NSC_0x0104_arr_ARRY_SIZE; i++)
	{
		NS_Can_Wakeup_Configuration_0x0104_arr[i] = eeprom_read(Can_Wake_Configurations_OFFSET + i);
	}

	for(i = 0; i < NSC_0x0104_arr_ARRY_SIZE; i++)
	{
		if(NS_Can_Wakeup_Configuration_0x0104_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == NSC_0x0104_arr_ARRY_SIZE)	/*Default value write*/
		{
			NS_Can_Wakeup_Configuration_0x0104_arr[0]=0x0A;
			NS_Can_Wakeup_Configuration_0x0104_arr[1]=0x0A;
			NS_Can_Wakeup_Configuration_0x0104_arr[2]=0x3;
			NS_Can_Wakeup_Configuration_0x0104_arr[3]=0x64;
			NS_Can_Wakeup_Configuration_0x0104_arr[4]=0x2;
			
			write_NS_Can_Wakeup_Configuration_0x0104(&NS_Can_Wakeup_Configuration_0x0104_arr[0]);
		}
	}
	IVN_cfg_CAN_Wakeup_Msg_Count = NS_Can_Wakeup_Configuration_0x0104_arr[0];
	IVN_cfg_Quick_Recovery_timeout_period = NS_Can_Wakeup_Configuration_0x0104_arr[1]*10;
	IVN_cfg_Quick_Recovery_retry_limit = NS_Can_Wakeup_Configuration_0x0104_arr[2];
	IVN_cfg_Slow_Recovery_timeout_period = NS_Can_Wakeup_Configuration_0x0104_arr[3]*10;
	IVN_cfg_Slow_Recovery_retry_limit = NS_Can_Wakeup_Configuration_0x0104_arr[4];
	
}

/* **********************************************************************************************************************
 * Function Name: write_NS_Can_Wakeup_Configuration_0x0104
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_Can_Wakeup_Configuration_0x0104(uint8_t *Config_Value)
{
	uint8_t i = 0;

	for(i = 0; i < NSC_0x0104_arr_ARRY_SIZE; i++)
	{
		eeprom_write((Can_Wake_Configurations_OFFSET+i),Config_Value[i]);
	}
}

//------> to comment the below function
/* **********************************************************************************************************************
 * Function Name: read_NS_Fault_Msg_Details_0x0105
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Fault_Msg_Details_0x0105(void)
{

	uint8_t i = 0;
	uint16_t Temp_IVN_cfg_Message_ID1;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg1;
	uint8_t Temp_IVN_cfg_NS_Status_Msg1; 
	uint16_t Temp_IVN_cfg_Message_ID2;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg2;
	uint8_t Temp_IVN_cfg_NS_Status_Msg2; 
	uint16_t Temp_IVN_cfg_Message_ID3;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg3;
	uint8_t Temp_IVN_cfg_NS_Status_Msg3;
	uint16_t Temp_IVN_cfg_Message_ID4;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg4;
	uint8_t Temp_IVN_cfg_NS_Status_Msg4;
	uint16_t Temp_IVN_cfg_Message_ID5;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg5;
	uint8_t Temp_IVN_cfg_NS_Status_Msg5;
	uint16_t Temp_IVN_cfg_Message_ID6;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg6;
	uint8_t Temp_IVN_cfg_NS_Status_Msg6;
	uint16_t Temp_IVN_cfg_Message_ID7;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg7;
	uint8_t Temp_IVN_cfg_NS_Status_Msg7;
	uint16_t Temp_IVN_cfg_Message_ID8;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg8;
	uint8_t Temp_IVN_cfg_NS_Status_Msg8;
	uint16_t Temp_IVN_cfg_Message_ID9;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg9;
	uint8_t Temp_IVN_cfg_NS_Status_Msg9;
	uint16_t Temp_IVN_cfg_Message_ID10;
	uint8_t Temp_IVN_cfg_Occurance_Count_Msg10;
	uint8_t Temp_IVN_cfg_NS_Status_Msg10;
	
	 

	for(i = 0; i < NSC_0x0105_arr_ARRY_SIZE; i++)
	{
		NS_Fault_Msg_Details_0x0105_arr[i] = eeprom_read(NS_Fault_Msg_Details_OFFSET + i);
	}

	Temp_IVN_cfg_Message_ID1 = (IVN_cfg_Message_ID1); // IVN_cfg_Message_ID1 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID1 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID1>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg1 = (IVN_cfg_Occurance_Count_Msg1); // IVN_cfg_Occurance_Count_Msg1 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg1<<3)|((Temp_IVN_cfg_Message_ID1>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg1 = (IVN_cfg_NS_Status_Msg1); // IVN_cfg_NS_Status_Msg1 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg1);
	
	Temp_IVN_cfg_Message_ID2 = (IVN_cfg_Message_ID2); // IVN_cfg_Message_ID2 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID2 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID2>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg2 = (IVN_cfg_Occurance_Count_Msg2); // IVN_cfg_Occurance_Count_Msg2 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg2<<3)|((Temp_IVN_cfg_Message_ID2>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg2 = (IVN_cfg_NS_Status_Msg2); // IVN_cfg_NS_Status_Msg2 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg2);
	
	Temp_IVN_cfg_Message_ID3 = (IVN_cfg_Message_ID3); // IVN_cfg_Message_ID3 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID3 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID3>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg3 = (IVN_cfg_Occurance_Count_Msg3); // IVN_cfg_Occurance_Count_Msg3 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg3<<3)|((Temp_IVN_cfg_Message_ID3>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg3 = (IVN_cfg_NS_Status_Msg3); // IVN_cfg_NS_Status_Msg3 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg3);
	
	Temp_IVN_cfg_Message_ID4 = (IVN_cfg_Message_ID4); // IVN_cfg_Message_ID4 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID4 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID4>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg4 = (IVN_cfg_Occurance_Count_Msg4); // IVN_cfg_Occurance_Count_Msg4 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg4<<3)|((Temp_IVN_cfg_Message_ID4>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg4 = (IVN_cfg_NS_Status_Msg4); // IVN_cfg_NS_Status_Msg4 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg4);
	
	Temp_IVN_cfg_Message_ID5 = (IVN_cfg_Message_ID5); // IVN_cfg_Message_ID5 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID5 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID5>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg5 = (IVN_cfg_Occurance_Count_Msg5); // IVN_cfg_Occurance_Count_Msg5 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg5<<3)|((Temp_IVN_cfg_Message_ID5>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg5 = (IVN_cfg_NS_Status_Msg5); // IVN_cfg_NS_Status_Msg5 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg5);
	
	Temp_IVN_cfg_Message_ID6 = (IVN_cfg_Message_ID6); // IVN_cfg_Message_ID6 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID6 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID6>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg6 = (IVN_cfg_Occurance_Count_Msg6); // IVN_cfg_Occurance_Count_Msg6 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg6<<3)|((Temp_IVN_cfg_Message_ID6>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg6 = (IVN_cfg_NS_Status_Msg6); // IVN_cfg_NS_Status_Msg6 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg6);
	
	Temp_IVN_cfg_Message_ID7 = (IVN_cfg_Message_ID7); // IVN_cfg_Message_ID7 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID7 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID7>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg7 = (IVN_cfg_Occurance_Count_Msg7); // IVN_cfg_Occurance_Count_Msg7 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg7<<3)|((Temp_IVN_cfg_Message_ID7>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg7 = (IVN_cfg_NS_Status_Msg7); // IVN_cfg_NS_Status_Msg7 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg7);
	
	Temp_IVN_cfg_Message_ID8 = (IVN_cfg_Message_ID8); // IVN_cfg_Message_ID8 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID8 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID8>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg8 = (IVN_cfg_Occurance_Count_Msg8); // IVN_cfg_Occurance_Count_Msg8 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg8<<3)|((Temp_IVN_cfg_Message_ID8>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg8 = (IVN_cfg_NS_Status_Msg8); // IVN_cfg_NS_Status_Msg8 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg8);
	
	Temp_IVN_cfg_Message_ID9 = (IVN_cfg_Message_ID9); // IVN_cfg_Message_ID9 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID9 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID9>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg9 = (IVN_cfg_Occurance_Count_Msg9); // IVN_cfg_Occurance_Count_Msg9 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg9<<3)|((Temp_IVN_cfg_Message_ID9>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg9 = (IVN_cfg_NS_Status_Msg9); // IVN_cfg_NS_Status_Msg9 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg9);
	
	Temp_IVN_cfg_Message_ID10 = (IVN_cfg_Message_ID10); // IVN_cfg_Message_ID10 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[0]= (Temp_IVN_cfg_Message_ID10 & 0xFF);
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Message_ID10>>8)&0x07);
	Temp_IVN_cfg_Occurance_Count_Msg10 = (IVN_cfg_Occurance_Count_Msg10); // IVN_cfg_Occurance_Count_Msg10 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[1]=((Temp_IVN_cfg_Occurance_Count_Msg10<<3)|((Temp_IVN_cfg_Message_ID10>>8)&0x07));
	Temp_IVN_cfg_NS_Status_Msg10 = (IVN_cfg_NS_Status_Msg10); // IVN_cfg_NS_Status_Msg10 --final value should replace here 
	NS_Fault_Msg_Details_0x0105_arr[2]= (Temp_IVN_cfg_NS_Status_Msg10);

}

/* **********************************************************************************************************************
 * Function Name: read_NS_NW_Msg_Config_0x010E
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_NW_Msg_Config_0x010E(void)
{
	uint8_t i = 0,def_var_cntr=0;

	for(i = 0; i < NSC_0x010E_arr_ARRY_SIZE; i++)
	{
		NS_NW_Msg_config_0x010E_arr[i] = eeprom_read(NW_Msg_Config_OFFSET + i);
	}

	for(i = 0; i < NSC_0x010E_arr_ARRY_SIZE; i++)
	{
		if(NS_NW_Msg_config_0x010E_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == NSC_0x010E_arr_ARRY_SIZE)	/*Default value write*/
		{
			NS_NW_Msg_config_0x010E_arr[0]=0;
			NS_NW_Msg_config_0x010E_arr[1]=0;
			
			
			write_NS_NW_Msg_Config_0x010E(&NS_NW_Msg_config_0x010E_arr[0]);
		}
	}
	Msg_enable_IC4_1000_Test = (NS_NW_Msg_config_0x010E_arr[0]&0x01);
	Msg_enable_IC6_1000_Test = ((NS_NW_Msg_config_0x010E_arr[0]&0x02)>>1);
	Msg_enable_IC7_1000_Test = ((NS_NW_Msg_config_0x010E_arr[0]&0x04)>>2);
	Msg_enable_IC_NMV_Test = ((NS_NW_Msg_config_0x010E_arr[0]&0x08)>>3);
	
	
}

/* **********************************************************************************************************************
 * Function Name: write_NS_NW_Msg_Config_0x010E
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_NW_Msg_Config_0x010E(uint8_t *Config_Value)
{
	uint8_t i = 0;

	for(i = 0; i < NSC_0x010E_arr_ARRY_SIZE; i++)
	{
		eeprom_write((NW_Msg_Config_OFFSET+i),Config_Value[i]);
	}
}

//--------> comment the below function
/* **********************************************************************************************************************
 * Function Name: read_NS_Fault_Type_Status_Info_Configuration_0x0106
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_Fault_Type_Status_Info_Configuration_0x0106(void)
{

	uint8_t i = 0;
	uint8_t Temp_IVN_cfg_FC_Not_matching;
	uint8_t Temp_IVN_cfg_RX_msg_Periodicity;
	uint8_t Temp_IVN_cfg_TX_msg_Received; 
	uint8_t Temp_IVN_cfg_NSM_Msg_Timout;
	uint8_t Temp_IVN_cfg_SK2_Not_matching;
	uint8_t Temp_IVN_cfg_Play_protection;
	uint8_t Temp_IVN_cfg_Reserved; 

	
	memset(&NS_Fault_Type_Status_Info_0x0106_arr[0], 0, NSC_0x0106_ARRY_SIZE);
	

	for(i = 0; i < NSC_0x0106_ARRY_SIZE; i++)
	{
		NS_Fault_Type_Status_Info_0x0106_arr[i] = eeprom_read(NS_Fault_Type_Config_OFFSET + i);
	}

	Temp_IVN_cfg_FC_Not_matching = (IVN_cfg_FC_Not_matching);  // IVN_cfg_FC_Not_matching --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_FC_Not_matching);
	
	Temp_IVN_cfg_RX_msg_Periodicity = (IVN_cfg_RX_msg_Periodicity);// IVN_cfg_RX_msg_Periodicity --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_RX_msg_Periodicity<<1);
	
	Temp_IVN_cfg_TX_msg_Received = (IVN_cfg_TX_msg_Received);// IVN_cfg_TX_msg_Received --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_TX_msg_Received << 2);
	
	Temp_IVN_cfg_NSM_Msg_Timout = (IVN_cfg_NSM_Msg_Timout); // IVN_cfg_NSM_Msg_Timout --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_NSM_Msg_Timout <<3);
	
	Temp_IVN_cfg_SK2_Not_matching = (IVN_cfg_SK2_Not_matching); // IVN_cfg_SK2_Not_matching --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_SK2_Not_matching <<4);
	
	Temp_IVN_cfg_Play_protection = (IVN_cfg_Play_protection); // IVN_cfg_Play_protection --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_Play_protection <<5);
	
	Temp_IVN_cfg_Reserved = (IVN_cfg_Reserved); // Diag_RW_TT_SET1_Immo --final value should replace here 
	NS_Fault_Type_Status_Info_0x0106_arr[1] |= (Temp_IVN_cfg_Reserved <<6);
	

}

/* **********************************************************************************************************************
 * Function Name: write_Default_PARA
 * Description  : This function read all default values.
 * Arguments    : none
 * Return Value : None
 ********************************************************************************************************************** */
void write_Default_PARA(void)
{
	/*write Feature Code 0 by default*/
	for(uint8_t i=0;i<20; i++)
	{
		Feature_code_arr[i] = 0;
	}
	write_feature_code(&Feature_code_arr[0]);

	/*write Variant Code 0 by default*/
	for(uint8_t i=0;i<20; i++)
	{
		Varient_code_arr[i] = 0;
	}
	write_varient_code(&Varient_code_arr[0]);

	/*IFE Parameters*/
	DID_IFE_ECUIdentifier_0x0520_arr[0] = 20;	//This is the initial threshold time after which the calculation of IFE will begin
	DID_IFE_ECUIdentifier_0x0520_arr[1] = 1;	//This is the interval at which the fuel consumption and distance covered samples are taken
	DID_IFE_ECUIdentifier_0x0520_arr[2] = 2;	//Size of the rolling average window
	DID_IFE_ECUIdentifier_0x0520_arr[3] = 1;	//This is the interval of the IFE display
	DID_IFE_ECUIdentifier_0x0520_arr[4] = 30;	//This is the Max IFE Display value.
	New_eeprom_memwrite_32bytes(DID_0520_OFFSET, DID_IFE_ECUIdentifier_0x0520_arr, ECUIdentifier_0x0520_ARRY_SIZE);

	/*AFE Parameters*/
	DID_AFE_ECUIdentifier_0x0525_arr[0] = 100;	// FC_Factor
	DID_AFE_ECUIdentifier_0x0525_arr[1] = 5;	// AFE_CALC_INTERVAL
	DID_AFE_ECUIdentifier_0x0525_arr[2] = 60;	// AFE_DISPLAY_INTERVAL
	DID_AFE_ECUIdentifier_0x0525_arr[3] = 10;	// AFE_DISPLAY_FILTER_A
	DID_AFE_ECUIdentifier_0x0525_arr[4] = 50;//5;	// AFE_DISPLAY_FILTER_B 0 to 7 bits
	DID_AFE_ECUIdentifier_0x0525_arr[5] = 0;	// AFE_DISPLAY_FILTER_B 8 to 15 bits
	DID_AFE_ECUIdentifier_0x0525_arr[6] = 0;	// AFE_DRIVE_CYCLE_RESET_ENABLE.
	DID_AFE_ECUIdentifier_0x0525_arr[7] = 2;	// AFE_DRIVE_CYCLE_RESET_INTERVAL.
	DID_AFE_ECUIdentifier_0x0525_arr[8] = 10;	// AFE_INIT_THRESHOLD_DISTANCE.
	New_eeprom_memwrite_32bytes(DID_0525_OFFSET, DID_AFE_ECUIdentifier_0x0525_arr, ECUIdentifier_0x0525_ARRY_SIZE);

	/*Service Distance Slab config.*/
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0] = 8;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[1] = 90;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[2] = 100;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[3] = 100;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[4] = 100;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[5] = 100;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[6] = 100;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[7] = 100;
	DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[8] = 50;
	New_eeprom_memwrite_32bytes(DID_050F_OFFSET, DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr, ECUIdentifier_0x050F_ARRY_SIZE);

	/*DTE-AFE-TRIP Parameters*/
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0] = 50;	// DTE_AFE_AVG_KM
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[1] = 14;	// DTE_DEFAULT_FE
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[2] = 5;	// DTE_DISPLAY_FILTER_A
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[3] = 20;	// DTE_DISPLAY_FILTER_B
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[4] = 50;	// DTE_DISPLAY_FILTER_C
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[5] = 230/*30*/;	// DTE_AFE_MAX
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[6] = 0/*7*/;	// DTE_AFE_MIN
	DID_DTE_AFE_ECUIdentifier_0x04D2_arr[7] = 0;	// Unit Selection
	New_eeprom_memwrite_32bytes(DID_04D2_OFFSET, DID_DTE_AFE_ECUIdentifier_0x04D2_arr, ECUIdentifier_0x04D2_ARRY_SIZE);

	/*Over speed Parameters*/
	DID_OVERSPEED_0x04F7_arr[0] = 80;	//OverSpeedLevel1Val
	DID_OVERSPEED_0x04F7_arr[1] = 120;	//OverSpeedLevel2Val
	DID_OVERSPEED_0x04F7_arr[2] = 1;	//OverSpeedLevel1Type
	DID_OVERSPEED_0x04F7_arr[3] = 60;	//OverSpeedLv1RepeatIntrval
	DID_OVERSPEED_0x04F7_arr[4] = 2;	//OverSpeedLevel2Type
	DID_OVERSPEED_0x04F7_arr[5] = 2;	//OverSpeedLv2RepeatIntrval
	DID_OVERSPEED_0x04F7_arr[6] = 5;	//OverSpeedHystresis
	New_eeprom_memwrite_32bytes(DID_04F7_OFFSET, DID_OVERSPEED_0x04F7_arr, ECUIdentifier_0x04F7_ARRY_SIZE);

	/*N/W security Parameters*/
	NS_Network_Security_key_SKC_0x0108_arr[0]=0x00;
	NS_Network_Security_key_SKC_0x0108_arr[1]=0x00;
	NS_Network_Security_key_SKC_0x0108_arr[2]=0x00;
	NS_Network_Security_key_SKC_0x0108_arr[3]=0x00;
	write_NS_Network_Security_key_SKC_0x0108(&NS_Network_Security_key_SKC_0x0108_arr[0]);

	/*N/W security Parameters*/
	NS_Network_Security_key_SKC1_0x0109_arr[0]=0x00;
	NS_Network_Security_key_SKC1_0x0109_arr[1]=0x00;
	NS_Network_Security_key_SKC1_0x0109_arr[2]=0x00;
	NS_Network_Security_key_SKC1_0x0109_arr[3]=0x00;
	write_NS_Network_Security_key_SKC1_0x0109(&NS_Network_Security_key_SKC1_0x0109_arr[0]);

	/*N/W security Parameters*/
	NS_Network_Security_key_SKC2_0x010A_arr[0]=0x00;
	NS_Network_Security_key_SKC2_0x010A_arr[1]=0x00;
	NS_Network_Security_key_SKC2_0x010A_arr[2]=0x00;
	NS_Network_Security_key_SKC2_0x010A_arr[3]=0x00;
	write_NS_Network_Security_key_SKC2_0x010A(&NS_Network_Security_key_SKC2_0x010A_arr[0]);

	/*N/W security Parameters*/
	NS_Network_Security_Configuration_0x010B_arr[0]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[1]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[2]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[3]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[4]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[5]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[6]=0x1;
	NS_Network_Security_Configuration_0x010B_arr[7]=0x0A;
	NS_Network_Security_Configuration_0x010B_arr[8]=0x5;
	NS_Network_Security_Configuration_0x010B_arr[9]=0x0A;
	NS_Network_Security_Configuration_0x010B_arr[10]=0x5;
	write_NS_Network_Security_Configuration_0x010B(&NS_Network_Security_Configuration_0x010B_arr[0]);

	/*AVG speed Parameters*/
	DID_AVG_SPEED_0x0511_arr[0] = 100;				//AVS_INITIAL_SAMPLE_DISTANCE
	DID_AVG_SPEED_0x0511_arr[1] = 10;				//AVS_SAMPLE_TIME
	New_eeprom_memwrite_32bytes(DID_0511_OFFSET, DID_AVG_SPEED_0x0511_arr, ECUIdentifier_0x0511_ARRY_SIZE);


	/*SeatBelt reminder Parameters*/
	DID_SEAT_BELT_REMAINDER_0x0507_arr[0] = 16;
	DID_SEAT_BELT_REMAINDER_0x0507_arr[1] = 60;
	DID_SEAT_BELT_REMAINDER_0x0507_arr[2] = 2;
	DID_SEAT_BELT_REMAINDER_0x0507_arr[3] = 60;
	DID_SEAT_BELT_REMAINDER_0x0507_arr[4] = 50;
	DID_SEAT_BELT_REMAINDER_0x0507_arr[5] = 25;
	DID_SEAT_BELT_REMAINDER_0x0507_arr[6] = 35;
	write_SEAT_BELT_REMAINDER(&DID_SEAT_BELT_REMAINDER_0x0507_arr[0]);


	/*Fuel Parameters*/
	for(uint8_t i=0;i<16;i++)
	{
		DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[i]=DID_FUEL_BAR_DEFAULT_VALUE_0x050B_arr[i];
	}
	write_FUEL_BAR_VALUE(&DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0]);
	for(uint8_t i=0;i<4;i++)
	{
		DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[i]=DID_FUEL_TT_DEFAULT_VALUE_0x050D_arr[i];
	}
	write_FUEL_TT_VALUE(&DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0]);
	fuel_did();

	/*Temp. Parameters*/
	for(uint8_t i=0;i<16;i++)
	{
		DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[i]=DID_TEMP_BAR_DEFAULT_VALUE_0x050C_arr[i];
	}
	write_TEMP_BAR_VALUE(&DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0]);
	for(uint8_t i=0;i<4;i++)
	{
		DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[i] = DID_TEMP_TT_DEFAULT_VALUE_0x050E_arr[i];
	}
	write_TEMP_TT_VALUE_CONFIG(&DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0]);

}


/* **********************************************************************************************************************
 * Function Name: read_NS_CAN_WEKUP_Configuration_PKE_0x010D
 * Description  : This function read SPAREPART NUMBER data.
 * Arguments    : *SPAREPART_NUMBER
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void read_NS_CAN_WEKUP_Configuration_PKE_0x010D(void)
{
	uint8_t i = 0,def_var_cntr=0;

/***************************/

	for(i = 0; i < CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE; i++)
	{
		NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[i] = eeprom_read(CAN_WEKUP_Configuration_PKE_OFFSET + i);
	}

	for(i = 0; i < CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE; i++)
	{
		if(NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[i] == 0xFF)
		{
			//def_var_cntr++;
		}
		if(def_var_cntr == CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE)	/*Default value write*/
		{
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0]=0x1E;   // 30- 0x1E
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[1]=0x00;   // 400 - 0x190
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[2]=0x50;
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[3]=0x00;   // 60 - 0x3C
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[4]=0x0C;
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[5]=0x01;   // 2000 -0x07D0
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[6]=0x90;
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[7]=0x01;   // 2000 -0x07D0
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[8]=0x90;
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[9]=0x00;    // 10 -0x0A
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[10]=0x02;
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[11]=0x00;   // 20- 0x14
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[12]=0x04;

			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[13]=0x00;   // 10 -0x0A
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[14]=0x02;
			
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[15]=0x00;    //100 - 0x64
			NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[16]=0x14;			
			write_NS_CAN_WEKUP_Configuration_PKE_0x010D(&NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0]);
		}
	}
	
	IVN_Cfg_T_MBFM_PKE_COMMAND_TIME = NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0];
    IVN_Cfg_T_WAKEUP_WAIT_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[1] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[2]) * 5);
	IVN_Cfg_T_WAKEUP_SYNC_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[3] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[4]) * 5);
	IVN_Cfg_T_SLEEP_WAIT1_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[5] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[6]) * 5);
	IVN_Cfg_T_KEEP_NORMAL_MIN_TIMEOUT = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[7] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[8]) * 5); 
	IVN_Cfg_T_START_WAKEUPTX_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[9] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[10]) * 5);
	IVN_Cfg_T_WAKEUPTX_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[11] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[12]) * 5);
	IVN_Cfg_T_START_ACTIVETX_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[13] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[14]) * 5);
	IVN_Cfg_T_ACTIVETX_Time = (((NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[15] << 8)| NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[16]) * 5);
	
	
}

/* **********************************************************************************************************************
 * Function Name: write_NS_CAN_WEKUP_Configuration_PKE_0x010D
 * Description  : This function write feature code data.
 * Arguments    : *feature_code
 * Return Value : None
 ********************************************************************************************************************** */
PRIVATE void write_NS_CAN_WEKUP_Configuration_PKE_0x010D(uint8_t *Security_key)
{
	uint8_t i = 0;

	for(i = 0; i < CAN_WEKUP_CONFIG_PKE_0x010D_ARRY_SIZE; i++)
	{
		eeprom_write((CAN_WEKUP_Configuration_PKE_OFFSET+i),Security_key[i]);
	}
}

