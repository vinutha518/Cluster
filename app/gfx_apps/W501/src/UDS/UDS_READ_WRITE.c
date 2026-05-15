/*
 * UDS_READ_WRITE.c
 *
 *  Created on: Jan 14, 2019
 *      Author: MSWELT12
 */
/* **********************************************************************************************************************
File            :       UDS_READ_WRITE.c
Author          :       Akshay Wankhade
Model           :       Mahindra W501
Revision Author :
Reviewed By     :
Company         :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

/***********************************************************************************************************************
Includes
 ********************************************************************************************************************** */

//#include "define.h"
#include "r_typedefs.h"
#include "my_can.h"
#include "UDSS.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
/*#include <stdlib.h>*/
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "digitalGuage.h"
#include "tachometer.h"
#include "odometer.h"
#include "r_cg_serial_user.h"


void udss_Readdatabyidentifier(void);
void WriteDataByIdentifier(uint16_t DID);
void WriteDataByIdentifier_multipleframes(void);
void udss_Readdatabyidentifier_MultipleDID(void);
uint8_t DID_0507_maxvalue(void);
uint8_t DID_0510_maxvalue(void);
uint8_t DID_0511_maxvalue(void);
uint8_t DID_0520_maxvalue(void);
uint8_t DID_0522_maxvalue(void);
uint8_t DID_0525_maxvalue(void);
uint8_t DID_050B_maxvalue(void);
uint8_t DID_050C_maxvalue(void);
uint8_t DID_050D_maxvalue(void);
uint8_t DID_050E_maxvalue(void);
uint8_t DID_050F_maxvalue(void);
uint8_t DID_0502_maxvalue(void);
uint8_t DID_04FB_maxvalue(void);
uint8_t DID_04FC_maxvalue(void);
uint8_t DID_04FD_maxvalue(void);
uint8_t DID_04FE_maxvalue(void);
uint8_t DID_04D2_maxvalue(void);
uint8_t DID_04F7_maxvalue(void);
uint8_t DID_0533_maxvalue(void);
uint8_t DID_NM_0103_maxvalue(void);
uint8_t DID_NM_0104_maxvalue(void);
uint8_t DID_NM_010B_maxvalue(void);
uint8_t fuel_value_compare_050B(void);
uint8_t Temp_value_compare_050C(void);
uint8_t Fuel_TT_value_compare_050D(void);
uint8_t Temp_TT_value_compare_050E(void);

uint32_t ODO_dist_in_km = 0;
uint8_t Diag_gear_indc;

extern struct FLAGS EOL;

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
extern uint8_t  DID_AFE_ECUIdentifier_0x0525_arr[ECUIdentifier_0x0525_ARRY_SIZE];
extern uint8_t  DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[ECUIdentifier_0x050F_ARRY_SIZE];
extern uint8_t  DID_DTE_AFE_ECUIdentifier_0x04D2_arr[ECUIdentifier_0x04D2_ARRY_SIZE];
extern uint8_t  DID_FUEL_DEAD_VOLUME_0x04D5_arr[ECUIdentifier_0x04D5_ARRY_SIZE];
extern uint8_t  DID_NVM_SERVICE_DONE_0x04F3_arr[ECUIdentifier_0x04F3_ARRY_SIZE];
extern uint8_t  DID_ODOMETER_RAW_0x04F4_arr[ECUIdentifier_0x04F4_ARRY_SIZE];
extern uint8_t  DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[ECUIdentifier_0x04F6_ARRY_SIZE];
extern uint8_t  DID_OVERSPEED_0x04F7_arr[ECUIdentifier_0x04F7_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_0x04F8_arr[ECUIdentifier_0x04F8_ARRY_SIZE];
extern uint8_t  DID_ILLUMINATION_STATUS_0xF084_arr[ECUIdentifier_0xF084_ARRY_SIZE];
extern uint8_t  DID_BAR_GRAPH_STATUS_0xF083_arr[ECUIdentifier_0xF083_ARRY_SIZE];
extern uint8_t  DID_GEAR_INDICATION_STATUS_0xF085_arr[ECUIdentifier_0xF085_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_STATUS_SET6_0xF096_arr[ECUIdentifier_0xF096_ARRY_SIZE];
extern uint8_t  DID_ODOMETER_0xF1B0_arr[ECUIdentifier_0xF1B0_ARRY_SIZE];
extern uint8_t  DID_ODOMETER_0xF1B0_read_arr[ECUIdentifier_0xF1B0_ARRY_SIZE];
extern uint8_t  DID_VEHICLE_SPEED_0xF1B2_arr[ECUIdentifier_0xF1B2_ARRY_SIZE];
extern uint8_t  DID_ENGINE_SPEED_0xF1B3_arr[ECUIdentifier_0xF1B3_ARRY_SIZE];
extern uint8_t  DID_RESET_REASON_0xF1B4_arr[ECUIdentifier_0xF1B4_ARRY_SIZE];
extern uint8_t  DID_RESET_COUNTER_0xF1B5_arr[ECUIdentifier_0xF1B5_ARRY_SIZE];
extern uint8_t  DID_IGNITION_COUNTER_0xF1B6_arr[ECUIdentifier_0xF1B6_ARRY_SIZE];
extern uint8_t  DID_EEPROM_LAYOUT_0xF1B7_arr[ECUIdentifier_0xF1B7_ARRY_SIZE];
extern uint8_t  DID_AFE_ECU_IDENTIFIER_0x0523_arr[ECUIdentifier_0x0523_ARRY_SIZE];
extern uint8_t  DID_AFE_GD_ECUIdentifier_0x0524_arr[ECUIdentifier_0x0524_ARRY_SIZE];
extern uint8_t  DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[ECUIdentifier_0x04CF_ARRY_SIZE];
extern uint8_t  DID_DTE_ECUIdentifier_0x04D0_arr[ECUIdentifier_0x04D0_ARRY_SIZE];
extern uint8_t  NS_Network_Security_key_SKC_0x0108_arr[SKC_0x0108_arr_ARRY_SIZE];
extern uint8_t  NS_Network_Security_key_SKC1_0x0109_arr[SKC1_0x0109_arr_ARRY_SIZE];
extern uint8_t  NS_Network_Security_key_SKC2_0x010A_arr[SKC2_0x010A_arr_ARRY_SIZE];
extern uint8_t  NS_Network_Security_Configuration_0x010B_arr[NSC_0x010B_arr_ARRY_SIZE];
//ETM
extern uint8_t  DID_ETM_0x0500_arr[ECUIdentifier_0x0500_ARRY_SIZE];
extern uint8_t  fuel_widget,gear_to_HMI,gear_shift_to_HMI,Diag_Ignition_counter;
extern uint16_t AFEDispVal,CalAvgKmplAFE,CalAvgKmplAFE_GD,Batt_ADC_value,reset_counter;
extern uint32_t AccFuelConsp,AccDistAFE,AccFuelConspGD,AccDistAFEGD,DTE_Disp_Val,batt_vtg,speed_steps,tacho_steps,FUEL_LEVEL_FOR_DTE;
extern uint32_t fuel_in_ign_on;

extern uint8_t  U8_Reset_DTE;
extern _Bool Flag_AFE_reset;
/**********************************************************************************
## Module:      UDSS_APP.c
## Function:    udss_Readdatabyidentifier()
## Description: Reads data by indentifier.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void udss_Readdatabyidentifier(void)
{
	if(Functional_Addressing_Flag == CLEAR)
	{
		uint16_t DID = CLEAR;
		DID=(UDS_Buffer[2] << 8)|UDS_Buffer[3];
		switch(DID)
		{

		case DID_UPDATEHISTORY_0x0201:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x80;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = 0x02;
				UDS_TX_DATA[4] = 0x01;
				memcpy(&UDS_TX_DATA[5], &History_zone_Full, 125);
				Respond_Multiple_Frames(0x80);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_VIN_RW_0xF190:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x14;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &VIN_arr, 17);
				Respond_Multiple_Frames(0x14);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}
		case DID_ECU_HARDWARE_VERSION_NUMBER_0xF193:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] =  0x07;
				TX_DATA[1] =  0x62;
				TX_DATA[2] =  UDS_Buffer[2];
				TX_DATA[3] =  UDS_Buffer[3];
				TX_DATA[4] =  Ecu_hardware_version_number_arr[0];
				TX_DATA[5] =  Ecu_hardware_version_number_arr[1];
				TX_DATA[6] =  Ecu_hardware_version_number_arr[2];
				TX_DATA[7] =  Ecu_hardware_version_number_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
		case DID_BOOT_FLAG_0x0200:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = BOOT_FLAG_STATUS;
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;

			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
		}

		case DID_ACTIVE_DIAG_SESSION_0xF186:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = ACTIVE_DIAG_SESSION;
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
		}

		case DID_APP_SOFTWARE_IDENTIFICATION_0xF181:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = 0xF1;
				UDS_TX_DATA[4] = 0x81;
				memcpy(&UDS_TX_DATA[5], &Ecu_software_version_number_arr[0],8);
				Respond_Multiple_Frames(0x0B);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_BOOT_SOFTWARE_IDENTIFICATION_0xF180:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				read_BOOT_SOFTWARE_IDENTIFICATION(&Boot_software_identification_arr[0]);
/*
				for(uint8_t i = 0; i < 8; i++)
				{
					Boot_software_identification_arr[i] = eeprom_read(BOOT_SOFT_ID_OFFSET + i);
				}
*/					memcpy(&UDS_TX_DATA[5], &Boot_software_identification_arr[0], 8);
				Respond_Multiple_Frames(0x0B);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER,NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		case DID_VEH_MANU_SPAREPART_NUMBER_0xF187:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x13;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Sparepart_number_arr[0], 16);
				Respond_Multiple_Frames(0x13);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_SYS_SUPPLIER_IDENTIFIER_0xF18A:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x13;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Supplier_identification_arr[0], 16);
				Respond_Multiple_Frames(0x13);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_ECU_MANUFACTURING_DATE_RW_0xF18B:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x09;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Manufacturing_date_arr[0], 6);
				Respond_Multiple_Frames(0x09);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_ECU_SERIAL_NUMBER_0xF18C:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Ecu_serial_number_arr[0], 8);
				Respond_Multiple_Frames(0x0B);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_ENGINE_TYPE_RW_0xF197:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x13;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Engine_type_arr[0], 16);
				Respond_Multiple_Frames(0x13);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_TESTER_SERIAL_NUMBER_RW_0xF198:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x13;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &tester_serial_number_arr[0], 16);
				Respond_Multiple_Frames(0x13);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_PROGRAMMING_DATE_RW_0xF199:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x09;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Programming_date_arr[0], 6);
				Respond_Multiple_Frames(0x09);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_ECU_INSTALLATION_DATE_RW_0xF19D:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x09;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Ecu_installation_date_arr[0], 6);
				Respond_Multiple_Frames(0x09);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_MODEL_NUMBER_RW_0x0100:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x15;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Model_number_arr[0], 18);
				Respond_Multiple_Frames(0x17);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case NS_NSC_0x010B:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0E;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &NS_Network_Security_Configuration_0x010B_arr[0],11);
				Respond_Multiple_Frames(0x10);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_VARIANT_CODE_RW_0x0101:
		{
			if(ext_session_active == SET)
			{
				if(Security_Access_Success == SET)
				{
					if(UDS_Buffer[0] == 0x03)
					{
						UDS_TX_DATA[0] = 0x10;
						UDS_TX_DATA[1] = 0x17;
						UDS_TX_DATA[2] = 0x62;
						UDS_TX_DATA[3] = UDS_Buffer[2];
						UDS_TX_DATA[4] = UDS_Buffer[3];
						memcpy(&UDS_TX_DATA[5], &Varient_code_arr[0], 20);
						Respond_Multiple_Frames(0x19);
					}
					else
					{
						Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
						Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
					}
				}
				else
				{
					Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SAD_0x33);
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SNS_0x7F);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case DID_FEATURE_CODE_RW_0x0102:
		{
			if(ext_session_active == SET)
			{
				if(Security_Access_Success == SET)
				{
					if(UDS_Buffer[0] == 0x03)
					{
						UDS_TX_DATA[0] = 0x10;
						UDS_TX_DATA[1] = 0x17;
						UDS_TX_DATA[2] = 0x62;
						UDS_TX_DATA[3] = UDS_Buffer[2];
						UDS_TX_DATA[4] = UDS_Buffer[3];
						// memcpy(&TX_DATA[4], &Feature_code_arr[0], 3);
						memcpy(&UDS_TX_DATA[5], &Feature_code_arr[0], 20);
						Respond_Multiple_Frames(0x19);
					}
					else
					{
						Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
						Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
					}
				}
				else
				{
					Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SAD_0x33);
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SNS_0x7F);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case NS_VARIANT_CODE_RW_0x0107:
		{
			if(ext_session_active == SET)
			{
				if(Security_Access_Success == SET)
				{
					if(UDS_Buffer[0] == 0x03)
					{
						UDS_TX_DATA[0] = 0x10;
						UDS_TX_DATA[1] = 0x17;
						UDS_TX_DATA[2] = 0x62;
						UDS_TX_DATA[3] = UDS_Buffer[2];
						UDS_TX_DATA[4] = UDS_Buffer[3];
						memcpy(&UDS_TX_DATA[5], &NS_Varient_code_arr[0], 20);
						Respond_Multiple_Frames(0x19);
					}
					else
					{
						Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
						Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
					}
				}
				else
				{
					Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SAD_0x33);
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SNS_0x7F);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		case NS_Network_Security_key_SKC_0x0108:

			if(ext_session_active == SET)
			{
				if(Security_Access_Success == SET)
				{
					if(UDS_Buffer[0] == 0x03)
					{
						TX_DATA[0] = 0x07;
						TX_DATA[1] = 0x62;
						TX_DATA[2] = UDS_Buffer[2];
						TX_DATA[3] = UDS_Buffer[3];
						TX_DATA[4] = NS_Network_Security_key_SKC_0x0108_arr[0];
						TX_DATA[5] = NS_Network_Security_key_SKC_0x0108_arr[1];
						TX_DATA[6] = NS_Network_Security_key_SKC_0x0108_arr[2];
						TX_DATA[7] = NS_Network_Security_key_SKC_0x0108_arr[3];

					}
					else
					{
						Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
					}
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
				else
				{
					Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SAD_0x33);
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SNS_0x7F);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case NS_Network_Security_key_SKC1_0x0109:
			if(ext_session_active == SET)
			{
				if(Security_Access_Success == SET)
				{
					if(UDS_Buffer[0] == 0x03)
					{
						TX_DATA[0] = 0x07;
						TX_DATA[1] = 0x62;
						TX_DATA[2] = UDS_Buffer[2];
						TX_DATA[3] = UDS_Buffer[3];
						TX_DATA[4] = NS_Network_Security_key_SKC1_0x0109_arr[0];
						TX_DATA[5] = NS_Network_Security_key_SKC1_0x0109_arr[1];
						TX_DATA[6] = NS_Network_Security_key_SKC1_0x0109_arr[2];
						TX_DATA[7] = NS_Network_Security_key_SKC1_0x0109_arr[3];

					}
					else
					{
						Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
					}
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
				else
				{
					Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SAD_0x33);
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SNS_0x7F);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case NS_Network_Security_key_SKC2_0x010A:
			if(ext_session_active == SET)
			{
				if(Security_Access_Success == SET)
				{
					if(UDS_Buffer[0] == 0x03)
					{
						TX_DATA[0] = 0x07;
						TX_DATA[1] = 0x62;
						TX_DATA[2] = UDS_Buffer[2];
						TX_DATA[3] = UDS_Buffer[3];
						TX_DATA[4] = NS_Network_Security_key_SKC2_0x010A_arr[0];
						TX_DATA[5] = NS_Network_Security_key_SKC2_0x010A_arr[1];
						TX_DATA[6] = NS_Network_Security_key_SKC2_0x010A_arr[2];
						TX_DATA[7] = NS_Network_Security_key_SKC2_0x010A_arr[3];

					}
					else
					{
						Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
					}
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
				else
				{
					Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SAD_0x33);
					Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
				}
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_SNS_0x7F);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		case DID_GRAPHIC_VERSION_0xF1AF:
		{
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &Graphics_version_read[0],8);
				Respond_Multiple_Frames(0x0D);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
		}

		/********************************** ECU Indentifier W501***********************************/

		case DID_FUEL_BAR_VALUE_CONFIG_0x050B:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x13;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0],16);
				Respond_Multiple_Frames(0x13);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_TEMP_BAR_VALUE_CONFIG_0x050C:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10; /*FF with data length 259byte */
				UDS_TX_DATA[1] = 0x13;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0],16);
				Respond_Multiple_Frames(0x13);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_FUEL_TT_VALUE_CONFIG_0x050D:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0];
				TX_DATA[5] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[1];
				TX_DATA[6] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[2];
				TX_DATA[7] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_TEMP_TT_VALUE_CONFIG_0x050E:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0];
				TX_DATA[5] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[1];
				TX_DATA[6] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[2];
				TX_DATA[7] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[3];

			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_CHIME_CONFIG_0x0503:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0D;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_CHIMES_CONFIG_0x0503_arr[0],10);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_MESSAGE_CONFIG_0x0504:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x12;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_MESSAGE_CONFIG_0x0504_arr[0],15);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_SEAT_BELT_CONFIG_0x0507:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0A;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_SEAT_BELT_REMAINDER_0x0507_arr[0],7);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_TDMS_CONFIG_0x0510:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x08;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_TIRE_DIRE_MNGT_0x0510_arr[0],5);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_IFE_CONFIG_0x0520:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x08;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_IFE_ECUIdentifier_0x0520_arr[0],5);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
