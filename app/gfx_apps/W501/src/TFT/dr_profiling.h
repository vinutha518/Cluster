/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                  dr_profiling.h                                 ***
***                                                                                 ***
***                                   Project Drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup XX

  \Subject         DROME -  XX.

  \Reference       None

  \Abstract
          This file implements the profiling function used to measure how much time
        different parts of the dromelib takes to execute.

  \Version history record:

  $Id: dr_profiling.h 

\{
***************************************************************************************
*/

#ifndef DR_PROFILING_H_
#define DR_PROFILING_H_

#define DR_TIME_PROFILING

#ifdef DR_TIME_PROFILING

#include "drhost.h"
#include "dr.h"

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/
typedef struct
{
   U32 time;
   U32 max;
   U32 count;
   U32 tot;
} dr_ProfilingData;

typedef enum
{
   DROME_DO_CYCLIC,
   DRWIDIF_DO_CYCLIC,
   DRWIDIF_DO_CYCLIC_DRST_HNDL_EV_N_LOGICS_0,
   DRWIDBIF_COLLECT_EVENT_HANDLING_WIDGETS,
   DRSMEVH_SEND_EVENTS_2_HNDLRS,
   DRSMIF_SEND_EV_2_SM,
   DRSMEVH_SEND_EVENT_2_WIDGET,
   DRSMIF_CHECK_ACTIVATION_GLOBAL_COND,
   DRANIM_UPDATE_CURRENT_TIME,
   DRWIDIF_DO_CYCLIC_DRST_HNDL_EV_N_LOGICS_1,
   DRWIDIF_DO_CYCLIC_REDRAW_START,
   DRWIDIF_DO_CYCLIC_REDRAW_PROG,
   DRWIDIF_DO_CYCLIC_REDRAW_END,
   DRWIDIF_DO_CYCLIC_REFRESH_START,
   DRWIDIF_DO_CYCLIC_REFRESH_PROG,
   DRWIDIF_DO_CYCLIC_REFRESH_END,
   DRWIDBIF_GET_NEXT_WIDGET,
   INC_WIDGET_ITERATOR,
   DRWIDBIF_GET_BASIC_PROPS,
   DRWIDBIF_GET_SPEC_PROPS,
   DRWIDBIF_GET_WIDGET_TYPE,
   DRAW_WIDGET,
   DRAW_WIDGET_NOT_VISIBLE,
   PUSH_WIDGET,
   POP_WIDGET,
   PUSH_REFERENCE_WIDGET,
   GET_BBOX,
   DRMATH_BBOX_INIT,
   DRWIDIF_DO_CYCLIC_VID_MNGM_0,
   DRWIDIF_DO_CYCLIC_VID_MNGM_1,
   DRWIDIF_DO_CYCLIC_UPDATE_DISPLAY,
   DRWD_DRAW_VIEW,
   DRWD_DRAW_BOX,
   DRWD_DRAW_IMAGE,
   DRWD_DRAW_BARGRAPH_WIDGET,
   DRWDLST_DRAW_LIST_CONTAINER_WIDGET,
   DRWDLST_DRAW_LIST_COLUMN_TEXT_WIDGET,
   DRWDLST_DRAW_LIST_COLUMN_IMAGE_WIDGET,
   DRWDSTW_DRAW_SCROLLING_TEXT,
   DRWDSPRITE_DRAW_SPRITE,
   DRWD_DRAW_SCROLLBAR_WIDGET,
   DRWD_DRAW_TEXT_LABEL_WIDGET,
   DRWD_DRAW_DATA_LABEL_WIDGET,
   DRWD_DRAW_OPACITY_CONTAINER,
   DRWD_DRAW_PANNING_CONTAINER,
   APP_SET_DP_VAL_AS_NUMERIC,
   APP_GET_DP_VAL_AS_NUMERIC,
   APP_GET_DP_VAL_MAX_VAL,
   APP_GET_DP_VAL_AS_STRING,
   CREATE_PRERENDER_CONTEXT,
   DRWD_UPDATE_DISPLAY,
   DRWDTXT_RELOAD_LB_BUFFER,
   DRWDTXT_GET_NEXT_LINE_OF_TEXT,
   DRGODBIF_GET_CHAR_INDEX,
   DRGODBIF_GET_FONT_META_DATA,
   DRGLGMM_GET_IMAGEDATA,
   DRGLGMM_SCRATCH_ALLOCATE,
   DRGLIF_DO_CYCLIC,
   DRGLIF_DO_CYCLIC_CLR_RECT0,
   DRGLIF_DO_CYCLIC_CLR_RECT1,
   DRGLIF_DO_CYCLIC_CLR_RECT2,
   DRGLIF_DO_CYCLIC_CLR_RECT3,
   DRGLIF_DO_CYCLIC_CLR_RECT4,
   DRGLIF_DO_CYCLIC_CLR_RECT5,
   DRGLIF_DO_CYCLIC_DRAW_RECT0,
   DRGLIF_DO_CYCLIC_DRAW_RECT1,
   DRGLIF_DO_CYCLIC_DRAW_RECT2,
   DRGLIF_DO_CYCLIC_DRAW_RECT3,
   DRGLIF_DO_CYCLIC_DRAW_RECT4,
   DRGLIF_DO_CYCLIC_DRAW_RECT5,
   DRGLIF_DO_CYCLIC_DRAW_ARC0,
   DRGLIF_DO_CYCLIC_DRAW_ARC1,
   DRGLIF_DO_CYCLIC_DRAW_ARC2,
   DRGLIF_DO_CYCLIC_DRAW_ARC_TXT_0,
   DRGLIF_DO_CYCLIC_DRAW_ARC_TXT_1,
   DRGLIF_DO_CYCLIC_DRAW_ARC_TXT_2,
   DRGLIF_DO_CYCLIC_PASTE_IMG0,
   DRGLIF_DO_CYCLIC_PASTE_IMG1,
   DRGLIF_DO_CYCLIC_PASTE_IMG2,
   DRGLIF_DO_CYCLIC_PASTE_IMG3,
   DRGLIF_DO_CYCLIC_PASTE_IMG4,
   DRGLIF_DO_CYCLIC_PASTE_IMG5,
   DRGLIF_DO_CYCLIC_PASTE_IMG6,
   DRGLIF_DO_CYCLIC_PASTE_IMG7,
   DRGLIF_DO_CYCLIC_PASTE_IMG8,
   DRGLIF_DO_CYCLIC_WR_TXT0,
   DRGLIF_DO_CYCLIC_TF_TXT0,
   DRGLIF_DO_CYCLIC_TF_TXT1,
   DRGLIF_DO_CYCLIC_TF_TXT2,
   DRGLIF_DO_CYCLIC_TF_TXT3,
   DRGLIF_DO_CYCLIC_TF_TXT4,
   DRGODBIF_GET_CHAR_META_DATA,
   GET_TRANSF_RECT,
   ROT_TEXTURE_MAPPING,
   DRGLIF_START_PRERENDER,
   DRGLIF_FREE_VIEW_RESOURCES,
   DRGLDISP_HAS_PENDING_RENDERING_CMDS,
   DRGLDISP_SWITCH_DISP_IMG,
   DRGLDISP_SET_TEMP_FRAMEBUFFER,
   DRGLDISP_DO_CYCLIC,
   PARSING_DONE_TO_START_GPU,
   WAIT_FOR_GPU,
   DRGLVID_SET_VIDEO_PARAMS,
   DRGLVID_SET_VIDEO_CAPTURE_ENABLE,
   DRWIDIF_GET_CYCLIC_TIME,
   DRWDLOMGR_INIT,
   INC_WIDGET_ITERATOR_INNER_LOOP,
   DRWDIF_UPDATEDISPLAY1,
   DRWDIF_UPDATEDISPLAY2,
   DRWDIF_UPDATEDISPLAY3,
   MAX_PROFILING_ID
} dr_ProfilingIds;



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

   /* Time measure functions */
   Public void dr_profiling_begin(dr_ProfilingIds id);
   Public void dr_profiling_end(dr_ProfilingIds id);
   Public U32 dr_profiling_getMaxTime(dr_ProfilingIds id);
   Public U32 dr_profiling_getMeasuredTime(void);
   Public void dr_profiling_resetId(dr_ProfilingIds id);
   Public void dr_profiling_reset(void);
   Public void dr_profiling_getDataRange(dr_ProfilingData* data, dr_ProfilingIds from, dr_ProfilingIds to);
   Public void dr_profiling_getData(dr_ProfilingData* data, dr_ProfilingIds measure_point);
   Public const U8* dr_profiling_getIdName(dr_ProfilingIds id);


   #define dr_profiling_BEGIN(id)    dr_profiling_begin(id)
   #define dr_profiling_END(id)      dr_profiling_end(id)
   #define dr_profiling_GET_MAX_TIME(id) dr_profiling_getMaxTime(id)
   #define dr_profiling_GET_MEASURED_TIME() dr_profiling_getMeasuredTime()
   #define dr_profiling_RESET_ID(id) dr_profiling_resetId(id)
   #define dr_profiling_RESET()      dr_profiling_reset()
   #define dr_profiling_GET_DATA_RANGE(data, from, to) dr_profiling_getDataRange(data, from, to)
   #define dr_profiling_GET_DATA(data, id) dr_profiling_getData(data, id)
   #define dr_profiling_GET_ID_NAME(id) dr_profiling_getIdName(id)

   /* Model analyze functions */
   Public void dr_profiling_clear_widgetlists(void);
   Public void dr_profiling_clear_refresh_widgetlists(void);
   Public void dr_profiling_add_redraw_widget(dr_WidgetHandle wh);
   Public void dr_profiling_add_refresh_widget(dr_WidgetHandle wh);
   Public void dr_profiling_print_widgetlist(void);
   Public const U8* dr_profiling_getWidgetTypeName(dr_WidgetHandle type);

   #define dr_profiling_CLEAR_WIDGETLISTS()     dr_profiling_clear_widgetlists()
   #define dr_profiling_CLEAR_REFRESH_WIDGETLISTS() dr_profiling_clear_refresh_widgetlists()
   #define dr_profiling_ADD_REDRAW_WIDGET(wh)   dr_profiling_add_redraw_widget(wh)
   #define dr_profiling_ADD_REFRESH_WIDGET(wh)  dr_profiling_add_refresh_widget(wh)
   #define dr_profiling_PRINT_WIDGETLIST()      //dr_profiling_print_widgetlist()
   #define dr_profiling_GET_WIDGET_TYPE_NAME(type) dr_profiling_getWidgetTypeName(type)


#else
   #define dr_profiling_BEGIN(id)
   #define dr_profiling_END(id)
   #define dr_profiling_GET_MAX_TIME(id)
   #define dr_profiling_GET_MEASURED_TIME()
   #define dr_profiling_RESET_ID(id)
   #define dr_profiling_RESET()
   #define dr_profiling_GET_DATA_RANGE(data, from, to)
   #define dr_profiling_GET_DATA(data, id)
   #define dr_profiling_GET_ID_NAME(id)

   #define dr_profiling_CLEAR_WIDGETLISTS()
   #define dr_profiling_CLEAR_REFRESH_WIDGETLISTS()
   #define dr_profiling_ADD_REDRAW_WIDGET(wh)
   #define dr_profiling_ADD_REFRESH_WIDGET(wh)
   #define dr_profiling_PRINT_WIDGETLIST()

#endif

#endif /* DR_PROFILING_H_ */
/** \} 
*********************************** End of file **************************************/
