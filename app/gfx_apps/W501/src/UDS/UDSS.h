/*
 * UDSS.H
 *
 *  Created on: Jan 14, 2019
 *      Author: MSWELT12
 */

#ifndef UDSS_H_
#define UDSS_H_

/* **********************************************************************************************************************
File            :       UDSS.h
Author          :       Akshay Wankhade
Model           :       Mahindra W501
Revision Author :
Reviewed By     :
Company         :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune
/* **********************************************************************************************************************/
#define 	PRIVATE				static
#define		PUBLIC



typedef union
{
  unsigned char BYTEs[4];
  unsigned int BYTES[2];
  unsigned long DTC_STATUS;
}DTC;

typedef struct
{
	uint32_t Timeout_Counter;
	uint32_t Timeout_Threshold;
	uint32_t Recovery_Counter;
	uint32_t Recovery_Threshold;
	uint16_t  Message_Received_Counter;
	uint16_t  Message_Received_Threshold;
	uint8_t  Message_Timeout_Status;
	uint8_t  Message_Recovery_Status;
}Message_Timeout_t;


extern DTC DTC_SYSTEM_CHECK_ENGINE_TT_A002;


extern void Process_UDSData(void);
//#if 0
extern void Bit_flag_Access(void);
extern void Respond_Single_Frame(uint8_t);
extern void Save_NegResponse(uint8_t,uint8_t);
extern void Process_UDSData(void);
extern void MyErrorHandler(void);
extern void Clear_UDS_variables(void);
extern void send_possitive_response_reset(void);
extern void Respond_Multiple_Frames(uint16_t);
void seedFlowControl(void);
void control_dtc_fun(void);
void communication_control_fun(void);
void updateAppVersion(void);
void Write_HW_version(uint8_t *appAddr);
void Read_HW_version(void);
void updateHWVersion(void);

//extern void WriteDataByIdentifier(uint8_t DID);
//extern void WriteDataByIdentifier_multipleframes();
//extern void udss_Readdatabyidentifier();
/*extern unsigned long seed_and_key_algorithm(void);*/
//#endif
extern void Write_UDS_Data(void);
PUBLIC void read_dtcs(void);

extern uint8_t PROGRAMMING_FLAG;

extern uint8_t wdt_reset_cntr;
extern uint8_t write_seed;
extern uint16_t STminval;
extern uint8_t STmin_cntr;

extern _Bool ext_session_active;
extern _Bool Security_Access_Success;
extern _Bool stop_communication;
extern _Bool DTC_stop;

extern _Bool UDS_IO_Control_0xF093_Flag;
extern _Bool UDS_IO_Control_0xF092_Flag;

extern _Bool Functional_Addressing_Flag;
extern uint16_t P2CAN_ECU_TIMEOUT;
extern uint8_t history_zone[25];
extern uint8_t BOOT_FLAG_STATUS;
extern uint8_t ACTIVE_DIAG_SESSION;

extern uint8_t UDS_TX_DATA[3500];
extern uint8_t History_zone_Full[125];
extern uint8_t Rx_multiplebytes[1500];

extern _Bool disable_can_rx_flag;
extern _Bool disable_can_tx_flag;
extern _Bool disable_can_rx_flag_req;
extern _Bool disable_can_tx_flag_req;
extern _Bool prog_session_active;
extern _Bool dtc_log_lock_flag;
extern uint8_t IO_TFT_TEST;
#define 	DTC_CNF							0x08
#define 	DTC_CNF_TEST_FAIL					0x09
#define		DTC_TEST_FAIL						0x01

typedef union 
{
  unsigned char Bytes_in_seed[8];
  unsigned long long seed_as_a_whole;  
}seed_value_level3;

typedef union 
{
  unsigned char Bytes_in_seed[8];
  unsigned long seed_as_a_whole;  
} seed_value;

extern seed_value Seed;
extern seed_value_level3 Seed_level3;
//extern seed_valuel3;
//PRIVATE void random_valuel3(seed_valuel3* randaom_value);
/*
typedef union
{
  unsigned char Bytes_Hour_value[4];
  unsigned long Hour_value;
} HOUR_METER;


typedef union
{
  unsigned char BYTE[4];
  unsigned int BYTES[2];
  unsigned long DTC_STATUS;
}DTC;

typedef union
{
  unsigned char CURRENT_RPM_BYTE[2];
  unsigned int CURRENT_RPM;
}RPM_VALUE;
 */

//extern uint8_t history_zone[25];
//extern uint8_t UDS_TX_DATA[250];
//extern uint8_t Rx_multiplebytes[1500];
#if 0
extern __boolean REQUEST_3SEC;
extern __boolean REQUEST_5SEC;
#endif
/*
union Uds_data
{
  struct{

      unsigned char  UDS_Tx_0:1;
      unsigned char  UDS_Tx_1:1;
      unsigned char  UDS_Tx_2:1;
      unsigned char  UDS_Tx_3:1;
      unsigned char  UDS_Tx_4:1;
      unsigned char  UDS_Tx_5:1;
      unsigned char  UDS_Tx_6:1;
      unsigned char  UDS_Tx_7:1;
  }bits[8];
  unsigned char byte[8];
};*/

#define FALSE 0
#define TRUE 1
//extern seed_value Seed;
#define APPLICATION_START_ADDR  0x0000
#define BLOCK_LENGTH_36         1024
#define USER_ROM                0x00004000
#define START_BLOCK             16
#define END_BLOCK               96
/*UDS SERVICES*/
#define SESSION_CONTROL         0x10
#define SECURITY_ACCESS         0x27
#define ROUTINE_CONTROL         0x31
#define ACCESS_TIMINGPARAM      0x83
#define REQUEST_DWNLD           0x34
#define TRANSFER_DATA           0x36
#define TRANSFER_EXIT           0x37
#define ECU_RESET               0x11
#define TESTER_PRESENT			0x3E
#define CONTROL_DTC				0x85U
#define COMMUNICATION_CONTROL	0x28U
#define COMM_CONTROL            0x28
#define CONTROL_DTC             0x85
#define CLEAR_DTC               0x14
#define WRITE_DATA_ID           0x2E
#define READ_DATA_BY_IDENTIFIER 0x22
#define IOCONTROL               0x2F
#define READ_DTC                0x19
//#define TRUE                    0x01
//#define FALSE                   0x00
#define FC_BLOCK_SIZE           0x00
#define STmin                   0x04	//0x02
#define DEFAULT_SESSION         0x01
#define PROGRAMMING_SESSION     0x02
#define EXTENDED_SESSION        0x03
#define EXTENDED_SESSION_SPR	0x83
#define REQUEST_SEED            0x01
#define SEND_KEY_8_BYTE			0x05U
#define SEND_KEY                0x02

#define REQUEST_SEED_LEVEL3     0x05U
#define SEND_KEY_LEVEL3		0x06U
#define START_ROUTINE           0x01
#define STOP_ROUTINE            0x02
#define REQUEST_RESULT          0x03
#define SAVE_TIMINGPARAM        0x04
#define HARD_RESET              0x01
#define SOFT_RESET              0x03
#define SHORT_TERM_ADJ          0x03
#define FREEZE_STATE            0x02
#define RESET_DEFAULT           0x01
#define RETURN_CONTROL          0x00

