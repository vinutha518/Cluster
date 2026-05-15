#ifndef _MY_CAN_H
#define _MY_CAN_H

#define IC7_1000_TEST 0x22B
#define IC6_1000_TEST 0x223
#define IC4_1000_TEST 0x224
#define IC_NSM		  0x28B
#define IC10_200	  0x441

typedef struct
{
    uint16_t Failure_Counter;
    uint16_t Recovery_Counter;
    uint8_t Failure_Status;
    uint8_t Recovery_Status;
}Content_Failure_t;


void my_can0_init(void);
void my_can0_tx(unsigned char ,unsigned char* );
int my_can0_rx(void);
void can_IGNOFF_init(void);
void can_IGNOFF_FIFO_rx(void);
int my_can_process (unsigned char );
void my_can0x211_tx(unsigned char ,unsigned char*  );
void my_CAN0_test(void);
void my_can_pin_test(void);
unsigned char my_menu_change_SCREEN(void);
void my_can0_FIFO_rx(void);
void my_can0_tx_ICTEST(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data );
void my_can0_tx_IC4TEST(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data );
void my_can0_tx_IC6TEST(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data );
void my_can0_tx_IC_NSM(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data );
void my_can0_tx_IC10_200(uint16_t tx1_can_id,unsigned char tx_msg_DLC,unsigned char* tx1_can_msg_data );


extern unsigned char rx_flag;
extern unsigned char tx_done;

extern unsigned char my_angle_val;
#define TX_BUFFER 	1
#define TX_RX_FIFO	2

#define TX_MODE		TX_BUFFER

#define CLEAR 0
#define SET  1

extern uint64_t S3ECU_cntr,NBs_cntr;
//unsigned short S3ECU_cntr;
extern unsigned short P2CAN_ECU_Cntr;
extern unsigned char UDS_Buffer[250];
extern uint8_t TX_DATA[8];
extern unsigned short P2CAN_ECU_TIMEOUT;  /* 50ms/100us */
/****************** UDS *******************************/

void my_can0_FIFO_rx(void);
void my_can0_UDS_tx(void);
//extern unsigned char PROGRAMMING_FLAG;
extern unsigned char BOOT_FLAG_STATUS;
extern void tx_msgbuf_processing(unsigned char ,unsigned char* );
typedef struct
{
      unsigned char  Single_Frame:1;
      unsigned char  First_Frame:1;
      unsigned char  Cons_Frame:1;
      unsigned char  Flow_Control:1;
      unsigned char  Tx_4:1;
      unsigned char  Tx_5:1;
      unsigned char  Tx_6:1;
      unsigned char  Tx_7:1;
}Frame;

typedef union
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

}RX_DATA;
extern RX_DATA CAN_EMS1_10[8];
extern RX_DATA CAN_EMS36_10[8];
extern RX_DATA CAN_ESC2_10[8];
extern RX_DATA CAN_EMS3_10[8] ;
extern RX_DATA CAN_ESC5_10[8];
extern RX_DATA CAN_EMS8_10	[8];
extern RX_DATA CAN_EMS4_20	[8];
extern RX_DATA CAN_TCU5_10  [8];
extern RX_DATA CAN_MBFM5_100 [8];
extern RX_DATA CAN_EMS29_100 [8];
extern RX_DATA CAN_TCU6_20 [8];
extern RX_DATA CAN_ESC12_10 [8];
extern RX_DATA CAN_SRS1_20[8];
extern RX_DATA CAN_EMS6_500 [8];
extern RX_DATA CAN_EMS12_200[8];
extern RX_DATA CAN_PKE_ICU2_100[8];
extern RX_DATA CAN_MBFM1_100[8];
extern RX_DATA CAN_IS3_500[8];
extern RX_DATA CAN_IS1_100[8];
extern RX_DATA CAN_IC1_100[8];
extern RX_DATA CAN_IC2_100[8];
extern RX_DATA CAN_IC7_1000_TEST[8];
extern RX_DATA CAN_IC6_1000_TEST[8];
extern RX_DATA CAN_IC4_1000_TEST[8];
extern RX_DATA CAN_MBFM9_500[8];
extern RX_DATA CAN_RPAS1_20[8];
extern RX_DATA CAN_RPAS2_100[8];
extern RX_DATA CAN_EMS5_500[8];
extern RX_DATA CAN_MBFM_PAS1_50[8];
//#ifdef SPRINT4
extern RX_DATA CAN_IS16_1000[8];
extern RX_DATA CAN_ESC7_20[8];
/* TDMS tesing */
extern RX_DATA CAN_SAS1_10[8];
/* ECO_SCORE Testing */
extern RX_DATA CAN_IS4_500[8];
/* Audio Info Display */
extern RX_DATA CAN_IS14_200[8];
extern RX_DATA CAN_IS15_200[8];

