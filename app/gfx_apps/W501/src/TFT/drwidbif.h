/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                       DRWIDBIF.H                                ***
***                                                                                 ***
***                                      Project drome                              ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup WIDBIF

   \Subject    Drome widget database interface.

   \Reference  n/a

   \Abstract

      This module declares the data types and functions used for exchanging widget
      related data between Widget Drawing functins and the Database Wrapper. See the
      Drome software users guide for details about widget properties and events.
      See drwidbif.c for implementation details.


   \Version history record:

   $Id: drwidbif.h 10 2018-12-03 11:36:39Z stoneridge $

@{
***************************************************************************************
*/
#ifndef DRWIDBIF_H
#define DRWIDBIF_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "dr.h"
#include "drome.h"
#include "drmathif.h"
#include "drmathbboxif.h"


/*
**=====================================================================================
** Widget configuration
**=====================================================================================
*/

#define DR_WIDGET_ENABLED  1U
#define DR_WIDGET_DISABLED 0U

#if defined(drome_CONF_WIDGET_ANIMATION_CONTAINER)
   #define drwidbif_WIDGET_ANIMATION_CONTAINER DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_ANIMATION_CONTAINER DR_WIDGET_DISABLED
#endif

#if defined(drome_CONF_WIDGET_TEXT_LABEL) || defined(drome_CONF_WIDGET_COND_TEXT_LABEL)
   #define drwidbif_WIDGET_TEXT_LABEL DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_TEXT_LABEL DR_WIDGET_DISABLED
#endif


#ifdef drome_CONF_WIDGET_DATA_LABEL
   #define drwidbif_WIDGET_DATA_LABEL DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_DATA_LABEL DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_CONDITIONAL_DATA_LABEL
   #define drwidbif_CONF_WIDGET_CONDITIONAL_DATA_LABEL DR_WIDGET_ENABLED
#else
   #define drwidbif_CONF_WIDGET_CONDITIONAL_DATA_LABEL DR_WIDGET_DISABLED
#endif


#ifdef drome_CONF_WIDGET_LINE
   #define drwidbif_WIDGET_LINE DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_LINE DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_BOX
   #define drwidbif_WIDGET_BOX DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_BOX DR_WIDGET_DISABLED
#endif

#if defined(drome_CONF_WIDGET_IMAGE) || defined(drome_CONF_WIDGET_CONDITIONAL_IMAGE)
   #define drwidbif_WIDGET_IMAGE DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_IMAGE DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_BARGRAPH
   #define drwidbif_WIDGET_BARGRAPH DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_BARGRAPH DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_LIST_CONTAINER
   #define drwidbif_WIDGET_LIST_CONTAINER DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_LIST_CONTAINER DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_EVENT_MAP
   #define drwidbif_WIDGET_EVENT_MAP DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_EVENT_MAP DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_LIST_COL_TEXT
   #if (drwidbif_WIDGET_LIST_CONTAINER == DR_WIDGET_ENABLED)
      #define drwidbif_WIDGET_LIST_COL_TEXT DR_WIDGET_ENABLED
   #else
      #error "Can't have LIST_COL_TEXT widget without LIST_CONTAINER widget!"
   #endif
#else
   #define drwidbif_WIDGET_LIST_COL_TEXT DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_LIST_COL_IMAGE
   #if (drwidbif_WIDGET_LIST_CONTAINER == DR_WIDGET_ENABLED)
      #define drwidbif_WIDGET_LIST_COL_IMAGE DR_WIDGET_ENABLED
   #else
      #error "Can't have LIST_COL_IMAGE widget without LIST_CONTAINER widget!"
   #endif
#else
   #define drwidbif_WIDGET_LIST_COL_IMAGE DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_LIST_COL_CONTACT
   #if (drwidbif_WIDGET_LIST_CONTAINER == DR_WIDGET_ENABLED)
      #define drwidbif_WIDGET_LIST_COL_CONTACT DR_WIDGET_ENABLED
   #else
      #error "Can't have LIST_COL_CONTACT widget without LIST_CONTAINER widget!"
   #endif
#else
   #define drwidbif_WIDGET_LIST_COL_CONTACT DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_LIST_COL_FLEXITEXT
   #if (drwidbif_WIDGET_LIST_CONTAINER == DR_WIDGET_ENABLED)
      #define drwidbif_WIDGET_LIST_COL_FLEXITEXT DR_WIDGET_ENABLED
   #else
      #error "Can't have LIST_COL_FLEXITEXT widget without LIST_CONTAINER widget!"
   #endif
#else
   #define drwidbif_WIDGET_LIST_COL_FLEXITEXT DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_SCROLLBAR
   #define drwidbif_WIDGET_SCROLLBAR DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_SCROLLBAR DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_COMPLEX_TELLTALE
   #define drwidbif_WIDGET_COMPLEX_TELLTALE DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_COMPLEX_TELLTALE DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_TIME_EVENT
   #define drwidbif_WIDGET_TIME_EVENT DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_TIME_EVENT DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_SCROLLING_TEXT
   #define drwidbif_WIDGET_SCROLLING_TEXT DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_SCROLLING_TEXT DR_WIDGET_DISABLED
#endif

#ifdef drome_CONF_WIDGET_SIMPLE_POINTER
   #define drwidbif_WIDGET_SIMPLE_POINTER DR_WIDGET_ENABLED
#else
   #define drwidbif_WIDGET_SIMPLE_POINTER DR_WIDGET_DISABLED
#endif

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/* Special values used for action event slot index, see also the applicable widgets below */
#define drwidbif_EVIX_TICK          0xFD
#define drwidbif_EVIX_INITIALIZE    0xFE
#define drwidbif_EVIX_NOT_FOUND     0xFF

/** Maximum view nesting depth. \n
 ** This is the maximum number of nested containers and references that is allowed for
 ** a single view, including the base view container. */
#define drwidbif_MAX_NESTING_DEPTH  30U

/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/

typedef U32 drwidbif_TextId;
typedef U16 drwidbif_TextListId;
typedef U16 drwidbif_ImageListId;
typedef U16 drwidbif_U16ListId;
typedef U16 drwidbif_PropertyListId;

/** Horizontal alignment specification */
typedef enum
{
  DR_ALIGN_LEFT=0U,
  DR_ALIGN_CENTER=1U,
  DR_ALIGN_RIGHT=2U
} drwidbif_XAlign;

/** Vertical alignment specification */
typedef enum
{
  DR_ALIGN_TOP=0U,
  DR_ALIGN_MIDDLE=1U,
  DR_ALIGN_BOTTOM=2U
} drwidbif_YAlign;

/** Physical direction specification */
typedef enum
{
  DR_DIR_LEFT_TO_RIGHT=0U,
  DR_DIR_RIGHT_TO_LEFT=1U,
  DR_DIR_BOTTOM_TO_TOP=2U,
  DR_DIR_TOP_TO_BOTTOM=3U
} drwidbif_Direction;

/** Time event widget operating mode */
typedef enum
{
  DR_LONG_PRESS=0U,
  DR_REPEAT=1U,
  DR_TIMEOUT=2U
} drwidbif_Mode;

/** Animation widgets fill options*/
typedef enum
{
	DR_FREEZE=0U,
	DR_REMOVE=1U
}drwidbif_Animation_Fill;

/** Animation widgets restart options*/
typedef enum
{
	DR_ALWAYS=0U,
	DR_WHEN_NOT_ACTIVE=1U
}drwidbif_Animation_Restart;

/** Panning directions*/
typedef enum
{
	DR_HORIZONTAL=0U,
	DR_VERTICAL=1U,
	DR_BOTH=2U
}drwidbif_Panning_Direction;

typedef enum
{
	DR_RUN_ONCE = 0U,
	DR_RUN_CONTINUOUS = 1U
}drwidbif_Sprite_Mode;

/** List widget scrolling types */
typedef enum
{
   DR_ZERO_OFFSET = 0,
   DR_ONE_OFFSET = 1U,
   DR_PAGE_JUMP = 2U,
   DR_HIGHLIGHT_ONLY = 3U
}drwidbif_Scroll_Type;

typedef enum
{
   DR_NO_ROTATION = 0U,
   DR_ROTATE_CLIP_MIN = 1U,
   DR_ROTATE_CLIP_MAX = 2U
}drwidbif_Circular_Bargraph_Clip_Mode;

typedef enum
{
   DR_X_ALIGN_COLS_LEFT = 0U,
   DR_X_ALIGN_COLS_CENTER = 1U,
   DR_X_ALIGN_COLS_RIGHT = 2U,
   DR_X_ALIGN_COLS_EVEN_SPACES = 3U
}drwidbif_Layout_Manager_X_Align_Cols;

typedef enum
{
   DR_Y_ALIGN_ROWS_TOP = 0U,
   DR_Y_ALIGN_ROWS_CENTER = 1U,
   DR_Y_ALIGN_ROWS_BOTTOM = 2U,
   DR_Y_ALIGN_ROWS_EVEN_SPACES = 3U
}drwidbif_Layout_Manager_Y_Align_Rows;

/*======= W i d g e t  B a s i c  P r o p e r t i e s ===============================*/

/* Widget common/inherited flags */
#define DRWIDATA_BASICPROP_FLAG_ACTIVATED            0x01
#define DRWIDATA_BASICPROP_FLAG_VISIBLE              0x02
#define DRWIDATA_BASICPROP_FLAG_ENABLED              0x04
#define DRWIDATA_BASICPROP_FLAG_DYNAMIC              0x08   /* Widget uses dynamic properties */
#define DRWIDATA_BASICPROP_FLAG_ONLY_STATIC_GRAPHICS 0x10   /* No need to update widget with a extra high frequency */
#define DRWIDATA_BASICPROP_FLAG_POST_RENDER          0x20   /* The drawing of this container should be a post-render */

typedef struct
{
   //U16 height; - Specific prop (if needed)
   //U16 width; - Specific prop (if needed)
   U16 x;
   U16 y;
   //drwidbif_XAlign x_align; - Not supported
   //drwidbif_YAlign y_align; - Not supported
   drmathif_Matrix transform;
   U8 basicprop_flags;
} drwidbif_BasicProps;


/*======= A n i m a t i o n  C o n t a i n e r  W i d g e t =========================*/

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour background_colour;
} drwidbif_AnimationContainerProps; //TODO: remove, old widget no longer used

