/* **********************************************************************************************************************
File                    :       batt.c
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */

#include "ADC.h"
#include "r_typedefs.h"
#include "UDS_DTC.h"
#include "UDSS.h"
#include "Memmory_Map.h"
#include "r_cg_serial_user.h"

void Battery_Condition(void);
/*
#ifdef GRAYSCALE_TFT
uint16_t LOWV_75 = 1450;
uint16_t LOWV_80 = 1565;
uint16_t HIGHV_16 = 3180;
uint16_t HIGHV_165 = 3245;
#else
*/
uint16_t LOWV_75 = 1435;
uint16_t LOWV_80 = 1541;
uint16_t HIGHV_16 = 3235;
uint16_t HIGHV_165 = 3311;
//#endif

/*For Network Monitoring*/
uint16_t LOWV_CAN_90 = 1780;
uint16_t HIGHV_CAN_95 = 1895;
uint16_t LOWV_CAN_155 = 3155;
uint16_t HIGHV_CAN_160 = 3230;

/*For Network Monitoring*/
_Bool LVSD_CAN = 1;
_Bool HVSD_CAN = 1;

uint8_t LVSD_flag;
uint8_t HVSD_flag;

uint8_t LVSD_flag_1;
uint8_t HVSD_flag_1;
uint16_t Adc1Seccntr=0;
uint16_t Adc1SeccntrON=0;


_Bool LVSD_Prev=0;
_Bool HVSD_Prev=0;
_Bool ADC_Timer1secStart=0;
_Bool ADC_Timer1secStart_ON=0;
_Bool disable_can_rx_flag = 0;
_Bool disable_can_tx_flag = 0;

_Bool BATT_LVHV_CLR_Write = 0;
_Bool BATT_LVHV_SET_Write = 0;
extern uint8_t LVHV_detect;