extern RX_DATA CAN_SRS1_20_ext[8];
extern RX_DATA CAN_IC10_200[8];
//#endif
#endif

/* ************************************************************************** */
extern unsigned char Network_Message_Monitor_Start;
extern unsigned char EMS36_10_message_ON;
extern unsigned char ESC2_10_message_ON;
extern unsigned char EMS1_10_message_ON;
extern unsigned char EMS3_10_message_ON;
extern unsigned char ESC5_10_message_ON;
extern unsigned char EMS8_10_message_ON;
extern unsigned char EMS4_20_message_ON;
extern unsigned char TCU5_10_message_ON;
extern unsigned char MBFM5_100_message_ON;
extern unsigned char EMS29_100_message_ON;
extern unsigned char TCU6_20_message_ON;
extern unsigned char ESC12_10_message_ON;
extern unsigned char SRS1_20_message_ON;
extern unsigned char EMS6_500_message_ON;
extern unsigned char EMS12_200_message_ON;
extern unsigned char PKE_ICU2_100_message_ON;
extern unsigned char MBFM1_100_message_ON;
extern unsigned char MBFM9_500_message_ON;
extern unsigned char IS3_500_message_ON;
extern unsigned char IS1_100_message_ON;
extern unsigned char EMS_NSM_message_ON;
extern unsigned char TCU_NSM_message_ON;
extern unsigned char ESC_NSM_message_ON;
extern unsigned char IS_NSM_message_ON;
extern unsigned char IGNOFF_MBFM1_100_message_timeout_flag;
extern unsigned char RPAS1_20_message_ON;
extern unsigned char RPAS2_100_message_ON;
extern unsigned char EMS5_500_message_ON;
extern unsigned char MBFM_PAS1_50_message_ON;
//#ifdef SPRINT4
extern unsigned char IS16_1000_message_ON;
extern unsigned char ESC7_20_message_ON;
/*TDMS */
extern unsigned char SAS1_10_message_ON;
/* ECO_SCORE  */
extern unsigned char IS4_500_message_ON;
/* Audio Info Display */
extern unsigned char IS14_200_message_ON;
extern unsigned char IS15_200_message_ON;
//#endif
extern _Bool CAN_init_done;

extern uint16_t Network_Message_Monitor_Counter;
extern uint16_t IGNOFF_MBFM1_100_Message_Period_Counter;
extern uint16_t IGNOFF_CAN_counter;
/* ************************************************************************** */
/************msg_timeout variables***********************************/
extern uint16_t IGNOFF_MBFM1_100_Message_Recovery_Counter;
extern uint8_t IVN_ESC2_10_Msgtimeout_Cnt_u1g;
extern uint8_t IVN_ESC12_10_Msgtimeout_Cnt_u1g;

/* Message and Signal Contrnt Varibles */
extern uint16_t Message_Content_Failure_Threshold;
extern uint16_t Message_Content_Recovery_Threshold;
extern uint16_t Signal_Content_Failure_Threshold;
extern uint16_t Signal_Content_Recovery_Threshold;

