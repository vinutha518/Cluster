/****************************************************************************
** Includes                                                                **
*****************************************************************************/
#include<stdlib.h>
#include "r_typedefs.h"
#include "init.h"
#include "UDSS.h"
#include "TDMS.h"
#include "my_can.h"
#include "batt.h"
#include "UDS_DTC.h"
#include "UDSS_APP.h"
#include "gui.h"
#include "alert.h"
#include "druserconf.h"
#include "drome.h"
#include "parallel_to_serial.h"
#include "swsc.h"

void TDMS_Alert_Screen_Display(void);
void TDMS_Main_Screen(void);

TDMS_Y TdmsY;
/************************************************************************************************************
** Local Variables                                                                                         **
************************************************************************************************************/
static  _Bool       TDMS_Alert_Config;              /* used for TDMS Alert configuration */
static  _Bool       TDMS_main_screen_config;        /* used for TDMS main screen configuration */
int16_t             TDMS_Abolute_Angle_Value;       /* This is the value read from CAN_SAS1_10 message */
uint8_t             Display_TDMS_Level = 0U;        /* This is used for main screen display level */
uint8_t             TDMS_Level = 0;                     /* This is used to update final TDMS level to TDMS display  */
_Bool               TDMS_Alert_Condition;           /* This is used to trigger alert condition to TDMS alert display*/ 
_Bool               Var_Coding_Electronic_Controlled_Power_Steering; /*This flag is used for Variant code check for power steering */
_Bool               EPS_TT_Functionality;  /* This  flag has been taken into consideration when var code of power steering is enabled */
_Bool               TDMS_alert_active = 0U;
_Bool               TDMS_popup_shown = 0;
//static  uint8_t     Condition_Timer = 0U;

/******************************************************************************
** Required Input                                                            **
******************************************************************************/
 extern uint16_t    final_vechile_speed;
 extern uint8_t     Gear_Configuration;     /* This flag used to check gear configuration as MT(Manual Transmission) or AT(Autonomous Transmission) */
 extern uint32_t    display_speedometer_speed;  /* Used for Vehicle speed */
/******************************************************************************
** Used HMI Variables                                                         **
******************************************************************************/
uint8_t             TDMS_Error_Code;        /* Used Failsafe condition  */
uint16_t            TDMS_Rotation_Angle;    /* Used for TDMS level */
uint16_t            TDMS_Rotation_Value;    /* Used for rotation angle  */
uint8_t             TDMS_Turn_Side;         /* used for Direction side */
uint8_t             TDMS_Alert_Zero_Degree_imageIndex;
uint8_t             TDMS_MainScreen_Zero_Degree_imageIndex;

uint8_t             TDMS_Left_Wheel_X = 78;	/*default for main screen*/
uint8_t             TDMS_Left_Wheel_Y = 162;
uint8_t             TDMS_Right_Wheel_X = 146;
uint8_t             TDMS_Right_Wheel_Y = 162;

uint8_t             TDMS_Left_Wheel_Alert_X = 77; /*default for alert*/
uint8_t             TDMS_Left_Wheel_Alert_Y = 160;
uint8_t             TDMS_Right_Wheel_Alert_X = 147;
uint8_t             TDMS_Right_Wheel_Alert_Y = 160;
/******************************************************************************
** Used For Timer                                                         **
******************************************************************************/
 uint16_t  TDMS_Alert_Screen_SlewTimer;    /* Used for Alert */  
 uint16_t  TDMS_Slew_Timer_5s;    /* Used for Alert */  
 /* Used for Main screen */
 uint16_t  TDMS_Main_Screen_SlewTimer;
 /* Conditional timer */
 uint16_t     Condition_Timer = 0U;
 static uint8_t TDMS_Alert_HMI_Level = 11; /*Prev. level hold*/


extern uint8_t dont_throw_alerts_alert_on_screen_present;
extern uint64_t start_alart_timeout_5sec;
/**********************************************************************************************************************
 *  Calibration Data                                                                                                 **
 *  Values read from diagnostics for calculation of TDMS level                                                       **
 *********************************************************************************************************************/
#if 0
uint8_t    Diag_RW_TDMS_Level1_Threshold = 30;
uint8_t    Diag_RW_TDMS_Level2_Threshold = 100;
uint8_t    Diag_RW_TDMS_Level3_Threshold = 100;
uint8_t    Diag_RW_TDMS_Level4_Threshold = 100;
uint8_t    Diag_RW_TDMS_Level5_Threshold = 100;
#endif
uint8_t    Diag_RW_TDMS_Level1_Threshold;
uint8_t    Diag_RW_TDMS_Level2_Threshold;
uint8_t    Diag_RW_TDMS_Level3_Threshold;
uint8_t    Diag_RW_TDMS_Level4_Threshold;
uint8_t    Diag_RW_TDMS_Level5_Threshold;
/**********************************************************************************************************************
 *  Function definitions
 *********************************************************************************************************************/
/*******************************************************************************
** Syntax           : void TDMS_Init( void )                                  **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used to Initialize output variabls     ** 
**                    after ignition is on                                    **
**                                                                            **
*******************************************************************************/
void TDMS_Init( void )
{
    TdmsY.TDMS_Alert_Screen_Valid = OFF;
    TdmsY.TDMS_Main_Screen_Valid = OFF;
    TdmsY.TDMS_Alert_Main_Screen_level = 0; 
    TdmsY.TDMS_Invalid_Can_Msg = FALSE;
    TdmsY.TDMS_Can_Missing = FALSE;
    /* Used for HMI Normal Condition */
     TDMS_Error_Code = FALSE;
}

/*******************************************************************************
** Syntax           : void TDMS_Config( void )                                **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for Feature and Var code check    **
**                    of TDMS Alert and TDMS display.                         **
**                                                                            **
**                                                                            **
*******************************************************************************/
void TDMS_Config( void )
{
    if((FEATURE_CODE_TDMS_DISPLAY == TRUE) && (FEATURE_CODE_TDMS_CONFIG == TRUE))
    {
        /* This FLAG indicates the TDMS Alert and Main-Screen Display condition */
        TDMS_Alert_Config = TRUE;
        TDMS_main_screen_config = TRUE;
    }
    else if((FEATURE_CODE_TDMS_DISPLAY == TRUE) && (FEATURE_CODE_TDMS_CONFIG == FALSE))
    {
        TDMS_Alert_Config = TRUE;
        TDMS_main_screen_config = FALSE;
    }
    else if((FEATURE_CODE_TDMS_DISPLAY == FALSE) && (FEATURE_CODE_TDMS_CONFIG == TRUE))
    {
        TDMS_Alert_Config = FALSE;
        TDMS_main_screen_config = TRUE;
    }
    else
    {
        TDMS_Alert_Config = FALSE;
        TDMS_main_screen_config = FALSE;
    }
}

