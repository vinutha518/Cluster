/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                 druserconf.h                                    ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***                     Copyright (C) Stoneridge Electronics AB                     ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup DROMEIF 

  \Subject    Automatically generated user configuration file 

  \Reference  None

  \Abstract 

  The events, data definitions and other options are generated from the user 
  configuration file. Please do not modify this file! 
  

  \Version history record: 

  $Id:$

\{
***************************************************************************************
*/

#ifndef DRUSERCONF_H
#define DRUSERCONF_H


/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/


#include "drhost.h"


/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/* CRC32-code of user config options at compile time */
#define drome_USER_CONFIG_CRC32 0x589F1EAUL

/*### drome_Event begin ###*/
typedef enum
{
   drome_NULL_EVENT,
/* Input events */
   drome_KB_MENU,   /* Keyboard right event */
   drome_KB_OK,   /* Keyboard ok event */
   drome_KB_UP,   /* Keyboard UP event */
   drome_KB_DOWN,   /* Keyboard DOWN event */
   drome_KB_OK_LONG,   /* Keyboard DOWN event */
   drome_InEventMain_screen_timeout,   /* Main_screen_timeout event */
   drome_InEvent_DEF_Level_Low,   /* Alert2  event */
   drome_InEvent_DEF_level_Low_Engine_Not_Start,   /* Alert3  event */
   drome_InEvent_Incorrect_DEF_Detected,   /* Alert4 event */
   drome_InEvent_Incorrect_DEF_Detected_Engine_Not_Start,   /* Alert5  event */
   drome_InEvent_No_restart_Fill_DEF,   /* Alert3 event */
   drome_InEvent_No_start_Fill_DEF,   /* Alert4 event */
   drome_InEvent_No_restart_Check_DEF,   /* Alert7  event */
   drome_InEvent_No_start_Check_DEF,   /* Alert8  event */
   drome_InEvent_DEF_Dosing_Malfunction,   /* Alert9  event */
   drome_InEvent_DEF_Dosing_Malfunction_Engine_Not_Start,   /* Alert10  event */
   drome_InEvent_No_restart_Dose_Malfunction,   /* Alert11  event */
   drome_InEvent_No_start_Dose_Malfunction,   /* Alert12  event */
   drome_InEvent_Fuel_Jump,   /* Fuel Bar jump event */
   drome_InEvent_Fuel_Return,   /* Fuel Bar Return event */
   drome_InEvent_Temp_Jump,   /* Temp Bar jump event */
   drome_InEvent_Temp_Return,   /* Temp Bar Return event */
   drome_InEvent_REGEN_Required,   /* REGEN required Alert event */
   drome_InEvent_REGEN_aborted,   /* REGEN aborted Alert event */
   drome_InEvent_REGEN_completed,   /* REGEN completed Alert event */
   drome_InEvent_REGEN_started,   /* REGEN completed Alert event */
   drome_InEvent_REGEN_not_started,   /* REGEN completed Alert event */
   drome_InEvent_Alert_Timeout,   /* REGEN Alert timeout event */
   drome_InEvent_Seat_Belt,   /* REGEN Alert timeout event */
   drome_InEvent_Check_ABS,   /* REGEN Alert timeout event */
   drome_InEvent_Low_Brake_Fluid,   /* REGEN Alert timeout event */
   drome_InEvent_Low_Engine_Oil_Pressure,   /* REGEN Alert timeout event */
   drome_InEvent_Engine_Temp_High,   /* REGEN Alert timeout event */
   drome_InEvent_Low_Fuel,   /* REGEN Alert timeout event */
   drome_InEvent_DPF_Clogged,   /* REGEN Alert timeout event */
   drome_InEvent_BACK_REGEN,   /* REGEN Alert timeout event */
   drome_InEvent_REGEN_STARTED_timeout,   /* InEvent_REGEN_STARTED_timeout event */
   drome_InEvent_REGEN_NOTSTARTED_timeout,   /* InEvent_REGEN_NOTSTARTED_timeout event */
   drome_InEvent_Short_circuit,   /* InEvent_Short_circuit event */
   drome_InEvent_Open_circuit,   /* InEvent_Open_circuit event */
   drome_InEvent_Invalid_tempbar,   /* InEvent_Invalid_tempbar event */
   drome_InEvent_Invalid_toNormal_temp,   /* InEvent_Invalid_toNormal_temp event */
   drome_InEvent_Low_Battery,   /* Low_Battery event */
   drome_InEvent_Overspeed,   /* Overspeed event */
   drome_InEvent_Water_in_Fuel,   /* Water_in_Fuel event */
   drome_InEvent_parkbrake,   /* parkbrake event */
   drome_InEvent_check_engine,   /* Check_engine event */
   drome_InEvent_Check_EBD,   /* Check_EBD event */
   drome_InEvent_Service_Due,   /* Check_EBD event */
   drome_InEvent_Check_ESP,   /* Check_EBD event */
   drome_InEvent_Door_Open,   /* Check_EBD event */
   drome_InEvent_Backdoor_doorClose,   /* Check_EBD event */
   drome_InEvent_Backdoor_dooropen,   /* Check_EBD event */
   drome_InEvent_Codriver_doorclose,   /* Check_EBD event */
   drome_InEvent_Codriver_dooropen,   /* Check_EBD event */
   drome_InEvent_Hood_doorclose,   /* Check_EBD event */
   drome_InEvent_Hood_dooropen,   /* Check_EBD event */
   drome_InEvent_RearRight_doorclose,   /* Check_EBD event */
   drome_InEvent_RearRight_dooropen,   /* Check_EBD event */
   drome_InEvent_Rearleft_dooropen,   /* Check_EBD event */
   drome_InEvent_Rearleft_doorclose,   /* Check_EBD event */
   drome_InEvent_driver_doorclose,   /* Check_EBD event */
   drome_InEvent_driver_dooropen,   /* Check_EBD event */
   drome_InEvent_Alldoorclose,   /* Alldoorclose */
   drome_InEvent_Cruise_ON,   /* Cruise ON */
   drome_InEvent_Cruise_OFF,   /* Cruise OFF */
   drome_InEvent_External_Lamp,   /* External Lamp */
   drome_InEvent_ECO_ON,   /* ECO Event */
   drome_InEvent_PAB_OFF,   /* PAB OFF Event */
   drome_InEvent_Trailer_SW_ON,   /* Trailer SW ON Event */
   drome_InEvent_Trailer_SW_OFF,   /* Trailer SW OFF Event */
   drome_InEvent_Horn_Failure,   /* Horn Failure Event */
   drome_InEvent_Left_Turn_Lamp_Failure,   /* Left Turn Lamp failure event */
   drome_InEvent_Right_Turn_Lamp_Failure,   /* Right Turn Lamp failure event */
   drome_InEvent_Tail_Lamp_Failure,   /* Tail Lamp failure event */
   drome_InEvent_RPAS_Supply_Failure,   /* RPAS Supply Failure Event */
   drome_InEvent_Key_In_Reminder,   /* Key In Reminder Event */
   drome_InEvent_ECO_OFF,   /* Park Lamp Reminder Event */
   drome_InEvent_Func_safety,   /* Function Safety Event */
   drome_InEvent_Left_DRL_Lamp_Failure,   /* DRL/Park Lamp Failure Event */
   drome_InEvent_Right_DRL_Lamp_Failure,   /* Function Safety Event */
   drome_InEvent_Stop_Vehicle_For_Setting,   /* Lamp Fail Event */
   drome_InEvent_Longpress_Alert_Timeout,   /* Function Safety Event */
   drome_InEvent_REGEN_Requested,   /* Regen Requested Event */
   drome_InEvent_AFE_Back,   /* Back to main setting page Event */
   drome_InEvent_TimeF_Back,   /* Back to main setting page Event */
   drome_InEvent_Clock_Back,   /* Back to main setting page Event */
   drome_InEvent_TempU_Back,   /* Back to main setting page Event */
   drome_InEvent_1st_detected,   /* After eeprom read AFE 1st row detected Event */
   drome_InEvent_2nd_detected,   /* After eeprom read AFE 2nd row detected Event */
   drome_InEvent_3rd_detected,   /* After eeprom read AFE 3rd row detected Event */
   drome_InEvent_Back_to_TripA,   /* InEvent_Back_to_TripA from Setting page Event */
   drome_InEvent_Dooropen_IGNOFF,   /* InEvent_Dooropen_IGNOFF from Setting page Event */
   drome_InEvent_External_Lamp_IGNOFF,   /* InEvent_External_Lamp_IGNOFF Event */
   drome_InEvent_ETM_11sec_timeout,   /* InEvent_External_Lamp_IGNOFF Event */
   drome_InEvent_ETM_30sec_timeout,   /* InEvent_External_Lamp_IGNOFF Event */
   drome_InEvent_ETM_Entry,   /* InEvent_External_Lamp_IGNOFF Event */
   drome_InEvent_RPAS_screen_ACTIVE,   /* InEvent_RPAS_screen_ACTIVE Event */
   drome_InEvent_RPAS_screen_timeout,   /* InEvent_RPAS_screen_timeout Event */
   drome_InEvent_Drive_Summary,   /* InEvent_Drive_Summary Event */
   drome_InEvent_Drive_Sum_Timeout,   /* InEvent_Drive_Sum_Timeout Event */
   drome_InEvent_BS6_permanent_alert_inactive,   /* InEvent_BS6_permanent_alert_inactive Event */
   drome_InEvent_IOcontrol_TFT,   /* InEvent_IOcontrol_TFT Event */
   drome_InEvent_IOcontrol_Exit,   /* InEvent_IOcontrol_Exit Event */
   drome_InEvent_WH_Backdoor_doorClose,   /* Check_EBD event */
   drome_InEvent_WH_Backdoor_dooropen,   /* Check_EBD event */
   drome_InEvent_WH_Codriver_doorclose,   /* Check_EBD event */
   drome_InEvent_WH_Codriver_dooropen,   /* Check_EBD event */
   drome_InEvent_WH_Hood_doorclose,   /* Check_EBD event */
   drome_InEvent_WH_Hood_dooropen,   /* Check_EBD event */
   drome_InEvent_WH_RearRight_doorclose,   /* Check_EBD event */
   drome_InEvent_WH_RearRight_dooropen,   /* Check_EBD event */
   drome_InEvent_WH_Rearleft_dooropen,   /* Check_EBD event */
   drome_InEvent_WH_Rearleft_doorclose,   /* Check_EBD event */
   drome_InEvent_WH_driver_doorclose,   /* Check_EBD event */
   drome_InEvent_WH_driver_dooropen,   /* Check_EBD event */
   drome_InEvent_WH_Alldoorclose,   /* WH_Alldoorclose event */
   drome_InEvent_All_Pixel_OFF,   /* InEvent_All_Pixel_OFF event */
   drome_InEvent_All_Pixel_ON,   /* InEvent_All_Pixel_ON event */
   drome_InEvent_All_Pixel_White,   /* InEvent_All_Pixel_White event */
   drome_InEvent_All_Pixel_checker,   /* InEvent_All_Pixel_checker event */
   drome_InEvent_All_Pixel_checker_Inverted,   /* InEvent_All_Pixel_checker_Inverted event */
   drome_InEvent_start_animation,   /* InEvent_start_animation event */
   drome_InEvent_Setting_Timeout,   /* InEvent_Setting_Timeout event */
   drome_InEvent_HDC_Active,   /* InEvent_HDC_Active event */
   drome_InEvent_HDC_Passive,   /* InEvent_HDC_Passive event */
   drome_InEvent_HDC_Ready,   /* InEvent_HDC_Ready event */
   drome_InEvent_4X4_alert_trigger,   /* InEvent_4X4_alert_trigger event */
   drome_InEvent_4x4_Animation_Start,   /* InEvent_4x4_Animation_Start event */
   drome_InEvent_4x4_Animation_Stop,   /* InEvent_4x4_Animation_Stop event */
   drome_InEvent_4x4_2WD_Animation_Start,   /* InEvent_4x4_2WD_Animation_Start */
   drome_InEvent_VI_4X4_2WD_Animation_Start,   /* InEvent_VI_4X4_2WD_Animation_Start */
   drome_InEvent_VI_4X4_2WD_Animation_Stop,   /* InEvent_VI_4X4_2WD_Animation_Stop */
   drome_InEvent_VI_4X4_4WD_Animation_Start,   /* InEvent_VI_4X4_4WD_Animation_Start */
   drome_InEvent_VI_4X4_4WD_Animation_Stop,   /* InEvent_VI_4X4_4WD_Animation_Stop */
   drome_InEvent_EcoScore_Jump,   /* InEvent_EcoScore_Jump */
   drome_InEvent_EcoScore_Return,   /* InEvent_EcoScore_Return */
   drome_InEvent_TDMS_alert_trigger,   /* InEvent_TDMS_alert_trigger event */
   drome_InEvent_AT_Alert_Detected,   /* InEvent_AT_Alert_Detected event */
   drome_InEvent_AT_Alert_Timeout,   /* InEvent_AT_Alert_Timeout event */
   drome_LAST_IN_EVENT,   /* Last in event to drome */
/* Output events */
   drome_OutEvent_mainscreen,   /* Outgoing event from mainscreen */
   drome_OutEvent_REGEN_started,   /* Outgoing event from REGEN startrd page */
   drome_OutEvent_REGEN_not_started,   /* Outgoing event from REGEN not startrd page */
   drome_OutEvent_REGEN_requested,   /* Outgoing event from REGEN requested page */
   drome_OutEvent_REGEN_completed_popup,   /* Outgoing event from REGEN requested page */
   drome_OutEvent_REGEN_aborted_popup,   /* Outgoing event from REGEN requested page */
   drome_OutEvent_logo_completed,   /* Outgoing event from logo completed page */
   drome_OutEvent_Alert_started,   /* Outgoing event from Alert started page */
   drome_OutEvent_Alert_Escape,   /* Outgoing event for Alert Escape page */
   drome_Out_EV_AFE_Reset,   /* Outgoing event for Alert Escape page */
   drome_Out_EV_Setting_clock,   /* Outgoing event for Alert Escape page */
   drome_Out_EV_Timeformat,   /* Outgoing event for Alert Escape page */
   drome_Out_AFE_first_row,   /* Outgoing event for Alert Escape page */
   drome_Out_AFE_second_row,   /* Outgoing event for Alert Escape page */
   drome_Out_AFE_third_row,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_AFE_1st_row_selected,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_AFE_2nd_row_selected,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_AFE_3rd_row_selected,   /* Outgoing event for Alert Escape page */
   drome_Out_AFEMenu_Entered,   /* Outgoing event for Out_AFEMenu_Entered */
   drome_Out_TimeF_first_row,   /* Outgoing event for Alert Escape page */
   drome_Out_TimeF_second_row,   /* Outgoing event for Alert Escape page */
   drome_Out_TimeF_third_row,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_TimeF_1st_row_selected,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_TimeF_2nd_row_selected,   /* Outgoing event for Alert Escape page */
   drome_Out_TimeFMenu_Entered,   /* Outgoing event for Out_AFEMenu_Entered */
   drome_Out_EV_Clock,   /* Outgoing event for Out_EV_Clock */
   drome_Out_EV_TimeSetting,   /* Outgoing event for Out_EV_TimeSetting */
   drome_OutEvent_TimeS_Back,   /* Outgoing event for OutEvent_TimeS_Back */
   drome_OutEvent_TimeSet_Entered,   /* Outgoing event for OutEvent_TimeSet_Entered */
   drome_OutEvent_start_1st_increment,   /* Outgoing event for OutEvent_start_1st_increment */
   drome_OutEvent_start_2nd_increment,   /* Outgoing event for OutEvent_start_2nd_increment */
   drome_OutEvent_start_3rd_increment,   /* Outgoing event for OutEvent_start_3rd_increment */
   drome_OutEvent_start_4th_increment,   /* Outgoing event for OutEvent_start_4th_increment */
   drome_OutEvent_start_5th_increment,   /* Outgoing event for OutEvent_start_5th_increment */
   drome_OutEvent_Backsave_time,   /* Outgoing event for OutEvent_Backsave_time */
   drome_OutEvent_WH_Page,   /* Outgoing event for OutEvent_WH_Page */
   drome_OutEvent_WH_Back,   /* Outgoing event for OutEvent_WH_Back */
   drome_Out_TempU_first_row,   /* Outgoing event for Alert Escape page */
   drome_Out_TempU_second_row,   /* Outgoing event for Alert Escape page */
   drome_Out_TempU_third_row,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_TempU_1st_row_selected,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_TempU_2nd_row_selected,   /* Outgoing event for Alert Escape page */
   drome_OutEvent_TempU_Entered,   /* Outgoing event for OutEvent_TempU_Entered */
   drome_OutEvent_TFT_Check_start,   /* Outgoing event for OutEvent_TFT_Check_start */
   drome_OutEvent_TFT_Check_end,   /* Outgoing event for OutEvent_TFT_Check_end */
   drome_OutEvent_PAB_deselected,   /* Outgoing event for OutEvent_PAB_deselected */
   drome_OutEvent_PAB_row,   /* Outgoing event for OutEvent_PAB_row */
   drome_OutEvent_PAB_selected,   /* Outgoing event for OutEvent_PAB_selected */
   drome_OutEvent_Setting_Entered,   /* Outgoing event for OutEvent_Setting_Entered */
   drome_OutEvent_Setting_Back,   /* Outgoing event for OutEvent_Setting_Back */
   drome_OutEvent_PAB_disable_No_detected,   /* Outgoing event for OutEvent_PAB_disable_No_detected */
   drome_OutEvent_PAB_disable_Yes_detected,   /* Outgoing event for OutEvent_PAB_disable_Yes_detected */
   drome_OutEvent_PAB_enable_No_detected,   /* Outgoing event for OutEvent_PAB_enable_No_detected */
   drome_OutEvent_PAB_enable_Yes_detected,   /* Outgoing event for OutEvent_PAB_enable_Yes_detected */
   drome_OutEvent_Drive_Sum_Alert_Escape,   /* Outgoing event for OutEvent_Drive_Sum_Alert_Escape */
   drome_OutEvent_Drive_Sum_Alert_started,   /* Outgoing event for OutEvent_Drive_Sum_Alert_started */
   drome_OutEvent_Skip_EngOilPressureLow,   /* Outgoing event for OutEvent_Skip_EngOilPressureLow */
   drome_OutEvent_Skip_EngTempHigh,   /* Outgoing event for OutEvent_Skip_EngTempHigh */
   drome_OutEvent_Skip_LowFuel,   /* Outgoing event for OutEvent_Skip_LowFuel */
   drome_OutEvent_Skip_CheckEng,   /* Outgoing event for OutEvent_Skip_CheckEng */
   drome_OutEvent_Skip_LowBrakefluid,   /* Outgoing event for OutEvent_Skip_LowBrakefluid */
   drome_OutEvent_Skip_ParkBrake,   /* Outgoing event for OutEvent_Skip_ParkBrake */
   drome_OutEvent_Skip_PABoff,   /* Outgoing event for OutEvent_Skip_PABoff */
   drome_OutEvent_Skip_DPFclogged,   /* Outgoing event for OutEvent_Skip_DPFclogged */
   drome_OutEvent_Skip_RegenRequired,   /* Outgoing event for OutEvent_Skip_RegenRequired */
   drome_OutEvent_Skip_RegenRequested,   /* Outgoing event for OutEvent_Skip_RegenRequested */
   drome_OutEvent_Skip_RegenStarted,   /* Outgoing event for OutEvent_Skip_RegenStarted */
   drome_OutEvent_Skip_RegenNotStarted,   /* Outgoing event for OutEvent_Skip_RegenNotStarted */
   drome_OutEvent_Skip_RegenCompleted,   /* Outgoing event for OutEvent_Skip_RegenCompleted */
   drome_OutEvent_Skip_RegenAborted,   /* Outgoing event for OutEvent_Skip_RegenAborted */
   drome_OutEvent_Skip_DoorOpen,   /* Outgoing event for OutEvent_Skip_DoorOpen */
   drome_OutEvent_Skip_EBDmalfunction,   /* Outgoing event for OutEvent_Skip_EBDmalfunction */
   drome_OutEvent_Skip_ABSmalfunction,   /* Outgoing event for OutEvent_Skip_ABSmalfunction */
   drome_OutEvent_Skip_ESPmalfunction,   /* Outgoing event for OutEvent_Skip_ESPmalfunction */
   drome_OutEvent_Skip_RPASsupplyFailure,   /* Outgoing event for OutEvent_Skip_RPASsupplyFailure */
   drome_OutEvent_Skip_RKE_Battery_Low,   /* Outgoing event for OutEvent_Skip_RKE_Battery_Low */
   drome_OutEvent_Skip_WaterInFuel,   /* Outgoing event for OutEvent_Skip_WaterInFuel */
   drome_OutEvent_Skip_ServiceReminder,   /* Outgoing event for OutEvent_Skip_ServiceReminder */
   drome_OutEvent_Skip_FuncSafety,   /* Outgoing event for OutEvent_Skip_FuncSafety */
   drome_OutEvent_Skip_DEF_Dosing_Malfunction,   /* Outgoing event for OutEvent_Skip_DEF_Dosing_Malfunction */
   drome_OutEvent_Skip_DEF_Dosing_Malfunction_Engine,   /* Outgoing event for OutEvent_Skip_DEF_Dosing_Malfunction_Engine */
   drome_OutEvent_Skip_DEF_Level_Low,   /* Outgoing event for OutEvent_Skip_DEF_Level_Low */
   drome_OutEvent_Skip_DEF_level_Low_Engine_Not_Start,   /* Outgoing event for OutEvent_Skip_DEF_level_Low_Engine_Not_Start */
   drome_OutEvent_Skip_Incorrect_DEF_Detected,   /* Outgoing event for OutEvent_Skip_Incorrect_DEF_Detected */
   drome_OutEvent_Skip_Incorrect_DEF_Detected_Engine,   /* Outgoing event for OutEvent_Skip_Incorrect_DEF_Detected_Engine */
   drome_OutEvent_Skip_No_restart_Check_DEF,   /* Outgoing event for OutEvent_Skip_No_restart_Check_DEF */
   drome_OutEvent_Skip_No_restart_Dose_Malfunction,   /* Outgoing event for OutEvent_Skip_No_restart_Dose_Malfunction */
   drome_OutEvent_Skip_No_restart_Fill_DEF,   /* Outgoing event for OutEvent_Skip_No_restart_Fill_DEF */
   drome_OutEvent_Skip_No_start_Check_DEF,   /* Outgoing event for OutEvent_Skip_No_start_Check_DEF */
   drome_OutEvent_Skip_No_start_Dose_Malfunction,   /* Outgoing event for OutEvent_Skip_No_start_Dose_Malfunction */
   drome_OutEvent_Skip_No_start_Fill_DEF,   /* Outgoing event for OutEvent_Skip_No_start_Fill_DEF */
   drome_OutEvent_TimeS_Highlight_Confirm,   /* Outgoing event for OutEvent_TimeS_Highlight_Confirm */
   drome_OutEvent_TimeS_Nohighlight_Confirm,   /* Outgoing event for OutEvent_TimeS_Nohighlight_Confirm */
   drome_OutEvent_TimeS_NoHighlight_Back,   /* Outgoing event for OutEvent_TimeS_NoHighlight_Back */
   drome_OutEvent_0throw_detected,   /* Outgoing event for OutEvent_0throw_detected */
   drome_OutEvent_Selection_IOTFT,   /* Outgoing event for OutEvent_Selection_IOTFT */
   drome_OutEvent_PAB_Entered,   /* Outgoing event for OutEvent_PAB_Entered */
   drome_OutEvent_PAB_disable_entered,   /* Outgoing event for OutEvent_PAB_disable_entered */
   drome_OutEvent_PAB_enable_entered,   /* Outgoing event for OutEvent_PAB_enable_entered */
   drome_OutEvent_TripA,   /* Outgoing event for OutEvent_TripA */
   drome_OutEvent_TripAC,   /* Outgoing event for OutEvent_TripAC */
   drome_OutEvent_TripA_AVS,   /* Outgoing event for OutEvent_TripA_AVS */
   drome_OutEvent_TripA_AVS_DT,   /* Outgoing event for OutEvent_TripA_AVS_DT */
   drome_OutEvent_TripA_DT,   /* Outgoing event for OutEvent_TripA_DT */
   drome_OutEvent_TripB,   /* Outgoing event for OutEvent_TripB */
   drome_OutEvent_TripB_AVS,   /* Outgoing event for OutEvent_TripB_AVS */
   drome_OutEvent_TripB_AVS_DT,   /* Outgoing event for OutEvent_TripB_AVS_DT */
   drome_OutEvent_TripB_DT,   /* Outgoing event for OutEvent_TripB_DT */
   drome_OutEvent_Driveinfo_page,   /* Outgoing event for OutEvent_Driveinfo_page */
   drome_OutEvent_Digitalclock,   /* Outgoing event for OutEvent_Digitalclock */
   drome_OutEvent_Regen,   /* Outgoing event for OutEvent_Regen */
   drome_OutEvent_FC_AFE,   /* Outgoing event for OutEvent_FC_AFE */
   drome_OutEvent_FC_AFE_IFE,   /* Outgoing event for OutEvent_FC_AFE_IFE */
   drome_OutEvent_FC_DTE,   /* Outgoing event for OutEvent_FC_DTE */
   drome_OutEvent_FC_ECO,   /* Outgoing event for OutEvent_FC_ECO */
   drome_OutEvent_FC_IFE,   /* Outgoing event for OutEvent_FC_IFE */
   drome_OutEvent_FC_Page,   /* Outgoing event for OutEvent_FC_Page */
   drome_OutEvent_start_1st_decrement,   /* Outgoing event for OutEvent_start_1st_decrement */
   drome_OutEvent_start_3rd_decrement,   /* Outgoing event for OutEvent_start_3rd_decrement */
   drome_OutEvent_start_5th_decrement,   /* Outgoing event for OutEvent_start_5th_decrement */
   drome_OutEvent_Skip_LeftDRL,   /* Outgoing event for OutEvent_Skip_LeftDRL */
   drome_OutEvent_Skip_LeftTurn,   /* Outgoing event for OutEvent_Skip_LeftTurn */
   drome_OutEvent_Skip_RightDRL,   /* Outgoing event for OutEvent_Skip_RightDRL */
   drome_OutEvent_Skip_RightTurn,   /* Outgoing event for OutEvent_Skip_RightTurn */
   drome_OutEvent_Skip_TailLamp,   /* Outgoing event for OutEvent_Skip_TailLamp */
   drome_OutEvent_Skip_HornFailure,   /* Outgoing event for OutEvent_Skip_HornFailure */
   drome_OutEvent_Skip_TDMS,   /* Outgoing event for OutEvent_Skip_TDMS */
   drome_OutEvent_Vehicle_Info_Exit,   /* Outgoing event for OutEvent_Vehicle_Info_Exit */
   drome_OutEvent_AT_Gear_Alert_Started,   /* Outgoing event for OutEvent_AT_Gear_Alert_Started */
   drome_MAX_EVENTS
} drome_Event;
/*### drome_Event end ###*/

#ifdef NAME_EVENTS
const U8 * drome_EventNames[]=
{
   (const U8 *) "drome_nullEvent",
   (const U8 *) "KB_MENU",
   (const U8 *) "KB_OK",
   (const U8 *) "KB_UP",
   (const U8 *) "KB_DOWN",
   (const U8 *) "KB_OK_LONG",
   (const U8 *) "InEventMain_screen_timeout",
   (const U8 *) "InEvent_DEF_Level_Low",
   (const U8 *) "InEvent_DEF_level_Low_Engine_Not_Start",
   (const U8 *) "InEvent_Incorrect_DEF_Detected",
   (const U8 *) "InEvent_Incorrect_DEF_Detected_Engine_Not_Start",
   (const U8 *) "InEvent_No_restart_Fill_DEF",
   (const U8 *) "InEvent_No_start_Fill_DEF",
   (const U8 *) "InEvent_No_restart_Check_DEF",
   (const U8 *) "InEvent_No_start_Check_DEF",
   (const U8 *) "InEvent_DEF_Dosing_Malfunction",
   (const U8 *) "InEvent_DEF_Dosing_Malfunction_Engine_Not_Start",
   (const U8 *) "InEvent_No_restart_Dose_Malfunction",
   (const U8 *) "InEvent_No_start_Dose_Malfunction",
   (const U8 *) "InEvent_Fuel_Jump",
   (const U8 *) "InEvent_Fuel_Return",
   (const U8 *) "InEvent_Temp_Jump",
   (const U8 *) "InEvent_Temp_Return",
   (const U8 *) "InEvent_REGEN_Required",
   (const U8 *) "InEvent_REGEN_aborted",
   (const U8 *) "InEvent_REGEN_completed",
   (const U8 *) "InEvent_REGEN_started",
   (const U8 *) "InEvent_REGEN_not_started",
   (const U8 *) "InEvent_Alert_Timeout",
   (const U8 *) "InEvent_Seat_Belt",
   (const U8 *) "InEvent_Check_ABS",
   (const U8 *) "InEvent_Low_Brake_Fluid",
   (const U8 *) "InEvent_Low_Engine_Oil_Pressure",
   (const U8 *) "InEvent_Engine_Temp_High",
   (const U8 *) "InEvent_Low_Fuel",
   (const U8 *) "InEvent_DPF_Clogged",
   (const U8 *) "InEvent_BACK_REGEN",
   (const U8 *) "InEvent_REGEN_STARTED_timeout",
   (const U8 *) "InEvent_REGEN_NOTSTARTED_timeout",
   (const U8 *) "InEvent_Short_circuit",
   (const U8 *) "InEvent_Open_circuit",
   (const U8 *) "InEvent_Invalid_tempbar",
   (const U8 *) "InEvent_Invalid_toNormal_temp",
   (const U8 *) "InEvent_Low_Battery",
   (const U8 *) "InEvent_Overspeed",
   (const U8 *) "InEvent_Water_in_Fuel",
   (const U8 *) "InEvent_parkbrake",
   (const U8 *) "InEvent_check_engine",
   (const U8 *) "InEvent_Check_EBD",
   (const U8 *) "InEvent_Service_Due",
   (const U8 *) "InEvent_Check_ESP",
   (const U8 *) "InEvent_Door_Open",
   (const U8 *) "InEvent_Backdoor_doorClose",
   (const U8 *) "InEvent_Backdoor_dooropen",
   (const U8 *) "InEvent_Codriver_doorclose",
   (const U8 *) "InEvent_Codriver_dooropen",
   (const U8 *) "InEvent_Hood_doorclose",
   (const U8 *) "InEvent_Hood_dooropen",
   (const U8 *) "InEvent_RearRight_doorclose",
   (const U8 *) "InEvent_RearRight_dooropen",
   (const U8 *) "InEvent_Rearleft_dooropen",
   (const U8 *) "InEvent_Rearleft_doorclose",
   (const U8 *) "InEvent_driver_doorclose",
   (const U8 *) "InEvent_driver_dooropen",
   (const U8 *) "InEvent_Alldoorclose",
   (const U8 *) "InEvent_Cruise_ON",
   (const U8 *) "InEvent_Cruise_OFF",
   (const U8 *) "InEvent_External_Lamp",
   (const U8 *) "InEvent_ECO_ON",
   (const U8 *) "InEvent_PAB_OFF",
   (const U8 *) "InEvent_Trailer_SW_ON",
   (const U8 *) "InEvent_Trailer_SW_OFF",
   (const U8 *) "InEvent_Horn_Failure",
   (const U8 *) "InEvent_Left_Turn_Lamp_Failure",
   (const U8 *) "InEvent_Right_Turn_Lamp_Failure",
   (const U8 *) "InEvent_Tail_Lamp_Failure",
   (const U8 *) "InEvent_RPAS_Supply_Failure",
   (const U8 *) "InEvent_Key_In_Reminder",
   (const U8 *) "InEvent_ECO_OFF",
   (const U8 *) "InEvent_Func_safety",
   (const U8 *) "InEvent_Left_DRL_Lamp_Failure",
   (const U8 *) "InEvent_Right_DRL_Lamp_Failure",
   (const U8 *) "InEvent_Stop_Vehicle_For_Setting",
   (const U8 *) "InEvent_Longpress_Alert_Timeout",
   (const U8 *) "InEvent_REGEN_Requested",
   (const U8 *) "InEvent_AFE_Back",
   (const U8 *) "InEvent_TimeF_Back",
   (const U8 *) "InEvent_Clock_Back",
   (const U8 *) "InEvent_TempU_Back",
   (const U8 *) "InEvent_1st_detected",
   (const U8 *) "InEvent_2nd_detected",
   (const U8 *) "InEvent_3rd_detected",
   (const U8 *) "InEvent_Back_to_TripA",
   (const U8 *) "InEvent_Dooropen_IGNOFF",
   (const U8 *) "InEvent_External_Lamp_IGNOFF",
   (const U8 *) "InEvent_ETM_11sec_timeout",
   (const U8 *) "InEvent_ETM_30sec_timeout",
   (const U8 *) "InEvent_ETM_Entry",
   (const U8 *) "InEvent_RPAS_screen_ACTIVE",
   (const U8 *) "InEvent_RPAS_screen_timeout",
   (const U8 *) "InEvent_Drive_Summary",
   (const U8 *) "InEvent_Drive_Sum_Timeout",
   (const U8 *) "InEvent_BS6_permanent_alert_inactive",
   (const U8 *) "InEvent_IOcontrol_TFT",
   (const U8 *) "InEvent_IOcontrol_Exit",
   (const U8 *) "InEvent_WH_Backdoor_doorClose",
   (const U8 *) "InEvent_WH_Backdoor_dooropen",
   (const U8 *) "InEvent_WH_Codriver_doorclose",
   (const U8 *) "InEvent_WH_Codriver_dooropen",
   (const U8 *) "InEvent_WH_Hood_doorclose",
   (const U8 *) "InEvent_WH_Hood_dooropen",
   (const U8 *) "InEvent_WH_RearRight_doorclose",
   (const U8 *) "InEvent_WH_RearRight_dooropen",
   (const U8 *) "InEvent_WH_Rearleft_dooropen",
   (const U8 *) "InEvent_WH_Rearleft_doorclose",
   (const U8 *) "InEvent_WH_driver_doorclose",
   (const U8 *) "InEvent_WH_driver_dooropen",
   (const U8 *) "InEvent_WH_Alldoorclose",
   (const U8 *) "InEvent_All_Pixel_OFF",
   (const U8 *) "InEvent_All_Pixel_ON",
   (const U8 *) "InEvent_All_Pixel_White",
   (const U8 *) "InEvent_All_Pixel_checker",
   (const U8 *) "InEvent_All_Pixel_checker_Inverted",
   (const U8 *) "InEvent_start_animation",
   (const U8 *) "InEvent_Setting_Timeout",
   (const U8 *) "InEvent_HDC_Active",
   (const U8 *) "InEvent_HDC_Passive",
   (const U8 *) "InEvent_HDC_Ready",
   (const U8 *) "InEvent_4X4_alert_trigger",
   (const U8 *) "InEvent_4x4_Animation_Start",
   (const U8 *) "InEvent_4x4_Animation_Stop",
   (const U8 *) "InEvent_4x4_2WD_Animation_Start",
   (const U8 *) "InEvent_VI_4X4_2WD_Animation_Start",
   (const U8 *) "InEvent_VI_4X4_2WD_Animation_Stop",
   (const U8 *) "InEvent_VI_4X4_4WD_Animation_Start",
   (const U8 *) "InEvent_VI_4X4_4WD_Animation_Stop",
   (const U8 *) "InEvent_EcoScore_Jump",
   (const U8 *) "InEvent_EcoScore_Return",
   (const U8 *) "InEvent_TDMS_alert_trigger",
   (const U8 *) "InEvent_AT_Alert_Detected",
   (const U8 *) "InEvent_AT_Alert_Timeout",
   (const U8 *) "LAST_IN_EVENT",
   (const U8 *) "OutEvent_mainscreen",
   (const U8 *) "OutEvent_REGEN_started",
   (const U8 *) "OutEvent_REGEN_not_started",
   (const U8 *) "OutEvent_REGEN_requested",
   (const U8 *) "OutEvent_REGEN_completed_popup",
   (const U8 *) "OutEvent_REGEN_aborted_popup",
   (const U8 *) "OutEvent_logo_completed",
   (const U8 *) "OutEvent_Alert_started",
   (const U8 *) "OutEvent_Alert_Escape",
   (const U8 *) "Out_EV_AFE_Reset",
   (const U8 *) "Out_EV_Setting_clock",
   (const U8 *) "Out_EV_Timeformat",
   (const U8 *) "Out_AFE_first_row",
   (const U8 *) "Out_AFE_second_row",
   (const U8 *) "Out_AFE_third_row",
   (const U8 *) "OutEvent_AFE_1st_row_selected",
   (const U8 *) "OutEvent_AFE_2nd_row_selected",
   (const U8 *) "OutEvent_AFE_3rd_row_selected",
   (const U8 *) "Out_AFEMenu_Entered",
   (const U8 *) "Out_TimeF_first_row",
   (const U8 *) "Out_TimeF_second_row",
   (const U8 *) "Out_TimeF_third_row",
   (const U8 *) "OutEvent_TimeF_1st_row_selected",
   (const U8 *) "OutEvent_TimeF_2nd_row_selected",
   (const U8 *) "Out_TimeFMenu_Entered",
   (const U8 *) "Out_EV_Clock",
   (const U8 *) "Out_EV_TimeSetting",
   (const U8 *) "OutEvent_TimeS_Back",
   (const U8 *) "OutEvent_TimeSet_Entered",
   (const U8 *) "OutEvent_start_1st_increment",
   (const U8 *) "OutEvent_start_2nd_increment",
   (const U8 *) "OutEvent_start_3rd_increment",
   (const U8 *) "OutEvent_start_4th_increment",
   (const U8 *) "OutEvent_start_5th_increment",
   (const U8 *) "OutEvent_Backsave_time",
   (const U8 *) "OutEvent_WH_Page",
   (const U8 *) "OutEvent_WH_Back",
   (const U8 *) "Out_TempU_first_row",
   (const U8 *) "Out_TempU_second_row",
   (const U8 *) "Out_TempU_third_row",
   (const U8 *) "OutEvent_TempU_1st_row_selected",
   (const U8 *) "OutEvent_TempU_2nd_row_selected",
   (const U8 *) "OutEvent_TempU_Entered",
   (const U8 *) "OutEvent_TFT_Check_start",
   (const U8 *) "OutEvent_TFT_Check_end",
   (const U8 *) "OutEvent_PAB_deselected",
   (const U8 *) "OutEvent_PAB_row",
   (const U8 *) "OutEvent_PAB_selected",
   (const U8 *) "OutEvent_Setting_Entered",
   (const U8 *) "OutEvent_Setting_Back",
   (const U8 *) "OutEvent_PAB_disable_No_detected",
   (const U8 *) "OutEvent_PAB_disable_Yes_detected",
   (const U8 *) "OutEvent_PAB_enable_No_detected",
   (const U8 *) "OutEvent_PAB_enable_Yes_detected",
   (const U8 *) "OutEvent_Drive_Sum_Alert_Escape",
   (const U8 *) "OutEvent_Drive_Sum_Alert_started",
   (const U8 *) "OutEvent_Skip_EngOilPressureLow",
   (const U8 *) "OutEvent_Skip_EngTempHigh",
   (const U8 *) "OutEvent_Skip_LowFuel",
   (const U8 *) "OutEvent_Skip_CheckEng",
   (const U8 *) "OutEvent_Skip_LowBrakefluid",
   (const U8 *) "OutEvent_Skip_ParkBrake",
   (const U8 *) "OutEvent_Skip_PABoff",
   (const U8 *) "OutEvent_Skip_DPFclogged",
   (const U8 *) "OutEvent_Skip_RegenRequired",
   (const U8 *) "OutEvent_Skip_RegenRequested",
   (const U8 *) "OutEvent_Skip_RegenStarted",
   (const U8 *) "OutEvent_Skip_RegenNotStarted",
   (const U8 *) "OutEvent_Skip_RegenCompleted",
   (const U8 *) "OutEvent_Skip_RegenAborted",
   (const U8 *) "OutEvent_Skip_DoorOpen",
   (const U8 *) "OutEvent_Skip_EBDmalfunction",
   (const U8 *) "OutEvent_Skip_ABSmalfunction",
   (const U8 *) "OutEvent_Skip_ESPmalfunction",
   (const U8 *) "OutEvent_Skip_RPASsupplyFailure",
   (const U8 *) "OutEvent_Skip_RKE_Battery_Low",
   (const U8 *) "OutEvent_Skip_WaterInFuel",
   (const U8 *) "OutEvent_Skip_ServiceReminder",
   (const U8 *) "OutEvent_Skip_FuncSafety",
   (const U8 *) "OutEvent_Skip_DEF_Dosing_Malfunction",
   (const U8 *) "OutEvent_Skip_DEF_Dosing_Malfunction_Engine",
   (const U8 *) "OutEvent_Skip_DEF_Level_Low",
   (const U8 *) "OutEvent_Skip_DEF_level_Low_Engine_Not_Start",
   (const U8 *) "OutEvent_Skip_Incorrect_DEF_Detected",
   (const U8 *) "OutEvent_Skip_Incorrect_DEF_Detected_Engine",
   (const U8 *) "OutEvent_Skip_No_restart_Check_DEF",
   (const U8 *) "OutEvent_Skip_No_restart_Dose_Malfunction",
   (const U8 *) "OutEvent_Skip_No_restart_Fill_DEF",
   (const U8 *) "OutEvent_Skip_No_start_Check_DEF",
   (const U8 *) "OutEvent_Skip_No_start_Dose_Malfunction",
   (const U8 *) "OutEvent_Skip_No_start_Fill_DEF",
   (const U8 *) "OutEvent_TimeS_Highlight_Confirm",
   (const U8 *) "OutEvent_TimeS_Nohighlight_Confirm",
   (const U8 *) "OutEvent_TimeS_NoHighlight_Back",
   (const U8 *) "OutEvent_0throw_detected",
   (const U8 *) "OutEvent_Selection_IOTFT",
   (const U8 *) "OutEvent_PAB_Entered",
   (const U8 *) "OutEvent_PAB_disable_entered",
   (const U8 *) "OutEvent_PAB_enable_entered",
   (const U8 *) "OutEvent_TripA",
   (const U8 *) "OutEvent_TripAC",
   (const U8 *) "OutEvent_TripA_AVS",
   (const U8 *) "OutEvent_TripA_AVS_DT",
   (const U8 *) "OutEvent_TripA_DT",
   (const U8 *) "OutEvent_TripB",
   (const U8 *) "OutEvent_TripB_AVS",
   (const U8 *) "OutEvent_TripB_AVS_DT",
   (const U8 *) "OutEvent_TripB_DT",
   (const U8 *) "OutEvent_Driveinfo_page",
   (const U8 *) "OutEvent_Digitalclock",
   (const U8 *) "OutEvent_Regen",
   (const U8 *) "OutEvent_FC_AFE",
   (const U8 *) "OutEvent_FC_AFE_IFE",
   (const U8 *) "OutEvent_FC_DTE",
   (const U8 *) "OutEvent_FC_ECO",
   (const U8 *) "OutEvent_FC_IFE",
   (const U8 *) "OutEvent_FC_Page",
   (const U8 *) "OutEvent_start_1st_decrement",
   (const U8 *) "OutEvent_start_3rd_decrement",
   (const U8 *) "OutEvent_start_5th_decrement",
   (const U8 *) "OutEvent_Skip_LeftDRL",
   (const U8 *) "OutEvent_Skip_LeftTurn",
   (const U8 *) "OutEvent_Skip_RightDRL",
   (const U8 *) "OutEvent_Skip_RightTurn",
   (const U8 *) "OutEvent_Skip_TailLamp",
   (const U8 *) "OutEvent_Skip_HornFailure",
   (const U8 *) "OutEvent_Skip_TDMS",
   (const U8 *) "OutEvent_Vehicle_Info_Exit",
   (const U8 *) "OutEvent_AT_Gear_Alert_Started",
   (const U8 *) "drome_MAX_EVENTS"
};
#endif

/*### drome_AppDataPoolValues begin ###*/
typedef enum
{
   drome_Null_Menu_value,  /*  property of widget */
   drome_TimeHour_value,  /*  property of widget */
   drome_TimeMin_value,  /*  property of widget */
   drome_Odo_value,  /*  property of widget */
   drome_OAT_value,  /*  property of widget */
   drome_TripA_value,  /*  property of widget */
   drome_Avg_Speedvalue_A,  /*  property of widget */
   drome_DT_Hrvalue_TripA,  /*  property of widget */
   drome_DT_Minvalue_TripA,  /*  property of widget */
   drome_TripB_value,  /*  property of widget */
   drome_TripAC_value,  /*  property of widget */
   drome_Speedo_value,  /*  property of widget */
   drome_AFE_value,  /*  property of widget */
   drome_IFE_Bargraphvalue,  /*  property of widget */
   drome_Digital_clock_Hrvalue,  /*  property of widget */
   drome_Digital_clock_minvalue,  /*  property of widget */
   drome_MT_AT_value,  /*  property of widget */
   drome_Gear_shift_value,  /*  property of widget */
   drome_Gear_shift_UP,  /*  property of widget */
   drome_Gear_shift_DOWN,  /*  property of widget */
   drome_Gear_shift_BLANK,  /*  property of widget */
   drome_Avg_Speed_logo,  /*  property of widget */
   drome_Avg_Speed_logo_BLANK,  /*  property of widget */
   drome_Avg_Speed_logo_value,  /*  property of widget */
   drome_Driving_Time_logo,  /*  property of widget */
   drome_Driving_Time_logo_BLANK,  /*  property of widget */
   drome_Driving_Time_logo_value,  /*  property of widget */
   drome_Time_Notation_logo,  /*  property of widget */
   drome_Time_Notation_logo_BLANK,  /*  property of widget */
   drome_Time_Notation_logo_value,  /*  property of widget */
   drome_Avg_Speed_Unit,  /*  property of widget */
   drome_Avg_Speed_Unit_BLANK,  /*  property of widget */
   drome_Avg_Speed_Unit_value,  /*  property of widget */
   drome_Driving_Time_Unit,  /*  property of widget */
   drome_Driving_Time_Unit_BLANK,  /*  property of widget */
   drome_Driving_Time_Unit_value,  /*  property of widget */
   drome_DEF_Engine_NotStart_value,  /*  property of widget */
   drome_FEATURE_CODE_REGEN_KB_DOWN,  /*  property of widget decision point */
   drome_Page_scrolling_value,  /*  property of widget Page_scrolling_value */
   drome_Empty_Fuel,  /*  property of widget EmptyFuel Blinking Value */
   drome_Full_Fuel,  /*  property of widget FullFuel Blinking Value */
   drome_AMPM_Unit_value,  /*  property of widget AMPM Conditional image value */
   drome_Digital_Clock_Dots_value,  /*  property of Digital Clock Dots value */
   drome_Full_temp,  /*  property of temp full with blinking condition */
   drome_conditional_barblink,  /*  property of temp 7 bar with blinking condition */
   drome_Fuel_Bargraph_value,  /*  property of Bargraph widget Fuel */
   drome_Temp_Bargraph_value,  /*  property of Bargraph widget Temp */
   drome_DTE_value,  /*  property of DTE  */
   drome_IFE_value,  /*  property of Bargraph widget IFE  */
   drome_DTE_Unit_value,  /*  property of widget DTE  */
   drome_Temp_Unit_CF_value,  /*  property of widget Temp unit  */
   drome_Feature_Code_AFE_IFE,  /*  property of widget Temp unit  */
   drome_Feature_Code_DTE,  /*  property of widget Temp unit  */
   drome_Feature_Code_EcoScore,  /*  property of widget Temp unit  */
   drome_Feature_Code_TripA_AvgSpeed_Drivetime,  /*  property of widget Temp unit  */
   drome_Feature_Code_TripB_AvgSpeed_Drivetime,  /*  property of widget Temp unit  */
   drome_Individual_Door_Open_value,  /*  property of widget Temp unit  */
   drome_Feature_Code_Digital_Speedo,  /*  property of widget Temp unit  */
   drome_Avg_Speedvalue_B,  /*  property of widget */
   drome_DT_Hrvalue_TripB,  /*  property of widget */
   drome_DT_Minvalue_TripB,  /*  property of widget */
   drome_InstEco_value,  /*  property of widget */
   drome_Door_Codriver_Index,  /*  property of widget */
   drome_Door_Driver_Index,  /*  property of widget */
   drome_Door_RearLeft_Index,  /*  property of widget */
   drome_Door_RearRight_Index,  /*  property of widget */
   drome_Door_Back_Index,  /*  property of widget */
   drome_Door_Hood_Index,  /*  property of widget */
   drome_Feature_Code_Trip_AC,  /*  property of widget */
   drome_Diag_Unit_Selection,  /*  property of widget */
   drome_Setting_imageIndex,  /*  property of widget */
   drome_Setting_imageList,  /*  property of widget */
   drome_Setting_textDynamic,  /*  property of widget */
   drome_Setting_Speed_present,  /*  property of widget */
   drome_Setting_AFE_imageIndex,  /*  property of widget */
   drome_Setting_AFE_imageList,  /*  property of widget */
   drome_Setting_AFE_textDynamic,  /*  property of widget */
   drome_Clock_typeimageIndex,  /*  property of widget */
   drome_Setting_Clock_textDynamic,  /*  property of widget */
   drome_Setting_ClockT_imageIndex,  /*  property of widget */
   drome_Timeformat12_imageIndex,  /*  property of widget */
   drome_Timeformat24_imageIndex,  /*  property of widget */
   drome_AFE_row_selection,  /*  property of widget */
   drome_Setting_Timevalue_HR_1,  /*  property of widget */
   drome_Setting_Timevalue_HR_0,  /*  property of widget */
   drome_Setting_Timevalue_MIN_1,  /*  property of widget */
   drome_Setting_Timevalue_MIN_0,  /*  property of widget */
   drome_AMPM_UnitSet_value,  /*  property of widget */
   drome_WH_Page1_imageIndex,  /*  property of widget */
   drome_WH_Page2_imageIndex,  /*  property of widget */
   drome_WH_Page3_imageIndex,  /*  property of widget */
   drome_WH_Page4_imageIndex,  /*  property of widget */
   drome_WH_Page5_imageIndex,  /*  property of widget */
   drome_WH_Page6_imageIndex,  /*  property of widget */
   drome_WH_Page7_imageIndex,  /*  property of widget */
   drome_WH_Page8_imageIndex,  /*  property of widget */
   drome_WH_Page9_imageIndex,  /*  property of widget */
   drome_WH_Page10_imageIndex,  /*  property of widget */
   drome_WH_Page11_imageIndex,  /*  WH_Page11_imageIndex */
   drome_WH_Page12_imageIndex,  /*  WH_Page12_imageIndex */
   drome_WH_Page13_imageIndex,  /*  property of widget */
   drome_WH_Page14_imageIndex,  /*  property of widget */
   drome_WH_Page15_imageIndex,  /*  WH_Page15_imageIndex */
   drome_WH_Page16_imageIndex,  /*  WH_Page16_imageIndex */
   drome_WH_Page17_imageIndex,  /*  WH_Page17_imageIndex */
   drome_WH_Page18_imageIndex,  /*  WH_Page18_imageIndex */
   drome_WH_Page19_imageIndex,  /*  WH_Page19_imageIndex */
   drome_WH_Page20_imageIndex,  /*  WH_Page20_imageIndex */
   drome_WH_Page21_imageIndex,  /*  WH_Page21_imageIndex */
   drome_WH_Page22_imageIndex,  /*  WH_Page22_imageIndex */
   drome_WH_Page23_imageIndex,  /*  WH_Page23_imageIndex */
   drome_WH_Page24_imageIndex,  /*  WH_Page24_imageIndex */
   drome_WH_Page25_imageIndex,  /*  WH_Page25_imageIndex */
   drome_WH_Page26_imageIndex,  /*  WH_Page26_imageIndex */
   drome_WH_Page27_imageIndex,  /*  WH_Page27_imageIndex */
   drome_WH_Page28_imageIndex,  /*  WH_Page28_imageIndex */
   drome_Total_WH_Pages1,  /*  property of widget */
   drome_Next_WH_Pages,  /*  property of widget */
   drome_ETM_TFT_Test_Index,  /*  property of widget */
   drome_ETM_SW_NVM_Version_Index,  /*  property of widget */
   drome_ETM_SW_ROM_Version_Index,  /*  property of widget */
   drome_ETM_Fuel_Rad_Count_index,  /*  property of widget */
   drome_ETM_Fuel_RAW_ADC_Index,  /*  property of widget */
   drome_ETM_Fuel_Supply_ADC_index,  /*  property of widget */
   drome_ETM_BATT_SUPPLY_ADC_Index,  /*  property of widget */
   drome_ETM_DTC_Check_Index,  /*  property of widget */
   drome_RPAS_RCenter_imageIndex,  /*  property of widget */
   drome_RPAS_LCenter_imageIndex,  /*  property of widget */
   drome_RPAS_RLeft_imageIndex,  /*  property of widget */
   drome_RPAS_RRight_imageIndex,  /*  property of widget */
   drome_RPAS_DIST_value,  /*  property of widget */
   drome_RPAS_sect_zone_calb,  /*  property of widget RPAS_sect_zone_calb */
   drome_RPAS_DIST_text_display,  /*  property of widget */
   drome_RPAS_All_sensor_error,  /*  property of widget */
   drome_Regen_page_index,  /*  property of widget */
   drome_Feature_Code_AFER_PAB,  /*  property of widget */
   drome_PAB_Disable_select_Index,  /*  property of widget */
   drome_PAB_Enable_select_Index,  /*  property of widget */
   drome_Odoerror_imageIndex,  /*  property of widget */
   drome_Odoerror_TripA_imageIndex,  /*  property of widget Odoerror_TripA_imageIndex */
   drome_Odoerror_TripB_imageIndex,  /*  property of widget Odoerror_TripB_imageIndex  */
   drome_Drive_Sum_AvgSpeed,  /*  property of widget Drive_Sum_AvgSpeed */
   drome_Drive_Sum_Distance,  /*  property of widget Drive_Sum_Distance  */
   drome_Drive_Sum_DTE,  /*  property of widget Drive_Sum_DTE */
   drome_Drive_Sum_DT_Hrvalue,  /*  property of widget Drive_Sum_DT_Hrvalue  */
   drome_Drive_Sum_DT_Minvalue,  /*  property of widget Drive_Sum_DT_Minvalue */
   drome_Aerrow_Down_Index,  /*  property of widget Aerrow_Down_Index */
   drome_Welcome_imageIndex,  /*  property of widget Welcome_imageIndex */
   drome_IOcontrol_TFT_Index,  /*  property of widget IOcontrol_TFT_Index */
   drome_TimeSet_BackIndex,  /*  property of widget TimeSet_BackIndex */
   drome_TimeSet_ConfirmIndex,  /*  property of widget TimeSet_ConfirmIndex */
   drome_No_warning_index,  /*  property of widget No_warning_index */
   drome_Aerrow_UP_Index,  /*  property of widget Aerrow_UP_Index */
   drome_Time_SHr_Zeroconcat_Index,  /*  property of widget Time_SHr_Zeroconcat_Index */
   drome_Time_SMin_Zeroconcat_Index,  /*  property of widget Time_SMin_Zeroconcat_Index */
   drome_IFE_MidDisplay_value,  /*  property of widget IFE_MidDisplay_value */
   drome_IFE_MaxDisplay_value,  /*  property of widget IFE_MaxDisplay_value */
   drome_REGEN_HoldOk_imageIndex,  /*  property of widget IFE_MaxDisplay_value */
   drome_Default_page,  /*  property of widget Default_page */
   drome_BSVI_ImageIndex,  /*  property of widget BSVI_ImageIndex */
   drome_WH_BSVI_Errorcode,  /*  property of widget WH_BSVI_Errorcode */
   drome_BS_VI_E12E13,  /*  property of widget BS_VI_E12E13 */
   drome_BS_VI_E22E23,  /*  property of widget BS_VI_E22E23 */
   drome_BS_VI_E32E33,  /*  property of widget BS_VI_E32E33 */
   drome_4X4_Text_Index,  /*  property of widget 4X4_Text_Index */
   drome_4WD_Framerate,  /*  property of widget 4WD_Framerate */
   drome_4WD_Visiblity,  /*  property of widget 4WD_Visiblity */
   drome_2WD_Visiblity,  /*  property of widget 2WD_Visiblity */
   drome_VI_2WD_Visiblity,  /*  property of widget VI_2WD_Visiblity */
   drome_VI_4WD_Visiblity,  /*  property of widget VI_4WD_Visiblity */
   drome_VI_4WD_Framerate,  /*  property of widget VI_4WD_Framerate */
   drome_Feature_Code_VI_4X4,  /*  property of widget Feature_Code_VI_4X4 */
   drome_4WD_not_engaged,  /*  property of widget 4WD_not_engaged */
   drome_Menu_Bar_FC_Index,  /*  property of widget Menu_Bar_FC_Index */
   drome_Eco_Leaf_Value,  /*  property of widget Eco_Leaf_Value */
   drome_Eco_Error_Code,  /*  property of widget Eco_Error_Code */
   drome_Feature_Code_Power_Torque,  /*  property of widget Feature_Code_Power_Torque */
   drome_Power_Rotation_Angle,  /*  property of widget Power_Rotation_Angle */
   drome_Torque_Rotation_Angle,  /*  property of widget Torque_Rotation_Angle */
   drome_Power_Torque_Error_Code,  /*  property of widget Power_Torque_Error_Code */
   drome_Feature_Code_Roll_Pitch,  /*  property of widget Feature_Code_Roll_Pitch */
   drome_Roll_Rotation_Angle,  /*  property of widget Roll_Rotation_Angle */
   drome_Pitch_Rotation_Angle,  /*  property of widget Pitch_Rotation_Angle */
   drome_Roll_Pitch_Error_Code,  /*  property of widget Roll_Pitch_Error_Code */
   drome_Roll_Safety_Limit,  /*  property of widget Roll_Safety_Limit */
   drome_Pitch_Safety_Limit,  /*  property of widget Pitch_Safety_Limit */
   drome_Compass_Direction_Index,  /*  property of widget Compass_Direction_Index */
   drome_Compass_Error_Code,  /*  property of widget Compass_Error_Code */
   drome_Feature_Code_Compass,  /*  property of widget Feature_Code_Compass */
   drome_Altimeter_Dist_Value,  /*  property of widget Altimeter_Dist_Value */
   drome_Altimeter_Error_Code,  /*  property of widget Altimeter_Error_Code */
   drome_Feature_Code_Altimeter,  /*  property of widget Feature_Code_Altimeter */
   drome_Feature_Code_TDMS,  /*  property of widget Feature_Code_TDMS */
   drome_TDMS_Error_Code,  /*  property of widget TDMS_Error_Code */
   drome_TDMS_Rotation_Angle,  /*  property of widget TDMS_Rotation_Angle */
   drome_TDMS_Rotation_Value,  /*  property of widget TDMS_Rotation_Value */
   drome_TDMS_Turn_Side,  /*  property of widget TDMS_Turn_Side */
   drome_Pitch_Safety_Limit_txt,  /*  property of widget Pitch_Safety_Limit_txt */
   drome_Roll_Safety_Limit_txt,  /*  property of widget Roll_Safety_Limit_txt */
   drome_Media_Icon_Change,  /*  property of widget Media_Icon_Change */
   drome_Media_AM_Band_Info,  /*  property of widget Media_AM_Band_Info */
   drome_Media_AM_FM_Text,  /*  property of widget Media_AM_FM_Text */
   drome_Media_Song_Name,  /*  property of widget Media_Song_Name */
   drome_Media_Center_Patch,  /*  property of widget Media_Center_Patch */
   drome_Media_Play_Back,  /*  property of widget Media_Play_Back */
   drome_Feature_Code_Media,  /*  property of widget Feature_Code_Media */
   drome_Media_Error_Code,  /*  property of widget Media_Error_Code */
   drome_FM_Band_Value,  /*  property of widget FM_Band_Value */
   drome_AM_Freq_Range_visible,  /*  property of widget AM_Freq_Range_visible */
   drome_FM_Freq_Range_visible,  /*  property of widget FM_Freq_Range_visible */
   drome_TDMS_Alert_Zero_Degree_imageIndex,  /*  property of widget TDMS_Alert_Zero_Degree_imageIndex */
   drome_TDMS_MainScreen_Zero_Degree_imageIndex,  /*  property of widget TDMS_MainScreen_Zero_Degree_imageIndex */
   drome_Vehicle_Info_UP_Down_Arrow_Visible,  /*  property of widget Vehicle_Info_UP_Down_Arrow_Visible */
   drome_AT_Alert_imageIndex,  /*  property of widget AT_Alert_imageIndex */
   drome_TDMS_Left_Wheel_X,  /*  property of widget TDMS_Left_Wheel_X */
   drome_TDMS_Left_Wheel_Y,  /*  property of widget TDMS_Left_Wheel_Y */
   drome_TDMS_Right_Wheel_X,  /*  property of widget TDMS_Right_Wheel_X */
   drome_TDMS_Right_Wheel_Y,  /*  property of widget TDMS_Right_Wheel_Y */
   drome_TDMS_Left_Wheel_Alert_X,  /*  property of widget TDMS_Left_Wheel_Alert_X */
   drome_TDMS_Left_Wheel_Alert_Y,  /*  property of widget TDMS_Left_Wheel_Alert_Y */
   drome_TDMS_Right_Wheel_Alert_X,  /*  property of widget TDMS_Right_Wheel_Alert_X */
   drome_TDMS_Right_Wheel_Alert_Y,  /*  property of widget TDMS_Right_Wheel_Alert_Y */
   drome_MAX_DATAPOOL
} drome_AppDataPoolValues;
/*### drome_AppDataPoolValues end ###*/

/* Indexed images */
typedef enum
{
   drome_Gear_ind_autoMan_selIx_ix=0
} drome_ConditionalIndexImages;


typedef enum 
{
   drome_Gear_ind_autoMan_selIx_man=0,
   drome_Gear_ind_autoMan_selIx_auto=1
}  drome_Gear_ind_autoMan_selIx;



/* Indexed texts */
typedef enum
{
   drome_Oil_measure_ix=0
} drome_ConditionalIndexTexts;


typedef enum 
{
   drome_Oil_measure_Oil_measure_1=0   /* Text.. */
}  drome_Oil_measure;



/*
**=====================================================================================
** Public data declarations
**=====================================================================================
*/


/*
**=====================================================================================
** Public function declarations and macros
**=====================================================================================
*/
/**
\ } // End DROMEIF
*/
#endif /* #ifdef DRCONF_H */