/*======= G e n e r a l  A n i m a t i o n  C o n t a i n e r  E v e n t s ==============*/
#define drwidbif_EVIX_ANIMATION_EVENT_ACTIVATE        0
#define drwidbif_EVIX_ANIMATON_EVENT_DEACTIVATE       1
#define drwidbif_EVIX_ANIMATON_EVENT_REVERSE          2

typedef struct
{
	dr_EventHandle activation_event;
	dr_EventHandle deactivation_event;
	dr_EventHandle reverse_event;
}drwidbif_AnimationInEvents;

typedef struct
{
	dr_EventHandle end_event;
	dr_EventHandle reverse_end_event;
}drwidbif_AnimationOutEvents;

typedef struct
{
	drwidbif_AnimationInEvents 	action_events;
	drwidbif_AnimationOutEvents 	reaction_events;
}drwidbif_AnimationController;

typedef struct
{
   U32 progress_curve;
   drwidbif_Animation_Fill fill;
   drwidbif_Animation_Restart restart;
   U16 duration;
   Boolean init_at_end_value;
}drwidbif_BaseAnimContainerProps;

/*======= C o l o r  A n i m a t i o n  C o n t a i n e r  W i d g e t ==============*/

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour color;
   dr_Colour color_to;
   U16 animation_id;
   drwidbif_BaseAnimContainerProps animation;
}drwidbif_ColorAnimationProps;

