/* **********************************************************************************************************************
File                    :       swsc.c
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */

#include "r_typedefs.h"
#include "swsc.h"
#ifndef _GUI_H
#include "gui.h"
#endif
#include "drome.h"
#include "druserconf.h"
#include "r_gpio_api.h"
#include "digitalGuage.h"
#include "r_cg_timer.h"
#include "UDSS.h"
#include "batt.h"
#include "alert.h"
#include "serial_to_parallel.h"

//uint16_t SWSC_ADC_value; /* after adding this file in vlib folder need to delete this variable */

#ifdef U350
uint32_t Counter_SET_KEY_LongPress_100usec;
uint32_t Counter_MODE_KEY_LongPress_100usec;
uint32_t Counter_SET_KEY_100usec;
uint32_t Counter_MODE_KEY_100usec;

uint8_t SET_KEY_Flag   = 0;
uint8_t MODE_KEY_Flag   = 0;

uint8_t MODE_key_press = 0;
uint8_t SET_key_press = 0;

_Bool Set_Key_stuck = 0;
_Bool Mode_Key_stuck = 0;

uint8_t MODE_key_LongPress;
uint8_t MODE_key_ShortPress;
uint8_t SET_key_LongPress;
uint8_t SET_key_ShortPress;
uint8_t dont_throw_alerts_BS6_alert_on_screen_present = 0;
uint8_t OK_pressed = 0;
_Bool SET_5seckey_press = 0;
//_Bool ok_modelong_press = 0;
uint8_t Ok_key_LongPress;
_Bool avoid_trip_reset = 0;
extern uint8_t skip_alert_Enable;
void SWSC_Mono_Condition(void);
//_Bool TPMS_ENTER_PR = 0;
#else
/*W501*/
uint16_t Counter_OK_KEY_LongPress_100usec;
uint16_t Counter_OK_KEY_100usec;
uint16_t Counter_UP_KEY_LongPress_100usec;
uint16_t Counter_UP_KEY_100usec;
uint16_t Counter_DOWN_KEY_100usec;
uint16_t Counter_MENU_KEY_100usec;

uint8_t OK_KEY_Flag   = 0;
uint8_t UP_KEY_Flag   = 0;
uint8_t DOWN_KEY_Flag = 0;
uint8_t MENU_KEY_Flag = 0;
uint8_t Counter_2s;
uint8_t AFE_resetflag = 0;

uint8_t Ok_key_LongPress;
uint8_t Ok_key_ShortPress;
uint8_t Down_key_ShortPress;
uint8_t Up_key_ShortPress;
uint8_t Menu_key_ShortPress;
uint8_t dont_throw_alerts_BS6_alert_on_screen_present = 0;
_Bool avoid_trip_reset = 0;
uint8_t OK_pressed = 0;
uint8_t SET_key_press = 0;
_Bool SET_5seckey_press = 0;
_Bool key_released = 0;
_Bool Up_key_released = 0;
_Bool Menu_key_released = 0;
_Bool Down_key_released = 0;

extern _Bool ok_modelong_press;
extern uint8_t skip_alert_Enable;
extern uint64_t Settingmenu_timeout_10sec; 
extern _Bool lightonoff_modelong_press;
void SWSC_Condition(void);
//_Bool TPMS_ENTER_PR = 0;
#endif
_Bool PAB_jump_driveinfo = 0;
extern _Bool Flag_AFE_reset;
extern uint64_t BS6_alert_timeout_4sec;
#ifdef U350

