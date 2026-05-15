/* **********************************************************************************************************************
File                    :       gearIndication.c
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

 *********************************************************************************************************************** */


/* =========================================================================
 Includes
============================================================================ */
#include "init.h"
#include "r_typedefs.h"
#include "digitalGuage.h"
#include "UDSS.h"
#include "my_can.h"
#include "batt.h"
#include "gui.h"
#include "gearIndication.h"
#include "UDS_DTC.h"
#include "UDSS_APP.h"
#include "parallel_to_serial.h"
/* ************************************************************************* */
void Current_Gear_and_Gear_Shift_Indication(void);
uint8_t IO_Control_Gear_indication(void);
uint8_t IO_Control_Gear_shift_indication(void);
/* ************************************************************************* */
uint8_t Shift_UP_St   = 0;
uint8_t Shift_Down_St = 0;
uint8_t Gear_config = 0;
//uint8_t Diag_RW_Gear_Disp_Target_Current_Sel = 1; /* See once in UDSS.h & remove */

union Major_Gear
{
	struct
	{
		unsigned char Neutral:1;
		unsigned char First_Gear:1;
		unsigned char Second_Gear:1;
		unsigned char Third_Gear:1;
		unsigned char Fourth_Gear:1;
		unsigned char Fifth_Gear:1;
		unsigned char Sixth_Gear:1;
		unsigned char Reverse:1;
	}bitField;
	unsigned char data;
};

union Minor_Gear
{
	unsigned char data;
};

union Display_Gear
{
	struct
	{
		unsigned char Neutral:1;
		unsigned char First_Gear:1;
		unsigned char Second_Gear:1;
		unsigned char Third_Gear:1;
		unsigned char Fourth_Gear:1;
		unsigned char Fifth_Gear:1;
		unsigned char Sixth_Gear:1;
		unsigned char Reverse:1;
	}bitField;
	unsigned char data;
};

union Major_Gear Major_Gear_St;
union Minor_Gear Minor_Gear_St;
union Display_Gear Gear;
uint8_t gear_shift_to_HMI = 2;
#define SHIFT_UP 0
#define SHIFT_DOWN 1
#define SHIFT_BLANK 2
uint8_t gear_to_HMI = 55;
extern uint8_t Diag_gear_indc;
#define MT_N 0
#define MT_1 1
#define MT_2 2
#define MT_3 3
#define MT_4 4
#define MT_5 5
#define MT_6 6
#define MT_R 7

#define AT_1 8
#define AT_2 9
#define AT_3 10
#define AT_4 11
#define AT_5 12
#define AT_6 13
#define AT_P 14
#define AT_R 15
#define AT_N 16
#define AT_D 17
#define  GEAR_DASH 18
#define GEAR_BLANK 55/*this will show blank on HMI*/
//#define GEAR_NULL
extern uint32_t Sound_Bit;

extern _Bool Buzz_rever_gr;
extern _Bool Buzz_rever_gr_SB;
extern uint8_t Buzz_rever_gr_cntr;
extern _Bool Buzz_rever_gr_compl;
/*************************************/
extern uint8_t Reversegear_missed;
extern uint8_t cycle_Reversegear_completed;

extern _Bool chime_start1;
extern _Bool Seatbelt_ongoing;
_Bool Reverse_gear_active = 0;
//#ifdef SPRINT4
uint8_t Gear_Configuration = 0;
//#endif
/*************************************/

/* ************************************************************************* */

/* ***********************************************************************************************
 * Function Name: Current_Gear_and_Gear_Shift_Indication
 * Description  : This function used for Current_Gear_and_Gear_Shift_Indication.
 * Arguments    : None
 * Return Value : None
 ************************************************************************************************ */
