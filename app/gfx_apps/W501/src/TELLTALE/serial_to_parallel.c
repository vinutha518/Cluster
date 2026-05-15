/*
 * serial_to_parallel.c
 *
 *  Created on: 22-Jan-2019
 *      Author: MSWELT19
 */
#include "r_typedefs.h"
#include "my_can.h"
#include "serial_to_parallel.h"
#include "parallel_to_serial.h"
#include "TLC6C_S_P.h"
#include "dbgmacro.h"
#include "errmgr.h"
#include "dr7f701461.dvf.h"
#include "r_bsp_api.h"         /* Board support package */
#include "sound_pcm.h"
#include "UDSS.h"
#include "r_cg_timer.h"
#include "r_gpio_api.h"
#include "sound_pcm.h"
#include "r_dev_api.h"
#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"
#include "r_gpio_api.h"
#include "r_taub_api.h"

#include "drome.h"
#include "druserconf.h"
/* *********Variables Definition************************************************************************ */
_Bool sw_press_ok = 0;
_Bool Digital_Speedo_Guage_off = 0;
Led_status ls;    /* define a union struct for bitwise access */
Led_status ls_Dummy;
extern _Bool PROVEOUT_OK;
/***************************************/
extern uint32_t key;
extern _Bool lightonoff_modelong_press;
static union
{
	struct{
		unsigned char  Rx_1:1;
		unsigned char  Rx_2:1;
		unsigned char  Rx_3:1;
		unsigned char  Rx_4:1;
		unsigned char  Rx_5:1;
		unsigned char  Rx_6:1;
		unsigned char  Rx_7:1;
		unsigned char  Rx_8:1;
	}bits;
	unsigned char byte;

}DID_TT_bit_data_set2;
/***************************************/
void OP_SHIFT_DELAY (void)
{
	uint8_t ocnts=250;
	for(ocnts=250;ocnts>0;ocnts--);
}

void output_shift_register(void)
{
	uint8_t i=0;
	uint16_t temp=1;
	_Bool j;

	R_GPIO_WritePin(10, 11, 0);  /*LATCH_ENABLE_LOW*/
	R_GPIO_WritePin(11, 0, 1);		/*LATCH_CLR=1*/
	R_GPIO_WritePin(11, 1, 0);		/*LED_ENABLE=0*/

	Led_Shift = Led_status_word;

	for(i=0;i<16;i++)
	{
		OP_SHIFT_DELAY();
		j = 1 & (Led_Shift >> i);
		R_GPIO_WritePin(11, 3, j ); 		/*LED_SI*/
		OP_SHIFT_DELAY();
		R_GPIO_WritePin(11, 2, 1);		/*LED_CLK*/
		OP_SHIFT_DELAY();
		R_GPIO_WritePin(11, 2, 0);
		OP_SHIFT_DELAY();
	}

	R_GPIO_WritePin(10, 11, 1);		/*LATCH_ENABLE*/
	OP_SHIFT_DELAY();
	R_GPIO_WritePin(10, 11, 0);
	OP_SHIFT_DELAY();
}

void All_Telltale_OFF(void)
{
	uint8_t j;
	AEBS_OFF_TT      = 0;
	SEAT_BELT_TT     = 0;
	LOW_FUEL_TT      = 0;
	BSG_TT           = 0;
	FRONT_FOG_TT     = 0;
	REAR_FOG_TT      = 0;
	PARK_LAMP_TT     = 0;
	OVER_SPEED_TT    = 0;
	FOUR_WD_LOW_TT   = 0;
	FOUR_WD_HIGH_TT  = 0;
	Water_in_fuel_TT = 0;
	TEMP_TT          = 0;
	Glow_Plug_TT     = 0;
	REGEN_TT         = 0;
	DOOR_OPEN_TT     = 0;
	ECO_TT           = 0;
	IMMOBIIZER_TT     =0;
	GSdata0[0] = GSdata1[0] ;
	GSdata0[40] = GSdata1[40] ;

	SendGSData(GSdata0);
	output_shift_register();
	for(j=0;j<48;j++)
	{
		GSdata1[j] = GSdata0[j] ;
	}
}

