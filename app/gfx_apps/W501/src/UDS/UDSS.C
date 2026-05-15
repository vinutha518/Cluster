/*
 * UDSS.C
 *
 *  Created on: Jan 14, 2019
 *      Author: MSWELT12
 */
/*
 * UDSS.c
 *
 *  Created on: Jan 12, 2019
 *      Author: MSWELT12
 */
/***********************************************************************************************************************
File             :       UDSS.c
Author           :       Akshay Wankhade
Model            :       Mahindra W501
Revision Author  :
Reviewed By      :
Company          :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

/***********************************************************************************************************************
Includes
 ********************************************************************************************************************** */
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
#include "Memmory_Map.h"
#include "my_SeedKey.h"
#include "UDS_DTC.h"
#include "r_cg_serial_user.h"
#include "dr7f701461.dvf.h"
#include "digitalGuage.h"
#include "drome.h"
#include "druserconf.h"
#include "TLC6C_S_P.h"
/******************************W501 data *******************************/
// FEATURE CODE
bit_flag  feature_code_1stbyte;
bit_flag  feature_code_2ndbyte;
bit_flag  feature_code_3rdbyte;
bit_flag  feature_code_4thbyte;
bit_flag  feature_code_5thbyte;
bit_flag  feature_code_6thbyte;
bit_flag  feature_code_7thbyte;
bit_flag  feature_code_8thbyte;
bit_flag  feature_code_9thbyte;
bit_flag  feature_code_10thbyte;
bit_flag  feature_code_11thbyte;
bit_flag  feature_code_12thbyte;
bit_flag  feature_code_13thbyte;
bit_flag  feature_code_14thbyte;
bit_flag  feature_code_15thbyte;
bit_flag  feature_code_16thbyte;
bit_flag  feature_code_17thbyte;
bit_flag  feature_code_18thbyte;
bit_flag  feature_code_19thbyte;
bit_flag  feature_code_20thbyte;

//VARIENT CODE
bit_flag  Varient_code_1st_byte;
bit_flag  Varient_code_2nd_byte;
bit_flag  Varient_code_3rd_byte;
bit_flag  Varient_code_4th_byte;
bit_flag  Varient_code_5th_byte;

bit_flag  Varient_code_6th_byte;
bit_flag  Varient_code_7th_byte;
bit_flag  Varient_code_8th_byte;
bit_flag  Varient_code_9th_byte;
bit_flag  Varient_code_10th_byte;
bit_flag  Varient_code_11th_byte;
bit_flag  Varient_code_12th_byte;
bit_flag  Varient_code_13th_byte;
bit_flag  Varient_code_14th_byte;
bit_flag  Varient_code_15th_byte;
bit_flag  Varient_code_16th_byte;
bit_flag  Varient_code_17th_byte;
bit_flag  Varient_code_18th_byte;
bit_flag  Varient_code_19th_byte;
bit_flag  Varient_code_20th_byte;

//IVN VARIENT CODE
bit_flag  NS_Varient_code_1st_byte;
bit_flag  NS_Varient_code_2nd_byte;
bit_flag  NS_Varient_code_3rd_byte;
bit_flag  NS_Varient_code_4th_byte;
bit_flag  NS_Varient_code_5th_byte;
bit_flag  NS_Varient_code_6th_byte;
bit_flag  NS_Varient_code_7th_byte;
bit_flag  NS_Varient_code_8th_byte;
bit_flag  NS_Varient_code_9th_byte;
bit_flag  NS_Varient_code_10th_byte;
bit_flag  NS_Varient_code_11th_byte;
bit_flag  NS_Varient_code_12th_byte;
bit_flag  NS_Varient_code_13th_byte;
bit_flag  NS_Varient_code_14th_byte;
bit_flag  NS_Varient_code_15th_byte;
bit_flag  NS_Varient_code_16th_byte;
bit_flag  NS_Varient_code_17th_byte;
bit_flag  NS_Varient_code_18th_byte;
bit_flag  NS_Varient_code_19th_byte;
bit_flag  NS_Varient_code_20th_byte;

//FULE BAR ON OFF VALUES
bit_flag  Fuel_Bar_Status_1st_byte;
bit_flag  Fuel_Bar_Status_2nd_byte;
bit_flag  Fuel_Bar_Status_3rd_byte;
bit_flag  Fuel_Bar_Status_4th_byte;
bit_flag  Fuel_Bar_Status_5th_byte;
bit_flag  Fuel_Bar_Status_6th_byte;
bit_flag  Fuel_Bar_Status_7th_byte;
bit_flag  Fuel_Bar_Status_8th_byte;

bit_flag  Fuel_Bar_Status_9th_byte;
bit_flag  Fuel_Bar_Status_10th_byte;
bit_flag  Fuel_Bar_Status_11th_byte;
bit_flag  Fuel_Bar_Status_12th_byte;
bit_flag  Fuel_Bar_Status_13th_byte;
bit_flag  Fuel_Bar_Status_14th_byte;
bit_flag  Fuel_Bar_Status_15th_byte;
bit_flag  Fuel_Bar_Status_16th_byte;

//TEMPRATURE BAR ON OFF VALUES
bit_flag  Temp_Bar_Status_1st_byte;
bit_flag  Temp_Bar_Status_2nd_byte;
bit_flag  Temp_Bar_Status_3rd_byte;
bit_flag  Temp_Bar_Status_4th_byte;
bit_flag  Temp_Bar_Status_5th_byte;
bit_flag  Temp_Bar_Status_6th_byte;
bit_flag  Temp_Bar_Status_7th_byte;
bit_flag  Temp_Bar_Status_8th_byte;
bit_flag  Temp_Bar_Status_9th_byte;
bit_flag  Temp_Bar_Status_10th_byte;
bit_flag  Temp_Bar_Status_11th_byte;
bit_flag  Temp_Bar_Status_12th_byte;
bit_flag  Temp_Bar_Status_13th_byte;
bit_flag  Temp_Bar_Status_14th_byte;
bit_flag  Temp_Bar_Status_15th_byte;
bit_flag  Temp_Bar_Status_16th_byte;

//FUEL TELLTALE BLINKING CONDITIONS
bit_flag  Fuel_Tell_Tale_value_1st_byte;
bit_flag  Fuel_Tell_Tale_value_2nd_byte;
bit_flag  Fuel_Tell_Tale_value_3rd_byte;
bit_flag  Fuel_Tell_Tale_value_4th_byte;

//TEMPRATURE TELLTALE BLINKING CONDITIONS
bit_flag  Temp_Tell_Tale_value_1st_byte;
bit_flag  Temp_Tell_Tale_value_2nd_byte;
bit_flag  Temp_Tell_Tale_value_3rd_byte;
bit_flag  Temp_Tell_Tale_value_4th_byte;

//DID
//Chime Configuration 503//
bit_flag  Chime_503_0th_byte;
bit_flag  Chime_503_1st_byte;
bit_flag  Chime_503_2nd_byte;
bit_flag  Chime_503_3rd_byte;
bit_flag  Chime_503_4th_byte;
bit_flag  Chime_503_5th_byte;
bit_flag  Chime_503_6th_byte;
bit_flag  Chime_503_7th_byte;
bit_flag  Chime_503_8th_byte;
bit_flag  Chime_503_9th_byte;

//Message Configuration 503//
bit_flag  Message_504_0th_byte;
bit_flag  Message_504_1st_byte;
bit_flag  Message_504_2nd_byte;
bit_flag  Message_504_3rd_byte;
bit_flag  Message_504_4th_byte;
bit_flag  Message_504_5th_byte;
bit_flag  Message_504_6th_byte;
bit_flag  Message_504_7th_byte;
bit_flag  Message_504_8th_byte;
bit_flag  Message_504_9th_byte;
bit_flag  Message_504_10th_byte;
bit_flag  Message_504_11th_byte;
bit_flag  Message_504_12th_byte;
bit_flag  Message_504_13th_byte;
bit_flag  Message_504_14th_byte;
bit_flag  Message_504_15th_byte;
//
//ETM	500
bit_flag Message_500_0th_byte;
//

uint8_t   IO_Parameter_Request_Status;
bit_flag  IO_Control_State_TT_SET1;
bit_flag  IO_Control_Enable_TT_SET1;
bit_flag  IO_Control_State_TT_SET2;
bit_flag  IO_Control_Enable_TT_SET2;
bit_flag  IO_Control_State_TT_SET3;
bit_flag  IO_Control_Enable_TT_SET3;
bit_flag  IO_Control_State_TT_SET4;
bit_flag  IO_Control_Enable_TT_SET4;
bit_flag  IO_Control_State_TT_SET5;
bit_flag  IO_Control_Enable_TT_SET5;
bit_flag  IO_Control_State_TT_SET6;
bit_flag  IO_Control_Enable_TT_SET6;
bit_flag  IO_Control_State_Bar_Graph_Status;
bit_flag  IO_Control_Enable_Bar_Graph;
bit_flag  IO_Control_State_Gear_IND_Status_Byte0;
bit_flag  IO_Control_State_Gear_IND_Status_Byte1;
bit_flag  IO_Control_Enable_Gear_IND;
uint16_t  IO_Control_State_Speedometer_Status;
uint16_t  IO_Control_State_Tachometer_Status;
uint8_t   IO_Control_State_Tachometer_Redline_Status;
bit_flag  IO_Control_Enable_Guages;
uint8_t   IO_Control_State_Dial_Illumination_Status;
uint8_t   IO_Control_State_LCD_Illumination_Status;
uint8_t   IO_Control_State_Pointer_Illumination_Status;
bit_flag  IO_Control_Enable_Illumination;
uint8_t   IO_Control_State_Display_Status;
bit_flag  IO_Control_Enable_Display;

/* Chime and Message Activation */
bit_flag  IO_Control_State_Chime_Activation[10];
bit_flag  IO_Control_Enable_Chime_Activation[10];
bit_flag  IO_Control_State_Message_Activation[15];
bit_flag  IO_Control_Enable_Message_Activation[15];

uint8_t IO_CONTROL_SET1_TT;
uint8_t IO_CONTROL_SET2_TT;
uint8_t IO_CONTROL_SET3_TT;
uint8_t IO_CONTROL_SET4_TT;
uint8_t IO_CONTROL_SET5_TT;
uint8_t IO_CONTROL_SET6_TT;
uint8_t IO_Control_Illumination_set;
uint8_t IO_TFT_TEST;
extern uint8_t pre_temp_val;
void Bit_flag_Access(void);
void IO_Control(void);
void IO_Control_MultiFrame(void);
extern unsigned char TX_DATA[];
extern uint16_t reset_counter;

/*********************************************************************/

#define MASK_LOWER_NIBBLE   0x0F
#define MASK_UPPER_NIBBLE   0xF0
#define CHAR_BIT            8
#define protected_write(preg,pstatus,reg,value) do {                    \
		(preg)=0xa5u;           \
		(reg)=(value);          \
		(reg)=~(value);         \
		(reg)=(value);          \
}while((pstatus)==1u)

/* *********Variables Definition*************************************************************************/
uint8_t Bytes[4] = { 0, 0, 0, 0 };
#if 0
const uint32_t Data_Matrix[16] = {      0x428A2F98,
		0x71374491,
		0xB5C0FBCF,
		0xE9B5DBA5,
		0x3956C25B,
		0x59F111F1,
		0x923F82A4,
		0xAB1C5ED5,
		0xD807AA98,
		0x12835B01,
		0x243185BE,
		0x550C7DC3,
		0x72BE5D74,
		0x80DEB1FE,
		0x9BDC06A7,
		0xC19BF174
};
#endif
#if 0
const uint32_t Data_Matrix[16] = {      0xF549506A,
		0x47152AB0,
		0x2E31DA76,
		0x53266FE2,
		0x874318C0,
		0x2FE296BF,
		0xCB84F0FE,
		0xE806EC53,
		0x018F28ED,
		0x487E89DE,
		0xCDC1B660,
		0xFC0829D3,
		0x847FCE7C,
		0x17E8DD4D,
		0xC666FF1C,
		0xBDC5DBE9
};
#endif