#define NRC_SRNS_0x11			0x11
#define NRC_SFNC_0x12           0x12
#define NRC_ICDL_0x13           0x13
#define NRC_WAIT_0x78           0x78
#define NRC_SNS_0x7F            0x7F
#define NRC_ENA_0x36            0x36
#define NRC_WRSQ_0x24           0x24
#define NRC_SFNS_0x7E           0x7E
#define NRC_SAD_0x33            0x33
#define NRC_ROFR_0x31           0x31
#define NRC_CNC_0x22            0x22
#define NRC_TDSP_0x71           0x71
#define NRC_GPF_0x72            0x72
#define NRC_WBSC_0x73           0x73
#define NRC_WKEY_0x35           0x35
#define NRC_EXCEEDATT_0x36      0x36
#define NRC_RTDNE_0x37          0x37
#define NRC_EIR_0x83			0x83
#define NRC_VSTH_0x88			0x88

//#define DID_BOOT_FLAG_0x0200   0x0200
#define DID_ERASE_FLASH_0xFF00  0xFF00
#define COMPARE_CHECKSUM        0xFF01
//#define DID_UPDATEHISTORY_0x0201        0x0201

/* DID for Read dat by identifier*/

#define DID_BOOT_FLAG_0x0200                    0x0200 /**/
#define DID_ACTIVE_DIAG_SESSION_0xF186          0xF186
#define DID_UPDATEHISTORY_0x0201                0x0201
#define DID_DEF_BAR_NUMBER_0x050B               0x050B /**/
#define DID_DEF_LEVEL_PERCENT_0x050C            0x050C /**/
#define DID_ENGINE_RUN_HOUR_0x050D              0x050D /**/
#define DID_ENGINE_TEMP_ACTUAL_0x050E           0x050E
#define DID_SERVICE_REMINDER_0x051F             0x051F
//#define VIN_NUM                               0xF190


/*********** ECU IDENTIFICATION **************/

#define DID_BOOT_SOFTWARE_IDENTIFICATION_0xF180         0xF180
#define DID_APP_SOFTWARE_IDENTIFICATION_0xF181          0xF181
#define DID_APP_DATA_IDENTIFICATION_0xF182              0xF182
#define DID_VEH_MANU_SPAREPART_NUMBER_0xF187            0xF187
#define DID_SYS_SUPPLIER_IDENTIFIER_0xF18A              0xF18A
#define DID_ECU_MANUFACTURING_DATE_RW_0xF18B            0xF18B
#define DID_ECU_SERIAL_NUMBER_0xF18C                    0xF18C
#define DID_VIN_RW_0xF190                               0xF190
#define DID_ECU_HARDWARE_NUMBER_0xF192                  0xF192
#define DID_ECU_HARDWARE_VERSION_NUMBER_0xF193          0xF193
#define DID_ECU_SOFTWARE_NUMBER_0xF194                  0xF194
#define DID_ECU_SOFTWARE_VERSION_NUMBER_0xF195          0xF195
#define DID_ENGINE_TYPE_RW_0xF197                       0xF197
#define DID_TESTER_SERIAL_NUMBER_RW_0xF198              0xF198
#define DID_PROGRAMMING_DATE_RW_0xF199                  0xF199
#define DID_ECU_INSTALLATION_DATE_RW_0xF19D             0xF19D
//#define DID_SYSTEM_SUPPLIER_PART_NUMBER_0xF1F0        0xF1F0
#define DID_MODEL_NUMBER_RW_0x0100                      0x0100
#define DID_VARIANT_CODE_RW_0x0101                      0x0101
#define DID_FEATURE_CODE_RW_0x0102                      0x0102
#define NS_VARIANT_CODE_RW_0x0107				    	0x0107
#define DID_SYSTEM_SUPPLIER_PART_NO_0xF1F0				0xF1F0
//#define DID_ECU_MANUFACTURING_DATE_RW_0xF18B          0xF18B
#define DID_ROUTINE_ID_0xF090                           0xF090
#define ECU_RESET_LENGTH								2U
#define COMMUNICATION_CONTROL_LENGTH					3U
#define CONTROL_DTC_SETTING_LENGTH						2U
#define CLEAR_ALL_DTC_LENGTH							4U
#define READ_DTC_LENGTH									3U
#define READ_DTC_FREEZE_FRAME_LENGTH					6U
#define TRANSFER_EXIT_LENGTH							1U
#define NS_NSC_0x010B									0x010B

/******************************* W501 Idetifires ********************************/
#define DID_GRAPHIC_VERSION_0xF1AF                      0xF1AF
//#define DID_BOOT_FLAG_0x0200                          0x0200
//#define DID_ACTIVE_DIAG_SESSION_0xF186                0xF186

/*ECU Identifiers start below*/

#define DID_CHIME_CONFIG_0x0503                			0x0503
#define DID_MESSAGE_CONFIG_0x0504                		0x0504
#define DID_SEAT_BELT_CONFIG_0x0507            			0x0507
#define DID_RPM_CHIME_CONFIG_0x0508                		0x0508
#define DID_SPEED_CHIME_CONFIG_0x0509              		0x0509
#define DID_TDMS_CONFIG_0x0510              			0x0510
#define DID_AVG_SPEED_CONFIG_0x0511              		0x0511
#define DID_IFE_CONFIG_0x0520              				0x0520
#define DID_OSEK_NM_CONFIG_0x0521              			0x0521
#define DID_RPAS_CONFIG_0x0522              			0x0522
#define DID_AFE1_CONFIG_0x0523              			0x0523
#define DID_AFE_GD_CONFIG_0x0524              			0x0524
#define DID_AFE2_CONFIG_0x0525              			0x0525
#define DID_FUEL_BAR_VALUE_CONFIG_0x050B                0x050B
#define DID_TEMP_BAR_VALUE_CONFIG_0x050C                0x050C
#define DID_FUEL_TT_VALUE_CONFIG_0x050D                 0x050D
#define DID_TEMP_TT_VALUE_CONFIG_0x050E                 0x050E
#define DID_SERVICE_SLAB_CONFIG_0x050F              	0x050F
#define DID_TELLTALE_FB_CONFIG_0x04CF              	    0x04CF
#define DID_DTE_CONFIG_0x04D0              				0x04D0
#define DID_FUEL_GEAN_INDICATOR_CONFIG_0x04D1           0x04D1
#define DID_AFE_DTE_CONFIG_0x04D2              			0x04D2
#define DID_FUEL_DEAD_VOLUME_CONFIG_0x04D5				0x04D5
#define DID_NVM_SERVICE_DONE_CONFIG_0x04F3				0x04F3
#define DID_ODOMETER_RAW_CONFIG_0x04F4					0x04F4
#define DID_DID_ODOMETER_PERCENTAGE_CONFIG_0x04F6		0x04F6
#define DID_OVERSPEED_CONFIG_0x04F7						0x04F7
#define DID_TPMS_TELLTALE_CONFIG_0x04F8					0x04F8
#define DID_FUEL_CONS_ACCM_CONFIG_0x04F9				0x04F9
#define DID_TELL_TALE_STATUS_SET1_CONFIG_0xF07F         0xF07F
#define DID_TELL_TALE_STATUS_SET2_CONFIG_0xF080			0xF080
#define DID_TELL_TALE_STATUS_SET3_CONFIG_0xF081			0xF081
#define DID_TELL_TALE_STATUS_SET4_CONFIG_0xF08E			0xF08E
#define DID_TELL_TALE_STATUS_SET5_CONFIG_0xF08F			0xF08F
#define DID_GAUGES_CONFIG_0xF082						0xF082
#define DID_ILLUMINATION_STATUS_CONFIG_0xF084			0xF084
#define DID_BAR_GRAPH_STATUS_CONFIG_0xF083				0xF083
#define DID_GEAR_INDICATION_STATUS_CONFIG_0xF085		0xF085
#define DID_DISPLAY_IO_CONTROL_0xF086					0xF086
#define DID_CHIME_ACTIATION_IO_CONTROL_0xF094			0xF094
#define DID_MESSAGE_ACTIATION_IO_CONTROL_0xF095			0xF095
#define DID_TELL_TALE_STATUS_SET6_CONFIG_0xF096			0xF096
#define DID_ODOMETER_CONFIG_0xF1B0						0xF1B0
#define DID_BATTERY_VOLTAGE_CONFIG_0xF1B1				0xF1B1
#define DID_VEHICLE_SPEED_CONFIG_0xF1B2					0xF1B2
#define DID_ENGINE_SPEED_CONFIG_0xF1B3					0xF1B3
#define DID_RESET_REASON_CONFIG_0xF1B4					0xF1B4
#define DID_RESET_COUNTER_CONFIG_0xF1B5					0xF1B5
#define DID_IGNITION_COUNTER_CONFIG_0xF1B6				0xF1B6
#define DID_EEPROM_LAYOUT_CONFIG_0xF1B7					0xF1B7
//ETM
#define DID_ETM_CONFIG_0x0500							0x0500
#define DID_TT_BLINK_PATTERN3_0x0502					0x0502
#define DID_TT_BLINK_PATTERN4_0x04FB					0x04FB
#define DID_TT_BLINK_PATTERN5_0x04FC					0x04FC
#define DID_TT_BLINK_PATTERN8_0x04FD					0x04FD
#define DID_TT_BLINK_PATTERN9_0x04FE					0x04FE
#define DID_ROLL_PITCH_DETECTION_0x0533					0x0533
#define DID_AT_GEAR_ALERT_0x0543						0x0543