void Current_Gear_and_Gear_Shift_Indication(void)
{
//#ifndef SPRINT4
//uint8_t Gear_Configuration = 0;
//#endif

	uint8_t STS_Eng_data = 0U;
	uint8_t CURRENT_GEAR_MT = 0U;
	uint8_t TARGET_GEAR_GSI = 0U;
	uint8_t CLUTCH_STS      = 0U;
	uint8_t GEAR_UP_FLG     = 0U;
	uint8_t GEAR_DOWN_FLG   = 0U;
	uint8_t TGS_LEVER       = 0U;
	uint8_t Gear_Actual     = 0U;

	//Diag_RW_Gear_Disp_Target_Current_Sel = (Assign value from UDS);

	/* checking ignition ON or OFF */
	if(IGN_ON_flag == 1)
	{
		/* checking LVSD & HVSD flag */
		if((LVSD_flag == 0U) && (HVSD_flag == 0U))
		{
			if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 0) && (VAR_CODING_TRANSMISSION_CONTROL_UNIT == 0))
			{
				Gear_Configuration = 0;
			}
			else if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (VAR_CODING_TRANSMISSION_CONTROL_UNIT == 0))
			{
				Gear_Configuration = 2;		/*MT*/
				Gear_config = 1;
			}
			else if((VARIANT_CODING_ENGINE_MANAGEMENT_SYSTEM == 1) && (VAR_CODING_TRANSMISSION_CONTROL_UNIT == 1))
			{
				Gear_Configuration = 3;		/*AT*/
				Gear_config = 2;
			}
			else
			{
				Gear_Configuration = 1;
				Gear_config = 0;
			}

			switch(Gear_Configuration)
			{
			case 0:
				Major_Gear_St.data = 0;
				Minor_Gear_St.data = 0;
				Shift_UP_St   = 0;
				Shift_Down_St = 0;

				gear_shift_to_HMI = SHIFT_BLANK;
				gear_to_HMI = GEAR_BLANK;
				break;

			case 1:
				Major_Gear_St.data = 0;
				Minor_Gear_St.data = 0;
				Shift_UP_St   = 0;
				Shift_Down_St = 0;

				gear_shift_to_HMI = SHIFT_BLANK;
				gear_to_HMI = GEAR_BLANK;
				break;

			case 2:
				STS_Eng_data = CAN_EMS1_10[6].byte;
				STS_Eng_data = ((((STS_Eng_data) & (0xE0)) >> 5) & (0x07));

				if(FEATURE_CODE_GSI_CONFIG == 0)
				{
					gear_shift_to_HMI=SHIFT_BLANK;
					if((EMS1_10_message_timeout_flag == 0) && (EMS3_10_message_timeout_flag == 0))
					{
						if(STS_Eng_data == 2) /* Manual Transmission - Gear Indication Functional Logic */
						{

							CURRENT_GEAR_MT = CAN_EMS1_10[5].byte;
							CURRENT_GEAR_MT = ((CURRENT_GEAR_MT) & (0x0F));

							if(CURRENT_GEAR_MT == 0x07)/* Reverse */
							{
								Reverse_gear_active = 1;
							}
							else
							{
								Reverse_gear_active = 0;
								Buzz_rever_gr_compl = 0;//test
							}

							/* if(CURRENT_GEAR_MT == 0xF)   // to log Gear signal invalid DTC
							{
								//Failure_DTC_Set(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
							}
							else
							{
								//Failure_DTC_Clear(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
							} */

							TARGET_GEAR_GSI = CAN_EMS8_10[3].byte;
							TARGET_GEAR_GSI = ((((TARGET_GEAR_GSI) & (0x78)) >> 3) & (0x0F));

							/* if(TARGET_GEAR_GSI == 0xF)   // to log gear signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
							} */

							CLUTCH_STS      = CAN_EMS3_10[3].byte;
							CLUTCH_STS		= ((((CLUTCH_STS) & (0xE0)) >> 5) & (0x07));

							/* if(CLUTCH_STS == 0x7)   // to log clutch signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
							} */

#ifdef GRAYSCALE_TFT
							/* for Reverse Alarm start*/
							if(Diag_RW_Reverse_Chime_Enable==1)
							{
								if(CURRENT_GEAR_MT == 0x07) /* Reverse */
								{
									/* Display "R" */

									if(((Sound_Bit == 0x0) || (Sound_Bit == 0x40) || (Sound_Bit == 0x200)) && Buzz_rever_gr_compl == 0)
									{
										Sound_Bit = 0x40;
										Buzz_rever_gr=1;
										reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
									}
									else
									{
										Buzz_rever_gr=0;
									}
									Buzz_rever_gr_SB = 1;
								}
								else
								{
									if(Sound_Bit == 0x40)
									{
										Sound_Bit = 0x0;
									}

									Buzz_rever_gr_SB=0;
									Buzz_rever_gr=0;
									Buzz_rever_gr_cntr=0;
									Buzz_rever_gr_compl=0;
									if(Diag_RW_RPAS_Display_Screen_source == 1)
									{
										reverse_played = 1;
									}
									else
									{
										reverse_played = 0;
									}
								}
							}
							else
							{
								reverse_played = 1;//to work RPAS chime if reverse chime disable
							}
							/* For Reverse Alarm end*/
#endif

#ifdef COLOR_TFT
							/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........Reverse gear indication*/
							if(Diag_RW_Reverse_Chime_Enable==1)
							{
								if((CURRENT_GEAR_MT == 0x07)&&(Reversegear_ongoing == 0)/*&&(chime_start1 == 0)*/&&(cycle_Reversegear_completed == 0)) /* Reverse */
								{
									if((Sound_Bit == 0x0 || Sound_Bit == 0x40 || Sound_Bit == 0x200)  && (cycle_Reversegear_completed == 0))
									{
										Sound_Bit = 0x40;						/*0010 0000 - Reverse Alarm sound bit*/
										Seatbelt_ongoing=0;
										Reversegear_ongoing = 1;
										chime_start1 = 1;
										Reversegear_missed = 0;
										reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
									}
									else
									{
										Reversegear_ongoing = 0;
										Reversegear_missed = 0;
									}
									Buzz_rever_gr_SB = 1;						/*Reverse gear occured - used for Seat belt*/
								}
								else if(CURRENT_GEAR_MT != 0x07)
								{
									if(Sound_Bit == 0x40)
									{
										Sound_Bit = 0x00;						/**Sound bit CLEAR**/
									}
									Reversegear_ongoing = 0;
									cycle_Reversegear_completed = 0;
									Reversegear_missed = 1;
									Buzz_rever_gr_SB = 0;						/*Reverse gear occured*/
									if(Diag_RW_RPAS_Display_Screen_source == 1)
									{
										reverse_played = 1;
									}
									else
									{
										reverse_played = 0;
									}
								}
							}
							else
							{
								reverse_played = 1;//to work RPAS chime if reverse chime disable
							}
#endif
							if(CURRENT_GEAR_MT == 0) /* Neutral */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Neutral = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_N;
									Diag_gear_indc=0;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Neutral = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_N;
									Diag_gear_indc=MT_N;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}

							}
							else if(CURRENT_GEAR_MT == 0x01) /* 1st Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.First_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_1;
									Diag_gear_indc=MT_1;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.First_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_1;
									Diag_gear_indc=MT_1;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x02) /* 2nd Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Second_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_2;
									Diag_gear_indc=MT_2;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Second_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_2;
									Diag_gear_indc=MT_2;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x03) /* 3rd Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Third_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_3;
									Diag_gear_indc=MT_3;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Third_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_3;
									Diag_gear_indc=MT_3;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x04) /* 4th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fourth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_4;
									Diag_gear_indc=MT_4;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fourth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_4;
									Diag_gear_indc=MT_4;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x05) /* 5th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fifth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_5;
									Diag_gear_indc=MT_5;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fifth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_5;
									Diag_gear_indc=MT_5;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x06) /* 6th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Sixth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_6;
									Diag_gear_indc=MT_6;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Sixth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI = MT_6;
									Diag_gear_indc=MT_6;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x07) /* Reverse */
							{
								Major_Gear_St.data = 0;
								Major_Gear_St.bitField.Reverse = 1;
								Minor_Gear_St.data = 0;
								gear_to_HMI = MT_R;
								Diag_gear_indc=MT_R;
							}
							else if((CURRENT_GEAR_MT >= 0x08) && (CURRENT_GEAR_MT <= 0x0F)) /* Reserved & Invalid */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Minor_Gear_St.data = 0;
									gear_to_HMI = GEAR_BLANK;
									Diag_gear_indc=0x0F;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Minor_Gear_St.data = 0;
									gear_to_HMI = GEAR_BLANK;
									Diag_gear_indc=0x0F;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}


						}
						else /* STS_ENG != 2 */
						{
							Major_Gear_St.data = 0;
							Minor_Gear_St.data = 0;
							Shift_UP_St   = 0;
							Shift_Down_St = 0;

							gear_to_HMI = GEAR_BLANK;
						}
					}
					else //timeout
					{
						Major_Gear_St.data = 0;
						Minor_Gear_St.data = 0;
						Shift_UP_St   = 0;
						Shift_Down_St = 0;
						gear_to_HMI = GEAR_DASH;
						Diag_gear_indc=0x0F;
						Buzz_rever_gr_compl = 0;//test

						/*Only take care for Low End*/
					Buzz_rever_gr_SB=0;
					Buzz_rever_gr=0;
					Buzz_rever_gr_cntr=0;
					Buzz_rever_gr_compl=0;

					Reversegear_ongoing = 0;//Reverse chime for W501H
					cycle_Reversegear_completed = 0;
					}

				}/* end */


