
#include "r_typedefs.h"
#include "dr7f701461.dvf.h"
#include "stdlib.h"
#include "my_tell_tale.h"
#include "r_tick_api.h"
//#include "sgui.h"

void my_tell_tale_init(void)
{
/*
	// SFMA CLK P21_0
	PORT_ISOPMC21_0 = 0;
	PORT_ISOPM21_0 = 0;
	PORT_ISOP21_0 = 0;
	
	// SFMA CS P21_1
	PORT_ISOPMC21_1 = 0;
	PORT_ISOPM21_1 = 0;
	PORT_ISOP21_1 = 0;
	
	// SFMA data0 P21_2
	PORT_ISOPMC21_2 = 0;
	PORT_ISOPM21_2 = 0;
	PORT_ISOP21_2 = 0;
	
	// SFMA data1 P21_3
	PORT_ISOPMC21_3 = 0;
	PORT_ISOPM21_3 = 0;
	PORT_ISOP21_3 = 0;
	
	// SFMA data2 P21_4
	PORT_ISOPMC21_4 = 0;
	PORT_ISOPM21_4 = 0;
	PORT_ISOP21_4 = 0;
	
	// SFMA data3 P21_5
	PORT_ISOPMC21_5 = 0;
	PORT_ISOPM21_5 = 0;
	PORT_ISOP21_5 = 0;
	
	// SFMA data4 P21_6
	PORT_ISOPMC21_6 = 0;
	PORT_ISOPM21_6 = 0;
	PORT_ISOP21_6 = 0;
	
	// SFMA data5 P21_7
	PORT_ISOPMC21_7 = 0;
	PORT_ISOPM21_7 = 0;
	PORT_ISOP21_7 = 0;
	
	// SFMA data6 P21_8
	PORT_ISOPMC21_8 = 0;
	PORT_ISOPM21_8 = 0;
	PORT_ISOP21_8 = 0;
	
	// SFMA data7 P21_9
	PORT_ISOPMC21_9 = 0;
	PORT_ISOPM21_9 = 0;
	PORT_ISOP21_9 = 0;
*/
	// H/V direction on LOE TFT
/*    
	PORT_ISOPMC42_12 = 0;
	PORT_ISOPM42_12 = 0;
	PORT_ISOP42_12 = 1;

	PORT_ISOPMC42_13 = 0;
	PORT_ISOPM42_13 = 0;
	PORT_ISOP42_13 = 1;

	PORT_ISOPMC42_10 = 0;
	PORT_ISOPM42_10 = 0;
	PORT_ISOP42_10 = 0;
	
*/	PORT_ISOPMC45_1 = 0;
	PORT_ISOPM45_1 = 0;
	PORT_ISOP45_1 = 0;
	
	PORT_ISOPMC42_10 = 0;
	PORT_ISOPM42_10 = 0;
	PORT_ISOP42_10 = 1;

	// TURN Left P17_0
	PORT_ISOPM17_0 = 0;
	PORT_ISOP17_0 = 0;
	
	// TURN Right P17_1
	PORT_ISOPM17_1 = 0;
	PORT_ISOP17_1 = 0;
	
	// High Beam P17_2
	PORT_ISOPM17_2 = 0;
	PORT_ISOP17_2 = 0;
	
	// Service P17_3
	PORT_ISOPM17_3 = 0;
	PORT_ISOP17_3 = 0;
	
	// Low Battery P17_4
	PORT_ISOPM17_4 = 0;
	PORT_ISOP17_4 = 0;
	
	// Mal Function P17_5
	PORT_ISOPM17_5 = 0;
	PORT_ISOP17_5 = 0;
	
	// Neutral P17_6
	PORT_ISOPM17_6 = 0;
	PORT_ISOP17_6 = 0;
	
	// ABS ON/OFF P17_7
	PORT_ISOPM17_7 = 0;
	PORT_ISOP17_7 = 0;
	
	// Bluetooth P17_9
	PORT_ISOPM17_9 = 0;
	PORT_ISOP17_9 = 0;
	
	// Engine oil temp P17_10
	PORT_ISOPM17_10 = 0;
	PORT_ISOP17_10 = 0;
	
	// engine coolant temp P16_10
	PORT_ISOPM16_10 = 0;
	PORT_ISOP16_10 = 0;
	
	// engine oil pressure P16_11
	PORT_ISOPM16_11 = 0;
	PORT_ISOP16_11 = 0;
	
	// Turn Right sense
	PORT_ISOPMC16_6 = 0;
	PORT_ISOPM16_6 = 1;
	PORT_ISOPIBC16_6 = 1;
	
/*	//----TFT RESET signal
	PORT_ISOPMC42_13 = 0;
	PORT_ISOPM42_13 = 0;
	PORT_ISOP42_13 = 1;
	
	//-------- direction flags
	PORT_ISOPMC42_10 = 0;
	PORT_ISOPM42_10 = 0;
	PORT_ISOP42_10 = 1;
	
	PORT_ISOPMC45_1 = 0;
	PORT_ISOPM45_1 = 0;
	PORT_ISOP45_1 = 1;
	
*/	
	
	
	
	
	

}

