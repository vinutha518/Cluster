
/* =========================================================================
 Includes
============================================================================ */
#include "init.h"
#include "r_typedefs.h"
#include "digitalGuage.h"
#include "UDSS.h"
#include "my_can.h"
#include "batt.h"
#include "ADC.h"
#include "Memmory_Map.h"
#include "r_cg_serial_user.h"
#include "gui.h"
#include "drome.h"
#include "druserconf.h"
#include "UDSS_APP.h"
#include "UDS_DTC.h"

void Get_Default_ADCvalue(void);
void Display_Fuel_Bar(void);
uint8_t Current_Fuel_Bar(void);
void ADC_Avg(void);
void Fuel_Init(void);
void IGN_mode(void);

uint8_t fuel_value=0;
uint8_t pre_fuel_val=0;
uint8_t new_fuel_val=0;
uint8_t fuel_widget = 0U;
uint8_t Present_level = 0U;
uint8_t displayed_fuel_level = 0U;
uint8_t New_fuel_level = 0U;
uint8_t recovery_fuel_level = 0U;
uint8_t current_fuel_in_spd = 0;
uint8_t current_fuel_in_hlt = 0;
uint8_t fuel_segment = 0;

uint32_t FLOAT_RESISTANCE_RAW;
uint32_t FLOAT_RESISTANCE_FILTER;
uint32_t FUEL_LVL_RAW;
uint8_t FLOAT_SENSOR_RESISTANCE_RAW;
uint8_t FLOAT_SENSOR_RESISTANCE_RAW_AVG;//Raw resistance level
uint32_t FUEL_LEVEL_FOR_DTE;
uint32_t FUEL_LEVEL_RAW;
uint32_t Currunt_Fuel_level;
uint32_t Referance_Fuel_level;
extern uint32_t FUEL_LEVEL_RAW_ADC;
uint8_t damping_per_liter;
_Bool read_fuel = 0;

uint8_t  cnt  = 0;
uint8_t  cnt2 = 0;
uint8_t  array[8] = {0,0,0,0,0,0,0,0};
uint8_t  current_fuel_bar = 0;
uint16_t New_fuel_read_ad1 = 0; /* after adding this file in vlib folder need to delete this variable */
uint16_t New_fuel_read_avg = 0;

uint8_t	bar_cnt;
uint32_t Fuel_ml;
uint32_t Fuel_ml_DTE;
uint8_t fuel_error = 0;
uint8_t fuel_fault = 0;
_Bool fuel_fault_30sec = 0;
uint8_t from_recovery = 0;
uint8_t fuel_fault_TT_blink = 0;

uint16_t Fuel_level_Max[8] = {2934,2701,2453,2175,1843,1484,1184,882};	/* Fuel Bar OFF ADC count */
uint16_t Fuel_level_Min[8] = {3044,2813,2550,2305,1974,1658,1305,1021}; /* Fuel Bar ON ADC count */

uint8_t reference_level = 0;
uint8_t ADC_Read_Flag = 0;
uint8_t ADC_Read_300msec_Flag = 0;
uint8_t delta_flag_1msec_Flag = 0;
uint8_t wait_13_sec_flag = 0;

uint32_t Counter_30sec_Flag = 0;
uint8_t Counter_10sec_Flag = 0;//r
uint8_t Counter_5min_Flag = 0;//
uint8_t Counter_5min_spd_flag = 0;//r
uint8_t Counter_13sec_Flag = 0;//
uint8_t Counter_13sec_ign_Flag = 0;
uint8_t Counter_5sec_Flag = 0;//r
_Bool Counter_FF_5sec_Flag = 0;
uint8_t Recovery_Counter_10sec_Flag = 0;//set after Open/short Recovery
uint8_t fast_drain_start = 0;
uint8_t Counter_300msec_Flag = 0;
_Bool Counter_330msec_Flag = 0;
/*Fast fill new delay*/
uint8_t Per_Min_Counter_Flag = 0;
uint8_t Counter_3min_Flag = 0;

uint32_t Timer_ON_30sec = 0;
uint8_t Timer_ON_10sec = 0;
uint8_t Timer_ON_13sec = 0;//
uint8_t Timer_ON_13sec_ign = 0;//
uint8_t Timer_ON_5sec = 0;//r
_Bool Timer_fastfill_5sec = 0;
uint8_t Timer_recovery_ON_10sec = 0;//Start after Recovery
uint8_t Timer_ON_300msec = 0;//
uint8_t Timer_ON_100msec = 0;//
uint8_t Timer_ON_5min = 0;//
uint8_t Timer_ON_5min_spd = 0;//r
uint8_t Timer_ON_fast_drain = 0;//r
/*Fast fill new delay*/
uint8_t Timer_ON_Per_Liter = 0;
uint32_t Per_Minute_count;
uint8_t Timer_ON_3min = 0;
_Bool fast_drain = 0;	//Enter in Fast drain flag
_Bool Fast_fill_Started = 0;// For DTE
static uint8_t speed_flag = 1;//r
static uint8_t IGN_STATUS = 1;//r for memory read cycle
_Bool Fuel_init_done = 0;
uint8_t go_to_sleep;


uint8_t u8_DTE_Fuel_En = 0U;
uint16_t ADC_Array1[10] = {0};
uint16_t ADC_Array2[10] = {0};

_Bool fuel_level_copy = 0;
uint8_t Ign_bar_check = 0;//for IGN on fuel update logic
uint8_t fuel_level_copy_counter = 10;	//to copy first time fuel level in to Currunt_Fuel_level
/*************************************************/
extern uint8_t empty_blinkpattern_flag;
extern uint8_t full_blinkpattern_flag;
extern uint8_t toggle_fuel;
extern uint8_t IGN_ON_fuel;
extern _Bool update_fuel_Bar;//for test
extern uint8_t IGN_ON_for_fuel;
extern _Bool Flag_AFE_reset;
extern _Bool Flag_AFE_reset_trigger;
extern uint8_t IGN_toggle_flag;
/*************************************************/
_Bool Fuel_open_short_chk = 1;
uint8_t shortckt__30sec_start = 0;
uint8_t recovery__30sec_start = 0;
uint8_t openckt__30sec_start = 0;
uint8_t timer_30sec_completed = 0;
uint8_t copy_ADC = 0;
#ifdef COLOR_TFT
#define slow_damping 9305//5000(21min)For W501H(Rounded of 9302.325581)per min count 232.55
#endif
#ifdef GRAYSCALE_TFT
#ifdef U350
#define slow_damping 8335//For U350 (Rounded of 8333.333333)per min count 208.33
#else
#define slow_damping 7780//7410 For W501L (Rounded of 7407.407407)per min count 185.18
#endif
#endif
uint32_t ADC_Update_Speed =	slow_damping;
uint8_t Fuel_Liter=0;
_Bool Read_Fuel_on_hlt = 0;
extern uint8_t BATT_DISCONNECTION;