#if 1
				/*FEATURE_CODE_CURRENT_TARGET_SEL==1(Current)*/
				if((FEATURE_CODE_GSI_CONFIG == 1) && (FEATURE_CODE_CURRENT_TARGET_SEL == 1))
				{
					if((EMS1_10_message_timeout_flag == 0) && (EMS3_10_message_timeout_flag == 0))
					{
						if((STS_Eng_data == 2)) /* Manual Transmission - Gear Indication Functional Logic */
						{

							CURRENT_GEAR_MT = CAN_EMS1_10[5].byte;
							CURRENT_GEAR_MT = ((CURRENT_GEAR_MT) & (0x0F));

							if(CURRENT_GEAR_MT == 0x07)/* Reverse */
							{
								Reverse_gear_active = 1;
							}
							else
							{
								Reverse_gear_active = 0;
								Buzz_rever_gr_compl = 0;//test
							}

							/* if(CURRENT_GEAR_MT == 0xF)   // to log Gear signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
							} */

							TARGET_GEAR_GSI = CAN_EMS8_10[3].byte;
							TARGET_GEAR_GSI = ((((TARGET_GEAR_GSI) & (0x78)) >> 3) & (0x0F));

							/* if(TARGET_GEAR_GSI == 0xF)   // to log gear signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
							} */

							CLUTCH_STS      = CAN_EMS3_10[3].byte;
							CLUTCH_STS		= ((((CLUTCH_STS) & (0xE0)) >> 5) & (0x07));

							/* if(CLUTCH_STS == 0x7)   // to log cluh signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
							} */
#ifdef GRAYSCALE_TFT
							/* for Reverse Alarm start*/
							if(Diag_RW_Reverse_Chime_Enable==1)
							{
								if(CURRENT_GEAR_MT == 0x07) /* Reverse */
								{
									/* Display "R" */

									if(((Sound_Bit == 0x0) || (Sound_Bit == 0x40) || (Sound_Bit == 0x200) || (Sound_Bit == 0x20000) || (Sound_Bit == 0x30000) || (Sound_Bit == 0x40000)) && Buzz_rever_gr_compl == 0)
									{
										Sound_Bit = 0x40;
										Buzz_rever_gr=1;
										reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
									}
									else
									{
										Buzz_rever_gr=0;
									}
									Buzz_rever_gr_SB = 1;
								}
								else
								{
									if(Sound_Bit == 0x40)
									{
										Sound_Bit = 0x0;
									}

									Buzz_rever_gr_SB=0;
									Buzz_rever_gr=0;
									Buzz_rever_gr_cntr=0;
									Buzz_rever_gr_compl=0;
									if(Diag_RW_RPAS_Display_Screen_source == 1)
									{
										reverse_played = 1;
									}
									else
									{
										reverse_played = 0;
									}
								}
							}
							else
							{
								reverse_played = 1;//to work RPAS chime if reverse chime disable
							}
							/* For Reverse Alarm end*/
#endif

#ifdef COLOR_TFT
							/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........Reverse gear indication*/
							if(Diag_RW_Reverse_Chime_Enable==1)
							{
								if((CURRENT_GEAR_MT == 0x07)&&(Reversegear_ongoing == 0)/*&&(chime_start1 == 0)*/&&(cycle_Reversegear_completed == 0)) /* Reverse */
								{
									if((Sound_Bit == 0x0 || Sound_Bit == 0x40 || Sound_Bit == 0x200)  && (cycle_Reversegear_completed == 0))
									{
										Sound_Bit = 0x40;						/*0010 0000 - Reverse Alarm sound bit*/
										Seatbelt_ongoing=0;
										Reversegear_ongoing = 1;
										chime_start1 = 1;
										Reversegear_missed = 0;
										reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
									}
									else
									{
										Reversegear_ongoing = 0;
										Reversegear_missed = 0;
									}
									Buzz_rever_gr_SB = 1;						/*Reverse gear occured - used for Seat belt*/
								}
								else if(CURRENT_GEAR_MT != 0x07)
								{
									if(Sound_Bit == 0x40)
									{
										Sound_Bit = 0x00;						/**Sound bit CLEAR**/
									}
									Reversegear_ongoing = 0;
									cycle_Reversegear_completed = 0;
									Reversegear_missed = 1;
									Buzz_rever_gr_SB = 0;						/*Reverse gear occured*/
									if(Diag_RW_RPAS_Display_Screen_source == 1)
									{
										reverse_played = 1;
									}
									else
									{
										reverse_played = 0;
									}
								}
							}
							else
							{
								reverse_played = 1;//to work RPAS chime if reverse chime disable
							}
#endif

							if(CURRENT_GEAR_MT == 0) /* Neutral */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Neutral = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_N;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Neutral = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_N;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}

							}
							else if(CURRENT_GEAR_MT == 0x01) /* 1st Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.First_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_1;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.First_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_1;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x02) /* 2nd Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Second_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_2;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Second_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_2;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x03) /* 3rd Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Third_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_3;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Third_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_3;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x04) /* 4th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fourth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_4;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fourth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_4;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x05) /* 5th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fifth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_5;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fifth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_5;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x06) /* 6th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Sixth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_6;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Sixth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_6;
									Diag_gear_indc=CURRENT_GEAR_MT;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(CURRENT_GEAR_MT == 0x07) /* Reverse */
							{
								Major_Gear_St.data = 0;
								Major_Gear_St.bitField.Reverse = 1;
								Minor_Gear_St.data = 0;
								gear_to_HMI=MT_R;
								Diag_gear_indc=CURRENT_GEAR_MT;
							}
							else if((CURRENT_GEAR_MT >= 0x08) && (CURRENT_GEAR_MT <= 0x0F)) /* Reserved & Invalid */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Minor_Gear_St.data = 0;

									gear_to_HMI = GEAR_BLANK;//
									Diag_gear_indc=0x0F;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Minor_Gear_St.data = 0;

									gear_to_HMI = GEAR_BLANK;//
									Diag_gear_indc=0x0F;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}

							/* Manual Transmission - Gear Recommendation Functional Logic */
							/* Checking Shift Up & Shift Down */
							GEAR_UP_FLG   = (((CAN_EMS8_10[1].byte) >> 5) & 1); /* storing 3rd bit */
							GEAR_DOWN_FLG = (((CAN_EMS8_10[1].byte) >> 6) & 1); /* storing 2nd bit */

							//if(EMS8_10_message_timeout_flag == 0)
							//{
							if((GEAR_UP_FLG == 0) && (GEAR_DOWN_FLG == 1))
							{
								Shift_UP_St   = 0;
								Shift_Down_St = 1;
								gear_shift_to_HMI=SHIFT_DOWN;
							}
							else if((GEAR_UP_FLG == 1) && (GEAR_DOWN_FLG == 0))
							{
								Shift_UP_St   = 1;
								Shift_Down_St = 0;
								gear_shift_to_HMI=SHIFT_UP;
							}
							else if((GEAR_UP_FLG == 0) && (GEAR_DOWN_FLG == 0))
							{
								Shift_UP_St   = 0;
								Shift_Down_St = 0;
								gear_shift_to_HMI=SHIFT_BLANK;//
							}
							else if((GEAR_UP_FLG == 1) && (GEAR_DOWN_FLG == 1))
							{
								Shift_UP_St   = 0;
								Shift_Down_St = 0;
								gear_shift_to_HMI=SHIFT_BLANK;//
							}
							//}
							/*else
							{
								gear_shift_to_HMI=SHIFT_BLANK;
							}*/

						}
						else /* STS_ENG != 2 */
						{
							Major_Gear_St.data = 0;
							Minor_Gear_St.data = 0;
							Shift_UP_St   = 0;
							Shift_Down_St = 0;

							gear_shift_to_HMI = SHIFT_BLANK;//
							gear_to_HMI = GEAR_BLANK;//
							
						}
					}
					else
					{
						Major_Gear_St.data = 0;
						Minor_Gear_St.data = 0;
						Shift_UP_St   = 0;
						Shift_Down_St = 0;

						gear_shift_to_HMI = SHIFT_BLANK;
						gear_to_HMI = GEAR_DASH;
						Diag_gear_indc=0x0F;
						Buzz_rever_gr_compl = 0;//test

						/*Only take care for Low End*/
					Buzz_rever_gr_SB=0;
					Buzz_rever_gr=0;
					Buzz_rever_gr_cntr=0;
					Buzz_rever_gr_compl=0;
					Reversegear_ongoing = 0;//Reverse chime for W501H
					cycle_Reversegear_completed = 0;
					}


				}/* end */