/*======= O p a c i t y  A n i m a t i o n  C o n t a i n e r  W i d g e t ==========*/
typedef struct
{
   U16 width;
   U16 height;
   U16 opacity;
   U16 opacity_from;
   U16 opacity_to;
   U16 animation_id;
   drwidbif_BaseAnimContainerProps animation;
}drwidbif_OpacityAnimationProps;

/*======= P a n n i n g  A n i m a t i o n  C o n t a i n e r  W i d g e t ==============*/
typedef struct
{
   U16 width;
   U16 height;
   drwidbif_Panning_Direction direction;
}drwidbif_PanningAnimationProps;

/*======= R o t a t i o n  A n i m a t i o n  C o n t a i n e r  W i d g e t ========*/
typedef struct
{
   U16 width;
   U16 height;
   U16 rotation_angle;
   U16 rotation_angle_to;
   U16 rotation_origo_x;
   U16 rotation_origo_y;
   U16 animation_id;
   drwidbif_BaseAnimContainerProps animation;
}drwidbif_RotationAnimationProps;

/*======= S c a l e  A n i m a t i o n  C o n t a i n e r  W i d g e t ==============*/
typedef struct
{
   U16 width;
   U16 height;
   U16 scale_origo_x;
   U16 scale_origo_y;
   U16 scale_x;
   U16 scale_x_to;
   U16 scale_y;
   U16 scale_y_to;
   U16 animation_id;
   drwidbif_BaseAnimContainerProps animation;
}drwidbif_ScaleAnimationProps;

/*======= T r a n s l a t e  A n i m a t i o n  C o n t a i n e r  W i d g e t ==============*/
typedef struct
{
   U16 width;
   U16 height;
   U16 x_from;
   U16 x_to;
   U16 y_from;
   U16 y_to;
   U16 animation_id;
   drwidbif_BaseAnimContainerProps animation;
}drwidbif_TranslateAnimationProps;