#if 0
		case DID_OSEK_NM_CONFIG_0x0521:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_OSEK_NM_0x0521_arr[0],8);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
#endif

		case DID_RPAS_CONFIG_0x0522:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0],8);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_AFE2_CONFIG_0x0525:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0C;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_AFE_ECUIdentifier_0x0525_arr[0],9);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_SERVICE_SLAB_CONFIG_0x050F:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0C;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0],9);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_AFE_DTE_CONFIG_0x04D2:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0],8);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_OVERSPEED_CONFIG_0x04F7:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0A;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_OVERSPEED_0x04F7_arr[0],7);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_ILLUMINATION_STATUS_CONFIG_0xF084:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x09;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_ILLUMINATION_STATUS_0xF084_arr[0],6);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
#if 0
		case DID_RESET_REASON_CONFIG_0xF1B4:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x08;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				for(uint8_t i=0;i<reset_counter;i++)
				{
					UDS_TX_DATA[5+i]=0x02;
					if(i == 4)
					{
						i = reset_counter;
					}
				}
				//memcpy (&UDS_TX_DATA[5], &DID_RESET_REASON_0xF1B4_arr[0],5);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;
#endif
		case DID_EEPROM_LAYOUT_CONFIG_0xF1B7:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0B;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_EEPROM_LAYOUT_0xF1B7_arr[0],8);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_AFE1_CONFIG_0x0523:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0D;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				UDS_TX_DATA[5] = (AFEDispVal>>8)&(0xFF);
				UDS_TX_DATA[6] = (AFEDispVal)&(0xFF);
				UDS_TX_DATA[7] = (CalAvgKmplAFE>>8)&(0xFF);
				UDS_TX_DATA[8] = (CalAvgKmplAFE)&(0xFF);
				UDS_TX_DATA[9] = (AccFuelConsp>>24)&(0xFF);
				UDS_TX_DATA[10] =(AccFuelConsp>>16)&(0xFF);
				UDS_TX_DATA[11] =(AccFuelConsp>>8)&(0xFF);
				UDS_TX_DATA[12] =(AccFuelConsp)&(0xFF);
				UDS_TX_DATA[13] =(AccDistAFE>>8)&(0xFF);
				UDS_TX_DATA[14] =(AccDistAFE)&(0xFF);
				//memcpy (&UDS_TX_DATA[5], &DID_AFE_ECU_IDENTIFIER_0x0523_arr[0],10);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		case DID_AFE_GD_CONFIG_0x0524://IPT

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x0D;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				UDS_TX_DATA[5] = (AFEDispVal>>8)&(0xFF);
				UDS_TX_DATA[6] = (AFEDispVal)&(0xFF);
				UDS_TX_DATA[7] = (CalAvgKmplAFE_GD>>8)&(0xFF);
				UDS_TX_DATA[8] = (CalAvgKmplAFE_GD)&(0xFF);
				UDS_TX_DATA[9] = (AccFuelConspGD>>24)&(0xFF);
				UDS_TX_DATA[10] =(AccFuelConspGD>>16)&(0xFF);
				UDS_TX_DATA[11] =(AccFuelConspGD>>8)&(0xFF);
				UDS_TX_DATA[12] =(AccFuelConspGD)&(0xFF);
				UDS_TX_DATA[13] =(AccDistAFEGD>>8)&(0xFF);
				UDS_TX_DATA[14] =(AccDistAFEGD)&(0xFF);
				//memcpy (&UDS_TX_DATA[5], &DID_AFE_GD_ECUIdentifier_0x0524_arr[0],10);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

		/* case DID_TELLTALE_FB_CONFIG_0x04CF:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x1D;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy (&UDS_TX_DATA[5], &DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[0],26);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break; */

		case DID_DTE_CONFIG_0x04D0:

			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x09;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				UDS_TX_DATA[5] = ((AFEDispVal*10)-70);
				UDS_TX_DATA[6] = (AccFuelConsp>>8) & (0XFF);
				UDS_TX_DATA[7] = AccFuelConsp & (0XFF);
				UDS_TX_DATA[8] = AccDistAFE & 0xFF;
				UDS_TX_DATA[9] = (DTE_Disp_Val>>8) & (0XFF);//IPT
				UDS_TX_DATA[10] = (DTE_Disp_Val) & (0XFF);
				//memcpy (&UDS_TX_DATA[5], &DID_DTE_ECUIdentifier_0x04D0_arr[0],6);
				Respond_Multiple_Frames(UDS_TX_DATA[1]+2);
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
			break;

#if 0
			//READ SINGLE FRAME
		case DID_RPM_CHIME_CONFIG_0x0508:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_CHIME_ACTIVATION_0x0508_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
#endif
		case DID_SPEED_CHIME_CONFIG_0x0509:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_CHIME_ACTIVATION_0x0509_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;

			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_AVG_SPEED_CONFIG_0x0511:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_AVG_SPEED_0x0511_arr[0];
				TX_DATA[5] = DID_AVG_SPEED_0x0511_arr[1];
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_FUEL_DEAD_VOLUME_CONFIG_0x04D5:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_FUEL_DEAD_VOLUME_0x04D5_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_NVM_SERVICE_DONE_CONFIG_0x04F3:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_NVM_SERVICE_DONE_0x04F3_arr[0];
				TX_DATA[5] = 0x00;
				TX_DATA[6] = 0x00;
				TX_DATA[7] = 0x00;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_ODOMETER_RAW_CONFIG_0x04F4:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_ODOMETER_RAW_0x04F4_arr[0];
				TX_DATA[5] = 0x00;
				TX_DATA[6] = 0x00;
				TX_DATA[7] = 0x00;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_TPMS_TELLTALE_CONFIG_0x04F8:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_0x04F8_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