#endif
				/*FEATURE_CODE_CURRENT_TARGET_SEL==0(Target)*/
				else if((FEATURE_CODE_GSI_CONFIG == 1) && (FEATURE_CODE_CURRENT_TARGET_SEL == 0))
				{

					if((EMS8_10_message_timeout_flag == 0) && (EMS3_10_message_timeout_flag == 0) && (EMS1_10_message_timeout_flag == 0))
					{
						if((STS_Eng_data == 2)) /* Manual Transmission - Gear Indication Functional Logic */
						{
							CURRENT_GEAR_MT = CAN_EMS1_10[5].byte;
							CURRENT_GEAR_MT = ((CURRENT_GEAR_MT) & (0x0F));

							TARGET_GEAR_GSI = CAN_EMS8_10[3].byte;
							TARGET_GEAR_GSI = ((((TARGET_GEAR_GSI) & (0x78)) >> 3) & (0x0F));

/*Current Gear need to consider for Reverse_gear_active status*/

							if(CURRENT_GEAR_MT == 0x07)//TARGET_GEAR_GSI
							{
								Reverse_gear_active = 1;

							}
							else
							{
								Reverse_gear_active = 0;
								Buzz_rever_gr_compl = 0;//test
							}

							CLUTCH_STS      = CAN_EMS3_10[3].byte;
							CLUTCH_STS		= ((((CLUTCH_STS) & (0xE0)) >> 5) & (0x07));

							/* if(CURRENT_GEAR_MT == 0xF)   // to log Gear signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_GEAR_SIGNAL_CONTFAIL_E003);
							} */

							/* if(TARGET_GEAR_GSI == 0xF)   // to log gear signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_TARGET_GEAR_SIGNAL_CONTFAIL_E074);
							} */

							/* if(CLUTCH_STS == 0x7)   // to log cluh signal invalid DTC
							{
								Failure_DTC_Set(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
							}
							else
							{
								Failure_DTC_Clear(DTC_EMS_CLUCH_SIGNAL_CONTFAIL_E009);
							} */

#ifdef GRAYSCALE_TFT
							/* for Reverse Alarm start*/
							if(Diag_RW_Reverse_Chime_Enable==1)
							{
								if(CURRENT_GEAR_MT == 0x07) /* Reverse */
								{
									/* Display "R" */

									if(((Sound_Bit == 0x0) || (Sound_Bit == 0x40) || (Sound_Bit == 0x200)) && Buzz_rever_gr_compl == 0)
									{
										Sound_Bit = 0x40;
										Buzz_rever_gr=1;
										reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/

									}
									else
									{
										Buzz_rever_gr=0;
									}
									Buzz_rever_gr_SB = 1;
								}
								else
								{
									if(Sound_Bit == 0x40)
									{
										Sound_Bit = 0x0;
									}

									Buzz_rever_gr_SB=0;
									Buzz_rever_gr=0;
									Buzz_rever_gr_cntr=0;
									Buzz_rever_gr_compl=0;
									if(Diag_RW_RPAS_Display_Screen_source == 1)
									{
										reverse_played = 1;
									}
									else
									{
										reverse_played = 0;
									}
									//									W501_LOW_CounterBz_3sec = 0;
								}
							}
							else
							{
								reverse_played = 1;//to work RPAS chime if reverse chime disable
							}
							/* For Reverse Alarm end*/
#endif

#ifdef COLOR_TFT
							/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........Reverse gear indication*/
							if(Diag_RW_Reverse_Chime_Enable==1)
							{
								if((CURRENT_GEAR_MT == 0x07)&&(Reversegear_ongoing == 0)/*&&(chime_start1 == 0)*/&&(cycle_Reversegear_completed == 0)) /* Reverse */
								{
									if((Sound_Bit == 0x0 || Sound_Bit == 0x40 || Sound_Bit == 0x200)  && (cycle_Reversegear_completed == 0))
									{
										Sound_Bit = 0x40;						/*0010 0000 - Reverse Alarm sound bit*/
										Seatbelt_ongoing=0;
										Reversegear_ongoing = 1;
										chime_start1 = 1;
										Reversegear_missed = 0;
										reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
									}
									else
									{
										Reversegear_ongoing = 0;
										Reversegear_missed = 0;
									}
									Buzz_rever_gr_SB = 1;						/*Reverse gear occured - used for Seat belt*/
								}
								else if(CURRENT_GEAR_MT != 0x07)
								{
									if(Sound_Bit == 0x40)
									{
										Sound_Bit = 0x00;						/**Sound bit CLEAR**/
									}
									Reversegear_ongoing = 0;
									cycle_Reversegear_completed = 0;
									Reversegear_missed = 1;
									Buzz_rever_gr_SB = 0;						/*Reverse gear occured*/
									if(Diag_RW_RPAS_Display_Screen_source == 1)
									{
										reverse_played = 1;
									}
									else
									{
										reverse_played = 0;
									}
								}
							}
							else
							{
								reverse_played = 1;//to work RPAS chime if reverse chime disable
							}
#endif

							if(TARGET_GEAR_GSI == 0) /* Neutral */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Neutral = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_N;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Neutral = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_N;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}

							}
							else if(TARGET_GEAR_GSI == 0x01) /* 1st Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.First_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_1;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.First_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_1;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(TARGET_GEAR_GSI == 0x02) /* 2nd Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Second_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_2;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Second_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_2;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(TARGET_GEAR_GSI == 0x03) /* 3rd Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Third_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_3;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Third_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_3;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(TARGET_GEAR_GSI == 0x04) /* 4th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fourth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_4;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fourth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_4;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(TARGET_GEAR_GSI == 0x05) /* 5th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fifth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_5;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Fifth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_5;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(TARGET_GEAR_GSI == 0x06) /* 6th Gear */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Sixth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_6;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Major_Gear_St.bitField.Sixth_Gear = 1;
									Minor_Gear_St.data = 0;
									gear_to_HMI=MT_6;
									Diag_gear_indc=TARGET_GEAR_GSI;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}
							else if(TARGET_GEAR_GSI == 0x07) /* REVERSE Gear */
							{
								Major_Gear_St.data = 0;
								Major_Gear_St.bitField.Reverse = 1;
								Minor_Gear_St.data = 0;
								gear_to_HMI=MT_R;
								Diag_gear_indc=TARGET_GEAR_GSI;
							}
							else if((TARGET_GEAR_GSI >= 0x08) && (TARGET_GEAR_GSI <= 0x0F)) /* Reserved & Invalid */
							{
								if(CLUTCH_STS == 0)
								{
									Major_Gear_St.data = 0;
									Minor_Gear_St.data = 0;

									gear_to_HMI = GEAR_BLANK;//
									Diag_gear_indc=0x0F;
								}
								else if((CLUTCH_STS >= 0x03) && (CLUTCH_STS <= 0x06))
								{
									Major_Gear_St.data = 0;
									Minor_Gear_St.data = 0;

									gear_to_HMI = GEAR_BLANK;//
									Diag_gear_indc=0x0F;
								}
								else
								{
									gear_to_HMI = GEAR_BLANK;
								}
							}

							/* Manual Transmission - Gear Recommendation Functional Logic */
							/* Checking Shift Up & Shift Down */
							GEAR_UP_FLG   = (((CAN_EMS8_10[1].byte) >> 5) & 1); /* storing 3rd bit */
							GEAR_DOWN_FLG = (((CAN_EMS8_10[1].byte) >> 6) & 1); /* storing 2nd bit */

							if((GEAR_UP_FLG == 0) && (GEAR_DOWN_FLG == 1))
							{
								Shift_UP_St   = 0;
								Shift_Down_St = 1;
								gear_shift_to_HMI=SHIFT_DOWN;
							}
							else if((GEAR_UP_FLG == 1) && (GEAR_DOWN_FLG == 0))
							{
								Shift_UP_St   = 1;
								Shift_Down_St = 0;
								gear_shift_to_HMI=SHIFT_UP;
							}
							else if((GEAR_UP_FLG == 0) && (GEAR_DOWN_FLG == 0))
							{
								Shift_UP_St   = 0;
								Shift_Down_St = 0;
								gear_shift_to_HMI=SHIFT_BLANK; //
							}
							else if((GEAR_UP_FLG == 1) && (GEAR_DOWN_FLG == 1))
							{
								Shift_UP_St   = 0;
								Shift_Down_St = 0;
								gear_shift_to_HMI=SHIFT_BLANK; //
							}

						}


						else /* STS_ENG != 2 */
						{
							Major_Gear_St.data = 0;
							Minor_Gear_St.data = 0;
							Shift_UP_St   = 0;
							Shift_Down_St = 0;

							gear_shift_to_HMI=SHIFT_BLANK; //
							gear_to_HMI = GEAR_BLANK;//
						}
					}
					else
					{
						Major_Gear_St.data = 0;
						Minor_Gear_St.data = 0;
						Shift_UP_St   = 0;
						Shift_Down_St = 0;

						gear_shift_to_HMI=SHIFT_BLANK;
						gear_to_HMI = GEAR_DASH;
						Diag_gear_indc=0x0F;
						Buzz_rever_gr_compl = 0;//test

						/*Only take care for Low End*/
					Buzz_rever_gr_SB=0;
					Buzz_rever_gr=0;
					Buzz_rever_gr_cntr=0;
					Buzz_rever_gr_compl=0;
					}


				} /* end */


				break;

			case 3:
				/* below lines for 19 feb (from #if 1 to #endif) */