/*======= S p r i t e  A n i m a t i o n  W i d g e t =========================*/
typedef struct
{
	dr_EventHandle activation_event;
	dr_EventHandle deactivation_event;
}drwidbif_SpriteInEvents;

typedef struct
{
	dr_EventHandle end_event;
}drwidbif_SpriteOutEvents;

typedef struct
{
	drwidbif_SpriteInEvents 	action_events;
	drwidbif_SpriteOutEvents 	reaction_events;
}drwidbif_SpriteController;

typedef struct
{
	drwidbif_Animation_Fill fill;
	drwidbif_Animation_Restart restart;
	U16 frame_rate;
   U16 sprite_id;
   drwidbif_Sprite_Mode mode;
   U16 animation_id;
} drwidbif_SpriteAnimationProps;

/*======= V i r t u a l  G a u g e  C o n t a i n e r  W i d g e t =======*/
#define drwidbif_EVIX_VIRTUAL_GAUGE_RESET    0U

typedef struct
{
   U16 width;
   U16 height;
   dr_DPHandle angle_hdl;
   S16 max_angle;
   S16 min_angle;
   S16 value;
   S16 max_value;
   S16 min_value;
   S16 velocity;
   S16 acceleration;
} drwidbif_VirtualGaugeContainerProps;

/*======= V i r t u a l  G a u g e  N e e d l e   W i d g e t =======*/
typedef struct
{
   U16 width;
   U16 height;
   dr_DPHandle angle_hdl;
   U16 angle_offset;
   U16 rotation_origo_x;
   U16 rotation_origo_y;
} drwidbif_VirtualGaugeNeedleProps;

/*======= V i r t u a l  G a u g e  C i r c u l a r   B a r g r a p h   W i d g e t =======*/
typedef struct
{
   U16 width;
   U16 height;
   dr_DPHandle angle_hdl;
   U16 angle_min;
   U16 angle_offset;
   U16 rotation_origo_x;
   U16 rotation_origo_y;
   U16 radius_min;
   U16 radius_max;
   U16 image_pos_x;
   U16 image_pos_y;
   dr_Colour fill_color;
   dr_ImageId image;
   drwidbif_Circular_Bargraph_Clip_Mode clip_mode;
} drwidbif_VirtualGaugeCircularBargraphProps;

/*======= L a y o u t  M a n a g e r  W i d g e t =======*/
typedef struct
{
   U16 width;
   U16 height;
   U16 columnCount;
   U16 columnLeft;
   U16 columnWidth;
   U16 rowCount;
   U16 rowTop;
   U16 rowHeight;
   drwidbif_Layout_Manager_X_Align_Cols xAlignColumns;
   drwidbif_Layout_Manager_Y_Align_Rows yAlignRows;
} drwidbif_LayoutManagerProps;

/*======= V i d e o  W i d g e t ====================================================*/

typedef struct
{
  U16 width;
  U16 height;
  S8 brightness;
  U8 channel;
  S8 colour_red_scaling;
  S8 colour_green_scaling;
  S8 colour_blue_scaling;
  S8 colour_saturation;
  S8 contrast;
  dr_VideoDeinterlaceMode deinterlace_mode;
  S16 image_shift_horizontal;
  S16 image_shift_vertical;
  dr_Colour key_colour;
  U16 rotation;
  U8 scaling;
} drwidbif_VideoWidgetProps;


/*======= B a r g r a p h  W i d g e t ==============================================*/

typedef struct
{
   U16 width;
   U16 height;
   U32 value;
   U32 max_value;
   U16 range_high;
   U16 range_low;
   dr_Colour  colour_fill_low;
   dr_Colour  colour_fill_high;
   dr_ImageId img_fill_low;
   dr_ImageId img_fill_high;
   U16        img_blend_with_colour;
   U16        colour_fill_hyst_low;
   U16        colour_fill_hyst_high;
   U16        colour_fill_is_high_dp_hdl;
   dr_Colour  background;
   dr_Colour  border_colour;
   dr_ImageId img_background;
   dr_ImageId img_foreground;
   U8 bar_ofs_left;
   U8 bar_ofs_top;
   U8 bar_ofs_right;
   U8 bar_ofs_bottom;
   U8 border_thickness;
   drwidbif_Direction direction;
} drwidbif_BargraphProps;

/*======= B o x  W i d g e t ========================================================*/