//ETM
		case DID_ETM_CONFIG_0x0500:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_ETM_0x0500_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_BAR_GRAPH_STATUS_CONFIG_0xF083:

			if(UDS_Buffer[0] == 0x03)
			{

				DID_BAR_GRAPH_STATUS_0xF083_arr[0]=((temp_widget&(0x0f))|((fuel_widget & 0x0f)<<4));//ipt
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_BAR_GRAPH_STATUS_0xF083_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_GEAR_INDICATION_STATUS_CONFIG_0xF085:

			if(UDS_Buffer[0] == 0x03)
			{

				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = (Diag_gear_indc)&0x0f;
				if(gear_shift_to_HMI == 0)
				{
					TX_DATA[5] = (0x01);//up
				}
				else if(gear_shift_to_HMI == 1)
				{
					TX_DATA[5] = (0x02);//down
				}
				else
				{
					TX_DATA[5] = 0;
				}
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_TELL_TALE_STATUS_SET6_CONFIG_0xF096:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_STATUS_SET6_0xF096_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_ODOMETER_CONFIG_0xF1B0:	//Reads data by indentifier.

			if(UDS_Buffer[0] == 0x03)
			{
			        ODO_dist_in_km = final_odomter_distance / 1000;
				TX_DATA[0] = 0x06;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = ((ODO_dist_in_km >> 16) & 0xFF);
				TX_DATA[5] = ((ODO_dist_in_km >> 8) & 0xFF);
				TX_DATA[6] = (ODO_dist_in_km & 0xFF);
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_VEHICLE_SPEED_CONFIG_0xF1B2:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				if(FEATURE_CODE_SPEED_INPUT == 0)
				{
				    TX_DATA[4] = CAN_ESC2_10[1].byte;//IPT
				    TX_DATA[5] = CAN_ESC2_10[0].byte;
				}
				else
				{
				    TX_DATA[4] = CAN_ESC12_10[1].byte;//IPT
				    TX_DATA[5] = CAN_ESC12_10[0].byte;
				}
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_ENGINE_SPEED_CONFIG_0xF1B3:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = CAN_EMS1_10[2].byte;//IPT
				TX_DATA[5] = CAN_EMS1_10[1].byte;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
#if 0
		case DID_RESET_COUNTER_CONFIG_0xF1B5:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = (reset_counter>>8)&(0xFF);
				TX_DATA[5] = (reset_counter)&(0xFF);
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
#endif
		case DID_IGNITION_COUNTER_CONFIG_0xF1B6:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = ((Diag_Ignition_counter>>8)&0xFF);//ipt
				TX_DATA[5] = (Diag_Ignition_counter)&(0xFF);
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_BATTERY_VOLTAGE_CONFIG_0xF1B1:

			if(UDS_Buffer[0] == 0x03)
			{
				batt_vtg=(Batt_ADC_value*100)/204;
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = ((batt_vtg>>8)&(0xFF));
				TX_DATA[5] = (batt_vtg)&(0xFF);//ipt
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_GAUGES_CONFIG_0xF082:

			if(UDS_Buffer[0] == 0x03)
			{
				uint16_t speed_angle,tacho_angle,tacho_redline;
				tacho_angle = (((tacho_steps/16612)*200)+200);
				speed_angle = (((speed_steps/16612)*200)+200);

				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x08;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				UDS_TX_DATA[5] = (speed_angle>>8)&0xff;//ipt
				UDS_TX_DATA[6] = (speed_angle)&0xff;
				UDS_TX_DATA[7] = (tacho_angle>>8)&0xff;
				UDS_TX_DATA[8] = (tacho_angle)&0xff;
				UDS_TX_DATA[9] = 0;
				//memcpy(&UDS_TX_DATA[5], &DID_GAUGES_0xF082_arr[0], 5);
				Respond_Multiple_Frames((UDS_TX_DATA[1] + 2));
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}

			break;

		case DID_TELL_TALE_STATUS_SET5_CONFIG_0xF08F:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_STATUS_SET5_0xF08F_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_TELL_TALE_STATUS_SET4_CONFIG_0xF08E:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_STATUS_SET4_0xF08E_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;


		case DID_TELL_TALE_STATUS_SET3_CONFIG_0xF081:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_STATUS_SET3_0xF081_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_TELL_TALE_STATUS_SET2_CONFIG_0xF080:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_STATUS_SET2_0xF080_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_TELL_TALE_STATUS_SET1_CONFIG_0xF07F:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_FUEL_GEAN_INDICATOR_CONFIG_0x04D1:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x06;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = (FUEL_LEVEL_FOR_DTE>>8)&0xFF;//IPT
				TX_DATA[5] = (FUEL_LEVEL_FOR_DTE)&0xFF;
				TX_DATA[6] = gear_to_HMI;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_DID_ODOMETER_PERCENTAGE_CONFIG_0x04F6:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0];
				TX_DATA[5] = 0;
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;

		case DID_FUEL_CONS_ACCM_CONFIG_0x04F9:

			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = ((fuel_in_ign_on-FUEL_LEVEL_FOR_DTE)>>8)&(0XFF);
				TX_DATA[5] = (fuel_in_ign_on-FUEL_LEVEL_FOR_DTE)&(0XFF);
				TX_DATA[6] = 0;
				TX_DATA[7] = 0;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
		
		case DID_TT_BLINK_PATTERN3_0x0502:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0];
				TX_DATA[5] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[1];
				TX_DATA[6] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[2];
				TX_DATA[7] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		case DID_TT_BLINK_PATTERN4_0x04FB:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0];
				TX_DATA[5] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[1];
				TX_DATA[6] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[2];
				TX_DATA[7] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		case DID_TT_BLINK_PATTERN5_0x04FC:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0];
				TX_DATA[5] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[1];
				TX_DATA[6] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[2];
				TX_DATA[7] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		case DID_TT_BLINK_PATTERN8_0x04FD:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0];
				TX_DATA[5] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[1];
				TX_DATA[6] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[2];
				TX_DATA[7] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		case DID_TT_BLINK_PATTERN9_0x04FE:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x07;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0];
				TX_DATA[5] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[1];
				TX_DATA[6] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[2];
				TX_DATA[7] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[3];
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		case DID_ROLL_PITCH_DETECTION_0x0533:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_Roll_Pitch_Detection_0x0533_arr[0];
				TX_DATA[5] = DID_Roll_Pitch_Detection_0x0533_arr[1];
				TX_DATA[6] = 0x00;
				TX_DATA[7] = 0x00;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
#ifdef SPRINT4		
		case DID_AT_GEAR_ALERT_0x0543:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = DID_AT_GEAR_ALERT_0x0543_arr[0];
				TX_DATA[5] = DID_AT_GEAR_ALERT_0x0543_arr[1];
				TX_DATA[6] = 0x00;
				TX_DATA[7] = 0x00;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
#endif		
		case NS_Network_Mgmt_Config_0x0103:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x15;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &NS_NM_Configuration_0x0103_arr[0], 18);
				Respond_Multiple_Frames((UDS_TX_DATA[1] + 2));
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
		break;
		
		case NS_CAN_Wake_Up_Config_0x0104:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x08;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &NS_Can_Wakeup_Configuration_0x0104_arr[0], 5);
				Respond_Multiple_Frames((UDS_TX_DATA[1] + 2));
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
		break;
		
		case NS_Network_Security_Fault_MSG_Detail_0x0105:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x21;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &NS_Fault_Msg_Details_0x0105_arr[0], 30);
				Respond_Multiple_Frames((UDS_TX_DATA[1] + 2));
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
		break;
		
		case NS_Network_Security_Fault_Type_STS_Info_0x0106:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x04;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = NS_Fault_Type_Status_Info_0x0106_arr[0];
				TX_DATA[5] = 0x00;
				TX_DATA[6] = 0x00;
				TX_DATA[7] = 0x00;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		case NS_CAN_Wake_Up_Config_PKE_0x010D:
			if(UDS_Buffer[0] == 0x03)
			{
				UDS_TX_DATA[0] = 0x10;
				UDS_TX_DATA[1] = 0x14;
				UDS_TX_DATA[2] = 0x62;
				UDS_TX_DATA[3] = UDS_Buffer[2];
				UDS_TX_DATA[4] = UDS_Buffer[3];
				memcpy(&UDS_TX_DATA[5], &NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0], 17);
				Respond_Multiple_Frames((UDS_TX_DATA[1] + 2));
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
				Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			}
		break;
		
		case NS_Network_MSG_Config_0x010E:
			if(UDS_Buffer[0] == 0x03)
			{
				TX_DATA[0] = 0x05;
				TX_DATA[1] = 0x62;
				TX_DATA[2] = UDS_Buffer[2];
				TX_DATA[3] = UDS_Buffer[3];
				TX_DATA[4] = NS_NW_Msg_config_0x010E_arr[0];
				TX_DATA[5] = NS_NW_Msg_config_0x010E_arr[1];
				TX_DATA[6] = 0x00;
				TX_DATA[7] = 0x00;
			}
			else
			{
				Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			}
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		break;
		
		default:
			Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ROFR_0x31);
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
			break;
		}
	}
}
/* **********************************************************************************************
## Module:      UDSS_APP.c
## Function:    WriteDataByIdentifier()
## Description: Write data by identifier.
## Parameter:   None
## Return£º     void
 *********************************************************************************************** */
PUBLIC void WriteDataByIdentifier(uint16_t DID)
{
	if(Functional_Addressing_Flag == CLEAR)
	{
		if(ext_session_active == SET)
		{
			if((Security_Access_Success == SET))// && (ext_session_active == SET))
			{
				if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
				{
					if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
					{
						if(UDS_Buffer[0] >= 0x04)
						{
						DID=(UDS_Buffer[2] << 8)|UDS_Buffer[3];
						switch(DID)
						{

						case DID_FUEL_TT_VALUE_CONFIG_0x050D:
						{
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_050D_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0], &UDS_Buffer[4],4);
									EOL.F_FUEL_TT_VALUE_CONFIG = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						}

						case DID_TEMP_TT_VALUE_CONFIG_0x050E:
						{
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_050E_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0], &UDS_Buffer[4],4);
									EOL.F_TEMP_TT_VALUE_CONFIG = SET;
									Write_UDS_Data();
								}
								else
								{
									Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
								}
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						}

						case NS_Network_Security_key_SKC_0x0108:
						{
							if(UDS_Buffer[0] == 0x07)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_Network_Security_key_SKC_0x0108_arr[0], &UDS_Buffer[4],4);
								EOL.F_NS_SKC_0108_CONFIG = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						}

						case NS_Network_Security_key_SKC1_0x0109:
						{
							if(UDS_Buffer[0] == 0x07)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_Network_Security_key_SKC1_0x0109_arr[0], &UDS_Buffer[4],4);
								EOL.F_NS_SKC1_0109_CONFIG = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						}

						case NS_Network_Security_key_SKC2_0x010A:
						{
							if(UDS_Buffer[0] == 0x07)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_Network_Security_key_SKC2_0x010A_arr[0], &UDS_Buffer[4],4);
								EOL.F_NS_SKC2_010A_CONFIG = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						}
#if 0
						case DID_RPM_CHIME_CONFIG_0x0508:
						{
							if(UDS_Buffer[0] == 0x04)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_CHIME_ACTIVATION_0x0508_arr[0], &UDS_Buffer[4],1);
								EOL.F_CHIME_ACTIVATION_0x0508 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}