#define NS_Network_Security_key_SKC_0x0108              0x0108
#define NS_Network_Security_key_SKC1_0x0109             0x0109
#define NS_Network_Security_key_SKC2_0x010A             0x010A
#define NS_Network_Mgmt_Config_0x0103					0x0103
#define NS_CAN_Wake_Up_Config_0x0104					0x0104
#define NS_Network_Security_Fault_MSG_Detail_0x0105		0x0105
#define NS_Network_Security_Fault_Type_STS_Info_0x0106	0x0106
#define NS_CAN_Wake_Up_Config_PKE_0x010D				0x010D
#define NS_Network_MSG_Config_0x010E					0x010E
/***************************** W501 bitflag of feature code***********************/

typedef union
{
	struct
	{
		unsigned char  UDS_Msg_0:1;
		unsigned char  UDS_Msg_1:1;
		unsigned char  UDS_Msg_2:1;
		unsigned char  UDS_Msg_3:1;
		unsigned char  UDS_Msg_4:1;
		unsigned char  UDS_Msg_5:1;
		unsigned char  UDS_Msg_6:1;
		unsigned char  UDS_Msg_7:1;
	}bits;
	unsigned char byte;
}bit_flag;

extern bit_flag  feature_code_1stbyte;
extern bit_flag  feature_code_2ndbyte;
extern bit_flag  feature_code_3rdbyte;
extern bit_flag  feature_code_4thbyte;
extern bit_flag  feature_code_5thbyte;
extern bit_flag  feature_code_6thbyte;
extern bit_flag  feature_code_7thbyte;
extern bit_flag  feature_code_8thbyte;
//#ifdef SPRINT4
extern bit_flag  feature_code_10thbyte;
extern bit_flag  feature_code_11thbyte;
extern bit_flag  feature_code_13thbyte;
//#endif
extern bit_flag  feature_code_16thbyte;
extern bit_flag  feature_code_17thbyte;
extern bit_flag  Varient_code_1st_byte;
extern bit_flag  Varient_code_2nd_byte;
extern bit_flag  Varient_code_4th_byte;
extern bit_flag  Varient_code_5th_byte;
extern bit_flag  Fuel_Bar_Status_1st_byte;
extern bit_flag  Fuel_Bar_Status_2nd_byte;
extern bit_flag  Fuel_Bar_Status_3rd_byte;
extern bit_flag  Fuel_Bar_Status_4th_byte;
extern bit_flag  Fuel_Bar_Status_5th_byte;
extern bit_flag  Fuel_Bar_Status_6th_byte;
extern bit_flag  Fuel_Bar_Status_7th_byte;
extern bit_flag  Fuel_Bar_Status_8th_byte;
extern bit_flag  Fuel_Bar_Status_9th_byte;
extern bit_flag  Fuel_Bar_Status_10th_byte;
extern bit_flag  Fuel_Bar_Status_11th_byte;
extern bit_flag  Fuel_Bar_Status_12th_byte;
extern bit_flag  Fuel_Bar_Status_13th_byte;
extern bit_flag  Fuel_Bar_Status_14th_byte;
extern bit_flag  Fuel_Bar_Status_15th_byte;
extern bit_flag  Fuel_Bar_Status_16th_byte;
extern bit_flag  Temp_Bar_Status_1st_byte;
extern bit_flag  Temp_Bar_Status_2nd_byte;
extern bit_flag  Temp_Bar_Status_3rd_byte;
extern bit_flag  Temp_Bar_Status_4th_byte;
extern bit_flag  Temp_Bar_Status_5th_byte;
extern bit_flag  Temp_Bar_Status_6th_byte;
extern bit_flag  Temp_Bar_Status_7th_byte;
extern bit_flag  Temp_Bar_Status_8th_byte;
extern bit_flag  Temp_Bar_Status_9th_byte;
extern bit_flag  Temp_Bar_Status_10th_byte;
extern bit_flag  Temp_Bar_Status_11th_byte;
extern bit_flag  Temp_Bar_Status_12th_byte;
extern bit_flag  Temp_Bar_Status_13th_byte;
extern bit_flag  Temp_Bar_Status_14th_byte;
extern bit_flag  Temp_Bar_Status_15th_byte;
extern bit_flag  Temp_Bar_Status_16th_byte;
extern bit_flag  Fuel_Tell_Tale_value_1st_byte;
extern bit_flag  Fuel_Tell_Tale_value_2nd_byte;
extern bit_flag  Fuel_Tell_Tale_value_3rd_byte;
extern bit_flag  Fuel_Tell_Tale_value_4th_byte;
extern bit_flag  Temp_Tell_Tale_value_1st_byte;
extern bit_flag  Temp_Tell_Tale_value_2nd_byte;
extern bit_flag  Temp_Tell_Tale_value_3rd_byte;
extern bit_flag  Temp_Tell_Tale_value_4th_byte;

//IVN VARIENT CODE
extern bit_flag  NS_Varient_code_1st_byte;
extern bit_flag  NS_Varient_code_2nd_byte;
extern bit_flag  NS_Varient_code_3rd_byte;
extern bit_flag  NS_Varient_code_4th_byte;
extern bit_flag  NS_Varient_code_5th_byte;
extern bit_flag  NS_Varient_code_6th_byte;
extern bit_flag  NS_Varient_code_7th_byte;
extern bit_flag  NS_Varient_code_8th_byte;
extern bit_flag  NS_Varient_code_9th_byte;
extern bit_flag  NS_Varient_code_10th_byte;
extern bit_flag  NS_Varient_code_11th_byte;
extern bit_flag  NS_Varient_code_12th_byte;
extern bit_flag  NS_Varient_code_13th_byte;
extern bit_flag  NS_Varient_code_14th_byte;
extern bit_flag  NS_Varient_code_15th_byte;
extern bit_flag  NS_Varient_code_16th_byte;
extern bit_flag  NS_Varient_code_17th_byte;
extern bit_flag  NS_Varient_code_18th_byte;
extern bit_flag  NS_Varient_code_19th_byte;
extern bit_flag  NS_Varient_code_20th_byte;