#if 1
				/* AT Indication */
				if(FEATURE_CODE_GSI_CONFIG == 0) /* AT Indication Strategy(A) */
				{
					if(TCU6_20_message_timeout_flag == 0)	/*Added for v2.1.4 release*/
					{
					TGS_LEVER = ((CAN_TCU6_20[2].byte) & (0x0F));/* storing Lower nibble of  */

					if(TGS_LEVER == 0x08) /* Reverse */
					{
						Reverse_gear_active = 1;
					}
					else
					{
						Reverse_gear_active = 0;
						Buzz_rever_gr_compl = 0;//test
					}
					/* if(TGS_LEVER == 0xF)   // to log Gear signal invalid DTC
					{
						Failure_DTC_Set(DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073);
					}
					else
					{
						Failure_DTC_Clear(DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073);
					} */

#ifdef GRAYSCALE_TFT
					/* for Reverse Alarm start*/
					if(Diag_RW_Reverse_Chime_Enable==1)
					{
						if(TGS_LEVER == 0x08) /* Reverse */
						{
							/* Display "R" */

							if(((Sound_Bit == 0x0) || (Sound_Bit == 0x40) || (Sound_Bit == 0x200)) && Buzz_rever_gr_compl == 0)
							{
								Sound_Bit = 0x40;
								Buzz_rever_gr=1;
								reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
							}
							else
							{
								Buzz_rever_gr=0;
							}
							Buzz_rever_gr_SB = 1;
						}
						else
						{
							if(Sound_Bit == 0x40)
							{
								Sound_Bit = 0x0;
							}
							Buzz_rever_gr=0;
							Buzz_rever_gr_SB=0;
							Buzz_rever_gr_cntr=0;
							Buzz_rever_gr_compl=0;
							if(Diag_RW_RPAS_Display_Screen_source == 1)
							{
								reverse_played = 1;
							}
							else
							{
								reverse_played = 0;
							}
//							W501_LOW_CounterBz_3sec = 0;
						}
					}
					else
					{
						reverse_played = 1;//to work RPAS chime if reverse chime disable
					}
					/* For Reverse Alarm end*/
#endif

#ifdef COLOR_TFT
					/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........Reverse gear indication*/
					if(Diag_RW_Reverse_Chime_Enable==1)
					{
						if((TGS_LEVER == 0x08)&&(Reversegear_ongoing == 0)/*&&(chime_start1 == 0)*/&&(cycle_Reversegear_completed == 0)) /* Reverse */
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x40 || Sound_Bit == 0x200) && (cycle_Reversegear_completed == 0))
							{
								Sound_Bit = 0x40;						/*0100 0000 - Reverse Alarm sound bit*/
								Seatbelt_ongoing=0;
								Reversegear_ongoing = 1;
								chime_start1 = 1;
								Reversegear_missed = 0;
								reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
							}
							else
							{
								Reversegear_ongoing = 0;
								Reversegear_missed = 0;
							}
							Buzz_rever_gr_SB = 1;						/*Reverse gear occured - used for Seat belt*/
						}
						else if(TGS_LEVER != 0x08)
						{
							if(Sound_Bit == 0x40)
							{
								Sound_Bit = 0x0;						/**Sound bit CLEAR**/
							}
							Reversegear_ongoing = 0;
							cycle_Reversegear_completed = 0;
							Reversegear_missed = 1;
							Buzz_rever_gr_SB = 0;						/*Reverse gear not occured*/
							if(Diag_RW_RPAS_Display_Screen_source == 1)
							{
								reverse_played = 1;
							}
							else
							{
								reverse_played = 0;
							}
						}
					}
					else
					{
						reverse_played = 1;//to work RPAS chime if reverse chime disable
					}
