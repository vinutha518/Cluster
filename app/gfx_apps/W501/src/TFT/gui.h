/*
***************************************************************************************
***************************************************************************************
***
***     File: gui.h
***
***     Project: YMER
***
***     Copyright (C) Stoneridge Electronics AB, 1990 - $Date:: 2013#$
***
***************************************************************************************
**************************************************************************************/
/** \file

   \ingroup    RT

   \Subject    Interface to drome

   \Reference  YMER SW ARCHITECTURE DESIGN

   \Abstract

   This header file contains the Public definitions and declarations of module gui.c.


   \Version Id:

   "$Id: gui.h 147 2013-07-04 07:40:06Z STONERIDGE\HAW $"


@{
***************************************************************************************
*/




/*
**====================================================================================
** Double inclusion protection
**====================================================================================
*/

#ifndef GUI_H
#define GUI_H


/*
**====================================================================================
** Imported definitions
**====================================================================================
*/

#include "r_typedefs.h"
//#include  "rtwtypes.h"

/*
**====================================================================================
** Public type definitions
**====================================================================================
*/




/*
**====================================================================================
** Public constant definitions for external access
**====================================================================================
*/
/*typedef enum {
   BUTTON_JUMP,
   BUTTON_RIGHT,
   BUTTON_DOWN,
   BUTTON_LEFT,
   BUTTON_NONE
}EventBarEnum;*/

typedef enum {
   BUTTON_UP,
   BUTTON_RIGHT,
   BUTTON_DOWN,
   BUTTON_LEFT,
   BUTTON_OK_LONG,
   BUTTON_NONE
} ButtonValueEnum;

#define IMG_STEP 240
/*
**====================================================================================
** Function prototype declarations for external access
**====================================================================================
*/


/*************************************************************************************/
extern U8 button_value;
extern uint16_t  AFEDispVal;             /* '<Root>/U16_AFEDispVal' */
extern uint8_t codoor_index;
extern uint8_t Codriver_index;
extern uint8_t Driver_index;
extern uint8_t Rearleft_index;
extern uint8_t RearRight_index;
extern uint8_t Backd_index;
extern uint8_t Hood_index ;
extern _Bool Codoor_set;
extern _Bool driverdoor_set;
extern _Bool Rearleftdoor_set;
extern _Bool RearRightdoor_set;
extern _Bool Backdoor_set;
extern _Bool Codoor_display;
extern _Bool Driver_display;
extern _Bool Rearleft_display;
extern _Bool Rearright_display;
extern _Bool Back_display;
extern
void
gui_taskInit(
      void);

/** Task initialization run before the os is started. Interrupts are disabled.
** \note gui_start() must be called to complete the start of the component.
**************************************************************************************/
extern uint64_t first_screen_timeout;
extern uint64_t start_alart_timeout_5sec;
extern uint8_t logo_completed;
extern uint64_t regen_req_page_timeout;
extern uint8_t regen_page_active;
extern uint8_t regen_page_started;
extern uint8_t gear_to_HMI;
extern uint8_t gear_shift_to_HMI;
extern uint8_t first_screen_timeout_start;
extern uint8_t regen_page_notstarted;
extern uint8_t regen_reqpage_to_notstarted;
extern uint8_t empty_blinkpattern_flag;
extern uint8_t full_blinkpattern_flag;
extern uint8_t toggle_fuel;
//ETM
extern uint64_t ETM_test_start_timeout;
extern uint8_t ETM_TFT_test_index;

extern uint8_t AFE_selected;
extern uint8_t TimeF_selected;
extern uint8_t TempU_selected;
extern uint8_t PAB_selected;
extern uint8_t AM_flag;
extern _Bool Timesetting_completed;
extern _Bool Arrowfill_flag;
extern _Bool ArrowUpfill_flag;
extern uint8_t skip_redraw;
extern _Bool ret_redraw;
extern _Bool draw_viewkey;
extern uint32_t startkey;
extern uint8_t HANDLE_DROME_START;
extern _Bool BS6_permanent_timeout;
extern _Bool BS6_permanent_skip;
extern uint8_t TimeSet_BackIndex;
extern uint8_t TimeSet_ConfirmIndex;
extern _Bool WH_Dooropen_active;
extern _Bool Setting_timeout_start;
extern _Bool Dashes_colon;
extern uint8_t default_page;
extern uint8_t A_flag;
extern uint8_t U_flag;
extern uint32_t drome_count1us;
extern uint8_t index_4X4_Text;
extern _Bool TimeF_selection;
extern _Bool Bootup_Trigger;
extern _Bool WH_dooropen_present;
/*************************************************************************************/
extern
void
gui_taskEntry(
      void);

/** Task entry function
**************************************************************************************/


/*************************************************************************************/
extern
void
gui_taskInit(
      void);

/** Task initialization run before the os is started. Interrupts are disabled.
**************************************************************************************/


/*************************************************************************************/
extern
void
gui_taskEntry(
       void);

/** Task entry function.
**************************************************************************************/
/*extern
void
gui_init(
       void);*/

/** gui_init function.
 * **************************************************************************************/

extern
void
handleDromeState(
       void);

/** gui_start function.
**************************************************************************************/
extern
void
handleButtonPress(
       void);

/** handleButtonPress function.
**************************************************************************************/

#endif


/************************************************************************************/
/** @}
************************************ End of file ************************************/