extern _Bool DTE_Update;
/*************************************************/
#define Half_tank 30000
#define STEP_SIZE 0.001220703125	//(5/4096)v
#define ADC_LOOP_COUNTER  10
enum
{
	Fuel_Jump,
	Fuel_Return,
	Fuel_NONE
};
void Get_Default_ADCvalue(void)
{
	int k,m;
	float slope;
	uint16_t ADC_Count;
	for(k=0;k<8;k++)
	{
		Fuel_level_Max[k] = DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[k];
	}

	m=8;
	for(k=0;k<8;k++)
	{
		Fuel_level_Min[k] = DID_FUEL_BAR_VALUE_CONFIG_0x050B_arr[m];
		m++;
	}

	for(k=0;k<8;k++)
	{
		if((Fuel_level_Max[k] >= 10) && (Fuel_level_Max[k] <= 25))
		{
			slope = 0.02649;
		}
		else if((Fuel_level_Max[k] >= 26) && (Fuel_level_Max[k] <= 45))
		{
			slope = 0.02936;
		}
		else if((Fuel_level_Max[k] >= 46) && (Fuel_level_Max[k] <= 65))
		{
			slope = 0.0323;
		}
		else if((Fuel_level_Max[k] >= 66) && (Fuel_level_Max[k] <= 85))
		{
			slope = 0.0351;
		}
		else if((Fuel_level_Max[k] >= 86) && (Fuel_level_Max[k] <= 105))
		{
			slope = 0.038;
		}
		else if((Fuel_level_Max[k] >= 106) && (Fuel_level_Max[k] <= 125))
		{
			slope = 0.0409;
		}
		else if((Fuel_level_Max[k] >= 126) && (Fuel_level_Max[k] <= 145))
		{
			slope = 0.0438;
		}
		else if((Fuel_level_Max[k] >= 146) && (Fuel_level_Max[k] <= 165))
		{
			slope = 0.0467;
		}
		else if((Fuel_level_Max[k] >= 166) && (Fuel_level_Max[k] <= 180))
		{
			slope = 0.0495;
		}

		//Fuel_level_Max[k] = Fuel_level_Max[k]/slope;

//		ADC_Count =   - (0.0897*Fuel_level_Max[k]*Fuel_level_Max[k]) + (34.683*Fuel_level_Max[k]) - 37.909;
//		Fuel_level_Max[k] = ADC_Count;

/*y = -0.0833x2 + 32.739x + 186.15*/
#ifdef U350
		ADC_Count = -(0.0731*Fuel_level_Max[k]*Fuel_level_Max[k]) + (31.351*Fuel_level_Max[k]) + 223.95;//U350 Updated table
#else
		ADC_Count = -(0.0833* Fuel_level_Max[k]*Fuel_level_Max[k]) + (32.739*Fuel_level_Max[k]) + 186.15;//W501
#endif
		Fuel_level_Max[k] = ADC_Count;
	}

	for(k=0;k<8;k++)
	{
		if((Fuel_level_Min[k] >= 10) && (Fuel_level_Min[k] <= 25))
		{
			slope = 0.02649;
		}
		else if((Fuel_level_Min[k] >= 26) && (Fuel_level_Min[k] <= 45))
		{
			slope = 0.02936;
		}
		else if((Fuel_level_Min[k] >= 46) && (Fuel_level_Min[k] <= 65))
		{
			slope = 0.0323;
		}
		else if((Fuel_level_Min[k] >= 66) && (Fuel_level_Min[k] <= 85))
		{
			slope = 0.0351;
		}
		else if((Fuel_level_Min[k] >= 86) && (Fuel_level_Min[k] <= 105))
		{
			slope = 0.038;
		}
		else if((Fuel_level_Min[k] >= 106) && (Fuel_level_Min[k] <= 125))
		{
			slope = 0.0409;
		}
		else if((Fuel_level_Min[k] >= 126) && (Fuel_level_Min[k] <= 145))
		{
			slope = 0.0438;
		}
		else if((Fuel_level_Min[k] >= 146) && (Fuel_level_Min[k] <= 165))
		{
			slope = 0.0467;
		}
		else if((Fuel_level_Min[k] >= 166) && (Fuel_level_Min[k] <= 180))
		{
			slope = 0.0495;
		}

		//Fuel_level_Min[k] = Fuel_level_Min[k]/slope;

//		ADC_Count =   - (0.0897*Fuel_level_Min[k]*Fuel_level_Min[k]) + (34.683*Fuel_level_Min[k]) - 37.909;
//		Fuel_level_Min[k] = ADC_Count;

/*y = -0.0833x2 + 32.739x + 186.15*/

#ifdef U350
		ADC_Count = -(0.0731*Fuel_level_Min[k]*Fuel_level_Min[k]) + (31.351*Fuel_level_Min[k]) + 223.95;//U350 Updated table
#else
		ADC_Count = -(0.0833* Fuel_level_Min[k]*Fuel_level_Min[k]) + (32.739*Fuel_level_Min[k]) + 186.15;//W501
#endif
		Fuel_level_Min[k] = ADC_Count;
	}

}

/* ***********************************************************************************************
* Function Name: Display_Fuel_Bar
* Description  : This function used to display Fuel bar.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
void Display_Fuel_Bar(void)
{
	uint8_t fuel_adc_value,fu;

#if 0
	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{

			/* comparing previously displayed_fuel_level with Present_level */
			if(displayed_fuel_level > Present_level)
			{
				displayed_fuel_level--;
			}
			else if(displayed_fuel_level < Present_level)
			{
				displayed_fuel_level++;
			}

			switch(displayed_fuel_level)
			{
			case 0:
				/* All segment OFF */
				fuel_segment = 0;
				//new_fuel_val = 0;
				break;

			case 1:
				/* 1st segment ON */
				fuel_segment = 1;
				//new_fuel_val = 1;
				break;

			case 2:
				/* 1st and 2nd segment ON */
				fuel_segment = 2;
				//new_fuel_val = 2;
				break;

			case 3:
				/* 1 to 3rd segment ON */
				fuel_segment = 3;
				//new_fuel_val = 3;
				break;

			case 4:
				/* 1 to 4th segment ON */
				fuel_segment = 4;
				//new_fuel_val = 4;
				break;

			case 5:
				/* 1 to 5th segment ON */
				fuel_segment = 5;
				//new_fuel_val = 5;
				break;

			case 6:
				/* 1 to 6th segment ON */
				fuel_segment = 6;
				//new_fuel_val = 6;
				break;

			case 7:
				/* 1 to 7th segment ON */
				fuel_segment = 7;
				//new_fuel_val = 7;

				break;

			case 8:
				/* 1 to 8th segment ON */
				fuel_segment = 8;
				//new_fuel_val = 8;
				break;

			default:
				break;
			}

			/* displayed_fuel_level need to store in eeprom (use eeprom_read();)  */

		}
		else
		{
			/* display off */
			fuel_segment = 0;
			//new_fuel_val = 0;

		}
	}
	else
	{
		/* display off */
		fuel_segment = 0;
		//new_fuel_val = 0;
	}
#endif
#if 1	//to avoid stepping after IGN on
	if(IGN_ON_fuel == 1)
	{
		IGN_ON_fuel = 0;
		IGN_ON_for_fuel = 0;
		pre_fuel_val = 0;//r
		pre_fuel_val = Current_Fuel_Bar();
		fuel_widget = pre_fuel_val;
		Present_level = Current_Fuel_Bar();//r
		//current_fuel_in_spd = 1;//r
	}
#endif




#if 1
	//new_fuel_val = displayed_fuel_level;
	if(pre_fuel_val < new_fuel_val)
	{
		fuel_value = 0;
	}
	else if(pre_fuel_val > new_fuel_val)
	{
		fuel_value = 1;
	}
	else
	{
		fuel_value = 2;
	}
	if(logo_completed==1)
	{
		switch (fuel_value)
		{
		case Fuel_Jump:
//			(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
				fuel_widget++;
				pre_fuel_val++;
			break;
		case Fuel_Return:
//			(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Return);
				fuel_widget--;
				pre_fuel_val--;
			break;
		case Fuel_NONE:
		default:
			break;
		}

		if(pre_fuel_val >= 8)
		{
			pre_fuel_val = 8;
		}
		if(fuel_widget >= 8)
		{
			fuel_widget = 8;
		}
		/*
		if((New_fuel_read_ad1 >= 3505)||(New_fuel_read_ad1 <= 390))	//Open ckt and short ckt condition
		{
			//Blink all segments @ (1/6) Hz
		}
		*/
/*			Added in IGN Mode function

		if(New_fuel_read_ad1 >= 3505)	//Open ckt and short ckt condition
		{
			shortckt__30sec_start = 0;
			recovery__30sec_start = 0;
			openckt__30sec_start = 1;
			if(timer_30sec_completed == 1)
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_Open_circuit);
				empty_blinkpattern_flag = 1;							//Blink all segments @ (1/6) Hz
				full_blinkpattern_flag = 0;
				timer_30sec_completed =0;
//				openckt__30sec_start = 0;
			}
			else
				timer_30sec_completed = 0;
		}
		else if(New_fuel_read_ad1 <= 490)
		{
			openckt__30sec_start = 0;
			recovery__30sec_start = 0;
			shortckt__30sec_start = 1;
			if(timer_30sec_completed == 1)
			{
				(void)drome_sendEventToDrome((U32)drome_InEvent_Short_circuit);
				full_blinkpattern_flag = 1;
				empty_blinkpattern_flag = 0;
				timer_30sec_completed =0;
			}
			else
			{
				timer_30sec_completed = 0;
			}
		}
		else if((empty_blinkpattern_flag) ||(full_blinkpattern_flag))
		{
			openckt__30sec_start = 0;
			shortckt__30sec_start = 0;
			recovery__30sec_start = 1;

			if(timer_30sec_completed == 1)
			{
				full_blinkpattern_flag = 0;
				empty_blinkpattern_flag = 0;
				timer_30sec_completed = 0;
			}
			else
				timer_30sec_completed = 0;
		}
		else
		{
			full_blinkpattern_flag = 0;
			empty_blinkpattern_flag = 0;
		}
*/
	}

#endif

}