//DID
//Chime Configuration 503//
extern bit_flag  Chime_503_0th_byte;
extern bit_flag  Chime_503_1st_byte;
extern bit_flag  Chime_503_2nd_byte;
extern bit_flag  Chime_503_3rd_byte;
extern bit_flag  Chime_503_4th_byte;
extern bit_flag  Chime_503_5th_byte;
extern bit_flag  Chime_503_6th_byte;
extern bit_flag  Chime_503_7th_byte;
extern bit_flag  Chime_503_8th_byte;
extern bit_flag  Chime_503_9th_byte;

//Message Configuration 503//
extern bit_flag  Message_504_0th_byte;
extern bit_flag  Message_504_1st_byte;
extern bit_flag  Message_504_2nd_byte;
extern bit_flag  Message_504_3rd_byte;
extern bit_flag  Message_504_4th_byte;
extern bit_flag  Message_504_5th_byte;
extern bit_flag  Message_504_6th_byte;
extern bit_flag  Message_504_7th_byte;
extern bit_flag  Message_504_8th_byte;
extern bit_flag  Message_504_9th_byte;
extern bit_flag  Message_504_10th_byte;
extern bit_flag  Message_504_11th_byte;
extern bit_flag  Message_504_12th_byte;
extern bit_flag  Message_504_13th_byte;
extern bit_flag  Message_504_14th_byte;
extern bit_flag  Message_504_15th_byte;

//ETM
extern bit_flag Message_500_0th_byte;

extern uint8_t   IO_Parameter_Request_Status;
extern bit_flag  IO_Control_State_TT_SET1;
extern bit_flag  IO_Control_Enable_TT_SET1;
extern bit_flag  IO_Control_State_TT_SET2;
extern bit_flag  IO_Control_Enable_TT_SET2;
extern bit_flag  IO_Control_State_TT_SET3;
extern bit_flag  IO_Control_Enable_TT_SET3;
extern bit_flag  IO_Control_State_TT_SET4;
extern bit_flag  IO_Control_Enable_TT_SET4;
extern bit_flag  IO_Control_State_TT_SET5;
extern bit_flag  IO_Control_Enable_TT_SET5;
extern bit_flag  IO_Control_State_TT_SET6;
extern bit_flag  IO_Control_Enable_TT_SET6;
extern bit_flag  IO_Control_State_Bar_Graph_Status;
extern bit_flag  IO_Control_Enable_Bar_Graph;
extern bit_flag  IO_Control_State_Gear_IND_Status_Byte0;
extern bit_flag  IO_Control_State_Gear_IND_Status_Byte1;
extern bit_flag  IO_Control_Enable_Gear_IND;
extern uint16_t  IO_Control_State_Speedometer_Status;
extern uint16_t  IO_Control_State_Tachometer_Status;
extern uint8_t   IO_Control_State_Tachometer_Redline_Status;
extern bit_flag  IO_Control_Enable_Guages;
extern uint8_t   IO_Control_State_Dial_Illumination_Status;
extern uint8_t   IO_Control_State_LCD_Illumination_Status;
extern uint8_t   IO_Control_State_Pointer_Illumination_Status;
extern bit_flag  IO_Control_Enable_Illumination;
extern uint8_t   IO_Control_State_Display_Status;
extern bit_flag  IO_Control_Enable_Display;

extern uint8_t IO_CONTROL_SET1_TT;
extern uint8_t IO_CONTROL_SET2_TT;
extern uint8_t IO_CONTROL_SET3_TT;
extern uint8_t IO_CONTROL_SET4_TT;
extern uint8_t IO_CONTROL_SET5_TT;
extern uint8_t IO_CONTROL_SET6_TT;
extern uint8_t IO_Control_Illumination_set;

extern bit_flag  IO_Control_State_Chime_Activation[10];
extern bit_flag  IO_Control_Enable_Chime_Activation[10];
extern bit_flag  IO_Control_State_Message_Activation[15];
extern bit_flag  IO_Control_Enable_Message_Activation[15];
//

//Variant Code Variables
extern uint8_t U8g_Block_All_ECU_CAN_MSG;

extern uint8_t U8g_Block_EMS_ECU_CAN_MSG;
extern uint8_t U8g_Block_ABS_ECU_CAN_MSG;
extern uint8_t U8g_Block_ESC_ECU_CAN_MSG;
extern uint8_t U8g_Block_SAS_ECU_CAN_MSG;
extern uint8_t U8g_Block_PKE_ECU_CAN_MSG;
extern uint8_t U8g_Block_ICU_ECU_CAN_MSG;

extern uint8_t U8g_Block_SRS_ECU_CAN_MSG;
extern uint8_t U8g_Block_TCU_ECU_CAN_MSG;
extern uint8_t U8g_Block_IC_ECU_CAN_MSG;
extern uint8_t U8g_Block_BCM_ECU_CAN_MSG;
extern uint8_t U8g_Block_IS_ECU_CAN_MSG;
extern uint8_t U8g_Block_RPAS_ECU_CAN_MSG;
#if 0
#define VAR_CODING_ENGINE_MANAGEMENT_SYSTEM			1
#define FEATURE_CODE_BATTERY_TT_TYPE				0
#define FEATURE_CODE_DPF_TT_CONFIG					0
#define FEATURE_CODE_ECO_TT_CONFIG 					1
#define VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM	1
#define FEATURE_CODE_CODRIVER_SBR_TT_CONFIG			1

//#define FEATURE_CODE_FUEL_TYPE 					1

#define FEATURE_CODE_GLOWPLUG_TT_CONFIG 			1
#define FEATURE_CODE_CRUISE_TT_CONFIG 				1
#define FEATURE_CODE_DEF_TT_CONFIG					1
#define FEATURE_CODE_REGEN							1
#define FEATURE_CODE_HIGHBEAM_TT_TYPE				0
#define VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE	1
#define FEATURE_CODE_PARK_TT_TYPE					0
#define FEATURE_CODE_FRONT_FOG_TT_TYPE				0
#define FEATURE_CODE_REAR_FOG_TT_TYPE				0
#define FEATURE_CODE_4WD_LOGIC						0
#define VAR_CODING_ELECTRONICS_STABILITY_CONTROL  	0
#define VAR_CODING_ANTILOCK_BRAKING_SYSTEM			1
#define FEATURE_CODE_OVERSPEED_TT_CONFIG 			1
#define FEATURE_CODE_SPEED_INPUT					0
#define VAR_CODING_TRANSMISSION_CONTROL_UNIT		1
#define FEATURE_CODE_START_STOP_TT_CONFIG			1
#define FEATURE_CODE_TPMS_TT_CONFIG 				1
#define VAR_CODING_ICUA3							1
#endif
/********************Feature code 1st byte data flag*****************************/
#define FEATURE_CODE 								feature_code_1stbyte.bits.UDS_Msg_0
#define FEATURE_CODE_FUEL_TYPE 						feature_code_1stbyte.bits.UDS_Msg_1
//#define FEATURE_CODE_FUEL_TYPE 					1
#define FEATURE_CODE_START_STOP_TT_CONFIG 			feature_code_1stbyte.bits.UDS_Msg_2
#define FEATURE_CODE_CRUISE_TT_CONFIG 				feature_code_1stbyte.bits.UDS_Msg_3
#define FEATURE_CODE_GLOWPLUG_TT_CONFIG 			feature_code_1stbyte.bits.UDS_Msg_4
#define FEATURE_CODE_BSG_TT_CONFIG 					feature_code_1stbyte.bits.UDS_Msg_5
#define FEATURE_CODE_ECO_TT_CONFIG 					feature_code_1stbyte.bits.UDS_Msg_6
#define FEATURE_CODE_RHD_LHD_CONFIG 				feature_code_1stbyte.bits.UDS_Msg_7

