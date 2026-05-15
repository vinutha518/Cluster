#include "r_typedefs.h"
//#include "dr7f701441.dvf.h"
#include "dr7f701461.dvf.h"
#include "my_can.h"
#include "parallel_to_serial.h"
//#include "100_us_timer.h"
#include "r_gpio_api.h"
#include "UDS_DTC.h"
#include "UDSS.h"
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "digitalGuage.h"
#include "alert.h"
#include "gui.h"
#include "init.h"
#include "VIN_Mismatch.h"
#include "batt.h"
#include "sound_pcm.h"
//#include "my_userdefine.h"

//**************************************************************************************************************************
#define my_protected_write(preg,reg,value)   preg=0xa5;\
		reg=value;\
		reg=~value;\
		reg=value;
#define IGN 	R_GPIO_PinRead(0,1)//	PORT_AWOP0_1  /* Ignition */

unsigned long ver;
unsigned char ver1;
uint8_t UDS_Buffer[250]={0};
uint8_t TX_DATA[8];
extern unsigned char self_test_done;
extern _Bool IVN_RxMAC_Indicator;
extern uint8_t IVN_RxNSMMsgArrive;
extern uint8_t IVN_RxSynchMsgArrive;
extern _Bool IVN_SecurityEnableFlag;
extern Split32 IVN_MacRxBuffer;
extern Split32 IVN_MAC0RxBuffer;
extern uint8_t IGNOFF_display_timeout;
//extern uint16_t DTC_ID;
unsigned char tx_done;
unsigned char rx_flag;
unsigned int i;
unsigned char rcvd_key;

unsigned char my_speed_val;
unsigned char my_immo_val;
unsigned int my_rpm_val;
unsigned char my_menu_cmd;
unsigned int my_batt_vg = 480;

/* ************************************************************************** */
unsigned char Network_Message_Monitor_Start = 0;
unsigned char EMS36_10_message_ON;
unsigned char ESC2_10_message_ON;
unsigned char EMS1_10_message_ON;
unsigned char EMS3_10_message_ON;
unsigned char ESC5_10_message_ON;
unsigned char EMS8_10_message_ON;
unsigned char EMS4_20_message_ON;
unsigned char TCU5_10_message_ON;
unsigned char MBFM5_100_message_ON;
unsigned char EMS29_100_message_ON;
unsigned char TCU6_20_message_ON;
unsigned char ESC12_10_message_ON;
unsigned char SRS1_20_message_ON;
unsigned char EMS6_500_message_ON;
unsigned char EMS12_200_message_ON;
unsigned char PKE_ICU2_100_message_ON;
unsigned char MBFM1_100_message_ON;
unsigned char MBFM9_500_message_ON;
unsigned char IS3_500_message_ON;
unsigned char IS1_100_message_ON;
unsigned char EMS_NSM_message_ON;
unsigned char TCU_NSM_message_ON;
unsigned char ESC_NSM_message_ON;
unsigned char IS_NSM_message_ON;
unsigned char RPAS1_20_message_ON;
unsigned char RPAS2_100_message_ON;
unsigned char EMS5_500_message_ON;
unsigned char MBFM_PAS1_50_message_ON;
unsigned char IGNOFF_MBFM1_100_message_timeout_flag;
//#ifdef SPRINT4
unsigned char IS16_1000_message_ON;
unsigned char ESC7_20_message_ON;
/* TDMS */
unsigned char SAS1_10_message_ON;
/* ECO_SCORE */
unsigned char IS4_500_message_ON;
/* Audio Info Display */
unsigned char IS14_200_message_ON;
unsigned char IS15_200_message_ON;
//#endif
_Bool CAN_init_done = CLEAR;

uint8_t display_active;
uint8_t IGNOFF_MBFM_CAN_ON;
_Bool audio_init_done = 0;
uint32_t MBFM_msgtimeout = 0;
extern _Bool DOOR_OPEN_IGN_OFF;
extern _Bool IMMO_IGN_OFF;

uint16_t Network_Message_Monitor_Counter = 0;
uint16_t IGNOFF_MBFM1_100_Message_Period_Counter = 0;
uint16_t IGNOFF_CAN_counter = 0;
/************msg_timeout variables***********************************/
uint16_t IGNOFF_MBFM1_100_Message_Recovery_Counter = 0;


uint8_t IVN_ESC2_10_Msgtimeout_Cnt_u1g=0;
uint8_t IVN_ESC12_10_Msgtimeout_Cnt_u1g=0;
//uint8_t ESC2_10_Message_Recovery_Counter=0;
//uint8_t ESC12_10_Message_Recovery_Counter=0;
/**************msg_content_variables********************************/
uint16_t Message_Content_Failure_Threshold;
uint16_t Message_Content_Recovery_Threshold;

Content_Failure_t EMS1_10_Message_Content;
Content_Failure_t EMS36_10_Message_Content;
Content_Failure_t EMS3_10_Message_Content;
Content_Failure_t EMS8_10_Message_Content;
Content_Failure_t EMS4_20_Message_Content;
Content_Failure_t EMS29_100_Message_Content;
Content_Failure_t EMS6_500_Message_Content;
Content_Failure_t EMS12_200_Message_Content;
Content_Failure_t EMS5_500_Message_Content;

Content_Failure_t ESC5_10_Message_Content;	//NA
Content_Failure_t ESC2_10_Message_Content;
Content_Failure_t ESC12_10_Message_Content;

Content_Failure_t TCU5_10_Message_Content;	//NA
Content_Failure_t TCU6_20_Message_Content;

Content_Failure_t MBFM1_100_Message_Content;	//NA
Content_Failure_t MBFM5_100_Message_Content;
Content_Failure_t MBFM9_500_Message_Content;
Content_Failure_t MBFM_PAS1_50_Message_Content;

Content_Failure_t SRS1_20_Message_Content;	//NA

Content_Failure_t PKE_ICU2_100_Message_Content;	//NA

Content_Failure_t RPAS1_20_Message_Content;	//NA
Content_Failure_t RPAS2_100_Message_Content;

Content_Failure_t IS1_100_Message_Content;	//NA
Content_Failure_t IS3_500_Message_Content;
//#ifdef SPRINT4
/* TDMS */
Content_Failure_t SAS1_10_Message_Content;
//#endif

/**************Signal_content_counter_variables*********************/
uint16_t Signal_Content_Failure_Threshold;
uint16_t Signal_Content_Recovery_Threshold;

Content_Failure_t EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content;
Content_Failure_t EMS1_10_Temp_Sig_Content;
Content_Failure_t EMS1_10_RPM_Sig_Content;
Content_Failure_t EMS1_10_GEAR_Sig_Content;
Content_Failure_t EMS1_10_INDC_CRUISE_Sig_Content;
Content_Failure_t EMS3_10_CLUCH_Sig_Content;
Content_Failure_t EMS3_10_STS_ESS_INDC_Sig_Content;
Content_Failure_t EMS8_10_TARGET_GEAR_Sig_Content;
Content_Failure_t EMS4_20_ENG_DRIVE_MODE_Sig_Content;
Content_Failure_t TCU5_10_GEAR_ACTUAL_Sig_Content;
Content_Failure_t TCU5_10_GEAR_TARGET_Sig_Content;
Content_Failure_t TCU6_20_TGS_LEVER_Sig_Content;
Content_Failure_t TCU6_20_MODE_SIGNAL_Sig_Content;
Content_Failure_t TCU6_20_INDC_AT_MALFUNC_Sig_Content;
Content_Failure_t SRS1_20_INDC_SRSC_Sig_Content;
Content_Failure_t EMS6_500_DISP_AMBT_TEMP_Sig_Content;
Content_Failure_t MBFM1_100_MBFM_ENGOFF_Sig_Content;
Content_Failure_t RPAS1_20_DISP_DIST_Sig_Content;
Content_Failure_t ESC2_10_ABS_VEH_SPEED_Sig_Content;
Content_Failure_t ESC2_10_ABS_ODO_DIST_Sig_Content;
#ifdef SPRINT4
/* TDMS */
Content_Failure_t SAS1_10_ABOLUTE_ANGLE_Sig_Content;
Content_Failure_t SAS1_10_STS_SAS_FAILURE_Sig_Content;
Content_Failure_t SAS1_10_STS_SAS_CALIB_Sig_Content;
Content_Failure_t SAS1_10_STS_SAS_TRIM_Sig_Content;
Content_Failure_t SAS1_10_SAS_MSG_CNT_Sig_Content;
Content_Failure_t SAS1_10_SAS_CRC_Sig_Content;
#endif
/* ************************************************************************** */

#define EMS36_10 		0x142 /*Engine speed. Used for tacho analog gauge*/
#define ESC2_10 		0x2CC /*Odo and speedo gauge Note: 2different signals for odo and speedo*/
#define EMS1_10 		0x124 /*Engine running status*/
#define EMS3_10 		0x108
#define ESC5_10 		0x10D
#define EMS8_10 		0x126
#define EMS4_20 		0x130
#define TCU5_10 		0x136
#define MBFM5_100 		0x214
#define EMS29_100 		0x233
#define TCU6_20 		0x278
#define ESC12_10 		0x282
#define SRS1_20 		0x2C0
#define EMS6_500 		0x308
#define EMS12_200 		0x310
#define PKE_ICU2_100 	0x342
#define MBFM1_100 		0x348
#define IS3_500 		0x3CA
#define IS1_100 		0x220
#define MBFM9_500       0x21F
#define RPAS1_20 		0x227
#define RPAS2_100		0x229
#define EMS5_500		0x3C0
#define MBFM_PAS1_50	0x326		/*ADDED FOR W501*/

//nsm
#define EMS_NSM			0x285
#define ESC_NSM			0x289
#define IS_NSM			0x28E
#define PKE_ICU_NSM		0x287
#define SRS_NSM			0x28C
#define TCU_NSM			0x288
#define SYNC_MSG_SP		0x286
#ifdef SPRINT4
#define ESC7_20			0x170	/*ADDED FOR W501*/
#define IS16_1000       0x666
/* TDMS */
#define SAS1_10         0x114
/* ECO_SCORE */
#define IS4_500         0x3CB
/* Audio Info Display */
#define IS14_200         0x57E
#define IS15_200         0x57D
#endif
//**************************************************************************************************************************
//**************************************************************************************************************************
unsigned char rx_msg_DLC;
unsigned int rx_msg_ID;
uint32_t rx_msg_data0, rx_msg_data1, rx_msg_rmnd;