uint32_t mask = 0;

uint64_t keyvalidation_level3;
uint32_t keyvalidation;
seed_value Seed;
seed_value_level3 Seed_level3;
uint8_t seed_received;
seed_value_level3 received_key3;
seed_value received_key;
unsigned long long key_val;
uint8_t write_seed;
uint8_t no_of_attempts = 0;
//uint8_t PROGRAMMING_FLAG;
uint8_t Wrong_seed_Attempts;
_Bool Security_Access_Success;
_Bool Security_Access_Level3_Success;
_Bool access_tmparameter;
//_Bool REQUEST_3SEC;
//_Bool REQUEST_5SEC;
uint64_t key_64;
uint32_t Address	= 0x00004000;
uint32_t SID36_Address	= CLEAR;
uint8_t Rx_multiplebytes[1500];
uint8_t wdt_reset_cntr;
uint16_t Rx_multiplecount;
uint8_t StoredService_ID;
uint8_t expectedblock_seq   = CLEAR;
uint16_t No_Expected_Frames;
uint16_t No_Received_Frames = CLEAR;
uint16_t No_Expected_Bytes  = CLEAR;
//uint16_t P2CAN_ECU_TIMEOUT  = 500;  /* 50ms/100us */
extern uint64_t S3ECU_cntr;
uint32_t Download_Addr;
extern uint64_t NBs_cntr;
extern const uint16_t S3ECU_TIMEOUT;

extern uint32_t final_tachometer_speed;
extern uint16_t final_vechile_speed;

unsigned char Read_Boot_Vrs[8];
unsigned char Read_HW_Vrs[8];
extern unsigned char App_Vesion[8];
extern unsigned char App_Vesion_U350[8];
extern unsigned char HWC_Version[4];
extern unsigned char HWG_Version[4];
extern unsigned char HWU_Version[4];

//extern RX_DATA INDC_DEF_Level;
//extern unsigned char UDS_Buffer[250]={0};
/* *******Variable Declaration************************************************************************ */
extern Frame FrameType;
_Bool ext_session_active;
_Bool prog_session_active = 0;//r
_Bool stop_communication;
_Bool DTC_stop,flow_Control_State_rec;
_Bool Functional_Addressing_Flag = CLEAR;
_Bool UDS_IO_Control_State_0xF093_Flag;
_Bool UDS_IO_Control_State_0xF092_Flag;
uint8_t PROGRAMMING_FLAG;
/* *******Function Declaration************************************************************************ */
unsigned char seed_received1;
unsigned char seed_multiplebytes[8];
unsigned char  seed_multiplecount;
void Process_UDSData(void);
PRIVATE void udss_SessionControl(void);
PRIVATE void tester_present_service(void);
//void udss_Readdatabyidentifier();
//void WriteDataByIdentifier(uint8_t DID);
//void WriteDataByIdentifier_multipleframes();
void Respond_Single_Frame(uint8_t);
void Respond_Single_Frame(uint8_t);
PRIVATE void security_Access(void);
void sendFlowControl(void);
void Save_NegResponse(uint8_t,uint8_t);
void Clear_UDS_variables(void);
void send_possitive_response_reset(void);
#if 0
// after verified elimnated 2 functions 
PRIVATE uint32_t rotl32 (uint32_t n, uint16_t c);
PRIVATE uint32_t rotr32 (uint32_t n, uint16_t c);
#endif 
void random_value(seed_value* randaom_value);   
void random_value_level3(seed_value_level3* randaom_value); 
void Security_Level1(void);
void Security_Level3(void);
void Securitykey_Level3(void);
PRIVATE uint32_t seed_and_key_algorithm(void);
void Respond_Multiple_Frames(uint16_t length);
//void udss_Readdatabyidentifier(void);
void ecu_reset(void);
void communication_Control_State_fun(void);
uint8_t History_zone_Full[125];
uint8_t BOOT_FLAG_STATUS ;

//union Uds_data UDS_TX_DATA;
uint8_t UDS_TX_DATA[3500];
uint8_t ACTIVE_DIAG_SESSION = SET;
uint8_t DEF_BAR_NUMBER;
uint8_t DEF_LEVEL_PERCENT;
uint8_t BATTERY_VOLTAGE;

uint16_t STminval;
uint8_t STmin_cntr;

uint8_t RESET_SERVICE_REMINDER_UDS = CLEAR;

_Bool disable_can_rx_flag_req = 0;
_Bool disable_can_tx_flag_req = 0;
_Bool dtc_log_lock_flag = 0;

/**************************************/
uint8_t history_zone[25];

/* Seed Counters of Level 1 & 3 */
/* uint8_t seed_counter1 = 45;
uint8_t counter1=2;
uint8_t counter2=34; */

//struct FLAGS EOL;

/******************  W501  ********************/
/***********************************************************************************************************
## Module:      UDSS.c
## Function:    bit_Access()
## Description: This function is used to access bit data from data byte.
## Parameter:   None
## Return£º     void
 **********************************************************************************************************/

void Bit_flag_Access(void)
{

	feature_code_1stbyte.byte=Feature_code_arr[0];
	feature_code_2ndbyte.byte=Feature_code_arr[1];
	feature_code_3rdbyte.byte=Feature_code_arr[2];
	feature_code_4thbyte.byte=Feature_code_arr[3];
	feature_code_5thbyte.byte=Feature_code_arr[4];
	feature_code_6thbyte.byte=Feature_code_arr[5];
	feature_code_7thbyte.byte=Feature_code_arr[6];
	feature_code_8thbyte.byte=Feature_code_arr[7];
	feature_code_9thbyte.byte=Feature_code_arr[8];
	feature_code_10thbyte.byte=Feature_code_arr[9];
	feature_code_11thbyte.byte=Feature_code_arr[10];
	feature_code_12thbyte.byte=Feature_code_arr[11];
	feature_code_13thbyte.byte=Feature_code_arr[12];
	feature_code_14thbyte.byte=Feature_code_arr[13];
	feature_code_15thbyte.byte=Feature_code_arr[14];
	feature_code_16thbyte.byte=Feature_code_arr[15];
	feature_code_17thbyte.byte=Feature_code_arr[16];
	feature_code_18thbyte.byte=Feature_code_arr[17];
	feature_code_19thbyte.byte=Feature_code_arr[18];
	feature_code_20thbyte.byte=Feature_code_arr[19];

	Varient_code_1st_byte.byte=Varient_code_arr[0];
	Varient_code_2nd_byte.byte=Varient_code_arr[1];
	Varient_code_3rd_byte.byte=Varient_code_arr[2];
	Varient_code_4th_byte.byte=Varient_code_arr[3];
	Varient_code_5th_byte.byte=Varient_code_arr[4];
	/*Reserved*/
	Varient_code_6th_byte.byte=Varient_code_arr[5];
	Varient_code_7th_byte.byte=Varient_code_arr[6];
	Varient_code_8th_byte.byte=Varient_code_arr[7];
	Varient_code_9th_byte.byte=Varient_code_arr[8];
	Varient_code_10th_byte.byte=Varient_code_arr[9];
	Varient_code_11th_byte.byte=Varient_code_arr[10];
	Varient_code_12th_byte.byte=Varient_code_arr[11];
	Varient_code_13th_byte.byte=Varient_code_arr[12];
	Varient_code_14th_byte.byte=Varient_code_arr[13];
	Varient_code_15th_byte.byte=Varient_code_arr[14];
	Varient_code_16th_byte.byte=Varient_code_arr[15];
	Varient_code_17th_byte.byte=Varient_code_arr[16];
	Varient_code_18th_byte.byte=Varient_code_arr[17];
	Varient_code_19th_byte.byte=Varient_code_arr[18];
	Varient_code_20th_byte.byte=Varient_code_arr[19];


	NS_Varient_code_1st_byte.byte=NS_Varient_code_arr[0];
	NS_Varient_code_2nd_byte.byte=NS_Varient_code_arr[1];
	NS_Varient_code_3rd_byte.byte=NS_Varient_code_arr[2];
	NS_Varient_code_4th_byte.byte=NS_Varient_code_arr[3];
	NS_Varient_code_5th_byte.byte=NS_Varient_code_arr[4];
	NS_Varient_code_6th_byte.byte=NS_Varient_code_arr[5];
	NS_Varient_code_7th_byte.byte=NS_Varient_code_arr[6];
	NS_Varient_code_8th_byte.byte=NS_Varient_code_arr[7];
	NS_Varient_code_9th_byte.byte=NS_Varient_code_arr[8];
	NS_Varient_code_10th_byte.byte=NS_Varient_code_arr[9];
	NS_Varient_code_11th_byte.byte=NS_Varient_code_arr[10];
	NS_Varient_code_12th_byte.byte=NS_Varient_code_arr[11];
	NS_Varient_code_13th_byte.byte=NS_Varient_code_arr[12];
	NS_Varient_code_14th_byte.byte=NS_Varient_code_arr[13];
	NS_Varient_code_15th_byte.byte=NS_Varient_code_arr[14];
	NS_Varient_code_16th_byte.byte=NS_Varient_code_arr[15];
	NS_Varient_code_17th_byte.byte=NS_Varient_code_arr[16];
	NS_Varient_code_18th_byte.byte=NS_Varient_code_arr[17];
	NS_Varient_code_19th_byte.byte=NS_Varient_code_arr[18];
	NS_Varient_code_20th_byte.byte=NS_Varient_code_arr[19];

	Fuel_Bar_Status_1st_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0];
	Fuel_Bar_Status_2nd_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[1];
	Fuel_Bar_Status_3rd_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[2];
	Fuel_Bar_Status_4th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[3];
	Fuel_Bar_Status_5th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[4];
	Fuel_Bar_Status_6th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[5];
	Fuel_Bar_Status_7th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[6];
	Fuel_Bar_Status_8th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[7];

	Fuel_Bar_Status_9th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[8];
	Fuel_Bar_Status_10th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[9];
	Fuel_Bar_Status_11th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[10];
	Fuel_Bar_Status_12th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[11];
	Fuel_Bar_Status_13th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[12];
	Fuel_Bar_Status_14th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[13];
	Fuel_Bar_Status_15th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[14];
	Fuel_Bar_Status_16th_byte.byte=DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[15];

	Temp_Bar_Status_1st_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0]-40);
	Temp_Bar_Status_2nd_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[1]-40);
	Temp_Bar_Status_3rd_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[2]-40);
	Temp_Bar_Status_4th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[3]-40);
	Temp_Bar_Status_5th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[4]-40);
	Temp_Bar_Status_6th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[5]-40);
	Temp_Bar_Status_7th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[6]-40);
	Temp_Bar_Status_8th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[7]-40);
	Temp_Bar_Status_9th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[8]-40);
	Temp_Bar_Status_10th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[9]-40);
	Temp_Bar_Status_11th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[10]-40);
	Temp_Bar_Status_12th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[11]-40);
	Temp_Bar_Status_13th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[12]-40);
	Temp_Bar_Status_14th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[13]-40);
	Temp_Bar_Status_15th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[14]-40);
	Temp_Bar_Status_16th_byte.byte=(DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[15]-40);

	Fuel_Tell_Tale_value_1st_byte.byte=DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0];
	Fuel_Tell_Tale_value_2nd_byte.byte=DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[1];
	Fuel_Tell_Tale_value_3rd_byte.byte=DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[2];
	Fuel_Tell_Tale_value_4th_byte.byte=DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[3];

	Temp_Tell_Tale_value_1st_byte.byte=(DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0]-40);
	Temp_Tell_Tale_value_2nd_byte.byte=(DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[1]-40);
	Temp_Tell_Tale_value_3rd_byte.byte=(DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[2]-40);
	Temp_Tell_Tale_value_4th_byte.byte=(DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[3]-40);

	//Chime Configuration 503//
	Chime_503_0th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[0];
	Chime_503_1st_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[1];
	Chime_503_2nd_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[2];
	Chime_503_3rd_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[3];
	Chime_503_4th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[4];
	Chime_503_5th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[5];
	Chime_503_6th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[6];
	Chime_503_7th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[7];
	Chime_503_8th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[8];
	Chime_503_9th_byte.byte=DID_CHIMES_CONFIG_0x0503_arr[9];

	//Message Configuration 503//
	Message_504_0th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[0];
	Message_504_1st_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[1];
	Message_504_2nd_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[2];
	Message_504_3rd_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[3];
	Message_504_4th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[4];
	Message_504_5th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[5];
	Message_504_6th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[6];
	Message_504_7th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[7];
	Message_504_8th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[8];
	Message_504_9th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[9];
	Message_504_10th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[10];
	Message_504_11th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[11];
	Message_504_12th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[12];
	Message_504_13th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[13];
	Message_504_14th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[14];
	Message_504_15th_byte.byte=DID_MESSAGE_CONFIG_0x0504_arr[15];

	//ETM
	Message_500_0th_byte.byte=DID_ETM_0x0500_arr[0];
}




