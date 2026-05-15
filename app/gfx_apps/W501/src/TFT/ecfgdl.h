/*
***************************************************************************************
***************************************************************************************
***
***     File: ecfgdl.h  
***
***     Project: SRE Common Software Component
***
***     Copyright (C) Stoneridge Electronics AB, 1990 - $Date:: 2015#$
***
***     This file is a part of the Stoneridge Common Software Component Repository
***     and may only be used in accordance with the terms and conditions stipulated
***     in the agreement contract under which it has been supplied.
***
***************************************************************************************
**************************************************************************************/
/** \file

   \ingroup    SRE_COMMON_SOFTWARE_COMPONENT

   \Subject    Public configuration of the data layer

   \Reference  1231/001-05368 SRE Common Platform Architecture

   \Abstract
   
   This header file contains the Public configuration of the data layer.


   \Version Id:
   
   "$Id: ecfgdl.h 591 2015-09-08 09:09:59Z STH $"


@{
***************************************************************************************
*/




/*
**====================================================================================
** Double inclusion protection
**====================================================================================
*/

#ifndef ECFGDL_H
#define ECFGDL_H


/*
**====================================================================================
** Imported definitions
**====================================================================================
*/

#include "typedefs.h"
#include "dldef.h"


/*
**====================================================================================
** Data id definitions
**====================================================================================
*/