/****************** Feature code 2nd byte data flag*****************************/
#define FEATURE_CODE_OVERSPEED_TT_CONFIG 			feature_code_2ndbyte.bits.UDS_Msg_0
#define FEATURE_CODE_BATTERY_TT_TYPE 				feature_code_2ndbyte.bits.UDS_Msg_1
#define FEATURE_CODE_IS_ILLUMINATION_CONFIG 		feature_code_2ndbyte.bits.UDS_Msg_2
#define FEATURE_CODE_CLOCK_CONFIG 					feature_code_2ndbyte.bits.UDS_Msg_3
#define FEATURE_CODE_AFE_IND 						feature_code_2ndbyte.bits.UDS_Msg_4
#define FEATURE_CODE_DTE_IND 						feature_code_2ndbyte.bits.UDS_Msg_5
#define FEATURE_CODE_IFE_IND 						feature_code_2ndbyte.bits.UDS_Msg_6
#define FEATURE_CODE_PODS 							feature_code_2ndbyte.bits.UDS_Msg_7
/****************** Feature code 3rd byte data flag*****************************/
#define FEATURE_CODE_TPMS_TT_CONFIG					feature_code_3rdbyte.bits.UDS_Msg_0
#define FEATURE_CODE_GSI_CONFIG						feature_code_3rdbyte.bits.UDS_Msg_1
#define FEATURE_CODE_DEF_TT_CONFIG					feature_code_3rdbyte.bits.UDS_Msg_2
#define FEATURE_CODE_DPF_TT_CONFIG					feature_code_3rdbyte.bits.UDS_Msg_3
#define FEATURE_CODE_RPAS_IND						feature_code_3rdbyte.bits.UDS_Msg_4
#define FEATURE_CODE_FPAS_IND						feature_code_3rdbyte.bits.UDS_Msg_5
#define FEATURE_CODE_CODRIVER_SBR_TT_CONFIG			feature_code_3rdbyte.bits.UDS_Msg_6
#define FEATURE_CODE_TRANSFER_MODE_CONFIG			feature_code_3rdbyte.bits.UDS_Msg_7
/****************** Feature code 4th byte data flag*****************************/
#define FEATURE_CODE_AVERAGE_SPEED_CONFIG			feature_code_4thbyte.bits.UDS_Msg_0
#define FEATURE_CODE_DRIVING_TIME_CONFIG			feature_code_4thbyte.bits.UDS_Msg_1
#define FEATURE_CODE_STEER_MODE_CONFIG				feature_code_4thbyte.bits.UDS_Msg_2
#define FEATURE_CODE_DIGITAL_SPEED_CONFIG			feature_code_4thbyte.bits.UDS_Msg_3
#define FEATURE_CODE_OAT_CONFIG						feature_code_4thbyte.bits.UDS_Msg_4
#define FEATURE_CODE_AUTO_START_STOP_TIME			feature_code_4thbyte.bits.UDS_Msg_5
#define FEATURE_CODE_TDMS_CONFIG					feature_code_4thbyte.bits.UDS_Msg_6
#define FEATURE_CODE_HIGHBEAM_TT_TYPE				feature_code_4thbyte.bits.UDS_Msg_7
/****************** Feature code 5th byte data flag*****************************/
#define FEATURE_CODE_FRONT_FOG_TT_TYPE				feature_code_5thbyte.bits.UDS_Msg_0
#define FEATURE_CODE_SERVICE_REMINDER_CONFIG		feature_code_5thbyte.bits.UDS_Msg_1
#define FEATURE_CODE_KEY_IN_REMINDER_CONFIG			feature_code_5thbyte.bits.UDS_Msg_2
#define FEATURE_CODE_PARK_LAMP_ON_CONFIG 			feature_code_5thbyte.bits.UDS_Msg_3

//#define FEATURE_CODE_OAT_CONFIG 					feature_code_5thbyte.bits.UDS_Msg_4
//#define FEATURE_CODE_AUTO_START_STOP_TIME			feature_code_5thbyte.bits.UDS_Msg_5
//#define FEATURE_CODE_TDMS_CONFIG					feature_code_5thbyte.bits.UDS_Msg_6
//#define FEATURE_CODE_HIGH BEAM_TT_TYPE			feature_code_5thbyte.bits.UDS_Msg_7
/****************** Feature code 6th byte data flag*****************************/
#define FEATURE_CODE_4WD_LOGIC						feature_code_6thbyte.bits.UDS_Msg_1
#define FEATURE_CODE_REAR_FOG_TT_TYPE				feature_code_6thbyte.bits.UDS_Msg_2
#define FEATURE_CODE_REGEN 							feature_code_6thbyte.bits.UDS_Msg_3
#define FEATURE_CODE_SPEED_INPUT     				feature_code_6thbyte.bits.UDS_Msg_4
#define FEATURE_CODE_AC_TRIP_METER   				feature_code_6thbyte.bits.UDS_Msg_5
#define FEATURE_CODE_SEAT_BELT_TT_TYPE 				feature_code_6thbyte.bits.UDS_Msg_6
#define FEATURE_CODE_PARK_TT_TYPE					feature_code_6thbyte.bits.UDS_Msg_7


/****************** Feature code 7th byte data flag*****************************/
#define FEATURE_CODE_ECO_SCORE 							feature_code_7thbyte.bits.UDS_Msg_3
#define FEATURE_CODE_TRIP_AC_RESET						feature_code_7thbyte.bits.UDS_Msg_4
#define FEATURE_CODE_CURRENT_TARGET_SEL   				feature_code_7thbyte.bits.UDS_Msg_5
#define FEATURE_CODE_FUNC_SAFETY		 				feature_code_7thbyte.bits.UDS_Msg_6
#define FEATURE_CODE_TEST_MSG_CONFIG					feature_code_7thbyte.bits.UDS_Msg_7

/****************** Feature code 8th byte data flag*****************************/
#define FEATURE_CODE_AFE_RESET_TYPES 					feature_code_8thbyte.bits.UDS_Msg_0
#define FEATURE_CODE_POWER_SAVER						feature_code_8thbyte.bits.UDS_Msg_1
#define FEATURE_CODE_PAB_SETTING   						feature_code_8thbyte.bits.UDS_Msg_3