#endif
					if(TGS_LEVER == 0x09) /* Park */
					{
						/* Display "P" */
						gear_to_HMI=AT_P;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x08) /* Reverse */
					{
						/* Display "R" */
						gear_to_HMI=AT_R;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x07) /* Neutral */
					{
						/* Display "N" */
						gear_to_HMI=AT_N;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x06) /* Drive */
					{
						/* Display "D" */
						gear_to_HMI=AT_D;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x0A) /* Tip+ */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x0B) /* Tip- */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0F;
					}
					else if((TGS_LEVER >=0) && (TGS_LEVER <=5)) /* Reserveed */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0F;
					}
					else if(TGS_LEVER == 0x0F) /* Signal Invalid */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0F;
					}
					else if(TGS_LEVER == 0x0E) /* Middle position */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0F;
					}
					else if(TGS_LEVER == 0x0C) /* M forward drive position */
					{
						Gear_Actual = ((CAN_TCU5_10[6].byte) & (0x0F));

						/* if(Gear_Actual == 0xF)   // to log Gear signal invalid DTC
						{
							Failure_DTC_Set(DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072);
						}
						else
						{
							Failure_DTC_Clear(DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072);
						} */

						if(Gear_Actual == 0x01) /* 1st Gear */
						{
							/* Display 1st Gear */
							Gear.data = 0;
							Gear.bitField.First_Gear = 1;
							gear_to_HMI=AT_1;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x02) /* 2nd Gear */
						{
							/* Display 2nd Gear */
							Gear.data = 0;
							Gear.bitField.Second_Gear = 1;
							gear_to_HMI=AT_2;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x03) /* 3rd Gear */
						{
							/* Display 3rd Gear */
							Gear.data = 0;
							Gear.bitField.Third_Gear = 1;
							gear_to_HMI=AT_3;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x04) /* 4th Gear */
						{
							/* Display 4th Gear */
							Gear.data = 0;
							Gear.bitField.Fourth_Gear = 1;
							gear_to_HMI=AT_4;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x05) /* 5th Gear */
						{
							/* Display 5th Gear */
							Gear.data = 0;
							Gear.bitField.Fifth_Gear = 1;
							gear_to_HMI=AT_5;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x06) /* 6th Gear */
						{
							/* Display 6th Gear */
							Gear.data = 0;
							Gear.bitField.Sixth_Gear = 1;
							gear_to_HMI=AT_6;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x00) /* Neutral */
						{
							/* Display Blank */
							Gear.data = 0;
							gear_to_HMI=GEAR_BLANK;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x0A) /* Reverse */
						{
							/* Display Blank */
							Gear.data = 0;
							gear_to_HMI=GEAR_BLANK;
							Diag_gear_indc=0x0F;
						}
						else if(((Gear_Actual >= 0x07) && (Gear_Actual <= 0x09))
								|| ((Gear_Actual >= 0x0B) && (Gear_Actual <= 0x0F))) /* 6th Gear */
						{
							/* Display Blank */
							Gear.data = 0;
							gear_to_HMI=GEAR_BLANK;
							Diag_gear_indc=0x0F;
						}
					}
					gear_shift_to_HMI=SHIFT_BLANK;
					}
					else
					{							/*Only take care for Low End*/
						Buzz_rever_gr_SB=0;
						Buzz_rever_gr=0;
						Buzz_rever_gr_cntr=0;
						Buzz_rever_gr_compl=0;
						Reversegear_ongoing = 0;//Reverse chime for W501H
						cycle_Reversegear_completed = 0;
					}
				}