/* ***********************************************************************************************
* Function Name: Current_fuel_bar
* Description  : This function used to know current duel bar.
* Arguments    : None
* Return Value : None
************************************************************************************************ */
uint8_t Current_Fuel_Bar(void)
{
	if(Ign_bar_check <= 3)
	{
	    for(cnt = 0; cnt<8;cnt++)	     	                /* check for all eight values of array */
	    {
	//        if(New_fuel_read_ad1 >= Fuel_level_Min[cnt]) /* ori=<= check if <= respectv. ON values in min array value start from 8th value */
	        if(New_fuel_read_ad1 <= Fuel_level_Min[cnt])
	    	{
				array[cnt] = 1;//0
			}
	        else if(New_fuel_read_ad1 >= Fuel_level_Max[cnt])
			{
				array[cnt] = 0;//1
			}
		}
#if 0
		for(cnt = 0; cnt<8;cnt++)	     	           /* check for all eight values of array */
	    {
	//        if(New_fuel_read_ad1 <= Fuel_level_Max[cnt]) /* ori= >= check if >= respectv. OFF values in max array value start from 8th value */
	        if(New_fuel_read_ad1 >= Fuel_level_Max[cnt])
			{
				array[cnt] = 0;//1
			}
		}
#endif
		current_fuel_bar =  0;                             /* clear the previous value of Fuel bar level */
			for(cnt2 = 0;cnt2<8;cnt2++)	     	        /*leave the 0th position and update fuel bars according to 0 or 1 value in array*/
		    {
		        if(array[cnt2]==1)                          /* if value is 1 then incr. by 1*/
				{
					current_fuel_bar++;
				}
			}
		Ign_bar_check++;
	}

	else
	{
		for(cnt = 0; cnt<8;cnt++)	     	                /* check for all eight values of array */
		{
			if(New_fuel_read_ad1 >= Fuel_level_Min[cnt]) /* ori=<= check if <= respectv. ON values in min array value start from 8th value */
  //        if(New_fuel_read_ad1 <= Fuel_level_Min[cnt])
			{
				array[cnt] = 0;//1
			}
		}

		for(cnt = 0; cnt<8;cnt++)	     	           /* check for all eight values of array */
		{
			if(New_fuel_read_ad1 <= Fuel_level_Max[cnt]) /* ori= >= check if >= respectv. OFF values in max array value start from 8th value */
  //        if(New_fuel_read_ad1 >= Fuel_level_Max[cnt])
			{
				array[cnt] = 1;//0
			}
		}

		current_fuel_bar =  0;                             /* clear the previous value of Fuel bar level */
		for(cnt2 = 0;cnt2<8;cnt2++)	     	        /*leave the 0th position and update fuel bars according to 0 or 1 value in array*/
		{
			if(array[cnt2]==1)                          /* if value is 1 then incr. by 1*/
			{
				current_fuel_bar++;
			}
		}
	}
	/*
	Present_level  = current_fuel_bar;
	New_fuel_level = current_fuel_bar;*/

	return current_fuel_bar;
}


/* **********************************************************************************************************************
* Function Name: ADC_Avg
* Description  : This function gives average of 10 samples .
* Arguments    : None
* Return Value : None
********************************************************************************************************************** */
void ADC_Avg(void)
{
	uint16_t sum = 0U, sum1=0;
	uint8_t i;
	uint16_t value = 0,value1=0,value2 =0;

	for (i = 0; i < 10; i++)
	{
		sum = sum + ADC_Array1[i];  			 /*  Get AD measurement (12 bits value)  */
		sum1=sum1 + ADC_Array2[i];
	}

	value = (sum) / (10);    /* Averge of 10 samples done */
	value1 = (sum1) / (10);  /* Averge of 10 samples of ADC Vref done */

	value2 = ((4096 * value) / value1);
	if(value2 < 0)
	{
		value2 = 0;
	}
	else if(value2 > 4095)
	{
		value2 =  4095;
	}

	New_fuel_read_avg = value2; /* Store Final Avg Value of ADC */

//	if(copy_ADC < 10)
//	{
//		copy_ADC++;
//		New_fuel_read_ad1 = New_fuel_read_avg;
//	}
//	else				//to avoid copy_ADC overflow
//	{
//		copy_ADC = 11;
//	}
}

/* **********************************************************************************************************************
* Function Name: Fuel_Init
* Description  : This function Initialize fuel after IGN on .
* Arguments    : None
* Return Value : None
********************************************************************************************************************** */
void Fuel_Init(void)
{
	if(IGN_STATUS == 1)	//r		Read present level from memory one time for every IGN cycle
	{
		IGN_STATUS = 0;
		IGN_ON_fuel = 1;//test
		displayed_fuel_level = eeprom_read(FUEL_BAR_OFFSET); /* read  last fuel bar which is displayed on LCD & store in "displayed_fuel_level"  */
		//IGN_toggle_flag = eeprom_read(IGN_TOGGLE_FLAG); /* read ignition toggle flag */we've read it at IGN on only.

		//			fuel_widget = Current_Fuel_Bar();//r new_fuel_val
		Fast_fill_Started = eeprom_read(FAST_FILL_ENTRY_OFFSET);//To Update Fuel,DTE immediately if IGN toggle in Fast-fill mode
		Present_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET);
		if(Fast_fill_Started == 1)
		{
			ADC_Update_Speed = 200;
		}

#if 1
		New_fuel_read_ad1 = New_fuel_read_avg;
		New_fuel_level = Current_Fuel_Bar();

		if(displayed_fuel_level>8)
		{
			if(Present_level>8)
			{
				Present_level = New_fuel_level+1;
				displayed_fuel_level = New_fuel_level;
			}
			displayed_fuel_level = New_fuel_level;
		}


		if((BATT_DISCONNECTION != 0))
		{
			Present_level = New_fuel_level;
			fuel_widget = New_fuel_level;
			New_fuel_read_ad1 = New_fuel_read_avg;
		}
		else if(((New_fuel_level!=Present_level)||(Fast_fill_Started == 1))&&(IGN_toggle_flag==0xAA))
		{
			//if(New_fuel_level>Present_level)
			{
				DTE_Update = 1U;		//To update DTE after refuel
			}
			Present_level = New_fuel_level;
			fuel_widget = New_fuel_level;
			New_fuel_read_ad1 = New_fuel_read_avg;
		}
		else
		{
			eeprom_memread_32bytes(Filter_ADC_Count_OFFSET,&New_fuel_read_ad1,2);
			New_fuel_level = displayed_fuel_level;
			Present_level = displayed_fuel_level;
			fuel_widget = displayed_fuel_level;
			new_fuel_val = displayed_fuel_level;
			pre_fuel_val = displayed_fuel_level;

			IGN_ON_fuel = 0;//try
			IGN_ON_for_fuel = 0;//try

		}

#ifdef U350
		if(FEATURE_CODE_FUEL_TANK_SELECT == 1)//60L tank Configuration.
		{
			/*Calculated FLOAT_SENSOR_RESISTANCE_RAW, FLOAT_SENSOR_RESISTANCE_RAW_AVG For Input to Fuel level Calculation */
			FLOAT_SENSOR_RESISTANCE_RAW = (0.00001 * New_fuel_read_ad1 * New_fuel_read_ad1) + (0.0101 * New_fuel_read_ad1) + 7.2125;//(Fuel_ADC_value/10.5);
			FLOAT_SENSOR_RESISTANCE_RAW_AVG = (0.00001 * New_fuel_read_avg * New_fuel_read_avg) + (0.0101 * New_fuel_read_avg) + 7.2125;//undamped Resistance value
		/*	FUEL_LEVEL_FOR_DTE = (-(0.0125 * New_fuel_read_ad1 * New_fuel_read_ad1) + (27.688 *New_fuel_read_ad1) + 43078);
			FUEL_LEVEL_RAW = (-(0.0125 * New_fuel_read_avg * New_fuel_read_avg) + (27.688 *New_fuel_read_avg) + 43078);*/
			FUEL_LEVEL_FOR_DTE = ((-2.637*FLOAT_SENSOR_RESISTANCE_RAW*FLOAT_SENSOR_RESISTANCE_RAW)-(54.155*FLOAT_SENSOR_RESISTANCE_RAW)+61586);//damped Resistance based formula
			FUEL_LEVEL_RAW = ((-2.637*FLOAT_SENSOR_RESISTANCE_RAW_AVG*FLOAT_SENSOR_RESISTANCE_RAW_AVG)-(54.155*FLOAT_SENSOR_RESISTANCE_RAW_AVG)+61586);//10 sample avg. Resistance based formula
		}
		else if(FEATURE_CODE_FUEL_TANK_SELECT == 0)//50L tank Configuration.
		{
			if(New_fuel_read_avg>=3000)//0L to 12L	//New_fuel_read_ad1
			{
				FUEL_LEVEL_FOR_DTE = (-(0.0946*New_fuel_read_ad1*New_fuel_read_ad1)+(516.72*New_fuel_read_ad1)-687084);
				FUEL_LEVEL_RAW = (-(0.0946*New_fuel_read_avg*New_fuel_read_avg)+(516.72*New_fuel_read_avg)-687084);
			}
			else if((New_fuel_read_avg<3000)&&(New_fuel_read_avg>=2000))//13L to 36L	New_fuel_read_ad1
			{
				FUEL_LEVEL_FOR_DTE = (-(0.0046*New_fuel_read_ad1*New_fuel_read_ad1)+(0.9866*New_fuel_read_ad1)+52165);
				FUEL_LEVEL_RAW = (-(0.0046*New_fuel_read_avg*New_fuel_read_avg)+(0.9866*New_fuel_read_avg)+52165);
			}
			else if((New_fuel_read_avg<2000)&&(New_fuel_read_avg>=930))//37L-50L	New_fuel_read_ad1
			{
				FUEL_LEVEL_FOR_DTE = ((0.0027*New_fuel_read_ad1*New_fuel_read_ad1)-(17.822*New_fuel_read_ad1)+62431);
				FUEL_LEVEL_RAW = ((0.0027*New_fuel_read_avg*New_fuel_read_avg)-(17.822*New_fuel_read_avg)+62431);
			}
			else if((New_fuel_read_avg<930)&&(New_fuel_read_avg>=630))	//New_fuel_read_ad1
			{
				FUEL_LEVEL_FOR_DTE = 50000;							//showing 50L up to 15 ohm
				FUEL_LEVEL_RAW = 50000;
			}
		}
#else
		/*Real Tank values of W501*/  //y = =(-0.0044*x*x)-(5.0343*x)+62636
			FUEL_LEVEL_FOR_DTE =((-0.0044*New_fuel_read_ad1*New_fuel_read_ad1)-(5.0343*New_fuel_read_ad1)+62636);
			FUEL_LEVEL_RAW  =	((-0.0044*New_fuel_read_avg*New_fuel_read_avg)-(5.0343*New_fuel_read_avg)+62636);
	//		FUEL_LEVEL_FOR_DTE = (-(0.0004 * New_fuel_read_ad1 * New_fuel_read_ad1) - (13.591 *New_fuel_read_ad1) + 50850);
	//		FUEL_LEVEL_RAW =  (-(0.0004 * New_fuel_read_avg * New_fuel_read_avg) - (13.591 *New_fuel_read_avg) + 50850);

#endif
		Currunt_Fuel_level = FUEL_LEVEL_FOR_DTE;//try

#endif
		Fuel_init_done = 1;
	}
}