extern Content_Failure_t EMS1_10_Message_Content;
extern Content_Failure_t EMS36_10_Message_Content;
extern Content_Failure_t EMS3_10_Message_Content;
extern Content_Failure_t EMS8_10_Message_Content;
extern Content_Failure_t EMS4_20_Message_Content;
extern Content_Failure_t EMS29_100_Message_Content;
extern Content_Failure_t EMS6_500_Message_Content;
extern Content_Failure_t EMS12_200_Message_Content;

extern Content_Failure_t ESC5_10_Message_Content;	//NA
extern Content_Failure_t ESC2_10_Message_Content;
extern Content_Failure_t ESC12_10_Message_Content;

extern Content_Failure_t TCU5_10_Message_Content;	//NA
extern Content_Failure_t TCU6_20_Message_Content;

extern Content_Failure_t MBFM1_100_Message_Content;	//NA
extern Content_Failure_t MBFM5_100_Message_Content;
extern Content_Failure_t MBFM9_500_Message_Content;
extern Content_Failure_t MBFM_PAS1_50_Message_Content;

extern Content_Failure_t SRS1_20_Message_Content;	//NA

extern Content_Failure_t PKE_ICU2_100_Message_Content;	//NA

extern Content_Failure_t RPAS1_20_Message_Content;	//NA
extern Content_Failure_t RPAS2_100_Message_Content;

extern Content_Failure_t IS1_100_Message_Content;	//NA
extern Content_Failure_t IS3_500_Message_Content;
//#ifdef SPRINT4
/*TDMS */
extern Content_Failure_t SAS1_10_Message_Content;
//#endif

/**************Signal_content_counter_variables*********************/
extern Content_Failure_t EMS36_10_ENG_TRQ_AFTER_RED1_Sig_Content;
extern Content_Failure_t EMS1_10_Temp_Sig_Content;
extern Content_Failure_t EMS1_10_RPM_Sig_Content;
extern Content_Failure_t EMS1_10_GEAR_Sig_Content;
extern Content_Failure_t EMS1_10_INDC_CRUISE_Sig_Content;
extern Content_Failure_t EMS3_10_CLUCH_Sig_Content;
extern Content_Failure_t EMS3_10_STS_ESS_INDC_Sig_Content;
extern Content_Failure_t EMS8_10_TARGET_GEAR_Sig_Content;
extern Content_Failure_t EMS4_20_ENG_DRIVE_MODE_Sig_Content;
extern Content_Failure_t TCU5_10_GEAR_ACTUAL_Sig_Content;
extern Content_Failure_t TCU5_10_GEAR_TARGET_Sig_Content;
extern Content_Failure_t TCU6_20_TGS_LEVER_Sig_Content;
extern Content_Failure_t TCU6_20_MODE_SIGNAL_Sig_Content;
extern Content_Failure_t TCU6_20_INDC_AT_MALFUNC_Sig_Content;
extern Content_Failure_t SRS1_20_INDC_SRSC_Sig_Content;
extern Content_Failure_t EMS6_500_DISP_AMBT_TEMP_Sig_Content;
extern Content_Failure_t MBFM1_100_MBFM_ENGOFF_Sig_Content;
extern Content_Failure_t RPAS1_20_DISP_DIST_Sig_Content;
extern Content_Failure_t ESC2_10_ABS_VEH_SPEED_Sig_Content;
extern Content_Failure_t ESC2_10_ABS_ODO_DIST_Sig_Content;

#ifdef SPRINT4
/* TDMS */
extern Content_Failure_t SAS1_10_ABOLUTE_ANGLE_Sig_Content;
#endif
/* ************************************************************************** */
extern uint8_t display_active;
extern uint8_t IGNOFF_MBFM_CAN_ON;
/* ************************************************************************** */
