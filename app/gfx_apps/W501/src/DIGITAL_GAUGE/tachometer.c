/*
 * tachometer.c
 *
 *  Created on: 08-Feb-2019
 *      Author: MSWELT23
 */

#include "init.h"
#include "digitalGuage.h"
#include "r_typedefs.h"
#include "UDSS.h"
#include "tachometer.h"
#include "my_can.h"
#include "batt.h"
#include "UDS_DTC.h"
#include "UDSS_APP.h"

Split16 down_scale1;

uint32_t final_tachometer_speed = 0;
uint32_t display_tachometer_speed = 0;

_Bool Tacho_invalid_default = 0;
void tachometer(void);
/* ***********************************************************************************************
 * Function Name: tachometer
 * Description  : This function used for tachometer of vechile.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void tachometer(void)
{
	down_scale1.u16=0;

	//uint16_t display_tachometer_speed;

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if(VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM  == 1U)
			{
				if(EMS1_10_message_timeout_flag == 0)
				{
					/* EMS36_10 message */
					down_scale1.u8[0] = CAN_EMS1_10[1].byte;
					down_scale1.u8[1] = CAN_EMS1_10[2].byte;


					if((down_scale1.u16 == 0xFFFF) || (down_scale1.u16 == 0x0))   // to log Gear signal invalid DTC
					{
						//Failure_DTC_Set(DTC_EMS_RPM_SIGNAL_CONTFAIL_E004);
						Tacho_invalid_default	=	1;
					}
					else
					{
						//Failure_DTC_Clear(DTC_EMS_RPM_SIGNAL_CONTFAIL_E004);
						Tacho_invalid_default	=	0;
					}

					final_tachometer_speed = (down_scale1.u16);


					if(final_tachometer_speed < 8000) /* ENG speed < 8000 rpm */
					{
						display_tachometer_speed = final_tachometer_speed;
					}
					else if(final_tachometer_speed > 8000) /* ENG speed > 8000 rpm */
					{
						display_tachometer_speed = 7999;
					}
				}
				else if(EMS1_10_message_timeout_flag != 0)
				{
					display_tachometer_speed = 7999;
				}
				else
				{
					/* Noting */
				}
			}
			else
			{
				display_tachometer_speed = 0;
			}
		}
		else
		{
			/* disable digital speedometer */
			display_tachometer_speed = 0;
		}
	}
	else
	{
		/* display off */
		display_tachometer_speed = 0;
	}

	down_scale1.u8[1]=  (display_tachometer_speed) & (0xFF00);//Upper_bit
	down_scale1.u8[0]=  (display_tachometer_speed) & (0x00FF);//Lower_bit
}