typedef struct
{
  U16 width;
  U16 height;
  U16       thickness;
  dr_Colour background;
  dr_Colour foreground;
  Boolean   use_background;
} drwidbif_BoxProps;

/*======= D a t a  L a b e l  W i d g e t ===========================================*/

#define DRWIDATA_DATALABEL_FLAG_APPEND  0x01
#define DRWIDATA_DATALABEL_FLAG_ROUND   0x02

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour bg_colour_enabled;
   dr_Colour bg_colour_disabled;
   U8        datalabel_flags;       // See mask definitions above.
   U8        digits_fraction;
   U8        digits_integer;
   U8        filter;
   dr_FontId font;
   drwidbif_TextId text;
   dr_Colour text_color;
   dr_Colour disabled_text_color;
   U16       value_dp_hdl;
   drwidbif_XAlign x_text_align;
   drwidbif_YAlign y_text_align;
   Boolean  trunc_with_dots;
}  drwidbif_DataLabelProps;

/*======= I m a g e  W i d g e t ====================================================*/

typedef struct
{
  dr_ImageId image;
  dr_Colour col;
  Boolean transparent;
} drwidbif_ImageProps;

/*======= T e x t  L a b e l  W i d g e t ===========================================*/

typedef struct
{
   U16 width;
   U16 height;
   dr_FontId font;
   dr_Colour disabled_text_color;
   dr_Colour text_color;
   drwidbif_XAlign x_text_align;
   drwidbif_YAlign y_text_align;
   drwidbif_TextId text;
   Boolean trunc_with_dots;
   Boolean right_to_left_language;
   Boolean carret_visible;
   Boolean forceLinebreak;
   Boolean forceLinebreakWithHyphen;
} drwidbif_TextLabelProps;

/*======= V i e w  W i d g e t ======================================================*/

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour color;
}  drwidbif_ViewProps;

/*======= L i n e  W i d g e t ======================================================*/

typedef struct
{
  dr_Colour foreground;
  U8 thickness;
  U16 x1;
  U16 y1;
  U16 x2;
  U16 y2;
} drwidbif_LineProps;

/*======= L i s t  C o n t a i n e r  W i d g e t ===================================*/

#define drwidbif_LIST_MAX_ENTRIES    32   /* Max numb. of static lines in a list */
#if drwidbif_LIST_MAX_ENTRIES > 32
   #error "drwidbif_LIST_MAX_ENTRIES bitmasks doesn't fit into an U32!"
#endif

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour background;
   Boolean dynamic_rows_first;
   dr_Colour highlight_colour;
   U16 highlight_extra_height;
   dr_ImageId highlight_image;
   U16 highlight_offset_left;
   dr_ImageId img_bottom;
   dr_ImageId img_left;
   dr_ImageId img_right;
   dr_ImageId img_top;
   dr_ImageId list_end_marker_image;
   U8  list_ofs_bottom;
   U8  list_ofs_left;
   U8  list_ofs_right;
   U8  list_ofs_top;
   U8  scroll_type;
   U32 row_enabled;
   U16 row_highlight_dp_hdl;
   U16 row_count_static;
   U16 row_count_dynamic;
   U16 row_count_visible_dp_hdl;
   U32 row_visible;
   U16 row_simultaneously;
   U16 row_top_dp_hdl;
#ifdef drome_CONF_USE_ANIMATED_LISTS
   U16 animation_id;
#endif
   drwidbif_BaseAnimContainerProps animation;

} drwidbif_ListContainerProps;

#define drwidbif_EVIX_LIST_CONT_HL_UP     0
#define drwidbif_EVIX_LIST_CONT_HL_DOWN   1
#define drwidbif_EVIX_LIST_CONT_HL_TOP    2
#define drwidbif_EVIX_LIST_CONT_HL_BOTTOM 3
#define drwidbif_EVIX_LIST_CONT_HL_SELECT 4
#define drwidbif_EVIX_LIST_CONT_COUNT     5

typedef struct
{
  drwidbif_U16ListId selected_row_list;
  dr_EventHandle selected_row_dynamic;
  dr_EventHandle new_row_highlighted;
} drwidbif_ListContainerReactionEvents;

typedef struct
{
   Boolean dynamic_rows_first;
   Boolean has_end_marker_image;
   U16 row_highlight_dp_hdl;
   U16 row_count_dynamic;
   U16 row_count_static;
   U16 row_count_visible;
   U16 row_simultaneously;
   U16 row_top;
   U32 row_visible;
   Boolean wrap_around;
   drwidbif_ListContainerReactionEvents reaction_events;
} drwidbif_ListContainerController;