/*******************************************************************************
** Syntax           : _Bool TDMS_SAS_Validity_Check( void )                   **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Returns the validity of SAS Message                     **
**                                                                            **
** Description      : This function is used for validation check of all       **
**                    signals received on CAN_SAS1_10 message as valid or     **
**                    invalid                                                 **
**                                                                            **
*******************************************************************************/
_Bool TDMS_SAS_Validity_Check( void )
{
    /* CAN message SAS1_10 signal parameter */
    uint8_t     STS_SAS_Failure_Value;
    uint8_t     STS_SAS_Calib_Value;
    uint8_t     STS_SAS_Trim_Value;
    uint8_t     SAS_Checksum_Value;
    /* validity flags */
    _Bool       SAS_Checksum_Valid;
    _Bool       Abolute_Angle_Valid;
    /*CAN SAS1_10 signal parsing*/
    STS_SAS_Failure_Value = ((uint8_t)((CAN_SAS1_10[3].byte) & 0x01) * FACTOR);   
    STS_SAS_Calib_Value = ((uint8_t)(((CAN_SAS1_10[3].byte) & 0x02) >> 1) * FACTOR);
    STS_SAS_Trim_Value = ((uint8_t)(((CAN_SAS1_10[3].byte) & 0x04) >> 2) * FACTOR);
    SAS_Checksum_Value = ((uint8_t)(((CAN_SAS1_10[4].byte) & 0xF0) >> 4) * FACTOR);
    TDMS_Abolute_Angle_Value = ((int16_t)((CAN_SAS1_10[1].byte << 8) | (CAN_SAS1_10[0].byte)) * ABOLUTE_ANGLE_FACTOR);
    /* Initial check for Power steering */
    Var_Coding_Electronic_Controlled_Power_Steering = 1;/*This used when Var code is disabled*/
    if(Var_Coding_Electronic_Controlled_Power_Steering == ON)
    {
        EPS_TT_Functionality = OFF; /*EPS_TT functionality to be checked, but it is assigned as 0 for testing */
    }
    /* Validation of Checksum values received from CAN_SAS1_10  */
    if((SAS_Checksum_Value <= CRC_MAXIMUM))
    {
        /* This FLAG indicates SAS Checksum values are valid or not */
        SAS_Checksum_Valid = TRUE;
    }
    else
    {
        SAS_Checksum_Valid = FALSE;
        TdmsY.TDMS_Invalid_Can_Msg = TRUE;
    }
    /* Validation check of Abolute angle value received from CAN_SAS1_10  */
    if((TDMS_Abolute_Angle_Value >= ABOLUTE_ANGLE_MINIMUM) && (TDMS_Abolute_Angle_Value <= ABOLUTE_ANGLE_MAXIMUM)/*&&(TDMS_Abolute_Angle_Value != 0)*/)
    {
        /* This FLAG indicates SAS ABOLUTE Angle value are valid or not */
        Abolute_Angle_Valid = TRUE;
    }
    else
    {
        Abolute_Angle_Valid = FALSE;					/*Invalid condition*/
        TdmsY.TDMS_Invalid_Can_Msg = TRUE;
    }

    if(Var_Coding_Electronic_Controlled_Power_Steering == ON)
    {
        /* Complete SAS signal Validity check to decide its valid or not with EPS_TT_Functionality*/
        if(((TDMS_main_screen_config == TRUE) || (TDMS_Alert_Config == TRUE)) && (SAS1_10_message_timeout_flag == FALSE) && (STS_SAS_Calib_Value == TRUE) && (STS_SAS_Failure_Value == TRUE)
            && (STS_SAS_Trim_Value == TRUE) && (EPS_TT_Functionality == OFF) && (SAS_Checksum_Valid == TRUE) && (Abolute_Angle_Valid == TRUE))
        {
            /* when all message are valid then update SAS validity state as valid */
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        /* Complete SAS signal Validity check to decide its valid or not  without EPS_TT_Functionality */
        if(((TDMS_main_screen_config == TRUE) || (TDMS_Alert_Config == TRUE)) && (SAS1_10_message_timeout_flag == FALSE) && (STS_SAS_Calib_Value == TRUE) && (STS_SAS_Failure_Value == TRUE)
            && (STS_SAS_Trim_Value == TRUE) && (SAS_Checksum_Valid == TRUE) && (Abolute_Angle_Valid == TRUE))
        {
            /* when all message are valid then update SAS validity state as valid */
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/*******************************************************************************
** Syntax           : int8_t TDMS_Calculate_Level( void )                    **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : Returns the TDMS level                                  **
**                                                                            **
** Description      : This function is used for calculation of TDMS Level.    **
**                                                                            **
**                                                                            **
*******************************************************************************/
uint8_t TDMS_Calculate_Level( void )
{
    /*Reading values from diagnostics*/
    Diag_RW_TDMS_Level1_Threshold = Diag_RW_Level1_Threshold_Angle;
    Diag_RW_TDMS_Level2_Threshold = Diag_RW_Level2_Threshold_Angle;
    Diag_RW_TDMS_Level3_Threshold = Diag_RW_Level3_Threshold_Angle;
    Diag_RW_TDMS_Level4_Threshold = Diag_RW_Level4_Threshold_Angle;
    Diag_RW_TDMS_Level5_Threshold = Diag_RW_Level5_Threshold_Angle;
    TDMS_Abolute_Angle_Value = ((int16_t)((CAN_SAS1_10[1].byte << 8) | (CAN_SAS1_10[0].byte)) * ABOLUTE_ANGLE_FACTOR);

    uint8_t TDMS_Current_Level = 0;/* Used to store current TDMS level */
    /* Checking for Inner Turn  */
    if((TDMS_Abolute_Angle_Value) > (   Diag_RW_TDMS_Level1_Threshold + 
                                        Diag_RW_TDMS_Level2_Threshold + 
                                        Diag_RW_TDMS_Level3_Threshold +
                                        Diag_RW_TDMS_Level4_Threshold + 
                                        Diag_RW_TDMS_Level5_Threshold))
    {
        TDMS_Current_Level = LEFT_LEVEL_FIVE; /* Set TDMS Left level5 */

        if(startkey == 0x22072)		/*TDMS alert*/
        {
        	 start_alart_timeout_5sec = TRUE; /*To restrict alert timeout*/
        	 
        }
    }
    else if(((  Diag_RW_TDMS_Level1_Threshold + 
                Diag_RW_TDMS_Level2_Threshold + 
                Diag_RW_TDMS_Level3_Threshold +
                Diag_RW_TDMS_Level4_Threshold + 
                Diag_RW_TDMS_Level5_Threshold)  >= TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value > (   Diag_RW_TDMS_Level1_Threshold +
                                                Diag_RW_TDMS_Level2_Threshold + 
                                                Diag_RW_TDMS_Level3_Threshold + 
                                                Diag_RW_TDMS_Level4_Threshold)))
    {
        TDMS_Current_Level = LEFT_LEVEL_FOUR; /* Set TDMS Left level4 */

        if(startkey == 0x22072)		/*TDMS alert*/
        {
        	 start_alart_timeout_5sec = TRUE; /*To restrict alert timeout*/
        	
        }
    }
    else if(((  Diag_RW_TDMS_Level1_Threshold + 
                Diag_RW_TDMS_Level2_Threshold + 
                Diag_RW_TDMS_Level3_Threshold +
                Diag_RW_TDMS_Level4_Threshold) >= TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value > (   Diag_RW_TDMS_Level1_Threshold +
                                                Diag_RW_TDMS_Level2_Threshold + 
                                                Diag_RW_TDMS_Level3_Threshold)))
    {
        TDMS_Current_Level = LEFT_LEVEL_THREE; /* Set TDMS Left level3 */
        if(startkey == 0x22072)		/*TDMS alert*/
        {
        	 start_alart_timeout_5sec = TRUE; /*To restrict alert timeout*/
        	 
        }
    }
    else if(((  Diag_RW_TDMS_Level1_Threshold + 
                Diag_RW_TDMS_Level2_Threshold + 
                Diag_RW_TDMS_Level3_Threshold) >= TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value > (   Diag_RW_TDMS_Level1_Threshold +
                                                Diag_RW_TDMS_Level2_Threshold)))
    {
        TDMS_Current_Level = LEFT_LEVEL_TWO; /* Set TDMS Left level2 */
    }
    else if(((  Diag_RW_TDMS_Level1_Threshold + 
                Diag_RW_TDMS_Level2_Threshold) >= TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value > (Diag_RW_TDMS_Level1_Threshold)))
    {
        TDMS_Current_Level = LEFT_LEVEL_ONE; /* Set TDMS Left level1 */
    }
/* Checking for Center level */
    else if(((Diag_RW_TDMS_Level1_Threshold) >= TDMS_Abolute_Angle_Value) && (TDMS_Abolute_Angle_Value >= (-Diag_RW_TDMS_Level1_Threshold)))
    {
        TDMS_Current_Level = CENTER_LEVEL; /* Set TDMS Center level */
    }
/* Checking for Outer Turn */
    else if(((  -Diag_RW_TDMS_Level1_Threshold) > TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value >= (  -Diag_RW_TDMS_Level1_Threshold - 
                                                Diag_RW_TDMS_Level2_Threshold)))
    {
        TDMS_Current_Level = RIGHT_LEVEL_ONE; /* Set TDMS Right level1 */
    }
    else if(((  -Diag_RW_TDMS_Level1_Threshold - 
                Diag_RW_TDMS_Level2_Threshold) > TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value >= (  -Diag_RW_TDMS_Level1_Threshold -
                                                Diag_RW_TDMS_Level2_Threshold - 
                                                Diag_RW_TDMS_Level3_Threshold)))
    {
        TDMS_Current_Level = RIGHT_LEVEL_TWO; /* Set TDMS Right level2 */
    }
    else if(((  -Diag_RW_TDMS_Level1_Threshold - 
                Diag_RW_TDMS_Level2_Threshold - 
                Diag_RW_TDMS_Level3_Threshold) > TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value >= (  -Diag_RW_TDMS_Level1_Threshold -
                                                Diag_RW_TDMS_Level2_Threshold - 
                                                Diag_RW_TDMS_Level3_Threshold - 
                                                Diag_RW_TDMS_Level4_Threshold)))
    {
        TDMS_Current_Level = RIGHT_LEVEL_THREE; /* Set TDMS Right level3 */
        if(startkey == 0x22072)		/*TDMS alert*/
             {

             	 start_alart_timeout_5sec = TRUE; /*To restrict alert timeout*/
             	
             }
    }
    else if(((  -Diag_RW_TDMS_Level1_Threshold - 
                Diag_RW_TDMS_Level2_Threshold - 
                Diag_RW_TDMS_Level3_Threshold - 
                Diag_RW_TDMS_Level4_Threshold) > TDMS_Abolute_Angle_Value) && 
                (TDMS_Abolute_Angle_Value >= (  -Diag_RW_TDMS_Level1_Threshold -
                                                Diag_RW_TDMS_Level2_Threshold - 
                                                Diag_RW_TDMS_Level3_Threshold - 
                                                Diag_RW_TDMS_Level4_Threshold - 
                                                Diag_RW_TDMS_Level4_Threshold)))
    {
        TDMS_Current_Level = RIGHT_LEVEL_FOUR; /* Set TDMS Right level4 */
        if(startkey == 0x22072)		/*TDMS alert*/
             {
             	 start_alart_timeout_5sec = TRUE; /*To restrict alert timeout*/
             	
             }
    }
    else if((   -Diag_RW_TDMS_Level1_Threshold - 
                Diag_RW_TDMS_Level2_Threshold - 
                Diag_RW_TDMS_Level3_Threshold - 
                Diag_RW_TDMS_Level4_Threshold - 
                Diag_RW_TDMS_Level5_Threshold) > (TDMS_Abolute_Angle_Value))
    {
    	TDMS_Current_Level = RIGHT_LEVEL_FIVE; /* Set TDMS Right level5 */
    	if(startkey == 0x22072)		/*TDMS alert*/
    	{
    		start_alart_timeout_5sec = TRUE; /*To restrict alert timeout*/
    		
    	}
    }
    else
    {
        TDMS_Current_Level = INVALID_LEVEL;  /* As we are checking all level calculation hence do nothing just return */
    }
    return TDMS_Current_Level;     
}

/*******************************************************************************
** Syntax           : void TDMS_Alert_Screen_Display( void )                  **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for displaying TDMS Level on      **
**                    Alert screen.                                           **
**                                                                            **
*******************************************************************************/
void TDMS_Alert_Screen_Display( void )
{
    //static uint8_t  TDMS_Alert_Screen_SlewTimer;
    static uint8_t  TDMS_Alert_Screen_Trigger_Level = 0U;
    //static uint8_t  TDMS_Slew_Timer_5s;
    static uint8_t /*TDMS_Alert_HMI_Level = 11,*/TDMS_alert_start = 0;/* used for 0,1,2 levels */
    TDMS_Level = TDMS_Calculate_Level();/*Get TDMS level for current Abolute angle received*/
    /*If trigger level is not equal to current tdms level during debounce time, reset the timer*/
    if( (TDMS_Alert_Screen_Trigger_Level != TDMS_Level) )
    {
        TDMS_Alert_Screen_Trigger_Level = TDMS_Level;
        TDMS_Alert_Screen_SlewTimer = 0U;
        TDMS_Slew_Timer_5s = 0U;/*If there is change in TDMS level then alert screen 5s timer should reset*/
    }
    /*200ms debounce timer for TDMS level change*/
    if(TDMS_Alert_Screen_SlewTimer >= TDMS_SLEW_20)
    {
        if(TDMS_Alert_Screen_Trigger_Level == TDMS_Level)
        {
            Condition_Timer = 0U;
            TDMS_alert_start = 1;
            Display_TDMS_Level = TDMS_Alert_Screen_Trigger_Level;
            TdmsY.TDMS_Alert_Main_Screen_level = Display_TDMS_Level; /* Final TDMS Level display */
            /* TDMS level
            left 5 = ,left 4 = ,left 3 = ,left 2 = ,left 1 = ,Center 0 = 360,Right 1 = ,Right 2 = ,Right 3 = ,Right 4 = ,Right 5 =  */
            switch (TdmsY.TDMS_Alert_Main_Screen_level)
            {
            case 0:
            	TDMS_Rotation_Angle = 360;	//364
            	TDMS_Rotation_Value = 0;
            	TDMS_Turn_Side = 0;

        		TDMS_Left_Wheel_X = 77;
        		TDMS_Left_Wheel_Y = 175;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 175;

            	TDMS_Left_Wheel_Alert_X = 77;
            	TDMS_Left_Wheel_Alert_Y = 160;
            	TDMS_Right_Wheel_Alert_X = 147;
            	TDMS_Right_Wheel_Alert_Y = 160;

            	/* Used for HMI alert mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = TRUE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = TRUE;
            	break;
            case 1:
            	TDMS_Rotation_Angle = 350;//347;
            	TDMS_Rotation_Value = 10;
            	TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 77;
        		TDMS_Left_Wheel_Y = 174;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 175;

            	TDMS_Left_Wheel_Alert_X = 77;
            	TDMS_Left_Wheel_Alert_Y = 159;
            	TDMS_Right_Wheel_Alert_X = 147;
            	TDMS_Right_Wheel_Alert_Y = 160;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 2:
            	TDMS_Rotation_Angle = 340;//337;
            	TDMS_Rotation_Value = 20;
            	TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 75;
        		TDMS_Left_Wheel_Y = 173;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

            	TDMS_Left_Wheel_Alert_X = 75;
            	TDMS_Left_Wheel_Alert_Y = 158;
            	TDMS_Right_Wheel_Alert_X = 147;
            	TDMS_Right_Wheel_Alert_Y = 160;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 3:
            	TDMS_Rotation_Angle = 330;//327;
            	TDMS_Rotation_Value = 30;
            	TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 75;
        		TDMS_Left_Wheel_Y = 172;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

            	TDMS_Left_Wheel_Alert_X = 75;
            	TDMS_Left_Wheel_Alert_Y = 158;
            	TDMS_Right_Wheel_Alert_X = 147;
            	TDMS_Right_Wheel_Alert_Y = 160;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 4:
            	TDMS_Rotation_Angle = 320;//317;
            	TDMS_Rotation_Value = 40;
            	TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 73;
        		TDMS_Left_Wheel_Y = 171;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

            	TDMS_Left_Wheel_Alert_X = 73;
            	TDMS_Left_Wheel_Alert_Y = 156;
            	TDMS_Right_Wheel_Alert_X = 148;
            	TDMS_Right_Wheel_Alert_Y = 159;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 5:
            	TDMS_Rotation_Angle = 310;//307;
            	TDMS_Rotation_Value = 50;
            	TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 73;
        		TDMS_Left_Wheel_Y = 171;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

            	TDMS_Left_Wheel_Alert_X = 73;
            	TDMS_Left_Wheel_Alert_Y = 156;
            	TDMS_Right_Wheel_Alert_X = 148;
            	TDMS_Right_Wheel_Alert_Y = 159;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            	/* Used for right level 6-10*/
            case 6:
            	TDMS_Rotation_Angle = 370;//373;
            	TDMS_Rotation_Value = 10;
            	TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 78;
        		TDMS_Left_Wheel_Y = 176;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

            	TDMS_Left_Wheel_Alert_X = 78;
            	TDMS_Left_Wheel_Alert_Y = 162;
            	TDMS_Right_Wheel_Alert_X = 146;
            	TDMS_Right_Wheel_Alert_Y = 162;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 7:
            	TDMS_Rotation_Angle = 380;//383;
            	TDMS_Rotation_Value = 20;
            	TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 78;
        		TDMS_Left_Wheel_Y = 178;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

            	TDMS_Left_Wheel_Alert_X = 78;
            	TDMS_Left_Wheel_Alert_Y = 162;
            	TDMS_Right_Wheel_Alert_X = 146;
            	TDMS_Right_Wheel_Alert_Y = 162;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 8:
            	TDMS_Rotation_Angle = 390;//393;
            	TDMS_Rotation_Value = 30;
            	TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 78;
        		TDMS_Left_Wheel_Y = 179;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

            	TDMS_Left_Wheel_Alert_X = 78;
            	TDMS_Left_Wheel_Alert_Y = 162;
            	TDMS_Right_Wheel_Alert_X = 146;
            	TDMS_Right_Wheel_Alert_Y = 161;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 9:
            	TDMS_Rotation_Angle = 400;//403;
            	TDMS_Rotation_Value = 40;
            	TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 76;
        		TDMS_Left_Wheel_Y = 179;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

            	TDMS_Left_Wheel_Alert_X = 76;
            	TDMS_Left_Wheel_Alert_Y = 164;
            	TDMS_Right_Wheel_Alert_X = 146;
            	TDMS_Right_Wheel_Alert_Y = 162;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
            	break;
            case 10:
            	TDMS_Rotation_Angle = 410;//413;
            	TDMS_Rotation_Value = 50;
            	TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 76;
        		TDMS_Left_Wheel_Y = 180;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

            	TDMS_Left_Wheel_Alert_X = 76;
//            	TDMS_Left_Wheel_Alert_Y = 165;
            	TDMS_Left_Wheel_Alert_Y = 168;
            	TDMS_Right_Wheel_Alert_X = 146;
            	TDMS_Right_Wheel_Alert_Y = 162;

            	/* Used for mainly Center level */
            	TDMS_Alert_Zero_Degree_imageIndex = FALSE;
            	/* Used for HMI MainScreen Center level Adjustment */
            	TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
                break;
            default:
                break;
            }
            TDMS_Alert_Screen_SlewTimer = 0U;
        }
        else
        {
        	TDMS_alert_start = 0;
        }
    }
    else
    {
    	TDMS_alert_start = 0;
    }
    /*5s timer for alert screen to display TDMS level 0,1,2*/
//    if( TDMS_Slew_Timer_5s >= TDMS_SLEW_500 )
    {
        if(((TDMS_Alert_HMI_Level != Display_TDMS_Level)&&(TDMS_alert_start == 1))||(TDMS_popup_skip == 1))
        {
        	if((TDMS_Alert_HMI_Level != LEFT_LEVEL_THREE)&&(TDMS_Alert_HMI_Level != LEFT_LEVEL_FOUR)&&(TDMS_Alert_HMI_Level != LEFT_LEVEL_FIVE)
        	&&(TDMS_Alert_HMI_Level != RIGHT_LEVEL_THREE)&&(TDMS_Alert_HMI_Level != RIGHT_LEVEL_FOUR)&&(TDMS_Alert_HMI_Level != RIGHT_LEVEL_FIVE))
        	{
        		TDMS_popup_shown = 0;
        	}
            if( Display_TDMS_Level == CENTER_LEVEL || Display_TDMS_Level == LEFT_LEVEL_ONE || Display_TDMS_Level == LEFT_LEVEL_TWO ||
                Display_TDMS_Level == RIGHT_LEVEL_ONE || Display_TDMS_Level == RIGHT_LEVEL_TWO )
            {
            /*If there is no change in TDMS level for 5 sec, then alert screen should be off*/
            TdmsY.TDMS_Alert_Screen_Valid = OFF;
            TdmsY.TDMS_Main_Screen_Valid = OFF;

          //  TDMS_Alert_HMI_Level = Display_TDMS_Level;
                if (((TDMS_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present == FALSE) /*&& (dont_throw_alerts_BS6_alert_on_screen_present == FALSE)*/&&(skip_flag == 0))||((TDMS_popup_shown == 0)&&(TDMS_popup_skip == 1)))
                {
                	Priority_flag = 1;
#ifdef SPRINT4
                    (void)drome_sendEventToDrome((U32)drome_InEvent_TDMS_alert_trigger);
#endif
                    dont_throw_alerts_alert_on_screen_present = TRUE;
                    TDMS_popup_shown = 1;
                    TDMS_alert_active = 0;
                    TDMS_popup_skip = 0;
                    TDMS_Alert_HMI_Level = Display_TDMS_Level;
                }                        
            }
            else
            {
                /*TDMS level is other than 0,1,2 then alert screen should be always kept on*/
                TdmsY.TDMS_Alert_Screen_Valid = ON;
                TdmsY.TDMS_Main_Screen_Valid = ON;
                if ((TDMS_popup_shown == 0)&&(dont_throw_alerts_alert_on_screen_present == FALSE) /*&& (dont_throw_alerts_BS6_alert_on_screen_present == FALSE)*/&&(skip_flag == 0))
                {
                	Priority_flag = 1;
#ifdef SPRINT4
                    (void)drome_sendEventToDrome((U32)drome_InEvent_TDMS_alert_trigger);
#endif
                    dont_throw_alerts_alert_on_screen_present = TRUE;
                    start_alart_timeout_5sec = TRUE;
                    TDMS_popup_shown = 1;
                    TDMS_alert_active = 0;
                    TDMS_popup_skip = 0;
//                    TDMS_Alert_HMI_Level = Display_TDMS_Level;
                }   
                TDMS_popup_shown = 1;
            }
            TDMS_Slew_Timer_5s = 0U;
            TDMS_Alert_HMI_Level = Display_TDMS_Level;
        }
        
    }
//    else
    {
        //TDMS_Alert_Screen_SlewTimer++;
        //TDMS_Slew_Timer_5s++;
    }
}

/*******************************************************************************
** Syntax           : void TDMS_Main_Screen( void )                           **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for displaying TDMS level on      **
**                    Main screen.                                            **
**                                                                            **
*******************************************************************************/
void TDMS_Main_Screen( void )
{
   // static uint8_t  TDMS_Main_Screen_SlewTimer;
    static uint8_t  TDMS_Main_Screen_Trigger_Level = 0U;

    TDMS_Level = TDMS_Calculate_Level();
    /*If trigger level is not equal to current tdms level during debounce time, reset the timer*/
    if( (TDMS_Main_Screen_Trigger_Level != TDMS_Level) )
    {
        TDMS_Main_Screen_Trigger_Level = TDMS_Level;
 //       TDMS_Main_Screen_SlewTimer = 0U;
    }
    /* 200ms debounce timer for TDMS level change*/
    else if(TDMS_Main_Screen_SlewTimer >= TDMS_SLEW_20)
    {
        if(TDMS_Main_Screen_Trigger_Level == TDMS_Level)
        {
        	Display_TDMS_Level = TDMS_Main_Screen_Trigger_Level;
        	TdmsY.TDMS_Alert_Main_Screen_level = Display_TDMS_Level; /* Final TDMS Level display */
        	/* TDMS level
            left 5 = ,left 4 = ,left 3 = ,left 2 = ,left 1 = ,Center 0 = 360,Right 1 = ,Right 2 = ,Right 3 = ,Right 4 = ,Right 5 =  */
        	switch (TdmsY.TDMS_Alert_Main_Screen_level)
        	{
        	case 0:
        		TDMS_Rotation_Angle = 360;//364;
        		TDMS_Rotation_Value = 0;
        		TDMS_Turn_Side = 0;

        		TDMS_Left_Wheel_X = 77;
        		TDMS_Left_Wheel_Y = 175;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 175;

        		TDMS_Left_Wheel_Alert_X = 77;
        		TDMS_Left_Wheel_Alert_Y = 160;
        		TDMS_Right_Wheel_Alert_X = 147;
        		TDMS_Right_Wheel_Alert_Y = 160;

        		/* Used for HMI Center level Adjustment */
        		TDMS_MainScreen_Zero_Degree_imageIndex = TRUE;
        		break;
        	case 1:
        		TDMS_Rotation_Angle = 350;//347;
        		TDMS_Rotation_Value = 10;
        		TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 77;
        		TDMS_Left_Wheel_Y = 174;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 175;

        		TDMS_Left_Wheel_Alert_X = 77;
        		TDMS_Left_Wheel_Alert_Y = 159;
        		TDMS_Right_Wheel_Alert_X = 147;
        		TDMS_Right_Wheel_Alert_Y = 160;

        		/* Used for HMI Center level Adjustment */
        		TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 2:
        		TDMS_Rotation_Angle = 340;//337;
        		TDMS_Rotation_Value = 20;
        		TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 75;
        		TDMS_Left_Wheel_Y = 173;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 175;

        		TDMS_Left_Wheel_Alert_X = 75;
        		TDMS_Left_Wheel_Alert_Y = 158;
        		TDMS_Right_Wheel_Alert_X = 147;
        		TDMS_Right_Wheel_Alert_Y = 160;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 3:
        		TDMS_Rotation_Angle = 330;//327;
        		TDMS_Rotation_Value = 30;
        		TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 75;
        		TDMS_Left_Wheel_Y = 172;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

        		TDMS_Left_Wheel_Alert_X = 75;
        		TDMS_Left_Wheel_Alert_Y = 158;
        		TDMS_Right_Wheel_Alert_X = 147;
        		TDMS_Right_Wheel_Alert_Y = 160;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 4:
        		TDMS_Rotation_Angle = 320;//317;
        		TDMS_Rotation_Value = 40;
        		TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 73;
        		TDMS_Left_Wheel_Y = 171;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

        		TDMS_Left_Wheel_Alert_X = 73;
        		TDMS_Left_Wheel_Alert_Y = 156;
        		TDMS_Right_Wheel_Alert_X = 148;
        		TDMS_Right_Wheel_Alert_Y = 159;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 5:
        		TDMS_Rotation_Angle = 310;//307;
        		TDMS_Rotation_Value = 50;
        		TDMS_Turn_Side = 1;

        		TDMS_Left_Wheel_X = 73;
        		TDMS_Left_Wheel_Y = 171;
        		TDMS_Right_Wheel_X = 147;
        		TDMS_Right_Wheel_Y = 174;

        		TDMS_Left_Wheel_Alert_X = 73;
        		TDMS_Left_Wheel_Alert_Y = 156;
        		TDMS_Right_Wheel_Alert_X = 148;
        		TDMS_Right_Wheel_Alert_Y = 159;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        		/* Used for right level 6-10*/
        	case 6:
        		TDMS_Rotation_Angle = 370;//373;
        		TDMS_Rotation_Value = 10;
        		TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 78;
        		TDMS_Left_Wheel_Y = 176;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

        		TDMS_Left_Wheel_Alert_X = 78;
        		TDMS_Left_Wheel_Alert_Y = 162;
        		TDMS_Right_Wheel_Alert_X = 146;
        		TDMS_Right_Wheel_Alert_Y = 162;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 7:
        		TDMS_Rotation_Angle = 380;//383;
        		TDMS_Rotation_Value = 20;
        		TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 78;
        		TDMS_Left_Wheel_Y = 178;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

        		TDMS_Left_Wheel_Alert_X = 78;
        		TDMS_Left_Wheel_Alert_Y = 162;
        		TDMS_Right_Wheel_Alert_X = 146;
        		TDMS_Right_Wheel_Alert_Y = 162;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 8:
        		TDMS_Rotation_Angle = 390;//393;
        		TDMS_Rotation_Value = 30;
        		TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 78;
        		TDMS_Left_Wheel_Y = 179;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

        		TDMS_Left_Wheel_Alert_X = 78;
        		TDMS_Left_Wheel_Alert_Y = 162;
        		TDMS_Right_Wheel_Alert_X = 146;
        		TDMS_Right_Wheel_Alert_Y = 161;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 9:
        		TDMS_Rotation_Angle = 400;//403;
        		TDMS_Rotation_Value = 40;
        		TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 76;
        		TDMS_Left_Wheel_Y = 179;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

        		TDMS_Left_Wheel_Alert_X = 76;
        		TDMS_Left_Wheel_Alert_Y = 164;
        		TDMS_Right_Wheel_Alert_X = 146;
        		TDMS_Right_Wheel_Alert_Y = 162;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	case 10:
        		TDMS_Rotation_Angle = 410;//413;
        		TDMS_Rotation_Value = 50;
        		TDMS_Turn_Side = 2;

        		TDMS_Left_Wheel_X = 76;
        		TDMS_Left_Wheel_Y = 180;
        		TDMS_Right_Wheel_X = 146;
        		TDMS_Right_Wheel_Y = 176;

        		TDMS_Left_Wheel_Alert_X = 76;
//        		TDMS_Left_Wheel_Alert_Y = 165;
        		TDMS_Left_Wheel_Alert_Y = 168;
        		TDMS_Right_Wheel_Alert_X = 146;
        		TDMS_Right_Wheel_Alert_Y = 162;

        		/* Used for HMI Center level Adjustment */
				TDMS_MainScreen_Zero_Degree_imageIndex = FALSE;
        		break;
        	default:
        		break;
        	}
        	TDMS_Main_Screen_SlewTimer = 0U;
        }
    }
    else
    {
    	//TDMS_Main_Screen_SlewTimer++;
    }
}

/*******************************************************************************
 ** Syntax           : _Bool TDMS_Alert_Condition_Check( void )                **
 **                                                                            **
 ** Parameters(in)   : None                                                    **
 **                                                                            **
 ** Parameters (out) : None                                                    **
 **                                                                            **
 ** Return value     : Returns the TDMS alert condition for MT/AT variant      **
 **                                                                            **
 ** Description      : This function is used to check TDMS alert and TDMS      **
 **                    Display conditions.                                     **
 **                                                                            **
 *******************************************************************************/
_Bool TDMS_Alert_Condition_Check( void )
{
	uint8_t     Current_Gear_MT_Value; /* For CAN_EMS1_10 signal */
	uint8_t     Clutch_Status_Value;/* For CAN_EMS3_10 signal */
	uint8_t     TGS_LEVER_Value;/* For CAN_TCU6_20 signal */

	uint8_t       TDMS_Gear_State;  /* This is used for TDMS gear state */

	/*CAN signal parsing*/
	/* For CAN_EMS1_10 signal */
	Current_Gear_MT_Value       = ((uint8_t)((CAN_EMS1_10[5].byte) & 0x0F) * FACTOR);   /* Storing values from CAN_EMS1_10 signal for current gear checking */

	/* For CAN_EMS3_10 signal */
	Clutch_Status_Value         = ((uint8_t)(((CAN_EMS3_10[3].byte) & 0xE0) >> 5) * FACTOR);   /* Storing values from CAN_EMS3_10 signal for Clutch status checking */

	/* For CAN_TCU6_20 signal */
	TGS_LEVER_Value             = ((uint8_t)((CAN_TCU6_20[2].byte) & 0x0F) * FACTOR);   /* Storing values from CAN_TCU6_20 signal for TGS_LEVER checking */
#if 0
	/* For CAN_ESC2_10 signal */
	Vehicle_Speed_Indication_Value = (uint16_t)((((CAN_ESC2_10[1].byte) << 8) | (CAN_ESC2_10[0].byte)) * VEHICLE_SPEED_FACTOR);   /* Storing values from CAN_ESC2_10 signal for Vehicle speed indication checking */
#endif
	/*Checking for TDMS Alert display condition for MT(Manual Transmission) Variant*/
	/* Validity check of current gear status received from CAN_EMS1_10 */
//	if((Current_Gear_MT_Value < CURRENT_GEAR_MT_MAXIMUM))
//	{
		/* TDMS Gear State Condition check  */
		if(Gear_Configuration == 2)
		{
			if((Current_Gear_MT_Value < CURRENT_GEAR_MT_MAXIMUM))
			{
				if(/*(Gear_Configuration == 2) && */(Current_Gear_MT_Value == 0))
				{
					/* This indicates TDMS Gear state */
					TDMS_Gear_State = NEUTRAL;/* For testing Purpose we are consider N(NEUTRAL) = 9 */

				}
				else
				{
					TDMS_Gear_State = OFF;
				}

				if((display_speedometer_speed <= VEHICLE_SPEED_MAXIMUM))
				{
					/* Validity check of Clutch status received from CAN_EMS3_10 */
					if((Clutch_Status_Value < CLUTCH_STATUS_MAXIMUM)&&(EMS3_10_message_timeout_flag == FALSE))
					{
						/* Validity check of vehicle speed received from CAN_ESC2_10 */
						//					if((display_speedometer_speed <= VEHICLE_SPEED_MAXIMUM))
						//					{
						/* TDMS Alert Display condition checking  */
						if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == FALSE))
						{
							TDMS_Alert_Condition = ON;
						}
						else if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == TRUE) && (TDMS_Gear_State == NEUTRAL) && (Clutch_Status_Value == ZERO))
						{
							TDMS_Alert_Condition = ON;
						}
						else
						{
							TDMS_Alert_Condition = OFF;
							TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
						}
					}
					else if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == FALSE))
					{
						TDMS_Alert_Condition = ON;
					}
					else
					{
						TDMS_Alert_Condition = OFF;
						TdmsY.TDMS_Main_Screen_Valid = OFF;
						TdmsY.TDMS_Alert_Screen_Valid = OFF;
						TdmsY.TDMS_Invalid_Can_Msg = TRUE;
						TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
					}
				}
				else
				{
					TDMS_Alert_Condition = OFF;
					TdmsY.TDMS_Alert_Screen_Valid = OFF;
					TdmsY.TDMS_Main_Screen_Valid = OFF;
					TdmsY.TDMS_Invalid_Can_Msg = TRUE;
					TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
				}
			}
			else
			{
				TDMS_Alert_Condition = OFF;
				TdmsY.TDMS_Alert_Screen_Valid = OFF;
				TdmsY.TDMS_Main_Screen_Valid = OFF;
				TdmsY.TDMS_Invalid_Can_Msg = TRUE;
				TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
			}
		}