void IGN_mode(void)
{
#if 1
	uint16_t delta_flag_1msec = 0U;
	uint8_t ifuel,i,IGN_FUEL;
if((fuel_fault == 0)&&((Fuel_ADC_value >= 200 )&&(Fuel_ADC_value <= 2195)))
//if((empty_blinkpattern_flag == 0) && (full_blinkpattern_flag == 0))
{
	Fuel_open_short_chk = 0;
	if(IGN_ON_flag == 0)    /* IGN ON to OFF mode */
	{
		Timer_ON_13sec = 0;				//initialize 13 sec timer after ignition off

		//Timer_ON_100msec = 0; /* from IGN ON condition */

		displayed_fuel_level = fuel_widget;
/*Avoid bar 0 on fast IGN toggle*/
//		eeprom_write(FUEL_BAR_OFFSET, displayed_fuel_level); /* store displayed_fuel_level in memory */

		//Timer_ON_13sec = 1;
		//if(Counter_13sec_Flag == 1)   /* Allow fuel is stable for 13 sec (not implemented) */
		//{
			//Counter_13sec_Flag = 0;
		current_fuel_in_spd = 0;//r
		Read_Fuel_on_hlt = 0;
		IGN_STATUS = 1;		//r when IGN goes on to off this flag get set

//			for(ifuel=0; ifuel< ADC_LOOP_COUNTER;)
//			{
//				//Timer_ON_300msec = 1;
//
//				//if(ADC_Read_300msec_Flag == 1)  /* For every 300 msec this flag becomes ON (not implemented) */
//				//{
//					//ADC_Read_300msec_Flag = 0;
//					ADC_Array1[ifuel] = Fuel_ADC_value;
//					ADC_Array2[ifuel] = Fuel_EXVrefADC_value;
//					ifuel++;
//				//}
//			}
		Timer_ON_13sec_ign = 1;			//Start 13 sec timer after ignition off
		if(Counter_13sec_ign_Flag == 1)	//after completing 13 sec from ignition off take 10 samples of 300ms
		{
//			Counter_13sec_Flag = 0;
			Timer_ON_300msec = 1;
			ADC_Avg();

			Present_level = Current_Fuel_Bar(); /*  New_fuel_read_ad1; */

			eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level); /* store Present_level in memory */
		//go_to_sleep	= 1;			//allow cluster to go in deep sleep
		}
//r			Present_level = 0;

		//}

		//Present_level = Current_Fuel_Bar(); /*  New_fuel_read_ad1; */

		//eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level); /* store Present_level in memory */

	}
#endif

//	else if((IGN_ON_flag == 1)&&(final_vechile_speed > 0.4))//Fuel change except fuel filling condition (Exit from fast fill)
//	else if((IGN_ON_flag == 1)&&(final_vechile_speed_gauge_raw >= 0x20))//Fuel change except fuel filling condition (speed >= 0.5kmph)
	else if((IGN_ON_flag == 1)&&(final_vechile_speed >= 1))
	{
		Fast_fill_Started = 0;//Used for DTE
		Timer_ON_13sec   = 0; /* from IGN OFF condition */
		Timer_ON_300msec = 0; /* from IGN OFF condition */
//		wait_13_sec_flag = 0;
		Counter_13sec_Flag = 0;
		fast_drain = 0;
		ADC_Update_Speed = slow_damping;//10000;//5000
		Read_Fuel_on_hlt = 0;
		speed_flag = 1;	//flag get set as vehicle speed > 0.5km
		Timer_fastfill_5sec = 0;//fast fill timer off
		Counter_FF_5sec_Flag = 0;//Fast fill entry flag 0
/*added*/
		ADC_Avg();//avg of 10 samples
		Fuel_Init();
#if 0
		//this part is called from Fuel_Init function.
		if(IGN_STATUS == 1)	//r		Read present level from memory one time for every IGN cycle
		{
			IGN_STATUS = 0;
			Present_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET);
			Currunt_Fuel_level = FUEL_LEVEL_FOR_DTE;
#if 1
//			fuel_widget = Current_Fuel_Bar();//r new_fuel_val
			New_fuel_level = Current_Fuel_Bar();
			if(New_fuel_level==Present_level)
			{
				New_fuel_level = displayed_fuel_level;
				Present_level = displayed_fuel_level;
				fuel_widget = displayed_fuel_level;
				IGN_ON_fuel = 0;//Try
				IGN_ON_for_fuel = 0;//try

			}
			else
			{
				Present_level = New_fuel_level;
				fuel_widget = New_fuel_level;
			}
#endif
		}
#endif
/*code added on 18-4-19*/
		//Present_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET); /* read fuel bar from memory & store in Present_level */
		//displayed_fuel_level = eeprom_read(FUEL_BAR_OFFSET); /* read  last fuel bar which is displayed on LCD */
				                                                         /* & store in "displayed_fuel_level"  */

				//r  every time when speed goes zero to non zero

		if(displayed_fuel_level >= 8)
		{
			displayed_fuel_level = 8;
		}
		if(Present_level >= 8)
		{
			Present_level = 8;
		}

//		for(ifuel=0; ifuel< ADC_LOOP_COUNTER;)
		{
			Timer_ON_100msec = 1;
//			if(ADC_Read_Flag == 1)  /* For every 100 msec this flag becomes ON */
//			{
//				ADC_Read_Flag = 0;
//				ADC_Array1[ifuel] = Fuel_ADC_value;
//				ADC_Array2[ifuel] = Fuel_EXVrefADC_value;
//				ifuel++;
//			}
		}

		//Timer_ON_100msec = 0;
		if (from_recovery == 0)
		{
			New_fuel_level = Current_Fuel_Bar(); /*  New_fuel_read_ad1; */
		}
//		if(current_fuel_in_spd == 0 )
//		{
//			if(IGN_ON_fuel == 1)
//			{
//				IGN_ON_fuel = 0;
//				IGN_ON_for_fuel = 0;
//				pre_fuel_val = 0;//r
//				IGN_FUEL = Current_Fuel_Bar();
//				pre_fuel_val = IGN_FUEL;
//				fuel_widget = IGN_FUEL;//r
//#if 0
//				for(i = 0; i < IGN_FUEL; i++)
//				{
////					(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
//					pre_fuel_val++;
//				}
//				Present_level = Current_Fuel_Bar();//r
//#endif
//				current_fuel_in_spd = 1;//r
//			}
//			else
//			{
//				new_fuel_val = Present_level;
//				Display_Fuel_Bar();
//			}
//		}
#if 1
		if(New_fuel_level == Present_level) /* IGN=ON, Speed > 0.5km, fuel not changed */
		{
			/* display "displayed_fuel_level" */
			new_fuel_val = New_fuel_level;
			Per_Minute_count = 0;
			Per_Min_Counter_Flag = 0;
			//if(read_fuel==0)
			//{
				fuel_level_copy_counter++;
				if(fuel_level_copy_counter>=10)
				{
					Currunt_Fuel_level = FUEL_LEVEL_FOR_DTE;		//Copy fuel level in Currunt_Fuel_level
					fuel_level_copy_counter = 0;
				}
			//damping_per_liter = ((Currunt_Fuel_level - FUEL_LEVEL_FOR_DTE)/1000);
			//	if(damping_per_liter)
			//	{
			//		read_fuel = 1; //not required as Currunt_Fuel_level will update continuously as fuel level in moving vehicle vary continue
			//	}
			//}
			//new_fuel_val = displayed_fuel_level;
//			Display_Fuel_Bar();//Uncomment

/***********For 20 sec offset in Speed *******************/
//			Timer_ON_5min_spd = 1;
//			if(Counter_5min_spd_flag==1)//
			{
				Timer_ON_5min_spd = 0;//r
				Counter_5min_spd_flag = 0;
				Display_Fuel_Bar();
				from_recovery =0;
			}
/***************************************************
			Timer_ON_5min = 1;//
			if(Counter_5min_Flag==1)//
			{
				Timer_ON_5min = 0;//r
				Counter_5min_Flag = 0;
				Display_Fuel_Bar();
			}
***************************************************/
		}
		else if(New_fuel_level < Present_level)	//fuel drain
		{
			/*
			Timer_ON_5min_spd = 1;//
			if(Counter_5min_spd_flag==1)//uncomment for enable 5 min damping
			{
				Timer_ON_5min_spd = 0;//r
				Counter_5min_spd_flag = 0;
				Present_level--;//Present_level = New_fuel_level;
				eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level);//update present level in EEPROM//r
				//New_fuel_level = Current_Fuel_Bar();
				new_fuel_val = New_fuel_level;//new_fuel_val = Present_level; //////
				Display_Fuel_Bar();			//update with 5 min damping.
			}
			 */
			read_fuel = 0;						//to read fuel again
			damping_per_liter = 0;//((Currunt_Fuel_level - FUEL_LEVEL_FOR_DTE)/1000);
			switch(damping_per_liter)
			{
			case 0:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*0.5);//for fuel change less than 1 liter added 30 sec damping
				break;
			case 1:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			case 2:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			case 3:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			case 4:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			default:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*5);//in case full to empty test (40 min damping for 8 bars)
				break;
			}
			if(Per_Min_Counter_Flag == 1)
			{
				Timer_ON_Per_Liter = 0;
				Per_Min_Counter_Flag = 0;
				Present_level--;
				eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level);//update present level in EEPROM//r
				new_fuel_val = New_fuel_level;
				Display_Fuel_Bar();
			}

		}

		else if(New_fuel_level > Present_level)	//fuel fill
		{
/*
			Timer_ON_5min_spd = 1;//
			if(Counter_5min_spd_flag==1)//uncomment for enable 5 min damping
			{
				current_fuel_in_spd = 1;
				Timer_ON_5min_spd = 0;//r
				Counter_5min_spd_flag = 0;
				Present_level++;//Present_level = New_fuel_level;
				eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level);//update present level in EEPROM//r
				//New_fuel_level = Current_Fuel_Bar();
				new_fuel_val = New_fuel_level;//new_fuel_val = Present_level; //////
				Display_Fuel_Bar();			//update with 5 min damping.
			}
*/
			read_fuel = 0;						//to read fuel again
			damping_per_liter = 0;//((FUEL_LEVEL_FOR_DTE - Currunt_Fuel_level)/1000);
			switch(damping_per_liter)
			{
			case 0:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*0.5);//for fuel change less than 1 liter added 30 sec damping
				break;
			case 1:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			case 2:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			case 3:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			case 4:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*damping_per_liter);
				break;
			default:
				Timer_ON_Per_Liter = 1;
				Per_Minute_count = (470000*5);//in case empty to full test (40 min damping for 8 bars)
				break;
			}
			if(Per_Min_Counter_Flag == 1)
			{
				Timer_ON_Per_Liter = 0;
				Per_Min_Counter_Flag = 0;
				Present_level++;
				eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level);//update present level in EEPROM//r
				new_fuel_val = New_fuel_level;
				Display_Fuel_Bar();
			}

