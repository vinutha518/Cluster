
#include "r_typedefs.h"
#include "dr7f701461.dvf.h"
#include "stdlib.h"
#include "my_tell_tale.h"
#include "r_tick_api.h"
//#include "sgui.h"

void my_tell_tale_init(void)
{
	unsigned int my_i;
// for TRULY RGB 240 x 320
#if 1

	//-------- TFT Backlight
	PORT_ISOPMC17_3 = 0;
	PORT_ISOPM17_3 = 0;
	PORT_ISOP17_3 = 1;
	
	//----TFT RESET signal
	PORT_ISOPMC44_5 = 0;
	PORT_ISOPM44_5 = 0;
	PORT_ISOP44_5 = 1;
	
	for(my_i = 0; my_i < 0xffff; my_i++);
	PORT_ISOP44_5 = 0;
	for(my_i = 0; my_i < 0xffff; my_i++);
	
	PORT_ISOP44_5 = 1;
	
	
#endif	
//----------------------------------------------------	
	

		
	
	
	

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