#endif
						case DID_SPEED_CHIME_CONFIG_0x0509:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if((UDS_Buffer[4] > 0) && (UDS_Buffer[4] < 21))
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_CHIME_ACTIVATION_0x0509_arr[0], &UDS_Buffer[4],1);
								EOL.F_CHIME_ACTIVATION_0x0509 = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}


						case DID_AVG_SPEED_CONFIG_0x0511:
						{
							if(UDS_Buffer[0] == 0x05)
							{
							    if(DID_0511_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_AVG_SPEED_0x0511_arr[0], &UDS_Buffer[4],2);
									EOL.F_AVG_SPEED = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						}

						case DID_FUEL_DEAD_VOLUME_CONFIG_0x04D5:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if((UDS_Buffer[4] > 0) && (UDS_Buffer[4] < 101))
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_FUEL_DEAD_VOLUME_0x04D5_arr[0], &UDS_Buffer[4],1);
								EOL.F_FUEL_DEAD_VOLUME = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_NVM_SERVICE_DONE_CONFIG_0x04F3:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if(UDS_Buffer[4] < 2)
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_NVM_SERVICE_DONE_0x04F3_arr[0], &UDS_Buffer[4],1);
								EOL.F_NVM_SERVICE_DONE = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_ODOMETER_RAW_CONFIG_0x04F4:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if(UDS_Buffer[4] < 2)
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_ODOMETER_RAW_0x04F4_arr[0], &UDS_Buffer[4],1);
								EOL.F_ODOMETER_RAW = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_DID_ODOMETER_PERCENTAGE_CONFIG_0x04F6:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if((UDS_Buffer[4] > 1) && (UDS_Buffer[4] < 26))
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0], &UDS_Buffer[4],1);
								EOL.F_ODOMETER_PERCENTAGE = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_TPMS_TELLTALE_CONFIG_0x04F8:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if((UDS_Buffer[4] > 4) && (UDS_Buffer[4] < 121))
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_TELL_TALE_0x04F8_arr[0], &UDS_Buffer[4],1);
								EOL.F_TELL_TALE = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}
	//ETM
						case DID_ETM_CONFIG_0x0500:
						{
							if(UDS_Buffer[0] == 0x04)
							{
							    if(UDS_Buffer[4] < 2)
							    {
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_ETM_0x0500_arr[0], &UDS_Buffer[4],1);
								EOL.F_ETM_CONFIG = SET;
								Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

	#if 0
						case DID_BAR_GRAPH_STATUS_CONFIG_0xF083:
						{
							if(UDS_Buffer[0] == 0x04)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_BAR_GRAPH_STATUS_0xF083_arr[0], &UDS_Buffer[4],1);
								EOL.F_BAR_GRAPH_STATUS_CONFIG_0xF083 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_GEAR_INDICATION_STATUS_CONFIG_0xF085:
						{
							if(UDS_Buffer[0] == 0x05)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_GEAR_INDICATION_STATUS_0xF085_arr[0], &UDS_Buffer[4],2);
								EOL.F_GEAR_INDICATION_STATUS_CONFIG_0xF085 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_TELL_TALE_STATUS_SET6_CONFIG_0xF096:
						{
							if(UDS_Buffer[0] == 0x04)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_TELL_TALE_STATUS_SET6_0xF096_arr[0], &UDS_Buffer[4],1);
								EOL.F_TELL_TALE_STATUS_SET6_CONFIG_0xF096 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}
	#endif
						case DID_ODOMETER_CONFIG_0xF1B0:		//Write data by identifier.
						{
							memcpy(&DID_ODOMETER_0xF1B0_arr[0], &UDS_Buffer[4],3);	//Copy UDS_Buffer to DID_ODOMETER_0xF1B0_arr

							if(UDS_Buffer[0] == 0x06)
							{
								DID_ODO_Update();
								if(invalid_ODO_input == 0)
								{
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_ODOMETER_0xF1B0_arr[0], &UDS_Buffer[4],3);
									EOL.F_ODOMETER_CONFIG_0xF1B0 = SET;
									Write_UDS_Data();
								}
								else
								{
									Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);		//invalid input
								}
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);		//Invalid Data length
							}


							break;
						}
	#if 0
						case DID_VEHICLE_SPEED_CONFIG_0xF1B2:
						{
							if(UDS_Buffer[0] == 0x05)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_VEHICLE_SPEED_0xF1B2_arr[0], &UDS_Buffer[4],2);
								EOL.F_VEHICLE_SPEED_CONFIG_0xF1B2 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_ENGINE_SPEED_CONFIG_0xF1B3:
						{
							if(UDS_Buffer[0] == 0x05)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_ENGINE_SPEED_0xF1B3_arr[0], &UDS_Buffer[4],2);
								EOL.F_ENGINE_SPEED_CONFIG_0xF1B3 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_RESET_COUNTER_CONFIG_0xF1B5:
						{
							if(UDS_Buffer[0] == 0x05)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_RESET_COUNTER_0xF1B5_arr[0], &UDS_Buffer[4],2);
								EOL.F_RESET_COUNTER_CONFIG_0xF1B5 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}

						case DID_IGNITION_COUNTER_CONFIG_0xF1B6:
						{
							if(UDS_Buffer[0] == 0x05)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_IGNITION_COUNTER_0xF1B6_arr[0], &UDS_Buffer[4],2);
								EOL.F_IGNITION_COUNTER_CONFIG_0xF1B6 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}

							break;
						}
	#endif
						case DID_TT_BLINK_PATTERN3_0x0502:
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_0502_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0], &UDS_Buffer[4],4);
									EOL.F_TELL_TALE_BLINK_PAT3 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
					
						break;
					
						case DID_TT_BLINK_PATTERN4_0x04FB:
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_04FB_maxvalue() == CLEAR)
							    {
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0], &UDS_Buffer[4],4);
									EOL.F_TELL_TALE_BLINK_PAT4 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
						break;
					
						case DID_TT_BLINK_PATTERN5_0x04FC:
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_04FC_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0], &UDS_Buffer[4],4);
									EOL.F_TELL_TALE_BLINK_PAT5 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31 );
							    }						
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
						break;
					
						case DID_TT_BLINK_PATTERN8_0x04FD:
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_04FD_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0], &UDS_Buffer[4],4);
									EOL.F_TELL_TALE_BLINK_PAT8 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }						
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}				
						break;
						
						case DID_TT_BLINK_PATTERN9_0x04FE:
							if(UDS_Buffer[0] == 0x07)
							{
							    if(DID_04FE_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0], &UDS_Buffer[4],4);
									EOL.F_TELL_TALE_BLINK_PAT9 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }						
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}	
							break;
						case DID_ROLL_PITCH_DETECTION_0x0533:
							if(UDS_Buffer[0] == 0x05)
							{
							    if(DID_0533_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_Roll_Pitch_Detection_0x0533_arr[0], &UDS_Buffer[4],2);
									EOL.F_ROLL_PITCH_DET_0x0533 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }						
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}	
							break;
#ifdef SPRINT4							
						case DID_AT_GEAR_ALERT_0x0543:
							if(UDS_Buffer[0] == 0x05)
							{
							    if(DID_0533_maxvalue() == CLEAR)
							    {
								
									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = UDS_Buffer[2];
									TX_DATA[3] = UDS_Buffer[3];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_AT_GEAR_ALERT_0x0543_arr[0], &UDS_Buffer[4],2);
									EOL.F_DID_AT_GEAR_ALERT_0x0543 = SET;
									Write_UDS_Data();
							    }
							    else
							    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							    }						
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}	
							break;
#endif							
						case NS_Network_MSG_Config_0x010E:
							if(UDS_Buffer[0] == 0x05)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = UDS_Buffer[2];
								TX_DATA[3] = UDS_Buffer[3];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_NW_Msg_config_0x010E_arr[0], &UDS_Buffer[4],2);
								EOL.F_NS_Network_MSG_Config_0x010E = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
							}
							break;
						default:
							Save_NegResponse(WRITE_DATA_ID, NRC_ROFR_0x31);
							break;
						}
						}
						else
						{
							Save_NegResponse(0x2E, NRC_ICDL_0x13);
						}						
					}
					else
					{
						Save_NegResponse(0x2E, NRC_VSTH_0x88);
					}
				}
				else
				{
					Save_NegResponse(0x2E, NRC_EIR_0x83);
				}
			}
			else
			{
				Save_NegResponse(0x2E, NRC_SAD_0x33);
			}
		}
		else
		{
			Save_NegResponse(0x2E,NRC_SNS_0x7F);
		}
		Respond_Single_Frame(0x2E);
	}
}

/* **********************************************************************************************
## Module:      UDSS_APP.c
## Function:    WriteDataByIdentifier_multipleframes()
## Description: Write multiple frames by identifier.
## Parameter:   None
## Return£º     void
 *********************************************************************************************** */