void All_Telltale_ON(void)
{
	AEBS_OFF_TT      = 0;
	SEAT_BELT_TT     = 0;
	LOW_FUEL_TT      = 0;
	BSG_TT           = 0;
	FRONT_FOG_TT     = 0;
	REAR_FOG_TT      = 0;
	PARK_LAMP_TT     = 0;
	OVER_SPEED_TT    = 0;
	FOUR_WD_LOW_TT   = 0;
	FOUR_WD_HIGH_TT  = 0;
	TEMP_TT          = 0;
	Glow_Plug_TT     = 0;
	REGEN_TT         = 0;
	DOOR_OPEN_TT     = 0;
	ECO_TT           = 0;
	IMMOBIIZER_TT     =0;
	EPS_MALFUNCTION_TT=0x000;


	if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
	{
		AEBS_MULFUNCTION_ON_TT=0xffe;
		Check_Engine_TT = 0xffe;
		MIL_TT = 0xffe;

		if(FEATURE_CODE_FUEL_TYPE==1)
		{
			Water_in_fuel_TT = 1;
		}
		if(FEATURE_CODE_DEF_TT_CONFIG==1)
		{
			DEF_TT =  0xffe;
		}
		if(FEATURE_CODE_CRUISE_TT_CONFIG==1)
		{
			CRUISE_TT=0xffe;
		}
		if(FEATURE_CODE_START_STOP_TT_CONFIG ==1)
		{
			AUTO_START_STOP_TT = 0xffe;
		}
		if(FEATURE_CODE_DPF_TT_CONFIG == 1)
		{
			DPF_TT = 0xffe;
		}

	}
	else
	{
		Check_Engine_TT = 0;
		MIL_TT = 0;
		Water_in_fuel_TT = 0;
		DEF_TT =  0;
		CRUISE_TT=0;
		AUTO_START_STOP_TT = 0;
		DPF_TT = 0;
	}

	if(VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM==1)
	{
		AIR_BAG_TT =0xffe;
		DID_TT_bit_data_set2.bits.Rx_1=1;
	}
	else
	{
		AIR_BAG_TT =0;
		DID_TT_bit_data_set2.bits.Rx_1=0;
	}

	if(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0)
	{
		ESP_MALFUNCTION_TT =0xffe;
		ESP_OFF_TT= 0xffe;
	}
	else
	{
		ESP_MALFUNCTION_TT =0;
		ESP_OFF_TT= 0;
	}

	if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==0 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==1)||
			(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0))
	{
		PARK_BRAKE_LOW_FLUID_EBD_TT = 0xffe;
		ABS_TT=0xffe;
	}
	else
	{
		PARK_BRAKE_LOW_FLUID_EBD_TT = 0;
		ABS_TT=0;
	}
	if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1) && (FEATURE_CODE_TPMS_TT_CONFIG==1))
	{
		GLOBAL_TPMS_MULFUNCTION_TT=0xffe;
	}
	else
	{
		GLOBAL_TPMS_MULFUNCTION_TT=0;
	}
	if(VAR_CODING_TRANSMISSION_CONTROL_UNIT == 1)
	{
		AT_FAIL_TT = 0xffe;
	}
	else
	{
		AT_FAIL_TT = 0;
	}

	//scan_TT_status();
	SendGSData(GSdata1);
	output_shift_register();
	//	EPS_MALFUNCTION_TT=0;
}
#ifdef PCBrevB
#else
void GI_Control(void)
{
	char m;
	static unsigned char tt_dcdata=0x4;
#if 0
	SP_LD1_TT=0xfff;
	SP_LD2_TT=0xfff;
	SP_LD3_TT=0xfff;
	RP_LD1_TT=0xfff;
	RP_LD2_TT=0xfff;
	RP_LD3_TT=0xfff;
	S_LD1_TT=0xfff;
	S_LD2_TT=0xfff;
	S_LD3_TT=0xfff;
	S_LD4_TT=0xfff;
	S_LD5_TT=0xfff;
	S_LD6_TT=0xfff;
	R_LD1_TT=0xfff;
	R_LD2_TT=0xfff;
	R_LD3_TT=0xfff;
	R_LD4_TT=0xfff;
	R_LD5_TT=0xfff;
	R_LD6_TT=0xfff;
	if(FEATURE_CODE_FUEL_TYPE == 1)
	{
		/* Diesel*/
		R_LD8_TT=0xfff;
		R_LD7_TT=0x000;

	}
	else
	{
		/* GAsoline*/
		R_LD8_TT=0;
		R_LD7_TT=0xfff;
	}
#endif

	//for(t=0x0f;t<0x7f;t+=4)
	if(tt_dcdata <=0x65 && PROVEOUT_OK==0)
	{



		DCdata1[0]=tt_dcdata;
		DCdata1[1]=tt_dcdata;
		DCdata1[2]=tt_dcdata;
		DCdata1[3]=tt_dcdata;
		DCdata1[4]=tt_dcdata;
		DCdata1[5]=tt_dcdata;




		DCdata2[15]=tt_dcdata;
		DCdata2[16]=tt_dcdata;
		DCdata2[18]=tt_dcdata;
		DCdata2[19]=tt_dcdata;
		DCdata2[20]=tt_dcdata;
		DCdata2[21]=tt_dcdata;
		DCdata2[22]=tt_dcdata;
		DCdata2[23]=tt_dcdata;

		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
		SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata2);
		cascade_TT_send_data();
		//OP_SHIFT_DELAY();


		S_LD1_TT=0xfff;
		S_LD2_TT=0xfff;
		S_LD3_TT=0xfff;
		S_LD4_TT=0xfff;
		S_LD5_TT=0xfff;
		S_LD6_TT=0xfff;
		R_LD1_TT=0xfff;
		R_LD2_TT=0xfff;
		R_LD3_TT=0xfff;
		R_LD4_TT=0xfff;
		R_LD5_TT=0xfff;
		R_LD6_TT=0xfff;

		/*	SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;*/

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_LD8_TT=0xfff;
			R_LD7_TT=0x000;

		}
		else
		{
			/* GAsoline*/
			R_LD8_TT=0;
			R_LD7_TT=0xfff;
		}
		//SendGSData(GSdata1);

		tt_dcdata+=4;
	}

}
void Check_TT_ETM_Mode(void)
{

	if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM==1)
	{
		AEBS_MULFUNCTION_ON_TT=0xfff;
		Check_Engine_TT = 0xfff;
		MIL_TT = 0xfff;
		ECT_high_TT=0xfff;
		Low_Oil_press_TT = 0xfff;

		if(FEATURE_CODE_FUEL_TYPE==1)
		{
			Water_in_fuel_TT = 1;

			if(FEATURE_CODE_GLOWPLUG_TT_CONFIG==1)
			{
				Glow_Plug_TT = 1;
			}
		}
		if(FEATURE_CODE_DEF_TT_CONFIG==1)
		{
			DEF_TT =  0xfff;
		}
		if(FEATURE_CODE_CRUISE_TT_CONFIG==1)
		{
			CRUISE_TT=0xfff;
		}
		if(FEATURE_CODE_START_STOP_TT_CONFIG ==1)
		{
			AUTO_START_STOP_TT = 0xfff;
		}
		if(FEATURE_CODE_DPF_TT_CONFIG == 1)
		{
			DPF_TT = 0xfff;
		}
		if(FEATURE_CODE_BATTERY_TT_TYPE==0)
		{
			Batt_chrg_TT = 0xfff;
		}
		if(FEATURE_CODE_REGEN==1)
		{
			REGEN_TT=1;
		}
		if(FEATURE_CODE_ECO_TT_CONFIG == 1)
		{
			ECO_TT = 1;
		}

	}
	else
	{
		AEBS_MULFUNCTION_ON_TT=0;
		Check_Engine_TT = 0;
		MIL_TT = 0;
		Water_in_fuel_TT = 0;
		DEF_TT =  0;
		CRUISE_TT=0;
		AUTO_START_STOP_TT = 0;
		DPF_TT = 0;
		ECT_high_TT=0;
		Batt_chrg_TT = 0;
		REGEN_TT=0;
		Low_Oil_press_TT = 0;
		Glow_Plug_TT = 0;
		ECO_TT = 0;
	}

	if(VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM==1)
	{
		AIR_BAG_TT =0xfff;
		SEAT_BELT_TT = 1;
		DID_TT_bit_data_set2.bits.Rx_1=1;
	}
	else
	{
		AIR_BAG_TT =0;
		SEAT_BELT_TT = 0;
		DID_TT_bit_data_set2.bits.Rx_1=0;
	}

	if(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0)
	{
		ESP_MALFUNCTION_TT =0xfff;
		ESP_OFF_TT= 0xfff;
		HDC_TT = 0xfff;
		HHC_MALFUNCTION_TT = 0xfff;
	}
	else
	{
		ESP_MALFUNCTION_TT =0;
		ESP_OFF_TT= 0;
		HDC_TT = 0;
		HHC_MALFUNCTION_TT = 0;
	}

	if((VAR_CODING_ELECTRONICS_STABILITY_CONTROL==0 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==1)||
			(VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1 && VAR_CODING_ANTILOCK_BRAKING_SYSTEM==0))
	{
		PARK_BRAKE_LOW_FLUID_EBD_TT = 0xfff;
		ABS_TT=0xfff;
	}
	else
	{
		PARK_BRAKE_LOW_FLUID_EBD_TT = 0;
		ABS_TT=0;
	}
	if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1) && (FEATURE_CODE_TPMS_TT_CONFIG==1))
	{
		GLOBAL_TPMS_MULFUNCTION_TT=0xfff;
	}
	else
	{
		GLOBAL_TPMS_MULFUNCTION_TT=0;
	}
	if(VAR_CODING_TRANSMISSION_CONTROL_UNIT == 1)
	{
		AT_FAIL_TT = 0xfff;
	}
	else
	{
		AT_FAIL_TT = 0;
	}

	if((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1) && (VAR_CODING_IMMOBILIZER_CONTROL_UNIT==1))
	{
		IMMOBIIZER_TT=1;
	}
	else
	{
		IMMOBIIZER_TT=0;
	}
	if(VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)
	{
		DOOR_OPEN_TT = 1;
	}
	else
	{
		DOOR_OPEN_TT = 0;
	}
	if((FEATURE_CODE_TRANSFER_MODE_CONFIG == 1)&&(FEATURE_CODE_4WD_LOGIC==0)||(FEATURE_CODE_4WD_LOGIC==1))
	{
		FOUR_WD_LOW_TT=1;
		FOUR_WD_HIGH_TT=1;
	}
	else
	{
		FOUR_WD_LOW_TT=0;
		FOUR_WD_HIGH_TT=0;
	}
	if(((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1) && (FEATURE_CODE_HIGHBEAM_TT_TYPE==0))||(FEATURE_CODE_HIGHBEAM_TT_TYPE==1))
	{
		High_beam_TT = 0xfff;
	}
	else
	{
		High_beam_TT = 0;
	}
	if(((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)&&(FEATURE_CODE_PARK_TT_TYPE==0 ))||(FEATURE_CODE_PARK_TT_TYPE==1))
	{
		PARK_LAMP_TT=1;
	}
	else
	{
		PARK_LAMP_TT=0;
	}
	if(((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)&&(FEATURE_CODE_FRONT_FOG_TT_TYPE==0 ))||(FEATURE_CODE_FRONT_FOG_TT_TYPE==1))
	{
		FRONT_FOG_TT = 1;
	}
	else
	{
		FRONT_FOG_TT =0;
	}
	if(((VAR_CODING_MAHINDRA_BODY_FUNCTION_MODULE==1)&&(FEATURE_CODE_REAR_FOG_TT_TYPE==0 ))||(FEATURE_CODE_REAR_FOG_TT_TYPE==1))
	{
		REAR_FOG_TT = 1;
	}
	else
	{
		REAR_FOG_TT =0;
	}
	if(FEATURE_CODE_SPEED_INPUT==0)
	{
		OVER_SPEED_TT = 1;
	}
	else
	{
		OVER_SPEED_TT = 0;
	}

	Turn_right_TT = 0xfff;
	Turn_left_TT = 0xfff;
	LOW_FUEL_TT=1;

	SendGSData(GSdata1);
	output_shift_register();
	//	EPS_MALFUNCTION_TT=0;
}
void GI_off_SW_PRESS(void)
{
	key = drome_getCurrentViewKey();

	if(((sw_press_ok==0)&&(key == 0x11021)&&(lightonoff_modelong_press == 1))||((sw_press_ok == 1)&&(key == 0x11021)&&(lightonoff_modelong_press == 0)))
	{
		lightonoff_modelong_press = 0;
		S_LD1_TT=0;
		S_LD2_TT=0;
		S_LD3_TT=0;
		S_LD4_TT=0;
		S_LD5_TT=0;
		S_LD6_TT=0;
		R_LD1_TT=0;
		R_LD2_TT=0;
		R_LD3_TT=0;
		R_LD4_TT=0;
		R_LD5_TT=0;
		R_LD6_TT=0;
		/*pointer LED*/

		SP_LD1_TT=0;
		SP_LD2_TT=0;
		SP_LD3_TT=0;
		RP_LD1_TT=0;
		RP_LD2_TT=0;
		RP_LD3_TT=0;

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_LD8_TT=0;//0xfff;
			R_LD7_TT=0x000;

		}
		else
		{
			/* GAsoline*/
			R_LD8_TT=0;
			R_LD7_TT=0;//0xfff;
		}
		sw_press_ok = 1;
		Digital_Speedo_Guage_off = 1;

	}


	if((sw_press_ok==1)&&(key == 0x11021)&&(lightonoff_modelong_press == 1))
	{
		lightonoff_modelong_press = 0;
		S_LD1_TT=0xfff;
		S_LD2_TT=0xfff;
		S_LD3_TT=0xfff;
		S_LD4_TT=0xfff;
		S_LD5_TT=0xfff;
		S_LD6_TT=0xfff;
		R_LD1_TT=0xfff;
		R_LD2_TT=0xfff;
		R_LD3_TT=0xfff;
		R_LD4_TT=0xfff;
		R_LD5_TT=0xfff;
		R_LD6_TT=0xfff;

		SP_LD1_TT=0xfff;
		SP_LD2_TT=0xfff;
		SP_LD3_TT=0xfff;
		RP_LD1_TT=0xfff;
		RP_LD2_TT=0xfff;
		RP_LD3_TT=0xfff;

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_LD8_TT=0xfff;
			R_LD7_TT=0x000;

		}
		else
		{
			/* GAsoline*/
			R_LD8_TT=0;
			R_LD7_TT=0xfff;
		}
		sw_press_ok = 0;
		Digital_Speedo_Guage_off = 0;
	}

	if((sw_press_ok == 1)&&((key == 0x11057)||(key == 0x11051)||(key == 0x11202)))	/*Warning History page*/ /*- Turn ON Lights when move to next screen*/
	{
		S_LD1_TT=0xfff;
		S_LD2_TT=0xfff;
		S_LD3_TT=0xfff;
		S_LD4_TT=0xfff;
		S_LD5_TT=0xfff;
		S_LD6_TT=0xfff;
		R_LD1_TT=0xfff;
		R_LD2_TT=0xfff;
		R_LD3_TT=0xfff;
		R_LD4_TT=0xfff;
		R_LD5_TT=0xfff;
		R_LD6_TT=0xfff;

		SP_LD1_TT=0xfff;
		SP_LD2_TT=0xfff;
		SP_LD3_TT=0xfff;
		RP_LD1_TT=0xfff;
		RP_LD2_TT=0xfff;
		RP_LD3_TT=0xfff;

		if(FEATURE_CODE_FUEL_TYPE == 1)
		{
			/* Diesel*/
			R_LD8_TT=0xfff;
			R_LD7_TT=0x000;

		}
		else
		{
			/* GAsoline*/
			R_LD8_TT=0;
			R_LD7_TT=0xfff;
		}
		//sw_press_ok = 0;
		Digital_Speedo_Guage_off = 0;
	}
}
#endif