/****************** Feature code 11th byte data flag*****************************/
#define FEATURE_CODE_CRUISE_SET_SPEED 					feature_code_11thbyte.bits.UDS_Msg_1
#define FEATURE_CODE_MEDIA_DISPLAY 						feature_code_11thbyte.bits.UDS_Msg_2
#define FEATURE_CODE_CAR_COLOR_SELECT 					feature_code_11thbyte.bits.UDS_Msg_3
#define FEATURE_CODE_WELCOME 							feature_code_11thbyte.bits.UDS_Msg_5
//#ifdef SPRINT4
/****************** Feature code 13th byte data flag*****************************/
#define FEATURE_CODE_TDMS_DISPLAY 					    feature_code_13thbyte.bits.UDS_Msg_0
//#endif
/****************** Feature code 16th byte data flag*****************************/
#define FEATURE_CODE_FUEL_TANK_SELECT					feature_code_16thbyte.bits.UDS_Msg_2
#define FEATURE_CODE_DISPLAY_4WD						feature_code_16thbyte.bits.UDS_Msg_4
/****************** Feature code 17th byte data flag*****************************/
#define FEATURE_CODE_AT_GEAR_ALERT						feature_code_17thbyte.bits.UDS_Msg_5
/*********************** Variant code 1st byte data flag***************************/
#define VAR_CODING_STATUS_OF_VARIANT_CODE				Varient_code_1st_byte.bits.UDS_Msg_0
#define VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM 		Varient_code_1st_byte.bits.UDS_Msg_1
#define VAR_CODING_ANTILOCK_BRAKING_SYSTEM 				Varient_code_1st_byte.bits.UDS_Msg_2
#define VAR_CODING_ELECTRONICS_STABILITY_CONTROL  		Varient_code_1st_byte.bits.UDS_Msg_3
#define VAR_CODING_PASSIVE_KEY_ENTRY_SMART_KEY_MODULE 	Varient_code_1st_byte.bits.UDS_Msg_6
#define VAR_CODING_IMMOBILIZER_CONTROL_UNIT				Varient_code_1st_byte.bits.UDS_Msg_7

/*********************** IVN NS Variant code 1st byte data flag***************************/
#define IVN_NS_VAR_CODING_STATUS_OF_VARIANT_CODE				NS_Varient_code_1st_byte.bits.UDS_Msg_0
#define IVN_NS_VAR_CODING_ENGINE_MANAGEMENT_SYSTEM 				NS_Varient_code_1st_byte.bits.UDS_Msg_1
#define IVN_NS_VAR_CODING_ELECTRONICS_STABILITY_CONTROL  		(NS_Varient_code_1st_byte.bits.UDS_Msg_3 | NS_Varient_code_1st_byte.bits.UDS_Msg_2)
#define IVN_NS_VAR_CODING_INFOTAINMENT_SYSTEM					NS_Varient_code_5th_byte.bits.UDS_Msg_5
#define IVN_NS_VAR_CODING_PASSIVE_KEY_ENTRY_SMART_KEY_MODULE	NS_Varient_code_1st_byte.bits.UDS_Msg_1
#define IVN_NS_VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM		NS_Varient_code_2nd_byte.bits.UDS_Msg_0
#define IVN_NS_VAR_CODING_TRANSMISSION_CONTROL_UNIT				NS_Varient_code_2nd_byte.bits.UDS_Msg_4

/*********************** Variant code 2nd byte data flag***************************/
#define VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM 		Varient_code_2nd_byte.bits.UDS_Msg_0
#define VAR_CODING_TRANSMISSION_CONTROL_UNIT 			Varient_code_2nd_byte.bits.UDS_Msg_4
/*********************** Variant code 4TH byte data flag***************************/
#define VARIANT_CODING_CLUSTER 							Varient_code_4th_byte.bits.UDS_Msg_0
#define VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE		Varient_code_4th_byte.bits.UDS_Msg_1
/*********************** Variant code 5TH byte data flag***************************/
#define VARIANT_CODING_INFOTAINMENT_SYSTEM			Varient_code_5th_byte.bits.UDS_Msg_5

/**************************** ECU identifier 0x050B ************************************************/
#define BAR_ONE_ON_VALUE_FUEL_GUAGE                 Fuel_Bar_Status_1st_byte.byte
#define BAR_TWO_ON_VALUE_FUEL_GUAGE  				Fuel_Bar_Status_2nd_byte.byte
#define BAR_THREE_ON_VALUE_FUEL_GUAGE				Fuel_Bar_Status_3rd_byte.byte
#define BAR_FOUR_ON_VALUE_FUEL_GUAGE				Fuel_Bar_Status_4th_byte.byte
#define BAR_FIFTH_ON_VALUE_FUEL_GUAGE				Fuel_Bar_Status_5th_byte.byte
#define BAR_SIX_ON_VALUE_FUEL_GUAGE					Fuel_Bar_Status_6th_byte.byte
#define BAR_SEVEN_ON_VALUE_FUEL_GUAGE				Fuel_Bar_Status_7th_byte.byte
#define BAR_EIGHT_ON_VALUE_FUEL_GUAGE				Fuel_Bar_Status_8th_byte.byte

#define BAR_ONE_OFF_VALUE_FUEL_GUAGE                Fuel_Bar_Status_9th_byte.byte
#define BAR_TWO_OFF_VALUE_FUEL_GUAGE  				Fuel_Bar_Status_10th_byte.byte
#define BAR_THREE_OFF_VALUE_FUEL_GUAGE				Fuel_Bar_Status_11th_byte.byte
#define BAR_FOUR_OFF_VALUE_FUEL_GUAGE				Fuel_Bar_Status_12th_byte.byte
#define BAR_FIFTH_OFF_VALUE_FUEL_GUAGE   			Fuel_Bar_Status_13th_byte.byte
#define BAR_SIX_OFF_VALUE_FUEL_GUAGE  				Fuel_Bar_Status_14th_byte.byte
#define BAR_SEVEN_OFF_VALUE_FUEL_GUAGE  			Fuel_Bar_Status_15th_byte.byte
#define BAR_EIGHT_OFF_VALUE_FUEL_GUAGE  			Fuel_Bar_Status_16th_byte.byte
/****************************** ECU IDENTIFIER 0x050C***********************************************/
#define BAR_ONE_ON_VALUE_TEMP_GUAGE                 Temp_Bar_Status_1st_byte.byte
#define BAR_TWO_ON_VALUE_TEMP_GUAGE  				Temp_Bar_Status_2nd_byte.byte
#define BAR_THREE_ON_VALUE_TEMP_GUAGE 				Temp_Bar_Status_3rd_byte.byte
#define BAR_FOUR_ON_VALUE_TEMP_GUAGE 				Temp_Bar_Status_4th_byte.byte
#define BAR_FIFTH_ON_VALUE_TEMP_GUAGE 				Temp_Bar_Status_5th_byte.byte
#define BAR_SIX_ON_VALUE_TEMP_GUAGE 				Temp_Bar_Status_6th_byte.byte
#define BAR_SEVEN_ON_VALUE_TEMP_GUAGE 				Temp_Bar_Status_7th_byte.byte
#define BAR_EIGHT_ON_VALUE_TEMP_GUAGE 				Temp_Bar_Status_8th_byte.byte