/*
** Place all data id's between the start and end marker of each type.
** Keep dummy id if type is not used, otherwise replace it with an id.
*/
typedef enum {
/*
** All 2-bit data id
*/
   dl_START_MARKER_2BIT,
   dl_NULL,
   dl_NVMM_DIAG_ACK,
   dl_NVMM_TIME_ACK,
   dl_NVMM_DTCMGR_ACK,
   dl_BUTTON_1,
   dl_BUTTON_2,
   dl_BUTTON_3,
   dl_BUTTON_4,
   dl_TELLTALE_BACKLIGHT_ENABLE,
#if 1
   dl_TT_D1,
   dl_TT_D2,
   dl_TT_D3,
   dl_TT_D4,
   dl_TT_D5,
   dl_TT_D6,
   dl_TT_D7,
   dl_TT_D8,
   dl_TT_D9,
   dl_TT_D10,
   dl_TT_D11,
   dl_TT_D12,
   dl_TT_D13,
   dl_TT_D14,
   dl_TT_D15,
   dl_TT_D16,
   dl_TT_D17G,
   dl_TT_D17Y,
   dl_TT_D17R,
   dl_TT_D18G,
   dl_TT_D18Y,
   dl_TT_D18R,
   dl_TT_D19G,
   dl_TT_D19Y,
   dl_TT_D19R,
   dl_TT_D22Y,
   dl_TT_D22R,
   dl_TT_D23G,
   dl_TT_D23Y,
   dl_TT_D23R,
   dl_TT_D24G,
   dl_TT_D24Y,
   dl_TT_D24R,
   dl_TT_D25,
   dl_TT_D26,
   dl_TT_D27,
   dl_TT_D28,
   dl_TT_D29,
   dl_TT_D1_STATUS,
   dl_TT_D2_STATUS,
   dl_TT_D3_STATUS,
   dl_TT_D4_STATUS,
   dl_TT_D5_STATUS,
   dl_TT_D6_STATUS,
   dl_TT_D7_STATUS,
   dl_TT_D8_STATUS,
   dl_TT_D9_STATUS,
   dl_TT_D10_STATUS,
   dl_TT_D11_STATUS,
   dl_TT_D12_STATUS,
   dl_TT_D13_STATUS,
   dl_TT_D14_STATUS,
   dl_TT_D15_STATUS,
   dl_TT_D16_STATUS,
   dl_TT_D17G_STATUS,
   dl_TT_D17Y_STATUS,
   dl_TT_D17R_STATUS,
   dl_TT_D18G_STATUS,
   dl_TT_D18Y_STATUS,
   dl_TT_D18R_STATUS,
   dl_TT_D19G_STATUS,
   dl_TT_D19Y_STATUS,
   dl_TT_D19R_STATUS,
   dl_TT_D22Y_STATUS,
   dl_TT_D22R_STATUS,
   dl_TT_D23G_STATUS,
   dl_TT_D23Y_STATUS,
   dl_TT_D23R_STATUS,
   dl_TT_D24G_STATUS,
   dl_TT_D24Y_STATUS,
   dl_TT_D24R_STATUS,
   dl_TT_D25_STATUS,
   dl_TT_D26_STATUS,
   dl_TT_D27_STATUS,
   dl_TT_D28_STATUS,
   dl_TT_D29_STATUS,
#endif

   dl_TT_CAB_TILT,
   dl_TT_HIGH_RENAGE_GEAR,
   dl_TT_AdBLUE,
   dl_TT_RETARDER,
   dl_TT_REDUCED_SPEED,
   dl_ADBLUE_WARNING,

   dl_END_MARKER_2BIT,
   
/*
** All 2-bit data id with status
*/
   dl_START_MARKER_2BIT_WS,
   dl_DUMMY_2BIT_WITH_STATUS,
   dl_END_MARKER_2BIT_WS,
   
/*
** All 4-bit data id
*/
   dl_START_MARKER_4BIT,
   dl_SUMMY_4BIT,
   dl_END_MARKER_4BIT,
   
/*
** All 4-bit data id with status
*/
   dl_START_MARKER_4BIT_WS,
   dl_SUMMY_4BIT_WITH_STATUS,
   dl_END_MARKER_4BIT_WS,
   
/*
** All U8 data id
*/
   dl_START_MARKER_U8,
   dl_SYSTEM_STATE,
   dl_GAUGE_SYS_STATE_TRANSITION_ACK,
   dl_IO_SYS_STATE_TRANSITION_ACK,
   dl_J1939_SYS_STATE_TRANSITION_ACK,
   dl_DIAG_SYS_STATE_TRANSITION_ACK,
   dl_GUI_SYS_STATE_TRANSITION_ACK,
   dl_USRLOGIC_SYS_STATE_TRANSITION_ACK,
   dl_DTCMGR_SYS_STATE_TRANSITION_ACK,
   dl_NVMMGR_SYS_STATE_TRANSITION_ACK,
   dl_TESTSRV_SYS_STATE_TRANSITION_ACK,
   dl_RTC_SYS_STATE_TRANSITION_ACK,
   dl_SOUND_CHANNEL,
   dl_SOUND_ACTIVE,
   dl_SOUND_PLAY,
   dl_TFT_BACKLIGHT,
   dl_GAUGE_BACKLIGHT,
   dl_INTERNAL_TEMP,
   dl_APG1,
   dl_APG2,
   dl_AdBlue,
   dl_DISPLAY_BRIGHTNESS,
   dl_DROME_EVENT,
   dl_END_MARKER_U8,
   
/*
** All U8 data id with status
*/
   dl_START_MARKER_U8_WS,
   dl_DUMMY_U8_WITH_STATUS,
   dl_END_MARKER_U8_WS,
   
/*
** All U16 data id
*/
   dl_START_MARKER_U16,
   dl_SOUND_NOF_REPEATS,
   dl_SPEED,
   dl_RPM,
   dl_KL30,
#if 0
   dl_APG1,
   dl_APG2,
#endif
   dl_END_MARKER_U16,
   
/*
** All U16 data id with status
*/
   dl_START_MARKER_U16_WS,
   dl_DUMMY_U16_WITH_STATUS,
   dl_END_MARKER_U16_WS,
   
/*
** All U32 data id
*/
   dl_START_MARKER_U32,
   dl_SOUND_FILE_ID,
   dl_TIME_DATE,
   dl_SET_NEW_TIME_DATE_REQ,
   dl_ODO,
   dl_TRIP,
   dl_END_MARKER_U32,
   
/*
** All U32 data id with status
*/
   dl_START_MARKER_U32_WS,
   dl_DUMMY_U32_WITH_STATUS,
   dl_END_MARKER_U32_WS,
   
/*
** All structure types
*/
   dl_START_MARKER_STRUCT_TYPE,
   dl_GAUGE0_CALIB_PARAMS,
   dl_GAUGE1_CALIB_PARAMS,
   dl_NVMM_DIAG_STRUCT,
   dl_NVMM_TIME_STRUCT,
   dl_NVMM_DTCMGR_STRUCT,
   dl_END_MARKER_STRUCT_TYPE
} dl_DataId;