enum ind_enum_t {
    my_ind_high_beam,
	my_ind_service,
    my_ind_low_batt,
    my_ind_mal_fn,
    my_ind_neutral,
    my_ind_abs,
	my_ind_bt,
    my_ind_e_oil_temp,
    my_ind_e_oil_prs,
    my_ind_e_cool_temp,
    my_ind_def
    
};
unsigned char my_flag;
unsigned char my_count;
unsigned char my_once=1;
void my_tell_tale_update(void)
{
	uint32_t new_index;
	
	new_index = (uint32_t)(rand() / (RAND_MAX + 1.0) * 9);
	if(my_once)
	{
		TURN_LEFT = OFF;
		TURN_RIGHT = OFF;
		my_once = 0;
	}
	HIGH_BEAM = OFF;
	SERV_ICON = OFF;
	LOW_BATT = OFF;
	MAL_FN = OFF;
	NEUTRAL_IND = OFF;
	ABS_ON_OFF = OFF;
	BT_ASSO = OFF;
	E_OIL_TEMP = OFF;
	E_COOL_TEMP = OFF;
	E_OIL_PRESS = OFF;
	
	
	switch(new_index)
	{
	
	case my_ind_high_beam:
		HIGH_BEAM = ON;
		break;
	
	case my_ind_service:
		SERV_ICON = ON;
		break;
		
	case my_ind_low_batt:
		LOW_BATT = ON;
		break;
	case my_ind_mal_fn:
		MAL_FN = ON;
		break;
	case my_ind_neutral:
		NEUTRAL_IND = ON;
		break;
	case my_ind_abs:
		ABS_ON_OFF = ON;
		break;
	case my_ind_bt:
		BT_ASSO = ON;
		break;
	case my_ind_e_oil_temp:
		E_OIL_TEMP = ON;
		break;	
	case my_ind_e_oil_prs:
		E_OIL_PRESS = ON;
		break;	
	case my_ind_e_cool_temp:
		E_COOL_TEMP = ON;
		break;	
		
	case my_ind_def:
		NEUTRAL_IND = ON;
		ABS_ON_OFF = ON;
		break;	
	default:
	
		break;

	}
}
#if 1
void my_update_flasher(void)
{	
	static uint32_t last_time = 0;
	//-------------  START INDICATORS LEFT RIGHT UPDATE -----------------------------
	if ((R_TICK_GetTimeMS(0) - last_time) > 400)
	{
		last_time = R_TICK_GetTimeMS(0);
		if(my_flag)
		{
			TURN_LEFT_TOGGLE = ON;
			TURN_RIGHT = OFF;
		}
		else
		{
			TURN_RIGHT_TOGGLE = ON;
			TURN_LEFT = OFF;
		}
		my_count++;
		if(my_count > 30)
		{
			my_count = 0;
			if(my_flag == 0)
			{
				my_flag = 1;
			}
			else
			{
				my_flag = 0;
			}
		}
		//update_aic_model_per_sec ();
	}
	//-------------  END INDICATORS LEFT RIGHT UPDATE -----------------------------
}

#endif
