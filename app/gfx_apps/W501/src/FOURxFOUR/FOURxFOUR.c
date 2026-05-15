/****************************************************************************
** Includes                                                                **
*****************************************************************************/
#include "r_typedefs.h"
#include "my_can.h"
#include "init.h"
#include "batt.h"
#include "FOURxFOUR.h"
#include "UDSS.h"
#include "UDSS_APP.h"
#include "UDS_DTC.h"
#include "swsc.h"
#include "gui.h"
#include "alert.h"
#include "druserconf.h"
#include "drome.h"

/******************************************************************************
** Local Variables                                                           **
******************************************************************************/

FourxFourY FxFY;

_Bool       FourxFour_Main_Screen_4WD;
_Bool       FourxFour_Engage_DisEngage_Alert_4WD;           
uint8_t     FourxFour_Main_Screen_Transfer_Mode = 0;        /** Used to get the Transfer mode from can message**/
uint8_t     FourxFour_Alert_Condition_4WD;                  /** Used for output condition for 4WD Alert **/
_Bool       FourxFour_Main_Screen_Display_Mode;             /** Used for output Display condition for 4WD Main screen display condition  **/
_Bool       FourxFour_Main_Screen_Display_4WD_Valid;        /** Used for Validity check of received CAN signal for Transfer mode **/
_Bool       FourxFour_Alert_Condition_4WD_Valid;            /** Used for Validity check of received CAN signal for transfer mode **/

_Bool       Feature_Code_Display_4WD;                       /** This is used for Main-Screen condition check **/
_Bool       Feature_Code_Transfer_Mode_Config;              /*If feature code is not set, use this*/
/******************************************************************************
** Calibration data                                                           **
******************************************************************************/
uint8_t      		Previous_Alert_Condition_Transfer_Mode;      /* Used to store previous transition mode value */
/*******************************************************************************/
extern uint8_t dont_throw_alerts_alert_on_screen_present;
extern uint64_t start_alart_timeout_5sec;
uint8_t var_4x4_frameRate = 0;
uint8_t var_4x4_2WD_visible = 0;
uint8_t var_4x4_4WD_visible = 0;
uint8_t var_4x4_4WD_animation = 0;

_Bool Alert_4WL_shown = 0;
_Bool Alert_4WH_shown = 0;
_Bool Mscreen_4WH_shown = 0;
_Bool Mscreen_4WL_shown = 0;

uint8_t VI_4WD_Framerate = 0;
uint8_t VI_2WD_Visiblity = 0;
uint8_t VI_4WD_Visiblity = 0;
uint8_t cnt4 = 0;
uint8_t cnt3 = 0;
_Bool var_2wd_animation_start = 0;

/*******************************************************************************
** Syntax           : void FourxFour_Init (void)                              **
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
void FourxFour_Init( void )
{
    FxFY.FourxFour_Alert_Condition_4WD = 0U;
    FxFY.FourxFour_Alert_Condition_4WD_Valid = FALSE; 
    FxFY.Invalid_FourxFour_Msg = TRUE;
    FxFY.FourxFour_Main_Screen_Display_4WD_Valid = FALSE; 
    FxFY.FourxFour_Main_Screen_Display_Mode = 0U;
}

/*******************************************************************************
** Syntax           : void FourxFour_Alert_Main_Screen_Display_4WD_Config(void) **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for initialization                **
**                                                                            **
*******************************************************************************/
void FourxFour_Alert_Main_Screen_Display_4WD_Config( void )
{
    if((FEATURE_CODE_TRANSFER_MODE_CONFIG == TRUE) && (Diag_RW_4WD_Change_Animation_Enable == TRUE) && (FEATURE_CODE_DISPLAY_4WD == TRUE))
    {
        /** This FLAG indicates the Alert and Main-Screen Display condition **/
        FourxFour_Engage_DisEngage_Alert_4WD = TRUE;
        FourxFour_Main_Screen_4WD = TRUE;
    }
    else if((FEATURE_CODE_TRANSFER_MODE_CONFIG == TRUE) && (Diag_RW_4WD_Change_Animation_Enable == FALSE) && (FEATURE_CODE_DISPLAY_4WD == TRUE))
    {
        /** This FLAG indicates the Alert and Main-Screen Display condition **/
        FourxFour_Engage_DisEngage_Alert_4WD = FALSE;
        FourxFour_Main_Screen_4WD = TRUE;
    }
    else if((FEATURE_CODE_TRANSFER_MODE_CONFIG == TRUE) && (Diag_RW_4WD_Change_Animation_Enable == TRUE) && (FEATURE_CODE_DISPLAY_4WD == FALSE))
    {
        /** This FLAG indicates the Alert and Main-Screen Display condition **/
        FourxFour_Engage_DisEngage_Alert_4WD = TRUE;
        FourxFour_Main_Screen_4WD = FALSE;
    }
    else
    {
        /** This FLAG indicates the Alert and Main-Screen Display condition **/
        FourxFour_Engage_DisEngage_Alert_4WD = FALSE;
        FourxFour_Main_Screen_4WD = FALSE;
    }

}