void Battery_Condition(void)
{
#if 0
	if(Batt_ADC_value > 3345) /* Batt_Volt > 16.5v */
	{
		LVSD_flag = 0;
		HVSD_flag = 1;
		HVSD_Prev=1;
		LVSD_Prev=0;
	}
	else if((Batt_ADC_value >= 3285 && Batt_ADC_value <= 3345) && (HVSD_Prev==1)) /* Decreasing Trend till 16 V */
	{
		LVSD_flag = 0;
		HVSD_flag = 1;
		LVSD_Prev=0;
	}
	else if(Batt_ADC_value < 1405) /* Batt_Volt < 7.5v */
	{
		LVSD_flag = 1;
		HVSD_flag = 0;
		LVSD_Prev=1;
		HVSD_Prev=0;
	}
	else if((Batt_ADC_value >= 1405 && Batt_ADC_value <= 1480) && (LVSD_Prev==1)) /* Decreasing Trend till 16 V */
	{
		LVSD_flag = 0;
		HVSD_flag = 1;
		HVSD_Prev=0;
	}
	else if((Batt_ADC_value >= 1405) && (Batt_ADC_value < 3285)) /* (Batt_Volt > 8v) && (Batt_Volt < 16v) */
	{
		LVSD_flag = 0;
		HVSD_flag = 0;
		HVSD_Prev=0;
		LVSD_Prev=0;
	}
	else
	{
		/*Noting*/
	}
#endif

#if 1

	if(Batt_ADC_value > HIGHV_165) /* Batt_Volt > 16.5v */
	{
		LVSD_flag_1 = 0;
		HVSD_flag_1 = 1;
	}
	else if(Batt_ADC_value < LOWV_75) /* Batt_Volt < 7.5v */
	{
		LVSD_flag_1 = 1;
		HVSD_flag_1 = 0;
	}
	else if((Batt_ADC_value >= LOWV_80) && (Batt_ADC_value <= HIGHV_16))  /* 8 to 16 volt*/
	{
		LVSD_flag_1 = 0;
		HVSD_flag_1 = 0;
	}
	else if((Batt_ADC_value >= LOWV_75) && (Batt_ADC_value < LOWV_80) && (LVSD_flag == 1)) /* 7.5v to 8v increasing */
	{
		LVSD_flag_1 = 1;
		HVSD_flag_1 = 0;
	}
	else if((Batt_ADC_value > HIGHV_16) && (Batt_ADC_value <= HIGHV_165) && (HVSD_flag == 1))  /* 16v to 16.5v* Decreasing Trend*/
	{
		LVSD_flag_1 = 0;
		HVSD_flag_1 = 1;
	}
	else if((Batt_ADC_value >= LOWV_75) && (Batt_ADC_value < LOWV_80) && (HVSD_flag == 0) && (LVSD_flag == 0))  /* 7.5v to 8v Decreasing */
	{
		LVSD_flag_1 = 0;
		HVSD_flag_1 = 0;
	}
	else if((Batt_ADC_value > HIGHV_16) && (Batt_ADC_value < HIGHV_165) && (HVSD_flag == 0) && (LVSD_flag == 0))   /* 16v to 16.5v  Increasing*/
	{
		LVSD_flag_1 = 0;
		HVSD_flag_1 = 0;
	}

	if(LVSD_flag_1==1 || HVSD_flag_1==1)
	{
		ADC_Timer1secStart=1;
		ADC_Timer1secStart_ON=0;
		BATT_LVHV_CLR_Write = 0;
	}
	else if(LVSD_flag_1==0 && HVSD_flag_1==0)
	{
		ADC_Timer1secStart=0;
		ADC_Timer1secStart_ON=1;
		BATT_LVHV_SET_Write = 0;
	}

#endif

	if(ADC_Timer1secStart==1)
	{
		Adc1Seccntr++;
		Adc1SeccntrON=0;
		if(Adc1Seccntr >= 10)
		{
			if(LVSD_flag_1==1)
			{
				LVSD_flag=1;
				Adc1Seccntr=11;
				LVHV_detect = 0x55;
				if(BATT_LVHV_SET_Write == 0)
				{
					BATT_LVHV_SET_Write = 1;
					eeprom_write(LVHV_Detect_OFFSET,LVHV_detect);
				}
			}
			else if(HVSD_flag_1==1)
			{
				HVSD_flag=1;
				Adc1Seccntr=11;
				LVHV_detect = 0x55;
				if(BATT_LVHV_SET_Write == 0)
				{
					BATT_LVHV_SET_Write = 1;
					eeprom_write(LVHV_Detect_OFFSET,LVHV_detect);
				}
			}
		}
	}
	else if(ADC_Timer1secStart_ON==1)
	{
		Adc1SeccntrON++;
		Adc1Seccntr=0;
		if(Adc1SeccntrON >= 10)
		{
			if(LVSD_flag_1==0)
			{
				LVSD_flag=0;
				Adc1SeccntrON=11;
				LVHV_detect = 0x11;
				if(BATT_LVHV_CLR_Write==0)
				{
					BATT_LVHV_CLR_Write = 1;
					eeprom_write(LVHV_Detect_OFFSET,LVHV_detect);
				}
			}
			if(HVSD_flag_1==0)
			{
				HVSD_flag=0;
				Adc1SeccntrON=11;
				LVHV_detect = 0x11;
				if(BATT_LVHV_CLR_Write==0)
				{
					BATT_LVHV_CLR_Write = 1;
					eeprom_write(LVHV_Detect_OFFSET,LVHV_detect);
				}
			}
		}

	}
	else
	{
		ADC_Timer1secStart=0;
		Adc1Seccntr=0;
		Adc1SeccntrON=0;
		LVSD_flag=0;
		HVSD_flag=0;
	}

	if(Batt_ADC_value < LOWV_CAN_90)/* Batt_Volt < 9.0v */
	{
		LVSD_CAN = 1;
		HVSD_CAN = 0;
	}
	else if((Batt_ADC_value > LOWV_CAN_90)&&(Batt_ADC_value < HIGHV_CAN_95)&&(LVSD_CAN == 1))
		/* (Batt_Volt > 9.0v)&&(Batt_Volt < 9.5v)&&(LVSD_CAN == SET) */
	{
		LVSD_CAN = 1;
		HVSD_CAN = 0;
	}
	else if((Batt_ADC_value > HIGHV_CAN_95)&&(Batt_ADC_value < LOWV_CAN_155))/*(Batt_Volt > 9.5v)&&(Batt_Volt < 15.5v)*/
	{
		LVSD_CAN = 0;
		HVSD_CAN = 0;
	}
	else if(Batt_ADC_value > HIGHV_CAN_160)/* Batt_Volt > 16.0v */
	{
		LVSD_CAN = 0;
		HVSD_CAN = 1;
	}
	else if((Batt_ADC_value > LOWV_CAN_155)&&(Batt_ADC_value < HIGHV_CAN_160)&&(HVSD_CAN == 1))/* Batt_Volt < 15.5v */
	{
		LVSD_CAN = 0;
		HVSD_CAN = 1;
	}
#if 1
	if(LVSD_flag == 1)
	{
		voltage_Failure_DTC_Set(DTC_LVSD_A000);
		voltage_Failure_DTC_Clear(DTC_HVSD_A000);
		disable_can_rx_flag = 1;
		disable_can_tx_flag = 1;
	}
	else if(HVSD_flag == 1)
	{
		voltage_Failure_DTC_Set(DTC_HVSD_A000);
		voltage_Failure_DTC_Clear(DTC_LVSD_A000);
		disable_can_rx_flag = 1;
		disable_can_tx_flag = 1;
	}
	else if((HVSD_flag == 0) && (LVSD_flag == 0))
	{
		voltage_Failure_DTC_Clear(DTC_HVSD_A000);
		voltage_Failure_DTC_Clear(DTC_LVSD_A000);
		disable_can_rx_flag = 0;
		disable_can_tx_flag = 0;
	}
#endif
}