#if 0
			else if(current_fuel_in_spd == 0 )
			{
				if(IGN_ON_fuel == 1)
				{
					IGN_ON_fuel = 0;
					IGN_ON_for_fuel = 0;
					pre_fuel_val = 0;//r
					IGN_FUEL = Current_Fuel_Bar();
					pre_fuel_val = IGN_FUEL;
					fuel_widget = IGN_FUEL;//r
#if 0
					for(i = 0; i < IGN_FUEL; i++)
					{
//						(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
						pre_fuel_val++;
					}
#endif
					Present_level = Current_Fuel_Bar();//r
					current_fuel_in_spd = 1;//r
				}
				else
				{
					new_fuel_val = Present_level;
					Display_Fuel_Bar();
				}
			}
#endif
		}


		else
		{
			/* display "New_fuel_level"  */
			Present_level = New_fuel_level;
			//New_fuel_level = Current_Fuel_Bar();

			new_fuel_val = Present_level; //////
//r			Timer_ON_5min = 0;//
			Timer_ON_5min_spd = 0;//r
//			if(Counter_5min_Flag==1)//
//			{
//				Timer_ON_5min = 0;//r
//				Counter_5min_Flag = 0;
//				Display_Fuel_Bar();
//			}
		}
#endif

	}

#if 1	//With fast fill logic

//	else if((IGN_ON_flag == 1)&&(final_vechile_speed < 0.5))//Fuel change except fuel filling condition
//	else if((IGN_ON_flag == 1)&&(final_vechile_speed_gauge_raw < 0x20))//Fuel change except fuel filling condition (speed <0.5kmph)
	else if((IGN_ON_flag == 1)&&(final_vechile_speed < 1))
	{
		Timer_ON_100msec = 1;//calculate fuel ADC after every 100ms
		ADC_Avg();//need to check impact

//		New_fuel_read_ad1 = New_fuel_read_avg;//make both values same in speed 0 condition
		Fuel_Init();
#if 0
		//this part is called from Fuel_Init function.
		if(IGN_STATUS == 1)	//r		Read present level from memory one time for every IGN cycle
		{
			IGN_STATUS = 0;
//			fuel_widget = Current_Fuel_Bar();//r new_fuel_val
			Present_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET);

#if 1
			New_fuel_read_ad1 = New_fuel_read_avg;
			New_fuel_level = Current_Fuel_Bar();
			if((New_fuel_level==Present_level)&&(BATT_DISCONNECTION == 0))
			{
				eeprom_memread_32bytes(Filter_ADC_Count_OFFSET,&New_fuel_read_ad1,2);
				New_fuel_level = displayed_fuel_level;
				Present_level = displayed_fuel_level;
				fuel_widget = displayed_fuel_level;
				new_fuel_val = displayed_fuel_level;
				pre_fuel_val = displayed_fuel_level;

				IGN_ON_fuel = 0;//try
				IGN_ON_for_fuel = 0;//try

			}
			else
			{
				Present_level = New_fuel_level;
				fuel_widget = New_fuel_level;
				New_fuel_read_ad1 = New_fuel_read_avg;
			}
#ifdef U350
				if(FEATURE_CODE_FUEL_TANK_SELECT == 1)//60L tank Configuration.
				{
					FUEL_LEVEL_FOR_DTE = (-(0.0125 * New_fuel_read_ad1 * New_fuel_read_ad1) + (27.688 *New_fuel_read_ad1) + 43078);
					FUEL_LEVEL_RAW = (-(0.0125 * New_fuel_read_avg * New_fuel_read_avg) + (27.688 *New_fuel_read_avg) + 43078);
				}
				else if(FEATURE_CODE_FUEL_TANK_SELECT == 0)//50L tank Configuration.
				{
					if(New_fuel_read_ad1>=3000)//0L to 12L
					{
						FUEL_LEVEL_FOR_DTE = (-(0.0946*New_fuel_read_ad1*New_fuel_read_ad1)+(516.72*New_fuel_read_ad1)-687084);
						FUEL_LEVEL_RAW = (-(0.0946*New_fuel_read_avg*New_fuel_read_avg)+(516.72*New_fuel_read_avg)-687084);
					}
					else if((New_fuel_read_ad1<3000)&&(New_fuel_read_ad1>=2000))//13L to 36L
					{
						FUEL_LEVEL_FOR_DTE = (-(0.0046*New_fuel_read_ad1*New_fuel_read_ad1)+(0.9866*New_fuel_read_ad1)+52165);
						FUEL_LEVEL_RAW = (-(0.0046*New_fuel_read_avg*New_fuel_read_avg)+(0.9866*New_fuel_read_avg)+52165);
					}
					else if((New_fuel_read_ad1<2000)&&(New_fuel_read_ad1>=930))//37L-50L
					{
						FUEL_LEVEL_FOR_DTE = ((0.0027*New_fuel_read_ad1*New_fuel_read_ad1)-(17.822*New_fuel_read_ad1)+62431);
						FUEL_LEVEL_RAW = ((0.0027*New_fuel_read_avg*New_fuel_read_avg)-(17.822*New_fuel_read_avg)+62431);
					}
					else if((New_fuel_read_ad1<930)&&(New_fuel_read_ad1>=630))
					{
						FUEL_LEVEL_FOR_DTE = 50000;							//showing 50L up to 15 ohm
						FUEL_LEVEL_RAW = 50000;
					}
				}
#else
			FUEL_LEVEL_FOR_DTE = ((-0.0044*New_fuel_read_ad1*New_fuel_read_ad1)-(5.0343*New_fuel_read_ad1)+62636);
			FUEL_LEVEL_RAW =  ((-0.0044*New_fuel_read_avg*New_fuel_read_avg)-(5.0343*New_fuel_read_avg)+62636);

#endif
				Currunt_Fuel_level = FUEL_LEVEL_FOR_DTE;//try

#endif
		}