/*******************************************************************************
** Syntax           : void Main_Screen_Display_4WD( void )                    **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for actual condition check for    **
**                    Main Screen display.                                    **
**                    This function shall be called every 100ms               **
**                                                                            **
*******************************************************************************/
void Main_Screen_Display_4WD( void )
{
    /*All Feature code and diag code check*/
    FourxFour_Alert_Main_Screen_Display_4WD_Config();
    if(IGN_ON_flag == ON)
    {
        if((LVSD_flag == FALSE) && (HVSD_flag == FALSE))
        {
            if(FourxFour_Main_Screen_4WD == TRUE)
            {
                FourxFour_Main_Screen_Transfer_Mode = (CAN_IC2_100[3].byte & 0x60) >> 5;   /** Storing values from CAN_IC2_100 signal to Alert and Power Display condition **/
                if((FourxFour_Main_Screen_Transfer_Mode >= FOURxFOUR_MIN ) && (FourxFour_Main_Screen_Transfer_Mode < FOURxFOUR_MAX))
                {
                    FxFY.Invalid_FourxFour_Msg = FALSE;
                    FxFY.FourxFour_Main_Screen_Display_4WD_Valid = ON;  /** This flag used for validity check of Transfer Mode value for Main screen display **/
                    FxFY.FourxFour_Main_Screen_Display_Mode = FourxFour_Main_Screen_Transfer_Mode; /** Display Four Wheel drive HIGH on Main screen **/
                    if ((FxFY.FourxFour_Main_Screen_Display_Mode == TWO_WD)/*&&(var_2wd_animation_start == 0)*/)
                    {
 //                      (void)drome_sendEventToDrome((U32)drome_InEvent_VI_4X4_4WD_Animation_Stop);
//                       (void)drome_sendEventToDrome((U32)drome_InEvent_VI_4X4_2WD_Animation_Start); /* make 2WD screen on */
                       index_4X4_Text = TWO_WD;
 //                      VI_4WD_Framerate = 10;
 //                      VI_2WD_Visiblity = 1;
                       Mscreen_4WH_shown = 0;
                       Mscreen_4WL_shown = 0;
                       VI_4WD_Visiblity = 0;
                       var_2wd_animation_start = 1;
                    }
                    else if ((FxFY.FourxFour_Main_Screen_Display_Mode == FOUR_WDH)&&(Mscreen_4WH_shown == 0))
                    {
                        (void)drome_sendEventToDrome((U32)drome_InEvent_VI_4X4_2WD_Animation_Stop);
                       (void)drome_sendEventToDrome((U32)drome_InEvent_VI_4X4_4WD_Animation_Start); /* make 4WDH screen on */
                       index_4X4_Text = FOUR_WDH;
                       Mscreen_4WH_shown = 1;
                       Mscreen_4WL_shown = 0;
                       VI_4WD_Framerate = 8;//10;
                       VI_2WD_Visiblity = 0;
                       VI_4WD_Visiblity = 1;
                    }
                    else if ((FxFY.FourxFour_Main_Screen_Display_Mode == FOUR_WDL)&&(Mscreen_4WL_shown == 0))
                    {
                        (void)drome_sendEventToDrome((U32)drome_InEvent_VI_4X4_2WD_Animation_Stop);
                       (void)drome_sendEventToDrome((U32)drome_InEvent_VI_4X4_4WD_Animation_Start); /* make 4WDL screen on */
                       index_4X4_Text = FOUR_WDL;
                       Mscreen_4WL_shown = 1;
                       Mscreen_4WH_shown = 0;
#ifdef GRAYSCALE_TFT
                       VI_4WD_Framerate = 3;
#else
                       //VI_4WD_Framerate = 6;//3;
                       VI_4WD_Framerate = 3;
#endif
                       VI_2WD_Visiblity = 0;
                       VI_4WD_Visiblity = 1;
                    }
                    else
                    {
 //                   	Mscreen_4WH_shown = 0;
//                    	Mscreen_4WL_shown = 0;
                        /* code */
                    }                   
                    
                }
                else
                {
                    FxFY.Invalid_FourxFour_Msg = TRUE;
                    FxFY.FourxFour_Main_Screen_Display_4WD_Valid = OFF;  /** This flag used for validity check of Transfer Mode value for Main screen display **/
                    Mscreen_4WH_shown = 0;
                    Mscreen_4WL_shown = 0;

                }
            }
            else
            {
                /*** Main Screen display should be inactivated  ***/
                FxFY.FourxFour_Main_Screen_Display_4WD_Valid = OFF;
                Mscreen_4WH_shown = 0;
                Mscreen_4WL_shown = 0;
            }
        }
        else
        {
            /*** Main Screen  display should be inactivated  ***/
            FxFY.FourxFour_Main_Screen_Display_4WD_Valid = OFF;
            Mscreen_4WH_shown = 0;
            Mscreen_4WL_shown = 0;
        }
    }
    else
    {
        /*** Main Screen  display should be inactivated  ***/
        FxFY.FourxFour_Main_Screen_Display_4WD_Valid = OFF;
        Mscreen_4WH_shown = 0;
        Mscreen_4WL_shown = 0;
    }

    if(startkey != 0x11202) /*if on screen apart from 4WD screen then variable resetting - 25-05-20*/
    {
    	Mscreen_4WH_shown = 0;
    	Mscreen_4WL_shown = 0;
    }
}