/***********************************************************************************************************
## Module:      UDSS.c
## Function:    Process_UDSData()
## Description: Process data received on UDS.
## Parameter:   None
## Return£º     void
 **********************************************************************************************************/
PUBLIC void Process_UDSData(void)
{
	uint8_t Service_ID, count1,count2,count3;
	uint16_t DID = CLEAR;
	//uint64_t seed;
	//seed_valuel3 received_key;
	unsigned char test[4];

	srand(rand());

	if(FrameType.Single_Frame == SET)
	{
		Service_ID = UDS_Buffer[1];
		switch(Service_ID)
		{
		case SESSION_CONTROL:
			udss_SessionControl();
			S3ECU_cntr = SET;
			break;

		case SECURITY_ACCESS:
			security_Access();
			S3ECU_cntr = SET;
			break;

		case TESTER_PRESENT:
			tester_present_service();
			S3ECU_cntr = SET;
			break;

		case READ_DATA_BY_IDENTIFIER:
			if(UDS_Buffer[0] == 0x03)
			{
				udss_Readdatabyidentifier();
				S3ECU_cntr = SET;
			}
			else if(UDS_Buffer[0] > 0x03)
			{
				udss_Readdatabyidentifier_MultipleDID();
				S3ECU_cntr = SET;
			}
			else
			{
				Save_NegResponse(UDS_Buffer[1], NRC_ICDL_0x13);
				Respond_Single_Frame(UDS_Buffer[1]);
				S3ECU_cntr = SET;
			}
			break;

		case WRITE_DATA_ID:
			WriteDataByIdentifier(DID);
			S3ECU_cntr = SET;
			break;

		case CONTROL_DTC:
			control_dtc_fun();
			S3ECU_cntr = SET;
			break;

		case COMMUNICATION_CONTROL:
			communication_Control_State_fun();
			S3ECU_cntr = SET;
			break;

		case READ_DTC:
			if(UDS_Buffer[0] >= 0x03)
			{
				read_dtcs();
				S3ECU_cntr = SET;
			}
			else
			{
				Save_NegResponse(UDS_Buffer[1], NRC_ICDL_0x13);
				Respond_Single_Frame(UDS_Buffer[1]);
				S3ECU_cntr = SET;
			}
			break;

		case CLEAR_DTC:
			clear_dtcs();
			S3ECU_cntr = SET;
			break;
			
		case ECU_RESET:	
			ecu_reset();
			break;
			
		case IOCONTROL:
			S3ECU_cntr = SET;
			IO_Control();
			break;

		default:
			Save_NegResponse(UDS_Buffer[1], NRC_SRNS_0x11);
			Respond_Single_Frame(UDS_Buffer[1]);
			S3ECU_cntr = SET;
			break;
		}
		FrameType.Single_Frame = CLEAR;
	}
	else if(FrameType.First_Frame == TRUE)
	{
		//if(UDS_Buffer[2] == SECURITY_ACCESS )
		//{
		//	seedFlowControl();
		//}
		//else
		//{
			sendFlowControl();
		//}

	}
	else if(FrameType.Cons_Frame == TRUE)
	{
			No_Received_Frames++;
			NBs_cntr = SET;
			for(count1 = 1; count1 < 8; count1++)
			{
				Rx_multiplebytes[Rx_multiplecount++] = UDS_Buffer[count1];
			}
//		}
		if(No_Received_Frames == No_Expected_Frames)
		{
			NBs_cntr = CLEAR;
			switch(StoredService_ID)
			{

			case WRITE_DATA_ID:
				WriteDataByIdentifier_multipleframes();
				S3ECU_cntr = SET;
				break;
				
			case READ_DATA_BY_IDENTIFIER:
				 udss_Readdatabyidentifier_MultipleDID();
				 S3ECU_cntr = SET;
				 break;
					
			case SECURITY_ACCESS:
				Securitykey_Level3();
				S3ECU_cntr = SET;
				break;
				
			case IOCONTROL:
				S3ECU_cntr = SET;
				IO_Control_MultiFrame();
				break;			

			default:
				Save_NegResponse(Rx_multiplebytes[2], NRC_SRNS_0x11);
				Respond_Single_Frame(UDS_Buffer[1]);
				S3ECU_cntr = SET;
				break;
			}
			No_Received_Frames = CLEAR;
			No_Expected_Frames = CLEAR;
			No_Expected_Bytes  = CLEAR;
			Rx_multiplecount   = CLEAR;
		}
		FrameType.Cons_Frame = CLEAR;
	}
	else if(FrameType.Flow_Control == TRUE)
	{
		flow_Control_State_rec = SET;
		STminval = ((uint16_t)UDS_Buffer[2] * 1000);
		if(STminval == FALSE)
		{
			STminval = 4000;
		}

		Respond_Multiple_Frames(0xFF);
	}
	/* memset(UDS_Buffer,0,sizeof(UDS_Buffer)); */
}

/*******************************************************************************************************
## Module:      UDSS.c
## Function:    udss_SessionControl()
## Description: Controls the UDS sessions.
## Parameter:   None
## Return£º     void
 ********************************************************************************************************/