//* **********************************************************************************************************************
//* Function Name: SWSC_Mono_Condition
//* Description  : This function Reads Switch condition.
//* Arguments    : None
//* Return Value : None
//********************************************************************************************************************** */
void SWSC_Mono_Condition(void)
{
	uint32_t key;
	/* Note:=> need to check "Button_pressed" condition from button.c in every "if" condition */
	/*Mode Switch*/
	if((R_GPIO_PinRead(17,4) == 0)&&(logo_completed==1))//mode switch	start normal switch operation after logo_complete
	{
		MODE_KEY_Flag = 1;
//		if(MODE_key_LongPress == 1)
//		{
//			button_value = 0;		//OK function for Mode long press
//			MODE_key_ShortPress = 0;
//		}
//		else if(MODE_key_ShortPress == 1)
//		{
//			button_value = 1;		//Menu function for Mode Short press
//		}
//		if(Counter_MODE_KEY_LongPress_100usec >= 50000) /* checking "MODE_KEY" is pressed for 5sec or not  */
//		{
//			/* MODE_KEY is pressed for 5 sec */
//			MODE_key_LongPress  = 1;
//			MODE_key_ShortPress = 0;
//			button_value = 0;		//OK function for Mode long press
//		}
//		else if(Counter_MODE_KEY_100usec >= 2000/*5000*/)
//		{
//			/* MODE_KEY is pressed for 500 msec */
//			MODE_key_ShortPress = 1;
//			button_value = 1;		//Menu function for Mode Short press
//		}
	}
	else
	{
		MODE_key_press = 0;
		MODE_KEY_Flag = 0;
		MODE_key_ShortPress      = 0;
		Counter_MODE_KEY_LongPress_100usec = 0;
	}

	/*SET Switch*/
	if((R_GPIO_PinRead(17,5) == 0)&&(logo_completed==1))//Set switch	start normal switch operation after logo_complete
	{
		SET_KEY_Flag = 1;
//		if(SET_key_LongPress == 1)
//		{
//			button_value = 4;		//OK function for Mode long press
//			SET_key_ShortPress = 0;
//		}
//		else if(SET_key_ShortPress == 1)
//		{
//			button_value = 2;		//Menu function for Mode Short press
//		}
//		if(Counter_SET_KEY_LongPress_100usec >= 50000) /* checking "MODE_KEY" is pressed for 5sec or not  */
//		{
//			/* SET_KEY is pressed for 5 sec */
//			SET_key_LongPress  = 1;
//			SET_key_ShortPress = 0;
//			button_value = 4;		//OK long function for Set long press
//		}
//		else if(Counter_SET_KEY_100usec >= 2000/*5000*/)
//		{
//			/* SET_KEY is pressed for 300 msec */
//			SET_key_ShortPress = 1;
//			button_value = 2;		//Down function for Set Short press
//		}
	}
	else
	{
		SET_key_press = 0;
		SET_KEY_Flag = 0;
		SET_key_ShortPress      = 0;
		Counter_SET_KEY_LongPress_100usec = 0;
		avoid_trip_reset = 0;
	}

	if(MODE_key_press==1 && SET_key_press==1)
	{
		 reset_odometer();
	}
/*************************************************************************************************************/
	if(logo_completed==1)
	{
		switch (button_value)
	      {
	         case BUTTON_UP:
	            (void)drome_sendEventToDrome((U32)drome_KB_OK);
//	            ok_modelong_press = 1;
	            button_value = BUTTON_NONE;
	            break;
	         case BUTTON_RIGHT:
	            (void)drome_sendEventToDrome((U32)drome_KB_MENU);
	            button_value = BUTTON_NONE;
	            break;
	         case BUTTON_DOWN:
	             if(skip_alert_Enable == 1)
	             {
					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
					skip_alert_Enable = 2;
	             }
	             else if(skip_alert_Enable == 3)
	             {
	            	 (void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
	            	 skip_alert_Enable = 0;
	             }
	             else
	             {
	            	 (void)drome_sendEventToDrome((U32)drome_KB_DOWN);
	             }
	            button_value = BUTTON_NONE;
	            break;
	         case BUTTON_LEFT:
	            (void)drome_sendEventToDrome((U32)drome_KB_UP);
	            button_value = BUTTON_NONE;
	            break;
	         case BUTTON_OK_LONG:
	        	/* (void)drome_sendEventToDrome((U32)drome_KB_OK_LONG);*/

	        	 key = drome_getCurrentViewKey();
//	        	 if(dont_throw_alerts_BS6_alert_on_screen_present == 1)
//	        	 {
//	        		 dont_throw_alerts_BS6_alert_on_screen_present = 0;
//        			(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
//        			Counter_MODE_KEY_LongPress_100usec = 0;
//        			button_value = BUTTON_NONE;
//	        	 }
//	        	 else
//	        	 {
	        		 if((key == 0x22042)||(key == 0x22043)||(key == 0x22044)//incorrect DEF
	        			||(key == 0x22046)||(key == 0x22047)||(key == 0x22048)//Dosing Malfunction
						||(key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
	        		 {
	        			 (void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);
	        			 dont_throw_alerts_BS6_alert_on_screen_present = 0;
	        			 avoid_trip_reset = 1;
	        		 }
	        		 else if((key == 0x11001)&&(avoid_trip_reset == 0))
	        		 {
	        			 reset_tripmeterA();
	        		 }
	        		 else if((key == 0x11002)&&(avoid_trip_reset == 0))
	        		 {
	        			 reset_tripmeterB();
	        		 }
	        		 else if((key == 0x11003)&&(avoid_trip_reset == 0))
	        		 {
	        			 reset_AC_tripmeter();
	        		 }
	        		 else if((key == 0x11011)&&(avoid_trip_reset == 0))
	        		 {
	        			 Flag_AFE_reset = 1;
	        		 }
//	        	 }
	        	 button_value = BUTTON_NONE;
	            break;
	         case BUTTON_NONE:
	         default:
	        	 OK_pressed = 0;
//	        	 ok_modelong_press = 0;
	            break;
	      }
	}
}
#else
///* **********************************************************************************************************************
//* Function Name: SWSC_Condition
//* Description  : This function Reads Switch condition.
//* Arguments    : None
//* Return Value : None
//********************************************************************************************************************** */
void SWSC_Condition(void)
{
	/* Note:=> need to check "Button_pressed" condition from button.c in every "if" condition */
	/* OK_KEY */
	uint32_t key;
	/*start normal switch operation after logo_complete*/
	if(((SWSC_ADC_value >= ADC_COUNT_OK_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_OK_KEY_OFF))&&(logo_completed==1))
	{
		OK_KEY_Flag = 1;

		if(Counter_OK_KEY_LongPress_100usec >= 17000/*34000*/) /*50000 checking "OK_KEY" is pressed for 5sec or not  */
		{
			/* OK_KEY is pressed for 2.5 sec */
			//Ok_key_LongPress  = 1;
			Ok_key_ShortPress = 0;
			button_value = 4;
			key = drome_getCurrentViewKey();

			if((key == 0x11021)&&(key_released == 1))
			{
//				key_released = 0;
			lightonoff_modelong_press = 1;
			}
			if((Counter_OK_KEY_LongPress_100usec >= 34000)&&(Ok_key_LongPress == 0))
			{
				Ok_key_LongPress = 1;//for regen
//				key = drome_getCurrentViewKey();
				if((key == 0x22042)||(key == 0x22043)||(key == 0x22044)//incorrect DEF
						||(key == 0x22046)||(key == 0x22047)||(key == 0x22048)//Dosing Malfunction
						||(key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);
					dont_throw_alerts_BS6_alert_on_screen_present = 0;
					avoid_trip_reset = 1;
					/*to avoid regen Entry in permanent alert skip*/
					Counter_OK_KEY_LongPress_100usec = 0;
					Ok_key_LongPress = 0;
				}

			}
		}
		else
		{

		}
//		else if((Counter_OK_KEY_100usec > 300) && (Counter_OK_KEY_100usec <= 34000))//(Counter_OK_KEY_100usec >= 3000)//
//		{
//			/* OK_KEY is pressed for 500 msec */
//			Ok_key_ShortPress = 1;
//			button_value = 0;
//		}
	}
	else if((Counter_OK_KEY_100usec > 300) && (Counter_OK_KEY_100usec <= 17000/*34000*/))//(Counter_OK_KEY_100usec >= 3000)//
	{
		/* OK_KEY is pressed for 500 msec */
		OK_KEY_Flag = 0;
		Ok_key_ShortPress = 1;
		button_value = 0;
		Counter_OK_KEY_100usec = 0;
	}
	else
	{
		OK_KEY_Flag = 0;
		Ok_key_ShortPress      = 0;
		Ok_key_LongPress       = 0;
		Counter_OK_KEY_100usec = 0;
		avoid_trip_reset = 0;
		key_released = 1;
//		ok_modelong_press = 0;
	}

	/* UP_KEY */
	if((SWSC_ADC_value >= ADC_COUNT_UP_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_UP_KEY_OFF))
	{
		UP_KEY_Flag = 1;

//		if(Counter_UP_KEY_100usec >= 2000/*5000*/) /* checking "UP_KEY" is pressed for 500msec or not  */
//		{
//			/* UP_KEY is pressed for 500 msec */
//			Up_key_ShortPress = 1;
//			button_value = 3;
//			Counter_UP_KEY_100usec = 0;
//		}
	}
	else
	{
		if(UP_KEY_Flag == 1)
		{
			Up_key_released = 1;
		}
		else
		{
	//		Up_key_released = 0;
		}
		UP_KEY_Flag = 0;
		Up_key_ShortPress = 0;
//		Counter_UP_KEY_100usec = 0;
	}

	/* DOWN_KEY */
	if((SWSC_ADC_value >= ADC_COUNT_DOWN_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_DOWN_KEY_OFF))
	{
		DOWN_KEY_Flag = 1;

//		if(Counter_DOWN_KEY_100usec >= 2000/*5000*/) /* checking "DOWN_KEY" is pressed for 500msec or not  */
//		{
//			/* DOWN_KEY is pressed for 500 msec */
//			Down_key_ShortPress = 1;
//			button_value = 2;
//			Counter_DOWN_KEY_100usec = 0;
//		}
	}
	else
	{
		if(DOWN_KEY_Flag == 1)
		{
			Down_key_released = 1;
		}
		DOWN_KEY_Flag = 0;
		Down_key_ShortPress = 0;
		//		Counter_DOWN_KEY_100usec = 0;
	}

	/* MENU_KEY */
	if((SWSC_ADC_value >= ADC_COUNT_MENU_KEY_ON) && (SWSC_ADC_value < ADC_COUNT_MENU_KEY_OFF))
	{
		MENU_KEY_Flag = 1;

//		if(Counter_MENU_KEY_100usec >= 2000/*5000*/) /* checking "MENU_KEY" is pressed for 500msec or not  */
//		{
//			/* MENU_KEY is pressed for 500 msec */
//			Menu_key_ShortPress = 1;
//			button_value = 1;
//			Counter_MENU_KEY_100usec = 0;
//		}
	}
	else
	{
		if(MENU_KEY_Flag == 1)
				{
					Menu_key_released = 1;
				}
		MENU_KEY_Flag = 0;
		Menu_key_ShortPress = 0;
//r		Counter_MENU_KEY_100usec = 0;
	}
	if((logo_completed==1)&&(LVSD_flag == 0U)&&(HVSD_flag == 0U))
	{
	 switch (button_value)
	      {
	         case BUTTON_UP:
	          /*  if(skip_alert_Enable == 1)
	            {
	            	(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
	            	skip_alert_Enable = 2;
	            }
				else if(skip_alert_Enable == 3)
				{
					(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);
					skip_alert_Enable = 0;
				}
				else*/
				{
					 if(startkey != 0x22023)
					 {
									 ok_modelong_press = 1;
					 }
					 (void)drome_sendEventToDrome((U32)drome_KB_OK);

				}
	            Settingmenu_timeout_10sec = 0;
	            button_value = BUTTON_NONE;
	            key_released = 0;
				if(skip_flag == 1)						/*Key pressed for alert skip & redisplay alert counter timeout*/
				{
					start_alart_timeout_5sec=1;
					BS6_alert_timeout_4sec = 0;
				}
				WH_dooropen_present = 0;	/*wARNING HISTORY PARAMETERS RESET WHEN SCROLLING*/
				WH_backdoor = 0;
				WH_Hooddoor = 0;
				WH_codriverdoor = 0;
				WH_driverdoor = 0;
				WH_Rearleftdoor = 0;
				WH_Rearrightdoor = 0;
	            break;
	         case BUTTON_RIGHT:
	            (void)drome_sendEventToDrome((U32)drome_KB_MENU);
	            Settingmenu_timeout_10sec = 0;
	            button_value = BUTTON_NONE;
	            key_released = 0;
	            ok_modelong_press = 0;
				if(skip_flag == 1)						/*Key pressed for alert skip & redisplay alert counter timeout*/
				{
					start_alart_timeout_5sec=1;
					BS6_alert_timeout_4sec = 0;
				}

				if((startkey == 0x22037) ||(startkey == 0x22038))
				{
					PAB_jump_driveinfo = 1;
				}
				else
				{
					PAB_jump_driveinfo = 0;
				}
	            break;
	         case BUTTON_DOWN:
	        	 if((startkey >= 0x11059)&&(startkey <= 0x11094))
	        	 {
	        		 (void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);	/*warning history when scrolling doors inactive*/
	        	 }
	            (void)drome_sendEventToDrome((U32)drome_KB_DOWN);
	            Settingmenu_timeout_10sec = 0;
	            Arrowfill_flag = 1;
	            A_flag = 3;
	            button_value = BUTTON_NONE;
	            key_released = 0;
	            ok_modelong_press = 0;
	            WH_dooropen_present = 0;
	            WH_backdoor = 0;
	            WH_Hooddoor = 0;
	            WH_codriverdoor = 0;
	            WH_driverdoor = 0;
	            WH_Rearleftdoor = 0;
	            WH_Rearrightdoor = 0;
	            if(skip_flag == 1)						/*Key pressed for alert skip & redisplay alert counter timeout*/
	            {
					start_alart_timeout_5sec=1;
					BS6_alert_timeout_4sec = 0;
				}
	            break;
	         case BUTTON_LEFT:
	        	 if((startkey >= 0x11059)&&(startkey <= 0x11094))
	        	 {
	        		 (void)drome_sendEventToDrome((U32)drome_InEvent_WH_Alldoorclose);	/*warning history when scrolling doors inactive*/
	        	 }
	        	 (void)drome_sendEventToDrome((U32)drome_KB_UP);
	        	 Settingmenu_timeout_10sec = 0;
	        	 ArrowUpfill_flag = 1;
	        	 U_flag = 3;
	        	 button_value = BUTTON_NONE;
	        	 key_released = 0;
	        	 ok_modelong_press = 0;
	        	 WH_dooropen_present = 0;
	        	 WH_backdoor = 0;
	        	 WH_Hooddoor = 0;
	        	 WH_codriverdoor = 0;
	        	 WH_driverdoor = 0;
	        	 WH_Rearleftdoor = 0;
	        	 WH_Rearrightdoor = 0;
	        	 if(skip_flag == 1)						/*Key pressed for alert skip & redisplay alert counter timeout*/
	        	 {
	        		 start_alart_timeout_5sec=1;
	        		 BS6_alert_timeout_4sec = 0;
	        	 }
	        	 break;
	         case BUTTON_OK_LONG:
	            (void)drome_sendEventToDrome((U32)drome_KB_OK_LONG);
	            key_released = 0;
	            ok_modelong_press = 0;
	            //Counter_OK_KEY_LongPress_100usec = 0;

	 			if(skip_flag == 1)						/*Key pressed for alert skip & redisplay alert counter timeout*/
	 			{
	 				start_alart_timeout_5sec=1;
	 				BS6_alert_timeout_4sec = 0;
	 			}
	 			 key = drome_getCurrentViewKey();
#if 0
        		 if((key == 0x22042)||(key == 0x22043)||(key == 0x22044)//incorrect DEF
        			||(key == 0x22046)||(key == 0x22047)||(key == 0x22048)//Dosing Malfunction
					||(key == 0x22050)||(key == 0x22051)||(key == 0x22052))//DEF Level
        		 {
        			 (void)drome_sendEventToDrome((U32)drome_InEvent_Longpress_Alert_Timeout);
        			 dont_throw_alerts_BS6_alert_on_screen_present = 0;
        			 avoid_trip_reset = 1;
        		 }
#endif
        		 if((key == 0x11001)&&(avoid_trip_reset == 0))
	        	 {
	        		 reset_tripmeterA();
	        	 }
	        	 else if((key == 0x11002)&&(avoid_trip_reset == 0))
	        	 {
	        		 reset_tripmeterB();
	        	 }
	        	 else if((key == 0x11003)&&(avoid_trip_reset == 0))
	        	 {
	        		 reset_AC_tripmeter();
	        	 }
	        	 else if(((key == 0x11011)||(key == 0x11014))&&(avoid_trip_reset == 0)) /*AFE can reset by button for all Setting options*/
	        	 {
	        		 Flag_AFE_reset = 1;
	        	 }
        		 Settingmenu_timeout_10sec = 0;
	            button_value = BUTTON_NONE;
	            break;
	         case BUTTON_NONE:
	         default:
#ifdef COLOR_TFT
	        	 Arrowfill_flag = 0;
	        	 ArrowUpfill_flag = 0;
#endif
	        	 lightonoff_modelong_press = 0;
//	        	 ok_modelong_press = 0;
//	        	 key_released = 1;
	            break;
	      }
	 GI_off_SW_PRESS();
	}
	else
	{
		  button_value = BUTTON_NONE;
	}
}
#endif