PUBLIC void WriteDataByIdentifier_multipleframes()
{
	uint16_t DID = CLEAR;
    if(Functional_Addressing_Flag == CLEAR)
    {
	if(ext_session_active == SET)
	{
		if((Security_Access_Success == SET))//&& (ext_session_active == SET))
		{
			if((final_tachometer_speed == 0)||(final_tachometer_speed == 65535))
			{
				if((final_vechile_speed == 0)||(final_vechile_speed == 65535))
				{
					DID = ((Rx_multiplebytes[3] << 8) | Rx_multiplebytes[4]); // byte no 6 to +25

					switch(DID)
					{

					case DID_VIN_RW_0xF190:
					{
						if(Rx_multiplebytes[1] == 0x14)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = 0xF1;
							TX_DATA[3] = 0x90;
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&VIN_arr[0], &Rx_multiplebytes[5], 17);
							EOL.F_VIN = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse(WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(0x2E);
						break;
					}

					case DID_PROGRAMMING_DATE_RW_0xF199:
					{
						if(Rx_multiplebytes[1] == 0x09)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&Programming_date_arr[0], &Rx_multiplebytes[5], 6);
							EOL.F_Programming_date = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_MODEL_NUMBER_RW_0x0100:
					{
						if(Rx_multiplebytes[1] == 0x15)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&Model_number_arr[0], &Rx_multiplebytes[5], 18);
							EOL.F_Model_number = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_ECU_INSTALLATION_DATE_RW_0xF19D:
					{
						if(Rx_multiplebytes[1] == 0x09)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&Ecu_installation_date_arr[0], &Rx_multiplebytes[5], 6);
							EOL.F_Ecu_installation_date = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case NS_NSC_0x010B:
					{
						if(Rx_multiplebytes[1] == 0x0E)
						{
							if((Rx_multiplebytes[5] > 0) && (Rx_multiplebytes[12] > 0) && (Rx_multiplebytes[14] > 0))
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_Network_Security_Configuration_0x010B_arr[0], &Rx_multiplebytes[5], 11);
								EOL.F_NS_NSConfiguration_0x010B = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
							}
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_ENGINE_TYPE_RW_0xF197:
					{
						if(Rx_multiplebytes[1] == 0x13)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&Engine_type_arr[0], &Rx_multiplebytes[5], 16);
							EOL.F_Engine_type = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_TESTER_SERIAL_NUMBER_RW_0xF198:
					{
						if(Rx_multiplebytes[1] == 0x13)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&tester_serial_number_arr[0], &Rx_multiplebytes[5], 16);
							EOL.F_tester_serial_number = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_FEATURE_CODE_RW_0x0102:
						if(Rx_multiplebytes[1] == 0x17)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = 0x01;
							TX_DATA[3] = 0x02;
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							//memcpy(&Feature_code_arr[0], &UDS_Buffer[4], 3);
							memcpy(&Feature_code_arr[0], &Rx_multiplebytes[5], 20);
							EOL.F_FEATURE_CODE = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse(WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;

					case DID_VARIANT_CODE_RW_0x0101:
					
						if(Rx_multiplebytes[1] == 0x17)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							// memcpy(&Varient_code_arr[0], &Rx_multiplebytes[5], 5);
							memcpy(&Varient_code_arr[0], &Rx_multiplebytes[5], 20);
							EOL.F_Varient_code = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					

					case NS_VARIANT_CODE_RW_0x0107:
					
						if(Rx_multiplebytes[1] == 0x17)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							// memcpy(&Varient_code_arr[0], &Rx_multiplebytes[5], 5);
							memcpy(&NS_Varient_code_arr[0], &Rx_multiplebytes[5], 20);
							EOL.F_NS_Varient_code = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					
					/***************************Ecu Identifier W501*******************************************/

					case DID_CHIME_CONFIG_0x0503:
					{
						if(Rx_multiplebytes[1] == 0xD)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&DID_CHIMES_CONFIG_0x0503_arr[0], &Rx_multiplebytes[5],10);
							EOL.F_CHIMES_CONFIG = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_MESSAGE_CONFIG_0x0504:
					{
						if(Rx_multiplebytes[1] == 0x12)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&DID_MESSAGE_CONFIG_0x0504_arr[0], &Rx_multiplebytes[5],15);
							EOL.F_MESSAGE_CONFIG = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_SEAT_BELT_CONFIG_0x0507:
					{
						if(Rx_multiplebytes[1] == 0xA)
						{
						    if(DID_0507_maxvalue() == CLEAR)
						    {
							
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_SEAT_BELT_REMAINDER_0x0507_arr[0], &Rx_multiplebytes[5],7);
								EOL.F_SEAT_BELT_REMAINDER = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_FUEL_BAR_VALUE_CONFIG_0x050B:
					{
						if(Rx_multiplebytes[1] == 0x13)
						{
						    if(DID_050B_maxvalue() == CLEAR)
						    {
							if(fuel_value_compare_050B() == CLEAR)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0], &Rx_multiplebytes[5],16);
								EOL.F_FUEL_BAR_VALUE_CONFIG = SET;
								Write_UDS_Data();
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_CNC_0x22);
							}
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_TEMP_BAR_VALUE_CONFIG_0x050C:
					{
						if(Rx_multiplebytes[1] == 0x13)
						{
						    if(DID_050C_maxvalue() == CLEAR)
						    {
							if(Temp_value_compare_050C() == CLEAR)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0], &Rx_multiplebytes[5],16);
								EOL.F_TEMP_BAR_VALUE_CONFIG = SET;
								Write_UDS_Data();
							}
							else
							{
							    Save_NegResponse( WRITE_DATA_ID, NRC_CNC_0x22);
							}
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_TDMS_CONFIG_0x0510:
					{
						if(Rx_multiplebytes[1] == 0x8)
						{
						    if(DID_0510_maxvalue() == CLEAR)
						    {
							
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_TIRE_DIRE_MNGT_0x0510_arr[0], &Rx_multiplebytes[5],5);
								EOL.F_TIRE_DIRE_MNGT = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_IFE_CONFIG_0x0520:
					{
						if(Rx_multiplebytes[1] == 0x8)
						{
						    if(DID_0520_maxvalue() == CLEAR)
						    {
							
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_IFE_ECUIdentifier_0x0520_arr[0], &Rx_multiplebytes[5],5);
								EOL.F_IFE_ECUIdentifier_0x0520 = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
#if 0
					case DID_OSEK_NM_CONFIG_0x0521:
					{
						if(Rx_multiplebytes[1] == 0xB)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&DID_OSEK_NM_0x0521_arr[0], &Rx_multiplebytes[5],8);
							EOL.F_OSEK_NM = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
#endif
					case DID_RPAS_CONFIG_0x0522:
					{
						if(Rx_multiplebytes[1] == 0xB)
						{
						    if(DID_0522_maxvalue() == CLEAR)
						    {
							
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0], &Rx_multiplebytes[5],8);
								EOL.F_RPAS_MSG_STAYBACK_TIME = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_AFE2_CONFIG_0x0525:
					{
						if(Rx_multiplebytes[1] == 0xC)
						{
						    if(DID_0525_maxvalue() == CLEAR)
						    {
						    	Flag_AFE_reset= 1;
						    	U8_Reset_DTE = 1U;
						    	TX_DATA[0] = 0x03;
						    	TX_DATA[1] = 0x6E;
						    	TX_DATA[2] = Rx_multiplebytes[3];
						    	TX_DATA[3] = Rx_multiplebytes[4];
						    	TX_DATA[4] = 0x00;
						    	TX_DATA[5] = 0x00;
						    	TX_DATA[6] = 0x00;
						    	TX_DATA[7] = 0x00;
						    	memcpy(&DID_AFE_ECUIdentifier_0x0525_arr[0], &Rx_multiplebytes[5],9);
						    	EOL.F_AFE_ECUIdentifier_0x0525 = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_SERVICE_SLAB_CONFIG_0x050F:
					{
						if(Rx_multiplebytes[1] == 0xC)
						{
						    if(DID_050F_maxvalue() == CLEAR)
						    {
							
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0], &Rx_multiplebytes[5],9);
								EOL.F_SERVICE_DIST_SAP_CONFIG = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_AFE_DTE_CONFIG_0x04D2:
					{
						if(Rx_multiplebytes[1] == 0xB)
						{
						    if(DID_04D2_maxvalue() == CLEAR)
						    {
								if((Rx_multiplebytes[7] < Rx_multiplebytes[8]) && (Rx_multiplebytes[8] < Rx_multiplebytes[9]))
								{
									/*DTE Reset*/
									Flag_AFE_reset= 1;
									U8_Reset_DTE = 1U;

									TX_DATA[0] = 0x03;
									TX_DATA[1] = 0x6E;
									TX_DATA[2] = Rx_multiplebytes[3];
									TX_DATA[3] = Rx_multiplebytes[4];
									TX_DATA[4] = 0x00;
									TX_DATA[5] = 0x00;
									TX_DATA[6] = 0x00;
									TX_DATA[7] = 0x00;
									memcpy(&DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0], &Rx_multiplebytes[5],8);
									EOL.F_DTE_AFE_ECUIdentifier_0x04D2 = SET;
									Write_UDS_Data();

								}
								else
								{
									Save_NegResponse( WRITE_DATA_ID, NRC_CNC_0x22);
								}
						    }
						    else
						    {
								Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_OVERSPEED_CONFIG_0x04F7:
					{
						if(Rx_multiplebytes[1] == 0xA)
						{
						    if(DID_04F7_maxvalue() == CLEAR)
						    {
							
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&DID_OVERSPEED_0x04F7_arr[0], &Rx_multiplebytes[5],7);
								EOL.F_DTE_AFE_OVERSPEED = SET;
								Write_UDS_Data();
						    }
						    else
						    {
							Save_NegResponse( WRITE_DATA_ID, NRC_ROFR_0x31);
						    }
						}
						else
						{
						    Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
	#if 0
					case DID_ILLUMINATION_STATUS_CONFIG_0xF084:
					{
						if(Rx_multiplebytes[1] == 0x9)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&DID_ILLUMINATION_STATUS_0xF084_arr[0], &Rx_multiplebytes[5],6);
							EOL.F_ILLUMINATION_STATUS_CONFIG_0xF084 = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_RESET_REASON_CONFIG_0xF1B4:
					{
						if(Rx_multiplebytes[1] == 0x8)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&DID_RESET_REASON_0xF1B4_arr[0], &Rx_multiplebytes[5],5);
							EOL.F_RESET_REASON_CONFIG_0xF1B4 = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					case DID_EEPROM_LAYOUT_CONFIG_0xF1B7:
					{
						if(Rx_multiplebytes[1] == 0xB)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&DID_EEPROM_LAYOUT_0xF1B7_arr[0], &Rx_multiplebytes[5],8);
							EOL.F_RESET_COUNTER_CONFIG_0xF1B7 = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
	#endif
				case NS_Network_Mgmt_Config_0x0103:
					{
						if(Rx_multiplebytes[1] == 0x15)
						{
							if(DID_NM_0103_maxvalue() == CLEAR)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_NM_Configuration_0x0103_arr[0], &Rx_multiplebytes[5],18);
								EOL.F_NS_NW_Mgmt_Config_0x0103 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, 0x31);
							}
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
					
					case NS_CAN_Wake_Up_Config_0x0104:
					{
						if(Rx_multiplebytes[1] == 0x08)
						{
							if(DID_NM_0104_maxvalue() == CLEAR)
							{
								TX_DATA[0] = 0x03;
								TX_DATA[1] = 0x6E;
								TX_DATA[2] = Rx_multiplebytes[3];
								TX_DATA[3] = Rx_multiplebytes[4];
								TX_DATA[4] = 0x00;
								TX_DATA[5] = 0x00;
								TX_DATA[6] = 0x00;
								TX_DATA[7] = 0x00;
								memcpy(&NS_Can_Wakeup_Configuration_0x0104_arr[0], &Rx_multiplebytes[5],5);
								EOL.F_NS_CAN_WakeUp_Config_0x0104 = SET;
								Write_UDS_Data();
							}
							else
							{
								Save_NegResponse( WRITE_DATA_ID, 0x31);
							}
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
					
					case NS_CAN_Wake_Up_Config_PKE_0x010D:
					{
						if(Rx_multiplebytes[1] == 0x14)
						{
							TX_DATA[0] = 0x03;
							TX_DATA[1] = 0x6E;
							TX_DATA[2] = Rx_multiplebytes[3];
							TX_DATA[3] = Rx_multiplebytes[4];
							TX_DATA[4] = 0x00;
							TX_DATA[5] = 0x00;
							TX_DATA[6] = 0x00;
							TX_DATA[7] = 0x00;
							memcpy(&NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0], &Rx_multiplebytes[5],17);
							EOL.F_NS_CAN_WU_Config_PKE_0x010D = SET;
							Write_UDS_Data();
						}
						else
						{
							Save_NegResponse( WRITE_DATA_ID, NRC_ICDL_0x13);
						}
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}

					default:
						Save_NegResponse(WRITE_DATA_ID, NRC_ROFR_0x31);
						Respond_Single_Frame(WRITE_DATA_ID);
						break;
					}
				}
				else
				{
					Save_NegResponse(WRITE_DATA_ID, NRC_VSTH_0x88);
					Respond_Single_Frame(WRITE_DATA_ID);
				}
			}
			else
			{
				Save_NegResponse(WRITE_DATA_ID, NRC_EIR_0x83);
				Respond_Single_Frame(WRITE_DATA_ID);
			}
		}
		else
		{
			Save_NegResponse(WRITE_DATA_ID, NRC_SAD_0x33);
			Respond_Single_Frame(WRITE_DATA_ID);
		}
	}
	else
	{
		Save_NegResponse(WRITE_DATA_ID, NRC_SNS_0x7F);
		Respond_Single_Frame(WRITE_DATA_ID);
	}

	memset(Rx_multiplebytes,0,sizeof(Rx_multiplebytes));
}
}

uint8_t Read_Multi_DID(uint16_t DID, uint16_t start_byte)
{
    uint16_t  return_bytes_value=0;
	switch(DID)
	{
		case DID_UPDATEHISTORY_0x0201:
			UDS_TX_DATA[start_byte]=0x7F;
			UDS_TX_DATA[start_byte+1] = 0x02;
			UDS_TX_DATA[start_byte+2] = 0x01;
			memcpy(&UDS_TX_DATA[start_byte+3], &History_zone_Full, 125);
			return_bytes_value=125+3;
			break;
			
		case DID_BOOT_SOFTWARE_IDENTIFICATION_0xF180:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x80;
			memcpy(&UDS_TX_DATA[start_byte+3], &Boot_software_identification_arr, 8);
			return_bytes_value=8+3;
			break;
			
		case DID_APP_SOFTWARE_IDENTIFICATION_0xF181:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x81;
			memcpy(&UDS_TX_DATA[start_byte+3], &Ecu_software_version_number_arr, 8);
			return_bytes_value=8+3;
			break;
			
		case DID_VEH_MANU_SPAREPART_NUMBER_0xF187:
			UDS_TX_DATA[start_byte]=0x12;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x87;
			memcpy(&UDS_TX_DATA[start_byte+3], &Sparepart_number_arr[0], 16);
			return_bytes_value=16+3;
			break;	
			
		case DID_SYS_SUPPLIER_IDENTIFIER_0xF18A:
			UDS_TX_DATA[start_byte]=0x12;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x8A;
			memcpy(&UDS_TX_DATA[start_byte+3], &Supplier_identification_arr[0], 16);
			return_bytes_value=16+3;
			break;
			
		case DID_ECU_MANUFACTURING_DATE_RW_0xF18B:
			UDS_TX_DATA[start_byte]=0x08;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x8B;
			memcpy(&UDS_TX_DATA[start_byte+3], &Manufacturing_date_arr[0], 6);
			return_bytes_value=6+3;
			break;		
			
		case DID_ECU_SERIAL_NUMBER_0xF18C:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x8C;
			memcpy(&UDS_TX_DATA[start_byte+3], &Ecu_serial_number_arr[0], 8);
			return_bytes_value=8+3;
			break;
		
		case DID_VIN_RW_0xF190:
			UDS_TX_DATA[start_byte]=0x13;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x90;
			memcpy(&UDS_TX_DATA[start_byte+3], &VIN_arr, 17);
			return_bytes_value=17+3;
			break;
				
		case DID_ENGINE_TYPE_RW_0xF197:
			UDS_TX_DATA[start_byte]=0x12;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x97;
			memcpy(&UDS_TX_DATA[start_byte+3], &Engine_type_arr[0], 16);
			return_bytes_value=16+3;
			break;
		
		case DID_TESTER_SERIAL_NUMBER_RW_0xF198:
			UDS_TX_DATA[start_byte]=0x12;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x98;
			memcpy(&UDS_TX_DATA[start_byte+3], &tester_serial_number_arr[0], 16);
			return_bytes_value=16+3;
			break;
			
		case DID_PROGRAMMING_DATE_RW_0xF199:
			UDS_TX_DATA[start_byte]=0x08;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x99;
			memcpy(&UDS_TX_DATA[start_byte+3], &Programming_date_arr[0], 6);
			return_bytes_value=6+3;
			break;	
		
		case DID_ECU_INSTALLATION_DATE_RW_0xF19D:
			UDS_TX_DATA[start_byte]=0x08;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x9D;
			memcpy(&UDS_TX_DATA[start_byte+3], &Ecu_installation_date_arr[0], 6);
			return_bytes_value=6+3;
			break;
			
		case DID_MODEL_NUMBER_RW_0x0100:
			UDS_TX_DATA[start_byte]=0x14;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x00;
			memcpy(&UDS_TX_DATA[start_byte+3], &Model_number_arr[0], 18);
			return_bytes_value=18+3;
			break;	

		case DID_VARIANT_CODE_RW_0x0101:
			UDS_TX_DATA[start_byte]=0x16;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x01;
			memcpy(&UDS_TX_DATA[start_byte+3], &Varient_code_arr[0], 20);
			return_bytes_value=20+3;
			break;
			
		case DID_FEATURE_CODE_RW_0x0102:
			UDS_TX_DATA[start_byte]=0x16;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x02;
			memcpy(&UDS_TX_DATA[start_byte+3], &Feature_code_arr[0], 20);
			return_bytes_value=20+3;
			break;
			
		case DID_GRAPHIC_VERSION_0xF1AF:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xAF;
			memcpy(&UDS_TX_DATA[start_byte+3], &Graphics_version_read[0], 8);
			return_bytes_value=8+3;
			break;
			
		/*Ecu id */
		case DID_CHIME_CONFIG_0x0503:
			UDS_TX_DATA[start_byte]=0x0C;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x03;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_CHIMES_CONFIG_0x0503_arr[0], 10);
			return_bytes_value=10+3;
			break;
			
		case DID_MESSAGE_CONFIG_0x0504:
			UDS_TX_DATA[start_byte]=0x11;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x04;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_MESSAGE_CONFIG_0x0504_arr[0], 15);
			return_bytes_value=15+3;
			break;
			
		case DID_SEAT_BELT_CONFIG_0x0507:
			UDS_TX_DATA[start_byte]=0x09;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x07;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_SEAT_BELT_REMAINDER_0x0507_arr[0], 7);
			return_bytes_value=7+3;
			break;
		
		case DID_TDMS_CONFIG_0x0510:
			UDS_TX_DATA[start_byte]=0x07;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x10;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_TIRE_DIRE_MNGT_0x0510_arr[0], 5);
			return_bytes_value=5+3;
			break;
		
		case DID_IFE_CONFIG_0x0520:
			UDS_TX_DATA[start_byte]=0x07;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x20;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_IFE_ECUIdentifier_0x0520_arr[0], 5);
			return_bytes_value=5+3;
			break;
			
		case DID_RPAS_CONFIG_0x0522:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x22;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_RPAS_MSG_STAYBACK_TIME_0x0522_arr[0], 8);
			return_bytes_value=8+3;
			break;
			
		case DID_AFE1_CONFIG_0x0523:
			UDS_TX_DATA[start_byte]=0x0C;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x23;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_AFE_ECU_IDENTIFIER_0x0523_arr[0], 10);
			return_bytes_value=10+3;
			break;
			
		case DID_AFE_GD_CONFIG_0x0524:
			UDS_TX_DATA[start_byte]=0x0C;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x24;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_AFE_GD_ECUIdentifier_0x0524_arr[0], 10);
			return_bytes_value=10+3;
			break;
			
		case DID_AFE2_CONFIG_0x0525:
			UDS_TX_DATA[start_byte]=0x0B;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x25;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_AFE_ECUIdentifier_0x0525_arr[0], 9);
			return_bytes_value=9+3;
			break;
			
		case DID_FUEL_BAR_VALUE_CONFIG_0x050B:
			UDS_TX_DATA[start_byte]=0x12;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x0B;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[0], 16);
			return_bytes_value=16+3;
			break;
			
		case DID_TEMP_BAR_VALUE_CONFIG_0x050C:
			UDS_TX_DATA[start_byte]=0x12;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x0C;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_TEMP_BAR_VALUE_CONFIG_0x050C_arr[0], 16);
			return_bytes_value=16+3;
			break;
			
		case DID_SERVICE_SLAB_CONFIG_0x050F:
			UDS_TX_DATA[start_byte]=0x0B;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x0F;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_SERVICE_DIST_SLAB_CONFIG_0x050F_arr[0], 9);
			return_bytes_value=9+3;
			break;
			
		case DID_TELLTALE_FB_CONFIG_0x04CF:
			UDS_TX_DATA[start_byte]=0x1E;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xCF;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_TELL_TALE_FEEDBACK_ADC_VALUE_0x04CF_arr[0], 28);
			return_bytes_value=28+3;
			break;	
			
		case DID_DTE_CONFIG_0x04D0:
			UDS_TX_DATA[start_byte]=0x08;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xD0;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_DTE_ECUIdentifier_0x04D0_arr[0], 6);
			return_bytes_value=6+3;
			break;
			
		case DID_AFE_DTE_CONFIG_0x04D2:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xD2;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_DTE_AFE_ECUIdentifier_0x04D2_arr[0], 8);
			return_bytes_value=8+3;
			break;
			
		case DID_OVERSPEED_CONFIG_0x04F7:
			UDS_TX_DATA[start_byte]=0x09;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xF7;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_OVERSPEED_0x04F7_arr[0], 7);
			return_bytes_value=7+3;
			break;
			
		case DID_GAUGES_CONFIG_0xF082:
			UDS_TX_DATA[start_byte]=0x07;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x82;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_GAUGES_0xF082_arr[0], 5);
			return_bytes_value=5+3;
			break;
			
		case DID_ILLUMINATION_STATUS_CONFIG_0xF084:
			UDS_TX_DATA[start_byte]=0x08;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x84;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_ILLUMINATION_STATUS_0xF084_arr[0], 6);
			return_bytes_value=6+3;
			break;	
			
		case DID_RESET_REASON_CONFIG_0xF1B4:
			UDS_TX_DATA[start_byte]=0x07;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB4;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_RESET_REASON_0xF1B4_arr[0], 5);
			return_bytes_value=5+3;
			break;
			
		case DID_EEPROM_LAYOUT_CONFIG_0xF1B7:
			UDS_TX_DATA[start_byte]=0x0A;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB7;
			memcpy(&UDS_TX_DATA[start_byte+3], &DID_EEPROM_LAYOUT_0xF1B7_arr[0], 8);
			return_bytes_value=8+3;
			break;
		/*Network mangement DID*/	
		case NS_Network_Mgmt_Config_0x0103:
			UDS_TX_DATA[start_byte]=0x14;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x03;
			memcpy(&UDS_TX_DATA[start_byte+3], &NS_NM_Configuration_0x0103_arr[0], 18);
			return_bytes_value=18+3;
			break;
			
		case NS_CAN_Wake_Up_Config_0x0104:
			UDS_TX_DATA[start_byte]=0x07;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x04;
			memcpy(&UDS_TX_DATA[start_byte+3], &NS_Can_Wakeup_Configuration_0x0104_arr[0], 5);
			return_bytes_value=5+3;
			break;
			
		case NS_Network_Security_Fault_MSG_Detail_0x0105:
			UDS_TX_DATA[start_byte]=0x20;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x05;
			memcpy(&UDS_TX_DATA[start_byte+3], &NS_Fault_Msg_Details_0x0105_arr[0], 30);
			return_bytes_value=30+3;
			break;
			
		case NS_VARIANT_CODE_RW_0x0107:
			UDS_TX_DATA[start_byte]=0x16;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x07;
			memcpy(&UDS_TX_DATA[start_byte+3], &NS_Varient_code_arr[0], 20);
			return_bytes_value=20+3;
			break;
			
		case NS_NSC_0x010B:
			UDS_TX_DATA[start_byte]=0x0D;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x0B;
			memcpy(&UDS_TX_DATA[start_byte+3], &NS_Network_Security_Configuration_0x010B_arr[0], 11);
			return_bytes_value=11+3;
			break;
			
		case NS_CAN_Wake_Up_Config_PKE_0x010D:
			UDS_TX_DATA[start_byte]=0x13;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x0D;
			memcpy(&UDS_TX_DATA[start_byte+3], &NS_CAN_WEKUP_Configuration_PKE_0x010D_arr[0], 17);
			return_bytes_value=17+3;
			break;	
																												
		/*Single frame */
		
		case DID_BOOT_FLAG_0x0200:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x02;
			UDS_TX_DATA[start_byte+2] = 0x00;
			UDS_TX_DATA[start_byte+3] = BOOT_FLAG_STATUS;
			return_bytes_value=1+3;
			break;
								
		case DID_ACTIVE_DIAG_SESSION_0xF186:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x86;
			UDS_TX_DATA[start_byte+3] = ACTIVE_DIAG_SESSION;
			return_bytes_value=1+3;
			break;
			
		case DID_ECU_HARDWARE_VERSION_NUMBER_0xF193:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0x93;
			UDS_TX_DATA[start_byte+3] = Ecu_hardware_version_number_arr[0];
			UDS_TX_DATA[start_byte+4] = Ecu_hardware_version_number_arr[1];
			UDS_TX_DATA[start_byte+5] = Ecu_hardware_version_number_arr[2];
			UDS_TX_DATA[start_byte+6] = Ecu_hardware_version_number_arr[3];
			return_bytes_value=4+3;
			break;
		
		case DID_TT_BLINK_PATTERN3_0x0502:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x02;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_TELL_TALE_BLINK_PATTERN3_0x0502_arr[3];
			return_bytes_value=4+3;
			break;
				
		case DID_SPEED_CHIME_CONFIG_0x0509:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x09;
			UDS_TX_DATA[start_byte+3] = DID_CHIME_ACTIVATION_0x0509_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_AVG_SPEED_CONFIG_0x0511:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x11;
			UDS_TX_DATA[start_byte+3] = DID_AVG_SPEED_0x0511_arr[0];
			UDS_TX_DATA[start_byte+4] =	DID_AVG_SPEED_0x0511_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_FUEL_TT_VALUE_CONFIG_0x050D:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x0D;
			UDS_TX_DATA[start_byte+3] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_FUEL_TT_VALUE_CONFIG_0x050D_arr[3];
			return_bytes_value=4+3;
			break;
			
		case DID_TEMP_TT_VALUE_CONFIG_0x050E:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x0E;
			UDS_TX_DATA[start_byte+3] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_TEMP_TT_VALUE_CONFIG_0x050E_arr[3];
			return_bytes_value=4+3;
			break;
			
		case DID_TT_BLINK_PATTERN4_0x04FB:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xFB;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_TELL_TALE_BLINK_PATTERN4_0x04FB_arr[3];
			return_bytes_value=4+3;
			break;
			
		case DID_TT_BLINK_PATTERN5_0x04FC:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xFC;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_TELL_TALE_BLINK_PATTERN5_0x04FC_arr[3];
			return_bytes_value=4+3;
			break;
			
		case DID_TT_BLINK_PATTERN8_0x04FD:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xFD;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_TELL_TALE_BLINK_PATTERN8_0x04FD_arr[3];
			return_bytes_value=4+3;
			break;
			
		case DID_TT_BLINK_PATTERN9_0x04FE:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xFE;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[2];
			UDS_TX_DATA[start_byte+6] = DID_TELL_TALE_BLINK_PATTERN9_0x04FE_arr[3];
			return_bytes_value=4+3;
			break;
			
		case DID_FUEL_GEAN_INDICATOR_CONFIG_0x04D1:
			UDS_TX_DATA[start_byte]=0x05;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xD1;
			UDS_TX_DATA[start_byte+3] = DID_FUEL_GEAN_INDICATOR_0x04D1_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_FUEL_GEAN_INDICATOR_0x04D1_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_FUEL_GEAN_INDICATOR_0x04D1_arr[2];
			return_bytes_value=3+3;
			break;
			
		case DID_FUEL_DEAD_VOLUME_CONFIG_0x04D5:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xD5;
			UDS_TX_DATA[start_byte+3] = DID_FUEL_DEAD_VOLUME_0x04D5_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_NVM_SERVICE_DONE_CONFIG_0x04F3:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xF3;
			UDS_TX_DATA[start_byte+3] = DID_NVM_SERVICE_DONE_0x04F3_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_ODOMETER_RAW_CONFIG_0x04F4:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xF4;
			UDS_TX_DATA[start_byte+3] = DID_ODOMETER_RAW_0x04F4_arr[0];
			return_bytes_value=1+3;
			break;		
			
		case DID_DID_ODOMETER_PERCENTAGE_CONFIG_0x04F6:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xF6;
			UDS_TX_DATA[start_byte+3] = DID_DID_ODOMETER_PERCENTAGE_0x04F6_arr[0];
			return_bytes_value=1+3;
			break;	
			
		case DID_TPMS_TELLTALE_CONFIG_0x04F8:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xF8;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_0x04F8_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_FUEL_CONS_ACCM_CONFIG_0x04F9:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0x04;
			UDS_TX_DATA[start_byte+2] = 0xF9;
			UDS_TX_DATA[start_byte+3] = DID_AFE_ECUIdentifier_0x04F9_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_AFE_ECUIdentifier_0x04F9_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_TELL_TALE_STATUS_SET1_CONFIG_0xF07F:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x7F;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_STATUS_SET1_0xF07F_arr[0];
			return_bytes_value=1+3;
			break;	
			
		case DID_TELL_TALE_STATUS_SET2_CONFIG_0xF080:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x80;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_STATUS_SET2_0xF080_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_TELL_TALE_STATUS_SET3_CONFIG_0xF081:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x81;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_STATUS_SET3_0xF081_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_BAR_GRAPH_STATUS_CONFIG_0xF083:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x83;
			UDS_TX_DATA[start_byte+3] = DID_BAR_GRAPH_STATUS_0xF083_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_GEAR_INDICATION_STATUS_CONFIG_0xF085:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x85;
			UDS_TX_DATA[start_byte+3] = DID_GEAR_INDICATION_STATUS_0xF085_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_GEAR_INDICATION_STATUS_0xF085_arr[1];
			return_bytes_value=2+3;
			break;		
				
		case DID_TELL_TALE_STATUS_SET4_CONFIG_0xF08E:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x8E;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_STATUS_SET4_0xF08E_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_TELL_TALE_STATUS_SET5_CONFIG_0xF08F:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x8F;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_STATUS_SET5_0xF08F_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_TELL_TALE_STATUS_SET6_CONFIG_0xF096:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0xF0;
			UDS_TX_DATA[start_byte+2] = 0x96;
			UDS_TX_DATA[start_byte+3] = DID_TELL_TALE_STATUS_SET6_0xF096_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_ODOMETER_CONFIG_0xF1B0:
			UDS_TX_DATA[start_byte]=0x05;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB0;
			UDS_TX_DATA[start_byte+3] = DID_ODOMETER_0xF1B0_read_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_ODOMETER_0xF1B0_read_arr[1];
			UDS_TX_DATA[start_byte+5] = DID_ODOMETER_0xF1B0_read_arr[2];
			return_bytes_value=3+3;
			break;
			
		case DID_BATTERY_VOLTAGE_CONFIG_0xF1B1:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB1;
			UDS_TX_DATA[start_byte+3] = DID_BATTERY_VOLTAGE_0xF1B1_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_BATTERY_VOLTAGE_0xF1B1_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_VEHICLE_SPEED_CONFIG_0xF1B2:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB2;
			UDS_TX_DATA[start_byte+3] = DID_VEHICLE_SPEED_0xF1B2_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_VEHICLE_SPEED_0xF1B2_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_ENGINE_SPEED_CONFIG_0xF1B3:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB3;
			UDS_TX_DATA[start_byte+3] = DID_ENGINE_SPEED_0xF1B3_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_ENGINE_SPEED_0xF1B3_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_RESET_COUNTER_CONFIG_0xF1B5:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB5;
			UDS_TX_DATA[start_byte+3] = DID_RESET_COUNTER_0xF1B5_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_RESET_COUNTER_0xF1B5_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_IGNITION_COUNTER_CONFIG_0xF1B6:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0xF1;
			UDS_TX_DATA[start_byte+2] = 0xB6;
			UDS_TX_DATA[start_byte+3] = DID_IGNITION_COUNTER_0xF1B6_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_IGNITION_COUNTER_0xF1B6_arr[1];
			return_bytes_value=2+3;
			break;
			
		case DID_ETM_CONFIG_0x0500:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x00;
			UDS_TX_DATA[start_byte+3] = DID_ETM_0x0500_arr[0];
			return_bytes_value=1+3;
			break;
			
		case DID_ROLL_PITCH_DETECTION_0x0533:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x33;
			UDS_TX_DATA[start_byte+3] = DID_Roll_Pitch_Detection_0x0533_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_Roll_Pitch_Detection_0x0533_arr[1];
			return_bytes_value=2+3;
			break;
