/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                     dr.h                                        ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2019#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup DROMEIF

   \Subject    Drome package internal data type definitions.

   \Reference  None

   \Abstract
         This file contains data type definitions shared among the different
         sub-packages of the Drome MCU source.


   \Version history record: 

   $Id: dr.h 39 2019-06-19 09:10:47Z stoneridge $

   \{ 
***************************************************************************************
*/

#ifndef DR_H
#define DR_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drome.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/* Supported display types, see drapp_DISPLAY_TYPE (add at the end, don't change numbers!) */
#define dr_DISP_TYPE_QVGA_1      0
#define dr_DISP_TYPE_WQVGA_1     1
#define dr_DISP_TYPE_WVGA_1      2
#define dr_DISP_TYPE_WQVGA_2     3
#define dr_DISP_TYPE_WVGA_2      4
#define dr_DISP_TYPE_QVGA_2      5
#define dr_DISP_TYPE_QVGA_3      6
#define dr_DISP_TYPE_VX_WVGA_1   7
#define dr_DISP_TYPE_SONY_WVGA_1 8
#define dr_DISP_TYPE_QWGA_VX     9

/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/

/** Video signal format */
typedef enum
{
   dr_VSIGFMT_AUTO,              /**< Automatic detection */
   dr_VSIGFMT_PAL,               /**< 720x576 pixels, 50fps interlaced */
   dr_VSIGFMT_NTSC               /**< 720x480 pixels, 60fps interlaced */
} dr_VideoSignalFormat;

/** Video deinterlace mode */
typedef enum
{
   dr_VDEINTM_WEAVE,             /**< Weave = merge fields into complete frames */
   dr_VDEINTM_BOB,               /**< BOB = Expand each field to a full frame */
   dr_VDEINTM_1FIELD             /**< 1 field = Expand odd fields, ignore even */
} dr_VideoDeinterlaceMode;

/** Color value, 4 bit/sample, Alpha:Red:Green:Blue (MSB to LSB) */
typedef drome_Colour dr_Colour;
#define  dr_NULL_COLOUR       0

/** Rectangular display coordinate (4:1 resolution) */
typedef union {
   struct {
      /* Little endian! */
      U16 x;   /**< X coordinate, 1/4 pel/pit */
      U16 y;   /**< Y coordinate, 1/4 pel/pit */
   } c;
   U32 yx;
} dr_RectCoord;

/** Polar display coordinate (4:1 resolution) */
typedef struct {
   U16 r;      /**< Radius, 1/4 pel/bit */
   U16 a;      /**< Angle [0..360[ degrees, 1/4 deg/bit */
} dr_PolCoord;

/** DSM handle for dynamic state machines */
typedef U32 dr_DSMHandle;
#define  dr_NULL_DSM          0

/** State handle for states */
typedef U32 dr_StateHandle;
#define  dr_ROOT_STATE        1
#define  dr_NULL_STATE        0

/** Transition handle for transitions */
typedef U32 dr_TransitionHandle;
#define  dr_NULL_TRANSITION   0

/** Action handle for actions */
typedef U32 dr_ActionHandle;
#define  dr_NULL_ACTION       0

/** Change Mediator handle for change Mediators */
typedef U16 dr_ChangeMediatorHandle;
#define  dr_NULL_CHANGE_MEDIATOR       0

/** Expression handle for expressions */
typedef U32 dr_ExpressionHandle;
#define  dr_NULL_EXPRESSION   0

/** View handle for views */
typedef U32 dr_ViewHandle;
#define  dr_NULL_VIEW         0

/** Event handle for events */
typedef U32 dr_EventHandle;
#define dr_EVENT_INITIALIZE   ((dr_EventHandle)-1)
#define dr_EVENT_TICK         ((dr_EventHandle)-2)
#define dr_EVENT_PAN          ((dr_EventHandle)-3)

/** Data pool handles for data pool values */
typedef U16 dr_DPHandle;
#define dr_NULL_DP        0U

/** Widget handle for widgets */
typedef U32 dr_WidgetHandle;
#define dr_NULL_WIDGET        0

typedef U32 dr_TextId;


/** Display orientation
 ** Rotation is counted clockwise from the normal orientation of the physical display,
 ** i.e. with the scan start at the upper left corner.
 ** NOTE: Using define instead of enum to make labels usable in # if statements.
 */
typedef U8 dr_DispOrient;
#define dr_ORIENT_0DEG     0     /**< - Physical top facing up */
#define dr_ORIENT_90DEG    1     /**< - Physical top facing right */
#define dr_ORIENT_180DEG   2     /**< - Physical top facing down */
#define dr_ORIENT_270DEG   3     /**< - Physical top facing left */

/** Unique ID number for each font included in the graphical object database.
 ** This number is a sub-set of the dr_ObjectId ID numbers.
 */
typedef U16 dr_FontId;

/** Unique ID number for each image included in the graphical object database.
 ** This number is a sub-set of the dr_ObjectId ID numbers.
 */
typedef U16 dr_ImageId;

/** Unique ID number for each item included in the graphical object database 
 ** This number includes the non-overlapping ranges of dr_FontId and dr_ImageId.
 */
typedef U16 dr_ObjectId;
#define dr_NULL_OBJECT     0

/** Status return value
 ** Everything from dr_ST_ERROR and up is an error.
 */
typedef enum
{
   dr_ST_OK,            /**< - OK, Done, finnished, accepted, etc. */
   dr_ST_BUSY,          /**< - Busy, repeat request. */
   dr_ST_ERROR,         /**< - General error (depreciated, use specific codes when possible!) */
   dr_ST_BAD_ARG,       /**< - Error in function call arguments. */
   dr_ST_BAD_STATE,     /**< - Not correct state for request. */
   dr_ST_NOT_FOUND,     /**< - Failed to look up item, e.g. DB object. */
   dr_ST_NO_VIDMEM      /**< - Failed to allocate video memory */
} dr_Status;


#define dr_RTL_STR                0xE000   /*< indicates that a text string contains right to left characters,
                                               used when rendering bidi strings. Should be stored in db as first character in such string.*/
/*
**=====================================================================================
** Public data declarations
**=====================================================================================
*/

/*
**=====================================================================================
** Public function prototypes and macros
**=====================================================================================
*/

/* Implementation for these functions can be found in drome.c */
Public void dr_wrapDetectedDromeError( drome_ErrorCode error_code );
Public void dr_wrapSafeToUpdateGlobalProperties(drome_RedrawType redraw_type, Boolean* wait_for_property_update);

#endif /* ifdef DR_H */
/** \} // End DROMEIF */
/********************************** End of file **************************************/