#endif

				else if(FEATURE_CODE_GSI_CONFIG == 1) /* AT Indication Strategy(B) */
				{
					if(TCU6_20_message_timeout_flag == 0)	/*Added for v2.1.4 release*/
				{
					TGS_LEVER = ((CAN_TCU6_20[2].byte) & (0x0F));/* storing Lower nibble of  */

					if(TGS_LEVER == 0x08) /* Reverse */
					{
						Reverse_gear_active = 1;
					}
					else
					{
						Reverse_gear_active = 0;
						Buzz_rever_gr_compl = 0;//test
					}
					/* if(TGS_LEVER == 0xF)   // to log Gear signal invalid DTC
					{
						Failure_DTC_Set(DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073);
					}
					else
					{
						Failure_DTC_Clear(DTC_TCU_TGS_LEVER_SIGNAL_CONTFAIL_E073);
					} */
#ifdef GRAYSCALE_TFT
					/* for Reverse Alarm start*/
					if(Diag_RW_Reverse_Chime_Enable==1)
					{
						if(TGS_LEVER == 0x08) /* Reverse */
						{
							/* Display "R" */
							if(((Sound_Bit == 0x0) || (Sound_Bit == 0x40) || (Sound_Bit == 0x200)) && Buzz_rever_gr_compl == 0)
							{
								Sound_Bit = 0x40;
								Buzz_rever_gr=1;
								reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
							}
							else
							{
								Buzz_rever_gr=0;
								if(Sound_Bit == 0x40)
								{
									Sound_Bit = 0x0;
								}
							}
							Buzz_rever_gr_SB = 1;
						}
						else
						{
							if(Sound_Bit == 0x40)
							{
								Sound_Bit = 0x0;
							}
							if(Diag_RW_RPAS_Display_Screen_source == 1)
							{
								reverse_played = 1;
							}
							else
							{
								reverse_played = 0;
							}
							Buzz_rever_gr_SB=0;
							Buzz_rever_gr=0;
							Buzz_rever_gr_cntr=0;
							Buzz_rever_gr_compl=0;
//							W501_LOW_CounterBz_3sec = 0;
						}
					}
					else
					{
						reverse_played = 1;//to work RPAS chime if reverse chime disable
					}
					/* For Reverse Alarm end*/
#endif

#ifdef COLOR_TFT
					/*4rec_RSST_nSndlD_RPAS_St1_St2_cont_16k - WAV.........Reverse gear indication*/
					if(Diag_RW_Reverse_Chime_Enable==1)
					{
						if((TGS_LEVER == 0x08)&&(Reversegear_ongoing == 0)&&/*(chime_start1 == 0)&&*/(cycle_Reversegear_completed == 0)) /* Reverse */
						{
							if((Sound_Bit == 0x0 || Sound_Bit == 0x40 || Sound_Bit == 0x200)  && (cycle_Reversegear_completed == 0))
							{
								Sound_Bit = 0x40;						/*0100 0000 - Reverse Alarm sound bit*/
								Seatbelt_ongoing=0;
								Reversegear_ongoing = 1;
								chime_start1 = 1;
								Reversegear_missed = 0;
								reverse_played = 1;		/*Flag added for RPAS Level chime should play after reverse*/
							}
							else
							{
								Reversegear_ongoing = 0;
								Reversegear_missed = 0;
							}
							Buzz_rever_gr_SB = 1;						/*Reverse gear occured - used for Seat belt*/
						}
						else if(TGS_LEVER != 0x08)
						{
							if(Sound_Bit == 0x40)
							{
								Sound_Bit = 0x00;						/**Sound bit CLEAR**/
							}
							Reversegear_ongoing = 0;
							cycle_Reversegear_completed = 0;
							Reversegear_missed = 1;
							Buzz_rever_gr_SB = 0;						/*Reverse gear not occured*/
							if(Diag_RW_RPAS_Display_Screen_source == 1)
							{
								reverse_played = 1;
							}
							else
							{
								reverse_played = 0;
							}
						}
					}
					else
					{
						reverse_played = 1;//to work RPAS chime if reverse chime disable
					}
#endif

					if(TGS_LEVER == 0x09) /* Park */
					{
						/* Display "P" */
						gear_to_HMI=AT_P;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x08) /* Reverse */
					{
						/* Display "R" */
						gear_to_HMI=AT_R;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x07) /* Neutral */
					{
						/* Display "N" */
						gear_to_HMI=AT_N;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x06) /* Drive */
					{
						/* Display "D" */
						gear_to_HMI=AT_D;
						Diag_gear_indc=TGS_LEVER;
					}
					else if(TGS_LEVER == 0x0A) /* Tip+ */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0f;
					}
					else if(TGS_LEVER == 0x0B) /* Tip- */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0f;
					}
					else if((TGS_LEVER >=0) && (TGS_LEVER <=5)) /* Reserveed */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0f;
					}
					else if(TGS_LEVER == 0x0F) /* Signal Invalid */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0f;
					}
					else if(TGS_LEVER == 0x0E) /* Middle position */
					{
						/* Display "Blank" */
						gear_to_HMI=GEAR_BLANK;
						Diag_gear_indc=0x0f;
					}
					else if(TGS_LEVER == 0x0C) /* M forward drive position */
					{
						Gear_Actual = ((CAN_TCU5_10[6].byte) & (0x0F));

						/* if(Gear_Actual == 0xF)   // to log Gear signal invalid DTC
						{
							Failure_DTC_Set(DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072);
						}
						else
						{
							Failure_DTC_Clear(DTC_TCU_GEAR_ACTUAL_SIGNAL_CONTFAIL_E072);
						} */


						if(Gear_Actual == 0x01) /* 1st Gear */
						{
							/* Display 1st Gear */
							Gear.data = 0;
							Gear.bitField.First_Gear = 1;
							gear_to_HMI=AT_1;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x02) /* 2nd Gear */
						{
							/* Display 2nd Gear */
							Gear.data = 0;
							Gear.bitField.Second_Gear = 1;
							gear_to_HMI=AT_2;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x03) /* 3rd Gear */
						{
							/* Display 3rd Gear */
							Gear.data = 0;
							Gear.bitField.Third_Gear = 1;
							gear_to_HMI=AT_3;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x04) /* 4th Gear */
						{
							/* Display 4th Gear */
							Gear.data = 0;
							Gear.bitField.Fourth_Gear = 1;
							gear_to_HMI=AT_4;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x05) /* 5th Gear */
						{
							/* Display 5th Gear */
							Gear.data = 0;
							Gear.bitField.Fifth_Gear = 1;
							gear_to_HMI=AT_5;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x06) /* 6th Gear */
						{
							/* Display 6th Gear */
							Gear.data = 0;
							Gear.bitField.Sixth_Gear = 1;
							gear_to_HMI=AT_6;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x00) /* Neutral */
						{
							/* Display Blank */
							Gear.data = 0;
							gear_to_HMI=GEAR_BLANK;
							Diag_gear_indc=Gear_Actual;
						}
						else if(Gear_Actual == 0x0A) /* Reverse */
						{
							/* Display Blank */
							Gear.data = 0;
							gear_to_HMI=GEAR_BLANK;
							Diag_gear_indc=0x0F;
						}
						else if(((Gear_Actual >= 0x07) && (Gear_Actual <= 0x09))
								|| ((Gear_Actual >= 0x0B) && (Gear_Actual <= 0x0F))) /* 6th Gear */
						{
							/* Display Blank */
							Gear.data = 0;
							gear_to_HMI=GEAR_BLANK;
							Diag_gear_indc=0x0F;
						}

					}
					/* Checking Shift Up & Shift Down */
					GEAR_UP_FLG   = (((CAN_EMS8_10[1].byte) >> 5) & 1); /* storing 3rd bit */
					GEAR_DOWN_FLG = (((CAN_EMS8_10[1].byte) >> 6) & 1); /* storing 2nd bit */
					if((GEAR_UP_FLG == 0) && (GEAR_DOWN_FLG == 1))
					{
						Shift_UP_St   = 0;
						Shift_Down_St = 1;
						gear_shift_to_HMI=SHIFT_DOWN;
					}
					else if((GEAR_UP_FLG == 1) && (GEAR_DOWN_FLG == 0))
					{
						Shift_UP_St   = 1;
						Shift_Down_St = 0;
						gear_shift_to_HMI=SHIFT_UP;
					}
					else if((GEAR_UP_FLG == 0) && (GEAR_DOWN_FLG == 0))
					{
						Shift_UP_St   = 0;
						Shift_Down_St = 0;
						gear_shift_to_HMI=SHIFT_BLANK;
					}
					else if((GEAR_UP_FLG == 1) && (GEAR_DOWN_FLG == 1))
					{
						Shift_UP_St   = 0;
						Shift_Down_St = 0;
						gear_shift_to_HMI=SHIFT_BLANK;
					}
					}
					else
					{
						Buzz_rever_gr_SB=0;
						Buzz_rever_gr=0;
						Buzz_rever_gr_cntr=0;
						Buzz_rever_gr_compl=0;
						Reversegear_ongoing = 0;//Reverse chime for W501H
						cycle_Reversegear_completed = 0;
					}
				}

				break;

			default:
				break;

			}

		}
		else
		{
			Major_Gear_St.data = 0;
			Minor_Gear_St.data = 0;
			Shift_UP_St   = 0;
			Shift_Down_St = 0;

			gear_shift_to_HMI = SHIFT_BLANK;
			gear_to_HMI = GEAR_BLANK;
		}
	}
	else
	{
		Major_Gear_St.data = 0;
		Minor_Gear_St.data = 0;
		Shift_UP_St   = 0;
		Shift_Down_St = 0;
	}

}