#ifdef SPRINT4			
		case DID_AT_GEAR_ALERT_0x0543:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0x05;
			UDS_TX_DATA[start_byte+2] = 0x33;
			UDS_TX_DATA[start_byte+3] = DID_AT_GEAR_ALERT_0x0543_arr[0];
			UDS_TX_DATA[start_byte+4] = DID_AT_GEAR_ALERT_0x0543_arr[1];
			return_bytes_value=2+3;
			break;
#endif			
			/*Network mangement DID*/									
		case NS_Network_Security_Fault_Type_STS_Info_0x0106:
			UDS_TX_DATA[start_byte]=0x03;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x06;
			UDS_TX_DATA[start_byte+3] = NS_Fault_Type_Status_Info_0x0106_arr[0];
			return_bytes_value=1+3;
			break;
			
		case NS_Network_Security_key_SKC_0x0108:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x08;
			UDS_TX_DATA[start_byte+3] = NS_Network_Security_key_SKC_0x0108_arr[0];
			UDS_TX_DATA[start_byte+4] = NS_Network_Security_key_SKC_0x0108_arr[1];
			UDS_TX_DATA[start_byte+5] = NS_Network_Security_key_SKC_0x0108_arr[2];
			UDS_TX_DATA[start_byte+6] = NS_Network_Security_key_SKC_0x0108_arr[3];
			return_bytes_value=4+3;
			break;
			
		case NS_Network_Security_key_SKC1_0x0109:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x09;
			UDS_TX_DATA[start_byte+3] = NS_Network_Security_key_SKC1_0x0109_arr[0];
			UDS_TX_DATA[start_byte+4] = NS_Network_Security_key_SKC1_0x0109_arr[1];
			UDS_TX_DATA[start_byte+5] = NS_Network_Security_key_SKC1_0x0109_arr[2];
			UDS_TX_DATA[start_byte+6] = NS_Network_Security_key_SKC1_0x0109_arr[3];
			return_bytes_value=4+3;
			break;
			
		case NS_Network_Security_key_SKC2_0x010A:
			UDS_TX_DATA[start_byte]=0x06;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x0A;
			UDS_TX_DATA[start_byte+3] = NS_Network_Security_key_SKC2_0x010A_arr[0];
			UDS_TX_DATA[start_byte+4] = NS_Network_Security_key_SKC2_0x010A_arr[1];
			UDS_TX_DATA[start_byte+5] = NS_Network_Security_key_SKC2_0x010A_arr[2];
			UDS_TX_DATA[start_byte+6] = NS_Network_Security_key_SKC2_0x010A_arr[3];
			return_bytes_value=4+3;
			break;
			
		case NS_Network_MSG_Config_0x010E:
			UDS_TX_DATA[start_byte]=0x04;
			UDS_TX_DATA[start_byte+1] = 0x01;
			UDS_TX_DATA[start_byte+2] = 0x0E;
			UDS_TX_DATA[start_byte+3] = NS_NW_Msg_config_0x010E_arr[0];
			UDS_TX_DATA[start_byte+4] = NS_NW_Msg_config_0x010E_arr[1];
			return_bytes_value=2+3;
			break;	
																							
	}
	return return_bytes_value;
}
	