#endif
		if(speed_flag == 1)
		{
			Counter_13sec_Flag = 0;//r 	clear flag only after speed present to speed not present
			Timer_ON_13sec   = 1;	//start timer of 13 sec only after speed present to speed not present
			speed_flag = 0;				//every time when speed goes non zero to zero
			//New_fuel_level = Current_Fuel_Bar(); /* bar count New_fuel_read_ad1; */
			if(New_fuel_level >= 8)
			{
				New_fuel_level = 8;
			}
			//eeprom_write(FUEL_BAR_PRESENT_OFFSET, New_fuel_level); /* store Present_level in memory after 13 sec when speed <0.5 and IGN on */
		}

		else
//		Timer_ON_13sec   = 1;	//start timer of 13 sec
		Timer_ON_5min_spd = 0;	//r
//r		Counter_13sec_Flag = 0;
//r		Present_level = Current_Fuel_Bar();
		/* read fuel bar from memory & store in Present_level after IGN off(commented before)*/

//		if(Counter_13sec_Flag==1)	//After 13 sec		GG-70/1
//		{
//			Counter_13sec_Flag = 0;//r		comment it after un-commenting first if(speed_flag == 1) block
//			if(speed_flag == 1)//r
//			{
//				speed_flag = 0;				//every time when speed goes non zero to zero
////r				Timer_ON_100msec = 1;		//calculate avg of 10 samples after 13 sec
////r				ADC_Avg();					//avg of 10 samples
////r				Present_level = Current_Fuel_Bar(); /* bar count New_fuel_read_ad1; */
//
//				New_fuel_level = Current_Fuel_Bar(); /*read new fuel level  New_fuel_read_ad1; */
//
//				if(New_fuel_level >= 8)
//				{
//					New_fuel_level = 8;
//				}
//
//				eeprom_write(FUEL_BAR_PRESENT_OFFSET, New_fuel_level); /* store Present_level in memory after 13 sec when speed <0.5 and IGN on */
//			}
//		}

		if(current_fuel_in_spd == 0)//current_fuel_in_hlt
		{
			if(IGN_ON_fuel == 1)
				{
					IGN_ON_fuel = 0;
					IGN_ON_for_fuel = 0;
					pre_fuel_val = 0;//r
					IGN_FUEL = Current_Fuel_Bar();
					pre_fuel_val = IGN_FUEL;
					fuel_widget = IGN_FUEL;//r
#if 0
					for(i = 0; i < IGN_FUEL; i++)
					{
//						(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
						pre_fuel_val++;
					}
#endif
					Present_level = Current_Fuel_Bar();//r
					current_fuel_in_spd = 1;//r
				}
					else
					{
						new_fuel_val = New_fuel_level;
						pre_fuel_val = New_fuel_level;	//to avoid stepping after IGN ON
						Display_Fuel_Bar();
					}
		}

//		if(read_fuel==0)
//		{
//			Currunt_Fuel_level = FUEL_LEVEL_FOR_DTE;
//			read_fuel = 1;
//		}

		if(Counter_13sec_Flag==1)	//After 13 sec		GG-70/1
		{
			current_fuel_in_spd = 1;//current_fuel_in_hlt = 1;
			Timer_ON_100msec = 1;	//calculate avg of 10 samples after 13 sec
			ADC_Avg();//avg of 10 samples

			if(Read_Fuel_on_hlt == 0)
			{
				Read_Fuel_on_hlt = 1;//clear it in speed > 0.5
				//FUEL_LEVEL_RAW_ADC(Raw Level)		FUEL_LEVEL_RAW(Damped)
				Referance_Fuel_level = FUEL_LEVEL_RAW;//Save raw level after Speed 0 and 13 sec(FUEL_LEVEL_FOR_DTE)
				Timer_fastfill_5sec = 1;//5sec timer on
			}
			if (from_recovery == 0)
			{
				New_fuel_level = Current_Fuel_Bar(); /*read new fuel level  New_fuel_read_ad1; */
				if(Counter_FF_5sec_Flag == 1)//5 sec timer comp.			//clear 5 sec flag in speed > 0.5
				{
					if((FUEL_LEVEL_RAW > Referance_Fuel_level)&&(fast_drain == 0))//fill
					{
						Fuel_Liter = ((FUEL_LEVEL_RAW - Referance_Fuel_level)/1000);
						if(Fuel_Liter >= 5)
						{
							ADC_Update_Speed = 200;
							Fast_fill_Started = 1;				//Fast fill entry flag for DTE
						}
					}
					else if((FUEL_LEVEL_RAW < Referance_Fuel_level)&&(Fast_fill_Started == 0))
					{
						Fuel_Liter = ((Referance_Fuel_level - FUEL_LEVEL_RAW)/1000);
						if((Fuel_Liter>=10)&&(FUEL_LEVEL_RAW < Half_tank))
						{
							ADC_Update_Speed = 200;
							fast_drain = 1;
						}
					}
				}

			}
			if(New_fuel_level == Present_level) /* IGN=ON, Speed > 0.5km, fuel not changed */
			{
				/* display "displayed_fuel_level" */
				new_fuel_val = New_fuel_level;
				//new_fuel_val = displayed_fuel_level;
//	check effect after this comment				eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level);//update present level in EEPROM//r
				Timer_ON_5min = 0;//Stop and reset timer//r
				Display_Fuel_Bar();//Uncomment 		quick update
				Timer_ON_30sec = 0;	//if fuel level not changed
				Counter_5sec_Flag = 0;//r
				Counter_10sec_Flag = 0;//r
//				fast_drain = 0;
				if(from_recovery == 1)
				{
					Timer_recovery_ON_10sec = 1;
					if(Recovery_Counter_10sec_Flag == 1)
					{
						Present_level = New_fuel_level;			//to stable bar after recovery
						from_recovery = 0;
						Recovery_Counter_10sec_Flag = 0;
					}
				}
				else
				{
					Timer_recovery_ON_10sec = 0;
				}


			}
			else if(fast_drain == 1)
			//fuel fill with 1 bar delta (consider) ||((fuel drain >= 10L)&&(current fuel level < 19.5))
			{

				if (from_recovery == 0)
				{
					Timer_ON_5sec = 1;//start timer of 5 sec to check change in fuel(not delta)
					if(Counter_5sec_Flag == 1)//flag set after 20 sec
					{
						Timer_ON_5sec = 0;
						if(New_fuel_level < Present_level)	//fuel drain
						{
							Timer_ON_10sec = 1;//
							if(Counter_10sec_Flag==1)
							{
								Counter_10sec_Flag = 0;
								Present_level--;
								new_fuel_val = New_fuel_level;
								Display_Fuel_Bar();
							}
						}
					}
				}

			}
			else if(New_fuel_level < Present_level)	//fuel drain
			{
/**/
				if(FUEL_LEVEL_RAW <= Currunt_Fuel_level)
				{
					damping_per_liter = 0;//((Currunt_Fuel_level - FUEL_LEVEL_RAW)/1000);
					switch(damping_per_liter)
					{
					case 0:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*0.50);//for fuel change less than 1 liter added 30 sec damping
						break;
					case 1:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					case 2:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					case 3:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					case 4:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					default:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*5);//in case not enter in fast drain (5 min damping per bar)
						break;
					}
				}
				if(Per_Min_Counter_Flag == 1)
				{
					Timer_ON_Per_Liter = 0;
					Per_Min_Counter_Flag = 0;
					Present_level--;
					new_fuel_val = New_fuel_level;
					Display_Fuel_Bar();
				}
/**/

			}
//r			eeprom_write(FUEL_BAR_PRESENT_OFFSET, Present_level);//update present level in EEPROM

			/*******************************GG-70/2 part b (New logic)******************************/
			else if(New_fuel_level > Present_level)					//fuel fill
			{
				/*Fuel Change (Fill) with less than 5L*/
				read_fuel = 0;						//to read fuel again
				if(FUEL_LEVEL_RAW >= Currunt_Fuel_level)
				{
					damping_per_liter = 0;//((FUEL_LEVEL_RAW - Currunt_Fuel_level)/1000);
					switch(damping_per_liter)
					{
					case 0:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*0.16);//for fuel change less than 1 liter added 30 sec damping
						break;
					case 1:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					case 2:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					case 3:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					case 4:
						Timer_ON_Per_Liter = 1;
						Per_Minute_count = (470000*damping_per_liter);
						break;
					default:

						Timer_ON_Per_Liter = 0;//disable per liter damping
						Timer_ON_5sec = 1;//start timer of 5 sec to check change in fuel(not delta)
						if(Counter_5sec_Flag == 1)//flag set after 20 sec
						{
							Timer_ON_5sec = 0;
							if(New_fuel_level == Present_level)
							{
								Display_Fuel_Bar();//display fuel level quickly
								Counter_5sec_Flag = 0;
							}
							else if(New_fuel_level > Present_level)
							{
								Timer_ON_10sec = 1;//
	//r							Counter_30sec_Flag = 1;
								if(Counter_10sec_Flag==1)
								{
									Counter_10sec_Flag = 0;
									Present_level++;
									new_fuel_val = New_fuel_level;
									Display_Fuel_Bar();
								}
							}
						}
						break;
					}
				}
				if(Per_Min_Counter_Flag == 1)
				{
					Timer_ON_Per_Liter = 0;
					Per_Min_Counter_Flag = 0;
					Present_level++;
					new_fuel_val = New_fuel_level;
					Display_Fuel_Bar();
				}

				/**************************************/
