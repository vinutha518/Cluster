/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                  drconf.h                                       ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***                     Copyright (C) Stoneridge Electronics AB                     ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup DROMEIF 

  \Subject    Automatically generated configuration file 

  \Reference  None

  \Abstract 

  The events, data definitions and other options are generated from the configuration file.
  Please do not modify this file! 
  

  \Version history record: 

  $Id:$

\{
***************************************************************************************
*/

#ifndef DRCONF_H
#define DRCONF_H


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

/* CRC32-code of config options at compile time */
#define drome_CONFIG_CRC32 0xE67A1836UL

/*Size, in bytes, needed by the internal datapool*/
#define drome_MAX_INTERNAL_DATAPOOL_SIZE 0x1F4U

/*Size, in bytes, needed by the non-volatile part of the internal datapool*/
#define drome_MAX_INTERNAL_NV_DATA_SIZE 0x50U

/*Maximum number of history states allowed*/
#define drome_MAX_NO_OF_HISTORY_STATES 0xAU

/*Maximum number of change mediators allowed*/
#define drome_MAX_NO_OF_CHANGE_MEDIATORS 0x5U

/*Maximum number of animation widgets allowed*/
#define drome_MAX_NO_OF_ANIMATION_WIDGETS 0x64U

/* Features in use */
#define drome_CONF_USE_SM_AND_EVENTS 	

#define drome_CONF_USE_DYNAMIC_VARIANTS 	

#define drome_CONF_USE_ANIMATED_LISTS 	

/* Display properties */
#define drome_DISP_WIDTH	240U
#define drome_DISP_HEIGHT	320U
#define drome_DISP_ORIENT	dr_ORIENT_0DEG
#define drome_GLIB_VARIANT drome_GLIB_DHD


/* widgets in use */
#define drome_CONF_WIDGET_REFERENCE
#define drome_CONF_WIDGET_ANIMATION_COLOR_CONTAINER
#define drome_CONF_WIDGET_ANIMATION_OPACITY_CONTAINER
#define drome_CONF_WIDGET_ANIMATION_PANNING_CONTAINER
#define drome_CONF_WIDGET_ANIMATION_ROTATION_CONTAINER
#define drome_CONF_WIDGET_ANIMATION_SCALE_CONTAINER
#define drome_CONF_WIDGET_ANIMATION_TRANSLATE_CONTAINER
#define drome_CONF_WIDGET_SPRITE_ANIMATION
#define drome_CONF_WIDGET_VIRTUAL_GAUGE_CIRCULAR_BARGRAPH
#define drome_CONF_WIDGET_VIRTUAL_GAUGE_CONTAINER
#define drome_CONF_WIDGET_VIRTUAL_GAUGE_NEEDLE
#define drome_CONF_WIDGET_ANIMATION_CONTAINER
#define drome_CONF_WIDGET_BARGRAPH
#define drome_CONF_WIDGET_BOX
#define drome_CONF_WIDGET_CONDITIONAL_IMAGE
#define drome_CONF_WIDGET_COND_TEXT_LABEL
#define drome_CONF_WIDGET_CONDITIONAL_REFERENCE
#define drome_CONF_WIDGET_CONDITIONAL_DATA_LABEL
#define drome_CONF_WIDGET_CONTAINER
#define drome_CONF_WIDGET_EVENT_MAP
#define drome_CONF_WIDGET_IMAGE
#define drome_CONF_WIDGET_TEXT_LABEL
#define drome_CONF_WIDGET_LAYOUT_MANAGER
#define drome_CONF_WIDGET_LINE
#define drome_CONF_WIDGET_LIST_CONTAINER
#define drome_CONF_WIDGET_LIST_COL_CONTACT
#define drome_CONF_WIDGET_LIST_COL_FLEXITEXT
#define drome_CONF_WIDGET_LIST_COL_IMAGE
#define drome_CONF_WIDGET_LIST_COL_TEXT
#define drome_CONF_WIDGET_SCROLLBAR
#define drome_CONF_WIDGET_SCROLLING_TEXT
#define drome_CONF_WIDGET_TIME_EVENT
#define drome_CONF_WIDGET_DATA_LABEL
#define drome_CONF_WIDGET_VIEW

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