#if 0
		else
		{
			TDMS_Alert_Condition = OFF;
			TdmsY.TDMS_Invalid_Can_Msg = TRUE;
			TdmsY.TDMS_Alert_Screen_Valid = OFF;
			TdmsY.TDMS_Main_Screen_Valid = OFF;
			TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
		}
#endif

#if 0
		/* Validity check of Clutch status received from CAN_EMS3_10 */
		if((Clutch_Status_Value < CLUTCH_STATUS_MAXIMUM))
		{
			/* Validity check of vehicle speed received from CAN_ESC2_10 */
			if((display_speedometer_speed <= VEHICLE_SPEED_MAXIMUM))
			{
				/* TDMS Alert Display condition checking  */
				if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == FALSE))
				{
					TDMS_Alert_Condition = ON;
				}
				else if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == TRUE) && (TDMS_Gear_State == NEUTRAL) && (Clutch_Status_Value == ZERO))
				{
					TDMS_Alert_Condition = ON;
				}
				else
				{
					TDMS_Alert_Condition = OFF;
					TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
				}
			}
			else
			{
				TDMS_Alert_Condition = OFF;
				TdmsY.TDMS_Main_Screen_Valid = OFF;
				TdmsY.TDMS_Alert_Screen_Valid = OFF;
				TdmsY.TDMS_Invalid_Can_Msg = TRUE;
				TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
			}
		}
		else
		{
			TDMS_Alert_Condition = OFF;
			TdmsY.TDMS_Alert_Screen_Valid = OFF;
			TdmsY.TDMS_Main_Screen_Valid = OFF;
			TdmsY.TDMS_Invalid_Can_Msg = TRUE;
			TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
		}