uint8_t IO_Control_Gear_indication(void)
{
	uint8_t gear_return_sts;
	if(Gear_config == 1)//MT
	{
		if(IO_Control_Enable_Gear_IND.byte & 0x01)
		{
			if(IO_Control_State_Gear_IND_Status_Byte0.byte <= 7)
			{
				gear_return_sts = IO_Control_State_Gear_IND_Status_Byte0.byte;        // display N
				Diag_gear_indc = IO_Control_State_Gear_IND_Status_Byte0.byte;
			}
			else if((IO_Control_State_Gear_IND_Status_Byte0.byte>7)&&(IO_Control_State_Gear_IND_Status_Byte0.byte<=14))
			{
				gear_return_sts = 55;      // display Blank
				Diag_gear_indc = 0x0F;
			}
			else
			{
				gear_return_sts = 18;      // display --
				Diag_gear_indc = 0x0F;
			}
		}
		else
		{
			gear_return_sts = 55;      // display Blank
			Diag_gear_indc = 0x0F;

		}
	}
	else if(Gear_config == 2)//AT
	{
		if(IO_Control_Enable_Gear_IND.byte & 0x01)
		{
			if((IO_Control_State_Gear_IND_Status_Byte0.byte < 5)||((IO_Control_State_Gear_IND_Status_Byte0.byte > 10)&&(IO_Control_State_Gear_IND_Status_Byte0.byte <=14)))
			{
				gear_return_sts = 55;        // display blank
				Diag_gear_indc = 0x0F;
			}
			else if(IO_Control_State_Gear_IND_Status_Byte0.byte==0x0F)
			{
				gear_return_sts = 18;      // display Blank
				Diag_gear_indc = 0x0F;
			}
			else
			{
				gear_return_sts = 23-IO_Control_State_Gear_IND_Status_Byte0.byte;      // display gear
				Diag_gear_indc = IO_Control_State_Gear_IND_Status_Byte0.byte;
			}
		}
		else
		{
			gear_return_sts = 55;      // display Blank
			Diag_gear_indc = 0x0F;

		}
	}
	return gear_return_sts;
}
uint8_t IO_Control_Gear_shift_indication(void)
{
	uint8_t gear_shift_sts;
	if(Gear_config == 1)
	{
		if((IO_Control_Enable_Gear_IND.byte & 0x04)||(IO_Control_Enable_Gear_IND.byte & 0x08))
		{
			if((IO_Control_State_Gear_IND_Status_Byte1.byte & 0x01)&&(IO_Control_Enable_Gear_IND.byte & 0x04))
			{
				gear_shift_sts = 0;          //display Up
				gear_shift_to_HMI=0;
			}
			else if((IO_Control_Enable_Gear_IND.byte & 0x08)&& (IO_Control_State_Gear_IND_Status_Byte1.byte & 0x02))
			{
				gear_shift_sts = 1;          //display D
				gear_shift_to_HMI=1;
			}
			else
			{
				gear_shift_sts = 2;          //display OFF
			}
		}
		else
		{
			gear_shift_sts = 2;          //display OFF
		}
	}
	return gear_shift_sts;
}