/*======= L i s t  C o l u m n  T e x t  W i d g e t ================================*/

typedef struct
{
   U16 width;
   U16 height;
   dr_FontId font;
   drwidbif_TextListId text_static;
   U16 text_dynamic_dp_hdl;
   drwidbif_XAlign text_align;
   dr_Colour text_colour_disabled;
   dr_Colour text_colour_enabled;
   dr_Colour text_colour_disabled_highlight;
   dr_Colour text_colour_enabled_highlight;
   Boolean trunc_with_dots;
   Boolean right_to_left_language;
} drwidbif_ListColumnTextProps;

/*======= L i s t  C o l u m n  C o n t a c t  W i d g e t ==========================*/

typedef struct
{
   U16 width;
   dr_FontId text_font;
   dr_FontId sub_text_font;
   U16 text_dp_hdl;
   U16 sub_text_dp_hdl;
   drwidbif_XAlign text_align;
   drwidbif_XAlign sub_text_align;
   dr_Colour text_colour;
   dr_Colour sub_text_colour;
   dr_Colour text_colour_highlight;
   U16 left_image_enable_dp_hdl;
   dr_ImageId left_image;
   U16 right_image_enable_dp_hdl;
   dr_ImageId right_image;
   U16 type_image_index_dp_hdl;
   drwidbif_ImageListId type_images;
   Boolean trunc_with_dots;
   Boolean right_to_left_language;
} drwidbif_ListColumnContactProps;

/*======= L i s t  C o l u m n  I m a g e  W i d g e t ==============================*/

typedef struct
{
   U16 width;
   U16 height;
   drwidbif_ImageListId image_list;
   U16 image_dp_hdl;
   U8 image_mode;
   Boolean transparent;
} drwidbif_ListColumnImageProps;

/*======= L i s t  C o l u m n  F l e x i  T e x t  W i d g e t ================================*/

typedef struct
{
   U16 width;
   dr_FontId font;
   drwidbif_TextListId text_internal;
   U16 text_external_dp_hdl;
   U16 row_source_dp_hdl;
   U16 index_text_internal_dp_hdl;
   drwidbif_XAlign text_align;
   dr_Colour text_colour;
   dr_Colour text_colour_highlight;
   Boolean trunc_with_dots;
   Boolean right_to_left_language;
} drwidbif_ListColumnFlexiTextProps;

/*======= S c r o l l b a r  W i d g e t ============================================*/

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour  background_colour;
   dr_ImageId background_image;
   U16 element_current;
   U16 element_count;
   U16 element_simultaneously;
   dr_Colour  slider_colour;
   dr_ImageId slider_image;
   U16 slider_width;
   U16 slider_length;
   Boolean vertical;
} drwidbif_ScrollbarProps;

/*======= C o m p l e x  T e l l t a l e  W i d g e t ===============================*/

typedef struct
{
   U16 width;
   U16 height;
   drwidbif_U16ListId tt_id_list;
   drwidbif_U16ListId tt_position_list;
   drwidbif_U16ListId tt_priority_list;
   drwidbif_ImageListId tt_symbol_list;
   U16 tt_count;
   dr_ImageId empty_slot_image;
   U8 column_count;
   U8 row_count;
   U8 layout_mode;
   U16 tt_activity_range1;
   U16 tt_activity_range2;
   U16 tt_activity_range2_start;
} drwidbif_ComplexTelltaleProps;

/*======= E v e n t  M a p  W i d g e t =============================================*/

/* Widget has no visible properties */

#define drwidbif_EVIX_EVENT_MAP_IN        0

typedef struct
{
   dr_EventHandle in_event;
   dr_EventHandle out_event;
} drwidbif_EventMapController;


/*======= T i m e  E v e n t  W i d g e t =============================================*/

#define drwidbif_EVIX_TIME_EVENT_START        0
#define drwidbif_EVIX_TIME_EVENT_STOP         1

typedef struct
{
   U16                           mode;
   U16                           timeDelay;
   U16                           timeRepeat;
   U16                           timeStore;
   Boolean                       resetWhenChangingView;

   dr_EventHandle                event_start;   // Event_Start
   dr_EventHandle                event_stop;    // Event_Stop

   dr_EventHandle                event_initial;   // Event_Initial
   dr_EventHandle                event_delay;     // Event_Delay
   dr_EventHandle                event_repeat;    // Event_Repeat
} drwidbif_TimeEventController;

/*======= S c r o l l i n g  T e x t  W i d g e t ===================================*/

