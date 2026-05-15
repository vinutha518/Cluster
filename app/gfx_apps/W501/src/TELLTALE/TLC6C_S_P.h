/*
 * TLC6C_S_P.h
 *
 *  Created on: 28-Jan-2019
 *      Author: MSWELT19
 */

#ifndef TLC6C_S_P_H_
#define TLC6C_S_P_H_









/****IOpin
DLED_GCLK	P17_8
DLED_BLANK	P17_7
DLED_ERRO	P1_5
DLED_SDI	P1_1
DLED_SCL	P1_2
DLED_LATCH	P1_4
DLED_SDO	P1_0
****/

#ifdef PCBrevB
#define MIL_TT							GSdata1[0]
#define Check_Engine_TT					GSdata1[1]
#define EPS_MALFUNCTION_TT				GSdata1[2]
#define ESP_OFF_TT						GSdata1[4]
#define ESP_MALFUNCTION_TT				GSdata1[3]
#define ABS_TT							GSdata1[5]
#define PARK_BRAKE_LOW_FLUID_EBD_TT		GSdata1[7]
#define AIR_BAG_TT						GSdata1[6]
#define	AT_FAIL_TT						GSdata1[8]
#define	CRUISE_TT						GSdata1[10]
#define	AUTO_START_STOP_TT				GSdata1[9]
#define	PRESS_BRAKE_PEDAL_TT			GSdata1[11]
#define	HHC_MALFUNCTION_TT				GSdata1[14]
#define	GLOBAL_TPMS_MULFUNCTION_TT		GSdata1[12]
#define	HDC_TT							GSdata1[13]
#define	Turn_right_TT					GSdata1[17]
#define	Turn_left_TT					GSdata1[15]
#define	AEBS_MULFUNCTION_ON_TT			GSdata1[16]
#define	DEF_TT							GSdata1[20]
#define	High_beam_TT					GSdata1[18]
#define	DPF_TT							GSdata1[19]
#define	Low_Oil_press_TT				GSdata1[23]
#define	ECT_high_TT						GSdata1[21]
#define	Batt_chrg_TT					GSdata1[22]
#else
#define Turn_left_TT 						GSdata1[0]  //R0
#define  MIL_TT 					      	GSdata1[1]
#define AEBS_MULFUNCTION_ON_TT				GSdata1[2]
#define CRUISE_TT 							GSdata1[3]    //R1
#define GLOBAL_TPMS_MULFUNCTION_TT 			GSdata1[4]
#define EPS_MALFUNCTION_TT 					GSdata1[5]
#define HDC_TT 								GSdata1[6]   //R2
#define ESP_OFF_TT							GSdata1[7]
#define	PARK_BRAKE_LOW_FLUID_EBD_TT			GSdata1[8]
#define	ECT_high_TT							GSdata1[9]    //R3
#define	ESP_MALFUNCTION_TT					GSdata1[10]
#define	HHC_MALFUNCTION_TT 					GSdata1[11]
#define	RP_LD3_TT								GSdata1[12]
#define	RP_LD2_TT								GSdata1[13]
#define	PRESS_BRAKE_PEDAL_TT				GSdata1[14]
#define	R_LD8_TT								GSdata1[15]
#define	R_LD7_TT							GSdata1[16]
#define	RP_LD1_TT							GSdata1[17]
#define	R_LD5_TT							GSdata1[18]
#define	R_LD4_TT								GSdata1[19]
#define	R_LD6_TT							GSdata1[20]
#define	R_LD2_TT							GSdata1[21]
#define	R_LD1_TT							GSdata1[22]
#define	R_LD3_TT								GSdata1[23]

#define S_LD2_TT						    GSdata1[24]//2R0
#define S_LD1_TT							GSdata1[25]
#define S_LD3_TT  							GSdata1[26]
#define S_LD5_TT 							GSdata1[27]//2R1
#define S_LD4_TT 							GSdata1[28]//2G1
#define S_LD6_TT							GSdata1[29]//2B1
#define SP_LD2_TT							GSdata1[30]//2R2
#define SP_LD1_TT							GSdata1[31]
#define	SP_LD3_TT							GSdata1[32]
#define	DEF_TT								GSdata1[33]//2R3
#define	DPF_TT								GSdata1[34]
#define	High_beam_TT						GSdata1[35]
#define	Low_Oil_press_TT					GSdata1[36]//2R4
#define	AUTO_START_STOP_TT					GSdata1[37]
#define	Check_Engine_TT						GSdata1[38]
#define	AIR_BAG_TT							GSdata1[39]//2R5
#define	Turn_right_TT						GSdata1[40]
#define	AT_FAIL_TT							GSdata1[41]
#define	Batt_chrg_TT						GSdata1[42]//2R6
#define	SPEAR_TT1							GSdata1[43]
#define	ABS_TT								GSdata1[44]
#define	SPEAR_TT2							GSdata1[45]//2R7
#define	SPEAR_TT3							GSdata1[46]
#define	SPEAR_TT4							GSdata1[47]

#endif



void SendDCData(int CMD, int UD, char GS_MODE, char DSTMR, char ADSRPM, char DC_Range_Blue, char DC_Range_Green, char DC_Range_Red, char BCB, char BCG, char BCR, char *DCdata);//Send DC/BC/FC data thru GS Data Path
void SendGSData(int *temp);			// Send Gray Scale data

void DIAG_TT_init(void);
void ReadDCData(void);
void IP_SHIFT_DELAY_Alt (void);
void Illumination_steps(void);
void Illumination_Proveout(void);

void load_tst_tt_flags(uint8_t );	/*This Function Loads Value before IGN OFF to Global Flags */


#ifdef PCBrevB
#else
void cascade_TT_send_data(void);
#endif

extern char DCdata0[];
extern char ERROR_Clear[];
extern char GLOBAL_RESET[];
extern char DCdata1[];
extern char DCdata2[];
extern int GSdata0[];
extern int GSdataON[];
extern int GSdata1[];

extern int read_SID[576];
extern _Bool DC_data_process_flag;
#endif /* TLC6C_S_P_H_ */