/*
**====================================================================================
** Definitions of data readers.
**====================================================================================
*/

typedef enum {
   dl_DR_ANONYMOUS   = 0x0000U,
   dl_DR_GAUGE       = 0x0001U,
   dl_DR_IO          = 0x0002U,
   dl_DR_J1939       = 0x0004U,
   dl_DR_DIAG        = 0x0008U,
   dl_DR_GUI         = 0x0010U,
   dl_DR_DTCMGR      = 0x0020U,
   dl_DR_CFGMGR      = 0x0040U,
   dl_DR_TESTSRV     = 0x0080U,
   dl_DR_SYSMGR      = 0x0100U,
   dl_DR_NVMHANDLER  = 0x0200U,
   dl_DR_USRLOGIC    = 0x0400U,
   dl_DR_RTC         = 0x0800U
   
} dl_Reader;

#ifdef DL_PRIVATE_CONFIGURATION
   /*
   ** Define data type used internally by the data layer component.
   ** Make sure all defined readers (bit positions) fits within selected data type!
   */
   typedef U16 ReaderFlagType;
#endif



/*
**====================================================================================
** Structure data types
**====================================================================================
*/

typedef struct {
   U8  gauge_phase;
   U8  parking_pos;
   U16  scale_offset;
   U8  center_angle;
   U8  center_dist;
   U8  hysteresis;
   U8  layout;
} dl_GaugeCalibParams;

typedef struct {
   U16  parameter_id;
   U16  size;
   void*  data_ptr;
   U8  action;
} dl_NVMemoryParams;
/*
**====================================================================================
** Generate variables for each data id of struct type
**====================================================================================
*/

#ifdef DL_PRIVATE_CONFIGURATION
   /*
   ** Generate variables for each data id of struct type
   */
   MAKE_STRUCT_DATA(dl_GaugeCalibParams, dl_GAUGE0_CALIB_PARAMS);
   MAKE_STRUCT_DATA(dl_GaugeCalibParams, dl_GAUGE1_CALIB_PARAMS);
   MAKE_STRUCT_DATA(dl_NVMemoryParams, dl_NVMM_DIAG_STRUCT);
   MAKE_STRUCT_DATA(dl_NVMemoryParams, dl_NVMM_TIME_STRUCT);
   MAKE_STRUCT_DATA(dl_NVMemoryParams, dl_NVMM_DTCMGR_STRUCT);
   
   /*
   ** Connect each struct data id with the struct data type
   ** OBS! The order which the data id appears in the table
   ** must match the order they appear in the dl_DataId enum!
   */
   START_OF_ID_TO_STRUCT_TABLE {
      /*
      **                   Data id                 Data type
      */
      CONNECT_ID_TO_STRUCT(dl_GAUGE0_CALIB_PARAMS,  dl_GaugeCalibParams),
      CONNECT_ID_TO_STRUCT(dl_GAUGE1_CALIB_PARAMS,  dl_GaugeCalibParams),
      CONNECT_ID_TO_STRUCT(dl_NVMM_DIAG_STRUCT, dl_NVMemoryParams),
      CONNECT_ID_TO_STRUCT(dl_NVMM_TIME_STRUCT, dl_NVMemoryParams),
      CONNECT_ID_TO_STRUCT(dl_NVMM_DTCMGR_STRUCT, dl_NVMemoryParams),
      END_OF_ID_TO_STRUCT_TABLE
   };
#endif

#endif


/************************************************************************************/
/** @}
************************************ End of file ************************************/