/**********************************************************************************
## Module:      UDSS_APP.c
## Function:    udss_Readdatabyidentifier()
## Description: Reads data by indentifier.
## Parameter:   None
## Return£º     void
 **********************************************************************************/
PUBLIC void udss_Readdatabyidentifier_MultipleDID(void)
{
	if(Functional_Addressing_Flag == CLEAR)
	{
		uint16_t DID = CLEAR;
		uint8_t  no_of_DID=CLEAR;
		uint8_t DID_buffer_filling=CLEAR;
		uint16_t start_byte=3;
		uint16_t no_of_DID_filled = 0;
		uint8_t extract_did = 0;
		if(((Rx_multiplebytes[1]-1)%2)==0)
		{
			no_of_DID = (Rx_multiplebytes[1]-1)/2;
			extract_did=3;
			for(DID_buffer_filling=0;DID_buffer_filling<no_of_DID;DID_buffer_filling++)
			{
			    DID=(Rx_multiplebytes[extract_did] << 8)|Rx_multiplebytes[extract_did+1];
				no_of_DID_filled=Read_Multi_DID(DID ,start_byte);
				extract_did=extract_did+2;
				start_byte=start_byte+no_of_DID_filled;
			}
			UDS_TX_DATA[0] = 0x10;
			UDS_TX_DATA[1] = start_byte - 2;
			UDS_TX_DATA[2] = 0x62;
			Respond_Multiple_Frames(UDS_TX_DATA[1] + 2);
		}
		else
		{
			Save_NegResponse(READ_DATA_BY_IDENTIFIER, NRC_ICDL_0x13);
			Respond_Single_Frame(READ_DATA_BY_IDENTIFIER);
		}
	}
}

uint8_t DID_0507_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] >= 10)&&(Rx_multiplebytes[5] <= 200) && (Rx_multiplebytes[6] <= 255) && (Rx_multiplebytes[7] >= 1)&&(Rx_multiplebytes[7] <= 20) && (Rx_multiplebytes[8] <= 255) &&
	   (Rx_multiplebytes[9] <= 255) && (Rx_multiplebytes[10] <= 100) && (Rx_multiplebytes[11] <= 255))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
} 

uint8_t DID_0510_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] <= 150) && (Rx_multiplebytes[6] <= 150) && (Rx_multiplebytes[7] <= 150) && 
	   (Rx_multiplebytes[8] <= 150) &&(Rx_multiplebytes[9] <= 150))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
} 

uint8_t DID_0511_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((UDS_Buffer[4] <= 255) && (UDS_Buffer[5] <= 255))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_0520_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] >= 1)&&(Rx_multiplebytes[5] <= 255) && (Rx_multiplebytes[6] >=1)&&(Rx_multiplebytes[6] <= 255) && (Rx_multiplebytes[7] >=1)&&(Rx_multiplebytes[7] <= 100) && 
	   (Rx_multiplebytes[8] >= 1)&&(Rx_multiplebytes[8] <= 100) &&(Rx_multiplebytes[9] <= 255))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
} 

 uint8_t DID_0522_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] <= 1) && (Rx_multiplebytes[6] <= 11) && (Rx_multiplebytes[7] <= 1) && (Rx_multiplebytes[8] <= 30) &&
	   (Rx_multiplebytes[9] <= 5) && (Rx_multiplebytes[10] <= 100) && (Rx_multiplebytes[11] <= 100)&&(Rx_multiplebytes[12] >= 4)&&(Rx_multiplebytes[12] <= 14))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
 
uint8_t DID_0525_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	uint16_t AFE_display_filterB=(uint16_t)((Rx_multiplebytes[9] << 8)|Rx_multiplebytes[10]);
	
	if((Rx_multiplebytes[5] <= 255) && (Rx_multiplebytes[6] >= 1)&&(Rx_multiplebytes[6] <= 100) && (Rx_multiplebytes[7] >= 1)&&(Rx_multiplebytes[7] <= 100)&&
	   (Rx_multiplebytes[8] >= 1)&&(Rx_multiplebytes[8] <= 50) &&
	   (AFE_display_filterB >=100) &&(AFE_display_filterB <=3000) && 
	   (Rx_multiplebytes[11] <= 0x01) && (Rx_multiplebytes[12] >=1)&&
	   (Rx_multiplebytes[12] <= 48) && (Rx_multiplebytes[13] >= 1)&&(Rx_multiplebytes[13] <= 50))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
} 