/**************W501********************/
RX_DATA CAN_EMS1_10[8]     = {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS36_10[8]    = {0,0,0,0,0,0,0,0};
RX_DATA CAN_ESC2_10[8]  	= {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS3_10[8]    	= {0,0,0,0,0,0,0,0};
RX_DATA CAN_ESC5_10[8]			= {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS8_10	[8]		= {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS4_20	[8]		= {0,0,0,0,0,0,0,0};
RX_DATA CAN_TCU5_10  [8]       = {0,0,0,0,0,0,0,0};
RX_DATA CAN_MBFM5_100 [8]       = {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS29_100 [8]       = {0,0,0,0,0,0,0,0};
RX_DATA CAN_TCU6_20 [8]         = {0,0,0,0,0,0,0,0};
RX_DATA CAN_ESC12_10 [8]        = {0,0,0,0,0,0,0,0};
RX_DATA CAN_SRS1_20[8]          = {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS6_500 [8]        = {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS12_200[8]        = {0,0,0,0,0,0,0,0};
RX_DATA CAN_PKE_ICU2_100[8]     = {0,0,0,0,0,0,0,0};
RX_DATA CAN_MBFM1_100[8]        = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IS3_500[8]          = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IS1_100[8]          = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IC1_100[8]          = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IC2_100[8]          = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IC7_1000_TEST[8]    = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IC6_1000_TEST[8]    = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IC4_1000_TEST[8]    = {0,0,0,0,0,0,0,0};
RX_DATA CAN_MBFM9_500[8]        = {0,0,0,0,0,0,0,0};

//NSM Message Rx
RX_DATA CAN_EMS_NSM[8]     = {0,0,0,0,0,0,0,0};
RX_DATA CAN_ESC_NSM[8]     = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IS_NSM[8]      = {0,0,0,0,0,0,0,0};
RX_DATA CAN_PKE_ICU_NSM[8] = {0,0,0,0,0,0,0,0};
RX_DATA CAN_SRS_NSM[8]     = {0,0,0,0,0,0,0,0};
RX_DATA CAN_TCU_NSM[8]     = {0,0,0,0,0,0,0,0};
RX_DATA CAN_SYNC_MSG_SP[8] = {0,0,0,0,0,0,0,0};
RX_DATA CAN_RPAS1_20[8]    = {0,0,0,0,0,0,0,0};
RX_DATA CAN_RPAS2_100[8]   = {0,0,0,0,0,0,0,0};
RX_DATA CAN_EMS5_500[8]	   = {0,0,0,0,0,0,0,0};
RX_DATA CAN_MBFM_PAS1_50[8]= {0,0,0,0,0,0,0,0};
//#ifdef SPRINT4
RX_DATA CAN_IS16_1000[8]   = {0,0,0,0,0,0,0,0};
RX_DATA CAN_ESC7_20[8]     = {0,0,0,0,0,0,0,0};
/* TDMS */
RX_DATA CAN_SAS1_10[8]	   = {0,0,0,0,0,0,0,0};
/* ECO_SCORE */
RX_DATA CAN_IS4_500[8]	   = {0,0,0,0,0,0,0,0};
/* Audio Info Display */
RX_DATA CAN_IS14_200[8]	   = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IS15_200[8]	   = {0,0,0,0,0,0,0,0};
//#endif
//Tx
RX_DATA CAN_IC_NSM[8]      = {0,0,0,0,0,0,0,0};
RX_DATA CAN_IC10_200[8]	   = {0,0,0,0,0,0,0,0};

RX_DATA CAN_SRS1_20_ext[8]          = {0,0,0,0,0,0,0,0};

#define         MSGBUF_BASE_ADD       ((uint32_t)(0xFFD00608))
#define         MSGBUF1_BASE_ADD	  ((uint32_t)(0xFFD00618))
#define         MSGBUF2_BASE_ADD	  ((uint32_t)(0xFFD00628))
#define         MSGBUF3_BASE_ADD      ((uint32_t)(0xFFD00638))
#define         MSGBUF4_BASE_ADD      ((uint32_t)(0xFFD00648))
#define         MSGBUF5_BASE_ADD      ((uint32_t)(0xFFD00658))
#define         MSGBUF6_BASE_ADD      ((uint32_t)(0xFFD00668))
#define         MSGBUF7_BASE_ADD      ((uint32_t)(0xFFD00678))

#define         MSGBUF8_BASE_ADD      ((uint32_t)(0xFFD00688))
#define         MSGBUF9_BASE_ADD      ((uint32_t)(0xFFD00698))
#define         MSGBUF10_BASE_ADD      ((uint32_t)(0xFFD006A8))
#define         MSGBUF11_BASE_ADD      ((uint32_t)(0xFFD006B8))
#define         MSGBUF12_BASE_ADD      ((uint32_t)(0xFFD006C8))
#define         MSGBUF13_BASE_ADD      ((uint32_t)(0xFFD006D8))
#define         MSGBUF14_BASE_ADD      ((uint32_t)(0xFFD006E8))

#define         MSGBUF15_BASE_ADD      ((uint32_t)(0xFFD006F8))
#define         MSGBUF16_BASE_ADD      ((uint32_t)(0xFFD00708))
#define         MSGBUF17_BASE_ADD      ((uint32_t)(0xFFD00718))
#define         MSGBUF18_BASE_ADD      ((uint32_t)(0xFFD00728))
#define         MSGBUF19_BASE_ADD      ((uint32_t)(0xFFD00738))
//NSM Message Rx
#define         MSGBUF20_BASE_ADD      ((uint32_t)(0xFFD00748))
#define         MSGBUF21_BASE_ADD      ((uint32_t)(0xFFD00758))
#define         MSGBUF22_BASE_ADD      ((uint32_t)(0xFFD00768))
#define         MSGBUF23_BASE_ADD      ((uint32_t)(0xFFD00778))
#define         MSGBUF24_BASE_ADD      ((uint32_t)(0xFFD00788))
#define         MSGBUF25_BASE_ADD      ((uint32_t)(0xFFD00798))
#define         MSGBUF26_BASE_ADD      ((uint32_t)(0xFFD007A8))
//#ifdef SPRINT4
#define         MSGBUF27_BASE_ADD      ((uint32_t)(0xFFD007B8)) // 9B
#define         MSGBUF28_BASE_ADD      ((uint32_t)(0xFFD007C8)) // 9C
#define         MSGBUF29_BASE_ADD      ((uint32_t)(0xFFD007D8)) // 9D
#define         MSGBUF30_BASE_ADD      ((uint32_t)(0xFFD007E8)) // 9E
#define         MSGBUF31_BASE_ADD      ((uint32_t)(0xFFD007F8)) // 9F
/* TDMS */
#define         MSGBUF32_BASE_ADD      ((uint32_t)(0xFFD00808)) // A0
/* ECO_SCORE */
#define         MSGBUF33_BASE_ADD      ((uint32_t)(0xFFD00818)) // A1
/* AUDIO_INFO Display */
#define         MSGBUF34_BASE_ADD      ((uint32_t)(0xFFD00828)) // A2
#define         MSGBUF35_BASE_ADD      ((uint32_t)(0xFFD00838)) // A3            
//#endif

uint8_t * MsgBuf_address;

uint8_t frame_type;
Frame FrameType;
#define SET		1U
#define CLEAR		0U

//unsigned short P2CAN_ECU_Cntr;
///_Bool ext_session_active;
//_Bool prog_session_active;
void tx_msgbuf_processing(unsigned char ,unsigned char* );
void Signal_Content_Failure_Verication(Content_Failure_t * Content_Status, uint32_t Signal_Value,
	uint32_t Min_Value, uint32_t Max_Value, uint32_t Invalid_Value);
void Message_Content_Failure_Verication(Content_Failure_t * Content_Status, uint8_t Rx_Msg_DLC);
#ifdef SPRINT4
void Message_Content_Failure_Verication_SAS(Content_Failure_t * Content_Status, uint8_t Rx_Msg_DLC);
#endif
uint32_t Get_Signal_Value(uint8_t * Buffer, uint8_t Start_Index, uint8_t Bits);
//**************************************************************************************************************************
//**************************************************************************************************************************

void my_can0_init(void)
{
	//---------- Set CAN0 port pins -------------------
	// CAN0TX --> P0_4
	// CAN0RX --> P0_5
	//	PORT_AWO.PMC0.UINT16 |= 0x0030;
	//	PORT_AWO.PFCE0.UINT16 |= 0x0030;
	//	PORT_AWO.PFC0.UINT16 |= 0x0030;
	//	PORT_AWO.PM0.UINT16 &= 0xFFEF;
	//	PORT_AWO.PM0.UINT16 |= 0x0020;

	//	R_GPIO_WritePin(3,4,1);
	//	R_GPIO_WritePin(16,8,1);

	//---------- Set CAN1 port pins -------------------
	//CAN 1 PORT_TX P0_6   PORT_RX P0_7
	//	PORT_AWO.PMC0.UINT16 |= 0x00C0;
	//	PORT_AWO.PFCE0.UINT16 |= 0x00C0;
	//	PORT_AWO.PFC0.UINT16 |= 0x00C0;
	//	PORT_AWO.PM0.UINT16 &= 0xFFBF;
	//	PORT_AWO.PM0.UINT16 |= 0x0080;

	/* CAN 2 configuration   */
	// CAN0TX --> P0_8
	// CAN0RX --> P0_9

	PORT_AWO.PMC0.UINT16 |= 0x0300;

	PORT_AWO.PFCE0.UINT16 |= 0x0300;

	PORT_AWO.PFC0.UINT16 |= 0x0300;

	PORT_AWO.PM0.UINT16 &= 0xFEFF;
	PORT_AWO.PM0.UINT16 |= 0x0200;
	/* CAN wakeup and enable  */


	while((RSCAN0.GSTS.UINT8[0] && 0x08)==0x08);		//wait until CAN RAM is initiallized

	RSCAN0.GCTR.UINT8[0] &= 0xFB;	//Transition to global reset mode from global stop mode
	//GSLPR-->0 other than stop mode
	//GSLPR-->1 stop mode

	//RSCAN0.C1CTR.UINT8[0] &= 0xFB;	//Transition to channel reset mode from channel stop mode
	RSCAN0.C2CTR.UINT8[0] &= 0xFB;	//Transition to channel reset mode from channel stop mode
	//CSLPR-->0 other than stop mode
	//CSLPR-->1 stop mode

	//---------- Baud settings for 8MHz clock -------------------------------------
#if 1
	//RSCAN0.GCFG.UINT32 = 0x10;	//xin clock selected w501
	//RSCAN0.GCFG.UINT32 = 0x2010;	//xin clock selected w501
	RSCAN0.GCFG.UINT32 = 0x4010;	//xin clock selected w501

	//RSCAN0.C0CFG.UINT32 = 0x00230007;	// 125kbps, 8Tq, 1,3,4, 62.5% sampling
	//RSCAN0.C0CFG.UINT32 = 0x00140007;	// 125kbps, 8Tq, 1,2,5, 75% sampling

	//RSCAN0.C0CFG.UINT32 = 0x00230003;	// 250kbps, 8Tq, 1,3,4, 62.5% sampling
	//RSCAN0.C0CFG.UINT32 = 0x00140003;	// 250kbps, 8Tq, 1,2,5, 75% sampling CAN0
	//RSCAN0.C1CFG.UINT32 = 0x00140001;	// 500kbps, 8Tq, 1,2,5, 75% sampling CAN1
	RSCAN0.C2CFG.UINT32 = 0x00140001;	// 500kbps, 8Tq, 1,2,5, 75% sampling CAN2


	//RSCAN0.C0CFG.UINT32 = 0x00230001;	// 500kbps, 8Tq, 1,3,4, 62.5% sampling
	//RSCAN0.C0CFG.UINT32 = 0x00140001;	// 500kbps, 8Tq, 1,2,5, 75% sampling

	//RSCAN0.C0CFG.UINT32 = 0x00230000;	// 1Mbps, 8Tq, 1,3,4, 62.5% sampling
	//RSCAN0.C0CFG.UINT32 = 0x00140000;	// 1Mbps, 8Tq, 1,2,5, 75% sampling

#endif	

	//--------- Receive Rule settings for RX FIFO -------------------------------------
#if 0
	RSCAN0.GAFLCFG0.UINT8[3] = 0x01;	// No. of rules for channel 0
	RSCAN0.GAFLECTR.UINT8[1] = 0x01;	// Enable write to receive rule table
	RSCAN0.GAFLECTR.UINT8[0] = 0x00;	// receive rule page no.0 configuration

	// receive rule 1
	RSCAN0.GAFLID0.UINT16[0] = 0x7E2;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM0.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_0.UINT8[1] = 0x00;		
	RSCAN0.GAFLP1_0.UINT32 = 0x01;		// Receive FIFO 0 selected

	RSCAN0.GAFLECTR.UINT8[1] = 0x00;	// Disable write to receive rule table
#endif
	//--------- Receive Rule settings for RX FIFO -------------------------------------
#if 0
	RSCAN0.GAFLCFG0.UINT8[3] = 0x02;	// No. of rules for channel 0
	RSCAN0.GAFLECTR.UINT8[1] = 0x01;	// Enable write to receive rule table
	RSCAN0.GAFLECTR.UINT8[0] = 0x00;	// receive rule page no.0 configuration

	// receive rule 1
	RSCAN0.GAFLID0.UINT16[0] = IC_DIAG_REQ;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM0.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP00.UINT8[1] = 0x00;
	RSCAN0.GAFLP10.UINT32 = 0x01;		// Receive FIFO 0 selected

	// receive rule 2
	RSCAN0.GAFLID1.UINT16[0] = 0x7DF;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM1.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP01.UINT8[1] = 0x00;
	RSCAN0.GAFLP11.UINT32 = 0x01;		// Receive FIFO 0 selected

	RSCAN0.GAFLECTR.UINT8[1] = 0x00;	// Disable write to receive rule table
#endif
	//--------- Receive Rule settings for TX/RX FIFO -------------------------------------
#if 0
	//--------- Receive Rule settings for TX/RX FIFO -------------------------------------
	RSCAN0.GAFLCFG0.UINT8[3] = 0x04;	// No. of rules for channel 0 -> 12 rules 0x0C
	RSCAN0.GAFLECTR.UINT8[1] = 0x01;	// Enable write to receive rule table
	RSCAN0.GAFLECTR.UINT8[0] = 0x00;	// receive rule page no.configuration

	// receive rule 1
	RSCAN0.GAFLID0.UINT16[0] = 0x111;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM0.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_0.UINT8[1] = 0x00;		
	RSCAN0.GAFLP1_0.UINT32 = 0x0100;		// TX/RX FIFO 0 selected

	// receive rule 2
	RSCAN0.GAFLID1.UINT16[0] = 0x222;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM1.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_1.UINT8[1] = 0x00;		
	RSCAN0.GAFLP1_1.UINT32 = 0x0100;		// Receive FIFO 0 selected

	// receive rule 3
	RSCAN0.GAFLID2.UINT16[0] = 0x333;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM2.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_2.UINT8[1] = 0x00;		
	RSCAN0.GAFLP1_2.UINT32 = 0x0100;		// Receive FIFO 0 selected	

	// receive rule 4
	RSCAN0.GAFLID3.UINT16[0] = 0x444;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM3.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_3.UINT8[1] = 0x00;		
	RSCAN0.GAFLP1_3.UINT32 = 0x0100;		// Receive FIFO 0 selected	
	RSCAN0.GAFLECTR.UINT8[1] = 0x00;	// Disable write to receive rule table
#endif	
	//--------- Receive Buffer settings -------------------------------------
#ifdef SPRINT4
	RSCAN0.RMNB.UINT8[0] = 0x30;/*0x22*/;			// no. of receive buffers 38 max supported are 48
#else	
	RSCAN0.RMNB.UINT8[0] = 0x23;/*0x22*/;			// no. of receive buffers	34
#endif
	//--------- Receive Rule settings for RX Buffer -------------------------------------
#if 1
#ifdef SPRINT4
RSCAN0.GAFLCFG0.UINT8[1] = 0x80; /*0x22*/;		// No. of rules for channel 2	0x17
#else
	RSCAN0.GAFLCFG0.UINT8[1] = 0x23; /*0x22*/;		// No. of rules for channel 2	0x17
#endif	
	//RSCAN0.GAFLCFG0.UINT8[2] = 0x14;	// No. of rules for channel 1

	RSCAN0.GAFLECTR.UINT8[1] = 0x01;	// Enable write to receive rule table

	// Page 0 starts
	RSCAN0.GAFLECTR.UINT8[0] = 0x00;	// receive rule for page no.0 configuration

	// receive rule 1
	RSCAN0.GAFLID0.UINT16[0] = EMS36_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM0.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_0.UINT8[1] = 0x80;		// receive buffer 1 used for storing message

	// receive rule 2
	RSCAN0.GAFLID1.UINT16[0] = 0x100/*ESC2_10*/;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM1.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_1.UINT8[1] = 0x81;		// receive buffer 2 used for storing message

	// receive rule 3
	RSCAN0.GAFLID2.UINT16[0] = EMS1_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM2.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_2.UINT8[1] = 0x82;		// receive buffer 3 used for storing message

	// receive rule 4
	RSCAN0.GAFLID3.UINT16[0] = EMS3_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM3.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_3.UINT8[1] = 0x83;		// receive buffer 4 used for storing message

	// receive rule 5
	RSCAN0.GAFLID4.UINT16[0] = ESC5_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM4.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_4.UINT8[1] = 0x84;		// receive buffer 5 used for storing message

	// receive rule 6
	RSCAN0.GAFLID5.UINT16[0] = EMS8_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM5.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_5.UINT8[1] = 0x85;		// receive buffer 6 used for storing message

	// receive rule 7
	RSCAN0.GAFLID6.UINT16[0] = EMS4_20;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM6.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_6.UINT8[1] = 0x86;		// receive buffer 7 used for storing message

	// receive rule 8
	RSCAN0.GAFLID7.UINT16[0] = TCU5_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM7.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_7.UINT8[1] = 0x87;		// receive buffer 8 used for storing message

	// receive rule 9
	RSCAN0.GAFLID8.UINT16[0] = MBFM5_100;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM8.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_8.UINT8[1] = 0x88;		// receive buffer 9 used for storing message

	// receive rule 10
	RSCAN0.GAFLID9.UINT16[0] = EMS29_100;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM9.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_9.UINT8[1] = 0x89;		// receive buffer 10 used for storing message

	// receive rule 11
	RSCAN0.GAFLID10.UINT16[0] = TCU6_20;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM10.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_10.UINT8[1] = 0x8A;		// receive buffer 11 used for storing message

	// receive rule 12
	RSCAN0.GAFLID11.UINT16[0] = 0x200;/*ESC12_10;*/	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM11.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_11.UINT8[1] = 0x8B;		// receive buffer 12 used for storing message

	// receive rule 13
	RSCAN0.GAFLID12.UINT16[0] = SRS1_20;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM12.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_12.UINT8[1] = 0x8C;		// receive buffer 13 used for storing message

	// receive rule 14
	RSCAN0.GAFLID13.UINT16[0] = EMS6_500;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM13.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_13.UINT8[1] = 0x8D;		// receive buffer 14 used for storing message

	// receive rule 15
	RSCAN0.GAFLID14.UINT16[0] = EMS12_200;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM14.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_14.UINT8[1] = 0x8E;		// receive buffer 15 used for storing message

	// receive rule 16
	RSCAN0.GAFLID15.UINT16[0] = PKE_ICU2_100;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM15.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_15.UINT8[1] = 0x8F;		// receive buffer 16 used for storing message

	// Page 1 starts
	RSCAN0.GAFLECTR.UINT8[0] = 0x01;	// receive rule for page no.1 configuration

	// receive rule 17
	RSCAN0.GAFLID0.UINT16[0] = MBFM1_100;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM0.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_0.UINT8[1] = 0x90;		// receive buffer 17 used for storing message

	// receive rule 18
	RSCAN0.GAFLID1.UINT16[0] = IS3_500;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM1.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_1.UINT8[1] = 0x91;		// receive buffer 18 used for storing message

	// receive rule 19
	RSCAN0.GAFLID2.UINT16[0] = IS1_100;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM2.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_2.UINT8[1] = 0x92;		// receive buffer 19 used for storing message

	// receive rule 24		MBFM 9_500
	RSCAN0.GAFLID7.UINT16[0] = MBFM9_500;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM7.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_7.UINT8[1] = 0x93;		// receive buffer 20 used for storing message

	// receive rule 25		EMS_NSM
	RSCAN0.GAFLID8.UINT16[0] = EMS_NSM;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM8.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_8.UINT8[1] = 0x94;		// receive buffer 21 used for storing message

	// receive rule 26		ESC_NSM
	RSCAN0.GAFLID9.UINT16[0] = ESC_NSM;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM9.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_9.UINT8[1] = 0x95;		// receive buffer 22 used for storing message

	// receive rule 27		IS_NSM
	RSCAN0.GAFLID10.UINT16[0] = IS_NSM;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM10.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_10.UINT8[1] = 0x96;		// receive buffer 23 used for storing message

	// receive rule 28		PKE_ICU_NSM
	RSCAN0.GAFLID11.UINT16[0] = PKE_ICU_NSM;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM11.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_11.UINT8[1] = 0x97;		// receive buffer 24 used for storing message

	// receive rule 29		SRS_NSM
	RSCAN0.GAFLID12.UINT16[0] = SRS_NSM;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM12.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_12.UINT8[1] = 0x98;		// receive buffer 25 used for storing message

	// receive rule 30		TCU_NSM
	RSCAN0.GAFLID13.UINT16[0] = TCU_NSM;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM13.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_13.UINT8[1] = 0x99;		// receive buffer 26 used for storing message

	// receive rule 31		SYNC_MSG_SP
	RSCAN0.GAFLID14.UINT16[0] = SYNC_MSG_SP;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM14.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_14.UINT8[1] = 0x9A;		// receive buffer 27 used for storing message

	// receive rule 32		RPAS1_20
	RSCAN0.GAFLID15.UINT16[0] = RPAS1_20;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM15.UINT32 = 0xfff;				// ID bits are compared compared
	RSCAN0.GAFLP0_15.UINT8[1] = 0x9B;		// receive buffer 28 used for storing message
#if 0
	// receive rule 20 fifo
	RSCAN0.GAFLID3.UINT16[0] = 0x7E2;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM3.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_3.UINT8[1] = 0x00;
	RSCAN0.GAFLP1_3.UINT32 = 0x01;		// Receive FIFO 0 selected
#endif
	// receive rule 20
	RSCAN0.GAFLID3.UINT16[0] = 0x7E2;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM3.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_3.UINT8[1] = 0x00;
	RSCAN0.GAFLP1_3.UINT32 = 0x0100;		// TX/RX FIFO 0 selected

	//	RSCAN0.GAFLECTR.UINT8[1] = 0x00;	// Disable write to receive rule table

	// receive rule 21
	RSCAN0.GAFLID4.UINT16[0] = 0x7DF;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM4.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_4.UINT8[1] = 0x00;
	RSCAN0.GAFLP1_4.UINT32 = 0x0100;		// TX/RX FIFO 0 selected
	// receive rule 22
	RSCAN0.GAFLID5.UINT16[0] = ESC2_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM5.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_5.UINT8[1] = 0x00;
	RSCAN0.GAFLP1_5.UINT32 = 0x0100;		// TX/RX FIFO 0 selected

	// receive rule 23
	RSCAN0.GAFLID6.UINT16[0] = ESC12_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM6.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_6.UINT8[1] = 0x00;
	RSCAN0.GAFLP1_6.UINT32 = 0x0100;		// TX/RX FIFO 0 selected

	/*Page 2 Start*/
	RSCAN0.GAFLECTR.UINT8[0] = 0x02;	// receive rule for page no.2 configuration

	// receive rule 33		RPAS2_100
	RSCAN0.GAFLID0.UINT16[0] = RPAS2_100;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM0.UINT32 = 0xfff;				// ID bits are compared compared
	RSCAN0.GAFLP0_0.UINT8[1] = 0x9C;		// receive buffer 28 used for storing message
#ifdef SPRINT4
	// receive rule 34
	RSCAN0.GAFLID1.UINT16[0] = IS16_1000;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM1.UINT32 = 0xff;		// ID bits are compared compared
	RSCAN0.GAFLP0_1.UINT8[1] = 0x9D;  // receive buffer 29 used for storing message

	// receive rule 35		MBFM_PAS1_50
	RSCAN0.GAFLID2.UINT16[0] = MBFM_PAS1_50;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM2.UINT32 = 0xfff;				// ID bits are compared compared
	RSCAN0.GAFLP0_2.UINT8[1] = 0x9E;			// receive buffer 30 used for storing message

	// receive rule 36
	RSCAN0.GAFLID3.UINT16[0] = ESC7_20;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM3.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_3.UINT8[1] = 0x9F;  // receive buffer 31 used for storing message

	// receive rule 37		EMS5_500
	RSCAN0.GAFLID4.UINT16[0] = EMS5_500;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM4.UINT32 = 0xfff;				// ID bits are compared compared
	RSCAN0.GAFLP0_4.UINT8[1] = 0x00;	//0x9D		// receive buffer 28 used for storing message
	RSCAN0.GAFLP1_4.UINT32 = 0x0100;		// TX/RX FIFO 0 selected
    
	// receive rule 38
	RSCAN0.GAFLID5.UINT16[0] = SAS1_10;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM5.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_5.UINT8[1] = 0xA0;  // receive buffer 32 used for storing message

	// receive rule 39   IS4_500
	RSCAN0.GAFLID6.UINT16[0] = IS4_500;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM6.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_6.UINT8[1] = 0xA1;  // receive buffer 33 used for storing message

	// receive rule 40   IS14_200
	RSCAN0.GAFLID7.UINT16[0] = IS14_200;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM7.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_7.UINT8[1] = 0xA2;  // receive buffer 33 used for storing message

	// receive rule 41   IS15_200
	RSCAN0.GAFLID8.UINT16[0] = IS15_200;	// Standard, Data frame, 11 bit ID
	//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
	RSCAN0.GAFLM8.UINT32 = 0xfff;		// ID bits are compared compared
	RSCAN0.GAFLP0_8.UINT8[1] = 0xA3;  // receive buffer 33 used for storing message
#else
	// receive rule 34		EMS5_500
	RSCAN0.GAFLID1.UINT16[0] = EMS5_500;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM1.UINT32 = 0xfff;				// ID bits are compared compared
	RSCAN0.GAFLP0_1.UINT8[1] = 0x00;	//0x9D		// receive buffer 28 used for storing message
	RSCAN0.GAFLP1_1.UINT32 = 0x0100;		// TX/RX FIFO 0 selected

	// receive rule 35		MBFM_PAS1_50
	RSCAN0.GAFLID2.UINT16[0] = MBFM_PAS1_50;	// Standard, Data frame, 11 bit ID
	RSCAN0.GAFLM2.UINT32 = 0xfff;				// ID bits are compared compared
	RSCAN0.GAFLP0_2.UINT8[1] = 0x9E;			// receive buffer 28 used for storing message
#endif
	RSCAN0.GAFLECTR.UINT8[1] = 0x00;	// Disable write to receive rule table
#endif

	//--------- Receive Buffer settings -------------------------------------
	//	RSCAN0.RMNB.UINT8[0] = 0x0C;		// no. of receive buffers

#if 1
	/* Configure the CANout buffer  */
	RSCAN0.TMIEC1.UINT32 = 0x00000000;		// Transmit Buffer 0 interrupt dissable for 0 to 31 transmit buffer

	/* TX buffer 01 */
	//	RSCAN0.TMID0.UINT16[0] = 0x01C0 ;		// TX Transmit message, standard data frame,
	//	RSCAN0.TMPTR0.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer can1 16 */
	RSCAN0.TMID16.UINT16[0] = 0x01C0 ;		// TX Transmit message, standard data frame,
	RSCAN0.TMPTR16.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer 02 */
	//	RSCAN0.TMID1.UINT16[0] = 0x02C0 ;		// TX Transmit message, standard data frame,
	//	RSCAN0.TMPTR1.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer can1 17 */
	RSCAN0.TMID17.UINT16[0] = 0x02C0 ;		// TX Transmit message, standard data frame,
	RSCAN0.TMPTR17.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer 03 */
	//	RSCAN0.TMID2.UINT16[0] = 0x03C0 ;		// TX Transmit message, standard data frame,
	//	RSCAN0.TMPTR2.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer can1 18 */
	RSCAN0.TMID18.UINT16[0] = 0x03C0 ;		// TX Transmit message, standard data frame,
	RSCAN0.TMPTR18.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer 04 */
	//	RSCAN0.TMID04.UINT16[0] = 0x01C1 ;		// TX Transmit message, standard data frame,
	//	RSCAN0.TMPTR04.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer can1 19 */
	RSCAN0.TMID19.UINT16[0] = 0x01C1 ;		// TX Transmit message, standard data frame,
	RSCAN0.TMPTR19.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer 05 */
	//	RSCAN0.TMID4.UINT16[0] = 0x02C1 ;		// TX Transmit message, standard data frame,
	//	RSCAN0.TMPTR4.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer can1 20 */
	RSCAN0.TMID20.UINT16[0] = 0x02C1 ;		// TX Transmit message, standard data frame,
	RSCAN0.TMPTR20.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer 06 */
	//	RSCAN0.TMID5.UINT16[0] = 0x03C1 ;		// TX Transmit message, standard data frame,
	//	RSCAN0.TMPTR5.UINT16[1] =0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

	/* TX buffer can1 21 */
	RSCAN0.TMID21.UINT16[0] = 0x03C1 ;		// TX Transmit message, standard data frame,
	RSCAN0.TMPTR21.UINT16[1] =0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;
#endif
	//-----------Transmit/receive FIFO settings in Receive mode -----------------
#if 1	
	RSCAN0.CFCC0.UINT8[2] = 0x00;		// Receive mode selected
	RSCAN0.CFCC0.UINT8[1] = 0x13;		// interrupt on every message transmitted, FIFO depth 16 messages
	//RSCAN0.CFCC0.UINT8[1] = 0x03;		// Interrupt on 1/8 message received in FIFO, Receive FIFO has 16 message depth
	RSCAN0.CFCC0.UINT8[0] = 0x03;		// transmit/receive FIFO receive interupt enabled
#endif
	//--------- Receive FIFO Buffer settings -------------------------------------
#if 1
	RSCAN0.RFCC0.UINT8[1] = 0x13;		// Interrupt on every message, Receive FIFO has 16 message depth
	//RSCAN0.RFCC0.UINT8[1] = 0x03;		// Interrupt on 1/8 message received in FIFO, Receive FIFO has 16 message depth
	RSCAN0.RFCC0.UINT8[0] = 0x03;		// Receive FIFO buffers used and interrupt enabled
#endif
	//----------- Global Interrupt -------------------------------------
	//RSCAN0.GCTR.UINT8[1] = 0x07;		// Transmit history interrupt, FIFO msg lost interrupt & DLC error interrupt enabled
	//RSCAN0.C0CTR.UINT16[1] = 0x0001;	// Trasmit abort interrupt enabled
	//RSCAN0.C0CTR.UINT8[1] = 0x01;		// Bus error interrupt enabled

	//------------ Interrupt setting -----------------------------------
	INTC2EITB69 = 1;//0;		// 0-Direct jump method, 1-vector method
	INTC2EIP69 = 1;				// level 1 priority

	INTC2EIRF69 = 0;		// clear interrupt flag
	INTC2EIMK69 = 0;		// enable error interrupt

	INTC2EITB70 = 1;//0;		// 0-Direct jump method, 1-vector method
	INTC2EIP70 = 1;				// level 1 priority

	INTC2EIRF70 = 0;		// clear interrupt flag
	INTC2EIMK70 = 0;		// enable error interrupt

	INTC2EITB71 = 1;//0;		// 0-Direct jump method, 1-vector method
	INTC2EIP71 = 1;				// level 1 priority

	INTC2EIRF71 = 0;		// clear interrupt flag
	INTC2EIMK71 = 0;		// enable error interrupt

	INTC2EITB72 = 1;//0;		// 0-Direct jump method, 1-vector method
	INTC2EIP72 = 1;				// level 1 priority

	INTC2EIRF72 = 0;		// clear interrupt flag
	INTC2EIMK72 = 0;		// enable error interrupt

	INTC2EITB73 = 1;//0;		// 0-Direct jump method, 1-vector method
	INTC2EIP73 = 1;				// level 1 priority

	INTC2EIRF73 = 0;		// clear interrupt flag	
	INTC2EIMK73 = 1;		// enable error interrupt, 1 = dissable 0 = enable


	//------------ Operating mode --------------------------------------

	RSCAN0.GCTR.UINT8[0] = 0x00;		// Global operating mode
	for(i=0;i<0xfff;i++);	//wait for transistion

	RSCAN0.RFCC0.UINT8[0] |= 0x01;		// receive FIFO is used

	//RSCAN0.C0CTR.UINT8[0] = 0x00;	//channel communication mode w501
	//RSCAN0.C1CTR.UINT8[0] = 0x00;	//channel communication mode w501
	RSCAN0.C2CTR.UINT8[0] = 0x00;	//channel communication mode w501
	for(i=0;i<0xfff;i++);	//wait for transistion

	RSCAN0.CFCC0.UINT8[0] |= 0x01;		// transmit/receive FIFO is used

	CAN_init_done = SET;
}

int my_can0_rx(void)
{
	uint8_t rx_data_cnt=0;
	uint8_t rx_data[8] = { 0 };
	uint32_t SignalValue = 0;

	//-------------------- Rx by reading Message buffer-------------------------------------------------------
#if 0
	if((RSCAN0.RMND0.UINT8[0] & 0x10) == 0x10)	// check if RMNS bit of RMND register is set to '1'
	{
		RSCAN0.RMND0.UINT8[0] &= 0xEF;		// clear RMNS bit

		rx_msg_ID = RSCAN0.RMID4.UINT16[0];		// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR4.UINT8[3] & 0xF0) >> 4;	// read received message data length

		rx_msg_data0 = RSCAN0.RMDF04.UINT32;
		rx_msg_data1 = RSCAN0.RMDF14.UINT32;
	}
#endif
	//-------------------- Rx by reading Message buffer-------------------------------------------------------
#if 1
	/* EMS36_10 0x142 */
	if(((RSCAN0.RMND0.UINT8[0] & 0x01) == 0x01) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '1'
	{
		EMS36_10_message_ON = 1;
		//EMS36_10_message_timeout_flag = 0;

		RSCAN0.RMND0.UINT8[0] &= 0xFE;						// clear RMNS bit
		MsgBuf_address=(MSGBUF_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID0.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR0.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS36_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		    rx_data[rx_data_cnt] = CAN_EMS36_10[rx_data_cnt].byte;
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS36_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS36_10_Message_Content, rx_msg_DLC);

		    if(EMS36_10_Message_Content.Failure_Status == CLEAR)
		    {
			SignalValue = Get_Signal_Value(&rx_data, 24, 12);
			Signal_Content_Failure_Verication(&EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content, SignalValue, 0, 2046, 2047);
		    }
		    else
		    {
			EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Failure_Counter = 0;
			EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(EMS36_10_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			if(EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS36_10[3].byte = rx_data[3];
 - 			    CAN_EMS36_10[4].byte |= (rx_data[4] & 0x0F);
 - 			}
 - 
 - 			CAN_EMS36_10[0].byte = rx_data[0];
 - 			CAN_EMS36_10[1].byte = rx_data[1];
 - 			CAN_EMS36_10[2].byte = rx_data[2];
 - 			CAN_EMS36_10[4].byte |= (rx_data[4] & 0xF0);
 - 			CAN_EMS36_10[5].byte = rx_data[5];
 - 			CAN_EMS36_10[6].byte = rx_data[6];
 - 			CAN_EMS36_10[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    EMS36_10_Message_Content.Failure_Counter = 0;
		    EMS36_10_Message_Content.Recovery_Counter = 0;
		    EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Failure_Counter = 0;
		    EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS36_10_message_ON = 0;
	}
#ifdef SPRINT4
	// SAS1_10
	
		//if(((RSCAN0.RMND1.UINT8[0] & 0x01) == 0x01) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))        /* For TDMS testing only enbling EMS variant */
		if(((RSCAN0.RMND1.UINT8[0] & 0x01) == 0x01) && (U8g_Block_SAS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '1'
		{
			RSCAN0.RMND1.UINT8[0] &= 0xFE;						// clear RMNS bit
			MsgBuf_address=(MSGBUF32_BASE_ADD);

			SAS1_10_message_ON = 1;

			rx_msg_ID = RSCAN0.RMID32.UINT16[0];					// read received message ID
			rx_msg_DLC = (RSCAN0.RMPTR32.UINT8[3] & 0xF0) >> 4;	// read received message data length

			for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
			{
				CAN_SAS1_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
				rx_data[rx_data_cnt] = CAN_SAS1_10[rx_data_cnt].byte;
			}
			if((SAS1_10_Message.Message_Timeout_Status == CLEAR) && (LVSD_CAN == 0) && (HVSD_CAN == 0))
			{
				Message_Content_Failure_Verication_SAS(&SAS1_10_Message_Content, rx_msg_DLC);

				if(SAS1_10_Message_Content.Failure_Status == CLEAR)
				{
					SignalValue = Get_Signal_Value(&rx_data, 0, 16);
					Signal_Content_Failure_Verication(&SAS1_10_ABOLUTE_ANGLE_Sig_Content,SignalValue, 0, 32766, 32767);
				}
				else
				{
					SAS1_10_ABOLUTE_ANGLE_Sig_Content.Failure_Counter = 0;
					SAS1_10_ABOLUTE_ANGLE_Sig_Content.Recovery_Counter = 0;
				}
				if(SAS1_10_Message_Content.Failure_Status == CLEAR)
				{
					SignalValue = Get_Signal_Value(&rx_data, 24, 1);
					Signal_Content_Failure_Verication(&SAS1_10_STS_SAS_FAILURE_Sig_Content,SignalValue, 0, 1, 2);
				}
				else
				{
					SAS1_10_STS_SAS_FAILURE_Sig_Content.Failure_Counter = 0;
					SAS1_10_STS_SAS_FAILURE_Sig_Content.Recovery_Counter = 0;
				}
				if(SAS1_10_Message_Content.Failure_Status == CLEAR)
				{
					SignalValue = Get_Signal_Value(&rx_data, 25, 1);
					Signal_Content_Failure_Verication(&SAS1_10_STS_SAS_CALIB_Sig_Content,SignalValue, 0, 1, 2);
				}
				else
				{
					SAS1_10_STS_SAS_CALIB_Sig_Content.Failure_Counter = 0;
					SAS1_10_STS_SAS_CALIB_Sig_Content.Recovery_Counter = 0;
				}
				if(SAS1_10_Message_Content.Failure_Status == CLEAR)
				{
					SignalValue = Get_Signal_Value(&rx_data, 26, 1);
					Signal_Content_Failure_Verication(&SAS1_10_STS_SAS_TRIM_Sig_Content,SignalValue, 0, 1, 2);
				}
				else
				{
					SAS1_10_STS_SAS_TRIM_Sig_Content.Failure_Counter = 0;
					SAS1_10_STS_SAS_TRIM_Sig_Content.Recovery_Counter = 0;
				}
				if(SAS1_10_Message_Content.Failure_Status == CLEAR)
				{
					SignalValue = Get_Signal_Value(&rx_data, 32, 4);
					Signal_Content_Failure_Verication(&SAS1_10_SAS_MSG_CNT_Sig_Content,SignalValue, 0, 15, 16);
				}
				else
				{
					SAS1_10_SAS_MSG_CNT_Sig_Content.Failure_Counter = 0;
					SAS1_10_SAS_MSG_CNT_Sig_Content.Recovery_Counter = 0;
				}

				if(SAS1_10_Message_Content.Failure_Status == CLEAR)
				{
					SignalValue = Get_Signal_Value(&rx_data, 36, 4);
					Signal_Content_Failure_Verication(&SAS1_10_SAS_CRC_Sig_Content,SignalValue, 0, 15, 16);
				}
				else
				{
					SAS1_10_SAS_CRC_Sig_Content.Failure_Counter = 0;
					SAS1_10_SAS_CRC_Sig_Content.Recovery_Counter = 0;
				}
		    }
			else
			{
				SAS1_10_Message_Content.Failure_Counter = 0;
				SAS1_10_Message_Content.Recovery_Counter = 0;
			}
			
		}
		else
		{
			SAS1_10_message_ON = 0;
		}
#endif
#if 0
	/* ESC2_10 0x2CC */
	if((RSCAN0.RMND0.UINT8[0] & 0x02) == 0x02)				// check if RMNS bit of RMND register is set to '2'
	{
		ESC2_10_message_ON = 1;
		//ESC2_10_message_timeout_flag   = 0;

		RSCAN0.RMND0.UINT8[0] &= 0xFD;						// clear RMNS bit
		MsgBuf_address=(MSGBUF1_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID1.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR1.UINT8[3] & 0xF0) >> 4;	// read received message data length

		rx_msg_data0 = RSCAN0.RMDF0_1.UINT32;
		rx_msg_data1 = RSCAN0.RMDF1_1.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_ESC2_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}
	}
	else
	{
		ESC2_10_message_ON = 0;
		//ESC2_10_Message_Period_Counter = 0;
	}
#endif
	/* EMS1_10 0x124 */
	if(((RSCAN0.RMND0.UINT8[0] & 0x04) == 0x04) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '3'
	{
		EMS1_10_message_ON = 1;
		//EMS1_10_message_timeout_flag = 0;

		RSCAN0.RMND0.UINT8[0] &= 0xFB;						// clear RMNS bit
		MsgBuf_address=(MSGBUF2_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID2.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR2.UINT8[3] & 0xF0) >> 4;	// read received message data length

		//rx_msg_data0 = RSCAN0.RMDF0_2.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_2.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS1_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		    rx_data[rx_data_cnt] = CAN_EMS1_10[rx_data_cnt].byte;
		}

		
		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS1_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS1_10_Message_Content, rx_msg_DLC);

		    if(EMS1_10_Message_Content.Failure_Status == CLEAR)
		    {
			SignalValue = Get_Signal_Value(&rx_data, 56, 8);
			Signal_Content_Failure_Verication(&EMS1_10_Temp_Sig_Content, SignalValue, 0, 254, 255);
		    
			SignalValue = Get_Signal_Value(&rx_data, 8, 16);
			Signal_Content_Failure_Verication(&EMS1_10_RPM_Sig_Content, SignalValue, 0, 7999, 65535);
		    
			if(FEATURE_CODE_CURRENT_TARGET_SEL == SET)
			{
			    SignalValue = Get_Signal_Value(&rx_data, 40, 4);
			    Signal_Content_Failure_Verication(&EMS1_10_GEAR_Sig_Content, SignalValue, 0, 14, 15);
			}
		    
			SignalValue = Get_Signal_Value(&rx_data, 38, 2);
			Signal_Content_Failure_Verication(&EMS1_10_INDC_CRUISE_Sig_Content, SignalValue, 0, 2, 3);
		    }
		    else
		    {
			EMS1_10_Temp_Sig_Content.Failure_Counter = 0;
			EMS1_10_Temp_Sig_Content.Recovery_Counter = 0;
			EMS1_10_RPM_Sig_Content.Failure_Counter = 0;
			EMS1_10_RPM_Sig_Content.Recovery_Counter = 0;
			EMS1_10_GEAR_Sig_Content.Failure_Counter = 0;
			EMS1_10_GEAR_Sig_Content.Recovery_Counter = 0;
			EMS1_10_INDC_CRUISE_Sig_Content.Failure_Counter = 0;
			EMS1_10_INDC_CRUISE_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(EMS1_10_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			if(EMS1_10_Temp_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS1_10[7].byte = rx_data[7];
 - 			}
 - 			
 - 			if(EMS1_10_RPM_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS1_10[1].byte = rx_data[1];
 - 				CAN_EMS1_10[2].byte = rx_data[2];
 - 			}
 - 			
 - 			if(EMS1_10_GEAR_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS1_10[5].byte |= (rx_data[5] & 0x0F);
 - 			}
 - 			
 - 			if(EMS1_10_Temp_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS1_10[4].byte |= (rx_data[7] & 0xC0);
 - 			}
 - 
 - 			CAN_EMS1_10[0].byte = rx_data[0];
 - 			CAN_EMS1_10[3].byte = rx_data[3];
 - 			CAN_EMS1_10[4].byte |= (rx_data[4] & 0x3F);
 - 			CAN_EMS1_10[5].byte |= (rx_data[5] & 0xF0);
 - 			CAN_EMS1_10[6].byte = rx_data[6];
 - 		    }*/
		}
		else
		{
		    EMS1_10_Message_Content.Failure_Counter = 0;
		    EMS1_10_Message_Content.Recovery_Counter = 0;
		    EMS1_10_Temp_Sig_Content.Failure_Counter = 0;
		    EMS1_10_Temp_Sig_Content.Recovery_Counter = 0;
		    EMS1_10_RPM_Sig_Content.Failure_Counter = 0;
		    EMS1_10_RPM_Sig_Content.Recovery_Counter = 0;
		    EMS1_10_GEAR_Sig_Content.Failure_Counter = 0;
		    EMS1_10_GEAR_Sig_Content.Recovery_Counter = 0;
		    EMS1_10_INDC_CRUISE_Sig_Content.Failure_Counter = 0;
		    EMS1_10_INDC_CRUISE_Sig_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS1_10_message_ON = 0;
	}

	/* EMS3_10 0x108 */
	if(((RSCAN0.RMND0.UINT8[0] & 0x08) == 0x08) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '4'
	{
		EMS3_10_message_ON = 1;
		//EMS3_10_message_timeout_flag = 0;

		RSCAN0.RMND0.UINT8[0] &= 0xF7;						// clear RMNS bit
		MsgBuf_address=(MSGBUF3_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID3.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR3.UINT8[3] & 0xF0) >> 4;	// read received message data length
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS3_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		    rx_data[rx_data_cnt] = CAN_EMS3_10[rx_data_cnt].byte;
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS3_10_Message_Content, rx_msg_DLC);

		    if(EMS3_10_Message_Content.Failure_Status == CLEAR)
		    {
			SignalValue = Get_Signal_Value(rx_data, 29, 3);
			Signal_Content_Failure_Verication(&EMS3_10_CLUCH_Sig_Content, SignalValue, 0, 6, 7);

			SignalValue = Get_Signal_Value(rx_data, 56, 3);
			Signal_Content_Failure_Verication(&EMS3_10_STS_ESS_INDC_Sig_Content, SignalValue, 0, 6, 7);
		    }
		    else
		    {
			EMS3_10_CLUCH_Sig_Content.Failure_Counter = 0;
			EMS3_10_CLUCH_Sig_Content.Recovery_Counter = 0;
			EMS3_10_STS_ESS_INDC_Sig_Content.Failure_Counter = 0;
			EMS3_10_STS_ESS_INDC_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(EMS3_10_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			if(EMS3_10_CLUCH_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS3_10[3].byte |= (rx_data[3] & 0xE0);
 - 			}
 - 
 - 			if(EMS3_10_STS_ESS_INDC_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS3_10[7].byte |= (rx_data[7] & 0x07);
 - 			}
 - 
 - 			CAN_EMS3_10[0].byte = rx_data[0];
 - 			CAN_EMS3_10[1].byte = rx_data[1];
 - 			CAN_EMS3_10[2].byte = rx_data[2];
 - 			CAN_EMS3_10[3].byte |= (rx_data[3] & 0x1F);
 - 			CAN_EMS3_10[4].byte = rx_data[4];
 - 			CAN_EMS3_10[5].byte = rx_data[5];
 - 			CAN_EMS3_10[6].byte = rx_data[6];
 - 			CAN_EMS3_10[7].byte |= (rx_data[7] & 0xF1);
 - 		    }*/
		}
		else
		{
		    EMS3_10_CLUCH_Sig_Content.Failure_Counter = 0;
		    EMS3_10_CLUCH_Sig_Content.Recovery_Counter = 0;
		    EMS3_10_STS_ESS_INDC_Sig_Content.Failure_Counter = 0;
		    EMS3_10_STS_ESS_INDC_Sig_Content.Recovery_Counter = 0;
		    EMS3_10_Message_Content.Failure_Counter = 0;
		    EMS3_10_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS3_10_message_ON = 0;
	}

	/* ESC5_10 0x10D */
	if(((RSCAN0.RMND0.UINT8[0] & 0x10) == 0x10) && ((U8g_Block_ESC_ECU_CAN_MSG == CLEAR)  || (U8g_Block_ABS_ECU_CAN_MSG == CLEAR)))				// check if RMNS bit of RMND register is set to '5'
	{
		ESC5_10_message_ON = 1;
		//ESC5_10_message_timeout_flag = 0;

		RSCAN0.RMND0.UINT8[0] &= 0xEF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF4_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID4.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR4.UINT8[3] & 0xF0) >> 4;	// read received message data length

		//rx_msg_data0 = RSCAN0.RMDF0_4.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_4.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_ESC5_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_ESC5_10[rx_data_cnt].byte;	/* Store Data */
		}

		if((ESC5_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&ESC5_10_Message_Content, rx_msg_DLC);

/*		    if(ESC5_10_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			CAN_ESC5_10[0].byte = rx_data[0];
 - 			CAN_ESC5_10[1].byte = rx_data[1];
 - 			CAN_ESC5_10[2].byte = rx_data[2];
 - 			CAN_ESC5_10[3].byte = rx_data[3];
 - 			CAN_ESC5_10[4].byte = rx_data[4];
 - 			CAN_ESC5_10[5].byte = rx_data[5];
 - 			CAN_ESC5_10[6].byte = rx_data[6];
 - 			CAN_ESC5_10[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    ESC5_10_Message_Content.Failure_Counter = 0;
		    ESC5_10_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		ESC5_10_message_ON = 0;
	}

	/* EMS8_10 0x126 */
	if(((RSCAN0.RMND0.UINT8[0] & 0x20) == 0x20) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '6'
	{
		EMS8_10_message_ON = 1;
		//EMS8_10_message_timeout_flag = 0;

		RSCAN0.RMND0.UINT8[0] &= 0xDF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF5_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID5.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR5.UINT8[3] & 0xF0) >> 4;	// read received message data length

		//rx_msg_data0 = RSCAN0.RMDF0_5.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_5.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS8_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_EMS8_10[rx_data_cnt].byte;	/* Store Data */
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS8_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS8_10_Message_Content, rx_msg_DLC);

		    if((EMS8_10_Message_Content.Failure_Status == CLEAR) && (FEATURE_CODE_CURRENT_TARGET_SEL == CLEAR))
		    {
			SignalValue = Get_Signal_Value(rx_data, 27, 4);
			Signal_Content_Failure_Verication(&EMS8_10_TARGET_GEAR_Sig_Content, SignalValue, 0, 14, 15);
		    }
		    else
		    {
			EMS8_10_TARGET_GEAR_Sig_Content.Failure_Counter = 0;
			EMS8_10_TARGET_GEAR_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(EMS8_10_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			if(EMS8_10_TARGET_GEAR_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS8_10[3].byte |= (rx_data[3] & 0x71);
 - 			}
 - 
 - 			CAN_EMS8_10[0].byte = rx_data[0];
 - 			CAN_EMS8_10[1].byte = rx_data[1];
 - 			CAN_EMS8_10[2].byte = rx_data[2];
 - 			CAN_EMS8_10[3].byte |= (rx_data[3] & 0x87);
 - 			CAN_EMS8_10[4].byte = rx_data[4];
 - 			CAN_EMS8_10[5].byte = rx_data[5];
 - 			CAN_EMS8_10[6].byte = rx_data[6];
 - 			CAN_EMS8_10[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    EMS8_10_TARGET_GEAR_Sig_Content.Failure_Counter = 0;
		    EMS8_10_TARGET_GEAR_Sig_Content.Recovery_Counter = 0;
		    EMS8_10_Message_Content.Failure_Counter = 0;
		    EMS8_10_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS8_10_message_ON = 0;
	}

	/* EMS4_20 0x130 */
	if(((RSCAN0.RMND0.UINT8[0] & 0x40) == 0x40) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '7'
	{
		EMS4_20_message_ON = 1;

		RSCAN0.RMND0.UINT8[0] &= 0xBF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF6_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID6.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR6.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS4_20[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_EMS4_20[rx_data_cnt].byte;	/* Store Data */
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS4_20_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS4_20_Message_Content, rx_msg_DLC);

		    if(EMS4_20_Message_Content.Failure_Status == CLEAR)
		    {
			SignalValue = Get_Signal_Value(rx_data, 53, 3);
			Signal_Content_Failure_Verication(&EMS4_20_ENG_DRIVE_MODE_Sig_Content, SignalValue, 0, 6, 7);
		    }
		    else
		    {
			EMS4_20_ENG_DRIVE_MODE_Sig_Content.Failure_Counter = 0;
			EMS4_20_ENG_DRIVE_MODE_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(EMS4_20_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        if(EMS4_20_ENG_DRIVE_MODE_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			   CAN_EMS4_20[6].byte |= (rx_data[6] & 0xE0);
 - 			}
 - 
 - 			CAN_EMS4_20[0].byte = rx_data[0];
 - 			CAN_EMS4_20[1].byte = rx_data[1];
 - 			CAN_EMS4_20[2].byte = rx_data[2];
 - 			CAN_EMS4_20[3].byte = rx_data[3];
 - 			CAN_EMS4_20[4].byte = rx_data[4];
 - 			CAN_EMS4_20[5].byte = rx_data[5];
 - 			CAN_EMS4_20[6].byte |= (rx_data[6] & 0x1F);
 - 			CAN_EMS4_20[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    EMS4_20_ENG_DRIVE_MODE_Sig_Content.Failure_Counter = 0;
		    EMS4_20_ENG_DRIVE_MODE_Sig_Content.Recovery_Counter = 0;
		    EMS4_20_Message_Content.Failure_Counter = 0;
		    EMS4_20_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS4_20_message_ON = 0;
	}

	/* TCU5_10 0x136 */
	if(((RSCAN0.RMND0.UINT8[0] & 0x80) == 0x80) && (U8g_Block_TCU_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '8'
	{
		TCU5_10_message_ON = 1;

		RSCAN0.RMND0.UINT8[0] &= 0x7F;						// clear RMNS bit
		MsgBuf_address=(MSGBUF7_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID7.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR7.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_TCU5_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_TCU5_10[rx_data_cnt].byte;	/* Store Data */
		}

		if((TCU5_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&TCU5_10_Message_Content, rx_msg_DLC);

		    if(TCU5_10_Message_Content.Failure_Status == CLEAR)
		    {
				if(FEATURE_CODE_CURRENT_TARGET_SEL == SET)
				{
		            SignalValue = Get_Signal_Value(rx_data, 48, 4);
				    Signal_Content_Failure_Verication(&TCU5_10_GEAR_ACTUAL_Sig_Content, SignalValue, 0, 14, 15);
				}
				else
				{
			        SignalValue = Get_Signal_Value(rx_data, 52, 4);
			        Signal_Content_Failure_Verication(&TCU5_10_GEAR_TARGET_Sig_Content, SignalValue, 0, 14, 15);
				}
			}
		    else
		    {
		        TCU5_10_GEAR_ACTUAL_Sig_Content.Failure_Counter = 0;
			TCU5_10_GEAR_ACTUAL_Sig_Content.Recovery_Counter = 0;
			TCU5_10_GEAR_TARGET_Sig_Content.Failure_Counter = 0;
			TCU5_10_GEAR_TARGET_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(TCU5_10_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        if(TCU5_10_GEAR_ACTUAL_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_TCU5_10[6].byte |= (rx_data[6] & 0x0F);
 - 			}
 - 
 - 			if(TCU5_10_GEAR_TARGET_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_TCU5_10[6].byte |= (rx_data[6] & 0xF0);
 - 			}
 - 
 - 			CAN_TCU5_10[0].byte = rx_data[0];
 - 			CAN_TCU5_10[1].byte = rx_data[1];
 - 			CAN_TCU5_10[2].byte = rx_data[2];
 - 			CAN_TCU5_10[3].byte = rx_data[3];
 - 			CAN_TCU5_10[4].byte = rx_data[4];
 - 			CAN_TCU5_10[5].byte = rx_data[5];
 - 			CAN_TCU5_10[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    TCU5_10_GEAR_ACTUAL_Sig_Content.Failure_Counter = 0;
		    TCU5_10_GEAR_ACTUAL_Sig_Content.Recovery_Counter = 0;
		    TCU5_10_GEAR_TARGET_Sig_Content.Failure_Counter = 0;
		    TCU5_10_GEAR_TARGET_Sig_Content.Recovery_Counter = 0;
		    TCU5_10_Message_Content.Failure_Counter = 0;
		    TCU5_10_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		TCU5_10_message_ON = 0;
	}

	/* MBFM5_100 0x214 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x01) == 0x01) && (U8g_Block_BCM_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '1'
	{
		MBFM5_100_message_ON = 1;

		RSCAN0.RMND0.UINT8[1] &= 0xFE;						// clear RMNS bit
		MsgBuf_address=(MSGBUF8_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID8.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR8.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_MBFM5_100[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_MBFM5_100[rx_data_cnt].byte;	/* Store Data */
		}
#ifndef U350
		if((MBFM1_100_Message.Message_Timeout_Status == CLEAR) && (MBFM5_100_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&MBFM5_100_Message_Content, rx_msg_DLC);

/*		    if(MBFM5_100_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_MBFM5_100[0].byte = rx_data[0];
 - 			CAN_MBFM5_100[1].byte = rx_data[1];
 - 			CAN_MBFM5_100[2].byte = rx_data[2];
 - 			CAN_MBFM5_100[3].byte = rx_data[3];
 - 			CAN_MBFM5_100[4].byte = rx_data[4];
 - 			CAN_MBFM5_100[5].byte = rx_data[5];
 - 			CAN_MBFM5_100[6].byte = rx_data[6];
 - 			CAN_MBFM5_100[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    MBFM5_100_Message_Content.Failure_Counter = 0;
		    MBFM5_100_Message_Content.Recovery_Counter = 0;
		}
#endif
	}
	else
	{
		MBFM5_100_message_ON = 0;
	}

	/* EMS29_100 0x233 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x02) == 0x02) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '2'
	{
		EMS29_100_message_ON = 1;

		RSCAN0.RMND0.UINT8[1] &= 0xFD;						// clear RMNS bit
		MsgBuf_address=(MSGBUF9_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID9.UINT16[0];					// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR9.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS29_100[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_EMS29_100[rx_data_cnt].byte;	/* Store Data */
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS29_100_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS29_100_Message_Content, rx_msg_DLC);

/*		    if(EMS29_100_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_EMS29_100[0].byte = rx_data[0];
 - 			CAN_EMS29_100[1].byte = rx_data[1];
 - 			CAN_EMS29_100[2].byte = rx_data[2];
 - 			CAN_EMS29_100[3].byte = rx_data[3];
 - 			CAN_EMS29_100[4].byte = rx_data[4];
 - 			CAN_EMS29_100[5].byte = rx_data[5];
 - 			CAN_EMS29_100[6].byte = rx_data[6];
 - 			CAN_EMS29_100[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    EMS29_100_Message_Content.Failure_Counter = 0;
		    EMS29_100_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS29_100_message_ON = 0;
	}

	/* TCU6_20 0x278 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x04) == 0x04) && (U8g_Block_TCU_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '3'
	{
		TCU6_20_message_ON = 1;

		RSCAN0.RMND0.UINT8[1] &= 0xFB;						// clear RMNS bit
		MsgBuf_address=(MSGBUF10_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID10.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR10.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_TCU6_20[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_TCU6_20[rx_data_cnt].byte;	/* Store Data */
		}

		if((TCU5_10_Message.Message_Timeout_Status == CLEAR) && (TCU6_20_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&TCU6_20_Message_Content, rx_msg_DLC);

		    if(TCU6_20_Message_Content.Failure_Status == CLEAR)
		    {
		        SignalValue = Get_Signal_Value(rx_data, 16, 4);
			Signal_Content_Failure_Verication(&TCU6_20_TGS_LEVER_Sig_Content, SignalValue, 0, 12, 15);

			SignalValue = Get_Signal_Value(rx_data, 20, 2);
			Signal_Content_Failure_Verication(&TCU6_20_MODE_SIGNAL_Sig_Content, SignalValue, 0, 2, 3);

			SignalValue = Get_Signal_Value(rx_data, 22, 2);
			Signal_Content_Failure_Verication(&TCU6_20_INDC_AT_MALFUNC_Sig_Content, SignalValue, 0, 2, 3);
		    }
		    else
		    {
		        TCU6_20_TGS_LEVER_Sig_Content.Failure_Counter = 0;
			TCU6_20_TGS_LEVER_Sig_Content.Recovery_Counter = 0;
			TCU6_20_MODE_SIGNAL_Sig_Content.Failure_Counter = 0;
			TCU6_20_MODE_SIGNAL_Sig_Content.Recovery_Counter = 0;
			TCU6_20_INDC_AT_MALFUNC_Sig_Content.Failure_Counter = 0;
			TCU6_20_INDC_AT_MALFUNC_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(TCU6_20_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        if(TCU6_20_TGS_LEVER_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_TCU6_20[2].byte |= (rx_data[2] & 0x0F);
 - 			}
 - 
 - 			if(TCU6_20_MODE_SIGNAL_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_TCU6_20[2].byte |= (rx_data[2] & 0x30);
 - 			}
 - 
 - 			if(TCU6_20_INDC_AT_MALFUNC_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_TCU6_20[2].byte |= (rx_data[2] & 0xC0);
 - 			}
 - 
 - 			CAN_TCU6_20[0].byte = rx_data[0];
 - 			CAN_TCU6_20[1].byte = rx_data[1];
 - 			CAN_TCU6_20[2].byte = rx_data[2];
 - 			CAN_TCU6_20[3].byte = rx_data[3];
 - 			CAN_TCU6_20[4].byte = rx_data[4];
 - 			CAN_TCU6_20[5].byte = rx_data[5];
 - 			CAN_TCU6_20[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    TCU6_20_Message_Content.Failure_Counter = 0;
		    TCU6_20_Message_Content.Recovery_Counter = 0;
		    TCU6_20_TGS_LEVER_Sig_Content.Failure_Counter = 0;
		    TCU6_20_TGS_LEVER_Sig_Content.Recovery_Counter = 0;
		    TCU6_20_MODE_SIGNAL_Sig_Content.Failure_Counter = 0;
		    TCU6_20_MODE_SIGNAL_Sig_Content.Recovery_Counter = 0;
		    TCU6_20_INDC_AT_MALFUNC_Sig_Content.Failure_Counter = 0;
		    TCU6_20_INDC_AT_MALFUNC_Sig_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		TCU6_20_message_ON = 0;
	}
#if 0
	/* ESC12_10 0x282 */
	if((RSCAN0.RMND0.UINT8[1] & 0x08) == 0x08)				// check if RMNS bit of RMND register is set to '4'
	{
		ESC12_10_message_ON = 1;
		//ESC12_10_message_timeout_flag = 0;

		RSCAN0.RMND0.UINT8[1] &= 0xF7;						// clear RMNS bit

		rx_msg_ID = RSCAN0.RMID11.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR11.UINT8[3] & 0xF0) >> 4;	// read received message data length
		MsgBuf_address=(MSGBUF11_BASE_ADD );

		//rx_msg_data0 = RSCAN0.RMDF0_11.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_11.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_ESC12_10[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}
	}
	else
	{
		ESC12_10_message_ON = 0;
		//ESC12_10_Message_Period_Counter = 0;
	}
#endif
	/* SRS1_20 0x2C0 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x10) == 0x10) && (U8g_Block_SRS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '5'
	{
		SRS1_20_message_ON = 1;

		RSCAN0.RMND0.UINT8[1] &= 0xEF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF12_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID12.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR12.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_SRS1_20[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_SRS1_20[rx_data_cnt].byte;	/* Store Data */
		}

		if((SRS1_20_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&SRS1_20_Message_Content, rx_msg_DLC);

		    if(SRS1_20_Message_Content.Failure_Status == CLEAR)
		    {
		        SignalValue = Get_Signal_Value(rx_data, 0, 2);
			Signal_Content_Failure_Verication(&SRS1_20_INDC_SRSC_Sig_Content, SignalValue, 0, 3, 2);
		    }
		    else
		    {
		        SRS1_20_INDC_SRSC_Sig_Content.Failure_Counter = 0;
			SRS1_20_INDC_SRSC_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(SRS1_20_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        if(SRS1_20_INDC_SRSC_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_SRS1_20[0].byte |= (rx_data[0] & 0x03);
 - 			}
 - 
 - 			CAN_SRS1_20[0].byte |= (rx_data[0] & 0x1C);
 - 			CAN_SRS1_20[1].byte = rx_data[1];
 - 			CAN_SRS1_20[2].byte = rx_data[2];
 - 			CAN_SRS1_20[3].byte = rx_data[3];
 - 			CAN_SRS1_20[4].byte = rx_data[4];
 - 			CAN_SRS1_20[5].byte = rx_data[5];
 - 			CAN_SRS1_20[6].byte = rx_data[6];
 - 			CAN_SRS1_20[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    SRS1_20_INDC_SRSC_Sig_Content.Failure_Counter = 0;
		    SRS1_20_INDC_SRSC_Sig_Content.Recovery_Counter = 0;
		    SRS1_20_Message_Content.Failure_Counter = 0;
		    SRS1_20_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		SRS1_20_message_ON = 0;
	}

	/* EMS6_500 0x308 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x20) == 0x20) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '6'
	{
		EMS6_500_message_ON = 1;
		
		RSCAN0.RMND0.UINT8[1] &= 0xDF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF13_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID13.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR13.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS6_500[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_EMS6_500[rx_data_cnt].byte;	/* Store Data */
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS6_500_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS6_500_Message_Content, rx_msg_DLC);

		    if(EMS6_500_Message_Content.Failure_Status == CLEAR)
		    {
		        SignalValue = Get_Signal_Value(rx_data, 8, 8);
			Signal_Content_Failure_Verication(&EMS6_500_DISP_AMBT_TEMP_Sig_Content, SignalValue, 0, 254, 255);
		    }
		    else
		    {
		        EMS6_500_DISP_AMBT_TEMP_Sig_Content.Failure_Counter = 0;
			EMS6_500_DISP_AMBT_TEMP_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(EMS6_500_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        if(EMS6_500_DISP_AMBT_TEMP_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_EMS6_500[1].byte = rx_data[1];
 - 			}
 - 
 - 			CAN_EMS6_500[0].byte = rx_data[0];
 - 			CAN_EMS6_500[2].byte = rx_data[2];
 - 			CAN_EMS6_500[3].byte = rx_data[3];
 - 			CAN_EMS6_500[4].byte = rx_data[4];
 - 			CAN_EMS6_500[5].byte = rx_data[5];
 - 			CAN_EMS6_500[6].byte = rx_data[6];
 - 			CAN_EMS6_500[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    EMS6_500_DISP_AMBT_TEMP_Sig_Content.Failure_Counter = 0;
		    EMS6_500_DISP_AMBT_TEMP_Sig_Content.Recovery_Counter = 0;
		    EMS6_500_Message_Content.Failure_Counter = 0;
		    EMS6_500_Message_Content.Failure_Counter = 0;
		}
	}
	else
	{
		EMS6_500_message_ON = 0;
	}

	/* EMS12_200 0x310 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x40) == 0x40) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '7'
	{
		EMS12_200_message_ON = 1;

		RSCAN0.RMND0.UINT8[1] &= 0xBF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF14_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID14.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR14.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_EMS12_200[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_EMS12_200[rx_data_cnt].byte;	/* Store Data */
		}

		if((EMS3_10_Message.Message_Timeout_Status == CLEAR) && (EMS12_200_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&EMS12_200_Message_Content, rx_msg_DLC);

/*		    if(EMS12_200_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_EMS12_200[0].byte = rx_data[0];
 - 			CAN_EMS12_200[1].byte = rx_data[1];
 - 			CAN_EMS12_200[2].byte = rx_data[2];
 - 			CAN_EMS12_200[3].byte = rx_data[3];
 - 			CAN_EMS12_200[4].byte = rx_data[4];
 - 			CAN_EMS12_200[5].byte = rx_data[5];
 - 			CAN_EMS12_200[6].byte = rx_data[6];
 - 			CAN_EMS12_200[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    EMS12_200_Message_Content.Failure_Counter = 0;
		    EMS12_200_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		EMS12_200_message_ON = 0;
	}

	/* PKE_ICU2_100 0x342 */
	if(((RSCAN0.RMND0.UINT8[1] & 0x80) == 0x80) && ((U8g_Block_PKE_ECU_CAN_MSG == CLEAR) || (U8g_Block_ICU_ECU_CAN_MSG == CLEAR)))				// check if RMNS bit of RMND register is set to '8'
	{
		PKE_ICU2_100_message_ON = 1;

		RSCAN0.RMND0.UINT8[1] &= 0x7F;						// clear RMNS bit
		MsgBuf_address=(MSGBUF15_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID15.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR15.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_PKE_ICU2_100[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_PKE_ICU2_100[rx_data_cnt].byte;	/* Store Data */
		}

		if((PKE_ICU2_100_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&PKE_ICU2_100_Message_Content, rx_msg_DLC);

/*		    if(PKE_ICU2_100_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_PKE_ICU2_100[0].byte = rx_data[0];
 - 			CAN_PKE_ICU2_100[1].byte = rx_data[1];
 - 			CAN_PKE_ICU2_100[2].byte = rx_data[2];
 - 			CAN_PKE_ICU2_100[3].byte = rx_data[3];
 - 			CAN_PKE_ICU2_100[4].byte = rx_data[4];
 - 			CAN_PKE_ICU2_100[5].byte = rx_data[5];
 - 			CAN_PKE_ICU2_100[6].byte = rx_data[6];
 - 			CAN_PKE_ICU2_100[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    PKE_ICU2_100_Message_Content.Failure_Counter = 0;
		    PKE_ICU2_100_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		PKE_ICU2_100_message_ON = 0;
	}

	/* MBFM1_100 0x348 */
	if(((RSCAN0.RMND0.UINT8[2] & 0x01) == 0x01) && (U8g_Block_BCM_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '1'
	{
		MBFM1_100_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xFE;						// clear RMNS bit
		MsgBuf_address=(MSGBUF16_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID16.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR16.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_MBFM1_100[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_MBFM1_100[rx_data_cnt].byte;	/* Store Data */
		}

		if((MBFM1_100_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&MBFM1_100_Message_Content, rx_msg_DLC);

		    if(MBFM1_100_Message_Content.Failure_Status == CLEAR)
		    {
			SignalValue = Get_Signal_Value(rx_data, 16, 11);
			Signal_Content_Failure_Verication(&MBFM1_100_MBFM_ENGOFF_Sig_Content, SignalValue, 0, 1920, 2047);
		    }
		    else
		    {
			MBFM1_100_MBFM_ENGOFF_Sig_Content.Failure_Counter = 0;
			MBFM1_100_MBFM_ENGOFF_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(MBFM1_100_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			if(MBFM1_100_MBFM_ENGOFF_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_MBFM1_100[2].byte = rx_data[2];
 - 			    CAN_MBFM1_100[3].byte |= (rx_data[3] & 0x07);
 - 			}
 - 			
 - 		        CAN_MBFM1_100[0].byte = rx_data[0];
 - 			CAN_MBFM1_100[1].byte = rx_data[1];
 - 			CAN_MBFM1_100[3].byte |= (rx_data[3] & 0xF1);
 - 			CAN_MBFM1_100[4].byte = rx_data[4];
 - 			CAN_MBFM1_100[5].byte = rx_data[5];
 - 			CAN_MBFM1_100[6].byte = rx_data[6];
 - 			CAN_MBFM1_100[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    MBFM1_100_Message_Content.Failure_Counter = 0;
		    MBFM1_100_Message_Content.Recovery_Counter = 0;
		    MBFM1_100_MBFM_ENGOFF_Sig_Content.Failure_Counter = 0;
		    MBFM1_100_MBFM_ENGOFF_Sig_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		MBFM1_100_message_ON = 0;
	}
#ifndef U350
	/* IS3_500 0x3CA */
	if(((RSCAN0.RMND0.UINT8[2] & 0x02) == 0x02) && (U8g_Block_IS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '2'
	{
		IS3_500_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xFD;						// clear RMNS bit
		MsgBuf_address=(MSGBUF17_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID17.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR17.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_IS3_500[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_IS3_500[rx_data_cnt].byte;	/* Store Data */
		}

		if((IS1_100_Message.Message_Timeout_Status == CLEAR) && (IS3_500_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&IS3_500_Message_Content, rx_msg_DLC);

/*		    if(IS3_500_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_IS3_500[0].byte = rx_data[0];
 - 			CAN_IS3_500[1].byte = rx_data[1];
 - 			CAN_IS3_500[2].byte = rx_data[2];
 - 			CAN_IS3_500[3].byte = rx_data[3];
 - 			CAN_IS3_500[4].byte = rx_data[4];
 - 			CAN_IS3_500[5].byte = rx_data[5];
 - 			CAN_IS3_500[6].byte = rx_data[6];
 - 			CAN_IS3_500[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    IS3_500_Message_Content.Failure_Counter = 0;
		    IS3_500_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		IS3_500_message_ON = 0;
	}
#endif
	/* IS1_100 0x220 */
	if(((RSCAN0.RMND0.UINT8[2] & 0x04) == 0x04) && (U8g_Block_IS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '3'
	{
		IS1_100_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xFB;						// clear RMNS bit
		MsgBuf_address=(MSGBUF18_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID18.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR18.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_IS1_100[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_IS1_100[rx_data_cnt].byte;	/* Store Data */
		}

		if((IS1_100_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&IS1_100_Message_Content, rx_msg_DLC);

/*		    if(IS1_100_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_IS1_100[0].byte = rx_data[0];
 - 			CAN_IS1_100[1].byte = rx_data[1];
 - 			CAN_IS1_100[2].byte = rx_data[2];
 - 			CAN_IS1_100[3].byte = rx_data[3];
 - 			CAN_IS1_100[4].byte = rx_data[4];
 - 			CAN_IS1_100[5].byte = rx_data[5];
 - 			CAN_IS1_100[6].byte = rx_data[6];
 - 			CAN_IS1_100[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    IS1_100_Message_Content.Failure_Counter = 0;
		    IS1_100_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		IS1_100_message_ON = 0;
	}

#ifndef U350
	/* MBFM9_500 0x21F */
	if(((RSCAN0.RMND0.UINT8[2] & 0x08) == 0x08) && (U8g_Block_BCM_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '4'
	{
		MBFM9_500_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xF7;						// clear RMNS bit
		MsgBuf_address=(MSGBUF19_BASE_ADD );

		rx_msg_ID = RSCAN0.RMID19.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR19.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_MBFM9_500[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_MBFM9_500[rx_data_cnt].byte;	/* Store Data */
		}		

		if((MBFM1_100_Message.Message_Timeout_Status == CLEAR) && (MBFM9_500_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&MBFM9_500_Message_Content, rx_msg_DLC);

/*		    if(MBFM9_500_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_MBFM9_500[0].byte = rx_data[0];
 - 			CAN_MBFM9_500[1].byte = rx_data[1];
 - 			CAN_MBFM9_500[2].byte = rx_data[2];
 - 			CAN_MBFM9_500[3].byte = rx_data[3];
 - 			CAN_MBFM9_500[4].byte = rx_data[4];
 - 			CAN_MBFM9_500[5].byte = rx_data[5];
 - 			CAN_MBFM9_500[6].byte = rx_data[6];
 - 			CAN_MBFM9_500[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    MBFM9_500_Message_Content.Failure_Counter = 0;
		    MBFM9_500_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		MBFM9_500_message_ON = 0;
	}
#endif
	/* EMS_NSM			0x285 */
	if(((RSCAN0.RMND0.UINT8[2] & 0x10) == 0x10) && (IVN_NS_VAR_CODING_ENGINE_MANAGEMENT_SYSTEM==1) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '4'
	{
		EMS_NSM_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xEF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF20_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID20.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR20.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_EMS_NSM[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=1;
		IVN_RxSynchMsgArrive=0;
		IVN_RxMAC_Indicator = CAN_EMS_NSM[4].bits.Rx_1;

		IVN_MacRxBuffer.u8[0] = CAN_EMS_NSM[0].byte;
		IVN_MacRxBuffer.u8[1] = CAN_EMS_NSM[1].byte;
		IVN_MacRxBuffer.u8[2] = CAN_EMS_NSM[2].byte;
		IVN_MacRxBuffer.u8[3] = CAN_EMS_NSM[3].byte;
	}
	else
	{
		EMS_NSM_message_ON = 0;
	}

	/* ESC_NSM			0x289 */
	if(((RSCAN0.RMND0.UINT8[2] & 0x20) == 0x20) && (IVN_NS_VAR_CODING_ELECTRONICS_STABILITY_CONTROL==1) && ((U8g_Block_ESC_ECU_CAN_MSG == CLEAR)  || (U8g_Block_ABS_ECU_CAN_MSG == CLEAR)))				// check if RMNS bit of RMND register is set to '4'
	{
		ESC_NSM_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xDF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF21_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID21.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR21.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_ESC_NSM[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=1;
		IVN_RxSynchMsgArrive=0;
		IVN_RxMAC_Indicator = CAN_ESC_NSM[4].bits.Rx_1;

		IVN_MacRxBuffer.u8[0] = CAN_ESC_NSM[0].byte;
		IVN_MacRxBuffer.u8[1] = CAN_ESC_NSM[1].byte;
		IVN_MacRxBuffer.u8[2] = CAN_ESC_NSM[2].byte;
		IVN_MacRxBuffer.u8[3] = CAN_ESC_NSM[3].byte;

	}
	else
	{
		ESC_NSM_message_ON = 0;
	}

	/* IS_NSM			0x28E */
	if(((RSCAN0.RMND0.UINT8[2] & 0x40) == 0x40) && (IVN_NS_VAR_CODING_INFOTAINMENT_SYSTEM==1) && (U8g_Block_IS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '4'
	{
		IS_NSM_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0xBF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF22_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID22.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR22.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_IS_NSM[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=1;
		IVN_RxSynchMsgArrive=0;
		IVN_RxMAC_Indicator = CAN_IS_NSM[4].bits.Rx_1;

		IVN_MacRxBuffer.u8[0] = CAN_IS_NSM[0].byte;
		IVN_MacRxBuffer.u8[1] = CAN_IS_NSM[1].byte;
		IVN_MacRxBuffer.u8[2] = CAN_IS_NSM[2].byte;
		IVN_MacRxBuffer.u8[3] = CAN_IS_NSM[3].byte;


	}
	else
	{
		IS_NSM_message_ON = 0;
	}

	/* PKE_ICU_NSM		0x287 */
	if(((RSCAN0.RMND0.UINT8[2] & 0x80) == 0x80)  && (IVN_NS_VAR_CODING_PASSIVE_KEY_ENTRY_SMART_KEY_MODULE == 1) && ((U8g_Block_PKE_ECU_CAN_MSG == CLEAR) || (U8g_Block_ICU_ECU_CAN_MSG == CLEAR)))				// check if RMNS bit of RMND register is set to '4'
	{
		//	MBFM9_500_message_ON = 1;

		RSCAN0.RMND0.UINT8[2] &= 0x7F;						// clear RMNS bit
		MsgBuf_address=(MSGBUF23_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID23.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR23.UINT8[3] & 0xF0) >> 4;	// read received message data length

		//rx_msg_data0 = RSCAN0.RMDF0_18.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_18.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_PKE_ICU_NSM[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=1;
		IVN_RxSynchMsgArrive=0;
		IVN_RxMAC_Indicator = CAN_PKE_ICU_NSM[4].bits.Rx_1;

		IVN_MacRxBuffer.u8[0] = CAN_PKE_ICU_NSM[0].byte;
		IVN_MacRxBuffer.u8[1] = CAN_PKE_ICU_NSM[1].byte;
		IVN_MacRxBuffer.u8[2] = CAN_PKE_ICU_NSM[2].byte;
		IVN_MacRxBuffer.u8[3] = CAN_PKE_ICU_NSM[3].byte;

	}
	else
	{
		//	MBFM9_500_message_ON = 0;
		//IS1_100_Message_Period_Counter = 0;
	}


	/* SRS_NSM			0x28C */
	if(((RSCAN0.RMND0.UINT8[3] & 0x01) == 0x01) && (IVN_NS_VAR_CODING_SUPPLIMENTARY_RESTRAINT_SYSTEM == 1) && (U8g_Block_SRS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '4'
	{
		//	MBFM9_500_message_ON = 1;

		RSCAN0.RMND0.UINT8[3] &= 0xFE;						// clear RMNS bit
		MsgBuf_address=(MSGBUF24_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID24.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR24.UINT8[3] & 0xF0) >> 4;	// read received message data length

		//rx_msg_data0 = RSCAN0.RMDF0_18.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_18.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_SRS_NSM[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=1;
		IVN_RxSynchMsgArrive=0;
		IVN_RxMAC_Indicator = CAN_SRS_NSM[4].bits.Rx_1;

		IVN_MacRxBuffer.u8[0] = CAN_SRS_NSM[0].byte;
		IVN_MacRxBuffer.u8[1] = CAN_SRS_NSM[1].byte;
		IVN_MacRxBuffer.u8[2] = CAN_SRS_NSM[2].byte;
		IVN_MacRxBuffer.u8[3] = CAN_SRS_NSM[3].byte;

	}
	else
	{
		//	MBFM9_500_message_ON = 0;
		//IS1_100_Message_Period_Counter = 0;
	}


	/* TCU_NSM			0x288 */
	if(((RSCAN0.RMND0.UINT8[3] & 0x02) == 0x02) && (IVN_NS_VAR_CODING_TRANSMISSION_CONTROL_UNIT == 1) && (U8g_Block_TCU_ECU_CAN_MSG == CLEAR))			// check if RMNS bit of RMND register is set to '4'
	{
		TCU_NSM_message_ON = 1;

		RSCAN0.RMND0.UINT8[3] &= 0xFD;						// clear RMNS bit
		MsgBuf_address=(MSGBUF25_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID25.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR25.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_TCU_NSM[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=1;
		IVN_RxSynchMsgArrive=0;
		IVN_RxMAC_Indicator = CAN_TCU_NSM[4].bits.Rx_1;

		IVN_MacRxBuffer.u8[0] = CAN_TCU_NSM[0].byte;
		IVN_MacRxBuffer.u8[1] = CAN_TCU_NSM[1].byte;
		IVN_MacRxBuffer.u8[2] = CAN_TCU_NSM[2].byte;
		IVN_MacRxBuffer.u8[3] = CAN_TCU_NSM[3].byte;



	}
	else
	{
		TCU_NSM_message_ON = 0;
	}

	/* SYNC_MSG_SP		0x286 */
	if((RSCAN0.RMND0.UINT8[3] & 0x04) == 0x04)				// check if RMNS bit of RMND register is set to '4'
	{
		//	MBFM9_500_message_ON = 1;

		RSCAN0.RMND0.UINT8[3] &= 0xFB;						// clear RMNS bit
		MsgBuf_address=(MSGBUF26_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID26.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR26.UINT8[3] & 0xF0) >> 4;	// read received message data length

		//rx_msg_data0 = RSCAN0.RMDF0_18.UINT32;
		//rx_msg_data1 = RSCAN0.RMDF1_18.UINT32;
		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
			CAN_SYNC_MSG_SP[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	/* Store Data */
		}

		IVN_RxNSMMsgArrive=0;
		IVN_RxSynchMsgArrive=1;

		IVN_MacRxBuffer.u8[0] = CAN_TCU_NSM[4].byte;
		IVN_MacRxBuffer.u8[1] = CAN_TCU_NSM[5].byte;
		IVN_MacRxBuffer.u8[2] = CAN_TCU_NSM[6].byte;
		IVN_MacRxBuffer.u8[3] = CAN_TCU_NSM[7].byte;

		IVN_MAC0RxBuffer.u8[0] = CAN_TCU_NSM[0].byte;
		IVN_MAC0RxBuffer.u8[1] = CAN_TCU_NSM[1].byte;
		IVN_MAC0RxBuffer.u8[2] = CAN_TCU_NSM[2].byte;
		IVN_MAC0RxBuffer.u8[3] = CAN_TCU_NSM[3].byte;

	}
	else
	{
		//	MBFM9_500_message_ON = 0;
		//IS1_100_Message_Period_Counter = 0;
	}
/****************************************************************************************************************/
	/* RPAS1_20 0x227 */
	if(((RSCAN0.RMND0.UINT8[3] & 0x08) == 0x08) && (U8g_Block_RPAS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '3'
	{
		RPAS1_20_message_ON = 1;

		RSCAN0.RMND0.UINT8[3] &= 0xF7;						// clear RMNS bit
		MsgBuf_address=(MSGBUF27_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID27.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR27.UINT8[3] & 0xF0) >> 4;	// read received message data length


		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_RPAS1_20[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_RPAS1_20[rx_data_cnt].byte;	/* Store Data */
		}

		if((RPAS1_20_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&RPAS1_20_Message_Content, rx_msg_DLC);

		    if(RPAS1_20_Message_Content.Failure_Status == CLEAR)
		    {
			SignalValue = Get_Signal_Value(rx_data, 56, 8);
			Signal_Content_Failure_Verication(&RPAS1_20_DISP_DIST_Sig_Content, SignalValue, 0, 250, 255);
		    }
		    else
		    {
			RPAS1_20_DISP_DIST_Sig_Content.Failure_Counter = 0;
			RPAS1_20_DISP_DIST_Sig_Content.Recovery_Counter = 0;
		    }

/*		    if(RPAS1_20_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 			if(RPAS1_20_DISP_DIST_Sig_Content.Failure_Status == CLEAR)
 - 			{
 - 			    CAN_RPAS1_20[7].byte = rx_data[7];
 - 			}
 - 			
 - 		        CAN_RPAS1_20[0].byte = rx_data[0];
 - 			CAN_RPAS1_20[1].byte = rx_data[1];
 - 			CAN_RPAS1_20[2].byte = rx_data[2];
 - 			CAN_RPAS1_20[3].byte = rx_data[3];
 - 			CAN_RPAS1_20[4].byte = rx_data[4];
 - 			CAN_RPAS1_20[5].byte = rx_data[5];
 - 			CAN_RPAS1_20[6].byte = rx_data[6];
 - 		    }*/
		}
		else
		{
		    RPAS1_20_DISP_DIST_Sig_Content.Failure_Counter = 0;
		    RPAS1_20_DISP_DIST_Sig_Content.Recovery_Counter = 0;
		    RPAS1_20_Message_Content.Failure_Counter = 0;
		    RPAS1_20_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		RPAS1_20_message_ON = 0;
	}
/****************************************************************************************************************/

	/* RPAS2_100			0x229 */
	if(((RSCAN0.RMND0.UINT8[3] & 0x10) == 0x10) && (U8g_Block_RPAS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '5'
	{
		RPAS2_100_message_ON = 1;

		RSCAN0.RMND0.UINT8[3] &= 0xEF;						// clear RMNS bit
		MsgBuf_address=(MSGBUF28_BASE_ADD);

		rx_msg_ID = RSCAN0.RMID28.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR28.UINT8[3] & 0xF0) >> 4;	// read received message data length

		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
		{
		    CAN_RPAS2_100[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		    rx_data[rx_data_cnt] = CAN_RPAS2_100[rx_data_cnt].byte;	/* Store Data */
		}

		if((RPAS1_20_Message.Message_Timeout_Status == CLEAR) && (RPAS2_100_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
		{
		    Message_Content_Failure_Verication(&RPAS2_100_Message_Content, rx_msg_DLC);

/*		    if(RPAS2_100_Message_Content.Failure_Status == CLEAR)
 - 		    {
 - 		        CAN_RPAS2_100[0].byte = rx_data[0];
 - 			CAN_RPAS2_100[1].byte = rx_data[1];
 - 			CAN_RPAS2_100[2].byte = rx_data[2];
 - 			CAN_RPAS2_100[3].byte = rx_data[3];
 - 			CAN_RPAS2_100[4].byte = rx_data[4];
 - 			CAN_RPAS2_100[5].byte = rx_data[5];
 - 			CAN_RPAS2_100[6].byte = rx_data[6];
 - 			CAN_RPAS2_100[7].byte = rx_data[7];
 - 		    }*/
		}
		else
		{
		    RPAS2_100_Message_Content.Failure_Counter = 0;
		    RPAS2_100_Message_Content.Recovery_Counter = 0;
		}
	}
	else
	{
		RPAS2_100_message_ON = 0;
	}

/*******************************************************************************************************************/

		/* EMS5_500					0x3C0 */
/*	if(((RSCAN0.RMND0.UINT8[3] & 0x20) == 0x20) && (U8g_Block_EMS_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '5'
 - 	{
 - 		EMS5_500_message_ON = 1;
 - 
 - 		RSCAN0.RMND0.UINT8[3] &= 0xDF;						// clear RMNS bit
 - 		MsgBuf_address=(MSGBUF29_BASE_ADD);
 - 
 - 		rx_msg_ID = RSCAN0.RMID29.UINT16[0];				// read received message ID
 - 		rx_msg_DLC = (RSCAN0.RMPTR29.UINT8[3] & 0xF0) >> 4;	// read received message data length
 - 
 - 		for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
 - 		{
 - 			CAN_EMS5_500[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));	@/@*$ Store Data $*@/@
 - 		}
 - 	}
 - 	else
 - 	{
 - 		EMS5_500_message_ON = 0;
 - 	}*/
/*******************************************************************************************************************/
#ifndef U350
	/* MBFM_PAS1_50				0x326 */
	if(((RSCAN0.RMND0.UINT8[3] & 0x40) == 0x40) && (U8g_Block_BCM_ECU_CAN_MSG == CLEAR))				// check if RMNS bit of RMND register is set to '6'
	{
	    MBFM_PAS1_50_message_ON = 1;

	    RSCAN0.RMND0.UINT8[3] &= 0xBF;						// clear RMNS bit
	    MsgBuf_address=(MSGBUF30_BASE_ADD);

	    rx_msg_ID = RSCAN0.RMID30.UINT16[0];				// read received message ID
	    rx_msg_DLC = (RSCAN0.RMPTR30.UINT8[3] & 0xF0) >> 4;	// read received message data length

	    for( rx_data_cnt = 0;  rx_data_cnt < 8;  rx_data_cnt++)
	    {
		CAN_MBFM_PAS1_50[rx_data_cnt].byte = *(MsgBuf_address + (0x01 * rx_data_cnt));
		rx_data[rx_data_cnt] = CAN_MBFM_PAS1_50[rx_data_cnt].byte;	/* Store Data */
	    }

	    if((MBFM1_100_Message.Message_Timeout_Status == CLEAR) && (MBFM_PAS1_50_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
				(HVSD_CAN == 0))
	    {
		Message_Content_Failure_Verication(&MBFM_PAS1_50_Message_Content, rx_msg_DLC);

/*		if(MBFM_PAS1_50_Message_Content.Failure_Status == CLEAR)
 - 		{
 - 		    CAN_MBFM_PAS1_50[0].byte = rx_data[0];
 - 		    CAN_MBFM_PAS1_50[1].byte = rx_data[1];
 - 		    CAN_MBFM_PAS1_50[2].byte = rx_data[2];
 - 		    CAN_MBFM_PAS1_50[3].byte = rx_data[3];
 - 		    CAN_MBFM_PAS1_50[4].byte = rx_data[4];
 - 		    CAN_MBFM_PAS1_50[5].byte = rx_data[5];
 - 		    CAN_MBFM_PAS1_50[6].byte = rx_data[6];
 - 		    CAN_MBFM_PAS1_50[7].byte = rx_data[7];
 - 		}*/
	    }
	    else
	    {
		MBFM_PAS1_50_Message_Content.Failure_Counter = 0;
		MBFM_PAS1_50_Message_Content.Recovery_Counter = 0;
	    }
	}
	else
	{
	    MBFM_PAS1_50_message_ON = 0;
	}
#endif
/*******************************************************************************************************************/
	/* 0x7E2 */
#endif
#if 0
	if((RSCAN0.RMND0.UINT8[2] & 0x08) == 0x08)				// check if RMNS bit of RMND register is set to '3'
	{
		RSCAN0.RMND0.UINT8[2] &= 0xF7;						// clear RMNS bit

		rx_msg_ID = RSCAN0.RMID19.UINT16[0];				// read received message ID
		rx_msg_DLC = (RSCAN0.RMPTR19.UINT8[3] & 0xF0) >> 4;	// read received message data length

		rx_msg_data0 = RSCAN0.RMDF0_19.UINT32;
		rx_msg_data1 = RSCAN0.RMDF1_19.UINT32;

		UDS_Buffer[0] = RSCAN0.RMDF0_19.UINT8[0];
		UDS_Buffer[1] = RSCAN0.RMDF0_19.UINT8[1];
		UDS_Buffer[2] = RSCAN0.RMDF0_19.UINT8[2];
		UDS_Buffer[3] = RSCAN0.RMDF0_19.UINT8[3];
		UDS_Buffer[4] = RSCAN0.RMDF1_19.UINT8[0];
		UDS_Buffer[5] = RSCAN0.RMDF1_19.UINT8[1];
		UDS_Buffer[6] = RSCAN0.RMDF1_19.UINT8[2];
		UDS_Buffer[7] = RSCAN0.RMDF1_19.UINT8[3];

		my_can0_FIFO_rx();
	}

#endif

	//-------------------- Rx by reading Rx FIFO buffer-------------------------------------------------------
#if 0
	RSCAN0.RFSTS0.UINT8[0] &= 0xF7;	// CLEAR RFIF flag
	if((RSCAN0.RFSTS0.UINT8[0] & 0x01) != 0x01)
	{
		do
		{
			rx_msg_ID = RSCAN0.RFID0.UINT16[0];
			rx_msg_DLC = (RSCAN0.RFPTR0.UINT8[3] & 0xf0) >> 4;

			rx_msg_data0 = RSCAN0.RFDF00.UINT32;
			rx_msg_data1 = RSCAN0.RFDF10.UINT32;

			RSCAN0.RFPCTR0.UINT8[0] = 0xFF;
		}while((RSCAN0.RFSTS0.UINT8[0] & 0x01) != 0x01);
	}
#endif

	//-------------------- Rx by reading Tx/Rx FIFO buffer-------------------------------------------------------

	unsigned char temp1, temp2;
#if 0
	RSCAN0.CFSTS0.UINT8[0] &= 0xF7;	// CLEAR CFRXIF flag
	if((RSCAN0.CFSTS0.UINT8[0] & 0x01) != 0x01)
	{
		do
		{
			rx_msg_ID = RSCAN0.CFID0.UINT16[0];
			rx_msg_DLC = (RSCAN0.CFPTR0.UINT8[3] & 0xf0) >> 4;

			switch(rx_msg_ID)
			{
			case 0x111:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				my_speed_val = RSCAN0.CFDF0_0.UINT8[0];
				break;
			case 0x222:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				my_immo_val = RSCAN0.CFDF0_0.UINT8[0];
				break;
			case 0x333:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				temp1 = RSCAN0.CFDF0_0.UINT8[1];
				temp2 = RSCAN0.CFDF0_0.UINT8[0];

				my_rpm_val = (((unsigned int)temp2 << 8) | temp1);
				break;

			case 0x444:
				temp1 = RSCAN0.CFDF0_0.UINT8[1];
				temp2 = RSCAN0.CFDF0_0.UINT8[0];
				my_batt_vg = RSCAN0.CFDF0_0.UINT8[0]; //(((unsigned int)temp2 << 8) | temp1);
				break;
			default:
				break;

			}
			//rx_msg_data0 = RSCAN0.CFDF0_0.UINT8[LL];



			RSCAN0.CFPCTR0.UINT8[0] = 0xFF;
		}while((RSCAN0.CFSTS0.UINT8[0] & 0x01) != 0x01);
	}
	//my_pressed_key = my_can_process(rx_msg_data0);
	//return my_pressed_key;
	return rx_msg_data0;
#endif
	//--------------------------------------------------------------------------------------------------------
}

void my_can0_FIFO_rx(void)
{
    uint8_t rx_data[8] = { 0 };
    uint32_t SignalValue = 0;
    
	if((RSCAN0.CFSTS0.UINT8[0] & 0x01) != 0x01)
	{
		do
		{
			rx_msg_ID = RSCAN0.CFID0.UINT16[0];
			rx_msg_DLC = (RSCAN0.CFPTR0.UINT8[3] & 0xf0) >> 4;

			switch(rx_msg_ID)
			{
			case 0x7E2:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				//my_speed_val = RSCAN0.CFDF0_0.UINT8[0];
				UDS_Buffer[0]= RSCAN0.CFDF0_0.UINT8[0];
				UDS_Buffer[1]= RSCAN0.CFDF0_0.UINT8[1];
				UDS_Buffer[2]= RSCAN0.CFDF0_0.UINT8[2];
				UDS_Buffer[3]= RSCAN0.CFDF0_0.UINT8[3];
				UDS_Buffer[4]= RSCAN0.CFDF1_0.UINT8[0];
				UDS_Buffer[5]= RSCAN0.CFDF1_0.UINT8[1];
				UDS_Buffer[6]= RSCAN0.CFDF1_0.UINT8[2];
				UDS_Buffer[7]= RSCAN0.CFDF1_0.UINT8[3];

				Functional_Addressing_Flag = CLEAR;
				if(rx_msg_ID == 0x7E2)
				{
					/*			frame_type = ((RSCAN0.RFDF0_0.UINT8[0] & 0xF0) >> 4);*/
					//	frame_type = ((RSCAN0.RMDF0_19.UINT8[0] & 0xF0) >> 4);
					frame_type = ((UDS_Buffer[0] & 0xF0) >> 4);
					switch(frame_type)
					{
					case 0:
						FrameType.Single_Frame = SET;
						P2CAN_ECU_Cntr         = SET;
						S3ECU_cntr             = CLEAR;
						break;

					case 1:
						FrameType.First_Frame = SET;
						P2CAN_ECU_Cntr        = SET;
						S3ECU_cntr            = CLEAR;
						break;

					case 2:
						FrameType.Cons_Frame  = SET;
						rx_msg_DLC            = 3;
						break;

					case 3:
						FrameType.Flow_Control = SET;
						//flow_control_rec     = SET;
						break;

					default:

						break;
					}
					//		Process_UDSData();
				}
				break;
			case 0x7DF:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				//my_speed_val = RSCAN0.CFDF0_0.UINT8[0];
				UDS_Buffer[0]= RSCAN0.CFDF0_0.UINT8[0];
				UDS_Buffer[1]= RSCAN0.CFDF0_0.UINT8[1];
				UDS_Buffer[2]= RSCAN0.CFDF0_0.UINT8[2];
				UDS_Buffer[3]= RSCAN0.CFDF0_0.UINT8[3];
				UDS_Buffer[4]= RSCAN0.CFDF1_0.UINT8[0];
				UDS_Buffer[5]= RSCAN0.CFDF1_0.UINT8[1];
				UDS_Buffer[6]= RSCAN0.CFDF1_0.UINT8[2];
				UDS_Buffer[7]= RSCAN0.CFDF1_0.UINT8[3];

				Functional_Addressing_Flag = SET;
				
				if(rx_msg_ID == 0x7DF)
				{
					frame_type = ((UDS_Buffer[0] & 0xF0) >> 4);

					switch(frame_type)
					{
					case 0:
						FrameType.Single_Frame = SET;
						P2CAN_ECU_Cntr         = SET;
						S3ECU_cntr             = CLEAR;
						break;

					default:
						break;
					}
				}
				break;
			case ESC2_10:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				if((U8g_Block_ESC_ECU_CAN_MSG == CLEAR)  || (U8g_Block_ABS_ECU_CAN_MSG == CLEAR))
				{
				    //my_speed_val = RSCAN0.CFDF0_0.UINT8[0];
				    CAN_ESC2_10[0].byte= RSCAN0.CFDF0_0.UINT8[0];
				    CAN_ESC2_10[1].byte= RSCAN0.CFDF0_0.UINT8[1];
				    CAN_ESC2_10[2].byte= RSCAN0.CFDF0_0.UINT8[2];
				    CAN_ESC2_10[3].byte= RSCAN0.CFDF0_0.UINT8[3];
				    CAN_ESC2_10[4].byte= RSCAN0.CFDF1_0.UINT8[0];
				    CAN_ESC2_10[5].byte= RSCAN0.CFDF1_0.UINT8[1];
				    CAN_ESC2_10[6].byte= RSCAN0.CFDF1_0.UINT8[2];
				    CAN_ESC2_10[7].byte= RSCAN0.CFDF1_0.UINT8[3];
				    
				    rx_data[0] = CAN_ESC2_10[0].byte;
				    rx_data[1] = CAN_ESC2_10[1].byte;
				    rx_data[2] = CAN_ESC2_10[2].byte;
				    rx_data[3] = CAN_ESC2_10[3].byte;
				    rx_data[4] = CAN_ESC2_10[4].byte;
				    rx_data[5] = CAN_ESC2_10[5].byte;
				    rx_data[6] = CAN_ESC2_10[6].byte;
				    rx_data[7] = CAN_ESC2_10[7].byte;

				    ESC2_10_message_ON = 1;

				    if((ESC5_10_Message.Message_Timeout_Status == CLEAR) && (ESC2_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
							(HVSD_CAN == 0))
				    {
					Message_Content_Failure_Verication(&ESC2_10_Message_Content, rx_msg_DLC);

					if(FEATURE_CODE_SPEED_INPUT == CLEAR)
					{
					    if((ESC2_10_Message_Content.Failure_Status == CLEAR))
					    {
						SignalValue = Get_Signal_Value(rx_data, 0, 16);
						Signal_Content_Failure_Verication(&ESC2_10_ABS_VEH_SPEED_Sig_Content, SignalValue, 0, 19200, 65535);

						SignalValue = Get_Signal_Value(rx_data, 16, 16);
						Signal_Content_Failure_Verication(&ESC2_10_ABS_ODO_DIST_Sig_Content, SignalValue, 0, 65534, 65535);
					    }
					    else
					    {
						ESC2_10_ABS_VEH_SPEED_Sig_Content.Failure_Counter = 0;
						ESC2_10_ABS_VEH_SPEED_Sig_Content.Recovery_Counter = 0;
						ESC2_10_ABS_ODO_DIST_Sig_Content.Failure_Counter = 0;
						ESC2_10_ABS_ODO_DIST_Sig_Content.Recovery_Counter = 0;
					    }
					}
				    }
				    else
				    {
					ESC2_10_Message_Content.Failure_Counter = 0;
					ESC2_10_Message_Content.Recovery_Counter = 0;
				    }
				}

				odometer();
				break;
			case ESC12_10:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
				if((U8g_Block_ESC_ECU_CAN_MSG == CLEAR)  || (U8g_Block_ABS_ECU_CAN_MSG == CLEAR))
				{
				    //my_speed_val = RSCAN0.CFDF0_0.UINT8[0];
				    CAN_ESC12_10[0].byte= RSCAN0.CFDF0_0.UINT8[0];
				    CAN_ESC12_10[1].byte= RSCAN0.CFDF0_0.UINT8[1];
				    CAN_ESC12_10[2].byte= RSCAN0.CFDF0_0.UINT8[2];
				    CAN_ESC12_10[3].byte= RSCAN0.CFDF0_0.UINT8[3];
				    CAN_ESC12_10[4].byte= RSCAN0.CFDF1_0.UINT8[0];
				    CAN_ESC12_10[5].byte= RSCAN0.CFDF1_0.UINT8[1];
				    CAN_ESC12_10[6].byte= RSCAN0.CFDF1_0.UINT8[2];
				    CAN_ESC12_10[7].byte= RSCAN0.CFDF1_0.UINT8[3];
				    
				    rx_data[0] = CAN_ESC12_10[0].byte;
				    rx_data[1] = CAN_ESC12_10[1].byte;
				    rx_data[2] = CAN_ESC12_10[2].byte;
				    rx_data[3] = CAN_ESC12_10[3].byte;
				    rx_data[4] = CAN_ESC12_10[4].byte;
				    rx_data[5] = CAN_ESC12_10[5].byte;
				    rx_data[6] = CAN_ESC12_10[6].byte;
				    rx_data[7] = CAN_ESC12_10[7].byte;

				    ESC12_10_message_ON = 1;

				    if((ESC5_10_Message.Message_Timeout_Status == CLEAR) && (ESC12_10_Message.Message_Timeout_Status == CLEAR)&& (LVSD_CAN == 0)&&
							(HVSD_CAN == 0))
				    {
					Message_Content_Failure_Verication(&ESC12_10_Message_Content, rx_msg_DLC);

					if(FEATURE_CODE_SPEED_INPUT == SET)
					{
					    if((ESC12_10_Message_Content.Failure_Status == CLEAR))
					    {
						SignalValue = Get_Signal_Value(rx_data, 16, 16);
						Signal_Content_Failure_Verication(&ESC2_10_ABS_VEH_SPEED_Sig_Content, SignalValue, 0, 19200, 65535);

						SignalValue = Get_Signal_Value(rx_data, 0, 16);
						Signal_Content_Failure_Verication(&ESC2_10_ABS_ODO_DIST_Sig_Content, SignalValue, 0, 65534, 65535);
					    }
					    else
					    {
						ESC2_10_ABS_VEH_SPEED_Sig_Content.Failure_Counter = 0;
						ESC2_10_ABS_VEH_SPEED_Sig_Content.Recovery_Counter = 0;
						ESC2_10_ABS_ODO_DIST_Sig_Content.Failure_Counter = 0;
						ESC2_10_ABS_ODO_DIST_Sig_Content.Recovery_Counter = 0;
					    }
					}
				    }
				    else
				    {
					ESC12_10_Message_Content.Failure_Counter = 0;
					ESC12_10_Message_Content.Recovery_Counter = 0;
				    }
				}

				odometer();

				break;

			case EMS5_500:
				rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
				rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;

				CAN_EMS5_500[0].byte= RSCAN0.CFDF0_0.UINT8[0];
				CAN_EMS5_500[1].byte= RSCAN0.CFDF0_0.UINT8[1];
				CAN_EMS5_500[2].byte= RSCAN0.CFDF0_0.UINT8[2];
				CAN_EMS5_500[3].byte= RSCAN0.CFDF0_0.UINT8[3];
				CAN_EMS5_500[4].byte= RSCAN0.CFDF1_0.UINT8[0];
				CAN_EMS5_500[5].byte= RSCAN0.CFDF1_0.UINT8[1];
				CAN_EMS5_500[6].byte= RSCAN0.CFDF1_0.UINT8[2];
				CAN_EMS5_500[7].byte= RSCAN0.CFDF1_0.UINT8[3];

				VIN_Mismatch(&CAN_EMS5_500);
			default:
				break;
			}
			RSCAN0.CFPCTR0.UINT8[0] = 0xFF;
		}while((RSCAN0.CFSTS0.UINT8[0] & 0x01) != 0x01);
	}
}
//**************************************************************************************************************************

//#pragma interrupt CAN_Global_Error_ISR(enable=false)
#pragma ghs interrupt
//void CAN_Global_Error_ISR(void)
void INTRCANGERR (void)
{
	//__nop();
}

//**************************************************************************************************************************

//#pragma interrupt CAN_Rx_FIFO_ISR(enable=false)
#pragma ghs interrupt
//void CAN_Rx_FIFO_ISR(void)
void INTRCANGRECC (void)
{
	//__nop();
	//rx_flag = 1;
	RSCAN0.RFSTS0.UINT8[0] &= 0xF7;	// CLEAR RFIF flag
}

//**************************************************************************************************************************

//#pragma interrupt CAN0_Error_ISR(enable=false)
#pragma ghs interrupt
//void CAN0_Error_ISR(void)
void INTRCAN0ERR(void)
{
	//__nop();
	//RSCAN0.C0ERFL.UINT32 = 0x0000;// w501
	//RSCAN0.C1ERFL.UINT32 = 0x0000;// w501
	RSCAN0.C2ERFL.UINT32 = 0x0000;// w501

}

//**************************************************************************************************************************

//#pragma interrupt CAN0_Tx_Rx_FIFO_Receive_ISR(enable=false)
#pragma ghs interrupt
//void CAN0_Tx_Rx_FIFO_Receive_ISR(void)
void INTRCAN0REC(void)
{
	//__nop();
	// rx_flag = 1;
    CAN_Bus_Off = CLEAR;
	if(IGN==1)
	{
	my_can0_FIFO_rx();
	}
	else if(IGN==0)
	{

		can_IGNOFF_FIFO_rx();
		IGNOFF_CAN_counter = 0;

	}
	RSCAN0.CFSTS0.UINT8[0] &= 0xF7;	// CLEAR Interrupt flag
}

//**************************************************************************************************************************

//#pragma interrupt CAN0_Tx_ISR(enable=false)
#pragma ghs interrupt
//void CAN0_Tx_ISR(void)
void INTRCAN0TRX(void)
{

#if TX_MODE == TX_BUFFER	
	RSCAN0.TMSTS0.UINT8 = 0x00;
	//__nop();
#else						// TX_RX_FIFO	
	RSCAN0.CFSTS0.UINT8[0] &= 0xE7;	//cler interrupt flags
#endif
	INTC2EIC73 &= 0xEFFF;
	//	__nop();
	tx_done = 1;

}

//**************************************************************************************************************************
/********************* UDS message processing ********************************/
#if 0
void tx_msgbuf_processing(unsigned char tx_msg_DLC,unsigned char* tx_msg_data)
{
	RSCAN0.TMIEC0.UINT8[0] = 0x01;		// Transmit Buffer 0 interrupt enabled

	RSCAN0.TMID0.UINT16[0] = 0x7EA;	// Transmit message, standard data frame,

	RSCAN0.TMPTR0.UINT16[1] = 0x8012;	// 8 data bytes, label value as 0x12

	RSCAN0.TMDF0_0.UINT8[0] =  tx_msg_data[0];	// data bytes 0
	RSCAN0.TMDF0_0.UINT8[1] =  tx_msg_data[1];
	RSCAN0.TMDF0_0.UINT8[2] =  tx_msg_data[2];
	RSCAN0.TMDF0_0.UINT8[3] =  tx_msg_data[3];

	RSCAN0.TMDF1_0.UINT8[0] =  tx_msg_data[4];
	RSCAN0.TMDF1_0.UINT8[1] =  tx_msg_data[5];
	RSCAN0.TMDF1_0.UINT8[2] =  tx_msg_data[6];
	RSCAN0.TMDF1_0.UINT8[3] =  tx_msg_data[7];

	if((RSCAN0TMSTS0 &0xF9) == 0x00)	//check if no other transmit request is present or transmission is in progress
		RSCAN0TMC0 = 0x01;			// Transmission requested - TMTR bit
}
#endif

#if 0
//Receive buffer for FIFO
void my_can0_FIFO_rx(void)
{
	unsigned char frame_type;
#if 0
	rx_msg_rmnd=RSCAN0.RFSTS0.UINT32;
	RSCAN0.RFSTS0.UINT8[0] &= 0xF7;	// CLEAR RFIF flag
	if((RSCAN0.RFSTS0.UINT8[0] & 0x01) != 0x01)
	{
		do
		{
			rx_msg_ID = RSCAN0.RFID0.UINT16[0];
			rx_msg_DLC = (RSCAN0.RFPTR0.UINT8[3] & 0xf0) >> 4;

			rx_msg_data0 = RSCAN0.RFDF0_0.UINT32;
			rx_msg_data1 = RSCAN0.RFDF1_0.UINT32;

			UDS_Buffer[0] = RSCAN0.RFDF0_0.UINT8[0];
			UDS_Buffer[1] = RSCAN0.RFDF0_0.UINT8[1];
			UDS_Buffer[2] = RSCAN0.RFDF0_0.UINT8[2];
			UDS_Buffer[3] = RSCAN0.RFDF0_0.UINT8[3];
			UDS_Buffer[4] = RSCAN0.RFDF1_0.UINT8[0];
			UDS_Buffer[5] = RSCAN0.RFDF1_0.UINT8[1];
			UDS_Buffer[6] = RSCAN0.RFDF1_0.UINT8[2];
			UDS_Buffer[7] = RSCAN0.RFDF1_0.UINT8[3];

			PORT_ISOPNOT16|=0x04u;
			RSCAN0.RFPCTR0.UINT8[0] = 0xFF;
		}while((RSCAN0.RFSTS0.UINT8[0] & 0x01) != 0x01);
#endif
		if(rx_msg_ID == 0x7E2)
		{
			/*			frame_type = ((RSCAN0.RFDF0_0.UINT8[0] & 0xF0) >> 4);*/
			frame_type = ((RSCAN0.RMDF0_19.UINT8[0] & 0xF0) >> 4);
			switch(frame_type)
			{
			case 0:
				FrameType.Single_Frame = SET;
				P2CAN_ECU_Cntr         = SET;
				S3ECU_cntr             = CLEAR;
				break;

			case 1:
				FrameType.First_Frame = SET;
				P2CAN_ECU_Cntr        = SET;
				S3ECU_cntr            = CLEAR;
				break;

			case 2:
				FrameType.Cons_Frame  = SET;
				rx_msg_DLC            = 3;
				break;

			case 3:
				FrameType.Flow_Control = SET;
				//flow_control_rec     = SET;
				break;

			default:

				break;
			}
			//		Process_UDSData();
		}

	}

#endif

	//void CAN_Zero_tx(void)

	void tx_msgbuf_processing(unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data)
	{
		uint16_t tx1_can_id 				 = 0x07EA;
		uint8_t  tx1_can_msg_l			 = 0x08;

		/* Transmit buffer 16-31 from CAN0(CAN driver -> channel 1) */

		/* Transmition buffer 16 from CAN driver channel 1 */
		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID32.UINT16[0] = tx1_can_id ;		// Transmit message, standard data frame,
		RSCAN0.TMPTR32.UINT16[1] = 0x0013 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_32.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_32.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_32.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_32.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_32.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_32.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_32.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_32.UINT8[HH] = tx1_can_msg_data[7];
		if((RSCAN0.TMSTS32.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS32.UINT8 &= 0xF9;

			RSCAN0.TMC32.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}
	/* Transmission message Dummy CAN ID  */

	void my_can0x211_tx(unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		uint16_t tx1_can_id 				 = 0x0211;
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//	uint8_t  tx1_can_msg_data[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};

		/* Transmit buffer 32-45 from CAN0(CAN driver -> channel 2) */

		/* Transmition buffer 33 from CAN driver channel 2 */

		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID33.UINT16[0] = tx1_can_id ;		// Transmit message, standard data frame,
		RSCAN0.TMPTR33.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_33.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_33.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_33.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_33.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_33.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_33.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_33.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_33.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS33.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS33.UINT8 &= 0xF9;

			RSCAN0.TMC33.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}
#if 1
	void my_can0_tx(unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		uint16_t tx1_can_id 				 = 0x0340;
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//		uint8_t  tx1_can_msg_data[8] = {0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		/* Transmit buffer 00-15 from CAN0(CAN driver -> channel 0) */

		/* Transmition buffer 00 from CAN driver channel 0 */
		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID34.UINT16[0] = tx1_can_id ;			// Transmit message, standard data frame,
		RSCAN0.TMPTR34.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_34.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_34.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_34.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_34.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_34.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_34.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_34.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_34.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS34.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS34.UINT8 &= 0xF9;

			RSCAN0.TMC34.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}

	void my_can0_tx_IC4TEST(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		//uint16_t tx1_can_id 				 = 0x0340;
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//		uint8_t  tx1_can_msg_data[8] = {0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		/* Transmit buffer 00-15 from CAN0(CAN driver -> channel 0) */

		/* Transmition buffer 00 from CAN driver channel 0 */
		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID35.UINT16[0] = tx1_can_id ;			// Transmit message, standard data frame,
		RSCAN0.TMPTR35.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_35.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_35.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_35.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_35.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_35.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_35.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_35.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_35.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS35.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS35.UINT8 &= 0xF9;

			RSCAN0.TMC35.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}

	void my_can0_tx_ICTEST(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		//uint16_t tx1_can_id 				 = 0x0340;
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//		uint8_t  tx1_can_msg_data[8] = {0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		/* Transmit buffer 00-15 from CAN0(CAN driver -> channel 0) */

		/* Transmition buffer 00 from CAN driver channel 0 */
		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID36.UINT16[0] = tx1_can_id ;			// Transmit message, standard data frame,
		RSCAN0.TMPTR36.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_36.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_36.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_36.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_36.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_36.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_36.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_36.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_36.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS36.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS36.UINT8 &= 0xF9;

			RSCAN0.TMC36.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}
#if 1
	void my_can0_tx_IC6TEST(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		//uint16_t tx1_can_id 				 = 0x0340;
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//		uint8_t  tx1_can_msg_data[8] = {0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		/* Transmit buffer 00-15 from CAN0(CAN driver -> channel 0) */

		/* Transmition buffer 00 from CAN driver channel 0 */
		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID37.UINT16[0] = tx1_can_id ;			// Transmit message, standard data frame,
		RSCAN0.TMPTR37.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_37.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_37.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_37.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_37.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_37.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_37.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_37.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_37.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS37.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS37.UINT8 &= 0xF9;

			RSCAN0.TMC37.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}
#endif
#endif

	void my_can0_tx_IC_NSM(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//		uint8_t  tx1_can_msg_data[8] = {0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		/* Transmit buffer 00-15 from CAN0(CAN driver -> channel 0) */
		/* Transmition buffer 00 from CAN driver channel 0 */

		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID38.UINT16[0] = tx1_can_id ;			// Transmit message, standard data frame,
		RSCAN0.TMPTR38.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_38.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_38.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_38.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_38.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_38.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_38.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_38.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_38.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS38.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS38.UINT8 &= 0xF9;

			RSCAN0.TMC38.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}
	/**/
	void my_can0_tx_IC10_200(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data )
	{
		uint8_t  tx1_can_msg_l			 = tx_msg_DLC;
		//		uint8_t  tx1_can_msg_data[8] = {0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00};
		/* Transmit buffer 00-15 from CAN0(CAN driver -> channel 0) */
		/* Transmition buffer 00 from CAN driver channel 0 */

		RSCAN0.TMIEC1.UINT8[0] = 0x00;					/* Disable the TX finish interrupt */
		RSCAN0.TMID39.UINT16[0] = tx1_can_id ;			// Transmit message, standard data frame,
		RSCAN0.TMPTR39.UINT16[1] = 0x0012 + ((((uint16_t)(tx1_can_msg_l))<<12)&0xF000) ;				// 8 data bytes, label value as 0x12 historic buffer
		RSCAN0.TMDF0_39.UINT8[LL] = tx1_can_msg_data[0];
		RSCAN0.TMDF0_39.UINT8[LH] = tx1_can_msg_data[1];
		RSCAN0.TMDF0_39.UINT8[HL] = tx1_can_msg_data[2];
		RSCAN0.TMDF0_39.UINT8[HH] = tx1_can_msg_data[3];
		RSCAN0.TMDF1_39.UINT8[LL] = tx1_can_msg_data[4];
		RSCAN0.TMDF1_39.UINT8[LH] = tx1_can_msg_data[5];
		RSCAN0.TMDF1_39.UINT8[HL] = tx1_can_msg_data[6];
		RSCAN0.TMDF1_39.UINT8[HH] = tx1_can_msg_data[7];

		if((RSCAN0.TMSTS39.UINT8 &0xF9) == 0x00)		//check if no other transmit request is present or transmission is in progress
		{
			RSCAN0.TMSTS39.UINT8 &= 0xF9;

			RSCAN0.TMC39.UINT8 = 0x01;	// Transmission requested - TMTR bit
		}
	}
	/**/
	void can_IGNOFF_init(void)
	{ uint8_t Backlight_ON_IGNOFF = 1;

		PORT_AWO.PMC0.UINT16 |= 0x0300;
		PORT_AWO.PFCE0.UINT16 |= 0x0300;
		PORT_AWO.PFC0.UINT16 |= 0x0300;
		PORT_AWO.PM0.UINT16 &= 0xFEFF;
		PORT_AWO.PM0.UINT16 |= 0x0200;
		/* CAN wakeup and enable  */


		while((RSCAN0.GSTS.UINT8[0] && 0x08)==0x08);		//wait until CAN RAM is initiallized
		RSCAN0.GCTR.UINT8[0] &= 0xFB;	//Transition to global reset mode from global stop mode
		RSCAN0.C2CTR.UINT8[0] &= 0xFB;	//Transition to channel reset mode from channel stop mode
#if 1

		RSCAN0.GCFG.UINT32 = 0x4010;	//xin clock selected w501
		RSCAN0.C2CFG.UINT32 = 0x00140001;	// 500kbps, 8Tq, 1,2,5, 75% sampling CAN2

#endif

		//--------- Receive Buffer settings -------------------------------------
		RSCAN0.RMNB.UINT8[0] = 0x01;		// no. of receive buffers	01
		//--------- Receive Rule settings for RX Buffer -------------------------------------
#if 1
		RSCAN0.GAFLCFG0.UINT8[1] = 0x1;	// No. of rules for channel 2	0x17
		RSCAN0.GAFLECTR.UINT8[1] = 0x01;	// Enable write to receive rule table

		RSCAN0.GAFLECTR.UINT8[0] = 0x00;	// receive rule for page no.0 configuration

		// receive rule 1
		RSCAN0.GAFLID0.UINT16[0] = MBFM1_100;	// Standard, Data frame, 11 bit ID
		//RSCAN0.GAFLM0.UINT32 = 0x00;		// ID bits are not compared compared
		RSCAN0.GAFLM0.UINT32 = 0xff;		// ID bits are compared compared
		RSCAN0.GAFLP0_0.UINT8[1] = 0x00;
		RSCAN0.GAFLP1_0.UINT32 = 0x0100;		// TX/RX FIFO 0 selected

		RSCAN0.GAFLECTR.UINT8[1] = 0x00;	// Disable write to receive rule table
#endif

#if 1
		/* Configure the CANout buffer  */
		RSCAN0.TMIEC1.UINT32 = 0x00000000;		// Transmit Buffer 0 interrupt dissable for 0 to 31 transmit buffer

		/* TX buffer 01 */
		//	RSCAN0.TMID0.UINT16[0] = 0x01C0 ;		// TX Transmit message, standard data frame,
		//	RSCAN0.TMPTR0.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer can1 16 */
		RSCAN0.TMID16.UINT16[0] = 0x01C0 ;		// TX Transmit message, standard data frame,
		RSCAN0.TMPTR16.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer 02 */
		//	RSCAN0.TMID1.UINT16[0] = 0x02C0 ;		// TX Transmit message, standard data frame,
		//	RSCAN0.TMPTR1.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer can1 17 */
		RSCAN0.TMID17.UINT16[0] = 0x02C0 ;		// TX Transmit message, standard data frame,
		RSCAN0.TMPTR17.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer 03 */
		//	RSCAN0.TMID2.UINT16[0] = 0x03C0 ;		// TX Transmit message, standard data frame,
		//	RSCAN0.TMPTR2.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer can1 18 */
		RSCAN0.TMID18.UINT16[0] = 0x03C0 ;		// TX Transmit message, standard data frame,
		RSCAN0.TMPTR18.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer 04 */
		//	RSCAN0.TMID04.UINT16[0] = 0x01C1 ;		// TX Transmit message, standard data frame,
		//	RSCAN0.TMPTR04.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer can1 19 */
		RSCAN0.TMID19.UINT16[0] = 0x01C1 ;		// TX Transmit message, standard data frame,
		RSCAN0.TMPTR19.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer 05 */
		//	RSCAN0.TMID4.UINT16[0] = 0x02C1 ;		// TX Transmit message, standard data frame,
		//	RSCAN0.TMPTR4.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer can1 20 */
		RSCAN0.TMID20.UINT16[0] = 0x02C1 ;		// TX Transmit message, standard data frame,
		RSCAN0.TMPTR20.UINT16[1] = 0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer 06 */
		//	RSCAN0.TMID5.UINT16[0] = 0x03C1 ;		// TX Transmit message, standard data frame,
		//	RSCAN0.TMPTR5.UINT16[1] =0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;				// 8 data bytes, label value as 0x12 historic buffer

		/* TX buffer can1 21 */
		RSCAN0.TMID21.UINT16[0] = 0x03C1 ;		// TX Transmit message, standard data frame,
		RSCAN0.TMPTR21.UINT16[1] =0x0012 + ((((uint16_t)(8))<<12)&0xF0000) ;
#endif
		//-----------Transmit/receive FIFO settings in Receive mode -----------------
#if 1
		RSCAN0.CFCC0.UINT8[2] = 0x00;		// Receive mode selected
		RSCAN0.CFCC0.UINT8[1] = 0x13;		// interrupt on every message transmitted, FIFO depth 16 messages
		//RSCAN0.CFCC0.UINT8[1] = 0x03;		// Interrupt on 1/8 message received in FIFO, Receive FIFO has 16 message depth
		RSCAN0.CFCC0.UINT8[0] = 0x03;		// transmit/receive FIFO receive interupt enabled
#endif
		//--------- Receive FIFO Buffer settings -------------------------------------
#if 1
		RSCAN0.RFCC0.UINT8[1] = 0x13;		// Interrupt on every message, Receive FIFO has 16 message depth
		//RSCAN0.RFCC0.UINT8[1] = 0x03;		// Interrupt on 1/8 message received in FIFO, Receive FIFO has 16 message depth
		RSCAN0.RFCC0.UINT8[0] = 0x03;		// Receive FIFO buffers used and interrupt enabled
#endif
		//----------- Global Interrupt -------------------------------------
		//RSCAN0.GCTR.UINT8[1] = 0x07;		// Transmit history interrupt, FIFO msg lost interrupt & DLC error interrupt enabled
		//RSCAN0.C0CTR.UINT16[1] = 0x0001;	// Trasmit abort interrupt enabled
		//RSCAN0.C0CTR.UINT8[1] = 0x01;		// Bus error interrupt enabled

		//------------ Interrupt setting -----------------------------------
		INTC2EITB69 = 1;//0;		// 0-Direct jump method, 1-vector method
		INTC2EIP69 = 1;				// level 1 priority

		INTC2EIRF69 = 0;		// clear interrupt flag
		INTC2EIMK69 = 0;		// enable error interrupt

		INTC2EITB70 = 1;//0;		// 0-Direct jump method, 1-vector method
		INTC2EIP70 = 1;				// level 1 priority

		INTC2EIRF70 = 0;		// clear interrupt flag
		INTC2EIMK70 = 0;		// enable error interrupt

		INTC2EITB71 = 1;//0;		// 0-Direct jump method, 1-vector method
		INTC2EIP71 = 1;				// level 1 priority

		INTC2EIRF71 = 0;		// clear interrupt flag
		INTC2EIMK71 = 0;		// enable error interrupt

		INTC2EITB72 = 1;//0;		// 0-Direct jump method, 1-vector method
		INTC2EIP72 = 1;				// level 1 priority

		INTC2EIRF72 = 0;		// clear interrupt flag
		INTC2EIMK72 = 0;		// enable error interrupt

		INTC2EITB73 = 1;//0;		// 0-Direct jump method, 1-vector method
		INTC2EIP73 = 1;				// level 1 priority

		INTC2EIRF73 = 0;		// clear interrupt flag
		INTC2EIMK73 = 1;		// enable error interrupt, 1 = dissable 0 = enable


		//------------ Operating mode --------------------------------------

		RSCAN0.GCTR.UINT8[0] = 0x00;		// Global operating mode
		for(i=0;i<0xfff;i++);	//wait for transistion

		RSCAN0.RFCC0.UINT8[0] |= 0x01;		// receive FIFO is used

		//RSCAN0.C0CTR.UINT8[0] = 0x00;	//channel communication mode w501
		//RSCAN0.C1CTR.UINT8[0] = 0x00;	//channel communication mode w501
		RSCAN0.C2CTR.UINT8[0] = 0x00;	//channel communication mode w501
		for(i=0;i<0xfff;i++);	//wait for transistion

		RSCAN0.CFCC0.UINT8[0] |= 0x01;		// transmit/receive FIFO is used

		/*if(RSCAN0.CFSTS0.UINT8[0] != 0x01)
		{
			Backlight_ON_IGNOFF = 0;
		}
		if(Backlight_ON_IGNOFF == 0)
		{
			IGNOFF_Dooropen_check_done = 1;
			IGNOFF_ExtLamp_check_done = 1;
		}
		else
		{
			//-------- TFT Backlight
			PORT_ISOPMC17_3 = 0;
			PORT_ISOPM17_3 = 0;
			PORT_ISOP17_3 = 1;
		}*/
		CAN_init_done = SET;//test
	}

	void can_IGNOFF_FIFO_rx(void)
	{
		if((RSCAN0.CFSTS0.UINT8[0] & 0x01) != 0x01)
		{					//logo_completed=1;
			do
			{
				rx_msg_ID = RSCAN0.CFID0.UINT16[0];
				rx_msg_DLC = (RSCAN0.CFPTR0.UINT8[3] & 0xf0) >> 4;

				switch(rx_msg_ID)
				{
				case MBFM1_100:
					IGNOFF_MBFM_CAN_ON = 1;
					MBFM_msgtimeout = 0;
					if(audio_init_done == 0)
					{
						SoundPCM_FIRST();
						audio_init_done = 1;
					}
					rx_msg_data0 = RSCAN0.CFDF0_0.UINT32;
					rx_msg_data1 = RSCAN0.CFDF1_0.UINT32;
					//my_speed_val = RSCAN0.CFDF0_0.UINT8[0];
					UDS_Buffer[0]= RSCAN0.CFDF0_0.UINT8[0];
					UDS_Buffer[1]= RSCAN0.CFDF0_0.UINT8[1];
					UDS_Buffer[2]= RSCAN0.CFDF0_0.UINT8[2];
					UDS_Buffer[3]= RSCAN0.CFDF0_0.UINT8[3];
					UDS_Buffer[4]= RSCAN0.CFDF1_0.UINT8[0];
					UDS_Buffer[5]= RSCAN0.CFDF1_0.UINT8[1];
					UDS_Buffer[6]= RSCAN0.CFDF1_0.UINT8[2];
					UDS_Buffer[7]= RSCAN0.CFDF1_0.UINT8[3];

					Key_In_Reminder(&UDS_Buffer[1]);
					IGN_OFF_CHECKLAMP_ALERT(&UDS_Buffer[1]);		/*Check Ext lamp*/

					IGN_OFF_DOOROPEN_ALERT(&UDS_Buffer[4]);			/*Door Open*/
					IGN_OFF_PARK_LAMP(&UDS_Buffer[0]);
//					Key_In_Reminder(&UDS_Buffer[1]);

					break;

//				default:
//					IGNOFF_MBFM_CAN_ON = 0;
//					display_active = 0;
//					IMMO_IGN_OFF=0;
//					DOOR_OPEN_IGN_OFF=0;
//					break;
				}
				RSCAN0.CFPCTR0.UINT8[0] = 0xFF;
				if(IGNOFF_MBFM_CAN_ON == 1) /*Logic added for from IGN ON to OFF CAN - MBFM timeout - 26-05-20 */
				{
				MBFM_msgtimeout++;
				}
				if(MBFM_msgtimeout > 100)
				{
					IGNOFF_MBFM_CAN_ON = 0;
					wait_state = 0;
					IGNOFF_display_timeout = 1; /*FOR alert IGN off alert exit state - 28-05-20*/
					audio_init_done = 0;
				}

			}while((RSCAN0.CFSTS0.UINT8[0] & 0x01) != 0x01);
//			handleDromeState();	/*function renaming*/
//			draw_grayscale();
		}
	/*	else
		{
			IGNOFF_MBFM_CAN_ON = 0;

		}*/
	}

void Message_Content_Failure_Verication(Content_Failure_t * Content_Status, uint8_t Rx_Msg_DLC)
{
    if(CAN_Bus_Off == CLEAR)
    {
	if(Content_Status->Failure_Status == CLEAR)
	{
	    if(Rx_Msg_DLC != 8)
	    {
		Content_Status->Failure_Counter++;

		if(Content_Status->Failure_Counter >= Message_Content_Failure_Threshold)
		{
		    Content_Status->Failure_Counter = 0;
		    Content_Status->Failure_Status = SET;
		    Content_Status->Recovery_Counter = 0;
		    Content_Status->Recovery_Status = CLEAR;
		}
	    }
	    else
	    {
		Content_Status->Failure_Counter = 0;
		Content_Status->Failure_Status = CLEAR;
	    }
	}
	else
	{
	    if(Rx_Msg_DLC == 8)
	    {
		Content_Status->Recovery_Counter++;

		if(Content_Status->Recovery_Counter >= Message_Content_Recovery_Threshold)
		{
		    Content_Status->Failure_Counter = 0;
		    Content_Status->Failure_Status = CLEAR;
		    Content_Status->Recovery_Counter = 0;
		    Content_Status->Recovery_Status = SET;
		}
	    }
	    else
	    {	    
		Content_Status->Recovery_Counter = 0;
		Content_Status->Recovery_Status = CLEAR;
	    }
	}
    }
}
#ifdef SPRINT4
/*TDMS*/
void Message_Content_Failure_Verication_SAS(Content_Failure_t * Content_Status, uint8_t Rx_Msg_DLC)
{
    if(CAN_Bus_Off == CLEAR)
    {
	if(Content_Status->Failure_Status == CLEAR)
	{
	    if(Rx_Msg_DLC != 5)
	    {
		Content_Status->Failure_Counter++;

		if(Content_Status->Failure_Counter >= Message_Content_Failure_Threshold)
		{
		    Content_Status->Failure_Counter = 0;
		    Content_Status->Failure_Status = SET;
		    Content_Status->Recovery_Counter = 0;
		    Content_Status->Recovery_Status = CLEAR;
		}
	    }
	    else
	    {
		Content_Status->Failure_Counter = 0;
		Content_Status->Failure_Status = CLEAR;
	    }
	}
	else
	{
	    if(Rx_Msg_DLC == 5)
	    {
		Content_Status->Recovery_Counter++;

		if(Content_Status->Recovery_Counter >= Message_Content_Recovery_Threshold)
		{
		    Content_Status->Failure_Counter = 0;
		    Content_Status->Failure_Status = CLEAR;
		    Content_Status->Recovery_Counter = 0;
		    Content_Status->Recovery_Status = SET;
		}
	    }
	    else
	    {	    
		Content_Status->Recovery_Counter = 0;
		Content_Status->Recovery_Status = CLEAR;
	    }
	}
    }
}
#endif
uint32_t Get_Signal_Value(uint8_t * Buffer, uint8_t Start_Bit, uint8_t Bits)
{
    uint32_t Value = 0, Byte_Mask = 0x000000FF;
    uint8_t Bit_Mask = 0x00, i = 0,j = 0, Bytes = 0, Remaining_Bits = 0, Start_Byte = 0, StartBit = 0;

	Start_Byte = (Start_Bit / 8);
    Bytes = (Bits / 8);
    Remaining_Bits = (Bits % 8);
	StartBit = (Start_Bit % 8);

    if(Bytes > 0)
    {
	for(i = 0; i < Bytes; i++)
	{
	    Value |= (uint32_t)(Buffer[Start_Byte + i] << (i*8));
	}
    }

    if(Remaining_Bits > 0)
    {
	for(j = StartBit; j < (StartBit + Remaining_Bits); j++)
	{
	    Bit_Mask |= (1 << j);
	}

	Value |= (uint32_t)((Buffer[Start_Byte + i] & Bit_Mask) << (i*8));
    }

	Value = (Value >> StartBit);

    return Value;
}

void Signal_Content_Failure_Verication(Content_Failure_t * Content_Status, uint32_t Signal_Value,
	uint32_t Min_Value, uint32_t Max_Value, uint32_t Invalid_Value)
{
    if(CAN_Bus_Off == CLEAR)
    {
	if(Content_Status->Failure_Status < 2)
	{
	    if((Signal_Value < Min_Value) || (Signal_Value > Max_Value) || (Signal_Value == Invalid_Value))
	    {
		Content_Status->Failure_Counter++;
		Content_Status->Failure_Status = SET;

		if(Content_Status->Failure_Counter >= Message_Content_Failure_Threshold)
		{
		    Content_Status->Failure_Counter = 0;
		    Content_Status->Failure_Status = 2;
		    Content_Status->Recovery_Counter = 0;
		    Content_Status->Recovery_Status = CLEAR;
		}
	    }
	    else
	    {
		Content_Status->Failure_Counter = 0;
		Content_Status->Failure_Status = CLEAR;
	    }
	}
	else
	{
	    if((Signal_Value >= Min_Value) && (Signal_Value <= Max_Value) && (Signal_Value != Invalid_Value))
	    {
		Content_Status->Recovery_Counter++;

		if(Content_Status->Recovery_Counter >= Message_Content_Recovery_Threshold)
		{
		    Content_Status->Failure_Counter = 0;
		    Content_Status->Failure_Status = CLEAR;
		    Content_Status->Recovery_Counter = 0;
		    Content_Status->Recovery_Status = SET;
		}
	    }
	    else
	    {	    
		Content_Status->Recovery_Counter = 0;
		Content_Status->Recovery_Status = CLEAR;
	    }
	}
    }
}
