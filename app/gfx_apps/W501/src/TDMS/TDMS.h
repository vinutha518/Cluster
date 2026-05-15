/*******************************************************************************
**   Private Macro Definitions                                                **
*******************************************************************************/

#define ON 1U
#define OFF 0U

#define ZERO 0
/* Used for CAN_TCU6_20 signal */
#define SEVEN 7
#define NINE 9

/* Use for CAN_SAS1_10 signal validity */
#define FAILURE_FACTOR 0.1
#define FACTOR 1U
#define CRC_MINIMUM 0U
#define CRC_MAXIMUM 15U
#define ABOLUTE_ANGLE_FACTOR 0.1
#define ABOLUTE_ANGLE_MINIMUM -780
#define ABOLUTE_ANGLE_MAXIMUM 779.9
#define TDMS_SLEW_20 20
#define TDMS_SLEW_50 50
#define TDMS_SLEW_500 500

/* Used for current gear number */
#define CURRENT_GEAR_MT_MINIMUM 0
#define CURRENT_GEAR_MT_MAXIMUM 15

/* For Clutch Status Received */
#define CLUTCH_STATUS_MINIMUM 0
#define CLUTCH_STATUS_MAXIMUM 7

/* For TGS_LEVER Status received */
#define TGS_LEVER_MINIMUM 0
#define TGS_LEVER_MAXIMUM 15

/* For Vehicle speed status received */
#define VEHICLE_SPEED_FACTOR 0.015625
#define VEHICLE_SPEED_MINIMUM 0
#define VEHICLE_SPEED_MAXIMUM 300
/* For testing purpose only */
#define NEUTRAL 9 /*Testing, it can be changed*/

#define MT 1
/* This macro used for Previous TDMS level condition */
#define NILL 6 /* This might be changed */

/* TDMS Levels Declaration */
#define LEFT_LEVEL_ONE 1
#define LEFT_LEVEL_TWO 2
#define LEFT_LEVEL_THREE 3
#define LEFT_LEVEL_FOUR 4
#define LEFT_LEVEL_FIVE 5
#define CENTER_LEVEL 0
/* For testing purpose only     */
#define RIGHT_LEVEL_ONE 6
#define RIGHT_LEVEL_TWO 7
#define RIGHT_LEVEL_THREE 8
#define RIGHT_LEVEL_FOUR 9
#define RIGHT_LEVEL_FIVE 10
#define INVALID_LEVEL 0xFF
/* used for HMI */
#define CAN_INVALID_VALUE       2U
#define CAN_MISSING_VALUE       1U
extern  void TDMS_Alert_Screen_Display(void);
/******************************************************************************
** Used HMI Variables                                                         **
******************************************************************************/
extern uint8_t             TDMS_Error_Code;        /* Used Failsafe condition  */
extern uint16_t            TDMS_Rotation_Angle;    /* Used for TDMS level */
extern uint16_t            TDMS_Rotation_Value;    /* Used for rotation angle  */
extern uint8_t             TDMS_Turn_Side;         /* used for Direction side */
extern uint8_t             TDMS_Alert_Zero_Degree_imageIndex;
extern uint8_t             TDMS_MainScreen_Zero_Degree_imageIndex;

extern uint8_t             TDMS_Left_Wheel_X;
extern uint8_t             TDMS_Left_Wheel_Y;
extern uint8_t             TDMS_Right_Wheel_X;
extern uint8_t             TDMS_Right_Wheel_Y;
extern uint8_t             TDMS_Left_Wheel_Alert_X;
extern uint8_t             TDMS_Left_Wheel_Alert_Y;
extern uint8_t             TDMS_Right_Wheel_Alert_X;
extern uint8_t             TDMS_Right_Wheel_Alert_Y;
/******************************************************************************
** Used For Timer                                                         **
******************************************************************************/
 extern uint16_t  TDMS_Alert_Screen_SlewTimer;    /* Used for Alert */  
 extern uint16_t  TDMS_Slew_Timer_5s;    /* Used for Alert */  
 /* Used for Main screen */
 extern uint16_t  TDMS_Main_Screen_SlewTimer;
 /* Conditional timer */
 extern uint16_t     Condition_Timer;
 extern _Bool        TDMS_alert_active;
 extern _Bool        TDMS_popup_shown;

/**********************************************************************************************************************
 *  Calibration Data                                                                                                 **
 *  Values read from diagnostics for calculation of TDMS level                                                       **
 *********************************************************************************************************************/
extern uint8_t Diag_RW_TDMS_Level1_Threshold;
extern uint8_t Diag_RW_TDMS_Level2_Threshold;
extern uint8_t Diag_RW_TDMS_Level3_Threshold;
extern uint8_t Diag_RW_TDMS_Level4_Threshold;
extern uint8_t Diag_RW_TDMS_Level5_Threshold;
/*******************************************************************************
**  GLOBAL FUNCTION PROTOTYPES                                                **
*******************************************************************************/

/*******************************************************************************
** Syntax           : void TDMS_Display_Meter( void )                         **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for actual condition check for    **
**                    TDMS Main Screen display & TDMS level updation.         **
**                    This function shall be called                           **
**                                                                            **
*******************************************************************************/
extern void TDMS_Display_Meter(void);
extern void TDMS_Main_Screen(void);

/*******************************************************************************
** Syntax           : void TDMS_Init( void )                                  **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for Initialize                    **
**                                                                            **
*******************************************************************************/
extern void TDMS_Init(void);

/* Output structure for TDMS */
typedef struct
{
    _Bool TDMS_Alert_Screen_Valid;               /* This flag is used to tell that Alert should be active or not */
    _Bool TDMS_Main_Screen_Valid;         /* This flag is used to tell that main screen should be active or not */
    uint8_t TDMS_Alert_Main_Screen_level; /* TDMS display level for both Alert and Main screen */
    _Bool TDMS_Invalid_Can_Msg;           /* If value is out of range, this flag is set */
    _Bool TDMS_Can_Missing;
} TDMS_Y;

extern TDMS_Y TdmsY;