uint8_t DID_050B_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] <= 140) && (Rx_multiplebytes[6] <= 140) && (Rx_multiplebytes[7] <= 140) && (Rx_multiplebytes[8] <= 140) &&
	   (Rx_multiplebytes[9] <= 140) && (Rx_multiplebytes[10] <= 140) && (Rx_multiplebytes[11] <= 140)&&(Rx_multiplebytes[12] <= 140) &&
	   (Rx_multiplebytes[13] <= 140) && (Rx_multiplebytes[14] <= 140) && (Rx_multiplebytes[15] <= 140)&&(Rx_multiplebytes[16] <= 140) &&
	   (Rx_multiplebytes[17] <= 140) && (Rx_multiplebytes[18] <= 140) && (Rx_multiplebytes[19] <= 140)&&(Rx_multiplebytes[20] <= 140))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_050C_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] <= 255) && (Rx_multiplebytes[6] <= 255) && (Rx_multiplebytes[7] <= 255) && (Rx_multiplebytes[8] <= 255) &&
	   (Rx_multiplebytes[9] <= 255) && (Rx_multiplebytes[10] <= 255) && (Rx_multiplebytes[11] <= 255)&&(Rx_multiplebytes[12] <= 255) &&
	   (Rx_multiplebytes[12] <= 255) && (Rx_multiplebytes[13] <= 255) && (Rx_multiplebytes[14] <= 255)&&(Rx_multiplebytes[15] <= 255) &&
	   (Rx_multiplebytes[16] <= 255) && (Rx_multiplebytes[17] <= 255) && (Rx_multiplebytes[18] <= 255)&&(Rx_multiplebytes[19] <= 255))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_050D_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((UDS_Buffer[4] <= 140) && (UDS_Buffer[5] <= 140)&& (UDS_Buffer[6] <= 140)&& (UDS_Buffer[7] <= 140))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_050E_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((UDS_Buffer[4] <= 255) && (UDS_Buffer[5] <= 255)&& (UDS_Buffer[6] <= 255)&& (UDS_Buffer[7] <= 255))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_050F_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] <= 255) && (Rx_multiplebytes[6] <= 255) && (Rx_multiplebytes[7] <= 255) && (Rx_multiplebytes[8] <= 255) &&
	   (Rx_multiplebytes[9] <= 255) && (Rx_multiplebytes[10] <= 255) && (Rx_multiplebytes[11] <= 255)&&(Rx_multiplebytes[12] <= 255) &&
	   (Rx_multiplebytes[13] <= 100))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_0502_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	uint16_t Telltale_ESC_on_time=0;
	uint16_t Telltale_ESC_off_time=0;
	Telltale_ESC_on_time = (uint16_t)((UDS_Buffer[4] << 8)|(UDS_Buffer[5]));
	Telltale_ESC_off_time = (uint16_t)((UDS_Buffer[6] << 8)|(UDS_Buffer[7]));
	if((Telltale_ESC_on_time >=100)&&(Telltale_ESC_on_time <=5000)&& (Telltale_ESC_off_time>=100)&&(Telltale_ESC_off_time<=5000))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
 
 uint8_t DID_04FB_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	uint16_t Telltale_Imm_on_time=0;
	uint16_t Telltale_Imm_off_time=0;
	Telltale_Imm_on_time = (uint16_t)((UDS_Buffer[4] << 8)|(UDS_Buffer[5]));
	Telltale_Imm_off_time = (uint16_t)((UDS_Buffer[6] << 8)|(UDS_Buffer[7]));
	if((Telltale_Imm_on_time >=50)&&(Telltale_Imm_on_time <=5000)&& (Telltale_Imm_off_time>=250)&&(Telltale_Imm_off_time<=5000))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
 
 uint8_t DID_04FC_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	uint16_t Telltale_Imm_5_on_time=0;
	uint16_t Telltale_Imm_5_off_time=0;
	Telltale_Imm_5_on_time = (uint16_t)((UDS_Buffer[4] << 8)|(UDS_Buffer[5]));
	Telltale_Imm_5_off_time = (uint16_t)((UDS_Buffer[6] << 8)|(UDS_Buffer[7]));
	if((Telltale_Imm_5_on_time >= 80)&&(Telltale_Imm_5_on_time <=5000)&& (Telltale_Imm_5_off_time>=250)&&(Telltale_Imm_5_off_time<=5000))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
 
uint8_t DID_04FD_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	uint16_t Telltale_TPMS8_on_time=0;
	uint16_t Telltale_TPMS8_off_time=0;
	Telltale_TPMS8_on_time = (uint16_t)((UDS_Buffer[4] << 8)|(UDS_Buffer[5]));
	Telltale_TPMS8_off_time = (uint16_t)((UDS_Buffer[6] << 8)|(UDS_Buffer[7]));
	if((Telltale_TPMS8_on_time >=100)&&(Telltale_TPMS8_on_time <=5000)&& (Telltale_TPMS8_off_time>=100)&&(Telltale_TPMS8_off_time<=5000))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_04FE_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	uint16_t Telltale_TPMS9_on_time=0;
	uint16_t Telltale_TPMS9_off_time=0;
	Telltale_TPMS9_on_time = (uint16_t)((UDS_Buffer[4] << 8)|(UDS_Buffer[5]));
	Telltale_TPMS9_off_time = (uint16_t)((UDS_Buffer[6] << 8)|(UDS_Buffer[7]));
	if((Telltale_TPMS9_on_time >=100)&&(Telltale_TPMS9_on_time <=5000)&&(Telltale_TPMS9_off_time>=100)&&(Telltale_TPMS9_off_time<=5000))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
 
 uint8_t DID_04D2_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] >= 1)&&(Rx_multiplebytes[5] <= 100) && (Rx_multiplebytes[6] <= 230) && (Rx_multiplebytes[7] >=1)&&(Rx_multiplebytes[7] <= 50) && (Rx_multiplebytes[8] >= 1) &&(Rx_multiplebytes[8] <= 50) &&
	   (Rx_multiplebytes[9] >=1)&&(Rx_multiplebytes[9] <= 50) && (Rx_multiplebytes[10] <= 230) &&(Rx_multiplebytes[11] <= 230)&&(Rx_multiplebytes[12] <= 3))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
uint8_t DID_04F7_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5] >= 40)&&(Rx_multiplebytes[5] <= 100) && (Rx_multiplebytes[6] > 100)&&(Rx_multiplebytes[6] <= 220) && (Rx_multiplebytes[7] <= 0X02) && (Rx_multiplebytes[8] <= 255) &&
	   (Rx_multiplebytes[9] <= 2) && (Rx_multiplebytes[10] <= 255) && (Rx_multiplebytes[11] <= 30))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}

uint8_t DID_0533_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((UDS_Buffer[4] <= 120) && (UDS_Buffer[5] <= 120))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
#ifdef SPRINT4
uint8_t DID_0543_maxvalue(void)
{
	uint8_t DID_value_validation_u1l=CLEAR;
	if((UDS_Buffer[4] <= 100) && (UDS_Buffer[5] <= 30))
	{
		DID_value_validation_u1l = CLEAR;
	}
	else
	{
		DID_value_validation_u1l = SET;
	}
	return DID_value_validation_u1l;
}
#endif
uint8_t DID_NM_0103_maxvalue(void)
{
	uint8_t DID_value_0103_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5]!=0)&&(Rx_multiplebytes[6]!=0)&&(Rx_multiplebytes[7]!=0)&&(Rx_multiplebytes[8]!=0)&&
	   (Rx_multiplebytes[9]!=0)&&(Rx_multiplebytes[10]!=0)&&(Rx_multiplebytes[11]!=0)&&(Rx_multiplebytes[12]!=0)&&
	   (Rx_multiplebytes[13]!=0)&&(Rx_multiplebytes[14]!=0)&&(Rx_multiplebytes[15]!=0)&&(Rx_multiplebytes[16]!=0)&&
	   (Rx_multiplebytes[17]!=0)&&(Rx_multiplebytes[18]!=0)&&(Rx_multiplebytes[19]!=0)&&(Rx_multiplebytes[20]!=0)&&
	   (Rx_multiplebytes[21]<=1)&&(Rx_multiplebytes[22]>=10)&&(Rx_multiplebytes[22]<=255))
	{
		DID_value_0103_validation_u1l=CLEAR;
	}
	else
	{
		DID_value_0103_validation_u1l=SET;
	}
	return DID_value_0103_validation_u1l;
}

uint8_t DID_NM_0104_maxvalue(void)
{
	uint8_t DID_value_0104_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5]!=0)&&(Rx_multiplebytes[6]!=0)&&(Rx_multiplebytes[7]!=0)&&(Rx_multiplebytes[8]!=0)&&
	   (Rx_multiplebytes[9]!=0))
	{
		DID_value_0104_validation_u1l=CLEAR;
	}
	else
	{
		DID_value_0104_validation_u1l=SET;
	}
	return DID_value_0104_validation_u1l;
}

uint8_t DID_NM_010B_maxvalue(void)
{
	uint8_t DID_value_010B_validation_u1l=CLEAR;
	if((Rx_multiplebytes[5]!=0)&&(Rx_multiplebytes[12]!=0)&&(Rx_multiplebytes[14]!=0))
	{
		DID_value_010B_validation_u1l=CLEAR;
	}
	else
	{
		DID_value_010B_validation_u1l=SET;
	}
	return DID_value_010B_validation_u1l;
}

uint8_t fuel_value_compare_050B(void)
{
	uint8_t DID_ON_compare_validation_u1l=SET;
	uint8_t DID_OFF_compare_validation_u1l=SET;
	if((Rx_multiplebytes[5]> Rx_multiplebytes[6]) && (Rx_multiplebytes[6] > Rx_multiplebytes[7]) && 
	   (Rx_multiplebytes[7]> Rx_multiplebytes[8]) && (Rx_multiplebytes[8] > Rx_multiplebytes[9]) &&
	   (Rx_multiplebytes[9]> Rx_multiplebytes[10]) && (Rx_multiplebytes[10] > Rx_multiplebytes[11]) && 
	   (Rx_multiplebytes[11]> Rx_multiplebytes[12])) 
	{
		DID_ON_compare_validation_u1l = CLEAR;
	}
	
	if((Rx_multiplebytes[13]> Rx_multiplebytes[14]) && (Rx_multiplebytes[14] > Rx_multiplebytes[15]) && 
	   (Rx_multiplebytes[15]> Rx_multiplebytes[16]) && (Rx_multiplebytes[16] > Rx_multiplebytes[17]) &&
	   (Rx_multiplebytes[17]> Rx_multiplebytes[18]) && (Rx_multiplebytes[18] > Rx_multiplebytes[19]) && 
	   (Rx_multiplebytes[19]> Rx_multiplebytes[20])) 
	{
		DID_OFF_compare_validation_u1l = CLEAR;
	}
	return (DID_ON_compare_validation_u1l|DID_OFF_compare_validation_u1l);
}

uint8_t Temp_value_compare_050C(void)
{
	uint8_t DID_TEMPON_compare_validation_u1l=SET;
	uint8_t DID_TEMPOFF_compare_validation_u1l=SET;
	if((Rx_multiplebytes[5]< Rx_multiplebytes[6]) && (Rx_multiplebytes[6] < Rx_multiplebytes[7]) && 
	   (Rx_multiplebytes[7]< Rx_multiplebytes[8]) && (Rx_multiplebytes[8] < Rx_multiplebytes[9]) &&
	   (Rx_multiplebytes[9]< Rx_multiplebytes[10]) && (Rx_multiplebytes[10] < Rx_multiplebytes[11]) && 
	   (Rx_multiplebytes[11]< Rx_multiplebytes[12])) 
	{
		DID_TEMPON_compare_validation_u1l = CLEAR;
	}
	
	if((Rx_multiplebytes[13]< Rx_multiplebytes[14]) && (Rx_multiplebytes[14] < Rx_multiplebytes[15]) && 
	   (Rx_multiplebytes[15]< Rx_multiplebytes[16]) && (Rx_multiplebytes[16] < Rx_multiplebytes[17]) &&
	   (Rx_multiplebytes[17]< Rx_multiplebytes[18]) && (Rx_multiplebytes[18] < Rx_multiplebytes[19]) && 
	   (Rx_multiplebytes[19]< Rx_multiplebytes[20])) 
	{
		DID_TEMPOFF_compare_validation_u1l = CLEAR;
	}
	return (DID_TEMPON_compare_validation_u1l|DID_TEMPOFF_compare_validation_u1l);
}

uint8_t Fuel_TT_value_compare_050D(void)
{
	uint8_t DID_Fuel_TT_compare_u1l=SET;
	if((UDS_Buffer[4]< UDS_Buffer[5]) && (UDS_Buffer[5] < UDS_Buffer[6]) && 
	   (UDS_Buffer[6]< UDS_Buffer[7]))
	{
		DID_Fuel_TT_compare_u1l = CLEAR;
	}
	
	return DID_Fuel_TT_compare_u1l;
}

uint8_t Temp_TT_value_compare_050E(void)
{
	uint8_t DID_TEMP_TT_compare_u1l=SET;
	if((UDS_Buffer[4]< UDS_Buffer[5]) && (UDS_Buffer[5] < UDS_Buffer[6]) && 
	   (UDS_Buffer[6]< UDS_Buffer[7]))
	{
		DID_TEMP_TT_compare_u1l = CLEAR;
	}
	
	return DID_TEMP_TT_compare_u1l;
}