PRIVATE void udss_SessionControl(void)
{
	unsigned char PROGRAMMING_READ=1;

		if(UDS_Buffer[0] == 0x02) /* Datalength is correct */
		{
			switch(UDS_Buffer[2])
			{
				case DEFAULT_SESSION:
					/* Write Programming flag = 0 to EEPROM
                                        make externded session flag = CLEAR; */
					if(Functional_Addressing_Flag == CLEAR)
					{
					    ACTIVE_DIAG_SESSION = DEFAULT_SESSION;
					    ext_session_active  = CLEAR;
					    prog_session_active = CLEAR;//r
					    Security_Access_Success = CLEAR;
					    Security_Access_Level3_Success = CLEAR;
					    //		disable_can_rx_flag = CLEAR; w501
					    //	disable_can_tx_flag = CLEAR;
					    stop_communication  = CLEAR;
					    TX_DATA[0] = 0x06;
					    TX_DATA[1] = 0x50;
					    TX_DATA[2] = 0x01;
					    TX_DATA[3] = (uint8_t)((P2CAN_ECU_TIMEOUT/10)>>8);
					    TX_DATA[4] = (uint8_t)(P2CAN_ECU_TIMEOUT/10);
					    TX_DATA[5] = (uint8_t)((S3ECU_TIMEOUT/100)>>8);
					    TX_DATA[6] = (uint8_t)(S3ECU_TIMEOUT/100);
					    TX_DATA[7] = 0x00;					
					    S3ECU_cntr = CLEAR;
						Clear_UDS_variables();
					    Respond_Single_Frame(SESSION_CONTROL);
					}
					break;

				case PROGRAMMING_SESSION:
					//ACTIVE_DIAG_SESSION = PROGRAMMING_SESSION;
					//Security_Access_Success = SET;
					//ext_session_active = SET;
					if(Functional_Addressing_Flag == CLEAR)
					{
					    if((ext_session_active == SET) && (Security_Access_Level3_Success == SET))//Security_Access_Success
					    {
							if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
							{
								if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
								{
									ACTIVE_DIAG_SESSION = PROGRAMMING_SESSION;
									prog_session_active = SET;//r
									PROGRAMMING_FLAG = CLEAR;
									delay_time(100);
									eeprom_writebt(PROGRAMMING_FLAG_OFFSET, PROGRAMMING_FLAG);/* Write Programming flag = 1 to EEPROM  */
									delay_time(100);
								//	eeprom_write(PROGRAMMING_FLAG_OFFSET, PROGRAMMING_FLAG);
									S3ECU_cntr = SET;
									delay_time(100);
									PROGRAMMING_READ = eeprom_read(PROGRAMMING_FLAG_OFFSET);
									delay_time(100);
									Save_NegResponse(SESSION_CONTROL, NRC_WAIT_0x78);
									Respond_Single_Frame(SESSION_CONTROL);
									/* Delay1(2); */
									//	wdt_reset_cntr = SET;
									eeprom_write(rtc_hrs,RTCA0HOURC);
									eeprom_write(rtc_min,RTCA0MINC );
									eeprom_write(rtc_sec,RTCA0SECC);
									protected_write(SYSPROTCMD0,SYSPROTS0,SYSSWRESA,0x01);/*Software reset*/
									/* RESET_WDTSIGNAL; */
								}
								else
								{
									Save_NegResponse(SESSION_CONTROL, NRC_VSTH_0x88);	//VSTH -Vehicle Speed Too High
									//Respond_Single_Frame(SESSION_CONTROL); 
								}
							}
							else
							{
								Save_NegResponse(SESSION_CONTROL, NRC_EIR_0x83);	  // EIR -Engine is running
								//Respond_Single_Frame(SESSION_CONTROL);
							}
								
							}
					    else if(ext_session_active == SET) /* if cluster is in Extended session */
					    {
						  Save_NegResponse(SESSION_CONTROL,NRC_SAD_0x33);
					    }
					    else
					    {
						  Save_NegResponse(SESSION_CONTROL,NRC_SFNS_0x7E);
					    }
					    Respond_Single_Frame(SESSION_CONTROL);
					}
					break;

				case EXTENDED_SESSION:
				case EXTENDED_SESSION_SPR:
					ACTIVE_DIAG_SESSION = EXTENDED_SESSION;
					prog_session_active = CLEAR;//r
					Security_Access_Success = CLEAR;
					Security_Access_Level3_Success = CLEAR;
					ext_session_active  = SET;
					if(UDS_Buffer[2] != EXTENDED_SESSION_SPR)
					{
					    TX_DATA[0] = 0x06;
					    TX_DATA[1] = 0x50;
					    TX_DATA[2] = 0x03;
					    TX_DATA[3] = (uint8_t)((P2CAN_ECU_TIMEOUT/10)>>8);
					    TX_DATA[4] = (uint8_t)(P2CAN_ECU_TIMEOUT/10);
					    TX_DATA[5] = (uint8_t)((S3ECU_TIMEOUT/100)>>8);
					    TX_DATA[6] = (uint8_t)(S3ECU_TIMEOUT/100);
					    TX_DATA[7] = 0x00;
						Respond_Single_Frame(SESSION_CONTROL);
					}
					S3ECU_cntr = SET;
					break;

				default:
					Save_NegResponse(SESSION_CONTROL, NRC_SFNC_0x12);
					Respond_Single_Frame(SESSION_CONTROL);
					break;
			}
		}
		else /* Datalength is incorrect */
		{
			Save_NegResponse(SESSION_CONTROL, NRC_ICDL_0x13);
			Respond_Single_Frame(SESSION_CONTROL);
		}
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	security_Access()
## Description: Process security access request received on UDS .
## Parameter:	None
## Return£º  	void
**********************************************************************************/
void security_Access(void)
{
    if(Functional_Addressing_Flag == CLEAR)
    {
	if(ext_session_active == SET)
	{
		if((UDS_Buffer[0] == 0x02) || (UDS_Buffer[0] == 0x06 )) /* Datalength is correct */
		{
			switch(UDS_Buffer[2])
			{
			      case REQUEST_SEED:
				Security_Level1();
			      break;
			      
			      case REQUEST_SEED_LEVEL3:
				Security_Level3();
			      break;
				       		
					
				case SEND_KEY:
					if((seed_received == SET) && (no_of_attempts < 11))
					{
						seed_received = CLEAR;
						received_key.Bytes_in_seed[0] = UDS_Buffer[6];
						received_key.Bytes_in_seed[1] = UDS_Buffer[5];
						received_key.Bytes_in_seed[2] = UDS_Buffer[4];
						received_key.Bytes_in_seed[3] = UDS_Buffer[3];	
						//key_val = seed_and_key_algorithm();
						generateKeyLevel1(Seed.seed_as_a_whole, &keyvalidation);
						if(keyvalidation == received_key.seed_as_a_whole)
						{
					        	no_of_attempts = 0;		
							Security_Access_Success = SET;
							Security_Access_Level3_Success = CLEAR;
							//ACTIVE_DIAG_SESSION = SECURITY_ACCESS;
							TX_DATA[0] = 0x02;
							TX_DATA[1] = 0x67;
							TX_DATA[2] = 0x02;
							TX_DATA[3] = 0x00;
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							Respond_Single_Frame(SECURITY_ACCESS);
						}
						else
						{
							no_of_attempts++;
							if(no_of_attempts < 11)
							{
								if(no_of_attempts > 1)
								{
								  REQUEST_3SEC = SET;	
								}
								Save_NegResponse(0x27, NRC_WKEY_0x35);
								Respond_Single_Frame(SECURITY_ACCESS);
							}
							else
							{
								no_of_attempts = 12;
								Save_NegResponse(0x27, NRC_EXCEEDATT_0x36);
								Respond_Single_Frame(SECURITY_ACCESS);
							}
						}
					}
					else
					{	
						Save_NegResponse(0x27, NRC_WRSQ_0x24);
						Respond_Single_Frame(SECURITY_ACCESS);
					}
					
				        break;
					
				default:
					Save_NegResponse(0x27, NRC_SFNC_0x12);
					Respond_Single_Frame(SECURITY_ACCESS);
				        break;
			}
		}
		else /* Datalength is incorrect */
		{
			Save_NegResponse(0x27, NRC_ICDL_0x13);
			Respond_Single_Frame(SECURITY_ACCESS);
		}
	}
	else
    	{
		     Save_NegResponse(0x27,NRC_SNS_0x7F);
			 Respond_Single_Frame(SECURITY_ACCESS);
			 
	   	}
	if(Wrong_seed_Attempts > 10)
	{	
		Save_NegResponse(0x27, NRC_ENA_0x36);
		Respond_Single_Frame(SECURITY_ACCESS);
	}
	//memset(Rx_multiplebytes, 0, sizeof(Rx_multiplebytes)); 
     }	
}

/**********************************************************************************************************
## Module:      UDSS.c
## Function:    sendFlowControl()
## Description: Controls the data sending flow.
## Parameter:   None
## Return£º     void
 **********************************************************************************************************/
PRIVATE void sendFlowControl(void)
{
	uint8_t count      = CLEAR;
	//Rx_multiplecount   = CLEAR;
	No_Expected_Frames = CLEAR;
	No_Received_Frames = CLEAR;
	StoredService_ID   = UDS_Buffer[2];
	No_Expected_Bytes  = ((UDS_Buffer[0] & 0x0F) << 8) | (UDS_Buffer[1]);
	No_Expected_Frames = (No_Expected_Bytes - 6) / 7;

	if((No_Expected_Bytes - 6) % 7)
	{
		No_Expected_Frames++;
	}

	for(count = 0; count < 8; count++)
	{
		Rx_multiplebytes[Rx_multiplecount] = UDS_Buffer[count];
		Rx_multiplecount++;
	}

	TX_DATA[0] = 0x30;
	TX_DATA[1] = FC_BLOCK_SIZE;
	TX_DATA[2] = STmin;
	TX_DATA[3] = 0x00;
	TX_DATA[4] = 0x00;
	TX_DATA[5] = 0x00;
	TX_DATA[6] = 0x00;
	TX_DATA[7] = 0x00;

	if(P2CAN_ECU_Cntr > 0)
	{
		//tx_msgbuf_processing(15, 8, TX_DATA);
		tx_msgbuf_processing( 8, TX_DATA);
		NBs_cntr = SET;
	}
	memset(TX_DATA, 0, sizeof(TX_DATA));
	FrameType.First_Frame = CLEAR;
	P2CAN_ECU_Cntr = CLEAR;
}

/**********************************************************************************
## Module:      UDSS.c
## Function:    tester_present_service()
## Description: tester present service functionality.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PRIVATE void tester_present_service(void)
{
	if (UDS_Buffer[0] == 0x02)
	{
		if(UDS_Buffer [2] == 0x00)
		{
			TX_DATA[0] = 0x02;
			TX_DATA[1] = 0x7E;
			TX_DATA[2] = UDS_Buffer[2];
			TX_DATA[3] = 0X00;
			TX_DATA[4] = 0x00;
			TX_DATA[5] = 0x00;
			TX_DATA[6] = 0x00;
			TX_DATA[7] = 0x00;
			Respond_Single_Frame(TESTER_PRESENT);
		}
		else if(UDS_Buffer[2] == 0x80)
		{
			S3ECU_cntr = 1;
		}
		else
		{
			Save_NegResponse(TESTER_PRESENT,NRC_SFNC_0x12);
			Respond_Single_Frame(TESTER_PRESENT);
		}
	}
	else
	{
		Save_NegResponse(TESTER_PRESENT,NRC_ICDL_0x13);
		Respond_Single_Frame(TESTER_PRESENT);
	}
}

/**********************************************************************************
## Module:      UDSS.c
## Function:    Respond_Single_Frame()
## Description: Respond single frame .
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void Respond_Single_Frame(uint8_t service_id)
{
	uint8_t TX_DATA_NRC78[8];

	if(P2CAN_ECU_Cntr > 0)
	{
		S3ECU_cntr = SET;
	}

	P2CAN_ECU_Cntr 	       = CLEAR;
	FrameType.Single_Frame = CLEAR;
	//tx_msgbuf_processing(15, 8, TX_DATA);
	tx_msgbuf_processing(8, TX_DATA);
	memset(TX_DATA, 0, sizeof(TX_DATA));
}

/**********************************************************************************
## Module:      UDSS.c
## Function:    Save_NegResponse()
## Description: Save_NegResponse.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void Save_NegResponse(uint8_t serviceID,uint8_t NRC)
{
	TX_DATA[0] = 0x03;
	TX_DATA[1] = 0x7F;
	TX_DATA[2] = serviceID;
	TX_DATA[3] = NRC;
	TX_DATA[4] = 0x00;
	TX_DATA[5] = 0x00;
	TX_DATA[6] = 0x00;
	TX_DATA[7] = 0x00;
}

/**********************************************************************************
## Module:      UDSS.c
## Function:    Clear_UDS_variables()
## Description: clear all variables used for UDS on session timeout.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void Clear_UDS_variables(void)
{
	ext_session_active      = CLEAR;
	Security_Access_Success = CLEAR;
	ACTIVE_DIAG_SESSION 	= DEFAULT_SESSION;
	disable_can_rx_flag_req 	= CLEAR;
	disable_can_tx_flag_req 	= CLEAR;
	stop_communication  	= CLEAR;
	dtc_log_lock_flag       = CLEAR;
	seed_received = CLEAR;
	IO_Parameter_Request_Status = CLEAR;
	IO_Control_Illumination_set =CLEAR;
	temp_widget = 0;
	pre_temp_val=0;
	if(IO_Parameter_Request_Status == CLEAR)
	{
		IO_CONTROL_SET1_TT=0;
		IO_CONTROL_SET2_TT=0;
		IO_CONTROL_SET3_TT=0;
		IO_CONTROL_SET4_TT=0;
		IO_CONTROL_SET5_TT=0;
		IO_CONTROL_SET6_TT=0;

		IO_Control_State_Speedometer_Status = 0;
		IO_Control_State_Tachometer_Status = 0;
		IO_TFT_TEST = 0;
		IO_Control_State_Bar_Graph_Status.byte = 0x00;
		IO_Control_Enable_Bar_Graph.byte = 0x00;
		IO_Control_State_Gear_IND_Status_Byte0.byte = 0x00;
		IO_Control_State_Gear_IND_Status_Byte1.byte = 0x00;
		IO_Control_Enable_Gear_IND.byte = 0x00;
		IO_Control_State_Display_Status = 0x00;
		IO_Control_Enable_Display.byte = 0x00;
		IO_Control_Enable_Guages.byte = 0x00;
		Turn_left_TT = 0;
		Turn_right_TT =0;
		drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
	}
	//eeprom_write(RESET_BOOT_FLAG,0x00); W501
}

/**********************************************************************************
## Module:      UDSS.c
## Function:    send_possitive_response_reset()
## Description: send possitive response for reset after successfully downloading application.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void send_possitive_response_reset(void)
{
	TX_DATA[0] = 0x02;
	TX_DATA[1] = 0x51;
	TX_DATA[2] = 0x01;
	TX_DATA[3] = 0x00;
	TX_DATA[4] = 0x00;
	TX_DATA[5] = 0x00;
	TX_DATA[6] = 0x00;
	TX_DATA[7] = 0x00;
	//tx_msgbuf_processing(15, 8, TX_DATA);
	tx_msgbuf_processing(8, TX_DATA);
}

/**********************************************************************************
## Module:      UDSS.c
## Function:    random_value()
## Description: generates random seed.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PRIVATE void random_value(seed_value* randaom_value)
{
	uint8_t i;
    /* seed_counter1++;
	srand(seed_counter1); */

	for(i=0; i<4; i++)
	{
		randaom_value->Bytes_in_seed[i] = rand();
	}
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	random_value_level3()
## Description: generates random seed.
## Parameter:	None
## Return£º  	void
**********************************************************************************/
void random_value_level3(seed_value_level3* randaom_value)
{
  unsigned char i;
  /* counter1++;
  srand(counter1);//Bytes_in_seed[1]
  eeprom_write(SEED1_COUNTER_OFFSET, counter1);  */
  for(i = 0; i < 4; i++)
  {
    (randaom_value -> Bytes_in_seed[i]) = rand();
  }
  /* counter2++;
  srand(counter2);//Bytes_in_seed[2]
  eeprom_write(SEED1_COUNTER_OFFSET, counter2); */ 
  for(i = 4; i < 8; i++)
  {
    (randaom_value -> Bytes_in_seed[i]) = rand();
  }
  
}
/**********************************************************************************
## Module:      UDSS.c
## Function:    UpdateHistoryZone()
## Description: UpdateHistoryZone.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
/*void UpdateHistoryZone(void)
{

}
 */

/**********************************************************************************
## Module:      UDSS.c
## Function:    Respond_Multiple_Frames()
## Description: Transmit response with more than 8 bytes.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void Respond_Multiple_Frames(uint16_t length)
{
	uint8_t i;
	uint16_t j;

	PRIVATE uint8_t no_frames_sent = CLEAR;
	PRIVATE uint8_t byte_cnt       = CLEAR;
	PRIVATE uint8_t no_of_frames   = CLEAR;
	PRIVATE uint8_t sequence_no    = 0x21;

	if(flow_Control_State_rec == FALSE)
	{
		j            = length;
		length       = length - 8;
		no_of_frames = length / 7;

		if(length % 7)
			//if(length <= 8)
		{
			no_of_frames++;
		}

		for(i = 0; i < 8; i++)
		{
			TX_DATA[i] = UDS_TX_DATA[i];
		}
		byte_cnt = 8;
		FrameType.Single_Frame = CLEAR;
		S3ECU_cntr = SET;
	}
	else if(no_of_frames > 0)
	{
		if(no_frames_sent < (no_of_frames - 1))
		{
			TX_DATA[0] = sequence_no;
			for(i = 1; i < 8; i++)
			{
				TX_DATA[i] = UDS_TX_DATA[byte_cnt++];
			}
			sequence_no++;
			if(sequence_no > 0x2F)
			{
				sequence_no = 0x20;
			}
			STmin_cntr = SET;
			no_frames_sent++;
			FrameType.Flow_Control = CLEAR;
		}
		else
		{
			TX_DATA[0] = sequence_no;
			for(i = 1; i < 8; i++)
			{
				TX_DATA[i] = UDS_TX_DATA[byte_cnt++];
			}
			sequence_no = 0x21;

			memset(UDS_TX_DATA, 0, sizeof(UDS_TX_DATA));
			memset(UDS_Buffer, 0, sizeof(UDS_Buffer));

			FrameType.Flow_Control = CLEAR;
			flow_Control_State_rec       = CLEAR;
			STminval               = CLEAR;
			STmin_cntr             = CLEAR;
			S3ECU_cntr = SET;
			no_frames_sent = CLEAR;
			no_of_frames   = CLEAR;
			tx_msgbuf_processing(8, TX_DATA);
			P2CAN_ECU_Cntr = CLEAR;
		}
	}
	//tx_msgbuf_processing(15, 8, TX_DATA);
	if(no_of_frames > 0)
	{
	    tx_msgbuf_processing(8, TX_DATA);
	    P2CAN_ECU_Cntr = CLEAR;
	}
	else
	{
	    FrameType.Flow_Control = CLEAR;
	    flow_Control_State_rec = CLEAR;
	    STminval               = CLEAR;
	    STmin_cntr             = CLEAR;
		S3ECU_cntr = SET;
	}
}
/**********************************************************************************
## Module:   	UDSS.c
## Function: 	sendFlowControl_SEED()
## Description: Controls sending data flow.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void seedFlowControl(void)
{
	unsigned char count      = CLEAR;
	seed_received1 = SET;
	//Rx_multiplecount = CLEAR;
	No_Expected_Frames = CLEAR;
	StoredService_ID   = UDS_Buffer[2];
	No_Expected_Bytes  = (((UDS_Buffer[0] & 0x0F) << 8) | (UDS_Buffer[1]));
	//No_Expected_Frames = ((No_Expected_Bytes - 6) / 7);

	//	if((No_Expected_Bytes - 6) % 7)
	//	{
	//		No_Expected_Frames++;
	//	}

	for(count = 4; count < 8; count++)
	{
		seed_multiplebytes[seed_multiplecount] = UDS_Buffer[count];
		seed_multiplecount++;
	}

	TX_DATA[0] = 0x30;
	TX_DATA[1] = 0x00;
	TX_DATA[2] = STmin;
	TX_DATA[3] = 0x00;
	TX_DATA[4] = 0x00;
	TX_DATA[5] = 0x00;
	TX_DATA[6] = 0x00;
	TX_DATA[7] = 0x00;

	if(P2CAN_ECU_Cntr > 0)
	{
		tx_msgbuf_processing(8, TX_DATA);
		NBs_cntr = 1;
	}

	memset(TX_DATA, 0, sizeof(TX_DATA));

	FrameType.First_Frame = CLEAR;
	P2CAN_ECU_Cntr        = CLEAR;
}


/**********************************************************************************
## Module:   	UDSS.c
## Function: 	control_dtc_fun()
## Description: control dtc request processing .
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void control_dtc_fun(void)
{
	uint8_t send_response;
    if(ext_session_active == SET) 
    {
	   if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
	   {
			if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
			{	
				if(UDS_Buffer[0] == 2)
				{
						switch(UDS_Buffer[2])
						{
								case 0x01:
										TX_DATA[0] = 0x02;
										TX_DATA[1] = 0xC5;
										TX_DATA[2] = 0x01;
										TX_DATA[3] = 0x00;
										TX_DATA[4] = 0x00;
										TX_DATA[5] = 0x00;
										TX_DATA[6] = 0x00;
										TX_DATA[7] = 0x00;                        
										send_response = SET;
								case 0x81:
										/* DTC_stop = OFF; *//* Star DTC logging */
										dtc_log_lock_flag = CLEAR;
									   
									break;
						
								case 0x02:
										TX_DATA[0] = 0x02;
										TX_DATA[1] = 0xC5;
										TX_DATA[2] = 0x02;
										TX_DATA[3] = 0x00;
										TX_DATA[4] = 0x00;
										TX_DATA[5] = 0x00;
										TX_DATA[6] = 0x00;
										TX_DATA[7] = 0x00;                        
										send_response = SET;
						
								case 0x82:
										/* DTC_stop = ON; *//* Stop DTC logging */
										 dtc_log_lock_flag = SET;
									break;
						
								default:
										send_response = SET;
										Save_NegResponse(CONTROL_DTC,  NRC_SFNC_0x12 );
													  
									break;                                         		
						}
				
				}
				else
				{
					send_response = SET;
					Save_NegResponse(CONTROL_DTC, NRC_ICDL_0x13);    
				}
				if(send_response == TRUE)
				{                       
					Respond_Single_Frame(CONTROL_DTC);
				}
				else
				{
						S3ECU_cntr     = SET;
						P2CAN_ECU_Cntr = CLEAR;
				}
			}
	        else
			{
				Save_NegResponse(CONTROL_DTC, NRC_VSTH_0x88);	  // EIR -Engine is running
			Respond_Single_Frame(CONTROL_DTC);
			}
     	}
	    else
		{
			Save_NegResponse(CONTROL_DTC, NRC_EIR_0x83);	  // EIR -Engine is running
			Respond_Single_Frame(CONTROL_DTC);
		}
    }
    else
    {	
	  Save_NegResponse(CONTROL_DTC,NRC_SNS_0x7F);
	  Respond_Single_Frame(CONTROL_DTC); 
    }
}
/**********************************************************************************
## Module:   	UDSS.c
## Function: 	communication_Control_State_fun()
## Description: communication_control request processing .
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void communication_Control_State_fun(void)
{
	uint8_t send_response;
    if(ext_session_active == SET)
    {
		if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
		{
			if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
			{	
				if(UDS_Buffer[0] == 3)
				{
	                switch(UDS_Buffer[2])
	                {
	                        case 0x00:
									if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
									{
										TX_DATA[0] = 0x02;
										TX_DATA[1] = 0x68;
										TX_DATA[2] = 0x00;
										TX_DATA[3] = 0x00;
										TX_DATA[4] = 0x00;
										TX_DATA[5] = 0x00;
										TX_DATA[6] = 0x00;
										TX_DATA[7] = 0x00;                        
										send_response = SET;
									}
									else
									{
										send_response = SET;
										Save_NegResponse(COMM_CONTROL, NRC_ROFR_0x31);						
									}
					
	                        case 0x80:                                                              
	                                if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
	                                {
	                                        /* Enable Rx and Tx */  
	                                    disable_can_rx_flag_req = CLEAR;
	                                    disable_can_tx_flag_req = CLEAR;
	                                    stop_communication  = CLEAR;
	                                }
	                        	break;
					
	                        case 0x01:
									if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
									{
										TX_DATA[0] = 0x02;
										TX_DATA[1] = 0x68;
										TX_DATA[2] = 0x01;
										TX_DATA[3] = 0x00;
										TX_DATA[4] = 0x00;
										TX_DATA[5] = 0x00;
										TX_DATA[6] = 0x00;
										TX_DATA[7] = 0x00;                        
										send_response = SET;
									}
									else
									{
										send_response = SET;
										Save_NegResponse(COMM_CONTROL, NRC_ROFR_0x31);	
															
									}
					
	                        case 0x81:
	                                if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
	                                {
	                                        /* Enable Rx
	                                        Disable Tx */   
	                                    disable_can_rx_flag_req = CLEAR;
	                                    disable_can_tx_flag_req = SET;
	                                    stop_communication  = SET;
	                                }                               
	                        	break;
				
	                        case 0x02:  
									if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
									{
										TX_DATA[0] = 0x02;
										TX_DATA[1] = 0x68;
										TX_DATA[2] = 0x02;
										TX_DATA[3] = 0x00;
										TX_DATA[4] = 0x00;
										TX_DATA[5] = 0x00;
										TX_DATA[6] = 0x00;
										TX_DATA[7] = 0x00;                        
										send_response = SET;
									}
									else
									{
										send_response = SET;
										Save_NegResponse(COMM_CONTROL, NRC_ROFR_0x31);					
									}
					
	                        case 0x82:
	                                if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
	                                {                       
	                                        /*Disable Rx
	                                        Enable Tx */    
	                                        disable_can_rx_flag_req = SET;
	                                        disable_can_tx_flag_req = CLEAR;
	                                        stop_communication  = SET;
	                                }
	                       		break;
					
	                        case 0x03:
									if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
									{
														TX_DATA[0] = 0x02;
														TX_DATA[1] = 0x68;
														TX_DATA[2] = 0x03;
														TX_DATA[3] = 0x00;
														TX_DATA[4] = 0x00;
														TX_DATA[5] = 0x00;
														TX_DATA[6] = 0x00;
														TX_DATA[7] = 0x00;                        
														send_response = SET;
									}
									else
									{
										send_response = SET;
										Save_NegResponse(COMM_CONTROL, NRC_ROFR_0x31);
										
									}
					
	                        case 0x83:
	                                if((UDS_Buffer[3] == 0x01) || (UDS_Buffer[3] == 0x03))
	                                {                       
	                                        /*Disable Rx
	                                        Disable Tx */   
	                                        disable_can_rx_flag_req = SET;
	                                        disable_can_tx_flag_req = SET;
	                                        stop_communication  = SET;
	                                }
	                        	break;
					
	                        default:
	                                send_response = SET;
									Save_NegResponse(COMM_CONTROL, NRC_SFNC_0x12);
				    
	                                break;
					  
	                }
			 
			
				}
				else
				{
						send_response = SET;
						Save_NegResponse(COMM_CONTROL, NRC_ICDL_0x13);
						
				} 
			   
				if(send_response == SET)
				{                       
						Respond_Single_Frame(COMM_CONTROL);
				}
				else
				{
						S3ECU_cntr     = SET;
						P2CAN_ECU_Cntr = CLEAR;
				}
			}
			else 
			{
			  Save_NegResponse(COMM_CONTROL, NRC_VSTH_0x88);	//VSTH -Vehicle Speed Too High
			  Respond_Single_Frame(COMM_CONTROL);
			}	       
	    }
	    else
	    {
			Save_NegResponse(COMM_CONTROL, NRC_EIR_0x83);	  // EIR -Engine is running 
			Respond_Single_Frame(COMM_CONTROL);
		}
	
    }
    else
    {	
	    Save_NegResponse(COMM_CONTROL,NRC_SNS_0x7F);
	    Respond_Single_Frame(COMM_CONTROL); 
    }
}
/**********************************************************************************
## Module:   	UDSS.c
## Function: 	ecu_reset(void)
## Description: Ecu reset request processing .
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void ecu_reset(void)
{
    if(Functional_Addressing_Flag == CLEAR)
    {
		if((prog_session_active == SET) || (ext_session_active == SET))
		{
			if(Security_Access_Success == SET)    
			{
				if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
				{
					if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
					{
						if(UDS_Buffer[0] == 2)
						{
							switch(UDS_Buffer[2])
							{
								case HARD_RESET:
											  
												//eeprom_write(RESET_BOOT_FLAG, 1);// Write Programming flag = 1 to EEPROM				
												//eeprom_write(PROGRAMMING_FLAG_OFFSET, 1);
												//Save_NegResponse(0x11,NRC_WAIT_0x78);
												TX_DATA[0] = 0x02;  
												TX_DATA[1] = 0x51;
												TX_DATA[2] = UDS_Buffer[2];
												TX_DATA[3] = 0x00;
												TX_DATA[4] = 0x00; 
												TX_DATA[5] = 0x00;
												TX_DATA[6] = 0x00; 
												TX_DATA[7] = 0x00; 
												no_of_attempts=0;
												//reset_counter++;
												//eeprom_writebt(RESET_COUNTER_OFFSET,reset_counter);
												//delay_time(500);   /* To Allow wait to completed on PC application positive reponse  delay is added */
												wdt_reset_cntr = SET;								
												//delay_time(2000); 
												//RESET_WDTSIGNAL;						
												//__asm("0xFF;");
									break;
									
								default:
										Save_NegResponse(0x11, NRC_SFNC_0x12); 
										
								break;
							}
						}
						else
						{
							Save_NegResponse(0x11, NRC_ICDL_0x13);
						}
					}
					else
					{
						Save_NegResponse(0x11, NRC_VSTH_0x88);
					}
				}
				else
				{
					Save_NegResponse(0x11, NRC_EIR_0x83);
				}
			}
			else
			{
				Save_NegResponse(0x11, NRC_SAD_0x33);
			}
		}	
		else
		{
			Save_NegResponse(0x11, NRC_SNS_0x7F);	
		}   
	   
		Respond_Single_Frame(0x11);
	}
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	IO_Control(void)
## Description: IO Control request processing .
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void IO_Control(void)
{
    uint16_t IOID = CLEAR;
    if(Functional_Addressing_Flag == CLEAR)
    {
	if(ext_session_active == SET)
	{
		if(Security_Access_Success == SET)
		{
			if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
			{
				if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
				{
					if(UDS_Buffer[0] >= 0x04)
					{				
				
					IOID = ((UDS_Buffer[2] << 8) | UDS_Buffer[3]);
					
					switch(IOID)
					{
						case DID_TELL_TALE_STATUS_SET1_CONFIG_0xF07F:						
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{
									IO_Control_State_TT_SET1.byte = 0x00;
									IO_Control_Enable_TT_SET1.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET1_TT=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;	
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{
									IO_Control_State_TT_SET1.byte = UDS_Buffer[5];
									IO_Control_Enable_TT_SET1.byte = UDS_Buffer[6];
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET1_TT=1;
									TX_DATA[0] = 0x05;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = UDS_Buffer[5];
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									IO_CONTROL_SET1_TT=0;
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								IO_CONTROL_SET1_TT=0;
							}				
						break;
						
						case DID_TELL_TALE_STATUS_SET2_CONFIG_0xF080:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{							
									IO_Control_State_TT_SET2.byte = 0x00;
									IO_Control_Enable_TT_SET2.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET2_TT=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;	
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;	
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{							
									IO_Control_State_TT_SET2.byte = UDS_Buffer[5];
									IO_Control_Enable_TT_SET2.byte = UDS_Buffer[6];
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET2_TT=1;
									TX_DATA[0] = 0x05;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = UDS_Buffer[5];
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;	
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									IO_CONTROL_SET2_TT=0;
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								IO_CONTROL_SET2_TT=0;
							}
						break;
						
						case DID_TELL_TALE_STATUS_SET3_CONFIG_0xF081:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{
									IO_Control_State_TT_SET3.byte = 0x00;
									IO_Control_Enable_TT_SET3.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET3_TT=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{
									IO_Control_State_TT_SET3.byte = UDS_Buffer[5];
									IO_Control_Enable_TT_SET3.byte = UDS_Buffer[6];
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET3_TT=1;
									TX_DATA[0] = 0x05;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = UDS_Buffer[5];
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									IO_CONTROL_SET3_TT=0;
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								IO_CONTROL_SET3_TT=0;
							}
						break;
						
						case DID_TELL_TALE_STATUS_SET4_CONFIG_0xF08E:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{
									IO_Control_State_TT_SET4.byte = 0x00;
									IO_Control_Enable_TT_SET4.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET4_TT=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{
									IO_Control_State_TT_SET4.byte = UDS_Buffer[5];
									IO_Control_Enable_TT_SET4.byte = UDS_Buffer[6];
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET4_TT=1;
									TX_DATA[0] = 0x05;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = UDS_Buffer[5];
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									IO_CONTROL_SET4_TT=0;
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								IO_CONTROL_SET4_TT=0;
							}
						break;
						
						case DID_TELL_TALE_STATUS_SET5_CONFIG_0xF08F:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{
									IO_Control_State_TT_SET5.byte = 0x00;
									IO_Control_Enable_TT_SET5.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET5_TT=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{
									IO_Control_State_TT_SET5.byte = UDS_Buffer[5];
									IO_Control_Enable_TT_SET5.byte = UDS_Buffer[6];
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET5_TT=1;
									TX_DATA[0] = 0x05;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = UDS_Buffer[5];
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									IO_CONTROL_SET5_TT=0;
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								IO_CONTROL_SET5_TT=0;
							}
						break;
						
						case DID_BAR_GRAPH_STATUS_CONFIG_0xF083:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{							
									IO_Control_State_Bar_Graph_Status.byte = 0x00;
									IO_Control_Enable_Bar_Graph.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									temp_widget = 0;
									pre_temp_val=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{							
									if(((UDS_Buffer[5] & 0x0F) < 0x09) && ((UDS_Buffer[5] & 0xF0) < 0x90))
									{
										IO_Control_State_Bar_Graph_Status.byte = UDS_Buffer[5];
										IO_Control_Enable_Bar_Graph.byte = UDS_Buffer[6];
										IO_Parameter_Request_Status = UDS_Buffer[4];
										TX_DATA[0] = 0x05;
										TX_DATA[1] = 0x6F;
										TX_DATA[2] = UDS_Buffer[2];
										TX_DATA[3] = UDS_Buffer[3];
										TX_DATA[4] = UDS_Buffer[4];
										TX_DATA[5] = UDS_Buffer[5];
										TX_DATA[6] = 0x00;
										TX_DATA[7] = 0x00;
									}
									else
									{
										Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
									}
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
							}
						break;
						
						case DID_GEAR_INDICATION_STATUS_CONFIG_0xF085:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{							
									IO_Control_State_Gear_IND_Status_Byte0.byte = 0x00;
									IO_Control_State_Gear_IND_Status_Byte1.byte = 0x00;
									IO_Control_Enable_Gear_IND.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}							
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x07)
								{							
									if(UDS_Buffer[6] < 0x03)
									{
										IO_Control_State_Gear_IND_Status_Byte0.byte = UDS_Buffer[5];
										IO_Control_State_Gear_IND_Status_Byte1.byte = UDS_Buffer[6];
										IO_Control_Enable_Gear_IND.byte = UDS_Buffer[7];
										IO_Parameter_Request_Status = UDS_Buffer[4];			
										TX_DATA[0] = 0x06;
										TX_DATA[1] = 0x6F;
										TX_DATA[2] = UDS_Buffer[2];
										TX_DATA[3] = UDS_Buffer[3];
										TX_DATA[4] = UDS_Buffer[4];
										TX_DATA[5] = UDS_Buffer[5];
										TX_DATA[6] = UDS_Buffer[6];					
										TX_DATA[7] = 0x00;
									}
									else
									{
										Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
									}
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}						
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
							}
						break;
						
						case DID_DISPLAY_IO_CONTROL_0xF086:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{
									IO_TFT_TEST = 0;
									IO_Control_State_Display_Status = 0x00;
									IO_Control_Enable_Display.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[7] = 0x00;								
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{							
	#ifdef GRAYSCALE_TFT
									if(UDS_Buffer[5] < 0x05)
	#else
									if(UDS_Buffer[5] < 0x06)
	#endif
									{
										IO_TFT_TEST = 1;
										IO_Control_State_Display_Status = UDS_Buffer[5];
										IO_Control_Enable_Display.byte = UDS_Buffer[6];
										IO_Parameter_Request_Status = UDS_Buffer[4];				
										TX_DATA[0] = 0x04;
										TX_DATA[1] = 0x6F;
										TX_DATA[2] = UDS_Buffer[2];
										TX_DATA[3] = UDS_Buffer[3];
										TX_DATA[4] = UDS_Buffer[4];
										TX_DATA[5] = UDS_Buffer[5];					
										TX_DATA[7] = 0x00;							
									}
									else
									{
										Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
									}
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
							}
						break;
						
						case DID_TELL_TALE_STATUS_SET6_CONFIG_0xF096:
							if(UDS_Buffer[4] == 0x00)
							{
								if(UDS_Buffer[0] == 0x04)
								{
									IO_Control_State_TT_SET6.byte = 0x00;
									IO_Control_Enable_TT_SET6.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET6_TT=0;
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
								}
							}
							else if(UDS_Buffer[4] == 0x03)
							{
								if(UDS_Buffer[0] == 0x06)
								{
									IO_Control_State_TT_SET6.byte = UDS_Buffer[5];
									IO_Control_Enable_TT_SET6.byte = UDS_Buffer[6];
									IO_Parameter_Request_Status = UDS_Buffer[4];
									IO_CONTROL_SET6_TT=1;
									TX_DATA[0] = 0x05;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = UDS_Buffer[4];
									TX_DATA[5] = UDS_Buffer[5];
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									IO_CONTROL_SET6_TT=0;
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								IO_CONTROL_SET6_TT=0;
							}
						break;
						
						case DID_GAUGES_CONFIG_0xF082:
							if(UDS_Buffer[0] == 0x04)
							{
								if(UDS_Buffer[4] == 0x00)
								{
									IO_Control_State_Speedometer_Status = 0x0000;
									IO_Control_State_Tachometer_Status = 0x0000;
									IO_Control_State_Tachometer_Redline_Status = 0x00;
									IO_Control_Enable_Guages.byte = 0x00;
									IO_Parameter_Request_Status = UDS_Buffer[5];
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[3];
									TX_DATA[3] = UDS_Buffer[4];
									TX_DATA[4] = UDS_Buffer[5];
									TX_DATA[5] = 0x00;	
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;	
									//Respond_Single_Frame(IOCONTROL);  //W501 high end zero response sent to close this issue.
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
							}									
						break;
						
						case DID_ILLUMINATION_STATUS_CONFIG_0xF084:
							if(UDS_Buffer[0] == 0x04)
							{
								if(UDS_Buffer[4] == 0x00)
								{							
									IO_Control_State_LCD_Illumination_Status = 0x00;
									IO_Control_State_Dial_Illumination_Status = 0x00;
									IO_Control_State_Pointer_Illumination_Status = 0x00;
									IO_Control_Enable_Illumination.byte = 0x00;
									IO_Control_Illumination_set =0;
									IO_Parameter_Request_Status = UDS_Buffer[5];								
									TX_DATA[0] = 0x04;
									TX_DATA[1] = 0x6F;
									TX_DATA[2] = UDS_Buffer[3];
									TX_DATA[3] = UDS_Buffer[4];
									TX_DATA[4] = UDS_Buffer[5];
									TX_DATA[5] = 0x00;	
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;	
									//Respond_Single_Frame(IOCONTROL); //W501 high end zero response sent to close this issue.
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
								}
							}
							else
							{
								Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
							}	
						break;
						
/*						case DID_CHIME_ACTIATION_IO_CONTROL_0xF094:
 - 							if(UDS_Buffer[0] == 0x04)
 - 							{
 - 								if(UDS_Buffer[4] == 0x00)
 - 								{							
 - 									memset(IO_Control_State_Chime_Activation, 0, sizeof(IO_Control_State_Chime_Activation));
 - 									memset(IO_Control_Enable_Chime_Activation, 0, sizeof(IO_Control_Enable_Chime_Activation));
 - 									IO_Parameter_Request_Status = UDS_Buffer[5];								
 - 									TX_DATA[0] = 0x04;
 - 									TX_DATA[1] = 0x6F;
 - 									TX_DATA[2] = UDS_Buffer[3];
 - 									TX_DATA[3] = UDS_Buffer[4];
 - 									TX_DATA[4] = UDS_Buffer[5];
 - 									TX_DATA[5] = 0x00;	
 - 									TX_DATA[6] = 0x00;
 - 									TX_DATA[7] = 0x00;	
 - 									Respond_Single_Frame(IOCONTROL);
 - 								}
 - 								else
 - 								{
 - 									Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
 - 								}
 - 							}
 - 							else
 - 							{
 - 								Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
 - 							}
 - 						break;
 - 						
 - 						case DID_MESSAGE_ACTIATION_IO_CONTROL_0xF095:
 - 							if(UDS_Buffer[0] == 0x04)
 - 							{
 - 								if(UDS_Buffer[4] == 0x00)
 - 								{					
 - 									memset(IO_Control_State_Message_Activation, 0, sizeof(IO_Control_State_Message_Activation));
 - 									memset(IO_Control_Enable_Message_Activation, 0, sizeof(IO_Control_Enable_Message_Activation));
 - 									IO_Parameter_Request_Status = UDS_Buffer[5];								
 - 									TX_DATA[0] = 0x04;
 - 									TX_DATA[1] = 0x6F;
 - 									TX_DATA[2] = UDS_Buffer[3];
 - 									TX_DATA[3] = UDS_Buffer[4];
 - 									TX_DATA[4] = UDS_Buffer[5];
 - 									TX_DATA[5] = 0x00;	
 - 									TX_DATA[6] = 0x00;
 - 									TX_DATA[7] = 0x00;	
 - 									Respond_Single_Frame(IOCONTROL);
 - 								}
 - 								else
 - 								{
 - 									Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
 - 								}
 - 							}
 - 							else
 - 							{
 - 								Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
 - 							}
 - 						break;*/
						
						default:
							Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
						break;
					}
					}
					else
					{
						Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
					}
				}
				else
				{
					Save_NegResponse(IOCONTROL, NRC_VSTH_0x88);
				}
			}
			else
			{
				Save_NegResponse(IOCONTROL, NRC_EIR_0x83);
			}
		}
		else
		{
			Save_NegResponse(IOCONTROL, NRC_SAD_0x33);
		}
	}
	else
	{
		Save_NegResponse(IOCONTROL, NRC_SNS_0x7F);
	}
	
	Respond_Single_Frame(IOCONTROL);
    }
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	IO_Control_MultiFrame(void)
## Description: IO Control MultiFrame request processing .
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void IO_Control_MultiFrame(void)
{
    uint16_t IOID = CLEAR;
    if(Functional_Addressing_Flag == CLEAR)
    {
		if(ext_session_active == SET)
		{
			if(Security_Access_Success == SET)
			{
				if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
				{
					if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
					{
						IOID = ((Rx_multiplebytes[3] << 8) | Rx_multiplebytes[4]);
						
						switch(IOID)
						{
							case DID_GAUGES_CONFIG_0xF082:
								if(Rx_multiplebytes[1] == 0x0A)
								{
									if(Rx_multiplebytes[5] == 0x03)
									{							
										IO_Control_State_Speedometer_Status = ((Rx_multiplebytes[6] << 8) | Rx_multiplebytes[7]);
										IO_Control_State_Tachometer_Status = ((Rx_multiplebytes[8] << 8) | Rx_multiplebytes[9]);
										IO_Control_State_Tachometer_Redline_Status = Rx_multiplebytes[10];
										if((IO_Control_State_Speedometer_Status < 54000) &&
											(IO_Control_State_Tachometer_Status < 54000))
										{
											IO_Control_Enable_Guages.byte = Rx_multiplebytes[11];
											IO_Parameter_Request_Status = Rx_multiplebytes[5];
											UDS_TX_DATA[0] = 0x10;
											UDS_TX_DATA[1] = 0x0B;
											UDS_TX_DATA[2] = 0x6F;
											UDS_TX_DATA[3] = Rx_multiplebytes[3];
											UDS_TX_DATA[4] = Rx_multiplebytes[4];
											UDS_TX_DATA[5] = Rx_multiplebytes[5];
											UDS_TX_DATA[6] = Rx_multiplebytes[6];
											UDS_TX_DATA[7] = Rx_multiplebytes[7];
											UDS_TX_DATA[8] = Rx_multiplebytes[8];
											UDS_TX_DATA[9] = Rx_multiplebytes[9];
											UDS_TX_DATA[10] = Rx_multiplebytes[10];
											
											Respond_Multiple_Frames(UDS_TX_DATA[1]);
										}
										else
										{
											IO_Control_State_Speedometer_Status = 0x0000;
											IO_Control_State_Tachometer_Status = 0x0000;
											IO_Control_State_Tachometer_Redline_Status = 0x00;
											Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
											Respond_Single_Frame(IOCONTROL);
										}
									}
									else
									{
										Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
										Respond_Single_Frame(IOCONTROL);
									}
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									Respond_Single_Frame(IOCONTROL);
								}						
							break;
							
							case DID_ILLUMINATION_STATUS_CONFIG_0xF084:
								if(Rx_multiplebytes[1] == 0x0B)
								{
									if(Rx_multiplebytes[5] == 0x03)
									{							
										if((Rx_multiplebytes[6] < 0x65) &&
											(Rx_multiplebytes[7] < 0x65) &&
											(Rx_multiplebytes[8] < 0x65) &&
											(Rx_multiplebytes[9] == 0x00) &&
											(Rx_multiplebytes[10] == 0x00) &&
											(Rx_multiplebytes[11] == 0x00))
										{
											IO_Control_State_LCD_Illumination_Status = Rx_multiplebytes[6];
											IO_Control_State_Dial_Illumination_Status = Rx_multiplebytes[7];
											IO_Control_State_Pointer_Illumination_Status = Rx_multiplebytes[8];
											IO_Control_Enable_Illumination.byte = Rx_multiplebytes[12];
											IO_Parameter_Request_Status = Rx_multiplebytes[5];
											IO_Control_Illumination_set =1;
											UDS_TX_DATA[0] = 0x10;
											UDS_TX_DATA[1] = 0x0C;
											UDS_TX_DATA[2] = 0x6F;
											UDS_TX_DATA[3] = Rx_multiplebytes[3];
											UDS_TX_DATA[4] = Rx_multiplebytes[4];
											UDS_TX_DATA[5] = Rx_multiplebytes[5];
											UDS_TX_DATA[6] = Rx_multiplebytes[6];
											UDS_TX_DATA[7] = Rx_multiplebytes[7];
											UDS_TX_DATA[8] = Rx_multiplebytes[8];
											UDS_TX_DATA[9] = Rx_multiplebytes[9];
											UDS_TX_DATA[10] = Rx_multiplebytes[10];
											UDS_TX_DATA[11] = Rx_multiplebytes[11];
										
											Respond_Multiple_Frames(UDS_TX_DATA[1]);
										}
										else
										{
											Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
											Respond_Single_Frame(IOCONTROL);
										}
									}
									else
									{
										Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
										Respond_Single_Frame(IOCONTROL);
									}
								}
								else
								{
									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
									Respond_Single_Frame(IOCONTROL);
								}
							break;
							
/*							case DID_CHIME_ACTIATION_IO_CONTROL_0xF094:
 - 								if(Rx_multiplebytes[1] == 0x18)
 - 								{
 - 									if(Rx_multiplebytes[5] == 0x03)
 - 									{							
 - 										if((Rx_multiplebytes[10] == 0x00) && (Rx_multiplebytes[11] == 0x00) &&
 - 											(Rx_multiplebytes[12] == 0x00) && (Rx_multiplebytes[13] == 0x00) &&
 - 											(Rx_multiplebytes[14] == 0x00) && (Rx_multiplebytes[15] == 0x00) &&
 - 											(Rx_multiplebytes[20] == 0x00) && (Rx_multiplebytes[21] == 0x00) &&
 - 											(Rx_multiplebytes[22] == 0x00) && (Rx_multiplebytes[23] == 0x00) &&
 - 											(Rx_multiplebytes[24] == 0x00) && (Rx_multiplebytes[25] == 0x00))
 - 										{
 - 											memcpy(IO_Control_State_Chime_Activation, &Rx_multiplebytes[6], 10);
 - 											memcpy(IO_Control_Enable_Chime_Activation, &Rx_multiplebytes[16], 10);
 - 											IO_Parameter_Request_Status = Rx_multiplebytes[5];
 - 											UDS_TX_DATA[0] = 0x10;
 - 											UDS_TX_DATA[1] = 0x0E;
 - 											UDS_TX_DATA[2] = 0x6F;
 - 											memcpy(&UDS_TX_DATA[3], &Rx_multiplebytes[3], 13);
 - 										
 - 											Respond_Multiple_Frames(UDS_TX_DATA[1]);
 - 										}
 - 										else
 - 										{
 - 											Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
 - 											Respond_Single_Frame(IOCONTROL);
 - 										}
 - 									}
 - 									else
 - 									{
 - 										Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
 - 										Respond_Single_Frame(IOCONTROL);
 - 									}
 - 								}
 - 								else
 - 								{
 - 									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
 - 									Respond_Single_Frame(IOCONTROL);
 - 								}
 - 							break;
 - 							
 - 							case DID_MESSAGE_ACTIATION_IO_CONTROL_0xF095:
 - 								if(Rx_multiplebytes[1] == 0x22)
 - 								{
 - 									if(Rx_multiplebytes[5] == 0x03)
 - 									{							
 - 										if((Rx_multiplebytes[19] == 0x00) && (Rx_multiplebytes[20] == 0x00) &&
 - 											(Rx_multiplebytes[34] == 0x00) && (Rx_multiplebytes[35] == 0x00))
 - 										{
 - 											memcpy(IO_Control_State_Message_Activation, &Rx_multiplebytes[6], 15);
 - 											memcpy(IO_Control_Enable_Message_Activation, &Rx_multiplebytes[21], 15);
 - 											IO_Parameter_Request_Status = Rx_multiplebytes[5];
 - 											UDS_TX_DATA[0] = 0x10;
 - 											UDS_TX_DATA[1] = 0x11;
 - 											UDS_TX_DATA[2] = 0x6F;
 - 											memcpy(&UDS_TX_DATA[3], &Rx_multiplebytes[3], 18);
 - 										
 - 											Respond_Multiple_Frames(UDS_TX_DATA[1]);
 - 										}
 - 										else
 - 										{
 - 											Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
 - 											Respond_Single_Frame(IOCONTROL);
 - 										}
 - 									}
 - 									else
 - 									{
 - 										Save_NegResponse(IOCONTROL, NRC_SFNC_0x12);
 - 										Respond_Single_Frame(IOCONTROL);
 - 									}
 - 								}
 - 								else
 - 								{
 - 									Save_NegResponse(IOCONTROL, NRC_ICDL_0x13);
 - 									Respond_Single_Frame(IOCONTROL);
 - 								}
 - 							break;*/
							
							default:
								Save_NegResponse(IOCONTROL,NRC_ROFR_0x31);
								Respond_Single_Frame(IOCONTROL);
							break;
						}
					}
					else
					{
						Save_NegResponse(IOCONTROL, NRC_VSTH_0x88);
						Respond_Single_Frame(IOCONTROL);
					}
				}
				else
				{
					Save_NegResponse(IOCONTROL, NRC_EIR_0x83);
					Respond_Single_Frame(IOCONTROL);
				}
			}
			else
			{
				Save_NegResponse(IOCONTROL, NRC_SAD_0x33);
				Respond_Single_Frame(IOCONTROL);
			}
		}
		else
		{
			Save_NegResponse(IOCONTROL, NRC_SNS_0x7F);
			Respond_Single_Frame(IOCONTROL);
		}
    }
}


/**********************************************************************************
## Module:      UDSS.c
## Function:    Security_Level1()
## Description: Security_Level1  functionality.
## Parameter:   
## Return£º     void
**********************************************************************************/
void Security_Level1(void)
{
  
	if(no_of_attempts < 10)
	{
	     if((REQUEST_3SEC == CLEAR) && (REQUEST_5SEC == CLEAR))
	     {
					    
		 if(Security_Access_Success != SET)
		 {
		       random_value(&Seed);
		       write_seed    = SET;
	      	       seed_received = SET;
		       TX_DATA[0] = 0x06;
		       TX_DATA[1] = 0x67;
		       TX_DATA[2] = 0x01;
		       TX_DATA[3] = Seed.Bytes_in_seed[3];
		       TX_DATA[4] = Seed.Bytes_in_seed[2];
		       TX_DATA[5] = Seed.Bytes_in_seed[1];
		       TX_DATA[6] = Seed.Bytes_in_seed[0];
		       TX_DATA[7] = 0x00;
		       Respond_Single_Frame(0x27);	    	
		  }
		  else
		  {
				  TX_DATA[0] = 0x06;
	              TX_DATA[1] = 0x67;
	              TX_DATA[2] = 0x01;
	              TX_DATA[3] = 0x00;
	              TX_DATA[4] = 0x00;
	              TX_DATA[5] = 0x00;
	              TX_DATA[6] = 0x00;
	              TX_DATA[7] = 0x00;
		      Respond_Single_Frame(0x27);		   
	          }
	}
	else
        {
	     Save_NegResponse(SECURITY_ACCESS, NRC_RTDNE_0x37);
	     Respond_Single_Frame(SECURITY_ACCESS);
        }
    }
    else
    {
	Save_NegResponse(SECURITY_ACCESS, NRC_EXCEEDATT_0x36);
	Respond_Single_Frame(SECURITY_ACCESS);
    }    
  
    
}
/**********************************************************************************
## Module:      UDSS.c
## Function:    Security_Level3()
## Description: Security_Level1  functionality.
## Parameter:   
## Return£º     void
**********************************************************************************/
void Security_Level3(void)
{
    if(Functional_Addressing_Flag == CLEAR)
    {
     if(no_of_attempts < 10)
     {
	  if((REQUEST_3SEC == CLEAR) && (REQUEST_5SEC == CLEAR))
	  {

	        if(Security_Access_Level3_Success != SET)
                {
		      random_value_level3(&Seed_level3);
		      write_seed    = SET;
		      seed_received = SET;
		      UDS_TX_DATA[0] = 0x10;
	              UDS_TX_DATA[1] = 0x0A;
		      UDS_TX_DATA[2] = 0x67;
		      UDS_TX_DATA[3] = UDS_Buffer[2];   
		      UDS_TX_DATA[4] =Seed_level3.Bytes_in_seed[7];
		      UDS_TX_DATA[5] =Seed_level3.Bytes_in_seed[6];
		      UDS_TX_DATA[6]=Seed_level3.Bytes_in_seed[5];
		      UDS_TX_DATA[7]=Seed_level3.Bytes_in_seed[4];
		      UDS_TX_DATA[8]=Seed_level3.Bytes_in_seed[3];
		      UDS_TX_DATA[9]=Seed_level3.Bytes_in_seed[2];
		      UDS_TX_DATA[10]=Seed_level3.Bytes_in_seed[1];
		      UDS_TX_DATA[11]=Seed_level3.Bytes_in_seed[0];
		      //memcpy(&UDS_TX_DATA[4], &Seed_level3.Bytes_in_seed[0], 8);
		      Respond_Multiple_Frames(0x0A);
	        }
	        else
		    {
	        	UDS_TX_DATA[0] = 0x10;
	        	UDS_TX_DATA[1] = 0x0A;
	        	UDS_TX_DATA[2] = 0x67;
	        	UDS_TX_DATA[3] = UDS_Buffer[2];
	        	UDS_TX_DATA[4] = 0;
	        	UDS_TX_DATA[5] = 0;
	        	UDS_TX_DATA[6]= 0;
	        	UDS_TX_DATA[7]= 0;
	        	UDS_TX_DATA[8]= 0;
	        	UDS_TX_DATA[9]= 0;
	        	UDS_TX_DATA[10]= 0;
	        	UDS_TX_DATA[11]= 0;
	        	Respond_Multiple_Frames(0x0A);

	        }
	  }
	  else
          {
	       Save_NegResponse(SECURITY_ACCESS, NRC_RTDNE_0x37);
	       Respond_Single_Frame(SECURITY_ACCESS);
          }
    }
    else
    {
	Save_NegResponse(SECURITY_ACCESS, NRC_EXCEEDATT_0x36);
	Respond_Single_Frame(SECURITY_ACCESS);
    }    
    memset(Rx_multiplebytes, 0, sizeof(Rx_multiplebytes));
    }
}
/**********************************************************************************
## Module:      UDSS.c
## Function:    Securitykey_Level3()
## Description: Securitykey_Level3  functionality.
## Parameter:   
## Return£º     void
**********************************************************************************/
void Securitykey_Level3(void)
{
     if((seed_received == SET) && (no_of_attempts < 11))
     {
	  seed_received = CLEAR;
	  received_key3.Bytes_in_seed[0] = Rx_multiplebytes[11];
	  received_key3.Bytes_in_seed[1] = Rx_multiplebytes[10];
	  received_key3.Bytes_in_seed[2] = Rx_multiplebytes[9];
	  received_key3.Bytes_in_seed[3] = Rx_multiplebytes[8];	
	  received_key3.Bytes_in_seed[4] = Rx_multiplebytes[7];
	  received_key3.Bytes_in_seed[5] = Rx_multiplebytes[6];
	  received_key3.Bytes_in_seed[6] = Rx_multiplebytes[5];
	  received_key3.Bytes_in_seed[7] = Rx_multiplebytes[4];
	  generateKeyLevel3(Seed_level3.seed_as_a_whole,&keyvalidation_level3);	
	  
	  if(keyvalidation_level3 == received_key3.seed_as_a_whole) 
	  {			
		no_of_attempts = 0;		
		Security_Access_Level3_Success = SET;
		Security_Access_Success = CLEAR;
		//ACTIVE_DIAG_SESSION = PROGRAMMING_SESSION;
		if((Rx_multiplebytes[1] == 0x0A)&&(Rx_multiplebytes[3] == 0x06))
		{
		     TX_DATA[0] = 0x02;
		     TX_DATA[1] = 0x67;
		     TX_DATA[2] = 0x06;
		     TX_DATA[3] = 0x00;
		     TX_DATA[4] = 0x00;
		     TX_DATA[5] = 0x00;
		     TX_DATA[6] = 0x00;
		     TX_DATA[7] = 0x00;
		     Respond_Single_Frame(SECURITY_ACCESS);
		}
		else
		{
			if(Rx_multiplebytes[1] == 0x0A)
			{
				Save_NegResponse(0x27, 0x12);
				Respond_Single_Frame(SECURITY_ACCESS);
			}
			else
			{
				Save_NegResponse(0x27, 0x13);
				Respond_Single_Frame(SECURITY_ACCESS);
			}
		}
	   }
	   else
	   {
		 no_of_attempts++;
		 if(no_of_attempts < 11)
		 {
		       if(no_of_attempts > 1)
		       {
			     REQUEST_3SEC = SET;	
		       }
		       Save_NegResponse(0x27, NRC_WKEY_0x35);
		       Respond_Single_Frame(SECURITY_ACCESS);
		 }
		  else
		  {
		       no_of_attempts = 12;
		       Save_NegResponse(0x27, NRC_EXCEEDATT_0x36);
		       Respond_Single_Frame(SECURITY_ACCESS);
		  }
	    }
	}
	else
	{	
	     Save_NegResponse(0x27, NRC_WRSQ_0x24);
	     Respond_Single_Frame(SECURITY_ACCESS);
	}
	
}
/**********************************************************************************
## Module:   	UDSS.c
## Function: 	Write_boot_version()
## Description: Write application version to EEPROM.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void Write_boot_version(uint8_t *appAddr)
{
	eeprom_write(APP_SOFT_ID_OFFSET + 0, appAddr[0]);
	eeprom_write(APP_SOFT_ID_OFFSET + 1, appAddr[1]);
	eeprom_write(APP_SOFT_ID_OFFSET + 2, appAddr[2]);
	eeprom_write(APP_SOFT_ID_OFFSET + 3, appAddr[3]);
	eeprom_write(APP_SOFT_ID_OFFSET + 4, appAddr[4]);
	eeprom_write(APP_SOFT_ID_OFFSET + 5, appAddr[5]);
	eeprom_write(APP_SOFT_ID_OFFSET + 6, appAddr[6]);
	eeprom_write(APP_SOFT_ID_OFFSET + 7, appAddr[7]);
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	Read_boot_version()
## Description: Read application version from EEPROM.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void Read_boot_version(void)
{
	Read_Boot_Vrs[0] = eeprom_read(APP_SOFT_ID_OFFSET + 0);
	Read_Boot_Vrs[1] = eeprom_read(APP_SOFT_ID_OFFSET + 1);
	Read_Boot_Vrs[2] = eeprom_read(APP_SOFT_ID_OFFSET + 2);
	Read_Boot_Vrs[3] = eeprom_read(APP_SOFT_ID_OFFSET + 3);
	Read_Boot_Vrs[4] = eeprom_read(APP_SOFT_ID_OFFSET + 4);
	Read_Boot_Vrs[5] = eeprom_read(APP_SOFT_ID_OFFSET + 5);
	Read_Boot_Vrs[6] = eeprom_read(APP_SOFT_ID_OFFSET + 6);
	Read_Boot_Vrs[7] = eeprom_read(APP_SOFT_ID_OFFSET + 7);
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	updateAppVersion()
## Description: Update application version to EEPROM.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void updateAppVersion(void)
{
	char validstring[8];
	unsigned char ret=SET,count;
	memset(Read_Boot_Vrs, 0, sizeof(Read_Boot_Vrs));

	Read_boot_version();

#ifdef COLOR_TFT
	ret=SET;
	for(count = 0; count < 8; count++)
	{
		if(App_Vesion[count] != Read_Boot_Vrs[count])
		{
			ret = CLEAR;
		}
	}

	if(ret == CLEAR )
	{
		Write_boot_version(&App_Vesion[0]);
	}
#endif

#ifdef GRAYSCALE_TFT
#ifdef U350
	ret=SET;
	for(count = 0; count < 8; count++)
	{
		if(App_Vesion_U350[count] != Read_Boot_Vrs[count])
		{
			ret = CLEAR;
		}
	}

	if(ret == CLEAR )
	{
		Write_boot_version(&App_Vesion_U350[0]);
	}
#else
	ret=SET;
	for(count = 0; count < 8; count++)
	{
		if(App_Vesion[count] != Read_Boot_Vrs[count])
		{
			ret = CLEAR;
		}
	}

	if(ret == CLEAR )
	{
		Write_boot_version(&App_Vesion[0]);
	}
#endif
#endif
}


/**********************************************************************************
## Module:   	UDSS.c
## Function: 	Write_boot_version()
## Description: Write application version to EEPROM.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void Write_HW_version(uint8_t *appAddr)
{
	eeprom_write(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 0, appAddr[0]);
	eeprom_write(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 1, appAddr[1]);
	eeprom_write(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 2, appAddr[2]);
	eeprom_write(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 3, appAddr[3]);
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	Read_boot_version()
## Description: Read application version from EEPROM.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void Read_HW_version(void)
{
	Read_HW_Vrs[0] = eeprom_read(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 0);
	Read_HW_Vrs[1] = eeprom_read(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 1);
	Read_HW_Vrs[2] = eeprom_read(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 2);
	Read_HW_Vrs[3] = eeprom_read(SYS_SUP_ECU_HARD_VER_NO_OFFSET + 3);
}

/**********************************************************************************
## Module:   	UDSS.c
## Function: 	updateAppVersion()
## Description: Update application version to EEPROM.
## Parameter:	None
## Return£º  	void
 **********************************************************************************/
void updateHWVersion(void)
{
	char validstring[4];
	unsigned char ret=SET,count;
	memset(Read_Boot_Vrs, 0, sizeof(Read_Boot_Vrs));

	Read_HW_version();

#ifdef COLOR_TFT
	for(count = 0; count < 4; count++)
	{
		if(HWC_Version[count] != Read_Boot_Vrs[count])
		{
			ret = CLEAR;
		}
	}

	if(ret == CLEAR )
	{
		Write_HW_version(&HWC_Version[0]);
	}
#endif

#ifdef GRAYSCALE_TFT
	for(count = 0; count < 4; count++)
	{
		if(HWG_Version[count] != Read_Boot_Vrs[count])
		{
			ret = CLEAR;
		}
	}

	if(ret == CLEAR )
	{
		Write_HW_version(&HWG_Version[0]);
	}
#endif

#ifdef U350
	for(count = 0; count < 4; count++)
	{
		if(HWU_Version[count] != Read_Boot_Vrs[count])
		{
			ret = CLEAR;
		}
	}

	if(ret == CLEAR )
	{
		Write_HW_version(&HWU_Version[0]);
	}
#endif
}