#endif
		/* Checking for TDMS Alert display condition for AT(Autonomous Transmission) Variant */
		/* Validity check of TGS_LEVER status received from CAN_TCU6_20 */
		//	if(TGS_LEVER_Value <= TGS_LEVER_MAXIMUM)
		//	{
		else if(Gear_Configuration == 3)
		{
			/* Validity check of vehicle speed received from CAN_ESC2_10 */
			if((display_speedometer_speed <= VEHICLE_SPEED_MAXIMUM))
			{
				if((TGS_LEVER_Value <= TGS_LEVER_MAXIMUM)&&(TCU6_20_message_timeout_flag == FALSE))
				{
					/* Validity check of vehicle speed received from CAN_ESC2_10 */
					//				if((display_speedometer_speed <= VEHICLE_SPEED_MAXIMUM))
					//				{
					/* TDMS Alert Display condition checking  */
					if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == FALSE))
					{
						TDMS_Alert_Condition = ON;
					}
					else if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == TRUE) && (TGS_LEVER_Value == SEVEN))
					{
						TDMS_Alert_Condition = ON;
					}
					else if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == TRUE) && (TGS_LEVER_Value == NINE))
					{
						TDMS_Alert_Condition = ON;
					}
					else
					{
						TDMS_Alert_Condition = OFF;
						TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
					}
				}
				else if((display_speedometer_speed == ZERO) && (PARK_BRAKE_CPU == FALSE))
				{
					TDMS_Alert_Condition = ON;
				}
				else
				{
					TDMS_Alert_Condition = OFF;
					TdmsY.TDMS_Main_Screen_Valid = OFF;
					TdmsY.TDMS_Alert_Screen_Valid = OFF;
					TdmsY.TDMS_Invalid_Can_Msg = TRUE;
					TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
				}
			}
			else
			{
				/* When Gear Configuration is not AT varaint then No Alert */
				TDMS_Alert_Condition = OFF;
				TdmsY.TDMS_Main_Screen_Valid = OFF;
				TdmsY.TDMS_Alert_Screen_Valid = OFF;
				TdmsY.TDMS_Invalid_Can_Msg = TRUE;
				TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
				//				if(Gear_Configuration != 2)
				//				{
				//					TDMS_Alert_Condition = OFF;
				//					TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
				//				}
			}

		}
		else
		{
			TDMS_Alert_Condition = OFF;
			TdmsY.TDMS_Alert_Screen_Valid = OFF;
			TdmsY.TDMS_Main_Screen_Valid = OFF;
			TdmsY.TDMS_Invalid_Can_Msg = TRUE;
			TDMS_Alert_HMI_Level = 11;	/*Reset level to retriggered @ active condition*/
		}

		/* As depending on variant used i.e AT or MT This condition will change accordingly */
		return TDMS_Alert_Condition;
}

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
void TDMS_Display_Meter( void )
{
	/* Initialization of TDMS Configuration */
	TDMS_Config();
	if(IGN_ON_flag == ON)
	{
		if((LVSD_flag == FALSE) && (HVSD_flag == FALSE))
		{
			if(TDMS_Alert_Config == TRUE) /* for Alert config check */
			{
				/* HMI Normal Condition */
				TDMS_Error_Code = FALSE;

				if((SAS1_10_message_timeout_flag == FALSE)&&((ESC2_10_message_timeout_flag == FALSE)||(ESC12_10_message_timeout_flag == FALSE)))
				{
					if(((EMS1_10_message_timeout_flag == FALSE)&&(EMS3_10_message_timeout_flag == FALSE))
							||(TCU6_20_message_timeout_flag == FALSE)||(PARK_BRAKE_CPU == FALSE))
						//				if((SAS1_10_message_timeout_flag == FALSE) && (EMS1_10_message_timeout_flag == FALSE) && (EMS3_10_message_timeout_flag == FALSE) &&
						//						(ESC2_10_message_timeout_flag == FALSE) && (TCU6_20_message_timeout_flag == FALSE))
					{
						if(TDMS_SAS_Validity_Check() == TRUE)
						{
							TdmsY.TDMS_Alert_Screen_Valid = ON;
							TdmsY.TDMS_Invalid_Can_Msg = FALSE;
							TdmsY.TDMS_Can_Missing = FALSE;
							TDMS_Alert_Condition = TDMS_Alert_Condition_Check();/* Alert condition check */
							if(TDMS_Alert_Condition == TRUE )
							{
								TdmsY.TDMS_Invalid_Can_Msg = FALSE;
								TdmsY.TDMS_Main_Screen_Valid = OFF;
								TDMS_Alert_Condition = ON;
								Condition_Timer++;
								/* 500ms Debounce Timer for all the alert conditions to satisfy */
								if(Condition_Timer >= TDMS_SLEW_50)
								{
									//Condition_Timer = 0U;
									if(TDMS_Alert_Condition == TRUE)
									{
										//                                    TDMS_Alert_Screen_Display();
										TDMS_alert_active = 1;
									}
									else
									{
										TDMS_alert_active = 0;
										/*Do nothing*/
									}
								}
								else
								{
									/*Do nothing*/
								}
							}
							else
							{
								TDMS_alert_active = 0;
								Condition_Timer = 0U;
								if(/*(TDMS_popup_shown == 1)&&*/(startkey == 0x22072))	/*Alert Disable*/
								{
									start_alart_timeout_5sec=0;
									Priority_flag = 0;
									dont_throw_alerts_alert_on_screen_present=0;
									(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
								}
								TDMS_popup_shown = 0;
								if(TDMS_popup_skip == 1)
								{
									TDMS_popup_skip = 0;
								}
								TdmsY.TDMS_Invalid_Can_Msg = TRUE;
							}
						}
						else
						{
							if(/*(TDMS_popup_shown == 1)&&*/(startkey == 0x22072))	/*Alert Disable*/
							{
								start_alart_timeout_5sec=0;
								Priority_flag = 0;
								dont_throw_alerts_alert_on_screen_present=0;
								(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
							}
							TDMS_popup_shown = 0;
							if(TDMS_popup_skip == 1)
							{
								TDMS_popup_skip = 0;
							}
							TDMS_alert_active = 0;
							TdmsY.TDMS_Alert_Screen_Valid = OFF;
							TdmsY.TDMS_Main_Screen_Valid = OFF;
							TdmsY.TDMS_Invalid_Can_Msg = TRUE;
							TdmsY.TDMS_Can_Missing = FALSE;
							/* CAN Invalid */
							TDMS_Error_Code = CAN_INVALID_VALUE;
						}
					}
					//
					else
					{
						if(/*(TDMS_popup_shown == 1)&&*/(startkey == 0x22072))	/*Alert Disable*/
						{
							start_alart_timeout_5sec=0;
							Priority_flag = 0;
							dont_throw_alerts_alert_on_screen_present=0;
							(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
						}
						TDMS_popup_shown = 0;
						if(TDMS_popup_skip == 1)
						{
							TDMS_popup_skip = 0;
						}
						TDMS_alert_active = 0;
						TdmsY.TDMS_Alert_Screen_Valid = OFF;
						TdmsY.TDMS_Main_Screen_Valid = OFF;
						TdmsY.TDMS_Invalid_Can_Msg = TRUE;
						TdmsY.TDMS_Can_Missing = TRUE;
						TDMS_Error_Code = CAN_MISSING_VALUE; /* Communication error */
					}
				}
				else
				{
					if(/*(TDMS_popup_shown == 1)&&*/(startkey == 0x22072))	/*Alert Disable*/
					{
						start_alart_timeout_5sec=0;
						Priority_flag = 0;
						dont_throw_alerts_alert_on_screen_present=0;
						(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
					}
					TDMS_popup_shown = 0;
					if(TDMS_popup_skip == 1)
					{
						TDMS_popup_skip = 0;
					}
					TDMS_alert_active = 0;
					TdmsY.TDMS_Alert_Screen_Valid = OFF;
					TdmsY.TDMS_Main_Screen_Valid = OFF;
					TdmsY.TDMS_Invalid_Can_Msg = TRUE;
					TdmsY.TDMS_Can_Missing = TRUE;
					TDMS_Error_Code = CAN_MISSING_VALUE; /* Communication error */
				}
			}
			else
			{
				TDMS_alert_active = 0;
				Condition_Timer = 0U;
				TdmsY.TDMS_Alert_Screen_Valid = OFF;
			}
			if (TDMS_main_screen_config == TRUE)
			{
				/* HMI Normal Condition */
				TDMS_Error_Code = FALSE;
				if(SAS1_10_message_timeout_flag == FALSE)
				{
					if(TDMS_SAS_Validity_Check() == TRUE)
					{
						TdmsY.TDMS_Main_Screen_Valid = ON;
						if(TdmsY.TDMS_Main_Screen_Valid == TRUE)
						{
							TdmsY.TDMS_Invalid_Can_Msg = FALSE;
							TdmsY.TDMS_Can_Missing = FALSE;
							if(TDMS_Alert_Condition == FALSE)
							{
								/* TDMS Alert Screen display should be inactivated  */
								TdmsY.TDMS_Main_Screen_Valid = ON;
								TDMS_Main_Screen();
							}
							else
							{
								TdmsY.TDMS_Main_Screen_Valid = OFF;
							}
						}
						else
						{
							/*Code*/
						}
					}
                    else
                    {
                        TdmsY.TDMS_Main_Screen_Valid = OFF;
                        TdmsY.TDMS_Invalid_Can_Msg = TRUE;
                        /* CAN Invalid */
                        TDMS_MainScreen_Zero_Degree_imageIndex = FALSE; /*Default / Invalid condition -> zero degree image index disable*/

                        TDMS_Error_Code = CAN_INVALID_VALUE;
                    }   
                }
                else
                {
                    TdmsY.TDMS_Can_Missing = TRUE;
                    TdmsY.TDMS_Invalid_Can_Msg = TRUE;
                    /* Communication error */
                    TDMS_Error_Code = CAN_MISSING_VALUE; 
                }    
            }
            else
            {
                TdmsY.TDMS_Main_Screen_Valid = OFF;
            }    
        }
        else
        {
            /* TDMS Main/Alert Screen display should be inactivated  */
            TdmsY.TDMS_Main_Screen_Valid = OFF;
            TdmsY.TDMS_Alert_Screen_Valid = OFF;
            TDMS_alert_active = 0;
        }
    }
    else
    {
        /* TDMS Main/Alert Screen display should be inactivated  */
        TdmsY.TDMS_Main_Screen_Valid = OFF;
        TdmsY.TDMS_Alert_Screen_Valid = OFF;
        TDMS_alert_active = 0;
    }
}