#define drwidbif_EVIX_SCRTXT_UP           0
#define drwidbif_EVIX_SCRTXT_DOWN         1
#define drwidbif_EVIX_SCRTXT_PAGE_UP      2
#define drwidbif_EVIX_SCRTXT_PAGE_DOWN    3
#define drwidbif_EVIX_SCRTXT_HOME         4
#define drwidbif_EVIX_SCRTXT_END          5
#define drwidbif_EVIX_SCRTXT_COUNT        6

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour         background_colour;
   U16               end_of_list_dp_hdl;
   dr_FontId         font;
   Boolean           overlap;
   U16               row_count_dp_hdl;
   U16               row_simultaneously_dp_hdl;
   U16               row_top_dp_hdl;
   drwidbif_TextId   text;
   dr_Colour         text_colour_disabled;
   dr_Colour         text_colour_enabled;
   drwidbif_XAlign   x_text_align;
   Boolean           trunc_with_dots;
   Boolean           forceLinebreak;
   Boolean           forceLinebreakWithHyphen;
   Boolean           right_to_left_language;
   Boolean           carret_visible;
} drwidbif_ScrollingTextProps;

typedef struct
{
   Boolean           overlap;
   U16               row_count_dp_hdl;
   U16               row_simultaneously_dp_hdl;
   U16               row_top_dp_hdl;
   dr_EventHandle    reaction_event_end_of_list;
} drwidbif_ScrollingTextController;

/*======= S i m p l e   P o i n t e r  W i d g e t ===================================*/

typedef struct
{
   U16 width;
   U16 height;
   dr_Colour         background_colour;
   dr_ImageId        background_image;
   U16               deflection_angle;
   U16               direction;
   U16               point_count;
   dr_Colour         pointer_colour;
   drwidbif_U16ListId polygon_list;
   U16               x_origo;
   U16               y_origo;
   U16               zero_angle;

} drwidbif_SimplePointerProps;
/*======= W i d g e t  T y p e s ====================================================*/

typedef enum
{
  DR_INVALID_WIDGET,
  DR_WIDGET_ANIMATION_CONTAINER,
  DR_WIDGET_BARGRAPH,
  DR_WIDGET_BOX,
  DR_WIDGET_COLOR_ANIMATION_CONTAINER,
  DR_WIDGET_CONTAINER,
  DR_WIDGET_CONDITIONAL_IMAGE,
  DR_WIDGET_CONDITIONAL_REFERENCE,
  DR_WIDGET_CONDITIONAL_TEXT,
  DR_WIDGET_DATA_LABEL,
  DR_WIDGET_EVENT_MAP,
  DR_WIDGET_IMAGE,
  DR_WIDGET_LINE,
  DR_WIDGET_LIST_COL_IMAGE,
  DR_WIDGET_LIST_COL_TEXT,
  DR_WIDGET_LIST_COL_CONTACT,
  DR_WIDGET_LIST_COL_FLEXITEXT,
  DR_WIDGET_LIST_CONTAINER,
  DR_WIDGET_OPACITY_ANIMATION_CONTAINER,
  DR_WIDGET_PANNING_ANIMATION_CONTAINER,
  DR_WIDGET_REFERENCE,
  DR_WIDGET_ROTATION_ANIMATION_CONTAINER,
  DR_WIDGET_SCALE_ANIMATION_CONTAINER,
  DR_WIDGET_SCROLLBAR,
  DR_WIDGET_SYNC,
  DR_WIDGET_TEXT_LABEL,
  DR_WIDGET_VIEW,
  DR_WIDGET_TIME_EVENT,
  DR_WIDGET_TRANSLATE_ANIMATION_CONTAINER,
  DR_WIDGET_VIDEO,
  DR_WIDGET_COMPLEX_TELLTALE,
  DR_WIDGET_SCROLLING_TEXT,
  DR_WIDGET_SIMPLE_POINTER,
  DR_WIDGET_SPRITE_ANIMATION,
  DR_WIDGET_CONDITIONAL_DATA_LABEL,
  DR_WIDGET_VIRTUAL_GAUGE_CONTAINER,
  DR_WIDGET_VIRTUAL_GAUGE_NEEDLE,
  DR_WIDGET_VIRTUAL_GAUGE_CIRCULAR_BARGRAPH,
  DR_WIDGET_LAYOUT_MANAGER,
  DR_NUM_WIDGETS,
  DR_WIDGET_END_VIEW_MARK
} drwidbif_WidgetType;
 
   
/*======= W i d g e t  I t e r a t o r ==============================================*/