/*******************************************************************************
** Syntax           : void Alert_Condition_4WD_Meter( void )                  **
**                                                                            **
** Parameters(in)   : None                                                    **
**                                                                            **
** Parameters (out) : None                                                    **
**                                                                            **
** Return value     : None                                                    **
**                                                                            **
** Description      : This function is used for actual condition check for    **
**                    Alert display.                                          **
**                    This function shall be called every 100ms               **
**                                                                            **
*******************************************************************************/
void Alert_Condition_4WD_Meter( void )
{
    uint8_t             Current_Alert_Condition_Transfer_Mode;

    /** Initialization of 4WD Alert and Main Screen display condition **/
    FourxFour_Alert_Main_Screen_Display_4WD_Config();
    if(IGN_ON_flag == ON)
    {
        if((LVSD_flag == FALSE) && (HVSD_flag == FALSE))
        {
            if(FourxFour_Engage_DisEngage_Alert_4WD == TRUE)
            {
                Current_Alert_Condition_Transfer_Mode = (CAN_IC2_100[3].byte & 0x60) >> 5;   /** Storing values from CAN_IC2_100 signal to Alert and Power Display condition **/
                if((Current_Alert_Condition_Transfer_Mode >= FOURxFOUR_MIN) && (Current_Alert_Condition_Transfer_Mode < FOURxFOUR_MAX))
                {
                    FxFY.Invalid_FourxFour_Msg = FALSE;
                    FxFY.FourxFour_Alert_Condition_4WD_Valid = ON;  /** This flag used for validity check of Transfer Mode value for Alert functionality **/
                    if((Previous_Alert_Condition_Transfer_Mode == TWO_WD) && (Current_Alert_Condition_Transfer_Mode == FOUR_WDL))
                    {
                    /** In this case transition from 2WD to 4WDL should be shown **/
                        FxFY.FourxFour_Alert_Condition_4WD = TWO_WD_4WDL; /** Display Two Wheel drive Low on Alert screen **/

                        if ((FxFY.FourxFour_Alert_Condition_4WD == TWO_WD_4WDL)&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)&& (skip_flag == 0))
                        {
                        	Priority_flag = 1;

                        	(void)drome_sendEventToDrome((U32)drome_InEvent_4X4_alert_trigger);
                        	(void)drome_sendEventToDrome((U32)drome_InEvent_4x4_Animation_Start);
                        	Alert_4WL_shown = 1;
                        	Alert_4WH_shown = 0;
                        	Previous_Alert_Condition_Transfer_Mode = FOUR_WDL;
                        	dont_throw_alerts_alert_on_screen_present=1;
                        	skip_alert_Enable = 1;
                        	alerts_history[2] = 1;
                        	index_4X4_Text = FOUR_WDL;
#ifdef COLOR_TFT
                        	var_4x4_frameRate = 3;
#else
                        	var_4x4_frameRate = 4;	/*For changes W501L237 dated 090620*/
#endif
                        	var_4x4_2WD_visible = 0;
                        	var_4x4_4WD_visible = 1;
                        }                    
                        
                    }
                    else if((Previous_Alert_Condition_Transfer_Mode == FOUR_WDL) && (Current_Alert_Condition_Transfer_Mode == TWO_WD))
                    {
                        /** In this case transition from 4WDL to 2WD should be shown **/
                        FxFY.FourxFour_Alert_Condition_4WD = FOUR_WDL_2WD; /** Display Two Wheel drive on Alert screen **/

                        if ((FxFY.FourxFour_Alert_Condition_4WD == FOUR_WDL_2WD)/*&&(dont_throw_alerts_alert_on_screen_present==0)&&(dont_throw_alerts_BS6_alert_on_screen_present==0)*/)
                        {
//                            Priority_flag = 1;
//						    (void)drome_sendEventToDrome((U32)drome_InEvent_4X4_alert_trigger);
//				    		(void)drome_sendEventToDrome((U32)drome_InEvent_4x4_2WD_Animation_Start);
				    		Previous_Alert_Condition_Transfer_Mode = TWO_WD;
//                           dont_throw_alerts_alert_on_screen_present=1;
//                            skip_alert_Enable = 1;
//                            alerts_history[2] = 1;
//                            index_4X4_Text = TWO_WD;

				    		/* var_4x4_frameRate = 10; */
//                            var_4x4_2WD_visible = 1;
				    		Alert_4WL_shown = 0;
				    		Alert_4WH_shown = 0;
				    		Priority_flag = 0;
				    		start_alart_timeout_5sec=0;
				    		dont_throw_alerts_alert_on_screen_present=0;
				    		(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
				    		var_4x4_4WD_visible = 0;
                        }  

                    }
                    else if((Previous_Alert_Condition_Transfer_Mode == TWO_WD) && (Current_Alert_Condition_Transfer_Mode == FOUR_WDH))
                    {
                        /** In this case transition from 2WD to 4WDH should be shown **/
                        FxFY.FourxFour_Alert_Condition_4WD = TWO_WD_4WDH; /** Display Four Wheel drive HIGH on Alert screen **/

                        if ((FxFY.FourxFour_Alert_Condition_4WD == TWO_WD_4WDH) && (dont_throw_alerts_alert_on_screen_present==0) && (dont_throw_alerts_BS6_alert_on_screen_present==0)&& (skip_flag == 0))
                        {
                            Priority_flag = 1;
						    (void)drome_sendEventToDrome((U32)drome_InEvent_4X4_alert_trigger);
				    		(void)drome_sendEventToDrome((U32)drome_InEvent_4x4_Animation_Start);
				    		Previous_Alert_Condition_Transfer_Mode = FOUR_WDH;
                            dont_throw_alerts_alert_on_screen_present=1;
                            skip_alert_Enable = 1;
                            alerts_history[2] = 1;
                            index_4X4_Text = FOUR_WDH;
                            Alert_4WH_shown = 1;
                            Alert_4WL_shown = 0;
                            var_4x4_frameRate = 8;//10;
                            var_4x4_2WD_visible = 0;
                            var_4x4_4WD_visible = 1;
                        } 
                        
                    }
                    else if((Previous_Alert_Condition_Transfer_Mode == FOUR_WDH) && (Current_Alert_Condition_Transfer_Mode == FOUR_WDL))
                    {
                        /** In this case transition from 4WDH to 4WDL should be shown **/
                        FxFY.FourxFour_Alert_Condition_4WD = FOUR_WDH_FOUR_WDL; /** Display Four Wheel drive Low on Alert screen **/

                        if (((FxFY.FourxFour_Alert_Condition_4WD == FOUR_WDH_FOUR_WDL) && (dont_throw_alerts_alert_on_screen_present==0) && (dont_throw_alerts_BS6_alert_on_screen_present==0)&& (skip_flag == 0))||((startkey==0x22070)&&(Alert_4WH_shown == 1)))
                        {
                            Priority_flag = 1;
						    (void)drome_sendEventToDrome((U32)drome_InEvent_4X4_alert_trigger);
				    		(void)drome_sendEventToDrome((U32)drome_InEvent_4x4_Animation_Start);
				    		Previous_Alert_Condition_Transfer_Mode = FOUR_WDL;
                            dont_throw_alerts_alert_on_screen_present=1;
                            skip_alert_Enable = 1;
                            alerts_history[2] = 1;
                            index_4X4_Text = FOUR_WDL;
                            Alert_4WL_shown = 1;
                            Alert_4WH_shown = 0;

#ifdef COLOR_TFT
                        	var_4x4_frameRate = 3;
#else
                        	var_4x4_frameRate = 4;		/*For changes W501L237 dated 090620*/
#endif
                            var_4x4_2WD_visible = 0;
                            var_4x4_4WD_visible = 1;
                        }
                        
                        
                    }
                    else if((Previous_Alert_Condition_Transfer_Mode == FOUR_WDL) && (Current_Alert_Condition_Transfer_Mode == FOUR_WDH))
                    {
                        /** In this case transition from 4WDL to 4WDH should be shown **/
                        FxFY.FourxFour_Alert_Condition_4WD = FOUR_WDL_4WDH; /** Display Four Wheel drive HIGH on Alert screen **/

                        if (((FxFY.FourxFour_Alert_Condition_4WD == FOUR_WDL_4WDH) && (dont_throw_alerts_alert_on_screen_present==0) && (dont_throw_alerts_BS6_alert_on_screen_present==0)  && (skip_flag == 0))||((startkey==0x22070)&&(Alert_4WL_shown == 1)))
                        {
                            Priority_flag = 1;
						    (void)drome_sendEventToDrome((U32)drome_InEvent_4X4_alert_trigger);
				    		(void)drome_sendEventToDrome((U32)drome_InEvent_4x4_Animation_Start);
				    		Previous_Alert_Condition_Transfer_Mode = FOUR_WDH;
                            dont_throw_alerts_alert_on_screen_present=1;
                            skip_alert_Enable = 1;
                            alerts_history[2] = 1;
                            index_4X4_Text = FOUR_WDH;
                            Alert_4WH_shown = 1;
                            Alert_4WL_shown = 0;
                            var_4x4_frameRate = 8;//10;
                            var_4x4_2WD_visible = 0;
                            var_4x4_4WD_visible = 1;
                        }
                        
                    }
                    else if((Previous_Alert_Condition_Transfer_Mode == FOUR_WDH) && (Current_Alert_Condition_Transfer_Mode == TWO_WD))
                    {
                             /** In this case transition from 4WDL to 4WDH should be shown **/
                        FxFY.FourxFour_Alert_Condition_4WD = FOUR_WDH_TWO_WD; /** Display Two Wheel drive on Alert screen **/

                         if ((FxFY.FourxFour_Alert_Condition_4WD == FOUR_WDH_TWO_WD) /*&& (dont_throw_alerts_alert_on_screen_present==0) && (dont_throw_alerts_BS6_alert_on_screen_present==0)*/)
                        {
//                            Priority_flag = 1;
//						    (void)drome_sendEventToDrome((U32)drome_InEvent_4X4_alert_trigger);
//				    		(void)drome_sendEventToDrome((U32)drome_InEvent_4x4_2WD_Animation_Start);
				    		Previous_Alert_Condition_Transfer_Mode = TWO_WD;
//                           dont_throw_alerts_alert_on_screen_present=1;
//                            skip_alert_Enable = 1;
//                            alerts_history[2] = 1;
//                            index_4X4_Text = TWO_WD;

				    		/*  var_4x4_frameRate = 10; */
//                           var_4x4_2WD_visible = 1;
				    		Alert_4WL_shown = 0;
				    		Alert_4WH_shown = 0;
				    		var_4x4_4WD_visible = 0;
				    		Priority_flag = 0;
				    		start_alart_timeout_5sec=0;
				    		dont_throw_alerts_alert_on_screen_present=0;
				    		(void)drome_sendEventToDrome((U32)drome_InEvent_Alert_Timeout);	//timeout alert as soon as condition not satisfy
                        }
                    }
                    else
                    {
                    	/* FourxFour_Alert_Condition_4WD = Previous_Alert_Condition_Transfer_Mode;  Display Four Wheel drive LOW on Main screen **/
                    	Previous_Alert_Condition_Transfer_Mode = Current_Alert_Condition_Transfer_Mode;
//                    	Alert_4WL_shown = 0;
//                    	Alert_4WH_shown = 0;
                    }
                }
                else
                {
                    FxFY.Invalid_FourxFour_Msg = TRUE;
                    FxFY.FourxFour_Alert_Condition_4WD_Valid = OFF;  /** This flag used for validity check of Transfer Mode value for Main screen display **/
                }
            }
            else
            {
                /*** Main Screen display should be inactivated  ***/
                FxFY.FourxFour_Alert_Condition_4WD_Valid = OFF;
                Alert_4WL_shown = 0;
                Alert_4WH_shown = 0;
            }

       }
       else
       {
           /*** Main Screen  display should be inactivated  ***/
           FxFY.FourxFour_Alert_Condition_4WD_Valid = OFF;
           Alert_4WL_shown = 0;
           Alert_4WH_shown = 0;
       }
    }
    else
    {
        /*** Main Screen  display should be inactivated  ***/
    	FxFY.FourxFour_Alert_Condition_4WD_Valid = OFF;
    	Alert_4WL_shown = 0;
    	Alert_4WH_shown = 0;
    }
}