#define BAR_ONE_OFF_VALUE_TEMP_GUAGE                Temp_Bar_Status_9th_byte.byte
#define BAR_TWO_OFF_VALUE_TEMP_GUAGE  				Temp_Bar_Status_10th_byte.byte
#define BAR_THREE_OFF_VALUE_TEMP_GUAGE 				Temp_Bar_Status_11th_byte.byte
#define BAR_FOUR_OFF_VALUE_TEMP_GUAGE 				Temp_Bar_Status_12th_byte.byte
#define BAR_FIFTH_OFF_VALUE_TEMP_GUAGE 				Temp_Bar_Status_13th_byte.byte
#define BAR_SIX_OFF_VALUE_TEMP_GUAGE 				Temp_Bar_Status_14th_byte.byte
#define BAR_SEVEN_OFF_VALUE_TEMP_GUAGE 				Temp_Bar_Status_15th_byte.byte
#define BAR_EIGHT_OFF_VALUE_TEMP_GUAGE 				Temp_Bar_Status_16th_byte.byte
/*************************** ECU IDENTOFIER 0x050D ***************************************************/
#define CONTINUE_OFF_VALUE_LFW_TT                   Fuel_Tell_Tale_value_1st_byte.byte
#define CONTINUE_ON_VALUE_LFW_TT                    Fuel_Tell_Tale_value_2nd_byte.byte
#define BLINK_OFF_VALUE_LFW_TT                      Fuel_Tell_Tale_value_3rd_byte.byte
#define BLINK_ON_VALUE_LFW_TT                       Fuel_Tell_Tale_value_4th_byte.byte
/*************************** ECU IDENTOFIER 0x050E ************************************************/
#define CONTINUE_OFF_VALUE_TEMP_TT				    Temp_Tell_Tale_value_1st_byte.byte
#define CONTINUE_ON_VALUE_TEMP_TT                   Temp_Tell_Tale_value_2nd_byte.byte
#define BLINK_OFF_VALUE_TEMP_TT                     Temp_Tell_Tale_value_3rd_byte.byte
#define BLINK_ON_VALUE_TEMP_TT                      Temp_Tell_Tale_value_4th_byte.byte

//DID
//ETM
#define Diag_RW_ETM_enable                				Message_500_0th_byte.byte
//Chime
#define Diag_RW_SeatBelt_Chime_Enable               	Chime_503_0th_byte.bits.UDS_Msg_0
#define Diag_RW_HighTemp_Chime_Enable					Chime_503_0th_byte.bits.UDS_Msg_1
#define Diag_RW_LowOilPressure_Chime_Enable				Chime_503_0th_byte.bits.UDS_Msg_2
#define Diag_RW_LowBrakeFluid_Chime_Enable				Chime_503_0th_byte.bits.UDS_Msg_3
#define Diag_RW_ParkBrake_Chime_Enable					Chime_503_0th_byte.bits.UDS_Msg_4
#define Diag_RW_DoorOpen_Chime_Enable					Chime_503_0th_byte.bits.UDS_Msg_5
#define Diag_RW_OverSpeed_Chime_Enable					Chime_503_0th_byte.bits.UDS_Msg_6
#define Diag_RW_LowFuel_Chime_Enable                	Chime_503_0th_byte.bits.UDS_Msg_7

#define Diag_RW_Park_Lamp_reminder_Chime_Enable     	Chime_503_1st_byte.bits.UDS_Msg_0
#define Diag_RW_TurnSignal_LR_IGN_OFF_Chime_Enable		Chime_503_1st_byte.bits.UDS_Msg_1
#define Diag_RW_TurnSignal_LR_IGN_ON_Chime_Enable		Chime_503_1st_byte.bits.UDS_Msg_2
#define Diag_RW_Message_Notification_Chime_Enable		Chime_503_1st_byte.bits.UDS_Msg_3
#define Diag_RW_Key_in_reminder_Chime_Enable			Chime_503_1st_byte.bits.UDS_Msg_4
#define Diag_RW_WIF_Chime_Enable						Chime_503_1st_byte.bits.UDS_Msg_5
#define Diag_RW_Sunroof_open_Chime_Enable				Chime_503_1st_byte.bits.UDS_Msg_6
#define Diag_RW_Reverse_Chime_Enable					Chime_503_1st_byte.bits.UDS_Msg_7

#define Diag_RW_Welcome_Chime_Enable					Chime_503_2nd_byte.bits.UDS_Msg_0
#define Diag_RW_Goodbye_Chime_Enable					Chime_503_2nd_byte.bits.UDS_Msg_1
#define Diag_RW_Pre_Welcome_Chime_Enable				Chime_503_2nd_byte.bits.UDS_Msg_2
#define Diag_RW_FPASS_Chime_Enable						Chime_503_2nd_byte.bits.UDS_Msg_3
#define Diag_RW_RPASS_Chime_Enable						Chime_503_2nd_byte.bits.UDS_Msg_4
#define Diag_RW_AMT_ParkBrake_Chime_Enable				Chime_503_2nd_byte.bits.UDS_Msg_5
#define Diag_RW_AT_fail_Chime_Enable					Chime_503_2nd_byte.bits.UDS_Msg_6
#define Diag_RW_Rear_SeatBelt_Chime_Enable				Chime_503_2nd_byte.bits.UDS_Msg_7

#define Diag_RW_RPASS_Fail_Chime_Enable					Chime_503_3rd_byte.bits.UDS_Msg_0
#define Diag_RW_AMT_Chime_Enable						Chime_503_3rd_byte.bits.UDS_Msg_1
#define Diag_RW_PKE_Key_Out_Chime_Enable				Chime_503_3rd_byte.bits.UDS_Msg_2
#define Diag_RW_PKE_FOB_Key_Chime_Enable				Chime_503_3rd_byte.bits.UDS_Msg_3
#define Diag_RW_RPASS_Self_check_Chime_Enable			Chime_503_3rd_byte.bits.UDS_Msg_4
#define Diag_RW_Roll_Pitch Chime_Enable			        Chime_503_3rd_byte.bits.UDS_Msg_7 /* newly added*/
//remaining 3 to 9 bytes reserved

//Message 0540
#define Diag_RW_SeatBelt_Warn_Enable					Message_504_0th_byte.bits.UDS_Msg_0
#define Diag_RW_EngineTempHigh_Warn_Enable				Message_504_0th_byte.bits.UDS_Msg_1
#define Diag_RW_OilPressure_Warn_Enable					Message_504_0th_byte.bits.UDS_Msg_2
#define Diag_RW_LowBrakeFluid_Warn_Enable				Message_504_0th_byte.bits.UDS_Msg_3
#define Diag_RW_ParkBrake_Warn_Enable					Message_504_0th_byte.bits.UDS_Msg_4
#define Diag_RW_DoorOpen_Warn_Enable					Message_504_0th_byte.bits.UDS_Msg_5
#define Diag_RW_RPAS_Error_Warn_Enable					Message_504_0th_byte.bits.UDS_Msg_6	/*Diag_RW_BonnetOpen_Warn Enable commented as per Badam Bala's mail dated 26-07-19*/
#define Diag_RW_LowFuel_Warn_Enable						Message_504_0th_byte.bits.UDS_Msg_7

#define Diag_RW_WIF_Warn_Enable							Message_504_1st_byte.bits.UDS_Msg_0
#define Diag_RW_ServiceReminder_Warn_Enable				Message_504_1st_byte.bits.UDS_Msg_1
#define Diag_RW_OverSpeed_Warn_Enable					Message_504_1st_byte.bits.UDS_Msg_2
#define Diag_RW_CheckEngine_Warn_Enable					Message_504_1st_byte.bits.UDS_Msg_3
#define Diag_RW_PABOFF_Warn_Enable						Message_504_1st_byte.bits.UDS_Msg_4
#define Diag_RW_TDMS_Screen_Enable						Message_504_1st_byte.bits.UDS_Msg_5
#define Diag_RW_TurnByTurnNavg_warn_Enable				Message_504_1st_byte.bits.UDS_Msg_6
#define Diag_RW_Check_Turn_Lamp_Warn_Enable				Message_504_1st_byte.bits.UDS_Msg_7