/** The widget iterator is usedas a handle by a client that want's to access the
 ** widgets of a view. The accumulated x/y offset values and the basic property
 ** flags include all container/reference widgets in the path, even the current
 ** widget if that is a (visible) container. */
typedef struct
{
  /* This actual data is only used internally by drwidbif.c */
  U32 widget_index;                                ///< Index of "current" widget
  Boolean is_in_active_animation_container;        ///< "Current" widget is in an active animation container
  drmathbbox_BBox aabb;                            ///< The axis aligned bounding box of the current widget
} drwidbif_WidgetIterator;

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

Public void drwidbif_init( const void * db_ptr, const void * text_ptr );

#ifdef drome_CONF_USE_LANGUAGE_PACKS
Public void drwidbif_initLanguagePack( const void * db_ptr);
Public const U8* drwidbif_getLangPackSrePartNo(void);
Public const U8* drwidbif_getLangPackCustomerPartNo(void);
#endif

Public U16 drwidbif_getNumberOfLanguages( void );
Public drome_LanguageId drwidbif_getLanguageCode( U16 index );
Public Boolean drwidbif_setLanguage( drome_LanguageId language );

#ifdef drome_CONF_USE_DYNAMIC_VARIANTS
Public U16 drwidbif_getNumberOfVariants( void );
Public drome_VariantId drwidbif_getVariantCode( U16 index );
Public Boolean drwidbif_setVariant( drome_VariantId variant );
#endif

Public Boolean drwidbif_getViewWithKey( U32 key, dr_ViewHandle *view_hdl_ptr );

Public void drwidbif_collectEventHandlingWidgets(dr_ViewHandle view_hdl);
Public void drwidbif_invalidateWidgetIterator( drwidbif_WidgetIterator* iterator_ptr );
Public Boolean drwidbif_initWidgetIterator(
   dr_ViewHandle view_hdl, drwidbif_WidgetIterator* iterator_ptr
);
Public Boolean drwidbif_getCurrentWidget(
   const drwidbif_WidgetIterator* iterator_ptr, dr_WidgetHandle* widget_hdl_ptr
);
Public Boolean drwidbif_getNextWidget(
   drwidbif_WidgetIterator* iterator_ptr,
   dr_WidgetHandle*  widget_hdl_ptr
);
Public Boolean drwidbif_getInheritedBasicProperties(
   const drwidbif_WidgetIterator* iterator_ptr, drwidbif_BasicProps* b_props_ptr
);

Public Boolean drwidbif_getWidgetBasicProperties(
   dr_WidgetHandle widget_hdl, drwidbif_BasicProps* b_props_ptr
);
Public drwidbif_WidgetType drwidbif_getWidgetType( dr_WidgetHandle widget_hdl );
Public dr_WidgetHandle drwidbif_getParentWidget(dr_WidgetHandle widget_hdl);
Public U32 drwidbif_getNumberOfVisibleChildren(dr_WidgetHandle widget_hdl, U32 stop_count_limit, dr_WidgetHandle* next_widget_hdl_ptr);
Public dr_WidgetHandle drwidbif_getWidget(U16 animation_id);
Public U16 drwidbif_getNoOfAnimations(void);
Public Boolean drwidbif_getSpecificProperties(
   dr_WidgetHandle widget_hdl, void* data_ptr, U32 size
);

#ifdef drome_CONF_USE_SM_AND_EVENTS
Public Boolean drwidbif_getControllerData(
   dr_WidgetHandle widget_hdl, void* data_ptr, U32 size 
);
Public U8 drwidbif_findActionEvent(
   dr_WidgetHandle widget_hdl, dr_EventHandle event_hdl
);
#endif

#ifdef CUNIT_TEST
Public U16 wrap_convertUTF8ToUTF16(
   const UTF8* utf8_in_ptr, U16 offset, U16* utf16_out_ptr, U16 utf16_buf_len
);
#endif

Public U16 drwidbif_getText( dr_TextId id, U16 * str_ptr, U16 max_len, U16 offset, Boolean * is_rtl );
Public U16 drwidbif_getIndexedText(
   drwidbif_TextListId id, U16 index, U16 * str_ptr, U16 max_len, Boolean * is_rtl
);
Public dr_ImageId drwidbif_getIndexedImage( drwidbif_ImageListId id, U16 index );
Public const U16 * drwidbif_getU16List( drwidbif_U16ListId id );

Public U32 drwidbif_getViewKey( dr_ViewHandle view_hdl );
#endif
/**
@} // End WIDBIF
********************************** End of file **************************************/