/*				if((New_fuel_level == 1 && Present_level == 0)||(New_fuel_level == 2 && Present_level == 1))//4 min to upadate bar
				{
					Timer_ON_4min = 1;
					if(Counter_4min_Flag == 1)//flag set after 4 min
					{
						Timer_ON_4min = 0;
						Counter_4min_Flag = 0;
						Present_level++;
						new_fuel_val = New_fuel_level;
						Display_Fuel_Bar();
					}
				}
				else if((New_fuel_level == 3 && Present_level == 2)||(New_fuel_level == 5 && Present_level == 4))//3 min to upadate bar
				{
					Timer_ON_3min = 1;
					if(Counter_3min_Flag == 1)//flag set after 3 min
					{
						Timer_ON_3min = 0;
						Counter_3min_Flag = 0;
						Present_level++;
						new_fuel_val = New_fuel_level;
						Display_Fuel_Bar();
					}
				}
				else
				{
*/					//used 20 sec insted of 30 sec b'coz program takes 10sec extra to set
				  if(AFE_selected == 3)
				  {
					  Flag_AFE_reset_trigger = 1;	/*After Refueling AFE ready to reset*/
						//Flag_AFE_reset = 1;
				  }
/*					moved to default case
  					Timer_ON_5sec = 1;//start timer of 5 sec to check change in fuel(not delta)
					if(Counter_5sec_Flag == 1)//flag set after 20 sec
					{
						Timer_ON_5sec = 0;
						if(New_fuel_level == Present_level)
						{
							Display_Fuel_Bar();//display fuel level quickly
							Counter_5sec_Flag = 0;
						}
						else if(New_fuel_level > Present_level)
						{
							Timer_ON_10sec = 1;//
//r							Counter_30sec_Flag = 1;
							if(Counter_10sec_Flag==1)
							{
								Counter_10sec_Flag = 0;
								Present_level++;
								new_fuel_val = New_fuel_level;
								Display_Fuel_Bar();
							}
						}
					}
*/
//				}//RR
//r					Counter_30sec_Flag = 0;
			}
		}
	}

	if((Flag_AFE_reset_trigger == 1)&&(vehicle_moving_state == 1))
	{
		Flag_AFE_reset = 1;/*After refueling AFE if vehicle in moving state AFE get reset*/
		Flag_AFE_reset_trigger = 0;
	}
}

	if((IGN_ON_flag == 1)&&((Fuel_ADC_value < 200 )||(Fuel_ADC_value > 2195)))// Short/Open ckt detection		170/1980
	{
		Fast_fill_Started = 0;//used for DTE
		recovery_fuel_level = eeprom_read(FUEL_BAR_PRESENT_OFFSET);		//ref level for recovery
		fuel_fault = 1;

		if(Fuel_ADC_value > 2195)	//Open ckt and short ckt condition
		{
			shortckt__30sec_start = 0;
			recovery__30sec_start = 0;
			openckt__30sec_start = 1;
			if((timer_30sec_completed == 1)||(Fuel_open_short_chk == 1))
			{
				Fuel_open_short_chk = 0;
				fuel_fault_TT_blink=1;
				fuel_fault_30sec = 1;
				IGN_ON_fuel = 0;
				openckt__30sec_start = 0;//r
#if 0
				for(i = 0; i < pre_fuel_val; i++)//8=pre_fuel_val
				{
//					(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Return);
					//pre_fuel_val--;
					if(pre_fuel_val < 0)
					{
						pre_fuel_val = 0;
					}
				}
#endif
				//open DTC
				Failure_DTC_Set(DTC_FUEL_OC_FAIL_A004);

				(void)drome_sendEventToDrome((U32)drome_InEvent_Open_circuit);
				empty_blinkpattern_flag = 1;							//Blink all segments @ (1/6) Hz
				full_blinkpattern_flag = 0;
				timer_30sec_completed =0;
				fuel_error = 1;
//				openckt__30sec_start = 0;
			}
			else
				timer_30sec_completed = 0;
		}
		else if(Fuel_ADC_value < 200 )
		{
			openckt__30sec_start = 0;
			recovery__30sec_start = 0;
			shortckt__30sec_start = 1;
			if((timer_30sec_completed == 1)||(Fuel_open_short_chk == 1))
			{
				Fuel_open_short_chk = 0;
				fuel_fault_TT_blink=1;
				fuel_fault_30sec = 1;
				IGN_ON_fuel = 0;
				shortckt__30sec_start = 0;//r
//				for(i = 0; i < pre_fuel_val; i++)//8=pre_fuel_val
//				{
//					(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Return);
//					//pre_fuel_val--;
//					if(pre_fuel_val < 0)
//					{
//						pre_fuel_val = 0;
//					}
#if 0
				for(i = pre_fuel_val; i < 8; i++)//8=pre_fuel_val
					{
//					(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
					//pre_fuel_val++;
					if(pre_fuel_val > 8)
					{
						pre_fuel_val = 8;
					}
				}
#endif
				//Short DTC
				Failure_DTC_Set(DTC_FUEL_SC_FAIL_A004);
				(void)drome_sendEventToDrome((U32)drome_InEvent_Short_circuit);
				full_blinkpattern_flag = 1;
				empty_blinkpattern_flag = 0;
				timer_30sec_completed =0;
				fuel_error = 1;
			}
			else
			{
				timer_30sec_completed = 0;
			}
		}
		else
		{
			fuel_fault = 0;
			fuel_fault_30sec = 0;
		}
/*		else if((empty_blinkpattern_flag) ||(full_blinkpattern_flag))
		{
			openckt__30sec_start = 0;
			shortckt__30sec_start = 0;
			recovery__30sec_start = 1;

			if(timer_30sec_completed == 1)
			{
				full_blinkpattern_flag = 0;
				empty_blinkpattern_flag = 0;
				timer_30sec_completed = 0;

				if( fuel_error == 1)
				{
					fuel_error = 0;
					pre_fuel_val = 0;//r

					for(i = 0; i < recovery_fuel_level; i++)
					{
						(void)drome_sendEventToDrome((U32)drome_InEvent_Fuel_Jump);
						pre_fuel_val++;
					}
					Present_level = Current_Fuel_Bar();//r

				}
//				fuel_error = 0;//	flag to send failsafe (0xffff) value on CAN
			}
			else
				timer_30sec_completed = 0;
		}
		else
		{
			full_blinkpattern_flag = 0;
			empty_blinkpattern_flag = 0;
		}
		*/
	}
	else if((empty_blinkpattern_flag) ||(full_blinkpattern_flag))	//Fault recovery (after blinking)
	{
		openckt__30sec_start = 0;
		shortckt__30sec_start = 0;
		recovery__30sec_start = 1;
/*****Recover at new level****/
		Timer_ON_100msec = 1;//calculate fuel ADC after every 100ms
		ADC_Avg();
		recovery_fuel_level = Current_Fuel_Bar();//test read only
/*****/
		if(timer_30sec_completed == 1)
		{
			fuel_fault_TT_blink=0;
//			full_blinkpattern_flag = 0;
//			empty_blinkpattern_flag = 0;
			recovery__30sec_start = 0;//r
			timer_30sec_completed = 0;
			if( fuel_error == 1)
			{
				fuel_error = 0;
//				pre_fuel_val = 0;//r
//				recovery_fuel_level = Current_Fuel_Bar();
				if(empty_blinkpattern_flag == 1)
				{
					/*New Logic*/
					New_fuel_read_ad1 = New_fuel_read_avg;
					New_fuel_level = Current_Fuel_Bar();
					Present_level = New_fuel_level;
					fuel_widget = New_fuel_level;
					pre_fuel_val = New_fuel_level;//modified for Open Short correct recovery
/*
					Added new logic for recovery (Same as Battery disconnect)
					pre_fuel_val = Current_Fuel_Bar();//recovery_fuel_level;//0;//r
					fuel_widget =  Current_Fuel_Bar();//recovery_fuel_level;//r
					New_fuel_level =  Current_Fuel_Bar();//recovery_fuel_level;//r1
					Present_level =  Current_Fuel_Bar();//New_fuel_level;//r1
*/
					empty_blinkpattern_flag = 0;
					fuel_fault = 0;
					fuel_fault_30sec = 0;
					//clear DTC
					Failure_DTC_Clear(DTC_FUEL_OC_FAIL_A004);
					Failure_DTC_Clear(DTC_FUEL_SC_FAIL_A004);
				}
				else if(full_blinkpattern_flag == 1)
				{
					/*New Logic*/
					New_fuel_read_ad1 = New_fuel_read_avg;
					New_fuel_level = Current_Fuel_Bar();
					Present_level = New_fuel_level;
					fuel_widget = New_fuel_level;
					pre_fuel_val = New_fuel_level;//modified for Open Short correct recovery
/*
					Added new logic for recovery (Same as Battery disconnect)
					pre_fuel_val =  Current_Fuel_Bar();//recovery_fuel_level;//8;//r
					fuel_widget =  Current_Fuel_Bar();//recovery_fuel_level;//r
					New_fuel_level =  Current_Fuel_Bar();//recovery_fuel_level;//r1
					Present_level =  Current_Fuel_Bar();//New_fuel_level;//r1
*/
					full_blinkpattern_flag = 0;
					fuel_fault = 0;
					fuel_fault_30sec = 0;

					//clear DTC
					Failure_DTC_Clear(DTC_FUEL_OC_FAIL_A004);
					Failure_DTC_Clear(DTC_FUEL_SC_FAIL_A004);
				}
			}
			from_recovery = 1;
//				fuel_error = 0;//	flag to send failsafe (0xffff) value on CAN
		}
		else
			timer_30sec_completed = 0;
	}
	else
	{
		fuel_fault_TT_blink=0;
		fuel_fault = 0;// to avoid fuel stuck from 2-3 or 3-2
		fuel_fault_30sec = 0;
		shortckt__30sec_start = 0;//clear all timer if fuel fault not present
		recovery__30sec_start = 0;
		openckt__30sec_start = 0;

	}

