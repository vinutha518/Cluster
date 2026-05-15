
#include "UDS_DTC.h"
#include "UDSS.h"
#include "UDSS_APP.h"

#define SET			1U
#define CLEAR		0U
uint8_t U8g_Block_All_ECU_CAN_MSG = 1;	/*change 290220*/

uint8_t U8g_Block_EMS_ECU_CAN_MSG = 0;
uint8_t U8g_Block_ABS_ECU_CAN_MSG = 0;
uint8_t U8g_Block_ESC_ECU_CAN_MSG = 0;
uint8_t U8g_Block_SAS_ECU_CAN_MSG = 0;
uint8_t U8g_Block_PKE_ECU_CAN_MSG = 0;
uint8_t U8g_Block_ICU_ECU_CAN_MSG = 0;

uint8_t U8g_Block_SRS_ECU_CAN_MSG = 0;
uint8_t U8g_Block_TCU_ECU_CAN_MSG = 0;
uint8_t U8g_Block_IC_ECU_CAN_MSG = 0;
uint8_t U8g_Block_BCM_ECU_CAN_MSG = 0;
uint8_t U8g_Block_IS_ECU_CAN_MSG = 0;
uint8_t U8g_Block_RPAS_ECU_CAN_MSG=0;

extern DTC DTC_IVN_VARIENT_ERR_D505_Buff;

void Check_Vaild_Variant_Code(void)
{

    if((Varient_code_arr[0] & 0x1) == 0x00)
    {
	DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[3] = DTC_CNF_TEST_FAIL; /*DTC currently present*/
	U8g_Block_All_ECU_CAN_MSG = SET;
    } 
    else if((Varient_code_arr[0] & 0x1) == 0x01)
    {
	if(((Varient_code_arr[0] >> 2) & 0x3) == 0x03)      /*ABS & ESC*/
	{
	    DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[3] = DTC_CNF_TEST_FAIL;  /*DTC currently present*/
	    U8g_Block_All_ECU_CAN_MSG = SET;			
	}
	else if(((Varient_code_arr[0] >> 6) & 0x3) == 0x03) /*PKE/SKM & ICU*/
	{
	    DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[3] = DTC_CNF_TEST_FAIL;  /*DTC currently present*/
	    U8g_Block_All_ECU_CAN_MSG = SET;			
	}
/*	else if(((Varient_code_arr[1] >> 4) & 0x1) == 0x01) @/@*$AMT & TCU  only TCU bit checked, AMT bit need to verify$*@/@
 - 	{
 - 	   DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[3] = 0x09;  @/@*$DTC currently present$*@/@
 - 	   U8g_Block_All_ECU_CAN_MSG = SET;			
 - 	}*/
	else
	{
		if(DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[3] == DTC_CNF_TEST_FAIL)
		{
			DTC_IVN_VARIENT_ERR_D505_Buff.BYTEs[3] = DTC_CNF; /*DTC previously present*/
		}
	    U8g_Block_All_ECU_CAN_MSG = CLEAR;
	}
		
	/****************POWERTRAIN***********************/
	if(((Varient_code_arr[0] >> 1) & 0x1) == CLEAR)
	{
	    U8g_Block_EMS_ECU_CAN_MSG = 1;                  /* EMS */
	}
	else
	{
	    U8g_Block_EMS_ECU_CAN_MSG = 0;                  /* EMS */
	}
	
	if(((Varient_code_arr[0] >> 2) & 0x1) == CLEAR)
	{
	    U8g_Block_ABS_ECU_CAN_MSG = 1;                  /* ABS */
	}
	else
	{
	    U8g_Block_ABS_ECU_CAN_MSG = 0;                  /*ABS*/
	}
		
	if(((Varient_code_arr[0] >> 3) & 0x1) == CLEAR)
	{
	    U8g_Block_ESC_ECU_CAN_MSG = SET;                //ESC	
	}
	else
	{
	    U8g_Block_ESC_ECU_CAN_MSG = CLEAR;              //ESC
	}
		
	if(((Varient_code_arr[0] >> 5) & 0x1) == CLEAR)
	{
	    U8g_Block_SAS_ECU_CAN_MSG = SET;  		
	}
	else
	{
	    U8g_Block_SAS_ECU_CAN_MSG = CLEAR;              //SAS
	}

	if(((Varient_code_arr[0] >> 6) & 0x1) == CLEAR)
	{
	    U8g_Block_PKE_ECU_CAN_MSG = SET; 		
	}
	else
	{
	    U8g_Block_PKE_ECU_CAN_MSG = CLEAR;               // PKE/SKM
	}
	
	if(((Varient_code_arr[0] >> 7) & 0x1) == CLEAR)
	{
	    U8g_Block_ICU_ECU_CAN_MSG = SET; 			
	}	
	else
	{
	    U8g_Block_ICU_ECU_CAN_MSG = CLEAR;                //ICU
	}
		
	if((Varient_code_arr[1] & 0x1) == CLEAR)
	{
	    U8g_Block_SRS_ECU_CAN_MSG = SET;		
	}
	else
	{
	    U8g_Block_SRS_ECU_CAN_MSG = CLEAR;                 //SRS
	}
		
	if(((Varient_code_arr[1] >> 4) & 0x1) == CLEAR)
	{
	    U8g_Block_TCU_ECU_CAN_MSG = SET; 		
	}	
	else
	{
	    U8g_Block_TCU_ECU_CAN_MSG = CLEAR;               //TCU
	}
	/**************************************************/
	
	/*******************BODY**************************/
	if((Varient_code_arr[3] & 0x1) == CLEAR)
	{
	    U8g_Block_IC_ECU_CAN_MSG = SET; 			
	}
	else
	{
	    U8g_Block_IC_ECU_CAN_MSG = CLEAR;                // IC
	}
		
	if(((Varient_code_arr[3] >> 1) & 0x1) == CLEAR)
	{
	    U8g_Block_BCM_ECU_CAN_MSG = SET; 			
	}
	else
	{
	    U8g_Block_BCM_ECU_CAN_MSG = CLEAR;                // MBFM/BCM
	}
	
	if(((Varient_code_arr[6]) & 0x1) == CLEAR)
	{
	    U8g_Block_RPAS_ECU_CAN_MSG = SET; 			
	}
	else
	{
	    U8g_Block_RPAS_ECU_CAN_MSG = CLEAR;                // RPAS
	}
		
	if(((Varient_code_arr[4] >> 5) & 0x1) == CLEAR)
	{
	    U8g_Block_IS_ECU_CAN_MSG = SET;//IS		
	}
	else
	{
	    U8g_Block_IS_ECU_CAN_MSG = CLEAR;              //IS
	}		
    }
}