#define Diag_RW_Cruise_Warn_Enable						Message_504_2nd_byte.bits.UDS_Msg_0
#define Diag_RW_ECO_Warn_Enable							Message_504_2nd_byte.bits.UDS_Msg_1
#define Diag_RW_HHC_Warn_Enable							Message_504_2nd_byte.bits.UDS_Msg_2
#define Diag_RW_ESPMalfunction_Warn_Enable				Message_504_2nd_byte.bits.UDS_Msg_3
#define Diag_RW_ABS_Warn_Enable							Message_504_2nd_byte.bits.UDS_Msg_4
#define Diag_RW_EBD_Warn_Enable							Message_504_2nd_byte.bits.UDS_Msg_5
#define Diag_RW_Park_Lamp_Warn_Enable					Message_504_2nd_byte.bits.UDS_Msg_6
#define Diag_RW_Trailer_SW_ON_Warn_Enable				Message_504_2nd_byte.bits.UDS_Msg_7

#define Diag_RW_Sunroof_open_Warn_Enable				Message_504_3rd_byte.bits.UDS_Msg_0
#define Diag_RW_FlexiSteer_Warn_Enable					Message_504_3rd_byte.bits.UDS_Msg_1
#define Diag_RW_StopSetting_Warn_Enable					Message_504_3rd_byte.bits.UDS_Msg_2
#define Diag_RW_Fun_Safety_Warn_Enable					Message_504_3rd_byte.bits.UDS_Msg_3
#define Diag_RW_Park_lamp_failure_Warn_Enable			Message_504_3rd_byte.bits.UDS_Msg_4
#define Diag_RW_Left_turn_lamp_failure_Warn_Enable		Message_504_3rd_byte.bits.UDS_Msg_5
#define Diag_RW_Right_turn_lamp_failure_Warn_Enable		Message_504_3rd_byte.bits.UDS_Msg_6
#define Diag_RW_PAS_Status_Warn_Enable					Message_504_3rd_byte.bits.UDS_Msg_7

#define Diag_RW_Horn_failure_Warn_Enable				Message_504_4th_byte.bits.UDS_Msg_0
#define Diag_RW_AMT_Not_Allowed_Warn_Enable				Message_504_4th_byte.bits.UDS_Msg_1
#define Diag_RW_AMT_Press_Brake_Warn_Enable				Message_504_4th_byte.bits.UDS_Msg_2
#define Diag_RW_AMT_Shift_to_N_Warn_Enable				Message_504_4th_byte.bits.UDS_Msg_3
#define Diag_RW_AMT_Failure_Warn_Enable					Message_504_4th_byte.bits.UDS_Msg_4
#define Diag_RW_AMT_Lockeout_Warn_Enable				Message_504_4th_byte.bits.UDS_Msg_5
#define Diag_RW_AMT_Sport_Mode_Status_warn_Enable		Message_504_4th_byte.bits.UDS_Msg_6
#define Diag_RW_AMT_Pullover_warn_Enable				Message_504_4th_byte.bits.UDS_Msg_7

#define Diag_RW_PKE_Key_Out_warn_Enable					Message_504_5th_byte.bits.UDS_Msg_0
#define Diag_RW_PKE_Fob_Key_reminder_warn_Enable		Message_504_5th_byte.bits.UDS_Msg_1
#define Diag_RW_PKE_Fob_Auth_Fail_warn_Enable			Message_504_5th_byte.bits.UDS_Msg_2
#define Diag_RW_RKE_Low_battery_warn_Enable				Message_504_5th_byte.bits.UDS_Msg_3
#define Diag_RW_ESCL_Jam_warn_Enable					Message_504_5th_byte.bits.UDS_Msg_4
#define Diag_RW_ESCL_Not_Locked_warn_Enable				Message_504_5th_byte.bits.UDS_Msg_5
#define Diag_RW_ESCL_Fail_warn_Enable					Message_504_5th_byte.bits.UDS_Msg_6
#define Diag_RW_ESCL_Not_Learned_warn_Enable			Message_504_5th_byte.bits.UDS_Msg_7

#define Diag_RW_PKE_Error_warn_Enable					Message_504_6th_byte.bits.UDS_Msg_0
#define Diag_RW_PKE_Fob_Batt_Discharge_warn_Enable		Message_504_6th_byte.bits.UDS_Msg_1
#define Diag_RW_AMT_Engage_Park_Brake_warn_Enable		Message_504_6th_byte.bits.UDS_Msg_2
#define Diag_RW_Tail_lamp_warn_Enable					Message_504_6th_byte.bits.UDS_Msg_3
#define Diag_RW_RPAS_supply_warn_Enable					Message_504_6th_byte.bits.UDS_Msg_4
#define Diag_RW_PKE_PN_Off_warn_Enable					Message_504_6th_byte.bits.UDS_Msg_5
#define Diag_RW_PKE_Engine_STSP_SW_Malfion_warn_Enable	Message_504_6th_byte.bits.UDS_Msg_6
#define Diag_RW_PKE_Door_lock_warn_Enable				Message_504_6th_byte.bits.UDS_Msg_7

#define Diag_RW_Incorrect_DEF_warn_Enable				Message_504_7th_byte.bits.UDS_Msg_0
#define Diag_RW_DEF_Dosing_warn_Enable					Message_504_7th_byte.bits.UDS_Msg_1
#define Diag_RW_DEF_Level_warn_Enable					Message_504_7th_byte.bits.UDS_Msg_2
#define Diag_RW_DPF_Clogged_warn_Enable					Message_504_7th_byte.bits.UDS_Msg_3
#define Diag_RW_Regen_Started_warn_Enable				Message_504_7th_byte.bits.UDS_Msg_4
#define Diag_RW_Regen_Not_started_warn_Enable			Message_504_7th_byte.bits.UDS_Msg_5
#define Diag_RW_Regen_Completed_warn_Enable				Message_504_7th_byte.bits.UDS_Msg_6
#define Diag_RW_Regen_Aborted_warn_Enable				Message_504_7th_byte.bits.UDS_Msg_7

#define Diag_RW_Regen_Requested_warn_Enable				Message_504_8th_byte.bits.UDS_Msg_0
#define Diag_RW_ESCL_System_Saftey_failure_warn_Enable  Message_504_8th_byte.bits.UDS_Msg_1
#define Diag_RW_Regen_Required_warn_Enable				Message_504_8th_byte.bits.UDS_Msg_2

#define Diag_RW_TPMS_Low_Tyre_Pressure_Enable  			Message_504_9th_byte.bits.UDS_Msg_6
#define Diag_RW_TPMS_High_Tyre_Pressure_Enable			Message_504_9th_byte.bits.UDS_Msg_7

#define Diag_RW_TPMS_Sys_Fault_Enable 					Message_504_10th_byte.bits.UDS_Msg_0
#define Diag_RW_TPMS_Tyre_Leakage_Enable 				Message_504_10th_byte.bits.UDS_Msg_1
#define Diag_RW_TPMS_High_Temp_Enable 					Message_504_10th_byte.bits.UDS_Msg_2
#define Diag_RW_TPMS_Low_Temp_Enable 					Message_504_10th_byte.bits.UDS_Msg_3
#define Diag_RW_RKE_Key_in_reminder_warn_Enable 		Message_504_10th_byte.bits.UDS_Msg_4

#define Diag_RW_4WD_Change_Animation_Enable 			Message_504_11th_byte.bits.UDS_Msg_6

#define Diag_RW_HDC_Status_Enable 						Message_504_12th_byte.bits.UDS_Msg_2
#define Diag_RW_Roll_Alert_Enable 						Message_504_12th_byte.bits.UDS_Msg_4
#define Diag_RW_Pitch_Alert_Enable 						Message_504_12th_byte.bits.UDS_Msg_5

//remaining 8 to 14 bytes reserved


#endif /* UDSS_H_ */