#endif
/*CAN MSG OUTPUT*/
//	FLOAT_RESISTANCE_RAW = STEP_SIZE*Fuel_ADC_value;//Raw count
//	FLOAT_RESISTANCE_FILTER = STEP_SIZE*New_fuel_read_ad1;//Filter count



	//Generated formula for ADC to resistance value= (y=0.00001x2 + 0.0101x + 7.2125)
	/*Insert Fuel value in ml*/

		if(fuel_error == 1)
		{
			Fuel_ml = 65535;
			Fuel_ml_DTE =  65535;
			FLOAT_SENSOR_RESISTANCE_RAW = 0xFF;// send Fail safe 255 on CAN
			FLOAT_RESISTANCE_RAW = 0x3FF;//Raw count
			FLOAT_RESISTANCE_FILTER = 0x3FF;//Filter count
		}
		else
		{
//			Fuel_ml = -(0.0004*New_fuel_read_ad1*New_fuel_read_ad1) - (13.591*New_fuel_read_ad1) + 50850;
			bar_cnt	= Current_Fuel_Bar();
#ifdef U350
			if(FEATURE_CODE_FUEL_TANK_SELECT == 1)//60L tank Configuration.
			{
			/*	FUEL_LEVEL_FOR_DTE = (-(0.0125 * New_fuel_read_ad1 * New_fuel_read_ad1) + (27.688 *New_fuel_read_ad1) + 43078);
				FUEL_LEVEL_RAW = (-(0.0125 * New_fuel_read_avg * New_fuel_read_avg) + (27.688 *New_fuel_read_avg) + 43078);*/
				FUEL_LEVEL_FOR_DTE = ((-2.637*FLOAT_SENSOR_RESISTANCE_RAW*FLOAT_SENSOR_RESISTANCE_RAW)-(54.155*FLOAT_SENSOR_RESISTANCE_RAW)+61586);//damped Resistance based formula
				FUEL_LEVEL_RAW = ((-2.637*FLOAT_SENSOR_RESISTANCE_RAW_AVG*FLOAT_SENSOR_RESISTANCE_RAW_AVG)-(54.155*FLOAT_SENSOR_RESISTANCE_RAW_AVG)+61586);//10 sample avg. Resistance based formula
			}
			else if(FEATURE_CODE_FUEL_TANK_SELECT == 0)//50L tank Configuration.
			{
				if(New_fuel_read_avg>=3000)//0L to 12L	//New_fuel_read_ad1
				{
					FUEL_LEVEL_FOR_DTE = (-(0.0946*New_fuel_read_ad1*New_fuel_read_ad1)+(516.72*New_fuel_read_ad1)-687084);
					FUEL_LEVEL_RAW = (-(0.0946*New_fuel_read_avg*New_fuel_read_avg)+(516.72*New_fuel_read_avg)-687084);
				}
				else if((New_fuel_read_avg<3000)&&(New_fuel_read_avg>=2000))//13L to 36L	//New_fuel_read_ad1
				{
					FUEL_LEVEL_FOR_DTE = (-(0.0046*New_fuel_read_ad1*New_fuel_read_ad1)+(0.9866*New_fuel_read_ad1)+52165);
					FUEL_LEVEL_RAW = (-(0.0046*New_fuel_read_avg*New_fuel_read_avg)+(0.9866*New_fuel_read_avg)+52165);
				}
				else if((New_fuel_read_avg<2000)&&(New_fuel_read_avg>=930))//37L-50L	//New_fuel_read_ad1
				{
					FUEL_LEVEL_FOR_DTE = ((0.0027*New_fuel_read_ad1*New_fuel_read_ad1)-(17.822*New_fuel_read_ad1)+62431);
					FUEL_LEVEL_RAW = ((0.0027*New_fuel_read_avg*New_fuel_read_avg)-(17.822*New_fuel_read_avg)+62431);
				}
				else if((New_fuel_read_avg<930)&&(New_fuel_read_avg>=630))	//New_fuel_read_ad1
				{
					FUEL_LEVEL_FOR_DTE = 50000;							//showing 50L up to 15 ohm
					FUEL_LEVEL_RAW = 50000;
				}
			}

			switch (bar_cnt)
			{
			case 0:
				Fuel_ml = 4000;
				break;
			case 1:
				Fuel_ml = 12000;
				break;
			case 2:
				Fuel_ml = 20000;
				break;
			case 3:
				Fuel_ml = 28000;
				break;
			case 4:
				Fuel_ml = 33000;
				break;
			case 5:
				Fuel_ml = 41000;
				break;
			case 6:
				Fuel_ml = 49000;
				break;
			case 7:
				Fuel_ml = 54000;
				break;
			case 8:
				Fuel_ml = 59000;
				break;
			default:
				break;
			}
#else
			FUEL_LEVEL_FOR_DTE = ((-0.0044*New_fuel_read_ad1*New_fuel_read_ad1)-(5.0343*New_fuel_read_ad1)+62636);
			FUEL_LEVEL_RAW =  ((-0.0044*New_fuel_read_avg*New_fuel_read_avg)-(5.0343*New_fuel_read_avg)+62636);

			switch (bar_cnt)
			{
			case 0:
				Fuel_ml = 4000;
				break;
			case 1:
				Fuel_ml = 8000;
				break;
			case 2:
				Fuel_ml = 12000;
				break;
			case 3:
				Fuel_ml = 15000;
				break;
			case 4:
				Fuel_ml = 20000;
				break;
			case 5:
				Fuel_ml = 23000;
				break;
			case 6:
				Fuel_ml = 29000;
				break;
			case 7:
				Fuel_ml = 34000;
				break;
			case 8:
				Fuel_ml = 39000;
				break;
			default:
				break;
			}
#endif
			Fuel_ml_DTE = FUEL_LEVEL_FOR_DTE;
			FLOAT_SENSOR_RESISTANCE_RAW = (0.00001 * New_fuel_read_ad1 * New_fuel_read_ad1) + (0.0101 * New_fuel_read_ad1) + 7.2125;//(Fuel_ADC_value/10.5);
			FLOAT_SENSOR_RESISTANCE_RAW_AVG = (0.00001 * New_fuel_read_avg * New_fuel_read_avg) + (0.0101 * New_fuel_read_avg) + 7.2125;//undamped Resistance value
			FLOAT_RESISTANCE_RAW = (Fuel_ADC_value/4);//Raw count
			FUEL_LVL_RAW = Fuel_ml;//Fuel level Raw
			FLOAT_RESISTANCE_FILTER = (New_fuel_read_ad1/4);//Filter count
		}
		u8_DTE_Fuel_En = 1U;

}
/********************************************************************************
								Reference logic
		raw_adc_count		FUEL_LEVEL_RAW_ADC			Pure raw
		New_fuel_read_avg	FUEL_LEVEL_RAW				Avg of 10 Count(Internal Calculation Fuel Level)
		New_fuel_read_ad1	FUEL_LEVEL_FOR_DTE			Damped with Bar time

********************************************************************************/
